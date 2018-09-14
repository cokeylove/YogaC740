/**************************************************************************************
 *  SMSC CONFIDENTIAL                                                                 *
 *  Copyright (c) 2008 Standard Microsystems Corporation. All rights reserved.        *
 *  This program contains proprietary and confidential information.                   *
 *                                                                                    *
 * THIS SOFTWARE PROVIDED BY STANDARD MICROSYSTEMS CORPORATION`("SMSC")IS SAMPLE CODE *
 * INTENDED FOR EVALUATION PURPOSES ONLY.  IT IS NOT INTENDED FOR COMMERCIAL USE.     *
 * THIS SOFTWARE IS PROVIDED BY SMSC "AS IS" AND ANY EXPRESS OR IMPLIED               *
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF               *
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND             *
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL SMSC BE LIABLE FOR ANY DIRECT,          *
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES                 *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;       *
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND        *
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT         *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF           *
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                  *
 **************************************************************************************

	Program Name: UCS1002 Charger Sample Program Version 1.3.
	Copyright (C) 2012 Standard Microsystems Corporation
	All rights reverved.

	This is sample program for UCS1002 to work as a dedicated charger in DCE mode.

	Reference:
		SMSC UCS1002 DS Rev. 1.1 (11-21-11).pdf

===============================================================================
Revision History:

Date
Version   	who  Comment
----------  --- --------------------------------------------------------------
2012/03/04	jc	.Initial version.
V1.0
				Size information reported by C51
				MODULE INFORMATION:   STATIC OVERLAYABLE
				CODE SIZE        =    826    ----
				CONSTANT SIZE    =     69    ----
			   	XDATA SIZE       =      8    ----
			    PDATA SIZE       =   ----       3
				DATA SIZE        =   ----    ----
				END OF MODULE INFORMATION.

2012/03/15	jc	.Fine tune timing.
V1.1

2012/03/23  jc 	.Fine tune current parameters.
V1.2
				Size information reported by C51
				MODULE INFORMATION:   STATIC OVERLAYABLE
				CODE SIZE        =    889    ----
				CONSTANT SIZE    =     71    ----
				XDATA SIZE       =     10    ----
				PDATA SIZE       =   ----       3
				DATA SIZE        =   ----    ----
				IDATA SIZE       =   ----    ----
				BIT SIZE         =   ----    ----
				END OF MODULE INFORMATION.

**************************************************************************************/
#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

#if UCS1022_Support
//extern void UCS1002DCECharger_Init( void );
extern void UCS1002HCACharger_Init();

typedef unsigned char xdata	db;
typedef unsigned int  xdata	dw;
//typedef unsigned char 		BYTE;

#define UCSADDRESS 		0x57	// UCS1002's slave address

//#ifdef	LOWACT				// POWER control
#define	PWRCTRL			0xA4	// Low active
//#else
//#define	PWRCTRL		0xA6	// High active
//#endif

//#define	Normal_USB	0xB8	// Low active, Set BC1.2 CDP mode.
#define	Normal_USB		0x98	// Low active, Set Data pass through mode.
#define	Data_OFF		0x9A	// Low active

	const	db 	code	bCRS[] = 	// common registers for init. routine
	{0x16, 0x19, 0x1e, 0x1f, 0x20, 0x21, 0x25, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x47, 0x49, 0x4b, 0x51};
	const db 	code	bCRD[ sizeof(bCRS)/sizeof(BYTE)] = 	 // data to be writen to common registers for init. routine
	{0x8c, 0x07, 0x01, 0x01, 0x16, 0x7f, 0x14, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x96};

volatile db	bTimeCnt;
    db	 bState;
    db   bNum;
    db   bTmp;
    db   bMax;
    db	 bLastPN;
    db   i;
    db   j;
    db   kk;
    dw	 wC;


