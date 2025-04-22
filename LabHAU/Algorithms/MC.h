#ifndef MC_H
#define	MC_H

#include "boards.h"
#include "motors.h"
#include "Common/Utils.h"

typedef struct {
    float Gain; // Gain to convert ADC to real voltage
    int32_t Offset; // Offset value 
    int32_t Iir; // IIR filter buffer
    int32_t Val; // Present ADC value
} inv_vol_cxt_t, inv_cur_cxt_t;

typedef struct {
    inv_vol_cxt_t Vol; // Voltage of phase or source
    inv_cur_cxt_t Cur; // Current of phase or source
} inv_phase_cxt_t, inv_src_cxt_t;

typedef struct {
    uint16_t PwmDutyMax; // Max PWM duty
    int32_t AdcVref; // ADC Vref
    int32_t AdcReso; // ADC resolution
    inv_vol_cxt_t InterVref; // Internal Vref
    inv_src_cxt_t Source; // Inverter power source context
    inv_phase_cxt_t PhaseU; // Inverter phase U context
    inv_phase_cxt_t PhaseV; // Inverter phase V context
    inv_phase_cxt_t PhaseW; // Inverter phase W context
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

typedef void (*mc_process_fnc)(void*); // Motor control process function

extern inv_ui_cxt_t InvUiCxt; // defined in board.c
extern mc_inputs_t McInputs; // defined in MC.c
extern mc_outputs_t McOutputs; // defined in MC.c

void MC_Init(void);
void MC_Task(void);
/* **************************************************** User's implementation */
void MC_myInit(void);
void MC_myProcess(void);
/* ************************************************************************** */
#define McIs    McInputs.Source.I
#define McIu    McInputs.PhaseU.I
#define McIv    McInputs.PhaseV.I
#define McIw    McInputs.PhaseW.I

#define McUs    McInputs.Source.U
#define McUu    McInputs.PhaseU.U
#define McUv    McInputs.PhaseV.U
#define McUw    McInputs.PhaseW.U

#define McSpeed McInputs.Speed

#define McDutyU McOutputs.DutyU
#define McDutyV McOutputs.DutyV
#define McDutyW McOutputs.DutyW

#define McDutyMax InvUiCxt.PwmDutyMax

#endif
