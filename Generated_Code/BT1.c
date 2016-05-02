/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : BT1.c
**     Project     : Robo
**     Processor   : MK22FX512VLQ12
**     Component   : Bluetooth_EGBT
**     Version     : Component 01.051, Driver 01.00, CPU db: 3.00.000
**     Repository  : My Components
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-04-26, 08:42, # CodeGen: 5
**     Abstract    :
**          Driver for e-Gizmo Bluetooth module
**     Comment     :
**         Bluetooth module on the sensor shield
**     Settings    :
**          Component name                                 : BT1
**          Firmware                                       : HC06
**          Response timeout (ms)                          : 850
**          CMD Pin                                        : Disabled
**          State Pin                                      : Disabled
**          Rx Pull-Up                                     : Enabled
**            Pin Name                                     : E
**            Pin Number                                   : 1
**          Shell                                          : Enabled
**            Shell                                        : CLS1
**          Interfaces                                     : 
**            Utility                                      : UTIL1
**            Wait                                         : WAIT1
**            Serial                                       : Serial
**     Contents    :
**         Init                      - void BT1_Init(void);
**         Deinit                    - void BT1_Deinit(void);
**         GetLastError              - BT1_TEnumErrors BT1_GetLastError(void);
**         GetErrorString            - BT1_TConstStringPtr BT1_GetErrorString(BT1_TEnumErrors errorNo);
**         SendATCommand             - byte BT1_SendATCommand(byte *cmd, byte *rxBuf, size_t rxBufSize, byte...
**         StdCmd                    - byte BT1_StdCmd(byte *cmd, byte *rxBuf, size_t rxBufSize, byte...
**         StdOKCmd                  - byte BT1_StdOKCmd(byte *cmd);
**         QueryString               - byte BT1_QueryString(byte *cmd, byte *expectedResponse, byte *string, size_t...
**         btTestUART                - byte BT1_btTestUART(void);
**         btQueryFirmwareVersionStr - byte BT1_btQueryFirmwareVersionStr(byte *buf, size_t bufSize);
**         btSetDeviceName           - byte BT1_btSetDeviceName(byte *name);
**         btSetPairingPasskey       - byte BT1_btSetPairingPasskey(byte *pwd);
**         btSetBaud                 - byte BT1_btSetBaud(dword baud);
**         ParseCommand              - byte BT1_ParseCommand(const unsigned char* cmd, bool *handled, const...
**         RecvChar                  - byte BT1_RecvChar(byte *ch);
**         SendChar                  - byte BT1_SendChar(byte ch);
**         GetCharsInRxBuf           - word BT1_GetCharsInRxBuf(void);
**         StdIOKeyPressed           - bool BT1_StdIOKeyPressed(void);
**         StdIOReadChar             - void BT1_StdIOReadChar(byte *c);
**         StdIOSendChar             - void BT1_StdIOSendChar(byte ch);
**
**     License   :  Open Source (LGPL)
**     Copyright : (c) Copyright Erich Styger, 2013-2014, all rights reserved. BC04 contribution by Mihai Surdeanu.
**     Web: http://mcuoneclipse.com
**     This an open source software implementing an LED driver using Processor Expert.
**     This is a free software and is opened for education, research and commercial developments under license policy of following terms:
**     * This is a free software and there is NO WARRANTY.
**     * No restriction on use. You can use, modify and redistribute it for personal, non-profit or commercial product UNDER YOUR RESPONSIBILITY.
**     * Redistributions of source code must retain the above copyright notice.
** ###################################################################*/
/*!
** @file BT1.c
** @version 01.00
** @brief
**          Driver for e-Gizmo Bluetooth module
*/         
/*!
**  @addtogroup BT1_module BT1 module documentation
**  @{
*/         

/* MODULE BT1. */

#include "BT1.h"

/* No cmd pin configured */
#define BT1_SET_CMD_MODE()                          /* do nothing */
#define BT1_SET_DATA_MODE()                         /* do nothing */
#define BT1_CLR_RX_BUF()                            (void)Serial1_ClearRxBuf() /* clear input buffer */

static BT1_TEnumErrors BT1_lastErr = BT1_NO_ERROR; /* holds the last error */

static const unsigned char *BT1_ErrorStringTable[] = {
  /*  0 */ (unsigned char*)"Command Error/Invalid Command",
  /*  1 */ (unsigned char*)"Results in default value",
  /*  2 */ (unsigned char*)"PSKEY write error",
  /*  3 */ (unsigned char*)"Device name too long (>32 characters)",
  /*  4 */ (unsigned char*)"No device name specified (0 length)",
  /*  5 */ (unsigned char*)"Bluetooth address NAP is too long",
  /*  6 */ (unsigned char*)"Bluetooth address UAP is too long",
  /*  7 */ (unsigned char*)"Bluetooth address LAP is too long",
  /*  8 */ (unsigned char*)"PIO map not specified (0 length)",
  /*  9 */ (unsigned char*)"Invalid PIO port number entered",
  /*  A */ (unsigned char*)"Device Class is not specified (0 length)",
  /*  B */ (unsigned char*)"Device Class too long",
  /*  C */ (unsigned char*)"Inquire Access Code not specified (0 length)",
  /*  D */ (unsigned char*)"Inquire Access Code too long",
  /*  E */ (unsigned char*)"Invalid Inquire Access code entered",
  /*  F */ (unsigned char*)"Pairing Password not specified (0 length)",
  /* 10 */ (unsigned char*)"Pairing Password too long (>16 characters)",
  /* 11 */ (unsigned char*)"Invalid Role entered",
  /* 12 */ (unsigned char*)"Invalid Baud Rate entered",
  /* 13 */ (unsigned char*)"Invalid Stop Bit entered",
  /* 14 */ (unsigned char*)"Invalid Parity Bit entered",
  /* 15 */ (unsigned char*)"No device in Pairing List",
  /* 16 */ (unsigned char*)"SPP not initialized",
  /* 17 */ (unsigned char*)"SPP already initialized",
  /* 18 */ (unsigned char*)"Invalid Inquiry Mode",
  /* 19 */ (unsigned char*)"Inquire Timeout occurred",
  /* 1A */ (unsigned char*)"Invalid/zero length address entered",
  /* 1B */ (unsigned char*)"Invalid Security Mode entered",
  /* 1C */ (unsigned char*)"Invalid Encryption Mode entered",
  /* 1D */ (unsigned char*)"No Error",
};

static bool isRxError(unsigned char *buf, BT1_TEnumErrors *errno)
{
  /* return true for "ERROR:(1D)\r\n" at the end of the buffer and returns the error */
  size_t idx;
  unsigned char ch_low, ch_high;
  const unsigned char *p;
  uint8_t val;
  unsigned char hexBuf[sizeof("0x1F")];
  #define _IS_ERR_NUMBER(ch) ((ch>='0' && ch<='9') || (ch>='A' && ch<='D')) /* 0..1D */

  idx = UTIL1_strlen((char*)buf);
  if (idx >= sizeof("ERROR(0)\r\n")) {
    if (UTIL1_strtailcmp(buf, (unsigned char*)")\r\n") == 0) {
      if (UTIL1_strncmp((char*)&buf[idx - sizeof("ERROR(0)\r\n") - 1], (char*)"ERROR(", sizeof("ERROR(")) == 0) {
        ch_high = '0';
        ch_low = buf[idx-sizeof("0)\r\n")-1];
      } else if (UTIL1_strncmp((char*)&buf[idx - sizeof("ERROR(00)\r\n") - 1], (char*)"ERROR(", sizeof("ERROR(")) == 0) {
        ch_high = buf[idx - sizeof("00)\r\n") - 1];
        ch_low = buf[idx - sizeof("0)\r\n") - 1];
      } else { /* initialize in any case */
        ch_high = '\0';
        ch_low = '\0';
      }
      if (_IS_ERR_NUMBER(ch_high) && _IS_ERR_NUMBER(ch_low)) {
        UTIL1_strcpy(hexBuf, sizeof(hexBuf), (unsigned char*)"0x");
        UTIL1_chcat(hexBuf, sizeof(hexBuf), ch_high);
        UTIL1_chcat(hexBuf, sizeof(hexBuf), ch_low);
        p = &hexBuf[0];
        if (UTIL1_ScanHex8uNumber(&p, &val) == ERR_OK) {
          *errno = (BT1_TEnumErrors)val;
          return TRUE; /* bingo! */
        }
      }
    }
  }
  return FALSE; /* not an error */
}

static uint8_t RxResponse(unsigned char *rxBuf, size_t rxBufLength, uint16_t msTimeout, unsigned char *expectedTail)
{
  unsigned char ch;
  uint8_t res = ERR_OK;
  unsigned char *p;

  if (rxBufLength < sizeof("x\r\n")) {
    return ERR_OVERFLOW; /* not enough space in buffer */
  }
  p = rxBuf;
  p[0] = '\0';
  for(;;) { /* breaks */
    if (msTimeout == 0) {
      break; /* will decide outside of loop if it is a timeout. */
    } else if (rxBufLength == 0) {
      res = ERR_OVERFLOW; /* not enough space in buffer */
      break;
    } else if (Serial1_GetCharsInRxBuf() > 0) {
      if (Serial1_RecvChar(&ch) != ERR_OK) {
        res = ERR_RXEMPTY;
        break;
      }
      *p++ = ch;
      *p = '\0'; /* always terminate */
      rxBufLength--;
    } else if (expectedTail[0] != '\0'
    			&& UTIL1_strtailcmp(rxBuf, expectedTail) == 0) {
      break; /* finished */
    } else if (isRxError(rxBuf, &BT1_lastErr)) {
      break; /* error */
    } else {
      WAIT1_WaitOSms(1);
      msTimeout--;
    }
  } /* for */
  if (msTimeout==0) { /* timeout! */
    if (expectedTail[0] != '\0' /* timeout, and we expected something: an error for sure */
        || rxBuf[0] == '\0' /* timeout, did not know what to expect, but received nothing? There has to be a response. */
       )
    {
      res = ERR_FAULT;
    }
  }
  return res;
}

static uint8_t ExecuteQueryString(uint8_t(*f)(unsigned char*,size_t), unsigned char *buf, size_t bufSize, unsigned char *appendStr)
{
  byte status = f(buf, bufSize);
  if (status == ERR_OK) {
    UTIL1_strcat(buf, bufSize, (unsigned char*)appendStr);
    return ERR_OK;
  } else {
    UTIL1_strcpy(buf, bufSize, (unsigned char*)"ERROR");
    UTIL1_strcat(buf, bufSize, (unsigned char*)appendStr);
    return ERR_FAILED;
  }
}

static uint8_t PrintShortStatus(const CLS1_StdIOType *io)
{
  uint8_t res = ERR_OK;

  CLS1_SendStatusStr((unsigned char*)"BT1", (unsigned char*)"\r\n", io->stdOut);
  if (BT1_btTestUART()!=ERR_OK) {
    CLS1_SendStatusStr((unsigned char*)"  Test UART", (unsigned char*)"FAILED", io->stdOut);
    /* even if it failed, this is most likely because we are in transparent mode, so that would be ok */
  } else {
    CLS1_SendStatusStr((unsigned char*)"  Test UART", (unsigned char*)"OK", io->stdOut);
  }
  CLS1_SendStr((unsigned char*)" (use 'BT1 status' for full status)\r\n", io->stdOut);
  return res;
}

static uint8_t PrintLongStatus(const CLS1_StdIOType *io)
{
  uint8_t buf[BT1_MAX_RESPONSE_SIZE];

  CLS1_SendStatusStr((unsigned char*)"BT1", (unsigned char*)"\r\n", io->stdOut);

  if (BT1_btTestUART() != ERR_OK) {
    CLS1_SendStatusStr((unsigned char*)"  Test UART", (unsigned char*)"FAILED\r\n", io->stdOut);
    return ERR_FAILED;
  } else {
    CLS1_SendStatusStr((unsigned char*)"  Test UART", (unsigned char*)"OK\r\n", io->stdOut);
  }

  (void)ExecuteQueryString(BT1_btQueryFirmwareVersionStr, buf, sizeof(buf), (unsigned char*)"\r\n");
  CLS1_SendStatusStr((unsigned char*)"  Firmware", buf, io->stdOut);

  return ERR_OK;
}

static uint8_t PrintHelp(const CLS1_StdIOType *io)
{
  CLS1_SendHelpStr((unsigned char*)"BT1", (unsigned char*)"Group of BT1 commands\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Print help or status information\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  pin <pin>", (unsigned char*)"Set pairing pin, e.g. 'BT1 pin 1234'\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  name <name>", (unsigned char*)"Set device name e.g. 'BT1 name Blue'\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  baud <baud>", (unsigned char*)"Set baud (1200, 2400, 4800, ... 115200), e.g. 'BT1 baud 9600'\r\n", io->stdOut);
#ifdef BT1_FIRMWARE_HC05 /* HC06 does not need \r\n */
  CLS1_SendHelpStr((unsigned char*)"  cmd <AT command>", (unsigned char*)"Send an AT command, e.g AT+NAME?\r\n", io->stdOut);
#else
  CLS1_SendHelpStr((unsigned char*)"  cmd <AT command>", (unsigned char*)"Send an AT command, e.g AT+VERSION\r\n", io->stdOut);
#endif
  CLS1_SendHelpStr((unsigned char*)"  send <string>", (unsigned char*)"Send a string to the Bluetooth UART bridge\r\n", io->stdOut);
  return ERR_OK;
}

static uint8_t SendCommand(const unsigned char *cmd, const CLS1_StdIOType *io)
{
  unsigned char buf[BT1_MAX_RESPONSE_SIZE];
  unsigned char cmdBuf[BT1_MAX_AT_CMD_SIZE];
  uint8_t res = ERR_OK;

  UTIL1_strcpy(cmdBuf, sizeof(cmdBuf), cmd);
  buf[0] = '\0';
  if (BT1_SendATCommand(cmdBuf, buf, sizeof(buf), (unsigned char*)"") != ERR_OK) {
    CLS1_SendStr((unsigned char*)"***Response not OK\r\n", io->stdOut);
    res = ERR_FAILED;
  }
  CLS1_SendStr(buf, io->stdOut);
  /* HC06 does not respond with OK\r\n at the end */
  CLS1_SendStr((unsigned char*)"\r\n", io->stdOut);
  return res;
}

/*
** ===================================================================
**     Method      :  BT1_SendATCommand (component Bluetooth_EGBT)
**     Description :
**         Sends an AT command string (with the AT in it) to the module.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * cmd             - Pointer to AT command string with the AT
**                           in it
**       * rxBuf           - Pointer to buffer for the response
**                           from the device
**         rxBufSize       - size of the receiving buffer
**                           in bytes
**       * expectedTailStr - Pointer to the
**                           function will read from the device until
**                           the expected tail string is read. Typically
**                           this is "OK" terminated by backslash r and
**                           backslash n
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte BT1_SendATCommand(byte *cmd, byte *rxBuf, size_t rxBufSize, byte *expectedTailStr)
{
  word snt;
  byte res;

  BT1_SET_CMD_MODE();
  BT1_CLR_RX_BUF();
  if (Serial1_SendBlock(cmd, (word)UTIL1_strlen((char*)cmd), &snt) != ERR_OK) {
    return ERR_FAILED;
  }
  res = RxResponse(rxBuf, rxBufSize, BT1_TIMEOUT_MS, expectedTailStr);
  BT1_SET_DATA_MODE();
  return res;
}

/*
** ===================================================================
**     Method      :  BT1_StdCmd (component Bluetooth_EGBT)
**     Description :
**         Send standard command: "<cmd>\r
**         " ==> "<expectedResponse>"
**     Parameters  :
**         NAME            - DESCRIPTION
**       * cmd             - Pointer to command string
**       * rxBuf           - Pointer to buffer for the answer
**         rxBufSize       - 
**       * expectedResponse - Pointer to
**                           expected response
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte BT1_StdCmd(byte *cmd, byte *rxBuf, size_t rxBufSize, byte *expectedResponse)
{
  /* Send standard command: "<cmd>\r\n" ==> "<expectedResponse>" */
  uint8_t res = ERR_OK;
  unsigned char cmdBuf[BT1_MAX_AT_CMD_SIZE];

  UTIL1_strcpy(cmdBuf, sizeof(cmdBuf), cmd);
  for(;;) { /* breaks */
    if (BT1_SendATCommand(cmdBuf, rxBuf, rxBufSize, expectedResponse) != ERR_OK) {
      res = ERR_FAILED;
      break;
    }
    if (UTIL1_strcmp((char*)rxBuf, (char*)expectedResponse) != 0) {
      res = ERR_FAILED;
      break;
    }
    break; /* exit for(;;) */
  } /* for */
  return res;
}

/*
** ===================================================================
**     Method      :  BT1_StdOKCmd (component Bluetooth_EGBT)
**     Description :
**         Send standard command: "<cmd>\r
**         " ==> "OK\r
**         "
**     Parameters  :
**         NAME            - DESCRIPTION
**       * cmd             - Pointer to command string
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte BT1_StdOKCmd(byte *cmd)
{
  /* Send standard command: "<cmd>\r\n" ==> "OK\r\n" */
  unsigned char rxBuf[sizeof("FAIL\r\n")+1]; /* could be FAIL or OK */

  /* HC06 does not have \r\n line ending */
  return BT1_StdCmd(cmd, rxBuf, sizeof(rxBuf), (unsigned char*)"OK");
}

/*
** ===================================================================
**     Method      :  BT1_QueryString (component Bluetooth_EGBT)
**     Description :
**         get string from device: "<cmd>\r
**         " ==> "<response><string>\r
**         OK\r
**         "
**     Parameters  :
**         NAME            - DESCRIPTION
**       * cmd             - Pointer to command string
**       * expectedResponse - Pointer to
**                           expected response string
**       * string          - Pointer to buffer where to store the
**                           string value
**         stringSize      - 
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte BT1_QueryString(byte *cmd, byte *expectedResponse, byte *string, size_t stringSize)
{
  /* get string: "<cmd>\r\n" ==> "<expectedResponse><string>\r\nOK\r\n" */
  /* example get string: "AT+RNAME?\r\n" ==> "+NAME:<name>\r\nOK\r\n"
    cmd: "AT+RNAME?\r\n"
    expected response: "+NAME:"
  */
  uint8_t res = ERR_OK;
  unsigned char rxBuf[BT1_MAX_RESPONSE_SIZE];
  unsigned char cmdBuf[BT1_MAX_AT_CMD_SIZE];
  size_t responseLen;

  responseLen = UTIL1_strlen((char*)expectedResponse);
  string[0] = '\0'; /* initialize buffer */
  UTIL1_strcpy(cmdBuf, sizeof(cmdBuf), cmd);
  for(;;) { /* breaks */
    if (BT1_SendATCommand(cmdBuf, rxBuf, sizeof(rxBuf), (unsigned char*)"") != ERR_OK) {
      res = ERR_FAILED;
      break;
    }
    if (UTIL1_strncmp((char*)rxBuf, (char*)expectedResponse, responseLen) != 0) {
      res = ERR_FAILED;
      break;
    }
    UTIL1_strcpy(string, stringSize, &rxBuf[responseLen]); /* store name */
    break; /* exit for(;;) */
  } /* for */
  return res;
}

/*
** ===================================================================
**     Method      :  BT1_GetLastError (component Bluetooth_EGBT)
**     Description :
**         Returns the last error reported by the device.
**     Parameters  : None
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
BT1_TEnumErrors BT1_GetLastError(void)
{
  BT1_TEnumErrors err = BT1_lastErr;

  BT1_lastErr = BT1_NO_ERROR; /* reset error */
  return err;
}

