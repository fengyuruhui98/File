//sh_globle.c

#ifndef _SH_GLOBLE_C_
#define _SH_GLOBLE_C_
//start of file

#define	FAMILY_SJT	0
#define	FAMILY_M1	1
#define FAMILY_QR	2
#define	FAMILY_CPU_CARD	3	
#define	FAMILY_CPU_PF	4
#define	FAMILY_CPU_EM	5

UBYTE	gCardFamily;	//0:����Ʊ��1��M1��, 2����ά�룬 3��CPU��(����)��4��CPU���ַ�����5��CPU���ַ�Ա����

UBYTE gResponseStatus;
UBYTE gGetCardInfoStep;
UWORD gDebugStep;
UBYTE gPowerOffFlag;
UDWORD mobilestarttime,mobilecurrent;
UBYTE TimeBuf[80];
UDWORD dwgCmdExeTime;
UBYTE bgDeviceStatus;

UBYTE bgTaskKeyStep;
UBYTE bgTaskKeyInbuf[256];
UBYTE bgTaskKeyOutbuf[256];
UBYTE bgTaskKeyFinishState;
UBYTE gbTaskKeyNum;
UBYTE gThisCardSnr[16];
UBYTE bgHaltFlag=0;
//�Ϻ���

UBYTE gRedSign;

UBYTE gGetCardInfoStep;				//0x00Ϊ���Զ���
UBYTE  gCardType;					//����
UBYTE  cardversion;					//���汾
UBYTE  gCardStatus,gCardRecPtr;
UWORD  gCardCnt;
UBYTE  gCardExpiredDate[4];			//��ʧЧ����

UBYTE  NormalCard;					//��ͨ��ƴο�
UBYTE  ReadMode;					//������ʽ
UBYTE  UseType;						//�ƴο����÷�ʽ
UBYTE  StartUseDay[4];				//�ƴο���������
UBYTE  FirstDay[4];					//�ƴο��״�ʹ������
UWORD  UseTimes;					//�ƴο��ۻ�ʹ�ô���
UBYTE  DayToAllow;					//�ƴο���������
UBYTE  info[5];						//��ͨ����Ϣ
UBYTE leisure[5];					//�ۻ��������ۻ����
UBYTE  gMetroIoFlag;				//����������־
UBYTE  gMetroIoFlag34,gMetroIoFlagBk,gMetroIoFlag34Valid,gMetroIoFlagBkValid;
UDWORD gCardPurse1,gCardPurse2,gCardPurse;	//Ǯ��ֵ
UDWORD gCardAddSum;					//�ۻ����ʽ��
UDWORD gCardLastRemain;				//���Ͻ���ǰ���
UDWORD gCardThisTransValue;			//���ν��׽��
UDWORD gCardThisRemain;				//���׺����
UBYTE  gStationTimeOverRideFlag;	//�·��ĵ��������Żݱ�־
UDWORD gCardLastTransValue; //�ϴν��׽��

//�Ϻ�����Ʊ
UBYTE hh_key[6];											//��Կ
UBYTE hh_mid[2];											//�����̴���
UBYTE hh_SaleBuf[12];//b5,6,7					//��Ʊ��Ϣ
UBYTE hh_B9[4];												//BLOCK9����
UBYTE hh_Entry[8];										//��վ��Ϣ
UBYTE hh_Exit[12];										//��վ��Ϣ
UBYTE hh_trans_Line,hh_trans_Station,hh_trans_Equip;		//����վ���豸

UBYTE  gPosNo[4];					//POS����
UBYTE  gTransTime[7];			//����ʱ��
UDWORD gPosTransCnt;      //������ˮ��
UBYTE  gRelativePtr;			//��¼���ָ��
UBYTE  gTAC[4];						//TAC

UBYTE Rand[10];
int gCurBaudRate;
UBYTE Eepromptr1,Eepromptr2;
UBYTE bgRledState,bgGledState,bgRState,bgGState,bgBuzzerState;

//UBYTE gTemp[200];
//UBYTE gTemp2[200];
UBYTE timeflag;

//4016
char *temp_4016;
UBYTE buf_4016[4];

//4020 :20190530 add
char *temp_4020;
UBYTE buf_4020[4];

//2.4G
//UBYTE bgZTEReceLen;
//UBYTE bpgZTEReceBlock[300];
//UBYTE bgZTEReceState;

