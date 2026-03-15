#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum : uint32_t {
    DELAY_UNITS_MS = 0,
    DELAY_UNITS_S = 1
} delay_units_t;

void sys_clock_init(void);
void delay(uint32_t units, delay_units_t unit);

#ifdef __cplusplus
}
#endif

#endif /* TIMER_H */