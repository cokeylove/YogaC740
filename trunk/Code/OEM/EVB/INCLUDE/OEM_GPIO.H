/*-----------------------------------------------------------------------------
 * TITLE: OEM_GPIO.H
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef OEM_GPIO_H
#define OEM_GPIO_H

//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
#define HighActive  	1
#define LowActive     	0

//-----------------------------------------------------------------
// GPIO Registers define
//-----------------------------------------------------------------
/*  General Control Register (GCR)  */
// BIT 7     -  GPIOB5 HIGH Follow LPCRST# HIGH-to-LOW
// BIT 6     -  WUI7 Enabled
// BIT 5     -  WUI6 Enabled
// BIT [2:1] -  [01] LPCRST# in GPIOB7
//                   [10] LPCRST# in GPIOD2
//                   [11] LPCRST# is dsiabled
#define GCR_Init		0x06    //
#define GCR2_Init		0x20	//Turn on SMBus4 function.
#define GPOTH_Init      0X20     
#define GCR10_Init      0x01 	//Modify EC reset time to 8s. 




//-----------------------------------------------------------------
// GPIO Port A Registers define
//-----------------------------------------------------------------
#define PortA_Init		0x07            // PORT Data 

#define PortA0_Ctrl		OUTPUT			//O   24      PWR_LED#
#define PortA1_Ctrl		OUTPUT			//O   25      BATT_CHG_LED#
#define PortA2_Ctrl		OUTPUT			//O   28      BATT_LOW_LED#
#define PortA3_Ctrl		ALT				//O   29      EC_BKL_EN(backlight of keyboard)
#define PortA4_Ctrl		OUTPUT          //O   30      EC_FAN_PWM1         	
#define PortA5_Ctrl		OUTPUT       	//O   31      EC_FAN_PWM
#define PortA6_Ctrl		INPUT+PULL_DW	//O   32      BEEP#
#define PortA7_Ctrl		OUTPUT	        //O   34   	  EC_VCCST_EN

#define PWR_WHITELED_ON()				CLEAR_MASK(GPDRA,BIT(0))
#define PWR_WHITELED_OFF()				SET_MASK(GPDRA,BIT(0)) 
#define PWR_WHITELED_ALT                GPCRA0 = ALT  
#define PWR_WHITELED_OUTPUT		  		GPCRA0 = OUTPUT	
#define PWR_WHITELED_INPUT		    	GPCRA0= INPUT   

#define BAT_LOW_LED_ON()			    CLEAR_MASK(GPDRA,BIT(2))
#define BAT_LOW_LED_OFF()			    SET_MASK(GPDRA,BIT(2))

#define PWR_AMBERLED_ON()				CLEAR_MASK(GPDRA,BIT(1))
#define PWR_AMBERLED_OFF()				SET_MASK(GPDRA,BIT(1)) 
#define PWR_AMBERLED_ALT                GPCRA1 = ALT  
#define PWR_AMBERLED_OUTPUT		  		GPCRA1 = OUTPUT	
#define PWR_AMBERLED_INPUT		    	GPCRA1= INPUT   


#define FAN1_PWM					    DCR4
#define FAN1_PWM_INDW				    GPCRA4 = INPUT+PULL_DW	// Set FAN1_PWM input+pull down.
#define FAN1_PWM_ALT				    GPCRA4 = ALT			// Set FAN1_PWM Alt.
#define FAN1_PWM_OUT				    GPCRA4 = OUTPUT			// Set FAN1_PWM OUTPUT.
#define EC_FAN1_PWM_HI()			    SET_MASK(GPDRA,BIT(4))

#define FAN_PWM						    DCR5
#define FAN_PWM_INDW				    GPCRA5 = INPUT+PULL_DW	// Set FAN_PWM input+pull down.
#define FAN_PWM_ALT					    GPCRA5 = ALT			// Set FAN_PWM Alt.
#define FAN_PWM_OUT					    GPCRA5 = OUTPUT			// Set FAN_PWM OUTPUT.
#define EC_FAN_PWM_HI()				    SET_MASK(GPDRA,BIT(5))

