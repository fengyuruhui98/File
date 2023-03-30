//SH_TRANSPORT_OPERATION.c
#ifndef _SH_TRANSPORT_OPERATION_C_
#define _SH_TRANSPORT_OPERATION_C_

#define DEBUG_TRANSPORT

unsigned char transport_15_data[30];		//公共应用信息文件
unsigned char transport_16_data[55];		//持卡人基本信息文件
unsigned char transport_17_data[60];		//管理信息文件
unsigned char transport_1A_data[128];		//公共交通过程信息变长记录文件
unsigned char transport_1E_data[48];		//公共交通过程信息循环记录文件
unsigned char transport_balance[4];

unsigned char capp_init[19];			//init capp purcahse return
unsigned char capp_debit[8];			//debit capp purchase return

unsigned long CRC32_Value;
UBYTE temp_crc[4];

/************************************************************************************************
函数：Transport_GetFiles15
功能：读交通部15文件
************************************************************************************************/
unsigned short Transport_GetFiles15(unsigned char *out_buf)
{
	
int ret,i;
unsigned char buf[40];
unsigned char cpubuf[80], Le;
UBYTE cpulen;

#ifdef DEBUG_TRANSPORT
	printf("\n***Transport_GetFiles15***\n");
#endif	
	//read file 15
	memcpy(buf, "\x00\xb0\x95\x00\x1e", 5);
	ret = mifpro_icmd0(5,buf,cpubuf);
#ifdef DEBUG_TRANSPORT
	printf("\n15 mifpro_icmd0 :%d\n",ret);
	printf("buf:");
	for (i = 0; i < 5; i++)
	{
		printf(" %02x",buf[i]);
	}
#endif
	if(ret < 0)
	{
		return CE_READERROR;
	}
	
	if((cpubuf[ret-2]!=0x90)||(cpubuf[ret-1]!=0x00))
	{
#ifdef	DEBUG_TRANSPORT
		printf("\nGetFiles15 return :%02x %02x",cpubuf[ret-2],cpubuf[ret-1]);
#endif
		return CE_READERROR;
	}

	if(ret != 0x1e + 2)
	{	
		return CE_INVALIDCARD;
	}
	memcpy(transport_15_data, cpubuf, 30);
#ifdef DEBUG_TRANSPORT
	printf("\ntransport_15_data:");
	for(i = 0; i < 30; i++)
	printf("%02x ", cpubuf[i]);
	printf("\n");
	printf("file 15 len: %02x\n", ret);
	printf("logicid: %02x %02x %02x %02x %02x %02x %02x %02x\n", 
			transport_15_data[0], transport_15_data[1], transport_15_data[2], transport_15_data[3],transport_15_data[4], transport_15_data[5], transport_15_data[6], transport_15_data[7]);
	printf("appindex: %02x\n", transport_15_data[8]);
	printf("app ver: %02x\n", transport_15_data[9]);
	printf("app sn: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
			transport_15_data[10], transport_15_data[11], transport_15_data[12], transport_15_data[13], transport_15_data[14], transport_15_data[15], transport_15_data[16], transport_15_data[17], transport_15_data[18], transport_15_data[19]);
	printf("app startdate: %02x%02x-%02x-%02x\n", transport_15_data[20], transport_15_data[21], transport_15_data[22], transport_15_data[23]);
	printf("valid date: %02x%02x-%02x-%02x\n", transport_15_data[24], transport_15_data[25], transport_15_data[26], transport_15_data[27]);
	printf("card type: %02x\n", transport_15_data[28]);
	printf("cash peldge: %02x\n", transport_15_data[29]);
#endif
	
	return 0;
	
}

/************************************************************************************************
函数：Transport_GetFiles16
功能：读交通部16文件
************************************************************************************************/
unsigned short Transport_GetFiles16(unsigned char *out_buf)
{/*
unsigned char buf[100], cpubuf[300];
unsigned short cpulen;
int ret;

	memcpy(out_buf, "\x4b\x0a", 2);
	memcpy(buf, "\x00\xb0\x87\x00", 4);
	buf[4] = SZ_TRANSPORT_16_LEN;
	ret = mifpro_icmd(buf, 5, cpubuf, &cpulen);
	if(ret != 0)
	{
		memcpy(&out_buf[2], cpubuf, 2);
		return CE_READERROR;
	}
	memcpy(out_buf, "\x4b\x0b", 2);
#ifdef DEBUG_TRANSPORT
	printf("file 16 len %02x persontype:%02x name %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x idtype:%02x persionid:%02x%02x%02x%02x%02x%02x\n", 
		cpulen, cpubuf[0], cpubuf[1], cpubuf[2], cpubuf[3], cpubuf[4], cpubuf[5], cpubuf[6], cpubuf[7], cpubuf[8], cpubuf[9], cpubuf[10],
		cpubuf[21], cpubuf[22], cpubuf[23], cpubuf[24], cpubuf[25], cpubuf[26], cpubuf[27]);
#endif
	if( (cpubuf[cpulen - 2] != 0x90) || (cpubuf[cpulen - 1] != 0x00) )
		return CE_INVALIDCARD;
	memcpy(transport_16_data, cpubuf, SZ_TRANSPORT_16_LEN);
*/
}

/************************************************************************************************
函数：Transport_GetFiles17
功能：读交通部17文件
************************************************************************************************/
unsigned short Transport_GetFiles17(unsigned char *out_buf)
{
int ret, i;
unsigned char buf[80];
unsigned char cpubuf[100], Le;
UBYTE cpulen;

#ifdef DEBUG_TRANSPORT
	printf("\n***Transport_GetFiles17***\n");
#endif
	
	//read file 17 
	memcpy(out_buf, "\x4b\xa3", 2);
	memcpy(buf,"\x00\xb0\x97\x00\x3c", 5);
	ret = mifpro_icmd0(5, buf,cpubuf);
#ifdef DEBUG_TRANSPORT
	printf("\n17 mifpro_icmd0 :%d\n",ret);
#endif
	if(ret < 0)
	{
		return CE_READERROR;
	}
#ifdef DEBUG_TRANSPORT
	printf("\nbuf:");
	for (i = 0; i < 5; i++)
	{
		printf(" %02x",buf[i]);
	}
#endif
	
	if((ret == 2) && (cpubuf[0] == 0x6c))
	{
		buf[4] = Le = cpubuf[1];
		ret = mifpro_icmd0( 5, buf,cpubuf);
		if(ret < 0)
		{
			return CE_READERROR;
		}
	}
	if((cpubuf[ret - 2] != 0x90) || (cpubuf[ret - 1] != 0x00))
	{
#ifdef	DEBUG_TRANSPORT
		printf("\nfile 17:%02x %02x\n", cpubuf[0], cpubuf[1]);
#endif
		return CE_INVALIDCARD;
	}
	memcpy(transport_17_data, cpubuf, ret - 2);
	//判断是否为互联互通卡，不是返回14无效卡
	if(memcmp(&transport_17_data[8],"\x00\x00",2)==0)
	{
		gResponseStatus=CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}	
#ifdef DEBUG_TRANSPORT
	printf("\ntransport_17_data:");
	for(i = 0; i < ret - 2; i++)
	{
		printf("%02x ", cpubuf[i]);
	}
	printf("\n");
	printf("file 17:len: %02x\n", ret);
	printf("international code : %02x %02x %02x %02x\n",transport_17_data[0], transport_17_data[1], transport_17_data[2], transport_17_data[3]);
	printf("province code : %02x %02x\n",transport_17_data[4], transport_17_data[5]);
	printf("city code : %02x %02x\n",transport_17_data[6], transport_17_data[7]);
	printf("union : %02x %02x\n",transport_17_data[8], transport_17_data[9]);
	printf("card type : %02x\n",transport_17_data[10]);
#endif

	return 0;
}

/************************************************************************************************
函数：Transport_GetFiles1A
功能：读交通部1A文件
************************************************************************************************/
char Transport_GetFiles1A(unsigned char *out_buf)
{
int ret, i;
long balance;
unsigned char buf[40];
unsigned char cpubuf[300],Le;
UBYTE cpulen;
	
#ifdef DEBUG_TRANSPORT
	printf("\n***Transport_GetFiles1A***\n");
#endif	
	
	//read file 1a - variable file length
	memcpy(buf, "\x00\xb2\x01\x00\x30", 5);
	buf[3] = (0x1a << 3) | 0x04;
	buf[4] = Le = TRANSPORT_1A_LEN;
	ret = mifpro_icmd0(5, buf, cpubuf);
#ifdef DEBUG_TRANSPORT
	printf("\n1A mifpro_icmd0 :%d\n",ret);
#endif
	if(ret < 0)
	{
		return CE_READERROR;
	}
#ifdef DEBUG_TRANSPORT
	printf("\nbuf:");
	for (i=0; i<5; i++)
	{
		printf(" %02x",buf[i]);
	}
#endif	

	if((ret == 2) && cpubuf[0] == 0x6c)//6C:长度错误
	{
		buf[4] = Le = cpubuf[1];
		ret = mifpro_icmd0(5, buf, cpubuf);
		if(ret < 0)
		{
			return CE_READERROR;
		}
	}
	if((cpubuf[ret - 2]!= 0x90)||(cpubuf[ret - 1]!= 0x00))
	{
		return CE_INVALIDCARD;
	}
#ifdef DEBUG_TRANSPORT
	printf("read file 1a len :%d\n", Le);
	for(i = 0; i < ret; i++) printf("%02x ", cpubuf[i]);
	printf("\n");
#endif

	if(ret != Le + 2)
	{
		return CE_INVALIDCARD;
	}
	memcpy(transport_1A_data, cpubuf, TRANSPORT_1A_LEN);
#ifdef DEBUG_TRANSPORT
//	printf("\n");
//	for(i = 0; i < cpulen; i++)
//		printf("%02x ", cpubuf[i]);
//	printf("\n");
	printf("\nfile 1a consume flag :%02x%02x recordlen :%02x%02x UNFlag :%02x lockedflag :%02x\n", 
		transport_1A_data[0], transport_1A_data[1], transport_1A_data[2], transport_1A_data[3], transport_1A_data[4], transport_1A_data[5]);
	printf("\nSN :%02x%02x%02x%02x %02x%02x%02x%02x status :%02x en-city :%02x%02x ex-city :%02x%02x en-organition :%02x%02x%02x%02x %02x%02x%02x%02x ex-organition :%02x%02x%02x%02x %02x%02x%02x%02x\n", 
		transport_1A_data[6], transport_1A_data[7], transport_1A_data[8], transport_1A_data[9], transport_1A_data[10], transport_1A_data[11], transport_1A_data[12], transport_1A_data[13], 
		transport_1A_data[14], transport_1A_data[15], transport_1A_data[16], transport_1A_data[17], transport_1A_data[18], transport_1A_data[19], transport_1A_data[20], transport_1A_data[21], transport_1A_data[22], 
		transport_1A_data[23], transport_1A_data[24], transport_1A_data[25], transport_1A_data[26], transport_1A_data[27], transport_1A_data[28], transport_1A_data[29], transport_1A_data[30], transport_1A_data[31], transport_1A_data[32], transport_1A_data[33], transport_1A_data[34]);
	printf("\nen-station :%02x%02x%02x%02x %02x%02x%02x%02x ex-station :%02x%02x%02x%02x %02x%02x%02x%02x en-device :%02x%02x%02x%02x %02x%02x%02x%02x ex-device :%02x%02x%02x%02x %02x%02x%02x%02x \n",
		transport_1A_data[35], transport_1A_data[36], transport_1A_data[37], transport_1A_data[38], transport_1A_data[39], transport_1A_data[40], transport_1A_data[41], transport_1A_data[42], transport_1A_data[43],
		transport_1A_data[44], transport_1A_data[45], transport_1A_data[46], transport_1A_data[47], transport_1A_data[48], transport_1A_data[49], transport_1A_data[50], transport_1A_data[51], transport_1A_data[52],
		transport_1A_data[53], transport_1A_data[54], transport_1A_data[55], transport_1A_data[56], transport_1A_data[57], transport_1A_data[58], transport_1A_data[59], transport_1A_data[60], transport_1A_data[61], 
		transport_1A_data[62], transport_1A_data[63], transport_1A_data[64], transport_1A_data[65], transport_1A_data[66]);
	printf("\nen-time :%02x%02x-%02x-%02x %02x:%02x:%02x ex-time :%02x%02x-%02x-%02x %02x:%02x:%02x maxConsume %02x%02x%02x%02x en-line %02x ex-line %02x \n",
		transport_1A_data[67], transport_1A_data[68], transport_1A_data[69], transport_1A_data[70], transport_1A_data[71], transport_1A_data[72] ,transport_1A_data[73],
		transport_1A_data[74], transport_1A_data[75], transport_1A_data[76], transport_1A_data[77], transport_1A_data[78], transport_1A_data[79], transport_1A_data[80],
		transport_1A_data[81], transport_1A_data[82], transport_1A_data[83], transport_1A_data[84], transport_1A_data[85], transport_1A_data[86]);
	printf("\nen-tranamount :%02x%02x%02x%02x en-balance :%02x%02x%02x%02x shiji-tranamount :%02x%02x%02x%02x \n",
		transport_1A_data[87], transport_1A_data[88], transport_1A_data[89], transport_1A_data[90], transport_1A_data[91], transport_1A_data[92], transport_1A_data[93], transport_1A_data[94], transport_1A_data[95], transport_1A_data[96], transport_1A_data[97], transport_1A_data[98]);
	printf("\nfinal trans time :%02x%02x-%02x-%02x %02x:%02x:%02x",
		transport_1A_data[99],transport_1A_data[100],transport_1A_data[101],transport_1A_data[102],transport_1A_data[103],transport_1A_data[104],transport_1A_data[105]);
	printf("\nfinal trans device id :%02x%02x%02x%02x",
		transport_1A_data[106],transport_1A_data[107],transport_1A_data[108],transport_1A_data[109]);
	printf("\nleiji trans amount :%02x%02x%02x  leiji trans number :%02x%02x",
		transport_1A_data[110],transport_1A_data[111],transport_1A_data[112],transport_1A_data[113],transport_1A_data[114]);
	printf("\nRFU :%02x%02x%02x%02x%02x",
		transport_1A_data[115],transport_1A_data[116],transport_1A_data[117],transport_1A_data[118],transport_1A_data[119]);
	printf("\nEn-station sign :%02x",transport_1A_data[120]);
	printf("\nEx-station sign :%02x",transport_1A_data[121]);
	printf("\nEn-Ex status :%02x",transport_1A_data[122]);
	printf("\nRFU :%02x",transport_1A_data[123]);
	printf("\nCRC32 :%02x%02x%02x%02x\n",transport_1A_data[124],transport_1A_data[125],transport_1A_data[126],transport_1A_data[127]);
#endif

/*	//read balance---P1 00表示含透支金额  修改为03可以读取不含透支金额
	memcpy(out_buf, "\xfa\x20", 2);
	memcpy(buf, "\x80\x5c\x03\x02\x04", 5);
	ret = mifpro_icmd0(5,buf, cpubuf);
	if(ret < 0)
	{
		return CE_READERROR;
	}
	if((cpubuf[ret - 2]!= 0x90)||(cpubuf[ret - 1]!= 0x00))
	{
		return CE_READERROR;
	}
#ifdef DEBUG_TRANSPORT
	printf("\nread balance return len %d %02x %02x %02x %02x \n", ret, cpubuf[0], cpubuf[1], cpubuf[2], cpubuf[3]);
#endif
	
	if(ret != 6)
	{
		return CE_INVALIDCARD;
	}
	memcpy(transport_balance,cpubuf,4);
	ByteToLong(&balance, cpubuf);
#ifdef DEBUG_TRANSPORT
	printf("cpu balance:%d\n",&balance);
#endif*/
	
	return 0;
}

