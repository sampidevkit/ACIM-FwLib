#ifndef PWRMONI_H
#define	PWRMONI_H

#include "../Common/LibDef.h"
#include "config/labHau/labHau_Cfg.h"

typedef enum {
    PWRMONI_IU,
    PWRMONI_IV,
    PWRMONI_IW,
    PWRMONI_VU,
    PWRMONI_VV,
    PWRMONI_VW,
    PWRMONI_VDC,
    PWRMONI_IDC,
    PWRMONI_TEMP,
    PWRMONI_ALL
} pwrmoni_ch_t;

typedef struct {
    int32_t V;
    int32_t I;
} pwrmoni_phase_t;

typedef struct {
    pwrmoni_phase_t PhaseU;
    pwrmoni_phase_t PhaseV;
    pwrmoni_phase_t PhaseW;
    pwrmoni_phase_t Main;
    int32_t Temp;
} pwrmoni_cxt_t;

typedef struct
{
    
} pwrmoni_fnc_t;

extern pwrmoni_fnc_t PwrMoniCxt;

void PwrMoni_Init(void);
void PwrMoni_Tasks(void);
void PwrMoni_Get(pwrmoni_ch_t channel, void *pResult);

#endif
