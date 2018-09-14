/*-----------------------------------------------------------------------------
 * TITLE: OEM_6064.H
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef OEM_6064_H
#define OEM_6064_H

extern void Hook_60Port(BYTE KBHIData);
extern void Hook_64Port(BYTE KBHICmd);
extern void Hook_Keyboard_Cmd(BYTE kbcmd);
extern void Hook_Keyboard_Cmd_Parameter(BYTE kbcmdp);
extern void Hook_Mouse_D4Cmd(BYTE mscmd);
extern void Hook_Mouse_90Cmd(BYTE mscmd);
extern void Hook_Mouse_91Cmd(BYTE mscmd);
extern void Hook_Mouse_92Cmd(BYTE mscmd);
extern void Hook_Mouse_93Cmd(BYTE mscmd);

extern void Cmd_40(BYTE sCount);
extern void Cmd_41_A0(BYTE nPort);
extern void Cmd_41_A1(BYTE nPort);
extern void Cmd_41_A2(BYTE nPort);
extern void Cmd_41_A3(BYTE nPort);
extern void Cmd_42(BYTE eFlashBank);
extern void Cmd_43(BYTE nPort,BYTE nData);
extern void Cmd_44(BYTE nPort);
extern void Cmd_45(BYTE nPort, BYTE sCount);
extern void Cmd_46(BYTE nPort,BYTE nData);
extern void Cmd_47(BYTE nPort, BYTE nData);
extern void Cmd_49(BYTE nPort);//HEGANGS021:one key wake
extern void Cmd_4A(BYTE nPort, BYTE nData);//MEILING009:Add 8 sec reset test function.
extern void Cmd_4E(BYTE nPort, BYTE nOffSet);
extern void Cmd_51(BYTE nPort);
extern void Cmd_52_A0(BYTE nPort);
extern void Cmd_52_A5(BYTE nPort);
extern void Cmd_52_A3(BYTE nPort);
extern void Cmd_52_A4(BYTE nPort);
extern void Cmd_52_A6(BYTE nPort); //LMLKBL0018:add.
extern void Cmd_52_A7(BYTE nPort); //LMLKBL0018:add.
extern void Cmd_53(BYTE nPort,BYTE nDataH,BYTE nDataM,BYTE nDataL);
extern void Cmd_55(BYTE nData);//hang check
extern void Cmd_58(BYTE sCount);
extern void Cmd_59(BYTE nPort,BYTE nData, BYTE nData2);
extern void Cmd_5C(BYTE nPort);
extern void Cmd_5D(BYTE sCount);
extern void Cmd_71(BYTE nData);
extern void Cmd_72(BYTE nData);
extern void Cmd_75(void); //ANGELAG012:add.
extern void Cmd_7A(BYTE nPort,BYTE nData);  //ANGELAS033:Add
extern void Cmd_7B(BYTE nPort,BYTE nDataH,BYTE nDataL);  //ANGELAS033:Add
extern void Cmd_7E(BYTE nPort,XBYTE nDataH,XBYTE nDataL); //MEILING031: add .
extern void Cmd_97(BYTE nData);
extern void Cmd_B0(BYTE nPort,BYTE nData);
//start for save P80 code to setup. 
extern void Cmd_B1(BYTE nPort,BYTE nData);
extern void Cmd_B2(BYTE nPort,BYTE nData);
//end for save P80 code to setup. 
extern void Cmd_B3(BYTE nPort,BYTE nDataH,BYTE nDataL);
extern void Update_EEPROM(void);
extern void Update_EEPROMMark(void);
extern void Update_EEPROMB07(void);
extern void Erase_EEPROMAll(void);
extern void Chk_pDevStus(void);
extern void Chk_KB_Backlight(void);
extern void Chk_UEFIStatus(void);
extern void Clr_UEFIStatus(void);
extern void Update_16ByteEEPROM(void);

extern void Cmd_B4(void);

#define SysAction_Nothing                   0x01
#define SysAction_EC_Shutdown               0x02
#define SysAction_EC_Restart                0x03
#define SysAction_SysShutdown_EC_AutoBoot   0x04

#endif
