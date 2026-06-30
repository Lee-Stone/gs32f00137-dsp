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

#include "hrpwm_test.h"

volatile float hrpwma_duty = HRPWMA_DUTY;
volatile float hrpwmb_duty = HRPWMB_DUTY;
volatile uint32_t hrpwm_period = HRPWMA_PERIOD;

void hrpwm_test_init(void)
{
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    /* ---- HRPWMA: ª•≤π (GPIO0/1, EPWM1) ---- */
    HRPWM_GPIO_Init(HRPWMA_GPIO_A_MUX, HRPWMA_GPIO_A, HRPWMA_GPIO_B_MUX, HRPWMA_GPIO_B);
    /* ---- HRPWMB: ∂¿¡¢ (GPIO7, EPWM2) ---- */
    HRPWM_GPIO_Init(HRPWMB_GPIO_A_MUX, HRPWMB_GPIO_A, 0, 0);

#if PLAN == 1
    HRPWM_Module_Init(HRPWMA_BASE, HRPWMA_PERIOD, true, HRPWMA_DEADTIME_US);
    HRPWM_SetDuty(HRPWMA_BASE, HRPWMA_DUTY);

    HRPWM_Module_Init(HRPWMB_BASE, HRPWMB_PERIOD, false, 0.0f);
    HRPWM_SetDuty(HRPWMB_BASE, HRPWMB_DUTY);
#else
	uint32_t cmpa, cmpb, cmpc, cmpd;
	uint32_t dt;

	/* ---- HRPWMA: ª•≤π (GPIO0/1, EPWM1) ---- */
	HRPWM_Module_Init(HRPWMA_BASE, HRPWMA_PERIOD);
	cmpa = (uint32_t)(HRPWMA_PHASE * HRPWMA_PERIOD * 256.0f);
	cmpb = (uint32_t)((HRPWMA_PHASE + HRPWMA_DUTY) * HRPWMA_PERIOD * 256.0f);
	dt   = (uint32_t)(DEVICE_AHBCLK_FREQ * HRPWMA_DEADTIME_US / 1000000U) * 256U;
	cmpc = cmpa + dt;
	cmpd = cmpb + dt;
	HRPWM_Update(HRPWMA_BASE, HRPWMA_PERIOD, cmpa, cmpb, cmpc, cmpd);

	/* ---- HRPWMB: ∂¿¡¢ (GPIO7, EPWM2) ---- */
	HRPWM_Module_Init(HRPWMB_BASE, HRPWMB_PERIOD);
	cmpa = 256;
	cmpb = (uint32_t)(HRPWMB_DUTY * HRPWMB_PERIOD * 256.0f);
	HRPWM_Update(HRPWMB_BASE, HRPWMB_PERIOD, cmpa, cmpb, 0, 0);
#endif

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);
}

void hrpwm_test_run(void)
{
    while(1)
    {
#if PLAN == 1
        HRPWM_SetDuty(HRPWMA_BASE, hrpwma_duty);
        HRPWM_SetDuty(HRPWMB_BASE, hrpwmb_duty);
#else
		uint32_t cmpa, cmpb, cmpc, cmpd;
		uint32_t dt;

		/* ---- HRPWMA: ª•≤π (GPIO0/1, EPWM1) ---- */
		cmpa = (uint32_t)(HRPWMA_PHASE * hrpwm_period * 256.0f);
		cmpb = (uint32_t)((HRPWMA_PHASE + hrpwma_duty) * hrpwm_period * 256.0f);
		dt   = (uint32_t)(DEVICE_AHBCLK_FREQ * HRPWMA_DEADTIME_US / 1000000U) * 256U;
		cmpc = cmpa + dt;
		cmpd = cmpb + dt;
		HRPWM_Update(HRPWMA_BASE, (uint16_t)hrpwm_period, cmpa, cmpb, cmpc, cmpd);

		/* ---- HRPWMB: ∂¿¡¢ (GPIO7, EPWM2) ---- */
		cmpa = 256;
		cmpb = (uint32_t)(hrpwmb_duty * hrpwm_period * 256.0f);
		HRPWM_Update(HRPWMB_BASE, (uint16_t)hrpwm_period, cmpa, cmpb, 0, 0);
#endif
    }
}
