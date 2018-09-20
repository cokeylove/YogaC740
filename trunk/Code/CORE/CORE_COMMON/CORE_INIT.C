/*-----------------------------------------------------------------------------
 * TITLE: CORE_INIT.C
 *
 * Author : Dino
 *
 * Copyright (c) ITE INC. All Rights Reserved.
 *
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>
//----------------------------------------------------------------------------
// FUNCTION: cold_reset - Initialization when power goes on
//----------------------------------------------------------------------------
void cold_reset(void)
{
	Ext_Cb2.byte = EXT_CB2_INIT;    		/* Update Ext_Cb3 a little further down */
	Save_Typematic = SAVE_TYPEMATIC_INIT;
	Save_Kbd_State = (SAVE_KBD_STATE_INIT & ~maskLEDS) | ( 0& maskLEDS);

    Led_Ctrl = LED_CTRL_INIT;
    Led_Data = 0;
    Gen_Info = 0;   /* Clear general control. */

    Int_Var.Ticker_10 = 10;

    Flag.PASS_READY = 0;    /* Password not loaded. */
    Int_Var.Ticker_100 = 50;

    Flag.SCAN_INH = 1;      /* Scanner transmission inhibited. */
    Flag.VALID_SCAN = 0;

    Flag.NEW_PRI_K = 0;
    Flag.NEW_PRI_M = 0;

    Flag.LED_ON = 1;
    Ccb42 = CCB42_INIT;

    Pass_On = 0;      /* Password enable send code. */
    Pass_Off = 0;     /* Password disable send code. */
    Pass_Make1 = 0;   /* Reject make code 1. */
    Pass_Make2 = 0;   /* Reject make code 2. */

    MULPX = 0;

}


//----------------------------------------------------------------------------
// FUNCTION:   general_reset1, general_reset2
// Initialize things common to both Cold/Warm reset.
//----------------------------------------------------------------------------
static void common_reset(void)  /* MCHT, 27/05/04 */ // static void general_reset1(void)
{
    Service  = 0;       /* Clear service requests */
    Service1 = 0;

    Kbd_Response = 0;   /* Clear response codes */
    Timer_A.byte = 0;   /* Clear Timer A events */
    Timer_B.byte = 0;   /* Clear Timer B events */
    Scanner_State = 0;  /* Clear scanner state */
}


//----------------------------------------------------------------------------
// FUNCTION: Init_Mouse - Initialize the Intelligent mouse variables.
//----------------------------------------------------------------------------
void Init_Mouse (void)
{

}


void Core_Initialization(void)
{
	CLEAR_MASK(RAM_16FF,BIT0);		// for AX
	Init_GPIO();

#if	Support_External_IO
    Init_IT8302();
#endif	

    Core_Init_SMBus();		
	check8sreset(); 

    Init_Kbd();
	cold_reset();
	common_reset();
	Init_Mouse();
	Init_Timers();
    Core_Init_Regs();
	#if SUPPORT_UCSI
    ucsi_reset();//Change MMIO to mailbox for accessing UCSI registers.
    #endif

	//Add 8 sec reset test function.
	if(IS_MASK_CLEAR(SysStatus2, b3EC8S_Reset) && (ShutDnCause == SC_ECColdBoot))
	{
		BRAMBK0[126] = 0x00;
		BRAMBK0[127] = 0x00;
	}
	ResetTestFlag = BRAMBK0[126];
	RamDebug(0xC0);
	RamDebug(ResetTestFlag);
	switch(ResetTestFlag)
	{
	case 1:
		SET_MASK(GCR9, BIT3);
		GPCRI5 = ALT;
		RamDebug(0xC1);
		break;
	case 2:
		CLEAR_MASK(GCR9,BIT3);
		GPCRI5 = BRAMBK0[127];
		RamDebug(0xC2);
		break;
	default:
		CLEAR_MASK(GCR9,BIT3);
		RamDebug(0xC3);
		break;
	}
	RamDebug(GCR9);
	RamDebug(GPCRI5);
	//MEILING009:E+ enable 8s EC reset and GPO out signal from GPI5.
}

