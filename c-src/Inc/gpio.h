#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "af.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_BASE 0x48000000U
#define GPIO_PORTA_OFFSET 0x0000U
#define GPIO_PORTB_OFFSET 0x0400U
#define GPIO_PORTC_OFFSET 0x0800U
#define GPIO_PORTD_OFFSET 0x0C00U
#define GPIO_PORTE_OFFSET 0x1000U
#define GPIO_PORTF_OFFSET 0x1400U
#define GPIO_PORTG_OFFSET 0x1800U

#define GPIO_C2G_RESET_VALUE 0xFFFFFFFFU

typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
    volatile uint32_t BRR;
} GPIO_Registers;

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
    PUSH_PULL = 0,
    OPEN_DRAIN = 1 
} PinOutputType;

typedef enum {
    NO_PULL = 0x00U,
    PULL_UP = 0x01U,
    PULL_DOWN = 0x02U
} PullUpPullDown;

typedef enum {
    PIN_LOW = 0,
    PIN_HIGH = 1
} PinState;

typedef struct {
    GPIO_Registers *port;
    GPIO_Pin pin;
    PinMode mode;
    PinOutputType outputType;
    PullUpPullDown pull;
    AF_TYPE alternateFunction;
} GPIO;

#define GPIO_CONFIG(port_, pin_, mode_, output_type_, pull_, af_) \
    ((GPIO){                                                  \
        .port = (GPIO_Registers *)(port_),                           \
        .pin = (pin_),                                               \
        .mode = (mode_),                                             \
        .outputType = (output_type_),                                \
        .pull = (pull_),                                             \
        .alternateFunction = (af_)                                   \
    })

void gpio_init(void);
void gpio_set_pin_mode(GPIO gpio);
void gpio_set_pin_output(GPIO gpio);
void gpio_clear_pin_output(GPIO gpio);
void atomic_gpio_set_pin_output(GPIO gpio);
void atomic_gpio_clear_pin_output(GPIO gpio);

PinState gpio_pin_status(GPIO gpio);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_H */
