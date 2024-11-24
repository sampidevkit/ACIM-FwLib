#include "MC.h"

#ifndef USE_MY_MOTOR_CONTROL_ALGORITHM
mc_inputs_t McInputs;
mc_outputs_t McOutputs;

void MC_Init(void)
{
    
}

void MC_Process(void)
{
    
}
#else
#warning "You are using customize motor control algotithm"
#endif
