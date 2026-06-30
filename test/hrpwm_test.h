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

#ifndef HRPWM_TEST_H_
#define HRPWM_TEST_H_

#include "device.h"
#include "bsp_hrpwm.h"

/* ---- HRPWMA: ª•≤π (GPIO0=EPWM1A, GPIO1=EPWM1B) ---- */
#define HRPWMA_GPIO_A           0
#define HRPWMA_GPIO_B           1
#define HRPWMA_GPIO_A_MUX       GPIO_0_EPWM1_A
#define HRPWMA_GPIO_B_MUX       GPIO_1_EPWM1_B
#define HRPWMA_BASE             EPWM1_BASE
#define HRPWMA_FREQ             20000U
#define HRPWMA_DUTY             0.5002f
#define HRPWMA_DEADTIME_US      1.0f
#if PLAN == 2
#define HRPWMA_PERIOD           (DEVICE_AHBCLK_FREQ / HRPWMA_FREQ)
#define HRPWMA_PHASE            0.0f
#else
#define HRPWMA_PERIOD           (DEVICE_AHBCLK_FREQ / (HRPWMA_FREQ * 2U))
#endif

/* ---- HRPWMB: ∂¿¡¢ (GPIO7=EPWM2A) ---- */
#define HRPWMB_GPIO_A           7
#define HRPWMB_GPIO_A_MUX       GPIO_7_EPWM2_A
#define HRPWMB_BASE             EPWM2_BASE
#define HRPWMB_FREQ             20000U
#define HRPWMB_DUTY             0.5005f
#if PLAN == 2
#define HRPWMB_PERIOD           (DEVICE_AHBCLK_FREQ / HRPWMB_FREQ)
#else
#define HRPWMB_PERIOD           (DEVICE_AHBCLK_FREQ / (HRPWMB_FREQ * 2U))
#endif

extern volatile float hrpwma_duty;
extern volatile float hrpwmb_duty;
extern volatile uint32_t hrpwm_period;

void hrpwm_test_init(void);
void hrpwm_test_run(void);

#endif /* HRPWM_TEST_H_ */
