/*----------------------------------------------------------------------------
 * Filename: OEM_Ver.C  For Chipset: ITE.IT85XX
 *
 * Function: Project version
 *
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//----------------------------------------------------------------------------
// Project Information
//----------------------------------------------------------------------------
const unsigned char code ProjectInfoI[] =
{
	ProjectID0L,
	ProjectID0H,
	REV0_BYTE0,
	REV0_BYTE1,
	REV0_BYTE2,
	REV0_BYTE3,
	REV0_BYTE4,

	CHIP_TYPE0,
	CUSTOMER_ID0,
	0x00,
	"QIQY5",	// formodule name
};

const unsigned char code ProjectInfoII[] =
{
	ProjectID0L,
	ProjectID0H,
	REV0_BYTE0,
	REV0_BYTE1,
	REV0_BYTE2,
	REV0_BYTE3,
	REV0_BYTE4,

	CHIP_TYPE0,
	CUSTOMER_ID0,
	0x00,
	"QIQY6",	// formodule name
};
