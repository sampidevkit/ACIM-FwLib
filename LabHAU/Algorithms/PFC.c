#include "PFC.h"

pfc_ui_cxt_t PfcUiCxt;
static volatile int PfcCnt;

#ifndef USE_MY_PFC_ALGORITHM
pfc_inputs_t PfcInputs;
pfc_outputs_t PfcOutputs;

void PFC_Init(void)
{
    printf("\r\n%s done", __FUNCTION__);
}

void PFC_Process(void)
{
    
}
#else
#warning "You are using customize PFC algotithm"
#endif
