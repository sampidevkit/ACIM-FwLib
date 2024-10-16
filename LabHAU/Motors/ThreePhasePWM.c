#include "ThreePhasePWM.h"

void ThreePhasePWM_Init(uint32_t fpwm)
{
    Pwm3PCxt.SetFreq(fpwm);
}

inline void ThreePhasePWM_SetDuty(phase_t phase, uint16_t duty)
{
    switch(phase)
    {
        case PWM_U:
            Pwm3PCxt.SetDutyU(duty);
            break;

        case PWM_V:
            Pwm3PCxt.SetDutyV(duty);
            break;

        case PWM_W:
            Pwm3PCxt.SetDutyW(duty);
            break;

        default:
            break;
    }
}

inline void ThreePhasePWM_SetDutyPercent(phase_t phase, float percent)
{
    uint16_t duty;

    percent=(percent*65536.0)/100.0;
    duty=(uint16_t) percent;

    switch(phase)
    {
        case PWM_U:
            Pwm3PCxt.SetDutyU(duty);
            break;

        case PWM_V:
            Pwm3PCxt.SetDutyV(duty);
            break;

        case PWM_W:
            Pwm3PCxt.SetDutyW(duty);
            break;

        default:
            break;
    }
}

inline void ThreePhasePWM_SetDuty3P(uint16_t dutyU, uint16_t dutyV, uint16_t dutyW)
{
    Pwm3PCxt.SetDuty3P(dutyU, dutyV, dutyW);
}

inline void ThreePhasePWM_SetDutyPercent3P(float percentU, float percentV, float percentW)
{
    uint16_t dutyU, dutyV, dutyW;

    percentU=(percentU*65536.0)/100.0;
    dutyU=(uint16_t) percentU;

    percentV=(percentV*65536.0)/100.0;
    dutyV=(uint16_t) percentV;

    percentW=(percentW*65536.0)/100.0;
    dutyW=(uint16_t) percentW;

    Pwm3PCxt.SetDuty3P(dutyU, dutyV, dutyW);
}
