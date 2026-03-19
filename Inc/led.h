#ifndef LED_H
#define LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "gpio.h"

typedef enum : GPIO_Port {
    LedPort1 = PORTG,
    LedPort2 = PORTF,
} LedPort;

typedef enum : GPIO_Pin {
    LED1 = PIN_9,
    LED2 = PIN_11,
} LedPin;

void led_init(void);
void led_on(LedPort port, LedPin pin);
void led_off(LedPort port, LedPin pin);
void led_toggle(LedPort port, LedPin pin);

#ifdef __cplusplus
}
#endif

#endif /* LED_H */