/*-----------------------------------------------------------------------------
 * TITLE: OEM_MAILBOX.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>



//----------------------------------------------------------------------------
// The function of Mailbox handler
//  Note :
//----------------------------------------------------------------------------
void Hook_MailBox(BYTE Cmd)
{
	BYTE i;

	switch(Cmd)
	{
	case 0x40:
		Cmd_40(MMIO_iDATA8C);
		break;
	case 0x41:
		if(MMIO_iDATA8C==0xA0) Cmd_41_A0(0xFE);
		if(MMIO_iDATA8C==0xA1) Cmd_41_A1(0xFE);
		if(MMIO_iDATA8C==0xA2) Cmd_41_A2(0xFE);
		if(MMIO_iDATA8C==0xA3) Cmd_41_A3(0xFE);
		break;
	case 0x42:
		Cmd_42(MMIO_iDATA8C);
		break;
	case 0x43:
		Cmd_43(0xFE,MMIO_iDATA8C);
		break;
	case 0x45:
		Cmd_45(0xFE,MMIO_iDATA8C);
		break;
	case 0x46:
		Cmd_46(0xFE,MMIO_iDATA8C);
		break;
	case 0x47:
		Cmd_47(0xFE,MMIO_iDATA8C);
		break;
		break;
	case 0x49:
		break;
	case 0x4B:
		i = MMIO_iDATA8C; //Mos: What it is??
		eEEPROMAddrsss 	= MMIO_iDATA8D;
		eEEPROMBank		= MMIO_iDATA8E;
		eEEPROMData		= MMIO_iDATA8F;
        SET_MASK(LENOVOPMFW_Temp,EEPROM_Token);
		Update_EEPROMMark();
        CLEAR_MASK(LENOVOPMFW_Temp,EEPROM_Token);
		Data2PortDirect(0xFE, 0x00);   
		break;

	case 0x4C:
		i = MMIO_iDATA8C;
		eEEPROMAddrsss = MMIO_iDATA8D;
		eEEPROMBank = MMIO_iDATA8E;
		Cmd_4E(0xFE,eEEPROMAddrsss);
		break;

	case 0x4D:
		eEEPROMAddrsss = MMIO_iDATA8C;
		eEEPROMData	= MMIO_iDATA8D;
        SET_MASK(LENOVOPMFW_Temp,EEPROM_Token);
		Update_EEPROMMark();
        CLEAR_MASK(LENOVOPMFW_Temp,EEPROM_Token);
		Data2PortDirect(0xFE, 0x00);         
		break;
	case 0x4E:
		eEEPROMAddrsss = MMIO_iDATA8C;
		Cmd_4E(0xFE,eEEPROMAddrsss);
		break;
	case 0x4F: //reference from Hook_64Port
		Erase_EEPROMAll();
		break;
	case 0x51:
		Cmd_51(0xFE);
		break;
	case 0x52:
		if(MMIO_iDATA8C==0xA0) Cmd_52_A0(0xFE);
		if(MMIO_iDATA8C==0xA3) Cmd_52_A3(0xFE);
		if(MMIO_iDATA8C==0xA4) Cmd_52_A4(0xFE);
		if(MMIO_iDATA8C==0xA5) Cmd_52_A5(0xFE);
		if(MMIO_iDATA8C==0xA6) Cmd_52_A6(0xFE); //LMLKBL0018:add.
		if(MMIO_iDATA8C==0xA7) Cmd_52_A7(0xFE); //LMLKBL0018:add.
		break;
	case 0x53:
		break;
	case 0x56:
		break;
	case 0x58:
		Cmd_58(MMIO_iDATA8C);
		break;
	case 0x59:
		Cmd_59(0xFE,MMIO_iDATA8C,0);
		break;
	case 0x5C:
		Cmd_5C(0xFE);
		break;
	case 0x5D:
		Cmd_5D(MMIO_iDATA8C);
		break;
	case 0x97:
		Cmd_97(MMIO_iDATA8C);
		break;
	case 0xB0:
		Cmd_B0(0xFE,MMIO_iDATA8C);
		break;
	case 0xB4:
		Cmd_B4();
		break;
	case 0x77:
		//CacheDma(1,FSCEHighLevel);
        DisableAllInterrupt();
		Erase_Eflash_1K(0x01,0x20,0x00);
        EnableAllInterrupt();
		break;
	case 0x88:
		//CacheDma(3,FSCEHighLevel);
		//Copy_Eflash_1K(0x00,0x00,0x00, 0x01,0x20,0x00);
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void H2RAMprocess(void)
{
	if( (MMIO_CMD_Ctrl & MMIO_Start_flage)==0 ) return;
	if( (MMIO_iCMD < 0x40) || (MMIO_iCMD >0x60) ) return;
	Clear_RAM_Range(MMIO_rDATA[0],0x07);	// Clear H2RAM return data space.
	//(H2RAM_Table[MMIO_iCMD>>4])();

	MMIO_rDATA_index = 0;
	Hook_MailBox(MMIO_iCMD);

	MMIO_iCMD = 0x00;

	MMIO_CMD_Ctrl &= 0xFD; // Stop Start
}


void VPC_interface(void)
{
	if(nVPC_CMD)
	{
		if(nVPC_CMD != 0x38) //ANGELAS072:add
        	RamDebug(nVPC_CMD);
		uReserve07.fbit.nVPCDriverIn = 1;

		switch(nVPC_CMD)
		{
		case VPCcmdQuery:					//0x10 Query VPC
			nVPC_DATA = uVPCeventSource;
			uVPCeventSource = 0;			//Clear it after PM driver read.
			break;

		case VPCcmdRdBrightnessMax:			//0x11 Read Brightness Max
			//if ( (SYS_STATUS & 0x07) == 0x04 )	// Win8
			if (( (SYS_STATUS & 0x07) >= 0x04 )&&( (SYS_STATUS & 0x07) <= 0x06))	// Win8,win8.1  or win10 ANGELAS010: add win10 mode
			{ nVPC_DATA = WIN8_BRI_MAX_STEP;}	//nVPC_DATA = 0x15;
			else
			{ nVPC_DATA = BRIGHT_MAX_STEP; }	//nVPC_DATA = 0x10;
			break;

		case VPCcmdRdBrightness:			//0x12 Read Brightness
			nVPC_DATA = nBrightValue;
			break;

		case VPCcmdWrBrightness:			//0x13 Write Brightness
			nBrightValue = nVPC_DATA;
			break;

		case VPCcmdRdWlanStatus:			//0x14 Read WLAN (1: enable, 0: disable)
			if( nKillSwitchGET )
				nVPC_DATA = nWirelessLanGET; // need save to EEPROM
			else
				nVPC_DATA = 0;
            ECSMI_SCIEvent(WLANBTFOROSD);//:Add sent Qevent 0x42 for WLAN&BT on/off under win7.
			break;

		case VPCcmdWrWlanStatus:			//0x15 Write WLAN
			if( nKillSwitchGET )
			{
				if ( IS_MASK_SET(DEVICEMODULE,WIRELESS_EXIST) )
				{
					if( nVPC_DATA )
					{
						SET_MASK(DEVICEMODULE,WIRELESS);// Enable WLAN.
						WLAN_OFF_Cnt = 1;				// Delay 0~100msec.
						SET_MASK(pDevStus, pWireless);	// bit1: 1:Wlan Enable.
					}
					else
					{
						CLR_MASK(DEVICEMODULE,WIRELESS);// Disable WLAN.
						WLAN_OFF_Cnt = 1;				// Delay 0~100msec.
						CLR_MASK(pDevStus, pWireless);	// bit1: 0:Wlan Disable.
					}
                    ECSMI_SCIEvent(WLANBTFOROSD);//:Add sent Qevent 0x42 for WLAN&BT on/off under win7.
				}
				else
				{
					CLR_MASK(DEVICEMODULE,WIRELESS);// Disable WLAN.
					WLAN_OFF_Cnt = 1;				// Delay 0~100msec.
					CLR_MASK(pDevStus, pWireless);	// bit1: 0:Wlan Disable.
				}
			}
			break;

		case VPCcmdRdBluetoothStatus:		//0x16 Read Bluetooth
			if( nKillSwitchGET )
				nVPC_DATA = nBlueToothGET;
			else
				nVPC_DATA = 0;
            ECSMI_SCIEvent(WLANBTFOROSD);//:Add sent Qevent 0x42 for WLAN&BT on/off under win7.
            break;

		case VPCcmdWrBluetoothStatus:		//0x17 Write Bluetooth
			if( nKillSwitchGET )
			{
				if ( IS_MASK_SET(DEVICEMODULE,BLUETOOTH_EXIST) )
				{
					if( nVPC_DATA )
					{
						nBlueToothEN;
						SET_MASK(pDevStus, pBluetooth);	// bit2: 1:Bluetooth Enable.
					}
					else
					{
						nBlueToothDIS;
						CLR_MASK(pDevStus, pBluetooth);	// bit2: 0:Bluetooth Disable.
					}
                    ECSMI_SCIEvent(WLANBTFOROSD);//:Add sent Qevent 0x42 for WLAN&BT on/off under win7.
				}
			}
			break;

		case VPCcmdRdInverterStatus:		//0x18 Read Inverter On/Off
			if ( !Read_ENBKL_IN() )
				nVPC_DATA = 0;
			else
				nVPC_DATA = 1;
			break;

		case VPCcmdRdNovoStatus:			//0x19 Read Novo
			nVPC_DATA = uNovoVPCCount;		//uNovoVPCCount count 2sec set uNovoVPCCount=1;
			uNovoVPCCount = 0;
			break;

		case VPCcmdQuery2:					//0x1A Query2 VPC
			nVPC_DATA = uVPCeventSource2;
            uVPCeventSource2=0x00; 
			break;

		case VPCcmdRdTouchPadStatus:		//0x1B Read TouchPad
		//	if(IS_MASK_SET(pDevStus, pENABLE_TP))//ANGELAG008: remove	// cmd 59 7A,7B.......
				if(IS_MASK_CLEAR(pDevStatus1, b7DisableTP)) //ANGELAG008: add 
				nVPC_DATA = 1;
			else
				nVPC_DATA = 0;
			break;

		case VPCcmdWrTouchPadStatus:		// 0x1C Write TouchPad
			// optimize ,add no tp driver mode

//72JERRY056:s+Add enable/disable TP function by HW pin.
			if(READ_EC_TP_ON())
				{
				EC_TP_ON_LOW();
				SET_MASK(pDevStatus1, b7DisableTP);
				}
			else
				{
				EC_TP_ON_HI();
				CLEAR_MASK(pDevStatus1, b7DisableTP);
				}
			uVPCeventSource = TouchPad;
			uVPCeventSource2 = 0;
			ECQEvent(SDV_VPC_notify);
//72JERRY056:e+Add enable/disable TP function by HW pin.
			//Add VPC control touchpad function.
		/*	if(uReserve07.fbit.nTPDriverIn ==0) //ANGELAG008: remove start
			{
				if (IS_MASK_SET(pDevStus, pENABLE_TP))
				{

					ECSend2Port(2, 0xF5);
                    TPCLK_OUTPUT;
                    TPCLK_LOW();
					TouchPadCount = 3;
					CLR_MASK(pDevStus, pENABLE_TP);
				}
				else
				{
					TPCLK_OUTPUT;
      				TPCLK_HI();
                    TPCLK_ALT;
					ECSend2Port(2, 0xF4);
					TouchPadCount = 3;
					SET_MASK(pDevStus, pENABLE_TP);
				}
				//Add Sent Q_EVENT to VPC show TP OSD.
	        	uVPCeventSource = TouchPad;
		    	uVPCeventSource2 = 0;
				ECSMI_SCIEvent(SDV_VPC_notify);
				//Add Sent Q_EVENT to VPC show TP OSD.		
			}
			else
			{
				e0_prefix_code(0x51,MAKE_EVENT);
				CPL_MASK(pDevStus, pENABLE_TP);
				SET_MASK(StatusKeeper, b5TP_Event);
				e0_prefix_code(0x51,BREAK_EVENT);
			}*/ //ANGELAG008: remove end
			//ANGELAG008: add start
							/*if(IS_MASK_SET(pDevStatus1,b5TPDRIVER_STATUS))
				{
                 		 if((nVPC_DATA==0x01)&&IS_MASK_CLEAR(pDevStatus1,b7DisableTP)) //Set Enable Command, but now state is Enable
				  	break;
				  if((nVPC_DATA==0x00)&&IS_MASK_SET(pDevStatus1,b7DisableTP))   //Set Disable Command, but now state is Disable
				  	break;	

				  if(IS_MASK_SET(pDevStatus1,b7DisableTP))  //enable TP
			      {
					ECSMI_SCIEvent(0x3E);
					SET_MASK(pDevStatus3,b6EnDisTPSend); 
					if(IS_MASK_SET(pDevStatus1,b7DisableTP))
					{
					   SET_MASK(pDevStatus3,b7TPStateFormer);
					}
					else
					{
					   CLEAR_MASK(pDevStatus3,b7TPStateFormer);
					}  				
				 }
			     else  //Disable TP
			     {
					 ECSMI_SCIEvent(0x3F);
					 SET_MASK(pDevStatus3,b6EnDisTPSend); 
					 if(IS_MASK_SET(pDevStatus1,b7DisableTP))
					 {
						SET_MASK(pDevStatus3,b7TPStateFormer);
					 }
					 else
					 {
						CLEAR_MASK(pDevStatus3,b7TPStateFormer);
					 }					 
			     }	
			    }*///72JERRY043:Support PTP TP show OSD,there isn't driver notify EC TP status,EC keep flag to rom.
			//ANGELAG008: add end
			// Add VPC control touchpad function.
			// optimize ,add no tp driver mode.
