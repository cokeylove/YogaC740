/*-----------------------------------------------------------------------------
 * TITLE: OEM_TIMER.H
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *---------------------------------------------------------------------------*/

#ifndef OEM_TIMER_H
#define OEM_TIMER_H

/*===========================================================================
 * Timer count setup
 * ======================================================================== */
#define NOTIME          0		//Indicated Zero

#define Timer_800     	800  	//Indicated 800 count
#define Timer_700     	700  	//Indicated 700 count
#define Timer_600     	600  	//Indicated 600 count
#define Timer_500     	500  	//Indicated 500 count
#define Timer_400     	400  	//Indicated 400 count
#define Timer_300     	300  	//Indicated 300 count
#define Timer_200     	200  	//Indicated 200 count
#define Timer_100     	100  	//Indicated 100 count
#define Timer_80     	80  	//Indicated 80  count
#define Timer_50   		50		//Indicated 50  count
#define Timer_40   		40		//Indicated 40  count
#define Timer_30   		30		//Indicated 30  count
#define Timer_20		20		//Indicated 20  count
#define Timer_10		10		//Indicated 10  count
#define Timer_9			9 		//Indicated 9 	count
#define Timer_8			8 		//Indicated 8 	count
#define Timer_7			7 		//Indicated 7 	count
#define Timer_6			6 		//Indicated 6 	count
#define Timer_5			5 		//Indicated 5 	count
#define Timer_4			4 		//Indicated 4 	count
#define Timer_3			3 		//Indicated 3 	count
#define Timer_2			2 		//Indicated 2 	count
#define Timer_1			1		//Indicated 1 	count

extern void SystemAutoOn(void);
extern void InternalWDTNow(BYTE SID);
extern void EnableInternalWDT(void);
extern void DisableInternalWDT(void);
extern void ResetInternalWDT(void);

extern FLAG ChkTimeScale(PORT_BYTE_PNTR countadr, BYTE timeout);	// Timer center.
extern void EscScanCode(void);


#endif
