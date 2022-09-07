//task_emu.c

#ifndef _TASK_EMU_C_
#define _TASK_EMU_C_
//start of file

sem_t g_samcalwait, g_samcalreturn;
sem_t g_usermainwait, g_usermainreturn;
sem_t g_usermainztewait;

pthread_t	g_pthsamID;

UBYTE bgTaskKeySubStep=0;
UBYTE gTaskBusyFlag=0;
UBYTE	bgSMPADF;								//
UBYTE bgTest24GFlag = 0;		//
//2013/11/13 19:31:46
UBYTE gAbortReaderFlag=0;
//UBYTE bpgSmpsamAppSjt;

UBYTE bpgChk24GModuleFlag = 1;


/*===============================================================================
函数：
功能：
=================================================================================*/
void set_task_key(UBYTE step)
{
	//OS_CPU_SR  cpu_sr;
	
	//os_int_disable();
	bgTaskKeyStep=step;
	bgTaskKeySubStep=0;
	task_key();
	//os_int_enable();
	//sem_post(&g_samcalwait);
	
	return;	
}	

/*===============================================================================
函数：
功能：
=================================================================================*/
void task_com(void)
{
	/*

//	
if(gJtProt0.rece_state != PROT_JT_RECE_WAIT_HEAD) goto label_prot_jt;
if(gJtProt0.send_state == PROT_JT_SEND_WAIT_ACK0) goto label_prot_jt;
if(gJtProt0.send_state == PROT_JT_SEND_WAIT_ACK1) goto label_prot_jt;
if(gJtProt0.send_state == PROT_JT_SEND_START) goto label_prot_jt;
if(gDtProt0.rece_state != PROT_DT_RECE_WAIT_STX)  goto label_prot_dt;
if(gDtProt0.send_state == PROT_DT_SEND_START) goto label_prot_dt;	

//无数据
if(uart_rece_is_empty(UART_DT_CMD_INDEX)) return;
if(uart_check_byte(UART_DT_CMD_INDEX) == PROT_DT_STX) goto label_prot_dt;
if(uart_check_byte(UART_JT_CMD0_INDEX) == PROT_JT_BLOCK_HEAD) goto label_prot_jt;
uart_get_byte(UART_DT_CMD_INDEX);
return;
	
//uart命令处理---------------------------------------------------------
label_prot_jt:
prot_jt_send_process(UART_JT_CMD0_INDEX,&gJtProt0);
prot_jt_rece_process(UART_JT_CMD0_INDEX,&gJtProt0);
if(gJtProt0.send_state == PROT_JT_SEND_START) goto label_prot_jt; 
return;
//
label_prot_dt:
prot_dt_send_process(UART_DT_CMD_INDEX,&gDtProt0);
prot_dt_rece_process(UART_DT_CMD_INDEX,&gDtProt0);
if(gDtProt0.send_state == PROT_DT_SEND_START) goto label_prot_dt; 
return;*/
}	

