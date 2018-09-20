/*----------------------------------------------------------------------------
 * Filename: OEM_Power.H  For Chipset: ITE.IT85XX
 *
 * Function: Power sequnce control function example
 *
 * Author  : Dino
 *
 * Copyright (c) 2009 - , ITE Tech. Inc. All Rights Reserved.
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef OEM_POWER_H
#define OEM_POWER_H

//-----------------------------------------------------------------------------
// Define
//-----------------------------------------------------------------------------
#define	SYSTEM_S0			0x10
#define	SYSTEM_S3			0x33
#define	SYSTEM_S4			0x44
#define	SYSTEM_S5			0x55
#define SYSTEM_DSX          0x66 //JIMGBRWT01

#define	SYSTEM_S5_S0		0x50
#define	SYSTEM_S4_S0		0x40
#define	SYSTEM_S3_S0		0x30
#define SYSTEM_DSX_S5       0x60

#define	SYSTEM_S0_S3		0x03
#define	SYSTEM_S0_S4		0x04
#define	SYSTEM_S0_S5		0x05
#define SYSTEM_S5_DSX       0x06

#define	SYSTEM_EC_WDTRST	0xFF
#define SYSTEM_S3_DS3		0x60
#define SYSTEM_DS3			0x63
#define SYSTEM_DS3_S3		0x65
#define	SYSTEM_S3S4			0x34

//-----------------------------------------------------------------------------
// System status define
//-----------------------------------------------------------------------------
#define SystemIsS0          SysPowState==SYSTEM_S0
#define SystemIsS3          SysPowState==SYSTEM_S3
#define SystemIsS4          SysPowState==SYSTEM_S4
#define SystemIsS5          SysPowState==SYSTEM_S5	// equal 0x55.
#define SystemIsDSX         SysPowState==SYSTEM_DSX

#define SystemNotS0         SysPowState!=SYSTEM_S0
#define SystemNotS3         SysPowState!=SYSTEM_S3
#define SystemNotS4         SysPowState!=SYSTEM_S4
#define SystemNotS5         SysPowState!=SYSTEM_S5
//-----------------------------------------------------------------------------
// Shutdown cause
//-----------------------------------------------------------------------------
#define SC_EC_0x0000        0xEC    // EC code reset
#define SC_ExtWatchDog      0xFF    // external watch-dog
#define SC_IntWatchDog      0xFE    // internal watch-dog
#define SC_ECColdBoot       0xFC    // EC VSTBY or WRST reset
#define SC_EC8SRESET        0xFD    // EC VSTBY or WRST reset ANGELAS089:add

//B04 + e
#define CPU_PWROK_VLIMIT    0X111  //0.8V   0.8*(3/3FF)=0X111 JERRYB0033
void Write_NOVOS4(void);
extern void CheckResetSource(void);
extern void OEM_LPC_Reset(void);
extern void Oem_SysPowerContrl(void);
extern void Oem_S5S0Sequence(void);
extern void Oem_S0S5Sequence(void);
extern void Oem_S3S0Sequence(void);
extern void Oem_S0S3Sequence(void);
extern void Oem_S5DSXSequence(void);
extern void Oem_DSXS5Sequence(void);
extern void CheckAutoPowerOn(void);
//extern void CHK_BATT_5PERCL(void);
extern void check8sreset(void); //ANGELAS041:add
extern void S34toDSx(void);
extern void DSxtoS34(void);
extern void S0S3_VR_ON(void);
extern void S0S3_SUSP(void);  //ANGELAS063:add
extern void S0S5_SYS_ON(void); //ANGELAS063:add
extern void S0S5_EC_VCCST_EN(void); //ANGELAS063:add
extern void PulseSBPowerButton(void); 
extern void CheckSBPowerButton(void); 


extern void SendSynapticsIDSequence();
extern void SendElanIDSequence();
extern void ReadTPid();
extern void Check_EC_ON();
extern void Lowpower_ENfunction(void); //ANGELAS047:add //ANGELAS059: add extern
extern void Lowpower_DISfunction(void);	//ANGELAS047:add //ANGELAS059: add extern
//extern void GC6_FBClamp(void);

typedef struct PowerSEQ
{
    FUNCT_PTR_B_V	func;	// Do step function
	WORD	delay;			// delay time (ms)
	BYTE	checkstatus;	// step function is check pin status (EX: slp_s3 HWPG ...)
} sPowerSEQ;

//MEILING002: S+
typedef struct PMICStruct
{
	BYTE  	cmd;
    XBYTE 	*smbdataA;
}sPMICStruct;

//address
#define powerICaddress 	0x68    
//command
#define VENDODID 		0x00  //default: 0x67
#define REVID 			0x01  //default: ---
#define SDWN 			0x49  //default: 0x00
#define EN_RW			0xD1  //default: 0xBF
#define EN_MASK			0xD2  //default: 0x00
#define EN_DEGL_SEL1	0xD3  //default: 0x00
#define EN_DEGL_SEL2    0xD4  //default: 0x00
#define PG_STATE 		0xD5  //default: 0x00
#define OVP_REG			0xD6  //default: 0x00
#define UVP_REG			0xD7  //default: 0x00
#define OCP_REG			0xD8  //default: 0x00
#define OTP_REG			0xD9  //default: 0x00
#define BUCK1CTRL		0xDA  //default: 0x24
#define BUCK2CTRL		0xDB  //default: 0x04
#define BUCK3CTRL		0xDC  //default: 0x32
#define LDO1CTRL		0xDD  //default: 0x1A
#define LDO2CTRL		0xDE  //default: 0x03
#define DISCHCTRL1		0xDF  //default: 0x14
#define DISCHCTRL2		0xE0  //default: 0x00
#define OC_CTRL			0xE1  //default: 0x26
#define BUCK_FSW		0xE2  //default: 0x01

extern void InitPMIC(void);
extern BYTE CheckInitPMIC(void);
extern void InitPMICReg(void);
extern BYTE PowerIICWriteByte(BYTE Reg_offset, BYTE PS_data);
extern void PollingOTP(void);
//MEILING002: E+

#endif
