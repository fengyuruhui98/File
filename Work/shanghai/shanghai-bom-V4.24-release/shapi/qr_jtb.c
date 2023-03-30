#ifndef QR_JTB_C
#define QR_JTB_C

struct QR_JTB		st_qr_jtb;
struct QR_JTB_EX	st_qr_jtb_ex;
unsigned char qr_jtb_source[1024];

UBYTE getinfo_QR_JTB(unsigned char *data, int len)
{
unsigned char buf[500];
unsigned char user_id[100], user_id_len[2];
int rtn;
long length;
int i, jtb_map;
struct Cert_Content cert;
unsigned char map_type;

	memcpy(qr_jtb_source, data, len);
	memcpy(&st_qr_jtb.version, data, sizeof(struct QR_JTB));
	memcpy(st_qr_jtb_ex.defined_data, &data[sizeof(struct QR_JTB)], st_qr_jtb.defined_len);
	memcpy(&st_qr_jtb_ex.os_sign[0], &data[sizeof(struct QR_JTB) + st_qr_jtb.defined_len], sizeof(struct QR_JTB_EX) - 32);
	
#ifdef DEBUG_PRINT
	printf("\nQR(JTB):version %02x len %04x\n", st_qr_jtb.version, st_qr_jtb.len);
	printf("os_pb_title %02x os_pb_service[4] %02x%02x%02x%02x os_pb_index %02x os_pb_format %02x\n",
			st_qr_jtb.os_pb_title, st_qr_jtb.os_pb_service[0], st_qr_jtb.os_pb_service[1],st_qr_jtb.os_pb_service[2], st_qr_jtb.os_pb_service[3], st_qr_jtb.os_pb_index, st_qr_jtb.os_pb_format);
	printf("os_pb_id[4]	%02x%02x%02x%02x os_pb_date[2] %02x%02x os_pb_sn[3] %02x%02x%02x os_pb_sign_al %02x\n",
			st_qr_jtb.os_pb_id[0], st_qr_jtb.os_pb_id[1], st_qr_jtb.os_pb_id[2], st_qr_jtb.os_pb_id[3], st_qr_jtb.os_pb_date[0], st_qr_jtb.os_pb_date[1], 
			st_qr_jtb.os_pb_sn[0], st_qr_jtb.os_pb_sn[1],st_qr_jtb.os_pb_sn[2],st_qr_jtb.os_pb_sign_al);
	printf("os_pl_enc_al %02x os_pl_para %02x os_pl_len %02x \nos_pl_key:\n",
			st_qr_jtb.os_pb_enc_al, st_qr_jtb.os_pb_len);
	for(i = 0; i < 33; i++)
		printf("%02x", st_qr_jtb.os_pb_key[i]);		
	printf("\nos_pl_sign:");
	for(i = 0; i < 64; i++)
		printf("%02x", st_qr_jtb.os_pb_sign[i]);
	printf("\npay_sn:");
	for(i = 0; i < 16; i++)               
		printf("%02x", st_qr_jtb.pay_sn[i]);
	printf("\n os_userid:");
	for(i = 0; i < 10; i++)
		printf("%02x", st_qr_jtb.os_userid[i]);
	printf("\nos_orgnization:");
	for(i = 0; i < 4; i++)
		printf("%02x", st_qr_jtb.os_orgnization[i]);
	printf("\nos_platid:");
	for(i = 0; i < 4; i++)
		printf("%02x", st_qr_jtb.os_platid[i]);
	printf("\n type %d maxCosume[3] %02x%02x%02x\nuser_pubkey:", 
		st_qr_jtb.type, st_qr_jtb.maxCosume[0], st_qr_jtb.maxCosume[1], st_qr_jtb.maxCosume[2]);		
	for(i = 0; i < 33; i++)
		printf("%02x", st_qr_jtb.user_pubkey[i]);
	printf("\nvalid_date %08x valid_time %04x defined_len %d\ndefine_data ", st_qr_jtb.valid_date, st_qr_jtb.valid_time, st_qr_jtb.defined_len);
	
	for(i = 0; i < st_qr_jtb.defined_len; i++)
		printf("%02x", st_qr_jtb_ex.defined_data[i]);
	printf("\nos_sign:");
	for(i = 0; i < 65; i++)	
		printf("%02x", st_qr_jtb_ex.os_sign[i]);
	printf("\n generate_time %08x\n user_sign:", st_qr_jtb_ex.generate_time);
	for(i = 0; i < 65; i++)
		printf("%02x", st_qr_jtb_ex.user_sign[i]);
#endif
	
	//����4026�����й�Կ
	rtn = get_cert_pb(0x00, st_qr_jtb.version, st_qr_jtb.os_pb_index, &cert);
	if( rtn )
	{
		goto lable_bom_cert;
	}
	//��ǩ֤��
	if( st_qr_jtb.version == 0x81 )
	{
	//���������Կ֤��ǩ����useridȡֵ�����������Կ֤��ķ����ʶ||��CA������5�ֽ�
		memcpy(user_id, cert.service_flag, 4);
		user_id[4] = cert.index;
		user_id[5] = 0;
		user_id_len[0] = 0;
		user_id_len[1] = 40;
	}else
	{
		memcpy(user_id, iUnionPay_id, 16);
		memcpy(user_id_len, iUnionPay_id_len, 2);
	}
	rtn = SM2_Operation(user_id, user_id_len, 0, &cert.pb[0], &cert.pb[32], 
				  &st_qr_jtb.os_pb_format, sizeof(struct os_pb_key) - 64 - 6, &st_qr_jtb.os_pb_sign[0], &st_qr_jtb.os_pb_sign[32]);
	if( rtn )
	{
		goto label_bom_sm2err;             //20220222
	}
	//��֤��������ǩ��
	if( st_qr_jtb.version == 0x81)
	{
		//���������Ȩǩ����useridȡֵ�����������||����ƽ̨��Ź�8�ֽ�
		memcpy(user_id, &st_qr_jtb.os_orgnization[0], 4);
		memcpy(&user_id[4], &st_qr_jtb.os_platid[0], 4);
		user_id_len[0] = 0;
		user_id_len[1] = 64;
	}else
	{
		memcpy(user_id, iUnionPay_id, 16);
		memcpy(user_id_len, iUnionPay_id_len, 2);
	}
	rtn = SM2_Operation(user_id, user_id_len, st_qr_jtb.os_pb_key[0], &st_qr_jtb.os_pb_key[1], NULL,
				&qr_jtb_source[3], len - 65 - 65 - 4 - 3, &st_qr_jtb_ex.os_sign[1], &st_qr_jtb_ex.os_sign[33]);
	if( rtn )
	{
		goto label_bom_sm2err;       //20220222
	}
	//��֤�û�ǩ��  
	if(st_qr_jtb.version == 0x81)
	{
	//�û��ʻ�˽Կǩ����useridȡֵ���û��ʻ��Ź�10�ֽ�
		memcpy(user_id, st_qr_jtb.os_userid, 10);
		user_id_len[0] = 0;
		user_id_len[1] = 80;
	}else
	{
		memcpy(user_id, iUnionPay_id, 16);
		memcpy(user_id_len, iUnionPay_id_len, 2);
	}
	rtn = SM2_Operation(user_id, user_id_len, st_qr_jtb.user_pubkey[0], &st_qr_jtb.user_pubkey[1], NULL, 
				  &qr_jtb_source[0], (sizeof(struct QR_JTB) + sizeof(struct QR_JTB_EX) - 32 - 65 + st_qr_jtb.defined_len), &st_qr_jtb_ex.user_sign[1], &st_qr_jtb_ex.user_sign[33]);
	if( rtn )
	{
		goto label_bom_sm2err;      //20220222
	}
//	//0x81�汾��ά�룬�Ƿǻ�ͨ���ͣ��жϷ����������Ϊ31047700��02002900��Ĭ��Ϊ���ض�ά�룬��������ֹ��
	if( (st_qr_jtb.version == 0x81) && ((memcmp(st_qr_jtb.os_orgnization, "\x31\x04\x77\x00", 4) != 0) && (memcmp(st_qr_jtb.os_orgnization, "\x02\x00\x29\x00", 4) != 0)) ){
		goto label_err_key;     
	}
	if( !((st_qr_jtb.version >=0x80) && (st_qr_jtb.version <= 0xFF)))    //((data[0] >=0x80) && (data[0] <= 0xFF))
		goto label_bom_qrver;  //2022.06.08
	//if( (st_qr_jtb.version != 0x81) && (st_qr_jtb.version != 0x80))
		//goto label_bom_qrver;      //20220222
//		
//	//����0x80�汾��ά�룬�ǻ�ͨ���ͣ��жϷ����������Ϊ31047700��02002900��Ĭ��Ϊ���ض�ά�룬������Ҫ�ж�4020�����������ڵķ�����������뷢��ƽ̨���룬�����ڰ�����������ֹ
//	if( (st_qr_jtb.version == 0x80) && ((memcmp(st_qr_jtb.os_orgnization, "\x31\x04\x77\x00", 4) != 0) && (memcmp(st_qr_jtb.os_orgnization, "\x02\x00\x29\x00", 4) != 0)) )
//	{
//		
//	}
	//uword_to_buf_reverse(st_qr_jtb.len, (char *)&st_qr_jtb.len);
	
	rtn = get_qr_map(st_qr_jtb.os_orgnization, st_qr_jtb.os_platid, 0, st_qr_jtb.version, &map_type);
	if( rtn ){
		goto lable_bom_map;
	}
	
	buf[0] = MQR_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = CE_OK;
	buf[3] = 0x01;
	
	buf[4] = 0x00;		//JTB QR
	//��汾
	buf[5] = st_qr_jtb.version;
	//�û�������	BYTE	2	������ʱΪ�������������ʶ��D.2��02002900��ͨ����Ϊ������Ӧ�����ͣ���ͨ���Զ���������ֶ�14.5��������0000
	memset(&buf[6], 0x00, 2);
	//�û��˻���	BYTE	10	��ͨ��Ψһ���ţ���3104770011223344556F
	memcpy(&buf[8], st_qr_jtb.os_userid, 10);
	//��ҵ�����ֶ�	BYTE	2	������ʱΪ:�����ҵ���ñ�ʶ 02002900��ͨ����Ϊ����ҵ�����ֶΣ���ͨ���Զ���������ֶ�14.4��������FFFF
	memset(&buf[18], 0xff, 2);
	if( ((memcmp(st_qr_jtb.os_orgnization, "\x02\x00\x29\x00", 4) == 0) || (memcmp(st_qr_jtb.os_orgnization, "\x31\x04\x77\x00", 4) == 0)) && (st_qr_jtb.defined_len == 32) )
	{//
		jtb_map = 2;
		if(st_qr_jtb_ex.defined_data[0] & 0x40)
			jtb_map += 1;
		if(st_qr_jtb_ex.defined_data[0] & 0x20)
			jtb_map += 8;
		if( st_qr_jtb_ex.defined_data[0] & 0x10 )
		{
			if ((st_qr_jtb_ex.defined_data[jtb_map] & 0x80) == 0)
				goto label_bom_train_nouse;
			memcpy(&buf[18], &st_qr_jtb_ex.defined_data[jtb_map], 2);
			jtb_map += 2;
		}
		
		if( st_qr_jtb_ex.defined_data[0] & 0x08 ){
			memcpy(&buf[6], &st_qr_jtb_ex.defined_data[jtb_map], 2);
		}
	}
	
	//�û�����	BYTE	1	
	buf[20] = st_qr_jtb.type;
	//���ʹ�ý��	BYTE	3	��λ�� �磺0x0007d0 ��20Ԫ��
	memcpy(&buf[21], st_qr_jtb.maxCosume, 3);
	//��ά����Чʱ��	BYTE	2	��ά����Чʱ�䣬���ά������ʱ��һ����ƶ�ά����Чʱ�䡣����Ϊ��λ��
	memcpy(&buf[24], &st_qr_jtb.valid_time, 2);
	//��ά������ʱ��	BYTE	4	��ά�����ɵ�ʱ�����ʹ��UTC��0ʱ����ʱ��1970��1��1��00:00:00�����ڵ�������
	memcpy(&buf[26], &st_qr_jtb_ex.generate_time, 4);
	//
	ShortToByte(st_qr_jtb.valid_time, (char *)&st_qr_jtb.valid_time);
	LongToByte(st_qr_jtb.valid_date, (char *)&st_qr_jtb.valid_date);
	LongToByte(st_qr_jtb_ex.generate_time, (char *)&st_qr_jtb_ex.generate_time);
	if( (st_qr_jtb_ex.generate_time) > st_qr_jtb.valid_date)
		goto label_bom_qrtimeerr;
	//ǩ����Կ����	BYTE	1	���ڲ��ҹ�Կ������ 
	buf[30] = st_qr_jtb.os_pb_index;
	//������	BYTE	9	��ʱȫ00
	memset(&buf[31], 0x00, 9);
	//��ҵ������	BYTE	1	4020ӳ���Ŀ�����
	buf[40] = map_type;

	memcpy(&buf[41], &qr_jtb_source[0], 213);
	shmt_prot_send(buf, 254);

	
	buf[0] = MQR_READCARD;
	buf[1] = gReadCmdTimes;
	buf[2] = CE_OK;
	buf[3] = 0x82;
	
	memcpy(&buf[4], &qr_jtb_source[213], len - 213);
	shmt_prot_send(buf, len - 213 + 4);
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
	//��ά�����
	buf[4] = 0x00;
	//��汾
	buf[5] = 0x00;
	//�û�������
	memset(&buf[6], 0x00, 2);
	//�û��˻���
	memset(&buf[8], 0x00, 10);
	//��ҵ�����ֶ�
	memset(&buf[18], 0x00, 2);
	//�û�����
	buf[20] = 0x00;
	//���ʹ�ý��
	memset(&buf[21], 0x00, 3);
	//��ά����Чʱ��
	memset(&buf[24], 0x00, 2);
	//��ά������ʱ��
	memset(&buf[26], 0x00, 4);
	//ǩ����Կ����
	buf[30] = 0x00;
	//������
	buf[31] = CE_BOM_CERT;
	memset(&buf[32], 0x00, 8);
	//��ҵ������
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
	//��ά�����
	buf[4] = 0x00;
	//��汾
	buf[5] = 0x00;
	//�û�������
	memset(&buf[6], 0x00, 2);
	//�û��˻���
	memset(&buf[8], 0x00, 10);
	//��ҵ�����ֶ�
	memset(&buf[18], 0x00, 2);
	//�û�����
	buf[20] = 0x00;
	//���ʹ�ý��
	memset(&buf[21], 0x00, 3);
	//��ά����Чʱ��
	memset(&buf[24], 0x00, 2);
	//��ά������ʱ��
	memset(&buf[26], 0x00, 4);
	//ǩ����Կ����
	buf[30] = 0x00;
	//������
	buf[31] = CE_BOM_MAP;
	memset(&buf[32], 0x00, 8);
	//��ҵ������
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
	//��ά�����
	buf[4] = 0x00;
	//��汾
	buf[5] = 0x00;
	//�û�������
	memset(&buf[6], 0x00, 2);
	//�û��˻���
	memset(&buf[8], 0x00, 10);
	//��ҵ�����ֶ�
	memset(&buf[18], 0x00, 2);
	//�û�����
	buf[20] = 0x00;
	//���ʹ�ý��
	memset(&buf[21], 0x00, 3);
	//��ά����Чʱ��
	memset(&buf[24], 0x00, 2);
	//��ά������ʱ��
	memset(&buf[26], 0x00, 4);
	//ǩ����Կ����
	buf[30] = 0x00;
	//������
	buf[31] = CE_BOM_SM2ERR;
	memset(&buf[32], 0x00, 8);
	//��ҵ������
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
	//��ά�����
	buf[4] = 0x00;
	//��汾
	buf[5] = 0x00;
	//�û�������
	memset(&buf[6], 0x00, 2);
	//�û��˻���
	memset(&buf[8], 0x00, 10);
	//��ҵ�����ֶ�
	memset(&buf[18], 0x00, 2);
	//�û�����
	buf[20] = 0x00;
	//���ʹ�ý��
	memset(&buf[21], 0x00, 3);
	//��ά����Чʱ��
	memset(&buf[24], 0x00, 2);
	//��ά������ʱ��
	memset(&buf[26], 0x00, 4);
	//ǩ����Կ����
	buf[30] = 0x00;
	//������
	buf[31] = CE_BOM_TRAIN_NOT_USE;
	memset(&buf[32], 0x00, 8);
	//��ҵ������
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
	//��ά�����
	buf[4] = 0x00;
	//��汾
	buf[5] = 0x00;
	//�û�������
	memset(&buf[6], 0x00, 2);
	//�û��˻���
	memset(&buf[8], 0x00, 10);
	//��ҵ�����ֶ�
	memset(&buf[18], 0x00, 2);
	//�û�����
	buf[20] = 0x00;
	//���ʹ�ý��
	memset(&buf[21], 0x00, 3);
	//��ά����Чʱ��
	memset(&buf[24], 0x00, 2);
	//��ά������ʱ��
	memset(&buf[26], 0x00, 4);
	//ǩ����Կ����
	buf[30] = 0x00;
	//������
	buf[31] = CE_BOM_CHUXING_NOT_USE;
	memset(&buf[32], 0x00, 8);
	//��ҵ������
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
	//��ά�����
	buf[4] = 0x00;
	//��汾
	buf[5] = 0x00;
	//�û�������
	memset(&buf[6], 0x00, 2);
	//�û��˻���
	memset(&buf[8], 0x00, 10);
	//��ҵ�����ֶ�
	memset(&buf[18], 0x00, 2);
	//�û�����
	buf[20] = 0x00;
	//���ʹ�ý��
	memset(&buf[21], 0x00, 3);
	//��ά����Чʱ��
	memset(&buf[24], 0x00, 2);
	//��ά������ʱ��
	memset(&buf[26], 0x00, 4);
	//ǩ����Կ����
	buf[30] = 0x00;
	//������
	buf[31] = CE_BOM_QR_TIME_ERR;
	memset(&buf[32], 0x00, 8);
	//��ҵ������
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
	//��ά�����
	buf[4] = 0x00;
	//��汾
	buf[5] = 0x00;
	//�û�������
	memset(&buf[6], 0x00, 2);
	//�û��˻���
	memset(&buf[8], 0x00, 10);
	//��ҵ�����ֶ�
	memset(&buf[18], 0x00, 2);
	//�û�����
	buf[20] = 0x00;
	//���ʹ�ý��
	memset(&buf[21], 0x00, 3);
	//��ά����Чʱ��
	memset(&buf[24], 0x00, 2);
	//��ά������ʱ��
	memset(&buf[26], 0x00, 4);
	//ǩ����Կ����
	buf[30] = 0x00;
	//������
	buf[31] = CE_BOM_QR_VER_ERR;
	memset(&buf[32], 0x00, 8);
	//��ҵ������
	buf[40] = 0x00;
	
	shmt_prot_send(buf, 41);
	//gResponseStatus = CE_BOM_QR_VER_ERR;
	//return CE_BOM_QR_VER_ERR;
	return 0;
}

