#ifndef BOARDS_H
#define	BOARDS_H

#include "definitions.h"
#include "labhau_cfg.h"
#include "Common/LibDef.h"

void VDC_Enable(void);
void VDC_Disable(void);

void DevMode_Enable(void);
void DevMode_Disable(void);

void LedRun_On(void);
void LedRun_Off(void);
void LedRun_Toggle(void);

void LedErr_On(void);
void LedErr_Off(void);
void LedErr_Toggle(void);

bool Button_Get(void);

size_t DV_Write(uint8_t* pWrBuffer, const size_t size);

int32_t ADC_GetInvVdc(void);
int32_t ADC_GetInvIdc(void);
int32_t ADC_GetInvIu(void);
int32_t ADC_GetInvIv(void);
int32_t ADC_GetInvVu(void);
int32_t ADC_GetInvVv(void);
int32_t ENC_GetSpeed(void);

int32_t ADC_GetPfcVac(void);
int32_t ADC_GetPfcVdc(void);
int32_t ADC_GetPfcIdc(void);
int32_t ADC_GetPfcIa(void);
int32_t ADC_GetPfcIb(void);

void PFCPWM_SetDutyA(uint16_t Duty);
void PFCPWM_SetDutyB(uint16_t Duty);

void INVPWM_SetDuty(uint16_t DutyU, uint16_t DutyV, uint16_t DutyW);

#endif
