/*-----------------------------------------------------------------------------
 * TITLE: OEM_BATTERY.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

void UpdateNameSpace(void)
{
	nBattGasgauge=BAT1PERCL;
	
    if((BAT1PERCL<=0x06)&&IS_MASK_CLEAR(SYS_STATUS,AC_ADP))
    {
       SET_MASK(Thro_Status2, b5Bat_5PProtect);
    }
    else
    {
       CLR_MASK(Thro_Status2, b5Bat_5PProtect);
    }
       
	nCycleCounter = EC_oCCBQl;
	EC_BatteryStatusL = nBattery0x16L;
	EC_BatteryStatusH = nBattery0x16H;

	if(IS_MASK_SET(nBattery0x16L,FullyChg))
    {
		SET_MASK(nBatteryStatus1,ENFULL);

        if(nBattGasgauge == 100) 
        {
            if(IS_MASK_CLEAR(LENOVOPMFW_Temp,BATTERY_FULLED_100))
            {
                SET_MASK(LENOVOPMFW_Temp,BATTERY_FULLED_100);
                ECSMI_SCIEvent(ACPI_BAT1IN_SCI); // Notify BIOS Update Batt Status
            }
        }
   	}
	else
    {
		CLEAR_MASK(nBatteryStatus1,ENFULL);
        CLEAR_MASK(LENOVOPMFW_Temp,BATTERY_FULLED_100);
	}

	Lenovo_Battery_EM80();
	
#if turboboostandstorage //storage mode action with support turbo boost charger

	if(testtoolflag==0)//Add flag for test tool to keep the battery RSOC at 60%
	{
		if(IS_MASK_SET(LENOVOPMFW,BATTERY_STORAGE) && IS_MASK_CLEAR(LENOVOPMFW,BATTERY_MAIN_CAL))
		{
			nRemainingCapL = Bat0x0FTempL;
			nRemainingCapH = Bat0x0FTempH;
		
			SET_MASK(LENOVOPMFW_Temp,BATTERY_CYCLE_RUNNING);
		
			if(nBattGasgauge >= 55)
			{
				if( IS_MASK_SET(ACOFF_SOURCE, BATTLEARN) )
				{ 
			    	ECSMI_SCIEvent(ACPI_BAT1IN_SCI); 
            	}
				CLEAR_MASK(ACOFF_SOURCE, BATTLEARN);
				BATT_LEN_OFF();
				//Change Status bit to avoid bit has been reset
            	if (IS_MASK_CLEAR(StatusKeeper, BatteryProtectCHG))
            	{
                	SET_MASK(nStopChgStat3H, EmStopChgarg);
            	}
            	else
            	{
                	CLEAR_MASK(nStopChgStat3H, EmStopChgarg);
                	if(nBattGasgauge == 60)
		        	{
						if(KeepBattRemineCap == 0)
						{ 
					    	KeepBattRemineCap = (WORD)(nRemainingCapH << 8) + nRemainingCapL; 
                    	}
						else
						{
							CalcBatRCC =(nFullChgCapH<<8)+nFullChgCapL;//Modify battery discharge when battery RSOC is 59% in storage mode.
							//Avoid that stop charger but OS show 59%
							if(((WORD)(nRemainingCapH << 8) + nRemainingCapL) > (KeepBattRemineCap + (CalcBatRCC/200)))//Modify battery discharge when battery RSOC is 59% in storage mode.
							{
								KeepBattRemineCap = 0;
								CLEAR_MASK(StatusKeeper, BatteryProtectCHG);
								ECSMI_SCIEvent(ACPI_BAT1IN_SCI); // Notify BIOS Update Batt Status
							}
						}
					}
            	}
				//ACOFF_LOW();
			
			}
			else if (nBattGasgauge < 55)
			{
				CLEAR_MASK(ACOFF_SOURCE, BATTLEARN);
				BATT_LEN_OFF(); //MosG25:add battery learn mode
				//Mos: Change Status bit to avoid bit has been reset
            	SET_MASK(StatusKeeper, BatteryProtectCHG);
				CLR_MASK(nStopChgStat3H,EmStopChgarg);
				//ACOFF_LOW();											// charge
			}
		}
		else 
		{
			//if (IS_MASK_SET(EC_BatteryStatusL,FullyChg)&&(IS_MASK_SET(SYS_STATUS,AC_ADP))) //MEILING025:remove.
			if (IS_MASK_SET(EC_BatteryStatusL,FullyChg)&&(IS_MASK_SET(SYS_STATUS,AC_ADP))&&(nBattGasgauge==100)) //MEILING025:add.
			{
				nRemainingCapL = nFullChgCapL;
				nRemainingCapH = nFullChgCapH;
			}
			else
			{
				//Mos: return RSOC(0x0d)=0 to BIOS if battery error
				if (ChkBattery_abnormal_status == ChkBattery_abnormal_status_error)
				{
					//nRemainingCapL = 0;  //LMLKBL0006:remove.
					//nRemainingCapH = 0;  //LMLKBL0006:remove.
				}
				else
				{
					nRemainingCapL = Bat0x0FTempL;
					nRemainingCapH = Bat0x0FTempH;
				}
			}

        	if(IS_MASK_SET(LENOVOPMFW_Temp,BATTERY_CYCLE_RUNNING))
        	{
            	CLEAR_MASK(LENOVOPMFW_Temp,BATTERY_CYCLE_RUNNING);

            	if(IS_MASK_SET(ACOFF_SOURCE,BATTLEARN))
            	{
                	CLEAR_MASK(ACOFF_SOURCE, BATTLEARN);
	 				BATT_LEN_OFF(); //MosG25:add battery learn mode
                	//ACOFF_LOW();
            	}
        	}

	 		//Mos: Clear BATTERY_CYCLE_fulchg when change to Longest Battery Mode
        	CLEAR_MASK(StatusKeeper, BatteryProtectCHG);
	 		CLR_MASK(nStopChgStat3H,EmStopChgarg);
    	}
	}
	else if(testtoolflag==1)
	{
		if (IS_MASK_SET(LENOVOPMFW,BATTERY_STORAGE) && IS_MASK_CLEAR(LENOVOPMFW,BATTERY_MAIN_CAL)) 
		{
			//Mos: return RSOC(0x0d)=0 to BIOS if battery error
			/*  
			if ((ChkBattery_abnormal_status == ChkBattery_abnormal_status_error)
				|| (Chk_Trickle_Current_status == Chk_Trickle_Current_status_error))
			{
				//nRemainingCapL = 0;
				//nRemainingCapH = 0;
				//Mos: Disable report RSOC(0x0d) = 0 to BIOS, need to modify spec
				nRemainingCapL = Bat0x0FTempL;
				nRemainingCapH = Bat0x0FTempH;
			}
			else
			*/  		
		
			nRemainingCapL = Bat0x0FTempL;
			nRemainingCapH = Bat0x0FTempH;
		
			SET_MASK(LENOVOPMFW_Temp,BATTERY_CYCLE_RUNNING);

			if (nBattGasgauge > 60)
			{
				SET_MASK(ACOFF_SOURCE, BATTLEARN);
				BATT_LEN_ON();	//add battery learn mode
				/*
                  	 	if(IS_MASK_SET(CHGIC_ReadCmd0x37L, TurboBoost)&&(nNowCurrentH & 0x80))
                   		{
                  			ACOFF_LOW();
					CLEAR_MASK(ACOFF_SOURCE, BATTLEARN);
				}
                   		else                      
					ACOFF_HI();		// discharge
				*/
			}
			else if (nBattGasgauge >= 55 && nBattGasgauge <= 60)
			{
				if ( IS_MASK_SET(ACOFF_SOURCE, BATTLEARN) )
				{ 
			    	ECSMI_SCIEvent(ACPI_BAT1IN_SCI); 
            	}
				CLEAR_MASK(ACOFF_SOURCE, BATTLEARN);
				BATT_LEN_OFF(); //Mos //add battery learn mode
				//Mos: Change Status bit to avoid bit has been reset
            	if (IS_MASK_CLEAR(StatusKeeper, BatteryProtectCHG))
            	{
                	SET_MASK(nStopChgStat3H, EmStopChgarg);
            	}
            	else
            	{
                	CLEAR_MASK(nStopChgStat3H, EmStopChgarg);
                	if (nBattGasgauge == 60)
		        	{
						if (KeepBattRemineCap == 0)
						{ 
					    	KeepBattRemineCap = (WORD)(nRemainingCapH << 8) + nRemainingCapL; 
                    	}
						else
						{
							CalcBatRCC =(nFullChgCapH<<8)+nFullChgCapL; //ANGELAS101:Modify battery discharge when battery RSOC is 59% in storage mode.
							//Mos: Avoid that stop charger but OS show god damn 59%...
							//if (((WORD)(nRemainingCapH << 8) + nRemainingCapL) > (KeepBattRemineCap + 10))//ANGELAS101:Modify battery discharge when battery RSOC is 59% in storage mode.
			     			if (((WORD)(nRemainingCapH << 8) + nRemainingCapL) > (KeepBattRemineCap + (CalcBatRCC/200)))//ANGELAS101:Modify battery discharge when battery RSOC is 59% in storage mode.
							{
								KeepBattRemineCap = 0;
								CLEAR_MASK(StatusKeeper, BatteryProtectCHG);
								ECSMI_SCIEvent(ACPI_BAT1IN_SCI); // Notify BIOS Update Batt Status
							}
						}
					}
            	}
				//ACOFF_LOW();											// charge
			}
			else if (nBattGasgauge < 55)
			{
				CLEAR_MASK(ACOFF_SOURCE, BATTLEARN);
				BATT_LEN_OFF(); //add battery learn mode
				//Mos: Change Status bit to avoid bit has been reset
            	SET_MASK(StatusKeeper, BatteryProtectCHG);
				CLR_MASK(nStopChgStat3H,EmStopChgarg);
				//ACOFF_LOW();											// charge
			}
		}
	
		else
		{
			//if (IS_MASK_SET(EC_BatteryStatusL,FullyChg)&&(IS_MASK_SET(SYS_STATUS,AC_ADP))) //MEILING025:remove.
			if (IS_MASK_SET(EC_BatteryStatusL,FullyChg)&&(IS_MASK_SET(SYS_STATUS,AC_ADP))&&(nBattGasgauge==100)) //MEILING025:add.
			{
				nRemainingCapL = nFullChgCapL;
				nRemainingCapH = nFullChgCapH;
			}
			else
			{
				//Mos: return RSOC(0x0d)=0 to BIOS if battery error
				if (ChkBattery_abnormal_status == ChkBattery_abnormal_status_error)
				{
					//nRemainingCapL = 0;  //LMLKBL0006:remove.
					//nRemainingCapH = 0;  //LMLKBL0006:remove.
				}
				else
				{
					nRemainingCapL = Bat0x0FTempL;
					nRemainingCapH = Bat0x0FTempH;
				}
			}

    		if(IS_MASK_SET(LENOVOPMFW_Temp,BATTERY_CYCLE_RUNNING))
    		{
        		CLEAR_MASK(LENOVOPMFW_Temp,BATTERY_CYCLE_RUNNING);

        		if(IS_MASK_SET(ACOFF_SOURCE,BATTLEARN))
        		{
            		CLEAR_MASK(ACOFF_SOURCE, BATTLEARN);
	 				BATT_LEN_OFF(); //add battery learn mode
            		// ACOFF_LOW();
        		}
    		}

			//Mos: Clear BATTERY_CYCLE_fulchg when change to Longest Battery Mode
    		CLEAR_MASK(StatusKeeper, BatteryProtectCHG);
			CLR_MASK(nStopChgStat3H,EmStopChgarg);
 		}
	}

