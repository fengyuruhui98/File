#ifndef BIN_EOD
#define BIN_EOD

//Kunming Standard
//#pragma pack(1)
struct PublicTitle
{
    unsigned long lngTableVersion;         //
    unsigned long lngExpiredTime;          //��Ч������
};
//**************************************1040
//
struct PARA_1040_GROUP
{
    unsigned char bytGroupType;            //
    char strGroupNameEn[7];	               //
    unsigned long lngGroupPointer;         //
};
struct PARA_1040
{
    unsigned char bytStaID[4];             //SC�ڵ��ʶ��
    short int intStaPos;                   //
    unsigned char strStaNameAbbrEn[6];     //��վ��ƣ�Ӣ��
    unsigned char strStaNameEn[32];        //��վ���ƣ�Ӣ��
    unsigned long lngStaPointer;           //����ָ��
    unsigned char bytCCID[4];              //����ڵ��ʶ��
    unsigned char TranRegPlan[7][4][2];    //ά���Ĵ����ƻ�
    unsigned char OperRegPlan[7][4][2];    //ά����ƼĴ����ƻ�
    unsigned char bytstrStaNameEnRFU1;                 //
    short int intRFU;                      //
    unsigned char bytWorkTime[3];          //��Ӫʱ��
    unsigned char bytRFU2;                 //
    //bytRFU3(191) As Byte                 //�豸�鶨�屣��
    struct PARA_1040_GROUP tpGroup[16];    //
};
//
struct PARA_1040_SLE
{
    long lngExpiredTime;                   //����¼��Чʱ��
    unsigned char bytSLEID[4];             //�豸��ʶ��
    unsigned char bytSLEIP[4];             //�豸IP
    short int intSLEPort;                  //����˿�
    unsigned char bytBelongTo;             //�������
    unsigned char bytSLEType;              //�豸����
};
//*********************************************
//*****************************************2000
struct PARA_2000
{
    unsigned char bytReWaiting;            //�ط�ǰ�ȴ�ʱ��
    unsigned char bytReNum;                //�ط�����
    unsigned char bytWait;                 //�ȴ���������ʱ��
    unsigned char bytNoAnswer;				//
    unsigned char bytParaSyn[4];           //����ͬ��
    unsigned char bytStopOper[4];			//
    unsigned char bytParaWait;				//
    unsigned char bytOvertime;             //Ӧ��ʱʱ��
    unsigned char bytStaIP[4];             //��������IP
    unsigned char bytBakIP[4];             //��������IP
    short int intPort1;                    //ͨ�Ź��1�˿�
    short int intPort2;                    //ͨ�Ź��2�˿�
    short int intPort3;                    //ͨ�Ź��3�˿�
    short int intPort4;                    //ͨ�Ź��4�˿�
}__attribute__( ( packed, aligned(1) ) );
//*********************************************
//****************************************3002
struct PARA_3002
{
    //
    unsigned char bytLoginTimeout;         //��¼��ʱʱ��
    short int intLongoutTimeout;    //ǩ�˳�ʱʱ��
    unsigned char bytOperateTimeout;       //�����ȴ�ʱ��
    unsigned char bytFullOperate;          //���״洢������
    unsigned char bytWarningDefer;         //����ʱ��
    unsigned char bytUpRecMinNum;          //�ύ�������ݱ�������
    unsigned char bytUpRecTime;            //�ύ��������ʱ��
    short int intBCU;               //��������λ
    unsigned char bytUpStatus;				//
    unsigned char bytRFU[9];               //
    //
    short int intRFU1;              //
    unsigned char bytRFU1;                 //
    unsigned char bytRFU2;                 //
    unsigned char bytRFU3;                 //
    unsigned char bytCoinBoxFulling;       //Ӳ�һ����佫���ٷֱ�
    unsigned char bytCoinBoxFulled;        //Ӳ�һ��������ٷֱ�
    unsigned long lngCoinBoxNum;           //Ӳ�һ���������
    short int intMinCoinNum;        //Ӳ����������ͱ�����
    short int intMaxCoinNum;        //Ӳ���������������
    unsigned char bytRFU4;                 //
    unsigned char bytRFU5;                 //
    //short int intRFU2;              //
    //unsigned char bytRFU6;                 //
    //
    unsigned char bytNoteFulling;          //ֽ�һ����佫���ٷֱ�
    unsigned char bytNoteFulled;           //ֽ�һ��������ٷֱ�
    unsigned char bytNoteEmpting;          //ֽ�������佫�հٷֱ�
    unsigned char bytMaxNoteBoxNum;        //ֽ���������������
    unsigned char bytRFU7;                 //
    unsigned char bytRFU8;                 //
    short int intRFU3;              //
    unsigned char bytRFU9[7];              //
    //
    unsigned char bytRFU10;                //
    unsigned char bytRFU11;                //
    unsigned char bytTicketReReadWrite;    //��Ʊ������/дʧ�ܴ���
    short int intRFU4;              //
    unsigned char bytRFU12;                //
    unsigned char bytRFU13;                //
    unsigned char bytRFU14;                //
    unsigned char bytRFU15;                //
    unsigned char bytRFU16[7];             //
    //
    unsigned char bytCSCReReadWrite;       //���ܿ�������/д����
    unsigned char bytRFU17[15];            //
};
//*********************************************
//*****************************************3004
struct PARA_3004
{
    unsigned char bytCancelWait;           //���׳�ʱʱ��
    short int intSLESetPara;        //�豸���ò���
    unsigned char bytPeakTicket[32];       //�߷�ʱ�ܾ���Ʊ��
    unsigned char bytNotpeakTicket[32];    //�Ǹ߷�ʱ�ܾ���Ʊ��
    unsigned char bytMonitorSLE[32];       //����豸
};
//*********************************************
//****************************************3006
struct PARA_3006
{
    unsigned char bytSCID[4];              //��վ�ڵ���
    char strStaNameEn[32];                //��վ���ƣ�Ӣ��
    unsigned long lngStaNamePointer;       //��վ����ָ�루�У�
    //bytBelongtoLine(11) As Byte     //��վ������·
};
//*********************************************
//****************************************3007
struct PARA_3007
{
    unsigned char bytCCID[4];              //����ڵ���
    char strLineNameEn[32];                //�������ƣ�Ӣ��
    unsigned long lngLineNamePointer;      //��������ָ�루�У�
};
//********************************************
//****************************************3008
struct PARA_3008
{
    short int intFailCode;          //�¼�����
    char strFailNameEn[32];         //�¼�������Ӣ��
    unsigned long lngFailNamePointer;      //�¼��������У�
    unsigned char bytFailLevel;            //�¼���𡶼���
    unsigned char bytRFU;                  //
};
//********************************************
//****************************************3009
struct PARA_3009
{
    unsigned long lngOperatorID;           //����Ա���
    unsigned char bytEnPIN[8];             //���ܺ�����
    unsigned char bytOperatorLevel;        //����Ա���
    unsigned char bytAccessDevice;         //�ɷ����豸
    unsigned char bytRFU[32];              //
    long lngExpiredDate;          //��������
};
struct PARA_3009_LINE
{
    unsigned long lngOperatorID;           //����Ա���
    unsigned char bytEnPIN[8];             //���ܺ�����
    unsigned char bytOperatorLevel;        //����Ա���
    unsigned char bytAccessDevice;         //�ɷ����豸
    unsigned char bytRFU[32];              //
    long lngExpiredDate;          //��������
};
//********************************************
//****************************************3010
struct PARA_3010
{
    char strResource[16];      //
};
//********************************************
//****************************************3082
struct PARA_3082
{
    unsigned char bytStation[6][4];         //
};
//*******************************************
struct PARA_3083_ROUTE
{
	unsigned char bytLine;
	char bytRFU[4];
};
struct PARA_3083
{
	unsigned char bytIndex;
	char strMainFileName[24];
	unsigned short intMainWidth;
	unsigned short intMainHeight;
	unsigned short intLangX;
	unsigned short intLangY;
	unsigned char bytLangWidht;
	unsigned char bytLangHeight;
	
