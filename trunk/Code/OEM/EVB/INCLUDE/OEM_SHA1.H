/************************************************
	sha1.h - sha-1 authentication algorithmic head file

	Copyright (c) Lenovo Corporation 2011.
	All rights reserved.

	Revision : 0.1
	Release Date : Aug 30, 2011
	Author : Eddie Ma
	Approval :
	Description :
		1. Initial version.
************************************************/

// Varible definition
//

//unsigned long K[80] _at_  0x800;
//unsigned long Word[80] _at_ 0xA00;


#ifdef SHA1_C
#undef SHA1_C

unsigned long W_SHA1_Mod[16];
unsigned long H_SHA1_Mod[5];


/*--------------------------------------------------------
Name: sha1_auth
Function: SHA1 Authenication arithmetic and private key.
Input: The pointer of challenge buffer -> unsigned int challenge[5]
      For example:
      If the challenge is:  00112233445566778899AABBCCCCDDDDEEEEFFFF
      The challenge buffer order is:  Challenge[0] = 0x00112233
				         											Challenge[1] = 0x44556677
			        	 										  Challenge[2] = 0x8899AABB
				          									  Challenge[3] = 0xCCCCDDDD
 				         											Challenge[4] = 0xEEEEFFFF

Return: The pointer of response buffer -> unsinged int response[5]
     For example:
     If the response is: 00112233445566778899AABBCCCCDDDDEEEEFFFF
     The response buffer order is :  Response[0] = 0x00112233
				           									 Response [1] = 0x44556677
			        	   									 Response [2] = 0x8899AABB
				            								 Response [3] = 0xCCCCDDDD
 				           									 Response [4] = 0xEEEEFFFF

---------------------------------------------------------*/
unsigned long xdata *sha1_auth(unsigned long xdata *challengerbuffer);


#else

extern unsigned long xdata *sha1_auth(unsigned long xdata *challengerbuffer);

#endif /* ifndef SHA1_C */
