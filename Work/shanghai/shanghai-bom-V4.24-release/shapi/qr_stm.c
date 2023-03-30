#ifndef QR_STM_C
#define QR_STM_C

struct QR_STM		st_qr_stm;
unsigned char qr_stm_source[1024];


UBYTE getinfo_QR_STM(unsigned char *data, int len)
{
unsigned char buf[500];
unsigned char bleData[100];
int rtn;
long length;
int i;
struct Cert_Content cert;
	
	memcpy(qr_stm_source, data, len);
	memcpy(&st_qr_stm.title[0], data, sizeof(struct QR_STM));
#ifdef DEBUG_QR
	memcpy(buf, st_qr_stm.title, 9);
	buf[9] = 0x00;
	printf("QR(STM)%s: len %04x version %02x\n", buf, st_qr_stm.len, st_qr_stm.version);
	
	printf("index %02x organize %02x%02x%02x%02x RfU %02x%02x rfu %02x%02x%02x al_flag %02x\n", 
		st_qr_stm.index, st_qr_stm.organize[0], st_qr_stm.organize[1], st_qr_stm.organize[2], st_qr_stm.organize[3],
		st_qr_stm.RFU[0], st_qr_stm.RFU[1], st_qr_stm.rfu[0], st_qr_stm.rfu[1], st_qr_stm.rfu[2], st_qr_stm.al_flag);
	
	printf("user_id:");
	for(i = 0; i < 16; i++)
		printf("%02X ", st_qr_stm.user_id[i]);
	printf("\n");
	printf("user_uuid:");
	for(i = 0; i < 10; i++)
		printf("%02X ", st_qr_stm.user_uuid[i]);
	printf("\n");
	printf("type %02x generate_time %08x valid_time %04x\n", st_qr_stm.type, st_qr_stm.generate_time, st_qr_stm.valid_time);
	printf("define_data:");
	for(i = 0; i < 32; i++)
		printf("%02X ", st_qr_stm.define_data[i]);
	printf("\nsign %02x\n", st_qr_stm.sign_format);
	printf("rs:");
	for(i = 0; i < 64; i++)
		printf("%02X ", st_qr_stm.rs[i]);
	printf("\n");
	
#endif
	
	//查找4026参数中公钥
	rtn = get_cert_pb(0x02, st_qr_stm.version, st_qr_stm.index, &cert);
	if( rtn )
	{
		goto lable_bom_cert;
	}
	rtn = SM2_Operation(iUnionPay_id, iUnionPay_id_len, 0, &cert.pb[0], &cert.pb[32], 
				  &qr_stm_source[11], len - 65 - 11, &st_qr_stm.rs[0], &st_qr_stm.rs[32]);

	if( rtn )
	{
		goto label_bom_sm2err;
	}

	buf[0] = MQR_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = CE_OK;
	buf[3] = 0x81;
	
	buf[4] = 0x02;
	//stm码版本
	buf[5] = st_qr_stm.version;          //版本01
	//用户属性域	BYTE	2	其它：0000
	memcpy(&buf[6], "\x00\x00", 2);
	//用户账户号	BYTE	10	申通码账号：如3104770011223344556F
	memcpy(&buf[8], st_qr_stm.user_uuid, 10);
	//行业启用字段	BYTE	2	其它：FFFF
	memcpy(&buf[18], "\xFF\xFF", 2);
	//用户类型	BYTE	1	4020映射前的卡类型
	buf[20] = st_qr_stm.type;
	//最大使用金额	BYTE	3	其它：0000
	memcpy(&buf[21], "\x00\x00\x00", 3);
	//二维码有效时间	BYTE	2	二维码有效时间，与二维码生成时间一起控制二维码有效时间。以秒为单位。
	memcpy(&buf[24], &st_qr_stm.valid_time, 2);
	//二维码生成时间	BYTE	4	二维码生成的时间戳，使用UTC（0时区）时间1970年1月1日00:00:00到现在的秒数。
	memcpy(&buf[26], &st_qr_stm.generate_time, 4);
	//签名公钥索引	BYTE	1	用于查找公钥的索引
	buf[30] = st_qr_stm.index; 
	//保留域	BYTE	9	暂时全00
	memset(&buf[31], 0x00, 9);
	//卡业务类型	BYTE	1	4020映射后的卡类型
	buf[40] = st_qr_stm.type;
	//原码数据	BYTE		二进制码值
	memcpy(&buf[41], qr_stm_source, len);
	
	shmt_prot_send(buf, 41 + len);
	gStatus = TRANS_STATUS;
	
	return 0;
	
label_err_key:
	gResponseStatus = CE_READERROR;
	return CE_READERROR;
	
lable_bom_cert:
	buf[0] = MQR_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = 0x00;
	buf[3] = 0x81;
	//二维码介质
	buf[4] = 0x00;
	//码版本
	buf[5] = 0x00;
	//用户属性域
	memset(&buf[6], 0x00, 2);
	//用户账户号
	memset(&buf[8], 0x00, 10);
	//行业启用字段
	memset(&buf[18], 0x00, 2);
	//用户类型
	buf[20] = 0x00;
	//最大使用金额
	memset(&buf[21], 0x00, 3);
	//二维码有效时间
	memset(&buf[24], 0x00, 2);
	//二维码生成时间
	memset(&buf[26], 0x00, 4);
	//签名公钥索引
	buf[30] = 0x00;
	//保留域
	buf[31] = CE_BOM_CERT;
	memset(&buf[32], 0x00, 8);
	//卡业务类型
	buf[40] = 0x00;
	
	shmt_prot_send(buf, 41);
	//gResponseStatus = CE_BOM_SM2ERR;
	//return CE_BOM_SM2ERR;
	return 0;
	
lable_bom_map:
	buf[0] = MQR_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = 0x00;
	buf[3] = 0x81;
	//二维码介质
	buf[4] = 0x00;
	//码版本
	buf[5] = 0x00;
	//用户属性域
	memset(&buf[6], 0x00, 2);
	//用户账户号
	memset(&buf[8], 0x00, 10);
	//行业启用字段
	memset(&buf[18], 0x00, 2);
	//用户类型
	buf[20] = 0x00;
	//最大使用金额
	memset(&buf[21], 0x00, 3);
	//二维码有效时间
	memset(&buf[24], 0x00, 2);
	//二维码生成时间
	memset(&buf[26], 0x00, 4);
	//签名公钥索引
	buf[30] = 0x00;
	//保留域
	buf[31] = CE_BOM_MAP;
	memset(&buf[32], 0x00, 8);
	//卡业务类型
	buf[40] = 0x00;
	
	shmt_prot_send(buf, 41);
	//gResponseStatus = CE_BOM_SM2ERR;
	//return CE_BOM_SM2ERR;
	return 0;
	
label_bom_sm2err:             //20220222
	buf[0] = MQR_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = 0x00;
	buf[3] = 0x81;
	//二维码介质
	buf[4] = 0x00;
	//码版本
	buf[5] = 0x00;
	//用户属性域
	memset(&buf[6], 0x00, 2);
	//用户账户号
	memset(&buf[8], 0x00, 10);
	//行业启用字段
	memset(&buf[18], 0x00, 2);
	//用户类型
	buf[20] = 0x00;
	//最大使用金额
	memset(&buf[21], 0x00, 3);
	//二维码有效时间
	memset(&buf[24], 0x00, 2);
	//二维码生成时间
	memset(&buf[26], 0x00, 4);
	//签名公钥索引
	buf[30] = 0x00;
	//保留域
	buf[31] = CE_BOM_SM2ERR;
	memset(&buf[32], 0x00, 8);
	//卡业务类型
	buf[40] = 0x00;
	
	shmt_prot_send(buf, 41);
	//gResponseStatus = CE_BOM_SM2ERR;
	//return CE_BOM_SM2ERR;
	return 0;
	
label_bom_train_nouse:
	buf[0] = MQR_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = 0x00;
	buf[3] = 0x81;
	//二维码介质
	buf[4] = 0x00;
	//码版本
	buf[5] = 0x00;
	//用户属性域
	memset(&buf[6], 0x00, 2);
	//用户账户号
	memset(&buf[8], 0x00, 10);
	//行业启用字段
	memset(&buf[18], 0x00, 2);
	//用户类型
	buf[20] = 0x00;
	//最大使用金额
	memset(&buf[21], 0x00, 3);
	//二维码有效时间
	memset(&buf[24], 0x00, 2);
	//二维码生成时间
	memset(&buf[26], 0x00, 4);
	//签名公钥索引
	buf[30] = 0x00;
	//保留域
	buf[31] = CE_BOM_TRAIN_NOT_USE;
	memset(&buf[32], 0x00, 8);
	//卡业务类型
	buf[40] = 0x00;
	
	shmt_prot_send(buf, 41);
	//gResponseStatus = CE_BOM_TRAIN_NOT_USE;
	//return CE_BOM_TRAIN_NOT_USE;	
	return 0;	
	
label_bom_chuxing_nouse: 
		buf[0] = MQR_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = 0x00;
	buf[3] = 0x81;
	//二维码介质
	buf[4] = 0x00;
	//码版本
	buf[5] = 0x00;
	//用户属性域
	memset(&buf[6], 0x00, 2);
	//用户账户号
	memset(&buf[8], 0x00, 10);
	//行业启用字段
	memset(&buf[18], 0x00, 2);
	//用户类型
	buf[20] = 0x00;
	//最大使用金额
	memset(&buf[21], 0x00, 3);
	//二维码有效时间
	memset(&buf[24], 0x00, 2);
	//二维码生成时间
	memset(&buf[26], 0x00, 4);
	//签名公钥索引
	buf[30] = 0x00;
	//保留域
	buf[31] = CE_BOM_CHUXING_NOT_USE;
	memset(&buf[32], 0x00, 8);
	//卡业务类型
	buf[40] = 0x00;
	
	shmt_prot_send(buf, 41);
	//gResponseStatus = CE_BOM_CHUXING_NOT_USE;
	//return CE_BOM_CHUXING_NOT_USE;
	return 0;
	
label_bom_qrtimeerr:	
		buf[0] = MQR_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = 0x00;
	buf[3] = 0x81;
	//二维码介质
	buf[4] = 0x00;
	//码版本
	buf[5] = 0x00;
	//用户属性域
	memset(&buf[6], 0x00, 2);
	//用户账户号
	memset(&buf[8], 0x00, 10);
	//行业启用字段
	memset(&buf[18], 0x00, 2);
	//用户类型
	buf[20] = 0x00;
	//最大使用金额
	memset(&buf[21], 0x00, 3);
	//二维码有效时间
	memset(&buf[24], 0x00, 2);
	//二维码生成时间
	memset(&buf[26], 0x00, 4);
	//签名公钥索引
	buf[30] = 0x00;
	//保留域
	buf[31] = CE_BOM_QR_TIME_ERR;
	memset(&buf[32], 0x00, 8);
	//卡业务类型
	buf[40] = 0x00;
	
	shmt_prot_send(buf, 41);
	//gResponseStatus = CE_BOM_QR_TIME_ERR;
	//return CE_BOM_QR_TIME_ERR;
	return 0;
	
label_bom_qrver:
		buf[0] = MQR_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = 0x00;
	buf[3] = 0x81;
	//二维码介质
	buf[4] = 0x00;
	//码版本
	buf[5] = 0x00;
	//用户属性域
	memset(&buf[6], 0x00, 2);
	//用户账户号
	memset(&buf[8], 0x00, 10);
	//行业启用字段
	memset(&buf[18], 0x00, 2);
	//用户类型
	buf[20] = 0x00;
	//最大使用金额
	memset(&buf[21], 0x00, 3);
	//二维码有效时间
	memset(&buf[24], 0x00, 2);
	//二维码生成时间
	memset(&buf[26], 0x00, 4);
	//签名公钥索引
	buf[30] = 0x00;
	//保留域
	buf[31] = CE_BOM_QR_VER_ERR;
	memset(&buf[32], 0x00, 8);
	//卡业务类型
	buf[40] = 0x00;
	
	shmt_prot_send(buf, 41);
	//gResponseStatus = CE_BOM_QR_VER_ERR;
	//return CE_BOM_QR_VER_ERR;
	return 0;
}

