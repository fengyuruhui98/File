#ifndef QR_SSM_H
#define QR_SSM_H

struct QR_SSM
{
	unsigned char	title[20];			//A	����	20	https://s.sh.gov.cn/
	unsigned int	len;				//B	�볤��	4	��A��B���⣬�������롱��ֵ���ȣ��ֽ�����
	unsigned char 	sign[64];			//C	����ǩ��	64	ʹ�á������롱֤��˽Կ�ԡ������롱��A��B��C��FF���������ֵ����SM2ǩ�����ǩ��ֵ
	//D	��������	47	
	unsigned char 	pb_uuid[32];		//D.1	UUID	32	�������롱UUID��Ψһ
	unsigned char	pb_special[2];		//D.2	��ʶ��	2	�����������ʶ��
	unsigned char	pb_type[2];			//D.3	�ͻ������ͣ�	2	������족APP: 06 ������족΢��С����: 07 ������족֧����С����: 08
	unsigned char 	pb_valid_date[14];	//D.4	��ʧЧʱ��� 	14	20211231235959
	unsigned char 	pb_index;			//D.5	B��ǩ����Ӧ�Ĺ�Կ֤����Կ����	��������	1
	//E	��ͨ��ҵ��չ��	96	
	//E.1	��ͨ��չ��	32	���±�
	unsigned char 	jt_sn[10];			//1	��ͨ����	10	��ͨ��Ψһ���ţ���3104770011223344556F
	unsigned char 	jt_flag[2];			//2	��ҵ�����ֶ�	2	�����ҵ���ñ�ʶ����λʹ�ã����±�
	unsigned char	jt_type;			//3	�û�����	1	0x01-0x64���磺01 ��ͨ��     02 ���˿� 03 ���˿� 04 ���Կ�
	unsigned char	jt_maxCosume[3];	//4	���ʹ�ý��	2	��λ�� �磺0x07d0 ��20Ԫ��
	unsigned short	jt_valid_time;		//5	��ά����Чʱ��	2	��ά����Чʱ�䣬���ά������ʱ��һ����ƶ�ά����Чʱ�䡣����Ϊ��λ��
	unsigned int	jt_generate_time;	//6	��ά������ʱ��	4	��ά�����ɵ�ʱ�����ʹ��UTC��0ʱ����ʱ��1970��1��1��00:00:00�����ڵ�������
	unsigned char 	jt_index;			//7	ǩ����Կ����	1	���ڲ��ҹ�Կ������ 
	unsigned char 	jt_rfu[9];			//8	������	8	��ʱȫ00
	//E.2	����ǩ��	64	�Թ�������D.1��D.2��D.3��D.4����ҵ��չ��E.1ʹ����ҵ˽ԿSM2ǩ�����ǩ��ֵ
	unsigned char	jt_sign[64];		
	//��	��	��	
	unsigned char 	tail[2];			//FF	 ## 	2	
}__attribute__( ( packed, aligned(1) ) );


UBYTE getinfo_QR_SSM(unsigned char *data, int len);


#endif