 /*-----------------------------------------------------------------------------
 * Filename:OEM_MEMORY.C     For Chipset: ITE.IT85XX
 *
 * Function: Memory Definition for extern all code base reference
 *
 * [Memory Map Description]
 *
 * Chip Internal Ram : 0x00-0xFF For Kernel and Chip Level use
 *
 ***************************************************
 * Chip 8500       : External ram 0x000-0x7FF
 * Chip Other 85XX : External ram 0x000-0xFFF
 ***************************************************
 *
 * [OEM Memory Rang]
 * [External Ram]
 * 0x100-0x1FF   OEM RAM
 * 0x200-0x2FF   OEM RAM SPI buffer array
 * 0x300-0x3FF   OEM RAM EC Space
 * 0x400-0x4FF   OEM RAM
 * 0x500-0x5FF   OEM RAM
 * 0x600-0x6FF   OEM RAM

 * 0x800-0x8FF   OEM RAM keyboard matrix
 * 0x900-0x9FF   OEM RAM
 * 0xA00-0xAFF   OEM RAM
 * 0xB00-0xBFF   OEM RAM
 * 0xC00-0xCFF   OEM RAM
 * 0xD00-0xDFF   OEM RAM
 * 0xE00-0xEFF   OEM RAM
 * 0xF00-0xFFF   OEM RAM For HSPI ram code function
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/

#ifndef OEM_MEMORY_H
#define OEM_MEMORY_H

#define EC_RAMBase			0x0000

#define OEMRAM1				EC_RAMBase+0x0100   // EC_RAMBase+0x0100
#define OEMRAM2				EC_RAMBase+0x0200   // EC_RAMBase+0x0200
#define OEMRAM3				EC_RAMBase+0x0300   // EC_RAMBase+0x0300
#define OEMRAM4				EC_RAMBase+0x0400   // EC_RAMBase+0x0400
#define OEMRAM5				EC_RAMBase+0x0500   // EC_RAMBase+0x0500
#define OEMRAM6				EC_RAMBase+0x0600   // EC_RAMBase+0x0600

#define OEMRAM8				EC_RAMBase+0x0800   // EC_RAMBase+0x0800
#define OEMRAM9				EC_RAMBase+0x0900   // EC_RAMBase+0x0900
#define OEMRAMA				EC_RAMBase+0x0A00   // EC_RAMBase+0x0A00
#define OEMRAMB				EC_RAMBase+0x0B00   // EC_RAMBase+0x0B00
#define OEMRAMC				EC_RAMBase+0x0C00   // EC_RAMBase+0x0C00
#define OEMRAMD				EC_RAMBase+0x0D00   // EC_RAMBase+0x0D00
#define OEMRAME				EC_RAMBase+0x0E00   // EC_RAMBase+0x0E00
#define OEMRAMF				EC_RAMBase+0x0F00   // EC_RAMBase+0x0F00

#define EVENT_BUFFER_SIZE 8

//*******************************************************************************
//-------------------------------------------------------------------------------
//	0x100-0x1FF   OEM RAM 1
//-------------------------------------------------------------------------------
//*******************************************************************************
#define ECRAM1            	OEMRAM1

extern XBYTE	SysPowState;					//_at_(OEMRAM1+0x00);
extern XBYTE	PWSeqStep;			    		//_at_(OEMRAM1+0x01);
extern XBYTE	DeepSleepCunt;		    		//_at_(OEMRAM1+0x02);
extern XWORD    DelayDoPowerSeq;				//_at_(OEMRAM1+0x03);
extern XWORD    PowSeqDelay;					//_at_(OEMRAM1+0x05);

extern XBYTE	SBSWReleaseCunt;    //	_at_(OEMRAM1+0x07); //(byte)  //T067B+

extern XBYTE	Test109;						//_at_(OEMRAM1+0x09);
extern XBYTE    UART_DB_RAM;					//_at_(OEMRAM1+0x0B);
extern XBYTE	KB_OBF_count;					//_at_(OEMRAM1+0x0C);
extern XBYTE	UpdateLEDBL_delay;				//_at_(OEMRAM1+0x0D);
extern XBYTE	uVGATurboFun;					//_at_(OEMRAM1+0x0E);
#define 		uEnableGPS			BIT0
#define 		uDisPowerSteeringOK	BIT1
#define 		uDisVGATurboOK		BIT2
#define 		uDisCPUTurboOK		BIT3
#define 		uDisVGATurboD4OK		BIT4
#define 		uDisVGATurboD2OK		BIT5
#define 		uIntVGATurbo		BIT5 ///ANGELAS103:Add GPU turbo enable/disable Q_event.
//  #define 		uDisCPUTurboOK		BIT3   
//#define 		uEnableOK			BIT3
//#define 		uTruboLED_ON		BIT4
//#define 		uChagnedToQuiet		BIT5
//#define 		uExitFromQuiet		BIT6
//#define 		uReserve0E_bit7		BIT7

extern XBYTE   	CmdData2;						//_at_(ECRAM1+0x0F);

extern XBYTE  	BatSMbusFailCount;				//_at_(ECRAM1+0x10);
extern XBYTE  	Batpollstep1;					//_at_(ECRAM1+0x11);
extern XBYTE  	EC_oCCBQl;						//_at_(ECRAM1+0x12);
extern XBYTE  	EC_oCCBQh;						//_at_(ECRAM1+0x13);
extern XBYTE  	EC_oCBTl;						//_at_(ECRAM1+0x14);
extern XBYTE  	EC_oCBTh;						//_at_(ECRAM1+0x15);
extern XBYTE  	BAT1PERCL;						//_at_(ECRAM1+0x16);
extern XBYTE  	BAT1PERCH;						//_at_(ECRAM1+0x17);

extern XBYTE	uVPCeventSource;				//_at_(ECRAM1+0x18);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define FnQ                   BIT0
#define General                  BIT1
#define Inverter                 BIT2
#define Novo                     BIT3
#define Brightness               BIT4
#define TouchPad                 BIT5
#define Display                  BIT6
#define Camera                   BIT7

extern XBYTE	uVPCeventSource2;				//_at_(ECRAM1+0x19);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#define ODD_Eject                BIT0  //MEILING029:remove.
#define App_Control_Mic			 BIT0 //MEILING029:add.
#define AllRF                    BIT1
#define UserSelfDefine           BIT2
#define SwitchDisplayResolution  BIT3
#define Switch_EQ                BIT4
#define App_Control_RF           BIT5
#define GCPU_Notify              BIT6
#define ODD_Notify               BIT7

extern XBYTE	uNovoVPCCount;					//_at_(ECRAM1+0x1A);
extern XBYTE   	DebugCombineKey;				//_at_(ECRAM1+0x1B);
extern XBYTE	PowerLed_Count;					//_at_(ECRAM1+0x1C);
extern XBYTE    LED_KB_PWM_Step;				//_at_(ECRAM1+0x1D);

extern XBITS_8 	uReserve07;						//_at_(ECRAM1+0x1E);
#define uACInOutBeep			bit0
#define uE_PanelOK				bit1
#define nPmeWakeEN				bit2
#define nPmeWakeDIS				bit3
#define nTPDriverIn				bit4
#define nVPCDriverIn			bit5
#define nFanManual				bit6
#define b7Fan2Manual			bit7

//MARTINH154:Add : start
//#define    b0_CMOS_FunctionKey     BIT0 //ANGELAS052:remove
//#define    b1_CMOS_delay1flag      BIT1 //ANGELAS052:remove
//MARTINH154:Add : end

extern XBYTE  	CombineKeyStatus;				//_at_(ECRAM1+0x1F);
#define b0FnRN					BIT0
#define b1Win_Q_Esc				BIT1

extern XBYTE  	CHGIC_SMbusFailCnt;				//_at_(ECRAM1+0x20);
extern XBYTE  	CHGIC_ptr;						//_at_(ECRAM1+0x21);
extern XBYTE  	CHGIC_InputCurrentL;			//_at_(ECRAM1+0x22);
extern XBYTE  	CHGIC_InputCurrentH;			//_at_(ECRAM1+0x23);
extern XBYTE  	CHGIC_ReadCmd0x14L;				//_at_(ECRAM1+0x24);
extern XBYTE  	CHGIC_ReadCmd0x14H;				//_at_(ECRAM1+0x25);
extern XBYTE  	CHGIC_ReadCmd0x15L;				//_at_(ECRAM1+0x26);
extern XBYTE  	CHGIC_ReadCmd0x15H;				//_at_(ECRAM1+0x27);
extern XBYTE  	CHGIC_ReadCmd0x3FL;				//_at_(ECRAM1+0x28);
extern XBYTE  	CHGIC_ReadCmd0x3FH;				//_at_(ECRAM1+0x29);
extern XWORD  	BATTEMP;						//_at_(ECRAM1+0x2A);
extern XBYTE   	EC_C_modeL;						//_at_(ECRAM1+0x2C);
extern XBYTE	EC_C_modeH;						//_at_(ECRAM1+0x2D);
extern XBYTE	Charge_current_flag;					//_at_(ECRAM1+0x2e);//72JERRY066:Modify charge current vlave for power limit.
#define       b0overpower      BIT0
extern XBYTE	AdapterID;						//_at_(ECRAM1+0x2F);

extern XBYTE	eFlashVarifyOK;					//_at_(ECRAM1+0x30);
extern XBYTE	eFlash_r_data;					//_at_(ECRAM1+0x31);
extern XBYTE	eFlashA1;						//_at_(ECRAM1+0x32);
extern XBYTE	eFlashA0;						//_at_(ECRAM1+0x33);
extern XBYTE	eEEPROMBank;					//_at_(ECRAM1+0x34);
extern XBYTE	eEEPROMAddrsss;					//_at_(ECRAM1+0x35);
extern XBYTE   	eEEPROMData;					//_at_(ECRAM1+0x36);
extern XBYTE	eEEPROMMarkData;				//_at_(ECRAM1+0x37);
extern XBYTE    e256ByteCnt;					//_at_(ECRAM1+0x38);
extern XBYTE   	eUpdateEEPROMCnt;				//_at_(ECRAM1+0x39);
extern XBYTE   	eUpdatepDevStusCnt;				//_at_(ECRAM1+0x3A);
extern XBYTE  	BatLowCnt;						//_at_(ECRAM1+0x3B);
extern XBYTE  	WSMbusTemp01;					//_at_(ECRAM1+0x3C);
extern XBYTE  	WSMbusTemp02;					//_at_(ECRAM1+0x3D);
extern XBYTE  	SMbusFailCnt2;					//_at_(ECRAM1+0x3E);
extern XBYTE  	SMbusFailCnt3;					//_at_(ECRAM1+0x3F);

extern XBYTE   	CombineKeyRN;					//_at_(ECRAM1+0x40);
extern XBYTE    EscScanCodeDelay;				//_at_(ECRAM1+0x41);
extern XWORD  	API_ID;							//_at_(ECRAM1+0x42); 
extern XBYTE    SMbusFailCnt4;                   //_at_(ECRAM1+0x44);// Start Shipmode disable 5s loop once 20130608 20:28 

typedef enum
{
	Chk_Trickle_Current_status_normal,
	Chk_Trickle_Current_status_error
}Chk_Trickle_Current_status_type;
extern XBYTE	Chk_Trickle_Current_status;		// _at_(ECRAM1+0x45);
extern XBYTE	Chk_Trickle_Current_count;		// _at_(ECRAM1+0x46);
extern XWORD	Chk_Trickle_Current_Now_Current;// _at_(ECRAM1+0x47); //0x47~0x48
extern XWORD	Chk_Trickle_Current_Chg_Current;// _at_(ECRAM1+0x49); //0x49~0x4A
extern XBYTE  	Get_Batt_debounce_count;		// _at_(ECRAM1+0x4B);
extern XBYTE  	Get_Batt_debounce_hash1;		// _at_(ECRAM1+0x4C);
extern XBYTE  	Get_Batt_debounce_hash2;		// _at_(ECRAM1+0x4D);
extern XBYTE   	CombineKeyIO;					// _at_(ECRAM1+0x4E);
extern XBYTE   	GPIO_STATUS;					// _at_(ECRAM1+0x4F);
#define GPIO_RSMRST				BIT0			// PB.7 RSMRST.
#define GPIO_PBTN_OUT			BIT1			// PF.1 PBTN_OUT.
#define GPIO_VDDQ_PGOOD			BIT2			// PI.1 VDDQ_PGOOD
#define GPIO_PM_SLP_S3			BIT3			// PH.1 PM_SLP_S3
#define GPIO_PM_SLP_S4			BIT4			// PH.2 PM_SLP_S4
#define GPIO_PM_SLP_S5			BIT5			// Px.x PM_SLP_S5
#define GPIO_VR_ON				BIT6			// PE.3 VR_ON
#define GPIO_PCH_PWROK			BIT7			// PH.6 PCH_PWROK

extern XBYTE	Bat0x0BTempL;					//_at_(ECRAM1+0x50);
extern XBYTE	Bat0x0BTempH;					//_at_(ECRAM1+0x51);
extern XBYTE   	Bat0x0BFakeCnt;					//_at_(ECRAM1+0x52);
extern XBYTE   	Bat0x08OTPCnt;					//_at_(ECRAM1+0x53);
extern XBYTE   	S3ResumeRSOC;					//_at_(ECRAM1+0x54);
extern XBYTE	BatteryAlarm;					//_at_(ECRAM1+0x55);
#define BATOCP					BIT0
#define BATOTP					BIT1
#define ACSTPP					BIT2
#define BATPercl_5				BIT3
#define HybridSTPP				BIT4
#define BATLOWVOL				BIT5  

extern XBYTE   	CHGIC_ReadCmd0x12L;				//_at_(ECRAM1+0x56);
#define ChargeInhibit				BIT0
//#define BoostIndication				BIT2  //ANGELAS016
//#define TurboBoost					BIT3 //ANGELAS016
#define IDCHG_GAIN				BIT3 //ANGELAS016:Change charge IC option setting.
#define IADP_GAIN				BIT4 //ANGELAS016:Change charge IC option setting.
#define BatLearnEnable			BIT5 //JERRYCRZ010:Through the charger IC option control battery learn mode.
#define IFAULT_LOW					BIT7 //ANGELAS016
 
extern XBYTE   	CHGIC_ReadCmd0x12H;				//_at_(ECRAM1+0x57);
//#define IFAULT_HI					BIT0 //ANGELAS016
//#define ACOKDeglitchTime        		BIT7 //ANGELAS016
//ANGELAS016:S+ Change charge IC option setting.
#define PWM_FREQ				BIT0
#define PWM_FREQ1				BIT1
#define WDTMR_ADJ				BIT5
#define WDTMR_ADJ1				BIT6
#define LOWPWR_EN				BIT7
//ANGELAS016:E+ Change charge IC option setting.

extern XBYTE    Bat0x3ETempL;					//_at_(ECRAM1+0x58);
extern XBYTE    Bat0x3ETempH;					//_at_(ECRAM1+0x59);
extern XBYTE	EC_BatteryStatusL;				//_at_(ECRAM1+0x5A);
extern XBYTE	EC_BatteryStatusH;				//_at_(ECRAM1+0x5B);
extern XBYTE    BattTemp;						//_at_(ECRAM1+0x5C);
extern XBYTE	BatteryOTPRelease;				//_at_(ECRAM1+0x5D);
extern XBYTE	BatteryOTP;						//_at_(ECRAM1+0x5E);
extern XBYTE	BatteryOTPShutdown;				//_at_(ECRAM1+0x5F);

extern XBYTE	FAN_Tab_OFF;					//_at_(ECRAM1+0x60);
extern XBYTE	FAN_Tab_LOW;					//_at_(ECRAM1+0x61);
extern XBYTE	FAN_Tab_Step1;					//_at_(ECRAM1+0x62);
extern XBYTE	FAN_Tab_Step2;					//_at_(ECRAM1+0x63);
extern XBYTE	FAN_Tab_HI;						//_at_(ECRAM1+0x64);
extern XBYTE	THR_PRO_OFF;					//_at_(ECRAM1+0x65);
extern XBYTE	THR_PRO_ON;						//_at_(ECRAM1+0x66);
extern XBYTE	THR_Turo_Rem;					//_at_(ECRAM1+0x67);
extern XBYTE	THR_Turo_OFF;					//_at_(ECRAM1+0x68);
extern XBYTE	THR_Tab_Shut;					//_at_(ECRAM1+0x69);
extern XBYTE	KB_S3Dly;						//_at_(ECRAM1+0x6A);
extern XBYTE	SLI_FAN_Tab_OFF;				//_at_(ECRAM1+0x6B);
extern XBYTE	SLI_FAN_Tab_LOW;				//_at_(ECRAM1+0x6C);
extern XBYTE	SLI_FAN_Tab_Step1;				//_at_(ECRAM1+0x6D);
extern XBYTE	SLI_FAN_Tab_Step2;				//_at_(ECRAM1+0x6E);
extern XBYTE	SLI_FAN_Tab_HI;					//_at_(ECRAM1+0x6F);

extern XBYTE	VFAN_Tab_OFF;					//_at_(ECRAM1+0x70);
extern XBYTE	VFAN_Tab_LOW;					//_at_(ECRAM1+0x71);
extern XBYTE	VFAN_Tab_Step1;					//_at_(ECRAM1+0x72);
extern XBYTE	VFAN_Tab_Step2;					//_at_(ECRAM1+0x73);
extern XBYTE	VFAN_Tab_HI;					//_at_(ECRAM1+0x74);
extern XBYTE	VTHR_PRO_OFF;					//_at_(ECRAM1+0x75);
extern XBYTE	VTHR_PRO_ON;					//_at_(ECRAM1+0x76);
extern XBYTE	VTHR_Turo_Rem;					//_at_(ECRAM1+0x77);
extern XBYTE	VTHR_Turo_OFF;					//_at_(ECRAM1+0x78);
extern XBYTE	VTHR_Tab_Shut;					//_at_(ECRAM1+0x79);


extern XBYTE	SLI_THR_PRO_OFF;				//_at_(ECRAM1+0x7B);
extern XBYTE	SLI_THR_PRO_ON;					//_at_(ECRAM1+0x7C);
extern XBYTE	SLI_THR_Turo_Rem;				//_at_(ECRAM1+0x7D);
extern XBYTE	SLI_THR_Turo_OFF;				//_at_(ECRAM1+0x7E);
extern XBYTE	SLI_THR_Tab_Shut;				//_at_(ECRAM1+0x7F);

extern XBYTE	inhibit2sec; 					//_at_(ECRAM1+0x80);
extern XBYTE	Battdata_ready; 				//_at_(ECRAM1+0x81);
extern WORD		TrickleChgTimeOutCnt;			//_at_(ECRAM1+0x82);//0x82,0x83
extern WORD		FastChgTimeOutCnt;				//_at_(ECRAM1+0x84);//0x84,0x85
extern XBYTE	RSMshutdownCnt;					//_at_(ECRAM1+0x86);
typedef enum
{
	ChkBattery_abnormal_status_normal,
	ChkBattery_abnormal_status_wait5sec,
	ChkBattery_abnormal_status_error
}ChkBattery_abnormal_status_type;
extern XBYTE	ChkBattery_abnormal_status;		//_at_(ECRAM1+0x87);
extern XBYTE	ChkBattery_abnormal_count;		//_at_(ECRAM1+0x88);
extern XBYTE	LOWBATT_3TIMES; 				//_at_(ECRAM1+0x89);

extern XBYTE	Bat0x00TempL; 					//_at_(ECRAM1+0x8C);
extern XBYTE	Bat0x00TempH; 					//_at_(ECRAM1+0x8D);
extern XBYTE	Bat0x0FTempL; 					//_at_(ECRAM1+0x8E);
extern XBYTE	Bat0x0FTempH; 					//_at_(ECRAM1+0x8F);

typedef enum
{
	Chk_Wrong_ADP_Status_Null,
	Chk_Wrong_ADP_Status_wait_2sec,
	Chk_Wrong_ADP_Status_wait_10sec,
	Chk_Wrong_ADP_Status_ACOFF_Lock
}Chk_Wrong_ADP_Status_Type;
extern XBYTE	Chk_Wrong_ADP_Status;			//_at_(ECRAM1+0x90);
extern XBYTE   ACIN_FallINT_Count;				//_at_(ECRAM1+0x91);
extern XWORD Chk_Wrong_10ms_Count;				//_at_(ECRAM1+0x92);
typedef enum
{
	Chk_ACOP_Status_Null,
	Chk_ACOP_Status_wait_10sec,
	Chk_ACOP_Status_ACOFF_Lock
}Chk_ACOP_Status_Type;
extern XBYTE	Chk_ACOP_Status;				//_at_(ECRAM1+0x94);
extern XWORD	Chk_ACOP_Bat_Chg_Current;		//using integer		//_at_(ECRAM1+0x95);
extern XWORD	Chk_ACOP_10ms_Count;			//_at_(ECRAM1+0x97);

typedef enum
{
	Chk_Hybrid_STPP_Status_CP,
	Chk_Hybrid_STPP_Status_Charger_Turbo,
	Chk_Hybrid_STPP_Status_Disable_CPUTurbo,
	Chk_Hybrid_STPP_Status_CPU_throttling,
	Chk_Hybrid_STPP_Status_Recover_CPU_throttling,
	Chk_Hybrid_STPP_Status_Recover_Charger_Turbo
}Chk_Hybrid_STPP_Status_Type;
extern XBYTE	Chk_Hybrid_STPP_Status;				//_at_(ECRAM1+0x99);
extern XBYTE	Chk_Hybrid_STPP_min_BattGasgauge;	//_at_(ECRAM1+0x9A);
extern XWORD Chk_Hybrid_STPP_Batt_Output_Power;		//_at_(ECRAM1+0x9B);
extern XWORD Chk_Hybrid_STPP_Batt_Output_Power_Limit;		//_at_(ECRAM1+0x9D);
extern XBYTE	BackLight_En_Delay;				//_at_(ECRAM1+0x9F);

extern XBYTE	UCS1002ID;						//_at_(ECRAM1+0xA0);
extern XWORD	Chk_Hybrid_STPP_Turboboost_Battery_Current_limit;	//_at_(ECRAM1+0xA2);

//ANGELAS016:s+Change charge IC option setting.
extern XBYTE	CHGIC_ReadCmd0x37L;						//_at_(ECRAM1+0xA4);
//#define BoostIndication				BIT1 //ANGELAS053: BIT2 TO BIT1
//#define TurboBoost					BIT2 //ANGELAS053: BIT3 TO BIT2
//#define IFAULT_LOW					BIT6
//#define IFAULT_HI					BIT7
extern XBYTE	CHGIC_ReadCmd0x37H;						//_at_(ECRAM1+0xA5);
#define ACOKDeglitchTime        		BIT4
//ANGELAS016:e+Change charge IC option setting.

extern XWORD	ADPI_Data[4];					//_at_(ECRAM1+0xA8);//0xA8~0xAF

//ANGELAS016:s+Change charge IC option setting.
extern XBYTE	chargerInitIndex;        				//_at_(ECRAM1+0xB0);
extern XBYTE   CHGIC_ReadCmd0x3BL;						//_at_(ECRAM1+0xB1);
extern XBYTE   CHGIC_ReadCmd0x3BH;						//_at_(ECRAM1+0xB2);
extern XBYTE	CHGIC_ReadCmd0x3CL;						//_at_(ECRAM1+0xB3);
extern XBYTE	CHGIC_ReadCmd0x3CH;						//_at_(ECRAM1+0xB4);
extern XBYTE	CHGIC_ReadCmd0x3DL;						//_at_(ECRAM1+0xB5);
extern XBYTE	CHGIC_ReadCmd0x3DH;						//_at_(ECRAM1+0xB6);
extern XBYTE	CHGIC_ReadCmd0x39L;						//_at_(ECRAM1+0xB7);
extern XBYTE	CHGIC_ReadCmd0x39H;						//_at_(ECRAM1+0xB8);
extern XBYTE	CHGIC_WriteCmd0x38L;						//_at_(ECRAM1+0xB9);
extern XBYTE	CHGIC_WriteCmd0x38H;						//_at_(ECRAM1+0xBA);
extern XBYTE	CHGIC_WriteCmd0x37L;						//_at_(ECRAM1+0xBB);

#define TurboBoost					BIT2 //ANGELAS053: BIT3 TO BIT2

extern XBYTE	CHGIC_WriteCmd0x37H;						//_at_(ECRAM1+0xBC);
extern XBYTE	CHGIC_WriteCmd0x12L;						//_at_(ECRAM1+0xBD);
extern XBYTE	CHGIC_WriteCmd0x12H;						//_at_(ECRAM1+0xBE);
//ANGELAS016:e+Change charge IC option setting.

extern XWORD	XWTemp1;						//_at_(ECRAM1+0xC0);
extern XWORD	OCPCapacityRelease;				//_at_(ECRAM1+0xC2);
extern XWORD    OCPCapacity;					//_at_(ECRAM1+0xC4);
extern XBYTE	ECBIOSVersionH;					//_at_(ECRAM1+0xC6);
extern XBYTE	ECBIOSVersionL;					//_at_(ECRAM1+0xC7);
extern XBYTE    ECBIOSUbSVersion;				//_at_(ECRAM1+0xC8);

extern XBYTE    ADPIDON10MS_NUM;				//_at_(ECRAM1+0xC9); //T06A +
extern XBYTE    TEST_FLAG;					    //_at_(ECRAM1+0xCA); //T14K +
#define Fan_Control_Auto      BIT7			    //T14K+

extern XBYTE    AdapterIDOn_Flag;               //_at_(ECRAM1+0xCB);  //MEILING001:add.
extern XBYTE    ResetTestFlag;                  //_at_(ECRAM1+0xCC);  //MEILING009:add.
extern XBYTE    CPUProchotONCnt;                //_at_(ECRAM1+0xCD);  //MEILING033:add.  //MEILING055:modify name from ProchotONCnt to CPUProchotONCnt.
extern XBYTE    GPUProchotDelay;              	//_at_(ECRAM1+0xCE);  //MEILING042:add.


extern XBYTE	ADPI2Sec;						//_at_(ECRAM1+0xCF);
extern XWORD	TurboBoostCP;					//_at_(ECRAM1+0xD0);
extern XWORD	TurboBoostEn;					//_at_(ECRAM1+0xD2);
extern XWORD	TurboBoostthrottlEn;			//_at_(ECRAM1+0xD4);
extern XWORD	TurboBoostthrottlDis;			//_at_(ECRAM1+0xD6);
extern XWORD	TurboBoostDis;					//_at_(ECRAM1+0xD8);
extern XWORD   	ACModeSTPPEn;					//_at_(ECRAM1+0xDA);
extern XWORD   	ACModeSTPPDis;					//_at_(ECRAM1+0xDC);
extern XWORD	ADPI_AvgData;					//_at_(ECRAM1+0xDE);

extern XBYTE    POWER_FLAG1;            		//_at_ OEMRAM1+0xE0;	//EC power flag 1
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define adapter_in      	BIT0  	    		// 1: AC adapter exist
#define wait_PSW_off    	BIT1  	    		// 1: waiting for PSW released
#define wati_NOVO_off		BIT2				// 1: waiting for NOVO release
#define KB_LED_Status		BIT3				// 1:
#define LAN_WAKE_Status		BIT4				//
#define Lid_Act             BIT5                //change LID implement method.

/*  BATTERYS INFORMATION    */
extern XBYTE    BT1_STATUS1;            		//_at_ OEMRAM1+0xE1;	//Battery 1 status1 flag
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define bat_in         		BIT0	    		// 1: Battery 1 exist
#define bat_valid           BIT1   	    		// 1: Battery 1 battery idetify OK
#define bat_InCharge   		BIT2   	    		// 1: battery 1 in any charge phase
#define bat_Full            BIT3   	    		// 1: Battery 1 fully charged
#define bat_Dischg     		BIT4   	    		// 1: Battery 1 in discharging
#define bat_Dead       		BIT5   	    		// 1: Battery 1 fully discharged
#define bat_abnormal        BIT6   	    		// 1: battery 1 abnormal

