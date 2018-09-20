/*-----------------------------------------------------------------------------
 * TITLE: OEM_6064.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

void Cmd_B4(void)
{
	//Stop charger when EC and Bios flash
	SET_MASK(nStopChgStat3H, ECFLASH);
	FAN_PWM_OUT;		// Set FAN_PWM OUTPUT.
	EC_FAN_PWM_HI();	// Set FAN Full On.

	CHGIC_ptr = 2;		//reminder: call to write to SmartChg
	WriteSmartChgIC();
	RamProgram(00);
}

void GetData2(BYTE nPort)
{
	TR1 = 0;					// Disable timer1
	ET1 = 0;					// Disable timer1 interrupt
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	TH1 = Timer_20ms>>8;		// Set timer1 counter 20ms
	TL1 = Timer_20ms;			// Set timer1 counter 20ms
	TF1 = 0;					// Clear overflow flag
	TR1 = 1;					// Enable timer1

	while (!TF1)
	{
		if (nPort == 0x60)
		{
			if(IS_MASK_SET(KBHISR,IBF))
			{
				if(IS_MASK_CLEAR(KBHISR,C_D))	// Data Port
				{
					CmdData2 = KBHIDIR;
				}
				TR1 = 0;			// Disable timer 1
				TF1 = 0;			// clear overflow flag
				ET1 = 1;			// Enable timer1 interrupt
				return;
			}
		}
		else if (nPort == 0x62)
		{
			if(IS_MASK_SET(PM1STS,IBF))
			{
				if(IS_MASK_CLEAR(PM1STS,C_D))	// Data Port
				{
					CmdData2 = PM1DI;
				}
				TR1 = 0;			// Disable timer 1
				TF1 = 0;			// clear overflow flag
				ET1 = 1;			// Enable timer1 interrupt
				return;
			}
		}
	}
	TR1 = 0;					// Disable timer 1
	TF1 = 0;					// clear overflow flag
	ET1 = 1;					// Enable timer1 interrupt
}


//----------------------------------------------------------------------------
// The function of 60 port
//  Note :
//  KBHIStep != 0x00 is necessary.
//----------------------------------------------------------------------------
void Hook_60Port(BYTE KBHICmd)
{
	BYTE i;

	switch(KBHICmd)
	{
	case 0x40:
		Cmd_40(KBHIData);
		break;
	case 0x41:
		if(KBHIData==0xA0) Cmd_41_A0(0x60);
		if(KBHIData==0xA1) Cmd_41_A1(0x60);
		if(KBHIData==0xA2) Cmd_41_A2(0x60);
		if(KBHIData==0xA3) Cmd_41_A3(0x60);
		break;
	case 0x42:
		Cmd_42(KBHIData);
		break;
	case 0x43:
		Cmd_43(0x60,KBHIData);
		break;
	case 0x45:
		Cmd_45(0x60,KBHIData);
		break;

	case 0x46:
		Cmd_46(0x60,KBHIData);
		break;
	case 0x47:
		Cmd_47(0x60,KBHIData);
		break;
	case 0x49:
		break;
	case 0x4B:
		if (KBHIStep == 0x04)
		{
			i = KBHIData;
			break;
		}
		if (KBHIStep == 0x03)
		{
			eEEPROMAddrsss = KBHIData;
			if ( eEEPROMAddrsss == 0xE0 )	// Check MBID address low byte.
			{ 
				SET_MASK(MBID_Reload, b0MBID_High); 
            }
			break;
		}
		if (KBHIStep == 0x02)
		{
			eEEPROMBank = KBHIData;
			if ( eEEPROMBank == 0x07 )	// Check MBID address high byte.
			{ 
				SET_MASK(MBID_Reload, b1MBID_LOW); 
            }
			break;
		}
		if (KBHIStep == 0x01)
		{
			eEEPROMData	= KBHIData;//Add the difference between G and Z project
            SET_MASK(LENOVOPMFW_Temp,EEPROM_Token);
			Update_EEPROMMark();
            CLEAR_MASK(LENOVOPMFW_Temp,EEPROM_Token);
			Data2PortDirect(0x60, 0x00);                     
		}

		if ( (MBID_Reload & 0x03) == 0x03 )
		{
			MBID_Reload = 0;
		}
		else
		{ 
			MBID_Reload = 0; 
        }
		break;

	case 0x4C:
		if (KBHIStep == 0x03)
		{
			i = KBHIData;
			break;
		}
		if (KBHIStep == 0x02)
		{
			eEEPROMAddrsss = KBHIData;
			break;
		}
		if (KBHIStep == 0x01)
		{
			eEEPROMBank = KBHIData;
			Cmd_4E(0x60,eEEPROMAddrsss);
		}
		break;

	case 0x4D:
		if (KBHIStep == 0x02)
		{
			eEEPROMAddrsss = KBHIData;
			break;
		}
		if (KBHIStep == 0x01)
		{
			eEEPROMData	= KBHIData;
            SET_MASK(LENOVOPMFW_Temp,EEPROM_Token);
			Update_EEPROMMark();
            CLEAR_MASK(LENOVOPMFW_Temp,EEPROM_Token);
		}
		break;
	case 0x4E:
		eEEPROMAddrsss = KBHIData;
		Cmd_4E(0x60,eEEPROMAddrsss);
		break;
	case 0x52:
		if(KBHIData==0xA0) Cmd_52_A0(0x60);
		if(KBHIData==0xA3) Cmd_52_A3(0x60);
		if(KBHIData==0xA4) Cmd_52_A4(0x60);
		if(KBHIData==0xA5) Cmd_52_A5(0x60);
		if(KBHIData==0xA6) Cmd_52_A6(0x60); 
		if(KBHIData==0xA7) Cmd_52_A7(0x60); 
		break;
	case 0x53:
		if (KBHIStep == 0x01)
		{
			Cmd_53(0x60,KBHIData2,KBHIData1,KBHIData);
		}
		break;
	case 0x56:
		break;
	case 0x58:
		Cmd_58(KBHIData);
		break;
	case 0x59:
		Cmd_59(0x60,KBHIData,0);
		break;
	case 0x5C:
		Cmd_5C(0x60);
		break;
	case 0x5D:
		Cmd_5D(KBHIData);
		break;
	case 0x71:
        Cmd_71(KBHIData);
		break;
	case 0x72:
		Cmd_72(KBHIData);//add flag for tool to keep the battery at 60%
		break;			
	case 0x7A:
		Cmd_7A(0x60,KBHIData);
		break;
	case 0x7B:
		if( KBHIStep == 0x01 )
		{
			Cmd_7B(0x60,KBHIData1,KBHIData);;
		}
		break;			
	//Add 7Eh Command to access all EC RAM.
	case 0x7E:
		if( KBHIStep == 0x01 )
		{
			Cmd_7E(0x60,KBHIData1,KBHIData);
		}
		break;
	case 0x97:
		Cmd_97(KBHIData);
		break;
	case 0xB0:
		Cmd_B0(0x60,KBHIData);
		break;
	case 0xBC:
        USB_ON_INPUT;
        USB_Delay=KBHIData;
        if(USB_Delay==0)
        {
            USB_Delay=0x0A;
        }
		break;
	case 0xB3:
		if (KBHIStep == 0x01)
		{
			Cmd_B3(0x60,KBHIData1,KBHIData);
		}
		break;
	}
}

//----------------------------------------------------------------------------
// The function of 64 port
//----------------------------------------------------------------------------
void Hook_64Port(BYTE KBHICmd)
{
	switch(KBHICmd)
	{
	case 0x40:
		KBHIStep=0x01;
		break;
	case 0x41:
		KBHIStep=0x01;
		break;
	case 0x42:
		KBHIStep=0x01;
		break;
	case 0x43:
		KBHIStep=0x01;
		break;
	case 0x44:
		Cmd_44(0x60);
		break;
	case 0x45:
		KBHIStep=0x01;
		break;
	case 0x46:
		KBHIStep=0x01;
		break;
	case 0x47:
		KBHIStep=0x01;
		break;
	case 0x49:
		Cmd_49(0x60);//HEGANGS021:one key wake
		break;
	case 0x4B:
		KBHIStep=0x04;
		break;
	case 0x4C:
		KBHIStep=0x03;
		break;
	case 0x4D:
		KBHIStep=0x02;
		break;
	case 0x4E:
		KBHIStep=0x01;
		break;
	case 0x4F:
		Erase_EEPROMAll();
		break;
	//Add CMD 0X50 for osAging S4 keep EC power.
	case 0x50:
		SET_MASK(ACPI_HOTKEY, b6Cmd_NoShut);//for osAging s4 keep EC power
		break;
	case 0x51:
		Cmd_51(0x60);
		break;
	case 0x52:
		KBHIStep=0x01;
		break;
	case 0x53:
		KBHIStep=0x02;
		break;
	case 0x56:
		break;
	case 0x58:
		KBHIStep=0x01;
		break;
	case 0x59:
		KBHIStep=0x01;
		break;
	case 0x5C:
		break;
	case 0x5D:
		KBHIStep=0x01;
		break;
	case 0x5E:			// UNLOCK SHIP MODE 
		#if shipmodesupport		//add ship mode judge
		Unlock_ShipMode();
		#endif
		break;
	case 0x5F:	
		#if shipmodesupport		//add ship mode judge//	LOCK SHIP MODE 
		ShipModeEn=0xA5;  
		Data2PortDirect(0x60, 0x5A);
        RamDebug(0xA5);
		#endif
		break;
	case 0x71:
		KBHIStep=0x02;//Add 60/64 71cmd follow new EC common SPEC.
		break;
		case 0x72:
		KBHIStep=0x01;//add flag for tool to keep the battery at 60%
		break;
	case 0x75:
		Cmd_75(); 
		break;
	case 0x7A:
		KBHIStep=0x01;
		break;
	case 0x7B:
		KBHIStep=0x02;
		break;
	//Add 7Eh Command to access all EC RAM.
	case 0x7E:
		KBHIStep=0x02;
		break;
	//Add workaround for hang bios.
	case 0x7C:
		Bioswatchdog=1;
		Bioswatchdogtime=0;
		Data2PortDirect(0x60, 0x55);//add return date for bios.
		break;
	case 0x7D:
		Bioswatchdog=0;
		Data2PortDirect(0x60, 0x55);//add return date for bios.
		break;
	//Add workaround for hang bios.
	case 0x97:
		KBHIStep=0x01;
		break;
	case 0xB0:
		KBHIStep=0x01;
		break;
	//start for save P80 code to setup. 
	case 0xB1:
		Cmd_B1(0x60,0xB1);
		break;
	case 0xB2:
		Cmd_B2(0x60,0xB2);
		break;
	//end for save P80 code to setup. 
	case 0xB3:
		KBHIStep=0x02;
		break;
	case 0xB4:
		Cmd_B4();
		break;
	case 0xBC:
		KBHIStep=0x01;
		break;
	case 0x77:
        DisableAllInterrupt();
		Erase_Eflash_1K(0x01,0x20,0x00);
        EnableAllInterrupt();
		break;
	case 0x88:

		break;

	default:
		break;
	}
}

//-----------------------------------------------------------------------
// kbcmd : keyboard command from 0x60 port
//-----------------------------------------------------------------------
void Hook_Keyboard_Cmd(BYTE kbcmd)
{

}

//-----------------------------------------------------------------------
// kbcmdp : keyboard command parameter from 0x60 port
// for example keyboard command (0xED, 0xF0, 0xF3)
//-----------------------------------------------------------------------
void Hook_Keyboard_Cmd_Parameter(BYTE kbcmdp)
{

}

//-----------------------------------------------------------------------
// mscmd : mosue command from 0x64 port 0xD4 command
//-----------------------------------------------------------------------
void Hook_Mouse_D4Cmd(BYTE mscmd)
{

}

//-----------------------------------------------------------------------
// mscmd : mosue command from 0x64 port 0x90 command
//-----------------------------------------------------------------------
void Hook_Mouse_90Cmd(BYTE mscmd)
{

}

//-----------------------------------------------------------------------
// mscmd : mosue command from 0x64 port 0x91 command
//-----------------------------------------------------------------------
void Hook_Mouse_91Cmd(BYTE mscmd)
{

}

//-----------------------------------------------------------------------
// mscmd : mosue command from 0x64 port 0x92 command
//-----------------------------------------------------------------------
void Hook_Mouse_92Cmd(BYTE mscmd)
{

}

//-----------------------------------------------------------------------
// mscmd : mosue command from 0x64 port 0x93 command
//-----------------------------------------------------------------------
void Hook_Mouse_93Cmd(BYTE mscmd)
{

}

void Cmd_40(BYTE sCount)
{
	AutoTimer = sCount;
    RamDebug(0x40); 
    RamDebug(AutoTimer); 
	SET_MASK(ACPI_HOTKEY, b6Cmd_NoShut);
}

void Cmd_41_A0(BYTE nPort) 
{
	Data2PortDirect(nPort, REV0_BYTE0);   //Data2Port to Data2PortDirect
}

void Cmd_41_A1(BYTE nPort) 
{
	Data2Port(nPort, 0x43);     // 'C' 0x43
	MultiB2Port(nPort, 0x4F);  // 'O' 0x4F
	MultiB2Port(nPort, 0x4D);  // 'M' 0x4D
	MultiB2Port(nPort, 0x50);  // 'P' 0x50
	MultiB2Port(nPort, 0x41);  // 'A' 0x41
	Data2PortDirect(nPort, 0x4C);  // 'L' 0x4C  /MultiB2Port to Data2PortDirect
}

void Cmd_41_A2(BYTE nPort) 
{	//Return EEPROM slave address
	Data2PortDirect(nPort, 0);    //Data2Port to Data2PortDirect
}

void Cmd_41_A3(BYTE nPort) 
{	//Return Thermal chip slave address
	Data2PortDirect(nPort, 0);    //Data2Port to Data2PortDirect
}

void Cmd_42(BYTE eFlashBank)
{
	eEEPROMBank = eFlashBank;
}

void Cmd_43(BYTE nPort,BYTE nData)
{
	Data2PortDirect(nPort,( *((XBYTE *)(OEMRAM4|nData)) ));    //Data2Port to Data2PortDirect //Modify 43 cmd return EC space value(0x400) follow EC spec.
}

void Cmd_44(BYTE nPort) 
{
	Data2Port(nPort, ProjectID0L);
	Data2PortDirect(nPort, ProjectID0H); //MultiB2Port to Data2PortDirect
}

void Cmd_45(BYTE nPort, BYTE sCount)
{
	switch( sCount )
	{

	case 0x20:	//Enable power switch WDT function
		  CLEAR_MASK(pProject4,pPWSWdisable); //HEGANGS028:Disable POWERSW when flash bios
		  break;	  
	case 0x21:	//Disable power switch WDT function
		  SET_MASK(pProject4,pPWSWdisable);  
		  break;

	case 0x80:	// Disable ME Flash.
		pchME_FLAS_OUT;
		pchME_FLAS_HI();
		break;
	case 0x81:	// Enable ME Flash.
		pchME_FLAS_LOW();
		pchME_FLAS_INDW;
		break;

	case 0x82:
		SET_MASK(SYS_STATUS, b6BIOS_Flash);	// For BIOS use flash.
		break;

	case 0x83:
		SET_MASK(SysStatus,LidKBIgnore);
        Ccb42_DISAB_KEY = 1;
		Flag.SCAN_INH = 1;				// Inhibit scanner (internal keyboard).
		Lock_Scan();					// Lock scanner
		ECSend2Port(2, 0xF5);			// Disable TouchPad.
		break;

	#if Support_AOU5_V1_2//modify USB charge setting.
	case 0x8E:	// USB Enable Charger.
		if (IS_MASK_CLEAR(EMStatusBit, b1SetUSBChgEn))	// if USB charger disable, then enable it..
		{
			SET_MASK(EMStatusBit, b1SetUSBChgEn);// USB enable charger.
			eEEPROMBank= 0x07;
			eEEPROMAddrsss = 0xE4;
			eEEPROMData = EMStatusBit;
			SET_MASK(LENOVOPMFW_Temp,EEPROM_Token);
			Update_EEPROMMark();
			CLEAR_MASK(LENOVOPMFW_Temp,EEPROM_Token);
		}
		break;

	case 0x8F:	// USB Disable charger.
		if (IS_MASK_SET(EMStatusBit, b1SetUSBChgEn))		// if USB charger enable, then disable it..
		{
			CLR_MASK(EMStatusBit, b1SetUSBChgEn);// USB enable charger.
			eEEPROMBank= 0x07;
			eEEPROMAddrsss = 0xE4;
			eEEPROMData = EMStatusBit;
			SET_MASK(LENOVOPMFW_Temp,EEPROM_Token);
			Update_EEPROMMark();
			CLEAR_MASK(LENOVOPMFW_Temp,EEPROM_Token);
		}
		break;
	#endif                 
	case 0x90:
		CURRENT_STATUS &= 0x3F;
		CURRENT_STATUS |= 0x40;				// For BIOS set Legacy HDD.
		Chk_UEFIStatus();					// Check UEFI status.
		break;
	case 0x91:
		CURRENT_STATUS &= 0x3F;
		CURRENT_STATUS |= 0x80;				// For BIOS set UEFI HDD.
		Chk_UEFIStatus();					// Check UEFI status.
		break;

	case 0xA2:
		SET_MASK(DEVICEMODULE,WWAN_3G_EXIST);	// 3G module exist
		break;

	case 0xA3:
		CLR_MASK(DEVICEMODULE,WWAN_3G_EXIST);	// 3G module not exist
		break;

	case 0xA4:									// Return WLAN & Bluetooth status
		Data2PortDirect(nPort, DEVICEMODULE);   //
		break;

	case 0xA5:
		CLR_MASK(DEVICEMODULE,WIRELESS_EXIST);	// Wirless LAN not exist
		CLR_MASK(DEVICEMODULE,BLUETOOTH_EXIST);	// BlueTooth not exist
		break;

	case 0xA6:
		SET_MASK(DEVICEMODULE,WIRELESS_EXIST);	// Wirless LAN exist
		CLR_MASK(DEVICEMODULE,BLUETOOTH_EXIST);	// BlueTooth not exist
		break;

	case 0xA7:
		CLR_MASK(DEVICEMODULE,WIRELESS_EXIST);	// Wirless LAN not exist
		SET_MASK(DEVICEMODULE,BLUETOOTH_EXIST);	// BlueTooth exist
		break;

	case 0xA8:
		SET_MASK(DEVICEMODULE,WIRELESS_EXIST);	// Wirless LAN exist
		SET_MASK(DEVICEMODULE,BLUETOOTH_EXIST);	// BlueTooth exist
        break;

	case 0xA9:
		SET_MASK(LENOVODEVICE,Camera_EXIST);// Camera exist
		break;

	case 0xAA:
		CLR_MASK(LENOVODEVICE,Camera_EXIST);// Camera not exist
		break;

	case 0xAB:								// Return MBID
		Data2PortDirect(nPort, uMBID);   
		break;

	case 0xAE:
		Data2PortDirect(nPort, uNovoVPCCount);  
		uNovoVPCCount=0;		//optimize clear NOVO button states function.
		break;
		//Add voice wake up cmd for BIOS.
	case 0xAF:
		Data2PortDirect(nPort, uWOVSTATUS); 
		uWOVSTATUS=0;	
		break;
	case 0xB0:								// CPU is TJ85
		cCPUKind = 0;
		break;
	case 0xB1:								// CPU is TJ90
		cCPUKind = 1;
		break;
	case 0xB2:								// CPU is TJ100
		cCPUKind = 2;
		break;
	case 0xBD:								// CPU is TJ105
		cCPUKind = 3;
		break;
	case 0xBE:								// CPU is Dual core TJ100
		cCPUKind = 4;
		break;
	case 0xBF:								// CPU is Quad core TJ100
		cCPUKind = 5;
		break;

	case 0xCF:
		if ( IS_MASK_SET(CombineKeyStatus, b0FnRN) )
		{ Data2PortDirect(nPort, 1); }   //Data2Port to Data2PortDirect
		else
		{ Data2PortDirect(nPort, 0); }    // Data2Port to Data2PortDirect
		break;

	case 0xD4:      						// Return device
		Data2PortDirect(nPort, pDevStus);  //Data2Port to Data2PortDirect
		break;

	case 0xE7:
		SET_MASK(pProject0,b4VGAType);		//UMA
		//add UMA cmd define.
		uMBGPU = 0x01;
       	if (IS_MASK_SET(SYS_STATUS,AC_ADP))  //read adapter ID after know UMA or dis.
      	{
   		 	AdapterIDOn_Flag = 0x01; 
 		 	ADPIDON10MS_NUM=0x0a;  
      	} 
      	//add UMA cmd define.

		InitChargerIC(); 
		
		break;

	case 0xE9:
		uReserve07.fbit.uACInOutBeep = 0;	// Enable AC in/out beep
		break;

	case 0xEA:
		uReserve07.fbit.uACInOutBeep = 1;	// Disable AC in/out beep
		break;
	//Distinguish between 310 or 510.
	case 0xEB:
		SET_MASK(pProject0,b7GSKL310OR510);	// 310/510 BIT7=1 310
		break;

	//add AMD or NV discrete cmd define.
	case 0xF0: //NV
		CLR_MASK(pProject0,b4VGAType);		//Discreate
		uMBGPU = 0x02;
		uPJID = 0x02;
       	if (IS_MASK_SET(SYS_STATUS,AC_ADP))  //read adapter ID after know UMA or dis.
      	{
   		 	AdapterIDOn_Flag = 0x01; 
 		 	ADPIDON10MS_NUM=0x0a;  
      	} 

		InitChargerIC();
		
		break;
	case 0xF1: //AMD
		CLR_MASK(pProject0,b4VGAType);		//Discreate
		uMBGPU = 0x02;
		uPJID = 0x01;
       	if (IS_MASK_SET(SYS_STATUS,AC_ADP))  //read adapter ID after know UMA or dis.
      	{
   		 	AdapterIDOn_Flag = 0x01; 
 		 	ADPIDON10MS_NUM=0x0a;  
      	} 

		InitChargerIC(); 
		
		break;
	//add AMD or NV discrete cmd define.
	}
}

void Cmd_46(BYTE nPort,BYTE nData)
{
	BYTE BRPM_Manual;
	switch( nData )
	{
	case 0x81:
		Data2PortDirect(nPort, nAtmFanSpeed);		//Reading FAN speed from FAN1
		break;
	case 0x82:
		Data2PortDirect(nPort, nAtmFan1Speed);			//Reading FAN speed from FAN2
		break;
	case 0x84:
		uReserve07.fbit.nFanManual = 0;		// return EC control.
		break;
	case 0x85:            //HEGANGS049:Add the 530s Highper formance and yoga30 quite mode function
		HighPerf = 1;		// Set high performance fantable
		break;
	case 0x86:
		HighPerf = 0;		// Set low performance fantable
		break;
	case 0x87:
		QuiteMode = 1;		// Set quite mode fantable
		break;
	case 0x88:
		QuiteMode = 0;		// disable quite mode fantable
		break;
	default:
		uReserve07.fbit.nFanManual = 1;
		if ( nData >= 160 )
		{
			nData -= 160;
			ManualFan2PRM = nData;
			FAN1_PWM_ALT;//72JERRY026: Modify double fan setting for MFG tool.
			FAN1_PWM = ManualFanPRM + 2;//72JERRY026: Modify double fan setting for MFG tool.
			FAN1_SPEED_ALT;//72JERRY026: Modify double fan setting for MFG tool.
		}
		else
		{
			ManualFanPRM = nData;
			FAN_PWM_ALT;
			FAN_PWM = ManualFanPRM + 2;
			FAN_SPEED_ALT;
			ManualFan2PRM = nData;//72JERRY026: Modify double fan setting for MFG tool.
			FAN1_PWM_ALT;//72JERRY026: Modify double fan setting for MFG tool.
			FAN1_PWM = ManualFanPRM + 2;//72JERRY026: Modify double fan setting for MFG tool.
			FAN1_SPEED_ALT;//72JERRY026: Modify double fan setting for MFG tool.
		}

		/*if ( uReserve07.fbit.b7Fan2Manual == 0 )
		{ BRPM_Manual = ManualFanPRM; }		// Common FAN1 PRM value.
		else
	 	{ BRPM_Manual = ManualFan2PRM; }	*/// Respective FAN PRM value.//72JERRY026:- Modify double fan setting for MFG tool.
		break;
	}
}