/*=================================================================================
������
���ܣ�
===================================================================================*/
int mifpro_ats0(UBYTE *outbuf)
{
UBYTE ret,obytes;

ret = mifpro_ats(1,outbuf,&obytes);
if(ret) return -1;

return obytes;	
}

	
/*=================================================================================
������
���ܣ�
===================================================================================*/
int mifpro_icmd0(UBYTE len,UBYTE *sbuf,UBYTE *outbuf2)
{
UBYTE ret,obytes;
UWORD	apduLen;
UWORD i;

apduLen = 0;

	ret = mifpro_icmd(sbuf,len,outbuf2,&apduLen);
#ifdef	_DEBUG_BOM_
	debug_printf("apdu: ");
		for(i = 0; i < len; i++)
			debug_printf("%02X", sbuf[i]);
	debug_printf("\n	:");
		for(i = 0; i< apduLen; i++)
			debug_printf("%02X", outbuf2[i]);
	debug_printf("\n");
#endif
if(ret) return -1;

return apduLen;	
}	


/*------------------------------------------------------
�潻ͨ����¼���洢��¼��ʽ��3����ˮ�ţ���4�����ţ�		   
��2�����׼���������3������4��TAC��		  

-----------------------------------------------------*/
UBYTE save_trans_record(UBYTE *record)
{
	UWORD tint;
	tint=Eepromptr1*16+256;
	ee_write(tint,16,record);
	Eepromptr1=(Eepromptr1+1)%8;
	return 0;
}
/*------------------------------------------------------
�污�Ϳ���¼���洢��¼��ʽ��4����ˮ�ţ���2��ʹ�ô�����
��4��TAC����4�����ţ���2������00��				    

-----------------------------------------------------*/
UBYTE save_sigal_record(UBYTE *record)
{
	UWORD tint;
	tint=Eepromptr2*16+384;
	ee_write(tint,16,record);
	Eepromptr2=(Eepromptr2+1)%8;	
	return 0;
}
/*------------------------------------------------------
��QR��¼���洢��¼��ʽ��4����ˮ�ţ���10�����ţ�		   
��8��divery����4��GMT����4��TAC��		  

-----------------------------------------------------*/
UBYTE save_qr_record(UBYTE *record)
{
UWORD tint;
	
	tint=ADDR_QR_REC;
	ee_write(tint,32,record);
	
	return 0;
}
/*------------------------------------------------------
��ODA TAC��¼���洢��¼��ʽ��3����ˮ�ţ���2�������д��룩		   
��10�����˺ţ���1�����˺���ţ���4��TAC��		  

-----------------------------------------------------*/
UBYTE save_oda_record(UBYTE *record)
{
UWORD tint;
	
	tint=ADDR_ODA_REC;
	ee_write(tint,32,record);
	
	return 0;
}


/*------------------------------------------------------
�潻ͨ����¼���洢��¼��ʽ��3����ˮ�ţ���10�����ţ���2�����׼�������+1����Կ������
+1���㷨��ʶ��+2���ѻ�������ţ�+4��PSAM��ˮ�ţ���3������4��TAC��
2019-06-01	  
-----------------------------------------------------*/
UBYTE save_transport_record(UBYTE *record)
{
	UWORD tint;
	int i;
	
#ifdef _DEBUG_BOM_
debug_printf("\nsave_transport_record...=");
for(i = 0;i < 30;i++)
{
	debug_printf("%02x ",record[i]);
}
debug_printf("\n");
#endif

	tint=ADDR_TRANSPORT_REC;
	ee_write(tint,32,record);
	
	return 0;
}
/*------------------------------------------------------
������ˮ��ȡ��ͨ����¼���洢��¼��ʽ��3����ˮ�ţ���4�����ţ�		   
��2�����׼���������3������4��TAC��	    
���ؼ�¼��ʽ��4�����ţ���2�����׼���������3������4��TAC��			 

-----------------------------------------------------*/
UBYTE get_trans_record(UBYTE *transptr,UBYTE *record_out,UBYTE ex_get)//3,13
{
UBYTE i;
UWORD  tint;

	for(i=0;i<8;i++)															//ԭ���� 
	{
		tint=368-16*i;
		ee_read(tint,16,record_out);
		if((UBYTE)record_out[0]!=(UBYTE)transptr[0])continue;
		if((UBYTE)record_out[1]!=(UBYTE)transptr[1])continue;
		if((UBYTE)record_out[2]!=(UBYTE)transptr[2])continue;
		if(ex_get)
		{
			if((UBYTE)record_out[3]!=(UBYTE)transptr[3])continue;
			if((UBYTE)record_out[4]!=(UBYTE)transptr[4])continue;
			if((UBYTE)record_out[5]!=(UBYTE)transptr[5])continue;
			if((UBYTE)record_out[6]!=(UBYTE)transptr[6])continue;
			if((UBYTE)record_out[7]!=(UBYTE)transptr[7])continue;
			if((UBYTE)record_out[8]!=(UBYTE)transptr[8])continue;
		}
		return 0;
	} 
	return OP_ERR;	
}
/*------------------------------------------------------
ȡ���Ϳ���¼���洢��¼��ʽ��4����ˮ�ţ���2��ʹ�ô�����		   
��4��TAC����4�����ţ���2������00��			   
���ؼ�¼��ʽ��2��ʹ�ô�������4��TAC����4�����ţ���2������00��			 

-----------------------------------------------------*/
UBYTE get_sigal_record(UBYTE *transptr,UBYTE *record_out)
{
	UBYTE i;
	UWORD tint;
		for(i=0;i<8;i++)															//ԭ���� 
		{
			tint=496-16*i;
			ee_read(tint,16,record_out);
			if((UBYTE)record_out[0]!=(UBYTE)transptr[3])continue;
			if((UBYTE)record_out[1]!=(UBYTE)transptr[2])continue;
			if((UBYTE)record_out[2]!=(UBYTE)transptr[1])continue;
			if((UBYTE)record_out[3]!=(UBYTE)transptr[0])continue;
			return 0;
		}
	return OP_ERR;		
}

