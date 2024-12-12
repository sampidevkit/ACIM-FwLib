#include "MC.h"
#include "Debugger/DataVisualizer.h"

mc_inputs_t McInputs;
mc_outputs_t McOutputs;
static volatile int McCount;

#ifdef USE_MC_FREE_RUN
static const uint16_t dutyTable[42]={
    1499, 1274, 1055, 848, 653, 476, 323, 197,
    101, 35, 2, 5, 38, 107, 203, 332,
    485, 662, 857, 1067, 1286, 1511, 1736, 1955,
    2162, 2357, 2531, 2681, 2807, 2903, 2966, 2996,
    2993, 2957, 2888, 2786, 2657, 2504, 2324, 2129,
    1919, 1697
};

static int u=0;
static int cnt=0;
#endif

static void InvAdc_IntCb(uintptr_t pt) // <editor-fold defaultstate="collapsed" desc="ADC interrupt callback">
{
    INV_ADC_InterruptDisable();
    INV_ADC_InterruptClear();

    if(++McCount>=INV_IIR_FILTER_HARDNESS)
    {
        McCount=0;
        InvUiCxt.PhaseU.Cur.Val=iir(&InvUiCxt.PhaseU.Cur.Iir, (int16_t) INV_ADC_GetIuChannel(), INV_IIR_FILTER_HARDNESS);
        InvUiCxt.PhaseU.Vol.Val=iir(&InvUiCxt.PhaseU.Vol.Iir, (int16_t) INV_ADC_GetVuChannel(), INV_IIR_FILTER_HARDNESS);
        InvUiCxt.PhaseV.Cur.Val=iir(&InvUiCxt.PhaseV.Cur.Iir, (int16_t) INV_ADC_GetIvChannel(), INV_IIR_FILTER_HARDNESS);
        InvUiCxt.PhaseV.Vol.Val=iir(&InvUiCxt.PhaseV.Vol.Iir, (int16_t) INV_ADC_GetVvChannel(), INV_IIR_FILTER_HARDNESS);
        InvUiCxt.Source.Cur.Val=iir(&InvUiCxt.Source.Cur.Iir, (int16_t) INV_ADC_GetIdcChannel(), INV_IIR_FILTER_HARDNESS);
        InvUiCxt.Source.Vol.Val=iir(&InvUiCxt.Source.Vol.Iir, (int16_t) INV_ADC_GetVdcChannel(), INV_IIR_FILTER_HARDNESS);

#if(1) // Use FPU
        McInputs.Source.I=(int32_t) ((float) InvUiCxt.Source.Cur.Val*InvUiCxt.Source.Cur.Gain.val-(float) InvUiCxt.Source.Cur.Offset); // mA
        McInputs.Source.U=(int32_t) ((float) InvUiCxt.Source.Vol.Val*InvUiCxt.Source.Vol.Gain.val-(float) InvUiCxt.Source.Vol.Offset); // mV
        McInputs.PhaseU.I=(int32_t) ((float) InvUiCxt.PhaseU.Cur.Val*InvUiCxt.PhaseU.Cur.Gain.val-(float) InvUiCxt.PhaseU.Cur.Offset); // mA
        McInputs.PhaseU.U=(int32_t) ((float) InvUiCxt.PhaseU.Vol.Val*InvUiCxt.PhaseU.Vol.Gain.val-(float) InvUiCxt.PhaseU.Vol.Offset); // mV
        McInputs.PhaseV.I=(int32_t) ((float) InvUiCxt.PhaseV.Cur.Val*InvUiCxt.PhaseV.Cur.Gain.val-(float) InvUiCxt.PhaseV.Cur.Offset); // mA
        McInputs.PhaseV.U=(int32_t) ((float) InvUiCxt.PhaseV.Vol.Val*InvUiCxt.PhaseV.Vol.Gain.val-(float) InvUiCxt.PhaseV.Vol.Offset); // mV

        //        McInputs.Source.I=(int32_t) InvUiCxt.Source.Cur.Val;
        //        McInputs.Source.U=(int32_t) InvUiCxt.Source.Vol.Val;
        //        McInputs.PhaseU.I=(int32_t) InvUiCxt.PhaseU.Cur.Val;
        //        McInputs.PhaseU.U=(int32_t) InvUiCxt.PhaseU.Vol.Val;
        //        McInputs.PhaseV.I=(int32_t) InvUiCxt.PhaseV.Cur.Val;
        //        McInputs.PhaseV.U=(int32_t) InvUiCxt.PhaseV.Vol.Val;
#else
        McInputs.Source.I=(InvUiCxt.Source.Cur.Val*InvUiCxt.Source.Cur.Gain.num)/InvUiCxt.Source.Cur.Gain.den-InvUiCxt.Source.Cur.Offset; // mA
        McInputs.Source.U=(InvUiCxt.Source.Vol.Val*InvUiCxt.Source.Vol.Gain.num)/InvUiCxt.Source.Vol.Gain.den-InvUiCxt.Source.Vol.Offset; // mV
        McInputs.PhaseU.I=(InvUiCxt.PhaseU.Cur.Val*InvUiCxt.PhaseU.Cur.Gain.num)/InvUiCxt.PhaseU.Cur.Gain.den-InvUiCxt.PhaseU.Cur.Offset; // mA
        McInputs.PhaseU.U=(InvUiCxt.PhaseU.Vol.Val*InvUiCxt.PhaseU.Vol.Gain.num)/InvUiCxt.PhaseU.Vol.Gain.den-InvUiCxt.PhaseU.Vol.Offset; // mV
        McInputs.PhaseV.I=(InvUiCxt.PhaseV.Cur.Val*InvUiCxt.PhaseV.Cur.Gain.num)/InvUiCxt.PhaseV.Cur.Gain.den-InvUiCxt.PhaseV.Cur.Offset; // mA
        McInputs.PhaseV.U=(InvUiCxt.PhaseV.Vol.Val*InvUiCxt.PhaseV.Vol.Gain.num)/InvUiCxt.PhaseV.Vol.Gain.den-InvUiCxt.PhaseV.Vol.Offset; // mV
#endif
        McInputs.PhaseW.I=McInputs.Source.I-McInputs.PhaseU.I-McInputs.PhaseV.I;
        McInputs.PhaseW.U=-McInputs.PhaseU.U-McInputs.PhaseV.U;
        McInputs.Speed=INV_ENC_GetSpeed();
        MC_Process();
        INV_PWM_SetDuty(McOutputs.DutyU, McOutputs.DutyV, McOutputs.DutyW);
    }
    else
    {
        iir(&InvUiCxt.PhaseU.Cur.Iir, (int16_t) INV_ADC_GetIuChannel(), INV_IIR_FILTER_HARDNESS);
        iir(&InvUiCxt.PhaseU.Vol.Iir, (int16_t) INV_ADC_GetVuChannel(), INV_IIR_FILTER_HARDNESS);
        iir(&InvUiCxt.PhaseV.Cur.Iir, (int16_t) INV_ADC_GetIvChannel(), INV_IIR_FILTER_HARDNESS);
        iir(&InvUiCxt.PhaseV.Vol.Iir, (int16_t) INV_ADC_GetVvChannel(), INV_IIR_FILTER_HARDNESS);
        iir(&InvUiCxt.Source.Cur.Iir, (int16_t) INV_ADC_GetIdcChannel(), INV_IIR_FILTER_HARDNESS);
        iir(&InvUiCxt.Source.Vol.Iir, (int16_t) INV_ADC_GetVdcChannel(), INV_IIR_FILTER_HARDNESS);
    }

    INV_ADC_InterruptClear();
    INV_ADC_InterruptEnable();
} // </editor-fold>

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
    McCount=0;

    InvUiCxt.PhaseU.Cur.Iir=0;
    InvUiCxt.PhaseU.Vol.Iir=0;

    InvUiCxt.PhaseV.Cur.Iir=0;
    InvUiCxt.PhaseV.Vol.Iir=0;

    InvUiCxt.Source.Cur.Iir=0;
    InvUiCxt.Source.Vol.Iir=0;

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
    INV_PWM_U_Enable();
    INV_PWM_V_Enable();
    INV_PWM_W_Enable();

