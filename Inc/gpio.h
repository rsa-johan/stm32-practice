#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_MODER_OFFSET 0x00U
#define GPIO_ODR_OFFSET 0x14U
#define GPIO_BSRR_OFFSET 0x18U
#define GPIO_OTYPER_OFFSET 0x04U
#define GPIO_PUPDR_OFFSET 0x0CU

#define GPIO_MODER(port) (*(volatile uint32_t *)((port) + GPIO_MODER_OFFSET))
#define GPIO_ODR(port) (*(volatile uint32_t *)((port) + GPIO_ODR_OFFSET))
#define GPIO_BSRR(port) (*(volatile uint32_t *)((port) + GPIO_BSRR_OFFSET))
#define GPIO_OTYPER(port) (*(volatile uint32_t *)((port) + GPIO_OTYPER_OFFSET))
#define GPIO_PUPDR(port) (*(volatile uint32_t *)((port) + GPIO_PUPDR_OFFSET))
    
#define GPIO_BASE 0x48000000U
#define GPIO_PORTA_OFFSET 0x0000U
#define GPIO_PORTB_OFFSET 0x0400U
#define GPIO_PORTC_OFFSET 0x0800U
#define GPIO_PORTD_OFFSET 0x0C00U
#define GPIO_PORTE_OFFSET 0x1000U
#define GPIO_PORTF_OFFSET 0x1400U
#define GPIO_PORTG_OFFSET 0x1800U

#define GPIO_C2G_RESET_VALUE 0xFFFFFFFFU

typedef uint32_t PinState;

typedef enum {
    PORTA = GPIO_BASE + GPIO_PORTA_OFFSET,
    PORTB = GPIO_BASE + GPIO_PORTB_OFFSET,
    PORTC = GPIO_BASE + GPIO_PORTC_OFFSET,
    PORTD = GPIO_BASE + GPIO_PORTD_OFFSET,
    PORTE = GPIO_BASE + GPIO_PORTE_OFFSET,
    PORTF = GPIO_BASE + GPIO_PORTF_OFFSET,
    PORTG = GPIO_BASE + GPIO_PORTG_OFFSET
} GPIO_Port;

typedef enum {
    PIN_0 = 0,
    PIN_1 = 1,
    PIN_2 = 2,
    PIN_3 = 3,
    PIN_4 = 4,
    PIN_5 = 5,
    PIN_6 = 6,
    PIN_7 = 7,
    PIN_8 = 8,
    PIN_9 = 9,
    PIN_10 = 10,
    PIN_11 = 11,
    PIN_12 = 12,
    PIN_13 = 13,
    PIN_14 = 14,
    PIN_15 = 15
} GPIO_Pin;

typedef enum {
    INPUT = 0x00U,
    OUTPUT = 0x01U,
    ALTERNATE_FUNCTION = 0x02U,
    ANALOG = 0x03U
} PinMode;

typedef enum {
    PUSH_PULL = 0x00U,
    OPEN_DRAIN = 0x01U
} PinOutputType;

typedef enum {
    NO_PULL = 0x00U,
    PULL_UP = 0x01U,
    PULL_DOWN = 0x02U
} PullUpPullDown;

void gpio_init(void);
void gpio_set_pin_mode(GPIO_Port port, GPIO_Pin pin, PinMode mode, PinOutputType outputType, PullUpPullDown pupd);
void gpio_set_pin_output(GPIO_Port port, GPIO_Pin pin);
void gpio_clear_pin_output(GPIO_Port port, GPIO_Pin pin);
void atomic_gpio_set_pin_output(GPIO_Port port, GPIO_Pin pin);
void atomic_gpio_clear_pin_output(GPIO_Port port, GPIO_Pin pin);

PinState gpio_pin_status(GPIO_Port port, GPIO_Pin pin);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_H */