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
#include "bsp_cputimer.h"

/* ---- CPUTimer 初始化 ---- */
void CPUTimer_Init(uint32_t base, uint32_t period, IRQn_Type IRQn, IrqHandlerType handler)
{
    /* 第1步：时钟使能 */
    CPUTimer_Clock_Init();
    /* 第2步：CPUTimer 模块初始化 */
    CPUTimer_Module_Init(base, period, IRQn, handler);
}

/* ---- CPUTimer 时钟初始化 ---- */
void CPUTimer_Clock_Init(void)
{
    /*
     * Actual clock initialization is performed in Device_init()
     */
}

/* ---- CPUTimer 模块初始化 ---- */
void CPUTimer_Module_Init(uint32_t base, uint32_t period, IRQn_Type IRQn, IrqHandlerType handler)
{
    /* 初始化 CPUTimer 并使能中断 */
    CPUTimer_init(base, period);
    Interrupt_register(IRQn, handler);
    Interrupt_enable(IRQn);
}
