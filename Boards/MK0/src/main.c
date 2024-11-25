#include "definitions.h"
#include "labhau.h"

static void HwInfo(void)
{
    printf("\r\n\r\nLABHAU ACIM INVERTER");
    printf("\r\nHW: HW.ACIM-MK0");
    printf("\r\nPCB: HW.ACIM-MK0");
    printf("\r\nFW: %s-%s", __DATE__, __TIME__);
    printf("\r\nUDID: %08x-%08x-%08x-%08x", DEVSN3, DEVSN2, DEVSN1, DEVSN0);

    printf("\r\nClock: %dHz", CPU_CLOCK_FREQUENCY);
    printf("\r\nReset code: ", RCON);

    if(RCONbits.BCFGERR)
        printf("\r\n-->BCFGERR: An error occurred during a read of the Primary Configuration registers");

    if(RCONbits.BCFGFAIL)
        printf("\r\n-->BCFGFAIL: An error occurred during a read of the Primary and Alternate Configuration registers");

    if(RCONbits.CMR)
        printf("\r\n-->CMR: Configuration Mismatch Reset has occurred");

    if(RCONbits.EXTR)
        printf("\r\n-->EXTR:  Master Clear (pin) Reset has occurred");

    if(RCONbits.SWR)
        printf("\r\n-->SWR: Software Reset was executed");

    if(RCONbits.DMTO)
        printf("\r\n-->DMTO: A DMT time-out has occurred");

    if(RCONbits.WDTO)
        printf("\r\n-->WDTO: Watchdog Timer Time-out Flag bit");

    if(RCONbits.SLEEP)
        printf("\r\n-->SLEEP: Device was in Sleep mode");

    if(RCONbits.IDLE)
        printf("\r\n-->IDLE: Device was in Idle mode");

    if(RCONbits.BOR)
        printf("\r\n-->BOR: Brown-out Reset has occurred");

    if(RCONbits.POR)
        printf("\r\n-->POR: Power-on Reset has occurred");

    RCON&=0xF3FFFD00; // Clear bit 27, 26, 9, 7,6,5,4,3,2,1,0
    printf("\r\n\r\n");
}

int main(void)
{
    int8_t DoNext;
    tick_cxt_t Tick;

    SYS_Initialize(NULL);
    Tick_Init(WDT_Clear);
    HwInfo();
    LABHAU_Init();
    DoNext=0;
    Tick_Reset(&Tick);

    while(true)
    {
        WDT_Clear();

        switch(DoNext)
        {
            case 0:
                if(Tick_Is_Over_Ms(Tick, 3000))
                {
                    Motor_Init(); // Load Motor parameters
                    PFC_Init();
                    MC_Init();
                    DV_Init();
                    DoNext++;
                }
                break;
                
            case 1:
            default:
                DV_Tasks();
                
                if(Tick_Is_Over_Ms(Tick, 500))
                    LedRun_Toggle();
                break;
        }
    }

    return ( EXIT_FAILURE);
}

