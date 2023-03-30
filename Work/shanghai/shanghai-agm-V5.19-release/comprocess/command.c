//command.c

#ifndef _COMMAND_C_
#define _COMMAND_C_
//start of file

UDWORD dwgCmdExeTime;
UBYTE bgDeviceStatus;

/*=====================================================================================
����:jt_cmd_process
����:
=======================================================================================*/
#ifdef _DEBUG_EN_
//#define _DEBUG_JT_CMD_
#endif
void jt_cmd_process(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes)
{
#ifndef _NO_EE_  
UWORD addr,bytes;
UBYTE ret;
#endif
UDWORD start_time;
//UDWORD ptr;

#ifdef _DEBUG_JT_CMD_
debug_printf("\x0d\x0a cmd bytes=[%d]\x0d\x0a",inbytes);
for(ret=0;ret<inbytes;ret++){
	debug_printf("%02X ",(UBYTE)inbuf[ret]);
	}
#endif

start_time = get_sys_time_cnt();

//mifare��Ƭ�����鴦��	
#ifdef CMD_MIF_START_CMD
if((inbuf[0] >= CMD_MIF_START_CMD) && (inbuf[0] <= CMD_MIF_END_CMD)){
	 cmd_mifare_process(inbuf,inbytes,outbuf,outbytes);
	 goto label_exit;
	 }
#endif

//flash�����
#ifndef _NO_FLASH_
if( (((UBYTE)inbuf[0]>=CMD_RD_FLASH)&&((UBYTE)inbuf[0]<=CMD_ERASE_FLASH)) ||
	  (((UBYTE)inbuf[0]>=CMD_WR_FLASH_BUF)&&((UBYTE)inbuf[0]<=CMD_READ_FLASHBUF))){
     cmd_flash_process(inbuf,inbytes,outbuf,outbytes);
     goto label_exit;
     }
#endif

switch((UBYTE)inbuf[0]){
case CMD_REPORT_VER:
     if(inbytes != 1){
     	  if(inbuf[1] != 0) goto label_err;
     	  #ifdef CMD_ISO15693_SET
     	  if(inbytes < 3) goto label_err;
         cmd_iso15693_process(&inbuf[2],inbytes-2,outbuf,outbytes);
	       goto label_exit;     	  		
     	  #endif  	
         }
     strcpy((char *)outbuf,"HHJTMETRO2010_20100130");
	   *outbytes = strlen((char *)outbuf);
	   goto label_exit;
#ifdef CMD_RESET
case CMD_RESET:
	   if(inbytes != 1) goto label_err;
	   timer_set(TIMER_RESET,1000);
	   bgHaltFlag	=1;
	   goto label_ok;	   
#endif	   	
case CMD_SET_STATUS:
	   if(inbytes != 2) goto label_err;
	   bgDeviceStatus = inbuf[1];
           goto label_ok;		   
#ifdef CMD_REC_GET_BACKWARD	   
case CMD_REC_GET_BACKWARD:	
	   switch(inbytes){
	     case 3:
	   	   buf_to_uword(&inbuf[1],addr);
	   	   ptr = addr;
	   	   break;
	   	 case 5:
	   	   buf_to_udword(&inbuf[1],ptr);
	   	   break;
	   	 default:
	   	 	 goto label_err;  
	   	   }	  	
	   ret = rec_rd_backward(ptr,outbuf);
	   if(ret != 0){
	   	   outbuf[0] = 0x01;
	   	   outbuf[1] = ret;
	   	   *outbytes = 2;
	   	   goto label_exit;
	   	   }
	   *outbytes = 128;
	   goto label_exit;	   
#endif	   
#ifdef CMD_REC_GET_FORWARD	   
case CMD_REC_GET_FORWARD:	
	   switch(inbytes){
	     case 3:
	   	   buf_to_uword(&inbuf[1],addr);
	   	   ptr = addr;
	   	   break;
	   	 case 5:
	   	   buf_to_udword(&inbuf[1],ptr);
	   	   break;
	   	 default:
	   	 	 goto label_err;  
	   	   }	  	
	   ret = rec_rd_forward(ptr,outbuf);
	   if(ret != 0) goto label_err_ret;
	   *outbytes = 128;
	   goto label_exit;	   
#endif	   
#ifdef CMD_REC_CLR_N
case CMD_REC_CLR_N:
	   switch(inbytes){
	     case 3:
	   	   buf_to_uword(&inbuf[1],addr);
	   	   ptr = addr;
	   	   break;
	   	 case 5:
	   	   buf_to_udword(&inbuf[1],ptr);
	   	   break;
	   	 default:
	   	 	 goto label_err;  
	   	   }	  	
	   ret = rec_clr_n(ptr);
	   if(ret != 0) goto label_err_ret;
	   goto label_ok;
#endif
#ifdef CMD_REC_GET_NUM
case CMD_REC_GET_NUM:
     if(inbytes == 1){
        ptr = rec_get_num();	
        outbuf[0] = 0;
        udword_to_buf(ptr,&outbuf[1]);
        *outbytes = 5;
        goto label_exit;		
        }
     if(inbytes == 2){
        ptr = rec_get_freenum();	
        outbuf[0] = 0;
        udword_to_buf(ptr,&outbuf[1]);
        *outbytes = 5;
        goto label_exit;		
        }
     goto label_err;   
#endif
#ifdef CMD_REC_SAVE
case CMD_REC_SAVE:
	   if(inbytes == 13){
	   	  rec_all_set();
	   	  ret = rec_init_ptr(&inbuf[1],&inbuf[9]);
	   	  if(ret) goto label_err_ret;	   	  	  
	   	  rec_init();
	   	  goto label_ok;
	   	  }
	   if((UBYTE)inbytes == 129){
	   	  ret = rec_wr(&inbuf[1],1);
	   	  if(ret) goto label_err_ret;
	   	  goto label_ok;	
	   	  }
	   goto label_err;	  
#endif
//----------------------------------------------------	
#ifdef CMD_RD_EE
case CMD_RD_EE:
     if(inbytes != 4) goto label_err;
     *((char *)&addr+INT_HIGH)   = inbuf[1];
     *((char *)&addr+INT_LOW) = inbuf[2];
     bytes = inbuf[3];
     if(addr >= MAX_EEPROM_SIZE) goto label_err;
     if((addr+bytes) > MAX_EEPROM_SIZE) goto label_err;
     ret = ee_read(addr,bytes,&outbuf[1]);
     if(ret) goto label_err;
     outbuf[0] = 0;
     *outbytes = bytes+1;
     goto label_exit;
#endif 
//---------------------------------------------------
#ifdef CMD_WR_EE
case CMD_WR_EE:
     if(inbytes < 4) goto label_err;
     bytes = inbytes-3;
     *((char *)&addr+INT_HIGH) = inbuf[1];
     *((char *)&addr+INT_LOW) = inbuf[2];
     if(addr >= MAX_EEPROM_SIZE) goto label_err;
     if((addr+bytes) > MAX_EEPROM_SIZE) goto label_err;
     ret = ee_write(addr,bytes,&inbuf[3]);
     if(ret != 0) goto label_err;
     goto label_ok;	   
#endif

//---------------------------------------------------
#ifdef CMD_DES_ENCODE
case CMD_DES_ENCODE:
     if(inbytes != 17) goto label_err;
     des_encode(&inbuf[1],&inbuf[9],outbuf);
     *outbytes = 8;
     return;
#endif
//---------------------------------------------------
#ifdef CMD_DES_DECODE
case CMD_DES_DECODE:
     if(inbytes != 17) goto label_err;
     des_decode(&inbuf[1],&inbuf[9],outbuf);
     *outbytes = 8;
     return;
#endif

//---------------------------------------------------
#ifdef CMD_COMPUTE_TAC 
#endif
//---------------------------------------------------
#ifdef CMD_WR_TIME
case CMD_WR_TIME: 
	   if(inbytes == 7){
	      if(rtc_wr_time(&inbuf[1]) != 0) goto label_err;
	      goto label_ok;
	      }	
	   if(inbytes == 8){
	   	  if(inbuf[1] != 0x20) goto label_err;
	      if(rtc_wr_time(&inbuf[2]) != 0) goto label_err;
	      goto label_ok;
	      }	
	   goto label_err;		
#endif
//---------------------------------------------------
#ifdef CMD_RD_TIME 
case CMD_RD_TIME:
	   if(inbytes != 1) goto label_err;
	   if(rtc_rd_time(outbuf) != 0) goto label_err;
	   *outbytes = 6;			  
	   return; 
#endif
//---------------------------------------------------
#ifdef CMD_KEY_CLASS
case CMD_KEY_CLASS:
	   if(inbytes != 1) goto label_err;
	   outbuf[0] = 0;
	   bytes = 1;
	   while(kb_check()){
	   	 outbuf[bytes++] = kb_getch();
	   	 }
	   *outbytes = bytes;
	   return;	 
#endif
//-----------------------------------------------------------------------------     	
#ifdef CMD_CPU_ATR           
case CMD_CPU_ATR:
if(inbytes == 1) goto label_atr;
if(inbytes == 2){                //ѡ��SAM
   if((UBYTE)inbuf[1] >= MAX_SAM_INDEX) goto label_err;
   sam_select(inbuf[1]);
   goto label_ok;	
   }	 	
if(inbytes == 3){                //����SAM������
   if((UBYTE)inbuf[1] >= MAX_SAM_INDEX) goto label_err;
   sam_set_speed(inbuf[1],inbuf[2]);
   goto label_ok;	
   }	 	
goto label_exit;

label_atr:
if(sam_atr(outbuf,outbytes) != 0) goto label_err;
goto label_exit;
#endif
//---------------------------------------------------------------------------------------
#ifdef CMD_CPU_T0
case CMD_CPU_T0:
if(inbytes < 6) goto label_err;
ret = sam_apdu(&inbuf[1],inbytes-1,outbuf,outbytes);
if(ret != 0) goto label_err;
goto label_exit;
#endif
/*----------------------------------------------------
------------------------------------------------------*/
#ifdef  CMD_NJAFC_CLASS
case (UBYTE)CMD_NJAFC_CLASS:
	 if(inbytes < 2) goto label_err;
   cmd_njafc_process(&inbuf[1],inbytes-1,outbuf,outbytes);
	 goto label_exit;
#endif	


/*----------------------------------------------------
------------------------------------------------------*/
#ifdef  CMD_MIFPRO_CLASS
case (UBYTE)CMD_MIFPRO_CLASS:
	 if(inbytes < 2) goto label_err;
   cmd_mifpro_process(&inbuf[1],inbytes-1,outbuf,outbytes);
	 goto label_exit;
#endif	

/*----------------------------------------------------
------------------------------------------------------*/
#ifdef  VOICE_CLASS
case (UBYTE)VOICE_CLASS:
	 if(inbytes < 3) goto label_err;
	 if(inbuf[1] != 0x03) goto label_err; 
	 voice_play_list(&inbuf[2],inbytes-2);
   goto label_ok; 	
#endif

/*----------------------------------------------------
------------------------------------------------------*/
#ifdef CMD_PSD_DL_CLASS
case (UBYTE)CMD_PSD_DL_CLASS:
   psd_dl_proc(inbuf,inbytes,outbuf,outbytes);
   goto label_exit;
#endif

/*----------------------------------------------------
------------------------------------------------------*/
#ifdef DEBUG_CLASS
case (UBYTE)DEBUG_CLASS:
	    if((UBYTE)inbuf[1] == (UBYTE)0xff){
	    	if(inbytes < 3) goto label_err;
	    	cmd_debug_process(&inbuf[2],inbytes-2,outbuf,outbytes);
	    	goto label_exit;	
	    	}
	  goto label_err;  
#endif

/*----------------------------------------------------
------------------------------------------------------*/
#ifdef SHIP_CLASS
case (UBYTE)SHIP_CLASS:
   cmd_ship_process(&inbuf[1],inbytes-1,outbuf,outbytes);
   goto label_exit;
#endif

/*----------------------------------------------------
------------------------------------------------------*/
#ifdef DISP_CLASS
case (UBYTE)DISP_CLASS:
	   if(inbytes == 1) goto label_err;
	   if(inbuf[1] != 0x10) goto label_err;
 	      while(bgLcdIntState==LCD_INT_CMD_IN){ 
  	      	delay_tick();
  	      	}  	      
  	      memcpy((char *)bpgLcdIntBuf,(char *)inbuf,inbytes);
  	      bgLcdIntBytes = inbytes;
  	      wgLcdIntTimeOut = 3000;
  	      bgLcdIntState=LCD_INT_CMD_IN;
  	      while(bgLcdIntState!=LCD_INT_PROCESSED){delay_tick();}
  	      if(bgLcdOutResp != 0){
  	      	  outbuf[0] = 0x80;
  	      	  outbuf[1] = bgLcdOutResp;
  	      	  *outbytes = 2;
  	      	  return;
  	      	  }
  	      memcpy((char *)outbuf,(char *)bpgLcdOutBuf,bgLcdOutBytes);
  	      *outbytes = bgLcdOutBytes;
  	      return;	   			   
#endif


/*----------------------------------------------------
------------------------------------------------------*/
default:
	  jt_append_cmd_process(inbuf,inbytes,outbuf,outbytes);
    return;
}

//label_err_ret:
//outbuf[0] = 0x01;
//outbuf[1] = ret;
//*outbytes = 2;
//goto label_exit;	

label_err:
outbuf[0] = 0xff;
*outbytes = 1;
goto label_exit;

label_ok:
outbuf[0] = 0x00;
*outbytes = 1;
goto label_exit;

label_exit:
if(memcmp((char *)inbuf,(char *)"\xff\xff\x01",3) != 0){
  dwgCmdExeTime = get_sys_time_cnt()-start_time;
  }
#ifdef _DEBUG_JT_CMD_
debug_printf("\x0d\x0a resp bytes[%d]\x0d\x0a",*outbytes);
for(ret=0;ret<*outbytes;ret++){
	debug_printf("%02X ",(UBYTE)outbuf[ret]);
	}
#endif
  
return;
}

