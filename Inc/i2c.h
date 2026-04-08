#ifndef I2C_H
#define I2C_H

#include "gpio.h"
#include "af.h"
#include <stdbool.h>
#include <stdint.h>

#define I2C3_BASE 0x40005C00U

#define I2C_CR1_PE (1U << 0)

#define I2C_CR2_SADD_Pos 0U
#define I2C_CR2_RD_WRN (1U << 10)
#define I2C_CR2_START (1U << 13)
#define I2C_CR2_STOP (1U << 14)
#define I2C_CR2_NBYTES_Pos 16U
#define I2C_CR2_AUTOEND (1U << 25)

#define I2C_ISR_TXIS (1U << 1)
#define I2C_ISR_RXNE (1U << 2)
#define I2C_ISR_NACKF (1U << 4)
#define I2C_ISR_STOPF (1U << 5)
#define I2C_ISR_TC (1U << 6)
#define I2C_ISR_BUSY (1U << 15)

#define I2C_ICR_NACKCF (1U << 4)
#define I2C_ICR_STOPCF (1U << 5)

#define I2C_NBYTES_MAX 255U
#define I2C_TIMEOUT 100000U

#define I2C_TIMING_64KHZ_16MHZ 0x10707DBCU
#define I2C_TIMING_100KHZ_16MHZ 0x40B285C2U

#define I2C_SDA_GPIO GPIO_CONFIG(PORTG, PIN_8, ALTERNATE_FUNCTION, OPEN_DRAIN, NO_PULL, AF_I2C3_SCL)
#define I2C_SCL_GPIO GPIO_CONFIG(PORTG, PIN_7, ALTERNATE_FUNCTION, OPEN_DRAIN, NO_PULL, AF_I2C3_SCL)

typedef enum {
    Audio = 1
} I2C_Slave ;

typedef struct {
    GPIO sda;
    GPIO scl;
    I2C_Slave slave;
    uint32_t timing;
} I2C_Config ;

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t OAR1;
    volatile uint32_t OAR2;
    volatile uint32_t TIMINGR;
    volatile uint32_t TIMEOUTR;
    volatile uint32_t ISR;
    volatile uint32_t ICR;
    volatile uint32_t PECR;
    volatile uint32_t RXDR;
    volatile uint32_t TXDR;
} I2C_Registers;

typedef struct {
    I2C_Registers *registers;
    I2C_Config config;
} I2C ;

#define I2C_CONFIG(slave_, timing_) \
    ((I2C){                         \
        .registers = (I2C_Registers *)I2C3_BASE, \
        .config = {                 \
            .sda = I2C_SDA_GPIO,    \
            .scl = I2C_SCL_GPIO,    \
            .slave = (slave_),      \
            .timing = (timing_)     \
        }                           \
    })

extern const I2C i2c3;

void i2c_init(void);
bool i2c_write_reg(I2C_Slave slave, uint8_t reg, uint8_t value);
bool i2c_read_reg(I2C_Slave slave, uint8_t reg, uint8_t *buffer, uint32_t len);

#endif /* I2C_H */
