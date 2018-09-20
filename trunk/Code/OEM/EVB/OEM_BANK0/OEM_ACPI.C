/*-----------------------------------------------------------------------------
 * TITLE: OEM_ACPI.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//----------------------------------------------------------------------------
// The hook function of 62 port data
//  Note :
//  PM1Step != 0x00 is necessary.
//----------------------------------------------------------------------------
#pragma ot(8, speed)    
void Hook_62Port(BYTE PM1Cmd)
{
	BYTE i;

	switch(PM1Cmd)
	{
	case 0x40:
		Cmd_40(PM1Data);
		break;
	case 0x41:
		if(PM1Data==0xA1) Cmd_41_A1(0x62);
		break;
	case 0x42:
		Cmd_42(PM1Data);
		break;
	case 0x43:
		Cmd_43(0x62,PM1Data);
		break;
	case 0x45:
		Cmd_45(0x62,PM1Data);
		break;
	case 0x46:
		Cmd_46(0x62,PM1Data); 
		break;
	case 0x47:
		Cmd_47(0x62,PM1Data);
		break;
	case 0x49:
		break;
	case 0x4B:
		if (PM1Step == 0x04)
		{
			i = PM1Data;
			break;
		}
		if (PM1Step == 0x03)
		{
			eEEPROMAddrsss = PM1Data;
			break;
		}
		if (PM1Step == 0x02)
		{
			eEEPROMBank = PM1Data;
			break;
		}
		if (PM1Step == 0x01)
		{
			eEEPROMData	= PM1Data;
            SET_MASK(LENOVOPMFW_Temp,EEPROM_Token);
			Update_EEPROMMark();
            CLEAR_MASK(LENOVOPMFW_Temp,EEPROM_Token);
			Data2PortDirect(0x62, 0x00);  
		}
		break;

	case 0x4C:
		if (PM1Step == 0x03)
		{
			i = PM1Data;
			break;
		}
		if (PM1Step == 0x02)
		{
			eEEPROMAddrsss = PM1Data;
			break;
		}
		if (PM1Step == 0x01)
		{
			eEEPROMBank = PM1Data;
			Cmd_4E(0x62,eEEPROMAddrsss);
		}
		break;

	case 0x4D:
		if (PM1Step == 0x02)
		{
			eEEPROMAddrsss = PM1Data;
			break;
		}
		if (PM1Step == 0x01)
		{
			eEEPROMData	= PM1Data;
            SET_MASK(LENOVOPMFW_Temp,EEPROM_Token);
			Update_EEPROMMark();
            CLEAR_MASK(LENOVOPMFW_Temp,EEPROM_Token);
			Data2PortDirect(0x62, 0x00);  
		}
		break;

	case 0x4E:
		eEEPROMAddrsss = PM1Data;
		Cmd_4E(0x62,eEEPROMAddrsss);
		break;

	case 0x51:
		break;

	case 0x52:
		if(PM1Data==0xA0) Cmd_52_A0(0x62);
		if(PM1Data==0xA3) Cmd_52_A3(0x62);
		if(PM1Data==0xA4) Cmd_52_A4(0x62);
		if(PM1Data==0xA5) Cmd_52_A5(0x62);
		if(PM1Data==0xA6) Cmd_52_A6(0x62); 
		if(PM1Data==0xA7) Cmd_52_A7(0x62); 
		break;

	case 0x53:
		if (PM1Step == 0x01)
		{
			Cmd_53(0x62,PM1Data2,PM1Data1,PM1Data);
		}
		break;
	case 0x56:
		break;
	case 0x58:
		Cmd_58(PM1Data);
		break;
	case 0x59:
		Cmd_59(0x62,PM1Data,0);
		break;
	case 0x5C:
		Cmd_5C(0x62);
		break;
	case 0x5D:
		Cmd_5D(PM1Data);
		break;
    case 0x71:
        Cmd_71(PM1Data);
		break;
	case 0x72:
		Cmd_72(PM1Data);//add flag for tool to keep the battery at 60%
		break;			
	case 0x7A:
		Cmd_7A(0x62,PM1Data);
		break;
	case 0x7B:
		if( PM1Step == 0x01 )
		{
			Cmd_7B(0x62,PM1Data1,PM1Data);;
		}
		break;			
	//Add 7Eh Command to access all EC RAM.
	case 0x7E:
		if( PM1Step == 0x01 )
		{
			Cmd_7E(0x62,PM1Data1,PM1Data);
		}
		break;
	case 0xB0:
		Cmd_B0(0x62,PM1Data);
		break;
	case 0xB3:
		if (PM1Step == 0x01)
		{
			Cmd_B3(0x62,PM1Data1,PM1Data);
		}
		break;
	}
}

//----------------------------------------------------------------------------
// The hook function of 66 port command.
//----------------------------------------------------------------------------
#pragma ot(8, speed)    
void Hook_66Port(BYTE PM1Cmd)
{
	switch(PM1Cmd)
	{
	case 0x40:
		PM1Step=0x01;
		break;
	case 0x41:
		PM1Step=0x01;
		break;
	case 0x42:
		PM1Step=0x01;
		break;
	case 0x43:
		PM1Step=0x01;
		break;
	case 0x44:
		Cmd_44(0x62);
		break;
	case 0x45:
		PM1Step=0x01;
		break;
	case 0x46:
		PM1Step=0x01;
		break;
	case 0x47:
		PM1Step=0x01;
		break;
	case 0x49:
		Cmd_49(0x62);
		break;
	case 0x4B:
		PM1Step=0x04;
		break;
	case 0x4C:
		PM1Step=0x03;
		break;
	case 0x4D:
		PM1Step=0x02;
		break;
	case 0x4E:
		PM1Step=0x01;
		break;

	case 0x4F:
		Erase_EEPROMAll();
		break;
//Add CMD 0X50 for osAging S4 keep EC power.
		case 0x50:
			SET_MASK(ACPI_HOTKEY, b6Cmd_NoShut);//for osAging s4 keep EC power
			break;
	// Add CMD 0X50 for osAging S4 keep EC power.
	case 0x51:
		Cmd_51(0x62);
		break;
	case 0x52:
		PM1Step=0x01;
		break;
	case 0x53:
		PM1Step=0x03; //change 2 to 3
		break;
	case 0x56:
		break;
	case 0x58:
		PM1Step=0x01;
		break;
	case 0x59:
		PM1Step=0x01;
		break;
	case 0x5C:
		break;
	case 0x5D:
		PM1Step=0x01;
		break;
	case 0x5E:			// UNLOCK SHIP MODE 
		#if shipmodesupport		//add ship mode judge
		Unlock_ShipMode();
		#endif
		break;
	case 0x5F:	
		#if shipmodesupport		//add ship mode judge//	LOCK SHIP MODE 
		ShipModeEn=0xA5;  
		Data2PortDirect(0x62, 0x5A);
        RamDebug(0xA5);
		#endif
		break;
    case 0x71:
		PM1Step=0x01; 
		break;
   	case 0x72:
		PM1Step=0x01;
		break;
	case 0x75:
		Cmd_75(); 
		break;
	case 0x7A:
		PM1Step=0x01;
		break;
	case 0x7B:
		PM1Step=0x02;
		break;	
	//Add workaround for hang bios.
	case 0x7C:
		Bioswatchdog=1;
		Bioswatchdogtime=0;//clear time.
		Data2PortDirect(0x62, 0x55);//add return date for bios.
		break;
	case 0x7D:
		Bioswatchdog=0;
		Data2PortDirect(0x62, 0x55);//add return date for bios.
		break;
	//Add 7Eh Command to access all EC RAM.
	case 0x7E:
		PM1Step=0x02;
		break;
	case 0xB0:
		PM1Step=0x01;
		break;
	//start for save P80 code to setup. 
	case 0xB1:
		Cmd_B1(0x62,0xB1);
		break;
	case 0xB2:
		Cmd_B2(0x62,0xB2);
		break;
	//end for save P80 code to setup. 
	case 0xB3:
		PM1Step=0x02;
		break;

	}
}


#if ACPI_Smbus
BYTE vOSSMbusWBlcok(void)
{
	 RamDebug(0xFA);
    if(SmBat_Addr == SMB_ADDR)
       return (bWSMBusBlock(SMbusChB, SMbusRBK, SMB_ADDR, SMB_CMD, &SMB_DATA, SMB_BCNT,SMBus_NeedPEC));
	#if SUPPORT_CYPRESS_PD_CCG4
    else if(Cypress_Addr == SMB_ADDR)
       return (bI2cMasterWrite(SMbusChA, SMB_ADDR, SMB_BCNT, SMB_CMD,(BYTE *)&SMB_DATA));//72JERRY003: Modify flash PD FW interface.//72JERRY022: Modify GPIO setting follow SIT.CHANGE B TO C
	#endif   
    else 
	   return (bWSMBusBlock(SMbusChB, SMbusRBK, SMB_ADDR, SMB_CMD, &SMB_DATA, SMB_BCNT,SMBus_NeedPEC));
	
}

BYTE vOSSMbusRBlcok(void)
{
    if(SmBat_Addr == SMB_ADDR)
	   return (bRSMBusBlock(SMbusChB, SMbusRBK, SMB_ADDR, SMB_CMD, &SMB_DATA));
	#if SUPPORT_CYPRESS_PD_CCG4
    if(Cypress_Addr == SMB_ADDR)
       return (bI2cMasterWriteToRead(SMbusChA, SMB_ADDR, SMB_CMD, SMB_DATA, SMB_BCNT, (BYTE *)&SMB_DATA1)); //72JERRY003: Modify flash PD FW interface. //72JERRY022
    #endif
	else 
		return (bRSMBusBlock(SMbusChB, SMbusRBK, SMB_ADDR, SMB_CMD, (BYTE *)&SMB_DATA));//72JERRY003: Modify flash PD FW interface.
	
}

BYTE vOSSMbusRByte(void)
{
	return (bRWSMBus(SMbusChB, SMbusRB, SMB_ADDR, SMB_CMD, &SMB_DATA, SMBus_NeedPEC));
}

BYTE vOSSMbusRWord(void)
{
	return (bRWSMBus(SMbusChB, SMbusRW, SMB_ADDR, SMB_CMD, &SMB_DATA, SMBus_NeedPEC));
}

BYTE vOSSMbusWBYTE(void)
{
	return (bRWSMBus(SMbusChB, SMbusWB, SMB_ADDR, SMB_CMD, &SMB_DATA, SMBus_NeedPEC));
}

BYTE vOSSMbusWWord(void)
{
	return (bRWSMBus(SMbusChB, SMbusWW, SMB_ADDR, SMB_CMD, &SMB_DATA, SMBus_NeedPEC));
}

void DummyFunction(void)
{
}

const FUNCT_PTR_B_V code vOSSMbusComd_table[16] =
{								//the low nibble of SMB_PRTC
	DummyFunction,		//0x00
	DummyFunction,		//0x01
	DummyFunction,		//0x02-Write Quick Command
	DummyFunction,		//0x03-Read Quick Command
	DummyFunction,		//0x04-Send Byte
	DummyFunction,		//0x05-Receive Byte
	vOSSMbusWBYTE,		//0x06-Write Byte
	vOSSMbusRByte,		//0x07-Read Byte
	vOSSMbusWWord,		//0x08-Write Word
	vOSSMbusRWord,		//0x09-Read Word
	vOSSMbusWBlcok,		//0x0A-Write Block
	vOSSMbusRBlcok,		//0x0B-Read Block
	DummyFunction,		//0x0C-Process Call
	DummyFunction,		//0x0D-Write Block-Read Block Process Call
	DummyFunction,		//0x0E
	DummyFunction		//0x0F
};

// ACPI response to writes to SMBus Protocol register.
void OSSMbusComd(void)
{
	if( SMB_PRTC != 0x00 )
	{
  		SMB_STS = 0x00;

      	if((vOSSMbusComd_table[(SMB_PRTC & 0x0F)])()) {
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
      	SMB_PRTC = 0x00;

  	}
}

#endif

//----------------------------------------------------------------------------
// Hook function of ACPI command
//----------------------------------------------------------------------------
void Hook_ACPICommand(void)
{
    #if ACPI_Smbus
    OSSMbusComd();		// ACPI SMBus command
    #endif
}

//-------------------------------------------------------------------------------
// parameter "QeventSCI" only, For a pulsed SCI
//------------------------------------------------------------------------------
void OEM_ACPI_Gen_Int(void)
{
    ACPI_Gen_Int(QeventSCI);
}

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
void ResetSCIEvent(void)
{

    CLEAR_MASK(PM1STS,SCIEVT);
    ECCheckBurstMode = 0;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: ECQEvent -Setup cause flag for an SCI and start the ACPI_Timer.
 *
 * sci_number = 1 through 255.  The Host uses this number to determine the
 * cause of the SCI.
 * ------------------------------------------------------------------------- */
