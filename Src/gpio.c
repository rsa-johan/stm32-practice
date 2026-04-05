#include "gpio.h"
#include "af.h"

static inline void gpio_reset(Gpio gpio);
static inline void gpio_reset(Gpio gpio)
{
    int shift = gpio.pin << 1;
    GPIO_MODER(gpio.port) |= (ANALOG << shift);
    uint32_t mode = GPIO_MODER(gpio.port);
    (void)mode;
}

static inline void gpio_set_pupd(Gpio gpio);
static inline void gpio_set_pupd(Gpio gpio)
{
    (void)gpio.pull;
    if (gpio.outputType == OPEN_DRAIN) {
        GPIO_OTYPER(gpio.port) |= (1U << gpio.pin);
    } else {
        GPIO_OTYPER(gpio.port) &= ~(1U << gpio.pin);
    }
}

void gpio_init(void)
{
    /* Implement GPIO initialization here. */
}

static void gpio_set_af_type(Gpio gpio)
{
    GPIO_AFR(gpio.port, gpio.pin) |= gpio.alternateFunction << ((gpio.pin & 0x07U) << 2);
}

void gpio_set_pin_mode(Gpio gpio)
{
    gpio_reset(gpio);
    GPIO_MODER(gpio.port) &= ~((~gpio.mode & 0x03U) << (gpio.pin << 1));
    uint32_t pinMode = GPIO_MODER(gpio.port);
    (void)pinMode;
    gpio_set_pupd(gpio);
    if (gpio.mode == ALTERNATE_FUNCTION) {
        gpio_set_af_type(gpio);
    }
}


PinState gpio_pin_status(Gpio gpio)
{
    /* Implement GPIO pin get here. */
    return (PinState)((GPIO_ODR(gpio.port) & (1U << gpio.pin)) >> gpio.pin); /* Return the state of the pin */
}

void gpio_set_pin_output(Gpio gpio)
{
    /* Implement GPIO pin set here. */
    GPIO_ODR(gpio.port) |= (1U << gpio.pin); /* Set the pin */
}

void gpio_clear_pin_output(Gpio gpio)
{
    /* Implement GPIO pin clear here. */
    GPIO_ODR(gpio.port) &= ~(1U << gpio.pin); /* Clear the pin */
}

void atomic_gpio_set_pin_output(Gpio gpio)
{
    /* Implement atomic GPIO pin set here. */
    GPIO_BSRR(gpio.port) = (1U << gpio.pin); /* Set the pin atomically */
}

void atomic_gpio_clear_pin_output(Gpio gpio)
{
    /* Implement atomic GPIO pin clear here. */
    GPIO_BSRR(gpio.port) = (1U << (gpio.pin + 16U)); /* Clear the pin atomically */
}
