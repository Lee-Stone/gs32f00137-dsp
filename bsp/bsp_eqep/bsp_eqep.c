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
#include "bsp_eqep.h"

/* ---- eQEP 一站式初始化 ---- */
void EQEP_Init(uint32_t base, uint32_t unitTimePeriod,
               uint32_t a_mux, uint32_t a_pin,
               uint32_t b_mux, uint32_t b_pin,
               uint32_t i_mux, uint32_t i_pin,
               uint32_t s_mux, uint32_t s_pin)
{
    /* 第1步：时钟使能（由 Device_init 完成） */
    EQEP_Clock_Init();
    /* 第2步：GPIO 引脚复用配置 */
    EQEP_GPIO_Init(a_mux, a_pin, b_mux, b_pin, i_mux, i_pin, s_mux, s_pin);
    /* 第3步：eQEP 模块寄存器配置 */
    EQEP_Module_Init(base, unitTimePeriod);
}

/* ---- eQEP 时钟初始化（由 Device_init 完成） ---- */
void EQEP_Clock_Init(void)
{
    /*
     * Actual clock initialization is performed in Device_init()
     */
}

/* ---- eQEP GPIO 引脚初始化 ---- */
void EQEP_GPIO_Init(uint32_t a_mux, uint32_t a_pin,
                    uint32_t b_mux, uint32_t b_pin,
                    uint32_t i_mux, uint32_t i_pin,
                    uint32_t s_mux, uint32_t s_pin)
{
    /* eQEPA 输入引脚 */
    GPIO_setPinConfig(a_mux);
    GPIO_setPadConfig(a_pin, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(a_pin, GPIO_QUAL_SYNC);
    /* eQEPB 输入引脚 */
    GPIO_setPinConfig(b_mux);
    GPIO_setPadConfig(b_pin, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(b_pin, GPIO_QUAL_SYNC);
    /* eQEP Index 输入引脚 */
    if (i_mux != 0U && i_pin != 0U)
    {
        GPIO_setPinConfig(i_mux);
        GPIO_setPadConfig(i_pin, GPIO_PIN_TYPE_STD);
        GPIO_setQualificationMode(i_pin, GPIO_QUAL_SYNC);
    }
    /* eQEP Strobe 输入引脚 */
    if (s_mux != 0U && s_pin != 0U)
    {
        GPIO_setPinConfig(s_mux);
        GPIO_setPadConfig(s_pin, GPIO_PIN_TYPE_STD);
        GPIO_setQualificationMode(s_pin, GPIO_QUAL_SYNC);
    }
}

/* ---- eQEP 模块寄存器配置 ---- */
void EQEP_Module_Init(uint32_t base, uint32_t unitTimePeriod)
{
    /* 选择 eQEPA/B/Index 信号源为器件引脚 */
    EQEP_SourceSelect source =
    {
        EQEP_SOURCE_DEVICE_PIN,     /* eQEPA 来源 */
        EQEP_SOURCE_DEVICE_PIN,     /* eQEPB 来源 */
        EQEP_SOURCE_DEVICE_PIN      /* eQEP Index 来源 */
    };
    EQEP_selectSource(base, source);

    /* 设置 Strobe 信号源为 GPIO */
    EQEP_setStrobeSource(base, EQEP_STROBE_FROM_GPIO);

    /* 设置输入极性（全部不取反） */
    EQEP_setInputPolarity(base, false, false, false, false);

    /* 配置正交解码单元 */
    EQEP_setDecoderConfig(base, BSP_EQEP_DECODER_CONFIG);

    /* 设置仿真模式：立即停止 */
    EQEP_setEmulationMode(base, EQEP_EMULATIONMODE_STOPIMMEDIATELY);

    /* 配置位置计数器 */
    EQEP_setPositionCounterConfig(base, BSP_EQEP_POS_RESET_MODE, BSP_EQEP_MAX_POSITION);

    /* 设置初始位置为 0 */
    EQEP_setPosition(base, 0U);

    /* 使能单元定时器 */
    EQEP_enableUnitTimer(base, unitTimePeriod);

    /* 禁止看门狗定时器 */
    EQEP_disableWatchdog(base);

    /* 配置位置锁存模式：单元超时锁存 */
    EQEP_setLatchMode(base, EQEP_LATCH_UNIT_TIME_OUT);

    /* QMA 模块旁路 */
    EQEP_setQMAModuleMode(base, EQEP_QMA_MODE_BYPASS);

    /* 禁止 Index 期间方向变化检测 */
    EQEP_disableDirectionChangeDuringIndex(base);

    /* 位置计数器初始化模式：不动作 */
    EQEP_setPositionInitMode(base, EQEP_INIT_DO_NOTHING);

    /* 设置软件初始化位置计数器 */
    EQEP_setSWPositionInit(base, true);

    /* 初始化位置值设为 0 */
    EQEP_setInitialPosition(base, 0U);

    /* 禁止所有中断 */
    EQEP_disableInterrupt(base,
        (EQEP_INT_GLOBAL           |
         EQEP_INT_POS_CNT_ERROR    |
         EQEP_INT_PHASE_ERROR      |
         EQEP_INT_DIR_CHANGE       |
         EQEP_INT_WATCHDOG         |
         EQEP_INT_UNDERFLOW        |
         EQEP_INT_OVERFLOW         |
         EQEP_INT_POS_COMP_READY   |
         EQEP_INT_POS_COMP_MATCH   |
         EQEP_INT_STROBE_EVNT_LATCH|
         EQEP_INT_INDEX_EVNT_LATCH |
         EQEP_INT_UNIT_TIME_OUT    |
         EQEP_INT_QMA_ERROR));

    /* 清除所有中断标志 */
    EQEP_clearInterruptStatus(base,
        (EQEP_INT_GLOBAL           |
         EQEP_INT_POS_CNT_ERROR    |
         EQEP_INT_PHASE_ERROR      |
         EQEP_INT_DIR_CHANGE       |
         EQEP_INT_WATCHDOG         |
         EQEP_INT_UNDERFLOW        |
         EQEP_INT_OVERFLOW         |
         EQEP_INT_POS_COMP_READY   |
         EQEP_INT_POS_COMP_MATCH   |
         EQEP_INT_STROBE_EVNT_LATCH|
         EQEP_INT_INDEX_EVNT_LATCH |
         EQEP_INT_UNIT_TIME_OUT    |
         EQEP_INT_QMA_ERROR));

    /* 使能 eQEP 模块 */
    EQEP_enableModule(base);

    /* 配置边沿捕获单元 */
    EQEP_setCaptureConfig(base, BSP_EQEP_CAP_CLK_PRESCALE, BSP_EQEP_UNIT_POS_EVNT_DIV);

    /* 使能边沿捕获 */
    EQEP_enableCapture(base);
}