/************************************************************************************************
函数：Transport_GetFiles1E
功能：读交通部1E文件
************************************************************************************************/
unsigned short Transport_GetFiles1E(char record, unsigned char *out_buf)
{
	
int ret,i;
unsigned char buf[40];
unsigned char cpubuf[80], Le;
UBYTE cpulen;

#ifdef DEBUG_TRANSPORT
	printf("\n***Transport_GetFiles1E***\n");
#endif	
	//read file 15
	memcpy(buf, "\x00\xb2\x01\xF4\x30", 5); //30为长度 （0x1E << 3）| 0x04为F4
	buf[2] = record;
	//buf[3] = (0x1e << 3) | 0x04;
	ret = mifpro_icmd0(5,buf, cpubuf);     //5 ->4
	
#ifdef DEBUG_TRANSPORT
	printf("\nbuf:");
	for (i = 0; i < 5; i++)
	{
		printf(" %02x",buf[i]);
	}
#endif
	if(ret < 0)
	{
		return CE_READERROR;
	}
	if((cpubuf[ret - 2]!= 0x90)||(cpubuf[ret - 1]!= 0x00))
	{
#ifdef	DEBUG_TRANSPORT
		printf("\nGetFiles1E mifpro_icmd return %d\n", ret);
		printf("\nzdd20220321---GetFiles1E mifpro_icmd return:%02x,%02x", cpubuf[ret - 2], cpubuf[ret - 1]);
#endif
		return CE_READERROR;
	}

	if(ret != 0x30 + 2)
	{
		return CE_INVALIDCARD;
	}
	memcpy(transport_1E_data, cpubuf, 48);
	memcpy(out_buf, cpubuf, 48);
#ifdef DEBUG_TRANSPORT
	printf("\ntransport_1E_data:");
	for(i = 0; i < ret; i++)
	{	
		printf("%02x ", cpubuf[i]);
	}
	printf("\n");
	printf("file 1E len:%02x\n",ret);
	printf("trans type :%02x\n",transport_1E_data[0]);
	printf("device ID :%02x %02x %02x %02x %02x %02x %02x %02x\n",transport_1E_data[1],transport_1E_data[2],transport_1E_data[3],transport_1E_data[4],transport_1E_data[5],transport_1E_data[6],transport_1E_data[7],transport_1E_data[8]);
	printf("Industry code :%02x\n",transport_1E_data[9]);
	printf("line :%02x %02x\n",transport_1E_data[10],transport_1E_data[11]);
	printf("station :%02x %02x\n",transport_1E_data[12],transport_1E_data[13]);
	printf("yunying code :%02x %02x\n",transport_1E_data[14],transport_1E_data[15]);
	printf("RFU :%02x\n",transport_1E_data[16]);
	printf("transaction amount :%02x %02x %02x %02x\n",transport_1E_data[17],transport_1E_data[18],transport_1E_data[19],transport_1E_data[20]);
	printf("Post-transaction balance :%02x %02x %02x %02x\n",transport_1E_data[21],transport_1E_data[22],transport_1E_data[23],transport_1E_data[24]);
	printf("trans time :%02x %02x %02x %02x %02x %02x %02x\n",transport_1E_data[25],transport_1E_data[26],transport_1E_data[27],transport_1E_data[28],transport_1E_data[29],transport_1E_data[30],transport_1E_data[31]);
	printf("CODE_CITY :%02x %02x\n",transport_1E_data[32],transport_1E_data[33]);
	printf("CODE_ORGANIZATION :%02x %02x %02x %02x %02x %02x %02x %02x\n",transport_1E_data[34],transport_1E_data[35],transport_1E_data[36],transport_1E_data[37],transport_1E_data[38],transport_1E_data[39],transport_1E_data[40],transport_1E_data[41]);
	printf("RFU :%02x %02x %02x %02x %02x %02x\n",transport_1E_data[42],transport_1E_data[43],transport_1E_data[44],transport_1E_data[45],transport_1E_data[46],transport_1E_data[47]);		
#endif
	
	return 0;
	
}

/************************************************************************************************
函数：sh_transport_get_info
功能：获取交通部卡信息
************************************************************************************************/
UBYTE sh_transport_get_info(unsigned char *data)
{
	unsigned char buf[500], inbuf[300], outbuf[300];
	unsigned char inbytes, obytes, outbytes;
	int rtn, ret,cnt;
	long len;
	int i;
	UBYTE out_buf[300];
	
#ifdef DEBUG_TRANSPORT
	printf("\n***sh_transport_get_info***\n");
#endif 

	memset(data,0,sizeof(data));
//select transport AID
	memcpy(buf,"\x00\xA4\x04\x00\x08\xA0\x00\x00\x06\x32\x01\x01\x05\00",14);
#ifdef DEBUG_TRANSPORT
	printf("\ntransport...1\n");
#endif

	memset(outbuf,0,sizeof(outbuf));
	ret=mifpro_icmd0(14,buf,outbuf);
	if(ret < 0)
	{
		return OP_ERR;
	}
	
#ifdef DEBUG_TRANSPORT
	printf("\nselect transport AID...ret=%d\n",ret);
	printf("outbuf :");
	for(i=0;i<ret;i++)
	{
		printf(" %02x",outbuf[i]);	
	}
	printf("\n"); 
#endif

	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
		gResponseStatus=CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		return OP_ERR;	
	}
	gCardStatus=outbuf[30];
	//printf("gcardstatus 1 :%02x\n",gCardStatus);
	cardversion=outbuf[31];
	//printf("cardversion 1 :%02x\n",cardversion);
	sh_ticket_family = SH_TICKET_JTB;

	//read 15
	ret=Transport_GetFiles15(out_buf);
#ifdef DEBUG_TRANSPORT
	printf("\nread 15 ret :%d\n",ret);
#endif
	if(ret!=0)
	{
		return OP_ERR;
	}

	//read 17
	ret=Transport_GetFiles17(out_buf);
#ifdef DEBUG_TRANSPORT
	printf("\nread 17 ret :%d\n",ret);
