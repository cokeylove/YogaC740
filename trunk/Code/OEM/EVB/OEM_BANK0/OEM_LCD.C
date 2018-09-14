/*-----------------------------------------------------------------------------
 * TITLE: OEM_LCD.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


//-----------------------------------------------------------------------------
// Initialize Brightness Frequency.
//-----------------------------------------------------------------------------
void Init_LCD_Freq(void)
{
	#if EC_Brightness_Ctrl
	WORD BLCDFreq;

	if ( (SYS_STATUS & 0x07) == 0x03 )	// Win7
	{ BLCDFreq = (WORD)(Bri_Table[cPanelId][BRIGHT_MAX_STEP + 2] << 8) | Bri_Table[cPanelId][BRIGHT_MAX_STEP + 1]; }
	else
	{ BLCDFreq = (WORD)(WIN8_Bri_Tab[cPanelId][WIN8_BRI_MAX_STEP + 2] << 8) | WIN8_Bri_Tab[cPanelId][WIN8_BRI_MAX_STEP + 1]; }

	C7CPRS = (24 * 1000) / BLCDFreq;
	if ( BLCDFreq > 5000 )
	{ C7CPRS++; }
	#endif	// EC_Brightness_Ctrl
}

/* ========================================================================
 * Brightness table define here
 * ======================================================================== */
#if EC_Brightness_Ctrl
BYTE code Bri_Table[BriTable_MAX][BRIGHT_MAX_STEP + 4] =
{
// Dark <------------------------------------------------> Light,	DAC_BRIG, Freq(2 byte), BL_Stable_time
	1,	2,	2,	3,	5,	7,	10,	14,	19,	24,	31,	39,	46,	56,	67,	100,  00,	0xF4, 0x01,			0x03,	//0x00 Default 500Hz
	1,	2,	2,	3,	5,	7,	10,	14,	19,	24,	31,	39,	46,	56,	67,	100,  00, 	0x10, 0x27,			0x03,	//0x01 AUO B156XTN02 10KHz
	2,	2,	3,	4,	6,	9,	13,	18,	23,	30,	38,	45,	55,	65,	78,	100,  00, 	0x58, 0x02,			0x03,	//0x02 LGD LP156WH4-TLN1 600Hz
	1,	2,	3,	5,	7,	11,	15,	21,	27,	34,	42,	52,	63,	73,	86,	100,  00, 	0x58, 0x02,			0x03,	//0x03 LGD LP156WF1-TLB2(FHD_Anti) 600Hz
	1,	2,	3,	4,	6,	8,	12,	16,	22,	28,	35,	43,	52,	61,	73,	100,  00, 	0x58, 0x02,			0x03,	//0x04 SEC LTN156AT24-L01 600Hz
	2,	2,	3,	4,	6,	10,	14,	19,	25,	31,	39,	48,	57,	68,	79,	100,  00, 	0xE8, 0x03,			0x03,	//0x05 CMI N156BGE-L21	1KHz
	2,	2,	3,	4,	6,	10,	14,	19,	25,	32,	41,	50,	61,	74,	88,	100,  00, 	0x58, 0x02,			0x03,	//0x06 AUO B140XTN03 600Hz
	2,	2,	3,	4,	7,	10,	14,	19,	25,	33,	41,	50,	61,	73,	86,	100,  00, 	0x58, 0x02,			0x03,	//0x07 LGD LP140WH2-TLE2 600Hz
	2,	3,	3,	5,	7,	11,	16,	21,	28,	36,	44,	54,	65,	76,	89,	100,  00, 	0x58, 0x02,			0x03,	//0x08 SEC LTN140AT20-L01 600Hz
	2,	3,	3,	5,	7,	11,	15,	21,	27,	35,	43,	53,	63,	75,	87,	100,  00, 	0x2C, 0x01,			0x03,	//0x09 CMI N140BGE-L42	300Hz
	5,	6,	7,	8,	9,	10,	14,	19,	25,	31,	39,	48,	57,	68,	79,	100,  00, 	0x58, 0x02,			0x03,	//0x0A LGD LP156WF1-TLC1(FHD_Glare) 600Hz
	1,	2,	2,	3,	5,	8,	12,	17,	22,	28,	36,	44,	53,	63,	75,	100,  00, 	0xE8, 0x03,			0x03,	//0x0B BOE HB156WX1-100 1KHz
};

