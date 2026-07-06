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

#ifndef ADC_TEST_H_
#define ADC_TEST_H_

#include "device.h"
#include "bsp_adc.h"

#define ADC_BASE            ADCA_BASE

/* ---- ADC 采样窗口（SYSCLK 周期数） ---- */
#define ADC_ACQPS           50U

/* ---- ADC channels ---- */
#define ADC_CH1             ADC_CH_ADCIN15   /* pin5  = A15 */
#define ADC_CH2             ADC_CH_ADCIN11   /* pin6  = A11 */
#define ADC_CH3             ADC_CH_ADCIN0    /* pin7  = A0  */
#define ADC_CH4             ADC_CH_ADCIN12   /* pin8  = A12 */
#define ADC_CH5             ADC_CH_ADCIN8    /* pin9  = A8  */
#define ADC_CH6             ADC_CH_ADCIN9    /* pin13 = A9  */
#define ADC_CH7             28U              /* internal temp*/

#define ADC_SOC_COUNT       7U

/* ---- Result buffer ---- */
extern volatile uint16_t adc_results[ADC_SOC_COUNT];

void adc_test_init(void);
void adc_test_run(void);
void ADC_ReadResults(void);

#endif /* ADC_TEST_H_ */
