#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void *createTask(void (*taskFunction)(void *), const char *name, uint16_t stackSize, void *parameters, uint32_t priority);
void *endTask(void);
void runScheduler(void);
void yield(void);

#ifdef __cplusplus
}
#endif

#endif // THREAD_H