UBYTE get_qr_record(UBYTE *inbuf,UBYTE *record)
{
UWORD tint;
UBYTE rec[32];
	
	tint=ADDR_QR_REC;
	ee_read(tint, 32, rec);

	if(memcmp(inbuf, rec, 22) == 0)
	{
		memcpy(record, &rec[22], 8);
		return 0;
	}
	return OP_ERR;
}

UBYTE get_oda_record(UBYTE *inbuf,UBYTE *record)
{
UWORD tint;
UBYTE rec[32];
	
	tint=ADDR_ODA_REC;
	ee_read(tint, 32, rec);

	if(memcmp(inbuf, rec, 16) == 0)
	{
		memcpy(record, &rec[16], 4);
		return 0;
	}
	return OP_ERR;
}

/*==============================================================================
������get_transport_record
���ܣ�ȡ��ͨ����¼
================================================================================*/
UBYTE get_transport_record(UBYTE *inbuf,UBYTE *record)
{
UWORD tint;
UBYTE rec[32];
	
	tint=ADDR_TRANSPORT_REC;
	ee_read(tint, 32, rec);

	if(memcmp(inbuf, rec, 15) == 0)
	{
		memcpy(record, rec, 32);
		return 0;
	}
	return OP_ERR;
}
/*==============================================================================
������gledantenna
���ܣ��������̵���ʾ
================================================================================*/
void gledantenna(UBYTE option)
{
//ʵ��---------------------------------------------------------------------------	
/*if(option == LED_OFF){gpd_clr_bit(BIT04_MASK);}
else{gpd_set_bit(BIT04_MASK);}
bgGState = option;*/
return;
}	
/*==============================================================================
������rledantenna
���ܣ������̺����ʾ
================================================================================*/
void rledantenna(UBYTE option)
{
//ʵ��---------------------------------------------------------------------------	
/*if(option == LED_OFF){gpd_clr_bit(BIT03_MASK);}
else{gpd_set_bit(BIT03_MASK);}
bgRState = option;*/
return;
}	
/*==============================================================================
������buzzerantenna
���ܣ������̷�����
================================================================================*/
void buzzerantenna(UBYTE option)
{
//ʵ��---------------------------------------------------------------------------	
/*if(option == LED_OFF){gpd_clr_bit(BIT05_MASK);}
else{gpd_set_bit(BIT05_MASK);}
bgBuzzerState = option;*/
return;
}	

