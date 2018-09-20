/*-----------------------------------------------------------------------------
 * TITLE: OEM_LED.C
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

/* ----------------------------------------------------------------------------
 * FUNCTION:   OEM_Write_Leds
 *
 * Write to SCROLL LOCK, NUM LOCK, CAPS LOCK, and any custom LEDs (including
 * an "Fn" key LED).
 *
 * Input: data (LED_ON bit) = Data to write to the LEDs
 *         bit 0 - SCROLL, Scroll Lock
 *         bit 1 - NUM,    Num Lock
 *         bit 2 - CAPS,   Caps Lock
 *         bit 3 - OVL,    Fn LED
 *         bit 4 - LED4,   Undefined
 *         bit 5 - LED5,   Undefined
 *         bit 6 - LED6,   Undefined
 *         bit 7 - LED7,   Undefined
 *
 * Updates Led_Data variable and calls send_leds if appropriate.
 * ------------------------------------------------------------------------- */
void OEM_Write_Leds(BYTE data_byte)
{
    Led_Data = data_byte;
	//if ( uMBID & 0x40 )
    //if ( uMBID & 0x01 ) 	// Check 15"G52:modify board id for MFG  from 0x04 to 0x01 follow new ec common spec
    //if( ( uMBID & 0x03 == 0x01 ) ||( uMBID & 0x03 == 0x02 ) )	//ANGELAS008: Modidy board ID for check 15" and 17"
    if ( uMBID & 0x01 ) 	// Check 15"G52:modify board id for MFG  from 0x04 to 0x01 follow new ec common spec //ANGELAS009 remove ANGELAS008
    {
	    if ( data_byte & BIT1 )
	    {
		   	Hook_NUMLED_ON();
			SET_MASK(SYS_MISC1, b1Num_LED);
	    }
	    else
	    {
			Hook_NUMLED_OFF();
			CLR_MASK(SYS_MISC1, b1Num_LED);
	     }
    }

    if ( data_byte & BIT2 )
    {
		Hook_CAPLED_ON();
		SET_MASK(SYS_MISC1, b2Cap_LED);
    }
	else
	{
		Hook_CAPLED_OFF();
		CLR_MASK(SYS_MISC1, b2Cap_LED);
	}
}

//add new LED control function.
void Battery_Charge_Discharge_LED_CONTROL(WORD LED_ON,BYTE LED_OFF)   
{
	if(!(BAT_LED_Cnt_OFF|BAT_LED_Cnt_ON))
	{
		BAT_LED_Cnt_ON = LED_ON;
		BAT_LED_Cnt_OFF = LED_OFF;
	}
	if(BAT_LED_Cnt_ON)
	{
		BAT_LED_Cnt_ON = BAT_LED_Cnt_ON - 1;
		if( (BAT1PERCL >= 0) && (BAT1PERCL <= 80) )
		{     
			BAT_LOW_LED_ON();
			BAT_CHG_LED_OFF();
		}
		else
		{
			BAT_CHG_LED_ON();         
			BAT_LOW_LED_OFF();
		}
		
	}
	else if(BAT_LED_Cnt_OFF)
	{
		BAT_LED_Cnt_OFF = BAT_LED_Cnt_OFF - 1;
		BAT_CHG_LED_OFF();	         //LED OFF
		BAT_LOW_LED_OFF();	
	}
	
	
}

void Battery_LED_CONTROL(WORD LED_ON,BYTE LED_OFF)   
{
	BAT_CHG_LED_OFF();
	if(!(BAT_LED_Cnt_OFF|BAT_LED_Cnt_ON))
	{
		BAT_LED_Cnt_ON = LED_ON;
		BAT_LED_Cnt_OFF = LED_OFF;
	}
	
	if(BAT_LED_Cnt_ON)
	{
		BAT_LED_Cnt_ON = BAT_LED_Cnt_ON - 1;
		BAT_LOW_LED_ON(); 	
	}
	else if(BAT_LED_Cnt_OFF)
	{
		BAT_LED_Cnt_OFF = BAT_LED_Cnt_OFF - 1;
		BAT_LOW_LED_OFF();       
	}
	
	
}

void Battery_LED_Reset(void)
{
	
	BAT_LED_Cnt_ON = 0;
	BAT_LED_Cnt_OFF = 0;

	BAT_LOW_LED_OFF();         //LED OFF
	BAT_CHG_LED_OFF();

		
}


