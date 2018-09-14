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

//-----------------------------------------------------------------------------
// Adapter in function
//-----------------------------------------------------------------------------
void AdapterIn(void)
{
	//DPWROK_EC_HI();
	/*ANGELAS031 remove start
	if(SystemIsS5)
	{
		//ANGELAS005:s+ Support UMA SKU EC_ON pin design.
		if(NTC_V1>682)
		{
			GPCRF0 = OUTPUT; 
			SET_MASK(GPDRF,BIT(0));
			NTC_FLAG=1; //ANGELAS007:Optimize power on sequence.
		}
		else
		//ANGELAS005:e+ Support UMA SKU EC_ON pin design.
		{ //ANGELAS007:Optimize power on sequence.
			GPCRF0 = INPUT; //ANGELAS020
			EC_ON_HI();
			NTC_FLAG=0; //ANGELAS007:Optimize power on sequence.
		} //ANGELAS007:Optimize power on sequence.
		Delay1MS(1);
		PM_PWRBTN_HI();
		Delay1MS(10);
		PCH_PWR_EN_ON();
		Delay1MS(30);
		RSMRST_HI();
		//Delay1MS(20);
	}
	*///ANGELAS031 remove end

	PECI_SETPL2Watts(2);
	
	if ( IS_MASK_CLEAR(SYS_STATUS,AC_ADP) )
	{ 
		//cBATTThrottling = 0;  //MEILING048:remove.//MEILING051:-Add cpu P_STATE function.
	}
	
	AC_PRESENT_HI(); 
	ACInOutIntEnable(); //MEILING039:add.
	Unlock_ShipMode();  
	SET_MASK(SYS_STATUS,AC_ADP);	//set AC flag
	if( IS_MASK_SET(nBatteryStatus1, ENEXIST) )  //G10:add check battery state
	{
		Chk_Battery_Full();
	}

   	ECSMI_SCIEvent(ACPI_ACIN_SCI);
   	if( SystemIsS0 )
	{
		PWM_BEEP = 0x7F;			// Out beep
		PWM_LEDBeep_CNT = 0x0A;		// 50ms*10
	}
	//SetACIN_Int(); //ANGELAS079:remove
	InitChargerIC();//ANGELAS016:Change charge IC option setting.
	BatSMbusFailCount = 0;
	nBattErrorCnt = 0;
	//Mos: Set 0x12h and 0x3Fh before set ChargeCurrent and ChargeVoltage.
	CHGIC_ptr = 1;
	WriteSmartChgIC();
	WriteSmartChgIC();
	CHGIC_ptr = 3;
	ReadSmartChgIC();
    if (IS_MASK_SET(SYS_STATUS,AC_ADP)) 
    {
	    //ADAPTERIDON_LOW(); //MEILING001:remove.
	    AdapterIDOn_Flag = 0x01; //MEILING001:add.
        ADPIDON10MS_NUM =0x0A; //change 0x05 to 0x0A to avoid read dirty data.
    }    
//   SetPowerBatteryparameter(); 
//   ADAPTERIDON_HI();  
	if(SystemIsS4||SystemIsS5||SystemIsDSX)
	{
		S4S5_KeyWake(2);//HEGANGS021:one key wake
	}
}

