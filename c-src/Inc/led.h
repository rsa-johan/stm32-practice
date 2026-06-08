#ifndef LED_H
#define LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "gpio.h"
#include "i2c.h"
#include "mfx.h"

typedef enum {
    Self,
    Expanded
} LedType;

typedef struct {
    LedType type;
    GPIO gpio;
    I2C i2c;
    mfx_config io_expander_config;
} LedInfo;

#define LED1 ((LedInfo){Self, GPIO_CONFIG(PORTG, PIN_9, OUTPUT, PUSH_PULL, PULL_DOWN, AF_NONE), i2c_get_device(NONE), get_mfx_config(MFX_NONE) })
#define LED2 ((LedInfo){Expanded, GPIO_CONFIG(PORTF, PIN_6, OUTPUT, PUSH_PULL, PULL_DOWN, AF_NONE), i2c_get_device(MFX), get_mfx_config(MFX_LED_2) })
#define LED3 ((LedInfo){Self, GPIO_CONFIG(PORTF, PIN_11, OUTPUT, PUSH_PULL, PULL_DOWN, AF_NONE), i2c_get_device(NONE), get_mfx_config(MFX_NONE) })

void led_init(void);
void led_on(LedInfo led);
void led_off(LedInfo led);
void led_toggle(LedInfo led);

#ifdef __cplusplus
}
#endif

#endif /* LED_H */
