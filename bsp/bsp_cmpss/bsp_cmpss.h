/*
 *  Copyright (c) Gejian Semiconductors 2023
 *  All rights reserved.
 */

#ifndef BSP_CMPSS_H_
#define BSP_CMPSS_H_

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"

void CMPSS_GPIO_Init(uint32_t pin, uint32_t hpMuxSelect, uint32_t hpChannel,
                     uint32_t lpMuxSelect, uint32_t lpChannel);
void CMPSS_Module_Init(uint32_t base, uint16_t dacHigh, uint16_t dacLow);

#endif /* BSP_CMPSS_H_ */