#define PWM_BEEP					    DCR6
#define BEEP_INIT					    GPCRA6 = PortA6_Ctrl	// init PWM Beep.
#define BEEP_ALT					    GPCRA6 = ALT			// Set PWM BEEP ALT.

#define EC_VCCST_EN_HI()			    SET_MASK(GPDRA,BIT(7))
#define EC_VCCST_EN_LOW()			    CLEAR_MASK(GPDRA,BIT(7))

//-----------------------------------------------------------------
// GPIO Port B Registers define
//-----------------------------------------------------------------
#define PortB_Init		0x00            // PORT Data 

#define PortB0_Ctrl		INPUT			//O    	108 	   ACIN(G_Skylake)
#define PortB1_Ctrl		INPUT			//O    	109 	   LID_SW#
#define PortB2_Ctrl		OUTPUT         	//O    	123 	   EC_VCCIO_EN(G_Skylake)
#define PortB3_Ctrl		INPUT			//A    	110 	   PWRSW#
#define PortB4_Ctrl		INPUT			//A    	111 	   NO_USE(reseved)(720S)
#define PortB5_Ctrl		OUTPUT			//O    	126    EC_RTCRST#_ON(reseved)(720S)
#define PortB6_Ctrl		OUTPUT			//O    	4      KBRST#
#define PortB7_Ctrl		INPUT			//O    	112 	  3VLP

#define Read_AC_IN()		IS_MASK_SET(GPDRB,BIT(0))
#define AC_IN_ALT			GPCRB0 = ALT	// Set AC_IN Alternate.
#define AC_IN_INPUT			GPCRB0 = INPUT	// Set AC_IN INPUT.

#define Read_LID_SW_IN()	IS_MASK_SET(GPDRB,BIT(1))

#define EC_VCCIO_EN_HI()	SET_MASK(GPDRB,BIT(2))
#define EC_VCCIO_EN_LOW()	CLEAR_MASK(GPDRB,BIT(2))

#define Read_EC_PWRBTN() 	IS_MASK_CLEAR(GPDRB,BIT(3))

#define RTCRST_ON_HI()		SET_MASK(GPDRB,BIT(5))
#define RTCRST_ON_LOW()		CLEAR_MASK(GPDRB,BIT(5))

#define KBRST_ON()			CLEAR_MASK(GPDRB,BIT(6))
#define KBRST_OFF()			SET_MASK(GPDRB,BIT(6))
#define RCING_LOW()			CLEAR_MASK(GPDRB,BIT(6))
#define RCING_HI()			SET_MASK(GPDRB,BIT(6))

//-----------------------------------------------------------------
// GPIO Port C Registers define
//-----------------------------------------------------------------
#define PortC_Init		0x00            // PORT Data  

#define PortC0_Ctrl		OUTPUT	        //O    	119	 	EC_RSMRST#(720S)
#define PortC1_Ctrl		ALT			    //IO   	115		EC_SMB_CK1
#define PortC2_Ctrl		ALT			    //IO   	116		EC_SMB_CK1
#define PortC3_Ctrl		INPUT           //I	      56		EC_ID
#define PortC4_Ctrl		OUTPUT	        //I	      120	EC_ON_5V(720S)
#define PortC5_Ctrl		INPUT           //O	      57		530 high/ yoga low
#define PortC6_Ctrl		OUTPUT		    //O       124		SUSP#
#define PortC7_Ctrl		OUTPUT		    //O       16		EC_RX

#define RSMRST_LOW()		CLEAR_MASK(GPDRC,BIT(0))
#define RSMRST_HI()			SET_MASK(GPDRC,BIT(0))
#define Read_RSMRST()		IS_MASK_SET(GPDRC,BIT(0))

