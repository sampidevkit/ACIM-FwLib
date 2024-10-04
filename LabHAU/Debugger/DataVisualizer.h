#ifndef DATAVISUALIZER_H
#define DATAVISUALIZER_H

#include "Common/LibDef.h"
#include "Project_Cfg.h"

typedef union {
    uint8_t Raw[40];

    struct {
        uint8_t Header;
        uint32_t Vdc;
        uint32_t Idc;
        uint32_t Vu;
        uint32_t Vv;
        uint32_t Vw;
        int32_t Iu;
        int32_t Iu;
        int32_t Iu;
        int32_t Temp;
        uint8_t Footer;
        uint8_t Rfu1;
        uint8_t Rfu2;
    };
} __attribute_packed__ vi_cxt_t;

typedef union {
    uint8_t Raw[36];

    struct {
        uint8_t Header;
        uint32_t Vu;
        uint32_t Vv;
        uint32_t Vw;
        int32_t Iu;
        int32_t Iu;
        int32_t Iu;
        int32_t Temp;
        int32_t Speed;
        uint8_t Footer;
        uint8_t Rfu1;
        uint8_t Rfu2;
    };
} speed_cxt_t;

/* **************************************************************** Prototype */
void DV_PortWrite(uint8_t c);
/* ************************************************************************** */
void DV_VIPlot_Init(vi_cxt_t *pCxt);
void DV_VIPlot(const vi_cxt_t *pCxt);

void DV_SpeedPlot_Init(speed_cxt_t *pCxt);
void DV_SpeedPlot(const speed_cxt_t *pCxt);

#endif
