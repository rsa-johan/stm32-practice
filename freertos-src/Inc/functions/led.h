#pragma once
#include "common.h"

typedef enum {
    Led_1 = 0,
    Led_2,
    Led_Max
} Led;

typedef struct Led_config {
    GPIO_InitTypeDef gpio;
    GPIO_TypeDef *gpio_port;
} Led_config;

typedef Led_config Led_config_set[Led_Max];

void led_setup(void);

Led_config get_led_config(Led);
void led_on(Led led);
void led_off(Led led);
void led_toggle(Led led);