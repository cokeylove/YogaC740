/*-----------------------------------------------------------------------------
 * TITLE: OEM_BATTERY.H
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef BATTERY_H
#define BATTERY_H

/*-----------------------------------------------------------------------------
 * Data Parameter definition
 *---------------------------------------------------------------------------*/
#define SMbusTOC    200     // SMbus re-try counter

#define SMbusChA    0x00    // SMbus channel A index,SMbusCh1.
#define SMbusChB    0x01    // SMbus channel B index,SMbusCh2.
#define SMbusChC    0x02    // SMbus channel C index,SMbusCh3.
#define SMbusChD    0x03    // SMbus channel C index,SMbusCh4.
// msmart
//#define SMbusChC    0x00    // SMbus channel C index

//-----------------------------------------------------------------
// Define SMBus device address
//-------------------------------------------- --------------------
#define SmBat_Addr			0x16	// smart battery address
#define Charger_Addr		0x12	// charger address
#define BatteryAddress		0x16
#define SmartChgAddress		0x12


//-----------------------------------------------------------------
// Define smart battery command
//-------------------------------------------- --------------------
#define C_access           	0x00  	// manufacture access
#define C_RCalarm          	0x01 	// remaining capacity alarm
#define C_RTalarm          	0x02   	// remaining time alarm
#define C_mode             	0x03  	// battery mode
#define C_AtRate           	0x04   	// AtRate
#define C_AtRateFull       	0x05  	// AtRate time to full
#define C_AtRateEmpty      	0x06  	// AtRate time to empty
#define C_AtRateOK         	0x07   	// AtRate OK
#define C_temp             	0x08   	// temperature
#define C_volt             	0x09   	// voltage
#define C_current          	0x0A   	// current
#define C_AVcurrent        	0x0B   	// average current
#define C_MaxError         	0x0C  	// maximum error
#define C_RSOC             	0x0D  	// relative state of charge
#define C_ASOC             	0x0E   	// absolute state of charge
#define C_RMcap            	0x0F   	// remaining capacity
#define C_FCcap            	0x10   	// full charge capacity
#define C_RunTimeEmpty     	0x11 	// run time to empty
#define C_AVTimeEmpty      	0x12  	// average time to empty
#define C_AVTimeFull       	0x13   	// average time to full
#define C_ChargingI  	    0x14  	// charging current
#define C_ChargingV     	0x15   	// charging voltage
#define C_BatStatus        	0x16   	// battery status
#define C_CycleCount       	0x17   	// cycle count
#define C_DCap             	0x18   	// design capacity
#define C_DVolt            	0x19   	// design voltage
#define C_Spec             	0x1A 	// spec information
#define C_Date             	0x1B   	// manufacture date
#define C_SerialNo         	0x1C   	// serial number
#define C_Mname            	0x20 	// manufacturer name
#define C_Dname            	0x21  	// device name
#define C_Dchem            	0x22  	// device chemistry
#define C_Mdata            	0x23 	// manufacture data
#define C_OpMfgFun5         0x2F    //OptionalMfgFunction5 //MEILING031:add.
#define C_D_FET            	0x34 	// Ship Mode and Additional Over Discharge Mode.

#define C_FW_Status         0x35    // FW Update Status.
#define C_FW_Data           0x36    // FW Update Data.
#define C_BAT_Info          0x37    // Get Battery Information.

#define C_Cell1            	0x3F 	// manufacture data
#define C_Cell2            	0x3E 	// manufacture data  //OptionalMfgFunction2
#define C_LVMfgFun2   		0x3E	// LV Smart Battery
#define C_Cell3            	0x3D 	// manufacture data
#define C_Cell4            	0x3C 	// manufacture data

#define FirstUsedDate       0x3F   // FirstUsedDate   //ANGELAS033:Add

#define	NONE_BAT			0x00
#define	PWR_CHANGE			0x01
#define	CHARGE_BAT			0x02
#define	DISCHARGE_BAT		0x03
#define	PRE_CHARGE			0x04
#define	BAT_ERR				0x05

//-----------------------------------------------------------------
// Define charger command
//-------------------------------------------- --------------------
#define C_ChargerMode		0x12    // Set charger mode M+
#define C_ChargeCurrent 	0x14    // Set charge curretn
#define C_ChargeVoltage		0x15    // Set charge voltage
#define C_InputCurrent		0x3F    // Set input current
#define C_ChargerMode1		0x3B    // charger option1 
#define C_ChargerProchot	0x3C    // ProchotOption0
#define C_ChargerProchot1	0x3D    // ProchotOption1
#define C_ChargerDischargeCurrent		0x39    // Set Charge min voltage
#define C_ChargerMode2		0x38    // charger option2 
#define C_ChargerMode3		0x37   //charger option3 
#define C_MFID				0xFE	// Get Manufacturer ID
#define C_DeviceID			0xFF	// Get device ID


typedef struct RSmbusBStruct
{
    BYTE  	cmd;
    XBYTE 	*smbdataA;
	BYTE    block_limit;
} sRSmbusBStruct;
//ANGELAS070:add start
typedef struct RTVTstruct
{
	BYTE TEM;
	WORD VOL;
}SRTVTstruct;
//ANGELAS070:add end
//-----------------------------------------------------------------
// Charge and discharge charger rules
//-------------------------------------------- --------------------
#define ChangeChgBattery	0x82
#define ChangeDischgBattery 0x20
#define ABatteryLow			0x05
//#define BBatteryLow		0x05
#define ACriticalLow		0x02
//#define BCriticalLow		0x02
#define AOverVoltage		13400
//#define BOverVoltage		13400


