/*******************************************************************************
  Motor Control PWM (MCPWM) Peripheral Library (PLIB)

  Company:
    Microchip Technology Inc.

  File Name:
    plib_mcpwm.c

  Summary:
    MCPWM Source File

  Description:
    None

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
#include "device.h"
#include "plib_mcpwm.h"
#include "interrupts.h"

// *****************************************************************************

// *****************************************************************************
// Section: MCPWM Implementation
// *****************************************************************************
// *****************************************************************************
static volatile MCPWM_CH_OBJECT mcpwmObj[12];

void MCPWM_Initialize (void)
{
    /* PTCON register  */
    /*  SEVTPS  = 0 */
    /*  PCLKDIV = 0 */
    /*  SEIEN   = false */
    PTCON = 0x0;

    /* STCON register  */
    /*  SEVTPS  = 0 */
    /*  SCLKDIV = 0 */
    /*  SSEIEN = false */
    STCON = 0x0;

    PTPER = 3000;
    STPER = 2000;
    SEVTCMP = 10;
    SSEVTCMP = 10;

    /*********** Channel 5 Configurations **********/
    /* PWMCON5 register  */
    /*  MTBS   = 0 */
    /*  PTDIR  =  0 */
    /*  ECAM   =  1 */
    /*  DTCP   =  0 */
    /*  DTC    =  0 */
    /*  ITB    = 0 */
    /*  PWMHIEN =  false */
    /*  PWMLIEN = false */
    /*  TRGIEN = false */
    /*  CLIEN = false */
    /*  FLTIEN = false */
    PWMCON5 = 0x400;

    /* IOCON5 register  */
    /*  SWAP    = 0*/
    /*  PMOD    = 0*/
    /*  POLH    = 0*/
    /*  POLL    = 0*/
    /*  FLTDAT  = 0b00 */
    /*  FLTMOD  = 1 */
    /*  PENH  = 1 */
    /*  PENL  = 1 */
    /*  FLTPOL  = 1  */
    /*  FLTSRC  = 0  */
    /*  CLDAT  = 0b00 */
    /*  CLMOD  = 0 */
    /*  CLPOL  = 0  */
    /*  CLSRC  = 0  */
    IOCON5 = 0x5c000;

    PDC5 = 0;
    SDC5 = 500;
    PHASE5 = 0;

    /* Dead Time */
    DTR5 = 76;
    ALTDTR5 = 76;

    /* Trigger Generator */
    TRGCON5 = 0x400;
    TRIG5 = 2990;
    STRIG5 = 0;

    /* leading edge blanking */
    /* LEBCON5 register  */
    /*  CLLEBEN    = false  */
    /*  FLTLEBEN   = false */
    /*  PLF        = 0  */
    /*  PLR        = 0  */
    /*  PHF        = 0  */
    /*  PHR        = 0  */
    LEBCON5 = 0x0;
    LEBDLY5 = 10;


    /*********** Channel 6 Configurations **********/
    /* PWMCON6 register  */
    /*  MTBS   = 0 */
    /*  PTDIR  =  0 */
    /*  ECAM   =  1 */
    /*  DTCP   =  0 */
    /*  DTC    =  0 */
    /*  ITB    = 0 */
    /*  PWMHIEN =  false */
    /*  PWMLIEN = false */
    /*  TRGIEN = false */
    /*  CLIEN = false */
    /*  FLTIEN = false */
    PWMCON6 = 0x400;

    /* IOCON6 register  */
    /*  SWAP    = 0*/
    /*  PMOD    = 0*/
    /*  POLH    = 0*/
    /*  POLL    = 0*/
    /*  FLTDAT  = 0b00 */
    /*  FLTMOD  = 1 */
    /*  PENH  = 1 */
    /*  PENL  = 1 */
    /*  FLTPOL  = 1  */
    /*  FLTSRC  = 0  */
    /*  CLDAT  = 0b00 */
    /*  CLMOD  = 0 */
    /*  CLPOL  = 0  */
    /*  CLSRC  = 0  */
    IOCON6 = 0x5c000;

    PDC6 = 0;
    SDC6 = 500;
    PHASE6 = 0;

    /* Dead Time */
    DTR6 = 76;
    ALTDTR6 = 76;

    /* Trigger Generator */
    TRGCON6 = 0x400;
    TRIG6 = 2990;
    STRIG6 = 0;

    /* leading edge blanking */
    /* LEBCON6 register  */
    /*  CLLEBEN    = false  */
    /*  FLTLEBEN   = false */
    /*  PLF        = 0  */
    /*  PLR        = 0  */
    /*  PHF        = 0  */
    /*  PHR        = 0  */
    LEBCON6 = 0x0;
    LEBDLY6 = 10;


    /*********** Channel 12 Configurations **********/
    /* PWMCON12 register  */
    /*  MTBS   = 0 */
    /*  PTDIR  =  0 */
    /*  ECAM   =  1 */
    /*  DTCP   =  0 */
    /*  DTC    =  0 */
    /*  ITB    = 0 */
    /*  PWMHIEN =  false */
    /*  PWMLIEN = false */
    /*  TRGIEN = false */
    /*  CLIEN = false */
    /*  FLTIEN = true */
    PWMCON12 = 0x800400;

    /* IOCON12 register  */
    /*  SWAP    = 0*/
    /*  PMOD    = 0*/
    /*  POLH    = 0*/
    /*  POLL    = 0*/
    /*  FLTDAT  = 0b00 */
    /*  FLTMOD  = 1 */
    /*  PENH  = 1 */
    /*  PENL  = 1 */
    /*  FLTPOL  = 1  */
    /*  FLTSRC  = 0  */
    /*  CLDAT  = 0b00 */
    /*  CLMOD  = 0 */
    /*  CLPOL  = 0  */
    /*  CLSRC  = 0  */
    IOCON12 = 0x5c000;

    PDC12 = 0;
    SDC12 = 500;
    PHASE12 = 0;

    /* Dead Time */
    DTR12 = 76;
    ALTDTR12 = 76;

    /* Trigger Generator */
    TRGCON12 = 0x400;
    TRIG12 = 2990;
    STRIG12 = 0;

    /* leading edge blanking */
    /* LEBCON12 register  */
    /*  CLLEBEN    = false  */
    /*  FLTLEBEN   = false */
    /*  PLF        = 0  */
    /*  PLR        = 0  */
    /*  PHF        = 0  */
    /*  PHR        = 0  */
    LEBCON12 = 0x0;
    LEBDLY12 = 10;

    /* Enable interrupt */
    IEC7SET = _IEC7_PWM12IE_MASK;
    mcpwmObj[11].callback = NULL;

}


