/*-----------------------------------------------------------------------------
 * TITLE: OEM_HOSTIF.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//-----------------------------------------------------------------------------
// put data to PM1 port
//-----------------------------------------------------------------------------
void Data_To_PM1(BYTE data_byte)
{
	PM1DO = data_byte;
}

//-----------------------------------------------------------------------------
// put PM1 port data to buffer
//-----------------------------------------------------------------------------
void PM1_DataPending(BYTE nPending)
{
    if( PM1PendingRXCount > 6 )  return;

    PM1DataPending[PM1PendingRXCount] = nPending;
    PM1PendingRXCount++;
    SetServiceSendFlag();
}

//-----------------------------------------------------------------------------
// get PM1 port data from buffer
//-----------------------------------------------------------------------------
BYTE GetPM1_PendingData(void)
{
	ITempB03 = PM1DataPending[(PM1PendingTXCount&0x07)];
	PM1PendingTXCount++;
	if( PM1PendingTXCount >= PM1PendingRXCount ) {
		PM1PendingTXCount = 0;
		PM1PendingRXCount = 0;
	}
	return ITempB03;
}

//-----------------------------------------------------------------------------
// put data to PM2 port
//-----------------------------------------------------------------------------
void Data_To_PM2(BYTE data_byte)
{
	PM2DO = data_byte;
}

//-----------------------------------------------------------------------------
// put PM2 port data to buffer
//-----------------------------------------------------------------------------
void PM2_DataPending(BYTE nPending)
{
    if( PM2PendingRXCount > 6 )  return;

    PM2DataPending[PM2PendingRXCount] = nPending;
    PM2PendingRXCount++;
    SetServiceSendFlag();
}

//-----------------------------------------------------------------------------
// get PM2 port data from buffer
//-----------------------------------------------------------------------------
BYTE GetPM2_PendingData(void)
{
	ITempB03 = PM2DataPending[(PM2PendingTXCount&0x07)];
	PM2PendingTXCount++;
	if( PM2PendingTXCount >= PM2PendingRXCount ) {
		PM2PendingTXCount = 0;
		PM2PendingRXCount = 0;
	}
	return ITempB03;
}

const BYTE code initsio_table[]=
{
				// Configure and Enable Logical Device 06h(KBD)
	0x07 ,0x06,	// Select Logical Device 06h(KBD)
  	0x70 ,0x01,	// Set IRQ=01h for Logical Device 06h(KBD)
  	0x30 ,0x01,	// Enable Logical Device 06h(Mouse)
				// Configure and Enable Logical Device 05h(Mouse)
 	0x07 ,0x05,	// Select Logical Device 05h(Mouse)
  	0x70 ,0x0C,	// Set IRQ=0Ch for Logical Device 05h(Mouse)
	0x30 ,0x01,	// Enable Logical Device 05h(Mouse)
				// Enable Logical Device 11h(PM1)
   	0x07 ,0x11,	// Select Logical Device 11h(PM1)
  	0x70 ,0x00,	// Clear IRQ=0 for  Logical Device 11h(PM1)
 	0x30 ,0x01,	// Enable Logical Device 11h(PM1)
				// Enable Logical Device 12h(PM2)
	0x07 ,0x12,	// Select Logical Device 12h(PM2)
	0x70 ,0x00,	// Clear IRQ=0 for  Logical Device 12h(PM2)
	0x30 ,0x01,	// Enable Logical Device 12h(PM2)
				// Enable Logical Device 04h(MSWC)
 	0x07 ,0x04,	// Logical Device 04h(MSWC)
 	0x30 ,0x01,	// Enable MSWC 
 
	//0x07 ,0x10, // Logical Device 10h(BRAM)
	//0x62 ,0x05,
	//0x63 ,0x00, // BRAMLD address is 0x500ndex) and 0x501ata) 
	//0xF3 ,0x3F,  // msmart test
	//0x30 ,0x01, // Enable BRAM
	
 	0x07, 0x10, // Logical Device 10h(BRAM)
 	0x60, 0x02, // I/O Base Address 700h  Bank0//72JERRY083:Change MMIO to mailbox for accessing UCSI registers.
 	0x61, 0x70, // I/O Base Address 700h//72JERRY083:Change MMIO to mailbox for accessing UCSI registers.
	//0x62, 0x07, // I/O Base Address 702h  Bank1
	//0x63, 0x02, // I/O Base Address 702h
	0xF3 ,0x3F, // msmart test
	0xF4 ,0x3F, // msmart test
 	0x70, 0x00, // Set Without IRQ
 	0x30, 0x01, // Enable BRAM

 	0x07 ,0x0F,	// Logical Device 0Fh(Shared Memory)
	0x60 ,0x02, // Set Base address 200h
	0x61 ,0x00, 

				// Define EC internal RAM base address on LPC memory space.
	0xF5 ,0x00, // H2RAM-HLPC Base Address Bits [15:12] (HLPCRAMBA[15:12]) high nibble
	0xF6 ,0x00, // H2RAM-HLPC Base Address Bits [23:16] (HLPCRAMBA[23:16])

	0x30 ,0x01,	// MSWC Shared Memory

    0x07 ,0x17,	// Select Logical Device 17h(PM3)
	0x62 ,0x00, // IO Port 6A4		
	0x63 ,0x6C, //
	0x60 ,0x00, // IO Port 6A0		
	0x61 ,0x68, //
	0x30 ,0x01,	// Enable Logical Device 17h(PM3)
};
//----------------------------------------------------------------------------
// Init super IO function
//----------------------------------------------------------------------------
void InitSio(void)
{
    BYTE code * data_pntr;
    BYTE cnt;

  	SET_MASK(LSIOHA,LKCFG);
  	SET_MASK(IBMAE,CFGAE);
  	SET_MASK(IBCTL,CSAE);

    cnt=0;
    data_pntr=initsio_table;
    while(cnt < (sizeof(initsio_table)/2) )
    {
        IHIOA=0;              // Set indirect Host I/O Address
        IHD=*data_pntr;
        while( IS_MASK_SET(IBCTL,CWIB));
        data_pntr ++;

        IHIOA=1;              // Set indirect Host I/O Address
        IHD=*data_pntr;
        while( IS_MASK_SET(IBCTL,CWIB));
        data_pntr ++;
        cnt ++;
    }

 	CLEAR_MASK(LSIOHA,LKCFG);
  	CLEAR_MASK(IBMAE,CFGAE);
  	CLEAR_MASK(IBCTL,CSAE);
}

//----------------------------------------------------------------------------
// Init system memory cycle to EC external ram 
//----------------------------------------------------------------------------
void InitSysMemory2ECRam(void)
{
	HRAMW0BA = 0x00;					// Define RAM window 0 base address 0x000
	HRAMW0AAS |= HostRamSize2048Byte;	// Host RAM Window 0 Size 2K bytes
	//HRAMWC |= Window0En;
	HRAMW1BA = 0x80;					// Define RAM window 1 base address 0x800
	HRAMW1AAS |= HostRamSize2048Byte;	// Host RAM Window 1 Size 2K bytes
	HRAMWC = 0x03;						// open two windows
}