void ECQEvent(BYTE sci_number)
{
	if(IS_MASK_CLEAR(SYS_MISC1,ACPI_OS) || (SysPowState!=SYSTEM_S0))
	{
		return;
	}

	ITempB05 = SCI_Event_In_Index; 			// Get the input index.
	SCI_Event_Buffer[ITempB05] = sci_number; // Put the SCI number in the buffer.
 	ITempB05++;    							// Increment the index.
 	if (ITempB05 >= EVENT_BUFFER_SIZE)
	{
     	ITempB05 = 0;
 	}
 											// If the buffer is not full, update the input index.
  	if (ITempB05 != SCI_Event_Out_Index)
	{
   		SCI_Event_In_Index = ITempB05;
    }
	SET_MASK(PM1STS,SCIEVT);
    OEM_ACPI_Gen_Int();
}

//Hang check
void ECSMIEvent(void)
{
	 if(SysPowState!=SYSTEM_S0)
    {
    	return;
    }
    SET_MASK(PM1STS,SMIEVT);
	DisableAllInterrupt();
    Hook_SMIOFF();
	Hook_SMION();
    /* Delay 15.26 us */
    WNCKR = 0x00;
	Hook_SMIOFF();
    EnableAllInterrupt();
}
//	Hang check
//Enable SMI for hang debug.
void ECSMI_SCIEvent(BYTE smi_number)
{
		ECQEvent(smi_number);
}

