/*-----------------------------------------------------------------------------
 * TITLE: CORE_MAIN.C - Main Program for KBC firmware.
 *
 * Main processing loop (interrupt driven) for the keyboard controller/scanner.
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
 *  FUNCTION: main - Main service loop.
 *
 *  Wait in idle state until an IRQ causes an exit from idle.  If the IRQ
 *  handler posted a service request (via bSERVICE) then dispatch control to
 *  the appropriate service handler.  Otherwise, go back to idle state.  After
 *  all service requests have been handled, return to idle state.
 * ------------------------------------------------------------------------- */
void main(void)
{
	DisableAllInterrupt();
	SP = 0xC0;					// Setting stack pointer
	_nop_();
    _nop_();
    _nop_();
    _nop_();
    DCache = 0x03;
       
	CLEAR_MASK(RAM_16FF,BIT0);	// for AX

	if(Hook_ECRetunrMainFuncKeepCondition()==0x33)  // Exit from follow mode or EC scatch ROM
	{
		CLEAR_MASK(FBCFG,SSMC); // disable scatch ROM
		_nop_();
	    MPRECF = 0x01;
	    _nop_();
	    MPRECF = 0x01;
	    _nop_();
	    MPRECF = 0x01;
	    _nop_();
	    MPRECF = 0x01;
	    _nop_();
	    _nop_();
        WinFlashMark = 0xFC;
		WinFlashMark1=0x55;		//G21: Add WinFlashMark1 check
        WinFlashMark2 = 0x00;
        ResetBANKDATA();        // init bank mechanism to code bank 0
        Hook_ECExitFollowMode();
        Init_Timers();
	    EnableModuleInterrupt();
	    CheckECCHIPVer();
	}
	else
	{
		ECSleepCount = 3;
		//ProcessSID(COLDBOOT_ID);
		//pLastSID  = COLDBOOT_ID; //ANGELAS089: remove
        //uMBID = Read_Eflash_Byte(EEPROMA2,(EEPROMA1_B03 | 0x07) ,0xE0);

		EEPROM_PwrSts = Read_Eflash_Byte(EEPROMA2,(EEPROMA1_B03 | 0x07),0xDF);
		if( (EEPROM_PwrSts & 0x10) != 0 )
		{
			Core_Init_Regs();
			if( CheckCrisisMode() )
			{ 
			    SET_MASK(SYS_MISC1,Crisis_On);
				SET_MASK(SYS_MISC1,BATCRISIS_FLAG);
			}
			else
			{ 
			    CLR_MASK(SYS_MISC1,Crisis_On); 
				CLR_MASK(SYS_MISC1,BATCRISIS_FLAG); 
            }
		}

		Core_Initialization();
		Oem_Initialization();
        InitEnableInterrupt();
		
        #ifdef UART_Debug
        Init_UART();
        printf("\n************************************");
        printf("\nEC Init OK !!!");
        printf("\n************************************");
        #endif
	}
	
	#if !EN_PwrSeqTest
		#if WDT_Support
		EnableInternalWDT();
		#endif
	#endif
	
    #if Support_Mirror_Code
    if(!((WinFlashMark == 0xFC)&&(WinFlashMark2 == 0x00)&&(WinFlashMark1==0x55))) //Add WinFlashMark1 check
    {
    	RamDebug(0x88);
       	Check_Mirror_Occurs(); 
       	if(Read_Eflash_Byte(0x00, 0x00, 0x4D) != 00)
       	{
           	Do_Eflash_Write_1Byte(0x00, 0x00, 0x00, 0x4D);            
           	RamDebug(0x66);
	    	FLHCTRL3R |= 0x80;	
	     	HINSTC1 |= 0x40;           
            RamDebug(0x77);               
       	}
    }    
    #endif
	ProcessSID(ShutDnCause); //ANGELAS089:add
	//T11N + e
	while(1)
   	{
        if(OEM_SkipMainServiceFunc()==Normal_MainService)
        {
    		main_service();
    		EnableModuleInterrupt();
    		_nop_();
    		_nop_();
    		_nop_();
    		_nop_();
    
    
        #if TouchPad_only  //Update PS2 code base.
    
        #else
        	if(PS2CheckPendingISR()==0x00)
           	{
            	ScanAUXDeviceStep();
            }
        #endif

        #ifdef SMBusServiceCenterFunc
    		if((Service==0x00)&&(Service1==0x00)&&(CheckSMBusNeedService()==SMBus_ClearService))
        #else
            if((Service==0x00)&&(Service1==0x00))
        #endif
            {
                if (TH0< Timer_1ms>>8)
                    TH0 = Timer_1ms>>8;
            }
        }
  	}
}

