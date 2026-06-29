/*
 *   Copyright (c) Gejian Semiconductors 2023
 *   All rights reserved.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef BSP_SCI_H_
#define BSP_SCI_H_

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"

/* ---- Base + IRQ ---- */
#define BSP_SCI_BASE            SCIA_BASE
#define BSP_SCI_IRQ_NUMBER      INT_SCIA_RX

/* ---- Pin definitions ---- */
#define BSP_SCI_TX_GPIO             29
#define BSP_SCI_RX_GPIO             28
#define BSP_SCI_TX_GPIO_MUX         GPIO_29_SCIA_TX
#define BSP_SCI_RX_GPIO_MUX         GPIO_28_SCIA_RX

/* ---- DMA switch ---- */
#define BSP_SCI_DMA                 0

void SCI_Init(uint32_t base, uint32_t baud, IRQn_Type IRQn, IrqHandlerType handler);
void SCI_Clock_Init(void);
void SCI_GPIO_Init(uint32_t tx_mux, uint32_t tx_pin, uint32_t rx_mux, uint32_t rx_pin);
void SCI_Module_Init(uint32_t base, uint32_t baud, IRQn_Type IRQn, IrqHandlerType handler);

#if BSP_SCI_DMA
#define DMAMUX_SCI_TX           DMAMUX_ReqId_scia_tx
#define DMAMUX_SCI_RX           DMAMUX_ReqId_scia_rx
#define SCI_DMA_IRQ_NUMBER      INT_DMA1_CH1
#define SCI_DMA_BASE            DMA1_BASE
#define SCI_DMA_TX_CH           DMA1_CH1_BASE
#define SCI_DMA_RX_CH           DMA1_CH2_BASE
#define SCI_DMA_BUF_SIZE        32
extern uint8_t SciDmaBuf[SCI_DMA_BUF_SIZE];

void SCI_DMA_Init(uint32_t sci_base);
void SCI_DMA_TX_Init(uint32_t sci_base);
void SCI_DMA_RX_Init(uint32_t sci_base);
void DMA_Send_SCI_Data(uint32_t dma_base, uint32_t sci_base, IRQn_Type IRQn,
                       uint32_t srcaddr, uint32_t srcdata_length);
void DMA_Recv_SCI_Data(uint32_t dma_base, uint32_t sci_base, IRQn_Type IRQn,
                       uint32_t destaddr, uint32_t destdata_length);
#endif

#endif /* BSP_SCI_H_ */
