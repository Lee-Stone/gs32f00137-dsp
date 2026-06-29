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

#ifndef BSP_EQEP_H_
#define BSP_EQEP_H_

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"

/* ---- EQEP1 引脚配置 ---- */
#define BSP_EQEP1_A_GPIO            6
#define BSP_EQEP1_B_GPIO            7
#define BSP_EQEP1_INDEX_GPIO        9
#define BSP_EQEP1_STROBE_GPIO       8
#define BSP_EQEP1_A_GPIO_MUX        GPIO_6_EQEP1_A
#define BSP_EQEP1_B_GPIO_MUX        GPIO_7_EQEP1_B
#define BSP_EQEP1_INDEX_GPIO_MUX    GPIO_9_EQEP1_INDEX
#define BSP_EQEP1_STROBE_GPIO_MUX   GPIO_8_EQEP1_STROBE

/* ---- EQEP2 引脚配置 ---- */
#define BSP_EQEP2_A_GPIO            18
#define BSP_EQEP2_B_GPIO            19
#define BSP_EQEP2_STROBE_GPIO       4
#define BSP_EQEP2_A_GPIO_MUX        GPIO_18_EQEP2_A
#define BSP_EQEP2_B_GPIO_MUX        GPIO_19_EQEP2_B
#define BSP_EQEP2_STROBE_GPIO_MUX   GPIO_4_EQEP2_STROBE

/* ---- eQEP 解码器配置 ---- */
#define BSP_EQEP_DECODER_CONFIG     (EQEP_CONFIG_QUADRATURE | \
                                     EQEP_CONFIG_2X_RESOLUTION | \
                                     EQEP_CONFIG_NO_SWAP | \
                                     EQEP_CONFIG_IGATE_DISABLE)

/* ---- eQEP 位置计数器配置 ---- */
#define BSP_EQEP_POS_RESET_MODE     EQEP_POSITION_RESET_MAX_POS
#define BSP_EQEP_MAX_POSITION       4294967295U

/* ---- eQEP 捕获单元配置 ---- */
#define BSP_EQEP_CAP_CLK_PRESCALE   EQEP_CAPTURE_CLK_DIV_128
#define BSP_EQEP_UNIT_POS_EVNT_DIV  EQEP_UNIT_POS_EVNT_DIV_256

/* ---- eQEP 方向定义 ---- */
#define BSP_EQEP_DIR_FORWARD        1
#define BSP_EQEP_DIR_REVERSE       -1

/* ---- eQEP 中断号 ---- */
#define BSP_EQEP1_IRQ               INT_EQEP1
#define BSP_EQEP2_IRQ               INT_EQEP2

void EQEP_Clock_Init(void);

void EQEP_GPIO_Init(uint32_t a_mux, uint32_t a_pin,
                    uint32_t b_mux, uint32_t b_pin,
                    uint32_t i_mux, uint32_t i_pin,
                    uint32_t s_mux, uint32_t s_pin);

void EQEP_Module_Init(uint32_t base, uint32_t unitTimePeriod);

void EQEP_Init(uint32_t base, uint32_t unitTimePeriod,
               uint32_t a_mux, uint32_t a_pin,
               uint32_t b_mux, uint32_t b_pin,
               uint32_t i_mux, uint32_t i_pin,
               uint32_t s_mux, uint32_t s_pin);

#endif /* BSP_EQEP_H_ */