//使用住建部SAM卡的TAC计算密钥。
//a.	发送通用DES计算初始化指令，指令格式如下：
//801A440110 112233445566778F(用户帐户号后8字节) 02002900FF000000（固定发卡机构代码）
//b.	发送通用DES计算指令，指令格式如下:
//80 FA 01 00 20 + DATA
/*
UBYTE	qr_stm_tac(UBYTE *in_buf, UBYTE *out_buf)
{
UBYTE	inbuf[255], outbuf[255], KeyBuf[100],buf1[100];
UBYTE	inbytes, obytes;
int 	ret, i;
	
	if(gPSAMFileFlag != 0x00){
		bgSmpsamIndex = SAMLOCATION_4;
	memcpy(KeyBuf,"\x00\xa4\x00\x00\x02\x10\x03",7);
	i = 7;
	ret = sam_apdu(bgSmpsamIndex,KeyBuf,i,buf1,&obytes,0,0);
	if(ret != 0)
	{ 
		#ifdef _DEBUG_GETCARDINFO_
		debug_printf("\x0d\x0a Err:getcardinfo,select 1003");
		#endif
		bgSmpsamIndex = SAMLOCATION_4;
		sam_select(bgSmpsamIndex);
		sam_set(bgSmpsamIndex,SAM_ETU_93,16);
		ret = sam_atr(bgSmpsamIndex,buf1,&obytes);
		if(ret != 0)
		{
			goto label_err;
		}
		memcpy(KeyBuf,"\x00\xa4\x00\x00\x02\x10\x03",7);
		i = 7;
		ret = sam_apdu(bgSmpsamIndex,KeyBuf,i,buf1,&obytes,0,0);
		if(ret != 0)
		{
			goto label_err;
		}
	}	
	//if((buf1[0] != 0x61) &&((UBYTE)buf1[0] != 0x90))  //2013/11/17 22:01:12
	if((buf1[obytes-2] != 0x61) &&((UBYTE)buf1[obytes-2] != 0x90))
	{
  	#ifdef _DEBUG_GETCARDINFO_
		debug_printf("\x0d\x0a Err:select 1003 resp");
		#endif
    goto label_err;
	}
	gPSAMFileFlag = 0x00;
	}
	
	memcpy(inbuf,"\x80\x1a\x44\x01\x10",5);
	memcpy(&inbuf[5], &in_buf[27], 8);
	memcpy(&inbuf[13],"\x02\x00\x29\x00\xff\x00\x00\x00",8);
	inbytes=21;
	ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);

#ifdef	DEBUG_QR
	printf("SH MQR TAC init send:");
	for(i = 0; i < inbytes; i++)
		printf("%02x", inbuf[i]);
	printf("\n");
	printf(" return :");
	for(i = 0; i < obytes; i++)
		printf("%02x", outbuf[i]);
	printf("\n");
#endif
  	if(ret != 0) 
  	{
  		gResponseStatus = CE_PSAMERROR;
  		goto label_err;
  	}
  		
  	if( (outbuf[obytes-2]!=0x90)||(outbuf[obytes-1]!=0x00)) 
  	{
   		goto label_err;
	}
	
  	memcpy(inbuf,"\x80\xfa\x01\x00\x30",5);
  	//
	//交易类型标志	H	1
  	inbuf[5] = 0x88;
	//终端设备流水号	H	4
  	memcpy(&inbuf[6], &in_buf[12], 4);
	//终端设备节点	H	4
	memcpy(&inbuf[10], &in_buf[16], 4);
	//终端设备交易时间	H	4
	memcpy(&inbuf[14], &in_buf[20], 4);
	//服务器返回流水号（脱机填0）	H	4
	memcpy(&inbuf[18], &in_buf[0], 4);
	//服务器返回时间（脱机填0）	H	4
	memcpy(&inbuf[22], &in_buf[4], 4);
	//服务器返回金额（脱机填0）	H	4
	memcpy(&inbuf[26], &in_buf[8], 4);
	//二维码介质	H	1
	//二维码账户号	H	10
	//二维码有效时间	H	2
	//二维码生成时间	H	4
	memcpy(&inbuf[30], &in_buf[24], 17);
	//SAM卡号	D	4
	memcpy(&inbuf[47], bpgSmpsamNo, 4);
	//填充字符	H	2
	memcpy(&inbuf[51], "\x80\x00", 2);
  	inbytes = 53;
	//delay_ms(6);
  	ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
#ifdef	DEBUG_QR
	printf("SH PTC TAC cal send:");
	for(i = 0; i < inbytes; i++)
		printf("%02x", inbuf[i]);
	printf("\n");
	printf(" return :");
	for(i = 0; i < obytes; i++)
		printf("%02x", outbuf[i]);
	printf("\n");
#endif
  	if(ret != 0) 
  	{
    	goto label_err;
	}
  	if((outbuf[obytes-2]!=0x61)&&(outbuf[obytes-2]!=0x90))
  	{
    	goto label_err;
	}
  	if(obytes == (4+2)) 
  	{
	  	gDebugStep=0x100b;
		memcpy(bgTaskKeyOutbuf,outbuf,4);
		goto label_ok;
	}
  	if((outbuf[obytes-2]==0x61)&&(outbuf[obytes-1]==4)) 
  	{
		memcpy(inbuf,"\x00\xc0\x00\x00\x04",5);
	  	inbytes = 5;
		//delay_ms(6);
	  	ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
	  	if((ret != 0) || (obytes != 6)) goto label_err;
	}
label_ok:
	// 
  	memcpy(out_buf,outbuf,4);
  	return 0;
	//
label_err:
	gResponseStatus = CE_PSAMERROR;
	return OP_ERR;
}
*/
//使用地铁单程票SAM卡的TAC计算密钥。
//a.	发送通用DES计算初始化指令，指令格式如下：
//801A240110 112233445566778F(用户帐户号后8字节)
//b.	发送通用DES计算指令，指令格式如下:
//80 FA 01 00 30 + DATA

