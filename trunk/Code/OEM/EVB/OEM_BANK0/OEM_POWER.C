/*----------------------------------------------------------------------------
 * Filename: OEM_Power.C  For Chipset: ITE.IT85XX
 *
 * Function: Power sequnce control function example
 *
 * Author  : Dino
 *
 * Copyright (c) 2009 - , ITE Tech. Inc. All Rights Reserved.
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

// ----------------------------------------------------------------------------
// To read variable [RSTStatus] to get EC reset source
// ----------------------------------------------------------------------------
void CheckResetSource(void)
{
//add debug information for EC reset status(1.external watch-dog, 2.internal watch-dog; 3. EC VSTBY or WRST reset,4.EC code reset).
	RamDebug(0xBB); //ANGELAS089:add
	RamDebug(RSTStatus);
	RamDebug(GPCRA0);
	RamDebug(PWRSW8SRST);  //ANGELAS089:add
	RamDebug(0xBB);     //ANGELAS089:add
	switch(RSTStatus&0x03)
	{
	case 0:
	case 1:
		if(GPCRA0==0x80)
		{
			ShutDnCause = SC_ECColdBoot;
		}
		else
		{
			ShutDnCause = SC_EC_0x0000;
		}
		break;
				
	case 2:
		if(GPCRA0==0x80)
		{
			ShutDnCause = SC_IntWatchDog;
		}
		else
		{
			ShutDnCause = SC_EC_0x0000;
		}
		break;		   
	
	case 3:
		if(GPCRA0==0x80)
		{
			ShutDnCause = SC_ExtWatchDog;
		}
		else
		{
			ShutDnCause = SC_EC_0x0000;
		}
		break;
	}
	
	//ANGELAS089 add start
	if(IS_MASK_SET(PWRSW8SRST,BIT(1)))
	{
    	ShutDnCause = SC_EC8SRESET;
	}
	//ANGELAS089 add end
	
//add debug information for EC reset status(1.external watch-dog, 2.internal watch-dog; 3. EC VSTBY or WRST reset,4.EC code reset).		
}

void Check_LPC_Reset(void)
{
	if(!Read_LPC_RST())
	{
		//GATEA20_HI();
   		CLEAR_MASK(KBHISR,SYSF);
	    CLEAR_MASK(SYS_MISC1,ACPI_OS);
		OEM_LPC_Reset();	// initial OEM status.
	}
}

void OEM_LPC_Reset(void)
{
	BYTE BSLI_Det;
	BYTE EVENT_BUFFER_CNT;//72JERRY063:Clear Qevent Buffer when enter S3, S5, and LPC RESET.
	uReserve07.fbit.nTPDriverIn = 0;
	uReserve07.fbit.nVPCDriverIn = 0;
	uReserve07.fbit.nFanManual = 0; //ANGELAS030:Clear thermal tool flag under S3/S4/S5. return EC control
	//SYS_STATUS &= 0xF8;			// Clear OS mode.
	CPU_TYPE &= 0x3f;//CLEAR BIT6 and BIT7 Unit abnormal shuntdown will not clear GPU flag,so when enter S5,WB and S3 we clear GPU flag to not read GPU temperature.
	SET_MASK(pDevStus, pENABLE_TP);	// Turn on TouchPad.
	//CMOS_Cam_ON();					// Turn on Camera power.
	CLR_MASK(cCmd, b3BkOff);		// Turn on backlight.
	CLR_MASK(OEMControl, b1BkOff);	//ANGELAG005:add
	SMartNoise = 0x04;				// Stop Fan Dust mode.
	BackLight_En_Delay = 40;			//:Increase lit backlight time for reboot.//:Panel background light delay about 1s after Lenovo logo show out when reboot change 40 to 3. //:Remove  CHANGE 3 TO 40 
	//if (IS_MASK_SET(LENOVOPMFW,BATTERY_MAIN_CAL))
	//{
		CLEAR_MASK(LENOVOPMFW,BATTERY_MAIN_CAL);
		CLEAR_MASK(LENOVOPMFW,BATTERY_CAL_END);
		LENOVOPMFW_Temp = 0;
    	CLEAR_MASK(ACOFF_SOURCE, BATTLEARN);
    	// ACOFF_LOW();
	//}
	//uNovoVPCCount = 0;			// Clear NOVO VPC status.    ANGELAS004 Remove  clear novo button status when LPC reset function.
//72JERRY063:s+Clear Qevent Buffer when enter S3, S5, and LPC RESET.
for( EVENT_BUFFER_CNT=0; EVENT_BUFFER_CNT<=EVENT_BUFFER_SIZE; EVENT_BUFFER_CNT++)
	{
					 SCI_Event_Buffer[EVENT_BUFFER_CNT] = 0x00;						
	}
	SCI_Event_In_Index	= 0x00; 													  //clear SCI index   
	SCI_Event_Out_Index = 0x00; 
//	72JERRY063:e+Clear Qevent Buffer when enter S3, S5, and LPC RESET.	
	//#if SupportReadTPID
	//ReadTPid();
	//#endif
	EC_TP_ON_HI();//enable TP //72JERRY056:Add enable/disable TP function by HW pin.
	CLEAR_MASK(pDevStatus1, b7DisableTP);//enable TP//72JERRY056:Add enable/disable TP function by HW pin.
	F2_Pressed=0;//72JERRY059:Optimize F2/F12 flag to BIOS.
//ANGELAG008: add start
	CLEAR_MASK(pDevStatus3,b6EnDisTPSend);
	CLEAR_MASK(pDevStatus3,b7TPStateFormer);
	CLEAR_MASK(pDevStatus1,b0F6ScanCodeSend);
	CLEAR_MASK(pDevStatus1,b1F6DisableTP);
	CLEAR_MASK(pDevStatus1,b4LNVAPP_STATUS);
	CLEAR_MASK(pDevStatus1,b5TPDRIVER_STATUS);
	CLEAR_MASK(pDevStatus1,b6MODE_STATUS);
	if(IS_MASK_CLEAR(P80CMOSSts,P80CMOSDis)) //HEGANG0027:Add bios hang check
	{
		bios_check = 0;
	}
	//CLEAR_MASK(pDevStatus1,b7DisableTP);//72JERRY043:Support PTP TP show OSD,there isn't driver notify EC TP status,EC keep flag to rom.
//ANGELAG008: add end
}


void CHK_SUSACK(void)
{
#if 0
	if(!Read_SUSWARN())
	{
		if (Read_SUSACK())
		{
			if (SUSACK_LOW_CNT > 500)  //for DS3 test
			{
				SUSACK_LOW();
			}
			else
			{
				SUSACK_LOW_CNT++;
			}
		}
		else
		{	SUSACK_LOW_CNT = 0;	}
	}
	else
	{
		if (!Read_SUSACK())
		{
			if (SUSACK_HI_CNT > 20)
			{
				SUSACK_HI();
			}
			else
			{
				SUSACK_HI_CNT++;
			}
		}
		else
		{	SUSACK_HI_CNT = 0;		}
	}
#endif
}

#if 0
void CHK_DRAMRST_CNTRL(void)
{
	if (Read_VR_ON())
	{
		if (!Read_DRAMRST_CNTRL_EC())
		{
			if (DRAMRST_CNTRL_CNT > 200)
			{
				DRAMRST_CNTRL_EC_HI();
			}
			else
			{
				DRAMRST_CNTRL_CNT++;
			}
		}
	}
	else
	{ DRAMRST_CNTRL_CNT = 0; }

	if (!Read_SLPS3())
	{
		if ( SysPowState != SYSTEM_S5_S0 )
		{
			DRAMRST_CNTRL_EC_LOW();
			DRAMRST_CNTRL_CNT = 0;
		}
/*
		if (DRAMRST_CNTRL_LOW_CNT > 20)
		{
			DRAMRST_CNTRL_EC_LOW();
		}
		else
		{
			DRAMRST_CNTRL_LOW_CNT++;
		}
*/
	}
	else
	{ DRAMRST_CNTRL_LOW_CNT = 0; }
}
#endif

