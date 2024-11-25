#include "DataVisualizer.h"
#include "boards.h"

static volatile plot_ring_buff_t DvBuff;
static plot_cxt_t DvData[DV_BUFFER_SIZE];

void DV_Init(void)
{
    DvBuff.head=0;
    DvBuff.tail=0;
    printf("\r\n%s done", __FUNCTION__);
}

void DV_Plot(int32_t Input0, int32_t Input1, int32_t Input2)
{
    DvData[DvBuff.tail].Raw[0]=0x03;

    DvData[DvBuff.tail].Raw[1]=(uint8_t) (Input0>>24);
    DvData[DvBuff.tail].Raw[2]=(uint8_t) (Input0>>16);
    DvData[DvBuff.tail].Raw[3]=(uint8_t) (Input0>>8);
    DvData[DvBuff.tail].Raw[4]=(uint8_t) (Input0);

    DvData[DvBuff.tail].Raw[5]=(uint8_t) (Input1>>24);
    DvData[DvBuff.tail].Raw[6]=(uint8_t) (Input1>>16);
    DvData[DvBuff.tail].Raw[7]=(uint8_t) (Input1>>8);
    DvData[DvBuff.tail].Raw[8]=(uint8_t) (Input1);

    DvData[DvBuff.tail].Raw[9]=(uint8_t) (Input2>>24);
    DvData[DvBuff.tail].Raw[10]=(uint8_t) (Input2>>16);
    DvData[DvBuff.tail].Raw[11]=(uint8_t) (Input2>>8);
    DvData[DvBuff.tail].Raw[12]=(uint8_t) (Input2);

    DvData[DvBuff.tail].Raw[13]=0xFC;

    if(DvBuff.tail<DV_BUFFER_SIZE)
        DvBuff.tail++;
    else
        DvBuff.tail=0;
}

void DV_Tasks(void)
{
    size_t sz;

    if(DvBuff.head==DvBuff.tail)
        return;

    sz=0;

    do
        sz+=DV_Write(&DvData[DvBuff.head].Raw[sz], 14);
    while(sz!=14);

    DvBuff.head++;

    if(DvBuff.head>=DV_BUFFER_SIZE)
        DvBuff.head=0;
}