//LMLKBL0015:add start.
#define SMBUS_CK1_ALT       GPCRC1 = ALT;
#define SMBUS_DA1_ALT       GPCRC2 = ALT;
#define SMBUS_CK1_IN        GPCRC1 = INPUT;
#define SMBUS_DA1_IN        GPCRC2 = INPUT;
#define SMBUS_CK1_HIGH      SET_MASK(GPDRC,BIT(1))
#define SMBUS_DA1_HIGH      SET_MASK(GPDRC,BIT(2))

#define Read_EC_ID()  		IS_MASK_SET(GPDRC,BIT(3))

#define EC_ON_5V_HI()		SET_MASK(GPDRC,BIT(4))
#define EC_ON_5V_LOW()		CLEAR_MASK(GPDRC,BIT(4))

#define Read_YOGA_ID()      IS_MASK_SET(GPDRC,BIT(5)) //72JERRY012

#define SUSP_OFF()			CLEAR_MASK(GPDRC,BIT(6))
#define SUSP_ON() 			SET_MASK(GPDRC,BIT(6))
#define Read_SUSP()			IS_MASK_SET(GPDRC,BIT(6))

#define EC_RX_ALT			GPCRC7 = ALT	// Set EC_RX Alternate.
#define EC_RX_OUTPUT		GPCRC7 = OUTPUT	// Set EC_RX OUTPUT.
#define EC_RX_INPUT			GPCRC7 = INPUT	// Set EC_RX INPUT.

//no function
#define O_DEBUG_CLK_HI()    SET_MASK(GPDRC,BIT(7))
#define O_DEBUG_CLK_LO()    CLEAR_MASK(GPDRC,BIT(7))


//-----------------------------------------------------------------
// GPIO Port D Registers define
//-----------------------------------------------------------------
#define PortD_Init		0x00            // PORT Data

#define PortD0_Ctrl		INPUT			//I     18       PM_SLP_S3#
#define PortD1_Ctrl		INPUT			//I     21       PM_SLP_S4#
#define PortD2_Ctrl		INPUT			//I     22 	PLT_RST#
#define PortD3_Ctrl		OUTPUT			//O    23 	EC_SCI#
#define PortD4_Ctrl		OUTPUT			//O    15 	EC_SMI#  //hang check
#define PortD5_Ctrl		INPUT		    //O    33	 	USB_ON#
#define PortD6_Ctrl		INPUT+PULL_DW	//A    47 	EC_FAN_SPEED
#define PortD7_Ctrl		INPUT+PULL_DW   //O    48 	EC_FAN1_SPEED(720S)   	

#define Read_SLPS3()		IS_MASK_SET(GPDRD,BIT(0))

#define Read_SLPS4()		IS_MASK_SET(GPDRD,BIT(1))

#define Read_LPC_RST()		IS_MASK_SET(GPDRD,BIT(2))

#define SCI_ON()			CLEAR_MASK(GPDRD,BIT(3)) // pull high after sus_p
#define SCI_OFF()			SET_MASK(GPDRD,BIT(3))
#define SCI_LOW()			CLEAR_MASK(GPDRD,BIT(3))
#define SCI_HI()			SET_MASK(GPDRD,BIT(3))

#define SMI_ON()		CLEAR_MASK(GPDRD,BIT(4)) // hang check
#define SMI_OFF()		SET_MASK(GPDRD,BIT(4))
#define SMI_LOW()		CLEAR_MASK(GPDRD,BIT(4))
#define SMI_HI()		SET_MASK(GPDRD,BIT(4))

#define USB_ON_INPUT		GPCRD5 = INPUT			// SET USB_ON# INPUT.
#define USB_ON_OUTPUT		GPCRD5 = OUTPUT			// SET USB_ON# OUTPUT.
#define USB_ON_LOW()		CLR_MASK(GPDRD,BIT(5))  // Turn On USB_ON#.

#define FAN_SPEED_ALT		GPCRD6 = ALT			// Set FAN Speed Alt.
#define FAN_SPEED_INDW		GPCRD6 = INPUT+PULL_DW	// Set FAN Speed input+pull down.

