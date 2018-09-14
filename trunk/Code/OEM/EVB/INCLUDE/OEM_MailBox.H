/*-----------------------------------------------------------------------------
 * TITLE: OEM_MAILBOX.H
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef	OEM_MailBox_H
#define OEM_MailBox_H

#define VPCeventOneKey				0x01
#define VPCeventGenenal				0x02
#define VPCcmdBattHealth			0x02
//#define VPCeventWLAN				0x01
//#define VPCeventBluetooth			0x02
#define VPCeventInverter			0x04
#define VPCeventNovo				0x08
#define VPCeventBrightness			0x10
#define VPCeventTouchPad			0x20
#define VPCeventDisplay				0x40
#define VPCeventCamera				0x80

#define VPCeventODDEject			0x01
#define VPCeventAllRF				0x02
#define VPCeventCameraUtility		0x04
#define VPCeventNormalWide			0x08
#define VPCeventEQ 					0x10
#define VPCeventRFAP				0x20
#define VPCeventGCPUAP				0x40
#define VPCeventODD					0x80

#define VPCcmdQuery					0x10
#define VPCcmdRdBrightnessMax		0x11
#define VPCcmdRdBrightness			0x12
#define VPCcmdWrBrightness			0x13
#define VPCcmdRdWlanStatus			0x14
#define VPCcmdWrWlanStatus			0x15
#define VPCcmdRdBluetoothStatus		0x16
#define VPCcmdWrBluetoothStatus		0x17
#define VPCcmdRdInverterStatus		0x18
#define VPCcmdRdNovoStatus			0x19
#define VPCcmdQuery2				0x1A
#define VPCcmdRdTouchPadStatus		0x1B
#define VPCcmdWrTouchPadStatus		0x1C
#define VPCcmdRdCameraStatus		0x1D
#define VPCcmdWrCameraStatus		0x1E
#define VPCcmdRd3GStatus			0x1F
#define VPCcmdWr3GStatus			0x20
#define VPCcmdRdODDStatus			0x21
#define VPCcmdWrDustModeStatus		0x22
#define VPCcmdRdAllRFStatus			0x23
#define VPCcmdWrAllRFStatus			0x24
#define VPCcmdRdCameraUtilityStatus	0x25
#define VPCcmdWrCameraUtilityStatus	0x26
#define VPCcmdRdEQBTNStatus			0x27
#define VPCcmdRdGCPUstatus			0x29
#define	VPCcmdYogaModeControl		0x2A
#define VPCcmdRdDustModeStatus		0x2B

#define VPCcmdSetSuperPerformance	0x28
#define VPCcmdRdSuperPerformance	0x2D

#define VPCcmdRdGenenalstatus		0x2F
#define VPCcmdRdOnekeyStatus		0x31
#define VPCcmdWrQuietStatus			0x32
#define VPCcmdWrBacklightStatus 	0x33
#define VPCcmdRdBatteryACStatus 	0x38  //Add VPC 0x38 command.
//ANGELAS064:add start
#define    DiagReadECReg        0x11
#define    DiagWriteECReg       0x12 
#define    DiagReadECRAM        0x13 
#define    DiagWriteECRAM       0x14 

#define    DiagSMBusOperate     0x20   

#define    DiagSimulateFnKey       0x21 
#define    DiagSimulateQEvent      0x22  //MEILING013:add.
#define    DiagReadTemp            0x26 
#define    DiagReadFanSpeed        0x27 
#define    DiagWriteFanSpeed       0x28  
#define    DiagReadThermalTable    0x29 
#define    DiagWriteThermalTable   0x2A 
#define    DiagRecoverThermalTable 0x2B 

#define    DiagReadECSpace       0x80 
#define    DiagWriteECSpace      0x81 
#define    ClearAllDiagRam       0xFF    
extern void Diag_interface(void);
extern void TPInfor_ToVPC(void);  //ANGELAG008: add
//ANGELAS064:add end
extern void VPC_interface(void);
extern void RamDebug(unsigned char dbgcode);
extern void ThermalMailBox(void);
#endif