//*****************************************************************************
//
//  oem_ucsi_opm_ppm.c
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author:
//
//*****************************************************************************
//*****************************************************************************
// Include all header file
#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

#if SUPPORT_UCSI
/**
 * ****************************************************************************
 * external memory copy
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void std_memcpy(XBYTE *p_destination,XBYTE *p_source,XWORD P_num)
{
	XWORD l_num;

	for(l_num=0x00;l_num<P_num;l_num++)
	{
		*(p_destination+l_num) = *(p_source+l_num);
	}
}

/**
 * ****************************************************************************
 * external memory set
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void std_memset(XBYTE *p_ptr,XBYTE p_value,XWORD P_num)
{
	XWORD l_num;

	for(l_num=0x00;l_num<P_num;l_num++)
	{
		*(p_ptr+l_num) = p_value;
	}
}

/**
 * ****************************************************************************
 * external memory compare
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
BYTE std_memcmp(XBYTE *ptr1,XBYTE *ptr2,XWORD P_num)
{

	XWORD l_num;

	for(l_num=0x00;l_num<P_num;l_num++)
	{
		if(*(ptr1+l_num) != *(ptr2+l_num))
		{	/* no match */
			return 1;
		}
	}

	/* match */
	return 0;
}

BYTE CCG4_UCSI_Reg_Read(BYTE Cmd_Region,BYTE *R_data_buffer,BYTE R_Length)
{
	XBYTE SMB_DATA_OEM = Cmd_Region;

	if (I2CCompatibleWrite2Read(UCSI_PD_channel,CYPRESS_CCG4_UCSI_ADDR,1,&SMB_DATA_OEM,R_Length,R_data_buffer))
	{
		return TRUE;
	}

	return FALSE;
}

BYTE CCG4_UCSI_Reg_Write(BYTE Cmd_Region,BYTE *W_data_buffer,BYTE W_Length)
{
	BYTE i;

	UCSI_TUNNEL_Buffer[0] = Cmd_Region;

	for (i = 0; i < W_Length ; i++)
	{
		UCSI_TUNNEL_Buffer[i+1] = *W_data_buffer;
		W_data_buffer++;
	}

	if (I2CCompatibleWrite(UCSI_PD_channel,CYPRESS_CCG4_UCSI_ADDR,(W_Length+1),&UCSI_TUNNEL_Buffer))
	{
		return TRUE;
	}

	return FALSE;
}

void ucsi_reset(void)
{
	//P_UCSI_DATA p = &pUCSI;

	

	//pUCSI = *p;
	//72JERRY083:s+Change MMIO to mailbox for accessing UCSI registers.
	BYTE index;
	std_memset(&pUCSI,0x00,sizeof(T_UCSI_DATA));
	for(index=0; index< 128; index++)
	{
		BRAMBK0[index] = 0x00;
	}
	//72JERRY083:e+Change MMIO to mailbox for accessing UCSI registers.	
}

BYTE ucsi_init(XBYTE *pBuffer)
{
	if (pBuffer)
	{
		std_memset(pBuffer,0x00,sizeof(T_UCSI_DATA));

		//pUCSI = (P_UCSI_DATA)pBuffer;

		// pPPM.pUCSI->u16Version = UCSI_TUNNEL_VERSION;

		ucsi_reset();
	}

	UCSI_Tunnel_Proc = TUNNEL_WAIT_INIT;
	//UCSI_Tunnel_Step = 0;

	SCLKTS_A = 0x03; // Select the SMBUS1 clock to 400Khz.

///	Init_PD_INT_Wakeup(); Must init it

	return sizeof(T_UCSI_DATA);
}


void CCG4_tunnel_set_alert(void)
{
	//if (pPPM.u16NotifyMask & BIT0)
	//{
	//	UCSI_TUNNEL_DEBUG(0xC5);
	//	pPPM.u8MISC.field.sci_wait_ack = 1;
		ECSMI_SCIEvent(QUCSI_Data_Ready);
	//}
	//else
	//{
	//	UCSI_TUNNEL_DEBUG(0xC6);
	//}
}

void CCG4_Tunnel_Proc_Change(BYTE proc_step)
{
	/* Proc Change Debug */
	//UCSI_TUNNEL_DEBUG(0xC0+proc_step);

	//UCSI_Tunnel_Step = 0;
	UCSI_Tunnel_Proc = proc_step;
}

