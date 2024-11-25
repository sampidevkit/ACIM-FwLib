#include "MC.h"


mc_inputs_t McInputs;
mc_outputs_t McOutputs;
static int InvCnt;

static void InvAdc_IntCb(uint32_t ch, uintptr_t pt)
{
    INV_ADC_InterruptDisable();
    INV_ADC_InterruptClear();
    InvUiCxt.PhaseU.Cur.Val=iir(&InvUiCxt.PhaseU.Cur.Iir, (int16_t) INV_ADC_GetIuChannel(), INV_IIR_FILTER_HARDNESS);
    InvUiCxt.PhaseU.Vol.Val=iir(&InvUiCxt.PhaseU.Vol.Iir, (int16_t) INV_ADC_GetVuChannel(), INV_IIR_FILTER_HARDNESS);
    InvUiCxt.PhaseV.Cur.Val=iir(&InvUiCxt.PhaseV.Cur.Iir, (int16_t) INV_ADC_GetIvChannel(), INV_IIR_FILTER_HARDNESS);
    InvUiCxt.PhaseV.Vol.Val=iir(&InvUiCxt.PhaseV.Vol.Iir, (int16_t) INV_ADC_GetVvChannel(), INV_IIR_FILTER_HARDNESS);
    InvUiCxt.Power.Cur.Val=iir(&InvUiCxt.Power.Cur.Iir, (int16_t) INV_ADC_GetIdcChannel(), INV_IIR_FILTER_HARDNESS);
    InvUiCxt.Power.Vol.Val=iir(&InvUiCxt.Power.Vol.Iir, (int16_t) INV_ADC_GetVdcChannel(), INV_IIR_FILTER_HARDNESS);

    if(++InvCnt>=INV_IIR_FILTER_HARDNESS)
    {
        int32_t tmp;

        InvCnt=0;
        // I=Vsh/Rsh=Vo/gain/Rsh
        // =Vo/(gain*Rsh)
        // =(Adc*Vref/Reso)/(gain*Rsh)
        // =(Adc*Vref)/(Reso*gain*Rsh)
        InvUiCxt.PhaseU.Cur.Val*=InvUiCxt.AdcVref;

        MC_Process();
    }

    INV_ADC_InterruptClear();
    INV_ADC_InterruptEnable();
}

static void InvPwm_IntCb(uint32_t ch, uintptr_t pt)
{
    INV_PWM_Disable();
    VDC_Disable();
    DevMode_Enable();
    LedErr_On();
    printf("\r\nPWM Fault has occurred");
}

void MC_Init(void)
{
    InvCnt=0;

    InvUiCxt.PhaseU.Cur.Iir=0;
    InvUiCxt.PhaseU.Vol.Iir=0;

    InvUiCxt.PhaseV.Cur.Iir=0;
    InvUiCxt.PhaseV.Vol.Iir=0;

    InvUiCxt.Power.Cur.Iir=0;
    InvUiCxt.Power.Vol.Iir=0;

    INV_ADC_InterruptDisable();
    INV_ADC_InterruptClear();
    INV_ADC_SetCallback(InvAdc_IntCb);
    INV_ADC_InterruptEnable();

    INV_PWM_Disable();
    INV_PWM_InterruptDisable();
    INV_PWM_InterruptClear();
    INV_PWM_SetCallback(InvPwm_IntCb);
    INV_PWM_InterruptEnable();
    INV_PWM_Start();

#ifdef USE_MY_MOTOR_CONTROL_ALGORITHM
    MC_myInit();
#endif

    printf("\r\n%s done", __FUNCTION__);
}

void MC_Process(void)
{
#ifdef USE_MY_MOTOR_CONTROL_ALGORITHM
    MC_myProcess();
#else

#endif
}