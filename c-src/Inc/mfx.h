#ifndef MFX_H
#define MFX_H

#include <stdint.h>

#define MFX_REG_ADDR_GPIO_SET_1 0x6CU
#define MFX_REG_ADDR_GPIO_SET_2 0x6DU
#define MFX_REG_ADDR_GPIO_SET_3 0x6EU

#define MFX_REG_ADDR_GPIO_CLR_1 0x70U
#define MFX_REG_ADDR_GPIO_CLR_2 0x71U
#define MFX_REG_ADDR_GPIO_CLR_3 0x72U

typedef struct {
} mfx_config;

typedef enum {
    MFX_LED_2,
    MFX_LED_4,
    MFX_NONE
} MFX_IO_Component ;

mfx_config get_mfx_config(MFX_IO_Component component)
{
    mfx_config config = {0};
    switch (component) {
        case MFX_LED_2:
        case MFX_LED_4:
            break;
        case MFX_NONE:
            break;
    }

    return config;
}

#endif /* MFX_H */