#else

	if (IS_MASK_SET(LENOVOPMFW,BATTERY_STORAGE) && IS_MASK_CLEAR(LENOVOPMFW,BATTERY_MAIN_CAL)) //T075
	{
		//Mos: return RSOC(0x0d)=0 to BIOS if battery error
		/* 
		if ((ChkBattery_abnormal_status == ChkBattery_abnormal_status_error)
			|| (Chk_Trickle_Current_status == Chk_Trickle_Current_status_error))
		{
			//nRemainingCapL = 0;
			//nRemainingCapH = 0;
			//Mos: Disable report RSOC(0x0d) = 0 to BIOS, need to modify spec
			nRemainingCapL = Bat0x0FTempL;
			nRemainingCapH = Bat0x0FTempH;
		}
		else
		*/  		
		{
			nRemainingCapL = Bat0x0FTempL;
			nRemainingCapH = Bat0x0FTempH;
		}

    	SET_MASK(LENOVOPMFW_Temp,BATTERY_CYCLE_RUNNING);

		if (nBattGasgauge > 60)
		{
			SET_MASK(ACOFF_SOURCE, BATTLEARN);
			BATT_LEN_ON();	//add battery learn mode
        	
		}
		else if (nBattGasgauge >= 55 && nBattGasgauge <= 60)
		{
			if ( IS_MASK_SET(ACOFF_SOURCE, BATTLEARN) )
			{ 
				ECSMI_SCIEvent(ACPI_BAT1IN_SCI); 
        	}
			CLEAR_MASK(ACOFF_SOURCE, BATTLEARN);
			BATT_LEN_OFF(); //add battery learn mode
			// Change Status bit to avoid bit has been reset
        	if (IS_MASK_CLEAR(StatusKeeper, BatteryProtectCHG))
        	{
            	SET_MASK(nStopChgStat3H, EmStopChgarg);
        	}
        	else
        	{
           		CLEAR_MASK(nStopChgStat3H, EmStopChgarg);
            	if (nBattGasgauge == 60)
		    	{
					if (KeepBattRemineCap == 0)
					{ 
						KeepBattRemineCap = (WORD)(nRemainingCapH << 8) + nRemainingCapL; 
                	}
					else
					{
						CalcBatRCC =(nFullChgCapH<<8)+nFullChgCapL; //ANGELAS101:Modify battery discharge when battery RSOC is 59% in storage mode.
						//Mos: Avoid that stop charger but OS show god damn 59%...
						//if (((WORD)(nRemainingCapH << 8) + nRemainingCapL) > (KeepBattRemineCap + 10))//ANGELAS101:Modify battery discharge when battery RSOC is 59% in storage mode.
						if (((WORD)(nRemainingCapH << 8) + nRemainingCapL) > (KeepBattRemineCap + (CalcBatRCC/200)))//ANGELAS101:Modify battery discharge when battery RSOC is 59% in storage mode.
						{
							KeepBattRemineCap = 0;
							CLEAR_MASK(StatusKeeper, BatteryProtectCHG);
							ECSMI_SCIEvent(ACPI_BAT1IN_SCI); // Notify BIOS Update Batt Status
						}
					}
				}
        	}
			//ACOFF_LOW();											// charge
		}
		else if (nBattGasgauge < 55)
		{
			CLEAR_MASK(ACOFF_SOURCE, BATTLEARN);
			BATT_LEN_OFF(); //add battery learn mode
			//Mos: Change Status bit to avoid bit has been reset
        	SET_MASK(StatusKeeper, BatteryProtectCHG);
			CLR_MASK(nStopChgStat3H,EmStopChgarg);
			//ACOFF_LOW();											// charge	
		}
	}	
	else 
	{
		//if (IS_MASK_SET(EC_BatteryStatusL,FullyChg)&&(IS_MASK_SET(SYS_STATUS,AC_ADP))) //MEILING025:remove.
		if (IS_MASK_SET(EC_BatteryStatusL,FullyChg)&&(IS_MASK_SET(SYS_STATUS,AC_ADP))&&(nBattGasgauge==100)) //MEILING025:add.
		{
			nRemainingCapL = nFullChgCapL;
			nRemainingCapH = nFullChgCapH;
		}
		else
		{
			//Mos: return RSOC(0x0d)=0 to BIOS if battery error
			if (ChkBattery_abnormal_status == ChkBattery_abnormal_status_error)
			{
				//nRemainingCapL = 0;  //LMLKBL0006:remove.
				//nRemainingCapH = 0;  //LMLKBL0006:remove.
			}
			else
			{
				nRemainingCapL = Bat0x0FTempL;
				nRemainingCapH = Bat0x0FTempH;
			}
		}

    	if(IS_MASK_SET(LENOVOPMFW_Temp,BATTERY_CYCLE_RUNNING))
    	{
        	CLEAR_MASK(LENOVOPMFW_Temp,BATTERY_CYCLE_RUNNING);

        	if(IS_MASK_SET(ACOFF_SOURCE,BATTLEARN))
        	{
            	CLEAR_MASK(ACOFF_SOURCE, BATTLEARN);
	 			BATT_LEN_OFF(); //MosG25:add battery learn mode
            	// ACOFF_LOW();
        	}
    	}

		//Mos: Clear BATTERY_CYCLE_fulchg when change to Longest Battery Mode
    	CLEAR_MASK(StatusKeeper, BatteryProtectCHG);
		CLR_MASK(nStopChgStat3H,EmStopChgarg);
	}

#endif

}

void ChkLENOVOPMFW(void)
{
	if (IS_MASK_SET(LENOVOPMFW,BATTERY_MAIN_CAL))			// check Calibration Cycle enable
	{
		if (cBattFlag0.fbit.cBF0_FullToTarget == 0)
		{
		   	if (IS_MASK_CLEAR(LENOVOPMFW,BATTERY_CAL_END))
		   	{
				cBattFlag0.fbit.cBF0_FullToTarget = 1;		// charge to 100% then discharge to 1%
				cBattFlag0.fbit.cBF0_Full = 0;
				cBattFlag0.fbit.cBF0_GoTarget = 0;
				cTargetGauge = 1;
				//CLR_MASK(LENOVOPMFW,BATTERY_CYCLE);
		   	}
		}
	}
	else if (IS_MASK_SET(LENOVOPMFW,BATTERY_CAL_END))
	{
		CLR_MASK(LENOVOPMFW,BATTERY_CAL_END);
		cBattFlag0.fbit.cBF0_FullToTarget = 0;
		cBattFlag0.fbit.cBF0_Full = 0;
		cBattFlag0.fbit.cBF0_GoTarget = 0;
       	//Modify cancel the running of the gauge reset function, the battery can't resume charge. 
        CLEAR_MASK(ACOFF_SOURCE, BATTLEARN);
		BATT_LEN_OFF(); 
		//ACOFF_LOW();
	}
}

void ChkGoTarget(void)
{
	if (cBattFlag0.fbit.cBF0_GoTarget ==1)
	{
		if ((cTargetGauge == nBattGasgauge) ||  (nBattGasgauge < cTargetGauge))
		{
			CLEAR_MASK(ACOFF_SOURCE, BATTLEARN);
			BATT_LEN_OFF(); //add battery learn mode
			//ACOFF_LOW();											// charge
			cBattFlag0.fbit.cCmdAcOff = 0;
			if (IS_MASK_SET(LENOVOPMFW,BATTERY_MAIN_CAL))
			{
				SET_MASK(LENOVOPMFW,BATTERY_CAL_END);
				CLR_MASK(LENOVOPMFW,BATTERY_MAIN_CAL);
                cTargetGauge=0;
                SET_MASK(LENOVOPMFW_Temp,BATTERY_CALIBRATION_OK);   // release Calibration mode
                uVPCeventSource = General;
                uVPCeventSource2 = 0;
                ECSMI_SCIEvent(SDV_VPC_notify);
                ChkLENOVOPMFW();
			}
			return;
		}
		if (nBattGasgauge > cTargetGauge)
		{
			SET_MASK(ACOFF_SOURCE, BATTLEARN);
			BATT_LEN_ON();	//add battery learn mode
			//ACOFF_HI();		// discharge
			cBattFlag0.fbit.cCmdAcOff = 1;
		}
	}
	else if (cBattFlag0.fbit.cBF0_FullToTarget == 1)
	{
		if (cBattFlag0.fbit.cBF0_Full ==0)
		{
			if (IS_MASK_SET(nBattery0x16L,FullyChg))
			{
				cBattFlag0.fbit.cBF0_Full=1;
				cBattFlag0.fbit.cBF0_GoTarget = 1;					// Discharge or Charge to cTargetGauge%
			}
		}
	}
}

void ChkS3ResumeRSOC(void)
{
	/*if (IS_MASK_CLEAR(SYS_STATUS,AC_ADP))
	{
		if ( SystemIsS3 )
		{
			if (nBattGasgauge <= S3ResumeRSOC)	// check battery under 5%. //martin0216: meet spec: Battery RSOC<6%, and PC is in S3,System wakes up to S0 automatically(System wakes up from S3)
			{
            	             PWSeqStep = 1;
				PowSeqDelay = 1;
                          RamDebug(0x31);       
            	             SysPowState=SYSTEM_S3_S0;
			}
		}
	}*///ANGELAS093:-Modify battery RSOC below 5% can't into S3.
	//ANGELAS093:s+Modify battery RSOC below 5% can't into S3.
	if(IS_MASK_SET(battery_critical,DCdischargeto5ins3))
	{
		PWSeqStep = 1;
		PowSeqDelay = 1;
        RamDebug(0x37);  
        SysPowState=SYSTEM_S3_S0;
		CLEAR_MASK(battery_critical,DC0ver5enterS3);
		CLEAR_MASK(battery_critical,DCdischargeto5ins3);
	}
	//ANGELAS093:s+Modify battery RSOC below 5% can't into S3.
}
//ANGELAS093:s+Modify battery RSOC below 5% can't into S3.
void ChkS3DCRSOC(void)
{
	if(SystemIsS3)
	{
   		if(nBattGasgauge != S3ResumeRSOC)
		{
			SET_MASK(battery_critical, DC0ver5enterS3);
		}
   		else
   		{	  		
			if(IS_MASK_CLEAR(SYS_STATUS,AC_ADP)&&(nBattGasgauge== S3ResumeRSOC)&&IS_MASK_SET(battery_critical,DC0ver5enterS3))
			{
				SET_MASK(battery_critical,DCdischargeto5ins3);	
			}
			else
			{
				CLEAR_MASK(battery_critical,DCdischargeto5ins3);
			}
			//JIMGBRW025 CLEAR_MASK(battery_critical, DC0ver5enterS3);
		}
	}
	else
	{
		CLEAR_MASK(battery_critical, DC0ver5enterS3);
		CLEAR_MASK(battery_critical,DCdischargeto5ins3);
	}
}
//ANGELAS093:e+Modify battery RSOC below 5% can't into S3.
//ANGELAS043:Add start
void RSOC1Pto0P_ShutdownCheck(void)
{
    if(IS_MASK_SET(SYS_STATUS,AC_ADP)||Read_AC_IN()||nBattGasgauge > 0x01||SystemNotS0)
    {
       RSOC1PTO0PCount = 0x00;
	   RSOC1PTO0PSaveSpace = 0x00;
    }
	else
	{
	   if(nBattGasgauge == 0x01)
	   {
	      SET_MASK(RSOC1PTO0PSaveSpace,RSOCIs1P);
	   }
	   else if((nBattGasgauge == 0x00)&&IS_MASK_SET(RSOC1PTO0PSaveSpace,RSOCIs1P))
	   {
	      	if ( RSOC1PTO0PCount >= RSOC_Shut_Cnt )	// Read 30 times.
	        {
				ProcessSID(RSOC_1Pto0P_ID);  //Shutdown ID 0x40		
		
			    SET_MASK(SysStatus,ERR_ShuntDownFlag);	
		
				RSOC1PTO0PCount = 0x00;
	            RSOC1PTO0PSaveSpace = 0x00;
				//SET_MASK(USB_Charger, b2USB_TmlDis);	// Disable USB charger.//72JERRY046:clear Unexpected power shutdown flag when power on.
				RSMRST_LOW();
				Delay1MS(1);
				RSMRST_HI();				
	        }
	        else
	        { 
	            RSOC1PTO0PCount++; 
			}
	   }
	}
}
//ANGELAS043:Add end
void RSMRST_shutdown(void)					//shutdown (RSMRST HI-->low-->HI)

{
	RSMRST_LOW();					//shutdown (remark first)
	Delay1MS(1);
	RSMRST_HI();
	RSMshutdownCnt++;
}

