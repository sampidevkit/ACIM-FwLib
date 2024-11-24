#ifndef MC_H
#define	MC_H

#include "boards.h"
#include "motors.h"

typedef struct
{
    int32_t U; // Current
    int32_t I; // Voltage
} vi_cxt_t;

typedef struct {
    vi_cxt_t Source; // Power supply
    vi_cxt_t PhaseU; // Voltage & Current of phase U
    vi_cxt_t PhaseV; // Voltage & Current of phase V
    vi_cxt_t PhaseW; // Voltage & Current of phase W
    int32_t Speed; // Motor speed in RPM
    void *pExpData; // Expanded data inputs
} mc_inputs_t;

typedef struct {
    uint16_t DutyU; // PWM duty of phase U
    uint16_t DutyV; // PWM duty of phase V
    uint16_t DutyW; // PWM duty of phase W
    void *pExpData; // Expanded data outputs
} mc_outputs_t;

extern mc_inputs_t McInputs;
extern mc_outputs_t McOutputs;

void MC_Init(void);
void MC_Process(void);

#endif
