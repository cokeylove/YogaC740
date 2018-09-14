/*-----------------------------------------------------------------------------
 * TITLE: OEM_FLASH.H
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef	OEM_FLASH_H
#define	OEM_FLASH_H

extern void SetLPCClockFreeRun(void);
extern void Eflash_CmdRamCode(void);
extern void RamProgram(BYTE useport);
extern void RamCode(void);
extern BYTE Read_Eflash_Byte(BYTE FA2,BYTE FA1,BYTE FA0);
extern BYTE Erase_Eflash_1K(BYTE FA2,BYTE FA1,BYTE FA0);
extern void Copy_Eflash_1K(BYTE SFA2,BYTE SFA1,BYTE SFA0, BYTE DFA2, BYTE DFA1, BYTE DFA0);
extern void Write_Eflash_1Byte(BYTE FA2,BYTE FA1,BYTE FA0,BYTE OneByte);
extern void Do_Eflash_Write_1Byte(BYTE OneByte,BYTE Addr2,BYTE Addr1,BYTE Addr0);  //T11 +

extern void FSCEHighLevel( BYTE SpiCmd );
extern void FSPIRomCheckBusy(void);
extern void FSPIRomWriteEnable(void);
extern void FSPIRomWriteDisable(void);

#endif