//shtrans.c

#ifndef _SHTRANS_C_
#define _SHTRANS_C_
//#define _READ_BLOCK_

#ifdef _debug_test_
#define _DEBUG_SHTRANS_
#endif

code UBYTE gCardDir[16]={0x00,0x10,0x01,0x04,0x03,			/*����Ŀ¼��Ϣ*/
                         0x03,0x03,0x20,0x21,0x27,
                         0x22,0x23,0x31,0x32,0x33,0x34};         
code UBYTE gCardRecPos[10] = {14,16,17,18,20,21,22,24,25,26};	//��ͨ��������¼λ��
code UBYTE gCardRecPos2[28] = {14,16,17,18,20,21,22,24,25,26,28,29,30,36,37,38,
							   40,41,42,44,45,46,48,49,50,52,53,54};
code UBYTE trans_id128[3]={0x11,0x19,0x15};
code UBYTE trans_id[14]={0x11,0x11,0x12,0x1f,0x00,0x04,0x26,0x27,0x19,0x35,0x34,0x06,0x07,0x14};	//20070813Э��

UBYTE gCPUflag;//0x55:CPU   0x00:M1

UBYTE KeyBuf[60],gB29[20];
UBYTE gRestoreFlag,gClrFlag;					 //	gRestoreFlag:0����Ҫ�ָ����ݣ�0x55��Ҫ�ָ�.
													 //gClrFlag:0����Ҫ�������, 0x55��Ҫ�������.
UBYTE save[11];					//���������ж�����	
UBYTE gB32[20],gB33[20], gB34[20];		//����ר�ü�¼����	
UBYTE gB32BK[20],gB33BK[20];
UBYTE gBFirst;					//������¼�ṩ����	
UBYTE ChangeDate;				//0x55:��Ҫ������������
UBYTE recordsum[3];				//������¼���ۻ����
UBYTE recordcnt;				//������¼���ۻ�����
UBYTE recordtime[2];			//������¼���ۻ����ж�ʱ��


UBYTE hh_trans_id;			//����Ʊ���ײ���
UBYTE b9error;					//B9����
//
UBYTE gM1Size0x53Flag;
//2019-06-19
UBYTE sh_ticket_family;

/**************************************
������getcardinfo
���ܣ��Ϻ���ͨ������
**************************************/
#define _DEBUG_GETCARDINFO_
#ifdef _DEBUG_D_
#endif

//#define _GETCARDINFO_
UBYTE getcardinfo(void)
{
	UBYTE i,ret;
	UBYTE buf[50],buf1[50];
	UBYTE temp;
	
#ifdef DEBUG_PRINT
	debug_printf("\x0d\x0a M1 info!");
#endif
	sh_ticket_family=0;
	gCPUflag = 0;
	gRestoreFlag = 0;
	gClrFlag = 0x55;
	gMetroIoFlag = 0;
	ChangeDate = 0;
	gNotShCard = 0x00;
	gM1Size0x53Flag = 0;
#ifdef _GETCARDINFO_
	printf("\nsh_ticket_family = %d\n",sh_ticket_family);
#endif
//#ifdef _GETCARDINFO_
//  memcpy(buf,"\xd1\xd2\xd3",3);
//  shmt_prot_send(buf,3);
//#endif
//20130901
	//mcml_pwr_on();
  //
  for(i = 0; i < 3; i++)
  {
		ret=mcml_request(PICC_REQALL,buf);
		if(ret == 0)
			break;
	}
	if(ret!=0)
	{
		#ifdef DEBUG_PRINT
		debug_printf("\x0d\x0a M1 mcml_request Err!");
	#endif
		return OP_ERR;
	}
//
#ifdef DEBUG_TIME
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x01", 3);
#endif	
	ret=mcml_anticoll(gThisCardSnr);
	if(ret!=0)
	{
#ifdef DEBUG_PRINT
	debug_printf("\x0d\x0a M1 mcml_anticoll Err!");
#endif
		return OP_ERR;
	}
#ifdef DEBUG_TIME
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x02", 3);
#endif	
	gDebugStep=3;
	ret=mcml_select(gThisCardSnr,&i);
	if(ret!=0) //���Ӷ�0x53���ж�,��������2011-1-19 15:49:00
	{
		#ifdef DEBUG_PRINT
		debug_printf("\x0d\x0a M1 mcml_select Err!");
		#endif
		return OP_ERR;
	}
	if(i == 0x53)  gM1Size0x53Flag = 1;//2011-3-29 10:46:08
//2013/11/1 10:01:54
//	#ifdef DEBUG_PRINT
//		if(i != 0x53)
//		{
//			gCPUflag = 0x55;
//			#ifdef DEBUG_PRINT
//					debug_printf(" CPU Flag0x53= %02X \n",gCPUflag);
//			#endif
//			goto label_readerr;
//		}
//	#endif
	//set_card_type(ISO14443A_SH_TYPE);
		
	gDebugStep=4;
	memset(buf,0,16);
	memcpy(gKeyA,"\xfc\x61\xcb\xb5\x37\x51",6);   //��ʽ
	ret=mcmh_read_simple(1,buf,1,0);
	
	if(ret!=0)
	{
		//��֧�ֲ�����ԿM1��
//		memcpy(gKeyA,"\x70\xa4\xf4\x82\x01\x6d",6);   //����
//		ret=mcmh_read_simple(1,buf,0,0);
		if(ret!=0)
		{
			if((ReadMode == 0x90)||(ReadMode == 0x91))
			{
				gCPUflag = 0x00;
				#ifdef DEBUG_PRINT
					debug_printf(" CPU Flag1= %02X \n",gCPUflag);
				#endif
				return OP_ERR;
			}
			else
			{
				gCPUflag = 0x55;
				#ifdef DEBUG_PRINT
					debug_printf(" CPU Flag2= %02X \n",gCPUflag);
				#endif
				//mcml_pwr_off();
				//delay_ms(30);
				goto label_readerr;
			}
			#ifdef DEBUG_PRINT
				debug_printf(" CPU Flag3= %02X \n",gCPUflag);
			#endif
		}
	}	
	gDebugStep=5;
	if(lrc_chk2(buf,15) != 0)
	{
		goto label_invalid;
	}
	if(!gM1Size0x53Flag)//2011-3-29 10:45:58 M1���ּ����ܾ���53��ʶ��
	{
		goto label_invalid;
	}
	if(gPSAMFileFlag == 0x00)
	{
		goto skip_select;
	}
	bgSHpsamIndex = SAMLOCATION_5;
	memcpy(KeyBuf,"\x00\xa4\x00\x00\x02\x10\x03",7);
	i = 7;
	ret = sam_apdu(bgSHpsamIndex,KeyBuf,i,buf1,&temp,0,0);
	if(ret != 0)
	{ 
		#ifdef _DEBUG_GETCARDINFO_
		debug_printf("\x0d\x0a Err:getcardinfo,select 1003");
		#endif
		bgSHpsamIndex = SAMLOCATION_5;
		sam_select(bgSHpsamIndex);
		sam_set(bgSHpsamIndex,SAM_ETU_93,16);
		ret = sam_atr(bgSHpsamIndex,buf1,&temp);
		if(ret != 0)
		{
			goto label_samerr;
		}
		memcpy(KeyBuf,"\x00\xa4\x00\x00\x02\x10\x03",7);
		i = 7;
		ret = sam_apdu(bgSHpsamIndex,KeyBuf,i,buf1,&temp,0,0);
		if(ret != 0)
		{
			goto label_samerr;
		}
	}	
	//if((buf1[0] != 0x61) &&((UBYTE)buf1[0] != 0x90))  //2013/11/17 22:01:12
	if((buf1[temp-2] != 0x61) &&((UBYTE)buf1[temp-2] != 0x90))
	{
  	#ifdef _DEBUG_GETCARDINFO_
		debug_printf("\x0d\x0a Err:select 1003 resp");
		#endif
    goto label_samerr;
	}
	gPSAMFileFlag = 0x00;
skip_select:	

//	if(mcmh_read(1,buf,0,0)!=0)
//	{
//		goto label_readerr;
//	}
//	if(lrc_chk2(buf,15) != 0)
//	{
//		goto label_invalid;
//	}
	memcpy(KeyBuf,gThisCardSnr,4);
	memcpy(KeyBuf+4,buf+8,6);
	KeyBuf[10]=gCardDir[1];
	KeyBuf[11]=gCardDir[3];
	KeyBuf[12]=gCardDir[4];
	KeyBuf[13]=gCardDir[7];
	KeyBuf[14]=gCardDir[8];
	SHGetKeyAStep1(KeyBuf,15);
//	memcpy(bgTaskKeyInbuf,KeyBuf,15);
//	gbTaskKeyNum=5;
//	bgTaskKeyStep=TASK_KEY_GET_SHKEYA;
	gDebugStep=6;
	
	//printf("\n buf[0]= %02X\n",buf[0]);
	
	if(buf[0] < 0x10) 
	{
		goto label_invalid;
	}
	gDebugStep=7;
	
//	20090628 add
//	if(buf[6] != gCityCode0) goto label_invalid;
//	if(buf[7] != gCityCode1) goto label_invalid;
	gCardType = buf[14];
	gDebugStep=8;
	if(buf[0]==0x18)
	{
		NormalCard=CNTCARD;
	}
	else
	{
		NormalCard=COMMCARD;
	}
	if((NormalCard==CNTCARD)&&(ReadMode==0))
	{
		goto label_invalid;
	}
	gDebugStep=9;
	UseType=buf[1];
	if(UseType==0) 
	{
		StartUseDay[0]=buf[2];
		StartUseDay[1]=buf[3];
		StartUseDay[2]=buf[4];
		StartUseDay[3]=buf[5];
	}
	if(SHGetKeyAStep2()!=0)						//ȡ��Կ
	{		
		goto label_samerr;
	}
	gDebugStep=10;
	memcpy(gKeyA[1],bgTaskKeyOutbuf,6);
	memcpy(gKeyA[3],bgTaskKeyOutbuf+6,6);
	memcpy(gKeyA[4],bgTaskKeyOutbuf+12,6);
	memcpy(gKeyA[5],bgTaskKeyOutbuf+12,6);
	memcpy(gKeyA[6],bgTaskKeyOutbuf+12,6);
	memcpy(gKeyA[7],bgTaskKeyOutbuf+18,6);
	memcpy(gKeyA[8],bgTaskKeyOutbuf+24,6);
	
	KeyBuf[10]=gCardDir[2];						//����ʣ�µ�2��
	SHGetKeyAStep1(KeyBuf,11);

//	memcpy(bgTaskKeyInbuf,KeyBuf,11);
//	gbTaskKeyNum=1;
//	bgTaskKeyStep=TASK_KEY_GET_SHKEYA;


	if(NormalCard==CNTCARD)
	{
		goto label_countc_process;
	}
//		
	//read block12
//	temp=0;
//	
//	read_b12:
//	gDebugStep=11;
//	if(temp>2)
//	{
//		gDebugStep=16;
//		goto label_readerr;
//	}
	if(mcmh_read(12,buf,0,0)!=0)
	{
		gDebugStep=17;
		goto label_readerr;
	}
//	if(mcmh_read(12,buf1,2,0)!=0)
//	{
//		goto label_readerr;
//	}
//	if(memcmp(buf,buf1,16)!=0)
//	{
//		temp++;
//		goto read_b12;
//	}
	
	gDebugStep=12;
	if(lrc_chk2(buf,15)!=0)
	{
		return trans_c();
	}
	gDebugStep=13;
	if((buf[0] == 0) && (buf[1] == 0) && (buf[2] == 0))		  //�����������
	{		
			  
	    return trans_a();
    }
    gDebugStep=14;
    if((buf[0] == 0x55) && ((UBYTE)buf[1] == (UBYTE)0xaa) &&(buf[2] == 0x55))			//�ϴν����쳣�жϣ���Ҫ�ñ����ڵ����ݻָ�������Ϣ�������ݣ�
    {
    																					//�����ձ������ݿ��ڵĹ�����Ϣ����������ݸ��ǹ�����Ϣ���ڵ���Ӧ���ݣ�
	    gCardRecPtr = buf[7];															//ͬʱҪ���Ǯ�����ݵ�һ���ԣ����ݱ������ݿ���Ǯ�����ѡ����ȷ�����ݿ���лָ���
	    gCardStatus = buf[10];						   									//�����ݻָ������󰴽������̼������С�
    	if(gCardStatus!=2)
    	{
    		goto label_invalid;
    	}
    	*((char *)&gCardCnt+INT_HIGH) = buf[8];
    	*((char *)&gCardCnt+INT_LOW) = buf[9];
		gRestoreFlag = 0x55;
    	gMetroIoFlag = buf[14];
    	if(gCardRecPtr>9)
		{
		 	goto label_invalid;
	    }
    	if(gCardCnt ==0xffff) 
    	{
    		goto label_invalid;
    	}
    
		//B29,B30�ָ�
		if(mcmh_read(30,buf,0,0)!=0)
		{
			goto label_readerr;
		}
		if(lrc_chk2(buf,15)!=0)
		{
			if(mcmh_read(29,buf,2,0)!=0)
				goto label_readerr;
			if(lrc_chk2(buf,15)!=0)
			{
				memset(buf,0,16);
				mcmh_write(29,buf,2,0);
				mcmh_write(30,buf,2,0);	 
			}
			else
			{
				if(mcmh_write(30,buf,2,0)!=0)
				goto label_writeerr;
			}
		}
		else
		{
			if(mcmh_write(29,buf,2,0)!=0)
				goto label_writeerr;
		}
		if(NormalCard==COMMCARD)
		{	
			info[0]=buf[2];
			info[1]=buf[3];
			info[2]=buf[4];
			info[3]=buf[5];
			info[4]=buf[6];	
			memcpy(leisure,&buf[10],5);
	    }

		return trans_b();
	}
	gDebugStep=15;
	return trans_c();				//û��"55AA55"�Ľ��ױ�־���򱸷����ݿ���У�鲻��ȷ��
									//����Ϊ�ϴν��׳ɹ���ɣ�����ָ���Ƭ���ݡ�
									

label_countc_process:
	for(i=1;i<7;i++)						//�ƴο���Կ����
	{
		gKeyA[i][1]=gKeyA[i][1]-0x55;
	}
	memcpy(gKeyA[7],gKeyA[6],6);
	memcpy(gKeyA[8],gKeyA[6],6);
//	memcpy(&KeyBuf[6],gKeyA[3],6);
//	return 1;
	//�� block 12
	temp=0;
	Count_read_b12:
	gDebugStep=13;
	if(temp>2)
	{
		goto label_readerr;
	}
	if(mcmh_read(12,buf,0,0)!=0)
	{
		goto label_readerr;
	}
	if(mcmh_read(12,buf1,0,0)!=0)
	{
		goto label_readerr;
	}
	if(memcmp(buf,buf1,16)!=0)
	{
		temp++;
		goto Count_read_b12;
	}
	if(lrc_chk2(buf,15) != 0)
	{    
    	return trans_c2();
    }
	if((buf[0] == 0) && (buf[1] == 0) && (buf[2] == 0))		//�����������
	{ 
    	return trans_a2();
    }
	if((buf[0] == 0x55) && (buf[1] ==0xaa) &&(buf[2] == 0x55))		//�ϴν����쳣�жϣ���Ҫ�ñ����ڵ����ݻָ�������Ϣ�������ݣ�
    {
		gDebugStep=63;						                        //�����ձ������ݿ��ڵĹ�����Ϣ����������ݸ��ǹ�����Ϣ���ڵ���Ӧ���ݣ�
		gCardRecPtr = buf[7];										//ͬʱҪ���Ǯ�����ݵ�һ���ԣ����ݱ������ݿ���Ǯ�����ѡ����ȷ�����ݿ���лָ���
    	gCardStatus = buf[10];	
    	*((char *)&UseTimes+INT_HIGH) = buf[11];
		*((char *)&UseTimes+INT_LOW) = buf[12];
		*((char *)&gCardCnt+INT_HIGH) = buf[8];
    	*((char *)&gCardCnt+INT_LOW) = buf[9];
    	FirstDay[0]=buf[3];
		FirstDay[1]=buf[4];
		FirstDay[2]=buf[5];
		FirstDay[3]=buf[6];
		gRestoreFlag = 0x55;
    	gMetroIoFlag = buf[14];
    	if(gCardStatus!=1&&gCardStatus!=2)
    	{
    		goto label_invalid;
    	}
    	if(gCardRecPtr>9)
    	{
    		goto label_invalid;
    	}
    	if(gCardCnt >= 0xffff) 
		{
			goto label_invalid;
		}
		 gDebugStep=64;
		return trans_b2();
	}	
	return trans_c2();
	
label_invalid:
		gResponseStatus=CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_readerr:
		gResponseStatus=CE_READERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_writeerr:
		gResponseStatus=CE_WRITEERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_samerr:
		#ifdef _DEBUG_GETCARDINFO_
		debug_printf("\x0d\x0a Err:getcardinfo,sam err,debug step=%d",gDebugStep);
		#endif
	
		gResponseStatus=CE_PSAMERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
}

