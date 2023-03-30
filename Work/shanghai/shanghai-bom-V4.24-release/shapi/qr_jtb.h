#ifndef QR_JTB_H
#define QR_JTB_H

struct os_pb_key
{//3	���������Կ֤��	117	���������Կ֤�顣	
	unsigned char 	os_pb_title;		//1	��¼ͷ	1	ʮ������ֵ��24��
	unsigned char 	os_pb_serivce[4];	//2	�����ʶ	4	��ʶһ����ͨ���񣬽���ӦӦ�õ�˽��Ӧ�ñ�ʶ��չ(PIX)���Ҳ�ʮ�����ơ�0�����ɡ�01010000�� = ��ͨ�����ֽ�Ӧ��
	unsigned char 	os_pb_index;		//3	�� CA ��Կ����	1	�� CA ϵͳ����ǩ�����������Կ֤��Ĺ�Կ����
	unsigned char 	os_pb_format;		//4	֤���ʽ	1	ʮ�����ƣ�ֵΪ��12��
	unsigned char	os_pb_id[4];			//5	���������ʶ	4	��֤���������ͳһ�ƶ����·�
	unsigned char 	os_pb_date[2];		//6	֤��ʧЧ����	2	MMYY���ڴ����ں�����֤����Ч
	unsigned char 	os_pb_sn[3];		//7	֤�����к�	3	�ɸ� CA ���������֤��ģ�Ψһ�Ķ�������
	unsigned char	os_pb_sign_al;		//8	���������Կǩ���㷨��ʶ	1	��ʶ���������Կǩ���㷨��04����SM2��16���ƣ�
	unsigned char 	os_pb_enc_al;		//9	���������Կ�����㷨��ʶ	1	��ʶ���������Կ�����㷨��04����SM2��16���ƣ�
	unsigned char 	os_pb_para;			//10	���������Կ������ʶ	1	���ڱ�ʶ��Բ���߲�����Ĭ��Ϊ16���ơ�00��
	unsigned char 	os_pb_len;			//11	���������Կ����	1	��ʶ���������Կ���ֽڳ���
	unsigned char 	os_pb_key[33];		//12	���������Կ	33	���ֶ�����Բ�����ϵ�һ����
	unsigned char 	os_pb_sign[64];		//13	����ǩ��	64	�� CA �Ա���4��12�����ݼ���� SM2 ǩ�� r||s
}__attribute__( ( packed, aligned(1) ) );

struct QR_JTB
{
	unsigned char version;			//1	��ά��汾	1	��ά��汾�š�	0x80Ϊ��ͨ�汾/0x81Ϊ���ذ汾
	unsigned short len;				//2	��ά�����ݳ���	2	��3��17�ֶε��ܳ��ȡ�	
	//3	���������Կ֤��	117	���������Կ֤�顣	
	unsigned char 	os_pb_title;		//1	��¼ͷ	1	ʮ������ֵ��24��
	unsigned char 	os_pb_service[4];	//2	�����ʶ	4	��ʶһ����ͨ���񣬽���ӦӦ�õ�˽��Ӧ�ñ�ʶ��չ(PIX)���Ҳ�ʮ�����ơ�0�����ɡ�01010000�� = ��ͨ�����ֽ�Ӧ��
	unsigned char 	os_pb_index;		//3	�� CA ��Կ����	1	�� CA ϵͳ����ǩ�����������Կ֤��Ĺ�Կ����
	unsigned char 	os_pb_format;		//4	֤���ʽ	1	ʮ�����ƣ�ֵΪ��12��
	unsigned char	os_pb_id[4];			//5	���������ʶ	4	��֤���������ͳһ�ƶ����·�
	unsigned char 	os_pb_date[2];		//6	֤��ʧЧ����	2	MMYY���ڴ����ں�����֤����Ч
	unsigned char 	os_pb_sn[3];		//7	֤�����к�	3	�ɸ� CA ���������֤��ģ�Ψһ�Ķ�������
	unsigned char	os_pb_sign_al;		//8	���������Կǩ���㷨��ʶ	1	��ʶ���������Կǩ���㷨��04����SM2��16���ƣ�
	unsigned char 	os_pb_enc_al;		//9	���������Կ�����㷨��ʶ	1	��ʶ���������Կ�����㷨��04����SM2��16���ƣ�
	unsigned char 	os_pb_para;			//10	���������Կ������ʶ	1	���ڱ�ʶ��Բ���߲�����Ĭ��Ϊ16���ơ�00��
	unsigned char 	os_pb_len;			//11	���������Կ����	1	��ʶ���������Կ���ֽڳ���
	unsigned char 	os_pb_key[33];		//12	���������Կ	33	���ֶ�����Բ�����ϵ�һ����
	unsigned char 	os_pb_sign[64];		//13	����ǩ��	64	�� CA �Ա���4��12�����ݼ���� SM2 ǩ�� r||s
	