//ʹ��ס����SAM����TAC������Կ��
//a.	����ͨ��DES�����ʼ��ָ�ָ���ʽ���£�
//801A440110 112233445566778F(�û��ʻ��ź�8�ֽ�) 02002900FF000000���̶������������룩
//b.	����ͨ��DES����ָ�ָ���ʽ����:
//80 FA 01 00 20 + DATA

UBYTE	qr_jtb_tac(UBYTE *in_buf, UBYTE *out_buf)
{
UBYTE	inbuf[255], outbuf[255], KeyBuf[100],buf1[100];
UBYTE	inbytes, obytes;
int 	ret, i;
	
	if(gPSAMFileFlag != 0x00){
		bgSHpsamIndex = SAMLOCATION_5;
	memcpy(KeyBuf,"\x00\xa4\x00\x00\x02\x10\x03",7);
	i = 7;
	ret = sam_apdu(bgSHpsamIndex,KeyBuf,i,buf1,&obytes,0,0);
	if(ret != 0)
	{ 
		#ifdef _DEBUG_GETCARDINFO_
		debug_printf("\x0d\x0a Err:getcardinfo,select 1003");
		#endif
		bgSHpsamIndex = SAMLOCATION_5;
		sam_select(bgSHpsamIndex);
		sam_set(bgSHpsamIndex,SAM_ETU_93,16);
		ret = sam_atr(bgSHpsamIndex,buf1,&obytes);
		if(ret != 0)
		{
			goto label_err;
		}
		memcpy(KeyBuf,"\x00\xa4\x00\x00\x02\x10\x03",7);
		i = 7;
		ret = sam_apdu(bgSHpsamIndex,KeyBuf,i,buf1,&obytes,0,0);
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
	ret = sam_apdu(bgSHpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);

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
	//�������ͱ�־	H	1
  	inbuf[5] = 0x88;
	//�ն��豸��ˮ��	H	4
  	memcpy(&inbuf[6], &in_buf[12], 4);
	//�ն��豸�ڵ�	H	4
	memcpy(&inbuf[10], &in_buf[16], 4);
	//�ն��豸����ʱ��	H	4
	memcpy(&inbuf[14], &in_buf[20], 4);
	//������������ˮ�ţ��ѻ���0��	H	4
	memcpy(&inbuf[18], &in_buf[0], 4);
	//����������ʱ�䣨�ѻ���0��	H	4
	memcpy(&inbuf[22], &in_buf[4], 4);
	//���������ؽ��ѻ���0��	H	4
	memcpy(&inbuf[26], &in_buf[8], 4);
	//��ά�����	H	1
	//��ά���˻���	H	10
	//��ά����Чʱ��	H	2
	//��ά������ʱ��	H	4
	memcpy(&inbuf[30], &in_buf[24], 17);
	//SAM����	D	4
	memcpy(&inbuf[47], bpgSHpsamNo, 4);
	//����ַ�	H	2
	memcpy(&inbuf[51], "\x80\x00", 2);
  	inbytes = 53;
	//delay_ms(6);
  	ret = sam_apdu(bgSHpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
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
	  	ret = sam_apdu(bgSHpsamIndex,inbuf,inbytes,outbuf,&obytes,0,0);
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
	
#endif