/****************************************************************
������trans_a
���ܣ���ͨ������A����
****************************************************************/
UBYTE trans_a(void)
{
	UBYTE buf[20],buf1[20];
	UBYTE i;
	
	gMetroIoFlagBk = 0;
	gMetroIoFlagBkValid =1;
	//��B13
	gDebugStep=20;
	if(mcmh_read(13,buf,2,0) != 0) 
	{
		goto label_err_read;
	}
	gDebugStep=21;
	if(lrc_chk2(buf,15) != 0)
	{
	    goto label_err_invalid;
	}
	gDebugStep=22;
	gCardStatus = buf[11];
	if(gCardStatus!=2)
	{
		goto label_err_invalid;
	}
	gDebugStep=23;
	gCardRecPtr = buf[12];
	*((char *)&gCardCnt+INT_HIGH) = buf[13];
	*((char *)&gCardCnt+INT_LOW) = buf[14];
	//B29��B30������ 
	
	if(mcmh_read(29,buf,0,0)!=0)
	{
		goto label_err_read;
	}
	if(mcmh_read(30,buf1,2,0)!=0)
	{
		goto label_err_read;
	}
	
	
	
	gDebugStep=24;
	if(lrc_chk2(buf,15)!=0)
	{
//		if(mcmh_read(30,buf1,2,0)!=0)
//		{
//			goto label_err_read;
//		}
		if(lrc_chk2(buf1,15)!=0)
		{
			memset(buf,0,16);
			mcmh_write(29,buf,2,0);
			mcmh_write(30,buf,2,0);	 
		}
		else
		{		 		
			if(mcmh_write(29,buf1,2,0)!=0)
			{
				goto label_err_write;
			}
		}	
	}
	else
	{				
		if(memcmp(buf1,buf,16)!=0)		 
			if(mcmh_write(30,buf,2,0)!=0)
			{
				goto label_err_write;
			}
	}
	if(NormalCard==COMMCARD)
	{	
		info[0]=buf[2];
		info[1]=buf[3];
		info[2]=buf[4];
		info[3]=buf[5];
		info[4]=buf[6];	
		memcpy(leisure,&buf[10],5);
	}
			
	//read block4
	gDebugStep=25;
	if(mcmh_read(4,buf,0,0) != 0) 
	{
		goto label_err_read;
	}
	gDebugStep=26;
	if(lrc_chk2(buf,15) != 0)
	{
		goto label_err_invalid;
	}
	cardversion=buf[8];
	//��B5
	gDebugStep=27;
	if(mcmh_read(5,buf,2,0) != 0) 
	{
		goto label_err_read;
	}
	gDebugStep=28;
	if(purse_chk(buf) != 0)
	{
	   goto label_err_invalid;
	}
	*((char *)&gCardPurse1+LONG_HIGH0) = buf[0];
	*((char *)&gCardPurse1+LONG_HIGH1) = buf[1];
	*((char *)&gCardPurse1+LONG_HIGH2) = buf[2];
	*((char *)&gCardPurse1+LONG_HIGH3) = buf[3];
	gCardPurse = gCardPurse1;	
	gDebugStep=29;
	if(mcmh_read(6,buf,2,0) != 0) 
	{
		goto label_err_read;
	}
	gDebugStep=30;
	if(purse_chk(buf) != 0) 
	{
		goto label_restore_b6;
	}	
	*((char *)&gCardPurse2+LONG_HIGH0) = buf[0];
	*((char *)&gCardPurse2+LONG_HIGH1) = buf[1];
	*((char *)&gCardPurse2+LONG_HIGH2) = buf[2];
	*((char *)&gCardPurse2+LONG_HIGH3) = buf[3];
	
	//���뱾����������
	gDebugStep=31;
	for(i=0;i<4;i++)
	{
		if(save[i]!=gThisCardSnr[i])
		{
			goto  notsave;
		}			
	}
	for(i=0;i<3;i++)
	{
		if(save[i+4]!=*((char *)&gCardPurse2+i))		//�Ƚ�Ǯ���ĵ�3�ֽ�
			goto notsave;
	}
	gCardCnt--;
	if(save[7]!=*((char *)&gCardCnt+INT_HIGH))
	{
		gCardCnt++;
		goto notsave;
	}
	if(save[8]!=*((char *)&gCardCnt+INT_LOW))
	{
		gCardCnt++;
		goto notsave;
	}
	if((save[9]!=gCardRecPtr-1)&&(save[9]!=gCardRecPtr+9))
	{
		gCardCnt++;
		goto notsave;
	}
	gCardRecPtr--;
	if(gCardRecPtr>9)
	{
		gCardRecPtr=9;
	}
	buf[0]=save[10];
	buf[15]=save[10];
	for(i=1;i<15;i++)
	{
		buf[i]=0;
	}
	gMetroIoFlag=save[10];
	if(mcmh_write(34,buf,0,0) != 0)
	{
		goto label_err_write;
	}
	if(mcmh_restore(6,0,0) != 0) 
	{
		goto label_err_read;
	}
	if(mcmh_transfer(5,2,0) != 0) 
	{
		goto label_err_write;
	}
	for(i=0;i<11;i++)
	{
		buf[i]=0;
	}
	buf[11]=gCardStatus;
	buf[12]=gCardRecPtr;
	buf[13]=*((char *)&gCardCnt+INT_HIGH);
	buf[14]=*((char *)&gCardCnt+INT_LOW);
	lrc_gen2(buf,15);
	if(mcmh_write(13,buf,0,0) != 0)
	{
		goto label_err_write;
	}
	for(i=11;i<15;i++)
	{
		buf[i]=0;
	}
	mcmh_write(gCardRecPos[gCardRecPtr%10],buf,2,0);
	if(mcmh_read(5,buf,2,0) != 0) 
	{
		goto label_err_read;
	}
	if(purse_chk(buf) != 0)
	{
    	goto label_err_invalid;
    }
	*((char *)&gCardPurse1+LONG_HIGH0) = buf[0];
	*((char *)&gCardPurse1+LONG_HIGH1) = buf[1];
	*((char *)&gCardPurse1+LONG_HIGH2) = buf[2];
	*((char *)&gCardPurse1+LONG_HIGH3) = buf[3];
	if(gCardPurse1 != gCardPurse2) 
	{
		goto label_err_write;
	}
	gCardPurse = gCardPurse2;
	goto label_backup;
notsave:

	gDebugStep=32;
	if(mcmh_read(34,buf,0,0) != 0)
	{
		goto label_err_read;
	}
	gMetroIoFlag=buf[0];
	gMetroIoFlag34 = gMetroIoFlag;
	memcpy(gB34, buf, 16);
	if(lrc_chk2(buf,15)==0)
	{
		gMetroIoFlag34Valid = 0;
	}
	else
	{
		gMetroIoFlag34Valid = 1;
	}
	gDebugStep=33;
	if(mcmh_read(32,gB32,2,0)!=0)
	{
		goto label_err_read;
	}
	lrc3_gen(gB32,16);
	memcpy(gB32BK,gB32,20);
	if(mcmh_read(33,gB33,2,0)!=0)
	{
		goto label_err_read;
	}
	lrc3_gen(gB33,16);
	memcpy(gB33BK,gB33,20);
	gDebugStep=34;
	

	if(gCardPurse1 == gCardPurse2) 
	{
		goto label_backup;
	}
label_restore_b6:
	if(mcmh_restore(5,0,0) != 0) 
	{
		goto label_err_read;
	}
	if(mcmh_transfer(6,2,0) != 0) 
	{
		goto label_err_write;
	}
	#ifdef _READBACK_
	if(mcmh_read(6,buf,2,0) != 0) 
	{
		goto label_err_read;
	}
	if(purse_chk(buf) != 0) 
	{
		goto label_err_write;
	}
	*((char *)&gCardPurse2+LONG_HIGH0) = buf[0];
	*((char *)&gCardPurse2+LONG_HIGH1) = buf[1];
	*((char *)&gCardPurse2+LONG_HIGH2) = buf[2];
	*((char *)&gCardPurse2+LONG_HIGH3) = buf[3];
	if(gCardPurse2 != gCardPurse1) 
	{
		goto label_err_write;
	}
	#endif	//readback
	 
label_backup:
	gClrFlag = 0;
	gDebugStep=35;

	return trans_d();	
	
	label_err_invalid:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_INVALIDCARD;
	return OP_ERR;

	label_err_write:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_WRITEERROR;
	return OP_ERR;

	label_err_read:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_READERROR;
	return OP_ERR;
}
/****************************************************************
������trans_b
���ܣ���ͨ������B����
****************************************************************/
UBYTE trans_b(void)
{
	UBYTE i,buf[20];
	
	gMetroIoFlag34 = 0;
	gMetroIoFlag34Valid =1;
	gMetroIoFlagBk = gMetroIoFlag;
	gMetroIoFlagBkValid =0;
	if((gMetroIoFlag != 0x55) && (gMetroIoFlag != 0x66)) 
	{
		goto label_read_b6; 
	}
	//�ؽ�B34
	buf[0] = gMetroIoFlag;
	for(i=1;i<15;i++) buf[i] = 0; 
	lrc_gen2(buf,15);
	if(mcmh_write(34,buf,2,0) != 0) 
	{
		goto label_err_write;
	}
	//��B6
label_read_b6:
	if(mcmh_read(32,gB32,2,0)!=0)
	{
		goto label_err_read;
	}
	lrc3_gen(gB32,16);
	memcpy(gB32BK,gB32,20);
	if(mcmh_read(33,gB33,2,0)!=0)
	{
		goto label_err_read;
	}
	lrc3_gen(gB33,16);
	memcpy(gB33BK,gB33,20);
	if(mcmh_read(4,buf,0,0) != 0) 
	{
		goto label_err_read;
	}
	if(lrc_chk2(buf,15) != 0)
	{
		goto label_invalid;
	}
	cardversion=buf[8];
	if(mcmh_read(6,buf,2,0) != 0) 
	{
		goto label_err_read; 
	}
	if(purse_chk(buf) != 0) 
	{
		goto label_invalid;
	}
	*((char *)&gCardPurse2+LONG_HIGH0) = buf[0];
	*((char *)&gCardPurse2+LONG_HIGH1) = buf[1];
	*((char *)&gCardPurse2+LONG_HIGH2) = buf[2];
	*((char *)&gCardPurse2+LONG_HIGH3) = buf[3];
	gCardPurse = gCardPurse2;
	if(mcmh_read(5,buf,2,0) != 0) 
	{
		goto label_err_read;
	}
	if(purse_chk(buf) != 0) 
	{
		goto label_restore_b5;
	}
	*((char *)&gCardPurse1+LONG_HIGH0) = buf[0];
	*((char *)&gCardPurse1+LONG_HIGH1) = buf[1];
	*((char *)&gCardPurse1+LONG_HIGH2) = buf[2];
	*((char *)&gCardPurse1+LONG_HIGH3) = buf[3];
	if(gCardPurse1 == gCardPurse2) 
	{	if((gMetroIoFlag != 0x55) && (gMetroIoFlag != 0x66))
		{
			if(mcmh_read(34,buf,0,0) != 0) 
			{
				goto label_err_read;
			}
			gMetroIoFlag=buf[0];
			gMetroIoFlag34 = gMetroIoFlag;
			memcpy(gB34, buf, 16);
			if(lrc_chk2(buf,15)==0)
			{
				gMetroIoFlag34Valid =0;
			}
			else
			{
				gMetroIoFlag34Valid = 1;
			}
			if(mcmh_read(12,buf,0,0) != 0) 
		    {
				goto label_err_read;
			}
			buf[14]=gMetroIoFlag;
			lrc_gen2(buf,15);
			if(mcmh_write(12,buf,2,0) != 0) 
		    {
				goto label_err_write;
			}
		}
		return trans_d();
	}
	//ʹ��B6�ָ�B5,trans_e
label_restore_b5:
	if(mcmh_restore(6,2,0) != 0) 
	{
		goto label_err_read; 
	}
	if(mcmh_transfer(5,2,0) != 0)
	{
		goto label_err_write;
	}	
#ifdef _READBACK_
	if(mcmh_read(5,buf,2,0) != 0) 
	{
		goto label_err_read;
	}
	if(purse_chk(buf) != 0) 
	{
		goto label_err_read;
	}
	*((char *)&gCardPurse1+LONG_HIGH0) = buf[0];
	*((char *)&gCardPurse1+LONG_HIGH1) = buf[1];
	*((char *)&gCardPurse1+LONG_HIGH2) = buf[2];
	*((char *)&gCardPurse1+LONG_HIGH3) = buf[3];
	if(gCardPurse1 != gCardPurse2) 
	{
		goto label_err_write;
	}
#endif
	
	return trans_d();
	
	label_invalid:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_INVALIDCARD;
	return OP_ERR;

	label_err_write:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_WRITEERROR;
	return OP_ERR;

	label_err_read:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_READERROR;
	return OP_ERR;
}
/****************************************************************
������trans_c
���ܣ���ͨ������C����
****************************************************************/
UBYTE trans_c(void)
{
	UBYTE buf[20];
	UBYTE i;
	
	
	gMetroIoFlagBk = 0;
	gMetroIoFlagBkValid =1;
	gDebugStep = 40;
	if(mcmh_read(13,buf,2,0) != 0) 
	{
		goto label_err_read;
	}
	gDebugStep = 41;
	if(lrc_chk2(buf,15) != 0) 
	{
		goto label_invalid;
	}
	gCardStatus = buf[11];
	if(gCardStatus!=2)
	{
		goto label_invalid;
	}
	gDebugStep = 42;
	gCardRecPtr = buf[12];
	*((char *)&gCardCnt+INT_HIGH) = buf[13];
	*((char *)&gCardCnt+INT_LOW) = buf[14];
	if(mcmh_read(34,buf,0,0)!=0)
	{
		goto label_err_read;
	}
	gMetroIoFlag=buf[0];
	gMetroIoFlag34 = gMetroIoFlag;
	memcpy(gB34, buf, 16);
	if(lrc_chk2(buf,15)==0)
	{
		gMetroIoFlag34Valid = 0;
	}
	else
	{
		gMetroIoFlag34Valid =1;
	}
	if(mcmh_read(32,gB32,2,0)!=0)
	{
		goto label_err_read;
	}
	lrc3_gen(gB32,16);
	memcpy(gB32BK,gB32,20);
	if(mcmh_read(33,gB33,2,0)!=0)
	{
		goto label_err_read;
	}
	lrc3_gen(gB33,16);
	memcpy(gB33BK,gB33,20);
	gDebugStep = 43;
	if(mcmh_read(4,buf,0,0) != 0) 
	{
		goto label_err_read;
	}
	if(lrc_chk2(buf,15) != 0)
	{
		goto label_invalid;
	}
	cardversion=buf[8];
	//��B6
	if(mcmh_read(6,buf,2,0) != 0) 
	{
		goto label_err_read;
	}
	if(purse_chk(buf) != 0) 
	{
		goto label_invalid;
	}
	*((char *)&gCardPurse2+LONG_HIGH0) = buf[0];
	*((char *)&gCardPurse2+LONG_HIGH1) = buf[1];
	*((char *)&gCardPurse2+LONG_HIGH2) = buf[2];
	*((char *)&gCardPurse2+LONG_HIGH3) = buf[3];
	gCardPurse = gCardPurse2;
	//���뱾����������
	for(i=0;i<4;i++)
	{
		if(save[i]!=gThisCardSnr[i])
		{
			goto  notsave;
		}			
	}
	for(i=0;i<3;i++)
	{
		if(save[i+4]!=*((char *)&gCardPurse2+i))
		{
			goto notsave;
		}
	}
	gCardCnt--;
	if(save[7]!=*((char *)&gCardCnt+INT_HIGH))
	{
		gCardCnt++;
		goto notsave;
	}
	if(save[8]!=*((char *)&gCardCnt+INT_LOW))
	{
		gCardCnt++;
		goto notsave;
	}
	if((save[9]!=gCardRecPtr-1)&&(save[9]!=gCardRecPtr+9))
	{
		gCardCnt++;
		goto notsave;
	}
	gCardCnt++;
	if(gMetroIoFlag==0x55)
	{
		gMetroIoFlag=0x66;
	}
	else
	{
		if(gMetroIoFlag==0x66)
		{
			gMetroIoFlag=0x55;
		}
		else
		{
			gMetroIoFlag=0;
		}
	}
	gClrFlag = 0;
notsave:
	gDebugStep = 44; 
	//��B5
	if(mcmh_read(5,buf,2,0) != 0) 
	{
		goto label_err_read;
	}
	if(purse_chk(buf) != 0) 
	{
		goto label_invalid;	
	}
	*((char *)&gCardPurse1+LONG_HIGH0) = buf[0];
	*((char *)&gCardPurse1+LONG_HIGH1) = buf[1];
	*((char *)&gCardPurse1+LONG_HIGH2) = buf[2];
	*((char *)&gCardPurse1+LONG_HIGH3) = buf[3];
	gDebugStep = 45;
	if(gCardPurse1 == gCardPurse2) 
	{
		//2011-8-11 10:56:51 ����Ϊ�Ը���block30Ϊ��
		if(mcmh_read(30,buf,0,0)!=0)
		{
			goto label_err_read;
		}
		if(lrc_chk2(buf,15)!=0)
		{
			if(mcmh_read(29,buf,2,0)!=0)
			{
				goto label_err_read;
			}
			if(lrc_chk2(buf,15)!=0)
			{
				memset(buf,0,16);
				mcmh_write(30,buf,2,0);
				mcmh_write(29,buf,2,0);	 
			}
			else
			{
				if(mcmh_write(30,buf,2,0)!=0)
				{
					goto label_err_write;
				}
			}	
		}
		else
		{
			gDebugStep = 46;
			if(mcmh_write(29,buf,2,0)!=0)
			{
				goto label_err_write;
			}
		}
		gDebugStep = 47;
		if(NormalCard==COMMCARD)
		{	
			info[0]=buf[2];
			info[1]=buf[3];
			info[2]=buf[4];
			info[3]=buf[5];
			info[4]=buf[6];	
			memcpy(leisure,&buf[10],5);
	  }	
		goto label_gen_bakup;		
	}
	if(mcmh_read(30,buf,0,0)!=0)
	{
		goto label_err_read;
	}
	if(lrc_chk2(buf,15)!=0)
	{
		if(mcmh_read(29,buf,2,0)!=0)
		{
			goto label_err_read;
		}
		if(lrc_chk2(buf,15)!=0)
		{
			memset(buf,0,16);
			mcmh_write(29,buf,2,0);
			mcmh_write(30,buf,2,0);	 
		}
		else
		{
			if(mcmh_write(30,buf,2,0)!=0)
			{
				goto label_err_write;
			}
		}	
	}
	else
	{
		if(mcmh_write(29,buf,2,0)!=0)
		{
			goto label_err_write;
		}
	}
	if(NormalCard==COMMCARD)
	{	
		info[0]=buf[2];
		info[1]=buf[3];
		info[2]=buf[4];
		info[3]=buf[5];
		info[4]=buf[6];	
		memcpy(leisure,&buf[10],5);
	}
	//next3:	 
	gCardCnt--;
	gCardRecPtr = (gCardRecPtr+9)%10;
	gRestoreFlag = 0x55;
	//��������
label_gen_bakup:	
	gDebugStep = 48;
	buf[0] = 0x55;
	buf[1] = 0xaa; 
	buf[2] = 0x55;
	buf[3] = *((char *)&gCardPurse+LONG_HIGH0); 
	buf[4] = *((char *)&gCardPurse+LONG_HIGH1); 
	buf[5] = *((char *)&gCardPurse+LONG_HIGH2); 
	buf[6] = *((char *)&gCardPurse+LONG_HIGH3); 
	buf[8] = *((char *)&gCardCnt+INT_HIGH);
	buf[9] = *((char *)&gCardCnt+INT_LOW);
	buf[7] = gCardRecPtr;
	buf[10] = gCardStatus;
	buf[11] = 0;
	buf[12] = 0;
	buf[13] = 0;
	buf[14] = gMetroIoFlag;
	lrc_gen2(buf,15);
	if(mcmh_write(12,buf,2,0) != 0) 
	{
		goto label_err_write;
	}
	if(gCardPurse1 == gCardPurse2) 
	{
		return trans_d();
	}
	//ʹ��B6�ָ�B5,trans_e
	if(mcmh_restore(6,0,0) != 0) 
	{
		goto label_err_read;
	}
	if(mcmh_transfer(5,2,0) != 0) 
	{
		goto label_err_write;
	}
#ifdef _READBACK_
	if(mcmh_read(5,buf,2,0) != 0) 
	{
		goto label_err_read;
	}
	if(purse_chk(buf) != 0) 
	{
		goto label_err_write;
	}
	*((char *)&gCardPurse1+LONG_HIGH0) = buf[0];
	*((char *)&gCardPurse1+LONG_HIGH1) = buf[1];
	*((char *)&gCardPurse1+LONG_HIGH2) = buf[2];
	*((char *)&gCardPurse1+LONG_HIGH3) = buf[3];
	if(gCardPurse1 != gCardPurse2) 
	{
		goto label_err_write;
	}
#endif
	
	return trans_d();
	
	label_invalid:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_INVALIDCARD;
	return OP_ERR;

	label_err_write:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_WRITEERROR;
	return OP_ERR;

	label_err_read:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_READERROR;
	return OP_ERR;
}
/****************************************************************
������trans_d
���ܣ���ͨ������D����
****************************************************************/
UBYTE trans_d(void)
{
	UBYTE buf[20];
	UBYTE i;
	
	
	if(SHGetKeyAStep2()!=0)						//ȡ��Կ
	{
		goto label_samerr;
	}
	memcpy(gKeyA[2],bgTaskKeyOutbuf,6);
	gDebugStep=36;
	//�б�Ҫʱ�ָ�ָ���  .
	if(gRestoreFlag == 0x55)
	{
		for(i=0;i<11;i++) buf[i] = 0;
		buf[11] = gCardStatus;
		buf[12] = gCardRecPtr;
		buf[13] = *((char *)&gCardCnt+INT_HIGH);
		buf[14] = *((char *)&gCardCnt+INT_LOW);
		lrc_gen2(buf,15);
		if(mcmh_write(13,buf,0,0) != 0) 
		{
			goto label_err_write;
		}
		for(i=11;i<15;i++)
		{
			buf[i]=0;
		}
		mcmh_write(gCardRecPos[gCardRecPtr%10],buf,2,0);
	}
	gDebugStep=37;
	//�������
	if(gClrFlag == 0x55)
	{
		memset(buf,0,16);
		if(mcmh_write(12,buf,2,0) != 0) 
		{
			goto label_err_write;
		}
	}
	gDebugStep=38;
	if(gCardCnt >= 0xffff)
	{
		goto label_invalid;
	}
	gDebugStep=39;
	if(gCardRecPtr > 9)
	{
		goto label_invalid;
	}
	gDebugStep=40;
	if(gCardStatus!=0x02)
	{
		goto label_invalid;
	}
	//��B8
	gDebugStep=41;
	if(mcmh_read(8,buf,2,0) != 0) 
	{
		goto label_err_read;
	}
	gCardExpiredDate[0] = buf[0x0c];
	gCardExpiredDate[1] = buf[0x0d];
	gCardExpiredDate[2] = buf[0x0e];
	gCardExpiredDate[3] = buf[0x0f];
	*((char *)&gCardAddSum+LONG_HIGH0) = buf[0];
	*((char *)&gCardAddSum+LONG_HIGH1) = buf[1];
	*((char *)&gCardAddSum+LONG_HIGH2) = buf[2];
	*((char *)&gCardAddSum+LONG_HIGH3) = buf[3];
	//���ж�����������
	gCardLastRemain = gCardAddSum+gCardPurse;
	//printf("card balance is %08x\n", gCardLastRemain);
	if(gCardLastRemain>0x7fffffff)
	{
		gCardThisRemain = ~gCardLastRemain+1;
		//printf("card balance is %08x\n", gCardThisRemain);
		if(gCardThisRemain>3000)
		{
			goto label_invalid;
		}
		gCardThisRemain = gCardLastRemain;
	}
	gDebugStep=42;
	if(trans_ms() != 0)
	{
   		return OP_ERR;
   	}	
	return 0;
	
	label_invalid:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_INVALIDCARD;
	return OP_ERR;

	label_err_write:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_WRITEERROR;
	return OP_ERR;

	label_err_read:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_READERROR;
	return OP_ERR;
	label_samerr:
	gResponseStatus=CE_PSAMERROR;
	gGetCardInfoStep=0xff;
	return OP_ERR;
}
/****************************************************************
������trans_a2
���ܣ��ƴο�����A����
****************************************************************/
UBYTE trans_a2(void)
{
	UBYTE buf[20],buf1[20];
	UBYTE i;
	
	gMetroIoFlagBk = 0;
	gMetroIoFlagBkValid =1;
	gClrFlag = 0;
	//��B13
	gDebugStep=49;
	if(mcmh_read(13,buf,2,0) != 0) 
	{
		goto label_err_read;
	}
	gDebugStep=50;
	if(lrc_chk2(buf,15) != 0)
	{
	    goto label_err_invalid;
	}
	gDebugStep=51;
	*((char *)&UseTimes+INT_HIGH)=buf[5];
	*((char *)&UseTimes+INT_LOW)=buf[6];
	FirstDay[0]=buf[7];
	FirstDay[1]=buf[8];
	FirstDay[2]=buf[9];
	FirstDay[3]=buf[10];
	gCardStatus = buf[11];
	if(gCardStatus!=1&&gCardStatus!=2)
	{
		goto label_err_invalid;
	}
	gDebugStep=52;
	gCardRecPtr = buf[12];
	if(gCardRecPtr>9)
	{
		goto label_err_invalid;
	}
	*((char *)&gCardCnt+INT_HIGH) = buf[13];
	*((char *)&gCardCnt+INT_LOW) = buf[14];
	gDebugStep=53;
	if(gCardCnt>=0xffff)
	{
		goto label_err_invalid;
	}
	gDebugStep=54;
	if(mcmh_read(4,buf,0,0) != 0) 
	{
		goto label_err_read;
	}
	if(lrc_chk2(buf,15) != 0)
	{
		goto label_err_invalid;
	}
	cardversion=buf[8];
	//��B5
	gDebugStep=55;
	if(mcmh_read(5,buf,2,0) != 0) 
	{
		goto label_err_read;
	}
	if(purse_chk(buf) != 0)
	{
	   goto label_err_invalid;
	}
	*((char *)&gCardPurse1+LONG_HIGH0) = buf[0];
	*((char *)&gCardPurse1+LONG_HIGH1) = buf[1];
	*((char *)&gCardPurse1+LONG_HIGH2) = buf[2];
	*((char *)&gCardPurse1+LONG_HIGH3) = buf[3];
	gCardPurse = gCardPurse1;
	gDebugStep=56;
	if(mcmh_read(6,buf,2,0) != 0) 
	{
		goto label_err_read;
	}
	if(purse_chk(buf) != 0) 
	{
		goto label_restore_b6;
	}
	*((char *)&gCardPurse2+LONG_HIGH0) = buf[0];
	*((char *)&gCardPurse2+LONG_HIGH1) = buf[1];
	*((char *)&gCardPurse2+LONG_HIGH2) = buf[2];
	*((char *)&gCardPurse2+LONG_HIGH3) = buf[3];
	
	//���뱾����������
	for(i=0;i<4;i++)
	{
		if(save[i]!=gThisCardSnr[i])
		{
			goto  notsave2;
		}			
	}
	for(i=0;i<3;i++)
	{
		if(save[i+4]!=*((char *)&gCardPurse2+i))
		{
			goto notsave2;
		}
	}
	gCardCnt--;
	if(save[7]!=*((char *)&gCardCnt+INT_HIGH))
	{
		gCardCnt++;
		goto notsave2;
	}
	if(save[8]!=*((char *)&gCardCnt+INT_LOW))
	{
		gCardCnt++;
		goto notsave2;
	}
	if((save[9]!=gCardRecPtr-1)&&(save[9]!=gCardRecPtr+9))
	{
		gCardCnt++;
		goto notsave2;
	}
	gCardRecPtr--;
	if(gCardRecPtr>9)
	{
		gCardRecPtr=0;
	}
	if(mcmh_read(34,buf1,0,0)!=0)
	{
		goto label_err_read;
	}
	if(buf1[0]==0x66)
	{
		UseTimes--;
	}
	buf[0]=save[10];
	buf[15]=save[10];
	for(i=1;i<15;i++)
	{
		buf[i]=0;
	}
	if(mcmh_write(34,buf,2,0) != 0)
	{
		goto label_err_write;
	}
	gMetroIoFlag=save[10];
	if(mcmh_restore(6,0,0) != 0) 
	{
		goto label_err_read;
	}
	if(mcmh_transfer(5,2,0) != 0) 
	{
		goto label_err_write;
	}
	for(i=0;i<5;i++)
	{
		buf[i]=0;
	}
	buf[5] =*((char *)&UseTimes+INT_HIGH);
	buf[6] =*((char *)&UseTimes+INT_LOW);
	buf[7] =FirstDay[0];
	buf[8] =FirstDay[1];
	buf[9] =FirstDay[2];
	buf[10] =FirstDay[3];
	buf[11]=gCardStatus;
	buf[12]=gCardRecPtr;
	buf[13]=*((char *)&gCardCnt+INT_HIGH);
	buf[14]=*((char *)&gCardCnt+INT_LOW);
	lrc_gen2(buf,15);
	if(mcmh_write(13,buf,0,0) != 0)
	{
		goto label_err_write;
	}
	for(i=5;i<15;i++)
	{
		buf[i]=0;
	}
	mcmh_write(gCardRecPos[gCardRecPtr%10],buf,2,0);
	if(mcmh_read(5,buf,2,0) != 0) goto label_err_read;
	if(purse_chk(buf) != 0)
	{
	    goto label_err_invalid;
	}
	*((char *)&gCardPurse1+LONG_HIGH0) = buf[0];
	*((char *)&gCardPurse1+LONG_HIGH1) = buf[1];
	*((char *)&gCardPurse1+LONG_HIGH2) = buf[2];
	*((char *)&gCardPurse1+LONG_HIGH3) = buf[3];
	if(gCardPurse1 != gCardPurse2) 
	{
		goto label_err_write;
	}
	gCardPurse = gCardPurse2;
	goto label_backup;
notsave2:
	gDebugStep=57;
	if(mcmh_read(34,buf,2,0) != 0)
	{
		goto label_err_write;
	}
	gMetroIoFlag=buf[0];
	gMetroIoFlag34 = gMetroIoFlag;
	if(lrc_chk2(buf,15)==0)
	{
		gMetroIoFlag34Valid = 0;
	}
	else
	{
		gMetroIoFlag34Valid = 1;
	}
	if(mcmh_read(32,gB32,2,0)!=0)
	{
		goto label_err_read;
	}
	lrc3_gen(gB32,16);
	memcpy(gB32BK,gB32,20);
	if(mcmh_read(33,gB33,2,0)!=0)
	{
		goto label_err_read;
	}
	lrc3_gen(gB33,16);
	memcpy(gB33BK,gB33,20);
	if(gCardPurse1 != gCardPurse2) 
	{
		goto label_restore_b6;
	}
	gDebugStep=58;
	goto label_backup;
label_restore_b6:
	gDebugStep=59;
	if(mcmh_restore(5,2,0) != 0) 
	{
		goto label_err_read;
	}
	if(mcmh_transfer(6,2,0) != 0)
	{
		goto label_err_write;
	}
#ifdef _READBACK_
	if(mcmh_read(6,buf,2,0) != 0) 
	{
		goto label_err_read;
	}
	if(purse_chk(buf) != 0) 
	{
		goto label_err_write;
	}
	*((char *)&gCardPurse2+LONG_HIGH0) = buf[0];
	*((char *)&gCardPurse2+LONG_HIGH1) = buf[1];
	*((char *)&gCardPurse2+LONG_HIGH2) = buf[2];
	*((char *)&gCardPurse2+LONG_HIGH3) = buf[3];
	if(gCardPurse2 != gCardPurse1) 
	{
		goto label_err_write;
	}
#endif	//readback	
	gCardPurse = gCardPurse1;
	
label_backup:
	gDebugStep=60;
	if(mcmh_read(13,buf,0,0) != 0) 
	{
		goto label_err_read;
	}
	if(lrc_chk2(buf,15) != 0)
	{
	    goto label_err_invalid;
	}
	
	buf[0]=0x55;
	buf[1]=0xaa;
	buf[2]=0x55;
	memcpy(buf+3,buf+7,4);
	buf[10]=buf[11];
	buf[7]=buf[12];
	buf[8]=buf[13];
	buf[9]=buf[14];
	buf[11]=*((char *)&UseTimes+INT_HIGH);
	buf[12]=*((char *)&UseTimes+INT_LOW);
	buf[13]=0;
	buf[14]=gMetroIoFlag;
	lrc_gen2(buf,15);
	if(mcmh_write(12,buf,2,0) != 0) 
	{
		goto label_err_write;
	}
	gDebugStep=61;
	return trans_d2();

	
	label_err_invalid:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_INVALIDCARD;
	return OP_ERR;

	label_err_write:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_WRITEERROR;
	return OP_ERR;

	label_err_read:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_READERROR;
	return OP_ERR;
}
/****************************************************************
������trans_b2
���ܣ��ƴο�����B����
****************************************************************/
UBYTE trans_b2(void)
{
	UBYTE buf[20],buf1[20];
	UBYTE i;
	
	//label_read_b4:
	gDebugStep=65;
	if(mcmh_read(4,buf,0,0) != 0) 
	{
		goto label_err_read;
	}
	if(lrc_chk2(buf,15) != 0)
	{
		goto label_invalid;
	}
	cardversion=buf[8];
	if(mcmh_read(6,buf,2,0) != 0) 
	{
		goto label_err_read; 
	}
	if(purse_chk(buf) != 0) 
	{
		goto label_invalid;
	}
	*((char *)&gCardPurse2+LONG_HIGH0) = buf[0];
	*((char *)&gCardPurse2+LONG_HIGH1) = buf[1];
	*((char *)&gCardPurse2+LONG_HIGH2) = buf[2];
	*((char *)&gCardPurse2+LONG_HIGH3) = buf[3];
	gCardPurse = gCardPurse2;
	if(mcmh_read(5,buf,2,0) != 0) 
	{
		goto label_err_read;
	}
	if(purse_chk(buf) != 0) 
	{
		goto label_restore_b5;
	}
	*((char *)&gCardPurse1+LONG_HIGH0) = buf[0];
	*((char *)&gCardPurse1+LONG_HIGH1) = buf[1];
	*((char *)&gCardPurse1+LONG_HIGH2) = buf[2];
	*((char *)&gCardPurse1+LONG_HIGH3) = buf[3];
	if(gCardPurse1!=gCardPurse2)
	{
		goto label_restore_b5;
	}
	goto  label_next;
	//ʹ��B6�ָ�B5,trans_e
label_restore_b5:
	gDebugStep=66;
	if(mcmh_restore(6,2,0) != 0) 
	{
		goto label_err_read;
	}
	if(mcmh_transfer(5,2,0) != 0) 
	{
		goto label_err_write;
	}
#ifdef _READBACK_
	if(mcmh_read(5,buf,2,0) != 0) 
	{
		goto label_err_read;
	}
	if(purse_chk(buf) != 0) 
	{
		goto label_err_read;
	}
	*((char *)&gCardPurse1+LONG_HIGH0) = buf[0];
	*((char *)&gCardPurse1+LONG_HIGH1) = buf[1];
	*((char *)&gCardPurse1+LONG_HIGH2) = buf[2];
	*((char *)&gCardPurse1+LONG_HIGH3) = buf[3];
	if(gCardPurse1 != gCardPurse2) 
	{
		goto label_err_write;
	}
#endif

label_next:
	gDebugStep=67;
	if(mcmh_read(32,gB32,2,0)!=0)
	{
		goto label_err_read;
	}
	lrc3_gen(gB32,16);
	memcpy(gB32BK,gB32,20);
	if(mcmh_read(33,gB33,2,0)!=0)
	{
		goto label_err_read;
	}
	lrc3_gen(gB33,16);
	memcpy(gB33BK,gB33,20);
	gMetroIoFlag34 = 0;
	gMetroIoFlag34Valid =1;
	gMetroIoFlagBk = gMetroIoFlag;
	gMetroIoFlagBkValid =0;
	if((gMetroIoFlag != 0x55) && (gMetroIoFlag != 0x66))
	{
		for(i=0;i<5;i++)
		{
			buf1[i]=0;
		}
		buf1[5]=*((char *)&UseTimes+0);
		buf1[6]=*((char *)&UseTimes+1);
		buf1[7]=FirstDay[0];
		buf1[8]=FirstDay[1];
		buf1[9]=FirstDay[2];
		buf1[10]=FirstDay[3];
		buf1[11]=gCardStatus;
		buf1[12]=gCardRecPtr;
		buf1[13]=*((char *)&gCardCnt+INT_HIGH);
		buf1[14]=*((char *)&gCardCnt+INT_LOW);
		lrc_gen2(buf1,15);
		if(mcmh_write(13,buf1,0,0) != 0) 
		{
			goto label_err_write;
		}
		for(i=5;i<15;i++)
		{
			buf1[i]=0;
		}
		mcmh_write(gCardRecPos[gCardRecPtr%10],buf1,2,0);
		if(mcmh_read(34,buf,0,0) != 0) 
		{
			goto label_err_read;
		}
		gMetroIoFlag=buf[0];
		gMetroIoFlag34 = gMetroIoFlag;
		if(lrc_chk2(buf,15)==0)
		{
			gMetroIoFlag34Valid =0;
		}
		else
		{
			gMetroIoFlag34Valid =1;
		}
		if(mcmh_read(12,buf,0,0) != 0) 
		{
			goto label_err_read;
		}
		if(lrc_chk2(buf,15) != 0)
		{
			goto label_err_read;
		}
		buf[14]=gMetroIoFlag;
		lrc_gen2(buf,15);
		if(mcmh_write(12,buf,2,0) != 0) 
		{
			goto label_err_write;
		}
	}

	return trans_d2();
	
	label_invalid:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_INVALIDCARD;
	return OP_ERR;

	label_err_write:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_WRITEERROR;
	return OP_ERR;

	label_err_read:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_READERROR;
	return OP_ERR;
}
/****************************************************************
������trans_c2
���ܣ��ƴο�����C����
****************************************************************/
UBYTE trans_c2(void)
{
	UBYTE buf[20];
	UBYTE equ;
	
	gMetroIoFlagBk = 0;
	gMetroIoFlagBkValid =1;
	//��B13
	if(mcmh_read(13,buf,2,0) != 0) 
	{
		goto label_err_read;
	}
	if(lrc_chk2(buf,15) != 0)
	{
	    goto label_invalid;
	}
	gCardStatus = buf[11];
	if(gCardStatus!=1&&gCardStatus!=2)
	{
		goto label_invalid;
	}
	gCardRecPtr = buf[12];
	if(gCardRecPtr>9)
	{
		goto label_invalid;
	}
	*((char *)&gCardCnt+INT_HIGH) = buf[13];
	*((char *)&gCardCnt+INT_LOW) = buf[14];
	if(gCardCnt==0xffff)
	{
		goto label_invalid;
	}	
	*((char *)&UseTimes+INT_HIGH)=buf[5];
	*((char *)&UseTimes+INT_LOW)=buf[6];
	FirstDay[0]=buf[7];
	FirstDay[1]=buf[8];
	FirstDay[2]=buf[9];
	FirstDay[3]=buf[10];
	if(mcmh_read(4,buf,0,0) != 0) 
	{	
		goto label_err_read;
	}
	if(lrc_chk2(buf,15) != 0)
 	{
 		goto label_invalid;
	}
	cardversion=buf[8];
	if(mcmh_read(6,buf,2,0) != 0) 
	{
		goto label_err_read; 
	}
	if(purse_chk(buf) != 0) 
	{
		goto label_invalid;
	}
	*((char *)&gCardPurse2+LONG_HIGH0) = buf[0];
	*((char *)&gCardPurse2+LONG_HIGH1) = buf[1];
	*((char *)&gCardPurse2+LONG_HIGH2) = buf[2];
	*((char *)&gCardPurse2+LONG_HIGH3) = buf[3];
	gCardPurse=gCardPurse2;
	if(mcmh_read(5,buf,2,0) != 0) 
	{
		goto label_err_read;
	}
	if(purse_chk(buf) != 0) 
	{
		goto label_invalid;
	}
	*((char *)&gCardPurse1+LONG_HIGH0) = buf[0];
	*((char *)&gCardPurse1+LONG_HIGH1) = buf[1];
	*((char *)&gCardPurse1+LONG_HIGH2) = buf[2];
	*((char *)&gCardPurse1+LONG_HIGH3) = buf[3];
	if(gCardPurse1!=gCardPurse2)
	{
		equ=0x55;
	}
	else
	{
		equ=0;
	}
	if(mcmh_read(34,buf,0,0)!=0)
	{
		goto label_err_read;
	}
	gMetroIoFlag=buf[0];
	gMetroIoFlag34 = gMetroIoFlag;	
	if(lrc_chk2(buf,15) != 0)
 	{
 		gMetroIoFlag34Valid =1;
	}
	else
	{
		gMetroIoFlag34Valid =0;
	}
	
	if(mcmh_read(32,gB32,2,0)!=0)
	{
		goto label_err_read;
	}
	lrc3_gen(gB32,16);
	memcpy(gB32BK,gB32,20);
	if(mcmh_read(33,gB33,2,0)!=0)
	{
		goto label_err_read;
	}
	lrc3_gen(gB33,16);
	memcpy(gB33BK,gB33,20);
	if(equ==0x55)
	{
		gCardRecPtr--;
		if(gCardRecPtr>9)
		{
			gCardRecPtr=9;
		}
		gCardCnt--;
		UseTimes--;
		if(gMetroIoFlag==0x55)
		{
			gMetroIoFlag=0x66;
		}
		else
		{
			if(gMetroIoFlag==0x66)
			{
				gMetroIoFlag=0x55;
			}
			else
			{
				gMetroIoFlag=0;
			}
		}
		buf[0]=0;
		buf[15]=0;
		mcmh_write(gCardRecPos[gCardRecPtr%10],buf,2,0);
	}
	buf[0]=0x55;
	buf[1]=0xaa;
	buf[2]=0x55;
	buf[3]=FirstDay[0];
	buf[4]=FirstDay[1];
	buf[5]=FirstDay[2];
	buf[6]=FirstDay[3];
	buf[7]=gCardRecPtr;
	buf[8]=*((char *)&gCardCnt+0);
	buf[9]=*((char *)&gCardCnt+1);
	buf[10]=gCardStatus;
	buf[11]=*((char *)&UseTimes+INT_HIGH);
	buf[12]=*((char *)&UseTimes+INT_LOW);
	buf[13]=0;
	buf[14]=gMetroIoFlag;
	lrc_gen2(buf,15);
	if(mcmh_write(12,buf,2,0) != 0) 
	{
		goto label_err_write;
	}
	if(equ)
	{
		equ=0;
		//ʹ��B6�ָ�B5,trans_e
		if(mcmh_restore(6,0,0) != 0) 
		{
			goto label_err_read;
		}
		if(mcmh_transfer(5,2,0) != 0) 
		{
			goto label_err_write;
		}
		if(mcmh_read(5,buf,2,0) != 0) 
		{
			goto label_err_read;
		}
		if(purse_chk(buf) != 0) 
		{
			goto label_err_write;
		}
	}
	return trans_d2();
	
	label_invalid:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_INVALIDCARD;
	return OP_ERR;

	label_err_write:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_WRITEERROR;
	return OP_ERR;

	label_err_read:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_READERROR;
	return OP_ERR;
}
/****************************************************************
������trans_d2
���ܣ��ƴο�����D����
****************************************************************/
UBYTE trans_d2(void)
{
	UBYTE buf[20];
	UDWORD maxtime;
	
	if(SHGetKeyAStep2()!=0)						//ȡ��Կ
	{
		goto label_samerr;
	}
	memcpy(gKeyA[2],bgTaskKeyOutbuf,6);
	gKeyA[2][1]=gKeyA[2][1]-0x55;
	//��B8
	gDebugStep=62;
	if(mcmh_read(8,buf,0,0) != 0) 
	{
		goto label_err_read;
	}
	gCardExpiredDate[0] = buf[0x0c];
	gCardExpiredDate[1] = buf[0x0d];
	gCardExpiredDate[2] = buf[0x0e];
	gCardExpiredDate[3] = buf[0x0f];
	*((char *)&gCardAddSum+LONG_HIGH0) = buf[0];
	*((char *)&gCardAddSum+LONG_HIGH1) = buf[1];
	*((char *)&gCardAddSum+LONG_HIGH2) = buf[2];
	*((char *)&gCardAddSum+LONG_HIGH3) = buf[3];
	*((char *)&maxtime+LONG_HIGH0) =buf[0];
	*((char *)&maxtime+LONG_HIGH1) =buf[1];
	*((char *)&maxtime+LONG_HIGH2) =buf[2];
	*((char *)&maxtime+LONG_HIGH3) =buf[3];
	DayToAllow=buf[4];
	gCardLastRemain=maxtime+gCardPurse;
	//printf("cnt card blance %08x\n", gCardLastRemain);
	if(gCardLastRemain>0x7fffffff)
	{
		goto label_invalid;
	}
	if(gCardLastRemain==0)
	{
		goto label_invalid;
	}
	if(UseType==0)
	{
		goto chuli;	
	}
	if(memcmp(FirstDay,"\x0\x0\x0\x0",4)==0)
	{
		ChangeDate=0x55;
	}
chuli:
	if(trans_ms() != 0)
	{
	   return OP_ERR;
	}
	
	return 0;
	
	label_invalid:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_INVALIDCARD;
	return OP_ERR;
	label_err_read:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_READERROR;
	return OP_ERR;
	label_samerr:
	gResponseStatus=CE_PSAMERROR;
	gGetCardInfoStep=0xff;
	return OP_ERR;
}

