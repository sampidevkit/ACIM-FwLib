#include "DataVisualizer.h"
#include "boards.h"

static volatile int head, tail;
static volatile uint8_t DvData[DV_BUFFER_SIZE][DV_NUM_OF_VAR*4+2];

void DV_Init(void)
{
    head=0;
    tail=0;
    printf("\r\n%s done", __FUNCTION__);
}

static void DV_Plot(const int32_t *pD)
{
    uint8_t i, j;

    DvData[head][0]=0x03;

    for(i=0, j=1; i<DV_NUM_OF_VAR; i++)
    {
        DvData[head][j++]=(uint8_t) (pD[i]);
        DvData[head][j++]=(uint8_t) (pD[i]>>8);
        DvData[head][j++]=(uint8_t) (pD[i]>>16);
        DvData[head][j++]=(uint8_t) (pD[i]>>24);
    }

    DvData[head][j]=0xFC;

    if(head<(DV_BUFFER_SIZE-1))
        head++;
    else
        head=0;
}

#if(DV_NUM_OF_VAR==3)
void DV_PlotData(int32_t Input0, int32_t Input1, int32_t Input2)
#elif(DV_NUM_OF_VAR==4)
void DV_PlotData(int32_t Input0, int32_t Input1, int32_t Input2, int32_t Input3)
#elif(DV_NUM_OF_VAR==5)

void DV_PlotData(int32_t Input0, int32_t Input1, int32_t Input2, int32_t Input3, int32_t Input4)
#endif
{
    uint32_t var[5];

#if(DV_NUM_OF_VAR>=3)
    var[0]=Input0;
    var[1]=Input1;
    var[2]=Input2;
#endif

#if(DV_NUM_OF_VAR>=4)
    var[3]=Input3;
#endif

#if(DV_NUM_OF_VAR==5)
    var[4]=Input4;
#endif

    DV_Plot((const int32_t *) var);
}

void DV_Tasks(void)
{
    uint8_t i, sz;
    uint8_t Raw[DV_NUM_OF_VAR*4+2];

    if(head==tail)
        return;

    sz=DV_NUM_OF_VAR*4+2;

    for(i=0; i<sz; i++)
        Raw[i]=DvData[tail][i];

    i=0;

    do
        i+=DV_Write(&Raw[i], sz-i);
    while(i<sz);

    if(tail<(DV_BUFFER_SIZE-1))
        tail++;
    else
        tail=0;
}