void MCPWM_Start(void)
{
    /* Enable MCPWM module */
    PTCONbits.PTEN = 0x1;
}

void MCPWM_Stop(void)
{
    /* Disable MCPWM module */
    PTCONbits.PTEN = 0x0;
}

void MCPWM_PrimaryPeriodSet(uint16_t period)
{
    PTPER = period;
}

uint16_t MCPWM_PrimaryPeriodGet(void)
{
    return (uint16_t)PTPER;
}


void MCPWM_SecondaryPeriodSet(uint16_t period)
{
    STPER = period;
}

uint16_t MCPWM_SecondaryPeriodGet(void)
{
    return (uint16_t)STPER;
}

void MCPWM_ChannelPrimaryDutySet(MCPWM_CH_NUM channel, uint16_t duty)
{
    *(&PDC1 + (0x40U * (channel))) = duty;
}

void MCPWM_ChannelSecondaryDutySet(MCPWM_CH_NUM channel, uint16_t duty)
{
    *(&SDC1 + (0x40U * (channel))) = duty;
}

void MCPWM_ChannelDeadTimeSet(MCPWM_CH_NUM channel, uint16_t high_deadtime, uint16_t low_deadtime)
{
    *(&DTR1 + (0x40U * (channel))) = ((uint32_t)high_deadtime & (uint32_t)0x3FFFU);
    *(&ALTDTR1 + (0x40U * (channel))) = ((uint32_t)low_deadtime & (uint32_t)0x3FFFU);
}

