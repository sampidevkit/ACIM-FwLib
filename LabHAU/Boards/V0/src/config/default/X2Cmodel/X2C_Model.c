/**
 * @file
 * @brief This file contains the initialization and communication functions for the X2C model.
 *
 * @copyright (c) 2013, Linz Center of Mechatronics GmbH (LCM)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 * conditions, and the following disclaimer in the documentation and/or other materials provided
 * with the distribution.
 *
 * 3. Neither the name of the [organization] nor the names of its contributors may be used to
 * endorse or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*******************************************************************************
Headers inclusions
*******************************************************************************/
#include "Target.h"
#include "TableStruct.h"
#include "LNet.h"
#include "Services.h"
#include "BlockServices.h"
#include "X2C_Model.h"

/*******************************************************************************
 Macro and constants
*******************************************************************************/
/* LNet protocol buffersize and node identifier */
#define LNET_BUFFERSIZE ((uint8)255)
#define LNET_NODEID ((uint8)1)

/* Macro to kick the watchdog */
#define KICK_DOG ClrWdt()

/*******************************************************************************
 Private data types
*******************************************************************************/

/* Structure for compilation date and time */
const struct {
    uint8 date[11];
    uint8 time[8];
} compilationDate = { __DATE__, __TIME__ };

/*******************************************************************************
Private variables
*******************************************************************************/
/* Communication state machine for X2C model */

static tSerial interface;

static tLNet protocol;
static uint8 bufferLNet[LNET_BUFFERSIZE];

/*******************************************************************************
Interface variables
*******************************************************************************/
/* X2C maintenance structure */
volatile tTableStruct TheTableStruct;
volatile tTableStruct *TableStruct = &TheTableStruct;

/*******************************************************************************
Private Functions
*******************************************************************************/
/*! \brief Initialize version information for the X2C model.
 *
 * Details.
 * This function initializes version information for the X2C
 * model by setting the program version and compilation date/time.
 *
 * @param[in]: tblStruct   X2C maintenance structure
 * @param[in]: appVersion  Version number
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
static void initVersionInfo(volatile tTableStruct *tblStruct, uint16 appVersion)
{
    tblStruct->framePrgVersion = appVersion;
    tblStruct->framePrgCompDateTime = (uint8 *)&compilationDate;
}

/*******************************************************************************
 * Interface Functions
*******************************************************************************/

/*! \brief Initialize X2C and its components.
 *
 * Details.
 * This function initializes the integrated monitor, configures
 * the LNet protocol, sets up communication interfaces, and
 * prepares the X2C environment for operation. It also
 * initializes version information.
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
void X2C_Initialize(void)
{
    /*! \brief Initialize the integrated monitor
     *  - Configuration of the LNet protocol:
     *    - Node-ID: 1, Buffer size: 255
     */
    initTableStruct();
    initCommunication(&interface);
    initLNet(&protocol, bufferLNet, LNET_BUFFERSIZE, LNET_NODEID);
    linkCommunication((tProtocol *)&protocol, &interface);
    initServiceTable((tProtocol *)&protocol);
    addCoreServices((tProtocol *)&protocol);
    addBlockServices((tProtocol *)&protocol);
    addTableStructProtocol((tProtocol *)&protocol);
    TableStruct->DSPState = PRG_LOADED_STATE;

    initVersionInfo(TableStruct, (uint16)0x0001); /*! Software version 1 */
    TableStruct->TFncTable = blockFunctionTable;
    TableStruct->TParamTable = parameterIdTable;

    /*! Initialize X2C */
    X2C_Init();
}

/*! \brief Handle communication for the X2C model.
 *
 * Details.
 * This function handles communication for the X2C model. Depending on the communication
 * interface (e.g., UART or USB-CDC), it follows the communication state machine to
 * ensure proper initialization and operation of the communication protocol.
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
void X2C_Communicate(void)
{
    protocol.communicate((tProtocol *)&protocol);
}

/*! \brief Update the X2C model.
 *
 * Details.
 * This function updates the X2C model, allowing it to
 * perform its real-time control and monitoring functions.
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
void X2C_UpdateModel(void)
{
    /** Read input ports */
    X2C_InputPortsRead();

    /** Update model */
    X2C_Update();

    /** Write output ports */
    X2C_OutputPortsWrite();
}
