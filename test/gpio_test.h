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

#ifndef GPIO_TEST_H_
#define GPIO_TEST_H_

#include "device.h"
#include "bsp_gpio.h"

#define INT1_GPIO_MUX           GPIO_233_GPIO233
#define INT1_GPIO               233
#define INT2_GPIO_MUX           GPIO_237_GPIO237
#define INT2_GPIO               237
#define INT3_GPIO_MUX           GPIO_231_GPIO231
#define INT3_GPIO               231
#define INT4_GPIO_MUX           GPIO_238_GPIO238
#define INT4_GPIO               238
#define INT5_GPIO_MUX           GPIO_241_GPIO241
#define INT5_GPIO               241
#define INT6_GPIO_MUX           GPIO_225_GPIO225
#define INT6_GPIO               225

extern volatile uint32_t int1_cnt;
extern volatile uint32_t int2_cnt;
extern volatile uint32_t int3_cnt;
extern volatile uint32_t int4_cnt;
extern volatile uint32_t int5_cnt;
extern volatile uint32_t int6_cnt;

void gpio_test_init(void);
void gpio_test_run(void);

#endif /* GPIO_TEST_H_ */