void DownBatteryPState()
{
	if ((cBATTThrottling !=0)&&(BatteryAlarm == 0)&&(0x00==ADPIDON10MS_NUM)) //add ADPIDON10MS_NUM judge.
		cBATTThrottling--;
}

void UpBatteryPState()
{
	//if (cBATTThrottling != 0x0F)
	if (cBATTThrottling != PSTATE_MAXStep)//0ptimize CPU P_state (change 16 step to 8 step).
		cBATTThrottling++;
}

//MEILING055:S+ decrease GPU throttling every 2s after PnP AC.
void DownACtoBatteryGPUState()
{
	if((cGPUACtoBattThrottling !=0)&&(cGPUACtoBattTime==0))
	{
		if(IS_MASK_SET(SYS_STATUS,AC_ADP))
		{
			cGPUACtoBattThrottling=0;
		}
		else
		{
			cGPUACtoBattThrottling--;
			cGPUACtoBattTime = 4;//2s to do an action
			//ANGELAG017: add start
			if((nBattGasgauge > 20) && (cGPUACtoBattThrottling == 1))
			{
				cGPUACtoBattThrottling=0;
				cGPUACtoBattTime = 0;  ///////2s to do an action
			}
			else if((nBattGasgauge <= 20) && (cGPUACtoBattThrottling == 2))
			{
				cGPUACtoBattThrottling=0;
				cGPUACtoBattTime = 0;  ///////2s to do an action
			}
			////ANGELAG017: add end
		}
	}
}


//add start CPU prochot control function.
void CPUProchotCtrl()
{
    if(IS_MASK_CLEAR(Thro_Status, b0ProCH_CPU)&&IS_MASK_CLEAR(Thro_Status2, b3Batt_OTP+b7ProCH_Psys)&&(CPUProchotONCnt == 0))
	{	// turn off prochot.  //add prochot on 2S when AC out and battery over temperatrue and battery RSOC low.
	    H_PROCHOT_OFF(); 
	    nRealThermalPinDIS;
	    CLR_MASK(pPROCHOT, b0Thermal_PRCOHOTon);	// for AP display.
	}
	else
 	{	// turn on prochot.
	    H_PROCHOT_ON();
	    nRealThermalPinEN;
		SET_MASK(pPROCHOT, b0Thermal_PRCOHOTon);	// for AP display.
	}     
}

void ChkBattery_OTP()
{
	ITempW01 = (WORD)((EC_oCBTh<<8)+EC_oCBTl);		// get temperature
	if (ITempW01 > 2730)
	{
		nBattAverTemp = (ITempW01 - 2730) / 10;
	}
	else
	{
		return;
	}

	//LMLKBL0012:S+ add battery OTP shutdown protect in AC mode.
	if((SystemIsS0)&&(nBattAverTemp >= BatteryOTPShutdown))  //65
	{
		#if !EN_PwrSeqTest
        SET_MASK(SysStatus,ERR_ShuntDownFlag);//Add battery over temperature protect.
		ProcessSID(BATTOVERTEMP_ID);
		RSMRST_shutdown();
		#endif
	}
	//LMLKBL0012:E+.
	
	//if ((SystemIsS0)&&(IS_MASK_CLEAR(SYS_STATUS,AC_ADP)))     
	if ((SystemIsS0)&&(IS_MASK_SET(nBatteryStatH,CMBS_DISCHARGE)))     
	{
		//LMLKBL0012:S-.
		/*if(nBattAverTemp >= BatteryOTPShutdown)  //65
		{
			#if !EN_PwrSeqTest
            		SET_MASK(SysStatus,ERR_ShuntDownFlag);//Add battery over temperature protect.
			ProcessSID(BATTOVERTEMP_ID);
			RSMRST_shutdown();
			#endif
		}*/
		//LMLKBL0012:E-.
		
		if (nBattAverTemp >= BatteryOTP)				// 60
		{
			SET_MASK(nStopChgStat3L,ENOVERTEMP);
			SET_MASK(BatteryAlarm,BATOTP);
			//UpBatteryPState();  //MEILING048:remove.//MEILING051:-Add cpu P_STATE function.
			//nRemainingCapL = 0;  //LMLKBL0003:remove.
			//nRemainingCapH = 0;  //LMLKBL0003:remove.

			//MEILING033:add start prochot on and dGPU throttling set D5.
			cGPUBattOTPThrottling = 4;
            SET_MASK(Thro_Status2, b3Batt_OTP);
			SET_MASK(GPU_Prochot, b3_battery_OTP);
			//MEILING033:add end.

			//MEILING033:remove start.
			/*if (nBattOvrTempCnt > 80) 					// 2 min
			{
				#if !EN_PwrSeqTest
                		SET_MASK(SysStatus,ERR_ShuntDownFlag);//Add battery over temperature protect.
				ProcessSID(BATTOVERTEMP_ID);
				RSMRST_shutdown();
				#endif
			}
			else
				nBattOvrTempCnt ++;*/
			//MEILING033:remove end.
			
		}

		else if (nBattAverTemp < BatteryOTPRelease)			//58
		{
			CLR_MASK(BatteryAlarm,BATOTP);
			CLR_MASK(nStopChgStat3L,ENOVERTEMP);
			//nBattOvrTempCnt =0;  //MEILING033:remove.

			//MEILING033:add start prochot off and dGPU throttling set D1.
			cGPUBattOTPThrottling = 0; 
            CLEAR_MASK(Thro_Status2, b3Batt_OTP);
			CLR_MASK(GPU_Prochot, b3_battery_OTP);
			//MEILING033:add end.
			
		}
	}
	else
	{
		CLR_MASK(BatteryAlarm,BATOTP);
		CLR_MASK(nStopChgStat3L,ENOVERTEMP);
		CLEAR_MASK(Thro_Status2, b3Batt_OTP); //MEILING033:add.
		cGPUBattOTPThrottling = 0; //MEILING033:add dGPU throttling to D1.
		CLR_MASK(GPU_Prochot, b3_battery_OTP); 
		//nBattOvrTempCnt =0;  //MEILING033:remove.
	}
}

//T069 + s
//////////////////////////////////////////////////////
// check battery out power over 28W
//////////////////////////////////////////////////////
void ChkBattery_OverConsumption()  //MEILING033:modify function name.
{
    WORD BattPresentVolt; 
	WORD BattNowCurrent; 
    BYTE Power_Temp;
    
	if ((!Read_AC_IN()) || (IS_MASK_SET(CHGIC_ReadCmd0x12L,BatLearnEnable))) //MEILING033:add learn mode condition.
	{ 
        if (nNowCurrentH & 0x80)
		{		   
		    BattNowCurrent=0xFFFF - (WORD)((nNowCurrentH<<8)+nNowCurrentL);
            BattPresentVolt = (WORD)((nPresentVoltH << 8) + nPresentVoltL);
            Power_Temp=(BYTE) ((BattPresentVolt*0.1/100) *( BattNowCurrent*0.1/100));     
            Chk_Hybrid_STPP_Batt_Output_Power =(LWORD)( Power_Temp*1000000 /292968.75);  
			XWTemp1 = 0xFFFF - (WORD)((nNowCurrentH<<8)+nNowCurrentL);
			
			//MEILING049:S+Modify power limit under DC mode to improve performance follow EC v16.
            if(Power_Temp >= SysPower_ProchotOn) //MEILING033:change 45W to 28W.
            {  
            	cGPUBattThrottling = 4; //MEILING033:add dGPU throttling to D5.
                SET_MASK(Thro_Status, b7ProCH_BATT); 
			}
			//MEILING042:S+ add GPU throttling according to system consumption.
			else if( (Power_Temp < SysPower_ProchotOn) && (Power_Temp >= SysPower_GPUThrottling) )
			{
				//MEILING050:s+Distinguish between 310 or 510.
				if(IS_MASK_SET(pProject0,b7GSKL310OR510))//310
				{
					cGPUBattThrottling = 2; 
					cBATTThrottling = 9;//MEILING051:Add cpu P_STATE function.
				}
				else
				{
					cGPUBattThrottling = 3; 
					cBATTThrottling = 7;//MEILING053:add.
				}
                CLR_MASK(Thro_Status, b7ProCH_BATT);
				//MEILING050:e+Distinguish between 310 or 510.
			}
			else if( (Power_Temp < SysPower_GPUThrottling) && (Power_Temp >= SysPower_ProchotOff) )
			{
				//MEILING050:s+Distinguish between 310 or 510.
				if(IS_MASK_SET(pProject0,b7GSKL310OR510))
				{
					cGPUBattThrottling = 1; 
					cBATTThrottling = 0; //MEILING053:add.
				}
				else
				{
					cGPUBattThrottling = 2; 
					cBATTThrottling = 0; //MEILING053:add.
				}
                CLR_MASK(Thro_Status, b7ProCH_BATT);
				//MEILING050:e+Distinguish between 310 or 510.
			}
			//MEILING042:E+.
            else if(Power_Temp < SysPower_ProchotOff)  //MEILING033:change 35W to 20W.
            { 
            	cGPUBattThrottling = 0; //MEILING033:add dGPU throttling to D1.
            	cBATTThrottling = 0; //MEILING051:Add cpu P_STATE function.
                CLR_MASK(Thro_Status, b7ProCH_BATT); 
			}
       	}       
	}
	else
	{
		cGPUBattThrottling = 0; //MEILING033:add dGPU throttling to D1.
		cBATTThrottling = 0; //MEILING053:add.
        CLR_MASK(Thro_Status, b7ProCH_BATT);
	}
}
//MEILING049:E+Modify power limit under DC mode to improve performance follow EC v16.

void ChkBattery_OCP()
{
//ANGELAG017: remove start
/*	if (!Read_AC_IN())
	{
		if (nNowCurrentH & 0x80)
		{
			XWTemp1 = 0xFFFF - (WORD)((nNowCurrentH<<8)+nNowCurrentL);

			if (XWTemp1 > OCPCapacity)
			{
				SET_MASK(BatteryAlarm,BATOCP);
				//UpBatteryPState();  //MEILING048:remove.//MEILING051:-Add cpu P_STATE function.
			}
			if (XWTemp1 < OCPCapacityRelease)
			{
				CLR_MASK(BatteryAlarm,BATOCP);
			}
		}
	}
	else
	{
		CLR_MASK(BatteryAlarm,BATOCP);
	}*/
	//ANGELAG017: remove end
	////ANGELAG017: add start
	if ((SystemIsS0) && (!Read_AC_IN()))
	{
		if (Bat0x00TempL&0x20)				// 0x00  bit5
		{ 
			BatteryOCPDelay=10; //last at least 1s
			cGPUBattOCPThrottling = 4; //set DGPU D5
		    SET_MASK(LENOVOBATT2,BATTERY_OCP); //battery OCP CPU prochot
	    	}
		else
		{ 
			if(BatteryOCPDelay==0)
			{
				cGPUBattOCPThrottling = 0; //set DGPU D1
		    		CLR_MASK(LENOVOBATT2,BATTERY_OCP); 
			}
	    	}
	}
	else
	{
	  	if(BatteryOCPDelay==0)
		{
			cGPUBattOCPThrottling = 0; //set DGPU D1
	    		CLR_MASK(LENOVOBATT2,BATTERY_OCP); 
		}
	}
	//ANGELAG017: add end
}


