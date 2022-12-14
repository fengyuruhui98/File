//SH_CPU_TRANS.c
#ifndef _SH_CPU_TRANS_C_
#define _sh_CPU_TRANS_C_

#define TEST_LOCK
//#define	DEBUG_TIME_CPU
#define DEBUG_RECORD

UBYTE gPosID[6];
UBYTE gKeyVersion;
UBYTE gAppSn[10];
UBYTE gYaJin;
UBYTE gKeyBacon;				//密钥标志
UBYTE gAluBacon;				//算法标志
UBYTE ICC[4];						//伪随机数
UBYTE gB29Buf[20];
UBYTE gPSAMFileFlag;
UDWORD gPurseOffset;
UBYTE gCPUValid;
UBYTE gNotShCard;
//UBYTE Rand[8];
UBYTE gCityCode[2];
UBYTE gRecCpu0017[48];

//2013/11/20 9:22:26
UBYTE gPPSMode = 0; //1：PPS关 0：PPS开

//2018/07/30
UBYTE gODAData[100];//ODA数据

UBYTE JTBData[20];//交通部记录缓存

/*****************************************************
函数：getcardinfo_CPU
功能：读上海CPU交通卡的信息
*****************************************************/
//#ifdef DEBUG_PRINT
//#define _DEBUG_GETCARDINFO_CPU_
//#define _GETCARDINFO_CPU_
//#endif
char getcardinfo_CPU(void)
{
	int ret,i;
	UBYTE buf[64], ppsbuf[48];
	UBYTE outbuf[300];
	UBYTE cnt;
	UBYTE tbuf[80];
	UBYTE temp_IssuerCode[8];//发卡机构代码：用于住建部白名单判断
	
		//2013/11/18 10:34:52
	UBYTE bGjFlag = 0;
	UBYTE bPfFlag = 0;  
	
	#ifdef _debug_test_
	UDWORD	tint0=0,tint1=0;
	#endif
	
	gCPUValid = 0x55;
	gCPUflag = 0;
	gNotShCard = 0x00;
	gM1Size0x53Flag = 0;

	#ifdef _GETCARDINFO_CPU_
	debug_printf("\x0d\x0a getcardinfo_CPU 1\n");
	#endif 
	#ifdef _debug_test_
	tint0 = GetTickcount();
	#endif
	gDebugStep = 0x3079;
	
	
	//为何request
	//ret=mcml_request(PICC_REQALL,buf);//PICC_REQSTD
	for(i = 0;i < 3; i++)
	{
		set_timeout(6000);
		ret=mcml_request(PICC_REQALL,buf);//PICC_REQSTD
		#ifdef _GETCARDINFO_CPU_
			printf("mcml_request...ret= %d\n",ret);
		#endif
		if(ret == 0)
			break;
	}

	#ifdef _debug_test_
		tint1 = GetTickcount();
		debug_printf("\n mcml_request(%d) \n",tint1-tint0);
	#endif
	if(ret!=0)
	{
		#ifdef _GETCARDINFO_CPU_
			debug_printf("\x0d\x0a getcardinfo_CPU mcml_request Err\n");
		#endif
		return OP_ERR;
	}
	gThisCardSnr[6] = buf[0];
	gThisCardSnr[7] = buf[1];
	gDebugStep = 0x3080;


	ret=mcml_anticoll(gThisCardSnr);
#ifdef _GETCARDINFO_CPU_
	printf("\nUID 1:");
	for(i=0;i<5;i++)
	{
		printf(" %02x",gThisCardSnr[i]);
	}
	printf("\n");
#endif
	if(ret!=0)
	{
		#ifdef _GETCARDINFO_CPU_
		debug_printf("\x0d\x0a getcardinfo_CPU mcml_anticoll Err\n");
		#endif
		return OP_ERR;
	}
	gDebugStep = 0x3081;
	ret=mcml_select(gThisCardSnr,&cnt);
#ifdef _GETCARDINFO_CPU_
	printf("UID 2:");
	for(i=0;i<5;i++)
	{
		printf(" %02x",gThisCardSnr[i]);
	}
	printf("\n");
#endif
	if(ret!=0)
	{
		#ifdef _GETCARDINFO_CPU_
		debug_printf("\x0d\x0a getcardinfo_CPU mcml_select Err\n");
		#endif 
		return OP_ERR;
	}
	//交易保护：验算MAC2失败时，重新读到该卡时返回14无效卡
	if(memcmp(&temp_uid[0],gThisCardSnr,4) == 0)
	{
		gResponseStatus=CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}

	if(cnt == 0x53)  gM1Size0x53Flag = 1;//2011-3-29 10:46:08
			
	gDebugStep = 0x3082;
	gThisCardSnr[5] = cnt;
	ret = mifpro_ats0(buf);
	if(ret<0)
	{
		#ifdef _GETCARDINFO_CPU_
		debug_printf("getcardinfo_CPU mifpro_ats0 Err\n");
		#endif 
		//mcml_pwr_off();
		return OP_ERR;
	}
#ifdef DEBUG_TIME_CPU
	memcpy(ppsbuf, "\x01\x00\x01", 3);
	memcpy(&ppsbuf[3], buf, 3);
	ReaderResponse(csc_comm, 0x00, 0xF0, ppsbuf, 6);
#endif	
	//卡片pps
#ifdef _GETCARDINFO_CPU_
	debug_printf("gPPS_CPU buf=%02X %02X %02X\n",buf[0],buf[1],buf[2]);
#endif

	//是否PPS	 
	 if((gPPSMode == 1) || (gMode == BOM_MODE))
	 {
	 		goto card_wo_pps;
	 }
	 
#ifdef CARD_PPS_ON	
	if((buf[0] > 1) && (buf[1]&0x10)) 
	{
		if((buf[2]&0x44) == 0x44) 
		{
			//2014/10/17 15:19:13最高只能切到424
			ret = mifpro_pps(RF_PPS1_424K,buf);
			if(ret)
			{
				mcml_pwr_off();
				gDebugStep = 0x8801;
				return OP_ERR;
			}
			ret = mifpro_set_speed(RF_SPEED_424K,RF_SPEED_424K);
			if(ret)
			{
				mcml_pwr_off();
				gDebugStep = 0x8802;
				return OP_ERR;
			}
			#ifdef _GETCARDINFO_CPU_
					debug_printf("424pps_CPU 3\n");
			#endif
			/*
			ret = mifpro_pps(RF_PPS1_848K,buf);
			if(ret)
			{
				mcml_pwr_off();
				gDebugStep = 0x8801;
				return OP_ERR;
			}
			ret = mifpro_set_speed(RF_SPEED_848K,RF_SPEED_848K);
			if(ret)
			{
				mcml_pwr_off();
				gDebugStep = 0x8802;
				return OP_ERR;
			}
			#ifdef _GETCARDINFO_CPU_
					debug_printf("848pps_CPU 3\n");
			#endif
			*/
		}
		else if((buf[2]&0x22) == 0x22) 
		{
			ret = mifpro_pps(RF_PPS1_424K,buf);
			if(ret)
			{
				mcml_pwr_off();
				gDebugStep = 0x8803;
				return OP_ERR;
			}
			ret = mifpro_set_speed(RF_SPEED_424K,RF_SPEED_424K);
			if(ret)
			{
				mcml_pwr_off();
				gDebugStep = 0x8804;
				return OP_ERR;
			}
			#ifdef _GETCARDINFO_CPU_
					debug_printf("424pps_CPU 3\n");
			#endif
		}		
		else if((buf[2]&0x11) == 0x11) 
		{
			ret = mifpro_pps(RF_PPS1_212K,buf);
			if(ret)
			{
				mcml_pwr_off();
				gDebugStep = 0x8805;
				return OP_ERR;
			}
			ret = mifpro_set_speed(RF_SPEED_212K,RF_SPEED_212K);
			if(ret)
			{
				mcml_pwr_off();
				gDebugStep = 0x8806;
				return OP_ERR;
			}
			#ifdef _GETCARDINFO_CPU_
					debug_printf("212pps_CPU 3\n");
			#endif
		}
  }
#endif

#ifdef DEBUG_TIME_CPU
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x02", 3);
#endif	

card_wo_pps:
/*
//  //公交优先还是浦发优先
//  if((buf[0] > 1) && (buf[1]&0x70))
//  {
//  	cnt = (buf[1]&0x70)>>4;
//  	switch(cnt){
//  	   case 7: cnt = 3;break;
//  	   case 3: cnt = 2;break;
//  	   case 1: cnt = 1;break;
//  	}
//  	
//  	if(memcmp(&buf[cnt+2],"\x20\x90",2) == 0) //FM,公交优先
//  	{
//       bGjFlag = 1;
//       bPfFlag  = 0;
//       goto label_gongjiao;
//    }
//    else {
//			bPfFlag  = 1;bGjFlag = 0;
//			ret = pf_mobile_ticket_process();
//			if(ret == CE_NO_PPSE)
//			{			
//				goto label_gongjiao;	
//			}
//			else if(ret == CE_UNKNOWN)
//			{
//				gGetCardInfoStep = 0;
//				return CE_UNKNOWN;	
//			}
//			else{		
//				return CE_OK_PF_MOBILE;	
//			}
//    }
//  }
  
  
	//
//label_gongjiao:
*/
	gDebugStep = 0x3083;
	memcpy(buf,"\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01\x00",15); //公交卡应用：住建部AID
#ifdef _GETCARDINFO_CPU_
	debug_printf("getcardinfo_CPU 2\n");
#endif
	memset(outbuf,0,sizeof(outbuf));
	ret = mifpro_icmd0(15,buf,outbuf);
	
#ifdef _GETCARDINFO_CPU_
	debug_printf("mifpro_icmd0...ret=%d\n",ret);
	debug_printf("outbuf :");
	for(i=0;i<ret;i++)
	{
		debug_printf(" %02x",outbuf[i]);	
	}
	debug_printf("\n");
#endif

	if(ret<0)
	{
		goto label_readerr;
	}
#ifdef _GETCARDINFO_CPU_
	debug_printf("getcardinfo_CPU 3\n");
#endif
	gDebugStep = 0x3084;
#ifdef _GETCARDINFO_CPU_
	printf("outbuf[ret-2]=%02x,outbuf[ret-1]=%02x\n",outbuf[ret-2],outbuf[ret-1]);
#endif
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
		if(ret>2)
		{
			gCPUValid = 0x00;
			goto get_cardtype;
		}
		if( (outbuf[ret-2] == 0x6a) && (outbuf[ret-1] == 0x81) )
		{
			//gResponseStatus=CE_INVALIDCARD;
			//gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		if( (outbuf[ret-2] == 0x93) && (outbuf[ret-1] == 0x03) )
		{
			gResponseStatus=CE_INVALIDCARD;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		
#ifdef _GETCARDINFO_CPU_
		for(i=0;i<80;i++)
			printf("*%02X",outbuf[i]);
		debug_printf("\ngetcardinfo_CPU2 outbuf=%02X %02X\n",outbuf[ret-2],outbuf[ret-1]);
#endif
		//2013/5/22 10:23:58 浦发手机支付
JTB_label:
		//if((outbuf[ret-2]==0x6a)&&(outbuf[ret-1]==0x82)) 
		{
			#ifdef _GETCARDINFO_CPU_
			debug_printf("getcardinfo_CPU 4-->pufa mobile\n");
			#endif
			
			#ifdef DEBUG_TIME_CPU
			ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x03", 3);
			#endif	
			//pf_mobile_ticket_process();			
			//return CE_OK_PF_MOBILE;
			
//			if(pf_mobile_ticket_process() == CE_UNKNOWN)
//			{			
//				gGetCardInfoStep = 0;
//				return CE_UNKNOWN;	
//			}
//			else
//			{
//				//返回01块数据
//				return CE_OK_PF_MOBILE;	
//			}
			ret=pf_mobile_ticket_process();
			if (ret==CE_UNKNOWN) 
			{
				gGetCardInfoStep = 0;
				return CE_UNKNOWN;	
			}
			else if (ret == OP_ERR) 
			{
				return OP_ERR;	
			}
			else
			{
				//返回01块数据
				return CE_OK_PF_MOBILE;	
			}	
			
		}
		gDebugStep = 0x3085;
		mcml_pwr_off();
		goto label_invalid;
	}
	gCPUflag = 0x55;
	//
	gDebugStep = 0x3086;
	memset(gB32,0,20);
	memset(gB33,0,20);
	

//#ifdef _GETCARDINFO_CPU_
//	memset(tbuf,0,sizeof(tbuf));
//	memcpy(buf,"\x00\xb0\x95\x00\x1e",5);
//		shmt_prot_send(buf,5);
//	ret = mifpro_icmd0(5,buf,tbuf);
//	if(ret<0)
//		printf("tbuf Err!\n");
//	shmt_prot_send(tbuf,ret);
//	printf("tbuf ret1=%d!\n",ret);
//	
//	memset(tbuf,0,sizeof(tbuf));
//	memcpy(buf,"\x00\xb0\x95\x00\x04",5);
//	shmt_prot_send(buf,5);
//	ret = mifpro_icmd0(5,buf,tbuf);
//	if(ret<0)
//		printf("tbuf Err!\n");
//	shmt_prot_send(tbuf,ret);
//	printf("tbuf ret2=%d!\n",ret);
//#endif



get_cardtype:	
	gDebugStep = 0x3087;
	//寻找\x9f\x0c
	for(cnt=0;cnt<ret-2;cnt++){
		if(memcmp(&outbuf[cnt],"\x9f\x0c",2) == 0) 
		{
			cnt = cnt + 3;
 	  	   goto label_find_9f0c;
		}
	}
 	
 	//没必要read 0015
 	//read 0015
	gDebugStep = 0x30c3;
	memcpy(buf,"\x00\xb0\x95\x00\x1e",5);
#ifdef _GETCARDINFO_CPU_
	shmt_prot_send(buf,5);
#endif
	ret = mifpro_icmd0(5,buf,outbuf);
#ifdef _GETCARDINFO_CPU_
	printf("\nread 0015 outbuf:");
	for(i=0;i<ret;i++)
	{
		printf(" %02x",outbuf[i]);
	}
	printf("\n");
#endif
	if(ret<0)
	{
		goto label_readerr;
	}
#ifdef _GETCARDINFO_CPU_
		shmt_prot_send(outbuf,ret);
#endif
	gDebugStep = 0x30c4;
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
		goto label_invalid;
	}
	cnt = 0;
	//	 	
label_find_9f0c:		
	gDebugStep = 0x308b;
	memcpy(gCityCode,&outbuf[cnt+2],2);//2011-3-29 10:55:19	
  //if((!gM1Size0x53Flag)&&(memcmp(gCityCode,"\x20\x00",2) == 0))  goto label_invalid;//2012/5/11 11:27:31,去掉CPU卡0x53的判断
  	
	gCardStatus = outbuf[cnt+8];
	//printf("cnt =%d\n",cnt+8);	
	cardversion = outbuf[cnt+9];
	//printf("cnt =%d\n",cnt+9);	
	
	memcpy(gAppSn,&outbuf[cnt+10],10);
	memcpy(StartUseDay,&outbuf[cnt+20],4);
	memcpy(gCardExpiredDate,&outbuf[cnt+24],4);
	gCardType = outbuf[cnt+28];
	gYaJin = outbuf[cnt+29];
	gDebugStep = 0x308c;
#ifdef _GETCARDINFO_CPU_
	printf("\ngCityCode:%02x%02x\n",gCityCode[0],gCityCode[2]);
	printf("gCardStatus :%02x\n",gCardStatus);
	printf("cardversion :%02x\n",cardversion);
	printf("\ngAppSn :");
	for(i=0;i<10;i++)
	{
		printf("%02x ",gAppSn[i]);
	}
	printf("\n");
#endif	
	
//判断白名单	
	memset(temp_IssuerCode,0xFF,6);
	memcpy(&temp_IssuerCode[6],gCityCode,2);
#ifdef _GETCARDINFO_CPU_
	printf("\ntemp_IssuerCode :");
	for(i=0;i<8;i++)
	{
		printf(" %02x",temp_IssuerCode[i]);
	}
	printf("\n");
#endif
	if((memcmp(gAppSn,"\x20\x00",2)!=0))
	{
#ifdef _GETCARDINFO_CPU_
		printf("\ntpWhite4020.white_head.ContentNumber :%04x\n",tpWhite4020.white_head.ContentNumber);
#endif
		for(i=0;i<tpWhite4020.white_head.ContentNumber;i++)
		{
			if(memcmp(&tpWhite4020.White_Content_val[i].CardIssuer,temp_IssuerCode,8)==0)
			{
				break;
			}
		}
		if(i==tpWhite4020.white_head.ContentNumber)
		{
			goto JTB_label;	
		}
	}
	
#ifdef _GETCARDINFO_CPU_
	printf("gCPUValid...1=%d\n",gCPUValid);
#endif

	if(gCPUValid==0)
	{
		mcml_pwr_off();
//		delay_ms(2);
		goto label_invalid;
	}
	gDebugStep = 0x308d;
#ifdef _GETCARDINFO_CPU_
	printf("gPSAMFileFlag =:%02x",gPSAMFileFlag);
#endif

	if(gPSAMFileFlag == 0x55) 
	{
		goto skip_select;
	}
	bgSHpsamIndex = SAMLOCATION_5;
	memcpy(buf,"\x00\xa4\x00\x00\x02\x10\x01",7);//cpu应用切换 (不需要切换)
	i = 7;
	gDebugStep = 0x308e;
	ret = sam_apdu(bgSHpsamIndex,buf,i,outbuf,&cnt,0,0);//没必要 sam卡切换不要放在寻卡里面
#ifdef _GETCARDINFO_CPU_
	printf("sam_apdu  ret=%d",ret);
#endif
	if(ret != 0)
	{
		gDebugStep = 0x308f;
		bgSHpsamIndex = SAMLOCATION_5;
		sam_select(bgSHpsamIndex);
		sam_set(bgSHpsamIndex,SAM_ETU_93,16);
		ret = sam_atr(bgSHpsamIndex,outbuf,&cnt);
		if(ret != 0)
		{
			goto label_samerr;
		}
		gDebugStep = 0x3090;
		memcpy(buf,"\x00\xa4\x00\x00\x02\x10\x01",7);
		i = 7;
		ret = sam_apdu(bgSHpsamIndex,buf,i,outbuf,&cnt,0,0);
		if(ret != 0)
		{
			goto label_samerr;
		}
	}
	gDebugStep=0x3091;
	//if((outbuf[0] != 0x61) &&((UBYTE)outbuf[0] != 0x90))  //2013/11/17 22:08:04
	if((outbuf[cnt-2] != 0x61) &&((UBYTE)outbuf[cnt-2] != 0x90))
	{
		goto label_samerr;
	}
	gPSAMFileFlag = 0x55;

skip_select:	
	gDebugStep = 0x3092;
	//外部认证
	memcpy(buf,"\x00\x84\x00\x00\x08",5);
	#ifdef _GETCARDINFO_CPU_
	//shmt_prot_send(buf,5);
	#endif
	ret = mifpro_icmd0(5,buf,outbuf);
	if(ret<0)
	{
		goto label_readerr;
	}
	#ifdef _GETCARDINFO_CPU_
	//shmt_prot_send(outbuf,ret);
	#endif
	gDebugStep = 0x3093;
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
		goto label_readerr;
	}
	gDebugStep = 0x3094;
	if(ret!=10)
	{
		goto label_readerr;
	}
	memcpy(Rand,outbuf,8);
	bgSHpsamIndex = SAMLOCATION_5;
	gDebugStep = 0x3095;
	memcpy(buf,"\x80\x1a\x45\x01\x10",5);//密钥分散：将指定的KEY分散至临时密钥寄存器中。45(P1):密钥用途  01(P2):(密钥标识)  10(Lc):分散数据长度，分散级数为2。
	for(i=0;i<8;i++)
	{
		buf[5+i] = gAppSn[9-i];
	}
	memcpy(&buf[13],"\x20\x00\xff",3);
	memset(&buf[16],0,5);
	i = 21;
	ret = sam_apdu(bgSHpsamIndex,buf,i,outbuf,&cnt,0,0);
