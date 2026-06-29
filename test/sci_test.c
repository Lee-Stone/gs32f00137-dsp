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

#include "sci_test.h"
#include "bsp_sci.h"

uint16_t data = 0;

/* ---- SCI 环回接收中断 ---- */
__INTERRUPT void SCI_IRQHandler(void)
{
    uint32_t status = SCI_getInterruptStatus(SCI_BASE);
    if (status & SCI_INT_RXFF) {
        uint16_t rx_data = SCI_readCharNonBlocking(SCI_BASE);
        SCI_writeCharNonBlocking(SCI_BASE, rx_data);
        SCI_clearInterruptStatus(SCI_BASE, SCI_INT_RXFF);
    }
}

/* ---- RS485 接收中断 ---- */
__INTERRUPT void RS485_IRQHandler(void)
{
    uint32_t status = SCI_getInterruptStatus(RS485_BASE);
    if (status & SCI_INT_RXFF) {
    	data = SCI_readCharNonBlocking(RS485_BASE);
        SCI_writeCharNonBlocking(RS485_BASE, data);
        SCI_clearInterruptStatus(RS485_BASE, SCI_INT_RXFF);
    }
}

void sci_test_init(void)
{
    /* ---- SCI (UART) ---- */
    SCI_GPIO_Init(SCI_TX_GPIO_MUX, SCI_TX_GPIO, SCI_RX_GPIO_MUX, SCI_RX_GPIO);
    SCI_Module_Init(SCI_BASE, SCI_BAUD, SCI_IRQ_NUMBER, &SCI_IRQHandler);

    /* ---- RS485 (SCIC) ---- */
    SCI_GPIO_Init(RS485_TX_GPIO_MUX, RS485_TX_GPIO, RS485_RX_GPIO_MUX, RS485_RX_GPIO);
    SCI_Module_Init(RS485_BASE, RS485_BAUD, RS485_IRQ_NUMBER, &RS485_IRQHandler);
}

void sci_test_run(void)
{
    /* 环回测试：上位机发送 → MCU 回显 */
}
