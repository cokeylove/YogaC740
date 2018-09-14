/*-----------------------------------------------------------------------------
 * TITLE: CORE_XLT.C - Scan Matrix handler at CORE level
 *
 * The procedure Send_Scan2 of this module is called to translate keyboard
 * scanner contact events into set 2 scan codes.
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/
 
#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

/* ----------------------------------------------------------------------------
 * FUNCTION: Send_Scan2
 *
 * Input: key   = Row/Column number
 *                key.field.output = 0 - 15
 *                key.field.input  = 0 - 7
 *
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *
 *        Scanner_State byte
 *
 *        Ext_Cb3_OVL_STATE
 *
 * On return, Scanner_State byte and Ext_Cb3_OVL_STATE are updated.
 *
 * Combination flag for each row/column
 *     0 = Insyde Software key number 2 in Row/Column Table
 *     1 = index for control_ptr_table in Row/Column Table
 *
 * Insyde Software Key Number 2
 *     01-7F = Same as Scan Code, Set 2
 *     80-FF = E0-prefix, cursor, printscreen, pause/break, special key
 * ------------------------------------------------------------------------- */
void Send_Scan2(union KEY key, BYTE event)
{
	if(IS_MASK_SET(UtilityFlag,KBDebug))
	{
		if(KeyScanACK==KU_ACK_Start)
		{
			if(event==MAKE_EVENT)
			{
				KeyScanKSI = key.field.input;
				KeyScanKSO = key.field.output;
				KeyScanACK = KU_ACK_Keyboard;
			}
		}
	}
	else
	{
		#if SUPPORT_KB_RAM_TABLE
	    //Scanner_RAM_Pntr     = &Rc_RAM_Table;
	    //Extendkey_RAM_Pntr   = &Rc_RAM_Table18;

	    ITempB04 = (key.field.input << 4) | key.field.output;
	    ITempB03 = *(Scanner_RAM_Pntr + ITempB04);
		#else
	    //Scanner_Table_Pntr   = &Rc_ROM_Tables;
	    //Extendkey_Table_Pntr = &EtKey_ROM_Tables;

    	ITempB04 = (key.field.input << 4) | key.field.output;
		ITempB03 = *(Scanner_Table_Pntr + ITempB04);
		#endif

		Hook_keyboard(key.field.input, key.field.output);
    	Send_Key(ITempB03, event);
	}
}


/* ----------------------------------------------------------------------------
 * FUNCTION: Send_Key
 * ------------------------------------------------------------------------- */
void Send_Key(BYTE table_entry, BYTE event)
{
    BYTE temp;
    const BYTE code *pntr;
    BYTE temp_table_entry;

    OEM_Hook_Send_Key(table_entry,event);

    if(OEM_Hook_Skip_Send_Key()==0xFF)
    {
        return;
    }

    temp_table_entry = table_entry;

    Buffer_Mark();									// Mark Buffer in case of overrun.
    temp_scanner_state.byte = Scanner_State;
    if (table_entry >= SSKEY2_OVL_CODE)				// Fn key + any key.
    {   
        /*
        	temp = (table_entry - SSKEY2_OVL_CODE);
        	temp = temp << 1;   						// Multiply 2 because each entry takes 2 bytes 
		if (temp_scanner_state.field.tFN)
		{   										// Fn key
            		temp++; // Increment index to get the odd byte of table entry 
        	}
       	 table_entry = sskey2_overlay_table[temp];	// Get a sskey2 value. 
        	*/
        
        temp = (table_entry - SSKEY2_OVL_CODE);
        temp = temp << 1;// Multiply 2 because each entry takes 2 bytes 

        #if KB_FnStickKey
        if(FnStickKey)
        {  
            temp++; // Increment index to get the odd byte of table entry 
        }
        else
        #endif
        {
    		if (temp_scanner_state.field.tFN)
    		{// Fn key
                temp++; // Increment index to get the odd byte of table entry 
    			table_entry_bk =temp_table_entry;
            }
            else
            {
                if(event==BREAK_EVENT)
                {
    				if(table_entry_bk==temp_table_entry)				
                    {
                        temp++;     // Increment index to get the odd byte of table entry 
                    }
                    table_entry_bk = 0x00;
                }
                else	//MAKE_EVENT
                {
                    if(table_entry_bk==temp_table_entry)
    				{
                        temp++;     // Increment index to get the odd byte of table entry 
                    }
                    else
                    {
                        table_entry_bk = 0x00;
                    }
                }
            }
        }
		//Modify hotkey(F1-F12) implement method.
		if((IS_MASK_CLEAR(ACPI_HOTKEY, HotkeyDisable))&&(IS_MASK_SET(SYS_MISC1,ACPI_OS)))//72JERRY024
		{
        	table_entry = sskey2_overlay_table1[temp];	// Get a sskey2 value. 
		}
		else
		{
			table_entry = sskey2_overlay_table[temp];	// Get a sskey2 value. 
		}
       //Modify hotkey(F1-F12) implement method. 
    }
    else if (table_entry >= SSKEY2_SPE_CODE)		// Combination Key
    {  
        temp = (table_entry - SSKEY2_SPE_CODE); 	// Get index into table. 
        table_entry = sskey2_A2_table[temp].comb;	// table_entry contains the combination table entry. 
        pntr = sskey2_A2_table[temp].pntr;			// pntr points to the first key byte. 
        //pntr += calc_index_comb(table_entry);		// Check combination.  Point to the correct byte. 
        pntr += calc_index_comb(table_entry,temp_table_entry,event);
        table_entry = *pntr;    				// Get a sskey2. 
    }

    sskey3_proc(table_entry, event);    		// Generate scan code, set 2.

	Scanner_State = temp_scanner_state.byte;	// Update scanner state. 

    #if KB_FnStickKey
    if(FnStickKey)
    {   
        if(event==BREAK_EVENT)
        {
            if(temp_table_entry!=0x8E)  // Fn key
            {
                FnStickKey=0;
            }
        }
    }

    if(event==BREAK_EVENT)
    {
        if(table_entry_bk==temp_table_entry)
        {
            table_entry_bk=0x00;
        }
    }
    #endif
}

