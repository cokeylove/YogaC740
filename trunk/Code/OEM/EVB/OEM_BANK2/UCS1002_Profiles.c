/**************************************************************************************
 *  SMSC CONFIDENTIAL                                                                 *
 *  Copyright (c) 2008 Standard Microsystems Corporation. All rights reserved.        *
 *  This program contains proprietary and confidential information.                   *
 *                                                                                    *
 * THIS SOFTWARE PROVIDED BY STANDARD MICROSYSTEMS CORPORATION`("SMSC")IS SAMPLE CODE *
 * INTENDED FOR EVALUATION PURPOSES ONLY.  IT IS NOT INTENDED FOR COMMERCIAL USE.     *
 * THIS SOFTWARE IS PROVIDED BY SMSC "AS IS" AND ANY EXPRESS OR IMPLIED               *
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF               *
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND             *
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL SMSC BE LIABLE FOR ANY DIRECT,          *
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES                 *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;       *
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND        *
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT         *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF           *
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                  *
 **************************************************************************************

	Program Name: UCS1002 Charger Sample Program Version 1.3.
	Copyright (C) 2012 Standard Microsystems Corporation
	All rights reverved.

	This is sample program for UCS1002 to work as a dedicated charger in DCE mode.

	Reference:
		SMSC UCS1002 DS Rev. 1.1 (11-21-11).pdf

===============================================================================
Revision History:

Date
Version   	who  Comment
----------  --- --------------------------------------------------------------
2012/03/04	jc	.Initial version.
V1.0
				Size information reported by C51
				MODULE INFORMATION:   STATIC OVERLAYABLE
				CODE SIZE        =    826    ----
				CONSTANT SIZE    =     69    ----
			   	XDATA SIZE       =      8    ----
			    PDATA SIZE       =   ----       3
				DATA SIZE        =   ----    ----
				END OF MODULE INFORMATION.

2012/03/15	jc	.Fine tune timing.
V1.1

2012/03/23  jc 	.Fine tune current parameters.
V1.2
				Size information reported by C51
				MODULE INFORMATION:   STATIC OVERLAYABLE
				CODE SIZE        =    889    ----
				CONSTANT SIZE    =     71    ----
				XDATA SIZE       =     10    ----
				PDATA SIZE       =   ----       3
				DATA SIZE        =   ----    ----
				IDATA SIZE       =   ----    ----
				BIT SIZE         =   ----    ----
				END OF MODULE INFORMATION.

**************************************************************************************/
const	unsigned char	code 	bLProfile[] =
{
	0x03,	0x05,
	0x80, 	0x46, 0x48, 0x4A, 0x4C,
	0x0d, 	0x0A, 0x00, 0x0A, 0x00,	// Profile 2
	0x05, 	0x06, 0x04, 0x29, 0x02,	// Profile 1
	0x0d,	0x10, 0x05, 0x93, 0x00,	// Profile 5
	0x05, 	0x26, 0x02, 0x29, 0x02,
	0x05,	0x16, 0x05, 0x19, 0x05,
	//0x0d,	0x10, 0x05, 0x93, 0x00,	// Profile 5
	0x0d,	0x26, 0x02, 0x09, 0x04,
	0x0d,	0x10, 0x05, 0x46, 0x00,
};


