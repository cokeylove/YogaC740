/*-----------------------------------------------------------------------------
 * TITLE: OEM_EVENT.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//-----------------------------------------------------------------------------
// no change function

//-----------------------------------------------------------------------------
void NullEvent(void)
{

}

/*
*******************************************************************************
* Function name: AdapterIn       
*
* Descriptoin: Adapter in function.
*
* Invoked by: EventManager()
*
* TimeDiv:NA
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/
void AdapterIn(void)
{
	PECI_SETPL2Watts(2);
	
	
	AC_PRESENT_HI(); 
	ACInOutIntEnable(); 
	Unlock_ShipMode();  
	SET_MASK(SYS_STATUS,AC_ADP);	//set AC flag
	if( IS_MASK_SET(nBatteryStatus1, ENEXIST) )  //add check battery state
	{
		Chk_Battery_Full();
	}

   	ECSMI_SCIEvent(ACPI_ACIN_SCI);
   	if( SystemIsS0 )
	{
		PWM_BEEP = 0x7F;			// Out beep
		PWM_LEDBeep_CNT = 0x0A;		// 50ms*10
	}

	InitChargerIC();//Change charge IC option setting.
	BatSMbusFailCount = 0;
	nBattErrorCnt = 0;
	// Set 0x12h and 0x3Fh before set ChargeCurrent and ChargeVoltage.
	CHGIC_ptr = 1;
	WriteSmartChgIC();
	WriteSmartChgIC();
	CHGIC_ptr = 3;
	ReadSmartChgIC();
    if (IS_MASK_SET(SYS_STATUS,AC_ADP)) 
    {
	    AdapterIDOn_Flag = 0x01; 
        ADPIDON10MS_NUM =0x0A; //change 0x05 to 0x0A to avoid read dirty data.
    }    
	if(SystemIsS4||SystemIsS5||SystemIsDSX)
	{
		S4S5_KeyWake(2);//HEGANGS021:one key wake
	}
}

/*
*******************************************************************************
* Function name: AdapterOut       
*
* Descriptoin: Adapter out function.
*
* Invoked by: EventManager()
*
* TimeDiv:NA
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/
void AdapterOut(void)
{
	AdapterIDOn_Flag = 0x00;
	AdapterID=0x00;
	TYPEC_Adapter_Watts=0x00;
		
	BATT_LEN_OFF();  //remove AC_OFF function follow charge IC bq24780.
	AC_PRESENT_LOW(); 
	ACInOutIntEnable(); 
    CLEAR_MASK(SYS_STATUS,b4IllegalAdp);
	CLEAR_MASK(SYS_STATUS,b5UnSuitAdpPow);
	SetVGA_AC_DET();
	PECI_SETPL2Watts(1);

	CLEAR_MASK(SYS_STATUS,AC_ADP);  //clear battery flag
	Chk_Battery_Full();
   	ECSMI_SCIEvent(ACPI_ACOUT_SCI);

	//Set P-State highest to force that ChkBattery_OCP will decress P-State step by step.
	SET_MASK(BatteryAlarm,BATOCP);
	ThrottlingControl(); 

   	if( SystemIsS0 )
	{
		PWM_BEEP = 0x7F;			// Out beep
		PWM_LEDBeep_CNT = 0x0A;		// 50ms*10


        if(IS_MASK_SET(LENOVOPMFW,BATTERY_MAIN_CAL))
        {
		    uVPCeventSource = General;
		    uVPCeventSource2 = 0;
		    ECSMI_SCIEvent(SDV_VPC_notify);
        }
	}
//Modify the USB mouse can wake up from S3 at DC mode when the LID closed,not follow UI SPEC.
	if((SystemIsS3)&&(!Read_LID_SW_IN()))
	{
		USB_ON_INPUT;
	}
		//Modify the USB mouse can wake up from S3 at DC mode when the LID closed,not follow UI SPEC.
	nAvgCurrentL = 0;
	nAvgCurrentH = 0;
	Bat0x0BFakeCnt = 60;
    DSxPowState = SYSTEM_S3S4;
    ADPIDON10MS_NUM=0xFF; 
	InitChargerIC();
}


/*
*******************************************************************************
* Function name: PSWPressed       
*
* Descriptoin: power button was pressed.
*
* Invoked by: EventManager()
*
* TimeDiv:NA
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/
void PSWPressed(void)
{
	if ( IS_MASK_SET(cCmd, b6TestBtnEn) )
	{
		Buffer_Key(0xE0);		//
		Buffer_Key(0x25);		//
		F_Service_SEND = 1;
	}

	ECSleepCount = 3;

	//Clear PswPressedCounter
	CountSecAfterPswPressed = 0;

	CLEAR_MASK(SysStatus2,PWRBTN_press_retry);

	//Add ignore power button press when lid close.
	if(!Read_LID_SW_IN())
	{ 
		return; 
	}
	//
	
    switch (SysPowState)
    {
    case SYSTEM_S4 :
    	break;
        
    case SYSTEM_DSX:
    case SYSTEM_S5 :
		//S+Modify code for befor power on check Battery RSOC in DC mode.
		if(!Read_AC_IN()||IS_MASK_SET(ACOFF_SOURCE, BATTLEARN))
		{			 	
			BatSMbusFailCount = 0;
			if(!bRWSMBus(SMbusChB, SMbusRW, SmBat_Addr, C_RSOC, &BAT1PERCL,SMBus_NoPEC))
			{
				BatSMbusFailCount++;	
				while((BatSMbusFailCount<=4)) 
				{		
					if(!bRWSMBus(SMbusChB, SMbusRW, SmBat_Addr, C_RSOC, &BAT1PERCL,SMBus_NoPEC))
					{
						BatSMbusFailCount++;
					}
					else
			 		{
						BatSMbusFailCount=0;
			            break; 
					}
				}
			 				  			 			 
			}
			if(BatSMbusFailCount>=4) 
				break;
			
			if (BAT1PERCL < 0x02)  //modify battery capacity point from 0 to 1 for fliker.
			{
				SET_MASK(nBatteryStatL,CMBS_LOWBATT);
				LOWBATT_3TIMES = 100;		// set 5 sec
				break;
			}
		}

		if ((!Read_AC_IN()))			// if DC mode
		{
			BatSMbusFailCount = 0;
			nBattErrorCnt = 0;
			if (IS_MASK_SET(LENOVOBATT,BATTERY_LEGAL))	//SHA1 fail
			{
				Service_Auth_Step = 1;				// start SHA1 again
				SHA1failCnt = 1;
			}
		}
        if(IS_MASK_SET(SysStatus2, b3EC8S_Reset))
        {
            break;
        }
		uNovoVPCCount = 0;
		ONEKEY_FLAG = 0;//HEGANGS021:one key wake
		PWSeqStep = 1;
        PowSeqDelay = 0x00;
        RamDebug(0x51);
		SysPowState=SYSTEM_S5_S0;
		break;

    case SYSTEM_S3 :
	case SYSTEM_S0 :
		RamDebug(0xF4);
		if (IS_MASK_CLEAR(pProject4,pPWSWdisable)) //HEGANGS028:Disable POWERSW when flash bios
		{
			PM_PWRBTN_LOW();
		}   
		SET_MASK(SysStatus2,PWRBTN_press_retry);//Modify press button 1s enter S3/S4/shutdown follow UI spec.
		
		break;

    default :
       	break;
    }
}

/*
*******************************************************************************
* Function name: PSWReleased       
*
* Descriptoin: power button was released.
*
* Invoked by: EventManager()
*
* TimeDiv:NA
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/
void PSWReleased(void)
{
	CLEAR_MASK(F2_Pressed, F12Flag);// HEGANGS034:Modify onekey battery function
	CLEAR_MASK(F2_Pressed, F2Flag); 
	NOVO_COUNTER = T_PSWOFF;//Hang check
	ONEKEY_FLAG=0;	//HEGANGS025: Modify onekey battery function
	//ONEKEY_TEMP_FLAG = 0;
	if(ONEKEY_TEMP_FLAG)
	SET_MASK(OEMControl,b3ONEKEY_POWERUP);
	if ( IS_MASK_SET(cCmd, b6TestBtnEn) )
	{
		Buffer_Key(0xE0);	//
		Buffer_Key(0xF0);	//
		Buffer_Key(0x25);	//
		F_Service_SEND = 1;
	}
    
    if(IS_MASK_SET(SysStatus2, b3EC8S_Reset)) 
	{
		CLEAR_MASK(SysStatus2, b3EC8S_Reset);
	}
      PWRBTN_press_debounce=0;//Modify press button 1s enter S3/S4/shutdown follow UI spec.
	  CLEAR_MASK(SysStatus2,PWRBTN_press_retry);   //Modify press button 1s enter S3/S4/shutdown follow UI spec.           

    if(!Read_LID_SW_IN())
	{ 
		return; 
	}


	PSW_COUNTER = T_PSWOFF;	// initialize counter
	switch (SysPowState)
	{
	case SYSTEM_S4 :
		break; 

	case SYSTEM_S5 :
		break;

	case SYSTEM_S3 :
	case SYSTEM_S0 :
		PM_PWRBTN_HI();
		//ECQEvent(ACPI_PWRBTN_SCI); //Send POWER Button Low Event when Release Power button
		break;

	default :
		break;
	}
}

/*
*******************************************************************************
* Function name: PSWOverrided       
*
* Descriptoin: power button was overrided.
*
* Invoked by: EventManager()
*
* TimeDiv:NA
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/
void PSWOverrided(void)
{
	if(!Read_LID_SW_IN())
	{ return; }
    if (Read_EC_PWRBTN()&&Read_EC_NOVO())
    { 
		return; 
	}
	PowerBottonEvent();//Modify press button 1s enter S3/S4/shutdown follow UI spec.
}


/*
*******************************************************************************
* Function name: NOVOPressed       
*
* Descriptoin: NOVO button was pressed.
*
* Invoked by: EventManager()
*
* TimeDiv:NA
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/
void NOVOPressed(void)
{
	if ( IS_MASK_SET(cCmd, b6TestBtnEn) )
	{
		Buffer_Key(0xE0);	//
		Buffer_Key(0x49);	// page down make
		F_Service_SEND = 1;
	}

	ECSleepCount = 3;

	CLEAR_MASK(SysStatus2,b1NOVOBTN_retry);

    if(!Read_LID_SW_IN())
	{ return; }

	//Clear NOVOPressedCounter
	CountSecAfterNOVOPressed = 0;

	switch (SysPowState)
    {
	case SYSTEM_S4 :
    case SYSTEM_S5 :
    case SYSTEM_DSX:
		//Modify code for befor power on check Battery RSOC in DC mode.
		if(!Read_AC_IN()||IS_MASK_SET(ACOFF_SOURCE, BATTLEARN))
		{			 	
			BatSMbusFailCount = 0;
			if(!bRWSMBus(SMbusChB, SMbusRW, SmBat_Addr, C_RSOC, &BAT1PERCL,SMBus_NoPEC))
			{
				BatSMbusFailCount++;	
				while(BatSMbusFailCount <=4) 
				{		
					if(!bRWSMBus(SMbusChB, SMbusRW, SmBat_Addr, C_RSOC, &BAT1PERCL,SMBus_NoPEC))
					{
						BatSMbusFailCount++;										 
					}
					else
			 		{
						BatSMbusFailCount=0;
			 			break;
					}
				}
			 				  			 			 
			}
			if(BatSMbusFailCount>=4) 
			 	break;
			
			if (BAT1PERCL < 0x02)  //modify battery capacity point from 0 to 1 for fliker.
			{
				SET_MASK(nBatteryStatL,CMBS_LOWBATT);
				LOWBATT_3TIMES = 100;		// set 5 sec
				break;
			}
		}
		//Modify code for befor power on check Battery RSOC in DC mode.
		if(LOWBATT_3TIMES == 0)
        {
          	// NOVO button should be no function when resuming from S4
	        if( (EEPROM_PwrSts & 0x10) == 0 )
	        {
				if ( (EEPROM_PwrSts & 0x01) != 0 )
				{ 
				    break; 
                }
	        }
			uNovoVPCCount = 1;
			PWSeqStep = 1;
			PowSeqDelay = 0x00;
            RamDebug(0x52);
			SysPowState=SYSTEM_S5_S0;
		}
        break;

	case SYSTEM_S3 :
      	break;
	case SYSTEM_S0 :
		// NOVO button should initialize ONEKEY Recovery Windows Module,
		// or be no function if ONEKEY Recovery Windows Module hasn't been installed
		if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
		{
			uNovoVPCCount++;
			if( uNovoVPCCount > (10+1) )
				{ uNovoVPCCount--; }

		}
    default :
        break;
    }
}


/*
*******************************************************************************
* Function name: NOVOReleased       
*
* Descriptoin: NOVO button was released.
*
* Invoked by: EventManager()
*
* TimeDiv:NA
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/
void NOVOReleased(void)
{
	NOVO_COUNTER = T_PSWOFF;//Hang check
	ONEKEY_FLAG = 0;
	ONEKEY_TEMP_FLAG = 0; //HEGANGS025: Modify onekey battery function
	if ( IS_MASK_SET(cCmd, b6TestBtnEn) )
	{
		Buffer_Key(0xE0);	//
		Buffer_Key(0xF0);	//
		Buffer_Key(0x49);	// page down make
		F_Service_SEND = 1;
	}

    if(!Read_LID_SW_IN())
	{ return; }

	switch (SysPowState)
	{
	case SYSTEM_S4 :

	case SYSTEM_S5 :

	case SYSTEM_S3 :
		break;
	case SYSTEM_S0 :
		if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
		{
			if( uNovoVPCCount > 10 )		// Delay 100 mSec.
		    { uNovoVPCCount = 1; }
		    else
			{ uNovoVPCCount = 0; }
			uVPCeventSource = VPCeventNovo; // bit3:NOVO.
		    uVPCeventSource2 = 0;
			ECQEvent(SDV_VPC_notify);		// 0x44 for Levono used.
		}
	default :
		break;
	}
}

/*
*******************************************************************************
* Function name: NOVOOverrided       
*
* Descriptoin: NOVO button was overrided.
*
* Invoked by: EventManager()
*
* TimeDiv:NA
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/
void NOVOOverrided(void)
{
	//HEGANGS022:Add SMI hang check function
	if (!Read_EC_NOVO())
	{
		return; 
	}
	if(NOVO_COUNTER != 0)
	{
		NOVO_COUNTER--;
		return;
	}
	NOVO_COUNTER = T_PSWOFF;
	ECSMIEvent();
//HEGANGS022:Add SMI hang check function
}

/*
*******************************************************************************
* Function name: LanWakeLow       
*
* Descriptoin: LAN_WAKE was low.
*
* Invoked by: EventManager()
*
* TimeDiv:NA
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/
void LanWakeLow(void)
{
	#if	Support_EC_LANWake

	switch (SysPowState)
	{
	case SYSTEM_S4 :
		break;

	case SYSTEM_S5 :
		break;

	case SYSTEM_S3 :
		if ( IS_MASK_SET(AOAC_STATUS, ISCT_Enable) )	// Check ISCT enable?
		{
			PM_PWRBTN_LOW();
			AOAC_STATUS |= 0x80;	// Set PME(LAN) wake function.
		}
		else
		{
			if ( IS_MASK_SET(SYS_STATUS,AC_ADP) )
 			{ 
 				RamDebug(0x99);
				//PCIE_WAKE_OUTPUT;
				//PCIE_WAKE_LOW();
            } // Set PME(LAN) wake function.
		}
		break;

	case SYSTEM_S0 :
		break;

	default :
		break;
	}
	#endif	// Support_EC_LANWake
}

/*
*******************************************************************************
* Function name: LanWakeHi       
*
* Descriptoin: LAN_WAKE was Hi.
*
* Invoked by: EventManager()
*
* TimeDiv:NA
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/
void LanWakeHi(void)
{
	#if	Support_EC_LANWake

	switch (SysPowState)
	{
	case SYSTEM_S4 :
		break;

	case SYSTEM_S5 :
		break;

	case SYSTEM_S3 :
		PM_PWRBTN_HI();
        RamDebug(0x9A);
		//PCIE_WAKE_HI();
		//PCIE_WAKE_INPUT;
		break;

	case SYSTEM_S0 :
		PM_PWRBTN_HI();
		//PCIE_WAKE_HI();
		//PCIE_WAKE_INPUT;
		break;

	default :
		PM_PWRBTN_HI();
	//	PCIE_WAKE_HI();
	//	PCIE_WAKE_INPUT;
		break;
	}
	#endif	// Support_EC_LANWake
}
//-----------------------------------------------------------------------------
// VoiceWakeSystem(): When mic input voice string match "xxx", Realtek chip 
// notify EC and EC wake up system from s3/S4/S5 except modern standby(s3).
//Voice wake system event function
//-----------------------------------------------------------------------------
void VoiceWakeSystem(void)
{
 if(cDev.fbit.cD_WovMFGtest==1)//enable wov MFG test
 	{
		return;
 	}
 	//:Add CMD 0x59 test voice wake up for MFG.
	switch(SysPowState)
	{
	  case SYSTEM_S5:
	  case SYSTEM_DSX:// Modify Voice Wake system method,Add DC S5 wake up status.
	  //Required to power on at battery RSOC below 2% in S5/S4 follow UI spec.
	  if(!Read_AC_IN()||IS_MASK_SET(ACOFF_SOURCE, BATTLEARN))
		{			 	
			BatSMbusFailCount = 0;
			if(!bRWSMBus(SMbusChB, SMbusRW, SmBat_Addr, C_RSOC, &BAT1PERCL,SMBus_NoPEC))
			{
				BatSMbusFailCount++;	
				while((BatSMbusFailCount<=4)) 
				{		
					if(!bRWSMBus(SMbusChB, SMbusRW, SmBat_Addr, C_RSOC, &BAT1PERCL,SMBus_NoPEC))
					{
						BatSMbusFailCount++;
					}
					else
			 		{
						BatSMbusFailCount=0;
			            break; //JIMGBRW022
					}
				}
			 				  			 			 
			}
			if(BatSMbusFailCount>=4) 
				break;
			
			if (BAT1PERCL < 0x02)  //T064: change 0x05 to 0x00 G23:modify battery capacity point from 0 to 1 for fliker.
			{
				SET_MASK(nBatteryStatL,CMBS_LOWBATT);
				LOWBATT_3TIMES = 100;		// set 5 sec
				break;
			}
		}
	  //72JERRY076:e+Required to power on at battery RSOC below 2% in S5/S4 follow UI spec.
	   if(IS_MASK_SET(SysStatus2, b3EC8S_Reset))
        {
            break;
        }//72JERRY040:Modify press power button more than 8S can power on.
	  	PWSeqStep = 1;
			PowSeqDelay = 0x00;
			uWOVSTATUS=1;//72JERRY032: Add voice wake up cmd for BIOS.
            RamDebug(0x55);
			SysPowState=SYSTEM_S5_S0;
			break;
	  case SYSTEM_S3:
	  	    PWSeqStep = 1;
			PowSeqDelay = 0x00;
			uWOVSTATUS=1;//72JERRY032: Add voice wake up cmd for BIOS.
            RamDebug(0x56);
			SysPowState=SYSTEM_S3_S0;
			break;
		default:
			break;
	}
}

/*
*******************************************************************************
* Function name: Battery1In       
*
* Descriptoin: battery 1 in function.
*
* Invoked by: EventManager()
*
* TimeDiv:NA
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/
void Battery1In(void)
{
	SET_MASK(SEL_STATE0,PRESENT_A);  	//set battery flag
	SET_MASK(nBatteryStatus1,ENEXIST);  //set battery flag
	FirstGetBatData();					//first get battery data
   	ECSMI_SCIEvent(ACPI_BAT1IN_SCI);
	Service_Auth_Step = 1;				// start SHA1
	//Clean Get_Batt_debounce_count to make it check battery data stable or not?
	Get_Batt_debounce_count = 0;
}

/*
*******************************************************************************
* Function name: Battery1Out       
*
* Descriptoin: battery 1 out function.
*
* Invoked by: EventManager()
*
* TimeDiv:NA
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/
void Battery1Out(void)
{
	PORT_BYTE_PNTR BAT1_pntr;

	CLEAR_MASK(SEL_STATE0,PRESENT_A);  	//clear battery flag
	CLEAR_MASK(nBatteryStatus1,ENEXIST);	//clear battery flag
	BAT1_pntr = &SEL_STATE0;	// Base on address 0x04BF.
	for( ITempW01=0; ITempW01<=57; ITempW01++ )	// Clear 04BF~04F8.
	{
		*BAT1_pntr=0;
        BAT1_pntr++;
	}

	BAT1_pntr = &BATTMANUFACTURE;	// Base on address 0x048F.
	for( ITempW01=0; ITempW01<=16; ITempW01++ )	// Clear 048F~049F.
	{
		*BAT1_pntr=0;
        BAT1_pntr++;
	}


    if(SystemIsS0 && IS_MASK_SET(LENOVOPMFW,BATTERY_MAIN_CAL))
    {
        uVPCeventSource = General;
		uVPCeventSource2 = 0;
		ECSMI_SCIEvent(SDV_VPC_notify);
    }

	Battdata_ready = 0;
	BatSMbusFailCount = 0;
	nBattErrorCnt = 0;
	LENOVOBATT = 0;
	EM8_TEST = 0;
	LV_Authen_Step_CNT = 0;
	SHA1failCnt = 0;
    SHA1FailRetry=0x00;
	Bat1_FPChgFlag = 0;
	CellCount=0; //MEILING017: add
	ECSMI_SCIEvent(ACPI_BAT1IN_SCI);
}


/*
*******************************************************************************
* Function name: LID_Pressed       
*
* Descriptoin: Lid pressed.
*
* Invoked by: EventManager()
*
* TimeDiv:NA
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/
void LID_Pressed(void)
{
    CLEAR_MASK(SWI_EVENT,LID);
	ECQEvent(SCI_LID_Close);
	RamDebug(0x15);
	//Modify the USB mouse can wake up from S3 at DC mode when the LID closed,not follow UI SPEC.
	if((SystemIsS3)&&(!Read_AC_IN()))
	{
		USB_ON_INPUT;
	}
	
}

/*
*******************************************************************************
* Function name: LID_Released       
*
* Descriptoin: Lid released.
*
* Invoked by: EventManager()
*
* TimeDiv:NA
*
* Arguments: NA   
*
* Return Values: NA
*******************************************************************************
*/
void LID_Released(void)
{
	SET_MASK(SWI_EVENT,LID);
	if ((SystemIsS3))
	{
		PulseSBPowerButton();
	}
    ECQEvent(SCI_LID_Open);
	RamDebug(0x16);
}


