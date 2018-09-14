/*-----------------------------------------------------------------------------
 * TITLE: OEM_PECI.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

// Hank 0104 +
//-----------------------------------------------------------------------
// Enable PECI interface function
//-----------------------------------------------------------------------
void Init_PECI(void)
{
    PECI_ALT;			// Set PECI pin to ALT.
    GCR2 |= PECIE;      // Enable PECI
}
//-----------------------------------------------------------------------
// Init timer1 for interface time-out
//-----------------------------------------------------------------------
void PECI_InitTimeOutTimer(void)
{
    TR1 = 0;                 	// Disable timer1
    ET1 = 0;                  	// Disable timer1 interrupt
   	// TH1 = Timer_2ms>>8;        // Set timer1 counter 30ms//JERRYB0047£º
   	// TL1 = Timer_2ms;           // Set timer1 counter 30ms//JERRYB0047£º
    TH1 = Timer_30ms>>8;        // Set timer1 counter 30ms//:+modify PECI time out timer
    TL1 = Timer_30ms;           // Set timer1 counter 30ms//£º+Modify PECI time out timer
    TF1 = 0;                  	// Clear overflow flag
    TR1 = 1;                 	// Enable timer1
}
//-----------------------------------------------------------------------
// Enables the PECI host controller.
//-----------------------------------------------------------------------
void PECI_HostEnable(void)
{
    HOCTLR |= (FIFOCLR+FCSERR_ABT+PECIHEN+CONCTRL);
}
//-----------------------------------------------------------------------
// Check PECI host busy
//-----------------------------------------------------------------------
BYTE PECI_CheckHostBusy(void)
{
	PECI_InitTimeOutTimer();

	while (!TF1) 
	{
		if(IS_MASK_CLEAR(HOSTAR, HOBY))
		{
			TR1=0;
			TF1=0;
			ET1=1;
			PECI_TIMEOUT=0x01;	// set timeout flag
			break;
		}
	}

	if(TF1) 
	{
		TR1=0;
		TF1=0;
		ET1=1;
		PECI_TIMEOUT=0x00;	// clear timeout flag
		return(0);
	}
	return(1);
}
//-----------------------------------------------------------------------
// The PECI host controller will perform the desired transaction.
//-----------------------------------------------------------------------
void PECI_HostControl(BYTE control)
{
    HOCTLR |= control;
}
//-----------------------------------------------------------------------
// Check PECI host Finish
//-----------------------------------------------------------------------
BYTE PECI_CheckHostFinish(void)
{
	BYTE error;
	BYTE status,result;
	error = 0x00;

	PECI_InitTimeOutTimer();
    while (!TF1)
	{
        status = HOSTAR;

        if(status!=0x00)
        {
            if(IS_MASK_SET(status, FINISH))
            {
                TR1=0;
                TF1=0;
                error = 0x00;
                break;
            }
            else if(IS_MASK_SET(status, RD_FCS_ERR))
            {
                error = 0x01;
                break;
            }
            else if(IS_MASK_SET(status, WR_FCS_ERR))
            {
                error = 0x01;
                break;
            }
            else if(IS_MASK_SET(status, EXTERR))
            {
                error = 0x02;
                break;
            }
            else if(IS_MASK_SET(status, BUSERR))
            {
                error = 0x02;

                break;
            }
            else if(IS_MASK_SET(status, TEMPERR))
            {
                error = 0x03;

                while(1)
                break;
            }
        }
    }

	if((TF1)||(error!=0x00))
	{
		if(error == 0x02)           		// EXTERR and BUSERR error
		{
			SET_MASK(RSTC4,BIT4);   		// Reset PECI interface
		}
		result=0x00;    					// error
	}
	else
	{
		result=0x01;    					// Finish
	}

	TR1=0;
	TF1=0;
	ET1=1;
	return(result);
}
//-----------------------------------------------------------------------
// The function of clearing Host Status Register
//-----------------------------------------------------------------------
void ResetPECIStatus(void)
{
    HOSTAR = 0xFE;
}
//-----------------------------------------------------------------------
// Disable the PECI host controller.
//-----------------------------------------------------------------------
void PECI_HostDisable(void)
{
    HOCTLR = 0x00;
}
//-----------------------------------------------------------------------
// Process all PECI command
//  return :
//			0x00 : done
//			0xFF : error
//  Input :
//			(1) address : The address of processor
//			(2) WriteLen : write length
//			(3) ReadLen : read length
//			(4) CMD : PECI command
//			(5) *WriteBuf : buffer point of data write to PECI
//				format : 1) Host ID[7:1]&Retry[0]
//						 2) index
//						 3) LSB
//						 4) MSB
//						 5) Data
//			(6) *ReadBuf : buffer point for read back data
//-----------------------------------------------------------------------
BYTE PECI_CMD(BYTE address, BYTE WL, BYTE RL, BYTE CMD, BYTE *WriteBuf, BYTE *ReadBuf)
{
	BYTE done;

	done = 0xFF;
	PECI_HostEnable();
	HOTRADDR = address;
	HOWRLR = WL;
	HORDLR = RL;
	HOCMDR = CMD;

	for(ITempB01=0; ITempB01<WL; ITempB01++) 
	{
		HOWRDR = *(WriteBuf + ITempB01);
	}

    if(PECI_CheckHostBusy())
	{
	    if (CMD == 0xA5)
        	PECI_HostControl(0x03);
		else
        	PECI_HostControl(START);
        if(PECI_CheckHostFinish())
		{
        	done = 0x00;
            for(ITempB01=0x00;ITempB01<HORDLR;ITempB01++) 
			{
				*(ReadBuf+ITempB01)=HORDDR;
            }
        }
    }
	ResetPECIStatus();
    PECI_HostDisable();

	return(done);
}

// ----------------------------------------------------------------------------
// The function of PECI3.0 Read the Maximum Allowed Processor Temperature
// ----------------------------------------------------------------------------
void PECI_ReadCPUTJMAX(void)
{

    *(&PECIBuffer+0x00)=0x00;				// Host ID
    *(&PECIBuffer+0x01)=PECI_Index_TTR;		// Index
    *(&PECIBuffer+0x02)=0x00;				// LSB
    *(&PECIBuffer+0x03)=0x00;				// MSB
    if( PECI_CMD(PECI_CPU_ADDR, 0x05, 0x05, PECI_CMD_RdPkgConfig, &PECIBuffer, &PECIBuffer) == 0x00 )
	{
       	TJMAX = *(&PECIBuffer+0x03);		// get CPU Tj max from buffer
		//if(TJMAX == 0x00)
		//	RamDebug(0xBB);
		PECI_Err_CNT = 0;	// Clear error count.
		CLR_MASK(ERR_THMSTS, b3PECITempEr);		// Clear bit3 CPU read temperature error.
    }
	else
    {
    	//Optimize read CPU temperature function.
    	SET_MASK(ERR_THMSTS, b3PECITempEr);
    	//RamDebug(0xCC);
    	/*
    		// When PECI fail,BIOS will trigger SMI to read DTS.
    		ECQEvent(DTS_READ_CPU_TEMP);	
		TEMP_Buff_3 = TEMP_Buff_2;
		TEMP_Buff_2 = TEMP_Buff_1;
		TEMP_Buff_1 = nCpuTemp;
		TEMP_Buff_3 = (TEMP_Buff_1 + TEMP_Buff_2 + TEMP_Buff_3)/3;	// CPU average temperature.
		*/
		//Optimize read CPU temperature function.
		/*
		if ( PECI_Err_CNT == 0 )		// 3 Sec.
		{
			PECI_Err_CNT++;
			Buffer_Key(0x11);	 //ALT 	
			Buffer_Key(0x0C);	// F4 

			Buffer_Key(0xF0);	
			Buffer_Key(0x11);	
			Buffer_Key(0xF0);	
			Buffer_Key(0x0C);
		}*/
    	/*
		PECI_Err_CNT++;	// increase read fail count
		if( IS_MASK_CLEAR(ERR_THMSTS, b3PECITempEr) )// Check bit3 is Error.
		{
			if ( PECI_Err_CNT > 7 )		// 3 Sec.
			{
				SET_MASK(ERR_THMSTS, b3PECITempEr);	// Set bit3 CPU read temperature error.
			}
		}
		else
		{
			if ( PECI_Err_CNT > 68 )		// 30 Sec.
			{
				#if !EN_PwrSeqTest
				//SET_MASK(SysStatus,ERR_ShuntDownFlag);
				//ProcessSID(CPUCOMMFAIL_ID);	// 0x22 shutdown ID.
				PECI_Err_CNT = 0;			// Clear error count.
				//RSMRST_LOW();
				//Delay1MS(1);
				//RSMRST_HI();
				#endif	// EN_PwrSeqTest
			}
		}*///When PECI fail,BIOS will trigger SMI to read DTS.
	}
}
// ----------------------------------------------------------------------------
// The function of PECI3.0 CPU temperature read
// ----------------------------------------------------------------------------
void PECI_ReadCPUTemp(void)
{
    WORD cputemp;
    BYTE tempoffset;

    if( PECI_CMD(PECI_CPU_ADDR, 0x01, 0x02, PECI_CMD_GetTemp, &PECIBuffer,&PECIBuffer) == 0x00 ) // Read OK
    {
    	PECI_GET_TEMPL = *(&PECIBuffer+0x00);
    	PECI_GET_TEMPH = *(&PECIBuffer+0x01);
        cputemp = ((*(&PECIBuffer+0x01))<<8)+(*(&PECIBuffer+0x00));	// MSB+LSB
        cputemp = (~cputemp)+1;					// 2's complement
        tempoffset = cputemp>>6;				// 1/64 degrees centigrade
        if(tempoffset <= TJMAX)					// PECI thermal reading temperature readings are
        {										// not reliable at temperatures above Tjmax
            //nCpuTemp = TJMAX - tempoffset;		// Save temperature
            TEMP_Buff_0= TJMAX - tempoffset;//72JERRY057:Update fan table follow'720S_13IKB fan table_V03'.
        }
		else
		{	// Over Tjmax.
			//nCpuTemp = TJMAX;					// The same as TjMax.

			TEMP_Buff_0 = TJMAX;					// The same as TjMax.//72JERRY057:Update fan table follow'720S_13IKB fan table_V03'.
			//RamDebug(0xBB);
		}
		PECI_Err_CNT = 0;	// Clear error count.
		CLR_MASK(ERR_THMSTS, b3PECITempEr);		// Clear bit3 CPU read temperature error.    

		if ( IS_MASK_CLEAR(Fan_Debug_Temp,b0CPU_Temp) )	// Debug CPU Temperature, Engineer myself control.
		{
			if((TEMP_Buff_0&0x80)==0x80)//Filter the value of the CPU temperature greater than 128//optimize G69//72JERRY057:Update fan table follow'720S_13IKB fan table_V03'.
        	{
         		return;
        	}  
        	else
			{
				TEMP_Buff_10 = TEMP_Buff_9;
				TEMP_Buff_9 = TEMP_Buff_8;
				TEMP_Buff_8 = TEMP_Buff_7;
				TEMP_Buff_7 = TEMP_Buff_6;
				TEMP_Buff_6 = TEMP_Buff_5;
				TEMP_Buff_5 = TEMP_Buff_4;
				TEMP_Buff_4 = TEMP_Buff_3;
				TEMP_Buff_3 = TEMP_Buff_2;
				TEMP_Buff_2 = TEMP_Buff_1;
				TEMP_Buff_1 = TEMP_Buff_0;
				TEMP_Buff_3 = (TEMP_Buff_1 + TEMP_Buff_2 + TEMP_Buff_3+ TEMP_Buff_4+ TEMP_Buff_5+ TEMP_Buff_6+ TEMP_Buff_7+ TEMP_Buff_8+ TEMP_Buff_9+ TEMP_Buff_10)/10;	// CPU average temperature.
				nCpuTemp = TEMP_Buff_3;
//72JERRY057:Update fan table follow'720S_13IKB fan table_V03'.
        	}
		}
    }
	else
	{
		//Optimize read CPU temperature function.
		SET_MASK(ERR_THMSTS, b3PECITempEr);
    	//RamDebug(0xAA);
    	/*
    		//When PECI fail,BIOS will trigger SMI to read DTS.
    		ECQEvent(DTS_READ_CPU_TEMP);	
		TEMP_Buff_3 = TEMP_Buff_2;
		TEMP_Buff_2 = TEMP_Buff_1;
		TEMP_Buff_1 = nCpuTemp;
		TEMP_Buff_3 = (TEMP_Buff_1 + TEMP_Buff_2 + TEMP_Buff_3)/3;	// CPU average temperature.
		*/
			
		//Optimize read CPU temperature function.		
		/*
		PECI_Err_CNT++;	// increase read fail count
		if( IS_MASK_CLEAR(ERR_THMSTS, b3PECITempEr) )// Check bit3 is Error.
		{
			if ( PECI_Err_CNT > 3 )		// 3 Sec.
			{
				SET_MASK(ERR_THMSTS, b3PECITempEr);	// Set bit3 CPU read temperature error.
				Buffer_Key(0x11);	 //ALT 	
			Buffer_Key(0x0C);	// F4 

			Buffer_Key(0xF0);	
				Buffer_Key(0x11);	
				Buffer_Key(0xF0);	
				Buffer_Key(0x0C);
			}
		}
		else
		{
			if ( PECI_Err_CNT > 34 )		// 30 Sec.
			{
				#if !EN_PwrSeqTest
				SET_MASK(SysStatus,ERR_ShuntDownFlag);
				ProcessSID(CPUCOMMFAIL_ID);	// 0x22 shutdown ID.
				PECI_Err_CNT = 0;			// Clear error count.
				//RSMRST_LOW();
				//Delay1MS(1);
				//RSMRST_HI();
				#endif	// EN_PwrSeqTest
			}
		}*///// When PECI fail,BIOS will trigger SMI to read DTS.
	}
}