void ChkBattery_LowVoltage()
{
    WORD CV1,CV2,CV3,CV4;
	
	if (!Read_AC_IN())
	{
		if (nNowCurrentH & 0x80)
		{
			if(bRSMBusBlock(SMbusChB, SMbusRBK, SmBat_Addr, C_Mdata, &BAT1_MD_1))
            {
				
		    	CV4 = (BAT1_MD_6 << 8)+BAT1_MD_5;   // Get Main battery desgin voltage   //MEILING016:remove.  //MEILING017:add.
		        CV3 = (BAT1_MD_8 << 8)+BAT1_MD_7;   // Get Main battery desgin voltage   //MEILING016:remove.  //MEILING017:add.
		        CV2 = (BAT1_MD_A << 8)+BAT1_MD_9;   // Get Main battery desgin voltage  //MEILING016:change CV3 to CV2.  
		        CV1 = (BAT1_MD_C << 8)+BAT1_MD_B;   // Get Main battery desgin voltage  //MEILING016:change CV4 to CV1.  
		        //if ((CV1 < 3000) || (CV2 < 3000) || (CV3 < 3000) || (CV4 < 3000) )  //change 4th && to ||  //MEILING016:remove.
				//if ((CV1 < 3000) || (CV2 < 3000)) //MEILING016:add.  //MEILING017:remove.
				if ((CV1 < 3000) || (CV2 < 3000) || ((CellCount>=3) && (CV3 < 3000)) || ((CellCount==4) && (CV4 < 3000)) ) //MEILING017: add
				{
					if(SystemIsS0)
                    {
                        RamDebug(0xB1);
                        //ECQEvent(0x27);                              
                        SET_MASK(nBatteryStatH, CMBS_CRITICALLOW);                            
                    }            
				}
              	else
                {
                   	CLR_MASK(nBatteryStatH, CMBS_CRITICALLOW);  
                }
 

		        //if ((CV1 < 3300) || (CV2 < 3300) || (CV3 < 3300) || (CV4 < 3300) )  //MEILING017:remove.
		        if ((CV1 < 3000) || (CV2 < 3000) || ((CellCount>=3) && (CV3 < 3000)) || ((CellCount==4) && (CV4 < 3000)) ) //MEILING017: add
				{
					SET_MASK(BatteryAlarm,BATLOWVOL);
					//UpBatteryPState();  //MEILING048:remove.//MEILING051:-Add cpu P_STATE function.
				}
                else 
				{
					CLR_MASK(BatteryAlarm,BATLOWVOL);
				}      
            }   
		}
	}
	else
	{
		CLR_MASK(BatteryAlarm,BATLOWVOL);
        CLR_MASK(nBatteryStatH, CMBS_CRITICALLOW);     
	}
}


//MEILING033: add start check battery RSOC low function.
void CkkBattery_RSOCLow()
{
	if ((!Read_AC_IN()) || (IS_MASK_SET(CHGIC_ReadCmd0x12L,BatLearnEnable)))
	{
		if(nBattGasgauge < BattRSOC_ProchotOn) //turn on prochot and dGPU throttling to D5.
		{
			cGPUBattLowThrottling = 4; //ANGELAG013: D4 to D5 //MEILING055:change D5 to D4.
			//cBATTLowThrottling=7; //ANGELAG013: remove //MEILING055:add.
            //SET_MASK(Thro_Status2, b6BattRSOC_Low);   //MEILING039:add.
		}
		else if(nBattGasgauge >= BattRSOC_ProchotOn) //turn off prochot and dGPU throttling to D1.
		{
			cGPUBattLowThrottling = 0; 
            //CLEAR_MASK(Thro_Status2, b6BattRSOC_Low);  //MEILING039:add.
		}
	}
	else
	{
		cGPUBattLowThrottling = 0; 
		cBATTLowThrottling=0; //MEILING055:add.
        //CLEAR_MASK(Thro_Status2, b6BattRSOC_Low);  //MEILING039:add.
	}
}
//MEILING033:add end.

void ChkBattery_FCCchg()
{
	ChkBattery_FCCchg_count++;
	if (ChkBattery_FCCchg_count >= 100) //Mos: 10 Sec(100ms 100times)
	{
		ChkBattery_FCCchg_count = 0;
		ChkBattery_FCCchg_count2++;
		if(ChkBattery_FCCchg_count2 < 30)
		{
		  return;
		}
		ChkBattery_FCCchg_count2 = 0x00;
		//Mos: Load default
		if ((ChkBattery_FCCchg_lastFCCL == 0) && (ChkBattery_FCCchg_lastFCCH == 0))
		{
			ChkBattery_FCCchg_lastFCCL = nFullChgCapL;
			ChkBattery_FCCchg_lastFCCH = nFullChgCapH;
		}

		//Mos: Check FCC and Notify OS if FCC change for each 10 sec
		if ((ChkBattery_FCCchg_lastFCCL != nFullChgCapL)
			|| (ChkBattery_FCCchg_lastFCCH != nFullChgCapH))//change "&&" to "||"
		{
			ECSMI_SCIEvent(ACPI_BAT1IN_SCI);
			ChkBattery_FCCchg_lastFCCL = nFullChgCapL;
			ChkBattery_FCCchg_lastFCCH = nFullChgCapH;
		}
	}
}

void ChkAvgCurrent()
{
	//Mos: Modify for meet specification
	//Average current report to OS
	//Timer<=60 seconds(The timer starts counting when AC adapter plug out.)
	//Report "0x00" to EC name space 0xd2, 0xd3 by one time, and then
	//Report battery Current(0x0a) to EC name space 0xd2, 0xd3
	//Reset condition:When Timer>60 seconds,Report battery AverageCurrent(0x0b) to EC name space 0xd2, 0xd3
	if (Bat0x0BTempH & 0x80)
	{
		XWTemp1 = 0xFFFF - (WORD)((Bat0x0BTempH<<8)+Bat0x0BTempL);	
		if (XWTemp1 < 400)
		{
			return;
		}
	}	
	// Filter the dirty data because when AC not exit, there will only be discharge current.
  	if(IS_MASK_CLEAR(SYS_STATUS,AC_ADP) && SystemIsS0 && IS_MASK_CLEAR(Bat0x0BTempH,BIT7)) //change 'nNowCurrentH' to 'Bat0x0BTempH'
    {
   
         if(IS_MASK_SET(nNowCurrentH,BIT7))
        {
             nAvgCurrentL = nNowCurrentL;
		     nAvgCurrentH = nNowCurrentH;
        }  		 
        return;
    }
  	// Filter the dirty data because when AC not exit, there will only be discharge current.
	if ( Bat0x0BFakeCnt == 0)
	{
		nAvgCurrentL = Bat0x0BTempL;
		nAvgCurrentH = Bat0x0BTempH;
	}
	else if(Bat0x0BFakeCnt < 60)
	{
		nAvgCurrentL = nNowCurrentL;
		nAvgCurrentH = nNowCurrentH;
	}
	else if(Bat0x0BFakeCnt == 60)
	{
		nAvgCurrentL = 0;
		nAvgCurrentH = 0;
	}
}

void RST_ChgTimeOutCnt(void)
{
	TrickleChgTimeOutCnt = 0;
	FastChgTimeOutCnt = 0;
	CLEAR_MASK(nStopChgStat3L,ENSTOPCHG);
	CLEAR_MASK(nStopChgStat3H,ENTRITIMEOUT); //MARTINO006:change 'nStopChgStat3L' to 'nStopChgStat3H'
}

void Battery100ms(void)
{
	if (inhibit2sec!=0)
		inhibit2sec--;
	else
		CLEAR_MASK(nStopChgStat3L,ENCHARGESUSP);		// use for detect battery charging suspend
}
void ChkPsys(void) // HEGANGS006£ºModify protect setting
{
	if(Read_AC_IN()) // HEGANGS032:Modify power setting 
	{
		if(nBattGasgauge>=5)
		    cGPUBattThrottling = 0;
		else
		    cGPUBattThrottling = 2;
		return;
	}
	
	if(Proch_Delay1>0)
	    Proch_Delay1--;
	if(Proch_Delay1>10)
	    Proch_Delay1=10;	
	    
	if ((SystemIsS0) && (!Read_AC_IN()) && (cGPUACtoBattTime == 0))		//DC Mode
	{
		if(CellCount==2)
		{
			if (nBattGasgauge > 20)
			{
				if (Psys_AvgData >= 403) //1.26V  63W //1.18 = 403
				{	
					SET_MASK(GPU_Prochot, b1_Psys);  // D5
					SET_MASK(Thro_Status2, b7ProCH_Psys);
					Proch_Delay1=10;
				}
				else if ((Psys_AvgData >= 355) && (Psys_AvgData < 403)) //54W--63W 
				{
					if(Proch_Delay1==0)
						{
					cGPUBattThrottling = 3; 	//D4
					CLR_MASK(GPU_Prochot, b1_Psys); 
					CLR_MASK(Thro_Status2, b7ProCH_Psys);
						}
				}
				else if(Psys_AvgData < 355)   //less than 54W, 1.08V   //1.04
				{
					if(Proch_Delay1==0)
						{
					cGPUBattThrottling = 2; //D3
					CLR_MASK(GPU_Prochot, b1_Psys);
					CLR_MASK(Thro_Status2, b7ProCH_Psys);
					}
				}
			}
			else if (nBattGasgauge > 7)	//7%-20%
			{
				if (Psys_AvgData >= 410) //1.26V  63W(1.26*1024/3=430) //1.2
				{
					SET_MASK(GPU_Prochot, b1_Psys); //D5
					SET_MASK(Thro_Status2, b7ProCH_Psys);
				}
				else if(Psys_AvgData < 403)	//54W, 1.08V //1.18
				{
					cGPUBattThrottling = 3; //D4
					CLR_MASK(GPU_Prochot, b1_Psys);
					CLR_MASK(Thro_Status2, b7ProCH_Psys);
				}
			}
			else		//0%-7%
			{
				cGPUBattThrottling = 4; //D5
				SET_MASK(GPU_Prochot, b1_Psys); 
				if (Psys_AvgData >= 246)    //36W
				{
					SET_MASK(Thro_Status2, b7ProCH_Psys);
				}
				else if(Psys_AvgData < 246)	
				{
					CLR_MASK(Thro_Status2, b7ProCH_Psys);
				}
			}
		}
		else if(CellCount==3)
		{
			if (Psys_AvgData >= 314)    //0.96v 48w //0.9=307 //314 0.92 46w
			{
				SET_MASK(GPU_Prochot, b1_Psys); 
				SET_MASK(Thro_Status2, b7ProCH_Psys);
			}
			else if(Psys_AvgData < 273)	//0.84 42w //0.8=273
			{
				CLR_MASK(GPU_Prochot, b1_Psys);
				CLR_MASK(Thro_Status2, b7ProCH_Psys);
			}
		}
	}
	else
	{
		cGPUBattThrottling= 0;
		CLR_MASK(GPU_Prochot, b1_Psys);
		CLR_MASK(Thro_Status2, b7ProCH_Psys);
	}

}
void TurboDCOnly(void) 
{
	if ((SystemIsS0) && (!Read_AC_IN())) //HEGANGS013:Modify  cpu turbo setting
	{
		if (nBattGasgauge >= 70)
		{
			if (VGA_TBuff3 >= 51)				// Check turbo resume.
			{ 
				SET_MASK(Thro_Status, b6Turbo_DC_CPU);
			}
			else if(VGA_TBuff3 < 47)
			{
				CLR_MASK(Thro_Status, b6Turbo_DC_CPU); 
			}
		}
		else
		{
			if((VGA_TBuff3 >= 51)||(TEMP_Buff_3>=65))		// Check turbo Off.
			{ 
				SET_MASK(Thro_Status, b6Turbo_DC_CPU);
			}
			else if((VGA_TBuff3 < 47)&&(TEMP_Buff_3 < 60))
			{
				CLR_MASK(Thro_Status, b6Turbo_DC_CPU); 
			}
		}
	}
	else
	{
		CLR_MASK(Thro_Status,b6Turbo_DC_CPU); 
	}
	if ( IS_MASK_CLEAR(Thro_Status, b3Turbo_CPU+b6Turbo_DC_CPU ))
	{
		if ( IS_MASK_SET(uVGATurboFun,uDisCPUTurboOK) )		
		{
			CLR_MASK(uVGATurboFun, uDisCPUTurboOK);	
            ECQEvent(EN_CPUTURBO_67);  
		}
	}
	else
	{
	 	if ( IS_MASK_CLEAR(uVGATurboFun,uDisCPUTurboOK) )	// bit3.
	 	{
			SET_MASK(uVGATurboFun, uDisCPUTurboOK);			// Set bit3. 
            ECQEvent(DIS_CPUTURBO_66);  
	 	}
	}
}
void Battery1Sec(void)
{
	//DownBatteryPState();  //MEILING048:remove.//MEILING051:-Add cpu P_STATE function.
	if (Bat0x0BFakeCnt != 0)
		Bat0x0BFakeCnt--;

	if (BatSMbusFailCount != 0)
	{
    	SET_MASK(Bat1_FPChgFlag,BIT(0));
    	nBattErrorCnt++;
        if (nBattErrorCnt > 250)
    		nBattErrorCnt = 251;
    	if (BatSMbusFailCount > 220)
    		BatSMbusFailCount = 221;
	}
	else
	{
		nBattErrorCnt = 0;
		if (nNowCurrentL|nNowCurrentH)			// if  nNowCurrent != 0
		{
			if ((nNowCurrentH&0x80) == 0)		// nNowCurrent > 0, charging
			{								//current(0x0A)>0mA&polling time10cycles
				nBattTempCnt ++;
				if (nBattTempCnt >= 10)
					nBattTempCnt = 11;			// nBattTempCnt > 10
			}
			else								// discharging
			{
				nBattTempCnt = 0;
				RST_ChgTimeOutCnt();
			}
		}
		else
		{
			nBattTempCnt = 0;
		}
	}
}