//-----------------------------------------------------------------------------
// Handle System Power Control
// Called by service_1mS -> Timer1msEvent -> Hook_Timer1msEvent
//-----------------------------------------------------------------------------
void Oem_SysPowerContrl(void)
{
	//CHK_SUSACK();
	//CHK_DRAMRST_CNTRL();

    // Check current System PowerState to see what should we do.
    switch (SysPowState)
    {
   	case SYSTEM_S0:
		if(!Read_SLPS3())
		{
			
			if (S3S4DelayCnt > 10)
			{
              	PWSeqStep = 1;
				PowSeqDelay = 00;
                RamDebug(0xD0);        
              	SysPowState=SYSTEM_S0_S3;
				//S0S3_VR_ON(); // include SYS_PWROK, PCH_PWROK,VR_ON enable orderly.
				if (!Read_SLPS4())
				{
					PWSeqStep = 1;
					PowSeqDelay = 0x00;
                    RamDebug(0x0D);
    				SysPowState=SYSTEM_S0_S5;
				}
				else if (IS_MASK_SET(SysStatus,CloseLid))
				{
					//CLEAR_MASK(SysStatus,CloseLid);
					SET_MASK(SysStatus,EnterS3Lid);
				}
			}
			else
			{	
				S3S4DelayCnt++;	
			}
		}
		else
		{	
			S3S4DelayCnt = 0;		
		}
		Check_LPC_Reset();
		break;

   	case SYSTEM_S3:
			     
        if(Read_SLPS3())  //S3 to S0
        {
            PWSeqStep = 3;
		    PowSeqDelay = 20;
            RamDebug(0x33);   
           	SysPowState=SYSTEM_S3_S0;
        }
		// for EM9 adapter detection and protection       
        if(IS_MASK_SET(SYS_STATUS,b4IllegalAdp)&&Read_AC_IN())
        {
   		    SET_MASK(SYS_STATUS,AC_ADP);	//set AC in  flag
		    //ACOFF_LOW();
			BATT_LEN_OFF();//JERRYCRZ030¡êo
        }
		//for EM9 adapter detection and protection 
        break;

    case SYSTEM_S4:
    case SYSTEM_S5:
		if((Read_SLPS3() && Read_SLPS4())) 
		{
			PWSeqStep = 1;
			PowSeqDelay = 0x00;
            RamDebug(0x53);
			SysPowState=SYSTEM_S5_S0;
		}
		CheckAutoPowerOn();
  		// (remove to avoid turn off EC_ON)          		Check_EC_ON();
  		//ANGELAS031 Check_EC_ON(); //martin0313C+
  		//ANGELAS032:add start
  		if((ShipModeEn==0xA5)||IS_MASK_SET(EMStatusBit2,b0SetBatteryShipMode))
  		{
  			CLEAR_MASK(ACPI_HOTKEY, b7BIOS_NoShut);
			CLEAR_MASK(ACPI_HOTKEY, b6Cmd_NoShut);
  		}
		//ANGELAS032:add end
		 if(( IS_MASK_CLEAR(ACPI_HOTKEY, b7BIOS_NoShut))&& (IS_MASK_CLEAR(ACPI_HOTKEY, b6Cmd_NoShut))&&(LOWBATT_3TIMES==0)) 
		  { 
		     //S34toDSx(); 
		      if ((!Read_AC_IN())&&IS_MASK_CLEAR(cCmd, bPCHPWR_Keep)) {
			  	 PWSeqStep = 1;
			     PowSeqDelay = 0x00;
			  	   SysPowState=SYSTEM_S5_DSX;
		      	}
		  }	
#if SUPPORT_PMIC
		//MEILING002: S+
		if(PowerOTP_REG)
		{
			if ( IS_MASK_CLEAR(SysStatus, ERR_ShuntDownFlag) ) //ANGELAG007: add
			{ ProcessSID(PMIC_Shutdown_ID); } //ANGELAG007: add
			OTP_TEMP = PowerOTP_REG;
			PowerOTP_REG = 0;
			RSMRST_LOW();
			Delay1MS(12);
			EC_ON_LOW();
			PowerInitOK = 0;
			if(Read_AC_IN())
			{
				//DSxPowState = SYSTEM_DSxOK;
			}
		}
#endif
  		//for EM9 adapter detection and protection       
        if(IS_MASK_SET(SYS_STATUS,b4IllegalAdp)&&Read_AC_IN())
        {
   		    SET_MASK(SYS_STATUS,AC_ADP);	//set AC in  flag
		    //   ACOFF_LOW();
			BATT_LEN_OFF();//JERRYCRZ030
        }
		//for EM9 adapter detection and protection 
        break;

		case SYSTEM_DSX:
			CheckAutoPowerOn(); 
			if(nBattGasgauge<=10)
			EC_ON_5V_LOW();
		 if((Read_AC_IN())||(LOWBATT_3TIMES!=0)){   //JERRYBR012:Fixed Power led will not flick 3 times when power on after discharged battery to 0% till system shut down.TDMS#110153 
		 	PWSeqStep = 1;
			PowSeqDelay = 0x00;
		    SysPowState=SYSTEM_DSX_S5;
		 	}
		break;
		

	 case SYSTEM_DSX_S5:
		    Oem_DSXS5Sequence();
	    break;
    case SYSTEM_S5_S0:
		Oem_S5S0Sequence();
		break;

    case SYSTEM_S3_S0:
		Oem_S3S0Sequence();
        break;

    case SYSTEM_S0_S3:
		Oem_S0S3Sequence();
		/*                       
		if (!Read_SLPS4())
		{
			PWSeqStep = 1;
			PowSeqDelay = 0x00;
    			SysPowState=SYSTEM_S0_S5;
		}
		*/
        break;

    case SYSTEM_S0_S4:
        break;

    case SYSTEM_S0_S5:
		Oem_S0S5Sequence();
        break;
	 case SYSTEM_S5_DSX:
		 Oem_S5DSXSequence();
		break;
	case SYSTEM_EC_WDTRST:
		break;

   	default:
         SysPowState=SYSTEM_DSX; 
			if(!Read_AC_IN())
      {   
             PWSeqStep = 1;
			 PowSeqDelay = 0x00;
			  SysPowState = SYSTEM_S5_DSX;
      }
      else
      {
             PWSeqStep = 1;
			PowSeqDelay = 0x00;
			  SysPowState = SYSTEM_DSX_S5;
      }
         	break;
    }
}

void SetS3GPIO(void)
{
	//USB_CHG_EN_HI();//72JERRY008: Add Always On USB feature follow 'lenovo Always On USB Ver5'.
    //CHG_MOD1_LOW();//72JERRY008:-
   // CHG_MOD2_HI();//72JERRY008:-
    //CHG_MOD3_LOW();//72JERRY008:-
}

void SetS0GPIO(void)
{
	SCI_HI();			  	//RUNSCI Hi
	RCING_HI();

	SMBus2_CLK_ALT;			// SMBus2 CLK to alternate pin
	SMBus2_DAT_ALT;			// SMBus2 DATA to alternate pin

	#if FAN_TABLE_Already
	FAN_PWM_ALT;			// Set FAN_PWM Alt.
	FAN1_PWM_ALT;//72JERRY001: Modify GPIO setting and fan1 setting.
	#else
	FAN_PWM_OUT;			// Set FAN_PWM OUTPUT.
	EC_FAN_PWM_HI();
	FAN1_PWM_OUT;//72JERRY001: Modify GPIO setting and fan1 setting.
	EC_FAN1_PWM_HI();//72JERRY001: Modify GPIO setting and fan1 setting.
	
	#endif	// FAN_TABLE_Already

	FAN_SPEED_ALT;			// Set FAN Speed Alt.
	FAN1_SPEED_ALT;//72JERRY001: Modify GPIO setting and fan1 setting.
	InitSio();

	KBHISR = 0x00;
	KBHIKDOR = 0x00;
	CLEAR_MASK(KBHISR,SYSF);

	SPCTRL1 = 0x83;			//msmart test
	BRAM3E =0;
	BRAM3F =0;

	PECIDelayCnt = 0x04;

	Fan_Init();
	//pDevStus=Read_Eflash_Byte(EEPROMA2,(EEPROMA1_B03|0x07),0xE1);

#if UCS1022_Support
	USB_CH_OUTPUT;
	//if ( IS_MASK_CLEAR(USB_Charger, b0USB_EN) )
	if ( IS_MASK_CLEAR(EMStatusBit, b1SetUSBChgEn) )
	{ USB_CH_LOW(); }
	CLR_MASK(USB_Charger, b1USB_initOK);
#endif	// UCS1022_Support

	if ( IS_MASK_SET(SYS_MISC1, b1Num_LED) )
	{ //NUMLED_ON(); 
	}
	else
	{ //NUMLED_OFF(); 
	}
}

void SetS5GPIO(void)
{
	SCI_LOW();			//RUNSCI Hi
	RCING_LOW();
	SMBus2_CLK_INDW;	// SMBus2 CLK to Input+pull_DW pin
	SMBus2_DAT_INDW;	// SMBus2 DATA to Input+pull_DW pin
	FAN_PWM_INDW;		// Set FAN_PWM input+pull_dw.
	FAN_SPEED_INDW;		// Set FAN Speed input+pull_dw.
	FAN1_PWM_INDW;//72JERRY001: Modify GPIO setting and fan1 setting.
	FAN1_SPEED_INDW;//72JERRY001: Modify GPIO setting and fan1 setting.

	CAPLED_OFF();		// Turn off Cap LED.
	/*
	if(IS_MASK_CLEAR(EMStatusBit,b1SetUSBChgEn))
	//USB_ON_INPUT;
	if(IS_MASK_CLEAR(EMStatusBit,b6RdUSBChgS45))
	//USB_ON_INPUT;
	if ( SysPowState == SYSTEM_S0_S5 ) //ANGELAS063:add
	{
		USB_ON_INPUT;	// Disable USB wake.

	}
	else
	{
		if ( IS_MASK_CLEAR(SYS_STATUS,AC_ADP) )
		{ 
			 
        }	// turn off LAN power.
		else
		{ 
			
        }	// Turn on LAN power.
		
		{
			USB_ON_OUTPUT;
			USB_ON_LOW();	// Turn on USB power.
		}
	}*/
	AC_IN_INPUT;

#if UCS1022_Support
	//if ( IS_MASK_SET(USB_Charger, b0USB_EN) )
	if ( IS_MASK_SET(EMStatusBit, b1SetUSBChgEn) )
	{ UCS1002HCACharger_Init(); }
	else 
	{
		if ( SysPowState != SYSTEM_S0_S5 )
		{ UCS1002HCACharger_Init(); }	// Entry S3 status.
	}
#endif	// UCS1022_Support
	CLR_MASK(SYS_MISC1,b5Crisis_LED);	// Clear Crisis LED.
}

void S5DSX_RSMRST_LOW()
{
  RSMRST_LOW();
}
void S5DSX_PCH_PWR_EN_OFF()
{
   Lowpower_ENfunction();
   
}

void Enter_ShipMode()
{
  #if shipmodesupport
  if((ShipModeEn==0xA5)||IS_MASK_SET(EMStatusBit2,b0SetBatteryShipMode)) {
  	ShipModeEn=0x00;
	Lock_ShipMode();
  	}
  #endif
}
void RecordenEnterShipMode()
{
 #if shipmodesupport
  if((ShipModeEn==0xA5)||IS_MASK_SET(EMStatusBit2,b0SetBatteryShipMode)) {
   ProcessSID(0xA6);
  if(nBattGasgauge < 95)
	{
		ProcessSID(0xA8);
		while(1);
	}
  	}
 #endif
}
void S5DSX_CHECK_RSMRST_P()
{
 
}

void S5DSX_EC_ON_LOW()
{
	EC_ON_LOW();
	if(IS_MASK_CLEAR(EMStatusBit,b1SetUSBChgEn))
	EC_ON_5V_LOW();
	if(IS_MASK_CLEAR(EMStatusBit,b6RdUSBChgS45))
	EC_ON_5V_LOW();
	if(nBattGasgauge<=10)
	EC_ON_5V_LOW();
	PowerInitOK=0;//HEGANGS015:Clear pmic initial ok flag in DC S5
	SMBUS_CK3_IN;//72JERRY052:Modify the SMBUS leakage in DC S5.
	SMBUS_DA3_IN;//72JERRY052:Modify the SMBUS leakage in DC S5.
	//EC_ON_1_8V_LOW();//72JERRY037: -Modify power sequence and GPIO setting follow EE.
	// EC_CCG4_ON_LOW();
	PM_PWRBTN_LOW();
}
//72JERRY037: S+Modify power sequence and GPIO setting follow EE.
void S5DSX_EC_ON_18V_LOW()
{
	EC_ON_1_8V_LOW();
}
//72JERRY037:E+ Modify power sequence and GPIO setting follow EE.
void S5DSX_Init_Status()
{
	SysPowState = SYSTEM_DSX;
}


