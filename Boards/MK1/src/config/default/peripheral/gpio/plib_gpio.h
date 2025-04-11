/*******************************************************************************
  GPIO PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_gpio.h UUUUUUUUU

  Summary:
    GPIO PLIB Header File

  Description:
    This library provides an interface to control and interact with Parallel
    Input/Output controller (GPIO) module.

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

#ifndef PLIB_GPIO_H
#define PLIB_GPIO_H

#include <device.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************


/*** Macros for ENC_A pin ***/
#define ENC_A_Get()               ((PORTG >> 6) & 0x1U)
#define ENC_A_GetLatch()          ((LATG >> 6) & 0x1U)
#define ENC_A_PIN                  GPIO_PIN_RG6

/*** Macros for ENC_B pin ***/
#define ENC_B_Get()               ((PORTG >> 7) & 0x1U)
#define ENC_B_GetLatch()          ((LATG >> 7) & 0x1U)
#define ENC_B_PIN                  GPIO_PIN_RG7

/*** Macros for IDC_SEN pin ***/
#define IDC_SEN_Get()               ((PORTB >> 0) & 0x1U)
#define IDC_SEN_GetLatch()          ((LATB >> 0) & 0x1U)
#define IDC_SEN_PIN                  GPIO_PIN_RB0

/*** Macros for DEV_RELAY pin ***/
#define DEV_RELAY_Set()               (LATBSET = (1U<<2))
#define DEV_RELAY_Clear()             (LATBCLR = (1U<<2))
#define DEV_RELAY_Toggle()            (LATBINV= (1U<<2))
#define DEV_RELAY_OutputEnable()      (TRISBCLR = (1U<<2))
#define DEV_RELAY_InputEnable()       (TRISBSET = (1U<<2))
#define DEV_RELAY_Get()               ((PORTB >> 2) & 0x1U)
#define DEV_RELAY_GetLatch()          ((LATB >> 2) & 0x1U)
#define DEV_RELAY_PIN                  GPIO_PIN_RB2

/*** Macros for VDC_DIV pin ***/
#define VDC_DIV_Get()               ((PORTC >> 0) & 0x1U)
#define VDC_DIV_GetLatch()          ((LATC >> 0) & 0x1U)
#define VDC_DIV_PIN                  GPIO_PIN_RC0

/*** Macros for IND_ERR pin ***/
#define IND_ERR_Set()               (LATCSET = (1U<<1))
#define IND_ERR_Clear()             (LATCCLR = (1U<<1))
#define IND_ERR_Toggle()            (LATCINV= (1U<<1))
#define IND_ERR_OutputEnable()      (TRISCCLR = (1U<<1))
#define IND_ERR_InputEnable()       (TRISCSET = (1U<<1))
#define IND_ERR_Get()               ((PORTC >> 1) & 0x1U)
#define IND_ERR_GetLatch()          ((LATC >> 1) & 0x1U)
#define IND_ERR_PIN                  GPIO_PIN_RC1

/*** Macros for IND_RUN pin ***/
#define IND_RUN_Set()               (LATCSET = (1U<<11))
#define IND_RUN_Clear()             (LATCCLR = (1U<<11))
#define IND_RUN_Toggle()            (LATCINV= (1U<<11))
#define IND_RUN_OutputEnable()      (TRISCCLR = (1U<<11))
#define IND_RUN_InputEnable()       (TRISCSET = (1U<<11))
#define IND_RUN_Get()               ((PORTC >> 11) & 0x1U)
#define IND_RUN_GetLatch()          ((LATC >> 11) & 0x1U)
#define IND_RUN_PIN                  GPIO_PIN_RC11

/*** Macros for MC_IW_SEN pin ***/
#define MC_IW_SEN_Get()               ((PORTA >> 8) & 0x1U)
#define MC_IW_SEN_GetLatch()          ((LATA >> 8) & 0x1U)
#define MC_IW_SEN_PIN                  GPIO_PIN_RA8

/*** Macros for MC_IU_SEN pin ***/
#define MC_IU_SEN_Get()               ((PORTA >> 4) & 0x1U)
#define MC_IU_SEN_GetLatch()          ((LATA >> 4) & 0x1U)
#define MC_IU_SEN_PIN                  GPIO_PIN_RA4

/*** Macros for SLED pin ***/
#define SLED_Set()               (LATDSET = (1U<<8))
#define SLED_Clear()             (LATDCLR = (1U<<8))
#define SLED_Toggle()            (LATDINV= (1U<<8))
#define SLED_OutputEnable()      (TRISDCLR = (1U<<8))
#define SLED_InputEnable()       (TRISDSET = (1U<<8))
#define SLED_Get()               ((PORTD >> 8) & 0x1U)
#define SLED_GetLatch()          ((LATD >> 8) & 0x1U)
#define SLED_PIN                  GPIO_PIN_RD8

/*** Macros for DV_TXD pin ***/
#define DV_TXD_Get()               ((PORTC >> 10) & 0x1U)
#define DV_TXD_GetLatch()          ((LATC >> 10) & 0x1U)
#define DV_TXD_PIN                  GPIO_PIN_RC10

