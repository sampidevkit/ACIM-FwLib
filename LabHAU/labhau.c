#include "labhau.h"
#include "Common/Tick.h"
#include "Algorithms/MC.h"
#include "Debugger/DataVisualizer.h"

int main(void)
{
    int8_t DoNext;
    tick_cxt_t Tick;

    DoNext=0;
    Tick_Init(ClrWdt);
    Tick_Reset(&Tick);
    /* *************************************************************** System */
    System_Init();
    DevMode_Enable();
    LedErr_Off();
    LedRun_On();
    VDC_Enable();

    while(1)
    {
        switch(DoNext)
        {
            case 0:
                ClrWdt();

                if(Tick_Is_Over_Ms(Tick, 3000))
                {
                    Motor_Init(); // Load Motor parameters
                    MC_Init();
                    DV_Init();
                    DoNext++;
                }
                break;

            case 1:
            default:
                DV_Tasks();

                if(Tick_Is_Over_Ms(Tick, 500))
                {
                    ClrWdt();
                    LedRun_Toggle();
                }
                break;
        }
    }

    return (-1);
}
