/*-----------------------------------------------------------------------------
 * TITLE: OEM_EVENT.H
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef OEM_INPUT_H
#define OEM_INPUT_H

//-----------------------------------------------------------------------------
// OEM_Event.C Debounce time setting
//-----------------------------------------------------------------------------
#define T_AC_DEBOUNCE    	    5       // 10 ms (timebase: 10 ms) //72JERRY022: AC_PRESENT# after resmrst#
#define T_PSW_DEBOUNCE   	    5   	// 50 ms (timebase: 10 ms)//ANGELAS007:Optimize power on sequence. 5 to 1 //ANGELAS031: 1 to 5
#define T_PSW_DEBOUNCE_S0   	100    	// 1s (timebase: 10 ms)  //LMLNANO008:add.
#define T_BAT_DEBOUNCE   	    15      // 150 ms (timebase: 10 ms)
#define T_NOVO_DEBOUNCE   	    5       // 50 ms (timebase: 10 ms)  //LMLNANO008:add.
#define T_PSWOFF         	    500 	// 5 secs (timebase: 10ms) EC 5sec override turn off system power
#define T_EXTEVT_DEBOUNCE 	    10      // 100 ms (timebase: 10 ms)
#define T_LAN_WAKE_DEBOUNCE   	30    	// 300 ms (timebase: 10 ms)
#define T_LID_DEBOUNCE          2      // 20ms change LID implement method.
#define T_VOICE_WAKE_DEBOUNCE   1
//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------
extern void EventManager(BYTE device_id);
extern void PowerBottonEvent(void);//72JERRY064:Modify press button 1s enter S3/S4/shutdown follow UI spec.
struct sDebounce
{
	XBYTE	*REG;
	BYTE	Flag;
	XBYTE	*Cunter;
	BYTE	Time;
	FUNCT_PTR_V_V	press;
	FUNCT_PTR_V_V	release;
	FUNCT_PTR_V_V	nochange;
};

//=============================================================================

#endif