const sPowerSEQ code asPowerSEQS5DSX[]=
{
  { 0x0000,         		   0,   0,  },	// step0
  { S5DSX_RSMRST_LOW,           5,   0,  },  //step1//JERRYBR005:Modify GPIO setting and power sequence.
  { S5DSX_PCH_PWR_EN_OFF,	   5,  	0,  },    //step2//JERRYBR005:Modify GPIO setting and power sequence.
  {S5DSX_EC_ON_18V_LOW,		2,	0,},//72JERRY037: Modify power sequence and GPIO setting follow EE.
  { S5DSX_EC_ON_LOW,	  	 0,	0,  },  //step4
  { S5DSX_Init_Status,		   0,		0,	},  //step5
  { Enter_ShipMode,            120,        0}, //step6
 { RecordenEnterShipMode,		0,       0},//step7
};


 
void Oem_S5DSXSequence(void)
{
  if(PowSeqDelay != 0x00 )
  {
    PowSeqDelay--;        //1m Sec count base
    if(PowSeqDelay != 0x00 )
    {
      return;
    }
  }

  while(PWSeqStep<(sizeof(asPowerSEQS5DSX)/sizeof(sPowerSEQ)))
  {
    if(asPowerSEQS5DSX[PWSeqStep].checkstatus==0x00) // Do function
    {
      (asPowerSEQS5DSX[PWSeqStep].func)();
      	PowSeqDelay = asPowerSEQS5DSX[PWSeqStep].delay;
      	PWSeqStep++;
      if(PowSeqDelay!=0x00)
      {
        break;
      }
    }
    else	// Check input status pin
    {
      if((asPowerSEQS5DSX[PWSeqStep].func)())
      {
        PowSeqDelay = asPowerSEQS5DSX[PWSeqStep].delay;
        PWSeqStep++;
      }
      break;
    }
  }
}


void DSXS5_EC_ON_HI()
{
	SMBUS_CK3_ALT;//72JERRY052:Modify the SMBUS leakage in DC S5.
	SMBUS_DA3_ALT;//72JERRY052:Modify the SMBUS leakage in DC S5.
	EC_ON_HI();
	EC_ON_5V_HI();
	PM_PWRBTN_HI();
}
BYTE DSXS5_PMIC_Initial(void)
{
	InitPMIC();
	if(!PowerInitOK)
		return 0;
	else
		return 1;
}
void DSXS5_EC_ON_1_8HI()
{
	EC_ON_1_8V_HI();
}
void DSXS5_RSMRST_HI()
{
	RSMRST_HI();
}

void DSXS5_Init_Status()
{
	AC_PRESENT_HI();  //72JERRY017: Modify poweron sequence 
    SysPowState = SYSTEM_S5;
}


const sPowerSEQ code asPowerSEQDSXS5[]=
{
  { 0x0000,         		   0,   0,  },	// step0
  { DSXS5_EC_ON_HI,	           0,	0,  },  //step1//72JERRY017: Modify poweron sequence 
  { DSXS5_PMIC_Initial,		   0,	1,	},
  { DSXS5_EC_ON_1_8HI,		   30,  0,  },//72JERRY025: Modify power sequence follow EE.
  { DSXS5_RSMRST_HI,           20,  0,  },  //step6
  { DSXS5_Init_Status,		   0,	0,	}, //step7
};


void Oem_DSXS5Sequence(void)
{
  if(PowSeqDelay != 0x00 )
  {
    PowSeqDelay--;        //1m Sec count base
    if(PowSeqDelay != 0x00 )
    {
      return;
    }
  }

  while(PWSeqStep<(sizeof(asPowerSEQDSXS5)/sizeof(sPowerSEQ)))
  {
    if(asPowerSEQDSXS5[PWSeqStep].checkstatus==0x00) // Do function
    {
      (asPowerSEQDSXS5[PWSeqStep].func)();
      	PowSeqDelay = asPowerSEQDSXS5[PWSeqStep].delay;
      	PWSeqStep++;
      if(PowSeqDelay!=0x00)
      {
        break;
      }
    }
    else	// Check input status pin
    {
      if((asPowerSEQDSXS5[PWSeqStep].func)())
      {
        PowSeqDelay = asPowerSEQDSXS5[PWSeqStep].delay;
        PWSeqStep++;
      }
      break;
    }
  }
}
//============S5 -> S0==========================================================
BYTE S5S0_STEP1(void)
{
    CLR_MASK(cCmd, bPCHPWR_Keep);    
    AutoTimer=0x00; 
    if(IS_MASK_SET(SysStatus2, b3EC8S_Reset)) 
	{
	  	CLEAR_MASK(SysStatus2, b3EC8S_Reset);
	}
	SMBUS_CK3_ALT;//72JERRY052:Modify the SMBUS leakage in DC S5.
	SMBUS_DA3_ALT;//72JERRY052:Modify the SMBUS leakage in DC S5.
	if(ONEKEY_TEMP_FLAG) //HEGANGS025: Modify onekey battery function
	{
		DCR3 = 0;
	}
	EC_ON_HI(); //ANGELAS038:add
	EC_ON_5V_HI();
	PM_PWRBTN_HI();
	return(1);
}

void S5S0_EC_ON_1_8_EN(void) 
{
   EC_ON_1_8V_HI();
}

void S5S0_EC_CCG4_ON_HI(void) 
{
 //  EC_CCG4_ON_HI();
}//72JERRY025: Modify power sequence follow EE.

#if SUPPORT_PMIC
//MEILING002: S+
BYTE S5S0_PMIC_Initial(void)
{
	InitPMIC(); 
	if(!PowerInitOK)
		return 0;
	else
		return 1;
}
//MEILING002: E+
#endif

void S5S0_AC_PRESENT_EN(void) 
{
	uPGID= Read_Eflash_Byte(EEPROMA2,(EEPROMA1_B03 | 0x07) ,0xE0); //:Add the difference between G and Z project
    MBID_FB=uPGID;       
	PM_PWRBTN_HI();
    if(IS_MASK_SET(SYS_STATUS,AC_ADP)) 
    {
	   //AC_PRESENT_HI();
    } 
}


void S5S0_RSMRST_EN(void)
{
	RSMRST_HI();
}

void S5S0_PM_PWER_BTN2(void)
{
	PM_PWRBTN_LOW();
	SET_MASK(CMOS_TEST,BIT2); //ANGELAS052:add
}
// open
void S5S0_PM_PWER_BTN3(void)
{
	PM_PWRBTN_HI();
}

BYTE S5S0_CHK_S4(void)
{
	if(Read_SLPS4())
	{
	    PM_PWRBTN_HI(); 
		//SYSON_HI(); //ANGELAS063:remove
		//EC_VCCST_EN_HI(); //ANGELAS063:add//72JERRY037:- Modify power sequence and GPIO setting follow EE.
		return(1);
	}
    //Reset_Delay=0x44; //:Remove work around
    PM_PWRBTN_LOW(); 
	return(0);
}

BYTE S5S0_CHK_S3(void)
{
	if(Read_SLPS3())
	{
		return(1);
	}
   	//Reset_Delay=0x55;  //Remove work around
	return(0);
}

void S5S0_EC_VCCST_EN(void)
{
	EC_VCCST_EN_HI();
}

//MEILING001:S+
void S5S0_EC_VPP_PWREN(void)
{
	EC_VPP_PWREN_HI();
}
//MEILING001:E+

//ANGELAS063:add start
void S5S0_SYSON(void)
{
	SYSON_HI();
}
//ANGELAS063:add end
void S5S0_GPO7(void)
{
//	VGA_AC_DET_HI();
}
void S5S0_SUSP(void)
{
	SUSP_ON();
    //ROMPWREN_HIGH(); 
	CURRENT_STATUS &= 0xC0; //Clear flags of entering and resuming from S3 & S4 &S5
    //LAN_PWR_IN; 
	SetS0GPIO();
	//USB_CHG_EN_HI();////72JERRY008:-
    //CHG_MOD1_HI();//72JERRY008:-
    //CHG_MOD2_HI();//72JERRY008:-
   // CHG_MOD3_LOW();//72JERRY008:-
}

void S5S0_EC_VCCIO_EN(void)
{
	EC_VCCIO_EN_HI();
}

void S5S0_NVDD_PWR(void)
{
	#if	Support_External_IO
    NVDD_PWR_EN_HI();
	S_NVDD_PWR_EN_HI();
	#endif	// Support_External_IO
}

void S5S0_USBON(void)
{
	USB_ON_OUTPUT;
	USB_ON_LOW();
	SET_MASK(EMStatusBit, b7RdUSBChgEn);	// default support USB charger S4/S5.
}

void S5S0_VCCST_PWRGD(void)
{
	EC_VCCST_PWRGD_HI();
}

//same as S5S0_CHK_SA_PWRGOOD, only change name for clear read.

BYTE S5S0_CHK_VDDQ_PWRGOOD(void)
{
	BYTE BPwrDate=0x03;
    // Reset_Delay=0x00;  //Remove work around
	if(Read_VDDQ_PGOOD())
	{
		//bRWSMBus(SMbusCh4, SMbusWB, TmlIC_Addr, TmlIC_Rate, &BPwrDate, SMBus_NoPEC);
		//LED_KB_PWM_Step=Read_Eflash_Byte(EEPROMA2,(EEPROMA1_B03|0x07),0xE2);//72JERRY039:Modify KB backlight setting follow UI SPEC.
		return(1);
	}
	return(0);
}



void S5S0_VR_ON(void)
{
	Clr_UEFIStatus();	// Clear UEFI status for BIOS used.
	VR_ON_ON();
 
 	// for EM9 adapter detection and protection     
    //SetPowerBatteryparameter(); 
    if (IS_MASK_SET(SYS_STATUS,AC_ADP))   
    {
   		AdapterIDOn_Flag = 0x01; //MEILING001:add.
 		ADPIDON10MS_NUM=0xa0;  // Modify read adapter ID time, in order to enter S4 under DC mode ,then plug in AC and wake up from S4 not pop up limited support.
    } 
	//:for EM9 adapter detection and protection   
	MuteCount = 4;
}

BYTE S5S0_CHK_VRREADY(void)
{
	//if(Read_VR_CPU_PWROK())
	if(Read_VR_CPU_PWROK > CPU_PWROK_VLIMIT)
	{
		return(1);
	}
	return(0);
}

void S5S0_PCH_PWROK(void)
{
	PCH_PWROK_HI();
    PwrOnDly5Sec =5;	//optimize PwrOnDly5Sec,from 40 to 5.
}