/*
** ===================================================================
**     Method      :  BT1_GetErrorString (component Bluetooth_EGBT)
**     Description :
**         Returns for a given error code the error string describing
**         the problem.
**     Parameters  :
**         NAME            - DESCRIPTION
**         errorNo         - 
**     Returns     :
**         ---             - Error string
** ===================================================================
*/
BT1_TConstStringPtr BT1_GetErrorString(BT1_TEnumErrors errorNo)
{
  if (errorNo>BT1_NO_ERROR) {
    return (unsigned char*)"***illegal error number";
  }
  return BT1_ErrorStringTable[errorNo];
}

/*
** ===================================================================
**     Method      :  BT1_Init (component Bluetooth_EGBT)
**     Description :
**         Initializes the module
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#include "PORT_PDD.h" /* include PDD macro header file */

#define BT1_RX_PULLUP_PIN    1                             /* pin number */
#define BT1_RX_PULLUP_PORT   PORT##E##_BASE_PTR            /* port name */

void BT1_Init(void)
{
  BT1_lastErr = BT1_NO_ERROR; /* reset error */
  /* Enable internal pull up for Rx (microcontroller side), see http://mcuoneclipse.com/2014/03/30/getting-bluetooth-working-with-jy-mcu-bt_board-v1-06/ */
  PORT_PDD_SetPinPullSelect(BT1_RX_PULLUP_PORT, BT1_RX_PULLUP_PIN, PORT_PDD_PULL_UP); /* configure for pull-up */
  PORT_PDD_SetPinPullEnable(BT1_RX_PULLUP_PORT, BT1_RX_PULLUP_PIN, PORT_PDD_PULL_ENABLE); /* enable pull-up */
}

