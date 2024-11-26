#include "motors.h"

const motor_fpara_t MotorFParameters={

};

motor_ipara_t MotorIParameters;

void Motor_Init(void)
{
#ifdef USE_MY_MOTOR_PARAMETERS
#warning "You are using Motor_myInit()"
    Motor_myInit();
#else
#warning "Your Motor_myInit() has NOT been called in this option"
    printf("\r\n%s done", __FUNCTION__);
#endif
}