void SMSC_ProfilesUpdate(XBYTE AddrH,XBYTE AddrL,XBYTE ByteCnt)
{
    XBYTE Temp;
    XWORD AddrTemp;
    PORT_BYTE_PNTR byte_register_pntr;

    byte_register_pntr = 0x2205;

    AddrH += 0xC0;

    AddrTemp = (AddrH<<8)+AddrL;

//    CacheDma(1,FSCEHighLevel);
    DisableAllInterrupt();
    for(Temp=0; Temp<ByteCnt; Temp++)
    {
        Write_Eflash_1Byte(0x01, AddrTemp>>8 ,AddrTemp ,*byte_register_pntr);
        byte_register_pntr++;
        AddrTemp++;
    }
    EnableAllInterrupt();

}


void SMSC_ProfilesRead(XBYTE AddrH,XBYTE AddrL,XBYTE ByteCnt)
{
    XBYTE Temp;
    XWORD AddrTemp;
    PORT_BYTE_PNTR byte_register_pntr;

    byte_register_pntr = 0x2205;

    AddrH += 0xC0;

    AddrTemp = (AddrH<<8)+AddrL;

//	CacheDma(1,FSCEHighLevel);
    DisableAllInterrupt();

    for(Temp=0; Temp<ByteCnt; Temp++)
    {
        *byte_register_pntr = Read_Eflash_Byte(0x01,AddrTemp>>8 ,AddrTemp);
        byte_register_pntr++;
        AddrTemp++;
    }
    EnableAllInterrupt();
}
#endif	// UCS1022_Support

void Ck_UpdateProfiles()
{
	#if UCS1022_Support
    if(SMSC_Update_Issue != 0x55)
    { return; }

    switch(SMSC_Update_Cmd)
    {
        case 0x01: // Erase eFlash 0x1C000 ~ 0x1C3FF
        DisableAllInterrupt();
//	    CacheDma(1,FSCEHighLevel);
        Erase_Eflash_1K(0x01,0xC0,0x00);
        EnableAllInterrupt();
        SMSC_Update_Cmd |= 0x80;
        break;

        case 0x02: // Write Profiles into eFlash
        SMSC_ProfilesUpdate(SMSC_Update_StartAddrH,SMSC_Update_StartAddrL,SMSC_Update_ByteCnt);
        SMSC_Update_Cmd |= 0x80;
        break;

        case 0x03: // Read Profiles from eFlash
        SMSC_ProfilesRead(SMSC_Update_StartAddrH,SMSC_Update_StartAddrL,SMSC_Update_ByteCnt);
        SMSC_Update_Cmd |= 0x80;
        break;
    }
	#endif	// UCS1022_Support
}


/*******************************************************************************
 TimerX_Init

 This function configs EC's TimerX interrupts every 50 mS for TimerX_ISR !
*******************************************************************************/
void TimerX_Init(void)
{
	// TODO: Init. TimerX_ISR for UCS1002 here
}

/*******************************************************************************
 TimerX_ISR

 Timer interrupt service ountine must be adapted to your EC !
*******************************************************************************/
void UCS1002_TimerX_FakeISR (void)
{
	#if UCS1022_Support
	if ( SystemIsS3 )
	{
		// Check AC mode and battery <30%
		if ( IS_MASK_CLEAR(SYS_STATUS,AC_ADP) && (nBattGasgauge <= USB_BAT_LOW) )
 		{ return; }
	}
	if ( SystemIsS5 )
	{
		if ( IS_MASK_CLEAR(USB_Charger, b1USB_initOK) ) { return; }

		//if ( IS_MASK_CLEAR(USB_Charger, b0USB_EN) )	// check to always charger.
		if ( IS_MASK_CLEAR(EMStatusBit, b1SetUSBChgEn) )	// check to always charger.
		{ return; }
		else
		{
			// Check AC mode and battery <30%
			if ( IS_MASK_CLEAR(SYS_STATUS,AC_ADP) && (nBattGasgauge <= USB_BAT_LOW) )
	 		{ return; }
		}
	}
	if ( !++bTimeCnt )	--bTimeCnt;	// keep underflow
	// TODO: EOI of TimerX_ISR
	#endif	// UCS1022_Support
}

