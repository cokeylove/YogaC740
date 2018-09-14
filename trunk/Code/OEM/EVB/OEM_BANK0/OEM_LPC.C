/*-----------------------------------------------------------------------------
 * TITLE: OEM_LPC.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

/*****************************************************************************/
// Procedure: MuteProcess										TimeDiv: 50mSec
// Description:
// GPIO: GPIOJ1
// Referrals:
/*****************************************************************************/
void MuteProcess(void)
{
	//ANGELAG010: add start
	if(!Read_SLPS3()) 
    	{
		EC_MUTE_ACT();
		return;
	}
	//ANGELAG010: add end
	if ( SystemIsS0 )
	{	// Check ISCT and command 94 status.
		if( MuteCount == 0 )
		{
			if ( (IS_MASK_CLEAR(uISCT_2, b3ISCT_MUTE)) && (IS_MASK_CLEAR(cCmd, b5VolMute)) )
			{
				EC_MUTE_INACT();
				return;
			}
		}
		else
		{ 
		    MuteCount--; 
        }
	}
	EC_MUTE_ACT();
}

/*****************************************************************************/
// Procedure: CameraProcess									TimeDiv: 100mSec
// Description:
// GPIO: GPIOG7
// Referrals:
/*****************************************************************************/
void CameraProcess(void)
{
#if Support_WebCam
	if ( SystemIsS0 )
	{
		if ( IS_MASK_SET(SYS_MISC1,ACPI_OS) )
		{
			if( nCameraExistGET && (IS_MASK_SET(pDevStus, pCamera)) && (IS_MASK_CLEAR(uISCT_2, b4ISCT_Camera)) )
			{
				SET_MASK(LENOVODEVICE,Camera_PwrOn);	// Set Camera power enable bit.
				CMOS_Cam_ON();
			}
			else
			{
				CLR_MASK(LENOVODEVICE,Camera_PwrOn);	// Set Camera power disable bit.
				CMOS_Cam_OFF();
			}
		}
	}
	//else if ( SystemIsS5 || SystemIsS3 )
	//{ CMOS_Cam_ON(); }
#endif	// Support_WebCam
}

/*****************************************************************************/
// Procedure: WirelessProcess									TimeDiv: 100mSec
// Description:
// GPIO: GPIOG7
// Referrals: disable and enable Wireless.
/*****************************************************************************/
void WirelessProcess(void)
{
#if Havekillswitch
	/*if( pDevStus.bit.pKillSwitch != nKillSwitchGET )
	{
		if( IS_MASK_SET(cCmd, b6TestBtnEn) )
			StringToHost1( sKillSW_scan_codes );
		pDevStus.bit.pKillSwitch = nKillSwitchGET;

		pSmiTrigSource4.bit.STS4_WLSwitch = 1;
		if( pDevStus.bit.pKillSwitch )
		{ SET_MASK(DEVICEMODULE,b4KILL_STATUS); }
		else
		{ CLR_MASK(DEVICEMODULE,b4KILL_STATUS); }

		if ( cSysStus.bit.cSS_S0 && !cSysStus.bit.cSS_S3
			&& !pwrSUSPactive && cSysStus.bit.cSS_Acpi )
		{
			uVPCeventSource = 0x00;
			uVPCeventSource2 = VPCeventAllRF;
			pSmiTrigSource4.bit.STS4_VPC = 1;
		}
	}
	*/
#else
	SET_MASK(DEVICEMODULE,b4KILL_STATUS);  // workaround for no killswitch
#endif

	if ( SystemIsS0 && (IS_MASK_SET(SYS_MISC1,ACPI_OS)) )
	{
		#if Havekillswitch
		if( nKillSwitchGET )
		{
			if ( uWLBTLanTemp != DEVICEMODULE )
			{
				uWLBTLanTemp = DEVICEMODULE;
				ECQEvent(WLAN_BTN_EVENT_65);	// 0x65 WLAN status notify.
			}
		}
		#else
		//Add sent Qevent 0x42 for WLAN&BT on/off under win7.
		/*if ( IS_MASK_SET(DEVICEMODULE,WIRELESS_EXIST) )	// Check WLAN exist.
		{
			if ( IS_MASK_SET(pDevStus, pWireless) )
			{
				SET_MASK(DEVICEMODULE,WIRELESS);	// Eanble WLAN.
				//if ( WLAN_OFF_Cnt == 0 )
				//{ WL_OFF_ON(); }
				//{ WLAN_OFF_Cnt--; }
				//WL_OFF_ON();
			}
			else
			{
				//WL_OFF_OFF();
				//if ( WLAN_OFF_Cnt == 0 )
				//{ CLR_MASK(DEVICEMODULE,WIRELESS);  }	// Disable WLAN.
				//else
				//{ WLAN_OFF_Cnt--; }
				CLR_MASK(DEVICEMODULE,WIRELESS);// Disable WLAN.
			}
		}
		else
		{
			//WL_OFF_OFF();
			CLR_MASK(DEVICEMODULE,WIRELESS);	// Disable WLAN.
		}

		if ( IS_MASK_SET(DEVICEMODULE,BLUETOOTH_EXIST) )	// Check BT exist.
		{
			if ( IS_MASK_SET(pDevStus, pBluetooth) )
			{ 
			        nBlueToothEN; 
                     }
			else
			{ 
			        nBlueToothDIS; 
                      }
		}
		else
		{ 
		        nBlueToothDIS; 
             }*/
		// Add sent Qevent 0x42 for WLAN&BT on/off under win7.

		if ( uWLBTLanTemp != DEVICEMODULE )
		{
			uWLBTLanTemp = DEVICEMODULE;
			ECQEvent(WLAN_BTN_EVENT_65);	// 0x65 WLAN status notify.
		}
		#endif
	}
}

