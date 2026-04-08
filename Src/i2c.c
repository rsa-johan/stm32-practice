#include "i2c.h"

static uint32_t i2c_make_cr2(I2C_Slave slave, uint32_t nbytes, uint32_t mode_flags)
{
    return ((uint32_t)slave << I2C_CR2_SADD_Pos) |
           (nbytes << I2C_CR2_NBYTES_Pos) |
           mode_flags;
}

static bool i2c_wait_for_flag(uint32_t flag)
{
    uint32_t timeout = I2C_TIMEOUT;

    while ((i2c3.registers->ISR & flag) == 0U) {
        if ((i2c3.registers->ISR & I2C_ISR_NACKF) != 0U) {
            i2c3.registers->ICR = I2C_ICR_NACKCF;
            return false;
        }

        if (timeout-- == 0U) {
            return false;
        }
    }

    return true;
}

static bool i2c_wait_while_busy(void)
{
    uint32_t timeout = I2C_TIMEOUT;

    while ((i2c3.registers->ISR & I2C_ISR_BUSY) != 0U) {
        if (timeout-- == 0U) {
            return false;
        }
    }

    return true;
}
void i2c_init(void)
{
    I2C AudioController = I2C_CONFIG(AudioController, I2C_TIMING_100KHZ_16MHZ);
    gpio_set_pin_mode(AudioController.config.sda);
    gpio_set_pin_mode(AudioController.config.scl);

    AudioController.registers->CR1 &= ~I2C_CR1_PE;
    AudioController.registers->TIMINGR = AudioController.config.timing;
    AudioController.registers->ICR = I2C_ICR_STOPCF | I2C_ICR_NACKCF;
    AudioController.registers->CR1 |= I2C_CR1_PE;
}

bool i2c_write_reg(I2C_Slave slave, uint8_t reg, uint8_t value)
{
    if (!i2c_wait_while_busy()) {
        return false;
    }

    i2c3.registers->ICR = I2C_ICR_STOPCF | I2C_ICR_NACKCF;
    i2c3.registers->CR2 = i2c_make_cr2(slave, 2U, I2C_CR2_AUTOEND);
    i2c3.registers->CR2 |= I2C_CR2_START;

    if (!i2c_wait_for_flag(I2C_ISR_TXIS)) {
        return false;
    }
    i2c3.registers->TXDR = reg;

    if (!i2c_wait_for_flag(I2C_ISR_TXIS)) {
        return false;
    }
    i2c3.registers->TXDR = value;

    if (!i2c_wait_for_flag(I2C_ISR_STOPF)) {
        return false;
    }

    i2c3.registers->ICR = I2C_ICR_STOPCF;
    return true;
}

bool i2c_read_reg(I2C_Slave slave, uint8_t reg, uint8_t *buffer, uint32_t len)
{
    uint32_t idx = 0U;

    if ((buffer == 0) || (len == 0U) || (len > I2C_NBYTES_MAX)) {
        return false;
    }

    if (!i2c_wait_while_busy()) {
        return false;
    }

    i2c3.registers->ICR = I2C_ICR_STOPCF | I2C_ICR_NACKCF;
    i2c3.registers->CR2 = i2c_make_cr2(slave, 1U, 0U);
    i2c3.registers->CR2 |= I2C_CR2_START;

    if (!i2c_wait_for_flag(I2C_ISR_TXIS)) {
        return false;
    }
    i2c3.registers->TXDR = reg;

    if (!i2c_wait_for_flag(I2C_ISR_TC)) {
        return false;
    }

    i2c3.registers->CR2 = i2c_make_cr2(slave, len, I2C_CR2_RD_WRN | I2C_CR2_AUTOEND);
    i2c3.registers->CR2 |= I2C_CR2_START;

    for (idx = 0U; idx < len; idx++) {
        if (!i2c_wait_for_flag(I2C_ISR_RXNE)) {
            return false;
        }

        buffer[idx] = (uint8_t)i2c3.registers->RXDR;
    }

    if (!i2c_wait_for_flag(I2C_ISR_STOPF)) {
        return false;
    }

    i2c3.registers->ICR = I2C_ICR_STOPCF;
    return true;
}