/****************************************************************
������trans_ms
���ܣ�������Ϣ����
****************************************************************/
UBYTE trans_ms(void)
{
	if(gMetroIoFlag==0x55)
	{
		goto trans_mb;
	}
	if(gMetroIoFlag==0x66)
	{
		goto trans_ma;
	}
	gBFirst=34;
	return 0; 
trans_ma:
	gBFirst = 33;
	return 0;
trans_mb:
	gBFirst = 32;
	return 0;
}
/****************************************************************
������trans_entry
���ܣ�������վ����
****************************************************************/
UBYTE trans_entry(void)
{
	UWORD  tint;
	UBYTE  buf[20],buf32[16],snbuf[6],eeflag;
	UBYTE  temp2;
	UBYTE  buf33[3];
	#ifdef _DEBUG_SHTRANS_
		debug_printf("\x0d\x0a trans_entry ");
	#endif
	//�������� 
	save[0]=gThisCardSnr[0];
	save[1]=gThisCardSnr[1];
	save[2]=gThisCardSnr[2];
	save[3]=gThisCardSnr[3];
	save[4]=*((char *)&gCardPurse+LONG_HIGH0); 
	save[5]=*((char *)&gCardPurse+LONG_HIGH1); 
	save[6]=*((char *)&gCardPurse+LONG_HIGH2); 
	save[7]=*((char *)&gCardCnt+INT_HIGH);
	save[8]=*((char *)&gCardCnt+INT_LOW);
	save[9]=gCardRecPtr;
	save[10]=gMetroIoFlag;
	
	eeflag = 0 ;
	
	//дB12���� 
	buf[0] = 0x55;
	buf[1] = 0xaa; 
	buf[2] = 0x55;
	if(NormalCard!=CNTCARD)
	{
		buf[3] = *((char *)&gCardPurse+LONG_HIGH0); 
		buf[4] = *((char *)&gCardPurse+LONG_HIGH1); 
		buf[5] = *((char *)&gCardPurse+LONG_HIGH2); 
		buf[6] = *((char *)&gCardPurse+LONG_HIGH3); 
		buf[8] = *((char *)&gCardCnt+INT_HIGH);
		buf[9] = *((char *)&gCardCnt+INT_LOW);		
	}
	else
	{
		if(ChangeDate==0x55)
		{
			buf[3]=gTransTime[0];
			buf[4]=gTransTime[1];
			buf[5]=gTransTime[2];
			buf[6]=gTransTime[3];
		}
		else
		{
			buf[3]=FirstDay[0];
			buf[4]=FirstDay[1];
			buf[5]=FirstDay[2];
			buf[6]=FirstDay[3];
		}
		buf[8] = *((char *)&gCardCnt+INT_HIGH);
		buf[9] = *((char *)&gCardCnt+INT_LOW);
	}
	buf[7] = gCardRecPtr;
	buf[10] = gCardStatus;
	if(NormalCard!=CNTCARD)
	{
		buf[11] = 0;
		buf[12] = 0;
	}
	else
	{	
		buf[11]=*((char *)&UseTimes+INT_HIGH);
		buf[12]=*((char *)&UseTimes+INT_LOW);
	}
	buf[13] = 0;
	buf[14] = gMetroIoFlag;
	lrc_gen2(buf,15);
	if(mcmh_write(12,buf,0,0) != 0) 
	{
		gResponseStatus = CE_WRITEERROR;
		return OP_ERR;
	}
	
	gCardThisTransValue =0;    
	gCardThisRemain = gCardLastRemain-gCardThisTransValue;
	//����TAC
	KeyBuf[0]=0x88;
	KeyBuf[1] = *((char *)&gPosTransCnt+LONG_HIGH2);
	KeyBuf[2] = *((char *)&gPosTransCnt+LONG_HIGH1);
	KeyBuf[3] = *((char *)&gPosTransCnt+LONG_HIGH0);
	KeyBuf[4] = gCityCode0;
	KeyBuf[5] = gCityCode1;
	KeyBuf[6] = gThisCardSnr[0];
	KeyBuf[7] = gThisCardSnr[1];
	KeyBuf[8] = gThisCardSnr[2];
	KeyBuf[9] = gThisCardSnr[3];
	KeyBuf[10] = gCardType;
	KeyBuf[11] = *((char *)&gCardLastRemain+LONG_HIGH0);
	KeyBuf[12] = *((char *)&gCardLastRemain+LONG_HIGH1);
	KeyBuf[13] = *((char *)&gCardLastRemain+LONG_HIGH2);
	KeyBuf[14] = *((char *)&gCardThisTransValue+LONG_HIGH0);
	KeyBuf[15] = *((char *)&gCardThisTransValue+LONG_HIGH1);
	KeyBuf[16] = *((char *)&gCardThisTransValue+LONG_HIGH2);
	memcpy(&KeyBuf[17],gTransTime,7);
	KeyBuf[24] = *((char *)&gCardCnt+INT_HIGH);
	KeyBuf[25] = *((char *)&gCardCnt+INT_LOW);
	memcpy(&KeyBuf[26],bpgSHpsamNo,4);
	KeyBuf[30] = 0x80;
	KeyBuf[31] = 0x00;
	SHTransTACStep1(KeyBuf);
		
	if(NormalCard==CNTCARD)
	{
		goto timecntpre;
	}
	//�������Ͻ��׼�¼
	trans_gen_card_record(0x11,buf);

	if(mcmh_write(gCardRecPos[gCardRecPtr%10],buf,0,0) != 0)
	{
		gResponseStatus = CE_WRITEERROR;
	 	return OP_ERR;
	}
	if(checkrec_inmemory(REC_EXIT)!=0)
	{
		if(mcmh_read(33,gB33,0,0)!=0)
		{
			gResponseStatus = CE_READERROR;
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
		if(mcmh_read(32,gB32,0,0)!=0)
		{
			gResponseStatus = CE_READERROR;
			return OP_ERR;
		}
		lrc3_gen(gB32,16);
		memcpy(gB32BK,gB32,20);
	}
	memcpy(buf,gB32,16);
	if(memcmp(buf,"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff",16)==0)
	{
		eeflag = 0x55;
		ee_read(ADDR_CHECK_B32,4,snbuf);
		if(memcmp(gThisCardSnr,snbuf,4)==0)
		{
			ee_read(ADDR_CHECK_B32+4,16,buf);
			memcpy(buf32,buf,16);
		}
	}
	else
	memcpy(buf32,buf,16);
	//���¿��ϵ���ר����������	   
 
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

	if(mcmh_write(32,buf,0,0) != 0)
	{
		if(eeflag==0)
		{
			ee_write(ADDR_CHECK_B32,4,gThisCardSnr);
			ee_write(ADDR_CHECK_B32+4,16,buf32);
		}
		gResponseStatus = CE_WRITEERROR;
	 	return OP_ERR;
	}
	//write b34
	buf[0] = 0x55;
	buf[15] = 0x55;
	for(tint=1;tint<15;tint++) 
	{
		buf[tint] = 0;
	}
	if(mcmh_write(34,buf,2,0) != 0)
	{
		gResponseStatus = CE_WRITEERROR;
	 	return OP_ERR;
	}
	//write b13
	for(temp2=0;temp2<11;temp2++)
	{
		buf[temp2] = 0;
	}
	buf[11] = gCardStatus;
	buf[12] = (gCardRecPtr+1)%10;
	tint = gCardCnt+1;
	buf[13] = *((char *)&tint+INT_HIGH);
	buf[14] = *((char *)&tint+INT_LOW);
	lrc_gen2(buf,15);
	if(mcmh_write(13,buf,0,0) != 0)
	{
		gResponseStatus = CE_WRITEERROR;
	 	return OP_ERR;
	}
	if(SHGetTacStep2()!=0)					   //ȡTAC
	{
		gResponseStatus = CE_PSAMERROR;
		return OP_ERR;
	}
	memcpy(gTAC,bgTaskKeyOutbuf,4);
	#ifdef _DEBUG_SHTRANS_
		debug_printf("\x0d\x0a GET TAC=%02x %02x %02x %02x ",gTAC[0],gTAC[1],gTAC[2],gTAC[3]);
	#endif
	for(temp2=0;temp2<16;temp2++)
	{
		buf[temp2] = 0;
	}
	if(mcmh_write(12,buf,2,0) != 0)
	{
	  	gResponseStatus = CE_PUTBACKCARD1;
	    return OP_ERR;
	}
	for(temp2=0;temp2<11;temp2++)
	{
		save[temp2]=0;	   
	}
	
	return 0;
timecntpre:					//�ƴο�����
	//д��������¼
	trans_gen_card_record(0x11,buf);	
	if(mcmh_write(gCardRecPos[gCardRecPtr],buf,0,0) != 0) 
	{
		gResponseStatus = CE_WRITEERROR;
 		return OP_ERR;
	}
	if(mcmh_read(33,buf,0,0)!=0)
	{
		gResponseStatus = CE_READERROR;
	 	return OP_ERR;
	}
	temp2=buf[0];								//�����Ż����� 
	buf33[0]=buf[10];
	buf33[1]=buf[11];
	buf33[2]=buf[12];
	if(mcmh_read(32,buf,2,0)!=0)		
	{
		gResponseStatus = CE_READERROR;
	 	return OP_ERR;
	}
	if(memcmp(buf,"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff",16)==0)
	{
		eeflag = 0x55;
		ee_read(ADDR_CHECK_B32,4,snbuf);
		if(memcmp(gThisCardSnr,snbuf,4)==0)
		{
			ee_read(ADDR_CHECK_B32+4,16,buf);
			memcpy(buf32,buf,16);
		}
	}
	else
	memcpy(buf32,buf,16);
	//���¿��ϵ���ר����������	   
	 
	if(gStationTimeOverRideFlag == 0x00) 
	{
			if(temp2&0x08)
				buf[0] = 0x48;
			else
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
			buf[0]=gStationTimeOverRideFlag*2;
			buf[0]+=0x40;
			if(temp2&0x08)
				buf[0]+=0x09;
		}
		if(gStationTimeOverRideFlag&0x08)
		{
			buf[10]=buf33[0];
			buf[11]=buf33[1];
			buf[12]=buf33[2];
			buf[13]=*((char *)&gCardThisRemain+LONG_HIGH0);
			buf[14]=*((char *)&gCardThisRemain+LONG_HIGH1);
			goto wr32cnt;
		}
	}
	memcpy(buf+1,gTransTime+1,5);
	memcpy(buf+6,gPosNo,4);
	buf[10]=0;
	buf[11]=0;
	buf[12]=0;
	buf[13]=*((char *)&gCardThisRemain+LONG_HIGH0);
	buf[14]=*((char *)&gCardThisRemain+LONG_HIGH1);
wr32cnt:
	lrc_gen2(buf,15);
	if(mcmh_write(32,buf,2,0) != 0)
	{
		if(eeflag==0)
		{
			ee_write(ADDR_CHECK_B32,4,gThisCardSnr);
			ee_write(ADDR_CHECK_B32+4,16,buf32);
		}
		gResponseStatus = CE_WRITEERROR;
	 	return OP_ERR;
	}
	//write b34
	buf[0] = 0x55;
	buf[15] = 0x55;
	for(tint=1;tint<15;tint++) 
	{
		buf[tint] = 0;
	}
	if(mcmh_write(34,buf,2,0) != 0)
	{
		gResponseStatus = CE_WRITEERROR;
	 	return OP_ERR;
	}
	for(temp2=0;temp2<5;temp2++)
	{
		buf[temp2] = 0;
	}
	buf[5]=*((char *)&UseTimes+INT_HIGH);
	buf[6]=*((char *)&UseTimes+INT_LOW);
	if(ChangeDate==0x55)
	{
		ChangeDate=0;
		FirstDay[0]=gTransTime[0];
		FirstDay[1]=gTransTime[1];
		FirstDay[2]=gTransTime[2];
		FirstDay[3]=gTransTime[3];		
	}
	memcpy(buf+7,FirstDay,4);
	buf[11]=gCardStatus;
	buf[12] = (gCardRecPtr+1)%10;
	tint = gCardCnt+1;
	buf[13] = *((char *)&tint+INT_HIGH);
	buf[14] = *((char *)&tint+INT_LOW);
	lrc_gen2(buf,15);
	if(mcmh_write(13,buf,0,0) != 0)
	{
		gResponseStatus = CE_WRITEERROR;
	    return OP_ERR;
	}
	if(SHGetTacStep2()!=0)					   //����TAC
	{
		gResponseStatus = CE_PSAMERROR;
		return OP_ERR;
	}
	memcpy(gTAC,bgTaskKeyOutbuf,4);
	for(temp2=0;temp2<16;temp2++)
	{
		buf[temp2] = 0;
	}
	if(mcmh_write(12,buf,2,0) != 0)
	{
		gResponseStatus = CE_PUTBACKCARD1;
	    return OP_ERR;										
	}
	for(temp2=0;temp2<11;temp2++)
	{
		save[temp2]=0;	   
	}
	return 0;
}
/****************************************************************
������trans_exit
���ܣ�������վ����
****************************************************************/
#define M1_debug
UBYTE trans_exit(UBYTE offsetnum)
{
	UWORD tint;
	UBYTE buf[20];
	UBYTE temp;
	UDWORD temp2,tint2;
	#ifdef _DEBUG_SHTRANS_
			debug_printf("\x0d\x0a in function trans_exit");
	#endif
	//�������� 
	save[0]=gThisCardSnr[0];
	save[1]=gThisCardSnr[1];
	save[2]=gThisCardSnr[2];
	save[3]=gThisCardSnr[3];
	save[4]=*((char *)&gCardPurse+LONG_HIGH0); 
	save[5]=*((char *)&gCardPurse+LONG_HIGH1); 
	save[6]=*((char *)&gCardPurse+LONG_HIGH2); 
	save[7]=*((char *)&gCardCnt+INT_HIGH);
	save[8]=*((char *)&gCardCnt+INT_LOW);
	save[9]=gCardRecPtr;
	save[10]=gMetroIoFlag;
	//дB12����
	buf[0] = 0x55;
	buf[1] = 0xaa; 
	buf[2] = 0x55;
	if(NormalCard!=CNTCARD)
	{
		buf[3] = *((char *)&gCardPurse+LONG_HIGH0); 
		buf[4] = *((char *)&gCardPurse+LONG_HIGH1); 
		buf[5] = *((char *)&gCardPurse+LONG_HIGH2); 
		buf[6] = *((char *)&gCardPurse+LONG_HIGH3); 
		buf[8] = *((char *)&gCardCnt+INT_HIGH);
		buf[9] = *((char *)&gCardCnt+INT_LOW);
	}
	else
	{
		if(ChangeDate==0x55)
		{
			buf[3]=gTransTime[0];
			buf[4]=gTransTime[1];
			buf[5]=gTransTime[2];
			buf[6]=gTransTime[3];
		}
		else
		{
			buf[3]=FirstDay[0];
			buf[4]=FirstDay[1];
			buf[5]=FirstDay[2];
			buf[6]=FirstDay[3];
		}
		buf[8] = *((char *)&gCardCnt+INT_HIGH);
		buf[9] = *((char *)&gCardCnt+INT_LOW);
	}
	buf[7] = gCardRecPtr;
	buf[10] = gCardStatus;
	if(NormalCard!=CNTCARD)
	{
		buf[11] = 0;
		buf[12] = 0;
	}
	else
	{	
		buf[11]=*((char *)&UseTimes+INT_HIGH);
		buf[12]=*((char *)&UseTimes+INT_LOW);
	}
	buf[13] = 0;
	buf[14] = gMetroIoFlag;
	lrc_gen2(buf,15);	
	if(mcmh_write(12,buf,0,0) != 0) 
	{
		gResponseStatus = CE_WRITEERROR;
		return OP_ERR;
	}
	
	//����TAC
	KeyBuf[0]=0x88;
	KeyBuf[1] = *((char *)&gPosTransCnt+LONG_HIGH2);
	KeyBuf[2] = *((char *)&gPosTransCnt+LONG_HIGH1);
	KeyBuf[3] = *((char *)&gPosTransCnt+LONG_HIGH0);
	KeyBuf[4] = gCityCode0;
	KeyBuf[5] = gCityCode1;
	KeyBuf[6] = gThisCardSnr[0];
	KeyBuf[7] = gThisCardSnr[1];
	KeyBuf[8] = gThisCardSnr[2];
	KeyBuf[9] = gThisCardSnr[3];
	KeyBuf[10] = gCardType;
	KeyBuf[11] = *((char *)&gCardLastRemain+LONG_HIGH0);
	KeyBuf[12] = *((char *)&gCardLastRemain+LONG_HIGH1);
	KeyBuf[13] = *((char *)&gCardLastRemain+LONG_HIGH2);
	KeyBuf[14] = *((char *)&gCardThisTransValue+LONG_HIGH0);
	KeyBuf[15] = *((char *)&gCardThisTransValue+LONG_HIGH1);
	KeyBuf[16] = *((char *)&gCardThisTransValue+LONG_HIGH2);
	memcpy(&KeyBuf[17],gTransTime,7);
	KeyBuf[24] = *((char *)&gCardCnt+INT_HIGH);
	KeyBuf[25] = *((char *)&gCardCnt+INT_LOW);
	memcpy(&KeyBuf[26],bpgSHpsamNo,4);
	KeyBuf[30] = 0x80;
	KeyBuf[31] = 0x00;
	#ifdef M1_debug
	printf("begintac-1\n");
	#endif
	SHTransTACStep1(KeyBuf);
	#ifdef M1_debug
	printf("begintac-2\n");
	#endif
	
	if(checkrec_inmemory(REC_ENTRY)!=0)
	{
		if(mcmh_read(32,gB32,0,0)!=0)
		{
			gResponseStatus = CE_READERROR;
			return OP_ERR;
		}
		lrc3_gen(gB32,16);
		memcpy(gB32BK,gB32,20);
	}
	memcpy(buf,gB32,16);
	gStationTimeOverRideFlag=buf[0];			//��ͨ��
	*((char *)&tint+INT_LOW) = buf[9];
	*((char *)&tint+INT_HIGH) = buf[10];
	tint2=0;
	*((char *)&tint2+LONG_HIGH0)=buf[10];		//�ƴο�
	*((char *)&tint2+LONG_HIGH1)=buf[11];
	*((char *)&tint2+LONG_HIGH2)=buf[12];
	if(NormalCard==CNTCARD)
	{
		goto timecntpre;
	}
	if(ReadMode==0)					//0:�������Ż�
	{
		goto pv128;					//Э��汾12��8
	}
	//�Ĺ�����¼�� 
	if(mcmh_read(29,buf,0,0)!=0)
	{
		gResponseStatus = CE_READERROR;
		return OP_ERR;
	}
	buf[0]=gPosNo[1];
	buf[1]=gPosNo[3];
	buf[2]=gPosNo[0];
	buf[2]|=0x80;
	buf[3]=gTransTime[2];
	buf[4]=gTransTime[3];
	buf[5]=gTransTime[4];
	buf[6]=gTransTime[5];
	lrc_gen2(buf,15);
	if(mcmh_write(29,buf,2,0) != 0) 
	{
		gResponseStatus = CE_WRITEERROR;
		return OP_ERR;
	}
	memcpy(gB29,buf,16);
pv128:
	gCardThisRemain = gCardLastRemain-gCardThisTransValue;
	if(gCardThisTransValue==0)				//����Ǯ��
	{
		goto cardrec;
	}
	if(mcmh_decrement(5,gCardThisTransValue,0,0) != 0) 
	{
			gResponseStatus = CE_WRITEERROR;
			return OP_ERR;
	}
	if(mcmh_transfer(5,2,0) != 0) 
	{
			gResponseStatus = CE_WRITEERROR;
		    return OP_ERR;
	}
cardrec:
	//�������Ͻ��׼�¼	  
	if(ReadMode==0)
	{
		if(offsetnum<3)
			trans_gen_card_record(trans_id128[offsetnum],buf);
		else
			trans_gen_card_record(0x11,buf);	
		goto wr_record;
	}
	if(offsetnum<13)
		trans_gen_card_record(trans_id[offsetnum],buf);
	else
		trans_gen_card_record(0x11,buf);
wr_record:
	if(mcmh_write(gCardRecPos[gCardRecPtr%10],buf,2,0) != 0) 
	{
			gResponseStatus = CE_WRITEERROR;
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
	
	if(ReadMode==0)
	{
		buf[0]=0x81;
		goto   setb33;
	}
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
setb33:
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
		tint=tint+(UWORD)gCardThisTransValue;
		buf[9]=*((char *)&tint+INT_LOW);
		buf[10]=*((char *)&tint+INT_HIGH);
	}
	else
	{
		buf[9]  = *((char *)&gCardThisTransValue+LONG_HIGH0);
		buf[10] = *((char *)&gCardThisTransValue+LONG_HIGH1);
	}
	lrc_gen2(buf,15);
	if(mcmh_write(33,buf,0,0) != 0) 
	{
		gResponseStatus = CE_WRITEERROR;
		return OP_ERR;
	}	
	//write b34
	buf[0] = 0x66;
	buf[15] = 0x66;
	for(temp=1;temp<15;temp++) buf[temp] = 0;
	if(mcmh_write(34,buf,2,0) != 0)
	{
		gResponseStatus = CE_WRITEERROR;
		return OP_ERR;
	}
	memset(buf,0,16);
	buf[11] = gCardStatus;
	buf[12] = (gCardRecPtr+1)%10;
	tint = gCardCnt+1;
	buf[13] = *((char *)&tint+INT_HIGH);
	buf[14] = *((char *)&tint+INT_LOW);
	lrc_gen2(buf,15);
	if(mcmh_write(13,buf,0,0) != 0) 
	{
			gResponseStatus = CE_WRITEERROR;
			return OP_ERR;
	}
	
	
	if(SHGetTacStep2()!=0)					   //����TAC
	{	 
			#ifdef M1_debug
	printf("begintac-3\n");
	#endif
		gResponseStatus = CE_PSAMERROR;
		return OP_ERR;
			#ifdef M1_debug
	printf("begintac-4\n");
	#endif
	} 
	memcpy(gTAC,bgTaskKeyOutbuf,4);
	memset(buf,0,16);
	if(mcmh_write(12,buf,2,0) != 0)
	{
	   	gResponseStatus = CE_PUTBACKCARD1;
	    return OP_ERR;
    }
	for(temp=0;temp<11;temp++)
	{
		save[temp]=0;	   
	}
	//����B6
	if(gCardThisTransValue != 0)
	{
  		mcmh_decrement(6,gCardThisTransValue,0,0);
  		mcmh_transfer(6,2,0);
	}
	if(ReadMode!=0)
	{
		mcmh_write(30,gB29,0,0);	
	}
	return 0;
timecntpre:
	//ִ�пۿ�
	gCardThisRemain = gCardLastRemain-gCardThisTransValue;
	if(gCardThisTransValue==0)
	{
		goto  reccnt;
	}if(mcmh_decrement(5,gCardThisTransValue,0,0) != 0)
	{
		gResponseStatus = CE_WRITEERROR;
	 	return OP_ERR;
	}
	if(mcmh_transfer(5,2,0) != 0) 
	{
		gResponseStatus = CE_WRITEERROR;
	 	return OP_ERR;
	}
reccnt:
	trans_gen_card_record(0x11,buf);
	if(mcmh_write(gCardRecPos[gCardRecPtr],buf,0,0) != 0) 
	{
		gResponseStatus = CE_WRITEERROR;
	    return OP_ERR;
	}
	//дB33
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
	memcpy(buf+1,gTransTime+1,5);
	buf[6] = gPosNo[0];
	buf[7] = gPosNo[1];
	buf[8] = gPosNo[2];
	buf[9] = gPosNo[3];
	if(gStationTimeOverRideFlag&0x10)
	{
		tint2=tint2+gCardThisTransValue;
		buf[10]=*((char *)&tint2+LONG_HIGH0);
		buf[11]=*((char *)&tint2+LONG_HIGH1);
		buf[12]=*((char *)&tint2+LONG_HIGH2);
	}
	else
	{
		buf[10]  = *((char *)&gCardThisTransValue+LONG_HIGH0);
		buf[11]  = *((char *)&gCardThisTransValue+LONG_HIGH1);
		buf[12]  = *((char *)&gCardThisTransValue+LONG_HIGH2);
	}
	buf[13] = *((char *)&gCardThisRemain+LONG_HIGH0);
	buf[14] = *((char *)&gCardThisRemain+LONG_HIGH1);
	lrc_gen2(buf,15);	
	if(mcmh_write(33,buf,0,0) != 0)
	{
		gResponseStatus = CE_WRITEERROR;
	    return OP_ERR;
	}
	//write b34
	buf[0] = 0x66;
	buf[15] = 0x66;
	for(tint=1;tint<15;tint++) buf[tint] = 0;
	if(mcmh_write(34,buf,2,0) != 0)
	{
		gResponseStatus = CE_WRITEERROR;
	    return OP_ERR;
	}
	//write B13
	for(temp2=0;temp2<5;temp2++)
	{
		buf[temp2] = 0;
	}
	UseTimes++;
	buf[5]=*((char *)&UseTimes+INT_HIGH);
	buf[6]=*((char *)&UseTimes+INT_LOW);
	if(ChangeDate==0x55)
	{
		ChangeDate=0;
		FirstDay[0]=gTransTime[0];
		FirstDay[1]=gTransTime[1];
		FirstDay[2]=gTransTime[2];
		FirstDay[3]=gTransTime[3];		
	}
	memcpy(buf+7,FirstDay,4);
	buf[11]=gCardStatus;
	buf[12] = (gCardRecPtr+1)%10;
	tint = gCardCnt+1;
	buf[13] = *((char *)&tint+INT_HIGH);
	buf[14] = *((char *)&tint+INT_LOW);
	lrc_gen2(buf,15);
	if(mcmh_write(13,buf,0,0) != 0)
	{
		gResponseStatus = CE_WRITEERROR;
	    return OP_ERR;
	}
		#ifdef M1_debug
	printf("begintac-5\n");
	#endif
	if(SHGetTacStep2()!=0)					   //ȡTAC
	{	 
		gResponseStatus = CE_PSAMERROR;
		return OP_ERR;
	}
		#ifdef M1_debug
	printf("begintac-6\n");
	#endif
	memcpy(gTAC,bgTaskKeyOutbuf,4);
	memset(buf,0,16);
	if(mcmh_write(12,buf,2,0) != 0)
	{
		gResponseStatus = CE_PUTBACKCARD1;
	    return OP_ERR;										
	}
	for(temp=0;temp<11;temp++)
	{
		save[temp]=0;	   
	}
	if(gCardThisTransValue!=0)
	{
		mcmh_restore(5,2,0);
		mcmh_transfer(6,2,0);
	}
	return 0;
}
/***********************************************************************************
������trans_lock
���ܣ���������
*************************************************************************************/
UBYTE trans_lock(UBYTE lock_type)
{
	UBYTE buf[20];
	UBYTE i;
	UWORD tint;
	
	for(i=0;i<MAX_TRY;i++)
	{
   		if(mcml_request(PICC_REQSTD,buf) == 0) 
   		{
   			goto label_begin;
  		}
  	}
	gResponseStatus = CE_CARDREMOVED;
	return OP_ERR;  
	
label_begin:
	if(gCardStatus != 0x02)
	{
   		gResponseStatus = CE_INVALIDFLAG;
		return OP_ERR;
   	}	
	//�������� 
	save[0]=gThisCardSnr[0];//UID
	save[1]=gThisCardSnr[1];
	save[2]=gThisCardSnr[2];
	save[3]=gThisCardSnr[3];
	save[4]=*((char *)&gCardPurse+LONG_HIGH2); //Ǯ��ֵ
	save[5]=*((char *)&gCardPurse+LONG_HIGH1); 
	save[6]=*((char *)&gCardPurse+LONG_HIGH0); 
	save[7]=*((char *)&gCardCnt+INT_HIGH);
	save[8]=*((char *)&gCardCnt+INT_LOW);
	save[9]=gCardRecPtr;
	save[10]=gMetroIoFlag;
	
	//write B12
	buf[0] = 0x55;
	buf[1] = 0xaa; 
	buf[2] = 0x55;
	buf[3] = *((char *)&gCardPurse+LONG_HIGH0); 
	buf[4] = *((char *)&gCardPurse+LONG_HIGH1); 
	buf[5] = *((char *)&gCardPurse+LONG_HIGH2); 
	buf[6] = *((char *)&gCardPurse+LONG_HIGH3); 
	buf[8] = *((char *)&gCardCnt+INT_HIGH);
	buf[9] = *((char *)&gCardCnt+INT_LOW);
	buf[7] = gCardRecPtr;
	buf[10] = gCardStatus;
	buf[11] = 0;
	buf[12] = 0;
	buf[13] = 0;
	buf[14] = gMetroIoFlag;
	lrc_gen2(buf,15);
	if(mcmh_write(12,buf,0,0) != 0) 
	{
		goto label_err_write;
	}
	//�������ϼ�¼  
	gCardThisRemain = gCardLastRemain;
	gCardThisTransValue = 0;
	trans_gen_card_record(0x12,buf);
	if(mcmh_write(gCardRecPos[gCardRecPtr],buf,0,0) != 0) 
	{
		goto label_err_write;
	}
	//дB13
	for(i=0;i<11;i++) buf[i] = 0;
	buf[11]=0x03;
	buf[12]=(gCardRecPtr+1)%10;
	tint = gCardCnt+1;
	buf[13]=*((char *)&tint+INT_HIGH);
	buf[14]=*((char *)&tint+INT_LOW);
	lrc_gen2(buf,15);
	if(mcmh_write(13,buf,0,0) != 0) 
	{
		goto label_err_write;
	}
	if(lock_type!=0)
	{
		if(mcmh_read(5,buf,0,0) != 0) 
		{
			goto label_err_read;
		}
		if(purse_chk(buf) != 0) 
		{
			goto label_B6;
		}
		*((char *)&gCardPurse1+LONG_HIGH0) = buf[0];
		*((char *)&gCardPurse1+LONG_HIGH1) = buf[1];
		*((char *)&gCardPurse1+LONG_HIGH2) = buf[2];
		*((char *)&gCardPurse1+LONG_HIGH3) = buf[3];

		if(mcmh_decrement(5,(UDWORD)gCardPurse1,2,0)!=0) 
		{
			goto label_err_write;
		}
		if(mcmh_transfer(5,2,0) != 0) 
		{
			goto label_err_write;
		}
		if(mcmh_restore(5,2,0) != 0) 
		{
			goto label_err_read;
		}
		if(mcmh_transfer(6,2,0) != 0) 
		{
			goto label_err_write;
		}
		goto label_record;


	label_B6:
		if(mcmh_read(6,buf,2,0) != 0) 
		{
			goto label_err_read;
		}
		if(purse_chk(buf) != 0) 
		{
			goto label_err_read;
		}
		*((char *)&gCardPurse1+LONG_HIGH0) = buf[0];
		*((char *)&gCardPurse1+LONG_HIGH1) = buf[1];
		*((char *)&gCardPurse1+LONG_HIGH2) = buf[2];
		*((char *)&gCardPurse1+LONG_HIGH3) = buf[3];

		if(mcmh_decrement(6,(UDWORD)gCardPurse1,2,0)!=0) 
		{
			goto label_err_write;
		}
		if(mcmh_transfer(6,2,0) != 0) 
		{
			goto label_err_write;
		}
		if(mcmh_restore(6,2,0) != 0) 
		{
			goto label_err_read;
		}
		if(mcmh_transfer(5,2,0) != 0) 
		{
			goto label_err_write;
		}
	}
	
label_record:
	//�������
	for(i=0;i<16;i++) buf[i]=0;
	if(mcmh_write(12,buf,0,0) != 0)
	{
		gResponseStatus = CE_PUTBACKCARD1;
		return OP_ERR;	
	}
	for(i=0;i<11;i++)
	{
		save[i]=0;
	}
	
	return 0;
	
	label_err_read:
	gResponseStatus = CE_READERROR;
	return OP_ERR;

	label_err_write:
	gResponseStatus = CE_WRITEERROR;
	return OP_ERR;	
	

}
/***********************************************************************************
������trans_readrecord
���ܣ���ѯ����¼
************************************************************************************/
UBYTE trans_readrecord(UBYTE *record_buf)
{
	UBYTE buf[4];
	UBYTE i;
	
	for(i=0;i<MAX_TRY;i++)
	{
   		if(mcml_request(PICC_REQSTD,buf) == 0) 
   		{
   			goto label_begin;
  		}
  	}
	gResponseStatus = CE_CARDREMOVED;
	return OP_ERR;  
	
label_begin:
	i = (gCardRecPtr+10-gRelativePtr)%10;					//����ͨ����gCardRecPtrΪ��ǰ��¼ 
	if(mcmh_read(gCardRecPos[i],record_buf,2,0) != 0)		//���·����У��ƴο�����ͨ���ļ�¼�ṹһ�� 
	{
    	goto label_err_read;
    }
	if((lrc_chk2(record_buf,15) != 0) || (record_buf[0]==0) || ((unsigned char)record_buf[0] == (unsigned char)0xff))
	{
   		gResponseStatus = CE_BADPARAM;
   		return OP_ERR;
   	}	
	return 0;
	
	label_err_read:
	gResponseStatus = CE_READERROR;
	return OP_ERR;
}
/***********************************************************************************
������trans_gen_card_record
���ܣ���������¼
************************************************************************************/
void trans_gen_card_record(UBYTE record_type,UBYTE *gCardRecord)
{
	UWORD tint;
	UBYTE ch;
	
	//write card record
	gCardRecord[0] = record_type;
	//POS����
	gCardRecord[1] = gPosNo[0]; 
	gCardRecord[2] = gPosNo[1];
	gCardRecord[3] = gPosNo[3];
	//���׽��
	gCardRecord[4] = *((char *)&gCardThisTransValue+LONG_HIGH0);
	gCardRecord[5] = *((char *)&gCardThisTransValue+LONG_HIGH1);
	ch = *((char *)&gCardThisTransValue+LONG_HIGH3);
	if(ch > 127) gCardRecord[9] = 0xff;
	else gCardRecord[9] = *((char *)&gCardThisTransValue+LONG_HIGH2);
	
	//���׺�Ǯ�����
	gCardRecord[6] = *((char *)&gCardThisRemain+LONG_HIGH0);
	gCardRecord[7] = *((char *)&gCardThisRemain+LONG_HIGH1);
	ch = *((char *)&gCardThisRemain+LONG_HIGH3);
	if(ch > 127) gCardRecord[8] = 0xff;
	else gCardRecord[8] = *((char *)&gCardThisRemain+LONG_HIGH2);
	//����ʱ��
	tint = ((gTransTime[1]/16)*10+(gTransTime[1]%16))+2000;
	tint *= 16;
	tint += ((gTransTime[2]/16)*10+(gTransTime[2]%16));	
	gCardRecord[10] = *((char *)&tint+INT_HIGH);
	gCardRecord[11] = *((char *)&tint+INT_LOW);
	tint = ((gTransTime[3]/16)*10+(gTransTime[3]%16))*32*64;
	tint += ((gTransTime[4]/16)*10+(gTransTime[4]%16))*64;
	tint += ((gTransTime[5]/16)*10+(gTransTime[5]%16));
	gCardRecord[12] = *((char *)&tint+INT_HIGH);
	gCardRecord[13] = *((char *)&tint+INT_LOW);
	gCardRecord[14] = ((gTransTime[6]/16)*10+(gTransTime[6]%16))*4;

 
	//lrc_gen2(gCardRecord,15);
	gCardRecord[15] = 0;
	for(ch=0;ch<15;ch++) gCardRecord[15] ^= gCardRecord[ch];
	return;
	
}
/****************************************************************
������trans_decrement
���ܣ��������ۿ��
****************************************************************/
UBYTE trans_decrement(void)
{
	UWORD tint;
	UBYTE buf[20];
	UBYTE temp;
	UDWORD temp2;
	#ifdef _DEBUG_SHTRANS_
			debug_printf("\x0d\x0a in function trans_decrement");
	#endif
	//�������� 
	save[0]=gThisCardSnr[0];
	save[1]=gThisCardSnr[1];
	save[2]=gThisCardSnr[2];
	save[3]=gThisCardSnr[3];
	save[4]=*((char *)&gCardPurse+LONG_HIGH0); 
	save[5]=*((char *)&gCardPurse+LONG_HIGH1); 
	save[6]=*((char *)&gCardPurse+LONG_HIGH2); 
	save[7]=*((char *)&gCardCnt+INT_HIGH);
	save[8]=*((char *)&gCardCnt+INT_LOW);
	save[9]=gCardRecPtr;
	save[10]=gMetroIoFlag;
	
	//����TAC
	KeyBuf[0]=0x88;
	KeyBuf[1] = *((char *)&gPosTransCnt+LONG_HIGH2);
	KeyBuf[2] = *((char *)&gPosTransCnt+LONG_HIGH1);
	KeyBuf[3] = *((char *)&gPosTransCnt+LONG_HIGH0);
	KeyBuf[4] = gCityCode0;
	KeyBuf[5] = gCityCode1;
	KeyBuf[6] = gThisCardSnr[0];
	KeyBuf[7] = gThisCardSnr[1];
	KeyBuf[8] = gThisCardSnr[2];
	KeyBuf[9] = gThisCardSnr[3];
	KeyBuf[10] = gCardType;
	KeyBuf[11] = *((char *)&gCardLastRemain+LONG_HIGH0);
	KeyBuf[12] = *((char *)&gCardLastRemain+LONG_HIGH1);
	KeyBuf[13] = *((char *)&gCardLastRemain+LONG_HIGH2);
	KeyBuf[14] = *((char *)&gCardThisTransValue+LONG_HIGH0);
	KeyBuf[15] = *((char *)&gCardThisTransValue+LONG_HIGH1);
	KeyBuf[16] = *((char *)&gCardThisTransValue+LONG_HIGH2);
	memcpy(&KeyBuf[17],gTransTime,7);
	KeyBuf[24] = *((char *)&gCardCnt+INT_HIGH);
	KeyBuf[25] = *((char *)&gCardCnt+INT_LOW);
	memcpy(&KeyBuf[26],bpgSHpsamNo,4);
	KeyBuf[30] = 0x80;
	KeyBuf[31] = 0x00;
	SHTransTACStep1(KeyBuf);
	
	//дB12����
	buf[0] = 0x55;
	buf[1] = 0xaa; 
	buf[2] = 0x55;
	if(NormalCard!=CNTCARD)
	{
		buf[3] = *((char *)&gCardPurse+LONG_HIGH0); 
		buf[4] = *((char *)&gCardPurse+LONG_HIGH1); 
		buf[5] = *((char *)&gCardPurse+LONG_HIGH2); 
		buf[6] = *((char *)&gCardPurse+LONG_HIGH3); 
		buf[8] = *((char *)&gCardCnt+INT_HIGH);
		buf[9] = *((char *)&gCardCnt+INT_LOW);
	}
	else
	{
		if(ChangeDate==0x55)
		{
			buf[3]=gTransTime[0];
			buf[4]=gTransTime[1];
			buf[5]=gTransTime[2];
			buf[6]=gTransTime[3];
		}
		else
		{
			buf[3]=FirstDay[0];
			buf[4]=FirstDay[1];
			buf[5]=FirstDay[2];
			buf[6]=FirstDay[3];
		}
		buf[8] = *((char *)&gCardCnt+INT_HIGH);
		buf[9] = *((char *)&gCardCnt+INT_LOW);
	}
	buf[7] = gCardRecPtr;
	buf[10] = gCardStatus;
	if(NormalCard!=CNTCARD)
	{
		buf[11] = 0;
		buf[12] = 0;
	}
	else
	{	
		buf[11]=*((char *)&UseTimes+INT_HIGH);
		buf[12]=*((char *)&UseTimes+INT_LOW);
	}
	buf[13] = 0;
	buf[14] = gMetroIoFlag;
	lrc_gen2(buf,15);	
	if(mcmh_write(12,buf,0,0) != 0) 
	{
		gResponseStatus = CE_WRITEERROR;
		return OP_ERR;
	}
	
	
	
//	if(checkrec_inmemory(REC_ENTRY)!=0)
//	{
//		if(mcmh_read(32,gB32,0,0)!=0)
//		{
//			gResponseStatus = CE_READERROR;
//			return OP_ERR;
//		}
//		lrc3_gen(gB32,16);
//		memcpy(gB32BK,gB32,20);
//	}
//	memcpy(buf,gB32,16);
//	gStationTimeOverRideFlag=buf[0];			//��ͨ��
//	*((char *)&tint+INT_LOW) = buf[9];
//	*((char *)&tint+INT_HIGH) = buf[10];
//	tint2=0;
//	*((char *)&tint2+LONG_HIGH0)=buf[10];		//�ƴο�
//	*((char *)&tint2+LONG_HIGH1)=buf[11];
//	*((char *)&tint2+LONG_HIGH2)=buf[12];
	if(NormalCard==CNTCARD)
	{
		goto timecntpre;
	}
//	if(ReadMode==0)					//0:�������Ż�
//	{
//		goto pv128;					//Э��汾12��8
//	}
//	//�Ĺ�����¼�� 
//	if(mcmh_read(29,buf,0,0)!=0)
//	{
//		gResponseStatus = CE_READERROR;
//		return OP_ERR;
//	}
//	buf[0]=gPosNo[1];
//	buf[1]=gPosNo[3];
//	buf[2]=gPosNo[0];
//	buf[2]|=0x80;
//	buf[3]=gTransTime[2];
//	buf[4]=gTransTime[3];
//	buf[5]=gTransTime[4];
//	buf[6]=gTransTime[5];
//	lrc_gen2(buf,15);
//	if(mcmh_write(29,buf,2,0) != 0) 
//	{
//		gResponseStatus = CE_WRITEERROR;
//		return OP_ERR;
//	}
//	memcpy(gB29,buf,16);
//pv128:
	gCardThisRemain = gCardLastRemain-gCardThisTransValue;
	if(gCardThisTransValue==0)				//����Ǯ��
	{
		goto cardrec;
	}
	if(mcmh_decrement(5,gCardThisTransValue,0,0) != 0) 
	{
			gResponseStatus = CE_WRITEERROR;
			return OP_ERR;
	}
	if(mcmh_transfer(5,2,0) != 0) 
	{
			gResponseStatus = CE_WRITEERROR;
		    return OP_ERR;
	}
cardrec:
	//�������Ͻ��׼�¼	  
//	if(ReadMode==0)
//	{
//		if(offsetnum<3)
//			trans_gen_card_record(trans_id128[offsetnum],buf);
//		else
//			trans_gen_card_record(0x11,buf);	
//		goto wr_record;
//	}
//	if(offsetnum<13)
//		trans_gen_card_record(trans_id[offsetnum],buf);
//	else
		trans_gen_card_record(0x11,buf);
//wr_record:
	if(mcmh_write(gCardRecPos[gCardRecPtr%10],buf,2,0) != 0) 
	{
			gResponseStatus = CE_WRITEERROR;
			return OP_ERR;
	} 		
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
//	if(ReadMode==0)
//	{
//		buf[0]=0x81;
//		goto   setb33;
//	}
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
//setb33:
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
//	if(mcmh_write(33,buf,0,0) != 0) 
//	{
//		gResponseStatus = CE_WRITEERROR;
//		return OP_ERR;
//	}	
//	//write b34
//	buf[0] = 0x66;
//	buf[15] = 0x66;
//	for(temp=1;temp<15;temp++) buf[temp] = 0;
//	if(mcmh_write(34,buf,2,0) != 0)
//	{
//		gResponseStatus = CE_WRITEERROR;
//		return OP_ERR;
//	}
	memset(buf,0,16);
	buf[11] = gCardStatus;
	buf[12] = (gCardRecPtr+1)%10;
	tint = gCardCnt+1;
	buf[13] = *((char *)&tint+INT_HIGH);
	buf[14] = *((char *)&tint+INT_LOW);
	lrc_gen2(buf,15);
	if(mcmh_write(13,buf,0,0) != 0) 
	{
			gResponseStatus = CE_WRITEERROR;
			return OP_ERR;
	}
	if(SHGetTacStep2()!=0)					   //����TAC
	{	 
		gResponseStatus = CE_PSAMERROR;
		return OP_ERR;
	} 
	memcpy(gTAC,bgTaskKeyOutbuf,4);
	memset(buf,0,16);
	if(mcmh_write(12,buf,2,0) != 0)
	{
	   	gResponseStatus = CE_PUTBACKCARD1;
	    return OP_ERR;
    }
	for(temp=0;temp<11;temp++)
	{
		save[temp]=0;	   
	}
	//����B6
	if(gCardThisTransValue != 0)
	{
  		mcmh_decrement(6,gCardThisTransValue,0,0);
  		mcmh_transfer(6,2,0);
	}
//	if(ReadMode!=0)
//	{
//		mcmh_write(30,gB29,0,0);	
//	}
	return 0;
timecntpre:
	//ִ�пۿ�
	gCardThisRemain = gCardLastRemain-gCardThisTransValue;
	if(gCardThisTransValue==0)
	{
		goto  reccnt;
	}
	if(mcmh_decrement(5,gCardThisTransValue,0,0) != 0)
	{
		gResponseStatus = CE_WRITEERROR;
	 	return OP_ERR;
	}
	if(mcmh_transfer(5,2,0) != 0) 
	{
		gResponseStatus = CE_WRITEERROR;
	 	return OP_ERR;
	}
reccnt:
	trans_gen_card_record(0x11,buf);
	if(mcmh_write(gCardRecPos[gCardRecPtr],buf,0,0) != 0) 
	{
		gResponseStatus = CE_WRITEERROR;
	    return OP_ERR;
	}
//	//дB33
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
//	memcpy(buf+1,gTransTime+1,5);
//	buf[6] = gPosNo[0];
//	buf[7] = gPosNo[1];
//	buf[8] = gPosNo[2];
//	buf[9] = gPosNo[3];
//	if(gStationTimeOverRideFlag&0x10)
//	{
//		tint2=tint2+gCardThisTransValue;
//		buf[10]=*((char *)&tint2+LONG_HIGH0);
//		buf[11]=*((char *)&tint2+LONG_HIGH1);
//		buf[12]=*((char *)&tint2+LONG_HIGH2);
//	}
//	else
//	{
//		buf[10]  = *((char *)&gCardThisTransValue+LONG_HIGH0);
//		buf[11]  = *((char *)&gCardThisTransValue+LONG_HIGH1);
//		buf[12]  = *((char *)&gCardThisTransValue+LONG_HIGH2);
//	}
//	buf[13] = *((char *)&gCardThisRemain+LONG_HIGH0);
//	buf[14] = *((char *)&gCardThisRemain+LONG_HIGH1);
//	lrc_gen2(buf,15);	
//	if(mcmh_write(33,buf,0,0) != 0)
//	{
//		gResponseStatus = CE_WRITEERROR;
//	    return OP_ERR;
//	}
//	//write b34
//	buf[0] = 0x66;
//	buf[15] = 0x66;
//	for(tint=1;tint<15;tint++) buf[tint] = 0;
//	if(mcmh_write(34,buf,2,0) != 0)
//	{
//		gResponseStatus = CE_WRITEERROR;
//	    return OP_ERR;
//	}
	//write B13
	for(temp2=0;temp2<5;temp2++)
	{
		buf[temp2] = 0;
	}
	UseTimes++;
	buf[5]=*((char *)&UseTimes+INT_HIGH);
	buf[6]=*((char *)&UseTimes+INT_LOW);
	if(ChangeDate==0x55)
	{
		ChangeDate=0;
		FirstDay[0]=gTransTime[0];
		FirstDay[1]=gTransTime[1];
		FirstDay[2]=gTransTime[2];
		FirstDay[3]=gTransTime[3];		
	}
	memcpy(buf+7,FirstDay,4);
	buf[11]=gCardStatus;
	buf[12] = (gCardRecPtr+1)%10;
	tint = gCardCnt+1;
	buf[13] = *((char *)&tint+INT_HIGH);
	buf[14] = *((char *)&tint+INT_LOW);
	lrc_gen2(buf,15);
	if(mcmh_write(13,buf,0,0) != 0)
	{
		gResponseStatus = CE_WRITEERROR;
	    return OP_ERR;
	}
	if(SHGetTacStep2()!=0)					   //ȡTAC
	{	 
		gResponseStatus = CE_PSAMERROR;
		return OP_ERR;
	}
	memcpy(gTAC,bgTaskKeyOutbuf,4);
	memset(buf,0,16);
	if(mcmh_write(12,buf,2,0) != 0)
	{
		gResponseStatus = CE_PUTBACKCARD1;
	    return OP_ERR;										
	}
	for(temp=0;temp<11;temp++)
	{
		save[temp]=0;	   
	}
	if(gCardThisTransValue!=0)
	{
		mcmh_restore(5,2,0);
		mcmh_transfer(6,2,0);
	}
	return 0;
}
/****************************************************************
������trans_update
���ܣ����������´���
****************************************************************/
UBYTE trans_update(UBYTE update_type,UBYTE *rec_buf)
{
	UWORD tint;
	UBYTE buf[20];
	UBYTE temp;
	UDWORD temp2;
	#ifdef _DEBUG_SHTRANS_
			debug_printf("\x0d\x0a in function trans_decrement");
	#endif
	//�������� 
	save[0]=gThisCardSnr[0];
	save[1]=gThisCardSnr[1];
	save[2]=gThisCardSnr[2];
	save[3]=gThisCardSnr[3];
	save[4]=*((char *)&gCardPurse+LONG_HIGH0); 
	save[5]=*((char *)&gCardPurse+LONG_HIGH1); 
	save[6]=*((char *)&gCardPurse+LONG_HIGH2); 
	save[7]=*((char *)&gCardCnt+INT_HIGH);
	save[8]=*((char *)&gCardCnt+INT_LOW);
	save[9]=gCardRecPtr;
	save[10]=gMetroIoFlag;
	
	//����TAC
	KeyBuf[0]=0x88;
	KeyBuf[1] = *((char *)&gPosTransCnt+LONG_HIGH2);
	KeyBuf[2] = *((char *)&gPosTransCnt+LONG_HIGH1);
	KeyBuf[3] = *((char *)&gPosTransCnt+LONG_HIGH0);
	KeyBuf[4] = gCityCode0;
	KeyBuf[5] = gCityCode1;
	KeyBuf[6] = gThisCardSnr[0];
	KeyBuf[7] = gThisCardSnr[1];
	KeyBuf[8] = gThisCardSnr[2];
	KeyBuf[9] = gThisCardSnr[3];
	KeyBuf[10] = gCardType;
	KeyBuf[11] = *((char *)&gCardLastRemain+LONG_HIGH0);
	KeyBuf[12] = *((char *)&gCardLastRemain+LONG_HIGH1);
	KeyBuf[13] = *((char *)&gCardLastRemain+LONG_HIGH2);
	KeyBuf[14] = *((char *)&gCardThisTransValue+LONG_HIGH0);
	KeyBuf[15] = *((char *)&gCardThisTransValue+LONG_HIGH1);
	KeyBuf[16] = *((char *)&gCardThisTransValue+LONG_HIGH2);
	memcpy(&KeyBuf[17],gTransTime,7);
	KeyBuf[24] = *((char *)&gCardCnt+INT_HIGH);
	KeyBuf[25] = *((char *)&gCardCnt+INT_LOW);
	memcpy(&KeyBuf[26],bpgSHpsamNo,4);
	KeyBuf[30] = 0x80;
	KeyBuf[31] = 0x00;
	SHTransTACStep1(KeyBuf);
	
	//дB12����
	buf[0] = 0x55;
	buf[1] = 0xaa; 
	buf[2] = 0x55;
	if(NormalCard!=CNTCARD)
	{
		buf[3] = *((char *)&gCardPurse+LONG_HIGH0); 
		buf[4] = *((char *)&gCardPurse+LONG_HIGH1); 
		buf[5] = *((char *)&gCardPurse+LONG_HIGH2); 
		buf[6] = *((char *)&gCardPurse+LONG_HIGH3); 
		buf[8] = *((char *)&gCardCnt+INT_HIGH);
		buf[9] = *((char *)&gCardCnt+INT_LOW);
	}
	else
	{
		if(ChangeDate==0x55)
		{
			buf[3]=gTransTime[0];
			buf[4]=gTransTime[1];
			buf[5]=gTransTime[2];
			buf[6]=gTransTime[3];
		}
		else
		{
			buf[3]=FirstDay[0];
			buf[4]=FirstDay[1];
			buf[5]=FirstDay[2];
			buf[6]=FirstDay[3];
		}
		buf[8] = *((char *)&gCardCnt+INT_HIGH);
		buf[9] = *((char *)&gCardCnt+INT_LOW);
	}
	buf[7] = gCardRecPtr;
	buf[10] = gCardStatus;
	if(NormalCard!=CNTCARD)
	{
		buf[11] = 0;
		buf[12] = 0;
	}
	else
	{	
		buf[11]=*((char *)&UseTimes+INT_HIGH);
		buf[12]=*((char *)&UseTimes+INT_LOW);
	}
	buf[13] = 0;
	buf[14] = gMetroIoFlag;
	lrc_gen2(buf,15);	
	if(mcmh_write(12,buf,0,0) != 0) 
	{
		gResponseStatus = CE_WRITEERROR;
		return OP_ERR;
	}
	
	if(NormalCard==CNTCARD)
	{
		goto timecntpre;
	}

	gCardThisRemain = gCardLastRemain-gCardThisTransValue;
	if(gCardThisTransValue==0)				//����Ǯ��
	{
		goto cardrec;
	}
	if(mcmh_decrement(5,gCardThisTransValue,0,0) != 0) 
	{
			gResponseStatus = CE_WRITEERROR;
			return OP_ERR;
	}
	if(mcmh_transfer(5,2,0) != 0) 
	{
			gResponseStatus = CE_WRITEERROR;
		    return OP_ERR;
	}
cardrec:
	//�������Ͻ��׼�¼	  
	trans_gen_card_record(0x14,buf);
	if(mcmh_write(gCardRecPos[gCardRecPtr%10],buf,2,0) != 0) 
	{
			gResponseStatus = CE_WRITEERROR;
			return OP_ERR;
	} 		
	//���¿��ϵ���ר����������	
	if(update_type==1)
	{
		if(mcmh_write(32,rec_buf,2,0) != 0) 
		{
				gResponseStatus = CE_WRITEERROR;
				return OP_ERR;
		} 
		memset(buf,0,16);
		buf[0] = 0x55;
		buf[15] = 0x55;
		if(mcmh_write(34,buf,2,0) != 0) 
		{
				gResponseStatus = CE_WRITEERROR;
				return OP_ERR;
		}
	}
	else
	{
		if(mcmh_write(33,rec_buf,2,0) != 0) 
		{
				gResponseStatus = CE_WRITEERROR;
				return OP_ERR;
		} 
		memset(buf,0,16);
		buf[0] = 0x66;
		buf[15] = 0x66;
		if(mcmh_write(34,buf,2,0) != 0) 
		{
				gResponseStatus = CE_WRITEERROR;
				return OP_ERR;
		}
	}
	//дB13
	memset(buf,0,16);
	buf[11] = gCardStatus;
	buf[12] = (gCardRecPtr+1)%10;
	tint = gCardCnt+1;
	buf[13] = *((char *)&tint+INT_HIGH);
	buf[14] = *((char *)&tint+INT_LOW);
	lrc_gen2(buf,15);
	if(mcmh_write(13,buf,0,0) != 0) 
	{
			gResponseStatus = CE_WRITEERROR;
			return OP_ERR;
	}
	if(SHGetTacStep2()!=0)					   //����TAC
	{	 
		gResponseStatus = CE_PSAMERROR;
		return OP_ERR;
	} 
	memcpy(gTAC,bgTaskKeyOutbuf,4);
	memset(buf,0,16);
	if(mcmh_write(12,buf,2,0) != 0)
	{
	   	gResponseStatus = CE_PUTBACKCARD1;
	    return OP_ERR;
    }
	for(temp=0;temp<11;temp++)
	{
		save[temp]=0;	   
	}
	//����B6
	if(gCardThisTransValue != 0)
	{
  		mcmh_decrement(6,gCardThisTransValue,0,0);
  		mcmh_transfer(6,2,0);
	}
	return 0;
timecntpre:
	//ִ�пۿ�
	gCardThisRemain = gCardLastRemain-gCardThisTransValue;
	if(gCardThisTransValue==0)
	{
		goto  reccnt;
	}
	if(mcmh_decrement(5,gCardThisTransValue,0,0) != 0)
	{
		gResponseStatus = CE_WRITEERROR;
	 	return OP_ERR;
	}
	if(mcmh_transfer(5,2,0) != 0) 
	{
		gResponseStatus = CE_WRITEERROR;
	 	return OP_ERR;
	}
reccnt:
	trans_gen_card_record(0x65,buf);
	if(mcmh_write(gCardRecPos[gCardRecPtr],buf,0,0) != 0) 
	{
		gResponseStatus = CE_WRITEERROR;
	    return OP_ERR;
	}
	//���¿��ϵ���ר����������	
	if(update_type==1)
	{
		if(mcmh_write(32,rec_buf,2,0) != 0) 
		{
				gResponseStatus = CE_WRITEERROR;
				return OP_ERR;
		} 
		memset(buf,0,16);
		buf[0] = 0x55;
		buf[15] = 0x55;
		if(mcmh_write(34,buf,2,0) != 0) 
		{
				gResponseStatus = CE_WRITEERROR;
				return OP_ERR;
		}
	}
	else
	{
		if(mcmh_write(33,rec_buf,2,0) != 0) 
		{
				gResponseStatus = CE_WRITEERROR;
				return OP_ERR;
		} 
		memset(buf,0,16);
		buf[0] = 0x66;
		buf[15] = 0x66;
		if(mcmh_write(34,buf,2,0) != 0) 
		{
				gResponseStatus = CE_WRITEERROR;
				return OP_ERR;
		}
	}

	//write B13
	for(temp2=0;temp2<5;temp2++)
	{
		buf[temp2] = 0;
	}
	UseTimes++;
	buf[5]=*((char *)&UseTimes+INT_HIGH);
	buf[6]=*((char *)&UseTimes+INT_LOW);
	if(ChangeDate==0x55)
	{
		ChangeDate=0;
		FirstDay[0]=gTransTime[0];
		FirstDay[1]=gTransTime[1];
		FirstDay[2]=gTransTime[2];
		FirstDay[3]=gTransTime[3];		
	}
	memcpy(buf+7,FirstDay,4);
	buf[11]=gCardStatus;
	buf[12] = (gCardRecPtr+1)%10;
	tint = gCardCnt+1;
	buf[13] = *((char *)&tint+INT_HIGH);
	buf[14] = *((char *)&tint+INT_LOW);
	lrc_gen2(buf,15);
	if(mcmh_write(13,buf,0,0) != 0)
	{
		gResponseStatus = CE_WRITEERROR;
	    return OP_ERR;
	}
	if(SHGetTacStep2()!=0)					   //ȡTAC
	{	 
		gResponseStatus = CE_PSAMERROR;
		return OP_ERR;
	}
	memcpy(gTAC,bgTaskKeyOutbuf,4);
	memset(buf,0,16);
	if(mcmh_write(12,buf,2,0) != 0)
	{
		gResponseStatus = CE_PUTBACKCARD1;
	    return OP_ERR;										
	}
	for(temp=0;temp<11;temp++)
	{
		save[temp]=0;	   
	}
	if(gCardThisTransValue!=0)
	{
		mcmh_restore(5,2,0);
		mcmh_transfer(6,2,0);
	}
	return 0;
}
/****************************************************************************
������char hh_getcardinf
���ܣ��Ϻ�����Ʊ����			 
****************************************************************************/
UBYTE hh_getcardinf(UBYTE ID)
{
	UBYTE i,ret;
	UBYTE buf[20];
	
	gDebugStep=0;
	gResponseStatus =0;
	gRestoreFlag = 0;
	gClrFlag = 0x55;
#ifdef _DEBUG_SHTRANS_
				debug_printf("\n hh_getcardinf\n");
#endif
	if(memcmp(bgSmpsamADF, "\x10\x01", 2) != 0)
	{
		ret = select_1001or1002_app(1);
		if(ret==0)bpgSmpsamAppSjtFlag = 1;
	}
#ifdef DEBUG_TIME
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x03", 3);
#endif	
	if(ID==0)										//�������Ϳ�
	{
		hh_key[4]=0x00;
		hh_key[5]=0x00;	
	}

	if(ID==1)										//���籡�Ϳ�
	{
		hh_key[4]=0xff;
		hh_key[5]=0xff;
	} 
	gDebugStep = 0x01;
	
	if(tocken_read(1,gThisCardSnr)!=0)
	//if(tocken_read(1,buf)!=0)
	{
		#ifdef _debug_test_
		debug_printf("\n tocken_read 1 err\n");
		#endif
		goto label_err_read;
	}
#ifdef _READ_BLOCK_
	debug_printf("\ntocken_read block 1: %02X %02X %02X %02X",gThisCardSnr[0],gThisCardSnr[1],gThisCardSnr[2],gThisCardSnr[3]);
#endif	
#ifdef DEBUG_TIME
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x04", 3);
#endif	

	gDebugStep = 0x02;
	if(token_mcml_read(0,buf)!=0)
	{
		goto label_err_read;
	}
#ifdef _READ_BLOCK_
	debug_printf("\ntocken_read block 0: %02X %02X %02X %02X",buf[0],buf[1],buf[2],buf[3]);
#endif	
#ifdef DEBUG_TIME
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x05", 3);
#endif	

	gDebugStep=0x03;
	hh_mid[0]=buf[2];
	hh_mid[1]=buf[3];
	if(tocken_read(3,buf)!=0)
	{
		goto label_err_read; 
	}
#ifdef _READ_BLOCK_
	debug_printf("\ntocken_read block 3: %02X %02X %02X %02X",buf[0],buf[1],buf[2],buf[3]);
#endif	
#ifdef DEBUG_TIME
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x06", 3);
#endif

	gDebugStep = 0x04;
	KeyBuf[6] = buf[1];
	KeyBuf[7] = buf[2];
	KeyBuf[12]= buf[0];
	KeyBuf[13]= buf[3];
	
	if(tocken_read(2,buf)!=0)
	{
		goto label_err_read;
	}
#ifdef _READ_BLOCK_	
	debug_printf("\ntocken_read block 2: %02X %02X %02X %02X",buf[0],buf[1],buf[2],buf[3]);
#endif
	gDebugStep=0x05;
	KeyBuf[0]=buf[0];
	KeyBuf[1]=buf[1];
	memcpy(&KeyBuf[2],gThisCardSnr,4);
	
	if(tocken_read(4,buf)!=0)
	{
		goto label_err_read;
	}
	gDebugStep = 0x06;
	memcpy(&KeyBuf[8],buf,4);
	
	if(tocken_read(5,buf)!=0)
	{
		goto label_err_read;
	}
#ifdef _READ_BLOCK_
	debug_printf("\ntocken_read block 5: %02X %02X %02X %02X",buf[0],buf[1],buf[2],buf[3]);
#endif

#ifdef DEBUG_TIME
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x07", 3);
#endif	
	gDebugStep = 0x07;
	if(buf[3]>=140)
	{
		KeyBuf[6] = buf[3];
	}
	//������Կ
	#ifdef _DEBUG_SHTRANS_
				debug_printf("\n START CAL TOCKEN KEY");
	#endif
	if(SMGetKeyAStep1(KeyBuf,14)!=0)
	{
			goto label_err_key;
	}
	gDebugStep=0x08;
	#ifdef _DEBUG_SHTRANS_
				debug_printf("\n START GET TOCKEN KEY");
	#endif
	if(SMGetKeyAStep2()!=0)
	{
			#ifdef _DEBUG_SHTRANS_
				debug_printf("\x0d\x0a GET TOCKEN KEY ERROR");
			#endif
			goto label_err_key;
	}
#ifdef DEBUG_TIME
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x08", 3);
#endif	
	memcpy(hh_key,bgTaskKeyOutbuf,4);
	gDebugStep=0x09;
	//��֤��Կ
	#ifdef _DEBUG_SHTRANS_
				debug_printf("\n START AUTH TOCKEN KEY,KEY=%02x %02x %02x %02x",hh_key[0],hh_key[1],hh_key[2],hh_key[3]);
	#endif
	//nhh_mcml_load_key(0,0,0,hh_key);
	mcml_load_key(0,0,0,hh_key);
	//if(nhh_mcml_authentication(0,0,0)!=0) 
	if(mcml_authentication(0,0,0)!=0)
	{
			#ifdef _DEBUG_SHTRANS_
				debug_printf("\x0d\x0a TOCKEN KEY AUTH ERROR");
			#endif
			goto label_err_key;
	}
	gDebugStep=0x0a;
	if(tocken_read(9,hh_B9)!=0)
	{
		goto label_err_read;
	}
#ifdef	_READ_BLOCK_
	debug_printf("\ntocken_read block 9: %02X %02X %02X %02X",hh_B9[0],hh_B9[1],hh_B9[2],hh_B9[3]);
#endif
	
	gDebugStep = 0x0b;
	if((lrc_chk2(hh_B9,3)!=0)||((hh_B9[0]==0xff)&&(hh_B9[1]==0xff)&&(hh_B9[2]==0xff)&&(hh_B9[3]==0xff))||((hh_B9[0]==0)&&(hh_B9[1]==0)&&(hh_B9[2]==0)&&(hh_B9[3]==0)))
	{
		//2015/2/3 15:49:04����B9ȫ0�ж�
		b9error=0x55;				//b9error=0x55	 У����� 
		if(tocken_read(15,&buf[4])!=0)
		{
			goto label_err_read;
		}
		#ifdef	_READ_BLOCK_
			debug_printf("\ntocken_read block 15: %02X %02X %02X %02X",buf[4],buf[5],buf[6],buf[7]);
		#endif
		if(lrc_chk2(&buf[4],3)!=0)
		{
			goto label_buneng_shiyong;
		}
		hh_B9[0]=buf[4];
		gCardCnt=0;
		if(tocken_read(5,hh_SaleBuf)!=0)
		{
			goto label_err_read;
		}
		if(tocken_read(6,&hh_SaleBuf[4])!=0)
		{
			goto label_err_read;
		}
		if(tocken_read(7,&hh_SaleBuf[8])!=0)
		{
			goto label_err_read;
		}
		hh_SaleBuf[8]=buf[5];
		hh_SaleBuf[9]=buf[6];
	
	}
	else
	{
		b9error=0;
		*((char *)&gCardCnt+INT_HIGH)=hh_B9[1];
		*((char *)&gCardCnt+INT_LOW)=hh_B9[2];
		//read sale block
		if(tocken_read(5,hh_SaleBuf)!=0)
		{
			goto label_err_read;
		}
		#ifdef	_READ_BLOCK_
			debug_printf("\ntocken_read block 5 hh_SaleBuf[0-3]: %02X %02X %02X %02X",hh_SaleBuf[0],hh_SaleBuf[1],hh_SaleBuf[2],hh_SaleBuf[3]);
		#endif
		if(tocken_read(6,&hh_SaleBuf[4])!=0)
		{
			goto label_err_read;
		}
		#ifdef	_READ_BLOCK_
			debug_printf("\ntocken_read block 6 hh_SaleBuf[4-7]: %02X %02X %02X %02X",hh_SaleBuf[4],hh_SaleBuf[5],hh_SaleBuf[6],hh_SaleBuf[7]);
		#endif
		if(tocken_read(7,&hh_SaleBuf[8])!=0)
		{
			goto label_err_read;	
		}
		#ifdef	_READ_BLOCK_
			debug_printf("\ntocken_read block 7 hh_SaleBuf[8-11]: %02X %02X %02X %02X",hh_SaleBuf[8],hh_SaleBuf[9],hh_SaleBuf[10],hh_SaleBuf[11]);
		#endif
	}
	gDebugStep=0x0c;
	if(((UBYTE)(hh_B9[0])&0x0e)==(UBYTE)0x00)
	{	
		//if(ReadMode==0x80)  //20191009�޸ģ��޸�90ģʽ����ʱ����ʱ�ƴ�Ʊ������վʱ���״�ʹ�õĵڶ���Ʊ������ʱ��᲻��д��������ʱ�仹Ϊ0.
		{
			if(tocken_read(14,hh_Exit+8)!=0)
			{
				goto label_err_read;
			}
			#ifdef	_READ_BLOCK_
				debug_printf("\ntocken_read block 14-1 hh_Exit[8-11]: %02X %02X %02X %02X\n",hh_Exit[8],hh_Exit[9],hh_Exit[10],hh_Exit[11]);
			#endif
		}
		for(i=0;i<4;i++)
		{
			gTransTime[i]=(UBYTE)(hh_SaleBuf[4+i]);
	 	}
	 	hh_trans_Line=(UBYTE)hh_SaleBuf[0];	
		hh_trans_Station=(UBYTE)hh_SaleBuf[1];
		hh_trans_Equip=(UBYTE)hh_SaleBuf[2];
		return 0;
	}
	if(((UBYTE)(hh_B9[0])&0x0e)==(UBYTE)0x02)
	{	
		//if(ReadMode==0x80)  //20191009�޸ģ��޸�90ģʽ����ʱ����ʱ�ƴ�Ʊ������վʱ���״�ʹ�õĵڶ���Ʊ������ʱ��᲻��д��������ʱ�仹Ϊ0.
		{
			if(tocken_read(14,hh_Exit+8)!=0)
			{
				goto label_err_read;
			}
			#ifdef	_READ_BLOCK_
				debug_printf("\ntocken_read block 14-2 hh_Exit[8-11]: %02X %02X %02X %02X\n",hh_Exit[8],hh_Exit[9],hh_Exit[10],hh_Exit[11]);
			#endif
		}
		for(i=0;i<4;i++)
		{
			gTransTime[i]=(UBYTE)(hh_SaleBuf[4+i]);
	 	}
	 	#ifdef	_READ_BLOCK_
			debug_printf("gTransTime: %02X %02X %02X %02X\n",gTransTime[0],gTransTime[1],gTransTime[2],gTransTime[3]);
		#endif
	 	hh_trans_Line=(UBYTE)hh_SaleBuf[0];	
		hh_trans_Station=(UBYTE)hh_SaleBuf[1];
		hh_trans_Equip=(UBYTE)hh_SaleBuf[2];
		return 0;
	}
	if((((UBYTE)(hh_B9[0])&0x0e)==(UBYTE)0x08)||(((UBYTE)(hh_B9[0])&0x0e)==(UBYTE)0x04)||(((UBYTE)(hh_B9[0])&0x0e)==(UBYTE)0x06))
	{
		//read entry block
		if(tocken_read(10,hh_Entry)!=0)
		{
			goto label_err_read;
		}
		if(tocken_read(11,&hh_Entry[4])!=0)
		{
			goto label_err_read;
		}
		if(tocken_read(14,hh_Exit+8)!=0)
		{
			goto label_err_read;
		}
		#ifdef	_READ_BLOCK_
			debug_printf("\ntocken_read block 14-3 hh_Exit[8-11]: %02X %02X %02X %02X\n",hh_Exit[8],hh_Exit[9],hh_Exit[10],hh_Exit[11]);
		#endif
		if(lrc_chk2(hh_Entry,7)!=0)
		{
			goto label_buneng_shiyong;
		}
		for(i=0;i<4;i++)
		{
			gTransTime[i]=(UBYTE)hh_Entry[i];
		}
		hh_trans_Line=(UBYTE)hh_Entry[4];
		hh_trans_Station=(UBYTE)hh_Entry[5];
		hh_trans_Equip=(UBYTE)hh_Entry[6];
		
		return 0;		
	}
	if((((UBYTE)(hh_B9[0])&0x0e)==(UBYTE)0x0c)||(((UBYTE)(hh_B9[0])&0x0e)==(UBYTE)0x0a))
	{
		//read exit block
		if(tocken_read(12,hh_Exit)!=0)
		{
			goto label_err_read;
		}
		if(tocken_read(13,&hh_Exit[4])!=0)
		{
			goto label_err_read;
		}
		if(tocken_read(14,&hh_Exit[8])!=0)
		{
			goto label_err_read;
		}
		#ifdef	_READ_BLOCK_
			debug_printf("\ntocken_read block 14-4 hh_Exit[8-11]: %02X %02X %02X %02X\n",hh_Exit[8],hh_Exit[9],hh_Exit[10],hh_Exit[11]);
		#endif
	  //if(lrc_chk2(hh_Exit,10)!=0)							   //ԭ���򲻿���B14�ĺ������ֽ� 
		if(lrc_chk2(hh_Exit,11)!=0)
		{
			goto label_buneng_shiyong;
		}
		for(i=0;i<4;i++)
		{
			gTransTime[i]=(UBYTE)hh_Exit[i];
		}
		hh_trans_Line=(UBYTE)hh_Exit[4];
		hh_trans_Station=(UBYTE)hh_Exit[5];
		hh_trans_Equip=(UBYTE)hh_Exit[6];
		return 0;	
	}
		
label_buneng_shiyong:
	gResponseStatus =CE_INVALIDCARD;
	gGetCardInfoStep=0xff;
	return OP_ERR;
	
label_err_no_card:
	gGetCardInfoStep=0xff;
	gResponseStatus =CE_CARDREMOVED;
	return OP_ERR;
	
label_err_key:
	gGetCardInfoStep=0xff;
	gResponseStatus =CE_AUTHENERROR;
	return OP_ERR;
	
label_err_read:
	for(i=0;i<MAX_TRY;i++)
	{
	  if(mcml_request(PICC_REQSTD,buf) == 0)
	  	goto label_card_request;
	}
	goto label_err_no_card;
	label_card_request:
	gGetCardInfoStep=0xff;
	gResponseStatus = CE_READERROR;
	return OP_ERR;
}
/****************************************************************************
������hh_trans_sale
���ܣ�����IC����Ʊ			 
****************************************************************************/
UBYTE hh_trans_sale(void)
{
	UBYTE buf[16];
	UWORD i;
	//����TAC
	memcpy(KeyBuf,gThisCardSnr,4);
	memcpy(&KeyBuf[4],gTransTime,4);
	KeyBuf[8] = hh_SaleBuf[8];			//����ǰ���,����ǰ
	KeyBuf[9] = hh_SaleBuf[9];
	KeyBuf[10]= *((char *)&gCardThisTransValue+LONG_HIGH1);		//���׽��,����ǰ
	KeyBuf[11]= *((char *)&gCardThisTransValue+LONG_HIGH0);
	KeyBuf[12]= hh_trans_Line;
	KeyBuf[13]= hh_trans_Station;
	memcpy(&KeyBuf[14],gPosNo,4);
	KeyBuf[16]= 0x13;
	KeyBuf[18]= *((char *)&gPosTransCnt+LONG_HIGH3);
	KeyBuf[19]= *((char *)&gPosTransCnt+LONG_HIGH2);
	KeyBuf[20]= *((char *)&gPosTransCnt+LONG_HIGH1);
	KeyBuf[21]= *((char *)&gPosTransCnt+LONG_HIGH0);
	KeyBuf[22]= *((char *)&gCardCnt+INT_HIGH);
	KeyBuf[23]= *((char *)&gCardCnt+INT_LOW);
	memcpy(&KeyBuf[24],bpgSmpsamNo,4);
	KeyBuf[28]= 0x80;
	KeyBuf[29]= 0x00;
	KeyBuf[30]= 0x00;
	KeyBuf[31]= 0x00;
	SHMetroTACStep1(KeyBuf);
	
	if(b9error==0x55)
	{
		b9error=0;
	}
	else
	{
		buf[0]=hh_B9[0];
		buf[1]=hh_SaleBuf[8];
		buf[2]=hh_SaleBuf[9];
		lrc_gen2(buf,3);
		if(tocken_write(15,buf)!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
	}
	buf[0]=*((char *)&gPosNo);
	buf[1]=*((char *)&gPosNo+1);
	buf[2]=*((char *)&gPosNo+3);
	buf[3]=gCardType;
	if(tocken_write(5,buf)!=0)
	{
		gResponseStatus=CE_WRITEERROR;
		return OP_ERR;
	}
	buf[0]=	gTransTime[0];
	buf[1]=	gTransTime[1];
	buf[2]=	gTransTime[2];
	buf[3]=	gTransTime[3];
	if(tocken_write(6,buf)!=0)
	{
		if(tocken_write(6,buf)!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
	}
	buf[0] = *((char *)&gCardThisTransValue+LONG_HIGH1);
	buf[1] = *((char *)&gCardThisTransValue+LONG_HIGH0);
	buf[2] = hh_mid[0];
	buf[3] = hh_mid[1];
  if(tocken_write(7,buf)!=0)
	{
		gResponseStatus=CE_WRITEERROR;
		return OP_ERR;
	}
	memset(buf,0,16);
	for(i=10;i<15;i++)
	{
		if(tocken_write(i,buf)!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
	}
	if(SMGetTacStep2()!=0)												//ȡTAC
	{
		gResponseStatus = CE_PSAMERROR;
		return OP_ERR;
	}
	memcpy(gTAC,bgTaskKeyOutbuf,4);	
	buf[0]=hh_trans_id;
	i=gCardCnt+1;
	buf[1]=*((char *)&i+INT_HIGH);
	buf[2]=*((char *)&i+INT_LOW);					
	lrc_gen2(buf,3);
	if(tocken_write(9,buf)!=0)
	{							
		gResponseStatus=CE_WRITEERROR;
		return OP_ERR;
	}
	return 0;
}
/****************************************************************************
������hh_trans_entry
���ܣ�����IC����վ			 
****************************************************************************/
UBYTE hh_trans_entry(void)
{
	UBYTE buf[20];
	UWORD temp;
	//
	UBYTE resp_buf[20];
	 
#ifdef DEBUG_TIME
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x09", 3);
#endif	
	//����TAC
	memcpy(KeyBuf,gThisCardSnr,4);
	memcpy(&KeyBuf[4],gTransTime,4);
	KeyBuf[8] = hh_SaleBuf[8];			//����ǰ���,����ǰ
	KeyBuf[9] = hh_SaleBuf[9];
	KeyBuf[10]= 0x00;								//���׽��,����ǰ
	KeyBuf[11]= 0x00;
	KeyBuf[12]= hh_trans_Line;
	KeyBuf[13]= hh_trans_Station;
	memcpy(&KeyBuf[14],gPosNo,4);
	KeyBuf[16]= 0x1a;
	KeyBuf[18]= *((char *)&gPosTransCnt+LONG_HIGH3);
	KeyBuf[19]= *((char *)&gPosTransCnt+LONG_HIGH2);
	KeyBuf[20]= *((char *)&gPosTransCnt+LONG_HIGH1);
	KeyBuf[21]= *((char *)&gPosTransCnt+LONG_HIGH0);
	KeyBuf[22]= *((char *)&gCardCnt+INT_HIGH);
	KeyBuf[23]= *((char *)&gCardCnt+INT_LOW);
	memcpy(&KeyBuf[24],bpgSmpsamNo,4);
	KeyBuf[28]= 0x80;
	KeyBuf[29]= 0x00;
	KeyBuf[30]= 0x00;
	KeyBuf[31]= 0x00;
	SHMetroTACStep1(KeyBuf);
	
#ifdef DEBUG_TIME
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x0a", 3);
#endif	
	//��ʼ����
	if(b9error==0x55)
	{
		b9error=0;
	}
	else
	{
		buf[0]=hh_B9[0];
		buf[1]=hh_SaleBuf[8];
		buf[2]=hh_SaleBuf[9];
		lrc_gen2(buf,3);
		if(tocken_write(15,buf)!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
	}

	memcpy(buf,gTransTime,4);
	buf[4]=gPosNo[0];
	buf[5]=gPosNo[1];
	buf[6]=gPosNo[3];
	lrc_gen2(buf,7);
	if(tocken_write(10,gTransTime)!=0)
		if(tocken_write(10,gTransTime)!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
#ifdef DEBUG_TIME
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x0b", 3);
#endif	
	if(tocken_write(11,&buf[4])!=0)
		if(tocken_write(11,&buf[4])!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}

#ifdef DEBUG_TIME
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x0c", 3);
#endif	
	if(SMGetTacStep2()!=0)												//ȡTAC
	{
		gResponseStatus = CE_PSAMERROR;
		return OP_ERR;
	}
#ifdef DEBUG_TIME
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x0d", 3);
#endif	
	memcpy(gTAC,bgTaskKeyOutbuf,4);				
	buf[0]=hh_trans_id;
	temp=gCardCnt+1;
	buf[1]=*((char *)&temp+INT_HIGH);
	buf[2]=*((char *)&temp+INT_LOW);
	lrc_gen2(buf,3);
	if(tocken_write(9,buf)!=0)
	{							
		gResponseStatus=CE_WRITEERROR;
		return OP_ERR;
	}	
#ifdef DEBUG_TIME
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x0e", 3);
#endif	
	return 0;
}
/****************************************************************************
������hh_trans_entry2
���ܣ����ⱡ��IC����վ			 
****************************************************************************/
UBYTE hh_trans_entry2(UBYTE b14com)
{
	UBYTE buf[20];
	UWORD temp;
	UBYTE	id;
	
	id = hh_trans_id&0x0e;
	printf("\nid = %02x",id);
	//����TAC
	memcpy(KeyBuf,gThisCardSnr,4);
	memcpy(&KeyBuf[4],gTransTime,4);
	KeyBuf[8] = hh_SaleBuf[8];			//����ǰ���,����ǰ
	KeyBuf[9] = hh_SaleBuf[9];
	KeyBuf[10]= 0x00;								//���׽��,����ǰ
	KeyBuf[11]= 0x00;
	KeyBuf[12]= hh_trans_Line;
	KeyBuf[13]= hh_trans_Station;
	memcpy(&KeyBuf[14],gPosNo,4);
	KeyBuf[16]= 0x1a;
	KeyBuf[18]= *((char *)&gPosTransCnt+LONG_HIGH3);
	KeyBuf[19]= *((char *)&gPosTransCnt+LONG_HIGH2);
	KeyBuf[20]= *((char *)&gPosTransCnt+LONG_HIGH1);
	KeyBuf[21]= *((char *)&gPosTransCnt+LONG_HIGH0);
	KeyBuf[22]= *((char *)&gCardCnt+INT_HIGH);
	KeyBuf[23]= *((char *)&gCardCnt+INT_LOW);
	memcpy(&KeyBuf[24],bpgSmpsamNo,4);
	KeyBuf[28]= 0x80;
	KeyBuf[29]= 0x00;
	KeyBuf[30]= 0x00;
	KeyBuf[31]= 0x00;
	SHMetroTACStep1(KeyBuf);
	
	//��ʼ����
	if(b9error==0x55)
	{
		b9error=0;
	}
	else
	{
		buf[0]=hh_B9[0];
		buf[1]=hh_SaleBuf[8];
		buf[2]=hh_SaleBuf[9];
		lrc_gen2(buf,3);
		if(tocken_write(15,buf)!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
	}
	
	memcpy(buf,gTransTime,4);
	if(tocken_read(11,&hh_Entry[4])!=0)
		goto label_err_read;
	if(id==4)
	{
		goto  label_interchangeentry;
/*		if(tocken_read(11,&hh_Entry[4])!=0)
		{
			gResponseStatus=CE_READERROR;
//			gResponseStatus=1;
			return -1;
		}
		buf[4]=hh_Entry[4];
		buf[5]=hh_Entry[5];
		buf[6]=hh_Entry[6];				 */
	}
	if(id==8)
	{
		buf[4]=gPosNo[0];
		buf[5]=gPosNo[1];
		buf[6]=gPosNo[3];
	}
	lrc_gen2(buf,7);
	if(tocken_write(10,gTransTime)!=0)
		if(tocken_write(10,gTransTime)!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
	if(tocken_write(11,&buf[4])!=0)
		if(tocken_write(11,&buf[4])!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
label_interchangeentry:
	if(tocken_read(12,hh_Exit)!=0)
	{
		goto label_err_read;
	}
	if(tocken_read(13,&hh_Exit[4])!=0)
	{	
		goto label_err_read;
	}
	if(tocken_read(14,&hh_Exit[8])!=0)
	{
		goto label_err_read;
	}
#ifdef	_READ_BLOCK_
	printf("\ntocken_read block 14-4 hh_Exit[8-11]: %02X %02X %02X %02X\n",hh_Exit[8],hh_Exit[9],hh_Exit[10],hh_Exit[11]);			
#endif
#ifdef	_READ_BLOCK_
	printf("b14com--1= %02x\n",b14com);
#endif
	if(b14com&0x02)
	{
		if(b14com&0x01)
		{
			 hh_Exit[8]=gTransTime[0];
			 hh_Exit[9]=gTransTime[1];
			 hh_Exit[10]=gTransTime[2];
			 hh_Exit[11]=gTransTime[3];
			 if(tocken_write(14,hh_Exit+8)!=0)
				if(tocken_write(14,hh_Exit+8)!=0)
				{
					gResponseStatus=CE_WRITEERROR;
					return OP_ERR;
				}
		}	
	}
	else
	{
		if(id==0x04)
		{
			*((char *)&temp+INT_HIGH)=hh_Exit[10];
			*((char *)&temp+INT_LOW)=hh_Exit[11];
			temp=temp+(UBYTE)hh_Exit[8]*256+hh_Exit[9];
			hh_Exit[10]=*((char *)&temp+INT_HIGH);
			hh_Exit[11]=*((char *)&temp+INT_LOW);
		}
		if(id==0x08)
		{
			hh_Exit[10]=0;
			hh_Exit[11]=0;
		}
		if(tocken_write(14,hh_Exit+8)!=0)
			if(tocken_write(14,hh_Exit+8)!=0)
			{
				gResponseStatus=CE_WRITEERROR;
				return OP_ERR;
			}
	}
		
	memcpy(buf,hh_Exit,7);
	memcpy(buf+7,hh_Exit+8,4);
	lrc_gen2(buf,11);
	buf[7]=buf[11];
	if(tocken_write(13,buf+4)!=0)
	{
		if(tocken_write(13,buf+4)!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
	}
	memcpy(buf,&hh_SaleBuf[8],4);
	buf[0]=*((char *)&gCardThisRemain+LONG_HIGH1);
	buf[1]=*((char *)&gCardThisRemain+LONG_HIGH0);
	if(tocken_write(7,buf)!=0)
	{
		gResponseStatus=CE_WRITEERROR;
		return OP_ERR;
	}
	if(SMGetTacStep2()!=0)												//ȡTAC
	{
		gResponseStatus = CE_PSAMERROR;
		return OP_ERR;
	}
	memcpy(gTAC,bgTaskKeyOutbuf,4);			
	
	buf[0]=hh_trans_id;
	temp=gCardCnt+1;
	buf[1]=*((char *)&temp+INT_HIGH);
	buf[2]=*((char *)&temp+INT_LOW);
	lrc_gen2(buf,3);
	if(tocken_write(9,buf)!=0)
	{			
		gResponseStatus=CE_WRITEERROR;
		return OP_ERR;
	}
	
	return 0;
label_err_read:
	gResponseStatus = CE_READERROR;
	return OP_ERR;
}
/****************************************************************************
������hh_trans_exit
���ܣ�����IC����վ			 
****************************************************************************/
UBYTE hh_trans_exit(UBYTE wrcom)
{
	UBYTE buf[16];
	UWORD temp;
	#ifdef _debug_test_
	UDWORD	tint0=0,tint1=0;
	#endif
	
	//����TAC
	memcpy(KeyBuf,gThisCardSnr,4);
	memcpy(&KeyBuf[4],gTransTime,4);
	KeyBuf[8] = hh_SaleBuf[8];								//����ǰ���,����ǰ
	KeyBuf[9] = hh_SaleBuf[9];
	KeyBuf[10]= *((char *)&gCardThisTransValue+LONG_HIGH1);					//���׽��,����ǰ
	KeyBuf[11]= *((char *)&gCardThisTransValue+LONG_HIGH0);
	KeyBuf[12]= hh_trans_Line;
	KeyBuf[13]= hh_trans_Station;
	memcpy(&KeyBuf[14],gPosNo,4);
	KeyBuf[16]= 0x15;
	KeyBuf[18]= *((char *)&gPosTransCnt+LONG_HIGH3);
	KeyBuf[19]= *((char *)&gPosTransCnt+LONG_HIGH2);
	KeyBuf[20]= *((char *)&gPosTransCnt+LONG_HIGH1);
	KeyBuf[21]= *((char *)&gPosTransCnt+LONG_HIGH0);
	KeyBuf[22]= *((char *)&gCardCnt+INT_HIGH);
	KeyBuf[23]= *((char *)&gCardCnt+INT_LOW);
	memcpy(&KeyBuf[24],bpgSmpsamNo,4);
	KeyBuf[28]= 0x80;
	KeyBuf[29]= 0x00;
	KeyBuf[30]= 0x00;
	KeyBuf[31]= 0x00;
	SHMetroTACStep1(KeyBuf);

	
	//��ʼ����
	if(b9error==0x55)
	{
		b9error=0;
	}
	else
	{
		buf[0]=hh_B9[0];
		buf[1]=hh_SaleBuf[8];
		buf[2]=hh_SaleBuf[9];
		lrc_gen2(buf,3);
		#ifdef _debug_test_
		tint0 = GetTickcount();
		#endif
		if(tocken_write(15,buf)!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
		#ifdef _debug_test_
		tint1 = GetTickcount();
		debug_printf("\n SHMetroTACStep1(%d) \n",tint1-tint0);
		#endif
	}
	if(tocken_read(14,&hh_Exit[8])!=0)
	{
		gResponseStatus=CE_READERROR;
		return OP_ERR;
	}
	//memcpy(buf,gTransTime,4);
	buf[0]=gTransTime[0];
	buf[1]=gTransTime[1];
	buf[2]=gTransTime[2];
	buf[3]=gTransTime[3];
	buf[4]=gPosNo[0];
	buf[5]=gPosNo[1];
	buf[6]=gPosNo[3];
	if(wrcom&0x01)
	{
		buf[7]=hh_Exit[8];
		buf[8]=hh_Exit[9];
	}
	else
	{
		buf[7]=*((char *)&gCardThisTransValue+LONG_HIGH1);
		buf[8]=*((char *)&gCardThisTransValue+LONG_HIGH0);
	}
	buf[9]=hh_Exit[10];
	buf[10]=hh_Exit[11];
	lrc_gen2(buf,11);
	buf[7]=buf[11];
	if(tocken_write(12,gTransTime)!=0)
		if(tocken_write(12,gTransTime)!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
	if(tocken_write(13,buf+4)!=0)
		if(tocken_write(13,buf+4)!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
	if(wrcom&0x01)
	{
		goto   notwrite;
	}
	buf[0]=*((char *)&gCardThisTransValue+LONG_HIGH1);
	buf[1]=*((char *)&gCardThisTransValue+LONG_HIGH0);
	buf[2]=hh_Exit[10];
	buf[3]=hh_Exit[11];
	if(tocken_write(14,buf)!=0)
		if(tocken_write(14,buf)!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
notwrite:			
	buf[0]=0xff;
	buf[1]=0xff;
	buf[2]=0xff;
	buf[3]=0xff;
	if(tocken_write(9,buf)!=0)
	{
		gResponseStatus=CE_WRITEERROR;
		return OP_ERR;
	}
	buf[2]=hh_SaleBuf[10];
	buf[3]=hh_SaleBuf[11];
	buf[0]=*((char *)&gCardThisRemain+LONG_HIGH1);
	buf[1]=*((char *)&gCardThisRemain+LONG_HIGH0);
	if(tocken_write(7,buf)!=0)
	{
		gResponseStatus = CE_WRITEERROR;
		return OP_ERR;
	}
	if(SMGetTacStep2()!=0)												//ȡTAC
	{
		gResponseStatus = CE_PSAMERROR;
		return OP_ERR;
	}
	memcpy(gTAC,bgTaskKeyOutbuf,4);		
				
	buf[0]=hh_trans_id;
	temp=gCardCnt+1;
	buf[1]=*((char *)&temp+INT_HIGH);
	buf[2]=*((char *)&temp+INT_LOW);
	lrc_gen2(buf,3);
	if(tocken_write(9,buf)!=0)
	{							
		gResponseStatus=CE_WRITEERROR;
		return OP_ERR;
	}
	return 0;
}
/*********************************************************
������hh_trans_update
���ܣ�����Ʊ����
*********************************************************/
UBYTE hh_trans_update(UBYTE *rec_buf)
{
	UBYTE buf[20];
	UBYTE writecmd;
	UBYTE updatetype;
	UWORD temp;
	
	updatetype = rec_buf[0];
	writecmd = rec_buf[19]&0x07;
	
	//����TAC
	memcpy(KeyBuf,gThisCardSnr,4);
	memcpy(&KeyBuf[4],gTransTime,4);
	KeyBuf[8] = hh_SaleBuf[8];								//����ǰ���,����ǰ
	KeyBuf[9] = hh_SaleBuf[9];
	KeyBuf[10]= *((char *)&gCardThisTransValue+LONG_HIGH1);					//���׽��,����ǰ
	KeyBuf[11]= *((char *)&gCardThisTransValue+LONG_HIGH0);
	KeyBuf[12]= hh_trans_Line;
	KeyBuf[13]= hh_trans_Station;
	memcpy(&KeyBuf[14],gPosNo,4);
	KeyBuf[16]= 0x15;
	KeyBuf[18]= *((char *)&gPosTransCnt+LONG_HIGH3);
	KeyBuf[19]= *((char *)&gPosTransCnt+LONG_HIGH2);
	KeyBuf[20]= *((char *)&gPosTransCnt+LONG_HIGH1);
	KeyBuf[21]= *((char *)&gPosTransCnt+LONG_HIGH0);
	KeyBuf[22]= *((char *)&gCardCnt+INT_HIGH);
	KeyBuf[23]= *((char *)&gCardCnt+INT_LOW);
	memcpy(&KeyBuf[24],bpgSmpsamNo,4);
	KeyBuf[28]= 0x80;
	KeyBuf[29]= 0x00;
	KeyBuf[30]= 0x00;
	KeyBuf[31]= 0x00;
	SHMetroTACStep1(KeyBuf);
	//��ʼ����
	if(b9error==0x55)
	{
		b9error=0;
	}
	else
	{
		buf[0]=hh_B9[0];
		buf[1]=hh_SaleBuf[8];
		buf[2]=hh_SaleBuf[9];
		lrc_gen2(buf,3);
		if(tocken_write(15,buf)!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
	}
	
	//��д���׼�¼
	if(updatetype == 0)
	{
		if(tocken_read(12,&hh_Exit[0])!=0)
		{
			gResponseStatus=CE_READERROR;
			return OP_ERR;
		}
		if(tocken_read(13,&hh_Exit[4])!=0)
		{
			gResponseStatus=CE_READERROR;
			return OP_ERR;
		}
		if(tocken_read(14,&hh_Exit[8])!=0)
		{
			gResponseStatus=CE_READERROR;
			return OP_ERR;
		}
		if(tocken_read(7,&hh_SaleBuf[8])!=0)
		{
			gResponseStatus=CE_READERROR;
			return OP_ERR;
		}
		switch(writecmd)
		{
			case 0:				
				hh_Exit[8]=*((char *)&gCardThisTransValue+LONG_HIGH1);
				hh_Exit[9]=*((char *)&gCardThisTransValue+LONG_HIGH0);
        hh_SaleBuf[10]=rec_buf[17];
        hh_SaleBuf[11]=rec_buf[18];
        break;
      case 1:
      	hh_Exit[8]=*((char *)&gCardThisTransValue+LONG_HIGH1);
				hh_Exit[9]=*((char *)&gCardThisTransValue+LONG_HIGH0);
				break;
			case 2:
				hh_SaleBuf[10]=rec_buf[17];
        hh_SaleBuf[11]=rec_buf[18];
        break;
      case 3:
      	break;
      case 4:
      case 6:
      	memcpy(&hh_Exit[8],gTransTime,4);
      	hh_SaleBuf[10]=rec_buf[17];
        hh_SaleBuf[11]=rec_buf[18];
        break;
      case 7:
      case 5:
      	memcpy(&hh_Exit[8],gTransTime,4);
      	break;
      default:
      	break;
		}
		lrc_gen2(hh_Exit,7);	
		hh_Exit[7]=hh_Exit[7]^hh_Exit[8]^hh_Exit[9]^hh_Exit[10]^hh_Exit[11];
		if(tocken_write(7,&hh_SaleBuf[8])!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
		if(tocken_write(13,&hh_Exit[4])!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
		if(tocken_write(14,&hh_Exit[8])!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
	}
	//д��Ʊ��¼
	if(updatetype == 1)
	{
		hh_SaleBuf[0]=rec_buf[2];
		hh_SaleBuf[1]=rec_buf[3];
		hh_SaleBuf[2]=rec_buf[4];
		if(tocken_read(12,&hh_Exit[0])!=0)
		{
			gResponseStatus=CE_READERROR;
			return OP_ERR;
		}
		if(tocken_read(13,&hh_Exit[4])!=0)
		{
			gResponseStatus=CE_READERROR;
			return OP_ERR;
		}
		if(tocken_read(14,&hh_Exit[8])!=0)
		{
			gResponseStatus=CE_READERROR;
			return OP_ERR;
		}
		if(tocken_read(7,&hh_SaleBuf[8])!=0)
		{
			gResponseStatus=CE_READERROR;
			return OP_ERR;
		}
		switch(writecmd)
		{
			case 0:				
				hh_Exit[8]=*((char *)&gCardThisTransValue+LONG_HIGH1);
				hh_Exit[9]=*((char *)&gCardThisTransValue+LONG_HIGH0);
        hh_SaleBuf[10]=rec_buf[17];
        hh_SaleBuf[11]=rec_buf[18];
        break;
      case 1:
      	hh_Exit[8]=*((char *)&gCardThisTransValue+LONG_HIGH1);
				hh_Exit[9]=*((char *)&gCardThisTransValue+LONG_HIGH0);
				break;
			case 2:
				hh_SaleBuf[10]=rec_buf[17];
        hh_SaleBuf[11]=rec_buf[18];
        break;
      case 3:
      	break;
      case 4:
      case 6:
      	memcpy(&hh_Exit[8],gTransTime,4);
      	hh_SaleBuf[10]=rec_buf[17];
        hh_SaleBuf[11]=rec_buf[18];
        break;
      case 7:
      case 5:
      	memcpy(&hh_Exit[8],gTransTime,4);
      	break;
      default:
      	break;
		}
		lrc_gen2(hh_Exit,7);	
		hh_Exit[7]=hh_Exit[7]^hh_Exit[8]^hh_Exit[9]^hh_Exit[10]^hh_Exit[11];
		if(tocken_write(7,&hh_SaleBuf[8])!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
		if(tocken_write(13,&hh_Exit[4])!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
		if(tocken_write(14,&hh_Exit[8])!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
		if(tocken_write(5,&hh_SaleBuf[0])!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
	}
	//д��վ���׼�¼
	if(updatetype == 2)
	{
		memcpy(hh_Entry,gTransTime,4);
		hh_Entry[4]=rec_buf[2];
		hh_Entry[5]=rec_buf[3];
		hh_Entry[6]=rec_buf[4];
		hh_SaleBuf[8] = *((char *)&gCardThisRemain+LONG_HIGH1);
		hh_SaleBuf[9] = *((char *)&gCardThisRemain+LONG_HIGH0);
		lrc_gen2(hh_Entry,7);
		if(tocken_write(10,&hh_Entry[0])!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
		if(tocken_write(11,&hh_Entry[4])!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
		if(tocken_read(12,&hh_Exit[0])!=0)
		{
			gResponseStatus=CE_READERROR;
			return OP_ERR;
		}
		if(tocken_read(13,&hh_Exit[4])!=0)
		{
			gResponseStatus=CE_READERROR;
			return OP_ERR;
		}
		if(tocken_read(14,&hh_Exit[8])!=0)
		{
			gResponseStatus=CE_READERROR;
			return OP_ERR;
		}
		if(tocken_read(7,&hh_SaleBuf[8])!=0)
		{
			gResponseStatus=CE_READERROR;
			return OP_ERR;
		}
		switch(writecmd)
		{
			case 0:				
				hh_Exit[8]=*((char *)&gCardThisTransValue+LONG_HIGH1);
				hh_Exit[9]=*((char *)&gCardThisTransValue+LONG_HIGH0);
        hh_SaleBuf[10]=rec_buf[17];
        hh_SaleBuf[11]=rec_buf[18];
        break;
      case 1:
      	hh_Exit[8]=*((char *)&gCardThisTransValue+LONG_HIGH1);
				hh_Exit[9]=*((char *)&gCardThisTransValue+LONG_HIGH0);
				break;
			case 2:
				hh_SaleBuf[10]=rec_buf[17];
        hh_SaleBuf[11]=rec_buf[18];
        break;
      case 3:
      	break;
      case 4:
      case 6:
      	memcpy(&hh_Exit[8],gTransTime,4);
      	hh_SaleBuf[10]=rec_buf[17];
        hh_SaleBuf[11]=rec_buf[18];
        break;
      case 7:
      case 5:
      	memcpy(&hh_Exit[8],gTransTime,4);
      	break;
      default:
      	break;
		}
		lrc_gen2(hh_Exit,7);	
		hh_Exit[7]=hh_Exit[7]^hh_Exit[8]^hh_Exit[9]^hh_Exit[10]^hh_Exit[11];
		if(tocken_write(7,&hh_SaleBuf[8])!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
		if(tocken_write(13,&hh_Exit[4])!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
		if(tocken_write(14,&hh_Exit[8])!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
		if(tocken_write(5,&hh_SaleBuf[0])!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
	}
	//д��վ���׼�¼
	if(updatetype == 3)
	{
		memcpy(hh_Exit,gTransTime,4);
		hh_Exit[4]=rec_buf[2];
		hh_Exit[5]=rec_buf[3];
		hh_Exit[6]=rec_buf[4];
		hh_SaleBuf[8] = *((char *)&gCardThisRemain+LONG_HIGH1);
		hh_SaleBuf[9] = *((char *)&gCardThisRemain+LONG_HIGH0);

		if(tocken_read(12,&hh_Exit[0])!=0)
		{
			gResponseStatus=CE_READERROR;
			return OP_ERR;
		}
		if(tocken_read(13,&hh_Exit[4])!=0)
		{
			gResponseStatus=CE_READERROR;
			return OP_ERR;
		}
		if(tocken_read(14,&hh_Exit[8])!=0)
		{
			gResponseStatus=CE_READERROR;
			return OP_ERR;
		}
		if(tocken_read(7,&hh_SaleBuf[8])!=0)
		{
			gResponseStatus=CE_READERROR;
			return OP_ERR;
		}
		switch(writecmd)
		{
			case 0:				
				hh_Exit[8]=*((char *)&gCardThisTransValue+LONG_HIGH1);
				hh_Exit[9]=*((char *)&gCardThisTransValue+LONG_HIGH0);
        hh_SaleBuf[10]=rec_buf[17];
        hh_SaleBuf[11]=rec_buf[18];
        break;
      case 1:
      	hh_Exit[8]=*((char *)&gCardThisTransValue+LONG_HIGH1);
				hh_Exit[9]=*((char *)&gCardThisTransValue+LONG_HIGH0);
				break;
			case 2:
				hh_SaleBuf[10]=rec_buf[17];
        hh_SaleBuf[11]=rec_buf[18];
        break;
      case 3:
      	break;
      case 4:
      case 6:
      	memcpy(&hh_Exit[8],gTransTime,4);
      	hh_SaleBuf[10]=rec_buf[17];
        hh_SaleBuf[11]=rec_buf[18];
        break;
      case 7:
      case 5:
      	memcpy(&hh_Exit[8],gTransTime,4);
      	break;
      default:
      	break;
		}
		lrc_gen2(hh_Exit,7);	
		hh_Exit[7]=hh_Exit[7]^hh_Exit[8]^hh_Exit[9]^hh_Exit[10]^hh_Exit[11];
		if(tocken_write(7,&hh_SaleBuf[8])!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
		if(tocken_write(13,&hh_Exit[4])!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
		if(tocken_write(14,&hh_Exit[8])!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
		if(tocken_write(5,&hh_SaleBuf[0])!=0)
		{
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
	}
	if(SMGetTacStep2()!=0)												//ȡTAC
	{
		gResponseStatus = CE_PSAMERROR;
		return OP_ERR;
	}
	memcpy(gTAC,bgTaskKeyOutbuf,4);		
				
	buf[0]=hh_trans_id;
	temp=gCardCnt+1;
	buf[1]=*((char *)&temp+INT_HIGH);
	buf[2]=*((char *)&temp+INT_LOW);
	lrc_gen2(buf,3);
	if(tocken_write(9,buf)!=0)
	{							
		gResponseStatus=CE_WRITEERROR;
		return OP_ERR;
	}
	return 0;
}
/*********************************************************
������get_metro_rec
���ܣ�������־���Ϸ�ʱ��ȡ������¼
*********************************************************/
UBYTE get_metro_rec(UBYTE *record_buf)
{
	UBYTE buf32[16],buf33[16];
	UBYTE result,i;
	
	result=0;
	if(checkrec_inmemory(REC_EXIT)!=0)
	{
		if(gCPUflag==0x55)
		{
			i = get_record(0x11,0x02,16,gB33);
		}
		else
		{
			i = mcmh_read(33,gB33,0,0);
		}	
		if(i)	
//		if(mcmh_read(33,gB33,0,0)!=0)
		{
			return 0x55;
		}
		lrc3_gen(gB33,16);
		memcpy(gB33BK,gB33,20);
	}
	memcpy(buf33,gB33,16);
	if(checkrec_inmemory(REC_ENTRY)!=0)
	{
		if(gCPUflag==0x55)
		{
			i = get_record(0x11,0x01,16,gB32);
		}
		else
		{
			i = mcmh_read(32,gB32,0,0);
		}	
		if(i)	
//		if(mcmh_read(32,gB32,0,0)!=0)
		{
			return 0x55;
		}
		lrc3_gen(gB32,16);
		memcpy(gB32BK,gB32,20);
	}
	memcpy(buf32,gB32,16);
	
	if(lrc_chk2(buf32,15)!=0)
		result=5;
	if(memcmp(buf32,"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",16)==0)
		result=5;
	if(memcmp(buf32,"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff",16)==0)
		result=5;
	if(lrc_chk2(buf33,15)!=0)
		result|=0x50;
	if(memcmp(buf33,"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",16)==0)
		result|=0x50;
	if(memcmp(buf33,"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff",16)==0)
		result|=0x50;
	if(result==0)
	{
		for(i=1;i<6;i++)
		{
			if(buf32[i]!=buf33[i])
			{
				if(buf32[i]<buf33[i])
				{
					memcpy(record_buf,buf33,16);
				}
				else
					memcpy(record_buf,buf32,16);
				return 0;
			}
		}
		if(gMode==ENTRY_MODE)
		{
			memcpy(record_buf,buf33,16);
		}
		else
			memcpy(record_buf,buf32,16);
		return 0;
	}
	if(result==5)
	{
		memcpy(record_buf,buf33,16);
		return 0;
	}
	if(result==0x50)
	{
		memcpy(record_buf,buf32,16);
		return 0;
	}
	return 1;
}
/*********************************************************
������checkrec_inmemory
���ܣ���ȡ�ڴ��е�ר�ü�¼����У��
*********************************************************/
UBYTE checkrec_inmemory(UBYTE type)
{
	if(type)
	{
		if(lrc3_chk(gB33,16)!=0)
		{
			if(lrc3_chk(gB33BK,16)!=0)
			{
				return 1;
			}
			memcpy(gB33,gB33BK,20);
			return 0;
		}
		if(lrc3_chk(gB33BK,16)!=0)
		{
			memcpy(gB33BK,gB33,20);
		}
		return 0;
	}
	else
	{
		if(lrc3_chk(gB32,16)!=0)
		{
			if(lrc3_chk(gB32BK,16)!=0)
			{
				return 1;
			}
			memcpy(gB32,gB32BK,20);
			return 0;
		}
		if(lrc3_chk(gB32BK,16)!=0)
		{
			memcpy(gB32BK,gB32,20);
		}
		return 0;
	}
	return 1;
}
#endif