	unsigned char 	pay_sn[16];			//4	֧���˻���	16	��֧���˻�ϵͳ�Զ��塣	
	unsigned char 	os_userid[10];		//5	�û��ʻ���	10	�ɷ�������˻�����ƽ̨���塣	�磺3104770011223344556F ���׹����е�Ψһ����
	unsigned char 	os_orgnization[4];	//6	���������	4	����ֽ������ͳһ���䡣	�Ϻ���02002900��31047700���������
	unsigned char 	os_platid[4];		//7	����ƽ̨���	4	����ֽ������ͳһ���䡣	
	unsigned char 	type;				//8	���˻�����	1	�û��ʻ��ŵ����͡�����JTT978.3��	00��01����ͨ�� 02��ѧ���� 03�����˿� 04�����Կ� 05�����˿�
	unsigned char	maxCosume[3];		//9	�������ѽ������	3	��ά��֧���������ѽ�����ޣ���֧���˻�ϵͳ���ݵ�ǰ�û�����״̬������Ȩ�������ڵ������ѽ���ʱ����Ϊ�ܷ�˳����ж����ݡ�	�磺0007D0��46Ԫ
	unsigned char 	user_pubkey[33];	//10	�û��ʻ���Կ	33	����ѹ����֧���˻�ϵͳ���û��ʻ���Կ���ݣ�ѹ��������GM/T0003��	
	unsigned int	valid_date;			//11	�û��ʻ�ϵͳ��Ȩ����ʱ��	4	�û��˻�ϵͳ��Ȩ����ʱ�䣬ʹ��UTC��0ʱ����ʱ��1970��1��1��00:00:00�����ڵ�������	
	unsigned short	valid_time;			//12	��ά����Чʱ��	2	��ά����Чʱ�䣬���ά������ʱ��һ����ƶ�ά����Чʱ�䡣����Ϊ��λ��	
	unsigned char 	defined_len;		//13	��������Զ����򳤶�	1	��������Զ��������ݳ��ȣ����32	���0x20
}__attribute__( ( packed, aligned(1) ) );

struct QR_JTB_EX
{
	unsigned char	defined_data[32];	//14	��������Զ�����	32	��������Զ��壬�ɷ�������Զ��塣	
	unsigned char	os_sign[65];		//15	���������Ȩǩ��	65	�������˽Կǩ����ǩ�����ݰ�����������3��14�ֶΡ�	
	unsigned int 	generate_time;		//16	��ά������ʱ��	4	��ά�����ɵ�ʱ�����ʹ��UTC��0ʱ����ʱ��1970��1��1��00:00:00�����ڵ�������	
	unsigned char 	user_sign[65];		//17	�û��˻�˽Կǩ��	65	�û��˻�˽Կǩ�����ݣ�ǩ�����ݰ�����������1��16�ֶΡ�	
}__attribute__( ( packed, aligned(1) ) );


UBYTE getinfo_QR_JTB(unsigned char *data, int len);
UBYTE	qr_jtb_tac(UBYTE *in_buf, UBYTE *out_buf);

#endif