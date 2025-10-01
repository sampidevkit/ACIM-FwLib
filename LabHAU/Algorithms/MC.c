#include "MC.h"
#include "Common/Tick.h"
#include "Debugger/DataVisualizer.h"
#include "peripheral/qei/plib_qei1.h"

/* ******************************************************* EXTERNAL VARIABLES */
mc_inputs_t McInputs;
mc_outputs_t McOutputs;
/* ********************************************************** LOCAL VARIABLES */
static tick_cxt_t McTick;
static volatile uint8_t McDoNext=0xFF;

/* ******************************************************** PRIVATE FUNCTIONS */
static void AdcCalib_IntCb(void) // <editor-fold defaultstate="collapsed" desc="ADC calibration interrupt callback">
{
    static uint16_t CalibCnt=0;
    static uint32_t CalibLoop=0;

    INV_ADC_InterruptDisable();
    while(INV_ADC_ResultIsReady()==0);

    if(CalibLoop<100000)
    {
        CalibLoop++;
        // Get ADC, skip all results in the first 5s
        iir(&InvCxt.InterVref.Iir, (int16_t) INV_ADC_GetInternalVrefChannel(), INV_IIR_FILTER_HARDNESS);
        iir(&InvCxt.Source.Vol.Iir, (int16_t) INV_ADC_GetVdcChannel(), INV_IIR_FILTER_HARDNESS);
        iir(&InvCxt.Source.Cur.Iir, (int16_t) INV_ADC_GetIdcChannel(), INV_IIR_FILTER_HARDNESS);
        iir(&InvCxt.PhaseU.Cur.Iir, (int16_t) INV_ADC_GetIuChannel(), INV_IIR_FILTER_HARDNESS);
        iir(&InvCxt.PhaseU.Vol.Iir, (int16_t) INV_ADC_GetUuChannel(), INV_IIR_FILTER_HARDNESS);
        iir(&InvCxt.PhaseV.Cur.Iir, (int16_t) INV_ADC_GetIvChannel(), INV_IIR_FILTER_HARDNESS);
        iir(&InvCxt.PhaseV.Vol.Iir, (int16_t) INV_ADC_GetUvChannel(), INV_IIR_FILTER_HARDNESS);
    }
    else if(CalibCnt<16384) // calibrate in 16384 PWM cycles x50us = 819200us= 819.2ms
    {
        CalibCnt++;
        // Get ADC values, and make cumulation
        InvCxt.InterVref.Val+=iir(&InvCxt.InterVref.Iir, (int16_t) INV_ADC_GetInternalVrefChannel(), INV_IIR_FILTER_HARDNESS);
        InvCxt.Source.Vol.Val+=iir(&InvCxt.Source.Vol.Iir, (int16_t) INV_ADC_GetVdcChannel(), INV_IIR_FILTER_HARDNESS);
        InvCxt.Source.Cur.Val+=iir(&InvCxt.Source.Cur.Iir, (int16_t) INV_ADC_GetIdcChannel(), INV_IIR_FILTER_HARDNESS);
        InvCxt.PhaseU.Cur.Val+=iir(&InvCxt.PhaseU.Cur.Iir, (int16_t) INV_ADC_GetIuChannel(), INV_IIR_FILTER_HARDNESS);
        InvCxt.PhaseU.Vol.Val+=iir(&InvCxt.PhaseU.Vol.Iir, (int16_t) INV_ADC_GetUuChannel(), INV_IIR_FILTER_HARDNESS);
        InvCxt.PhaseV.Cur.Val+=iir(&InvCxt.PhaseV.Cur.Iir, (int16_t) INV_ADC_GetIvChannel(), INV_IIR_FILTER_HARDNESS);
        InvCxt.PhaseV.Vol.Val+=iir(&InvCxt.PhaseV.Vol.Iir, (int16_t) INV_ADC_GetUvChannel(), INV_IIR_FILTER_HARDNESS);

        if(CalibCnt==16384)
        {
            McDoNext=2;
            return; // return without ADC enabling
        }
    }

    INV_ADC_InterruptEnable();
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
    InvCxt.PhaseU.Vol.Val=iir(&InvCxt.PhaseU.Vol.Iir, (int16_t) INV_ADC_GetUuChannel(), INV_IIR_FILTER_HARDNESS);
    InvCxt.PhaseV.Cur.Val=iir(&InvCxt.PhaseV.Cur.Iir, (int16_t) INV_ADC_GetIvChannel(), INV_IIR_FILTER_HARDNESS);
    InvCxt.PhaseV.Vol.Val=iir(&InvCxt.PhaseV.Vol.Iir, (int16_t) INV_ADC_GetUvChannel(), INV_IIR_FILTER_HARDNESS);
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

    tmp=(InvCxt.PhaseU.Vol.Val-InvCxt.PhaseU.Vol.Offset)*InvCxt.AdcVref;
    tmp/=(float) InvCxt.AdcReso;
    McInputs.PhaseU.U=(int32_t) (tmp*InvCxt.PhaseU.Vol.Gain);

    tmp=(InvCxt.PhaseV.Cur.Val-InvCxt.PhaseV.Cur.Offset)*InvCxt.AdcVref;
    tmp/=(float) InvCxt.AdcReso;
    McInputs.PhaseV.I=(int32_t) (tmp*InvCxt.PhaseV.Cur.Gain);

    tmp=(InvCxt.PhaseV.Vol.Val-InvCxt.PhaseV.Vol.Offset)*InvCxt.AdcVref;
    tmp/=(float) InvCxt.AdcReso;
    McInputs.PhaseV.U=(int32_t) (tmp*InvCxt.PhaseV.Vol.Gain);

    McInputs.PhaseW.I=McInputs.Source.I-McInputs.PhaseU.I-McInputs.PhaseV.I;
    McInputs.PhaseW.U=McInputs.Source.U-McInputs.PhaseU.U-McInputs.PhaseV.U;
    McInputs.Speed=InvCxt.Encoder.Val;

    McInputs.dt=INV_TMR_GetUs();
    INV_TMR_Restart();
    // Run controller algorithm
    MC_myProcess();
    // Update PWM outputs
    INV_ADC_InterruptEnable();
    INV_PWM_SetDuty(McOutputs.DutyU, McOutputs.DutyV, McOutputs.DutyW);
} // </editor-fold>

