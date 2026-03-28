#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#define FREQ_HZ 16000U // timer clock freq is 16 MHz -> ms

#define xTIM_CR_OFFSET 0x000U
#define xTIM_CNT_OFFSET 0x024U
#define xTIM_PSC_OFFSET 0x028U
#define xTIM_ARR_OFFSET 0x02CU
#define xTIM_DIER_OFFSET 0x0CU
#define xTIM_SR_OFFSET 0x010U
#define xTIM_EGR_OFFSET 0x014U


#define TIMER_BASE 0x40000000U

#define TIM_CR(timer) (*(volatile uint32_t *)(TIMER_BASE + (timer) + xTIM_CR_OFFSET))
#define TIM_CNT(timer) (*(volatile uint32_t *)(TIMER_BASE + (timer) + xTIM_CNT_OFFSET))
#define TIM_PSC(timer) (*(volatile uint32_t *)(TIMER_BASE + (timer) + xTIM_PSC_OFFSET))
#define TIM_ARR(timer) (*(volatile uint32_t *)(TIMER_BASE + (timer) + xTIM_ARR_OFFSET))
#define TIM_DIER(timer) (*(volatile uint32_t *)(TIMER_BASE + (timer) + xTIM_DIER_OFFSET))
#define TIM_SR(timer) (*(volatile uint32_t *)(TIMER_BASE + (timer) + xTIM_SR_OFFSET))
#define TIM_EGR(timer) (*(volatile uint32_t *)(TIMER_BASE + (timer) + xTIM_EGR_OFFSET))

#define TIM_CR_CEN (1U << 0)
#define TIM_DIER_UIE (1U << 0)
#define TIM_SR_UIF (1U << 0)
#define TIM_EGR_UG (1U << 0)

typedef enum {
    TIM_2 = 0x0000U,
    TIM_3 = 0x0400U,
    TIM_4 = 0x0800U,
    TIM_5 = 0x0C00U,
    TIM_6 = 0x1000U,
    TIM_7 = 0x1400U
} timer_t;

typedef enum {
    DELAY_UNITS_MS = 1,
    DELAY_UNITS_S = 1000
} delay_units_t;


void timer_init(void);
//void delay(uint32_t units, delay_units_t unit, uint8_t has_callback, void *callback_fn);
void delay(uint32_t units, delay_units_t unit);
void delay_while(uint32_t units, delay_units_t unit);

#ifdef __cplusplus
}
#endif

#endif /* TIMER_H */