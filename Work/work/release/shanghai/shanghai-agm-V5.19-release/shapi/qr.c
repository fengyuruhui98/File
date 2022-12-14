#ifndef QR_C
#define QR_C

unsigned char qrData[512], qrDataBak[512],qrDataEx[512], blnQR = 0;
unsigned char qr_consume_key[18];
unsigned char qr_consume_process_key[16];
unsigned char qrKeyBuf[100], sendMAC[20];
UDWORD	dwTranstime;
unsigned char transTime[7], curTime[7];
unsigned char QR_TYPE = 0;
int 		qr_family;	//0:MDH 1:JTB  2:SSM 	3:STM

#define DEBUG_QR

#define	DEBUG_NOCHECK
/*
UBYTE getinfo_QR(unsigned char *data)
{
	unsigned char buf[500], inbuf[300], outbuf[300];
	unsigned char inbytes, obytes, outbytes;
	int rtn, ret;
	long len;
	int i;
	
	memcpy(qrData, data, 88);

	//计算消费密钥
	//账号后8个字节
	memcpy(&qrKeyBuf[0], &qrData[9], 12);
	//卡类型
	qrKeyBuf[12] = qrData[27];
	//账号后2个字节
	memcpy(&qrKeyBuf[13], &qrData[15], 2);
	
	//
	if(memcmp(&qrData[1], "\xff\xff\xff\xff\xff\xff", 6) != 0x00)
	{
		bleOffline();
		startBLEOnline(qrData);
		blnQR = 0xff;
	}

	if(memcmp(bgSmpsamADF, "\x10\x01", 2) != 0)
	{
		memcpy(inbuf, "\x00\xa4\x00\x00\x02\x10\x01",7);
		inbytes = 7;
		ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
		print_debug(outbuf, outbytes);
		if(ret != 0)
		{
			gResponseStatus =CE_PSAMERROR;
			stopBLEOffline();
			blnQR = 0x00;
			goto label_err_key;
		}
		if(((UBYTE)outbuf[outbytes-2]!= (UBYTE)0x61) &&((UBYTE)outbuf[outbytes-2]!= 0x90))
		{
			gResponseStatus =CE_PSAMERROR;
			stopBLEOffline();
			blnQR = 0x00;
			goto label_err_key;
		}
		memcpy(bgSmpsamADF, "\x10\x01", 2);
	}
	memcpy(inbuf, "\x80\xfc\x10\x10\x0f", 5);
	memcpy(&inbuf[5], qrKeyBuf, 15);
	ret = sam_apdu(bgSmpsamIndex, inbuf, 20, outbuf, &obytes, 0, 0);
#ifdef	DEBUG_QR
	print_debug(inbuf, 20);
	printf("sam apdu return %d\n", ret);
	print_debug(outbuf, obytes);
#endif
	if(ret != 0) 
	{
		gResponseStatus =CE_PSAMERROR;
		stopBLEOffline();
		blnQR = 0x00;
		goto label_err_key;
	}
	if((outbuf[obytes-2]!=0x61)&&(outbuf[obytes-2]!=0x90))
	{
		gResponseStatus =CE_PSAMERROR;
		stopBLEOffline();
		blnQR = 0x00;
		goto label_err_key;
	}
	memcpy(qr_consume_key, outbuf, 18);

//	if(QRGetKeyAStep1(qrKeyBuf)!=0)
//	{
//		gResponseStatus =CE_AUTHENERROR;
//		stopBLEOffline();
//		goto label_err_key;
//	}	
//
//	if(QRGetKeyAStep2()!=0)
//	{
//#ifdef DEBUG_QR
//		printf("\n GET TOCKEN KEY ERROR\n");
//#endif
//		gResponseStatus =CE_AUTHENERROR;
//		stopBLEOffline();
//		goto label_err_key;
//	}
//	//一卡一密
//	memcpy(qr_consume_key, bgTaskKeyOutbuf, 16);
	//过程密钥
	WatchDiversity(qr_consume_key, &qrData[72], qr_consume_process_key, 0xff);
#ifdef DEBUG_QR
	printf("qr process key:");
	for(i = 0; i< 16; i++)
		printf("%02x ", qr_consume_process_key[i]);
	printf("\n");
#endif
	//
	memset(buf, 0x00, sizeof(buf));
	memcpy(&buf[8], qrData, 80);
	CmdWatchCalMac(80, &buf[8], buf, qr_consume_process_key, &buf[100], 0xff);
	if(memcmp(&buf[100], &qrData[80], 4) != 0)
	{
#ifdef DEBUG_QR
		printf("invalid card\n");
#endif
		stopBLEOffline();
		gResponseStatus =CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		blnQR = 0x00;
		goto label_err_key;
	}

	//应用区MAC计算
	memset(buf, 0x00, sizeof(buf));
	if(memcmp(&qrData[28], buf, 25) == 0)
		memset(&qrData[53], 0x00, 4);
	else 
	{
		memcpy(&buf[8], &qrData[28], 25);
		CmdWatchCalMac(25, &buf[8], buf, qr_consume_key, &buf[60], 0xff);
		if(memcmp(&buf[60], &qrData[53], 4) == 0)
			memset(&qrData[53], 0x00, 4);
		else
			memset(&qrData[53], 0xFF, 4);
	}
	
	buf[0] = QR_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = CE_OK;
	buf[3] = 0x81;
	
	memcpy(&buf[4], qrData, 80);
	shmt_prot_send(buf, 84);
	gStatus = TRANS_STATUS;
	
	return 0;
	
label_err_key:
	

	return OP_ERR;
}
*/