void LID_PAD_Pressed(void)
{
	 SET_MASK(SYS_STATUS2,PadLidClose);
     RamDebug(0x93);
	 if(SystemIsS0)
     {	   
 				if  (IS_MASK_SET(SYS_MISC1, ACPI_OS)) 
				{
					if(READ_EC_TP_ON())
					{
						EC_TP_ON_LOW();
						SET_MASK(pDevStatus1, b7DisableTP);
						uVPCeventSource = TouchPad;
						uVPCeventSource2 = 0;
						ECQEvent(SDV_VPC_notify);
					}
				}
			    //PAD Cover Close (PAD LID Close)
				if(IS_MASK_SET(SYS_MISC1,ACPI_OS))
				{
				  if((IS_MASK_CLEAR(pDevStatus2,b1TransAP_CTRL))&&(IS_MASK_SET(pDevStatus1,b5TPDRIVER_STATUS))
					&&(IS_MASK_CLEAR(pDevStatus1, b7DisableTP)))
				  {
					ECQEvent(0x3F);	

					SET_MASK(pDevStatus3,b6EnDisTPSend); 
					if(IS_MASK_SET(pDevStatus1,b7DisableTP))
					{
					   SET_MASK(pDevStatus3,b7TPStateFormer);
					}
					else
					{
					   CLEAR_MASK(pDevStatus3,b7TPStateFormer);
					}  

					RamDebug(0xBF);   
				  }
				}
				
				SET_MASK(pDevStatus2,b2PadLid_Close);
				
				if(IS_MASK_CLEAR(pDevStatus2,b1TransAP_CTRL))	 
				{
				  SET_MASK(pDevStatus1,b2DisableKB);
				  RamDebug(0xB4); 

				  if(IS_MASK_SET(EM9_NEWFUN, b7_LShiftPress))  
				  { 
						simple_code(0x12,BREAK_EVENT);
						CLEAR_MASK(EM9_NEWFUN,b7_LShiftPress);
				  }
				  if(IS_MASK_SET(EM9_NEWFUN,b6_RShiftPress))
				  {
						simple_code(0x59,BREAK_EVENT);
						CLEAR_MASK(EM9_NEWFUN, b6_RShiftPress);
				  }
				  if(IS_MASK_SET(EM9_NEWFUN, b5_LAltPress)) 
				  { 
						simple_code(0x11,BREAK_EVENT);
						CLEAR_MASK(EM9_NEWFUN,b5_LAltPress);
				  }
				  if(IS_MASK_SET(EM9_NEWFUN,b4_RAltPress))
				  {
						 e0_prefix_code(0x11,BREAK_EVENT);
						 CLEAR_MASK(EM9_NEWFUN, b4_RAltPress);
				  }
				  if(IS_MASK_SET(EM9_NEWFUN, b3_LCtrlPress)) 
				  { 
						 simple_code(0x14,BREAK_EVENT);
						 CLEAR_MASK(EM9_NEWFUN,b3_LCtrlPress);
				  }
				  if(IS_MASK_SET(EM9_NEWFUN,b2_RCtrlPress))
				  {
						 e0_prefix_code(0x14,BREAK_EVENT);
						 CLEAR_MASK(EM9_NEWFUN, b2_RCtrlPress);
				  } 

				}												
		}	 	
}