#define FAN1_SPEED_ALT		GPCRD7 = ALT			// Set FAN Speed Alt.
#define FAN1_SPEED_INDW		GPCRD7 = INPUT+PULL_DW	// Set FAN Speed input+pull down.

//#define EC_TS_ON_HI()       SET_MASK(GPDRD,BIT(7)) // (first mask)
//#define EC_TS_ON_LOW()      CLEAR_MASK(GPDRD,BIT(7))
//#define EC_TS_ON_INPUT	    GPCRD7 = INPUT	   
//#define EC_TS_ON_OUTPUT		GPCRD7 = OUTPUT	 

//-----------------------------------------------------------------
// GPIO Port E Registers define
//-----------------------------------------------------------------
#define PortE_Init		0x00                    //

#define PortE0_Ctrl		OUTPUT			//O     19       RESEVE
#define PortE1_Ctrl		INPUT			//I      82      AOU_DET
#define PortE2_Ctrl		INPUT           //I      83      VDDQ_PGOOD
#define PortE3_Ctrl		OUTPUT			//O     84   	    EC_VPP_PWREN
#define PortE4_Ctrl		OUTPUT			//I      125     VR_ON
#define PortE5_Ctrl		OUTPUT			//O 	    35       USB_CHG_EN 
#define PortE6_Ctrl		INPUT           //O	    17      EC_TX             //set to Input for judge whether debug card input or not   
#define PortE7_Ctrl		OUTPUT			//O	    20      RESEVE

#define Read_AOU_DET()      IS_MASK_SET(GPDRE,BIT(1))

#define Read_VDDQ_PGOOD()		IS_MASK_SET(GPDRE,BIT(2))

#define EC_VPP_PWREN_HI()       SET_MASK(GPDRE,BIT(3))
#define EC_VPP_PWREN_LOW()      CLR_MASK(GPDRE,BIT(3))

#define VR_ON_OFF() 			CLEAR_MASK(GPDRE,BIT(4))
#define VR_ON_ON()  			SET_MASK(GPDRE,BIT(4))
#define Read_VR_ON()			IS_MASK_SET(GPDRE,BIT(4))

//#define DPWROK_EC_LOW()			CLEAR_MASK(GPDRE,BIT(5))
//#define DPWROK_EC_HI()			SET_MASK(GPDRE,BIT(5))
#define  USB_CHG_EN_HI()				SET_MASK(GPDRE,BIT(5))
#define  USB_CHG_EN_LOW()				CLEAR_MASK(GPDRE,BIT(5))

#define EC_TX_ALT			GPCRE6 = ALT	// Set EC_TX Alternate.
#define EC_TX_OUTPUT		GPCRE6 = OUTPUT	// Set EC_TX OUTPUT.
#define EC_TX_INPUT			GPCRE6 = INPUT	// Set EC_RX INPUT.

//no function
#define O_DEBUG_DAT_HI()    SET_MASK(GPDRE,BIT(6))
#define O_DEBUG_DAT_LO()    CLEAR_MASK(GPDRE,BIT(6))
#define Read_EC_TX()		IS_MASK_SET(GPDRE,BIT(6))  


#define BAT_CHG_LED_ON()			CLEAR_MASK(GPDRE,BIT(7))//HEGANGS042:Modify the led behavior follow new spec
#define BAT_CHG_LED_OFF()			SET_MASK(GPDRE,BIT(7))

//-----------------------------------------------------------------
// GPIO Port F Registers define
//-----------------------------------------------------------------
#define PortF_Init		0x00           //72JERRY060:CCG4 INT

