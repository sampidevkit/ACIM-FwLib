#include "SixStep.h"
#include "../System/Tick.h"

static uint8_t lastStep=6;
static tick_cxt_t Tick={1, 0, 0};

void SixStep_Init(uint32_t Dtime)
{
    SixStep_Stop();
    DeadTmrCxt.SetPeriod(Dtime);
    DeadTmrCxt.SetCallbackHandler();
    Tick_Reset(&Tick);
}

inline void SixStep_Stop(void)
{
    lastStep=6; // Stop state
    // Turn off all switches
    MC_PWM_UH_Clear();
    MC_PWM_VH_Clear();
    MC_PWM_WH_Clear();
    MC_PWM_UL_Clear();
    MC_PWM_VL_Clear();
    MC_PWM_WL_Clear();
    // Waiting for all switches are off
    while(MC_PWM_UH_Get());
    while(MC_PWM_VH_Get());
    while(MC_PWM_WH_Get());
    while(MC_PWM_UL_Get());
    while(MC_PWM_VL_Get());
    while(MC_PWM_WL_Get());

    DeadTmrCxt.Stop();
}

void SixStep_SetState(uint8_t step)
{
    // Turn off all switches
    MC_PWM_UH_Clear();
    MC_PWM_VH_Clear();
    MC_PWM_WH_Clear();
    MC_PWM_UL_Clear();
    MC_PWM_VL_Clear();
    MC_PWM_WL_Clear();
    // Waiting for all switches are off
    while(MC_PWM_UH_Get());
    while(MC_PWM_VH_Get());
    while(MC_PWM_WH_Get());
    while(MC_PWM_UL_Get());
    while(MC_PWM_VL_Get());
    while(MC_PWM_WL_Get());
    // Update new state
    switch(step)
    {
        case 0: // Rotor position (deg)=[0; 60) <=> [U, V, W]=[VDC, GND, OFF]
            MC_PWM_UH_Set();
            MC_PWM_VL_Set();
            break;

        case 1: // Rotor position (deg)=[60; 120) <=> [U, V, W]=[VDC, OFF, GND]
            MC_PWM_UH_Set();
            MC_PWM_WL_Set();
            break;

        case 2: // Rotor position (deg)=[120; 180) <=> [U, V, W]=[OFF, VDC, GND]
            MC_PWM_VH_Set();
            MC_PWM_WL_Set();
            break;

        case 3: // Rotor position (deg)=[180; 240) <=> [U, V, W]=[GND, VDC, OFF]
            MC_PWM_VH_Set();
            MC_PWM_UL_Set();
            break;

        case 4: // Rotor position (deg)=[240; 300) <=> [U, V, W]=[GND, OFF, VDC]
            MC_PWM_WH_Set();
            MC_PWM_UL_Set();
            break;

        case 5: // Rotor position (deg)=[300; 360) <=> [U, V, W]=[OFF, GND, VDC]
            MC_PWM_WH_Set();
            MC_PWM_VL_Set();
            break;

        default: // exception - stop the motor
            SixStep_Stop();
            return;
    }

    if(lastStep!=step)
    {
        DeadTmrCxt.Start();
        lastStep=step;
    }
}

void SixStep_Test(uint32_t delay)
{
    if(Tick_Is_Over_Ms(Tick, delay))
    {
        if(lastStep>4)
            SixStep_SetState(0);
        else
            SixStep_SetState(lastStep+1);
    }
}