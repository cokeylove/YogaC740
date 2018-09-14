/*-----------------------------------------------------------------------------
 * TITLE: OEM_PORT686C.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//ANGELAS002:S+Support 686C flash EC in windows or Shell
void PM2_6CCmd_DC(void)
{
    ITE_Flash_Utility_WINF();  //BROOKEW008: support 686C flash EC
}
//ANGELAS002:E+Support 686C flash EC in windows or Shell

