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

#include "device.h"
#include "driverlib.h"
#include "board_cfg.h"
#include "bsp_sci.h"

void SCI_Init(uint32_t base, uint32_t baud, IRQn_Type IRQn, IrqHandlerType handler)
{
    /* 第1步：使能外设时钟 */
    SCI_Clock_Init();
    /* 第2步：配置引脚复用 */
    SCI_GPIO_Init(BSP_SCI_TX_GPIO_MUX, BSP_SCI_TX_GPIO, BSP_SCI_RX_GPIO_MUX, BSP_SCI_RX_GPIO);
    /* 第3步：配置外设寄存器 */
    SCI_Module_Init(base, baud, IRQn, handler);
}

void SCI_Clock_Init(void)
{
    /*
     * Actual clock initialization is performed in Device_init()
     * */
}

void SCI_GPIO_Init(uint32_t tx_mux, uint32_t tx_pin, uint32_t rx_mux, uint32_t rx_pin)
{
    /* 设置 TX 引脚为上拉模式 */
    GPIO_setPadConfig(tx_pin, GPIO_PIN_TYPE_PULLUP);
    /* 设置 RX 引脚为上拉模式 */
    GPIO_setPadConfig(rx_pin, GPIO_PIN_TYPE_PULLUP);
    /* 将 GPIO 复用为 TX 功能 */
    GPIO_setPinConfig(tx_mux);
    /* 将 GPIO 复用为 RX 功能 */
    GPIO_setPinConfig(rx_mux);
}

void SCI_Module_Init(uint32_t base, uint32_t baud, IRQn_Type IRQn, IrqHandlerType handler)
{
    /* 软件复位，进入空闲状态 */
    SCI_performSoftwareReset(base);
    /* 配置数据格式（8数据位、1停止位、无校验）和波特率 */
    SCI_setConfig(base, DEVICE_APBCLK_FREQ, baud, (SCI_CONFIG_WLEN_8 | SCI_CONFIG_STOP_ONE | SCI_CONFIG_PAR_NONE));
    /* 复位通道 */
    SCI_resetChannels(base);
    /* 复位 RX/TX FIFO */
    SCI_resetRxFIFO(base);
    SCI_resetTxFIFO(base);
    /* 清除 FIFO 中断标志 */
    SCI_clearInterruptStatus(base, SCI_INT_TXFF | SCI_INT_RXFF);
    /* 使能 SCI 模块 */
    SCI_enableModule(base);
    /* 使能 FIFO */
    SCI_enableFIFO(base);
    /* 使能 TX/RX FIFO 中断 */
    SCI_enableInterrupt(base, SCI_INT_TXFF | SCI_INT_RXFF);

#if BSP_SCI_DMA
    /* DMA 模式：FIFO 水位 TX0/RX1 */
    SCI_setFIFOInterruptLevel(base, SCI_FIFO_TX0, SCI_FIFO_RX1);
    SCI_DMA_Init(base);
    Interrupt_SetPriority(IRQn, 0, 0);
    Interrupt_register(IRQn, handler);
    Interrupt_disable(IRQn);
#else
    /* 中断模式：FIFO 水位 TX0/RX1 */
    SCI_setFIFOInterruptLevel(base, SCI_FIFO_TX0, SCI_FIFO_RX1);
    Interrupt_register(IRQn, handler);
    Interrupt_enable(IRQn);
#endif

    /* 再次软件复位，确保配置生效 */
    SCI_performSoftwareReset(base);
}

#if BSP_SCI_DMA

void SCI_DMA_Init(uint32_t sci_base)
{
    /* 初始化 DMA1 控制器 */
    DMA_initController(SCI_DMA_BASE);
    /* 初始化 TX DMA 通道（内存→SCI_THR） */
    SCI_DMA_TX_Init(sci_base);
    /* 初始化 RX DMA 通道（SCI_RBR→内存） */
    SCI_DMA_RX_Init(sci_base);
}

uint8_t SciDmaBuf[SCI_DMA_BUF_SIZE] = {0};

void SCI_DMA_TX_Init(uint32_t sci_base)
{
    DMA_ConfigParams txDmaCfg = {0};
    /* 停止 TX DMA 通道 */
    DMA_stopChannel(SCI_DMA_TX_CH);
    /* 使能传输完成中断 */
    txDmaCfg.enableInterrupt = 1;
    /* 目标请求 ID：SCI TX */
    txDmaCfg.dmaDstReqId = DMAMUX_SCI_TX;
    /* 源地址：DMA 发送缓冲区 */
    txDmaCfg.srcAddr = (uint32_t)(SciDmaBuf);
    /* 目标地址：SCI 发送保持寄存器 */
    txDmaCfg.destAddr = (uint32_t)(sci_base + SCI_O_RBR_THR_DLL);
    /* 块传输大小 */
    txDmaCfg.blockTS = SCI_DMA_BUF_SIZE;
    /* 传输类型：内存到外设 */
    txDmaCfg.ttfc    = DMA_TT_FC_1_M2P_DMAC;
    /* 源 burst 长度 */
    txDmaCfg.srcBtl  = DMA_BTL_1;
    /* 禁止源地址重载 */
    txDmaCfg.reloadSrc = false;
    /* 目标 burst 长度 */
    txDmaCfg.destBtl = DMA_BTL_1;
    /* 源地址自增 */
    txDmaCfg.srcAddrDirect = DMA_ADDR_INCRE;
    /* 目标地址不变（固定指向 THR 寄存器） */
    txDmaCfg.destAddrDirect = DMA_ADDR_NO_CHANGE;
    /* 源传输宽度：1 字节 */
    txDmaCfg.srcTrWidthBytes = DMA_TR_WIDTH_BYTE_1;
    /* 目标传输宽度：1 字节 */
    txDmaCfg.destTrWidthBytes= DMA_TR_WIDTH_BYTE_1;
    /* 应用配置 */
    DMA_configChannel(SCI_DMA_TX_CH, &txDmaCfg);
    /* 清除传输完成中断标志 */
    DMA_clearInterrupt(SCI_DMA_TX_CH, DMA_INT_TFR);
    /* 取消屏蔽传输完成中断 */
    DMA_unMaskInterrupt(SCI_DMA_TX_CH, DMA_INT_TFR);
}