/* ----------------------------------------------------------------------------
 * FUNCTION: main_service - Check for new/more service requests.
 *
 * Check for a request flag.  The check is done according to priority.  If a
 * request flag is set, handle the request and loop back to get the flags
 * again.  Do not follow through to check the next flag.  The flags are to be
 * checked in order.
 * ------------------------------------------------------------------------- */
void main_service(void)
{
    #ifdef SMBusServiceCenterFunc
    while((Service!=0x00)||(Service1!=0x00)||(CheckSMBusNeedService()==SMBus_NeedService))
    #else
    while((Service!=0x00)||(Service1!=0x00))
    #endif
    { 
		/* //
     		if( IS_MASK_SET(BATTUPDATEFW, BIT0))
     		{
      		 	if(F_Service_PCI2)
        		{
            			F_Service_PCI2=0;
            			service_pci2();
            			continue;
        		}
   	 	}
		*/     
        //-----------------------------------
        // Host command/data service
        //-----------------------------------
        if(F_Service_PCI)
        {
            F_Service_PCI=0;
            service_pci1();
            continue;
        }

        //-----------------------------------
        // Service unlock
        //-----------------------------------
        if(F_Service_UNLOCK)
        {
            F_Service_UNLOCK=0;
            service_unlock();
            continue;
        }

        //-----------------------------------
        // Send byte from KBC
        //-----------------------------------
        if(F_Service_SEND)
        {
            F_Service_SEND=0;
            service_send();
            continue;
        }

        //-----------------------------------
        // Send PS2 interface data
        //-----------------------------------
        if(F_Service_Send_PS2)
        {
            F_Service_Send_PS2=0;
            service_PS2_data();
            continue;
        }

        //-----------------------------------
        // process PS2 interface data
        //-----------------------------------
        if(F_Service_PS2)
        {
            F_Service_PS2=0;
            service_ps2();
            continue;
        }

        //-----------------------------------
        // process SMBus interface data
        //-----------------------------------
        #ifdef SMBusServiceCenterFunc
        ServiceSMBus();
        #endif
       if(F_Service_PCI2)
        {
            F_Service_PCI2=0;
            service_pci2();
            continue;
        }
        //-----------------------------------
        // 1 millisecond elapsed
        //-----------------------------------
        if(F_Service_MS_1)
        {
            F_Service_MS_1=0;
            service_1mS();
            continue;
        }

        //-----------------------------------
        // Secondary Host command/data service
        //-----------------------------------
        if(F_Service_PCI2)
        {
            F_Service_PCI2=0;
            service_pci2();
            continue;
        }

        //-----------------------------------
        // Keyboard scanner service
        //-----------------------------------
        if(F_Service_KEY)
        {
            F_Service_KEY=0;
            service_scan();
            continue;
        }

        //-----------------------------------
        //
        //-----------------------------------
        Hook_main_service_H();

        //-----------------------------------
        // Low level event
        //-----------------------------------
        if(F_Service_Low_LV)
        {
            F_Service_Low_LV=0;
            service_Low_LVEvent();
            continue;
        }

        //-----------------------------------
        // Third Host command/data service
        //-----------------------------------
        if(F_Service_PCI3)
        {
            F_Service_PCI3=0;
            service_pci3();
            continue;
        }

        //-----------------------------------
        // CIR IRQ
        //-----------------------------------
        if(F_Service_CIR)
        {
            F_Service_CIR=0;
            service_cir();
            continue;
        }

        //-----------------------------------
        // fourth command/data service
        //-----------------------------------
        if(F_Service_PCI4)
        {
            F_Service_PCI4=0;
            service_pci4();
            continue;
        }

        //------------------------------------
        // service_OEM_1
        //------------------------------------
        if(F_Service_OEM_1)
        {
            F_Service_OEM_1=0;
            service_OEM_1();
            continue;
        }

        //------------------------------------
        // service_OEM_2
        //------------------------------------
        if(F_Service_OEM_2)
        {
            F_Service_OEM_2=0;
            service_OEM_2();
            continue;
        }

        //------------------------------------
        // service_OEM_3
        //------------------------------------
        if(F_Service_OEM_3)
        {
            F_Service_OEM_3=0;
            service_OEM_3();
            continue;
        }

        //------------------------------------
        // service_OEM_4
        //------------------------------------
        if(F_Service_OEM_4)
        {
            F_Service_OEM_4=0;
            service_OEM_4();
            continue;
        }

        //-----------------------------------
        //
        //-----------------------------------
        Hook_main_service_L();
    }
}