/*******************************************************************************
	Issuse a read byte command to UCS1002 via SMBus

	Input: bCMData = Target register Address for Reading
	output: Byte date of the UCS1002's register : [bCMData]
*******************************************************************************/
#if UCS1022_Support
BYTE ReadByte( BYTE bCMData )
{

	// ToDo: call to EC's SMBus read byte function here
//	return	SMB_ByteRead( UCSADDRESS, bCMData );
	BYTE BsData;
	bRWSMBus(SMbusCh2, SMbusRB, 0xAD, bCMData, &BsData, 0);

	return BsData;
}

/*******************************************************************************
	Issuse a write byte command to UCS1002 via SMBus

	Input: bCMData = Target register Address for writing
		   bData = output data for writing to UCS1002
	output: NONE
*******************************************************************************/
void WriteByte( BYTE bCMData, BYTE bData )
{
	// ToDo: call to EC's SMBus write byte function here
//	SMB_ByteWrite( UCSADDRESS, bCMData, bData )
	bRWSMBus(SMbusCh2, SMbusWB, 0xAC, bCMData, &bData, SMBus_NoPEC);
}

/*******************************************************************************
	Turn on port Power
*******************************************************************************/
void PwrOn( void )
{
	WriteByte( 0x17, PWRCTRL);
}
/*******************************************************************************
	Turn off port Power
*******************************************************************************/
void PwrOff( void )
{
	WriteByte( 0x17, PWRCTRL ^ 0x02);
}

/*******************************************************************************
	Config profile
*******************************************************************************/
void CProfile( void )
{
	for( bTmp = 1; bTmp < bLProfile[1] ; ++bTmp)
		WriteByte( bLProfile[bTmp+2], bLProfile[ 2+ (i+1)*bLProfile[1] + bTmp] );
}
#endif	// UCS1022_Support

/*****************************************************************************/
// Procedure: UCS1002ID_Main								TimeDiv: 100mSec
// Description: Check USB charger of device.
// GPIO:
// Referrals:
/*****************************************************************************/
void UCS1002ID_Main(void)
{
	#if UCS1022_Support
	// Check SMSC ID and thermal over temperature.
	if ( (UCS1002ID != SMSC_UCS1002ID)  || (IS_MASK_SET(USB_Charger, b2USB_TmlDis)) )
	{ return; }

	if ( SystemIsS3 )
	{
		// Check AC mode and battery > 30%
		if ( IS_MASK_SET(SYS_STATUS,AC_ADP) || (nBattGasgauge > USB_BAT_LOW) )
 		{ UCS1002HCACharger(); }
        else //Disable charger.
        { WriteByte( 0x17, Data_OFF); }	// Power OFF.
	}
	else if ( SystemIsS5 )
    {
        //if ( (IS_MASK_SET(USB_Charger, b0USB_EN)) )	// check to always charger.
        if ( IS_MASK_SET(EMStatusBit, b1SetUSBChgEn) )	// check to always charger.
		{
			if ( IS_MASK_CLEAR(USB_Charger, b1USB_initOK) )
			{
				UCS1002HCACharger_Init();	// Re initial USB IC.
			 	Init_USB_S0();
				SET_MASK(USB_Charger, b1USB_initOK);
				SMSC_ChgDelay = 10;	// delay x * 100msec.
				USB_ChargerStep = 0;
			}

		    // Check AC mode and battery > 30%
			if ( IS_MASK_SET(SYS_STATUS,AC_ADP) || (nBattGasgauge > USB_BAT_LOW) )
			{ UCS1002HCACharger(); }
            else
            { WriteByte( 0x17, Data_OFF); }	// Power OFF.
		}
        else // Disable charger.
        { WriteByte( 0x17, Data_OFF); }	// Power OFF.
    }
	else if ( SystemIsS0 )
 		 {
		 	if ( IS_MASK_SET(EMStatusBit, b1SetUSBChgEn) )	// check to always charger.
			{ SET_MASK(EMStatusBit, b6RdUSBChgS45); }
		 	else
			{ CLR_MASK(EMStatusBit, b6RdUSBChgS45); }

 			if ( IS_MASK_CLEAR(USB_Charger, b1USB_initOK) )
			{
				UCS1002HCACharger_Init();	// Re initial USB IC.
			 	Init_USB_S0();
				SET_MASK(USB_Charger, b1USB_initOK);
				SMSC_ChgDelay = 10;	// delay x * 100msec.
				USB_ChargerStep = 0;
			}

			if ( SMSC_ChgDelay == 0 )
			{
				switch ( USB_ChargerStep )
				{
					case 0:	// Normal behavior.
					default:
						USB_Error = ReadByte(0x10);
						if ( IS_MASK_SET(USB_Error, b7USB_ErrorCtl) )		// Check 10H bit7.
						{
							if ( IS_MASK_SET(USB_Error, b0USB_OverILIM) )	// Check USB OverILIM.
							{
								WriteByte( 0x15, 0x01);	// Send alert status to PCH.
								USB_ChargerStep = 1;	// Step1: Recover 15H status.
								break;
							}
							else
							{
								PwrOff();				// USB SW power Off.
								USB_ChargerStep = 2;	// Step2: USB SW power On.
							}
						}
						break;
						case 1:	// Recover 15H status.
						WriteByte( 0x15, 0x81);	// Send alert status to PCH.
						PwrOff();				// USB SW power Off.
						USB_ChargerStep = 2;	// Step2: USB SW power On.
						break;
						case 2:	// USB SW Power On.
						WriteByte( 0x17, Normal_USB);
						USB_ChargerStep = 0;	// Step0: Normal behavior.
						break;
				}
			}
			else
			{ SMSC_ChgDelay--; }
 		}
		else
		{ WriteByte( 0x17, Data_OFF); }	// Power OFF.
}

