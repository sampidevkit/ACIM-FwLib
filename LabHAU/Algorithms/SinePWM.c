#include "SinePWM.h"
#include "MC.h"
#include "Debugger/DataVisualizer.h"

typedef struct
{
    const uint16_t dutyTable[42];
    int u;
    int cnt;
    int max;
    bool inited;
} mc_sine_pwm_cxt;

static mc_sine_pwm_cxt SpwmCxt={
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

static mc_process_fnc Callback=NULL;

void SinePWM_Process(void) // <editor-fold defaultstate="collapsed" desc="Sine PWM process">
{
    if(++SpwmCxt.cnt>=SpwmCxt.max)
    {
        int v, w;

        SpwmCxt.cnt=0;
        McOutputs.DutyU=SpwmCxt.dutyTable[SpwmCxt.u];
        v=SpwmCxt.u+14;

        if(v>=42)
            v=v-42;

        McOutputs.DutyV=SpwmCxt.dutyTable[v];

        w=v+14;

        if(w>=42)
            w=w-42;

        McOutputs.DutyW=SpwmCxt.dutyTable[w];

        if(++SpwmCxt.u>=42)
            SpwmCxt.u=0;
        
        if(Callback)
            Callback(NULL);
    }
} // </editor-fold>

void SinePWM_Init(uint32_t freq, void (*FncCb)(void*)) // <editor-fold defaultstate="collapsed" desc="Sine PWM init">
{
    float tmp=freq;

    printf("\r\n%s init: ", __FUNCTION__);
    DevMode_Enable();
    Callback=FncCb;
    tmp=1/tmp; // calculate period (s)
    tmp*=1E6; // period in us
    SpwmCxt.max=(int) (tmp/(50*42)); // calculate interrupt period

    if(SpwmCxt.max<=0)
    {
        freq=47;
        SpwmCxt.max=10;
        printf("frequency is too high, load default ");
    }

    SpwmCxt.u=0;
    SpwmCxt.cnt=0;
    DevMode_Disable();
    printf("F=%dHz, Cnt=%d", freq, SpwmCxt.max);
} // </editor-fold>
