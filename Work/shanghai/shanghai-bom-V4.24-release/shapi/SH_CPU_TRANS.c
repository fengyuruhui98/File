//SH_CPU_TRANS.c
#ifndef _SH_CPU_TRANS_C_
#define _sh_CPU_TRANS_C_

#define TEST_LOCK
//#define	DEBUG_TIME_CPU

#define ENTRY_UPDATE   0x01
#define EXIT_UPDATE    0x02

UBYTE gPosID[6];
UBYTE gKeyVersion;
UBYTE gAppSn[10];
UBYTE gYaJin;
UBYTE gKeyBacon;				//��Կ��־
UBYTE gAluBacon;				//�㷨��־
UBYTE ICC[4];						//α�����
UBYTE gB29Buf[20];
UBYTE gPSAMFileFlag;
UDWORD gPurseOffset;
UBYTE gCPUValid;
UBYTE gNotShCard;
//UBYTE Rand[8];
UBYTE gCityCode[2];
UBYTE gRecCpu0017[48];

UBYTE gODARec1[100];
UBYTE gODARec2[100];
//2013/11/20 9:22:26
UBYTE gPPSMode = 0; //1��PPS�� 0��PPS��



/*****************************************************
������getcardinfo_CPU
���ܣ����Ϻ�CPU��ͨ������Ϣ
*****************************************************/
//#define _DEBUG_GETCARDINFO_CPU_