void Cmd_47(BYTE nPort, BYTE nData)
{
	if (nData != 0x80)
    {
		cOsLedCtrl.fbit.cOL_CtrlRight = 1;
    }
	switch( nData )
	{
	case 0x80:			// Return back the control right to EC
		cOsLedCtrl.word = 0x00;
        cOsLed1Ctrl.word = 0x00;
		OEM_Write_Leds(Led_Data);   //Hook Oem On-board LED control
		break;
	case 0x81:			// System LED on and control by OS
		cOsLedCtrl.fbit.cOL_SysLed = 1;
		break;
	case 0x82:			// Power management LED on and control by OS
		cOsLedCtrl.fbit.cOL_PwrLed = 1;
		break;
	case 0x83:			// Charge LED on and control by OS
		cOsLedCtrl.fbit.cOL_ChgLed = 1;
		break;
	case 0x84:			// Discharge LED on and control by OS
		cOsLedCtrl.fbit.cOL_DisChgLed = 1;
		break;
	case 0x85:			// T/P active LED on and control by OS
		cOsLedCtrl.fbit.cOL_TPActLed = 1;
		break;
	case 0x86:			// Blue Tooth LED on and control by OS
		cOsLedCtrl.fbit.cOL_BTLed = 1;
		break;
	case 0x87:			// T/P lock LED on and control by OS
		cOsLedCtrl.fbit.cOL_TPLockLed = 1;
		break;
	case 0x88:			// All LED off and control by OS
		cOsLedCtrl.word=0;
		cOsLedCtrl.fbit.cOL_CtrlRight = 1; //G63:Modify LED control for MFG tool.
		//cOsLedCtrl.word = 0x0080;	// Set cOL_CtrlRight == 1.
        cOsLed1Ctrl.word = 0x00;
		break;
	case 0x89:			// HDD LED on and control by OS
		cOsLedCtrl.fbit.cOL_HddLed = 1;
		break;
	case 0x8A:			// FDD LED on and control by OS
		cOsLedCtrl.fbit.cOL_FddLed = 1;
		break;
	case 0x8B:			// Media LED on and control by OS
		cOsLedCtrl.fbit.cOL_MediaLed = 1;
		break;
	case 0x8C:			// TV LED on and control by OS
		cOsLedCtrl.fbit.cOL_TVLed = 1;
		break;
	case 0x8D:			// Email LED on and control by OS
		cOsLedCtrl.fbit.cOL_MailLed = 1;
		break;
	case 0x8E:			// Wireless LED on and control by OS
		cOsLedCtrl.fbit.cOL_WLANLed = 1;
		break;
	case 0x8F:			// Numlock LED Controlled by OS
		cOsLedCtrl.fbit.cOL_NumlockLed = 1;
		break;
	case 0x90:			// Padslock(Cursorlock) LED Control by OS
		cOsLedCtrl.fbit.cOL_PadslockLed = 1;
		break;
	case 0x91:			// Capslock LED Control by OS
		cOsLed1Ctrl.fbit.cOL1_CapslockLed = 1;
		break;
	case 0x92:			// Scrolllock LED Control by OS
		cOsLed1Ctrl.fbit.cOL1_ScrolllockLed = 1;
		break;
	case 0x93:			// Special LED1 (Special LED for testing in production line)
		cOsLed1Ctrl.fbit.cOL1_SpecialLed1 = 1;
		break;
	case 0x94:			// KB backlight LED control by OS.
		cOsLed1Ctrl.fbit.cOL1_KBBacklight = 1;
		break;
	case 0x95:			// Special LED3 (Special LED for testing in production line)
		cOsLed1Ctrl.fbit.cOL1_SpecialLed3 = 1;
		break;
	case 0x96:			// Special LED3 (Special LED for testing in production line)
		cOsLed1Ctrl.fbit.cOL1_PwrAmber = 1;
		break;
	}
}
void Cmd_49(BYTE nPort)
{
		Data2PortDirect(nPort, ONEKEY_FLAG);  
		ONEKEY_TEMP_FLAG=ONEKEY_FLAG;		//HEGANGS021:one key wake
		ONEKEY_FLAG=0;
}
//Add 8 sec reset test function.
void Cmd_4A(BYTE nPort, BYTE nData)
{
	if(nData == 0x01)//Enable
	{
		ResetTestFlag = 0x01;
		BRAMBK0[127] = GPCRI5;
	}
	else if(nData == 0x02)
	{
		ResetTestFlag = 0x02;
	}
	BRAMBK0[126] = ResetTestFlag;

	WinFlashMark=0x53;//B05:Modify after flash EC can auto poweron follow bios.
    WinFlashMark2=0x35;//B05:Modify after flash EC can auto poweron follow bios.
 	InternalWDTNow(RESETEST_ID);
}