static void service_Low_LVEvent(void)
{
    if((KBPendingTXCount != KBPendingRXCount )||(scan.kbf_head != scan.kbf_tail))
    {
	     SetServiceSendFlag();
         if(IS_MASK_SET(BATTUPDATEFW,PriBattInhib)) //ANGELAG008:add
		 	F_Service_SEND = 1;                     //ANGELAG008:add
    }

    if(IS_MASK_SET(KBHISR,P_IBF))
    {
  	    F_Service_PCI = 1;
    }

    if(IS_MASK_SET(PM1STS,P_IBF))
    {
  	    F_Service_PCI2 = 1;
    }
}

//----------------------------------------------------------------------------
// FUNCTION: service_unlock
// Unlock aux devices and re-enable Host interface IRQ if it is ok to do so.
//----------------------------------------------------------------------------
static void service_unlock(void)
{
	Unlock_Scan();
}

//------------------------------------------------------------
// Polling events
//------------------------------------------------------------
void service_1mS(void)
{
    Timer1msEvent();
    Timer1msCnt++;
    if(Timer1msCnt>=10)
    {
        Timer1msCnt = 0x00;
    }

    if(Hook_Only_Timer1msEvent()==Only_Timer1msEvent)
    {
        return;
    }
	//72JERRY076: S+Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
    if((Timer1msCnt%2)==0x00)
    {
      Timer2msEvent();
      Timer2msCnt++;
      if(Timer2msCnt & 1)
      {
          Timer4msEventA();
      }
      else
      {
        Timer4msEventB();
        switch(Timer2msCnt)
        {
          case 2:
             Timer20msEventA();
          break;

          case 4:
              //Timer20msEventB();
          break;

          case 6:
             //Timer20msEventA();
          break;

          case 8:
              //Timer20msEventB();
          break;
          default:
              Timer2msCnt=0;
            break;
        }
      }
    }
	//72JERRY076: E+Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
    if((Timer1msCnt%5)==0x00)
    {
	    Timer5msEvent();
	    Timer5msCnt++;
	    if ( Timer5msCnt & 1 )  	// 10ms events //1 5ms---3 5ms ---5 5ms
	    {
            Timer10msEventA();
        
	    }
	    else
	    {
		    Timer10msEventB();
     	    switch( Timer5msCnt )   // Share Loading Branch Control
    	    {
       		case 2: 
                Timer50msEventA();
              	break;
          	case 4: 
                Timer50msEventB();
             	break;
        	case 6: 
               	Timer50msEventC();
              	break;
          	case 8: 
                Timer100msCntB++;
         		if ( Timer100msCntB & 1 )
             	{
                  	Timer100msEventA();
              	}
             	else
             	{
                   	Timer100msEventB();
              	}
               	break;

           	default:
				Timer5msCnt=0;
              	break;
     	   	}

    	    if ( Timer5msCnt == 0x00 )
    	    {       			// 50msec
          	    Timer100msCnt ++;
          	    if ( Timer100msCnt & 1 )
         	    {
             	    Timer100msEventC();
          	    }
         	    else
     		    {       		

          		    switch( Timer100msCnt )
              	    {
                	case 2:	
                        Timer500msEventA();
                 		break;
                 	case 4:	
                        Timer500msEventB();
                      	break;
                 	case 6:	
                        Timer500msEventC();
                   		break;
                 	case 8:	
                        Timer1SecEventA();
                     	break;
				    case 10: 	
                        Timer1SecEventB();
                     	break;
                 	case 12:	
                        Timer500msEventA();
                      	break;
                	case 14:	
                        Timer500msEventB();
                    	break;
               		case 16: 	
                        Timer500msEventC();
                      	break;
                	case 18: 	
                        Timer1SecEventC();
                  		break;
                  	default:        // 1 Sec
                     	Timer100msCnt = 0;
                  		Timer1SecCnt ++;
                    	if ( Timer1SecCnt == 60 )
                      	{
                         	Timer1MinEvent();
                         	Timer1SecCnt=0;
                     	}
                    	break;
          		    }
              	}
       		}
   		}
	}
}

