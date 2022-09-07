//user_main.c
//用户应用在此函数实现

//#define SCANNERPROTOCOL


#ifndef _USER_MAIN_C_
#define _USER_MAIN_C_
//start of file

//#define _DEBUG_READ_
//#define _SPEED_TEST_
//#define _DEBUG_STEP_OUT_
extern unsigned char qrDataBak[512];
/*=============================================================================================
函数：user_main
功能：用户主程序
注意：在此函数中不用使用死循环方式
===============================================================================================*/
#ifdef DEBUG_PRINT
#define _DEBUG_user_main_
#endif
void user_main(void)
{
	UBYTE ret,para;
	int counter,i;
	UBYTE buf[16];
	UBYTE ec;
		
#ifdef _DEBUG_user_main_
//if((gGetCardInfoStep == 0)&&(gStatus == READ_STATUS)) debug_printf("\nuser_main,gGetCardInfoStep[%02x],gStatus[%02x],gTaskBusyFlag[%02x],gPowerOffFlag[%02x]....\n",gGetCardInfoStep,gStatus,gTaskBusyFlag,gPowerOffFlag);
#endif
	//debug_printf("\nuser_main debug!\n");

	watchdog(); //
	if(gGetCardInfoStep!=0) 
	{
		#ifdef _DEBUG_user_main_
		debug_printf("\nuser_mainXXXXXXX 1 gGetCardInfoStep=%02X gTaskBusyFlag=%02X\n",gGetCardInfoStep,gTaskBusyFlag);
		#endif
		return;
	}
	if(gStatus != READ_STATUS) 
	{
		#ifdef _DEBUG_user_main_
		debug_printf("\nuser_mainXXXXXXX 2");	
		#endif
		return;
	}//2011-4-2 16:32:27
/*if(gTaskBusyFlag!=0) return;
if(gPowerOffFlag&0x01){
		if(timer_check(TIMER_MS_POWEROFF))
		{
			timer_clr(TIMER_MS_POWEROFF);
			gPowerOffFlag++;
		}		
		return;
	}*/ 
 
#ifdef _DEBUG_user_main_
	debug_printf("\nuser_main....2");
	debug_printf("\nlibjr1004.a-20130725");
#endif
	sh_ticket_family=0;//20191016
	counter=0;
	gDebugStep=0;
	lastSector=0;
	mcml_pwr_on();
	set_timeout(6000);
	for(counter=0;counter<3;counter++)
	{
		//mcml_pwr_on();
		//set_timeout(6000);
		memset(buf,0,sizeof(buf));
		//ret=mcml_request(PICC_REQSTD,buf);
		ret=mcml_request2(PICC_REQALL,buf); //2014/9/11 10:00:16
		if (ret == 0) 
			break;
		//mcml_pwr_off();	
		//set_timeout(8000);
	}

	if(counter == 3)
	{
		mcml_pwr_off();	
		#ifdef _DEBUG_user_main_
		debug_printf("\nuser_mainXXXXXXX 33\n");
		#endif	
		//2013/10/29 15:37:30
		//delay_ms(2);
		//set_timeout(5000);
		//sem_post(&g_usermainwait);		
		return;		
	}
	if(gTaskBusyFlag!=0) 
	{
		#ifdef _DEBUG_user_main_
		debug_printf("\nuser_mainXXXXXXX 3\n");	
		#endif
		//sem_post(&g_usermainwait);
		return;
	}

#ifdef _DEBUG_user_main_
	debug_printf("\nuser_main....3,req buf[%02x][%02x]\n",buf[0],buf[1]);
	//ReaderResponse(csc_comm, 0x00, 0xF0, buf, 2);
#endif

 
#ifdef DEBUG_TIME
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x00", 3);
#endif	
//寻找到卡片 
	gDebugStep=1;
	gTaskBusyFlag = 1;
	if(memcmp(buf,"\x04\x00",2) == 0){
	 	if((UBYTE)ReadMode==(UBYTE)0x90) goto label_cpu;
	 	if((UBYTE)ReadMode==(UBYTE)0x91) goto label_cpu;
	 	goto label_m1;	
	}
	//2013/9/16 11:36:15 新卡
	if(memcmp(buf,"\x01\x00",2) == 0){
		if((UBYTE)ReadMode==(UBYTE)0x90) goto label_cpu;
		if((UBYTE)ReadMode==(UBYTE)0x91) goto label_cpu;
	 	goto label_m1;	
	}
	//
	if(memcmp(buf,"\x08\x00",2) == 0){//2011-1-19 15:32:37 嘉兴市民卡
	 	if((UBYTE)ReadMode==(UBYTE)0x90) goto label_cpu;
	 	if((UBYTE)ReadMode==(UBYTE)0x91) goto label_cpu;
	 	goto label_m1;	
	}
	if(memcmp(buf,"\x02\x00",2) == 0){//2013/5/22 10:12:00 浦发手机支付
	 	if((UBYTE)ReadMode==(UBYTE)0x90) goto label_cpu;
	 	if((UBYTE)ReadMode==(UBYTE)0x91) goto label_cpu;
	 	goto label_m1;	
	}
	if(memcmp(buf,"\x03\x00",2) == 0){
		 para = 0;
	 	 goto label_metro;
	 }
	if(memcmp(buf,"\x00\x36",2) == 0){
		 para = 1;
	 	 goto label_metro;
	 }
	gTaskBusyFlag = 0;	 
	return;

//CPU卡处理--------------------------------------------------
label_cpu:
	ret = getcardinfo_CPU();
#ifdef _DEBUG_user_main_
	printf("getcardinfo_CPU...ret=%02x\n",ret);
#endif
	if(ret == CE_OK_PF_MOBILE) goto label_exit;
	if(ret == CE_UNKNOWN) goto label_only_exit;
	//if(ret==0) goto label_info_rpt;
	if(ret==0) 
	{
		goto label_info_rpt;
	}
#ifdef _DEBUG_user_main_
	printf("ReadMode =%02x; gCPUValid =%02x; gCPUflag =%02x\n",ReadMode,gCPUValid,gCPUflag);
#endif
	//BOM机应用未解锁报告
	if((ReadMode == 0x91)&&(gCPUValid==0)){
		buf[0] = SM_READCARD;
		buf[1] = gReadCmdTimes;
		buf[2] = CE_OK;
		buf[3] = 0x91;
		memcpy(&buf[4],gThisCardSnr,8);
		buf[12] = gAppSn[0];
		buf[13] = gAppSn[1];
		buf[14] = gCardType;
		shmt_prot_send(buf,15);
		gStatus=IDLE_STATUS;
		//
		set_auto_abort_flag();  //20110216add
	   
		 mcml_pwr_off();
//		 delay_ms(2);
	 		goto label_exit;		
	}	
	
	//if(((ReadMode == 0x90)||(ReadMode == 0x91))&&(gCPUflag == 0x00)) goto label_m1;
	if(((ReadMode == 0x90)||(ReadMode == 0x91))&&(gCPUflag == 0x00))
	{
		goto label_m1;
	} 
	//中间错误，继续寻卡
	if(gGetCardInfoStep==0){
		 mcml_pwr_off();
//		 delay_ms(2);
		 goto label_only_exit;
	}	
	ec = gResponseStatus;
	goto label_ec_rpt;

//M1卡处理---------------------------------	
label_m1:
#ifdef _DEBUG_user_main_
	printf("bgSHPsamValid=%d\n",bgSHPsamValid);
#endif
	if(bgSHPsamValid!=1){
	  ec = CE_NCSCSAM;
	  goto label_ec_rpt;
  }	
	bgSHpsamIndex=SAMLOCATION_5;			//ָ指定SAM卡座
	ret=getcardinfo();
#ifdef _DEBUG_user_main_
	printf("M1_getcardinfo...ret=%d\n",ret);
#endif
	if(ret==0) goto label_info_rpt;
	if(((ReadMode == 0x80)||(ReadMode == 0x81))&&(gCPUflag == 0x55)) goto label_cpu;
	if(gGetCardInfoStep!=0){			
	  ec = gResponseStatus;
	  goto label_ec_rpt;
  }
//中间错误，继续寻卡
goto label_only_exit;
  
//单程票处理---------------------------------	
label_metro:
	if(bgSmPsamValid!=1){
		ec = CE_STSAM;
		goto label_ec_rpt;
	}
	bgSmpsamIndex=SAMLOCATION_4;			//指定SAM卡座
	ret=hh_getcardinf(para);
	if(ret!=0){
		if(gGetCardInfoStep != 0xff) goto label_only_exit; //中间错误，继续寻卡
		ec = gResponseStatus;
		goto label_ec_rpt;
	}
	check_card_metro_info_rpt();  
	goto label_exit;

//错误出口
label_ec_rpt:
	check_card_err_rpt(ec);
	gStatus=IDLE_STATUS;
//
	set_auto_abort_flag();  //20110216add
	
	goto label_exit;

label_info_rpt:

	if(ReadMode&0x01) bom_info_rpt();
	else not_bom_info_rpt();		
	goto label_exit;

label_exit:
	//gTaskBusyFlag = 0;
	gGetCardInfoStep = 0xff;
	return;		

label_only_exit:
	gTaskBusyFlag = 0;	
#ifdef _DEBUG_user_main_
	debug_printf("\nuser_mainXXXXXXX 4 gTaskBusyFlag=%02X gGetCardInfoStep=%02X\n",gTaskBusyFlag,gGetCardInfoStep);
#endif	
//sem_post(&g_usermainwait); 
	return;	
}


