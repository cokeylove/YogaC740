#ifndef OEM_CYPRESS_H
#define OEM_CYPRESS_H
#define Cypress_Addr                               0x10                                   // Cypress pd address
typedef struct tI2cControlReg
{
	XBYTE *bCtrl;
	XBYTE *bCtrl2;
	XBYTE *bSlaveAddr;
	XBYTE *bStatus;
	XBYTE *bI2cdata;
} tsI2cControlReg;

extern const tsI2cControlReg code conI2cControlReg[];





extern BYTE CheckI2CStatus(BYTE Channel);
extern BYTE bI2cMasterWrite(BYTE bChannel, BYTE bSlaveAddress,BYTE bDataLength, BYTE Offset, XBYTE *Var);
extern BYTE bI2cMasterRead(BYTE bChannel, BYTE bSlaveAddress, BYTE bDataLength, XBYTE *Var);
extern BYTE bI2cMasterWriteToRead(BYTE bChannel, BYTE bSlaveAddress, BYTE Offset,BYTE Offset1,BYTE bDataLength, XBYTE *Var);



#endif