void Cmd_4E(BYTE nPort, BYTE nOffSet)
{
	BYTE i,j,k;
	if (eEEPROMBank == 07 && (nOffSet > 0xEF) && (nOffSet!=0xF1))
	{
		RamDebug(0x4E);
		RamDebug(eEEPROMBank);
		RamDebug(nOffSet);
		nOffSet = nOffSet & 0x0F;
		for (i=0; i<4 ;i++)
		{
			for (j=0; j< 0x10 ;j++)
			{
				k = Read_Eflash_Byte(EEPROMA2,(EEPROMA1_16Byte + i),(j*16));

				if(k == 0xFF)
				{
					break;
				}
				else
				{
					eEEPROMData = Read_Eflash_Byte(EEPROMA2,(EEPROMA1_16Byte + i),((j*16) | nOffSet));
				}
			}
			if (k == 0xFF)
				break;
		}
	}
	else
	{
		eEEPROMData=Read_Eflash_Byte(EEPROMA2,(eEEPROMBank|EEPROMA1_B03),nOffSet);
	}
	Data2PortDirect(nPort, eEEPROMData);  
}


void Cmd_51(BYTE nPort) {
	Data2Port(nPort, REV0_BYTE0);
	MultiB2Port(nPort, REV0_BYTE1);
 
	Data2PortDirect(nPort, REV0_BYTE3);  
}

