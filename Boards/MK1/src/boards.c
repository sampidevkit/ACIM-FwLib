#include "boards.h"
#include "definitions.h"
#include "Algorithms/MC.h"

inv_ui_cxt_t InvUiCxt={
    .PwmDutyMax=3000, // See in MCPWM_PrimaryPeriodGet()
    .AdcVref=3300, // mV
    .AdcReso=4096, // 12bit
    .InterVref.Gain=(float) 1200, // for Vband gap=1.2V
    .PhaseU.Cur.Gain=(float) 10.0, // 100mV/A
    .PhaseV.Cur.Gain=(float) 10.0, // 100mV/A
    .Source.Cur.Gain=(float) 10.0, // 100mV/A
    .Source.Vol.Gain=(float) 71.677, // mV/V
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

void ClrWdt(void) // <editor-fold defaultstate="collapsed" desc="Clear Watching-dog-timer">
{
    WDT_Clear();
} // </editor-fold>

/* ************************************************************ Power control */
void VDC_Enable(void)
{
    //PWR_RELAY_Set();
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
void LedRun_On(void)
{
    IND_RUN_Set();
}

void LedRun_Off(void)
{
    IND_RUN_Clear();
}

void LedRun_Toggle(void)
{
    IND_RUN_Toggle();
}

/* ****************************************************** LED for error state */
void LedErr_On(void)
{
    IND_ERR_Set();
}

void LedErr_Off(void)
{
    IND_ERR_Clear();
}

void LedErr_Toggle(void)
{
    IND_ERR_Toggle();
}

/* ******************************************************** LED for MCU state */
void LedMcu_On(void)
{
    SLED_Set();
}

void LedMcu_Off(void)
{
    SLED_Clear();
}

void LedMcu_Toggle(void)
{
    SLED_Toggle();
}

/* ******************************************************************* Button */
bool Button_Get(void)
{
    return 1; // no button
}

/* ********************************************************** Data visualizer */
size_t DV_Write(uint8_t* pWrBuffer, const size_t size)
{
    return UART2_Write(pWrBuffer, size);
}

/* ************************************************************** INV encoder */
int32_t INV_ENC_GetSpeed(void)
{
    return (int32_t) QEI1_VelocityGet();
}

/* *************************************************************** INV analog */
bool INV_ADC_ResultIsReady(void)
{
    bool ready=1;

    ready&=ADCHS_ChannelResultIsReady(ADCHS_CH0);
    ready&=ADCHS_ChannelResultIsReady(ADCHS_CH2);
    ready&=ADCHS_ChannelResultIsReady(ADCHS_CH3);
    ready&=ADCHS_ChannelResultIsReady(ADCHS_CH5);

    return ready;
}

uint16_t INV_ADC_GetInternalVrefChannel(void)
{
    // AN50 - Internal Vref=1200mV
    ADCHS_ChannelConversionStart(ADCHS_CH50);
    return ADCHS_ChannelResultGet(ADCHS_CH50);
}

uint16_t INV_ADC_GetVdcChannel(void)
{
    return ADCHS_ChannelResultGet(ADCHS_CH5); // AN6
}

uint16_t INV_ADC_GetIdcChannel(void)
{
    return ADCHS_ChannelResultGet(ADCHS_CH2); // AN2
}

uint16_t INV_ADC_GetIuChannel(void)
{
    return ADCHS_ChannelResultGet(ADCHS_CH0); // AN24
}

uint16_t INV_ADC_GetIvChannel(void)
{
    return ADCHS_ChannelResultGet(ADCHS_CH3); // AN26
}

/* ****************************************************************** INV PWM */
void INV_PWM_SetDuty(uint16_t DutyU, uint16_t DutyV, uint16_t DutyW)
{
    MCPWM_ChannelPrimaryDutySet(MCPWM_CH_12, DutyU);
    MCPWM_ChannelPrimaryDutySet(MCPWM_CH_5, DutyV);
    MCPWM_ChannelPrimaryDutySet(MCPWM_CH_6, DutyW);
}

void INV_PWM_SetCallback(void (*callback) (uint32_t status, uintptr_t context))
{
    MCPWM_CallbackRegister(MCPWM_CH_12, (MCPWM_CH_CALLBACK) callback, (uintptr_t) NULL);
}

void INV_PWM_InterruptEnable(void)
{
    EVIC_SourceEnable(INT_SOURCE_PWM12);
}

void INV_PWM_InterruptDisable(void)
{
    EVIC_SourceDisable(INT_SOURCE_PWM12);
}

void INV_PWM_InterruptClear(void)
{
    EVIC_SourceStatusClear(INT_SOURCE_PWM12);
}

void INV_PWM_Start(void)
{
    MCPWM_Start();
}

void INV_PWM_Enable(void)
{
    MCPWM_ChannelPinsOwnershipEnable(MCPWM_CH_12);
    MCPWM_ChannelPinsOwnershipEnable(MCPWM_CH_5);
    MCPWM_ChannelPinsOwnershipEnable(MCPWM_CH_6);
    MCPWM_Start();
}

void INV_PWM_U_Enable(void)
{
    MCPWM_ChannelPinsOwnershipEnable(MCPWM_CH_12);
}

void INV_PWM_V_Enable(void)
{
    MCPWM_ChannelPinsOwnershipEnable(MCPWM_CH_5);
}

void INV_PWM_W_Enable(void)
{
    MCPWM_ChannelPinsOwnershipEnable(MCPWM_CH_6);
}

void INV_PWM_Disable(void)
{
    MCPWM_ChannelPinsOwnershipDisable(MCPWM_CH_12);
    MCPWM_ChannelPinsOwnershipDisable(MCPWM_CH_5);
    MCPWM_ChannelPinsOwnershipDisable(MCPWM_CH_6);
    MCPWM_Stop();
}

void INV_PWM_U_Disable(void)
{
    MCPWM_ChannelPinsOwnershipDisable(MCPWM_CH_12);
}

void INV_PWM_V_Disable(void)
{
    MCPWM_ChannelPinsOwnershipDisable(MCPWM_CH_5);
}

void INV_PWM_W_Disable(void)
{
    MCPWM_ChannelPinsOwnershipDisable(MCPWM_CH_6);
}