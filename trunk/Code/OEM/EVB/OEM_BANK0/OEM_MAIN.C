/*-----------------------------------------------------------------------------
 * TITLE: OEM_MAIN.C - OEM specific code
 *
 * Some "HOOKS" from CHIPHOOK.C are copied into this file in order to replace
 * (or augment) core functions.
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

/*
*******************************************************************************
* Function name: Hook_Timer1msEvent       
*
* Descriptoin: Hook 1ms events.EventId is 0 ~ 9 cycle
*
* Invoked by: Timer1msEvent()
*
* TimeDiv:1ms
*
* Arguments: Timer1msCnt   
*
* Return Values: NA
*******************************************************************************
*/
void Hook_Timer1msEvent(IBYTE EventId)
{
	if(SystemIsS0)
    {
	  	if(USB_Delay!=0x00)
	 	{
			USB_Delay--;
			if(USB_Delay==0x00)
			{
            	USB_ON_OUTPUT;
                USB_ON_LOW();
			}
        }
    }  
	
    PollingBIOS80Port();
    EventManager(EventId);      // Polling system event
    Oem_SysPowerContrl();       // System Power Control
    SetVGA_AC_DET();

    if ( !Read_ENBKL_IN() )
    { 
        BKOFF_OFF(); 
		EC_TS_ON_LOW(); //Add enable/disable touch panel pin.
    }
		
}

/*
*******************************************************************************
* Function name: Hook_Timer5msEvent       
*
* Descriptoin: Hook 5ms events.
*
* Invoked by: Timer5msEvent()
*
* TimeDiv:NA
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/
void Hook_Timer5msEvent(void)
{

}

//Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
//------------------------------------------------------------
// Hook 2ms events
//------------------------------------------------------------
void Hook_Timer2msEvent(void)
{

}
//------------------------------------------------------------
// Hook 4ms events
//------------------------------------------------------------
void Hook_Timer4msEventA(void)
{

}
//------------------------------------------------------------
// Hook 4ms events
//------------------------------------------------------------
void Hook_Timer4msEventB(void)
{

}
//------------------------------------------------------------
// Hook20ms events
//------------------------------------------------------------
void Hook_Timer20msEventA(void)
{

	#if SUPPORT_UCSI
		ucsi_run();//UCSI feature
	#endif
}
// Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.

/*
*******************************************************************************
* Function name: Hook_Timer10msEventA       
*
* Descriptoin: Hook 10ms events.
*
* Invoked by: Timer10msEventA()
*
* TimeDiv:NA
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/
void Hook_Timer10msEventA(void)
{
	ScanADCFixChannel();
	Backlight_Control();
	EscScanCode(); 
#if !EN_PwrSeqTest
	if (PWRBTN_pressCnt !=0)
	{
		PWRBTN_pressCnt--;
		if (PWRBTN_pressCnt==0)
		{
			PM_PWRBTN_HI();
		}
	}
	
    AdapterIdentification(); //support identify adapter ID 
#endif
	//change deep sleep From 1ms hook to 10ms hook start
#if	Support_EC_Sleep
	if( CheckCanEnterDeepSleep() )
	{
		InitEnterDeepSleep();
		EnableAllInterrupt();	// enable global interrupt
		PLLCTRL = 0x01;         //PLL(Phase Locked Loop) will be powered down after setting PD bit in PCON and enter an EC power-down mode.
		PCON	= 0x02; 		/* Enter Deep Sleep mode.  */ //Power Down Mode  Set bit1 to ¡°1¡± to enter a Sleep (a.k.a. power-down) or Doze mode immediately.
		_nop_();				/* Wake-up delay.  */
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		IER3 = 0x00;
		InitWakeFromDeepSleep();
		WakeUp_DO_Function();
	}
	else
	{
		PCON=1; // Enter idle mode
	}
#else
	PCON=1;	// Enter idle mode
#endif	// Support_EC_Sleep
	//change deep sleep From 1ms hook to 10ms hook end
}