#ifdef USE_MY_MOTOR_CONTROL_ALGORITHM
    MC_myInit();
#elif defined(USE_MC_FREE_RUN)
    u=0;
    cnt=0;
    DevMode_Disable();
#else
#endif

    printf("\r\n%s done", __FUNCTION__);
} // </editor-fold>

void MC_Process(void) // <editor-fold defaultstate="collapsed" desc="Motor controller process">
{
#ifdef USE_MY_MOTOR_CONTROL_ALGORITHM
#warning "You are using MC_myProcess()"
    MC_myProcess();
#else
#warning "Your MC_myProcess() has NOT been called in this option"
#if defined(USE_MC_FREE_RUN)
    if(++cnt>=(19/INV_IIR_FILTER_HARDNESS)) // 20000us/50us/42=19
    {
        int v, w;
        int sz=42, sz13=14;

        cnt=0;
        McOutputs.DutyU=dutyTable[u];
        v=u+sz13;

        if(v>=sz)
            v=v-sz;

        McOutputs.DutyV=dutyTable[v];

        w=v+sz13;

        if(w>=sz)
            w=w-sz;

        McOutputs.DutyW=dutyTable[w];

        if(++u>=sz)
            u=0;

        //DV_Plot(McOutputs.DutyU, McOutputs.DutyV, McOutputs.DutyW);
        //DV_Plot(McInputs.PhaseU.I, McInputs.PhaseV.I, McInputs.PhaseW.I);
    }
#endif
#endif
} // </editor-fold>