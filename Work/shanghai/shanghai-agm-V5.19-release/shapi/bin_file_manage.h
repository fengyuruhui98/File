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

//4020����������		2019-06-04
struct White_Head
{
	unsigned long ParaVersion;		//�����汾	Long	4
	unsigned long ContentNumber;	//���ݼ�¼��	Long	4
	unsigned char SHA_value[20];			//��¼��У��ֵ	Byte	20	
}__attribute__( ( packed, aligned(1) ) );
typedef	struct White_Head	White_Head_t;

struct White_Content
{
//��Ʊ���Ͷ�Ӧ��¼
	unsigned char CardIssuer[8];			//����������ʶ	ȫ��ƥ�䣨ֱ�ӱȽϣ� Byte  8
	unsigned char IIN[4];							//6λ��iin+2λ��չ��0015�ļ���Ӧ�����кŵ�2-9λ��	ȫ��ƥ�䣨ֱ�ӱȽϣ� Byte  4
	unsigned char CardType;						//������	ס����������ʱ��Ϊ���汾	Byte	1
	unsigned char RFU[2];							//Ԥ��	Byte	2
	unsigned char TicketType;					//����Ʊ������	Byte	1
}__attribute__( ( packed, aligned(1) ) );
typedef struct White_Content	White_Content_t;

struct White_4020
{
	White_Head_t	white_head;	
	White_Content_t	*White_Content_val;
}__attribute__( ( packed, aligned(1) ) );
typedef	struct White_4020	White_4020_t;

//4026-��ͨ����ά���֤���ļ�
struct Cert_Head
{
	unsigned int 	ParaVersion;		//�����汾		Long	4
	unsigned int 	ContentNumber;		//���ݼ�¼��		Long	4
	unsigned char 	rfu[4];				//������		Byte	4
	unsigned char 	SHA_value[20];			//��¼��У��ֵ	��¼�����ݵ�SHA-1ֵ	Byte	20
}__attribute__( ( packed, aligned(1) ) );
typedef	struct Cert_Head	Cert_Head_t;

