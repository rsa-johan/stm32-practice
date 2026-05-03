#include "i2c.h"
#include "signal.h"
#include <cstdint>
#include <machine/endian.h>
#include <stdint.h>

static inline void clear_enable(I2C i2c_device) 
{
    i2c_device.registers->CR1 &= ~I2C_CR1_PE;
}

static inline void set_enable(I2C i2c_device)
{
    i2c_device.registers->CR1 |= I2C_CR1_PE;
}

static inline uint32_t read_enable(I2C i2c_device)
{
    return (i2c_device.registers->CR1 | I2C_CR1_PE) >> 0;
}

static inline void immediate_reset(I2C i2c_device)
{
    clear_enable(i2c_device);
    while(read_enable(i2c_device) == 1U);
    set_enable(i2c_device);
}

static inline void analog_filter_enable(I2C i2c_device) 
{
    i2c_device.registers->CR1 |= I2C_CR1_ANF;
}

static inline void set_timing_r(I2C i2c_device)
{
    i2c_device.registers->TIMINGR = i2c_device.config.timing;
}

static inline void clear_nostretch(I2C i2c_device)
{
    i2c_device.registers->CR1 &= ~I2C_CR1_NOS;
}

static inline void enable_nostretch(I2C i2c_device)
{
    if (i2c_device.config.mode != Target) return;
    i2c_device.registers->CR1 |= I2C_CR1_NOS;
}

__SIG_T write_data(I2C* self, uint8_t data[], uint32_t size)
{
    for (int i = 0; i < size; i++)
    {
        self->registers->TXDR = data[i];
    }
}

__SIG_T read_data(I2C* self, uint32_t* data)
{
    return NO_ERROR;
}

__SIG_T async_read_data(I2C* self, uint32_t* data)
{
    return NO_ERROR;
}

__SIG_T setup(I2C* self)
{
    gpio_set_pin_mode(self->config.sda);
    gpio_set_pin_mode(self->config.scl);

    self->registers->CR1 |= I2C_CR1_RXIE(self->config.non_blocking);
    self->registers->CR1 |= I2C_CR1_TCIE(self->config.non_blocking);
    self->registers->CR2 |= I2C_CR2_AUTOEND;
    
    return NO_ERROR;
}

void i2c_init(void)
{
    I2C i2c_device = I2C_CONFIG(Audio, I2C_TIMING_100KHZ_16MHZ, Controller, NonBlocking);
    i2c_device.setup = setup;
    i2c_device.read_data = read_data;
    i2c_device.write_data = write_data;

    clear_enable(i2c_device);
    i2c_device.setup(&i2c_device);
    analog_filter_enable(i2c_device);
    set_timing_r(i2c_device);
    clear_nostretch(i2c_device);
}