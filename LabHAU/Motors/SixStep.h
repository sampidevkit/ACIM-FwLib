#ifndef SIXSTEP_H
#define	SIXSTEP_H

#include "../Common/LibDef.h"
#include "config/labhau/labhau_cfg.h"

typedef struct {
    void (*Start)(void);
    void (*Stop)(void);
    void (*SetPeriod)(uint16_t ms);
    void (*SetCallbackHandler)(void);
} sixstep_deadtmr_t;

extern sixstep_deadtmr_t DeadTmrCxt; // Defined in SixStep_Cfg.c

// Called 1 time in APP_Initialize()
// Dtime is max periode for an state, value is in ms
#define SIXSTEP_DEADTIME_DEFAULT 100
void SixStep_Init(uint32_t Dtime);

// Disable all switches
void SixStep_Stop(void);

// Set state
// step=[0:5]
void SixStep_SetState(uint8_t step);

#endif
