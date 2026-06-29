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

#include <string.h>
#include "device.h"
#include "driverlib.h"
#include "mcan.h"
#include "board_cfg.h"
#include "bsp_can.h"

/* ---- 消息 RAM 布局 ---- */
#define MCAN_STD_FILTER_NUM         4U
#define MCAN_STD_FILTER_ADDR        0U
#define MCAN_RXFIFO0_ADDR           (MCAN_STD_FILTER_ADDR + MCAN_STD_FILTER_NUM)
#define MCAN_RXFIFO0_SIZE           8U
#define MCAN_TXFIFO_ADDR            (MCAN_RXFIFO0_ADDR + MCAN_RXFIFO0_SIZE)
#define MCAN_TXFIFO_SIZE            8U
#define MCAN_MSG_ELEM_SIZE          MCAN_ELEM_SIZE_8BYTES

/*
 * PLL_CLK = 360MHz → CAN_CLK = PLL / 9 = 40MHz
 * 500Kbps: 40MHz / ((30+7+3) * (1+1)) = 40MHz / 80 = 500Kbps, SP = (1+30)/(1+30+7) = 82%
 */
#define CAN_CLK_SRC              CAN_CLK_TYPE_PLL
#define CAN_CLK_DIV              9U              /* PLL 360MHz / 9 = 40MHz */
#define CAN_CLK_FREQ             40000000U
#define CAN_CLK_PRESCALER        1U              /* 寄存器值, 有效分频 = 1+1 = 2 */
#define CAN_CLK_SEG1             30U
#define CAN_CLK_SEG2             7U

void CAN_Clock_Init(void)
{
    SysCtl_setCanClkSrcSel(CAN_CLK_SRC);
    SysCtl_setCanClkDiv(CAN_CLK_DIV);
}