void S5S0_SYS_PWROK(void)
{
	SYS_PWROK_HI();
}

void LPC_RST(void)
{
	#if	Support_External_IO
	DGPU_HOLD_RST_HI();
	S_DGPU_RST_HI();
	#endif	// Support_External_IO
}


void S5S0_Init_Status(void)
{
	if(IS_MASK_CLEAR(P80CMOSSts,P80CMOSDis))
	{
		bios_check = 0;//HEGANGS022:Add SMI hang check function
	}
	CLEAR_MASK(ACPI_HOTKEY, b7BIOS_NoShut); //support RTC wake and psw can wake during RTC wake enable  in DC mode except ship mode enable //ANGELAS031
	CLEAR_MASK(ACPI_HOTKEY, b6Cmd_NoShut); //ANGELAS031
	ShipModeEn=0x00; //ANGELAS037: add 
	Fan_ON_Count = FAN_Boot_On;	// Turn on Fan 3 Sec.
	nBrightValue = 17;			// Default WIN8 brightness 110~130 nit.
	nCpuTemp = VGA_TEMP = EXTVGA_TEMP = nSKINTemp90 = 32;	// Default temperature value.
	ThermistorCPU_TEMP=32;	//ANGELAS070:add
	//ANGELAS033:Add start	
  	if(IS_MASK_SET(SEL_STATE0,PRESENT_A))
  	{
    	batteryFirstUsedDateL = batteryFirstUsedDateH = 0x00;
	  	if(bRWSMBus(SMbusChB,SMbusRW,SmBat_Addr,FirstUsedDate,&batteryFirstUsedDateL,SMBus_NeedPEC))
	  	{
			if(batteryFirstUsedDateL || batteryFirstUsedDateH )
		   		CLEAR_MASK(pProject0,b3uBatteryTimeNoOK);
			else
				SET_MASK(pProject0,b3uBatteryTimeNoOK);
			RamDebug(0xAE);
	  	}
  	}			
	//ANGELAS033:Add end
	//SysPowState = SYSTEM_S0;	// Set System S0 status. //ANGELAS045:remove
	LED_KB_PWM_Count = 42;	// Turn on KB BL LED.  //MEILING030:change 3 to 10(1.5s to 5s).//72JERRY039:Modify KB backlight setting follow UI SPEC.
	ACPI_HOTKEY &= 0x1F;	// Clear bit6,7, can cut power in the battery mode.
	SYS_STATUS &= 0xF8;		// Clear OS mode.
	//DSxPowState = SYSTEM_S3S4;	// Clear Deep status. //ANGELAS045:remove
	CLR_MASK(cCmd, b3BkOff);	// Turn on backlight.
	CLR_MASK(OEMControl, b1BkOff); //ANGELAG005:add
	BackLight_En_Delay =16;	// Delay 280msec turn on backlight.//reduce light backlight time for clodboot.from 28 to 16.
    delayEDPTm = 5;                             // Delay 50ms to initial EDP.   // CMW Temp
	//SetACIN_Int(); //ANGELAS079:remove
	SET_MASK(pDevStus, pENABLE_TP);	// Turn on TouchPad.
	//turn on touch sensor power
	//if ( uMBID & 0x08 )  // add MBID judge.
    //add lid flag to BIOS, whether need to open  backlight.
    CLEAR_MASK(USB_Charger, b2USB_TmlDis);//enable USB charge when Unexpected power shutdown//72JERRY046:clear Unexpected power shutdown flag when power on.
    if(Read_LID_SW_IN())
    	SET_MASK(SWI_EVENT,LID);
    else
        CLEAR_MASK(SWI_EVENT,LID);

	 if(!Read_LID_PAD())
	 {
		SET_MASK(pDevStatus1,b2DisableKB);
	 }
	 else
	 {
	   CLEAR_MASK(pDevStatus1,b2DisableKB);  
	 }
    #if SupportReadTPID
    ReadTPid();
    #endif
    uReserve07.fbit.uACInOutBeep = 1;
	//CLEAR_MASK(BATTUPDATEFW,BIT0); //ANGELAS054:add //ANGELAS083:remove
	EC_TP_ON_HI();//enable TP //72JERRY056:Add enable/disable TP function by HW pin.
	CLEAR_MASK(pDevStatus1, b7DisableTP);//enable TP//72JERRY056:Add enable/disable TP function by HW pin.
	Lowpower_DISfunction(); //ANGELAS047:add
	//PECIBIOSTEST=2;//when enter S3/S4/CB,BIOS sent B4 CMD to EC from 66port for peci fail.// : CHANGE FROM 0 TO 2//remove 
	CPUThrottlingDelayTime = 60; //MEILING052:add delay time 1 minute to send 0x1D event.
	SysPowState = SYSTEM_S0;	//ANGELAS045:add set S0 power state.
	DSxPowState = SYSTEM_S3S4;	// Clear Deep status. //ANGELAS045:add
}

/* follow ITE EVB Power Sequence 20111202.pdf
;===========================================================================================
; Power On (DC mode)
;===========================================================================================
;
;S5_ENABLE(O)   +----------------------------------------------------------------------------
;--------------+(1)Always power group 3,5VA (if Adapter in,this signal will exist always)
;
;PM_Power_BTN(O)            +----------------------+    ~       +-----------------------
;----------------------+                                       +--~----+
;
;RSMRST(O)                           +----------------------------------------
;-------------------------+
;
;AC_Present (Intel Only and if AC attached)(O)   +----------------------------------------
;----------------------------------------+
;
;                                                                                                        +---------------------
;SLP_S4#(I) SLP4(I)-------------------------------------------+
;
;                                                                                                               +---------------------
;IMVP_PWRGD (I)  ------------------------------------------------+
;                                                                                                                           +------------------
;S0_Power_Good(O)------------------------------------------------------+
;========================================================================
*/

//-------------------------------------------------------------------------------------
//  Do S5 -> S0 power sequnce
//-------------------------------------------------------------------------------------
const sPowerSEQ code asPowerSEQS5S0[]=
{
	{ 0x0000, 				0,		0,  },	//Dummy step always in the top of tabel
	{ S5S0_STEP1,			0,		1,	},  //72JERRY017: Modify poweron sequence 
	{ S5S0_PMIC_Initial, 	0,		1,	},
	{S5S0_EC_ON_1_8_EN, 	30,     0,  },  //72JERRY025: Modify power sequence follow EE.
//	{S5S0_EC_CCG4_ON_HI,     26,     0, },//72JERRY025: Modify power sequence follow EE.
	{ S5S0_RSMRST_EN,		20,		0,  },
	{ S5S0_AC_PRESENT_EN,	76,		0,  }, 
	{ S5S0_PM_PWER_BTN2,    20,		0,  },
	{ S5S0_PM_PWER_BTN3,    0,      0,  },   
	{ S5S0_CHK_S4,			0, 		1,	}, 
	{ S5S0_EC_VPP_PWREN,    2,      0,  }, 
	{ S5S0_SYSON,			0, 		0,	}, 
	{ S5S0_CHK_VDDQ_PWRGOOD,2,	  	1,	},//72JERRY037: Modify power sequence and GPIO setting follow EE.
	{ S5S0_EC_VCCST_EN,  	2,		0,	},
	{ S5S0_CHK_S3,			0,     	1,  }, 
	{ SLPS3_Sleep_En,       0,      0,  }, 
	{ SLPS4_Sleep_En,       0,      0,  }, 
	{ S5S0_SUSP,			2, 		0,	},  
	{ S5S0_EC_VCCIO_EN,		8, 		0,	},  
    { S5S0_USBON,			50,		0,	},
    { S5S0_VCCST_PWRGD,		5,		0,	},
	{ S5S0_VR_ON,			0,		0,	},    
	{ S5S0_CHK_VRREADY,		10,		1,	},
	{ S5S0_PCH_PWROK,		100,	0,	}, 
	{ S5S0_SYS_PWROK,		49,		0,	},	
	{ S5S0_Init_Status,		0,		0,	},
};

//----------------------------------------------------------------------------
// S5 to S0 sequnce
//----------------------------------------------------------------------------
void Oem_S5S0Sequence(void)
{
    if ( PowSeqDelay != 0x00 )
    {
        PowSeqDelay--; 									//1m Sec count base
        if ( PowSeqDelay != 0x00 )	return;
    }

	while(PWSeqStep<(sizeof(asPowerSEQS5S0)/sizeof(sPowerSEQ)))
	{
		if(asPowerSEQS5S0[PWSeqStep].checkstatus==0x00)	// Do function
		{
			(asPowerSEQS5S0[PWSeqStep].func)();
			PowSeqDelay = asPowerSEQS5S0[PWSeqStep].delay;
			PWSeqStep++;
			if(PowSeqDelay!=0x00)
			{
				break;
			}
		}
		else	// Check input status pin
		{
			if((asPowerSEQS5S0[PWSeqStep].func)())
			{
				PowSeqDelay = asPowerSEQS5S0[PWSeqStep].delay;
			    PWSeqStep++;
			}
			break;
		}
	}
}

//============S0 -> S5==========================================================
void S0S5_VR_ON(void)
{
	SYS_PWROK_LOW();
	PCH_PWROK_LOW();
	VR_ON_OFF();
	EC_VCCST_PWRGD_LOW();
//	USB_ON_INPUT; //ANGELAS031: add
	EC_VCCIO_EN_LOW();
	#if	Support_External_IO
	DGPU_HOLD_RST_LOW();
	S_DGPU_RST_LOW();
	#endif	// Support_External_IO
}

void S0S5_NVDD_OFF(void)
{
	#if	Support_External_IO
	NVDD_PWR_EN_LOW();
	S_NVDD_PWR_EN_LOW();
	#endif	// Support_External_IO
}

void S0S5_SUSP(void)
{
	SUSP_OFF();
	SetS5GPIO();
	//USB_CHG_EN_LOW();//72JERRY045:-Modify USB charge setting in DC S5.
   // CHG_MOD1_LOW();//72JERRY045:-Modify USB charge setting in DC S5.
    //CHG_MOD2_LOW();//72JERRY045:-Modify USB charge setting in DC S5.
    //CHG_MOD3_LOW();//72JERRY045:-Modify USB charge setting in DC S5.
}
void S0S5_EC_VCCST_EN(void)
{
	EC_VCCST_EN_LOW();
}

//MEILING001:S+
void S0S5_EC_VPP_PWREN(void)
{
	EC_VPP_PWREN_LOW();
}
//MEILING001:E+

void S0S5_GPO7(void)
{
	//VGA_AC_DET_LO();
}

