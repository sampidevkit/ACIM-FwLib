/*
 * Copyright (c) 2013, Linz Center of Mechatronics GmbH (LCM)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 * conditions and the following disclaimer in the documentation and/or other materials provided
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

#ifndef __CONFIGX2C_H__
#define __CONFIGX2C_H__

/*******************************************************************************
 Header inclusions
*******************************************************************************/
#include "X2C.h"
#include "X2C_Communication.h"

/*******************************************************************************
 User defined data-structure
*******************************************************************************/

/*******************************************************************************
 Static functions
*******************************************************************************/
/*******************************************************************************
 Static Interface Functions
*******************************************************************************/

/*! \brief Read input ports
 *
 * Details.
 * Read input ports
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
static inline void X2C_InputPortsRead(void)
{

}

/*! \brief Write output ports
 *
 * Details.
 * Write output ports
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
static inline void X2C_OutputPortsWrite( void )
{

}

/*******************************************************************************
 * Interface functions
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
void X2C_Initialize(void);

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
void X2C_Communicate(void);

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
void X2C_UpdateModel(void);

#endif
