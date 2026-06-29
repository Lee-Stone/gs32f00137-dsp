/*
 * SFO.h
 *
 *  Created on: 2025 May 21
 *      Author: hpec
 */

#ifndef SFO_V2_1_H_
#define SFO_V2_1_H_

#include <string.h>

//*****************************************************************************
//! Values that can be passed to SFO().
//*****************************************************************************
typedef enum SFO_Status
{
	SFO_Success = 0,		/*	No Error  */
	EPWMVERSION_Error = 1,  /* Version number error */
	LoopCnt_Error = 2,	    /* LoopCnt Cannot be zero */
	HRMStep_Error = 3,	    /* Exceeding the maximum delay */
	Clock_Ratio_Error = 4,	/* PLL and AHB  clock ratio read and write do not match */
	Cycle_Error = 5,		/* Insufficient delay time */
	Pointer_Error = 6,		/* Illegal pointer */
	PLLCLK_Error = 7,		/* PLLCLK is too high/low */
	DLL_Unlock_Error = 8,	/* DLLCLK is unlocked */
	Cycle_Error_Default = 9, /* Excessive voltage or temperature, either too high or too low */
}SFO_Status_t;

typedef struct SFO_Params
{
	uint32_t PLL_CLK;
	uint32_t AHB_CLK;
	uint32_t EPWMVERSION;
	uint16_t loop_cnt;
	uint16_t Cycle_Delay;
	uint16_t HR_mstep;
	uint16_t HR_mstep_Auto;
}SFO_Params_t;

typedef struct SFOLib_Version
{
	uint32_t	Version;
	uint16_t	Major;
	uint16_t	Minor;
	uint16_t	Revision;
}SFOLib_Version_t;

SFO_Status_t SFO(SFO_Params_t *SFO_ParamsStruct);
uint8_t Get_SFOLib_Version(SFOLib_Version_t *SFOLib_Version);

static inline uint8_t
SFO_Struct_deinit(SFO_Params_t *SFO_ParamsStruct)
{
	if(NULL == SFO_ParamsStruct)
	{
		return 1;
	}
	SFO_ParamsStruct->AHB_CLK = 0;
	SFO_ParamsStruct->EPWMVERSION = 0;
	SFO_ParamsStruct->HR_mstep = 0;
	SFO_ParamsStruct->PLL_CLK = 0;
	SFO_ParamsStruct->loop_cnt = 0;
	SFO_ParamsStruct->HR_mstep_Auto = 0;
	SFO_ParamsStruct->Cycle_Delay = 0;
	return 0;
}

static inline uint8_t
SFO_Struct_init(SFO_Params_t *SFO_ParamsStruct,uint16_t loop_cnt)
{
	if(NULL == SFO_ParamsStruct)
	{
		return 1;
	}
#ifdef DEVICE_AHBCLK_DIV
# ifdef DEVICE_PLLCLK_FREQ
	SFO_ParamsStruct->PLL_CLK =  DEVICE_PLLCLK_FREQ;
	SFO_ParamsStruct->AHB_CLK = DEVICE_AHBCLK_FREQ;
# else
	SFO_ParamsStruct->PLL_CLK =  DEVICE_SYSCLK_FREQ*2;
	SFO_ParamsStruct->AHB_CLK = DEVICE_AHBCLK_FREQ;
# endif
#else
# ifdef DEVICE_PLLCLK_FREQ
	SFO_ParamsStruct->PLL_CLK = DEVICE_PLLCLK_FREQ;
	SFO_ParamsStruct->AHB_CLK = DEVICE_SYSCLK_FREQ/2;
# else
	SFO_ParamsStruct->PLL_CLK = DEVICE_SYSCLK_FREQ*2;
	SFO_ParamsStruct->AHB_CLK = DEVICE_SYSCLK_FREQ/2;
# endif
#endif
	SFO_ParamsStruct->EPWMVERSION = EPWM_VERSION;
	SFO_ParamsStruct->loop_cnt = loop_cnt;
#if (EPWM_VERSION == 12)
#elif (EPWM_VERSION == 22)||(EPWM_VERSION == 30)
	SFO_ParamsStruct->HR_mstep_Auto = 256;
#endif
	return 0;
}

#endif /* SFO_V2_H_ */
