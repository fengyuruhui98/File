//sh_globle.c

#ifndef _SH_GLOBLE_C_
#define _SH_GLOBLE_C_
//#define _DEBUG_BOM_
//start of file

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
UBYTE temp_uid[4];//临时UID：用于8054指令验算MAC2失败时保存该卡UID，再次读卡时，如果select的UID和该UID相同，返回14无效卡
UBYTE bgHaltFlag=0;
//上海卡

UBYTE gRedSign;

UBYTE gGetCardInfoStep;				//0x00为可以读卡
UBYTE  gCardType;					//卡型
UBYTE  cardversion;					//卡版本
UBYTE  gCardStatus,gCardRecPtr;
UWORD  gCardCnt;
UBYTE  gCardExpiredDate[4];			//卡失效日期

UBYTE  NormalCard;					//普通或计次卡
UBYTE  ReadMode;					//读卡方式
UBYTE  UseType;						//计次卡启用方式
UBYTE  StartUseDay[4];				//计次卡发售日期
UBYTE  FirstDay[4];					//计次卡首次使用日期
UWORD  UseTimes;					//计次卡累积使用次数
UBYTE  DayToAllow;					//计次卡允许天数
UBYTE  info[5];						//普通卡信息
UBYTE leisure[5];					//累积天数，累积金额
UBYTE  gMetroIoFlag;				//地铁进出标志
UBYTE  gMetroIoFlag34,gMetroIoFlagBk,gMetroIoFlag34Valid,gMetroIoFlagBkValid;
UDWORD gCardPurse1,gCardPurse2,gCardPurse;	//钱包值
UDWORD gCardAddSum;					//累积充资金额
UDWORD gCardLastRemain;				//卡上交易前余额
UDWORD gCardThisTransValue;			//本次交易金额
UDWORD gCardThisRemain;				//交易后余额
UBYTE  gStationTimeOverRideFlag;	//下发的地铁交易优惠标志
UDWORD gCardLastTransValue; //上次交易金额

//上海单程票
UBYTE hh_key[6];											//密钥
UBYTE hh_mid[2];											//制造商代码
UBYTE hh_SaleBuf[12];//b5,6,7					//售票信息
UBYTE hh_B9[4];												//BLOCK9内容
UBYTE hh_Entry[8];										//进站信息
UBYTE hh_Exit[12];										//出站信息
UBYTE hh_trans_Line,hh_trans_Station,hh_trans_Equip;		//交易站点设备

UBYTE  gPosNo[4];					//POS机号
UBYTE  gTransTime[7];			//交易时间
UDWORD gPosTransCnt;      //交易流水号
UBYTE  gRelativePtr;			//记录相对指针
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
函数：
功能：
===================================================================================*/
int mifpro_ats0(UBYTE *outbuf)
{
UBYTE ret,obytes;

	ret = mifpro_ats(1,outbuf,&obytes);
	if(ret) return -1;
	
	return obytes;	
}

	
/*=================================================================================
函数：
功能：
===================================================================================*/
int mifpro_icmd0(UBYTE len,UBYTE *sbuf,UBYTE *outbuf2)
{
UBYTE ret;
UWORD obytes;

	ret = mifpro_icmd(sbuf,len,outbuf2,&obytes);
	if(ret) return -1;
	
	return obytes;	
}	