void Power_LED_CONTROL(LED_STATUSE STATUS)   //HEGANGS042:Modify the led behavior follow new spec
{
	if( BAT1PERCL > 0x14 )
	{
		PWR_AMBERLED_OUTPUT;
		CLR_MASK(PWM1LHE,DIMMING_ENABLE);
		PWR_AMBERLED_OFF();
		switch(STATUS)
		{
			case  LEDOFF:
				PWR_WHITELED_OUTPUT;
				CLR_MASK(PWM0LHE,DIMMING_ENABLE);
				PWR_WHITELED_OFF();
				break;
			case  LEDON:
				PWR_WHITELED_OUTPUT;
				CLR_MASK(PWM0LHE,DIMMING_ENABLE);
				PWR_WHITELED_ON();
				break;
			case LEDDIMMING:
				PWR_WHITELED_ALT;
				SET_MASK(PWM0LHE,DIMMING_ENABLE);
				break;
			default:
				break;
		}
	}
	else
	{
		PWR_WHITELED_OUTPUT;
		CLR_MASK(PWM0LHE,DIMMING_ENABLE);
		PWR_WHITELED_OFF();
		switch(STATUS)
		{
			case  LEDOFF:
				PWR_AMBERLED_OUTPUT;
				CLR_MASK(PWM1LHE,DIMMING_ENABLE);
				PWR_AMBERLED_OFF();
				break;
			case  LEDON:
				PWR_AMBERLED_OUTPUT;
				CLR_MASK(PWM1LHE,DIMMING_ENABLE);
				PWR_AMBERLED_ON();
				break;
			case LEDDIMMING:
				PWR_AMBERLED_ALT;
				SET_MASK(PWM1LHE,DIMMING_ENABLE);
				break;
			default:
				break;
			}
	}
}

void Lenovo_LED(void)
{
	if ( SystemIsS5||SystemIsDSX|| (SysPowState==SYSTEM_DSX_S5) )
	{ 
		PWR_LED_Debug=0;//Hang check
		Power_LED_CONTROL(LEDOFF);
   	}
	else
	{
		LOWBATT_3TIMES =0;
		if ( SystemIsS3 || (IS_MASK_SET(uISCT_2, b5ISCT_PwrLED)) )	// Check S3 and ISCT status.
		{
			Power_LED_CONTROL(LEDDIMMING);
		}
		else
		{
			if ( SystemIsS0 || (SysPowState==SYSTEM_S5_S0))	// Check S0 and power on sequence status.
			{
				if(PWR_LED_Debug) //Hang check
				{
					Power_LED_CONTROL(LEDDIMMING);
					Buffer_Key(0x14);	
					Buffer_Key(0x23);
					Buffer_Key(0xF0);
					Buffer_Key(0x23);	
					Buffer_Key(0x23);
					F_Service_SEND = 1;
				}
				else
				{
					if(ONEKEY_TEMP_FLAG) //HEGANGS025: Modify onekey battery function
					{
						Power_LED_CONTROL(LEDOFF);
					}
					else
					{
						Power_LED_CONTROL(LEDON);
					}
				}
			}
		}
	}

  	if ( IS_MASK_SET(SYS_MISC1,b5Crisis_LED) )
	{
		if(IS_MASK_SET(SEL_STATE0,PRESENT_A))
		{	 
		 	Battery_LED_CONTROL(20,10);//Amber, 1000ms on/ 500ms off
		}
		else
		{  
		 	Battery_LED_Reset();
		}	
	}
	else
	{
		if( IS_MASK_SET(LV_Authen_Step_CNT,BIT(6)) )
		{	// authentication Fail
	        if(SystemIsS0 || Read_AC_IN())
	        {
				Battery_LED_CONTROL(5,5);//Amber, 250ms on/ 250ms off
	        }
	        else // not S0 & Battery Only
	        { 
		      	Battery_LED_Reset();
		   	}	
		}
		else if( IS_MASK_SET(Bat1_FPChgFlag,BIT(0)) )
		{	
	        if(SystemIsS0 || Read_AC_IN())
	        {
			   	Battery_LED_CONTROL(5,5);//Amber, 250ms on/ 250ms off
	        }
	        else // not S0 & Battery Only
	        { 
				Battery_LED_Reset();
			}	
		}
		else if( Read_AC_IN() )
		{	// AC IN
			LOWBATT_3TIMES =0;
			
			if(IS_MASK_SET(SEL_STATE0,PRESENT_A))
			{	
				Battery_Charge_Discharge_LED_CONTROL(1,0);//White solid on
			}
			else
			{ 
			 	Battery_LED_Reset();
			}	// Off Amber and Green.
		}
		else
		{	
			if ( SystemIsS5 ||SystemIsDSX ||(SysPowState==SYSTEM_DSX_S5))
			{
				if (LOWBATT_3TIMES ==0)
				{
				   	Battery_LED_Reset();
				}
				else
				{	//low battery flash 3 times
					if (LOWBATT_3TIMES > 80)
					{
					   	Battery_LED_CONTROL(1,0);// On Amber
					}
					else if (LOWBATT_3TIMES > 60)
					{
					   	Battery_LED_CONTROL(0,1);// Off Amber
					}
					else if (LOWBATT_3TIMES > 40)
					{
					  	Battery_LED_CONTROL(1,0);// On Amber
					}
					else if (LOWBATT_3TIMES > 20)
					{
					  	Battery_LED_CONTROL(0,1);// Off Amber
					}
					else if (LOWBATT_3TIMES > 0)
					{
					    Battery_LED_CONTROL(1,0);// On Amber
                    }
					LOWBATT_3TIMES--;
					if (LOWBATT_3TIMES==0)
					{
					   	Battery_LED_Reset();	 // Off Amber
					}
				}
			}
			else
			{
				Battery_LED_Reset();
			}
		}
	}
}