//-----------------------------------------------------------------------------
// Adapter out function
//-----------------------------------------------------------------------------
void AdapterOut(void)
{
	//ADAPTERIDON_HI();//when remove AC do not read adapter ID. //MEILING001:remove.
	AdapterIDOn_Flag = 0x00;//MEILING001:add.
	AdapterID=0x00;//72JERRY069
	TYPEC_Adapter_Watts=0x00;//72JERRY069
	//MEILING039:S- remove.
	/*if(SystemIsS0)	//ANGELAS095:Modify PROCHOT# unplug AC under S5.
	{
		H_PROCHOT_ON(); //ANGELAS026:Modify PROCHOT# function.
		ProchotONCnt = 2; //MEILING033:add(Prochot on 2S when AC out).
	}*/
	//MEILING039:E-.
	
	BATT_LEN_OFF();//JERRYCRZ030 remove AC_OFF function follow charge IC bq24780.
	AC_PRESENT_LOW(); 
	ACInOutIntEnable(); //MEILING054:add.
    CLEAR_MASK(SYS_STATUS,b4IllegalAdp);
	CLEAR_MASK(SYS_STATUS,b5UnSuitAdpPow);
	SetVGA_AC_DET();
	PECI_SETPL2Watts(1);

	CLEAR_MASK(SYS_STATUS,AC_ADP);  //clear battery flag
	Chk_Battery_Full();
   	ECSMI_SCIEvent(ACPI_ACOUT_SCI);

	//Mos: Set P-State highest to force that ChkBattery_OCP will decress P-State step by step.
	SET_MASK(BatteryAlarm,BATOCP);
    // if(SMB_BCNT==0x55)  
	//cBATTThrottling = 0x0F;
	//cBATTThrottling = PSTATE_MAXStep;//0ptimize CPU P_state (change 16 step to 8 step).  //MEILING048:remove.//MEILING051:-Add cpu P_STATE function.
	//Mos: Send QEvent to Bios
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
//72JERRY050:S+Modify the USB mouse can wake up from S3 at DC mode when the LID closed,not follow UI SPEC.
	if((SystemIsS3)&&(!Read_LID_SW_IN()))
		{USB_ON_INPUT;
		}
		//72JERRY050:E+Modify the USB mouse can wake up from S3 at DC mode when the LID closed,not follow UI SPEC.
	nAvgCurrentL = 0;
	nAvgCurrentH = 0;
	Bat0x0BFakeCnt = 60;
	//CLR_MASK(LENOVOPMFW,BATTERY_CYCLE);
    DSxPowState = SYSTEM_S3S4;
    ADPIDON10MS_NUM=0xFF; 
	InitChargerIC();
	//SetACIN_Int();
}

//-----------------------------------------------------------------------------
//power button was pressed
//-----------------------------------------------------------------------------
void PSWPressed(void)
{
	if ( IS_MASK_SET(cCmd, b6TestBtnEn) )
	{
		Buffer_Key(0xE0);		//
		Buffer_Key(0x25);		//
		F_Service_SEND = 1;
	}
	/*// ANGELAS007: s-Optimize power on sequence.
	//ANGELAS005:s+ Support UMA SKU EC_ON pin design.
	if(NTC_V1>682)
	{
		GPCRF0 = OUTPUT;
		SET_MASK(GPDRF,BIT(0));
	}
	else
	//ANGELAS005:e+ Support UMA SKU EC_ON pin design.

	//EC_ON_HI();
	//EC_ON2_HI();
	*///ANGELAS007: e-Optimize power on sequence.
	ECSleepCount = 3;

	//Mos: Clear PswPressedCounter
	CountSecAfterPswPressed = 0;
	/*//ANGELAS007:s-Optimize power on sequence.
	//JERRYCRZ022:+ S Filter press power button over 8s can auto power on.
	#if !EN_PwrSeqTest
	if (LID_readytime !=0)
	{
		//SET_MASK(SysStatus2,PWRBTN_press_retry);  
		return;
	}
	 #endif
	//JERRYCRZ022:+ E Filter press power button over 8s can auto power on.	
	*///ANGELAS007:e-Optimize power on sequence.
	CLEAR_MASK(SysStatus2,PWRBTN_press_retry);
	/*ANGELAS036: remove start
	if(!Read_LID_SW_IN())
	{
		DSxPowState = SYSTEM_S3S4; //ANGELAS031
		return;
	}
  	//LID have not ready, it will influence power button on, so remove it temporary
	*///ANGELAS036: remove end

	//LMLKBL0008:S+ Add ignore power button press when lid close.
	if(!Read_LID_SW_IN())
	{ return; }
	//LMLKBL0008:E+.
	
    switch (SysPowState)
    {
    case SYSTEM_S4 :
    	break;
	case SYSTEM_DSX://72JERRY006: Add DC S5 states.
    case SYSTEM_S5 :
		//ANGELAS111:S+Modify code for befor power on check Battery RSOC in DC mode.
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
		//ANGELAS111:E+Modify code for befor power on check Battery RSOC in DC mode.
		//if ((Battdata_ready != 1)&&(!Read_AC_IN()||Read_ACOFF()))
		/*if ((Battdata_ready != 1)&&(!Read_AC_IN()))
		{
			FirstGetBatData();
			if (Battdata_ready != 1)
			{
				Battdata_ready = 0xFF;
				break;
			}
		}
		//if ((BAT1PERCL < 0x02)&&(!Read_AC_IN()||Read_ACOFF()))  //change 0x05 to 0x00 G23:modify battery capacity point from 0 to 1 for fliker.
		if ((BAT1PERCL < 0x02)&&(!Read_AC_IN()))  //change 0x05 to 0x00 G23:modify battery capacity point from 0 to 1 for fliker.
		{
			SET_MASK(nBatteryStatL,CMBS_LOWBATT);
			LOWBATT_3TIMES = 100;		// set 5 se
			break;
		}*///ANGELAS111:-Modify code for befor power on check Battery RSOC in DC mode.
		//if ((!Read_AC_IN()||Read_ACOFF()))			// if DC mode
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
	//	ONEKEY_TEMP_FLAG = 0;
		PWSeqStep = 1;
        PowSeqDelay = 0x00;
        RamDebug(0x51);
		SysPowState=SYSTEM_S5_S0;
		break;

    case SYSTEM_S3 :
	case SYSTEM_S0 :
		RamDebug(0xF4);//test 
		if (IS_MASK_CLEAR(pProject4,pPWSWdisable)) //HEGANGS028:Disable POWERSW when flash bios
		{
			PM_PWRBTN_LOW();
		}   
		SET_MASK(SysStatus2,PWRBTN_press_retry);//72JERRY064:Modify press button 1s enter S3/S4/shutdown follow UI spec.
		
		break;

    default :
       	break;
    }
}

