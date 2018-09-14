//*****************************************************************************
//
//  oem_ucsi_cyp_ccg4.h
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author:
//
//*****************************************************************************

#ifndef OEM_UCSI_CYP_CCG4_H
#define OEM_UCSI_CYP_CCG4_H

#define UCSI_TUNNEL_DEBUG(X)     RamDebug(X)

/* ----------------------- */
/*  CCG4 TUNNEL HW Setting */
/* ----------------------- */
#define UCSI_PD_channel         SMbusCh1
#define CYPRESS_CCG4_UCSI_ADDR  0x90
#define CYPRESS_CCG4_HPI_ADDR   0x10

#define IS_UCSI_TUNNEL_INT()    IS_MASK_CLEAR(GPDRF,BIT4)


/* ----------------------- */
/*  CCG4 TUNNEL SW Setting */
/* ----------------------- */
#define UCSI_TUNNEL_VERSION     0x0100 //  V1.0


/* ----------------------- */
/*   UCSI region Offset  */
/* ----------------------- */
enum _UCSI_REGION_OFFSET_
{
	UCSI_Version = 0,
	UCSI_Reserved = 2,
	UCSI_Cci = 4, //72JERRY083:Change MMIO to mailbox for accessing UCSI registers.
	UCSI_Control = 8,//72JERRY083:Change MMIO to mailbox for accessing UCSI registers.
	UCSI_MessageIn = 16,
	UCSI_MessageOut = 32
};

/* ----------------------- */
/*   UCSI region SIZE  */
/* ----------------------- */
enum _UCSI_REGION_SIZE_
{
	UCSI_Version_SIZE = 2,
	UCSI_Reserved_SIZE = 2,
	UCSI_CCI_SIZE = 4,
	UCSI_CONTROL_SIZE = 8,
	UCSI_MessageIn_SIZE = 16,
	UCSI_MessageOut_SIZE = 16
};


/* ----------------------- */
/*   UCSI_Tunnel_Proc       */
/* ----------------------- */
enum _UCSI_TUNNEL_Proc_
{
	TUNNEL_WAIT_INIT = 1,
	TUNEL_IRQ_HANDLER,
	TUNNEL_SERVICE,
	TUNNEL_LPM_SERVICE,
	TUNNEL_OPM_SERVICE
};


/* ----------------------- */
/* Data structure  Field : [Table 3-1] */
/* ----------------------- */
typedef volatile struct _T_UCSI_DATA_
{
	/* BCD version of the UCSI Spec.(PPM->OPM) */
	WORD u16Version;		    // 0x00 - 0x01
	WORD u16Reserved;		    // 0x02 - 0x03

	/* USB Type-C Command Status & Connector Change Indication(PPM->OPM) */
	union
	{
		BYTE byte;
		struct
		{
			FIELD b1Reserved: 1;	//00: Reserved(1)
			FIELD b7CCI: 7;		//01: Connector Change Indicator(7)
		} field;
	} unCCI0;			        // 0x04

	union
	{
		BYTE byte;
		struct
		{
			FIELD b8DataLen: 8;		//08: Data Length(8)
		} field;
	} unCCI1;                   // 0x05

	union
	{
		BYTE byte;
		struct
		{
			FIELD b8Reserved: 8;		//16: Reserved(8)
		} field;
	} unCCI2;                   // 0x06

	union
	{
		BYTE byte;
		struct
		{
			FIELD b1Reserved: 1;			//24: Reserved(1)
			FIELD b1IdrNotSupported: 1;		//25: Not Supported Indicator(1)
			FIELD b1IdrCancelCompleted: 1;	//26: Cancel Completed Indicator(1)
			FIELD b1IdrResetCompleted: 1;	//27: Reset Completed Indicator(1)
			FIELD b1IdrBusy: 1;				//28: Busy Indicator(1)
			FIELD b1IdrAckCmd: 1;			//29: Ack Cmd Indicator(1)
			FIELD b1IdrError: 1;			//30: Error Indicator(1)
			FIELD b1IdrCmdCompleted: 1;		//31: Cmd Completed Indicator(1)
		} field;
	} unCCI3;                   // 0x07

	/* USB Type-C Control(OPM->PPM) */
	struct
	{
		BYTE u8Cmd;		    //Command           // 0x08
		BYTE u8DataLen;		//Data Length       // 0x09
		BYTE pCmdSpec[6];	//Command Specific  // 0x0A - 0x0F
	} stControl;

	/* USB Type-C Message In(PPM->OPM) */
	BYTE pMsgIn[16];                            // 0x10 - 0x1F
	/* USB Type-C Message Out(OPM->PPM) */
	BYTE pMsgOut[16];                           // 0x20 - 0x2F
} xdata T_UCSI_DATA, *P_UCSI_DATA;