void IFFSProcess(void)
{
	if (uIffsCnt == 0 ) 
    	return;
	else 
        uIffsCnt--;

	if (uIffsCnt==0)
	{
		CLR_MASK(uCritBattWakeThre,IFFS_Enable);
		if (SystemIsS3)
		{
			if (nBattGasgauge < uCritBattWakeThre)
			{
				SET_MASK(uCritBattWakeThre,IFFS_Enable);
            	PWSeqStep = 1;
				PowSeqDelay = 1;
                RamDebug(0x32);       
            	SysPowState=SYSTEM_S3_S0;
			}
		}
	}
}
//ANGELAS057:add start
void ECPowerDownEnableExternalTimer2(void)
{
    LWORD timersetting;
    LWORD g_ECPowerDownPeriodWakeUpTime;
    g_ECPowerDownPeriodWakeUpTime=240;  //MARTINH090:change '120' to '240'

    if (g_ECPowerDownPeriodWakeUpTime != 0x00)
    {
       	timersetting = (LWORD)(g_ECPowerDownPeriodWakeUpTime * 32);

       	CLEAR_MASK(CGCTRL1R, BIT6);         // ETWD clock operation

        ET2PSR = 0x02;                      // Prescaler 32 HZ
        ET2CNTLH2R =timersetting >> 16;  
        ET2CNTLHR = timersetting >> 8;
        ET2CNTLLR = timersetting;  

        ISR7= Int_ET2Intr;      // clear extern timer2  interrupt state register
        SET_MASK(IER7, Int_ET2Intr);        // enable extern timer2 interrupt
    }
       
}
void ECDisableExternalTimer2(void)
{
    CLEAR_MASK(IER7, Int_ET2Intr);     // disable extern timer2 interrupt 
    SET_MASK(ISR7, Int_ET2Intr);        // clear extern timer2  interrupt state register
}
//ANGELAS057:add end
void SetACIN_Int(void)
{
	SET_MASK(IELMR13,Int_WKO101);
	CLR_MASK(WUEMR10,WUC_WUI53); 
	CLR_MASK(IPOLR13,Int_WKO101);// rising
	ISR13 = Int_WKO101;
	SET_MASK(IER13, Int_WKO101);
}