/*===================================================================================
函数：
功能：
=====================================================================================*/
void check_card_err_rpt(UBYTE ec)
{
	UBYTE buf[10];
	//
	buf[0]=SM_READCARD;
	buf[1]=gReadCmdTimes;
	buf[2]=ec;
	if(ec == 1) {//2011-9-15 14:07:24
		buf[2] = CE_UNKNOWN;
		gStatus = IDLE_STATUS;
		gTaskBusyFlag = 0;	 
	  gGetCardInfoStep = 0xff;
  }
#ifdef _DEBUG_STEP_OUT_
	buf[3]=(UBYTE)(gDebugStep/256);
	buf[4]=(UBYTE)(gDebugStep%256);
	shmt_prot_send(buf,5);
#else
	shmt_prot_send(buf,3);
#endif
//
return;	
}	


/*===================================================================================
函数：
功能：
=====================================================================================*/
void check_card_metro_info_rpt(void)
{
	UBYTE buf[64];
	UBYTE counter;
	//
	memset(buf,0,64);
	//	
	buf[0] = HH_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = CE_OK;
	buf[3] = 0x81;
	buf[4] = hh_mid[1];//MID
	buf[5] = hh_mid[0];
	buf[6] = gThisCardSnr[3];
	buf[7] = gThisCardSnr[2];
	buf[8] = gThisCardSnr[1];
	buf[9] = gThisCardSnr[0];
	
	buf[10] =hh_SaleBuf[3];//card type
	
	buf[11] =hh_SaleBuf[9];
	buf[12] =hh_SaleBuf[8];
	buf[13] =hh_B9[0];
	buf[14] =hh_B9[2];	 
	buf[15] =hh_B9[1];
	buf[16] =gTransTime[3];
	buf[17] =gTransTime[2];
	buf[18] =gTransTime[1];
	buf[19] =gTransTime[0];
	buf[20] =hh_trans_Line;
	buf[21] =hh_trans_Station;
	buf[22] =hh_trans_Equip;
	buf[23] =hh_SaleBuf[7];
	buf[24] =hh_SaleBuf[6];
	buf[25] =hh_SaleBuf[5];
	buf[26] =hh_SaleBuf[4];
	
	buf[27] =hh_SaleBuf[2];
	buf[28] =hh_SaleBuf[1];
	buf[29] =hh_SaleBuf[0];
	
	buf[30] =hh_SaleBuf[11];
	buf[31] =hh_SaleBuf[10];
	if((ReadMode==0x00)||(ReadMode==0x01)||(ReadMode==0x02))
	{
		counter = 32;
		shmt_prot_send(buf,counter);
		gStatus=TRANS_STATUS;
		set_auto_abort_flag();	//20100210add
		gGetCardInfoStep = 0xff;
		return;
	}
	buf[32] = hh_Exit[11];
	buf[33] = hh_Exit[10];
	buf[34] = hh_Exit[9];
	buf[35] = hh_Exit[8];
	counter = 36;
	shmt_prot_send(buf,counter);
	gStatus=TRANS_STATUS;
	set_auto_abort_flag();	//20100210add
	gGetCardInfoStep = 0xff;
	return;		
}	

