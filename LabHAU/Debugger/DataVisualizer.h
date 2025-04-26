#ifndef DATAVISUALIZER_H
#define DATAVISUALIZER_H

#include "Common/LibDef.h"
#include "labhau_cfg.h"

/* ************************************************************************** */
void DV_Init(void);
void DV_PlotData(const int32_t *pD);
// Current plot: Input[0:2]=[Iu, Iv, Iw]
// Voltage plot: Input[0:2]=[Vu, Vv, Vw]
// Speed plot: Input[0:2]=[SpeedSet, SpeedEst, SpeedReal]
void DV_PlotData3(int32_t Input0, int32_t Input1, int32_t Input2);
#define DV_Plot(Input0, Input1, Input2) DV_PlotData3((int32_t)Input0, (int32_t)Input1, (int32_t)Input2)
#define DV_FPlot(Input0, Input1, Input2) DV_PlotData3((int32_t)Input0, (int32_t)Input1, (int32_t)Input2)
void DV_PlotData4(int32_t Input0, int32_t Input1, int32_t Input2, int32_t Input3);
void DV_PlotData5(int32_t Input0, int32_t Input1, int32_t Input2, int32_t Input3, int32_t Input4);
void DV_Tasks(void);

#endif
