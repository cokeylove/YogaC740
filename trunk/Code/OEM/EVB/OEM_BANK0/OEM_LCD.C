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
		if ( (Read_ENBKL_IN()) && (IS_MASK_CLEAR(cCmd, b3BkOff)) && Read_LID_SW_IN()&&(IS_MASK_CLEAR(OEMControl, b1BkOff)))
		{
			//Enable Backlight after PWM stable
			if (BackLight_En_Delay != 0)
			{
				BackLight_En_Delay--;
			}
			else
			{
				BKOFF_ON();
				EC_TS_ON_HI(); //Add enable/disable touch panel pin.
				CLR_MASK(pProject0,b0DispToggleEn);
				return;
			}
		}
	}
	BKOFF_OFF();
	EC_TS_ON_LOW(); //Add enable/disable touch panel pin.
}

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