BYTE CCG4_Tunnel_Release_Interrupt(BYTE INTR)
{
	XBYTE result = FALSE;
	XBYTE PD_Write_Buffer[3]; 

	/* Release the CCG4_INT# pin to high */
	PD_Write_Buffer[0] = 0x06;  // INTR_REG (0x0006)
	PD_Write_Buffer[1] = 0x00;
	PD_Write_Buffer[2] = INTR;  // Write one to clear for BIT3 (Release the INT# pin)

	if (I2CCompatibleWrite(UCSI_PD_channel, CYPRESS_CCG4_HPI_ADDR ,3,&PD_Write_Buffer))
	{
		result = TRUE;
	}

	return result;
}

BYTE CCG4_Read_Interrupt_status(void)
{
	XBYTE result = FALSE;
    XBYTE PD_Write_Buffer[2];
	XBYTE PD_Read_Buffer[2];

	/* Read status*/
	PD_Write_Buffer[0] = 0x06;
	PD_Write_Buffer[1] = 0x00;

	if (I2CCompatibleWrite2Read(UCSI_PD_channel,CYPRESS_CCG4_HPI_ADDR ,2,&PD_Write_Buffer,1,&PD_Read_Buffer))
	{
		UCSI_Int_status = PD_Read_Buffer[0];
		result = TRUE;
		return TRUE;
	}
	RamDebug(0xDC);
	RamDebug(0xFF);
	UCSI_Int_status = 0x00;

	return result;
}
//
// 72JERRY069: Start Support identify typec adapter ID 
//
BYTE CCG4_Read_Adapter_Watts(void)
{
	XBYTE result = FALSE;
    XBYTE PD_Write_Buffer[2];
	XBYTE PD_Read_Buffer[2];

	/* Read status*/
	PD_Write_Buffer[0] = 0x41;
	PD_Write_Buffer[1] = 0x00;

	if (I2CCompatibleWrite2Read(UCSI_PD_channel,CYPRESS_CCG4_HPI_ADDR ,2,&PD_Write_Buffer,1,&PD_Read_Buffer))
	{
		TYPEC_Adapter_Watts = PD_Read_Buffer[0];
		result = TRUE;
		return TRUE;
	}

	TYPEC_Adapter_Watts = 0x00;

	return result;
}
void TypecAdapterDetection(void)
{
	if(IS_MASK_CLEAR(SYS_STATUS,AC_ADP))
	{
        CLEAR_MASK(SYS_STATUS,b5UnSuitAdpPow);
	    CLEAR_MASK(SYS_STATUS,b4IllegalAdp);
	    TYPEC_Adapter_Watts=0;
	    AdapterID=0;
        return;
	}
	if (!CCG4_Read_Adapter_Watts())
	{
	    	return; 
	}
	switch(TYPEC_Adapter_Watts)
	{
        case 45:
           AdapterID = AdapterID_45W;
           break;
        case 65:
           AdapterID = AdapterID_65W;
           break;
        case 90:
           AdapterID = AdapterID_90W;
           break;
//72JERRY075: Start optimize 
       // case 120:
       //    AdapterID = AdapterID_120W;
       //   break;
       //case 135:
       //    AdapterID = AdapterID_135W;
       //    break;
       // case 175:
      //     AdapterID = AdapterID_170W;
      //     break;
      //  case 230:
      //     AdapterID = AdapterID_230W;
      //     break;
//72JERRY075: End
        case 0xFF:
           AdapterID = AdapterID_NON_SUPPORT;
           break;
        default:
			AdapterID = AdapterID_NON_SUPPORT;//72JERRY076: NO adapter ID
           break;
	}
	if(TYPEC_Adapter_Watts>=45)
	{
		CLEAR_MASK(SYS_STATUS,b5UnSuitAdpPow);
	    CLEAR_MASK(SYS_STATUS,b4IllegalAdp);
	}
	if(TYPEC_Adapter_Watts<45 && TYPEC_Adapter_Watts>=36)
	{
		SET_MASK(SYS_STATUS,b5UnSuitAdpPow);
	    CLEAR_MASK(SYS_STATUS,b4IllegalAdp);
	}
	if(TYPEC_Adapter_Watts<36 && TYPEC_Adapter_Watts>=5)
	{
		CLEAR_MASK(SYS_STATUS,b5UnSuitAdpPow);
	    SET_MASK(SYS_STATUS,b4IllegalAdp);
	}
}
//
//72JERRY069: End 
//
/**
 ** The main process of handling UCSI
 ** Hook 100ms
 **/

