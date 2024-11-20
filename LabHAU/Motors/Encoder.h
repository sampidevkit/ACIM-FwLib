#ifndef ENCODER_H
#define	ENCODER_H

#include "../Common/LibDef.h"
#include "config/labhau/labhau_cfg.h"

typedef struct
{
    
} encoder_t;

extern encoder_t encoderCxt;

void Encoder_Init(void);
uint32_t Encoder_GetSpeed(void);

#endif
