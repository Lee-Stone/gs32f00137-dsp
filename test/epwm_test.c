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

#include "epwm_test.h"

/* ---- EPWM test init: 1 complementary pair + 2 independent ---- */
void epwm_test_init(void)
{
    /* ---- Disable sync for multi-instance config ---- */
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    /* ---- EPWMA: complementary output + dead-band (GPIO0/1) ---- */
    // EPWM_GPIO_Init(EPWMA_GPIO_A_MUX, EPWMA_GPIO_A, EPWMA_GPIO_B_MUX, EPWMA_GPIO_B);
    // EPWM_Module_Init(EPWMA_BASE, EPWMA_PERIOD, EPWMA_COMPARE, true, EPWMA_DEADTIME_US);

    /* ---- EPWMB: independent output (GPIO7) ---- */
    // EPWM_GPIO_Init(EPWMB_GPIO_A_MUX, EPWMB_GPIO_A, 0, 0);
    // EPWM_Module_Init(EPWMB_BASE, EPWMB_PERIOD, EPWMB_COMPARE, false, 0.0f);

    /* ---- EPWMC: independent output (GPIO3 via EPWM4B) ---- */
    EPWM_GPIO_Init(0, 0, EPWMC_GPIO_B_MUX, EPWMC_GPIO_B);
    EPWM_Module_Init(EPWMC_BASE, EPWMC_PERIOD, EPWMC_COMPARE, false, 0.0f);

    /* ---- Enable sync to align all PWM time bases ---- */
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);
}

/* ---- EPWM test run ---- */
void epwm_test_run(void)
{
	/* Ê¾²¨Æ÷²é¿´ GPIO0 GPIO1 GPIO3 GPIO7 */
}