/*------------------------------------------------------
存交通卡记录，存储记录格式：3（流水号）＋4（卡号）		   
＋2（交易计数器）＋3（余额）＋4（TAC）		  

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
存薄型卡记录，存储记录格式：4（流水号）＋2（使用次数）
＋4（TAC）＋4（卡号）＋2（保留00）				    

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
存QR记录，存储记录格式：4（流水号）＋10（卡号）		   
＋8（divery）＋4（GMT）＋4（TAC）		  

-----------------------------------------------------*/
UBYTE save_qr_record(UBYTE *record)
{
	UWORD tint;
	int i;
	
#ifdef _DEBUG_BOM_
	debug_printf("\nsave_QR_record :");
	for(i = 0;i < 30;i++)
	{
		debug_printf("%02x ",record[i]);
	}
	debug_printf("\n");
#endif
	
	tint=ADDR_QR_REC;
	ee_write(tint,32,record);
	
	return 0;
}
/*------------------------------------------------------
存ODA记录，存储记录格式：3（流水号）＋2（发卡行代码）+10（卡号）		   
＋1（主账号序列号）＋4（TAC）		  

-----------------------------------------------------*/
UBYTE save_oda_record(UBYTE *record)
{
	UWORD tint;
	int i;
	
#ifdef _DEBUG_BOM_
	debug_printf("\nsave_oda_record...=");
	for(i = 0;i < 20;i++)
	{
		debug_printf("%02x ",record[i]);
	}
	debug_printf("\n");
#endif

	tint=ADDR_ODA_REC;
	ee_write(tint,32,record);
	
	return 0;
}

/*------------------------------------------------------
存交通部记录，存储记录格式：3（流水号）＋10（卡号）＋2（交易计数器）+1（密钥索引）
+密钥版本号+1（算法标识）+2（脱机交易序号）+4（PSAM流水号）＋3（余额）＋4（TAC）
2019-06-01	  
-----------------------------------------------------*/
UBYTE save_transport_record(UBYTE *record)
{
	UWORD tint;
	int i;
	
#ifdef _DEBUG_BOM_
	printf("\nsave_transport_record...=");
	for(i = 0;i < 31;i++)
	{
		printf("%02x ",record[i]);
	}
	printf("\n");
#endif

	tint=ADDR_TRANSPORT_REC;
	ee_write(tint,32,record);
	
	return 0;
}

