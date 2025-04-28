#include "labhau.h"

int main(void)
{
    System_Init();
    Tick_Init(ClrWdt);
    MC_Init();

    while(1)
    {
        ClrWdt();
        MC_Task();
    }

    return (-1);
}
