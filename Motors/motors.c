#include "motors.h"

const motor_fpara_t MotorFParameters={

};

motor_ipara_t MotorIParameters;

void Motor_Init(void)
{
#ifdef USE_MY_MOTOR_PARAMETERS
    Motor_myInit();
#else
    printf("\r\n%s done", __FUNCTION__);
#endif
}