UBYTE	qr_stm_tac(UBYTE *in_buf, UBYTE *out_buf)
{
UBYTE	inbuf[255], outbuf[255], KeyBuf[100],buf1[100];
UBYTE	inbytes, obytes;
int 	ret, i;
	
	if(memcmp(bgSmpsamADF, "\x10\x01", 2) != 0x00)
	{
		memcpy(KeyBuf,"\x00\xa4\x00\x00\x02\x10\x01",7);
		i = 7;
		ret = sam_apdu(bgSmpsamIndex,KeyBuf,i,buf1,&obytes,0,0);
		if(ret != 0)
		{ 
			#ifdef _DEBUG_GETCARDINFO_
			debug_printf("\x0d\x0a Err:getcardinfo,select 1003");
			#endif
			sam_select(bgSmpsamIndex);
			sam_set(bgSmpsamIndex,SAM_ETU_93,16);
			ret = sam_atr(bgSmpsamIndex,buf1,&obytes);
			if(ret != 0)
			{
				//printf("\nzdderr1\n");
				goto label_err;
			}
			memcpy(KeyBuf,"\x00\xa4\x00\x00\x02\x10\x01",7);
			i = 7;
			ret = sam_apdu(bgSmpsamIndex,KeyBuf,i,buf1,&obytes,0,0);
			if(ret != 0)
			{
				//printf("\nzdderr2\n");
				goto label_err;
			}
		}	
		//if((buf1[0] != 0x61) &&((UBYTE)buf1[0] != 0x90))  //2013/11/17 22:01:12
		if((buf1[obytes-2] != 0x61) &&((UBYTE)buf1[obytes-2] != 0x90))
		{
	  	#ifdef _DEBUG_GETCARDINFO_
			debug_printf("\x0d\x0a Err:select 1001 resp");
			#endif
			//printf("\nzdderr3\n");
	    goto label_err;
		}
	}
	
	memcpy(inbuf,"\x80\x1a\x24\x01\x08",5);
	memcpy(&inbuf[5], &in_buf[27], 8);
	inbytes=13;
	ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);