/******************************************************************************
	S0 USB charger initial status.
******************************************************************************/
void Init_USB_S0(void)
{
	PwrOff();
	WriteByte( 0x18, 0x4C);
	WriteByte( 0x17, Normal_USB);
	#endif	// UCS1022_Support
}

/*******************************************************************************
	Init. UCS1002 to charge in DCE mode
*******************************************************************************/
void UCS1002HCACharger_Init( void )
{
	#if UCS1022_Support
	TimerX_Init();

	PwrOff( );
	bNum = -1;
	bState= 0;
        kk = 0;

	for ( i = 0 ; i < sizeof(bCRS)/sizeof(BYTE)  ; ++i)
		WriteByte( bCRS[i], bCRD[i] );

	WriteByte( 0x15, 0x81);
	#endif	// UCS1022_Support
}

#if UCS1022_Support
BYTE ReadUCS1022ID(void)
{
    return ReadByte(0xFD);
}
///*******************************************************************************
//	UCS1002 DCE Charger State machine
//*******************************************************************************/
void UCS1002HCACharger( void )
{
	switch( bState )
	{
		case 0:
		case 12:
			PwrOff( );
			WriteByte( 0x18, 0x5C );
			bTimeCnt = 0;
			++bState;

		case 1:
		case 13:
			if ( bTimeCnt > 40 )
			{
				bTimeCnt = 0;
                kk = 0;
				bState = 1;
				return;
			}

			if ( !(ReadByte( 0x0F ) & 0x10) && !(ReadByte( 0x11 ) & 0x01) )
				return;

			if ( bState > 10 )
			{
				bState = 18;
				return;
			}

			i = bMax = 0, j = -1, bLastPN = bNum;
			bState = 10;

		case 10:
			WriteByte( 0x18,  0x5D );

			if ( i && bNum != -1 )
			{
				if ( bLastPN == bNum)
				i = bLastPN;
				else if ( i == bNum )
				++i;
			}

			if ( i >= bLProfile[0]  )
			{
				bState = 20;
				return;
			}
			else if ( i == 1 )
				bLastPN = -3;

			CProfile( );

			bTimeCnt = 0;
			bState = 11;
		case 11:
			if ( bTimeCnt < 14  )
				return;
			PwrOn( );

			wC = bTmp = bTimeCnt = 0;
			bState = 17;
		case 17:

					if ( !(ReadByte( 0x0f ) & 0x10) )
				        {
					    bState = 12;
					    return;
				        }

			if(	bTimeCnt/10 > bTmp  )
			{
				bState =  ReadByte( 0x00 );
				wC += bState;
				if ( ++bTmp >=  bLProfile[ 2+ (i+1)*bLProfile[1] ] )
			{
				bTmp = wC / bTmp ;
					if ( bState > bTmp )
						bTmp = bState;

				if ( bTmp > bMax )
				{
					bMax = bTmp;
				 	j = i;
					}

					if ( (bLProfile[2] & 0x80) && bMax >= 56 && bLastPN == -3 )
					{
						if ( i == j )
				        {
					     bState = 30;
					     return;
				        }
						else i = bLProfile[0];
				}

				PwrOff( );
		case 18:
					if ( i && bNum != -1 && bLastPN == bNum)
						i = 1,	bLastPN = -2;

				else
					++i;

				bState = 10;
			}
				else
					bState = 17;
			}
			return;

		case 20:
			i = j;
			if ( j == -1 )
				i = 0;
			CProfile( );

			bTimeCnt = 0;
			bState = 22;
		case 22:
			if ( bTimeCnt < 20  )
				return;
			PwrOn();


			if ( j == -1 )
			{
                        bTmp = 0;

		case 23:
				bState = 24;
                        bTimeCnt = 0;
		case 24:
				if ( bTimeCnt < 200  )
				{
					if ( ReadByte( 0x0f ) & 0x10 )
						return;
					bTmp = 30;
				}

				bState = ReadByte( 0x00 );
				if ( bState >= 56 )
				{
					bMax = bState;
					bState = 30;
				}
				else if ( bTmp++ < 30 )
					bState = 23;
				else
					bState = 0;
				return;
			}

			bState = 30;

		case 30:
			if ( j ) bNum = j;
			else bNum = -1;

		case 32:
			++bState;
			wC = j = bTmp = bTimeCnt = 0;

		case 31:
		case 33:
			if ( ReadByte(0x11) & 0x02 )
				bState = 0;

			else if ( bTimeCnt > 40  )
			{
				if ( bMax < 56 )
			{
					 if ( (bTimeCnt = ReadByte( 0x00 )) >= 56  )
						bMax = bTimeCnt;
					 else if ( !kk && ++wC > 150 )
					{
						++kk;
						bState = 0;
						return;
					}
				}

				if ( bState > 31 )
				{
					bTimeCnt = 0;
					return;
				}


				i = ReadByte( 0x0F ) & 0x18;

				if (  !(i & 0x08 ) )
				{
					if ( bTmp < 2 )
						PwrOff( );

					if ( (!i && j ) || j >= 3 )
					{
						if ( j >= 3 && i == 0x10 )
							bState = 32;
						else
							bState = 0;
						return;
					}
					++j;
					if ( bTmp < 2  )
					{
						++bTmp;
						bState = 35;
						bTimeCnt = 0;

		case 35:
						if ( bTimeCnt < 14  )
							return;
	  					PwrOn();
					}

				}
				else j = 0;
				bTimeCnt = 0;
				bState = 31;
			}
			return;

		default:
			break;
	}
}
#endif	// UCS1022_Support

#if 0
/*******************************************************************************
	Example of Main routine
*******************************************************************************/
void main ( void )
{

	UCS1002HCACharger_Init( ); // call to init. 50mS timer and UCS1002 first

	while(1)
	{

		// recommand to keep calling to UCS1002DCECharger function in shorter than every 100 mS
		UCS1002HCACharger( );

	}
}
#endif
