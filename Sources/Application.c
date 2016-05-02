/*
 * Application.c
 *
 *  Created on: 28.02.2016
 *      Author: Erich Styger
 */

#include "Platform.h"
#include "Application.h"
#include "Event.h"
#include "LED.h"
#include "WAIT1.h"
#include "CS1.h"
#include "Keys.h"
#include "CLS1.h"
#if PL_CONFIG_HAS_BUZZER
  #include "Buzzer.h"
#endif
#if PL_CONFIG_HAS_RTOS
  #include "FRTOS1.h"
  #include "RTOS.h"
#endif
#if PL_CONFIG_HAS_SHELL
  #include "Shell.h"
#endif

#if PL_CONFIG_HAS_EVENTS
void APP_EventHandler(EVNT_Handle event) {
  switch(event) {
  case EVNT_STARTUP:
    LED1_On(); /* just do something */
#if PL_CONFIG_HAS_BUZZER
    BUZ_PlayTune(BUZ_TUNE_WELCOME);
#endif
    break;
  case EVNT_LED_HEARTBEAT:
    LED1_Neg();
    break;
#if PL_CONFIG_HAS_KEYS
  #if PL_CONFIG_NOF_KEYS>=1
  case EVNT_SW1_PRESSED:
    LED2_Neg();
    //CLS1_SendStr("SW1 pressed\r\n", CLS1_GetStdio()->stdOut);
    SHELL_SendString("SW1 pressed\r\n");
#if PL_CONFIG_HAS_BUZZER
    BUZ_PlayTune(BUZ_TUNE_BUTTON);
#endif
    break;
  case EVNT_SW1_RELEASED:
    LED2_Neg();
    //CLS1_SendStr("SW1 released\r\n", CLS1_GetStdio()->stdOut);
    SHELL_SendString("SW1 released\r\n");
    break;
  case EVNT_SW1_LPRESSED:
    LED2_Neg();
    //CLS1_SendStr("SW1 long pressed\r\n", CLS1_GetStdio()->stdOut);
    SHELL_SendString("SW1 long pressed\r\n");
#if PL_CONFIG_HAS_BUZZER
    BUZ_PlayTune(BUZ_TUNE_BUTTON_LONG);
#endif
    break;
  #endif
  #if PL_CONFIG_NOF_KEYS>=2
  case EVNT_SW2_PRESSED:
    LED2_Neg();
    //CLS1_SendStr("SW2 pressed\r\n", CLS1_GetStdio()->stdOut);
    SHELL_SendString("SW2 pressed\r\n");
    break;
  #endif
  #if PL_CONFIG_NOF_KEYS>=3
  case EVNT_SW3_PRESSED:
    LED2_Neg();
    //CLS1_SendStr("SW3 pressed\r\n", CLS1_GetStdio()->stdOut);
    SHELL_SendString("SW3 pressed\r\n");
    break;
  #endif
  #if PL_CONFIG_NOF_KEYS>=4
  case EVNT_SW4_PRESSED:
    LED2_Neg();
    //CLS1_SendStr("SW4 pressed\r\n", CLS1_GetStdio()->stdOut);
    SHELL_SendString("SW4 pressed\r\n");
    break;
  #endif
  #if PL_CONFIG_NOF_KEYS>=5
  case EVNT_SW5_PRESSED:
    LED2_Neg();
    //CLS1_SendStr("SW5 pressed\r\n", CLS1_GetStdio()->stdOut);
    SHELL_SendString("SW5 pressed\r\n");
    break;
  #endif
  #if PL_CONFIG_NOF_KEYS>=6
  case EVNT_SW6_PRESSED:
    LED2_Neg();
    //CLS1_SendStr("SW6 pressed\r\n", CLS1_GetStdio()->stdOut);
    SHELL_SendString("SW6 pressed\r\n");
    break;
  #endif
  #if PL_CONFIG_NOF_KEYS>=7
  case EVNT_SW7_PRESSED:
    LED2_Neg();
    //CLS1_SendStr("SW7 pressed\r\n", CLS1_GetStdio()->stdOut);
    SHELL_SendString("SW7 pressed\r\n");
    break;
  #endif
#endif
  } /* switch */
}
#endif /* PL_CONFIG_HAS_EVENTS */

void APP_Start(void) {
#if 0 && PL_CONFIG_BOARD_IS_ROBO_V2 /* already done in keys.c */
  /* SW1: enable and turn on pull-up resistor for PTA14 (push button) */
  PORT_PDD_SetPinPullSelect(PORTA_BASE_PTR, 14, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTA_BASE_PTR, 14, PORT_PDD_PULL_ENABLE);
#endif
#if PL_CONFIG_BOARD_IS_ROBO_V2
  /* pull-ups for Quadrature Encoder Pins */
  PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 10, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 10, PORT_PDD_PULL_ENABLE);
  PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 11, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 11, PORT_PDD_PULL_ENABLE);
  PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 16, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 16, PORT_PDD_PULL_ENABLE);
  PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 17, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 17, PORT_PDD_PULL_ENABLE);
#endif
  PL_Init();
#if PL_CONFIG_HAS_EVENTS
  EVNT_SetEvent(EVNT_STARTUP);
#endif
#if CLS1_DEFAULT_SERIAL
  CLS1_SendStr("Hello World!\r\n", CLS1_GetStdio()->stdOut);
#endif
#if PL_CONFIG_HAS_RTOS
  vTaskStartScheduler(); /* start the RTOS, create the IDLE task and run my tasks (if any) */
  /* does usually not return! */
#else
  for(;;) {
#if PL_CONFIG_HAS_KEYS
    KEY_Scan();
#endif
#if PL_CONFIG_HAS_EVENTS
    EVNT_HandleEvent(APP_EventHandler, TRUE);
#endif
    WAIT1_Waitms(25); /* just wait for some arbitrary time .... */
  }
#endif
}