//----------------------------------------------------------------------------
// Read EC ram Space
//----------------------------------------------------------------------------
BYTE Hook_ReadMapECSpace(BYTE MapIndex)
{
	return( *((XBYTE *)(ECRAM|MapIndex)) );
}

void SetECSpaceAsRO(BYTE *formerByte,BYTE *writeByte,BYTE bitMask)
{   
    BYTE index = 0x00;
	BYTE maskTemp = 0x00;
	BYTE indexTemp = 0x01;
	for(index=0x00;index<=0x07;index++)
	{
	    indexTemp = 0x01 << index;
	    maskTemp = bitMask & indexTemp;
	    if(maskTemp)
	    {
	        if(IS_MASK_SET(*formerByte,maskTemp))
			  SET_MASK(*writeByte,maskTemp);
		   else
			  CLEAR_MASK(*writeByte,maskTemp);
	    }
	}
}


//----------------------------------------------------------------------------
// Write EC ram Space
//----------------------------------------------------------------------------
void Hook_WriteMapECSpace(BYTE MapIndex, BYTE data1)
{
	BYTE *formerdate_XPntr;
	BYTE *writedate_XPntr;
	BYTE *Tmp_XPntr;
	Tmp_XPntr= (XBYTE *)(ECRAM|MapIndex);
    if(MapIndex == 0xA1)
	{
	   RamDebug(0xA1);
	   RamDebug(data1);
    }
    if(MapIndex == 0x43)
	{
	    if((IS_MASK_SET(data1,b5TPDRIVER_STATUS))&&(IS_MASK_CLEAR(pDevStatus1,b5TPDRIVER_STATUS)))
		{
		  RamDebug(0xD2);
		  RamDebug(data1);
		  RamDebug(pDevStatus1);
		  if(IS_MASK_SET(pDevStatus1,b4LNVAPP_STATUS))  
		  {
			 if(IS_MASK_SET(pDevStatus1,b6MODE_STATUS))
			 {
			    ECQEvent(0x3F);
			 }
			 else
			 {
			    ECQEvent(0x3E);
			 }
		  }
		} 
		formerdate_XPntr = &pDevStatus1;
		writedate_XPntr  = &data1;
		SetECSpaceAsRO(formerdate_XPntr,writedate_XPntr,b0F6ScanCodeSend+b1F6DisableTP+b2DisableKB+b3Scancode_STATUS+b4LNVAPP_STATUS+b6MODE_STATUS); 	  
    }	
	*Tmp_XPntr= data1;
	VPC_interface();
	ThermalMailBox();
}