//#if 0
//#define CHG_ABattery()	GPIOA.GPin5 = 1;GPIOA.GPin6 = 0;
//#define CHG_BBattery()	GPIOA.GPin5 = 0;GPIOA.GPin6 = 1;
//#define DISCHG_ABattery()	GPIOA.GPin7 = 1;
//#define DISCHG_BBattery()	GPIOA.GPin7 = 0;
//#endif
//#define CHG_ABattery()		DACDAT0=0xE7; 	//Set charge current
//#define CHG_BBattery()		;
//#define DISCHG_ABattery()	DACDAT0=0; 		//off charge current
//#define DISCHG_BBattery()	;
//#define	NONE_BAT		0x00
//#define	PWR_CHANGE		0x01
//#define	CHARGE_BAT		0x02
//#define	DISCHARGE_BAT	0x03
//#define	PRE_CHARGE		0x04
//#define	BAT_ERR			0x05


//extern bit bCheckChargeDeviceID(void);
//extern bit bCheckChargeMFID(void);
//extern bit bSetChargeCurrent(BYTE LowByte, BYTE HithByte);
//extern bit bSetChargeVoltage(BYTE LowByte, BYTE HithByte);
//extern bit bSetInputCurrent(BYTE LowByte, BYTE HithByte);
//extern bit bSetChargerMode(BYTE LowByte, BYTE HithByte);	// M+
extern void vPollingBatteryData(void);
extern void Charge_or_Discharge(void);
extern void	AC_DC_EVENT(void);
extern void	CheckAdapterInOut(void);
//extern void Get_All_PSID_Info(void);
//extern void bCheckBatteryIn(void);
//extern void vProcessDischarge(void);
//extern void vChkBatteyStatus(void);
//extern void ChkBatteryPercentChange(void);
//extern void vProcessCharge(void);
//extern bit bAOverVoltage(void);
//extern bit bBOverVoltage(void);
//extern bit bAOverTemperature(void);
//extern bit bBOverTemperature(void);
//extern void vCoypBatteryData(void);
//extern void vGetSelectorState(void);
extern void ChkBattery_Percl();
extern void Clear_Batt_First_Used_Date(void); //ANGELAG015: add

//-----------------------------------------------------------------
// PSID
//-------------------------------------------- --------------------
#define	PSID_FAIL		0
#define	PSID_GOOD		1
#define	ROM_SKIP		0xCC
//#define	ROM_READ			0x33
//#define	MEM_STATUS_READ	0xAA
#define	MEM_READ		0xF0

//----------------------------------------------------------------------------
// ROM Layout
//----------------------------------------------------------------------------
//#define PSID_DELL_HEADER        	0x00   	// 'D', 'E', 'L', 'L'
//#define PSID_REV_NUM            		0x04   	// 2 bytes ASCII
//#define PSID_CLASS_OF_PRODUCT   0x06   	// 'AC090', 'PS130'
#define PSID_WATTS            		0x08  	// 3 bytes,
//#define PSID_VOLTAGE            		0x0B   	// 3 bytes, ACSII: '195' = 19.5V
//#define PSID_CURRENT            		0x0E   	// 3 bytes, ASCII: '045' = 4.5A
//#define PSID_PPID               		0x11    // Label On AC Adapter
//#define PSID_PPID_MFGID			0x19	// Supplier MFG ID
//#define PSID_CHECKSUM           		0x28	// 2 byte CRC = X^16 + X^15 + X^2 + 1
//----------------------------------------------------------------------------
// adapter types
//----------------------------------------------------------
//#define AC_TYPE_MAX                     230
//#define AC_ADAPTER_MIN			65
#define AC_TYPE_UNKNOWN                 0
#define AC_TYPE_45W                	    45
#define AC_TYPE_50W                     50
#define AC_TYPE_60W                     60
#define AC_TYPE_65W                     65
#define AC_TYPE_70W                     70
#define AC_TYPE_90W                     90
#define AC_TYPE_120W                    120
#define AC_TYPE_130W                    130
#define AC_TYPE_150W                    150
#define AC_TYPE_230W                    230
#define AC_TYPE_240W					240
//#define ACAV_HW     bAdapter_In

extern void OEM_PollingBatData_TASK(void);
extern void WriteSmartChgIC(void);
extern void ReadSmartChgIC(void);
extern void Lenovo_Battery_EM80(void);
extern void Lenovo_Battery_EM60(void);
extern void FirstGetBatData(void);
extern void Battery1Sec(void);
extern void Chk_Shutdown_Volt(void);
extern void Chk_CHG_TimeOut(void);
extern void Chk_BatSMbusFailCount(void);
extern void Unlock_ShipMode(void);
extern void Lock_ShipMode(void);
extern void Chk_Battery_Full(void);
extern void UpBatteryPState();
extern void DownBatteryPState();
extern void CPUProchotCtrl();//MEILING033:add.
extern void Battery100ms(void);
extern void ChkBattery_abnormal(void);
extern void LV_BAT_Authentication(void);
extern void BATT_LEN_OFF(void);
extern void BATT_LEN_ON(void);
extern void PollingCPURT(void); //ANGELAS070:add
extern void PollingGPURT(void); //ANGELAS070:add
extern void ChkS3DCRSOC(void);//ANGELAS093
extern void DownACtoBatteryGPUState();  //MEILING055:add.
extern void Battery_Expresscharge(void);
extern void ChkPsys(void);
extern void TurboDCOnly(void); 
#endif
