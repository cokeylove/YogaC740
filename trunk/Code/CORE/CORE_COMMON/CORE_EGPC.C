/*-----------------------------------------------------------------------------
 * TITLE: CORE_EGPC.C
 *
 * Author : Cark
 *
 * Copyright (c) ITE INC. All Rights Reserved.
 *
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

#define T_EGPC_LOOP         0x100

//-----------------------------------------------------------------------------
// Read External GPIO
//-----------------------------------------------------------------------------
bit EGPC_Read( BYTE Addr )
{
	#if	Support_External_IO
    EADDR = Addr;

    if( (EDAT >> 5) & 0x01 )
        return  TRUE;

	#endif	// Support_External_IO
    return  FALSE;
}

//-----------------------------------------------------------------------------
// Write External GPIO
//-----------------------------------------------------------------------------
void EGPC_Write( BYTE Addr, BYTE Data )
{
	#if	Support_External_IO
    BYTE EGPC_TimeOut;

    EADDR = Addr;
    EDAT  = Data;

    EGPC_TimeOut = T_EGPC_LOOP;		// Count 0x100
    while( (ESTS & 0x01) == 0 )
    {
        EGPC_TimeOut--;
        if( EGPC_TimeOut == 0 )
		{ break; }
    }   // Check transfer cycle done
    #endif	// Support_External_IO
}

//-----------------------------------------------------------------------------
// Set External GPIO
//-----------------------------------------------------------------------------
void Set_EGPC( BYTE Addr )
{
	#if	Support_External_IO
    BYTE EGPC_TimeOut;

    EADDR = Addr;
    EDAT |= 0x01;

    EGPC_TimeOut = T_EGPC_LOOP;
    while( (ESTS & 0x01) == 0 )
    {
        EGPC_TimeOut--;
        if( EGPC_TimeOut == 0 )     break;
    }   // Check transfer cycle done
	#endif	// Support_External_IO
}


//-----------------------------------------------------------------------------
// Clear External GPIO
//-----------------------------------------------------------------------------
void Clr_EGPC(BYTE Addr)
{
	#if	Support_External_IO
    BYTE EGPC_TimeOut;

    EADDR = Addr;           // Address
    EDAT &= ~0x01;          // Data

    EGPC_TimeOut = T_EGPC_LOOP;
    while( (ESTS & 0x01) == 0 )
    {
        EGPC_TimeOut--;
        if( EGPC_TimeOut == 0 )     break;
    }   // Check transfer cycle done
    #endif	// Support_External_IO
}

//-----------------------------------------------------------------------------
void Init_IT8302(void)
{
	#if	Support_External_IO
    EGPC_Write(EG_FB_CLAMP,     EX_INPUT);	// 000100 00 GPIO4   FB_CLAMP
    EGPC_Write(EG_GC6_EVENT,    EX_INPUT);	// 000101 00 GPIO5   GC6_EVENT#
    EGPC_Write(EG_DGPU_PWROK,   EX_INPUT);	// 000111 00 GPIO7   DGPU_PWROK
    EGPC_Write(EG_DGPU_PWR_EN,  EX_OUT_LOW);// 001001 00 GPIO9   DGPU_PWR_EN
    EGPC_Write(EG_DGPU_HOLD_RST,EX_OUT_LOW);// 001011 00 GPIO11  DGPU_HOLD_RST#
    EGPC_Write(EG_DGPU_GC6_EN,  EX_INPUT);	// 001101 00 GPIO13  DGPU_GC6_EN
    EGPC_Write(EG_NVDD_PWR_EN,  EX_OUT_LOW);// 010010 00 GPIO18  NVDD_PWR_EN
    EGPC_Write(EG_KBL_DET,      EX_INPUT);	// 010100 00 GPIO20  KBL_DET#

    EGPC_Write(EG_GPIO22,       EX_OUT_LOW);// 010110 00 GPIO22
    EGPC_Write(EG_S_NVDD_PWR_EN,EX_OUT_LOW);// 011000 00 GPIO24  S_NVDD_PWR_EN
    EGPC_Write(EG_GPIO26,       EX_INPUT);	// 011010 00 GPIO26
    EGPC_Write(EG_S_DGPU_RST,   EX_OUT_LOW);// 011011 00 GPIO27  S_DGPU_RST
    EGPC_Write(EG_S_DGPU_PWR_EN,EX_OUT_LOW);// 011101 00 GPIO29  S_DGPU_PWR_EN
    EGPC_Write(EG_S_DGPU_PWROK, EX_INPUT);	// 011111 00 GPIO31  S_DGPU_PWROK
    EGPC_Write(EG_S_GC6_EVENT,  EX_INPUT);	// 100001 00 GPIO33  S_GC6_EVENT#
    EGPC_Write(EG_S_GC6_EN,     EX_INPUT);	// 100011 00 GPIO35  S_GC6_EN
    #endif	// Support_External_IO
}

void Init_IT8302_ControlPin(void) // Control Pin Default Output Low
{
	#if	Support_External_IO
    DGPU_PWR_EN_LOW();
    DGPU_HOLD_RST_LOW();
    DGPU_GC6_EN_LOW();
    NVDD_PWR_EN_LOW();
    S_NVDD_PWR_EN_LOW();
    S_DGPU_RST_LOW();
    S_DGPU_PWR_EN_LOW();
	DGPU_FB_CLAMP_LOW();
	#endif	// Support_External_IO
}