#define PortF0_Ctrl		OUTPUT			//O     85	EC_ON //ANGELAS020: INPUT to OUTPUT
#define PortF1_Ctrl		OUTPUT		    //O     86  	PBTN_OUT#
#define PortF2_Ctrl		ALT			    //I      87  	EC_SMB_CK3(BG_Skylake)  //MEILING001:change INPUT to ALT
#define PortF3_Ctrl		ALT             //IO	    88  	EC_SMB_DA3(BG_Skylake)  //MEILING001:change INPUT to ALT
#define PortF4_Ctrl		INPUT   		//IO    89  	TAB_SW#
#define PortF5_Ctrl		OUTPUT	        //IO    90  	CHG_MOD1(720s)
#define PortF6_Ctrl		INPUT	        //A     117 	PECI_EC
#define PortF7_Ctrl		OUTPUT          //O	   118  	CHG_MOD3

//ANGELAS038:add start
#define EC_ON_HI()  		SET_MASK(GPDRF,BIT(0))
#define EC_ON_LOW() 		CLEAR_MASK(GPDRF,BIT(0))
//ANGELAS038:add end

#define PM_PWRBTN_LOW()		CLEAR_MASK(GPDRF,BIT(1))
#define PM_PWRBTN_HI()		SET_MASK(GPDRF,BIT(1))
#define Read_PM_PWRBTN()	IS_MASK_SET(GPDRF,BIT(1))

#define SMBUS_CK3_ALT       GPCRF2 = ALT;//72JERRY052:Modify the SMBUS leakage in DC S5.
#define SMBUS_DA3_ALT       GPCRF3 = ALT;//72JERRY052:Modify the SMBUS leakage in DC S5.
#define SMBUS_CK3_IN        GPCRF2 = INPUT;//72JERRY052:Modify the SMBUS leakage in DC S5.
#define SMBUS_DA3_IN        GPCRF3 = INPUT;//72JERRY052:Modify the SMBUS leakage in DC S5.
#define SMBUS_CK3_HIGH      SET_MASK(GPDRF,BIT(2))//72JERRY052:Modify the SMBUS leakage in DC S5.
#define SMBUS_DA3_HIGH      SET_MASK(GPDRF,BIT(3))//72JERRY052:Modify the SMBUS leakage in DC S5.

#define Read_LID_PAD()      IS_MASK_SET(GPDRF,BIT(4)) 

#define CHG_MOD1_HI()		SET_MASK(GPDRF,BIT(5))
#define CHG_MOD1_LOW()		CLEAR_MASK(GPDRF,BIT(5))

#define PECI_INDW			GPCRF6 = INPUT			// Set PECI INPUT.
#define PECI_ALT			GPCRF6 = ALT+PULL_DW			// Set PECI Alternate.//72JERRY012: PECI for MS SLPS0
                         
#define CHG_MOD3_HI()		SET_MASK(GPDRF,BIT(7))
#define CHG_MOD3_LOW()		CLEAR_MASK(GPDRF,BIT(7))
//-----------------------------------------------------------------
// GPIO Port G Registers define
//-----------------------------------------------------------------
#define PortG_Init		0x00            // PORT Data     //ANGELAS035: 01 to 00

#define PortG0_Ctrl		OUTPUT		   //O     106 	BKOFF
#define PortG1_Ctrl		OUTPUT         //O     107 	SYSON
#define PortG2_Ctrl		INPUT		   //A     100 	GPG2 for mirror
#define PortG3_Ctrl		INPUT          //A     101	EC_SPI_CS1# // change OUTPUT to INPUT
#define PortG4_Ctrl		INPUT          //A     102 	EC_SPI_SI     // change OUTPUT to INPUT        
#define PortG5_Ctrl		INPUT          //A     103      EC_SPI_SO                       		
#define PortG6_Ctrl     INPUT+PULL_DW  //O    104	      ME_FLASH
#define PortG7_Ctrl		INPUT          //A    105       EC_SPI_CLK   

#define BKOFF_OFF()			CLEAR_MASK(GPDRG,BIT(0))
#define BKOFF_ON()			SET_MASK(GPDRG,BIT(0))
#define Read_BKOFF()		GET_MASK(GPDRG, BIT(0))

