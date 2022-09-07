#ifndef QR_SSM_C
#define QR_SSM_C
#define DEBUG_QR

struct QR_SSM	st_qr_ssm;

unsigned char qr_ssm_source[1024];
unsigned char qr_ssm_desource[1024];


UBYTE getinfo_QR_SSM(unsigned char *data, int len)
{
unsigned char buf[500];
unsigned char signData[500];
int rtn,base64UrlRtn, SSMlen;
unsigned int validTime;
long length;
int i;
struct Cert_Content cert;
unsigned char map_type;

	memset(qr_ssm_desource, 0x00, 500);
	memcpy(qr_ssm_source, data, len);
	memcpy(qr_ssm_desource, data, len);
	data[len - 2] = 0;
	//rtn = base64urlsafe_decode(&data[24]);
	//printf("SSM source(%d) :%s\n", len, data);
	rtn = ifbase64(&data[24], &qr_ssm_desource[24]);
	if( rtn == 1)
	{
		printf("zddtesttttttttcccc\n");
		goto label_err_key;
	}
	SSMlen = rtn = base64url_decode(&data[24], &qr_ssm_desource[24]);
	printf("解码长度:%d\n", SSMlen);	
	printf("zdd-decode-base64urlsafe-SSM source(%d) :%s\n", len, qr_ssm_source);
#ifdef DEBUG_QR
	printf("SSM source(%d) :%s\n", len, qr_ssm_source);
	printf("base64(%d):", rtn);
	for(i = 24; i < rtn + 24; i++)
		printf("%02x", qr_ssm_desource[i]);
	printf("\n");
#endif
	memcpy(&st_qr_ssm.title[0], &qr_ssm_desource[0], rtn + 24);
#ifdef DEBUG_QR
	printf("QR(SSM): len %08x\ntitle ", st_qr_ssm.len);
	
	memset(buf, 0x00, 21);
	memcpy(buf, st_qr_ssm.title, 20);
	printf("%s\nsign ", buf);
	for(i = 0; i < 64; i++)
		printf("%02x", st_qr_ssm.sign[i]);
	printf("\n Pb uuid ");
	for(i = 0; i < 32; i++)
		printf("%02x", st_qr_ssm.pb_uuid[i]);
	printf("\n special %02x%02x type %02x%02x \n", st_qr_ssm.pb_special[0], st_qr_ssm.pb_special[1], st_qr_ssm.pb_type[0], st_qr_ssm.pb_type[1]);

	memset(buf, 0x00, 15);
	memcpy(buf, st_qr_ssm.pb_valid_date, 14);
	printf("valid date %s  index %02x\n jt user ", buf, st_qr_ssm.pb_index);

	for(i = 0; i < 10; i++)
		printf("%02x", st_qr_ssm.jt_sn[i]);
	printf("\nflag %02x%02x type %02x maxcosume %02x%02x%02x valid time %04x generate time %08x index %02x\n",
		st_qr_ssm.jt_flag[0], st_qr_ssm.jt_flag[1], st_qr_ssm.jt_type, st_qr_ssm.jt_maxCosume[0], st_qr_ssm.jt_maxCosume[1], st_qr_ssm.jt_maxCosume[2], st_qr_ssm.jt_valid_time, st_qr_ssm.jt_generate_time, st_qr_ssm.jt_index);
	//jt_rfu[8];
	printf("sign ");
	//E.2	数字签名	64	对公共区域D.1、D.2、D.3、D.4及行业扩展域E.1使用行业私钥SM2签名后的签名值
	for(i = 0; i < 64; i++)
		printf("%02x", st_qr_ssm.jt_sign[i]);
	printf("\n");
#endif


	//char *p = (char *)&rtn;
	//char a = p[0];
	//char b = p[1];
	//char c = p[2];
	//char d = p[3];
	unsigned int temp = st_qr_ssm.len;
	char *q = (char *)&temp;
	char e = q[3];
	char f = q[2];
	char g = q[1];
	char h = q[0];
	int m = 0;
	m += (e - '0');
	m += 10*(f-'0');
	m += 100*(g-'0');
	m += 1000*(h-'0');
	printf("\nmmmm:%d\n", m);
	printf("\nSSmm:%d\n", SSMlen);


	//printf("\na:%c\n\nb:%c\n\nc:%c\n\nd:%c\n\ne:%c\n\nf:%c\n\ng:%c\n\nh:%c\n",a,b,c,d,e,f,g,h);
	//if(SSMlen != m){
		//printf("SSMlen source(%d) :\n", rtn);
		//goto label_err_key;
	//}

	//查找4026参数中公钥
	rtn = get_cert_pb(0x01, 0x70, st_qr_ssm.pb_index, &cert);
	if( rtn )
	{
		goto lable_bom_cert;
	//memcpy(&cert.pb[0], "\x1b\xcf\xfa\x23\x8c\xee\x06\x81\x93\x36\xeb\xd5\x6f\x05\x96\x0e\x9a\xa2\xe0\xa8\xc2\xe4\x7d\xdd\x12\x24\xf4\x6a\x79\xe9\xbf\x44", 32);
	//memcpy(&cert.pb[32], "\xea\x3d\x08\xcf\x02\x71\xb7\xb4\x02\x88\x5b\xab\xd0\xb9\x78\xe6\x85\xba\x93\x1c\xd7\x48\x7b\x67\xca\x6f\x0b\x57\x2d\x78\xf5\x50", 32);
	}
	rtn = SM2_Operation(iUnionPay_id, iUnionPay_id_len, 0, &cert.pb[0], &cert.pb[32],
				  &st_qr_ssm.pb_uuid[0], m - 64, &st_qr_ssm.sign[0], &st_qr_ssm.sign[32]);        //- 24 - 2
	if( rtn )
	{
		goto label_bom_sm2err;      //20220222
	}

	//查找4026参数中公钥
	rtn = get_cert_pb(0x00, 0x70, st_qr_ssm.jt_index, &cert);
	if( rtn )
	{
		goto lable_bom_cert;
	}
	memcpy(&signData[0], st_qr_ssm.pb_uuid, 32);
	memcpy(&signData[32], st_qr_ssm.pb_type, 16);
	memcpy(&signData[48], st_qr_ssm.jt_sn, 32);
	rtn = SM2_Operation(iUnionPay_id, iUnionPay_id_len, 0, &cert.pb[0], &cert.pb[32],
				  &signData[0], 80, &st_qr_ssm.jt_sign[0], &st_qr_ssm.jt_sign[32]);
	if( rtn )
	{
		goto label_bom_sm2err;    //20220222
	}
	
	if( (st_qr_ssm.pb_special[0] & 0x40) == 0x00)  //20220222
		goto label_bom_chuxing_nouse;
	//行业启用字段
	if( (st_qr_ssm.jt_flag[0] & 0x80) == 0x00 )
	{
		goto label_bom_train_nouse;   //20220222
	}
	//uword_to_buf_reverse(st_qr_ssm.jt_maxCosume, (char *)&st_qr_ssm.jt_maxCosume);

	rtn = get_qr_map(st_qr_ssm.jt_sn, st_qr_ssm.jt_sn, 1, 0x80, &map_type);
	if( rtn )
	{
		goto lable_bom_map;
	}

	buf[0] = MQR_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = CE_OK;
	buf[3] = 0x01;
	//
	buf[4] = 0x01;
	//码版本
	buf[5] = 0x70;
	//用户属性域
	memcpy(&buf[6], st_qr_ssm.pb_special, 2);
	//if( (st_qr_ssm.pb_special[0] & 0x40) == 0 )
		//goto label_err_key;
	//用户账户号
	memcpy(&buf[8], &st_qr_ssm.jt_sn[0], 10);
	//行业启用字段
	memcpy(&buf[18], &st_qr_ssm.jt_flag[0], 2);
	//if( (st_qr_ssm.jt_flag[0] & 0x80) == 0x00 )
		//goto label_err_key;
	//用户类型
	buf[20] = st_qr_ssm.jt_type;
	//最大使用金额
	memcpy(&buf[21], st_qr_ssm.jt_maxCosume, 3);
	//二维码有效时间
	memcpy(&buf[24], &st_qr_ssm.jt_valid_time, 2);
	//二维码生成时间
	memcpy(&buf[26], &st_qr_ssm.jt_generate_time, 4);
	//
	memcpy(&signData[0], st_qr_ssm.pb_valid_date, 14);
	signData[14] = 0x00;
	str2nib(signData, &signData[20]);
	validTime = timestr2long(&signData[21]) + TIME2000;
	ShortToByte(st_qr_ssm.jt_valid_time, (char *)&st_qr_ssm.jt_valid_time);
	LongToByte(st_qr_ssm.jt_generate_time, (char *)&st_qr_ssm.jt_generate_time);
	//unsigned int eighthour = 0x7080;                                                       //UTC0到北京时间-zdd-2021.11.16
	//st_qr_ssm.jt_generate_time = st_qr_ssm.jt_generate_time + eighthour;
#ifdef DEBUG_QR
	printf("changed valid_time %04x changed generate_time %08x changed valid_date %08x(%02x%02x-%02x-%02x %02x:%02x:%02x)\n",
		st_qr_ssm.jt_valid_time, st_qr_ssm.jt_generate_time, validTime, signData[20],signData[21],signData[22],signData[23],signData[24],signData[25],signData[26] );
#endif
  unsigned int eighthour = 0x7080;                                                       //UTC0到北京时间-zdd-2021.11.16
	eighthour = st_qr_ssm.jt_generate_time + eighthour;
	if( eighthour > validTime )                   //st_qr_ssm.jt_generate_time
		goto label_bom_qrtimeerr;          //20220222
	//签名公钥索引
	buf[30] = st_qr_ssm.jt_index;
	//保留域
	memset(&buf[31], 0x00, 9);
	//卡业务类型
	buf[40] = map_type;
	
	for(i = 20; i < (len-22); i++)
		printf("%02x", qr_ssm_source[i]);
	printf("\n");
	//原码数据
	if( len - 22 + 40 < 255)
	{
		memcpy(&buf[41], &qr_ssm_source[20], len - 22);
		buf[3] = 0x81;
		shmt_prot_send(buf, len - 22 + 40);
	}else
	{
		memcpy(&buf[41], &qr_ssm_source[20], 213);       //215全部改为213
		shmt_prot_send(buf, 254);                           //zdd1128

		buf[3] = 0x82;
		memcpy(&buf[4], &qr_ssm_source[20 + 213], len - 213 - 22);
		shmt_prot_send(buf, 4 + len - 213 -22);
	}
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

#endif