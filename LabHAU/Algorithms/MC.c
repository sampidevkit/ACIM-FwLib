#include "MC.h"
#include "Debugger/DataVisualizer.h"

mc_inputs_t McInputs;
mc_outputs_t McOutputs;
static volatile int McDoNext=(-1);
static void *McProcArg=NULL;
static mc_process_fnc McProcFnc=NULL;

static void InvAdcRun_IntCb(uintptr_t pt) // <editor-fold defaultstate="collapsed" desc="ADC interrupt callback">
{
    INV_ADC_InterruptDisable();
    INV_ADC_InterruptClear();

    if(McProcFnc)
    {
        InvUiCxt.PhaseU.Cur.Val=iir(&InvUiCxt.PhaseU.Cur.Iir, (int16_t) INV_ADC_GetIuChannel(), INV_IIR_FILTER_HARDNESS);
        InvUiCxt.PhaseU.Vol.Val=iir(&InvUiCxt.PhaseU.Vol.Iir, (int16_t) INV_ADC_GetVuChannel(), INV_IIR_FILTER_HARDNESS);
        InvUiCxt.PhaseV.Cur.Val=iir(&InvUiCxt.PhaseV.Cur.Iir, (int16_t) INV_ADC_GetIvChannel(), INV_IIR_FILTER_HARDNESS);
        InvUiCxt.PhaseV.Vol.Val=iir(&InvUiCxt.PhaseV.Vol.Iir, (int16_t) INV_ADC_GetVvChannel(), INV_IIR_FILTER_HARDNESS);
        InvUiCxt.Source.Cur.Val=iir(&InvUiCxt.Source.Cur.Iir, (int16_t) INV_ADC_GetIdcChannel(), INV_IIR_FILTER_HARDNESS);
        InvUiCxt.Source.Vol.Val=iir(&InvUiCxt.Source.Vol.Iir, (int16_t) INV_ADC_GetVdcChannel(), INV_IIR_FILTER_HARDNESS);

        McInputs.Source.I=(int32_t) ((float) InvUiCxt.Source.Cur.Val*InvUiCxt.Source.Cur.Gain.val-(float) InvUiCxt.Source.Cur.Offset); // mA
        McInputs.Source.U=(int32_t) ((float) InvUiCxt.Source.Vol.Val*InvUiCxt.Source.Vol.Gain.val-(float) InvUiCxt.Source.Vol.Offset); // mV
        McInputs.PhaseU.I=(int32_t) ((float) InvUiCxt.PhaseU.Cur.Val*InvUiCxt.PhaseU.Cur.Gain.val-(float) InvUiCxt.PhaseU.Cur.Offset); // mA
        McInputs.PhaseU.U=(int32_t) ((float) InvUiCxt.PhaseU.Vol.Val*InvUiCxt.PhaseU.Vol.Gain.val-(float) InvUiCxt.PhaseU.Vol.Offset); // mV
        McInputs.PhaseV.I=(int32_t) ((float) InvUiCxt.PhaseV.Cur.Val*InvUiCxt.PhaseV.Cur.Gain.val-(float) InvUiCxt.PhaseV.Cur.Offset); // mA
        McInputs.PhaseV.U=(int32_t) ((float) InvUiCxt.PhaseV.Vol.Val*InvUiCxt.PhaseV.Vol.Gain.val-(float) InvUiCxt.PhaseV.Vol.Offset); // mV

        McInputs.PhaseW.I=McInputs.Source.I-McInputs.PhaseU.I-McInputs.PhaseV.I;
        McInputs.PhaseW.U=-McInputs.PhaseU.U-McInputs.PhaseV.U;
        McInputs.Speed=INV_ENC_GetSpeed();

        McProcFnc(McProcArg);

        INV_PWM_SetDuty(McOutputs.DutyU, McOutputs.DutyV, McOutputs.DutyW);
    }

    INV_ADC_InterruptClear();
    INV_ADC_InterruptEnable();
} // </editor-fold>