//	ee_write(10,1,&bgSHpsamIndex);
//	ee_write(11,21,buf);
//	ee_write(32,cnt,outbuf);
	gDebugStep = 0x3096;
	if(ret != 0)
	{
    	goto label_samerr;
  	}
	gDebugStep = 0x3097;	
	if(outbuf[cnt-2]!=0x90)
	{
		goto label_samerr;
	}
	gDebugStep = 0x3098;
	//DES计算
	memcpy(buf,"\x80\xfa\x00\x00\x08",5);
	memcpy(&buf[5],Rand,8);
	i = 13;
	ret = sam_apdu(bgSHpsamIndex,buf,i,outbuf,&cnt,0,0);
	if(ret != 0)
	{
	  goto label_samerr;
	}	
	gDebugStep = 0x3099;
//  	ee_write(34,1,&bgSHpsamIndex);
//		ee_write(35,13,buf);
//		ee_write(48,cnt,outbuf);
 	if((outbuf[cnt-2]==0x61)&&(outbuf[cnt-1]==0x08))
  {
	  gDebugStep = 0x309a;
	  memcpy(buf,"\x00\xc0\x00\x00\x08",5);
	  i = 5;
	  ret = sam_apdu(bgSHpsamIndex,buf,i,outbuf,&cnt,0,0);
		if(ret != 0)
		{
	    goto label_samerr;
	  }
		gDebugStep = 0x309b;	
		if(cnt!=10)
		{
		  goto label_samerr;
		}
	  memcpy(&buf[5],outbuf,8);
  }
  else
  {
	  if((outbuf[cnt-2]==0x90)&&(outbuf[cnt-1]==0x00))
	  {
	  	memcpy(&buf[5],outbuf,8);
	  } 
	  else
	  {
	  	gDebugStep = 0x309c;
	  	goto label_samerr;
	  }
	}

	//2011-1-20 11:12:40
	if( (memcmp(gAppSn,"\x20\x00",2)==0) || (memcmp(gAppSn, "\x21\x53", 2) == 0) )
	{//上海卡或昆山卡//2015/9/2 16:14:02只有上海卡
		//外部认证
		//printf("gAppSn=%02x%02x\n",gAppSn[0],gAppSn[1]);
		if(memcmp(gAppSn, "\x20\x00", 2) == 0)
		{
			gDebugStep = 0x309d;
			memcpy(buf,"\x00\x82\x00\x02\x08",5);//外部认证
			ret = mifpro_icmd0(13,buf,outbuf);
			if(ret<0)
			{
				goto label_readerr;
			}
			gDebugStep = 0x309e;
			if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
			{
				#ifdef _GETCARDINFO_CPU_
					debug_printf(" ****0082 ret=%02X %02X**** \n",outbuf[ret-2],outbuf[ret-1]);
				#endif 
				gDebugStep = 0x309f;
				if((outbuf[ret-2]!=0x6a)||(outbuf[ret-1]!=0x88))
					goto label_readerr;
				gDebugStep = 0x30a0;
				if(cardversion!=0x03)
				{
					goto label_invalid;
				}
			}
		}
		//读地铁专用记录
		gDebugStep = 0x30a1;
		ret = get_record(0x11,0x03,16,outbuf);
		if(ret)
		{
			return OP_ERR;
		}
		gDebugStep = 0x30a2;
		if(lrc_chk2(outbuf,15)!=0)
		{
			gMetroIoFlag = 0;
		}
		else
		{
			gMetroIoFlag = outbuf[0];
		}
		memcpy(gB34, outbuf, 16);
		gDebugStep = 0x30a3;
		ret = get_record(0x11,0x01,16,outbuf);
		if(ret)
		{
			return OP_ERR;
		}
		memcpy(gB32,outbuf,16);
		lrc3_gen(gB32,16);
		gStationTimeOverRideFlag = gB32[0];
		
		//保存上次交易金额
		//*((char *)&gCardLastTransValue+INT_LOW) = gB32[10];
		//*((char *)&gCardLastTransValue+INT_LOW) = gB32[9];
		
		memcpy(gB32BK,gB32,20);
		
		gDebugStep = 0x30a4;
		ret = get_record(0x11,0x02,16,outbuf);
		if(ret)
		{
			return OP_ERR;
		}
		gDebugStep = 0x30a5;
		memcpy(gB33,outbuf,16);
		lrc3_gen(gB33,16);
		memcpy(gB33BK,gB33,20);
		
		#ifdef _DEBUG_GETCARDINFO_CPU_
			debug_printf("\x0d\x0a B32:");
			for(i=0;i<16;i++) debug_printf("%02X ",(UBYTE)gB32[i]);
			debug_printf("\x0d\x0a B33:");
			for(i=0;i<16;i++) debug_printf("%02X ",(UBYTE)gB33[i]);
		#endif
			
		gDebugStep = 0x30a6;
		//ret = get_record(0x11,0x03,16,outbuf);
		//if(ret)
		//{
		//	return OP_ERR;
		//}
		memcpy(outbuf, gB34, 16);
		gMetroIoFlag = outbuf[0];
		trans_ms();
		gDebugStep = 0x30a7;
		ret = get_record(0x10,0x02,16,outbuf);
		if(ret)
		{
			return OP_ERR;
		}
		memcpy(info,&outbuf[2],5);
		memcpy(leisure,&outbuf[10],5);
		memcpy(gB29Buf,outbuf,16);
		
		//2015/2/2 14:45:52
		if(gBFirst == 32){
						//保存上次交易金额
			gCardLastTransValue = 0;
			*((char *)&gCardLastTransValue+INT_HIGH) = gB32[10]; //INT_HIGH
			*((char *)&gCardLastTransValue+INT_LOW) = gB32[9];
		}
		if(gBFirst == 33){
						//保存上次交易金额
			gCardLastTransValue = 0;
			*((char *)&gCardLastTransValue+INT_HIGH) = gB33[10]; //INT_HIGH
			*((char *)&gCardLastTransValue+INT_LOW) = gB33[9];
		}
		
		#ifdef _GETCARDINFO_CPU_
			debug_printf("\x0d\x0a gBFirst=%d gCardLastTransValue = %08X ",gBFirst,gCardLastTransValue);
		#endif
	}
	else //外地卡
	{
		gDebugStep = 0x30a8;
		//2014/9/25 15:22:31在互联互通流程中，终端机具依据0x15文件中应用类型标识字段的内容，进行用户卡启用状态判断，非0x00则进入下一步操作，如为0x00则本次交易终止
		if(gCardStatus == 0)
			goto label_invalid;
			
		ret = get_capp_record(0x17,0x09,48,outbuf);
		if(ret)
		{
			return OP_ERR;
		}
		memcpy(gRecCpu0017,outbuf,48);//2011-3-29 11:10:24
#ifdef _GETCARDINFO_CPU_
		printf("\ngRecCpu0017 :");
		for(i=0;i<48;i++)
		{
			printf("%02x ",gRecCpu0017[i]);
		}
		printf("\n");
#endif
		memset(gB29,0,16);
		memset(gB29Buf,0,20);
		memset(info,0,5);
		memset(leisure,0,5);
		gDebugStep = 0x30a9;
	  if((UBYTE)outbuf[26] == (UBYTE)0x55)  
    {//进站
	    gMetroIoFlag = 0x55;
	    gBFirst = 32;
	    memset(gB32,0,16);
	    memcpy(gB32,&outbuf[27],16);
	    lrc_gen2(gB32,15);
	    lrc3_gen(gB32,16);
	    memcpy(gB32BK,gB32,20);
	    //
	    memset(gB33,0,16);
	    lrc3_gen(gB33,16);
	    memcpy(gB33BK,gB33,20);
	    gStationTimeOverRideFlag = gB32[0];
  	}
	  else if((UBYTE)outbuf[26] == (UBYTE)0x66) 
    {//出站
	 		gMetroIoFlag = 0x66;
	 		gBFirst = 33;
	 		memset(gB33,0,16);
	 		memcpy(gB33,&outbuf[27],16);
	 		
	 		
	 		lrc_gen2(gB33,15);
	 		lrc3_gen(gB33,16);
			memcpy(gB33BK,gB33,20);
			//
	 		memset(gB32,0,16);
	 		lrc3_gen(gB32,16);
	 		memcpy(gB32BK,gB32,20);
    }
    else		
    {//虚拟出站
	 		gMetroIoFlag = 0x66;
	 		gBFirst = 33;
	 		memset(gB33,0,16);
    	gB33[0] = 0x81;
	 		lrc_gen2(gB33,15);
	 		lrc3_gen(gB33,16);
			memcpy(gB33BK,gB33,20);
     	//
    	memset(gB32,0,16);
     	lrc3_gen(gB32,16);
     	memcpy(gB32BK,gB32,20);
	  }
	}
	
	//为了加快速度，将取认证码放在读卡中
	gDebugStep = 0x30aa;
	memcpy(buf,"\x80\xca\x00\x00\x09",5);
	ret = mifpro_icmd0(5,buf,outbuf);
	if(ret<0)
	{
		goto label_readerr;
	}
	gDebugStep = 0x30ab;
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
		goto label_invalid;
	}
	
	memcpy(bgTaskKeyInbuf,outbuf,9);
	set_task_key(TASK_KEY_SEND_AUTHCODE);
  
  //init for capp purchase
	gDebugStep = 0x30ac;
	memcpy(buf,"\x80\x50\x03\x02\x0B",5);
	buf[5] = gKeyVersion;
	memset(&buf[6],0,4);
	memcpy(&buf[10],gPosID,6);
  //ee_write(256,16,buf);
	ret = mifpro_icmd0(16,buf,outbuf);
	if(ret<0)
	{
		goto label_readerr;
	}
  //ee_write(272,20,outbuf);
	gDebugStep = 0x30ad;
#ifdef _GETCARDINFO_CPU_
		debug_printf("\n****8050 ret=%02X %02X**** \n",outbuf[ret-2],outbuf[ret-1]);
#endif 
	if((outbuf[ret-2]==0x62)||(outbuf[ret-1]==0x81))//2015/8/21 15:33:15 CPU优先是 8050失败返回6281时，继续M1流程
	{
#ifdef _GETCARDINFO_CPU_
		debug_printf(" ****8050=6281**** \n");
#endif 
		mcml_pwr_off();	
	  	//set_timeout(8000);
		gCPUflag = 0;
		return OP_ERR;
	}
	gDebugStep = 0x31ad;
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
		goto label_invalid;
	}
	gDebugStep = 0x30ae;
	if(ret!=17)
	{
		goto label_invalid;
	}
	memset(buf,0,4);
	memcpy(&buf[1],&outbuf[6],3);
	buf_to_udword(buf,gPurseOffset);					//透支额
	buf_to_udword(outbuf,gCardPurse);//钱包值
	gCardLastRemain = gCardPurse-gPurseOffset;//卡上交易前金额
	gDebugStep = 0x30af;
	if(gCardLastRemain>0x7fffffff)
	{
		gDebugStep = 0x30b0;
		gCardThisRemain = ~gCardLastRemain+1;
		if(gCardThisRemain>3000)
		{
			goto label_invalid;
		}
		gCardThisRemain = gCardLastRemain;
	}
	buf_to_uword(&outbuf[4],gCardCnt);
	gKeyBacon = outbuf[9];
	gAluBacon = outbuf[10];
	memcpy(ICC,&outbuf[11],4);
	return 0;
	
