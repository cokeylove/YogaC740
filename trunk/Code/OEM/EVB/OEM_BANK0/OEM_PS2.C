/*-----------------------------------------------------------------------------
 * TITLE: OEM_PS2.C
 *
 * Author : Dino
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

// ----------------------------------------------------------------------------
// To define "Hook_ECACKMouseDriver" is necessary
// If define "Hook_ECACKMouseDriver" EC firmware need Respond mouse driver 
//  via Hook_ECRespondtoMouseDriver function
// ----------------------------------------------------------------------------
void Hook_ECRespondtoMouseDriver(BYTE mscmd)
{
    
}

// ----------------------------------------------------------------------------
// Hook kernel service_ps2 function
// ----------------------------------------------------------------------------
void Hook_service_ps2(BYTE ps2_channel, BYTE ps2_data)
{

}

// ----------------------------------------------------------------------------
// Disable port 0 ( send 0xF5 command to device)
// ----------------------------------------------------------------------------
void Hook_DisablePS2Port_0(void)
{
    DisablePS2Port_0();
}

// ----------------------------------------------------------------------------
// Disable port 1 ( send 0xF5 command to device)
// ----------------------------------------------------------------------------
void Hook_DisablePS2Port_1(void)
{
    DisablePS2Port_1();
}

// ----------------------------------------------------------------------------
// Disable port 2 ( send 0xF5 command to device)
// ----------------------------------------------------------------------------
void Hook_DisablePS2Port_2(void)
{
    DisablePS2Port_2();
}

// ----------------------------------------------------------------------------
// Enable port 0 ( send 0xF4 command to device) 
// ----------------------------------------------------------------------------
void Hook_EnablePS2Port_0(void)
{
    EnablePS2Port_0();
}

// ----------------------------------------------------------------------------
// Enable port 1 ( send 0xF4 command to device) 
// ----------------------------------------------------------------------------
void Hook_EnablePS2Port_1(void)
{
    EnablePS2Port_1();
}

// ----------------------------------------------------------------------------
// Enable port 0 ( send 0xF4 command to device) 
// ----------------------------------------------------------------------------
void Hook_EnablePS2Port_2(void)
{
    EnablePS2Port_2();
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
void Hook_TPOnlyLowLevelFunc(void)
{

}