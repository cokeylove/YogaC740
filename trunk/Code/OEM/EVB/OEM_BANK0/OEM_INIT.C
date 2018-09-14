/*-----------------------------------------------------------------------------
 * TITLE: OEM_INIT.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//----------------------------------------------------------------------------
// Oem_StartUp
//----------------------------------------------------------------------------
void Oem_StartUp(void)
{

}

//----------------------------------------------------------------------------
// The function of init. registers
//----------------------------------------------------------------------------
//const struct REG_INIT_DEF code reg_init_table[] =
const sREG_INIT_DEF code reg_init_table[] =
{
// ADC
  	{ &ADCSTS    	,AINITB             }, 	// Enable Analog accuracy initialization
    //{ &ADCSTS    	,0                    		}, 	// Stop adc accuracy initialization
    { &ADCSTS    	,0x80          	   	},  // Stop adc accuracy initialization
	{ &ADCCFG    	,0x21         	   	},  //
    { &KDCTL     	,AHCE               },  // Enable Hardware Callibration
	//{ &ADCCFG    	,DFILEN  	   		},	// Enable Digital Filter enable,
	{ &ADCCTL		,0x15	   			},

// DAC
    { &DACPWRDN  	,0x00               }, 	//

//PWM
	{ &ZTIER		,0x00				},	// Disable
	{ &C0CPRS		,0x23				},	// Channel 0 Clock Prescaler Register 
	{ &CTR			,0xFF				},	// Cycle Time 0
	//{ &DCR0		,0xFF				},	// Channel 0 PWM Duty Cycle
	{ &DCR1			,0xFF				},	// Channel 1 PWM Duty Cycle
	{ &DCR2			,0xFF				},	// Channel 2 PWM Duty Cycle
	{ &DCR3			,0x00				},	// Channel 3 PWM Duty Cycle
	{ &DCR4			,0x00				},	// Channel 4 PWM Duty Cycle
	{ &DCR5			,0x00				},	// Channel 5 PWM Duty Cycle
	//{ &DCR6		,0x00				},	// Channel 6 PWM Duty Cycle
	{ &DCR7			,0x00				},	// Channel 7 PWM Duty Cycle
	{ &PWMPOL		,0x00				},	// PWM Polarity
	{ &PCFSR		,0x05				},	// bit7=1: Disable CR256 channel 1 output. bit0-bit3: 0 => select 32.768 kHz, 1=> select EC clock frequency
	{ &PCSSGL		,0x1F				},	// Select channel 0-3 GroupG36 //HEGANGS042:Modify the led behavior follow new spec
	{ &PCSSGH		,0xCA				},	// Select channel 4-7 Group
	{ &CR256PCSSG	,0x80				},	// CR256 Prescaler Clock Source Select Group
	{ &PCSGR		,0x00				},	// Prescaler Clock Source Gating Register
	{ &C4CPRS		,0xE3				},	// Channel 4 Clock Prescaler Register
	{ &C4MCPRS		,0x00				},	// Channel 4 Clock Prescaler MSB Register
	{ &C6CPRS		,0x01				},	// 23KHz Channel 6 Clock Prescaler Register for fan
	{ &C6MCPRS		,0x00				},	// 23KHz Channel 6 Clock Prescaler MSB Register
	{ &C7CPRS		,0x00				},	// Channel 7 Clock Prescaler Register G36  //MEILING030:change 0xE0 to 0x00.
	{ &C7MCPRS		,0x00				},	// Channel 7 Clock Prescaler MSB Register
	{ &CLK6MSEL		,0x10				},	// PWM Clock 6MHz Select Register G36
	{ &CTR1			,0xFF				},	// Cycle Time 1
	{ &CTR2			,0xC8				},	// 23KHz Cycle Time 2(Fan used)
	{ &CTR3			,0xFF				},	// Cycle Time 3(Panel used)
	{ &PWMODENR		,0x06				},	// PWM output open-drain  //cwy
	{ &TSWCTRL		,0x0A				},	// Enable TACH0A,TACH1A speed.
	{ &ZTIER		,0x02				},	// Enable
	//{ &GCR19        ,0x00				},	
	{ &GCR15 		,0xC0				},  // Tony: Fix the issue of black screen when plug out 80port.
	{ &PWM0LCR1 	,0x35				},  // MEILING030:add.
	{ &PWM0LCR2 	,0x50				},  // MEILING030:add.
	{ &PWM1LCR1 	,0x35				},  //HEGANGS042:Modify the led behavior follow new spec
	{ &PWM1LCR2 	,0x50				},  
};

void Init_Regs(void)
{
	BYTE index = 0x00;
    while(index < (sizeof(reg_init_table)/sizeof(sREG_INIT_DEF)))
    {
        Tmp_XPntr = reg_init_table[index].address;
      	*Tmp_XPntr = reg_init_table[index].initdata;
        index ++;
    }
	//RamDebug(0xAA);
}


//for initial PWM setting
const sREG_INIT_DEF code Init_SMBus_table[] =
{
// SMBus
  { &SMB4P7USL		,0x28		},	// 100K
  { &SMB4P0USH   	,0x25      	},
  { &SMB300NS     	,0x03      	},
  { &SMB250NS     	,0x05      	},//:modify SMBUS date step from 0x02 to 0x03// G62:Modify SMBUS date step from 0x03 to 0x05.
  { &SMB25MS       	,0x19      	},
  { &SMB45P3USL   	,0xA5      	},
  { &SMBS4P3USH   	,0x01      	},
  { &SMB4P7A4P0H	,0x00      	},

  { &HOCTL2_A 		,0x01      	},
  { &HOCTL_A   		,0x03      	},
  { &HOCTL_A  		,0x01      	},
  { &HOSTA_A    	,0xFF      	},

  { &HOCTL2_B   	,0x01      	},
  { &HOCTL_B    	,0x03      	},
  { &HOCTL_B    	,0x01      	},
  { &HOSTA_B    	,0xFF      	},

  { &HOCTL2_C		,0x01		},
  { &HOCTL_C		,0x03		},
  { &HOCTL_C		,0x01		},
  { &HOSTA_C		,0xFF		},
};
//ITE Smart update for initial SMBUS
void Init_SMBus_Regs(void)
{
    BYTE index = 0x00;
    while(index < (sizeof(Init_SMBus_table)/sizeof(sREG_INIT_DEF)))
    {
        Tmp_XPntr = Init_SMBus_table[index].address;
		*Tmp_XPntr = Init_SMBus_table[index].initdata;
        index ++;
    }
}


//----------------------------------------------------------------------------
// Oem_Initialization - Initialize the registers, data variables, and oem
// functions after kernel init. function.
//----------------------------------------------------------------------------
void Oem_Initialization(void)
{

	ECBIOSVersionH = REV0_BYTE0;
	ECBIOSVersionL = REV0_BYTE1;
	ECBIOSUbSVersion = REV0_BYTE3;
    
    TP_Type =0xFF; //T071A+
    // GPCRM4=0X02;
	//CheckKSO1617Support();//72JERRY004: Remove KSO16 and KSO17 init.
	Init_Regs();
	Init_SMBus_Regs();	// initial SMBUS
	//8586 BX test	ChangePLLFrequency(0x07);
	InitSysMemory2ECRam();		// H2RAM function
	Init_ADC();
	Init_PECI();//:Change PECI initialize after S0 states.
	InitChargerIC();//ANGELAS016:Change charge IC option setting.
	//SetPowerBatteryparameter();
	Init_VC(3);	// Init All VC Channel.	// CMW 20121022
	//LID_readytime = 11; //ANGELAS007:Optimize power on sequence.

	eEEPROMData = Read_Eflash_Byte(EEPROMA2,(EEPROMA1_B03 | 0x07) ,0xE4);	// read USB charger status of EEPROM 0x0001B7E4
#if Support_AOU5_V1_2
		if (IS_MASK_SET(eEEPROMData,b1SetUSBChgEn))
	{
		SET_MASK(EMStatusBit,b1SetUSBChgEn);	// Enable Charger in S4/S5.
		CLR_MASK(LENOVOPMFW_Temp,b5EC_ON_EN);	// EC not turn off USB.
	}
	else
	{ CLEAR_MASK(EMStatusBit,b1SetUSBChgEn); }	// Disable Charger in S4/S5.
	#endif   ////72JERRY008:
#if Support_USB_Charge  //T19+
    #if UCS1022_Support
	//UCS1002ID = ReadUCS1022ID();
	UCS1002ID = SMSC_UCS1002ID;
	//SET_MASK(EMStatusBit, b1SetUSBChgEn);
	if (IS_MASK_SET(eEEPROMData,b1SetUSBChgEn))
	{
		SET_MASK(EMStatusBit,b1SetUSBChgEn);	// Enable Charger in S4/S5.
		/*//ANGELAS007:Optimize power on sequence.
		//ANGELAS005:s+ Support UMA SKU EC_ON pin design.
		if(NTC_V1>682)
		{
			GPCRF0 = OUTPUT;
			SET_MASK(GPDRF,BIT(0));
		}
		else
		//ANGELAS005:e+ Support UMA SKU EC_ON pin design.
		*///ANGELAS007:Optimize power on sequence.
		//EC_ON_HI(); //ANGELAS038:remove 
		//EC_ON2_HI();
		CLR_MASK(LENOVOPMFW_Temp,b5EC_ON_EN);	// EC not turn off USB.
	}
	else
	{ CLEAR_MASK(EMStatusBit,b1SetUSBChgEn); }	// Disable Charger in S4/S5.
    #else
	if (IS_MASK_SET(eEEPROMData,b1SetUSBChgEn))	// Check S4/S5 feature.
	{
		SET_MASK(EMStatusBit,b1SetUSBChgEn);	// Enable Charger in S4/S5.
		/*//ANGELAS007:Optimize power on sequence.
		//ANGELAS005:s+ Support UMA SKU EC_ON pin design.
		if(NTC_V1>682)
		{
			GPCRF0 = OUTPUT;
			SET_MASK(GPDRF,BIT(0));
		}
		else
		//ANGELAS005:e+ Support UMA SKU EC_ON pin design.
		*///ANGELAS007:Optimize power on sequence.
		//EC_ON_HI(); //ANGELAS038:remove 
		//EC_ON2_HI();
		CLR_MASK(LENOVOPMFW_Temp,b5EC_ON_EN);	// EC not turn off USB.
	}
	else
	{ CLEAR_MASK(EMStatusBit,b1SetUSBChgEn); }	// Disable Charger in S4/S5.
    #endif	// UCS1022_Support
