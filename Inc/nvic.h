#ifndef NVIC_H
#define NVIC_H

#ifdef __cplusplus
extern "C" {
#endif

#define NVIC_ISER_BASE 0xE000E100

#define NVIC_ISER(bit) (*((volatile uint32_t *)(NVIC_ISER_BASE + ((bit >> 5) << 2))))
#define NVIC_ISER_MASK(bit) (0x1U << bit)

typedef enum {
    NVIC_IRQ_TIM2 = 28,
    NVIC_IRQ_TIM3 = 29,
    NVIC_IRQ_TIM4 = 30,
    NVIC_IRQ_TIM5 = 50,
} nvic_irq_t;

#ifdef __cplusplus
}
#endif

#endif /* NVIC_H */