UBYTE getinfo_QR(unsigned char *data, long len)
{
	unsigned char buf[500], inbuf[300], outbuf[300];
	unsigned char inbytes, obytes, outbytes;
	int rtn, ret;
	int i;
	
	if(memcmp(bgSmpsamADF, "\x10\x01", 2) != 0)
	{
		memcpy(inbuf, "\x00\xa4\x00\x00\x02\x10\x01",7);
		inbytes = 7;
		ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
		print_debug(outbuf, outbytes);
		if(ret != 0)
		{
			gResponseStatus =CE_PSAMERROR;
			stopBLEOffline();
			blnQR = 0x00;
			return OP_ERR;
		}
		if(((UBYTE)outbuf[outbytes-2]!= (UBYTE)0x61) &&((UBYTE)outbuf[outbytes-2]!= 0x90))
		{
			gResponseStatus =CE_PSAMERROR;
			stopBLEOffline();
			blnQR = 0x00;
			return OP_ERR;
		}
		memcpy(bgSmpsamADF, "\x10\x01", 2);
	}
//	if(len == 84)
//		return getinfo_QR_V1(data);
//	else
//		return getinfo_QR_V2(data);
	if(len == 84)
	{
		qr_family = 0;
		QR_TYPE = 0;
#ifdef DEBUG_QR
		printf("\ngetinfo 1 QR_TYPE =%d\n",QR_TYPE);
		printf("QR(Ver1)..1:");
		for(i = 0; i < 128; i++)
		{
			printf("%02X ", data[i]);
		}
		printf("\n");
#endif
		return getinfo_QR_V1(data);
	}else if( (len >= 332) && ((data[0] >=0x80) && (data[0] <= 0xFF)))
	{
//		gResponseStatus =CE_INVALIDCARD;
//		return OP_ERR;
		qr_family = 1;
		return getinfo_QR_JTB(data, len);
	}else if( (memcmp(data, "https://s.sh.gov.cn/", 20) == 0) && (memcmp(&data[len - 2], "##", 2) == 0) )
	{
//		gResponseStatus =CE_INVALIDCARD;
//		return OP_ERR;
		qr_family = 2;
		return getinfo_QR_SSM(data, len);
	}else if( memcmp(data, "SHSTMETRO", 9) == 0)
	{
//		gResponseStatus =CE_INVALIDCARD;
//		return OP_ERR;
		qr_family = 3;
		return getinfo_QR_STM(data, len);
	}else if( len == 128 )
	{
		qr_family = 0;
		QR_TYPE = 1;
#ifdef DEBUG_QR
		printf("\ngetinfo 2 QR_TYPE =%d\n",QR_TYPE);
		printf("QR(Ver2)..1:");
		for(i = 0; i < 128; i++)
		{
			printf("%02X ", data[i]);
		}
		printf("\n");
#endif
		return getinfo_QR_V2(data);
	}else
	{
		gResponseStatus = CE_NOFUNCTION;
		return CE_NOFUNCTION;
	}	
}

UBYTE getinfo_QR_V1(unsigned char *data)
{
	unsigned char buf[500], inbuf[300], outbuf[300];
	unsigned char inbytes, obytes, outbytes;
	unsigned char bleData[100];

	int rtn, ret;
	long len;
	int i;

	memcpy(qrData, data, 88);
#ifdef DEBUG_QR
	printf("QR(Ver1)..2:");
	for(i = 0; i < 128; i++)
		printf("%02X ", qrData[i]);
	printf("\n");
#endif
	//设备类型
	qrData[84] = 0x00;
	if(ReadMode&0x01)
		qrData[84] = 0x01;
	
	//计算消费密钥
	//账号后8个字节
	memcpy(&qrKeyBuf[0], &qrData[9], 12);
	//卡类型
	qrKeyBuf[12] = qrData[27];
	//账号后2个字节
	memcpy(&qrKeyBuf[13], &qrData[15], 2);
	
	//
	if(memcmp(&qrData[1], "\xff\xff\xff\xff\xff\xff", 6) != 0x00)
	{
		//00 + ble Mac (6) + mac(4)
		memcpy(bleData, qrData, 7);
		memcpy(&bleData[7], &qrData[80], 4);
		startBLEOnline(bleData);

		blnQR = 0xff;
	}

//	if(QRGetKeyAStep1(qrKeyBuf)!=0)
//	{
//		gResponseStatus =CE_AUTHENERROR;
//		stopBLEOffline();
//		goto label_err_key;
//	}	
//
//	if(QRGetKeyAStep2()!=0)
//	{
//#ifdef DEBUG_QR
//		printf("\n GET TOCKEN KEY ERROR\n");
//#endif
//		gResponseStatus =CE_AUTHENERROR;
//		stopBLEOffline();
//		goto label_err_key;
//	}
//	//一卡一密
//	memcpy(qr_consume_key, bgTaskKeyOutbuf, 16);
	memcpy(inbuf, "\x80\xfc\x10\x10\x0f", 5);
	memcpy(&inbuf[5], qrKeyBuf, 15);
	ret = sam_apdu(bgSmpsamIndex, inbuf, 20, outbuf, &obytes, 0, 0);
#ifdef	DEBUG_QR
	print_debug(inbuf, 20);
	printf("sam apdu return %d\n", ret);
	print_debug(outbuf, obytes);
#endif
	if(ret != 0) 
	{
		gResponseStatus =CE_PSAMERROR;
		stopBLEOffline();
		blnQR = 0x00;
		goto label_err_key;
	}
	if((outbuf[obytes-2]!=0x61)&&(outbuf[obytes-2]!=0x90))
	{
		gResponseStatus =CE_PSAMERROR;
		stopBLEOffline();
		blnQR = 0x00;
		goto label_err_key;
	}
	memcpy(qr_consume_key, outbuf, 18);

	//过程密钥
	WatchDiversity(qr_consume_key, &qrData[72], qr_consume_process_key, 0xff);
#ifdef DEBUG_QR
	printf("qr process key:");
	for(i = 0; i< 16; i++)
		printf("%02x ", qr_consume_process_key[i]);
	printf("\n");
#endif
	//
	memset(buf, 0x00, sizeof(buf));
	memcpy(&buf[8], qrData, 80);
	CmdWatchCalMac(80, &buf[8], buf, qr_consume_process_key, &buf[100], 0xff);
	if(memcmp(&buf[100], &qrData[80], 4) != 0)
	{
#ifdef DEBUG_QR
		printf("invalid card, calMAC %02x%02x%02x%02x\n", buf[100], buf[101], buf[102], buf[103]);
#endif
		stopBLEOffline();
		gResponseStatus =CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		goto label_err_key;
	}

	//应用区MAC计算
	memset(buf, 0x00, sizeof(buf));
	if(memcmp(&qrData[28], buf, 25) == 0)
		memset(&qrData[53], 0x00, 4);
	else 
	{
		memcpy(&buf[8], &qrData[28], 25);
		CmdWatchCalMac(25, &buf[8], buf, qr_consume_key, &buf[60], 0xff);
		if(memcmp(&buf[60], &qrData[53], 4) == 0)
			memset(&qrData[53], 0x00, 4);
		else
			memset(&qrData[53], 0xFF, 4);
	}
	
	buf[0] = QR_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = CE_OK;
	buf[3] = 0x81;
	
	memcpy(&buf[4], qrData, 80);
	shmt_prot_send(buf, 84);
	gStatus = TRANS_STATUS;
	
	return 0;
	
label_err_key:	
	return OP_ERR;
}

