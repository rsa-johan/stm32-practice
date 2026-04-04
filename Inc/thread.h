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

#define CONTROL_nPRIV_Msk (1U << 0) 

typedef uint32_t Delay;

typedef uint32_t TaskIndex;
typedef uint32_t Tick;
typedef enum {
    DELAY_UNITS_MS = 1,
    DELAY_UNITS_S = 1000
} delay_units_t;
typedef enum {
    UNUSED = 0,
    ACTIVE = 1,
    SUSPENDED,
    COMPLETED
} TaskState;

typedef struct TaskControlBlock{
    uint32_t *stackPointer;
    uint32_t priority;
    TaskState state;
    Tick endTick;
    const char *name;
} TaskControlBlock;


//m-level
void scheduleNextTask(void) __attribute__((used));
void endTask(void) __attribute__((used));

//p-level
void yield(void);
void createTask(void (*taskFunction)(void *), const char *name, uint16_t stackSize, void *parameters, uint32_t priority);
void runScheduler(void);

// timer-level
void interruptTask(Delay delay);
void resumeTask(TaskIndex index);
void threadDelay(Delay delay_time, delay_units_t unit);

//task-utils
TaskIndex getCurrentTaskIndex(void);

#ifdef __cplusplus
}
#endif

#endif /* THREAD_H */
