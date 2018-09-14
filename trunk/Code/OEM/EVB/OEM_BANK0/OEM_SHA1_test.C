#include "CORE_INCLUDE.H"
#include "OEM_INCLUDE.H"

#if SHA1_Support

void ITE_SHA1_01(void)
{
		// initial Hash value
		H0 = 0x67452301;
		H1 = 0xEFCDAB89;
		H2 = 0x98BADCFE;
 		H3 = 0x10325476;
		H4 = 0xC3D2E1F0;

		// initial Challenge
		/* from BJ*/
		SHA1_W[0] = 0x23BA805C;
		SHA1_W[1] = 0x4470FEE1;
		SHA1_W[2] = 0x061D9366;
		SHA1_W[3] = 0x27C9A85B;
		SHA1_W[9] = 0x80000000;

		for(ITempB03=10;ITempB03<15;ITempB03++) {
			SHA1_W[ITempB03]=0x00000000;
		}
		//SHA1_W[10] = 0x00000000;
		//SHA1_W[11] = 0x00000000;
		//SHA1_W[12] = 0x00000000;
		//SHA1_W[13] = 0x00000000;
		//SHA1_W[14] = 0x00000000;
		SHA1_W[15] = 0x00000120;

		if( ITempB02==0 ) {
			SHA1_W[4] = 0xC82CA3CA;
			SHA1_W[5] = 0x10DEC726;
			SHA1_W[6] = 0x8E070A7C;
			SHA1_W[7] = 0xF0D1FE82;
			SHA1_W[8] = 0x20AAD3B8;
		} else {
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

void ITE_SHA1_02(void)
{
		ITempB01=16;
		while(ITempB01<80) {					//W[t] = SHA1CircularShift(1,W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16]);
			ITempW01 = (&SHA1_W);				// ITempW01 point to Wt(16)
			ITempW01 += (ITempB01<<2);

			AddrZ_High = (ITempW01 >> 8);		// Z point to Wt(16)
			AddrZ_Low  = (ITempW01 & 0xFF);
			AddrX_High = ((ITempW01-64) >> 8);	// X point to Wt head
			AddrX_Low  = ((ITempW01-64) & 0xFF);

			Cal_CTRL2 = W_T_BIT;
			ITempB01++;
		}

		/* Initial ize the five working variable, a, b, c, d, and e, with the (i-1) hash value */
		/* Copy Hash value to A,B,C,D,E */
		//A = H0;
		N_Bit_Shift = 0;
		ITempW01 = &H0;
		AddrX_High = (ITempW01 >> 8);
		AddrX_Low  = (ITempW01 & 0xFF);
		ITempW01 = &SHA1_a;
		AddrZ_High = (ITempW01 >> 8);
		AddrZ_Low  = (ITempW01 & 0xFF);
		Cal_CTRL = Rotate_Shift_right_BIT;

		//B = H1;
		AddrX_Low+=4;
		AddrZ_Low+=4;
		Cal_CTRL = Rotate_Shift_right_BIT;

		//C = H2;
		AddrX_Low+=4;
		AddrZ_Low+=4;
		Cal_CTRL = Rotate_Shift_right_BIT;

		//D = H3;
		AddrX_Low+=4;
		AddrZ_Low+=4;
		Cal_CTRL = Rotate_Shift_right_BIT;

		//E = H4;
		AddrX_Low+=4;
		AddrZ_Low+=4;
		Cal_CTRL = Rotate_Shift_right_BIT;

		/*  Original SHA-1 code as below */
		//	for(t = 0; t < 80; t++) {
		//		if(t>=0 && t<20)
		//			temp = SHA1CircularShift(5,A) + ((B & C) | ((~B) & D)) + E + W[t] + K[0];
		//		else if(t>=20 && t<40)
		//			temp = SHA1CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[1];
		//		else if(t>=40 && t<60)
		//			temp = SHA1CircularShift(5,A) + ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
		//		else if(t>=60 && t<80)
		//			temp = SHA1CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[3];
		//
		//		E = D;
		//		D = C;
		//		C = SHA1CircularShift(30,B);
		//		B = A;
		//		A = temp;
		//	}

		/*  Because this function need chip support */
		/*  So, A B C D E must continuous in memory */

		ITempW01 = &SHA1_a;
		AddrX_High = (ITempW01 >> 8);
		AddrX_Low  = (ITempW01 & 0xFF);
		ITempW01 = &WS_temp;
		AddrZ_High = (ITempW01 >> 8);
		AddrZ_Low  = (ITempW01 & 0xFF);

		K0 = 0x5A827999;
		ITempW01 = &K0;
		AddrY_High = (ITempW01 >> 8);
		AddrY_Low  = (ITempW01 & 0xFF);
		for(ITempB01 = 0; ITempB01 < 20; ITempB01++) {
			ITempW01 = (&SHA1_W);	// ITempW01 point to Wt(0~19)
			ITempW01 += (ITempB01<<2);
			AddrW_High = (ITempW01 >> 8);
			AddrW_Low  = (ITempW01 & 0xFF);
			Cal_CTRL2 = Ch_BCD_BIT;
		}

		K0 = 0x6ED9EBA1;
	//	AddrY_High = (ITempW01 >> 8);
	//	AddrY_Low  = (ITempW01 & 0xFF);
		for(ITempB01 = 20; ITempB01 < 40; ITempB01++) {
			ITempW01 = (&SHA1_W);	// ITempW01 point to Wt(20~39)
			ITempW01 += (ITempB01<<2);
			AddrW_High = (ITempW01 >> 8);
			AddrW_Low  = (ITempW01 & 0xFF);
			Cal_CTRL2 = PR_BCD_BIT;
		}

		K0 = 0x8F1BBCDC;
	//	AddrY_High = (ITempW01 >> 8);
	//	AddrY_Low  = (ITempW01 & 0xFF);
		for(ITempB01 = 40; ITempB01 < 60; ITempB01++) {
			ITempW01 = (&SHA1_W);	// ITempW01 point to Wt(40~59)
			ITempW01 += (ITempB01<<2);
			AddrW_High = (ITempW01 >> 8);
			AddrW_Low  = (ITempW01 & 0xFF);
			Cal_CTRL2 = Maj_BCD_BIT;
		}

		K0 = 0xCA62C1D6;
	//	AddrY_High = (ITempW01 >> 8);
	//	AddrY_Low  = (ITempW01 & 0xFF);
		for(ITempB01 = 60; ITempB01 < 80; ITempB01++) {
			ITempW01 = (&SHA1_W);	// ITempW01 point to Wt(60~79)
			ITempW01 += (ITempB01<<2);
			AddrW_High = (ITempW01 >> 8);
			AddrW_Low  = (ITempW01 & 0xFF);
			Cal_CTRL2 = PR_BCD_BIT;
		}
}

void ITE_SHA1_03(void)
{
		/* Original SHA-1 code as below */
		// Hn0 = H0 + A;
		// Hn1 = H1 + B;
		// Hn2 = H2 + C;
		// Hn3 = H3 + D;
		// Hn4 = H4 + E;

		/* SHA1 compute result store at buffer(Hn0) */
		// Hn0 = H0 + A;
		ITempW01 = &H0;
		AddrX_High = (ITempW01 >> 8);
		AddrX_Low  = (ITempW01 & 0xFF);

		ITempW01 = &Hn0;
		AddrZ_High = (ITempW01 >> 8);
		AddrZ_Low  = (ITempW01 & 0xFF);

		ITempW01 = &SHA1_a;
		AddrY_High = (ITempW01 >> 8);
		AddrY_Low  = (ITempW01 & 0xFF);
		Cal_CTRL =  ADD_BIT;

		// Hn1 = H1 + B;
		AddrX_Low+=4;
		AddrY_Low+=4;
		AddrZ_Low+=4;
		Cal_CTRL =  ADD_BIT;

		// Hn2 = H2 + C;
		AddrX_Low+=4;
		AddrY_Low+=4;
		AddrZ_Low+=4;
		Cal_CTRL =  ADD_BIT;

		// Hn3 = H3 + D;
		AddrX_Low+=4;
		AddrY_Low+=4;
		AddrZ_Low+=4;
		Cal_CTRL =  ADD_BIT;

		// Hn4 = H4 + E;
		AddrX_Low+=4;
		AddrY_Low+=4;
		AddrZ_Low+=4;
		Cal_CTRL =  ADD_BIT;
}

void ITE_SHA1(void)
{
	// enable EC SHA1 function
	EC201Ch |= 0x10;

	for(ITempB02=0;ITempB02<2;ITempB02++)
	{
		CacheDma(3,ITE_SHA1_01);
		ITE_SHA1_01();
		CacheDma(3,ITE_SHA1_02);
		ITE_SHA1_02();
		CacheDma(3,ITE_SHA1_03);
		ITE_SHA1_03();
	}
}

//===================================================================
// Service SHA1 algorithm
//===================================================================
void OEM_BAT_Authentication(void)
{
	if(Service_Auth_Step) {
		CAPLED_ON();
		ITE_SHA1();
		CAPLED_OFF();
		Service_Auth_Step=0;
	}
}
#endif