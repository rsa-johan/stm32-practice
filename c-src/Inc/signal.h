#ifndef SIGNAL_H
#define SIGNAL_H

#include <stdint.h>

/* Error signals */
typedef enum {
    NO_ERROR = 0,
    /* I2C */
    I2C_READ_ERROR,
    I2C_WRITE_ERROR,
} __SIG_T;

#endif /* SIGNAL_H */
