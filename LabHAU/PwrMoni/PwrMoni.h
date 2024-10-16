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
    PWRMONI_TEMP
} pwrmoni_ch_t;

void PwrMoni_Init(void);
void PwrMoni_Tasks(void);

#endif