//MEILING039:S+ add AC out interrupt enable and disable function.
void ACInOutIntEnable(void)  // AC_IN#   WU101 --------> int106 
{ 
	if(Read_AC_IN()) 
	{ 
        SET_MASK(WUEMR10, BIT(5));//  Either-edge (rising-edge or falling-edge) triggered is selected.
	} 
	else 
	{ 
		CLEAR_MASK(WUEMR10, BIT(5)); 
	}
	
    WUESR10 = BIT(5); 

    SET_MASK(IER13, Int_WKO101); 
   	ISR13 = Int_WKO101;     
} 

void ACInOutIntDisable(void) // AC_IN#   WU101 --------> int106 
{ 
    ISR13 = Int_WKO101; // clear int 106 for  ISR 
    CLEAR_MASK(IER13, Int_WKO101); 
    WUESR10 = 0x20;  // clear wakeup status 
}
//MEILING039:E+ add end.
//72JERRY020:s+ Modify enter EC sleep setting.
void BatteryOutWakeEnable(void)		// GPI2   WUI73 -------> int126
{
	WUEMR13 &= ~0x02;					// Wake-up falling-edge triggered is selected.
	WUESR13 = 0x02;
	
	SET_MASK(IER15, Int_WKO121);	  	// enable int121 for  Battery In
	ISR15 = Int_WKO121;     			// clear int 121 for  ISR
	
}
void BatteryINWakeEnable(void)	// BATT_TEMP(GPI2)   WU121 -------> INT126
{
	WUEMR13 |= 0x02;					// Wake-up falling-edge triggered is selected.
	WUESR13 = 0x02;
	
	SET_MASK(IER15, Int_WKO121);	  	// enable int126 for  Battery In
	ISR15 = Int_WKO121;     			// clear int 126 for  ISR
	
}
//72JERRY020:e+ Modify enter EC sleep setting.
void SetPWRSW_Int(void)
{
	//GPCRB3 = ALT;  //  ON/OFF
	SET_MASK(IELMR1,Int_WKO25);
	SET_MASK(WUEMR2,WUC_PWRSW);
	//SET_MASK(WUENR2,WUC_PWRSW);
	CLR_MASK(IPOLR1,Int_WKO25);				// falling
	SET_MASK(IER1, Int_WKO25);				// Enable PWRSW Interrupt
}

void Setlanwake_Int(void)
{

	SET_MASK(IELMR2,BIT(5));
	SET_MASK(WUEMR2,BIT(2));
	//SET_MASK(WUENR2,WUC_PWRSW);
	CLR_MASK(IPOLR2,BIT(5));				// falling
	SET_MASK(IER2, BIT(5));				// Enable LANWAKE Interrupt
}


void SetNovo_Int(void)
{
	//GPCRJ = ALT;   // NOVO
	SET_MASK(IELMR16,Int_WKO128);
	SET_MASK(WUEMR14,WUC_WUI80);
	//SET_MASK(WUENR14,WUC_WUI80);
	SET_MASK(IPOLR16,Int_WKO128);				// falling
	SET_MASK(IER16, Int_WKO128);				// Enable Novo Interrupt
}

/*****************************************************
*****************************************************/
void MXLID_Wake_En(void) 				// LID_SW----GPB1
{
	SET_MASK(WUEMR10, BIT(6));		//  Either-edge (rising-edge or falling-edge) triggered is selected	
	WUESR10=BIT6;	
	ISR13 = BIT3;
	SET_MASK(IER13, BIT3);
}
 
void MXLID_Wake_Dis(void) 				// LID_SW----GPB1
{
	CLEAR_MASK(IER13, BIT3);
}

/*****************************************************
*****************************************************/

void SlpS3_Wake_En(void)  			// novo----GPD0
{
	//SET_MASK(IELMR0, Int_WKO20);	// select edge trigger mode 
	//SET_MASK(IPOLR0, Int_WKO20);	// select falling edge trigger

	//WUEMR2 = BIT0;				// 0: Rising-edge triggered is selected
	CLEAR_MASK(WUEMR2,BIT0); //ANGELAS071: add
	WUESR2 = BIT0;
	ISR0 = Int_WKO20;
	SET_MASK(IER0, Int_WKO20);
}
void SlpS3_Wake_Dis(void) 			// SlpS3----GPD0
{
	CLEAR_MASK(IER0, Int_WKO20);
}