/* ----------------------------------------------------------------------------
 * FUNCTION: Clear_Fn_Keys
 * ------------------------------------------------------------------------- */
void Clear_Fn_Keys(void)
{
    //Fn_Make_Key1 = 0;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: sskey3_proc
 *
 * Input: sskey2 = Insyde Software Key 2
 *        event =  Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *
 * Note: FUNCT_PTR_V_BS is a pointer to a function that returns nothing (V for
 *       void) and takes a BYTE and a BYTE for parameters (B for BYTE, S for
 *       BYTE).
 * ------------------------------------------------------------------------- */
// Key Code Process Table.
const FUNCT_PTR_V_BS code kcp_vector_table[] =
{
    simple_code,        // index 00h DO_SIMPLE_CODE IBM key number 83,84 
    e0_prefix_code,     // index 01h DO_E0_PREFIX_CODE
    cursor_shift,       // index 02h DO_CURSOR_SHIFT
    cursor_numlock,     // index 03h DO_CURSOR_NUMLOCK
    special_code,       // index 04h DO_SPECIAL_CODE IBM key number 126 
    control_effect,     // index 05h DO_CONTROL_EFFECT Shift, Ctrl, Alt, Numlock 
    control_effect_e0,	// index 06h DO_CONTROL_EFFECT_E0 Ctrl, Alt 
    costomer_function	// index 07h DO_COSTOMER_FUNCTION costomer function key
};

static void sskey3_proc(BYTE sskey2, BYTE event)
{
    BYTE code_byte;
    BYTE index;
    FLAG quick_key = 0;

    if (sskey2 == 0)				// Null code 
    {   			
        ;					 
    }
    else if ((sskey2 & 0x80) == 0)
    {
     	simple_code(sskey2, event);	// 01h through 7Fh = scan code. 
    }
    else  							// 80h through FFh. 
    {   
        sskey2 &= 0x7F;
        sskey2 = sskey2 << 1;
        code_byte = sskey3_80_table[sskey2+0];
        index = sskey3_80_table[sskey2+1];
        (kcp_vector_table[index])(code_byte, event); // Do procedure 
    } 
}


/* ----------------------------------------------------------------------------
 * FUNCTION: calc_index_comb
 *
 * Input: combination setting
 *             Bit   Meaning
 *             ---   --------
 *             7     reserved
 *             6     reserved
 *             5     Fn
 *             4     Num Lock
 *             3     Ctrl
 *             2     Alt
 *             1     Shift
 *             0     reserved
 *
 *       temp_scanner_state
 *
 * Return: offset.
 * ------------------------------------------------------------------------- */
//static BYTE calc_index_comb(BYTE comb)
static BYTE calc_index_comb(BYTE comb, BYTE table_entry, BYTE event)
{
    BYTE offset;
    BYTE bit_num;

    offset  = 0;
    bit_num = 0;

    if (comb & BIT(1))
    {   									// Combination has Shift. 
    	if (temp_scanner_state.field.tLEFT | temp_scanner_state.field.tRIGHT)
		{   								// Either left or right shift is pressed. 
            offset |= Byte_Mask(bit_num);
        }
        bit_num++;  						// Increment bit position. 
    }

    if (comb & BIT(2))
    {   									// Combination has Alt. 
		if (temp_scanner_state.field.tALT)
        {   								// Alt is pressed. 
            offset |= Byte_Mask(bit_num);
        }
        bit_num++;  						// Increment bit position. 
    }

   	if (comb & BIT(3))
    {   									// Combination has Ctrl. 
		if (temp_scanner_state.field.tCONTROL)
        {  	 								// Ctrl is pressed. 
            offset |= Byte_Mask(bit_num);
        }
        bit_num++;  						// Increment bit position. 
    }

    if (comb & BIT(4))
    {   									// Combination has Num Lock. 
        if (temp_scanner_state.field.tNUM_LOCK)
        {   								// NumLock has been pressed. 
            offset |= Byte_Mask(bit_num);
        }
        bit_num++; 	 						// Increment bit position. 
    }

    if (comb & BIT(5))
    {   									// Combination has Fn. 
        //if (temp_scanner_state.field.tFN)
        //{   								// Fn is pressed. 
        //    offset |= Byte_Mask(bit_num);
        //}
	/*if((table_entry - SSKEY2_SPE_CODE) >= 0x11 && (table_entry - SSKEY2_SPE_CODE) <= 0x1C)
	{
	    if(IS_MASK_CLEAR(ACPI_HOTKEY, HotkeyDisable))
	    {
	          if (!temp_scanner_state.field.tFN) // no has Fn.
		   {
			offset |= Byte_Mask(bit_num);
		    }
	     }
	     else //HotKey_Mode = 0
	     {
		    if (temp_scanner_state.field.tFN)  //has Fn.
		    {									// Fn is pressed. 
			offset |= Byte_Mask(bit_num);
		    }
	      }
       }
       else
	{*/
         if (temp_scanner_state.field.tFN)
         {   								// Fn is pressed. 
             offset |= Byte_Mask(bit_num);
             table_entry_bk = table_entry;
         }
         else
         {
            if(event==BREAK_EVENT)
            {
                if(table_entry_bk==table_entry)
                {
                    offset |= Byte_Mask(bit_num);  
                }
                table_entry_bk = 0x00;
            }
            else
            {
                if(table_entry_bk==table_entry)
                {
                    offset |= Byte_Mask(bit_num);
                }
                else
                {
                    table_entry_bk = 0x00;
                }
            }
          }
        #if KB_FnStickKey
        if(FnStickKey)
        {   								// Fn is pressed. 
            offset |= Byte_Mask(bit_num);
        }
        #endif
        
        bit_num++;  						// Increment bit position. 
    }

    if (comb & BIT(6))
    {   									// Combination has Hook_calc_index_comb_BIT6 
        if(Hook_calc_index_comb_BIT6()==0xFF)
        {
            offset |= Byte_Mask(bit_num);
        }
        bit_num++;  						// Increment bit position. 
    }

    if (comb & BIT(7))
    {   									// Combination has Hook_calc_index_comb_BIT7 
        if(Hook_calc_index_comb_BIT7()==0xFF)
        {
            offset |= Byte_Mask(bit_num);
        }
        bit_num++;  						// Increment bit position. 
    }
    
    return(offset);
}

/* ----------------------------------------------------------------------------
 * FUNCTION: simple_code
 *
 * Input: scan_code, Set 2
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 * ------------------------------------------------------------------------- */
void simple_code(BYTE scan_code, BYTE event)
{
    if (event == BREAK_EVENT)
    {  
        Buffer_Key(0xF0);	// Buffer break prefix for break contact. F0h is break prefix. 
    }
    Buffer_Key(scan_code);  // Buffer base code. 
}

/* ----------------------------------------------------------------------------
 * FUNCTION: e0_prefix_code
 *
 * Input: scan_code, Set 2
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 * ------------------------------------------------------------------------- */
void e0_prefix_code(BYTE scan_code, BYTE event)
{
    Buffer_Key(0xE0);   	// Buffer E0h prefix. 
    if (event == BREAK_EVENT)
    {   					// Buffer break prefix for break contact. F0h is break prefix. 
        Buffer_Key(0xF0);
    }
    Buffer_Key(scan_code);  // Buffer base code. 
}

/* ----------------------------------------------------------------------------
 * FUNCTION: cursor_shift
 *
 * Input: scan_code, Set 2
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *        temp_scanner_state
 * ------------------------------------------------------------------------- */
/* BREAK_EVENT key string for cursor_shift and cursor_numlock. */
const BYTE code csr_brk1[] = {0xE0,0xF0,0x00};

static void cursor_shift(BYTE scan_code, BYTE event)
{
    static const BYTE code csr_sftl_mk[]   = {0xE0,0xF0,0x12,0x00};
    static const BYTE code csr_sftr_mk[]   = {0xE0,0xF0,0x59,0x00};
    static const BYTE code csr_sftl_brk2[] = {0xE0,0x12,0x00};
    static const BYTE code csr_sftr_brk2[] = {0xE0,0x59,0x00};

    if (event == BREAK_EVENT)
    {   /* Key has just been released. This is a "break event". */
        Buffer_String(csr_brk1);    		/* Buffer pre-string. */
        Buffer_Key(scan_code);      		/* Buffer base code. */

        if (temp_scanner_state.field.tLEFT)
        {  									/* Left shift has been pressed. */
            Buffer_String(csr_sftl_brk2);   /* Buffer tail-string. */
        }

        if (temp_scanner_state.field.tRIGHT)
        { 									/* Right shift has been pressed. */
            Buffer_String(csr_sftr_brk2);   /* Buffer tail-string. */
        }
    }
    else if (event == MAKE_EVENT)
    {   /* Key is pressed for the first time, a "make event". */
        									/* Left and/or Right SHIFT is pressed. */
        if (temp_scanner_state.field.tLEFT)
        {  									/* Left SHIFT is pressed. */
            Buffer_String(csr_sftl_mk); 	/* Buffer pre-string. */
        }

        if (temp_scanner_state.field.tRIGHT)
        { 									/* Right SHIFT is pressed. */
            Buffer_String(csr_sftr_mk); 	/* Buffer pre-string. */
        }

        Buffer_Key(0xE0);       			/* Buffer end of pre-string. */
        Buffer_Key(scan_code);  			/* Buffer base code. */
    }
    else
    {   /* Key is being held pressed for a length of time, a "repeat event". */
        Buffer_Key(0xE0);       			/* Buffer E0h prefix */
        Buffer_Key(scan_code);  			/* Buffer base code */
    } 
}

/* ----------------------------------------------------------------------------
 * FUNCTION: cursor_numlock
 *
 * Input: scan_code, Set 2
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 * ------------------------------------------------------------------------- */
static void cursor_numlock(BYTE scan_code, BYTE event)
{
    static const BYTE code csr_numlock_mk[]   = {0xE0,0x12,0xE0,0x00};
    static const BYTE code csr_numlock_brk2[] = {0xE0,0xF0,0x12,0x00};

    if (event == BREAK_EVENT)
    { 
        Buffer_String(csr_brk1);            // Buffer pre-string. 
        Buffer_Key(scan_code);              // Buffer base code. 
        Buffer_String(csr_numlock_brk2);    // Buffer tail-string. 
    }
    else if (event == MAKE_EVENT)
    {  
        									// The Num Lock key is pressed. 
        Buffer_String(csr_numlock_mk);  	// Buffer pre-string. 
        Buffer_Key(scan_code);          	// Buffer base code. 
    }
    else
    {  
        Buffer_Key(0xE0);       			// Buffer E0h prefix. 
        Buffer_Key(scan_code);  			// Buffer base code. 
    } 
}


/* ----------------------------------------------------------------------------
 * FUNCTION: special_code
 *
 * For IBM Key Number 126. This key generates only make code.
 *
 * Input: code
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 * ------------------------------------------------------------------------- */
void special_code(BYTE code_byte, BYTE event)
{
    static const BYTE code key126_normal_mk[] = {0xE1,0x14,0x77,0xE1,0xF0,0x14,0xF0,0x77,0x00};
    //static const BYTE code key126_ctrl_mk[]   = {0xE0,0x7E,0xE0,0xF0,0x7E,0x00};//72JERRY038:-Modify break scancode.
	 static const BYTE code key126_ctrl_mk[]   = {0x14,0xE0,0x7E,0xE0,0xF0,0x7E,0xF0,0x14,0x00};//72JERRY038:+Modify break scancode.

    if (event == MAKE_EVENT)
    {
        if (code_byte == 0) 
		{   
           	Buffer_String(key126_normal_mk);// Buffer normal code string.
        }
        else 
		{  
            Buffer_String(key126_ctrl_mk);	// Buffer Ctrl case string.
        }
    }
}

/* ----------------------------------------------------------------------------
 * FUNCTION: costomer_function
 *
 * Input: code
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 * ------------------------------------------------------------------------- */

//----------------------------------------------------------------------------
// The function of hotkey Fn + Fx
//----------------------------------------------------------------------------
static void costomer_function(BYTE code_byte, BYTE event)
{
	(HotKey_Fn_Fx[code_byte])(event);
	if (scan.kbf_head != scan.kbf_tail)
	{
        SetServiceSendFlag();
	}
}

/* ----------------------------------------------------------------------------
 * FUNCTION: control_effect
 *
 * Generate scan code set 2 and update scanner status.
 *
 * Input: state flags
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *        temp_scanner_state
 *
 *  The following bits are defined for state flags and temp_scanner_state:
 *       NUM_LOCK
 *       LEFT
 *       RIGHT
 *       ALT
 *       CONTROL
 *       OVERLAY
 *       FN
 *       
 *
 * Returns: Updated temp_scanner_state
 * ------------------------------------------------------------------------- */
static void control_effect(BYTE state, BYTE event)
{
    if ((event == MAKE_EVENT)||(event == BREAK_EVENT))
    {
    	if (state & LShift)				// First generate scan code set 2.
		{
        	simple_code(0x12, event);
			//T057A + s
			/*   //T057B +
               	if(event == MAKE_EVENT)
               	{
                    	SET_MASK(EM9_NEWFUN,b7_ShiftPress);
               	}
               	else if(event == BREAK_EVENT)
                   {
                        	CLEAR_MASK(EM9_NEWFUN,b7_ShiftPress);
                   }
			*/  //T057B +                     
			//T057A + e
    	}
 		else if (state & LAlt)
		{
        	simple_code(0x11, event);
    	}
		else if (state & LCtrl) 
		{
        	simple_code(0x14, event);
    	}
    	else if (state & RShift) 
		{
            simple_code(0x59, event);
			//T057A + s
			/*   //T057B+
                    if(event == MAKE_EVENT)
                    {
                    	SET_MASK(EM9_NEWFUN,b7_ShiftPress);
                    }
                    else if(event == BREAK_EVENT)
                    {
                        	CLEAR_MASK(EM9_NEWFUN,b7_ShiftPress);
                    }
			*/   //T057B+                     
			//T057A + e                
    	}
		//else if (state & FN) 
		//{
        //	e0_prefix_code(0x63, event);
    	//}
    }
   
    if (event == MAKE_EVENT)		 	// Then update scanner state.
    {   // Key is pressed for the first time. This is a "make event".
		// Set scanner state for SHIFT, Alt, Ctrl, or Fn.
    	temp_scanner_state.byte |= state;
        if (state & FN) 
        {
            Hook_Fn_Key_Make();
            #if KB_FnStickKey
            FnStickKey = 1;
            #endif
        }
    }
    else if (event == BREAK_EVENT)
    {   
        if (state & FN) 
        {
            Hook_Fn_Key_Break();
        }
     	temp_scanner_state.byte &= ~state;
    }
    else
    { 
        ;   // Do nothing for repeat event. 
    }
}


/* ----------------------------------------------------------------------------
 * FUNCTION: control_effect_e0
 *
 * Generate scan code set 2 and update scanner status.
 * Only for Alt-Right and Ctrl-Right.
 *
 * Input: state flags
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *        temp_scanner_state
 *
 * The following bits are defined for state_flags and temp_scanner_state:
 *       NUM_LOCK
 *       LEFT
 *       RIGHT
 *       ALT
 *       CONTROL
 *       OVERLAY
 *       FN
 *
 * Return: Updated temp_scanner_state
 * ------------------------------------------------------------------------- */
static void control_effect_e0(BYTE state, BYTE event)
{   // This is only called for Alt-Right and Ctrl-Right. 
    if ((event == MAKE_EVENT)||(event == BREAK_EVENT))
    {
    	// First setup to generate scan code set 2. 
    	if (state & RAlt) 
		{
			e0_prefix_code(0x11, event);
    	}
		if (state & RCtrl) 
		{
			e0_prefix_code(0x14, event);
    	}
	}

	if (event == MAKE_EVENT)
 	{   	// Key is pressed for the first time. This is a "make event". 
            // Set one flag in scanner state flags. 
     	temp_scanner_state.byte |= state;
  	}
	else if (event == BREAK_EVENT)
	{   	// Key has just been released. This is a "break event". 
            // Clear one flag in scanner state flags. 
      	temp_scanner_state.byte &= ~state;
  	}
 	else
  	{   
       	;   // Do nothing for repeat event. 
 	} 		
}