void S0S5_SYS_ON(void)
{
	SYSON_LOW();
	Write_NOVOS4();
}
void S0S5_DPWROK_OFF(void)
{
	if(IS_MASK_CLEAR(cCmd, bPCHPWR_Keep))
	{
	    // DPWROK_EC_LOW();
        RSMRST_LOW(); 
    }
}


void S0S5_Init_Status(void)
{
BYTE EVENT_BUFFER_CNT;//72JERRY063:Clear Qevent Buffer when enter S3, S5, and LPC RESET.
	F2_Pressed=0; //ANGELAG001: ADD
	CLEAR_MASK(pProject4,pPWSWdisable);//HEGANGS028:Disable POWERSW when flash bios
	//DSxPowState = SYSTEM_S3S4; //ANGELAS045:remove
	//ANGELAS033:Add start	
/*		
  	if(IS_MASK_SET(SEL_STATE0,PRESENT_A))
  	{
    	if(IS_MASK_SET(pProject0,b1uFUdayClr))
    	{
      		batteryFirstUsedDateH = 0x00;
	  		batteryFirstUsedDateL = 0x00;
      		if(bRWSMBus(SMbusChB,SMbusWW,SmBat_Addr,FirstUsedDate,&batteryFirstUsedDateL,SMBus_NeedPEC))
      		{
        		CLEAR_MASK(pProject0,b1uFUdayClr);
        		RamDebug(0xAD);
      		}
    	}
  	}
*/
	//ANGELAS033:Add end
//72JERRY063:s+Clear Qevent Buffer when enter S3, S5, and LPC RESET.
	for( EVENT_BUFFER_CNT=0; EVENT_BUFFER_CNT<=EVENT_BUFFER_SIZE; EVENT_BUFFER_CNT++)
	{
					 SCI_Event_Buffer[EVENT_BUFFER_CNT] = 0x00;						
	}
	SCI_Event_In_Index	= 0x00; 													  //clear SCI index   
	SCI_Event_Out_Index = 0x00;
	//72JERRY063:e+Clear Qevent Buffer when enter S3, S5, and LPC RESET. 
	Bioswatchdog=0;////ANGELAS106:Optimize ANGELAS094 clear time.
	//SysPowState = SYSTEM_S5; //ANGELAS045:remove
	cDev.fbit.cD_WovMFGtest = 0;//72JERRY065:Add CMD 0x59 test voice wake up for MFG.
	uNovoVPCCount = 0;				// Clear NOVO VPC status.
	uWOVSTATUS =0;					// Clear WOV status.//Add voice wake up cmd for BIOS.
	nCpuTemp = TEMP_Buff_1 = TEMP_Buff_2 = TEMP_Buff_3 = TEMP_Buff_4 = TEMP_Buff_5 = TEMP_Buff_6 = TEMP_Buff_7 = TEMP_Buff_8 = TEMP_Buff_9 = TEMP_Buff_10 = TEMP_Buff_0 =0x00; //:Clear CPU & VGA & thermal IC temp after system shutdown or enter s3.//72JERRY057:Update fan table follow'720S_13IKB fan table_V03'.
	VGA_TEMP = VGA_TBuff3 = VGA_TBuff2 = VGA_TBuff1 = 0x00;	//:Clear CPU & VGA & thermal IC temp after system shutdown or enter s3.
	nNBTemp = nRamTemp =0;//:Clear CPU & VGA & thermal IC temp after system shutdown or enter s3.
	CLEAR_MASK(nBatteryStatH, CMBS_CRITICALLOW); 
	CPU_TYPE &= 0x3f;//:CLEAR BIT6 and BIT7 Unit abnormal shuntdown will not clear GPU flag,so when enter S5,WB and S3 we clear GPU flag to not read GPU temperature.
	CPU_Shut_Cnt = VGA_Shut_Cnt = EXTVGA_Shut_Cnt = 0;	// Clear shutdown conunt.
	ERR_THMSTS = 0;					// Clear thermal status fail bit.
	Thro_Status = Thro_Status2 = 0;	// clear Throttling status.
	ECSleepCount = 3;				// Delay entry EC sleep.
#if TouchPad_only
    PowerOffClearTPPendingData();
#endif
    if(Read_LID_SW_IN())
    	SET_MASK(SWI_EVENT,LID);
    else
        CLEAR_MASK(SWI_EVENT,LID);

       
	if (cSysActionFlag == SysAction_EC_Restart || cSysActionFlag == SysAction_SysShutdown_EC_AutoBoot)
	{
	    WinFlashMark = 0x53;
        WinFlashMark2 = 0x35;
		InternalWDTNow(SYSACTION_ID);
	}
	//else if(( (WinFlashMark == 0xFC)&&(WinFlashMark1==0x55)) || (cSysActionFlag == SysAction_EC_Shutdown)) //ANGELAG012:remove.
	else if(cSysActionFlag == SysAction_EC_Shutdown) //ANGELAG012:add.
	{
		WinFlashMark = 0xAA;
		WinFlashMark1=0x88;
		InternalWDTNow(FLASH_ID);
	}

	Clr_Thermal_Tab();	// Clear Thermal table value.

	if(IS_MASK_SET(SysStatus,LidKBIgnore))
	{
		Ccb42_DISAB_KEY = 0;
		Flag.SCAN_INH = 0;	// not Inhibit scanner (internal keyboard).
		Unlock_Scan();		// UnLock scanner
		CLEAR_MASK(SysStatus,LidKBIgnore);
	}
#if SUPPORT_PMIC
	//ANGELAG007: add start
	if(PowerOVP_REG|PowerUVP_REG|PowerOCP_REG|PowerOTP_REG)
	{
		if ( IS_MASK_CLEAR(SysStatus, ERR_ShuntDownFlag) )
		{ ProcessSID(PMIC_Shutdown_ID); }
	}
	//ANGELAG007: add end

	PollingOTP();//MEILING002:add.
#endif
	if ( IS_MASK_CLEAR(SysStatus, ERR_ShuntDownFlag) )
	{ ProcessSID(S5_ID); }
	CLEAR_MASK(SysStatus,ERR_ShuntDownFlag);	// Clear Shutdown ID bit.
	initMark = 0x00;	// CMW 20121203 for PS8625 translator
	TEMP_Error = 0x00; // CMW 20121203 for PS8625 translator
	PECI_Err_CNT=0;//When enter S3/S4/S4,clear PECI fail count.
	// PECI_INDW;			// Set PECI pin to ALT.//Change PECI initialize after S0 states.//: +remove  AND 
    //CLR_MASK(GCR2, PECIE);     // Enable PECI//Change PECI initialize after S0 states.//: +remove  AND 
	Lowpower_ENfunction(); 
//ANGELAG008: add start
	CLEAR_MASK(pDevStatus3,b6EnDisTPSend);
	CLEAR_MASK(pDevStatus3,b7TPStateFormer);
	CLEAR_MASK(pDevStatus1,b0F6ScanCodeSend);
	CLEAR_MASK(pDevStatus1,b1F6DisableTP);
	CLEAR_MASK(pDevStatus1,b4LNVAPP_STATUS);
	CLEAR_MASK(pDevStatus1,b5TPDRIVER_STATUS);
	CLEAR_MASK(pDevStatus1,b6MODE_STATUS);
	//CLEAR_MASK(pDevStatus1,b7DisableTP);//72JERRY043:Support PTP TP show OSD,there isn't driver notify EC TP status,EC keep flag to rom.
	EC_TP_ON_LOW();//disable tp//72JERRY056:Add enable/disable TP function by HW pin.
	SET_MASK(pDevStatus1, b7DisableTP);//72JERRY056:Add enable/disable TP function by HW pin.
	
//ANGELAG008: add end
	SysPowState = SYSTEM_S5;
	
	if((ONEKEY_TEMP_FLAG)&&(IS_MASK_SET(OEMControl,b3ONEKEY_POWERUP)))//HEGANGS025: Modify onekey battery function
	{
		S4S5_KeyWake(0);
	}
	ONEKEY_FLAG = 0;
	ONEKEY_TEMP_FLAG = 0;
	CLEAR_MASK(OEMControl,b3ONEKEY_POWERUP);
}

//-------------------------------------------------------------------------------------
//  Do S0 -> S5 power sequnce
//-------------------------------------------------------------------------------------
const sPowerSEQ code asPowerSEQS0S5[]=
{
	{ 0x0000, 				0,		0,  },	// Dummy step always in the top of tabel
	{ S0S5_VR_ON,			0,		0,	},  
	{ S0S5_SUSP, 		   	0,		0,  },
	{ S0S5_SYS_ON,			0,		0,	},
	{ S0S5_EC_VCCST_EN,		30,		0,	}, 
	{ S0S5_EC_VPP_PWREN,    10,     0,  },
	{ S0S5_Init_Status,		0,		0,	},
};

//----------------------------------------------------------------------------
// S0 to S5 sequnce
//----------------------------------------------------------------------------
void Oem_S0S5Sequence(void)
{
	if ( PowSeqDelay != 0x00 )
	{
		PowSeqDelay--;				//1m Sec count base
		if ( PowSeqDelay != 0x00 )
		{
			return;
		}
	}

	while(PWSeqStep<(sizeof(asPowerSEQS0S5)/sizeof(sPowerSEQ)))
	{
		if(asPowerSEQS0S5[PWSeqStep].checkstatus==0x00) // Do function
		{
			(asPowerSEQS0S5[PWSeqStep].func)();
			PowSeqDelay = asPowerSEQS0S5[PWSeqStep].delay;
			PWSeqStep++;
			if(PowSeqDelay!=0x00)
			{
				break;
			}
		}
		else											// Check input status pin
		{
			if((asPowerSEQS0S5[PWSeqStep].func)())
			{
				PowSeqDelay = asPowerSEQS0S5[PWSeqStep].delay;
				PWSeqStep++;
			}
			break;
		}
	}
}

//============S0 -> S3==========================================================
void S0S3_VR_ON(void)
{
	SYS_PWROK_LOW();
	PCH_PWROK_LOW();
  	VR_ON_OFF();	
	EC_VCCST_PWRGD_LOW();
	EC_VCCIO_EN_LOW();
	#if	Support_External_IO	//cwy
	NVDD_PWR_EN_LOW();
	S_NVDD_PWR_EN_LOW();
	S_DGPU_PWR_EN_LOW();
	DGPU_PWR_EN_LOW();
	#endif	// Support_External_IO
}

