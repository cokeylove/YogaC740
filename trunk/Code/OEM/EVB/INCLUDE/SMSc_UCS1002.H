
#ifndef SMSC_UCS1002_H
#define SMSC_UCS1002_H

#define SMSC_UCS1002ID  0x4E

void Init_USB_S0(void);
extern void UCS1002HCACharger_Init(void);
extern void UCS1002HCACharger(void);
extern BYTE ReadUCS1022ID(void);
extern void UCS1002_TimerX_FakeISR(void);
extern void UCS1002ID_Main(void);
extern void Ck_UpdateProfiles();
extern void SMSC_ProfilesRead(XBYTE AddrH,XBYTE AddrL,XBYTE ByteCnt);
extern void SMSC_ProfilesUpdate(XBYTE AddrH,XBYTE AddrL,XBYTE ByteCnt);
#endif