UBYTE getinfo_QR_V2(unsigned char *data)
{
	unsigned char buf[500], buf1[500], inbuf[300], outbuf[300];
	unsigned char inbytes, obytes, outbytes;
	unsigned char bleData[100];
	int rtn, ret;
	long len;
	int i;
	
	memcpy(qrData, data, 128);
	memcpy(qrDataEx, &data[68], 44+16);
#ifdef DEBUG_QR
	printf("QR(Ver2)..2:");
	for(i = 0; i < 128; i++)
		printf("%02X ", qrData[i]);
	printf("\n");
#endif
	//设备类型
	qrData[84] = 0x00;
	if(ReadMode&0x01)
		qrData[84] = 0x01;
	
	//计算消费密钥
	//账号后8个字节
	memcpy(&qrKeyBuf[0], &qrData[9], 12);
	//卡类型
	qrKeyBuf[12] = qrData[27];
	//账号后2个字节
	memcpy(&qrKeyBuf[13], &qrData[15], 2);
	
	//
	if(memcmp(&qrData[1], "\xff\xff\xff\xff\xff\xff", 6) != 0x00)
	{
		//printf("0x84 %02x\n", qrData[84]);
		memcpy(bleData, qrData, 7);
		memcpy(&bleData[7], &qrData[124], 4);
		startBLEOnline(bleData);

		blnQR = 0xff;
	}

//	if(QRGetKeyAStep1(qrKeyBuf)!=0)
//	{
//		gResponseStatus =CE_AUTHENERROR;
//		stopBLEOffline();
//		goto label_err_key;
//	}	
//
//	if(QRGetKeyAStep2()!=0)
//	{
//#ifdef DEBUG_QR
//		printf("\n GET TOCKEN KEY ERROR\n");
//#endif
//		gResponseStatus =CE_AUTHENERROR;
//		stopBLEOffline();
//		goto label_err_key;
//	}
//	//一卡一密
//	memcpy(qr_consume_key, bgTaskKeyOutbuf, 16);
	memcpy(inbuf, "\x80\xfc\x10\x10\x0f", 5);
	memcpy(&inbuf[5], qrKeyBuf, 15);
	ret = sam_apdu(bgSmpsamIndex, inbuf, 20, outbuf, &obytes, 0, 0);
#ifdef	DEBUG_QR
	print_debug(inbuf, 20);
	printf("sam apdu return %d\n", ret);
	print_debug(outbuf, obytes);
#endif
	if(ret != 0) 
	{
		gResponseStatus =CE_PSAMERROR;
		if(blnQR)
			getBlEOnlineReturn(sendMAC);

		stopBLEOffline();
		blnQR = 0x00;
		goto label_err_key;
	}
	if((outbuf[obytes-2]!=0x61)&&(outbuf[obytes-2]!=0x90))
	{
		gResponseStatus =CE_PSAMERROR;
		if(blnQR)
			getBlEOnlineReturn(sendMAC);
		stopBLEOffline();
		blnQR = 0x00;
		goto label_err_key;
	}
	memcpy(qr_consume_key, outbuf, 18);

	//过程密钥
	WatchDiversity(qr_consume_key, &qrData[116], qr_consume_process_key, 0xff);
#ifdef DEBUG_QR
	printf("qr process key:");
	for(i = 0; i< 16; i++)
		printf("%02x ", qr_consume_process_key[i]);
	printf("\n");
#endif
	//
	memset(buf, 0x00, sizeof(buf));
	memcpy(&buf[8], qrData, 68);
	memcpy(&buf[76], qrDataEx, 60);
	CmdWatchCalMac(124, &buf[8], buf, qr_consume_process_key, &buf[400], 0xff);
#ifdef DEBUG_QR
	printf("buf[400-403] %02x %02x %02x %02x\n", buf[400], buf[401], buf[402], buf[403]);
#endif
	if(memcmp(&buf[400], &qrData[124], 4) != 0)
	{
#ifdef DEBUG_QR
		printf("invalid card, calMAC %02x %02x %02x %02x\n", qrData[124], qrData[125], qrData[126], qrData[127]);
#endif
		if(blnQR)
			getBlEOnlineReturn(sendMAC);

		stopBLEOffline();
		gResponseStatus =CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		goto label_err_key;
	}

	//应用区MAC计算
	memset(buf, 0x00, sizeof(buf));
	if(memcmp(&qrData[28], buf, 25) == 0)
		memset(&qrData[53], 0x00, 4);
	else 
	{
		memcpy(&buf[8], &qrData[28], 25);
		CmdWatchCalMac(25, &buf[8], buf, qr_consume_key, &buf[60], 0xff);
#ifdef DEBUG_QR
		printf("Application MAC buf[60-63] :%02x %02x %02x %02x\n",buf[60],buf[61],buf[62],buf[63]);
#endif
		if(memcmp(&buf[60], &qrData[53], 4) == 0)
			memset(&qrData[53], 0x00, 4);
		else
			memset(&qrData[53], 0xFF, 4);
	}
	buf[0] = QR_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = CE_OK;
	buf[3] = 0x01;
	//
	memcpy(&buf[4], qrData, 68);
	memcpy(&buf[72], &qrData[112], 12);
	

	//应用扩展区MAC计算
	memset(buf1, 0x00, sizeof(buf1));
	if(memcmp(&qrDataEx[0], buf1, 40) == 0)
		memset(&qrDataEx[40], 0x00, 4);
	else
	{
		//memcpy(&buf[8], &qrDataEx[0], 40);
		memcpy(&buf1[8], &qrDataEx[0], 16);
#ifdef DEBUG_QR
		printf("\nqrDataEx :");
		for(i=0;i<16;i++)
		{
			printf(" %02x",qrDataEx[i]);
		}
		printf("\n");
#endif
		CmdWatchCalMac(40, &buf1[8], buf1, qr_consume_key, &buf1[60], 0xff);
#ifdef DEBUG_QR
		printf("KuoZhanQu MAC buf[60-63] %02x %02x %02x %02x\n",buf1[60],buf1[61],buf1[62],buf1[63]);
		printf("qrDataEx[40-43] %02x %02x %02x %02x\n",qrDataEx[40],qrDataEx[41],qrDataEx[42],qrDataEx[43]);
#endif
		if (memcmp(&buf1[60], &qrDataEx[40], 4) == 0)
			memset(&qrDataEx[40], 0x00, 4);
		else
			memset(&qrDataEx[40], 0xFF, 4);
	}
	
	buf1[0] = QR_READCARD;
	buf1[1] = gReadCmdTimes;
	buf1[2] = CE_OK;
	buf1[3] = 0x82;
	
	memcpy(&buf1[4], qrDataEx, 44);

	shmt_prot_send(buf, 84);
	shmt_prot_send(buf1, 48);

	gStatus = TRANS_STATUS;
	
	return 0;
	
label_err_key:
	return OP_ERR;
}