//ANGELAS044:add start
void SlpS4_Wake_En(void)  			// SlpS4----GPD1
{
	WUESR2 = BIT1;
	ISR3 = Int_WKO21; //ANGELAS063: ISR0 TO ISR3
	SET_MASK(IER3, Int_WKO21); //ANGELAS063:IER0 TO IER3
}
void SlpS4_Wake_Dis(void) 			// SlpS4----GPD1
{
	CLEAR_MASK(IER3, Int_WKO21); //ANGELAS063:IER0 TO IER3
}
//ANGELAS063:add start
void SLPS3_Sleep_En(void)  			// novo----GPD0
{
	WUEMR2 |= BIT0;				// 1: falling-edge triggered is selected
	WUESR2 = BIT0;
	ISR0 = Int_WKO20;
	SET_MASK(IER0, Int_WKO20);
}
void SLPS4_Sleep_En(void)  			// novo----GPD0
{
	WUEMR2 |= BIT1;				// 1: falling-edge triggered is selected
	WUESR2 = BIT1;
	ISR3 = Int_WKO21;
	SET_MASK(IER3, Int_WKO21);
}
//ANGELAS063:add end
//72JERRY028: s+Modify Voice Wake signal status in deep sleep.
void SetWOV_Int(void)// WOV ----GPC5
{

	CLEAR_MASK(WUEMR11,WUC_WUI61);// rising-edge
	WUESR11 = WUC_WUI61;
	ISR14 = Int_WKO109;
	SET_MASK(IER14, Int_WKO109);
}
//72JERRY028: e+Modify Voice Wake signal status in deep sleep.
void InterKBDWakeEnable(void)
{
	 CLEAR_MASK(pDevStatus1,b2DisableKB);//MARTINY013:add
	KSOL=0x00;    //W120:sometimes from pad to NB kb can't wake up.        
	KSOH1=0x00;  //W120
	WUEMR3 = 0xFF;                  			// KSI0 ~ KSI7 falling edge
	WUESR3 = 0xFF;                  			// Clear WU20 ~ WU27 wakeup status
	WUENR3 = 0xFF;                  			// Enable WU20 ~ WU27
	ISR1 = Int_WKINTC;              			// Clear INT13 interrupt flag		
	SET_MASK(IER1,Int_WKINTC);      			// Enable INT13                			// Enable WU20 ~ WU27
	SET_MASK(IER1,Int_KB);      // enable KSI interrupt
}
void InterKBDWakeDisable(void)
{
	WUESR3 = 0xFF; //W131
	ISR1 = Int_WKINTC;              			// Clear INT13 interrupt flag	
	WUENR3 = 0; // W131: Clear interrupt
	CLEAR_MASK(IER1,Int_WKINTC);      	// Enable INT13                			// Enable WU20 ~ WU27
}
//ANGELAS044:add end

/*****************************************************
*****************************************************/
void PS2_Wake_En(void)
{
    WUENR1 |= BIT1;             		// Enable WU42
    WUEMR1 |= BIT1;             		// PS2DAT0
    WUESR1 = BIT1;              		// Clear WU42 wakeup status

}
void SetExTimer1(void)
{
	ETPSR = 0x00;
	ETCNTLHR = 0x7F;//0x06;
	ETCNTLLR = 0xF8;//0x66;					//Period 1 sec	//Period 50ms
	IER3 |= 0x40;							//ExTimer1
	/*
	ETPSR = 0x01;
	ETCNTLHR = 0x04;
	ETCNTLLR = 0x00;					//Period 31.25ms(26.91ms, Do Not Modify)
	IER3 |= 0x40;						//ExTimer1
	*/
}

