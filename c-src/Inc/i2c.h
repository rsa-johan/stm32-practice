#ifndef I2C_H
#define I2C_H

#include "gpio.h"
#include "af.h"
#include "signal.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define I2C3_BASE 0x40005C00U

#define I2C_NBYTES_MAX 255U
#define I2C_TIMEOUT 100000U

#define I2C_TIMING_64KHZ_16MHZ 0x10707DBCU
#define I2C_TIMING_100KHZ_16MHZ 0x40B285C2U

#define I2C_SDA_GPIO GPIO_CONFIG(PORTG, PIN_8, ALTERNATE_FUNCTION, OPEN_DRAIN, NO_PULL, AF_I2C3_SCL)
#define I2C_SCL_GPIO GPIO_CONFIG(PORTG, PIN_7, ALTERNATE_FUNCTION, OPEN_DRAIN, NO_PULL, AF_I2C3_SCL)

typedef enum {
    I2C_MODE_CONTROLLER,
    I2C_MODE_TARGET,
} I2C_Mode;

typedef enum {
    I2C_ADDR_AUDIO = 0x34U,
} I2C_Address;

typedef struct {
    GPIO sda;
    GPIO scl;
    I2C_Address address;
    uint32_t timing;
    I2C_Mode mode;
    bool enable_interrupts;
} I2C_Config;

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
} I2C;

#define I2C_CONFIG(address_, timing_, mode_, interrupts_)       \
    ((I2C){                                                     \
        .registers = (I2C_Registers *)I2C3_BASE,                \
        .config = {                                             \
            .sda = I2C_SDA_GPIO,                                \
            .scl = I2C_SCL_GPIO,                                \
            .address = (address_),                              \
            .timing = (timing_),                                \
            .mode = (mode_),                                    \
            .enable_interrupts = (interrupts_)                  \
        }                                                       \
    })

void i2c_init(void);
__SIG_T i2c_setup(I2C *dev);
__SIG_T i2c_write(I2C *dev, const uint8_t *data, uint32_t size);
__SIG_T i2c_read(I2C *dev, uint8_t *data, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif /* I2C_H */
