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
#include "bsp_cmpss.h"

/* ---- CMPSS GPIO 初始化 ---- */
void CMPSS_GPIO_Init(uint32_t pin, uint32_t hpMuxSelect, uint32_t hpChannel, uint32_t lpMuxSelect, uint32_t lpChannel)
{
    /* 设置 GPIO 模拟模式 */
    GPIO_setAnalogMode(pin, GPIO_ANALOG_ENABLED);

    /* 路由 High/Low 比较器正输入端到指定引脚 */
    ASysCtl_selectCMPHPMux(hpMuxSelect, hpChannel);
    ASysCtl_selectCMPLPMux(lpMuxSelect, lpChannel);
}

/* ---- CMPSS 模块初始化 ---- */
void CMPSS_Module_Init(uint32_t base, uint16_t dacHigh, uint16_t dacLow)
{
    /* Step 1: 配置比较器 — 负端=内部DAC */
    CMPSS_configHighComparator(base, CMPSS_INSRC_DAC);
    CMPSS_configLowComparator(base, CMPSS_INSRC_DAC);

    /* Step 2: 配置内部 DAC */
    CMPSS_configDACHigh(base, (CMPSS_DACVAL_SYSCLK | CMPSS_DACREF_VDDA | CMPSS_DACSRC_SHDW));
    CMPSS_configDACLow(base, CMPSS_DACSRC_SHDW);

    /* Step 3: 设置 DAC 阈值 */
    CMPSS_setDACValueHigh(base, dacHigh);
    CMPSS_setDACValueLow(base, dacLow);

    /* Step 4: 配置数字滤波器 */
    CMPSS_configFilterHigh(base, 0U, 1U, 1U);
    CMPSS_configFilterLow(base, 0U, 1U, 1U);

    /* Step 5: 配置输出 */
    CMPSS_configOutputsHigh(base, (CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP));
    CMPSS_configOutputsLow(base, (CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP));

    /* Step 6: 配置迟滞 */
    CMPSS_setHysteresis(base, 0U);

    /* Step 7: 配置斜坡发生器 */
    CMPSS_configRamp(base, 0U, 0U, 0U, 1U, true);

    /* Step 8: 禁用 PWMSYNC 复位锁存 */
    CMPSS_disableLatchResetOnPWMSYNCHigh(base);
    CMPSS_disableLatchResetOnPWMSYNCLow(base);

    /* Step 9: 配置消隐 */
    CMPSS_configBlanking(base, 1U);
    CMPSS_disableBlanking(base);

    /* Step 10: PWMSYNC 不影响锁存 */
    CMPSS_configLatchOnPWMSYNC(base, false, false);

    /* Step 11: 使能 CMPSS 模块 */
    CMPSS_enableModule(base);

    /* Step 12: 等待 DAC 上电稳定 */
    DEVICE_DELAY_US(500);

    /* Step 13: 清除数字滤波器锁存 */
    CMPSS_clearFilterLatchHigh(base);
    CMPSS_clearFilterLatchLow(base);
}