static void InvPwm_IntCb(void) // <editor-fold defaultstate="collapsed" desc="PWM fault interrupt callback">
{
    printf("\r\nPWM Fault has occurred");
    VDC_Disable();
    DevMode_Enable();
    INV_PWM_Disable();
    INV_TMR_Stop();
    McDoNext=5;
    INV_PWM_SetCallback(NULL);
    INV_ADC_SetInterruptCallback(NULL);

} // </editor-fold>

/* ********************************************************* PUBLIC FUNCTIONS */
void MC_Init(void) // <editor-fold defaultstate="collapsed" desc="Motor controller init">
{
    printf("\r\nMotor controller initialize");
    McDoNext=0;
    LedErr_Off();
    LedRun_Off();
    LedMcu_On();
    VDC_Disable();
    DevMode_Enable();
} // </editor-fold>

void MC_Task(void) // <editor-fold defaultstate="collapsed" desc="Motor controller task">
{
    switch(McDoNext)
    {
        case 0:
        {
            printf("\r\nMC ADC calibrating");
            McDoNext=1;
            LedRun_On();
            Tick_Reset(&McTick);

            InvCxt.InterVref.Offset=0;
            InvCxt.PhaseU.Cur.Offset=0;
            InvCxt.PhaseV.Cur.Offset=0;
            InvCxt.Source.Cur.Offset=0;
            //InvCxt.Source.Vol.Offset=0; // set in board.c

            InvCxt.InterVref.Val=0;
            InvCxt.Source.Vol.Val=0;
            InvCxt.Source.Cur.Val=0;
            InvCxt.PhaseU.Cur.Val=0;
            InvCxt.PhaseU.Vol.Val=0;
            InvCxt.PhaseV.Cur.Val=0;
            InvCxt.PhaseV.Vol.Val=0;
            InvCxt.Encoder.Val=0;

            InvCxt.Encoder.Iir=0;
            InvCxt.PhaseU.Cur.Iir=0;
            InvCxt.PhaseU.Vol.Iir=0;
            InvCxt.PhaseV.Cur.Iir=0;
            InvCxt.PhaseV.Vol.Iir=0;
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
            break;
        }

        case 1: // waiting for ADC interrupt processing and set next state
        {
            if(Tick_Is_Over_Ms(McTick, 500))
                printf("... ");
            break;
        }

        case 2:
        {
            McDoNext=3;
            Tick_Reset(&McTick);
            // Calculate average ADC value
            InvCxt.PhaseU.Cur.Offset=InvCxt.PhaseU.Cur.Val/16384;
            InvCxt.PhaseU.Vol.Offset=InvCxt.PhaseU.Vol.Val/16384;
            InvCxt.PhaseV.Cur.Offset=InvCxt.PhaseV.Cur.Val/16384;
            InvCxt.PhaseV.Vol.Offset=InvCxt.PhaseV.Vol.Val/16384;
            InvCxt.Source.Cur.Offset=InvCxt.Source.Cur.Val/16384;

            printf("\r\nOFFSET VALUES:");
            printf("\r\n->Vdco Adc=%d", InvCxt.Source.Vol.Offset);
            printf("\r\n->Idco Adc=%d", InvCxt.Source.Cur.Offset);
            printf("\r\n->Uuo Adc=%d", InvCxt.PhaseU.Vol.Offset);
            printf("\r\n->Iuo Adc=%d", InvCxt.PhaseU.Cur.Offset);
            printf("\r\n->Uvo Adc=%d", InvCxt.PhaseV.Vol.Offset);
            printf("\r\n->Ivo Adc=%d", InvCxt.PhaseV.Cur.Offset);
            break;
        }

        case 3:
        {
            if(Tick_Is_Over_Ms(McTick, 1000))
            {
                printf("\r\nMC processing...");
                McDoNext=4;
                Motor_Init(); // Load Motor parameters
                INV_PWM_Disable();
                MC_myInit();
                INV_TMR_Start();
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

        case 4:
            if(Tick_Is_Over_Ms(McTick, 500))
            {
                static bool pintfEn=0;

                pintfEn^=1;
                LedMcu_Toggle();

                if(pintfEn==1)
                {
                    printf("\r\ndt=%d us", McInputs.dt);
                    printf("\r\nSpeed=%d", McInputs.Speed);
                    printf("\r\nVref=%d mV", InvCxt.AdcVref);
                    printf("\r\nVdc=%d mV, ADC=%d", McInputs.Source.U, InvCxt.Source.Vol.Val);
                    printf("\r\nIdc=%d mA, ADC=%d", McInputs.Source.I, InvCxt.Source.Cur.Val);
                    printf("\r\nIu=%d mA, ADC=%d", McInputs.PhaseU.I, InvCxt.PhaseU.Cur.Val);
                    printf("\r\nUu=%d mV, ADC=%d", McInputs.PhaseU.U, InvCxt.PhaseU.Vol.Val);
                    printf("\r\nIv=%d mA, ADC=%d\r\n", McInputs.PhaseV.I, InvCxt.PhaseV.Cur.Val);
                    printf("\r\nUv=%d mV, ADC=%d\r\n", McInputs.PhaseV.U, InvCxt.PhaseV.Vol.Val);
                }
            }
            break;

        case 5:
        {
            printf("\r\nMotor controller deinitialize");
            McDoNext=6;
            LedErr_On();
            LedRun_Off();
            Tick_Reset(&McTick);
            break;
        }

        case 6:
        default:
        {
            if(Tick_Is_Over_Ms(McTick, 1000))
            {
                LedMcu_Toggle();
                printf("\r\nSpeed=%d", INV_ENC_GetSpeed());
            }
            break;
        }
    }
} // </editor-fold>
