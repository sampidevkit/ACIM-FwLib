#include "MC.h"
#include "Debugger/DataVisualizer.h"

mc_inputs_t McInputs;
mc_outputs_t McOutputs;
static int McDoNext=(-1);

static void InvPwm_IntCb(uint32_t ch, uintptr_t pt) // <editor-fold defaultstate="collapsed" desc="PWM fault interrupt callback">
{
    INV_PWM_Disable();
    VDC_Disable();
    DevMode_Enable();
    LedErr_On();
    printf("\r\nPWM Fault has occurred");
} // </editor-fold>

void MC_Init(void) // <editor-fold defaultstate="collapsed" desc="Motor controller init">
{
    McDoNext=0;

    InvUiCxt.InterVref.Iir=0;
    InvUiCxt.InterVref.Val=0;

    InvUiCxt.PhaseU.Cur.Iir=0;
    InvUiCxt.PhaseU.Cur.Val=0;
    InvUiCxt.PhaseU.Vol.Iir=0;
    InvUiCxt.PhaseU.Vol.Val=0;

    InvUiCxt.PhaseV.Cur.Iir=0;
    InvUiCxt.PhaseV.Cur.Val=0;
    InvUiCxt.PhaseV.Vol.Iir=0;
    InvUiCxt.PhaseV.Vol.Val=0;

    InvUiCxt.Source.Cur.Iir=0;
    InvUiCxt.Source.Cur.Val=0;
    InvUiCxt.Source.Vol.Iir=0;
    InvUiCxt.Source.Vol.Val=0;

    VDC_Disable();
    DevMode_Enable();

    INV_PWM_Disable();
    INV_PWM_InterruptDisable();
    INV_PWM_InterruptClear();
    INV_PWM_SetCallback(InvPwm_IntCb);
    INV_PWM_InterruptEnable();
    INV_PWM_SetDuty(0, 0, 0);
    INV_PWM_Enable();

    printf("\r\nMC ADC calibrating...");
} // </editor-fold>

