#include "led.h"
#include "thread.h"
#include "timer.h"
#include "nvic.h"
#include <stdbool.h>
#include <stdint.h>


static inline nvic_irq_t get_nvic_from_timer(timer_t timer)
{
    switch (timer) {
        case TIM_2:
            return NVIC_IRQ_TIM2;
        case TIM_3:
            return NVIC_IRQ_TIM3;
        case TIM_4:
            return NVIC_IRQ_TIM4;
        case TIM_5:
            return NVIC_IRQ_TIM5;
        default:
            return NVIC_IRQ_TIM2; // Default case
    }
}

static inline timer_t get_timer_from_task_index(TaskIndex index)
{
    switch (index) {
        case 0:
            return TIM_2;
        case 1:
            return TIM_3;
        case 2:
            return TIM_4;
        case 3:
            return TIM_5;
        default:
            return TIM_2;
    }
}

static inline void enable_timer_interrupt(timer_t timer)
{
    TIM_DIER(timer) |= TIM_DIER_UIE;
}

static inline void disable_timer_interrupt(timer_t timer)
{
    TIM_DIER(timer) &= ~TIM_DIER_UIE;
}

static inline void clear_update_interrupt_flag(timer_t timer)
{
    if (TIM_SR(timer) & TIM_SR_UIF) {
        TIM_SR(timer) &= ~TIM_SR_UIF;
    }
}

static inline uint32_t __read_interrupt_status(timer_t timer) {
    nvic_irq_t irq = get_nvic_from_timer(timer);
    return (uint32_t)(NVIC_IABR(irq));
}

static inline void __reinit_cnt(timer_t timer)
{
    TIM_EGR(timer) |= TIM_EGR_UG;
}

static inline void __reset_timer(timer_t timer)
{
    TIM_CNT(timer) = 0;
    TIM_CR(timer) &= ~TIM_CR_CEN;
    clear_update_interrupt_flag(timer);
}

static inline void __start_timer(timer_t timer)
{
    TIM_CR(timer) |= TIM_CR_CEN;
}

static inline void __enable_iser(timer_t timer)
{
    nvic_irq_t irq = get_nvic_from_timer(timer);
    NVIC_ISER(irq) = (uint32_t)NVIC_ISER_MASK(irq);
}

static inline void __enable_irq(void)
{
    __asm volatile("cpsie i" ::: "memory");
}

static inline void set_psc(timer_t timer, uint32_t psc_value) 
{
    TIM_PSC(timer) = psc_value - 1U;
}

static inline void set_arr(timer_t timer, uint32_t arr)
{
    TIM_ARR(timer) = arr - 1U;
}
    
void delay(uint32_t delay_time, delay_units_t unit) 
{
    TaskIndex currentTaskIndex = getCurrentTaskIndex();
    timer_t timer = get_timer_from_task_index(currentTaskIndex);
    __reset_timer(timer);
    set_arr(timer,  delay_time * unit);
    enable_timer_interrupt(TIM_2);
    __start_timer(timer);
    interruptTask();
}

void timer_init(void)
{
    // prescaler setup
    set_psc(TIM_2, FREQ_HZ/DELAY_UNITS_MS);

    // reset of interrupt, counter and timer
    disable_timer_interrupt(TIM_2);
    __reinit_cnt(TIM_2);
    __reset_timer(TIM_2);

    // enable interrupt in 
    __enable_iser(TIM_2);
    __enable_irq();
}

__attribute__((interrupt)) void TIM2_IRQHandler(void)
{
    timer_t timer = TIM_2;
    disable_timer_interrupt(timer);
    clear_update_interrupt_flag(timer);
    resumeTask((TaskIndex)0);
}

__attribute__((interrupt)) void TIM3_IRQHandler(void)
{
    timer_t timer = TIM_3;
    disable_timer_interrupt(timer);
    clear_update_interrupt_flag(timer);
    resumeTask((TaskIndex)1);
}

__attribute__((interrupt)) void TIM4_IRQHandler(void)
{
    timer_t timer = TIM_4;
    disable_timer_interrupt(timer);
    clear_update_interrupt_flag(timer);
    resumeTask((TaskIndex)2);
}

__attribute__((interrupt)) void TIM5_IRQHandler(void)
{
    timer_t timer = TIM_5;
    disable_timer_interrupt(timer);
    clear_update_interrupt_flag(timer);
    resumeTask((TaskIndex)3);
}


// void delay(uint32_t delay_time, delay_units_t unit, uint8_t has_callback, void *callback_fn) 
// {
//     // proto for sth else
//     (void)has_callback;
//     (void)callback_fn;
    
//     timer_t timer = TIM_2;
//     __reset_timer(timer);
//     set_arr(timer,  delay_time * unit);
//     enable_timer_interrupt(TIM_2);
//     __start_timer(timer);
//     // interruptTask();
// }