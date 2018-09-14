/*-----------------------------------------------------------------------------
 * TITLE: OEM_CYPRESS.C
 *
 * Author : Parade Technologies inc.
 *
 * Note : These functions are reference code and LCFC setting code about PS8622.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------
 */
#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>
#if SUPPORT_CYPRESS_PD_CCG4
const tsI2cControlReg code conI2cControlReg[]=
{	
    // bCtrl , bCtrl2   , bSlaveAddr,bStatus , bI2cdata
	{&HOCTL_A, &HOCTL2_A, &TRASLA_A, &HOSTA_A, &HOBDB_A},
	{&HOCTL_B, &HOCTL2_B, &TRASLA_B, &HOSTA_B, &HOBDB_B},
	{&HOCTL_C, &HOCTL2_C, &TRASLA_C, &HOSTA_C, &HOBDB_C}
	//{&HOCTL_D, &HOCTL2_D, &TRASLA_D, &HOSTA_D, &HOBDB_D},
};

//--------------------------------------------
BYTE CheckI2CStatus(BYTE Channel)
{
	BYTE bI2C_ERROR=TRUE;
	BYTE bI2C_status=0x00;


	TR1 = 0;			                    // disable timer1
    ET1 = 0;                  	            // Disable timer1 interrupt
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    TH1 = Timer_26ms>>8;                    // Set timer1 counter 26ms
    TL1 = Timer_26ms;                       // Set timer1 counter 26ms
    TF1 = 0;                  	            // Clear overflow flag
    TR1 = 1;                 	            // Enable timer1

    while(!TF1)                             // Not time out
    {
		bI2C_status = *conI2cControlReg[Channel].bStatus;
		
		if((bI2C_status& 0x7C ) != 0x00 )
		{
			// RamDebug(0xC0);	
			// RamDebug(bI2C_status);
			// RamDebug(HOCTL_A);
			// RamDebug(HOCTL2_A);
			// RamDebug(TRASLA_A);
			
			*conI2cControlReg[Channel].bCtrl |= 0x02;
			_nop_();
			*conI2cControlReg[Channel].bCtrl &= ~0x02;
			*conI2cControlReg[Channel].bCtrl2 &= 0xFD;					// Disable I2C_EN for stop
			*conI2cControlReg[Channel].bStatus |= 0x80;				// Clear byte done
			break;
		}
		else if(( bI2C_status & 0x82 ) != 0x00 )
		{	
			// RamDebug(0xC1);
			// RamDebug(bI2C_status);
			bI2C_ERROR = FALSE;
			break;
		}
	}

	if(TF1)                                 // time-out
    {
        TR1 = 0;			                // disable timer1
 	    TF1 = 0;			                // clear overflow flag
	    ET1 = 1;			                // Enable timer1 interrupt
		bI2C_ERROR = FALSE;
    }

	return bI2C_ERROR;
}

//================================================================
// Name: bI2cMasterWrite
// Input:	bChannel = 0-3
//			bSlaveAddress = Slave address
//			bDataLength = Max 256
//			*Var = Data buffer
// Return: 0 = Fail, 1 = Correct Pass
//===============================================================
BYTE bI2cMasterWrite(BYTE bChannel, BYTE bSlaveAddress,BYTE bDataLength, BYTE Offset, XBYTE *Var)
{
	BYTE bIndex, bFinishFlag;

	bFinishFlag = 1;
	bIndex = 0;
	*conI2cControlReg[bChannel].bCtrl2 = 0x03;							// Enable SMBus and compatible I2C
	*conI2cControlReg[bChannel].bSlaveAddr = (bSlaveAddress&0xFE);		// Set slave address
	*conI2cControlReg[bChannel].bStatus |= 0xFE;						// Clear error flag
	*conI2cControlReg[bChannel].bI2cdata = Offset;						// Set first byte
	*conI2cControlReg[bChannel].bCtrl = 0x5C;							// Start send (None interrupt)

	do {
		bDataLength--;
	    if (CheckI2CStatus( bChannel )) 
		{
     	    bFinishFlag = 0;
			break;
		}
		else if (bDataLength) {		//*** Send data ***//
			*conI2cControlReg[bChannel].bI2cdata = *(Var+bIndex);		// Next byte
			*conI2cControlReg[bChannel].bStatus |= 0x80;				// Clear byte done
			bIndex++;
			
		}
	}
	while (bDataLength);

	*conI2cControlReg[bChannel].bCtrl2 &= 0xF5;						// Disable I2C_EN for stop
	*conI2cControlReg[bChannel].bStatus |= 0x80;					// Clear byte done
	return bFinishFlag;
}