// ----------------------------------------------------------------------------
// The function of setting PL2 watts
// ----------------------------------------------------------------------------
void PECI_SETPL2Watts(BYTE watts)
{
	//T034-	if( SystemNotS0 ) 
    if( SystemNotS0 )  //T034A+
        return;

	//Bit3~Bit5		0x01:XE(55w), 0x02:QuadCore(45w), 0x03:DualCore(35w), 0x04: UL(25w), 0x00: ULV(17w)
	if( (CPU_TYPE & 0x38) == 0x08 )
	{         // XE
		cPacketPowerLimit1 = 55;
	}
	else if( (CPU_TYPE & 0x38) == 0x10 )
	{         // QuadCore
		cPacketPowerLimit1 = 45;
	}
	else if( (CPU_TYPE & 0x38) == 0x18 )
	{         // DualCore
		cPacketPowerLimit1 = 35;
	}
	else if( (CPU_TYPE & 0x38) == 0x20 )
	{         // UL
		cPacketPowerLimit1 = 25;
	}
	else
	{
		cPacketPowerLimit1 = 35;
	}

	if( watts == 0x01 )
	{
		cWriteCurrentPL2 = (cPacketPowerLimit1 * 8)|0x8000;
	}
	else if( watts == 0x02 )
	{
		cWriteCurrentPL2 = (cPacketPowerLimit1 * 10)|0x8000;
	}
	else
	{
		return;
	}


      *(&PECIBuffer+0x00)=0x00;
      *(&PECIBuffer+0x01)=PECI_Index_PPL2;
      *(&PECIBuffer+0x02)=0x00;
      *(&PECIBuffer+0x03)=0x00;
      *(&PECIBuffer+0x04)=(BYTE)cWriteCurrentPL2;
      *(&PECIBuffer+0x05)=(BYTE)(cWriteCurrentPL2>>8);
      *(&PECIBuffer+0x06)=0;
      *(&PECIBuffer+0x07)=0;

	if( PECI_CMD(PECI_CPU_ADDR, 0x0A, 0x01, PECI_CMD_WrPkgConfig, &PECIBuffer,&PECIBuffer) == 0x00 )
	{
		/*
		PECI_CMD(PECI_CPU_ADDR, 0x05, 0x05, PECI_CMD_RdPkgConfig, &PECIBuffer,&PECIBuffer);

		if( (*(&PECIBuffer+0x01) == cWriteCurrentPL2) &&
			(*(&PECIBuffer+0x02) == (cWriteCurrentPL2 >> 8)))
		{
			return;
		}
		*/
		return;
	}
	PECI_ERROR++;

	/*
    	*(&PECIBuffer+0x00)=0x00;
    	*(&PECIBuffer+0x01)=PECI_Index_PPL2;
    	*(&PECIBuffer+0x02)=0x00;
    	*(&PECIBuffer+0x03)=0x00;
	*(&PECIBuffer+0x04)= cWriteCurrentPL2;
	*(&PECIBuffer+0x05)=(cWriteCurrentPL2 >> 8);
	*(&PECIBuffer+0x06)=0;
	*(&PECIBuffer+0x07)=0;
	PECI_CMD(PECI_CPU_ADDR, 0x0A, 0x01, PECI_CMD_WrPkgConfig, &PECIBuffer,&PECIBuffer);
	*/
}
//Optimize read CPU temperature function.
void DTS_ReadCPUTemp(void)
{
	ECQEvent(DTS_READ_CPU_TEMP);	
	/*TEMP_Buff_3 = TEMP_Buff_2;
	TEMP_Buff_2 = TEMP_Buff_1;
	TEMP_Buff_1 = nCpuTemp;
	TEMP_Buff_3 = (TEMP_Buff_1 + TEMP_Buff_2 + TEMP_Buff_3)/3;*/	// CPU average temperature.
	TEMP_Buff_10 = TEMP_Buff_9;
	TEMP_Buff_9 = TEMP_Buff_8;
	TEMP_Buff_8 = TEMP_Buff_7;
	TEMP_Buff_7 = TEMP_Buff_6;
	TEMP_Buff_6 = TEMP_Buff_5;
	TEMP_Buff_5 = TEMP_Buff_4;
	TEMP_Buff_4 = TEMP_Buff_3;
	TEMP_Buff_3 = TEMP_Buff_2;
	TEMP_Buff_2 = TEMP_Buff_1;
	TEMP_Buff_1 = TEMP_Buff_0;
	TEMP_Buff_3 = (TEMP_Buff_1 + TEMP_Buff_2 + TEMP_Buff_3+ TEMP_Buff_4+ TEMP_Buff_5+ TEMP_Buff_6+ TEMP_Buff_7+ TEMP_Buff_8+ TEMP_Buff_9+ TEMP_Buff_10)/10;	// CPU average temperature.
	nCpuTemp = TEMP_Buff_3;
//72JERRY057:Update fan table follow'720S_13IKB fan table_V03'.
}
//Optimize read CPU temperature function.