/*
** ===================================================================
**     Method      :  BT1_Deinit (component Bluetooth_EGBT)
**     Description :
**         Deinitializes the module
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void BT1_Deinit(void)
{
  /* nothing to do */
}

/*
** ===================================================================
**     Method      :  BT1_btTestUART (component Bluetooth_EGBT)
**     Description :
**         Tests the UAR connection with AT
**     Parameters  : None
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte BT1_btTestUART(void)
{
#ifdef BT1_FIRMWARE_HC05
  /* 1: Test UART connection: "AT\r\n" ==> "OK\r\n" */
#else /* HC06 */
  /* 1: Test UART connection: "AT" ==> "OK" */
#endif
  return BT1_StdOKCmd((unsigned char*)"AT");
}

/*
** ===================================================================
**     Method      :  BT1_btQueryFirmwareVersionStr (component Bluetooth_EGBT)
**     Description :
**         Queries the firmware version
**     Parameters  :
**         NAME            - DESCRIPTION
**       * buf             - Pointer to buffer where to store the string
**         bufSize         - Size of the buffer in bytes.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte BT1_btQueryFirmwareVersionStr(byte *buf, size_t bufSize)
{
  /* 3: Query firmware version: "AT+VERSION" ==> "OKlinvor1.6" */
  return BT1_QueryString((unsigned char*)"AT+VERSION", (unsigned char*)"OK", buf, bufSize);
}