const sRSmbusBStruct code ABatCommandTable [] =
{//     command               REG                      no.
	{ C_Date,		&nManufactureDateL		,0x00},				//Batpollstep1 = 0    	WORD
	{ C_Dchem,		&batteryChemistry		,0x04},				//Batpollstep1 = 1    	BLOCK
	{ C_Mname,		&BATTMANUFACTURE		,0x0A},				//Batpollstep1 = 2    	BLOCK
	{ C_Dname,		&BATTDEVICENAME			,0x0A},				//Batpollstep1 = 3		BLOCK
	{ C_OpMfgFun5,  &BAT1_Bar_Code          ,0x0A},             //Batpollstep1 = 4       BLOCK  //MEILING031:add. 
	{ C_Date,		&nManufactureDateL		,0x00},				//Batpollstep1 = 5  	WORD
	{ C_DCap,	    &nDesignCapL			,0x00},				//Batpollstep1 = 6  	WORD    0~8 initial only
	{ C_DVolt,		&nDesignVoltL			,0x00},				//Batpollstep1 = 7  	WORD
	{ C_SerialNo,	&nSerialNumL			,0x00},				//Batpollstep1 = 8  	WORD
	{ C_mode,		&EC_C_modeL				,0x00},				//Batpollstep1 = 9		WORD
	{ C_ChargingV,  &nChargingVoltL			,0x00},				//Batpollstep1 = 10  	WORD
	{ C_CycleCount,	&EC_oCCBQl				,0x00},				//Batpollstep1 = 11  	WORD
	{ C_volt,		&nPresentVoltL			,0x00},				//Batpollstep1 = 12  	WORD
	{ C_current,	&nNowCurrentL			,0x00},				//Batpollstep1 = 13  	WORD
	{ C_ChargingI,  &nBattCharCurrentL      ,0x00},				//Batpollstep1 = 14  	WORD
	{ C_BatStatus,	&nBattery0x16L			,0x00},				//Batpollstep1 = 15  	WORD
	{ C_RMcap,		&Bat0x0FTempL			,0x00},				//Batpollstep1 = 16  	WORD    9~19 normal polling
	{ C_current,	&nNowCurrentL			,0x00},				//Batpollstep1 = 17  	WORD
	{ C_FCcap,		&nFullChgCapL			,0x00},				//Batpollstep1 = 18 	WORD
	{ C_temp,		&EC_oCBTl				,0x00},				//Batpollstep1 = 19 	WORD
	{ C_RSOC,	 	&BAT1PERCL  			,0x00},				//Batpollstep1 = 20 	WORD
	{ C_current,	&nNowCurrentL			,0x00},				//Batpollstep1 = 21  	WORD
	{ C_AVcurrent,	&Bat0x0BTempL			,0x00}, 			//Batpollstep1 = 22 	WORD
	{ C_LVMfgFun2,	&Bat0x3ETempL			,0x00},				//Batpollsetp1 = 23     WORD
	{ C_access,		&Bat0x00TempL			,0x00}, 			//Batpollsetp1 = 24 	WORD
	{ C_current,	&nNowCurrentL			,0x00},				//Batpollstep1 = 25  	WORD
	{ C_RSOC,	 	&BAT1PERCL  			,0x00},				//Batpollstep1 = 26 	WORD  //Add read battery RSOC at bottom of table.
	//{ C_D_FET,	&SHIPMODE_L			,0x00}, 				//Batpollstep1 = 27 	WORD

};

//-----------------------------------------------------------------------------
// get 1 battery data
//-----------------------------------------------------------------------------
void GetBatData(BYTE _STEP)
{
	switch (_STEP)
    {
    	case 1:	//block data
    	//Mos: Break C_Dchem directly
    	//Cause That memory overlap with C_Dname, it will cause Data unstable, I can't get same data 3 times
    			//break;//ANGELAS107:-Add code for GBSI function.
		case 2: //block data
		case 3: //block
		case 4: //block data //MEILING031:add.
			if(	!bRSMBusBlock(SMbusChB, SMbusRBK, SmBat_Addr,
				ABatCommandTable[_STEP].cmd,
				ABatCommandTable[_STEP].smbdataA))
			{	// SMBUS FAIL
				BatSMbusFailCount++;
				//ResetSMBus(SMbusChB); //MEILING002: add  //MEILING014:remove.
			}
			else
			{	// SMBUS OK
				BatSMbusFailCount=0;
			}
			break;

		default: //word data
			if(!bRWSMBus(SMbusChB, SMbusRW, SmBat_Addr,
			   ABatCommandTable[_STEP].cmd,
			   ABatCommandTable[_STEP].smbdataA,
			   SMBus_NoPEC))
			{	// SMBUS FAIL
				BatSMbusFailCount++;
				//ResetSMBus(SMbusChB); //MEILING002: add  //MEILING014:remove.
			}
			else
			{	// SMBUS OK
				BatSMbusFailCount=0;
			}

		        break;
	}
}


void ChkBattery_Percl()
{
    bRWSMBus(SMbusChB, SMbusRW, SmBat_Addr,C_RSOC,&BAT1PERCL ,SMBus_NoPEC);
}

//-----------------------------------------------------------------------------
// battery plug in first fast update all information
//-----------------------------------------------------------------------------
void FirstGetBatData(void)
{
  	BYTE i,j;
	WORD DesignVoltage; //MEILING017: add

  	Batpollstep1 = 0;
  	nBatteryStatL = 0;
  	for (i=0;i<(sizeof(ABatCommandTable)/4);i++)
  	{
    	GetBatData(Batpollstep1);
    	Batpollstep1++;
  	}
    Batpollstep1=8;
    SHA1_SEED = (WORD)((TL0+Bat0x0FTempL)<<8);

	//Mos: Move to OEM_PollingBatData_TASK() to make sure Battery data stable.
	/*if( ( BATTMANUFACTURE[0] == 'S' ) && ((BATTMANUFACTURE[1] == 'A' )||( BATTMANUFACTURE[1] == 'a' )) )
	    nBatteryStatL |= 0x10 ;	// SANYO
	else if( (BATTMANUFACTURE[0] == 'S' ) && ( BATTMANUFACTURE[1] == 'O' ) )
	    nBatteryStatL |= 0x20 ;	// Sony
	else if( (BATTMANUFACTURE[0] == 'P' ) && (( BATTMANUFACTURE[1] == 'A' )|| ( BATTMANUFACTURE[1] == 'a' )) )
		nBatteryStatL |= 0x40 ;	// Panasonic
	else if( (BATTMANUFACTURE[0] == 'S' ) && ( BATTMANUFACTURE[1] == 'U' ) )
		nBatteryStatL |= 0x50 ;	// Samsung
	else if( (BATTMANUFACTURE[0] == 'L' ) && ( BATTMANUFACTURE[1] == 'G' ) )
		nBatteryStatL |= 0x30 ;	// LG
	else if( (BATTMANUFACTURE[0] == 'C' ) && (( BATTMANUFACTURE[1] == 'P' ) || ( BATTMANUFACTURE[1] == 'p' )) )
		nBatteryStatL |= 0x60;	// CPT Celxpert
	else if( (BATTMANUFACTURE[0] == 'S' ) && (BATTMANUFACTURE[1] == 'M' ) )
		nBatteryStatL |= 0x70;	// Simplo*/
	
 	//ANGELAS107:S+Add code for GBSI function.
    if((batteryChemistry[0]=='L')&&(batteryChemistry[1]=='I')&&(batteryChemistry[2]=='O')&&(batteryChemistry[3]=='N'))
		SET_MASK(nBatteryStatL,CMBS_BATTTYPE);	
	else
	    CLEAR_MASK(nBatteryStatL,CMBS_BATTTYPE);
    //ANGELAS107:E+Add code for GBSI function.

	OCPCapacity = (WORD)((nDesignCapH<<8)+nDesignCapL);
	OCPCapacity = OCPCapacity *8;
	//Mos: According Batt - Patrick request, reduce Over Current Point from 0.8 DesignCap to 0.78 DesignCap.
	//T059- OCPCapacity = OCPCapacity - (OCPCapacity * 2 / 10);
	//T059- OCPCapacity = OCPCapacity / (WORD)((nDesignVoltH<<8)+nDesignVoltL);
	//T059- OCPCapacity = OCPCapacity * 1000;

    OCPCapacity =(WORD)((LWORD)OCPCapacity*1000 / (WORD)((nDesignVoltH<<8)+nDesignVoltL));

	OCPCapacityRelease = (WORD)((nDesignCapH<<8)+nDesignCapL);
	OCPCapacityRelease = OCPCapacityRelease *7;

    OCPCapacityRelease =(WORD)((LWORD)OCPCapacityRelease*1000 / (WORD)((nDesignVoltH<<8)+nDesignVoltL));

	//MEILING017: add start
   	DesignVoltage=(WORD)((nDesignVoltH<<8)+ nDesignVoltL);
   	if((DesignVoltage >= 14000) && (DesignVoltage <= 15200))  //Cell: 4series   14V~15.2V
		CellCount=4;
	else if((DesignVoltage >= 9000) && (DesignVoltage <= 13200)) //Cell: 3series   10.5V~11.4V
		CellCount=3;
	else if((DesignVoltage >= 6000) && (DesignVoltage <= 8800)) //Cell: 2series   7V~7.6V
		CellCount=2;
	//MEILING017: add end

	S3ResumeRSOC = S3RSOCPercentage;		// Set S3 resuem in the battery under 5%.
	BatteryOTP = BatteryOTPHi;
	BatteryOTPRelease = BatteryOTPLow;
	BatteryOTPShutdown = BatteryOTPSD;


	UpdateNameSpace();
	Chk_Battery_Full();
	nBattErrorCnt = 0;
	SHA1_SEED = SHA1_SEED + (WORD)(TL0);
	srand(SHA1_SEED);

	if (BatSMbusFailCount==0)
	{
		Battdata_ready = 1;
		WSMbusTemp01 = EC_C_modeH;
		if ((WSMbusTemp01^0xE0)&0xE0)				// check bit15,bit14,bit13=1  ??
		{
			WSMbusTemp01=EC_C_modeL;
			WSMbusTemp02=EC_C_modeH;
			WSMbusTemp02|=0xE0;
			if(!bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, C_mode, &WSMbusTemp01,SMBus_NoPEC))
			{
				BatSMbusFailCount++;
			}
		}
	}
}

