/*-----------------------------------------------------------------------------
 * TITLE: OEM_ADC.H
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef OEM_ADC_H
#define OEM_ADC_H

//----------------------------------------------------------------------------
// The function of scaning ADC input
//----------------------------------------------------------------------------
#define ADCChannel_0     0x80
#define ADCChannel_1     0x81
#define ADCChannel_2     0x82
#define ADCChannel_3     0x83
#define ADCChannel_4     0x84
#define ADCChannel_5     0x85
#define ADCChannel_6     0x86
#define ADCChannel_7     0x87

#define ECprochot_65W         436            //ANGELAS075:ADD //64W, 1.28V
#define ECprochot_45WL         214            //1.56 39w
#define ECprochot_45WH         256            //1.76 44w

#define A45WCP					126//37W -> 1.85A  //MEILING001:add.
#define A65WCP					201//56W -> 2.8A   0.56 //T051 change 191 to 201   59W  //MEILING001:add.

/*//ANGELAS073:remove start
#define IOUT						2.5
#define A90WCP					276//256//512 //(((75/20)/IOUT)/(3/1024))	//75W -> 3.75A   //276  //change 261 to 276   81W
#define A90Wchargerturbo		283//566 //(((83/20)/IOUT)/(3/1024))		//83W -> 4.15A         
#define A90Wthrottling			300//582 //(((85/20)/IOUT)/(3/1024))		//85W -> 4.25A          //300 // change 290 to 300 88W
#define A90Wthrottlingrecove	      	273 //F80W  //512 //(((75/20)/IOUT)/(3/1024)) 	//75W -> 3.75A
#define A90Wturborecover		238//477 //(((70/20)/IOUT)/(3/1024))		//70W -> 3.5A
*/ //ANGELAS073:remove end
/* //ANGELAS075 remove start
#define A65WCP					201//56W -> 2.8A   0.56 //T051 change 191 to 201   59W
#define A65Wchargerturbo		208//60W -> 3A            //T051 change 205 to 208   61W 
#define A65Wthrottling			218//62W -> 3.1A         //T051 change 212 to 218   64W    
//#define A65Wthrottlingrecove	198//T051F: 58W //55W -> 2.75A 
#define A65Wthrottlingrecove		208//T051F:61W //55W -> 2.75A G31:change198 to 208 61w
#define A65Wturborecover		171//50W -> 2.5A
// for 45w adapter
//#define A45WCP				123//36W -> 1.8A 
#define A45WCP					126//37W -> 1.85A 
#define A45Wchargerturbo		137//40W -> 2A
//#define A45Wthrottling			139//42W -> 2.1A
#define A45Wthrottling			145//44W -> 2.2AG31:change139 to 145 44w
//#define A45Wthrottlingrecove	120//35W -> 1.75A
#define A45Wthrottlingrecove		133//39W -> 1.95AG31:change 120 to 133 39w
#define A45Wturborecover		115//30W -> 1.5A
*/ //ANGELAS075 remove end
// for 45w adapter
/*//ANGELAS073:remove start
#define A120WCP				353//348//696 //(((102/20)/IOUT)/(3/1024))	//102W -> 5.1A
#define A120Wchargerturbo		385//771 //(((113/20)/IOUT)/(3/1024))	//113W -> 5.6A
#define A120Wthrottling			392//785 //(((115/20)/IOUT)/(3/1024))	//115W -> 5.7A
#define A120Wthrottlingrecove		348//696 //(((102/20)/IOUT)/(3/1024))	//102W -> 5.1A
#define A120Wturborecover		331//662 //(((97/20)/IOUT)/(3/1024))		//97W -> 4.85A

#define A170WCP				499//494//989 //(((145/20)/IOUT)/(3/1024))	//145W -> 7.25A
#define A170Wchargerturbo		546//1092//(((160/20)/IOUT)/(3/1024))	//160W -> 8A
#define A170Wthrottling			563//1126//(((165/20)/IOUT)/(3/1024))	//165W -> 8.25A
#define A170Wthrottlingrecove		494//989 //(((145/20)/IOUT)/(3/1024))	//145W -> 7.25A
#define A170Wturborecover		477//955 //(((140/20)/IOUT)/(3/1024))	//140W -> 7A

#define A90WACmodethrottling		291//582		//85W
#define A90WACmodethrottlingrecove	273 //80W   //512 	//75W

#define A120WACmodethrottling		392//785 	//115W
#define A120WACmodethrottlingrecove	348//696		//102W

#define A170WACmodethrottling		563//1126 	//165W
#define A170WACmodethrottlingrecove	494//989		//145W
*/ //ANGELAS073:remove end
#define AdapterID_NON_SUPPORT   0xFF
#define AdapterID_90W           0x00
#define AdapterID_120W          0x01
#define AdapterID_65W           0x02
#define AdapterID_170W          0x03
#define AdapterID_135W          0x04
#define AdapterID_45W           0x05
#define AdapterID_230W			0x06
/*//ANGELAS073:remove start
#define VC0TriggerValue			400	// 0.8V
#define VC0RestoreValue			573	// 1.8V

#define VC1TriggerValue			337
#define VC1TriggerValue_65W		341	// 0.78V //change 266 to 341:1V    
#define VC1TriggerValue_90W		460	// 0.99V //change 346 to 460:1.35V 
#define VC1TriggerValue_120W	403	// 1.3V
#define VC1TriggerValue_170W	573	// 1.85V
#define VC1RestoreValue			256	// 0.8V

#define VC1RestoreValue_65W		208	// 61W   
#define VC1RestoreValue_90W		283	// 83W

#define VC2TriggerValue			0
#define VC2RestoreValue			0
*/ //ANGELAS073:remove end

//----------------------------------------------------------------------------
// Define used ADC channel
//----------------------------------------------------------------------------
//#define FixCh0			ADCChannel_5	// Save to ADP_I
//#define DyCh0			ADCChannel_0	// Save to NTC_V1
//#define DyCh1			ADCChannel_3	// Save to IMVP_IMON
//#define DyCh2			ADCChannel_2	// Save to BATT_TEMP
//----------------------------------------------------------------------------
//#define FixData0			ADP_I
//#define DyData0			NTC_V1
//#define DyData1			CPU_I 
//#define DyData2			BATTEMP
//----------------------------------------------------------------------------

//extern void DisableADCChannel(void); //ANGELAS073:remove
//extern void EnableADCChannel(void); //ANGELAS073:remove
extern void ScanADCFixChannel(void);
extern void ScanADCDyChannel1(void);
extern void Init_ADC(void);
extern void Init_VC(BYTE Group);
//extern void Lenovo_CPU_auto_mode(void); //ANGELAS073:remove
//extern void Chk_Wrong_ADP(void); //ANGELAS073:remove
//extern void Chk_ACOP(void); //ANGELAS073:remove
//extern void Chk_TurboBoost(void); //ANGELAS073:remove
//extern void Chk_Hybrid_STPP(void); //ANGELAS073:remove
extern void Chk_AC_STPP(void);
extern void SetPowerBatteryparameter(void);
extern void SetPowerBatteryparameter2(void); //MEILING001:add.
extern void AdapterIdentification(void);
//extern void AdpID170WAmode(void); //ANGELAS073:remove
//extern void SetTurbo_V_Thro(void); //ANGELAS073:remove
//extern void RestoreTurbo_V_Thro(void);  //ANGELAS073:remove
//extern void Set_SD_PWR_EN(void);  //MEILING001:remove.
extern void Chk_HybridFORBQ24780_STPP(void); //ANGELAS047:add
#endif