/*=====================================================================================
����:cmd_debug_process
����:
=======================================================================================*/
void cmd_debug_process(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes)
{
UDWORD tudword,tudword2;
UWORD tuword;
UBYTE i;
//UBYTE ret;
//UBYTE tempbuf[512];
	
switch((UBYTE)inbuf[0]){	
case DEBUG_GET_SYSTIME:
	   if(inbytes == 1){
	      outbuf[0] = 0;
	      tudword = get_sys_time_cnt();	   
	      outbuf[1] = *((char *)&tudword+LONG_HIGH3);
	      outbuf[2] = *((char *)&tudword+LONG_HIGH2);
	      outbuf[3] = *((char *)&tudword+LONG_HIGH1);
	      outbuf[4] = *((char *)&tudword+LONG_HIGH0);
	      *outbytes = 5;
	      return;
	      }
	   if(inbytes == 2){
	      outbuf[0] = 0;
	      tudword = dwgSystemTimeCnt;	   
	      outbuf[1] = *((char *)&tudword+LONG_HIGH3);
	      outbuf[2] = *((char *)&tudword+LONG_HIGH2);
	      outbuf[3] = *((char *)&tudword+LONG_HIGH1);
	      outbuf[4] = *((char *)&tudword+LONG_HIGH0);
	      *outbytes = 5;
	      return;
	      }
	   goto label_err;
case DEBUG_GET_LAST_EXEC_TIME:
	   if(inbytes != 1) goto label_err;
	   outbuf[0] = 0;
	   outbuf[1] = *((char *)&dwgCmdExeTime+LONG_HIGH3);
	   outbuf[2] = *((char *)&dwgCmdExeTime+LONG_HIGH2);
	   outbuf[3] = *((char *)&dwgCmdExeTime+LONG_HIGH1);
	   outbuf[4] = *((char *)&dwgCmdExeTime+LONG_HIGH0);
	   *outbytes = 5;
	   return;
case DEBUG_DELAY:
	   if(inbytes != 3) goto label_err;
	   *((char *)&tuword+INT_HIGH) = inbuf[1];
	   *((char *)&tuword+INT_LOW) = inbuf[2];
//	   delay_ms(tuword);
	   goto label_ok;	
#ifdef 	DEBUG_RC_RD_BYTE   
case DEBUG_RC_RD_BYTE:
	   if(inbytes != 2) goto label_rc_rd_all;
	   outbuf[1] = rc_read_byte(inbuf[1]);
	   outbuf[0] = 0;
	   *outbytes = 2;
	   return;	
	   
	   label_rc_rd_all:
	   if(inbytes != 1) goto label_err;
	   for(i=0;i<64;i++) outbuf[i+1] = rc_read_byte(i);
	   outbuf[0] = 0;
	   *outbytes = 65;	
	   return;
#endif
#ifdef 	DEBUG_RC_WR_BYTE   
case DEBUG_RC_WR_BYTE:
	   if(inbytes != 3) goto label_err;
	   rc_write_byte(inbuf[1],inbuf[2]);
	   goto label_ok;
#endif           
case DEBUG_RD_PORT:
	   if(inbytes != 5) goto label_err;
	   *((char *)&tudword+LONG_HIGH3) = inbuf[1];
	   *((char *)&tudword+LONG_HIGH2) = inbuf[2];
	   *((char *)&tudword+LONG_HIGH1) = inbuf[3];
	   *((char *)&tudword+LONG_HIGH0) = inbuf[4];
	   tudword2 = (u32)*(vu32 *)tudword;
	   outbuf[0] = 0;
	   outbuf[1] = *((char *)&tudword2+LONG_HIGH3);
	   outbuf[2] = *((char *)&tudword2+LONG_HIGH2);
	   outbuf[3] = *((char *)&tudword2+LONG_HIGH1);
	   outbuf[4] = *((char *)&tudword2+LONG_HIGH0);
	   *outbytes = 5;
	   return;	
case DEBUG_WR_PORT:
	   if(inbytes != 9) goto label_err;
	   *((char *)&tudword+LONG_HIGH3) = inbuf[1];
	   *((char *)&tudword+LONG_HIGH2) = inbuf[2];
	   *((char *)&tudword+LONG_HIGH1) = inbuf[3];
	   *((char *)&tudword+LONG_HIGH0) = inbuf[4];	   
	   *((char *)&tudword2+LONG_HIGH3) = inbuf[5];
	   *((char *)&tudword2+LONG_HIGH2) = inbuf[6];
	   *((char *)&tudword2+LONG_HIGH1) = inbuf[7];
	   *((char *)&tudword2+LONG_HIGH0) = inbuf[8];
	   *(vu32 *)tudword = tudword2;
	   goto label_ok;
case DEBUG_CHG_BAUD:
	   if(inbytes != 6) goto label_err;
	   *((char *)&tudword+LONG_HIGH3) = inbuf[2];
	   *((char *)&tudword+LONG_HIGH2) = inbuf[3];
	   *((char *)&tudword+LONG_HIGH1) = inbuf[4];
	   *((char *)&tudword+LONG_HIGH0) = inbuf[5];	   
	   uart_change_baud(inbuf[1],tudword);
	   goto label_ok;
#ifdef DEBUG_SAM           
case DEBUG_SAM:
	   if(inbytes < 2) goto label_err;
	   cmd_debug_sam(&inbuf[1],inbytes-1,outbuf,outbytes); 	   
	   return;
#endif           
#ifdef DEBUG_TEMP
case DEBUG_TEMP:
	   if(inbytes != 2) goto label_err;
     outbuf[0] = 0;
     outbuf[1] = 0;
     *outbytes = 2;
     return;
#endif	   
#ifdef DEBUG_LCD_CLASS
case DEBUG_LCD_CLASS:
	   if(inbytes < 2) goto label_err;
	   cmd_lcd_process(&inbuf[1],inbytes-1,outbuf,outbytes);
	   return;
#endif	
#ifdef DEBUG_ADC
case DEBUG_ADC:
	   if(inbytes != 2) goto label_err;
	   tuword = adc_get(inbuf[1]);
	   outbuf[0] = 0;
	   uword_to_buf(tuword,&outbuf[1]);	
	   *outbytes = 3;
	   return;
#endif	
#ifdef DEBUG_PRN_CLASS
case DEBUG_PRN_CLASS:
      if(inbytes < 2) goto label_err;
      cmd_debug_prn(&inbuf[1],inbytes-1,outbuf,outbytes);
      return;
#endif	   
#ifdef DEBUG_GPRS_POWER      
case DEBUG_GPRS_POWER:
	   if(inbytes != 2) goto label_err;
	   if(inbuf[1]){ gprs_pwr_on();}
	   else{ gprs_pwr_off();}		
	   goto label_ok;
#endif
#ifdef DEBUG_GPRS_PUT           
case DEBUG_GPRS_PUT:
     if(inbytes <=1 ) goto label_err;
     //ret = prot_lcd_tranceve(&inbuf[1],inbytes-1,&outbuf[1],outbytes,5000);
     //if(ret) goto label_err_ret;
     //outbuf[0]=0;
     //*outbytes = *outbytes+1;	
     //return;
     goto label_ok;
#endif     
default:	   
	   break;
  }
goto label_err;
//
//label_err_ret:
//outbuf[0] = 0x01;	
//outbuf[0] = ret;
//*outbytes = 2;
//return;
//
label_err:
outbuf[0] = 0xff;
*outbytes = 1;
return;
//
label_ok:
outbuf[0] = 0x00;
*outbytes = 1;
return;
}


