/*-----------------------------------------------------------------------------
 * TITLE: OEM_FLASH.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

void SetLPCClockFreeRun(void)
{
	GPCRH0 = OUTPUT;
	CLEAR_MASK(GPDRH,BIT(0));
}

//----------------------------------------------------------------------------
// The function of Scratch rom
//----------------------------------------------------------------------------
void RamProgram(BYTE useport)
{
	RamCodePort = useport;
	DisableAllInterrupt();

	Tmp_XPntr = 0x700;
	Tmp_code_pointer = (void *)Eflash_CmdRamCode;

   	for(ITempB01=0;ITempB01<255;ITempB01++)
   	{
		*Tmp_XPntr = *Tmp_code_pointer;
      	Tmp_XPntr++;
   		Tmp_code_pointer++;
   	}
	SET_MASK(FBCFG,SSMC);       	// enable scatch ROM

    _nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();

	RamCode();
}

//----------------------------------------------------------------------------
// Host send idle command to EC
//----------------------------------------------------------------------------

void Eflash_CmdRamCode(void)
{
	RamcodeCmd = 0x00;
	DisableInternalWDT();
	SET_MASK(SMECCS,HOSTWA);	// Host write allow
	KBHIKDOR = 0xFA;
 	while(1)
  	{
//  		BRAM31++;
		if( IS_MASK_CLEAR(KBHISR,IBF)) continue;
		if( IS_MASK_CLEAR(KBHISR,C_D)) continue;
		RamcodeCmd = KBHIDIR;
        if (RamcodeCmd == 0x8F)// || (BRAM31==0x55))	// EC exit follow mode
     	{
			WinFlashMark = 0x33;
            WinFlashMark2 = 0x88;
			break;
  		}
		else if (RamcodeCmd == 0x9F)
		{
			CKCON &= 0x3F;			// set 26 bit counter // bit7-6 00=14ms,01=113ms, 10=911ms, 11(1/9.2MHz)*2'26=7.2S
			CKCON |= 0x10;			// set 26 bit counter // bit7-6 00=14ms,01=113ms, 10=911ms, 11(1/9.2MHz)*2'26=7.2S
            WDTCON|=0x01;   		/* WDTRST = 1 Reset watch dog timer.*/
            WDTCON|=0x02;   		/* WDTEN  = 1 Enable watch dog.     */
            while(1);       		/* Wait for watch dog time-out      */
		}
	}
  	CLEAR_MASK(SMECCS,HOSTWA);  // Host write not allow
	_nop_();
	main();
}


#pragma ot(8, speed)
void FSCEHighLevel( BYTE SpiCmd )
{
    ECINDAR3 = 0x4F;//0xFF        // Enter follow mode
    ECINDAR2 = 0xFF;
    ECINDAR1 = 0xFE;
    ECINDAR0 = 0x00;
    ECINDDR  = 0xFF;        // SCE# high level

    ECINDDR  = 0xFF;        // SCE# high level
    ECINDAR1 = 0xFD;        // Address:FD00h
    ECINDDR  = SpiCmd;      // SPI Command

}

#pragma ot(8, speed)
void FSPIRomCheckBusy(void)
{
    while(1)
    {
        FSCEHighLevel( 0x05 );  // Read Status Reg (05h)
        if( ~ECINDDR & 0x01 )
        {
            break;
        }
    }
}
//cwy modified the write eflash enable 
#if 0
#pragma ot(8, speed)
void FSPIRomWriteEnable(void)
{
	SET_MASK(SMECCS,HOSTWA);
    ECINDAR3 = 0x4F;        // Enter follow mode
    ECINDAR2 = 0xFF;
    ECINDAR0 = 0x00;        // SCE# high level


    FSCEHighLevel( 0x01 );      // write status register (01h)
    ECINDDR = 0x00;             // clear all protect

    FSCEHighLevel( 0x06 );      // write enable (06h)
    while(1)
    {
        FSCEHighLevel( 0x05 );  // Read Status Reg (05h)
        if( ECINDDR & 0x02 )
        {
            break;
        }
    }
}

#pragma ot(8, speed)
void FSPIRomWriteDisable(void)
{
    FSCEHighLevel( 0x04 );
    ECINDAR3 = 0x40;        //Exit follow mode
    ECINDAR2 = 0x00;
    CLEAR_MASK(SMECCS,HOSTWA);
}

#endif  


void FSPIRomWriteEnable(void)
{
	ECINDAR3 = 0x4F; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				// FFFFExx = 0xFF  
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while(1)						// waiting spi free
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}								
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_WREN;			// Write enable	
    if(SPIID==SSTID)
    {
        ECINDAR1 = 0xFE;
        ECINDDR = 0xFF;				// SCE# high level
        ECINDAR1 = 0xFD;
        ECINDDR = SPICmd_EWSR;		// Enable Write Status Register
    }
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while(1)						// waiting spi free
	{
		if((ECINDDR&(SPIStatus_WEL+SPIStatus_BUSY))==0x02) // Check write enable and spi not busy
        {
            break;
        } 
	}
	ECINDAR3 = 0x40;     	//:00 to 40 //Exit follow mode
	ECINDAR2 = 0x00;
}
void FSPIRomWriteDisable(void)
{
	ECINDAR3 = 0x4F; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				// FFFFExx = 0xFF  
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while(1)						// waiting spi free
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}								
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_WRDI;			// Write disable	
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while(1)						// waiting spi free
	{
		if((ECINDDR&(SPIStatus_WEL+SPIStatus_BUSY))==0x00) // Check write disable and spi not busy
        {
            break;
        } 
	}
	ECINDAR3 = 0x40;     		//:00 to 40  //Exit follow mode
	ECINDAR2 = 0x00;
}


