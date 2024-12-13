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
    VDC_Disable();
    DevMode_Enable();
    LedErr_Off();
    LedRun_On();

    while(1)
    {
        switch(DoNext)
        {
            case 0:
                ClrWdt();

                if(Tick_Is_Over_Ms(Tick, 3000))
                {
                    DV_Init();
                    Motor_Init(); // Load Motor parameters
                    DoNext++;
                }
                break;

            case 1:
                ClrWdt();

                if(MC_Init())
                    DoNext++;
                break;

            default:
                DV_Tasks();

                if(Tick_Is_Over_Ms(Tick, 500))
                {
                    ClrWdt();
                    LedRun_Toggle();
#if(1)
                    printf("\r\nVDC=%d", McInputs.Source.U);
                    printf("\r\nIDC=%d", McInputs.Source.I);
                    printf("\r\nIU=%d", McInputs.PhaseU.I);
                    printf("\r\nIV=%d", McInputs.PhaseV.I);
                    printf("\r\nVU=%d", McInputs.PhaseU.U);
                    printf("\r\nVV=%d\r\n", McInputs.PhaseV.U);
#endif
                }
                break;
        }
    }

    return (-1);
}