#endif                           // Support_USB_Charge      
	CheckECCHIPVer();
	NOVO_COUNTER = T_PSWOFF;//Hang check
}

void CheckECCHIPVer(void)
{
	if( (ECHIPID1==0x85) && (ECHIPID2==0x86) )
	{
		if( ECHIPVER == 0x05 )
		{
			cCHIPType = 0x01;
		}
	}
}

//----------------------------------------------------------------------------
// The function of clearing external ram for OEM code
// Clear external ram (0x100~0xFFF)
//----------------------------------------------------------------------------
void Init_ClearRam(void)
{
    PORT_BYTE_PNTR byte_register_pntr;

    byte_register_pntr=0x100;
    while(byte_register_pntr<0x1000)// Clear external ram (0x100~0xFFF)
    {
        *byte_register_pntr=0;
          byte_register_pntr ++;
    }

    byte_register_pntr=0x2200;
    //while(byte_register_pntr<0x2280)// Clear Batt ram BANK0 (0x2200~0x227F)  //MEILING009:remove.
    while(byte_register_pntr<0x2270) //MEILING009:add.
    {
        *byte_register_pntr=0;
          byte_register_pntr ++;
    }
}

void Clear_RAM_Range(WORD nAddr, WORD nSize)
{
    Tmp_XPntr = (unsigned int*)nAddr;
    for( ITempW01=0; ITempW01<=nSize; ITempW01++ )
    {
        *Tmp_XPntr = 0;
        Tmp_XPntr++;
    }
}

