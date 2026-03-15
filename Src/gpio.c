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