/*** Macros for DB_TXD pin ***/
#define DB_TXD_Get()               ((PORTB >> 7) & 0x1U)
#define DB_TXD_GetLatch()          ((LATB >> 7) & 0x1U)
#define DB_TXD_PIN                  GPIO_PIN_RB7

/*** Macros for DB_RXD pin ***/
#define DB_RXD_Get()               ((PORTC >> 13) & 0x1U)
#define DB_RXD_GetLatch()          ((LATC >> 13) & 0x1U)
#define DB_RXD_PIN                  GPIO_PIN_RC13

/*** Macros for DV_RXD pin ***/
#define DV_RXD_Get()               ((PORTB >> 8) & 0x1U)
#define DV_RXD_GetLatch()          ((LATB >> 8) & 0x1U)
#define DV_RXD_PIN                  GPIO_PIN_RB8

/*** Macros for MC_PWM_WH pin ***/
#define MC_PWM_WH_Get()               ((PORTC >> 6) & 0x1U)
#define MC_PWM_WH_GetLatch()          ((LATC >> 6) & 0x1U)
#define MC_PWM_WH_PIN                  GPIO_PIN_RC6

/*** Macros for MC_PWM_WL pin ***/
#define MC_PWM_WL_Get()               ((PORTC >> 7) & 0x1U)
#define MC_PWM_WL_GetLatch()          ((LATC >> 7) & 0x1U)
#define MC_PWM_WL_PIN                  GPIO_PIN_RC7

/*** Macros for MC_PWM_VH pin ***/
#define MC_PWM_VH_Get()               ((PORTC >> 8) & 0x1U)
#define MC_PWM_VH_GetLatch()          ((LATC >> 8) & 0x1U)
#define MC_PWM_VH_PIN                  GPIO_PIN_RC8

/*** Macros for MC_PWM_UH pin ***/
#define MC_PWM_UH_Get()               ((PORTD >> 5) & 0x1U)
#define MC_PWM_UH_GetLatch()          ((LATD >> 5) & 0x1U)
#define MC_PWM_UH_PIN                  GPIO_PIN_RD5

/*** Macros for PC_PWM_UL pin ***/
#define PC_PWM_UL_Get()               ((PORTD >> 6) & 0x1U)
#define PC_PWM_UL_GetLatch()          ((LATD >> 6) & 0x1U)
#define PC_PWM_UL_PIN                  GPIO_PIN_RD6

/*** Macros for MC_PWM_VL pin ***/
#define MC_PWM_VL_Get()               ((PORTC >> 9) & 0x1U)
#define MC_PWM_VL_GetLatch()          ((LATC >> 9) & 0x1U)
#define MC_PWM_VL_PIN                  GPIO_PIN_RC9

/*** Macros for INV_FLT_N pin ***/
#define INV_FLT_N_Get()               ((PORTF >> 1) & 0x1U)
#define INV_FLT_N_GetLatch()          ((LATF >> 1) & 0x1U)
#define INV_FLT_N_PIN                  GPIO_PIN_RF1

/*** Macros for PWR_RELAY pin ***/
#define PWR_RELAY_Set()               (LATBSET = (1U<<12))
#define PWR_RELAY_Clear()             (LATBCLR = (1U<<12))
#define PWR_RELAY_Toggle()            (LATBINV= (1U<<12))
#define PWR_RELAY_OutputEnable()      (TRISBCLR = (1U<<12))
#define PWR_RELAY_InputEnable()       (TRISBSET = (1U<<12))
#define PWR_RELAY_Get()               ((PORTB >> 12) & 0x1U)
#define PWR_RELAY_GetLatch()          ((LATB >> 12) & 0x1U)
#define PWR_RELAY_PIN                  GPIO_PIN_RB12


// *****************************************************************************
/* GPIO Port

  Summary:
    Identifies the available GPIO Ports.

  Description:
    This enumeration identifies the available GPIO Ports.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all ports are available on all devices.  Refer to the specific
    device data sheet to determine which ports are supported.
*/


#define    GPIO_PORT_A  (0)
#define    GPIO_PORT_B  (1)
#define    GPIO_PORT_C  (2)
#define    GPIO_PORT_D  (3)
#define    GPIO_PORT_E  (4)
#define    GPIO_PORT_F  (5)
#define    GPIO_PORT_G  (6)
typedef uint32_t GPIO_PORT;

typedef enum
{
    GPIO_INTERRUPT_ON_MISMATCH,
    GPIO_INTERRUPT_ON_RISING_EDGE,
    GPIO_INTERRUPT_ON_FALLING_EDGE,
    GPIO_INTERRUPT_ON_BOTH_EDGES,
}GPIO_INTERRUPT_STYLE;

// *****************************************************************************
/* GPIO Port Pins

  Summary:
    Identifies the available GPIO port pins.

  Description:
    This enumeration identifies the available GPIO port pins.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all pins are available on all devices.  Refer to the specific
    device data sheet to determine which pins are supported.
*/