/*=====================================================================================
����:cmd_mifare_process
����:
=======================================================================================*/
#ifdef CMD_MIF_START_CMD
void cmd_mifare_process(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes)
{
UWORD i,bytes;
UDWORD value;
UBYTE ret;
	
switch((UBYTE)inbuf[0]){
//---------------------------------------------
#ifdef CMD_REQUEST
case CMD_REQUEST:
if(inbytes != 3) goto label_err;
*((char *)&bytes+INT_HIGH)  = inbuf[1];
*((char *)&bytes+INT_LOW)   = inbuf[2];
for(i=0;i<bytes;i++){
  if(mcml_request(PICC_REQSTD,outbuf)==0){
        *outbytes = 2;
        return;
        }
  }
goto label_ok;
#endif	
//-----------------------------------------------
//����ײ
//-----------------------------------------------
#ifdef CMD_ANTICOLL
case CMD_ANTICOLL:
switch(inbytes){
	case 1:
		  ret = mcml_anticoll(outbuf);
		  break;
  case 2:
  	  if(inbuf[1] == 0){
  	  	 ret = mcml_anticoll2(outbuf);  	  	  
	       break;
	       }
	     goto label_err;
	
	default:
		  goto label_err;
	}    
if(ret == 0){
    *outbytes = 5;
    return;
    }
goto label_err;
#endif
//-----------------------------------------------------
//ѡ��
//------------------------------------------------------
#ifdef CMD_SELECT
case CMD_SELECT:
if(inbytes == 7){
   ret = mcml_select2(&inbuf[1],&outbuf[1]);
   if(ret != 0){
     outbuf[0] = 0x02;
     outbuf[1] = ret;
     *outbytes = 2;
     return;
     }
   outbuf[0] = 0;
   *outbytes = 2;
   return;
   }	
if((inbytes != 6) && (inbytes != 5)) goto label_err;  
if(inbytes == 5) inbuf[5] = inbuf[1]^inbuf[2]^inbuf[3]^inbuf[4];	
ret = mcml_select(&inbuf[1],&outbuf[1]);
if(ret != 0){
	 outbuf[0] = 0x01;
	 outbuf[1] = ret;
	 *outbytes = 2;
	 return;
	 }
outbuf[0] = 0;
*outbytes = 2;
return;
#endif
//------------------------------------------------------
/*---------------------------------------------------
������Կ
-----------------------------------------------------*/
#ifdef CMD_LOAD_KEY
case CMD_LOAD_KEY:
if(inbytes != 10) goto label_err;
if(inbuf[2] == 0)  inbuf[2] = KEYA;
else   inbuf[2] = KEYB;
ret = mcml_load_key(inbuf[1],inbuf[2],inbuf[3],&inbuf[4]);
if(ret != 0) goto label_err;
goto label_ok;
#endif
/*----------------------------------------------------
����Ƚ�
-----------------------------------------------------*/
#ifdef CMD_AUTHENTICATION
case CMD_AUTHENTICATION:
if(inbytes == 4){
   if(inbuf[2] == 0)  inbuf[2] = KEYA;
   else inbuf[2] = KEYB;
   ret = mcml_authentication(inbuf[1],inbuf[2],inbuf[3]);
   if(ret != 0){
	   outbuf[0] = ret;
	   *outbytes = 1;
	   return;
	   }
   goto label_ok;   
   }
if(inbytes == 2){
   ret = mcml_authentication2(0,0,0);
   if(ret != 0){
	   outbuf[0] = ret;
	   *outbytes = 1;
	   return;
	   }
   goto label_ok;   
   }
goto label_err;   
#endif
/*--------------------------------------------------------
����
---------------------------------------------------------*/
#ifdef CMD_RD_BLOCK
case CMD_RD_BLOCK:
if(inbytes == 2){
   ret = mcml_read(inbuf[1],outbuf);
   if(ret != 0) goto label_err;
   *outbytes = 16;
   return;
   }
if(inbytes == 3){
   ret = mcml_read_4bytes(inbuf[1],outbuf);
   if(ret != 0) goto label_err;
   *outbytes = 4;
   return;
   }
goto label_err;   
#endif

/*---------------------------------------------------------
д��
----------------------------------------------------------*/
#ifdef CMD_WR_BLOCK
case CMD_WR_BLOCK:
if(inbytes == 18){
   ret = mcml_write(inbuf[1],&inbuf[2]);
   if(ret != 0) goto label_err;
   goto label_ok;
   }
if(inbytes == 6){
   ret = mcml_write_4bytes(inbuf[1],&inbuf[2]);
   if(ret != 0) goto label_err;
   goto label_ok;	
   }
goto label_err;
#endif

/*---------------------------------------------------------
ֹͣ��
----------------------------------------------------------*/
#ifdef CMD_HALT
case CMD_HALT:
ret = mcml_halt();
if(ret != 0) goto label_err;
goto label_ok;
#endif

/*---------------------------------------------------------
increment
----------------------------------------------------------*/
#ifdef CMD_INCREASE
case CMD_INCREASE:
if(inbytes != 6) goto label_err;
*((char *)&value+LONG_HIGH3) = inbuf[2];
*((char *)&value+LONG_HIGH2) = inbuf[3];
*((char *)&value+LONG_HIGH1) = inbuf[4];
*((char *)&value+LONG_HIGH0) = inbuf[5];
ret = mcml_increment(inbuf[1],value);
if(ret != 0) goto label_err;
goto label_ok;
#endif

/*-----------------------------------------------------------
decrement
------------------------------------------------------------*/
#ifdef CMD_DECREASE
case CMD_DECREASE:
if(inbytes != 6) goto label_err;
*((char *)&value+LONG_HIGH3) = inbuf[2];
*((char *)&value+LONG_HIGH2) = inbuf[3];
*((char *)&value+LONG_HIGH1) = inbuf[4];
*((char *)&value+LONG_HIGH0) = inbuf[5];
ret = mcml_decrement(inbuf[1],value);
if(ret != 0) goto label_err;
goto label_ok;
#endif

/*---------------------------------------------------------
MCM POWER OFF
----------------------------------------------------------*/
#ifdef CMD_PWR_OFF
case CMD_PWR_OFF:
//if(inbytes == 2){
//	rf_set(inbuf[1]);
//	goto label_ok;
//	}	
if(inbytes != 1) goto label_err;	
mcml_pwr_off();
goto label_ok;
#endif

/*-----------------------------------------------------------
restore
------------------------------------------------------------*/
#ifdef CMD_RESTORE
case CMD_RESTORE:
if(inbytes != 2) goto label_err;
ret = mcml_restore(inbuf[1]);
if(ret != 0) goto label_err;
goto label_ok;
#endif

/*-----------------------------------------------------------
transfer
-------------------------------------------------------------*/
#ifdef CMD_TRANSFER
case CMD_TRANSFER:
if(inbytes != 2) goto label_err;
ret = mcml_transfer(inbuf[1]);
if(ret != 0) goto label_err;
goto label_ok;
#endif
//------------------------------------------------------------
default:
	break;
  }	
//goto label_err;
//	
label_err:
outbuf[0] = 0xff;
*outbytes = 1;
return;
//
label_ok:
outbuf[0] = 0x00;
*outbytes = 1;
return;	
}	
#endif

