#include "boards.h"
#include "definitions.h"
#include "Algorithms/MC.h"

inv_ui_cxt_t InvUiCxt={
    .PwmDutyMax=3000, // See in MCPWM_PrimaryPeriodGet()
    .AdcVref=3300, // mV
    .AdcReso=4096, // 12bit

    /* Ish=Vsh/Rsh=(Vo/ampli_gain)/Rsh
          =Vo/(ampli_gain*Rsh)
          =(AdcVal*AdcVref/AdcReso)/(ampli_gain*Rsh)
          =(AdcVal*AdcVref)/(AdcReso*ampli_gain*Rsh)
    Gain=AdcVref/(AdcReso*ampli_gain*Rsh)
    => Ish=AdcVal*Gain-Ioffset
    Ioffset=Voffset/(ampli_gain*Rsh) */

    /* Vin=(Vdiv/Rbot)*(Rtop+Rbot)
       Vdiv=AdcVal*AdcVref/AdcReso
    => Vin=(AdcVal*AdcVref/(AdcReso*Rbot))*(Rtop+Rbot)
    Gain=AdcVref*(Rtop+Rbot)/(AdcReso*Rbot)
    Vin=AdcVal*Gain-Voffset */

    // Rsh=0.001Ohm
    // ampli_gain=200
    // Voffset=2500mV
    // Gain=3300/(4096*200*0.001)=4125/1024
    // Ioffset=2500/(200*0.001)=12500 (mA)
    .PhaseU.Cur.Gain.num=4125,
    .PhaseU.Cur.Gain.den=1024,
    .PhaseU.Cur.Gain.val=(float) (4125/1024),
    .PhaseU.Cur.Offset=12500,

    // Rsh=0.001Ohm
    // ampli_gain=200
    // Voffset=2500mV
    // Gain=3300/(4096*200*0.001)=4125/1024
    // Ioffset=2500/(200*0.001)=12500 (mA)
    .PhaseW.Cur.Gain.num=4125,
    .PhaseW.Cur.Gain.den=1024,
    .PhaseW.Cur.Gain.val=(float) (4125/1024),
    .PhaseW.Cur.Offset=12500,

    // Rsh=0.005Ohm
    // ampli_gain=50
    // Voffset=0mV
    // Gain=3300/(4096*50*0.005)=825/256
    // Ioffset=0 (mA))
    .Source.Cur.Gain.num=825,
    .Source.Cur.Gain.den=256,
    .Source.Cur.Gain.val=(float) (825/256),
    .Source.Cur.Offset=0,
    // Rtop=300k
    // Rbot=1.1k
    // Gain=3300*(300k+1.1k)/(300k*1.1k)=3011/1000
    // Offset=0V
    .Source.Vol.Gain.num=3011,
    .Source.Vol.Gain.num=1000,
    .Source.Vol.Gain.val=(float) (3011/1000),
    .Source.Vol.Offset=0
};

/* ******************************************************************* System */
void System_Init(void)
{
    SYS_Initialize(NULL);
    printf("\r\n\r\nLABHAU ACIM INVERTER");
    printf("\r\nPCB: HW.ACIM-MK1");
    printf("\r\nMCU: %s", DEVICE_NAME);
    printf("\r\nCore: %s", DEVICE_ARCH);
    printf("\r\nUDID: %08X-%08X-%08X-%08X", DEVSN3, DEVSN2, DEVSN1, DEVSN0);
    printf("\r\nClock: %dHz", CPU_CLOCK_FREQUENCY);
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
}

void ClrWdt(void)
{
    WDT_Clear();
}

/* ************************************************************ Power control */
void VDC_Enable(void)
{
    //VDC_ENABLE_Set();
    PWR_RELAY_Set();
}

void VDC_Disable(void)
{
    //VDC_ENABLE_Clear();
    PWR_RELAY_Clear();
}

/* ********************************************************* Dev mode control */
void DevMode_Enable(void)
{
    //DEV_MODE_DISABLE_Clear();
    DEV_RELAY_Clear();
}

void DevMode_Disable(void)
{
    //DEV_MODE_DISABLE_Set();
    DEV_RELAY_Set();
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
    //IND_ERR_N_Clear();
    IND_ERR_Set();
}

void LedErr_Off(void)
{
    //IND_ERR_N_Set();
    IND_ERR_Clear();
}

void LedErr_Toggle(void)
{
    //IND_ERR_N_Toggle();
    IND_ERR_Toggle();
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
uint16_t INV_ADC_GetVdcChannel(void)
{
    return ADCHS_ChannelResultGet(ADCHS_CH10);
}

uint16_t INV_ADC_GetIdcChannel(void)
{
    return ADCHS_ChannelResultGet(ADCHS_CH9);
}

uint16_t INV_ADC_GetIuChannel(void)
{
    return ADCHS_ChannelResultGet(ADCHS_CH0);
}

uint16_t INV_ADC_GetIvChannel(void)
{
    return ADCHS_ChannelResultGet(ADCHS_CH1);
}

uint16_t INV_ADC_GetVuChannel(void)
{
    return ADCHS_ChannelResultGet(ADCHS_CH3);
}

uint16_t INV_ADC_GetVvChannel(void)
{
    return ADCHS_ChannelResultGet(ADCHS_CH7);
}

uint16_t INV_ADC_GetSpeedRef(void)
{
    return 2047; // Not implement
}

void INV_ADC_SetCallback(void (*callback)(uintptr_t context))
{
    //ADCHS_EOSCallbackRegister((ADCHS_EOS_CALLBACK) callback, (uintptr_t) NULL);
    ADCHS_CallbackRegister(ADCHS_CH24, (ADCHS_EOS_CALLBACK) callback, (uintptr_t) NULL);
}

void INV_ADC_InterruptEnable(void)
{
    EVIC_SourceEnable(INT_SOURCE_ADC_EOS);
}

void INV_ADC_InterruptDisable(void)
{
    EVIC_SourceDisable(INT_SOURCE_ADC_EOS);
}

void INV_ADC_InterruptClear(void)
{
    EVIC_SourceStatusClear(INT_SOURCE_ADC_EOS);
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