void RAM_travel(WORD sour, WORD targ, BYTE count)
{
    Tmp_XPntr = (unsigned int*)sour;
    Tmp_XPntr1 = (unsigned int*)targ;
    while(count) 
    {
    	*Tmp_XPntr1++ = *Tmp_XPntr++;
    	count--;
    };
}


//----------------------------------------------------------------------------
// The function of DMA for scratch sram
//----------------------------------------------------------------------------
const sDMAScratchSRAM code asDMAScratchSRAM[]=
{
	{	&SCRA1H,	&SCRA1M,	&SCRA1L	},		// 1024 bytes (externl ram 0x800 ~ 0xBFF)
	{	&SCRA2H,	&SCRA2M,	&SCRA2L	},		// 512 bytes  (externl ram 0xC00 ~ 0xDFF)
	{	&SCRA3H,	&SCRA3M,	&SCRA3L	},		// 256 bytes  (externl ram 0xE00 ~ 0xEFF)
	{	&SCRA4H,	&SCRA4M,	&SCRA4L	}		// 256 bytes  (externl ram 0xF00 ~ 0xFFF)

};
void  CacheDma(BYTE sramnum,WORD addr)
{
	*asDMAScratchSRAM[sramnum].scarh = 0x80;
	*asDMAScratchSRAM[sramnum].scarm = (WORD) addr >> 8;   	// high byte of function address
	*asDMAScratchSRAM[sramnum].scarl = (WORD) addr & 0xff;	// low byte of function address
	*asDMAScratchSRAM[sramnum].scarh = 0x00;	// start cache dma
}



//----------------------------------------------------------------------------
// Hook_ECExitFollowMode
//----------------------------------------------------------------------------
void Hook_ECExitFollowMode(void)
{
	CLEAR_MASK(nStopChgStat3H, ECFLASH);
}

