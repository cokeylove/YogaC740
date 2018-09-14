/*-----------------------------------------------------------------------------
 * TITLE: OEM_FAN.H
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef OEM_FAN_H
#define OEM_FAN_H

//=============================================================================
// 	structure define
//=============================================================================
typedef struct FanControlstruct
{
	BYTE T_Off;
	BYTE T_Low;
	BYTE T_Step1;
	BYTE T_Step2;
	BYTE T_Step3; //MEILING003:add.
	BYTE T_High;
	XBYTE *DCRx;
} FanControlstruct;
//72JERRY014¡êo s+Add the second fan method
typedef struct Fan1Controlstruct
{
	BYTE T_Off;
	BYTE T_Low;
	BYTE T_Step1;
	BYTE T_Step2;
	BYTE T_Step3; //MEILING003:add.
	BYTE T_High;
	XBYTE *DCRx;
} Fan1Controlstruct;
//72JERRY014¡êo e+Add the second fan method

typedef struct CPUTHRstruct
{
	BYTE Thr_Off;
	BYTE Thr_On;
	BYTE Turbo_Rem;
	BYTE Turbo_Off;
	BYTE Thr_Shut;
} CPUTHRstruct;

typedef struct thermalstruct
{
	BYTE 	Level;
	BYTE 	CFanOn;
	BYTE 	CFanOff;
	BYTE	CRPM;
}thermal;

typedef struct thermaltablestruct
{
	BYTE	*FanOn;
	BYTE	*FanOff;
	BYTE	*RPM;
}thermaltable;


#define TEMP_TYPE_FAN1RPM	1   
#define TEMP_TYPE_FAN2RPM	2  

//========Temperature==========================================================
#define TmlIC_Addr			0x98    // Thermal IC SMBus address.//G12:Change thermal IC address from 0X9A to 0X98.
#define VGA_Addr			0x82			//0x9E    // AMD SMBus address.
#define EXTVGA_Addr			0x9E    // nVIDIA SMBus address.

#define TmlIC_Temp			0x00    // Thermal IC Temperature Data register.
#define Remote_Temp			0x01    // Thermal IC EXT 1 Temperature Data register.
#define TmlIC_Rate			0x04    // Thermal IC Conversion Rate register.
#define SSD_Temp			0x23    // Thermal IC EXT 2 Temperature Data register.

extern void ThmIC_Temperature(void);
extern void Oem_Thermal_Control(void);
extern void VGA_Temperature(void);
//extern void EXTVGA_Temperature(void); //ANGELAS070:remove
extern void Clr_Thermal_Tab(void);
//void Tml_SMLink(void); //ANGELAS050:remove
void FanTab_ToRAM(BYTE BStep);
void THRTab_ToRAM(BYTE BStep);
void VFanTab_ToRAM(BYTE BStep);
void VTHRTab_ToRAM(BYTE BStep);

void Fan1Tab_ToRAM(BYTE BStep);//72JERRY014¡êo Add the second fan method
void VFan1Tab_ToRAM(BYTE BStep);//72JERRY014¡êo Add the second fan method
//=============================================================================
//==========F-A-N==============================================================
#define FAN_PWM_Max			CTR2	// PWM Maximum CTR2.
#define FAN1_PWM_Max			CTR2	// PWM Maximum CTR2.//72JERRY014¡êo Add the second fan method
//T076-  #define FAN_Std_PWM_Max		Cal_PWM_Duty(FAN_Std_Max,FAN_PWM_Max)	// Standard PWM Maximum.
//T076-  #define FAN_DOS_PWM_Max		Cal_PWM_Duty(FAN_DOS_Max,FAN_PWM_Max)	// DOS PWM Maximum.

#define FAN_Std_PWM_Max		FAN_Std_Max	//T076+ // Standard PWM Maximum.
#define FAN_DOS_PWM_Max		FAN_DOS_Max	//T076+  // DOS PWM Maximum.

#define FAN1_Std_PWM_Max		FAN1_Std_Max	//T076+ // Standard PWM Maximum.//72JERRY014¡êo Add the second fan method
#define FAN1_DOS_PWM_Max		FAN1_DOS_Max	//T076+  // DOS PWM Maximum.//72JERRY014¡êo Add the second fan method

//#define FAN_Quite_PWM_Max	Cal_PWM_Duty(FAN_Quite_Max,FAN_PWM_Max)	// Quite PWM Maximum.
//#define FAN_Super_PWM_Max	Cal_PWM_Duty(FAN_SP_Max,FAN_PWM_Max)	// Super performance PWM Maximum.
//T076-  #define FAN_Std_PWM1		Cal_PWM_Duty(FAN_PWM_1,FAN_PWM_Max)		//
//T076-  #define FAN_Std_PWM2		Cal_PWM_Duty(FAN_PWM_2,FAN_PWM_Max)		//

#define FAN_Std_PWM1		FAN_PWM_1		//T076-  
#define FAN_Std_PWM2		FAN_PWM_2		//T076- 
#define FAN_Std_PWM3        FAN_PWM_3       //MEILING003:add.

#define FAN1_Std_PWM1		FAN1_PWM_1		//T076-  //72JERRY014¡êo Add the second fan method
#define FAN1_Std_PWM2		FAN1_PWM_2		//T076- //72JERRY014¡êo Add the second fan method
#define FAN1_Std_PWM3        FAN1_PWM_3       //MEILING003:add.//72JERRY014¡êo Add the second fan method

#define FAN_PWM_Min4		Cal_PWM_Duty(50,FAN_PWM_Max)	// 34% Standard PWM Minimum.	//CHANGE 34 TO 50
#define FAN_PWM_Min			Cal_PWM_Duty(29,FAN_PWM_Max)	//29 //T14A: cange 30 to 35   // 30% Standard PWM Minimum.



#define FAN_RPM_Min		    FAN_RPM_L  //x*100   T076+
#define FAN1_RPM_Min		    FAN1_RPM_L  //x*100   T076+//72JERRY033: Modify fan table follow '720S_13IKB fan table_V01'.

#define FAN_Std_RR1			Cal_PWM_Duty(1,FAN_PWM_Max)		// 1% Ramp rate1.
#define FAN_Std_RR2			Cal_PWM_Duty(2,FAN_PWM_Max)		// 2% Ramp rate2.
#define FAN_duty_offset		Cal_PWM_Duty(5,FAN_PWM_Max)		// 5% Duty cycle offset to target for duty ramp rate stting.
#define FAN_Boot_On			0x1e	// 3 Sec.//optimize power on fan boot time.change 0x03 to 0x1e.

// Temperature Data Index
#define TEMP_TYPE_CPU		0
#define TEMP_TYPE_VGA		1
#define TEMP_TYPE_EXT		2
#define TEMP_Compare		3
#define TEMP_TYPE_local		4   //G90
#define TEMP_TYPE_remote	5   //G90
#define nVramoverTemp       90 //G90
#define nRamoverTemp       90 	//G90
#define nCPUthermistoroverTemp       95 //ANGELAS070:add//72JERRY061:Update fan table follow'720S_13IKB fan table_V04'.
#define nGPUthermistoroverTemp       106 //ANGELAS070:add

#define CPUTurboOn_VGATemp           60 //ANGELAG006: modify to 60  //MEILING040:add.
#define CPUTurboOff_VGATemp          63  //ANGELAG006: modify to 63 //MEILING024:add.  //MEILING040:modify to 60.
//#define CPUTurboOff_VGATemp_15       60  //MEILING033:add 15' GPU temperature for CPU turbo off.  //MEILING040:remove.
#define CPUTurboOff_VGATemp_15       63  //ANGELAG006: ADD
/* //ANGELAG006: remove start
#define InGPUTurboOn_CPUTemp      	 51  //MEILING040:add.
#define InGPUTurboOff_CPUTemp     	 57  //MEILING040:add.
#define InGPUTurboOn_CPUTemp_15      54  //MEILING040:add.
#define InGPUTurboOff_CPUTemp_15     61  //MEILING040:add.
*/ //ANGELAG006: remove end
//ANGELAG006: add start
#define InGPUTurboOn_CPUTemp      	 60  //ANGELAG006:add.
#define InGPUTurboOff_CPUTemp     	 62  //ANGELAG006:add.
#define InGPUTurboOn_CPUTemp_15      60  //ANGELAG006:add.
#define InGPUTurboOff_CPUTemp_15     62  //ANGELAG006:add.
//ANGELAG006: add end
#define Fan_Num				2;
#define Fan_Step			5;