/*******************************************
������display_set
���ܣ�������LED��ʾ
*******************************************/
void display_set(UBYTE mode,UBYTE para)
{
	UBYTE temp;
	
	temp=mode&0xf0;
	switch(temp)
	{
		case 0x10:temp=mode&0x0f;
							timeflag&=0xfc;
							switch(temp)
							{
								case 0x00:gledantenna(LED_OFF);													
													break;
								case 0x01:timer_set(TIMER_MS_INDEX0,para*100);
													gledantenna(LED_ON);
													timeflag|=1;
													break;
								case 0x02:timer_set(TIMER_MS_INDEX0,300);
													gledantenna(LED_ON);
													timeflag|=2;
								default:	break;
							}
							break;
		case 0x20:temp=mode&0x0f;
							timeflag&=0xf3;
							switch(temp)
							{
								case 0x00:rledantenna(LED_OFF);													
													break;
								case 0x01:timer_set(TIMER_MS_INDEX1,para*100);
													rledantenna(LED_ON);
													timeflag|=0x04;
													break;
								case 0x02:timer_set(TIMER_MS_INDEX1,300);
													rledantenna(LED_ON);
													timeflag|=0x08;
								default:	break;
							}
		default:	break;
	}
	return;
}
/*******************************************
������buzzer_set
���ܣ�����������
*******************************************/
void buzzer_set(UBYTE mode,UBYTE para)
{
	UBYTE temp;
	temp=mode&0x0f;
	timeflag&=0xcf;
	switch(temp)
	{
		case 0x00:	buzzerantenna(LED_OFF);
								break;
		case 0x01:	timer_set(TIMER_MS_INDEX2,para*100);
								buzzerantenna(LED_ON);
								timeflag|=0x10;
								break;
		case 0x02:	timer_set(TIMER_MS_INDEX2,300);
								buzzerantenna(LED_ON);
								timeflag|=0x20;
		default:		break;
	}
	return;
}


/*=========================================================================
��������tocken_read
��  �ܣ�����
��ڲ���:	��
���ڲ���:��
===========================================================================*/

#define CHECK_681
/*
char tocken_read(UBYTE addr, UBYTE *readdata)
{
	UBYTE i,j;
	UBYTE buf[16];
	
	#ifdef CHECK_681
	UBYTE tbuf[16];
	#endif
	i=0;
	loop:
//	watchdog();
	if(token_mcml_read(addr,readdata)!=0)
	{
		i++;
		#ifdef _debug_test_
		debug_printf("\n token_mcml_read 1 err\n");
		#endif
	 	if(i<6)goto label_err;
	 	return OP_ERR;
	}
	
	#ifdef CHECK_681
	if(token_mcml_read(addr,tbuf)!=0)
	{
		i++;
	 	if(i<6)goto label_err;
	 	return OP_ERR;
	}
	if(memcmp(readdata,tbuf,4)!=0)
	{
		i++;
	 	if(i<6)goto loop;
	 	return OP_ERR;
	}
	#endif			 
	return 0;
	label_err:
	for(j=0;j<MAX_TRY;j++)
	{
	   if(mcml_request(PICC_REQSTD,buf)== 0)
	   {
	   		goto label_con;
		 }
	}
	return OP_ERR;
	label_con:
	
	if((buf[0]==0x03)&&(buf[1]==0x00))//�������Ϳ�
	{
			hh_key[4]=0x00;
			hh_key[5]=0x00;	
	}
	else if((buf[0]==0x00)&&(buf[1]==0x36))//���籡�Ϳ�		    
	{
			hh_key[4]=0xff;
			hh_key[5]=0xff;
	}
	else return OP_ERR;
	if(token_mcml_read(1,buf)!=0)
	if(token_mcml_read(1,buf)!=0)
	if(token_mcml_read(1,buf)!=0)
		return OP_ERR;
	if(addr<8)goto loop;
	
	mcml_load_key(0,0,0,hh_key);
	if(mcml_authentication(0,0,0)!=0) 
	if(mcml_authentication(0,0,0)!=0)
	if(mcml_authentication(0,0,0)!=0)
	{
		return OP_ERR;
	}
	goto loop;
}*/


/*=========================================================================
��������token_write
��  �ܣ�����
��ڲ���:	��
���ڲ���:��
===========================================================================*/
/*
char (UBYTE addr, UBYTE *writedata)
{
	UBYTE i,j;
	UBYTE buf[5];
	
	#ifdef CHECK_681
	UBYTE tbuf[5];
	#endif
	i=0;
	loop:
//	watchdog();
	if(token_mcml_write(addr,writedata)!=0)
	{
		i++;
	 	if(i<6)goto label_err;
	 	return OP_ERR;
	}
	
	//#ifdef CHECK_681
	if(token_mcml_read(addr,tbuf)!=0)
	{
		i++;
	 	if(i<6)goto label_err;
	 	return OP_ERR;
	}
	if(memcmp(writedata,tbuf,4)!=0)
	{
		i++;
	 	if(i<6)goto loop;
	 	return OP_ERR;
	}
	//#endif			 
	 return 0;
	label_err:
	for(j=0;j<MAX_TRY;j++)
	{
	   if(mcml_request(PICC_REQSTD,buf) == 0)
	   {
	   		goto label_con;
		 }
	}
	return OP_ERR;
	label_con:
	
	if((buf[0]==0x03)&&(buf[1]==0x00))//�������Ϳ�
	{
			hh_key[4]=0x00;
			hh_key[5]=0x00;	
	}
	else if((buf[0]==0x00)&&(buf[1]==0x36))//���籡�Ϳ�		    
	{
			hh_key[4]=0xff;
			hh_key[5]=0xff;
	}
	else return OP_ERR;
	if(token_mcml_read(1,buf)!=0)
	if(token_mcml_read(1,buf)!=0)
	if(token_mcml_read(1,buf)!=0)
		return OP_ERR;
	if(addr<8)goto loop;
	
	mcml_load_key(0,0,0,hh_key);
	if(mcml_authentication(0,0,0)!=0) 
	if(mcml_authentication(0,0,0)!=0)
	if(mcml_authentication(0,0,0)!=0)
	{
		return OP_ERR;
	}
	goto loop;
}
  */

