#ifndef BIN_FILE_MANAGE
#define BIN_FILE_MANAGE

//test
#define	DEBUG_TICKET		1

extern unsigned char reader_status;

//eeprom 8192byte
#define	EE_RESTART			8190		//1byte
#define	EE_UL_BACKUP		6144		//64+9byte+100

//0~99byte for UL transaction record
//0:upload the record flags:0,no; 1,ok
//1~4:record position
//5~75:ul record
#define	EE_UL_TRANSACTION	0

//100~199byte for metro-CPU transaction record
//0:upload the record flag :0,no; 1,ok
//1~4:record position
//105~ metro-cpu record
#define	EE_MCPU_TRANSACTION	100

#define	KM_RW_STOP			0xff
#define	KM_RW_IDLE			0x00
#define	KM_RW_SEARCH		0x01
#define	KM_RW_TRANSACTION	0x02
#define	KM_RW_RECORD		0x04

#define	SZ_DOWNLOAD_PARA	0
#define SZ_DOWNLOAD_TP		1
#define	SZ_DOWNLOAD_MODEL	2

#define	SZ_WAIVER_NOMAL		0
#define	SZ_WAIVER_FAILURE	1
#define	SZ_WAIVER_ENTRY		2
#define	SZ_WAIVER_DATE		3
#define	SZ_WAIVER_FARE		5
#define	SZ_WAIVER_EMERGENCY	6

long	tp_ver, wdmc_ver;

struct EOD_DOWNLOAD{
	unsigned char 	filetype;
	unsigned char 	filepath[200];
	unsigned char 	filename[41];
	unsigned long	  filelen;
	unsigned char 	md5[16];

	long		curFilelen;		//have sended file len
	unsigned short	totalFrame;		//total frame number
	unsigned short	curFrame;		//current frame
};
struct STATION_WAIVER_MODE{
	unsigned long waivermode_len;
	unsigned char *waivermode_val;
};
struct waiver_mode{
	unsigned char cur_sta_failure;
	unsigned char cur_sta_entry;
	unsigned char cur_sta_date;
	unsigned char cur_sta_fare;
	unsigned char cur_sta_emergency;
	
	unsigned char oth_sta_failure;
	unsigned char oth_sta_entry;
	unsigned char oth_sta_date;
	unsigned char oth_sta_fare;
	unsigned char oth_sta_emergency;
	
	unsigned long oth_entry_num;
	char *oth_entry_station;
	unsigned long sen_entry_num;
	char *sen_entry_station;
	
	unsigned char sen_sta_failure;
	unsigned char sen_sta_entry;
	unsigned char sen_sta_date;
	unsigned char sen_sta_fare;
	unsigned char sen_sta_emergency;
};

//4020白名单参数		2019-06-04
struct White_Head
{
	unsigned long ParaVersion;		//参数版本	Long	4
	unsigned long ContentNumber;	//内容记录数	Long	4
	unsigned char SHA_value[20];			//记录区校验值	Byte	20	
}__attribute__( ( packed, aligned(1) ) );
typedef	struct White_Head	White_Head_t;

struct White_Content
{
//车票类型对应记录
	unsigned char CardIssuer[8];			//发卡机构标识	全部匹配（直接比较） Byte  8
	unsigned char IIN[4];							//6位卡iin+2位扩展（0015文件中应用序列号第2-9位）	全部匹配（直接比较） Byte  4
	unsigned char CardType;						//卡类型	住建部白名单时作为卡版本	Byte	1
	unsigned char RFU[2];							//预留	Byte	2
	unsigned char TicketType;					//已有票种类型	Byte	1
}__attribute__( ( packed, aligned(1) ) );
typedef struct White_Content	White_Content_t;

struct White_4020
{
	White_Head_t	white_head;	
	White_Content_t	*White_Content_val;
}__attribute__( ( packed, aligned(1) ) );
typedef	struct White_4020	White_4020_t;

//4026-交通部二维码根证书文件
struct Cert_Head
{
	unsigned int 	ParaVersion;		//参数版本		Long	4
	unsigned int 	ContentNumber;		//内容记录数		Long	4
	unsigned char 	rfu[4];				//保留域		Byte	4
	unsigned char 	SHA_value[20];			//记录区校验值	记录区数据的SHA-1值	Byte	20
}__attribute__( ( packed, aligned(1) ) );
typedef	struct Cert_Head	Cert_Head_t;