#define PSTATE_MAXStep      0x0b //0ptimize CPU P_state (change 16 step to 8 step)//:Change MAX P_state from 7 step to 11 step.



extern void Oem_Fan_Speed(void);	// Read Fan Speed RPM.
extern void Oem_Fan_Control(void);	// Main FAN control.
extern void Chk_FAN_RPM_Control(void);	// Check FAN is AP control.
extern void Fan_Std_Mode(BYTE BMBStep);	// Fan Table:Standard Mode.
/* //ANGELAS050:remove start
void Fan_DOS_Mode(BYTE BMBStep);	// Fan Table:DOS Mode.
void Fan_Quite_Mode(BYTE BMBStep);	// Fan Table:Quite Mode.
void Fan_Super_Mode(BYTE BMBStep);	// Fan Table:Super performance Mode.
*/ //ANGELAS050:remove end
BYTE Fan_Temp_compare(BYTE BCPU_Step,BYTE BVGA_Step,BYTE BTType);	// Fan CPU and VGA compare temperature.
void Fan_Cal_PWMStep(BYTE BStep,BYTE BPWM_Max, BYTE BPWM_Min, BYTE BPWM_Step1, BYTE BPWM_Step2, BYTE BPWM_Step3, BYTE FAN_Type_Tab);	// Calculation temperature Step  //MEILING003:add step3 for fan control.
void Fan_Cal_PWMDuty(BYTE BPWM_Max, BYTE BPWM_Min , BYTE BTmin, BYTE BTmax, BYTE BTemp, XBYTE *BTg_Duty);	// Calculate PWM duty.
void Fan_Offset_Chk(BYTE Duty_Offset, XBYTE *BDCRx);	// Check PWM Duty Offset.
//72JERRY014¡êos+ Add the second fan method
extern void Oem_Fan1_Speed(void);	// Read Fan Speed RPM.
extern void Oem_Fan1_Control(void);	// Main FAN control.
extern void Chk_FAN1_RPM_Control(void);	// Check FAN is AP control.
void Fan1_Std_Mode(BYTE BMBStep);	// Fan Table:Standard Mode.
BYTE Fan1_Temp_compare(BYTE BCPU_Step,BYTE BVGA_Step,BYTE BTType);	// Fan CPU and VGA compare temperature.
void Fan1_Cal_PWMStep(BYTE BStep,BYTE BPWM_Max, BYTE BPWM_Min, BYTE BPWM_Step1, BYTE BPWM_Step2, BYTE BPWM_Step3, BYTE FAN_Type_Tab);	// Calculation temperature Step  //MEILING003:add step3 for fan control.
void Fan1_Cal_PWMDuty(BYTE BPWM_Max, BYTE BPWM_Min , BYTE BTmin, BYTE BTmax, BYTE BTemp, XBYTE *BTg_Duty);	// Calculate PWM duty.
void Fan1_Offset_Chk(BYTE Duty_Offset, XBYTE *BDCRx);	// Check PWM Duty Offset.
//72JERRY014¡êo e+Add the second fan method
//=============================================================================
//=======Throttling============================================================
/* //ANGELAS050:remove start
#if Support_Steering_Fun
#define VGA_Steer85_On		70			// VGA Steering ON
#define VGA_Steer85_Off		80			// VGA Steering OFF
#define VGA_Steer_On		80			// VGA Steering ON
#define VGA_Steer_Off		90			// VGA Steering OFF
#endif
*/ //ANGELAS050:remove end
#define Thro_Shut_Cnt		3			// 3 times.
#define RSOC_Shut_Cnt		30           //ANGELAS043: add

