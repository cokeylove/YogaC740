/*----------------------------------------------------------------------------
 * TITLE: CORE_ACPI.C - ACPI Handler
 *
 * SCI event generation.
 * ACPI SMBus transfer initiated by ACPI command.
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

//----------------------------------------------------------------------------
// Process Command/Data received from System via the 2nd Host interface
//----------------------------------------------------------------------------
void service_pci2(void)
{
    if ( IS_MASK_CLEAR(PM1STS,P_IBF) )
    {
		return;
    }
	SetTotalBurstTime();	// Set Burst mode total time (2ms)
 	service_pci2f();		// Process Command/Data
}

//----------------------------------------------------------------------------
// Process Command/Data received from System via the 2nd Host interface
//----------------------------------------------------------------------------
void service_pci2f(void)
{
	do
	{
		if ( PM1STS & P_C_D )			// Command port
 		{
   			PM1Cmd  = PM1DI;			// Load command
        	PM1Step =0x00;
        	if (PM1Cmd==0x80) ACPI_Cmd_80();			
			else if (PM1Cmd==0x81) ACPI_Cmd_81();	
			else if (PM1Cmd==0x82) ACPI_Cmd_82();	
			else if (PM1Cmd==0x83) ACPI_Cmd_83();	
			else
		    {
				Core_66Port(PM1Cmd);
           		Hook_66Port(PM1Cmd);
 	        }
  		}
  		else							// Data port
 		{
 			PM1Data2 = PM1Data1;
 			PM1Data1 = PM1Data;
  			PM1Data = PM1DI;			// Load data
			if (PM1Step!=0x00)
			{			
				if (PM1Cmd==0x80) ACPI_Cmd_80Data();			
				else if (PM1Cmd==0x81) ACPI_Cmd_81Data();
				else
				{					
					Core_62Port(PM1Cmd);    // Handle command data
				    Hook_62Port(PM1Cmd);
                } 
				PM1Step--;
			}
  		}
	}while((IS_MASK_SET(PM1STS,BURST)||ECCheckBurstMode)&&(CheckBurstMode()==1));

	TR1 = 0;			// disable timer1
 	TF1 = 0;			// clear overflow flag
	ET1 = 1;			// Enable timer1 interrupt
	EnableAllInterrupt();
	EnablePMCIBFInt();
	Hook_ACPICommand();
}

//----------------------------------------------------------------------------
// Handle ACPI commands.
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// Handle ACPI command 8x.
//----------------------------------------------------------------------------
void ACPI_Cmd_80(void)	// Read Embedded Controller.
{
	ACPI_Gen_Int(ACPICmdSCI);
	PM1Step =0x01;
}

void ACPI_Cmd_81(void)	// Write Embedded Controller.
{
 	ACPI_Gen_Int(ACPICmdSCI);
  	PM1Step =0x02;
}

void ACPI_Cmd_82(void)	// Burst Enable Embedded Controller.
{
    SET_MASK(SYS_MISC1,ACPI_OS);
	Bioswatchdog=0;
	Bioswatchdogtime=0;
	SET_MASK(PM1STS,BURST); // PM1STS.4 Set Burst mode flag
 	SendFromAcpi(0x90);		// Get Burst Acknowledge byte to send to Host.
}

void ACPI_Cmd_83(void)	// Burst Disable Embedded Controller.
{
	CLEAR_MASK(PM1STS,BURST); 	// PM1STS.4 Set Burst mode flag
 	ACPI_Gen_Int(ACPICmdSCI);   // Clear Burst bit in secondary Host interface status register.
}

void ACPI_Cmd_84(void)	// Query Embedded Controller.
{
	ITempB05 = 0x00;
	if (SCI_Event_Out_Index != SCI_Event_In_Index)
	{
		ITempB05 = SCI_Event_Buffer[SCI_Event_Out_Index];
    	SCI_Event_Out_Index++;
    	if (SCI_Event_Out_Index >= EVENT_BUFFER_SIZE)
		{
           	SCI_Event_Out_Index = 0;
   		}
   		if (SCI_Event_Out_Index == SCI_Event_In_Index)
         	CLEAR_MASK(PM1STS,SCIEVT);

		SendFromAcpi(ITempB05);
	}
	else
	{
		CLEAR_MASK(PM1STS,SCIEVT);
	}

}

//----------------------------------------------------------------------------
// Handle ACPI command 9x.
//----------------------------------------------------------------------------
void ACPI_Cmd_92(void)		// Oem function of reading EC external ram
{
	PM1Step = 0x02;
}

void ACPI_Cmd_93(void)		// Oem function of writing EC external ram
{
	PM1Step =0x03;
}

//----------------------------------------------------------------------------
// Handle ACPI command Dx.
//----------------------------------------------------------------------------
void ACPI_Cmd_DC(void)
{
    #if WDT_Support
    DisableInternalWDT();  //G64:Add disable watch dog when flash EC with CMD 0xDC.
    #endif
	ITE_Flash_Utility();    // for 8500
}

//----------------------------------------------------------------------------
// Handle ACPI command Fx.
//----------------------------------------------------------------------------
void ACPI_Cmd_F0(void)
{
	KeyScanACK = KU_ACK_Start;
}

//----------------------------------------------------------------------------
// Handle ACPI command data.
//----------------------------------------------------------------------------
void SendFromAcpi(BYTE ecdata)
{
	PM1DO = ecdata;
 	ACPI_Gen_Int(ACPICmdSCI);
}

//----------------------------------------------------------------------------
// Handle the data of ACPI command 8x
//----------------------------------------------------------------------------
void ACPI_Cmd_80Data(void)
{
	if (PM1Step == 1)
	{
		SendFromAcpi(Hook_ReadMapECSpace(PM1Data));
        ECCheckBurstMode = 0;
	}
}

void ACPI_Cmd_81Data(void)
{
 	ACPI_Gen_Int(ACPICmdSCI);
  	if (PM1Step == 2)
	{
     	PM1Data1=PM1Data;
	}
	else if(PM1Step == 1)
	{
		Hook_WriteMapECSpace(PM1Data1,PM1Data);
        ECCheckBurstMode = 0;
 	}
}

void ACPI_Cmd_82Data(void)
{

}

void ACPI_Cmd_83Data(void)
{

}

void ACPI_Cmd_84Data(void)
{

}

//----------------------------------------------------------------------------
// Handle the data of ACPI command 9x
//----------------------------------------------------------------------------
void ACPI_Cmd_92Data(void)
{
	switch(PM1Step)
	{
		case 2:
			PM1Data1=PM1Data;  		// address high
			break;

		case 1:
			Read_External_Ram();
			break;

		default:
			break;
	}
}

void ACPI_Cmd_93Data(void)
{
	if (PM1Step==0x03)
	{
   		PM1Data2=PM1Data;  // address high
	}
	else if (PM1Step==0x02)
	{
      	PM1Data1=PM1Data;  // address low
	}
	else if (PM1Step==0x01)
	{
    	Write_External_Ram();
	}
}

//----------------------------------------------------------------------------
// Read external ram Space
//----------------------------------------------------------------------------
void Read_External_Ram()
{
  	Tmp_XPntr=((PM1Data1<<8)+PM1Data);
	PM1DO = *Tmp_XPntr;
}

//----------------------------------------------------------------------------
// Write external ram Space
//----------------------------------------------------------------------------
void Write_External_Ram()
{
 	Tmp_XPntr=((PM1Data2<<8)+PM1Data1);
  	*Tmp_XPntr=PM1Data;
}

//----------------------------------------------------------------------------
// Burst mode or normal mode
//----------------------------------------------------------------------------
BYTE CheckBurstMode(void)
{
	while(IS_MASK_CLEAR(PM1STS,P_IBF))  // wait input buffer full
	{
		if(TF1)						    // time-out
		{
            TR1 = 0;
            TF1=0;
		    CLEAR_MASK(PM1STS,BURST);
	        ACPI_Gen_Int(ACPICmdSCI);
            ECCheckBurstMode = 0;
			return(0);
		}
	}
	return(1);
}

//----------------------------------------------------------------------------
// Set Burst mode total time (1ms)
//----------------------------------------------------------------------------
void SetTotalBurstTime(void)
{
    DisableAllInterrupt();
	TR1 = 0;						// disable timer1
	ET1 = 0;						// Disable timer1 interrupt
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    TH1 = T_ACPI_BurstTime>>8;      // Set timer1 counter 2ms
    TL1 = T_ACPI_BurstTime;         // Set timer1 counter 2ms
    TF1 = 0;						// clear overflow flag
	TR1 = 1;						// enable timer1
}

/* ----------------------------------------------------------------------------
 * FUNCTION: ACPI_Gen_Int - Generate an SCI interrupt.
 *
 * For a pulsed SCI (edge triggered), the SCI signal is pulsed.
 *
 * For a level triggered SCI, the SCI signal is set or cleared.  The Host will
 * send a command to read the cause of the SCI. The signal will be deactivated
 * when no more causes exist.
 * ------------------------------------------------------------------------- */
