//trans_245g.c

#ifndef _TRANS_245G_C_
#define _TRANS_245G_C_
//start of file

//
UBYTE bgCheck24GStep=0;


//#define _DEBUG_CHECK_245_BUF_PRINT_

/*=============================================================================================
函数：check_245g
功能：
===============================================================================================*/
#ifdef DEBUG_PRINT
//#define _DEBUG_CHECK_245_
#endif
void check_245g(void)
{
	UBYTE *rece_buf;
	UWORD i;
	UBYTE buf[257];
	
	#ifdef _DEBUG_CHECK_245_BUF_PRINT_
	UBYTE debug_buf[257];
	#endif
 
#ifdef DEBUG_PRINT
	debug_printf("\ncheck_245g start!\n");
#endif
  
//仅在寻卡状态才进入寻卡状态-----------  
	if((UBYTE)bgCheck24GStep == (UBYTE)0xff){
	  if(!is_zte_wait_process())
	  {
		   if(is_zte_send_timeout()) goto label_err;
		   return;
		}
		zte_processed();	
		bgCheck24GStep = 0;
	  gTaskBusyFlag = 0;//
	  return;
  }

	if(gGetCardInfoStep != 0) return;
	if(gTaskBusyFlag && (gTaskBusyFlag!=2)) return;//1--13.56M处理

	//寻卡过程处理--------------------------  
	rece_buf = gZteProt0.rece_buf;
	switch((UBYTE)bgCheck24GStep){
	case 0:	  //寻卡,1ms
		gDebugStep=0x1204;
	   //memcpy(buf,"\xa2\x31\x01\x2c",4);
	   memcpy(buf,"\xa2\x31\x00\x28",4); //\xa2\x31\x00\x00
     prot_zte_send_block(buf,4);
     //timer_set(gZteProt0.send_timer_index,20);
     prot_zte_rece_process(50);	//20
//     break;
//case 1:
//	   if(!is_zte_wait_process()){
//	   	  if(is_zte_send_timeout()) goto label_err;
//	   	  return;
//	   	  }
	   if(gZteProt0.send_state == ZTE_SEND_TIMEOUT)
	   {
	   		goto label_err;
	   }
	   gDebugStep=0x1205;
	   #ifdef _DEBUG_CHECK_245_BUF_PRINT_
	   uword_to_buf(gDebugStep,debug_buf);
	   debug_buf[2] = zte_rece_bytes;
	   memcpy(&debug_buf[3],rece_buf,zte_rece_bytes);
	   ReaderResponse(csc_comm,0x00,0xf0,debug_buf,zte_rece_bytes+3);
	   #endif
	   
	   	//2013/11/26 10:34:18 返回A001sim卡片未连接时 尝试断开连接
	   if(memcmp(rece_buf,"\xa0\x01",2) == 0)
	   	{
	   	  	memcpy(buf,"\xa2\x32\x00\x00",4);
          prot_zte_send_block(buf,4);
          prot_zte_rece_process(50);
          gDebugStep=0x1106;
          if(gZteProt0.send_state == ZTE_SEND_TIMEOUT)
          	goto label_err;
          //zte_set_wait(100);
          #ifdef _DEBUG_CHECK_245_
          	debug_printf("\x0d\x0a 断开连接");
          #endif     	
	        return;
	   } 
	   //
	   
	   
	   if(zte_rece_bytes < 3) goto label_err;  //2013/11/20 17:08:46
	   
	   if(memcmp(rece_buf,"\x0\x0",2) != 0){
	   	  if(memcmp(rece_buf,"\xa0\x01",2) == 0){
	   	  	memcpy(buf,"\xa2\x32\x00\x00",4);
          prot_zte_send_block(buf,4);
          prot_zte_rece_process(50);
          gDebugStep=0x1206;
          if(gZteProt0.send_state == ZTE_SEND_TIMEOUT)
          	goto label_err;
          //zte_set_wait(100);
          #ifdef _DEBUG_CHECK_245_
          debug_printf("\x0d\x0a 断开连接");
          #endif     	
	        return;
	   	  	} 
	   	  goto label_err;
	   }
	   gDebugStep=0x1207;
	   #ifdef _DEBUG_CHECK_245_
     debug_printf("\x0d\x0a OK:link,next sel adf");
     #endif     	
	   //寻卡成功，选择应用
	   memcpy(buf,"\xa2\x33\x00\xA4\x04\x00\x10\xD1\x56\x00\x01\x01\x80\x03\x80\x00\x00\x00\x01\x00\x00\x00\x00",23);
	   prot_zte_send_block(buf,23);
	   //
	   prot_zte_rece_process(40);	//40
//     break;
//case 2:
//	   if(!is_zte_wait_process()){
//	   	  if(is_zte_send_timeout()) goto label_err;
//	   	  return;
//	   	  }
	   if(gZteProt0.send_state == ZTE_SEND_TIMEOUT)
	   {
	   		goto label_err;
	   }
	   gDebugStep=0x1208;
	   #ifdef _DEBUG_CHECK_245_BUF_PRINT_
	   uword_to_buf(gDebugStep,debug_buf);
	   debug_buf[2] = zte_rece_bytes;
	   memcpy(&debug_buf[3],rece_buf,zte_rece_bytes);
	   ReaderResponse(csc_comm,0x00,0xf0,debug_buf,zte_rece_bytes+3);
	   #endif
     if(zte_rece_bytes < (0x1e + 3 + 2)){
     	  #ifdef _DEBUG_CHECK_245_
     	  debug_printf("\x0d\x0a Err:check_245g sel, sel adf ,rece bytes=%d",zte_rece_bytes);
     	  #endif     
     	  //goto label_card_invalid;
     	  goto label_err; //2013/11/20 11:34:59
     	  }
     gDebugStep=0x1209;
     if(memcmp(&rece_buf[zte_rece_bytes-2],"\x90\x00",2) != 0){
     	  #ifdef _DEBUG_CHECK_245_
     	  debug_printf("\x0d\x0a Err:check_245g sel, %02X %02X",(UBYTE)rece_buf[zte_rece_bytes-2],(UBYTE)rece_buf[zte_rece_bytes-1]);
     	  #endif     
     	  //goto label_card_invalid;
     	  goto label_err; //2013/11/20 11:34:55
     	  }
     //寻找\x9f\x0c\x1e
     gDebugStep=0x120a;
     for(i=0;i<zte_rece_bytes-5;i++){
     	 if(memcmp(&rece_buf[i],"\x9f\x0c\x1e",3) == 0) break;
     	 }
     gDebugStep=0x120b;
     if(i==(zte_rece_bytes-5)){
     	  #ifdef _DEBUG_CHECK_245_
     	  debug_printf("\x0d\x0a Err:check_245g ,sel adf,not found 9f 0c 1e");
     	  #endif
     	  goto label_card_invalid;     	
     	  }
     gDebugStep=0x120c;
     #ifdef _DEBUG_CHECK_245_
     debug_printf("\x0d\x0a OK:sel adf,next rd record");
     #endif     	
	   	
     //进入2.45G处理，此时不应再接受13.56M卡片处理
     gTaskBusyFlag = 2;
     
     //EF15
     i = i+3;
     memcpy(gAppSn,&rece_buf[i+10],10);
	   memcpy(gCardExpiredDate,&rece_buf[i+24],4);
	   gCardType = rece_buf[i+28];
	   LocationID = rece_buf[i+29];
	   //	 
	   //复合消费文件（0019）
	   memcpy(buf,"\xa2\x33\x00\xb2\x02\xcc\x40",7);//本地 
     if(g245_is_not_local()) buf[4] = 0x01;  //全国通用
	   prot_zte_send_block(buf,7);
	   
	   prot_zte_rece_process(80);	//200 100
//     break;
//case 3:
//	   if(!is_zte_wait_process()){
//	   	  if(is_zte_send_timeout()){
//	   	     #ifdef _DEBUG_CHECK_245_
//     	     debug_printf("\x0d\x0a Err:check 245g,send time out");
//     	     #endif     	  	 
//	   	  	 goto label_err;
//	   	  	 }
//	   	  return;
//	   	  }
	   if(gZteProt0.send_state == ZTE_SEND_TIMEOUT)
	   {
	   		goto label_err;
	   }
	   gDebugStep=0x120d;
	   #ifdef _DEBUG_CHECK_245_BUF_PRINT_
	   uword_to_buf(gDebugStep,debug_buf);
	   debug_buf[2] = zte_rece_bytes;
	   memcpy(&debug_buf[3],rece_buf,zte_rece_bytes);
	   ReaderResponse(csc_comm,0x00,0xf0,debug_buf,zte_rece_bytes+3);
	   #endif
     if(zte_rece_bytes != 68){
     	 #ifdef _DEBUG_CHECK_245_
     	 debug_printf("\x0d\x0a Err:check_245g rd record,bytes=%d",zte_rece_bytes);
     	 #endif
     	 goto label_card_invalid;
     	 }
     gDebugStep=0x120e;
     memmove(rece_buf,&rece_buf[2],66);	 
     memcpy(Record24G,rece_buf,64);
     if(g245_is_not_local()){
     	  	memcpy(gPosCityID,&rece_buf[22],2);
		      OtherTransFlag = rece_buf[24];
		      gMetroIoFlag = rece_buf[28];
		      memcpy(gTransTime,&rece_buf[15],7);
		      memcpy(EntryPosID,&rece_buf[29],4);
		      memcpy(ExitPosID,&rece_buf[33],4);
		      TotalPurse = 0;
		      *((char *)&TotalPurse+LONG_HIGH2) = rece_buf[37];
		      *((char *)&TotalPurse+LONG_HIGH1) = rece_buf[38];
		      *((char *)&TotalPurse+LONG_HIGH0) = rece_buf[39];
		      buf_to_uword(&rece_buf[40],TotalCnt);
		      LastTransValue = 0;
		      *((char *)&LastTransValue+LONG_HIGH1) = rece_buf[42];
		      *((char *)&LastTransValue+LONG_HIGH0) = rece_buf[43];
		      memcpy(gLastEntryTime,&rece_buf[44],7);
		      info[0] = rece_buf[53];
		      memcpy(&info[1],&rece_buf[54],4);
		      gTransTypeFlag = rece_buf[4];
     	    }	
     else{
     	    OtherTransFlag = 0; 
    	    gMetroIoFlag = rece_buf[3];
		      memcpy(gTransTime,&rece_buf[4],7);
		      memcpy(EntryPosID,&rece_buf[11],4);
		      memcpy(ExitPosID,&rece_buf[15],4);
		      TotalPurse = 0;
		      *((char *)&TotalPurse+LONG_HIGH2) = rece_buf[19];
		      *((char *)&TotalPurse+LONG_HIGH1) = rece_buf[20];
		      *((char *)&TotalPurse+LONG_HIGH0) = rece_buf[21];
		      buf_to_uword(&rece_buf[22],TotalCnt);
		      LastTransValue = 0;
		      *((char *)&LastTransValue+LONG_HIGH1) = rece_buf[24];
		      *((char *)&LastTransValue+LONG_HIGH0) = rece_buf[25];
		      memcpy(gLastEntryTime,&rece_buf[26],7);
		      info[0] = rece_buf[35];
		      memcpy(&info[1],&rece_buf[36],4);
		      
		      //20100315
		 	    memcpy(gPosCityID,"\x02\x10",2);//shanghai
		 	    gTransTypeFlag = 0x0;
		 	    }	
		 	#ifdef _DEBUG_CHECK_245_
      debug_printf("\x0d\x0a OK:now to get banlance");
      #endif     	
	    
     	//取余额
     	memcpy(buf,"\xa2\x33\x80\x5c\x00\xa0\x07",7);
	    prot_zte_send_block(buf,7);
	    
	    prot_zte_rece_process(50);	//50
//      break;
//case 4:
//	   if(!is_zte_wait_process()){
//	   	  if(is_zte_send_timeout()) goto label_err;
//	   	  return;
//	   	  }
     gDebugStep=0x120f;
     #ifdef _DEBUG_CHECK_245_BUF_PRINT_
	   	 uword_to_buf(gDebugStep,debug_buf);
		   memcpy(&debug_buf[2],rece_buf,zte_rece_bytes);
		   ReaderResponse(csc_comm,0x00,0xf0,debug_buf,zte_rece_bytes+2);
		 #endif
	   if(gZteProt0.send_state == ZTE_SEND_TIMEOUT)
	   {
	   		goto label_err;
	   }
	   gDebugStep=0x1210;
     if(zte_rece_bytes != 11){
     	 #ifdef _DEBUG_CHECK_245_
     	 debug_printf("\x0d\x0a Err:check_245g get remain,bytes=%d",zte_rece_bytes);
     	 #endif
     	  goto label_card_invalid;
     	  }
     gDebugStep=0x1211;
     memmove(rece_buf,&rece_buf[2],9);	    	  
     buf_to_udword(rece_buf,gCardPurse);
	   rece_buf[3] = 0;
	   buf_to_udword(&rece_buf[3],gPurseOffset);
	   gCardLastRemain = gCardPurse-gPurseOffset;
		 //读取成功
		 gStatus = TRANS_STATUS;
	   gGetCardInfoStep=0xff;
	   // 
	   buf[0] = MT_READCARD;
	   buf[1] = gReadCmdTimes;
	   buf[2] = CE_OK;
	   buf[3] = 0x01;
	   memcpy(&buf[4],gAppSn,10);
	   buf[14] = gCardType;
	   memcpy(&buf[15],gCardExpiredDate,4);
	   buf[19] = *((char *)&gCardLastRemain+LONG_HIGH0);
	   buf[20] = *((char *)&gCardLastRemain+LONG_HIGH1);
	   buf[21] = *((char *)&gCardLastRemain+LONG_HIGH2);
	   buf[22] = *((char *)&gCardLastRemain+LONG_HIGH3);	   
	   buf[23] = *((char *)&gPurseOffset+LONG_HIGH0);
	   buf[24] = *((char *)&gPurseOffset+LONG_HIGH1);
	   buf[25] = *((char *)&gPurseOffset+LONG_HIGH2);
	   shmt_prot_send(buf,26);
	   //	
	   buf[0] = MT_READCARD;
	   buf[1] = gReadCmdTimes;
	   buf[2] = CE_OK;
	   buf[3] = 0x82;
	   buf[4] = gMetroIoFlag;
	   memcpy(&buf[5],gLastEntryTime,7);
	   memcpy(&buf[12],gTransTime,7);
	   memcpy(&buf[19],EntryPosID,4);
	   memcpy(&buf[23],ExitPosID,4);
	   buf[27] = *((char *)&TotalPurse+LONG_HIGH0);
	   buf[28] = *((char *)&TotalPurse+LONG_HIGH1);
	   buf[29] = *((char *)&TotalPurse+LONG_HIGH2);
	   buf[30] = *((char *)&TotalCnt+INT_HIGH);
	   buf[31] = *((char *)&TotalCnt+INT_LOW);
	   buf[32] = *((char *)&LastTransValue+LONG_HIGH0);
	   buf[33] = *((char *)&LastTransValue+LONG_HIGH1);
	   memcpy(&buf[34],info,5);
	   buf[39] = gTransTypeFlag;
	   memcpy(&buf[40],gPosCityID,2);
	   buf[42] = 0;
	   buf[43] = 0;
	   shmt_prot_send(buf,44);
	   //
	   //gTaskBusyFlag = 0;
	   bgCheck24GStep = 0;
	   
	   set_auto_abort_flag();  //20100210add
	   
	   return;
	default:
		 gDebugStep=0x1212;
	   goto label_err;	   
	   }
	bgCheck24GStep++;
	return;	       
     
label_err:
	bgCheck24GStep = 0;
	gTaskBusyFlag = 0;
	return;

//卡片非法
label_card_invalid:
	gResponseStatus=CE_INVALIDCARD;
	gGetCardInfoStep=0xff;
	goto label_report;

//报告状态
label_report:
	buf[0]=SM_READCARD;
	buf[1]=0;
	buf[2]=gResponseStatus;
	shmt_prot_send(buf,3);
	gStatus=IDLE_STATUS;
	goto label_disconnect;

//解除链接
label_disconnect:
	memcpy(buf,"\xa2\x32\x00\x00",4);
	prot_zte_send_block(buf,4);
	prot_zte_rece_process(50);
	//zte_set_wait(100);
	//gTaskBusyFlag = 0;
	return;
}


//end of file
#endif




