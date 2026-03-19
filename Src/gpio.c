void gpio_init(void)
{
    gpio_set_pin_mode(PORTG, PIN_9, OUTPUT); /* Set PG9 as output */
}

void gpio_set_pin_mode(enum GPIO_Port port, enum GPIO_Pin pin, enum PinMode mode)
{
    /* Implement GPIO pin set here. */
    GPIO_MODER(port) &= ~(0x03U << (2 << pin)); /* Clear mode bits for the pin */
    GPIO_MODER(port) |= (mode << (2 << pin)); /* Set mode bits for the pin */
}

PinState gpio_pin_status(enum GPIO_Port port, enum GPIO_Pin pin)
{
    /* Implement GPIO pin get here. */
    return (GPIO_ODR(port) & (1U << pin)) >> pin; /* Return the state of the pin */
}

void gpio_set_pin_output(enum GPIO_Port port, enum GPIO_Pin pin)
{
    /* Implement GPIO pin set here. */
    GPIO_ODR(port) = (1U << pin); /* Set the pin */
}

void gpio_clear_pin_output(enum GPIO_Port port, enum GPIO_Pin pin)
{
    /* Implement GPIO pin clear here. */
    GPIO_ODR(port) = (0U << pin); /* Clear the pin */
}

void atomic_gpio_set_pin_output(enum GPIO_Port port, enum GPIO_Pin pin)
{
    /* Implement atomic GPIO pin set here. */
    GPIO_BSRR(port) = (1U << pin); /* Set the pin atomically */
}

void atomic_gpio_clear_pin_output(enum GPIO_Port port, enum GPIO_Pin pin)
{
    /* Implement atomic GPIO pin clear here. */
    GPIO_BSRR(port) = (1U << (pin + 16U)); /* Clear the pin atomically */
}