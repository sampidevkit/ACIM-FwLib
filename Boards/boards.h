#ifndef BOARDS_H
#define	BOARDS_H

#include "Common/LibDef.h"

/* ******************************************************************* System */
void System_Init(void);
void ClrWdt(void);
/* ************************************************************ Power control */
void VDC_Enable(void);
void VDC_Disable(void);
/* ********************************************************* Dev mode control */
void DevMode_Enable(void);
void DevMode_Disable(void);
/* **************************************************** LED for running state */
void LedRun_On(void);
void LedRun_Off(void);
void LedRun_Toggle(void);
/* ****************************************************** LED for error state */
void LedErr_On(void);
void LedErr_Off(void);
void LedErr_Toggle(void);
/* ******************************************************************* Button */
bool Button_Get(void);
/* ********************************************************** Data visualizer */
size_t DV_Write(uint8_t* pWrBuffer, const size_t size);
/* *************************************************************** PFC analog */
uint16_t PFC_ADC_GetVacChannel(void);
uint16_t PFC_ADC_GetVdcChannel(void);
uint16_t PFC_ADC_GetIdcChannel(void);
uint16_t PFC_ADC_GetIaChannel(void);
uint16_t PFC_ADC_GetIbChannel(void);
void PFC_ADC_SetCallback(void (*callback)(uint32_t channel, uintptr_t context));
/* ****************************************************************** PFC PWM */
uint16_t PFC_PWM_GetDutyMax(void);
void PFC_PWM_SetDutyA(uint16_t Duty);
void PFC_PWM_SetDutyB(uint16_t Duty);
void PFC_PWM_SetCallback(void (*callback) (uint32_t status, uintptr_t context));
/* ************************************************************** INV encoder */
int32_t INV_ENC_GetSpeed(void);
/* *************************************************************** INV analog */
uint16_t INV_ADC_GetVdcChannel(void);
uint16_t INV_ADC_GetIdcChannel(void);
uint16_t INV_ADC_GetIuChannel(void);
uint16_t INV_ADC_GetIvChannel(void);
uint16_t INV_ADC_GetVuChannel(void);
uint16_t INV_ADC_GetVvChannel(void);
uint16_t INV_ADC_GetSpeedRef(void);
void INV_ADC_SetCallback(void (*callback)(uintptr_t context));
void INV_ADC_InterruptEnable(void);
void INV_ADC_InterruptDisable(void);
void INV_ADC_InterruptClear(void);
/* ****************************************************************** INV PWM */
void INV_PWM_SetDuty(uint16_t DutyU, uint16_t DutyV, uint16_t DutyW);
void INV_PWM_SetCallback(void (*callback) (uint32_t status, uintptr_t context));
void INV_PWM_InterruptEnable(void);
void INV_PWM_InterruptDisable(void);
void INV_PWM_InterruptClear(void);
void INV_PWM_Start(void); // enable all PWM channels
void INV_PWM_U_Enable(void); // enable PWM channel U
void INV_PWM_V_Enable(void); // enable PWM channel V
void INV_PWM_W_Enable(void); // enable PWM channel W
void INV_PWM_Disable(void); // disable all PWM channels
void INV_PWM_U_Disable(void); // disable PWM channel U
void INV_PWM_V_Disable(void); // disable PWM channel V
void INV_PWM_W_Disable(void); // disable PWM channel W

#endif