BYTE code WIN8_Bri_Tab[BriTable_MAX][WIN8_BRI_MAX_STEP + 4] =
{
// Dark <-----------------------------------------------------------------------> Light, DAC_BRIG, Freq(2 byte), BL_Stable_time
	1,	2,	2,	3,	3,	5,	6,	7,	8,	10,	12,	14,	16,	19,	22,	26,	33,	42,	53,	66,	100,	00,		0x10, 0x27,		0x03,	//0x00 Default 500Hz
	1,	2,	2,	3,	3,	5,	6,	7,	8,	10,	12,	14,	16,	19,	22,	26,	33,	42,	53,	66,	100,	00, 	0x10, 0x27,		0x03,	//0x01 AUO B156XTN02 10KHz
	2,	3,	3,	4,	5,	6,	7,	9,	11,	13,	15,	18,	21,	24,	29,	33,	40,	51,	63,	77,	100, 	00, 	0x58, 0x02,		0x03,	//0x02 LGD LP156WH4-TLN1 600Hz
	3,	3,	4,	4,	5,	5,	7,	8,	9,	10,	12,	13,	16,	20,	23,	27,	33,	43,	55,	68,	100,   	00, 	0x58, 0x02,		0x03,	//0x03 LGD LP156WF1-TLB2(FHD_Anti) 600Hz
	1,	2,	2,	3,	4,	4,	6,	7,	9,	11,	13,	14,	17,	20,	24,	28,	33,	42,	52,	63,	100,   	00, 	0x58, 0x02,		0x03,	//0x04 SEC LTN156AT24-L01 600Hz
	2,	2,	3,	4,	5,	6,	7,	9,	11,	14,	16,	18,	21,	26,	30,	34,	41,	52,	64,	77,	100,   	00, 	0xE8, 0x03,		0x03,	//0x05 CMI N156BGE-L21	1KHz
	2,	2,	3,	3,	4,	6,	7,	9,	11,	13,	16,	18,	21,	25,	29,	34,	41,	54,	67,	83,	100,	00, 	0x58, 0x02,		0x03,	//0x06 AUO B140XTN03 600Hz
	2,	3,	3,	4,	4,	6,	7,	9,	11,	13,	16,	18,	21,	26,	29,	34,	41,	53,	65,	79,	100,  	00, 	0x58, 0x02,		0x03,	//0x07 LGD LP140WH2-TLE2 600Hz
	2,	2,	3,	4,	4,	6,	7,	9,	11,	13,	15,	17,	20,	24,	27,	32,	38,	48,	60,	72,	100,  	00, 	0x58, 0x02,		0x03,	//0x08 SEC LTN140AT20-L01 600Hz
	2,	3,	3,	4,	5,	7,	8,	10,	13,	15,	18,	20,	24,	28,	33,	38,	45,	58,	71,	85,	100,  	00, 	0x2C, 0x01,		0x03,	//0x09 CMI N140BGE-L42	300Hz
	2,	2,	3,	3,	4,	5,	6,	7,	9,	11,	12,	14,	16,	19,	23,	26,	31,	41,	49,	60,	100,	00, 	0x58, 0x02,		0x03,	//0x0A LGD LP156WF1-TLC1(FHD_Glare) 600Hz
	2,	2,	3,	3,	4,	6,	7,	9,	11,	13,	15,	17,	20,	24,	28,	33,	39,	50,	61,	74,	100,	00, 	0xE8, 0x03,		0x03,	//0x0B BOE HB156WX1-100 1KHz
};
#endif	// EC_Brightness_Ctrl

void Load_PWMTimeOut_Timer(void)
{
	#if EC_Brightness_Ctrl
    TR1 = 0;                    // Disable timer1
    ET1 = 0;                  	// Disable timer1 interrupt
    TL1 = Timer_5ms;      		// Reload timer
    TH1 = Timer_5ms>>8; 		// 5ms
	TF1 = 0;					// Clear overflow flag
	TR1 = 1;					// Enable timer1
	#endif	// EC_Brightness_Ctrl
}