void S0S3_SUSP(void)
{
	SUSP_OFF();
	#if	Support_External_IO
	DGPU_HOLD_RST_LOW();
	S_DGPU_RST_LOW();
	#endif	// Support_External_IO
	SetS5GPIO();
	//USB_CHG_EN_HI();//72JERRY008:-
    //CHG_MOD1_LOW();//72JERRY008:-
    //CHG_MOD2_HI();//72JERRY008:-
    //CHG_MOD3_LOW();//72JERRY008:-
}
void S0S3_EC_VCCST_EN(void)
{
	EC_VCCST_EN_LOW();
}
void S0S3_GPO7(void)
{
//	VGA_AC_DET_LO();
}
void S0S3_Init_Status(void)
{
BYTE EVENT_BUFFER_CNT;//72JERRY063:Clear Qevent Buffer when enter S3, S5, and LPC RESET.
	#if	Support_External_IO
	Init_IT8302();  //cwy
	#endif	// Support_External_IO
	CPU_TYPE &= 0x3f;//:CLEAR BIT6 and BIT7 Unit abnormal shuntdown will not clear GPU flag,so when enter S5,WB and S3 we clear GPU flag to not read GPU temperature.
	CPU_Shut_Cnt = VGA_Shut_Cnt = EXTVGA_Shut_Cnt = 0;	// Clear shutdown conunt.
	nCpuTemp = TEMP_Buff_1 = TEMP_Buff_2 = TEMP_Buff_3 = TEMP_Buff_4 = TEMP_Buff_5 = TEMP_Buff_6 = TEMP_Buff_7 = TEMP_Buff_8 = TEMP_Buff_9 = TEMP_Buff_10 = TEMP_Buff_0 =0x00; //:Clear CPU & VGA & thermal IC temp after system shutdown or enter s3.//72JERRY057:Update fan table follow'720S_13IKB fan table_V03'.
	VGA_TEMP = VGA_TBuff3 = VGA_TBuff2 = VGA_TBuff1 = 0x00;	//:Clear CPU & VGA & thermal IC temp after system shutdown or enter s3.
	nNBTemp = nRamTemp =0;//:Clear CPU & VGA & thermal IC temp after system shutdown or enter s3.
	//SysPowState=SYSTEM_S3;  //ANGELAS045:remove
	//DSxPowState = SYSTEM_S3S4;  //ANGELAS045:remove
//72JERRY063:s+Clear Qevent Buffer when enter S3, S5, and LPC RESET.
	for( EVENT_BUFFER_CNT=0; EVENT_BUFFER_CNT<=EVENT_BUFFER_SIZE; EVENT_BUFFER_CNT++)
	{
					 SCI_Event_Buffer[EVENT_BUFFER_CNT] = 0x00;						
	}
	SCI_Event_In_Index	= 0x00; 													  //clear SCI index   
	SCI_Event_Out_Index = 0x00; 
	//72JERRY063:e+Clear Qevent Buffer when enter S3, S5, and LPC RESET.
#if TouchPad_only
    PowerOffClearTPPendingData();
#endif  
	Clr_Thermal_Tab();				// Clear Thermal table value.
	ERR_THMSTS = 0;					// Clear thermal status fail bit.
	Thro_Status = Thro_Status2 = 0;	// clear Throttling status.  
	AOAC_STATUS &= 0x10;			// Clear ISCT status.
	initMark = 0x00;	// CMW 20121203
	TEMP_Error = 0x00;	// CMW 20121203
   	// EC_TS_ON_INPUT;   //   turn off touch sensor power
   // EC_TS_ON_LOW();//Turn off touch pad sensor power in s5/s3.  //MEILING001:add.
    PECI_Err_CNT=0;//:When enter S3/S4/S4,clear PECI fail count.
   	// PECI_INDW;			// Set PECI pin to ALT.//:Change PECI initialize after S0 states.//: +remove  AND 
  	// CLR_MASK(GCR2, PECIE);     // Enable PECI//:Change PECI initialize after S0 states.//: +remove  AND
	Lowpower_ENfunction(); //ANGELAS047:add
//ANGELAG008: add start
	CLEAR_MASK(pDevStatus3,b6EnDisTPSend);
	CLEAR_MASK(pDevStatus3,b7TPStateFormer);
	CLEAR_MASK(pDevStatus1,b0F6ScanCodeSend);
	CLEAR_MASK(pDevStatus1,b1F6DisableTP);
    CLEAR_MASK(pDevStatus1,b2DisableKB);
	 Enable_Any_Key_Irq();
	YogaModeStatus = 0x00;
	CLEAR_MASK(pDevStatus1,b4LNVAPP_STATUS);
	CLEAR_MASK(pDevStatus1,b5TPDRIVER_STATUS);
		CLEAR_MASK(pDevStatus1,b6MODE_STATUS);
	EC_TP_ON_LOW();//disable tp//72JERRY056:Add enable/disable TP function by HW pin.
	SET_MASK(pDevStatus1, b7DisableTP);//72JERRY056:Add enable/disable TP function by HW pin.
		//CLEAR_MASK(pDevStatus1,b7DisableTP);//72JERRY043:Support PTP TP show OSD,there isn't driver notify EC TP status,EC keep flag to rom.
//ANGELAG008: add end
   	SysPowState=SYSTEM_S3;  //ANGELAS045:add
   	//72JERRY050:S+Modify the USB mouse can wake up from S3 at DC mode when the LID closed,not follow UI SPEC.
   if((!Read_LID_SW_IN())&& !Read_AC_IN())
   	{USB_ON_INPUT;
   	}
	//72JERRY050:E+Modify the USB mouse can wake up from S3 at DC mode when the LID closed,not follow UI SPEC.
	DSxPowState = SYSTEM_S3S4;  //ANGELAS045:add
   
}

//-------------------------------------------------------------------------------------
//  Do S0 -> S3 power sequnce
//-------------------------------------------------------------------------------------
const sPowerSEQ code asPowerSEQS0S3[]=
{
  	{ 0x0000,         		0,    	0,  },  // Dummy step always in the top of tabel
  	{ S0S3_VR_ON,  	      	0,     	0,  }, 
  	{ S0S3_SUSP,     		0,    	0,  },  
  	{ S0S3_Init_Status,		0,		0,	},
};

//----------------------------------------------------------------------------
// S0 to S3 sequnce
//----------------------------------------------------------------------------
void Oem_S0S3Sequence(void)
{
  	if(PowSeqDelay != 0x00 )
  	{
    	PowSeqDelay--;        //1m Sec count base
    	if(PowSeqDelay != 0x00 )
    	{
    		return;
    	}
  	}

  	while(PWSeqStep<(sizeof(asPowerSEQS0S3)/sizeof(sPowerSEQ)))
  	{
    	if(asPowerSEQS0S3[PWSeqStep].checkstatus==0x00) // Do function
    	{
      		(asPowerSEQS0S3[PWSeqStep].func)();
      		PowSeqDelay = asPowerSEQS0S3[PWSeqStep].delay;
      		PWSeqStep++;
      		if(PowSeqDelay!=0x00)
      		{
        		break;
      		}
    	}
    	else                      // Check input status pin
    	{
      		if((asPowerSEQS0S3[PWSeqStep].func)())
      		{
        		PowSeqDelay = asPowerSEQS0S3[PWSeqStep].delay;
        		PWSeqStep++;
      		}
      		break;
    	}
  	}
}

//============S3 -> S0==========================================================
void S3S0_PM_PWER_BTN1(void)
{
	PM_PWRBTN_LOW();
}

void S3S0_PM_PWER_BTN2(void)
{
	PM_PWRBTN_HI();
	SysPowState=SYSTEM_S3;
}
void S3S0_EC_VCCST_EN(void)
{
	EC_VCCST_EN_HI();
}
void S3S0_GPO7(void)
{
//VGA_AC_DET_HI();
}
void S3S0_SUSP(void)
{
	PM_PWRBTN_HI();
	SUSP_ON();
	SetS0GPIO();
	USB_ON_OUTPUT;
	USB_ON_LOW();
	//USB_CHG_EN_HI();//72JERRY008:-
    //CHG_MOD1_HI();//72JERRY008:-
   // CHG_MOD2_HI();//72JERRY008:-
   // CHG_MOD3_LOW();//72JERRY008:-
    //LAN_PWR_IN; 
	#if	Support_External_IO
	DGPU_PWR_EN_HI();
	S_DGPU_PWR_EN_HI();
	Delay1MS(7);
	NVDD_PWR_EN_HI();
	S_NVDD_PWR_EN_HI();
	#endif	// Support_External_IO
}

void S3S0_EC_VCCIO_EN(void)
{
	EC_VCCIO_EN_HI();
}

void S3S0_VCCST_PWRGD(void)
{
	EC_VCCST_PWRGD_HI();
}

void S3S0_VR_ON(void)
{
	VR_ON_ON();
	MuteCount = 18;  
	//: for EM9 adapter detection and protection    
    if (IS_MASK_SET(SYS_STATUS,AC_ADP))   
    {
   		//ADAPTERIDON_LOW(); //MEILINNG001:remove.
   		AdapterIDOn_Flag = 0x01; //MEILING001:add.
 		ADPIDON10MS_NUM=0x20;
    }
	//:for EM9 adapter detection and protection 
}
void PCH_PWROK(void)
{
	PCH_PWROK_HI();
}


void S3S0_Init_Status(void)
{
	Fan_ON_Count = FAN_Boot_On;	// Turn on Fan 3 Sec.
	//SysPowState = SYSTEM_S0; //ANGELAS045:remove
	//DSxPowState = SYSTEM_S3S4;	// Clear Deep status. //ANGELAS045:remove
    RamDebug(0x9B);
    // AOAC_INPUT();   
	//LED_KB_PWM_Count = 3;	// Turn on KB BL LED.  //MEILING030:remove.
	CLR_MASK(cCmd, b3BkOff);	// Turn on backlight.
	CLR_MASK(OEMControl, b1BkOff); //ANGELAG005:add
	BackLight_En_Delay = 3;		// Delay 30msec turn on backlight.
    delayEDPTm = 40;                           // Delay 100ms to initial EDP. // CMW Temp
	//SetACIN_Int(); //ANGELAS079:remove
	SET_MASK(pDevStus, pENABLE_TP);	// Turn on TouchPad.
	CURRENT_STATUS &= 0xC0; 		//(JSN20120614) Clear flags of entering and resuming from S3 & S4 &S5
	//turn on touch sensor power
	//if ( uMBID & 0x08 )  //T28A +: add MBID judge.
	//if ( uMBID & 0x08 )  //T28A +: add MBID judge.  //T028C+
     {
          //EC_TS_ON_OUTPUT;
          //EC_TS_ON_LOW();//Remove  for SIT unit.//:-Add touch panel control signal. 
          //EC_TS_ON_HI(); //MEILING001:add.
     }      
	//if ( (IS_MASK_SET(ACPI_HOTKEY,b5BIOS_IFFS)) )
	//{ EC_LID_OUT_HI(); }
	Lowpower_DISfunction(); //ANGELAS047:add
	EC_TP_ON_HI();//enable TP//72JERRY056:Add enable/disable TP function by HW pin.
	CLEAR_MASK(pDevStatus1, b7DisableTP);//enable TP//72JERRY056:Add enable/disable TP function by HW pin.
	SysPowState = SYSTEM_S0;   //ANGELAS045:add
	DSxPowState = SYSTEM_S3S4;	// Clear Deep status. //ANGELAS045:add
}

