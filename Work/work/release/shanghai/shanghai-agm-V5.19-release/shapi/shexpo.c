//世博会项目
 
#ifndef _SHEXPO_C_
#define _SHEXPO_C_

#define _TESTCARD_

//#define _DEBUG_EXPO_



UBYTE Rand[10];
UBYTE IssueSn[5];
UBYTE AuthCode[10];
UBYTE IssueDep[2];
UBYTE IssueGroup[6];
UBYTE IssueDev;
UBYTE IssueStatus;
UBYTE AppointTime[4];
UBYTE UseCount[4];						//低字节在前
UBYTE BusCount[4];						//低字节在前
UBYTE SurfaceSn[4];
UBYTE ShExpoCardSn[8];
UBYTE gKeyACal[EM681_MAX_SECTOR][6];
UBYTE gKeyBCal[EM681_MAX_SECTOR][6];
const UBYTE Bkid2[6][2]={{0x02,1},{0x04,2},{0x06,3},{0x08,2},{0x0a,4},{0x0c,4}};
const UBYTE Akid2[5][2]={{0x03,2},{0x05,3},{0x07,2},{0x09,4},{0x0b,4}};		
const UBYTE KEYdes[8]={0x87,0x0b,0x88,0x2c,0x52,0xde,0xa0,0x40};
const UBYTE AccessBit[16][4]={{0x69,0x67,0x89,0x69},{0x78,0x77,0x88,0x69},
															{0x78,0x77,0x88,0x69},{0x18,0x77,0x8e,0x69},
															{0x7f,0x07,0x88,0x69},{0x7f,0x07,0x88,0x69},
															{0x18,0x77,0x8e,0x69},{0x7f,0x07,0x88,0x69},
															{0x7f,0x07,0x88,0x69},{0x7f,0x07,0x88,0x69},
															{0x7f,0x07,0x88,0x69},{0x7f,0x07,0x88,0x69},
															{0x7f,0x07,0x88,0x69},{0x7f,0x07,0x88,0x69},
															{0x7f,0x07,0x88,0x69},{0x7f,0x07,0x88,0x69}
															};