void Deal_CLEAR_CMOS(void)
{
	if (IS_MASK_SET(CMOS_TEST,b0_CMOS_FunctionKey))
  	{
		if(SystemIsS5)
		{     
	    	if(IS_MASK_CLEAR(CMOS_TEST,b1_CMOS_delay1flag))
			{ 
		    	cmosdelay++;
				RSMRST_LOW();
				if(cmosdelay==0x01)
				{
				}
				if(cmosdelay==0x02)
				{
		       		EC_ON_LOW();
				}
				if(cmosdelay==0x03)
				{
		        	PM_PWRBTN_LOW();
		        	AC_PRESENT_LOW();
				}
		    	if(cmosdelay==22)
				{   		
			    	RTCRST_ON_HI();
		        	cmosdelay=0;
		        	RamDebug(0x23);
					SET_MASK(CMOS_TEST,b1_CMOS_delay1flag);
					cmosdelay1 = 0x00;
				}	
			}
			else if(cmosdelay1>=35) //3.5S
			{
		 		cmosdelay++;
				RTCRST_ON_LOW();
				SET_MASK(CMOS_TEST,BIT3);	
				if(cmosdelay==10)
				{
					if(Read_AC_IN())
		    			EC_ON_HI();
		 		}
		    	if(cmosdelay==11)
				{
 					if(Read_AC_IN())
						PM_PWRBTN_HI();
				}		
				if(cmosdelay==12)
				{
			    	if(Read_AC_IN())
			    		{ 
			    		}
		 		}
		    	if(cmosdelay==15)
				{
					if(Read_AC_IN())
		        		RSMRST_HI();
			        	CLEAR_MASK(CMOS_TEST,b1_CMOS_delay1flag);
		            	CLEAR_MASK(CMOS_TEST,b0_CMOS_FunctionKey);
						CLEAR_MASK(CMOS_TEST,BIT3);
		            	cmosdelay1=0;
 					if(Read_AC_IN())
		            	AC_PRESENT_HI();	
		            	RamDebug(0x4A);
		 		}
		
			}
		}
  	}
  	else
  	{
	  	cmosdelay=0;
  	}
}


/*
*******************************************************************************
* Function name: Hook_Timer10msEventB      
*
* Descriptoin: Hook 10ms events.
*
* Invoked by: Timer10msEventB()
*
* TimeDiv:50ms
*
* Arguments: NA
*
* Return Values: NA
*******************************************************************************
*/
void Hook_Timer10msEventB(void)
{
    CheckSBPowerButton(); 
#if !EN_PwrSeqTest
	ScanADCDyChannel1();
	
	if(SystemIsS0&& (PwrOnDly5Sec==0)) 
	{
		PollingCPURT(); 
		PollingGPURT(); 
	}
	
#endif

  	if( delayEDPTm > 0 )       // CMW Temp
  	{
        delayEDPTm--;
  	}
	

	CPUProchotCtrl();
	GPUProchotOnControl(); 
}

/*
*******************************************************************************
* Function name: Hook_Timer50msEventA      
*
* Descriptoin: Hook 50ms events.
*
* Invoked by: Timer50msEventA()
*
* TimeDiv:50ms
*
* Arguments: NA
*
* Return Values: NA
*******************************************************************************
*/
void Hook_Timer50msEventA(void)
{
	if (cOsLedCtrl.fbit.cOL_CtrlRight ==0 )
		Lenovo_LED();
	else
		MFG_LED();

	if((GPUProchotONCnt == 0)&&(IS_MASK_CLEAR(BATTUPDATEFW,BIT0)))
		ChkPsys();
	TurboDCOnly();
}

/*
*******************************************************************************
* Function name: Hook_Timer50msEventB      
*
* Descriptoin: Hook 50ms events.
*
* Invoked by: Timer50msEventB()
*
* TimeDiv:50ms
*
* Arguments: NA
*
* Return Values: NA
*******************************************************************************
*/
void Hook_Timer50msEventB(void)
{
	PWM_TimeCount();
}


/*
*******************************************************************************
* Function name: Hook_Timer50msEventC       
*
* Descriptoin: Hook 50ms events.
*
* Invoked by: Timer50msEventC()
*
* TimeDiv:50ms
*
* Arguments: NA
*
* Return Values: NA
*******************************************************************************
*/
void Hook_Timer50msEventC(void)
{
	MuteProcess();
#if !EN_PwrSeqTest
	Chk_pDevStus();
	Update_EEPROMB07();
#endif
	
	if(SystemIsS0)
	{
		Chk_FAN_RPM_Control();	// Check FAN is AP control.
		Chk_FAN1_RPM_Control();	// Check FAN1 is AP control.//72JERRY014¡êo Add the second fan method
	}
}