//-----------------------------------------------------------------------------
//power button was released
//-----------------------------------------------------------------------------
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
      PWRBTN_press_debounce=0;//72JERRY064:Modify press button 1s enter S3/S4/shutdown follow UI spec.
	  CLEAR_MASK(SysStatus2,PWRBTN_press_retry);   //72JERRY064:Modify press button 1s enter S3/S4/shutdown follow UI spec.           
//	if(DSxPowState == SYSTEM_S3S4) //LMLNANO016:add.
//	{
//		PM_PWRBTN_HI(); //LMLNANO014:add. 
//	}
    if(!Read_LID_SW_IN())
	{ return; }

    //PM_PWRBTN_HI();   //LMLNANO014:remove.

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
		break;

	default :
		break;
	}
}
//-----------------------------------------------------------------------------
//power button was overrided
//-----------------------------------------------------------------------------
void PSWOverrided(void)
{
	if(!Read_LID_SW_IN())
	{ return; }
    if (Read_EC_PWRBTN()&&Read_EC_NOVO())
    { return; }
	PowerBottonEvent();//72JERRY064:Modify press button 1s enter S3/S4/shutdown follow UI spec.
	//LMLKBL0011:S- remove press power button 4s force shutdown function.
	/*if (SystemIsS0 || SystemIsS3)
	{
		if (Read_EC_PWRBTN())
		{
			PSW_COUNTER--;
			if (PSW_COUNTER == 0)
			{
				PWSeqStep = 1;
				PowSeqDelay = 0x00;
                RamDebug(0x0C);
				SysPowState=SYSTEM_S0_S5;
			}
		}
		else
		{
			PSW_COUNTER = T_PSWOFF;		// initialize counter
		}
	}*/
	//LMLKBL0011:E-.
}


//-----------------------------------------------------------------------------
//NOVO button was pressed
//-----------------------------------------------------------------------------
void NOVOPressed(void)
{
	if ( IS_MASK_SET(cCmd, b6TestBtnEn) )
	{
		Buffer_Key(0xE0);	//
		Buffer_Key(0x49);	// page down make
		F_Service_SEND = 1;
	}
	/*//ANGELAS007:s-Optimize power on sequence.
	//ANGELAS005:s+ Support UMA SKU EC_ON pin design.
	if(NTC_V1>682)
	{
		GPCRF0 = OUTPUT;
		SET_MASK(GPDRF,BIT(0));
	}
	else
	//ANGELAS005:e+ Support UMA SKU EC_ON pin design.

	//EC_ON_HI();
	//EC_ON2_HI();
	*///ANGELAS007:e-Optimize power on sequence.
	ECSleepCount = 3;
	/*//ANGELAS007:s-Optimize power on sequence.
	#if !EN_PwrSeqTest
	if (LID_readytime !=0)
	{
		//SET_MASK(SysStatus2,b1NOVOBTN_retry); 
		return;
	}
 	#endif
 	*///ANGELAS007:e-Optimize power on sequence.
	CLEAR_MASK(SysStatus2,b1NOVOBTN_retry);

    if(!Read_LID_SW_IN())
	{ return; }

	//Mos: Clear NOVOPressedCounter
	CountSecAfterNOVOPressed = 0;

	switch (SysPowState)
    {
	case SYSTEM_S4 :
		//break;
		case SYSTEM_DSX://72JERRY006: Add DC S5 states.
    case SYSTEM_S5 :
		// Battery capacity > 5% or not
		//ANGELAS111:S+Modify code for befor power on check Battery RSOC in DC mode.
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
			
			if (BAT1PERCL < 0x02)  //T064: change 0x05 to 0x00 G23:modify battery capacity point from 0 to 1 for fliker.
			{
				SET_MASK(nBatteryStatL,CMBS_LOWBATT);
				LOWBATT_3TIMES = 100;		// set 5 sec
				break;
			}
		}
		//ANGELAS111:E+Modify code for befor power on check Battery RSOC in DC mode.
		if(LOWBATT_3TIMES == 0)
        {
			//EEPROM_PwrSts = Read_Eflash_Byte(EEPROMA2,(EEPROMA1_B03 | 0x07),0xDF);
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
            RamDebug(0x52);// T045+
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
			//uNovoVPCCount=0;
			//PM_PWRBTN_LOW();
		}
		//break;
    default :
        break;
    }
}

