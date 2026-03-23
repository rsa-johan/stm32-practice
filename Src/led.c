#include "led.h"
#include "gpio.h"

void led_init() {
    led_off(LED1);
    led_off(LED2);
}

void led_on(LedInfo led) {
    atomic_gpio_clear_pin_output(led.port, led.pin);
}

void led_off(LedInfo led) {
    atomic_gpio_set_pin_output(led.port, led.pin);
}

void led_toggle(LedInfo led) {
    PinState pin_state = gpio_pin_status(led.port, led.pin);
    pin_state ? led_off(led) : led_on(led);
}