/*
*******************************************************************************
* Function name: Hook_Timer100msEventA       
*
* Descriptoin: Hook 100ms events.
*
* Invoked by: Timer100msEventA()
*
* TimeDiv:100ms
*
* Arguments: NA
*
* Return Values: NA
*******************************************************************************
*/
void Hook_Timer100msEventA(void)
{
	ThmIC_Temperature();  //Enable thermal IC feature.
	SetKeyboardLED(); //Modify KB backlight setting follow UI SPEC.	
#if !EN_PwrSeqTest
	#if WDT_Support
	ResetInternalWDT();
	#endif
#if Support_AOU5_V1_2
	 AOU_Main();
#endif 
  	if (IS_MASK_SET(SEL_STATE0,PRESENT_A))
  	{ 
  		OEM_PollingBatData_TASK(); 
    }
	else
	{ 
	    CLEAR_MASK(StatusKeeper, BatteryProtectCHG); 
	}                        
#endif	// EN_PwrSeqTest
	if (IS_MASK_SET(CMOS_TEST,b1_CMOS_delay1flag)&&IS_MASK_CLEAR(CMOS_TEST,BIT3))
	{
    	cmosdelay1++;
	}

}

/*
*******************************************************************************
* Function name: Hook_Timer100msEventB       
*
* Descriptoin: Hook 100ms events.
*
* Invoked by: Timer100msEventB()
*
* TimeDiv:100ms
*
* Arguments: NA
*
* Return Values: NA
*******************************************************************************
*/
void Hook_Timer100msEventB(void)
{
   VGA_Temperature();  // HEGANGS010:Enable GPU temperature and remove read remote temp
#if !EN_PwrSeqTest
	WirelessProcess();		// Control WLAN and BT.
	IFFSProcess();
#endif
	ACOutProchotRelease(); 
    if((FunctionKeyDebounce&0x0F)!=0)
    {
      FunctionKeyDebounce--;
      if((FunctionKeyDebounce&0x0F)==0x00)
      {
          (HotKey_Fn_Fx[((FunctionKeyDebounce&0xF0)>>0x04)-1])(1);
	      FunctionKeyDebounce=0x00;
      }	
    }
}


/*
*******************************************************************************
* Function name: Hook_Timer100msEventC       
*
* Descriptoin: Hook 100ms events.
*
* Invoked by: Timer100msEventC()
*
* TimeDiv:100ms
*
* Arguments: NA
*
* Return Values: NA
*******************************************************************************
*/
void Hook_Timer100msEventC(void)
{
#if !EN_PwrSeqTest
	Oem_Fan_Speed();
	Oem_Fan1_Speed();// Add the second fan method
	GPUThrottlingControl(); 
	
	Battery100ms();
	if (TouchPadCount !=0)
	{ 
	    TouchPadCount--; 
    }
	

	if ( uReserve07.fbit.nFanManual == 0 )
	{
		if((ManualFanPRM==0)||(ManualFan2PRM==0)) //Modify fan table follow '720S_13IKB fan table_V01'.
			{
		if ( IS_MASK_CLEAR(THERMAL_STATUS, INITOK) )
		{	// EC control fan.
			#if FAN_TABLE_Already
			Oem_Fan_Control();
			Oem_Fan1_Control();//Add the second fan method
			#endif	// FAN_TABLE_Already
		}
		else
		{  
        }
			}
	}

#endif
}