/*=================================================================================================
����:
����:
===================================================================================================*/
int InitComm(int port,int baudrate,int stopbits,char parity)
{
UBYTE baudindex;

if((port < 0)	|| (port >= MAX_UART_NUM)) return -1;
//	
stopbits = stopbits;
parity = parity;
//
switch(baudrate){
  case 1200:
  	   baudindex = BAUD1200_INDEX;
  	   break;
  case 2400:
  	   baudindex = BAUD2400_INDEX;
  	   break;
  case 4800:
  	   baudindex = BAUD4800_INDEX;
  	   break;
  case 9600:
  	   baudindex = BAUD9600_INDEX;
  	   break;
  case 19200:
  	   baudindex = BAUD19200_INDEX;
  	   break;
  case 28800:
  	   baudindex = BAUD28800_INDEX;
  	   break;
 	case 38400:
 			 baudindex = BAUD38400_INDEX;
  	   break;
  case 57600:
  	   baudindex = BAUD57600_INDEX;
  	   break;
  case 115200:
  	   baudindex = BAUD115200_INDEX;
  	   break;
  case 128000:
  	   baudindex = BAUD128000_INDEX;
  	   break;
  case 256000:
  	   baudindex = BAUD256000_INDEX;
  	   break;
  default:
  	   return -1;	   	
	}

//uart_change_baud(port,baudindex);
//
return 0;	
}	


/*-------------------------------------------------------------------------
	Function Name: 	CmdWatchCalMac
	Time:			2008-8-10 11:53:56
	Author:			Xin Hong-wei(kpsoliton@gmail.com)
	Parameters:
	
	Return:
	
	Remarks:
	����Ĺ������£�
		1. �����ݿ�pszBufIn����Ϊ��8�ֽ�Ϊ��λ���飻
		2. ������һ�����ݿ鳤��Ϊ8�ֽڣ���ô��������80 00 00 00 00 00 00 00
		3. ������һ�����ݿ鳤�Ȳ���8�ֽڣ���ô�ں�������80,���������8�ֽڣ�������00ֱ��Ϊ8�ֽڡ�
		4. ��ʼֵpszInitData�͵�һ�����ݿ����У�飬������ֵ����ԿpszKey���ܣ�
		5. �õ���ֵ��ڶ������ݿ����У�飬������ֵ����ԿpszKey���ܣ�
		6. ֱ�����һ�����ݿ����У�飬����Ϊֹ��
	
-------------------------------------------------------------------------*/
void	CmdWatchCalMac(UWORD nLenIn,UBYTE *pszBufIn,UBYTE *pszInitData,UBYTE *pszKey,UBYTE *pszMAC,UBYTE	bTriDes)
{
	UBYTE		pszBufAdd[8];
	UBYTE		pszBuf[8];
	UWORD		i,j;
	
	UWORD		nLenTotal;		//	�������һ�����ݿ�ĳ��ȡ�
	
	nLenTotal = (nLenIn/8+1)*8;		
	
	//	����Ĳ���
	memcpy(pszBufAdd,"\x80\x00\x00\x00\x00\x00\x00\x00",8);

	//	������ʼ��ֵ
	memcpy(pszBuf,pszInitData,8);
	
	for(i=0;i<nLenTotal;i=i+8) 
	{
		//	ȡ��8���ֽ����м�ֵ���������
		for(j=0;j<8;j++)
		{
			if((i+j)<nLenIn)
			{
				pszBuf[j] = pszBufIn[i+j] ^ pszBuf[j];
			}
			else
			{
				pszBuf[j] = pszBufAdd[i+j-nLenIn] ^ pszBuf[j];
			}
		}
		
		//	DES����,�����3DES��ôֻ����벿�ֽ��м���
		des_encode(pszKey,pszBuf,pszBuf);
	
	}
	
	//	�����3DES����ô�Ұ벿�ֽ��ܣ���벿�ּ���
	if(bTriDes)
	{
		des_decode(pszKey+8,pszBuf,pszBuf);
		des_encode(pszKey,pszBuf,pszBuf);
	}
	
	//	�õ�8���ֽڣ�ǰ��4����ΪMAC.
	memcpy(pszMAC,pszBuf,4);

	return;
}