/*===================================================================================
函数：
功能：
=====================================================================================*/
UBYTE check_card_get_b33(void)
{
	UBYTE ret;
	//
	if(checkrec_inmemory(REC_EXIT)==0) return 0;
	if(gCPUflag==0x55) ret = get_record(0x11,0x02,16,gB33);
	else ret = mcmh_read(33,gB33,0,0);
	if(ret) return 1;
	lrc3_gen(gB33,16);
	memcpy(gB33BK,gB33,20);
	//
	return 0;
}

/*===================================================================================
函数：
功能：
=====================================================================================*/
UBYTE check_card_get_b32(void)
{
	UBYTE ret;
	//
	if(checkrec_inmemory(REC_ENTRY)==0) return 0;
	if(gCPUflag==0x55) ret = get_record(0x11,0x01,16,gB32);
	else ret = mcmh_read(32,gB32,0,0);
	if(ret) return 1;
	lrc3_gen(gB32,16);
	memcpy(gB32BK,gB32,20);
	//
	return 0;
}


/*===================================================================================
函数：
功能：
=====================================================================================*/
void bom_info_rpt(void)
{
	UBYTE buf[64];
	UBYTE counter;
	
	memset(buf,0,64);
	
	if(gCPUflag == 0x55) goto label_cpu;
	//M1卡	
	buf[0] = SM_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = CE_OK;
	buf[3] = 0x01;
	memcpy(&buf[4],gThisCardSnr,4);
	buf[8] = gCityCode0;
	buf[9] = gCityCode1;
	buf[10] = gCardType;
	memcpy(&buf[11],gCardExpiredDate,4);
	buf[15] = *((char *)&gCardLastRemain+LONG_HIGH0);
	buf[16] = *((char *)&gCardLastRemain+LONG_HIGH1);
	buf[17] = *((char *)&gCardLastRemain+LONG_HIGH2);
	buf[18] = gCardStatus;
	uword_to_buf(gCardCnt,&buf[19]);
	buf[21] = cardversion;
	if(NormalCard==COMMCARD){
		memcpy(&buf[22],info,5);
		memcpy(&buf[27],&leisure[2],3);
	}
	else{
		buf[22] = DayToAllow;
		memcpy(&buf[23],&StartUseDay[1],3);
		memset(&buf[26],0,4);
	}
	memcpy(&buf[30],leisure,2);
	buf[32] = gMetroIoFlagBk;
	buf[33] = gMetroIoFlagBkValid;
	buf[34] = gMetroIoFlag34;
	buf[35] = gMetroIoFlag34Valid;
	counter = 36;
	shmt_prot_send(buf,counter);

	buf[0] = SM_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = CE_OK;
	buf[3] = 0x02;			
//
	if(check_card_get_b32()) goto label_err_remove;
	memcpy(&buf[4],gB32,16);
	counter = 20;
	shmt_prot_send(buf,counter);
	//			
	buf[0] = SM_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = CE_OK;
	buf[3] = 0x83;
	if(check_card_get_b33()) goto label_err_remove;
	memcpy(&buf[4],gB33,16);
	counter = 20;
	shmt_prot_send(buf,counter);
	gStatus=TRANS_STATUS;
//
	set_auto_abort_flag();	//20100210add
	gGetCardInfoStep = 0xff;
	return;

//CPU卡------------------------------
label_cpu:
	buf[0] = SM_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = CE_OK;
	buf[3] = 0x11;
	memcpy(&buf[4],&gAppSn[6],4);
	memcpy(&buf[8],gAppSn,2);
	buf[10] = gCardType;
	memcpy(&buf[11],gCardExpiredDate,4);
	udword_to_buf_reverse(gCardPurse,&buf[15]);
	buf[19] = *((char *)&gPurseOffset+LONG_HIGH0);
	buf[20] = *((char *)&gPurseOffset+LONG_HIGH1);
	buf[21] = *((char *)&gPurseOffset+LONG_HIGH2);
//buf[22] = gCardStatus;
	buf[22] = 0x02;
	uword_to_buf(gCardCnt,&buf[23]);
	buf[25] = cardversion;
	memcpy(&buf[26],info,5);
	memcpy(&buf[31],&leisure[2],3);
	memcpy(&buf[34],leisure,2);
	buf[36] = gMetroIoFlag;
	memcpy(&buf[37],&gAppSn[2],4);
	buf[41] = gAluBacon;
	memcpy(&buf[42],StartUseDay,4);
	buf[46] = gYaJin;
	if(gCardStatus&0x80)
	{
		buf[47] = 0x80;
	}
	else
	{
		buf[48] = 0;
	}			
	counter = 49;
	shmt_prot_send(buf,counter);
//
	if(gCardStatus&0x80)
	{
		gCardStatus = M1LOCKED;
	}
	if(memcmp(gAppSn,"\x21\x53",2)==0)
	{
		gCardStatus = M1LOCKED;
	}
	if((memcmp(gAppSn,"\x20\x00",2)!=0)&&(memcmp(gAppSn,"\x21\x53",2)!=0)&&(memcmp(gCityCode,"\x20\x00",2)!=0))
	//if((memcmp(gAppSn,"\x20\x00",2)!=0)&&(memcmp(gCityCode,"\x20\x00",2)!=0))//2015/9/15 10:02:55�����Ϻ��� ������Ϊ��ؿ�
	{
		gNotShCard = 0x55;//外地卡
	}
	else
	{
		gNotShCard = 0x00;//上海卡或昆山卡
	}
	
	buf[0] = SM_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = CE_OK;
	buf[3] = 0x12;
	if(check_card_get_b32()) goto label_err_remove;
	memcpy(&buf[4],gB32,16);
	counter = 20;
	shmt_prot_send(buf,counter);
//
	buf[0] = SM_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = CE_OK;
	buf[3] = 0x93;
	if(check_card_get_b33()) goto label_err_remove;
	memcpy(&buf[4],gB33,16);
	counter = 20;
	shmt_prot_send(buf,counter);
	gStatus=TRANS_STATUS;
	set_auto_abort_flag();	//20100210add
	gGetCardInfoStep = 0xff;
	return;

//
label_err_remove:
	check_card_err_rpt(CE_CARDREMOVED);
	gStatus=IDLE_STATUS;
//
	set_auto_abort_flag();  //20110216add
	gGetCardInfoStep = 0xff;
	return;		
}	

