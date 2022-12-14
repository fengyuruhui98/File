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
	printf("???볤??:%d\n", SSMlen);	
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
	//E.2	????ǩ??	64	?Թ???????D.1??D.2??D.3??D.4????ҵ??չ??E.1ʹ????ҵ˽ԿSM2ǩ??????ǩ??ֵ
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

	//????4026?????й?Կ
	rtn = get_cert_pb(0x01, 0x70, st_qr_ssm.pb_index, &cert);
	if( rtn )
	{
		goto label_err_key;
	//memcpy(&cert.pb[0], "\x1b\xcf\xfa\x23\x8c\xee\x06\x81\x93\x36\xeb\xd5\x6f\x05\x96\x0e\x9a\xa2\xe0\xa8\xc2\xe4\x7d\xdd\x12\x24\xf4\x6a\x79\xe9\xbf\x44", 32);
	//memcpy(&cert.pb[32], "\xea\x3d\x08\xcf\x02\x71\xb7\xb4\x02\x88\x5b\xab\xd0\xb9\x78\xe6\x85\xba\x93\x1c\xd7\x48\x7b\x67\xca\x6f\x0b\x57\x2d\x78\xf5\x50", 32);
	}
	rtn = SM2_Operation(iUnionPay_id, iUnionPay_id_len, 0, &cert.pb[0], &cert.pb[32],
				  &st_qr_ssm.pb_uuid[0], m - 64, &st_qr_ssm.sign[0], &st_qr_ssm.sign[32]);        //- 24 - 2
	if( rtn )
	{
		goto label_err_key;
	}
	

	//????4026?????й?Կ
	rtn = get_cert_pb(0x00, 0x70, st_qr_ssm.jt_index, &cert);
	if( rtn )
	{
		
		goto label_err_key;
	}
	memcpy(&signData[0], st_qr_ssm.pb_uuid, 32);
	memcpy(&signData[32], st_qr_ssm.pb_type, 16);
	memcpy(&signData[48], st_qr_ssm.jt_sn, 32);
	rtn = SM2_Operation(iUnionPay_id, iUnionPay_id_len, 0, &cert.pb[0], &cert.pb[32],
				  &signData[0], 80, &st_qr_ssm.jt_sign[0], &st_qr_ssm.jt_sign[32]);
	if( rtn )
	{
		goto label_err_key;
	}
	//??ҵ?????ֶ?
	if( (st_qr_ssm.jt_flag[0] & 0x80) == 0x00 )
	{
		
		printf("\n2022.0818 jt_flag\n");
		goto label_err_key;
	}
	//uword_to_buf_reverse(st_qr_ssm.jt_maxCosume, (char *)&st_qr_ssm.jt_maxCosume);

	rtn = get_qr_map(st_qr_ssm.jt_sn, st_qr_ssm.jt_sn, 1, 0x80, &map_type);
	if( rtn )
	{
		
		printf("\n2022.0818 qr map\n");
		goto label_err_key;
	}

	buf[0] = MQR_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = CE_OK;
	buf[3] = 0x01;
	//
	buf[4] = 0x01;
	//???汾
	buf[5] = 0x70;
	//?û???????
	memcpy(&buf[6], st_qr_ssm.pb_special, 2);
	if( (st_qr_ssm.pb_special[0] & 0x40) == 0 )
		goto label_err_key;
	//?û??˻???
	memcpy(&buf[8], &st_qr_ssm.jt_sn[0], 10);
	//??ҵ?????ֶ?
	memcpy(&buf[18], &st_qr_ssm.jt_flag[0], 2);
	if( (st_qr_ssm.jt_flag[0] & 0x80) == 0x00 )
		goto label_err_key;
	//?û?????
	buf[20] = st_qr_ssm.jt_type;
	//????ʹ?ý???
	memcpy(&buf[21], st_qr_ssm.jt_maxCosume, 3);
	//??ά????Чʱ??
	memcpy(&buf[24], &st_qr_ssm.jt_valid_time, 2);
	//??ά??????ʱ??
	memcpy(&buf[26], &st_qr_ssm.jt_generate_time, 4);
	//
	memcpy(&signData[0], st_qr_ssm.pb_valid_date, 14);
	signData[14] = 0x00;
	str2nib(signData, &signData[20]);
	validTime = timestr2long(&signData[21]) + TIME2000;
	ShortToByte(st_qr_ssm.jt_valid_time, (char *)&st_qr_ssm.jt_valid_time);
	LongToByte(st_qr_ssm.jt_generate_time, (char *)&st_qr_ssm.jt_generate_time);
	//unsigned int eighthour = 0x7080;                                                       //UTC0??????ʱ??-zdd-2021.11.16
	//st_qr_ssm.jt_generate_time = st_qr_ssm.jt_generate_time + eighthour;
#ifdef DEBUG_QR
	printf("changed valid_time %04x changed generate_time %08x changed valid_date %08x(%02x%02x-%02x-%02x %02x:%02x:%02x)\n",
		st_qr_ssm.jt_valid_time, st_qr_ssm.jt_generate_time, validTime, signData[20],signData[21],signData[22],signData[23],signData[24],signData[25],signData[26] );
#endif
  unsigned int eighthour = 0x7080;                                                       //UTC0??????ʱ??-zdd-2021.11.16
	eighthour = st_qr_ssm.jt_generate_time + eighthour;
	if( eighthour > validTime )                   //st_qr_ssm.jt_generate_time
		goto label_err_key;
	//ǩ????Կ????
	buf[30] = st_qr_ssm.jt_index;
	//??????
	memset(&buf[31], 0x00, 9);
	//??ҵ??????
	buf[40] = map_type;
	
	for(i = 20; i < (len-22); i++)
		printf("%02x", qr_ssm_source[i]);
	printf("\n");
	//ԭ??????
	if( len - 22 + 40 < 255)
	{
		memcpy(&buf[41], &qr_ssm_source[20], len - 22);
		buf[3] = 0x81;
		shmt_prot_send(buf, len - 22 + 40);
	}else
	{
		memcpy(&buf[41], &qr_ssm_source[20], 213);       //215ȫ????Ϊ213
		shmt_prot_send(buf, 254);                           //zdd1128

		buf[3] = 0x82;
		memcpy(&buf[4], &qr_ssm_source[20 + 213], len - 213 - 22);
		shmt_prot_send(buf, 4 + len - 213 -22);
	}
	gStatus = TRANS_STATUS;

	return 0;

label_err_key:
	return CE_NOFUNCTION;

}

#endif