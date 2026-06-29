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


/**
*   @file    board_cfg.h
*   @brief   board level configurations
*
*/

#ifndef __BOARD_CFG_H__
#define __BOARD_CFG_H__

#ifdef __cplusplus
extern "C"{
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/****************************************************
    GS32_PART_NUM == 0x00137; => 00137

    GS32_PIN_COUNT == 80 =>  80pin
    GS32_PIN_COUNT == 64 =>  64pin
    GS32_PIN_COUNT == 48 =>  48pin
******************************************************/

/**
  * @brief Configuration of the CRG
  */

/* ???????????? */
#define __HSE_ENABLE                    1

/* ??????????: 0 ???????, 1 ??????? */
#define __HSE_CLOCK_TYPE                0

/* ?څPLL???????: 10MHz IRC????2MHz~25MHz?????? */
#if __HSE_ENABLE == 0
    #define DEVICE_OSCSRC_FREQ          (10*1000*1000U)
#else
    #define DEVICE_OSCSRC_FREQ          (20*1000*1000U)
#endif

/* ????? <= 180MHz */
#define DEVICE_SYSCLK_FREQ              (180*1000*1000U)

/* ??????? */
#define DEVICE_SYSCLK_DIV               (2)

/* PLL??? */
#define DEVICE_PLLCLK_FREQ              (DEVICE_SYSCLK_FREQ * DEVICE_SYSCLK_DIV)

/* AHB????????? */
#define DEVICE_AHBCLK_DIV               (2)

/* AHB??????????????? */
#define DEVICE_AHBCLK_FREQ              (DEVICE_SYSCLK_FREQ/DEVICE_AHBCLK_DIV)

/* APB????????? = 1??2??4??8 */
#define DEVICE_APBCLK_DIV               (4)

/* APB?????????? MHz */
#define DEVICE_APBCLK_FREQ              (DEVICE_SYSCLK_FREQ/DEVICE_APBCLK_DIV)

/* ??????????????APB?????????? */
#define SOC_TIMER_FREQ                  (DEVICE_APBCLK_FREQ)

/**
  * @brief Configuration of the LED pins
  */
#define GPIO_PIN_LED1                   20U  // GPIO number for LED8
#define GPIO_PIN_LED2                   22U  // GPIO number for LED9
#define GPIO_CFG_LED1                   GPIO_20_GPIO20  // "pinConfig" for LED8
#define GPIO_CFG_LED2                   GPIO_22_GPIO22  // "pinConfig" for LED9

/**
  * @brief Configuration of the SCI pins
  */
#define SCIA_TX_PIN                     GPIO_29_SCIA_TX
#define SCIA_RX_PIN                     GPIO_28_SCIA_RX
#define SCIB_TX_PIN                     GPIO_14_SCIB_TX
#define SCIB_RX_PIN                     GPIO_15_SCIB_RX

#define LOG_SCI_BASE                    SCIA_BASE

#ifdef __cplusplus
}
#endif

#endif  /* __BOARD_CFG_H__ */