void print_debug(unsigned char *inbuf, unsigned char len)
{
unsigned char i;

	printf("=====\n");
	for(i = 0; i < len; i++)
		printf("%02x ", inbuf[i]);
	printf("\n");
}
/****************************************************************************
函数：qr_trans_exit
功能：二维码出站			 
****************************************************************************/
UBYTE qr_trans_exit(unsigned char *inbuf, unsigned char *outbuf)
{
UBYTE buf[200], ble[500], zero[8];
UWORD temp, lastAmount;
UBYTE recBuf[100];
	//
UBYTE resp_buf[200], bleResult;
int rtn, len,i;

	memset(zero, 0x00, sizeof(zero));
	//计算TAC
	memcpy(qrKeyBuf, &qrData[9], 8);
	memcpy(&qrKeyBuf[8], &inbuf[2], 4);
	qrKeyBuf[12] = 0x00;			//交易前余额,高在前
	qrKeyBuf[13] = 0x00;
	qrKeyBuf[14]= inbuf[17];	//交易金额,高在前
	qrKeyBuf[15]= inbuf[16];
	//上次交易站点
	//上次车站根据交易标志
	if((qrData[28] & 0xC0) == 0x40)
	{
		qrKeyBuf[16]= qrData[29];
		qrKeyBuf[17]= qrData[30];
	}else
	{
		qrKeyBuf[16] = qrData[37];
		qrKeyBuf[17] = qrData[38];
	}
	//
	memcpy(&qrKeyBuf[18], &inbuf[8],4);
	memcpy(&qrKeyBuf[22], &inbuf[12], 4);
	memcpy(&qrKeyBuf[26],bpgSmpsamNo,4);
	qrKeyBuf[30]= 0x80;
	qrKeyBuf[31]= 0x00;
#ifdef	DEBUG_QR	
	print_debug(qrKeyBuf, 32);
#endif
	//备份应用信息区数据
	memcpy(&qrDataBak[28], &qrData[28], 40);
	if(memcmp(&qrData[1], "\xff\xff\xff\xff\xff\xff", 6) != 0x00)
	{//组包蓝牙数据
		if( !blnQR)
		{
			gResponseStatus=CE_BADCOMMAND;
			blnQR = 0;
			return OP_ERR;
		}
		memset(buf,0,sizeof(buf));//20190813
		memcpy(buf, qrData, 84);
		//交易标志
		if(qrData[28] & 0x20)
			buf[28] = 0xA0;
		else
			buf[28] = 0x81;
		//device id
		memcpy(&buf[37], &inbuf[8], 4);
		//transaction time
		memcpy(&buf[41], &inbuf[2], 4);
		//transaction amount
		memcpy(&lastAmount, &qrData[45], 2);
		memcpy(&temp, &inbuf[16], 2);
		temp += lastAmount;
		memcpy(&buf[45], &temp, 2);
		//根据当前时间是否清0
		dwTranstime = (inbuf[2] << 24) + (inbuf[3] << 16) + (inbuf[4] << 8) + inbuf[5];
		dwTranstime -= TIME2000;
		long2timestr(dwTranstime, curTime);
#ifdef DEBUG_QR
		printf("\nExit command time :%08X\n", dwTranstime);
		print_debug(curTime, 7);
#endif
		dwTranstime = (qrData[33] << 24) + (qrData[34] << 16) + (qrData[35] << 8) + qrData[36];
		dwTranstime -= TIME2000;
		long2timestr(dwTranstime, transTime);
#ifdef DEBUG_QR
		printf("Exit last exit time :%08X\n", dwTranstime);
		print_debug(transTime, 7);
#endif
		if(memcmp(curTime, transTime, 2) != 0)
		{
			//累计金额及次数：跨月时先清0，在累计
			memset(&buf[47], 0x00, 6);
			memcpy(&buf[47],&inbuf[16],2);
			buf[49] = 0;
			buf[50]	=	0;
			buf[51] = 0;
			buf[52]	=	1;
		}
		else
		{	
			//累计金额
			memcpy(&dwTranstime, &qrData[47], 4);
			memcpy(&temp, &inbuf[16], 2);
			dwTranstime += temp;
			memcpy(&buf[47], &dwTranstime, 4);
			//累计
			temp = (qrData[51] << 8) + qrData[52];
			temp += 1;
			buf[51] = (unsigned char)(temp >> 8);
			buf[52] = (unsigned char)temp;
		}
/*//根据当前时间是否清0
		dwTranstime = (inbuf[2] << 24) + (inbuf[3] << 16) + (inbuf[4] << 8) + inbuf[5];
		dwTranstime -= TIME2000;
		long2timestr(dwTranstime, curTime);
#ifdef DEBUG_QR
		printf("\nExit command time :%08X\n", dwTranstime);
		print_debug(curTime, 7);
#endif
		dwTranstime = (qrData[33] << 24) + (qrData[34] << 16) + (qrData[35] << 8) + qrData[36];
		dwTranstime -= TIME2000;
		long2timestr(dwTranstime, transTime);
#ifdef DEBUG_QR
		printf("Exit last exit time :%08X\n", dwTranstime);
		print_debug(transTime, 7);
#endif
		if(memcmp(curTime, transTime, 2) != 0)
		{
			memset(&buf[47], 0x00, 6);
		}*/
		//计算应用区MAC
		memset(ble, 0x00, sizeof(ble));
		CmdWatchCalMac(25, &buf[28], ble, qr_consume_key, &buf[53], 0xff);
		
		//当日使用次数（出站换乘只加1次）	
#ifdef DEBUG_QR
		printf("\nExit 1 QR_TYPE =:%d\n",QR_TYPE);
#endif
		if(QR_TYPE == 1)
		{
			if( memcmp(curTime, transTime, 3) != 0)
			{//非本日，当日使用次数清0
				buf[67] = 0;
#ifdef DEBUG_QR
				printf("\nExit buf[67] 1:%d",buf[67]);
#endif
			}
			if((qrData[28] & 0x10)==0)//非换乘进站
			{
				buf[67] += 1;
#ifdef DEBUG_QR
				printf("\nExit buf[67] 2:%d",buf[67]);
#endif
			}	
			//QR_TYPE = 0;		
		}	
		else
		{
			//buf[67] = 0;                         zdd2022.01.14
		}
		
		//memset(ble,0x00,sizeof(ble));
		ble[0] = 0x00;
		ble[1] = 58;
		//命令字
		ble[2] = 0x31;
		//二维码命令代码
		ble[3] = 0x6B;
		//当前终端设备编号
		memcpy(&ble[4], &inbuf[8], 4);
		//终端设备流水号
		memcpy(&ble[8], &inbuf[12], 4);
		//交易时间
		memcpy(&ble[12], &inbuf[2], 4);
		//
		memcpy(&ble[16], &buf[28], 40);
		//过程密钥计算通信mac
		CmdWatchCalMac(53, &ble[3], zero, qr_consume_process_key, &ble[56], 0xff);
		
		//rtn = bleOnline(qrData, 84);
		rtn = getBlEOnlineReturn(sendMAC);
		if(rtn)
		{
			stopBLEOffline();
			if( rtn == 0x404B )
			{
				gResponseStatus= CE_BLUENOADDRESS;
			}else if( rtn == 0x404C )
			{
				gResponseStatus=CE_BLUEUNONLIE;
			}else
				gResponseStatus=CE_BLUEOVERTIME;
			blnQR = 0;
			return OP_ERR;
		}
		//检查链接返回的MAC:双向认证数据无应答码，得到的的应答数据
		if(memcmp(sendMAC, &qrData[17], 4) != 0)
		{
#ifdef	DEBUG_NOCHECK
			stopBLEOffline();
			gResponseStatus=CE_AUTHENERROR;
			blnQR = 0;
			return OP_ERR;
#endif
		}
		CmdWatchCalMac(4, &sendMAC[0], zero, qr_consume_process_key, &sendMAC[10], 0xff);
		if(memcmp(&sendMAC[4], &sendMAC[10], 4) != 0)
		{
#ifdef	DEBUG_NOCHECK
			stopBLEOffline();
			gResponseStatus=CE_AUTHENERROR;
			blnQR = 0;
			return OP_ERR;
#endif
		}
#ifdef DEBUG_QR
		printf("\nble :");
		for(i=0;i<60;i++)
		{
			printf(" %02x",ble[i]);
		}
		printf("\n");
#endif
		sendBLEData(ble, 60);
	}
	SHQRTACStep1(qrKeyBuf);
	
#ifdef DEBUG_TIME
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x0a", 3);
#endif	
	//开始交易
	if(memcmp(&qrData[1], "\xff\xff\xff\xff\xff\xff", 6) == 0x00)
	{//无蓝牙模块
		memset(outbuf, 0x00, 12);
	}
	else
	{//蓝牙交互
		//链接蓝牙

		//发送数据
		//rtn = bleSend(ble, 58 + 2, resp_buf, &len);
		//一般发送数据有应答码，返回的是LV的完整数据
		rtn = getBLESendReturn(resp_buf, &len);
#ifdef DEBUG_QR
		printf("\nrtn =%d",rtn);
#endif
		if(rtn)
		{
			stopBLEOffline();
			gResponseStatus=CE_CARDREMOVED;
			//获取TAC
			if(SHQRTACStep2()!=0)		//取TAC
			{
				gResponseStatus = CE_PSAMERROR;
				blnQR = 0;
				return OP_ERR;
			}
			memcpy(gTAC,bgTaskKeyOutbuf,4);				
			
			memcpy(&recBuf[0], &inbuf[12], 4);
			memcpy(&recBuf[4], &qrData[7], 10);
			if(QR_TYPE == 1)
			{
				memcpy(&recBuf[14], &qrData[116], 8);
				QR_TYPE = 0;
			}
			else
			{
				memcpy(&recBuf[14], &qrData[72], 8);//分散因子
			}
#ifdef DEBUG_QR
			printf("\nExit 2 QR_TYPE =%d",QR_TYPE);
#endif
			memcpy(&recBuf[22], &resp_buf[5], 4);
			memcpy(&recBuf[26], gTAC, 4);
			save_qr_record(recBuf);
			blnQR = 0;
			return OP_ERR;
		}
		//关闭蓝牙
		//rtn = bleOffline();
		stopBLEOffline();
		//检查应答代码
		if(resp_buf[3] != 0)
		{
			gResponseStatus = CE_WRITEERROR;
			blnQR = 0;
			return OP_ERR;
		}
		memcpy(outbuf, &resp_buf[4], 10);
	}

	//交易成功，备份账号信息
	memcpy(qrDataBak, qrData, 25);
	//获取TAC
	if(SHQRTACStep2()!=0)												//取TAC
	{
		gResponseStatus = CE_SAMERROR;
		blnQR = 0;
		return OP_ERR;
	}
	memcpy(gTAC,bgTaskKeyOutbuf,4);				
	
	memcpy(&recBuf[0], &inbuf[12], 4);
	memcpy(&recBuf[4], &qrData[7], 10);
#ifdef DEBUG_QR
	printf("\nExit 2 QR_TYPE =:%d",QR_TYPE);
#endif
	if(QR_TYPE == 1)
	{
		memcpy(&recBuf[14], &qrData[116], 8);
		QR_TYPE = 0;
	}
	else
	{
		memcpy(&recBuf[14], &qrData[72], 8);
	}
	memcpy(&recBuf[22], &resp_buf[5], 4);
	memcpy(&recBuf[26], gTAC, 4);
#ifdef DEBUG_QR
	printf("\nExit 3 QR_TYPE =:%d\n",QR_TYPE);
	printf("\nExit OVER!\n");
#endif
	save_qr_record(recBuf);
	blnQR = 0;
	return 0;
}