/*
** ===================================================================
**     Method      :  BT1_btSetDeviceName (component Bluetooth_EGBT)
**     Description :
**         Sets the device name
**     Parameters  :
**         NAME            - DESCRIPTION
**       * name            - Pointer to new device name
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte BT1_btSetDeviceName(byte *name)
{
  /* 6: Set name: "AT+NAME<name>" ==> "OKsetname" */
  unsigned char buf[sizeof("AT+NAME")+BT1_MAX_DEVICE_NAME_SIZE];
  unsigned char rxBuf[sizeof("OKsetname")+1];

  if (UTIL1_strlen((char*)name)>BT1_MAX_DEVICE_NAME_SIZE) {
    return ERR_FAILED; /* name too long */
  }
  UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"AT+NAME");
  UTIL1_strcat(buf, sizeof(buf), name);
  return BT1_StdCmd(buf, rxBuf, sizeof(rxBuf), (unsigned char*)"OKsetname");
}

/*
** ===================================================================
**     Method      :  BT1_btSetPairingPasskey (component Bluetooth_EGBT)
**     Description :
**         Sets the pairing passkey
**     Parameters  :
**         NAME            - DESCRIPTION
**       * pwd             - Pointer to new pairing passkey
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte BT1_btSetPairingPasskey(byte *pwd)
{
  /* 12: Set paring pass key: "AT+PIN<pwd>" ==> "OKsetPIN" */
  unsigned char buf[sizeof("AT+PIN1234")];
  unsigned char rxBuf[sizeof("OKsetPIN")];

  if (UTIL1_strlen((char*)pwd)>4) { /* check for password size */
    return ERR_FAILED;
  }
  UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"AT+PIN");
  UTIL1_strcat(buf, sizeof(buf), pwd);
  return BT1_StdCmd(buf, rxBuf, sizeof(rxBuf), (unsigned char*)"OKsetPIN");
}