//------------------------------------------------------------
// 1ms events
//------------------------------------------------------------
void Timer1msEvent(void)
{
    ReSendPS2PendingData();
    Hook_Timer1msEvent(Timer1msCnt);
}
//72JERRY076:S+ Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
//------------------------------------------------------------
// 2ms events
//------------------------------------------------------------
void Timer2msEvent(void)
{
    Hook_Timer2msEvent();
}
//------------------------------------------------------------
// 4ms events
//------------------------------------------------------------
void Timer4msEventA(void)
{
    Hook_Timer4msEventA();
}
//------------------------------------------------------------
// 4ms events
//------------------------------------------------------------
void Timer4msEventB(void)
{
    Hook_Timer4msEventB();
}
//------------------------------------------------------------
// 20ms events
//------------------------------------------------------------
void Timer20msEventA(void)
{
    Hook_Timer20msEventA();
}

//72JERRY076: E+Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
//------------------------------------------------------------
// 5ms events
//------------------------------------------------------------
void Timer5msEvent(void)
{
    F_Service_Low_LV = 1;
	if (Timer_A.fbit.TMR_SCAN)
	{
  		F_Service_KEY = 1;		// Request scanner service.
  	}
       Hook_Timer5msEvent();
}

//------------------------------------------------------------
// 10ms events
//------------------------------------------------------------
void Timer10msEventA(void)
{
    SetStartScanAUXFlag();
	Hook_Timer10msEventA();
}

//------------------------------------------------------------
// 10ms events
//------------------------------------------------------------
void Timer10msEventB(void)
{
	Hook_Timer10msEventB();
}

//------------------------------------------------------------
// 50ms events A
//------------------------------------------------------------
void Timer50msEventA(void)
{
	Hook_Timer50msEventA();
}

//------------------------------------------------------------
// 50ms events B
//------------------------------------------------------------
void Timer50msEventB(void)
{
	Hook_Timer50msEventB();
}

//------------------------------------------------------------
// 50ms events C
//------------------------------------------------------------
void Timer50msEventC(void)
{
	Hook_Timer50msEventC();
}

//------------------------------------------------------------
// 100ms events A
//------------------------------------------------------------
void Timer100msEventA(void)
{
	Hook_Timer100msEventA();
}