extern XBYTE    EVT_STATUS1;            		//_at_ OEMRAM1+0xE2;	//Event status flag 1
#define DummyFlag           BIT7        		// 1:CRT Sense
#define Lid_PAD_Act         BIT5                //MARTINM026: Add

extern XWORD	PSW_COUNTER;            		//_at_ OEMRAM1+0xE3;	//(word)

extern XBYTE   	DSxPowState;					//_at_ OEMRAM1+0xE5; 	//(byte)
extern XBYTE   	DS3PowSeqStep;			    	//_at_ OEMRAM1+0xE6;
extern XWORD   	DS3PowSeqDelay;					//_at_ OEMRAM1+0xE7; 	//(word)
extern XBYTE   	DRAMRST_CNTRL_CNT;				//_at_ OEMRAM1+0xE9;
//extern XWORD	SUSACK_LOW_CNT;					//_at_ OEMRAM1+0xEA;
//extern XBYTE   	SUSACK_HI_CNT;					//_at_ OEMRAM1+0xEC;
extern XBYTE	DRAMRST_CNTRL_LOW_CNT;			//_at_ OEMRAM1+0xED;
//extern XBYTE   	S3S4DelayCnt;					//_at_ OEMRAM1+0xEE; //ANGELAS012:Remove S3 delay.
extern XBYTE   	S3S4DelayCnt;					//_at_ OEMRAM1+0xEE; //ANGELAS063:add
extern XBYTE    CPUThrottlingDelayTime;         //_at_(ECRAM1+0xEF); //MEILING052:add.

#define DebounceRAM     OEMRAM1+0xF0
extern XBYTE	DEBOUNCE_CONT1;		    		//(DebounceRAM+0x00) 	//(byte)
extern XBYTE	DEBOUNCE_CONT2;		    		//(DebounceRAM+0x01) 	//(byte)
extern XBYTE	DEBOUNCE_CONT3;		    		//(DebounceRAM+0x02) 	//(byte)
extern XBYTE	DEBOUNCE_CONT4;		    		//(DebounceRAM+0x03) 	//(byte)
extern XBYTE	DEBOUNCE_CONT5;		    		//(DebounceRAM+0x04) 	//(byte)
extern XBYTE	DEBOUNCE_CONT6;		    		//(DebounceRAM+0x05) 	//(byte)
extern XBYTE	DEBOUNCE_CONT7;		    		//(DebounceRAM+0x06) 	//(byte)
extern XBYTE	DEBOUNCE_CONT8;		    		//(DebounceRAM+0x07) 	//(byte)
extern XBYTE	DEBOUNCE_CONT9;		    		//(DebounceRAM+0x08) 	//(byte)
extern XBYTE	DEBOUNCE_CONT10;				//(DebounceRAM+0x09) 	//(byte)
extern XBYTE	DEBOUNCE_CONT11;				//(DebounceRAM+0x0A) 	//(byte)
extern XBYTE	DEBOUNCE_CONT12;				//(DebounceRAM+0x0B) 	//(byte)
extern XBYTE	DEBOUNCE_CONT13;				//(DebounceRAM+0x0C) 	//(byte)
extern XBYTE	DEBOUNCE_CONT14;				//(DebounceRAM+0x0D) 	//(byte)
extern XBYTE	DEBOUNCE_CONT15;				//(DebounceRAM+0x0E) 	//(byte)
extern XBYTE	DEBOUNCE_CONT16;				//(DebounceRAM+0x0F) 	//(byte)

//*******************************************************************************
//-------------------------------------------------------------------------------
//	0x200-0x2FF   OEM RAM 2
//-------------------------------------------------------------------------------
//*******************************************************************************
#define ECRAM2            OEMRAM2
extern XBYTE	pProject0;					//_at_ OEMRAM2+0x02
#define b0DispToggleEn			BIT0
//ANGELAS033:Add start
#define b1uFUdayClr			    BIT1
#define b2uFUdayFail            BIT2
#define b3uBatteryTimeNoOK      BIT3
//ANGELAS033:Add end
#define b4VGAType				BIT4
#define b5FUBIOSWriteReady      BIT5   //ANGELAS033:Add
#define b6PostHotKey			BIT6

#define b7GSKL310OR510			BIT7//MEILING050:Distinguish between 310 or 510.

extern XBYTE	pDevStus;					//_at_ OEMRAM2+0x03 203
	#define	pKillSwitch			BIT0		// WLan & Bluetooth Button status
	#define	pWireless			BIT1		// Wireless status
	#define	pBluetooth			BIT2		// Bluetooth status
	#define	pENABLE_TP			BIT3		// 1 : Enable TP, 0 : Disable TP
	#define	pCamera				BIT4
	#define	pKBLight			BIT5
	#define	p3GDevice			BIT6
	#define	pSwitchableBTN		BIT7
	
extern XBYTE	BatSmartChargeL;		//_at_ OEMRAM2+0x04
	#define  b0QuickChargeSupport       BIT0       
	#define  b1QuickChargeEnable    	BIT1
extern XBYTE	BatSmartChargeH;		//_at_ OEMRAM2+0x05

extern XBYTE  batteryChemistry[4];          //_at_ OEMRAM2+06 //ANGELAS107:Add code for GBSI function.
#define	pReserve0B_bit0			BIT0
#define	pTheaterKey				BIT1
#define	pSecurityKey			BIT2
#define	pUtralPwrSavKey			BIT3		//Power scheme switch button
#define	pReserve13_bit4			BIT4		//Lenovo security essentials
#define	pEMturboBtn				BIT5		//Use this to notify EM when user push turbo button.

extern XBYTE pPROCHOT;						//_at_ OEMRAM2+0x0C;
#define	b0Thermal_PRCOHOTon		BIT0
#define b1ISCT_PROCHOTon		BIT1

extern XBYTE	pProject4;				//at_(OEMRAM2+0x0D)//HEGANGS028:Disable POWERSW when flash bios
#define  pPWSWdisable      BIT0         //HEGANGS028:Disable POWERSW when flash bios

extern XBYTE SMartNoise;					//_at_ OEMRAM2+0x11;
#define	b0FanCleanOn			BIT0
#define b1FanCleanStart			BIT1
#define b2StopReason			BIT2
#define	b4SmartNoiseDo			BIT4
#define b5DedustingMode			BIT5
#define b6SNBTN					BIT6
#define b7SMartNoiseOn			BIT7 		// Quite mode = power saver

extern XBYTE	uPGID;						//_at_ OEMRAM2+0x14; //Add the difference between G and Z project

extern XBYTE	uMBID;						//_at_ OEMRAM2+0x15;
extern XBYTE	MBID_READY;					//_at_ OEMRAM2+0x16; 
extern XBYTE	uMBGPU;						//_at_ OEMRAM2+0x17;
extern XBYTE	uMBpw;						//_at_ OEMRAM2+0x18;
extern XBYTE	uPJID;						//_at_ OEMRAM2+0x19;

extern XBYTE	BAT1_MD_1;			                //_at_(OEMRAM2+0x20);
extern XBYTE	BAT1_MD_2;			                //_at_(OEMRAM2+0x21);
extern XBYTE	BAT1_MD_3;			                //_at_(OEMRAM2+0x22);
extern XBYTE	BAT1_MD_4;			                //_at_(OEMRAM2+0x23);
extern XBYTE	BAT1_MD_5;			                // _at_(OEMRAM2+0x24);
extern XBYTE	BAT1_MD_6;			                //_at_(OEMRAM2+0x25);
extern XBYTE	BAT1_MD_7;			                //_at_(OEMRAM2+0x26);
extern XBYTE	BAT1_MD_8;			                //_at_(OEMRAM2+0x27);
extern XBYTE	BAT1_MD_9;			                //_at_(OEMRAM2+0x28);
extern XBYTE	BAT1_MD_A;			                // _at_(OEMRAM2+0x29);
extern XBYTE	BAT1_MD_B;			                //_at_(OEMRAM2+0x2A);
extern XBYTE	BAT1_MD_C;			                // _at_(OEMRAM2+0x2B);
extern XBYTE	BAT1_MD_D;			                // _at_(OEMRAM2+0x2C);
extern XBYTE	BAT1_MD_E;			                //_at_(OEMRAM2+0x2D);

