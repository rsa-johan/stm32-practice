#include "i2c.h"
#include "gpio.h"

void i2c_init(void)
{
    gpio_set_pin_mode(I2C_SDA_GPIO);
    gpio_set_pin_mode(I2C_SCL_GPIO);
}
