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

#ifndef BSP_I2C_H_
#define BSP_I2C_H_

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"

/* ---- Base ---- */
#define BSP_I2C_BASE            I2CA_BASE

/* ---- Pin definitions ---- */
#define BSP_I2C_SDA_GPIO            35
#define BSP_I2C_SCL_GPIO            37
#define BSP_I2C_SDA_GPIO_MUX        GPIO_35_I2CA_SDA
#define BSP_I2C_SCL_GPIO_MUX        GPIO_37_I2CA_SCL

#define BSP_EEPROM_ADDR_LENGTH      2U           /* AT24C64 使用 16 位内存地址 */

/* ---- 返回值 ---- */
#define BSP_I2C_SUCCESS             	0x0000U
#define BSP_I2C_ERROR_STOP_NOT_READY 	0x5555U
#define BSP_I2C_ERROR_BUS_BUSY      	0x1000U
#define BSP_I2C_ERROR_NACK          	0x2000U

void I2C_Init(uint32_t base, uint32_t bitrate);
void I2C_Clock_Init(void);
void I2C_GPIO_Init(uint32_t sda_mux, uint32_t sda_pin, uint32_t scl_mux, uint32_t scl_pin);
void I2C_Module_Init(uint32_t base, uint32_t bitrate);

uint16_t I2C_WriteData(uint32_t base, uint8_t devAddr, uint16_t memAddr, uint8_t *pData, uint16_t dataLen);
uint16_t I2C_ReadData(uint32_t base, uint8_t devAddr, uint16_t memAddr, uint8_t *pData, uint16_t dataLen);

#endif /* BSP_I2C_H_ */