#endif
	if(ret!=0)
	{
		return OP_ERR;
	}
	
	if( memcmp(&transport_15_data[0], SH_CODE_ORGANIZATION, 4) == 0)
	{
		//读本地卡地铁专用记录
		ret = get_record(0x11,0x03,16,outbuf);
		if(ret)
		{
			return OP_ERR;
		}
#ifdef DEBUG_TRANSPORT
		printf("\nread_record 11-03..1:");
		for(i=0;i<16;i++)
		{
			printf("%02x ",outbuf[i]);
		}
		printf("\n");
#endif
		if(lrc_chk2(outbuf,15)!=0)
		{
			gMetroIoFlag = 0;
		}
		else
		{
			gMetroIoFlag = outbuf[0];
		}
		memcpy(gB34, outbuf, 16);
		
#ifdef DEBUG_TRANSPORT
		printf("\nread_record 11-03..2:");
		for(i=0;i<16;i++)
		{
			printf("%02x ",outbuf[i]);
		}
		printf("\n");
#endif	

		ret = get_record(0x11,0x01,16,outbuf);
		if(ret)
		{
			return OP_ERR;
		}
#ifdef DEBUG_TRANSPORT
		printf("\nget_record 11-01..1:");
		for(i=0;i<16;i++)
		{
			printf("%02x ",outbuf[i]);
		}
		printf("\n");
#endif

		memcpy(gB32,outbuf,16);
		
#ifdef DEBUG_TRANSPORT
		printf("\nread_record 11-01..2:");
		for(i=0;i<16;i++)
		{
			printf("%02x ",gB32[i]);
		}
		printf("\n");
#endif	

		lrc3_gen(gB32,16);
#ifdef DEBUG_TRANSPORT
		printf("\nget_record 11-01..3:");
		for(i=0;i<16;i++)
		{
			printf("%02x ",gB32[i]);
		}
		printf("\n");
#endif
		gStationTimeOverRideFlag = gB32[0];
#ifdef DEBUG_TRANSPORT
		printf("\nInfo--gStationTimeOverRideFlag...1:%02x\n",gStationTimeOverRideFlag);
#endif		
		//保存上次交易金额
		//*((char *)&gCardLastTransValue+INT_LOW) = gB32[10];
		//*((char *)&gCardLastTransValue+INT_LOW) = gB32[9];
		
		memcpy(gB32BK,gB32,20);

		ret = get_record(0x11,0x02,16,outbuf);
		if(ret)
		{
			return OP_ERR;
		}
#ifdef DEBUG_TRANSPORT
		printf("\nget_record 11-02..1:");
		for(i=0;i<16;i++)
		{
			printf("%02x ",outbuf[i]);
		}
		printf("\n");
#endif

		memcpy(gB33,outbuf,16);
		
#ifdef DEBUG_TRANSPORT
		printf("\nget_record 11-02..2:");
		for(i=0;i<16;i++)
		{
			printf("%02x ",gB33[i]);
		}
		printf("\n");
#endif

		
#ifdef DEBUG_TRANSPORT
		printf("\nget_record 11-02..3:");
		for(i=0;i<16;i++)
		{
			printf("%02x ",gB33[i]);
		}
		printf("\n");
#endif	

		lrc3_gen(gB33,16);
		memcpy(gB33BK,gB33,20);
		
#ifdef DEBUG_TRANSPORT
		printf("\nread 11-01....B32:");
		for(i=0;i<16;i++) 
		{
			printf("%02X ",(UBYTE)gB32[i]);
		}
		printf("\n");
		printf("\nread 11-02....B33:");
		for(i=0;i<16;i++) 
		{
			printf("%02X ",(UBYTE)gB33[i]);
		}
		printf("\n");
#endif
			
//		//ret = get_record(0x11,0x03,16,outbuf);
//		//if(ret)
//		//{
//		//	return OP_ERR;
//		//}
		memcpy(outbuf, gB34, 16);
		gMetroIoFlag = outbuf[0];
		//printf("\n***gMetroIoFlag*** :%02x\n",gMetroIoFlag);
		trans_ms();
		//printf("\ngBFirst...1 =%d\n",gBFirst);
		
		ret = get_record(0x10,0x02,16,outbuf);
		if(ret)
		{
			return OP_ERR;
		}
#ifdef DEBUG_TRANSPORT
		printf("\nread 10-02 :");
		for(i=0;i<16;i++) 
		{
			printf("%02X ",outbuf[i]);
		}
		printf("\n");
#endif
		if((lrc_chk2(&outbuf[0],15)!=0))
		{
			ret = get_record(0x10,0x03,16,outbuf);
			if(ret)
			{
				return OP_ERR;
			}
			#ifdef DEBUG_TRANSPORT
				printf("\nread 10-03 :");
				for(i=0;i<16;i++) 
				{
					printf("%02X ",outbuf[i]);
				}
				printf("\n");
			#endif
			if((lrc_chk2(&outbuf[0],15)!=0))
			{
				memset(&outbuf[0],0,16);
				memcpy(info,&outbuf[2],5);
			}
			else
			{
				memcpy(info,&outbuf[2],5);
			}
		}
		else
		{
			memcpy(info,&outbuf[2],5);
		}
		
#ifdef DEBUG_TRANSPORT
		printf("Exchange info :%02x %02x %02x %02x %02x",info[0],info[1],info[2],info[3],info[4]);
#endif
		memcpy(leisure,&outbuf[10],5);
		memcpy(gB29Buf,outbuf,16);
#ifdef DEBUG_TRANSPORT
		printf("\nread gB29Buf:");
		for(i=0;i<16;i++) 
		{
			printf("%02X ",gB29Buf[i]);
		}
		printf("\n");
#endif

		if(gBFirst == 32)
		{
			//保存上次交易金额
			gCardLastTransValue = 0;
			*((char *)&gCardLastTransValue+INT_HIGH) = gB32[10]; //INT_HIGH
			*((char *)&gCardLastTransValue+INT_LOW) = gB32[9];
		}
		if(gBFirst == 33)
		{
			//保存上次交易金额
			gCardLastTransValue = 0;
			*((char *)&gCardLastTransValue+INT_HIGH) = gB33[10]; //INT_HIGH
			*((char *)&gCardLastTransValue+INT_LOW) = gB33[9];
		}
#ifdef DEBUG_TRANSPORT
		printf("\x0d\x0a gBFirst=%d gCardLastTransValue = %08X ",gBFirst,gCardLastTransValue);
#endif
	}
	else
	{//read 1A
		ret=Transport_GetFiles1A(out_buf);
#ifdef DEBUG_TRANSPORT
		printf("\nread 1A ret :%d\n",ret);
#endif
		if(ret!=0)
		{
			return OP_ERR;
		}
		//保存上次交易金额
		gCardLastTransValue = 0;
		*((char *)&gCardLastTransValue+LONG_HIGH0) = transport_1A_data[98]; 
		*((char *)&gCardLastTransValue+LONG_HIGH1) = transport_1A_data[97];
		*((char *)&gCardLastTransValue+LONG_HIGH2) = transport_1A_data[96];
		*((char *)&gCardLastTransValue+LONG_HIGH3) = 0;
#ifdef DEBUG_TRANSPORT
		printf("\ngCardLastTransValue :%08x\n",gCardLastTransValue);
		printf("transport_1A_data[95-98]..1:%02x %02x %02x %02x\n",transport_1A_data[95],transport_1A_data[96],transport_1A_data[97],transport_1A_data[98]);
#endif
		//CRC32校验
		CRC32_Value=Get_CRC32(&transport_1A_data[6],118);		
		LongToByte(CRC32_Value,&temp_crc[0]);
#ifdef DEBUG_TRANSPORT
		printf("\nCRC32_Value :%04x\n",CRC32_Value);
		printf("temp_crc :%02x%02x%02x%02x\n",temp_crc[0],temp_crc[1],temp_crc[2],temp_crc[3]);
#endif
		if(memcmp(temp_crc,&transport_1A_data[124],4)!=0)
		{
			printf("\nCRC32...Err!\n");
			memset(&transport_1A_data[6],0x00,122);
		}
		
		gStationTimeOverRideFlag = transport_1A_data[120];
		//printf("\nInfo--gStationTimeOverRideFlag...2:%02x\n",gStationTimeOverRideFlag);
	}
	
		//异地卡白名单判断
//	if( memcmp(&transport_15_data[0], SH_CODE_ORGANIZATION, 4) != 0)
//	{
//		//LongToByte(tpWhite4020.white_head.ContentNumber,(unsigned char *)&tpWhite4020.white_head.ContentNumber);
//		printf("\nContentNumber :%04x",tpWhite4020.white_head.ContentNumber);
//		for(i=0;i<tpWhite4020.white_head.ContentNumber;i++)
//		{
//			printf("\ni...=:%d\n",i);
//			printf("CardIssuer :%02x%02x%02x%02x %02x%02x%02x%02x\n",
//				tpWhite4020.White_Content_val[i].CardIssuer[0],tpWhite4020.White_Content_val[i].CardIssuer[1],tpWhite4020.White_Content_val[i].CardIssuer[2],tpWhite4020.White_Content_val[i].CardIssuer[3],
//				tpWhite4020.White_Content_val[i].CardIssuer[4],tpWhite4020.White_Content_val[i].CardIssuer[5],tpWhite4020.White_Content_val[i].CardIssuer[6],tpWhite4020.White_Content_val[i].CardIssuer[7]);
//			
//			if(memcmp(&tpWhite4020.White_Content_val[i].CardIssuer,&transport_15_data[0],8)==0)
//			{
//				goto label_read;
//			}
//					
//		}
//		gResponseStatus=0x27;
//		gGetCardInfoStep=0xff;
//		goto label_err;
//	}
//
//label_read:
	
	//read 1E
//	ret=Transport_GetFiles1E(1, out_buf);
//	if(ret<0)
//	{
//		return OP_ERR;
//	}
	
	gCardThisTransValue = 0;
	ret = init_for_capp_purchase(1,gCardThisTransValue,ch_transport_psam_id,outbuf);
#ifdef DEBUG_TRANSPORT
	printf("\ngetinfo init...ret :%d\n",ret);
#endif 	
	
	if (ret!=0) 
	{
		/*if(oldcmd == 0x23){
			oldcmd = 0x30;
			printf("\nOverwrite0x30to0x23\n");
			printf("\nOverwrite0x30to0x23\n");
			printf("\nOverwrite0x30to0x23\n");
			printf("\nOverwrite0x30to0x23\n");
			printf("\nOverwrite0x30to0x23\n");
			printf("\nOverwrite0x30to0x23\n");	
		}*/
		//outbuf[0] = oldcmd;
		outbuf[0] = 0x30;
		outbuf[1] = gReadCmdTimes;
		outbuf[2] = CE_READERROR;
		cnt = 3;
		shmt_prot_send(outbuf,cnt);
		return OP_ERR;
	}
	return 0;
	
}

