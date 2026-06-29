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

#include "i2c_test.h"
#include "bsp_i2c.h"

/* ---- I2C EEPROM 读写数组 ---- */
volatile uint8_t eeprom_read_data[5] = {0x00, 0x01, 0x02, 0x03, 0x04};
volatile uint8_t eeprom_write_data[5] = {0};

void i2c_test_init(void)
{
    /* ---- I2C EEPROM ---- */
    I2C_GPIO_Init(AT24C64_SDA_GPIO_MUX, AT24C64_SDA_GPIO, AT24C64_SCL_GPIO_MUX, AT24C64_SCL_GPIO);
    I2C_Module_Init(AT24C64_I2C_BASE, AT24C64_I2C_BITRATE);
}

void i2c_test_run(void)
{
	/* 上电读取 EEPROM 验证上次写入数据，GDB 调试可查看 eeprom_read_data */
	I2C_ReadData(AT24C64_I2C_BASE, AT24C64_I2C_ADDR, 0x00, (uint8_t *)&eeprom_read_data, sizeof(eeprom_read_data));

	while(1)
	{
		/* 通过 GDB 调试修改 eeprom_write_data */
		I2C_WriteData(I2CA_BASE, 0x50, 0x00, (uint8_t *)&eeprom_write_data, sizeof(eeprom_write_data));
		DELAY_US(1000 * 5000);
	}
}
