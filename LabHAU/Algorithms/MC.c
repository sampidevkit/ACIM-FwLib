#include "MC.h"

inv_ui_cxt_t InvUiCxt;
static volatile int InvCnt;

static void InvAdc_IntCb(uint32_t ch, uintptr_t pt)
{
    EVIC_SourceDisable(INT_SOURCE_ADC_DATA0);
    EVIC_SourceStatusClear(INT_SOURCE_ADC_DATA0);
    InvUiCxt.PhaseU.Cur.Val=iir(&InvUiCxt.PhaseU.Cur.Iir, (uint16_t) ADCHS_ChannelResultGet(ADCHS_CH0), INV_IIR_FILTER_HARDNESS);
    InvUiCxt.PhaseU.Vol.Val=iir(&InvUiCxt.PhaseU.Vol.Iir, (uint16_t) ADCHS_ChannelResultGet(ADCHS_CH2), INV_IIR_FILTER_HARDNESS);
    InvUiCxt.PhaseV.Cur.Val=iir(&InvUiCxt.PhaseV.Cur.Iir, (uint16_t) ADCHS_ChannelResultGet(ADCHS_CH1), INV_IIR_FILTER_HARDNESS);
    InvUiCxt.PhaseV.Vol.Val=iir(&InvUiCxt.PhaseV.Vol.Iir, (uint16_t) ADCHS_ChannelResultGet(ADCHS_CH7), INV_IIR_FILTER_HARDNESS);
    InvUiCxt.Power.Cur.Val=iir(&InvUiCxt.Power.Cur.Iir, (uint16_t) ADCHS_ChannelResultGet(ADCHS_CH9), INV_IIR_FILTER_HARDNESS);
    InvUiCxt.Power.Vol.Val=iir(&InvUiCxt.Power.Vol.Iir, (uint16_t) ADCHS_ChannelResultGet(ADCHS_CH10), INV_IIR_FILTER_HARDNESS);

    if(++PfcCnt>=INV_IIR_FILTER_HARDNESS)
    {
        float tmp;

        PfcCnt=0;
        tmp=InvUiCxt.PhaseU.Cur.Val;

        MC_Process();
    }

    EVIC_SourceStatusClear(INT_SOURCE_ADC_DATA0);
    EVIC_SourceEnable(INT_SOURCE_ADC_DATA0);
}

static void InvPwm_IntCb(uint32_t ch, uintptr_t pt)
{
    MCPWM_Stop();
    VDC_Disable();
    DevMode_Enable();
    LedErr_On();
    printf("\r\nPWM Fault has occurred");
}

#ifndef USE_MY_MOTOR_CONTROL_ALGORITHM
mc_inputs_t McInputs;
mc_outputs_t McOutputs;

void MC_Init(void)
{

}

void MC_Process(void)
{

}
#else
#warning "You are using customize motor control algotithm"
#endif