struct Cert_Content
{
//证书对应记录	
	unsigned char 	type;				//二维码介质		Byte	1
	unsigned char 	ver;				//版本号	0x80～0xFF	Byte	1
	unsigned char	head;				//记录头	十六进制0x21	Byte	1
	unsigned char	service_flag[4];	//服务标识	‘01010000’ = 借、贷记	Byte	4
	unsigned char 	rid[5];				//注册的应用提供商标识 (RID)	标识 RID：为十六进’A000000632’	Byte	5
	unsigned char	index;				//根 CA 公钥索引	唯一标识根 CA 公钥	Byte	1
	unsigned char	valid_date[2];		//证书失效日期	月和年(MMYY)，在该月最后一日之后证书失效	Byte	2
	unsigned char	al;					//根 CA 公钥算法标识	标识发卡机构公钥签名算法‘04’：SM2（16进制）	Byte	1
	unsigned char	al_flag;			//根 CA 公钥参数标识	用于标识椭圆曲线参数。‘00’	Byte	1
	unsigned char 	len;				//根 CA 公钥长度	标识发卡机构公钥的字节长度	Byte	1
	unsigned char 	pb[64];				//根 CA 公钥	该字段是椭圆曲线上的一个点	Byte	64
	unsigned char 	sign[64];			//数字签名	根 CA 公钥指数。	Byte	64
	unsigned char 	rfu[14];			//RFU	预留	Byte	14
}__attribute__( ( packed, aligned(1) ) );
typedef struct Cert_Content	Cert_Content_t;

struct Cert_4026
{
	Cert_Head_t	cert_head;	
	Cert_Content_t	*Cert_Content_val;
}__attribute__( ( packed, aligned(1) ) );
typedef	struct Cert_4026	Cert_4026_t;

White_4020_t	tpWhite4020;
Cert_4026_t		tpCert4026;

//正在使用的参数表表头变量
struct PublicTitle tpPubTitle1040, tpPubTitle2000, tpPubTitle3002, tpPubTitle3004, tpPubTitle3006, tpPubTitle3007;                    //
struct PublicTitle tpPubTitle3008, tpPubTitle3009, tpPubTitle3010, tpPubTitle3011, tpPubTitle3082, tpPubTitle3083;                    //
struct PublicTitle tpPubTitle3084, tpPubTitle3085, tpPubTitle4001, tpPubTitle4002, tpPubTitle4003, tpPubTitle4004;                    //
struct PublicTitle tpPubTitle4006, tpPubTitle4007, tpPubTitle4008, tpPubTitle4009, tpPubTitle4015;                    //
//新的参数表表头变量
struct PublicTitle tpNewPubTitle1040, tpNewPubTitle2000, tpNewPubTitle3002, tpNewPubTitle3004, tpNewPubTitle3006;                 //
struct PublicTitle tpNewPubTitle3007, tpNewPubTitle3008, tpNewPubTitle3009, tpNewPubTitle3010, tpNewPubTitle3011;                 //
struct PublicTitle tpNewPubTitle3082, tpNewPubTitle3083, tpNewPubTitle3084, tpNewPubTitle3085, tpNewPubTitle4001;                 //
struct PublicTitle tpNewPubTitle4002, tpNewPubTitle4003, tpNewPubTitle4004, tpNewPubTitle4006, tpNewPubTitle4007;                 //
struct PublicTitle tpNewPubTitle4008, tpNewPubTitle4009, tpNewPubTitle4015;                 //
//
struct ARRAY_BOUND tpArraybound;
//使用中的参数表内容变量
struct PARA_1040 tpStationSet1040;                    //车站配置
struct PARA_1040_SLE *tpSLESet1040sub;         		 //
struct PARA_2000 tpCommuPara2000;                     //线路内部通信参数
struct PARA_3002 tpAFCOperationPara3002;              //AFC设备运行参数
struct PARA_3004 tpBOMOperationPara3004;              //BOM运行参数
struct PARA_3006 *tpStationName3006;           		 //车站名称/线路设置表
struct PARA_3007 *tpLineName3007;                     //线路名称表
struct PARA_3008 *tpFailCode3008;                     //系统故障代码
struct PARA_3009 *tpOperator3009;                     //操作员表
struct PARA_3009_LINE *tpOperator3009LINE;            //操作员表
struct PARA_3010 *tpResource3010;                     //本地语言资源文件
struct PARA_3010 *tpResource3011;                     //清分语言资源文件
struct PARA_3082 *tpExchangeSta3082;                  //
struct PARA_3083 *tpMainInterface3083;
struct PARA_3084 *tpInterStation3084;
struct PARA_4001 *tpHoliday4001;                      //节日表
struct PARA_4002 tpTicketSet4002;                     //车票类型表
struct PARA_4002_TICKET *tpTicketInfo4002sub;         //
struct PARA_4002_TICKET tpTicketDef;                  //
struct PARA_4003 *tpFareValue4003;                    //费率表
unsigned short tpNetSJTPrice[64];						  //路网所有票价
struct PARA_4004 tpStationFare4004;                    //区域表
struct PARA_4004_FARE *tpStationFare4004sub;         	//
unsigned char *tpFare;                                //具体的费率区域
struct PARA_4006 *tpNotPeak4006;                      //非高峰时段表
struct PARA_4007_GRP *tpTicketBlacklist4007Grp;       //车票黑名单
struct PARA_4008_GRP *tpCSCBlacklist4008Grp;          //车票黑名单
struct PARA_4009 *tpTicketMap4009;                    //
struct PARA_4015 *tpCMCCMap4015;  
//
struct PARA_6002	tpUL6002;
unsigned short 		para_type;						//记录交易记录消息类型码
unsigned short 		position_6002;				//
//当前车站信息
unsigned short *tpSJTPrice;								 //可售票价