extern XBYTE  	BAT1_Bar_Code[32];                  //_at_(OEMRAM2+0x2E); //2E~4D  //MEILING031:add. 

extern XBYTE    UCSI_TUNNEL_Buffer[];               //_at_(OEMRAM2+0x4F); //4E~5F //72JERRY060


extern XBYTE	StartFanClean;				//_at_ OEMRAM2+0x61;
extern XBYTE	FanCleanFull;				//_at_ OEMRAM2+0x62;
extern XBYTE	FanCleanHalt;				//_at_ OEMRAM2+0x63;

extern XBYTE	pModuleID;					//_at_ OEMRAM2+0x66
extern XBYTE	pLastSID;					//_at_ OEMRAM2+0x67;
extern XBYTE	pLastSID2;					//_at_ OEMRAM2+0x68;
extern XBYTE	pLastSID3;					//_at_ OEMRAM2+0x69;
extern XBYTE	pLastSID4;					//_at_ OEMRAM2+0x6A;
////72JERRY008:s+
extern XBYTE  AOURSTCNT;             //_at_ OEMRAM2+0x70;
  
extern XBYTE  EC_AOU_DBG_FLAG;       //_at_ OEMRAM2+0x71;    
  // BIT6:0 used for setting battery level for testing.
 #define AOU_DC_LEVEL_DBG    BIT7
extern XBYTE  EC_AOU_PWR_TYPE;       //_at_ OEMRAM2+0x72;    
  // BIT7:4 AOU port power mode
  // BIT3:0 Non-AOU port power mode
  ////72JERRY008:e+
extern XBYTE	cPacketPowerLimit1;			// _at_ OEMRAM2+0x82
extern XWORD	KeepBattRemineCap;			// _at_ OEMRAM2+0x83
extern XBYTE	ChkBattery_FCCchg_count;	//_at_ OEMRAM2+0x85;
extern XBYTE	ChkBattery_FCCchg_lastFCCL;	//_at_ OEMRAM2+0x86;
extern XBYTE	ChkBattery_FCCchg_lastFCCH;	//_at_ OEMRAM2+0x87;
extern XBYTE	CountSecAfterPswPressed;	//_at_ OEMRAM2+0x88;
extern XBYTE	CountSecAfterNOVOPressed;	//_at_ OEMRAM2+0x89;

extern XBYTE	pVCompareFlag;				//_at_ OEMRAM2+0x8A;
#define VC0TriggerOn				BIT0
#define VC1TriggerOn				BIT1
#define VC2TriggerOn				BIT2
extern XWORD	NTC_V2;						//_at_OEMRAM2+0x8B;

//start for save P80 code to setup. 
extern XBYTE P80CMOS[];                     //_at_ OEMRAM2+0x90;
extern XBYTE P80Index;                      //_at_ OEMRAM2+0x97;
extern XBYTE P80CMOSSts;                    //_at_ OEMRAM2+0xA0;
#define P80CMOSDis                  BIT0
//end for save P80 code to setup. 
extern XWORD  Read_VR_CPU_PWROK;           //_at_ OEMRAM2+0xb0;
extern XBYTE CombineKeyShip;               //_at_ OEMRAM2+0xb3; 
extern XBYTE CombineKeyShip1;              //_at_ OEMRAM2+0xb4; 
extern LWORD CalcBatRCC;                   //_at_ OEMRAM2+0xb5; 

extern XBYTE PWR_LED_Debug ;                //    _at_(OEMRAM2+0xba);//HEGANG0022:Add smi hang check

//MEILING002: add start
extern XBYTE PMIC_counter;    		// _at_  OEMRAM2+0xC5;
extern XBYTE OTP_TEMP;    			// _at_  OEMRAM2+0xC6;
extern XBYTE PowerInitOK;       	//_at_   OEMRAM2+0xC7;
extern XBYTE PowerInitErrorCMD;     //_at_   OEMRAM2+0xC8;
extern XBYTE PowerInitErrorDATA;    //_at_   OEMRAM2+0xC9;
extern XBYTE PowerInitError;        //_at_   OEMRAM2+0xCA;
extern XBYTE PowerInitIndex;        //_at_   OEMRAM2+0xCB;
extern XBYTE PowerReturnData;    	// _at_  OEMRAM2+0xCC;

extern XBYTE PowerVENDODID;    		// _at_  OEMRAM2+0xCD;
extern XBYTE PowerREVID;    		// _at_  OEMRAM2+0xCE;
extern XBYTE PowerSDWN;    			// _at_  OEMRAM2+0xCF;
extern XBYTE PowerEN_RW;    		// _at_  OEMRAM2+0xD0;
extern XBYTE PowerEN_MASK;   	 	// _at_  OEMRAM2+0xD1;
extern XBYTE PowerEN_DEGL_SEL1;     // _at_  OEMRAM2+0xD2;
extern XBYTE PowerEN_DEGL_SEL2;     // _at_  OEMRAM2+0xD3;
extern XBYTE PowerPG_STATE;   		// _at_  OEMRAM2+0xD4;
extern XBYTE PowerOVP_REG;   		// _at_  OEMRAM2+0xD5;
extern XBYTE PowerUVP_REG;    		// _at_  OEMRAM2+0xD6;
extern XBYTE PowerOCP_REG;    		// _at_  OEMRAM2+0xD7;
extern XBYTE PowerOTP_REG;    		// _at_  OEMRAM2+0xD8;
extern XBYTE PowerBUCK1CTRL;    	// _at_  OEMRAM2+0xD9;
extern XBYTE PowerBUCK2CTRL;    	// _at_  OEMRAM2+0xDA;
extern XBYTE PowerBUCK3CTRL;    	// _at_  OEMRAM2+0xDB;
extern XBYTE PowerLDO1CTRL;    		// _at_  OEMRAM2+0xDC;
extern XBYTE PowerLDO2CTRL;   		// _at_  OEMRAM2+0xDD;
extern XBYTE PowerDISCHCTRL1;    	// _at_  OEMRAM2+0xDD;
extern XBYTE PowerDISCHCTRL2;    	// _at_  OEMRAM2+0xDF;
extern XBYTE PowerOC_CTRL;    		// _at_  OEMRAM2+0xE0;
extern XBYTE PowerBUCK_FSW;    		// _at_  OEMRAM2+0xE1;
//MEILING002: add end

extern XBYTE CellCount;    			// _at_  OEMRAM2+0xE5; //MEILING017: add

extern XBYTE BAT_LED_Cnt_ON;        //_at_(OEMRAM2+0xE6);   //MEILING030:add.
extern XBYTE BAT_LED_Cnt_OFF;       //_at_(OEMRAM2+0xE7);   //MEILING030:add.
extern XBYTE BAT_LOW_LED_Cnt;       //_at_(OEMRAM2+0xE8);   //MEILING030:add.

extern XBYTE	LENOVOBATT2;						//_at_(OEMRAM2+0xF0);		////ANGELAG017: add
#define BATTERY_OCP		BIT0				// 1: Battery OCP //ANGELAG017: add

//*******************************************************************************
//-------------------------------------------------------------------------------
//	0x300-0x3FF   OEM RAM 3
//-------------------------------------------------------------------------------
//*******************************************************************************
extern XBYTE SYS_MISC1;								//_at_ OEMRAM3+0x00;
#define 	b1Num_LED		BIT1
#define 	b2Cap_LED		BIT2
#define 	BATCRISIS_FLAG		BIT3 
#define 	b5Crisis_LED	BIT5
#define 	Crisis_On		BIT6
#define 	ACPI_OS			BIT7

extern XBYTE PWM_LEDBeep_CNT;						//_at_ OEMRAM3+0x01;
extern XBYTE AutoTimer;								//_at_ OEMRAM3+0x02;
#define 	b7AutoBootTm	BIT7
extern XBYTE uWOVSTATUS;						//_at_ OEMRAM3+0x03;//72JERRY032: Add voice wake up cmd for BIOS.
extern XBYTE 	QEVENT_DEBUG;					    //_at_ OEMRAM3+0x05; 

extern XBYTE SCI_Event_In_Index;					//_at_ OEMRAM3+0x06;
extern XBYTE SCI_Event_Out_Index;					//_at_ OEMRAM3+0x07;
extern XBYTE SCI_Event_Buffer[EVENT_BUFFER_SIZE];	//_at_ OEMRAM3+0x08;
extern XWORD ADP_I;									//_at_ OEMRAM3+0x10;
extern XWORD NTC_V1;								//_at_ OEMRAM3+0x12;

extern XWORD	NOVO_COUNTER 	;					//_at_ OEMRAM3+0x14; //HEGANG0022:Add smi hang check
//extern XWORD CPU_I;								//_at_ OEMRAM3+0x14;
//extern XWORD SLI_I;								//_at_ OEMRAM3+0x16;
//extern XWORD SD_PWR_EN;						//_at_ OEMRAM3+0x16;  //MEILING001:remove.
extern XBYTE LV_Authen_Step_CNT;					//_at_ OEMRAM3+0x18;
extern XBYTE Bat1_FPChgFlag;						//_at_ OEMRAM3+0x19;
extern XWORD Auto_mode_AVG;							//_at_ OEMRAM3+0x1D;
extern XBYTE Auto_mode_EnPtr;						//_at_ OEMRAM3+0x1F;
extern XWORD Auto_mode[20];							//_at_ OEMRAM3+0x20;

extern XBYTE PM1PendingTXCount;						//_at_ OEMRAM3+0x50;
extern XBYTE PM1PendingRXCount;						//_at_ OEMRAM3+0x51;
extern XBYTE PM1DataPending[6];						//_at_ OEMRAM3+0x52;
extern XBYTE PM2PendingTXCount;						//_at_ OEMRAM3+0x58;
extern XBYTE PM2PendingRXCount;						//_at_ OEMRAM3+0x59;
extern XBYTE PM2DataPending[6];						//_at_ OEMRAM3+0x5A;

extern XBYTE	uCritBattWakeThre;					//_at_ OEMRAM3+0x63
#define IFFS_Enable 		BIT0

extern XBYTE	uIffsCnt;							//_at_ OEMRAM3+0x64

extern XBYTE	uISCT;								//_at_ OEMRAM3+0x70
#define	b0IRST_WAKE_Enable		BIT0
#define b1IRST_WAKE_Finish		BIT1
#define b2IRST_TimeRecord		BIT2
#define b3ISCT_Permit			BIT3
#define	b4ISCT_FanDis			BIT4				// ISCT 1:Disable Fan cotrol.
#define b5ISCT_BKOFF			BIT5				// ISCT 1:Enable Backlight OFF.
#define b6ISCT_Wake				BIT6
#define b7ISCT_ShiftKey			BIT7

extern XBYTE	uISCT_2;							//_at_ OEMRAM3+0x71
#define	b0ISCT_pwrBTNpres		BIT0				// Check power button wake up status.
#define b1ISCT_LidWake			BIT1				// Check Lid wake up status.
#define b2ISCT_WlanLED			BIT2
#define b3ISCT_MUTE				BIT3				// ISCT 1:Enable MUTE.
#define	b4ISCT_Camera			BIT4				// ISCT 1:Disable Camera.
#define b5ISCT_PwrLED			BIT5				// ISCT 1:Disable Power LED.

extern XBYTE	Reset_Delay;                    	// _at_(OEMRAM3+0x72); 
extern XBYTE	Reset_Delay_Count;              	// _at_(OEMRAM3+0x73); 
extern XWORD   ShipModeACK;                         //_at_ OEMRAM3+0x75;//A53-H120128: Start Shipmode disable 5s loop once 20130608 20:28 
extern XBYTE   ShipModeCnt;                         //_at_ OEMRAM3+0x77;//A53-H120128: Start Shipmode disable 5s loop once 20130608 20:28 
extern XBYTE	ShipModeEn;                      	//_at_ OEMRAM3+0x79;
extern XBYTE ChkBattery_FCCchg_count2;				//_at_ OEMRAM3+0x8b;// 
extern XBYTE    PCH_ColdBoot_TimeOut;               //_at_OEMRAM3+0x8C; //ANGELAG012:add.

//THOMASG012: S+
extern XWORD	Psys_Data[4];					//_at_(OEMRAM3+0x8d);//0x8d-0x94
extern XWORD	Psys_AvgData;					//_at_(OEMRAM3+0x95); //0x95-0x96
extern XBYTE	Psys_Counter;					//_at_(OEMRAM3+0x97);	
extern XWORD  	Psys;							//_at_(OEMRAM3+0x98); 
//*******************************************************************************
//-------------------------------------------------------------------------------
//	0x400-0x4FF   OEM RAM 4
//-------------------------------------------------------------------------------
//*******************************************************************************
#define ECRAM				OEMRAM4
#define NameSpace			OEMRAM4

extern XBYTE	nVPC_CMD;		 				//_at_(NameSpace+0x00);		// Customer EC name space
extern XBYTE	nVPC_DATA;						//_at_(NameSpace+0x01);		// Customer EC name space
extern XBYTE	VPC_iStatus; 					//_at_(NameSpace+0x02);		// Customer EC name space
extern XBYTE	CRESERVED03;					//_at_(NameSpace+0x03);		// Customer EC name space
extern XBYTE	CRESERVED04; 					//_at_(NameSpace+0x04);		// Customer EC name space
extern XBYTE	CRESERVED05;					//_at_(NameSpace+0x05);		// Customer EC name space
extern XBYTE	nAtmFanSpeed; 					//_at_(NameSpace+0x06);		// Fan speed. For Lenovo ATM

extern XBYTE	LENOVOBATT;						//_at_(NameSpace+0x07);		//
#define BATTERY_USAGE		BIT0				// 1: Battery has been running over 72hrs, EM should pop out a message
#define BATTERY_LEGAL		BIT1				// 1: Battery is legal
#define BATTERY_Exhaustion	BIT2				// 1: Battery performance is on the normal status  //T082
#define NOTIFY_STORAGE		BIT3				// 1: Indicates close the storage mode
#define BATT_TEST_MODE		BIT4				// 1: Battery Testing Mode of SVCR (1:Start Battery Testing Mode: turn off all LED lights except the Power Button LED, Make the step1 fan on temperature increased by 10 degrees in thermal table.)
#define BAD_BATT			BIT5				// 1: indicates the main battery is bad
#define CPU_AUTO_MODE		BIT6				// 1: Auto mode
#define FAST_CHARGE 		BIT7				// 1: Fast Charge is running; 0: Fast Charge is stopping.

extern XBYTE	nACBriTableLimit;				//_at_(NameSpace+0x08);		// AC Brightness table limit
extern XBYTE	nDCBriTableLimit;				//_at_(NameSpace+0x09);		// DC brightness table limit
extern XBYTE	LENOVOPMFW; 					//_at_(NameSpace+0x0A);		//
#define	BATTERY_CAL_END		BIT0				//End the calibration cycle	//09111001 Isaac Start
#define AUTO_BRIGHTNESS		BIT1 				// 1 = Auto Brightness on
#define	BATTERY_MAIN_CAL	BIT2 				//calibration cycle for the main battery
#define	BATTERY_SEC_CAL		BIT3 				//calibration cycle for the second battery
#define	BATTERY_STORAGE		BIT4 				//battery storage mode
#define BATTERY_CYCLE		BIT5 				//battery Cycle mode
#define BATTERY_LONGEST		BIT6 				//Battery longest mode
#define BATTERY_GBMD		BIT7 				//Clear bit of advise user to select battery mode

extern XBYTE	SWITCHABLE;						//_at_(NameSpace+0x0B);		//
#define SWITCHABLEBTN   	BIT0  				// 1: discrete VGA  0: UMA VGA
#define HDMI				BIT1  				// 1 = HDMI exist
#define HybridGraphics		BIT2				// 1: Hybrid Graphics 0: External Graphic Only
#define Switchstate			BIT3				// 1: Switch started  0: Switch complete
#define EVENT_NOTIFY		BIT4 				//send input event notify 1:Send 0:Don't send	//09110502 Isaac
#define DCRFunction			BIT5				// 1: Enable  0: Disable
#define ColorEngine			BIT6				// 1: Enable  0: Disable
#define nSwitchableBTNGET	GET_MASK(SWITCHABLE,SWITCHABLEBTN)
#define nHybridGraphicsGET	GET_MASK(SWITCHABLE,HybridGraphics)
#define nHybridGraphicsEN	SET_MASK(SWITCHABLE,HybridGraphics)
#define nHybridGraphicsDIS	CLR_MASK(SWITCHABLE,HybridGraphics)
#define nSwitchstateGET 	GET_MASK(SWITCHABLE,Switchstate)

extern XBYTE	LENOVODEVICE;					//_at_(NameSpace+0x0C);		//
#define ODDPWROFF			BIT0				// 1: Power off
#define ODDPWRBTNON 		BIT1				// 1: Power on by button
#define ODDPWRKEYON 		BIT2				// 1: Power on by hotkey
#define Camera_EXIST		BIT3				// 1: The camera is exist
#define Camera_PwrOn		BIT4				// 1: The camera power is on
#define ODDS3State			BIT5				// Reserved for BIOS:Save ODD state when into S3 for BIOS used.
#define ODDPWRON			BIT6				// 1: ODD power on when system resume from S3/S4 or AC plug in.
#define ODDPWRStatus		BIT7				// 1: ODD have Power 0:ODD no power
#define nCameraExistGET 	GET_MASK(LENOVODEVICE,Camera_EXIST)
#define nCameraPwrGET		GET_MASK(LENOVODEVICE,Camera_PwrOn)
#define nCameraPwrCHG		CPL_MASK(LENOVODEVICE,Camera_PwrOn)

