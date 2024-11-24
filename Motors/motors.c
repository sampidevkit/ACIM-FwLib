#include "motors.h"

#ifndef USE_MY_MOTOR_PARAMETERS

const motor_fpara_t MotorFParameters={
    
};

motor_ipara_t MotorIParameters;

void Motor_Init(void)
{
    
}
#else
#warning "You are using customize motor parameters"
#endif
