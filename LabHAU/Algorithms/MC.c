#include "MC.h"
#include "Common/Tick.h"
#include "Debugger/DataVisualizer.h"

mc_inputs_t McInputs;
mc_outputs_t McOutputs;
static uint8_t McDoNext=0xFF;
static tick_cxt_t McTick;

static void AdcCalib_IntCb(void) // <editor-fold defaultstate="collapsed" desc="ADC calibration interrupt callback">
{
    static uint16_t CalibCnt=0;

    if(CalibCnt<16384) // calibrate in 16384 PWM cycles x50us = 819200us= 819.2ms
    {
        CalibCnt++;
        McDoNext=2;
        // Get ADC values, and make cumulation
        while(INV_ADC_ResultIsReady()==0);
        InvCxt.InterVref.Val+=iir(&InvCxt.InterVref.Iir, (int16_t) INV_ADC_GetInternalVrefChannel(), INV_IIR_FILTER_HARDNESS);
        InvCxt.Source.Vol.Val+=iir(&InvCxt.Source.Vol.Iir, (int16_t) INV_ADC_GetVdcChannel(), INV_IIR_FILTER_HARDNESS);
        InvCxt.Source.Cur.Val+=iir(&InvCxt.Source.Cur.Iir, (int16_t) INV_ADC_GetIdcChannel(), INV_IIR_FILTER_HARDNESS);
        InvCxt.PhaseU.Cur.Val+=iir(&InvCxt.PhaseU.Cur.Iir, (int16_t) INV_ADC_GetIuChannel(), INV_IIR_FILTER_HARDNESS);
        InvCxt.PhaseV.Cur.Val+=iir(&InvCxt.PhaseV.Cur.Iir, (int16_t) INV_ADC_GetIvChannel(), INV_IIR_FILTER_HARDNESS);
        InvCxt.Encoder.Val+=iir(&InvCxt.Encoder.Iir, (int16_t) INV_ENC_GetSpeed(), INV_IIR_FILTER_HARDNESS);
        //InvCxt.Source.Vol.Offset+=INV_ADC_GetVdcChannel(); // Do not calculate VDC offset
    }
} // </editor-fold>

static void AdcRunning_IntCb(void) // <editor-fold defaultstate="collapsed" desc="ADC running interrupt callback">
{
    float tmp;
    // Get ADC values
    INV_ADC_InterruptDisable();
    InvCxt.InterVref.Val=iir(&InvCxt.InterVref.Iir, (int16_t) INV_ADC_GetInternalVrefChannel(), INV_IIR_FILTER_HARDNESS);
    InvCxt.Source.Vol.Val=iir(&InvCxt.Source.Vol.Iir, (int16_t) INV_ADC_GetVdcChannel(), INV_IIR_FILTER_HARDNESS);
    InvCxt.Source.Cur.Val=iir(&InvCxt.Source.Cur.Iir, (int16_t) INV_ADC_GetIdcChannel(), INV_IIR_FILTER_HARDNESS);
    InvCxt.PhaseU.Cur.Val=iir(&InvCxt.PhaseU.Cur.Iir, (int16_t) INV_ADC_GetIuChannel(), INV_IIR_FILTER_HARDNESS);
    InvCxt.PhaseV.Cur.Val=iir(&InvCxt.PhaseV.Cur.Iir, (int16_t) INV_ADC_GetIvChannel(), INV_IIR_FILTER_HARDNESS);
    InvCxt.Encoder.Val=iir(&InvCxt.Encoder.Iir, (int16_t) INV_ENC_GetSpeed(), INV_IIR_FILTER_HARDNESS);
    // Recalculate Vref
    tmp=InvCxt.AdcReso;
    tmp*=InvCxt.InterVref.Gain;
    tmp/=(float) InvCxt.InterVref.Val;
    InvCxt.AdcVref=(int32_t) tmp;
    // Calculate inverter voltages & currents
    tmp=(InvCxt.Source.Vol.Val-InvCxt.Source.Vol.Offset)*InvCxt.AdcVref;
    tmp/=(float) InvCxt.AdcReso;
    McInputs.Source.U=(int32_t) (tmp*InvCxt.Source.Vol.Gain);

    tmp=(InvCxt.Source.Cur.Val-InvCxt.Source.Cur.Offset)*InvCxt.AdcVref;
    tmp/=(float) InvCxt.AdcReso;
    McInputs.Source.I=(int32_t) (tmp*InvCxt.Source.Cur.Gain);

    tmp=(InvCxt.PhaseU.Cur.Val-InvCxt.PhaseU.Cur.Offset)*InvCxt.AdcVref;
    tmp/=(float) InvCxt.AdcReso;
    McInputs.PhaseU.I=(int32_t) (tmp*InvCxt.PhaseU.Cur.Gain);

    tmp=(InvCxt.PhaseV.Cur.Val-InvCxt.PhaseV.Cur.Offset)*InvCxt.AdcVref;
    tmp/=(float) InvCxt.AdcReso;
    McInputs.PhaseV.I=(int32_t) (tmp*InvCxt.PhaseV.Cur.Gain);

    McInputs.PhaseW.I=McInputs.Source.I-labs(McInputs.PhaseU.I)-labs(McInputs.PhaseV.I);
    McInputs.Speed=InvCxt.Encoder.Val;
    // Run controller algorithm
    MC_myProcess();
    // Update PWM outputs
    INV_ADC_InterruptEnable();
    INV_PWM_SetDuty(McOutputs.DutyU, McOutputs.DutyV, McOutputs.DutyW);
} // </editor-fold>

