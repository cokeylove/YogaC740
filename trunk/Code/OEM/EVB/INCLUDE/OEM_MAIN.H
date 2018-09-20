
#ifndef OEM_MAIN_H
#define OEM_MAIN_H

//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------
extern void Hook_Timer1msEvent(IBYTE EventId);
extern void Hook_Timer5msEvent(void);
extern void Hook_Timer2msEvent(void);//Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
extern void Hook_Timer4msEventA(void);//Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
extern void Hook_Timer4msEventB(void);//Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
extern void Hook_Timer20msEventA(void);//Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.

extern void Hook_Timer10msEventA(void);
extern void Hook_Timer10msEventB(void);
extern void Hook_Timer50msEventA(void);
extern void Hook_Timer50msEventB(void);
extern void Hook_Timer50msEventC(void);
extern void Hook_Timer100msEventA(void);
extern void Hook_Timer100msEventB(void);
extern void Hook_Timer100msEventC(void);
extern void Hook_Timer500msEventA(void);
extern void Hook_Timer500msEventB(void);
extern void Hook_Timer500msEventC(void);
extern void Hook_Timer1SecEventA(void);
extern void Hook_Timer1SecEventB(void);
extern void Hook_Timer1SecEventC(void);
extern void Hook_Timer1MinEvent(void);
extern void service_OEM_1(void);
extern void service_OEM_2(void);
extern void service_OEM_3(void);
extern void service_OEM_4(void);
extern void Hook_main_service_H(void);
extern void Hook_main_service_L(void);
extern BYTE Hook_Only_Timer1msEvent(void);
extern BYTE OEM_SkipMainServiceFunc(void);

extern void PollingBIOS80Port(void);
extern void ProcessSID(BYTE SID);
extern bit CheckDebugMode(void);
extern void Workaround_For_HangBios(void);//ANGELAS094:Add workaround for hang bios.
extern void ACOutProchotRelease(void); //MEILING039:add.
extern void GPUProchotOnControl(void); //MEILING042:add.
extern void CPUThrottlingDelay(void); //MEILING052:add.
#endif
