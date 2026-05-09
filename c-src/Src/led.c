#include "led.h"
#include "gpio.h"

void led_init() {
    gpio_set_pin_mode(LED1.gpio);
    gpio_set_pin_mode(LED2.gpio);
    led_off(LED1);
    led_on(LED2);
}

void led_on(LedInfo led) {
    atomic_gpio_clear_pin_output(led.gpio);
}

void led_off(LedInfo led) {
    atomic_gpio_set_pin_output(led.gpio);
}

void led_toggle(LedInfo led) {
    PinState pin_state = gpio_pin_status(led.gpio);
    pin_state == PIN_HIGH ? led_on(led) : led_off(led);
}