void Set_PWM_Value(BYTE PWM_Value)
{
	#if EC_Brightness_Ctrl
	if ((DCR7 != 0) && (DCR7 != CTR3))
	{
		Load_PWMTimeOut_Timer();
		while(!TF1)
		{
			if ( Read_EC_INVT_PWM() == HighActive)
			{
				break;
			}
		}
		Load_PWMTimeOut_Timer();
		while(!TF1)
		{
			if (Read_EC_INVT_PWM()== LowActive)
			{
				DCR7 = PWM_Value;
				return;
			}
		}
	}
	else
	{ DCR7 = PWM_Value; }
	#endif	// EC_Brightness_Ctrl
}

/*****************************************************************************/
// Procedure: Brightness_Control								TimeDiv: 10mSec
// Description:
// GPIO:
// Referrals:
/*****************************************************************************/
void Brightness_Control(void)
{
	#if EC_Brightness_Ctrl
	BYTE BBri_Level;
	BYTE DCR7_Temp;

	if ( (SystemNotS0) || (!Read_ENBKL_IN()) )	// Check S0 state and LCD backlight.
	{
		if(!Read_BKOFF())
		{
			DCR7 = 0;
		}
		return;
	}

	if ( (SYS_STATUS & 0x07) == 0x03 )	// Win7
	{
		if ( nBrightValue >= BRIGHT_MAX_STEP )		// Over 16 Step duty.
		{ nBrightValue = (BRIGHT_MAX_STEP - 1); }
		BBri_Level = Cal_PWM_Duty(Bri_Table[cPanelId][nBrightValue],CTR3);
	}
	else
	{
		if ( nBrightValue >= WIN8_BRI_MAX_STEP )	// Over 21 Step duty.
		{ nBrightValue = (WIN8_BRI_MAX_STEP - 1); }
		BBri_Level = Cal_PWM_Duty(WIN8_Bri_Tab[cPanelId][nBrightValue],CTR3);
	}

	if ( DCR7 == BBri_Level ) { return; }

	DCR7_Temp = DCR7;
    if( DCR7 > BBri_Level )
	{
		//DCR7--;
		DCR7_Temp--;
		Set_PWM_Value(DCR7_Temp);
	}	// Over level.
	else
	{
		//DCR7++;
		DCR7_Temp++;
		Set_PWM_Value(DCR7_Temp);
	}	// Less level.
	#endif	// EC_Brightness_Ctrl
}

//=============================================================================
// 	Calculate PWM duty.
//=============================================================================
BYTE Cal_PWM_Duty(BYTE BCalDuty, BYTE BPWMBase)
{
	return ( BCalDuty = (WORD)((BCalDuty * BPWMBase) / 100));
}

/*****************************************************************************/
// Procedure: Backlight_Control									TimeDiv: 10mSec
// Description:
// GPIO: GPIOG2, GPIOF7
// Referrals:
/*****************************************************************************/
void Backlight_Control(void)
{
	if ( SystemIsS0 && (IS_MASK_CLEAR(uISCT, b5ISCT_BKOFF)) )	// Check S0 and ISCT BKOFF status.
	{
		//if ( (Read_ENBKL_IN()) && (IS_MASK_CLEAR(cCmd, b3BkOff)) && Read_LID_SW_IN())  //ANGELAG005:remove.
		if ( (Read_ENBKL_IN()) && (IS_MASK_CLEAR(cCmd, b3BkOff)) && Read_LID_SW_IN()&&(IS_MASK_CLEAR(OEMControl, b1BkOff))) //ANGELAG005:add. 
		{
			#if EC_Brightness_Ctrl
			if (DCR7 == 0)
			{	//Mos: force Back light enable after PWM stable.
				if ( (SYS_STATUS & 0x07) == 0x03 )	// Win7
				{ BackLight_En_Delay = Bri_Table[cPanelId][BRIGHT_MAX_STEP+3]; }
				else	// Win8
				{ BackLight_En_Delay = WIN8_Bri_Tab[cPanelId][WIN8_BRI_MAX_STEP+3]; }
			}
			#endif	// EC_Brightness_Ctrl

			//Mos: Enable Backlight after PWM stable
			if (BackLight_En_Delay != 0)
			{
				BackLight_En_Delay--;
			}
			else
			{
				BKOFF_ON();
				EC_TS_ON_HI(); //72JERRY049:Add enable/disable touch panel pin.
				CLR_MASK(pProject0,b0DispToggleEn);
				return;
			}
		}
	}
	BKOFF_OFF();
	EC_TS_ON_LOW(); //72JERRY049:Add enable/disable touch panel pin.
}