#define SYSON_LOW()			CLEAR_MASK(GPDRG,BIT(1))
#define SYSON_HI() 			SET_MASK(GPDRG,BIT(1))
#define Read_SYSON()		IS_MASK_SET(GPDRG,BIT(1))

#define pchME_FLAS_HI()		SET_MASK(GPDRG,BIT(6))		// ME Lock
#define pchME_FLAS_LOW()	CLEAR_MASK(GPDRG,BIT(6))	// ME unlock
#define pchME_FLAS_INDW		GPCRG6 = INPUT+PULL_DW		// SET ME Input+Pull down.
#define pchME_FLAS_OUT		GPCRG6 = OUTPUT				// SET ME Output.

//-----------------------------------------------------------------
// GPIO Port H Registers define
//-----------------------------------------------------------------
#define PortH_Init		0x09             //TYPE_C_M0 HIGH    CAPS_LED HIGH 

#define PortH0_Ctrl		INPUT		    //O   	93   TYPE_C_M0
#define PortH1_Ctrl		INPUT+PULL_DW	//IO  	94 	EC_SMB_CK2
#define PortH2_Ctrl		INPUT+PULL_DW	//IO  	95 	EC_SMB_DA2
#define PortH3_Ctrl		OUTPUT		    //A		96 	CAPS_LED#
#define PortH4_Ctrl		OUTPUT		    //O    	97 	PCH_PWR_EN(1.8V)
#define PortH5_Ctrl		OUTPUT		    //A    	98 	EC_BKL_EN
#define PortH6_Ctrl		OUTPUT			//A  	99	PCH_PWROK

#define TYPE_C_M0_HI()     SET_MASK(GPDRH,BIT(0))
#define TYPE_C_M0_LOW()    CLEAR_MASK(GPDRH,BIT(0))

#define SMBus2_CLK_INDW 	GPCRH1 = INPUT+PULL_DW	// Set SMBus2 CLK input+pull down.
#define SMBus2_CLK_ALT		GPCRH1 = ALT			// Set SMBus2 CLK Alternate.

#define SMBus2_DAT_INDW 	GPCRH2 = INPUT+PULL_DW	// Set SMBus2 DATA input+pull down.
#define SMBus2_DAT_ALT		GPCRH2 = ALT			// Set SMBus2 DATA Alternate.

#define CAPLED_ON()			CLEAR_MASK(GPDRH,BIT(3))
#define CAPLED_OFF()		SET_MASK(GPDRH,BIT(3))

#define EC_ON_1_8V_HI()    SET_MASK(GPDRH,BIT(4))
#define EC_ON_1_8V_LOW()   CLEAR_MASK(GPDRH,BIT(4))

#define EC_VCCST_PWRGD_HI() 		SET_MASK(GPDRH,BIT(5))
#define EC_VCCST_PWRGD_LOW()	 	CLEAR_MASK(GPDRH,BIT(5))

#define PCH_PWROK_LOW()		CLEAR_MASK(GPDRH,BIT(6))
#define PCH_PWROK_HI() 		SET_MASK(GPDRH,BIT(6))
#define Read_PCH_PWROK()	IS_MASK_SET(GPDRH,BIT(6))

//-----------------------------------------------------------------
// GPIO Port I Registers define
//-----------------------------------------------------------------
#define PortI_Init		0x08            // TYPE_C_M1 HIGH

#define PortI0_Ctrl		INPUT         //A    	66 	NTC_V1
#define PortI1_Ctrl		INPUT	      //A    	67 	NTC_V2
#define PortI2_Ctrl		INPUT		  //A    	68   ADD battery detect
#define PortI3_Ctrl		INPUT	      //O    	69 	TYPE_C_M1
#define PortI4_Ctrl		INPUT		  //I  	70 	CPU_VR_READY 0.8V
#define PortI5_Ctrl		INPUT	      //A   	71 	ADP_I
#define PortI6_Ctrl		INPUT		  //I  	72 	Psys(ADC Channel6)
#define PortI7_Ctrl		OUTPUT		  //I   	73 	SYS_PWROK(720S)

