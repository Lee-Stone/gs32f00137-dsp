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

#ifndef BSP_EPWM_H_
#define BSP_EPWM_H_

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"

/* ---- EPWM1 pins: GPIO0=A, GPIO1=B, complementary ---- */
#define BSP_EPWM1_A_GPIO            0
#define BSP_EPWM1_B_GPIO            1
#define BSP_EPWM1_A_GPIO_MUX        GPIO_0_EPWM1_A
#define BSP_EPWM1_B_GPIO_MUX        GPIO_1_EPWM1_B

/* ---- EPWM2 pins: GPIO7=A, GPIO3=B, complementary ---- */
#define BSP_EPWM2_A_GPIO            7
#define BSP_EPWM2_B_GPIO            3
#define BSP_EPWM2_A_GPIO_MUX        GPIO_7_EPWM2_A
#define BSP_EPWM2_B_GPIO_MUX        GPIO_3_EPWM2_B

/* ---- Dead-band default: 1us @ AHBCLK=90MHz -> 90 counts ---- */
#define BSP_EPWM_DB_DEFAULT_US  1.0f

void EPWM_Clock_Init(void);
void EPWM_GPIO_Init(uint32_t a_mux, uint32_t a_pin, uint32_t b_mux, uint32_t b_pin);
void EPWM_Module_Init(uint32_t base, uint16_t period, uint16_t compare, bool enableDeadband, float deadtimeUs);
void EPWM_Init(uint32_t base, uint16_t period, uint16_t compare, bool enableDeadband, float deadtimeUs);

#endif /* BSP_EPWM_H_ */
