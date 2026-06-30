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

#ifndef BSP_HRPWM_H_
#define BSP_HRPWM_H_

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"

/* ==== 方案选择 ============================================
 * PLAN 1: 硬件死区互补（UP-DOWN）
 * PLAN 2: 四比较值软件互补（UP）
 * ========================================================= */
#define PLAN 2

void HRPWM_GPIO_Init(uint32_t a_mux, uint32_t a_pin, uint32_t b_mux, uint32_t b_pin);

#if PLAN == 1
void HRPWM_Module_Init(uint32_t base, uint16_t period, bool enableDeadband, float deadtimeUs);
void HRPWM_SetDuty(uint32_t base, float f32Duty);
#elif PLAN == 2
void HRPWM_Module_Init(uint32_t base, uint16_t period);
void HRPWM_Update(uint32_t base, uint16_t period, uint32_t cmpa, uint32_t cmpb, uint32_t cmpc, uint32_t cmpd);
#endif

#endif /* BSP_HRPWM_H_ */