//72JERRY043:s+Support PTP TP show OSD,there isn't driver notify EC TP status,EC keep flag to rom.
			/*if(IS_MASK_SET(SYS_MISC1,ACPI_OS))
   		 {

			Buffer_Key(0x14); //"Ctrl" key
			Buffer_Key(0xe0); //
			Buffer_Key(0x1f); //"win" key
			Buffer_Key(0x5F); //"F24" key

				Buffer_Key(0xF0);
				Buffer_Key(0x14);
				Buffer_Key(0xe0);
				Buffer_Key(0xf0);
				Buffer_Key(0x1f);
				Buffer_Key(0xF0);
				Buffer_Key(0x5F);
	
		 eEEPROMData=Read_Eflash_Byte(EEPROMA2,(EEPROMA1_B03|0x07),0xE2);
			 if(eEEPROMData==0xff)
			 	{
			 	pDevStatus1=0;
			 	}
			 else
			 	{
			 	pDevStatus1=eEEPROMData;
			 	}
			 
			CPL_MASK(pDevStatus1, b7DisableTP);

			eEEPROMBank= 0x07;
    		eEEPROMAddrsss = 0xE2;
    		eEEPROMData = pDevStatus1;
    		Update_EEPROMMark();
			}
			uVPCeventSource = TouchPad;
		  	uVPCeventSource2 = 0;
		  	ECSMI_SCIEvent(SDV_VPC_notify);*///72JERRY051:Remove 72JERRY043 solution for PTP TP enable/disable.
			//72JERRY043:e+Support PTP TP show OSD,there isn't driver notify EC TP status,EC keep flag to rom.
			break;

		case VPCcmdRdCameraStatus:			// 0x1D Read Camera
			nVPC_DATA = nCameraPwrGET;		// need restore to EEPROM
			break;

		case VPCcmdWrCameraStatus:			//0x1E Write Camera
			if(nVPC_DATA)
			{ SET_MASK(pDevStus, pCamera); }	// Enable Camera Power.
			else
			{ CLR_MASK(pDevStus, pCamera); }	// Disable Camera Power.
			break;

		case VPCcmdRd3GStatus:				//0x1F Read 3G
			if( nKillSwitchGET )
				nVPC_DATA = nWWAN3GLanGET;
			else
				nVPC_DATA = 0;
			break;

		case VPCcmdWr3GStatus:				//0x20 Write 3G
			/*
	   		if(nKillSwitchGET)
	   		{
				pDevStus.bit.p3GDevice = nVPC_DATA;
				if(p3GDeviceGET)
					SET_MASK(DEVICEMODULE,WWAN_3G);
				else
					CLR_MASK(DEVICEMODULE,WWAN_3G);
	   		}
			*/
			break;

		case VPCcmdRdODDStatus:				//0x21 Read ODD Notify status
			if ( IS_MASK_SET(uODDPWRsaving, uODD_PWRon) )	// Check bit0: ODD power on.
			{
				CLR_MASK(uODDPWRsaving, uODD_PWRon);		// Clear bit0: ODD power on.
				nVPC_DATA = 0x01;			// User press eject btn
			}
			else if( IS_MASK_SET(uODDPWRsaving, uODD_PWRoff1) )
			{
				CLR_MASK(uODDPWRsaving, uODD_PWRoff1);		// Clear bit0: ODD power off.
				nVPC_DATA = 0x02;			// power off
			}
			else if( IS_MASK_SET(uODDPWRsaving, uODD_InitEnd) )
			{
				CLR_MASK(uODDPWRsaving, uODD_InitEnd);		// Clear bit6: ODD init End.
				nVPC_DATA = 0x03;			// initial completion
			}
			break;

		case VPCcmdWrDustModeStatus:		//0x22 Write DustMode
			#if Lenovo_FAN_Dust				// Dust mode
			if(nVPC_DATA)
			{
				//putchar('w');putchar('R');
				SET_MASK(SMartNoise, b5DedustingMode);
				SET_MASK(THERMAL_STATUS, INITOK);	// Enable Dust mode.
			}
			else
			{
				//putchar('w');putchar('S');
				//CLR_MASK(SMartNoise, b5DedustingMode);
				StartFanClean = 0; 						// stop dust mode
				CLR_MASK(THERMAL_STATUS, INITOK);		// Disable Dust mode.
				FAN_PWM = FAN_PWM_Min;					// PWM minimum.
				//SET_MASK(SMartNoise, b2StopReason);	// Dust mode by Cancel.
				SMartNoise = 0x04;
			}
			#endif	// Lenovo_FAN_Dust
			break;

		case VPCcmdRdAllRFStatus:			//0x23 Read All RF Status
			nVPC_DATA = nKillSwitchGET;
			break;

		case VPCcmdWrAllRFStatus:			//0x24 Write All RF Status
			break;

		case VPCcmdRdCameraUtilityStatus:	//0x25 Read Camera Utility Status
			nVPC_DATA = 0;
			break;

		case VPCcmdWrCameraUtilityStatus:	//0x26 Write Camera Utility Status
			break;

		case VPCcmdSetSuperPerformance: 	//0x28 Set SuperPerformance mode
			if( nVPC_DATA)
			{
				//printf("ssp");
				SET_MASK(uVGATurboFun, uEnableGPS);
				SET_MASK(EM7FUNCTION,LSPRUNNING);	// Enable Super performance mode.
			}
			else
			{
				//printf("dsp");
				CLR_MASK(uVGATurboFun, uEnableGPS);
				CLR_MASK(EM7FUNCTION,LSPRUNNING);	// Disable Super performance mode.
			}
			break;

		case VPCcmdRdGCPUstatus:			//0x29 Read GCPU notify status
			if(nSwitchstateGET)
			{ nVPC_DATA = 0; }
			else
			{ nVPC_DATA = 1; }
			break;
		case VPCcmdYogaModeControl:			//0x2A Yoga Mode Control
                RamDebug(nVPC_DATA); //MartinH008:add
				switch(nVPC_DATA)
                {				
				  case 0x01:
                       YogaModeStatus = H0x01NotebookMode;	
				       break;	
				  case 0x02:
                       YogaModeStatus = H0x02TabletMode;	
				        break;
				  case 0x03:
                       YogaModeStatus = H0x03StandMode;			
				       break;
				  case 0x04:
                       YogaModeStatus = H0x04TendMode;
				       break;										
				  case 0x08:     //Disable KB & TP
                    //if(!Read_LID_SW_IN())  break;    //MARTINY029:Remove

                    //RamDebug1(0xF5);    
			      	//RamDebug1(pDevStatus1);
			
					CLEAR_MASK(pDevStatus3,b5VPCEnDISTP);
                    SET_MASK(pDevStatus1,b6MODE_STATUS);
                    SET_MASK(pDevStatus1,b4LNVAPP_STATUS);
                    SET_MASK(pDevStatus1,b2DisableKB);
                    if(IS_MASK_CLEAR(pDevStatus1,b3Scancode_STATUS))
                    {
                        SET_MASK(pDevStatus1,b3Scancode_STATUS);
                    }

			        if((IS_MASK_SET(pDevStatus1,b5TPDRIVER_STATUS))&&(IS_MASK_CLEAR(pDevStatus1,b7DisableTP))) 
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

					    SET_MASK(pDevStatus1,b2DisableKB);
						//RamDebug1(0x61);
			         }
					if  (IS_MASK_SET(SYS_MISC1, ACPI_OS)) //HEGANGS027:Add the function of YMC control the TP
					{
						if(READ_EC_TP_ON())//HEGANGS033:Modify VPC notify when TP state is changed
						{
							EC_TP_ON_LOW();
							SET_MASK(pDevStatus1, b7DisableTP);
							uVPCeventSource = TouchPad;
							uVPCeventSource2 = 0;
							ECQEvent(SDV_VPC_notify);
						}
					}
				
					
                           SET_MASK(pDevStatus1,b2DisableKB);
                           //RamDebug1(0xB2);
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
				     break;
				   case  0x09:     //Enable KB & TP
                     //if(!Read_LID_SW_IN())   //MARTINY029:Remove
				   	 //  break;                         //MARTINY029:Remove

				     CLEAR_MASK(pDevStatus1,b6MODE_STATUS);
					 if(IS_MASK_CLEAR(pDevStatus2,b1TransAP_CTRL))  //MARTINY029:Add
					 {
				      if(!Read_LID_PAD())  
					    break; 
					 } 
                   //RamDebug1(0xF4);
				   //RamDebug1(pDevStatus1);
				   CLEAR_MASK(pDevStatus3,b5VPCEnDISTP); 

				   SET_MASK(pDevStatus1,b4LNVAPP_STATUS);
				   CLEAR_MASK(pDevStatus1,b2DisableKB);
				   if(IS_MASK_CLEAR(pDevStatus1,b3Scancode_STATUS))
				   {
						   SET_MASK(pDevStatus1,b3Scancode_STATUS);
				   }

			        if((IS_MASK_SET(pDevStatus1,b5TPDRIVER_STATUS))&&(IS_MASK_SET(pDevStatus1,b7DisableTP))) 
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

				       CLEAR_MASK(pDevStatus1,b2DisableKB);
				       Enable_Any_Key_Irq();
					   //RamDebug1(0x51);	
			        }
					if  (IS_MASK_SET(SYS_MISC1, ACPI_OS))//HEGANGS027:Add the function of YMC control the TP
					{
						if(!READ_EC_TP_ON())
						{
							if(Read_LID_PAD())//HEGANGS040:Ignore the enable TP CMD if PAD_LID is closed
							{
								EC_TP_ON_HI();
								CLEAR_MASK(pDevStatus1, b7DisableTP);
								uVPCeventSource = TouchPad;
								uVPCeventSource2 = 0;
								ECQEvent(SDV_VPC_notify);
							}
						}
					}

                    CLEAR_MASK(pDevStatus1,b2DisableKB);
			        Enable_Any_Key_Irq();
					//RamDebug1(0xB4);

		             break;
				
				  case 0x0A:
                      SET_MASK(pDevStatus2,b1TransAP_CTRL);
				      break;
                  case 0x0B:
                       CLEAR_MASK(pDevStatus2,b1TransAP_CTRL);
				       break;
				  default:
				        break;
			    }					
					
				 break;

		case VPCcmdRdDustModeStatus:		//0x2B Read Dust mode status
			#if Lenovo_FAN_Dust 			// Dust mode
			if ( IS_MASK_SET(SMartNoise, b1FanCleanStart) )
			{
				//putchar('r');putchar('R');
				nVPC_DATA = 0x03;	//Dust Mode running & support Dust mode
			}
			else
			{
				if ( IS_MASK_CLEAR(SMartNoise, b2StopReason) )
				{
					//putchar('r');putchar('S');
					nVPC_DATA = 0x05;	//Dust Mode stopped & support Dust mode
				}
				else
				{
					nVPC_DATA = 0x85;	//Dust Mode running & support Dust mode
				}
			}
				//ANGELAG008:add start
                #else
				   nVPC_DATA = 0x00;
				//ANGELAG008:add end
			#endif	// Lenovo_FAN_Dust
			break;

		case VPCcmdRdSuperPerformance:		//0x2D Read status of SuperPerformance mode on EM7
			nVPC_DATA = EM7FUNCTION;
			break;

		case VPCcmdRdOnekeyStatus:			//0x31 Read OneKey theater
			nVPC_DATA = 0x40;
			break;

		case VPCcmdRdGenenalstatus:			//0x2F Read Genenal event status	//09110502 Isaac Start
			//if( IS_MASK_SET(EM7FUNCTION,OVERTEMPATI) )			// ATI graphic.
			//{
			//	nVPC_DATA = 0x08;			//bit3
			//	CLR_MASK(EM7FUNCTION,OVERTEMPATI);	// Disable Over Temp ATI.
			//}
			if ( IS_MASK_SET(LENOVOPMFW,BATTERY_MAIN_CAL) || IS_MASK_SET(LENOVOPMFW_Temp,BATTERY_CALIBRATION_OK) )	// Battery Calibration
			{
				CLR_MASK(LENOVOPMFW,BATTERY_MAIN_CAL);
				CLR_MASK(LENOVOPMFW,BATTERY_CAL_END);
                CLR_MASK(LENOVOPMFW_Temp,BATTERY_CALIBRATION_OK);
				nVPC_DATA = 0x04;
			}
            if(IS_MASK_SET(EM9_NEWFUN,b0_FnPlus))
            {
                CLR_MASK(EM9_NEWFUN,b0_FnPlus);
                nVPC_DATA = 0x10; 
            }      
            else if(IS_MASK_SET(EM9_NEWFUN,b1_FnMinus))
            {
               	CLR_MASK(EM9_NEWFUN,b1_FnMinus);
               	nVPC_DATA = 0x20; 
            }                                                       
			break;

		case VPCcmdWrQuietStatus:			//0x32 Write Quiet Status
			break;

		case VPCcmdWrBacklightStatus:		//0x33 Write LCD backlight Status inverter
			if( Read_BKOFF() )
			{
				if( !nVPC_DATA )	// Turn off backlight.
				{
					//printf("\nDb");
					SET_MASK(cCmd, b3BkOff);
					SET_MASK(OEMControl, b1BkOff);//ANGELAG005:add
				}
			}
			else
			{
				if( nVPC_DATA == 0x01 )	// Turn on backlight.
				{
					//printf("\nEb");
					CLR_MASK(cCmd, b3BkOff);
					CLR_MASK(OEMControl, b1BkOff);//ANGELAG005:add
				}
			}
			break;

		//Add VPC 0x38 command.
		case VPCcmdRdBatteryACStatus:			//0x38 Read Battery\Adapter Plug in\Plug out status 
            nVPC_DATA = 0x00;
            if(IS_MASK_SET(SYS_STATUS,AC_ADP))
            {
            	SET_MASK(nVPC_DATA, BIT0);
                CLEAR_MASK(nVPC_DATA, BIT1);
            }	
			else
			{
                SET_MASK(nVPC_DATA, BIT1);
                CLEAR_MASK(nVPC_DATA, BIT0);
            }	
			if(IS_MASK_SET(SEL_STATE0,PRESENT_A))
            {
                SET_MASK(nVPC_DATA, BIT2);
                CLEAR_MASK(nVPC_DATA, BIT3);
            }	
			else
			{
                SET_MASK(nVPC_DATA, BIT3);
                CLEAR_MASK(nVPC_DATA, BIT2);
            }
			
			//Bit0: Adapter insert; Bit1: Adatper plugout ;
			//Bit2: the main battery insert; Bit3: the main battery plugout;
			//Bit4: the second battery insert  Bit5: the second battery plugout 		 	
	        break;
		default:
			break;
		}
		nVPC_CMD = 0x00;
	}
}