/*
*******************************************************************************
* Function name: Hook_Timer500msEventA       
*
* Descriptoin: Hook 500ms events.
*
* Invoked by: Timer500msEventA()
*
* TimeDiv:500ms
*
* Arguments: NA
*
* Return Values: NA
*******************************************************************************
*/
void Hook_Timer500msEventA(void)
{
#if !EN_PwrSeqTest
  	if (IS_MASK_SET(SEL_STATE0,PRESENT_A))
  	{
		Chk_BatSMbusFailCount();

		if (Read_AC_IN() && IS_MASK_CLEAR(BATTUPDATEFW,BIT0))
		{
		    Battery_Expresscharge();//Support fast charging and modify charger IC setting
    		WriteSmartChgIC();
			ReadSmartChgIC();
		}
  	}
  	
	//Update Battery info after FW update
	if(IS_MASK_SET(BATTUPDATEFW, BIT0))
		SET_MASK(StatusKeeper, BatteryFwUpdate);
	if(IS_MASK_SET(StatusKeeper, BatteryFwUpdate) && IS_MASK_CLEAR(BATTUPDATEFW, BIT0))
	{
		CLEAR_MASK(StatusKeeper, BatteryFwUpdate);
		FirstGetBatData();
		Service_Auth_Step = 1;				// start SHA1
	}
	if((IS_MASK_SET(SEL_STATE0,PRESENT_A))&&(BatSMbusFailCount==0)&&(Battdata_ready==0)) //HEGANG0020:Addd retry mechanism when first get battery data.
	{
		FirstGetBatData();
	}

    #if SHA1_Support
    if(nDesignCapL !=0 && nPresentVoltH != 0)
    {
		if ((IS_MASK_SET(SEL_STATE0,PRESENT_A))
      && IS_MASK_CLEAR(StatusKeeper, BatteryFwUpdate))//Add the flag to avoid battery SH1 authentication when battry update FW.
		{
			LV_BAT_Authentication();
		}
    }
    #endif	// SHA1_Support
#endif	// EN_PwrSeqTest
}

/*
*******************************************************************************
* Function name: Hook_Timer500msEventB       
*
* Descriptoin: Hook 500ms events.
*
* Invoked by: Timer500msEventB()
*
* TimeDiv:500ms
*
* Arguments: NA
*
* Return Values: NA
*******************************************************************************
*/

void Hook_Timer500msEventB(void)
{
#if !EN_PwrSeqTest
	Voicewakeup_MFG_Process();//Add CMD 0x59 test voice wake up for MFG.
	#if SW_ISCT
	ISCT_Process();
	#endif	

	if(cGPUACtoBattTime!=0)
	{
		if(Read_AC_IN()||(nBattGasgauge >7))
			cGPUACtoBattTime--;
	}
	DownACtoBatteryGPUState();

	if(PCH_ColdBoot_TimeOut > 0) 
	{
		PCH_ColdBoot_TimeOut--;
		if((PCH_ColdBoot_TimeOut == 0)&&SystemIsS0)
		{
			ProcessSID(COLDBOOTFAIL_ID);
			SET_MASK(ACPI_HOTKEY, b7BIOS_NoShut);
			RSMRST_LOW();
			Delay1MS(1); 
			RSMRST_HI();
		}
		
		if(SysPowState != SYSTEM_S0) 
			PCH_ColdBoot_TimeOut = 0; 
	}
		
#endif
}

/*
*******************************************************************************
* Function name: Hook_Timer500msEventC       
*
* Descriptoin: Hook 500ms events.
*
* Invoked by: Timer500msEventC()
*
* TimeDiv:500ms
*
* Arguments: NA
*
* Return Values: NA
*******************************************************************************
*/
void Hook_Timer500msEventC(void)
{
#if !EN_PwrSeqTest
	Clear_Batt_First_Used_Date(); 


	// Check KBC Output Buffer Full
    if (IS_MASK_SET(KBHISR,OBF)&&(SystemIsS0))
    {
		KB_OBF_count++;
		if ((KB_OBF_count & 0x0F) > 10)				// OBF=1 over 5sec?
		{
			KB_OBF_count = KB_OBF_count + 0x10;		// high nibble for count times of clear
			KB_OBF_count &= 0xF0 ;
			GENIRQ = 0x0C;
		}
    }
    else
    {
		KB_OBF_count &= 0xF0 ;
    }
#endif
}