//-----------------------------------------------------------------------------
//NOVO button was released
//-----------------------------------------------------------------------------
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

	//CLR_MASK(SysStatus2,b2NOVOKey_Press);		// Release NOVO key.   
    if(!Read_LID_SW_IN())
	{ return; }

	switch (SysPowState)
	{
	case SYSTEM_S4 :
		//break;
	case SYSTEM_S5 :
		//break;
	case SYSTEM_S3 :
		break;
	case SYSTEM_S0 :
		//PM_PWRBTN_HI();
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
		//break;
	default :
		break;
	}
}

//-----------------------------------------------------------------------------
//power button was overrided
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//LAN_WAKE was low
//-----------------------------------------------------------------------------
void LanWakeLow(void)
{
	#if	Support_EC_LANWake
	//if (uReserve07.fbit.nPmeWakeEN==0) return;

	switch (SysPowState)
	{
	case SYSTEM_S4 :
		break;

	case SYSTEM_S5 :
		//PWSeqStep = 1;
		//PowSeqDelay = 0x00;
		//SysPowState=SYSTEM_S5_S0;
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

//-----------------------------------------------------------------------------
//LAN_WAKE was hi
//-----------------------------------------------------------------------------
void LanWakeHi(void)
{
	#if	Support_EC_LANWake
	//if (uReserve07.fbit.nPmeWakeEN==0) return;

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
// 72JERRY012£ºVoice wake system event function
//-----------------------------------------------------------------------------
void VoiceWakeSystem(void)
{
 if(cDev.fbit.cD_WovMFGtest==1)//enable wov MFG test
 	{return;
 	}
 	//72JERRY065:Add CMD 0x59 test voice wake up for MFG.
	switch(SysPowState)
	{
	  case SYSTEM_S5:
	  case SYSTEM_DSX://72JERRY029: Modify Voice Wake system method,Add DC S5 wake up status.
	  //72JERRY076:s+Required to power on at battery RSOC below 2% in S5/S4 follow UI spec.
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

//-----------------------------------------------------------------------------
// battery 1 in function
//-----------------------------------------------------------------------------
void Battery1In(void)
{
	/*ANGELAS038: remove start
	//ANGELAS020:S+
	if(IS_MASK_CLEAR(SYS_STATUS,AC_ADP) &&(!(NTC_V1>682)))
	{
		GPCRF0 = INPUT; 
	}
	//ANGELAS020:E+
	//ANGELAS023:S+Modify UMA LED function.
	if((SystemIsS5) && (IS_MASK_CLEAR(SYS_STATUS,AC_ADP)))
	{
		if(NTC_V1>682)
			NTC_FLAG=1; 
		else
			NTC_FLAG=0;
	}
	//ANGELAS023:E+Modify UMA LED function.
	*///ANGELAS038:remove end
	SET_MASK(SEL_STATE0,PRESENT_A);  	//set battery flag
	SET_MASK(nBatteryStatus1,ENEXIST);  //set battery flag
	FirstGetBatData();					//first get battery data
   	ECSMI_SCIEvent(ACPI_BAT1IN_SCI);
	Service_Auth_Step = 1;				// start SHA1
 	/*    
       if (IS_MASK_SET(SYS_STATUS,AC_ADP)) 
       {
	    ADAPTERIDON_LOW();
       }
 	*/           
 	//SetPowerBatteryparameter(); 
  	//ADAPTERIDON_HI();   
	//Mos: Clean Get_Batt_debounce_count to make it check battery data stable or not?
	Get_Batt_debounce_count = 0;
}

//-----------------------------------------------------------------------------
// battery 1 out function
//-----------------------------------------------------------------------------
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
//change LID implement method.
void LID_Pressed(void)
{
    CLEAR_MASK(SWI_EVENT,LID);
	ECQEvent(SCI_LID_Close);
	RamDebug(0x15);
	//EC_TS_ON_LOW();//Add touch panel control signal.  //MEILING001:add.
	//72JERRY050:S+Modify the USB mouse can wake up from S3 at DC mode when the LID closed,not follow UI SPEC.
	if((SystemIsS3)&&(!Read_AC_IN()))
		{USB_ON_INPUT;
		}
	//72JERRY050:E+Modify the USB mouse can wake up from S3 at DC mode when the LID closed,not follow UI SPEC.
	
}
void LID_Released(void)
{
	SET_MASK(SWI_EVENT,LID);
	if ((SystemIsS3))
	{
		PulseSBPowerButton();
	}
    ECQEvent(SCI_LID_Open);
	RamDebug(0x16);
	//EC_TS_ON_HI();//Add touch panel control signal.  //MEILING001:add.
}

//MARTINM026:add start
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
    CLEAR_MASK(SYS_STATUS2,PadLidClose);//MARTINM037:Add
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

//change LID implement method.
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

// ----------------------------------------------------------------------------
// EventManager function
//
// Note :
//	If want to call the function of status not change, please set nochange_func = 1
// ----------------------------------------------------------------------------
#pragma OT(8, SPEED)
void EventManager(BYTE device_id)
{
	BYTE new_state,old_state;
	BYTE device_flag;
	//BYTE do_nochange_func = 0x00;
	switch(device_id)
	{
	case 0 :
    	if (Read_EC_PWRBTN()&&Read_EC_NOVO()) 	return;  //ANGELAS099:Optimize power switch and NOVO control method.//ANGELAS101:Optimize ANGELAS099 change break to return.
        new_state = Read_EC_PWRBTN();                            
		/*  
		if ( IS_MASK_CLEAR(SysStatus2,b2NOVOKey_Press) )
		{
			if (((Battdata_ready == 0xFF)&&(!Read_AC_IN()||Read_ACOFF()))||
				(IS_MASK_SET(SysStatus2,PWRBTN_press_retry)))
			{
				//new_state = !Read_EC_PWRBTN();       
				//old_state = Read_EC_PWRBTN();          
				new_state = Read_EC_PWRBTN();      
				old_state = !Read_EC_PWRBTN();          			
			}
			else
			{ new_state = Read_EC_PWRBTN(); }
		}
		else
		{ new_state = old_state = 0; }
		*/	
		break;

	case 1 :
		new_state = Read_AC_IN();
		break;

	case 2 :
		/*if((BATTEMP<0x390)) //martin0216 0x390 -> 0x390*3/0x400 -> 2.67
		{ new_state = 1; }
		else
		{ new_state = 0; }*/
		new_state=Read_BAT_IN();//G03:ADD battery detect
		break;

	case 3 :
		/*             
        	if ( Read_EC_NOVO() )                                                                      
 		{ SET_MASK(SysStatus2,b2NOVOKey_Press); }	// Press NOVO key.    

		if ( IS_MASK_SET(SysStatus2,b1NOVOBTN_retry) )
		{
			new_state = !Read_EC_NOVO();
			old_state = Read_EC_NOVO();
		}
		else
		{ new_state = Read_EC_NOVO(); }
		*/    	
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
		//achange LID implement method.
		if(IS_MASK_CLEAR(SysStatus,LidKBIgnore)) 
		{
			#if LID_OnlyQEvent_Support //add judge
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
		// change LID implement method.
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
			//LMLNANO008:S+ modify power button debunce time in S0.
			//if((device_id==0) && (SystemIsS0) && (IS_MASK_CLEAR(cCmd, b6TestBtnEn))) //LMLNANO026:add solution for P/L test Power Button.//72JERRY042:Modify press power button debounce in SO follow UI spec.
				//(*Tmp_XPntr1) = T_PSW_DEBOUNCE_S0;//72JERRY042:Modify press power button debounce in SO follow UI spec.
			//else//72JERRY042:Modify press power button debounce in SO follow UI spec.
			//LMLNANO008:E+.
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
//72JERRY064:s+Modify press button 1s enter S3/S4/shutdown follow UI spec.
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
//72JERRY064:e+Modify press button 1s enter S3/S4/shutdown follow UI spec.