//ANGELAG008: add start
void TPInfor_ToVPC(void)
{
   if(SystemIsS0)
   {
      if(IS_MASK_SET(pDevStatus3,b6EnDisTPSend))
      {
         if((IS_MASK_SET(pDevStatus3,b7DisableTP))!=(IS_MASK_SET(pDevStatus1,b7DisableTP)))
         {
            CLEAR_MASK(pDevStatus3,b6EnDisTPSend);
			if(uReserve07.fbit.nVPCDriverIn==1)   // Check VPC driver
			{
			  uVPCeventSource = TouchPad;
			  uVPCeventSource2 = 0;
			  ECQEvent(SDV_VPC_notify);
			}			
         }
      } 
   }  
}
//ANGELAG008: add end

void ThermalMailBox(void)
{
	if(EXT_NAMESPACE_BANK)
	{
       	RamDebug(EXT_NAMESPACE_BANK);
       	RamDebug(EXT_NAMESPACE_INDEX);
		switch(EXT_NAMESPACE_BANK)
		{
		case 0x81:
			if (EXT_NAMESPACE_INDEX==0x02)
				EXT_NAMESPACE_DATA = (FAN_PWM/2);
			if (EXT_NAMESPACE_INDEX==0x04)
				EXT_NAMESPACE_DATA = nAtmFanSpeed;
			else if (EXT_NAMESPACE_INDEX==0x07)
				EXT_NAMESPACE_DATA = FAN_Tab_OFF;
			else if (EXT_NAMESPACE_INDEX==0x08)
				EXT_NAMESPACE_DATA = FAN_Tab_LOW;
			else if (EXT_NAMESPACE_INDEX==0x09)
				EXT_NAMESPACE_DATA = FAN_Tab_HI;
			else if (EXT_NAMESPACE_INDEX==0x0A)
				EXT_NAMESPACE_DATA = THR_Turo_Rem;
			else if (EXT_NAMESPACE_INDEX==0x0B)
				EXT_NAMESPACE_DATA = THR_PRO_OFF;
			else if (EXT_NAMESPACE_INDEX==0x0C)
				EXT_NAMESPACE_DATA = THR_Turo_OFF;
			else if (EXT_NAMESPACE_INDEX==0x0D)
				EXT_NAMESPACE_DATA = THR_PRO_ON;
			else if (EXT_NAMESPACE_INDEX==0x0E)
				EXT_NAMESPACE_DATA = VFAN_Tab_OFF;
			else if (EXT_NAMESPACE_INDEX==0x0F)
				EXT_NAMESPACE_DATA = VFAN_Tab_LOW;
			else if (EXT_NAMESPACE_INDEX==0x10)
				EXT_NAMESPACE_DATA = VFAN_Tab_HI;
			else if (EXT_NAMESPACE_INDEX==0x11)
				EXT_NAMESPACE_DATA = VTHR_Turo_Rem;
			else if (EXT_NAMESPACE_INDEX==0x12)
				EXT_NAMESPACE_DATA = VTHR_PRO_OFF;
			else if (EXT_NAMESPACE_INDEX==0x13)
				EXT_NAMESPACE_DATA = VTHR_Turo_OFF;
			else if (EXT_NAMESPACE_INDEX==0x14)
				EXT_NAMESPACE_DATA = VTHR_PRO_ON;
			break;

		case 0x82:
			break;

		case 0x84:
			if (EXT_NAMESPACE_INDEX==0x00)
				EXT_NAMESPACE_DATA = 0;
			else if (EXT_NAMESPACE_INDEX==0x01)
				EXT_NAMESPACE_DATA = nRamTemp;
			else if (EXT_NAMESPACE_INDEX==0x02)
				EXT_NAMESPACE_DATA = nVramTemp;
			else if (EXT_NAMESPACE_INDEX==0x03)
				EXT_NAMESPACE_DATA = nNBTemp;
		}
        RamDebug(EXT_NAMESPACE_DATA);
		EXT_NAMESPACE_INDEX = 0;
		EXT_NAMESPACE_BANK = 0;
	}
}
//------------------------------------------------------------
// Debug function
//------------------------------------------------------------
#define DebugRamAddr 0xA00   //ANGELAS064:change '0xF00' to '0xA00'
#define DebugRamRange 0xFF