//#define EC_CCG4_ON_HI()		SET_MASK(GPDRI,BIT(0))
//#define EC_CCG4_ON_LOW()	CLEAR_MASK(GPDRI,BIT(0))

#define Read_BAT_IN()		IS_MASK_CLEAR(GPDRI,BIT(2))  //battery detect pin

//#define Read_EC_LID_OUT()   IS_MASK_SET(GPDRI,BIT(3))
//#define EC_LID_OUT_HI()     SET_MASK(GPDRI,BIT(3))
//#define EC_LID_OUT_LOW()    CLEAR_MASK(GPDRI,BIT(3))
#define TYPE_C_M1_HI()     SET_MASK(GPDRI,BIT(3))
#define TYPE_C_M1_LOW()    CLEAR_MASK(GPDRI,BIT(3))

#define SYS_PWROK_HI()		SET_MASK(GPDRI,BIT(7))
#define SYS_PWROK_LOW()		CLEAR_MASK(GPDRI,BIT(7))

//-----------------------------------------------------------------
// GPIO Port J Registers define
//-----------------------f------------------------------------------
#define PortJ_Init		0x80            // PORT Data  //HEGANG0001:Set VGA_AC_DET pull high

#define PortJ0_Ctrl		INPUT			//I    	76 	 NOVO#
#define PortJ1_Ctrl		OUTPUT			//O    	77 	 EC_MUTE#(720S)EC_SLP_S5#(reserved)
#define PortJ2_Ctrl		OUTPUT			//O    	78 	 SUSWARN#(reserved)(EC_TS_ON)//Add enable/disable touch panel pin.
#define PortJ3_Ctrl		OUTPUT			//O    	79 	 EC_STATUS#(reserved)(720S)//Add enable/disable TP function by HW pin.
#define PortJ4_Ctrl		OUTPUT       	//O    	80 	 H_PROCHOT#_EC
#define PortJ5_Ctrl		INPUT			//O    	81 	 ENBKL
#define PortJ6_Ctrl		OUTPUT			//O    	128   AC_PRESENT_R
#define PortJ7_Ctrl		OUTPUT			//O    	2 	 VGA_AC_DET(GPU_PROCHOT)

#define Read_EC_NOVO()		IS_MASK_CLEAR(GPDRJ,BIT(0))

#define EC_MUTE_ACT()		CLEAR_MASK(GPDRJ,BIT(1))
#define EC_MUTE_INACT()		SET_MASK(GPDRJ,BIT(1))

#define EC_TS_ON_HI()           SET_MASK(GPDRJ,BIT(2)) //Add enable/disable touch panel pin.
#define EC_TS_ON_LOW()          CLEAR_MASK(GPDRJ,BIT(2)) //Add enable/disable touch panel pin.
#define READ_EC_TS_ON()         IS_MASK_SET(GPDRJ,BIT(2))//Add enable/disable touch panel pin.

#define EC_TP_ON_HI()           SET_MASK(GPDRJ,BIT(3)) //Add enable/disable TP function by HW pin.
#define EC_TP_ON_LOW()          CLEAR_MASK(GPDRJ,BIT(3)) //Add enable/disable TP function by HW pin.
#define READ_EC_TP_ON()         IS_MASK_SET(GPDRJ,BIT(3))//Add enable/disable TP function by HW pin.

#define H_PROCHOT_ON()			SET_MASK(GPDRJ,BIT(4))  
#define H_PROCHOT_OFF()			CLEAR_MASK(GPDRJ,BIT(4))     

#define Read_ENBKL_IN()			IS_MASK_SET(GPDRJ,BIT(5))

#define AC_PRESENT_LOW() 		CLEAR_MASK(GPDRJ,BIT(6))
#define AC_PRESENT_HI()			SET_MASK(GPDRJ,BIT(6))