extern XBYTE	nRamTemp;						//_at_(NameSpace+0x0D);		// Ram temperature
extern XBYTE	nVramTemp; 						//_at_(NameSpace+0x0E);		// V Ram temperature
extern XBYTE	ATM_CMD;						//_at_(NameSpace+0x0F);		// ATM Fan control command

extern XBYTE	EM7FUNCTION;					//_at_(NameSpace+0x10);	// 0x10
#define PLATFORMTYPE1		BIT0				// 000-> SV+UMA 001-> ULV+UMA	011-> SV+NV
#define PLATFORMTYPE2		BIT1				// bit2~bit0
#define PLATFORMTYPE3		BIT2				// 010-> SV+ATI ?101-> ULV+NV	100-> ULV+ATI
#define LSPRUNNING			BIT3				// 1-indicate the temperature of ATI graphic platform CPU or GPU is too high / 0- low
#define OVERTEMPATI 		BIT4				// 1-indicate the temperature of ATI graphic platform CPU or
#define QIUETMODE			BIT5				// 1-indicate quiet mode(power saver)
#define ENABLES35			BIT6				// 1-enable S3.5

extern XBYTE	 BATTUPDATEFW;					//_at_(NameSpace+0x11);	// 0x11
#define  PriBattInhib		BIT0				// bit0 1-Disable reading information from primary battery for update battery firmware
#define  SecBattInhib	    BIT1				// bit1 1-Disable reading information from second battery for update battery firmware.

extern XBYTE	 FirmwareUpdateStatusL; 		//_at_(NameSpace+0x12);	// 0x12
extern XBYTE	 FirmwareUpdateStatusH; 		//_at_(NameSpace+0x13);	// 0x13
extern XBYTE	 BatterymakerIDL;				//_at_(NameSpace+0x14);	// 0x14
extern XBYTE	 BatterymakerIDH;				//_at_(NameSpace+0x15);	// 0x15
extern XBYTE	 HardwareIDL;					//_at_(NameSpace+0x16);	// 0x16
extern XBYTE	 HardwareIDH;					//_at_(NameSpace+0x17);	// 0x17
extern XBYTE	 FirmwareversionL;				//_at_(NameSpace+0x18);	// 0x18
extern XBYTE	 FirmwareversionH;				//_at_(NameSpace+0x19);	// 0x19
extern XBYTE	 DataVersionL;					//_at_(NameSpace+0x1A);	// 0x1A
extern XBYTE	 DataVersionH;					//_at_(NameSpace+0x1B);	// 0x1B
/*
0x37 (Get Battery Information ) 	Read Block
byte 0,1: The battery maker ID
byte 2,3: Hardware ID
byte 4,5: Firmware version (0 if the firmware is corrupted
byte 6,7: Data version (0 if this is not supported or the firmware is corrupted.
*/
extern XBYTE	 FBClamp;					//_at_(NameSpace+0x1C);	// 0x1C
#define FBClamp_REQ		BIT0					//
#define FBClamp_EN		BIT1					// Enter GC6
#define REQactive	BIT2

extern XBYTE	 ThermalMode;					//_at_(NameSpace+0x1D);	// 0x1D
#define ThermalMode_BALANCE  0x01
#define ThermalMode_QUIET  0x02
#define ThermalMode_HIGHP  0x03

extern XBYTE	 CPUI57;					//_at_(NameSpace+0x1E);	// 0x1E
#define I5CPU  0x05
#define I7CPU  0x07
extern XBYTE	 CRESERVED1F;					//_at_(NameSpace+0x1F);	// 0x1F
extern XBYTE	 CRESERVED20;					//_at_(NameSpace+0x20);	// 0x20
extern XBYTE	 CRESERVED21;					//_at_(NameSpace+0x21);	// 0x21
extern XBYTE	 CRESERVED22;					//_at_(NameSpace+0x22);	// 0x22
extern XBYTE	 CRESERVED23;					//_at_(NameSpace+0x23);	// 0x23
extern XBYTE	 CRESERVED24;					//_at_(NameSpace+0x24);	// 0x24
extern XBYTE	 CRESERVED25;					//_at_(NameSpace+0x25);	// 0x25
extern XBYTE	 CRESERVED26;					//_at_(NameSpace+0x26);	// 0x26
extern XBYTE	 CRESERVED27;					//_at_(NameSpace+0x27);	// 0x27
extern XBYTE	 CRESERVED28;					//_at_(NameSpace+0x28);	// 0x28
extern XBYTE	 CRESERVED29;					//_at_(NameSpace+0x29);	// 0x29
extern XBYTE	 CRESERVED2A;					//_at_(NameSpace+0x2A);	// 0x2A
extern XBYTE	 CRESERVED2B;					//_at_(NameSpace+0x2B);	// 0x2B
extern XBYTE	 CRESERVED2C;					//_at_(NameSpace+0x2C);	// 0x2C

extern XBYTE	 PANEL_INFO[16];	    			//_at_(NameSpace+0x30);	//ANGELAG008: modify to 16 bytes // 0x30~0x46	 // PanelROM 20100406 David
//ANGELAG008: add start
extern XBYTE	 pDevStatus1; 			      // _at_(NameSpace+0x43);  // 0x43
#define b0F6ScanCodeSend 	BIT0                //////1: have send 0: TP driver has responsed //W121
#define b1F6DisableTP       BIT1          //b1=1 F6 Disable TP
#define b2DisableKB         BIT2          //b2=1 disable kb
#define b3Scancode_STATUS   BIT3                ///b3=1 
#define b4LNVAPP_STATUS     BIT4                ///b4=1 
#define b5TPDRIVER_STATUS   BIT5                
#define b6MODE_STATUS       BIT6
#define b7DisableTP         BIT7
extern XBYTE   PDFWStatus;                         // _at_(NameSpace+0x44);//72JERRY060
#define b5PDFWUpdate	    BIT5
//ANGELAG008: ADD END
//ANGELAS107:S+Add code for GBSI function.

extern XBYTE	 pDevStatus2; 			           // _at_(NameSpace+0x49);  // 0x49
#define b1TransAP_CTRL      BIT1                //b1=1 transition AP control 
#define b2PadLid_Close      BIT2                //b2=1 pad lid close

extern XBYTE  YogaModeStatus;				    //_at_(NameSpace+0x4B);	// 0x4B
#define H0x01NotebookMode   0x01     //Notebook Mode
#define H0x02TabletMode     0x02     //Tablet Mode
#define H0x03StandMode      0x03     //Stand Mode
#define H0x04TendMode       0x04     //Tend Mode

extern XBYTE	batteryFirstUsedDateL;		//_at_ OEMRAM4+0x4c
extern XBYTE	batteryFirstUsedDateH;		//_at_ OEMRAM4+0x4d
//ANGELAS107:E+Add code for GBSI function.

extern XBYTE  EMStatusBit2;                                         //_at_(NameSpace+0x4A);	// 0x4A //ANGELAS032: add
#define b0SetBatteryShipMode		BIT0	// 0: turn off, 1:turn on ship mode //ANGELAS032: add

extern XBYTE	 ISCT_Timer;					//_at_(NameSpace+0x5A);	// 0x5A

// External name space 0x5D ~ 0x5F
extern XBYTE	 EXT_NAMESPACE_INDEX;   		//_at_(NameSpace+0x5D;	// 0x5D
extern XBYTE	 EXT_NAMESPACE_BANK;			//_at_(NameSpace+0x5E);	// 0x5E
extern XBYTE	 EXT_NAMESPACE_DATA;			//_at_(NameSpace+0x5F); // 0x5F

// SMBus EC interface ACPI RAM definition 0x60 ~ 0x87
extern XBYTE	SMB_PRTC;				//_at_ (NameSpace+0x60);// 0x60,
extern XBYTE	SMB_STS;				//_at_ (NameSpace+0x61);// 0x61,
extern XBYTE	SMB_ADDR;				//_at_ (NameSpace+0x62);// 0x62,
extern XBYTE	SMB_CMD;				//_at_ (NameSpace+0x63);// 0x63,
extern XBYTE	SMB_DATA;				//_at_ (NameSpace+0x64);// 0x64,
extern XBYTE    SMB_DATA1;				//		_at_ (NameSpace+0x65);
extern XBYTE	SMB_BCNT;				//_at_ (NameSpace+0x84);// 0x84,
// SBS Only Alarm registers
extern XBYTE	SMB_ALRA;				//_at_ (NameSpace+0x85);// 0x85,
extern XBYTE	SMB_ALRD0;				//_at_ (NameSpace+0x86);// 0x86,
extern XBYTE	SMB_ALRD1;				//_at_ (NameSpace+0x87);// 0x87,

extern XBYTE  	nBattery0x16L;      			//_at_(NameSpace+0x88);
#define SMBerror0		BIT0					// error code bit0
#define SMBerror1		BIT1					// error code bit1
#define SMBerror2		BIT2					// error code bit2
#define SMBerror3		BIT3					// error code bit3
#define FullyDsg		BIT4					// statusfully discharged
#define FullyChg		BIT5					// statusfully charged
#define Dsg				BIT6					// statusdischarging
#define Initialized		BIT7					// statusgauge initialized

extern XBYTE	nBattery0x16H;					//_at_(NameSpace+0x89);
#define RemTime_alarm	BIT0					// alarmremaining time
#define RemCap_alarm	BIT1					// alarmremaining capacity
//#define				BIT2						//
#define TermDsg_alarm	BIT3					// alarmterminate discharge
#define OverTemp_alarm	BIT4					// alarmover temperature
//#define				BIT5						//
#define TermChg_alarm	BIT6					// alarmterminate charge
#define OverChg_alarm	BIT7					// alarmover charge

extern XBYTE	EMStatusBit;					//_at_(NameSpace+0x8A);
#define b0SetKBLEDON	BIT0					// 0: turn off, 1:turn on keyboard led
#define	b1SetUSBChgEn	BIT1					// 0: Disable, 1:Enable usb charge in S4/S5 feature
#define	b2KBLEDChk		BIT2					//
#define b4RdKBLEDFun	BIT4					// 0: not support, 1:support keyboard led feature
#define	b5RdKBLEDOn		BIT5					// 0: OFF, 1:keyboard led turn on.
#define b6RdUSBChgS45	BIT6					// 0: OFF, 1: ON usb charge S4/S5.
#define b7RdUSBChgEn	BIT7					// 0: not support, 1: Support usb charge S4/S5 feature.

extern XBYTE 	TP_Type;						//_at_(NameSpace+0x8B);

extern XBYTE    OEMControl;                      //_at_(NameSpace+0x8C);  //ANGELAG005:add.
#define Expresschargemode		BIT0					// states: 0 off,1 on//72JERRY013:Add express charge feature.
#define b1BkOff         BIT1                    //0:screen backlight on, 1:screen backlight off.  //ANGELAG005:add.
#define b2ONEKEY_ENABLE            BIT2
#define b3ONEKEY_POWERUP            BIT3
#define b4DisUSBpower           BIT4

extern XBYTE	BATTMANUFACTURE[9];				//_at_(NameSpace+0x8F);	// (9 BYTES) Battery ManufactureName, reserve 9 byte
extern XBYTE	BATTDEVICENAME[8];				//_at_(NameSpace+0x98); // Only support main battery so far, 0x99~0x9F
												// deviece name ==> 8 byte  (2012.04.13 update by AC.)

extern XBYTE	AOAC_STATUS;					//_at_(NameSpace+0xA0);
#define ISCT_BAT_Temp		BIT0				// ISCT BAT temperature limitaion.
#define ISCT_BAT_Capy		BIT1				// ISCT BAT capacity limitaion.
#define ISCT_S0_ISCT		BIT2				// ISCT S0_ISCT status.
#define RESERVEA1_BIT3		BIT3				// Reserved
#define ISCT_Enable			BIT4				// ISCT support enable.
#define RESERVEA1_BIT5		BIT5				// Reserved

// bit6-7 ISCT wake up status. 01:EC time out, 02:PME(LAN) wake.

extern XBYTE	ACPI_HOTKEY;					//_at_(NameSpace+0xA1);		//
#define RESERVEA1_BIT0		BIT0				// Reserved    [L10110802]
#define VIDEO_KEY			BIT1				// Video output key  (1=Video output key pressed)
#define TOUCHPAD_BTN		BIT2				// Touch pad button (1=enable)
// #define RESERVEA1_BIT3		BIT3				// Reserved
#define HotkeyDisable		BIT3				// For hotkey disable in setup and hotkey behavior changed //T09A+
#define b4BIOS_Cover		BIT4				// For BIOS used.
#define b5BIOS_IFFS			BIT5				// BIOS enter IFFS status.
#define b6Cmd_NoShut		BIT6				// For command use to cut power bit.
#define b7BIOS_NoShut		BIT7				// For BIOS used.

extern XBYTE	nOSShutdownTemp2; 				//_at_(NameSpace+0xA2);		// OS Shutdown Temp. For system read setting. (DTS)
extern XBYTE	SYS_STATUS;						//_at_(NameSpace+0xA3);
#define OSTYPE0 			BIT0				// OSTYPE Bit2~Bit0 0x00:XP, 0x01:VISTA, 0x02:Linux, 0x03:WIN7, 0x04:WIN8, 0x05:WIN8.1, 0x06:WIN10
#define OSTYPE1 			BIT1				//
#define OSTYPE2 			BIT2				//
#define RESERVEA3_BIT3		BIT3				// Reserved
#define b4IllegalAdp		BIT4				// bit4:1 is Illegal Adapter  bit5bit4=00 is normal adapter bit5bit4=01 is illegal adp bit5bit4=10 is adapter power is not suitable    //T07C:brooke for EM9.0 Adapter protect 
#define b5UnSuitAdpPow     	BIT5				// bit5:1 is Adapter power is not suitable 	//T07C:brooke for EM9.0 Adapter protect 
#define b6BIOS_Flash		BIT6				// BIOS ignore Flash bit.
#define AC_ADP				BIT7				// (AC Adapter:0=OffLine, 1=OnLine)

extern XBYTE	WAKEUP_ENABLE; 					//_at_(NameSpace+0xA4);		//
#define PMEWAKE 			BIT0				// (PME Wk Enable:0=Disable, 1=Enable)
#define MDMWAKE 			BIT1				// (Modem Wk Enable:0=Disable, 1=Enable)
#define LANWAKE 			BIT2				// (LAN wakeup enable:0=Disable, 1=Enable)
#define RTCWAKE 			BIT3				// (RTC wakeup Enable :0=Disable, 1=Enable)
#define WLANWAKE			BIT4				// (Wireless LAN wakeup Enable:0=Disable, 1=Enable)
#define USBWAKE 			BIT5				// (USB wakeup Enable :0=Disable, 1=Enable)
#define KEYWAKE 			BIT6				// (Keyboard wakeup enable:0=Disable, 1=Enable)
#define TPWAKE				BIT7				// (TouchPad wakeup enable:0=Disable, 1=Enable)

extern XBYTE	ACOFF_SOURCE;					//_at_(NameSpace+0xA5);		//
#define CHARGECURRENT		BIT0				// 1 = AC_OFF for when stop charge but have charge current
#define ADPOVP				BIT1				// 1 = AC_OFF for adaptor improper
#define BATTLEARN			BIT2				// 1 = AC_OFF for battery at learning mode
#define ACMD 				BIT3				// 1 = AC_OFF for Command
#define BATTOVP 			       BIT4				// 1 = AC_OFF for battery OVP
#define LEAKCURRENT 		BIT5				// 1 = AC_OFF for Leakage current
#define AIRADP				BIT6				// 1 = AC_OFF for air adaptor
#define ACOP				       BIT7				// 1 = AC_OFF for ACOP

extern XBYTE	CURRENT_STATUS; 				//_at_(NameSpace+0xA6);		//A6
#define ENTER_S3			BIT0				// Entry S3 State flag
#define RESUME_S3			BIT1				// Resume S3 State flag
#define ENTER_S4			BIT2				// Entry S4 State flag
#define RESUME_S4			BIT3				// Resume S4 State flag
#define ENTER_S5			BIT4				// Entry S5 State flag
#define RESUME_S5			BIT5				// Resume S5 State flag
// Bit6,7 For BIOS use UEFI/Legacy judgement.

extern XBYTE	nOSThrottlingTemp;				//_at_(NameSpace+0xA7);		// OS Throttling Temp. For system read setting.
extern XBYTE	nOSShutdownTemp;				//_at_(NameSpace+0xA8);		// Throttling Temp put a temp in for system throttling.
extern XBYTE	nThrottlingAPSet;				//_at_(NameSpace+0xA9);

extern XBYTE	TcontrolTemp; 					//_at_(NameSpace+0xAA);		// For Tcontrol temp, sysbios will put Tcontrol temp
extern XBYTE	THERMAL_STATUS;					//_at_(NameSpace+0xAB);		//HW throttling define
#define MODE				BIT0				// (0=Local mode, 1=Remote mode)
#define INITOK				BIT3				// (0=Control by OS 1=control by EC)
#define FAN1ACTIVE			BIT4				// (1=Fan1 active)
#define FAN2ACTIVE			BIT5				// (1=Fan2 active)
#define FANSPEEDINITOK		BIT6				// (1=Fan speed timer initial OK)
#define SKINMODE			BIT7				// (0=skin address 90, 1=skin address 92)

extern XBYTE	nShutDownTemp;					//_at_(NameSpace+0xAC);		// Shutsown Temperature
extern XBYTE	nFanStatus1;					//_at_(NameSpace+0xAD);		// Just for utility (low nibble: Fan's speed step number ; high nibble: Fan's number )