void RamDebug(unsigned char dbgcode)
{
  	PORT_BYTE_PNTR ClearIndex;
 	PORT_BYTE_PNTR byte_register_pntr;
    BYTE index;
  	BYTE i;

    byte_register_pntr = DebugRamAddr+DebugRamRange;
    index = *byte_register_pntr;
    *byte_register_pntr +=1;

    if ( *byte_register_pntr == DebugRamRange )
   	{
   		*byte_register_pntr = 0;
       	ClearIndex= DebugRamAddr;

		for (i=0x00;i<DebugRamRange;i++)
     	{
         	*ClearIndex=0x00;
           	ClearIndex++;
     	}
  	}

	byte_register_pntr = DebugRamAddr + index;
    *byte_register_pntr = dbgcode;
}


//ANGELAS064:add start
void ClearDiagRam(void)
{
  	BYTE *ClearIndex;
  	WORD i;
    ClearIndex= (XBYTE *)DiagNameSpace;
	for (i=0x00;i<=0xFF;i++)
	{
		*ClearIndex=0x00;
		ClearIndex++;
	}

	//Rdbg_index=0;  
}
void DiagSMBusOperation(void)     
{
	BYTE result;
	BYTE HOSTx_x_State;
			 
	if( SMB_PRTC_Diag!= 0x00 )
	{
		SMB_STS_Diag = 0x00;

		SMB_PRTC_Diag = SMB_PRTC_Diag&0x0F; 
	#if 0
		if((vOSSMbusComd_table[(SMB_PRTC & 0x0F)])()) 
		{
			SMB_STS = 0x80;
		}
		else 
		{
			if( IS_MASK_SET(HOSTA_A,BIT0) ) { SMB_STS = 0x1A; }
			else if( IS_MASK_SET(HOSTA_A,BIT2) ) { SMB_STS = 0x17; }
			else if( IS_MASK_SET(HOSTA_A,BIT3) ) { SMB_STS = 0x07; }
			else if( IS_MASK_SET(HOSTA_A,BIT4) ) { SMB_STS = 0x19; }
			else if( IS_MASK_SET(HOSTA_A,BIT5) ) { SMB_STS = 0x10; }
			else if( IS_MASK_SET(HOSTA_A,BIT6) ) { SMB_STS = 0x18; }
		}
	#else
	
		if(SMB_PRTC_Diag == 0x04)		//0x04-Send Byte
		{
			result = bSMBusSendByte(SMB_EC_Chennel_Diag, SMB_ADDR_Diag, SMB_CMD_Diag );
		}
		else if(SMB_PRTC_Diag == 0x05)		//0x05-Receive Byte
		{
			result = bSMBusReceiveByte(SMB_EC_Chennel_Diag, SMB_ADDR_Diag, &SMB_DATA_Diag);
		}
		else if(SMB_PRTC_Diag == 0x06)		//0x06-Write Byte
		{
			result = bRWSMBus(SMB_EC_Chennel_Diag, SMbusWB, SMB_ADDR_Diag, SMB_CMD_Diag, &SMB_DATA_Diag, SMBus_NoPEC);
		}
		else if(SMB_PRTC_Diag == 0x07)		//0x07-Read Byte
		{
			result = bRWSMBus(SMB_EC_Chennel_Diag, SMbusRB, SMB_ADDR_Diag, SMB_CMD_Diag, &SMB_DATA_Diag, SMBus_NoPEC);
		}
		else if(SMB_PRTC_Diag == 0x08)		//0x08-Write Word
		{
			result = bRWSMBus(SMB_EC_Chennel_Diag, SMbusWW, SMB_ADDR_Diag, SMB_CMD_Diag, &SMB_DATA_Diag, SMBus_NoPEC);
		}
		else if(SMB_PRTC_Diag == 0x09)		//0x09-Read Word
		{
			result =  bRWSMBus(SMB_EC_Chennel_Diag, SMbusRW, SMB_ADDR_Diag, SMB_CMD_Diag, &SMB_DATA_Diag, SMBus_NoPEC);
		}
		else if(SMB_PRTC_Diag == 0x0A)		//0x0A-Write Block
		{
			result = bWSMBusBlock(SMB_EC_Chennel_Diag, SMbusWBK, SMB_ADDR_Diag, SMB_CMD_Diag, &SMB_DATA_Diag, SMB_BCNT_Diag,SMBus_NoPEC);
		}
		else if(SMB_PRTC_Diag == 0x0B)		//0x0B-Read Block
		{
			result = bRSMBusBlock(SMB_EC_Chennel_Diag, SMbusRBK, SMB_ADDR_Diag, SMB_CMD_Diag, &SMB_DATA_Diag);
		}

		if(SMbusCh2 == SMB_EC_Chennel_Diag)
		{
		    HOSTx_x_State = HOSTA_B;
		}
		else if(SMbusCh1 == SMB_EC_Chennel_Diag)
		{
			HOSTx_x_State = HOSTA_A;
		}
		else if(SMbusCh3== SMB_EC_Chennel_Diag)
		{
			HOSTx_x_State = HOSTA_C;
		}
				 
		if(result)
		{
			SMB_STS_Diag = 0x80;
		}
		else
		{
			//SMB_STS = 0x5A;//W074>>>:Battery FW update channel use B.
			if( IS_MASK_SET(HOSTx_x_State,BIT0) ) { SMB_STS_Diag = 0x1A; }
			else if( IS_MASK_SET(HOSTx_x_State,BIT2) ) { SMB_STS_Diag = 0x17; }
			else if( IS_MASK_SET(HOSTx_x_State,BIT3) ) { SMB_STS_Diag = 0x07; }
			else if( IS_MASK_SET(HOSTx_x_State,BIT4) ) { SMB_STS_Diag = 0x19; }
			else if( IS_MASK_SET(HOSTx_x_State,BIT5) ) { SMB_STS_Diag = 0x10; }
			else if( IS_MASK_SET(HOSTx_x_State,BIT6) ) { SMB_STS_Diag = 0x18; }
		}
	#endif
			
		SMB_PRTC_Diag = 0x00;
	
		//ECSMI_SCIEvent(ACPI_ACIN_SCI);	// toggle HOST
	}
}

