/*-----------------------------------------------------------------------------
 * TITLE: OEM_ADC.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>
//----------------------------------------------------------------------------
// The function of scaning ADC channel
//----------------------------------------------------------------------------
void ScanADCFixChannel(void)
{
   	if(IS_MASK_SET(VCH5CTL, BIT7))
    {
        VCH5CTL |= 0x80;        // write clear data vaild flag
	 	ADP_I= ((WORD)VCH5DATM << 8) + VCH5DATL;
    }
  	if(IS_MASK_SET(VCH4CTL, BIT7))
    {
        VCH4CTL |= 0x80;        // write clear data vaild flag
		Read_VR_CPU_PWROK= ((WORD)VCH4DATM << 8) + VCH4DATL;
    }
    if(IS_MASK_SET(VCH6CTL, BIT7))
	{
		BYTE i;					
		LWORD Psys_SUM = 0;		

		VCH6CTL |= 0x80;        // write clear data vaild flag
		Psys= ((WORD)VCH6DATM << 8) + VCH6DATL;
		
		for (i=3; i>0; i--)
		{
			Psys_Data[i] = Psys_Data[i-1];
			Psys_SUM += Psys_Data[i];
		}
		Psys_Data[0] = Psys;
		Psys_SUM += Psys_Data[0];
	Psys_AvgData = Psys_SUM / 4; //calculate 4 times average value
	}


}

void ScanADCDyChannel1(void)
{
	BYTE i;
	LWORD ADP_I_SUM = 0;

    if(IS_MASK_SET(VCH0CTL, BIT7))
    {
        VCH0CTL |= 0x80;        // write clear data vaild flag
	 	NTC_V1= ((WORD)VCH0DATM << 8) + VCH0DATL;
    }
    if(IS_MASK_SET(VCH1CTL, BIT7))
    {
        VCH1CTL |= 0x80;        // write clear data vaild flag
	 	NTC_V2= ((WORD)VCH1DATM << 8) + VCH1DATL;
    }
    
    if ((SysPowState == SYSTEM_S0)&& Read_AC_IN())//add ACIN condition.
    {
		ADPI2Sec++;
		for (i=3; i>0; i--)
		{
			ADPI_Data[i] = ADPI_Data[i-1];
			ADP_I_SUM += ADPI_Data[i];
		}
		ADPI_Data[0] = ADP_I;
		ADP_I_SUM += ADPI_Data[0];
		ADPI_AvgData = ADP_I_SUM / 4;
   
		if (!(ADPI2Sec%5))//Modify protect setting
		{	
			if(Proch_Delay>0) //Modify power setting
			Proch_Delay--;
			if(Proch_Delay>8)
			Proch_Delay=8;
			if(IS_MASK_CLEAR(SEL_STATE0,PRESENT_A))//AC mode (delay 400ms)
			{
				if(ADPI_AvgData >= 423) 
				{
					SET_MASK(Thro_Status2, b7ProCH_Psys);
					SET_MASK(GPU_Prochot, b1_Psys);
					Proch_Delay = 3;
				}
				else if(ADPI_AvgData < 396)
				{
					if(Proch_Delay == 0)
					{
						CLEAR_MASK(Thro_Status2, b7ProCH_Psys);
						CLEAR_MASK(GPU_Prochot, b1_Psys);
					}
				}
			}
			else   //AC+DC
			{
				if (nBattGasgauge >= 10)
				{
					if(ADPI_AvgData >= 512) //1.5v
					{
						SET_MASK(Thro_Status2, b7ProCH_Psys);
						SET_MASK(GPU_Prochot, b1_Psys);
					}
					else if(ADPI_AvgData < 512)
					{
						CLEAR_MASK(Thro_Status2, b7ProCH_Psys);
						CLEAR_MASK(GPU_Prochot, b1_Psys);
					}
				}
				else
				{
					if(ADPI_AvgData >= 409) //1.27   //1.19v = 406 58w  //64w 1.28v//1.24V//1.2v
					{
						SET_MASK(Thro_Status2, b7ProCH_Psys);
						SET_MASK(GPU_Prochot, b1_Psys);
						Proch_Delay = 8;
					}
					else if(ADPI_AvgData < 382)//1.1 //1.06 = 362 53w   //59w 1.18v 403//1.12V
					{
						if(Proch_Delay ==0)
						{
						CLEAR_MASK(Thro_Status2, b7ProCH_Psys);
						CLEAR_MASK(GPU_Prochot, b1_Psys);
						}
					}
					SET_MASK(GPU_Prochot, b1_Psys);
				}
			}
		}
		
		if (ADPI2Sec > 200)
		{
			if (IS_MASK_SET(SEL_STATE0, PRESENT_A))
			{
				Chk_HybridFORBQ24780_STPP(); 
			}
			else
			{
				Chk_AC_STPP();
			}
			ADPI2Sec=0;
		}
	}
}

//----------------------------------------------------------------------------
// Init ADC module
//----------------------------------------------------------------------------
void Init_ADC(void)
{
	VCH0CTL=ADCChannel_0;   // NTC_V1
	VCH1CTL=ADCChannel_1;   // NTC_V2
	VCH4CTL=0x90;	//CPU_VR_READY
	VCH5CTL=0x90;   // ADP_I (enable voltage channel for channel 5)
	VCH6CTL=0x90;   // psys

}

//---------------------------------------------------------------------------
// Init Voltage Compare Function
// Input: 1 Group. choice the init VC channel  0: VC0 1: VC1 2:VC2 3:All VC Channel
//---------------------------------------------------------------------------
void Init_VC(BYTE Group)
{
	int tGroup = (Group&0x03);
	if( (VCMP0CTL&0x80) || (VCMP1CTL&0x80) || (VCMP2CTL&0x80) )
	{
		IER18 |= 0x80;
	}
}

void Chk_HybridFORBQ24780_STPP(void)
{
	if(Read_AC_IN() && (nBattGasgauge > Chk_Hybrid_STPP_min_BattGasgauge))   //Modify the charger IC turbo boost point (RSOC>10% Enable)   
	{
		SET_MASK(CHGIC_WriteCmd0x37L, TurboBoost);
	 	if(IS_MASK_CLEAR(LENOVOPMFW,BATTERY_MAIN_CAL))  
		{
			cBattFlag0.fbit.cCmdAcOff=0; 			//Resume Charger
 		}
 	}
 	else
 	{
 		if(IS_MASK_CLEAR(LENOVOPMFW,BATTERY_MAIN_CAL))   
 		{
			cBattFlag0.fbit.cCmdAcOff=0;	
 		}
		
 		if(nBattGasgauge <= Chk_Hybrid_STPP_min_BattGasgauge)//change disableTurboBoost capacity from 40 to 36//G82:Change battery capacity from 39 to 40 for disable TurboBoost follow SPEC.
		{
			CLEAR_MASK(CHGIC_WriteCmd0x37L, TurboBoost);
		}
 		if(Read_AC_IN() && (ACModeSTPPEn!=0) && (nBattGasgauge <= (Chk_Hybrid_STPP_min_BattGasgauge-5)))
		{
			if (ADPI_AvgData > ACModeSTPPEn)
			{
				SET_MASK(BatteryAlarm,HybridSTPP);
			}
			if (ADPI_AvgData < ACModeSTPPDis)
			{
				CLR_MASK(BatteryAlarm,HybridSTPP);
			}
		}
		else
		{
			CLR_MASK(BatteryAlarm,HybridSTPP);
		}
 	}
}

void Chk_AC_STPP(void)
{
	if (Read_AC_IN())
	{
		if (1)
		{
			if (ADPI_AvgData > ACModeSTPPEn)
			{
				SET_MASK(BatteryAlarm,ACSTPP);
			}
			if (ADPI_AvgData < ACModeSTPPDis)
			{
				CLR_MASK(BatteryAlarm,ACSTPP);
			}
		}
	}
}

void SetPowerBatteryparameter(void)
{
	//for EM9 adapter detection and protection    //0.0029296875
	if((API_ID > 0x2DE) && (API_ID <= 0x374))  // >2.149  <=2.590   
	{
		AdapterID = AdapterID_170W;
	}
	else if((API_ID > 0x238) && (API_ID <= 0x2D0))  // 1.663(0x238) 2.109(0x2D0)  1.663x1024/3=0x238  
	{
		AdapterID = AdapterID_135W;
	}
	else if((API_ID > 0x190) && (API_ID <= 0x228)) //should change to >1.172(0x190) <=1.618(0x228) for 90W  ">0.693(0xEC) <=1.134(0x183) for 65W ",  //0.0029296875
	{
		AdapterID = AdapterID_90W;
	}
	else if ((API_ID > 0xEC) && (API_ID <= 0x183))//0.693<API_ID<1.134
	{
		AdapterID = AdapterID_65W;
	}
	else if ((API_ID > 0x50) && (API_ID <= 0xE2))//0.234<API_ID<0.663
	{
		AdapterID = AdapterID_45W;
	}
	else if((API_ID > 0x380)||(API_ID <=0x46)) // >2.626 or <0.207
	{
		AdapterID = AdapterID_NON_SUPPORT;
	}


	if(AdapterID==AdapterID_65W || AdapterID==AdapterID_90W || AdapterID==AdapterID_135W || AdapterID==AdapterID_170W)
    {
        CLEAR_MASK(SYS_STATUS,b5UnSuitAdpPow);
		CLEAR_MASK(SYS_STATUS,b4IllegalAdp);
	}
    else if(AdapterID==AdapterID_45W)					
	{
	    CLEAR_MASK(SYS_STATUS,b5UnSuitAdpPow);
		CLEAR_MASK(SYS_STATUS,b4IllegalAdp);
	}
	else if(AdapterID==AdapterID_NON_SUPPORT)
	{
		SET_MASK(SYS_STATUS,b4IllegalAdp);
		CLEAR_MASK(SYS_STATUS,b5UnSuitAdpPow);
          
        if(SystemIsS0) 
	    {
       
            BATT_LEN_ON();//remove AC_OFF function follow charge IC bq24780.    
		}
	} 
}	

//define another function for SetPowerBatteryparameter().
void SetPowerBatteryparameter2(void)
{
	CLEAR_MASK(SYS_STATUS,b5UnSuitAdpPow);
	CLEAR_MASK(SYS_STATUS,b4IllegalAdp);	
}
//---------------------------------------------------------------------------
// AdapterIdentification: hook 10ms
// Detect adapter ID then set protection if need
// Read ID via ADC/Typec
//---------------------------------------------------------------------------
void AdapterIdentification(void)
{
	if(AdapterIDOn_Flag) 
    {
        if(ADPIDON10MS_NUM>0x00)
        {
         	ADPIDON10MS_NUM--; 
        }
        else 
        {
            #if SUPPORT_ADCID_ADAPTER
                SetPowerBatteryparameter(); 
            #endif 
            #if SUPPORT_NOID_ADAPTER
                SetPowerBatteryparameter2(); 
            #endif
            #if SUPPORT_TYPEC_ADAPTER
               TypecAdapterDetection();
            #endif
            AdapterIDOn_Flag = 0x00;
            ADPIDON10MS_NUM=0xFF; 
        }    
    } 
    else 
    {
        if(ADPIDON10MS_NUM>0x01)
        {
            ADPIDON10MS_NUM--; 
        }
        else 
        {   
            if(ADPIDON10MS_NUM==0x01)
            { 
                ADPIDON10MS_NUM--;
            }     
         }
    }
   
}