void LID_PAD_Released(void)
{
    CLEAR_MASK(SYS_STATUS2,PadLidClose);
	RamDebug(0x94);
	if(SystemIsS0)
    {
    			 if  (IS_MASK_SET(SYS_MISC1, ACPI_OS))//HEGANGS027:Add the function of YMC control the TP
				{
					if(IS_MASK_CLEAR(pDevStatus2,b1TransAP_CTRL))
					{
						if(!READ_EC_TP_ON())
						{
							EC_TP_ON_HI();
							CLEAR_MASK(pDevStatus1, b7DisableTP);
							uVPCeventSource = TouchPad;
							uVPCeventSource2 = 0;
							ECQEvent(SDV_VPC_notify);
						}
					}
				}
                   //PAD Cover Open (PAD LID OPEN)
				 if(IS_MASK_SET(SYS_MISC1,ACPI_OS))
				 {
						if((IS_MASK_CLEAR(pDevStatus2,b1TransAP_CTRL))&&(IS_MASK_SET(pDevStatus1,b5TPDRIVER_STATUS))
							&&(IS_MASK_SET(pDevStatus1, b7DisableTP)))  
						{
							ECQEvent(0x3E); 	

							SET_MASK(pDevStatus3,b6EnDisTPSend); 
							if(IS_MASK_SET(pDevStatus1,b7DisableTP))
							{
							   SET_MASK(pDevStatus3,b7TPStateFormer);
							}
							else
							{
							   CLEAR_MASK(pDevStatus3,b7TPStateFormer);
							}  

						    RamDebug(0xBE);   
						}
				  }
				  		
				   CLEAR_MASK(pDevStatus2,b2PadLid_Close);
					 
				   if(IS_MASK_CLEAR(pDevStatus2,b1TransAP_CTRL))  
				   {
					 CLEAR_MASK(pDevStatus1,b2DisableKB);
					 
					 RamDebug(0xB5);
					 
					 Enable_Any_Key_Irq();
					 if (SystemIsS3)
					 {
					 	InterKBDWakeEnable(); 
					 }	
				   }											 								
		}		
}