/*===================================================================================
函数：
功能：
=====================================================================================*/
void not_bom_info_rpt(void)
{
UBYTE buf[64];
UBYTE counter;
UBYTE index;
UBYTE *buf_ptr;
UBYTE tbuf[20];
UBYTE flag;
UBYTE ret;

	memset(buf,0,64);
	//printf("not_bom_info_rpt...gCPUflag=%02x\n",gCPUflag);
	if(gCPUflag == 0x55) goto label_cpu;	
		
	buf[0] = SM_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = CE_OK;
	buf[3] = 0x01;
	memcpy(&buf[4],gThisCardSnr,4);
	buf[8] = gCityCode0;
	buf[9] = gCityCode1;
	buf[10] = gCardType;
	memcpy(&buf[11],gCardExpiredDate,4);
	buf[15] = *((char *)&gCardLastRemain+LONG_HIGH0);
	buf[16] = *((char *)&gCardLastRemain+LONG_HIGH1);
	buf[17] = *((char *)&gCardLastRemain+LONG_HIGH2);
	buf[18] = gCardStatus;
	if(ReadMode==0x00)
	{
		memset(&buf[19],0,4);
		counter = 23;
	}
		  
label_send_b32_b33:		  
	if((ReadMode==0x80)||(ReadMode==0x90)||(ReadMode==0x02))
	{
		index = 19;
		if(NormalCard==CNTCARD){
		  buf[index++]= DayToAllow;
		} 
	  	uword_to_buf(gCardCnt,&buf[index]); index+=2;
		buf[index++] = cardversion;//22:卡版本号
		if(NormalCard==CNTCARD)//上海计次卡
		{
			if(UseType==0){
				memcpy(&buf[index],&StartUseDay[1],3); 
			}
			else{
				memcpy(&buf[index],&FirstDay[1],3); 
			}	  
			index += 3;	  
		}
		//printf("\n\nInfo--gBFirst...:%02x\n",gBFirst);
		switch(gBFirst)
		{
			case 32:	
				if(check_card_get_b33()) 
				{
					gDebugStep = 0x10a0;
					goto label_err_remove;
				}
				buf_ptr = gB33;
				break;
			case 33:		  
				if(check_card_get_b32()) 
				{
					gDebugStep = 0x10a1;
					goto label_err_remove;
				}
				if(gMode==ENTRY_MODE)
				{
					if(memcmp(gB32,"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff",16)==0)
					{
						ee_read(ADDR_CHECK_B32,4,gB32);
						if(memcmp(gThisCardSnr,gB32,4)==0)
						{
							ee_read(ADDR_CHECK_B32+4,16,gB32);
							lrc3_gen(gB32,16);
							memcpy(gB32BK,gB32,20);
						}
					}
				} 
				buf_ptr = gB32;
				break;
			default:
				memset(tbuf,0,20);
				buf_ptr = tbuf;
				break;
		}		  
		if(gNotShCard == 0x55) //2011-3-29 16:19:09
		{
			if(gBFirst == 32) memcpy(&buf[index],&gRecCpu0017[33],3);//进站设备编码
			else memcpy(&buf[index],&gRecCpu0017[33],3);//出站设备编码
		}
		else 
		{
			if(NormalCard==COMMCARD){ 
				memcpy(&buf[index],&buf_ptr[6],3); 
			}
			else{
				buf[index++] = buf_ptr[6];
				buf[index++] = buf_ptr[7];
				buf[index++] = buf_ptr[9];
			}
		}	
		//普通卡
		counter = 30;
		if(NormalCard==COMMCARD){ 	
		  //if(gMode==ENTRY_MODE){ //diabled by xuxu,20100212
			memcpy(&buf[25],info,5);
			if(gCPUflag==0x55){
				memcpy(&buf[30],&gAppSn[2],4);
				buf[34] = gAluBacon;
				counter = 35;
			}
					//}  
		}
	}
	if(ReadMode==0x02)
	{
		 buf[3]|=0x80;
	}
	//科委新增
#ifdef	STC
	memcpy(&buf[counter], &gB34[1], 4);
	counter += 4;
#endif
	if(memcmp(gThisCardSnr,MUTE_CARD_SNR,4) != 0)
	{	 
  		shmt_prot_send(buf,counter);
	}
	
	if(ReadMode==0x02) 
	{
		gStatus=TRANS_STATUS;
		set_auto_abort_flag();	//20100210add
		gGetCardInfoStep = 0xff;
		return;
	}
	
	//	
	buf[0] = SM_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = CE_OK;
	buf[3] = 0x82;
	if(gCPUflag==0x55) 	buf[3] = 0x92;
	flag = 0;	
	switch(gBFirst)
	{
			case 32:
			    if(check_card_get_b32()) goto label_err_remove;
			    memcpy(&buf[4],gB32,16);	
					ret=buf[4]&0xc0;
			    if((lrc_chk2(&buf[4],15)!=0)||(ret != 0x40))
			    {
				     memset(&buf[4],0,16);
				     buf[4]=0x41;
				     buf[19]=0x41;
					   recordtime[0]=0;
					   recordtime[1]=0;
					   flag = 1;
				  }			
			    break;
			case 33:     
			    if(check_card_get_b33()) goto label_err_remove;
			    memcpy(&buf[4],gB33,16);	
	        ret=buf[4]&0xc0;
			    if((lrc_chk2(&buf[4],15)!=0)||(ret != 0x80))
			    {
				     memset(&buf[4],0,16);
				     buf[4]=0x81;
				     buf[19]=0x81;
					   recordtime[0]=0;
					   recordtime[1]=0;
				     flag = 1;
				  }			
			    break;
			default:   
				  ret=get_metro_rec(&buf[4]);
				  if(ret!=0)
				  {
					   if(ret==0x55) goto label_err_remove;
					   memset(&buf[4],0,16);
					   buf[4]=0x81;	//出站是81进站是41 
				     buf[19]=0x81;
					   recordtime[0]=0;
					   recordtime[1]=0;
				     flag = 1;
					}
					break;
	}		
	if(flag==0)
	{
			recordsum[0]=buf[18];
			recordsum[1]=buf[17];
			recordsum[2]=buf[16];
			recordcnt=buf[15];
			recordtime[0]=buf[5];
			recordtime[1]=buf[6];
	}
	counter = 20;
	//2011-3-29 16:19:09
	if((gNotShCard == 0x55)&&(memcmp(&gRecCpu0017[5],"\x20\x00",2) != 0))
	{
		memset(&buf[4],0,16);
		buf[4] = 0x81;
		buf[19] = 0x81;
	}
	if(memcmp(gThisCardSnr,MUTE_CARD_SNR,4) != 0)
	{	 
  	shmt_prot_send(buf,counter);
  }
	gStatus=TRANS_STATUS;
	set_auto_abort_flag();	//20100210add
	gGetCardInfoStep = 0xff;
	return;  

//-------------------------------------
label_cpu:
	buf[0] = SM_READCARD;//0x30
	buf[1] = gReadCmdTimes;
	buf[2] = CE_OK;
	buf[3] = 0x11;
	memcpy(&buf[4],&gAppSn[6],4);
	memcpy(&buf[8],gAppSn,2);
	buf[10] = gCardType;
	memcpy(&buf[11],gCardExpiredDate,4);
	buf[15] = *((char *)&gCardLastRemain+LONG_HIGH0);
	buf[16] = *((char *)&gCardLastRemain+LONG_HIGH1);
	buf[17] = *((char *)&gCardLastRemain+LONG_HIGH2);
	//buf[18] = gCardStatus;
	buf[18] = 0x02;
				
	NormalCard = COMMCARD;//COMMCARD 0x53:上海普通卡
	if(gCardStatus&0x80)
	{
		gCardStatus = M1LOCKED;
	}
	if(memcmp(gAppSn,"\x21\x53",2)==0)
	{
		gCardStatus = M1LOCKED;
	}
	if((cardversion<10)&&(cardversion!=2))
	{
		gCardStatus = 0;
	}
	if((memcmp(gAppSn,"\x20\x00",2)!=0)&&(memcmp(gAppSn,"\x21\x53",2)!=0)&&(memcmp(gCityCode,"\x20\x00",2)!=0))
	//if((memcmp(gAppSn,"\x20\x00",2)!=0)&&(memcmp(gCityCode,"\x20\x00",2)!=0))//2015/9/15 10:02:55除了上海卡 其他都为外地卡
	{
		gNotShCard = 0x55;//外地卡
	}
	else gNotShCard = 0x00;//上海卡或昆山卡

	goto label_send_b32_b33;

//
label_err_remove:
	check_card_err_rpt(CE_CARDREMOVED);
	gStatus=IDLE_STATUS;
	//
	set_auto_abort_flag();  //20110216add
	gGetCardInfoStep = 0xff;
	return;	
}