/*------------------------------------------------------
根据流水号取交通卡记录，存储记录格式：3（流水号）＋4（卡号）		   
＋2（交易计数器）＋3（余额）＋4（TAC）	    
返回记录格式：4（卡号）＋2（交易计数器）＋3（余额）＋4（TAC）			 

-----------------------------------------------------*/
UBYTE get_trans_record(UBYTE *transptr,UBYTE *record_out,UBYTE ex_get)//3,13
{
UBYTE i;
UWORD  tint;

	for(i=0;i<8;i++)															//原程序 
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
取薄型卡记录，存储记录格式：4（流水号）＋2（使用次数）		   
＋4（TAC）＋4（卡号）＋2（保留00）			   
返回记录格式：2（使用次数）＋4（TAC）＋4（卡号）＋2（保留00）			 

-----------------------------------------------------*/
UBYTE get_sigal_record(UBYTE *transptr,UBYTE *record_out)
{
	UBYTE i;
	UWORD tint;
		for(i=0;i<8;i++)															//原程序 
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

/*==============================================================================
函数：get_qr_record
功能：取QR记录
================================================================================*/
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

/*==============================================================================
函数：get_oda_record
功能：取ODA记录
================================================================================*/
UBYTE get_oda_record(UBYTE *inbuf,UBYTE *record)
{
	UWORD tint;
	UBYTE rec[32];
	int i;

#ifdef _DEBUG_BOM_
	debug_printf("\nget_oda_record_inbuf=");
	for(i = 0;i < 16;i++)
	{
		debug_printf("%02x ",inbuf[i]);
	}
	debug_printf("\n");
#endif

	tint=ADDR_ODA_REC;
	ee_read(tint, 32, rec);

#ifdef _DEBUG_BOM_
	debug_printf("\nee_read_rec=");
	for(i = 0;i < 20;i++)
	{
		debug_printf("%02x ",rec[i]);
	}
	debug_printf("\n");
#endif

	if(memcmp(inbuf, rec, 16) == 0)
	{
		memcpy(record, &rec[16], 4);
		
		#ifdef _DEBUG_BOM_
		debug_printf("\nODA_record=");
		for(i = 0;i < 4;i++)
		{
			debug_printf("%02x ",record[i]);
		}
		debug_printf("\n");
		#endif
		
		return 0;
	}
	return OP_ERR;
}

/*==============================================================================
函数：get_transport_record
功能：取交通部记录
================================================================================*/
UBYTE get_transport_record(UBYTE *inbuf,UBYTE *record)
{
UWORD tint;
UBYTE rec[32];
	
	tint=ADDR_TRANSPORT_REC;
	ee_read(tint, 32, rec);

	if(memcmp(inbuf, rec, 15) == 0)
	{
		memcpy(record, rec, 31);
		return 0;
	}
	return OP_ERR;
}
/*==============================================================================
函数：gledantenna
功能：天线盘绿灯显示
================================================================================*/
void gledantenna(UBYTE option)
{
//实际---------------------------------------------------------------------------	
/*if(option == LED_OFF){gpd_clr_bit(BIT04_MASK);}
else{gpd_set_bit(BIT04_MASK);}
bgGState = option;*/
return;
}	
/*==============================================================================
函数：rledantenna
功能：天线盘红灯显示
================================================================================*/
void rledantenna(UBYTE option)
{
//实际---------------------------------------------------------------------------	
/*if(option == LED_OFF){gpd_clr_bit(BIT03_MASK);}
else{gpd_set_bit(BIT03_MASK);}
bgRState = option;*/
return;
}	
/*==============================================================================
函数：buzzerantenna
功能：天线盘蜂鸣器
================================================================================*/
void buzzerantenna(UBYTE option)
{
//实际---------------------------------------------------------------------------	
/*if(option == LED_OFF){gpd_clr_bit(BIT05_MASK);}
else{gpd_set_bit(BIT05_MASK);}
bgBuzzerState = option;*/
return;
}	

/*******************************************
函数：display_set
功能：天线盘LED显示
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
函数：buzzer_set
功能：蜂鸣器设置
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
函数名：tocken_read
功  能：读卡
入口参数:	无
出口参数:无
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
	
	if((buf[0]==0x03)&&(buf[1]==0x00))//复旦薄型卡
	{
			hh_key[4]=0x00;
			hh_key[5]=0x00;	
	}
	else if((buf[0]==0x00)&&(buf[1]==0x36))//华虹薄型卡		    
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
函数名：token_write
功  能：读卡
入口参数:	无
出口参数:无
===========================================================================*/
/*
char tocken_write(UBYTE addr, UBYTE *writedata)
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
	
	if((buf[0]==0x03)&&(buf[1]==0x00))//复旦薄型卡
	{
			hh_key[4]=0x00;
			hh_key[5]=0x00;	
	}
	else if((buf[0]==0x00)&&(buf[1]==0x36))//华虹薄型卡		    
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
函数:
功能:
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
	计算的过程如下：
		1. 把数据块pszBufIn划分为以8字节为单位的组；
		2. 如果最后一个数据块长度为8字节，那么后面添加80 00 00 00 00 00 00 00
		3. 如果最后一个数据块长度不是8字节，那么在后面添加80,如果还不够8字节，再添加00直到为8字节。
		4. 初始值pszInitData和第一个数据块异或校验，产生的值用密钥pszKey加密，
		5. 得到的值与第二个数据块异或校验，产生的值用密钥pszKey加密，
		6. 直到最后一个数据块异或校验，加密为止。
	
-------------------------------------------------------------------------*/
void	CmdWatchCalMac(UWORD nLenIn,UBYTE *pszBufIn,UBYTE *pszInitData,UBYTE *pszKey,UBYTE *pszMAC,UBYTE	bTriDes)
{
	UBYTE		pszBufAdd[8];
	UBYTE		pszBuf[8];
	UWORD		i,j;
	
	UWORD		nLenTotal;		//	补足最后一个数据块的长度。
	
	nLenTotal = (nLenIn/8+1)*8;		
	
	//	补足的部分
	memcpy(pszBufAdd,"\x80\x00\x00\x00\x00\x00\x00\x00",8);

	//	拷贝初始的值
	memcpy(pszBuf,pszInitData,8);
	
	for(i=0;i<nLenTotal;i=i+8) 
	{
		//	取出8个字节与中间值做异或运算
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
		
		//	DES运算,如果是3DES那么只用左半部分进行加密
		des_encode(pszKey,pszBuf,pszBuf);
	
	}
	
	//	如果是3DES，那么右半部分解密，左半部分加密
	if(bTriDes)
	{
		des_decode(pszKey+8,pszBuf,pszBuf);
		des_encode(pszKey,pszBuf,pszBuf);
	}
	
	//	得到8个字节，前面4个作为MAC.
	memcpy(pszMAC,pszBuf,4);

	return;
}

/*-------------------------------------------------------------------------
	Function Name: 	WatchDiversity
	Time:			2005-5-29 16:22:52
	Author:			Hsin Honway(xinhongwei@sina.com)
	Parameters:
					pszMKKey	[in]		16字节的主密钥
					pszPID		[in]		8字节的分散因子
					pszSKKey	[out]		16字节的分散结果
					bTriDes		[in]		DES或者3DES加密。
	Return:
	
	Remarks:
					握奇的密钥分散算法	
					2005-05-29日辛宏伟已经验证OK.	
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
		
		//	左边8字节
		des_encode(pszKey, pszPID, pszBuf);
		des_decode(pszKey+8, pszBuf,pszBuf+8);
		des_encode(pszKey, pszBuf+8, pszBuf);
		memcpy(pszSKKey,pszBuf,8);
		
		//	输入数据取反
		memset(pszBuf,0x00,16);			
		for(i=0;i<8;i++) pszBuf[i] =~pszPID[i];
		
		//	右边8字节
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
函数：mcmh_writewithout
功能：写密钥区，不回读
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
函数：mcmh_authkeyblock
功能：认证密钥区
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
函数：
功能：
=======================================================================================*/
int ISO7816CommandSend(int index,int ibytes,char* ibuf)
{
/*UBYTE inbuf[256];
UBYTE inbytes;
	
if(index >= MAX_SAM_INDEX) return -1;
if(ibytes < 5) return -1;	
if(ibytes > 251) return -1;	
	 
gSamConfig[index].timeout = 1000;
	 
//根据sam index选择CPU
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
//超时等待
timer_set(TIMER_CMD_PROCESS_INDEX,gSamConfig[index].timeout);*/
//
return 0;		
}	


/*=====================================================================================
函数：
功能：
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
函数：prot_jt_send_block
功能：
==============================================================================*/
void shmt_prot_send(UBYTE *buf, UWORD bytes)
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
	fseek(fp,0,SEEK_END);//将文件的当前位置移动到文件末尾
	filelen = ftell(fp);//获得当前位置相对于文件首的位移，该位移值等于文件所含字节数
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
	else
	{
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
	fseek(fp,0,SEEK_END);//将文件的当前位置移动到文件末尾
	filelen = ftell(fp);//获得当前位置相对于文件首的位移，该位移值等于文件所含字节数
	#ifdef DEBUG_PRINT
	printf("filelen=%X ",filelen);
	#endif
	
	temp_4020 = (char *)malloc(sizeof(char)*filelen+1);
	fseek(fp,0,SEEK_SET);
	fread(temp_4020,	1, filelen, fp);
//	if(((filelen-28) % 16) == 0)
//	{
//		buf[0] = (filelen-40) / 296;
//	}
//	else
//	{
//		buf[0] = (filelen-40) /296 + 1;
//	}
//	//printf(" recordlen = %02X", buf[0]);
//	memcpy(&temp_4020[40],&buf[0],1);
//	fread(&temp_4020[41],1, filelen-40, fp);
	fclose(fp);
	
#ifdef DEBUG_PRINT
	printf("temp_4020:");
	for(i=0;i<filelen;i++)
	{
		printf(" %02X ",temp_4020[i]);
	}
#endif
	return;
}
//end of file
#endif