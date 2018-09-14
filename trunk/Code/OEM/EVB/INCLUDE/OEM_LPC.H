/*-----------------------------------------------------------------------------
 * TITLE: OEM_LPC.H
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *---------------------------------------------------------------------------*/

#ifndef OEM_LPC_H
#define OEM_LPC_H

//unsigned char Display_Debug_Message;	//Display debug message via serial port
										//  1  :   Enable, 0: Disable
//extern BYTE Display_Debug_Message;

extern void MuteProcess(void);
extern void CameraProcess(void);
extern void WirelessProcess(void);
extern void NovoProcess(void);
extern void IFFSProcess(void);
//extern void ECPowerDownEnableExternalTimer2(void);  //ANGELAS057:add //ANGELAS058:remove
//extern void ECDisableExternalTimer2(void);  //ANGELAS057:add  //ANGELAS058:remove
extern void SetACIN_Int(void);
extern void ACInOutIntEnable(void); //MEILING039:add.
extern void ACInOutIntDisable(void); //MEILING039:add.
extern void SetVGA_AC_DET(void);
extern void SetPWRSW_Int(void);
extern void Setlanwake_Int(void);
extern void SetNovo_Int(void);
extern void MXLID_Wake_En(void);
extern void SlpS3_Wake_En(void);
//ANGELAS044:add start
extern void SlpS3_Wake_Dis(void); 
extern void ECPowerDownEnableExternalTimer2(void);  
extern void ECDisableExternalTimer2(void);  
extern void SlpS4_Wake_En(void);		// GPD1 WUI1 ------> INT31
extern void SlpS4_Wake_Dis(void);		// GPD1 WUI1 ------> INT31
extern void InterKBDWakeEnable(void);
extern void InterKBDWakeDisable(void);
//ANGELAS044:add end
extern void SLPS3_Sleep_En(void); //ANGELAS063:add
extern void SLPS4_Sleep_En(void); //ANGELAS063:add
extern void SetExTimer1(void);
extern void WakeUp_30ms_Function(void);
extern void WakeUp_DO_Function(void);
extern void ISCT_Process(void);
extern void ISCT_TimerCnt(void);
extern void GL887_Main(void);
extern void BatteryINWakeEnable(void);		// GPI2   WUI73 -------> int126//72JERRY020: Modify enter EC sleep setting.
extern void BatteryOutWakeEnable(void);		// GPI2   WUI73 -------> int126//72JERRY020: Modify enter EC sleep setting.
extern void SetWOV_Int(void);//72JERRY028: Modify Voice Wake signal status in deep sleep.
#if SW_ISCT
#define ISCT_BAT_LOW			20		// Inhibit ISCT wake battery low capacity condition.
#define ISCT_BAT_OvTpLimit		50		// Inhibit ISCT wake 50 degree.
#define ISCT_BAT_UnTpLimit		0xFB	// Inhibit ISCT wake -5 degree
void ISCT_Behavior(void);
#endif

#endif