/*===================================================================================
������
���ܣ�
=====================================================================================*/
void qr_main()
{
int rtn;
long len;
unsigned char data[1000];
UBYTE	ec, trytimes;

	trytimes = 0;
label_rereadqr:
	memset(data,0,sizeof(data)/sizeof(char));
	rtn = scancontrol(data ,&len);
	printf("scancontrol return %d\n", rtn);
	if(rtn > 0)
	{
		if(rtn == 0xff)
			return;
			
		trytimes += 1;
		printf("trytimes %d\n", trytimes);
		if(trytimes < 4)
			goto label_rereadqr;
		return ;
	}
	gTaskBusyFlag = 1;
	if(rtn < 0)
	{
		check_card_err_rpt(CE_READERROR);
		gStatus=IDLE_STATUS;
		set_auto_abort_flag();
		return ;
	}
	//进行备份比较
	if( (memcmp(&data[7], &qrDataBak[7], 14) == 0) && (memcmp(&data[28], &qrDataBak[28], 25) == 0) )
	{
		gTaskBusyFlag = 0;
		memset(qrDataBak, 0x00, 25);
		return ;
	}
	rtn = getinfo_QR(data,len);
	
	//memset(data ,0x00, 100);
	//根据要求，满足移动条件下不通知上位机，重新继续寻卡流程
	if(rtn == CE_NOFUNCTION)
	{
		gTaskBusyFlag = 0;
		return ;
	}
	if(rtn != 0)
	{
		ec = gResponseStatus;
		check_card_err_rpt(ec);
		gStatus=IDLE_STATUS;
		set_auto_abort_flag();
		return ;
	}
	
	gStatus = TRANS_STATUS;
	return ;
}