/*-------------------------------------------------------------------------
	Function Name: 	WatchDiversity
	Time:			2005-5-29 16:22:52
	Author:			Hsin Honway(xinhongwei@sina.com)
	Parameters:
					pszMKKey	[in]		16�ֽڵ�����Կ
					pszPID		[in]		8�ֽڵķ�ɢ����
					pszSKKey	[out]		16�ֽڵķ�ɢ���
					bTriDes		[in]		DES����3DES���ܡ�
	Return:
	
	Remarks:
					�������Կ��ɢ�㷨	
					2005-05-29������ΰ�Ѿ���֤OK.	
-------------------------------------------------------------------------*/
UBYTE	WatchDiversity(UBYTE *pszMKKey,UBYTE *pszPID,UBYTE	*pszSKKey,UBYTE	bTriDes)
{
	int		i;
	UBYTE	pszBuf[16];
	UBYTE	pszKey[16];
	
	if(bTriDes)
	{
		memcpy(pszKey,pszMKKey,16);
		
		memset(pszBuf,0x00,16);
		
		//	���8�ֽ�
		des_encode(pszKey, pszPID, pszBuf);
		des_decode(pszKey+8, pszBuf,pszBuf+8);
		des_encode(pszKey, pszBuf+8, pszBuf);
		memcpy(pszSKKey,pszBuf,8);
		
		//	��������ȡ��
		memset(pszBuf,0x00,16);			
		for(i=0;i<8;i++) pszBuf[i] =~pszPID[i];
		
		//	�ұ�8�ֽ�
		des_encode(pszKey, pszBuf, pszBuf+8);
		des_decode(pszKey+8, pszBuf+8,pszBuf);
		des_encode(pszKey, pszBuf, pszBuf+8);	
		memcpy(pszSKKey+8,pszBuf+8,8);
	}
	else
	{
		des_encode(pszMKKey,pszPID, pszSKKey);
	}
	return 0;
}


/*=======================================================================================
������mcmh_writewithout
���ܣ�д��Կ�������ض�
=========================================================================================*/
UBYTE mcmh_writewithout(UBYTE addr, UBYTE *writedata,UBYTE type,UBYTE key)
{
UBYTE cnt,ret;
UBYTE buf[17];

//watchdog();
cnt = 0;
if(type == 1) goto write_label1;
if(type == 2) goto write_label2;

loop:
//watchdog();
if(cnt > MCMH_MAX_TRY) return 1;
cnt++;
if(mcml_request(PICC_REQSTD,buf) != 0){
    //gCardOpErr = 0;
    goto loop;
    }
if(mcml_select(gThisCardSnr,&ret) != 0){
    //gCardOpErr = 0;
    goto loop;
    }
write_label1:
if(key == 0){
   if(mcml_load_key(0,KEYA,addr/4,gKeyA[addr/4]) != 0){
      //gCardOpErr = 0;
      goto loop;
      }
   ret = mcml_authentication(0,KEYA,addr/4);
   if((char)ret != 0){
      //gCardOpErr = 0;
      goto loop;
      }
   }
else{
   if(mcml_load_key(0,KEYB,addr/4,gKeyB[addr/4]) != 0){
      //gCardOpErr = 0;
      goto loop;
      }
   ret = mcml_authentication(0,KEYB,addr/4);
   if((char)ret != 0){
      //gCardOpErr = 0;
      goto loop;
      }
   }

write_label2:
if(mcml_write(addr,writedata) != 0){
    //gCardOpErr = 0;
    goto loop;
    }

return 0;
}
/*=======================================================================================
������mcmh_authkeyblock
���ܣ���֤��Կ��
=========================================================================================*/
UBYTE mcmh_authkeyblock(UBYTE addr, UBYTE key)
{
UBYTE cnt,ret;
UBYTE buf[17];

//gLastBlockNo = addr;
//gCardOpErr = 0;
cnt = 0;

loop:
//watchdog();
if(cnt > MCMH_MAX_TRY) return 1;
cnt++;
if(mcml_request(PICC_REQSTD,buf) != 0){
    //gCardOpErr = 0;
    goto loop;
    }
if(mcml_select(gThisCardSnr,&ret) != 0){
    //gCardOpErr = 0;
    goto loop;
    }
if(key == 0){
   if(mcml_load_key(0,KEYA,addr/4,gKeyACal[addr/4]) != 0){
      //gCardOpErr = 0;
      goto loop;
      }
   ret = mcml_authentication(0,KEYA,addr/4);
   if((char)ret != 0){
      //gCardOpErr = 0;
      goto loop;
      }
   }
else{
   if(mcml_load_key(0,KEYB,addr/4,gKeyBCal[addr/4]) != 0){
      //gCardOpErr = 0;
      goto loop;
      }
   ret = mcml_authentication(0,KEYB,addr/4);
   if((char)ret != 0){
      //gCardOpErr = 0;
      goto loop;
      }
   }


return 0;
}



