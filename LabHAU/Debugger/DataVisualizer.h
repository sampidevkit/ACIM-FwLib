#ifndef DATAVISUALIZER_H
#define DATAVISUALIZER_H

#include "Common/LibDef.h"
#include "labhau_cfg.h"

/* ************************************************************************** */
#if(DV_NUM_OF_VAR==3)
void DV_PlotData(int32_t Input0, int32_t Input1, int32_t Input2);
#elif(DV_NUM_OF_VAR==4)
void DV_PlotData(int32_t Input0, int32_t Input1, int32_t Input2, int32_t Input3);
#elif(DV_NUM_OF_VAR==5)
void DV_PlotData(int32_t Input0, int32_t Input1, int32_t Input2, int32_t Input3, int32_t Input4);
#endif
#endif
