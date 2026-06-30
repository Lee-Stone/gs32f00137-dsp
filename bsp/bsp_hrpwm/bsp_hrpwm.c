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
#include "bsp_hrpwm.h"
#include "SFO_v2_1.h"

/* ---- SFO 校准参数 ---- */
static SFO_Params_t sfoParams;

/* ---- HRPWM GPIO 初始化 ---- */
void HRPWM_GPIO_Init(uint32_t a_mux, uint32_t a_pin, uint32_t b_mux, uint32_t b_pin)
{
    /* A 通道 */
    if (a_mux != 0) {
        GPIO_setPinConfig(a_mux);
        GPIO_setPadConfig(a_pin, GPIO_PIN_TYPE_STD);
        GPIO_setQualificationMode(a_pin, GPIO_QUAL_ASYNC);
    }
    /* B 通道 */
    if (b_mux != 0) {
        GPIO_setPinConfig(b_mux);
        GPIO_setPadConfig(b_pin, GPIO_PIN_TYPE_STD);
        GPIO_setQualificationMode(b_pin, GPIO_QUAL_ASYNC);
    }
}

/* ---- HRPWM 模块初始化 ---- */
#if PLAN == 1
void HRPWM_Module_Init(uint32_t base, uint16_t period, bool enableDeadband, float deadtimeUs)
#elif PLAN == 2
void HRPWM_Module_Init(uint32_t base, uint16_t period)
#endif
{
    /* 时钟分频：/1，不分配 */
    HRPWM_setClockPrescaler(base, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_1);
    /* 设置时基周期 */
    EPWM_setTimeBasePeriod(base, period);
    /* 计数器清零 */
    HRPWM_setTimeBaseCounter(base, 0);

#if PLAN == 1
    /* ---- PLAN 1: 增减计数 + 硬件死区互补 ---- */
    HRPWM_setTimeBaseCounterMode(base, EPWM_COUNTER_MODE_UP_DOWN);
#elif PLAN == 2
    /* ---- PLAN 2: 上升计数 + 四比较值独立控制 ---- */
    HRPWM_setTimeBaseCounterMode(base, EPWM_COUNTER_MODE_UP);
    EPWM_setEmulationMode(base, EPWM_EMULATION_FREE_RUN);
#endif

    /* 禁止相位加载 */
    EPWM_disablePhaseShiftLoad(base);
    /* 相位偏移 = 0 */
    EPWM_setPhaseShift(base, 0);

#if PLAN == 1
    /* CMPA/B 影子加载 */
    HRPWM_setCounterCompareShadowLoadMode(base, HRPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_ZERO);
    HRPWM_setCounterCompareShadowLoadMode(base, HRPWM_COUNTER_COMPARE_B, EPWM_COMP_LOAD_ON_CNTR_ZERO);

    /* A: UP_CMPA高, DOWN_CMPA低 */
    HRPWM_setActionQualifierAction(base, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    HRPWM_setActionQualifierAction(base, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    /* B: UP_CMPA低, DOWN_CMPA高（硬件互补） */
    HRPWM_setActionQualifierAction(base, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    HRPWM_setActionQualifierAction(base, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);

    /* 死区配置 */
    if (enableDeadband)
    {
        uint16_t db_cnt = (uint16_t)(DEVICE_AHBCLK_FREQ * deadtimeUs / 1000000U);
        EPWM_setRisingEdgeDeadBandDelayInput(base, EPWM_DB_INPUT_EPWMA);
        EPWM_setDeadBandDelayPolarity(base, EPWM_DB_RED, EPWM_DB_POLARITY_ACTIVE_HIGH);
        EPWM_setDeadBandDelayMode(base, EPWM_DB_RED, true);
        EPWM_setDeadBandDelayPolarity(base, EPWM_DB_FED, EPWM_DB_POLARITY_ACTIVE_LOW);
        EPWM_setDeadBandDelayMode(base, EPWM_DB_FED, true);
        EPWM_setRisingEdgeDelayCount(base, db_cnt);
        EPWM_setFallingEdgeDelayCount(base, db_cnt);
    }

#elif PLAN == 2
    /* 使能 CMPA1/B1 扩展（B 通道独立比较器）— 必须 EPWM+HRPWM 都调 */
    EPWM_enableCounterCompare_Extend(base);
    HRPWM_enableCounterCompare_Extend(base);

    /* CMPA/B 初值清零 + 影子加载（ZERO_PERIOD） */
    EPWM_setCounterCompareValue(base, EPWM_COUNTER_COMPARE_A, 0);
    HRPWM_setCounterCompareShadowLoadMode(base, HRPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_ZERO_PERIOD);
    EPWM_setCounterCompareValue(base, EPWM_COUNTER_COMPARE_B, 0);
    HRPWM_setCounterCompareShadowLoadMode(base, HRPWM_COUNTER_COMPARE_B, EPWM_COMP_LOAD_ON_CNTR_ZERO_PERIOD);

    /* CMPA1/B1 初值清零 + 影子加载 */
    EPWM_setCounterCompareValue(base, EPWM_COUNTER_COMPARE_A1_EXT, 0);
    HRPWM_setCounterCompareShadowLoadMode(base, EPWM_COUNTER_COMPARE_A1_EXT, EPWM_COMP_LOAD_ON_CNTR_ZERO_PERIOD);
    EPWM_setCounterCompareValue(base, EPWM_COUNTER_COMPARE_B1_EXT, 0);
    HRPWM_setCounterCompareShadowLoadMode(base, EPWM_COUNTER_COMPARE_B1_EXT, EPWM_COMP_LOAD_ON_CNTR_ZERO_PERIOD);

    /* A: UP_CMPA高, UP_CMPB低 */
    HRPWM_setActionQualifierAction(base, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    HRPWM_setActionQualifierAction(base, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_LOW,  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
    EPWM_disableActionQualifierShadowLoadMode(base, EPWM_ACTION_QUALIFIER_A);

    /* B: UP_CMPA1低, UP_CMPB1高 */
    HRPWM_setActionQualifierAction(base, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_LOW,  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA1);
    HRPWM_setActionQualifierAction(base, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB1);
    EPWM_disableActionQualifierShadowLoadMode(base, EPWM_ACTION_QUALIFIER_B);
#endif

    /* 使能 HRPWM 自动转换 */
    HRPWM_enableAutoConversion(base);
    /* 使能 HRPWM 校准 */
    HRPWM_enableCalibration(base);
    /* MEP 边沿：双边沿 */
    HRPWM_setMEPEdgeSelect(base, HRPWM_CHANNEL_A, HRPWM_MEP_CTRL_RISING_AND_FALLING_EDGE);
    HRPWM_setMEPEdgeSelect(base, HRPWM_CHANNEL_B, HRPWM_MEP_CTRL_RISING_AND_FALLING_EDGE);
    /* 影子加载事件 */
    HRPWM_setCounterCompareShadowLoadEvent(base, HRPWM_CHANNEL_A, HRPWM_LOAD_ON_CNTR_ZERO_PERIOD);
    HRPWM_setCounterCompareShadowLoadEvent(base, HRPWM_CHANNEL_B, HRPWM_LOAD_ON_CNTR_ZERO_PERIOD);

#if PLAN == 1
    /* 使能增减计数兼容模式 */
    if (base == EPWM1_BASE) {
        HRPWM_enableUpDownInit_Compatible(HRPWM1_Compatible);
    } else if (base == EPWM2_BASE) {
        HRPWM_enableUpDownInit_Compatible(HRPWM2_Compatible);
    } else if (base == EPWM3_BASE) {
        HRPWM_enableUpDownInit_Compatible(HRPWM3_Compatible);
    } else if (base == EPWM4_BASE) {
        HRPWM_enableUpDownInit_Compatible(HRPWM4_Compatible);
    }
#endif

    /* SFO 校准 */
    SFO_Struct_deinit(&sfoParams);
    SFO_Struct_init(&sfoParams, 5);
    SFO(&sfoParams);
}

#if PLAN == 1
/* ---- PLAN 1: 浮点占空比 ---- */
void HRPWM_SetDuty(uint32_t base, float f32Duty)
{
    uint16_t period;

    if      (base == EPWM1_BASE) period = EPWM_getTimeBasePeriod(base);
    else if (base == EPWM2_BASE) period = EPWM_getTimeBasePeriod(base);
    else return;

    uint32_t cmp = (uint32_t)((1.0f - f32Duty) * period * 256.0f);

    HRPWM_setCounterCompareValue_Compatible_Auto(base, HRPWM_COUNTER_COMPARE_A, cmp);
    HRPWM_setCounterCompareValue_Compatible_Auto(base, HRPWM_COUNTER_COMPARE_B, cmp);
}

#elif PLAN == 2
/* ---- 四比较值独立写入 + 动态调频 ---- */
void HRPWM_Update(uint32_t base, uint16_t period, uint32_t cmpa, uint32_t cmpb, uint32_t cmpc, uint32_t cmpd)
{
    EPWM_setTimeBasePeriod(base, period);
    HRPWM_setCounterCompareValue(base, HRPWM_COUNTER_COMPARE_A, cmpa);
    HRPWM_setCounterCompareValue(base, HRPWM_COUNTER_COMPARE_B, cmpb);
    HRPWM_setCounterCompareValue_Extend(base, EPWM_COUNTER_COMPARE_A1_EXT, cmpc);
    HRPWM_setCounterCompareValue_Extend(base, EPWM_COUNTER_COMPARE_B1_EXT, cmpd);
}
#endif