#ifdef DEBUG_PRINT
#define _GETCARDINFO_CPU_
#endif
char getcardinfo_CPU(void)
{
	int ret, i;
	UBYTE buf[64], ppsbuf[48];
	UBYTE outbuf[300];
	UBYTE cnt;
	UBYTE tbuf[80];
	
	//2013/11/18 10:34:52
	UBYTE bGjFlag = 0;
	UBYTE bPfFlag = 0;  
	UBYTE temp_IssuerCode[8];//�����������룺����ס�����������ж�
	
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
	
	
	//Ϊ��request
	//ret=mcml_request(PICC_REQALL,buf);//PICC_REQSTD
	for(i = 0;i < 3; i++)
	{
		ret=mcml_request(PICC_REQALL,buf);//PICC_REQSTD
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
	if(ret!=0)
	{
		#ifdef _GETCARDINFO_CPU_
		debug_printf("\x0d\x0a getcardinfo_CPU mcml_anticoll Err\n");
		#endif
		return OP_ERR;
	}
	gDebugStep = 0x3081;
	ret=mcml_select(gThisCardSnr,&cnt);
	if(ret!=0)
	{
		#ifdef _GETCARDINFO_CPU_
		debug_printf("\x0d\x0a getcardinfo_CPU mcml_select Err\n");
		#endif 
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
		mcml_pwr_off();
		return OP_ERR;
	}
#ifdef DEBUG_TIME_CPU
	memcpy(ppsbuf, "\x01\x00\x01", 3);
	memcpy(&ppsbuf[3], buf, 3);
	ReaderResponse(csc_comm, 0x00, 0xF0, ppsbuf, 6);
#endif	
	//��Ƭpps
	#ifdef _GETCARDINFO_CPU_
			debug_printf("gPPS_CPU buf=%02X %02X %02X\n",buf[0],buf[1],buf[2]);
	#endif

	//�Ƿ�PPS	 
	 //if(gPPSMode == 1)
	 if( (gPPSMode == 1) || (gMode == BOM_MODE))
	 {
	 		goto card_wo_pps;
	 }
	 
#ifdef CARD_PPS_ON	
	if((buf[0] > 1) && (buf[1]&0x10)) 
	{
		if((buf[2]&0x44) == 0x44) 
		{
			//2014/10/17 15:19:13���ֻ���е�424
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
		if((buf[2]&0x44) == 0x44) 
		{
			//2014/10/17 15:19:13���ֻ���е�424
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

//  //�������Ȼ����ַ�����
//  if((buf[0] > 1) && (buf[1]&0x70))
//  {
//  	cnt = (buf[1]&0x70)>>4;
//  	switch(cnt){
//  	   case 7: cnt = 3;break;
//  	   case 3: cnt = 2;break;
//  	   case 1: cnt = 1;break;
//  	}
//  	
//  	if(memcmp(&buf[cnt+2],"\x20\x90",2) == 0) //FM,��������
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

	gDebugStep = 0x3083;
	memcpy(buf,"\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01\x00",15); //������Ӧ��
	#ifdef _GETCARDINFO_CPU_
	debug_printf("getcardinfo_CPU 2\n");
	#endif
	memset(outbuf,0,sizeof(outbuf));
	ret = mifpro_icmd0(15,buf,outbuf);
	//printf("ret=%d\n",ret);
	if(ret<0)
	{
		goto label_readerr;
	}
	#ifdef _GETCARDINFO_CPU_
		debug_printf("getcardinfo_CPU 3 ret: %d\n", ret);
	#endif
	gDebugStep = 0x3084;
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
		if( (outbuf[ret-2] == 0x6a) && (outbuf[ret-1] == 0x81) )
		{
			gResponseStatus=CE_INVALIDCARD;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		if( (outbuf[ret-2] == 0x93) && (outbuf[ret-1] == 0x03) )
		{
			gResponseStatus=CE_INVALIDCARD;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		if(ret>2)
		{
			gCPUValid = 0x00;
			goto get_cardtype;
		}
		#ifdef _GETCARDINFO_CPU_
				for(i=0;i<80;i++)
					printf("*%02X",outbuf[i]);
				debug_printf("getcardinfo_CPU2 outbuf=%02X %02X\n",outbuf[ret-2],outbuf[ret-1]);
		#endif
JTB_label:
		//2013/5/22 10:23:58 �ַ��ֻ�֧��
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
				//����01������
				return CE_OK_PF_MOBILE;	
			}	
		}
		gDebugStep = 0x3085;
		mcml_pwr_off();
//		delay_ms(2);
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
  //Ѱ��\x9f\x0c
  for(cnt=0;cnt<ret-2;cnt++){
 	  if(memcmp(&outbuf[cnt],"\x9f\x0c",2) == 0) 
 	  	  {
 	  	   cnt = cnt + 3;
 	  	   goto label_find_9f0c;
 	      }
 	}
 	
 	//û��Ҫread 0015
 	//read 0015
	gDebugStep = 0x30c3;
	memcpy(buf,"\x00\xb0\x95\x00\x1e",5);
	#ifdef _GETCARDINFO_CPU_
		shmt_prot_send(buf,5);
	#endif
	ret = mifpro_icmd0(5,buf,outbuf);
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
  //if((!gM1Size0x53Flag)&&(memcmp(gCityCode,"\x20\x00",2) == 0))  goto label_invalid;//2012/5/11 11:27:31,ȥ��CPU��0x53���ж�
  	gApplicationTypeTag = outbuf[cnt+8]; //Ӧ�����ͱ�ʶ	1
	gCardStatus = outbuf[cnt+8];
	cardversion = outbuf[cnt+9];
	
	memcpy(gAppSn,&outbuf[cnt+10],10);
	memcpy(StartUseDay,&outbuf[cnt+20],4);
	memcpy(gCardExpiredDate,&outbuf[cnt+24],4);
	gCardType = outbuf[cnt+28];
	gYaJin = outbuf[cnt+29];

//�жϰ�����	
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
		//LongToByte(tpWhite4020.white_head.ContentNumber,(unsigned char *)&tpWhite4020.white_head.ContentNumber);
		printf("tpWhite4020.white_head.ContentNumber :%04x\n",tpWhite4020.white_head.ContentNumber);
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

	gDebugStep = 0x308c;
	if(gCPUValid==0)
	{
		mcml_pwr_off();
//		delay_ms(2);
		goto label_invalid;
	}
	gDebugStep = 0x308d;
	if(gPSAMFileFlag == 0x55) 
	{
		goto skip_select;
	}
	bgSHpsamIndex = SAMLOCATION_5;
	memcpy(buf,"\x00\xa4\x00\x00\x02\x10\x01",7);//cpuӦ���л� (����Ҫ�л�)
	i = 7;
	gDebugStep = 0x308e;
	ret = sam_apdu(bgSHpsamIndex,buf,i,outbuf,&cnt,0,0);//û��Ҫ sam���л���Ҫ����Ѱ������
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
	//�ⲿ��֤
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
	memcpy(buf,"\x80\x1a\x45\x01\x10",5);
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
   {//�Ϻ�������ɽ��//2015/9/2 16:14:02ֻ���Ϻ���
		  if(memcmp(gAppSn, "\x20\x00", 2) == 0)
		  {
			  //�ⲿ��֤
			  gDebugStep = 0x309d;
			  memcpy(buf,"\x00\x82\x00\x02\x08",5);
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
			printf("\nzdd-final\n");
			//������ר�ü�¼
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
			gDebugStep = 0x30a3;
			ret = get_record(0x11,0x01,16,outbuf);
			if(ret)
			{
				return OP_ERR;
			}
			memcpy(gB32,outbuf,16);
			lrc3_gen(gB32,16);
			gStationTimeOverRideFlag = gB32[0];
			
			//�����ϴν��׽��
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
			ret = get_record(0x11,0x03,16,outbuf);
			if(ret)
			{
				return OP_ERR;
			}
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
							//�����ϴν��׽��
				gCardLastTransValue = 0;
				*((char *)&gCardLastTransValue+INT_HIGH) = gB32[10]; //INT_HIGH
				*((char *)&gCardLastTransValue+INT_LOW) = gB32[9];
			}
			if(gBFirst == 33){
							//�����ϴν��׽��
				gCardLastTransValue = 0;
				*((char *)&gCardLastTransValue+INT_HIGH) = gB33[10]; //INT_HIGH
				*((char *)&gCardLastTransValue+INT_LOW) = gB33[9];
			}
			
			#ifdef _GETCARDINFO_CPU_
				debug_printf("\x0d\x0a gBFirst=%d gCardLastTransValue = %08X ",gBFirst,gCardLastTransValue);
			#endif
	}
	else //��ؿ�
	{
			gDebugStep = 0x30a8;
			//2014/9/25 15:22:31�ڻ�����ͨ�����У��ն˻�������0x15�ļ���Ӧ�����ͱ�ʶ�ֶε����ݣ������û�������״̬�жϣ���0x00�������һ����������Ϊ0x00�򱾴ν�����ֹ
			if(gCardStatus == 0)
				goto label_invalid;
				
			ret = get_capp_record(0x17,0x09,48,outbuf);
			if(ret)
			{
				return OP_ERR;
			}
			memcpy(gRecCpu0017,outbuf,48);//2011-3-29 11:10:24
			memset(gB29,0,16);
			memset(gB29Buf,0,20);
			memset(info,0,5);
			memset(leisure,0,5);
			gDebugStep = 0x30a9;
		if((UBYTE)outbuf[26] == (UBYTE)0x55)  
	    {  //��վ
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
	    {  //��վ
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
	    {  //�����վ
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
	
	//Ϊ�˼ӿ��ٶȣ���ȡ��֤����ڶ�����
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
		debug_printf(" ****8050 ret=%02X %02X**** \n",outbuf[ret-2],outbuf[ret-1]);
	#endif 
	if((outbuf[ret-2]==0x62)||(outbuf[ret-1]==0x81))//2015/8/21 15:33:15 CPU������ 8050ʧ�ܷ���6281ʱ������M1����
	{
	#ifdef _GETCARDINFO_CPU_
		debug_printf(" ****8050=6281**** \n");
	#endif 
		mcml_pwr_off();	
	  set_timeout(8000);
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
	buf_to_udword(buf,gPurseOffset);					//͸֧��
	buf_to_udword(outbuf,gCardPurse);
	gCardLastRemain = gCardPurse-gPurseOffset;
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
	gCardFamily = FAMILY_CPU_CARD;
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
������trans_entry_cpu
���ܣ�CPU����վ����
*************************************************************/
//#define _TRANS_ENTRY_CPU_
char trans_entry_cpu(void)
{
	int ret;
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
	buf_to_udword(buf,ret);					//͸֧��
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

	gCardThisRemain = gCardLastRemain;//for gCardThisTransValue=0
	gDebugStep = 0x3182;
	//init for sam 
	memcpy(buf,"\x80\x70\x00\x01\x28",5);
	memcpy(&buf[5],ICC,4);
	uword_to_buf(gCardCnt,&buf[9]);
	memset(&buf[11],0,4);
	buf[15] = 0x09;					//09:�������ѣ�06������Ǯ������
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
	
	//д����ר�ü�¼
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
	
	//д0017�ļ�	
	if(gNotShCard==0x55) //��ؿ�
	{
		//cancelled in 2011-7-22 15:59:03,modified back
		//		//2011-3-29 16:43:25
		//		memcpy(buf,gRecCpu0017,48);
		//		//дר�ü�¼
		//		if(gStationTimeOverRideFlag == 0x00) 
		//		{
		//			buf[26] = 0x41;		
		//		}
		//		else 
		//		{
		//			if(gStationTimeOverRideFlag==0x18)
		//			{
		//				if(temp2&0x08)
		//					buf[26]=	0x78;
		//				else
		//					buf[26]= 0x71;
		//			}
		//			else
		//			{
		//				buf[26]=gStationTimeOverRideFlag<<1;
		//				buf[26]+=0x40;
		//				if(buf[26]==0x50)
		//					if(temp2&0x08)
		//						buf[26]= 0x59;
		//			}
		//			if(gStationTimeOverRideFlag&0x08)
		//			{
		//				buf[35]=buf33[0];
		//				buf[36]=buf33[1];
		//				if((recordtime[0]!=gTransTime[1])||(recordtime[1]!=gTransTime[2]))
		//				{
		//					recordsum[0]=0;
		//					recordsum[1]=0;
		//					recordsum[2]=0;
		//					recordcnt=0;
		//				}
		//				//buf[11]=recordcnt;
		//				buf[39]=recordsum[0];
		//				buf[38]=recordsum[1];
		//				buf[37]=recordsum[2];
		//				goto skip_in_0017;
		//			}
		//		}
		//		buf[27]=gTransTime[1];
		//		buf[28]=gTransTime[2];
		//		buf[29]=gTransTime[3];
		//		buf[30]=gTransTime[4];
		//		buf[31]=gTransTime[5];
		//		buf[32]=gPosNo[0];
		//		buf[33]=gPosNo[1];
		//		buf[34]=gPosNo[3];
		//		buf[35]  = *((char *)&gCardThisTransValue+LONG_HIGH0);
		//		buf[36]  = *((char *)&gCardThisTransValue+LONG_HIGH1);
		//		if((recordtime[0]!=gTransTime[1])||(recordtime[1]!=gTransTime[2]))
		//		{
		//			recordsum[0]=0;
		//			recordsum[1]=0;
		//			recordsum[2]=0;
		//			recordcnt=0;
		//		}
		//		//buf[11]=recordcnt;
		//		buf[39]=recordsum[0];
		//		buf[38]=recordsum[1];
		//		buf[37]=recordsum[2];
		//skip_in_0017:
		//		//
		//		memcpy(buf,"\x09\x2e\x00",3);//1-1	�������ѱ�־	1	BCD��09
		//                                //2-2	��¼����	1	HEX��2E
		//                                //3-3	Ӧ��������־λ	1	HEX��00
		//		buf[3] = 0; //4-4	�������Ѽ�¼�汾��	1	HEX
		//  	buf[4] = 1;
		//  	memcpy(&buf[5],gPosID,6); //6-11	�ն˻���ţ����д��룩	6	BCD
		//  	//udword_to_buf(gCardThisRemain,&buf[11]);//12-15	�����ڲ��ۣ�
		//  	memcpy(&buf[11],"\x00\x00\x02\x58",4);
		//  	memcpy(&buf[15],gTransTime,7); //16-19	��������	4	YYYYMMDD
		//                                 //20-22	����ʱ��	3	HHMMSS
		//    memset(&buf[22],0,4); //23-26	ͨ�ø������Ѽ�¼MAC	4	HEX,��ʱ��0
		//    buf[26] = 0x41;//������
		//    gDebugStep = 0x3186;
		
		//дר�ü�¼
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
		memcpy(buf,"\x09\x2e\x00",3);//1-1	�������ѱ�־	1	BCD��09
                                //2-2	��¼����	1	HEX��2E
                                //3-3	Ӧ��������־λ	1	HEX��00
		buf[3] = 0; //4-4	�������Ѽ�¼�汾��	1	HEX
  	buf[4] = 1;
  	memcpy(&buf[5],gPosID,6); //6-11	�ն˻���ţ����д��룩	6	BCD
  	//udword_to_buf(gCardThisRemain,&buf[11]);//12-15	�����ڲ��ۣ�
  	memset(&buf[11],0,4);
  	memcpy(&buf[15],gTransTime,7); //16-19	��������	4	YYYYMMDD
                                 //20-22	����ʱ��	3	HHMMSS
    memset(&buf[22],0,4); //23-26	ͨ�ø������Ѽ�¼MAC	4	HEX,��ʱ��0
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
	//���¿��ϵ���ר����������	   
 	gDebugStep = 0x3187;
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
	//дB32
	lrc_gen2(buf,15);
	gDebugStep = 0x3188;
	ret = write_record(0x11,0x01,16,buf,0);
	if(ret)
	{
		gResponseStatus=CE_WRITEERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
	gDebugStep = 0x3189;
	memset(buf,0,16);
	buf[0] = 0x55;
	buf[15] = 0x55;
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
		return OP_ERR;//2015/8/26 14:16:25ʧ��ʱ ��������
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
������trans_exit_cpu
���ܣ�CPU����վ����
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
	#ifdef _TRANS_EXIT_CPU_
	int i;
	#endif
	//Ϊ�˼ӿ��ٶȣ���ȡ��֤����ڶ�����
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
	
	if(gNotShCard==0x55) //��ؿ�
	{
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
		
			memcpy(buf,"\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01\x00",15); //������Ӧ��
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
			
			//		ȡ��֤���
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
 
		
	}
	
	//init for capp purchase

	memcpy(buf,"\x80\x50\x03\x02\x0B",5);
	buf[5] = gKeyVersion;
	udword_to_buf(gCardThisTransValue,&buf[6]);
	memcpy(&buf[10],gPosID,6);
	gDebugStep = 0x3281;
	#ifdef _TRANS_EXIT_CPU_
	//shmt_prot_send(buf,16);
	#endif 
	ret = mifpro_icmd0(16,buf,outbuf);
	if(ret<0)
	{
		goto label_readerr;
	}
	#ifdef _TRANS_EXIT_CPU_
	//shmt_prot_send(outbuf,ret);
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
	buf_to_udword(buf,ret);					//͸֧��
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
	buf[15] = 0x09;//09:�������ѣ�06������Ǯ������
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
	if(gNotShCard==0x55) //��ؿ�
	{
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
		//		memcpy(buf,"\x09\x2e\x00",3);//1-1	�������ѱ�־	1	BCD��09
		//                                 //2-2	��¼����	1	HEX��2E
		//                                 //3-3	Ӧ��������־λ	1	HEX��00
		//		buf[3] = 0; //4-4	�������Ѽ�¼�汾��	1	HEX
		//  	buf[4] = 0;
		//  	memcpy(&buf[5],gPosID,6); //6-11	�ն˻���ţ����д��룩	6	BCD
		//  	//udword_to_buf(gCardThisRemain,&buf[11]);//12-15	�����ڲ��ۣ�
		//  	memset(&buf[11],0,4);
		//  	memcpy(&buf[15],gTransTime,7); //16-19	��������	4	YYYYMMDD
		//                                   //20-22	����ʱ��	3	HHMMSS
		//    memset(&buf[22],0,4); //23-26	ͨ�ø������Ѽ�¼MAC	4	HEX,��ʱ��0
		//    buf[26] = 0x81;//???
		//    //
		//    gDebugStep = 0x3287;

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
		memcpy(buf,"\x09\x2e\x00",3);//1-1	�������ѱ�־	1	BCD��09
                                //2-2	��¼����	1	HEX��2E
                                //3-3	Ӧ��������־λ	1	HEX��00
		buf[3] = 0; //4-4	�������Ѽ�¼�汾��	1	HEX
  	buf[4] = 0;
  	memcpy(&buf[5],gPosID,6); //6-11	�ն˻���ţ����д��룩	6	BCD
  	//udword_to_buf(gCardThisRemain,&buf[11]);//12-15	�����ڲ��ۣ�
  	memset(&buf[11],0,4);
  	memcpy(&buf[15],gTransTime,7); //16-19	��������	4	YYYYMMDD
                                 //20-22	����ʱ��	3	HHMMSS
    memset(&buf[22],0,4); //23-26	ͨ�ø������Ѽ�¼MAC	4	HEX,��ʱ��0
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
	//��д������¼��
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
	//���¿��ϵ���ר����������	
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
	
	
		//ret = get_record(0x06,1,0x17,&recbuf[1]);//��ֵ��ϸ�ļ�0008-->0006
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
	memset(buf,0,16);
	buf[0] = 0x66;
	buf[15] = 0x66;
	gDebugStep = 0x328c;
	ret = write_record(0x11,0x03,16,buf,0);
	if(ret)
	{
	  gResponseStatus=CE_WRITEERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
label_get_mac1:
	//ȡMAC1
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
		return OP_ERR;//2015/8/26 14:16:25ʧ��ʱ ��������
		//goto label_readerr;
	}
	#ifdef _TRANS_EXIT_CPU_
	shmt_prot_send(outbuf,ret);
	#endif
	//2013/11/5 16:44:31
	#ifdef _GETCARDINFO_CPU_
		printf("ret = %d\n",ret);
		printf("outbuf=");
				for(i=0;i<30;i++)
					printf("*%02X",outbuf[i]);
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
������trans_lock_cpu
���ܣ�CPU������
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
������history_record
���ܣ���CPU����ʷ���׼�¼
*************************************************************/
char history_record(UWORD index,UBYTE *outbuf)
{
	UBYTE ret,outbytes;
	UBYTE flag;
	UBYTE buf[20];
	if(index <= 8)
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
		ret = get_record(0x06,flag,0x17,&outbuf[1]);//��ֵ��ϸ�ļ�0008-->0006
		if(ret)
		{
			return OP_ERR;
		}
		outbuf[0] = flag-1;//flag
		return 0;
		switch(index)
		{
			case 1:	
				flag = 1;
				break;
			case 2:	
				flag = 2;
				break;
			case 4:	
				flag = 3;
				break;
			case 8:	
				flag = 4;
				break;
			default:
				gResponseStatus=CE_BADPARAM;
				gGetCardInfoStep=0xff;
				return OP_ERR;
		}
		ret = get_record(0x06,flag,0x17,&outbuf[1]);//��ֵ��ϸ�ļ�0008-->0006
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
		//2018/3/29 11:42
		//if(gCardStatus!=M1LOCKED)
		{
			ret = get_record(0x07,flag,0x10,&outbuf[1]);
		}
		//else
		//{
		//	ret = get_record(0x18,flag,0x17,&outbuf[1]);
		//}
		if(ret)
		{
			return OP_ERR;
		}
		if (lrc_chk2(&outbuf[1],15) != 0)
		{
			gResponseStatus = (flag) | 0x80;
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
		//����ȫΪ0 ֱ�ӷ���
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
		//����TAC
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
			gResponseStatus=CE_INVALIDCARD;
			return OP_ERR;
		}
		outbuf[0] = 14;//15
		//����ȫΪ0 ֱ�ӷ���
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
		//����TAC
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
			gResponseStatus=CE_INVALIDCARD;
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
������trans_decrement_cpu
���ܣ�CPU���ۿ��
*************************************************************/
char trans_decrement_cpu(void)
{
	int ret;
	UBYTE buf[60],outbuf[300];
	UBYTE n,outn;
	UBYTE mac1[4],mac2[4];
//	UDWORD temp2,tint;
	
	//Ϊ�˼ӿ��ٶȣ���ȡ��֤����ڶ�����
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
	
	memcpy(buf,"\x80\x50\x01\x02\x0B",5);				//��ͨ����
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
	buf_to_udword(buf,ret);					//͸֧��
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
	buf[15] = 0x06;					//09:�������ѣ�06������Ǯ������
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
	//��д������¼��
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
//	//���¿��ϵ���ר����������	
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
	//ȡMAC1
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
������trans_update_cpu
���ܣ�CPU������
*************************************************************/
char trans_update_cpu(UBYTE update_type,UBYTE *rec_buf)
{
	int ret;
	UBYTE buf[60],outbuf[300];
	UBYTE n,outn;
	UBYTE mac1[4],mac2[4];
//	UDWORD temp2,tint;
	
	//Ϊ�˼ӿ��ٶȣ���ȡ��֤����ڶ�����2018/5/4 10:44
//	memcpy(buf,"\x80\xca\x00\x00\x09",5);
//	ret = mifpro_icmd0(5,buf,outbuf);
//	if(ret<0)
//	{
//		goto label_readerr;
//	}
//	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
//	{
//		goto label_invalid;
//	}
//	
//	memcpy(bgTaskKeyInbuf,outbuf,9);
//	set_task_key(TASK_KEY_SEND_AUTHCODE);
	memcpy(buf,"\x00\xb0\x95\x00\x1e",5);
	ret = mifpro_icmd0(5,buf,outbuf);
	if(ret<0)
	{
		goto label_readerr;
	}
	gDebugStep = 0x83;
	
	memcpy(buf,"\x80\x50\x03\x02\x0B",5);				//��ͨ����
	buf[5] = gKeyVersion;
	udword_to_buf(gCardThisTransValue,&buf[6]);
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
	buf_to_udword(buf,ret);					//͸֧��
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
	buf[15] = 0x09;					//09:�������ѣ�06������Ǯ������
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
	
	if((memcmp(gCityCode,"\x20\x00",2) == 0) || (memcmp(gAppSn,"\x20\x00",2) == 0)) 
	{
		 //�Ϻ��� ����007�ļ�/0011�ļ�
		trans_gen_card_record(0x14, buf);
		//memcpy(buf,gCardRecord,16);
		if((!gM1ConsumptionStatus)&&(memcmp(gAppSn,"\x20\x00",2) == 0)) 
		{
			//00ΪM1����δ�ر�,�Ϻ���
			//update 0007
			memcpy(gBuf,"\x80\xdc\x00\x3b\x10",5);
			memcpy(&gBuf[5],buf,16);
			gBufBytes = 21;
		
			ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
			if(ret)  
			{
		  	   gDebugStep = 0x29c2;
		  	   gResponseStatus = CE_WRITEERROR;  
		  	   return (char)-1;
			}
			#ifdef _EMU_
			printf("\niso_apdu write 0007:");
			for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
			#endif	
		}

	if((gM1ConsumptionStatus)&&(memcmp(gAppSn,"\x20\x00",2) == 0)&&(cardversion!=0x02)&&((uchar)cardversion<=0x9)) 
	{
		//01ΪM1�����ѹر�,�Ϻ��������汾��02����00-09��
		//update 0007
		memcpy(gBuf,"\x80\xdc\x00\x3b\x10",5);
		memcpy(&gBuf[5],buf,16);
		gBufBytes = 21;
	
		ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
		if(ret)  
		{
	  	   gDebugStep = 0x2ac2;
	  	   gResponseStatus = CE_WRITEERROR;  
	  	   return (char)-1;
		}
		#ifdef _EMU_
		printf("\niso_apdu write 0007:");
		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
		#endif	
		}
	
		//���µ������
		if(update_type == ENTRY_UPDATE)
		{
		    gDebugStep = 0x30a5;
		    #ifdef _EMU_
		    printf("\ngUpdateRecord Entry:");
		    for(i=0;i<16;i++)  printf(" %02x",rec_buf[i]);
		    #endif
			ret = write_record(0x11,0x01,16,rec_buf, 0);
			if(ret)
		    {
			     gDebugStep = 0x30a6;
			     return (char)-1;
		    }
			memset(buf,0,sizeof(buf));
			buf[0] = 0x55;
			buf[15] = 0x55;
			ret = write_record(0x11,0x03,16,buf,0);
			if(ret)
		    {
			    gDebugStep = 0x30a7;
			    return (char)-1;
		    }
		}
		else
		{
		    gDebugStep = 0x30b5;
		    #ifdef _EMU_
		    printf("\ngUpdateRecord Exit:");
		    for(i=0;i<16;i++)  printf(" %02x",rec_buf[i]);
		    #endif
			ret = write_record(0x11,0x02,16,rec_buf, 0);
			if(ret)
			{
			   gDebugStep = 0x30b6;
			   return (char)-1;
			}
			memset(buf,0,sizeof(buf));
			buf[0] = 0x66;
			buf[15] = 0x66;
			ret = write_record(0x11,0x03,16,buf, 0);
			if(ret)
			{
			  gDebugStep = 0x30b7;
			  return (char)-1;
			}
		}
	}
	else if((memcmp(gCityCode,CityCode_KunShan,2) == 0)||(memcmp(gCityCode,CityCode_ChangShu,2) == 0)||(memcmp(gCityCode,CityCode_WuXi,2) == 0)) 
	{ 
		//��ɽ/����/������,ͬʱ����0011��0017�ļ�,20131128
		//���µ�����ǣ�0011�ļ�
		if(update_type == ENTRY_UPDATE)
		{
			gDebugStep = 0x30a5;
			#ifdef _EMU_
					printf("\n��ɽ/����/������->gUpdateRecord Entry:");
					for(i=0;i<16;i++)  printf(" %02x",rec_buf[i]);
			#endif
			ret = write_record(0x11,0x01,16,rec_buf,0);
			if(ret)
			{
				gDebugStep = 0x30a6;
				return (char)-1;
			}
			memset(buf,0,sizeof(buf));
			buf[0] = 0x55;
			buf[15] = 0x55;
			ret = write_record(0x11,0x03,16,buf,0);
			if(ret)
			{
				gDebugStep = 0x30a7;
				return (char)-1;
			}
		}
		else
		{
			gDebugStep = 0x30b5;
			#ifdef _EMU_
					printf("\n��ɽ/����/������->gUpdateRecord Exit:");
					for(i=0;i<16;i++)  printf(" %02x",rec_buf[i]);
			#endif
			ret = write_record(0x11,0x02,16,rec_buf,0);
			if(ret)
			{
				gDebugStep = 0x30b6;
				return (char)-1;
			}
			memset(buf,0,sizeof(buf));
			buf[0] = 0x66;
			buf[15] = 0x66;
			ret = write_record(0x11,0x03,16,buf,0);
			if(ret)
			{
				gDebugStep = 0x30b7;
				return (char)-1;
			}
		}

	    //����0017�ļ�
		memset(buf,0,sizeof(buf));
		memcpy(buf,"\x09\x2e\x00",3); //1-1	�������ѱ�־	1	BCD��09
		//2-2	��¼����	1	HEX��2E
		//3-3	Ӧ��������־λ	1	HEX��00
		buf[3] = 0; //4-4	�������Ѽ�¼�汾��	1	HEX
		if(update_type == ENTRY_UPDATE) buf[4] = 1; //5-5	�������ͣ�0x00��ʶ����ɣ�	1	HEX
		else buf[4] = 0;
		memcpy(&buf[5],gPosID,6); //6-11	�ն˻���ţ����д��룩	6	BCD
		//buf[11] = *((char *)&gCardThisRemain+0); //12-15	�����ڲ��ۣ�	4	HEX
		//buf[12] = *((char *)&gCardThisRemain+1); 
		//buf[13] = *((char *)&gCardThisRemain+2); 
		//buf[14] = *((char *)&gCardThisRemain+3); 
		memset(&buf[11],0,4);
		memcpy(&buf[15],gTransTime,7); //16-19	��������	4	YYYYMMDD
		//20-22	����ʱ��	3	HHMMSS
		memset(&buf[22],0,4); //23-26	ͨ�ø������Ѽ�¼MAC	4	HEX,��ʱ��0
		if(update_type == ENTRY_UPDATE)  buf[26] = 0x55;
		else buf[26] = 0x66;
		memcpy(&buf[27],rec_buf,16);         
		
		ret = write_capp_record(0x17,0x09,48,buf,0);
		if(ret)
		{
			gDebugStep = 0x39b7;
			return (char)-1;
		}
	}
	else  //���Ϻ���Ҳ����ɽ/����/������������0017�ļ�
	{
		//update 0017 ȫ�����������ļ�
	  
		//cancelled in 2011-7-22 15:19:31
		//  memset(buf,0,sizeof(buf));
		//  memcpy(buf,gRec0017,48);
		//  memcpy(buf,"\x09\x2e\x00",3); //1-1	�������ѱ�־	1	BCD��09
		//                                //2-2	��¼����	1	HEX��2E
		//                                //3-3	Ӧ��������־λ	1	HEX��00
		//  buf[3] = 0; //4-4	�������Ѽ�¼�汾��	1	HEX
		//  if(gUpdateType == ENTRY_UPDATE)  buf[4] = 1; //5-5	�������ͣ�0x00��ʶ����ɣ�	1	HEX
		//     else buf[4] = 0;
		//  memcpy(&buf[5],psam_no,6); //6-11	�ն˻���ţ����д��룩	6	BCD
		//  /*
		//  buf[11] = *((char *)&gCardThisRemain+0); //12-15	�����ڲ��ۣ�	4	HEX
		//  buf[12] = *((char *)&gCardThisRemain+1); 
		//  buf[13] = *((char *)&gCardThisRemain+2); 
		//  buf[14] = *((char *)&gCardThisRemain+3); 
		//  */
		//  if(gUpdateType == ENTRY_UPDATE)  memcpy(&buf[11],"\x00\x00\x02\x58",4);//6Ԫ
		//	 else memcpy(&buf[11],"\x00\x00\x00\x00",4);//0
		//
		//  memcpy(&buf[15],gTransTime,7); //16-19	��������	4	YYYYMMDD
		//                                 //20-22	����ʱ��	3	HHMMSS
		//  memset(&buf[22],0,4); //23-26	ͨ�ø������Ѽ�¼MAC	4	HEX,��ʱ��0
		//  buf[26] = gUpdateRecord[0];
		//  
		//  //���׼�¼
		//  //memcpy(&buf[27],gUpdateRecord,16);
		//  memcpy(&buf[35],&gUpdateRecord[9],2);//���׽��
		//  memcpy(&buf[37],&gUpdateRecord[12],3);//�ۼƽ��
		//  if(gUpdateType == ENTRY_UPDATE)  memcpy(&buf[27],&gUpdateRecord[1],8);//��վʱ�䣬��վ�豸����
		//	 else memcpy(&buf[40],&gUpdateRecord[1],8);//��վʱ�䣬��վ�豸����
		
		memset(buf,0,sizeof(buf));
		memcpy(buf,"\x09\x2e\x00",3); //1-1	�������ѱ�־	1	BCD��09
	                                //2-2	��¼����	1	HEX��2E
	                                //3-3	Ӧ��������־λ	1	HEX��00
		 buf[3] = 0; //4-4	�������Ѽ�¼�汾��	1	HEX
		if(update_type == ENTRY_UPDATE) 
			buf[4] = 1; //5-5	�������ͣ�0x00��ʶ����ɣ�	1	HEX
		else 
			buf[4] = 0;
		memcpy(&buf[5],gPosID,6); //6-11	�ն˻���ţ����д��룩	6	BCD
		//buf[11] = *((char *)&gCardThisRemain+0); //12-15	�����ڲ��ۣ�	4	HEX
		//buf[12] = *((char *)&gCardThisRemain+1); 
		//buf[13] = *((char *)&gCardThisRemain+2); 
		//buf[14] = *((char *)&gCardThisRemain+3); 
		memset(&buf[11],0,4);
		memcpy(&buf[15],gTransTime,7); //16-19	��������	4	YYYYMMDD
	                                 //20-22	����ʱ��	3	HHMMSS
		memset(&buf[22],0,4); //23-26	ͨ�ø������Ѽ�¼MAC	4	HEX,��ʱ��0
		if(update_type == ENTRY_UPDATE)  
			buf[26] = 0x55;
		else 
			buf[26] = 0x66;
		memcpy(&buf[27],rec_buf,16);         
	         
		ret = write_capp_record(0x17,0x09,48,buf,0);
		if(ret)
		{
			gDebugStep = 0x39b7;
			return (char)-1;
		}	
	}
	//ȡMAC1
	gDebugStep = 0x328d;
	if(SHSendAuthResp())
	{
		goto label_samerr;
	}
	buf_to_udword(bgTaskKeyOutbuf,gPosTransCnt);
	memcpy(mac1,&bgTaskKeyOutbuf[4],4);
  //�û����������,debit for purchase
	memcpy(gBuf,"\x80\x54\x01\x00\x0F",5);
	//gBuf[5] = *((char *)&gPosTransCnt+0);//�ն˽�����ˮ��	4
	//gBuf[6] = *((char *)&gPosTransCnt+1);
	//gBuf[7] = *((char *)&gPosTransCnt+2);
	//gBuf[8] = *((char *)&gPosTransCnt+3);
	udword_to_buf(gPosTransCnt, &gBuf[5]);
	memcpy(&gBuf[9],gTransTime,7);
	memcpy(&gBuf[16],mac1,4);//mac1
	gBufBytes = 20;
	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
	if(ret)  
	{
		gDebugStep = 0x30a2;
		gResponseStatus=CE_WRITEERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
#ifdef _EMU_
printf("\niso_apdu trans_cpu_update-->�û����������:");
for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
#endif
	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
  	   gDebugStep = 0x30a3;
  	   gResponseStatus=CE_MAC2ERR;
  	   gGetCardInfoStep=0xff;
		return OP_ERR;
	}	
	memcpy(gTAC,gBuf,4);//tac
	memcpy(gMAC,&gBuf[4],4);//mac2
	
	//SAM���������,credit sam for purchase
	memcpy(gBuf,"\x80\x72\x00\x00\x04",5);
	memcpy(&gBuf[5],gMAC,4);
	gBufBytes = 9;
	ret = sam_apdu(bgSHpsamIndex,gBuf,gBufBytes,gBuf,&gBufBytes, 0, 0);
  		#ifdef _EMU_
  		printf("\ncpu ->sam_apdu SAM���������:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  		#endif
	if(((uchar)gBuf[0] != (uchar)0x90) || (gBuf[1] != 0x0))
	{
		gDebugStep = 0x30a4;
		goto label_samerr;
	}
  			  
	//�ۿ������
	gCardThisRemain = gCardLastRemain - gCardThisTransValue;
	
	//������ݱ��
	gDebugStep = 0x30c0;

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
������get_record
���ܣ���ȡ�ļ��еļ�¼
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
	
	printf("\nzdd-get_recordret\n");
	printf("\nget_record...ret=%d\n",ret);
	printf("rec_buf=");
	for(i=0;i<ret;i++)
	{
		printf(" %02x",rec_buf[i]);
	}
	printf("\n");
	
	if(ret<0)
	{
		goto label_readerr;
	}
				printf("\nzdd-getrecord 06 - 1\n");
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
			printf("\nzdd-getrecord 06error\n");
			gResponseStatus=CE_NOLOG;
			gGetCardInfoStep=0xff;  
			return OP_ERR;
		}
		printf("\nzdd-getrecord 06error -1\n");
		goto label_invalid;
	}
	printf("\nzdd-getrecord 06 - 2\n");
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
������write_record
���ܣ����ļ���д���¼
*************************************************************/
//#define _WR_REC_
char write_record(UBYTE SFI_index,UBYTE rec_index,UBYTE len,UBYTE *rec_buf,UBYTE cycleflag)
{
	int ret;
	UBYTE buf[60],outbuf[300];
	memcpy(buf,"\x80\xdc",2);
	buf[2] = rec_index;
	buf[3] = (SFI_index<<3)|0x04;
	if(cycleflag)
	{
		buf[2] = 0;
		buf[3] = (SFI_index<<3)|0x03;
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
	UBYTE buf[500],outbuf[300];
	
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
int ret,ret2, rtn;
UBYTE buf[300];
UBYTE outbuf[300], tempbuf[1000];
UBYTE pfbuf[256];
UBYTE cnt;
int i;
UBYTE data[256];
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

	//ret = select_1001or1002_app(0);//�ɲ��� ��ǰĿ¼��¼����
	//if(ret ==0 )bpgSmpsamAppSjtFlag = 0; 
	//select ppse	
	gDebugStep = 0x3083;
	memcpy(buf,"\x00\xa4\x04\x00\x0e",5);
	memcpy(&buf[5],"2PAY.SYS.DDF01",14);
#ifdef _debug_pf_mobile_ticket_process_
	debug_printf("pf_mobile_ticket_process 1\n");
	for(i=0;i<19;i++) debug_printf(" %02x",buf[i]);
#endif

	ret = mifpro_icmd0(19,buf,outbuf);
#ifdef _debug_pf_mobile_ticket_process_
	debug_printf("\npf_mobile_ticket_process 2[%d]\n",ret);
	for(i=0;i<ret;i++) debug_printf(" %02x",outbuf[i]);
#endif

	if(ret < 0)
	{
		goto label_readerr;
	}	

////if((outbuf[ret-2]==0x6a)&&((UBYTE)outbuf[ret-1]==(UBYTE)0x82)) 
//	if((outbuf[ret-2]==0x6a)&&((UBYTE)outbuf[ret-1] & (UBYTE)0x80)) 
//	{
//		return CE_NO_PPSE;	
//	}
	
	
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00)) {
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
	printf("\npf_mobile_ticket_get_info ret =%d info_buf= \n",ret);
	for(i = 0;i < 256;i++)
		printf(" %02X ",buf[i]);
#endif
#ifdef _GETCARDINFO_CPU_
	printf("\npf_mobile_ticket_get_info ret =%d \n",ret);
#endif

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
		else if(ret == -400)
		{
			rtn=sh_transport_get_info(data);
			if (rtn == CE_INVALIDCARD)
			{
				outbuf[0] = 0x12;
				outbuf[1] = gReadCmdTimes;
				outbuf[2] = CE_INVALIDCARD;
				shmt_prot_send(outbuf,3);
				return 0;
			}
			else if(rtn!=0)
			{
				return OP_ERR;
			}
			gCardFamily = FAMILY_CPU_CARD;
			if(memcmp(transport_15_data,"\x02\x00\x29\x00",4)==0)
			{
				if(ReadMode==0x90)
				{
					//���1			
					outbuf[0] = 0x12;
					outbuf[1] = gReadCmdTimes;
					outbuf[2] = CE_OK;
					outbuf[3] = 0x01;
					memcpy(&outbuf[4],gThisCardSnr,4);//UID
					memcpy(&outbuf[8],transport_15_data,4);//��������
					memcpy(&outbuf[12],&transport_15_data[10],10);//Ӧ�����к�
					outbuf[22]=transport_17_data[10];//������
					memcpy(&outbuf[23],&transport_15_data[24],4);//��Ч��
					outbuf[27] = *((char *)&gCardThisRemain+LONG_HIGH0);//�����
					outbuf[28] = *((char *)&gCardThisRemain+LONG_HIGH1);
					outbuf[29] = *((char *)&gCardThisRemain+LONG_HIGH2);
					//outbuf[30]=gCardStatus;
					outbuf[30] = transport_15_data[9];//��״̬
					memcpy(&outbuf[31],&capp_init[4],2);//��������(�ѻ��������)
					outbuf[33] = transport_15_data[8];//���汾��
					//��һ����¼�豸�ţ�����0
					if(gBFirst == 32)
					{
						memcpy(&outbuf[34],&gB33[6],3);	
					}
					else
					{
						memcpy(&outbuf[34],&gB32[6],3);	
					}
					//������Ϣ
					memcpy(&outbuf[37],info,5);
					
					cnt=42;
					shmt_prot_send(outbuf,cnt);
					
					//���2
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
				     			outbuf[4]=0x41;
				     			outbuf[19]=0x41;
					   			recordtime[0]=0;
					   			recordtime[1]=0;
					   			flag = 1;
				  			}			
			    			break;
						case 33:     
			    			if(check_card_get_b33()) goto label_err_remove;
			    			memcpy(&outbuf[4],gB33,16);	
	        			ret=outbuf[4]&0xc0;
			    			if((lrc_chk2(&outbuf[4],15)!=0)||(ret != 0x80))
			    			{
				    			memset(&outbuf[4],0,16);
				    	 		outbuf[4]=0x81;
				     			outbuf[19]=0x81;
					   			recordtime[0]=0;
					   			recordtime[1]=0;
				     			flag = 1;
				  			}			
			    			break;
						default:   
				  			ret=get_metro_rec(&outbuf[4]);
				  			if(ret!=0)
				  			{
					   			if(ret==0x55) goto label_err_remove;
					   			memset(&outbuf[4],0,16);
					   			outbuf[4]=0x81;	//��վ��81��վ��41 
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
				{//91:BOMģʽ
					//���1			
					outbuf[0] = 0x12;
					outbuf[1] = gReadCmdTimes;
					outbuf[2] = CE_OK;
					outbuf[3] = 0x01;
					//UID
					memcpy(&outbuf[4],gThisCardSnr,4);
					//��������
					memcpy(&outbuf[8],transport_15_data,4);
					//Ӧ�����к�
					memcpy(&outbuf[12],&transport_15_data[10],10);
					//������
					outbuf[22]=transport_17_data[10];
					//Ѻ��
					outbuf[23]=transport_15_data[29];
					//��Ч��
					memcpy(&outbuf[24],&transport_15_data[24],4);
					//Ӧ����������
					memcpy(&outbuf[28],&transport_15_data[20],4);
					//�����
					outbuf[32] =*((char *)&gCardThisRemain+LONG_HIGH0);
					outbuf[33] =*((char *)&gCardThisRemain+LONG_HIGH1);
					outbuf[34] =*((char *)&gCardThisRemain+LONG_HIGH2);
					//͸֧�޶�
					outbuf[35] = capp_init[8];
					outbuf[36] = capp_init[7];
					outbuf[37] = capp_init[6];
					//��״̬
					outbuf[38] = transport_15_data[9];
					//��������(�ѻ��������)
					memcpy(&outbuf[39],&capp_init[4],2);
					//���汾��
					outbuf[41] = transport_15_data[8];
					//������Ϣ��
					memcpy(&outbuf[42],info,5);
					//�ۼƳ˳����
					memcpy(&outbuf[47],&leisure[2],3);
					//�ۼƳ˳�����
					memcpy(&outbuf[50],leisure,2);
					//����״̬
					if(gBFirst == 32){
						outbuf[52] = gMetroIoFlag;		
					}
			    else{
						outbuf[52] = 0x66;
					}
					
					UBYTE temp32[20],temp33[20];
					memcpy(&temp33[0],gB33,16);	
					memcpy(&temp32[0],gB32,16);
					if(memcmp(&temp33[0],"\x81\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x81",16) == 0) {
						outbuf[52] = 0x55;
						if(memcmp(&temp32[0],"\x41\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x41",16) == 0) 
							outbuf[52] = 0x66;
					}
				//SEND2:
					cnt=53;
					shmt_prot_send(outbuf,cnt);
					
					//���2
					outbuf[0] = 0x12;
					outbuf[1] = gReadCmdTimes;
					outbuf[2] = CE_OK;
					outbuf[3] = 0x02;
					memcpy(&outbuf[4],gB32,16);
					cnt=20;
					shmt_prot_send(outbuf,cnt);
					
					//���3
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
			{//��ؿ�
				if(ReadMode==0x90)
				{
					//���1			
					outbuf[0] = 0x13;
					outbuf[1] = gReadCmdTimes;
					outbuf[2] = CE_OK;
					outbuf[3] = 0x01;
					memcpy(&outbuf[4],gThisCardSnr,4);//UID
					memcpy(&outbuf[8],transport_15_data,4);//��������
					memcpy(&outbuf[12],&transport_15_data[10],10);//Ӧ�����к�
					outbuf[22]=transport_17_data[10];//������
					memcpy(&outbuf[23],&transport_15_data[24],4);//��Ч��
					outbuf[27] = transport_balance[3];//�����
					outbuf[28] = transport_balance[2];
					outbuf[29] = transport_balance[1];
					//outbuf[30]=gCardStatus;
					outbuf[30] = transport_15_data[9];//��״̬
					memcpy(&outbuf[31],&capp_init[4],2);//��������(�ѻ��������)
					outbuf[33] = transport_15_data[8];//���汾��
					//memset(&outbuf[34],0x00,3);
					//��һ����¼�豸��
					if(transport_1A_data[122]==0x55) 
					{
						//�����豸��ţ�3�ֽ�
						outbuf[34]	=	transport_1A_data[34];
						outbuf[35]	=	transport_1A_data[50];
						outbuf[36]	=	transport_1A_data[66];
					}
					else 
					{
						//�����豸��ţ�3�ֽ�
						outbuf[34]	=	transport_1A_data[26];
						outbuf[35]	=	transport_1A_data[42];
						outbuf[36]	=	transport_1A_data[58];
					}
					memset(&outbuf[37],0x00,5);//������Ϣ����ؿ���0
					cnt=42;
					shmt_prot_send(outbuf,cnt);
				
					//���2������ר�ý��׼�¼��16�ֽ�
					outbuf[0] = 0x13;
					outbuf[1] = gReadCmdTimes;
					outbuf[2] = CE_OK;
					outbuf[3] = 0x82;
					if(transport_1A_data[120] ==0x55)
					{
						//������־
						outbuf[4]	= transport_1A_data[120];
						//ʹ��ʱ���ꡢ�¡��ա�ʱ���֣���ֻ�е��ֽڣ���5�ֽڣ�
						outbuf[5]	=	transport_1A_data[68];
						outbuf[6]	=	transport_1A_data[69];
						outbuf[7]	=	transport_1A_data[70];
						outbuf[8]	=	transport_1A_data[71];
						outbuf[9]	=	transport_1A_data[72];
					
						//�����豸��ţ�3�ֽ�
						outbuf[10]	=	transport_1A_data[26];
						outbuf[11]	=	transport_1A_data[42];
						outbuf[12]	=	transport_1A_data[58];
					}
					else
					{
						//������־
						outbuf[4]	= transport_1A_data[121];
						//ʹ��ʱ���ꡢ�¡��ա�ʱ���֣���ֻ�е��ֽڣ���5�ֽڣ�
						outbuf[5]	=	transport_1A_data[75];
						outbuf[6]	=	transport_1A_data[76];
						outbuf[7]	=	transport_1A_data[77];
						outbuf[8]	=	transport_1A_data[78];
						outbuf[9]	=	transport_1A_data[79];
					
						//�����豸��ţ�3�ֽ�
						outbuf[10]	=	transport_1A_data[34];
						outbuf[11]	=	transport_1A_data[50];
						outbuf[12]	=	transport_1A_data[66];	
					}
					//���׽�2�ֽ�
					outbuf[13] = *((char *)&gCardThisTransValue+LONG_HIGH0);
					outbuf[14] = *((char *)&gCardThisTransValue+LONG_HIGH1);
					//�ۼƴ�����1�ֽ�
					outbuf[15] =	transport_1A_data[114];
					//�ۼƽ�3�ֽ�16���ƣ����ֽ���ǰ
					outbuf[16]	=	transport_1A_data[112];
					outbuf[17]	=	transport_1A_data[111];
					outbuf[18]	=	transport_1A_data[110];
					//У��
					lrc_gen2(&outbuf[4],15);
					cnt=20;
					shmt_prot_send(outbuf,cnt);
					gStatus=TRANS_STATUS;
					return 0;	
				}
				else
				{//91:BOMģʽ
					//���1			
					outbuf[0] = 0x13;
					outbuf[1] = gReadCmdTimes;
					outbuf[2] = CE_OK;
					outbuf[3] = 0x01;
					//UID
					memcpy(&outbuf[4],gThisCardSnr,4);
					//��������
					memcpy(&outbuf[8],transport_15_data,4);
					//Ӧ�����к�
					memcpy(&outbuf[12],&transport_15_data[10],10);
					//������
					outbuf[22]=transport_17_data[10];
					//Ѻ��
					outbuf[23]=transport_15_data[29];
					//��Ч��
					memcpy(&outbuf[24],&transport_15_data[24],4);
					//Ӧ����������
					memcpy(&outbuf[28],&transport_15_data[20],4);
					//�����
					outbuf[32] = transport_balance[3];
					outbuf[33] = transport_balance[2];
					outbuf[34] = transport_balance[1];
					//͸֧�޶�
					outbuf[35] = capp_init[8];
					outbuf[36] = capp_init[7];
					outbuf[37] = capp_init[6];
					//��״̬
					outbuf[38] = transport_15_data[9];
					//��������(�ѻ��������)
					memcpy(&outbuf[39],&capp_init[4],2);
					//���汾��
					outbuf[41] = transport_15_data[8];
					//������Ϣ
					//memset(&outbuf[42],0x00,5);
					memcpy(&outbuf[42], &transport_1A_data[115], 5);
					//�ۼƳ˳����
					outbuf[47]=transport_1A_data[112];
					outbuf[48]=transport_1A_data[111];
					outbuf[49]=transport_1A_data[110];
					//�ۼƳ˳�����
					memcpy(&outbuf[50],&transport_1A_data[113],2);
					//����״̬					
					outbuf[52]	=	transport_1A_data[122];				
					cnt=53;
					shmt_prot_send(outbuf,cnt);
					
					//���2������ר�ý��׼�¼��16�ֽ�
					outbuf[0] = 0x13;
					outbuf[1] = gReadCmdTimes;
					outbuf[2] = CE_OK;
					outbuf[3] = 0x02;
					//������־
					outbuf[4]	= transport_1A_data[120];
					//ʹ��ʱ���ꡢ�¡��ա�ʱ���֣���ֻ�е��ֽڣ���5�ֽڣ�
					outbuf[5]	=	transport_1A_data[68];
					outbuf[6]	=	transport_1A_data[69];
					outbuf[7]	=	transport_1A_data[70];
					outbuf[8]	=	transport_1A_data[71];
					outbuf[9]	=	transport_1A_data[72];
					//�����豸��ţ�3�ֽ�
					outbuf[10]	=	transport_1A_data[26];
					outbuf[11]	=	transport_1A_data[42];
					outbuf[12]	=	transport_1A_data[58];
					//���׽�2�ֽ�
					outbuf[13] = transport_1A_data[98];
					outbuf[14] = transport_1A_data[97];
					//�ۼƴ�����1�ֽ�
					outbuf[15] =	transport_1A_data[114];
					//�ۼƽ�3�ֽ�16���ƣ����ֽ���ǰ
					outbuf[16]	=	transport_1A_data[112];
					outbuf[17]	=	transport_1A_data[111];
					outbuf[18]	=	transport_1A_data[110];
					//У��
					lrc_gen2(&outbuf[4],15);
					cnt=20;
					shmt_prot_send(outbuf,cnt);
					
					//���3������ר�ý��׼�¼��16�ֽ�
					outbuf[0] = 0x13;
					outbuf[1] = gReadCmdTimes;
					outbuf[2] = CE_OK;
					outbuf[3] = 0x83;
					//������־
					outbuf[4]	= transport_1A_data[121];
					if( (transport_1A_data[122]== 0x55) && (transport_1A_data[120] & 0x04) )
					{
						//ʹ��ʱ���ꡢ�¡��ա�ʱ���֣���ֻ�е��ֽڣ���5�ֽڣ�
						outbuf[5]	=	transport_1A_data[100];
						outbuf[6]	=	transport_1A_data[101];
						outbuf[7]	=	transport_1A_data[102];
						outbuf[8]	=	transport_1A_data[103];
						outbuf[9]	=	transport_1A_data[104];
						//�����豸��ţ�3�ֽ�
						outbuf[10]	=	transport_1A_data[106];
						outbuf[11]	=	transport_1A_data[107];
						outbuf[12]	=	transport_1A_data[109];
					}else
					{
						//ʹ��ʱ���ꡢ�¡��ա�ʱ���֣���ֻ�е��ֽڣ���5�ֽڣ�
						outbuf[5]	=	transport_1A_data[75];
						outbuf[6]	=	transport_1A_data[76];
						outbuf[7]	=	transport_1A_data[77];
						outbuf[8]	=	transport_1A_data[78];
						outbuf[9]	=	transport_1A_data[79];
						//�����豸��ţ�3�ֽ�
						outbuf[10]	=	transport_1A_data[34];
						outbuf[11]	=	transport_1A_data[50];
						outbuf[12]	=	transport_1A_data[66];
					}
					//���׽�2�ֽ�
					outbuf[13] = transport_1A_data[98];
					outbuf[14] = transport_1A_data[97];
					//�ۼƴ�����1�ֽ�
					outbuf[15] =	transport_1A_data[114];
					//�ۼƽ�3�ֽ�16���ƣ����ֽ���ǰ
					outbuf[16]	=	transport_1A_data[112];
					outbuf[17]	=	transport_1A_data[111];
					outbuf[18]	=	transport_1A_data[110];
					//У��
					lrc_gen2(&outbuf[4],15);
					cnt=20;
					shmt_prot_send(outbuf,cnt);
					
					gStatus=TRANS_STATUS;
					
					return 0;
				}
			}
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
		for(i=0;i<128;i++) debug_printf(" %02x",buf[i]);
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

	if(buf[1] == 0xA1) 
	{//�Ϸ�ODA��
		outbuf[0] = ODA_READOK;
		outbuf[1] = gReadCmdTimes;
		outbuf[2] = CE_OK;
		outbuf[3] = 0x01;
		memcpy(&outbuf[4],&buf[2],37);
		memcpy(gODARec1, &buf[2], 37);
		cnt = 41;
		shmt_prot_send(outbuf,cnt);
		
		ret = oda_ticket_get_zone(buf);
#ifdef _debug_pf_mobile_ticket_process_
		printf("\noda_ticket_get_zone = %d", ret);
		for(i = 0;i < 256;i++)
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
		memcpy(&outbuf[4],&buf[2],48);
		memcpy(gODARec2, &buf[2], 48);
		cnt = 52;
		shmt_prot_send(outbuf,cnt);
	
		gStatus=TRANS_STATUS;
		gCardFamily = FAMILY_CPU_PF;
	  	return CE_OK;
	}
	else if(buf[1] == 0xA2)
	{//ODA��Ч��
		if(gMode == 0x60)
		{//BOM����δ��ͨ���˻���Ϣ 
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
			gCardFamily = FAMILY_CPU_PF;
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
	{//�ַ���ͨ��
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
		outbuf[4+34]=buf[27+34+2];outbuf[4+34+2]=buf[27+34]; //���¹����ͨ�ۼƽ����ֽ���ǰ��
		outbuf[4+39]=buf[27+39+1];outbuf[4+39+1]=buf[27+39]; //ʵ�ʽ��׽����ֽ���ǰ��
		cnt = 70;
		shmt_prot_send(outbuf,70);
		//tempbuf[cnt] = 70;
		//memcpy(&tempbuf[cnt+1], outbuf, 70);
		//cnt += sh_InsertDLE(&tempbuf[cnt], 70 + 1);
		//2013/11/12 19:57:48
		//writecom(csc_comm, tempbuf, cnt);
	
		gStatus=TRANS_STATUS;
		gCardFamily = FAMILY_CPU_PF;
	  	return CE_OK;
	}
	else 
	{//�ַ�Ա����
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
		outbuf[4+34]=buf[44+34+2];outbuf[4+34+2]=buf[44+34]; //���¹����ͨ�ۼƽ����ֽ���ǰ��
		outbuf[4+39]=buf[44+39+1];outbuf[4+39+1]=buf[44+39]; //ʵ�ʽ��׽����ֽ���ǰ��
		cnt = 49;
		shmt_prot_send(outbuf,cnt);
		gStatus=TRANS_STATUS;
		gCardFamily = FAMILY_CPU_EM;
  		return CE_OK;
	}	

label_err_remove:
	check_card_err_rpt(CE_CARDREMOVED);
	gStatus=IDLE_STATUS;
	//
	set_auto_abort_flag();  //20110216add
	gGetCardInfoStep = 0xff;
	return OP_ERR;	
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