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
#include "test_includes.h"

int main(void)
{
    /* Device initialization */
    Device_init();
    /* Disable all interrupts */
    __disable_irq();

    /* EPWM Init */
    epwm_test_init();

    /* HRPWM Init */
    hrpwm_test_init();

    /* ADC Init */
    adc_test_init();

    /* CMPSS Init */
    cmpss_test_init();

    /* CPUTimer Init */
    cputimer_test_init();

    /* Enable all interrupts */
    __enable_irq();

    /* EPWM Run */
    epwm_test_run();

    /* HRPWM Run */
    hrpwm_test_run();

    /* ADC Run */
    adc_test_run();

    /* CMPSS Run */
    cmpss_test_run();

    /* CPUTimer Run */
    cputimer_test_run();

    while (1)
    {

    }

    return 0;
}
