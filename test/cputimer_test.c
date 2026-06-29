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

#include "cputimer_test.h"
#include "adc_test.h"
#include "cmpss_test.h"

/* ---- CPUTimer1 ISR: 50us ---- */
__INTERRUPT void TIMER1_IRQHandler(void)
{
    /* Read latest ADC results */
    ADC_ReadResults();

    /* Read CMPSS results */
    CMPSS_ReadResults();

    CPUTimer_ClearInterruptStatus(CPUTIMER1_BASE);
    CPUTimer_clearOverflowFlag(CPUTIMER1_BASE);
}

/* ---- CPUTimer test init ---- */
void cputimer_test_init(void)
{
    /* ---- CPUTimer1: 20KHz (50us period) ---- */
	CPUTimer_Clock_Init();
    CPUTimer_Module_Init(CPUTIMER1_BASE, CPUTIMER_PERIOD, CPUTIMER_IRQN, &TIMER1_IRQHandler);
}

/* ---- CPUTimer test run ---- */
void cputimer_test_run(void)
{
	/* 定时器中断触发 */
}
