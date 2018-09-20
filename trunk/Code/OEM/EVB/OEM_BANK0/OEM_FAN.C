/*-----------------------------------------------------------------------------
 * TITLE: OEM_FAN.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

void Fan_Init(void)
{
	nFanStatus1 = Fan_Num;
	nFanStatus1 = (nFanStatus1 << 4) | Fan_Step;
	//nFanStatus1 = ((Fan_Num << 4) &0xF0) | (Fan_Step & 0x0F);
	nOSThrottlingTemp = 98; //MEILING024:change throttling temperatrue from 97 to 98.
	nOSShutdownTemp = 100;
	nShutDownTemp = 100;
}

/*****************************************************************************/
// Procedure: PWM_TimeCount									TimeDiv: 50mSec
// Description: PWM Beep function.
// GPIO:
// Referrals: Service short time PWM output, base on 50ms.
/*****************************************************************************/
void PWM_TimeCount(void)
{
	if (uReserve07.fbit.uACInOutBeep == 0)
	{
		if( PWM_LEDBeep_CNT == 0x00 )
		{
			BEEP_INIT;
			PWM_BEEP = 0x00;
			return;
		}

		BEEP_ALT;
		PWM_LEDBeep_CNT--;		// decrease timer
		// AC IN/OUT beep time
		if( PWM_LEDBeep_CNT == 0x00 )
		{
			BEEP_INIT;
			PWM_BEEP = 0x00;
		}
	}
	else
	{
		PWM_LEDBeep_CNT = 0;
		BEEP_INIT;
		PWM_BEEP = 0x00;
	}
}

/*****************************************************************************/
// Procedure: ThmIC_Temperature								TimeDiv: 500mSec
// Description: Read RAM temperature
// GPIO:
// Referrals:
/*****************************************************************************/
extern void TempProtect_VRAM(void);
void ThmIC_Temperature(void)
{
	BYTE BsData,BTmlCmd;

	if( SystemNotS0 || (PwrOnDly5Sec!=0) )	 //add PwrOnDly5Sec judge.
	{ 
		return;
	}
	switch( TmlICStep & 0x03 )  //  Change TmlICStep from 7 to 3
	{
		case 0: // HEGANGS010:Enable GPU temperature and remove read remote temp
			BTmlCmd = Remote_Temp;
			break;

		case 1:
			BTmlCmd = TmlIC_Temp;
			break;
			
		default:
			TmlICStep = 0;
			return;
	}
	if(!bRWSMBus(SMbusCh4, SMbusRB, TmlIC_Addr, BTmlCmd, &BsData, 0))  
	{      //  1. SMBus fail; 2. MBID OK; 3.  no UMA 37W ; 4.  no UMA 47W VRAM sensor  
		TMErrCnt++;
		TmlICStep++;
		if( IS_MASK_CLEAR(ERR_THMSTS, b0ThmICError) )// Check bit0 is Error.
		{
			if ( TMErrCnt > 7 )		// 3 Sec.
			{
				SET_MASK(ERR_THMSTS, b0ThmICError);	// Set bit0 Thermal IC read temperature error.
			}
		}
		else
		{
			if ( TMErrCnt > 68 )	// 30 Sec.
			{
				TMErrCnt = 0;					// Clear error count.
			}
		}
		ResetSMBus(SMbusCh4);
	}
	else
	{ 
		if(BsData == 0x00)	// read 0'C  
		{
			TMErrCnt++;
			TmlICStep++;
			if( IS_MASK_CLEAR(ERR_THMSTS, b0ThmICError) )// Check bit0 is Error.
			{
				if ( TMErrCnt > 7 )		// 3 Sec.
				{
					SET_MASK(ERR_THMSTS, b0ThmICError);	// Set bit0 Thermal IC read temperature error.
				}
			}
			else
			{
				if ( TMErrCnt > 68 )	// 30 Sec.
				{
					ProcessSID(THERMALCOMMFAIL_ID);	// 0x0B shutdown ID.
					TMErrCnt = 0;					// Clear error count.
				}
			}
		}
		else
		{
			switch( TmlICStep & 0x03 )
			{
				case 0:
					nRamTemp = BsData;	// Save VRAM temperature. 
					TMErrCnt = 00;		// Clear error count.
					CLEAR_MASK(ERR_THMSTS,b0ThmICError);	// Clear bit0.
					TmlICStep++;		// next step 1.
					break;
				case 1:
					nVramTemp = BsData;	// Save SSD temperature. 
					TMErrCnt = 00;		// Clear error count.
					CLEAR_MASK(ERR_THMSTS,b0ThmICError);	// Clear bit0.
					TmlICStep++;		// next step 2.
					break;      //Remove thermal IC read remote 2 temperature
		
				default:
					break;
			}
		}
	}

}
//-----------------------------------------------------------------------------
// Service short time PWM output, base on 50ms.
//-----------------------------------------------------------------------------
/* //ANGELAS050:remove start
void Tml_SMLink(void)
{
	if( !bRSMBusBlock(SMbusCh4, SMbusRBK, 0x96, 0x40, &MaxCPU_MCHTemp))//, 0x14, TRUE))
	{
		TMErrCnt++;
		if( IS_MASK_CLEAR(ERR_THMSTS, b0ThmICError) )// Check bit0 is Error.
		{
			if ( TMErrCnt > 35 )	//change 7 to 35	// 3 Sec.
			{
				SET_MASK(ERR_THMSTS, b0ThmICError);	// Set bit0 Thermal IC read temperature error.
			}
		}
		else
		{
			if ( TMErrCnt > 254 ) //change 68 to 254   	// 30 Sec.
			{
				//ProcessSID(THERMALCOMMFAIL_ID);	// 0x0B shutdown ID.
				TMErrCnt = 0;					// Clear error count.
				//RSMRST_LOW();
				//Delay1MS(1);
				//RSMRST_HI();
			}
		}
		ResetSMBus(SMbusCh4);
	}
	else
	{
		nOSShutdownTemp3 = MaxCPU_MCHTemp;		// Save PCH temperature.
		CLEAR_MASK(ERR_THMSTS,b0ThmICError);	// Clear bit0.
		TMErrCnt = 0;							// Clear error count.
		TmlICStep = 0;							// Reset Step.
	}
}
*///ANGELAS050:remove end
/*****************************************************************************/
// Procedure:  							TimeDiv: 500mSec
// Description:  
// GPIO:  
// Referrals:
/*****************************************************************************/
/* ANGELAS050:remove start
void TempProtect_VRAM(void)
{
		BYTE BsData,BTmlCmd;
		BTmlCmd = SSD_Temp;//change SSD_Temp to SSD_Temp
	


		//if((!bRWSMBus(SMbusCh4, SMbusRB, TmlIC_Addr, BTmlCmd, &BsData, 0))&&MBID_READY&&(!(uMBID&0x20)))
		if(!bRWSMBus(SMbusCh4, SMbusRB, TmlIC_Addr, BTmlCmd, &BsData, 0))// REMOVE MBID_READY judge
		{	 
			TMErrCnt++;
			if( IS_MASK_CLEAR(ERR_THMSTS, b0ThmICError) )// Check bit0 is Error.
			{
				if ( TMErrCnt > 7 ) 	// 3 Sec.
				{
					SET_MASK(ERR_THMSTS, b0ThmICError); // Set bit0 Thermal IC read temperature error.
				}
			}
			else
			{
				if ( TMErrCnt > 68 )	// 30 Sec.
				{
					//ProcessSID(THERMALCOMMFAIL_ID); // 0x0B shutdown ID.
					TMErrCnt = 0;					// Clear error count.
					//RSMRST_LOW();  
					//Delay1MS(1);
					//RSMRST_HI();
				}
			}
			ResetSMBus(SMbusCh4);
		}
		else
		{ 
		if(BsData == 0x00)// REMOVE MBID_READY judge
			//if((BsData == 0x00)&&MBID_READY&&(!(uMBID&0x20)))
			{
				TMErrCnt++;
				if( IS_MASK_CLEAR(ERR_THMSTS, b0ThmICError) )// Check bit0 is Error.
				{
					if ( TMErrCnt > 7 ) 	// 3 Sec.
					{
						SET_MASK(ERR_THMSTS, b0ThmICError); // Set bit0 Thermal IC read temperature error.
					}
				}
				else
				{
					if ( TMErrCnt > 68 )	// 30 Sec.
					{
						//ProcessSID(THERMALCOMMFAIL_ID); // 0x0B shutdown ID.
						TMErrCnt = 0;					// Clear error count.
						  // RSMRST_LOW(); 
						 //  Delay1MS(1);
						 //  RSMRST_HI();	
					}
				}
			}
			else
			{

						//nVramTemp = BsData; // Save VRAM temperature.  
						nNBTemp=BsData;  
						TMErrCnt = 00;		// Clear error count.
						CLEAR_MASK(ERR_THMSTS,b0ThmICError);	// Clear bit0.
						if(nNBTemp>=85) //change nVramTemp to nNBTemp
						{
						  // ProcessSID(ThermalICOVerTEPM_ID); // 0x0B shutdown ID.
						   //RSMRST_LOW(); 
						  // Delay1MS(1);
						  // RSMRST_HI();		
						}
			}
		  }
}

*///ANGELAS050:remove end
/*****************************************************************************/
// Procedure: VGA_Temperature								TimeDiv: 500mSec
// Description: Read VGA temperature(M/B)
// GPIO: GPIOF6(118)
// Referrals:
/*****************************************************************************/
void VGA_Temperature(void)
{
	BYTE BsData;
	//Add read AMD GPU thermal temperature  start
//	BYTE INIT_GPU_REG2[4] = {0x0f, 0x00, 0x01, 0xC5}; //ANGELAG016: remove
//ANGELAG016: add start
	 BYTE INIT_GPU_REG2[4] = {0x0f, 0x00, 0x00, 0x8f};
  	BYTE INIT_GPU_REG1[4] = {0x0f, 0x00, 0x00, 0x8e};
	BYTE INIT_GPU_REG0[4] = {0xc0, 0x30, 0x00, 0x14};
//ANGELAG016: add end
	if ( (CPU_TYPE & 0xC0) == 0x00 )	// Isn't check VGA support optimus?
	{ nHybridGraphicsDIS;}				// No optimus status.
 	else
 	{ nHybridGraphicsEN; }				// optimus status.

	//if( SystemNotS0 || (PwrOnDly5Sec!=0) )	 //T089: add PwrOnDly5Sec judge.

	// Check S0 and  VGA into.
	if( SystemNotS0 || !(nHybridGraphicsGET)|| (PwrOnDly5Sec!=0) ) //== to != // add PwrOnDly5Sec judge.modify nHybridGraphicsGET not take a reverse value 
	{  
        VGAErrCnt = 00;  
        VGA_TEMP=0;
        VGA_TBuff3=0;
        CLEAR_MASK(ERR_THMSTS,b1VGATempEr); 
        return; 
    }
     
	if((CPU_TYPE &0xc0)==0x40)
	{
//ANGELAG016: add start
	if(!bWSMBusBlock(SMbusCh4, SMbusWBK, VGA_Addr, 0x01,( BYTE* )&INIT_GPU_REG1,4,0))//load GPU register offset to SMB_ADDR
		{
		VGAErrCnt++;
		}
 	if(!bWSMBusBlock(SMbusCh4, SMbusWBK, VGA_Addr, 0x02,( BYTE* )&INIT_GPU_REG0,4,0))//sent data to be written into the DGPU rgister
		{
		VGAErrCnt++;
		}

	if(!bWSMBusBlock(SMbusCh4, SMbusWBK, VGA_Addr, 0x01,( BYTE* )&INIT_GPU_REG2,4,0))//load GPU register offset to SMB_ADDR
		{
		VGAErrCnt++;
		}
//ANGELAG016: add end
//ANGELAG016: remove start
	/*	if(!bWSMBusBlock(SMbusCh4, SMbusWBK, VGA_Addr, 0x01, &INIT_GPU_REG2, 4,0))
		{
			VGAErrCnt++;
		}*/
//ANGELAG016: remove end
			
		//vgaok=1;
		
		if(!bRSMBusBlock(SMbusCh4, SMbusRBK, VGA_Addr, 0x03, &sdAMDTH0))
		{
			VGAErrCnt++;
			if( IS_MASK_CLEAR(ERR_THMSTS, b1VGATempEr) )// Check bit1 is Error.
			{
				if ( VGAErrCnt > 35 )	//change 7 to 35  // 3 Sec.
				{
					SET_MASK(ERR_THMSTS, b1VGATempEr);	// Set bit1 Thermal IC read temperature error.
				}
			}
			else
			{
				if ( VGAErrCnt > 254 )		// from 68 to 255	// 30 Sec.
				{
					SET_MASK(SysStatus,ERR_ShuntDownFlag);
					ProcessSID(VGACOMMFAIL_ID);	// 0x20 shutdown ID.
					VGAErrCnt = 00;				// Clear error count.
					RSMRST_LOW();
					Delay1MS(1);
					RSMRST_HI();
				}
			}
			ResetSMBus(SMbusCh4);
		}
		//sdAMDTH0=sdAMDTH0<<24;
		else
		{
			VGA_TEMP=sdAMDTH0&0xff;
			VGAErrCnt = 00;		// Clear error count.
			CLEAR_MASK(ERR_THMSTS,b1VGATempEr);	// Clear bit1,bit5.

			if ( IS_MASK_CLEAR(Fan_Debug_Temp,b1VGA_Temp) )	// Debug VGA Temperature, Engineer myself control.
			{
		    	if((VGA_TEMP&0x80)==0x80)//Filter the value of the GPU temperature greater than 128 // optimize G69.
         		{
         			return;
         		}  
        		else
				{
					VGA_TBuff3 = VGA_TBuff2;
					VGA_TBuff2 = VGA_TBuff1;
					VGA_TBuff1 = VGA_TEMP;
					VGA_TBuff3 = (VGA_TBuff1 + VGA_TBuff2 + VGA_TBuff3)/3;		// VGA average temperature.
            	}
			}
		}
	}
	//Add read AMD GPU thermal temperature end
	if((CPU_TYPE &0xc0)==0x80)
	{
		if(!bRWSMBus(SMbusCh4, SMbusRB, EXTVGA_Addr, 0x00, &BsData, 0))// REMOVE MBID_READY judge

		//if((!bRWSMBus(SMbusCh4, SMbusRB, VGA_Addr, TmlIC_Temp, &BsData, 0))&&MBID_READY&&(!(uMBID&0x20))) //add MBID judge.
		{
        	VGAErrCnt++;
			if( IS_MASK_CLEAR(ERR_THMSTS, b1VGATempEr) )// Check bit1 is Error.
			{
				if ( VGAErrCnt > 35 )	//change 7 to 35  // 3 Sec.
				{
					SET_MASK(ERR_THMSTS, b1VGATempEr);	// Set bit1 Thermal IC read temperature error.
				}
			}
			else
			{
				if ( VGAErrCnt > 254 )		// from 68 to 255	// 30 Sec.
				{
					SET_MASK(SysStatus,ERR_ShuntDownFlag);
					ProcessSID(VGACOMMFAIL_ID);	// 0x20 shutdown ID.
					VGAErrCnt = 00;				// Clear error count.
					RSMRST_LOW();
					Delay1MS(1);
					RSMRST_HI();
				}
			}
			ResetSMBus(SMbusCh4);
		}
		else
		{
			if(BsData == 0x00)	// read 0'C  // REMOVE MBID_READY judge
			//if((BsData == 0x00)&&MBID_READY&&(!(uMBID&0x20)))// read 0'C 
			{
            	VGA_TEMP = BsData;      
				VGAErrCnt++;
				if( IS_MASK_CLEAR(ERR_THMSTS, b1VGATempEr) )// Check bit1 is Error.
				{
					if ( VGAErrCnt > 7 )	// 3 Sec.
					{
						SET_MASK(ERR_THMSTS, b1VGATempEr);	// Set bit1 Thermal IC read temperature error.
					}
				}
				else
				{
					if ( VGAErrCnt > 68 )			// 30 Sec.
					{
						SET_MASK(SysStatus,ERR_ShuntDownFlag);
						ProcessSID(VGACOMMFAIL_ID);	// 0x20 shutdown ID.
						VGAErrCnt = 00;				// Clear error count.
						RSMRST_LOW();
						Delay1MS(1);
						RSMRST_HI();
					}
				}
			}
			else
			{
				VGA_TEMP = BsData;
				VGAErrCnt = 00;		// Clear error count.
				CLEAR_MASK(ERR_THMSTS,b1VGATempEr);	// Clear bit1,bit5.

				if ( IS_MASK_CLEAR(Fan_Debug_Temp,b1VGA_Temp) )	// Debug VGA Temperature, Engineer myself control.
				{
					if((VGA_TEMP&0x80)==0x80)//Filter the value of the GPU temperature greater than 128// optimize G69
                	{
                 		return;
                	}  
           			else
					{
						VGA_TBuff3 = VGA_TBuff2;
						VGA_TBuff2 = VGA_TBuff1;
						VGA_TBuff1 = VGA_TEMP;
						VGA_TBuff3 = (VGA_TBuff1 + VGA_TBuff2 + VGA_TBuff3)/3;		// VGA average temperature.
					}
				}
			}
		}
	}
    
}