/*=====================================================================================
����:cmd_mifare_process
����:
=======================================================================================*/
#ifdef CMD_NJAFC_CLASS
void cmd_njafc_process(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes)
{
UBYTE ret;
	
switch((UBYTE)inbuf[0]){
	#ifdef CMD_SET_CRYPT   
  case (UBYTE)CMD_SET_CRYPT:
  	    if(inbytes != 2) goto label_err;
  	    ret = rc_select_op_type(inbuf[1]);
  	    if(ret) goto label_err;
  	    goto label_ok;	
  #endif	    
  //
  #ifdef CMD_M400_INIT    
  case (UBYTE)CMD_M400_INIT:
        if(inbytes != 1) goto label_err;
        rc_iso14443_typeb_init();
        goto label_ok;
  #endif
  //
  #ifdef CMD_M400_REQ    
  case (UBYTE)CMD_M400_REQ:
        if(inbytes != 1) goto label_err;
        m4_request();
        goto label_ok;
  #endif
  //
  #ifdef CMD_M400_CMD   
  case (UBYTE)CMD_M400_CMD:
        if((UBYTE)inbytes <= 1) goto label_err;
        inbytes -= 1;
        ret = m4_cmd(inbytes,&inbuf[1],outbytes,&outbuf[1]);
        if(ret != 0){
           outbuf[0] = 0x01;
           outbuf[1] = ret;
           *outbytes = 2;
           goto label_send;
           }
        outbytes[0] = outbytes[0]+1;
        outbuf[0] = 0;
        goto label_send;
   #endif
   //
   #ifdef CMD_M400_POLL            
   case (UBYTE)CMD_M400_POLL:
        if((UBYTE)inbytes != 1) goto label_err;
        ret = m4_poll(outbytes,&outbuf[1]);   
        if(ret != 0){
           outbuf[0] = 0x01;
           outbuf[1] = ret;
           *outbytes = 2;
           goto label_send;	
           }
        outbytes[0] = outbytes[0]+1;
        outbuf[0] = 0;
        goto label_send;	
   #endif      	    
   #ifdef CMD_DISP_STRING
   case CMD_DISP_STRING:
         if(inbytes <= 5) goto label_err;
         inbuf[inbytes] = 0;
         lcd_disp_string(inbuf[1],inbuf[2],inbuf[3],&inbuf[4]);
         goto label_ok;
    #endif
    #ifdef CMD_DISP_STRING_ENLARGE
    case CMD_DISP_STRING_ENLARGE:
         if(inbytes <= 5) goto label_err;
         inbuf[inbytes] = 0;
         lcd_disp_string_enlarge(inbuf[1],inbuf[2],inbuf[3],&inbuf[4]);
         goto label_ok;
    #endif
    #ifdef CMD_DISP_IMAGE
    case CMD_DISP_IMAGE:
         if(inbytes != 2) goto label_err;
         lcd_disp_image(inbuf[1]);
         goto label_ok;
    #endif
    #ifdef CMD_SET_HZ_MODE
    case CMD_SET_HZ_MODE:
        if(inbytes != 2) goto label_err;
        lcd_set_hzmode(inbuf[1]);
        goto label_ok;
    #endif
    #ifdef CMD_LCD_CLR
    case CMD_LCD_CLR:
        if(inbytes == 2){
           lcd_black();
           goto label_ok;
           }
        if(inbytes != 1) goto label_err;
        lcd_clr();
        goto label_ok;
    #endif   
   //
   default:
  	   break;	    
   }
goto label_err;

//	
label_ok:
outbuf[0] = 0x00;
*outbytes = 1;
return;
//  
label_err:
outbuf[0] = 0xff;
*outbytes = 1;
return;
//
//label_send:
//return;  
}
#endif