#ifdef	DEBUG_QR
	printf("SH MQR TAC init send:");
	for(i = 0; i < inbytes; i++)
		printf("%02x", inbuf[i]);
	printf("\n");
	printf(" return :");
	for(i = 0; i < obytes; i++)
		printf("%02x", outbuf[i]);
	printf("\n");
#endif
  	if(ret != 0) 
  	{
  		gResponseStatus = CE_PSAMERROR;
  		//printf("\nzdderr4\n");
  		goto label_err;
  	}
  		
  	if( (outbuf[obytes-2]!=0x90)||(outbuf[obytes-1]!=0x00)) 
  	{
  		//printf("\nzdderr8\n");
   		goto label_err;
	}
	
  	memcpy(inbuf,"\x80\xfa\x01\x00\x30",5);
  	//
  	memcpy(&inbuf[5], &in_buf[0], 41);
	//SAM卡号	D	4
	memcpy(&inbuf[46], bpgSmpsamNo, 4);
	//填充字符	H	2
	memcpy(&inbuf[50], "\x80\x00\x00", 3);
  	inbytes = 53;
	//delay_ms(6);
  	ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
#ifdef	DEBUG_QR
	printf("SH STM TAC cal send:");
	for(i = 0; i < inbytes; i++)
		printf("%02x", inbuf[i]);
	printf("\n");
	printf(" return :");
	for(i = 0; i < obytes; i++)
		printf("%02x", outbuf[i]);
	printf("\n");
#endif
  	if(ret != 0) 
  	{
  		//printf("\nzdderr5\n");
    	goto label_err;
	}
  	if((outbuf[obytes-2]!=0x61)&&(outbuf[obytes-2]!=0x90))
  	{
  		//printf("\nzdderr6\n");
    	goto label_err;
	}
  	if(obytes == (4+2)) 
  	{
	  	gDebugStep=0x100b;
		memcpy(bgTaskKeyOutbuf,outbuf,4);
		goto label_ok;
	}
  	if((outbuf[obytes-2]==0x61)&&(outbuf[obytes-1]==4)) 
  	{
		memcpy(inbuf,"\x00\xc0\x00\x00\x04",5);
	  	inbytes = 5;
		//delay_ms(6);
	  	ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
	  	if((ret != 0) || (obytes != 6)){ 
	  		//printf("\nzdderr7\n");
	  		goto label_err;
	  	}
	}
label_ok:
	// 
  	memcpy(out_buf,outbuf,4);
  	return 0;
	//
label_err:
	gResponseStatus = CE_PSAMERROR;
	return OP_ERR;
}

#endif