#define GPU_PROCHOT_ON()        CLR_MASK(GPDRJ,BIT(7))
#define GPU_PROCHOT_OFF()       SET_MASK(GPDRJ,BIT(7))


//-----------------------------------------------------------------------------
// DPGU
//-----------------------------------------------------------------------------
#define     EG_FB_CLAMP         0x10
#define     EG_GC6_EVENT        0x14
#define     EG_DGPU_PWROK       0x1C
#define     EG_DGPU_PWR_EN      0x24
#define     EG_DGPU_HOLD_RST    0x2C
#define     EG_DGPU_GC6_EN      0x34
#define     EG_NVDD_PWR_EN      0x48
#define     EG_KBL_DET          0x50

#define     EG_GPIO22           0x58
#define     EG_S_NVDD_PWR_EN    0x60
#define     EG_GPIO26           0x68
#define     EG_S_DGPU_RST       0x6C
#define     EG_S_DGPU_PWR_EN    0x74
#define     EG_S_DGPU_PWROK     0x7C
#define     EG_S_GC6_EVENT      0x84
#define     EG_S_GC6_EN         0x8C

#define DGPU_PWR_EN_HI()    EGPC_Write(EG_DGPU_PWR_EN,  0x03)
#define DGPU_PWR_EN_LOW()   EGPC_Write(EG_DGPU_PWR_EN,  0x02)

#define DGPU_HOLD_RST_HI()  EGPC_Write(EG_DGPU_HOLD_RST,0x03)
#define DGPU_HOLD_RST_LOW() EGPC_Write(EG_DGPU_HOLD_RST,0x02)

#define DGPU_GC6_EN_HI()    EGPC_Write(EG_DGPU_GC6_EN,  0x03)
#define DGPU_GC6_EN_LOW()   EGPC_Write(EG_DGPU_GC6_EN,  0x02)

#define NVDD_PWR_EN_HI()    EGPC_Write(EG_NVDD_PWR_EN,  0x03)
#define NVDD_PWR_EN_LOW()   EGPC_Write(EG_NVDD_PWR_EN,  0x02)

#define S_NVDD_PWR_EN_HI()  EGPC_Write(EG_S_NVDD_PWR_EN,0x03)
#define S_NVDD_PWR_EN_LOW() EGPC_Write(EG_S_NVDD_PWR_EN,0x02)

#define S_DGPU_RST_HI()     EGPC_Write(EG_S_DGPU_RST,   0x03)
#define S_DGPU_RST_LOW()    EGPC_Write(EG_S_DGPU_RST,   0x02)

#define S_DGPU_PWR_EN_HI()  EGPC_Write(EG_S_DGPU_PWR_EN,0x03)
#define S_DGPU_PWR_EN_LOW() EGPC_Write(EG_S_DGPU_PWR_EN,0x02)

#define DGPU_FB_CLAMP_HI()  EGPC_Write(EG_S_DGPU_PWR_EN,0x03)
#define DGPU_FB_CLAMP_LOW() EGPC_Write(EG_S_DGPU_PWR_EN,0x02)

#define IS_DGPU_PWROK()     EGPC_Read(EG_DGPU_PWROK)
#define IS_S_DGPU_PWROK()   EGPC_Read(EG_S_DGPU_PWROK)

//-----------------------------------------------------------------------------
// Input control
//-----------------------------------------------------------------------------
extern void Init_GPIO(void);
extern void Hook_SCION(void);
extern void Hook_SCIOFF(void);
extern void Hook_SMION(void);
extern void Hook_SMIOFF(void);
extern void Hook_A20ON(void);
extern void Hook_A20OFF(void);
extern void Hook_KBRSTON(void);
extern void Hook_KBRSTOFF(void);
extern void Hook_NUMLED_ON(void);
extern void Hook_NUMLED_OFF(void);
extern void Hook_CAPLED_ON(void);
extern void Hook_CAPLED_OFF(void);

typedef struct InitGPIOReg
{
    XBYTE 	*reg;
	BYTE	value;
} sInitGPIOReg;

#endif
