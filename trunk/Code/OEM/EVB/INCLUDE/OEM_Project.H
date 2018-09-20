/*----------------------------------------------------------------------------
 * Filename: OEM_PROJECT.H  For Chipset: ITE.IT85XX
 *
 * Function: Project function selection.
 *
 * Copyright (c) 2009 - , ITE Tech. Inc. All Rights Reserved.
 *
 *---------------------------------------------------------------------------*/

#ifndef OEM_Project_H
#define OEM_Project_H

//*****************************************************************************
// Kernel code feature
//*****************************************************************************
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ITE EC Function define
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define Support_EC_Sleep	        TRUE 
#define Support_External_IO	    	FALSE  // Support EC External IO function.

#define Support_Mirror_Code	    	TRUE  

#define Support_VC0_Function	    FALSE  // change False to true  // (set to FALSE to avoid turn off MainPWRON)  // Support Voltage Compare 0 Function.
#define Support_VC1_Function	    FALSE  // False(use FW to control later)   //: True  // Support Voltage Compare 1 Function.
#define Support_VC2_Function	    FALSE  // Support Voltage Compare 2 Function.

#define Support_USB_Charge	    	FALSE 
#define SUPPORT_UCSI                FALSE //Disable UCSI feature.
#define SUPPORT_TYPEC_ADAPTER       FALSE  
#define SUPPORT_ADCID_ADAPTER       FALSE
#define SUPPORT_NOID_ADAPTER        FALSE 
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ITE EC Function Setting and selection
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#if Support_Mirror_Code
#define EC_Signature_Flag	     0x10    // 0x94 to 0x10 use New FU to do CRC.    // For 8518 crystal free //0x11 to 0x10
#else
#define EC_Signature_Flag	     0xB5    // For 8518 crystal free
#endif

#if Support_Mirror_Code
#define EC_Mirror_Size		     0x7F
#else
#define	EC_Mirror_Size		     0xAA
#endif

//-----------------------------------------------------------------------------
// PS2 interface
//-----------------------------------------------------------------------------
#define TouchPad_only           TRUE    // True : Only touch pad device in this project //
                                        /* Note : If TouchPad_only is set, remeber to call
                                            PowerOffClearTPPendingData() function in power off sequnce
                                            include (S3, S4, and S5)!C*/

#if TouchPad_only
#define MultiplexedModes        FALSE   // 0 : Doesn't support multiplexed modes.
#else
#define MultiplexedModes        TRUE    // 1 : Support multiplexed modes.
#endif

#define Support_TPLED		    FALSE   // Support TouchPad LED. //change to false

//-----------------------------------------------------------------------------
// Power sequence only
//-----------------------------------------------------------------------------
#define EN_PwrSeqTest           FALSE   

//-----------------------------------------------------------------------------
// Keyboard scan
//-----------------------------------------------------------------------------
#define KB_FnStickKey           FALSE   // True : for Fn stick key function
#define SUPPORT_KB_RAM_TABLE  	TRUE	
#define SHA1_Support			TRUE	
#define WDT_Support				TRUE    

#define ACTION					0x03

#define EEPROMA2				0x01

#define EEPROMA1_D1				0xA0
#define EEPROMA1_D2				0xA4

#define EEPROMA1_M1D			0xA0
#define EEPROMA1_M2D			0xA4
#define EEPROMA1_M1				0xA8
#define EEPROMA1_M2				0xAC

#define EEPROMA1_B03			0xB0
#define EEPROMA1_B47		    0xB4

#define EEPROMA1_16Byte			0xB8
#define EEPROMM1_16Byte_end		0xBB

//*****************************************************************************
// UART debug interface
// Timer 2 baudrate = Fosc / 32 / (0x10000-RCAP2)
//*****************************************************************************
#define UART_Debug        // Define : enable Tx debug (GPIO)

#ifdef UART_Debug
// 9600 bps-------------------------
//#define R2HV                  0xFF
//#define R2LV                  0xE2
// 9600 bps-------------------------

// 19200 bps------------------------
//#define R2HV                  0xFF
//#define R2LV                  0xF1
// 19200 bps------------------------

// 57600 bps------------------------
#define R2HV                    0xFF
#define R2LV                    0xFB
// 57600 bps------------------------
#endif

#define AUTO_MODE_ON_EVENT		0xAE
#define AUTO_MODE_OFF_EVENT		0xAF

#define LCD_BRIG_DEC_EVENT		0x11	// Brightness decrease
#define LCD_BRIG_INC_EVENT		0x12	// Brightness increase
#define Fn_Q_EVENT				0x13

#define SCI_LID_Close			0x16	//change LID implement method.
#define SCI_LID_Open			0x15	//change LID implement method.

#define QUCSI_Data_Ready        0x20    // UCSI Data Ready //UCSI feature notify OS

#define CPU_SLOW_AD				0x1D	// EC set throttle function and P-state
#define GPU_SLOW                0x1E    //add GPU throttling event.
#define ACPI_BAT1IN_SCI			0x25	// Battery1 IN event
#define Powerbotton_SCI			0x32	// power button event//72JERRY064:Modify press button 1s enter S3/S4/shutdown follow UI spec.
#define ACPI_ACIN_SCI			0x37	// AC in event
#define ACPI_ACOUT_SCI			0x38	// AC out event

#define SDV_VPC_notify          0x44
#define F7_Airplan_notify       0x48   
#define ODD_HotKey_EVENT		0x52    // For ODD eject Hotkey event
#define ODD_BTN_EVENT			0x53    // For ODD eject Button event

#define EN_TURBO_61	 			0x61    // Enable VGA Turbo
#define EN_TURBO_62	 			0x62    // Enable VGA Turbo
#define DIS_TURBO_63	 		0x63    // Disable VGA Turbo
#define EN_PWRSTEER_64	 		0x64    //