void SetVGA_AC_DET(void)
{
	if( SystemIsS0 || SystemIsS3)
	{
		if (!Read_AC_IN())
		{
			AC_PRESENT_LOW();
		//	VGA_AC_DET_LO();
		}
		else
		{
			AC_PRESENT_HI();
           // if(SystemIsS0)
           // { VGA_AC_DET_HI(); }
           // else
           // { VGA_AC_DET_LO(); }
		}
	}
	if(SystemIsDSX)
	{
		TYPE_C_M0_LOW();
		TYPE_C_M1_LOW();
	}
	else
	{
		TYPE_C_M0_HI();
		TYPE_C_M1_HI();
	}
}
void SetAOU_DET_Int(void)//WU71 External Source from GPE1 WKO[71], to INT73 //HEGANGS021:one key wake
{
    SET_MASK(WUEMR7, BIT(1));//  Either-edge (rising-edge or falling-edge) triggered is selected.
	WUESR7 = BIT(1);         //  Clear wakeup status
	
	ISR9 = Int_WKO71;        // Clear interrupt status
    SET_MASK(IER9, Int_WKO71); // Enable interrupt
}

void WakeUp_DO_Function(void)
{
#if UCS1022_Support
    UCS1002_TimerX_FakeISR();
#endif	// UCS1022_Support

    if(ExtTimeCnt == 1)
    {
        UCS1002ID_Main();
        ExtTimeCnt=0;
    }
    else
    { 
        ExtTimeCnt++; 
    }

    ChkBattery_Percl();
	//Check_EC_ON();
	/*//ANGELAS007:Optimize power on sequence.
	//ANGELAS005:s+ Support UMA SKU EC_ON pin design.
	if(NTC_V1>682)
	{
		GPCRF0 = OUTPUT;
		SET_MASK(GPDRF,BIT(0));
	}
	else
	//ANGELAS005:e+ Support UMA SKU EC_ON pin design.
	*///ANGELAS007:Optimize power on sequence.
    //EC_ON_HI(); //ANGELAS038:remove  
	//	EC_ON2_HI();
    //EC_MainPwr_ON(); //ANGELAS044:remove

	if( Read_EC_NOVO() )
	{ uNovoVPCCount = 1; }
	else
	{ uNovoVPCCount = 0; }
	Init_VC(3);		// CMW Temp
}

void WakeUp_30ms_Function(void)
{
	Lenovo_LED();
}

/*****************************************************************************/
// Procedure: ISCT_Process									TimeDiv: 500mSec
// Description: Intel Smart Connect Technology process
// GPIO:
// Referrals:
/*****************************************************************************/
void ISCT_Process(void)
{
#if SW_ISCT
	if( (IS_MASK_SET(AOAC_STATUS, ISCT_Enable)) && (SystemNotS5) )		// Check ISCT enable?
	{
		if( IS_MASK_SET(nBatteryStatus1, ENEXIST) )		// Check BAT1 exist.
		{
			if( nBattGasgauge <= ISCT_BAT_LOW )			// Check BAT under 20%.
			{ 
				SET_MASK(AOAC_STATUS, ISCT_BAT_Capy); 
            }	// Battery under 20%
			else
			{ 
			    CLR_MASK(AOAC_STATUS, ISCT_BAT_Capy); 
            }	// Battery over 20%

			// Check BAT over 50 degree amd under -5 degree.
			//if( ( ( nBattAverTemp >= ISCT_BAT_OvTpLimit ) && !( nBattAverTemp & BATUnZeroTemp ) )
			//	|| ( (nBattAverTemp & BATUnZeroTemp) && ( nBattAverTemp <= ISCT_BAT_UnTpLimit ) ) )
			// Check BAT over 50 degree.
			if( nBattAverTemp >= ISCT_BAT_OvTpLimit )
			{ 
			    SET_MASK(AOAC_STATUS, ISCT_BAT_Temp); 
            }	// Battery Over 50 and under -5 degree.
			else
			{ 
			    CLR_MASK(AOAC_STATUS, ISCT_BAT_Temp); 
            }	// Battery under 50 and over -5 degree.
		}
		else
		{
			CLR_MASK(AOAC_STATUS, ISCT_BAT_Capy);		// Clear battery capacity of fail bit.
			CLR_MASK(AOAC_STATUS, ISCT_BAT_Temp);		// Clear battery temperature of fail bit.
		}

		if( SystemIsS3 )
		{
			if ( (AOAC_STATUS & 0xC0) != 0 )
			{
				SET_MASK(AOAC_STATUS, ISCT_S0_ISCT);	// Set S0_ISCT
				PM_PWRBTN_LOW();
			}
			ISCT_Behavior();
		}
	}
	else
	{
		AOAC_STATUS = 0xC0;		// Clear ISCT status.
		ISCT_Behavior();
	}
#endif	// SW_ISCT
}