/*
*******************************************************************************
* Function name: checkclearcmos       
*
* Descriptoin: Check clear cmos.
*
* Invoked by: Hook_Timer1SecEventA()
*
* TimeDiv:1sec
*
* Arguments: NA
*
* Return Values: NA
*******************************************************************************
*/
void checkclearcmos(void)
{
	if (IS_MASK_SET(CMOS_TEST,BIT2))
	{
		if((!Read_SLPS3())&&(!Read_SLPS4()))
		{
			cmosshutdelay++;
			RamDebug(0x4B);
		}
		else
		{
			cmosshutdelay=0;
			CLEAR_MASK(CMOS_TEST,BIT2);
		}
		if(cmosshutdelay==5)
		{
			 SysPowState=SYSTEM_S5;  
			 SET_MASK(CMOS_TEST,b0_CMOS_FunctionKey); 
			 CLEAR_MASK(CMOS_TEST,BIT2);
			 cmosshutdelay = 0x00;		 
		}
	}
}	


/*
*******************************************************************************
* Function name: Hook_Timer1SecEventA       
*
* Descriptoin: Hook 1sec events.
*
* Invoked by: Timer1SecEventA()
*
* TimeDiv:1sec
*
* Arguments: NA
*
* Return Values: NA
*******************************************************************************
*/
void Hook_Timer1SecEventA(void)
{
	checkclearcmos();
#if SUPPORT_PMIC
	if(PowerOVP_REG|PowerUVP_REG|PowerOCP_REG|PowerOTP_REG)
	{
		
	}
	else
	{
		if(SystemNotS5)
			PollingOTP();
	}
#endif
	if ( SystemIsS0 && (PwrOnDly5Sec!=0) )
	{ PwrOnDly5Sec--; } 
	
#if !EN_PwrSeqTest

    Oem_Thermal_Control();  // Read CPU temperature by PECI.//Add the second fan method

	OEM_Throttling_Ctrl();

	Battery1Sec();
    

	if ( SystemIsS5||SystemIsDSX )
	{
		if (ECSleepCount !=0)
		{ 
		    ECSleepCount--; 
        }
	}

	if (CountSecAfterPswPressed < 255)
		CountSecAfterPswPressed++;
	if (CountSecAfterNOVOPressed < 255)
		CountSecAfterNOVOPressed++;

	CPUThrottlingDelay();   
	
#endif
}


/*
*******************************************************************************
* Function name: Hook_Timer1SecEventB       
*
* Descriptoin: Hook 1sec events.
*
* Invoked by: Hook_Timer1SecEventB()
*
* TimeDiv:1sec
*
* Arguments: NA
*
* Return Values: NA
*******************************************************************************
*/
void Hook_Timer1SecEventB(void)
{
#if !EN_PwrSeqTest
	if(QEVENT_DEBUG!=0x00)
    {
        ECQEvent(QEVENT_DEBUG);
        QEVENT_DEBUG=0x00;
    }  
	SystemAutoOn();

	if(IS_MASK_CLEAR(BATTUPDATEFW,BIT0) && IS_MASK_SET(BT1_STATUS1,bat_in))
  	{
		bRWSMBus(SMbusChB, SMbusRW, SmBat_Addr,C_FW_Status,&FirmwareUpdateStatusL,SMBus_NoPEC);
		bRSMBusBlock(SMbusChB,SMbusRBK,SmBat_Addr,C_BAT_Info,&BatterymakerIDL);
  	}
#endif			
	if(IS_MASK_SET(SEL_STATE0,PRESENT_A))
 	{
     	if(IS_MASK_SET(pProject0,b5FUBIOSWriteReady))
     	{
		  	if(bRWSMBus(SMbusChB,SMbusWW,SmBat_Addr,FirstUsedDate,&batteryFirstUsedDateL,SMBus_NeedPEC))
		  	{    
				RamDebug(0xAF);
		  	}
		  	CLEAR_MASK(pProject0,b5FUBIOSWriteReady);
		  	CLEAR_MASK(pProject0,b3uBatteryTimeNoOK);
     	}	  
 	} 		

}

/*
*******************************************************************************
* Function name: Hook_Timer1SecEventC       
*
* Descriptoin: Hook 1sec events.
*
* Invoked by: Timer1SecEventC()
*
* TimeDiv:1sec
*
* Arguments: NA
*
* Return Values: NA
*******************************************************************************
*/