void Data_To_PM1_nWait(BYTE data_byte)
{
    Data_To_PM1(data_byte);

    TR1 = 0;                    // Disable timer1
    ET1 = 0;                  	// Disable timer1 interrupt
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    TH1 = Timer_85ms>>8;        // Set timer1 counter 26ms
    TL1 = Timer_85ms;           // Set timer1 counter 26ms
    TF1 = 0;                  	// Clear overflow flag
    TR1 = 1;                 	// Enable timer1

	while (!TF1)
	{
	    if(IS_MASK_CLEAR(PM1STS, OBF))
        {
            break;
        }
//open below 4 line        
        if(IS_MASK_SET(PM1STS, IBF))
        {
            break;
        }
	}

    TR1 = 0;			// disable timer1
 	TF1 = 0;			// clear overflow flag
	ET1 = 1;			// Enable timer1 interrupt
}


void Data2PortDirect(BYTE nPort, BYTE rData)
{
	switch(nPort) 
	{
	case 0x60:
		Data_To_Port(rData); 
		break;
	case 0x62:
		Data_To_PM1(rData);
		break;
	//Add 68/6C interface.
	case 0x68:
		Data_To_PM2(rData);
		break;			
	case 0xFE://Add case for Mailbox
		MMIO_rDATA[MMIO_rDATA_index] = rData;
		MMIO_rDATA_index++;
		break;
	}
}