/*=====================================================================================
������
���ܣ�
=======================================================================================*/
int ISO7816CommandSend(int index,int ibytes,char* ibuf)
{
/*UBYTE inbuf[256];
UBYTE inbytes;
	
if(index >= MAX_SAM_INDEX) return -1;
if(ibytes < 5) return -1;	
if(ibytes > 251) return -1;	
	 
gSamConfig[index].timeout = 1000;
	 
//����sam indexѡ��CPU
if((index/4) != 0){
   mcu2_select();
   }
else{
   mcu1_select();
   }	

inbuf[0] = SAM_CMD_APDU;
inbuf[1] = index%4;
inbuf[2] = gSamConfig[index].etu;
inbuf[3] = gSamConfig[index].wait_etu;
memcpy(&inbuf[4],ibuf,ibytes);
inbytes = 4+ibytes;   
sam_prot_rece_reset();
sam_prot_start_send(inbuf,inbytes);
//��ʱ�ȴ�
timer_set(TIMER_CMD_PROCESS_INDEX,gSamConfig[index].timeout);*/
//
return 0;		
}	


/*=====================================================================================
������
���ܣ�
=======================================================================================*/
int ISO7816CommandRece(int channel,int* obytes,char* obuf,char *sw1sw2)
{
/*UBYTE outbuf[256];
UBYTE outbytes;

channel = channel;
if(!sam_prot_rece_ready()){
	 #ifdef _NJAPI_SAM_DEBUG_
	 debug_printf("\x0d\x0a [ISO7816CommandRece]s1"); 
	 #endif
	 return -1;
	 }
//
sam_prot_rece_get(outbuf,&outbytes);
sam_prot_rece_reset();
if(outbytes < 5){
	 #ifdef _NJAPI_SAM_DEBUG_
	 debug_printf("\x0d\x0a [ISO7816CommandRece]s2:outbytes=%d",outbytes); 
	 #endif
	 return -1;
	 }
*obytes = outbytes-4;
memcpy(obuf,&outbuf[4],*obytes);	
memcpy(sw1sw2,&obuf[*obytes-2],2);*/
//
return 0;
}	


