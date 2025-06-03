#include "labhau.h"

static int exitRequested=0;

#if defined(__WIN32)
int retval=EXIT_FAILURE;

static void sigintHandler(int signum)
{
    exitRequested=1;
}
#endif

int main(void)
{
    if(System_Init())
    {
        Tick_Init(ClrWdt);
        MC_Init();

        while(exitRequested==0)
        {
            ClrWdt();
            MC_Task();
        }
    }

#if defined(__WIN32)
    signal(SIGINT, SIG_DFL);
#endif
    
    return 0;
}