/*****************************************************************************/
// Procedure: Lid_Process									TimeDiv: 500mSec
// Description:
// GPIO: GPIOH0, GPIOH3
// Referrals:
/*****************************************************************************/
#if 0
void Lid_Process(void)
{
	if ((SystemIsS5)||((DSxPowState == SYSTEM_DSxOK) && (IS_MASK_CLEAR(SysStatus,EnterDS3byLid))) )
	{
		//EC_LID_OUT_LOW();
        	CLEAR_MASK(SysStatus,LidStatus);
        	LID_DEBOUNCE_CNT =0;
	}
       else
      {
		if ( (IS_MASK_SET(ACPI_HOTKEY,b5BIOS_IFFS)) )
		{ 
		    return; 
              }

              if( Read_LID_SW_IN() )
             {	//LCD Cover Open (LID OPEN)
           
                 if(IS_MASK_CLEAR(SysStatus,LidStatus))
                {
                     LID_DEBOUNCE_CNT++;
                     if( LID_DEBOUNCE_CNT > _LID_DEBOUNCE )	// Debounce 2 timer.
                    {
					CLEAR_MASK(SysStatus,CloseLid);
					CLEAR_MASK(SysStatus,EnterS3Lid);
					if (IS_MASK_SET(SysStatus,EnterDS3byLid))
					{
						CLEAR_MASK(SysStatus,EnterDS3byLid);
						if ((DSxPowState == SYSTEM_DSxOK)||((DSxPowState == SYSTEM_S3S4)&&(SystemIsS3)))
						{
							PM_PWRBTN_LOW();
							PWRBTN_pressCnt = 6;		// delay 60ms.
						}
						else
						{
						 
							SET_MASK(SysStatus,LidStatus);
   	   	       		             	//EC_LID_OUT_HI();
   	   	       		             	ECSMI_SCIEvent(0x15); //T25B+  //T25+ //T25A- 
                  			              LID_DEBOUNCE_CNT = 0;
						}
					}
/*  
					else if(SystemIsS3)
					{
							PM_PWRBTN_LOW();
							PWRBTN_pressCnt = 6;		// delay 60ms.
					}
*/ 
					else
					{
					
                 	  	            SET_MASK(SysStatus,LidStatus);
             	     	                  // EC_LID_OUT_HI();
             	     	                   ECSMI_SCIEvent(0x15);  
                  		            LID_DEBOUNCE_CNT = 0;
					}
		        }
                }
            else
            { 
                LID_DEBOUNCE_CNT =0;
            }
        }
        else
        {	//LCD Cover Close (LID Close)
            if(IS_MASK_SET(SysStatus,LidStatus))
            {
                LID_DEBOUNCE_CNT++;
                if( LID_DEBOUNCE_CNT > _LID_CLOSE_DEBOUNCE ) //G66:Modify lid close debounce time from 1s to 3s. 
                
                {
                    CLEAR_MASK(SysStatus,LidStatus);
                    LID_DEBOUNCE_CNT = 0;
                     if(SystemNotS3)   
                    {
                        //EC_LID_OUT_LOW();
                         // SET_MASK(SysStatus,CloseLid);   
			   if(SystemIsS0)
			  { 
			        SET_MASK(SysStatus,CloseLid);    
			        ECSMI_SCIEvent(0x16);  
                       }	// For Deep S3 wake up.
                    }
                }
            }
            else
            { LID_DEBOUNCE_CNT = 0; }
        }
    }
}
#endif
//ANGELAG008: add start
void Lid_Process(void)
{
	if( Read_LID_SW_IN() )
	{
		if(!READ_EC_TP_ON())
		{
			EC_TP_ON_HI();
		}
	}
	else
	{
		if(READ_EC_TP_ON())
		{
			EC_TP_ON_LOW();
		}
	}
}
//ANGELAG008: add end
//72JERRY065:S+Add CMD 0x59 test voice wake up for MFG.
void Voicewakeup_MFG_Process(void)
{
if(cDev.fbit.cD_WovMFGtest==1)
{
	//if(Read_WOV_IRQ())
	{
//	EC_LID_OUT_LOW();
	}

}
}
//72JERRY065:E+Add CMD 0x59 test voice wake up for MFG.