static void InvPwm_IntCb(void) // <editor-fold defaultstate="collapsed" desc="PWM fault interrupt callback">
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
    Tick_Reset(&McTick);
    printf("\r\nMotor controller initialize");
} // </editor-fold>

void MC_Task(void) // <editor-fold defaultstate="collapsed" desc="Motor controller task">
{
    switch(McDoNext)
    {
        case 0:
        {
            McDoNext=1;
            LedRun_On();
            VDC_Disable();
            DevMode_Enable();

            InvCxt.InterVref.Offset=0;
            InvCxt.PhaseU.Cur.Offset=0;
            InvCxt.PhaseV.Cur.Offset=0;
            InvCxt.Source.Cur.Offset=0;
            //InvCxt.Source.Vol.Offset=0; // set in board.c

            InvCxt.InterVref.Val=0;
            InvCxt.Source.Vol.Val=0;
            InvCxt.Source.Cur.Val=0;
            InvCxt.PhaseU.Cur.Val=0;
            InvCxt.PhaseV.Cur.Val=0;
            InvCxt.Encoder.Val=0;

            InvCxt.Encoder.Iir=0;
            InvCxt.PhaseU.Cur.Iir=0;
            InvCxt.PhaseV.Cur.Iir=0;
            InvCxt.Source.Cur.Iir=0;
            InvCxt.Source.Vol.Iir=0;
            InvCxt.InterVref.Iir=0;

            INV_ADC_InterruptDisable();
            INV_ADC_SetInterruptCallback(AdcCalib_IntCb);
            INV_ADC_InterruptEnable();

            INV_PWM_Disable();
            INV_PWM_InterruptDisable();
            INV_PWM_InterruptClear();
            INV_PWM_SetCallback(InvPwm_IntCb);
            INV_PWM_InterruptEnable();
            INV_PWM_SetDuty(0, 0, 0);
            INV_PWM_Enable();

            printf("\r\nMC ADC calibrating...");
            break;
        }

        case 2:
        {
            McDoNext=3;
            Tick_Reset(&McTick);
            // Calculate average ADC value
            InvCxt.PhaseU.Cur.Offset=InvCxt.PhaseU.Cur.Val/16384;
            InvCxt.PhaseV.Cur.Offset=InvCxt.PhaseV.Cur.Val/16384;
            InvCxt.Source.Cur.Offset=InvCxt.Source.Cur.Val/16384;

            printf("\r\nOFFSET VALUES:");
            printf("\r\n->Vdco Adc=%d", InvCxt.Source.Vol.Offset);
            printf("\r\n->Idco Adc=%d", InvCxt.Source.Cur.Offset);
            printf("\r\n->Iuo Adc=%d", InvCxt.PhaseU.Cur.Offset);
            printf("\r\n->Ivo Adc=%d", InvCxt.PhaseV.Cur.Offset);
            break;
        }

        case 3:
        {
            if(Tick_Is_Over_Ms(McTick, 1000))
            {
                McDoNext=4;
                printf("\r\nMC processing...");
                INV_PWM_Disable();
                MC_myInit();
                INV_ADC_InterruptDisable();
                INV_ADC_SetInterruptCallback(AdcRunning_IntCb);
                INV_ADC_InterruptEnable();
                INV_PWM_InterruptClear();
                INV_PWM_Enable();
                VDC_Enable();
                LedRun_Off();
            }
            break;
        }

        default:
            break;
    }
} // </editor-fold>
