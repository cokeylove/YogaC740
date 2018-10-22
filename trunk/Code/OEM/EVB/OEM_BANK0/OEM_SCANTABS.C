/*-----------------------------------------------------------------
 * TITLE: OEM_SCANTABS.C - RC Table(s) for Scan Matix
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *----------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

const unsigned char code Rc_ROM_Tables[] =
{	// 15" US,UK and JP
    // Scanner Table 1  - 16 x 8 Scan Table 
    //00   01    02    03     04    05     06    07     08    09    10    11     12    13    14    15
	0x76,0xE3,0x61,0x00,0x34,0xFF,0xE5,0x64,0x52,0x00,0xE4,0x00,0x8A,0x00,0xED,0x00, // 00  
	0x0D,0xE2,0x58,0x00,0x2C,0x35,0x5B,0xE6,0x54,0x88,0x66,0x00,0x00,0x77,0x00,0x00, // 01  //72JERRY082: Modify KB SysRq function from Fn+Y to Fn+s by the latest UIspec.
	0xFE,0x24,0x1D,0x00,0x2D,0x3C,0x43,0x44,0xF9,0x00,0x6A,0x00,0xF2,0x00,0x00,0x82, // 02  
	0x0E,0xE1,0xE0,0x8C,0x2E,0x36,0x55,0xE7,0x4E,0x00,0xE8,0x91,0x00,0xF0,0xF4,0xF6, // 03  //LMLNANO002:change [3,11] from 0xF2 to 0x00, [3,13] from 0xF3 to 0x00, [3,15] from 0xF5 to 0xF6.
	0x1C,0x23,0xFB,0x00,0x2B,0x3B,0x42,0x4B,0x4C,0x00,0x5D,0x00,0x00,0x00,0x8E,0x84, // 04  //72JERRY082: Modify KB SysRq function from Fn+Y to Fn+s by the latest UIspec.
	0x16,0x26,0x1E,0x00,0x25,0x3D,0x3E,0x46,0x45,0x00,0xE9,0xEA,0xF3,0xEB,0x00,0xF7, // 05  //LMLNANO002:change [5,14] from 0xF7 to 0x00, [5,15] from 0xF6 to 0xF7.
	0x1A,0xFA,0x22,0x8D,0x2A,0x3A,0x41,0x49,0x5D,0x89,0x5A,0x00,0x00,0x00,0xF0,0x00, // 06  
	0x67,0x00,0x00,0x00,0xF8,0xFC,0x51,0x13,0x4A,0x00,0xFD,0xEE,0x8B,0xEC,0xEF,0x00, // 07  
//------------------------------------------------------------------------------------
//       00    01    02    03    04    05    06    07    08    09    0A    0B    0C    0D    0E    0F
//------------------------------------------------------------------------------------
// 0- 110  115   45     x     35    36    117  132   41     x    116     x     60     x      83    x
// 1-  16   114   30    x      21    22    28    118   27    44    15     x      x      90     x     x
// 2-  17    19    18    x      20    23    24    25    26     x     14      x     125   x      x     127
// 3-   1    113  112   58     6      7     13   119   12     x    120    126   x       x     80    81
// 4-  31    33    32    x      34    37    38    39    40     x     29     x      x       x     59    129
// 5-   2      4     3      x       5      8      9    10    11     x    121   122   124  123    x     85
// 6-  46    48    47   64     49    52    53    54    42    57    43     x      x       x     76     x
// 7- 131    x     x      x      50    51    56   133  	 55     x     61    84     62    89    79     x
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//      00    01    02    03    04    05    06    07    08    09    0A    0B    0C    0D    0E    0F
//------------------------------------------------------------------------------------
// 0- Esc   F4    \|     x      G      H     F6   132    '"      x     F5     x     LAlt   x     UpAr   x
// 1- Tab   F3  Caps   x      T      Y     ]}    F7     [{   LShf  Back   x     x    Nums   x      x
// 2-  Q     E    W      x       R     U      I      O      P      x ¢F14     x     Del    x       x    LWin
// 3- `~    F2   F1    LCtr   5%   6^   +=   F8     -_     x     F9    126   x      x     Play   Prev
// 4-  A     D    S      x        F     J       K     L      ;:      x     \|     x      x      x      Fn    129
// 5-  1!   3#  2@    x       4$    7&   8*    9(     0)      x    F10   F11   PrSc  F12    x    Next
// 6-  Z     C    X     RCtr    V     M     ,<    .>     #~  RShf  Entr   x      x      x      Ins    x
// 7- 131   x    x      x       B      N     56   133   /?     x    Spce  DnAr RAlt  RiAr  LeAr   x
//------------------------------------------------------------------------------------
};

const unsigned char code Rc_ROM_Tables_15BR[] =
{	// 15" Brazil
    // Scanner Table 1  - 16 x 8 Scan Table
    //00   01    02    03     04     05    06    07    08    09    10     11    12    13     14    15
	0x76,0xE3,0x61,0x00,0x34,0xFF,0xE5,0x64,0x52,0x00,0xE4,0x00,0x8A,0x00,0xED,0x00, // 00 
	0x0D,0xE2,0x58,0x00,0x2C,0x35,0x5B,0xE6,0x54,0x88,0x66,0x00,0x00,0x77,0x00,0x00, // 01  //72JERRY082: Modify KB SysRq function from Fn+Y to Fn+s by the latest UIspec.
	0xFE,0x24,0x1D,0x00,0x2D,0x3C,0x43,0x44,0xF9,0x00,0x6A,0x00,0xF2,0x00,0x00,0x82, // 02  
	0x0E,0xE1,0xE0,0x8C,0x2E,0x36,0x55,0xE7,0x4E,0x00,0xE8,0x91,0x00,0xF0,0xF4,0xF6, // 03   //LMLNANO002:change [3,11] from 0xF2 to 0x00, [3,13] from 0xF3 to 0x00, [3,15] from 0xF5 to 0xF6.
	0x1C,0x23,0xFB,0x00,0x2B,0x3B,0x42,0x4B,0x4C,0x00,0x5D,0x00,0x00,0x00,0x8E,0x84, // 04  //72JERRY082: Modify KB SysRq function from Fn+Y to Fn+s by the latest UIspec.
	0x16,0x26,0x1E,0x00,0x25,0x3D,0x3E,0x46,0x45,0x00,0xE9,0xEA,0xF3,0xEB,0x00,0xF7, // 05  //LMLNANO002:change [5,14] from 0xF7 to 0x00, [5,15] from 0xF6 to 0xF7.  
	0x1A,0xFA,0x22,0x51,0x2A,0x3A,0x41,0x49,0x5D,0x89,0x5A,0x00,0x00,0x00,0xF0,0x00, // 06 
	0x67,0x00,0x00,0x00,0xF8,0xFC,0x00,0x13,0x4A,0x00,0xFD,0xEE,0x8B,0xEC,0xEF,0x00, // 07
//------------------------------------------------------------------------------------
//        00    01    02    03    04    05    06    07    08    09    0A    0B    0C    0D    0E    0F
//------------------------------------------------------------------------------------
// 0-  110   115   45    x     35    36   117   132   41     x    116    x     60     x      83    x
// 1-  16    114    30    x     21    22    28   118   27     44   15     x      x      90     x     x
// 2-  17     19    18     x     20    23    24    25    26     x    14      x    125    x       x    127
// 3-   1     113  112   58     6     7      13   119   12     x    120   126   x       x      80   81
// 4-  31     33     32    x     34    37    38    39    40     x    29      x      x      x      59   129
// 5-   2       4      3     x       5     8       9    10    11     x    121  122   124   123    x     85
// 6-  46     48    47    56    49    52    53    54    42     57   43     x      x      x      76     x
// 7- 131     x      x     x      50    51    x    133    55     x     61    84    62    89     79     x
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//       00    01    02    03    04    05    06    07    08    09    0A    0B    0C    0D    0E    0F
//------------------------------------------------------------------------------------
// 0- Esc    F4     \|    x      G     H      F6   132    '"     x      F5     x     LAlt   x     UpAr   x
// 1- Tab   F3   Caps   x      T     Y      ]}    F7     [{   LShf  Back   x      x    Nums   x     x
// 2-  Q     E      W     x      R     U       I     O      P      x  ¢F14     x     Del    x       x    LWin
// 3- `~    F2    F1   LCtr   5%   6^    +=   F8     -_    x      F9    126    x      x     Play  Prev
// 4-  A     D      S      x      F     J       K     L       ;:     x      \|      x      x      x      Fn   129
// 5-  1!   3#    2@    x     4$    7&    8*    9(     0)     x      F10  F11   PrSc  F12     x   Next
// 6-  Z     C      X     56     V     M      ,<    .>     #~  RShf   Entr   x      x      x      Ins   x 
// 7- 131   x      x      x      B     N      x    133    /?    x      Spce DnAr RAlt  RiAr   LeAr  x
//------------------------------------------------------------------------------------
};

const unsigned char code EtKey_ROM_Tables[] =
{	//16   17   18		// 19X8 Scan Table
	0x6C,0x90,0x00,
	0x6B,0x75,0x00,
	0x7D,0x7B,0x00,  
	0x74,0x79,0x00, 
	0x69,0x73,0x00,
	0x70,0x72,0x00,
	0x7C,0x81,0x00,
	0x7A,0x71,0x00,
//------------------
//     16    17    18
//------------------
// 0- 91    95    x
// 1- 92    96    x
// 2- 101  105   x
// 3- 102  106   x
// 4- 93    97    x
// 5- 99    98    x
// 6- 100  108   x
// 7- 103  104   x
//------------------
//------------------
//     16   17   18
//------------------
// 0- 7    /      x
// 1- 4    8      x
// 2- 9    -      x
// 3- 6    +      x
// 4- 1    5      x
// 5- 0    2      x
// 6- *   Enter  x
// 7- 3    .       x
//------------------
};

const unsigned char code Rc_ROM_Tables_14[] =
{	// 14" US, UK and JP.
    // Scanner Table 1  - 16 x 8 Scan Table
     //00   01    02    03     04    05     06    07     08    09    10    11     12    13    14    15
	0x76,0xE3,0x61,0x00,0x34,0xFF,0xE5,0x64,0x52,0x00,0xE4,0x00,0x8A,0x00,0xED,0x00, // 00  
	0x0D,0xE2,0x58,0x00,0x2C,0x35,0x5B,0xE6,0x54,0x88,0x66,0x00,0x00,0x77,0x00,0x00, // 01  //72JERRY082: Modify KB SysRq function from Fn+Y to Fn+s by the latest UIspec.
	0xFE,0x24,0x1D,0x00,0x2D,0x3C,0x43,0x44,0xF9,0x00,0x6A,0x00,0xF2,0x00,0x00,0x82, // 02  
	0x0E,0xE1,0xE0,0x8C,0x2E,0x36,0x55,0xE7,0x4E,0x00,0xE8,0x91,0x00,0xF0,0xF4,0xF6, // 03  //LMLNANO002:change [3,11] from 0xF2 to 0x00, [3,13] from 0xF3 to 0x00, [3,15] from 0xF5 to 0xF6.
	0x1C,0x23,0xFB,0x00,0x2B,0x3B,0x42,0x4B,0x4C,0x00,0x5D,0x00,0x00,0x00,0x8E,0x84, // 04  //72JERRY082: Modify KB SysRq function from Fn+Y to Fn+s by the latest UIspec.
	0x16,0x26,0x1E,0x00,0x25,0x3D,0x3E,0x46,0x45,0x00,0xE9,0xEA,0xF3,0xEB,0x00,0xF7, // 05  //LMLNANO002:change [5,14] from 0xF7 to 0x00, [5,15] from 0xF6 to 0xF7.
	0x1A,0xFA,0x22,0x8D,0x2A,0x3A,0x41,0x49,0x5D,0x89,0x5A,0x00,0x00,0x00,0xF0,0x00, // 06  
	0x67,0x00,0x00,0x00,0xF8,0xFC,0x51,0x13,0x4A,0x00,0xFD,0xEE,0x8B,0xEC,0xEF,0x00,  
//------------------------------------------------------------------------------------
//       00    01    02    03    04    05    06    07    08    09    0A    0B    0C    0D    0E    0F
//------------------------------------------------------------------------------------
// 0- 110  115    45    x     35    36   117   132   41     x    116    x     60     x     83    x
// 1-  16   114    30    x     21    22    28   118   27     44    15    x      x      x      x     x
// 2-  17   19     18     x     20    23    24    25    26     x     14     x     x    	 x      x    127
// 3-   1   113   112   58     6      7     13   119   12     x    120   126   x      x      x    81
// 4-  31   33     32     x     34    37    38    39    40     x     29     x     x      x      59   129
// 5-   2    4       3      x      5      8      9     10    11     x    121   122  124  123   86    85
// 6-  46   48     47    64    49   52     53    54    42    57    43     x     x      x      76     x
// 7- 131   x      x      x     50    51    56    133   55     x    61     84   62     89    79     x
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//       00    01    02    03    04    05    06    07    08    09    0A    0B    0C    0D    0E    0F
//------------------------------------------------------------------------------------
// 0- Esc    F4    \|     x      G      H    F6    132    '"     x      F5     x     LAlt    x    UpAr  x
// 1- Tab   F3  Caps   x       T      Y    ]}     F7     [{   LShf  Back   x      x      x      x      x
// 2-  Q      E     W     x      R      U     I      O       P     x      14     x       x     x      x     LWin
// 3- `~    F2    F1   LCtr   5%   6^   +=    F8     -_    x      F9    126    x      x     x      x
// 4-  A      D     S     x       F      J     K      L       ;:     x      \|     x       x      x     Fn    129
// 5-  1!   3#   2@    x      4$    7&   8*    9(      0)     x     F10   F11   PrSc  F12   x      x
// 6-  Z     C      X   RCtr    V      M    ,<    .>      #~  RShf  Entr   x       x     x     Del     x
// 7- 131   x      x     x      B       N    56   133    /?    x     Spce  DnAr RAlt  RiAr  LeAr   x
//------------------------------------------------------------------------------------
};

const unsigned char code Rc_ROM_Tables_14BR[] =
{	// 14" Brazil
    // Scanner Table 1  - 16 x 8 Scan Table
     //00   01    02    03     04    05     06    07     08    09    10    11     12    13    14    15
	0x76,0xE3,0x61,0x00,0x34,0xFF,0xE5,0x64,0x52,0x00,0xE4,0x00,0x8A,0x00,0xED,0x00, // 00  
	0x0D,0xE2,0x58,0x00,0x2C,0x35,0x5B,0xE6,0x54,0x88,0x66,0x00,0x00,0x77,0x00,0x00, // 01  //72JERRY082: Modify KB SysRq function from Fn+Y to Fn+s by the latest UIspec.
	0xFE,0x24,0x1D,0x00,0x2D,0x3C,0x43,0x44,0xF9,0x00,0x6A,0x00,0xF2,0x00,0x00,0x82, // 02  
	0x0E,0xE1,0xE0,0x8C,0x2E,0x36,0x55,0xE7,0x4E,0x00,0xE8,0x91,0x00,0xF0,0xF4,0xF6, // 03  //LMLNANO002:change [3,11] from 0xF2 to 0x00, [3,13] from 0xF3 to 0x00, [3,15] from 0xF5 to 0xF6.
	0x1C,0x23,0xFB,0x00,0x2B,0x3B,0x42,0x4B,0x4C,0x00,0x5D,0x00,0x00,0x00,0x8E,0x84, // 04  //72JERRY082: Modify KB SysRq function from Fn+Y to Fn+s by the latest UIspec.
	0x16,0x26,0x1E,0x00,0x25,0x3D,0x3E,0x46,0x45,0x00,0xE9,0xEA,0xF3,0xEB,0x00,0xF7, // 05  //LMLNANO002:change [5,14] from 0xF7 to 0x00, [5,15] from 0xF6 to 0xF7.
	0x1A,0xFA,0x22,0x51,0x2A,0x3A,0x41,0x49,0x5D,0x89,0x5A,0x00,0x00,0x00,0xF0,0x00, // 06  
	0x67,0x00,0x00,0x00,0xF8,0xFC,0x00,0x13,0x4A,0x00,0xFD,0xEE,0x8B,0xEC,0xEF,0x00, 
};

const unsigned char code EtKey_ROM_Tables_1[] =
{	//16   17   18		// 19X8 Scan Table
	0x00,0x00,0x00,
	0x00,0x00,0x00,
	0x00,0x00,0x00,
	0x00,0x00,0x00,
	0x00,0x00,0x00,
	0x00,0x00,0x00,
	0x00,0x00,0x00,
	0x00,0x00,0x00,
};


const unsigned char code KB_S3WakeUP_Tables[] =
{
    // Scanner Table 1
	//00   01   02   03   04   05   06   07   08   09   10   11   12   13   14   15
	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55, //
 	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55, //
   	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55, //
 	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55, //
  	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55, //
  	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55, //
  	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55, //
  	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55  //
};

const unsigned char code EtKey_S3WakeUP_Tables[] =
{	//16   17   18		// 19X8 Scan Table
	0x55,0x55,0x55,
	0x55,0x55,0x55,
	0x55,0x55,0x55,
	0x55,0x55,0x55,
	0x55,0x55,0x55,
	0x55,0x55,0x55,
	0x55,0x55,0x55,
	0x55,0x55,0x55,
};

//----------------------------------------------------------------------------
// The function of hotkey Fn + F1
//----------------------------------------------------------------------------
void HotKey_Fn_F1(BYTE event)
{
	//MUTE
	if(IS_MASK_SET(SYS_MISC1,ACPI_OS))//: REMOVE MBID_READY judge
	{
		if ( event == MAKE_EVENT )
		{
		    Buffer_Key(0xE0);	/* Buffer E0h prefix */
		    Buffer_Key(0x23);	/* Buffer base code */
		}
		else
		{
			if ( event == BREAK_EVENT )
			{
	    		Buffer_Key(0xE0);	/* Buffer E0h prefix */
	    		Buffer_Key(0xF0);	/* Buffer base code */
	    		Buffer_Key(0x23);	/* Buffer base code */
			}
		}
	}
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F2
//----------------------------------------------------------------------------
void HotKey_Fn_F2(BYTE event)
{
	if(IS_MASK_SET(SYS_MISC1,ACPI_OS))// : REMOVE MBID_READY judge
	{
		if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
		{
			Buffer_Key(0xE0);	/* Buffer E0h prefix */
			Buffer_Key(0x21);	/* Buffer base code */
		}
		else
		{
			if (event == BREAK_EVENT)
			{
				Buffer_Key(0xE0);	/* Buffer E0h prefix */
				Buffer_Key(0xF0);	/* Buffer E0h prefix */
				Buffer_Key(0x21);	/* Buffer base code */
			} 
		}
	 }
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F3
//----------------------------------------------------------------------------
void HotKey_Fn_F3(BYTE event)
{
	if(IS_MASK_SET(SYS_MISC1,ACPI_OS))//// : REMOVE MBID_READY judge
	{
		if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
		{
			Buffer_Key(0xE0);	/* Buffer E0h prefix */
			Buffer_Key(0x32);	/* Buffer base code */
		}
		else
		{
			if (event == BREAK_EVENT)
			{
				Buffer_Key(0xE0);	/* Buffer E0h prefix */
				Buffer_Key(0xF0);	/* Buffer E0h prefix */
				Buffer_Key(0x32);	/* Buffer base code */	
			}
		} 
	}  
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F4
//----------------------------------------------------------------------------
void HotKey_Fn_F4(BYTE event)
{
	//MEILING029:S+ add send QEvent to APP for mic function.
	if(event == MAKE_EVENT)
    {
		if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
		{
	    	uVPCeventSource = 0;
		    uVPCeventSource2 = App_Control_Mic;
			ECSMI_SCIEvent(SDV_VPC_notify);     
		}
    }
	//MEILING029:E+.
	
	//MEILING029:S- remove send scan code to driver for mic function.
	//MEILING018:S+ add Mic enable/disable function.
	/*if(IS_MASK_SET(SYS_MISC1,ACPI_OS))
	{
		
		if (event == MAKE_EVENT)
		{	
			Buffer_Key(0xE0);	
			Buffer_Key(0x53);
		}
	}*/
	//MEILING018:E+.
	//MEILING029:E-.
	
	//MEILING006: S- remove close current window function.
	/*if(IS_MASK_SET(SYS_MISC1,ACPI_OS))
	 {
		if (event == MAKE_EVENT)
		{	
			Buffer_Key(0x11);	// ALT 	
			Buffer_Key(0x0C);	// F4 
		 }
		else
		{
			if (event == BREAK_EVENT)
			{
				Buffer_Key(0xF0);	
				Buffer_Key(0x11);	
				Buffer_Key(0xF0);	
				Buffer_Key(0x0C);	
			}
		}
	}*/
	//MEILING006: E-.
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F5
//----------------------------------------------------------------------------
void HotKey_Fn_F5(BYTE event)
{
	if(IS_MASK_SET(SYS_MISC1,ACPI_OS))
	{
		if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
		{
			Buffer_Key(0x03);
		}
		else
		{	
			if((event == BREAK_EVENT))
			{
				Buffer_Key(0xF0);
				Buffer_Key(0x03);
			}
		}
 	}  
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F6
//----------------------------------------------------------------------------
void HotKey_Fn_F6(BYTE event)
//TP enable/disable
{

//72JERRY056:s+Add enable/disable TP function by HW pin.
	if  (IS_MASK_SET(SYS_MISC1, ACPI_OS))
	{
		if(event == MAKE_EVENT)
			{
			if(READ_EC_TP_ON())
				{
				EC_TP_ON_LOW();
				SET_MASK(pDevStatus1, b7DisableTP);
				}
			else
				{
				EC_TP_ON_HI();
				CLEAR_MASK(pDevStatus1, b7DisableTP);
				}

			}
			uVPCeventSource = TouchPad;
			uVPCeventSource2 = 0;
			ECQEvent(SDV_VPC_notify);

		
		}
//72JERRY056:e+Add enable/disable TP function by HW pin.
		
	/*if ( (IS_MASK_SET(SYS_MISC1, ACPI_OS)) && (TouchPadCount == 0))
	{
   
		if(event == MAKE_EVENT)
		{
			if(IS_MASK_SET(pDevStatus1,b5TPDRIVER_STATUS)) 
			{
				  if(IS_MASK_SET(pDevStatus1,b7DisableTP)) 
				  {
					CLEAR_MASK(pDevStatus1,b1F6DisableTP);
					ECSMI_SCIEvent(0x3E);
					SET_MASK(pDevStatus3,b6EnDisTPSend); 
					if(IS_MASK_SET(pDevStatus1,b7DisableTP))
					{
					   SET_MASK(pDevStatus3,b7TPStateFormer);
					}
					else
					{
					   CLEAR_MASK(pDevStatus3,b7TPStateFormer);
					}  						
				 }
				 else  //Disable TP
				 {
					 SET_MASK(pDevStatus1,b1F6DisableTP);
					 ECSMI_SCIEvent(0x3F);
					 SET_MASK(pDevStatus3,b6EnDisTPSend); 
					 if(IS_MASK_SET(pDevStatus1,b7DisableTP))
					 {
						SET_MASK(pDevStatus3,b7TPStateFormer);
					 }
					 else
					 {
						CLEAR_MASK(pDevStatus3,b7TPStateFormer);
					 }			
				 }	
			}

		}
//ANGELAG008: add end
    }*/////72JERRY009:- Modify TP enable/disable function for Windows Precision Touchpads.
//72JERRY009:s+ Modify TP enable/disable function for Windows Precision Touchpads.
	/*if(IS_MASK_SET(SYS_MISC1,ACPI_OS))
    {

		if (event == MAKE_EVENT)
		{
			
			Buffer_Key(0x14); //"Ctrl" key
			
			Buffer_Key(0xe0); //
			Buffer_Key(0x1f); //"win" key
			
			Buffer_Key(0x5F); //"F24" key

		}
		else
		{
			if (event == BREAK_EVENT)
			{
				
				Buffer_Key(0xF0);
				Buffer_Key(0x14);
				Buffer_Key(0xe0);
				Buffer_Key(0xf0);
				Buffer_Key(0x1f);
				Buffer_Key(0xF0);
				Buffer_Key(0x5F);
				
			}
			//72JERRY043:s+Support PTP TP show OSD,there isn't driver notify EC TP status,EC keep flag to rom.
			 eEEPROMData=Read_Eflash_Byte(EEPROMA2,(EEPROMA1_B03|0x07),0xE2);
			 if(eEEPROMData==0xff)
			 	{
			 	pDevStatus1=0;
			 	}
			 else
			 	{
			 	pDevStatus1=eEEPROMData;
			 	}
			 
			CPL_MASK(pDevStatus1, b7DisableTP);

			eEEPROMBank= 0x07;
    		eEEPROMAddrsss = 0xE2;
    		eEEPROMData = pDevStatus1;
    		Update_EEPROMMark();
			if(uReserve07.fbit.nVPCDriverIn==1)   // Check VPC driver
			{
			  uVPCeventSource = TouchPad;
			  uVPCeventSource2 = 0;
			  ECQEvent(SDV_VPC_notify);
			  //72JERRY043:e+Support PTP TP show OSD,there isn't driver notify EC TP status,EC keep flag to rom.
		}
		}
	
	}
	//72JERRY009:e+ Modify TP enable/disable function for Windows Precision Touchpads.
	*///72JERRY051:Remove 72JERRY043 solution for PTP TP enable/disable.
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F7
//----------------------------------------------------------------------------
void HotKey_Fn_F7(BYTE event)
{
	//MEILING018:S+ add airplane fuciton.
	//MEILING006: S-remove airplane fuciton.
    if(event == MAKE_EVENT)
    {
		if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
		{
	    	uVPCeventSource = 0;
		    uVPCeventSource2 = App_Control_RF;
			ECSMI_SCIEvent(SDV_VPC_notify);     
			//ECSMI_SCIEvent(F7_Airplan_notify); 
		}
    }
    //MEILING006: E-.
    //MEILING018:E+.
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F8
//----------------------------------------------------------------------------
void HotKey_Fn_F8(BYTE event)
{
	//MEILING029:S+ add camera enable/disable function.
	if(event == MAKE_EVENT)
    {
		if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
		{
	    	nCameraPwrCHG;
		    uVPCeventSource = Camera;
		    uVPCeventSource2 = 0;
			ECSMI_SCIEvent(SDV_VPC_notify);
			CPL_MASK(pDevStus, pCamera);   
		}
    }
	//MEILING029:E+.
	
	//MEILING018:S- remove airplane fuciton.
	//MEILING006: S+ add airplane fuciton.
    /*if(event == MAKE_EVENT)
    	{
		if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
		{
	    	uVPCeventSource = 0;
		    uVPCeventSource2 = App_Control_RF;
			ECSMI_SCIEvent(SDV_VPC_notify);     
			//ECSMI_SCIEvent(F7_Airplan_notify); 
		}
    	}*/
    //MEILING006: E+.
    //MEILING018:E-.

	//MEILING006: S- remove view app function.
	/*if(IS_MASK_SET(SYS_MISC1,ACPI_OS))
	{
		if (event == MAKE_EVENT)
		{
			Buffer_Key(0x14);	 
    		Buffer_Key(0x11);	 
    		Buffer_Key(0x0D);	
		}
		else
		{
			if (event == BREAK_EVENT)
			{
				Buffer_Key(0xF0);
				Buffer_Key(0x14);
				Buffer_Key(0xF0);
    				Buffer_Key(0x11);
				Buffer_Key(0xF0);
    				Buffer_Key(0x0D);
			}
		}
	 }*/
	 //MEILING006: E-.
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F9
//----------------------------------------------------------------------------
void HotKey_Fn_F9(BYTE event)
{
	//MEILING018:S+ add Lock Screen function.
	if(IS_MASK_SET(SYS_MISC1,ACPI_OS))
    {
		if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
		{
			Buffer_Key(0xE0); //"WIN" key
			Buffer_Key(0x1F); 
			Buffer_Key(0x4B); //"L" key
		}
		else
		{
			if (event == BREAK_EVENT)
			{
				Buffer_Key(0xF0);
				Buffer_Key(0x4B);
				Buffer_Key(0xE0);
				Buffer_Key(0xF0);
				Buffer_Key(0x1F);
			}
		}
	}  
	//MEILING018:E+.

	//MEILING018:S- remove projector function.
	//MEILING006: S+ add change display mode function.(Fixed The display mode auto change to the nearly mode when change the mode via lenovo hotkey)
	/*if(IS_MASK_CLEAR(ACPI_HOTKEY, HotkeyDisable))
	{
	  	if(event == MAKE_EVENT)
		{
			if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
			{
				if( IS_MASK_CLEAR(SysStatus, FnF3WinKeyDn) )
				{
					e0_prefix_code(0x1F,event); // Win Key.
					SET_MASK(SysStatus, FnF3WinKeyDn);
				}
				simple_code(0x4D,event);		// P Key.
			}
		}
	
		if (event == BREAK_EVENT)
		{
			if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
			{
				simple_code(0x4D,event);	// P Key.
				SET_MASK(SysStatus, FnF3PKeyUp);
			}
		}
	}
	else
	{
		if(IS_MASK_SET(SYS_MISC1,ACPI_OS))
       	{
			if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
			{
				Buffer_Key(0xE0);
				Buffer_Key(0x1F);
				Buffer_Key(0x4D);
			}
			else
			{
				if (event == BREAK_EVENT)
				{
					Buffer_Key(0xF0);
					Buffer_Key(0x4D);
					Buffer_Key(0xE0);
					Buffer_Key(0xF0);
					Buffer_Key(0x1F);
				}
			}
		}
	}*/
	//MEILING006: E+.
	//MEILING018:E-.
	
	//MEILING006: S- remove turn on/off backlight function.
   	/*if(event == MAKE_EVENT)
    	{
		if ( IS_MASK_CLEAR(pProject0, b0DispToggleEn) )
		{
			SET_MASK(pProject0,b0DispToggleEn);
			SET_MASK(cCmd, b3BkOff);	// Turn off backlight.
		}
		else
		{
			CLR_MASK(cCmd, b3BkOff);	// Turn on backlight.
			uVPCeventSource = Inverter;
	    	uVPCeventSource2 = 0;
			ECSMI_SCIEvent(SDV_VPC_notify);
        	}
    	}*/
    //MEILING006: E-.
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F10
//----------------------------------------------------------------------------
void HotKey_Fn_F10(BYTE event)
{
	//MEILING018:S+ add projector function.(Fixed The display mode auto change to the nearly mode when change the mode via lenovo hotkey)
	if(IS_MASK_SET(ACPI_HOTKEY, HotkeyDisable))
	{
	  	if(event == MAKE_EVENT)
		{
			if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
			{
				if( IS_MASK_CLEAR(SysStatus, FnF3WinKeyDn) )
				{
					e0_prefix_code(0x1F,event); // Win Key.
					SET_MASK(SysStatus, FnF3WinKeyDn);
				}
				simple_code(0x4D,event);		// P Key.
			}
		}
	
		if (event == BREAK_EVENT)
		{
			if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
			{
				simple_code(0x4D,event);	// P Key.
				SET_MASK(SysStatus, FnF3PKeyUp);
			}
		}
	}
	else
	{
		if(IS_MASK_SET(SYS_MISC1,ACPI_OS))
       	{
			if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
			{
				Buffer_Key(0xE0);
				Buffer_Key(0x1F);
				Buffer_Key(0x4D);
			}
			else
			{
				if (event == BREAK_EVENT)
				{
					Buffer_Key(0xF0);
					Buffer_Key(0x4D);
					Buffer_Key(0xE0);
					Buffer_Key(0xF0);
					Buffer_Key(0x1F);
				}
			}
		}
	}
	//MEILING018:E+.
	
	//MEILING006: S- remove change display mode function.
	/*if(IS_MASK_SET(SYS_MISC1,ACPI_OS))
    	{
		if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
		{
			Buffer_Key(0xE0);
			Buffer_Key(0x1F);
			Buffer_Key(0x4D);
		}
		else
		{
			if (event == BREAK_EVENT)
			{
				Buffer_Key(0xF0);
				Buffer_Key(0x4D);
				Buffer_Key(0xE0);
				Buffer_Key(0xF0);
				Buffer_Key(0x1F);
			}
		}
	}*/
	//MEILING006: E-.
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F11
//----------------------------------------------------------------------------
void HotKey_Fn_F11(BYTE event)
{
    if( event != BREAK_EVENT )
    {
        if( cBrightDnDelayCnt == 0 )
        {
        	cBrightDnDelayCnt = Timer_5;
    		uVPCeventSource = VPCeventBrightness;
    		uVPCeventSource2 = 0;
            // Support for Windows 7 brightness control.
    		if( (SYS_STATUS & 0x07) != 0 )		// Check DOS mode.
    		{ 
    			if( (SYS_STATUS & 0x07) == 0x03 )	
                {
                	if( nBrightValue != 0 )
    				{
    					nBrightValue--;
    					cRecoveryBrightValue = nBrightValue;
    				}
                    if (uReserve07.fbit.nVPCDriverIn==1)	// Check VPC driver.
                    { 
    				    ECQEvent(SDV_VPC_notify);
                        ECQEvent(LCD_BRIG_DEC_EVENT); 
                    }
                }
                else
                {
    				ECQEvent(LCD_BRIG_DEC_EVENT); 
                }
            }	// 0x11 inform bios.
            //Support for Windows 7 brightness control.
    		else
    		{
    			if( nBrightValue != 0 )
    			{
    				nBrightValue--;
    				cRecoveryBrightValue = nBrightValue;
    			}
    		}
    	}
    	else
        { 
        	cBrightDnDelayCnt--; 
		}
    }
    else if( event == BREAK_EVENT )
    { 
    	cBrightDnDelayCnt = 0; 
	}

}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F12			[ Win + M ->  Win + Q ->  ESC ]
//----------------------------------------------------------------------------
void HotKey_Fn_F12(BYTE event)
{
   	if( event != BREAK_EVENT )
    {
        if( cBrightUpDelayCnt == 0 )
        {
            cBrightUpDelayCnt = Timer_5;
    		uVPCeventSource = VPCeventBrightness;
            uVPCeventSource2 = 0;
            //G80:+S Support for Windows 7 brightness control.
    		if( (SYS_STATUS & 0x07) != 0 )		// Check DOS mode.
    		{ 
    			if( (SYS_STATUS & 0x07) == 0x03 )	
                {
                	if( nBrightValue < (BRIGHT_MAX_STEP - 1) )
    				{
    					nBrightValue++;
    					cRecoveryBrightValue = nBrightValue;
    				}
                    if (uReserve07.fbit.nVPCDriverIn==1)	// Check VPC driver.
                    { 
    				    ECQEvent(SDV_VPC_notify);
                        ECQEvent(LCD_BRIG_INC_EVENT); 
                    }
                }
                else
               	{
    				ECQEvent(LCD_BRIG_INC_EVENT); 
                }
            }	// 0x11 inform bios.
            //Support for Windows 7 brightness control.
            else
    		{
    			if( nBrightValue < (BRIGHT_MAX_STEP - 1) )
    			{
    				nBrightValue++;
    				cRecoveryBrightValue = nBrightValue;
    			}
    		}
    	}
        else    
        { 
        	cBrightUpDelayCnt--;      //Modify the Windows 7 brightness control caused by mistake.
        }
	}
   	else if( event == BREAK_EVENT )
   	{
   		cBrightUpDelayCnt = 0;
    }
}

//----------------------------------------------------------------------------
// The function of hotkey Fn +Plus
//----------------------------------------------------------------------------
void HotKey_Fn_Plus(BYTE event)
{
    if(event == MAKE_EVENT)
    {
		if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
		{    
			// if(IS_MASK_SET(EM9_NEWFUN, b7_ShiftPress))  
            {      
	            SET_MASK(EM9_NEWFUN, b0_FnPlus);
		        uVPCeventSource = General;
		        uVPCeventSource2 = 0;
			 	ECSMI_SCIEvent(SDV_VPC_notify);
            }
		}
    }
}
//----------------------------------------------------------------------------
// The function of hotkey Fn +Plus
//----------------------------------------------------------------------------
void HotKey_Fn_Minus(BYTE event)
{
    if(event == MAKE_EVENT)
    {
		if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
		{
			// if(IS_MASK_SET(EM9_NEWFUN, b7_ShiftPress))  
            {      
	        	SET_MASK(EM9_NEWFUN, b1_FnMinus);
		       	uVPCeventSource = General;
		       	uVPCeventSource2 = 0;
				ECSMI_SCIEvent(SDV_VPC_notify);
            }
       	}
    }
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + ESC
//----------------------------------------------------------------------------
void HotKey_Fn_ESC(BYTE event)
{

}

//----------------------------------------------------------------------------
// The function of hotkey Fn + up arrow
//----------------------------------------------------------------------------
void HotKey_Fn_UP(BYTE event)
{
    //if((SYS_STATUS & 0x07) == 0x02)	// linpus
    //{
    //    e0_prefix_code(0x7F,event);	// ScanCode 0x54.
    //}
    //else
    //{
	/* 
    	if( event != BREAK_EVENT )
    	{
        	if( cBrightUpDelayCnt == 0 )
            {
            		cBrightUpDelayCnt = Timer_5;
    			uVPCeventSource = VPCeventBrightness;
                	uVPCeventSource2 = 0;
    			if( (SYS_STATUS & 0x07) != 0 )		// Check DOS mode.
    			{ ECQEvent(LCD_BRIG_INC_EVENT); }	// 0x12 inform bios.
    			else
    			{
    				if( nBrightValue < (BRIGHT_MAX_STEP - 1) )
    				{
    					nBrightValue++;
    					cRecoveryBrightValue = nBrightValue;
    				}
    				//if (uReserve07.fbit.nVPCDriverIn==1)	// Check VPC driver.
    				//{ ECQEvent(SDV_VPC_notify); }			// 0x44 for Levono used.
    			}
    		}
            else
            { cBrightUpDelayCnt--; }
    	}
    	else if( event == BREAK_EVENT )
    	{
            cBrightUpDelayCnt = 0;
        }
 	*/        
    //}
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + down arrow
//----------------------------------------------------------------------------
void HotKey_Fn_DOWN(BYTE event)
{
    //if((SYS_STATUS & 0x07) == 0x02)	// linpus
    //{
    //    e0_prefix_code(0x73,event);	// ScanCode 0x4C.
    //}
    //else
    //{
	/*
       if( event != BREAK_EVENT )
    	{
        	if( cBrightDnDelayCnt == 0 )
            {
            	cBrightDnDelayCnt = Timer_5;
    			uVPCeventSource = VPCeventBrightness;
    		    uVPCeventSource2 = 0;
    			if( (SYS_STATUS & 0x07) != 0 )		// Check DOS mode.
    			{ ECQEvent(LCD_BRIG_DEC_EVENT); }	// 0x11 inform bios.
    			else
    			{
    				if( nBrightValue != 0 )
    				{
    					nBrightValue--;
    					cRecoveryBrightValue = nBrightValue;
    				}
    				//if (uReserve07.fbit.nVPCDriverIn==1)	// Check VPC driver.
                	//{ ECQEvent(SDV_VPC_notify); }	// 0x44 for Levono used.
    			}
    		}
    		else
        	{ cBrightDnDelayCnt--; }
    	}
    	else if( event == BREAK_EVENT )
    	{ cBrightDnDelayCnt = 0; }
    	//}
	*/   
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + left arrow
//----------------------------------------------------------------------------
void HotKey_Fn_LEFT(BYTE event)
{
    if(event == MAKE_EVENT)
    {
		if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
		{       
	    	SET_MASK(EM9_NEWFUN, b0_FnPlus);
		    uVPCeventSource = General;
		    uVPCeventSource2 = 0;
			ECSMI_SCIEvent(SDV_VPC_notify);
		}
    }  
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + right arrow
//----------------------------------------------------------------------------
void HotKey_Fn_RIGHT(BYTE event)
{
    if(event == MAKE_EVENT)
    {
		if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
		{  
	    	SET_MASK(EM9_NEWFUN, b1_FnMinus);
		    uVPCeventSource = General;
		    uVPCeventSource2 = 0;
			ECSMI_SCIEvent(SDV_VPC_notify);
        }
    }
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + Space
//---------------------------------------------------------------------------
void HotKey_Fn_Space(BYTE event)
{
	/*if (event == MAKE_EVENT)
	{
		if(0x00==LED_KB_PWM_Step)   
        {
            LED_KB_PWM_Step=LED_KB_PWM_Step+2;   
        }     
        else
        {
            LED_KB_PWM_Step++;
        }  
		CLR_MASK(EMStatusBit, b2KBLEDChk);
		CLR_MASK(EMStatusBit, b0SetKBLEDON);
		UpdateLEDBL_delay = 6;			// Delay 3 sec for Update LED Back Light.
	}*///72JERRY022: -Modify GPIO setting follow SIT.

	//72JERRY022: S+Modify GPIO setting follow SIT.
	if(SystemIsS5 || SystemIsS3)
	{
		return;
	}

	
	if (event == MAKE_EVENT)
	{
		LED_KB_PWM_Step++; 
		if(LED_KB_PWM_Step == 3)
		{
			LED_KB_PWM_Step=0;
		}
//72JERRY036:s+ Modify KB backlight control for lenovo app setting.
		if(LED_KB_PWM_Step!=0)
		{
			SET_MASK(EMStatusBit, b0SetKBLEDON);
		}
		else
		{
			CLEAR_MASK(EMStatusBit, b0SetKBLEDON);
		}
//72JERRY036: e+Modify KB backlight control for lenovo app setting.	
		UpdateLEDBL_delay = 2;			// Delay 3 sec for Update LED Back Light.
		if(uReserve07.fbit.nVPCDriverIn==1)
		{
			uVPCeventSource = General;
    		uVPCeventSource2 = 0;
			ECSMI_SCIEvent(SDV_VPC_notify);
		}
//72JERRY022: E+Modify GPIO setting follow SIT.
	}
}
void HotKey_Fn_Q(BYTE event)
{
//	return;
	if(SystemIsS5 || SystemIsS3)
	{
		return;
	}
	if(Read_YOGA_ID())
	{
		return;
	}
	if (event == MAKE_EVENT)
	{
		if(ThermalMode == ThermalMode_BALANCE)
		{
			ThermalMode = ThermalMode_HIGHP;
		}
		else
		{
			ThermalMode = ThermalMode_BALANCE;
		}
		ECSMI_SCIEvent(Fn_Q_EVENT);
		uVPCeventSource = FnQ;
    	uVPCeventSource2 = 0;
		ECSMI_SCIEvent(SDV_VPC_notify);
	}
}
void HotKey_Fn_H(BYTE event)
{
	return;
}
void HotKey_Fn_B(BYTE event)
{
	return;
}

const FUNCT_PTR_V_B code HotKey_Fn_Fx[] =
{
	HotKey_Fn_F1,    	// Fn + F1
	HotKey_Fn_F2, 		// Fn + F2
	HotKey_Fn_F3, 		// Fn + F3
	HotKey_Fn_F4,       // Fn + F4
	HotKey_Fn_F5,    	// Fn + F5
	HotKey_Fn_F6, 		// Fn + F6
	HotKey_Fn_F7, 		// Fn + F7
	HotKey_Fn_F8,       // Fn + F8
	HotKey_Fn_F9,    	// Fn + F9
	HotKey_Fn_F10, 		// Fn + F10
	HotKey_Fn_F11, 		// Fn + F11
	HotKey_Fn_F12,    	// Fn + F12
	HotKey_Fn_ESC,    	// Fn + ESC
//	HotKey_Fn_UP,    	// Fn + up arrow
//	HotKey_Fn_DOWN,    	// Fn + down arrow
    HotKey_Fn_Plus,     // Fn+Plus
    HotKey_Fn_Minus,    // Fn+Plus
	HotKey_Fn_LEFT,    	// Fn + left arrow   //:Used for Fn+ "+"(small keyboard)
	HotKey_Fn_RIGHT,  	// Fn + right arrow  //:Used for Fn+ "-"(small keyboard)
	HotKey_Fn_Space,	// Fn + Space
	HotKey_Fn_Q,        // Fn + Q
	HotKey_Fn_H,        // Fn + H
	HotKey_Fn_B,        // Fn + B
};

//---------------------------------------------------------------------------
//	Numpad 7 Home	[B0h]
//	Numpad 8 UP  	[B1h]
//	Numpad 9 PGUp	[B2h]
//	Numpad *   		[B3h]
//	Numpad 4 Left	[B4h]
//	Numpad 5 		[B5h]
//	Numpad 6 Right	[B6h]
//	Numpad -  		[B7h]
//	Numpad 1 End	[B8h]
// 	Numpad 2 Down	[B9h]
//	Numpad 3 PGDn	[BAh]
//	Numpad + 		[BBh]
//	Numpad 0 Ins	[BCh]
// 	Numpad . Del	[BDh]
//	Numpad /      	[BEh]
//---------------------------------------------------------------------------

const BYTE code ss2_9C[] =	// Numpad 7
{
    0x3D,   // Normal 		// 7
    0xB0,   // Numlock
    0xB0,   // Fn
    0xB0    // Fn+Numlock
};

const BYTE code ss2_9D[] =	// Numpad 8
{
    0x3E,   // Normal 		// 8
    0xB1,   // Numlock
    0xB1,   // Fn
    0xB1    // Fn+Numlock
};

const BYTE code ss2_9E[] = 	// Numpad 9
{
    0x46,   // Normal 		// 9
    0xB2,   // Numlock
    0xB2,   // Fn
    0xB2    // Fn+Numlock
};

const BYTE code ss2_9F[] = 	// Numpad 0
{
    0x45,   // Normal 		// 0
    0xBE,   // Numlock
    0xBE,   // Fn
    0xBE    // Fn+Numlock
};

const BYTE code ss2_A0[] = 	// Numpad U
{
    0x3C,   // Normal 		// U
    0xB4,   // Numlock
    0xB4,   // Fn
    0xB4    // Fn+Numlock
};

const BYTE code ss2_A1[] = 	// Numpad I
{
    0x43,   // Normal 		// I
    0xB5,   // Numlock
    0xB5,   // Fn
    0xB5    // Fn+Numlock
};

const BYTE code ss2_A2[] =	// Numpad O
{
    0x44,   // Normal 		// O
    0xB6,   // Numlock
    0xB6,   // Fn
    0xB6    // Fn+Numlock
};

const BYTE code ss2_A3[] =	// Numpad P
{
    0x4D,   // Normal 		// P
    0xB3,   // Numlock
    0xB3,   // Fn
    0xB3    // Fn+Numlock
};

const BYTE code ss2_A4[] = 	// Numpad J
{
    0x3B,   // Normal 		// J
    0xB8,   // Numlock
    0xB8,   // Fn
    0xB8    // Fn+Numlock
};

const BYTE code ss2_A5[] = 	// Numpad K
{
    0x42,   // Normal 		// K
    0xB9,   // Numlock
    0xB9,   // Fn
    0xB9    // Fn+Numlock
};

const BYTE code ss2_A6[] =	// Numpad L
{
    0x4B,   // Normal 		// L
    0xBA,   // Numlock
    0xBA,   // Fn
    0xBA    // Fn+Numlock
};

const BYTE code ss2_A7[] = 	// Numpad ;:
{
    0x4C,   // Normal 		// ;:
    0xB7,   // Numlock
    0xB7,   // Fn
    0xB7    // Fn+Numlock
};

const BYTE code ss2_A8[] =	// Numpad M
{
    0x3A,   // Normal 		// M
    0xBC,   // Numlock
    0xBC,   // Fn
    0xBC    // Fn+Numlock
};

const BYTE code ss2_A9[] = 	// Numpad ,<
{
    0x41,   // Normal 		// ,<
    0x41,   // Numlock
    0x41,   // Fn
    0x41    // Fn+Numlock
};

const BYTE code ss2_AA[] = 	// Numpad .>
{
    0x49,   // Normal 		// .>
    0xBD,   // Numlock
    0xBD,   // Fn
    0xBD    // Fn+Numlock
};

const BYTE code ss2_AB[] = 	// Numpad /?
{
    0x4A,   // Normal 		// /?
    0xBB,   // Numlock
    0xBB,   // Fn
    0xBB    // Fn+Numlock
};

const BYTE code ss2_AC[] =
{
    0x5A,   // Normal
    0x81,   // Fn
};
//----------------------------------------------------------------
// Function key F1 ~ F12
//----------------------------------------------------------------
const BYTE code ss2_AD[] = 	// F1
{
    0x05,   // Normal
	//0x86,   // Sleep event
    0xD0,   // Fn
};

const BYTE code ss2_AE[] = 	// F2
{
    0x06,   // Normal
    0xD1,   // Fn
};

const BYTE code ss2_AF[] = 	// F3
{
    0x04,   // Normal
    0xD2,   // Fn
};

const BYTE code ss2_B0[] =	// F4
{
    0x0C,   // Normal
    0xD3,   // Fn
};

const BYTE code ss2_B1[] = 	// F5
{
    0x03,   // Normal
    0xD4,   // Fn
};

const BYTE code ss2_B2[] = 	// F6
{
    0x0B,   // Normal
    0xD5,   // Fn
};

const BYTE code ss2_B3[] = 	// F7
{
    0x80,   // Normal
    0xD6,   // Fn
};

const BYTE code ss2_B4[] = 	// F8
{
    0x0A,   // Normal
    0xD7,   // Num Lk   //T08H: from 0x77 to 0xD7 
};

const BYTE code ss2_B5[] = 	// F9
{
    0x01,   // Normal
	//0x9F,   // Play/Pause event
	0xD8,
};

const BYTE code ss2_B6[] = 	// F10
{
    0x09,   // Normal
	//0x9E,   // Stop event
	0xD9,
};

const BYTE code ss2_B7[] = 	// F11
{
    0x78,   // Normal
	//0x9D,   // Prev Track event
	0xDA,
};

const BYTE code ss2_B8[] = 	// F12
{
    0x07,   // Normal
    0xDB,
};

/* ----------------------------------------------------------------------------
 * FUNCTION: sskey2_A2_table
 *
 * The A2_TABLE structure contains the following.
 *
 * BYTE comb;  Bit   Setting Combination Key
 *            ---   -----------------------
 *             7    Hook_calc_index_comb_BIT7
 *             6    Hook_calc_index_comb_BIT6
 *             5    Fn
 *             4    Numlock
 *             3    Ctrl
 *             2    Alt
 *             1    Shift
 *             0    reserved
 *
 * BYTE *pntr; Pointer to array for key.
 * ------------------------------------------------------------------------- */
const A2_TABLE sskey2_A2_table[] =
{											// Keyboard matrix index
	// Keypad -->
    {0x30, ss2_9C },    /* index = 00h */ 	// 0x9C
    {0x30, ss2_9D },    /* index = 01h */	// 0x9D
    {0x30, ss2_9E },    /* index = 02h */	// 0x9E
    {0x30, ss2_9F },    /* index = 03h */	// 0x9F
    {0x30, ss2_A0 },    /* index = 04h */	// 0xA0
    {0x30, ss2_A1 },    /* index = 05h */	// 0xA1
    {0x30, ss2_A2 },    /* index = 06h */	// 0xA2
    {0x30, ss2_A3 },    /* index = 07h */	// 0xA3
    {0x30, ss2_A4 },    /* index = 08h */	// 0xA4
    {0x30, ss2_A5 },    /* index = 09h */	// 0xA5
    {0x30, ss2_A6 },    /* index = 0Ah */	// 0xA6
    {0x30, ss2_A7 },    /* index = 0Bh */	// 0xA7
    {0x30, ss2_A8 },    /* index = 0Ch */	// 0xA8
    {0x30, ss2_A9 },  	/* index = 0Dh */	// 0xA9
    {0x30, ss2_AA },  	/* index = 0Eh */	// 0xAA
    {0x30, ss2_AB },   	/* index = 0Fh */	// 0xAB
    {0x20, ss2_AC },  	/* index = 10h */	// 0xAC
	// Keypad <--

	// F1~F12 hotkeys -->
    {0x20, ss2_AD },    /* index = 11h */	// 0xAD
    {0x20, ss2_AE },    /* index = 12h */	// 0xAE
    {0x20, ss2_AF },    /* index = 13h */	// 0xAF
    {0x20, ss2_B0 },    /* index = 14h */	// 0xB0
    {0x20, ss2_B1 },    /* index = 15h */	// 0xB1
    {0x20, ss2_B2 },    /* index = 16h */	// 0xB2
    {0x20, ss2_B3 },    /* index = 17h */	// 0xB3
    {0x20, ss2_B4 },    /* index = 18h */	// 0xB4
    {0x20, ss2_B5 },  	/* index = 19h */	// 0xB5
    {0x20, ss2_B6 },  	/* index = 1Ah */	// 0xB6
    {0x20, ss2_B7 },   	/* index = 1Bh */	// 0xB7
    {0x20, ss2_B8 }     /* index = 1Ch */	// 0xB8
    // F1~F12 hotkeys <--
};
											// Reserved 0xB9 ~ 0xDF

/* ----------------------------------------------------------------------------
 * FUNCTION: sskey2_overlay_table
 * ------------------------------------------------------------------------- */
const BYTE code sskey2_overlay_table[] =
{//Normal   Fn   	// Keyboard matrix index
	0x05,0xD0,	// E0H    F1 <-> Mute 
	0x06,0xD1,	// E1H	F2 <-> Vol Down 
	0x04,0xD2,	// E2H	F3 <-> Vol Up   
	0x0C,0xD3,  // E3H    F4 <-> Mic            
	0x03,0xD4,	// E4H    F5 <-> Reflash 	  	 	
	0x0B,0xD5,  // E5H    F6 <-> Touchpad
	0x80,0xD6,  // E6H    F7 <-> Airplane
	0x0A,0xD7,  // E7H    F8 <-> Camera        
	0x01,0xD8,  // E8H    F9 <-> Screen Lock        
	0x09,0xD9,  // E9H    F10 <-> Display mode
	0x78,0xDA,	// EAH    F11 <-> Brightness Down         
	0x07,0xDB,	// EBH    F12 <-> Brightness Up  
	0x9B,0x95,  // ECH   	RightArrow <-> End  //MEILING004:change (0x76,0x00) to (0x95,0x9B)  //MEILING015:switch the function order.   	 
	0x98,0x96,	// EDH	UpArrow <-> PgUp  //MEILING004:change (0x98,0x00) to (0x96,0x98)	 //MEILING015:switch the function order. 
	0x99,0x97,	// EEH	DnArrow <-> PgDn //MEILING004:change (0x99,0x00) to (0x97,0x99)    //MEILING015:switch the function order. 
	0x9A,0x94,	// EFH	LeftArrow <-> Home  //MEILING004:change (0x9A,0x00) to (0x94,0x9A)  //MEILING015:switch the function order. 	 
	0xC2,0xC2,	// F0H	Insert  	
	0xC3,0xC2,  // F1H	PrtSc <-> Insert  //LMLNANO002:add PrtSc/Insert key.    	
	0xC0,0xC0,  // F2H   	Delete
	0xC3,0xC3,  // F3h 	PrtSc		  	 		 
	0x9F,0x9F,  // F4h 	Play/Pause   //MEILING007:change to Play/Pause event.				 
	0x9E,0x9E,  // F5h 	Stop 	    //MEILING007:change to Stop event.	  	 
	0x9D,0x9D,  // F6h 	Prev Track  //MEILING007:change to Prev Track event.	//MEILING032:modify "Prev Track" to "Fast Backward".	//MEILING034:change "www.Back" to "Prev Track".		  
	0x9C,0x9C,  // F7h 	Next Track  //MEILING007:change to Next Track event.	//MEILING032:modify "Next Track" to "Fast Forward".	//MEILING034:change "www.Forward" to "Next Track".			 
	0x32,0x92, 	// F8h	B <-> Break  //MEILING020:change (0x94,0x91) to (0x32,0x92)			  
	0x4D,0x91, 	// F9h 	P <-> Pause  //MEILING020:change (0x96,0x7E) to (0x4D,0x91)	
	0x21,0x7E,  // FAh	C <-> SrcLk  //MEILING020:change (0x97,0xC2) to (0x21,0x7E)
	0x1B,0x93,	// FBh	S <-> SysRq //MEILING020:change (0x95,0x92) to (0x1B,0x93)  //MEILING027:change (0x1B,0x93) to (0x35,0x93). //72JERRY082: Modify KB SysRq function from Fn+Y to Fn+s by the latest UIspec.
	0x31,0x31,	// FCh	N <-> Balance mode	
	0x29,0xE1,	// FDh   Space <-> KB Backlight
	0x15,0xE2,	// FEh	Q <-> Quiet mode
	0x33,0x33,	// FFh	H <-> HighPerf mode
};
												
const BYTE code sskey2_overlay_table1[] =
{//Normal   Fn   	// Keyboard matrix index
	0xD0,0x05,	// E0H    Mute <-> F1     	 
	0xD1,0x06,	// E1H	Vol Down <-> F2	  	 
	0xD2,0x04,	// E2H	Vol Up <-> F3	  	   
	0xD3,0x0C,  // E3H    Mic <-> F4            
	0xD4,0x03,	// E4H    Reflash <-> F5 	  	 	
	0xD5,0x0B,  // E5H    Touchpad <-> F6
	0xD6,0x80,  // E6H    Camera <-> F7
	0xD7,0x0A,  // E7H    Airplane <-> F8           
	0xD8,0x01,  // E8H    Display mode <-> F9        
	0xD9,0x09,  // E9H    F10           
	0xDA,0x78,	// EAH    Brightness Down <-> F11 	                
	0xDB,0x07,  // EBH    Brightness Up <-> F12	  	  
	0x9B,0x95,  // ECH    RightArrow <-> End  //MEILING004:change (0x00,0x76) to (0x9B,0x95)   	 
	0x98,0x96,	// EDH	UpArrow <-> PgUp   //MEILING004:change (0x00,0x98) to (0x98,0x96)	
	0x99,0x97,	// EEH	DnArrow <-> PgDn  //MEILING004:change (0x00,0x99) to (0x99,0x97)	
	0x9A,0x94,	// EFH	LeftArrow <-> Home  //MEILING004:change (0x00,0x9A) to (0x9A,0x94)	 
	0xC2,0xC2, 	// F0H	Insert  	
	0xC3,0xC2,  // F1H	PrtSc <-> Insert  //LMLNANO002:add PrtSc/Insert key.      	
	0xC0,0xC0,  // F2H    Delete  //
	0xC3,0xC3,  // F3h 	PrtSc	//	  	 		 
	0x9F,0x9F,  // F4h 	Play/Pause   //MEILING007:change to Play/Pause event.				 
	0x9E,0x9E,  // F5h 	Stop 	    //MEILING007:change to Stop event.	  	 
	0x9D,0x9D,  // F6h 	Prev Track  //MEILING007:change to Prev Track event.	//MEILING032:modify "Prev Track" to "Fast Backward".	//MEILING034:change "www.Back" to "Prev Track".			  
	0x9C,0x9C,  // F7h 	Next Track  //MEILING007:change to Next Track event.	//MEILING032:modify "Prev Track" to "Fast Backward".	//MEILING034:change "www.Forward" to "Next Track".	 			 
	0x32,0x92, 	// F8h	B <-> Break  //MEILING020:change (0x94,0x91) to (0x32,0x92)			  
	0x4D,0x91, 	// F9h 	P <-> Pause  //MEILING020:change (0x96,0x7E) to (0x4D,0x91)	
	0x21,0x7E,  // FAh	C <-> SrcLk  //MEILING020:change (0x97,0xC2) to (0x21,0x7E)
	0x1B,0x93,	// FBh	S <-> SysRq //MEILING020:change (0x95,0x92) to (0x1B,0x93)  //MEILING027:change (0x1B,0x93) to (0x35,0x93). //72JERRY082: Modify KB SysRq function from Fn+Y to Fn+s by the latest UIspec.
	0x31,0x31,	// FCh	N <-> Balance mode	
	0x29,0xE1,	// FDh   Space <-> KB Backlight
	0x15,0xE2,	// FEh	Q <-> Quiet mode
	0x33,0x33,	// FFh	H <-> HighPerf mode
};
												// Reserved 0xEE ~ 0xFF
/* ----------------------------------------------------------------------------
 * Insyde Software Key Number 2
 *
 * 00h         Null
 * 01h - 7Fh   Same as IBM Scan Code, Set2
 * 80h - BFh   Pre-Index for generation Scan Code, Set2
 * C0h - DFh   Pre-Index for PPK function
 * C0h - DFh   Pre-Index for SMI function
 * ------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------
 * FUNCTION: sskey3_80_table - Insyde Software Key Number 2
 * 80h - BFh   Pre-Index for generation Scan Code, Set2
 * ------------------------------------------------------------------------- */
const BYTE code sskey3_80_table[] =
{
//-----------------------------------------------------------------
// Index from keyboard matrix
//-----------------------------------------------------------------
									// Keyboard matrix index
    0x83, 	DO_SIMPLE_CODE,       	// [80h] F7 (118)
    0x5A, 	DO_E0_PREFIX_CODE,    	// [81h] Numpad Enter (108)
    0x1F, 	DO_E0_PREFIX_CODE,    	// [82h] Windows Left
    0x27, 	DO_E0_PREFIX_CODE,    	// [83h] Windows Right
    0x2F, 	DO_E0_PREFIX_CODE,    	// [84h] Application
    0x37, 	DO_E0_PREFIX_CODE,    	// [85h] Power event
    0x3F, 	DO_E0_PREFIX_CODE,    	// [86h] Sleep event
    0x5E, 	DO_E0_PREFIX_CODE,		// [87h] Wake event
    LShift, DO_CONTROL_EFFECT,		// [88h] 12 Shift(L)
    RShift, DO_CONTROL_EFFECT,		// [89h] 59 Shift(R)
    LAlt, 	DO_CONTROL_EFFECT,    	// [8Ah] 11 Alt(L)
    RAlt, 	DO_CONTROL_EFFECT_E0,	// [8Bh] E0 11 Alt(R)
    LCtrl, 	DO_CONTROL_EFFECT, 		// [8Ch] 14 Crtl(L)
    RCtrl, 	DO_CONTROL_EFFECT_E0,	// [8Dh] E0 14 Ctrl(R)
    FN, 	DO_CONTROL_EFFECT,    	// [8Eh] Fn
    OVLAY,	DO_CONTROL_EFFECT,		// [8Fh] Overlay
    0x4A, 	DO_E0_PREFIX_CODE,      // [90h] Reserved msmart "key pad /"
    0x00, 	DO_SPECIAL_CODE,      	// [91h] Pause (126)
    0x01, 	DO_SPECIAL_CODE,      	// [92h] Break
    0x84, 	DO_SIMPLE_CODE,       	// [93h] (124) Alt-Case (SysRq)
    0x6C, 	DO_E0_PREFIX_CODE,    	// [94h] Home
    0x69, 	DO_E0_PREFIX_CODE,    	// [95h] End
    0x7D, 	DO_E0_PREFIX_CODE,  	// [96h] Page up
    0x7A, 	DO_E0_PREFIX_CODE,    	// [97h] Page down
    0x75, 	DO_E0_PREFIX_CODE,    	// [98h] UP arrow
    0x72, 	DO_E0_PREFIX_CODE,    	// [99h] Down arrow
    0x6B, 	DO_E0_PREFIX_CODE,  	// [9Ah] Left arrow
    0x74, 	DO_E0_PREFIX_CODE,   	// [9Bh] Right arrow

//-----------------------------------------------------------------
// For MicroSoft enhance keyboard feature.
//-----------------------------------------------------------------
    0x4D, DO_E0_PREFIX_CODE,    	// [9Ch] Next Track event
    0x15, DO_E0_PREFIX_CODE,    	// [9Dh] Prev Track event
    0x3B, DO_E0_PREFIX_CODE,    	// [9Eh] Stop event
    0x34, DO_E0_PREFIX_CODE,    	// [9Fh] Play/Pause event
    0x23, DO_E0_PREFIX_CODE,    	// [A0h] Mute event
    0x32, DO_E0_PREFIX_CODE,    	// [A1h] Volume Up event
    0x21, DO_E0_PREFIX_CODE,    	// [A2h] Volume Down event
    0x48, DO_E0_PREFIX_CODE,    	// [A3h] Mail event
    0x10, DO_E0_PREFIX_CODE,    	// [A4h] Search event
    0x3A, DO_E0_PREFIX_CODE,    	// [A5h] Web/Home event
    0x38, DO_E0_PREFIX_CODE,    	// [A6h] Back event
    0x30, DO_E0_PREFIX_CODE,    	// [A7h] Forward event
    0x28, DO_E0_PREFIX_CODE,    	// [A8h] Stop event
    0x20, DO_E0_PREFIX_CODE,    	// [A9h] Refresh event
    0x18, DO_E0_PREFIX_CODE,    	// [AAh] Favorites event
    0x2B, DO_E0_PREFIX_CODE,    	// [ABh] Caluator event
    0x40, DO_E0_PREFIX_CODE,    	// [ACh] My Computer event
    0x50, DO_E0_PREFIX_CODE,    	// [ADh] Media event
    0x00, DO_SIMPLE_CODE,       	// [AEh] Reserved
    0x00, DO_SIMPLE_CODE,      		// [AFh] Reserved

//-----------------------------------------------------------------
// Index from sskey2_A2_table
//-----------------------------------------------------------------
	0x6C, DO_SIMPLE_CODE,     		// Numpad 7 Home	[B0h]
  	0x75, DO_SIMPLE_CODE,      		// Numpad 8 UP  	[B1h]
 	0x7D, DO_SIMPLE_CODE,       	// Numpad 9 PGUp	[B2h]
  	0x7C, DO_SIMPLE_CODE,       	// Numpad *   		[B3h]
	0x6B, DO_SIMPLE_CODE,       	// Numpad 4 Left	[B4h]
 	0x73, DO_SIMPLE_CODE,       	// Numpad 5 		[B5h]
	0x74, DO_SIMPLE_CODE,       	// Numpad 6 Right	[B6h]
	0x7B, DO_SIMPLE_CODE,       	// Numpad -  		[B7h]
	0x69, DO_SIMPLE_CODE,       	// Numpad 1 End		[B8h]
 	0x72, DO_SIMPLE_CODE,       	// Numpad 2 Down	[B9h]
	0x7A, DO_SIMPLE_CODE,       	// Numpad 3 PGDn	[BAh]
	0x79, DO_SIMPLE_CODE,       	// Numpad + 		[BBh]
	0x70, DO_SIMPLE_CODE,       	// Numpad 0 Ins		[BCh]
	0x71, DO_SIMPLE_CODE,      	 	// Numpad . Del		[BDh]
	0x4A, DO_E0_PREFIX_CODE, 		// Numpad /      	[BEh]

	0x00, DO_SIMPLE_CODE,      		// Reserved 		[BFh]

    0x71, DO_E0_PREFIX_CODE,  		// Delete 		[C0h]
    0x84, DO_SIMPLE_CODE,    		// SysRq			[C1h]
    0x70, DO_E0_PREFIX_CODE,    	// Insert			[C2h]
    0x7C, DO_E0_PREFIX_CODE,  		// Print Screen	[C3h]
	0x41, DO_E0_PREFIX_CODE,     	// Euro Sign 		[C4h]
	0x49, DO_E0_PREFIX_CODE,    	// Dollar Sign 		[C5h]
	0x00, DO_SIMPLE_CODE,      		// Reserved 		[C6h]
	0x00, DO_SIMPLE_CODE,      		// Reserved 		[C7h]
	0x00, DO_SIMPLE_CODE,      		// Reserved 		[C8h]
	0x00, DO_SIMPLE_CODE,      		// Reserved 		[C9h]
	0x00, DO_SIMPLE_CODE,      		// Reserved 		[CAh]
	0x00, DO_SIMPLE_CODE,      		// Reserved 		[CBh]
	0x00, DO_SIMPLE_CODE,      		// Reserved 		[CCh]
	0x00, DO_SIMPLE_CODE,      		// Reserved 		[CDh]
	0x00, DO_SIMPLE_CODE,      		// Reserved 		[CEh]
	0x00, DO_SIMPLE_CODE,      		// Reserved 		[CFh]

// Function key F1 ~ F12
    0	, DO_COSTOMER_FUNCTION,		// Function key F1	[D0h]
    1	, DO_COSTOMER_FUNCTION,		// Function key F2	[D1h]
    2	, DO_COSTOMER_FUNCTION,		// Function key F3	[D2h]
    3	, DO_COSTOMER_FUNCTION,		// Function key F4	[D3h]
    4	, DO_COSTOMER_FUNCTION,		// Function key F5	[D4h]
    5	, DO_COSTOMER_FUNCTION,		// Function key F6	[D5h]
    6	, DO_COSTOMER_FUNCTION,		// Function key F7	[D6h]
    7	, DO_COSTOMER_FUNCTION,		// Function key F8	[D7h]
    8	, DO_COSTOMER_FUNCTION,		// Function key F9	[D8h]
    9	, DO_COSTOMER_FUNCTION,		// Function key F10	[D9h]
    10	, DO_COSTOMER_FUNCTION,		// Function key F11	[DAh]
    11	, DO_COSTOMER_FUNCTION,		// Function key F12	[DBh]
	12	, DO_COSTOMER_FUNCTION,   	// Function key Esc	[DCh]
	13	, DO_COSTOMER_FUNCTION,   	// Function key Up arrow	[DDh]
	14	, DO_COSTOMER_FUNCTION,   	// Function key Down arrow	[DEh]
	15	, DO_COSTOMER_FUNCTION,  	// Function key Left arrow	[DFh]
	16	, DO_COSTOMER_FUNCTION,  	// Function key Right arrow	[E0h]
	17	, DO_COSTOMER_FUNCTION,  	// Function key Right arrow	[E1h]
	18  , DO_COSTOMER_FUNCTION,     // Function key Fn Q	[E2h]
	19  , DO_COSTOMER_FUNCTION,     // Function key Fn H	[E3h]
	20  , DO_COSTOMER_FUNCTION,     // Function key Fn N	[E4h]
};

									// Reserved 0xE0 ~ 0xFF

//-----------------------------------------------------------------
// The function of checking boot hotkey Dino 1024
//-----------------------------------------------------------------
#define DebugkeyFn        	0x8E   	// Fn
#define DebugKeyEqu       	0x55  	// =
#define DebugKeyS			0x1B	// S
#define DebugKeyA			0x1C	// A
#define DebugKeyR			0x2D	// R
#define DebugKeyN			0x31	// N
#define DebugKeyI			0x43	// I
#define DebugKeyO			0x44	// O
#define DebugKeyP			0x4D	// P//change "Fn+D"to"Fn+D+P".
#define DebugKeyD			0x23	// D  
#define DebugKeyV			0x2A	// V  ADD //ANGELAS032: add
#define Crisiskey05			0xE1	// F2    //ANGELAG001: add
#define Crisiskey06			0xEB	// F12  //ANGELAG001: add

//ANGELAG001:S+ add CrisisHotkey05 function.
void CrisisHotkey05(BYTE event)
{
	/*if((IS_MASK_CLEAR(ACPI_HOTKEY, HotkeyDisable))&&(FnStatus == 1))//"Fn+F2"
	{
		SET_MASK(F2_Pressed, F2Flag); 		CLEAR_MASK(F2_Pressed, F12Flag); 

	}
	else if((IS_MASK_SET(ACPI_HOTKEY, HotkeyDisable))&&(FnStatus == 0))//"F2"
	{
		SET_MASK(F2_Pressed, F2Flag); 
		CLEAR_MASK(F2_Pressed, F12Flag); 
	}*///72JERRY059:-Optimize F2/F12 flag to BIOS.
	//72JERRY059:S+Optimize F2/F12 flag to BIOS.
	// ( IS_MASK_CLEAR(SYS_MISC1, ACPI_OS) )
		{
		SET_MASK(F2_Pressed, F2Flag); 
		CLEAR_MASK(F2_Pressed, F12Flag); 
		}
		//72JERRY059:e+Optimize F2/F12 flag to BIOS.
}

void CrisisHotkey06(BYTE event)
{
	/*if((IS_MASK_CLEAR(ACPI_HOTKEY, HotkeyDisable))&&(FnStatus == 1))//"Fn+F12"
	{
		SET_MASK(F2_Pressed, F12Flag); 
		CLEAR_MASK(F2_Pressed, F2Flag); 
	}
	else if((IS_MASK_SET(ACPI_HOTKEY, HotkeyDisable))&&(FnStatus == 0))//"F12"
	{
		SET_MASK(F2_Pressed, F12Flag); 
		CLEAR_MASK(F2_Pressed, F2Flag); 
	}*///72JERRY059:-Optimize F2/F12 flag to BIOS.
	//72JERRY059:s+Optimize F2/F12 flag to BIOS.
	// ( IS_MASK_CLEAR(SYS_MISC1, ACPI_OS) )
		{
		SET_MASK(F2_Pressed, F12Flag); 
		CLEAR_MASK(F2_Pressed, F2Flag); 
		}

	//72JERRY059:e+Optimize F2/F12 flag to BIOS.
}
//ANGELAG001:E+.

void FnHotkey01(BYTE event)
{
	if(event == BREAK_EVENT)
	{
		DebugCombineKey=0x00;
	}
	else if(event == MAKE_EVENT)
	{
		DebugCombineKey=1;
	}
}

void FnHotkey02(BYTE event)
{
	if(event == MAKE_EVENT)
	{
		if (DebugCombineKey==1)
			DebugCombineKey=2;
		else
			DebugCombineKey=0;
	}
}

void FnHotkey03(BYTE event)
{
	if(event == MAKE_EVENT)
	{
		if (DebugCombineKey==2)
			DebugCombineKey=3;
		else
			DebugCombineKey=0;
	}
}

void FnHotkey04(BYTE event)
{
    if(IS_MASK_CLEAR(LENOVOPMFW_Temp,EEPROM_Token))
    {
	if(event == MAKE_EVENT)
	{
		if (DebugCombineKey==3)
		{
			DebugCombineKey=4;
			eEEPROMBank= 0x07;
			eEEPROMAddrsss = 0xF1;				// Save to B7 F1 (0x0001B7F1)
			eEEPROMData	= BRAM3F;
			Update_EEPROMMark();
			eUpdateEEPROMCnt = 2;
			cSPIEEPROM.fbit.cSPI_ROMCopyStart=1;
		}
		else
			DebugCombineKey=0;
	}
    }
}

const sBootHotKeyStruct code asBootHotKeyStruct[]=
{
	{ DebugkeyFn, 	FnHotkey01	},
	{ DebugKeyEqu, 	FnHotkey02	},
	{ DebugKeyS,    FnHotkey03	},
	{ DebugKeyA,	FnHotkey04	},
};

void CheckBootHotKey(matrix, event)
{
	BYTE index;
	for(index=0x00; index<(sizeof(asBootHotKeyStruct)/sizeof(sBootHotKeyStruct)); index++)
	{
		if(matrix==asBootHotKeyStruct[index].matrix)
		{
			(asBootHotKeyStruct[index].pfunction)(event);
			return;
		}
	}
	DebugCombineKey=0x00;
}

//change "Fn+D"to"Fn+D+P".
void FnHotkeyPort80Log01(BYTE event)
{
	/*if(event == BREAK_EVENT)
	{
		CombineKeyRN=0x00;
	}
	else if(event == MAKE_EVENT)
	{
		CombineKeyRN=1;
	}*/

	if((event == MAKE_EVENT)&&(FnStatus == 1))
	{
		CombineKeyRN=1;
	}
	//ANGELAS091: s+Optimize ANGELAS032 ( press "fn+s" then release "S", flag can't be clear.)
	else if(event == BREAK_EVENT)
	{
		CombineKeyRN=0;
	}
	//ANGELAS091:e+ Optimize ANGELAS032 ( press "fn+s" then release "S", flag can't be clear.)
}
// change "Fn+D"to"Fn+D+P".

void FnHotkeyPort80Log02(BYTE event)
{
	if(event == MAKE_EVENT)
	{
		if (CombineKeyRN==1)
			CombineKeyRN=2;
		else
			CombineKeyRN=0;
	}
}

void FnHotkeyPort80Log03(BYTE event)
{
	/*if(event == MAKE_EVENT)
	{
		if (CombineKeyRN==2)
			CombineKeyRN=3;
		else
			CombineKeyRN=0;
	}*/
	// 0ptimize"Fn+R"crisis recover bios follow lenovo spec(press "Fn+R" then press power button  that will enter crisis recover bios on DC or AC mode).
	if(IS_MASK_CLEAR(SYS_MISC1,BATCRISIS_FLAG))
	{
		if(((event == MAKE_EVENT)||(event==REPEAT_EVENT))&&(FnStatus == 1))
		{
			SET_MASK(SYS_MISC1,Crisis_On);
		}
		else 
		{
			CLEAR_MASK(SYS_MISC1,Crisis_On);
		}
	}
}

void FnHotkeyPort80Log04(BYTE event)
{
	if(event == MAKE_EVENT)
	{
		if (CombineKeyRN==3)
		{
			CombineKeyRN=4;
			SET_MASK(CombineKeyStatus, b0FnRN);
		}
		else
			CombineKeyRN=0;
	}
}
void FnHotkeyPort80Log05(BYTE event)
{
	if((event == MAKE_EVENT)&&(FnStatus == 1)&&(CombineKeyRN==1))//change "Fn+D"to"Fn+D+P".
	{
		SET_MASK(P80CMOSSts,P80CMOSDis);
		SET_MASK(ACPI_HOTKEY, b6Cmd_NoShut);//when battery mode press FN+D£¬do not cut power¡£
	}
}
const sBootHotKeyStruct code asCheckRNStruct[]=
{
	{ DebugKeyD, 		FnHotkeyPort80Log01	},//change "Fn+D"to"Fn+D+P".
	//{ DebugKeyEqu, 	FnHotkeyPort80Log02	},
	{ DebugKeyR,	    FnHotkeyPort80Log03	},
	//{ DebugKeyN,	      	FnHotkeyPort80Log04	},
	//{ DebugKeyP,	    FnHotkeyPort80Log05	},//change "Fn+D"to"Fn+D+P".  //MEILING026:remove.
	{ DebugKeyO,	    FnHotkeyPort80Log05	}, //MEILING026:add.
	{ Crisiskey05,	    CrisisHotkey05	}, //ANGELAG001: add
	{ Crisiskey06,	    CrisisHotkey06	}, //ANGELAG001:add.
};
void CheckRNHotKey(matrix, event)
{
	BYTE index;
	for(index=0x00; index<(sizeof(asCheckRNStruct)/sizeof(sBootHotKeyStruct)); index++)
	{
		if(matrix==asCheckRNStruct[index].matrix)
		{
			(asCheckRNStruct[index].pfunction)(event);
			return;
		}
	}
	CombineKeyRN=0;
}

void FnHotkeyGPIO01(BYTE event)
{
	if(event == BREAK_EVENT)
	{
		CombineKeyIO=0x00;
	}
	else if(event == MAKE_EVENT)
	{
		CombineKeyIO=1;
	}

}

void FnHotkeyGPIO02(BYTE event)
{
	if(event == MAKE_EVENT)
	{
		if (CombineKeyIO==1)
			CombineKeyIO=2;
		else
			CombineKeyIO=0;
	}
}

void FnHotkeyGPIO03(BYTE event)
{
	if(event == MAKE_EVENT)
	{
		if (CombineKeyIO==2)
			CombineKeyIO=3;
		else
			CombineKeyIO=0;
	}

}

void FnHotkeyGPIO04(BYTE event)
{
	if(event == MAKE_EVENT)
	{
		if (CombineKeyIO==3)
		{
			GPIO_STATUS = 0;
			if ( Read_RSMRST() )				// bit0:GPIO_RSMRST
			{ SET_MASK(GPIO_STATUS,GPIO_RSMRST); }
			if ( Read_PM_PWRBTN() )				// bit1:GPIO_PBTN_OUT
			{ SET_MASK(GPIO_STATUS,GPIO_PBTN_OUT); }
			if ( Read_VDDQ_PGOOD() )				// bit2:GPIO_VDDQ_PGOOD
			{ SET_MASK(GPIO_STATUS,GPIO_VDDQ_PGOOD); }
			if ( Read_SLPS3() )					// bit3:GPIO_PM_SLP_S3
			{ SET_MASK(GPIO_STATUS,GPIO_PM_SLP_S3); }
			if ( Read_SLPS4() )					// bit4:GPIO_PM_SLP_S4
			{ SET_MASK(GPIO_STATUS,GPIO_PM_SLP_S4); }
												// bit5:GPIO_PM_SLP_S5
			if ( Read_VR_ON() )					// bit6:GPIO_VR_ON
			{ SET_MASK(GPIO_STATUS,GPIO_VR_ON); }
			if ( Read_PCH_PWROK() )				// bit7:GPIO_PCH_PWROK
			{ SET_MASK(GPIO_STATUS,GPIO_PCH_PWROK); }

			CombineKeyIO=4;
            if(IS_MASK_CLEAR(LENOVOPMFW_Temp,EEPROM_Token))
            {

			eEEPROMBank= 0x07;
			eEEPROMAddrsss = 0xEF;				// Save to B7 EF (0x0001B7EF)
			eEEPROMData	= GPIO_STATUS;
			Update_EEPROMMark();
 			eUpdateEEPROMCnt = 2;
			cSPIEEPROM.fbit.cSPI_ROMCopyStart=1;
            }

		}
		else
		{ CombineKeyIO=0; }
	}
}

const sBootHotKeyStruct code asCheckIOStruct[]=
{
	{ DebugkeyFn, 	FnHotkeyGPIO01	},
	{ DebugKeyEqu, 	FnHotkeyGPIO02	},
	{ DebugKeyI,	FnHotkeyGPIO03	},
	{ DebugKeyO,   	FnHotkeyGPIO04	},
};
void CheckIOHotKey(matrix, event)
{
	BYTE index;
	for(index=0x00; index<(sizeof(asCheckIOStruct)/sizeof(sBootHotKeyStruct)); index++)
	{
		if(matrix==asCheckIOStruct[index].matrix)
		{
			(asCheckIOStruct[index].pfunction)(event);
			return;
		}
	}
	CombineKeyIO=0;
}

//ANGELAS032: s+ add
#if shipmodesupport 
void FnHotkeyBattShip01(BYTE event)
{
	if((event == MAKE_EVENT)&&(FnStatus == 1))
	{
CombineKeyShip |= 0x01;
			if(CombineKeyShip&0x03){
				ShipModeEn=0xA5; 
				if(SystemIsDSX){
			    SysPowState = SYSTEM_S5;
               RamDebug(0xFD);
					}
				  CombineKeyShip = 0x00;
			}
	}else if((event == BREAK_EVENT)||(FnStatus == 0))
	       CombineKeyShip &= ~0x01;
}

void FnHotkeyBattShip02(BYTE event)
{
	if((event == MAKE_EVENT)&&(FnStatus == 1)) 
	{
	       CombineKeyShip |= 0x02;
			if(CombineKeyShip&0x03){
			   CombineKeyShip = 0x00;

	       if(SystemIsDSX){
		   	RamDebug(0xFD);
			ShipModeEn=0xA5; 	        
			SysPowState = SYSTEM_S5;
	       	}
         }
}

else if((event == BREAK_EVENT)||(FnStatus == 0))
		   CombineKeyShip &=~0x02;
}
const sBootHotKeyStruct code asBattShipHotKeyStruct[]=
{
	{ DebugKeyS,    	FnHotkeyBattShip01	},
	{ DebugKeyV,	    FnHotkeyBattShip02	},  //Fn+S+V
};
void CheckBattShipHotKey(matrix, event)
{
	BYTE index;
	for(index=0x00; index<(sizeof(asBattShipHotKeyStruct)/sizeof(sBootHotKeyStruct)); index++)
	{
		if(matrix==asBattShipHotKeyStruct[index].matrix)
		{
			(asBattShipHotKeyStruct[index].pfunction)(event);
			return;
		}
	}
	CombineKeyShip=0x00;
	CombineKeyShip1=0x00; //ANGELAS056:add
}
#endif
//ANGELAS032:e+ add
//----------------------------------------------------------------------------
// table_entry : value of keyboard matrix table. for example Rc_ROM_Tables[]
// event : key MAKE_EVENT or BREAK_EVENT or REPEAT_EVENT
//----------------------------------------------------------------------------
void OEM_Hook_Send_Key(BYTE table_entry, BYTE event)
{
	//if(SystemIsS0)
	//{		
		CheckBootHotKey(table_entry,event);
		CheckRNHotKey(table_entry,event);
		CheckIOHotKey(table_entry,event);
		//ANGELAS032: s+ add
		#if shipmodesupport
		if((!Read_AC_IN())&&(SystemIsDSX))
		{
			CheckBattShipHotKey(table_entry,event);
		}
		#endif
		if(SystemIsS4||SystemIsS5||SystemIsDSX)//HEGANGS021:one key wake
		{
			if(event==BREAK_EVENT)
			S4S5_KeyWake(1);
		}
		//ANGELAS032: e+ add
	//}	
}

//----------------------------------------------------------------------------
// Return : 0xFF --> Send_Key function will be break; (no any scan code to host)
//----------------------------------------------------------------------------
BYTE OEM_Hook_Skip_Send_Key(void)
{
    return(0x00);
}

//-----------------------------------------------------------------------------
// Keyboard hook function (KSO0 - KSO15)
// KSIValus : keyboard KSI
// KSOValus : keyboard KSO
//-----------------------------------------------------------------------------
void Hook_keyboard(BYTE KSIValus, BYTE KSOValus)
{

}

//-----------------------------------------------------------------------------
// Keyboard hook function (KSO16 - KSO17 and GPIO KSO)
// KSIValus : keyboard KSI
// KSOValus : keyboard KSO
//-----------------------------------------------------------------------------
void Et_Hook_keyboard(BYTE KSIValus, BYTE KSOValus)
{

}

/* ---------------------------------------------------------------------------
 * FUNCTION: Hook_Setup_Scanner_Pntr - to define the possible scanner tables
 *
 *     0   Rc_RAM_Table
 *     1   ROM Table 1
 *     ...
 * ------------------------------------------------------------------------ */
void Hook_Setup_Scanner_Pntr(void)
{
	#if SUPPORT_KB_RAM_TABLE
	Scanner_RAM_Pntr = &KB_RAM_Table;
	Extendkey_RAM_Pntr = &ExKB_RAM_Table;
	LoadKBMatrixToRAM();
	#else
	Scanner_Table_Pntr = &Rc_ROM_Tables;
	Extendkey_Table_Pntr = &EtKey_ROM_Tables;
	#endif
}

/* ----------------------------------------------------------------------------
 * FUNCTION: LoadKBMatrixToRAM
 * ------------------------------------------------------------------------- */
void LoadKBMatrixToRAM(void)
{
	//if( uMBID & 0x40 )  //15'' for Zx10 //martin0219 check
    //if( uMBID & 0x01 )   //15'' for Zx10 // check G48
    //if( ( uMBID & 0x03 == 0x01 ) ||( uMBID & 0x03 == 0x02 ) )	//ANGELAS008: Modidy board ID for check 15" and 17"
    if( uMBID & 0x01 )   //15''  //ANGELAS009: Remove ANGELAS008
    {
		if ( (cKBStatus & 0x03) != 0x03 )
		{
	    	for( ITempB06=0; ITempB06<0x80; ITempB06++ )	//15" US,UK and JP.
	        {
	        	Tmp_XPntr = ( KB_RAM_Table + ITempB06 );
	            *Tmp_XPntr = Rc_ROM_Tables[ITempB06];
	        }
		}
		else
		{
		    for( ITempB06=0; ITempB06<0x80; ITempB06++ )	//15" Brazil.
	        {
	        	Tmp_XPntr = ( KB_RAM_Table + ITempB06 );
	            *Tmp_XPntr = Rc_ROM_Tables_15BR[ITempB06];
	        }
		}

        for( ITempB06=0; ITempB06<0x18; ITempB06++ )
        {
            Tmp_XPntr = ( ExKB_RAM_Table + ITempB06 );
            *Tmp_XPntr = EtKey_ROM_Tables[ITempB06];
        }
    }
    else     //14'' for Zx10 // check
    {
		if ( (cKBStatus & 0x03) != 0x03 )
		{
	    	for( ITempB06=0; ITempB06<0x80; ITempB06++ )	//14" US,UK and JP.
	        {
	        	Tmp_XPntr = ( KB_RAM_Table + ITempB06 );
	            *Tmp_XPntr = Rc_ROM_Tables_14[ITempB06];
	        }
		}
		else
		{
		    for( ITempB06=0; ITempB06<0x80; ITempB06++ )	//14" Brazil.
	        {
	            Tmp_XPntr = ( KB_RAM_Table + ITempB06 );
	            *Tmp_XPntr = Rc_ROM_Tables_14BR[ITempB06];
	        }
		}

        for( ITempB06=0; ITempB06<0x18; ITempB06++ )
        {
            Tmp_XPntr = ( ExKB_RAM_Table + ITempB06 );
            *Tmp_XPntr = EtKey_ROM_Tables_1[ITempB06];
        }
    }
}

//-----------------------------------------------------------------------------
// The function of setting GPIO scan pin to high
//-----------------------------------------------------------------------------
void Hook_SetGPIOScanPinH(void)
{

}

//-----------------------------------------------------------------------------
// The function of setting GPIO scan pin to low
//-----------------------------------------------------------------------------
void Hook_SetGPIOScanPinL(void)
{

}

//-----------------------------------------------------------------------------
// The function of setting GPIO scan pin output
//-----------------------------------------------------------------------------
void Hook_SetGPIOScanPinCtrl(void)
{

}

//-----------------------------------------------------------------------------
// Fn key make
//-----------------------------------------------------------------------------
void Hook_Fn_Key_Make(void)
{
	if ( IS_MASK_SET(cCmd, b6TestBtnEn) )
	{
   		Buffer_Key(0xE0);                   //
       	Buffer_Key(0x16);              		// page down make
   		F_Service_SEND = 1;
	}
	FnStatus = 1;							// for factory

}

//-----------------------------------------------------------------------------
// Fn key break
//-----------------------------------------------------------------------------
void Hook_Fn_Key_Break(void)
{
/*	if (SystemIsS3)
	{
		if((Read_LID_SW_IN())&&(Read_LID_PAD()))
		{
			PWSeqStep = 1;
			PowSeqDelay = 1;
			RamDebug(0x30);         //T045A
			SysPowState=SYSTEM_S3_S0;
		}
	}*/
//MEILING006: S+Fixed The display mode auto change to the nearly mode when change the mode via lenovo hotkey.
	if ( IS_MASK_SET(SysStatus, FnF3WinKeyDn) ) //FnF3PKeyUp
	{
		if(IS_MASK_CLEAR(SysStatus, FnF3PKeyUp))
		{
			simple_code(0x4D,BREAK_EVENT);	// P Key.
		}
		e0_prefix_code(0x1F,BREAK_EVENT);	// Win Key.
	
		SysStatus &= 0xF3;	// Clear Win+P Key.
	
		F_Service_SEND = 1;
		RamDebug(0x04);
	}
	//MEILING006: E+Fixed The display mode auto change to the nearly mode when change the mode via lenovo hotkey.

	if ( IS_MASK_SET(cCmd, b6TestBtnEn) )
	{
		Buffer_Key(0xE0);					//
		Buffer_Key(0xF0);					//
		Buffer_Key(0x16);					// page down make
		F_Service_SEND = 1;
	}
	FnStatus = 0;							// for factory
}

//-----------------------------------------------------------------------------
// Return : 0xFF --> sskey2_A2_table contains bit6
//-----------------------------------------------------------------------------
BYTE Hook_calc_index_comb_BIT6(void)
{
    return(0x00);
}

//-----------------------------------------------------------------------------
// Return : 0xFF --> sskey2_A2_table contains bit7
//-----------------------------------------------------------------------------
BYTE Hook_calc_index_comb_BIT7(void)
{
    return(0x00);
}
