#include "DataVisualizer.h"
#include "boards.h"

static void DV_Plot(const int32_t *pD)
{
    uint8_t i, j;
    uint8_t DvData[DV_NUM_OF_VAR*4+2];

    j=0;
    DvData[j++]=0x03;

    for(i=0; i<DV_NUM_OF_VAR; i++)
    {
        DvData[j++]=(uint8_t) (pD[i]);
        DvData[j++]=(uint8_t) (pD[i]>>8);
        DvData[j++]=(uint8_t) (pD[i]>>16);
        DvData[j++]=(uint8_t) (pD[i]>>24);
    }

    DvData[j++]=0xFC;

    i=0;

    do
        i+=DV_Write(&DvData[i], j-i);
    while(i<j);
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
