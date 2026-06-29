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

#ifndef EQEP_TEST_H_
#define EQEP_TEST_H_

#include "device.h"
#include "bsp_eqep.h"

/* ---- EQEP1 测试引脚 ---- */
#define EQEP1_A_GPIO            6
#define EQEP1_B_GPIO            7
#define EQEP1_INDEX_GPIO        9
#define EQEP1_STROBE_GPIO       8
#define EQEP1_A_GPIO_MUX        GPIO_6_EQEP1_A
#define EQEP1_B_GPIO_MUX        GPIO_7_EQEP1_B
#define EQEP1_INDEX_GPIO_MUX    GPIO_9_EQEP1_INDEX
#define EQEP1_STROBE_GPIO_MUX   GPIO_8_EQEP1_STROBE

/* ---- EQEP2 测试引脚 ---- */
#define EQEP2_A_GPIO            18
#define EQEP2_B_GPIO            19
#define EQEP2_STROBE_GPIO       4
#define EQEP2_A_GPIO_MUX        GPIO_18_EQEP2_A
#define EQEP2_B_GPIO_MUX        GPIO_19_EQEP2_B
#define EQEP2_STROBE_GPIO_MUX   GPIO_4_EQEP2_STROBE

/* ---- 单元定时器周期（SYSCLK 周期数） ---- */
#define EQEP_UNIT_TIME_PERIOD   (DEVICE_SYSCLK_FREQ / 100)

/* ---- 测试变量 ---- */
extern volatile uint32_t eqep1_position;
extern volatile int16_t  eqep1_direction;
extern volatile uint32_t eqep2_position;
extern volatile int16_t  eqep2_direction;

void eqep_test_init(void);
void eqep_test_run(void);

#endif /* EQEP_TEST_H_ */