#define     GPIO_PIN_RA0  (0U)
#define     GPIO_PIN_RA1  (1U)
#define     GPIO_PIN_RA4  (4U)
#define     GPIO_PIN_RA7  (7U)
#define     GPIO_PIN_RA8  (8U)
#define     GPIO_PIN_RA10  (10U)
#define     GPIO_PIN_RA11  (11U)
#define     GPIO_PIN_RA12  (12U)
#define     GPIO_PIN_RB0  (16U)
#define     GPIO_PIN_RB1  (17U)
#define     GPIO_PIN_RB2  (18U)
#define     GPIO_PIN_RB3  (19U)
#define     GPIO_PIN_RB4  (20U)
#define     GPIO_PIN_RB5  (21U)
#define     GPIO_PIN_RB6  (22U)
#define     GPIO_PIN_RB7  (23U)
#define     GPIO_PIN_RB8  (24U)
#define     GPIO_PIN_RB9  (25U)
#define     GPIO_PIN_RB10  (26U)
#define     GPIO_PIN_RB11  (27U)
#define     GPIO_PIN_RB12  (28U)
#define     GPIO_PIN_RB13  (29U)
#define     GPIO_PIN_RB14  (30U)
#define     GPIO_PIN_RB15  (31U)
#define     GPIO_PIN_RC0  (32U)
#define     GPIO_PIN_RC1  (33U)
#define     GPIO_PIN_RC2  (34U)
#define     GPIO_PIN_RC6  (38U)
#define     GPIO_PIN_RC7  (39U)
#define     GPIO_PIN_RC8  (40U)
#define     GPIO_PIN_RC9  (41U)
#define     GPIO_PIN_RC10  (42U)
#define     GPIO_PIN_RC11  (43U)
#define     GPIO_PIN_RC12  (44U)
#define     GPIO_PIN_RC13  (45U)
#define     GPIO_PIN_RC15  (47U)
#define     GPIO_PIN_RD5  (53U)
#define     GPIO_PIN_RD6  (54U)
#define     GPIO_PIN_RD8  (56U)
#define     GPIO_PIN_RE12  (76U)
#define     GPIO_PIN_RE13  (77U)
#define     GPIO_PIN_RE14  (78U)
#define     GPIO_PIN_RE15  (79U)
#define     GPIO_PIN_RF0  (80U)
#define     GPIO_PIN_RF1  (81U)
#define     GPIO_PIN_RG6  (102U)
#define     GPIO_PIN_RG7  (103U)
#define     GPIO_PIN_RG8  (104U)
#define     GPIO_PIN_RG9  (105U)

    /* This element should not be used in any of the GPIO APIs.
       It will be used by other modules or application to denote that none of the GPIO Pin is used */
#define    GPIO_PIN_NONE   (-1)

typedef uint32_t GPIO_PIN;


void GPIO_Initialize(void);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on multiple pins of a port
// *****************************************************************************
// *****************************************************************************

uint32_t GPIO_PortRead(GPIO_PORT port);

void GPIO_PortWrite(GPIO_PORT port, uint32_t mask, uint32_t value);

uint32_t GPIO_PortLatchRead ( GPIO_PORT port );

void GPIO_PortSet(GPIO_PORT port, uint32_t mask);

void GPIO_PortClear(GPIO_PORT port, uint32_t mask);

void GPIO_PortToggle(GPIO_PORT port, uint32_t mask);

void GPIO_PortInputEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortOutputEnable(GPIO_PORT port, uint32_t mask);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************

static inline void GPIO_PinWrite(GPIO_PIN pin, bool value)
{
     uint32_t xvalue = (uint32_t)value;
    GPIO_PortWrite((pin>>4U), (uint32_t)(0x1U) << (pin & 0xFU), (xvalue) << (pin & 0xFU));
}

static inline bool GPIO_PinRead(GPIO_PIN pin)
{
    return ((((GPIO_PortRead((GPIO_PORT)(pin>>4U))) >> (pin & 0xFU)) & 0x1U) != 0U);
}

static inline bool GPIO_PinLatchRead(GPIO_PIN pin)
{
    return (((GPIO_PortLatchRead((GPIO_PORT)(pin>>4U)) >> (pin & 0xFU)) & 0x1U) != 0U);
}

static inline void GPIO_PinToggle(GPIO_PIN pin)
{
    GPIO_PortToggle((pin>>4U), (uint32_t)0x1U << (pin & 0xFU));
}

static inline void GPIO_PinSet(GPIO_PIN pin)
{
    GPIO_PortSet((pin>>4U), (uint32_t)0x1U << (pin & 0xFU));
}

static inline void GPIO_PinClear(GPIO_PIN pin)
{
    GPIO_PortClear((pin>>4U), (uint32_t)0x1U << (pin & 0xFU));
}

static inline void GPIO_PinInputEnable(GPIO_PIN pin)
{
    GPIO_PortInputEnable((pin>>4U), (uint32_t)0x1U << (pin & 0xFU));
}

static inline void GPIO_PinOutputEnable(GPIO_PIN pin)
{
    GPIO_PortOutputEnable((pin>>4U), (uint32_t)0x1U << (pin & 0xFU));
}


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END
#endif // PLIB_GPIO_H
