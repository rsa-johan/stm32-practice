#ifndef FLASH_H
#define FLASH_H

#ifdef __cplusplus
extern "C" {    
#endif


#include <stdint.h>

#define FLASH_BASE (*(volatile uint32_t *)0x40022000U)

#ifdef __cplusplus
}
#endif
#endif /* FLASH_H */