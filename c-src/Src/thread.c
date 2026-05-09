#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "thread.h"
#include "timer.h"

/*
 * Simple cooperative / preemptive round-robin scheduler for Cortex-M3.
 *
 * This implementation is intentionally small and self-contained.
 * - createTask() allocates a stack and prepares the initial exception frame.
 * - yield() and SysTick trigger PendSV to perform a context switch.
 * - runScheduler() starts SysTick and switches to the first created task.
 */

static Tick currentTick;
static TaskControlBlock g_tasks[MAX_TASKS];
static TaskIndex g_currentTaskIndex;
static TaskControlBlock *g_currentTask;
static uint32_t g_createdTaskCount;

static uint32_t g_taskStacks[MAX_TASKS][MAX_STACK_WORDS];

static inline void __set_PSP(uint32_t psp)
{
    __asm volatile("msr psp, %0" :: "r"(psp) : "memory");
}

static inline void __dsb(void)
{
    __asm volatile("dsb");
}
static inline void __isb(void)
{
    __asm volatile("isb");
}

static inline void __increment_tick(void)
{
    currentTick++;
}

static inline void __svc_trigger(void)
{
    __asm volatile("SVC #0");
}

static inline void __trigger_pendsv(void)
{
    SCB_ICSR = SCB_ICSR_PENDSVSET_Msk;
}

static inline void __wait_for_interrupt(void)
{
    __asm volatile("wfi");
}

__attribute__((optimize("O3"))) void yield(void)
{
    /* Request a PendSV exception to perform a context switch. */
    __trigger_pendsv();
    __dsb();
    __isb();
}

__attribute__((naked)) void SVC_Handler(void)
{
    __asm volatile(
        "ldr r1, =g_currentTask\n"
        "ldr r2, [r1]\n"
        "ldr r0, [r2]\n"
        "ldmia r0!, {r4-r11}\n"
        "msr psp, r0\n"
        "movs r0, #2\n"
        "msr control, r0\n"
        "isb\n"
        "ldr lr, =0xFFFFFFFD\n"
        "bx lr\n"
    );
}

__attribute__((interrupt)) void SysTick_Handler(void)
{
    /* Trigger PendSV to perform a context switch on SysTick. */
    __increment_tick();
    __trigger_pendsv();
}

void createTask(void (*taskFunction)(void *), const char *name, uint16_t stackSize, void *parameters, uint32_t priority)
{
    (void)name;
    (void)priority;

    for (uint32_t i = 0; i < MAX_TASKS; ++i) {
        if (!g_tasks[i].state) {
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
            g_tasks[i].state = ACTIVE;
            g_tasks[i].priority = priority;
            g_tasks[i].name = name;
            g_tasks[i].endTick = 0;
            g_createdTaskCount++;

            return;
        }
    }

    return;
}

void scheduleNextTask(void)
{
    if (g_createdTaskCount == 0U) {
        g_currentTask = NULL;
        return;
    }

    for (;;) {
        uint32_t completedTasks = 0U;

        for (uint32_t i = 1U; i <= g_createdTaskCount; ++i) {
            uint32_t next = (g_currentTaskIndex + i) % g_createdTaskCount;

            if (g_tasks[next].state == SUSPENDED && currentTick >= g_tasks[next].endTick) {
                g_tasks[next].state = ACTIVE;
            }

            switch (g_tasks[next].state) {
            case ACTIVE:
                g_currentTaskIndex = next;
                g_currentTask = &g_tasks[next];
                return;
            case SUSPENDED:
                break;
            case COMPLETED:
                completedTasks++;
                break;
            case UNUSED:
            default:
                break;
            }
        }

        if (completedTasks >= g_createdTaskCount) {
            g_currentTask = NULL;
            return;
        }

        __wait_for_interrupt();
    }
}


void endTask(void)
{
    if (g_currentTask != NULL) {
        g_currentTask->state = COMPLETED;
    }

    /* Switch to next task; this function should never return. */
    yield();
    for (;;);
}

TaskIndex getCurrentTaskIndex(void)
{
    return g_currentTaskIndex;
}

void threadDelay(Delay delay_time, delay_units_t unit) 
{
    interruptTask(delay_time * unit);
}

void interruptTask(Delay delay)
{
    if (g_currentTask != NULL) {
        g_currentTask->state = SUSPENDED;
        if (delay > 0){
            g_currentTask->endTick = currentTick + delay;
        }
    }
    __trigger_pendsv();
}

void resumeTask(TaskIndex index)
{
    if (index < MAX_TASKS && g_tasks[index].state == SUSPENDED) {
        g_tasks[index].state = ACTIVE;
    }
    __trigger_pendsv();
}

__attribute__((optimize("O3"))) void runScheduler(void)
{
    /* Find first active task. */
    g_currentTaskIndex = 0;
    currentTick = 0;
    g_currentTask = NULL;
    while (g_currentTaskIndex < MAX_TASKS && g_tasks[g_currentTaskIndex].state != ACTIVE) {
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
    __set_PSP((uint32_t)g_currentTask->stackPointer);
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