/* ----------------------- */
/*  PPM Status (pPPM.u8State) */
/* ----------------------- */
typedef volatile struct _T_PPM_HANDLE_
{
	//T_UCSI_DATA *pUCSI;			// 0x00-0x01
	BYTE PDreg0;				// 0x02
	WORD u32CmdTimeout;		    // 0x03-0x04
	BYTE E0cmd;				    // 0x05, Change to 0xF0 via Port 0x6E as BIOS Vito's suggestions
	WORD u16NotifyMask;		    // 0x06-0x07
	WORD u16Event;				// 0x08-0x09
	WORD u16ErrorInfo;			// 0x0A-0x0B
	BYTE u8State;				// 0x0C
	BYTE u8ProcessCmd;			// 0x0D
	BYTE u8ProcessCmd_step;	    // 0x0E
	BYTE u8readINT_step;		// 0x0F
	BYTE u8OperationMode;		// 0x10
	union
	{
		BYTE byte;
		struct
		{
			FIELD sci_wait_ack : 1;
			FIELD ec_sci_int_ack : 1;
			FIELD ppm_reset_flag : 1;
		} field;
	} u8MISC;				    // 0x11
	union
	{
		BYTE byte;
		struct
		{
			FIELD external_supply : 1;	// 0x12
		} field;
	} u8ECevent;
	BYTE pdo[4];				// 0x13 0x14 0x15 0x16
} xdata T_PPM_HANDLE, *P_PPM_HANDLE;

/* ----------------------- */
/* OPM command: [Table A-51]  */
/* ----------------------- */
#define CMD_RESERVED					0x00
#define CMD_PPM_RESET				    0x01 //R (Required)
#define CMD_CANCEL				        0x02 //R
#define CMD_CONNECTOR_RESET			    0x03 //R
#define CMD_ACK_CC_CI				    0x04 //R
#define CMD_SET_NOTIFICATION_ENABLE		0x05 //R
#define CMD_GET_CAPABILITY			    0x06 //R
#define CMD_GET_CONNECTOR_CAPABILITY	0x07 //R
#define CMD_SET_UOM				        0x08 //O (Optional)
#define CMD_SET_UOR				        0x09 //R
#define CMD_SET_PDM				        0x0A //O
#define CMD_SET_PDR				        0x0B //R
#define CMD_GET_ALTERNATE_MODES			0x0C //O
#define CMD_GET_CAM_SUPPORTED			0x0D //O
#define CMD_GET_CURRENT_CAM			    0x0E //O
#define CMD_SET_NEW_CAM				    0x0F //O
#define CMD_GET_PDOS				    0x10 //O
#define CMD_GET_CABLE_PROPERTY			0x11 //O
#define CMD_GET_CONNECTOR_STATUS		0x12 //R
#define CMD_GET_ERROR_STATUS			0x13 //R
#define CMD_MAX_IDX				        0x14 //The count of OPM CMDs

/* ----------------------- */
/*   Function Extern       */
/* ----------------------- */
extern XBYTE ucsi_init(XBYTE *pBuffer);
extern XBYTE ucsi_run(void);
extern void ucsi_reset(void);//72JERRY083:Change MMIO to mailbox for accessing UCSI registers.
extern BYTE CCG4_UCSI_Reg_Read(BYTE Cmd_Region, BYTE *R_data_buffer, BYTE R_Length);
extern BYTE CCG4_UCSI_Reg_Write(BYTE Cmd_Region, BYTE *W_data_buffer, BYTE W_Length);
extern void TypecAdapterDetection(void);
#endif