#define WLAN_BTN_EVENT_65		0x65	// WLAN and BT status notify
#define DIS_CPUTURBO_66         0x66    //modify event name to disable.
#define EN_CPUTURBO_67         	0x67    //modify event name to enable.  
 #define DTS_READ_CPU_TEMP      0x50

#define WLANBTFOROSD         	0x42   	// Add sent Qevent 0x42 for WLAN&BT on/off under win7.

//*****************************************************************************
// OEM code feature
//*****************************************************************************
#define Init_FMSSR			    0x1F	// 0x1F 2M Bytes Share Rom
#define Init_BADR               0x01    // Base address select 0b00:2E 2F 0b01:4E 4F 0b10:User define SWCBALR | SWCBAHR
#define Init_SWCBAHR            0x00    // I/O Port High Byte
#define Init_SWCBALR           	0x4E    // I/O Port Low  Byte
#define Init_I2EC               0x43    // Enable I2EC R/W and log 80 port

//-----------------------------------------------------------------------------
// OEM define.
//-----------------------------------------------------------------------------
#define SW_ChkTimeScale    	   	TRUE	// Support Timer.
#define LID_OnlyQEvent_Support  TRUE    // change LID implement method.

#define Lenovo_VGATurbo	 		FALSE	// Support VGA Turbo.
#define FAN_TABLE_Already		TRUE     
#define SW_ISCT					FALSE	// Support ISCT function.
#define Support_TJ100			TRUE	// Support CPU TJ100.
#define ACPI_Smbus				TRUE	// Support ACPI command
#define Lenovo_FAN_Dust			TRUE	// Support FAN Dust function.
#define Support_EC_LANWake		FALSE    //TRUE	// Support Lan Wake by EC control.
#define shipmodesupport			TRUE    //add ship mode judge //ANGELAS032: flase to true
#define turboboostandstorage	TRUE    //storage mode action with support turbo boost charger
#define WorkaroundHangBios      TRUE    //Add workaround for hang bios.
#define SUPPORT_CYPRESS_PD_CCG4  TRUE   //Disable battery shal1 and enable flash PD FW.
#define SUPPORT_PMIC			TRUE
#define SUPPORT_GPU_PROCHOT     TRUE //HEGANGS002:Support gpu and cpu prochot
// Battery
#define S3RSOCPercentage	    0x05		// 5%
#define BatteryOTPSD  		    65          // 65 degree  
#define BatteryOTPHi		    60			// 60 degree  
#define BatteryOTPLow	        58			// 58 degree 
#define FastChgTimeOut		    720 		// 12 * 60 = 720min
#define BattRSOC_ProchotOn      7       //limit low percent power consumption
#define BattRSOC_ProchotOff     7       //limit low percent power consumption  
#define SysPower_ProchotOn      32       
#define SysPower_ProchotOff     20       
#define SysPower_GPUThrottling  25         
/*===========================================================================
 * AOU support
 *===========================================================================*/
#define Support_AOU5_V1_2                  TRUE      //1: support v1.2      0: support v1.0

//-----------------------------------------------------------------------------
// Shutdown ID
//-----------------------------------------------------------------------------
#define REV0_ID				           0x00	// Reserved
#define S5_ID				           0x01	// Shutdown ID for SB SLP_S5 signal
#define COMMAND_ID			           0x04	// Shutdown ID for command C:0x59 D:0xA2
#define FLASH_ID			           0x05	// Shutdown ID for flash
#define COLDBOOT_ID			           0x06	// Shutdown ID for EC power reset

#define BOOTFAIL_ID			           0x08	// Shutdown ID for boot fail restart by command C:0x40 D:0xXX

#define BATTOVERTEMP_ID		           0x0A	// Shutdown ID for battery over temp
#define THERMALCOMMFAIL_ID	           0x0B	// Shutdown ID for thermal sensor communication fail
#define BATTEMPTY_ID		           0x0C	// Shutdown ID for battery empty

#define BATTCOMMFAIL_ID			       0x0E	// Shutdown ID for battery communication fail
#define BATTABNORMAL_ID		           0x0F	// Shutdown ID for battery abnormal device ID

#define DTSOVERTEMP_ID		           0x11	// Shutdown ID for DTS over temp
#define VGAOVERTEMP_ID		           0x12	// Shutdown ID for VGA thermal sensor over temp
#define BATTLOWVOLT_ID		           0x13	// Shutdown ID for battery low voltage

#define EXTVGAOVERTEMP_ID	           0x1C	// Shutdown ID for External VGA thermal sensor over temp

#define VGACOMMFAIL_ID		           0x20	// Shutdown ID VGA thermal sensor communication fail
#define EXTVGACOMMFAIL_ID	           0x21	// Shutdown ID External VGA thermal sensor communication fail
#define CPUCOMMFAIL_ID		       	   0x22	// Shutdown ID CPU thermal sensor communication fail

#define ThermalIClocalOVerTEPM_ID	   0x30	// Shutdown ID thermal sensor local over temp  
#define ThermalICremoteOVerTEPM_ID	   0x31	// Shutdown ID thermal sensor remote over temp  
#define CPUthermistoroverTemp_ID	   0x33	// Shutdown ID CPU thermistor over temp  
#define GPUthermistoroverTemp_ID	   0x34	// Shutdown ID GPU thermistor over temp 

#define PMIC_Shutdown_ID	               0x36	

#define COLDBOOTFAIL_ID                0x3A 

#define RSOC_1Pto0P_ID	               0x40	

#define SYSACTION_ID                   0x41
#define WORKAROUND_ID                  0x42
#define FLASHEC_ID                     0x43
#define RESETEST_ID                    0x44

#endif