void MC_Task(void) // <editor-fold defaultstate="collapsed" desc="Motor controller task">
{
    float tmp;

    if(!INV_ADC_ResultIsReady()||(McDoNext==(-1)))
    {
        LedRun_Off();
        return;
    }

    LedRun_On();

    if(McDoNext<2000) // <editor-fold defaultstate="collapsed" desc="calibration task">
    {
        // Get ADC values, filter and make cumulation
        InvUiCxt.PhaseU.Cur.Val+=iir(&InvUiCxt.PhaseU.Cur.Iir, (int16_t) INV_ADC_GetIuChannel(), INV_IIR_FILTER_HARDNESS);
        InvUiCxt.PhaseU.Vol.Val+=iir(&InvUiCxt.PhaseU.Vol.Iir, (int16_t) INV_ADC_GetVuChannel(), INV_IIR_FILTER_HARDNESS);
        InvUiCxt.PhaseV.Cur.Val+=iir(&InvUiCxt.PhaseV.Cur.Iir, (int16_t) INV_ADC_GetIvChannel(), INV_IIR_FILTER_HARDNESS);
        InvUiCxt.PhaseV.Vol.Val+=iir(&InvUiCxt.PhaseV.Vol.Iir, (int16_t) INV_ADC_GetVvChannel(), INV_IIR_FILTER_HARDNESS);
        InvUiCxt.Source.Cur.Val+=iir(&InvUiCxt.Source.Cur.Iir, (int16_t) INV_ADC_GetIdcChannel(), INV_IIR_FILTER_HARDNESS);
        InvUiCxt.Source.Vol.Val+=iir(&InvUiCxt.Source.Vol.Iir, (int16_t) INV_ADC_GetVdcChannel(), INV_IIR_FILTER_HARDNESS);

        if(++McDoNext==2000)
        {
            // Calculate average ADC value
            InvUiCxt.PhaseU.Cur.Val/=2000;
            InvUiCxt.PhaseU.Vol.Val/=2000;
            InvUiCxt.PhaseV.Cur.Val/=2000;
            InvUiCxt.PhaseV.Vol.Val/=2000;
            InvUiCxt.Source.Cur.Val/=2000;
            InvUiCxt.Source.Vol.Val/=2000;
            InvUiCxt.PhaseW.Cur.Offset=0; // mA
            InvUiCxt.PhaseW.Vol.Offset=0; // mV
            // Clear all inverter variables
            InvUiCxt.PhaseU.Cur.Iir=0;
            InvUiCxt.PhaseU.Cur.Val=0;
            InvUiCxt.PhaseU.Vol.Iir=0;
            InvUiCxt.PhaseU.Vol.Val=0;

            InvUiCxt.PhaseV.Cur.Iir=0;
            InvUiCxt.PhaseV.Cur.Val=0;
            InvUiCxt.PhaseV.Vol.Iir=0;
            InvUiCxt.PhaseV.Vol.Val=0;

            InvUiCxt.PhaseW.Cur.Iir=0;
            InvUiCxt.PhaseW.Cur.Val=0;
            InvUiCxt.PhaseW.Vol.Iir=0;
            InvUiCxt.PhaseW.Vol.Val=0;

            InvUiCxt.Source.Cur.Iir=0;
            InvUiCxt.Source.Cur.Val=0;
            InvUiCxt.Source.Vol.Iir=0;
            InvUiCxt.Source.Vol.Val=0;

            MC_myInit();
            VDC_Enable();

            printf("\r\nCALIBRATED VALUES:");
            printf("\r\n->Vdco=%d mV (Adc=%d)", InvUiCxt.Source.Vol.Offset, InvUiCxt.Source.Vol.Val);
            printf("\r\n->Idco=%d mA (Adc=%d)", InvUiCxt.Source.Cur.Offset, InvUiCxt.Source.Cur.Val);
            printf("\r\n->Iuo=%d mA (Adc=%d)", InvUiCxt.PhaseU.Cur.Offset, InvUiCxt.PhaseU.Cur.Val);
            printf("\r\n->Ivo=%d mA (Adc=%d)", InvUiCxt.PhaseV.Cur.Offset, InvUiCxt.PhaseV.Cur.Val);
            printf("\r\n->Iwo=%d mA (Adc=%d)\r\n", InvUiCxt.PhaseW.Cur.Offset, InvUiCxt.PhaseW.Cur.Val);
        }
    } // </editor-fold>
    else // <editor-fold defaultstate="collapsed" desc="process task">
    {
        // Get ADC values
        InvUiCxt.InterVref.Val=iir(&InvUiCxt.InterVref.Iir, (int16_t) INV_ADC_GetInternalVrefChannel(), INV_IIR_FILTER_HARDNESS);
        InvUiCxt.Source.Vol.Val=iir(&InvUiCxt.Source.Vol.Iir, (int16_t) INV_ADC_GetVdcChannel(), INV_IIR_FILTER_HARDNESS);
        InvUiCxt.Source.Cur.Val=iir(&InvUiCxt.Source.Cur.Iir, (int16_t) INV_ADC_GetIdcChannel(), INV_IIR_FILTER_HARDNESS);
        InvUiCxt.PhaseU.Cur.Val=iir(&InvUiCxt.PhaseU.Cur.Iir, (int16_t) INV_ADC_GetIuChannel(), INV_IIR_FILTER_HARDNESS);
        InvUiCxt.PhaseV.Cur.Val=iir(&InvUiCxt.PhaseV.Cur.Iir, (int16_t) INV_ADC_GetIvChannel(), INV_IIR_FILTER_HARDNESS);
        // Recalculate Vref
        tmp=InvUiCxt.AdcReso;
        tmp*=InvUiCxt.InterVref.Gain;
        tmp/=(float) InvUiCxt.InterVref.Val;
        InvUiCxt.AdcVref=(int32_t) tmp;
        // Calculate inverter voltages & currents
        McInputs.Source.I=(int32_t) ((float) InvUiCxt.Source.Cur.Val*InvUiCxt.Source.Cur.Gain-(float) InvUiCxt.Source.Cur.Offset); // mA
        McInputs.Source.U=(int32_t) ((float) InvUiCxt.Source.Vol.Val*InvUiCxt.Source.Vol.Gain-(float) InvUiCxt.Source.Vol.Offset); // mV
        McInputs.PhaseU.I=(int32_t) ((float) InvUiCxt.PhaseU.Cur.Val*InvUiCxt.PhaseU.Cur.Gain-(float) InvUiCxt.PhaseU.Cur.Offset); // mA
        McInputs.PhaseV.I=(int32_t) ((float) InvUiCxt.PhaseV.Cur.Val*InvUiCxt.PhaseV.Cur.Gain-(float) InvUiCxt.PhaseV.Cur.Offset); // mA
        McInputs.PhaseW.I=McInputs.Source.I-McInputs.PhaseU.I-McInputs.PhaseV.I;
        McInputs.Speed=INV_ENC_GetSpeed();
        // Run controller algorithm
        MC_myProcess();
        // Update PWM outputs
        INV_PWM_SetDuty(McOutputs.DutyU, McOutputs.DutyV, McOutputs.DutyW);
    } // </editor-fold>

    LedRun_Off();
} // </editor-fold>
