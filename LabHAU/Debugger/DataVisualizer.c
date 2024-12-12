#include "DataVisualizer.h"
#include "boards.h"

static volatile int head, tail;
static volatile uint8_t DvData[DV_BUFFER_SIZE][14];

void DV_Init(void)
{
    head=0;
    tail=0;
    printf("\r\n%s done", __FUNCTION__);
}

void DV_PlotData(int32_t Input0, int32_t Input1, int32_t Input2)
{
    uint32_t *pD0=(uint32_t *) &Input0;
    uint32_t *pD1=(uint32_t *) &Input1;
    uint32_t *pD2=(uint32_t *) &Input2;

    DvData[head][0]=0x03;

    DvData[head][1]=(uint8_t) (*pD0);
    DvData[head][2]=(uint8_t) (*pD0>>8);
    DvData[head][3]=(uint8_t) (*pD0>>16);
    DvData[head][4]=(uint8_t) (*pD0>>24);

    DvData[head][5]=(uint8_t) (*pD1);
    DvData[head][6]=(uint8_t) (*pD1>>8);
    DvData[head][7]=(uint8_t) (*pD1>>16);
    DvData[head][8]=(uint8_t) (*pD1>>24);

    DvData[head][9]=(uint8_t) (*pD2);
    DvData[head][10]=(uint8_t) (*pD2>>8);
    DvData[head][11]=(uint8_t) (*pD2>>16);
    DvData[head][12]=(uint8_t) (*pD2>>24);

    DvData[head][13]=0xFC;

    if(head<(DV_BUFFER_SIZE-1))
        head++;
    else
        head=0;
}

void DV_Tasks(void)
{
    size_t sz;
    uint8_t Raw[14]; //={0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC};

    if(head==tail)
        return;

    for(sz=0; sz<14; sz++)
        Raw[sz]=DvData[tail][sz];

    sz=0;

    do
        sz+=DV_Write(&Raw[sz], 14);
    while(sz<14);

    if(tail<(DV_BUFFER_SIZE-1))
        tail++;
    else
        tail=0;

    //    if(head==tail)
    //        printf("\r\nEmpty");
}