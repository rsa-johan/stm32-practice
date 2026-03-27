#include "gpio.h"
#include <stdint.h>

static inline void gpio_reset(GPIO_Port port, GPIO_Pin pin);
static inline void gpio_reset(GPIO_Port port, GPIO_Pin pin)
{
    int shift = pin << 1; 
    GPIO_MODER(port) |= (ANALOG << shift);
    uint32_t mode = GPIO_MODER(port);
    (void)mode;
}

static inline void gpio_set_pupd(GPIO_Port port, GPIO_Pin pin, PinOutputType outputType, PullUpPullDown pupd);
static inline void gpio_set_pupd(GPIO_Port port, GPIO_Pin pin, PinOutputType outputType, PullUpPullDown pupd)
{
    (void)pupd;
    if (outputType == OPEN_DRAIN) {
        GPIO_OTYPER(port) |= (1U << pin); 
    } else {
        GPIO_OTYPER(port) &= ~(1U << pin); 
    }
    uint32_t type = GPIO_OTYPER(port);
    (void)type;
}

void gpio_init(void)
{
    /* Implement GPIO initialization here. */
}

void gpio_set_pin_mode(GPIO_Port port, GPIO_Pin pin, PinMode mode, PinOutputType outputType, PullUpPullDown pupd)
{
    gpio_reset(port, pin);
    GPIO_MODER(port) &= ~((~mode & 0x03) << (pin << 1));
    uint32_t pinMode = GPIO_MODER(port);
    (void)pinMode;
    gpio_set_pupd(port, pin, outputType, pupd);
}


PinState gpio_pin_status(GPIO_Port port, GPIO_Pin pin)
{
    /* Implement GPIO pin get here. */
    return (PinState)((GPIO_ODR(port) & (1U << pin)) >> pin); /* Return the state of the pin */
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