struct EOD_DOWNLOAD eod_download;
struct STATION_WAIVER_MODE tpStationWaiverMode;
struct waiver_mode	tpwaivermode;

int active_eod_file(char * filename, long *lngver, char ver_len);
int active_tp_file(char * filename, long *lngver);

char check_station_id(unsigned char *station_id);
int cal_fare_time(unsigned char faretier, unsigned long *lngovertime);
char cal_overtime(unsigned char *entrytime, unsigned char *curtime, unsigned char mileclass, unsigned char station_mode);
int cal_fare_value(unsigned char *srcstation_id, unsigned char *curtime, struct PARA_4002_TICKET *ticket, unsigned char *faretier, unsigned short *lngFareValue);
int fast_cal_fare(unsigned short shValue, unsigned char *faretier);
//int get_ticket_para(unsigned char chTickettype, TicketDefinition_t *td);

void get_cur_para_ver(short eod_type, unsigned char *eod_num, long eod_ver, unsigned char *out_buf);
void get_temp_para_ver(unsigned char *eod_file, short eod_type, unsigned char *eod_num, char eod_ctrl, unsigned char *out_buf);


long EOD_1040_StationSet();
long EOD_2000_CommuPara();
long EOD_3002_AFCOperationPara();
long EOD_3004_BOMOperationPara();
long EOD_3006_StationName();
long EOD_3007_LineName();
long EOD_3008_FailCode();
long EOD_3009_Operator();
long EOD_3010_Resource();
long EOD_3011_Resource();
long EOD_3082_ExchangeStation();
long EOD_3083_MainInterface();
long EOD_3085_ExitEntryAgain();
long EOD_3084_InterStation();
long EOD_4001_Holiday();
long EOD_4002_TicketSet();
long EOD_4003_FareValue();
long EOD_4004_StationFare();
long EOD_4006_NotPeak();
long EOD_4007_TicketBlacklist();
long EOD_4008_CSCBlacklist();
long EOD_4009_TicketMap();
long EOD_4015_CMCCMap();
//201906604
long EOD_4020_WhiteList();

unsigned short ee_write_last_record(unsigned short message_type);
unsigned short ee_read_last_record(unsigned short message_type, unsigned char *out_buf, unsigned char *out_len, char upload_control);

unsigned char IsTodayHoliday();
long InitPara();
void ReadFileTitle(char *strFileType, struct PublicTitle *tpPubTitle);
void InitNewPara();

void TicketValue(unsigned char bytTicketNum, unsigned char NotPeakCode,
                 unsigned char RebateCode, unsigned char CostCode, unsigned char TicketArea);
void *ExplainRejectNO(long lngRejectNO, char bytLanguageType);
char *SearchStationName(unsigned char bytcardline, unsigned char bytcardstation, char *strstaname, char chLanguage);
char *SearchLineName(unsigned char bytcardline, char *strlinename, char chLanguage);
void DealCardType(unsigned char bytCardType, char bytLine, char bytStation, char bytLanguageSel);
void StationPrice(unsigned char bytCardType, char bytLine, char bytStation, char bytLanguageSel);

void GetSJTPrice(unsigned char bytNotPeakCode, unsigned char bytRebateCode, unsigned char bytCostCode);
void GetNetSJTPrice(unsigned char bytNotPeakCode, unsigned char bytRebateCode, unsigned char bytCostCode);
void UpdatePrice();
void UpdateNetPrice();

void FreePara();

long BomOperator(unsigned long lngOperator, unsigned char *strOperatorPIN);
unsigned char binFileManage(unsigned char chcmd, unsigned char *cmd_buf, int cmd_len, unsigned char *out_buf, int *out_len);

//2019-06-05	SHA-1	
int check_sha1(char *inbuf);
int get_qr_map(unsigned char *os_pb_id, unsigned char *os_orgnization, unsigned char physical, unsigned char version, unsigned char *ticket_type);
int get_cert_pb(unsigned char family, unsigned char version, unsigned char index, struct Cert_Content *outbuf);


#endif