void Chk_Battery_Full(void)
{
	if (IS_MASK_CLEAR(SYS_STATUS,AC_ADP)||(IS_MASK_SET(SYS_STATUS,AC_ADP)&&IS_MASK_SET(ACOFF_SOURCE,BATTLEARN)))
	{
    	CLEAR_MASK(SEL_STATE0,CHARGE_A);		//clear  battery charging flag
    	CLEAR_MASK(nBatteryStatH,CMBS_CHARGE);	//clear  battery charging flag
    	SET_MASK(nBatteryStatH,CMBS_DISCHARGE);	//set battery discharging flag
    	return;
	}

	//if ((cBattFlag0.fbit.cCmdAcOff==1)||Read_ACOFF()||IS_MASK_SET(EC_BatteryStatusL,FullyChg)||((nStopChgStat3L|nStopChgStat3H)!=0))
	if ((cBattFlag0.fbit.cCmdAcOff==1)||IS_MASK_SET(EC_BatteryStatusL,FullyChg)||((nStopChgStat3L|nStopChgStat3H)!=0))
	{
		CLEAR_MASK(SEL_STATE0,CHARGE_A);			        //clear  battery charging flag
		CLEAR_MASK(nBatteryStatH,CMBS_CHARGE);		//clear  battery charging flag
		CLEAR_MASK(nBatteryStatH,CMBS_DISCHARGE);   	//clear battery discharging flag
		return;
	}
	else
	{
		if ((cBattFlag0.fbit.cBF0_GoTarget ==1) && (cTargetGauge == nBattGasgauge))
		{
			CLEAR_MASK(SEL_STATE0,CHARGE_A);			//clear  battery charging flag
			CLEAR_MASK(nBatteryStatH,CMBS_CHARGE);		//clear  battery charging flag
			CLEAR_MASK(nBatteryStatH,CMBS_DISCHARGE);	//clear battery discharging flag
			return;
		}
	}

	if (IS_MASK_CLEAR(LENOVOPMFW,BATTERY_STORAGE))  
	{
		if (IS_MASK_SET(nBattery0x16L,FullyChg))	// || IS_MASK_SET(CHGIC_ReadCmd0x12L,ChargeInhibit))
		{
			CLEAR_MASK(SEL_STATE0,CHARGE_A);  			//clear  battery charging flag
			CLEAR_MASK(nBatteryStatH,CMBS_CHARGE);  	//clear  battery charging flag
			SET_MASK(nBatteryStatH,CMBS_DISCHARGE); 	//set battery discharging flag
		}
		else
		{
			SET_MASK(SEL_STATE0,CHARGE_A);  			//set battery charging flag
			SET_MASK(nBatteryStatH,CMBS_CHARGE);  		//set battery charging flag
			CLEAR_MASK(nBatteryStatH,CMBS_DISCHARGE); 	//clear battery discharging flag
		}
	}
	else
	{
		if (IS_MASK_SET(EC_BatteryStatusL,FullyChg))	// || IS_MASK_SET(CHGIC_ReadCmd0x12L,ChargeInhibit))
		{
			CLEAR_MASK(SEL_STATE0,CHARGE_A);  			//clear  battery charging flag
			CLEAR_MASK(nBatteryStatH,CMBS_CHARGE);  	//clear  battery charging flag
		}
		else
		{
			SET_MASK(SEL_STATE0,CHARGE_A);  			//set battery charging flag
			SET_MASK(nBatteryStatH,CMBS_CHARGE);  		//set battery charging flag
			CLEAR_MASK(nBatteryStatH,CMBS_DISCHARGE); 	//clear battery discharging flag
		}
	}
}

void Unlock_ShipMode(void)
{
	/*BYTE sCmd;
	WORD sData;

	// first command = 0x34, data1 = 0x2000, data2 = 0x4000
    	sCmd = C_D_FET;
    	sData = 0x0020; 	// word form L/H
    	if (!bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, sCmd, &sData,SMBus_NoPEC))
    	{
      		SMbusFailCnt3++;
    	}

    	sCmd = C_D_FET;
	sData = 0x0040; 	// word form L/H
    	if (!bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, sCmd, &sData,SMBus_NoPEC))
    	{
      		SMbusFailCnt2++;
    	}
	*/
 	// change all SMBus_NoPEC to SMBus_NeedPEC
    if(IS_MASK_SET(BATTUPDATEFW,BIT0))
        return;
	ShipModeEn=0x00;   //ANGELAS032: add
	CLEAR_MASK(EMStatusBit2,b0SetBatteryShipMode); //ANGELAS079:add
	WSMbusTemp01=0x00;
	WSMbusTemp02=0x20;

	if(!bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC)) 
	{
		SMbusFailCnt3++;
	}

	WSMbusTemp01=0x00;
	WSMbusTemp02=0x40;

	if(!bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
	{
		SMbusFailCnt2++;
	}
	// start Shipmode disable 5s loop once
	if(bRWSMBus(SMbusChB, SMbusRW, SmBat_Addr, C_D_FET, &ShipModeACK,SMBus_NoPEC))
	{
	    if(ShipModeACK!=0x00)
	    {
           ShipModeCnt=1;
		}	
	}
	else
	{
	    SMbusFailCnt4++;
        ShipModeCnt=1;
	}
	// End Shipmode disable 5s loop once 
}