	unsigned char bytDir1Valid;
	unsigned short intDir1X;
	unsigned short intDir1Y;
	unsigned short intDir1Width;
	unsigned short intDir1Height;
	unsigned char bytdir1Index;
	
	unsigned char bytDir2Valid;
	unsigned short intDir2X;
	unsigned short intDir2Y;
	unsigned short intDir2Width;
	unsigned short intDir2Height;
	unsigned char bytDir2Index;

	unsigned char bytDir3Valid;
	unsigned short intDir3X;
	unsigned short intDir3Y;
	unsigned short intDir3Width;
	unsigned short intDir3Height;
	unsigned char bytDir3Index;

	unsigned char bytDir4Valid;
	unsigned short intDir4X;
	unsigned short intDir4Y;
	unsigned short intDir4Width;
	unsigned short intDir4Height;
	unsigned char bytDir4Index;
	
	unsigned char bytRouteNum;
	char bytRFU[4];
	
	struct PARA_3083_ROUTE tpRoute[16];
};
struct PARA_3084_STATION
{
	unsigned char bytStationID;
	unsigned short intX;
	unsigned short intY;
	unsigned char bytLength;
	unsigned char bytHeight;
	char bytRFU;
};
struct PARA_3084
{
	unsigned char bytLineID;
	char chStationFile[24];
	unsigned char bytStationNum;
	char bytRFU[4];
	struct PARA_3084_STATION tpStationRec[64];
};
//*****************************************
struct PARA_3085
{
    unsigned char bytStation1[6][4];         //
};
//********************************************
//****************************************4001
struct PARA_4001
{
    long lngStartHoliday;         //���տ�ʼʱ��
};
//********************************************
//****************************************4002
struct PARA_4002
{
	unsigned char bytGatePriority;
	unsigned char bytBomPriority;
    unsigned char bytRFU1[10];             //
    unsigned char bytRFU2[23];             //
    unsigned char bytJoinTime;			   //����ʱ�䣬�Է�Ϊ��λ
    short int intLosePunish;               //��Ʊ����
    short int intOverfarePunish;           //���˴���
    short int intOvertimePunish;           //��ʱ����
    short int intUpdatePunish;             //���´���
    short int intStoreInitVal;             //��ֵƱ��ʼֵ
    unsigned char bytEffectivePeriod;      //��Чʱ��
    unsigned char bytStayTime[64];         //����ʱ��
}__attribute__( ( packed, aligned(1) ) );
//
struct PARA_4002_TICKET
{
    unsigned char bytTicketCode;           //Ʊ������
    char strTicketNameEn[15];              //��Ʊ���ƣ�Ӣ��
    unsigned long lngTicketNamePointer;    //��Ʊ����ָ�루�У�
    short int intTicketControl;            //��Ʊ������
    short int intTicketVerify;             //��Ʊ��ָ֤ʾ
    short int intEffectivePeriod;          //��Ч�ڣ�������
    short int intSpecialPeriod;            //������֤��
    