//----------------------------------------------------------------------------
// EC leaves follow mode or EC scatch ROM and jump to main function
//  Note:
//      1. Before jump to main function if want to keep EC setting [return(0x33);] is necessary.
//      2. If you don't understand the use of Hook_ECRetunrMainFuncKeepCondition function,
//          don't change anything.
//      3. Always at bank0 (0x9000)
//----------------------------------------------------------------------------
BYTE Hook_ECRetunrMainFuncKeepCondition(void)
{
    if(WinFlashMark==0x33 && WinFlashMark2==0x88)
    {
        return(0x33);
    }
    else
    {
        return(0x00);
    }
}

void Check_Mirror_Occurs(void)
{
	if(IS_MASK_SET(FLHCTRL3R,BIT7))
	{
		SET_MASK(FLHCTRL3R,BIT7);
		SET_MASK(FLHCTRL3R,BIT0);
		SET_MASK(HINSTC1,BIT6);
	}
}

const sCharger chargerInitTable[] =
{
	{C_ChargerMode,  	       &CHGIC_WriteCmd0x12L     },//JIMAPLO009:Update charge IC from 24780 to 24780s. 
	{C_ChargeVoltage,	       &nChargingVoltL       	},
	{C_ChargeCurrent,          &nBattCharCurrentL       },     
	{C_ChargerMode1,           &CHGIC_ReadCmd0x3BL  	},	
	{C_InputCurrent, 	       &CHGIC_InputCurrentL		},
	{C_ChargerProchot, 	       &CHGIC_ReadCmd0x3CL		},
	{C_ChargerProchot1,        &CHGIC_ReadCmd0x3DL		},
	{C_ChargerDischargeCurrent, &CHGIC_ReadCmd0x39L		},	
	{C_ChargerMode2,           &CHGIC_WriteCmd0x38L  	},	 
	{C_ChargerMode3,           &CHGIC_WriteCmd0x37L  	}, 
};
void InitChargerIC(void)
{
	Chk_Hybrid_STPP_min_BattGasgauge = 10;
	chargerInitIndex=0;
	CHGIC_WriteCmd0x12L=0x18;
	
	if((SystemNotS0)&&(IS_MASK_CLEAR(SYS_STATUS,AC_ADP)))//DC only not s0
		CHGIC_WriteCmd0x12H=0xE1;
	else
		CHGIC_WriteCmd0x12H=0x61; 

	CHGIC_ReadCmd0x3BL=0x10; 
	CHGIC_ReadCmd0x3BH=0x46;

	CHGIC_WriteCmd0x38L=0x84;
	CHGIC_WriteCmd0x38H=0x03;

	CHGIC_WriteCmd0x37L=0xC4;//Modify power setting follow table 0822
	CHGIC_WriteCmd0x37H=0x86;

	CHGIC_ReadCmd0x3CL=0x56;
	CHGIC_ReadCmd0x3CH=0x4C; // HEGANGS008£ºMofify power setting

	if(IS_MASK_CLEAR(SYS_STATUS,AC_ADP)) 
		CHGIC_ReadCmd0x3DL=0x3C; //DC only : ACOK Disable
	else
		CHGIC_ReadCmd0x3DL=0x0D; //AC ONLY or AC+DC : ACOK Enable
	if(IS_MASK_CLEAR(SYS_STATUS,AC_ADP)&&(CellCount==3))
		CHGIC_ReadCmd0x3DH=0x31;	
	else
		CHGIC_ReadCmd0x3DH=0x71; 
	
	nChargingVoltL=0x60;//8.8V
	nChargingVoltH=0x22; 
	
	nBattCharCurrentL = 0x00;// 256mA  
	nBattCharCurrentH = 0x01;            

	if(IS_MASK_CLEAR(SEL_STATE0,PRESENT_A))//AC only // HEGANGS032:Modify power setting
	{
		CHGIC_InputCurrentL=0x00;//
		CHGIC_InputCurrentH=0x0B;
	}
	else  //AC+DC
	{
		if(nBattGasgauge >= 10)
		{
			CHGIC_InputCurrentL=0x80;//
			CHGIC_InputCurrentH=0x0B;
		}
		else
		{
			CHGIC_InputCurrentL=0x00;//
			CHGIC_InputCurrentH=0x0B;
		}
	}
	//CHGIC_InputCurrentL=0x00;//Input current register 1.85A
	//CHGIC_InputCurrentH=0x08;

	CHGIC_ReadCmd0x39L=0x00; //12A
	CHGIC_ReadCmd0x39H=0x28; //

	while(chargerInitIndex < (sizeof(chargerInitTable)/sizeof(sCharger)))
	{
		bRWSMBus(SMbusCh2, SMbusWW, Charger_Addr, chargerInitTable[chargerInitIndex].Cmd, 
		chargerInitTable[chargerInitIndex].Var,SMBus_NoPEC);
		chargerInitIndex++;
	}
  
}