//------------------------------------------------------------
// 100ms events B
//------------------------------------------------------------
void Timer100msEventB(void)
{
	Hook_Timer100msEventB();
}

//------------------------------------------------------------
// 100ms events C
//------------------------------------------------------------
void Timer100msEventC(void)
{
	Hook_Timer100msEventC();
}

//------------------------------------------------------------
// 500ms events A
//------------------------------------------------------------
void Timer500msEventA(void)
{
	Hook_Timer500msEventA();
}

//------------------------------------------------------------
// 500ms events B
//------------------------------------------------------------
void Timer500msEventB(void)
{
	Hook_Timer500msEventB();
}

//------------------------------------------------------------
// 500ms events C
//------------------------------------------------------------
void Timer500msEventC(void)
{
	Hook_Timer500msEventC();
}

//------------------------------------------------------------
// 1sec events A
//------------------------------------------------------------
void Timer1SecEventA(void)
{
	Hook_Timer1SecEventA();
}

//------------------------------------------------------------
// 1sec events B
//------------------------------------------------------------
void Timer1SecEventB(void)
{
	Hook_Timer1SecEventB();
}

//------------------------------------------------------------
// 1sec events C
//------------------------------------------------------------
void Timer1SecEventC(void)
{
	Hook_Timer1SecEventC();
}

//------------------------------------------------------------
// 1min events
//------------------------------------------------------------
void Timer1MinEvent(void)
{
    Hook_Timer1MinEvent();
}

bit CheckCanEnterDeepSleep()
{
  	//under S5&S3 battery mode  EC enter sleep mode start
	BYTE resault;
	resault = 0x00;
	//if(Read_AC_IN()) 
	//{
		//resault = 0x01;
	//}//72JERRY020:- Modify enter EC sleep setting.
	if(SystemIsS0) 							 // if system in S0
	{
		resault = 0x01;
	}
	//MARTINH154:Add  start
	if(IS_MASK_SET(CMOS_TEST,b0_CMOS_FunctionKey))							  // if system in S0
	{
		resault = 0x01;
	}
	//MARTINH154:Add end
	//ANGELAS044:add start
	if(Read_EC_PWRBTN())		                       
	{	
		resault = 0x01;
	}
	if(LOWBATT_3TIMES!=0)
	{
		resault=0x01;
	}
	if(IS_MASK_SET(ACPI_HOTKEY, b6Cmd_NoShut)||IS_MASK_SET(ACPI_HOTKEY, b7BIOS_NoShut))//72JERRY020: Modify enter EC sleep setting.
	{
		resault = 0x01;
	}
	//ANGELAS067:add start
	//if((SystemIsS5||SystemIsDSX)&&(!Read_AC_IN())&&( IS_MASK_CLEAR(ACPI_HOTKEY, b7BIOS_NoShut))
		//&&( IS_MASK_CLEAR(ACPI_HOTKEY, b6Cmd_NoShut)))
	//{
		//resault = 0x00;//72JERRY016: Enable EC sleep mode
	//}//72JERRY020:- Modify enter EC sleep setting.
	//ANGELAS067:add end
	if(Read_AC_IN()&&(IS_MASK_CLEAR(nBattery0x16L,FullyChg))&&(IS_MASK_SET(BT1_STATUS1, bat_in)))
	{
		resault=0x01;
	}
	if(LID_DEBOUNCE_CNT>0)//W127
	{
		resault=0x01;
	}
    //ANGELAS044:add end
   	//ANGELAS081:remove start
   	//if(Read_LID_SW_IN()&& IS_MASK_SET(SysStatus,CloseLid))//G83:Fixed cannot resume from S3 by open lid after enter S3 via close lid on DC mode.
    //{
    //	resault = 0x01;
    //}
    //ANGELAS081:remove end
	if(IS_MASK_SET(POWER_FLAG1, wait_PSW_off))	// power switch pressed
	{
		resault = 0x01;
	}
	 /*ANGELAS057:remove start
	if(SystemIsS3) //ANGELAS044:s5 to s3
	{
		resault = 0x01;
	}
	*///ANGELAS057:remove end
	if((SysPowState==SYSTEM_S5_S0)||(SysPowState==SYSTEM_S4_S0)||(SysPowState==SYSTEM_S3_S0)||(SysPowState==SYSTEM_S0_S5)||(SysPowState==SYSTEM_S0_S4)||(SysPowState==SYSTEM_S0_S3)||(SysPowState == SYSTEM_S5_DSX)||(SysPowState == SYSTEM_DSX_S5))//72JERRY020: Modify enter EC sleep setting.
	{
		resault = 0x01;
	}

	if(resault == 0x00)
	{
		if(DeepSleepCunt < 250)		  // Delay 2500 ms for EC deep sleep mode////G83
		{
			DeepSleepCunt++;
			resault = 0x02;
		}
		else
		{
			resault = 0x00;
			DeepSleepCunt = 0x00;
		}
	}
	else
	{
		DeepSleepCunt = 0x00;
	}

	switch(resault)
	{
		case 0:
			return(1);
			break;

		case 1:
			DeepSleepCunt=0x00;
			return(0);
			break;

		case 2:
			return(0);
			break;
	}

	return 0;	
//under S5 battery mode  EC enter deep sleep mode end

}