//-------------------------------------------------------------------------------------
//  Do S3 -> S0 power sequnce
//-------------------------------------------------------------------------------------
const sPowerSEQ code asPowerSEQS3S0[]=
{
  	{ 0x0000,         		0,    	0,  },	// Dummy step always in the top of tabel
  	{ S3S0_PM_PWER_BTN1,	20,	  	0,  },
  	{ S3S0_PM_PWER_BTN2,	2,    	0,  },
	{ S3S0_SUSP,     		2,    	0,  },		
	{ S3S0_EC_VCCIO_EN,     50,    	0,  },		
	{ S3S0_VCCST_PWRGD,     5,    	0,  },		
  	{ S3S0_VR_ON,			0,		0,	},
  	{ SLPS3_Sleep_En,       0,      0,  }, //ANGELAS063:add
  	{ SLPS4_Sleep_En,       0,      0,  }, //ANGELAS063:add
 	{ S5S0_CHK_VRREADY,		10,		1,	},	//  there S5S0... same as S3S0... 
 	{ S5S0_PCH_PWROK,		100,	0,	},	//  there S5S0... same as S3S0...  //LMLKBL0013:modify delay time from 85 to 100.
	{ S5S0_SYS_PWROK,		110,	0,	},	//  there S5S0... same as S3S0...
	{ S3S0_Init_Status,		0,		0,	},
};

//----------------------------------------------------------------------------
//S3 to S0 Sequence
//----------------------------------------------------------------------------
void Oem_S3S0Sequence(void)
{
  	if(PowSeqDelay != 0x00 )
  	{
    	PowSeqDelay--;        //1m Sec count base
    	if(PowSeqDelay != 0x00 )
    	{
      		return;
    	}
  	}

  	while(PWSeqStep<(sizeof(asPowerSEQS3S0)/sizeof(sPowerSEQ)))
  	{
    	if(asPowerSEQS3S0[PWSeqStep].checkstatus==0x00) // Do function
    	{
      		(asPowerSEQS3S0[PWSeqStep].func)();
      		PowSeqDelay = asPowerSEQS3S0[PWSeqStep].delay;
      		PWSeqStep++;
      		if(PowSeqDelay!=0x00)
      		{
        		break;
      		}
    	}
    	else	// Check input status pin
    	{
      		if((asPowerSEQS3S0[PWSeqStep].func)())
      		{
        		PowSeqDelay = asPowerSEQS3S0[PWSeqStep].delay;
        		PWSeqStep++;
      		}
      		break;
    	}
  	}
}
//=============================================================================
void CheckAutoPowerOn(void)
{
	if ((WinFlashMark == 0x53) && (WinFlashMark2 == 0x35) && (SystemIsS5 || SystemIsDSX))
	{
		//AutoTimer = 0x01;
		AutoTimer = 0x83;
		WinFlashMark = 0;
       	WinFlashMark2 = 0;
	}
}

void Check_EC_ON()
{
	//if ( IS_MASK_CLEAR(ACPI_HOTKEY, b7BIOS_NoShut)&&(LOWBATT_3TIMES==0)&&((SysStatus2&0x03)== 0))  	// Not cut power in the battery mode for BIOS used.
	if ( IS_MASK_CLEAR(ACPI_HOTKEY, b7BIOS_NoShut)&&(LOWBATT_3TIMES==0))  
	{
		if ( IS_MASK_CLEAR(ACPI_HOTKEY, b6Cmd_NoShut) )	// Not cut power in the battery mode for command used.
		{
            //if (!Read_AC_IN())   
            //if (!Read_AC_IN()&& (DSxPowState == SYSTEM_DSxOK))//SysPowState
            if (!Read_AC_IN()&& (SysPowState == SYSTEM_S5))
            {
			    //T31- EC_ON_LOW();
			    //EC_ON_Flag =0x55;  
			#if shipmodesupport		//:add ship mode judge
				if(ShipModeEn==0xA5)
				{
				  	ShipModeEn=0x00;
				   	Lock_ShipMode();
                  	GPCRC1=INPUT;
                   	GPCRC2=INPUT;
				   	Delay1MS(100);				   			   
				}
			#endif
                     			//Delay1MS(10); 
				RSMRST_LOW();
				Delay1MS(2); 
				//PCH_PWR_EN_OFF();
				Delay1MS(12); //:Add 12 ms delay between PCH_PWR_EN_OFF and EC_ON_LOW
				/*//ANGELAS038:remove start
				//ANGELAS005:s+ Support UMA SKU EC_ON pin design.
				//if(NTC_V1>682) 	//ANGELAS007:Optimize power on sequence.
				if(NTC_FLAG)//ANGELAS007:Optimize power on sequence.
				{
					CLR_MASK(GPDRF,BIT(0));
					//GPCRF0 = INPUT; //ANGELAS020
				}
				else
				//ANGELAS005:e+ Support UMA SKU EC_ON pin design.
				*///ANGELAS038:remove end
			    EC_ON_LOW();    
				PM_PWRBTN_LOW();
				//LID_readytime = 11 ;	 //ANGELAS007:Optimize power on sequence.									// LID switch wakeup time = 100ms
				CLR_MASK(LENOVOPMFW_Temp,b3EC_ONStatus);
				SET_MASK(LENOVOPMFW_Temp,b5EC_ON_EN);		  
		    }
    
			return;
		}
	}
	SET_MASK(LENOVOPMFW_Temp,b3EC_ONStatus);
	CLR_MASK(LENOVOPMFW_Temp,b5EC_ON_EN);
}

//ANGELAS041:add start
void check8sreset(void)
{
	
	RamDebug(ShutDnCause);
	Init_SMBus_Regs();
	if(IS_MASK_SET(PWRSW8SRST,BIT1))  //ANGELAS089:add
    {  
        SET_MASK(SysStatus2, b3EC8S_Reset);
        RamDebug(0xA8);
    }
    else 
    {
        CLEAR_MASK(SysStatus2, b3EC8S_Reset);
        RamDebug(0xA9);
    }
}
//ANGELAS041:add end



void SendSynapticsIDSequence()
{
    BYTE Temp;

	Temp=0;
	do 
	{
		Send2PortNWait(0x02, 0xE8, 1);
		Send2PortNWait(0x02, 0x00, 1);
		Temp++;
	}while(Temp<4);

	Send2PortNWait(0x02, 0xE9, 4);
}

void SendElanIDSequence()
{
    Send2PortNWait(0x02, 0xE6, 1);
    Send2PortNWait(0x02, 0xE6, 1);
    Send2PortNWait(0x02, 0xE6, 1);
    Send2PortNWait(0x02, 0xE9, 4);
}
void SendALPSIDSequence()
{
    Send2PortNWait(0x02, 0xE7, 1);
    Send2PortNWait(0x02, 0xE7, 1);
    Send2PortNWait(0x02, 0xE7, 1);
    Send2PortNWait(0x02, 0xE9, 4);
}
void ReadTPid()
{
    //BYTE Temp;
    // if((TP_Type == 0x01)||(TP_Type == 0x02)||(TP_Type == 0x03)) //G34: remove read TP ID only one time.
       // return;
    SendSynapticsIDSequence();

    if(TPID3 == 0x47)
    {
        TP_Type = 0x02;		// Synatics TouchPad.
    }
    else
    {
        SendElanIDSequence();
        if(TPID2 == 0x3C)
        {
            TP_Type = 0x01;	// ELAN ToucnPad.
        }
       else
      {
           SendALPSIDSequence();
           if(TPID2==0x73)
          {
                TP_Type =0x03;
          }
           
       }  
   	}

   	/*
    	Temp = Read_Eflash_Byte(0x01,0xB0,0xEF);

    	if(Temp != TP_Type)
    	{
        	eEEPROMBank= 0x00;
        	eEEPROMAddrsss = 0xEF;
        	eEEPROMData	= TP_Type;
        	Update_EEPROMMark();
    	}
	*/
}

void Write_NOVOS4(void)
{
	if(IS_MASK_CLEAR(LENOVOPMFW_Temp,EEPROM_Token))
	{
		EEPROM_PwrSts = Read_Eflash_Byte(EEPROMA2,(EEPROMA1_B03 | 0x07) ,0xDF);
		if(CURRENT_STATUS & ENTER_S4) // S4
	  	{	// Entering S4
			//if ( (SYS_STATUS & 0x07) == 0x04 )
			if (( (SYS_STATUS & 0x07) >= 0x04 )&&( (SYS_STATUS & 0x07) <= 0x06))	// Win8 or win10 ANGELAS010: add win10 mode
			{	// Win8
				if( (EEPROM_PwrSts & 0x10) == 0 )		// check S4 bit to before.
			    {
					if( (EEPROM_PwrSts & 0x01) != 0 )	// check WIN8 bit to before.
					{
						eEEPROMAddrsss = 0xDF;
						eEEPROMBank = 0x07;
						eEEPROMData = EEPROM_PwrSts = 0xEE;
						Update_EEPROMMark();
					}
			    }
				else
				{
					eEEPROMAddrsss = 0xDF;
					eEEPROMBank = 0x07;
					eEEPROMData = EEPROM_PwrSts = 0xEE;
					Update_EEPROMMark();
				}
			}
			else
			{	// Win7 or other.
				if( (EEPROM_PwrSts & 0x10) == 0 )		// check S4 bit to before.
			    {
					if( (EEPROM_PwrSts & 0x01) == 0 )	// check WIN8 bit to before.
					{
						eEEPROMAddrsss = 0xDF;
						eEEPROMBank = 0x07;
						eEEPROMData = EEPROM_PwrSts = 0xEF;
						Update_EEPROMMark();
					}
			    }
				else
				{
					eEEPROMAddrsss = 0xDF;
					eEEPROMBank = 0x07;
					eEEPROMData = EEPROM_PwrSts = 0xEF;
					Update_EEPROMMark();
				}
			}
		}
	  	else // S5
	  	{
			if ( (EEPROM_PwrSts & 0x10) == 0 )		// check S4 bit to before.
			{
				eEEPROMAddrsss = 0xDF;
				eEEPROMBank = 0x07;
				eEEPROMData = EEPROM_PwrSts = 0xFF;
				Update_EEPROMMark();
			}
			else
			{
				if ( (EEPROM_PwrSts & 0x01) == 0 )	// check S4 bit to before.
				{
					//if ( (SYS_STATUS & 0x07) != 0x04 )	// Check WIN8.
					if (!( (SYS_STATUS & 0x07) >= 0x04 )&&( (SYS_STATUS & 0x07) <= 0x06))	// check not Win8, win8.1  or win10 ANGELAS010: add win10 mode
					{
						eEEPROMAddrsss = 0xDF;
						eEEPROMBank = 0x07;
						eEEPROMData = EEPROM_PwrSts = 0xFF;
						Update_EEPROMMark();
					}
				}
			}
	  	}
	}
}