void Cmd_52_A0(BYTE nPort) { 
	Data2Port(nPort, 'D');		// 'V' 0x51
	MultiB2Port(nPort, 'S');	// 'I' 0x49
	MultiB2Port(nPort, '3');	// 'Q' 0x51
	MultiB2Port(nPort, '1');	// 'Y' 0x59
	Data2PortDirect(nPort, '2');
}

void Cmd_52_A3(BYTE nPort) {
	Data2PortDirect(nPort, (0x10+(CHIP_TYPE0&0x0F))); 
}

void Cmd_52_A4(BYTE nPort)
{
	if( IS_MASK_SET(SYS_MISC1,Crisis_On) )
	{
		Data2PortDirect(nPort,0x80); //Data2Port to Data2PortDirect
		SET_MASK(SYS_MISC1,b5Crisis_LED);
        CLR_MASK(SYS_MISC1,Crisis_On);
		CLR_MASK(SYS_MISC1,BATCRISIS_FLAG); 
	}
	else
	{
		Data2PortDirect(nPort,0x00);   // Data2Port to Data2PortDirect
		CLR_MASK(SYS_MISC1,b5Crisis_LED);
	}
}

void Cmd_52_A5(BYTE nPort) 
{
	Data2PortDirect(nPort, REV0_BYTE4); //Data2Port to Data2PortDirect
}

