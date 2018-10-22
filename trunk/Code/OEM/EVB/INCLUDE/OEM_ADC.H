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

#define ECprochot_65W         436             //64W, 1.28V
#define ECprochot_45WL         214            //1.56 39w
#define ECprochot_45WH         256            //1.76 44w

#define A45WCP					126           //37W -> 1.85A 
#define A65WCP					201           //56W -> 2.8A   0.56 //change 191 to 201   59W 

#define AdapterID_NON_SUPPORT   0xFF
#define AdapterID_90W           0x00
#define AdapterID_120W          0x01
#define AdapterID_65W           0x02
#define AdapterID_170W          0x03
#define AdapterID_135W          0x04
#define AdapterID_45W           0x05
#define AdapterID_230W			0x06

extern void ScanADCFixChannel(void);
extern void ScanADCDyChannel1(void);
extern void Init_ADC(void);
extern void Init_VC(BYTE Group);

extern void Chk_AC_STPP(void);
extern void SetPowerBatteryparameter(void);
extern void SetPowerBatteryparameter2(void);
extern void AdapterIdentification(void);

extern void Chk_HybridFORBQ24780_STPP(void);
#endif
