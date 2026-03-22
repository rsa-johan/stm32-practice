#include "gpio.h"
#include <stdint.h>

static inline void gpio_reset(GPIO_Port port, GPIO_Pin pin);
static inline void gpio_reset(GPIO_Port port, GPIO_Pin pin)
{
    GPIO_MODER(port) &= ~(0x03U << (pin << 1));
}

void gpio_init(void)
{
    gpio_set_pin_mode(PORTG, PIN_9, OUTPUT); /* Set PG9 as output */
}

void gpio_set_pin_mode(GPIO_Port port, GPIO_Pin pin, PinMode mode)
{
    /* Implement GPIO pin set here. */
    gpio_reset(port, pin);
    GPIO_MODER(port) |= (mode << (pin << 1));
    uint32_t pinMode = (GPIO_MODER(port) >> (pin << 1)) & 0x03U; /* Get the current mode of the pin */
}


PinState gpio_pin_status(GPIO_Port port, GPIO_Pin pin)
{
    /* Implement GPIO pin get here. */
    return (GPIO_ODR(port) & (1U << pin)) >> pin; /* Return the state of the pin */
}

void gpio_set_pin_output(GPIO_Port port, GPIO_Pin pin)
{
    /* Implement GPIO pin set here. */
    GPIO_ODR(port) |= (1U << pin); /* Set the pin */
}

void gpio_clear_pin_output(GPIO_Port port, GPIO_Pin pin)
{
    /* Implement GPIO pin clear here. */
    GPIO_ODR(port) &= ~(1U << pin); /* Clear the pin */
}

void atomic_gpio_set_pin_output(GPIO_Port port, GPIO_Pin pin)
{
    /* Implement atomic GPIO pin set here. */
    GPIO_BSRR(port) = (1U << pin); /* Set the pin atomically */
}

void atomic_gpio_clear_pin_output(GPIO_Port port, GPIO_Pin pin)
{
    /* Implement atomic GPIO pin clear here. */
    GPIO_BSRR(port) = (1U << (pin + 16U)); /* Clear the pin atomically */
}