/*
 * dma.h
 *
 *  Created on: Dec 8, 2019
 *      Author: rucha
 */

#ifndef DMA_H_
#define DMA_H_


#include "cir_buffer.h"

void DMA(void);
void transfer_DMA(ring_buffer *src,uint16_t *dest,uint8_t transfer_data);

#endif /* DMA_H_ */
