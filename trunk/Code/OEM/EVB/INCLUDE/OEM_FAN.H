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
	BYTE T_Step3; 
	BYTE T_High;
	XBYTE *DCRx;
} FanControlstruct;

//Add the second fan method
typedef struct Fan1Controlstruct
{
	BYTE T_Off;
	BYTE T_Low;
	BYTE T_Step1;
	BYTE T_Step2;
	BYTE T_Step3; 
	BYTE T_High;
	XBYTE *DCRx;
} Fan1Controlstruct;


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

extern void Clr_Thermal_Tab(void);

void FanTab_ToRAM(BYTE BStep);
void THRTab_ToRAM(BYTE BStep);
void VFanTab_ToRAM(BYTE BStep);
void VTHRTab_ToRAM(BYTE BStep);

void Fan1Tab_ToRAM(BYTE BStep);
void VFan1Tab_ToRAM(BYTE BStep);
//=============================================================================
//==========F-A-N==============================================================
#define FAN_PWM_Max			CTR2	// PWM Maximum CTR2.
#define FAN1_PWM_Max			CTR2	// PWM Maximum CTR2.//72JERRY014¡êo Add the second fan method
//  #define FAN_Std_PWM_Max		Cal_PWM_Duty(FAN_Std_Max,FAN_PWM_Max)	// Standard PWM Maximum.
//  #define FAN_DOS_PWM_Max		Cal_PWM_Duty(FAN_DOS_Max,FAN_PWM_Max)	// DOS PWM Maximum.

#define FAN_Std_PWM_Max		FAN_Std_Max	// Standard PWM Maximum.
#define FAN_DOS_PWM_Max		FAN_DOS_Max	// DOS PWM Maximum.

#define FAN1_Std_PWM_Max		FAN1_Std_Max	// Standard PWM Maximum.
#define FAN1_DOS_PWM_Max		FAN1_DOS_Max	// DOS PWM Maximum.

#define FAN_Std_PWM1		FAN_PWM_1		 
#define FAN_Std_PWM2		FAN_PWM_2		
#define FAN_Std_PWM3        FAN_PWM_3      

#define FAN1_Std_PWM1		FAN1_PWM_1	
#define FAN1_Std_PWM2		FAN1_PWM_2		
#define FAN1_Std_PWM3        FAN1_PWM_3    

#define FAN_PWM_Min4		Cal_PWM_Duty(50,FAN_PWM_Max)	// 34% Standard PWM Minimum.	//CHANGE 34 TO 50
#define FAN_PWM_Min			Cal_PWM_Duty(29,FAN_PWM_Max)	//29  // 30% Standard PWM Minimum.



#define FAN_RPM_Min		    FAN_RPM_L  //x*100  
#define FAN1_RPM_Min		    FAN1_RPM_L  //x*100   

#define FAN_Std_RR1			Cal_PWM_Duty(1,FAN_PWM_Max)		// 1% Ramp rate1.
#define FAN_Std_RR2			Cal_PWM_Duty(2,FAN_PWM_Max)		// 2% Ramp rate2.
#define FAN_duty_offset		Cal_PWM_Duty(5,FAN_PWM_Max)		// 5% Duty cycle offset to target for duty ramp rate stting.
#define FAN_Boot_On			0x1e	// 3 Sec.//optimize power on fan boot time.change 0x03 to 0x1e.

// Temperature Data Index
#define TEMP_TYPE_CPU		0
#define TEMP_TYPE_VGA		1
#define TEMP_TYPE_EXT		2
#define TEMP_Compare		3
#define TEMP_TYPE_local		4  
#define TEMP_TYPE_remote	5   
#define nVramoverTemp       90 
#define nRamoverTemp       90 	
#define nCPUthermistoroverTemp       95 
#define nGPUthermistoroverTemp       106 

#define CPUTurboOn_VGATemp           60 
#define CPUTurboOff_VGATemp          63 

#define CPUTurboOff_VGATemp_15       63  

#define InGPUTurboOn_CPUTemp      	 60  
#define InGPUTurboOff_CPUTemp     	 62  
#define InGPUTurboOn_CPUTemp_15      60  
#define InGPUTurboOff_CPUTemp_15     62 

#define Fan_Num				2;
#define Fan_Step			5;

#define PSTATE_MAXStep      0x0b 



extern void Oem_Fan_Speed(void);	// Read Fan Speed RPM.
extern void Oem_Fan_Control(void);	// Main FAN control.
extern void Chk_FAN_RPM_Control(void);	// Check FAN is AP control.
extern void Fan_Std_Mode(BYTE BMBStep);	// Fan Table:Standard Mode.

BYTE Fan_Temp_compare(BYTE BCPU_Step,BYTE BVGA_Step,BYTE BTType);	// Fan CPU and VGA compare temperature.
void Fan_Cal_PWMStep(BYTE BStep,BYTE BPWM_Max, BYTE BPWM_Min, BYTE BPWM_Step1, BYTE BPWM_Step2, BYTE BPWM_Step3, BYTE FAN_Type_Tab);	// Calculation temperature Step 
void Fan_Offset_Chk(BYTE Duty_Offset, XBYTE *BDCRx);	// Check PWM Duty Offset.

extern void Oem_Fan1_Speed(void);	// Read Fan Speed RPM.
extern void Oem_Fan1_Control(void);	// Main FAN control.
extern void Chk_FAN1_RPM_Control(void);	// Check FAN is AP control.
void Fan1_Std_Mode(BYTE BMBStep);	// Fan Table:Standard Mode.
BYTE Fan1_Temp_compare(BYTE BCPU_Step,BYTE BVGA_Step,BYTE BTType);	// Fan CPU and VGA compare temperature.
void Fan1_Cal_PWMStep(BYTE BStep,BYTE BPWM_Max, BYTE BPWM_Min, BYTE BPWM_Step1, BYTE BPWM_Step2, BYTE BPWM_Step3, BYTE FAN_Type_Tab);	// Calculation temperature Step  
void Fan1_Offset_Chk(BYTE Duty_Offset, XBYTE *BDCRx);	// Check PWM Duty Offset.
//Add the second fan method
//=============================================================================
//=======Throttling============================================================
#define Thro_Shut_Cnt		3			// 3 times.
#define RSOC_Shut_Cnt		30         

extern void OEM_Throttling_Ctrl(void);	// Temperature Protect Center.
void Thro_Std_Mode(BYTE BMBStep);		// Throttling: Standard Mode.

void Thro_Shut_Status(BYTE BStep, BYTE BTType);			// Throttling: Check throttle and shutdown status.
void Thro_Turbo(BYTE BTurboRem, BYTE BTurboOff, BYTE BTType);	// Throttling: Check GPU turbo status.
void Thro_Count_Shut(XBYTE *BShut_Cnt, BYTE TempType);	// Throttling: Shutdown count used.
//=============================================================================

extern void PWM_TimeCount(void);

extern void Oem_Fan_Speed(void);
extern void Fan_Init(void);
extern void ThrottlingControl(void);
extern void GPUThrottlingControl(void); 
extern void FAN_Dust_Mode(void);
#endif

