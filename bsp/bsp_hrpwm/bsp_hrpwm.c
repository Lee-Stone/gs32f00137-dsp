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
void HRPWM_Module_Init(uint32_t base, uint16_t period, bool enableDeadband, float deadtimeUs)
{
    /* 时钟分频：/1，不分配 */
    HRPWM_setClockPrescaler(base, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_1);
    /* 设置时基周期 */
    EPWM_setTimeBasePeriod(base, period);
    /* 计数器清零 */
    HRPWM_setTimeBaseCounter(base, 0);
    /* 增减计数模式 */
    HRPWM_setTimeBaseCounterMode(base, EPWM_COUNTER_MODE_UP_DOWN);
    /* 禁止相位加载 */
    EPWM_disablePhaseShiftLoad(base);
    /* 相位偏移 = 0 */
    EPWM_setPhaseShift(base, 0);

    /* CMPA：计数器归零时加载 */
    HRPWM_setCounterCompareShadowLoadMode(base, HRPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_ZERO);
    /* CMPB：计数器归零时加载 */
    HRPWM_setCounterCompareShadowLoadMode(base, HRPWM_COUNTER_COMPARE_B, EPWM_COMP_LOAD_ON_CNTR_ZERO);

    /* A 输出：UP_CMPA 变高，DOWN_CMPA 变低 */
    HRPWM_setActionQualifierAction(base, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    HRPWM_setActionQualifierAction(base, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    /* B 输出：UP_CMPA 变低，DOWN_CMPA 变高（互补） */
    HRPWM_setActionQualifierAction(base, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    HRPWM_setActionQualifierAction(base, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);

    /* 死区配置 */
    if (enableDeadband)
    {
        uint16_t db_cnt = (uint16_t)(DEVICE_AHBCLK_FREQ * deadtimeUs / 1000000U);
        /* 死区输入源：EPWMA */
        EPWM_setRisingEdgeDeadBandDelayInput(base, EPWM_DB_INPUT_EPWMA);
        /* 上升沿死区极性：高有效 */
        EPWM_setDeadBandDelayPolarity(base, EPWM_DB_RED, EPWM_DB_POLARITY_ACTIVE_HIGH);
        /* 使能上升沿死区 */
        EPWM_setDeadBandDelayMode(base, EPWM_DB_RED, true);
        /* 下降沿死区极性：低有效 */
        EPWM_setDeadBandDelayPolarity(base, EPWM_DB_FED, EPWM_DB_POLARITY_ACTIVE_LOW);
        /* 使能下降沿死区 */
        EPWM_setDeadBandDelayMode(base, EPWM_DB_FED, true);
        /* 设置上升沿死区延时 */
        EPWM_setRisingEdgeDelayCount(base, db_cnt);
        /* 设置下降沿死区延时 */
        EPWM_setFallingEdgeDelayCount(base, db_cnt);
    }

    /* 使能 HRPWM 自动转换 */
    HRPWM_enableAutoConversion(base);
    /* 使能 HRPWM 校准 */
    HRPWM_enableCalibration(base);
    /* A 通道 MEP 控制边沿：双边沿 */
    HRPWM_setMEPEdgeSelect(base, HRPWM_CHANNEL_A, HRPWM_MEP_CTRL_RISING_AND_FALLING_EDGE);
    /* A 通道事件：计数器归零或周期 */
    HRPWM_setCounterCompareShadowLoadEvent(base, HRPWM_CHANNEL_A, HRPWM_LOAD_ON_CNTR_ZERO_PERIOD);
    /* B 通道 MEP 控制边沿：双边沿 */
    HRPWM_setMEPEdgeSelect(base, HRPWM_CHANNEL_B, HRPWM_MEP_CTRL_RISING_AND_FALLING_EDGE);
    /* B 通道事件：计数器归零或周期 */
    HRPWM_setCounterCompareShadowLoadEvent(base, HRPWM_CHANNEL_B, HRPWM_LOAD_ON_CNTR_ZERO_PERIOD);

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

    /* SFO 校准：测量实际 MEP 步长 */
    SFO_Struct_deinit(&sfoParams);
    SFO_Struct_init(&sfoParams, 5);
    SFO(&sfoParams);
}

/* ---- HRPWM 高精度占空比设置 ---- */
void HRPWM_SetDuty(uint32_t base, uint16_t period, float duty, uint32_t compModuleA, uint32_t compModuleB)
{
    uint32_t cmp_set;

    /* UP-DOWN 模式：CMP = period * (1 - duty)，低 8 位 = MEP 小数 */
    cmp_set = (uint32_t)((1.0f - duty) * period * 256.0f);

    /* 设置 CMPA */
    if (compModuleA == HRPWM_COUNTER_COMPARE_A) {
        HRPWM_setCounterCompareValue_Compatible_Auto(base, HRPWM_COUNTER_COMPARE_A, cmp_set);
    }

    /* 设置 CMPB */
    if (compModuleB == HRPWM_COUNTER_COMPARE_B) {
        HRPWM_setCounterCompareValue_Compatible_Auto(base, HRPWM_COUNTER_COMPARE_B, cmp_set);
    }
}