extern XBYTE	REAL_THROTTLING_INDEX; 			//_at_(NameSpace+0xAE);
#define REAL_SW_THRM		BIT6				// (1=SW Throttling active)
#define REAL_HW_THRM_PIN	BIT7				// (1=HW Throttling Pin active)
#define nRealThermalPinEN	SET_MASK(REAL_THROTTLING_INDEX,REAL_HW_THRM_PIN)
#define nRealThermalPinDIS	CLR_MASK(REAL_THROTTLING_INDEX,REAL_HW_THRM_PIN)
#define nRealThermalPinGET	GET_MASK(REAL_THROTTLING_INDEX,REAL_HW_THRM_PIN)

extern XBYTE	THROTTLING_HW;					//_at_(NameSpace+0xAF);		//HW throttling define
#define TS_THERMAL			BIT0				// 1= Throttling for thermal
#define TS_BATTCAP			BIT1				// 1= Throttling for battery capacity
#define TS_THROT			BIT2				// Reserved for Cedar trail platform sync bit used. [L11080402]
#define RESERVEAF_BIT3		BIT3				// Reserved
#define RESERVEAF_BIT4		BIT4				// Reserved
#define RESERVEAF_BIT5		BIT5				// Reserved
#define RESERVEAF_BIT6		BIT6				// Reserved
#define RESERVEAF_BIT7		BIT7				// Reserved

extern XBYTE	nCpuTemp; 						//_at_(NameSpace+0xB0);		// CPU current temperature
extern XBYTE	nCpuTempLocal;					//_at_(NameSpace+0xB1);		// CPU local temperature
extern XBYTE	nSKINTemp90;					//_at_(NameSpace+0xB2);		// SKIN temperature address 90
//extern XBYTE	nOSShutdownTemp3;				//_at_(NameSpace+0xB3);		// OS Shutdown Temp. For system read setting.(VGA)//ANGELAS070:remove
extern XBYTE	ThermistorCPU_TEMP;				//_at_(NameSpace+0xB3);		//For system read setting.(VGA)//ANGELAS070:add
extern XBYTE	VGA_TEMP;						//_at_(NameSpace+0xB4);		// VGA temperature
extern XBYTE	EXTVGA_TEMP;					//_at_(NameSpace+0xB5);		// External VGA temperature.

extern XBYTE	nNBTemp;						//_at_(NameSpace+0xB6);		// NorthBridge temperature
extern XBYTE	ACPI_STAT;						//_at_(NameSpace+0xB7);
#define RESERVEB7_BIT0		BIT0				// Reserved [L10110801]
#define PASSWORD_BTN		BIT1				// 1=enable
#define DIGITAILMODE		BIT2				// 1=Digitial mode selected
#define CDMODELOCK			BIT3				// 1=CD lock mode enable
#define RESERVEB7_BIT4		BIT4				// Reserved [L10110801] [L11051201]
#define RESERVEB7_BIT5		BIT5				// Reserved [L10110801] [L11051201]
#define RESERVEB7_BIT6		BIT6				// Reserved
#define RESERVEB7_BIT7		BIT7				// Reserved

extern XBYTE	SWI_EVENT;						//_at_(NameSpace+0xB8);		// SWI Event indicators
#define RESERVEB8_BIT0		BIT0				// Reserved
#define LID 				BIT1				// Lid open event(0=off,1=on)
#define PME 				BIT2				// PME event(0=off,1=on)
#define POWERSW 			BIT3				// Power button event(0=off,1=on)
#define RING				BIT4				// Ring in event(0=off,1=on)
#define BTWAKE				BIT5				// Blue tooth wake up event(0=off,1=on)
#define RESERVEB8_BIT6		BIT6				// Reserved  
#define RESERVEB8_BIT7		BIT7				// Reserved

extern XBYTE	nBrightValue;					//_at_(NameSpace+0xB9);		// LCD Brightness value.

extern XBYTE	GPU_REAL_THROTTLING_INDEX; 		//_at_(NameSpace+0xBA);  //MEILING033:add(throttling step is 0:D1 to 4:D5).

extern XBYTE	DEVICEMODULE; 					//_at_(NameSpace+0xBB);
#define WIRELESS			BIT0				// 1 = wirless lan active
#define BLUETOOTH			BIT1				// 1 = blue tooth active
#define WIRELESS_EXIST		BIT2				// 1 = wirless lan exist
#define BLUETOOTH_EXIST 	BIT3				// 1 = blue tooth exist
#define b4KILL_STATUS 		BIT4				// 1 = kill switch active
#define WLAN_INITOK 		BIT5				// 1 = WLAN init OK
#define WWAN_3G 			BIT6				// 1 = 3G device active
#define WWAN_3G_EXIST		BIT7				// 1 = 3G device exist
#define nWirelessLanGET 	GET_MASK(DEVICEMODULE,WIRELESS)
#define nBlueToothEN		SET_MASK(DEVICEMODULE,BLUETOOTH)
#define nBlueToothDIS		CLR_MASK(DEVICEMODULE,BLUETOOTH)
#define nBlueToothGET		GET_MASK(DEVICEMODULE,BLUETOOTH)
#define nKillSwitchEN		SET_MASK(DEVICEMODULE,b4KILL_STATUS)
#define nKillSwitchDIS		CLR_MASK(DEVICEMODULE,b4KILL_STATUS)
#define nKillSwitchGET		GET_MASK(DEVICEMODULE,b4KILL_STATUS)
#define nKillSwitchCHG		CPL_MASK(DEVICEMODULE,b4KILL_STATUS)
#define nWWAN3GLanGET		GET_MASK(DEVICEMODULE,WWAN_3G)

extern XBYTE	nKbProjectID; 					//_at_(NameSpace+0xBC);
extern XBYTE	CPU_TYPE; 						//_at_(NameSpace+0xBD);
												// CPU_TYPE : bit0~bit2 0x00:Tj85,	  0x01:Tj90,	 0x02:Tj100,   0x03:Tj105
												// CPU TDP : bit3~bit5 0x01:XE(55w), 0x02:QuadCore(45w), 0x03:DualCore(35w), 0x04: UL(25w), 0x00: ULV(17w)
												// VGA Type : bit6~bit7 0x00:UMA, 0x01:Discrete, 0x11:Optimus.

extern XBYTE  	SEL_STATE0;      				//_at_(NameSpace+0xBE);
#define PRESENT_A			BIT0				// Present A (set if 1st battery present)
#define PRESENT_B			BIT1				// Presetn B (set if 2nd battery present)
#define RESERVEBE_BIT2		BIT2				// Reserved
#define RESERVEBE_BIT3		BIT3				//
#define CHARGE_A			BIT4				//Charge A(set if 1st battery be charging)
#define CHARGE_B			BIT5				//Charge B(set if 2 snd battery be charging)
#define RESERVEBE_BIT6		BIT6				//
#define RESERVEBE_BIT7		BIT7				//

extern XBYTE	SEL_STATE1;						//_at_(NameSpace+0xBF);
#define ENLLB				BIT0				// PWR_BY_A ( set if system power up by 1st)
#define ENCHECKEDV1 		BIT1				// PWR_BY_B ( set if system power up by 2nd)
#define ENSETEDV1			BIT2
#define ENSHUTDOWN5P		BIT3
#define ENREADEN			BIT4				// SMB_A (set if 1st battery on SMBus)
#define ENCOMMFAILSTART 	BIT5				// SMB_B (set if 2nd battery on SMBus)
#define RESERVEF1_BIT6		BIT6
#define RESERVEF1_BIT7		BIT7

extern XBYTE	nBatteryStatL; 					//_at_(NameSpace+0xC0);
#define CMBS_BATTTYPE		BIT0				//0=NiMh 1=Li-on
#define CMBS_BATTMODE		BIT1				//Battery mode (for AP read only)
#define CMBS_LOWBATT		BIT2				//Low battert
#define RESERVEC0_BIT3		BIT3				//
#define BATT_TYPE1			BIT4				//4~6 001:SANYO, 010:SONY ; 100:Panasonic; 101:Smasung; 011: LG; 110:Celxpert; 111:Simplo
#define BATT_TYPE2			BIT5				//
#define BATT_TYPE3			BIT6				//
#define RESERVEC0_BIT7		BIT7				//

extern XBYTE	nBatteryStatH; 					//_at_(NameSpace+0xC1);
#define CMBS_DISCHARGE		BIT0				// Discharging
#define CMBS_CHARGE 		BIT1				// Charging
#define CMBS_CRITICALLOW	BIT2				// Discharging and Now is ciritical low
#define RESERVEC1_BIT3		BIT3				//
#define RESERVEC1_BIT4		BIT4				//
#define RESERVEC1_BIT5		BIT5				//
#define RESERVEC1_BIT6		BIT6				//
#define RESERVEC1_BIT7		BIT7				//

extern XBYTE	nRemainingCapL; 				//_at_(NameSpace+0xC2);	// (WORD) Remaining Capacity
extern XBYTE	nRemainingCapH; 				//_at_(NameSpace+0xC3);
extern XBYTE	nSerialNumL; 					//_at_(NameSpace+0xC4);	// (WORD) Serial number
extern XBYTE	nSerialNumH; 					//_at_(NameSpace+0xC5);
extern XBYTE	nPresentVoltL; 					//_at_(NameSpace+0xC6);	// (WORD) Present voltage
extern XBYTE	nPresentVoltH; 					//_at_(NameSpace+0xC7);
extern XBYTE	nDesignVoltL; 					//_at_(NameSpace+0xC8);	// (WORD) Design voltage
extern XBYTE	nDesignVoltH; 					//_at_(NameSpace+0xC9);
extern XBYTE	nDesignCapL; 					//_at_(NameSpace+0xCA);	// (WORD) Design Capacity
extern XBYTE	nDesignCapH; 					//_at_(NameSpace+0xCB);
extern XBYTE	nFullChgCapL; 					//_at_(NameSpace+0xCC);	// (WORD) Full charge Capacity
extern XBYTE	nFullChgCapH; 					//_at_(NameSpace+0xCD);
extern XBYTE	nBattGasgauge; 					//_at_(NameSpace+0xCE);	// (BYTE) Battery percentage
extern XBYTE	nCycleCounter; 					//_at_(NameSpace+0xCF);	// (BYTE) Battery cycle counter
extern XBYTE	nNowCurrentL; 					//_at_(NameSpace+0xD0);	// (WORD) Battery current
extern XBYTE	nNowCurrentH; 					//_at_(NameSpace+0xD1);
extern XBYTE	nAvgCurrentL; 					//_at_(NameSpace+0xD2);	// (WORD) Battery average current
extern XBYTE	nAvgCurrentH; 					//_at_(NameSpace+0xD3);
extern XBYTE	nBattComsup; 					//_at_(NameSpace+0xD4);	// (BYTE) System power comsumption,Watt will not over 255. So just only need 1 byte.
extern XBYTE	nBattVolt; 						//_at_(NameSpace+0xD5);	// (BYTE) Battery Volt
extern XBYTE	nBattTsTemp; 					//_at_(NameSpace+0xD6);	// (BYTE) Battery Temp.
extern XBYTE	nBattAverTemp; 					//_at_(NameSpace+0xD7);	// (BYTE) Battery Average Temp.
extern XBYTE	nBattCharCurrentL; 				//_at_(NameSpace+0xD8);	// (WORD) Battery charge currrent.
extern XBYTE	nBattCharCurrentH; 				//_at_(NameSpace+0xD9);
extern XBYTE	nBattTempCnt; 					//_at_(NameSpace+0xDA);	// (BYTE) Battery current Temp sample counter
extern XBYTE	nBattCmdIdx; 					//_at_(NameSpace+0xDB);	// (BYTE) Battery Command index for read battery through SMBus
extern XBYTE	nBattGetBattCnt; 				//_at_(NameSpace+0xDC);	// (BYTE) Count up to communicate battery
extern XBYTE	nBattOvrTempCnt; 				//_at_(NameSpace+0xDD);	// (BYTE) Count up if battery is arrived overtemp.
extern XBYTE	nBattStatusSMBL; 				//_at_(NameSpace+0xDE);	// (WORD) Battrery SMBus status
extern XBYTE	nBattStatusSMBH; 				//_at_(NameSpace+0xDF);	//
extern XBYTE	nBattOvrVoltCnt; 				//_at_(NameSpace+0xE0);	// (BYTE) Battery over voltage counter
extern XBYTE	nBattErrorCnt; 					//_at_(NameSpace+0xE1);	// (BYTE) Communicatiion fail counter
extern XBYTE	nBattTsVolt; 					//_at_(NameSpace+0xE2);	// (BYTE) Battery voltage of ADC
extern XBYTE	nCell1VoltL;					//_at_(NameSpace+0xE3);
extern XBYTE	nCell1VoltH;					//_at_(NameSpace+0xE4);
extern XBYTE	nCell2VoltL;					//_at_(NameSpace+0xE5);
extern XBYTE	nCell2VoltH;					//_at_(NameSpace+0xE6);
extern XBYTE	nCell3VoltL;					//_at_(NameSpace+0xE7);
extern XBYTE	nCell3VoltH;					//_at_(NameSpace+0xE8);
extern XBYTE	nCell4VoltL;					//_at_(NameSpace+0xE9);
extern XBYTE	nCell4VoltH;					//_at_(NameSpace+0xEA);
extern XBYTE	nManufactureAccessL;			//_at_(NameSpace+0xEB);
extern XBYTE	nManufactureAccessH;			//_at_(NameSpace+0xEC);
extern XBYTE	SHIPMODE_L;						//_at_(NameSpace+0xED); // Read 0X34, RETURN VALUE
extern XBYTE	SHIPMODE_H;						//_at_(NameSpace+0xEE);
extern XBYTE	RESERVEDEF;						//_at_(NameSpace+0xEF);

extern XBYTE  	nBatteryStatus1;    			//_at_(NameSpace+0xF0);
#define ENEXIST			BIT0					// Battery exist
#define ENFULL			BIT1					// Battery full
#define ENEMPTY			BIT2					// Battery empty
#define ENFIRSTIN		BIT3					// Battery first in
#define RESERVEF0_BIT4	BIT4					//
#define RESERVEF0_BIT5	BIT5					//
#define ENRDCOMPLETE	BIT6					// Battery read complete
//#define RESERVEF0_BIT7	BIT7					//
#if SW_ISCT
#define BATUnZeroTemp	BIT7					// Temp of indicate under 0 degree
#endif
extern XBYTE	nBatteryStatus2;				//_at_(NameSpace+0xF1);
#define ENLLB			BIT0
#define ENCHECKEDV1		BIT1
#define ENSETEDV1		BIT2
#define ENSHUTDOWN5P	BIT3
#define ENREADEN		BIT4
#define ENCOMMFAILSTART	BIT5					//Start to count communication counter
#define RESERVEF1_BIT6	BIT6
#define RESERVEF1_BIT7	BIT7

extern XBYTE	nStopChgStat3L;					//_at_(NameSpace+0xF2);
#define ENSTOPCHG			BIT0				// Battery fast charging timeout
#define ENBADCELL			BIT1				// Battery bad cell
#define ENCOMMFAIL			BIT2
#define ENCHARGESUSP		BIT3				// use for detect battery charging suspend
#define ENCMDSTOPCHG		BIT4				// Battery command stop charge
#define ENCUSTOMER			BIT5				// Battery stop charge for Customer use
#define ENOVERVOLTAGE		BIT6
#define ENOVERTEMP			BIT7				// Battery over temperature

extern XBYTE	nStopChgStat3H;					//_at_(NameSpace+0xF3);
#define ENTRITIMEOUT		BIT0				// Battery trickle charging timeout
#define ENADPTHROTTLING		BIT1				// Stop charge of Adapter throttling
#define ENMANUFACTURE		BIT2				// Stop charge of Sony battery
#define HybridModeStopChg	BIT3
#define ECFLASH				BIT4				// Stop charge of flash
#define NotLenovoBattery	BIT5				// Stop charge if it is not Lenovo battery
#define EmStopChgarg		BIT6				// Battery health function used.Flag will set high for stop chgarg when battery chgarg full.
#define ENBOOSTSTOPCHG		BIT7

extern XBYTE	nManufactureDateL;				//_at_(NameSpace+0xF4);
extern XBYTE	nManufactureDateH;				//_at_(NameSpace+0xF5);
extern XBYTE	nChargingVoltL;					//_at_(NameSpace+0xF6);
extern XBYTE	nChargingVoltH;					//_at_(NameSpace+0xF7);

extern XBYTE	BATTCELL;						//_at_(NameSpace+0xF8);
#define ENBATT3CELL 		BIT0				// Set Battery is 3 cells
#define ENBATT4CELL 		BIT1				// Set Battery is 4 cells
#define ENBATT6CELL 		BIT2				// Set Battery is 6 cells
#define ENBATT8CELL 		BIT3				// Set Battery is 8 cells
#define ENBATT9CELL 		BIT4				// Set Battery is 9 cells
#define ENBATT12CELL		BIT5				// Set Battery is 12 cells
#define ENBATT3S			BIT6				// Set battery is 3 series
#define ENBATT4S			BIT7				// Set battery is 4 series

//extern XBYTE	PECIBIOSTEST;						// _at_(NameSpace+0xFC);  //£ºwhen enter S3/S4/CB,BIOS sent B4 CMD to EC from 66port for peci fail.//: Remove JERRYB0048 and  PECI workaround.
extern XBYTE	MBID_FB;						// _at_(NameSpace+0xFB);  //T054+
extern XBYTE F2_Pressed;                      //_at_(OEMRAM4+0xFC) //ANGELAG001:add.
#define F2Flag          BIT0  //ANGELAG001:add.
#define F12Flag         BIT1  //ANGELAG001:add.
extern XBYTE	nAtmFan1Speed; 					//_at_(NameSpace+0xFE);//72JERRY014: Add the second fan method
extern XBYTE	EM8_TEST;						//_at_(NameSpace+0xFF);	