//Add 68/6C interface.
void Data_To_PM2_nWait(BYTE data_byte)
{
    Data_To_PM2(data_byte);

    TR1 = 0;                    // Disable timer1
    ET1 = 0;                  	// Disable timer1 interrupt
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    TH1 = Timer_85ms>>8;        // Set timer1 counter 26ms
    TL1 = Timer_85ms;           // Set timer1 counter 26ms
    TF1 = 0;                  	// Clear overflow flag
    TR1 = 1;                 	// Enable timer1

	while (!TF1)
	{
	    if(IS_MASK_CLEAR(PM2STS, OBF))
        {
            break;
        }       
        if(IS_MASK_SET(PM2STS, IBF))
        {
            break;
        }
	}

    TR1 = 0;			// disable timer1
 	TF1 = 0;			// clear overflow flag
	ET1 = 1;			// Enable timer1 interrupt
}



void Data2Port(BYTE nPort, BYTE rData)
{
	switch(nPort) 
	{
	case 0x60:
		Data_To_Port_nWait(rData); 
		break;
	case 0x62:
		Data_To_PM1_nWait(rData);
		break;
	//68/6C interface.
	case 0x68:
		Data_To_PM2_nWait(rData);
		break;
	case 0xFE://Add case for Mailbox
		MMIO_rDATA[MMIO_rDATA_index] = rData;
		MMIO_rDATA_index++;
		break;
	}
}

void MultiB2Port(BYTE nPort, BYTE rData)
{
	switch(nPort) 
	{
	case 0x60:
		Data_To_Port_nWait(rData); 
		break;
	case 0x62:
		Data_To_PM1_nWait(rData);
		break;
	//Add 68/6C interface.
	case 0x68:
		Data_To_PM2_nWait(rData);
		break;			
	case 0xFE://Add case for Mailbox
		MMIO_rDATA[MMIO_rDATA_index] = rData;
		MMIO_rDATA_index++;
		break;
	}
}
