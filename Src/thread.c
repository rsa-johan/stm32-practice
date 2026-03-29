#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/cdefs.h>

#include "thread.h"

/*
 * Simple cooperative / preemptive round-robin scheduler for Cortex-M3.
 *
 * This implementation is intentionally small and self-contained.
 * - createTask() allocates a stack and prepares the initial exception frame.
 * - yield() and SysTick trigger PendSV to perform a context switch.
 * - runScheduler() starts SysTick and switches to the first created task.
 */

static TaskControlBlock g_tasks[MAX_TASKS];
static TaskIndex g_currentTaskIndex;
static TaskControlBlock *g_currentTask;

static uint32_t g_taskStacks[MAX_TASKS][MAX_STACK_WORDS];

static inline void setPSP(uint32_t psp)
{
    __asm volatile("msr psp, %0" :: "r"(psp) : "memory");
}

static inline void __set_CONTROL(uint32_t control)
{
    __asm volatile("msr control, %0" :: "r"(control) : "memory");
}

static inline uint32_t __get_CONTROL(void)
{
    uint32_t result;
    __asm volatile ("mrs %0, control" : "=r" (result) );
    return(result);
}

static inline void dsb(void)
{
    __asm volatile("dsb");
}
static inline void isb(void)
{
    __asm volatile("isb");
}

static inline void __svc_trigger(void)
{
    __asm volatile("SVC #0");
}

static void scheduleNextTask(void) __attribute__((used));
static void scheduleNextTask(void)
{
    if (g_currentTask == NULL) {
        return;
    }

    for (uint32_t i = 0; i < MAX_TASKS; ++i) {
        uint32_t next = (g_currentTaskIndex + i) % MAX_TASKS;
        if (g_tasks[next].active && !g_tasks[next].suspended) {
            g_currentTaskIndex = next;
            g_currentTask = &g_tasks[next];
            return;
        }
    }
}

void yield(void)
{
    /* Request a PendSV exception to perform a context switch. */
    SCB_ICSR = SCB_ICSR_PENDSVSET_Msk;
    dsb();
    isb();
}

__attribute__((interrupt)) void SVC_Handler(void)
{
  __asm volatile(
    ".global SVC_Handler_Main\n"
    "TST lr, #4\n"
    "ITE EQ\n"
    "MRSEQ r0, MSP\n"
    "MRSNE r0, PSP\n"
    "B SVC_Handler_Main\n"
  );
}

void SVC_Handler_Main(unsigned int *svc_args)
{
    __asm volatile(
        "mrs r0, psp\n"
        "stmdb r0!, {r4-r11}\n"
        "ldr r1, =g_currentTask\n"
        "ldr r2, [r1]\n"
        "str r0, [r2]\n"
        "push {lr}\n"
        "bl scheduleNextTask\n"
        "pop {lr}\n"
        "ldr r1, =g_currentTask\n"
        "ldr r2, [r1]\n"
        "ldr r0, [r2]\n"
        "ldmia r0!, {r4-r11}\n"
        "msr psp, r0\n"
        "bx lr\n"
    );
}

__attribute__((interrupt)) void SysTick_Handler(void)
{
    /* Trigger PendSV to perform a context switch on SysTick. */
    SCB_ICSR = SCB_ICSR_PENDSVSET_Msk;
}


void createTask(void (*taskFunction)(void *), const char *name, uint16_t stackSize, void *parameters, uint32_t priority)
{
    (void)name;
    (void)priority;

    for (uint32_t i = 0; i < MAX_TASKS; ++i) {
        if (!g_tasks[i].active) {
            uint32_t words = (stackSize + 3U) / 4U;
            if (words < 64U) {
                words = 64U;
            }
            if (words > MAX_STACK_WORDS) {
                words = MAX_STACK_WORDS;
            }

            uint32_t *stackBase = g_taskStacks[i];
            uint32_t *stackTop = stackBase + words;
            stackTop = (uint32_t *)((uintptr_t)stackTop & ~(uintptr_t)0x7U);

            /* Initial exception frame (as if returning from interrupt) */
            *(--stackTop) = 0x01000000U;                /* xPSR: Thumb bit */
            *(--stackTop) = (uint32_t)taskFunction;     /* PC */
            *(--stackTop) = (uint32_t)endTask;          /* LR */
            *(--stackTop) = 0U;                         /* R12 */
            *(--stackTop) = 0U;                         /* R3  */
            *(--stackTop) = 0U;                         /* R2  */
            *(--stackTop) = 0U;                         /* R1  */
            *(--stackTop) = (uint32_t)parameters;       /* R0  */

            /* Space for callee-saved registers (R4-R11). */
            for (int j = 0; j < 8; ++j) {
                *(--stackTop) = 0U;
            }

            g_tasks[i].stackPointer = stackTop;
            g_tasks[i].active = true;
            g_tasks[i].suspended = false;
            g_tasks[i].priority = priority;
            g_tasks[i].name = name;

            return;
        }
    }

    return;
}

void endTask(void)
{
    if (g_currentTask != NULL) {
        g_currentTask->active = false;
        g_currentTask->suspended = false;
    }

    /* Switch to next task; this function should never return. */
    yield();
    for (;;);
}

TaskIndex getCurrentTaskIndex(void)
{
    return g_currentTaskIndex;
}

void interruptTask()
{
    if (g_currentTask != NULL) {
        g_currentTask->active = true;
        g_currentTask->suspended = true;
    }
    yield();
}

void resumeTask(TaskIndex index)
{
    if (index < MAX_TASKS && g_tasks[index].suspended) {
        g_tasks[index].suspended = false;
        g_tasks[index].active = true;
    }
    yield();
}

void runScheduler(void)
{
    /* Find first active task. */
    g_currentTaskIndex = 0;
    while (g_currentTaskIndex < MAX_TASKS && (!g_tasks[g_currentTaskIndex].active || g_tasks[g_currentTaskIndex].suspended)) {
        g_currentTaskIndex++;
    }
    if (g_currentTaskIndex >= MAX_TASKS) {
        return;
    }

    g_currentTask = &g_tasks[g_currentTaskIndex];

    /* Set PendSV and SysTick to lowest priority (lowest preemption priority). */
    SCB_SHPR3 &= ~((0xFFUL << 16) | (0xFFUL << 24));
    SCB_SHPR3 |= ((0xFFUL << 16) | (0xEFUL << 24));

    /* Configure SysTick (adjust reload for your core clock). */
    const uint32_t reload = 16000U - 1U; /* ~1ms tick for 16MHz */
    SYST_RVR = reload;
    SYST_CVR = 0U;
    SYST_CSR = (1U << 2) | (1U << 1) | (1U << 0); /* CLKSOURCE=1, TICKINT=1, ENABLE=1 */

    /* Switch to process stack and start the first task. */
    setPSP((uint32_t)g_currentTask->stackPointer);
    __set_CONTROL(2U); 
    __svc_trigger();
}

__attribute__((naked)) void PendSV_Handler(void)
{
    __asm volatile(
        "mrs r0, psp\n"
        "stmdb r0!, {r4-r11}\n"
        "ldr r1, =g_currentTask\n"
        "ldr r2, [r1]\n"
        "str r0, [r2]\n"
        "push {lr}\n"
        "bl scheduleNextTask\n"
        "pop {lr}\n"
        "ldr r1, =g_currentTask\n"
        "ldr r2, [r1]\n"
        "ldr r0, [r2]\n"
        "ldmia r0!, {r4-r11}\n"
        "msr psp, r0\n"
        "bx lr\n"
    );
}
