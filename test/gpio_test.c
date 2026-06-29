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

#include "gpio_test.h"

/* ---- 六路外部中断调试计数变量 ---- */
volatile uint32_t int1_cnt = 0;
volatile uint32_t int2_cnt = 0;
volatile uint32_t int3_cnt = 0;
volatile uint32_t int4_cnt = 0;
volatile uint32_t int5_cnt = 0;
volatile uint32_t int6_cnt = 0;

/* ---- XINT ISR（每个通道独立） ---- */
__INTERRUPT void XINT1_IRQHandler(void) { GPIO_clearInterrupt_NO_XInt(233); int1_cnt++; }
__INTERRUPT void XINT2_IRQHandler(void) { GPIO_clearInterrupt_NO_XInt(237); int2_cnt++; }
__INTERRUPT void XINT3_IRQHandler(void) { GPIO_clearInterrupt_NO_XInt(231); int3_cnt++; }
__INTERRUPT void XINT4_IRQHandler(void) { GPIO_clearInterrupt_NO_XInt(238); int4_cnt++; }
__INTERRUPT void XINT5_IRQHandler(void) { GPIO_clearInterrupt_NO_XInt(241); int5_cnt++; }
__INTERRUPT void XINT6_IRQHandler(void) { GPIO_clearInterrupt_NO_XInt(225); int6_cnt++; }

void gpio_test_init(void)
{
    /* ---- GPIO 六路外部数字信号检测 ---- */
	EXTI_AIO_Init(INT1_GPIO_MUX, INT1_GPIO, GPIO_INT_XINT1, INT_XINT1, &XINT1_IRQHandler);
	EXTI_AIO_Init(INT2_GPIO_MUX, INT2_GPIO, GPIO_INT_XINT2, INT_XINT2, &XINT2_IRQHandler);
	EXTI_AIO_Init(INT3_GPIO_MUX, INT3_GPIO, GPIO_INT_XINT3, INT_XINT3, &XINT3_IRQHandler);
	EXTI_AIO_Init(INT4_GPIO_MUX, INT4_GPIO, GPIO_INT_XINT4, INT_XINT4, &XINT4_IRQHandler);
	EXTI_AIO_Init(INT5_GPIO_MUX, INT5_GPIO, GPIO_INT_XINT5, INT_XINT5, &XINT5_IRQHandler);
	EXTI_AIO_Init(INT6_GPIO_MUX, INT6_GPIO, GPIO_INT_XINT6, INT_XINT6, &XINT6_IRQHandler);

	GPIO_setPinConfig(GPIO_24_GPIO24);
	GPIO_setAnalogMode(24, GPIO_ANALOG_DISABLED);
	GPIO_enableWritePin(24);
	GPIO_writePin(24, 1);
}

void gpio_test_run(void)
{
//	while(1)
//	{
//		GPIO_writePin(24, 1);
//		DELAY_US(1000 * 200);
//		GPIO_writePin(24, 0);
//		DELAY_US(1000 * 200);
//	}
    /* 下降沿触发输入中断，通过 GDB 观察 int1_cnt ~ int6_cnt  */
}



