#ifndef LED_H
#define LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "gpio.h"

typedef struct {
    GPIO_Port port;
    GPIO_Pin pin;
} LedInfo;

#define LED1 ((LedInfo){PORTG, PIN_9})
#define LED2 ((LedInfo){PORTF, PIN_11})

void led_init(void);
void led_on(LedInfo led);
void led_off(LedInfo led);
void led_toggle(LedInfo led);

#ifdef __cplusplus
}
#endif

#endif /* LED_H */