    short int intInitVal;                  //��ʼƱֵ
    short int intDeposit;                  //Ѻ��(����ֵ)
    short int intCSCInitVal;               //��ͨ����ʼƱֵ
    short int intCSCInitBonus;             //��ͨ����ʼ����
    
    short int intCSCAddSel[4];             //��ͨ����ֵѡ��
    short int intCSCAddBonus[4];           //��ͨ����ֵ����
    //
    unsigned char bytNormalAreaIndex;		//
    unsigned char bytNormalExtend;           //
    unsigned char bytNormalNotpeakCode;     	//ͨ���Ǹ߷�ʱ���루����ֵ��
    unsigned char bytNormalRebateCode;      	//�ۿ۷�����
    unsigned char bytNormalCostCode;        	//ȫ�۷�����
    unsigned char bytWeekendAreaIndex;		//
    unsigned char bytWeekendExtend;         //
    unsigned char bytWeekendNotpeakCode;   	//��ĩ
    unsigned char bytWeekendRebateCode;    	//
    unsigned char bytWeekendCostCode;      	//
    unsigned char bytHolidayAreaIndex;		//
    unsigned char bytHolidayExtend;         //
    unsigned char bytHolidayNotpeakCode;   	//����
    unsigned char bytHolidayRebateCode;    	//
    unsigned char bytHolidayCostCode;      	//
    unsigned char bytNullAreaIndex;			//
    unsigned char bNullRFU;                	//
    unsigned char bNullNotpeakCode;        	//�յķ��ʼƻ�
    unsigned char bNullRebateCode;         	//
    unsigned char bNullCostCode;           	//
    //
    short int intAddedBonusTimes;
    unsigned long lngAddedBonusAmount;     	//
    short int intAddedBonusPercent;        	//
    short int intLianchAmount;  			//
    short int intLianchPercent; 			//
    unsigned char bytBonusPriority;				
    unsigned char bytLianchEffectime;
    unsigned char bytLianchTrade;       	//
    unsigned char bytSpecialMap;        	//
    unsigned char bytTimesPer;
    short int intOvertimePunish;
    short int intExitPunish;
    unsigned char bytMaxBalance[3];        //card max balance
    unsigned long lngMonthAmount;			//
    