void Diag_interface(void)
{
    BYTE *Tmp_XPntr;  
  	if(nDiagInterface_CMD)
	{
	    RamDebug(0xDC);  
        RamDebug(nDiagInterface_CMD);		
		RamDebug(nDiagInterface_ECRegH);   
		RamDebug(nDiagInterface_ECRegL);  

		switch(nDiagInterface_CMD)
		{
		case DiagReadECReg:					//0x11 Read EC Register
			nDiagInterface_DATA = *((XBYTE *)((nDiagInterface_ECRegH<<0x08)+nDiagInterface_ECRegL));
			RamDebug(nDiagInterface_DATA);  
			//uVPCeventSource = 0;			//Clear it after AP read.
			break;
		case DiagWriteECReg:					//0x12 Write EC Register
			*((XBYTE *)((nDiagInterface_ECRegH<<0x08)+nDiagInterface_ECRegL))= nDiagInterface_DATA;
			RamDebug(nDiagInterface_DATA);  
			//uVPCeventSource = 0;			//Clear it after AP read.
			break;	
        case DiagReadECRAM: 				//0x13 Read EC RAM
            nDiagInterface_DATA = *((XBYTE *)((nDiagInterface_ECRegH<<0x08)+nDiagInterface_ECRegL));
			RamDebug(nDiagInterface_DATA);  
	        break;
        case DiagWriteECRAM:					//0x14 Write EC RAM
	        *((XBYTE *)((nDiagInterface_ECRegH<<0x08)+nDiagInterface_ECRegL)) = nDiagInterface_DATA;
			RamDebug(nDiagInterface_DATA); 
	        break;
        case DiagReadECSpace: 				//0x80 Read EC Register
	        nDiagInterface_DATA =   *((XBYTE *)(NameSpace+nDiagInterface_ECRegL));
			RamDebug(nDiagInterface_DATA);  
	        break;
       	case DiagWriteECSpace:					//0x81 Write EC Register
	        *((XBYTE *)(NameSpace+nDiagInterface_ECRegL)) = nDiagInterface_DATA;
			RamDebug(nDiagInterface_DATA);  
	        break;				  

        case DiagSMBusOperate:					//0x20 Write EC Register
            if(0x00 == SMB_EC_Chennel_Diag)
				 SMB_EC_Chennel_Diag = SMbusCh2;
			else 
				 SMB_EC_Chennel_Diag--;
	        DiagSMBusOperation();
	        break;
 				 
        case DiagSimulateFnKey:					//0x21 Simulate Function Key
            if((nDiagInterface_DATA>0)&&(nDiagInterface_DATA<=12))
			{
			    FunctionKeyDebounce=(nDiagInterface_DATA<<0x04)+10; //ANGELAS084:add
			    (HotKey_Fn_Fx[nDiagInterface_DATA-1])(0);	//ANGELAS084:add
		    }  
	        break;
		//MEILING013:S+
		case DiagSimulateQEvent:
		   	ECQEvent(nDiagInterface_DATA);
		   	break;
		//MEILING013:E+
        case DiagReadTemp:					//0x26 Read temperature
            if(0x01 == nDiagInterface_DATA)
			{
				nDiagInterface_DATA = nCpuTemp;
		    } 
			else if(0x02 == nDiagInterface_DATA)
			{
				nDiagInterface_DATA = VGA_TEMP;
			}
			else if(0x03 == nDiagInterface_DATA)
			{
				//nDiagInterface_DATA = nVramTemp; //MEILING013:remove.
				nDiagInterface_DATA = EXTVGA_TEMP; //MEILING013:add.
			}
			else if(0x04 == nDiagInterface_DATA)
			{
				//nDiagInterface_DATA = nNBTemp;  //MEILING013:remove.
				nDiagInterface_DATA = ThermistorCPU_TEMP; //MEILING013:add.
			}
			else if(0x05 == nDiagInterface_DATA)
			{
				nDiagInterface_DATA = nRamTemp;
			}
			//MEILING013:S+
			else if(0x06 == nDiagInterface_DATA)
			{
				nDiagInterface_DATA = nVramTemp;
			}
			//MEILING013:E+
	        break;	
        case DiagReadFanSpeed:					//0x27 Read Fan Speed
            if(0x01 == nDiagInterface_DATA)
			{
			    nDiagInterface_DATA = nAtmFanSpeed;
		    } 
			else if(0x02 == nDiagInterface_DATA)
			{
				//nDiagInterface_DATA = nAtmFan2Speed;
				nDiagInterface_DATA = 0x00; //MEILING013:add.
			}
			//MEILING013:S+
			else if(0x03 == nDiagInterface_DATA)
			{
				nDiagInterface_DATA = FAN_PWM;
			}
			else if(0x04 == nDiagInterface_DATA)
			{
				//nDiagInterface_DATA = FAN2_PWM;
				nDiagInterface_DATA = 0x00;
			}
			//MEILING013:E+
	        break;	
        case DiagWriteFanSpeed:					//0x28 Write Fan Speed
            if(nDiagInterface_DATA >= 0x80) //bi7 = 1: Fan2, //bit7 = 0: Fan1
			{
			    uReserve07.fbit.nFanManual = 1;//MEILING013:add.
			    //DebugFan2RPMT = nDiagInterface_DATA -128;
			    ManualFanPRM = nDiagInterface_DATA - 128; //MEILING013:add.
		    } 
			else 
			{
				//uReserve07.fbit.b7Fan2Manual = 1;//MEILING013:add.
				//DebugFan2RPMT = nDiagInterface_DATA;
				//ManualFan2PRM = nDiagInterface_DATA;  //MEILING013:add.
			}
	        break;	
		case DiagReadThermalTable:					//0x29 Read Thermal Table
            /*
                   if(IS_MASK_CLEAR(nDiagInterface_DATA,FanNum)&&IS_MASK_CLEAR(nDiagInterface_DATA,FanType)) //Fan1 CPU
			{
				if(0x00 == (nDiagInterface_DATA&0x03))
			        	nDiagInterface_DATA = *ThermalTalbe1CPUDiag[(nDiagInterface_DATA&0x1C)>>0x02].FanOn;
				else if(0x01 == (nDiagInterface_DATA&0x03))
					nDiagInterface_DATA = *ThermalTalbe1CPUDiag[(nDiagInterface_DATA&0x1C)>>0x02].FanOff;
				else if(0x02 == (nDiagInterface_DATA&0x03))
					nDiagInterface_DATA = *ThermalTalbe1CPUDiag[(nDiagInterface_DATA&0x1C)>>0x02].RPM;
		      } 
			else  if(IS_MASK_CLEAR(nDiagInterface_DATA,FanNum)&&IS_MASK_SET(nDiagInterface_DATA,FanType))  //Fan1 VGA
			{
			 	if(0x00 == (nDiagInterface_DATA&0x03))
			     		nDiagInterface_DATA = *ThermalTalbe1VGADiag[(nDiagInterface_DATA&0x1C)>>0x02].FanOn;
				else if(0x01 == (nDiagInterface_DATA&0x03))
					nDiagInterface_DATA = *ThermalTalbe1VGADiag[(nDiagInterface_DATA&0x1C)>>0x02].FanOff;
				else if(0x02 == (nDiagInterface_DATA&0x03))
					nDiagInterface_DATA = *ThermalTalbe1VGADiag[(nDiagInterface_DATA&0x1C)>>0x02].RPM;
		      } 
                   else  if(IS_MASK_SET(nDiagInterface_DATA,FanNum)&&IS_MASK_CLEAR(nDiagInterface_DATA,FanType))  //Fan2 CPU
			{
			 	if(0x00 == (nDiagInterface_DATA&0x03))
			      		nDiagInterface_DATA = *ThermalTalbe2CPUDiag[(nDiagInterface_DATA&0x1C)>>0x02].FanOn;
				else if(0x01 == (nDiagInterface_DATA&0x03))
					nDiagInterface_DATA = *ThermalTalbe2CPUDiag[(nDiagInterface_DATA&0x1C)>>0x02].FanOff;
				else if(0x02 == (nDiagInterface_DATA&0x03))
					nDiagInterface_DATA = *ThermalTalbe2CPUDiag[(nDiagInterface_DATA&0x1C)>>0x02].RPM;
		       }
			else  if(IS_MASK_SET(nDiagInterface_DATA,FanNum)&&IS_MASK_SET(nDiagInterface_DATA,FanType))  //Fan2 VGA
			{
			  	if(0x00 == (nDiagInterface_DATA&0x03))
			       	nDiagInterface_DATA = *ThermalTalbe2VGADiag[(nDiagInterface_DATA&0x1C)>>0x02].FanOn;
				else if(0x01 == (nDiagInterface_DATA&0x03))
					nDiagInterface_DATA = *ThermalTalbe2VGADiag[(nDiagInterface_DATA&0x1C)>>0x02].FanOff;
				else if(0x02 == (nDiagInterface_DATA&0x03))
					nDiagInterface_DATA = *ThermalTalbe2VGADiag[(nDiagInterface_DATA&0x1C)>>0x02].RPM;
		       }
			*/
	        break;	
		case DiagWriteThermalTable:					//0x2A Read Thermal Table
            /*
                	if(IS_MASK_CLEAR(nDiagInterface_DATA,FanNum)&&IS_MASK_CLEAR(nDiagInterface_DATA,FanType)) //Fan1 CPU
			{
				if(0x00 == (nDiagInterface_DATA&0x03))
			        	*ThermalTalbe1CPUDiag[(nDiagInterface_DATA&0x1C)>>0x02].FanOn = nDiagInterface_DATA_High;
				else if(0x01 == (nDiagInterface_DATA&0x03))
				  	*ThermalTalbe1CPUDiag[(nDiagInterface_DATA&0x1C)>>0x02].FanOff = nDiagInterface_DATA_High;
				else if(0x02 == (nDiagInterface_DATA&0x03))
					*ThermalTalbe1CPUDiag[(nDiagInterface_DATA&0x1C)>>0x02].RPM = nDiagInterface_DATA_High;
		      	} 
			else  if(IS_MASK_CLEAR(nDiagInterface_DATA,FanNum)&&IS_MASK_SET(nDiagInterface_DATA,FanType))  //Fan1 VGA
			{
			   	if(0x00 == (nDiagInterface_DATA&0x03))
			          	*ThermalTalbe1VGADiag[(nDiagInterface_DATA&0x1C)>>0x02].FanOn = nDiagInterface_DATA_High;
				else if(0x01 == (nDiagInterface_DATA&0x03))
					*ThermalTalbe1VGADiag[(nDiagInterface_DATA&0x1C)>>0x02].FanOff = nDiagInterface_DATA_High;
				else if(0x02 == (nDiagInterface_DATA&0x03))
					*ThermalTalbe1VGADiag[(nDiagInterface_DATA&0x1C)>>0x02].RPM = nDiagInterface_DATA_High;
		      } 
			else  if(IS_MASK_SET(nDiagInterface_DATA,FanNum)&&IS_MASK_CLEAR(nDiagInterface_DATA,FanType))  //Fan2 CPU
			{
				if(0x00 == (nDiagInterface_DATA&0x03))
			        	*ThermalTalbe2CPUDiag[(nDiagInterface_DATA&0x1C)>>0x02].FanOn = nDiagInterface_DATA_High;
				else if(0x01 == (nDiagInterface_DATA&0x03))
					*ThermalTalbe2CPUDiag[(nDiagInterface_DATA&0x1C)>>0x02].FanOff = nDiagInterface_DATA_High;
				else if(0x02 == (nDiagInterface_DATA&0x03))
					*ThermalTalbe2CPUDiag[(nDiagInterface_DATA&0x1C)>>0x02].RPM = nDiagInterface_DATA_High;
		       }
			else  if(IS_MASK_SET(nDiagInterface_DATA,FanNum)&&IS_MASK_SET(nDiagInterface_DATA,FanType))  //Fan2 VGA
			{
				if(0x00 == (nDiagInterface_DATA&0x03))
			      		*ThermalTalbe2VGADiag[(nDiagInterface_DATA&0x1C)>>0x02].FanOn = nDiagInterface_DATA_High;
				else if(0x01 == (nDiagInterface_DATA&0x03))
					*ThermalTalbe2VGADiag[(nDiagInterface_DATA&0x1C)>>0x02].FanOff = nDiagInterface_DATA_High;
				else if(0x02 == (nDiagInterface_DATA&0x03))
					*ThermalTalbe2VGADiag[(nDiagInterface_DATA&0x1C)>>0x02].RPM = nDiagInterface_DATA_High;
		       }
			*/
	        break;
        case DiagRecoverThermalTable:					//0x2B: real thermal table control
            //InitThermalTable1();
			//InitThermalTable2();
	        break;				 				 
	    case ClearAllDiagRam:				  //0xFF Clear All Diag RAM
		    ClearDiagRam();				 
            break;
		default:
			break;	
		}
  	}
	nDiagInterface_CMD = 0x00;
}
//ANGELAS064:add end