/************************************************************************************************
函数：sh_transport_entry
功能：交通部进站
************************************************************************************************/
char sh_transport_entry(void)
{
	int i,ret;
	long balance;
	UBYTE mac1[4],mac2[4];
	UBYTE inbuf[256];
	UBYTE inbytes;
	UBYTE outbuf[256];
	UBYTE outbytes;
	UDWORD temp2,tint;//temp2:进出标志临时变量
	UBYTE buf[60];
	UBYTE buf32[16],buf33[3];//buf33:交易金额
	UBYTE in_bcd[3],out_bcd[8];//BCD格式交易流水号
	
#ifdef DEBUG_TRANSPORT
	printf("\n***sh_transport_entry***\n");
#endif 	

/*//read balance---P1 00表示含透支金额  修改为03可以读取不含透支金额
	memcpy(inbuf, "\x80\x5c\x03\x02\x04", 5);
	ret = mifpro_icmd(inbuf, 5, outbuf, &outbytes);
	if(ret != 0)
	{
		return CE_READERROR;
	}
#ifdef DEBUG_TRANSPORT
	printf("read balance return len %d ", outbytes);
	for(i=0;i<outbytes;i++)
	{
		printf(" %02x",outbuf[i]);
	}
	printf("\n");
#endif

	if(outbytes != 6)
	{
		return CE_INVALIDCARD;
	}
	ByteToLong(&balance, outbuf);
#ifdef DEBUG_TRANSPORT
	printf("cpu balance:%d\n",&balance);
#endif*/
	
	//read 15
	ret=Transport_GetFiles15(outbuf);

	if(ret<0)
	{
		return OP_ERR;
	}
	
	//交通部异地卡白名单判断
/*	if( memcmp(&transport_15_data[0], SH_CODE_ORGANIZATION, 4) != 0)
	{
		//LongToByte(tpWhite4020.white_head.ContentNumber,(unsigned char *)&tpWhite4020.white_head.ContentNumber);
		printf("\nContentNumber :%04x",tpWhite4020.white_head.ContentNumber);
		for(i=0;i<tpWhite4020.white_head.ContentNumber;i++)
		{
			printf("\ni...=:%d\n",i);
			printf("CardIssuer :%02x%02x%02x%02x %02x%02x%02x%02x\n",
				tpWhite4020.White_Content_val[i].CardIssuer[0],tpWhite4020.White_Content_val[i].CardIssuer[1],tpWhite4020.White_Content_val[i].CardIssuer[2],tpWhite4020.White_Content_val[i].CardIssuer[3],
				tpWhite4020.White_Content_val[i].CardIssuer[4],tpWhite4020.White_Content_val[i].CardIssuer[5],tpWhite4020.White_Content_val[i].CardIssuer[6],tpWhite4020.White_Content_val[i].CardIssuer[7]);
			
			if(memcmp(&tpWhite4020.White_Content_val[i].CardIssuer,&transport_15_data[0],8)==0)
			{
				goto label_trans;
			}
					
		}
		gResponseStatus=0x27;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
label_trans:
*/
	gCardThisTransValue = 0;//本次交易金额

	ret = init_for_capp_purchase(1,gCardThisTransValue,ch_transport_psam_id,outbuf);
#ifdef DEBUG_TRANSPORT
	printf("Entry init...ret: %d\n",ret);
#endif
	if(ret!=0)
	{
		gResponseStatus=CE_WRITEERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
	
	gCardThisRemain = gCardLastRemain - gCardThisTransValue;//交易后余额
#ifdef DEBUG_TRANSPORT
	printf("gCardLastRemain :%04x\n",gCardLastRemain);
	printf("gCardThisTransValue :%04x\n",gCardThisTransValue);
	printf("gCardThisRemain :%04x\n",gCardThisRemain);
#endif
	
//cal MAC1
	memset(inbuf,0x00,256);
	memcpy(inbuf,"\x80\x70\x00\x00\x24",5);
	//ICC：随机数
	memcpy(&inbuf[5],&capp_init[11],4);
	//Offline transaction No.：脱机交易序号
	memcpy(&inbuf[9],&capp_init[4],2);
	//transaction amount：交易金额
	memset(&inbuf[11],0x00,4);
	//复合消费交易类型
	inbuf[15] =	0x09;
	//trans Time：交易日期及时间
	memcpy(&inbuf[16],gTransTime,7);
	//key version：密钥版本
	inbuf[23] = capp_init[9];
	//cal ID：算法标识
	inbuf[24] = capp_init[10];
	//app sn：卡应用序列号
	memcpy(&inbuf[25],&transport_15_data[12],8);
	//logicid：发卡机构代码
	memcpy(&inbuf[33],transport_15_data,4);
	//fixed value：固定值
	memcpy(&inbuf[37],"\xFF\xFF\xFF\xFF",4);

	memcpy(bgTaskKeyInbuf,&inbuf[5],36);
	set_task_key(TASK_KEY_GET_JTBKEYA);		
	
	memcpy(mac1,&bgTaskKeyOutbuf[4],4);	
#ifdef DEBUG_TRANSPORT	
	printf("bgTaskKeyOutbuf :");
	for(i=0;i<8;i++)
	{
		printf(" %02x",bgTaskKeyOutbuf[i]);
	}
	printf("\n");
#endif
	//printf("mac1 :%02x %02x %02x %02x\n",mac1[0],mac1[1],mac1[2],mac1[3]);

//UPDATE CAPP DATA CACHE
	
	if( memcmp(&transport_15_data[0], SH_CODE_ORGANIZATION, 4) == 0)
	{//本地卡
		
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
		
		memcpy(buf32,buf,16);
		
		//更改本地卡地铁复合消费文件：0x11
		if(gStationTimeOverRideFlag == 0x00) 
		{
			buf[0] = 0x41;		
		}
		else 
		{
			if(gStationTimeOverRideFlag==0x18)
			{
				if(temp2&0x08)
				{
					buf[0]=	0x78;
				}
				else
				{	
					buf[0]= 0x71;
				}
			}
			else
			{
				buf[0]=gStationTimeOverRideFlag<<1;
				buf[0]+=0x40;
				if(buf[0]==0x50)
				{
					if(temp2&0x08)
					{
						buf[0]= 0x59;
					}
				}
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
		lrc_gen2(buf,15);
		ret = write_record(0x11,0x01,16,buf,0);
		if(ret)
		{
			gResponseStatus=CE_WRITEERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		
		memset(buf,0,16);
		buf[0] = 0x55;
		//buf[15] = 0x55;
		lrc_gen2(buf, 15);
#ifdef DEBUG_TRANSPORT
		printf("\nEn_write_record 11-03 :");
		for(i=0;i<16;i++)
		{
			printf("%02x ",buf[i]);
		}
		printf("\n");
#endif
		ret = write_record(0x11,0x03,16,buf,0);
		if(ret)
		{
			gResponseStatus=CE_WRITEERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		
		//产生卡记录
		memset(&inbuf,16,0x00);
		trans_gen_card_record(0x11,inbuf);
#ifdef DEBUG_TRANSPORT
		printf("\n0x07 En--write inbuf :");
		for(i=0;i<16;i++)
		{
			printf("%02x ",inbuf[i]);
		}
		printf("\n");
#endif
		//更新本地卡交易记录补充文件：0x07
		ret = write_record(0x07,0x01,16,inbuf,0);
		if(ret)
		{
			gResponseStatus=CE_WRITEERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
	}
	else	
	{//异地卡	

		if(gStationTimeOverRideFlag&0x08)//换乘进站
		{
			//交易流水号
			in_bcd[0]=*((char *)&gPosTransCnt+LONG_HIGH2);
			in_bcd[1]=*((char *)&gPosTransCnt+LONG_HIGH1);
			in_bcd[2]=*((char *)&gPosTransCnt+LONG_HIGH0);
			HexToBcd(in_bcd,out_bcd);
#ifdef DEBUG_TRANSPORT
			printf("\nin_bcd :%02x %02x %02x\n",in_bcd[0],in_bcd[1],in_bcd[2]);			
			printf("out_bcd :%02x %02x %02x %02x %02x %02x %02x %02x\n",out_bcd[0],out_bcd[1],out_bcd[2],out_bcd[3],out_bcd[4],out_bcd[5],out_bcd[6],out_bcd[7]);
#endif
			memcpy(&transport_1A_data[6],out_bcd,8);
			//trans status
			transport_1A_data[14] = 0x01;
			//city code
			memcpy(&transport_1A_data[15],SH_CODE_CITY,2);
			//max consume transaction amount
			memset(&transport_1A_data[81],0x00,4);
			//entry line code
			transport_1A_data[85] = 0x00;
			//entry transaction amount
			memset(&transport_1A_data[87], 0x00, 4);
			//entry Wallet balance
			memset(&transport_1A_data[91],0x00,4);
			//末笔交易时间
			memcpy(&transport_1A_data[99],gTransTime,7);
			//末笔轨道交通终端编号
			memcpy(&transport_1A_data[106],gPosNo,4);
			if((transport_1A_data[75]!=gTransTime[1])||(transport_1A_data[76]!=gTransTime[2]))
			{
				//累计交易金额
				memset(&transport_1A_data[110],0x00,3);
				//累计交易笔数
				memset(&transport_1A_data[113],0x00,2);
			}
			//换乘信息（行业信息+换乘时间 MMDDHHmm）
			//transport_1A_data[115]=(0xC0^gPosNo[0]);
			//memcpy(&transport_1A_data[116],&gTransTime[2],4);
			//进站交易标志
			//transport_1A_data[120]=gStationTimeOverRideFlag;		
			if(gStationTimeOverRideFlag==0x18)
			{
				if(transport_1A_data[121]&0x08)
				{
					transport_1A_data[120]=	0x78;
				}
				else
				{
					transport_1A_data[120]= 0x71;
				}
			}
			else
			{
				transport_1A_data[120]=gStationTimeOverRideFlag<<1;
				transport_1A_data[120]+=0x40;
				if(transport_1A_data[120]==0x50)
				{	
					if(transport_1A_data[121]&0x08)
					{
						transport_1A_data[120]= 0x59;
					}
				}
			}		
			//进出状态
			transport_1A_data[122]=0x55;
			//RFU
			transport_1A_data[123]=0x00;
		}	
		else
		{
			//更改异地城市轨道应用信息记录文件:0x1A
			//交易流水号
			in_bcd[0]=*((char *)&gPosTransCnt+LONG_HIGH2);
			in_bcd[1]=*((char *)&gPosTransCnt+LONG_HIGH1);
			in_bcd[2]=*((char *)&gPosTransCnt+LONG_HIGH0);
			HexToBcd(in_bcd,out_bcd);
#ifdef DEBUG_TRANSPORT
			printf("\nin_bcd :%02x %02x %02x\n",in_bcd[0],in_bcd[1],in_bcd[2]);			
			printf("out_bcd :%02x %02x %02x %02x %02x %02x %02x %02x\n",out_bcd[0],out_bcd[1],out_bcd[2],out_bcd[3],out_bcd[4],out_bcd[5],out_bcd[6],out_bcd[7]);
#endif
			memcpy(&transport_1A_data[6],out_bcd,8);
			//trans status
			transport_1A_data[14] = 0x01;
			//city code
			memcpy(&transport_1A_data[15],SH_CODE_CITY,2);
			//oragination code
			memset(&transport_1A_data[19],0x00,7);
			transport_1A_data[26]	=	gPosNo[0];
			//entry station code
			memset(&transport_1A_data[35], 0x00,7);
			transport_1A_data[42]	=	gPosNo[1];
			//entry station device 
			memset(&transport_1A_data[51],0x00,6);
			transport_1A_data[57]	=	gPosNo[2];
			transport_1A_data[58]	=	gPosNo[3];
			//entry time
			memcpy(&transport_1A_data[67],gTransTime,7);
			//max consume transaction amount
			memset(&transport_1A_data[81],0x00,4);
			//entry line code
			transport_1A_data[85] = 0x00;
			//entry transaction amount
			memset(&transport_1A_data[87], 0x00, 4);
			//entry Wallet balance
			memset(&transport_1A_data[91],0x00,4);
			//Actual transaction amount
			memset(&transport_1A_data[95],0x00,4);
			//末笔交易时间
			memcpy(&transport_1A_data[99],gTransTime,7);
			//末笔轨道交通终端编号
			memcpy(&transport_1A_data[106],gPosNo,4);
			if((transport_1A_data[75]!=gTransTime[1])||(transport_1A_data[76]!=gTransTime[2]))
			{
				//累计交易金额
				memset(&transport_1A_data[110],0x00,3);
				//累计交易笔数
				memset(&transport_1A_data[113],0x00,2);
			}
			//换乘信息（行业信息+换乘时间 MMDDHHmm）
			//transport_1A_data[115]=(0xC0^gPosNo[0]);
			//memcpy(&transport_1A_data[116],&gTransTime[2],4);
			//进站交易标志
			if(gStationTimeOverRideFlag == 0x10)//10:超时忽略正常进站
			{
				transport_1A_data[120]=0x60;
			}
			else
			{
				transport_1A_data[120]=0x41;
			}
			//进出状态
			transport_1A_data[122]=0x55;
			//RFU
			transport_1A_data[123]=0x00;
		}
		//CRC32
		CRC32_Value=0;
		memset(temp_crc,0x00,4);
		CRC32_Value=Get_CRC32(&transport_1A_data[6],118);
		LongToByte(CRC32_Value,&temp_crc[0]);
#ifdef DEBUG_TRANSPORT		
		printf("\nEn-CRC32_Value :%04x\n",CRC32_Value);
		printf("En-temp_crc :%02x%02x%02x%02x\n",temp_crc[0],temp_crc[1],temp_crc[2],temp_crc[3]);
#endif
		memcpy(&transport_1A_data[124],temp_crc,4);
		
		ret=write_capp_record(0x1A,transport_1A_data[1],TRANSPORT_1A_LEN,transport_1A_data,0);
		if(ret)
		{
			printf("write_capp_record...error!");
			gResponseStatus=CE_WRITEERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
	}
	
	//更改异地公共交通过程信息循环记录文件：0x1E
	//交易类型
	transport_1E_data[0]=0x03;
	//终端编号
	memset(&transport_1E_data[1],0x00,4);
	memcpy(&transport_1E_data[5],gPosNo,4);
	//行业代码
	transport_1E_data[9]=0x01;
	//线路
	transport_1E_data[10]=0x00;
	transport_1E_data[11]=gPosNo[0];
	//站点
	transport_1E_data[12]=0x00;
	transport_1E_data[13]=gPosNo[1];
	//运营代码
	memset(&transport_1E_data[14],0x00,2);//没找到定义，先填0
	//RFU
	transport_1E_data[16]=0x00;
	//交易金额：进站为0
	memset(&transport_1E_data[17],0x00,4);
	//交易后余额
	LongToByte(gCardThisRemain,&transport_1E_data[21]);
	//交易日期时间
	memcpy(&transport_1E_data[25],gTransTime,7);
	//受理方城市代码
	memcpy(&transport_1E_data[32],SH_CODE_CITY,2);
	//受理方机构标识
	memcpy(&transport_1E_data[34],HANDLE_CODE_ORGANIZATION,8);
	//RFU
	memset(&transport_1E_data[42],0x00,6);
		
	ret=write_capp_record(0x1E,0x00,TRANSPORT_1E_LEN,transport_1E_data,0);
	if(ret)
	{
		printf("write_capp_record...error!");
		gResponseStatus=CE_WRITEERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}

/*//DEBIT FOR CAPP PURCHASE
	memset(inbuf,0x00,sizeof(inbuf));
	memcpy(inbuf,"\x80\x54\x01\x00\x0F",5);
	memcpy(&inbuf[5],bgTaskKeyOutbuf,4);
	memcpy(&inbuf[9],gTransTime,7);
	memcpy(&inbuf[16],mac1,4);

#ifdef DEBUG_TRANSPORT
	printf("\ndebit for capp purchase inbuf :");	
	for(i=0;i<20;i++)
	{
		printf(" %02x",inbuf[i]);
	}
	printf("\n");
#endif
	memset(outbuf,0,sizeof(outbuf));
	ret = mifpro_icmd0(20,inbuf,outbuf);
	if(ret<0)
	{
		gResponseStatus = CE_CARDREMOVED;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
#ifdef DEBUG_TRANSPORT
	printf("debit for capp purchase...ret = %d\n",ret);
	printf("outbuf :");	
	for(i=0;i<ret;i++)
	{
		printf(" %02x",outbuf[i]);
	}
	printf("\n");
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
#ifdef DEBUG_TRANSPORT
	printf("\ngTAC :%02x %02x %02x %02x\n",gTAC[0],gTAC[1],gTAC[2],gTAC[3]);
#endif
	memcpy(mac2,&outbuf[4],4);
	gDebugStep = 0x3292;*/
	
//debit for capp purchase
	ret = debit_for_capp_purchase(bgTaskKeyOutbuf,outbuf);
#ifdef DEBUG_TRANSPORT
	printf("Entry debit...ret :%d\n",ret);
	//printf("debit...outlen:%d\n",outlen);
#endif
	if(ret<0)
	{
		gResponseStatus = CE_CARDREMOVED;
		gGetCardInfoStep=0xff;
		return OP_ERR;//失败时 返回闪卡
	}
	if(ret !=0 )
	{	
		goto label_writeerr;
	}
	memcpy(gTAC,outbuf,4);
	memcpy(mac2,&outbuf[4],4);
	
/*//credit sam for purchase
	memset(inbuf,0x00,sizeof(inbuf));
	memcpy(inbuf,"\x80\x72\x00\x00\x04",5);
	memcpy(&inbuf[5],mac2,4);
	inbytes = 9;
	
#ifdef DEBUG_TRANSPORT
	printf("\ncredit sam for purchase inbuf :");
	for(i=0;i<9;i++)
	{
		printf(" %02x",inbuf[i]);
	}
	printf("\n");
#endif

	ret=sam_apdu(bgSHJTBpsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);

#ifdef DEBUG_TRANSPORT
	printf("credit sam for purchase...ret = %d\n",ret);
	printf("outbuf :");	
	for(i=0;i<outbytes;i++)
	{
		printf(" %02x",outbuf[i]);
	}
	printf("\n");
#endif

	if(ret!=0)
	{
		goto label_samerr;
	}
	
	gDebugStep = 0x3293;
	if((outbuf[outbytes-2]!=0x90)||(outbuf[outbytes-1]!=0x00))
	{
		gResponseStatus=CE_MAC2ERR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
*/	
	ret = credit_sam_for_purchase(bgSHJTBpsamIndex,mac2,outbuf);
#ifdef DEBUG_TRANSPORT
	printf("Entry credit...ret :%d\n",ret);
#endif
	if (ret!=0) 
	{
		return OP_ERR;
	}
	return 0;

label_readerr:
		gResponseStatus=CE_READERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_writeerr:
		gResponseStatus=CE_WRITEERROR;
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

/************************************************************************************************
函数：sh_transport_exit
功能：交通部出站
************************************************************************************************/
char sh_transport_exit(UBYTE offsetnum)
{
	
	int i,ret;
	long balance;
	UBYTE mac1[4],mac2[4];
	UBYTE inbuf[256];
	UBYTE inbytes;
	UBYTE outbuf[256];
	UBYTE outbytes;
	UDWORD temp2,tint,tint2;
	UBYTE buf[60];
	UBYTE in_bcd[3],out_bcd[8];//BCD格式交易流水号
	
	
#ifdef DEBUG_TRANSPORT
	printf("\n***sh_transport_exit***\n");
#endif 	

/*//read balance---P1 00表示含透支金额  修改为03可以读取不含透支金额
memcpy(inbuf, "\x80\x5c\x00\x02\x04", 5);
	ret = mifpro_icmd(inbuf, 5, outbuf, &outbytes);
	if(ret != 0)
	{
		return CE_READERROR;
	}
//#ifdef DEBUG_TRANSPORT
	printf("read balance return len %d \n", outbytes);
	for(i=0;i<outbytes;i++)
	{
		printf(" %02x",outbuf[i]);
	}
	printf("\n");
//#endif

	if(outbytes != 6)
	{
		return CE_INVALIDCARD;
	}
	//ByteToLong(&balance, outbuf);
	buf_to_udword(outbuf,balance);
#ifdef DEBUG_TRANSPORT
	printf("cpu balance:%04x\n",balance);
#endif*/

	ret=Transport_GetFiles15(outbuf);

	if(ret<0)
	{
		return OP_ERR;
	}
	
	ret = init_for_capp_purchase(1,gCardThisTransValue,ch_transport_psam_id,outbuf);
#ifdef DEBUG_TRANSPORT
	printf("\nExit init ret :%d\n",ret);
#endif
	if (ret!=0) 
	{
		gResponseStatus=CE_WRITEERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
	//交易后余额
	gCardThisRemain = gCardLastRemain - gCardThisTransValue;
#ifdef DEBUG_TRANSPORT
	printf("gCardLastRemain :%04x\n",gCardLastRemain);
	printf("gCardThisTransValue :%04x\n",gCardThisTransValue);
	printf("gCardThisRemain :%04x\n",gCardThisRemain);
#endif
	
//cal MAC1
	memset(inbuf,0x00,256);
	memcpy(inbuf,"\x80\x70\x00\x00\x24",5);
	//ICC
	memcpy(&inbuf[5],&capp_init[11],4);
	//Offline transaction No.
	memcpy(&inbuf[9],&capp_init[4],2);
	//transaction amount
	udword_to_buf(gCardThisTransValue,&inbuf[11]);
	inbuf[15] =	0x09;
	//trans Time
	memcpy(&inbuf[16],gTransTime,7);
	//key version
	inbuf[23] = capp_init[9];
	//cal ID
	inbuf[24] = capp_init[10];
	//app sn
	memcpy(&inbuf[25],&transport_15_data[12],8);
	//logicid
	memcpy(&inbuf[33],transport_15_data,4);
	//fixed value
	memcpy(&inbuf[37],"\xFF\xFF\xFF\xFF",4);

	memcpy(bgTaskKeyInbuf,&inbuf[5],36);
	set_task_key(TASK_KEY_GET_JTBKEYA);
	
	memcpy(mac1,&bgTaskKeyOutbuf[4],4);
	
#ifdef DEBUG_TRANSPORT	
	printf("bgTaskKeyOutbuf :");
	for(i=0;i<8;i++)
	{
		printf(" %02x",bgTaskKeyOutbuf[i]);
	}
	printf("\n");
#endif

//UPDATE CAPP DATA CACHE
	
	if( memcmp(&transport_15_data[0], SH_CODE_ORGANIZATION, 4) == 0)
	{//本地卡
		
		//更新本地卡记录文件
		if(offsetnum<13)
		{
			trans_gen_card_record(trans_id[offsetnum],inbuf);
		}
		else
		{
			trans_gen_card_record(0x11,inbuf);
		}
#ifdef DEBUG_TRANSPORT
		printf("\n0x07 Ex-write inbuf :");
		for(i=0;i<16;i++)
		{
			printf("%02x ",inbuf[i]);
		}
		printf("\n");
#endif
		//ret = write_record(0x07,0x00,16,inbuf,1);
		ret = write_record(0x07,0x01,16,inbuf,0);
#ifdef DEBUG_TRANSPORT
		printf("\nEx-write_record 0x07 ret =%d\n",ret);
#endif
		if(ret)
		{
			gResponseStatus=CE_WRITEERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
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
  	
		ret = write_record(0x10,0x02,16,buf,0);
#ifdef DEBUG_TRANSPORT
		printf("\nEx-write_record 0x10-02 ret =%d\n",ret);
#endif
		if(ret)
		{
		  gResponseStatus=CE_WRITEERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
  	
		ret = write_record(0x10,0x03,16,buf,0);
#ifdef DEBUG_TRANSPORT
		printf("\nEx-write_record 0x10-03 ret =%d\n",ret);
#endif
		if(ret)
		{
		  gResponseStatus=CE_WRITEERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		//更新卡上地铁专用扇区交易
		if((recordtime[0]!=gTransTime[1])||(recordtime[1]!=gTransTime[2]))
		{
			recordsum[0]=0;//地铁记录中累积金额
			recordsum[1]=0;
			recordsum[2]=0;
			recordcnt=0;//地铁记录中累积次数
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
#ifdef DEBUG_TRANSPORT
		printf("\noffsetnum :%02x",offsetnum);	
		printf("\nEx--gStationTimeOverRideFlag 1:%02x\n",gStationTimeOverRideFlag);
#endif
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
//		if(gStationTimeOverRideFlag&0x20)
//		{	
//			buf[0]=0xa0;
//		}
//		else
//		{
//			buf[0]=0x81;
//		}
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
  	ret = write_record(0x11,0x02,16,buf,0);
		if(ret)
		{
			gResponseStatus=CE_WRITEERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
  	
		memset(buf,0,16);
		buf[0] = 0x66;
		//memcpy(&buf[1], &gB34[1], 14);
		//buf[15] = 0x66;
		lrc_gen2(buf, 15);
#ifdef DEBUG_TRANSPORT
		printf("\nEx_write_record 11-03 :");
		for(i=0;i<16;i++)
		{
			printf("%02x ",buf[i]);
		}
		printf("\n");
#endif
		ret = write_record(0x11,0x03,16,buf,0);
		if(ret)
		{
			gResponseStatus=CE_WRITEERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
	
	}
	else	
	{//异地卡					
//更改异地卡消费记录文件：0x1A
		//交易流水号
		in_bcd[0]=*((char *)&gPosTransCnt+LONG_HIGH2);
		in_bcd[1]=*((char *)&gPosTransCnt+LONG_HIGH1);
		in_bcd[2]=*((char *)&gPosTransCnt+LONG_HIGH0);
		HexToBcd(in_bcd,out_bcd);
		memcpy(&transport_1A_data[6],out_bcd,8);
		//trans status
		transport_1A_data[14] = 0x02;
		//city code
		memcpy(&transport_1A_data[17],SH_CODE_CITY,2);
		//oragination code
		memset(&transport_1A_data[27],0x00,7);
		transport_1A_data[34]	=	gPosNo[0];
		//exit station code
		memset(&transport_1A_data[43],0x00,7);
		transport_1A_data[50]	=	gPosNo[1];
		//exit station device code
		memset(&transport_1A_data[59],0x00,6);
		transport_1A_data[65]	=	gPosNo[2];
		transport_1A_data[66] =	gPosNo[3];
		//exit time
		memcpy(&transport_1A_data[74],gTransTime,7);
		//exit line code
		transport_1A_data[86] = 0x00;
		//实际交易金额
		tint2=gCardLastTransValue+(UWORD)gCardThisTransValue;		

		transport_1A_data[95]=*((char *)&tint2+LONG_HIGH3);
		transport_1A_data[96]=*((char *)&tint2+LONG_HIGH2);
		transport_1A_data[97]=*((char *)&tint2+LONG_HIGH1);
		transport_1A_data[98]=*((char *)&tint2+LONG_HIGH0);
#ifdef DEBUG_TRANSPORT
		printf("\tint2 :%04x\n",tint2);
		printf("transport_1A_data[95-98]..2:%02x %02x %02x %02x\n",transport_1A_data[95],transport_1A_data[96],transport_1A_data[97],transport_1A_data[98]);
#endif
		//末笔交易时间
		memcpy(&transport_1A_data[99],gTransTime,7);
		//末笔轨道交通终端编号
		memcpy(&transport_1A_data[106],gPosNo,4);
		//累计交易金额
		if((transport_1A_data[68]!=gTransTime[1])||(transport_1A_data[69]!=gTransTime[2]))
		{
			recordsum[0]=0;
			recordsum[1]=0;
			recordsum[2]=0;
			recordcnt=0;
		}
		*((char *)&temp2+LONG_HIGH3)=0;
		*((char *)&temp2+LONG_HIGH2)=recordsum[2];
		*((char *)&temp2+LONG_HIGH1)=recordsum[1];
		*((char *)&temp2+LONG_HIGH0)=recordsum[0];
		if(gCardThisTransValue>(0xffffff-temp2))
		{
			temp2=0xffffff;
		}
		else
		{
			temp2=temp2+gCardThisTransValue;
		}
		transport_1A_data[112]=*((char *)&temp2 +LONG_HIGH0);
		transport_1A_data[111]=*((char *)&temp2 +LONG_HIGH1);
		transport_1A_data[110]=*((char *)&temp2 +LONG_HIGH2);
		//累计交易笔数
		if(recordcnt!=0xff)
		{
			recordcnt++;
		}
		transport_1A_data[114]=recordcnt;
		//换乘信息（行业信息+换乘时间 MMDDHHmm）
		transport_1A_data[115]=(0xC0^gPosNo[0]);
		memcpy(&transport_1A_data[116],&gTransTime[2],4);
		//出站交易标志
		//printf("\nEx--gStationTimeOverRideFlag...2:%02x\n",gStationTimeOverRideFlag);
		if((offsetnum > 0x05)&&(offsetnum < 10))
		{
			if(gStationTimeOverRideFlag & 0x20)
				transport_1A_data[121] = 0xa9;
			else
				transport_1A_data[121] = 0x88;
		}
		else
		{	
			if(gStationTimeOverRideFlag & 0x20)
				transport_1A_data[121] = 0xa0;
			else
				transport_1A_data[121] = 0x81;
		}
		//进出状态
		transport_1A_data[122] = 0x66;
		//RFU
		transport_1A_data[123] = 0x00;
		//CRC32
		CRC32_Value=0;
		memset(temp_crc,0x00,4);
		CRC32_Value=Get_CRC32(&transport_1A_data[6],118);
		LongToByte(CRC32_Value,&temp_crc[0]);
#ifdef DEBUG_TRANSPORT		
		printf("\nEx-CRC32_Value :%04x\n",CRC32_Value);
		printf("Ex-temp_crc :%02x%02x%02x%02x\n",temp_crc[0],temp_crc[1],temp_crc[2],temp_crc[3]);
#endif
		memcpy(&transport_1A_data[124],&temp_crc[0],4);
		
		ret=write_capp_record(0x1A,transport_1A_data[1],TRANSPORT_1A_LEN,transport_1A_data,0);
		if(ret)
		{
			printf("write_capp_record...error!");
			gResponseStatus=CE_WRITEERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
	}
	
//更改异地公共交通过程信息循环记录文件：0x1E
	//交易类型
	transport_1E_data[0]=0x04;
	//终端编号
	memset(&transport_1E_data[1],0x00,4);
	memcpy(&transport_1E_data[5],gPosNo,4);
	//行业代码
	transport_1E_data[9]=0x01;
	//线路
	transport_1E_data[10]=0x00;
	transport_1E_data[11]=gPosNo[0];
	//站点
	transport_1E_data[12]=0x00;
	transport_1E_data[13]=gPosNo[1];
	//运营代码
	memset(&transport_1E_data[14],0x00,2);//
	//RFU
	transport_1E_data[16]=0x00;
	//交易金额：进站为0
	//memset(&transport_1E_data[17],0x00,4);
	LongToByte(gCardThisTransValue,&transport_1E_data[17]);
	//交易后余额
	LongToByte(gCardThisRemain,&transport_1E_data[21]);
	//交易日期时间
	memcpy(&transport_1E_data[25],gTransTime,7);
	//受理方城市代码
	memcpy(&transport_1E_data[32],SH_CODE_CITY,2);
	//受理方机构标识
	memcpy(&transport_1E_data[34],HANDLE_CODE_ORGANIZATION,8);
	//RFU
	memset(&transport_1E_data[42],0x00,6);

	ret=write_capp_record(0x1E,0x00,TRANSPORT_1E_LEN,transport_1E_data,0);
	if(ret)
	{
		printf("write_capp_record...error!");
		gResponseStatus=CE_WRITEERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
	
/*//debit for capp purchase
	memset(inbuf,0x00,sizeof(inbuf));
	memcpy(inbuf,"\x80\x54\x01\x00\x0F",5);
	memcpy(&inbuf[5],bgTaskKeyOutbuf,4);
	memcpy(&inbuf[9],gTransTime,7);
	memcpy(&inbuf[16],mac1,4);

#ifdef DEBUG_TRANSPORT
	printf("\ndebit for capp purchase inbuf :");	
	for(i=0;i<20;i++)
	{
		printf(" %02x",inbuf[i]);
	}
	printf("\n");
#endif
	memset(outbuf,0,sizeof(outbuf));
	ret = mifpro_icmd0(20,inbuf,outbuf);
	if(ret<0)
	{
		gResponseStatus = CE_CARDREMOVED;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
#ifdef DEBUG_TRANSPORT
	printf("debit for capp purchase...ret = %d\n",ret);
	printf("outbuf :");	
	for(i=0;i<ret;i++)
	{
		printf(" %02x",outbuf[i]);
	}
	printf("\n");
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
	*/

//debit for capp purchase
	ret = debit_for_capp_purchase(bgTaskKeyOutbuf,outbuf);
#ifdef DEBUG_TRANSPORT
	printf("Exit debit ret :%d\n",ret);
#endif
	if(ret<0)
	{
		gResponseStatus = CE_CARDREMOVED;
		gGetCardInfoStep=0xff;
		return OP_ERR;//失败时 返回闪卡
	}
	if(ret !=0 )
	{	
		goto label_writeerr;
	}
	memcpy(gTAC,outbuf,4);
	memcpy(mac2,&outbuf[4],4);

/*//credit sam for purchase
	memset(inbuf,0x00,sizeof(inbuf));
	memcpy(inbuf,"\x80\x72\x00\x00\x04",5);
	memcpy(&inbuf[5],mac2,4);
	inbytes = 9;
	
#ifdef DEBUG_TRANSPORT
	printf("\ncredit sam for purchase inbuf :");
	for(i=0;i<9;i++)
	{
		printf(" %02x",inbuf[i]);
	}
	printf("\n");
#endif

	ret=sam_apdu(bgSHJTBpsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);

#ifdef DEBUG_TRANSPORT
	printf("credit sam for purchase...ret = %d\n",ret);
	printf("outbuf :");	
	for(i=0;i<outbytes;i++)
	{
		printf(" %02x",outbuf[i]);
	}
	printf("\n");
#endif

	if(ret!=0)
	{
		goto label_samerr;
	}
	
	gDebugStep = 0x3293;
	if((outbuf[outbytes-2]!=0x90)||(outbuf[outbytes-1]!=0x00))
	{
		gResponseStatus=CE_MAC2ERR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}*/
	
	ret = credit_sam_for_purchase(bgSHJTBpsamIndex,mac2,outbuf);
#ifdef DEBUG_TRANSPORT
	printf("Exit credit ret :%d\n",ret);
#endif
	if (ret!=0) 
	{
		return OP_ERR;
	}
	return 0;
label_readerr:
		gResponseStatus=CE_READERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_writeerr:
		gResponseStatus=CE_WRITEERROR;
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

/************************************************************************************************
函数：sh_transport_update
功能：交通部更新
************************************************************************************************/
char sh_transport_update(UBYTE update_type,UBYTE *rec_buf)
{
	int i,ret;
	long balance;
	UBYTE mac1[4],mac2[4];
	UBYTE inbuf[256];
	UBYTE inbytes;
	UBYTE outbuf[256];
	UBYTE outbytes;
	UBYTE buf[60];
	UBYTE in_bcd[3],out_bcd[8];//BCD格式交易流水号
	
#ifdef DEBUG_TRANSPORT
	printf("\n***sh_transport_update***\n");
#endif 	

//read 0x15
	ret=Transport_GetFiles15(outbuf);

	if(ret<0)
	{
		return OP_ERR;
	}
	
//init for capp purchase
	ret = init_for_capp_purchase(1,gCardThisTransValue,ch_transport_psam_id,outbuf);
#ifdef DEBUG_TRANSPORT
	printf("init ret :%d\n",ret);
#endif
	if (ret!=0) 
	{
		return OP_ERR;
	}
	gCardThisRemain = gCardLastRemain - gCardThisTransValue;//交易后余额
	
//cal MAC1
	memset(inbuf,0x00,256);
	memcpy(inbuf,"\x80\x70\x00\x00\x24",5);
	//ICC
	memcpy(&inbuf[5],&capp_init[11],4);
	//Offline transaction No.
	memcpy(&inbuf[9],&capp_init[4],2);
	//transaction amount
	udword_to_buf(gCardThisTransValue,&inbuf[11]);
	inbuf[15] =	0x09;
	//trans Time
	memcpy(&inbuf[16],gTransTime,7);
	//key version
	inbuf[23] = capp_init[9];
	//cal ID
	inbuf[24] = capp_init[10];
	//app sn
	memcpy(&inbuf[25],&transport_15_data[12],8);
	//logicid
	memcpy(&inbuf[33],transport_15_data,4);
	//fixed value
	memcpy(&inbuf[37],"\xFF\xFF\xFF\xFF",4);

	memcpy(bgTaskKeyInbuf,&inbuf[5],36);
	set_task_key(TASK_KEY_GET_JTBKEYA);
	
	memcpy(mac1,&bgTaskKeyOutbuf[4],4);
	
//UPDATE CAPP DATA CACHE
	if( memcmp(&transport_15_data[0], SH_CODE_ORGANIZATION, 4) == 0)
	{//本地卡
		
		trans_gen_card_record(0x14,buf);
		ret = write_record(0x07,0x00,16,buf,0);
		if(ret)
		{
			return OP_ERR;
		}
		
		if(update_type == 0x01)
		{//进站更新
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
		{//出站更新
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
	}
	else
	{//异地卡
		//更改异地消费记录文件
		if(update_type == 0x01)
		{//进站更新
			//更改异地卡消费记录文件：0x1A
			//交易流水号
			in_bcd[0]=*((char *)&gPosTransCnt+LONG_HIGH2);
			in_bcd[1]=*((char *)&gPosTransCnt+LONG_HIGH1);
			in_bcd[2]=*((char *)&gPosTransCnt+LONG_HIGH0);
			HexToBcd(in_bcd,out_bcd);
			memcpy(&transport_1A_data[6],out_bcd,8);
			//trans status
			transport_1A_data[14] = 0x03;
			//city code
			memcpy(&transport_1A_data[15],SH_CODE_CITY,2);
			//oragination code
			memset(&transport_1A_data[19],0x00,7);
			transport_1A_data[26]	=	rec_buf[6];
			//entry station code
			memset(&transport_1A_data[35], 0x00,7);
			transport_1A_data[42]	=	rec_buf[7];
			//entry station device 
			memset(&transport_1A_data[51],0x00,6);
			transport_1A_data[57]	=	gPosNo[2];
			transport_1A_data[58]	=	rec_buf[8];
			//entry time
			//memcpy(&transport_1A_data[67],gTransTime,7);
			transport_1A_data[67]=gTransTime[0];
			memcpy(&transport_1A_data[68],&rec_buf[1],5);
			transport_1A_data[73]=gTransTime[6];
			//max consume transaction amount
			memset(&transport_1A_data[81],0x00,4);
			//entry line code
			transport_1A_data[85] = 0x00;
			//entry transaction amount
			memset(&transport_1A_data[87], 0x00, 4);
			//entry Wallet balance
			memset(&transport_1A_data[91],0x00,4);
			//Actual transaction amount
			//memset(&transport_1A_data[95],0x00,4);
			LongToByte(gCardThisTransValue, &transport_1A_data[95]);
			//末笔交易时间
			memcpy(&transport_1A_data[99],gTransTime,7);
			//末笔轨道交通终端编号
			memcpy(&transport_1A_data[106],gPosNo,4);
			//累计交易金额
			transport_1A_data[110] = rec_buf[14];
			transport_1A_data[111] = rec_buf[13];
			transport_1A_data[112] = rec_buf[12];
			//累计交易笔数
			transport_1A_data[113]=0x00;
			transport_1A_data[114]=rec_buf[11];

			//交易标志
			transport_1A_data[120]=rec_buf[0];
			//进出状态
			transport_1A_data[122]=0x55;
			//RFU
			transport_1A_data[123]=0x00;
			//CRC32
			CRC32_Value=0;
			memset(temp_crc,0x00,4);
			CRC32_Value=Get_CRC32(&transport_1A_data[6],118);
			LongToByte(CRC32_Value,&temp_crc[0]);
			memcpy(&transport_1A_data[124],&temp_crc[0],4);
		}
		else
		{//出站更新
			
			//交易流水号
			in_bcd[0]=*((char *)&gPosTransCnt+LONG_HIGH2);
			in_bcd[1]=*((char *)&gPosTransCnt+LONG_HIGH1);
			in_bcd[2]=*((char *)&gPosTransCnt+LONG_HIGH0);
			HexToBcd(in_bcd,out_bcd);
			memcpy(&transport_1A_data[6],out_bcd,8);
			//更改异地卡消费记录文件：0x1A
			//trans status
			transport_1A_data[14] = 0x04;
			//city code
			memcpy(&transport_1A_data[17],SH_CODE_CITY,2);
			//oragination code
			memset(&transport_1A_data[27],0x00,7);
			transport_1A_data[34]	=	rec_buf[6];
			//exit station code
			memset(&transport_1A_data[43],0x00,7);
			transport_1A_data[50]	=	rec_buf[7];
			//exit station device code
			memset(&transport_1A_data[59],0x00,6);
			transport_1A_data[65]	=	gPosNo[2];
			transport_1A_data[66] =	rec_buf[8];
			//exit time
			transport_1A_data[74] = gTransTime[0];
			memcpy(&transport_1A_data[75],&rec_buf[1],5);
			transport_1A_data[80] = gTransTime[6];
			//exit line code
			transport_1A_data[86] = 0x00;
			//实际交易金额
			transport_1A_data[95] = 0;
			transport_1A_data[96] = 0;
			transport_1A_data[97] = rec_buf[10];
			transport_1A_data[98] = rec_buf[9];
			//末笔交易时间
			memcpy(&transport_1A_data[99],gTransTime,7);
			//末笔轨道交通终端编号
			memcpy(&transport_1A_data[106],gPosNo,4);
			//累计交易金额
			//memcpy(&transport_1A_data[110],&rec_buf[12],3);
			transport_1A_data[110] = rec_buf[14];
			transport_1A_data[111] = rec_buf[13];
			transport_1A_data[112] = rec_buf[12];
			//累计交易笔数
			transport_1A_data[113]=0x00;
			transport_1A_data[114]=rec_buf[11];
			//RFU
			memset(&transport_1A_data[115],0x00,5);
			//交易标志
			transport_1A_data[121]=rec_buf[0];
			//进出状态
			transport_1A_data[122]=0x66;
			//RFU
			transport_1A_data[123]=0x00;
			//CRC32
			CRC32_Value=0;
			memset(temp_crc,0x00,4);
			CRC32_Value=Get_CRC32(&transport_1A_data[6],118);
			LongToByte(CRC32_Value,&temp_crc[0]);
			memcpy(&transport_1A_data[124],&temp_crc[0],4);
		}
	
		ret=write_capp_record(0x1A,transport_1A_data[1],TRANSPORT_1A_LEN,transport_1A_data,0);
		if(ret)
		{
			printf("write_capp_record...error!");
			gResponseStatus=CE_WRITEERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
	}
	
	//更改异地公共交通过程信息循环记录文件：0x1E
	//交易类型
	if(update_type=0x01)
	{
		transport_1E_data[0]=0x03;
	}
	else
	{
		transport_1E_data[0]=0x04;
	}
	//终端编号
	memset(&transport_1E_data[1],0x00,4);
	//memcpy(&transport_1E_data[5],gPosNo,4);
	transport_1E_data[5]=rec_buf[6];
	transport_1E_data[6]=rec_buf[7];
	transport_1E_data[7]=gPosNo[2];
	transport_1E_data[8]=rec_buf[8];
	//行业代码
	transport_1E_data[9]=0x01;
	//线路
	transport_1E_data[10]=0x00;
	transport_1E_data[11]=rec_buf[6];
	//站点
	transport_1E_data[12]=0x00;
	transport_1E_data[13]=rec_buf[7];
	//运营代码
	memset(&transport_1E_data[14],0x00,2);
	//RFU
	transport_1E_data[16]=0x00;
	//交易金额
	//memset(&transport_1E_data[17],0x00,4);
	LongToByte(gCardThisTransValue, &transport_1E_data[17]);
	//交易后余额
	LongToByte(gCardThisRemain,&transport_1E_data[21]);
	//交易日期时间
	memcpy(&transport_1E_data[25],gTransTime,7);
	//受理方城市代码
	memcpy(&transport_1E_data[32],SH_CODE_CITY,2);
	//受理方机构标识
	memcpy(&transport_1E_data[34],HANDLE_CODE_ORGANIZATION,8);
	//RFU
	memset(&transport_1E_data[42],0x00,6);

	ret=write_capp_record(0x1E,0x00,TRANSPORT_1E_LEN,transport_1E_data,0);
	if(ret)
	{
		printf("write_capp_record...error!");
		gResponseStatus=CE_WRITEERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
	
//debit for capp purchase	
	ret = debit_for_capp_purchase(bgTaskKeyOutbuf,outbuf);
#ifdef DEBUG_TRANSPORT	
	printf("debit...ret :%d\n",ret);
#endif
	if (ret!=0) 
	{
		return OP_ERR;
	}
	memcpy(gTAC,outbuf,4);
	memcpy(mac2,&outbuf[4],4);
	
//credit sam for purchase	
	ret = credit_sam_for_purchase(bgSHJTBpsamIndex,mac2,outbuf);
#ifdef DEBUG_TRANSPORT
	printf("credit...ret :%d\n",ret);
#endif
	if (ret!=0) 
	{
		return OP_ERR;
	}
	return 0;
	
}

/************************************************************************************************
函数：sh_transport_lockcard
功能：交通部锁卡
************************************************************************************************/
char sh_transport_lockcard(UBYTE lock_type)
{
	int i,ret;
	long balance;
	UBYTE mac[10],mac1[4],mac2[4];
	UBYTE inbuf[256];
	UBYTE inbytes;
	UBYTE outbuf[256];
	UBYTE outbytes;
	UDWORD temp2,tint;
	UBYTE buf[60];
	UBYTE cpubuf[256];
	UBYTE factor[16];
	UBYTE cpurandom[8];
	UWORD cnt;
	UBYTE out[300];
	
#ifdef DEBUG_TRANSPORT
	printf("\n***sh_transport_lockcard***\n");
#endif 

	if(lock_type != 0)
	{
/*//read balance---P1 00表示含透支金额  修改为03可以读取不含透支金额
		memcpy(inbuf, "\x80\x5c\x03\x02\x04", 5);
		ret = mifpro_icmd0( 5, inbuf,outbuf);
		
#ifdef DEBUG_TRANSPORT
		printf("\nread balance return len %d \n", ret);
		for(i=0;i<ret;i++)
		{
			printf(" %02x",outbuf[i]);
		}
		printf("\n");
#endif
		if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
		{
			return CE_READERROR;
		}
	
		if(ret != 6)
		{
			return CE_INVALIDCARD;
		}
		//ByteToLong(&balance, outbuf);
		buf_to_udword(outbuf,gCardThisTransValue);//交易金额
*/
		gCardThisTransValue = gCardLastRemain;
#ifdef DEBUG_TRANSPORT
		printf("\nLockCard gCardThisTransValue=%08x\n",gCardThisTransValue);
#endif
		//read 15
		ret=Transport_GetFiles15(outbuf);
		if(ret<0)
		{
			return OP_ERR;
		}
	
		//init for capp purchase
		ret = init_for_capp_purchase(1,gCardThisTransValue,ch_transport_psam_id,outbuf);
#ifdef DEBUG_TRANSPORT
		printf("init...ret: %d\n",ret);
#endif
		if (ret!=0) 
		{
			return OP_ERR;
		}
		gCardThisRemain = gCardLastRemain - gCardThisTransValue;
		
		//cal MAC1
		memset(inbuf,0x00,256);
		memcpy(inbuf,"\x80\x70\x00\x00\x24",5);
		//ICC
		memcpy(&inbuf[5],&capp_init[11],4);
		//Offline transaction No.
		memcpy(&inbuf[9],&capp_init[4],2);
		//transaction amount
		udword_to_buf(gCardThisTransValue,&inbuf[11]);
		inbuf[15] =	0x09;
		//trans Time
		memcpy(&inbuf[16],gTransTime,7);
		//key version
		inbuf[23] = capp_init[9];
		//cal ID
		inbuf[24] = capp_init[10];
		//app sn
		memcpy(&inbuf[25],&transport_15_data[12],8);
		//logicid
		memcpy(&inbuf[33],transport_15_data,4);
		//fixed value
		memcpy(&inbuf[37],"\xFF\xFF\xFF\xFF",4);
	
		memcpy(bgTaskKeyInbuf,&inbuf[5],36);
		set_task_key(TASK_KEY_GET_JTBKEYA);
		memcpy(mac1,&bgTaskKeyOutbuf[4],4);
#ifdef DEBUG_TRANSPORT	
		printf("bgTaskKeyOutbuf :");
		for(i=0;i<8;i++)
		{
			printf(" %02x",bgTaskKeyOutbuf[i]);
		}
		printf("\n");
#endif
		if( memcmp(&transport_15_data[0], SH_CODE_ORGANIZATION, 4) == 0)
		{//本地卡
			//更新本地卡0x07记录文件
			trans_gen_card_record(0x11,inbuf);
			//交易类型
			inbuf[0] = 0x12;
			//交易金额：填0
			inbuf[4] = 0x00;
			inbuf[5] = 0x00;
	#ifdef DEBUG_TRANSPORT
			printf("\n0x07 LockCard--write inbuf :");
			for(i=0;i<16;i++)
			{
				printf("%02x ",inbuf[i]);
			}
			printf("\n");
	#endif
			//ret = write_record(0x07,0x00,16,inbuf,1);
			ret = write_record(0x07,0x01,16,inbuf,0);
			printf("\nEx-write_record 0x07 ret =%d\n",ret);
			if(ret)
			{
				gResponseStatus=CE_WRITEERROR;
				gGetCardInfoStep=0xff;
				return OP_ERR;
			}
		}

		//更改公共交通过程信息循环记录文件：0x1E
		//交易类型
		transport_1E_data[0]=0x12;
		//终端编号
		memset(&transport_1E_data[1],0x00,4);
		memcpy(&transport_1E_data[5],gPosNo,4);
		//行业代码
		transport_1E_data[9]=0x01;
		//线路
		transport_1E_data[10]=0x00;
		transport_1E_data[11]=gPosNo[0];
		//站点
		transport_1E_data[12]=0x00;
		transport_1E_data[13]=gPosNo[1];
		//运营代码
		memset(&transport_1E_data[14],0x00,2);//
		//RFU
		transport_1E_data[16]=0x00;
		//交易金额：填0
		//LongToByte(gCardThisTransValue,&transport_1E_data[17]);
		memset(&transport_1E_data[17],0x00,4);
		//交易后余额
		LongToByte(gCardThisRemain,&transport_1E_data[21]);
		//交易日期时间
		memcpy(&transport_1E_data[25],gTransTime,7);
		//受理方城市代码
		memcpy(&transport_1E_data[32],SH_CODE_CITY,2);
		//受理方机构标识
		memcpy(&transport_1E_data[34],HANDLE_CODE_ORGANIZATION,8);
		//RFU
		memset(&transport_1E_data[42],0x00,6);
	
		ret=write_capp_record(0x1E,0x00,TRANSPORT_1E_LEN,transport_1E_data,0);
		if(ret)
		{
			printf("write_capp_record...error!");
			gResponseStatus=CE_WRITEERROR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		
		//debit for capp purchase
		ret = debit_for_capp_purchase(bgTaskKeyOutbuf,outbuf);
#ifdef DEBUG_TRANSPORT
		printf("debit...ret 1:%d\n",ret);
#endif
		if (ret!=0) 
		{
/*			memcpy(buf,"\x80\x5a\x00\x09\x02",5);
			buf_to_uword(&capp_init[4],cnt);
			cnt++;
			uword_to_buf(cnt,&buf[5]);
			buf[7] = 0x08;
			ret = mifpro_icmd0(8,buf,out);
			printf("ret =%d\n",ret);
			if(ret<0)
			{
				goto label_invalid;
			}
			if((out[ret-2]!=0x90)||(out[ret-1]!=0x00)||(ret!=10))
			{
				return OP_ERR;
			}*/
			goto label_writeerr;
		}
		memcpy(gTAC,outbuf,4);
		memcpy(mac2,&outbuf[4],4);
	}
	ret=Transport_GetFiles15(outbuf);

	if(ret<0)
	{
		return OP_ERR;
	}
	//取随机数
	memset(buf, 0x00, 60);
  memcpy(buf, "\x00\x84\x00\x00\x04", 5);
  ret = mifpro_icmd0(5,buf,cpubuf);
#ifdef DEBUG_TRANSPORT
  printf("\nLockCard get ICC ret=%d\n",ret);
  printf("cpubuf :");
  for(i=0;i<ret;i++)
  {
  	printf("%02x",cpubuf[i]);
  }
  printf("\n");
#endif

  if((cpubuf[ret - 2] != 0x90) || (cpubuf[ret - 1] != 0x00))
  {
    return OP_ERR;
  }
  //
  memset(cpurandom, 0x00, 8);
  memcpy(cpurandom, cpubuf, 4);
  //
  memset(factor, 0x00, 16);
	memcpy(factor, &transport_15_data[12], 8);
  memcpy(&factor[8], transport_15_data, 8);
  //
  memset(buf,0x00,60);
  memcpy(buf, cpurandom, 8);
  //application lock 
  memcpy(&buf[8], "\x84\x1e\x00\x00\x04", 5);
  buf[5 + 8] = 0x80;
	ret = cpu_cal_protect_mac(bgSHJTBpsamIndex,"\x45\x02",factor,buf,5+3+8,mac);
#ifdef DEBUG_TRANSPORT
  printf("\nLockCard MAC ret=%d\n",ret);
  printf("Mac :%02x%02x%02x%02x",mac[0],mac[1],mac[2],mac[3]);
#endif
	if(ret!=0)
  {
    return OP_ERR;
  }
  memcpy(&buf[5 + 8], mac, 4);
//Lock Card
	memset(inbuf,0,sizeof(inbuf));
	memcpy(inbuf,"\x84\x1E\x00\x00\x04",5);
	memcpy(&inbuf[5],mac,4);
	ret = mifpro_icmd0(9,inbuf,outbuf);
#ifdef DEBUG_TRANSPORT
  printf("\nLockCard ret=%d\n",ret);
  printf("inbuf :");
  for(i=0;i<9;i++)
  {
  	printf("%02x",inbuf[i]);
  }
  printf("\noutbuf :");
  for(i=0;i<ret;i++)
  {
  	printf("%02x",outbuf[i]);
  }
  printf("\n");
#endif
	if((outbuf[ret - 2] != 0x90) || (outbuf[ret - 1] != 0x00))
	{
		goto label_readerr;
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
label_readerr:
	gResponseStatus=CE_READERROR;
	gGetCardInfoStep=0xff;
	return OP_ERR;

}

/************************************************************************************************
函  数：init_for_capp_purchase
功  能：复合消费初始化
参  数:
			  key_index: 密钥索引
			  transvalue: 交易金额
			  psam_id: PSAM卡号
			  outlen：应答数据长度
			  outbuf: 应答数据
返回值：成功:0 失败:非0
************************************************************************************************/
unsigned char init_for_capp_purchase(UBYTE key_index, UDWORD transvalue, UBYTE *psam_id, UBYTE *outbuf)
{
	int i,ret;
	UBYTE inbuf[256];
	UBYTE inbytes;
	UBYTE buf[16];
	
	//initialize for capp
	memcpy(inbuf,"\x80\x50\x03\x02\x0B",5);
	//key
	inbuf[5] = key_index;
	udword_to_buf(transvalue,&inbuf[6]);
	memcpy(&inbuf[10], psam_id, 6);
	inbuf[16] = 0xf;
#ifdef DEBUG_TRANSPORT
	printf("\ninit for capp purchase inbuf :");
	for(i=0;i<17;i++)
	{
		printf(" %02x",inbuf[i]);	
	}
	printf("\n");
#endif
	//ret = mifpro_icmd(buf, 16, cpubuf, &cpulen);
	ret=mifpro_icmd0(17,inbuf,outbuf);
#ifdef DEBUG_TRANSPORT
	printf("\ninit_for ret=%d\n",ret);
#endif
	if(ret < 0)
	{
		return CE_INVALIDCARD;
	}
#ifdef DEBUG_TRANSPORT
	printf("init for capp purchase ret = %d\n",ret);
	printf("init for capp purchase outbuf :");
	for(i=0;i<ret;i++)
	{
		printf(" %02x",outbuf[i]);	
	}
	printf("\n"); 
#endif

	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
#ifdef DEBUG_TRANSPORT
		printf("cpu initial capp failure\n");
#endif
		return CE_INVALIDCARD;
	}
	
	if(ret != 17)
	{
		return CE_INVALIDCARD;
	}
	memset(capp_init,0x00,sizeof(capp_init));
	memcpy(capp_init,outbuf,15);
#ifdef DEBUG_TRANSPORT
	printf("old_balance :%02x %02x %02x %02x\n",capp_init[0],capp_init[1],capp_init[2],capp_init[3]);
	printf("Offline transaction No. :%02x %02x \n",capp_init[4],capp_init[5]);
	printf("Overdraft Limits :%02x %02x %02x\n",capp_init[6],capp_init[7],capp_init[8]);
	printf("key version: %02x\n",capp_init[9]);
	printf("cal ID :%02x\n",capp_init[10]);
	printf("ICC :%02x %02x %02x %02x\n",capp_init[11],capp_init[12],capp_init[13],capp_init[14]);
	printf("\n"); 
#endif 
	memset(buf,0,4);
	memcpy(&buf[1],&capp_init[6],3);
	buf_to_udword(buf,gPurseOffset);					//透支额
	buf_to_udword(capp_init,gCardPurse);			//钱包值
	buf_to_udword(capp_init,gCardLastRemain);	//交易前余额：等价于钱包值
	gCardLastRemain = gCardPurse-gPurseOffset;//钱包
	//gCardThisRemain = gCardPurse;//余额
	gCardThisRemain = gCardPurse-gPurseOffset;//交易后余额
#ifdef DEBUG_TRANSPORT	
	printf("gPurseOffset :%08x\n",gPurseOffset);
	printf("gCardPurse :%08x\n",gCardPurse);
	printf("gCardLastRemain :%08x\n",gCardLastRemain);
#endif 
	return 0;
}

/************************************************************************************************
函  数：debit_for_capp_purchase
功  能：复合应用消费
参  数:
			  sam_sn: 终端脱机交易序号+MAC1(8070指令返回)
			  outlen: 应答数据长度
			  outbuf: 应答数据
返回值：成功:0 失败:非0
************************************************************************************************/
unsigned char debit_for_capp_purchase(UBYTE *sam_sn, UBYTE *outbuf)
{
	int i,ret;
	UBYTE inbuf[256];
	UBYTE inbytes;
	UBYTE buf[16];
	
	memset(inbuf,0x00,sizeof(inbuf));
	memcpy(inbuf,"\x80\x54\x01\x00\x0F",5);
	memcpy(&inbuf[5],sam_sn,4);//终端脱机交易序号
	memcpy(&inbuf[9],gTransTime,7);
	memcpy(&inbuf[16],&sam_sn[4],4);//MAC1
	
#ifdef DEBUG_TRANSPORT
	printf("\ndebit for capp purchase inbuf :");	
	for(i=0;i<20;i++)
	{
		printf(" %02x",inbuf[i]);
	}
	printf("\n");
#endif
	memset(outbuf,0,sizeof(outbuf));
	ret = mifpro_icmd0(20,inbuf,outbuf);
	
#ifdef DEBUG_TRANSPORT
	printf("debit for capp purchase ret = %d\n",ret);
#endif
	if(ret < 0)
	{
		return -1;
	}	
#ifdef DEBUG_TRANSPORT
	printf("outbuf :");	
	for(i=0;i<ret;i++)
	{
		printf(" %02x",outbuf[i]);
	}
	printf("\n");
#endif
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00))
	{
		if((outbuf[ret-2]==0x93)&&(outbuf[ret-1]==0x02))
		{	
			gResponseStatus=CE_MAC2ERR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		goto label_writeerr;
	}
	
	if(ret!=10)
	{
		goto label_invalid;
	}
	
	//memcpy(gTAC,outbuf,4);
	//memcpy(mac2,&outbuf[4],4);
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

/************************************************************************************************
函  数：credit_sam_for_purchase
功  能：SAM卡验证MAC2
参  数:
			  sam_index: PSAM卡编号
			  mac2: MAC2
			  outbuf: 应答数据
返回值：成功:0 失败:非0
************************************************************************************************/
unsigned char credit_sam_for_purchase(UBYTE sam_index, UBYTE *mac2, UBYTE *outbuf)
{
	int i,ret;
	UBYTE inbuf[256];
	UBYTE inbytes;
	UBYTE out_buf[256];
	UBYTE outbytes;
	UBYTE buf[16];
	
	memset(inbuf,0x00,sizeof(inbuf));
	memcpy(inbuf,"\x80\x72\x00\x00\x04",5);
	memcpy(&inbuf[5],mac2,4);
	inbytes = 9;
	
#ifdef DEBUG_TRANSPORT
	printf("\ncredit sam for purchase inbuf :");
	for(i=0;i<9;i++)
	{
		printf(" %02x",inbuf[i]);
	}
	printf("\n");
#endif

	ret=sam_apdu(sam_index,inbuf,inbytes,out_buf,&outbytes,0,0);

#ifdef DEBUG_TRANSPORT
	printf("credit sam for purchase ret = %d\n",ret);
	printf("out_buf :");	
	for(i=0;i<outbytes;i++)
	{
		printf(" %02x",out_buf[i]);
	}
	printf("\n");
#endif
	memcpy(outbuf,out_buf,outbytes);
	if(ret!=0)
	{
		goto label_samerr;
	}
	
	if((out_buf[outbytes-2]!=0x90)||(out_buf[outbytes-1]!=0x00))
	{
		//返回MAC2错误时，备份UID
		memcpy(temp_uid,gThisCardSnr,4);
		gResponseStatus=CE_MAC2ERR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
	//验算MAC2通过时，将备份的UID清零
	memset(&temp_uid[0],0,sizeof(temp_uid));
	return 0;
label_samerr:
		gResponseStatus=CE_PSAMERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
}

/************************************************************************************************
函  数：cpu_cal_protect_mac
功  能：线路保护MAC
参  数:
			  sam_index: PSAM卡编号
			  key: 密钥
			  factor: 分散因子（应用序列号后8字节+发卡机构代码）
			  in_data:
			  in_len： 
			  mac: 返回数据
返回值：成功:0 失败:非0
************************************************************************************************/
unsigned char cpu_cal_protect_mac(unsigned char sam_index,unsigned char *key, unsigned char *factor, unsigned char *in_data, unsigned char in_len, unsigned char *mac)
{
	unsigned char buf[100], sambuf[100], samlen;
	char tmp[4], i ;
	unsigned char cpurandom[10], desdata[8];

	//init des
	memset(buf, 0x00, 100);
	memcpy(buf, "\x80\x1a\x45\x01\x10", 5);
	//key
	memcpy(&buf[2], key, 2);
	//factor
	memcpy(&buf[5], factor, 16);

#ifdef DEBUG_TRANSPORT
	printf("init des:");
	for(i = 0; i < 21; i++) printf("%02x ", buf[i]);
	printf("\n");
#endif

	if(sam_apdu(sam_index, buf, 5 + 16, sambuf, &samlen, 0, 0) != 0)
		return -5;
		
#ifdef DEBUG_TRANSPORT
	printf("init des %02x %02x\n", sambuf[0], sambuf[1]);
#endif

	if((samlen == 2) && (sambuf[0] == 0x61))
	{
		memcpy(buf, "\x00\xc0\x00\x00", 4);
		buf[4] = sambuf[1];
		if(sam_apdu(sam_index, buf, 5, sambuf, &samlen, 0, 2) != 0)
			return -6;
		if((sambuf[samlen - 2] != 0x90) || (sambuf[samlen - 1] != 0))
			return -7;
	}
	if((sambuf[samlen - 2] != 0x90) || (sambuf[samlen - 1] != 0))
		return -10;
	//des-calculate mac mode & have initial value first 8 bytes
	memset(buf, 0x00, 60);
	memcpy(buf, "\x80\xfa\x05\x00", 4);
	buf[4] = in_len;// - 8;
	memcpy(&buf[5], in_data, in_len);
	
#ifdef DEBUG_TRANSPORT
	printf("des data:");
	for(i = 0; i < 5 + in_len; i++) printf("%02x ", buf[i]);
	printf("\n");
#endif

	if(sam_apdu(sam_index, buf, 5 + in_len, sambuf, &samlen, 0, 6) != 0)
		return -8;
		
#ifdef DEBUG_TRANSPORT
	printf("des return %02x %02x \n", sambuf[0], sambuf[1]);
#endif

	if((samlen == 2) && (sambuf[0] == 0x61))
	{
		memcpy(buf, "\x00\xc0\x00\x00", 4);
		buf[4] = sambuf[1];
		if(sam_apdu(sam_index, buf, 5, sambuf, &samlen, 0, 2) != 0)
			return -6;
		if((sambuf[samlen - 2] != 0x90) || (sambuf[samlen - 1] != 0))
			return -7;
	}
#ifdef DEBUG_TRANSPORT
	printf("3des:");
	for(i = 0; i < samlen; i++) printf("%02x ", sambuf[i]);
	printf("\n");
#endif
	if((sambuf[samlen - 2] != 0x90) || (sambuf[samlen - 1] != 0))
		return -11;
	
	memcpy(mac, sambuf, samlen - 2);

	return 0;
}
#endif