/*
** ===================================================================
**     Method      :  BT1_btSetBaud (component Bluetooth_EGBT)
**     Description :
**         Sets the baud rate of the device.
**     Parameters  :
**         NAME            - DESCRIPTION
**         baud            - 1200, 2400, 4800, 9600, 19200, 38400,
**                           57600, 115200
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte BT1_btSetBaud(dword baud)
{
  unsigned char *response;
  unsigned char buf[sizeof("AT+BAUDx")];
  unsigned char rxBuf[sizeof("OK115200")];
  switch(baud) {
    case 1200:
      UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"AT+BAUD1");
      response = (unsigned char*)"OK1200";
      break;
    case 2400:
      UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"AT+BAUD2");
      response = (unsigned char*)"OK2400";
      break;
    case 4800:
      UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"AT+BAUD3");
      response = (unsigned char*)"OK4800";
      break;
    case 9600:
      UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"AT+BAUD4");
      response = (unsigned char*)"OK9600";
      break;
    case 19200:
      UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"AT+BAUD5");
      response = (unsigned char*)"OK19200";
      break;
    case 38400:
      UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"AT+BAUD6");
      response = (unsigned char*)"OK38400";
      break;
    case 57600:
      UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"AT+BAUD7");
      response = (unsigned char*)"OK57600";
      break;
    case 115200:
      UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"AT+BAUD8");
      response = (unsigned char*)"OK115200";
      break;
#if 0 /* intentionally disabled, as using these baud might prevent you to connect with an normal PC */
    case 230400:
      UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"AT+BAUD9");
      response = (unsigned char*)"OK230400";
      break;
    case 460800:
      UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"AT+BAUDA");
      response = (unsigned char*)"OK460800";
      break;
    case 921600:
      UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"AT+BAUDB");
      response = (unsigned char*)"OK921600";
      break;
    case 1382400:
      UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"AT+BAUDC");
      response = (unsigned char*)"OK1382400";
      break;
