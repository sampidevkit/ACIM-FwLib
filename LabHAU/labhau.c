#include "labhau.h"

void LABHAU_Init(void)
{
    DevMode_Enable();
    LedErr_Off();
    LedRun_On();

    /* **************************************************** Load ADC variables*/
    PfcCnt=0;
    InvCnt=0;


    EVIC_SourceDisable(INT_SOURCE_ADC_DATA0);
    EVIC_SourceStatusClear(INT_SOURCE_ADC_DATA0);
    ADCHS_CallbackRegister(ADCHS_CH0, (ADCHS_CALLBACK) InvAdc_IntCb, (uintptr_t) NULL);
    EVIC_SourceEnable(INT_SOURCE_ADC_DATA0);
    MCPWM_CallbackRegister(MCPWM_CH_12, InvPwm_IntCb, (uintptr_t) NULL);
    EVIC_SourceEnable(INT_SOURCE_PWM12);
    MCPWM_Start();
    MCPWM_ChannelPinsOwnershipDisable(MCPWM_CH_1);
    MCPWM_ChannelPinsOwnershipDisable(MCPWM_CH_2);
    MCPWM_ChannelPinsOwnershipDisable(MCPWM_CH_3);
    VDC_Enable();
}