UBYTE qr_trans_entry(unsigned char *inbuf, unsigned char *outbuf)
{
UBYTE buf[200], ble[500], zero[8];
UWORD temp;
//
UBYTE resp_buf[200], recBuf[100];
int rtn, len;
int i;

	
	memset(zero, 0x00, sizeof(zero));
	//计算TAC数据
	memcpy(qrKeyBuf, &qrData[9], 8);
	memcpy(&qrKeyBuf[8],&inbuf[2],4);
	qrKeyBuf[12] = 0x00;			//交易前余额,高在前
	qrKeyBuf[13] = 0x00;
	qrKeyBuf[14] = 0x00;			//交易金额,高在前
	qrKeyBuf[15 ]= 0x00;
	//上次车站根据交易标志
	if((qrData[28] & 0xC0) == 0x40)
	{
		qrKeyBuf[16]= qrData[29];
		qrKeyBuf[17]= qrData[30];
	}else
	{
		qrKeyBuf[16] = qrData[37];
		qrKeyBuf[17] = qrData[38];
	}
	//device id
	memcpy(&qrKeyBuf[18], &inbuf[8],4);
	memcpy(&qrKeyBuf[22], &inbuf[12], 4);
	memcpy(&qrKeyBuf[26],bpgSmpsamNo,4);
	qrKeyBuf[30]= 0x80;
	qrKeyBuf[31]= 0x00;
	
	//print_debug(qrKeyBuf, 32);
	//备份应用信息区数据
	memcpy(&qrDataBak[28], &qrData[28], 40);
	if(memcmp(&qrData[1], "\xff\xff\xff\xff\xff\xff", 6) != 0x00)
	{
		if( !blnQR)
		{
			gResponseStatus=CE_BADCOMMAND;
			blnQR = 0;
			return OP_ERR;
		}
	//组包蓝牙数据
		memset(buf,0,sizeof(buf));//20190813
		memcpy(buf, qrData, 84);
		//交易标志
		buf[28] = 0x41;
		//device id
		memcpy(&buf[29], &inbuf[8], 4);
		//transaction time
		memcpy(&buf[33], &inbuf[2], 4);
		//transaction amount
		memset(&buf[45], 0x00, 2);
		//
//		if(inbuf[16] & 0x8)
//		{
//			buf[28] &= 0x10;
//			buf[28] &= (qrData[28] & 0xC0);
//			memcpy(&buf[29], &qrData[29], 8);
//			memcpy(&buf[45], &qrData[45], 2);
//		}
		if (inbuf[16] == 0x00)
		{
			buf[28] = 0x41;
		}else
		{ 
			if(inbuf[16] == 0x18)
			{
				if(qrData[28] & 0x08)
					buf[28] = 0x78;
				else
					buf[28] = 0x71;
			}else 
			{
				buf[28] = inbuf[16] << 1;
				buf[28] += 0x40;
				if(buf[28] == 0x50)
					if(qrData[28] & 0x08)
						buf[28] = 0x59;
			}
			if(inbuf[16] & 0x08)
			{
				memcpy(&buf[29], &qrData[29], 8);
				memcpy(&buf[45], &qrData[45], 2);
			}
		}
		//根据当前时间是否清0
		dwTranstime = (inbuf[2] << 24) + (inbuf[3] << 16) + (inbuf[4] << 8) + inbuf[5];
		dwTranstime -= TIME2000;
		long2timestr(dwTranstime, curTime);
#ifdef DEBUG_QR
		printf("\nEntry command time :%08X\n", dwTranstime);
		print_debug(curTime, 7);
#endif	
		dwTranstime = (qrData[41] << 24) + (qrData[42] << 16) + (qrData[43] << 8) + qrData[44];
		dwTranstime -= TIME2000;
		long2timestr(dwTranstime, transTime);
#ifdef DEBUG_QR
		printf("Entry last exit time :%08X\n", dwTranstime);
		print_debug(transTime, 7);
#endif	
		if(memcmp(curTime, transTime, 2) != 0)
		{
			memset(&buf[47], 0x00, 6);
		}
		//计算应用区MAC
		memset(ble, 0x00, sizeof(ble));
		CmdWatchCalMac(25, &buf[28], ble, qr_consume_key, &buf[53], 0xff);
		if(QR_TYPE == 1)                        //20220301 zdd add
		{
			if( memcmp(curTime, transTime, 3) != 0)              //zdd2022.01.14changeif( (memcmp(curTime, transTime, 3) != 0)||(QR_TYPE != 1))
			{//非本日，当日使用次数清0
				buf[67] = 0;
				#ifdef DEBUG_QR
					printf("Entry buf[67] :%d\n",buf[67]);
				#endif
			}
		}


		ble[0] = 0x00;
		ble[1] = 58;
		//命令字
		ble[2] = 0x31;
		//二维码命令代码
		ble[3] = 0x6A;
		//当前终端设备编号
		memcpy(&ble[4], &inbuf[8], 4);
		//终端设备流水号
		memcpy(&ble[8], &inbuf[12], 4);
		//交易时间
		memcpy(&ble[12], &inbuf[2], 4);
		//
		memcpy(&ble[16], &buf[28], 40);
		//过程密钥计算通信mac
		CmdWatchCalMac(53, &ble[3], zero, qr_consume_process_key, &ble[56], 0xff);
		
		//chenck online
		rtn = getBlEOnlineReturn(sendMAC);
		
#ifdef DEBUG_QR
		print_debug(sendMAC, 10);
#endif
		if(rtn)
		{
			stopBLEOffline();
			if( rtn == 0x404B )
			{
				gResponseStatus= CE_BLUENOADDRESS;
			}else if( rtn == 0x404C )
			{
				gResponseStatus=CE_BLUEUNONLIE;
			}else
				gResponseStatus=CE_BLUEOVERTIME;
			blnQR = 0x00;
			return OP_ERR;
		}
		//检查链接返回的MAC
		if(memcmp(sendMAC, &qrData[17], 4) != 0)
		{
#ifdef DEBUG_NOCHECK
			stopBLEOffline();
			gResponseStatus=CE_AUTHENERROR;
			blnQR = 0x00;
			return OP_ERR;
#endif
		}
		CmdWatchCalMac(4, &sendMAC[0], zero, qr_consume_process_key, &sendMAC[10], 0xff);
		if(memcmp(&sendMAC[4], &sendMAC[10], 4) != 0)
		{
#ifdef DEBUG_NOCHECK
			stopBLEOffline();
			gResponseStatus=CE_AUTHENERROR;
			blnQR = 0x00;
			return OP_ERR;
#endif
		}
		
#ifdef DEBUG_QR
		printf("entry process key:");
		for(i = 0; i < 16; i++)
			printf("%02X", qr_consume_process_key[i]);
		printf("\n");
		printf("mac data:");
		for(i = 0; i < 60; i++)
			printf("%02X", ble[ i]);
		printf("\n");
#endif
		sendBLEData(ble, 60);
	}
#ifdef DEBUG_QR
	print_debug(qrKeyBuf, 32);
#endif
	//
	SHQRTACStep1(qrKeyBuf);
	
#ifdef DEBUG_TIME
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x0a", 3);
#endif	
	//开始交易
	if(memcmp(&qrData[1], "\xff\xff\xff\xff\xff\xff", 6) != 0x00)
	{//蓝牙交互
		//链接蓝牙
		//rtn = bleOnline(qrData, 84);
		//发送数据
		//rtn = bleSend(ble, 58 + 2, resp_buf, &len);
		rtn = getBLESendReturn(resp_buf, &len);
		if(rtn)
		{
			stopBLEOffline();
			gResponseStatus=CE_CARDREMOVED;
			if(SHQRTACStep2()!=0)												//取TAC
			{
				gResponseStatus = CE_SAMERROR;
				blnQR = 0x00;
				return OP_ERR;
			}
			memcpy(gTAC,bgTaskKeyOutbuf,4);				
		
			memcpy(&recBuf[0], &inbuf[12], 4);
			memcpy(&recBuf[4], &qrData[7], 10);
			if(QR_TYPE == 1)
			{
				memcpy(&recBuf[14], &qrData[116], 8);
				QR_TYPE = 0;
			}
			else
			{
				memcpy(&recBuf[14], &qrData[72], 8);
			}
			memcpy(&recBuf[22], &resp_buf[5], 4);
			memcpy(&recBuf[26], gTAC, 4);
			save_qr_record(recBuf);
			blnQR = 0x00;
			return OP_ERR;
		}
		//关闭蓝牙
		//rtn = bleOffline();
		stopBLEOffline();
		//检查应答代码
		if(resp_buf[3] != 0)
		{
			gResponseStatus = CE_WRITEERROR;
			blnQR = 0x00;
			return OP_ERR;
		}
		memcpy(outbuf, &resp_buf[4], 10);
	}

	//交易成功，备份账号信息
	memcpy(qrDataBak, qrData, 25);
	//获取TAC
	if(SHQRTACStep2()!=0)												//取TAC
	{
		gResponseStatus = CE_PSAMERROR;
		blnQR = 0x00;
		return OP_ERR;
	}
	memcpy(gTAC,bgTaskKeyOutbuf,4);				

	memcpy(&recBuf[0], &inbuf[12], 4);//流水号
	memcpy(&recBuf[4], &qrData[7], 10);//账户号
#ifdef DEBUG_QR
	printf("\nEntry 1 QR_TYPE =%d\n",QR_TYPE);
#endif
	if(QR_TYPE == 1)
	{
		memcpy(&recBuf[14], &qrData[116], 8);//分散因子
		QR_TYPE = 0;
	}
	else
	{
		memcpy(&recBuf[14], &qrData[72], 8);//分散因子
	}
#ifdef DEBUG_QR
	printf("Entry 2 QR_TYPE =%d\n",QR_TYPE);
#endif
	memcpy(&recBuf[22], &resp_buf[5], 4);//手机当前时间
	memcpy(&recBuf[26], gTAC, 4);//TAC
#ifdef DEBUG_QR
	debug_printf("\nsave_QR_recBuf :");
	for(i = 0;i < 30;i++)
	{
		debug_printf("%02x ",recBuf[i]);
	}
	debug_printf("\n");
#endif
	save_qr_record(recBuf);
	blnQR = 0x00;
	
	return 0;
}