void Hook_Timer1SecEventC(void)
{
	if(IS_MASK_CLEAR(StatusKeeper, BatteryFwUpdate) ) 
	{ 
		if(ShipModeCnt>0)
		{
			ShipModeCnt--;
			if(ShipModeCnt==0)
			{
				Unlock_ShipMode();
			}
		}
	} 

#if !EN_PwrSeqTest
	if ( uReserve07.fbit.nFanManual == 0 )
	{
		if ( IS_MASK_CLEAR(THERMAL_STATUS, INITOK) )
		{
		}
		else
		{ 
		    FAN_Dust_Mode(); 
        }
	}
	//Add workaround for hang bios.
	#if WorkaroundHangBios
    Workaround_For_HangBios();
    #endif
#endif	
}

/*
*******************************************************************************
* Function name: Hook_Timer1MinEvent       
*
* Descriptoin: Hook 1min events.
*
* Invoked by: Timer1MinEvent()
*
* TimeDiv:1 min
*
* Arguments: NA
*
* Return Values: NA
*******************************************************************************
*/

void Hook_Timer1MinEvent(void)
{
#if !EN_PwrSeqTest
	Chk_CHG_TimeOut();

	#if SW_ISCT
	ISCT_TimerCnt();
	#endif
#endif
}

//------------------------------------------------------------
// service_OEM_1
//------------------------------------------------------------
void service_OEM_1(void)
{

}

//------------------------------------------------------------
// service_OEM_2
//------------------------------------------------------------
void service_OEM_2(void)
{

}

//------------------------------------------------------------
// service_OEM_3
//------------------------------------------------------------
void service_OEM_3(void)
{

}

//------------------------------------------------------------
// service_OEM_4
//------------------------------------------------------------
void service_OEM_4(void)
{

}

//------------------------------------------------------------
//
//------------------------------------------------------------
void Hook_main_service_H(void)
{

}

//------------------------------------------------------------
//
//------------------------------------------------------------
void Hook_main_service_L(void)
{

}

/*
*******************************************************************************
* Function name: Hook_Only_Timer1msEvent      
*
* Descriptoin: 
*     Return :
*       Only_Timer1msEvent : Only Timer1msEvent and Hook_Timer1msEvent function
*       All_TimerEvent : All timer events are OK.
*
* Invoked by: service_1mS()
*
* TimeDiv:NA
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/

BYTE Hook_Only_Timer1msEvent(void)
{
	// Return :
	//  Only_Timer1msEvent : Only Timer1msEvent and Hook_Timer1msEvent function
	//  All_TimerEvent : All timer events are OK.
    return(All_TimerEvent);
}

/*
*******************************************************************************
* Function name: OEM_SkipMainServiceFunc      
*
* Descriptoin: 
*    Note:
*      1. Always return(0xFF|Normal_MainService) to run normal main_service function.
*      2. If you don't understand the use of OEM_SkipMainServiceFunc function, don't change anything.
*
* Invoked by: main()
*
* TimeDiv:NA
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/

BYTE OEM_SkipMainServiceFunc(void)
{

    // Return :
    return(Normal_MainService);
}


//-----------------------------------------------------------------------------
const BYTE code SEG7_Table[]=
{
	/*            a
               ---------
               |       |
           f   |   g   |  b
              ---------
               |       |
           e   |       |  c
               ---------
                   d      . h
     */
	// COMMON ANODE SEVEN SEGMENT DISPLAY TABLE
    //  0	  1     2     3     4     5     6     7
       0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    //  8     9     A     b     C     d     E     F
       0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E,
};