#endif
    default:
      return ERR_FAILED;
  } /* switch */
  return BT1_StdCmd(buf, rxBuf, sizeof(rxBuf), response);
}

/*
** ===================================================================
**     Method      :  BT1_ParseCommand (component Bluetooth_EGBT)
**     Description :
**         Shell Command Line parser. Method is only available if Shell
**         is enabled in the component properties.
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
byte BT1_ParseCommand(const unsigned char* cmd, bool *handled, const CLS1_StdIOType *io)
{
  byte res = ERR_OK;

  if (UTIL1_strcmp((char*)cmd, CLS1_CMD_HELP) == 0
    || UTIL1_strcmp((char*)cmd, "BT1 help") == 0) {
    *handled = TRUE;
    return PrintHelp(io);
  } else if (UTIL1_strcmp((char*)cmd, CLS1_CMD_STATUS) == 0) {
    *handled = TRUE;
    res = PrintShortStatus(io); /* only print short status, as full status will fail if module is connected */
  } else if ((UTIL1_strcmp((char*)cmd, "BT1 status") == 0)) {
    *handled = TRUE;
    res = PrintLongStatus(io);
  } else if (UTIL1_strncmp((char*)cmd, "BT1 pin ", sizeof("BT1 pin ") - 1) == 0) {
    *handled = TRUE;
    res = BT1_btSetPairingPasskey((unsigned char*)cmd + sizeof("BT1 pin ") - 1);
  } else if (UTIL1_strncmp((char*)cmd, "BT1 name ", sizeof("BT1 name ") - 1) == 0) {
    *handled = TRUE;
    res = BT1_btSetDeviceName((unsigned char*)cmd + sizeof("BT1 name ") - 1);
  } else if (UTIL1_strncmp((char*)cmd, "BT1 baud ", sizeof("BT1 baud ") - 1) == 0) {
    const unsigned char *p;
    uint32_t value;

    *handled = TRUE;
    p = cmd+sizeof("BT1 baud ") - 1;
    res = UTIL1_ScanDecimal32uNumber(&p, &value);
    if (res!=ERR_OK) {
      CLS1_SendStr((unsigned char*)"*** invalid number!\r\n", io->stdErr);
      res = ERR_FAILED;
    } else {
      res = BT1_btSetBaud(value);
    }
  } else if (UTIL1_strncmp((char*)cmd, "BT1 cmd ", sizeof("BT1 cmd ") - 1) == 0) {
    *handled = TRUE;
    res = SendCommand(cmd + sizeof("BT1 cmd ") - 1, io);
  } else if (UTIL1_strncmp((char*)cmd, "BT1 send ", sizeof("BT1 send ") - 1) == 0) {
    word strSize, snt;
    const unsigned char *p;

    *handled = TRUE;
    p = cmd+sizeof("BT1 send ")-1;
    strSize = (word)UTIL1_strlen((char*)p);
    if (Serial1_SendBlock((unsigned char*)p, strSize, &snt) != ERR_OK) {
      CLS1_SendStr((unsigned char*)"FAILED sending string\r\n", io->stdErr);
      res = ERR_FAILED;
    }
  }
  return res;
}

