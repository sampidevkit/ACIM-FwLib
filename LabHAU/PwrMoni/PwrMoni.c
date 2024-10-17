#include "PwrMoni.h"

void PwrMoni_Init(void)
{

}

void PwrMoni_Tasks(void)
{

}

void PwrMoni_Get(pwrmoni_ch_t channel, void *pResult)
{
    int32_t *p1=(int32_t *) pResult;
    pwrmoni_cxt_t *p2=(pwrmoni_cxt_t *) pResult;

    switch(channel)
    {
        case PWRMONI_IU:
            break;

        case PWRMONI_IV:
            break;

        case PWRMONI_IW:
            break;

        case PWRMONI_VU:
            break;

        case PWRMONI_VV:
            break;

        case PWRMONI_VW:
            break;

        case PWRMONI_VDC:
            break;

        case PWRMONI_IDC:
            break;

        case PWRMONI_TEMP:
            break;

        case PWRMONI_ALL:
            break;

        default:
            break;
    }
}