UBYTE qr_trans_update(unsigned char *inbuf, unsigned char *outbuf)
{
UBYTE buf[200], ble[500], zero[8];
UWORD temp;
	//
UBYTE resp_buf[200], recBuf[100];
int rtn, len;

	
	if( !blnQR)
	{
		gResponseStatus=CE_BADCOMMAND;
		blnQR = 0;
		return OP_ERR;
	}
	memset(zero, 0x00, sizeof(zero));
	//计算TAC
	//
	memcpy(qrKeyBuf, &qrData[9], 8);
	//
	memcpy(&qrKeyBuf[8],gTransTime,4);
	//交易前余额,高在前
	qrKeyBuf[12] = 0x00;			
	qrKeyBuf[13] = 0x00;
	//交易金额,高在前
	qrKeyBuf[14]= inbuf[18];				
	qrKeyBuf[15]= inbuf[17];
	//上次车站
	if( (qrData[28] & 0xC0) == 0x40)
	{
		qrKeyBuf[16] = qrData[29];
		qrKeyBuf[17] = qrData[30];
	}else
	{
		qrKeyBuf[16]= qrData[37];
		qrKeyBuf[17]= qrData[38];
	}
	//device id
	memcpy(&qrKeyBuf[18], &inbuf[8],4);
	//
	memcpy(&qrKeyBuf[22], &inbuf[12], 4);
	memcpy(&qrKeyBuf[26],bpgSmpsamNo,4);
	qrKeyBuf[30]= 0x80;
	qrKeyBuf[31]= 0x00;
	
	if(memcmp(&qrData[1], "\xff\xff\xff\xff\xff\xff", 6) != 0x00)
	{
		memcpy(buf, qrData, 84);
		//交易标志
		memcpy(&buf[28], &inbuf[19], 25);
		//计算应用区MAC
		memset(ble, 0x00, sizeof(ble));
		CmdWatchCalMac(25, &buf[28], ble, qr_consume_key, &buf[53], 0xff);
//		//链接蓝牙
//		rtn = bleOnline(qrData, 84, NULL);
//		if(rtn)
//		{
//			gResponseStatus=CE_WRITEERROR;
//			return OP_ERR;
//		}
		ble[0] = 0x00;
		ble[1] = 58;
		//命令字
		ble[2] = 0x31;
		//
		ble[3] = 0x6C;
		memcpy(&ble[4], &inbuf[8], 4);
		memcpy(&ble[8], &inbuf[12], 4);
		memcpy(&ble[12], &inbuf[2], 4);
		//
		memcpy(&ble[16], &buf[28], 40);
		//过程密钥计算通信mac
		CmdWatchCalMac(53, &ble[3], zero, qr_consume_process_key, &ble[56], 0xff);

		rtn = getBlEOnlineReturn(sendMAC);
		//print_debug(sendMAC, 10);
		if(rtn)
		{
			stopBLEOffline();
			if( rtn == 0x404B )
			{
				gResponseStatus= CE_BLUENOADDRESS;
			}else if( rtn == 0x404C )
			{
				gResponseStatus=CE_BLUEUNONLIE;
			}else
				gResponseStatus=CE_BLUEOVERTIME;
			return OP_ERR;
		}
		//检查链接返回的MAC
		if(memcmp(sendMAC, &qrData[17], 4) != 0)
		{
#ifdef DEBUG_NOCHECK
			stopBLEOffline();
			gResponseStatus=CE_AUTHENERROR;
			return OP_ERR;
#endif
		}
		CmdWatchCalMac(4, &sendMAC[0], zero, qr_consume_process_key, &sendMAC[10], 0xff);
		if(memcmp(&sendMAC[4], &sendMAC[10], 4) != 0)
		{
			stopBLEOffline();
			gResponseStatus=CE_AUTHENERROR;
			return OP_ERR;
		}
		//双向认证通过时更新应用区
		sendBLEData(ble, 60);
	}
	SHQRTACStep1(qrKeyBuf);
	
#ifdef DEBUG_TIME
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x0a", 3);
#endif	
	//开始交易
	if(memcmp(&qrData[1], "\xff\xff\xff\xff\xff\xff", 6) != 0x00)
	{//蓝牙交互
		rtn = getBLESendReturn(resp_buf, &len);
		if(rtn)
		{
			stopBLEOffline();
			gResponseStatus=CE_CARDREMOVED;
			if(SHQRTACStep2()!=0)												//取TAC
			{
				gResponseStatus = CE_PSAMERROR;
				return OP_ERR;
			}
			memcpy(gTAC,bgTaskKeyOutbuf,4);				
		
			memcpy(&recBuf[0], &inbuf[12], 4);
			memcpy(&recBuf[4], &qrData[7], 10);
			memcpy(&recBuf[14], &qrData[72], 8);
			memcpy(&recBuf[22], &resp_buf[5], 4);
			memcpy(&recBuf[26], gTAC, 4);
			save_qr_record(recBuf);
			return OP_ERR;
		}
		stopBLEOffline();
		//检查应答代码
		if(resp_buf[3] != 0)
		{
			gResponseStatus = CE_WRITEERROR;
			return OP_ERR;
		}
		memcpy(outbuf, &resp_buf[4], 10);
	}

	//获取TAC
	if(SHQRTACStep2()!=0)			//取TAC
	{
		gResponseStatus = CE_PSAMERROR;
		return OP_ERR;
	}
	memcpy(gTAC,bgTaskKeyOutbuf,4);				

	memcpy(&recBuf[0], &inbuf[12], 4);
	memcpy(&recBuf[4], &qrData[7], 10);
	memcpy(&recBuf[14], &qrData[72], 8);
	memcpy(&recBuf[22], &resp_buf[5], 4);
	memcpy(&recBuf[26], gTAC, 4);
	save_qr_record(recBuf);
	return 0;
}


