/*----------------------------------------------------------------------------
 * Filename: OEM_Ver.H  For Chipset: ITE.IT85XX
 *
 * Function: Project version
 *
 *---------------------------------------------------------------------------*/

#ifndef OEM_Ver_H
#define OEM_Ver_H
// REV0_BYTE0 set as Project ID "0x8D" as BIOS request, REV0_BYTE1 is the EC firmware version, REV0_BYTE3 is the test version, if 0, formal version
#define ProjectID0L		0x02	// Proect ID  (low byte)
#define ProjectID0H		0x00	// Proect ID  (high byte)
#define REV0_BYTE0		0x01	//T098: from 0 to 1 //T096 // Major number from 0 to 7
#define REV0_BYTE1		30	// 2nd version from 1 to 99
#define REV0_BYTE2		'A'		// Type of KB-BIOS from 'A' to 'Z'
#define REV0_BYTE3		0  // Verify byte from 0 to 15, 0 is ROM other is T01-T15
#define REV0_BYTE4		35	// Series number of EC F/W release 

#define CHIP_TYPE0		0x0A	// 8580 - Chip type
#define CUSTOMER_ID0	0x38	// Customer ID[L11072001]
#define tModuName0		"VIQY0" // formodule name

#endif