void MCPWM_ChannelPrimaryTriggerSet(MCPWM_CH_NUM channel, uint16_t trigger)
{
    *(&TRIG1 + (0x40U * (channel))) = trigger;
}

void MCPWM_ChannelSecondaryTriggerSet(MCPWM_CH_NUM channel, uint16_t trigger)
{
    *(&STRIG1 + (0x40U * (channel))) = trigger;
}

void MCPWM_ChannelLeadingEdgeBlankingDelaySet(MCPWM_CH_NUM channel, uint16_t delay)
{
    *(&LEBDLY1 + (0x40U * (channel))) = delay;
}

void MCPWM_ChannelPinsOverrideEnable(MCPWM_CH_NUM channel)
{
    *(&IOCON1 + (0x40U * (channel))) |= _IOCON1_OVRENL_MASK | _IOCON1_OVRENH_MASK;
}

void MCPWM_ChannelPinsOverrideDisable(MCPWM_CH_NUM channel)
{
    *(&IOCON1 + (0x40U * (channel))) &= ~(_IOCON1_OVRENL_MASK | _IOCON1_OVRENH_MASK);
}

void MCPWM_ChannelPinsOwnershipEnable(MCPWM_CH_NUM channel)
{
    *(&IOCON1 + (0x40U * (channel))) |= _IOCON1_PENH_MASK | _IOCON1_PENL_MASK;
}

void MCPWM_ChannelPinsOwnershipDisable(MCPWM_CH_NUM channel)
{
    *(&IOCON1 + (0x40U * (channel))) &= ~(_IOCON1_PENH_MASK | _IOCON1_PENL_MASK);
}



void __attribute__((used)) PWM12_InterruptHandler(void)
{
    uint32_t tmp;
    uintptr_t context = mcpwmObj[11].context;
    uint32_t status = PWMCON12 & MCPWM_STATUS_MASK;

    tmp = PWMCON12bits.PWMHIF;
    if (((PWMCON12bits.PWMHIEN) != 0U) && (tmp != 0U))
    {
        PWMCON12bits.PWMHIF = 0;
    }

    tmp = PWMCON12bits.PWMLIF;
    if (((PWMCON12bits.PWMLIEN) != 0U) && (tmp != 0U))
    {
        PWMCON12bits.PWMLIF = 0;
    }

    tmp = PWMCON12bits.TRGIF;
    if (((PWMCON12bits.TRGIEN) != 0U) && (tmp != 0U))
    {
        PWMCON12bits.TRGIF = 0;
    }

    tmp = PWMCON12bits.CLIF;
    if (((PWMCON12bits.CLIEN) != 0U) && (tmp != 0U))
    {
        PWMCON12bits.CLIEN = 0;
        PWMCON12bits.CLIF = 0;
    }

    tmp = PWMCON12bits.FLTIF;
    if (((PWMCON12bits.FLTIEN) != 0U) && (tmp != 0U))
    {
        PWMCON12bits.FLTIEN = 0;
        PWMCON12bits.FLTIF = 0;
    }

    IFS7CLR = _IFS7_PWM12IF_MASK;    //Clear IRQ flag

    PWMCON12bits.FLTIEN = 1;

    if( (mcpwmObj[11].callback != NULL))
    {
        mcpwmObj[11].callback((MCPWM_CH_STATUS)status, context);
    }
}


void MCPWM_CallbackRegister(MCPWM_CH_NUM channel, MCPWM_CH_CALLBACK callback, uintptr_t context)
{
    mcpwmObj[channel].callback = callback;
    mcpwmObj[channel].context = context;
}