//*******************************************************************************
//-------------------------------------------------------------------------------
//	0x500-0x5FF   OEM RAM 5
//-------------------------------------------------------------------------------
//*******************************************************************************
#define ECRAM5				OEMRAM5
extern XBYTE PECI_TIMEOUT;				//_at_ OEMRAM5 + 0x0B;
extern XBYTE PECI_ERROR;				//_at_ OEMRAM5 + 0x0C;
extern XBYTE PECIDelayCnt;				//_at_ OEMRAM5 + 0x0D;
#define PECIBuffer			(* (XBYTE * )( 0x0500 + 0x0E))
extern XWORD PECIBuffer0;				//_at_ OEMRAM5 + 0x0E;
extern XWORD PECIBuffer1;				//_at_ OEMRAM5 + 0x10;
extern XWORD PECIBuffer3;				//_at_ OEMRAM5 + 0x12;
extern XWORD PECIBuffer2;				//_at_ OEMRAM5 + 0x14;
extern XWORD PECIBuffer4;				//_at_ OEMRAM5 + 0x16;
extern XWORD PECIBuffer5;				//_at_ OEMRAM5 + 0x18;
extern XWORD PECIBuffer7;				//_at_ OEMRAM5 + 0x1A;
extern XWORD PECIBuffer6;				//_at_ OEMRAM5 + 0x1C;
// reserved 0x0C1F
extern XBYTE TJMAX;						//_at_ OEMRAM5 + 0x20;
extern XBYTE PECI_Err_CNT;				//_at_ OEMRAM5 + 0x21;

extern XBYTE PECI_GET_TEMPL;			//_at_ OEMRAM5 + 0x24;
extern XBYTE PECI_GET_TEMPH;			//_at_ OEMRAM5 + 0x25;

extern XBYTE ERR_THMSTS;				// _at_ OEMRAM5 + 0x39;
#define	b0ThmICError		BIT0		// Thermal IC read temperature error.
#define	b1VGATempEr			BIT1		// VGA read temperature error.
#define	b2EXTVGATempEr		BIT2		// External VGA read temperature error.
#define	b3PECITempEr		BIT3		// CPU read temperature error.

extern XBYTE TMErrCnt;					//_at_ OEMRAM5 + 0x3A;
extern XBYTE VGAErrCnt;					//_at_ OEMRAM5 + 0x3B;
extern XBYTE EXTVGAErrCnt;				//_at_ OEMRAM5 + 0x3C;
extern XBYTE TmlICStep;					//_at_ OEMRAM5 + 0x3D;
extern XBYTE vgaok;					//_at_ OEMRAM5 + 0x3E;

extern XWORD cWriteCurrentPL1;			//_at_ OEMRAMC + 0x40;
extern XWORD cWriteCurrentPL2;			//_at_ OEMRAMC + 0x42;

extern BYTE    USB_Delay;//			_at_ OEMRAM5 + 0x44; //USB0628+ 
extern XBYTE   AOU_IFG_Debounce_Cnt;//        _at_(OEMRAM5+0x45);  
extern XBYTE   AOU_status;                             //_at_ OEMRAM5+0x46;
 #define AOUPort_Dev    BIT0    // 1 = Exist USB Device in AOU port.
 #define AOUPort_DevSET    SET_MASK(AOU_status,AOUPort_Dev)
 #define AOUPort_DevCLR    CLEAR_MASK(AOU_status,AOUPort_Dev)
 #define AOUPort_DevGET    GET_MASK(AOU_status,AOUPort_Dev)

extern XBYTE     Fan1On_Step1;// 		    _at_ (OEMRAM5+0x50);
extern XBYTE     Fan1On_Step2;// 	        _at_ (OEMRAM5+0x51);
extern XBYTE     Fan1On_Step3;// 	     	_at_ (OEMRAM5+0x52);
extern XBYTE     Fan1On_Step4;// 			_at_ (OEMRAM5+0x53);
extern XBYTE     Fan1On_Step5;//     		    _at_(OEMRAM5+0x54);
extern XBYTE     Fan1On_Step6;//     		    _at_(OEMRAM5+0x55);
extern XBYTE     Fan1On_Step7;//     		    _at_(OEMRAM5+0x56);
extern XBYTE     Fan1On_Step8;//     		    _at_(OEMRAM5+0x57);
extern XBYTE     Fan2On_Step1;//   			_at_(OEMRAM5+0x58);
extern XBYTE     Fan2On_Step2;//   			_at_(OEMRAM5+0x59);
extern XBYTE     Fan2On_Step3;//   			_at_(OEMRAM5+0x5A);
extern XBYTE     Fan2On_Step4;//   			_at_(OEMRAM5+0x5B);
extern XBYTE     Fan2On_Step5;//    			_at_(OEMRAM5+0x5C);
extern XBYTE     Fan2On_Step6;//   			_at_(OEMRAM5+0x5D);
extern XBYTE     Fan2On_Step7;//   			_at_(OEMRAM5+0x5E);
extern XBYTE     Fan2On_Step8;//  			    _at_(OEMRAM5+0x5F);

//ANGELAG008: ADD START
extern  BYTE  pDevStatus3; 			//_at_ OEMRAM5 + 0x60;
#define b3VPCScanCodeSend   BIT3          //1: have send 0: TP driver has responsed   
#define b4VPCDisableTP      BIT4          //b4=1 disable TP by vpc      
#define b5VPCEnDISTP        BIT5       
#define b6EnDisTPSend       BIT6           //MARTINS048:change 'b6Transition_01cmd' to 'b6EnDisTPSend' 
#define b7TPStateFormer     BIT7           //MARTINS048:Add        
//ANGELAG008: ADD END
extern XBYTE MaxCPU_MCHTemp;			//_at_ OEMRAM5+0x6C
// Reserve 0x056D~057F
extern XBYTE SYS_STATUS2;			   //_at_ OEMRAM5 + 0x6D;//MARTINM026:add 
//#define    FolioCaseIn     BIT0       //(1:IN, 0:OUT)              //MARTINM026:add 
#define    PadLidClose     BIT1       //(1:Close, 0:Open)              //MARTINM037:add 

extern XBYTE    CMOS_TEST;			//_at_ OEMRAM5 + 0x6F; // RTC_RST
#define    b0_CMOS_FunctionKey     BIT0 //ANGELAS052:Add
#define    b1_CMOS_delay1flag      BIT1 //ANGELAS052:Add

extern XBYTE EC_DeepSleep_Temp0;        //_at_ OEMRAM5 + 0x70;
extern XBYTE EC_DeepSleep_Temp1;        //_at_ OEMRAM5 + 0x71;
extern XBYTE EC_DeepSleep_Temp2;        //_at_ OEMRAM5 + 0x72;
extern XBYTE EC_DeepSleep_Temp3;        //_at_ OEMRAM5 + 0x73;
extern XBYTE EC_DeepSleep_Temp4;        //_at_ OEMRAM5 + 0x74;
extern XBYTE EC_DeepSleep_Temp5;        //_at_ OEMRAM5 + 0x75;
extern XBYTE EC_DeepSleep_Temp6;        //_at_ OEMRAM5 + 0x76;

//MARTINH154:Add start
extern XBYTE cmosdelay;    			//_at_ OEMRAM5 + 0x77;
extern XBYTE cmosdelay1;    			//_at_ OEMRAM5 + 0x78;
extern XBYTE cmosshutdelay;   		//_at_ OEMRAM5 + 0x79;
//MARTINH154:Add end

extern XBYTE EC_DeepSleep_TempPortA;    //_at_ OEMRAM5 + 0x80;
extern XBYTE EC_DeepSleep_TempPortB;    //_at_ OEMRAM5 + 0x81;
extern XBYTE EC_DeepSleep_TempPortC;    //_at_ OEMRAM5 + 0x82;
extern XBYTE EC_DeepSleep_TempPortD;    //_at_ OEMRAM5 + 0x83;
extern XBYTE EC_DeepSleep_TempPortE;    //_at_ OEMRAM5 + 0x84;
extern XBYTE EC_DeepSleep_TempPortF;    //_at_ OEMRAM5 + 0x85;
extern XBYTE EC_DeepSleep_TempPortG;    //_at_ OEMRAM5 + 0x86;
extern XBYTE EC_DeepSleep_TempPortH;    //_at_ OEMRAM5 + 0x87;
extern XBYTE EC_DeepSleep_TempPortI;    //_at_ OEMRAM5 + 0x88;
extern XBYTE EC_DeepSleep_TempPortJ;    //_at_ OEMRAM5 + 0x89;

extern XBYTE      Fan1Off_Step1;//   		_at_(OEMRAM5+0x90);
extern XBYTE      Fan1Off_Step2;//     		_at_(OEMRAM5+0x91);
extern XBYTE      Fan1Off_Step3;//     		_at_(OEMRAM5+0x92);
extern XBYTE      Fan1Off_Step4;//     		_at_(OEMRAM5+0x93);
extern XBYTE      Fan1Off_Step5;//    		_at_(OEMRAM5+0x94);
extern XBYTE      Fan1Off_Step6;//     		_at_(OEMRAM5+0x95);
extern XBYTE      Fan1Off_Step7;//      		_at_(OEMRAM5+0x96);
extern XBYTE      Fan1Off_Step8;//      		_at_(OEMRAM5+0x97);
extern XBYTE      Fan2Off_Step1;//    		_at_(OEMRAM5+0x98);
extern XBYTE      Fan2Off_Step2;//     		_at_(OEMRAM5+0x99);
extern XBYTE      Fan2Off_Step3;//     		_at_(OEMRAM5+0x9A);
extern XBYTE      Fan2Off_Step4;//     		_at_(OEMRAM5+0x9B);
extern XBYTE      Fan2Off_Step5;//    		_at_(OEMRAM5+0x9C);
extern XBYTE      Fan2Off_Step6;//     		_at_(OEMRAM5+0x9D);
extern XBYTE      Fan2Off_Step7;//     		_at_(OEMRAM5+0x9E);
extern XBYTE      Fan2Off_Step8;//    		_at_(OEMRAM5+0x9F);


extern XBYTE testtoolflag;				//_at_ OEMRAM5 + 0xBC; 
extern XBYTE Bioswatchdog;				//_at_ OEMRAM5 + 0xBD;//ANGELAS094:Add workaround for hang bios.
extern XBYTE Bioswatchdogtime;				//_at_ OEMRAM5 + 0xBE;//ANGELAS094:Add workaround for hang bios.

extern XBYTE	ManualFanPRM;					//_at_(ECRAM5+0xBF);

//*******************************************************************************
//-------------------------------------------------------------------------------
//	0x600-0x6FF   OEM RAM 6
//-------------------------------------------------------------------------------
//*******************************************************************************
//T11: only change the comment from cCmd to cCHIPType, if detail, please refer to Oem_memory.c
extern XBYTE		cCmd;				//_at_ OEMRAM5+0xA0;
#define bPCHPWR_Keep			BIT2
#define b3BkOff 				BIT3
#define b5VolMute				BIT5	// Command to mute
#define b6TestBtnEn				BIT6	// Send scancode for test button
#define b7LidIgnore				BIT7	// Ignore lid function

extern XBYTE	cKBStatus;				//_at_ OEMRAM5+0xA1; 
#define cKBS_KBType0			BIT0	// Keyboard selection Bit0 control by EC
#define cKBS_KBType1    		BIT1	// Keyboard selection Bit1 control by EC
#define	cKBS_KBInit				BIT2	// Keyboard Type initial
#define	cKBS_TPInS3				BIT3
#define cKBS_TPData				BIT4
#define	cKBS_KBPress			BIT5
#define	cKBS_KBWakeUp			BIT6	// KB Wake up
#define	cKBS_TPWakeUp			BIT7	// TP Wake up

extern XBITS_8  	cBattFlag0;			//_at_ OEMRAM5+0xA2;
	#define	cBF0_Full			bit0
	#define	cBF0_FullToTarget	bit1
	#define	cBF0_GoTarget		bit2
	#define	cBF0_BLMode			bit3
	#define	cBF0_ACOFF			bit4
	#define	cCmdAcOff			bit7

extern XBITS_8  	cDev;   			//_at_ OEMRAM5+0xA3;
	#define	cD_LidResume		bit0
	#define	cD_LIDPressed		bit1	// In WinXP monitor turn off issue
	#define	cD_FanOn3SecFinish	bit2	// Fan on 3/5 seconds finish flag
	#define	cD_WovMFGtest	    bit3	// wake up MFG test//72JERRY065:Add CMD 0x59 test voice wake up for MFG.
	#define	cAcS3AutoResume		bit6	// Auto resume from S3 by AC plug in

extern XBITS_8  	cSPIEEPROM;  		//_at_ OEMRAM5+0xA4;
	#define	cSPI_Buffer1En		bit0
	#define	cSPI_DataInROM		bit1
	#define	cSPI_ROMCopyStart	bit2

extern XBITS_8  cThrottlingSet;			//_at_ OEMRAM5+0xA5 // 00: Nothing, 01: Thr 12.5%, 02: Thr 25%,..,08: Thr 100%
	#define	cReserve15_bit4		bit4
	#define	cThrottling			bit5
	#define	cTHRM_SW			bit6
	#define	cTHRM_PIN			bit7

extern XBYTE 	cTargetGauge;  			//_at_ OEMRAM5+0xA6;
extern XBYTE  battery_critical;       //_at_ OEMRAM5+0xA7; //ANGELAS093
  #define	DC0ver5enterS3		BIT0//ANGELAS093: Modify battery RSOC below 5% can't into S3.
  #define	DCdischargeto5ins3		BIT1//ANGELAS093: Modify battery RSOC below 5% can't into S3.

extern XBYTE 	cCPUKind;  				//_at_ OEMRAM5+0xA8;

extern XBYTE 	cPanelId; 				//_at_ OEMRAM5+0xA9;

extern XBYTE 	cBrightUpDelayCnt; 		//_at_ OEMRAM5+0xAA;
extern XBYTE 	cBrightDnDelayCnt; 		//_at_ OEMRAM5+0xAB;
extern XBYTE 	cRecoveryBrightValue; 	//_at_ OEMRAM5+0xAC;

extern XBYTE 	cSysActionFlag; 		//_at_ OEMRAM5+0xAD;

extern XBYTE 	cADPThrottling;			//_at_ OEMRAM5+0xAE;
extern XBYTE 	cTHERMThrottling;		//_at_ OEMRAM5+0xAF;
extern XBYTE 	cBATTThrottling;		      //_at_ OEMRAM5+0xB0;

extern XBITS_16  cSysStus;				//_at_ OEMRAM5+0xB1
	#define cSS_S0				bit0	// System in S0 system on enable	F6C0 bit0
	#define cSS_CrMode			bit1	// Crisis mode flag.If "0" ,system now in normal mode;If "1" ,system now in Crisis mode.  [L11042801]
	#define cSS_S3 				bit2 	// System in S3 InSuspToRAM
	#define cSS_S4 	        	bit3 	// System in S4 save to disk
	#define cSS_S5		    	bit4 	// System in S5 system off
	#define cSS_Acpi			bit5 	// System in ACPI mode
	#define cSS_Shutdown    	bit6 	// System prepare for shutdwon
	#define cSS_Shutdown4sec	bit7 	// System 4 second shutdown

	#define	cSS_BootRestartReady	bit8	// Ready run boot restart function
	#define	cSS_EcStable 		bit9 	// System is stable			F6C0 bit1
	#define cSS_SuspendReq		bit10	// System is going to go to S3
	#define	cSS_WakeupReq		bit11	// System is going to wake up
	#define	cSS_PMEStatus		bit12	// PME's status
	#define	cSS_S0Status		bit13	// System is on S4
	#define	cSS_ShutDownID		bit14	// For force shutdown if can't save shutdown ID to EEPROM
	#define	cSS_S3Finish		bit15	// S3 power sequence is finished

extern XBITS_16 	cBattInform;		//_at_ OEMRAM5+0xB3
	#define	cBI_CommFailDelay	bit0
	#define	cBI_LbBeep			bit1
	#define	cBI_BattUse			bit2	// F6C3
	#define	cBI_TrickleCharge	bit3
	#define cBI_NowCharge		bit4	// Battery charging status flag
	#define cBI_InLB 			bit5	// Battery in Low status
	#define cBI_InLLB			bit6	// Battery in critical low status flag (LLB indicator)
	#define cBI_StartCount		bit7	// Start bad cell timer flag

	#define cBI_Second2Finish	bit8	// Second4 timer finish flag
	#define	cBI_ReadBattEn 		bit9	// For Battery Protect
	#define	cBI_FlagT			bit10	// For Battery Protect
	#define	cBI_ThrottlingClear	bit11
	#define	cBI_BattOTPS4		bit12
	#define	cBI_BattStable		bit13	// F6C2 bit5
	#define	cBI_Charge2ndBatt	bit14	// For 2nd battery platform, 0: charge main battery, 1: charge 2nd battery
	#define	cReserveC2_bit15	bit15

extern XBITS_16 	cOsLedCtrl;			//_at_ OEMRAM5+0xB5
	#define cOL_SysLed	        bit0	// System LED Controlled by OS
	#define cOL_PwrLed	        bit1	// Power management LED Controlled by OS
	#define cOL_ChgLed	       	bit2	// Charge LED Controlled by OS
	#define cOL_DisChgLed	    bit3	// Dischage LED Controlled by OS
	#define cOL_TPActLed	    bit4	// TP active LED Controlled by OS
	#define cOL_BTLed	        bit5	// Blue tooth LED Controlled by OS
	#define cOL_TPLockLed	    bit6	// TP lock LED Controlled by OS
	#define cOL_CtrlRight       bit7	// OS get the Control right if set
	#define cOL_HddLed          bit8    // HDD LED Controlled by OS
    #define cOL_FddLed      	bit9    // FDD LED Controlled by OS
    #define cOL_MediaLed		bit10   // Media LED Controlled by OS
    #define cOL_MailLed			bit11   // E-Mail LED Controlled by OS
    #define cOL_WLANLed			bit12   // Wireless LAN LED Controlled by OS
    #define cOL_TVLed			bit13	// TV LED Controlled by OS
	#define	cOL_NumlockLed		bit14	// Numlock LED Controlled by OS
	#define	cOL_PadslockLed		bit15	// Padslock(Cursorlock) LED Controlled by OS

