#include "MC.h"
#include "Debugger/DataVisualizer.h"

mc_inputs_t McInputs;
mc_outputs_t McOutputs;
static int McCount;

#ifdef USE_MC_FREE_RUN
static const float SinTable42[42]={
    0.500, 0.425, 0.352, 0.283, 0.218, 0.159,
    0.108, 0.066, 0.034, 0.012, 0.001, 0.002,
    0.013, 0.036, 0.068, 0.111, 0.162, 0.221,
    0.286, 0.356, 0.429, 0.504, 0.579, 0.652,
    0.721, 0.786, 0.844, 0.894, 0.936, 0.968,
    0.989, 0.999, 0.998, 0.986, 0.963, 0.929,
    0.886, 0.835, 0.775, 0.710, 0.640, 0.566
};

static uint16_t DutyTable42[42];
#endif

static void InvAdc_IntCb(uint32_t ch, uintptr_t pt) // <editor-fold defaultstate="collapsed" desc="ADC interrupt callback">
{
    INV_ADC_InterruptDisable();
    INV_ADC_InterruptClear();
    InvUiCxt.PhaseU.Cur.Val=iir(&InvUiCxt.PhaseU.Cur.Iir, (int16_t) INV_ADC_GetIuChannel(), INV_IIR_FILTER_HARDNESS);
    InvUiCxt.PhaseU.Vol.Val=iir(&InvUiCxt.PhaseU.Vol.Iir, (int16_t) INV_ADC_GetVuChannel(), INV_IIR_FILTER_HARDNESS);
    InvUiCxt.PhaseV.Cur.Val=iir(&InvUiCxt.PhaseV.Cur.Iir, (int16_t) INV_ADC_GetIvChannel(), INV_IIR_FILTER_HARDNESS);
    InvUiCxt.PhaseV.Vol.Val=iir(&InvUiCxt.PhaseV.Vol.Iir, (int16_t) INV_ADC_GetVvChannel(), INV_IIR_FILTER_HARDNESS);
    InvUiCxt.Source.Cur.Val=iir(&InvUiCxt.Source.Cur.Iir, (int16_t) INV_ADC_GetIdcChannel(), INV_IIR_FILTER_HARDNESS);
    InvUiCxt.Source.Vol.Val=iir(&InvUiCxt.Source.Vol.Iir, (int16_t) INV_ADC_GetVdcChannel(), INV_IIR_FILTER_HARDNESS);

    if(++McCount>=INV_IIR_FILTER_HARDNESS)
    {
#if(1) // Use FPU
        McInputs.Source.I=(int32_t) ((float) InvUiCxt.Source.Cur.Val*InvUiCxt.Source.Cur.Gain.val); // mA
        McInputs.Source.U=(int32_t) ((float) InvUiCxt.Source.Vol.Val*InvUiCxt.Source.Vol.Gain.val); // mV
        McInputs.PhaseU.I=(int32_t) ((float) InvUiCxt.PhaseU.Cur.Val*InvUiCxt.PhaseU.Cur.Gain.val); // mA
        McInputs.PhaseU.U=(int32_t) ((float) InvUiCxt.PhaseU.Vol.Val*InvUiCxt.PhaseU.Vol.Gain.val); // mV
        McInputs.PhaseV.I=(int32_t) ((float) InvUiCxt.PhaseV.Cur.Val*InvUiCxt.PhaseV.Cur.Gain.val); // mA
        McInputs.PhaseV.U=(int32_t) ((float) InvUiCxt.PhaseV.Vol.Val*InvUiCxt.PhaseV.Vol.Gain.val); // mV
#else
        McInputs.Source.I=(InvUiCxt.Source.Cur.Val*InvUiCxt.Source.Cur.Gain.num)/InvUiCxt.Source.Cur.Gain.den; // mA
        McInputs.Source.U=(InvUiCxt.Source.Vol.Val*InvUiCxt.Source.Vol.Gain.num)/InvUiCxt.Source.Vol.Gain.den; // mV
        McInputs.PhaseU.I=(InvUiCxt.PhaseU.Cur.Val*InvUiCxt.PhaseU.Cur.Gain.num)/InvUiCxt.PhaseU.Cur.Gain.den; // mA
        McInputs.PhaseU.U=(InvUiCxt.PhaseU.Vol.Val*InvUiCxt.PhaseU.Vol.Gain.num)/InvUiCxt.PhaseU.Vol.Gain.den; // mV
        McInputs.PhaseV.I=(InvUiCxt.PhaseV.Cur.Val*InvUiCxt.PhaseV.Cur.Gain.num)/InvUiCxt.PhaseV.Cur.Gain.den; // mA
        McInputs.PhaseV.U=(InvUiCxt.PhaseV.Vol.Val*InvUiCxt.PhaseV.Vol.Gain.num)/InvUiCxt.PhaseV.Vol.Gain.den; // mV
#endif
        McInputs.PhaseW.I=-McInputs.PhaseU.I-McInputs.PhaseV.I;
        McInputs.PhaseW.U=-McInputs.PhaseU.U-McInputs.PhaseV.U;
        McInputs.Speed=INV_ENC_GetSpeed();
        MC_Process();
        McCount=0;
    }

    INV_PWM_SetDuty(McOutputs.DutyU, McOutputs.DutyV, McOutputs.DutyW);
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

#ifdef USE_MY_MOTOR_CONTROL_ALGORITHM
    MC_myInit();
#elif defined(USE_MC_FREE_RUN)
    int i;

    for(i=0; i<42; i++)
    {
        float tmp=SinTable42[i]*(float) InvUiCxt.PwmDutyMax;

        DutyTable42[i]=(uint16_t) tmp;
    }
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
    static int u=0;
    int v, w;
    int sz=42, sz13=14; // 42/3

    McOutputs.DutyU=DutyTable42[u];
    v=u+sz13;

    if(v>=sz)
        v=v-sz;

    McOutputs.DutyV=DutyTable42[v];

    w=v+sz13;

    if(w>=sz)
        w=w-sz;

    McOutputs.DutyW=DutyTable42[w];

    if(++u>=sz)
        u=0;
    
    //DV_Plot(McInputs.PhaseU.I, McInputs.PhaseV.I, McInputs.PhaseW.I);
    DV_Plot(McOutputs.DutyU, McOutputs.DutyV, McOutputs.DutyW);
#endif
#endif
} // </editor-fold>