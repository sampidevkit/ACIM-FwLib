#ifndef THREEPHASEPWM_H
#define	THREEPHASEPWM_H

#include "../Common/LibDef.h"
#include "config/labHau/labHau_Cfg.h"

typedef enum {
    PWM_U,
    PWM_V,
    PWM_W
} phase_t;

typedef struct {
    void (*SetFreq)(uint32_t fpwm);
    void (*SetDutyU)(uint16_t duty);
    void (*SetDutyV)(uint16_t duty);
    void (*SetDutyW)(uint16_t duty);
    void (*SetDuty3P)(uint16_t dutyU, uint16_t dutyV, uint16_t dutyW);
} threephase_pwm_t;

extern threephase_pwm_t Pwm3PCxt; // Defined in ThreePhasePWM_Cfg.c

void ThreePhasePWM_Init(uint32_t fpwm);
void ThreePhasePWM_SetDuty(phase_t phase, uint16_t duty);
void ThreePhasePWM_SetDutyPercent(phase_t phase, float percent);
void ThreePhasePWM_SetDuty3P(uint16_t dutyU, uint16_t dutyV, uint16_t dutyW);
void ThreePhasePWM_SetDutyPercent3P(float percentU, float percentV, float percentW);

#endif
