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

#ifndef BSP_CAN_H_
#define BSP_CAN_H_

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"

/* ---- Base + IRQ ---- */
#define BSP_CAN_BASE            CANA_BASE
#define BSP_CAN_IRQ_NUMBER      INT_CANA0               /* V30: MCAN Interrupt Line 0 = 214 */

/* ---- Pin definitions (32Pin: GPIO32 TX, GPIO11 RX) ---- */
#define BSP_CAN_TX_GPIO             32
#define BSP_CAN_RX_GPIO             11
#define BSP_CAN_TX_GPIO_MUX         GPIO_32_CANA_TX
#define BSP_CAN_RX_GPIO_MUX         GPIO_11_CANA_RX

/* ---- CAN config ---- */
#define BSP_CAN_BITRATE             500000U             /* 500 Kbps */

void CAN_Clock_Init(void);
void CAN_GPIO_Init(uint32_t tx_mux, uint32_t tx_pin, uint32_t rx_mux, uint32_t rx_pin);
void CAN_Module_Init(uint32_t base, uint32_t bitrate, IRQn_Type IRQn, IrqHandlerType handler, bool loopback);

bool CAN_SendMsg(uint32_t base, uint32_t msgId, uint8_t *pData, uint8_t dlc);

#endif /* BSP_CAN_H_ */
