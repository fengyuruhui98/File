//user_main.c
//�û�Ӧ���ڴ˺���ʵ��

#ifndef _USER_MAIN_C_
#define _USER_MAIN_C_
//start of file

//#define _DEBUG_READ_
//#define _SPEED_TEST_
//#define _DEBUG_STEP_OUT_

/*=============================================================================================
������user_main
���ܣ��û�������
ע�⣺�ڴ˺����в���ʹ����ѭ����ʽ
===============================================================================================*/
#ifdef DEBUG_PRINT
#define _DEBUG_user_main_
#endif
void user_main(void)
{
	UBYTE ret,para;
	int counter;
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

	counter=0;
	gDebugStep=0;
	mcml_pwr_on();
	//set_timeout(6000);
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
	  //set_timeout(100000);
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
//Ѱ�ҵ���Ƭ 
	gDebugStep=1;
	gTaskBusyFlag = 1;
	if(memcmp(buf,"\x04\x00",2) == 0){
	 	if((UBYTE)ReadMode==(UBYTE)0x90) goto label_cpu;
	 	if((UBYTE)ReadMode==(UBYTE)0x91) goto label_cpu;
	 	goto label_m1;	
	}
	//2013/9/16 11:36:15 �¿�
	if(memcmp(buf,"\x01\x00",2) == 0){
		if((UBYTE)ReadMode==(UBYTE)0x90) goto label_cpu;
		if((UBYTE)ReadMode==(UBYTE)0x91) goto label_cpu;
	 	goto label_m1;	
	}
	//
	if(memcmp(buf,"\x08\x00",2) == 0){//2011-1-19 15:32:37 ��������
	 	if((UBYTE)ReadMode==(UBYTE)0x90) goto label_cpu;
	 	if((UBYTE)ReadMode==(UBYTE)0x91) goto label_cpu;
	 	goto label_m1;	
	}
	if(memcmp(buf,"\x02\x00",2) == 0){//2013/5/22 10:12:00 �ַ��ֻ�֧��
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

//CPU������--------------------------------------------------
label_cpu:
	ret = getcardinfo_CPU();
	if(ret == CE_OK_PF_MOBILE) 
	{
		goto label_exit;
	}
	if(ret == CE_UNKNOWN) goto label_only_exit;
	if(ret==0) 
	{
		goto label_info_rpt;
	}
	//BOM��Ӧ��δ��������
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
	if(((ReadMode == 0x90)||(ReadMode == 0x91))&&(gCPUflag == 0x00)) goto label_m1;
//�м���󣬼���Ѱ��
	if(gGetCardInfoStep==0){
		 mcml_pwr_off();
//		 delay_ms(2);
		 goto label_only_exit;
	 }	
	ec = gResponseStatus;
	goto label_ec_rpt;

//M1������---------------------------------	
label_m1:
	if(bgSHPsamValid!=1){
	  ec = CE_NCSCSAM;
	  goto label_ec_rpt;
  }	
	bgSHpsamIndex=SAMLOCATION_5;			//ָ��SAM����
	ret=getcardinfo();	
	if(ret==0) 
	{
		gCardFamily = FAMILY_M1;
		goto label_info_rpt;
	}
	if(((ReadMode == 0x80)||(ReadMode == 0x81))&&(gCPUflag == 0x55)) goto label_cpu;
	if(gGetCardInfoStep!=0){			
	  ec = gResponseStatus;
	  goto label_ec_rpt;
  }
//�м���󣬼���Ѱ��
goto label_only_exit;
  
//����Ʊ����---------------------------------	
label_metro:
	if(bgSmPsamValid!=1){
		ec = CE_STSAM;
		goto label_ec_rpt;
	}
	bgSmpsamIndex=SAMLOCATION_4;			//ָ��SAM����
	ret=hh_getcardinf(para);
	if(ret!=0){
		if(gGetCardInfoStep != 0xff) goto label_only_exit; //�м���󣬼���Ѱ��
	  ec = gResponseStatus;
	  goto label_ec_rpt;
  }
	check_card_metro_info_rpt(); 
	gCardFamily = FAMILY_SJT; 
	goto label_exit;

//�������
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
������
���ܣ�
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
������
���ܣ�
=====================================================================================*/
void check_card_metro_info_rpt(void)
{
	UBYTE buf[64];
	UBYTE counter;
	int i;
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
#ifdef	_DEBUG_READ_
	debug_printf("SJT Read:");
	for (i = 0; i < counter; i++)
	{
		debug_printf("%02X ", buf[i]);
	}
	debug_printf("\n");
#endif	
	gStatus=TRANS_STATUS;
	set_auto_abort_flag();	//20100210add
	gGetCardInfoStep = 0xff;
	return;		
}	

/*===================================================================================
������
���ܣ�
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
������
���ܣ�
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
������
���ܣ�
=====================================================================================*/
void bom_info_rpt(void)
{
UBYTE buf[64], outbuf[100];
UBYTE counter;
int ret;
	
	memset(buf,0,64);
	
	if(gCPUflag == 0x55) goto label_cpu;
	//M1��
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
	buf[21] = cardversion;  //���汾�ţ������ţ�
	if(NormalCard==COMMCARD){
		memcpy(&buf[22],info,5);//������ҵ��Ϣ������ʱ��
		memcpy(&buf[27],&leisure[2],3);//�ۼƳ˳����
  }
	else{
		buf[22] = DayToAllow;
		memcpy(&buf[23],&StartUseDay[1],3);
		memset(&buf[26],0,4);
  }
	memcpy(&buf[30],leisure,2);//�ۼƳ˳�����
	buf[32] = gMetroIoFlagBk;//���ݽ���״̬1
	buf[33] = gMetroIoFlagBkValid;//���ݽ���״̬2
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
	if(check_card_get_b33()) goto label_err_remove;
	if(gBFirst == 32)
		memcpy(&buf[4],gB32,16);
	else
	memcpy(&buf[4], gB33, 16);
	counter = 20;
	shmt_prot_send(buf,counter);
	//			

	buf[0] = SM_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = CE_OK;
	buf[3] = 0x83;
	if(gBFirst == 32)
		memcpy(&buf[4], gB33, 16);
	else
		memcpy(&buf[4],gB32,16);
	counter = 20;
	shmt_prot_send(buf,counter);
	gStatus=TRANS_STATUS;
//
	set_auto_abort_flag();	//20100210add
	gGetCardInfoStep = 0xff;
	return;

//CPU��------------------------------
label_cpu:
//	memcpy(buf,"\x00\xb0\x95\x00\x1e",5);
//	ret = mifpro_icmd0(5,buf,outbuf);
//	if(ret<0)
//	{
//		buf[0] = SM_READCARD;
//		buf[1] = gReadCmdTimes;
//		buf[2] = CE_READERROR;
//		shmt_prot_send(buf,counter);
//		return ;
//	}


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
	//if(gCardStatus&0x80)
	{
		buf[47] = gApplicationTypeTag;
	}
	//else
	//{
	//	buf[47] = 0;
	//}			
	counter = 48;
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
		gNotShCard = 0x55;//��ؿ�
	}
	else
	{
		gNotShCard = 0x00;//�Ϻ�������ɽ��
	}

	buf[0] = SM_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = CE_OK;
	buf[3] = 0x12;
	if(check_card_get_b32()) goto label_err_remove;
	if(check_card_get_b33()) goto label_err_remove;
	
	if(gBFirst == 32)
		memcpy(&buf[4],gB32,16);
	else
		memcpy(&buf[4],gB33, 16);
	counter = 20;
	shmt_prot_send(buf,counter);
//

	buf[0] = SM_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = CE_OK;
	buf[3] = 0x93;
	if(gBFirst == 32)
		memcpy(&buf[4], gB33, 16);
	else
		memcpy(&buf[4],gB32,16);
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
������
���ܣ�
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
		buf[index++] = cardversion;
		if(NormalCard==CNTCARD)
		{
			if(UseType==0){
				memcpy(&buf[index],&StartUseDay[1],3); 
			}
			else{
				memcpy(&buf[index],&FirstDay[1],3); 
			}	  
			index += 3;	  
		}
		switch(gBFirst)
		{
		case 32:	
			if(check_card_get_b33()) {
				gDebugStep = 0x10a0;
				goto label_err_remove;
			}
			buf_ptr = gB33;
			break;
		case 33:		  
			if(check_card_get_b32()) {
				gDebugStep = 0x10a1;
				goto label_err_remove;
			}
			if(gMode==ENTRY_MODE){
				if(memcmp(gB32,"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff",16)==0){
					ee_read(ADDR_CHECK_B32,4,gB32);
					if(memcmp(gThisCardSnr,gB32,4)==0){
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
			if(gBFirst == 32) memcpy(&buf[index],&gRecCpu0017[33],3);//��վ�豸����
			else memcpy(&buf[index],&gRecCpu0017[33],3);//��վ�豸����
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
		//��ͨ��  
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

	if(memcmp(gThisCardSnr,MUTE_CARD_SNR,4) != 0)
	{	 
  		shmt_prot_send(buf,counter);
	}
	
	if(ReadMode==0x02) {
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
	switch(gBFirst){
	case 32:
		if(check_card_get_b32()) goto label_err_remove;
		memcpy(&buf[4],gB32,16);	
		ret=buf[4]&0xc0;
		if((lrc_chk2(&buf[4],15)!=0)||(ret != 0x40)){
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
		if((lrc_chk2(&buf[4],15)!=0)||(ret != 0x80)){
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
		if(ret!=0){
			if(ret==0x55) goto label_err_remove;
			memset(&buf[4],0,16);
			buf[4]=0x81;	//��վ��81��վ��41 
			buf[19]=0x81;
			recordtime[0]=0;
			recordtime[1]=0;
			flag = 1;
		}
		break;
	}		
	if(flag==0){
			recordsum[0]=buf[18];
			recordsum[1]=buf[17];
			recordsum[2]=buf[16];
			recordcnt=buf[15];
			recordtime[0]=buf[5];
			recordtime[1]=buf[6];
	}
	counter = 20;
	//2011-3-29 16:19:09
	if((gNotShCard == 0x55)&&(memcmp(&gRecCpu0017[5],"\x20\x00",2) != 0)) {
		memset(&buf[4],0,16);
		buf[4] = 0x81;
		buf[19] = 0x81;
	}

	if(memcmp(gThisCardSnr,MUTE_CARD_SNR,4) != 0){	 
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
				
	NormalCard = COMMCARD;
	if(gCardStatus&0x80){
		gCardStatus = M1LOCKED;
		}
	if(memcmp(gAppSn,"\x21\x53",2)==0){
		gCardStatus = M1LOCKED;
	}
	if((cardversion<10)&&(cardversion!=2)){
		gCardStatus = 0;
	}
	//if((memcmp(gAppSn,"\x20\x00",2)!=0)&&(memcmp(gAppSn,"\x21\x53",2)!=0)&&(memcmp(gCityCode,"\x20\x00",2)!=0))
	if((memcmp(gAppSn,"\x20\x00",2)!=0)&&(memcmp(gCityCode,"\x20\x00",2)!=0))//2015/9/15 10:02:55�����Ϻ��� ������Ϊ��ؿ�
	{
		gNotShCard = 0x55;//��ؿ�
	}
	else gNotShCard = 0x00;//�Ϻ�������ɽ��

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

void qr_main()
{
int rtn;
long len;
unsigned char data[1000];
UBYTE	ec, trytimes;

	trytimes = 0;
label_rereadqr:
	rtn = scancontrol(data ,&len);

	if(rtn > 0)
	{
		//��ͨ������Ҫ������ѯ��ά��
		if(rtn == 0xff)
			return ;
		//���򣬳��Զ���ض�
		trytimes +=1;
		if(trytimes < 8)
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
	rtn = getinfo_QR(data, len);
	//20220227 zdd BOM����Ҫ��������
	//����Ҫ�������ƶ������²�֪ͨ��λ�������¼���Ѱ������
	//if(rtn == CE_NOFUNCTION)
	//{
		//gTaskBusyFlag = 0;
		//return ;
	//}
	if(rtn != 0)
	{
		ec = gResponseStatus;
		check_card_err_rpt(ec);
		gStatus=IDLE_STATUS;
		set_auto_abort_flag();
		return ;
	}
	gCardFamily = FAMILY_QR;
	gStatus = TRANS_STATUS;
	return ;
}

void *pthusermain()
{
	#ifdef DEBUG_PRINT
	printf("usermain thread start\n");
#endif
	for(;;)
	{
		sem_wait(&g_usermainwait);
		user_main(); 
		if(gTaskBusyFlag == 0){
			//printf("\nzddinQR\n");
			qr_main();
		}
		//check_245g();
		if(gTaskBusyFlag == 0 )
			sem_post(&g_usermainwait); 
	}
/*UBYTE	buf[100];
UBYTE	i;

#ifdef DEBUG_PRINT
	printf("usermain thread start\n");
#endif
	for(;;)
	{
		sem_wait(&g_usermainwait);
		i = 0;
label_retry:
		user_main(); 
		if(gTaskBusyFlag == 0)
			qr_main();
		if(gTaskBusyFlag == 0)
		{
			if( i < 8 )
			{
				i++;
				goto label_retry;
			}
			i = 0;
		}
		//check_245g();
		//printf("Readmode %02x\n", ReadMode);
		if((ReadMode & 0x01) == 0x00)
		{
			if(gTaskBusyFlag == 0 )
				sem_post(&g_usermainwait); 
		}else
		{
			if(gTaskBusyFlag == 0)
			{
				gStatus=IDLE_STATUS;
				buf[0] = SM_READCARD;
				buf[1] = gReadCmdTimes;
				buf[2] = CE_NOCARD;
				shmt_prot_send(buf,3); 
			}
		}
	}*/
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
#endif