// ----------------------------------------------------------------------------
// Device insert/remove debounce routine.
// input:   device_id
// ----------------------------------------------------------------------------

const struct sDebounce code Debounce_TBL[] =
{

  	{&POWER_FLAG1	,wait_PSW_off	  ,&DEBOUNCE_CONT1	 ,T_PSW_DEBOUNCE	   	,PSWPressed	    ,PSWReleased    ,PSWOverrided	},
    {&POWER_FLAG1	,adapter_in 	  ,&DEBOUNCE_CONT2 	 ,T_AC_DEBOUNCE		 	,AdapterIn	    ,AdapterOut	   	,NullEvent	    },
	{&BT1_STATUS1	,bat_in			  ,&DEBOUNCE_CONT3 	 ,T_BAT_DEBOUNCE	    ,Battery1In	    ,Battery1Out	,NullEvent		},
	{&POWER_FLAG1	,wati_NOVO_off	  ,&DEBOUNCE_CONT4 	 ,T_NOVO_DEBOUNCE	   	,NOVOPressed	,NOVOReleased   ,NOVOOverrided  }, //LMLNANO008:Modify debunce time define.
	{&POWER_FLAG1	,LAN_WAKE_Status  ,&DEBOUNCE_CONT5 	 ,T_LAN_WAKE_DEBOUNCE 	,LanWakeLow	    ,LanWakeHi      ,NullEvent		},
	{&POWER_FLAG1	,Lid_Act       	  ,&DEBOUNCE_CONT6 	 ,T_LID_DEBOUNCE	    ,LID_Pressed	,LID_Released	,NullEvent		},//change LID implement method.
	{&EVT_STATUS1	,Lid_PAD_Act	  ,&DEBOUNCE_CONT7	,T_EXTEVT_DEBOUNCE	   ,LID_PAD_Pressed  ,LID_PAD_Released		,NullEvent			}, //MARTINM026: Add
	{&EVT_STATUS1	,DummyFlag 		  ,&DEBOUNCE_CONT8 	 ,T_EXTEVT_DEBOUNCE   	,NullEvent	    ,NullEvent		,NullEvent	    },
	{&EVT_STATUS1	,DummyFlag	      ,&DEBOUNCE_CONT9 	 ,T_EXTEVT_DEBOUNCE   	,NullEvent	    ,NullEvent		,NullEvent	    },
	{&EVT_STATUS1	,DummyFlag		  ,&DEBOUNCE_CONT10  ,T_EXTEVT_DEBOUNCE	 	,NullEvent	    ,NullEvent		,NullEvent	    },
};