//-----------------------------------------------------------------------------
// The function of pulsing S.B. power button 100ms
//-----------------------------------------------------------------------------
void PulseSBPowerButton(void)
{
	PM_PWRBTN_LOW();
    SBSWReleaseCunt = 20;   // decrease timer base is 10ms total time 200ms
}


//-----------------------------------------------------------------------------
// The function of releasing S.B. power button
//-----------------------------------------------------------------------------
void CheckSBPowerButton(void)
{
	if(SBSWReleaseCunt!=0x00)
	{
		SBSWReleaseCunt--;
		if(SBSWReleaseCunt==0x00)
		{
			PM_PWRBTN_HI();
		}
	}
}

void GC6_FBClamp(void)
{
	
	if( (IS_MASK_SET(SYS_MISC1, ACPI_OS)) && (SystemNotS3) && (SystemNotS5) )
	{  
		// for GC6  function control by interrupt
        if(IS_MASK_CLEAR(FBClamp, REQactive))
       	{
			WUEMR7 |= 0x02;
			WUESR7 = 0x02;
			SET_MASK(IER9, Int_WKO71);
			SET_MASK(FBClamp, REQactive);
        }
    }
	else
	{
		//FB_Clamp_OFF();
		CLR_MASK(FBClamp, FBClamp_EN);

		if(IS_MASK_SET(FBClamp, REQactive))
		{
			CLR_MASK(IER9, Int_WKO71);
			CLEAR_MASK(FBClamp, REQactive);
		}
	}
	/*    
       if(!Read_GC6_EVENT()&& IS_MASK_SET(FBClamp, FBClamp_REQ))
	{
		if(IS_MASK_CLEAR(FBClamp, FBClamp_EN))
		{
			FB_Clamp_ON();
			SET_MASK(FBClamp, FBClamp_EN);
			return;
		}
		else
		{
			FB_Clamp_OFF();
			CLR_MASK(FBClamp, FBClamp_EN);
			return;
		}
	}
	}

	else
	{
		FB_Clamp_OFF();
		CLR_MASK(FBClamp, FBClamp_EN);
	}
	*/   
}

//ANGELAS047:add start
void Lowpower_ENfunction(void)
{
	SET_MASK(CHGIC_WriteCmd0x12H,LOWPWR_EN);
  	if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ChargerMode, &CHGIC_WriteCmd0x12L,SMBus_NoPEC)) //ANGELAS059:0x12H to 0x12L 
    {
		CHGIC_SMbusFailCnt++;
			   
    }
}

void Lowpower_DISfunction(void)
{
	CLEAR_MASK(CHGIC_WriteCmd0x12H,LOWPWR_EN);
  	if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ChargerMode, &CHGIC_WriteCmd0x12L,SMBus_NoPEC)) //ANGELAS059:0x12H to 0x12L 
    {
		 CHGIC_SMbusFailCnt++;
	}
}
//ANGELAS047:add end
#if SUPPORT_PMIC
const sPMICStruct code powerInit[] =
{	// command         REG                      	
//	{ SDWN,			&PowerSDWN		  }, // HEGANGS008£ºMofify power setting
	{ EN_MASK,		&PowerEN_MASK	  },
	{ EN_RW,		&PowerEN_RW		  },
	{ EN_DEGL_SEL1,	&PowerEN_DEGL_SEL1},
	//{ EN_DEGL_SEL2,	&PowerEN_DEGL_SEL2},
	{ BUCK1CTRL,	&PowerBUCK1CTRL	  },
	{ BUCK2CTRL,	&PowerBUCK2CTRL	  },
	{ BUCK3CTRL,	&PowerBUCK3CTRL	  },
	{ LDO1CTRL,		&PowerLDO1CTRL	  },
	{ LDO2CTRL,		&PowerLDO2CTRL	  },
	{ DISCHCTRL1,	&PowerDISCHCTRL1  },
	{ DISCHCTRL2,	&PowerDISCHCTRL2  },
	{ OC_CTRL,		&PowerOC_CTRL	  },
	{ BUCK_FSW,		&PowerBUCK_FSW	  },
};

BYTE CheckInitPMIC(void)
{
	PowerReturnData=0xFE; //ANGELAN011 add
	if(!PowerIICWriteByte(0xE2,0x00)) //ANGELAN011 03 to 00
	{
		RamDebug(0xC1);
		return 0;
	}
	return 1;
}
void PMICID(void)
{
	if(!bRWSMBus(SMbusChA, SMbusRB, powerICaddress, VENDODID, &PowerVENDODID,SMBus_NoPEC))
	{
		PowerInitError++;
	}
}
void InitPMICReg(void)
{
	if(PowerVENDODID == 0)
	{
		PMICID(); //
	}
	PowerInitIndex=0;
	//PowerSDWN = 0x00; // HEGANGS008£ºMofify power setting
	PowerEN_MASK = 0x80; //
	PowerEN_RW = 0x80;  //set to 0x80 allow EC write PMIC

	if(PowerVENDODID==0x53) //5028
	{ 
		PowerBUCK1CTRL = 0x26; 
		if(Read_EC_ID()) 
		{
			PowerBUCK2CTRL = 0x24; // GPU is N16
		}
		else       
		{
			PowerBUCK2CTRL = 0x2C;// GPU is N17
		}
		PowerEN_DEGL_SEL1=0x04;
		PowerBUCK3CTRL = 0x36; 
		PowerLDO1CTRL = 0x1E;  
		PowerLDO2CTRL = 0x02; 
		PowerDISCHCTRL1 = 0x15;
		PowerDISCHCTRL2 = 0x05;
		PowerOC_CTRL = 0x26;
        PowerBUCK_FSW = 0x03; 
	}
	else //5075 
	{

		PowerBUCK1CTRL = 0x26; 
		if(Read_EC_ID()) 
		{
			PowerBUCK2CTRL = 0x08; // GPU is N16
		}
		else       
		{
			PowerBUCK2CTRL = 0x0C;// GPU is N17
		} 
		PowerEN_DEGL_SEL1=0x04;
		PowerBUCK3CTRL = 0x36; 
		PowerLDO1CTRL = 0x1E;  
		PowerLDO2CTRL = 0x04;
		PowerDISCHCTRL1 = 0x15;
		PowerDISCHCTRL2 = 0x05;
		PowerOC_CTRL = 0x26;
		PowerBUCK_FSW = 0x00; 
	} 

	while(PowerInitIndex < (sizeof(powerInit)/sizeof(sPMICStruct)))
	{
		if(PowerIICWriteByte(powerInit[PowerInitIndex].cmd, *powerInit[PowerInitIndex].smbdataA))
		{
			PowerInitIndex++;
		}
		else
		{
			PowerInitIndex++;
			RamDebug(0xC2);
			RamDebug(PowerInitErrorCMD);
			RamDebug(PowerInitErrorDATA);
		}
	}

	if(!PowerIICWriteByte(0xD1,0xBF))  // set to 0xBF, then PMIC run
	{
		RamDebug(0xC3);
		RamDebug(PowerInitErrorCMD);
		RamDebug(PowerInitErrorDATA);
	}
}

void InitPMIC(void)
{
	if(!PowerInitOK)
	{	
		if(CheckInitPMIC())
		{
			InitPMICReg();
			RamDebug(0xC0);
			PowerInitOK=1;
		}
	}
}

BYTE PowerIICWriteByte(BYTE Reg_offset, BYTE PS_data)
{
	PMIC_counter = 0;
	
	while(1)
	{
		if(!bRWSMBus(SMbusChA, SMbusWB, powerICaddress, Reg_offset, &PS_data, SMBus_NoPEC))
		{
			PowerInitError++;
			ResetSMBus(SMbusChA); 
		}
		if(!bRWSMBus(SMbusChA, SMbusRB, powerICaddress, Reg_offset, &PowerReturnData,SMBus_NoPEC))
		{
			PowerInitError++;
			ResetSMBus(SMbusChA);
		}
		if(PowerReturnData == PS_data)
		{
			return 1;
		}
		else
		{
			if(PMIC_counter == 5)
			{
				return 0;
			}
			PowerInitErrorCMD=Reg_offset;
			PowerInitErrorDATA=PowerReturnData;
			PMIC_counter++;
		}
	}
}

void PollingOTP(void)
{
	if(PowerInitOK==1)
	{
	//ANGELAG007: add start
		if(!bRWSMBus(SMbusChA, SMbusRB, powerICaddress, OVP_REG, &PowerOVP_REG,SMBus_NoPEC))
		{
			PowerInitError++;
		}
		if(!bRWSMBus(SMbusChA, SMbusRB, powerICaddress, UVP_REG, &PowerUVP_REG,SMBus_NoPEC))
		{
			PowerInitError++;
		}
		if(!bRWSMBus(SMbusChA, SMbusRB, powerICaddress, OCP_REG, &PowerOCP_REG,SMBus_NoPEC))
		{
			PowerInitError++;
		}
		//ANGELAG007: add end
		if(!bRWSMBus(SMbusChA, SMbusRB, powerICaddress, OTP_REG, &PowerOTP_REG,SMBus_NoPEC))
		{
			PowerInitError++;
		//	ResetSMBus(SMbusChA); //ANGELAG007: REMOVE
		}
	}
}
//MEILING002: E+
#endif