extern XBITS_16 	cOsLed1Ctrl;		//_at_ OEMRAM5+0xB7
	#define	cOL1_CapslockLed	bit0	// Capslock LED Conrtrolled by OS
	#define	cOL1_ScrolllockLed	bit1	// Scrolllock LED Conrtrolled by OS
	#define	cOL1_SpecialLed1	bit2	// Turbo LED
	#define	cOL1_KBBacklight	bit3
	#define	cOL1_SpecialLed3	bit4
	#define	cOL1_SwitchableBtnLed	bit5
	#define	cOL1_MuteLED		bit6
	#define	cOL1_VolUPLED		bit7
	#define	cOL1_VolDOWNLED 	bit8
	#define	cOL1_Fun1LED	 	bit9
	#define	cOL1_Fun2LED		bit10
	#define	cOL1_PwrAmber       bit11
extern XBYTE	initMark;			//OEMRAM5+0xB9 CMW 20121203 for PS8625 translator

extern XBYTE	TEMP_Error; 		//OEMRAM5+0xBA CMW 20121203 for PS8625 translator
	
extern XBYTE    EM9_NEWFUN;         	//_at_ OEMRAM5+0xBB;    
	#define   b0_FnPlus	       BIT0
	#define	  b1_FnMinus	   BIT1
	#define   b2_RCtrlPress    BIT2            
	#define   b3_LCtrlPress    BIT3
	#define   b4_RAltPress     BIT4
	#define   b5_LAltPress     BIT5
	#define   b6_RShiftPress   BIT6
    #define   b7_LShiftPress   BIT7      

//MEILING033:add start.
extern XBYTE	testtoolflag;			//_at_(OEMRAM5+0xBC); 
extern XBYTE	Bioswatchdog;			//_at_(OEMRAM5+0xBD); 
extern XBYTE	Bioswatchdogtime;		//_at_(OEMRAM5+0xBE); 

extern XBITS_8	cGPUThrottlingSet;  	//_at_ OEMRAM5+0xCA; 
extern XBYTE	cGPUThermalThrottling;	//_at_ OEMRAM5+0xC0;
extern XBYTE	cGPUBattThrottling;		//_at_ OEMRAM5+0xC1;
extern XBYTE    cGPUBattOTPThrottling;  //_at_ OEMRAM5+0xC2;
extern XBYTE    cGPUBattLowThrottling;  //_at_ OEMRAM5+0xC3;
//MEILING033:add end.

//MEILING055:S+.
extern XBYTE    cGPUACtoBattThrottling; //_at_ OEMRAM5+0xC4;
extern XBYTE    cGPUACtoBattTime;       //_at_ OEMRAM5+0xC5;
extern XBYTE    GPUProchotONCnt;        //_at_ OEMRAM5+0xC6;
extern XBYTE    cBATTLowThrottling;   	//_at_ OEMRAM5+0xC7;
//MEILING055:E+.
extern XBYTE    cGPUBattOCPThrottling;  //_at_ OEMRAM5+0xC8; //ANGELAG017: add
extern XBYTE    BatteryOCPDelay;        //_at_ OEMRAM5+0xC9; //ANGELAG017: add

extern XBYTE    GPU_Prochot;   	//_at_ OEMRAM5+0xCB; ///ANGELAG020: add
#define b0_Thermal BIT0 //ANGELAG020: add
#define b1_Psys 		BIT1 //ANGELAG024: add
#define b2_battery_OCP 	BIT2 //ANGELAG024: add
#define b3_battery_OTP 	BIT3 //ANGELAG024: add
#define b4_battery_low 	BIT4 //ANGELAG020: add

extern XBYTE	 ONEKEY_FLAG;//		_at_ (OEMRAM5+0xCC); //HEGANGS021:one key wake
extern XBYTE      FanTable_Type;//    		_at_(OEMRAM5+0xCD);
extern XBYTE	 ONEKEY_TEMP_FLAG;//		_at_ (OEMRAM5+0xCE); //HEGANGS021:one key wake
extern XBYTE    Proch_Delay;//		_at_ (OEMRAM5+0xCF);
extern XBYTE    Proch_Delay1;//		_at_ (OEMRAM5+0xD0);
extern XBYTE    cACIRQEventHappened;    //   _at_ (OEMRAM5+0xD2);	//THOMASG012:add.

extern XBYTE      Fan1RPM_Step1;    //  			_at_ (OEMRAM5+0xE0);
extern XBYTE      Fan1RPM_Step2;    //  			_at_ (OEMRAM5+0xE1); 
extern XBYTE      Fan1RPM_Step3;    //  			_at_ (OEMRAM5+0xE2);
extern XBYTE      Fan1RPM_Step4;    //  			_at_ (OEMRAM5+0xE3); 
extern XBYTE     Fan1RPM_Step5;    // 			_at_ (OEMRAM5+0xE4); 
extern XBYTE      Fan1RPM_Step6;    //  			_at_ (OEMRAM5+0xE5); 
extern XBYTE     Fan1RPM_Step7;    //  			_at_ (OEMRAM5+0xE6); 
extern XBYTE     Fan1RPM_Step8;    //  			_at_ (OEMRAM5+0xE7); 
extern XBYTE     Fan2RPM_Step1;    //  			_at_ (OEMRAM5+0xE8); 
extern XBYTE     Fan2RPM_Step2;    //  			_at_ (OEMRAM5+0xE9); 
extern XBYTE     Fan2RPM_Step3;    //  			_at_ (OEMRAM5+0xEA); 
extern XBYTE     Fan2RPM_Step4;    //  			_at_ (OEMRAM5+0xEB); 
extern XBYTE     Fan2RPM_Step5;    // 			_at_ (OEMRAM5+0xEC); 
extern XBYTE     Fan2RPM_Step6;    // 			_at_ (OEMRAM5+0xED); 
extern XBYTE     Fan2RPM_Step7;    // 			_at_ (OEMRAM5+0xEE); 
extern XBYTE     Fan2RPM_Step8;    // 			_at_ (OEMRAM5+0xEF); 
extern XBYTE     Fan1Level;    // 			    _at_(OEMRAM5+0xF8); 
extern XBYTE     Fan2Level;    //			    _at_(OEMRAM5+0xF9);
extern XBYTE     FanLevel;    //				_at_(OEMRAM5+0xF7);


extern XBYTE 	 FAN1Count;				//_at_ (OEMRAM5+0xD3); 
extern XBYTE 	 FAN2Count;				//_at_ (OEMRAM5+0xD4); 
extern XBYTE 	 HighPerf;				//_at_ (OEMRAM5+0xD5); //HEGANGS049:Add the 530s Highper formance and yoga30 quite mode function
extern XBYTE 	 QuiteMode;				//_at_ (OEMRAM5+0xD6); 

extern XBYTE     Fan1RPMTemp;    // 				_at_(OEMRAM5+0xFC); 
extern XBYTE     Fan2RPMTemp;    // 				_at_(OEMRAM5+0xFD);  
extern XBYTE     Fan1RPMCount;    // 				_at_(OEMRAM5+0xFE); 
extern XBYTE     Fan2RPMCount;    // 				_at_(OEMRAM5+0xFF);

extern XBYTE   Fan1RPM_TargetBackUp;    // 		    _at_(OEMRAM5+0x47);   
extern XBYTE  Fan2RPM_TargetBackUp;    // 		    _at_(OEMRAM5+0x48);

extern XBYTE   Fan1RPM_Target;    // 		    _at_(OEMRAM5+0x49);   
extern XBYTE  Fan2RPM_Target;    // 		    _at_(OEMRAM5+0x4A);
//*******************************************************************************
//-------------------------------------------------------------------------------
//	0x600-0x6FF   OEM RAM 6
//-------------------------------------------------------------------------------
//*******************************************************************************
  
	extern XBYTE   delayEDPTm;                           // _at_ OEMRAM6+0xBB
	
	extern XBYTE		cCHIPType;//						_at_ OEMRAM6+0xBC;	// 01: 8586EX
	extern XBYTE	VFAN_Tab_Step3;					//_at_(OEMRAM6+0xBD);
	extern XBYTE    FAN_Tab_Step3;                  //_at_(OEMRAM6+0xBE);
//*******************************************************************************
//-------------------------------------------------------------------------------
//	0x700-0x7FF   OEM RAM 7
//-------------------------------------------------------------------------------
//*******************************************************************************
// Keil C

//*******************************************************************************
//-------------------------------------------------------------------------------
//	0x800-0x8FF   OEM RAM 8
//-------------------------------------------------------------------------------
//*******************************************************************************
// Keyboard matrix
extern XBYTE KB_RAM_Table[128];					//_at_ OEMRAM8+0x00;	// 0x00
extern XBYTE ExKB_RAM_Table[24];				//_at_ OEMRAM8+0x80;	// 0x80
//72JERRY060: Start UCSI feature memory define
extern XBYTE   UCSI_Q20_Retry;                //_at_(OEMRAM8+0x9B);//72JERRY076: Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
extern XBYTE   TYPEC_Adapter_Watts;            // _at_(OEMRAM8+0x9C);
extern XBYTE   UCSI_Tunnel_Proc;                //_at_(OEMRAM8+0x9D); //brooke start
extern XBYTE   UCSI_Tunnel_Step;                //_at_(OEMRAM8+0x9E);
extern XBYTE   UCSI_Int_status;                 //_at_(OEMRAM8+0x9F);
#define b3UCSIIntStatus    BIT3

extern T_UCSI_DATA pUCSI;		                //_at_(OEMRAM8+0xA0); //brooke end //0xA0-0xCF total 48bytes. 72JERRY075
//extern T_PPM_HANDLE pPPM;		                //_at_(OEMRAMF+0xA0); //brooke end 72JERRY075
extern XBYTE   UCSI_Data_status;               //_at_(OEMRAM8+0xFF);
#define b7OSDataReady 	  BIT7
#define b6CCGDataReady      BIT6     
#define b5CmdCompletedIndicator BIT5 //72JERRY075    
#define b4QeventBIOSReceived BIT4            
#define b3Cmd04_ACK_CC_CI    BIT3    //72JERRY075      
//72JERRY060:End
//*******************************************************************************
//-------------------------------------------------------------------------------
//	0x900-0x9FF   OEM RAM 9
//-------------------------------------------------------------------------------
//*******************************************************************************
extern XBYTE MMIO_rDATA_index;					//_at_ OEMRAM9+0x81;	// 0x81
extern XBYTE MMIO_CMD_Ctrl;						//_at_ OEMRAM9+0x82;	// 0x82
#define MMIO_Processing_flage    BIT0
#define MMIO_Start_flage	     BIT1
extern XBYTE MMIO_rDATA[];						//_at_ OEMRAM9+0x83;	// 0x83 ~ 0x8A
/*
extern XBYTE MMIO_rDATA83;						//_at_ OEMRAM9+0x83;	// 0x83
extern XBYTE MMIO_rDATA84;						//_at_ OEMRAM9+0x84;	// 0x84
extern XBYTE MMIO_rDATA85;						//_at_ OEMRAM9+0x85;	// 0x85
extern XBYTE MMIO_rDATA86;						//_at_ OEMRAM9+0x86;	// 0x86
extern XBYTE MMIO_rDATA87;						//_at_ OEMRAM9+0x87;	// 0x87
extern XBYTE MMIO_rDATA88;						//_at_ OEMRAM9+0x88;	// 0x88
extern XBYTE MMIO_rDATA89;						//_at_ OEMRAM9+0x89;	// 0x89
extern XBYTE MMIO_rDATA9A;						//_at_ OEMRAM9+0x8A;	// 0x8A
*/
extern XBYTE MMIO_iCMD;							//_at_ OEMRAM9+0x8B;	// 0x8B
extern XBYTE MMIO_iDATA8C;						//_at_ OEMRAM9+0x8C;	// 0x8C
extern XBYTE MMIO_iDATA8D;						//_at_ OEMRAM9+0x8D; 	// 0x8D
extern XBYTE MMIO_iDATA8E;						//_at_ OEMRAM9+0x8E; 	// 0x8E
extern XBYTE MMIO_iDATA8F;						//_at_ OEMRAM9+0x8F; 	// 0x8F
extern XBYTE MMIO_iDATA90;						//_at_ OEMRAM9+0x90; 	// 0x90

extern XBYTE FnStatus;							//_at_ OEMRAM9+0xE0;

extern XBYTE  	SysStatus;                      //_at_ OEMRAM9+0xF0;
#define	LidStatus		    BIT0
#define	ERR_ShuntDownFlag	BIT1
#define FnF3WinKeyDn		BIT2
#define FnF3PKeyUp			BIT3
#define EnterDS3byLid		BIT4
#define CloseLid			BIT5
#define EnterS3Lid			BIT6
#define LidKBIgnore         BIT7

extern XBYTE LID_DEBOUNCE_CNT;                  //_at_ OEMRAM9+0xF1;
extern XBYTE PWRBTN_pressCnt;   				//_at_ OEMRAM9+0xF2;
//extern XBYTE LID_readytime; 					//_at_ OEMRAM9+0xF3; //ANGELAS007:Optimize power on sequence.
//extern XBYTE NTC_FLAG;  //ANGELAS007:Optimize power on sequence. //ANGELAS038:remove  
extern XBYTE ExtTimeCnt;                        //_at_ OEMRAM9+0xF4;

extern XBYTE SysStatus2;       		            //_at_ OEMRAM9+0xF5;
#define	PWRBTN_press_retry	BIT0
#define	b1NOVOBTN_retry		BIT1
//#define	b2NOVOKey_Press		BIT2      
#define	b3EC8S_Reset		BIT3      

extern XBYTE EC_ON_Flag;      		                                 //_at_ OEMRAM9+0xF6; 
extern XBYTE PWRBTN_press_debounce;      		                                 //_at_ OEMRAM9+0xF7; //72JERRY064:Modify press button 1s enter S3/S4/shutdown follow UI spec.
extern XBYTE TPID1;						        //_at_ OEMRAM9+0xFA;
extern XBYTE TPID2;						        //_at_ OEMRAM9+0xFB;
extern XBYTE TPID3;						        //_at_ OEMRAM9+0xFC;
extern XBYTE TPID4;						        //_at_ OEMRAM9+0xFD;

extern XBYTE LENOVOPMFW_Temp;                   //_at_ OEMRAM9+0xFF;
#define BATTERY_CYCLE_RUNNING	BIT0 		    //battery Cycle mode running  // Protection Mode
#define BATTERY_CALIBRATION_OK  BIT1            // Calibration Finish
#define BATTERY_FULLED_100      BIT2            // 100% Fulled ,from nonFull.
#define b3EC_ONStatus       	BIT3            // EC not entry Sleep status.
#define EEPROM_Token            BIT4
#define b5EC_ON_EN           	BIT5			// 1: EC turn on always electricity.
#define Auto_Mode_Off           BIT6
#define Auto_Mode_Change        BIT7

//*******************************************************************************
//-------------------------------------------------------------------------------
//	0xA00-0xAFF   OEM RAM A
//-------------------------------------------------------------------------------
//*******************************************************************************


//*******************************************************************************
//-------------------------------------------------------------------------------
//	0xB00-0xBFF   OEM RAM B
//-------------------------------------------------------------------------------
//*******************************************************************************

extern XBYTE TEMP_Buff_1;						//_at_ OEMRAMB + 0x00;
extern XBYTE TEMP_Buff_2;						//_at_ OEMRAMB + 0x01;
extern XBYTE TEMP_Buff_3;						//_at_ OEMRAMB + 0x02;
extern XBYTE EXT_Duty;							//_at_ OEMRAMB + 0x03;
extern XBYTE Target_Duty;						//_at_ OEMRAMB + 0x04;
extern XBYTE MBID_Reload;						//_at_ OEMRAMB + 0x05;
#define b0MBID_High 		BIT0				// MBID reload bit.
#define b1MBID_LOW			BIT1				// MBID reload bit.

extern XBYTE Fan_ON_Count;						//_at_ OEMRAMB + 0x06;
extern XBYTE Fan_Debug_Temp;					//_at_ OEMRAMB + 0x07;
#define b0CPU_Temp 			BIT0				// CPU Temperature.
#define b1VGA_Temp 			BIT1				// VGA Temperature.
#define b2EXTVGA_Temp 		BIT2				// SLI VGA Temperature.

extern XBYTE VGA_TBuff1;						//_at_ OEMRAMB + 0x08;
extern XBYTE VGA_TBuff2;						//_at_ OEMRAMB + 0x09;
extern XBYTE VGA_TBuff3;						//_at_ OEMRAMB + 0x0A;
extern XBYTE WLAN_OFF_Cnt;						//_at_ OEMRAMB + 0x0B;
extern XBYTE Average_Temp;						//_at_ OEMRAMB + 0x0C;	// Average temperature stage
extern XBYTE SLI_Status;						//_at_ OEMRAMB + 0x0D;
#define b0ODD_Mode 			BIT0				// SLI ODD Mode.
#define b1HDD_Mode 			BIT1				// SLI HDD Mode.
#define b2GPU_Mode 			BIT2				// SLI GPU Mode.
#define b3No_Fun 			BIT3				// No external device.
#define b4ODD_HDD 			BIT4				// SLI ODD/HDD Mode for EC debug.
#define b5CmdCtrl			BIT5				// For AP used.
#define b6FAN_Mode 			BIT6				// SLI FAN Mode for EC debug.
#define b7ECNo_Fun 			BIT7				// No external device for EC debug.