#pragma ot(8, speed)
void Write_Eflash_1Byte(BYTE FA2,BYTE FA1,BYTE FA0,BYTE OneByte)
{
    FSPIRomWriteEnable();
    FSCEHighLevel( 0x02 );    // Others Page program
    ECINDDR = FA2;            // A2
    ECINDDR = FA1;            // A1
    ECINDDR = FA0;            // A0
    ECINDDR = OneByte;
    FSPIRomCheckBusy();
    FSPIRomWriteDisable();

    eFlashVarifyOK = 0x78;
}

#pragma ot(8, speed)
//*************************************************************************
BYTE Erase_Eflash_1K(BYTE FA2,BYTE FA1,BYTE FA0)
{
    FSPIRomWriteEnable();
    FSCEHighLevel( 0xD7 );  // erase command 0x20:Sector 0xD8:Block
    ECINDDR = FA2;        // A2
    ECINDDR = FA1;        // A1
    ECINDDR = FA0;        // A0
    FSPIRomCheckBusy();
    FSPIRomWriteDisable();

	return (0x01);
}


//#pragma ot(8, speed)
void Copy_Eflash_1K(BYTE SFA2,BYTE SFA1,BYTE SFA0, BYTE DFA2, BYTE DFA1, BYTE DFA0)
{

}
void Eflash_Write_1Byte(void)
{
    ECINDAR3 = 0x4F; 
    ECINDAR2 = 0xFF;
    ECINDAR0 = 0x00;                        // FFFFExx = 0xFF  
    ECINDAR1 = 0xFE;
    ECINDDR = 0xFF;                         // SCE# high level
    ECINDAR1 = 0xFD;
    ECINDDR = SPICmd_ReadStatus;            // Read Status Register
    while(1)                                // waiting spi free
    {
        if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
    }                                                               
    ECINDAR1 = 0xFE;
    ECINDDR = 0xFF;                         // SCE# high level
    ECINDAR1 = 0xFD;
    ECINDDR = SPICmd_BYTEProgram;           // Send Program One Data Byte command
    ECINDDR = SPIAddrCycle[0];              // For 256 bytes function limite
    ECINDDR = SPIAddrCycle[1];
    ECINDDR = SPIAddrCycle[2];
    SPIIndex = 0x00;
    ECINDDR = SPIIDBuf[0];
    ECINDAR1 = 0xFE;
    ECINDDR = 0xFF;                         // SCE# high level
    ECINDAR1 = 0xFD;
    ECINDDR = SPICmd_ReadStatus;            // Read Status Register
    while(1)                                // waiting spi free
    {
        if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
    }       
    ECINDAR3 = 0x40;  //:00 to 40                 // Exit follow mode
    ECINDAR2 = 0x00;                           // 
}


//#pragma ot(8, speed)
BYTE Read_Eflash_Byte(BYTE FA2,BYTE FA1,BYTE FA0)
{
    ECINDAR3 = 0x40; // 0
	ECINDAR2 = FA2;
	ECINDAR1 = FA1;
	ECINDAR0 = FA0;
	return ECINDDR;
}

#pragma ot(7, speed)
void Do_Eflash_Write_1Byte(BYTE OneByte,BYTE Addr2,BYTE Addr1,BYTE Addr0)
{
    SPIIDBuf[0] = OneByte;
    SPIAddrCycle[0] = Addr2;
    SPIAddrCycle[1] = Addr1;
    SPIAddrCycle[2] = Addr0;
    DisableAllInterrupt();                   // Disable all interrupt
    LoadSPIFucnToRam(FSPIRomWriteEnable);
    SET_MASK(FBCFG,SSMC);                    // enable scatch ROM
    FuncAt_0xFE00();                         // do function in extern ram 0x600
    EnableAllInterrupt();                    // Enable all interrupt 
    CLEAR_MASK(FBCFG,SSMC);                  // disable scatch ROM
    LoadSPIFucnToRam(Eflash_Write_1Byte);    // Load function to ram
    SET_MASK(FBCFG,SSMC);                    // enable scatch ROM
    FuncAt_0xFE00();                         // do function in extern ram 0x600
    EnableAllInterrupt();                    // Enable all interrupt 
    CLEAR_MASK(FBCFG,SSMC);                  // disable scatch ROM
    LoadSPIFucnToRam(FSPIRomWriteDisable);   // Load function to ram
    SET_MASK(FBCFG,SSMC);                    // enable scatch ROM
    FuncAt_0xFE00();                         // do function in extern ram 0x600
    EnableAllInterrupt();                    // Enable all interrupt 
    CLEAR_MASK(FBCFG,SSMC);                  // disable scatch ROM
}