//-----------------------------------------------------------------------------
void InitEnterDeepSleep(void)
{
    EC_DeepSleep_Temp0 = ADCSTS;
    EC_DeepSleep_Temp1 = ADCCFG;
    EC_DeepSleep_Temp2 = DACPWRDN;
    EC_DeepSleep_Temp3 = CGCTRL1R;

    EC_DeepSleep_TempPortA = GPDRA;
    EC_DeepSleep_TempPortB = GPDRB;
    EC_DeepSleep_TempPortC = GPDRC;
    EC_DeepSleep_TempPortD = GPDRD;
    EC_DeepSleep_TempPortE = GPDRE;
    EC_DeepSleep_TempPortF = GPDRF;
    EC_DeepSleep_TempPortG = GPDRG;
    EC_DeepSleep_TempPortH = GPDRH;
    EC_DeepSleep_TempPortI = GPDRI;
    EC_DeepSleep_TempPortJ = GPDRJ;

  	EA = 0;
	KSOL	= 0x00;
	KSOH1	= 0x00;
	KSOH2	= 0x00;
	KSICTRL = 0x00;

	//MEILING030:S-remove change LED control mode.
	/*if(SystemIsS3)
	{
		GPCRA0 = ALT; 
		DCR0 = 0x7F; 
	}*/
	//MEILING030:S-remove change LED control mode.
	
	
	if(SystemIsDSX)
	{
	//	BAT_LOW_LED_ON();
	//	BAT_CHG_LED_ON(); //HEGANGS009:Enable ec sleep and modify DC-S5 gpio setting
		PWR_WHITELED_ON(); //HEGANGS042:Modify the led behavior follow new spec
		PWR_AMBERLED_ON();
		//NUMLED_ON();
		CAPLED_ON();
		//GPCRA0=INPUT;//72JERRY035: Modify EC sleep GPIO setting.
		//GPCRA1=INPUT;	//72JERRY035: Modify EC sleep GPIO setting.	
		//GPCRA2=INPUT;//72JERRY035: Modify EC sleep GPIO setting.
	}
	
	FPCFG&=0xBF;
	ADCCFG &= 0xFE; 
	CGCTRL2R = 0x70;
	CGCTRL3R = 0x2F; 
	
	//if(!(SystemIsS5||SystemIsDSX)) //72JERRY020: Modify enter EC sleep setting.
      	ECPowerDownEnableExternalTimer2(); 
		//72JERRY020:s+ Modify enter EC sleep setting.
	if(IS_MASK_SET(BT1_STATUS1, bat_in))	// battery Present
	{
		BatteryOutWakeEnable();
	}
	else
	{
		BatteryINWakeEnable();
	}
	//72JERRY020:e+ Modify enter EC sleep setting.
	//SetACIN_Int();
	ACInOutIntEnable();
    Setlanwake_Int();//Modify sometimes will wake up from S3 when plug AC adaptor after sending magic-package under DC mode.
	SetPWRSW_Int();
    SetNovo_Int();
	SetWOV_Int();//72JERRY028: Modify Voice Wake signal status in deep sleep.
    MXLID_Wake_En();
    SlpS3_Wake_En();
	InterKBDWakeEnable();

	ADCSTS	= 0x00;
	ADCCFG	= 0x00;
	DACPWRDN = 0xFF;
    CGCTRL1R = 0x0F;

	if(SystemIsDSX||SystemIsS5)//72JERRY020: Modify enter EC sleep setting.
	{ 
		//LMLKBL0015:add start.
		SMBUS_CK1_HIGH;
		SMBUS_DA1_HIGH;
		SMBUS_CK1_IN;
		SMBUS_DA1_IN;
		//LMLKBL0015:add end.

		//PM_PWRBTN_LOW(); //72JERRY035: Modify EC sleep GPIO setting.
		
		GPCRA5=INPUT; 
		//GPCRB6=INPUT;		//72JERRY035: Modify EC sleep GPIO setting.
		//GPCRC0=INPUT;			
		GPCRC7=INPUT;		
		//GPCRD3=INPUT;		
		//GPCRE3=INPUT;		
		//GPCRE7=INPUT; //LMLNANO013:remove.	
		GPCRG6=INPUT;	
		//GPCRH3=INPUT; //LMLNANO013:remove.
		//GPCRJ3=INPUT;
		GPCRJ4=INPUT;
		GPCRJ6=INPUT;
	} 

	WUESR1	= 0xFF;
	WUESR2	= 0xFF;
	WUESR3	= 0xFF;
	WUESR4	= 0xFF;
	WUESR7	= 0xFF;
	WUESR10	= 0xFF; // AC in
	WUESR11	= 0xFF;//72JERRY028: Modify Voice Wake signal status in deep sleep.
	WUESR14	= 0xFF; // Novo
	WUESR13	= 0xFF; // battery//72JERRY020: Modify enter EC sleep setting.

	ISR1 = 0xFF;
	ISR2 = 0xFF;
	ISR3 = 0xFF;
	ISR4 = 0xFF;
	ISR13= 0xFF; // AC in
	ISR14= 0xFF;//72JERRY028: Modify Voice Wake signal status in deep sleep.
	ISR16= 0xFF; // Novo
	ISR15= 0xFF; // battery//72JERRY020: Modify enter EC sleep setting.
	
}

