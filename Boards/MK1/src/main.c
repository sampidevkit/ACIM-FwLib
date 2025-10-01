#include <stdio.h>
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes

static int32_t Vbg, Iu, Idc, Iv, Vdc;
static uint32_t SumVal[5];
static uint32_t SumCnt;
static volatile bool PlotEn;

void ADC_IsrCallback(void)
{
    // Get Vbg
    while(!ADCHS_ChannelResultIsReady(ADCHS_CH50));
    SumVal[0]+=ADCHS_ChannelResultGet(ADCHS_CH50);
    // Get Iu
    while(!ADCHS_ChannelResultIsReady(ADCHS_CH0));
    SumVal[1]+=ADCHS_ChannelResultGet(ADCHS_CH0);
    // Get Idc
    while(!ADCHS_ChannelResultIsReady(ADCHS_CH2));
    SumVal[2]+=ADCHS_ChannelResultGet(ADCHS_CH2);
    // Get Iv
    while(!ADCHS_ChannelResultIsReady(ADCHS_CH3));
    SumVal[3]+=ADCHS_ChannelResultGet(ADCHS_CH3);
    // Get Vdc
    while(!ADCHS_ChannelResultIsReady(ADCHS_CH5));
    SumVal[4]+=ADCHS_ChannelResultGet(ADCHS_CH5);

    if(++SumCnt>=16384)
    {
        Vbg=SumVal[0]>>14;
        Iu=SumVal[1]>>14;
        Idc=SumVal[2]>>14;
        Iv=SumVal[3]>>14;
        Vdc=SumVal[4]>>14;
        SumVal[0]=0;
        SumVal[1]=0;
        SumVal[2]=0;
        SumVal[3]=0;
        SumVal[4]=0;
        SumCnt=0;
        PlotEn=1;
    }
}

int main(void)
{
    SYS_Initialize(NULL);
    printf("\rINIT\n");
    SumVal[0]=0;
    SumVal[1]=0;
    SumVal[2]=0;
    SumVal[3]=0;
    SumVal[4]=0;
    SumCnt=0;
    PlotEn=0;
    ADCHS_EOSCallbackRegister((ADCHS_EOS_CALLBACK) ADC_IsrCallback, (uintptr_t) NULL);
    MCPWM_ChannelPrimaryDutySet(MCPWM_CH_12, 0);
    MCPWM_ChannelPrimaryDutySet(MCPWM_CH_5, 0);
    MCPWM_ChannelPrimaryDutySet(MCPWM_CH_6, 0);
    MCPWM_ChannelPinsOwnershipEnable(MCPWM_CH_12);
    MCPWM_ChannelPinsOwnershipEnable(MCPWM_CH_5);
    MCPWM_ChannelPinsOwnershipEnable(MCPWM_CH_6);
    MCPWM_Start();
    printf("\rRUNNING\n");
    //uint32_t tk=CORETIMER_CounterGet();

    while(true)
    {
        WDT_Clear();
        SYS_Tasks();

        if(PlotEn==1)
            //if((CORETIMER_CounterGet()-tk)>=(4000*500))
        {
            //tk=CORETIMER_CounterGet();
            PlotEn=0;
            SLED_Toggle();
            printf("\r\nVbg=%d", Vbg);
            printf(", Vdc=%d", Vdc);
            printf(", Idc=%d", Idc);
            printf(", Iu=%d", Iu);
            printf(", Iv=%d", Iv);
        }
    }

    return (EXIT_FAILURE);
}
