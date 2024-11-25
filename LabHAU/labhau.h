#ifndef LABHAU_H
#define	LABHAU_H

#include "labhau_cfg.h"
#include "boards.h"
#include "motors.h"
#include "Algorithms/MC.h"
#include "Algorithms/PFC.h"
#include "Common/Tick.h"
#include "Common/Utils.h"
#include "Common/LibDef.h"
#include "Debugger/DataVisualizer.h"
#include "Maths/FastTrigonometric.h"

typedef struct {
    const uint32_t Rbot; // Top divider resistor
    const uint32_t Rtop; // Bottom divider resistor
    int32_t Iir; // IIR filter buffer
    int32_t Val; // Present ADC value
} vol_cxt_t;

typedef struct {
    const uint32_t Gain; // Vol/Vol gain 
    int32_t Iir; // IIR filter buffer
    int32_t Val; // Present ADC value
} cur_cxt_t;

typedef struct {
    vol_cxt_t Vol; // Voltage of phase or source
    cur_cxt_t Cur; // Current of phase or source
} phase_cxt_t, src_cxt_t;

typedef struct {
    vol_cxt_t Vac; // Alternative power input
    vol_cxt_t Vdc; // Output voltage
    cur_cxt_t Idc; // Total current of channel A & B
    cur_cxt_t Ia; // Current of PFC channel A
    cur_cxt_t Ib; // Current of PFC channel B
} pfc_ui_cxt_t;

typedef struct {
    src_cxt_t Power; // Inverter power source context
    phase_cxt_t PhaseU; // Inverter phase U context
    phase_cxt_t PhaseV; // Inverter phase V context
    phase_cxt_t PhaseW; // Inverter phase W context
} inv_ui_cxt_t;

extern pfc_ui_cxt_t PfcUiCxt;
extern inv_ui_cxt_t InvUiCxt;

void LABHAU_Init(void);

#endif
