#include "led.h"

void led_init() {
    led_off(LedPort1, LED1);
    led_off(LedPort2, LED2);
}

void led_on(LedPort port, LedPin pin) {
    atomic_gpio_set_pin_output(port, pin);
}

void led_off(LedPort port, LedPin pin) {
    atomic_gpio_clear_pin_output(port, pin);
}

void led_toggle(LedPort port, LedPin pin) {
    PinState pin_state = gpio_pin_status(port, pin);
    pin_state ? led_off(port, pin) : led_on(port, pin);
}