void SCI_DMA_RX_Init(uint32_t sci_base)
{
    DMA_ConfigParams rxDmaCfg = {0};
    /* 停止 RX DMA 通道 */
    DMA_stopChannel(SCI_DMA_RX_CH);
    /* 使能传输完成中断 */
    rxDmaCfg.enableInterrupt = 1;
    /* 源请求 ID：SCI RX */
    rxDmaCfg.dmaSrcReqId = DMAMUX_SCI_RX;
    /* 源地址：SCI 接收缓冲寄存器 */
    rxDmaCfg.srcAddr = (uint32_t)(sci_base + SCI_O_RBR_THR_DLL);
    /* 目标地址：DMA 接收缓冲区 */
    rxDmaCfg.destAddr = (uint32_t)SciDmaBuf;
    /* 块传输大小 */
    rxDmaCfg.blockTS = SCI_DMA_BUF_SIZE;
    /* 传输类型：外设到内存 */
    rxDmaCfg.ttfc = DMA_TT_FC_2_P2M_DMAC;
    /* 源 burst 长度 */
    rxDmaCfg.srcBtl  = DMA_BTL_1;
    /* 禁止目标地址重载 */
    rxDmaCfg.reloadDst = false;
    /* 目标 burst 长度 */
    rxDmaCfg.destBtl = DMA_BTL_1;
    /* 源地址不变（固定指向 RBR 寄存器） */
    rxDmaCfg.srcAddrDirect = DMA_ADDR_NO_CHANGE;
    /* 目标地址自增 */
    rxDmaCfg.destAddrDirect = DMA_ADDR_INCRE;
    /* 源传输宽度：1 字节 */
    rxDmaCfg.srcTrWidthBytes = DMA_TR_WIDTH_BYTE_1;
    /* 目标传输宽度：1 字节 */
    rxDmaCfg.destTrWidthBytes= DMA_TR_WIDTH_BYTE_1;
    /* 应用配置 */
    DMA_configChannel(SCI_DMA_RX_CH, &rxDmaCfg);
    /* 清除传输完成中断标志 */
    DMA_clearInterrupt(SCI_DMA_RX_CH, DMA_INT_TFR);
    /* 取消屏蔽传输完成中断 */
    DMA_unMaskInterrupt(SCI_DMA_RX_CH, DMA_INT_TFR);
}

void DMA_Send_SCI_Data(uint32_t dma_base, uint32_t sci_base, IRQn_Type IRQn, uint32_t srcaddr, uint32_t srcdata_length)
{
    /* 停止 DMA 通道 */
    DMA_stopChannel(dma_base);
    /* 清除传输完成中断标志 */
    DMA_clearInterrupt(dma_base, DMA_INT_TFR);
    /* 配置目标地址（SCI THR）和源地址 */
    DMA_configAddresses(dma_base, (uint32_t)(sci_base + SCI_O_RBR_THR_DLL), srcaddr);
    /* 配置传输字节数 */
    DMA_configBlockTs(dma_base, srcdata_length);
    /* 启动 DMA 传输 */
    DMA_startChannel(dma_base);
    /* 等待传输完成 */
    while((DMA_getInterruptStatus(dma_base) & DMA_INT_TFR) == 0);
    /* 停止 DMA 通道 */
    DMA_stopChannel(dma_base);
    /* 传输完成，使能 SCI 中断 */
    Interrupt_enable(IRQn);
}

void DMA_Recv_SCI_Data(uint32_t dma_base, uint32_t sci_base, IRQn_Type IRQn,
                       uint32_t destaddr, uint32_t destdata_length)
{
    /* 配置源地址（SCI RBR）和目标地址 */
    DMA_configAddresses(dma_base, destaddr, (uint32_t)(sci_base + SCI_O_RBR_THR_DLL));
    /* 配置传输字节数 */
    DMA_configBlockTs(dma_base, destdata_length);
    /* 启动 DMA 接收 */
    DMA_startChannel(dma_base);
    /* 使能 SCI 中断（配合超时检测） */
    Interrupt_enable(IRQn);
    /* 等待 DMA 传输完成 或 SCI 接收超时 */
    while(!(((DMA_getInterruptStatus(dma_base) & DMA_INT_TFR) == DMA_INT_TFR) ||
            (SCI_getInterruptStatus(sci_base) & SCI_RX_DATA_AVAILABLE_INT)));
    /* 停止 DMA 通道 */
    DMA_stopChannel(dma_base);
}

#endif
