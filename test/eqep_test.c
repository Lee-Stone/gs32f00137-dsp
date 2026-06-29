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

#include "eqep_test.h"

/* ---- ??????????? ---- */
volatile uint32_t eqep1_position = 0;
volatile int16_t  eqep1_direction = 0;
volatile uint32_t eqep2_position = 0;
volatile int16_t  eqep2_direction = 0;

/* ---- eQEP1 ???????ßÿ? ---- */
__INTERRUPT void INT_EQEP1_IRQHandler(void)
{
    /* ??˘b?????ßÿ??? */
    if ((EQEP_getInterruptStatus(EQEP1_BASE) & EQEP_INT_UNIT_TIME_OUT) != 0U)
    {
        /* ??????¶À?? */
        eqep1_position = EQEP_getPosition(EQEP1_BASE);
        /* ?????????? */
        eqep1_direction = EQEP_getDirection(EQEP1_BASE);
    }
    /* ????ßÿ??? */
    EQEP_clearInterruptStatus(EQEP1_BASE, EQEP_INT_UNIT_TIME_OUT | EQEP_INT_GLOBAL);
}

/* ---- eQEP2 ???????ßÿ? ---- */
__INTERRUPT void INT_EQEP2_IRQHandler(void)
{
    /* ??˘b?????ßÿ??? */
    if ((EQEP_getInterruptStatus(EQEP2_BASE) & EQEP_INT_UNIT_TIME_OUT) != 0U)
    {
        /* ??????¶À?? */
        eqep2_position = EQEP_getPosition(EQEP2_BASE);
        /* ?????????? */
        eqep2_direction = EQEP_getDirection(EQEP2_BASE);
    }
    /* ????ßÿ??? */
    EQEP_clearInterruptStatus(EQEP2_BASE, EQEP_INT_UNIT_TIME_OUT | EQEP_INT_GLOBAL);
}

/* ---- eQEP ???????? ---- */
void eqep_test_init(void)
{
    /* ---- EQEP1 ????? ---- */
    EQEP_GPIO_Init(EQEP1_A_GPIO_MUX, EQEP1_A_GPIO, EQEP1_B_GPIO_MUX, EQEP1_B_GPIO, EQEP1_INDEX_GPIO_MUX, EQEP1_INDEX_GPIO, EQEP1_STROBE_GPIO_MUX, EQEP1_STROBE_GPIO);
    EQEP_Module_Init(EQEP1_BASE, EQEP_UNIT_TIME_PERIOD);

    /* ??? EQEP1 ???????ßÿ? */
    EQEP_enableInterrupt(EQEP1_BASE, EQEP_INT_UNIT_TIME_OUT);
    Interrupt_SetPriority(INT_EQEP1, 2, 2);
    Interrupt_register(INT_EQEP1, INT_EQEP1_IRQHandler);
    Interrupt_enable(INT_EQEP1);

    /* ---- EQEP2 ????? ---- */
    EQEP_GPIO_Init(EQEP2_A_GPIO_MUX, EQEP2_A_GPIO, EQEP2_B_GPIO_MUX, EQEP2_B_GPIO, 0U, 0U, EQEP2_STROBE_GPIO_MUX, EQEP2_STROBE_GPIO);
    EQEP_Module_Init(EQEP2_BASE, EQEP_UNIT_TIME_PERIOD);

    /* ??? EQEP2 ???????ßÿ? */
    EQEP_enableInterrupt(EQEP2_BASE, EQEP_INT_UNIT_TIME_OUT);
    Interrupt_SetPriority(INT_EQEP2, 2, 2);
    Interrupt_register(INT_EQEP2, INT_EQEP2_IRQHandler);
    Interrupt_enable(INT_EQEP2);
}

/* ---- eQEP ???????? ---- */
void eqep_test_run(void)
{
    /*
     * ???????????? g_eqep1_position / g_eqep1_direction
     * ?? g_eqep2_position / g_eqep2_direction ?????
     *   - position ??????????????Å£
     *   - direction = 1??????? -1??????
     */
}
