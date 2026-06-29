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

#include "adc_test.h"

/* ---- ADC result buffer (DMA 自动填充) ---- */
volatile uint16_t adc_results[ADC_SOC_COUNT];

/* ---- DMA 中断：传输完成后置标志 ---- */
__interrupt void ADC_DMA_IRQHandler(void)
{
    DMA_clearInterrupt(DMA_CH1_BASE, DMA_INT_BLOCK);
}

/* ---- ADC test init: SOC 链 INT1 串联 0~5，SOC6 → INT2 → DMA ---- */
void adc_test_init(void)
{
    /* ---- Module base init ---- */
    ADC_Init(ADC_BASE);

    /* ---- SOC0~SOC5: INT1 链式触发 ---- */
    ADC_setupSOC(ADC_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY, ADC_CH1, ADC_ACQPS);
    ADC_setInterruptSOCTrigger(ADC_BASE, ADC_SOC_NUMBER0, ADC_INT_SOC_TRIGGER_ADCINT1);

    ADC_setupSOC(ADC_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_SW_ONLY, ADC_CH2, ADC_ACQPS);
    ADC_setInterruptSOCTrigger(ADC_BASE, ADC_SOC_NUMBER1, ADC_INT_SOC_TRIGGER_ADCINT1);

    ADC_setupSOC(ADC_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_SW_ONLY, ADC_CH3, ADC_ACQPS);
    ADC_setInterruptSOCTrigger(ADC_BASE, ADC_SOC_NUMBER2, ADC_INT_SOC_TRIGGER_ADCINT1);

    ADC_setupSOC(ADC_BASE, ADC_SOC_NUMBER3, ADC_TRIGGER_SW_ONLY, ADC_CH4, ADC_ACQPS);
    ADC_setInterruptSOCTrigger(ADC_BASE, ADC_SOC_NUMBER3, ADC_INT_SOC_TRIGGER_ADCINT1);

    ADC_setupSOC(ADC_BASE, ADC_SOC_NUMBER4, ADC_TRIGGER_SW_ONLY, ADC_CH5, ADC_ACQPS);
    ADC_setInterruptSOCTrigger(ADC_BASE, ADC_SOC_NUMBER4, ADC_INT_SOC_TRIGGER_ADCINT1);

    ADC_setupSOC(ADC_BASE, ADC_SOC_NUMBER5, ADC_TRIGGER_SW_ONLY, ADC_CH6, ADC_ACQPS);
    ADC_setInterruptSOCTrigger(ADC_BASE, ADC_SOC_NUMBER5, ADC_INT_SOC_TRIGGER_ADCINT1);

    /* ---- SOC6: 最后一轮 EOC → ADCINT2 → DMA ---- */
    ADC_setupSOC(ADC_BASE, ADC_SOC_NUMBER6, ADC_TRIGGER_SW_ONLY, ADC_CH7, ADC_ACQPS);
    ADC_setInterruptSOCTrigger(ADC_BASE, ADC_SOC_NUMBER6, ADC_INT_SOC_TRIGGER_ADCINT2);

    /* ---- ADCINT1: SOC0 EOC 触发，Continuous 模式链到 SOC1~SOC5 ---- */
    ADC_setInterruptSource(ADC_BASE, ADC_INT_NUMBER1, ADC_INT_TRIGGER_EOC0);
    ADC_enableInterrupt(ADC_BASE, ADC_INT_NUMBER1);
    ADC_clearInterruptStatus(ADC_BASE, ADC_INT_NUMBER1);
    ADC_enableContinuousMode(ADC_BASE, ADC_INT_NUMBER1);

    /* ---- ADCINT2: SOC6 EOC 触发 → DMAMUX → DMA_CH1 ---- */
    ADC_setInterruptSource(ADC_BASE, ADC_INT_NUMBER2, ADC_INT_TRIGGER_EOC6);
    ADC_enableInterrupt(ADC_BASE, ADC_INT_NUMBER2);
    ADC_clearInterruptStatus(ADC_BASE, ADC_INT_NUMBER2);
    ADC_enableContinuousMode(ADC_BASE, ADC_INT_NUMBER2);

    /* ---- DMA 初始化 ---- */
    ADC_DMA_Init((uint16_t *)adc_results, ADC_SOC_COUNT);
    /* ---- 注册 DMA 中断 ---- */
    Interrupt_register(INT_DMA1_CH1, &ADC_DMA_IRQHandler);
    Interrupt_enable(INT_DMA1_CH1);

    /* ---- 首次软件强制触发 ---- */
    ADC_forceMultipleSOC(ADC_BASE, (1U << ADC_SOC_COUNT) - 1U);
}

void adc_test_run(void)
{
	/* GDB 查看 adc_results 值 */
}

/* ---- 温度值 ---- */
volatile int16_t adc_temp_celsius;

/* ---- 50us CPUTimer ISR ---- */
void ADC_ReadResults(void)
{
	/* adc_results 处理 */

    /* 温度通道转摄氏度 */
    adc_temp_celsius = ADC_getTemperatureC(adc_results[6], 3.3);
}