    unsigned char bytRFU1[4];        		//
    
    unsigned char bytRFU2;                 //
    unsigned char bytRFU3;                 //
    unsigned char bytMaxMonthTimes;			//
    unsigned char bytMaxDayTimes;       	//ÿ�����˴�
    unsigned char bytRFU4;                 //
}__attribute__( ( packed, aligned(1) ) );
//********************************************
//****************************************4003
struct PARA_4003
{
    unsigned short intFareValue[64];          //Ʊ��1��64����ֵ
    unsigned short shRFU[32];              		//
};
//********************************************
//****************************************4005
struct PARA_4004
{
	unsigned short StationNum;
    unsigned char bytSCID[4];              //��վ��ʶ��
};
struct PARA_4004_FARE
{
    unsigned char bytExitSta[4];           //��վ��վ��ʶ��
    unsigned char bytFare;                 //Ʊ��
};
//********************************************
//****************************************4006
struct PARA_4006
{
    unsigned char bytPeakArray[7][12];     //�߷�/�Ǹ߷�ʱ�μ�¼
};
//********************************************
//****************************************4007(ԭʼ����)
struct PARA_4007
{
    unsigned char bytTicketType;           //��Ʊ����
    unsigned long lngSerialNo;             //
    unsigned char bytRejecttype;           //�ܾ�����
};
struct PARA_4007_GRP
{
    unsigned char varSerialNo[5];		  //Ʊ�������к����
    unsigned char bytRejecttype;            //
};
//********************************************
//****************************************4008(ԭʼ����)
struct PARA_4008
{
    unsigned char bytOperateType;          //�����룺���������ӻ���ɾ��������¼
    unsigned char bytTicketType;           //��Ʊ����
    long lngSerialNo;                      //���к�
    unsigned char bytRejecttype;           //�ܾ�����
};
struct PARA_4008_GRP
{
    unsigned char bytOperateType;          //
    unsigned char varSerialNo[5];          //Ʊ�������к����
    unsigned char bytRejecttype;           //
};
//****************************************4009
struct PARA_4009
{
    unsigned char bytNewType;              //��Ʊ������
    unsigned char bytOldType;              //����Ʊ������
};
//********************************************
//****************************************4015
struct PARA_4015
{
    //unsigned char bytCMCCType;             //��Ʊ������
    long lngCity;                 			 //
    unsigned char bytMapType;              	//����Ʊ������
};
//********************************************
//********************************************
struct PARA_6002
{
    unsigned char bytTranType;             	//�����¼���
    unsigned char bytTicketType;           	//������
    unsigned char bytOldTicketID[8];       	//ԭʼƱ��ʶ
    unsigned char bytNewTicketID[8];       	//��Ʊ��ʶ
    unsigned char bytOperatorID[4];        	//����Ա���
    unsigned char bytSAMID[4];            	//
    unsigned char bytTranTime[4];          	//��������ʱ��
    unsigned char bytBalance[4];     		//����ǰƱֵ
    unsigned char bytTranAmount[2];        	//���׽��
    unsigned char bytCurStation[4];    		//��ǰ��վ������ڵ��ʶ�루��·����վ���豸���͡����룩
    unsigned char bytLastStation[4];     	//�ϴ�ʹ�ó�վ����վ�����������·����վ��0��0��
    unsigned char bytStayJourney;          	//ʣ��˴�
    unsigned char bytCurDevice[4];         	//
    unsigned char bytTranTraceNo[4];       	//�ն���ˮ�ţ����ף�
    unsigned char bytTranCount[2];         	//Ʊ������
    unsigned char bytIssueDevice[3];        //line station deviceid
    unsigned char bytBonus[2];				//
    unsigned char bytTest;					//
    unsigned char bytRFU[5];               	//����
    unsigned char bytTAC[4];               	//TAC
};
struct PARA_6003
{
    unsigned char bytTranType;             	//�����¼���
    unsigned char bytCardType;             	//��Ʊ������
    unsigned char bytCardID[4];        		//ԭʼ����ʶ
    unsigned char bytAppSN[10];         	//�¿���ʶ
    unsigned char bytOperatorID[4];        	//����Ա���
    unsigned char bytSAMID[6];             	//SAM����
    unsigned char bytTranTime[4];          	//��������ʱ��
    unsigned char bytBalance[4];       		//����ǰƱֵ
    unsigned char bytTranAmount[2];        	//���׽��
    unsigned char bytCurStation[4];      	//��ǰ��վ�豸���ڵ��ʶ�룩
    unsigned char bytLastStation[4];     	//�ϴ�ʹ�ó�վ
    unsigned char bytCurDevice[4];          //
    unsigned char bytTranTraceNo[4];       	//������ˮ��
    unsigned char bytTranCount[2];         	//��������
    unsigned char bytCity[2];              	//���д���
    unsigned char bytAlgorithm;     		//�㷨
    unsigned char bytVersion;				//
    unsigned char bytDeposit[2];           	//Ѻ��
    unsigned char bytPreferential[2];      	//�Ż�ֵ
    unsigned char bytTranFlag;             	//���ױ�־
    unsigned char bytAddedAmount[3];       	//����
    unsigned char bytRFU[5];				//
    unsigned char bytTAC[4];               	//TAC
};
//�²�������¸�ʽ
//
struct NewPublicTitle
{
    short int intParaType;          //��������Ϣ��
    long NewParaTime;               //
    unsigned char blnUpdate;        //
};
struct ARRAY_BOUND
{
	long tpSLESet1040sub;
	long tpStationName3006;
	long tpLineName3007;
	long tpFailCode3008;
	long tpOperator3009;
	long tpResource3010;
	long tpResource3011;
	long tpExchangeSta3082;
	long tpMainInterface3083;
	long tpInterStation3084;
	long tpExchangeSta3085;
	long tpHoliday4001;
	long tpTicketInfo4002sub;
	long tpFareValue4003;
	long tpStationFare4004sub;
	long tpFare;
	long tpNotPeak4006;
	long tpTicketBlacklist4007Grp;
	long tpCSCBlacklist4008Grp;
	long tpTicketMap4009;
	long tpCMCCMap4015;
	long tpNewPubTitle;
	long lngWaitNo;
	long tpSJTPrice;
	long tpNetSJTPrice;
};
//#pragma pack()

//
struct NewPublicTitle tpNewPubTitle[40];	          //�²������ͷ

#endif