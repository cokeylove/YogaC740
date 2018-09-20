/*-----------------------------------------------------------------------------
 * TITLE: OEM_LCD.H
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef OEM_LCD_H
#define OEM_LCD_H

/*===========================================================================
 * PWM mode definition
 *===========================================================================*/
#define LCD_PWM_Frequency	6150	// PWM base frequency.
#define BRIGHT_MAX_STEP		0x0A	// Max. PWM duty step.//Change max brightness from 16 to 10 steps for win7 follow lenovo SPEC.
#define WIN8_BRI_MAX_STEP	0x15	// Max. PWM duty step.
#define BriTable_MAX		12		// Max. panel value.

#define	_LID_DEBOUNCE        2
#define _LID_CLOSE_DEBOUNCE  6 ////G66:Modify lid close debounce time from 1s to 3s. 


extern BYTE Cal_PWM_Duty(BYTE BCalDuty, BYTE BPWMBase);	// Calculate PWM duty.
/* ========================================================================
 * Brightness table define here
 * ======================================================================== */
extern BYTE code Bri_Table[BriTable_MAX][BRIGHT_MAX_STEP+4];
extern BYTE code WIN8_Bri_Tab[BriTable_MAX][WIN8_BRI_MAX_STEP+4];
extern void Backlight_Control(void);
extern void Lid_Process(void); //ANGELAG008: ADD 
extern void Voicewakeup_MFG_Process(void);//72JERRY065:Add CMD 0x59 test voice wake up for MFG.
#endif
