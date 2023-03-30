#ifndef QR_C
#define QR_C

unsigned char qrData[512], qrDataEx[512], blnQR = 0;
unsigned char qr_consume_key[16];
unsigned char qr_consume_process_key[16];
unsigned char qrKeyBuf[100], sendMAC[20];
UDWORD	dwTranstime;
unsigned char transTime[7], curTime[7];
unsigned char blnQRV1 = 0;		//Ver 2
unsigned char QR_TYPE = 0;
int 		qr_family;	//0:MDH 1:JTB  2:SSM 	3:STM

#define DEBUG_QR
//#define	DEBUG_NOCHECK

UBYTE getinfo_QR(unsigned char *data, int len)
{
	if(len == 84)
	{
		qr_family = 0;
		blnQRV1 = 0xff;
		QR_TYPE = 0;
		return getinfo_QR_V1(data);
	}else if( (memcmp(data, "https://s.sh.gov.cn/", 20) == 0) && (memcmp(&data[len - 2], "##", 2) == 0) )
	{
		qr_family = 2;
		return getinfo_QR_SSM(data, len);
	}else if( memcmp(data, "SHSTMETRO", 9) == 0)
	{
		qr_family = 3;
		return getinfo_QR_STM(data, len);
	}else if( (len >= 332))  //((data[0] >=0x80) && (data[0] <= 0xFF))
	{
		qr_family = 1;
		return getinfo_QR_JTB(data, len);
	}else if(len == 128)
	{
		QR_TYPE = 1;
		qr_family = 0;
		blnQRV1 = 0x00;
		return getinfo_QR_V2(data);
	}
}

