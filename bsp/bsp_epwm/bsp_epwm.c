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
#include "bsp_epwm.h"

/* ---- EPWM init ---- */
void EPWM_Init(uint32_t base, uint16_t period, uint16_t compare, bool enableDeadband, float deadtimeUs)
{
    /* Step 1: clock enable */
    EPWM_Clock_Init();
    /* Step 2: GPIO mux */
    EPWM_GPIO_Init(BSP_EPWM1_A_GPIO_MUX, BSP_EPWM1_A_GPIO, BSP_EPWM1_B_GPIO_MUX, BSP_EPWM1_B_GPIO);
    /* Step 3: module registers */
    EPWM_Module_Init(base, period, compare, enableDeadband, deadtimeUs);
}

/* ---- EPWM clock init ---- */
void EPWM_Clock_Init(void)
{
    /*
     * Actual clock initialization is performed in Device_init()
     */
}

/* ---- EPWM GPIO init: a_mux=0/a_pin=0 means skip A channel ---- */
void EPWM_GPIO_Init(uint32_t a_mux, uint32_t a_pin, uint32_t b_mux, uint32_t b_pin)
{
    /* A channel */
    if (a_mux != 0) {
        GPIO_setPinConfig(a_mux);
        GPIO_setPadConfig(a_pin, GPIO_PIN_TYPE_STD);
        GPIO_setQualificationMode(a_pin, GPIO_QUAL_ASYNC);
    }
    /* B channel */
    if (b_mux != 0) {
        GPIO_setPinConfig(b_mux);
        GPIO_setPadConfig(b_pin, GPIO_PIN_TYPE_STD);
        GPIO_setQualificationMode(b_pin, GPIO_QUAL_ASYNC);
    }
}

/* ---- EPWM module init: complementary output + optional dead-band + ADC trigger ---- */
void EPWM_Module_Init(uint32_t base, uint16_t period, uint16_t compare, bool enableDeadband, float deadtimeUs)
{
    /* ---- TB: up-down count, prescaler /1 ---- */
    EPWM_setClockPrescaler(base, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_1);
    EPWM_setTimeBasePeriod(base, period);
    EPWM_setTimeBaseCounter(base, 0);
    EPWM_setTimeBaseCounterMode(base, EPWM_COUNTER_MODE_UP_DOWN);
    EPWM_disablePhaseShiftLoad(base);
    EPWM_setPhaseShift(base, 0);
    EPWM_enableSyncOutPulseSource(base, EPWM_SYNC_OUT_PULSE_ON_CNTR_ZERO);

    /* ---- CC: CMPA with shadow load on zero ---- */
    EPWM_setCounterCompareValue(base, EPWM_COUNTER_COMPARE_A, compare);
    EPWM_setCounterCompareShadowLoadMode(base, EPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_ZERO);

    /* ---- AQ A: high on UP_CMPA, low on DOWN_CMPA ---- */
    EPWM_setActionQualifierAction(base, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(base, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_LOW,  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);

    /* ---- AQ B: complementary (low on UP_CMPA, high on DOWN_CMPA) ---- */
    EPWM_setActionQualifierAction(base, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_LOW,  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(base, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);

    /* ---- DB: dead-band to prevent shoot-through ---- */
    if (enableDeadband)
    {
        uint16_t db_cnt = (uint16_t)(DEVICE_AHBCLK_FREQ * deadtimeUs / 1000000U);
        /* RED: EPWMA input, active-high */
        EPWM_setRisingEdgeDeadBandDelayInput(base, EPWM_DB_INPUT_EPWMA);
        EPWM_setDeadBandDelayPolarity(base, EPWM_DB_RED, EPWM_DB_POLARITY_ACTIVE_HIGH);
        EPWM_setDeadBandDelayMode(base, EPWM_DB_RED, true);
        /* FED: active-low (inverted for B-channel) */
        EPWM_setDeadBandDelayPolarity(base, EPWM_DB_FED, EPWM_DB_POLARITY_ACTIVE_LOW);
        EPWM_setDeadBandDelayMode(base, EPWM_DB_FED, true);
        EPWM_setRisingEdgeDelayCount(base, db_cnt);
        EPWM_setFallingEdgeDelayCount(base, db_cnt);
    }
}