BYTE ucsi_run(void)
{
   if(IS_MASK_SET(PDFWStatus,b5PDFWUpdate)) //When PD FW udpate, EC don't handle UCSI
   {
       //72JERRY075: Start optimize ucsi function
   	   // if(SysPowState !=SYSTEM_S0)
   		//{
   	  	//	CLEAR_MASK(PDFWStatus,b5PDFWUpdate);  	
   	  	//	CCG4_Tunnel_Proc_Change(0xFF); //After FW update and shutdown, EC start to handle UCSI
	   // 	UCSI_Data_status=0x00;
	    //	UCSI_Int_status=0x00;
	   // 	ucsi_reset();
	  //  }
	  //72JERRY075: End
	   return 0;
   }
   if((IS_MASK_CLEAR(SYS_MISC1,ACPI_OS) ||((SYS_STATUS&0x07) != 0x06))&& (SysPowState !=SYSTEM_S0))    //72JERRY069: Only do UCSI in S0//72JERRY076: Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
   {
   	  	CLEAR_MASK(PDFWStatus,b5PDFWUpdate);  	
   	  	CCG4_Tunnel_Proc_Change(TUNNEL_SERVICE); //After FW update and shutdown, EC start to handle UCSI
	    UCSI_Data_status=0x00;
	    UCSI_Int_status=0x00;
	    UCSI_Q20_Retry=0;//72JERRY076: Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
	    ucsi_reset();
	    return 0;
	}

   //72JERRY075: End -Optimize ucsi method only running the method under S0 mode and after ACPI init.
	//TypecAdapterProction();
   // TUNNEL Initial Debug 
	//UCSI_TUNNEL_DEBUG(0xA0);
    //UCSI_TUNNEL_DEBUG(UCSI_Tunnel_Proc);
   switch (UCSI_Tunnel_Proc)
   {
	// ----------------------- 
	// USCI TUNNEL Ready check 
	// ----------------------- 
	case TUNNEL_WAIT_INIT:
		if (CCG4_UCSI_Reg_Read(UCSI_Version,&pUCSI.u16Version, UCSI_Version_SIZE))
		{    
			//RamDebug(0xD0);
			if (pUCSI.u16Version != 0x0000) // non-zero CCG4-Version
			{   
				//RamDebug(0xD1);
				CCG4_Tunnel_Proc_Change(TUNEL_IRQ_HANDLER);
			}
			else
			{
				//RamDebug(0xD2);
				CCG4_Tunnel_Proc_Change(TUNNEL_WAIT_INIT);
			}
		}
	    break;
    // ----------------------- 
	// USCI TUNNEL IRQ Handler
	// -----------------------
    case TUNEL_IRQ_HANDLER:
		//if (IS_UCSI_TUNNEL_INT())
		//{
			//RamDebug(0xD3);
		    if (CCG4_Read_Interrupt_status())
			{
				if(IS_MASK_SET(UCSI_Int_status,b3UCSIIntStatus))
		        {
					//RamDebug(0xD4);
					CCG4_Tunnel_Proc_Change(TUNNEL_LPM_SERVICE);
				}
			}
			break;
		//}
        //break;

	// ----------------------- 
	// USCI TUNNEL service check 
	// ----------------------- 
	case TUNNEL_SERVICE:
	//72JERRY083:s+Change MMIO to mailbox for accessing UCSI registers.
	  if(UCSI_WRITEIN_CMD0==0xE0)
	  {
	  	SET_MASK(UCSI_Data_status,b7OSDataReady);
	  	CLEAR_MASK(UCSI_Data_status,b6CCGDataReady);
	  	UCSI_WRITEIN_CMD0=0;
	  	RamDebug(0xE0);
	  }
	  else
	  {
	  	 if (IS_MASK_SET(UCSI_Data_status,b4QeventBIOSReceived))
	  	 {
	  	 	if (UCSI_READOUT_CMD0==0xE1)
	  	 	{
	  	 		CLEAR_MASK(UCSI_Data_status,b7OSDataReady);
	  	 		CLEAR_MASK(UCSI_Data_status,b6CCGDataReady);
	  			UCSI_READOUT_CMD0=0;
	  			RamDebug(0xE1);
	  	 	}
	  	 }
	  }
	  //72JERRY083:e+Change MMIO to mailbox for accessing UCSI registers.
// 72JERRY075: Start optimize clear Q20 flag after last cmd and cmd completed flag
		if(IS_MASK_SET(UCSI_Data_status,b5CmdCompletedIndicator) &&(IS_MASK_SET(UCSI_Data_status,b3Cmd04_ACK_CC_CI))) 
		{
			//CLEAR_MASK(UCSI_Data_status,b6CCGDataReady);//72JERRY076: Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
			CLEAR_MASK(UCSI_Data_status,b5CmdCompletedIndicator);
			CLEAR_MASK(UCSI_Data_status,b3Cmd04_ACK_CC_CI);
			//RamDebug(0xDD);//72JERRY076: Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
		}
//72JERRY075: Emd 
	    if(IS_MASK_SET(UCSI_Data_status,b7OSDataReady))
		{   
			//RamDebug(0xD8);
			UCSI_Q20_Retry=0;//72JERRY076: Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
            SET_MASK(UCSI_Data_status,b4QeventBIOSReceived);//72JERRY076: Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
			CCG4_Tunnel_Proc_Change(TUNNEL_OPM_SERVICE);
			break;
		}
		else
		{
		  //CLEAR_MASK(UCSI_Data_status,b6CCGDataReady);
		//}
		//if (IS_UCSI_TUNNEL_INT())
		//{
			//RamDebug(0xD5);
			if(IS_MASK_SET(UCSI_Data_status,b4QeventBIOSReceived))//72JERRY076: Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
			{
				if((IS_MASK_CLEAR(UCSI_Data_status,b6CCGDataReady)))//||(IS_MASK_SET(UCSI_Data_status,b4QeventBIOSReceived)))//72JERRY075
				{  
			//ucsi_reset();
					UCSI_Q20_Retry=0;//72JERRY076: Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
					if (CCG4_Read_Interrupt_status())
					{
				//RamDebug(0xD6);
						if(IS_MASK_SET(UCSI_Int_status,b3UCSIIntStatus))//72JERRY076: Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
		        		{
							//RamDebug(0xD7);
							//UCSI_TUNNEL_DEBUG(UCSI_Int_status);
							//ucsi_reset();//72JERRY076: Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
				 			CCG4_Tunnel_Proc_Change(TUNNEL_LPM_SERVICE);
							break;
						}
						//72JERRY076:S+ Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
						else
						{
							if((pUCSI.unCCI0.byte==0x04)&&(pUCSI.unCCI3.byte==0x00))
							{
								if(pUCSI.stControl.u8Cmd!=0x00)
								{
									//SET_MASK(UCSI_Data_status,b7OSDataReady);//72JERRY083: Remove patch for UCSI driver can't access _Q20.
									RamDebug(0xE3);
								}
							}
						}
					}
				}
//72JERRY075£ºStart optimize ucsi function( don't notify Q20 more times.)
				else
				{
					if(IS_MASK_SET(UCSI_Data_status,b6CCGDataReady))
					{
						UCSI_Q20_Retry--;
						if(UCSI_Q20_Retry==0)
						{
							UCSI_Q20_Retry=100;
							//CCG4_tunnel_set_alert();//Patch for UCSI driver can't access _Q20//72JERRY083: Remove patch for UCSI driver can't access _Q20.
						//72JERRY083: s+Optimize ucsi function to hang lenovo log.
							   if (CCG4_Read_Interrupt_status())
                              {
                                   if(IS_MASK_SET(UCSI_Int_status,b3UCSIIntStatus))
                              {
                                         //RamDebug(0xD4);
                                   CCG4_Tunnel_Proc_Change(TUNNEL_LPM_SERVICE);
                               }
                                 else
                                 {
						CCG4_tunnel_set_alert();
                                }
                           }
//72JERRY083: e+Optimize ucsi function to hang lenovo log.
					   	 	RamDebug(0xE2);
						}
					}
				}
          	}
			//72JERRY076: E+Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
//72JERRY075: end
		//	break;
		}
		//if(IS_MASK_CLEAR(UCSI_Data_status,b6CCGDataReady))
		//{  
		//if(IS_MASK_SET(UCSI_Data_status,b7OSDataReady))
		//{   
			//RamDebug(0xD8);
		//	CCG4_Tunnel_Proc_Change(TUNNEL_OPM_SERVICE);
		//	break;
		//}
		//}
	    break;

	//----------------------- 
	// LPM Service -> TUNNEL Service   
	// ----------------------- 
	case TUNNEL_LPM_SERVICE:
		if(IS_MASK_SET(UCSI_Int_status,b3UCSIIntStatus))
		{
		  if(IS_MASK_SET(SYS_MISC1,ACPI_OS) && ((SYS_STATUS&0x07) == 0x06) &&(SysPowState==SYSTEM_S0))//can move to in front of accessing PD//72JERRY076: Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
		  {
			if (CCG4_UCSI_Reg_Read(UCSI_MessageIn,&pUCSI.pMsgIn, UCSI_MessageIn_SIZE))
			{
				RamDebug(0xD9);
				std_memcpy(&UCSI_MSG_IN,&pUCSI.pMsgIn, UCSI_MessageIn_SIZE);//72JERRY083:Change MMIO to mailbox for accessing UCSI registers.
				//RamDebug(BRAMBK0[0x14]);
				if (CCG4_UCSI_Reg_Read(UCSI_Cci,&pUCSI.unCCI0, UCSI_CCI_SIZE))
				{
				    RamDebug(0xDA);
					std_memcpy(&UCSI_CCI,&pUCSI.unCCI0, UCSI_CCI_SIZE);//72JERRY083:Change MMIO to mailbox for accessing UCSI registers.
				    //RamDebug(BRAMBK0[0x10]);
				    //RamDebug(BRAMBK0[0x13]);
				    CLEAR_MASK(UCSI_Data_status,b5CmdCompletedIndicator);
//72JERRY075: Start check busy and complete cmd
				    if(pUCSI.unCCI3.byte==0x20)
				    {
				    	SET_MASK(UCSI_Data_status,b5CmdCompletedIndicator);
				    }
				    if(pUCSI.unCCI3.byte==0x10)
				    {
				    	RamDebug(0xE5);
				    	//CLEAR_MASK(UCSI_Data_status,b6CCGDataReady);//7RJERRY004: Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
				    	//CCG4_tunnel_set_alert();//7RJERRY004: Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
						//CCG4_Tunnel_Release_Interrupt(UCSI_Int_status);//7RJERRY004: Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
						
				    	//CCG4_Tunnel_Proc_Change(TUNNEL_SERVICE);//72JERRY076: Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
						//break;//72JERRY076: Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
				    }
//72JERRY075: End
				   	//SET_MASK(UCSI_Data_status,b6CCGDataReady);//72JERRY075
				   	if(IS_MASK_SET(SYS_MISC1,ACPI_OS) && ((SYS_STATUS&0x07) == 0x06) && (SysPowState==SYSTEM_S0))
	               	{
	               		SET_MASK(UCSI_Data_status,b6CCGDataReady); //72JERRY075: optimize 
                    	CCG4_tunnel_set_alert();
						CCG4_Tunnel_Release_Interrupt(UCSI_Int_status);
						UCSI_Q20_Retry=100;//72JERRY076: Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
				    	CCG4_Tunnel_Proc_Change(TUNNEL_SERVICE);
						break;
				   	}
				}
			}
		  }
		}
		break;

	// -----------------------
	//   OPM Service  -> TUNNEL Service  
	// ----------------------- 
	case TUNNEL_OPM_SERVICE:
	    if(IS_MASK_SET(UCSI_Data_status,b7OSDataReady))
		{  
		     std_memcpy(&pUCSI.pMsgOut,&UCSI_MSG_OUT,UCSI_MessageOut_SIZE);//72JERRY083:Change MMIO to mailbox for accessing UCSI registers.
			if (CCG4_UCSI_Reg_Write(UCSI_MessageOut,&pUCSI.pMsgOut,UCSI_MessageOut_SIZE))
		    {
				RamDebug(0xDB);
 				//RamDebug(BRAMBK0[0x24]);
				//std_memcpy(&UCSI_MSG_OUT,&pUCSI.pMsgOut,UCSI_MessageOut_SIZE);
				 std_memcpy(&pUCSI.stControl,&UCSI_CONTROL,UCSI_CONTROL_SIZE);//72JERRY083:Change MMIO to mailbox for accessing UCSI registers.
				if (CCG4_UCSI_Reg_Write(UCSI_Control,&pUCSI.stControl,UCSI_CONTROL_SIZE))
				{   
				   RamDebug(0xDC);
				   //RamDebug(BRAMBK0[0x34]);
				   //std_memcpy(&UCSI_CONTROL,&pUCSI.stControl,UCSI_CONTROL_SIZE);
//72JERRY075: Start optimize check last completed cmd 
				   CLEAR_MASK(UCSI_Data_status,b3Cmd04_ACK_CC_CI);
				   if(pUCSI.stControl.u8Cmd==0x04)
				   {
				   	  SET_MASK(UCSI_Data_status,b3Cmd04_ACK_CC_CI);
				   }
//72JERRY075: End
					CLEAR_MASK(UCSI_Data_status,b7OSDataReady);
				    //CLEAR_MASK(UCSI_Data_status,b6CCGDataReady);
					//CLEAR_MASK(UCSI_Data_status,b4QeventBIOSReceived);//72JERRY076: Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
				 
				//	ucsi_reset();//72JERRY076: Optimize ucsi function to handle OPM,EC notify OS every 2S by Q_20.
					CCG4_Tunnel_Proc_Change(TUNNEL_SERVICE);
				}
			}
        }
		break;
	default:
	    //CCG4_Tunnel_Proc_Change(TUNNEL_WAIT_INIT);
		CCG4_Tunnel_Proc_Change(TUNNEL_OPM_SERVICE);
	    UCSI_Data_status=0x00;
	    UCSI_Int_status=0x00;
	    ucsi_reset();
	    //SCLKTS_A = 0x03;//CLK 400KHz
	    break;
	}
	return 0;
}