/*=====================================================================================
����:
����:
=======================================================================================*/
#ifdef CMD_ISO15693_SET
void cmd_iso15693_process(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes)
{
UBYTE time_out,ret;
UWORD tword;
	
switch((UBYTE)inbuf[0]){
  case CMD_ISO15693_SET:
  	 if(inbytes != 2) goto label_err;
  	 rc_init_15693_std_mode();
  	 goto label_ok;			
  case CMD_ISO15693_CMD:
  	 if(inbytes < 5) goto label_err;
  	 tword = inbuf[1];
  	 tword = tword*1000/151;
  	 time_out = (UBYTE)tword;
  	 ret = rc_15693_cmd(&inbuf[2],inbytes-2,&outbuf[1],outbytes,time_out);
  	 if(ret != 0){
  	 	  outbuf[0] = 0x01;
  	 	  outbuf[1] = ret;
  	 	  *outbytes = 2;
  	 	  return;
  	 	  }
  	 outbuf[0] = 0;
  	 *outbytes = *outbytes+1;
  	 return;  	 	
  default:
  	 break;
  }
goto label_err;

//	
label_ok:
outbuf[0] = 0x00;
*outbytes = 1;
return;
//
label_err:
outbuf[0] = 0xff;
*outbytes = 1;
return;
}
#endif