//S+ add command for BIOS show shutdown ID.
void Cmd_52_A6(BYTE nPort)
{
	Data2PortDirect(nPort, EEPROMA2);
}

void Cmd_52_A7(BYTE nPort)
{
	Data2PortDirect(nPort, EEPROMA1_16Byte);
}


void Cmd_53(BYTE nPort,BYTE nDataH,BYTE nDataM,BYTE nDataL)
{
	eEEPROMData=Read_Eflash_Byte(nDataH,nDataM,nDataL);
	Data2PortDirect(nPort, eEEPROMData);   //Data2Port to Data2PortDirect
}

//Hang check
void Cmd_55(BYTE nData)
{
    if(0xb7==nData)
    {
		PWR_LED_Debug = 1;
    }
	else if(0xb8==nData)
	{
		PWR_LED_Debug = 0;
	}
	else if(0xb9==nData) //HEGANG0027:Add bios hang check
	{
		bios_check = 1;
	}
}
//Hang check

void Cmd_58(BYTE sCount)
{
	cPanelId = sCount;
}

void Cmd_59(BYTE nPort, BYTE nData, BYTE nData2)
{
	switch( nData )
	{
	case 0x60:	// Keep PCH power for RTC
		SET_MASK(cCmd, bPCHPWR_Keep);
		break;
	case 0x64:	// Enable scancode for test button
		SET_MASK(cCmd, b6TestBtnEn);
		DEBOUNCE_CONT1 = T_PSW_DEBOUNCE; //add solution for P/L test Power Button
		break;

	case 0x65:	// Disable scancode for test button
		CLR_MASK(cCmd, b6TestBtnEn);
		DEBOUNCE_CONT1 = T_PSW_DEBOUNCE_S0;	//add solution for P/L test Power Button
		break;

	case 0x6E:
		uCritBattWakeThre = nData;
		SET_MASK(uCritBattWakeThre,IFFS_Enable);
		uIffsCnt = 40;
		break;

    case 0x76:
		uReserve07.fbit.nFanManual = 0;			// return EC control.
		uReserve07.fbit.b7Fan2Manual = 0;		// return EC control.
		CLR_MASK(Thro_Status2, b4FAN1_FullOn);	// Disable Full on.
		break;
    case 0x77:
		uReserve07.fbit.nFanManual = 0;			// return EC control.
		uReserve07.fbit.b7Fan2Manual = 0;		// return EC control.
		SET_MASK(Thro_Status2, b4FAN1_FullOn);	// Enable Full on.
		break;

	case 0x7A:
		SET_MASK(pDevStus, pENABLE_TP);		// Enable TouchPad.
		EnablePS2Port_2();
		break;
	case 0x7B:
		CLEAR_MASK(pDevStus, pENABLE_TP);	// Disable TouchPad.
		DisablePS2Port_2();
		break;

    case 0x7C: // Enable Lid Keyboard function
       	CLEAR_MASK(SysStatus,LidKBIgnore);
        Ccb42_DISAB_KEY = 0;
		Flag.SCAN_INH = 0;				// Not Inhibit scanner (internal keyboard).
		Unlock_Scan();					// UnLock scanner
        break;

   	case 0x7D: // Disable Lid Keyboard function
       	SET_MASK(SysStatus,LidKBIgnore);
        Ccb42_DISAB_KEY = 1;
		Flag.SCAN_INH = 1;				// Inhibit scanner (internal keyboard).
		Lock_Scan();					// Lock scanner
        break;

	case 0x80:	// set battery charge to 100% then discharge to xx% and keep
		cBattFlag0.fbit.cBF0_FullToTarget = 1;
		cBattFlag0.fbit.cBF0_Full = 0;
		cBattFlag0.fbit.cBF0_GoTarget = 0;
		GetData2(nPort);
		cTargetGauge = CmdData2;
		if ( cTargetGauge > 100 )
			cTargetGauge = 0;
		break;

	case 0x81:	//charge/discharge to xx% and keep
		cBattFlag0.fbit.cBF0_FullToTarget = 0;
		cBattFlag0.fbit.cBF0_Full = 0;
		cBattFlag0.fbit.cBF0_GoTarget = 1;
		GetData2(nPort);
		cTargetGauge = CmdData2;
		if ( cTargetGauge > 100 )
			cTargetGauge = 0;
		break;
	case 0x82:	//disable battery charge xx% feature
		cBattFlag0.fbit.cBF0_FullToTarget = 0;
		cBattFlag0.fbit.cBF0_Full = 0;
		cBattFlag0.fbit.cBF0_GoTarget = 0;
        //Modify charge/discharge to xx% and keep for MFG.
        CLEAR_MASK(ACOFF_SOURCE, BATTLEARN);
		BATT_LEN_OFF(); 
        //Modify charge/discharge to xx% and keep for MFG.
		break;

	case 0x94:
		SET_MASK(cCmd, b5VolMute);
		EC_MUTE_ACT();					// Mute
		break;
	case 0x95:
		CLR_MASK(cCmd, b5VolMute);
		EC_MUTE_INACT();				// Unmute
		break;

	case 0x9A:							// Disable AC power source
		cBattFlag0.fbit.cCmdAcOff = 1;
		SET_MASK(ACOFF_SOURCE, ACMD);
		CLEAR_MASK(SYS_STATUS,AC_ADP);	//Clear AC in  flag
		//ACOFF_HI();
		BATT_LEN_ON();
		ECSMI_SCIEvent(ACPI_ACOUT_SCI);
		break;
	case 0x9B:							// Enable AC power source
		cBattFlag0.fbit.cCmdAcOff = 0;
		CLEAR_MASK(ACOFF_SOURCE, ACMD);
		SET_MASK(SYS_STATUS,AC_ADP);	//Set AC in  flag
		BATT_LEN_OFF();
		ECSMI_SCIEvent(ACPI_ACIN_SCI);
		break;
//72JERRY065:S+Add CMD 0x59 test voice wake up for MFG.
	case 0x9C:							// Enable voice wake up MFG test
		cDev.fbit.cD_WovMFGtest = 1;
		Data2PortDirect(nPort, 0x55);//72JERRY068:Optimize 72JERRY065 add the return value.
		
		break;

	case 0x9D:							// Disable voice wake up MFG test
		cDev.fbit.cD_WovMFGtest = 0;
		Data2PortDirect(nPort, 0x55);//72JERRY068:Optimize 72JERRY065 add the return value.
		//72JERRY065:E+Add CMD 0x59 test voice wake up for MFG.
		break;

	case 0x9E:							//Disable Internal keyboard.
		Flag.SCAN_INH = 1;				// Inhibit scanner (internal keyboard).
		Lock_Scan();					// Lock scanner
		break;

	case 0xA2:							// System shutdown
		SET_MASK(SysStatus,ERR_ShuntDownFlag);
		ProcessSID(COMMAND_ID); 		// Shutdown ID 0x04.
		PWSeqStep = 1;
		PowSeqDelay = 0x00;
        RamDebug(0x0A);
		SysPowState=SYSTEM_S0_S5;
		break;

	case 0xA3:							// System enter beep mode for LB state in CMOS setup
		cBattInform.fbit.cBI_LbBeep = 1;
		break;
	case 0xA4:							// System enter quiet mode for LB state in CMOS setup
		cBattInform.fbit.cBI_LbBeep = 0;
		break;

	case 0xB9:							// PME enable
		uReserve07.fbit.nPmeWakeEN = 1;
		break;
	case 0xBA:							// PME disable
		uReserve07.fbit.nPmeWakeDIS =0;
		break;

	case 0xC1:	// Force battery in learning state with AC
		cBattFlag0.fbit.cBF0_BLMode = 1;
		break;
	case 0xC2:	// Disable battery in learning state with AC
		cBattFlag0.fbit.cBF0_BLMode = 0;
		break;

	case 0xD2:	// Beep alarm 200ms
		PWM_BEEP = 0x7F;			// Out beep
		PWM_LEDBeep_CNT=0x04;		// 20*(1/100 sec)=200ms
		break;

	case 0xD7:	// Battery stop charge enable
		SET_MASK(nStopChgStat3L,ENCUSTOMER);
		break;

	case 0xD8:	// Battery stop charge disable
		CLEAR_MASK(nStopChgStat3L,ENCUSTOMER);
		break;

	case 0xD9: 	// Set System Flag
		SET_MASK(KBHISR,SYSF);
		break;

	case 0xDA: 	// Clear system Flag
		CLEAR_MASK(KBHISR,SYSF);
		break;

	case 0xE1:
		CLR_MASK(cCmd, b3BkOff);		// Turn on LCD backlight;
		CLR_MASK(OEMControl, b1BkOff);  
		//EC_TS_ON_HI(); 
		break;

	case 0xE2:
		SET_MASK(cCmd, b3BkOff);		// Turn off LCD backlight
		SET_MASK(OEMControl, b1BkOff);  
		//EC_TS_ON_LOW(); 
		break;

	case 0xE4: 	// Selection BR matrix
		cKBStatus |= 0x03;	// Set BR keyboard.
		LoadKBMatrixToRAM();
		break;
	case 0xE5: 	// Selection US matrix
		cKBStatus &= 0xFC;	// Set US keyboard.
		LoadKBMatrixToRAM();
		break;
	case 0xE6: 	// Selection JP matrix
		cKBStatus &= 0xFC;
		cKBStatus |= 0x02;	// Set JP keyboard.
		LoadKBMatrixToRAM();
		break;
	case 0xE7: 	// Selection UK matrix
		cKBStatus &= 0xFC;
		cKBStatus |= 0x01;	// Set UK keyboard.
		LoadKBMatrixToRAM();
		break;

	case 0xE8: 							// enable ACPI mode for Embedded control
		SET_MASK(SYS_MISC1,ACPI_OS);
		Bioswatchdog=0;
		Bioswatchdogtime=0;
		break;
	case 0xE9: 							// disable ACPI mode for Embedded control
		CLR_MASK(SYS_MISC1,ACPI_OS);
		break;

	case 0xF6:
	case 0xF7:
	case 0xF8:
	case 0xF9:
		if (IS_MASK_SET(SYS_MISC1,ACPI_OS))
		{
			cSysActionFlag = SysAction_SysShutdown_EC_AutoBoot;
		}
		else
		{
			if( nData == 0xF6 )			// ACTION
				cSysActionFlag = SysAction_EC_Restart;
			else if( nData == 0xF7 )	// Fixed system action to 3 - Restart system
				cSysActionFlag = SysAction_EC_Restart;
			else if( nData == 0xF8 )	// Fixed system action to 2 - Shutdown system
				cSysActionFlag = SysAction_EC_Shutdown;
			else if( nData == 0xF9 )	// Fixed system action to 1 - Nothing
				cSysActionFlag = SysAction_Nothing;
		}

		break;
	}
}