//add new LED control function.

void MFG_LED(void)
{
	if(cOsLedCtrl.fbit.cOL_PwrLed == 1)
		PWR_WHITELED_ON();
	else
		PWR_WHITELED_OFF();
	if(cOsLed1Ctrl.fbit.cOL1_PwrAmber == 1)
		PWR_AMBERLED_ON();
	else
		PWR_AMBERLED_OFF();
	if(cOsLedCtrl.fbit.cOL_ChgLed == 1)
		BAT_CHG_LED_ON(); 
	else
		BAT_CHG_LED_OFF(); 

	if(cOsLedCtrl.fbit.cOL_DisChgLed == 1)
		BAT_LOW_LED_ON();
	else
		BAT_LOW_LED_OFF(); 

    if( uMBID & 0x01 ) //modify board id for MFG  from 0x04 to 0x01 follow new ec common spec
	{
		if( cOsLedCtrl.fbit.cOL_NumlockLed == 1 )
		{	
		//NUMLED_ON();
		}
		else
		{
		//NUMLED_OFF();
		}
	}

	if( cOsLed1Ctrl.fbit.cOL1_CapslockLed == 1 )
		CAPLED_ON();
	else
		CAPLED_OFF();

	if( cOsLed1Ctrl.fbit.cOL1_KBBacklight == 1 )
		DCR3 = 0x00; //Modify GPIO setting follow SIT.
	else
		DCR3 = 0XFF; //Modify GPIO setting follow SIT.
}

//Modify GPIO setting follow SIT.
void SetKeyboardLED(void)
{
	BYTE BKBMax,BKBHalf;
	if ( (SystemNotS0) || (cOsLedCtrl.fbit.cOL_CtrlRight ==1) || (!Read_LID_SW_IN()) )
	{
		//DCR3 = 0; //Modify KB backlight setting follow UI SPEC.
		return;
	}
	if (IS_MASK_SET(pDevStatus1,b2DisableKB))//HEGANGS039:Disable KB backlight when KB is disabled by YMC
	{  
	   DCR3 = 0;
	   return;
	}
	if(ONEKEY_TEMP_FLAG) //HEGANGS025: Modify onekey battery function
	{
		DCR3 = 0;
		return;
	}
	BKBMax = 0xF0;
	BKBHalf = 0x25;  //REJERRY007:modify step1 backlight.

	if(UpdateLEDBL_delay!=0)
	{
		UpdateLEDBL_delay--;
		return;
	}

	if(uReserve07.fbit.nVPCDriverIn==1)
	{
		if ( IS_MASK_SET(EMStatusBit, b0SetKBLEDON) )
		{
   			if ( IS_MASK_SET(EMStatusBit, b2KBLEDChk) )
 			{
   				LED_KB_PWM_Step=1;
 			}
		}
		else
		{
			LED_KB_PWM_Step=0;
		}
	}	

	if ( (LED_KB_PWM_Count & 0x7F) != 0 )
	{
		if ( DCR3 == 0 )  
		{
			DCR3 = BKBMax; 
			LED_KB_PWM_Count |= 0x80;
		}
		LED_KB_PWM_Count--;
	}
	else
	{
		if ( IS_MASK_SET(LED_KB_PWM_Count, BIT7) ) //modify backlight control.
		{
			if(LED_KB_PWM_Step==0)
			{
				DCR3 = 0;
				LED_KB_PWM_Count = 0;
			}
		}
		else 	
		{
			if(LED_KB_PWM_Step==0)
			{
				
				DCR3 = 0;
				SET_MASK(EMStatusBit, b2KBLEDChk);//Modify KB backlight control for lenovo app setting.
			}
			else if(LED_KB_PWM_Step==1)
			{
			
				DCR3=BKBHalf;
				CLEAR_MASK(EMStatusBit, b2KBLEDChk);//Modify KB backlight control for lenovo app setting.
			}
			else if(LED_KB_PWM_Step==2)
			{
			
				DCR3 = BKBMax;
			CLEAR_MASK(EMStatusBit, b2KBLEDChk);//Modify KB backlight control for lenovo app setting.
			}
		}
	}

}
//Modify GPIO setting follow SIT.
