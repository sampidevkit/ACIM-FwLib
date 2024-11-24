#include "boards.h"
#include "motors.h"
#include "Algorithms/PFC.h"
#include "Algorithms/MC.h"

static void HwInfo(void)
{
    printf("\r\n%s done", __FUNCTION__);
}

int main(void)
{
    SYS_Initialize(NULL);
    HwInfo();
    Board_Init();
    Motor_Init();
    PFC_Init();
    MC_Init();

    while(true)
    {
        WDT_Clear();
    }

    return ( EXIT_FAILURE);
}

