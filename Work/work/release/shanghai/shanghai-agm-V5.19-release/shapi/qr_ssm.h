#ifndef QR_SSM_H
#define QR_SSM_H

struct QR_SSM
{
	unsigned char	title[20];			//A	域名	20	https://s.sh.gov.cn/
	unsigned int	len;				//B	码长度	4	除A、B域外，“随申码”码值长度（字节数）
	unsigned char 	sign[64];			//C	数字签名	64	使用“随申码”证书私钥对“随申码”除A、B、C、FF域以外的码值进行SM2签名后的签名值
	//D	公用区域	47	
	unsigned char 	pb_uuid[32];		//D.1	UUID	32	“随申码”UUID，唯一
	unsigned char	pb_special[2];		//D.2	标识域	2	随申码特殊标识域
	unsigned char	pb_type[2];			//D.3	客户端类型，	2	“随申办”APP: 06 “随申办”微信小程序: 07 “随申办”支付宝小程序: 08
	unsigned char 	pb_valid_date[14];	//D.4	码失效时间戳 	14	20211231235959
	unsigned char 	pb_index;			//D.5	B域签名对应的公钥证书密钥索引	二进制数	1
	//E	交通行业扩展域	96	
	//E.1	交通扩展域	32	见下表
	unsigned char 	jt_sn[10];			//1	交通卡号	10	交通码唯一卡号：如3104770011223344556F
	unsigned char 	jt_flag[2];			//2	行业启用字段	2	相关行业启用标识，按位使用，见下表
	unsigned char	jt_type;			//3	用户类型	1	0x01-0x64：如：01 普通卡     02 老人卡 03 军人卡 04 测试卡
	unsigned char	jt_maxCosume[3];	//4	最大使用金额	2	单位分 如：0x07d0 （20元）
	unsigned short	jt_valid_time;		//5	二维码有效时间	2	二维码有效时间，与二维码生成时间一起控制二维码有效时间。以秒为单位。
	unsigned int	jt_generate_time;	//6	二维码生成时间	4	二维码生成的时间戳，使用UTC（0时区）时间1970年1月1日00:00:00到现在的秒数。
	unsigned char 	jt_index;			//7	签名公钥索引	1	用于查找公钥的索引 
	unsigned char 	jt_rfu[9];			//8	保留域	8	暂时全00
	//E.2	数字签名	64	对公共区域D.1、D.2、D.3、D.4及行业扩展域E.1使用行业私钥SM2签名后的签名值
	unsigned char	jt_sign[64];		
	//…	…	…	
	unsigned char 	tail[2];			//FF	 ## 	2	
}__attribute__( ( packed, aligned(1) ) );


UBYTE getinfo_QR_SSM(unsigned char *data, int len);


#endif