struct Cert_Content
{
//֤���Ӧ��¼	
	unsigned char 	type;				//��ά�����		Byte	1
	unsigned char 	ver;				//�汾��	0x80��0xFF	Byte	1
	unsigned char	head;				//��¼ͷ	ʮ������0x21	Byte	1
	unsigned char	service_flag[4];	//�����ʶ	��01010000�� = �衢����	Byte	4
	unsigned char 	rid[5];				//ע���Ӧ���ṩ�̱�ʶ (RID)	��ʶ RID��Ϊʮ������A000000632��	Byte	5
	unsigned char	index;				//�� CA ��Կ����	Ψһ��ʶ�� CA ��Կ	Byte	1
	unsigned char	valid_date[2];		//֤��ʧЧ����	�º���(MMYY)���ڸ������һ��֮��֤��ʧЧ	Byte	2
	unsigned char	al;					//�� CA ��Կ�㷨��ʶ	��ʶ����������Կǩ���㷨��04����SM2��16���ƣ�	Byte	1
	unsigned char	al_flag;			//�� CA ��Կ������ʶ	���ڱ�ʶ��Բ���߲�������00��	Byte	1
	unsigned char 	len;				//�� CA ��Կ����	��ʶ����������Կ���ֽڳ���	Byte	1
	unsigned char 	pb[64];				//�� CA ��Կ	���ֶ�����Բ�����ϵ�һ����	Byte	64
	unsigned char 	sign[64];			//����ǩ��	�� CA ��Կָ����	Byte	64
	unsigned char 	rfu[14];			//RFU	Ԥ��	Byte	14
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

//����ʹ�õĲ�������ͷ����
struct PublicTitle tpPubTitle1040, tpPubTitle2000, tpPubTitle3002, tpPubTitle3004, tpPubTitle3006, tpPubTitle3007;                    //
struct PublicTitle tpPubTitle3008, tpPubTitle3009, tpPubTitle3010, tpPubTitle3011, tpPubTitle3082, tpPubTitle3083;                    //
struct PublicTitle tpPubTitle3084, tpPubTitle3085, tpPubTitle4001, tpPubTitle4002, tpPubTitle4003, tpPubTitle4004;                    //
struct PublicTitle tpPubTitle4006, tpPubTitle4007, tpPubTitle4008, tpPubTitle4009, tpPubTitle4015;                    //
//�µĲ�������ͷ����
struct PublicTitle tpNewPubTitle1040, tpNewPubTitle2000, tpNewPubTitle3002, tpNewPubTitle3004, tpNewPubTitle3006;                 //
struct PublicTitle tpNewPubTitle3007, tpNewPubTitle3008, tpNewPubTitle3009, tpNewPubTitle3010, tpNewPubTitle3011;                 //
struct PublicTitle tpNewPubTitle3082, tpNewPubTitle3083, tpNewPubTitle3084, tpNewPubTitle3085, tpNewPubTitle4001;                 //
struct PublicTitle tpNewPubTitle4002, tpNewPubTitle4003, tpNewPubTitle4004, tpNewPubTitle4006, tpNewPubTitle4007;                 //
struct PublicTitle tpNewPubTitle4008, tpNewPubTitle4009, tpNewPubTitle4015;                 //
//
struct ARRAY_BOUND tpArraybound;
//ʹ���еĲ��������ݱ���
struct PARA_1040 tpStationSet1040;                    //��վ����
struct PARA_1040_SLE *tpSLESet1040sub;         		 //
struct PARA_2000 tpCommuPara2000;                     //��·�ڲ�ͨ�Ų���
struct PARA_3002 tpAFCOperationPara3002;              //AFC�豸���в���
struct PARA_3004 tpBOMOperationPara3004;              //BOM���в���
struct PARA_3006 *tpStationName3006;           		 //��վ����/��·���ñ�
struct PARA_3007 *tpLineName3007;                     //��·���Ʊ�
struct PARA_3008 *tpFailCode3008;                     //ϵͳ���ϴ���
struct PARA_3009 *tpOperator3009;                     //����Ա��
struct PARA_3009_LINE *tpOperator3009LINE;            //����Ա��
struct PARA_3010 *tpResource3010;                     //����������Դ�ļ�
struct PARA_3010 *tpResource3011;                     //���������Դ�ļ�
struct PARA_3082 *tpExchangeSta3082;                  //
struct PARA_3083 *tpMainInterface3083;
struct PARA_3084 *tpInterStation3084;
struct PARA_4001 *tpHoliday4001;                      //���ձ�
struct PARA_4002 tpTicketSet4002;                     //��Ʊ���ͱ�
struct PARA_4002_TICKET *tpTicketInfo4002sub;         //
struct PARA_4002_TICKET tpTicketDef;                  //
struct PARA_4003 *tpFareValue4003;                    //���ʱ�
unsigned short tpNetSJTPrice[64];						  //·������Ʊ��
struct PARA_4004 tpStationFare4004;                    //�����
struct PARA_4004_FARE *tpStationFare4004sub;         	//
unsigned char *tpFare;                                //����ķ�������
struct PARA_4006 *tpNotPeak4006;                      //�Ǹ߷�ʱ�α�
struct PARA_4007_GRP *tpTicketBlacklist4007Grp;       //��Ʊ������
struct PARA_4008_GRP *tpCSCBlacklist4008Grp;          //��Ʊ������
struct PARA_4009 *tpTicketMap4009;                    //
struct PARA_4015 *tpCMCCMap4015;  
//
struct PARA_6002	tpUL6002;
unsigned short 		para_type;						//��¼���׼�¼��Ϣ������
unsigned short 		position_6002;				//
//��ǰ��վ��Ϣ
unsigned short *tpSJTPrice;								 //����Ʊ��

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