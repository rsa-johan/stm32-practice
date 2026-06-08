#include "led.h"

void led_init() {
    gpio_set_pin_mode(LED1.gpio);
    gpio_set_pin_mode(LED2.gpio);
    led_off(LED1);
    led_on(LED2);
}

static inline void set_mfx_led(LedInfo *led, I2C_Address addr, uint8_t *data)
{
    I2C dev = i2c_get_device(addr);
    i2c_read(&dev, data, sizeof(*data));
    i2c_write(&dev, led->io_expander_config)
    *data |= (1 << (uint8_t)led->gpio.pin);
    i2c_write(&dev, data, sizeof(*data));
}

void led_on(LedInfo led) {
    if (led.type == Expanded) {
        uint8_t data = 0;
        set_mfx_led(&led, MFX, &data);
        return;
    }
    atomic_gpio_clear_pin_output(led.gpio);
}

void led_off(LedInfo led) {
    if (led.type == Expanded) {
        uint8_t data = 0;
        set_mfx_led(&led, MFX, &data);
        return;
    }
    atomic_gpio_set_pin_output(led.gpio);
}

void led_toggle(LedInfo led) {
    PinState pin_state = gpio_pin_status(led.gpio);
    pin_state == PIN_HIGH ? led_on(led) : led_off(led);
}
