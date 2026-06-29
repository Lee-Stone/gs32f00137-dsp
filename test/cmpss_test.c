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

#include "cmpss_test.h"

/* ---- CMPSS 状态变量 ---- */
volatile uint16_t cmpss_status;
volatile uint16_t cmpss_high_out;
volatile uint16_t cmpss_low_out;

/* ---- CMPSS test init ---- */
void cmpss_test_init(void)
{
    /* ---- GPIO 初始化 ---- */
    CMPSS_GPIO_Init(CMPSS_GPIO_PIN, CMPSS_HP_MUX_SELECT, CMPSS_HP_CHANNEL, CMPSS_LP_MUX_SELECT, CMPSS_LP_CHANNEL);

    /* ---- 模块初始化 ---- */
    CMPSS_Module_Init(CMPSS_BASE, CMPSS_DAC_HIGH, CMPSS_DAC_LOW);
}

/* ---- CMPSS test run ---- */
void cmpss_test_run(void)
{
    /* GDB 查看 CMPSS 状态变量 */
}

/* ---- CMPSS Read Results ---- */
void CMPSS_ReadResults(void)
{
    cmpss_status   = CMPSS_getStatus(CMPSS_BASE);
    cmpss_high_out = CMPSS_GET_HIGH_OUT();
    cmpss_low_out  = CMPSS_GET_LOW_OUT();
}

/* ---- 动态设置 DAC 阈值 ---- */
void CMPSS_SetThreshold(uint16_t dacVal)
{
    CMPSS_setDACValueHigh(CMPSS_BASE, dacVal);
    CMPSS_setDACValueLow(CMPSS_BASE, dacVal);
}