//======================================================
// Name: bI2cMasterRead
// Input:	bChannel = 0-3
//			bSlaveAddress = Slave address
//			bDataLength = Max 256
//			*Var = Data buffer
// Return: 0 = Fail, 1 = Correct Pass
//======================================================
BYTE bI2cMasterRead(BYTE bChannel, BYTE bSlaveAddress, BYTE bDataLength, XBYTE *Var)
{
	BYTE	bIndex, bFinishFlag;

	bFinishFlag = 1;
	bIndex = 0;
	*conI2cControlReg[bChannel].bCtrl2 = 0x03;							// Enable SMBus and compatible I2C
	*conI2cControlReg[bChannel].bSlaveAddr = (bSlaveAddress|0x01);		// Set slave address
	*conI2cControlReg[bChannel].bStatus |= 0xFE;						// Clear error flag

	if (bDataLength == 1) {
		*conI2cControlReg[bChannel].bCtrl |= 0x20;						// Set last byte
	}

	*conI2cControlReg[bChannel].bCtrl = 0x5C;							// Start send (None interrupt)
	do {
		bDataLength--;
       	if (CheckI2CStatus( bChannel )) 
		{		
			bFinishFlag = 0;
			break;
		}

		*(Var+bIndex) = *conI2cControlReg[bChannel].bI2cdata;			// Store data
		bIndex++;

		if (bDataLength == 1) {
			*conI2cControlReg[bChannel].bCtrl |= 0x20;					// Set last byte
		}
		else if (bDataLength == 0) {
			*conI2cControlReg[bChannel].bCtrl2 &= 0xFD;				// Disable I2C_EN for stop
		}
		*conI2cControlReg[bChannel].bStatus |= 0x80;					// Clear byte done
	} while (bDataLength);

	return bFinishFlag;
}

//======================================================
// Name: bI2cMasterWriteToRead
// Input:	bChannel = 0-3
//			bSlaveAddress = Slave address
//			Offset = Offset
//			bDataLength = Max 256
//			*Var = Data buffer
// Return: 0 = Fail, 1 = Correct Pass
// Descript: Start - Slave address - Offset - Restart - Slave address - Data0 .. DataN - Stop
//======================================================
BYTE bI2cMasterWriteToRead(BYTE bChannel, BYTE bSlaveAddress, BYTE Offset,BYTE Offset1,BYTE bDataLength, XBYTE *Var)
{
	BYTE	bIndex, bFinishFlag;
	//BYTE	i;

	bFinishFlag = 1;
	bIndex = 0;
	*conI2cControlReg[bChannel].bCtrl2 = 0x03;							// Enable SMBus and compatible I2C
	*conI2cControlReg[bChannel].bSlaveAddr = (bSlaveAddress&0xFE);	// Set slave address
	*conI2cControlReg[bChannel].bStatus |= 0xFE;						// Clear error flag

	//*** Write offset ***//
	*conI2cControlReg[bChannel].bI2cdata = Offset;					// Set first byte
	*conI2cControlReg[bChannel].bCtrl = 0x5C;						// Start send (None interrupt)

	if (CheckI2CStatus( bChannel )) 
	{
		bFinishFlag = 0;
		// RamDebug(0xB0);
		return bFinishFlag;
	}
	
	//	*conI2cControlReg[bChannel].bStatus |= 0x80;					// Clear byte done

	//*** Write offset ***//
    *conI2cControlReg[bChannel].bI2cdata = Offset1;					// Set first byte
    *conI2cControlReg[bChannel].bStatus |= 0x80;					// Clear byte done
//	*conI2cControlReg[bChannel].bCtrl = 0x5C;						// Start send (None interrupt)
	if (CheckI2CStatus( bChannel )) 
	{
		// RamDebug(0xB1);
		bFinishFlag = 0;
		return bFinishFlag;
	}
	
	//*** Wait direction switch ***//
	*conI2cControlReg[bChannel].bCtrl2 |= 0x0C;						// Set I2C direction switch wait
	*conI2cControlReg[bChannel].bStatus |= 0x80;					// Clear byte done
	*conI2cControlReg[bChannel].bCtrl2 &= 0xFB;						// Release I2C switch wait
/*
	if (CheckI2CStatus( bChannel )) 
	{
		// RamDebug(0xB1);
		bFinishFlag = 0;
		return bFinishFlag;
	}
*/
	if (bDataLength == 1) {
		*conI2cControlReg[bChannel].bCtrl |= 0x20;						// Set last byte
	}

	*conI2cControlReg[bChannel].bStatus |= 0x80;						// Clear byte done
	
	do {
		bDataLength--;
       	if (CheckI2CStatus( bChannel )) 
		{		
			bFinishFlag = 0;
			break;
		}

		*(Var+bIndex) = *conI2cControlReg[bChannel].bI2cdata;			// Store data
		bIndex++;

		if (bDataLength == 1) {
			*conI2cControlReg[bChannel].bCtrl |= 0x20;					// Set last byte
		}
		else if (bDataLength == 0) {
			*conI2cControlReg[bChannel].bCtrl2 &= 0xF5;				// Disable I2C_EN for stop
		}
		*conI2cControlReg[bChannel].bStatus |= 0x80;					// Clear byte done
	} while (bDataLength);

	
	// *conI2cControlReg[bChannel].bCtrl2 &= 0xFD;						// Disable I2C_EN for stop
	// *conI2cControlReg[bChannel].bStatus |= 0x80;					// Clear byte done
	return bFinishFlag;
}
#endif