/*===============================================================================
函数：
功能：
=================================================================================*/
#ifdef _DEBUG_D_
#define _DEBUG_TASK_KEY_
#endif
void task_key(void)
{
UBYTE inbuf[257],outbuf[257];
UBYTE inbytes,obytes;
UBYTE outbytes;
int  ret, i;
UBYTE x;

//20130901-切换ADF--
	if(bgTaskKeyStep != TASK_KEY_SELECT_ADF) goto label_shanghai_card_key;
	switch(bgTaskKeySubStep)
	{
	case 0:	
		sam_select(bgSmpsamIndex);
		if(1 == bgSMPADF)
		{
			//if(1 == bpgSHpsamAppSjtFlag) return 0;
			memcpy(inbuf,"\x00\xa4\x00\x00\x02\x10\x01",7);
		}
		else{
			//if(0 == bpgSHpsamAppSjtFlag) return 0;
			memcpy(inbuf,"\x00\xa4\x00\x00\x02\x10\x02",7);
		}
		gDebugStep=0x4017;	
		inbytes = 7;
		ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
		if(ret != 0)
		{
			#ifdef _DEBUG_TASK_KEY_
			debug_printf("\n select1001or1002 send err\n");
			#endif
	    goto label_err;
	  }	
	  gDebugStep=0x4018;
		if(((UBYTE)outbuf[outbytes-2]!= (UBYTE)0x61) &&((UBYTE)outbuf[outbytes-2]!= 0x90))
		{
			#ifdef _DEBUG_TASK_KEY_
				debug_printf("\n select1001or1002 answer err,get %d %d\n",outbuf[0],outbuf[1]);
			#endif
			goto label_err;
	    
		} 
		#ifdef _DEBUG_switch_1001or1002_app
						debug_printf("\n select1001or1002 ok\n");
		#endif
		if(1 ==bgSMPADF)
			memcpy(bgSmpsamADF, "\x10\x01", 2);
		else
			memcpy(bgSmpsamADF, "\x10\x02", 2);
		goto label_ok;
	}
	bgTaskKeySubStep++;
	return;

	
//计算上海卡密钥-----------------------------------------------------
label_shanghai_card_key:
	if(bgTaskKeyStep != TASK_KEY_GET_SHKEYA) goto label_con1;
	switch(bgTaskKeySubStep)
	{
	case 0:	
		memcpy(inbuf,"\x80\xfc\x00\x01",4);
		inbuf[4]=gbTaskKeyNum+12;
		inbuf[5]=gCityCode0;
		inbuf[6]=gCityCode1;
		memcpy(&inbuf[7],bgTaskKeyInbuf,gbTaskKeyNum+10);
		inbytes =gbTaskKeyNum+17;
	  	gDebugStep=0x1005;
	  	ret = sam_apdu(bgSHpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
	//	ret = sam_apdu_send(bgSHpsamIndex,inbuf,inbytes);
	//	if(ret != 0) goto label_err;
	//	break;
	//case 1:				
	//  if(timer_check(TIMER_CMD_PROCESS_INDEX)) goto label_err;
	  	//else return;
	//  ret = sam_apdu_receive(outbuf,&obytes);
	  	if(ret != 0) 
	  	{
	   		goto label_err;
		}
	  	if((outbuf[obytes-2]!=0x61)&&(outbuf[obytes-2]!=0x90))
		{
	    	goto label_err;
		}
	  	if(obytes == (gbTaskKeyNum*6+2)) 
	  	{
		  	gDebugStep=0x100b;
			memcpy(bgTaskKeyOutbuf,outbuf,gbTaskKeyNum*6);
			goto label_ok;
		}
	  	if((outbuf[obytes-2]==0x61)&&(outbuf[obytes-1]==gbTaskKeyNum*6)) 
	  	{
	    	break;
		}
		
	case 1:  
	  	gDebugStep=0x1008;
		memcpy(inbuf,"\x00\xc0\x00\x00",4);
		inbuf[4]=gbTaskKeyNum*6;	
	  	inbytes = 5;
	  	//20130901
	  	//delay_ms(6);
	  	ret = sam_apdu(bgSHpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
	//  ret = sam_apdu_send(bgSHpsamIndex,inbuf,inbytes);
	//  if(ret != 0) goto label_err;
	//  break;
	//case 2:   
	  	gDebugStep=0x1009;
	//  if(timer_check(TIMER_CMD_PROCESS_INDEX)) goto label_err;
	  	//else return;
	//  gDebugStep=0x100a;
	//  ret = sam_apdu_receive(outbuf,&obytes);
	  	if((ret != 0) || (obytes != (gbTaskKeyNum*6+2))) 
	  	{
	  		goto label_err;
		}
	  	gDebugStep=0x100b;
		memcpy(bgTaskKeyOutbuf,outbuf,gbTaskKeyNum*6);
		goto label_ok;	
	}
	bgTaskKeySubStep++;
	return;  
  
//计算上海卡TAC-------------------------------------------------------
label_con1:
	if(bgTaskKeyStep != TASK_KEY_GET_SHTAC) goto label_con2;
	switch(bgTaskKeySubStep)
	{
	case 0:	
		memcpy(inbuf,"\x80\x1a\x44\x01\x10",5);
		memcpy(&inbuf[5],&bgTaskKeyInbuf[6],4);
		memcpy(&inbuf[9],"\xff\x00\x00\x00",4);
		inbuf[13]=gCityCode0;
		inbuf[14]=gCityCode1;
		memcpy(&inbuf[15],"\xff\x00\x00\x00\x00\x00",6);
		inbytes=21;
		label_sametoB28:
		ret = sam_apdu(bgSHpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
#ifdef	_DEBUG_TASK_KEY_
		printf("SH PTC TAC init send:");
		for(i = 0; i < inbytes; i++)
			printf("%02x", inbuf[i]);
		printf("\n");
		printf(" return :");
		for(i = 0; i < obytes; i++)
			printf("%02x", outbuf[i]);
		printf("\n");
#endif
	//	ret = sam_apdu_send(bgSHpsamIndex,inbuf,inbytes);
	//	if(ret != 0) goto label_err;
	//	break;
	//case 1:		
	//  if(timer_check(TIMER_CMD_PROCESS_INDEX)) goto label_err;
	//  	//else return;
	//  ret = sam_apdu_receive(outbuf,&obytes);
	  	if((ret != 0) || (outbuf[obytes-2]!=0x90)||(outbuf[obytes-1]!=0x00)) 
	  	{
	   		goto label_err;
		}
		break;
		
	case 1:
	  	memcpy(inbuf,"\x80\xfa\x01\x00\x20",5);
	  	memcpy(&inbuf[5],bgTaskKeyInbuf,32);
	  	inbytes=37;
	  //delay_ms(6);
	  	ret = sam_apdu(bgSHpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
#ifdef	_DEBUG_TASK_KEY_
		printf("SH PTC TAC cal send:");
		for(i = 0; i < inbytes; i++)
			printf("%02x", inbuf[i]);
		printf("\n");
		printf(" return :");
		for(i = 0; i < obytes; i++)
			printf("%02x", outbuf[i]);
		printf("\n");
#endif
	//  ret = sam_apdu_send(bgSHpsamIndex,inbuf,inbytes);
	//	if(ret != 0) goto label_err;
	//	break;
	//case 2:
	//	if(timer_check(TIMER_CMD_PROCESS_INDEX)) goto label_err;
	//	 // else return;
	//  ret = sam_apdu_receive(outbuf,&obytes);
	  	if(ret != 0) 
	  	{
	    	goto label_err;
		}
	  	if((outbuf[obytes-2]!=0x61)&&(outbuf[obytes-2]!=0x90))
	  	{
	    	goto label_err;
		}
	  	if(obytes == (4+2)) 
	  	{
		  	gDebugStep=0x100b;
			memcpy(bgTaskKeyOutbuf,outbuf,4);
			goto label_ok;
		}
	  	if((outbuf[obytes-2]==0x61)&&(outbuf[obytes-1]==4)) 
	  	{
	    	break;
		}
	
	case 2:
		memcpy(inbuf,"\x00\xc0\x00\x00\x04",5);
	  	inbytes = 5;
	  //delay_ms(6);
	  	ret = sam_apdu(bgSHpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
	//  ret = sam_apdu_send(bgSHpsamIndex,inbuf,inbytes);
	//  if(ret != 0) goto label_err;
	//  break;
	//case 3:  
	//  if(timer_check(TIMER_CMD_PROCESS_INDEX)) goto label_err;
	//  	//else return;
	//  ret = sam_apdu_receive(outbuf,&obytes);
	  	if((ret != 0) || (obytes != 6)) goto label_err;
	  // 
	  	memcpy(bgTaskKeyOutbuf,outbuf,4);
	  //
	  	goto label_ok;
	}
	bgTaskKeySubStep++;
	return;   
  
//计算单程票密钥-----------------------------------------------------
label_con2:
	if(bgTaskKeyStep != TASK_KEY_GET_SMKEYA) goto label_con3;
	switch(bgTaskKeySubStep){
	case 0:	
		inbuf[0] = 0x80;
		inbuf[1] = 0xfc;
		inbuf[2] = bgTaskKeyInbuf[13]/16+1;
		inbuf[3] = (bgTaskKeyInbuf[13]&0x0f)+1;
		inbuf[4] = 0x0d;
		memcpy(&inbuf[5],bgTaskKeyInbuf,13);
		inbytes = 18;
		#ifdef _DEBUG_SAM_
		debug_printf("\x0d\x0a send %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",inbuf[0],inbuf[1],inbuf[2],inbuf[3],inbuf[4],inbuf[5],inbuf[6],inbuf[7],inbuf[8],inbuf[9],inbuf[10],inbuf[11],inbuf[12],inbuf[13],inbuf[14],inbuf[15],inbuf[16],inbuf[17]);
		#endif
		ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
	//	ret = sam_apdu_send(bgSmpsamIndex,inbuf,inbytes);
	//	if(ret != 0)
	//	{
	//		#ifdef _DEBUG_SAM_
	//		debug_printf("\x0d\x0a send step1 err ");
	//		#endif
	//		goto label_err;
	//	}
	//	break;
	//case 1:	
	//  if(timer_check(TIMER_CMD_PROCESS_INDEX)) goto label_err;
	//  	//else return;
	//  ret = sam_apdu_receive(outbuf,&obytes);
	  if(ret != 0) 
	  {
	    goto label_err;
		}
	  if((outbuf[obytes-2]!=0x61)&&(outbuf[obytes-2]!=0x90))
	  {
	    goto label_err;
		}
	  if(obytes == (6+2)) 
	  {
		  gDebugStep=0x100b;
			memcpy(bgTaskKeyOutbuf,outbuf,6);
			goto label_ok;
		}
	  if((outbuf[obytes-2]==0x61)&&(outbuf[obytes-1]==6)) 
	  {
	    break;
		}
		
	case 1:
		memcpy(inbuf,"\x00\xc0\x00\x00\x06",5);
	  inbytes = 5;
	  //delay_ms(6);
	  ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
	//  ret = sam_apdu_send(bgSmpsamIndex,inbuf,inbytes);
	// 	if(ret != 0)
	//	{
	//		#ifdef _DEBUG_SAM_
	//						debug_printf("\x0d\x0a send step2 err.");
	//		#endif
	//		goto label_err;
	//	}
	//	break;
	//case 2:	
	//  if(timer_check(TIMER_CMD_PROCESS_INDEX)) goto label_err;
	//  	//else return;
	//  ret = sam_apdu_receive(outbuf,&obytes);
	  if((ret!=0)||(obytes!=8))
	  {
	  	#ifdef _DEBUG_SAM_
							debug_printf("\x0d\x0a rcv step2 err. return=%d  obytes=%02x ",ret,obytes);
			#endif
	  	goto label_err;
	  }
	  memcpy(bgTaskKeyOutbuf,outbuf,6);
	  goto label_ok;
	}
	bgTaskKeySubStep++;
	return;

//计算单程票TAC
label_con3:
	if(bgTaskKeyStep!=TASK_KEY_GET_SMTAC) goto label_con4;
	switch(bgTaskKeySubStep){
	case 0:	
		memcpy(inbuf,"\x80\x1a\x24\x01\x08",5);
		memcpy(&inbuf[5],bgTaskKeyInbuf,4);
		memcpy(&inbuf[9],"\xff\x00\x00\x00",4);
		inbytes=13;
		ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
	//	ret = sam_apdu_send(bgSmpsamIndex,inbuf,inbytes);
	//	if(ret != 0) goto label_err;
	//	break;
	//case 1:		
	//  if(timer_check(TIMER_CMD_PROCESS_INDEX)) goto label_err;
	//  	//else return;
	//  ret = sam_apdu_receive(outbuf,&obytes);
		if((ret != 0) || (outbuf[obytes-2]!=0x90)||(outbuf[obytes-1]!=0x00)) 
		{
	   		goto label_err;
		}
		break;
		
	case 1:
		memcpy(inbuf,"\x80\xfa\x01\x00\x20",5);
		memcpy(&inbuf[5],bgTaskKeyInbuf,32);
		inbytes=37;
	  //delay_ms(6);
	  ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
	//  ret = sam_apdu_send(bgSmpsamIndex,inbuf,inbytes);
	//	if(ret != 0) goto label_err;
	//	break;
	//case 2:
	//  if(timer_check(TIMER_CMD_PROCESS_INDEX)) goto label_err;
	//  	//else return;
	//  ret = sam_apdu_receive(outbuf,&obytes);
	  if(ret != 0) 
	  {
	    goto label_err;
		}
	  if((outbuf[obytes-2]!=0x61)&&(outbuf[obytes-2]!=0x90))
	  {
	    goto label_err;
		}
	  if(obytes == (4+2)) 
	  {
		  gDebugStep=0x100b;
			memcpy(bgTaskKeyOutbuf,outbuf,4);
			goto label_ok;
		}
	  if((outbuf[obytes-2]==0x61)&&(outbuf[obytes-1]==4)) 
	  {
	    break;
		}
	
	case 2:
		memcpy(inbuf,"\x00\xc0\x00\x00\x04",5);
	  inbytes = 5;
	  //delay_ms(6);
	  ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
	//  ret = sam_apdu_send(bgSmpsamIndex,inbuf,inbytes);
	//  if(ret != 0) goto label_err;
	//  break;
	//case 3:  
	//  if(timer_check(TIMER_CMD_PROCESS_INDEX)) goto label_err;
	//  	//else return;
	//  ret = sam_apdu_receive(outbuf,&obytes);
	  if((ret != 0) || (obytes != 6)) 
	  {
	  	goto label_err;
	  }
	  memcpy(bgTaskKeyOutbuf,outbuf,4);
	  goto label_ok;
	}
	bgTaskKeySubStep++;
	return;

//上海卡认证B28
label_con4:
	if(bgTaskKeyStep!=TASK_KEY_GET_SHB28) goto label_con5;
	memcpy(inbuf,"\x80\x1a\x44\x01\x10",5);
	memcpy(&inbuf[5],&bgTaskKeyInbuf[15],4);
	memcpy(&inbuf[9],"\xff\x00\x00\x00",4);
	memcpy(&inbuf[13],&bgTaskKeyInbuf[13],2);
	memcpy(&inbuf[15],"\xff\x00\x00\x00\x00\x00",6);
	inbytes=21;
	bgTaskKeyStep = TASK_KEY_GET_SHTAC;
	goto label_sametoB28;
	
//上海CPU卡交易前向SAM卡发送认证码
label_con5:
	if(bgTaskKeyStep!=TASK_KEY_SEND_AUTHCODE) goto label_con6;
	switch(bgTaskKeySubStep){
	case 0:	
		memcpy(inbuf,"\x80\xCA\x00\x00\x09",5);
		memcpy(&inbuf[5],bgTaskKeyInbuf,9);
		inbytes = 14;
		ret = sam_apdu(bgSHpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
	  //ret = sam_apdu_send(bgSHpsamIndex,inbuf,inbytes);
	//  if(ret != 0) goto label_err;
	  //break;
	//case 1: 
	//  if(timer_check(TIMER_CMD_PROCESS_INDEX)) goto label_err;
	  	//else return;
	//  ret = sam_apdu_receive(outbuf,&obytes);
	  if((ret != 0) || ((UBYTE)outbuf[obytes-2] != (UBYTE)0x90)||(outbuf[obytes-1]!=0x00)) 
	  {
	  	goto label_err;
		}
		goto label_ok;
	} 
	bgTaskKeySubStep++;
	return;


//移动手机支付交易前向SAM卡发送交易初始化命令
label_con6:
	if(bgTaskKeyStep!=TASK_KEY_INIT_FOR_SAM_2) goto label_con7;
	switch(bgTaskKeySubStep){	
	case 0:
		x = 0;
		memcpy(inbuf,"\x80\x70\x00\x00\x24",5);
		memcpy(&inbuf[5],bgTaskKeyInbuf,36);
		inbuf[41] = 0x08;
		inbytes = 42;
	  //ee_write(100,42,inbuf);
	  ret = sam_apdu(bgSMobilepsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
	//	ret = sam_apdu_send(bgSMobilepsamIndex,inbuf,inbytes);
	//  if(ret != 0) goto label_err;
	//  break;
	//case 1:  	
	//  if(timer_check(TIMER_CMD_PROCESS_INDEX)) goto label_err;
	//  	//else return;
	//  ret = sam_apdu_receive(outbuf,&obytes);
	  if(ret != 0) 
	  {
	  	goto label_err;
		}
		x = 1;
	  if((outbuf[obytes-2]!=0x61)&&(outbuf[obytes-2]!=0x90))
	  {
	    goto label_err;
		}
	  if(obytes == (8+2)) 
	  {
		  gDebugStep=0x100b;
			memcpy(bgTaskKeyOutbuf,outbuf,8);
			goto label_ok;
		}
	  if((outbuf[obytes-2]==0x61)&&(outbuf[obytes-1]==8)) 
	  {
	    break;
		}
	
	case 1:
		x = 2;
	 	memcpy(inbuf,"\x00\xc0\x00\x00\x08",5);
		inbytes = 5;
		//delay_ms(6);
		ret = sam_apdu(bgSMobilepsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
	//	ret = sam_apdu_send(bgSMobilepsamIndex,inbuf,inbytes);
	//  if(ret != 0) goto label_err;
	//  break;
	//case 2:  
	//  if(timer_check(TIMER_CMD_PROCESS_INDEX)) goto label_err;
	//  	//else return;
	//  ret = sam_apdu_receive(outbuf,&obytes);
	  if(ret != 0) 
	  {
	  	goto label_err;
		}
		x = 3;
		if(obytes!=10)
		{
			goto label_err;
		}
		memcpy(bgTaskKeyOutbuf,outbuf,8);
		goto label_ok;
	}
	bgTaskKeySubStep++;
	return;


//上海CPU卡交易前向SAM卡发送交易初始化命令
label_con7:
	if(bgTaskKeyStep!=TASK_KEY_INIT_FOR_SAM) goto label_con8;
	switch(bgTaskKeySubStep){	
	case 0:	
		memcpy(inbuf,"\x80\x70\x00\x01\x28",5);
		memcpy(&inbuf[5],bgTaskKeyInbuf,40);
		inbytes = 45;
	  //ee_write(37,45,inbuf);
	  ret = sam_apdu(bgSHpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
	//	ret = sam_apdu_send(bgSHpsamIndex,inbuf,inbytes);
	//  if(ret != 0) goto label_err;
	//  break;
	// case 1: 	
	//    if(timer_check(TIMER_CMD_PROCESS_INDEX)) goto label_err;
	//    	//else return;
	//    ret = sam_apdu_receive(outbuf,&obytes);
	  if(ret != 0) 
	  {
	    goto label_err;
		}
	  if((outbuf[obytes-2]!=0x61)&&(outbuf[obytes-2]!=0x90))
	  {
	    goto label_err;
		}
	  if(obytes == (8+2)) 
	  {
		  gDebugStep=0x100b;
			memcpy(bgTaskKeyOutbuf,outbuf,8);
			goto label_ok;
		}
	  if((outbuf[obytes-2]==0x61)&&(outbuf[obytes-1]==8)) 
	  {
	    break;
		}
		  
	case 1:
		memcpy(inbuf,"\x00\xc0\x00\x00\x08",5);
		inbytes = 5;
		//delay_ms(6);
		ret = sam_apdu(bgSHpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
		//		ret = sam_apdu_send(bgSHpsamIndex,inbuf,inbytes);
		//	  if(ret != 0) goto label_err;
		//	  break;
		//	case 2:  	
		//    if(timer_check(TIMER_CMD_PROCESS_INDEX)) goto label_err;
		//    	//else return;
		//    ret = sam_apdu_receive(outbuf,&obytes);
		if(ret != 0) 
		{
			goto label_err;
		}
		if(obytes!=10)
		{
			goto label_err;
		}
		memcpy(bgTaskKeyOutbuf,outbuf,8);
		goto label_ok;
	}
	bgTaskKeySubStep++;
	return;


//二维码消费密钥
label_con8:
	if(bgTaskKeyStep!=TASK_KEY_GET_QRKEYA) goto label_con9;
	switch(bgTaskKeySubStep)
	{	
	case 0:
		if(memcmp(bgSmpsamADF, "\x10\x01", 2) != 0)
		{
			memcpy(inbuf, "\x00\xa4\x00\x00\x02\x10\x01",7);
			inbytes = 7;
			ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
			if(ret != 0)
				goto label_err;
			if(((UBYTE)outbuf[outbytes-2]!= (UBYTE)0x61) &&((UBYTE)outbuf[outbytes-2]!= 0x90))
				goto label_err;
			memcpy(bgSmpsamADF, "\x10\x01", 2);
			printf("select adf 1001 ok\n");
		}
		memcpy(inbuf, "\x80\xfc\x10\x10\x0f", 5);
		memcpy(&inbuf[5], bgTaskKeyInbuf, 15);
		ret = sam_apdu(bgSmpsamIndex, inbuf, 20, outbuf, &obytes, 0, 0);
	  	if(ret != 0) 
	  	{
	  		printf("80 fc failure\n");
	   		goto label_err;
		}
	  	if((outbuf[obytes-2]!=0x61)&&(outbuf[obytes-2]!=0x90))
		{
			printf("80 fc failure un 9000 %02x%02x\n", outbuf[obytes-2] , outbuf[obytes-1]);
	    	goto label_err;
		}
		memcpy(bgTaskKeyOutbuf, outbuf, 18);
		goto label_ok;
	}
	bgTaskKeySubStep++;	
	return;
	
//二维码交易记录TAC
label_con9:
	if(bgTaskKeyStep!=TASK_KEY_GET_QRTAC) goto label_con10;
	switch(bgTaskKeySubStep){
	case 0:	
		memcpy(inbuf,"\x80\x1a\x24\x01\x08",5);
		memcpy(&inbuf[5],bgTaskKeyInbuf,8);
		inbytes=13;
		ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
		if((ret != 0) || (outbuf[obytes-2]!=0x90)||(outbuf[obytes-1]!=0x00)) 
		{
	   		goto label_err;
		}
	case 1:
		memcpy(inbuf,"\x80\xfa\x01\x00\x20",5);
		memcpy(&inbuf[5],bgTaskKeyInbuf,32);
		inbytes=37;
//		print_debug(inbuf, 37);
		ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
//	  	print_debug(outbuf, obytes);
	  	if(ret != 0) 
	  	{
	    	goto label_err;
		}
	  	if((outbuf[obytes-2]!=0x61)&&(outbuf[obytes-2]!=0x90))
	  	{
	    	goto label_err;
		}
	  	if(obytes == (4+2)) 
	  	{
			gDebugStep=0x100b;
			memcpy(bgTaskKeyOutbuf,outbuf,4);
			goto label_ok;
		}
	  	if((outbuf[obytes-2]==0x61)&&(outbuf[obytes-1]==4)) 
	  	{
	    	break;
		}
	
	case 2:
		memcpy(inbuf,"\x00\xc0\x00\x00\x04",5);
	  	inbytes = 5;
	  	ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
	  	if((ret != 0) || (obytes != 6)) 
	  	{
	  		goto label_err;
	  	}
	  	memcpy(bgTaskKeyOutbuf,outbuf,4);
	 	 goto label_ok;
	}
	bgTaskKeySubStep++;
	return;
//二维码交易记录TAC
label_con10:
	if(bgTaskKeyStep!=TASK_KEY_GET_ODATAC) goto label_con11;
	switch(bgTaskKeySubStep){
	case 0:	
		memcpy(inbuf,"\x80\x1a\x24\x01\x08",5);
		memcpy(&inbuf[5],bgTaskKeyInbuf,8);
		inbytes=13;
		ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
		if((ret != 0) || (outbuf[obytes-2]!=0x90)||(outbuf[obytes-1]!=0x00)) 
		{
	   		goto label_err;
		}
	case 1:
		memcpy(inbuf,"\x80\xfa\x01\x00\x20",5);
		memcpy(&inbuf[5],bgTaskKeyInbuf,32);
		inbytes=37;
//		print_debug(inbuf, 37);
		ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
//	  	print_debug(outbuf, obytes);
	  	if(ret != 0) 
	  	{
	    	goto label_err;
		}
	  	if((outbuf[obytes-2]!=0x61)&&(outbuf[obytes-2]!=0x90))
	  	{
	    	goto label_err;
		}
	  	if(obytes == (4+2)) 
	  	{
			gDebugStep=0x100b;
			memcpy(bgTaskKeyOutbuf,outbuf,4);
			goto label_ok;
		}
	  	if((outbuf[obytes-2]==0x61)&&(outbuf[obytes-1]==4)) 
	  	{
	    	break;
		}
	
	case 2:
		memcpy(inbuf,"\x00\xc0\x00\x00\x04",5);
	  	inbytes = 5;
	  	ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
	  	if((ret != 0) || (obytes != 6)) 
	  	{
	  		goto label_err;
	  	}
	  	memcpy(bgTaskKeyOutbuf,outbuf,4);
	 	goto label_ok;
	}
//交通部MAC1 20190404	
label_con11:
	if(bgTaskKeyStep!=TASK_KEY_GET_JTBKEYA) goto label_con12;
	switch(bgTaskKeySubStep)
	{	
	case 0:	
		memcpy(inbuf,"\x80\x70\x00\x00\x24",5);
		memcpy(&inbuf[5],bgTaskKeyInbuf,36);
		inbytes = 41;
		//ee_write(37,45,inbuf);
		ret = sam_apdu(bgSHJTBpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
		printf("\ncal MAC1 inbuf :");
		for(i=0;i<inbytes;i++)
		{
			printf(" %02x",inbuf[i]);
		}
		printf("\ncalculate mac1...ret=%d\n",ret);
		printf("outbytes...=%d\n",obytes);
		printf(" mac1 :");
		for(i=0;i<obytes;i++)
		{
			printf(" %02x",outbuf[i]);	
		}
		printf("\n"); 
		if(ret != 0) 
		{
			goto label_err;
		}
		if((outbuf[obytes-2]!=0x61)&&(outbuf[obytes-2]!=0x90))
		{
			goto label_err;
		}
		if(obytes == (8+2)) 
		{
		  gDebugStep=0x100b;
			memcpy(bgTaskKeyOutbuf,outbuf,8);
			goto label_ok;
		}
	  if((outbuf[obytes-2]==0x61)&&(outbuf[obytes-1]==8)) 
	  {
	    break;
		}
		  
	case 1:
		memcpy(inbuf,"\x00\xc0\x00\x00\x08",5);
		inbytes = 5;
		//delay_ms(6);
		ret = sam_apdu(bgSHJTBpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
		//		ret = sam_apdu_send(bgSHpsamIndex,inbuf,inbytes);
		//	  if(ret != 0) goto label_err;
		//	  break;
		//	case 2:  	
		//    if(timer_check(TIMER_CMD_PROCESS_INDEX)) goto label_err;
		//    	//else return;
		//    ret = sam_apdu_receive(outbuf,&obytes);
		if(ret != 0) 
		{
			goto label_err;
		}
		if(obytes!=10)
		{
			goto label_err;
		}
		memcpy(bgTaskKeyOutbuf,outbuf,8);
		goto label_ok;
	}
	bgTaskKeySubStep++;
	return;
	
label_con12:
	bgTaskKeySubStep=0;	
	return;
	//---------------------------------------
label_err:
	#ifdef _DEBUG_TASK_KEY_
	debug_printf("\x0d\x0a Task key:step=%d,sub step=%d",bgTaskKeyStep,bgTaskKeySubStep);
	#endif
	bgTaskKeyStep = TASK_KEY_FINISH;
	bgTaskKeyFinishState = 1;
	bgTaskKeySubStep = 0;
	//20130901
	//sem_post(&g_samcalreturn);
	return;
label_ok:
	bgTaskKeyStep = TASK_KEY_FINISH;
	bgTaskKeyFinishState = 0;
	bgTaskKeySubStep = 0;
	//sem_post(&g_samcalreturn);
	return;
}




/*===============================================================================
函数：
功能：
=================================================================================*/
//可关
void task_key_buzzle(void)
{
//
/*
if(timer_check(TIMER_500MS_INDEX))
{ 
  watchdog();
  timer_clr(TIMER_500MS_INDEX);
  if(bgGledState == LED_ON) gled(LED_OFF);
  else gled(LED_ON);
  if(gRedSign==1)
  {
  	rled(LED_ON);
  }
}
*/

//显示与蜂鸣器	
switch(timeflag&0x03)
{
	case 0x01:	if(timer_check(TIMER_MS_INDEX0))
							{
								  gledantenna(LED_OFF);
								  timeflag&=0xfc;
						  }
						  break;
	case 0x02:	if(timer_check(TIMER_MS_INDEX0))
							{
									timer_clr(TIMER_MS_INDEX0);
									if(bgGState == LED_ON) 
									{
										gledantenna(LED_OFF);
  								}
  								else 
  								{
  									gledantenna(LED_ON);
								  }
						  }
	default:	  break;
}
switch(timeflag&0x0c)
{
	case 0x04:	if(timer_check(TIMER_MS_INDEX1))
							{
								  rledantenna(LED_OFF);
								  timeflag&=0xf3;
						  }
						  break;
	case 0x08:	if(timer_check(TIMER_MS_INDEX1))
							{
									timer_clr(TIMER_MS_INDEX1);
									if(bgRState == LED_ON) 
									{
										rledantenna(LED_OFF);
  								}
  								else 
  								{
  									rledantenna(LED_ON);
								  }
						  }
	default:	  break;
}
switch(timeflag&0x30)
{
	case 0x10:	if(timer_check(TIMER_MS_INDEX2))
							{
								  buzzerantenna(LED_OFF);
								  timeflag&=0xcf;
						  }
						  break;
	case 0x20:	if(timer_check(TIMER_MS_INDEX2))
							{
									timer_clr(TIMER_MS_INDEX2);
									if(bgBuzzerState == LED_ON) 
									{
										buzzerantenna(LED_OFF);
  								}
  								else 
  								{
  									buzzerantenna(LED_ON);
								  }
						  }
	default:	  break;
  }

return;	
}	

//void *pthsamcal()
//{
//#ifdef DEBUG_PRINT
//	printf("sam calculation thread start\n");
//#endif
//	for(;;)
//	{
//	  sem_wait(&g_samcalwait);
//	  //task_key_buzzle();
//	  task_key();
//  }
//     
//return;  
//}

void task_key_start(void)
{
	task_key();
	return;	
}

int task_key_inquiry(void)
{
	
	
}


//end of file
#endif


