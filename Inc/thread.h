#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_TASKS 4
#define MAX_STACK_WORDS 256

/* Cortex-M3 system registers (minimal subset needed for this scheduler) */
#define SCB_ICSR (*(volatile uint32_t *)0xE000ED04U)
#define SCB_SHPR3 (*(volatile uint32_t *)0xE000ED20U)
#define SCB_ICSR_PENDSVSET_Msk (1UL << 28)

#define SYST_CSR (*(volatile uint32_t *)0xE000E010U)
#define SYST_RVR (*(volatile uint32_t *)0xE000E014U)
#define SYST_CVR (*(volatile uint32_t *)0xE000E018U)

typedef uint32_t TaskIndex;

typedef struct TaskControlBlock{
    uint32_t *stackPointer;
    bool active;
    bool suspended;
    uint32_t priority;
    const char *name;
} TaskControlBlock;


void yield(void);

void createTask(void (*taskFunction)(void *), const char *name, uint16_t stackSize, void *parameters, uint32_t priority);
void endTask(void) __attribute__((used));
void interruptTask(void);
void runScheduler(void);
void resumeTask(TaskIndex index);
TaskIndex getCurrentTaskIndex(void);

#ifdef __cplusplus
}
#endif

#endif /* THREAD_H */
