/*----------------------------------------------------------------------------
 * Filename: OEM_PM2.C  For Chipset: ITE.IT85XX
 *
 * Copyright (c) 2010 - , ITE Tech. Inc. All Rights Reserved. 
 *
 * You may not present,reproduce,distribute,publish,display,modify,adapt,
 * perform,transmit,broadcast,recite,release,license or otherwise exploit
 * any part of this publication in any form,by any means,without the prior
 * written permission of ITE Tech. Inc.	
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//ANGELAS064:add start
//----------------------------------------------------------------------------
// Read EC ram Space
//----------------------------------------------------------------------------
//72JERRY003:s+ Modify flash PD FW interface.
BYTE Hook_ReadECSpace(BYTE MapIndex)
{
	BYTE  Temp;
	Temp= *((XBYTE *)(NameSpace|MapIndex));
	return(Temp);	
}

void Hook_WriteECSpace(BYTE MapIndex, BYTE data1)
{
	*((XBYTE *)(NameSpace|MapIndex)) = data1;  
}

//----------------------------------------------------------------------------
// Write EC ram Space
//----------------------------------------------------------------------------
void Hook_WriteLCFCECSpace(BYTE MapIndex, BYTE data1)
{
	*((XBYTE *)(DiagECRAM|MapIndex)) = data1;
	Diag_interface();  
}
BYTE Hook_ReadLCFCECSpace(BYTE MapIndex)
{
	BYTE  Temp;
	Temp= *((XBYTE *)(DiagECRAM|MapIndex));
	return(Temp);	
}
void PM2_Cmd_80Data(void)    

{
	if (PM2DataCount == 1)
	{
		Data2Port(0x68,Hook_ReadECSpace(PM2Data));
	}
}
void PM2_Cmd_81Data(void)  
{
  	if (PM2DataCount == 2)
	{
     	PM2Data1=PM2Data;  		
	}
	else if(PM2DataCount == 1)
	{
		Hook_WriteECSpace(PM2Data1,PM2Data);
		Hook_ACPICommand(); 
 	}
}
//72JERRY003: e+Modify flash PD FW interface.
//----------------------------------------------------------------------------
// Handle the data of ACPI command 8x
//----------------------------------------------------------------------------
void PM2_Cmd_90Data(void)    

{
	if (PM2DataCount == 1)
	{
		Data2Port(0x68,Hook_ReadLCFCECSpace(PM2Data));
	}
}
void PM2_Cmd_91Data(void)  
{
  	if (PM2DataCount == 2)
	{
     	PM2Data1=PM2Data;  		
	}
	else if(PM2DataCount == 1)
	{
		Hook_WriteLCFCECSpace(PM2Data1,PM2Data);
		Hook_ACPICommand(); 
 	}
}
//ANGELAS064:add end

//----------------------------------------------------------------------------
// The function of debuging 68 port
//----------------------------------------------------------------------------
void Hook_68Port(BYTE PM2Cmd)
{ //G686C add below info G59:Add 68/6C interface.
    BYTE i;
	
	switch(PM2Cmd)
	{
	case 0x40:
		Cmd_40(PM2Data);
		break;
	case 0x41:
		if(PM2Data==0xA1) 
			Cmd_41_A1(0x68);
		break;
	case 0x42:
		Cmd_42(PM2Data);
		break;
	case 0x43:
		Cmd_43(0x68,PM2Data);
		break;
	case 0x45:
		Cmd_45(0x68,PM2Data);
		break;
	case 0x46:
		Cmd_46(0x68,PM2Data);
		break;
	case 0x47:
		Cmd_47(0x68,PM2Data);
		break;
	case 0x49:
		break;
	//MEILING009:S+ Add 8 sec reset test function.
	case 0x4A:
		Cmd_4A(0x68,PM2Data);
		break;
	//MEILING009:E+.
	case 0x4B:
		if (PM2DataCount == 0x04)
		{
			i = PM2Data;
			break;
		}
		if (PM2DataCount == 0x03)
		{
			eEEPROMAddrsss = PM2Data;
			break;
		}
		if (PM2DataCount == 0x02)
		{
			eEEPROMBank = PM2Data;
			break;
		}
		if (PM2DataCount == 0x01)
		{
			eEEPROMData = PM2Data;
			SET_MASK(LENOVOPMFW_Temp,EEPROM_Token);
			Update_EEPROMMark();
			CLEAR_MASK(LENOVOPMFW_Temp,EEPROM_Token);
			Data2PortDirect(0x68, 0x00);  
		}
		break;
	
	case 0x4C:
		if (PM2DataCount == 0x03)
		{
			i = PM2Data;
			break;
		}
		if (PM2DataCount == 0x02)
		{
			eEEPROMAddrsss = PM2Data;
			break;
		}
		if (PM2DataCount == 0x01)
		{
			eEEPROMBank = PM2Data;
			Cmd_4E(0x68,eEEPROMAddrsss);
		}
		break;
	
	case 0x4D:
		if (PM2DataCount == 0x02)
		{
			eEEPROMAddrsss = PM2Data;
			break;
		}
		if (PM2DataCount == 0x01)
		{
			eEEPROMData = PM2Data;
			SET_MASK(LENOVOPMFW_Temp,EEPROM_Token);
			Update_EEPROMMark();
			CLEAR_MASK(LENOVOPMFW_Temp,EEPROM_Token);
			Data2PortDirect(0x68, 0x00);  
		}
		break;
	
	case 0x4E:
		eEEPROMAddrsss = PM2Data;
		Cmd_4E(0x68,eEEPROMAddrsss);
		break;
	
	case 0x51:
		break;
	
	case 0x52:
		if(PM2Data==0xA0) Cmd_52_A0(0x68);
		if(PM2Data==0xA3) Cmd_52_A3(0x68);
		if(PM2Data==0xA4) Cmd_52_A4(0x68);
		if(PM2Data==0xA5) Cmd_52_A5(0x68);
		if(PM2Data==0xA6) Cmd_52_A6(0x68); //LMLKBL0018:add.
		if(PM2Data==0xA7) Cmd_52_A7(0x68); //LMLKBL0018:add.
		break;
	
	case 0x53:
		if (PM2DataCount == 0x01)
		{
			Cmd_53(0x68,PM2Data2,PM2Data1,PM2Data);
		}
		break;
	case 0x55:
		if (PM2DataCount == 0x01)  //Hang check
		{
			Cmd_55(PM2Data);
		}
		break;
	case 0x56:
		break;
	case 0x58:
		Cmd_58(PM2Data);
		break;
	case 0x59:
		Cmd_59(0x68,PM2Data,0);
		break;
	case 0x5C:
		Cmd_5C(0x68);
		break;
	case 0x5D:
		Cmd_5D(PM2Data);
		break;
    case 0x71:
		//LMLKBL0001:modify cmd 0x71 have one data.
        /*if (PM2DataCount == 0x01)
        	{
			Cmd_71(PM2Data1,PM2Data);//:Add 60/64 71cmd follow new EC common SPEC. :Add 62/66 71cmd
        	}*/
        Cmd_71(PM2Data);
        //LMLKBL0001:modify cmd 0x71 have one data.
		break;
	case 0x72:
		Cmd_72(PM2Data);//:add flag for  tool to keep the battery at 60% //:Add 62/66 72cmd.
		break;
	//ANGELAS033:Add start			
	case 0x7A:
		Cmd_7A(0x68,PM2Data);
		break;
	case 0x7B:
		if( PM2DataCount == 0x01 )
		{
			Cmd_7B(0x68,PM2Data1,PM2Data);;
		}
		break;			
	//ANGELAS033:Add end

	//MEILING031: add start
	case 0x7E:
		if (PM2DataCount == 0x01)
        {
            Cmd_7E(0x68,PM2Data1,PM2Data);
		}
		break;
	//MEILING031: add end
	//72JERRY003:s+ Modify flash PD FW interface.
	case 0x80:
		PM2_Cmd_80Data();
		break;					
	case 0x81:
		PM2_Cmd_81Data();
		break;
		//72JERRY003:e+ Modify flash PD FW interface.
	//ANGELAS064:Add start	
	case 0x90:
		PM2_Cmd_90Data();
		break;					
	case 0x91:
		PM2_Cmd_91Data();
		break;
	//ANGELAS064:Add end
			
	case 0xB0:
		Cmd_B0(0x68,PM2Data);
		break;
	case 0xB3:
		if (PM2DataCount == 0x01)
		{
			Cmd_B3(0x68,PM2Data1,PM2Data);
		}
		break;
	}
}


