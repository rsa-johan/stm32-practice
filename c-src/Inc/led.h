#ifndef LED_H
#define LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "gpio.h"
#include "i2c.h"

typedef struct {
    GPIO gpio;
    I2C i2c;
} LedInfo;

#define LED1 ((LedInfo){GPIO_CONFIG(PORTG, PIN_9, OUTPUT, PUSH_PULL, PULL_DOWN, AF_NONE), (get_i2c_device(NONE)) })
#define LED2 ((LedInfo){GPIO_CONFIG(PORTF, PIN_11, OUTPUT, PUSH_PULL, PULL_DOWN, AF_NONE), (get_i2c_device(NONE)) })
#define LED3 ((LedInfo){GPIO_CONFIG(PORTF, PIN_6, OUTPUT, PUSH_PULL, PULL_DOWN, AF_NONE), (get_i2c_device(MFX)) })

void led_init(void);
void led_on(LedInfo led);
void led_off(LedInfo led);
void led_toggle(LedInfo led);

#ifdef __cplusplus
}
#endif

#endif /* LED_H */