label_readerr:
		gResponseStatus=CE_READERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_invalid:
		gResponseStatus=CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_samerr:
		gResponseStatus=CE_PSAMERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
}
/*************************************************************
函数：trans_entry_cpu
功能：CPU卡进站交易
*************************************************************/
//#define _TRANS_ENTRY_CPU_
char trans_entry_cpu(void)
{
	int ret,i;
	UBYTE buf[60],outbuf[300];
	UBYTE n,outn;
	UBYTE mac1[4],mac2[4];
	UBYTE buf32[16],buf33[3];
	UBYTE temp2;
	
	gDebugStep = 0x3181;
	gCardThisTransValue = 0;

	/* 2010-4-13 10:07
	memcpy(buf,"\x80\xca\x00\x00\x09",5);
	ret = mifpro_icmd0(5,buf,outbuf);
	if(ret<0)
	{
		goto label_readerr;
	}
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
		goto label_invalid;
	}
	memcpy(bgTaskKeyInbuf,outbuf,9);
	set_task_key(TASK_KEY_SEND_AUTHCODE);
	*/
	
	/* 2010-4-13 14:25
	//init for capp purchase
	memcpy(buf,"\x80\x50\x03\x02\x0B",5);
	buf[5] = gKeyVersion;
	memset(&buf[6],0,4);
	memcpy(&buf[10],gPosID,6);
	//ee_write(4,16,buf);
	ret = mifpro_icmd0(16,buf,outbuf);
	if(ret<0)
	{
		goto label_readerr;
	}
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
		goto label_invalid;
	}
	if(ret!=17)
	{
		goto label_invalid;
	}
	//ee_write(20,17,outbuf);
	memset(buf,0,4);
	memcpy(&buf[1],&outbuf[6],3);
	buf_to_udword(buf,ret);					//透支额
	buf_to_udword(outbuf,gCardLastRemain);
	gCardLastRemain = gCardLastRemain-ret;
	buf_to_uword(&outbuf[4],gCardCnt);
	gKeyBacon = outbuf[9];
	gAluBacon = outbuf[10];	
	memcpy(ICC,&outbuf[11],4);
	*/
	if(SHSendAuthResp())
	{
		goto label_samerr;
	}
	//交易后余额
	gCardThisRemain = gCardLastRemain;//for gCardThisTransValue=0
	
	gDebugStep = 0x3182;
	//init for sam 
	memcpy(buf,"\x80\x70\x00\x01\x28",5);
	memcpy(&buf[5],ICC,4);
	uword_to_buf(gCardCnt,&buf[9]);
	memset(&buf[11],0,4);
	buf[15] = 0x09;					//09:复合消费，06：电子钱包消费
	memcpy(&buf[16],gTransTime,7);
	buf[23] = gKeyBacon;
	buf[24] = gAluBacon;
	udword_to_buf(gPosTransCnt,&buf[25]);
	memcpy(&buf[29],&gAppSn[2],8);
	memcpy(&buf[37],gAppSn,2);
	memcpy(&buf[39],"\xff\x00\x00\x00\x00\x00",6);
	memcpy(bgTaskKeyInbuf,&buf[5],40);
	set_task_key(TASK_KEY_INIT_FOR_SAM);
	//OSTimeDly(1);
	gDebugStep = 0x3183;
  //update capp date cache
	
	//写地铁专用记录
	if(checkrec_inmemory(REC_EXIT)!=0)
	{
		gDebugStep = 0x3184;
		ret = get_record(0x11,0x02,16,gB33);
		if(ret!=0)
		{
			gResponseStatus=CE_READERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		lrc3_gen(gB33,16);
		memcpy(gB33BK,gB33,20);
	}
	memcpy(buf,gB33,16);
	temp2=buf[0];
	buf33[0]=buf[9];
	buf33[1]=buf[10];
	
	if(checkrec_inmemory(REC_ENTRY)!=0)
	{
		gDebugStep = 0x3185;
		ret = get_record(0x11,0x01,16,gB32);
		if(ret!=0)
		{
			gResponseStatus=CE_READERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		lrc3_gen(gB32,16);
		memcpy(gB32BK,gB32,20);
	}
	memcpy(buf,gB32,16);
	
#ifdef _GETCARDINFO_CPU_
	printf("\ngB32 :");
	for(i=0;i<16;i++)
	{
		printf("%02x ",gB32[i]);
	}
	printf("\n");
#endif
//	if(memcmp(buf,"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff",16)==0)
//	{
//		eeflag = 0x55;
//		ee_read(ADDR_CHECK_B32,4,snbuf);
//		if(memcmp(gThisCardSnr,snbuf,4)==0)
//		{
//			ee_read(ADDR_CHECK_B32+4,16,buf);
//			memcpy(buf32,buf,16);
//		}
//	}
//	else
	memcpy(buf32,buf,16);
	
	//写0017文件	
#ifdef _GETCARDINFO_CPU_
	printf("gNotShCard=%02x\n",gNotShCard);
#endif
	if(gNotShCard==0x55) //外地卡
	{
		/*cancelled in 2011-7-22 15:59:03,modified back
				//2011-3-29 16:43:25
				memcpy(buf,gRecCpu0017,48);
				//写专用记录
				if(gStationTimeOverRideFlag == 0x00) 
				{
					buf[26] = 0x41;		
				}
				else 
				{
					if(gStationTimeOverRideFlag==0x18)
					{
						if(temp2&0x08)
							buf[26]=	0x78;
						else
							buf[26]= 0x71;
					}
					else
					{
						buf[26]=gStationTimeOverRideFlag<<1;
						buf[26]+=0x40;
						if(buf[26]==0x50)
							if(temp2&0x08)
								buf[26]= 0x59;
					}
					if(gStationTimeOverRideFlag&0x08)
					{
						buf[35]=buf33[0];
						buf[36]=buf33[1];
						if((recordtime[0]!=gTransTime[1])||(recordtime[1]!=gTransTime[2]))
						{
							recordsum[0]=0;
							recordsum[1]=0;
							recordsum[2]=0;
							recordcnt=0;
						}
						//buf[11]=recordcnt;
						buf[39]=recordsum[0];
						buf[38]=recordsum[1];
						buf[37]=recordsum[2];
						goto skip_in_0017;
					}
				}
				buf[27]=gTransTime[1];
				buf[28]=gTransTime[2];
				buf[29]=gTransTime[3];
				buf[30]=gTransTime[4];
				buf[31]=gTransTime[5];
				buf[32]=gPosNo[0];
				buf[33]=gPosNo[1];
				buf[34]=gPosNo[3];
				buf[35]  = *((char *)&gCardThisTransValue+LONG_HIGH0);
				buf[36]  = *((char *)&gCardThisTransValue+LONG_HIGH1);
				if((recordtime[0]!=gTransTime[1])||(recordtime[1]!=gTransTime[2]))
				{
					recordsum[0]=0;
					recordsum[1]=0;
					recordsum[2]=0;
					recordcnt=0;
				}
				//buf[11]=recordcnt;
				buf[39]=recordsum[0];
				buf[38]=recordsum[1];
				buf[37]=recordsum[2];
		skip_in_0017:
				//
				memcpy(buf,"\x09\x2e\x00",3);//1-1	复合消费标志	1	BCD－09
		                                //2-2	记录长度	1	HEX－2E
		                                //3-3	应用锁定标志位	1	HEX－00
				buf[3] = 0; //4-4	复合消费记录版本号	1	HEX
		  	buf[4] = 1;
		  	memcpy(&buf[5],gPosID,6); //6-11	终端机编号（城市代码）	6	BCD
		  	//udword_to_buf(gCardThisRemain,&buf[11]);//12-15	金额（用于补扣）
		  	memcpy(&buf[11],"\x00\x00\x02\x58",4);
		  	memcpy(&buf[15],gTransTime,7); //16-19	交易日期	4	YYYYMMDD
		                                 //20-22	交易时间	3	HHMMSS
		    memset(&buf[22],0,4); //23-26	通用复合消费记录MAC	4	HEX,暂时填0
		    buf[26] = 0x41;//？？？
		    gDebugStep = 0x3186;*/
		
		//写专用记录
#ifdef _GETCARDINFO_CPU_
		printf("\ngStationTimeOverRideFlag...1=%02x\n",gStationTimeOverRideFlag);
#endif
		if(gStationTimeOverRideFlag == 0x00) 
		{
			buf[0] = 0x41;		
		}
		else 
		{
			if(gStationTimeOverRideFlag==0x18)
			{
				if(temp2&0x08)
					buf[0]=	0x78;
				else
					buf[0]= 0x71;
			}
			else
			{
				buf[0]=gStationTimeOverRideFlag<<1;
				buf[0]+=0x40;
				if(buf[0]==0x50)
					if(temp2&0x08)
						buf[0]= 0x59;
			}
			if(gStationTimeOverRideFlag&0x08)
			{
				memcpy(&buf[1],&gRecCpu0017[28],8);//20191107
#ifdef _GETCARDINFO_CPU_
				printf("\nbuf :");
				for(i=0;i<8;i++)
				{
					printf("%02x ",buf[i+1]);
				}
				printf("\n");
#endif
				buf[9]=buf33[0];
				buf[10]=buf33[1];
				if((recordtime[0]!=gTransTime[1])||(recordtime[1]!=gTransTime[2]))
				{
					recordsum[0]=0;
					recordsum[1]=0;
					recordsum[2]=0;
					recordcnt=0;
				}
				buf[11]=recordcnt;
				buf[14]=recordsum[0];
				buf[13]=recordsum[1];
				buf[12]=recordsum[2];				
				goto skip_in_0017;
			}
		}
		buf[1]=gTransTime[1];
		buf[2]=gTransTime[2];
		buf[3]=gTransTime[3];
		buf[4]=gTransTime[4];
		buf[5]=gTransTime[5];
		buf[6]=gPosNo[0];
		buf[7]=gPosNo[1];
		buf[8]=gPosNo[3];
		buf[9]  = *((char *)&gCardThisTransValue+LONG_HIGH0);
		buf[10] = *((char *)&gCardThisTransValue+LONG_HIGH1);
		if((recordtime[0]!=gTransTime[1])||(recordtime[1]!=gTransTime[2]))
		{
			recordsum[0]=0;
			recordsum[1]=0;
			recordsum[2]=0;
			recordcnt=0;
		}
		buf[11]=recordcnt;
		buf[14]=recordsum[0];
		buf[13]=recordsum[1];
		buf[12]=recordsum[2];
skip_in_0017:
		lrc_gen2(buf,15);
		memcpy(&buf[27],buf,16);
		memcpy(buf,"\x09\x2e\x00",3);//1-1	复合消费标志	1	BCD－09
                                //2-2	记录长度	1	HEX－2E
                                //3-3	应用锁定标志位	1	HEX－00
		buf[3] = 0; //4-4	复合消费记录版本号	1	HEX
	  buf[4] = 1;
	  memcpy(&buf[5],gPosID,6); //6-11	终端机编号（城市代码）	6	BCD
	  //udword_to_buf(gCardThisRemain,&buf[11]);//12-15	金额（用于补扣）
	  memset(&buf[11],0,4);
	 	memcpy(&buf[15],gTransTime,7); //16-19	交易日期	4	YYYYMMDD
	                                 //20-22	交易时间	3	HHMMSS
    memset(&buf[22],0,4); //23-26	通用复合消费记录MAC	4	HEX,暂时填0
	  buf[26] = 0x55;
	  memset(&buf[43],0,5);
	  gDebugStep = 0x3186;		
	  ret = write_capp_record(0x17,0x09,48,buf,0);
	  if(ret)
		{
			gResponseStatus=CE_WRITEERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		goto label_get_mac1;
	}
	//更新卡上地铁专用扇区交易	   
 	gDebugStep = 0x3187;
#ifdef _GETCARDINFO_CPU_ 
 	printf("\ngStationTimeOverRideFlag...2=%02x\n",gStationTimeOverRideFlag);
#endif
	if(gStationTimeOverRideFlag == 0x00) 
	{
		buf[0] = 0x41;		
	}
	else 
	{
		if(gStationTimeOverRideFlag==0x18)
		{
			if(temp2&0x08)
				buf[0]=	0x78;
			else
				buf[0]= 0x71;
		}
		else
		{
			buf[0]=gStationTimeOverRideFlag<<1;
			buf[0]+=0x40;
			if(buf[0]==0x50)
				if(temp2&0x08)
					buf[0]= 0x59;
		}
		if(gStationTimeOverRideFlag&0x08)
		{
			buf[9]=buf33[0];
			buf[10]=buf33[1];
			if((recordtime[0]!=gTransTime[1])||(recordtime[1]!=gTransTime[2]))
			{
				recordsum[0]=0;
				recordsum[1]=0;
				recordsum[2]=0;
				recordcnt=0;
			}
			buf[11]=recordcnt;
			buf[14]=recordsum[0];
			buf[13]=recordsum[1];
			buf[12]=recordsum[2];
			goto wr32;
		}
	}
	buf[1]=gTransTime[1];
	buf[2]=gTransTime[2];
	buf[3]=gTransTime[3];
	buf[4]=gTransTime[4];
	buf[5]=gTransTime[5];
	buf[6]=gPosNo[0];
	buf[7]=gPosNo[1];
	buf[8]=gPosNo[3];
	buf[9]  = *((char *)&gCardThisTransValue+LONG_HIGH0);
	buf[10] = *((char *)&gCardThisTransValue+LONG_HIGH1);
	if((recordtime[0]!=gTransTime[1])||(recordtime[1]!=gTransTime[2]))
	{
		recordsum[0]=0;
		recordsum[1]=0;
		recordsum[2]=0;
		recordcnt=0;
	}
	buf[11]=recordcnt;
	buf[14]=recordsum[0];
	buf[13]=recordsum[1];
	buf[12]=recordsum[2];
		
wr32:
	//写B32
	lrc_gen2(buf,15);
	gDebugStep = 0x3188;	
	ret = write_record(0x11,0x01,16,buf,0);
	if(ret)
	{
		gResponseStatus=CE_WRITEERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
	//2018/1/31 13:44 为支持科委
	gDebugStep = 0x3189;
	if(buf[0] & 0x10)
	{
		memcpy(buf, gB34, 16);
	}else
	{
		memset(buf,0,16);
	}
	buf[0] = 0x55;
	//buf[15] = 0x55;
	lrc_gen2(buf, 15);
	ret = write_record(0x11,0x03,16,buf,0);
	if(ret)
	{
		gResponseStatus=CE_WRITEERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
	gDebugStep = 0x318a;
	
	if(gCardStatus!=M1LOCKED)
	{
		trans_gen_card_record(0x11,buf);
		ret = write_record(0x07,0x00,16,buf,1);
		if(ret)
		{
			gResponseStatus=CE_WRITEERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
	}
	
label_get_mac1:	
	gDebugStep = 0x318b;
	if(SHSendAuthResp())
	{
		goto label_samerr;
	}
	buf_to_udword(bgTaskKeyOutbuf,gPosTransCnt);
	memcpy(mac1,&bgTaskKeyOutbuf[4],4);
	gDebugStep = 0x318c;
	//ee_write(82,8,bgTaskKeyOutbuf);
	//debit for purchase
	memcpy(buf,"\x80\x54\x01\x00\x0F",5);
	udword_to_buf(gPosTransCnt,&buf[5]);
	memcpy(&buf[9],gTransTime,7);
	memcpy(&buf[16],mac1,4);
	//ee_write(90,20,buf);
#ifdef _TRANS_ENTRY_CPU_
	shmt_prot_send(buf,20);
#endif
	ret = mifpro_icmd0(20,buf,outbuf);
	if(ret<0)
	{
		gResponseStatus = CE_CARDREMOVED;
		gGetCardInfoStep=0xff;
		return OP_ERR;//2015/8/26 14:16:25失败时 返回闪卡
		//goto label_readerr;
	}
#ifdef _TRANS_ENTRY_CPU_
	shmt_prot_send(outbuf,ret);
#endif
	gDebugStep = 0x318d;
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
		if((outbuf[ret-2]==0x93)&&(outbuf[ret-1]==0x02))
		{
			gDebugStep = 0x318e;
			gResponseStatus=CE_MAC2ERR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		gDebugStep = 0x318f;
		goto label_invalid;
	}
	gDebugStep = 0x3190;
	if(ret!=10)
	{
		goto label_invalid;
	}
	memcpy(gTAC,outbuf,4);
	memcpy(mac2,&outbuf[4],4);
	gDebugStep = 0x3191;
	//credit sam for purchase
	memcpy(buf,"\x80\x72\x00\x00\x04",5);
	memcpy(&buf[5],mac2,4);
	n = 9;
#ifdef _TRANS_ENTRY_CPU_
	shmt_prot_send(buf,n);
#endif
	ret=sam_apdu(bgSHpsamIndex,buf,n,outbuf,&outn,0,0);
#ifdef _GETCARDINFO_CPU_
	printf("\n8072 ret =%d",ret);
	printf("\noutbuf :");	
	for(i=0;i<outn;i++)
	{
		printf(" %02x",outbuf[i]);
	}
	printf("\n");
#endif
	if(ret!=0)
	{
		goto label_samerr;
	}
#ifdef _TRANS_ENTRY_CPU_
	shmt_prot_send(outbuf,outn);
#endif
	if((outbuf[outn-2]!=0x90)||(outbuf[outn-1]!=0x00))
	{
		gDebugStep = 0x3192;
		gResponseStatus=CE_MAC2ERR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
	return 0;
	
label_readerr:
		gResponseStatus=CE_READERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_invalid:
		gResponseStatus=CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		return OP_ERR;	
label_samerr:
		gResponseStatus=CE_PSAMERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;	
}
/*************************************************************
函数：trans_exit_cpu
功能：CPU卡出站交易
*************************************************************/
//#define _TRANS_EXIT_CPU_
char trans_exit_cpu(UBYTE offsetnum)
{
	int ret,i;
	UBYTE buf[60],outbuf[300];
	UBYTE n,outn;
	UBYTE cnt;
	UBYTE mac1[4],mac2[4];
	UDWORD temp2,tint;
	UBYTE recbuf[100];
	//为了加快速度，将取认证码放在读卡中
	/* 2010-4-13 10:12
	gDebugStep = 0x3271;
	memcpy(buf,"\x80\xca\x00\x00\x09",5);
	ret = mifpro_icmd0(5,buf,outbuf);
	if(ret<0)
	{
		goto label_readerr;
	}
	gDebugStep = 0x3272;
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
		goto label_invalid;
	}
	gDebugStep = 0x3273;
	memcpy(bgTaskKeyInbuf,outbuf,9);
	set_task_key(TASK_KEY_SEND_AUTHCODE);
	*/
	//2013/10/29 14:28:26
	
	if( (gNotShCard==0x55) || (memcmp(gAppSn, "\x21\x53", 2) == 0) )
	{//外地卡
		/*
		mcml_pwr_off();
		//2013/11/5 17:10:1
		gDebugStep = 0x3271;
		ret=mcml_request(PICC_REQSTD,buf);
		if(ret!=0)
		{
				goto label_readerr;
		}
		gThisCardSnr[6] = buf[0];
		gThisCardSnr[7] = buf[1];
		gDebugStep = 0x3272;
		ret=mcml_anticoll(gThisCardSnr);
		if(ret!=0)
		{
				goto label_readerr;
		}
		gDebugStep = 0x3273;
		ret=mcml_select(gThisCardSnr,&cnt);
		if(ret!=0)
		{
				goto label_readerr;
		}
		gThisCardSnr[5] = cnt;
		gDebugStep = 0x3274;
		ret = mifpro_ats0(buf);
		if(ret < 0)
		{
				mcml_pwr_off();
				goto label_readerr;
		}	
		gDebugStep = 0x3275;
		
			memcpy(buf,"\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01\x00",15); //公交卡应用
			#ifdef _GETCARDINFO_CPU_
				debug_printf("  exit_CPU 2\n");
			#endif
			ret = mifpro_icmd0(15,buf,outbuf);
			if(ret<0)
			{
				goto label_readerr;
			}
			gDebugStep = 0x3265;
			if(SHSendAuthResp())
			{
				goto label_samerr;
			}
			gDebugStep = 0x3266;
			*/
		 	//read 0015
			gDebugStep = 0x36c3;
			memcpy(buf,"\x00\xb0\x95\x00\x1e",5);
			ret = mifpro_icmd0(5,buf,outbuf);
			if(ret<0)
			{
				goto label_readerr;
			}
			gDebugStep = 0x36c4;
			if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
			{
				goto label_invalid;
			}
			//		取认证码放
			gDebugStep = 0x36aa;
			memcpy(buf,"\x80\xca\x00\x00\x09",5);
			ret = mifpro_icmd0(5,buf,outbuf);
			if(ret<0)
			{
				goto label_readerr;
			}
			gDebugStep = 0x36ab;
			if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
			{
				goto label_invalid;
			}
			
			memcpy(bgTaskKeyInbuf,outbuf,9);
			set_task_key(TASK_KEY_SEND_AUTHCODE);
	}
	
	//init for capp purchase

	memcpy(buf,"\x80\x50\x03\x02\x0B",5);
	buf[5] = gKeyVersion;
	udword_to_buf(gCardThisTransValue,&buf[6]);
	memcpy(&buf[10],gPosID,6);
	gDebugStep = 0x3281;
#ifdef _TRANS_EXIT_CPU_
	shmt_prot_send(buf,16);
#endif 
	ret = mifpro_icmd0(16,buf,outbuf);
	if(ret<0)
	{
		goto label_readerr;
	}
#ifdef _TRANS_EXIT_CPU_
	//shmt_prot_send(outbuf,ret);
#endif
#ifdef _GETCARDINFO_CPU_
	debug_printf("\ninit for capp purchase,ret=%d:outbuf=",ret);
	for(i=0;i<ret;i++)debug_printf("% 02X",outbuf[i]);
	debug_printf("\n");
#endif
	//ee_write(0x90,ret,outbuf);
	gDebugStep = 0x3282;
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
		goto label_invalid;
	}
	gDebugStep = 0x3283;
	if(ret!=17)
	{
		goto label_invalid;
	}
	gDebugStep = 0x3284;
	memset(buf,0,4);
	memcpy(&buf[1],&outbuf[6],3);
	buf_to_udword(buf,ret);					//透支额
	buf_to_udword(outbuf,gCardLastRemain);
	gCardLastRemain = gCardLastRemain-ret;
	buf_to_uword(&outbuf[4],gCardCnt);
	gKeyBacon = outbuf[9];
	gAluBacon = outbuf[10];
	memcpy(ICC,&outbuf[11],4);
	//
	gCardThisRemain = gCardLastRemain - gCardThisTransValue;
	gDebugStep = 0x3285;
	if(SHSendAuthResp())
	{
		goto label_samerr;
	}
	gDebugStep = 0x3286;

	//init for sam 
	memcpy(buf,"\x80\x70\x00\x01\x28",5);
	memcpy(&buf[5],ICC,4);
	uword_to_buf(gCardCnt,&buf[9]);
  //memset(&buf[11],0,4);
	udword_to_buf(gCardThisTransValue,&buf[11]);
	buf[15] = 0x09;//09:复合消费，06：电子钱包消费
	memcpy(&buf[16],gTransTime,7);
	buf[23] = gKeyBacon;
	buf[24] = gAluBacon;
	udword_to_buf(gPosTransCnt,&buf[25]);
	memcpy(&buf[29],&gAppSn[2],8);
	memcpy(&buf[37],gAppSn,2);
	memcpy(&buf[39],"\xff\x00\x00\x00\x00\x00",6);
	memcpy(bgTaskKeyInbuf,&buf[5],40);
	set_task_key(TASK_KEY_INIT_FOR_SAM);
	//OSTimeDly(1);
	
  //update cache
	if(gNotShCard==0x55) //外地卡
	{
		/*
		//cancelled in 2011-7-22 16:03:25,modified back
		//		//2011-3-30 10:59:40
		//		memcpy(buf,gRecCpu0017,48);
		//		//
		//		if((recordtime[0]!=gTransTime[1])||(recordtime[1]!=gTransTime[2]))
		//		{
		//			recordsum[0]=0;
		//			recordsum[1]=0;
		//			recordsum[2]=0;
		//			recordcnt=0;
		//		}
		//		*((char *)&temp2+LONG_HIGH3)=0;
		//		*((char *)&temp2+LONG_HIGH2)=recordsum[0];
		//		*((char *)&temp2+LONG_HIGH1)=recordsum[1];
		//		*((char *)&temp2+LONG_HIGH0)=recordsum[2];
		//		if(gCardThisTransValue>(0xffffff-temp2))
		//		{
		//			temp2=0xffffff;
		//		}
		//		else
		//		{
		//			temp2=temp2+gCardThisTransValue;
		//		}
		//		buf[37]=*((char *)&temp2 +LONG_HIGH0);
		//		buf[38]=*((char *)&temp2 +LONG_HIGH1);
		//		buf[39]=*((char *)&temp2 +LONG_HIGH2);
		//		if(recordcnt!=0xff)
		//		{
		//			recordcnt++;
		//		}
		//		//buf[11]=recordcnt;
		//		
		//		if((offsetnum>0x05)&&(offsetnum<10))
		//		{
		//			if(gStationTimeOverRideFlag&0x20)
		//				buf[26]=0xa9;
		//			else
		//				buf[26]=0x88;
		//		}
		//		else
		//		{	
		//			if(gStationTimeOverRideFlag&0x20)
		//				buf[26]=0xa0;
		//			else
		//				buf[26]=0x81;
		//		}	
		//	  //
		//		buf[40] = gTransTime[1];
		//		buf[41] = gTransTime[2];
		//		buf[42] = gTransTime[3];
		//		buf[43] = gTransTime[4];
		//		buf[44] = gTransTime[5];
		//		buf[45] = gPosNo[0];
		//		buf[46] = gPosNo[1];
		//		buf[47] = gPosNo[3];
		//		if(gStationTimeOverRideFlag&0x10)
		//		{
		//			tint=tint+(UWORD)gCardThisTransValue;
		//			buf[35]=*((char *)&tint+INT_LOW);
		//			buf[36]=*((char *)&tint+INT_HIGH);
		//		}
		//		else
		//		{
		//			buf[35]  = *((char *)&gCardThisTransValue+LONG_HIGH0);
		//			buf[36] = *((char *)&gCardThisTransValue+LONG_HIGH1);
		//		}
		//    //
		//		memcpy(buf,"\x09\x2e\x00",3);//1-1	复合消费标志	1	BCD－09
		//                                 //2-2	记录长度	1	HEX－2E
		//                                 //3-3	应用锁定标志位	1	HEX－00
		//		buf[3] = 0; //4-4	复合消费记录版本号	1	HEX
		//  	buf[4] = 0;
		//  	memcpy(&buf[5],gPosID,6); //6-11	终端机编号（城市代码）	6	BCD
		//  	//udword_to_buf(gCardThisRemain,&buf[11]);//12-15	金额（用于补扣）
		//  	memset(&buf[11],0,4);
		//  	memcpy(&buf[15],gTransTime,7); //16-19	交易日期	4	YYYYMMDD
		//                                   //20-22	交易时间	3	HHMMSS
		//    memset(&buf[22],0,4); //23-26	通用复合消费记录MAC	4	HEX,暂时填0
		//    buf[26] = 0x81;//???
		//    //
		//    gDebugStep = 0x3287;
*/
		if((recordtime[0]!=gTransTime[1])||(recordtime[1]!=gTransTime[2]))
		{
			recordsum[0]=0;
			recordsum[1]=0;
			recordsum[2]=0;
			recordcnt=0;
		}
		*((char *)&temp2+LONG_HIGH3)=0;
		*((char *)&temp2+LONG_HIGH2)=recordsum[0];
		*((char *)&temp2+LONG_HIGH1)=recordsum[1];
		*((char *)&temp2+LONG_HIGH0)=recordsum[2];
		if(gCardThisTransValue>(0xffffff-temp2))
		{
			temp2=0xffffff;
		}
		else
		{
			temp2=temp2+gCardThisTransValue;
		}
		buf[12]=*((char *)&temp2 +LONG_HIGH0);
		buf[13]=*((char *)&temp2 +LONG_HIGH1);
		buf[14]=*((char *)&temp2 +LONG_HIGH2);
		if(recordcnt!=0xff)
		{
			recordcnt++;
		}
		buf[11]=recordcnt;
		buf[12]=*((char *)&temp2 +LONG_HIGH0);
		buf[13]=*((char *)&temp2 +LONG_HIGH1);
		buf[14]=*((char *)&temp2 +LONG_HIGH2);	
		
		if((offsetnum>0x05)&&(offsetnum<10))
		{
			if(gStationTimeOverRideFlag&0x20)
				buf[0]=0xa9;
			else
				buf[0]=0x88;
		}
		else
		{	
			if(gStationTimeOverRideFlag&0x20)
				buf[0]=0xa0;
			else
				buf[0]=0x81;
		}	
	
		buf[1] = gTransTime[1];
		buf[2] = gTransTime[2];
		buf[3] = gTransTime[3];
		buf[4] = gTransTime[4];
		buf[5] = gTransTime[5];
		buf[6] = gPosNo[0];
		buf[7] = gPosNo[1];
		buf[8] = gPosNo[3];
		if(gStationTimeOverRideFlag&0x10)
		{
			//tint=tint+(UWORD)gCardThisTransValue;
			tint=gCardLastTransValue+(UWORD)gCardThisTransValue;
			buf[9]=*((char *)&tint+INT_LOW);
			buf[10]=*((char *)&tint+INT_HIGH);
		}
		else
		{
			buf[9]  = *((char *)&gCardThisTransValue+LONG_HIGH0);
			buf[10] = *((char *)&gCardThisTransValue+LONG_HIGH1);
		}
		lrc_gen2(buf,15);
		memcpy(&buf[27],buf,16);
		memcpy(buf,"\x09\x2e\x00",3);//1-1	复合消费标志	1	BCD－09
                                //2-2	记录长度	1	HEX－2E
                                //3-3	应用锁定标志位	1	HEX－00
		buf[3] = 0; //4-4	复合消费记录版本号	1	HEX
  	buf[4] = 0;
  	memcpy(&buf[5],gPosID,6); //6-11	终端机编号（城市代码）	6	BCD
  	//udword_to_buf(gCardThisRemain,&buf[11]);//12-15	金额（用于补扣）
  	memset(&buf[11],0,4);
  	memcpy(&buf[15],gTransTime,7); //16-19	交易日期	4	YYYYMMDD
                                 //20-22	交易时间	3	HHMMSS
    memset(&buf[22],0,4); //23-26	通用复合消费记录MAC	4	HEX,暂时填0
    buf[26] = 0x66;
    memset(&buf[43],0,5);
    gDebugStep = 0x3287;
    ret = write_capp_record(0x17,0x09,48,buf,0);
    if(ret)
		{
			gResponseStatus=CE_WRITEERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		goto label_get_mac1;
	}
	
	
	if(gCardStatus!=M1LOCKED)
	{
		if(offsetnum<13)
			trans_gen_card_record(trans_id[offsetnum],buf);
		else
			trans_gen_card_record(0x11,buf);
		gDebugStep = 0x3288;
		ret = write_record(0x07,0x00,16,buf,1);
		if(ret)
		{
			gResponseStatus=CE_WRITEERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
	}
	//改写公交记录区
	memcpy(buf,gB29Buf,16);
	buf[0]=gPosNo[1];
	buf[1]=gPosNo[3];
	buf[2]=gPosNo[0];
	buf[2]|=0x80;
	buf[3]=gTransTime[2];
	buf[4]=gTransTime[3];
	buf[5]=gTransTime[4];
	buf[6]=gTransTime[5];
	lrc_gen2(buf,15);
	gDebugStep = 0x3289;
	ret = write_record(0x10,0x02,16,buf,0);
	if(ret)
	{
	  gResponseStatus=CE_WRITEERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
	gDebugStep = 0x328a;
	ret = write_record(0x10,0x03,16,buf,0);
	if(ret)
	{
	  gResponseStatus=CE_WRITEERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
	//更新卡上地铁专用扇区交易	
	if((recordtime[0]!=gTransTime[1])||(recordtime[1]!=gTransTime[2]))
	{
		recordsum[0]=0;
		recordsum[1]=0;
		recordsum[2]=0;
		recordcnt=0;
	}
	*((char *)&temp2+LONG_HIGH3)=0;
	*((char *)&temp2+LONG_HIGH2)=recordsum[0];
	*((char *)&temp2+LONG_HIGH1)=recordsum[1];
	*((char *)&temp2+LONG_HIGH0)=recordsum[2];
	if(gCardThisTransValue>(0xffffff-temp2))
	{
		temp2=0xffffff;
	}
	else
	{
		temp2=temp2+gCardThisTransValue;
	}
	buf[12]=*((char *)&temp2 +LONG_HIGH0);
	buf[13]=*((char *)&temp2 +LONG_HIGH1);
	buf[14]=*((char *)&temp2 +LONG_HIGH2);
	if(recordcnt!=0xff)
	{
		recordcnt++;
	}
	buf[11]=recordcnt;
	buf[12]=*((char *)&temp2 +LONG_HIGH0);
	buf[13]=*((char *)&temp2 +LONG_HIGH1);
	buf[14]=*((char *)&temp2 +LONG_HIGH2);	
	
	if((offsetnum>0x05)&&(offsetnum<10))
	{
		if(gStationTimeOverRideFlag&0x20)
			buf[0]=0xa9;
		else
			buf[0]=0x88;
	}
	else
	{	
		if(gStationTimeOverRideFlag&0x20)
			buf[0]=0xa0;
		else
			buf[0]=0x81;
	}	

	buf[1] = gTransTime[1];
	buf[2] = gTransTime[2];
	buf[3] = gTransTime[3];
	buf[4] = gTransTime[4];
	buf[5] = gTransTime[5];
	buf[6] = gPosNo[0];
	buf[7] = gPosNo[1];
	buf[8] = gPosNo[3];
	
	
		//ret = get_record(0x06,1,0x17,&recbuf[1]);//充值明细文件0008-->0006
		//if(ret)
		//{
		//	return OP_ERR;
		//}
	//#ifdef _DEBUG_BOM_
		//debug_printf("\nget_record:");
		//for(ret = 0;ret < 50; ret++)
		//		debug_printf("%X ",recbuf[ret]);
	//	debug_printf("\ngStationTimeOverRideFlag %X",gStationTimeOverRideFlag);
	//	debug_printf("\ngCardLastTransValue %X",gCardLastTransValue);
	//	debug_printf("\ngCardThisTransValue %X",gCardThisTransValue);
	//#endif
	if(gStationTimeOverRideFlag&0x10)
	{
		//#ifdef _DEBUG_BOM_
		//	debug_printf("\ntint+(UWORD)gCardThisTransValue... %02X ",tint);
		//#endif
		tint=gCardLastTransValue+(UWORD)gCardThisTransValue;
		buf[9]=*((char *)&tint+INT_LOW);
		buf[10]=*((char *)&tint+INT_HIGH);
	}
	else
	{
		//#ifdef _DEBUG_BOM_
		//	debug_printf("\ngCardThisTransValue...");
		//#endif
		buf[9]  = *((char *)&gCardThisTransValue+LONG_HIGH0);
		buf[10] = *((char *)&gCardThisTransValue+LONG_HIGH1);
	}
	
	lrc_gen2(buf,15);
	gDebugStep = 0x328b;
	ret = write_record(0x11,0x02,16,buf,0);
	if(ret)
	{
		gResponseStatus=CE_WRITEERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
	//2018/1/31 13:55
	memset(buf,0,16);
	buf[0] = 0x66;
	memcpy(&buf[1], &gB34[1], 14);
	//buf[15] = 0x66;
	lrc_gen2(buf, 15);
	gDebugStep = 0x328c;
	ret = write_record(0x11,0x03,16,buf,0);
	if(ret)
	{
		gResponseStatus=CE_WRITEERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
label_get_mac1:
	//取MAC1
	gDebugStep = 0x328d;
	if(SHSendAuthResp())
	{
		goto label_samerr;
	}
	buf_to_udword(bgTaskKeyOutbuf,gPosTransCnt);
	memcpy(mac1,&bgTaskKeyOutbuf[4],4);
	//debit for purchase
	memcpy(buf,"\x80\x54\x01\x00\x0F",5);
	udword_to_buf(gPosTransCnt,&buf[5]);
	memcpy(&buf[9],gTransTime,7);
	memcpy(&buf[16],mac1,4);
	gDebugStep = 0x328e;
#ifdef _TRANS_EXIT_CPU_
	shmt_prot_send(buf,20);
#endif
	memset(outbuf,0,sizeof(outbuf));
	ret = mifpro_icmd0(20,buf,outbuf);
	if(ret<0)
	{
		gResponseStatus = CE_CARDREMOVED;
		gGetCardInfoStep=0xff;
		return OP_ERR;//2015/8/26 14:16:25失败时 返回闪卡
		//goto label_readerr;
	}
#ifdef _TRANS_EXIT_CPU_
	shmt_prot_send(outbuf,ret);
#endif
	//2013/11/5 16:44:31
#ifdef _GETCARDINFO_CPU_
	printf("mifpro_icmd0...ret = %d\n",ret);
	printf("outbuf=");
	for(i=0;i<30;i++)
	{
		printf(" %02X",outbuf[i]);
	}
	debug_printf("\ngetcardinfo_MAC1 outbuf=%02X %02X\n",outbuf[ret-2],outbuf[ret-1]);
#endif
	
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
		if((outbuf[ret-2]==0x93)&&(outbuf[ret-1]==0x02))
		{
			gDebugStep = 0x328f;
			gResponseStatus=CE_MAC2ERR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		gDebugStep = 0x3290;
		goto label_invalid;
	}
	gDebugStep = 0x3291;
	if(ret!=10)
	{
		goto label_invalid;
	}
	memcpy(gTAC,outbuf,4);
	memcpy(mac2,&outbuf[4],4);
	gDebugStep = 0x3292;
	//credit sam for purchase
	memcpy(buf,"\x80\x72\x00\x00\x04",5);
	memcpy(&buf[5],mac2,4);
	n = 9;
#ifdef _TRANS_EXIT_CPU_
	shmt_prot_send(buf,n);
#endif
	ret=sam_apdu(bgSHpsamIndex,buf,n,outbuf,&outn,0,0);
	if(ret!=0)
	{
		goto label_samerr;
	}
#ifdef _TRANS_EXIT_CPU_
	shmt_prot_send(outbuf,outn);
#endif
	gDebugStep = 0x3293;
	if((outbuf[outn-2]!=0x90)||(outbuf[outn-1]!=0x00))
	{
		gResponseStatus=CE_MAC2ERR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
	
	return 0;
label_readerr:
		gResponseStatus=CE_READERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_invalid:
		gResponseStatus=CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		return OP_ERR;	
label_samerr:
		gResponseStatus=CE_PSAMERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;	
}
/*************************************************************
函数：trans_lock_cpu
功能：CPU卡锁卡
*************************************************************/
char trans_lock_cpu(UBYTE *mac2,UBYTE lock_level)
{
	UBYTE buf[30],outbuf[300];
	int ret;
	UBYTE outn,n;
	
	outn = lock_level;
	
	memcpy(buf,"\x00\x84\x00\x00\x04",5);
	ret = mifpro_icmd0(5,buf,outbuf);
	if(ret<0)
	{
		goto label_readerr;
	}
	if(ret!=6)
	{
		goto label_invalid;
	}
	memcpy(ICC,outbuf,4);
	if(memcmp(mac2,"\x00\x00\x00\x00",4)==0)
	{		
		memcpy(buf,"\x80\x1a\x45\x01\x10",5);
		memcpy(&buf[5],&gAppSn[2],8);
		memcpy(&buf[13],gAppSn,2);
		memcpy(&buf[15],"\xff\x00\x00\x00\x00\x00",6);
		n = 21;
		ret=sam_apdu(bgSHpsamIndex,buf,n,outbuf,&outn,0,0);
		if(ret!=0)
		{
			goto label_samerr;
		}
		if((outbuf[outn-2]!=0x90)||(outbuf[outn-1]!=0x00))
		{
			goto label_samerr;
		}
		memcpy(buf,"\x80\xfa\x05\x00\x10",5);
		memcpy(&buf[5],ICC,4);
		memcpy(&buf[9],"\x00\x00\x00\x00",4);
		memcpy(&buf[13],"\x84\x1E\x00\x01\x04\x80\x00\x00",8);
		#ifdef TEST_LOCK
		buf[16] = 0;
		#endif
		n = 21;
		ret=sam_apdu(bgSHpsamIndex,buf,n,outbuf,&outn,0,0);
		if(ret!=0)
		{
			goto label_samerr;
		}
		if((outbuf[outn-2]!=0x90)&&(outbuf[outn-2]!=0x61))
		{
			goto label_samerr;
		}
		if(outn == 2)
		{
			memcpy(buf,"\x00\xc0\x00\x00\x04",5);
			n = 5;
			ret=sam_apdu(bgSHpsamIndex,buf,n,outbuf,&outn,0,0);
			if(ret!=0)
			{
				goto label_samerr;
			}
			if(outn!=0x06)
			{
				goto label_samerr;
			}
		}
		memcpy(mac2,outbuf,4);
	}
	memcpy(buf,"\x84\x1E\x00\x01\x04",5);
	memcpy(&buf[5],mac2,4);
	#ifdef TEST_LOCK
		buf[3] = 0;
	#endif
	ret = mifpro_icmd0(5,buf,outbuf);
	if(ret<0)
	{
		goto label_readerr;
	}
	if((outbuf[0]!=0x90)||(outbuf[1]!=0x00))
	{
		goto label_readerr;
	}
	return 0;
label_readerr:
		gResponseStatus=CE_READERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_invalid:
		gResponseStatus=CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		return OP_ERR;	
label_samerr:
		gResponseStatus=CE_PSAMERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;	
}
/*************************************************************
函数：history_record
功能：读CPU卡历史交易记录
*************************************************************/
char history_record(UWORD index,UBYTE *outbuf)
{
	UBYTE ret,outbytes;
	UBYTE flag;
	UBYTE buf[20];
	int i;

	if(index<=8)
	{
		switch(index)
		{
			case 1:	flag = 1;
							break;
			case 2:	flag = 2;
							break;
			case 4:	flag = 3;
							break;
			case 8:	flag = 4;
							break;
			default:
							gResponseStatus=CE_BADPARAM;
							gGetCardInfoStep=0xff;
							return OP_ERR;
		}

		ret = get_record(0x06,flag,0x17,&outbuf[1]);//充值明细文件0008-->0006
		if(ret)
		{
			return OP_ERR;
		}
		outbuf[0] = flag-1;//flag
		return 0;
	}

	if(index<=0x2000)
	{
		switch(index)
		{
			case 0x0010:	flag = 1;
										break;
			case 0x0020:	flag = 2;
										break;
			case 0x0040:	flag = 3;
										break;
			case 0x0080:	flag = 4;
										break;
			case 0x0100:	flag = 5;
										break;
			case 0x0200:	flag = 6;
										break;
			case 0x0400:	flag = 7;
										break;
			case 0x0800:	flag = 8;
										break;
			case 0x1000:	flag = 9;
										break;
			case 0x2000:	flag = 10;
										break;				
			default:
							gResponseStatus=CE_BADPARAM;
							gGetCardInfoStep=0xff;
							return OP_ERR;										
		}
#ifdef DEBUG_RECORD
		printf("\ngCardStatus...2:%02x\n",gCardStatus);
#endif
		if(gCardStatus!=M1LOCKED)
		{
			ret = get_record(0x07,flag,0x10,&outbuf[1]);
#ifdef DEBUG_RECORD
			printf("\nZJB read 0x07...ret=%d\n",ret);
			printf("\n0x07 get outbuf :");
			for(i=0;i<16;i++)
			{
				printf("%02x ",outbuf[i+1]);
			}
			printf("\n");
#endif
		}
		else
		{
			ret = get_record(0x18,flag,0x17,&outbuf[1]);
#ifdef DEBUG_RECORD
			printf("\nZJB read 0x18...ret=%d\n",ret);
			printf("\n0x18 get outbuf :");
			for(i=0;i<16;i++)
			{
				printf("%02x ",outbuf[i+1]);
			}
			printf("\n");
#endif
		}
		if(ret)
		{
			return OP_ERR;
		}
		outbuf[0] = flag+3;//+4,20100206
		return 0;
	}
	if(index==0x4000)
	{
		gDebugStep = 0x09;
		memcpy(buf,"\x00\xa4\x00\x00\x02\x10\x03",7);
		ret = sam_apdu(bgSHpsamIndex,buf,7,outbuf,&outbytes,0,0);
		if(ret != 0)
		{
			bgSHPsamValid = 0;
			gResponseStatus=CE_PSAMERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
	  }	
	  gDebugStep = 0x0a;
		//if((outbuf[0] != 0x61) &&((UBYTE)outbuf[0] != 0x90)) //2013/11/17 22:53:52
		if((outbuf[outbytes-2] != 0x61) &&((UBYTE)outbuf[outbytes-2] != (UBYTE)0x90))
		{
			bgSHPsamValid = 0;
			gResponseStatus=CE_PSAMERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		gPSAMFileFlag = 0x00;
		bgSHPsamValid = 1;
		gDebugStep = 0x0b;
		ret = get_record(0x10,0x01,0x10,buf);
		if(ret)
		{
			return OP_ERR;
		}
		outbuf[0] = 14;//15
		//数据全为0 直接返回
		//if(memcmp(buf,"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",16)==0)
		//{
		//	memcpy(&outbuf[1],buf,16);
		//	memset(&outbuf[17],0,7);
		//	return 0;
		//}
		
		ret = 0xaa;
		for(flag=0;flag<16;flag++)
		{
			ret^=buf[flag];
		}
		if(ret)
		{
			gResponseStatus=CE_INVALIDCARD;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		gDebugStep = 0x0c;
		//计算TAC
		//os_lock();
		memcpy(bgTaskKeyInbuf,buf,13);
		bgTaskKeyInbuf[13]=gCityCode0;
		bgTaskKeyInbuf[14]=gCityCode1;
		memcpy(&bgTaskKeyInbuf[15],gThisCardSnr,4);
		memset(&bgTaskKeyInbuf[19],0,13);
		set_task_key(TASK_KEY_GET_SHB28);
		//os_unlock();		
		//if(bgEnUCOS) OSTimeDly(1);
		if(SHGetTacStep2())
		{
			gResponseStatus=CE_PSAMERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;	
		}
		memcpy(gTAC,bgTaskKeyOutbuf,4);
		gDebugStep = 0x0d;	
		if((buf[13]!=gTAC[0])||(buf[14]!=gTAC[1]))
		{
			gResponseStatus =CE_INVALIDCARD;
  		gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		memcpy(&outbuf[1],buf,16);
		memset(&outbuf[17],0,7);
		gDebugStep = 0x09;
		memcpy(buf,"\x00\xa4\x00\x00\x02\x10\x01",7);
		ret = sam_apdu(bgSHpsamIndex,buf,7,&outbuf[24],&outbytes,0,0);
		if(ret != 0)
		{
			bgSHPsamValid = 0;
			gResponseStatus=CE_PSAMERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
	  }	
	  gDebugStep = 0x0a;
		//if((outbuf[24] != 0x61) &&((UBYTE)outbuf[24] != 0x90))  //2013/11/17 22:57:49
		if((outbuf[24+outbytes-2] != 0x61) &&((UBYTE)outbuf[24+outbytes-2] != (UBYTE)0x90))
		{
			bgSHPsamValid = 0;
			gResponseStatus=CE_PSAMERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		gPSAMFileFlag = 0x55;
		bgSHPsamValid = 1;
		return 0;
	}
	if(index==0x8000)
	{
		ret = get_record(0x10,0x01,0x10,&outbuf[1]);
		if(ret)
		{
			return OP_ERR;
		}
		memcpy(buf,&outbuf[1],16);
		ret = 0xaa;
		for(flag=0;flag<16;flag++)
		{
			ret^=buf[flag];
		}
		if(ret)
		{
			gResponseStatus=CE_INVALIDCARD;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		memset(&outbuf[17],0,7);
		outbuf[0] = 15;//16
		return 0;
	}
	gResponseStatus=CE_BADPARAM;
	gGetCardInfoStep=0xff;
	return OP_ERR;
	

}
/*************************************************************
函数：trans_decrement_cpu
功能：CPU卡扣款交易
*************************************************************/
char trans_decrement_cpu(void)
{
	int ret;
	UBYTE buf[60],outbuf[300];
	UBYTE n,outn;
	UBYTE mac1[4],mac2[4];
//	UDWORD temp2,tint;
	
	//为了加快速度，将取认证码放在读卡中
	memcpy(buf,"\x80\xca\x00\x00\x09",5);
	ret = mifpro_icmd0(5,buf,outbuf);
	if(ret<0)
	{
		goto label_readerr;
	}
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
		goto label_invalid;
	}
	
	memcpy(bgTaskKeyInbuf,outbuf,9);
	set_task_key(TASK_KEY_SEND_AUTHCODE);
	gDebugStep = 0x83;
	
	memcpy(buf,"\x80\x50\x01\x02\x0B",5);				//普通消费
	if(gCardStatus!=M1LOCKED)
	{
		buf[2] = 0x03;
	}
//	if(gNotShCard==0x55)
//	{
//		buf[2] = 0x03;
//	}
	buf[5] = gKeyVersion;
	udword_to_buf(gCardThisTransValue,&buf[6]);
//	memset(&buf[6],0,4);
	memcpy(&buf[10],gPosID,6);
	ret = mifpro_icmd0(16,buf,outbuf);
	if(ret<0)
	{
		goto label_readerr;
	}
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
		goto label_invalid;
	}
	if(ret!=17)
	{
		goto label_invalid;
	}
	memset(buf,0,4);
	memcpy(&buf[1],&outbuf[6],3);
	buf_to_udword(buf,ret);					//透支额
	buf_to_udword(outbuf,gCardLastRemain);
	gCardLastRemain = gCardLastRemain-ret;
	buf_to_uword(&outbuf[4],gCardCnt);
	gKeyBacon = outbuf[9];
	gAluBacon = outbuf[10];
	memcpy(ICC,&outbuf[11],4);
	gCardThisRemain = gCardLastRemain - gCardThisTransValue;
	if(SHSendAuthResp())
	{
		goto label_samerr;
	}
	//init for sam 
	memcpy(buf,"\x80\x70\x00\x01\x28",5);
	memcpy(&buf[5],ICC,4);
	uword_to_buf(gCardCnt,&buf[9]);
//	memset(&buf[11],0,4);
	udword_to_buf(gCardThisTransValue,&buf[11]);
	buf[15] = 0x06;					//09:复合消费，06：电子钱包消费
	if(gCardStatus!=M1LOCKED)
	{
		buf[15] = 0x09;
	}
	if(gNotShCard==0x55)
	{
		buf[15] = 0x09;
	}
	memcpy(&buf[16],gTransTime,7);
	buf[23] = gKeyBacon;
	buf[24] = gAluBacon;
	udword_to_buf(gPosTransCnt,&buf[25]);
	memcpy(&buf[29],&gAppSn[2],8);
	memcpy(&buf[37],gAppSn,2);
	memcpy(&buf[39],"\xff\x00\x00\x00\x00\x00",6);
	memcpy(bgTaskKeyInbuf,&buf[5],40);
	set_task_key(TASK_KEY_INIT_FOR_SAM);
	//OSTimeDly(1);
	
//update cache
//	if(gNotShCard==0x55)
//	{
//		goto label_get_mac1;
//	}
	
	if(gCardStatus!=M1LOCKED)
	{
		trans_gen_card_record(0x11,buf);
		ret = write_record(0x07,0x00,16,buf,1);
		if(ret)
		{
			return OP_ERR;
		}
	}
	//改写公交记录区
//	memcpy(buf,gB29Buf,16);
//	buf[0]=gPosNo[1];
//	buf[1]=gPosNo[3];
//	buf[2]=gPosNo[0];
//	buf[2]|=0x80;
//	buf[3]=gTransTime[2];
//	buf[4]=gTransTime[3];
//	buf[5]=gTransTime[4];
//	buf[6]=gTransTime[5];
//	lrc_gen2(buf,15);
//	ret = write_record(0x10,0x02,16,buf);
//	if(ret)
//	{
//		return OP_ERR;
//	}
//	ret = write_record(0x10,0x03,16,buf);
//	if(ret)
//	{
//		return OP_ERR;
//	}
//	//更新卡上地铁专用扇区交易	
//	if((recordtime[0]!=gTransTime[1])||(recordtime[1]!=gTransTime[2]))
//	{
//		recordsum[0]=0;
//		recordsum[1]=0;
//		recordsum[2]=0;
//		recordcnt=0;
//	}
//	*((char *)&temp2+LONG_HIGH3)=0;
//	*((char *)&temp2+LONG_HIGH2)=recordsum[0];
//	*((char *)&temp2+LONG_HIGH1)=recordsum[1];
//	*((char *)&temp2+LONG_HIGH0)=recordsum[2];
//	if(gCardThisTransValue>(0xffffff-temp2))
//	{
//		temp2=0xffffff;
//	}
//	else
//	{
//		temp2=temp2+gCardThisTransValue;
//	}
//	buf[12]=*((char *)&temp2 +LONG_HIGH0);
//	buf[13]=*((char *)&temp2 +LONG_HIGH1);
//	buf[14]=*((char *)&temp2 +LONG_HIGH2);
//	if(recordcnt!=0xff)
//	{
//		recordcnt++;
//	}
//	buf[11]=recordcnt;
//	buf[12]=*((char *)&temp2 +LONG_HIGH0);
//	buf[13]=*((char *)&temp2 +LONG_HIGH1);
//	buf[14]=*((char *)&temp2 +LONG_HIGH2);	
//	
//	if((offsetnum>0x05)&&(offsetnum<10))
//	{
//		if(gStationTimeOverRideFlag&0x20)
//			buf[0]=0xa9;
//		else
//			buf[0]=0x88;
//	}
//	else
//	{	
//		if(gStationTimeOverRideFlag&0x20)
//			buf[0]=0xa0;
//		else
//			buf[0]=0x81;
//	}	
//
//	buf[1] = gTransTime[1];
//	buf[2] = gTransTime[2];
//	buf[3] = gTransTime[3];
//	buf[4] = gTransTime[4];
//	buf[5] = gTransTime[5];
//	buf[6] = gPosNo[0];
//	buf[7] = gPosNo[1];
//	buf[8] = gPosNo[3];
//	if(gStationTimeOverRideFlag&0x10)
//	{
//		tint=tint+(UWORD)gCardThisTransValue;
//		buf[9]=*((char *)&tint+INT_LOW);
//		buf[10]=*((char *)&tint+INT_HIGH);
//	}
//	else
//	{
//		buf[9]  = *((char *)&gCardThisTransValue+LONG_HIGH0);
//		buf[10] = *((char *)&gCardThisTransValue+LONG_HIGH1);
//	}
//	lrc_gen2(buf,15);
//	ret = write_record(0x11,0x02,16,buf);
//	if(ret)
//	{
//		return OP_ERR;
//	}
//	memset(buf,0,16);
//	buf[0] = 0x66;
//	buf[15] = 0x66;
//	ret = write_record(0x11,0x03,16,buf);
//	if(ret)
//	{
//		return OP_ERR;
//	}

//label_get_mac1:
	//取MAC1
	if(SHSendAuthResp())
	{
		goto label_samerr;
	}
	buf_to_udword(bgTaskKeyOutbuf,gPosTransCnt);
	memcpy(mac1,&bgTaskKeyOutbuf[4],4);
	//debit for purchase
	memcpy(buf,"\x80\x54\x01\x00\x0F",5);
	udword_to_buf(gPosTransCnt,&buf[5]);
	memcpy(&buf[9],gTransTime,7);
	memcpy(&buf[16],mac1,4);
	ret = mifpro_icmd0(20,buf,outbuf);
	if(ret<0)
	{
		goto label_readerr;
	}
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
		if((outbuf[ret-2]==0x93)&&(outbuf[ret-1]==0x02))
		{
			gResponseStatus=CE_MAC1ERR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		gDebugStep = outbuf[ret-2];
		goto label_invalid;
	}
	if(ret!=10)
	{
		goto label_invalid;
	}
	memcpy(gTAC,outbuf,4);
	memcpy(mac2,&outbuf[4],4);
	gDebugStep = 0x8d;
	//credit sam for purchase
	memcpy(buf,"\x80\x72\x00\x00\x04",5);
	memcpy(&buf[5],mac2,4);
	n = 9;
	ret=sam_apdu(bgSHpsamIndex,buf,n,outbuf,&outn,0,0);
	if(ret!=0)
	{
		goto label_samerr;
	}
	if((outbuf[outn-2]!=0x90)||(outbuf[outn-1]!=0x00))
	{
		gResponseStatus=CE_MAC2ERR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
//	gCardThisRemain = gCardLastRemain - gCardThisTransValue;
	return 0;
label_readerr:
		gResponseStatus=CE_READERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_invalid:
		gResponseStatus=CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		return OP_ERR;	
label_samerr:
		gResponseStatus=CE_PSAMERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;	
}
/*************************************************************
函数：trans_update_cpu
功能：CPU卡更新
*************************************************************/
char trans_update_cpu(UBYTE update_type,UBYTE *rec_buf)
{
	int ret;
	UBYTE buf[60],outbuf[300];
	UBYTE n,outn;
	UBYTE mac1[4],mac2[4];
//	UDWORD temp2,tint;
	
	//为了加快速度，将取认证码放在读卡中
	memcpy(buf,"\x80\xca\x00\x00\x09",5);
	ret = mifpro_icmd0(5,buf,outbuf);
	if(ret<0)
	{
		goto label_readerr;
	}
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
		goto label_invalid;
	}
	
	memcpy(bgTaskKeyInbuf,outbuf,9);
	set_task_key(TASK_KEY_SEND_AUTHCODE);
	gDebugStep = 0x83;
	
	memcpy(buf,"\x80\x50\x03\x02\x0B",5);				//普通消费
//	if(gCardStatus!=M1LOCKED)
//	{
//		buf[2] = 0x03;
//	}
	buf[5] = gKeyVersion;
	udword_to_buf(gCardThisTransValue,&buf[6]);
//	memset(&buf[6],0,4);
	memcpy(&buf[10],gPosID,6);
	ret = mifpro_icmd0(16,buf,outbuf);
	if(ret<0)
	{
		goto label_readerr;
	}
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
		goto label_invalid;
	}
	if(ret!=17)
	{
		goto label_invalid;
	}
	memset(buf,0,4);
	memcpy(&buf[1],&outbuf[6],3);
	buf_to_udword(buf,ret);					//透支额
	buf_to_udword(outbuf,gCardLastRemain);
	gCardLastRemain = gCardLastRemain-ret;
	buf_to_uword(&outbuf[4],gCardCnt);
	gKeyBacon = outbuf[9];
	gAluBacon = outbuf[10];
	memcpy(ICC,&outbuf[11],4);
	gCardThisRemain = gCardLastRemain - gCardThisTransValue;
	if(SHSendAuthResp())
	{
		goto label_samerr;
	}
	//init for sam 
	memcpy(buf,"\x80\x70\x00\x01\x28",5);
	memcpy(&buf[5],ICC,4);
	uword_to_buf(gCardCnt,&buf[9]);
  //memset(&buf[11],0,4);
	udword_to_buf(gCardThisTransValue,&buf[11]);
	buf[15] = 0x09;					//09:复合消费，06：电子钱包消费
	memcpy(&buf[16],gTransTime,7);
	buf[23] = gKeyBacon;
	buf[24] = gAluBacon;
	udword_to_buf(gPosTransCnt,&buf[25]);
	memcpy(&buf[29],&gAppSn[2],8);
	memcpy(&buf[37],gAppSn,2);
	memcpy(&buf[39],"\xff\x00\x00\x00\x00\x00",6);
	memcpy(bgTaskKeyInbuf,&buf[5],40);
	set_task_key(TASK_KEY_INIT_FOR_SAM);
	//OSTimeDly(1);
	
//update cache
	if(gNotShCard==0x55)
	{
		goto label_get_mac1;
	}
	
	if(gCardStatus!=M1LOCKED)
	{
		trans_gen_card_record(0x14,buf);
		ret = write_record(0x07,0x00,16,buf,1);
		if(ret)
		{
			return OP_ERR;
		}
	}
	if(update_type==1)
	{
		memset(buf,0,16);
		buf[0] = 0x55;
		buf[15] = 0x55;
		ret = write_record(0x11,0x03,16,buf,0);
		if(ret)
		{
			return OP_ERR;
		}
		ret = write_record(0x11,0x01,16,rec_buf,0);
		if(ret)
		{
			return OP_ERR;
		}
	}
	else
	{
		memset(buf,0,16);
		buf[0] = 0x66;
		buf[15] = 0x66;
		ret = write_record(0x11,0x03,16,buf,0);
		if(ret)
		{
			return OP_ERR;
		}
		ret = write_record(0x11,0x02,16,rec_buf,0);
		if(ret)
		{
			return OP_ERR;
		}
	}
label_get_mac1:
	//取MAC1
	if(SHSendAuthResp())
	{
		goto label_samerr;
	}
	buf_to_udword(bgTaskKeyOutbuf,gPosTransCnt);
	memcpy(mac1,&bgTaskKeyOutbuf[4],4);
	//debit for purchase
	memcpy(buf,"\x80\x54\x01\x00\x0F",5);
	udword_to_buf(gPosTransCnt,&buf[5]);
	memcpy(&buf[9],gTransTime,7);
	memcpy(&buf[16],mac1,4);
	ret = mifpro_icmd0(20,buf,outbuf);
	if(ret<0)
	{
		goto label_readerr;
	}
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
		if((outbuf[ret-2]==0x93)&&(outbuf[ret-1]==0x02))
		{
			gResponseStatus=CE_MAC1ERR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		gDebugStep = outbuf[ret-2];
		goto label_invalid;
	}
	if(ret!=10)
	{
		goto label_invalid;
	}
	memcpy(gTAC,outbuf,4);
	memcpy(mac2,&outbuf[4],4);
	gDebugStep = 0x8d;
	//credit sam for purchase
	memcpy(buf,"\x80\x72\x00\x00\x04",5);
	memcpy(&buf[5],mac2,4);
	n = 9;
	ret=sam_apdu(bgSHpsamIndex,buf,n,outbuf,&outn,0,0);
	if(ret!=0)
	{
		goto label_samerr;
	}
	if((outbuf[outn-2]!=0x90)||(outbuf[outn-1]!=0x00))
	{
		gResponseStatus=CE_MAC2ERR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
//	gCardThisRemain = gCardLastRemain - gCardThisTransValue;
	return 0;
label_readerr:
		gResponseStatus=CE_READERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_invalid:
		gResponseStatus=CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		return OP_ERR;	
label_samerr:
		gResponseStatus=CE_PSAMERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;	
}
/*************************************************************
函数：get_record
功能：读取文件中的记录
*************************************************************/
char get_record(UBYTE SFI_index,UBYTE rec_index,UBYTE len,UBYTE *rec_buf)
{
	int ret,i;
	UBYTE buf[20];
	memcpy(buf,"\x00\xb2",2);
	buf[2] = rec_index;
	buf[3] = (SFI_index<<3)|0x04;
	buf[4] = len;
	ret = mifpro_icmd0(5,buf,rec_buf);
/*	printf("\nget_record...ret=%d\n",ret);
	printf("rec_buf=");
	for(i=0;i<ret;i++)
	{
		printf(" %02x",rec_buf[i]);
	}
	printf("\n");*/
	if(ret<0)
	{
		goto label_readerr;
	}
	if((rec_buf[ret-2]!=0x90)||(rec_buf[ret-1]!=0x00))
	{
		if((rec_buf[ret-2]==0x6a)&&(rec_buf[ret-1]==0x83)) 
		{
			gResponseStatus=CE_NOLOG;
			gGetCardInfoStep=0xff;  
			return OP_ERR;
		}
		if((rec_buf[ret-2]==0x6a)&&(rec_buf[ret-1]==0x82)) 
		{
			gResponseStatus=CE_NOLOG;
			gGetCardInfoStep=0xff;  
			return OP_ERR;
		}
		goto label_invalid;
	}
	return 0;
label_readerr:
		gResponseStatus=CE_READERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_invalid:
		gResponseStatus=CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		return OP_ERR;
}

//#define _GET_CAPP_RECORD_
char get_capp_record(UBYTE SFI_index,UBYTE rec_index,UBYTE len,UBYTE *rec_buf)
{
	int ret;
	UBYTE buf[20];
	memcpy(buf,"\x00\xb2",2);
	buf[2] = rec_index;
	buf[3] = (SFI_index<<3);
	buf[4] = len;
	#ifdef _GET_CAPP_RECORD_
	shmt_prot_send(buf,5);
	#endif
	ret = mifpro_icmd0(5,buf,rec_buf);
	if(ret<0)
	{
		goto label_readerr;
	}
	#ifdef _GET_CAPP_RECORD_
	shmt_prot_send(rec_buf,ret);
	#endif
	if((rec_buf[ret-2]!=0x90)||(rec_buf[ret-1]!=0x00))
	{
		if((rec_buf[ret-2]==0x6a)&&(rec_buf[ret-1]==0x83)) 
		{
			gResponseStatus=CE_NOLOG;
			gGetCardInfoStep=0xff;  
			return OP_ERR;
		}
		if((rec_buf[ret-2]==0x6a)&&(rec_buf[ret-1]==0x82)) 
		{
			gResponseStatus=CE_NOLOG;
			gGetCardInfoStep=0xff;  
			return OP_ERR;
		}
		goto label_invalid;
	}
	return 0;
label_readerr:
		gResponseStatus=CE_READERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_invalid:
		gResponseStatus=CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		return OP_ERR;
}

/*************************************************************
函数：write_record
功能：向文件中写入记录
*************************************************************/
//#define _WR_REC_
char write_record(UBYTE SFI_index,UBYTE rec_index,UBYTE len,UBYTE *rec_buf,UBYTE cycleflag)
{
	int ret;
	UBYTE buf[60],outbuf[300];
	memcpy(buf,"\x80\xdc",2);
	buf[2] = rec_index;
	buf[3] = (SFI_index<<3)|0x04;
	//buf[3] = (SFI_index<<3);
	if(cycleflag)
	{
		buf[2] = 0;
		buf[3] = (SFI_index<<3)|0x03;
		//buf[3] = (SFI_index<<3);
	}
	buf[4] = len;
	memcpy(&buf[5],rec_buf,len);
#ifdef _WR_REC_
	shmt_prot_send(buf,5+len);
#endif	
	ret = mifpro_icmd0(5+len,buf,outbuf);
	if(ret<0)
	{
		goto label_writeerr;
	}
#ifdef _WR_REC_
	shmt_prot_send(outbuf,ret);
#endif
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
		goto label_invalid;
	}
	return 0;
label_writeerr:
		gResponseStatus=CE_WRITEERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_invalid:
		gResponseStatus=CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		return OP_ERR;
}

//#define _WR_CAPP_REC_
char write_capp_record(UBYTE SFI_index,UBYTE rec_index,UBYTE len,UBYTE *rec_buf,UBYTE cycleflag)
{
	int ret;
	UBYTE buf[256],outbuf[300];
	memcpy(buf,"\x80\xdc",2);
	buf[2] = rec_index;
	buf[3] = (SFI_index<<3);
	if(cycleflag)
	{
		buf[2] = 0;
		buf[3] = (SFI_index<<3);
	}
	buf[4] = len;
	memcpy(&buf[5],rec_buf,len);
	#ifdef _WR_CAPP_REC_
	shmt_prot_send(buf,5+len);
	#endif
	ret = mifpro_icmd0(5+len,buf,outbuf);
	if(ret<0)
	{
		goto label_writeerr;
	}
	#ifdef _WR_CAPP_REC_
	shmt_prot_send(outbuf,ret);
	#endif
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
		goto label_invalid;
	}
	return 0;
label_writeerr:
		gResponseStatus=CE_WRITEERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_invalid:
		gResponseStatus=CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		return OP_ERR;
}

#ifdef DEBUG_PRINT
#define _debug_pf_mobile_ticket_process_
#endif
char pf_mobile_ticket_process(void)
{
	int ret,ret2;
	int rtn;
	UBYTE data[256];
	UBYTE buf[256];
	UBYTE outbuf[256], tempbuf[1000];
	UBYTE obuf1[256],obuf2[256];
	UBYTE pfbuf[256];
	UBYTE cnt,i;
	UBYTE flag;

//2013/11/12 20:06:25
	unsigned char	tempData[600], bytCheck;
	int	intInsertLen = 1;
	int intLength;

#ifdef _debug_test_
UDWORD	tint0=0,tint1=0;
#endif

#ifdef _debug_pf_mobile_ticket_process_
//debug_printf("pf_mobile_ticket_process 1\n");
#endif

//20130901
	if(bgSMPADF != 2)
	{
		bgSMPADF = 2;
		set_task_key(TASK_KEY_SELECT_ADF);
	}

	//ret = select_1001or1002_app(0);//可不用 当前目录记录下来
	//if(ret ==0 )bpgSmpsamAppSjtFlag = 0;
	//select ppse
	gDebugStep = 0x3083;
	memcpy(buf,"\x00\xa4\x04\x00\x0e",5);
	memcpy(&buf[5],"2PAY.SYS.DDF01",14);
	
#ifdef _debug_pf_mobile_ticket_process_
	debug_printf("2PAY buf :\n");
	for(i=0;i<19;i++) debug_printf(" %02x",buf[i]);
#endif

	ret = mifpro_icmd0(19,buf,outbuf);
	
#ifdef _debug_pf_mobile_ticket_process_
	debug_printf("\n2PAY outbuf :[%d]\n",ret);
	for(i=0;i<ret;i++) debug_printf(" %02x",outbuf[i]);
#endif

	if(ret<0)
	{
		goto label_readerr;
	}

////if((outbuf[ret-2]==0x6a)&&((UBYTE)outbuf[ret-1]==(UBYTE)0x82))
//	if((outbuf[ret-2]==0x6a)&&((UBYTE)outbuf[ret-1] & (UBYTE)0x80))
//	{
//		return CE_NO_PPSE;
//	}
	
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
 	{
		goto label_readerr;
	}
#ifdef DEBUG_TIME_CPU
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x10", 3);
#endif	

	gCPUflag = 0x55;
#ifdef _debug_test_
	tint0 = GetTickcount();
#endif

	memset(buf,0,256);

#ifdef _debug_pf_mobile_ticket_process_
	debug_printf("\nbuf= \n");
	for(i=0;i<128;i++) debug_printf(" %02x",buf[i]);
#endif
	
	ret =  pf_mobile_ticket_get_info(outbuf,ret-2,buf);

#ifdef DEBUG_TIME_CPU
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x11", 3);
#endif

#ifdef _debug_test_
	tint1 = GetTickcount();
	debug_printf("\n pf_mobile_ticket_get_info(%d)ret=%d",tint1-tint0,ret);
#endif

#ifdef _debug_pf_mobile_ticket_process_
	printf("\npf_mobile_ticket_get_info ret =%d info_buf=\n",ret);
	for(i = 0;i < 128;i++)
		printf("%02X ",buf[i]);
	printf("\n");
#endif

#ifdef _GETCARDINFO_CPU_
	printf("\npf_mobile_ticket_get_info ret =%d \n",ret);
#endif
//	if(ret==28416)
//	{
//		goto transport_label;
//	}
	if(ret < 0)
	{
		if((ret == -302) || (ret == -315))
		{
				gGetCardInfoStep=0xff;
				gResponseStatus = CE_INVALIDCARD;
				outbuf[0] = PBOC_READCARD;
				outbuf[1] = gReadCmdTimes;
				outbuf[2] = gResponseStatus;		
				cnt = 3;
				shmt_prot_send(outbuf,cnt);
				gStatus=IDLE_STATUS;	
				//shmt_bom_func_err(outbuf,outbytes);
				return OP_ERR;
			
		}
		//20190315 Add
		else if(ret == -400)
		{
//transport_label:
			rtn=sh_transport_get_info(data);
			if(rtn!=0)
			{
				return OP_ERR;
			}
			if(memcmp(transport_15_data,"\x02\x00\x29\x00",4)==0)
			{//本地卡
				if((ReadMode==0x90)||(ReadMode==0x80))
				{//闸机模式
					//块号1			
					outbuf[0] = 0x12;
					outbuf[1] = gReadCmdTimes;
					outbuf[2] = CE_OK;
					outbuf[3] = 0x01;
					memcpy(&outbuf[4],gThisCardSnr,4);//UID
					memcpy(&outbuf[8],transport_15_data,4);//发卡机构
					memcpy(&outbuf[12],&transport_15_data[10],10);//应用序列号
					outbuf[22]=transport_17_data[10];//卡类型
					memcpy(&outbuf[23],&transport_15_data[24],4);//有效期
					outbuf[27] = *((char *)&gCardThisRemain+LONG_HIGH0);//卡余额
					outbuf[28] = *((char *)&gCardThisRemain+LONG_HIGH1);
					outbuf[29] = *((char *)&gCardThisRemain+LONG_HIGH2);
					//outbuf[30]=gCardStatus;
					outbuf[30] = transport_15_data[9];//卡状态
					memcpy(&outbuf[31],&capp_init[4],2);//卡计数器(脱机交易序号)
					outbuf[33] = transport_15_data[8];//卡版本号
					//另一条记录设备号
					#ifdef _GETCARDINFO_CPU_
					printf("\ngBFirst :%02x\n",gBFirst);
					#endif
					if(gBFirst == 32)
					{
						memcpy(obuf1,gB32,16);
						memcpy(obuf2,gB33,16);
				    if((lrc_chk2(&obuf1[0],15)!=0)||(lrc_chk2(&obuf2[0],15)!=0))
				    {	
				    	memset(&outbuf[34],0x00,3);
						}
						else
						{
							memcpy(&outbuf[34],&gB33[6],3);
						}
					}
					else
					{
						memcpy(obuf1,gB32,16);
						memcpy(obuf2,gB33,16);
				    if((lrc_chk2(&obuf1[0],15)!=0)||(lrc_chk2(&obuf2[0],15)!=0))
				    {
				    	memset(&outbuf[34],0x00,3);							
						}
						else
						{
							memcpy(&outbuf[34],&gB32[6],3);
						}
					}
					//换乘信息
					memcpy(&outbuf[37],info,5);
					
					cnt=42;
					shmt_prot_send(outbuf,cnt);
					
					//块号2
					outbuf[0] = 0x12;
					outbuf[1] = gReadCmdTimes;
					outbuf[2] = CE_OK;
					outbuf[3] = 0x82;
					//printf("\ngBFirst...2 = %d\n",gBFirst);
					flag = 0;
					switch(gBFirst)
					{
						case 32:
			    			if(check_card_get_b32()) goto label_err_remove;
			    			memcpy(&outbuf[4],gB32,16);
								ret=outbuf[4]&0xc0;
			    			if((lrc_chk2(&outbuf[4],15)!=0)||(ret != 0x40))
			    			{
				     			memset(&outbuf[4],0,16);
				     			memcpy(&outbuf[4],gB33,16);
				     			//if((lrc_chk2(&outbuf[4],15)!=0)||(ret != 0x80))
				     			if((lrc_chk2(&outbuf[4],15)!=0))
			    				{
				    				memset(&outbuf[4],0,16);
				    	 			outbuf[4]=0x81;
				     				outbuf[19]=0x81;
					   				recordtime[0]=0;
					   				recordtime[1]=0;
				     				flag = 1;
				  				}
				  			}			
			    			break;
						case 33:     
			    			if(check_card_get_b33()) goto label_err_remove;
			    			memcpy(&outbuf[4],gB33,16);	
	        			ret=outbuf[4]&0xc0;
			    			if((lrc_chk2(&outbuf[4],15)!=0)||(ret != 0x80))
			    			{
				    			memset(&outbuf[4],0,16);
				    			memcpy(&outbuf[4],gB32,16);
				    			if((lrc_chk2(&outbuf[4],15)!=0))
				    			{
				    				memset(&outbuf[4],0,16);
				    	 			outbuf[4]=0x81;
				     				outbuf[19]=0x81;
					   				recordtime[0]=0;
					   				recordtime[1]=0;
				     				flag = 1;
				    			}
				  			}			
			    			break;
						default:   
				  			ret=get_metro_rec(&outbuf[4]);
				  			if(ret!=0)
				  			{
					   			if(ret==0x55) goto label_err_remove;
					   			memset(&outbuf[4],0,16);
					   			outbuf[4]=0x81;	//出站是81进站是41 
				     			outbuf[19]=0x81;
					   			recordtime[0]=0;
					   			recordtime[1]=0;
				     			flag = 1;
								}
								break;
					}	
					if(flag==0)
					{
						recordsum[0]=outbuf[18];
						recordsum[1]=outbuf[17];
						recordsum[2]=outbuf[16];
						recordcnt=outbuf[15];
						recordtime[0]=outbuf[5];
						recordtime[1]=outbuf[6];
					}
					cnt=20;
					shmt_prot_send(outbuf,cnt);
					
					gStatus=TRANS_STATUS;		
					return 0;
				}
				else
				{//91:BOM模式
					//块号1			
					outbuf[0] = 0x12;
					outbuf[1] = gReadCmdTimes;
					outbuf[2] = CE_OK;
					outbuf[3] = 0x01;
					//UID
					memcpy(&outbuf[4],gThisCardSnr,4);
					//发卡机构
					memcpy(&outbuf[8],transport_15_data,4);
					//应用序列号
					memcpy(&outbuf[12],&transport_15_data[10],10);
					//卡类型
					outbuf[22]=transport_17_data[10];
					//押金
					outbuf[23]=transport_15_data[29];
					//有效期
					memcpy(&outbuf[24],&transport_15_data[24],4);
					//应用启动日期
					memcpy(&outbuf[28],&transport_15_data[20],4);
					//卡余额
					outbuf[32] =*((char *)&gCardThisRemain+LONG_HIGH0);
					outbuf[33] =*((char *)&gCardThisRemain+LONG_HIGH1);
					outbuf[34] =*((char *)&gCardThisRemain+LONG_HIGH2);
					//透支限额
					memcpy(&outbuf[35],&capp_init[6],3);
					//卡状态
					outbuf[38] = transport_15_data[9];
					//卡计数器(脱机交易序号)
					memcpy(&outbuf[39],&capp_init[4],2);
					//卡版本号
					outbuf[41] = transport_15_data[8];
					//换乘信息：
					memcpy(&outbuf[42],info,5);
					//累计乘车金额
					memcpy(&outbuf[47],&leisure[2],3);
					//累计乘车次数
					memcpy(&outbuf[50],leisure,2);
					//进出状态					
					outbuf[52] = gMetroIoFlag;				
					cnt=53;
					shmt_prot_send(outbuf,cnt);
					
					//块号2
					outbuf[0] = 0x12;
					outbuf[1] = gReadCmdTimes;
					outbuf[2] = CE_OK;
					outbuf[3] = 0x02;
					memcpy(&outbuf[4],gB32,16);
					cnt=20;
					shmt_prot_send(outbuf,cnt);
					
					//块号3
					outbuf[0] = 0x12;
					outbuf[1] = gReadCmdTimes;
					outbuf[2] = CE_OK;
					outbuf[3] = 0x83;
					memcpy(&outbuf[4],gB33,16);
					cnt=20;
					shmt_prot_send(outbuf,cnt);
					gStatus=TRANS_STATUS;
					return 0;
				}
			}
			else
			{//异地卡
				if((ReadMode==0x90)||(ReadMode==0x80))
				{//90:闸机模式
					//块号1			
					outbuf[0] = 0x13;
					outbuf[1] = gReadCmdTimes;
					outbuf[2] = CE_OK;
					outbuf[3] = 0x01;
					memcpy(&outbuf[4],gThisCardSnr,4);//UID
					memcpy(&outbuf[8],transport_15_data,4);//发卡机构
					memcpy(&outbuf[12],&transport_15_data[10],10);//应用序列号
					outbuf[22]=transport_17_data[10];//卡类型
					memcpy(&outbuf[23],&transport_15_data[24],4);//有效期
					outbuf[27] = *((char *)&gCardThisRemain+LONG_HIGH0);//卡余额
					outbuf[28] = *((char *)&gCardThisRemain+LONG_HIGH1);
					outbuf[29] = *((char *)&gCardThisRemain+LONG_HIGH2);
					outbuf[30] = transport_15_data[9];//卡状态
					memcpy(&outbuf[31],&capp_init[4],2);//卡计数器(脱机交易序号)
					outbuf[33] = transport_15_data[8];//卡版本号
					//memset(&outbuf[34],0x00,3);
					//另一条记录设备号
					if(transport_1A_data[122]==0x55) 
					{
						//交易设备编号：3字节
						if(transport_1A_data[120] & 0x04)//超乘返回末笔交易设备编号
						{
							outbuf[34]	=	transport_1A_data[106];
							outbuf[35]	=	transport_1A_data[107];
							outbuf[36]	=	transport_1A_data[109];
						}
						else
						{
							outbuf[34]	=	transport_1A_data[34];
							outbuf[35]	=	transport_1A_data[50];
							outbuf[36]	=	transport_1A_data[66];
						}

					}
					else 
					{
						//交易设备编号：3字节
						if(transport_1A_data[120] & 0x04)//超乘返回末笔交易设备编号
						{
							outbuf[34]	=	transport_1A_data[106];
							outbuf[35]	=	transport_1A_data[107];
							outbuf[36]	=	transport_1A_data[109];
						}
						else
						{
							outbuf[34]	=	transport_1A_data[26];
							outbuf[35]	=	transport_1A_data[42];
							outbuf[36]	=	transport_1A_data[58];
						}
					}
					//换乘信息（行业信息+换乘时间 MMDDHHmm）
					memcpy(&outbuf[37],&transport_1A_data[115],5);
					cnt=42;
					shmt_prot_send(outbuf,cnt);
				
					//块号2：地铁专用交易记录：16字节
					outbuf[0] = 0x13;
					outbuf[1] = gReadCmdTimes;
					outbuf[2] = CE_OK;
					outbuf[3] = 0x82;
					if(memcmp(temp_crc,&transport_1A_data[124],4)!=0)
					{
						outbuf[4]	= 0x81;
						memset(&outbuf[5],0x00,14);
						outbuf[19] = 0x81;
					}
					else
					{
						if(transport_1A_data[122]==0x55)
						{
							//进出标志
							outbuf[4]	= transport_1A_data[120];
							//使用时间年、月、日、时、分（年只有低字节，共5字节）
							outbuf[5]	=	transport_1A_data[68];
							outbuf[6]	=	transport_1A_data[69];
							outbuf[7]	=	transport_1A_data[70];
							outbuf[8]	=	transport_1A_data[71];
							outbuf[9]	=	transport_1A_data[72];
						
							//交易设备编号：3字节
							outbuf[10]	=	transport_1A_data[26];
							outbuf[11]	=	transport_1A_data[42];
							outbuf[12]	=	transport_1A_data[58];
						}
						else
						{
							//进出标志
							outbuf[4]	= transport_1A_data[121];
							//使用时间年、月、日、时、分（年只有低字节，共5字节）
							outbuf[5]	=	transport_1A_data[75];
							outbuf[6]	=	transport_1A_data[76];
							outbuf[7]	=	transport_1A_data[77];
							outbuf[8]	=	transport_1A_data[78];
							outbuf[9]	=	transport_1A_data[79];
						
							//交易设备编号：3字节
							outbuf[10]	=	transport_1A_data[34];
							outbuf[11]	=	transport_1A_data[50];
							outbuf[12]	=	transport_1A_data[66];	
						}
						//交易金额：2字节
						outbuf[13] = transport_1A_data[98];
						outbuf[14] = transport_1A_data[97];
						//累计次数：1字节
						outbuf[15] =	transport_1A_data[114];
						//累计金额，3字节16进制，低字节在前
						outbuf[16]	=	transport_1A_data[112];
						outbuf[17]	=	transport_1A_data[111];
						outbuf[18]	=	transport_1A_data[110];
						//校验
						lrc_gen2(&outbuf[4],15);
					}
					
					//memcpy(JTBData,&outbuf[4],16);
					//gStationTimeOverRideFlag = outbuf[4];
					
					recordsum[0]=outbuf[16];
					recordsum[1]=outbuf[17];
					recordsum[2]=outbuf[18];
					recordcnt=outbuf[15];
					recordtime[0]=outbuf[5];
					recordtime[1]=outbuf[6];
					#ifdef _GETCARDINFO_CPU_
					printf("\nrecordsum :%02x%02x%02x\n",recordsum[0],recordsum[1],recordsum[2]);
					printf("recordcnt :%02x\n",recordcnt);
					printf("recordtime :%02x%02x\n",recordtime[0],recordtime[1]);
					#endif
					cnt=20;
					shmt_prot_send(outbuf,cnt);
					
					gStatus=TRANS_STATUS;
					return 0;	
				}
				else
				{//91:BOM模式
					//块号1			
					outbuf[0] = 0x13;
					outbuf[1] = gReadCmdTimes;
					outbuf[2] = CE_OK;
					outbuf[3] = 0x01;
					//UID
					memcpy(&outbuf[4],gThisCardSnr,4);
					//发卡机构
					memcpy(&outbuf[8],transport_15_data,4);
					//应用序列号
					memcpy(&outbuf[12],&transport_15_data[10],10);
					//卡类型
					outbuf[22]=transport_17_data[10];
					//押金
					outbuf[23]=transport_15_data[29];
					//有效期
					memcpy(&outbuf[24],&transport_15_data[24],4);
					//应用启动日期
					memcpy(&outbuf[28],&transport_15_data[20],4);
					//卡余额
					outbuf[32] = *((char *)&gCardThisRemain+LONG_HIGH0);
					outbuf[33] = *((char *)&gCardThisRemain+LONG_HIGH1);
					outbuf[34] = *((char *)&gCardThisRemain+LONG_HIGH2);
					//透支限额
					memcpy(&outbuf[35],&capp_init[6],3);
					//卡状态
					outbuf[38] = transport_15_data[9];
					//卡计数器(脱机交易序号)
					memcpy(&outbuf[39],&capp_init[4],2);
					//卡版本号
					outbuf[41] = transport_15_data[8];
					//换乘信息
					memcpy(&outbuf[37],&transport_1A_data[115],5);
					//累计乘车金额
					outbuf[47]=transport_1A_data[112];
					outbuf[48]=transport_1A_data[111];
					outbuf[49]=transport_1A_data[110];
					//累计乘车次数
					memcpy(&outbuf[50],&transport_1A_data[113],2);
					//进出状态					
					outbuf[52]	=	transport_1A_data[122];				
					cnt=53;
					shmt_prot_send(outbuf,cnt);
					
					//块号2：地铁专用交易记录：16字节
					outbuf[0] = 0x13;
					outbuf[1] = gReadCmdTimes;
					outbuf[2] = CE_OK;
					outbuf[3] = 0x02;
					//进出标志
					outbuf[4]	= transport_1A_data[120];
					//使用时间年、月、日、时、分（年只有低字节，共5字节）
					outbuf[5]	=	transport_1A_data[68];
					outbuf[6]	=	transport_1A_data[69];
					outbuf[7]	=	transport_1A_data[70];
					outbuf[8]	=	transport_1A_data[71];
					outbuf[9]	=	transport_1A_data[72];
					//交易设备编号：3字节
					outbuf[10]	=	transport_1A_data[26];
					outbuf[11]	=	transport_1A_data[42];
					outbuf[12]	=	transport_1A_data[58];
					//交易金额：2字节
					//outbuf[13] = *((char *)&gCardThisTransValue+LONG_HIGH0);
					//outbuf[14] = *((char *)&gCardThisTransValue+LONG_HIGH1);
					outbuf[13] = transport_1A_data[98];
					outbuf[14] = transport_1A_data[97];
					//累计次数：1字节
					outbuf[15] =	transport_1A_data[114];
					//累计金额，3字节16进制，低字节在前
					outbuf[16]	=	transport_1A_data[112];
					outbuf[17]	=	transport_1A_data[111];
					outbuf[18]	=	transport_1A_data[110];
					//校验
					lrc_gen2(&outbuf[4],15);
					cnt=20;
					shmt_prot_send(outbuf,cnt);
					
					//块号3：地铁专用交易记录：16字节
					outbuf[0] = 0x13;
					outbuf[1] = gReadCmdTimes;
					outbuf[2] = CE_OK;
					outbuf[3] = 0x83;
					//进出标志
					outbuf[4]	= transport_1A_data[121];
					//使用时间年、月、日、时、分（年只有低字节，共5字节）
					outbuf[5]	=	transport_1A_data[75];
					outbuf[6]	=	transport_1A_data[76];
					outbuf[7]	=	transport_1A_data[77];
					outbuf[8]	=	transport_1A_data[78];
					outbuf[9]	=	transport_1A_data[79];
					//交易设备编号：3字节
					outbuf[10]	=	transport_1A_data[34];
					outbuf[11]	=	transport_1A_data[50];
					outbuf[12]	=	transport_1A_data[66];
					//交易金额：2字节
					outbuf[13] = *((char *)&gCardThisTransValue+LONG_HIGH0);
					outbuf[14] = *((char *)&gCardThisTransValue+LONG_HIGH1);
					//累计次数：1字节
					outbuf[15] =	transport_1A_data[114];
					//累计金额，3字节16进制，低字节在前
					outbuf[16]	=	transport_1A_data[112];
					outbuf[17]	=	transport_1A_data[111];
					outbuf[18]	=	transport_1A_data[110];
					//校验
					lrc_gen2(&outbuf[4],15);
					cnt=20;
					shmt_prot_send(outbuf,cnt);
					
					gStatus=TRANS_STATUS;
					return 0;
				}
			}
		}
		else
		{
			gGetCardInfoStep=0xff;
			gResponseStatus = CE_READERROR;
			outbuf[0] = PBOC_READCARD;
			outbuf[1] = gReadCmdTimes;
			outbuf[2] = gResponseStatus;		
			cnt = 3;
			shmt_prot_send(outbuf,cnt);
			gStatus=IDLE_STATUS;	
			//shmt_bom_func_err(outbuf,outbytes);
			return OP_ERR;
		}
	}
	else if( ret > 0)
	{
		gGetCardInfoStep=0xff;
		gResponseStatus = CE_READERROR;
		outbuf[0] = PBOC_READCARD;
		outbuf[1] = gReadCmdTimes;
		outbuf[2] = gResponseStatus;		
		cnt = 3;
		shmt_prot_send(outbuf,cnt);
		gStatus=IDLE_STATUS;	
		//shmt_bom_func_err(outbuf,outbytes);
		return OP_ERR;
		//return CE_UNKNOWN;
	}
/*
if(ret) {
    //gResponseStatus=buf[0];
		gGetCardInfoStep=0xff;
		
		outbuf[0] = PBOC_READCARD;
		outbuf[1] = gReadCmdTimes;
		outbuf[2] = CE_READERROR;
		outbuf[3] = 0xaa;
		outbuf[4] = 0xbb;	
		outbuf[5] = *((char*)&ret+1);	
		outbuf[6] = *((char*)&ret+0);	
		cnt = 7;
		shmt_prot_send(outbuf,cnt);
		gStatus=IDLE_STATUS;		
		return OP_ERR;	
}
*/
#ifdef _debug_pf_mobile_ticket_process_
		debug_printf("\nret =%X \n",ret);
		debug_printf("\npf_mobile_ticket_process 3 buf=\n");
		for(i=0;i<128;i++) debug_printf(" %02X",buf[i]);
		debug_printf("\n");
#endif
	
	gGetCardInfoStep=0xff;

/*
if(buf[0] != 0) {
	outbuf[0] = PBOC_READCARD;
	outbuf[1] = gReadCmdTimes;
	outbuf[2] = buf[0];
	outbuf[3] = 0xaa;
	outbuf[4] = 0xcc;			
	cnt = 5;
	shmt_prot_send(outbuf,cnt);
	gStatus=IDLE_STATUS;
	return OP_ERR;
}
*/

/*if(buf[1] != 0x90){//浦发普通卡
	outbuf[0] = PBOC_READCARD;
	outbuf[1] = gReadCmdTimes;
	outbuf[2] = CE_OK;
	outbuf[3] = 0x01;
	memcpy(&outbuf[4],&buf[2],18);
	outbuf[22]=buf[2+18+3];outbuf[23]=buf[2+18+2];outbuf[24]=buf[2+18+1];outbuf[25]=buf[2+18];
	outbuf[26]=buf[2+22+2];outbuf[27]=buf[2+22+1];outbuf[28]=buf[2+22];
	cnt = 29;
	//tempbuf[0] = 29;
	//memcpy(&tempbuf[1], outbuf, 29);
	//cnt = sh_InsertDLE(tempbuf, 29 + 1);
	shmt_prot_send(outbuf,cnt);
	//2013/11/12 19:56:02
	

	ret = pf_mobile_ticket_get_0221(buf);
		if(ret < 0)
	{
		if((ret == -302) || (ret == -315))
		{
				gGetCardInfoStep=0xff;
				gResponseStatus = CE_INVALIDCARD;
				outbuf[0] = PBOC_READCARD;
				outbuf[1] = gReadCmdTimes;
				outbuf[2] = gResponseStatus;
				cnt = 3;
				shmt_prot_send(outbuf,cnt);
				gStatus=IDLE_STATUS;
				//shmt_bom_func_err(outbuf,outbytes);
				return OP_ERR;
			
		}
		else{
				gGetCardInfoStep=0xff;
				gResponseStatus = CE_READERROR;
				outbuf[0] = PBOC_READCARD;
				outbuf[1] = gReadCmdTimes;
				outbuf[2] = gResponseStatus;
				cnt = 3;
				shmt_prot_send(outbuf,cnt);
				gStatus=IDLE_STATUS;
				//shmt_bom_func_err(outbuf,outbytes);
				return OP_ERR;
				}
	}
	else if( ret > 0)
	{
				gGetCardInfoStep=0xff;
				gResponseStatus = CE_READERROR;
				outbuf[0] = PBOC_READCARD;
				outbuf[1] = gReadCmdTimes;
				outbuf[2] = gResponseStatus;
				cnt = 3;
				shmt_prot_send(outbuf,cnt);
				gStatus=IDLE_STATUS;
				//shmt_bom_func_err(outbuf,outbytes);
				return OP_ERR;
			//return CE_UNKNOWN;
		}
	outbuf[0] = PBOC_READCARD;
	outbuf[1] = gReadCmdTimes;
	outbuf[2] = CE_OK;
	outbuf[3] = 0x82;
  if(gCPUflag==0x55) 	buf[3] = 0x92;
	memcpy(&outbuf[4],&buf[27],66);
	outbuf[4+34]=buf[27+34+2];outbuf[4+34+2]=buf[27+34]; //当月轨道交通累计金额（低字节在前）
	outbuf[4+39]=buf[27+39+1];outbuf[4+39+1]=buf[27+39]; //实际交易金额（低字节在前）
	cnt = 70;
	shmt_prot_send(outbuf,70);
	//tempbuf[cnt] = 70;
	//memcpy(&tempbuf[cnt+1], outbuf, 70);
	//cnt += sh_InsertDLE(&tempbuf[cnt], 70 + 1);
	//2013/11/12 19:57:48
	//writecom(csc_comm, tempbuf, cnt);
	
	
	gStatus=TRANS_STATUS;
  return CE_OK;
}
else {//浦发员工卡
	outbuf[0] = EMPLOY_READCARD;
	outbuf[1] = gReadCmdTimes;
	outbuf[2] = CE_OK;
	outbuf[3] = 0x01;
	memcpy(&outbuf[4],&buf[2],42);
	outbuf[23]=buf[2+19+3];outbuf[24]=buf[2+19+2];outbuf[25]=buf[2+19+1];outbuf[26]=buf[2+19];
	cnt = 46; 
	shmt_prot_send(outbuf,cnt);

	outbuf[0] = EMPLOY_READCARD;
	outbuf[1] = gReadCmdTimes;
	outbuf[2] = CE_OK;
	outbuf[3] = 0x82; 
  if(gCPUflag==0x55) 	buf[3] = 0x92;
	//memcpy(&outbuf[4],&buf[52],45);
	memcpy(&outbuf[4],&buf[44],45);
	outbuf[4+34]=buf[44+34+2];outbuf[4+34+2]=buf[44+34]; //当月轨道交通累计金额（低字节在前）
	outbuf[4+39]=buf[44+39+1];outbuf[4+39+1]=buf[44+39]; //实际交易金额（低字节在前）
	cnt = 49;
	shmt_prot_send(outbuf,cnt);
	gStatus=TRANS_STATUS;
  return CE_OK;
}	*/
	//printf("\nbuf[1]==%02x\n",buf[1]);
if(buf[1] == 0xA1) 
	{//合法ODA卡
		outbuf[0] = ODA_READOK;
		outbuf[1] = gReadCmdTimes;
		outbuf[2] = CE_OK;
		outbuf[3] = 0x01;
		memcpy(&outbuf[4],&buf[2],37);
		//备份ODA块号1数据
		memcpy(gODAData,&buf[2],37);

		cnt = 41;
		shmt_prot_send(outbuf,cnt);
		
		ret = oda_ticket_get_zone(buf);
#ifdef _debug_pf_mobile_ticket_process_
		printf("\noda_ticket_get_zone = %d", ret);
		for(i = 0;i < 50;i++)
		printf(" %02X ",buf[i]);
		printf("\n");
#endif
		if(ret < 0)
		{
			if((ret == -302) || (ret == -315))
			{
				gGetCardInfoStep=0xff;
				gResponseStatus = CE_INVALIDCARD;
				outbuf[0] = ODA_READOK;
				outbuf[1] = gReadCmdTimes;
				outbuf[2] = gResponseStatus;		
				cnt = 3;
				shmt_prot_send(outbuf,cnt);
				gStatus=IDLE_STATUS;	
				//shmt_bom_func_err(outbuf,outbytes);
				return OP_ERR;
			}
			else
			{
				gGetCardInfoStep=0xff;
				gResponseStatus = CE_READERROR;
				outbuf[0] = ODA_READOK;
				outbuf[1] = gReadCmdTimes;
				outbuf[2] = gResponseStatus;		
				cnt = 3;
				shmt_prot_send(outbuf,cnt);
				gStatus=IDLE_STATUS;	
				//shmt_bom_func_err(outbuf,outbytes);
				return OP_ERR;
			}
		}
		else if( ret > 0)
		{
			gGetCardInfoStep=0xff;
			gResponseStatus = CE_READERROR;
			outbuf[0] = ODA_READOK;
			outbuf[1] = gReadCmdTimes;
			outbuf[2] = gResponseStatus;		
			cnt = 3;
			shmt_prot_send(outbuf,cnt);
			gStatus=IDLE_STATUS;	
			//shmt_bom_func_err(outbuf,outbytes);
			return OP_ERR;
			//return CE_UNKNOWN;
		}
		outbuf[0] = ODA_READOK;
		outbuf[1] = gReadCmdTimes;
		outbuf[2] = CE_OK;
		outbuf[3] = 0x82;
		//memcpy(&outbuf[4],&buf[2],48);
		memcpy(&outbuf[4],&buf[2],34);
		outbuf[38] = buf[37];//交易金额；低字节在前
		outbuf[39] = buf[36];
		outbuf[40] = buf[40];//累计金额：低字节在前
		outbuf[41] = buf[39];
		outbuf[42] = buf[38];
		memcpy(&outbuf[43],&buf[41],9);
		//备份ODA块号2数据
		memcpy(&gODAData[37],&buf[2],48);

		cnt = 52;
		shmt_prot_send(outbuf,cnt);
	
		gStatus=TRANS_STATUS;
		//gCardFamily = FAMILY_CPU_PF;
	  	return CE_OK;
	}
	else if(buf[1] == 0xA2)
	{//ODA无效卡
		if(gMode == 0x60)
		{//BOM返回未开通的账户信息 
			ret = oda_zone_ticket_get_pan(buf);
#ifdef _debug_pf_mobile_ticket_process_
			printf("\noda_zone_ticket_get_pan = %d\n", ret);
#endif
			if(ret < 0)
			{
				if((ret == -302) || (ret == -315))
				{
					gGetCardInfoStep=0xff;
					gResponseStatus = CE_INVALIDCARD;
					outbuf[0] = ODA_READOK;
					outbuf[1] = gReadCmdTimes;
					outbuf[2] = gResponseStatus;		
					cnt = 3;
					shmt_prot_send(outbuf,cnt);
					gStatus=IDLE_STATUS;	
					//shmt_bom_func_err(outbuf,outbytes);
					return OP_ERR;
				}
				else
				{
					gGetCardInfoStep=0xff;
					gResponseStatus = CE_READERROR;
					outbuf[0] = ODA_READOK;
					outbuf[1] = gReadCmdTimes;
					outbuf[2] = gResponseStatus;		
					cnt = 3;
					shmt_prot_send(outbuf,cnt);
					gStatus=IDLE_STATUS;	
					//shmt_bom_func_err(outbuf,outbytes);
					return OP_ERR;
				}
			}
			else if( ret > 0)
			{
					gGetCardInfoStep=0xff;
					gResponseStatus = CE_READERROR;
					outbuf[0] = ODA_READOK;
					outbuf[1] = gReadCmdTimes;
					outbuf[2] = gResponseStatus;		
					cnt = 3;
					shmt_prot_send(outbuf,cnt);
					gStatus=IDLE_STATUS;	
					//shmt_bom_func_err(outbuf,outbytes);
					return OP_ERR;
				//return CE_UNKNOWN;
			}
			outbuf[0] = ODA_READERR;
			outbuf[1] = gReadCmdTimes;
			outbuf[2] = CE_OK;
			outbuf[3] = 0x81;
			memcpy(&outbuf[4],&buf[2],16);
			cnt = 20;

			shmt_prot_send(outbuf,cnt);
			gStatus=TRANS_STATUS;
			//gCardFamily = FAMILY_CPU_PF;
	  		return CE_OK;
		}else
		{
			gGetCardInfoStep = 0xff;
			gResponseStatus = CE_INVALIDCARD;
			outbuf[0] = ODA_READOK;
			outbuf[1] = gReadCmdTimes;
			outbuf[2] = gResponseStatus;
			outbuf[3] = 0x01;
			gStatus=IDLE_STATUS;
			cnt = 3;	
			shmt_prot_send(outbuf,cnt);
			return OP_ERR;
		}
	}	
	else if(buf[1] != 0x90) 
	{//浦发普通卡
		outbuf[0] = PBOC_READCARD;
		outbuf[1] = gReadCmdTimes;
		outbuf[2] = CE_OK;
		outbuf[3] = 0x01;
		memcpy(&outbuf[4],&buf[2],18);
		outbuf[22]=buf[2+18+3];outbuf[23]=buf[2+18+2];outbuf[24]=buf[2+18+1];outbuf[25]=buf[2+18];
		outbuf[26]=buf[2+22+2];outbuf[27]=buf[2+22+1];outbuf[28]=buf[2+22];
		cnt = 29;
		//tempbuf[0] = 29;
		//memcpy(&tempbuf[1], outbuf, 29);
		//cnt = sh_InsertDLE(tempbuf, 29 + 1);
		shmt_prot_send(outbuf,cnt);
		//2013/11/12 19:56:02
		
		ret = pf_mobile_ticket_get_0221(buf);
		if(ret < 0)
		{
			if((ret == -302) || (ret == -315))
			{
					gGetCardInfoStep=0xff;
					gResponseStatus = CE_INVALIDCARD;
					outbuf[0] = PBOC_READCARD;
					outbuf[1] = gReadCmdTimes;
					outbuf[2] = gResponseStatus;		
					cnt = 3;
					shmt_prot_send(outbuf,cnt);
					gStatus=IDLE_STATUS;	
					//shmt_bom_func_err(outbuf,outbytes);
					return OP_ERR;
				
			}
			else
			{
				gGetCardInfoStep=0xff;
				gResponseStatus = CE_READERROR;
				outbuf[0] = PBOC_READCARD;
				outbuf[1] = gReadCmdTimes;
				outbuf[2] = gResponseStatus;		
				cnt = 3;
				shmt_prot_send(outbuf,cnt);
				gStatus=IDLE_STATUS;	
				//shmt_bom_func_err(outbuf,outbytes);
				return OP_ERR;
			}
		}
		else if( ret > 0)
		{
				gGetCardInfoStep=0xff;
				gResponseStatus = CE_READERROR;
				outbuf[0] = PBOC_READCARD;
				outbuf[1] = gReadCmdTimes;
				outbuf[2] = gResponseStatus;		
				cnt = 3;
				shmt_prot_send(outbuf,cnt);
				gStatus=IDLE_STATUS;	
				//shmt_bom_func_err(outbuf,outbytes);
				return OP_ERR;
			//return CE_UNKNOWN;
		}
		outbuf[0] = PBOC_READCARD;
		outbuf[1] = gReadCmdTimes;
		outbuf[2] = CE_OK;
		outbuf[3] = 0x82;
		if(gCPUflag==0x55) 	buf[3] = 0x92;
		memcpy(&outbuf[4],&buf[27],66);
		outbuf[4+34]=buf[27+34+2];outbuf[4+34+2]=buf[27+34]; //当月轨道交通累计金额（低字节在前）
		outbuf[4+39]=buf[27+39+1];outbuf[4+39+1]=buf[27+39]; //实际交易金额（低字节在前）
		cnt = 70;
		shmt_prot_send(outbuf,70);
		//tempbuf[cnt] = 70;
		//memcpy(&tempbuf[cnt+1], outbuf, 70);
		//cnt += sh_InsertDLE(&tempbuf[cnt], 70 + 1);
		//2013/11/12 19:57:48
		//writecom(csc_comm, tempbuf, cnt);
	
		gStatus=TRANS_STATUS;
		//gCardFamily = FAMILY_CPU_PF;
	  	return CE_OK;
	}
	else 
	{//浦发员工卡
		outbuf[0] = EMPLOY_READCARD;
		outbuf[1] = gReadCmdTimes;
		outbuf[2] = CE_OK;
		outbuf[3] = 0x01;
		memcpy(&outbuf[4],&buf[2],42);
		outbuf[23]=buf[2+19+3];outbuf[24]=buf[2+19+2];outbuf[25]=buf[2+19+1];outbuf[26]=buf[2+19];
		cnt = 46; 
		shmt_prot_send(outbuf,cnt);
	
		outbuf[0] = EMPLOY_READCARD;
		outbuf[1] = gReadCmdTimes;
		outbuf[2] = CE_OK;
		outbuf[3] = 0x82; 
		if(gCPUflag==0x55) 	buf[3] = 0x92;
		//memcpy(&outbuf[4],&buf[52],45);
		memcpy(&outbuf[4],&buf[44],45);
		outbuf[4+34]=buf[44+34+2];outbuf[4+34+2]=buf[44+34]; //当月轨道交通累计金额（低字节在前）
		outbuf[4+39]=buf[44+39+1];outbuf[4+39+1]=buf[44+39]; //实际交易金额（低字节在前）
		cnt = 49;
		shmt_prot_send(outbuf,cnt);
		gStatus=TRANS_STATUS;
		//gCardFamily = FAMILY_CPU_EM;
  		return CE_OK;
	}	

label_err_remove:
	check_card_err_rpt(CE_CARDREMOVED);
	gStatus=IDLE_STATUS;
	//
	set_auto_abort_flag();  //20110216add
	gGetCardInfoStep = 0xff;
	return;	
label_readerr:
		gResponseStatus=CE_READERROR;
		gGetCardInfoStep=0xff;
		goto label_exit;
label_invalid:
		gResponseStatus=CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		goto label_exit;
label_exit:
		outbuf[0] = PBOC_READCARD;
		outbuf[1] = gReadCmdTimes;
		outbuf[2] = gResponseStatus;
		outbuf[3] = 0xaa;
		outbuf[4] = 0xcc;			
		cnt = 5;
		shmt_prot_send(outbuf,cnt);
		gStatus=IDLE_STATUS;		
		return OP_ERR;		
}


//end of file
#endif