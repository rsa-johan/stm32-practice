#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void yield(void);

void *createTask(void (*taskFunction)(void *), const char *name, uint16_t stackSize, void *parameters, uint32_t priority);

void runScheduler(void);

#ifdef __cplusplus
}
#endif

#endif /* THREAD_H */