//----------------------------------------------------------------------------
// The function of clearing internal and external ram for kernel code
// Clear external ram (0x000~0x0FF)
// Clear internal ram (0x20~0xCF)
//----------------------------------------------------------------------------
void Core_Init_ClearRam(void)
{
	BYTE Temp0;
    BYTE Temp1;
	BYTE Temp2;
	BYTE Temp3;//ANGELAS094:Add workaround for hang bios.
	BYTE Temp4;//HEGANGS022£ºAdd SMI hang check function
	IIBYTE *IdataIndex;
	PORT_BYTE_PNTR byte_register_pntr;
	IdataIndex = 0x20;
    byte_register_pntr=0;
	Temp0 = WinFlashMark;				//msmart// 
	Temp1 = WinFlashMark2;
	Temp2 = WinFlashMark1;				// G21: Add WinFlashMark1 check
	Temp3 = bios_count;//ANGELAS094:Add workaround for hang bios.
	Temp4 = bios_check; //HEGANGS022£ºAdd SMI hang check function
    while(byte_register_pntr<0x100)		// Clear external ram (0x000~0xFFF)
    {
        *byte_register_pntr=0;
        byte_register_pntr ++;
    }
	WinFlashMark = Temp0;				//msmart
	WinFlashMark2 = Temp1;				
	WinFlashMark1 = Temp2;				//G21: Add WinFlashMark1 check
	bios_count = Temp3;//ANGELAS094:Add workaround for hang bios.
	bios_check = Temp4;//HEGANGS022£ºAdd SMI hang check function

	while(IdataIndex<0xC0)				// Clear internal ram (0x20~0xBF)
    {
        *IdataIndex=0;
        IdataIndex ++;
    }
}

//----------------------------------------------------------------------------
// The function of init. registers of kernel code
//----------------------------------------------------------------------------
const sREG_INIT code Core_reg_init_table[] =
{
 		{ &FMSSR 		,Core_Init_FMSSR    }, 	// share rom size
// HOST interface
    	{ &SPCTRL1      ,Core_Init_I2EC     },  // Enable I2EC R/W
    	{ &BADRSEL      ,Core_Init_BADR     },  // Base address select 0b00:2E 2F 0b01:4E 4F 0b10:User define
    	{ &SWCBALR      ,Core_Init_SWCBALR  },
    	{ &SWCBAHR      ,Core_Init_SWCBAHR  },
		{ &RSTS			,0x84				},
		{ &SPECTRL2 	,Core_Init_SPCTRL2	},	// Enable port I2EC Enable
 		{ &PI2ECH		,Core_Init_PI2ECH	},
		{ &PI2ECL		,Core_Init_PI2ECL	},
// KBC and PM interface
		{ &KBIRQR 		,0x00              	}, 	// Disable KBC IRQ
        { &KBHICR 		,IBFCIE+PM1ICIE     }, 	// KBC port control IBFCIE+PMICIE
        { &PM1CTL		,IBFIE+SCINP        }, 	// EC port control  IBFCIE+SCI low active
        { &PM2CTL 		,IBFIE+SCINP        }, 	// EC port2 control IBFCIE+SCI low active
// PS2
        { &PSCTL1 		,PS2_InhibitMode    }, 	// Enable Debounce,Receive mode, Inhibit CLK
        { &PSCTL2 		,PS2_InhibitMode    }, 	// Enable Debounce,Receive mode, Inhibit CLK
        { &PSCTL3 		,PS2_InhibitMode    }, 	// Enable Debounce,Receive mode, Inhibit CLK
        #if TouchPad_only    //Update PS2 code base.
        { &PSINT1		,0x04				}, 	// Transation Done,Start Clock Interrupt
        { &PSINT2		,0x04				},	// Transation Done,Start Clock Interrupt
        { &PSINT3		,0x04				},	// Transation Done,Start Clock Interrupt
        #else
		{ &PSINT1		,0x06				}, 	// Transation Done,Start Clock Interrupt
        { &PSINT2		,0x06				},	// Transation Done,Start Clock Interrupt
        { &PSINT3		,0x06				},	// Transation Done,Start Clock Interrupt
        #endif
// Key Scan
        { &KSOCTRL   	,KSOOD + KSOPU      },	//
  		{ &KSICTRL   	,KSIPU       		},	//
//
 		{ &HOCTL2R		,0x02				},	//LMLKBL0016:Modify from 0x40 to 0x01.  //LMLKBL0017:change from 0x01 to 0x02.
 		{ &HINSTC2		,0x30				},	// Host Indirect & LPC Memory Cycle Target Slect e-Flash
 		// CMW 20121022 
 		{ &FLHCTRL3R	,0x01				},	// Force F-SPI Tri-State
};

void Core_Init_Regs(void)
{
	BYTE index = 0x00;
    while(index < (sizeof(Core_reg_init_table)/sizeof(sREG_INIT)))
    {
        Tmp_XPntr = Core_reg_init_table[index].address;
		*Tmp_XPntr = Core_reg_init_table[index].initdata;
        index ++;
    }

    CLEAR_MASK(FLHCTRL2R,BIT3);
}