/*
*******************************************************************************
* Function name: EventManager       
*
* Descriptoin: Event manager.
*              If want to call the function of status not change, please set nochange_func = 1
*
* Invoked by: Hook_Timer1msEvent()
*
* TimeDiv:1ms
*
* Arguments: EventId   
*
* Return Values: NA
*******************************************************************************
*/
#pragma OT(8, SPEED)
void EventManager(BYTE device_id)
{
	BYTE new_state,old_state;
	BYTE device_flag;
	switch(device_id)
	{
	case 0 :
    	if (Read_EC_PWRBTN()&&Read_EC_NOVO()) 	return;  
        new_state = Read_EC_PWRBTN();                            
		break;

	case 1 :
		new_state = Read_AC_IN();
		break;

	case 2 :
		new_state=Read_BAT_IN();//ADD battery detect
		break;

	case 3 :	
        new_state = Read_EC_NOVO(); 
		break;

	case 4 :
		#if	Support_EC_LANWake
		new_state = Read_LAN_WAKE_IN();
		#else
		new_state = 0x00;
		#endif	// Support_EC_LANWake
		break;

	case 5 :
		if(IS_MASK_CLEAR(SysStatus,LidKBIgnore)) 
		{
			#if LID_OnlyQEvent_Support 
			new_state = !Read_LID_SW_IN(); //Open: LID_SW_IN: high else: low
			#else
			new_state = 0x00;
			#endif
		}
		else
		{
			new_state = 0x00;
		}
		break;
	case 6 :
		
		new_state = !Read_LID_PAD(); //Open: LID_SW_IN: high else: low
		break;
	

	case 7:
        new_state = 0x00;
		break;

	case 8:
		new_state = 0x00;
		break;

	case 9:
        new_state = 0x00;
		break;

	default :
		new_state = 0x00;
		break;
	}

	Tmp_XPntr = Debounce_TBL[device_id].REG;
	Tmp_XPntr1 = Debounce_TBL[device_id].Cunter;
	device_flag = Debounce_TBL[device_id].Flag;

	old_state = ((*Tmp_XPntr & device_flag) != 0x00);
	if(new_state != old_state)
	{
		if((*Tmp_XPntr1) == 0x00)
		{
			//modify power button debunce time in S0.
			(*Tmp_XPntr1) = Debounce_TBL[device_id].Time;
		}
		else
		{
			(*Tmp_XPntr1) --;
            
			if((*Tmp_XPntr1) == 0)
			{
				if(new_state)
				{
					(Debounce_TBL[device_id].press)();
					*Tmp_XPntr |= device_flag;
				}
				else
				{
					(Debounce_TBL[device_id].release)();
					*Tmp_XPntr  &= (~device_flag);
				}
			}
		}
	}
	else
	{
		*Tmp_XPntr1 = 0x00;
	}

	(Debounce_TBL[device_id].nochange)();
}
//Modify press button 1s enter S3/S4/shutdown follow UI spec.
void PowerBottonEvent(void)
{
		 if(IS_MASK_SET(SysStatus2, PWRBTN_press_retry))
		 	{  PWRBTN_press_debounce++;
		 	if(PWRBTN_press_debounce>=1)
		 		{
		 		PWRBTN_press_debounce=0;
				CLEAR_MASK(SysStatus2,PWRBTN_press_retry);
				ECQEvent(Powerbotton_SCI);
		 		}
		 	}
}