/*****************************************************************************/
// Procedure: Oem_Thermal_Control								TimeDiv: 1 Sec
// Description: Read CPU temperature
// GPIO: GPIOF6(118)
// Referrals:
/*****************************************************************************/
void Oem_Thermal_Control(void)
{
	//if(SystemIsS0&& (PwrOnDly5Sec==0)) //add PwrOnDly5Sec judge.
	//if(SystemIsS0&& (PwrOnDly5Sec==0)&&Read_SLPS3() && Read_SLPS4()&&(PECIBIOSTEST==0)) //add PwrOnDly5Sec judge.//owhen enter S3/S4/CB,BIOS sent B4 CMD to EC from 66port for peci fail.// Remove PECI workaround.
	if(SystemIsS0&& (PwrOnDly5Sec==0)&&Read_SLPS3() && Read_SLPS4())// PECI workaround.
	{
		if(TJMAX == 0x00)
		{
			PECI_ReadCPUTJMAX();
			CPU_TYPE = CPU_TYPE & 0xFC; //
			if (TJMAX == 105)			//105
				CPU_TYPE |= 0x03;
			else if (TJMAX == 100)		//100
				CPU_TYPE |= 0x02;
			else if (TJMAX == 90)		//90
				CPU_TYPE |= 0x01;
		}
		else
		{ PECI_ReadCPUTemp(); }
		//Optimize read CPU temperature function.
		if( IS_MASK_SET(ERR_THMSTS, b3PECITempEr) )
		{
			DTS_ReadCPUTemp();
		}
		//0ptimize read CPU temperature function.
	}
}

/*****************************************************************************/
// Procedure: Oem_Fan_Speed										TimeDiv: 1 Sec
// Description: Read Fan RPM
// GPIO: GPIOD6(47)
// Referrals:
/*****************************************************************************/
void Oem_Fan_Speed(void)
{
	WORD Curr_Fan1Tachometer;
	if(SystemIsS0)
	{
		if ( (F1TMRR == 0) && (F1TLRR == 0) )
		{
			nAtmFanSpeed = 0;
		}
		else
		{
			Curr_Fan1Tachometer = (36000/((WORD)(F1TMRR<<8) + F1TLRR))*60;
			nAtmFanSpeed = ( Curr_Fan1Tachometer / 100 );
            FAN1_RPM=Curr_Fan1Tachometer;  
		}
		
	}
}
//72JERRY014¡êoS+ Add the second fan method
void Oem_Fan1_Speed(void)
{
	WORD Curr_Fan2Tachometer;
	if(SystemIsS0)
	{
		if ( (F2TMRR == 0) && (F2TLRR == 0) )
		{
			nAtmFan1Speed = 0;
		}
		else
		{
			Curr_Fan2Tachometer = (36000/((WORD)(F2TMRR<<8) + F2TLRR))*60;
			nAtmFan1Speed = ( Curr_Fan2Tachometer / 100 );
            FAN2_RPM=Curr_Fan2Tachometer;  
		}
		
	}
}

const thermaltable code Fan1Step[]= //fan1 step
{
	{&Fan1On_Step1,	&Fan1Off_Step1,	&Fan1RPM_Step1	},	// Level 0 	
    {&Fan1On_Step2,	&Fan1Off_Step2,	&Fan1RPM_Step2	},	// Level 1 
    {&Fan1On_Step3,	&Fan1Off_Step3,	&Fan1RPM_Step3	},	// Level 2 	
    {&Fan1On_Step4,	&Fan1Off_Step4,	&Fan1RPM_Step4	},	// Level 3 	
    {&Fan1On_Step5,	&Fan1Off_Step5,	&Fan1RPM_Step5	},	// Level 4  
    {&Fan1On_Step6,	&Fan1Off_Step6,	&Fan1RPM_Step6	},	// Level 5  
    {&Fan1On_Step7,	&Fan1Off_Step7,	&Fan1RPM_Step7	},	// Level 6 
};
const thermaltable code Fan2Step[]= //fan2 step
{
    {&Fan2On_Step1,	&Fan2Off_Step1,	&Fan2RPM_Step1	},	// Level 0 	
    {&Fan2On_Step2,	&Fan2Off_Step2,	&Fan2RPM_Step2	},	// Level 1 
    {&Fan2On_Step3,	&Fan2Off_Step3,	&Fan2RPM_Step3	},	// Level 2 	
    {&Fan2On_Step4,	&Fan2Off_Step4,	&Fan2RPM_Step4	},	// Level 3 	
    {&Fan2On_Step5,	&Fan2Off_Step5,	&Fan2RPM_Step5	},	// Level 4  
    {&Fan2On_Step6,	&Fan2Off_Step6,	&Fan2RPM_Step6	},	// Level 5  
    {&Fan2On_Step7,	&Fan2Off_Step7,	&Fan2RPM_Step7	},	// Level 6 
};
const thermal code Fan1Talbe1UMA[]= //UMA  FAN1_PWM 530  table1
{
    {0,	42,	0,	0  },	// Level 0  on off rpm	
    {1,	48,	37,	22 },	// Level 1 
    {2,	56,	43,	31 },	// Level 2 	
    {3,	63,	53,	35 },	// Level 3 	
    {4,	69,	58,	35 },	// Level 4  
    {5, 75,	60,	35 },	// Level 5  
    {6, 85,	64,	35 },	// Level 6 
};
const thermal code Fan1Talbe2UMA[]= //UMA  FAN1_PWM 530  table2
{
	{0,	42,	0,	0	},	// Level 0 	on off rpm	
    {1,	48,	37,	22	},	// Level 1 
    {2,	54,	43,	31	},	// Level 2 	
    {3,	57,	49,	36	},	// Level 3 	
    {4,	63,	53,	40	},	// Level 4  
    {5, 70,	58,	45	},	// Level 5  
    {6, 85,	65,	50	},	// Level 6 
};
const thermal code Fan1Talbe1DIS[]= //DIS 14''15" FAN1_PWM 530 High/balance
{
	{0, 44, 0,  0	},	// Level 0	
	{1, 54, 37, 22	},	// Level 1 
	{2, 59, 45, 27	},	// Level 2	
	{3, 64, 56, 35	},	// Level 3	
	{4, 70, 60, 35	},	// Level 4	
	{5, 78, 68, 35	},	// Level 5	
	{6, 85, 75, 35	},	// Level 6 
};
const thermal code Fan1Talbe2DIS_H[]= //DIS  FAN_PWM 530 table2 14" balance
{
	{0,	42,	0,	0	},	// Level 0 	
    {1,	48,	37,	22	},	// Level 1 
    {2,	53,	43,	27	},	// Level 2 	
    {3,	57,	49,	35	},	// Level 3 	
    {4,	95,	54,	39	},	// Level 4  
    {5, 98,	92,	46	},	// Level 5  
    {6, 99,	96,	51	},	// Level 6 
};
const thermal code Fan1Talbe2DIS_B[]= //DIS  FAN_PWM 530 table 2 15" balance
{
	{0,	42,	0,	0	},	// Level 0 	
    {1,	48,	37,	22	},	// Level 1 
    {2,	54,	43,	27	},	// Level 2 	
    {3,	58,	49,	35	},	// Level 3 	
    {4,	95,	55,	39	},	// Level 4  
    {5, 98,	92,	46	},	// Level 5  
    {6, 99,	96,	51	},	// Level 6 
};
const thermal code Fan2Talbe1DIS[]=  //DIS  FAN_PWM 530  FAN2 TABLE1 14''15'' High/balance
{
	{0, 44, 0,  0	},	// Level 0	
	{1, 54, 37, 22	},	// Level 1 
	{2, 59, 45, 31	},	// Level 2	
	{3, 63, 56, 32	},	// Level 3	
	{4, 69, 58, 32	},	// Level 4	
	{5, 75, 64, 32	},	// Level 5	
	{6, 85, 70, 32	},	// Level 6 
};
const thermal code  Fan2Talbe2DIS_H[]=  //DIS  FAN_PWM 530 14'' fan2 table 2 high
{
    {0,	42,	0,	0	},	// Level 0 	
    {1,	48,	37,	22	},	// Level 1 
    {2,	53,	43,	31	},	// Level 2 	
    {3,	57,	49,	32	},	// Level 3 	
    {4,	61,	54,	36	},	// Level 4  
    {5, 65,	58,	43	},	// Level 5  
    {6, 85,	62,	48	},	// Level 6  
};
const thermal code  Fan2Talbe2DIS_B[]=  //DIS  FAN_PWM 530 FAN 2 15'' table 2 balance
{
    {0,	42,	0,	0	},	// Level 0 	
    {1,	48,	37,	22	},	// Level 1 
    {2,	54,	43,	31	},	// Level 2 	
    {3,	58,	49,	32	},	// Level 3 	
    {4,	61,	55,	36	},	// Level 4  
    {5, 72,	59,	43	},	// Level 5  
    {6, 85,	67,	48	},	// Level 6  
};
const thermal code Fan1Talbe1UMAYOGA[]= //UMA  FAN_PWM YOGA530
{
    {0,	42,	33,	0	},	// Level 0 	
    {1,	47,	38,	26	},	// Level 1 
    {2,	52,	43,	31	},	// Level 2 	
    {3,	63,	48,	35	},	// Level 3 	
    {4,	127,60,	38	},	// Level 4  
    {5, 127,60,	38	},	// Level 5  
    {6, 127,60,	38	},	// Level 6 
};
const thermal code Fan1Talbe2UMAYOGA[]= //UMA  FAN_PWM YOGA530
{
	{0,	40,	33,	0	},	// Level 0 	
    {1,	45,	35,	31	},	// Level 1 
    {2,	50,	40,	35	},	// Level 2 	
    {3,	56, 46,	39	},	// Level 3 	
    {4,	62,	52,	44	},	// Level 4  
    {5, 62, 58,	50	},	// Level 5  
    {6, 127,58,	50	},	// Level 6 
};
const thermal code Fan1Talbe2QUMAYOGA[]= //UMA  FAN_PWM YOGA530
{
	{0,	40,	33,	0	},	// Level 0 	
    {1,	45,	35,	25	},	// Level 1 
    {2,	50,	40,	31	},	// Level 2 	
    {3,	56, 46,	35	},	// Level 3 	
    {4,	62,	52,	42	},	// Level 4  
    {5, 62, 58,	42	},	// Level 5  
    {6, 127,58,	42	},	// Level 6 
};
const thermal code Fan1Talbe1DISYOGA[]= //DIS FAN_PWM YOGA530
{
    {0,	40,	33,	0	},	// Level 0 	
    {1,	44,	37,	24	},	// Level 1 
    {2,	48,	41,	29	},	// Level 2 	
    {3,	127,45,	33	},	// Level 3 	
    {4,	127,60,	33	},	// Level 4  
    {5, 127,60,	33	},	// Level 5  
    {6, 127,60,	33	},	// Level 6 
};
const thermal code Fan1Talbe2DISYOGA[]= //DIS  FAN_PWM YOGA530
{
	{0,	38,	33,	0	},	// Level 0 	
    {1,	42,	35,	29	},	// Level 1 
    {2,	46,	39,	33	},	// Level 2 	
    {3,	50,	43,	36	},	// Level 3 	
    {4,	58,	47,	41	},	// Level 4  
    {5, 68,	52,	48	},	// Level 5  
    {6, 127,62,	51	},	// Level 6 
};

