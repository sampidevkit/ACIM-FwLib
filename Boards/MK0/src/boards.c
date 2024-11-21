#include "boards.h"
#include "peripheral/gpio/plib_gpio.h"

static void PfcAdc_IntCb(uint32_t ch, uintptr_t pt)
{

}

static void InvAdc_IntCb(uint32_t ch, uintptr_t pt)
{

}

static void PfcPwm_IntCb(uint32_t ch, uintptr_t pt)
{

}

static void InvPwm_IntCb(uint32_t ch, uintptr_t pt)
{

}

void Board_Init(void)
{

}

void VDC_Enable(void)
{
    VDC_ENABLE_Set();
}

void VDC_Disable(void)
{
    VDC_ENABLE_Clear();
}

void DevMode_Enable(void)
{
    DEV_MODE_DISABLE_Clear();
}

void DevMode_Disable(void)
{
    DEV_MODE_DISABLE_Set();
}

void LedRun_On(void)
{
    IND_RUN_Set();
}

void LedRun_Off(void)
{
    IND_RUN_Clear();
}

void LedRun_Toggle(void)
{
    IND_RUN_Toggle();
}

void LedErr_On(void)
{
    IND_ERR_N_Clear();
}

void LedErr_Off(void)
{
    IND_ERR_N_Set();
}

void LedErr_Toggle(void)
{
    IND_ERR_N_Toggle();
}

bool Button_Get(void)
{
    return 1; // no button
}

void PFCPWM_SetDutyA(uint16_t Duty)
{

}

void PFCPWM_SetDutyB(uint16_t Duty)
{

}

void INVPWM_SetDuty(uint16_t DutyU, uint16_t DutyV, uint16_t DutyW)
{

}