/*===================================================================================
������
���ܣ�
=====================================================================================*/
void *pthusermain()
{

#ifdef DEBUG_PRINT
	printf("usermain thread start\n");
#endif
#ifdef SCANNERPROTOCOL
	delay_ms(3000);
	ScannerProtocol();
#endif
	for(;;)
	{
		sem_wait(&g_usermainwait);
		user_main(); 
		if(gTaskBusyFlag == 0){
			qr_main();
		}
		//check_245g();
		if(gTaskBusyFlag == 0 )
			sem_post(&g_usermainwait); 
	}
}

//void *pthusermainzte()
//{
////2.4Gģ�����
//#ifdef DEBUG_PRINT
//	printf("usermain_zte thread start\n");
//#endif
//	for(;;)
//	{
//		sem_wait(&g_usermainztewait);
//		//reader_test_24G();
//		if(bgTest24GFlag == 0)
//			sem_post(&g_usermainztewait);
//		
//	}
//}


//end of file

/*===================================================================================
函数：读卡器与扫描头协议测试
功能：测试读卡器扫描头协议
=====================================================================================*/
void ScannerProtocol(){
		printf("\n#####Fudan Comm Scanner Update Begin#####: \n");
		FILE * intFile;
		char filename[100] = "Scanner";
		long i, j, k;
		
		//if(0 != FileisExist("./",)
		
		
		/*
		unsigned char cmdbuf[128];
		unsigned char bcc = 0;
		int i, j;
		printf("\n#####Fudan Comm ScannerProtocol cmd Test Begin#####: \n");
		char respbuf[512];
		unsigned short len;
		

		//cmd01
		memset(cmdbuf, 128, 0x00);
		cmdbuf[0] = 0x02;//起始 报文头
		cmdbuf[1] = 0x00;//报文长度（包含CLA~DATA域的数据长度)//高字节
		cmdbuf[2] = 0x02;//低字节
		cmdbuf[3] = 0xb0;//命令代码//高字节
		cmdbuf[4] = 0x00;//低字节
		//校验位
		for(j = 3; j <= 4; j++){
			bcc ^= cmdbuf[j];
		}
		cmdbuf[5] = bcc;
		cmdbuf[6] = 0x03;
		printf("Fudan Comm ScannerProtocol cmd0 send: ");
		for(j = 0; j < 7; j++)
		{
			printf("%02X", cmdbuf[j]);
		}	
		printf("\n");
		
		fdwd_comm(cmdbuf, 7, respbuf, &len, 10);
		printf("Fudan Comm ScannerProtocol cmd0 receive: ");
		for(j = 0; j < 2; j++)
		{
			printf("%02X", respbuf[j]);
		}	
		printf("\n");
		memset(respbuf, 512, 0x00);
		//writecom(csc_comm, cmd0buf, len0);
		delay_ms(3000);

		//cmd02
		memset(cmdbuf, 128, 0x00);
		cmdbuf[0] = 0x02;//起始 报文头
		cmdbuf[1] = 0x00;//报文长度（包含CLA~DATA域的数据长度)//高字节
		cmdbuf[2] = 0x42;//低字节
		cmdbuf[3] = 0xb0;//命令代码//高字节
		cmdbuf[4] = 0x01;//低字节
		memset(&cmdbuf[5], 0, 64);
		bcc = 0;
		for(j = 3; j <= 68; j++){
			bcc ^= cmdbuf[j];
		}
		cmdbuf[69] = bcc;
		cmdbuf[70] = 0x03;
		printf("Fudan Comm ScannerProtocol cmd1 send: ");
		for(j = 0; j < 71; j++)
		{
			printf("%02X", cmdbuf[j]);
		}	
		printf("\n");
		fdwd_comm(cmdbuf, 71, respbuf, &len, 10);
		printf("Fudan Comm ScannerProtocol cmd1 receive: ");
		for(j = 0; j < 2; j++)
		{
			printf("%02X", respbuf[j]);
		}	
		printf("\n");
		memset(respbuf, 512, 0x00);
		//writecom(csc_comm, cmd1buf, len1);
		delay_ms(3000);
		
		//cmd03
		memset(cmdbuf, 128, 0x00);
		cmdbuf[0] = 0x02;//起始 报文头
		cmdbuf[1] = 0x00;//报文长度（包含CLA~DATA域的数据长度)//高字节
		cmdbuf[2] = 0x06;//低字节
		cmdbuf[3] = 0xb0;//命令代码//高字节
		cmdbuf[4] = 0x02;//低字节
		memset(&cmdbuf[5], 0, 4);
		bcc = 0;
		for(j = 3; j <= 8; j++){
			bcc ^= cmdbuf[j];
		}
		cmdbuf[9] = bcc;
		cmdbuf[10] = 0x03;
		printf("Fudan Comm ScannerProtocol cmd2 send: ");
		for(j = 0; j < 11; j++)
		{
			printf("%02X", cmdbuf[j]);
		}	
		printf("\n");
		fdwd_comm(cmdbuf, 11, respbuf, &len, 10);
		printf("Fudan Comm ScannerProtocol cmd2 receive: ");
		for(j = 0; j < 512; j++)
		{
			printf("%02X", respbuf[j]);
		}	
		printf("\n");
		memset(respbuf, 512, 0x00);
		//writecom(csc_comm, cmd2buf, len2);
		delay_ms(3000);
		
		//cmd04
		memset(cmdbuf, 128, 0x00);
		cmdbuf[0] = 0x02;//起始 报文头
		cmdbuf[1] = 0x00;//报文长度（包含CLA~DATA域的数据长度)//高字节
		cmdbuf[2] = 0x02;//低字节
		cmdbuf[3] = 0xb0;//命令代码//高字节
		cmdbuf[4] = 0x03;//低字节
		//校验位
		for(j = 3; j <= 4; j++){
			bcc ^= cmdbuf[j];
		}
		cmdbuf[5] = bcc;
		cmdbuf[6] = 0x03;
		printf("Fudan Comm ScannerProtocol cmd3 send: ");
		for(j = 0; j < 7; j++)
		{
			printf("%02X", cmdbuf[j]);
		}	
		printf("\n");
		fdwd_comm(cmdbuf, 7, respbuf, &len, 10);
		printf("Fudan Comm ScannerProtocol cmd3 receive: ");
		for(j = 0; j < 19; j++)
		{
			printf("%02X", respbuf[j]);
		}	
		printf("\n");
		memset(respbuf, 512, 0x00);
		//writecom(csc_comm, cmd3buf, len3);
		delay_ms(3000);
		
		//cmd05
		memset(cmdbuf, 128, 0x00);
		cmdbuf[0] = 0x02;//起始 报文头
		cmdbuf[1] = 0x00;//报文长度（包含CLA~DATA域的数据长度)//高字节	
		cmdbuf[2] = 0x02;//低字节
		cmdbuf[3] = 0xb0;//命令代码//高字节
		cmdbuf[4] = 0x04;//低字节
		//校验位
		for(j = 3; j <= 4; j++){
			bcc ^= cmdbuf[j];
		}
		cmdbuf[5] = bcc;
		cmdbuf[6] = 0x03;
		printf("Fudan Comm ScannerProtocol cmd4 send: ");
		for(j = 0; j < 7; j++)
		{
			printf("%02X", cmdbuf[j]);
		}	
		printf("\n");
		fdwd_comm(cmdbuf, 7, respbuf, &len, 10);
		printf("Fudan Comm ScannerProtocol cmd4 receive: ");
		for(j = 0; j < 66; j++)
		{
			printf("%02X", respbuf[j]);
		}	
		printf("\n");
		memset(respbuf, 512, 0x00);
		//writecom(csc_comm, cmd4buf, len4);
		delay_ms(3000);
		
		//cmd06
		memset(cmdbuf, 128, 0x00);
		cmdbuf[0] = 0x02;//起始 报文头
		cmdbuf[1] = 0x00;//报文长度（包含CLA~DATA域的数据长度)//高字节	
		cmdbuf[2] = 0x02;//低字节
		cmdbuf[3] = 0xb0;//命令代码//高字节
		cmdbuf[4] = 0x05;//低字节
		//校验位
		for(j = 3; j <= 4; j++){
			bcc ^= cmdbuf[j];
		}
		cmdbuf[5] = bcc;
		cmdbuf[6] = 0x03;
		printf("Fudan Comm ScannerProtocol cmd5 send: ");
		for(j = 0; j < 7; j++)
		{
			printf("%02X", cmdbuf[j]);
		}	
		printf("\n");
		fdwd_comm(cmdbuf, 7, respbuf, &len, 10);
		printf("Fudan Comm ScannerProtocol cmd5 receive: ");
		for(j = 0; j < 2; j++)
		{
			printf("%02X", respbuf[j]);
		}	
		printf("\n");
		memset(respbuf, 512, 0x00);
		//writecom(csc_comm, cmd5buf, len5);
		delay_ms(3000);
		//传输
		
		
		
		//cmd07
		memset(cmdbuf, 128, 0x00);
		cmdbuf[0] = 0x02;//起始 报文头
		cmdbuf[1] = 0x00;//报文长度（包含CLA~DATA域的数据长度)//高字节	
		cmdbuf[2] = 0x00;//透传指令低字节
		cmdbuf[3] = 0x00;//命令代码//高字节
		cmdbuf[4] = 0x00;//低字节
		//校验位
		for(j = 3; j <= 4; j++){
			bcc ^= cmdbuf[j];
		}
		cmdbuf[5] = bcc;
		cmdbuf[6] = 0x03;
		printf("Fudan Comm ScannerProtocol cmd6 send: ");
		for(j = 0; j < 7; j++)
		{
			printf("%02X", cmdbuf[j]);
		}	
		printf("\n");
		fdwd_comm(cmdbuf, 7, respbuf, &len, 10);
		printf("Fudan Comm ScannerProtocol cmd6 receive: ");
		for(j = 0; j < 512; j++)
		{
			printf("%02X", respbuf[j]);
		}	
		printf("\n");
		memset(respbuf, 512, 0x00);
		//writecom(csc_comm, cmd6buf, len6);
		delay_ms(3000);
		printf("\n#####Fudan Comm ScannerProtocol cmd Test End#####\n");
		*/
}
#endif
