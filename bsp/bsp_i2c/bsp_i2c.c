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
#include "bsp_i2c.h"

/* ---- I2C统一初始化（3步骤） ---- */
void I2C_Init(uint32_t base, uint32_t bitrate)
{
	/* 第1步：使能外设时钟 */
    I2C_Clock_Init();
    /* 第2步：配置引脚复用 */
    I2C_GPIO_Init(BSP_I2C_SDA_GPIO_MUX, BSP_I2C_SDA_GPIO, BSP_I2C_SCL_GPIO_MUX, BSP_I2C_SCL_GPIO);
    /* 第3步：配置外设寄存器 */
    I2C_Module_Init(base, bitrate);
}

void I2C_Clock_Init(void)
{
    /*
     * Actual clock initialization is performed in Device_init()
     * */
}

void I2C_GPIO_Init(uint32_t sda_mux, uint32_t sda_pin, uint32_t scl_mux, uint32_t scl_pin)
{
    /* SDA */
    GPIO_setPinConfig(sda_mux);
    GPIO_setPadConfig(sda_pin, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(sda_pin, GPIO_QUAL_ASYNC);
    GPIO_setStrength(sda_pin, GPIO_DRV_20MA);

    /* SCL */
    GPIO_setPinConfig(scl_mux);
    GPIO_setPadConfig(scl_pin, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(scl_pin, GPIO_QUAL_ASYNC);
    GPIO_setStrength(scl_pin, GPIO_DRV_20MA);
}

void I2C_Module_Init(uint32_t base, uint32_t bitrate)
{
    /* 先将 I2C 置于复位状态 */
    I2C_disableModule(base);
    /* 控制器模式初始化 */
    I2C_initController(base, DEVICE_APBCLK_FREQ, bitrate * 1000U, I2C_DUTYCYCLE_50);
    /* 数据位为 8 位 */
    I2C_setBitCount(base, I2C_BITCOUNT_8);
    /* 7 位地址模式 */
    I2C_setAddressMode(base, I2C_ADDR_MODE_7BITS);
    /* 禁用 FIFO（ 使用直接寄存器查询XRDY/RRDY） */
    I2C_disableFIFO(base);
    /* 调试模式：挂起时自由运行 */
    I2C_setEmulationMode(base, I2C_EMULATION_FREE_RUN);
    /* 扩展模式：广播模式 */
    I2C_setExtendedMode(base, I2C_EADDR_MODE_BC);
    /* 使能模块 */
    I2C_enableModule(base);
}

/* ---- 内部 NACK 处理 ---- */
static uint16_t I2C_HandleNACK(uint32_t base)
{
    if (I2C_getStatus(base) & I2C_STS_NO_ACK) {
        I2C_clearStatus(base, I2C_STS_NO_ACK);
        I2C_sendStopCondition(base);
        return BSP_I2C_ERROR_NACK;
    }
    return BSP_I2C_SUCCESS;
}

uint16_t I2C_WriteData(uint32_t base, uint8_t devAddr, uint16_t memAddr,
                       uint8_t *pData, uint16_t dataLen)
{
    uint16_t addr[2];
    uint16_t i;
    uint16_t status;
    uint16_t attemptCount;

#if (BSP_EEPROM_ADDR_LENGTH == 2)
    addr[0] = (uint16_t)((memAddr >> 8) & 0xFFU);
    addr[1] = (uint16_t)(memAddr & 0xFFU);
#else
    addr[0] = (uint16_t)(memAddr & 0xFFU);
#endif

    /* 等待上次 STOP 完成 */
    if (I2C_getStopConditionStatus(base)) {
        return BSP_I2C_ERROR_STOP_NOT_READY;
    }

    /* 设置目标设备地址 */
    I2C_setTargetAddress(base, devAddr);

    /* 总线忙检测 */
    if (I2C_isBusBusy(base)) {
        return BSP_I2C_ERROR_BUS_BUSY;
    }

    /* 配置为控制器发送模式 */
    I2C_setConfig(base, I2C_CONTROLLER_SEND_MODE);

    /* 总数据量：内存地址字节 + 数据字节 */
    I2C_setDataCount(base, (uint16_t)(BSP_EEPROM_ADDR_LENGTH + dataLen));

    /* 发送 START */
    I2C_sendStartCondition(base);

    /* ---- 写入 EEPROM 内存地址 ---- */
    for (i = 0U; i < BSP_EEPROM_ADDR_LENGTH; i++) {
        I2C_putData(base, addr[i]);

        /* 等待发送缓冲就绪（XRDY） */
        while (!(I2C_getStatus(base) & I2C_STS_TX_DATA_RDY)) {}

        /* NACK 检查（可重试） */
        attemptCount = 0U;
        while (attemptCount <= 5000U) {
            status = I2C_HandleNACK(base);
            if (status == BSP_I2C_SUCCESS) break;
            attemptCount++;
            DEVICE_DELAY_US(1);
        }
        if (status != BSP_I2C_SUCCESS) {
            return status;
        }
    }

    /* ---- 写入数据字节 ---- */
    for (i = 0U; i < dataLen; i++) {
        I2C_putData(base, pData[i]);

        /* 等待发送缓冲就绪（XRDY） */
        while (!(I2C_getStatus(base) & I2C_STS_TX_DATA_RDY)) {}

        /* NACK 检测 */
        attemptCount = 0U;
        while (attemptCount <= 5000U) {
            status = I2C_HandleNACK(base);
            if (status == BSP_I2C_SUCCESS) break;
            attemptCount++;
            DEVICE_DELAY_US(1);
        }
        if (status != BSP_I2C_SUCCESS) {
            return status;
        }
    }

    /* 发送 STOP */
    I2C_sendStopCondition(base);

    /* 等待 STP 位硬件清零 */
    attemptCount = 0U;
    while (I2C_getStopConditionStatus(base) && attemptCount <= 3U) {
        DEVICE_DELAY_US(20);
        attemptCount++;
    }

    return BSP_I2C_SUCCESS;
}

uint16_t I2C_ReadData(uint32_t base, uint8_t devAddr, uint16_t memAddr,
                      uint8_t *pData, uint16_t dataLen)
{
    uint16_t addr[2];
    uint16_t i;
    uint16_t status;
    uint16_t attemptCount;

#if (BSP_EEPROM_ADDR_LENGTH == 2)
    addr[0] = (uint16_t)((memAddr >> 8) & 0xFFU);
    addr[1] = (uint16_t)(memAddr & 0xFFU);
#else
    addr[0] = (uint16_t)(memAddr & 0xFFU);
#endif

    /* 等待上次 STOP 完成 */
    if (I2C_getStopConditionStatus(base)) {
        return BSP_I2C_ERROR_STOP_NOT_READY;
    }

    /* 设置目标设备地址 */
    I2C_setTargetAddress(base, devAddr);

    /* 总线忙检测 */
    if (I2C_isBusBusy(base)) {
        return BSP_I2C_ERROR_BUS_BUSY;
    }

    /* ---- Step 1: 发送内存地址（无 STOP） ---- */
    I2C_setConfig(base, I2C_CONTROLLER_SEND_MODE);
    I2C_setDataCount(base, BSP_EEPROM_ADDR_LENGTH);
    I2C_sendStartCondition(base);

    for (i = 0U; i < BSP_EEPROM_ADDR_LENGTH; i++) {
        I2C_putData(base, addr[i]);

        /* 等待发送缓冲就绪（XRDY） */
        while (!(I2C_getStatus(base) & I2C_STS_TX_DATA_RDY)) {}

        /* NACK 检测 */
        attemptCount = 0U;
        while (attemptCount <= 5000U) {
            status = I2C_HandleNACK(base);
            if (status == BSP_I2C_SUCCESS) break;
            attemptCount++;
            DEVICE_DELAY_US(1);
        }
        if (status != BSP_I2C_SUCCESS) {
            return status;
        }
    }

    /* ---- Step 2: RESTART + 接收数据 ---- */
    I2C_setConfig(base, I2C_CONTROLLER_RECEIVE_MODE);
    I2C_setDataCount(base, dataLen);
    I2C_sendStartCondition(base);       /* RESTART */

    for (i = 0U; i < dataLen; i++) {
        /* 等待接收数据就绪（RRDY） */
        while (!(I2C_getStatus(base) & I2C_STS_RX_DATA_RDY)) {}

        pData[i] = (uint8_t)I2C_getData(base);

        /* NACK 检测 */
        attemptCount = 0U;
        while (attemptCount <= 5000U) {
            status = I2C_HandleNACK(base);
            if (status == BSP_I2C_SUCCESS) break;
            attemptCount++;
            DEVICE_DELAY_US(1);
        }
        if (status != BSP_I2C_SUCCESS) {
            return status;
        }
    }

    /* 发送 STOP */
    I2C_sendStopCondition(base);

    /* 等待 STP 位硬件清零 */
    attemptCount = 0U;
    while (I2C_getStopConditionStatus(base) && attemptCount <= 3U) {
        DEVICE_DELAY_US(20);
        attemptCount++;
    }

    return BSP_I2C_SUCCESS;
}