void CAN_GPIO_Init(uint32_t tx_mux, uint32_t tx_pin, uint32_t rx_mux, uint32_t rx_pin)
{
    /* CAN TX */
    GPIO_setPinConfig(tx_mux);
    GPIO_setPadConfig(tx_pin, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(tx_pin, GPIO_QUAL_ASYNC);

    /* CAN RX */
    GPIO_setPinConfig(rx_mux);
    GPIO_setPadConfig(rx_pin, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(rx_pin, GPIO_QUAL_ASYNC);
}

void CAN_Module_Init(uint32_t base, uint32_t bitrate, IRQn_Type IRQn, IrqHandlerType handler, bool loopback)
{
    MCAN_InitParams_t           initParam;
    MCAN_ConfigParams_t         configParams;
    MCAN_MsgRAMConfigParams_t   ramConfig;
    MCAN_BitTimingParams_t      bitTiming;
    MCAN_StdMsgIDFilterElement_t stdFilter;

    /* ---- 1. MCAN 基本初始化参数 ---- */
    memset(&initParam, 0, sizeof(initParam));
    initParam.fdMode          = false;
    initParam.fdFormat        = MCAN_FD_ISO_11898_1;
    initParam.brsEnable       = false;
    initParam.darEnable       = true;
    initParam.wmMarker        = MCAN_WMM_8BIT_MODE;
    initParam.emulationEnable = false;
    initParam.tdcEnable       = false;

    /* ---- 2. 全局配置 ---- */
    memset(&configParams, 0, sizeof(configParams));
    configParams.monEnable         = false;
    configParams.asmEnable         = false;
    configParams.timeoutCntEnable  = false;
    configParams.tsClock           = MCAN_INTERNAL_TIMESTAMP;
    configParams.tsSelect          = MCAN_TSCNTVAL_ALWAYS0;
    configParams.tsPrescalar       = 1U;
    configParams.filterConfig.rrfs = true;
    configParams.filterConfig.rrfe = true;
    configParams.filterConfig.anfs = MCAN_NON_MATCH_REJECT_ACCEPT;
    configParams.filterConfig.anfe = MCAN_NON_MATCH_REJECT_ACCEPT;

    /* ---- 3. 消息 RAM 布局 ---- */
    memset(&ramConfig, 0, sizeof(ramConfig));
    ramConfig.lss                  = MCAN_STD_FILTER_NUM;
    ramConfig.flssa                = MCAN_STD_FILTER_ADDR;
    ramConfig.lse                  = 0U;
    ramConfig.flesa                = 0U;
    ramConfig.rxFIFO0startAddr     = MCAN_RXFIFO0_ADDR;
    ramConfig.rxFIFO0size          = MCAN_RXFIFO0_SIZE;
    ramConfig.rxFIFO0waterMark     = 0U;
    ramConfig.rxFIFO0OpMode        = MCAN_RXFIFO_OP_MODE_BLOCKING;
    ramConfig.rxFIFO0ElemSize      = MCAN_MSG_ELEM_SIZE;
    ramConfig.rxFIFO1size          = 0U;
    ramConfig.rxBufStartAddr       = 0U;
    ramConfig.txStartAddr          = MCAN_TXFIFO_ADDR;
    ramConfig.txBufMode            = MCAN_TXBUF_OP_IN_QUEUE_MODE;   /* Queue: 按 ID 优先级发送 */
    ramConfig.txFIFOSize           = MCAN_TXFIFO_SIZE;
    ramConfig.txBufNum             = 0U;
    ramConfig.txBufElemSize        = MCAN_MSG_ELEM_SIZE;
    ramConfig.txEventFIFOSize      = 0U;

    /* ---- 4. 比特率配置 ---- */
    memset(&bitTiming, 0, sizeof(bitTiming));
    bitTiming.nomRatePrescalar  = CAN_CLK_PRESCALER;
    bitTiming.nomTimeSeg1       = CAN_CLK_SEG1;
    bitTiming.nomTimeSeg2       = CAN_CLK_SEG2;
    bitTiming.nomSynchJumpWidth = 1U;

    /* ---- 5. 执行初始化序列 ---- */
    /* 等待复位释放 */
    if (!MCAN_isInReset(base)) {
        /* 已释放，直接进入初始化 */
    }
    /* 等待消息 RAM 就绪 */
    {
        volatile uint32_t _tout = 100000U;
        while (!MCAN_isMemInitDone(base) && --_tout) {}
    }
    /* 进入 SW Init 模式 */
    if (!MCAN_setOpMode(base, MCAN_OPERATION_SW_INIT_MODE)) {
        return;     /* ← GDB 断点: MCAN 无法进入 Init 模式，时钟可能未使能 */
    }
    /* 初始化 MCAN */
    if (!MCAN_init(base, &initParam)) {
        return;     /* ← GDB 断点: MCAN_init 失败 */
    }
    /* 全局配置 */
    if (!MCAN_config(base, &configParams)) {
        return;     /* ← GDB 断点: MCAN_config 失败 */
    }
    /* 比特率 */
    if (!MCAN_setBitTime(base, &bitTiming)) {
        return;     /* ← GDB 断点: MCAN_setBitTime 失败 */
    }
    /* 消息 RAM 配置 */
    if (!MCAN_msgRAMConfig(base, &ramConfig)) {
        return;     /* ← GDB 断点: MCAN_msgRAMConfig 失败 */
    }

    /* ---- 6. 标准 ID 滤波器 ---- */
    memset(&stdFilter, 0, sizeof(stdFilter));
    stdFilter.sfid1  = 0U;
    stdFilter.sfid2  = 0x7FFU;                                              /* 接受所有标准 ID */
    stdFilter.sfec   = MCAN_STDIDF_ELE_PRIO_STORE_IN_FO0_OF_MATCH_ID;
    stdFilter.sft    = MCAN_STDIDF_RANGE_FROM_SFID1_TO_SFID2;
    stdFilter.ssync  = MCAN_STDIDF_SYNC_MSG_DISABLE;
    if (!MCAN_addStdMsgIDFilter(base, 0U, &stdFilter)) {
        return;     /* ← GDB 断点: 滤波器配置失败 */
    }

    /* ---- 7. 中断配置 ---- */
    MCAN_enableIntr(base, MCAN_INT_SRC_RxFIFO0_NEW_MSG, true);
    MCAN_selectIntrLine(base, MCAN_INT_SRC_RxFIFO0_NEW_MSG, MCAN_INTERRUPT_LINE_0);
    MCAN_enableIntrLine(base, MCAN_INTERRUPT_LINE_0, true);

    Interrupt_SetPriority(IRQn, 0U, 0U);
    Interrupt_register(IRQn, handler);
    Interrupt_enable(IRQn);

    /* ---- 8. 环回模式（必须在 Normal 模式前设置）---- */
    if (loopback) {
        MCAN_lpbkModeEnable(base, MCAN_LPBK_MODE_INTERNAL, true);
    }

    /* ---- 9. 进入正常模式 ---- */
    if (!MCAN_setOpMode(base, MCAN_OPERATION_NORMAL_MODE)) {
        return;
    }
}

bool CAN_SendMsg(uint32_t base, uint32_t msgId, uint8_t *pData, uint8_t dlc)
{
    MCAN_TxMessage_t txMsg;

    memset(&txMsg, 0, sizeof(txMsg));

    /* 标准帧 ID 需要移位到 bit 18 */
    txMsg.id  = ((msgId & 0x7FFU) << 18U);
    txMsg.rtr = 0U;                       /* 数据帧 */
    txMsg.xtd = 0U;                       /* 标准帧 (11-bit ID) */
    txMsg.esi = 0U;                       /* 错误状态指示 */
    txMsg.dlc = dlc;                      /* 数据长度 (0-8) */
    txMsg.brs = 0U;                       /* 无比特率切换 */
    txMsg.fdf = 0U;                       /* CAN 2.0 帧 */
    txMsg.efc = 0U;
    txMsg.mm  = 0U;

    if (dlc > 8U) dlc = 8U;
    if (pData != NULL) {
        uint8_t i;
        for (i = 0U; i < dlc; i++) {
            txMsg.data[i] = pData[i];
        }
    }

    /* 使用 TX FIFO/Queue 发送 */
    return MCAN_transmitMsgFifoQueue(base, &txMsg);
}