/*============================================================================
������prot_jt_send_block
���ܣ�
==============================================================================*/
void shmt_prot_send(UBYTE *buf,UWORD bytes)
{
unsigned char	sendbuf[1000];
long ret;
unsigned char chinfo[2000], chtemp[20];
long 	i;

	memset(sendbuf, 0x00, 1000);
	sendbuf[0] = bytes;
	
	if ((buf != NULL) && (bytes > 0))
		memcpy(&sendbuf[1], buf, bytes);

	ret = sh_InsertDLE(sendbuf, bytes + 1);
	
 #ifdef DEBUG_PRINT	
	memset(chinfo, 0x00, 2000);
	for(i = 0; i < ret; i++)
	{
		sprintf((char *)chtemp, "%02X ", sendbuf[i]);
		strcat((char *)chinfo, (char *)chtemp);
	}
	printf("response :%s\n", chinfo);
  #endif
  
	//back up the last message
	if(blnRetryFlag == 0)
	{
		retry_len = ret;
		memcpy(retry_buf, sendbuf, retry_len);
	}
	writecom(csc_comm, sendbuf, ret);

	return;
}	
//read 4016
void shmt_read_pf4016(UBYTE *namebuf)
{
	FILE *fp;
	unsigned char filename[100];
	UDWORD filelen;
	//UBYTE recordlen;
	char filepath[100];
	unsigned char buf[100];
	int ret,i;
	//struct HEAD_4016 head_pf4016;
	
	//sprintf(filename,"4016");
	sprintf(filename,"%s", namebuf);
	sprintf(filepath,"./%s",filename);	
	
	fp = fopen(filepath,"rb");
	if(fp == NULL)
	{
			temp_4016 = (char *)malloc(4);
			memcpy(temp_4016,"\x00\x00\x00\x00",4);
			memcpy(buf_4016,"\x00\x00\x00\x00",4);
			printf("shmt_read_pf4016 open Err!\n");
		return;
	}
	fseek(fp,0,SEEK_SET);
	fseek(fp,0,SEEK_END);
	filelen = ftell(fp);
	#ifdef DEBUG_PRINT
	printf("filelen=%X ",filelen);
	#endif
	
	temp_4016 = (char *)malloc(sizeof(char)*filelen+1);
	fseek(fp,0,SEEK_SET);
	fread(temp_4016,	1, 40, fp);
	if(((filelen-40) % 296) == 0)
	{
		buf[0] = (filelen-40) / 296;
	}
	else{
			buf[0] = (filelen-40) /296 + 1;
		}
	//printf(" recordlen = %02X", buf[0]);
	memcpy(&temp_4016[40],&buf[0],1);
	fread(&temp_4016[41],1, filelen-40, fp);
	fclose(fp);
	//if(memcmp(name))
	
	#ifdef DEBUG_PRINT
	printf("temp_4016:");
	for(i=0;i<50;i++)
	{
		printf(" %02X ",temp_4016[i]);
	}
	#endif
	return;
}

void shmt_read_data4016(FILE *fp, unsigned char *inbuf, int len)
{
	int i=0;
	struct DATA_4016 data_4016[len];
	
	while(fread(inbuf, 294, 1, fp) && i < len) 
	{   
		memcpy(data_4016[i].rid, inbuf, 5);
		memcpy(&data_4016[i].pubkeyindex, &inbuf[5], 1);
		memcpy(&data_4016[i].ackeydate, &inbuf[6], 4);
		memcpy(&data_4016[i].keyhashflag, &inbuf[10], 1);
		memcpy(&data_4016[i].keyalgflag, &inbuf[11], 1);
		memcpy(&data_4016[i].keylength, &inbuf[12], 2);
		memcpy(&data_4016[i].ackey, &inbuf[14], 256);
		memcpy(&data_4016[i].keydata, &inbuf[270], 4);
		memcpy(&data_4016[i].keylrc, &inbuf[274], 20);
		i++;         
  } 
	
	return;	
}


//read 4020
void shmt_read_4020(UBYTE *namebuf)
{
	FILE *fp;
	unsigned char filename[100];
	UDWORD filelen;
	//UBYTE recordlen;
	char filepath[100];
	unsigned char buf[100];
	int ret,i;

	sprintf(filename,"%s", namebuf);
	sprintf(filepath,"./%s",filename);	
	
	fp = fopen(filepath,"rb");
	if(fp == NULL)
	{
			temp_4020 = (char *)malloc(4);
			memcpy(temp_4020,"\x00\x00\x00\x00",4);
			memcpy(buf_4020,"\x00\x00\x00\x00",4);
			printf("shmt_read_pf4020 open Err!\n");
		return;
	}
	fseek(fp,0,SEEK_SET);
	fseek(fp,0,SEEK_END);//���ļ��ĵ�ǰλ���ƶ����ļ�ĩβ
	filelen = ftell(fp);//��õ�ǰλ��������ļ��׵�λ�ƣ���λ��ֵ�����ļ������ֽ���
	#ifdef DEBUG_PRINT
	printf("filelen=%X ",filelen);
	#endif
	
	temp_4020 = (char *)malloc(sizeof(char)*filelen+1);
	fseek(fp,0,SEEK_SET);
	fread(temp_4020,	1, 40, fp);
	if(((filelen-40) % 296) == 0)
	{
		buf[0] = (filelen-40) / 296;
	}
	else
	{
		buf[0] = (filelen-40) /296 + 1;
	}
	//printf(" recordlen = %02X", buf[0]);
	memcpy(&temp_4020[40],&buf[0],1);
	fread(&temp_4020[41],1, filelen-40, fp);
	fclose(fp);
	//if(memcmp(name))
	
	#ifdef DEBUG_PRINT
	printf("temp_4020:");
	for(i=0;i<50;i++)
	{
		printf(" %02X ",temp_4020[i]);
	}
	#endif
	return;
}
//end of file
#endif