/*=====================================================================================
����:
����:
=======================================================================================*/
#ifndef _NO_FLASH_
void cmd_flash_process(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes)
{
UBYTE ret;
UWORD page;
UDWORD long_page;
UWORD addr,bytes;
	
switch((UBYTE)inbuf[0]){
	case CMD_RD_FLASH:
       if(inbytes == 6){
       	   buf_to_uword(&inbuf[1],page);
       	   buf_to_uword(&inbuf[3],addr);          
           bytes = inbuf[5];
           long_page = page+0x10000;
           ret = flash_read(long_page,addr,bytes,&outbuf[1]);
           if(ret) goto label_err;
           outbuf[0] = 0;
           *outbytes = bytes+1;
           return;
           }
       if(inbytes == 8){
       	   buf_to_udword(&inbuf[1],long_page);
       	   buf_to_uword(&inbuf[5],addr);          
           bytes = inbuf[7];
           ret = flash_read(long_page,addr,bytes,&outbuf[1]);
           if(ret) goto label_err;
           outbuf[0] = 0;
           *outbytes = bytes+1;
           return;
           }
       goto label_err;
	case CMD_WR_FLASH:
		   if(inbytes < 6) goto label_err;
       bytes = inbytes-5;
       *((char *)&page+INT_HIGH) = inbuf[1];
       *((char *)&page+INT_LOW)  = inbuf[2];
       *((char *)&addr+INT_HIGH) = inbuf[3];
       *((char *)&addr+INT_LOW)  = inbuf[4];
       //
       long_page = 0x10000+page;
       ret = flash_write(long_page,addr,bytes,&inbuf[5]);
       if(ret != 0) goto label_err;
       goto label_ok;
	case CMD_ERASE_FLASH:
		   if(inbytes == 3){
          buf_to_uword(&inbuf[1],page);
          long_page = 0x10000+page;
          ret = flash_erase_page(long_page);
          if(ret != 0) goto label_err;
          goto label_ok;
          }
		   if(inbytes == 5){
          buf_to_udword(&inbuf[1],long_page);
          ret = flash_erase_page(long_page);
          if(ret != 0) goto label_err;
          goto label_ok;
          }
       goto label_err;
	case CMD_WR_FLASH_BUF:
       if((UBYTE)inbytes < 4) goto label_err;
       *((char *)&addr+INT_HIGH) = inbuf[1];
       *((char *)&addr+INT_LOW)  = inbuf[2];
       bytes = inbytes-3;
       ret = flash_write_buf(addr,bytes,&inbuf[3]);
       if(ret) goto label_err;
       goto label_ok;
	case CMD_FLASH_BUF_TO_MAIN:
       if(inbytes == 3){
          buf_to_uword(&inbuf[1],page);
          long_page = 0x10000+page;
          ret = flash_buf_to_main(long_page);
          if(ret != 0) goto label_err;
          goto label_ok;
          }
       if(inbytes == 5){
          buf_to_udword(&inbuf[1],long_page);
          ret = flash_buf_to_main(long_page);
          if(ret != 0) goto label_err;
          goto label_ok;
          }
       goto label_err;
	case CMD_FLASH2BUF:
       if(inbytes == 3){
       	  buf_to_uword(&inbuf[1],page);
       	  long_page = page+0x10000;
          ret = flash_main_to_buf(long_page);
          if(ret != 0) goto label_err;
          goto label_ok;
          }
       if(inbytes == 5){
          buf_to_udword(&inbuf[1],long_page);
          ret = flash_main_to_buf(long_page);
          if(ret != 0) goto label_err;
          goto label_ok;
          }          
       goto label_err;
	case CMD_READ_FLASHBUF:
  	 	 if(inbytes != 4) goto label_err;
       *((char *)&addr+INT_HIGH) = inbuf[1];
       *((char *)&addr+INT_LOW) = inbuf[2];
       bytes = inbuf[3];
       ret = flash_read_buf(addr,bytes,&outbuf[1]);
       outbuf[0] = 0;
       *outbytes = bytes+1;
       return;
  default:
  	   break;
  }
goto label_err;

//	
label_ok:
outbuf[0] = 0x00;
*outbytes = 1;
return;
//
label_err:
outbuf[0] = 0xff;
*outbytes = 1;
return;
}
#endif