const thermal code Fan1Talbe2QDISYOGA[]= //DIS  FAN_PWM YOGA530
{
	{0,	38,	33,	0	},	// Level 0 	
    {1,	42,	35,	29	},	// Level 1 
    {2,	46,	39,	33	},	// Level 2 	
    {3,	50,	43,	36	},	// Level 3 	
    {4,	58,	47,	40	},	// Level 4  
    {5, 68,	52,	40	},	// Level 5  
    {6, 127,62,	40	},	// Level 6 
};

const thermal code Fan2Talbe1DISYOGA[]=  //DIS  FAN_PWM YOGA530
{
    {0,	40,	33,	0	},	// Level 0 	
    {1, 44,	37,	24	},	// Level 1 
    {2,	48,	41,	28	},	// Level 2 	
    {3,	127,45,	30	},	// Level 3 	
    {4,	127,60,	30	},	// Level 4  
    {5, 127,60,	30	},	// Level 5  
    {6, 127,60,	30	},	// Level 6 
};
const thermal code  Fan2Talbe2DISYOGA[]=  //DIS  FAN_PWM YOGA530
{
    {0,	38,	33,	0	},	// Level 0 	
    {1,	42, 35,	28	},	// Level 1 
    {2,	46,	39,	33	},	// Level 2 	
    {3,	50,	43,	35	},	// Level 3 	
    {4,	58,	47,	38	},	// Level 4  
    {5, 68,	52,	45	},	// Level 5  
    {6, 127,62,	51	},	// Level 6  
};
const thermal code  Fan2Talbe2QDISYOGA[]=  //DIS  FAN_PWM YOGA530
{
    {0,	38,	33,	0	},	// Level 0 	
    {1,	42, 35,	28	},	// Level 1 
    {2,	46,	39,	30	},	// Level 2 	
    {3,	50,	43,	35	},	// Level 3 	
    {4,	58,	47,	38	},	// Level 4  
    {5, 68,	52,	38	},	// Level 5  
    {6, 127,62,	38	},	// Level 6  
};