void ACPI_Gen_Int(BYTE Qevent)
{
//ANGELAS024:s+Modify ACPI function.
	if(IS_MASK_CLEAR(SYS_MISC1,ACPI_OS) || (SysPowState!=SYSTEM_S0))
	{
		return;
	}
//ANGELAS024:e+Modify ACPI function.
    if(Qevent==QeventSCI)
    {
        DisableAllInterrupt();
    }
	Hook_SCION();
 	WNCKR = 0x00;               // Delay 15.26 us   T066A-//72JERRY079:Add delay time for SCI interrupt.
 	WNCKR = 0x00;               // Delay 15.26 us   T066A-//72JERRY079:Add delay time for SCI interrupt.
 	//WNCKR = 0x00;               // Delay 15.26 us   T066A-
	Hook_SCIOFF();
    if(Qevent==QeventSCI)
    {
        EnableAllInterrupt();
    }
}

//----------------------------------------------------------------------------
// The function of processing 62 port data
//----------------------------------------------------------------------------
#pragma ot(8, speed)    //T084+
void Core_62Port(BYTE PM1Data)
{
    switch(PM1Data)
    {
	/* 
       case 0x80:
        	ACPI_Cmd_80Data();
	    	break;

	case 0x81:
        	ACPI_Cmd_81Data();
	    	break;

      case 0x82:
        	ACPI_Cmd_82Data();
	    	break;

	case 0x83:
        	ACPI_Cmd_83Data();
	    	break;
	*/  
    case 0x84:
     	ACPI_Cmd_84Data();
	 	break;

    case 0x92:
        ACPI_Cmd_92Data();
        break;

    case 0x93:
        ACPI_Cmd_93Data();
        break;

	default:
	    break;
    }
}

//----------------------------------------------------------------------------
// The function of processing 66 port command.
//----------------------------------------------------------------------------
#pragma ot(8, speed)    //T084+
void Core_66Port(BYTE PM1Cmd)
{
    switch(PM1Cmd)
    {
	/*   //  
       case 0x80:
        	ACPI_Cmd_80();
	   	 break;

	case 0x81:
        	ACPI_Cmd_81();
	    	break;

       case 0x82:
        	ACPI_Cmd_82();
	    	break;

	case 0x83:
        	ACPI_Cmd_83();
	    	break;
	*/   //  
    case 0x84:
        ACPI_Cmd_84();
	    break;

    case 0x92:
        ACPI_Cmd_92();
        break;

    case 0x93:
        ACPI_Cmd_93();
        break;

    case 0xDC:
        ACPI_Cmd_DC();
        break;

    case 0xF0:
        ACPI_Cmd_F0();
        break;

	default:
	    break;
    }
}
