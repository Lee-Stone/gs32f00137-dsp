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

#ifndef EPWM_TEST_H_
#define EPWM_TEST_H_

#include "device.h"
#include "bsp_epwm.h"

/* ---- EPWMA: 互补 (GPIO0=EPWM1A, GPIO1=EPWM1B) ---- */
#define EPWMA_GPIO_A            0
#define EPWMA_GPIO_B            1
#define EPWMA_GPIO_A_MUX        GPIO_0_EPWM1_A
#define EPWMA_GPIO_B_MUX        GPIO_1_EPWM1_B
#define EPWMA_BASE              EPWM1_BASE
#define EPWMA_FREQ              20000U	// 频率20Khz
#define EPWMA_DUTY_A            0.50f	// 占空比 50%
#define EPWMA_DUTY_B            0.50f	// 占空比 50%
#define EPWMA_DEADTIME_US       1.0f	// 死区 1us
#define EPWMA_PERIOD            (DEVICE_AHBCLK_FREQ / (EPWMA_FREQ * 2U))
#define EPWMA_COMPARE           ((uint16_t)(EPWMA_PERIOD * EPWMA_DUTY_A))

/* ---- EPWMB: 独立 (GPIO7=EPWM2A) ---- */
#define EPWMB_GPIO_A            7
#define EPWMB_GPIO_A_MUX        GPIO_7_EPWM2_A
#define EPWMB_BASE              EPWM2_BASE
#define EPWMB_FREQ              20000U	// 频率20Khz
#define EPWMB_DUTY              0.50f	// 占空比 50%
#define EPWMB_PERIOD            (DEVICE_AHBCLK_FREQ / (EPWMB_FREQ * 2U))
#define EPWMB_COMPARE           ((uint16_t)(EPWMB_PERIOD * EPWMB_DUTY))

/* ---- EPWMC: 独立 (GPIO3=EPWM4B) ---- */
#define EPWMC_GPIO_B            3
#define EPWMC_GPIO_B_MUX        GPIO_3_EPWM4_B
#define EPWMC_BASE              EPWM4_BASE
#define EPWMC_FREQ              20000U	// 频率20Khz
#define EPWMC_DUTY              0.50f	// 占空比 50%
#define EPWMC_PERIOD            (DEVICE_AHBCLK_FREQ / (EPWMC_FREQ * 2U))
#define EPWMC_COMPARE           ((uint16_t)(EPWMC_PERIOD * EPWMC_DUTY))

void epwm_test_init(void);
void epwm_test_run(void);

#endif /* EPWM_TEST_H_ */