/*=====================================================================================
����:cmd_mifpro_process
����:
=======================================================================================*/
#ifdef CMD_MIFPRO_CLASS 
void cmd_mifpro_process(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes)
{
UBYTE len,ret;
//UBYTE ibuf[65];
UWORD tword;
UBYTE obytes;
	
switch((UBYTE)inbuf[0]){
case CMD_MIFPRO_ATS:
     if(inbytes != 2) goto label_err;
     bgCID = inbuf[1];
     ret = mifpro_ats(bgCID,outbuf,&obytes);
     if((char)ret == 0){
     	 *outbytes = obytes;
     	 goto label_send;
     	 }
     outbuf[0] = 0x01;
     outbuf[1] = ret;	 
     *outbytes = 2;
     goto label_send;
case CMD_MIFPRO_DESELECT:
     if(inbytes != 2) goto label_err;
     bgCID = inbuf[1];
     ret = mifpro_deselect(outbuf);
     if((char)ret > 0){
     	 *outbytes = ret;
     	 goto label_send;
     	 }
     goto label_err;	
case CMD_MIFPRO_ICMD_NOCHAIN:
     if(inbytes < 2) goto label_err;
     len = inbytes-1;
     ret = mifpro_icmd_nochain(len,&inbuf[1],&outbuf[1]);
     if((char)ret > 0){
     	 outbuf[0] = 0;
     	 *outbytes = ret+1;
     	 goto label_send;
     	 }
     outbuf[0] = 0xff;
     outbuf[1] = ret;
     *outbytes = 2;
     goto label_send;	
 case CMD_MIFPRO_ICMD_CHAIN:
     if(inbytes < 2) goto label_err;
     len = inbytes-1;
     ret = mifpro_icmd_chain(len,&inbuf[1],&outbuf[1]);
     if((char)ret > 0){          	
     	 outbuf[0] = 0;
     	 *outbytes = ret+1;
     	 goto label_send;
     	 }
     outbuf[0] = 0xff;
     outbuf[1] = ret;
     *outbytes = 2;
     goto label_send;	
case CMD_MIFPRO_WTX:
     if(inbytes != 1) goto label_err;
     ret = mifpro_wtx(outbuf);
     if((char)ret > 0){
     	  *outbytes = ret;
     	  goto label_send;
     	  }
     goto label_err;	
case CMD_MIFPRO_RBLOCK:
     if(inbytes != 1) goto label_err;
     ret = mifpro_ack(outbuf);
     if((char)ret > 0){
     	  *outbytes = ret;
     	  goto label_send;
     	  }
     goto label_err;	
case CMD_CRC_A:
     if(inbytes <= 1) goto label_err;
     inbytes -= 1;
     tword = rc_crc_a(&outbuf[1],inbytes);
     outbuf[0] = *((char *)&tword+INT_HIGH);
     outbuf[1] = *((char *)&tword+INT_LOW);
     *outbytes = 2;
     goto label_send;
case CMD_MIFPRO_ICMD:
     if(inbytes < 2) goto label_err;
     len = inbytes-1;
     ret = mifpro_icmd(&inbuf[1],len,&outbuf[1],&obytes);
     if((char)ret == 0){
     	 outbuf[0] = 0;
     	 *outbytes = obytes+1;
     	 goto label_send;
     	 }
     outbuf[0] = 0xff;
     outbuf[1] = ret;
     *outbytes = 2;
     goto label_send;	
case CMD_MIFPRO_NOACK:
     if(inbytes != 1) goto label_err;
     ret = mifpro_noack(outbuf);
     if((char)ret > 0){
     	  *outbytes = ret;
     	  goto label_send;
     	  }
     goto label_err;
default:
	break;
  }	
goto label_err;
//	
label_err:
outbuf[0] = 0xff;
*outbytes = 1;
return;
//
//label_ok:
//bpgBuf[0] = 0x00;
//bgBufBytes = 1;
//return;	

label_send:
return;
}	

#endif


/*===============================================================================================
����:cmd_debug_sam
����:
=================================================================================================*/
#ifdef DEBUG_SAM
void cmd_debug_sam(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes)
{
UBYTE tch;
	
if(inbytes == 0) goto label_bad_para;
//
sam_prot_rece_reset();
sam_prot_start_send(inbuf,inbytes);
//��ʱ�ȴ�1000ms
timer_set(TIMER_CMD_PROCESS_INDEX,1000);
while(!timer_check(TIMER_CMD_PROCESS_INDEX)){
  if(!sam_prot_rece_ready()){
  	 //if(bgEnUCOS) OSTimeDly(1);
  	 continue;	
  	 }
  outbuf[0] = 0;	 
  sam_prot_rece_get(&outbuf[1],&tch);
  sam_prot_rece_reset();
  *outbytes = tch+1;
  return;
  }	
goto label_over_time;	

label_bad_para:
outbuf[0] = 0x03;
*outbytes = 1;
return;

label_over_time:
outbuf[0] = 0x02;
*outbytes = 1;
return;	
}	
#endif

/*=====================================================================================
����:cmd_lcd_process
����:
=======================================================================================*/
#ifdef DEBUG_LCD_CLASS
void cmd_lcd_process(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes)
{
UBYTE x,y;
UBYTE mode;
	
switch((UBYTE)inbuf[0]){
	case CMD_LCD_CLR2:
		   if(inbytes != 1) goto label_err;
		   lcd_clr();
		   goto label_ok;	
	case CMD_LCD_BLACK:
		   if(inbytes != 1) goto label_err;
		   lcd_black();
		   goto label_ok;
	#ifdef CMD_LCD_DISP_1616	   
	case CMD_LCD_DISP_1616:
       if(inbytes != 35) goto label_err;
       x = inbuf[1];
       y = inbuf[2];
       inbuf[inbytes] = 0;
       lcd_disp_1616(x,y,&inbuf[3]);
       goto label_ok;
  #endif
	#ifdef CMD_LCD_DISP_1616_ENLARGE       
	case CMD_LCD_DISP_1616_ENLARGE:
       if(inbytes != 35) goto label_err;
       x = inbuf[1];
       y = inbuf[2];
       inbuf[inbytes] = 0;
       lcd_disp_1616_enlarge(x,y,&inbuf[3]);
       goto label_ok;		
  #endif
  #ifdef CMD_LCD_DISP_1612     
	case CMD_LCD_DISP_1612:
       if(inbytes != 27) goto label_err;
       x = inbuf[1];
       y = inbuf[2];
       inbuf[inbytes] = 0;
       lcd_disp_1612(x,y,&inbuf[3]);
       goto label_ok;		
  #endif
  #ifdef CMD_LCD_DISP_1612_ENLARGE     
	case CMD_LCD_DISP_1612_ENLARGE:
       if(inbytes != 27) goto label_err;
       x = inbuf[1];
       y = inbuf[2];
       inbuf[inbytes] = 0;
       lcd_disp_1612_enlarge(x,y,&inbuf[3]);
       goto label_ok;		
  #endif
  #ifdef CMD_LCD_DISP_1608     
	case CMD_LCD_DISP_1608:
       if(inbytes != 19) goto label_err;
       x = inbuf[1];
       y = inbuf[2];
       inbuf[inbytes] = 0;
       lcd_disp_1608(x,y,&inbuf[3]);
       goto label_ok;
  #endif
  #ifdef CMD_LCD_DISP_1608_ENLARGE     
	case CMD_LCD_DISP_1608_ENLARGE:
       if(inbytes != 19) goto label_err;
       x = inbuf[1];
       y = inbuf[2];
       inbuf[inbytes] = 0;
       lcd_disp_1608_enlarge(x,y,&inbuf[3]);
       goto label_ok;
  #endif     
	case CMD_LCD_DISP_STRING:
       if(inbytes < 4) goto label_err;
       mode = inbuf[1];	
       x = inbuf[2];
       y = inbuf[3];
       inbuf[inbytes] = 0;
       lcd_disp_string(mode,x,y,&inbuf[4]);
       goto label_ok;
	case CMD_LCD_DISP_STRING_ENLARGE:
       if(inbytes < 4) goto label_err;
       mode = inbuf[1];	
       x = inbuf[2];
       y = inbuf[3];
       inbuf[inbytes] = 0;
       lcd_disp_string_enlarge(mode,x,y,&inbuf[4]);
       goto label_ok;
	case CMD_LCD_DISP_IMAGE:
       if(inbytes != 2) goto label_err;
       lcd_disp_image(inbuf[1]);
       goto label_ok;
	case CMD_LCD_BLANK_LINE:
       if(inbytes != 2) goto label_err;
       lcd_blank_line(inbuf[1]);
       goto label_ok;
  #ifdef CMD_LCD_WR_CMD
//  case CMD_LCD_WR_CMD:
//       if(inbytes < 2) goto label_err;
//       for(i=0;i<(inbytes-1);i++){	
//         oled_send_cmd(inbuf[1+i]);
//         }
//       goto label_ok;
  #endif
  #ifdef CMD_LCD_WR_DATA	   
  case CMD_LCD_WR_DATA:
       if(inbytes < 2) goto label_err;
       for(i=0;i<(inbytes-1);i++){	
         oled_send_data(inbuf[1+i]);
         }
       goto label_ok;
  #endif     
  default:
  	   break;
  }	       
goto label_err;

//	
label_ok:
outbuf[0] = 0x00;
*outbytes = 1;
return;
//
label_err:
outbuf[0] = 0xff;
*outbytes = 1;
return;	
}	
#endif

