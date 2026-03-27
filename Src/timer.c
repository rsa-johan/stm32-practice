#include "thread.h"
#include "timer.h"
#include "nvic.h"
#include <stdint.h>

static inline void clear_update_interrupt_flag(timer_t timer)
{
    if (TIM_SR(timer) & TIM_SR_UIF) {
        TIM_SR(timer) &= ~TIM_SR_UIF;
    }
}

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

static inline void disable_timer_interrupt(timer_t timer)
{
    clear_update_interrupt_flag(timer);
    TIM_DIER(timer) &= ~TIM_DIER_UIE;
    TIM_CNT(timer) &= ~TIM_CR_CEN;
}

static inline void enable_timer_interrupt(timer_t timer)
{
    TIM_DIER(timer) |= TIM_DIER_UIE;
    TIM_CNT(timer) |= TIM_CR_CEN;
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
            return TIM_2; // Default case
    }
}

void delay_while(uint32_t units, delay_units_t unit)
{
    uint32_t time_units = units * (uint32_t)unit;
    while(time_units--);
}
    
void delay(uint32_t units, delay_units_t unit)
{
    TaskIndex currentTaskIndex = getCurrentTaskIndex();
    timer_t timer = get_timer_from_task_index(currentTaskIndex);

    uint32_t time_units = units * (uint32_t)unit;
    uint32_t auto_reload_value = time_units - 1;
    TIM_ARR(timer) = auto_reload_value;

    enable_timer_interrupt(timer);
    interruptTask();
}

void timer_init(void)
{
    uint32_t prescaler = 16000 - 1;
    TIM_PSC(TIM_2) = prescaler;
    TIM_PSC(TIM_3) = prescaler;
    TIM_PSC(TIM_4) = prescaler;
    TIM_PSC(TIM_5) = prescaler;
    NVIC_ISER(NVIC_IRQ_TIM2) |= (uint32_t)NVIC_ISER_MASK(NVIC_IRQ_TIM2);
    NVIC_ISER(NVIC_IRQ_TIM3) |= (uint32_t)NVIC_ISER_MASK(NVIC_IRQ_TIM3);
    NVIC_ISER(NVIC_IRQ_TIM4) |= (uint32_t)NVIC_ISER_MASK(NVIC_IRQ_TIM4);
    NVIC_ISER(NVIC_IRQ_TIM5) |= (uint32_t)NVIC_ISER_MASK(NVIC_IRQ_TIM5);
}

__attribute__((interrupt)) void TIM2_IRQHandler(void)
{
    timer_t timer = TIM_2;
    disable_timer_interrupt(timer);
    resumeTask((TaskIndex)0);
}

__attribute__((interrupt)) void TIM3_IRQHandler(void)
{
    timer_t timer = TIM_3;
    disable_timer_interrupt(timer);
    resumeTask((TaskIndex)1);
}

__attribute__((interrupt)) void TIM4_IRQHandler(void)
{
    timer_t timer = TIM_4;
    disable_timer_interrupt(timer);
    resumeTask((TaskIndex)2);
}

__attribute__((interrupt)) void TIM5_IRQHandler(void)
{
    timer_t timer = TIM_5;
    disable_timer_interrupt(timer);
    resumeTask((TaskIndex)3);
}