void Cmd_5C(BYTE nPort)
{
	//Get brightness level
    Data2PortDirect(nPort, nBrightValue);  
}

void Cmd_5D(BYTE sCount)
{
	if ( sCount != nBrightValue )	// unequal value.
	{
		if (( (SYS_STATUS & 0x07) >= 0x04 )&&( (SYS_STATUS & 0x07) <= 0x06))	// Win8, win8.1 or win10 ANGELAS010: add win10 mode
		{
			if( sCount >= WIN8_BRI_MAX_STEP )
			{ 
				nBrightValue = WIN8_BRI_MAX_STEP - 1;
            }	// Set max. level.
			else
			{ 
			    nBrightValue = sCount; 
            }
		}
		else
		{
			if( sCount >= BRIGHT_MAX_STEP )
			{ 
			    nBrightValue = BRIGHT_MAX_STEP - 1; 
            }		// Set max. level.
			else
			{ 
			    nBrightValue = sCount; 
            }
		}
	}
}
void Cmd_71(BYTE nData) //modify cmd 0x71 have one data.
{

	
	//add new cmd 0x71 define(data is board ID 0x14 or 0x15).
	uMBID=nData;
	LoadKBMatrixToRAM();//Add load keyboard matrix when know board ID

}
//Add flag for test tool to keep the battery RSOC at 60%
void Cmd_72(BYTE nData)
{
	switch( nData )
	{
	case 0x80:
		testtoolflag=1;
		break;
	case 0x81:
		testtoolflag=0;
	   	break;		
	}

}



void Cmd_75(void) 
{
	PCH_ColdBoot_TimeOut = 20;
}



void Cmd_7A(BYTE nPort,BYTE nData)
{
    RamDebug(0xAA);
	RamDebug(nData);
    switch(nData)
    {
	case 0x10:
		SET_MASK(pProject0,b1uFUdayClr);
	    break;
	case 0x11:
	    if(IS_MASK_SET(pProject0,b3uBatteryTimeNoOK))
	    {
		   	Data2Port(nPort, 0x01);
			RamDebug(0x01);
	    }
		else
		{
			Data2Port(nPort, 0x00);
			RamDebug(0x10);
		}
	    break;
	case 0x12:
	    SET_MASK(pProject0,b5FUBIOSWriteReady);
	    break;		   
	default:
       	break;
	}
}
void Cmd_7B(BYTE nPort,BYTE nDataH,BYTE nDataL)
{
    WORD FUDateTemp;
	FUDateTemp=0x0000; // add initial value is 0
	RamDebug(0xAB);
	RamDebug(nDataH);
	RamDebug(nDataL);
    switch(nDataH)
    {
	case 0x20:
		FUDateTemp = FUDateTemp | ((WORD)nDataL << 0x09);  //bit9~15: 0~127, year-1980 //add nDataL force switch to word type
		batteryFirstUsedDateH = (batteryFirstUsedDateH | (BYTE)(FUDateTemp >> 0x08));  
		batteryFirstUsedDateL = (batteryFirstUsedDateL | (BYTE)(FUDateTemp));
	    break;
	case 0x21:
	    FUDateTemp = FUDateTemp | ((WORD)nDataL << 0x05);  //bit5~8: 0~12, month 
	    batteryFirstUsedDateH = (batteryFirstUsedDateH | (BYTE)(FUDateTemp >> 0x08));  
		batteryFirstUsedDateL = (batteryFirstUsedDateL | (BYTE)(FUDateTemp));
	    break;
	case 0x22:
	    FUDateTemp = FUDateTemp | (WORD)nDataL;  //bit0~4: 0~31, date 
	    batteryFirstUsedDateH = (batteryFirstUsedDateH | (BYTE)(FUDateTemp >> 0x08));  
		batteryFirstUsedDateL = (batteryFirstUsedDateL | (BYTE)(FUDateTemp));
	    break;   
	default:
        break;
	}

	RamDebug(batteryFirstUsedDateH);
	RamDebug(batteryFirstUsedDateL);
}


void Cmd_7E(BYTE nPort,XBYTE nDataH,XBYTE nDataL)
{
   XBYTE temp;
   temp =  *((XBYTE *)((nDataH<<8)|nDataL));
   Data2PortDirect(nPort,temp);
}


void Cmd_97(BYTE nData)
{
	uReserve07.fbit.nTPDriverIn = 1;
	RamDebug(0x97);
	RamDebug(nData);
	switch( nData )
	{
	case 0x00:
		break;
	case 0x01:
		CLR_MASK(pDevStus, pENABLE_TP);
		if ( IS_MASK_SET(StatusKeeper, b5TP_Event) )
		{
			uVPCeventSource = TouchPad;
			uVPCeventSource2 = 0;
			ECSMI_SCIEvent(SDV_VPC_notify);
			CLR_MASK(StatusKeeper, b5TP_Event);
		}
		break;
	case 0x02:
		SET_MASK(pDevStus, pENABLE_TP);
		if ( IS_MASK_SET(StatusKeeper, b5TP_Event) )
		{
			uVPCeventSource = TouchPad;
			uVPCeventSource2 = 0;
			ECSMI_SCIEvent(SDV_VPC_notify);
			CLR_MASK(StatusKeeper, b5TP_Event);
		}
		break;
	}
}

