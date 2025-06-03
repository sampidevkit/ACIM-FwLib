#include "boards.h"
#include "Algorithms/MC.h"

inv_cxt_t InvCxt={
    .PwmDutyMax=3000, // See in MCPWM_PrimaryPeriodGet()
    .AdcVref=3300, // mV
    .AdcReso=4096, // 12bit
    .Encoder.Reso=2000, // 2000 pulses per round
    .InterVref.Gain=(float) 1200, // for Vband gap=1.2V
    .PhaseU.Cur.Gain=(float) 10.0, // 100mV/A
    .PhaseV.Cur.Gain=(float) 10.0, // 100mV/A
    .Source.Cur.Gain=(float) 10.0, // 100mV/A
    .Source.Vol.Gain=(float) 751.753507, // See in schematic page 5
    .Source.Vol.Offset=3 // ADC value
};

/* ******************************************************************* System */
bool System_Init(void) // <editor-fold defaultstate="collapsed" desc="System initialize">
{
    printf("\r\n\r\nLABHAU ACIM INVERTER");
    printf("\r\nPCB: HW.ACIM-MK1");
    printf("\r\nWINDOWS HOST");
    printf("\r\nSW: %s-%s", __DATE__, __TIME__);
    printf("\r\n\r\n");
} // </editor-fold>

inline void ClrWdt(void) // <editor-fold defaultstate="collapsed" desc="Clear Watching-dog-timer">
{

} // </editor-fold>

/* ************************************************************ Power control */
void VDC_Enable(void)
{
    printf("\r\nEnable VDC");
}

void VDC_Disable(void)
{
    printf("\r\nDisable VDC");
}

/* ********************************************************* Dev mode control */
void DevMode_Enable(void)
{
    printf("\r\nEnable DevMode");
}

void DevMode_Disable(void)
{
    printf("\r\nDisable DevMode");
}

/* **************************************************** LED for running state */
inline void LedRun_On(void)
{

}

inline void LedRun_Off(void)
{

}

inline void LedRun_Toggle(void)
{

}

/* ****************************************************** LED for error state */
inline void LedErr_On(void)
{
    
}

inline void LedErr_Off(void)
{

}

inline void LedErr_Toggle(void)
{

}

/* ******************************************************** LED for MCU state */
inline void LedMcu_On(void)
{

}

inline void LedMcu_Off(void)
{

}

inline void LedMcu_Toggle(void)
{

}

/* ******************************************************************* Button */
inline bool Button_Get(void)
{
    return 1; // no button
}

/* ********************************************************** Data visualizer */
inline size_t DV_Write(uint8_t* pWrBuffer, const size_t size)
{
    return UART2_Write(pWrBuffer, size);
}

/* ************************************************************** INV encoder */
inline int32_t INV_ENC_GetSpeed(void)
{
    return 0;
}

/* *************************************************************** INV analog */
inline void INV_ADC_InterruptEnable(void)
{

}

inline void INV_ADC_InterruptDisable(void)
{

}

inline bool INV_ADC_ResultIsReady(void)
{

    return ready;
}

inline void INV_ADC_SetInterruptCallback(void (*fnc)(void))
{
    ADCHS_EOSCallbackRegister((ADCHS_EOS_CALLBACK) fnc, (uintptr_t) NULL);
}

inline uint16_t INV_ADC_GetInternalVrefChannel(void)
{
    return ADCHS_ChannelResultGet(ADCHS_CH50); // AN50 - Internal Vref=1200mV
}

inline uint16_t INV_ADC_GetVdcChannel(void)
{
    return ADCHS_ChannelResultGet(ADCHS_CH5); // AN6
}

inline uint16_t INV_ADC_GetIdcChannel(void)
{
    return ADCHS_ChannelResultGet(ADCHS_CH2); // AN2
}

inline uint16_t INV_ADC_GetIuChannel(void)
{
    return ADCHS_ChannelResultGet(ADCHS_CH0); // AN24
}

inline uint16_t INV_ADC_GetIvChannel(void)
{
    return ADCHS_ChannelResultGet(ADCHS_CH3); // AN26
}

/* ****************************************************************** INV PWM */
inline void INV_PWM_SetDuty(uint16_t DutyU, uint16_t DutyV, uint16_t DutyW)
{
    MCPWM_ChannelPrimaryDutySet(MCPWM_CH_12, DutyU);
    MCPWM_ChannelPrimaryDutySet(MCPWM_CH_5, DutyV);
    MCPWM_ChannelPrimaryDutySet(MCPWM_CH_6, DutyW);
}

inline void INV_PWM_SetCallback(void (*fnc)(void))
{
    MCPWM_CallbackRegister(MCPWM_CH_12, (MCPWM_CH_CALLBACK) fnc, (uintptr_t) NULL);
}

inline void INV_PWM_InterruptEnable(void)
{
    EVIC_SourceEnable(INT_SOURCE_PWM12);
}

inline void INV_PWM_InterruptDisable(void)
{
    EVIC_SourceDisable(INT_SOURCE_PWM12);
}

inline void INV_PWM_InterruptClear(void)
{
    EVIC_SourceStatusClear(INT_SOURCE_PWM12);
}

inline void INV_PWM_Start(void)
{
    MCPWM_Start();
}

inline void INV_PWM_Enable(void)
{
    MCPWM_ChannelPinsOwnershipEnable(MCPWM_CH_12);
    MCPWM_ChannelPinsOwnershipEnable(MCPWM_CH_5);
    MCPWM_ChannelPinsOwnershipEnable(MCPWM_CH_6);
    MCPWM_Start();
}

inline void INV_PWM_U_Enable(void)
{
    MCPWM_ChannelPinsOwnershipEnable(MCPWM_CH_12);
}

inline void INV_PWM_V_Enable(void)
{
    MCPWM_ChannelPinsOwnershipEnable(MCPWM_CH_5);
}

inline void INV_PWM_W_Enable(void)
{
    MCPWM_ChannelPinsOwnershipEnable(MCPWM_CH_6);
}

inline void INV_PWM_Disable(void)
{
    MCPWM_ChannelPinsOwnershipDisable(MCPWM_CH_12);
    MCPWM_ChannelPinsOwnershipDisable(MCPWM_CH_5);
    MCPWM_ChannelPinsOwnershipDisable(MCPWM_CH_6);
    MCPWM_Stop();
}

inline void INV_PWM_U_Disable(void)
{
    MCPWM_ChannelPinsOwnershipDisable(MCPWM_CH_12);
}

inline void INV_PWM_V_Disable(void)
{
    MCPWM_ChannelPinsOwnershipDisable(MCPWM_CH_5);
}

inline void INV_PWM_W_Disable(void)
{
    MCPWM_ChannelPinsOwnershipDisable(MCPWM_CH_6);
}

/* **************************************************************** INV TIMER */
inline void INV_TMR_Start(void)
{
    T1CONSET=_T1CON_ON_MASK;
}

inline void INV_TMR_Restart(void)
{
    T1CONCLR=_T1CON_ON_MASK;
    TMR1=0x0; // clear counter
    T1CONSET=_T1CON_ON_MASK;
}

inline void INV_TMR_Stop(void)
{
    T1CONCLR=_T1CON_ON_MASK;
}

inline uint32_t INV_TMR_GetUs(void)
{
    float tk=TMR1+1;

    return (uint32_t) (0.5+tk/7.5f); // tk/TMR1_FrequencyGet()/1E6
}
