#ifndef DATAVISUALIZER_H
#define DATAVISUALIZER_H

#include "Common/LibDef.h"
#include "labhau_cfg.h"

typedef struct {
    uint8_t Raw[14];
} __attribute_packed__ plot_cxt_t;

typedef struct
{
    int head;
    int tail;
} __attribute_packed__ plot_ring_buff_t;

/* ************************************************************************** */
void DV_Init(void);

// Current plot: Input[0:2]=[Iu, Iv, Iw]
// Voltage plot: Input[0:2]=[Vu, Vv, Vw]
// Speed plot: Input[0:2]=[SpeedSet, SpeedEst, SpeedReal]
void DV_Plot(int32_t Input0, int32_t Input1, int32_t Input2);
void DV_Tasks(void);

#endif