UBYTE qr_cal_auth(unsigned char *in_buf, unsigned char *out_buf)
{
unsigned char buf[500], inbuf[300], outbuf[300];
unsigned char inbytes, obytes, outbytes;
int rtn, ret;
long len;
int i;
	
	if(memcmp(bgSmpsamADF, "\x10\x01", 2) != 0)
	{
		memcpy(inbuf, "\x00\xa4\x00\x00\x02\x10\x01",7);
		inbytes = 7;
		ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
		print_debug(outbuf, outbytes);
		if(ret != 0)
		{
			gResponseStatus =CE_PSAMERROR;
			blnQR = 0x00;
			return OP_ERR;
		}
		if(((UBYTE)outbuf[outbytes-2]!= (UBYTE)0x61) &&((UBYTE)outbuf[outbytes-2]!= 0x90))
		{
			gResponseStatus =CE_PSAMERROR;
			blnQR = 0x00;
			return OP_ERR;
		}
		memcpy(bgSmpsamADF, "\x10\x01", 2);
	}
	
	//
	memcpy(inbuf, "\x80\x1A\x08\x10\x00", 5);
	ret = sam_apdu(bgSmpsamIndex,inbuf,5,outbuf,&outbytes,0,0);
	if(ret != 0)
	{
		gResponseStatus =CE_PSAMERROR;
		blnQR = 0x00;
		return OP_ERR;
	}
	print_debug(outbuf, outbytes);
	if(((UBYTE)outbuf[outbytes-2]!= (UBYTE)0x61) &&((UBYTE)outbuf[outbytes-2]!= 0x90))
	{
		gResponseStatus =CE_PSAMERROR;
		blnQR = 0x00;
		return OP_ERR;
	}
	//
	memcpy(inbuf, "\x80\xfa\x00\x00\x08", 5);
	memcpy(&inbuf[5], &in_buf[4], 8);
	print_debug(inbuf, 13);
	ret = sam_apdu(bgSmpsamIndex,inbuf,13,outbuf,&outbytes,0,0);
	print_debug(outbuf, outbytes);
	if(ret != 0)
	{
		gResponseStatus =CE_PSAMERROR;
		blnQR = 0x00;
		return OP_ERR;
	}
	if(((UBYTE)outbuf[outbytes-2]!= (UBYTE)0x61) &&((UBYTE)outbuf[outbytes-2]!= 0x90))
	{
		gResponseStatus =CE_PSAMERROR;
		blnQR = 0x00;
		return OP_ERR;
	}
	
	memcpy(out_buf, &outbuf[0], 4);
	return 0;
}