void Cmd_B0(BYTE nPort,BYTE nData)
{
	Data2PortDirect(nPort,( *((XBYTE *)(NameSpace|nData)) )); 
}
//start for save P80 code to setup. 
void Cmd_B1(BYTE nPort,BYTE nData)
{
   switch(nData)
   {
   case 0xB1:
       Data2PortDirect(nPort, P80CMOSSts);  
	   break;
   default:
	   break;
   }

}
void Cmd_B2(BYTE nPort,BYTE nData)
{
  switch(nData)
  {
  case 0xB2:	
      Data2Port(nPort, P80Index);  
	  MultiB2Port(nPort, P80CMOS[0]);
	  MultiB2Port(nPort, P80CMOS[1]);
	  MultiB2Port(nPort, P80CMOS[2]);
	  MultiB2Port(nPort, P80CMOS[3]);
	  MultiB2Port(nPort, P80CMOS[4]);
	  MultiB2Port(nPort, P80CMOS[5]);
	  Data2PortDirect(nPort, P80CMOS[6]);   //MultiB2Port to Data2PortDirect
	  CLR_MASK(P80CMOSSts,P80CMOSDis);
	  CLR_MASK(ACPI_HOTKEY, b6Cmd_NoShut);//When battery mode press FN+D,do not cut power only for debug.
	  break;
  default:
	  break;
  }
	
}
//end for save P80 code to setup. 
void Cmd_B3(BYTE nPort,BYTE nDataH,BYTE nDataL)
{
	cSPIEEPROM.byte= 0x00;		//msmart test

	nDataH = nDataH &0x0F;
	ITempW01 = (WORD)((nDataH<<8)+nDataL);
	RamDebug(0xB3);
	RamDebug(*((XBYTE *) ITempW01));
	Data2PortDirect(nPort,( *((XBYTE *) ITempW01))); 
}

void Update_EEPROM(void)
{

	CacheDma(1,FSCEHighLevel);

	DisableAllInterrupt();
	if (eEEPROMBank <4)
	{
		Erase_Eflash_1K(EEPROMA2,EEPROMA1_M1,0x00);
	}
	if (eEEPROMBank>3 && eEEPROMBank <8)
	{
		Erase_Eflash_1K(EEPROMA2,EEPROMA1_M2,0x00);
	}
	Write_Eflash_1Byte(EEPROMA2,(eEEPROMBank | EEPROMA1_M1),eEEPROMAddrsss,eEEPROMData);

	if (eEEPROMBank <4)
	{
		for(eFlashA1=0x00;eFlashA1<4;eFlashA1++)					// 256 bytes *4
		{
			e256ByteCnt=0;
			do
			{
				eEEPROMData = Read_Eflash_Byte(EEPROMA2,(eFlashA1|EEPROMA1_M1),e256ByteCnt);
				if (eEEPROMData == 0xFF)
				{
					eEEPROMData = Read_Eflash_Byte(EEPROMA2,(eFlashA1|EEPROMA1_D1),e256ByteCnt);
					Write_Eflash_1Byte(EEPROMA2,(eFlashA1|EEPROMA1_M1),e256ByteCnt,eEEPROMData);
				}
				e256ByteCnt++;
			}while(e256ByteCnt!=0x00);								// To check 256 bytes
		}

		Erase_Eflash_1K(EEPROMA2,EEPROMA1_D1,0x00);
		for(eFlashA1=0x00;eFlashA1<4;eFlashA1++)					// 256 bytes *4
		{
			e256ByteCnt=0;
			do
			{
				eEEPROMData = Read_Eflash_Byte(EEPROMA2,(eFlashA1|EEPROMA1_M1),e256ByteCnt);
				Write_Eflash_1Byte(EEPROMA2,(eFlashA1|EEPROMA1_D1),e256ByteCnt,eEEPROMData);
				e256ByteCnt++;
			}while(e256ByteCnt!=0x00);
		}
	}
	if (eEEPROMBank>3 && eEEPROMBank <8)
	{
		for(eFlashA1=0x00;eFlashA1<4;eFlashA1++)					// 256 bytes *4
		{
			e256ByteCnt=0;
			do
			{
				eEEPROMData = Read_Eflash_Byte(EEPROMA2,(eFlashA1|EEPROMA1_D2),e256ByteCnt);
				Write_Eflash_1Byte(EEPROMA2,(eFlashA1|EEPROMA1_M2),e256ByteCnt,eEEPROMData);
				e256ByteCnt++;
			}while(e256ByteCnt!=0x00);								// To check 256 bytes
		}

		Erase_Eflash_1K(EEPROMA2,EEPROMA1_D2,0x00);
		for(eFlashA1=0x00;eFlashA1<4;eFlashA1++)					// 256 bytes *4
		{
			e256ByteCnt=0;
			do
			{
				eEEPROMData = Read_Eflash_Byte(EEPROMA2,(eFlashA1|EEPROMA1_M2),e256ByteCnt);
				Write_Eflash_1Byte(EEPROMA2,(eFlashA1|EEPROMA1_D2),e256ByteCnt,eEEPROMData);
				e256ByteCnt++;
			}while(e256ByteCnt!=0x00);
		}
	}
	EnableAllInterrupt();
}


void Update_EEPROMMark(void)
{
	CacheDma(1,FSCEHighLevel);
	DisableAllInterrupt();

	if (Read_Eflash_Byte(EEPROMA2,(0x03 | EEPROMA1_M1D),0xFF) == 0xFF)
	{
		Erase_Eflash_1K(EEPROMA2,EEPROMA1_M1,0x00);
		Erase_Eflash_1K(EEPROMA2,EEPROMA1_M1D,0x00);

		Erase_Eflash_1K(EEPROMA2,EEPROMA1_M2,0x00);
		Erase_Eflash_1K(EEPROMA2,EEPROMA1_M2D,0x00);

		Write_Eflash_1Byte(EEPROMA2,(0x03 | EEPROMA1_M1D),0xFF,0xAA);	//mark 0x55 0xAA
	}

	Write_Eflash_1Byte(EEPROMA2,(eEEPROMBank | EEPROMA1_M1D),eEEPROMAddrsss,eEEPROMData);
	Write_Eflash_1Byte(EEPROMA2,(eEEPROMBank | EEPROMA1_M1),eEEPROMAddrsss,0x00);
	//Bios request that we need fill EBH = 0xAA to response EEPROM write finished
	if ((eEEPROMBank == 0x00) && (eEEPROMAddrsss != 0xEB))
	{
		Write_Eflash_1Byte(EEPROMA2,(eEEPROMBank | EEPROMA1_M1D),0xEB,0xAA);
		Write_Eflash_1Byte(EEPROMA2,(eEEPROMBank | EEPROMA1_M1),0xEB,0x00);
	}

	eUpdateEEPROMCnt = 1;				
	cSPIEEPROM.fbit.cSPI_ROMCopyStart=1;
	Update_EEPROMB07();				
	EnableAllInterrupt();
}

void Erase_EEPROMAll(void)
{
	CacheDma(1,FSCEHighLevel);

	DisableAllInterrupt();

	Erase_Eflash_1K(EEPROMA2,EEPROMA1_M1,0x00);
	Erase_Eflash_1K(EEPROMA2,EEPROMA1_M1D,0x00);

	Erase_Eflash_1K(EEPROMA2,EEPROMA1_M2,0x00);
	Erase_Eflash_1K(EEPROMA2,EEPROMA1_M2D,0x00);

	Erase_Eflash_1K(EEPROMA2,EEPROMA1_B03,0x00);
	Erase_Eflash_1K(EEPROMA2,EEPROMA1_B47,0x00);
	EnableAllInterrupt();
}