extern XBYTE USB_Charger;						//_at_ OEMRAMB + 0x0E;
#define b0USB_EN 			BIT0				// USB charger bit, 1:Enable.
#define b1USB_initOK		BIT1				// USB charger bit, 1:initial finish.
#define b2USB_TmlDis		BIT2				// USB charger bit, 1:Disable USB charger.
#define b3USB_IDdeounce		BIT3				// USB charger bit, 1:SMSC charger IC Fail.


extern XBYTE USB_Error;							//_at_ OEMRAMB + 0x0F;
#define b0USB_OverILIM 		BIT0				//
#define b1USB_Back_Volt		BIT1				//
#define b2USB_Over_Volt		BIT2				//
#define b3USB_TSD			BIT3				//
#define b4USB_MinKeepOut	BIT4				//
#define b5USB_RESET			BIT5				//
#define b6USB_DischgErr 	BIT6				//
#define b7USB_ErrorCtl		BIT7				//

extern XBYTE MuteCount;							//_at_ OEMRAMB + 0x10;
extern XBYTE EXT_VGA_Buff1;						//_at_ OEMRAMB + 0x11;
extern XBYTE EXT_VGA_Buff2;						//_at_ OEMRAMB + 0x12;
extern XBYTE EXT_VGA_Buff3;						//_at_ OEMRAMB + 0x13;
extern XBYTE Thro_Status;						//_at_ OEMRAMB + 0x14;
#define	b0ProCH_CPU			BIT0
#define	b1ProCH_VGA			BIT1
#define	b2ProCH_EXTVGA		BIT2
#define	b3Turbo_CPU			BIT3
#define	b4Turbo_VGA			BIT4
#define	b5Turbo_EXTVGA		BIT5
#define	b6Turbo_DC_CPU		BIT6

#define	b7ProCH_BATT		BIT7    

extern XBYTE Thro_Status2;						//_at_ OEMRAMB + 0x15;
#define	b0Steer_CPU			BIT0
#define	b1Steer_VGA			BIT1
#define	b2Steer_EXTVGA		BIT2
#define b3Batt_OTP          BIT3  //MEILING033:add.
#define	b4FAN1_FullOn		BIT4
#define	b5Bat_5PProtect		BIT5   
#define b6BattRSOC_Low      BIT6  //MEILING033:add.
//#define	b7ProCH_ADP			BIT7  
#define b7ProCH_Psys       BIT7 

extern XBYTE CPU_Shut_Cnt;						//_at_ OEMRAMB + 0x16;
extern XBYTE VGA_Shut_Cnt;						//_at_ OEMRAMB + 0x17;
extern XBYTE EXTVGA_Shut_Cnt;					//_at_ OEMRAMB + 0x18;
extern XBYTE LED_KB_PWM_Count;					//_at_ OEMRAMB + 0x19;
extern XBYTE FAN2PRM; 							//_at_ OEMRAMB + 0x1A;
extern XBYTE FAN_CpTarget; 						//_at_ OEMRAMB + 0x1B;
extern XBYTE SMSC_ChgDelay;						//_at_ OEMRAMB + 0x1C;
extern XBYTE USB_ChargerStep;					//_at_ OEMRAMB + 0x1D;
extern XBYTE ManualFan2PRM;						//_at_ OEMRAM5 + 0xFA;
extern XWORD SHA1_SEED;							//_at_ OEMRAMB + 0x1F;

extern XBYTE uODDPWRsaving;						//_at_ OEMRAMB+0x21
#define uODD_PWRon			BIT0
#define uODD_BtnLowPluse	BIT1
#define uODD_PWRoff1		BIT2
#define uODD_delaytime		BIT3
#define uODD_low100ms		BIT4
#define uODD_ejectbtn		BIT5
#define uODD_InitEnd		BIT6
#define uODD_ODDWorkaround	BIT7
extern XBYTE uODDdelaytimeStep;					//_at_ OEMRAMB+0x22
extern XBYTE SHA1failCnt;						//_at_ OEMRAMB+0x23
extern XBYTE uWLBTLanTemp;						//_at_ OEMRAMB+0x24
extern XBYTE uODDlowCnt;						//_at_ OEMRAMB+0x25
extern XBYTE uODDtimeCnt;						//_at_ OEMRAMB+0x26
extern XBYTE Service_Auth_Step;					//_at_ OEMRAMB+0x27

extern XBYTE FAN_Std_Max;						//_at_ OEMRAMB + 0x28;
extern XBYTE FAN_DOS_Max;						//_at_ OEMRAMB + 0x29;
extern XBYTE FAN_Quite_Max;						//_at_ OEMRAMB + 0x2A;

//extern XBYTE FAN_SP_Max;						//_at_ OEMRAMB + 0x2B;
extern XBYTE FAN_RPM_L;                             //_at_ OEMRAMB + 0x2B;

extern XBYTE FAN_PWM_1;							//_at_ OEMRAMB + 0x2C;
extern XBYTE FAN_PWM_2;							//_at_ OEMRAMB + 0x2D;
extern XBYTE StatusKeeper;						//_at_ OEMRAMB + 0x2E;
#define	BatteryFwUpdate		BIT0
#define BatteryProtectCHG	BIT1
#define	b2CPU_Auto70		BIT2				// Bit2 CPU Auto mode.
#define	b3CPU_Auto			BIT3				// Bit3 CPU Auto mode.
#define	b4Bri_Cover			BIT4				// Bit4 Cover brightness default.
#define	b5TP_Event			BIT5				// Bit5 cover TP event.
#define	b6SLI_PWMDetect		BIT6				// Bit6 debug PWM detect.
#define b7SLI_SpdDetect		BIT7				// Bit7 debug Speed detect.

extern XBYTE ECSleepCount;						//_at_ OEMRAMB + 0x2F;

extern XBYTE TouchPadCount;						//_at_ OEMRAMB + 0x30;
extern XBYTE EEPROM_PwrSts;						//_at_ OEMRAMB + 0x31;

extern XWORD  FAN1_RPM;				//_at_ OEMRAMB + 0x32;  

extern XBYTE PwrOnDly5Sec;				//_at_ OEMRAMB + 0x34; 

extern XBYTE SHA1FailRetry;                      //_at_ OEMRAMB + 0x35; 

//ANGELAS043: add start
extern XBYTE RSOC1PTO0PCount;						//_at_ OEMRAMB + 0x36;  
extern XBYTE RSOC1PTO0PSaveSpace;					//_at_ OEMRAMB + 0x37;
#define RSOCIs1P  BIT0
#define RSOCIs0P  BIT1
//ANGELAS043:add end

extern XBYTE SLI_GPUMax;						//_at_ OEMRAMB + 0x38;
extern XBYTE SLI_GPUPWM1;						//_at_ OEMRAMB + 0x39;
extern XBYTE SLI_GPUPWM2;						//_at_ OEMRAMB + 0x3A;
extern XBYTE SLI_FANMax;						//_at_ OEMRAMB + 0x3B;
extern XBYTE SLI_FANPWM1;						//_at_ OEMRAMB + 0x3C;
extern XBYTE SLI_FANPWM2;						//_at_ OEMRAMB + 0x3D;

extern XBYTE FAN_PWM_3;                     	//_at_ OEMRAMB + 0x3E;  //MEILING003:add.

extern XBYTE Thro_Status3;						//_at_ OEMRAMB + 0x3F;  //MEILING040:add.
#define b0IntGPU_TURBO  BIT0  //MEILING040:add.

extern LWORD K[];								//_at_ OEMRAMB + 0x40;
extern LWORD RBATH0;							//_at_ OEMRAMB + 0x60; 	// 0x0B60 ~ 0x0B63
extern LWORD RBATH1;							//_at_ OEMRAMB + 0x64; 	// 0x0B64 ~ 0x0B67
extern LWORD RBATH2;							//_at_ OEMRAMB + 0x68; 	// 0x0B68 ~ 0x0B6B
extern LWORD RBATH3;							//_at_ OEMRAMB + 0x6C; 	// 0x0B6C ~ 0x0B6F
extern LWORD RBATH4;							//_at_ OEMRAMB + 0x60; 	// 0x0B60 ~ 0x0B73
extern LWORD BATchallenger[];		//_at_ OEMRAMB + 0x80; 	// 0x0B80 ~ 0x0B93
extern LWORD sdAMDTH0;
extern LWORD sdAMDTH1;
extern LWORD sdAMDTH2;
extern LWORD sdAMDTH3;
extern XBYTE Tgpuproup;        //:Add register for thermal test.
extern XBYTE Tgpuprodown;     //:Add register for thermal test.
extern XBYTE local_Shut_Cnt;
extern XBYTE remote_Shut_Cnt;   
extern XBYTE ShutDnCause;	                	// _at_ OEMRAMB + 0xA3;    
extern XBYTE FunctionKeyDebounce;	           	// _at_ OEMRAMB + 0xA4; //ANGELAS084:add
//72JERRY014: s+Add the second fan method
extern XWORD  FAN2_RPM;				//_at_ OEMRAMB + 0xA5;  
extern XBYTE FAN1_Std_Max;						//_at_ OEMRAMB + 0xA7;
extern XBYTE FAN1_DOS_Max;						//_at_ OEMRAMB + 0xA8;
extern XBYTE FAN1_Quite_Max;						//_at_ OEMRAMB + 0xA9;
extern XBYTE FAN1_RPM_L;                             //_at_ OEMRAMB + 0xAA;
extern XBYTE FAN1_PWM_1;							//_at_ OEMRAMB + 0xAB;
extern XBYTE FAN1_PWM_2;							//_at_ OEMRAMB + 0xAC;
extern XBYTE FAN1_PWM_3;                     	//_at_ OEMRAMB + 0xAD;  //MEILING003:add.

extern XBYTE	VFAN1_Tab_OFF;					//_at_(ECRAMB+0xAE);
extern XBYTE	VFAN1_Tab_LOW;					//_at_(ECRAMB+0xAF);
extern XBYTE	VFAN1_Tab_Step1;					//_at_(ECRAMB+0xB0);
extern XBYTE	VFAN1_Tab_Step2;					//_at_(ECRAMB+0xB1);
extern XBYTE	VFAN1_Tab_HI;					//_at_(ECRAMB+0xB2);
extern XBYTE	VFAN1_Tab_Step3;					//_at_(OEMRAMB+0xB3);
extern XBYTE    FAN1_Tab_Step3;                  //_at_(OEMRAMB+0xB4);
extern XBYTE	FAN1_Tab_OFF;					//_at_(ECRAMB+0xB5);
extern XBYTE	FAN1_Tab_LOW;					//_at_(ECRAMB+0xB6);
extern XBYTE	FAN1_Tab_Step1;					//_at_(ECRAMB+0xB7);
extern XBYTE	FAN1_Tab_Step2;					//_at_(ECRAMB+0xB8);
extern XBYTE	FAN1_Tab_HI;						//_at_(ECRAMB+0xB9);

extern XBYTE	ManualFan1PRM;					//_at_(ECRAMB+0xBB);
//72JERRY057:s+Update fan table follow'720S_13IKB fan table_V03'.
extern XBYTE TEMP_Buff_4;						//_at_ OEMRAMB + 0xBC;
extern XBYTE TEMP_Buff_5;						//_at_ OEMRAMB + 0xBD;
extern XBYTE TEMP_Buff_6;						//_at_ OEMRAMB + 0xBE;
extern XBYTE TEMP_Buff_7;						//_at_ OEMRAMB + 0xBF;
extern XBYTE TEMP_Buff_8;						//_at_ OEMRAMB + 0xC0;
extern XBYTE TEMP_Buff_9;						//_at_ OEMRAMB + 0xC1;
extern XBYTE TEMP_Buff_10;						//_at_ OEMRAMB + 0xC2;
extern XBYTE TEMP_Buff_0;						//_at_ OEMRAMB + 0xC3;
//72JERRY057:e+Update fan table follow'720S_13IKB fan table_V03'.
//72JERRY014: e+Add the second fan method
//*******************************************************************************
//-------------------------------------------------------------------------------
//	0xC00-0xCFF   OEM RAM C
//-------------------------------------------------------------------------------
//*******************************************************************************
#define ECRAMC				OEMRAMC
#define ECRAMD				OEMRAMD

extern LWORD	SHA1_W[];							//_at_ ECRAMC; // 0x0C00 ~ 0x0D3F

extern LWORD	WS_temp;							//_at_ ECRAMD+0x40; // 0x0D40 ~ 0x0D4F

extern LWORD	SHA1_a;								//_at_ ECRAMD+0x50; // 0x0D50 ~ 0x0D53
extern LWORD	SHA1_b;								//_at_ ECRAMD+0x54; // 0x0D54 ~ 0x0D57
extern LWORD	SHA1_c;								//_at_ ECRAMD+0x58; // 0x0D58 ~ 0x0D5B
extern LWORD	SHA1_d;								//_at_ ECRAMD+0x5C; // 0x0D5C ~ 0x0D5F
extern LWORD	SHA1_e;								//_at_ ECRAMD+0x60; // 0x0D60 ~ 0x0D63

extern LWORD	H0;									//_at_ ECRAMD+0x64; // 0x0D64 ~ 0x0D67
extern LWORD	H1;									//_at_ ECRAMD+0x68; // 0x0D68 ~ 0x0D6B
extern LWORD	H2;									//_at_ ECRAMD+0x6C; // 0x0D6C ~ 0x0D6F
extern LWORD	H3;									//_at_ ECRAMD+0x70; // 0x0D70 ~ 0x0D73
extern LWORD	H4;									//_at_ ECRAMD+0x74; // 0x0D74 ~ 0x0D77

extern LWORD	K0;									//_at_ ECRAMD+0x78; // 0x0D78 ~ 0x0D7B

extern LWORD	Hn0;								//_at_ ECRAMD+0x7C; // 0x0D7C ~ 0x0D7F
extern LWORD	Hn1;								//_at_ ECRAMD+0x80; // 0x0D80 ~ 0x0D83
extern LWORD	Hn2;								//_at_ ECRAMD+0x84; // 0x0D84 ~ 0x0D87
extern LWORD	Hn3;								//_at_ ECRAMD+0x88; // 0x0D88 ~ 0x0D8B
extern LWORD	Hn4;								//_at_ ECRAMD+0x8C; // 0x0D8C ~ 0x0D8F

extern LWORD	BATH0;								//_at_ ECRAMD+0x90; // 0x0D90 ~ 0x0D93
extern LWORD	BATH1;								//_at_ ECRAMD+0x94; // 0x0D94 ~ 0x0D97
extern LWORD	BATH2;								//_at_ ECRAMD+0x98; // 0x0D98 ~ 0x0D9B
extern LWORD	BATH3;								//_at_ ECRAMD+0x9C; // 0x0D9C ~ 0x0D9F
extern LWORD	BATH4;								//_at_ ECRAMD+0xA0; // 0x0DA0 ~ 0x0DA3

//extern XBYTE	Service_Auth_Step;					//_at_ ECRAMD+0xFF; // 0x0DFF
//*******************************************************************************
//-------------------------------------------------------------------------------
//	0xD00-0xDFF   OEM RAM D
//-------------------------------------------------------------------------------
//*******************************************************************************

//*******************************************************************************
//-------------------------------------------------------------------------------
//	0xE00-0xEFF   OEM RAM E
//-------------------------------------------------------------------------------
//*******************************************************************************

//*******************************************************************************
//-------------------------------------------------------------------------------
//	0xF00-0xFFF   OEM RAM F
//-------------------------------------------------------------------------------
//*******************************************************************************

//ANGELAS064:add start
#define DiagECRAM		        OEMRAMF     
#define DiagNameSpace			OEMRAMF 

extern XBYTE   nDiagInterface_CMD;             //_at_ OEMRAMF+0x00;  
extern XBYTE   nDiagInterface_DATA;            //_at_ OEMRAMF+0x01)  
#define  FanNum     BIT7   //0: Fan1, 1:Fan2     
#define  FanType    BIT5   //0: CPU, 1:VGA       
//BIT0~1:  Column; BIT2~4:  Row                         

extern XBYTE   nDiagInterface_ECRegL;          //_at_ OEMRAMF+0x02) //or used as EC Space offset
extern XBYTE   nDiagInterface_ECRegH;          //_at_ OEMRAMF+0x03) 

extern XBYTE   nDiagInterface_DATA_High;       //_at_ OEMRAMF+0x04)    

extern XBYTE 	SMB_PRTC_Diag;				   //_at_ OEMRAMF+0x60)// 0x60,
extern XBYTE 	SMB_STS_Diag;				   //_at_ OEMRAMF+0x61)// 0x61,
extern XBYTE 	SMB_ADDR_Diag;			       //_at_ OEMRAMF+0x62)// 0x62,
extern XBYTE 	SMB_CMD_Diag;				   //_at_ OEMRAMF+0x63)// 0x63,
extern XBYTE 	SMB_DATA_Diag;				   //_at_ OEMRAMF+0x64)// 0x64,
//0x64~0x83  for 32 bytes max block R/W 
extern XBYTE 	SMB_BCNT_Diag;				   //_at_ OEMRAMF+0x84)// 0x84,
// SBS Only Alarm registers  
extern XBYTE 	SMB_ALRA_Diag;				   //_at_ OEMRAMF+0x85)// 0x85,
extern XBYTE 	SMB_ALRD0_Diag;				   //_at_ OEMRAMF+0x86)// 0x86,
extern XBYTE 	SMB_ALRD1_Diag;				   //_at_ OEMRAMF+0x87)// 0x87,

extern XBYTE 	SMB_EC_Chennel_Diag;		   //_at_ OEMRAMF+0x88)// 0x88,

//ANGELAS064:add end
#endif