UBYTE qr_cal_process_mac(unsigned char *in_buf, unsigned char *out_buf)
{
unsigned char buf[500], inbuf[300], outbuf[300];
unsigned char inbytes, obytes, outbytes;
int rtn, ret;
long len;
	
	if(memcmp(bgSmpsamADF, "\x10\x01", 2) != 0)
	{
		memcpy(inbuf, "\x00\xa4\x00\x00\x02\x10\x01",7);
		inbytes = 7;
		ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
		print_debug(outbuf, outbytes);
		if(ret != 0)
		{
			gResponseStatus =CE_PSAMERROR;
			blnQR = 0x00;
			return OP_ERR;
		}
		if(((UBYTE)outbuf[outbytes-2]!= (UBYTE)0x61) &&((UBYTE)outbuf[outbytes-2]!= 0x90))
		{
			gResponseStatus =CE_PSAMERROR;
			blnQR = 0x00;
			return OP_ERR;
		}
		memcpy(bgSmpsamADF, "\x10\x01", 2);
	}
	//
	memcpy(qrData, &in_buf[2], 84);
	//print_debug(qrData, 84);
	memcpy(&qrKeyBuf[0], &qrData[9], 12);
	qrKeyBuf[12] = qrData[27];
	memcpy(&qrKeyBuf[13], &qrData[15], 2);
	
	memcpy(inbuf, "\x80\xfc\x10\x10\x0f", 5);
	memcpy(&inbuf[5], qrKeyBuf, 15);
	ret = sam_apdu(bgSmpsamIndex, inbuf, 20, outbuf, &obytes, 0, 0);
	//print_debug(outbuf, obytes);
	if(ret != 0) 
	{
		gResponseStatus =CE_PSAMERROR;
		return OP_ERR;
	}
	if((outbuf[obytes-2]!=0x61)&&(outbuf[obytes-2]!=0x90))
	{
		gResponseStatus =CE_AUTHENERROR;
		return OP_ERR;
	}
	memcpy(qr_consume_key, outbuf, 18);
	//过程密钥
	WatchDiversity(qr_consume_key, &qrData[72], qr_consume_process_key, 0xff);
	//应用认证码
	memset(buf, 0x00, sizeof(buf));
	memcpy(&buf[8], qrData, 80);
	CmdWatchCalMac(80, &buf[8], buf, qr_consume_process_key, &buf[100], 0xff);
	memcpy(out_buf, &buf[100], 4);
	//过程数据MAC
	memcpy(&buf[8], &qrData[28], 25);
	CmdWatchCalMac(25, &buf[8], buf, qr_consume_key, &buf[60], 0xff);
	memcpy(&out_buf[4], &buf[60], 4);
	//
	memcpy(&out_buf[8], qr_consume_key, 16);
	memcpy(&out_buf[24], qr_consume_process_key, 16);
	

	return 0;
}
//[2]
UBYTE sh_mqr_tac(UBYTE *inbuf, UBYTE *obuf)
{
int rtn;

printf("qr_family = %d\n", qr_family);
	switch(qr_family)
	{
	case 0:
		gResponseStatus = CE_INVALIDCARD;
		rtn = OP_ERR;
		break;
	case 1:	//JTB
		//rtn = gResponseStatus = qr_jtb_tac(&inbuf[2], obuf);
		//break;
	case 2:	//SSM
		rtn = gResponseStatus = qr_jtb_tac(&inbuf[2], obuf);
		break;
	case 3:	//STM
		rtn = gResponseStatus = qr_stm_tac(&inbuf[2], obuf);
		break;
	}
	return rtn;
}

#endif