/*
** ===================================================================
**     Method      :  BT1_RecvChar (component Bluetooth_EGBT)
**     Description :
**         Receives a character from the Bluetooth serial bridge
**     Parameters  :
**         NAME            - DESCRIPTION
**       * ch              - Pointer to character buffer
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
/*
byte BT1_RecvChar(byte *ch)
{
  *** Implemented as macro in the header file BT1.h
}
*/

/*
** ===================================================================
**     Method      :  BT1_SendChar (component Bluetooth_EGBT)
**     Description :
**         Sends a character to the Bluetooth serial bridge
**     Parameters  :
**         NAME            - DESCRIPTION
**         ch              - Character to be sent
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
/*
byte BT1_SendChar(byte ch)
{
  *** Implemented as macro in the header file BT1.h
}
*/

/*
** ===================================================================
**     Method      :  BT1_GetCharsInRxBuf (component Bluetooth_EGBT)
**     Description :
**         Returns the numbers of character in the receiver buffer.
**     Parameters  : None
**     Returns     :
**         ---             - Number of characters.
** ===================================================================
*/
/*
word BT1_GetCharsInRxBuf(void)
{
  *** Implemented as macro in the header file BT1.h
}
*/

/*
** ===================================================================
**     Method      :  BT1_StdIOKeyPressed (component Bluetooth_EGBT)
**     Description :
**         StdIO handler for Shell
**     Parameters  : None
**     Returns     :
**         ---             - True if there are characters in the
**                           input/RX buffer
** ===================================================================
*/
bool BT1_StdIOKeyPressed(void)
{
  return (bool)((BT1_GetCharsInRxBuf()==0U) ? FALSE : TRUE); /* true if there are characters in receive buffer */
}

/*
** ===================================================================
**     Method      :  BT1_StdIOReadChar (component Bluetooth_EGBT)
**     Description :
**         StdIO Handler for reading a character. It reads a zero byte
**         if there is no character.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * c               - Pointer to result character
**     Returns     : Nothing
** ===================================================================
*/
void BT1_StdIOReadChar(byte *c)
{
  if (BT1_RecvChar((uint8_t *)c) != ERR_OK) {
    /* failed to receive character: return a zero character */
    *c = '\0';
  }
}

/*
** ===================================================================
**     Method      :  BT1_StdIOSendChar (component Bluetooth_EGBT)
**     Description :
**         StdIO handler to sends a character 
**     Parameters  :
**         NAME            - DESCRIPTION
**         ch              - character to send
**     Returns     : Nothing
** ===================================================================
*/
void BT1_StdIOSendChar(byte ch)
{
  while (BT1_SendChar((uint8_t)ch)==ERR_TXFULL){} /* Send char */
}

/* END BT1. */

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
