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

#include "can_test.h"
#include "bsp_can.h"

/* ---- CAN 收发调试变量 ---- */
MCAN_RxMessage_t can_rx_msg = {0};
uint8_t can_tx_msg[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

/* ---- CAN 接收中断 ---- */
__INTERRUPT void CAN_IRQHandler(void)
{
    if (MCAN_getIntrStatus(CAN_BASE) & MCAN_INT_SRC_RxFIFO0_NEW_MSG) {
        memset((void *)&can_rx_msg, 0, sizeof(can_rx_msg));
        MCAN_receiveMsgFromFifo0(CAN_BASE, &can_rx_msg);
    }
    MCAN_clearIntrStatus(CAN_BASE, MCAN_INT_SRC_RxFIFO0_NEW_MSG);
}

void can_test_init(void)
{
    /* ---- CAN ---- */
    CAN_Clock_Init();
    CAN_GPIO_Init(CAN_TX_GPIO_MUX, CAN_TX_GPIO, CAN_RX_GPIO_MUX, CAN_RX_GPIO);
    CAN_Module_Init(CAN_BASE, CAN_BAUD, CAN_IRQ_NUMBER, &CAN_IRQHandler, false);
}

void can_test_run(void)
{
	/* 初始化发送一帧测试消息到 CAN 总线 */
	CAN_SendMsg(CANA_BASE, 0x123, can_tx_msg, sizeof(can_tx_msg));

	/* 使用 USB 转 CAN 做收发测试，GDB 观察 can_rx_msg*/
}
