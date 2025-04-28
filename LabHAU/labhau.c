#include "labhau.h"
#include "Common/Tick.h"
#include "Algorithms/MC.h"
#include "Debugger/DataVisualizer.h"
#include "peripheral/uart/plib_uart1.h"

int main(void)
{
    System_Init();
    Tick_Init(ClrWdt);
    MC_Init();
    DV_Init();

    while(1)
    {
        ClrWdt();
        MC_Task();
    }

    return (-1);
}