void Update_EEPROMB07(void)
{
	if (!SystemIsS0 || ((WinFlashMark == 0xFC)&&(WinFlashMark1==0x55))) 
		return; //Add WinFlashMark1 check

	if (eUpdateEEPROMCnt ==0 ) 
        return;
	else
		eUpdateEEPROMCnt--;

	if (eUpdateEEPROMCnt !=0 ) 
        return;

    CacheDma(1,FSCEHighLevel);

	DisableAllInterrupt();
#if !EN_PwrSeqTest
	#if WDT_Support
	    ResetInternalWDT();
	#endif
#endif

	for(eFlashA1=0x00;eFlashA1<4;eFlashA1++)	// 256 bytes *4
	{
		e256ByteCnt=0;
		do
		{
			eEEPROMData = Read_Eflash_Byte(EEPROMA2,(eFlashA1|EEPROMA1_M1),e256ByteCnt);
			if (eEEPROMData != 0x00)
			{
				eEEPROMData = Read_Eflash_Byte(EEPROMA2,(eFlashA1|EEPROMA1_B03),e256ByteCnt);
				Write_Eflash_1Byte(EEPROMA2,(eFlashA1|EEPROMA1_M1D),e256ByteCnt,eEEPROMData);
			}

			eEEPROMData = Read_Eflash_Byte(EEPROMA2,(eFlashA1|EEPROMA1_M2),e256ByteCnt);
			if (eEEPROMData != 0x00)
			{
				eEEPROMData = Read_Eflash_Byte(EEPROMA2,(eFlashA1|EEPROMA1_B47),e256ByteCnt);
				Write_Eflash_1Byte(EEPROMA2,(eFlashA1|EEPROMA1_M2D),e256ByteCnt,eEEPROMData);
			}
			e256ByteCnt++;
		}while(e256ByteCnt!=0x00);								// To check 256 bytes
	}

	Erase_Eflash_1K(EEPROMA2,EEPROMA1_B03,0x00);
	Erase_Eflash_1K(EEPROMA2,EEPROMA1_B47,0x00);

	for(eFlashA1=0x00;eFlashA1<4;eFlashA1++)					// 256 bytes *4
	{
		e256ByteCnt=0;
		do
		{
			eEEPROMData = Read_Eflash_Byte(EEPROMA2,(eFlashA1|EEPROMA1_M1D),e256ByteCnt);
			Write_Eflash_1Byte(EEPROMA2,(eFlashA1|EEPROMA1_B03),e256ByteCnt,eEEPROMData);

			eEEPROMData = Read_Eflash_Byte(EEPROMA2,(eFlashA1|EEPROMA1_M2D),e256ByteCnt);
			Write_Eflash_1Byte(EEPROMA2,(eFlashA1|EEPROMA1_B47),e256ByteCnt,eEEPROMData);

			e256ByteCnt++;

		}while(e256ByteCnt!=0x00);								// To check 256 bytes
	}


	Erase_Eflash_1K(EEPROMA2,EEPROMA1_M1D,0x00);
	cSPIEEPROM.fbit.cSPI_ROMCopyStart=0;

	EnableAllInterrupt();
}


void Chk_pDevStus(void)
{
    if(IS_MASK_CLEAR(LENOVOPMFW_Temp,EEPROM_Token))
    {
    	if (SystemIsS0 && (eUpdateEEPROMCnt==0) && (WinFlashMark != 0xFC))
    	{
    		eEEPROMData=Read_Eflash_Byte(EEPROMA2,(EEPROMA1_B03|0x07),0xE1);
    		if (eEEPROMData !=	pDevStus)
    		{
    			RamDebug(0xA1); //modify 0xAA to 0xA1 to avoid repeat
    			RamDebug(pDevStus);

    			eUpdatepDevStusCnt++;
    			eEEPROMBank= 0x07;
    			eEEPROMAddrsss = 0xE1;
    			eEEPROMData = pDevStus;

    		    Update_EEPROMMark();
    		}
    	}
   }
}

void Chk_KB_Backlight(void)
{
    if(IS_MASK_CLEAR(LENOVOPMFW_Temp,EEPROM_Token))
    {
    	if (SystemIsS0 && (eUpdateEEPROMCnt==0) && (WinFlashMark != 0xFC) && (IS_MASK_CLEAR(LED_KB_PWM_Count, BIT7)) )
    	{
			if (UpdateLEDBL_delay !=0)
			{
				UpdateLEDBL_delay --;
				if (UpdateLEDBL_delay ==0)
				{
   			 		eEEPROMData=Read_Eflash_Byte(EEPROMA2,(EEPROMA1_B03|0x07),0xE2);
    				if (eEEPROMData !=	LED_KB_PWM_Step)
    				{
    					RamDebug(0xBB);
    					RamDebug(LED_KB_PWM_Step);

    					eUpdatepDevStusCnt++;
    					eEEPROMBank= 0x07;
    					eEEPROMAddrsss = 0xE2;
    					eEEPROMData = LED_KB_PWM_Step;
    				    Update_EEPROMMark();
    				}
				}
			}
    	}
    }
}

void Chk_UEFIStatus(void)
{
    if(IS_MASK_CLEAR(LENOVOPMFW_Temp,EEPROM_Token))
    {
    	if (SystemIsS0 && (eUpdateEEPROMCnt==0) && (WinFlashMark != 0xFC))
    	{
    		eEEPROMData=Read_Eflash_Byte(EEPROMA2,(EEPROMA1_B03|0x00),0xF1);
    		if ( (eEEPROMData &= 0xC0) != (CURRENT_STATUS &= 0xC0) )
    		{
    			eEEPROMBank= 0x00;
    			eEEPROMAddrsss = 0xF1;
    			eEEPROMData = (CURRENT_STATUS &= 0xC0);
    		    Update_EEPROMMark();
    		}
    	}
    }
}
void Clr_UEFIStatus(void)
{
    if(IS_MASK_CLEAR(LENOVOPMFW_Temp,EEPROM_Token))
    {
    	if ( (eUpdateEEPROMCnt==0) && (WinFlashMark != 0xFC) )
    	{
    		eEEPROMData=Read_Eflash_Byte(EEPROMA2,(EEPROMA1_B03|0x00),0xF1);
    		CURRENT_STATUS &= 0x3F;
    		CURRENT_STATUS |= ( eEEPROMData & 0xC0);
    		if ( eEEPROMData != 0xC0 )
    		{
    			eEEPROMBank= 0x00;
    			eEEPROMAddrsss = 0xF1;
    			eEEPROMData = 0xC0;
    		    Update_EEPROMMark();

    		}
    	}

    }
}

void Update_16ByteEEPROM(void)
{
	BYTE i,j;

	CacheDma(1,FSCEHighLevel);
    DisableAllInterrupt();

	if (Read_Eflash_Byte(EEPROMA2,EEPROMM1_16Byte_end ,0xF0) != 0xFF)
	{
		Erase_Eflash_1K(EEPROMA2,EEPROMA1_16Byte,0x00);
	}

	for (i=0; i<4 ;i++) // 01B800 ~ 01BC00
	{
		for (j=0; j< 0x10 ;j++)
		{
			eEEPROMData = Read_Eflash_Byte(EEPROMA2,(EEPROMA1_16Byte + i),(j*16));
			if(eEEPROMData == 0xFF)
				break;
		}
		if (eEEPROMData == 0xFF)
			break;
	}
	Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16),j+80);
	Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+4,pLastSID);
	Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+6,TEMP_Buff_3);//Record CPU average temperature when CPU over temperture shuntdown .
    
	Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+8,VGA_TBuff3);//Add record GPU temperature when shutdown.


	if(pLastSID == PMIC_Shutdown_ID)
	{
		Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+10,PowerOVP_REG); 
		Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+11,PowerUVP_REG); 
		Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+12,PowerOCP_REG);

		Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+13,PowerOTP_REG); 
	} 
    EnableAllInterrupt();
}



