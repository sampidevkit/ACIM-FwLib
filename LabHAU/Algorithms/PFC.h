#ifndef PFC_H
#define	PFC_H

#include "boards.h"

typedef struct
{
    int32_t Ia; // Current of channel A
    int32_t Ib; // Current of channel B
    int32_t Idc; // Total current
    int32_t Vdc; // Output voltage
    int32_t Vac; // Input voltage
} pfc_inputs_t;

typedef struct
{
    uint16_t PwmA; // PWM duty channel A
    uint16_t PwmB; // PWM duty channel B
} pfc_outputs_t;

extern pfc_inputs_t PfcInputs;
extern pfc_outputs_t PfcOutputs;

void PFC_Init(void);
void PFC_Process(void);

#endif
