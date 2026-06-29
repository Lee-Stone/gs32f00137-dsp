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
#include "bsp_adc.h"

/* ---- ADC 一站式初始化 ---- */
void ADC_Init(uint32_t base)
{
    /* 第1步：时钟使能（由 Device_init 完成） */
    ADC_Clock_Init();
    /* 第2步：ADC 模块初始化 */
    ADC_Module_Init(base);
}

/* ---- ADC 时钟初始化（由 Device_init 完成） ---- */
void ADC_Clock_Init(void)
{
    /*
     * Actual clock initialization is performed in Device_init()
     */
}

/* ---- ADC 模块初始化 ---- */
void ADC_Module_Init(uint32_t base)
{
    /* 设置参考电压：外部 3.3V */
    ADC_setVREF(base, BSP_ADC_REF_MODE, BSP_ADC_REF_VOLT);
    /* 设置 INL 校准 */
    ADC_setINLTrim(base);
    /* 设置 ADC 时钟分频 */
    ADC_setPrescaler(base, BSP_ADC_CLK_DIV);
    /* EOC 脉冲模式 */
    ADC_setInterruptPulseMode(base, ADC_PULSE_END_OF_CONV);
    /* 使能温度传感器 */
    ASysCtl_enableTemperatureSensor();
    /* 使能 ADC 转换器 */
    ADC_enableConverter(base);

    /* 延时 5ms 等待 ADC 上电稳定 */
    DEVICE_DELAY_US(5000);

    /* 禁止突发模式 */
    ADC_disableBurstMode(base);
    /* SOC 优先级：轮询模式 */
    ADC_setSOCPriority(base, ADC_PRI_ALL_ROUND_ROBIN);
}

/* ---- DMA 初始化：ADCINT2 触发 DMA 自动搬运结果 ---- */
void ADC_DMA_Init(uint16_t *dstBuf, uint16_t count)
{
    DMA_initController(DMA_BASE);
    DMA_enableInterrupt(DMA_BASE);

    DMA_ConfigParams dmaCfg = {0};
    dmaCfg.enableInterrupt  = 1;
    dmaCfg.dmaSrcReqId      = DMAMUX_ReqId_Trig_ADCA_2int;
    dmaCfg.srcAddr          = (uint32_t)ADCARESULT_BASE;
    dmaCfg.destAddr         = (uint32_t)dstBuf;
    dmaCfg.blockTS          = count;
    dmaCfg.ttfc             = DMA_TT_FC_2_P2M_DMAC;
    dmaCfg.srcBtl           = DMA_BTL_1;
    dmaCfg.destBtl          = DMA_BTL_1;
    dmaCfg.reloadSrc        = true;
    dmaCfg.reloadDst        = true;
    dmaCfg.srcAddrDirect    = DMA_ADDR_INCRE;
    dmaCfg.destAddrDirect   = DMA_ADDR_INCRE;
    dmaCfg.srcTrWidthBytes  = DMA_TR_WIDTH_BYTE_2;
    dmaCfg.destTrWidthBytes = DMA_TR_WIDTH_BYTE_2;

    DMA_configChannel(DMA_CH1_BASE, &dmaCfg);
    DMA_clearInterrupt(DMA_CH1_BASE, DMA_INT_BLOCK);
    DMA_unMaskInterrupt(DMA_CH1_BASE, DMA_INT_BLOCK);

    DMA_clearTriggerFlag(DMA_CH1_BASE);

    DMA_startChannel(DMA_CH1_BASE);
}
