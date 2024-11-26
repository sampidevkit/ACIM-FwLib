#ifndef MC_H
#define	MC_H

#include "boards.h"
#include "motors.h"
#include "Common/Utils.h"

typedef struct {
    const gain_t Gain; // Gain to convert ADC to real voltage
    int32_t Iir; // IIR filter buffer
    int32_t Val; // Present ADC value
} inv_vol_cxt_t;

typedef struct {
    const gain_t Gain; // Gain to convert ADC to real current
    int32_t Iir; // IIR filter buffer
    int32_t Val; // Present ADC value
} inv_cur_cxt_t;

typedef struct {
    inv_vol_cxt_t Vol; // Voltage of phase or source
    inv_cur_cxt_t Cur; // Current of phase or source
} inv_phase_cxt_t, inv_src_cxt_t;

typedef struct {
    const int32_t AdcVref; // ADC Vref
    const int32_t AdcReso; // ADC resolution
    inv_src_cxt_t Source; // Inverter power source context
    inv_phase_cxt_t PhaseU; // Inverter phase U context
    inv_phase_cxt_t PhaseV; // Inverter phase V context
} inv_ui_cxt_t;

typedef struct {
    int32_t U; // Current
    int32_t I; // Voltage
} mc_ui_cxt_t;

typedef struct {
    mc_ui_cxt_t Source; // Power supply
    mc_ui_cxt_t PhaseU; // Voltage & Current of phase U
    mc_ui_cxt_t PhaseV; // Voltage & Current of phase V
    mc_ui_cxt_t PhaseW; // Voltage & Current of phase W
    int32_t Speed; // Motor speed in RPM
    void *pExpData; // Expanded data inputs
} mc_inputs_t;

typedef struct {
    uint16_t DutyU; // PWM duty of phase U
    uint16_t DutyV; // PWM duty of phase V
    uint16_t DutyW; // PWM duty of phase W
    void *pExpData; // Expanded data outputs
} mc_outputs_t;

extern inv_ui_cxt_t InvUiCxt; // defined in board.c
extern mc_inputs_t McInputs; // defined in MC.c
extern mc_outputs_t McOutputs; // defined in MC.c

void MC_Init(void);
void MC_myInit(void);
void MC_Process(void);
void MC_myProcess(void);

#endif