UBYTE sector;
UBYTE B13[20],B14[20],B17[20],B18[20],B25[20],B26[20];
/*******************************************
函数：shexpo_cardissue
功能：世博会门票发行
*******************************************/
UBYTE shexpo_cardissue(void)
{
	UBYTE counter,buf[40];
	UBYTE ret,i,n;
	counter=0;
	if(shexpo_SNAuth()!=0)
	{
		return OP_ERR;
	}
#ifdef _TESTCARD_
	for(i=0;i<EM681_MAX_SECTOR;i++)
	{
		memcpy(gKeyA[i],"\x53\x61\x90\x1a\x4e\xa4",6);
	}
	memcpy(gKeyB,gKeyA,EM681_MAX_SECTOR*6);
#endif
	
#ifndef _TESTCARD_
	memcpy(gKeyA[0],"\xb2\xdf\xbc\x4d\xbb\x5d",6);
	memcpy(gKeyA[1],"\x64\x73\xe5\x71\x86\xd8",6);
	memcpy(gKeyA[2],"\x64\x73\xe5\x71\x86\xd8",6);
	memcpy(gKeyA[5],"\x64\x73\xe5\x71\x86\xd8",6);
	memcpy(gKeyA[7],"\x64\x73\xe5\x71\x86\xd8",6);
	memcpy(gKeyA[8],"\x64\x73\xe5\x71\x86\xd8",6);
	memcpy(gKeyA[9],"\x64\x73\xe5\x71\x86\xd8",6);
	memcpy(gKeyA[10],"\x64\x73\xe5\x71\x86\xd8",6);
	memcpy(gKeyA[11],"\x64\x73\xe5\x71\x86\xd8",6);
	memcpy(gKeyA[12],"\x64\x73\xe5\x71\x86\xd8",6);
	memcpy(gKeyA[13],"\x64\x73\xe5\x71\x86\xd8",6);
	memcpy(gKeyA[14],"\x64\x73\xe5\x71\x86\xd8",6);
	memcpy(gKeyA[15],"\x64\x73\xe5\x71\x86\xd8",6);
	memcpy(gKeyA[3],"\xf4\xf0\xee\x4c\x98\x4a",6);
	memcpy(gKeyA[4],"\x83\x48\xdb\xcd\xe5\x7e",6);
	memcpy(gKeyA[6],"\x19\x31\x07\x6b\xae\x39",6);
	memcpy(gKeyB,gKeyA,EM681_MAX_SECTOR*6);
#endif

	memset(SurfaceSn,0,4);

	//memcpy(KeyBuf,Rand,8);
	memcpy(&KeyBuf[8],gThisCardSnr,4);
	memcpy(&KeyBuf[12],IssueSn,4);
	SEGetAuthCodeStep1(KeyBuf);
	memset(buf,0,16);
	buf[0]=gCardType;
	memcpy(&buf[1],AppointTime,4);
	lrc3_gen(buf,15);
	if(mcmh_write(12,buf,0,0)!=0)
	{
		goto label_write_err;
	}
	memcpy(buf,UseCount,4);
	buf[4]=~buf[0];
	buf[5]=~buf[1];
	buf[6]=~buf[2];
	buf[7]=~buf[3];
	memcpy(&buf[8],UseCount,4);
	buf[12]=0xff;
	buf[13]=0;
	buf[14]=0xff;
	buf[15]=0;
	if(mcmh_write(13,buf,2,0)!=0)
	{
		goto label_write_err;
	}
	if(mcmh_write(14,buf,2,0)!=0)
	{
		goto label_write_err;
	}
	memcpy(buf,BusCount,4);
	buf[4]=~buf[0];
	buf[5]=~buf[1];
	buf[6]=~buf[2];
	buf[7]=~buf[3];
	memcpy(&buf[8],BusCount,4);
	buf[12]=0;
	buf[13]=0xff;
	buf[14]=0;
	buf[15]=0xff;
	if(mcmh_write(25,buf,0,0)!=0)
	{
		goto label_write_err;
	}
	if(mcmh_write(26,buf,2,0)!=0)
	{
		goto label_write_err;
	}
	memset(buf,0,16);
	if(mcmh_write(17,buf,0,0)!=0)
	{
		goto label_write_err;
	}
	if(mcmh_write(18,buf,2,0)!=0)
	{
		goto label_write_err;
	}
	if(COMMONGetSAMStep2()!=0)
	{
		gResponseStatus=ACK_EXPO_PSAMERR;
		return OP_ERR;
	}
	memcpy(AuthCode,bgTaskKeyOutbuf,8);
	memcpy(Rand,&bgTaskKeyOutbuf[8],8);
	//计算KEY
	memcpy(KeyBuf,"\x80\xfc\x02\x02\x15",5);
	memcpy(&KeyBuf[5],gThisCardSnr,4);
	memcpy(&KeyBuf[9],IssueSn,4);
	memcpy(&KeyBuf[13],&AuthCode[4],4);
	KeyBuf[17]=0;
	memcpy(&KeyBuf[18],Rand,8);
	SEGetKeyStep1(KeyBuf,1,ONLYKEYB);
	memcpy(buf,IssueDep,2);
	memcpy(&buf[2],IssueSn,4);
	memcpy(&buf[6],AuthCode,8);
	buf[14]=cardversion;
	lrc3_gen(buf,15);
	if(mcmh_write(1,buf,0,0)!=0)
	{
		goto label_write_err;
	}
	memcpy(buf,IssueGroup,5);
	buf[5]=IssueDev;
	buf[6]=IssueStatus;
	memcpy(&buf[7],Rand,8);
	lrc3_gen(buf,15);
	if(mcmh_write(2,buf,2,0)!=0)
	{
		goto label_write_err;
	}
//	memcpy(gKeyACal[0],gKeyA[0],6);
	memcpy(gKeyACal[0],gThisCardSnr,4);
	memcpy(&gKeyACal[0][4],gThisCardSnr,2);
	sector=0;
	for(i=0;i<5;i++)
	{
		ret=sector;
		if(COMMONGetSAMStep2()!=0)
		{
			gResponseStatus=ACK_EXPO_PSAMERR;
			return OP_ERR;
		}
		if(sector==0)
		{
			memcpy(gKeyBCal[0],bgTaskKeyOutbuf,6);
			sector++;
		}
		else
		{
			for(n=0;n<Bkid2[i][1];n++)
			{
				memcpy(gKeyACal[ret+n],&bgTaskKeyOutbuf[n*6],6);
			}
			for(n=0;n<Bkid2[i][1];n++)
			{
				memcpy(gKeyBCal[ret+n],&bgTaskKeyOutbuf[Bkid2[i][1]*6+n*6],6);
			}
			sector+=Bkid2[i][1];
		}
		memcpy(KeyBuf,"\x80\xfc\x01",3);
		KeyBuf[3]=Akid2[i][0];
		KeyBuf[4]=Akid2[i][1]+20;
		memcpy(&KeyBuf[5],gThisCardSnr,4);
		memcpy(&KeyBuf[9],IssueSn,4);
		memcpy(&KeyBuf[13],AuthCode,4);
		for(n=0;n<Akid2[i][1];n++)
		{
			KeyBuf[17+n]=sector+n;
		}
		memcpy(&KeyBuf[17+n],Rand,8);
		memcpy(&KeyBuf[25+n],&AuthCode[4],4);
		SEGetKeyStep1(KeyBuf,n,KEYAANDB);
		for(n=0;n<Bkid2[i][1];n++)
		{
			memcpy(buf,gKeyACal[ret+n],6);
			memcpy(&buf[6],AccessBit[ret+n],4);
			memcpy(&buf[10],gKeyBCal[ret+n],6);
			if(mcmh_writewithout((ret+n)*4+3,buf,0,0)!=0)
			{
				goto label_write_err;
			}
			if(mcmh_authkeyblock((ret+n)*4,0)!=0)
			{
				gResponseStatus=ACK_EXPO_AUTHERR;
				return OP_ERR;
			}
			if(mcmh_authkeyblock((ret+n)*4,1)!=0)
			{
				gResponseStatus=ACK_EXPO_AUTHERR;
				return OP_ERR;
			}
		}
	}
	ret=sector;
	if(COMMONGetSAMStep2()!=0)
	{
		gResponseStatus=ACK_EXPO_PSAMERR;
		return OP_ERR;
	}
	for(n=0;n<Bkid2[5][1];n++)
	{
		memcpy(gKeyACal[ret+n],&bgTaskKeyOutbuf[n*6],6);
	}
	for(n=0;n<Bkid2[5][1];n++)
	{
		memcpy(gKeyBCal[ret+n],&bgTaskKeyOutbuf[Bkid2[i][1]*6+n*6],6);
	}
	sector+=Bkid2[5][1];
	for(n=0;n<Bkid2[5][1];n++)
	{
			memcpy(buf,gKeyACal[ret+n],6);
	//		memcpy(buf,gKeyA[ret+n],6);
			memcpy(&buf[6],AccessBit[ret+n],4);
			memcpy(&buf[10],gKeyBCal[ret+n],6);
			if(mcmh_writewithout((ret+n)*4+3,buf,0,0)!=0)
			{
				goto label_write_err;
			}
			if(mcmh_authkeyblock((ret+n)*4,0)!=0)
			{
				gResponseStatus=ACK_EXPO_AUTHERR;
				return OP_ERR;
			}
			if(mcmh_authkeyblock((ret+n)*4,1)!=0)
			{
				gResponseStatus=ACK_EXPO_AUTHERR;
				return OP_ERR;
			}
	}
	
	

	memcpy(gKeyA[1],gKeyACal[1],(EM681_MAX_SECTOR-1)*6);
	memcpy(gKeyB,gKeyBCal,EM681_MAX_SECTOR*6);
	return 0;
	
	label_write_err:
	gResponseStatus=ACK_EXPO_WRCARDERR;
	return OP_ERR;
}
/*******************************************
函数：shexpo_cardinfo
功能：世博会门票信息
*******************************************/
UBYTE shexpo_cardinfo(UBYTE *outbuf)
{
	UBYTE buf[40],ret;
	if(shexpo_SNAuth()!=0)
	{
		return OP_ERR;
	}
//	#ifdef _TESTCARD_
//			memcpy(gKeyA[0],"\x53\x61\x90\x1a\x4e\xa4",6);
//	#endif
//	
//	#ifndef _TESTCARD_
//			memcpy(gKeyA[0],"\xb2\xdf\xbc\x4d\xbb\x5d",6);
//	#endif
	memcpy(gKeyA[0],gThisCardSnr,4);
	memcpy(&gKeyA[0][4],gThisCardSnr,2);
	//读发行区
	memcpy(outbuf,ShExpoCardSn,8);
	if(mcmh_read(1,buf,0,0)!=0)
	{
		gResponseStatus=ACK_EXPO_RDCARDERR;
		return OP_ERR;
	}
	if(lrc3_chk(buf,15)!=0)
	{
		gResponseStatus=ACK_EXPO_INVALIDCARD;
		return OP_ERR;
	}
	memcpy(IssueSn,&buf[2],4);
	memcpy(AuthCode,&buf[6],8);
	memcpy(&outbuf[8],buf,6);
	outbuf[14]=buf[14];
	if(mcmh_read(2,buf,2,0)!=0)
	{
		gResponseStatus=ACK_EXPO_RDCARDERR;
		return OP_ERR;
	}
	if(lrc3_chk(buf,15)!=0)
	{
		gResponseStatus=ACK_EXPO_INVALIDCARD;
		return OP_ERR;
	}
	memcpy(&outbuf[15],buf,7);
	memcpy(Rand,&buf[7],8);
	
	//计算1~6KeyA
//	sector=1;
//	for(i=0;i<3;i++)
//	{
//		if(shexpo_calkey(Akid2[i][0],Akid2[i][1],0)!=0)
//			return OP_ERR;
//	}

	//1,2扇区
	memcpy(KeyBuf,"\x80\xfc\x01",3);
	KeyBuf[3]=0x03;
	KeyBuf[4]=22;
	memcpy(&KeyBuf[5],gThisCardSnr,4);
	memcpy(&KeyBuf[9],IssueSn,4);
	memcpy(&KeyBuf[13],AuthCode,4);	
	KeyBuf[17]=0x01;
	KeyBuf[18]=0x02;
	memcpy(&KeyBuf[19],Rand,8);
	SEGetKeyStep1(KeyBuf,2,ONLYKEYA);
	ret=COMMONGetSAMStep2();
	if(ret!=0)
	{
		gResponseStatus=ACK_EXPO_PSAMERR;
		return OP_ERR;
	}
	memcpy(gKeyA[1],bgTaskKeyOutbuf,12);
	//3,4,5扇区	
	memcpy(KeyBuf,"\x80\xfc\x01",3);
	KeyBuf[3]=0x05;
	KeyBuf[4]=23;
	memcpy(&KeyBuf[5],gThisCardSnr,4);
	memcpy(&KeyBuf[9],IssueSn,4);
	memcpy(&KeyBuf[13],AuthCode,4);	
	KeyBuf[17]=0x03;
	KeyBuf[18]=0x04;
	KeyBuf[19]=0x05;
	memcpy(&KeyBuf[20],Rand,8);
	SEGetKeyStep1(KeyBuf,3,ONLYKEYA);
	
//	return 0;
	
	//读销售区
	if(mcmh_read(4,buf,0,0)!=0)
	{
		gResponseStatus=ACK_EXPO_RDCARDERR;
		return OP_ERR;
	}
	if(lrc3_chk(buf,15)!=0)
	{
		memset(buf,0,16);
	}
	memcpy(&outbuf[22],buf,15);
	if(mcmh_read(5,buf,2,0)!=0)
	{
		gResponseStatus=ACK_EXPO_RDCARDERR;
		return OP_ERR;
	}
	if(lrc3_chk(buf,15)!=0)
	{
		memset(buf,0,16);
	}
	memcpy(&outbuf[37],buf,9);
	
	//读团体信息
	if(mcmh_read(8,buf,0,0)!=0)
	{
		gResponseStatus=ACK_EXPO_RDCARDERR;
		return OP_ERR;
	}
	if(lrc3_chk(buf,15)!=0)
	{
		memset(buf,0,16);
	}
	memcpy(&outbuf[46],buf,9);
	if(mcmh_read(9,buf,2,0)!=0)
	{
		gResponseStatus=ACK_EXPO_RDCARDERR;
		return OP_ERR;
	}
	if(lrc3_chk(buf,15)!=0)
	{
		memset(buf,0,16);
	}
	memcpy(&outbuf[55],buf,15);
	if(mcmh_read(10,buf,2,0)!=0)
	{
		gResponseStatus=ACK_EXPO_RDCARDERR;
		return OP_ERR;
	}
	if(lrc3_chk(buf,15)!=0)
	{
		memset(buf,0,16);
	}
	memcpy(&outbuf[70],buf,15);
	
	
	if(COMMONGetSAMStep2()!=0)
	{
		gResponseStatus=ACK_EXPO_PSAMERR;
		return OP_ERR;
	}
	//6扇区
	memcpy(gKeyA[3],bgTaskKeyOutbuf,18);
	memcpy(KeyBuf,"\x80\xfc\x01",3);
	KeyBuf[3]=0x07;
	KeyBuf[4]=21;
	memcpy(&KeyBuf[5],gThisCardSnr,4);
	memcpy(&KeyBuf[9],IssueSn,4);
	memcpy(&KeyBuf[13],AuthCode,4);	
	KeyBuf[17]=0x06;
	memcpy(&KeyBuf[18],Rand,8);
	SEGetKeyStep1(KeyBuf,1,ONLYKEYA);
	
	//使用信息
	if(mcmh_read(12,buf,0,0)!=0)
	{
		gResponseStatus=ACK_EXPO_RDCARDERR;
		return OP_ERR;
	}
	if(lrc3_chk(buf,15)!=0)
	{
		memset(buf,0,16);
	}
	memcpy(&outbuf[85],buf,5);
	
	
	if(mcmh_read(16,buf,0,0)!=0)
	{
		gResponseStatus=ACK_EXPO_RDCARDERR;
		return OP_ERR;
	}
	if(lrc3_chk(buf,15)!=0)
	{
		memset(buf,0,16);
	}
	outbuf[94]=buf[0];
	
	
	//预约信息
	if(mcmh_read(20,buf,0,0)!=0)
	{
		gResponseStatus=ACK_EXPO_RDCARDERR;
		return OP_ERR;
	}
	if(lrc3_chk(buf,15)!=0)
	{
		memset(buf,0,16);
	}
	memcpy(&outbuf[107],buf,11);
	if(COMMONGetSAMStep2()!=0)
	{
		gResponseStatus=ACK_EXPO_PSAMERR;
		return OP_ERR;
	}
	memcpy(gKeyA[6],bgTaskKeyOutbuf,6);
	//卡上信息恢复
	if(card_restore()!=0)
	{
		return OP_ERR;
	}
	memcpy(&outbuf[90],B13,4);
	memcpy(&outbuf[95],B17,12);
	//公交信息
	memcpy(&outbuf[118],B25,4);
	return 0;
}
/*******************************************
函数：shexpo_cardverify
功能：世博会门票密钥认证及数据认证
*******************************************/
UBYTE shexpo_cardverify(void)
{
	UBYTE buf[40],i;
	
	if(shexpo_SNAuth()!=0)
	{
		return OP_ERR;
	}
//	#ifdef _TESTCARD_
//			memcpy(gKeyA[0],"\x53\x61\x90\x1a\x4e\xa4",6);
//	#endif
//	
//	#ifndef _TESTCARD_
//			memcpy(gKeyA[0],"\xb2\xdf\xbc\x4d\xbb\x5d",6);
//	#endif
	memcpy(gKeyA[0],gThisCardSnr,4);
	memcpy(&gKeyA[0][4],gThisCardSnr,2);
	memcpy(gKeyACal[0],gKeyA[0],6);
	//读发行区
	if(mcmh_read(1,buf,0,0)!=0)
	{
		gResponseStatus=ACK_EXPO_RDCARDERR;
		return OP_ERR;
	}
	if(lrc3_chk(buf,15)!=0)
	{
		gResponseStatus=ACK_EXPO_INVALIDCARD;
		return OP_ERR;
	}
	memcpy(IssueSn,&buf[2],4);
	memcpy(AuthCode,&buf[6],8);
	if(mcmh_read(2,buf,2,0)!=0)
	{
		gResponseStatus=ACK_EXPO_RDCARDERR;
		return OP_ERR;
	}
	if(lrc3_chk(buf,15)!=0)
	{
		gResponseStatus=ACK_EXPO_INVALIDCARD;
		return OP_ERR;
	}
	memcpy(Rand,&buf[7],8);
	//计算密钥
	#ifdef _DEBUG_EXPO_
			debug_printf("\x0d\x0a start cal keya");
	#endif
	sector=1;
	for(i=0;i<5;i++)
	{
		if(shexpo_calkey(Akid2[i][0],Akid2[i][1],0)!=0)
			return OP_ERR;
	}
	#ifdef _DEBUG_EXPO_
			debug_printf("\x0d\x0a start cal keyb");
	#endif
	sector=0;
	for(i=0;i<6;i++)
	{
		if(shexpo_calkey(Bkid2[i][0],Bkid2[i][1],1)!=0)
			return OP_ERR;
	}
	memcpy(gKeyA[1],gKeyACal[1],(EM681_MAX_SECTOR-1)*6);
	memcpy(gKeyB,gKeyBCal,EM681_MAX_SECTOR*6);
	for(i=0;i<16;i++)
	{
		if(mcmh_authkeyblock(i*4,0)!=0)
		{
			gResponseStatus=ACK_EXPO_AUTHERR;
			return OP_ERR;
		}
		if(mcmh_authkeyblock(i*4,1)!=0)
		{
			gResponseStatus=ACK_EXPO_AUTHERR;
			return OP_ERR;
		}
	}
	if(mcmh_read(12,buf,0,0)!=0)
	{
		gResponseStatus=ACK_EXPO_RDCARDERR;
		return OP_ERR;
	}
	if(lrc3_chk(buf,15)!=0)
	{
		gResponseStatus=ACK_EXPO_INVALIDCARD;
		return OP_ERR;
	}
	if(gCardType!=buf[0])
	{
		gResponseStatus=ACK_EXPO_INVALIDCARD;
		return OP_ERR;
	}
	if(memcmp(AppointTime,&buf[1],4)!=0)
	{
		gResponseStatus=ACK_EXPO_INVALIDCARD;
		return OP_ERR;
	}	
	return 0;
}
/*******************************************
函数：shexpo_calkey
功能：世博会门票密钥计算
*******************************************/
UBYTE shexpo_calkey(UBYTE kid2,UBYTE sectnum,UBYTE KEYAB)
{
	UBYTE temp,i;
	UBYTE buf[40];
	UBYTE ret,outbytes,outbuf[40];
	
	watchdog();
	temp=sector;
	buf[0]=0x80;
	buf[1]=0xfc;
	if(KEYAB==0)
	{
			buf[2]=0x01;
	}
	else
	{
			buf[2]=0x02;
	}
	buf[3]=kid2;
	buf[4]=20+sectnum;
	memcpy(&buf[5],gThisCardSnr,4);
	memcpy(&buf[9],IssueSn,4);
	if(KEYAB==0)
	{
		memcpy(&buf[13],AuthCode,4);
	}
	else
	{
		memcpy(&buf[13],&AuthCode[4],4);
	}
	for(i=0;i<sectnum;i++)
	{
		buf[17+i]=temp++;
	}
	
	memcpy(&buf[17+i],Rand,8);
	i=25+sectnum;
	#ifdef _DEBUG_EXPO_
			if(sector==1)
			{
				debug_printf("\x0d\x0a cal =%02x %02x %02x %02x %02x %02x",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
				debug_printf("\x0d\x0a cal =%02x %02x %02x %02x %02x %02x",buf[6],buf[7],buf[8],buf[9],buf[10],buf[11]);
				debug_printf("\x0d\x0a cal =%02x %02x %02x %02x %02x %02x",buf[12],buf[13],buf[14],buf[15],buf[16],buf[17]);
				debug_printf("\x0d\x0a cal =%02x %02x %02x %02x %02x %02x",buf[18],buf[19],buf[20],buf[21],buf[22],buf[23]);
				debug_printf("\x0d\x0a cal =%02x %02x",buf[24],buf[25]);
			}
			debug_printf("\x0d\x0a cal sector=%02x",sector);
	#endif
	ret=sam_apdu(bgExpopsamIndex,buf,i,outbuf,&outbytes,0,0);

	if((ret!=0)||(outbuf[0]!=0x61)||(outbuf[1]!=sectnum*6))
	{
		gResponseStatus=ACK_EXPO_PSAMERR;
		return OP_ERR;
	}
	memcpy(buf,"\x00\xc0\x00\x00",4);
	buf[4]=sectnum*6;
	i=5;
	ret=sam_apdu(bgExpopsamIndex,buf,i,outbuf,&outbytes,0,0);
	if((ret!=0)||(outbytes!=(sectnum*6+2)))
	{

		gResponseStatus=ACK_EXPO_PSAMERR;
		return OP_ERR;
	}
	temp=sectnum; 
	if(KEYAB==0)
	{
		
			for(i=0;i<temp;i++)
			{
				memcpy(gKeyACal[sector++],&outbuf[i*6],6);
			}
	}
	else
	{
			#ifdef _DEBUG_EXPO_
			if(sectnum==1)
			{
				debug_printf("\x0d\x0a get cal =%02x %02x %02x %02x %02x %02x",outbuf[0],outbuf[1],outbuf[2],outbuf[3],outbuf[4],outbuf[5]);
			}
	#endif
			for(i=0;i<temp;i++)
			{
				memcpy(gKeyBCal[sector++],&outbuf[i*6],6);
			}
	}
	return 0;
}
/*******************************************
函数：shexpo_SNAuth
功能：世博会门票认证
*******************************************/
UBYTE shexpo_SNAuth(void)
{
	UBYTE counter,i,buf[10],srcbuf[10],destbuf[20];
	UBYTE ret;
	UBYTE key[8];
	
	memset(srcbuf,0,8);
	counter=0;
	while (counter<20) 
	{
		
	   ret=mcml_request(PICC_REQSTD,destbuf);
	   if (ret == 0) break;
	   counter++;
//	   delay_ms(2);
  }
  if(counter>=20)
  {
	  gResponseStatus=ACK_EXPO_NOCARD;
	  return OP_ERR;
	}
	buf[2]=destbuf[0];
	buf[3]=destbuf[1];
	#ifdef _DEBUG_EXPO_
			debug_printf("\x0d\x0a start anticoll");
	#endif
	
	ret=mcml_anticoll(gThisCardSnr);
	if(ret!=0)
	{
		gResponseStatus=ACK_EXPO_ANTICOLLERR;
		return OP_ERR;
	}
	buf[0]=gThisCardSnr[4];
	#ifdef _DEBUG_EXPO_
			debug_printf("\x0d\x0a start select  card sn=%02x %02x %02x %02x",gThisCardSnr[0],gThisCardSnr[1],gThisCardSnr[2],gThisCardSnr[3]);
	#endif
	gDebugStep=3;
	ret=mcml_select(gThisCardSnr,&i);
	if(ret!=0)
	{
		gResponseStatus=ACK_EXPO_ANTICOLLERR;
		return OP_ERR;
	}
	buf[1]=i;
	srcbuf[4]=gThisCardSnr[3];
	srcbuf[5]=gThisCardSnr[2];
	srcbuf[6]=gThisCardSnr[1];
	srcbuf[7]=gThisCardSnr[0];
	memcpy(key,KEYdes,8);
	des_encode(key,srcbuf,destbuf);
	memcpy(ShExpoCardSn,gThisCardSnr,4);
	memcpy(&ShExpoCardSn[4],buf,4);
	return 0;
}
/*******************************************
函数：card_restore
功能：世博会门票恢复
*******************************************/
UBYTE card_restore(void)
{
	UBYTE buf[20];
	
	watchdog();
	if(mcmh_read(13,buf,0,0)!=0)
	{
		gResponseStatus=ACK_EXPO_RDCARDERR;
		return OP_ERR;
	}
	if(purse_chk(buf)!=0)
	{
		memset(buf,0,16);
	}
	memcpy(B13,buf,16);
//	if(mcmh_read(14,buf,2,0)!=0)
//	{
//		gResponseStatus=ACK_EXPO_RDCARDERR;
//		return OP_ERR;
//	}
//	if(purse_chk(buf)!=0)
//	{
//		memset(buf,0,16);
//	}
	memcpy(B14,buf,16);
	
	if(mcmh_read(17,buf,0,0)!=0)
	{
		gResponseStatus=ACK_EXPO_RDCARDERR;
		return OP_ERR;
	}
	if(lrc3_chk(buf,15)!=0)
	{
		memset(buf,0,16);
	}
	memcpy(B17,buf,16);
//	if(mcmh_read(18,buf,2,0)!=0)
//	{
//		gResponseStatus=ACK_EXPO_RDCARDERR;
//		return OP_ERR;
//	}
//	if(lrc3_chk(buf,15)!=0)
//	{
//		memset(buf,0,16);
//	}
	memcpy(B18,buf,16);
	
	if(mcmh_read(25,buf,0,0)!=0)
	{
		gResponseStatus=ACK_EXPO_RDCARDERR;
		return OP_ERR;
	}
	if(purse_chk(buf)!=0)
	{
		memset(buf,0,16);
	}
	memcpy(B25,buf,16);
//	if(mcmh_read(26,buf,2,0)!=0)
//	{
//		gResponseStatus=ACK_EXPO_RDCARDERR;
//		return OP_ERR;
//	}
//	if(purse_chk(buf)!=0)
//	{
//		memset(buf,0,16);
//	}
	memcpy(B26,buf,16);
	return 0;
}

/***********************************************
函数：resetdefaultkey
功能：恢复初始密钥
***********************************************/
//#ifdef _TESTCARD_
UBYTE resetdefaultkey(void)
{
	UBYTE buf[40],ret,i;
	if(shexpo_SNAuth()!=0)
	{
		return OP_ERR;
	}
	
			memcpy(gKeyA[0],"\x53\x61\x90\x1a\x4e\xa4",6);

	
//	#ifndef _TESTCARD_
//			memcpy(gKeyA[0],"\xb2\xdf\xbc\x4d\xbb\x5d",6);
//	#endif
	
	//读发行区
	if(mcmh_read(1,buf,0,0)!=0)
	{
		gResponseStatus=ACK_EXPO_RDCARDERR;
		return OP_ERR;
	}
	if(lrc3_chk(buf,15)!=0)
	{
		gResponseStatus=ACK_EXPO_INVALIDCARD;
		return OP_ERR;
	}
	memcpy(IssueSn,&buf[2],4);
	memcpy(AuthCode,&buf[6],8);
	if(mcmh_read(2,buf,2,0)!=0)
	{
		gResponseStatus=ACK_EXPO_RDCARDERR;
		return OP_ERR;
	}
	if(lrc3_chk(buf,15)!=0)
	{
		gResponseStatus=ACK_EXPO_INVALIDCARD;
		return OP_ERR;
	}
	memcpy(Rand,&buf[7],8);
	sector=1;
	for(i=0;i<5;i++)
	{
		if(shexpo_calkey(Akid2[i][0],Akid2[i][1],0)!=0)
			return OP_ERR;
	}
	memcpy(gKeyA[1],gKeyACal[1],(EM681_MAX_SECTOR-1)*6);
	for(ret=0;ret<16;ret++)
	{
		memcpy(buf,gKeyA[0],6);
		memcpy(&buf[6],"\xff\x07\x80\x69",4);
		memcpy(&buf[10],gKeyA[0],6);
		if(mcmh_writewithout(ret*4+3,buf,0,0)!=0)
		{
			goto label_write_err;
		}
	}
	return 0;
	label_write_err:
	gResponseStatus=ACK_EXPO_WRCARDERR;
	return OP_ERR;
}
	//	#endif//resetdefaultkey
#endif