//72JERRY081: s+Dsable UCSI function.
#else
BYTE CCG4_Read_Adapter_Watts(void)
{
	XBYTE result = FALSE;
    XBYTE PD_Write_Buffer[2];
	XBYTE PD_Read_Buffer[2];

	/* Read status*/
	PD_Write_Buffer[0] = 0x41;
	PD_Write_Buffer[1] = 0x00;

	if (I2CCompatibleWrite2Read(UCSI_PD_channel,CYPRESS_CCG4_HPI_ADDR ,2,&PD_Write_Buffer,1,&PD_Read_Buffer))
	{
		TYPEC_Adapter_Watts = PD_Read_Buffer[0];
		result = TRUE;
		return TRUE;
	}

	TYPEC_Adapter_Watts = 0x00;

	return result;
}

void TypecAdapterDetection(void)
{
	if(IS_MASK_CLEAR(SYS_STATUS,AC_ADP))
	{
        CLEAR_MASK(SYS_STATUS,b5UnSuitAdpPow);
	    CLEAR_MASK(SYS_STATUS,b4IllegalAdp);
	    TYPEC_Adapter_Watts=0;
	    AdapterID=0;
        return;
	}
	if (!CCG4_Read_Adapter_Watts())
	{
	    	return; 
	}
	switch(TYPEC_Adapter_Watts)
	{
        case 45:
           AdapterID = AdapterID_45W;
           break;
        case 65:
           AdapterID = AdapterID_65W;
           break;
        case 90:
           AdapterID = AdapterID_90W;
           break;
//72JERRY075: Start optimize 
       // case 120:
       //    AdapterID = AdapterID_120W;
       //   break;
       //case 135:
       //    AdapterID = AdapterID_135W;
       //    break;
       // case 175:
      //     AdapterID = AdapterID_170W;
      //     break;
      //  case 230:
      //     AdapterID = AdapterID_230W;
      //     break;
//72JERRY075: End
        case 0xFF:
           AdapterID = AdapterID_NON_SUPPORT;
           break;
        default:
			AdapterID = AdapterID_NON_SUPPORT;//72JERRY076: NO adapter ID
           break;
	}
	if(TYPEC_Adapter_Watts>=45)
	{
		CLEAR_MASK(SYS_STATUS,b5UnSuitAdpPow);
	    CLEAR_MASK(SYS_STATUS,b4IllegalAdp);
	}
	if(TYPEC_Adapter_Watts<45 && TYPEC_Adapter_Watts>=36)
	{
		SET_MASK(SYS_STATUS,b5UnSuitAdpPow);
	    CLEAR_MASK(SYS_STATUS,b4IllegalAdp);
	}
	if(TYPEC_Adapter_Watts<36 && TYPEC_Adapter_Watts>=5)
	{
		CLEAR_MASK(SYS_STATUS,b5UnSuitAdpPow);
	    SET_MASK(SYS_STATUS,b4IllegalAdp);
	}
}
//72JERRY081: Dsable UCSI function.
#endif