extern void OEM_Throttling_Ctrl(void);	// Temperature Protect Center.
void Thro_Std_Mode(BYTE BMBStep);		// Throttling: Standard Mode.
/* //ANGELAS050:remove start
void Thro_DOS_Mode(BYTE BMBStep);		// Throttling: DOS Mode.
void Thro_Quite_Mode(BYTE BMBStep);		// Throttling: Quite Mode.
void Thro_Super_Mode(BYTE BMBStep);	// Throttling: Super performance Mode.
*/ //ANGELAS050:remove end
void Thro_Shut_Status(BYTE BStep, BYTE BTType);			// Throttling: Check throttle and shutdown status.
void Thro_Turbo(BYTE BTurboRem, BYTE BTurboOff, BYTE BTType);	// Throttling: Check GPU turbo status.
//void Thro_Street(BYTE BStep, BYTE BTType);				// Check VGA Street status. //ANGELAS050:remove
void Thro_Count_Shut(XBYTE *BShut_Cnt, BYTE TempType);	// Throttling: Shutdown count used.
//=============================================================================

extern void PWM_TimeCount(void);
//extern void ODDPowerSaving(void); //ANGELAS050:remove
extern void Oem_Fan_Speed(void);
extern void Fan_Init(void);
extern void ThrottlingControl(void);
extern void GPUThrottlingControl(void); //MEILING033:add.
extern void FAN_Dust_Mode(void);
#endif

