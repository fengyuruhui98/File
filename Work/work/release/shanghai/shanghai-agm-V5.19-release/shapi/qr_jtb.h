#ifndef QR_JTB_H
#define QR_JTB_H

struct os_pb_key
{//3	发码机构公钥证书	117	发码机构公钥证书。	
	unsigned char 	os_pb_title;		//1	记录头	1	十六进制值‘24’
	unsigned char 	os_pb_serivce[4];	//2	服务标识	4	标识一个交通服务，将相应应用的私有应用标识扩展(PIX)，右补十六进制‘0’构成‘01010000’ = 交通电子现金应用
	unsigned char 	os_pb_index;		//3	根 CA 公钥索引	1	根 CA 系统用来签发发码机构公钥证书的公钥索引
	unsigned char 	os_pb_format;		//4	证书格式	1	十六进制，值为‘12’
	unsigned char	os_pb_id[4];			//5	发码机构标识	4	由证书管理中心统一制定并下发
	unsigned char 	os_pb_date[2];		//6	证书失效日期	2	MMYY，在此日期后，这张证书无效
	unsigned char 	os_pb_sn[3];		//7	证书序列号	3	由根 CA 分配给这张证书的，唯一的二进制数
	unsigned char	os_pb_sign_al;		//8	发码机构公钥签名算法标识	1	标识发码机构公钥签名算法‘04’：SM2（16进制）
	unsigned char 	os_pb_enc_al;		//9	发码机构公钥加密算法标识	1	标识发码机构公钥加密算法‘04’：SM2（16进制）
	unsigned char 	os_pb_para;			//10	发码机构公钥参数标识	1	用于标识椭圆曲线参数。默认为16进制‘00’
	unsigned char 	os_pb_len;			//11	发码机构公钥长度	1	标识发码机构公钥的字节长度
	unsigned char 	os_pb_key[33];		//12	发码机构公钥	33	该字段是椭圆曲线上的一个点
	unsigned char 	os_pb_sign[64];		//13	数字签名	64	根 CA 对本表4至12项数据计算的 SM2 签名 r||s
}__attribute__( ( packed, aligned(1) ) );

struct QR_JTB
{
	unsigned char version;			//1	二维码版本	1	二维码版本号。	0x80为互通版本/0x81为本地版本
	unsigned short len;				//2	二维码数据长度	2	第3～17字段的总长度。	
	//3	发码机构公钥证书	117	发码机构公钥证书。	
	unsigned char 	os_pb_title;		//1	记录头	1	十六进制值‘24’
	unsigned char 	os_pb_service[4];	//2	服务标识	4	标识一个交通服务，将相应应用的私有应用标识扩展(PIX)，右补十六进制‘0’构成‘01010000’ = 交通电子现金应用
	unsigned char 	os_pb_index;		//3	根 CA 公钥索引	1	根 CA 系统用来签发发码机构公钥证书的公钥索引
	unsigned char 	os_pb_format;		//4	证书格式	1	十六进制，值为‘12’
	unsigned char	os_pb_id[4];			//5	发码机构标识	4	由证书管理中心统一制定并下发
	unsigned char 	os_pb_date[2];		//6	证书失效日期	2	MMYY，在此日期后，这张证书无效
	unsigned char 	os_pb_sn[3];		//7	证书序列号	3	由根 CA 分配给这张证书的，唯一的二进制数
	unsigned char	os_pb_sign_al;		//8	发码机构公钥签名算法标识	1	标识发码机构公钥签名算法‘04’：SM2（16进制）
	unsigned char 	os_pb_enc_al;		//9	发码机构公钥加密算法标识	1	标识发码机构公钥加密算法‘04’：SM2（16进制）
	unsigned char 	os_pb_para;			//10	发码机构公钥参数标识	1	用于标识椭圆曲线参数。默认为16进制‘00’
	unsigned char 	os_pb_len;			//11	发码机构公钥长度	1	标识发码机构公钥的字节长度
	unsigned char 	os_pb_key[33];		//12	发码机构公钥	33	该字段是椭圆曲线上的一个点
	unsigned char 	os_pb_sign[64];		//13	数字签名	64	根 CA 对本表4至12项数据计算的 SM2 签名 r||s
	
	unsigned char 	pay_sn[16];			//4	支付账户号	16	由支付账户系统自定义。	
	unsigned char 	os_userid[10];		//5	用户帐户号	10	由发码机构账户管理平台定义。	如：3104770011223344556F 交易过程中的唯一卡号
	unsigned char 	os_orgnization[4];	//6	发码机构号	4	由清分结算机构统一分配。	上海：02002900或31047700其他是异地
	unsigned char 	os_platid[4];		//7	发码平台编号	4	由清分结算机构统一分配。	
	unsigned char 	type;				//8	卡账户类型	1	用户帐户号的类型。（见JTT978.3）	00或01：普通卡 02：学生卡 03：老人卡 04：测试卡 05：军人卡
	unsigned char	maxCosume[3];		//9	单次消费金额上限	3	二维码支付单次消费金额上限，由支付账户系统根据当前用户消费状态进行授权。此域在单次消费交易时可作为能否乘车的判断依据。	如：0007D0即46元
	unsigned char 	user_pubkey[33];	//10	用户帐户公钥	33	经过压缩的支付账户系统中用户帐户公钥数据，压缩方法见GM/T0003。	
	unsigned int	valid_date;			//11	用户帐户系统授权过期时间	4	用户账户系统授权过期时间，使用UTC（0时区）时间1970年1月1日00:00:00到现在的秒数。	
	unsigned short	valid_time;			//12	二维码有效时间	2	二维码有效时间，与二维码生成时间一起控制二维码有效时间。以秒为单位。	
	unsigned char 	defined_len;		//13	发码机构自定义域长度	1	发码机构自定义域数据长度，最大32	最大0x20
}__attribute__( ( packed, aligned(1) ) );

struct QR_JTB_EX
{
	unsigned char	defined_data[32];	//14	发码机构自定义域	32	发码机构自定义，由发码机构自定义。	
	unsigned char	os_sign[65];		//15	发码机构授权签名	65	发码机构私钥签名，签名数据包括：本表中3～14字段。	
	unsigned int 	generate_time;		//16	二维码生成时间	4	二维码生成的时间戳，使用UTC（0时区）时间1970年1月1日00:00:00到现在的秒数。	
	unsigned char 	user_sign[65];		//17	用户账户私钥签名	65	用户账户私钥签名数据，签名数据包括：本表中1～16字段。	
}__attribute__( ( packed, aligned(1) ) );


UBYTE getinfo_QR_JTB(unsigned char *data, int len);
UBYTE	qr_jtb_tac(UBYTE *in_buf, UBYTE *out_buf);

#endif