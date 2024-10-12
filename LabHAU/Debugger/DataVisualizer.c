#include "DataVisualizer.h"

void DV_VIPlot_Init(vi_cxt_t *pCxt)
{
    pCxt->Header=0x03;
    pCxt->Footer=0xFC;
}

void DV_VIPlot(const vi_cxt_t *pCxt)
{
    int i;
    
    for(i=0; i<38; i++)
        DV_PortWrite(pCxt->Raw[i]);
}

void DV_SpeedPlot_Init(speed_cxt_t *pCxt)
{
    pCxt->Header=0x03;
    pCxt->Footer=0xFC;
}

void DV_SpeedPlot(const speed_cxt_t *pCxt)
{
    int i;
    
    for(i=0; i<34; i++)
        DV_PortWrite(pCxt->Raw[i]);
}