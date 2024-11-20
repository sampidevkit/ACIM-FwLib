#ifndef BOARDS_H
#define	BOARDS_H

#include "Common/LibDef.h"
#include "config/labhau/labhau_cfg.h"

typedef struct {
    const uint32_t Rbot; // Top divider resistor
    const uint32_t Rtop; // Bottom divider resistor
    int32_t Iir; // IIR filter buffer
    int32_t Val; // Real voltage in mV unit
} vol_cxt_t;

typedef struct {
    const uint32_t Gain; // Vol/Vol gain 
    int32_t Iir; // IIR filter buffer
    int32_t Val; // Real current in mA unit
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

void Board_Init(void);

void VDC_Enable(void);
void VDC_Disable(void);

void DevMode_Enable(void);
void DevMode_Disable(void);

void LedRun_On(void);
void LedRun_Off(void);
void LedRun_Toggle(void);

void LedErr_On(void);
void LedErr_Off(void);
void LedErr_Toggle(void);

bool Button_Get(void);

#define PFC_GetVac()    (PfcUiCxt.Vac.Val)
#define PFC_GetVdc()    (PfcUiCxt.Vdc.Val)
#define PFC_GetIa()     (PfcUiCxt.Ia.Val)
#define PFC_GetIb()     (PfcUiCxt.Ib.Val)
void PFCPWM_SetDutyA(uint16_t Duty);
void PFCPWM_SetDutyA(uint16_t Duty);

#define INV_GetVdc()    (InvUiCxt.Power.Vol.Val)
#define INV_GetIdc()    (InvUiCxt.Power.Cur.Val)
#define INV_GetVu()     (InvUiCxt.PhaseU.Vol.Val)
#define INV_GetIu()     (InvUiCxt.PhaseU.Cur.Val)
#define INV_GetVv()     (InvUiCxt.PhaseV.Vol.Val)
#define INV_GetIv()     (InvUiCxt.PhaseV.Cur.Val)
#define INV_GetVw()     (InvUiCxt.PhaseW.Vol.Val)
#define INV_GetIw()     (InvUiCxt.PhaseW.Cur.Val)
void INVPWM_SetDuty(uint16_t DutyU, uint16_t DutyV, uint16_t DutyW);

/* **************************************************** CONTROLLER ALGORITHMS */
// Please implement these functions in your application source
void PFC_Algorithm(void);
void INV_Algorithm(void);

#endif
