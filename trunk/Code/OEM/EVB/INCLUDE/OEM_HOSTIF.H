/*-----------------------------------------------------------------------------
 * TITLE: OEM_HOSTIF.H
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef OEM_HOSTIF_H
#define OEM_HOSTIF_H

extern void Data_To_PM1(BYTE data_byte);
extern void PM1_DataPending(BYTE nPending);
extern BYTE GetPM1_PendingData(void);
extern void Data_To_PM2(BYTE data_byte);
extern void PM2_DataPending(BYTE nPending);
extern BYTE GetPM2_PendingData(void);
extern void InitSio(void);
extern void InitSysMemory2ECRam(void);

#endif