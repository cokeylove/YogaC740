/*-----------------------------------------------------------------------------
 * TITLE: OEM_INI.H
 *
 * OEM defined constants for initializing variables to known values on reset.
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/
#ifndef OEM_INI_H
#define OEM_INI_H

extern void Oem_StartUp(void);
extern void Oem_Initialization(void);
extern void Init_ClearRam(void);
extern void Clear_RAM_Range(WORD nAddr, WORD nSize);
extern void RAM_travel(WORD sour, WORD targ, BYTE count);
extern void Hook_ECExitFollowMode(void);
extern BYTE Hook_ECRetunrMainFuncKeepCondition(void);
extern void Init_SMBus_Regs(void);
extern void Init_Regs(void);
extern void CheckECCHIPVer(void);
extern void Check_Mirror_Occurs(void); //T11B +
extern void InitChargerIC(void);//ANGELAS016:Change charge IC option setting.

extern void CacheDma(BYTE sramnum,WORD addr);

typedef struct REG_INIT_DEF
{
    XBYTE 	*address;
    BYTE  	initdata;
} sREG_INIT_DEF;

typedef struct DMAScratchSRAM
{
	XBYTE	*scarh	;
	XBYTE	*scarm	;
	XBYTE	*scarl	;
} sDMAScratchSRAM;
//ANGELAS016:s+Change charge IC option setting.
typedef struct Charger
{
	BYTE Cmd;
	BYTE *Var;
}sCharger;
//ANGELAS016:e+Change charge IC option setting.
#endif