void Lock_ShipMode(void)
{
	/*
	BYTE sCmd;
	WORD sData;

	// first command = 0x34, data1 = 0x0000, data2 = 0x1000
    	sCmd = C_D_FET;
    	sData = 0x0000; 	// word form L/H
    	if (!bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, sCmd, &sData,SMBus_NoPEC))
    	{
      		SMbusFailCnt3++;
    	}

    	sCmd = C_D_FET;
	sData = 0x0010; 	// word form L/H
    	if (!bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, sCmd, &sData,SMBus_NoPEC))
    	{
      		SMbusFailCnt2++;
    	}
	*/
 	//change all SMBus_NoPEC to SMBus_NeedPEC

	//LMLKBL0019:s+Add time delay for Clear First Used Date to Enable ship mode following the requirements of the battery and re-try mechanism.
	BYTE retryNum = 0x00; 
    SMbusFailCnt3 = 0x00; 
    SMbusFailCnt2 = 0x00;
	
	WSMbusTemp01=0x00;
	WSMbusTemp02=0x00;
	if(!bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
	{
		SMbusFailCnt3++;
	}
	
	Delay1MS(250);

	WSMbusTemp01=0x00;
	WSMbusTemp02=0x10;
	if(!bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
	{
	  	SMbusFailCnt2++;
	}

	for(retryNum=0x00;retryNum<0x06;retryNum++)
    {
    	if((SMbusFailCnt3 == 0x00)&&(SMbusFailCnt2 == 0x00))
        	break;

        ProcessSID(0xA5);
        SMbusFailCnt3 = 0x00;
        SMbusFailCnt2 = 0x00;
           
        Delay1MS(0x0A);
	Delay1MS(250); //ANGELAG015: add
           
        WSMbusTemp01=0x00;
        WSMbusTemp02=0x00;
        if(!bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
        {
        	SMbusFailCnt3++;
        }
           
       	Delay1MS(250);
       
        WSMbusTemp01=0x00;
        WSMbusTemp02=0x10;
        if(!bRWSMBus(SMbusChB, SMbusWW, SmBat_Addr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
        {
        	SMbusFailCnt2++;
        }
	}
	//LMLKBL0019:e+Add time delay for Clear First Used Date to Enable ship mode following the requirements of the battery and re-try mechanism.

}


//-----------------------------------------------------------------------------
// read all battery about information
// polling time : 1sec
//-----------------------------------------------------------------------------
void OEM_PollingBatData_TASK(void)
{
	XBYTE i,j;
    XBYTE *ptr;

  	if(IS_MASK_CLEAR(BATTUPDATEFW,BIT0))
  	{
    	GetBatData(Batpollstep1);
     //	ChkBattery_OverConsumption();  //MEILING033:change over 45W to 28W.
    	Batpollstep1++;

    	if(Batpollstep1 >= (sizeof(ABatCommandTable)/4))
    	{
			////////////////////////
			//Mos: Battery Debounce Block
			//Get 9 entry from battery table, loop and XOR each byte, calculate a hash byte
			//If hash result same as previous, then counter +1
			//If counter > 3 times, mean battery data stable, keep Batpollstep1 = 9 to skip first 9 entry in battery table
			//otherwise, clean counter and loop the table again.
			if (Get_Batt_debounce_count < 3)
			{
				Get_Batt_debounce_hash2 = Get_Batt_debounce_hash1;
				Get_Batt_debounce_hash1 = 0x00;
				for (i=0; i<9; i++)
				{
					if(ABatCommandTable[i].block_limit == 0x00)
						Get_Batt_debounce_hash1 ^= *ABatCommandTable[i].smbdataA;
					else
					{
						ptr = ABatCommandTable[i].smbdataA;
						for(j=0; j<ABatCommandTable[i].block_limit; j++)
						{
							Get_Batt_debounce_hash1 ^= *ptr;
							ptr++;
						}
					}
				}

				if (Get_Batt_debounce_hash2 == Get_Batt_debounce_hash1)
					Get_Batt_debounce_count++;
				else
					Get_Batt_debounce_count = 0;

				Batpollstep1=0; //revert Batpollstep1 for start over
				//Mos: Fill nBatteryStatL after Battery Data stable.
				if ( Get_Batt_debounce_count >= 3 )
				{
					if( ( BATTMANUFACTURE[0] == 'S' ) && ((BATTMANUFACTURE[1] == 'A' )||( BATTMANUFACTURE[1] == 'a' )) )
				    	nBatteryStatL |= 0x10 ;	// SANYO
					else if( (BATTMANUFACTURE[0] == 'S' ) && ( BATTMANUFACTURE[1] == 'O' ) )
				    	nBatteryStatL |= 0x20 ;	// Sony
					else if( (BATTMANUFACTURE[0] == 'P' ) && (( BATTMANUFACTURE[1] == 'A' )|| ( BATTMANUFACTURE[1] == 'a' )) )
						nBatteryStatL |= 0x40 ;	// Panasonic
					else if( (BATTMANUFACTURE[0] == 'S' ) && ( BATTMANUFACTURE[1] == 'U' ) )
						nBatteryStatL |= 0x50 ;	// Samsung
					else if( (BATTMANUFACTURE[0] == 'L' ) && ( BATTMANUFACTURE[1] == 'G' ) )
						nBatteryStatL |= 0x30 ;	// LG
					else if( (BATTMANUFACTURE[0] == 'C' ) && (( BATTMANUFACTURE[1] == 'P' ) || ( BATTMANUFACTURE[1] == 'p' )) )
						nBatteryStatL |= 0x60;	// CPT Celxpert
					else if( (BATTMANUFACTURE[0] == 'S' ) && (BATTMANUFACTURE[1] == 'M' ) )
						nBatteryStatL |= 0x70;	// Simplo*/
				}
			}
			else
			{
    			Batpollstep1=9;
			}
    	}
		UpdateNameSpace();
		ChkLENOVOPMFW();
		ChkGoTarget();
	  	Chk_Battery_Full();
		if(GPUProchotONCnt == 0) 
		{ //ANGELAG033: add
			ChkBattery_OTP();
			//ChkBattery_OCP();  //MEILING046:remove.//MEILING051:-Add cpu P_STATE function.
			ChkBattery_OCP();  //ANGELAG017: add
		//	ChkPsys();  //ANGELAG019: add
			CkkBattery_RSOCLow(); //MEILING033:add
		} //ANGELAG033: add
        ChkBattery_LowVoltage();
		ChkBattery_FCCchg();
		ChkAvgCurrent();
		ChkS3DCRSOC();//ANGELAS093:Modify battery RSOC below 5% can't into S3.
		ChkS3ResumeRSOC();
		RSOC1Pto0P_ShutdownCheck();//ANGELAS043:Add
  	}
}


//-----------------------------------------------------------------
// Service Charger SMBus
//-----------------------------------------------------------------
void WriteSmartChgIC(void)
{
	BYTE sCmd, *sData;
	WORD BattNowCurrent; 

	if (!Read_AC_IN())
	{
		return;
	}
	if(CHGIC_ptr>=4) 
	{
		CHGIC_ptr=0;
	}
	else 
	{
		CHGIC_ptr++;
	}
	switch( CHGIC_ptr )
	{
		case 0x00:
			sCmd  = C_ChargeCurrent;
			sData = &nBattCharCurrentL;
			break;
		case 0x01:
			sCmd  = C_ChargeVoltage;
			sData = &nChargingVoltL;
			break;
		case 0x02:
			if(IS_MASK_CLEAR(SEL_STATE0,PRESENT_A))//AC only
			{
				CHGIC_InputCurrentL=0x00;//
				CHGIC_InputCurrentH=0x0B;
			}
			else  //AC+DC
			{
				if(nBattGasgauge >= 10)
				{
					CHGIC_InputCurrentL=0x80;//
					CHGIC_InputCurrentH=0x0B;
				}
				else
				{
					CHGIC_InputCurrentL=0x00;//
					CHGIC_InputCurrentH=0x0B;
				}
			}
		    sCmd  = C_InputCurrent;
            sData = &CHGIC_InputCurrentL;
            break;
		case 0x03:
			if ((CHGIC_ReadCmd0x12L!=0) || (CHGIC_ReadCmd0x12H!=0))
			{
				if ((cBattFlag0.fbit.cCmdAcOff==1)||IS_MASK_SET(EC_BatteryStatusL,FullyChg))	
				{
					SET_MASK(CHGIC_WriteCmd0x12L,ChargeInhibit);	
				}
				else
				{
					if ((cBattFlag0.fbit.cBF0_GoTarget ==1) && (cTargetGauge == nBattGasgauge))
					{
						SET_MASK(CHGIC_WriteCmd0x12L,ChargeInhibit);	
					}
					else
					{
						CLR_MASK(CHGIC_WriteCmd0x12L,ChargeInhibit);	
					}
				}
				
				if (nStopChgStat3L|nStopChgStat3H|inhibit2sec|ACOFF_SOURCE)					
				{
					SET_MASK(CHGIC_WriteCmd0x12L,ChargeInhibit); 	
				}
				sCmd = C_ChargerMode;
				sData = &CHGIC_WriteCmd0x12L; 
			}
			else
			{
				return;
			}
			break;
		case 0x04:
	    	sCmd = C_ChargerMode3;
	    	sData = &CHGIC_WriteCmd0x37L;
	  		break;
		default:
			return;
	}
	if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, sCmd, sData,SMBus_NoPEC))
	{
		CHGIC_SMbusFailCnt++;
	}
}


const sRSmbusBStruct code ReadChgIcCmdTable [] =
{//     command                   REG 			 no.
  { C_ChargeCurrent,    &CHGIC_ReadCmd0x14L    	,0x00},
  { C_ChargeVoltage,    &CHGIC_ReadCmd0x15L    	,0x00},
  { C_InputCurrent,    	&CHGIC_ReadCmd0x3FL     ,0x00},
  { C_ChargerMode,     	&CHGIC_ReadCmd0x12L		,0x00},
  { C_ChargerMode3,     &CHGIC_ReadCmd0x37L		,0x00} 
};
void ReadSmartChgIC(void)
{
	//	if (IS_MASK_SET(SEL_STATE0,PRESENT_A))
	{
    	if(!bRWSMBus(SMbusChB, SMbusRW, Charger_Addr,
             ReadChgIcCmdTable[CHGIC_ptr].cmd,
             ReadChgIcCmdTable[CHGIC_ptr].smbdataA,
               SMBus_NoPEC))
    	{  // SMBUS FAIL
      		CHGIC_SMbusFailCnt++;
    	}
	}
}

void Chk_Shutdown_Volt(void)
{
	WORD cutoff_volt;
	#if EN_PwrSeqTest
		  return;
	#endif

	if( (BATTMANUFACTURE[0] == 'L' ) && ( BATTMANUFACTURE[1] == 'G' ) )
	{
		cutoff_volt = 8250 ;	//  shutdown Voltage 8.25V  for LG 3S battery
	}
	else
	{
        cutoff_volt = 8500;		// Voltage 8.5V for Sanyo battery
    }


	ITempW01 = (WORD)((nPresentVoltH<<8)+nPresentVoltL);	// get now voltage

	if (ITempW01 < cutoff_volt)	//  shutdown Voltage
	{
		BatLowCnt++;
		if ((BatLowCnt >= 6)&&((SysPowState==SYSTEM_S0)||(SysPowState==SYSTEM_S3)))
		{
        	//SET_MASK(SysStatus,ERR_ShuntDownFlag);
			//ProcessSID(BATTLOWVOLT_ID);
			//RSMRST_shutdown();	//shutdown (RSMRST HI-->low-->HI)
		}
	}
	else
	{
		BatLowCnt =0;
	}
}



void Chk_BAT1PERCL_5(void)
{
	if ((BAT1PERCL <= 0)&&(IS_MASK_SET(nBattery0x16L,Dsg)))	// BAT1PERCL <= 5%  //T064: change 5 to 0
	{
		//cBATTThrottling = 0x0F;
		//cBATTThrottling = PSTATE_MAXStep;//0ptimize CPU P_state (change 16 step to 8 step).  //MEILING048:remove.//MEILING051:-Add cpu P_STATE function.
		SET_MASK(BatteryAlarm,BATPercl_5);
	}
	else
	{
		if (IS_MASK_SET(BatteryAlarm,BATPercl_5))
		{
			CLEAR_MASK(BatteryAlarm,BATPercl_5);
			if (BatteryAlarm == 0)
			{
				//cBATTThrottling = 0;  //MEILING048:remove.//MEILING051:-Add cpu P_STATE function.
			}
		}
	}
}



void Chk_BatSMbusFailCount(void)
{
	BYTE sCmd, *sData;

	if (nBattErrorCnt==0)
	{
		CLEAR_MASK(nStopChgStat3L,ENCOMMFAIL);	// clear bat communication fail and clear STOP charge
		CLEAR_MASK(Bat1_FPChgFlag,BIT(0));
		Chk_BAT1PERCL_5();
		if (IS_MASK_SET(nBattery0x16L,Dsg))
		{
			Chk_Shutdown_Volt();
		}
		return;
	}

	//if (IS_MASK_CLEAR(SYS_STATUS,AC_ADP)||(IS_MASK_SET(SYS_STATUS,AC_ADP)&&Read_ACOFF()))		// discharge
	if (IS_MASK_CLEAR(SYS_STATUS,AC_ADP))		// discharge
	{

		RST_ChgTimeOutCnt();

		if (nBattErrorCnt==30)
		{
			//cBATTThrottling = 0x0F;
			//cBATTThrottling = PSTATE_MAXStep;//0ptimize CPU P_state (change 16 step to 8 step). //MEILING048:remove.//MEILING051:-Add cpu P_STATE function.

			//nRemainingCapL = 0;  //LMLKBL0006:remove.
			//nRemainingCapH = 0;  //LMLKBL0006:remove.
			SMbusFailCnt2++;
		}
		if (nBattErrorCnt>=150)
		{
			#if !EN_PwrSeqTest
			//cBATTThrottling = 0x0F;
			//cBATTThrottling = PSTATE_MAXStep;//0ptimize CPU P_state (change 16 step to 8 step).  //MEILING048:remove.//MEILING051:-Add cpu P_STATE function.
			SMbusFailCnt3++;
			nBattErrorCnt = 151;
			if ((SysPowState==SYSTEM_S0)||(SysPowState==SYSTEM_S3))
			{
                SET_MASK(SysStatus,ERR_ShuntDownFlag);//Add battery smbus fail shutdown ID.
				ProcessSID(BATTCOMMFAIL_ID);
				RSMRST_shutdown();
			}
			#endif
		}
	}
	else
	{
		//if (IS_MASK_SET(SYS_STATUS,AC_ADP)&&(!Read_ACOFF()))					// battery charge
		if (IS_MASK_SET(SYS_STATUS,AC_ADP))					// battery charge
		{
			if ((nBattErrorCnt != 0)&&(nBattErrorCnt < 30))
			{
				nBattCharCurrentL = 0x00;			// 256mA
				nBattCharCurrentH = 0x01;
				nChargingVoltL = 0xD0;				// 8.4V     //LMLKBL0002:change 0xA0 to 0xD0.
				nChargingVoltH = 0x20;              //LMLKBL0002:change 0x41 to 0x20.
				//CHGIC_InputCurrentL = 0x00;
				//CHGIC_InputCurrentH = 0x0C;
				SMbusFailCnt3++;
			}
			if ((nBattErrorCnt >= 30))				// disable charge
			{
				//nBattCharCurrentL = 0x00;			// 0 mA
				//nBattCharCurrentH = 0x00;
				//nChargingVoltL = 0x00;				// 0 V
				//nChargingVoltH = 0x00;
				//CHGIC_InputCurrentL = 0x00;
				//CHGIC_InputCurrentH = 0x00;
				RST_ChgTimeOutCnt();
				SET_MASK(nStopChgStat3L,ENCOMMFAIL);	// Set bat communication fail and STOP charge.
															//	charge inhibit
				SMbusFailCnt2++;
			}
		}
	}
}



void Chk_CHG_TimeOut(void)			// every 1 min
{
    if (nBattTempCnt < 10)
		return;
	if (IS_MASK_SET(CHGIC_ReadCmd0x12L,ChargeInhibit))
		return;
	
	if((nBattCharCurrentH > 1) || ((nBattCharCurrentH == 1) && (nBattCharCurrentL >= 0xF4))) // nBattCharCurrent >= 0x01F4  (500mA)
	{
		TrickleChgTimeOutCnt = 0;
		FastChgTimeOutCnt++;
		if (FastChgTimeOutCnt >= 720) //12hour
		{
			FastChgTimeOutCnt = 721;
			SET_MASK(nStopChgStat3L,ENSTOPCHG);
		}
	}
	else // nBattCharCurrent < 0x01F4  (500mA)
	{
		FastChgTimeOutCnt = 0;
		TrickleChgTimeOutCnt++;
		if (TrickleChgTimeOutCnt >= 360) //12hour
		{
			TrickleChgTimeOutCnt = 361;
			SET_MASK(nStopChgStat3H,ENTRITIMEOUT); //MARTINO006:change 'nStopChgStat3L' to 'nStopChgStat3H'
		}
	}
}

void ChkBattery_abnormal(void)
{
   	if(IS_MASK_SET(BATTUPDATEFW,BIT0))
           return;

	switch(ChkBattery_abnormal_status)
	{
	case ChkBattery_abnormal_status_normal:
		if(IS_MASK_SET(CHGIC_ReadCmd0x12L, ChargeInhibit)
			&& IS_MASK_CLEAR(nNowCurrentH, BIT7) && (nNowCurrentL > 100) )
		{
			ChkBattery_abnormal_count++;
			if (ChkBattery_abnormal_count>5)
			{
				ChkBattery_abnormal_status = ChkBattery_abnormal_status_error;
			}
		}
		else
		{
			ChkBattery_abnormal_status = ChkBattery_abnormal_status_normal;
			ChkBattery_abnormal_count = 0;
		}
		break;
	case ChkBattery_abnormal_status_error:
		//Mos: Reset until EC power down
		SET_MASK(ACOFF_SOURCE, CHARGECURRENT);
		BATT_LEN_ON(); //ANGELAS047:add
		//ACOFF_HI();
		break;
	default:
		ChkBattery_abnormal_status = ChkBattery_abnormal_status_normal;
		ChkBattery_abnormal_count = 0;
		break;
	}
}


BYTE Compare_data(XBYTE *buf1, XBYTE *buf2, BYTE count)
{
	do 
	{
		if( *buf1 != *buf2 ) return FALSE;
		buf1++;
		buf2++;
		count--;
	}while(count);
	return TRUE;
}

void Compare_Auth_Result(void)
{
	//inverse result for compare
	//inverse(&Hn0, &Hn4+3);
	if(Compare_data(&K, &RBATH0, 20) )
	{
		CLEAR_MASK(nStopChgStat3H,NotLenovoBattery); 	// SHA1 pass,legal
		CLEAR_MASK(LENOVOBATT,BATTERY_LEGAL);			// SHA1 pass,legal
		CLEAR_MASK(LV_Authen_Step_CNT,BIT(6)); 		// authentication ok
		SHA1failCnt=0x00;
		SHA1FailRetry=0x00;
	}
	else
	{
		SHA1failCnt ++;
		if (SHA1failCnt < 4)
		{
			Service_Auth_Step=23;						//if  SHA1failCnt <3, retry
		}
		else
		{
		    SHA1FailRetry++;
			//if(SHA1FailRetry < 10)  
		    if((SHA1FailRetry!= 3)&&(SHA1FailRetry<11)) 
		    {
		        Service_Auth_Step = 1;
		    }
            else if(SHA1FailRetry == 3)  //add judge         
		    {
			    SET_MASK(nStopChgStat3H,NotLenovoBattery);	// SHA1 no pass, battery illegal
			    SET_MASK(LENOVOBATT,BATTERY_LEGAL);			// SHA1 no pass, battery illegal
			    SET_MASK(LV_Authen_Step_CNT,BIT(6)); 		// authentication Fail
			    Service_Auth_Step = 1;  
		    }
       	}  
	}
}

BYTE SendChallengeToBat(void)
{
	BYTE SMBus_work;
	SMBus_work = bWSMBusBlock(SMbusChB, SMbusWBK, BatteryAddress, 0x27, &BATchallenger, 20, TRUE);
	if( SMBus_work ) 
		return TRUE;
	
	return FALSE;
}

BYTE GetChallengeFromBat(void)
{
	if( bRSMBusBlock(SMbusChB, SMbusRBK, BatteryAddress, 0x28, &RBATH0) ) 
		return TRUE;
	
	return FALSE;
}

void SetRandomKey(BYTE N)
{
	char *ptr;
	BYTE i;

	if( N==1 )
	{
		ptr = BATchallenger;
		for(i=0;i<20;i++)
		{
			*ptr = (char)(rand()&0xff);
			ptr++;
		}

		/* bq20zxx two pass example key */
		//SHA1_W[4] = 0x20212223;
		//SHA1_W[5] = 0x24252627;
		//SHA1_W[6] = 0x28292A2B;
		//SHA1_W[7] = 0x2C2D2E2F;
		//SHA1_W[8] = 0x30313233;

		/* LV Sample example key */
		//SHA1_W[4] =0xC82CA3CA;
		//SHA1_W[5] =0x10DEC726;
		//SHA1_W[6] =0x8E070A7C;
		//SHA1_W[7] =0xF0D1FE82;
		//SHA1_W[8] =0x20AAD3B8;


		/* SHA1 one pass example key */
		//SHA1_W[4] = 0x00000000;
		//SHA1_W[5] = 0x00000000;
		//SHA1_W[6] = 0x00000000;
		//SHA1_W[7] = 0x00000000;
		//SHA1_W[8] = 0x00000000;
	}
    else 
    {
		// move first time result to challenge
		//SHA1_W[4]=Hn0;
		N_Bit_Shift = 0;
		ITempW01 = &Hn0;
		AddrX_High = (ITempW01 >> 8);
		AddrX_Low  = (ITempW01 & 0xFF);
		ITempW01 = &SHA1_W[4];
		AddrZ_High = (ITempW01 >> 8);
		AddrZ_Low  = (ITempW01 & 0xFF);
		Cal_CTRL = Rotate_Shift_right_BIT;

		//SHA1_W[5]=Hn1;
		AddrX_Low+=4;
		AddrZ_Low+=4;
		Cal_CTRL = Rotate_Shift_right_BIT;

		//SHA1_W[6]=Hn2;
		AddrX_Low+=4;
		AddrZ_Low+=4;
		Cal_CTRL = Rotate_Shift_right_BIT;

		//SHA1_W[7]=Hn3;
		AddrX_Low+=4;
		AddrZ_Low+=4;
		Cal_CTRL = Rotate_Shift_right_BIT;

		//SHA1_W[8]=Hn4;
		AddrX_Low+=4;
		AddrZ_Low+=4;
		Cal_CTRL = Rotate_Shift_right_BIT;
	}
}



void LV_BAT_Authentication(void)
{
	unsigned long xdata * byte_sha1_pntr;
    int j;

	//===================================================================
	// Service SHA1 algorithm
	//===================================================================
	switch(Service_Auth_Step)
	{
	case 1:
		//InitializeFix();		// initial H and W(0~4/9~15)
		SetRandomKey(1);		// initial W(4~8)
		Service_Auth_Step=2;
		break;
	case 21:					// SHA1 start send data delay 10 sec
		SendChallengeToBat();	// send to battery
		Service_Auth_Step=22;
		break;
	case 22:
		byte_sha1_pntr = sha1_auth(&BATchallenger);
		for (j=0; j<5 ;j++)
		{
			K[j] = *byte_sha1_pntr;
			byte_sha1_pntr++;
		}
		Service_Auth_Step=23;
		break;
	case 23:
		Service_Auth_Step=24;
		break;
	case 24:
		Service_Auth_Step=25;	// wait  for battery authentication
		break;
	case 25:
		GetChallengeFromBat();
		Service_Auth_Step=0;
		Compare_Auth_Result();
		break;
	case 0:
		Service_Auth_Step=0;
		break;
	default:
		Service_Auth_Step ++;
		break;
	}
}


void Lenovo_Battery_EM80(void)
{
	if (EM8_TEST == 0)
	{
		if (IS_MASK_SET(Bat0x3ETempH,BIT0)&&(IS_MASK_SET(SYS_STATUS,AC_ADP))&&IS_MASK_CLEAR(LENOVOPMFW,BATTERY_STORAGE))		// 0x3E  bit8 G41:under storage mode Battery has been running over 72hrs ,EM can't pop out a message.
		{ 
			SET_MASK(LENOVOBATT,BATTERY_USAGE);
        }
		else
		{ 
		    CLEAR_MASK(LENOVOBATT,BATTERY_USAGE); 
        }

		if (Bat0x00TempL&0x03)				// 0x00  bit1, bit0
		{ 
		    SET_MASK(LENOVOBATT,BAD_BATT); 
        }	// Battery bad
		else
		{ 
		    CLEAR_MASK(LENOVOBATT,BAD_BATT); 
        }

		if (IS_MASK_SET(Bat0x3ETempH,BIT1))			// 0x3E  bit9
		{ 
		    SET_MASK(LENOVOBATT,BATTERY_Exhaustion); 
        }	// Battery poor  (Exhaustion)
		else
		{ 
		    CLEAR_MASK(LENOVOBATT,BATTERY_Exhaustion);   
        }
	}
	else
	{ 
	    LENOVOBATT = EM8_TEST ; 
    }
}

//Through the charger IC option control battery learn mode.
void BATT_LEN_OFF(void)
{
	CLEAR_MASK(CHGIC_WriteCmd0x12L,BatLearnEnable); //Change charge IC option setting. read to write
    if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ChargerMode, &CHGIC_WriteCmd0x12L,SMBus_NoPEC)) //Change charge IC option setting. read to write
    {
		CHGIC_SMbusFailCnt++;
		RamDebug(0x13);
    }
}
void BATT_LEN_ON(void)
{
 	SET_MASK(CHGIC_WriteCmd0x12L,BatLearnEnable); //Change charge IC option setting. read to write
	if (!bRWSMBus(SMbusChB, SMbusWW, Charger_Addr, C_ChargerMode, &CHGIC_WriteCmd0x12L,SMBus_NoPEC)) //Change charge IC option setting. read to write
    {
		CHGIC_SMbusFailCnt++;
		RamDebug(0x12);
    }
}
//Through the charger IC option control battery learn mode.

const SRTVTstruct code RTVTTable [] =
{
	{20,0x040E},
	{21,0x0409},
	{22,0x0404},
	{23,0x03FF},
	{24,0x03F9},
	{25,0x03F4},
	{26,0x03EE},
	{27,0x03E8},
	{28,0x03E2},
	{29,0x03DB},
	{30,0x03D5},
	{31,0x03CE},
	{32,0x03C7},
	{33,0x03C0},
	{34,0x03B9},
	{35,0x03B1},
	{36,0x03AA},
	{37,0x03A2},
	{38,0x039A},
	{39,0x0392},
	{40,0x0389},
	{41,0x0381},
	{42,0x0378},
	{43,0x0370},
	{44,0x0367},
	{45,0x035E},
	{46,0x0354},
	{47,0x034B},
	{48,0x0342},
	{49,0x0338},
	{50,0x032E},
	{51,0x0324},
	{52,0x031A},
	{53,0x0310},
	{54,0x0306},
	{55,0x02FC},
	{56,0x02F2},
	{57,0x02E7},
	{58,0x02DD},
	{59,0x02D2},
	{60,0x02C8},
	{61,0x02BD},
	{62,0x02B2},
	{63,0x02A8},
	{64,0x029D},
	{65,0x0292},
	{66,0x0287},
	{67,0x027D},
	{68,0x0272},
	{69,0x0267},
	{70,0x025D},
	{71,0x0252},
	{72,0x0247},
	{73,0x023D},
	{74,0x0232},
	{75,0x0228},
	{76,0x021E},
	{77,0x0213},
	{78,0x0209},
	{79,0x01FF},
	{80,0x01F5},
	{81,0x01EB},
	{82,0x01E1},
	{83,0x01D7},
	{84,0x01CE},
	{85,0x01C4},
	{86,0x01BB},
	{87,0x01B1},
	{88,0x01A8},
	{89,0x019F},
	{90,0x0196},
	{91,0x018E},
	{92,0x0185},
	{93,0x017C},
	{94,0x0174},
	{95,0x016C},
	{96,0x0164},
	{97,0x015C},
	{98,0x0154},
	{99,0x014C},
	{100,0x0145},
	{101,0x013D},
	{102,0x0136},
	{103,0x012F},
	{104,0x0128},
	{105,0x0121},
	{106,0x011A},

};

void PollingCPURT(void)
{
	BYTE i;
	BYTE j=0;

	for (i=0;i<(sizeof(RTVTTable)/sizeof(SRTVTstruct));i++)
	{
		if(NTC_V2<RTVTTable[j].VOL)
		{
			j++;
		}
		else if(NTC_V2>=RTVTTable[j].VOL)
		{
			ThermistorCPU_TEMP=RTVTTable[j].TEM;//Change fan table follow thermal team.
			return;
		}	
	}
}


void PollingGPURT(void)
{
	BYTE i;
	BYTE j=0;

	for (i=0;i<(sizeof(RTVTTable)/sizeof(SRTVTstruct));i++)
	{
		if(NTC_V1<RTVTTable[j].VOL)
		{
			j++;
		}
		else if(NTC_V1>=RTVTTable[j].VOL)
		{
			EXTVGA_TEMP=RTVTTable[j].TEM;//Change fan table follow thermal team.
			return;
		}		
	}
}

void Clear_Batt_First_Used_Date(void)
{
	if(IS_MASK_SET(SEL_STATE0,PRESENT_A)) 
	  {
	    	if(IS_MASK_SET(pProject0,b1uFUdayClr))
	    	{
			if(!bRWSMBus(SMbusChB,SMbusRW,SmBat_Addr,FirstUsedDate,&batteryFirstUsedDateL,SMBus_NeedPEC))
	      		{
	        		RamDebug(0xAE);
				return;
	      		}
			if(batteryFirstUsedDateH != 0x00 || batteryFirstUsedDateL != 0x00)
			{
		      		batteryFirstUsedDateH = 0x00;
			  	batteryFirstUsedDateL = 0x00;
		      		if(!bRWSMBus(SMbusChB,SMbusWW,SmBat_Addr,FirstUsedDate,&batteryFirstUsedDateL,SMBus_NeedPEC))
		      		{
		        		RamDebug(0xAD);
					return;
		      		}
			}
			else
			{
				CLEAR_MASK(pProject0,b1uFUdayClr);
			}
	    	}

	  }
}

//Start Support fast charging and modify charger IC setting
/* **************************************************************************************
* Battery_Expresscharge():
* 1. Battery Support Express charging
* 2. Lenovo App Control Express charging
*****************************************************************************************/
void Battery_Expresscharge(void)
{
   if(IS_MASK_CLEAR(EC_C_modeL,b4QuickChargeMode))
   {
    return;
   }//check support quick charge?
   if (IS_MASK_SET(OEMControl,Expresschargemode))
   { 
		if(IS_MASK_CLEAR(Bat0x3ETempH,Expresscharge_mode))
		{
			SET_MASK(Bat0x3ETempH,Expresscharge_mode);
		 	if(bRWSMBus(SMbusChB,SMbusWW,SmBat_Addr,C_LVMfgFun2,&Bat0x3ETempL,SMBus_NeedPEC))
	 		{
	 			RamDebug(0xD1);
	 		}
       }
	}
	else
	{
		if(IS_MASK_SET(Bat0x3ETempH,Expresscharge_mode))
		{
   		 	CLEAR_MASK(Bat0x3ETempH,Expresscharge_mode);
  		 	if(bRWSMBus(SMbusChB,SMbusWW,SmBat_Addr,C_LVMfgFun2,&Bat0x3ETempL,SMBus_NeedPEC))
	 		{
	 			RamDebug(0xD2);
	 		}
		}
	}
}