static void InvAdcCalib_IntCb(uintptr_t pt) // <editor-fold defaultstate="collapsed" desc="ADC interrupt callback">
{
    INV_ADC_InterruptDisable();
    INV_ADC_InterruptClear();

    InvUiCxt.PhaseU.Cur.Val+=iir(&InvUiCxt.PhaseU.Cur.Iir, (int16_t) INV_ADC_GetIuChannel(), INV_IIR_FILTER_HARDNESS);
    InvUiCxt.PhaseU.Vol.Val+=iir(&InvUiCxt.PhaseU.Vol.Iir, (int16_t) INV_ADC_GetVuChannel(), INV_IIR_FILTER_HARDNESS);
    InvUiCxt.PhaseV.Cur.Val+=iir(&InvUiCxt.PhaseV.Cur.Iir, (int16_t) INV_ADC_GetIvChannel(), INV_IIR_FILTER_HARDNESS);
    InvUiCxt.PhaseV.Vol.Val+=iir(&InvUiCxt.PhaseV.Vol.Iir, (int16_t) INV_ADC_GetVvChannel(), INV_IIR_FILTER_HARDNESS);
    InvUiCxt.Source.Cur.Val+=iir(&InvUiCxt.Source.Cur.Iir, (int16_t) INV_ADC_GetIdcChannel(), INV_IIR_FILTER_HARDNESS);
    InvUiCxt.Source.Vol.Val+=iir(&InvUiCxt.Source.Vol.Iir, (int16_t) INV_ADC_GetVdcChannel(), INV_IIR_FILTER_HARDNESS);

    if(McDoNext<2000)
    {
        McDoNext++;

        if(McDoNext==2000)
        {
            InvUiCxt.PhaseU.Cur.Val/=2000;
            InvUiCxt.PhaseU.Vol.Val/=2000;
            InvUiCxt.PhaseV.Cur.Val/=2000;
            InvUiCxt.PhaseV.Vol.Val/=2000;
            InvUiCxt.Source.Cur.Val/=2000;
            InvUiCxt.Source.Vol.Val/=2000;

            InvUiCxt.Source.Cur.Offset=(int32_t) ((float) InvUiCxt.Source.Cur.Val*InvUiCxt.Source.Cur.Gain.val); // mA
            InvUiCxt.Source.Vol.Offset=(int32_t) ((float) InvUiCxt.Source.Vol.Val*InvUiCxt.Source.Vol.Gain.val); // mV
            InvUiCxt.PhaseU.Cur.Offset=(int32_t) ((float) InvUiCxt.PhaseU.Cur.Val*InvUiCxt.PhaseU.Cur.Gain.val); // mA
            InvUiCxt.PhaseU.Vol.Offset=(int32_t) ((float) InvUiCxt.PhaseU.Vol.Val*InvUiCxt.PhaseU.Vol.Gain.val); // mV
            InvUiCxt.PhaseV.Cur.Offset=(int32_t) ((float) InvUiCxt.PhaseV.Cur.Val*InvUiCxt.PhaseV.Cur.Gain.val); // mA
            InvUiCxt.PhaseV.Vol.Offset=(int32_t) ((float) InvUiCxt.PhaseV.Vol.Val*InvUiCxt.PhaseV.Vol.Gain.val); // mV
        }
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

void MC_SetProcessFunction(void (*pFnc)(void *), void *pArg) // <editor-fold defaultstate="collapsed" desc="Set process function">
{
    INV_ADC_InterruptDisable();
    McProcFnc=pFnc;
    McProcArg=pArg;
    INV_ADC_InterruptClear();
    INV_ADC_InterruptEnable();
} // </editor-fold>

bool MC_Init(void) // <editor-fold defaultstate="collapsed" desc="Motor controller init">
{
    if(McDoNext==(-1))
    {
        McDoNext=0;

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

        INV_ADC_InterruptDisable();
        INV_ADC_InterruptClear();
        INV_ADC_SetCallback(InvAdcCalib_IntCb);
        INV_ADC_InterruptEnable();

        INV_PWM_Disable();
        INV_PWM_InterruptDisable();
        INV_PWM_InterruptClear();
        INV_PWM_SetCallback(InvPwm_IntCb);
        INV_PWM_InterruptEnable();
        INV_PWM_SetDuty(0, 0, 0);
        INV_PWM_Enable();

        printf("\r\nMC ADC calibrating...");
    }
    else if(McDoNext==2000)
    {
        printf(" done");
        McDoNext++;

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

        INV_ADC_InterruptDisable();
        INV_ADC_InterruptClear();
        MC_SetProcessFunction(MC_myProcess, NULL);
        MC_myInit();
        INV_ADC_SetCallback(InvAdcRun_IntCb);
        INV_ADC_InterruptEnable();

        VDC_Enable();
        printf("\r\n%s done", __FUNCTION__);
    }
    else if(McDoNext>=2001)
        return 1; // done

    return 0;
} // </editor-fold>

/* ******************************************************** Demo for Sine PWM */
typedef struct
{
    const uint16_t dutyTable[42];
    int u;
    int cnt;
    int max;
} mc_sine_pwm_cxt;

static void MC_SinePWM_Process(void *pArg) // <editor-fold defaultstate="collapsed" desc="Sine PWM process">
{
    mc_sine_pwm_cxt *pCxt=(mc_sine_pwm_cxt*) pArg;

    if(++pCxt->cnt>=pCxt->max)
    {
        int v, w;

        pCxt->cnt=0;
        McOutputs.DutyU=pCxt->dutyTable[pCxt->u];
        v=pCxt->u+14;

        if(v>=42)
            v=v-42;

        McOutputs.DutyV=pCxt->dutyTable[v];

        w=v+14;

        if(w>=42)
            w=w-42;

        McOutputs.DutyW=pCxt->dutyTable[w];

        if(++pCxt->u>=42)
            pCxt->u=0;

        //DV_Plot(McOutputs.DutyU, McOutputs.DutyV, McOutputs.DutyW);
        DV_Plot(McInputs.PhaseU.I, McInputs.PhaseV.I, McInputs.PhaseW.I);
    }
} // </editor-fold>

void MC_SinePWM_Init(uint32_t freq) // <editor-fold defaultstate="collapsed" desc="Sine PWM init">
{
    static mc_sine_pwm_cxt Cxt={
        .dutyTable=
        {
            1499, 1274, 1055, 848, 653, 476,
            323, 197, 101, 35, 2, 5,
            38, 107, 203, 332, 485, 662,
            857, 1067, 1286, 1511, 1736, 1955,
            2162, 2357, 2531, 2681, 2807, 2903,
            2966, 2996, 2993, 2957, 2888, 2786,
            2657, 2504, 2324, 2129, 1919, 1697
        }
    };

    float tmp=freq;

    printf("\r\n%s init: ", __FUNCTION__);
    tmp=1/tmp; // calculate period (s)
    tmp*=1E6; // period in us
    Cxt.max=(int) (tmp/(50*42)); // calculate interrupt period

    if(Cxt.max<=0)
    {
        freq=47;
        Cxt.max=10;
        printf("frequency is too high, load default ");
    }

    Cxt.u=0;
    Cxt.cnt=0;
    MC_SetProcessFunction(MC_SinePWM_Process, (void *) &Cxt);
    DevMode_Disable();
    printf("F=%dHz, Cnt=%d", freq, Cxt.max);
} // </editor-fold>