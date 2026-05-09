#include "i2c.h"

#define I2C_CR1_PE (1U << 0)
#define I2C_CR1_RXIE (1U << 2)
#define I2C_CR1_TCIE (1U << 6)
#define I2C_CR1_ANFOFF (1U << 12)
#define I2C_CR1_NOSTRETCH (1U << 17)

#define I2C_CR2_SADD_MASK (0x3FFU << 0)
#define I2C_CR2_RD_WRN (1U << 10)
#define I2C_CR2_START (1U << 13)
#define I2C_CR2_NBYTES_MASK (0xFFU << 16)
#define I2C_CR2_NBYTES(value) (((value) & 0xFFU) << 16)
#define I2C_CR2_AUTOEND (1U << 25)
#define I2C_CR2_TRANSFER_MASK \
    (I2C_CR2_SADD_MASK | I2C_CR2_RD_WRN | I2C_CR2_NBYTES_MASK | I2C_CR2_AUTOEND)

#define I2C_ISR_TXIS (1U << 1)
#define I2C_ISR_RXNE (1U << 2)
#define I2C_ISR_NACKF (1U << 4)
#define I2C_ISR_STOPF (1U << 5)
#define I2C_ISR_BUSY (1U << 15)

#define I2C_ICR_NACKCF (1U << 4)
#define I2C_ICR_STOPCF (1U << 5)

static I2C i2c3_audio = I2C_CONFIG(
    I2C_ADDR_AUDIO,
    I2C_TIMING_100KHZ_16MHZ,
    I2C_MODE_CONTROLLER,
    false
);

static inline void i2c_disable(I2C_Registers *regs)
{
    regs->CR1 &= ~I2C_CR1_PE;
}

static inline void i2c_enable(I2C_Registers *regs)
{
    regs->CR1 |= I2C_CR1_PE;
}

static inline void i2c_clear_flags(I2C_Registers *regs)
{
    regs->ICR = I2C_ICR_NACKCF | I2C_ICR_STOPCF;
}

static bool i2c_wait_clear(const I2C_Registers *regs, uint32_t mask)
{
    uint32_t timeout = I2C_TIMEOUT;

    while ((regs->ISR & mask) != 0U) {
        if (timeout-- == 0U) {
            return false;
        }
    }

    return true;
}

static bool i2c_wait_set_or_error(const I2C_Registers *regs, uint32_t mask)
{
    uint32_t timeout = I2C_TIMEOUT;

    while ((regs->ISR & mask) == 0U) {
        const uint32_t status = regs->ISR;

        if ((status & (I2C_ISR_NACKF | I2C_ISR_STOPF)) != 0U) {
            return false;
        }

        if (timeout-- == 0U) {
            return false;
        }
    }

    return true;
}

static void i2c_start_transfer(I2C *dev, uint32_t size, bool read)
{
    I2C_Registers *regs = dev->registers;
    uint32_t cr2 = regs->CR2 & ~I2C_CR2_TRANSFER_MASK;

    cr2 |= ((uint32_t)dev->config.address & I2C_CR2_SADD_MASK);
    cr2 |= I2C_CR2_NBYTES(size) | I2C_CR2_AUTOEND;

    if (read) {
        cr2 |= I2C_CR2_RD_WRN;
    }

    regs->CR2 = cr2 | I2C_CR2_START;
}

static __SIG_T i2c_finish_transfer(I2C_Registers *regs, __SIG_T error)
{
    if (!i2c_wait_set_or_error(regs, I2C_ISR_STOPF)) {
        error = (error == I2C_READ_ERROR) ? I2C_READ_ERROR : I2C_WRITE_ERROR;
    }

    i2c_clear_flags(regs);
    return error;
}

__SIG_T i2c_setup(I2C *dev)
{
    I2C_Registers *regs = dev->registers;
    uint32_t cr1 = regs->CR1;

    gpio_set_pin_mode(dev->config.sda);
    gpio_set_pin_mode(dev->config.scl);

    i2c_disable(regs);

    cr1 &= ~(I2C_CR1_PE | I2C_CR1_ANFOFF | I2C_CR1_NOSTRETCH | I2C_CR1_RXIE | I2C_CR1_TCIE);

    if (dev->config.mode == I2C_MODE_TARGET) {
        cr1 |= I2C_CR1_NOSTRETCH;
    }

    if (dev->config.enable_interrupts) {
        cr1 |= I2C_CR1_RXIE | I2C_CR1_TCIE;
    }

    regs->CR1 = cr1;
    regs->TIMINGR = dev->config.timing;
    i2c_clear_flags(regs);
    i2c_enable(regs);

    return NO_ERROR;
}

__SIG_T i2c_write(I2C *dev, const uint8_t *data, uint32_t size)
{
    I2C_Registers *regs = dev->registers;

    if ((data == 0) || (size == 0U) || (size > I2C_NBYTES_MAX)) {
        return I2C_WRITE_ERROR;
    }

    if (!i2c_wait_clear(regs, I2C_ISR_BUSY)) {
        return I2C_WRITE_ERROR;
    }

    i2c_clear_flags(regs);
    i2c_start_transfer(dev, size, false);

    for (uint32_t i = 0; i < size; ++i) {
        if (!i2c_wait_set_or_error(regs, I2C_ISR_TXIS)) {
            i2c_clear_flags(regs);
            return I2C_WRITE_ERROR;
        }

        regs->TXDR = data[i];
    }

    return i2c_finish_transfer(regs, NO_ERROR);
}

__SIG_T i2c_read(I2C *dev, uint8_t *data, uint32_t size)
{
    I2C_Registers *regs = dev->registers;

    if ((data == 0) || (size == 0U) || (size > I2C_NBYTES_MAX)) {
        return I2C_READ_ERROR;
    }

    if (!i2c_wait_clear(regs, I2C_ISR_BUSY)) {
        return I2C_READ_ERROR;
    }

    i2c_clear_flags(regs);
    i2c_start_transfer(dev, size, true);

    for (uint32_t i = 0; i < size; ++i) {
        if (!i2c_wait_set_or_error(regs, I2C_ISR_RXNE)) {
            i2c_clear_flags(regs);
            return I2C_READ_ERROR;
        }

        data[i] = (uint8_t)regs->RXDR;
    }

    return i2c_finish_transfer(regs, NO_ERROR);
}

void i2c_init(void)
{
    (void)i2c_setup(&i2c3_audio);
}
