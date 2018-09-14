/*-----------------------------------------------------------------------------
 * TITLE: OEM_LED.H
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/

#ifndef OEM_LED_H
#define OEM_LED_H

typedef enum _LED_STATUSE {
  LEDOFF = 0,
  LEDON,  
  LEDDIMMING,
} LED_STATUSE ;
//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------
extern void OEM_Write_Leds(BYTE data_byte);
extern void Battery_LED_Reset(void);//MEILING030:add.
extern void Battery_LED_CONTROL(WORD LED_ON,BYTE LED_OFF);//MEILING030:add.
extern void Battery_Charge_Discharge_LED_CONTROL(WORD LED_ON,BYTE LED_OFF);//MEILING030:add.
extern void Lenovo_LED(void);
extern void MFG_LED(void);
extern void SetKeyboardLED(void);
#endif