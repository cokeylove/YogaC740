/*-----------------------------------------------------------------------------
 * TITLE: OEM_EDP.C
 *
 * Author : Parade Technologies inc.
 *
 * Note : These functions are reference code and LCFC setting code about PS8622.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>
/* I2C command format:[Slave Address,Offset,Value]
A. Write/Read Slave Address PS8622 Device Slave Address: 0x10~0x1F
	0x10/1x11:	Write/Read value to register at offset of Page0. 
	0x12/1x13:	Write/Read value to register at offset of Page1. 
	0x14/1x15:	Write/Read value to register at offset of Page2. 
	0x16/1x17:	Write/Read value to register at offset of Page3. 
	0x18/1x19:	Write/Read value to register at offset of Page4. 
	0x1A/1x1B:	Write/Read value to register at offset of Page5. 
	0x1C/1x1D:	Write/Read value to register at offset of Page6.
	0x1E/1x1F:	Write/Read value to register at offset of Page7. 
B. Offset:
	Range:0x00~0xFF
C. Value:
	Range:0x00~0xFF
*/	
// Sample code for initialing PS8622 - Compliant to PS8622 initial code Ver1.05
// Note: Pin I2C_CFG = M
// void IICWrite(byte I2C_address,byte reg_offset,byte data);

void IICWrite( BYTE I2C_address, BYTE Reg_offset, BYTE PS_data ,BYTE index)
{
	if(!bRWSMBus(SMbusCh4, SMbusWB, I2C_address, Reg_offset, &PS_data, SMBus_NoPEC))
	{
		TEMP_Error++;
	}
	else
	{
		initMark++;
		//TEMP_Error = 0x00;
		if( index == (PS8625_INITMAX-1) )
		{
			initMark = 0xAA;
		}
	}
}

const sPSXSmbusStruct code initPS8625Table[] = 
{	// 	address		offset		value.
	{ PS8625P2,		0xa1		,0x01}, 	// HPD = Low		
	{ PS8625P4,		0x14 		,0x01}, 			
	{ PS8625P4,		0xe3		,0x20}, 			
	{ PS8625P4,		0xe2 		,0x80}, 			
	{ PS8625P4,		0x8a		,0x0c}, 			
	{ PS8625P4,		0x89		,0x08}, 			
	{ PS8625P4,		0x71		,0x2d}, 			
	{ PS8625P4,		0x7d		,0x07}, 			
	{ PS8625P4,		0x7b 		,0x00}, 			
	{ PS8625P4,		0x7a 		,0xfd}, 			
	{ PS8625P4,		0xc0		,0x12}, 			
	{ PS8625P4,		0xc1		,0x92}, 			
	{ PS8625P4,		0xc2		,0x1c}, 			
	{ PS8625P4,		0x32		,0x80}, 			
	{ PS8625P4,		0x00		,0xb0}, 			
	{ PS8625P4,		0x15		,0x40}, 			
	{ PS8625P4,		0x54		,0x10}, 			
	{ PS8625P0,		0x52		,0x20}, 			
	{ PS8625P0,		0xf1		,0x03}, 			
	{ PS8625P0,		0x62		,0x41}, 			
	{ PS8625P0,		0xf6		,0x01},
	{ PS8625P0,		0x77		,0x06},
	{ PS8625P0,		0x4c		,0x04},
	{ PS8625P1,		0xc0		,0x00},
	{ PS8625P1,		0xc1		,0x1c},
	{ PS8625P1,		0xc2		,0xf8},
	{ PS8625P1,		0xc3		,0x44},
	{ PS8625P1,		0xc4		,0x32},
	{ PS8625P1,		0xc5		,0x44},
	{ PS8625P1,		0xc6		,0x4c},
	{ PS8625P1,		0xc7		,0x56},
	{ PS8625P1,		0xc8		,0x35},
	{ PS8625P1,		0xca		,0x01},
	{ PS8625P1,		0xcb		,0x04},
	{ PS8625P1,		0xce		,0xd9},//test
	{ PS8625P4,		0x59		,0x60},
	{ PS8625P4,		0x54		,0x14},
	{ PS8625P2,		0xa1		,0x91},		// HPD = High
};


void Init_PS8625(void)
{

	IICWrite(initPS8625Table[initMark].smbaddr,initPS8625Table[initMark].smboffset,initPS8625Table[initMark].smbvalue,initMark);
	
}
