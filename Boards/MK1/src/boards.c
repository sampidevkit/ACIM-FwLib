#include "boards.h"
#include "definitions.h"
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
    .Source.Vol.Gain=(float) 236.0, // mV/V
    .Source.Vol.Offset=3 // ADC value
};

/* ******************************************************************* System */
void System_Init(void) // <editor-fold defaultstate="collapsed" desc="System initialize">
{
    SYS_Initialize(NULL);
    printf("\r\n\r\nLABHAU ACIM INVERTER");
    printf("\r\nPCB: HW.ACIM-MK1");
    printf("\r\nMCU: %s", DEVICE_NAME);
    printf("\r\nCore: %s", DEVICE_ARCH);
    printf("\r\nUDID: %08X-%08X-%08X-%08X", DEVSN3, DEVSN2, DEVSN1, DEVSN0);
    printf("\r\nClock: %dMHz", CPU_CLOCK_FREQUENCY/1000000);
    printf("\r\nFW: %s-%s", __DATE__, __TIME__);
    printf("\r\nReset code: 0x%08X", RCON);

    if(RCONbits.BCFGERR)
        printf("\r\n-->BCFGERR: An error occurred during a read of the Primary Configuration registers");

    if(RCONbits.BCFGFAIL)
        printf("\r\n-->BCFGFAIL: An error occurred during a read of the Primary and Alternate Configuration registers");

    if(RCONbits.CMR)
        printf("\r\n-->CMR: Configuration Mismatch Reset has occurred");

    if(RCONbits.EXTR)
        printf("\r\n-->EXTR:  Master Clear (pin) Reset has occurred");

    if(RCONbits.SWR)
        printf("\r\n-->SWR: Software Reset was executed");

    if(RCONbits.DMTO)
        printf("\r\n-->DMTO: A DMT time-out has occurred");

    if(RCONbits.WDTO)
        printf("\r\n-->WDTO: Watchdog Timer Time-out Flag bit");

    if(RCONbits.SLEEP)
        printf("\r\n-->SLEEP: Device was in Sleep mode");

    if(RCONbits.IDLE)
        printf("\r\n-->IDLE: Device was in Idle mode");

    if(RCONbits.BOR)
        printf("\r\n-->BOR: Brown-out Reset has occurred");

    if(RCONbits.POR)
        printf("\r\n-->POR: Power-on Reset has occurred");

    RCON&=0xF3FFFD00; // Clear bit 27, 26, 9, 7,6,5,4,3,2,1,0
    printf("\r\n\r\n");
} // </editor-fold>

inline void ClrWdt(void) // <editor-fold defaultstate="collapsed" desc="Clear Watching-dog-timer">
{
    WDT_Clear();
} // </editor-fold>

/* ************************************************************ Power control */
void VDC_Enable(void)
{
    PWR_RELAY_Set();
    printf("\r\nEnable VDC");
}

void VDC_Disable(void)
{
    PWR_RELAY_Clear();
    printf("\r\nDisable VDC");
}

/* ********************************************************* Dev mode control */
void DevMode_Enable(void)
{
    DEV_RELAY_Clear();
    printf("\r\nEnable DevMode");
}

void DevMode_Disable(void)
{
    //DEV_RELAY_Set();
    printf("\r\nDisable DevMode");
}

/* **************************************************** LED for running state */
inline void LedRun_On(void)
{
    IND_RUN_Set();
}

inline void LedRun_Off(void)
{
    IND_RUN_Clear();
}

inline void LedRun_Toggle(void)
{
    IND_RUN_Toggle();
}

/* ****************************************************** LED for error state */
inline void LedErr_On(void)
{
    IND_ERR_Set();
}

inline void LedErr_Off(void)
{
    IND_ERR_Clear();
}

inline void LedErr_Toggle(void)
{
    IND_ERR_Toggle();
}

/* ******************************************************** LED for MCU state */
inline void LedMcu_On(void)
{
    SLED_Set();
}

inline void LedMcu_Off(void)
{
    SLED_Clear();
}

inline void LedMcu_Toggle(void)
{
    SLED_Toggle();
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
    return (int32_t) QEI1_VelocityGet();
}

/* *************************************************************** INV analog */
inline void INV_ADC_InterruptEnable(void)
{
    IFS3CLR=_IFS3_AD1EOSIF_MASK;
    IEC3SET=_IEC3_AD1EOSIE_MASK;
}

inline void INV_ADC_InterruptDisable(void)
{
    IEC3CLR=_IEC3_AD1EOSIE_MASK;
}

inline bool INV_ADC_ResultIsReady(void)
{
    bool ready=ADCHS_ChannelResultIsReady(ADCHS_CH50);
    ready&=ADCHS_ChannelResultIsReady(ADCHS_CH0);
    ready&=ADCHS_ChannelResultIsReady(ADCHS_CH2);
    ready&=ADCHS_ChannelResultIsReady(ADCHS_CH3);
    ready&=ADCHS_ChannelResultIsReady(ADCHS_CH5);

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
