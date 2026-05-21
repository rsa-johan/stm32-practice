#include "led.h"

static Led_config_set config_set = {0};

void led_setup(void) {
    for (int led_i = 0; led_i < (int)Led_Max; led_i++) {
        Led_config config = get_led_config((Led)led_i);
        HAL_GPIO_Init(config.gpio_port, &config.gpio);
        config_set[led_i] = config;

        led_on((Led)led_i);
    }
}

Led_config get_led_config(Led led) {
    Led_config config = {0};
    switch (led) {
        case Led_1:
            config.gpio_port = GPIOG;
            config.gpio.Pin = GPIO_PIN_9;
            config.gpio.Mode = GPIO_MODE_OUTPUT_OD;
            config.gpio.Pull = GPIO_PULLUP;
            break;
        case Led_2:
            config.gpio_port = GPIOF;
            config.gpio.Pin = GPIO_PIN_11;
            config.gpio.Mode = GPIO_MODE_OUTPUT_OD;
            config.gpio.Pull = GPIO_PULLUP;
            break;
        case Led_Max:
            // Do nothing
            break;
    }

    return config;
}

void led_on(Led led) {
    Led_config config = config_set[(int)led];
    HAL_GPIO_WritePin(config.gpio_port, config.gpio.Pin, GPIO_PIN_SET);
}

void led_off(Led led) {
    Led_config config = config_set[(int)led];
    HAL_GPIO_WritePin(config.gpio_port, config.gpio.Pin, GPIO_PIN_RESET);
}

void led_toggle(Led led) {
    Led_config config = config_set[(int)led];
    HAL_GPIO_TogglePin(config.gpio_port, config.gpio.Pin);
}