//-----------------------------------------------------------------------------
// Init. thermal fan 1
//-----------------------------------------------------------------------------
void InitThermalTable1(void)
{
	BYTE index;
	index=0x00;
	if(ThermalMode == ThermalMode_BALANCE)
	{
		QuiteMode = 0;
		HighPerf = 0;
	}
	else if(ThermalMode == ThermalMode_QUIET)
	{
		QuiteMode = 1;
		HighPerf = 0;
	}
	else if(ThermalMode == ThermalMode_HIGHP)
	{
		QuiteMode = 0;
		HighPerf = 1;
	}
	
	if(Read_YOGA_ID())//yoga
	{
		if(!FanTable_Type) //TABLE1
		{
			if(uMBGPU&0x02)//DIS
			{
				if(nRamTemp>=48)
					FanTable_Type=1;
			}
			else  //UMA
			{
				if(nRamTemp>=51)
					FanTable_Type=1;
			}
		} 
		else //TABLE2
		{
			if(uMBGPU&0x02)//DIS
			{
				if(nRamTemp<=45)
					FanTable_Type=0;
			}
			else  //UMA
			{
				if(nRamTemp<=48)
					FanTable_Type=0;
			}
		}
		if(uMBGPU&0x02)//yoga DIS
		{
			if(!FanTable_Type)
			{	
				while(index<(sizeof(Fan1Talbe1DISYOGA)/sizeof(thermal)))
				{
					*Fan1Step[index].FanOn=Fan1Talbe1DISYOGA[index].CFanOn;
					*Fan1Step[index].FanOff=Fan1Talbe1DISYOGA[index].CFanOff;
					*Fan1Step[index].RPM=Fan1Talbe1DISYOGA[index].CRPM;
					index++;
				}
			}
			else
			{
				if(QuiteMode)
				{
					while(index<(sizeof(Fan1Talbe2QDISYOGA)/sizeof(thermal)))
					{
						*Fan1Step[index].FanOn=Fan1Talbe2QDISYOGA[index].CFanOn;
						*Fan1Step[index].FanOff=Fan1Talbe2QDISYOGA[index].CFanOff;
						*Fan1Step[index].RPM=Fan1Talbe2QDISYOGA[index].CRPM;
						index++;
					}
				}
				else
				{
					while(index<(sizeof(Fan1Talbe2DISYOGA)/sizeof(thermal)))
					{
						*Fan1Step[index].FanOn=Fan1Talbe2DISYOGA[index].CFanOn;
						*Fan1Step[index].FanOff=Fan1Talbe2DISYOGA[index].CFanOff;
						*Fan1Step[index].RPM=Fan1Talbe2DISYOGA[index].CRPM;
						index++;
					}
				}
			}
		}
		else //yoga UMA
		{
			if(!FanTable_Type)
			{
				while(index<(sizeof(Fan1Talbe1UMAYOGA)/sizeof(thermal)))
				{
					*Fan1Step[index].FanOn=Fan1Talbe1UMAYOGA[index].CFanOn;
					*Fan1Step[index].FanOff=Fan1Talbe1UMAYOGA[index].CFanOff;
					*Fan1Step[index].RPM=Fan1Talbe1UMAYOGA[index].CRPM;
					index++;
				}
			}
			else
			{
				if(QuiteMode)
				{
					while(index<(sizeof(Fan1Talbe2QUMAYOGA)/sizeof(thermal)))
					{
						*Fan1Step[index].FanOn=Fan1Talbe2QUMAYOGA[index].CFanOn;
						*Fan1Step[index].FanOff=Fan1Talbe2QUMAYOGA[index].CFanOff;
						*Fan1Step[index].RPM=Fan1Talbe2QUMAYOGA[index].CRPM;
						index++;
					}
				}
				else
				{
					while(index<(sizeof(Fan1Talbe2UMAYOGA)/sizeof(thermal)))
					{
						*Fan1Step[index].FanOn=Fan1Talbe2UMAYOGA[index].CFanOn;
						*Fan1Step[index].FanOff=Fan1Talbe2UMAYOGA[index].CFanOff;
						*Fan1Step[index].RPM=Fan1Talbe2UMAYOGA[index].CRPM;
						index++;
					}
				}
			}
		}
	}
	else //530
	{
		if(!FanTable_Type) //TABLE1
		{
			if(nRamTemp>=50)
				FanTable_Type=1;
		}
		else //TABLE2
		{
			if(nRamTemp<=45)
				FanTable_Type=0;
		}
		
		if(uMBGPU&0x02)//530 DIS
		{
			if(!FanTable_Type)        // table1  fan1 14''15" High/balance mode
			{
				while(index<(sizeof(Fan1Talbe1DIS)/sizeof(thermal)))
				{
					*Fan1Step[index].FanOn=Fan1Talbe1DIS[index].CFanOn;
					*Fan1Step[index].FanOff=Fan1Talbe1DIS[index].CFanOff;
					*Fan1Step[index].RPM=Fan1Talbe1DIS[index].CRPM;
					index++;
				}
			}
			else //table 2
			{
				if(HighPerf)  //14''15'' table 2  high mode
				{
					while(index<(sizeof(Fan1Talbe2DIS_H)/sizeof(thermal)))
					{
							*Fan1Step[index].FanOn=Fan1Talbe2DIS_H[index].CFanOn;
							*Fan1Step[index].FanOff=Fan1Talbe2DIS_H[index].CFanOff;
							*Fan1Step[index].RPM=Fan1Talbe2DIS_H[index].CRPM;
							index++;
					}
				}
				else      //14''15'' table 2  Balance
				{
					while(index<(sizeof(Fan1Talbe2DIS_B)/sizeof(thermal)))
					{
						*Fan1Step[index].FanOn=Fan1Talbe2DIS_B[index].CFanOn;
						*Fan1Step[index].FanOff=Fan1Talbe2DIS_B[index].CFanOff;
						*Fan1Step[index].RPM=Fan1Talbe2DIS_B[index].CRPM;
						index++;
					}
				}
			}
			
		}
		else //530 UMA
		{
			if(!FanTable_Type)   //table 1
			{
				while(index<(sizeof(Fan1Talbe1UMA)/sizeof(thermal)))
				{
					*Fan1Step[index].FanOn=Fan1Talbe1UMA[index].CFanOn;
					*Fan1Step[index].FanOff=Fan1Talbe1UMA[index].CFanOff;
					*Fan1Step[index].RPM=Fan1Talbe1UMA[index].CRPM;
					index++;
				}
			}
			else 				
			{
				while(index<(sizeof(Fan1Talbe2UMA)/sizeof(thermal)))   //table 2
				{
					*Fan1Step[index].FanOn=Fan1Talbe2UMA[index].CFanOn;
					*Fan1Step[index].FanOff=Fan1Talbe2UMA[index].CFanOff;
					*Fan1Step[index].RPM=Fan1Talbe2UMA[index].CRPM;
					index++;
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------
// Init. thermal fan 2
//-----------------------------------------------------------------------------
void InitThermalTable2(void)
{
	BYTE index;
	index=0x00;
	if(Read_YOGA_ID())
	{
		if(!FanTable_Type)//TABLE1
		{
			while(index<(sizeof(Fan2Talbe1DISYOGA)/sizeof(thermal)))
			{
				*Fan2Step[index].FanOn=Fan2Talbe1DISYOGA[index].CFanOn;
			    *Fan2Step[index].FanOff=Fan2Talbe1DISYOGA[index].CFanOff;
			    *Fan2Step[index].RPM=Fan2Talbe1DISYOGA[index].CRPM;
			    index++;
			}
		}
		else//TABLE2
		{
			if(QuiteMode)
			{
				while(index<(sizeof(Fan2Talbe2QDISYOGA)/sizeof(thermal)))
				{
					*Fan2Step[index].FanOn=Fan2Talbe2QDISYOGA[index].CFanOn;
					*Fan2Step[index].FanOff=Fan2Talbe2QDISYOGA[index].CFanOff;
					*Fan2Step[index].RPM=Fan2Talbe2QDISYOGA[index].CRPM;
					index++;
				}
			}
			else
			{
				while(index<(sizeof(Fan2Talbe2DISYOGA)/sizeof(thermal)))
				{
					*Fan2Step[index].FanOn=Fan2Talbe2DISYOGA[index].CFanOn;
					*Fan2Step[index].FanOff=Fan2Talbe2DISYOGA[index].CFanOff;
					*Fan2Step[index].RPM=Fan2Talbe2DISYOGA[index].CRPM;
					index++;
				}
			}
		}	
	}
	else//530
	{
		if(!FanTable_Type)//TABLE 1 DIS fan2 14''15'' high/balance
		{		  
			while(index<(sizeof(Fan2Talbe1DIS)/sizeof(thermal)))
				{
					*Fan2Step[index].FanOn=Fan2Talbe1DIS[index].CFanOn;
				  	*Fan2Step[index].FanOff=Fan2Talbe1DIS[index].CFanOff;
				    *Fan2Step[index].RPM=Fan2Talbe1DIS[index].CRPM;
				   	index++;
				}
		}
		else//TABLE2
		{
			if(HighPerf)
			{
				while(index<(sizeof(Fan2Talbe2DIS_H)/sizeof(thermal)))
				{
					*Fan2Step[index].FanOn=Fan2Talbe2DIS_H[index].CFanOn;
					*Fan2Step[index].FanOff=Fan2Talbe2DIS_H[index].CFanOff;
					*Fan2Step[index].RPM=Fan2Talbe2DIS_H[index].CRPM;
					index++;
				}
			}
			else
			{
				while(index<(sizeof(Fan2Talbe2DIS_B)/sizeof(thermal)))
				{
					*Fan2Step[index].FanOn=Fan2Talbe2DIS_B[index].CFanOn;
					*Fan2Step[index].FanOff=Fan2Talbe2DIS_B[index].CFanOff;
					*Fan2Step[index].RPM=Fan2Talbe2DIS_B[index].CRPM;
					index++;
				}
			}
		}	
	}
}

void Check_FANRPM_Count(BYTE *BRPM_Cnt, BYTE TempType)
{
	if ( *BRPM_Cnt >= 10 )
	{
		if ( TempType == TEMP_TYPE_FAN1RPM )
		{ 
			Fan1RPM_Target= Fan1RPM_TargetBackUp; 
		}	
		else if( TempType == TEMP_TYPE_FAN2RPM ) 
		{ 
			Fan2RPM_Target = Fan2RPM_TargetBackUp;
		}	

		*BRPM_Cnt = 0;
	}
	else
	{ (*BRPM_Cnt)++; }
}
void CheckFanRPM1(void)
{
	BYTE FanLevel_tcpu = Fan1Level;

// check CPU fan table2:		
	if(FanLevel_tcpu <((sizeof(Fan1Step)/sizeof(thermal))-1))
	{
		if(Average_Temp>=*Fan1Step[FanLevel_tcpu].FanOn)
		{
			FanLevel_tcpu+= 1;
		}
	}
	
	if (FanLevel_tcpu>0)
	{
		if (Average_Temp< *Fan1Step[FanLevel_tcpu].FanOff)
		{			
			FanLevel_tcpu-=1;
		}
	}

	Fan1Level = FanLevel_tcpu;
	Fan1RPMTemp =*Fan1Step[FanLevel_tcpu].RPM; 

	
    if(Fan1RPMTemp == Fan1RPM_TargetBackUp)  
    {
	    Check_FANRPM_Count(&Fan1RPMCount,TEMP_TYPE_FAN1RPM); 
    }
	else
	{
	   Fan1RPM_TargetBackUp = Fan1RPMTemp; 
	   Fan1RPMCount = 0x00;
	}
}

void CheckFanRPM2(void)
{
	BYTE FanLevel_tcpu = Fan2Level;

// check GPU fan table2:		
	if(FanLevel_tcpu <((sizeof(Fan2Step)/sizeof(thermal))-1))
	{
		if(Average_Temp>=*Fan2Step[FanLevel_tcpu].FanOn)
		{
			FanLevel_tcpu+= 1;
		}
	}
	
	if (FanLevel_tcpu>0)
	{
		if (Average_Temp< *Fan2Step[FanLevel_tcpu].FanOff)
		{			
			FanLevel_tcpu-=1;
		}
	}

	Fan2Level = FanLevel_tcpu;
	Fan2RPMTemp =*Fan2Step[FanLevel_tcpu].RPM; 

	
    if(Fan2RPMTemp == Fan2RPM_TargetBackUp)  
    {
	    Check_FANRPM_Count(&Fan2RPMCount,TEMP_TYPE_FAN2RPM); 
    }
	else
	{
	   Fan2RPM_TargetBackUp = Fan2RPMTemp; 
	   Fan2RPMCount = 0x00;
	}


}

void CheckFan1Level(void)
{
	BYTE FanLevel_tcpu = Fan1Level;

// check CPU fan table2:		
	if(FanLevel_tcpu <((sizeof(Fan1Step)/sizeof(thermal))-1))
	{
		if(TEMP_Buff_3>=*Fan1Step[FanLevel_tcpu].FanOn)
		{
			FanLevel_tcpu+= 1;
		}
	}
	
	if (FanLevel_tcpu>0)
	{
		if (TEMP_Buff_3< *Fan1Step[FanLevel_tcpu].FanOff)
		{			
			FanLevel_tcpu-=1;
		}
	}
	Fan1Level = FanLevel_tcpu;
}

void CheckFan2Level(void)
{
	BYTE FanLevel_tcpu = Fan2Level;

// check GPU fan table2:		
	if(FanLevel_tcpu <((sizeof(Fan2Step)/sizeof(thermal))-1))
	{
		if(VGA_TBuff3>=*Fan2Step[FanLevel_tcpu].FanOn)
		{
			FanLevel_tcpu+= 1;
		}
	}
	
	if (FanLevel_tcpu>0)
	{
		if (VGA_TBuff3< *Fan2Step[FanLevel_tcpu].FanOff)
		{			
			FanLevel_tcpu-=1;
		}
	}
	Fan2Level = FanLevel_tcpu;
}

void  CheckFanLevel(void)
{
	if (Fan1Level>=Fan2Level)
	{
		FanLevel=Fan1Level;
	}
	else
	{
		FanLevel=Fan2Level;
	}
}

void  CheckFan1RPM(void)
{
	Fan1RPMTemp =*Fan1Step[FanLevel].RPM; 
	
    if(Fan1RPMTemp == Fan1RPM_TargetBackUp)  
    {
		Check_FANRPM_Count(&Fan1RPMCount,TEMP_TYPE_FAN1RPM); 
    }
	else
	{
	   Fan1RPM_TargetBackUp = Fan1RPMTemp; 
	   Fan1RPMCount = 0x00;
	}
}

void  CheckFan2RPM(void)
{
	Fan2RPMTemp =*Fan2Step[FanLevel].RPM; 
	if(Fan2RPMTemp == Fan2RPM_TargetBackUp)  
	{
		Check_FANRPM_Count(&Fan2RPMCount,TEMP_TYPE_FAN2RPM); 
	}
	else
	{
		Fan2RPM_TargetBackUp = Fan2RPMTemp; 
	 	Fan2RPMCount = 0x00;
	}
}


void Fan1MainControl(void)
{
	if ( FAN1Count >= 0 )	// 100ms*20=2s
	{
		if(((Fan1RPM_Target*100)-50)>FAN1_RPM)
		{
			if(FAN_PWM<FAN_PWM_Max)
			FAN_PWM++;	
		}
		else if(((Fan1RPM_Target*100)+50)<FAN1_RPM)
		{
			if(FAN_PWM>0x00)
			FAN_PWM--;	
		}	

		FAN1Count = 0;
	}
	else
	{ (FAN1Count )++; }		
	
}
void Fan2MainControl(void)
{
	if ( FAN2Count  >= 0 )	// 100ms*20=2s
	{
		if(((Fan2RPM_Target*100)-50)>FAN2_RPM)
		{
			if(FAN1_PWM<FAN_PWM_Max)
			FAN1_PWM++;	
		}
		else if(((Fan2RPM_Target*100)+50)<FAN2_RPM)
		{
			if(FAN1_PWM>0x00)
			FAN1_PWM--;	
		}

		FAN2Count = 0;
	}
	else
	{ (FAN2Count)++; }	
}

void Oem_Fan_Control(void)
{
	FAN_PWM_ALT;		// Defind PWM is Alt.
	InitThermalTable1();
	// Check ISCT status.
	if ( IS_MASK_SET(uISCT, b4ISCT_FanDis) )	// Check ISCT Fan is status.
	{
		FAN_PWM = Target_Duty = 0;	// Turn off FAN.
		FAN1_RPM=0x00;
		return;
	}
	if ( (IS_MASK_SET(ERR_THMSTS, b1VGATempEr)) ||
		(IS_MASK_SET(Thro_Status2, b4FAN1_FullOn)) )  // Optimize read CPU temperature functio
	{
		FAN_PWM = Target_Duty = FAN_PWM_Max;	// Fan turn on max.
		return;
	}
	
	//Add workaround keep fan speed 50 duty under DOS when peci fail.
	if( IS_MASK_SET(ERR_THMSTS, b3PECITempEr)&&((SYS_STATUS & 0x07) == 0 ))
	{
		FAN_PWM = Target_Duty = FAN_PWM_Min4;	// 50% duty
		return;
	}
	//Add workaround keep fan speed 50 duty under DOS when peci fail.

	if ( Fan_ON_Count != 0 )					// Turn on Fan x sec.
	{
		Fan_ON_Count--;
		FAN_PWM = Target_Duty = FAN_PWM_Min;	// PWM min. 30%
		return;
	}

	if( SystemNotS0 )	// Not S0 status.
	{
		FAN_PWM = Target_Duty = 0;	// Turn off FAN.
		FAN1_RPM=0x00;
		return;
	}

	if(Read_YOGA_ID()) //yoga 
	{
		if ( TEMP_Buff_3 >= VGA_TBuff3 )
		{	// CPU > GPU
			Average_Temp = TEMP_Buff_3;
		}
		else
		{	// GPU > CPU
			Average_Temp = VGA_TBuff3;
		}

		if( Average_Temp <= Fan1Off_Step1)	//T105+
		{
			FAN_PWM = Target_Duty = 0;	// Turn off FAN.
			FAN1_RPM=0x00;
			return;
		}	

		if ( Average_Temp >= Fan1Off_Step1)  //T105+
		{
			if ( Average_Temp < Fan1On_Step1)  // change 0 to BTStepTemp//4 to 0
			{
				if ( FAN_PWM != 0 )
				{ 
					FAN_PWM = Target_Duty = Fan1RPM_Step1; //LMLKBL0004:add.  
				}	// PWM min. 30%
				else
				{ 
					FAN_PWM = Target_Duty = 0; // turn off fan.
					FAN1_RPM=0x00; //ANGELAS076:add 
				}				
				return;
			}
		}
		CheckFanRPM1();
	}
	
	else   //530s
	{
		if ( TEMP_Buff_3 >= VGA_TBuff3 )
		{	// CPU > GPU
			Average_Temp = TEMP_Buff_3;
		}
		else
		{	// GPU > CPU
			Average_Temp = VGA_TBuff3;
		}
		
		if( Average_Temp <= Fan1Off_Step2)	//T105+
		{
			FAN_PWM = Target_Duty = 0;	// Turn off FAN.
			FAN1_RPM=0x00;
			return;
		}	

		if ( Average_Temp >= Fan1Off_Step2)  //T105+
		{
			if ( Average_Temp < Fan1On_Step1)  // change 0 to BTStepTemp//4 to 0
			{
				if ( FAN_PWM != 0 )
				{ 
					FAN_PWM = Target_Duty = 0x2E; //LMLKBL0004:add.
				}	// PWM min. 30%
				else
				{ 
					FAN_PWM = Target_Duty = 0; // turn off fan.
					FAN1_RPM=0x00; //ANGELAS076:add 
				}				
				return;
			}
		}
		CheckFan1Level();
		CheckFan2Level();
		CheckFanLevel();
		CheckFan1RPM();
	}
	
	Fan1MainControl();                         
}
//72JERRY033: Modify fan table follow '720S_13IKB fan table_V01'.
//72JERRY014¡êoS+ Add the second fan method
//72JERRY078:Update fan table follow'720S_13IKB fan table_V07'. 
void Oem_Fan1_Control(void)
{
	FAN1_PWM_ALT;		// Defind PWM is Alt.
	InitThermalTable2();
	// Check ISCT status.
	if ( IS_MASK_SET(uISCT, b4ISCT_FanDis) )	// Check ISCT Fan is status.
	{
		FAN1_PWM = Target_Duty = 0;	// Turn off FAN.
		FAN2_RPM=0x00;
		return;
	}
	
	if ( (IS_MASK_SET(ERR_THMSTS, b1VGATempEr)) ||
		(IS_MASK_SET(Thro_Status2, b4FAN1_FullOn)) )  // Optimize read CPU temperature functio
	{
		FAN1_PWM = Target_Duty = FAN_PWM_Max;	// Fan turn on max.
		return;
	}
	
	//Add workaround keep fan speed 50 duty under DOS when peci fail.
	if( IS_MASK_SET(ERR_THMSTS, b3PECITempEr)&&((SYS_STATUS & 0x07) == 0 ))
	{
		FAN1_PWM = Target_Duty = FAN_PWM_Min4;	// 50% duty
		return;
	}
	//Add workaround keep fan speed 50 duty under DOS when peci fail.

	if ( Fan_ON_Count != 0 )					// Turn on Fan x sec.
	{
		Fan_ON_Count--;
		FAN1_PWM = Target_Duty = FAN_PWM_Min;	// PWM min. 30%
		return;
	}

	if( SystemNotS0 )	// Not S0 status.
	{
		FAN1_PWM = Target_Duty = 0;	// Turn off FAN.
		FAN2_RPM=0x00;
		return;
	}

	if(Read_YOGA_ID()) //yoga 
	{
		if ( TEMP_Buff_3 >= VGA_TBuff3 )
		{	// CPU > GPU
			Average_Temp = TEMP_Buff_3;
		}
		else
		{	// GPU > CPU
			Average_Temp = TEMP_Buff_3;
		}

		if( Average_Temp <= Fan2Off_Step1)	//T105+
		{
			FAN1_PWM = Target_Duty = 0;	// Turn off FAN.
			FAN2_RPM=0x00;
			return;
		}	

		if ( Average_Temp >= Fan2Off_Step1)  //T105+
		{
			if ( Average_Temp < Fan2On_Step1)  // change 0 to BTStepTemp//4 to 0
			{
				if ( FAN1_PWM != 0 )
				{ 
					FAN1_PWM = Target_Duty = Fan2RPM_Step1; //LMLKBL0004:add.  
				}	// PWM min. 30%
				else
				{ 
					FAN1_PWM = Target_Duty = 0; // turn off fan.
					FAN2_RPM=0x00; //ANGELAS076:add 
				}				
				return;
			}
		}
		CheckFanRPM2();
	}
	
	else   //530s
	{
		if(!FanTable_Type)  //table 1
		{
			if ( TEMP_Buff_3 >= VGA_TBuff3 )
			{	// CPU > GPU
				Average_Temp = TEMP_Buff_3;
			}
			else
			{	// GPU > CPU
				Average_Temp = VGA_TBuff3;
			}
		}
		else  ////table 2
		{
		 	if ( VGA_TBuff3 >= 0x3D )  // GPU >=61¡ã
			{	
				Average_Temp = VGA_TBuff3;
			}
			else    				   // GPU < 61¡ã
			{	
				if ( TEMP_Buff_3 >= VGA_TBuff3 )
				{	// CPU > GPU
					Average_Temp = TEMP_Buff_3;
				}
				else
				{	// GPU > CPU
					Average_Temp = VGA_TBuff3;
				}
			}
		}
		
		if( Average_Temp <= Fan2Off_Step2)	//T105+
		{
			FAN1_PWM = Target_Duty = 0;	// Turn off FAN.
			FAN2_RPM=0x00;
			return;
		}	

		if ( Average_Temp >= Fan2Off_Step2)  //T105+
		{
			if ( Average_Temp < Fan2On_Step1)  // change 0 to BTStepTemp//4 to 0
			{
				if ( FAN1_PWM != 0 )
				{ 
					FAN1_PWM = Target_Duty = 0x2C; //LMLKBL0004:add.
				}	// PWM min. 30%
				else
				{ 
					FAN1_PWM = Target_Duty = 0; // turn off fan.
					FAN2_RPM=0x00; //ANGELAS076:add 
				}				
				return;
			}
		}
		CheckFan1Level();
		CheckFan2Level();
		CheckFanLevel();
		CheckFan2RPM();
	}	
	
	Fan2MainControl();              
}
void Fan_Offset_Chk(BYTE Duty_Offset, XBYTE *BDCRx)
{           
	if(((Duty_Offset*100)-50)>FAN1_RPM)  //100 to 50
	{		       
    	if(*BDCRx<FAN_PWM_Max)
			{ *BDCRx += 1; /*RamDebug(0x71);*/}	// +1%
	}
    else if(((Duty_Offset*100)+50)<FAN1_RPM)   //100 to 50
    {
        if(*BDCRx>0x00)
			{ *BDCRx -= 1; /*RamDebug(0x73);*/}	// -1%
    }       

}
//72JERRY014¡êoS+ Add the second fan method
void Fan1_Offset_Chk(BYTE Duty_Offset, XBYTE *BDCRx)
{           
	if(((Duty_Offset*100)-50)>FAN2_RPM)  //100 to 50
	{		       
    	if(*BDCRx<FAN_PWM_Max)
			{ *BDCRx += 1; /*RamDebug(0x71);*/}	// +1%
	}
    else if(((Duty_Offset*100)+50)<FAN2_RPM)   //100 to 50
    {
        if(*BDCRx>0x00)
			{ *BDCRx -= 1; /*RamDebug(0x73);*/}	// -1%
    }       

}
/*****************************************************************************/
// Procedure: OEM_Throttling_Ctrl								TimeDiv: 1 Sec
// Description: System CPU/GPU Throttling Data
// GPIO:
// Referrals:  CPU/GPU Temperature
/*****************************************************************************/
//-----------------------------------------------------------------------------
/*const CPUTHRstruct code CPUTHRTable[]= //ANGELAG006: remove start
{ //      Thr_Off  Thr_On  Turbo_Rem   Turbo_Off    Thr_Shut	       Step  Tjxx  Throttling Table
//-----------------------------------------------------------------------------
	{	90, 	98, 		55, 	60, 		100 },	// 0 for DIS CPU 14'  //MEILING019:modify fan table start.  //MEILING024:change cpu turbo off from 70 to 65.  //MEILING040:modify turbo on/off start.
	{	92, 	99, 		57, 	61, 		105 },	// 1 for DIS GPU 14'
	
	{	90, 	98, 		59, 	64, 		100 },	// 2 for UMA CPU 14'  //MEILING024:change cpu turbo on from 60 to 65. //MEILING033:modify turbo on/off.
	{	100,	100,		100, 	100, 		100 },  // 3 for UMA GPU 14'

	{	90, 	98, 		55, 	60, 		100 },	// 4 for DIS CPU 15'  //MEILING024:change cpu turbo off from 70 to 65.  //MEILING033:modify turbo on/off.
	{	92, 	99, 		57, 	61, 		105 },	// 5 for DIS GPU 15'  //MEILING033:modify turbo on/off.
	
	{	90, 	98, 		58, 	62, 		100 },	// 6 for UMA CPU 15'  //MEILING024:change cpu turbo off from 60 to 65.  //MEILING033:modify turbo on/off.  //MEILING040:modify turbo on/off end.
	{	100,	100,		100, 	100, 		100 },  // 7 for UMA GPU 15'

	{	90,		98,			55,		60,			100	},  // 8 for DIS CPU 17'
	{	92,	    99,	    	57,		61,			105	},  // 9 for DIS GPU 17'

	{	90,		98,			58,		62,			100	},  // 10 for UMA CPU 17'
	{	100,	100,		100,	100,		100	}   // 11 for UMA GPU 17'  //MEILING019:modify fan table end.

};*/ //ANGELAG006: remove end

void Clr_Thermal_Tab(void)
{
	PORT_BYTE_PNTR Thermal_pntr;
    
    CLEAR_MASK(TEST_FLAG, Fan_Control_Auto);  
       
	Thermal_pntr = &FAN_Tab_OFF;	// Base on address 0x0160.
	for( ITempW01=0; ITempW01<=9; ITempW01++ )	// Clear 0160~0169.
	{
		*Thermal_pntr=0;
        Thermal_pntr++;
	}

	Thermal_pntr = &VFAN_Tab_OFF;	// Base on address 0x0170.
	for( ITempW01=0; ITempW01<=9; ITempW01++ )	// Clear 0170~0179.
	{
		*Thermal_pntr=0;
        Thermal_pntr++;
	}
}
//ANGELAG006: add start
const CPUTHRstruct code CPUTHRTable[]=
{ //      Thr_Off  Thr_On  Turbo_Rem   Turbo_Off    Thr_Shut	       Step  Tjxx  Throttling Table
//-----------------------------------------------------------------------------
	{	96, 	98, 		60, 		64, 		100 },	// 0 for DIS CPU 14'  //MEILING019:modify fan table start.  //MEILING024:change cpu turbo off from 70 to 65.  //MEILING040:modify turbo on/off start.
	{	92, 	99, 		60, 		65, 		105 },	// 1 for DIS GPU 14'
	
	{	90, 	98, 		65, 		70, 		100 },	// 2 for UMA CPU 14'  //MEILING024:change cpu turbo on from 60 to 65. //MEILING033:modify turbo on/off.
	{	100,	100,		100, 	100, 	100 },  // 3 for UMA GPU 14'

	{	90, 	98, 		60, 		65, 		100 },	// 4 for DIS CPU 15'  //MEILING024:change cpu turbo off from 70 to 65.  //MEILING033:modify turbo on/off.
	{	92, 	99, 		60, 		65, 		105 },	// 5 for DIS GPU 15'  //MEILING033:modify turbo on/off.
	
	{	90, 	98, 		65, 		70, 		100 },	// 6 for UMA CPU 15'  //MEILING024:change cpu turbo off from 60 to 65.  //MEILING033:modify turbo on/off.  //MEILING040:modify turbo on/off end.
	{	100,	100,		100, 	100, 	100 },  // 7 for UMA GPU 15'

	{	90,	98,		60,		65,		100	},  // 8 for DIS CPU 17'
	{	92,	99,	    	60,		65,		105	},  // 9 for DIS GPU 17'

	{	90,	98,		65,		70,		100	},  // 10 for UMA CPU 17'
	{	100,	100,		100,		100,		100	}   // 11 for UMA GPU 17'  //MEILING019:modify fan table end.

};
//ANGELAG006: add end
//72JERRY033: Modify fan table follow '720S_13IKB fan table_V01'.
//72JERRY061:Update fan table follow'720S_13IKB fan table_V04'.

void OEM_Throttling_Ctrl(void)
{
	BYTE BTemp_Type,BTbStep;
	
	BTbStep = 0;//MEILING023:add.

	if( SystemNotS0 ) // modify cpu keep max p_state,when battery RSOC>5% under DC mode.
	//if( (SystemNotS0&&IS_MASK_SET(SYS_STATUS,AC_ADP))|| ((BAT1PERCL>0x05)&&IS_MASK_CLEAR(SYS_STATUS,AC_ADP))) 
	{
	    H_PROCHOT_OFF();
		return;
	}
#if FAN_TABLE_Already
BTbStep = 0;

	
	//if ( IS_MASK_CLEAR(SYS_MISC1, ACPI_OS) )
	//{ Thro_DOS_Mode(BTbStep); }
	//  always std mode.
    // else
    // { 
		Thro_Std_Mode(BTbStep); 
    // }  

 	/*  
	else if ( IS_MASK_SET(EM7FUNCTION, QIUETMODE) )			// bit5:QIUETMODE, Check Quite mode.
	{ Thro_Quite_Mode(BTbStep); } 						// Quite
	else if ( IS_MASK_SET(EM7FUNCTION, LSPRUNNING) )	// bit3:LSPRUNNING, Check Super performance mode.
	{ Thro_Super_Mode(BTbStep); } 				// Super performance Mode.
	else
	{ Thro_Std_Mode(BTbStep); }
 	*/  
	nOSThrottlingTemp = THR_PRO_ON;	// For OS throttling temperature.
	nOSShutdownTemp = THR_Tab_Shut;	// For OS Shutdown temperature.
	nShutDownTemp = THR_Tab_Shut;	// Shutdown temperature.
#endif 
}

//-----------------------------------------------------------------------------
// Throttling: Standard Mode.
//-----------------------------------------------------------------------------
void Thro_Std_Mode(BYTE BMBStep)
{
	switch( (CPU_TYPE & 0x03) )
	{
	#if Support_TJ100
	case 2:		// Tj100 Step8 and VGA Step16.
		THRTab_ToRAM((0+BMBStep));	//:8 to 0  // Throttle Standard table save to RAM.
		Thro_Shut_Status((0+BMBStep),TEMP_TYPE_CPU); //:8 to 0  // Check Throttle and Shutdown status.
		// Check CPU Turbo function.
		Thro_Turbo(CPUTHRTable[(0+BMBStep)].Turbo_Rem,CPUTHRTable[(0+BMBStep)].Turbo_Off,TEMP_TYPE_CPU); //:8 to 0 
		break;
	#endif

	default:	// Tj90 Step4 and VGA Step16 Default.
		THRTab_ToRAM((0+BMBStep));//T14H:4 to 2	// Throttle Standard table save to RAM.
		Thro_Shut_Status((0+BMBStep),TEMP_TYPE_CPU); 	//:4 to 2		// Check Throttle and Shutdown status.
		// Check CPU Turbo function.
		Thro_Turbo(CPUTHRTable[(0+BMBStep)].Turbo_Rem,CPUTHRTable[(0+BMBStep)].Turbo_Off,TEMP_TYPE_CPU); //:4 to 2	
		break;
	}

	VTHRTab_ToRAM((1+BMBStep));//:16 to 4	// VGA Throttle Standard table save to RAM.
	Thro_Shut_Status((1+BMBStep),TEMP_TYPE_VGA); //:16 to 4  // Check Throttle and Shutdown status.
	// Check GPU Turbo function.
	Thro_Turbo(CPUTHRTable[(1+BMBStep)].Turbo_Rem,CPUTHRTable[(1+BMBStep)].Turbo_Off,TEMP_TYPE_VGA);//:16 to 4

    Thro_Shut_Status(0,TEMP_TYPE_local);//:Add thermal IC local and remote  over temperature protect.
    if(uMBGPU&0x02)//DIS  //ANGELAG012:add.
    {
    Thro_Shut_Status(0,TEMP_TYPE_remote);//:Add thermal IC local and remote  over temperature protect.
    }

}

//-----------------------------------------------------------------------------
// Throttling: Check throttling and Shutdown status.
//-----------------------------------------------------------------------------
void Thro_Shut_Status(BYTE BStep, BYTE BTType)
{
	BYTE BAvgTemp;

	if ( BTType == 2 )
	{	// Set External FAN.
		//if ( EXT_VGA_Buff3 <= CPUTHRTable[BStep].Thr_Off )		// Check prochot turn OFF.
		if ( EXT_VGA_Buff3 < THR_PRO_OFF)		//T14K+
		{ CLR_MASK(Thro_Status, b2ProCH_EXTVGA); }
		//else if ( EXT_VGA_Buff3 >= CPUTHRTable[BStep].Thr_On )	// Check prochot turn ON.
		else if ( EXT_VGA_Buff3 >= THR_PRO_ON)	//T14K+
		{ SET_MASK(Thro_Status, b2ProCH_EXTVGA); }

		//if ( EXT_VGA_Buff3 >= CPUTHRTable[BStep].Thr_Shut )	// Check shutdown status.
		if ( EXT_VGA_Buff3 >=THR_Tab_Shut)	//T14K+
		{ Thro_Count_Shut(&EXTVGA_Shut_Cnt,TEMP_TYPE_EXT); }	// Check x times for shutdown protection.
		else
		{ EXTVGA_Shut_Cnt = 0; }
	}
	else if ( BTType == 1 )
	{	// Set Internal VGA.
		//if ( VGA_TBuff3 <= CPUTHRTable[BStep].Thr_Off )	 	// Check prochot turn OFF.
		 	
		/*if ( VGA_TBuff3 < THR_PRO_OFF)		//+
			{ CLR_MASK(Thro_Status, b1ProCH_VGA); }
			//else if ( VGA_TBuff3 >= CPUTHRTable[BStep].Thr_On )	// Check prochot turn ON.
			else if ( VGA_TBuff3 >= THR_PRO_ON )	//+
		 	 { SET_MASK(Thro_Status, b1ProCH_VGA); }*/

			//if ( VGA_TBuff3 >= CPUTHRTable[BStep].Thr_Shut )  	// Check shutdown status.
			if ( VGA_TBuff3 >= VTHR_Tab_Shut )  //T14K+ //G87:Modify the GPU shutdown temperature  registers
			{ Thro_Count_Shut(&VGA_Shut_Cnt,TEMP_TYPE_VGA); }	// Check x times for shutdown protection.
			else
			{ VGA_Shut_Cnt = 0; }
	}
   	else if ( BTType == 4 ) //G90:Add thermal IC local and remote  over temperature protect.
    {
      	 if ( nVramTemp >= nVramoverTemp )  //ANGELAS070:remove
       // if ( ThermistorCPU_TEMP >= nCPUthermistoroverTemp )  //ANGELAS070:add //thermal ic
        { Thro_Count_Shut(&local_Shut_Cnt,TEMP_TYPE_local); }   // Check x times for shutdown protection.
        else
       	{ local_Shut_Cnt = 0; }
    }
    else if ( BTType == 5 )//G90:Add thermal IC local and remote  over temperature protect.
    {
       	if ( nRamTemp >= nRamoverTemp )  //ANGELAS070:remove //thermal remote
	   //	if ( EXTVGA_TEMP >= nGPUthermistoroverTemp )  //ANGELAS070:add
        { Thro_Count_Shut(&remote_Shut_Cnt,TEMP_TYPE_remote); }   // Check x times for shutdown protection.
        else
       	{ remote_Shut_Cnt = 0; }
    }
	else
	{	// Set Internal CPU.
		//if ( TEMP_Buff_3 <= CPUTHRTable[BStep].Thr_Off ) //-	// Check prochot turn OFF.
		if ( TEMP_Buff_3 < THR_PRO_OFF) //-
		{ CLR_MASK(Thro_Status, b0ProCH_CPU); }
		//else if ( TEMP_Buff_3 >= CPUTHRTable[BStep].Thr_On )  //- // Check prochot turn ON.
		else if ( TEMP_Buff_3 >= THR_PRO_ON)  //-
		{ SET_MASK(Thro_Status, b0ProCH_CPU); }

		//if ( TEMP_Buff_3 >= CPUTHRTable[BStep].Thr_Shut )	//-// Check shutdown status.
		if ( TEMP_Buff_3 >= THR_Tab_Shut )	//+
		{ Thro_Count_Shut(&CPU_Shut_Cnt,TEMP_TYPE_CPU); }	// Check x times for shutdown protection.
		else
		{ CPU_Shut_Cnt = 0; }
	}
	
}

//-----------------------------------------------------------------------------
// Throttling: Check CPU/VGA Turbo status.
//-----------------------------------------------------------------------------
void Thro_Turbo(BYTE BTurboRem, BYTE BTurboOff, BYTE BTType)
{
	BYTE BAvgTemp;

	if ( IS_MASK_CLEAR(SYS_MISC1, ACPI_OS) ) { return; }
	//if((CPU_TYPE &0xc0)!=0x80){return;}//G45:Add judge AMD GPU and NV GPU, AMD GPU not support torbo.//ANGELAS096:-Add CPU turbo enable/disable Q_event.

	if ( BTType == 2 )
	{	// Set External FAN.
		if ( EXT_VGA_Buff3 <= BTurboRem )				// Check turbo resume.
		{ CLR_MASK(Thro_Status, b5Turbo_EXTVGA); }
		else if ( EXT_VGA_Buff3 >= BTurboOff )			// Check turbo Off.
		{ SET_MASK(Thro_Status, b5Turbo_EXTVGA); }
	}
	else if ( BTType == 1 )
	{	// Set Internal VGA.
		//if ( TEMP_Buff_3 <= BTurboRem ) // Check turbo resume.  //MEILING024:remove.
		if ( VGA_TBuff3 <= BTurboRem ) // Check turbo resume.  //MEILING024:change to VGA temp.
		{ CLR_MASK(Thro_Status, b4Turbo_VGA); }
		//else if ( TEMP_Buff_3 >= BTurboOff )	  // Check turbo Off.  //MEILING024:remove.
		else if ( VGA_TBuff3 >= BTurboOff ) // Check turbo Off.  //MEILING024:change to VGA temp.
		{ SET_MASK(Thro_Status, b4Turbo_VGA); }
	}
	else
	{	// Set Internal CPU.
		if(Read_YOGA_ID()) //YOGA
		{
		/*	if(uMBGPU&0x02)//DIS
			{
				if(VGA_TBuff3>=52)
				{
					SET_MASK(Thro_Status, b3Turbo_CPU); 
				}
				else if( VGA_TBuff3 <= 47)	// Check turbo resume.
				{ 
					CLR_MASK(Thro_Status, b3Turbo_CPU);
				}
			}*/
			if(!QuiteMode)
			{
				/*if(CPUI57==0x05)
				{
					if((nVramTemp>=52)||( TEMP_Buff_3 > 92) )           //Thermal u1
					{
						SET_MASK(Thro_Status, b3Turbo_CPU); 
					}
					else if((nVramTemp <= 48)&&( TEMP_Buff_3 < 87) )	// Check turbo resume.
					{ 
						CLR_MASK(Thro_Status, b3Turbo_CPU);
					}
				}
				else*/
				{
					if((nVramTemp>=59)||( TEMP_Buff_3 > 97) )           //Thermal u1
					{
						SET_MASK(Thro_Status, b3Turbo_CPU); 
					}
					else if((nVramTemp <= 55)&&( TEMP_Buff_3 < 92) )	// Check turbo resume.
					{ 
						CLR_MASK(Thro_Status, b3Turbo_CPU);
					}
				}
			}
			else
			{
				if((nVramTemp>=56)||( TEMP_Buff_3 > 97) )           //Thermal u1
				{
					SET_MASK(Thro_Status, b3Turbo_CPU); 
				}
				else if((nVramTemp <= 52)&&( TEMP_Buff_3 < 92) )	// Check turbo resume.
				{ 
					CLR_MASK(Thro_Status, b3Turbo_CPU);
				}
			}
		}
		else//530s
		{ 
			if(uMBGPU&0x02)//DIS
			{
				/*if(( TEMP_Buff_3 > 62)&&(VGA_TBuff3>61))
				{
					SET_MASK(Thro_Status, b3Turbo_CPU); 
				}
				else if( TEMP_Buff_3 <= 58)	// Check turbo resume.
				{ 
					CLR_MASK(Thro_Status, b3Turbo_CPU);
				}*/
				/*if(CPUI57==0x05)
				{
					if((nVramTemp>=52)||( TEMP_Buff_3 > 92) )           //Thermal u1
					{
						SET_MASK(Thro_Status, b3Turbo_CPU); 
					}
					else if((nVramTemp <= 48)&&( TEMP_Buff_3 < 87) )	// Check turbo resume.
					{ 
						CLR_MASK(Thro_Status, b3Turbo_CPU);
					}
				}
				else*/
				{
					if((nVramTemp>=59)||( TEMP_Buff_3 > 97) )           //Thermal u1
					{
						SET_MASK(Thro_Status, b3Turbo_CPU); 
					}
					else if((nVramTemp <= 55)&&( TEMP_Buff_3 < 92) )	// Check turbo resume.
					{ 
						CLR_MASK(Thro_Status, b3Turbo_CPU);
					}
				}
			}
			else //uma
			{
				if( TEMP_Buff_3 > 85)
				{
					SET_MASK(Thro_Status, b3Turbo_CPU); 
				}
				else if( TEMP_Buff_3 <= 80)	// Check turbo resume.
				{ 
					CLR_MASK(Thro_Status, b3Turbo_CPU);
				}
			}
			
		}
	}
	
	//if ( IS_MASK_CLEAR(Thro_Status, (b3Turbo_CPU+b4Turbo_VGA+b5Turbo_EXTVGA)) )   
	//MEILING033:remove start.
	/*if ( IS_MASK_CLEAR(Thro_Status, b3Turbo_CPU) ) 
	{ cTHERMThrottling &= 0xF0; }	// Not need throttling.
	else
	{
	 	cTHERMThrottling &= 0xF0;	// Clear thermal throttling status.
		cTHERMThrottling |= 0x01;	// Set P-State level.
	}*/
	//MEILING033:remove end.
	
	/*
	// Add NV D4 turbo function when battery out power over 45W .
	if(uMBID==0x17)//support 17' and NV
	{
		if(IS_MASK_CLEAR(Thro_Status,b7ProCH_BATT))//battery mode power <35w
		{
			if ( IS_MASK_CLEAR(Thro_Status, (b4Turbo_VGA+b5Turbo_EXTVGA)) )
			//if (IS_MASK_CLEAR(BatteryAlarm,BATOCP)&&IS_MASK_CLEAR(Thro_Status, (b4Turbo_VGA+b5Turbo_EXTVGA)))
			{
				if ( IS_MASK_CLEAR(uVGATurboFun,uDisVGATurboOK) )		// bit2.
				{
					SET_MASK(uVGATurboFun, uDisVGATurboOK);			// Set bit2.
					CLR_MASK(uVGATurboFun, uDisVGATurboD4OK);			// Set bit4.
					CLR_MASK(uVGATurboFun, uDisVGATurboD2OK);			// Set bit5.
					RamDebug(0xEE);  RamDebug(0x63);       
					ECQEvent(DIS_TURBO_63); // 0x63 VGA ENABLE turbo.G24:remove GPU turbo Q event //:Add VGA turbo Q eventD1
				}
			}
			else
			{
	 			if ( IS_MASK_CLEAR(uVGATurboFun,uDisVGATurboD2OK) )	// bit2.
	 			{
					SET_MASK(uVGATurboFun, uDisVGATurboD2OK);			// Set bit5.
					CLR_MASK(uVGATurboFun, uDisVGATurboD4OK);			//  bit4.
					CLR_MASK(uVGATurboFun, uDisVGATurboOK);			//  bit2.
					RamDebug(0xEE);  
					RamDebug(0x62);        
					ECQEvent(EN_TURBO_62); // 0x62 VGA DISABLE turbo.G24:remove GPU turbo Q event//G58:Add VGA turbo Q eventD2
	 			}
			}
		}
		else//battery mode power >45w
		{
			if ( IS_MASK_CLEAR(uVGATurboFun,uDisVGATurboD4OK) )	// bit4.
			{
				SET_MASK(uVGATurboFun, uDisVGATurboD4OK);			// Set bit4.
				CLR_MASK(uVGATurboFun, uDisVGATurboOK);			// bit2.
				CLR_MASK(uVGATurboFun, uDisVGATurboD2OK);			// bit5.
				ECQEvent(EN_TURBO_61);   //////D4
			}
		}
	}
	else  //support nv only
	{

		if ( IS_MASK_CLEAR(Thro_Status, (b4Turbo_VGA+b5Turbo_EXTVGA)) )
		{
			if ( IS_MASK_SET(uVGATurboFun,uDisVGATurboOK) )		// bit2.
			{
				CLR_MASK(uVGATurboFun, uDisVGATurboOK);			// Set bit2.
				RamDebug(0xEE);  RamDebug(0x63);       
				ECQEvent(DIS_TURBO_63); // 0x63 VGA ENABLE turbo.:remove GPU turbo Q event //:Add VGA turbo Q event
			}
		}
		else
		{
	 		if ( IS_MASK_CLEAR(uVGATurboFun,uDisVGATurboOK) )	// bit2.
	 		{
				SET_MASK(uVGATurboFun, uDisVGATurboOK);			// Set bit2.
				RamDebug(0xEE);  RamDebug(0x62);      
				ECQEvent(EN_TURBO_62); // 0x62 VGA DISABLE turbo.G24:remove GPU turbo Q event//G58:Add VGA turbo Q event
	 		}
		}
	}
	//Add NV D4 turbo when battery out power over 45W.

	*///ANGELAS096:-Add CPU turbo enable/disable Q_event.
    //ANGELAS096:s+Add CPU turbo enable/disable Q_event.
	//Add CPU turbo (66,67)Q event follow thermal.
    if ( IS_MASK_CLEAR(Thro_Status, b3Turbo_CPU ))
	{
		if ( IS_MASK_SET(uVGATurboFun,uDisCPUTurboOK) )		
		{
			CLR_MASK(uVGATurboFun, uDisCPUTurboOK);	
            //ECQEvent(DIS_CPUTURBO_67);  //MEILING036:remove.
            ECQEvent(EN_CPUTURBO_67);  //MEILING036:add.
		}
	}
	else
	{
	 	if ( IS_MASK_CLEAR(uVGATurboFun,uDisCPUTurboOK) )	// bit3.
	 	{
			SET_MASK(uVGATurboFun, uDisCPUTurboOK);			// Set bit3. 
            //ECQEvent(EN_CPUTURBO_66);  //MEILING036:remove.
          ECQEvent(DIS_CPUTURBO_66);  //MEILING036:add.
	 	}
	}
	//ANGELAS096:e+Add CPU turbo enable/disable Q_event.
	// Remove CPU turbo(66,67)Q event.
	//ANGELAS103:S+Add GPU turbo enable/disable Q_event.
    //if(uMBGPU&0x01)//only UMA need support for SKL  //MEILING033:remove.
	{ 	
		if ( IS_MASK_CLEAR(Thro_Status, b4Turbo_VGA) )
		{
			if ( IS_MASK_SET(uVGATurboFun,uDisVGATurboOK) )
			{
				//ECQEvent(EN_TURBO_62);  //MEILING033:remove.
				cGPUThermalThrottling = 0; //MEILING033:add.
				CLR_MASK(uVGATurboFun, uDisVGATurboOK);	
			}
		
		}
		else
		{
			if ( IS_MASK_CLEAR(uVGATurboFun,uDisVGATurboOK) )	// bit2.
	 		{
				SET_MASK(uVGATurboFun, uDisVGATurboOK);			// Set bit2. 
            	//ECQEvent(DIS_TURBO_63); //MEILING033:remove.
            	cGPUThermalThrottling = 0;  //ANGELAG006: modify D2 to D3 //MEILING033:add.
	 		}
		}
    }

	//MEILING040:S+ add internal GPU turbo on/off.
	if(uMBGPU&0x01)
	{ 	
		if ( IS_MASK_CLEAR(Thro_Status3, b0IntGPU_TURBO) )
		{
			if ( IS_MASK_SET(uVGATurboFun,uIntVGATurbo) ) 
			{
				CLR_MASK(uVGATurboFun, uIntVGATurbo);	
				ECQEvent(EN_TURBO_62); 
			}
		
		}
		else
		{
			if ( IS_MASK_CLEAR(uVGATurboFun,uIntVGATurbo) )	// bit5.
	 		{
				SET_MASK(uVGATurboFun, uIntVGATurbo);       // set bit5.
            	ECQEvent(DIS_TURBO_63);
	 		}
		}
    }
	//MEILING040:E+
	
	//ANGELAS103:E+Add GPU turbo enable/disable Q_event.
}

//-----------------------------------------------------------------------------
// Throttling: Shutdown count used.
//-----------------------------------------------------------------------------
void Thro_Count_Shut(XBYTE *BShut_Cnt, BYTE TempType)
{
	if ( *BShut_Cnt >= Thro_Shut_Cnt )	// Read 3 times.
	{
		if ( TempType == 2 )
		{ ProcessSID(EXTVGAOVERTEMP_ID); }		// EXT VGAShutdown ID 0x1C.
		else if ( TempType == 1 )
		{ ProcessSID(VGAOVERTEMP_ID); }	// VGA Shutdown ID 0x12.
		else if ( TempType == 4 )
		{ ProcessSID(CPUthermistoroverTemp_ID); }	//ANGELAS082: ThermalIClocalOVerTEPM_ID to CPUthermistoroverTemp_ID//30 to33
		else if ( TempType == 5 )
		{ ProcessSID(GPUthermistoroverTemp_ID); }	//ANGELAS082: ThermalICremoteOVerTEPM_ID to GPUthermistoroverTemp_ID//31 to 34
		else
		{ ProcessSID(DTSOVERTEMP_ID); }	// CPU Shutdown ID 0x11.

        SET_MASK(SysStatus,ERR_ShuntDownFlag);
	    CKCON &= 0x3F;			// set 26 bit counter // bit7-6 00=14ms,01=113ms, 10=911ms, 11(1/9.2MHz)*2'26=7.2S
	    CKCON |= 0x10;			// set 26 bit counter // bit7-6 00=14ms,01=113ms, 10=911ms, 11(1/9.2MHz)*2'26=7.2S
        WDTCON|=0x01;   		/* WDTRST = 1 Reset watch dog timer.*/
        WDTCON|=0x02;   		/* WDTEN  = 1 Enable watch dog.     */
        while(1);       		/* Wait for watch dog time-out      */
		PWSeqStep = 1;
    	PowSeqDelay = 0x00;
        RamDebug(0x0F);  
		SysPowState = SYSTEM_S0_S5;			// EC force Shutdown.

		*BShut_Cnt = 0;
		//RSMRST_LOW();
		//Delay1MS(1);
		//RSMRST_HI();
		SET_MASK(USB_Charger, b2USB_TmlDis);	// Disable USB charger.
	}
	else
	{ (*BShut_Cnt)++; }
}

//-----------------------------------------------------------------------------
// Throttle table save to ram.
//-----------------------------------------------------------------------------
void THRTab_ToRAM(BYTE BStep)
{
    if(IS_MASK_SET(TEST_FLAG, Fan_Control_Auto))
    {
       return;
    }
	THR_PRO_OFF 	= CPUTHRTable[BStep].Thr_Off;
	THR_PRO_ON		= CPUTHRTable[BStep].Thr_On;
	THR_Turo_Rem	= CPUTHRTable[BStep].Turbo_Rem;
	THR_Turo_OFF	= CPUTHRTable[BStep].Turbo_Off;
	THR_Tab_Shut	= CPUTHRTable[BStep].Thr_Shut;
}
//-----------------------------------------------------------------------------
// VGA Throttle table save to ram.
//-----------------------------------------------------------------------------
void VTHRTab_ToRAM(BYTE BStep)
{
    if(IS_MASK_SET(TEST_FLAG, Fan_Control_Auto))
    {
       return;
    }
	VTHR_PRO_OFF 	= CPUTHRTable[BStep].Thr_Off;
	VTHR_PRO_ON		= CPUTHRTable[BStep].Thr_On;
	VTHR_Turo_Rem	= CPUTHRTable[BStep].Turbo_Rem;
	VTHR_Turo_OFF	= CPUTHRTable[BStep].Turbo_Off;
	VTHR_Tab_Shut	= CPUTHRTable[BStep].Thr_Shut;
}

/*****************************************************************************/
// Procedure: ThrottlingControl								TimeDiv: 100mSec
// Description: Chekc all throttling status.
// GPIO:
// Referrals:
/*****************************************************************************/
void ThrottlingControl(void)
{
	if ( SystemIsS0 )
	{
		//   Remove keep P1 states under battery mode.
		/*
	      if ( IS_MASK_CLEAR(SYS_STATUS,AC_ADP)&&(0x00==cBATTThrottling) )
	     { cBATTThrottling = 0x01; }
	     */
		//  Remove keep P1 states under battery mode.
		cThrottlingSet.byte &= 0xF0;	// Clear Throttling status.

		if( ( cThrottlingSet.byte & 0x0F ) < cADPThrottling )	// Compare current and ADP Throttling 
		// if( ( cThrottlingSet.byte & PSTATE_MAXStep ) < cADPThrottling )   //0ptimize CPU P_state (change 16 step to 8 step).//:Change MAX P_state from 7 step to 11 step.
		{
			cThrottlingSet.byte |= cADPThrottling;
		}
		if( ( cThrottlingSet.byte & 0x0F ) < cTHERMThrottling )	// Compare current and Thermal throttling./
		//if( ( cThrottlingSet.byte & PSTATE_MAXStep ) < cTHERMThrottling )//:0ptimize CPU P_state (change 16 step to 8 step).//-
		{
			cThrottlingSet.byte &= 0xF0;
			cThrottlingSet.byte |= cTHERMThrottling;
		}
		if( ( cThrottlingSet.byte & 0x0F ) < cBATTThrottling )	// Compare current and BAT throttling.
		//if( ( cThrottlingSet.byte & PSTATE_MAXStep ) < cBATTThrottling )//:0ptimize CPU P_state (change 16 step to 8 step).//-
		{
			cThrottlingSet.byte &= 0xF0;
			cThrottlingSet.byte |= cBATTThrottling;
		}
		
		//MEILING055:S+ add battery Low throttling level.
		if( ( cThrottlingSet.byte & 0x0F ) < cBATTLowThrottling )	// Compare current and BAT throttling.
		//if( ( cThrottlingSet.byte & PSTATE_MAXStep ) < cBATTThrottling )//:0ptimize CPU P_state (change 16 step to 8 step).//-
		{
			cThrottlingSet.byte &= 0xF0;
			cThrottlingSet.byte |= cBATTLowThrottling;
		}
		//MEILING055:E+
		
		if( ( cThrottlingSet.byte & 0x0F ) < nThrottlingAPSet )	// For AP or debug 
		//if( ( cThrottlingSet.byte & PSTATE_MAXStep ) < nThrottlingAPSet )//0ptimize CPU P_state (change 16 step to 8 step).//-
		{
			cThrottlingSet.byte &= 0xF0;
			cThrottlingSet.byte |= nThrottlingAPSet;
		}

		if( ( cThrottlingSet.byte & 0x0F ) != ( REAL_THROTTLING_INDEX & 0x0F ) )
		//if( ( cThrottlingSet.byte & PSTATE_MAXStep ) != ( REAL_THROTTLING_INDEX & PSTATE_MAXStep ) )//:0ptimize CPU P_state (change 16 step to 8 step).//-
		{	// Cpu throttling for power status change
			if( ( (nRealThermalPinGET) && (( cThrottlingSet.byte & 0x0F ) >= 0x01 ))|| (!nRealThermalPinGET) )
			//if( ( (nRealThermalPinGET) && (( cThrottlingSet.byte & PSTATE_MAXStep ) >= 0x01 ))|| (!nRealThermalPinGET) )//:0ptimize CPU P_state (change 16 step to 8 step).
			{
				cThrottlingSet.fbit.cTHRM_SW = 1;
                RamDebug(cADPThrottling);       
                RamDebug(cTHERMThrottling);       
                RamDebug(cBATTThrottling);        
                RamDebug(REAL_THROTTLING_INDEX);        
				REAL_THROTTLING_INDEX = (REAL_THROTTLING_INDEX & 0xF0) | (cThrottlingSet.byte & 0x0F);
                //REAL_THROTTLING_INDEX = (REAL_THROTTLING_INDEX & 0xF0) | (cThrottlingSet.byte & PSTATE_MAXStep);//:0ptimize CPU P_state (change 16 step to 8 step).//-
                RamDebug(REAL_THROTTLING_INDEX);      
			}
		}
		if(CPUThrottlingDelayTime == 0) //MEILING052:send the 0x1D event after delay time.
		{
			if( cThrottlingSet.fbit.cTHRM_SW )
			{
				cThrottlingSet.fbit.cTHRM_SW = 0;
				if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )	// Check OS mode.
				{
			    	RamDebug(0xEE);  RamDebug(CPU_SLOW_AD);        //T053+ 
					ECQEvent(CPU_SLOW_AD); 	// 0x1D inform bios.
				}
			}
		}
	}
}

//MEILING033:add start GPU throttling control function.
void GPUThrottlingControl(void)
{
	if ( SystemIsS0 && (CPU_TYPE&0x80) ) //ANGELAG014: modify to only NV GPU //MEILING044:add.
	{
		cGPUThrottlingSet.byte &= 0xF0;	// Clear Throttling status.

		if( ( cGPUThrottlingSet.byte & 0x0F ) < cGPUBattThrottling)	// Compare battery power Throttling 
		{
			cGPUThrottlingSet.byte |= cGPUBattThrottling;
		}
		
		//MEILING055:S+ add battery throttling level when PnP AC.
		if( ( cGPUThrottlingSet.byte & 0x0F ) < cGPUACtoBattThrottling)	// Compare PnP AC battery Throttling 
		{
			cGPUThrottlingSet.byte |= cGPUACtoBattThrottling;
		}
		//MEILING055:E+.
		
		if( ( cGPUThrottlingSet.byte & 0x0F ) < cGPUBattOTPThrottling)	// Compare battery OPT Throttling 
		{
			cGPUThrottlingSet.byte &= 0xF0;
			cGPUThrottlingSet.byte |= cGPUBattOTPThrottling;
		}
		if( ( cGPUThrottlingSet.byte & 0x0F ) < cGPUBattLowThrottling)	// Compare battery RSOC low Throttling 
		{
			cGPUThrottlingSet.byte &= 0xF0;
			cGPUThrottlingSet.byte |= cGPUBattLowThrottling;
		}
		if( ( cGPUThrottlingSet.byte & 0x0F ) < cGPUThermalThrottling)	// Compare current and Thermal throttling
		{
			cGPUThrottlingSet.byte &= 0xF0;
			cGPUThrottlingSet.byte |= cGPUThermalThrottling;
		}
		//ANGELAG017: add start
		if( ( cGPUThrottlingSet.byte & 0x0F ) < cGPUBattOCPThrottling)	// Compare current and Thermal throttling
		{
			cGPUThrottlingSet.byte &= 0xF0;
			cGPUThrottlingSet.byte |= cGPUBattOCPThrottling;
		}
		//ANGELAG017: add end
		if( ( cGPUThrottlingSet.byte & 0x0F ) != ( GPU_REAL_THROTTLING_INDEX & 0x0F ) )
		{	// send GPU throttling event.
			GPU_REAL_THROTTLING_INDEX = (GPU_REAL_THROTTLING_INDEX & 0xF0) | (cGPUThrottlingSet.byte & 0x0F);
			RamDebug(0xEE); 
			RamDebug(GPU_SLOW);  
			ECQEvent(GPU_SLOW); 	// 0x1E inform bios.
		}
	}
}
//MEILING033:add end.

/*****************************************************************************/
// Procedure: Chk_FAN_RPM_Control							TimeDiv: 50mSec
// Description: Check FAN is AP control.
// GPIO:
// Referrals:
/*****************************************************************************/
void Chk_FAN_RPM_Control(void)
{
	BYTE BRPM_Manual;
	if(ManualFanPRM !=0)//Add the second fan method
	{
		FAN_PWM_ALT; //Open fan control function.
		if ( (FAN_PWM != FAN_PWM_Max) && (nAtmFanSpeed != ManualFanPRM) )
		{
			if( nAtmFanSpeed > ManualFanPRM )
			{
				if( FAN_PWM > 0 )
				{ FAN_PWM--; }
			}
			else
			{
				if(FAN_PWM < FAN_PWM_Max)
				{ FAN_PWM++; }
				else
				{ FAN_PWM = FAN_PWM_Max; }
			}
		}
		//Open fan control function.
		if ( (FAN_PWM == FAN_PWM_Max) && (nAtmFanSpeed > ManualFanPRM) )
		{
			if( FAN_PWM > 0 )
			{ FAN_PWM--; }
		}
		//Open fan control function.
	}
	//Open fan control function.
	else
	{
		#if !FAN_TABLE_Already
		FAN_PWM_OUT;			// Set FAN_PWM OUTPUT.
		EC_FAN_PWM_HI();
		#endif	// FAN_TABLE_Already
	}
	//Open fan control function.
}
//Add the second fan method
void Chk_FAN1_RPM_Control(void)
{
	BYTE BRPM_Manual;
	if(ManualFan2PRM!=0)
	{
		FAN1_PWM_ALT; //Open fan control function.
		if ( (FAN1_PWM != FAN1_PWM_Max) && (nAtmFan1Speed != ManualFan2PRM) )
		{
			if( nAtmFan1Speed > ManualFan2PRM )
			{
				if( FAN1_PWM > 0 )
				{ FAN1_PWM--; }
			}
			else
			{
				if(FAN1_PWM < FAN1_PWM_Max)
				{ FAN1_PWM++; }
				else
				{ FAN1_PWM = FAN1_PWM_Max; }
			}
		}
		//Open fan control function.
		if ( (FAN1_PWM == FAN1_PWM_Max) && (nAtmFan1Speed > ManualFan2PRM) )
		{
			if( FAN1_PWM > 0 )
			{ FAN1_PWM--; }
		}
		//Open fan control function.
	}
	//Open fan control function.
	else
	{
		#if !FAN_TABLE_Already
		FAN1_PWM_OUT;			// Set FAN_PWM OUTPUT.
		EC_FAN1_PWM_HI();
		#endif	// FAN_TABLE_Already
	}
	//Open fan control function.
}
//Add the second fan method
/*****************************************************************************/
// Procedure: FAN_Dust_Mode										TimeDiv: 1 Sec
// Description: EM8.0 Dust function
// GPIO:
// Referrals:
/*****************************************************************************/
void FAN_Dust_Mode(void)
{
	#if Lenovo_FAN_Dust		// Dust mode
	if( (IS_MASK_SET(SYS_MISC1, ACPI_OS))&&(IS_MASK_CLEAR(LENOVOBATT, BATT_TEST_MODE))&&(SystemIsS0) )
	{
		if ( IS_MASK_SET(SMartNoise, b5DedustingMode) )
		{
			SET_MASK(SMartNoise, b1FanCleanStart);	//start Clean Dust
			StartFanClean = 120;
			CLR_MASK(SMartNoise, b5DedustingMode);
			CLR_MASK(SMartNoise, b2StopReason);		// Dust mode by Completed.
			Fan_Offset_Chk(FAN_Std_Max,&FAN_PWM);//
		}

		if ( IS_MASK_SET(SMartNoise, b1FanCleanStart))
		{
			if ( StartFanClean == 0 )
			{	// dust mode end
				CLR_MASK(THERMAL_STATUS, INITOK);	// turn back fan control right to EC.
				FAN_PWM = FAN_PWM_Min;				// PWM minimum.

				SMartNoise &= 0xEC;					// Clear bit0,1,4.
				FanCleanFull = 0;
				FanCleanHalt = 0;
				CLR_MASK(SMartNoise, b2StopReason);	// Dust mode by Completed.
			}
			else
			{
				StartFanClean--;
				if ( IS_MASK_CLEAR(SMartNoise, b0FanCleanOn))
				{
					if( !ChkTimeScale( &FanCleanFull,Timer_50 ) )  //modify to 50S.
					{
						FAN_PWM = CTR2;//Add fan can be reversed in dust removal mode.
					}
					else
					{ 
					    SET_MASK(SMartNoise, b0FanCleanOn); 
					}
				}
				else
				{
					if( !ChkTimeScale( &FanCleanHalt,Timer_40 ) ) //modify to 40S.
					{

					}
					else
					{ 
					    CLR_MASK(SMartNoise, b0FanCleanOn); 
					}
				}
			}
		}
	}
	else
	{
		CLR_MASK(THERMAL_STATUS, INITOK);	// turn back fan control right to EC.
		FanCleanFull = 0;
		FanCleanHalt = 0;
		SMartNoise = 0x04;					// Stop Fan Dust mode.
	}
	#endif	// Lenovo_FAN_Dust
}


