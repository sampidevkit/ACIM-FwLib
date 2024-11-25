#include "boards.h"
#include "definitions.h"

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

inline size_t DV_Write(uint8_t* pWrBuffer, const size_t size)
{
    return UART2_Write(pWrBuffer, size);
}

void PFCPWM_SetDutyA(uint16_t Duty)
{
    // Not implement in this design
}

void PFCPWM_SetDutyB(uint16_t Duty)
{
    // Not implement in this design
}

void INVPWM_SetDuty(uint16_t DutyU, uint16_t DutyV, uint16_t DutyW)
{
    MCPWM_ChannelPrimaryDutySet(MCPWM_CH_12, DutyU);
    MCPWM_ChannelPrimaryDutySet(MCPWM_CH_5, DutyV);
    MCPWM_ChannelPrimaryDutySet(MCPWM_CH_6, DutyW);
}