//----------------------------------------------------------------------------
// The function of debuging 6C port
//----------------------------------------------------------------------------
void Hook_6CPort(BYTE PM2Cmd)
{
    //G686C add below info :Add 68/6C interface.
	switch(PM2Cmd)
	{
	case 0x40:
		PM2DataCount=0x01;
		break;
	case 0x41:
		PM2DataCount=0x01;
		break;
	case 0x42:
		PM2DataCount=0x01;
		break;
	case 0x43:
		PM2DataCount=0x01;
		break;
	case 0x44:
		Cmd_44(0x68);
		break;
	case 0x45:
		PM2DataCount=0x01;
		break;
	case 0x46:
		PM2DataCount=0x01;
		break;
	case 0x47:
		PM2DataCount=0x01;
		break;
	case 0x49:
		Cmd_49(0x68);
		break;
	//MEILING009:S+ Add 8 sec reset test function.
	case 0x4A:
		PM2DataCount=0x01; 
		break;
	//MEILING009:E+.
	case 0x4B:
		PM2DataCount=0x04;
		break;
	case 0x4C:
		PM2DataCount=0x03;
		break;
	case 0x4D:
		PM2DataCount=0x02;
		break;
	case 0x4E:
		PM2DataCount=0x01;
		break;
	case 0x4F:
		Erase_EEPROMAll();
		break;
	//REJERRY093: Add CMD 0X50 for osAging S4 keep EC power.
		case 0x50:
			SET_MASK(ACPI_HOTKEY, b6Cmd_NoShut);//for osAging s4 keep EC power
			break;
	//REJERRY093: Add CMD 0X50 for osAging S4 keep EC power.
	case 0x51:
		Cmd_51(0x68);
		break;
	case 0x52:
		PM2DataCount=0x01;
		break;
	case 0x53:
		PM2DataCount=0x03;
		break;
	case 0x55://hang check
		PM2DataCount=0x01;
		break;
	case 0x56:
		break;
	case 0x58:
		PM2DataCount=0x01;
		break;
	case 0x59:
		PM2DataCount=0x01;
		break;
	case 0x5C:
		break;
	case 0x5D:
		PM2DataCount=0x01;
		break;
	//ANGELAS032: s+ add
	case 0x5E:			// UNLOCK SHIP MODE 
	#if shipmodesupport		//add ship mode judge
		Unlock_ShipMode();
	#endif
		break;
	case 0x5F:	
		#if shipmodesupport		//add ship mode judge//	LOCK SHIP MODE 
		ShipModeEn=0xA5;  
		Data2PortDirect(0x68, 0x5A);
        RamDebug(0xA5);
		#endif
		break;
	//ANGELAS032: e+ add
    case 0x71:
		PM2DataCount=0x02;//:Add 62/66 71cmd
		break;
    case 0x72:
		PM2DataCount=0x01;//:Add 62/66 72cmd.
		break;
	//ANGELAG012:S+.
	case 0x75:
		Cmd_75();
		break;
	//ANGELAG012:E+.
	//ANGELAS033:Add start
	case 0x7A:
		PM2DataCount=0x01;
		break;
	case 0x7B:
		PM2DataCount=0x02;
		break;
	//ANGELAS033:Add end
	//ANGELAS094:s+Add workaround for hang bios.
	case 0x7C:
		Bioswatchdog=1;
		Bioswatchdogtime=0;//ANGELAS106:Optimize ANGELAS094 clear time.
		Data2PortDirect(0x68, 0x55);//ANGELAS105:Optimize ANGELAS094 add return date for bios.
		break;
	case 0x7D:
		Bioswatchdog=0;
		Data2PortDirect(0x68, 0x55);//ANGELAS105:Optimize ANGELAS094 add return date for bios.
		break;
	//ANGELAS094:e+Add workaround for hang bios.

	//MEILING031: add start
    case 0x7E:
		PM2DataCount=0x02;
		break;
   	//MEILING031: add end
	//72JERRY003: s+Modify flash PD FW interface.
         case 0x80:	 
		PM2DataCount=0x01;
		break;
	case 0x81:	  
		PM2DataCount=0x02;
		break;	
		//72JERRY003: e+Modify flash PD FW interface.
	//ANGELAS064:add start
	case 0x90:	 
		PM2DataCount=0x01;
		break;
	case 0x91:	  
		PM2DataCount=0x02;
		break;				
	//ANGELAS064:add end
	case 0xB0:
		PM2DataCount=0x01;
		break;
	//: start for save P80 code to setup. 
	case 0xB1:
		Cmd_B1(0x68,0xB1);
		break;
	case 0xB2:
		Cmd_B2(0x68,0xB2);
		break;
	//: end for save P80 code to setup. 
	case 0xB3:
		PM2DataCount=0x02;
		break;
	//ANGELAS002:S+Support 686C flash EC in windows or Shell
	case 0xDC:
        PM2_6CCmd_DC();//BROOKEW008:support 686C flash EC.
        break;
	//ANGELAS002:E+Support 686C flash EC in windows or Shell
	}
}

//----------------------------------------------------------------------------
// Process Command/Data received from System via 68/6C interface
//----------------------------------------------------------------------------
void service_pci3(void)
{
	//G686C add info G59:Add 68/6C interface.
    if ( IS_MASK_CLEAR(PM2STS,P_IBF) )	
    {
	    return; 
    }
	if ( PM2STS & P_C_D)	// Command port
	{
        PM2DataCount =0;
   		PM2Cmd  = PM2DI;	// Load command 
		EnablePMCIBFInt();
		Hook_6CPort(PM2Cmd);    // Handle command		
	}
	else					// Data port
	{
	    PM2Data2 = PM2Data1;
 	    PM2Data1 = PM2Data;		
        PM2Data = PM2DI;
        EnablePMCIBFInt();
        if (PM2DataCount!=0x00) 
        {
		    Hook_68Port(PM2Cmd);   // Handle command data
		    PM2DataCount--;
	    }
	}

}

