#ifndef BIN_EOD
#define BIN_EOD

//Kunming Standard
//#pragma pack(1)
struct PublicTitle
{
    unsigned long lngTableVersion;         //
    unsigned long lngExpiredTime;          //有效的日期
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
    unsigned char bytStaID[4];             //SC节点标识码
    short int intStaPos;                   //
    unsigned char strStaNameAbbrEn[6];     //车站简称（英）
    unsigned char strStaNameEn[32];        //车站名称（英）
    unsigned long lngStaPointer;           //中文指针
    unsigned char bytCCID[4];              //中央节点标识码
    unsigned char TranRegPlan[7][4][2];    //维护寄存器计划
    unsigned char OperRegPlan[7][4][2];    //维护审计寄存器计划
    unsigned char bytstrStaNameEnRFU1;                 //
    short int intRFU;                      //
    unsigned char bytWorkTime[3];          //运营时间
    unsigned char bytRFU2;                 //
    //bytRFU3(191) As Byte                 //设备组定义保留
    struct PARA_1040_GROUP tpGroup[16];    //
};
//
struct PARA_1040_SLE
{
    long lngExpiredTime;                   //本记录生效时间
    unsigned char bytSLEID[4];             //设备标识码
    unsigned char bytSLEIP[4];             //设备IP
    short int intSLEPort;                  //服务端口
    unsigned char bytBelongTo;             //所属组别
    unsigned char bytSLEType;              //设备类型
};
//*********************************************
//*****************************************2000
struct PARA_2000
{
    unsigned char bytReWaiting;            //重发前等待时间
    unsigned char bytReNum;                //重发次数
    unsigned char bytWait;                 //等待建立连接时间
    unsigned char bytNoAnswer;				//
    unsigned char bytParaSyn[4];           //参数同步
    unsigned char bytStopOper[4];			//
    unsigned char bytParaWait;				//
    unsigned char bytOvertime;             //应答超时时间
    unsigned char bytStaIP[4];             //主服务器IP
    unsigned char bytBakIP[4];             //备服务器IP
    short int intPort1;                    //通信规程1端口
    short int intPort2;                    //通信规程2端口
    short int intPort3;                    //通信规程3端口
    short int intPort4;                    //通信规程4端口
}__attribute__( ( packed, aligned(1) ) );
//*********************************************
//****************************************3002
struct PARA_3002
{
    //
    unsigned char bytLoginTimeout;         //登录超时时间
    short int intLongoutTimeout;    //签退超时时间
    unsigned char bytOperateTimeout;       //操作等待时间
    unsigned char bytFullOperate;          //交易存储满控制
    unsigned char bytWarningDefer;         //报警时间
    unsigned char bytUpRecMinNum;          //提交交易数据笔数下限
    unsigned char bytUpRecTime;            //提交交易数据时间
    short int intBCU;               //金额基本单位
    unsigned char bytUpStatus;				//
    unsigned char bytRFU[9];               //
    //
    short int intRFU1;              //
    unsigned char bytRFU1;                 //
    unsigned char bytRFU2;                 //
    unsigned char bytRFU3;                 //
    unsigned char bytCoinBoxFulling;       //硬币回收箱将满百分比
    unsigned char bytCoinBoxFulled;        //硬币回收箱满百分比
    unsigned long lngCoinBoxNum;           //硬币回收箱容量
    short int intMinCoinNum;        //硬币找零箱最低报警数
    short int intMaxCoinNum;        //硬币找零箱最大容量
    unsigned char bytRFU4;                 //
    unsigned char bytRFU5;                 //
    //short int intRFU2;              //
    //unsigned char bytRFU6;                 //
    //
    unsigned char bytNoteFulling;          //纸币回收箱将满百分比
    unsigned char bytNoteFulled;           //纸币回收箱满百分比
    unsigned char bytNoteEmpting;          //纸币找零箱将空百分比
    unsigned char bytMaxNoteBoxNum;        //纸币找零箱最大容量
    unsigned char bytRFU7;                 //
    unsigned char bytRFU8;                 //
    short int intRFU3;              //
    unsigned char bytRFU9[7];              //
    //
    unsigned char bytRFU10;                //
    unsigned char bytRFU11;                //
    unsigned char bytTicketReReadWrite;    //车票连续读/写失败此数
    short int intRFU4;              //
    unsigned char bytRFU12;                //
    unsigned char bytRFU13;                //
    unsigned char bytRFU14;                //
    unsigned char bytRFU15;                //
    unsigned char bytRFU16[7];             //
    //
    unsigned char bytCSCReReadWrite;       //智能卡连续读/写次数
    unsigned char bytRFU17[15];            //
};
//*********************************************
//*****************************************3004
struct PARA_3004
{
    unsigned char bytCancelWait;           //交易超时时间
    short int intSLESetPara;        //设备配置参数
    unsigned char bytPeakTicket[32];       //高峰时拒绝的票类
    unsigned char bytNotpeakTicket[32];    //非高峰时拒绝的票类
    unsigned char bytMonitorSLE[32];       //监控设备
};
//*********************************************
//****************************************3006
struct PARA_3006
{
    unsigned char bytSCID[4];              //车站节点码
    char strStaNameEn[32];                //车站名称（英）
    unsigned long lngStaNamePointer;       //车站名称指针（中）
    //bytBelongtoLine(11) As Byte     //车站所属线路
};
//*********************************************
//****************************************3007
struct PARA_3007
{
    unsigned char bytCCID[4];              //中央节点码
    char strLineNameEn[32];                //中央名称（英）
    unsigned long lngLineNamePointer;      //中央名称指针（中）
};
//********************************************
//****************************************3008
struct PARA_3008
{
    short int intFailCode;          //事件代码
    char strFailNameEn[32];         //事件描述（英）
    unsigned long lngFailNamePointer;      //事件描述（中）
    unsigned char bytFailLevel;            //事件类别《级别》
    unsigned char bytRFU;                  //
};
//********************************************
//****************************************3009
struct PARA_3009
{
    unsigned long lngOperatorID;           //操作员编号
    unsigned char bytEnPIN[8];             //加密后密码
    unsigned char bytOperatorLevel;        //操作员类别
    unsigned char bytAccessDevice;         //可访问设备
    unsigned char bytRFU[32];              //
    long lngExpiredDate;          //过期日期
};
struct PARA_3009_LINE
{
    unsigned long lngOperatorID;           //操作员编号
    unsigned char bytEnPIN[8];             //加密后密码
    unsigned char bytOperatorLevel;        //操作员类别
    unsigned char bytAccessDevice;         //可访问设备
    unsigned char bytRFU[32];              //
    long lngExpiredDate;          //过期日期
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
    long lngStartHoliday;         //节日开始时间
};
//********************************************
//****************************************4002
struct PARA_4002
{
	unsigned char bytGatePriority;
	unsigned char bytBomPriority;
    unsigned char bytRFU1[10];             //
    unsigned char bytRFU2[23];             //
    unsigned char bytJoinTime;			   //联乘时间，以分为单位
    short int intLosePunish;               //丢票处罚
    short int intOverfarePunish;           //超乘处罚
    short int intOvertimePunish;           //超时处罚
    short int intUpdatePunish;             //更新处罚
    short int intStoreInitVal;             //储值票初始值
    unsigned char bytEffectivePeriod;      //有效时段
    unsigned char bytStayTime[64];         //逗留时间
}__attribute__( ( packed, aligned(1) ) );
//
struct PARA_4002_TICKET
{
    unsigned char bytTicketCode;           //票类型码
    char strTicketNameEn[15];              //车票名称（英）
    unsigned long lngTicketNamePointer;    //车票名称指针（中）
    short int intTicketControl;            //车票控制码
    short int intTicketVerify;             //车票验证指示
    short int intEffectivePeriod;          //有效期（天数）
    short int intSpecialPeriod;            //特殊验证期
    
