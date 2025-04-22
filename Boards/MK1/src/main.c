#include <stdio.h>
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes

int main(void)
{
    uint32_t cnt=0;

    SYS_Initialize(NULL);
    printf("\rINIT\n");

    MCPWM_ChannelPrimaryDutySet(MCPWM_CH_12, 0);
    MCPWM_ChannelPrimaryDutySet(MCPWM_CH_5, 0);
    MCPWM_ChannelPrimaryDutySet(MCPWM_CH_6, 0);
    MCPWM_ChannelPinsOwnershipEnable(MCPWM_CH_12);
    MCPWM_ChannelPinsOwnershipEnable(MCPWM_CH_5);
    MCPWM_ChannelPinsOwnershipEnable(MCPWM_CH_6);
    MCPWM_Start();
    printf("\rRUNNING\n");

    while(true)
    {
        WDT_Clear();
        SYS_Tasks();

        if(ADCHS_ChannelResultIsReady(ADCHS_CH0))
        {
            if(++cnt<10000)
            {
                ADCHS_ChannelResultGet(ADCHS_CH0);
                continue;
            }

            cnt=0;
            printf("\nCH24=%d", ADCHS_ChannelResultGet(ADCHS_CH0));

            if(ADCHS_ChannelResultIsReady(ADCHS_CH2))
                printf("\nCH26=%d", ADCHS_ChannelResultGet(ADCHS_CH2));

            if(ADCHS_ChannelResultIsReady(ADCHS_CH3))
                printf("\nCH2=%d", ADCHS_ChannelResultGet(ADCHS_CH3));

            if(ADCHS_ChannelResultIsReady(ADCHS_CH5))
                printf("\nCH6=%d", ADCHS_ChannelResultGet(ADCHS_CH5));
        }
    }

    return (EXIT_FAILURE);
}
