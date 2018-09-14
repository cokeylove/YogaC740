/*-----------------------------------------------------------------------------
 * TITLE: CORE_INIT.H
 *
 * Author : Cark
 *
 * Copyright (c) ITE INC. All Rights Reserved.
 *
 *---------------------------------------------------------------------------*/

#ifndef CORE_EGPC_H
#define CORE_EGPC_H

//-----------------------------------------------------------------------------
// External IO
//-----------------------------------------------------------------------------
#define EX_OUT_HI			0x03
#define EX_OUT_LOW     		0x02
//#define EX_OUT_HIPuHI		0x07
//#define EX_OUT_HIPuLOW	0x0B
//#define EX_OUT_LOWPuHI	0x06
//#define EX_OUT_LOWPuLOW	0x0A
#define EX_INPUT     		0x00
#define EX_IN_PuHI     		0x04
#define EX_IN_PuLOW     	0x08
#define EX_OPEN_Drain     	0x10

//-----------------------------------------------------------------------------
extern bit EGPC_Read( BYTE Addr );
extern void EGPC_Write( BYTE Addr, BYTE Data );
extern void Set_EGPC( BYTE Addr );
extern void Clr_EGPC(BYTE Addr);

extern void Init_IT8302(void);
extern void Init_IT8302_ControlPin(void);

#endif