/*
*******************************************************************************
* Function name: Out7SegLED      
*
* Descriptoin: Out 7Seg Led
*
* Invoked by: P80LedOut()
*
* TimeDiv:NA
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/

void Out7SegLED(BYTE bData)
{
	BYTE bIndex = 7;
	
	while(bIndex != -1)
	{
		if (bData & BIT(bIndex))
		{ 
		    O_DEBUG_DAT_HI(); 
        }
		else
		{ 
		    O_DEBUG_DAT_LO(); 
        }
		O_DEBUG_CLK_LO();
		O_DEBUG_CLK_HI();
		bIndex--;
	}
}

/*
*******************************************************************************
* Function name: Dark7SegLed      
*
* Descriptoin: Dark 7Seg Led
*
* Invoked by: NA
*
* TimeDiv:NA
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/

void Dark7SegLed(void)
{
	Out7SegLED(0xFF);	// drak LED
	Out7SegLED(0xFF);	// drak LED
}


/*
*******************************************************************************
* Function name: P80LedOut      
*
* Descriptoin: Port 80 LED out
*
* Invoked by: PollingBIOS80Port()
*
* TimeDiv:NA
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/

void P80LedOut(BYTE bData)
{
	BYTE code * data_pntr;
	// out upper byte
	data_pntr = SEG7_Table;					// get 7-seg table
	data_pntr +=((bData & 0xF0) >> 4);		// mask upper byte and shit 4 bytes	then get data of table

	BRAM3A = *data_pntr;
	Out7SegLED(*data_pntr);					// display upper value.

	// out lower byte
	data_pntr = SEG7_Table;					// get 7-seg table
	data_pntr += (bData & 0x0F);	 		// mask lower byte then get data of table.

	BRAM3B = *data_pntr;

	Out7SegLED(*data_pntr);					// dispaly lower value.
}

/*
*******************************************************************************
* Function name: PollingBIOS80Port      
*
* Descriptoin: Polling BIOS 80 port.
*
* Invoked by: Hook_Timer1msEvent()
*
* TimeDiv:1ms
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/
void PollingBIOS80Port(void)
{
	if ( SystemNotS0 )
	{
		EC_RX_INPUT;
		EC_TX_INPUT;
        UART_DB_RAM = 0x00; 
		return;
	}
	
	if((IS_MASK_CLEAR(UART_DB_RAM,BIT7))&&(IS_MASK_CLEAR(UART_DB_RAM,BIT3))) 
	{
		if(Read_EC_TX())     // EC_TX for detect debug card plug in //Don't hot plug
		{
			EC_TX_OUTPUT;
	    	EC_RX_OUTPUT; 
			UART_DB_RAM = UART_DB_RAM|0x80;
		}

        if(IS_MASK_CLEAR(P80CMOSSts,P80CMOSDis))
        {
            UART_DB_RAM = UART_DB_RAM|0x08;
        }
              
		return;
	}


	BRAM3D++;
	if(BRAM3F != BRAM3E)				// if no data in , return.
	{	// clear it.
		printf("\nP80h : 0x%bX",BRAM3F);
		BRAM3E=	BRAM3F;					// clear it.
		DelayXms(1);
	}
	else
	{
		if(BRAM3F != BRAM3C)			// if no data in , return.
		{
			BRAM3C= BRAM3F;				// clear it.
            if(IS_MASK_SET(UART_DB_RAM,BIT7))  
            {
				EC_RX_OUTPUT;
				EC_TX_OUTPUT;
        	    P80LedOut(BRAM3F);			// to show it
            }      
			// start for save P80 code to setup. 
        	if((P80Index < 0x07)&&(IS_MASK_CLEAR(P80CMOSSts,P80CMOSDis)))
			{
				P80CMOS[P80Index]= BRAM3F;
				P80Index++;
				if(P80Index==0x07)
				{
			    	P80Index=0;
				}
			}
			// end for save P80 code to setup. 
            if(IS_MASK_SET(UART_DB_RAM,BIT7)) 
            {
        		EC_RX_ALT;
				EC_TX_ALT;
            }
		}
	}
}

/*
*******************************************************************************
* Function name: ProcessSID       
*
* Descriptoin: Process shutdown ID.
*
* Invoked by: main(),Cmd_59(),RSOC1Pto0P_ShutdownCheck(),Lock_ShipMode(),Chk_BatSMbusFailCount(),
*             VGA_Temperature(),ReadPCHTemp(),Thro_Count_Shut(),Hook_Timer500msEventB(),
*             RecordenEnterShipMode(),Oem_SysPowerContrl(),S0S5_Init_Status(),SystemAutoOn()
*
* TimeDiv:NA
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/

void ProcessSID(BYTE SID)	// Shutdown ID.
{
	pLastSID4 = pLastSID3;
	pLastSID3 = pLastSID2;
	pLastSID2 = pLastSID;
	pLastSID  = SID;

	Update_16ByteEEPROM();
}

//----------------------------------------------------------------------------
// Output : 1 Debug mode
//          0 Normal mode
//----------------------------------------------------------------------------
// start for save P80 code to setup. 
bit CheckDebugMode(void)
{	// To add condition here
	BYTE BKSOL,BKSOH1,BKSOH2;
    BKSOL = KSOL;
    BKSOH1 = KSOH1;
    BKSOH2 = KSOH2;

	KSOL  = 0xFF;
	KSOH1 = 0xBF;
	KSOH2 = 0xFF;			// Fn key (scan line b14)
	WNCKR = 0x00;           // Delay 15.26 us
	WNCKR = 0x00;           // Delay 15.26 us
	
	if ((KSI&0x10) != 0)	// Fn key (data line b4)
	{
		KSOH1 = 0xFF;
        KSOL  = BKSOL;
        KSOH1 = BKSOH1;
        KSOH2 = BKSOH2;
	    return FALSE;
	}
	KSOL  = 0xFD;
	KSOH1 = 0xFF;
	KSOH2 = 0xFF;			// "D" key (scan line b1)
	WNCKR = 0x00;           // Delay 15.26 us
	WNCKR = 0x00;           // Delay 15.26 us
	
	if ((KSI&0x10) != 0)	// "D" key (data line b4)
	{
		KSOL  = 0xFF;
        KSOL  = BKSOL;
        KSOH1 = BKSOH1;
        KSOH2 = BKSOH2;
	    return FALSE;
	}
	KSOL  = 0xFF;
    KSOL  = BKSOL;
    KSOH1 = BKSOH1;
    KSOH2 = BKSOH2;

	return TRUE;
}

/*
*******************************************************************************
* Function name: Workaround_For_HangBios       
*
* Descriptoin: workaround for hang bios.
*
* Invoked by: Hook_Timer1SecEventC()
*
* TimeDiv:1s
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/
void Workaround_For_HangBios(void)
{
	if(Bioswatchdog==1)
	{
		Bioswatchdogtime=Bioswatchdogtime+1;
		if(Bioswatchdogtime==250)
		{
			BRAM[63]=0x55;
			bios_count++;
            WinFlashMark=0x53;//Modify after flash EC can auto poweron follow bios.
           	WinFlashMark2=0x35;//Modify after flash EC can auto poweron follow bios.
        	InternalWDTNow(WORKAROUND_ID);
		}
	}
	else
	{
		Bioswatchdogtime=0;
	}
}

/*
*******************************************************************************
* Function name: ACOutProchotRelease       
*
* Descriptoin: CPU and GPU prochot release after AC out 2S.
*
* Invoked by: Hook_Timer100msEventB()
*
* TimeDiv:100ms
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/
void ACOutProchotRelease(void)
{
	if(CPUProchotONCnt > 0) //modify ProchotONCnt name.
	{ 
		CPUProchotONCnt--; 
	}


	if(GPUProchotONCnt > 0)
	{ 
		if(Read_AC_IN()||(nBattGasgauge >7))
		GPUProchotONCnt--; 
	}

	if(BatteryOCPDelay > 0) 
	{ 
		BatteryOCPDelay--; 
	}
}

//add GPU prochot on after D5 3S.
#if SUPPORT_GPU_PROCHOT

/*
*******************************************************************************
* Function name: GPUProchotOnControl       
*
* Descriptoin: GPU prochot Control
*
* Invoked by: Hook_Timer10msEventA()
*
* TimeDiv:10ms
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/
void GPUProchotOnControl(void)
{
	if(SystemNotS0)
	{
		GPU_Prochot = 0;
		
		GPU_PROCHOT_ON();
		return;
		
	}

	if((GPUProchotONCnt == 0) && (IS_MASK_CLEAR(GPU_Prochot, (b0_Thermal+b1_Psys+b2_battery_OCP+b3_battery_OTP+b4_battery_low)))) 
	{
		GPU_PROCHOT_OFF();
	}
	else
	{
		GPU_PROCHOT_ON();
	}
}
#endif


/*
*******************************************************************************
* Function name: CPUThrottlingDelay       
*
* Descriptoin: CPU throttling 0x1D event delay
*
* Invoked by: Hook_Timer1SecEventA()
*
* TimeDiv:1s
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/

void CPUThrottlingDelay(void)
{
	if(CPUThrottlingDelayTime > 0)
        CPUThrottlingDelayTime--;
}

