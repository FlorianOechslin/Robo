/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : RNET1.h
**     Project     : Robo
**     Processor   : MK22FX512VLQ12
**     Component   : RNet
**     Version     : Component 01.083, Driver 01.00, CPU db: 3.00.000
**     Repository  : My Components
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-05-09, 11:10, # CodeGen: 13
**     Abstract    :
**          This components implements the RNet Stack.
**     Settings    :
**          Component name                                 : RNET1
**          Transceiver                                    : 
**            Transceiver Type                             : nRF24L01+
**            nRF24L01+                                    : Enabled
**              nRF24L01+                                  : RF1
**              Radio Channel                              : 70
**              Data Rate                                  : 1000 kBit
**              Payload Size                               : 32
**              Address                                    : 0x11, 0x22, 0x33, 0x44, 0x55
**            SMAC                                         : Disabled
**          Network                                        : 
**            Short Address Size                           : 8 Bits
**            Use ACK                                      : no
**          Queues                                         : 
**            Rx Message Queue Size                        : 6
**            Tx Message Queue Size                        : 6
**            Message Queue Blocking Time (ms)             : 200
**          Send Retry Count                               : 0
**          System                                         : 
**            Utility                                      : UTIL1
**            RTOS                                         : FRTOS1
**            Shell                                        : Enabled
**              Remote StdIO                               : Enabled
**                Queue length                             : 48
**                Queue Timeout (ms)                       : 500
**              Shell                                      : CLS1
**     Contents    :
**         SetChannel   - uint8_t RNET1_SetChannel(uint8_t channel);
**         Process      - uint8_t RNET1_Process(void);
**         PowerUp      - uint8_t RNET1_PowerUp(void);
**         ParseCommand - uint8_t RNET1_ParseCommand(const unsigned char *cmd, bool *handled, const...
**         Init         - void RNET1_Init(void);
**         Deinit       - void RNET1_Deinit(void);
**
**     License   : Open Source (MIT) (http://opensource.org/licenses/mit-license.html)
**     Copyright : Erich Styger, 2014, all rights reserved.
**     Web       : www.mcuoneclipse.com
**     Permission is hereby granted, free of charge, to any person obtaining a copy
**     of this software and associated documentation files (the "Software"), to deal
**     in the Software without restriction, including without limitation the rights
**     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
**     copies of the Software, and to permit persons to whom the Software is
**     furnished to do so, subject to the following conditions:
**     The above copyright notice and this permission notice shall be included in
**     all copies or substantial portions of the Software.
**     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
**     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
**     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
**     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
**     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
**     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
**     THE SOFTWARE.
** ###################################################################*/
/*!
** @file RNET1.h
** @version 01.00
** @brief
**          This components implements the RNet Stack.
*/         
/*!
**  @addtogroup RNET1_module RNET1 module documentation
**  @{
*/         

#ifndef __RNET1_H
#define __RNET1_H

/* MODULE RNET1. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */
#include "RF1.h"
#include "UTIL1.h"
#include "FRTOS1.h"
#include "CLS1.h"

#include "Cpu.h"


#define RNET1_PARSE_COMMAND_ENABLED  1  /* set to 1 if method ParseCommand() is present, 0 otherwise */

typedef enum {
  RNET1_RADIO_MSG_RECEIVED,        /* message has been received */
  RNET1_RADIO_MSG_SENT,            /* message has been sent */
  RNET1_RADIO_TIMEOUT,             /* timeout, no response received */
  RNET1_RADIO_RETRY,               /* retry, sending message again */
  RNET1_RADIO_RETRY_MSG_FAILED,    /* creating retry message failed */
  RNET1_RADIO_ACK_RECEIVED         /* acknowledge message received */
} RNET1_RadioEvent;

#define RNET1_CREATE_EVENTS   1  /* call user event handler */

void RF1_OnInterrupt(void);

void RNET1_Init(void);
/*
** ===================================================================
**     Method      :  RNET1_Init (component RNet)
**     Description :
**         Initializes the RNet Stack
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void RNET1_Deinit(void);
/*
** ===================================================================
**     Method      :  RNET1_Deinit (component RNet)
**     Description :
**         Deinitializes the RNet Stack
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

uint8_t RNET1_Process(void);
/*
** ===================================================================
**     Method      :  RNET1_Process (component RNet)
**     Description :
**         Processes the Radio Rx and Tx messages
**     Parameters  : None
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t RNET1_PowerUp(void);
/*
** ===================================================================
**     Method      :  RNET1_PowerUp (component RNet)
**     Description :
**         Initializes and powers the radio up.
**     Parameters  : None
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t RNET1_SetChannel(uint8_t channel);
/*
** ===================================================================
**     Method      :  RNET1_SetChannel (component RNet)
**     Description :
**         Sets the radio channel
**     Parameters  :
**         NAME            - DESCRIPTION
**         channel         - Channel number
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t RNET1_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io);
/*
** ===================================================================
**     Method      :  RNET1_ParseCommand (component RNet)
**     Description :
**         Shell Command Line parser. This method is enabled/disabled
**         depending on if you have the Shell enabled/disabled in the
**         properties.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * cmd             - Pointer to command string
**       * handled         - Pointer to variable which tells if
**                           the command has been handled or not
**       * io              - Pointer to I/O structure
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

/* END RNET1. */

#endif
/* ifndef __RNET1_H */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