//-----------------------------------------------------------------------------
void InitWakeFromDeepSleep(void)
{
   	FPCFG|=0x40;
   	ADCCFG |= 0x01;
    CGCTRL2R = 0x00;
    CGCTRL3R = 0x00;
	
	IER0 	= 0x00;  // 7 ~ 0
	IER1 	= 0x00;  // 15 ~ 8
	IER2 	= 0x00;  // 23 ~ 16
	IER3 	= 0x00;
	IER4  	= 0x00;
    IER9    = 0x00;
	IER13	= 0x00; // AC in
	IER16   = 0x00; // Novo
	IER14   = 0x00;//72JERRY028: Modify Voice Wake signal status in deep sleep.
	IER15   = 0x00; // battery//72JERRY020: Modify enter EC sleep setting.



 	ISR0  	= 0xFF;
 	ISR1  	= 0xFF;
 	ISR2 	= 0xFF;
 	ISR3  	= 0xFF;
	ISR4	= 0xFF;
    ISR9    = 0xFF;
    ISR13   = 0xFF; // AC in
    ISR14   = 0xFF;//72JERRY028: Modify Voice Wake signal status in deep sleep.
    ISR16   = 0xFF; // Novo
    ISR15= 0xFF; // battery//72JERRY020: Modify enter EC sleep setting.


	WUESR1	= 0xFF;
	WUESR2	= 0xFF;
	WUESR3	= 0xFF;
	WUESR4	= 0xFF;
	WUESR7	= 0xFF;
	WUESR10	= 0xFF; // AC in
	WUESR11	= 0xFF;//72JERRY028: Modify Voice Wake signal status in deep sleep.
	WUESR14	= 0xFF; // Novo
	WUESR13	= 0xFF; // battery//72JERRY020: Modify enter EC sleep setting.

    ADCSTS = EC_DeepSleep_Temp0;
    ADCCFG = EC_DeepSleep_Temp1;
    DACPWRDN = EC_DeepSleep_Temp2;
    CGCTRL1R = EC_DeepSleep_Temp3;

	Init_Kbd();
	Core_Init_Regs();
	Init_Regs();
	Enable_Any_Key_Irq();	//msmart

	//MEILING030:S- remove change power led control.
	/*if(SystemIsS3)
	{
		GPCRA0 = OUTPUT; 
	}*/
	//MEILING030:E-
	
	if(SystemIsDSX||SystemIsS5)//72JERRY020: Modify enter EC sleep setting.
	{
		//LMLKBL0015:add start.
		SMBUS_CK1_ALT;
		SMBUS_DA1_ALT;
		//LMLKBL0015:add end.
		
		GPCRA0=OUTPUT;
		GPCRA1=OUTPUT; //MEILING030:add.
		GPCRA2=OUTPUT;
		BAT_LOW_LED_OFF();
		PWR_WHITELED_OFF();//HEGANGS042:Modify the led behavior follow new spec
		PWR_AMBERLED_OFF();
		//NUMLED_OFF();
		CAPLED_OFF();
		BAT_CHG_LED_OFF();
		//PM_PWRBTN_HI(); //72JERRY020: Modify enter EC sleep setting.//72JERRY041:Modify PWRBT signal in DC mode setting to low.
	
		//GPCRA3=OUTPUT;		//72JERRY027: Modify KB backlight signal in deep sleep.
		GPCRA4=OUTPUT;	
		GPCRA5=OUTPUT;
		GPCRA7=OUTPUT;		
		GPCRB2=OUTPUT;		
		//GPCRB6=OUTPUT;	//72JERRY035: Modify EC sleep GPIO setting.	
		GPCRC0=OUTPUT;		
		GPCRC6=OUTPUT;		
		GPCRC7=OUTPUT;		
		GPCRD3=OUTPUT;		
		GPCRE3=OUTPUT;		
		GPCRE4=OUTPUT;
		VR_ON_OFF();
		//GPCRE7=OUTPUT;	//LMLNANO013:remove.	
		GPCRF1=OUTPUT;	
		//GPCRG0=OUTPUT;		 //ANGELAN007 remove
		//GPCRG6=OUTPUT;	
		//GPCRH3=OUTPUT;	//LMLNANO013:remove.
		GPCRH6=OUTPUT;	
		GPCRJ3=OUTPUT;
		GPCRJ4=OUTPUT;
		GPCRJ6=OUTPUT;	
	}	 
}

//----------------------------------------------------------------------------
// Output : 1 Crisis mode
//          0 Normal mode
//----------------------------------------------------------------------------
bit CheckCrisisMode(void)
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

	KSOL  = 0xEF;
	KSOH1 = 0xFF;
	KSOH2 = 0xFF;			// "R" key (scan line b4)
	WNCKR = 0x00;           // Delay 15.26 us
	WNCKR = 0x00;           // Delay 15.26 us
	if ((KSI&0x04) != 0)	// "R" key (data line b2)
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