UBYTE getinfo_QR_V1(unsigned char *data)
{
unsigned char buf[500];
unsigned char bleData[100];
int rtn;
long len;
int i;
	
	memcpy(qrData, data, 88);
#ifdef DEBUG_QR
	printf("QR(Ver1):");
	for(i = 0; i < 84; i++)
		printf("%02X ", qrData[i]);
	printf("\n");
#endif
	//�豸����
	qrData[84] = 0x00;
	if(ReadMode&0x01)
		qrData[84] = 0x01;
	
	//����������Կ
	//�˺ź�8���ֽ�
	memcpy(&qrKeyBuf[0], &qrData[9], 12);
	//������
	qrKeyBuf[12] = qrData[27];
	//�˺ź�2���ֽ�
	memcpy(&qrKeyBuf[13], &qrData[15], 2);
	
	//
	if(memcmp(&qrData[1], "\xff\xff\xff\xff\xff\xff", 6) != 0x00)
	{
		//printf("0x84 %02x\n", qrData[84]);
		//00 + ble Mac (6) + mac(4)
		memcpy(bleData, qrData, 7);
		memcpy(&bleData[7], &qrData[80], 5);
		startBLEOnline(bleData);
		blnQR = 0xff;
	}

	if(QRGetKeyAStep1(qrKeyBuf)!=0)
	{
		gResponseStatus =CE_AUTHENERROR;
		stopBLEOffline();
		goto label_err_key;
	}	

	if(QRGetKeyAStep2()!=0)
	{
#ifdef DEBUG_QR
		printf("\n GET TOCKEN KEY ERROR\n");
#endif
		gResponseStatus =CE_AUTHENERROR;
		stopBLEOffline();
		goto label_err_key;
	}
	//һ��һ��
	memcpy(qr_consume_key, bgTaskKeyOutbuf, 16);
	//������Կ
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

	//Ӧ����MAC����
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
unsigned char buf[500];
unsigned char bleData[100];
int rtn;
long len;
int i;
	
	memcpy(qrData, data, 128);
	memcpy(qrDataEx, &data[68], 44+16);
#ifdef DEBUG_QR
	printf("QR(Ver2):");
	for(i = 0; i < 128; i++)
		printf("%02X ", qrData[i]);
	printf("\n");
#endif
	//�豸����
	qrData[128] = 0x00;
	if(ReadMode&0x01)
		qrData[128] = 0x01;
	
	//����������Կ
	//�˺ź�8���ֽ�
	memcpy(&qrKeyBuf[0], &qrData[9], 12);
	//������
	qrKeyBuf[12] = qrData[27];
	//�˺ź�2���ֽ�
	memcpy(&qrKeyBuf[13], &qrData[15], 2);
	
	//
	if(memcmp(&qrData[1], "\xff\xff\xff\xff\xff\xff", 6) != 0x00)
	{
		//printf("0x84 %02x\n", qrData[84]);
		memcpy(bleData, qrData, 7);
		memcpy(&bleData[7], &qrData[124], 5);
		startBLEOnline(bleData);
		blnQR = 0xff;
	}

	if(QRGetKeyAStep1(qrKeyBuf)!=0)
	{
		gResponseStatus =CE_AUTHENERROR;
		stopBLEOffline();
		goto label_err_key;
	}	

	if(QRGetKeyAStep2()!=0)
	{
#ifdef DEBUG_QR
		printf("\n GET TOCKEN KEY ERROR\n");
#endif
		gResponseStatus =CE_AUTHENERROR;
		stopBLEOffline();
		goto label_err_key;
	}
	//һ��һ��
	memcpy(qr_consume_key, bgTaskKeyOutbuf, 16);
	//������Կ
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
	if(memcmp(&buf[400], &qrData[124], 4) != 0)
	{
#ifdef DEBUG_QR
		printf("invalid card, calMAC %02x%02x%02x%02x\n", buf[124], buf[125], buf[126], buf[127]);
#endif
		stopBLEOffline();
		gResponseStatus =CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		goto label_err_key;
	}

	//Ӧ����MAC����
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
	buf[3] = 0x01;
	//
	memcpy(&buf[4], qrData, 68);
	memcpy(&buf[72], &qrData[112], 12);
	shmt_prot_send(buf, 84);

	//Ӧ����չ��MAC����
	memset(buf, 0x00, sizeof(buf));
	if(memcmp(&qrDataEx[0], buf, 40) == 0)
		memset(&qrDataEx[40], 0x00, 4);
	else
	{
		memcpy(&buf[8], &qrDataEx[0], 40);
		CmdWatchCalMac(40, &buf[8], buf, qr_consume_key, &buf[60], 0xff);
		if( memcmp(&buf[60], &qrDataEx[40], 4) == 0)
			memset(&qrDataEx[40], 0x00, 4);
		else
			memset(&qrDataEx[40], 0xFF, 4);
	}
	
	buf[0] = QR_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = CE_OK;
	buf[3] = 0x82;
	
	memcpy(&buf[4], qrDataEx, 44);
	shmt_prot_send(buf, 48);
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
������qr_trans_exit
���ܣ���ά���վ			 
****************************************************************************/
UBYTE qr_trans_exit(unsigned char *inbuf, unsigned char *outbuf)
{
UBYTE buf[200], ble[500], zero[8];
UWORD temp, lastAmount;
UBYTE recBuf[100];
	//
UBYTE resp_buf[200];
int rtn, len;

	if( !blnQR)
	{
		gResponseStatus=CE_BADCOMMAND;
		blnQR = 0;
		return OP_ERR;
	}
	memset(zero, 0x00, sizeof(zero));
	//����TAC
	memcpy(qrKeyBuf, &qrData[9], 8);
	memcpy(&qrKeyBuf[8], &inbuf[2], 4);
	qrKeyBuf[12] = 0x00;			//����ǰ���,����ǰ
	qrKeyBuf[13] = 0x00;
	qrKeyBuf[14]= inbuf[17];								//���׽��,����ǰ
	qrKeyBuf[15]= inbuf[16];
	//�ϴν���վ��
	//�ϴγ�վ���ݽ��ױ�־
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
	
	print_debug(qrKeyBuf, 32);

	//
	if(memcmp(&qrData[1], "\xff\xff\xff\xff\xff\xff", 6) != 0x00)
	{//�����������
		memcpy(buf, qrData, 84);
		//���ױ�־
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
		//�ۼƽ��
		memcpy(&dwTranstime, &qrData[47], 4);
		memcpy(&temp, &inbuf[16], 2);
		dwTranstime += temp;
		memcpy(&buf[47], &dwTranstime, 4);
		//�ۼ�
		temp = (qrData[51] << 8) + qrData[52];
		temp += 1;
		buf[51] = (unsigned char)(temp >> 8);
		buf[52] = (unsigned char)temp;
		//����Ӧ����MAC
		memset(ble, 0x00, sizeof(ble));
		CmdWatchCalMac(25, &buf[28], ble, qr_consume_key, &buf[53], 0xff);
		//
		buf[67] += 1; 
		
		ble[0] = 0x00;
		ble[1] = 58;
		//������
		ble[2] = 0x31;
		//��ά���������
		ble[3] = 0x6B;
		//��ǰ�ն��豸���
		memcpy(&ble[4], &inbuf[8], 4);
		//�ն��豸��ˮ��
		memcpy(&ble[8], &inbuf[12], 4);
		//����ʱ��
		memcpy(&ble[12], &inbuf[2], 4);
		//
		memcpy(&ble[16], &buf[28], 40);
		//������Կ����ͨ��mac
		CmdWatchCalMac(53, &ble[3], zero, qr_consume_process_key, &ble[56], 0xff);
		
		//rtn = bleOnline(qrData, 84);
		rtn = getBlEOnlineReturn(sendMAC);
		if(rtn)
		{
			stopBLEOffline();
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
		//������ӷ��ص�MAC:˫����֤������Ӧ���룬�õ��ĵ�Ӧ������
		if(memcmp(sendMAC, &qrData[17], 4) != 0)
		{
#ifdef	DEBUG_NOCHECK
			stopBLEOffline();
			gResponseStatus=CE_INVALIDCARD;
			return OP_ERR;
#endif
		}
		CmdWatchCalMac(4, &sendMAC[0], zero, qr_consume_process_key, &sendMAC[10], 0xff);
		if(memcmp(&sendMAC[4], &sendMAC[10], 4) != 0)
		{
#ifdef	DEBUG_NOCHECK
			stopBLEOffline();
			gResponseStatus=CE_INVALIDCARD;
			return OP_ERR;
#endif
		}
		sendBLEData(ble, 60);
	}
	SHQRTACStep1(qrKeyBuf);
	
#ifdef DEBUG_TIME
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x0a", 3);
#endif	
	//��ʼ����
	if(memcmp(&qrData[1], "\xff\xff\xff\xff\xff\xff", 6) == 0x00)
	{//������ģ��
		memset(outbuf, 0x00, 12);
	}
	else
	{//��������
		//��������

		//��������
		//rtn = bleSend(ble, 58 + 2, resp_buf, &len);
		//һ�㷢��������Ӧ���룬���ص���LV����������
		rtn = getBLESendReturn(resp_buf, &len);
		if(rtn)
		{
			stopBLEOffline();
			gResponseStatus=CE_CARDREMOVED;
			//��ȡTAC
			if(SHQRTACStep2()!=0)		//ȡTAC
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
		//�ر�����
		//rtn = bleOffline();
		stopBLEOffline();
		//���Ӧ�����
		if(resp_buf[3] != 0)
		{
			gResponseStatus = CE_WRITEERROR;
			return OP_ERR;
		}
		memcpy(outbuf, &resp_buf[4], 10);
	}

	//��ȡTAC
	if(SHQRTACStep2()!=0)												//ȡTAC
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


UBYTE qr_trans_entry(unsigned char *inbuf, unsigned char *outbuf)
{
UBYTE buf[200], ble[500], zero[8];
UWORD temp;
	//
UBYTE resp_buf[200], recBuf[100];
int rtn, len;
int i;

	if( !blnQR)
	{
		gResponseStatus=CE_BADCOMMAND;
		blnQR = 0;
		return OP_ERR;
	}
	
	memset(zero, 0x00, sizeof(zero));
	//����TAC����
	memcpy(qrKeyBuf, &qrData[9], 8);
	memcpy(&qrKeyBuf[8],&inbuf[2],4);
	qrKeyBuf[12] = 0x00;			//����ǰ���,����ǰ
	qrKeyBuf[13] = 0x00;
	qrKeyBuf[14]= 0x00;								//���׽��,����ǰ
	qrKeyBuf[15]= 0x00;
	//�ϴγ�վ���ݽ��ױ�־
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
	
	print_debug(qrKeyBuf, 32);
	
	if(memcmp(&qrData[1], "\xff\xff\xff\xff\xff\xff", 6) != 0x00)
	{
	//�����������
		memcpy(buf, qrData, 84);
		//���ױ�־
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
		//���ݵ�ǰʱ���Ƿ���0
		dwTranstime = (inbuf[2] << 24) + (inbuf[3] << 16) + (inbuf[4] << 8) + inbuf[5];
		dwTranstime -= (TIME2000 + 8 * 3600);
		long2timestr(dwTranstime, curTime);
		dwTranstime = (qrData[33] << 24) + (qrData[34] << 16) + (qrData[35] << 8) + qrData[36];
		dwTranstime -= (TIME2000 + 8 * 3600);
		long2timestr(dwTranstime, transTime);
		if(memcmp(curTime, transTime, 2) != 0)
		{//�Ǳ�������ۼ���Ϣ
			memset(&buf[47], 0x00, 6);
		}
		//����Ӧ����MAC
		memset(ble, 0x00, sizeof(ble));
		CmdWatchCalMac(25, &buf[28], ble, qr_consume_key, &buf[53], 0xff);
		
		if( memcmp(curTime, transTime, 3) != 0)
		{//�Ǳ��գ�����ʹ�ô�����0
			buf[67] = 0;
		}
		ble[0] = 0x00;
		ble[1] = 58;
		//������
		ble[2] = 0x31;
		//��ά���������
		ble[3] = 0x6A;
		//��ǰ�ն��豸���
		memcpy(&ble[4], &inbuf[8], 4);
		//�ն��豸��ˮ��
		memcpy(&ble[8], &inbuf[12], 4);
		//����ʱ��
		memcpy(&ble[12], &inbuf[2], 4);
		//
		memcpy(&ble[16], &buf[28], 40);
		//������Կ����ͨ��mac
		CmdWatchCalMac(53, &ble[3], zero, qr_consume_process_key, &ble[56], 0xff);
		
		//chenck online
		rtn = getBlEOnlineReturn(sendMAC);
#ifdef DEBUG_QR
		print_debug(sendMAC, 10);
#endif
		if(rtn)
		{
			stopBLEOffline();
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
		//������ӷ��ص�MAC
		if(memcmp(sendMAC, &qrData[17], 4) != 0)
		{
#ifdef DEBUG_NOCHECK
			stopBLEOffline();
			gResponseStatus=CE_INVALIDCARD;
			return OP_ERR;
#endif
		}
		CmdWatchCalMac(4, &sendMAC[0], zero, qr_consume_process_key, &sendMAC[10], 0xff);
		if(memcmp(&sendMAC[4], &sendMAC[10], 4) != 0)
		{
#ifdef DEBUG_NOCHECK
			stopBLEOffline();
			gResponseStatus=CE_INVALIDCARD;
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
	//��ʼ����
	if(memcmp(&qrData[1], "\xff\xff\xff\xff\xff\xff", 6) != 0x00)
	{//��������
		//��������
		//rtn = bleOnline(qrData, 84);
		//��������
		//rtn = bleSend(ble, 58 + 2, resp_buf, &len);
		rtn = getBLESendReturn(resp_buf, &len);
		if(rtn)
		{
			stopBLEOffline();
			gResponseStatus=CE_CARDREMOVED;
			if(SHQRTACStep2()!=0)												//ȡTAC
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
		//�ر�����
		//rtn = bleOffline();
		stopBLEOffline();
		//���Ӧ�����
		if(resp_buf[3] != 0)
		{
			gResponseStatus = CE_WRITEERROR;
			return OP_ERR;
		}
		memcpy(outbuf, &resp_buf[4], 10);
	}

	//��ȡTAC
	if(SHQRTACStep2()!=0)												//ȡTAC
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


UBYTE qr_trans_update(unsigned char *inbuf, unsigned char *outbuf)
{
UBYTE buf[200], ble[500], zero[8];
UWORD temp;
	//
UBYTE resp_buf[200], recBuf[100];
int rtn, len;

	
	memset(zero, 0x00, sizeof(zero));
	//����TAC
	//
	memcpy(qrKeyBuf, &qrData[9], 8);
	//
	memcpy(&qrKeyBuf[8],gTransTime,4);
	//����ǰ���,����ǰ
	qrKeyBuf[12] = 0x00;			
	qrKeyBuf[13] = 0x00;
	//���׽��,����ǰ
	qrKeyBuf[14]= inbuf[17];
	qrKeyBuf[15]= inbuf[16];
	//�ϴγ�վ
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
	//
	SHQRTACStep1(qrKeyBuf);
	
	if(memcmp(&qrData[1], "\xff\xff\xff\xff\xff\xff", 6) != 0x00)
	{
		if( !blnQR)
		{
			gResponseStatus=CE_BADCOMMAND;
			blnQR = 0;
			return OP_ERR;
		}
		//startBLEOnline(qrData);
		
		memcpy(buf, qrData, 84);
		//���ױ�־
		memcpy(&buf[28], &inbuf[19], 25);
		//����Ӧ����MAC
		memset(ble, 0x00, sizeof(ble));
		CmdWatchCalMac(25, &inbuf[19], ble, qr_consume_key, &buf[53], 0xff);
//		//��������
//		rtn = bleOnline(qrData, 84, NULL);
//		if(rtn)
//		{
//			gResponseStatus=CE_WRITEERROR;
//			return OP_ERR;
//		}
		//
		//if( (inbuf[19] & 0xC0) == 0x40 )
		{//���º��վ״̬
			/*dwTranstime = (inbuf[2] << 24) + (inbuf[3] << 16) + (inbuf[4] << 8) + inbuf[5];
			dwTranstime -= (TIME2000 + 8 * 3600);
			long2timestr(dwTranstime, curTime);
			#ifdef DEBUG_QR
			printf("\ncommand time :%08X\n", dwTranstime);
			print_debug(curTime, 7);
			#endif	
			dwTranstime = (qrData[33] << 24) + (qrData[34] << 16) + (qrData[35] << 8) + qrData[36];
			dwTranstime -= (TIME2000 + 8 * 3600);
			long2timestr(dwTranstime, transTime);
			//
			#ifdef DEBUG_QR
			printf("last time :%08X\n", dwTranstime);
			print_debug(transTime, 7);
			#endif	*/
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
			if(QR_TYPE == 1)
			{
			if( memcmp(curTime, transTime, 3) != 0)//2022.07.06
			{//�Ǳ��գ�����ʹ�ô�����0
				buf[67] = 0;
				#ifdef DEBUG_QR
					printf("Entry buf[67] :%d\n",buf[67]);
				#endif
			}
			}
		}
		ble[0] = 0x00;
		ble[1] = 58;
		//������
		ble[2] = 0x31;
		//
		ble[3] = 0x6C;
		//��ǰ�ն��豸���
		memcpy(&ble[4], &inbuf[8], 4);
		//�ն��豸��ˮ��
		memcpy(&ble[8], &inbuf[12], 4);
		//����ʱ��
		memcpy(&ble[12], &inbuf[2], 4);
		//
		memcpy(&ble[16], &buf[28], 40);
		memcpy(&ble[16], &inbuf[19], 25);
		//������Կ����ͨ��mac
		CmdWatchCalMac(53, &ble[3], zero, qr_consume_process_key, &ble[56], 0xff);

		rtn = getBlEOnlineReturn(sendMAC);
#ifdef DEBUG_QR
		print_debug(sendMAC, 10);
#endif
		if(rtn)
		{
			stopBLEOffline();
			gResponseStatus=CE_WRITEERROR;
			return OP_ERR;
		}
		//������ӷ��ص�MAC
		if(memcmp(sendMAC, &qrData[17], 4) != 0)
		{
#ifdef DEBUG_NOCHECK
			stopBLEOffline();
			gResponseStatus=CE_INVALIDCARD;
			return OP_ERR;
#endif
		}
		CmdWatchCalMac(4, &sendMAC[0], zero, qr_consume_process_key, &sendMAC[10], 0xff);
		if(memcmp(&sendMAC[4], &sendMAC[10], 4) != 0)
		{
			stopBLEOffline();
			gResponseStatus=CE_INVALIDCARD;
			return OP_ERR;
		}
		//˫����֤ͨ��ʱ����Ӧ����
		sendBLEData(ble, 60);
	}
	
#ifdef DEBUG_TIME
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x0a", 3);
#endif	
	//��ʼ����
	if(memcmp(&qrData[1], "\xff\xff\xff\xff\xff\xff", 6) != 0x00)
	{//��������
		rtn = getBLESendReturn(resp_buf, &len);
		if(rtn)
		{
			stopBLEOffline();
			gResponseStatus=CE_CARDREMOVED;
			if(SHQRTACStep2()!=0)				//ȡTAC
			{
				gResponseStatus = CE_PSAMERROR;
				return OP_ERR;
			}
			memcpy(gTAC,bgTaskKeyOutbuf,4);				
		
			memcpy(&recBuf[0], &inbuf[12], 4);
			memcpy(&recBuf[4], &qrData[7], 10);
			if( blnQRV1 )
				memcpy(&recBuf[14], &qrData[72], 8);
			else
				memcpy(&recBuf[14], &qrData[116], 8);
			memcpy(&recBuf[22], &resp_buf[5], 4);
			memcpy(&recBuf[26], gTAC, 4);
			save_qr_record(recBuf);
			return OP_ERR;
		}
		stopBLEOffline();
		//���Ӧ�����
		if(resp_buf[3] != 0)
		{
			gResponseStatus = CE_WRITEERROR;
			return OP_ERR;
		}
		memcpy(outbuf, &resp_buf[4], 10);
	}

	//��ȡTAC
	if(SHQRTACStep2()!=0)			//ȡTAC
	{
		gResponseStatus = CE_PSAMERROR;
		return OP_ERR;
	}
	memcpy(gTAC,bgTaskKeyOutbuf,4);				

	memcpy(&recBuf[0], &inbuf[12], 4);
	memcpy(&recBuf[4], &qrData[7], 10);
	if(blnQRV1)
		memcpy(&recBuf[14], &qrData[72], 8);
	else
		memcpy(&recBuf[14], &qrData[116], 8);
	memcpy(&recBuf[22], &resp_buf[5], 4);
	memcpy(&recBuf[26], gTAC, 4);
	save_qr_record(recBuf);
	
	return 0;
}

UBYTE qr_read(unsigned char *inbuf, unsigned char *outbuf, unsigned short *outlen)
{
UBYTE	ec, trytimes;
int rtn;
unsigned char data[256];
long len;

	trytimes = 0;
label_rereadqr:
	rtn = scancontrol(data ,&len);

	if(rtn > 0)
	{
		//��ͨ������Ҫ������ѯ��ά��
		if(rtn == 0xff)
			return CE_QRERROR;
		//���򣬳��Զ���ض�
		trytimes +=1;
		if(trytimes < 100)
			goto label_rereadqr;
		return CE_NOCARD;
	}
	*outlen = len;
	memcpy(outbuf, data, len);
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
	/*case 1:	//JTB
		//break;
	case 2:	//SSM
		//break;
	case 3:	//STM
		rtn = gResponseStatus = qr_jtb_tac(&inbuf[2], obuf);
		break;*/
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