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
#include "bsp_gpio.h"

void EXTI_AIO_Init(uint32_t pinCfg, uint32_t pin, GPIO_ExternalIntNum extIntNum, IRQn_Type irq, IrqHandlerType handler)
{
    /* 将引脚复用为 GPIO 功能 */
    GPIO_setPinConfig(pinCfg);
    /* 关闭模拟模式（AIO 默认开启） */
    GPIO_setAnalogMode(pin, GPIO_ANALOG_DISABLED);
    /* 设置输入滤波：同步模式 + 510 周期去抖 */
    GPIO_setQualificationPeriod(pin, 510);
    /* 设置引脚类型：标准 + 下拉 */
    GPIO_setPadConfig(pin, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLDOWN);
    GPIO_setQualificationMode(pin, GPIO_QUAL_SYNC);
    /* 设置为 AIO 数字输入模式 */
    GPIO_setDirectionMode(pin, AIO_IE_MODE_IN);
    /* 将引脚映射到指定 XINT 通道 */
    GPIO_setInterruptPin(pin, extIntNum);
    /* 设置中断触发类型：上升沿 */
    GPIO_setInterruptType(extIntNum, GPIO_INT_TYPE_RISING_EDGE);
    /* 使能 XINT 通道中断 */
    GPIO_enableInterrupt(extIntNum);
    /* 注册 NVIC 中断处理函数 */
    Interrupt_register(irq, handler);
    /* 使能 NVIC 中断 */
    Interrupt_enable(irq);
}
