#ifndef I2C_H
#define I2C_H

#include "gpio.h"
#include "af.h"

#define I2C_SDA_GPIO GPIO_CONFIG(PORTG, PIN_8, ALTERNATE_FUNCTION, OPEN_DRAIN, NO_PULL, AF_I2C3_SCL)
#define I2C_SCL_GPIO GPIO_CONFIG(PORTG, PIN_7, ALTERNATE_FUNCTION, OPEN_DRAIN, NO_PULL, AF_I2C3_SCL)


void i2c_init(void);

#endif /* I2C_H */