/*=====================================================================================
����:
����:
=======================================================================================*/
#ifdef DEBUG_PRN_CLASS  
void cmd_debug_prn(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes)
{
UBYTE ret;
UWORD tword;
	
switch((UBYTE)inbuf[0]){
  case CMD_PRN_TO_HOME:
       if(inbytes != 1) goto label_err;
       ret = prn_to_home();
       if(ret) goto label_err;
       goto label_report_state;
  case CMD_PRN_FEED:
       if(inbytes != 4) goto label_err;
       *((char *)&tword+INT_HIGH) = inbuf[2];
       *((char *)&tword+INT_LOW) = inbuf[3];
       ret = prn_feed(inbuf[1],tword);
       if(ret) goto label_err;
       goto label_report_state;
  case CMD_PRN_GET_STATE:
       if(inbytes != 1) goto label_err;
       prn_get_state();
       goto label_report_state;
  case CMD_PRN_STRING:
       if(inbytes < 4) goto label_err;
  	   buf_to_uword(&inbuf[1],tword);
  	   inbuf[inbytes] = 0;
  	   ret = prn_print_str(tword,&inbuf[3]);
  	   if(ret) goto label_err;
       goto label_report_state;
  case CMD_PRN_TEST:
  	   if(inbytes != 9) goto label_err;
       prn_print_test(&inbuf[1]);
       goto label_report_state;
	case CMD_PRN_SET_POS:
		   if(inbytes != 5) goto label_err;
		   bgPrnOffsetA = inbuf[1];
		   bgPrnOffsetB = inbuf[2];
		   bgBOffset = inbuf[3];
		   bgPrnEnlargeFlag = inbuf[4];
       prn_set_pos();
       goto label_report_state;
  default:
  	   break;
  }
goto label_err;

//	
//label_ok:
//outbuf[0] = 0x00;
//*outbytes = 1;
//return;
//
label_err:
outbuf[0] = 0xff;
*outbytes = 1;
return;

label_report_state:
outbuf[0] = 0;
outbuf[1] = bgPrnState;
*outbytes = 2;
return;	
}	
#endif



/*===============================================================================
������psd_dl_proc
���ܣ�PSD���س���
=================================================================================*/
#ifdef CMD_PSD_DL_CLASS
void psd_dl_proc(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes)
{
UBYTE ret;	
UWORD page,addr_in_page,bytes;
	
switch((UBYTE)inbuf[1]){
  #ifdef CMD_PSD256_ERASE     
  case (UBYTE)CMD_PSD256_ERASE:
       if(inbytes != 3) goto label_err;
       page = inbuf[2];
       ret = dl_erase_page(page);
       if(ret != 0) goto label_err;
       break; 
  #endif
  #ifdef CMD_PSD256_WRITE
  case (UBYTE)CMD_PSD256_WRITE:
       if((UBYTE)inbytes <= 5) goto label_err;
       page = inbuf[2];
       buf_to_uword(&inbuf[3],addr_in_page);      	
       bytes = inbytes-5;
       if((bytes%8) != 0) goto label_err;
       //dl_decode(&inbuf[5],bytes);
       ret = dl_write(page,addr_in_page,bytes,&inbuf[5]);
       if(ret != 0) goto label_err;
       goto label_ok;
  #endif
  #ifdef CMD_PSD256_READ     
  case (UBYTE)CMD_PSD256_READ: 
       if(inbytes != 7) goto label_err;
       page = inbuf[2];
       buf_to_uword(&inbuf[3],addr_in_page);
       buf_to_uword(&inbuf[5],bytes);
       if((bytes%8) != 0) goto label_err;       	
       dl_read(page,addr_in_page,bytes,&outbuf[1]);
       outbuf[0] = 0;
       *outbytes = bytes+1;	
       return;
  #endif
  #ifdef CMD_32_TO_256
  case (UBYTE)CMD_32_TO_256:
       if(inbytes != 2) goto label_err;
       bgJumpTimeCnt = DL_DEFAULT_START_TIME;
       goto label_ok;
  #endif
  #ifdef CMD_DL_IS_IN_32K
  case (UBYTE)CMD_DL_IS_IN_32K:
  	   if(inbytes != 2) goto label_err;
  	   outbuf[0] = 0;
  	   outbuf[1] = dl_is_in_32k();	
  	   *outbytes = 2;
  	   return;
  #endif
  #ifdef  CMD_DL_WR_FLAG
  case (UBYTE)CMD_DL_WR_FLAG: 
  	   if(inbytes == 4){
  	   	  dl_clr_prog_info();
  	   	  goto label_ok;
  	   	  }
  	   if(inbytes == 14){
  	   	  dl_wr_prog_info(&inbuf[2]);
  	   	  goto label_ok;
  	   	  }	  
  	   if(inbytes != 3) goto label_err;
  	   dl_wr_flag(inbuf[2]);
  	   goto label_ok;
  #endif
  default:
       goto label_err;
  }

label_ok:
outbuf[0] = 0x00;
*outbytes = 1;
return;

label_err:
outbuf[0] = 0xff;
*outbytes = 1;
return;
}	
#endif


//end of file
#endif