    short int intInitVal;                  //初始票值
    short int intDeposit;                  //押金(购买值)
    short int intCSCInitVal;               //交通卡初始票值
    short int intCSCInitBonus;             //交通卡初始奖励
    
    short int intCSCAddSel[4];             //交通卡加值选择
    short int intCSCAddBonus[4];           //交通卡加值奖励
    //
    unsigned char bytNormalAreaIndex;		//
    unsigned char bytNormalExtend;           //
    unsigned char bytNormalNotpeakCode;     	//通常非高峰时段码（索引值）
    unsigned char bytNormalRebateCode;      	//折扣费率码
    unsigned char bytNormalCostCode;        	//全价费率码
    unsigned char bytWeekendAreaIndex;		//
    unsigned char bytWeekendExtend;         //
    unsigned char bytWeekendNotpeakCode;   	//周末
    unsigned char bytWeekendRebateCode;    	//
    unsigned char bytWeekendCostCode;      	//
    unsigned char bytHolidayAreaIndex;		//
    unsigned char bytHolidayExtend;         //
    unsigned char bytHolidayNotpeakCode;   	//假日
    unsigned char bytHolidayRebateCode;    	//
    unsigned char bytHolidayCostCode;      	//
    unsigned char bytNullAreaIndex;			//
    unsigned char bNullRFU;                	//
    unsigned char bNullNotpeakCode;        	//空的费率计划
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
    unsigned char bytMaxDayTimes;       	//每日最大乘次
    unsigned char bytRFU4;                 //
}__attribute__( ( packed, aligned(1) ) );
//********************************************
//****************************************4003
struct PARA_4003
{
    unsigned short intFareValue[64];          //票区1至64费率值
    unsigned short shRFU[32];              		//
};
//********************************************
//****************************************4005
struct PARA_4004
{
	unsigned short StationNum;
    unsigned char bytSCID[4];              //车站标识码
};
struct PARA_4004_FARE
{
    unsigned char bytExitSta[4];           //出站车站标识码
    unsigned char bytFare;                 //票区
};
//********************************************
//****************************************4006
struct PARA_4006
{
    unsigned char bytPeakArray[7][12];     //高峰/非高峰时段记录
};
//********************************************
//****************************************4007(原始定义)
struct PARA_4007
{
    unsigned char bytTicketType;           //车票类型
    unsigned long lngSerialNo;             //
    unsigned char bytRejecttype;           //拒绝类型
};
struct PARA_4007_GRP
{
    unsigned char varSerialNo[5];		  //票类与序列号组合
    unsigned char bytRejecttype;            //
};
//********************************************
//****************************************4008(原始定义)
struct PARA_4008
{
    unsigned char bytOperateType;          //操作码：决定是增加还是删除此条记录
    unsigned char bytTicketType;           //车票类型
    long lngSerialNo;                      //序列号
    unsigned char bytRejecttype;           //拒绝类型
};
struct PARA_4008_GRP
{
    unsigned char bytOperateType;          //
    unsigned char varSerialNo[5];          //票类与序列号组合
    unsigned char bytRejecttype;           //
};
//****************************************4009
struct PARA_4009
{
    unsigned char bytNewType;              //新票种类型
    unsigned char bytOldType;              //已有票种类型
};
//********************************************
//****************************************4015
struct PARA_4015
{
    //unsigned char bytCMCCType;             //新票种类型
    long lngCity;                 			 //
    unsigned char bytMapType;              	//已有票种类型
};
//********************************************
//********************************************
struct PARA_6002
{
    unsigned char bytTranType;             	//交易事件码
    unsigned char bytTicketType;           	//类型码
    unsigned char bytOldTicketID[8];       	//原始票标识
    unsigned char bytNewTicketID[8];       	//新票标识
    unsigned char bytOperatorID[4];        	//操作员编号
    unsigned char bytSAMID[4];            	//
    unsigned char bytTranTime[4];          	//交易日期时间
    unsigned char bytBalance[4];     		//交易前票值
    unsigned char bytTranAmount[2];        	//交易金额
    unsigned char bytCurStation[4];    		//当前车站计算机节点标识码（线路、车站、设备类型、编码）
    unsigned char bytLastStation[4];     	//上次使用车站（车站计算机）（线路、车站、0，0）
    unsigned char bytStayJourney;          	//剩余乘次
    unsigned char bytCurDevice[4];         	//
    unsigned char bytTranTraceNo[4];       	//终端流水号（交易）
    unsigned char bytTranCount[2];         	//票计数器
    unsigned char bytIssueDevice[3];        //line station deviceid
    unsigned char bytBonus[2];				//
    unsigned char bytTest;					//
    unsigned char bytRFU[5];               	//保留
    unsigned char bytTAC[4];               	//TAC
};
struct PARA_6003
{
    unsigned char bytTranType;             	//交易事件码
    unsigned char bytCardType;             	//车票类型码
    unsigned char bytCardID[4];        		//原始卡标识
    unsigned char bytAppSN[10];         	//新卡标识
    unsigned char bytOperatorID[4];        	//操作员编号
    unsigned char bytSAMID[6];             	//SAM卡号
    unsigned char bytTranTime[4];          	//交易日期时间
    unsigned char bytBalance[4];       		//交易前票值
    unsigned char bytTranAmount[2];        	//交易金额
    unsigned char bytCurStation[4];      	//当前车站设备（节点标识码）
    unsigned char bytLastStation[4];     	//上次使用车站
    unsigned char bytCurDevice[4];          //
    unsigned char bytTranTraceNo[4];       	//交易流水号
    unsigned char bytTranCount[2];         	//卡计数器
    unsigned char bytCity[2];              	//城市代码
    unsigned char bytAlgorithm;     		//算法
    unsigned char bytVersion;				//
    unsigned char bytDeposit[2];           	//押金
    unsigned char bytPreferential[2];      	//优惠值
    unsigned char bytTranFlag;             	//交易标志
    unsigned char bytAddedAmount[3];       	//保留
    unsigned char bytRFU[5];				//
    unsigned char bytTAC[4];               	//TAC
};
//新参数表更新格式
//
struct NewPublicTitle
{
    short int intParaType;          //参数表消息码
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
struct NewPublicTitle tpNewPubTitle[40];	          //新参数表表头

#endif