/*****************************************************************************/
// Procedure: ISCT_Behavior
// Description: Intel Smart Connect Technology mode behavior.
// GPIO:
// Referrals:
/*****************************************************************************/
void ISCT_Behavior(void)
{
	#if SW_ISCT
	if( (IS_MASK_SET(AOAC_STATUS, ISCT_S0_ISCT)) && (SystemNotS5) )			// Can do AOAC.
	{
		// AOAC_PWRON();						// ISCT Turn on WLAN.
		uISCT |= 0x30;						// bit4:Disable FAN, bit5:Enable BKOFF.
		uISCT_2 |= 0x28;					// bit3:Enable Mute, bit4:Disable Camera, bit5:Disable Power LED.
		SET_MASK(pPROCHOT, b1ISCT_PROCHOTon);	// Set CPU Prochot function.
	}
	else
	{
		// AOAC_PWRON();						// ISCT Turn on WLAN.
		uISCT &= 0xCF;						// bit4:Enable FAN, bit5:Disable BKOFF.
		uISCT_2 |= 0xC7;					// bit3:Disable Mute, bit4:Enable Camera, bit5:Enable Power LED.
		CLR_MASK(pPROCHOT, b1ISCT_PROCHOTon);	// Clear CPU Prochot function.
	}
	#endif	// SW_ISCT
}

/*****************************************************************************/
// Procedure: ISCT_Timer										TimeDiv: 1 Min
// Description: Intel Smart Connect Technology timer.
// GPIO:
// Referrals:
/*****************************************************************************/
void ISCT_TimerCnt(void)
{
	#if SW_ISCT
	if ( (IS_MASK_SET(AOAC_STATUS, ISCT_Enable)) && (SystemIsS3) )	// Check ISCT enable?
	{
		if ( ISCT_Timer == 0 )
		{
			AOAC_STATUS |= 0x40;	// Set EC wake function.
		}
		else
		{ 
		    ISCT_Timer--; 
        }
	}
	#endif	// SW_ISCT
}

/*****************************************************************************/
// Procedure: GL887_Main									TimeDiv: 100mSec
// Description: Check USB charger of device.
// GPIO:
// Referrals: Genesys GL887 IC.
/*****************************************************************************/
#if Support_USB_Charge 
    #if UCS1022_Support
    #else
void GL887_Main(void)
{
	if( SystemIsS3 || SystemIsS5 )
	{
		if ( (IS_MASK_SET(SYS_STATUS,AC_ADP) || (nBattGasgauge > USB_BAT_LOW)) && (IS_MASK_CLEAR(USB_Charger, b2USB_TmlDis)) )
	 	{
			if ( (IS_MASK_SET(EMStatusBit, b1SetUSBChgEn)) || (SystemIsS3) )	// check to always charger.
	  		{
				USB_CHG_MOD_HI();	// Genesys Auto mode(DCP and Apple 2A).
				USB_CH_INPUT;		// Turn on USB charger port.
				return;
			}
		}
		USB_CHG_MOD_LOW();		// Genesys CDP Mode
		USB_CH_OUTPUT;			// Turn off USB charger port.
		USB_CH_LOW();
	}
	else
	{
		USB_CHG_MOD_LOW();		// Genesys CDP Mode
		USB_CH_INPUT;			// Turn on USB charger port.
	}
}
    #endif
#endif                     

