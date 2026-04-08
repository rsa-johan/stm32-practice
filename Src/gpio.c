#include "gpio.h"
#include "af.h"

static inline void gpio_reset(GPIO gpio);
static inline void gpio_reset(GPIO gpio)
{
    uint32_t shift = gpio.pin << 1;
    gpio.port->MODER |= (ANALOG << shift);
    uint32_t mode = gpio.port->MODER;
    (void)mode;
}

static inline void gpio_set_pupd(GPIO gpio);
static inline void gpio_set_pupd(GPIO gpio)
{
    (void)gpio.pull;
    if (gpio.outputType == OPEN_DRAIN) {
        gpio.port->OTYPER |= (1U << gpio.pin);
    } else {
        gpio.port->OTYPER &= ~(1U << gpio.pin);
    }
}

void gpio_init(void)
{
    /* Implement GPIO initialization here. */
}

static void gpio_set_af_type(GPIO gpio)
{
    gpio.port->AFR[(gpio.pin >> 3) & 0x01U] |=
        gpio.alternateFunction << ((gpio.pin & 0x07U) << 2);
}

void gpio_set_pin_mode(GPIO gpio)
{
    gpio_reset(gpio);
    gpio.port->MODER &= ~((~gpio.mode & 0x03U) << (gpio.pin << 1));
    uint32_t pinMode = gpio.port->MODER;
    (void)pinMode;
    gpio_set_pupd(gpio);
    if (gpio.mode == ALTERNATE_FUNCTION) {
        gpio_set_af_type(gpio);
    }
}


PinState gpio_pin_status(GPIO gpio)
{
    /* Implement GPIO pin get here. */
    return (PinState)((gpio.port->ODR & (1U << gpio.pin)) >> gpio.pin); /* Return the state of the pin */
}

void gpio_set_pin_output(GPIO gpio)
{
    /* Implement GPIO pin set here. */
    gpio.port->ODR |= (1U << gpio.pin); /* Set the pin */
}

void gpio_clear_pin_output(GPIO gpio)
{
    /* Implement GPIO pin clear here. */
    gpio.port->ODR &= ~(1U << gpio.pin); /* Clear the pin */
}

void atomic_gpio_set_pin_output(GPIO gpio)
{
    /* Implement atomic GPIO pin set here. */
    gpio.port->BSRR = (1U << gpio.pin); /* Set the pin atomically */
}

void atomic_gpio_clear_pin_output(GPIO gpio)
{
    /* Implement atomic GPIO pin clear here. */
    gpio.port->BSRR = (1U << (gpio.pin + 16U)); /* Clear the pin atomically */
}
