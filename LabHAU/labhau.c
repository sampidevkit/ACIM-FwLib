#include "labhau.h"
#include "Common/Tick.h"
#include "Algorithms/MC.h"
#include "Debugger/DataVisualizer.h"
#include "peripheral/uart/plib_uart1.h"

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
    LedRun_Off();
    LedMcu_On();

    while(1)
    {
        ClrWdt();
        
        switch(DoNext)
        {
            case 0:
                if(Tick_Is_Over_Ms(Tick, 3000))
                {
                    DV_Init();
                    Motor_Init(); // Load Motor parameters
                    MC_Init(); // Init Motor controller
                    DoNext++;
                }
                break;

            case 1:
            default:
                DV_Tasks();

                if(Tick_Is_Over_Ms(Tick, 1000))
                {
                    LedMcu_Toggle();
#if(1)
                    printf("\r\nVref=%d mV", InvUiCxt.AdcVref);
                    printf("\r\nVdc=%d mV", McInputs.Source.U);
                    printf("\r\nIdc=%d mA", McInputs.Source.I);
                    printf("\r\nIu=%d mA", McInputs.PhaseU.I);
                    printf("\r\nIv=%d mA\r\n", McInputs.PhaseV.I);
#endif
                }
                break;
        }
        
        MC_Task();
    }

    return (-1);
}
