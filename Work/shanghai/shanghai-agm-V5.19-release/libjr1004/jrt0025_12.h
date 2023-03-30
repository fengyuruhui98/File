//*************************************************************************************
//jrt0025_12.c
//实现pboc2.0 jr/t0025 
//谷文龙 20111128
//*************************************************************************************
#ifndef _JRT002512_H_
#define _JRT002512_H_

	#define UBYTE unsigned char 
	#define UWORD  unsigned short
	#define UDWORD unsigned long
	typedef struct
  	{
  		unsigned char data[512] ; 
  		unsigned int length ; 

	} T_APDU ;


  	#define OK 0
	#define STATUS_PROTOCOL_ERROR           (0x000B)	
		
	#define VSM_OK							0x00
	/* parameter integrity */
	#define VSM_ERR_DATALEN					0x05
	#define VSM_ERR_NOTSUPPORTED			0x06
	#define VSM_ERR_PARAM_NOT_SUPPORT       0x07

	/* RSA Error*/
	#define VSM_ERR_RSA						0x0B
	#define VSM_ERR_RSAKEY_INCONS_SIZE      0x0C 
	#define VSM_ERR_RSAKEY_INCONS_BITDIFF   0x0D
	#define VSM_ERR_RSAKEY_INCONS_E			0x0D


	//define RSA compute padding
	#define VSM_RSA_PAD_NONE	1
	#define VSM_RSA_PAD_RND		2
	#define VSM_RSA_PAD_80		3
	//define RSA compute use PInV or QInV
	#define VSM_RSA_WITH_PINV	1
	#define VSM_RSA_WITH_QINV	2

	#define LENSINGLEAID	16
	#define TAGNUMBER			30//20130617
	
	#define TAGNUMBER_EXT			30

	#define EEPROMADDRESS	4096
	#define EEPMECOFFSET	1024
	
	
	#define ERR_g9f4a				-201
	#define ERR_HASH_ISSUE	-202
	#define ERR_HASH_PICC	-203
	#define ERR_HASH_DD	 	-204
	#define ERR_g5f24			-205
	#define ERR_NOT6A			-206
	#define ERR_DATAFORMAT			-207
	#define ERR_CVR			-208
	#define ERR_LEN9f10 -209
	#define ERR_CHKPARAM -210
	#define ERR_NOPDOL		-212
	#define ERR_NOCA 		-214
	#define ERR_NOCAIDX	-215
	#define ERR_NOTAG90	-216
	#define ERR_NOTAG8f	-217
	#define ERR_NOTAG9f66	-218
	#define ERR_9f02E0	-219    
	#define ERR_PDOL0	-220    
	#define ERR_BLKCARD	-221		
	#define ERR_NOPAN		-222		
	#define ERR_9f32		-223		
	#define ERR_9f46		-224		
	#define ERR_9f47		-225
	#define ERR_9f4b		-226
	#define ERR_NOPATCHPAN		-227
	#define ERR_HASHID		-228
	#define ERR_ISSUEID		-229
	#define ERR_9f4b_EXT	-230
	#define ERR_NOTBC			-231
	#define ERR_DBL5F24 	-232
	#define ERR_DBL57 	-233
	#define ERR_DBL5a 	-234
	#define ERR_LENBYTE 	-235
	#define ERR_MODEREC 	-236
	#define ERR_AUTHRECNO 	-237
	#define ERR_LASTRECNO 	-238
	#define ERR_SFI31 	-239
	#define ERR_SFI0 		-240
	#define ERR_FIRSTRECNO 		-241
	#define ERR_NO94 		-242

	#define ERR_NOAIDPATCH 		-246
	#define ERR_GPOLEN 		-247
	#define ERR_GPOMODE 		-248
	#define ERR_NO9f66 		-249
	#define ERR_FDDA00_9F08		-280
	#define ERR_FDDA01_9F6C		-281
	#define ERR_FDDA01_9F61		-282
	#define ERR_RMAC			-283
	#define ERR_PAGRMAC		-284

	#define ERR_NO82		-213
	#define ERR_NOAID			-211
	#define ERR_NO9f10 		-243
	#define ERR_NO9f26 		-244
	#define ERR_NO9f36 		-245
	#define ERR_NOdf61 		-250
	#define ERR_TRIPLEFLAG0121 		-251
	#define ERR_TRIPLEFLAG0221 		-252
	#define ERR_TRIPLEFLAG0321 		-253	
	
	#define ERR_CALL_BUSITYPE 		-301	
	#define ERR_MEC_APPLOCK 			-302
	#define ERR_PASS_CARD 			-308	//闪卡
	#define ERR_PROOV_TRANSNUM			-310	
	#define ERR_PROOV_APPSN			-311
	#define ERR_GTPHEADISNULL		-312
	#define ERR_GTPRECORDISNULL		-313
	#define ERR_GTPROOL_EEPRDERR		-314


	#define MAXNOCARDRETCODE	1000	

	typedef struct
	{
		UBYTE uTag[4] ;		 	//tag value , HEX
		UBYTE uParent[4] ; 	// this tag's Father
		char  sDescribe[32];//描述
		UBYTE		min_Len;			//最小长度
		UBYTE		max_Len;			//最大长度
		char	sCondition[8]; 	// M  O  C C*		
		UBYTE	uLength;
		UBYTE uValue[128]; 
	} T_TAG ;
	T_TAG tag_PPSE[8];
	
	typedef struct
	{
		UBYTE	uLength;
		UBYTE uName[2]; 
	} T_TAG_Data ;
	T_TAG_Data Tag_All[TAGNUMBER_EXT];
	
	typedef struct
	{
		UBYTE ATC[2]; 	
		UBYTE t9f02[6]; 
		UBYTE t9f21[3];	
		UBYTE t9a[3];	
		
		UBYTE t57[19];	
		UBYTE t9f10[33];
		UBYTE t5a[10];	
		UBYTE t5f24[3];	
		
		UBYTE t9f37[4];	
		UBYTE t82[2];	
		UBYTE t9f26[8];	
		UBYTE	t9F41[4];	

		
		UBYTE t9f66[4];	
		UBYTE t5f2a[2];	
		UBYTE t9f1e[8];	
		UBYTE t9f15[2];	
		
		UBYTE t9f39;	
		UBYTE t9f33[3];	
		UBYTE t9f1a[2];	
		UBYTE t9f1c[8];	
		UBYTE RFU[1];	
	} R_TransData ;
	R_TransData* g_TransData;


	unsigned int gPtr_Record;
	UBYTE gUID[16];
	UBYTE g9f66_PCD[4]; //终端交易属性  28 00 00 00 
	UBYTE g9f5d[6];     //可用脱机消费金额
	UBYTE uLen_g9f5d;
	UBYTE ugRand[4];    //9F3704 不可预知数
	UBYTE ugRand_RMAC[8];    //随机数 20130827
	UBYTE g9f02[6];  		// 授权金额
	UBYTE g9f02_BAK[6];  		// 授权金额
	UBYTE g9f03[6];  		// 其他金额
	UBYTE g9f1a[2];  		// 9F1A02 终端国家代码
	UBYTE g95[5];				//终端验证结果（TVR
	UBYTE g5f2a[2]; 		//5F2A02 交易货币代码
	UBYTE g9a[3];				//9A03  交易日期
	UBYTE g9c;			//01 交易类型
	UBYTE g90[256];	//发卡行公钥证书
	UBYTE uLen_g90;
	UBYTE g92[256];	//发卡行公钥证书余项
	UBYTE uLen_g92;
	
	UBYTE g9f32[4];	//发卡行公钥指数
	UBYTE uLen_g9f32=0;
	
	
	UBYTE g9f46[256];	//持卡人公钥证书
	UBYTE uLen_g9f46=0;
	
	UBYTE g9f47[4];	//持卡人公钥指数
	UBYTE uLen_g9f47=0;
	
	UBYTE g9f48[256];	//9F48 持卡人公钥余项
	UBYTE uLen_g9f48;
	
	UBYTE gSDFORAUTH[256];
	UBYTE uLen_gSDFORAUTH;
		
	UBYTE g9f4a[64];	
	UBYTE uLen_g9f4a;

	UBYTE g82[2];		
	UBYTE uLen_g82;	
	UBYTE g9f49[252];	
	UBYTE uLen_g9f49;
	UBYTE g5f24[3];		
	UBYTE g5f25[3];		
	UBYTE g5f30[2];		
	
	UBYTE g9f4b[256];	
	UBYTE uLen_g9f4b;

	UBYTE g9f10[32];	
	UBYTE uLen_g9f10;
	UBYTE gCVR[4];		
	UBYTE g9f6c[2];
	UBYTE uLen_g9f6c;
	UBYTE	g9f01[6];
	UBYTE	g9f40[5];
	UBYTE	g9f06[16];
	UBYTE	g9f34[3];
	UBYTE	g9f1e[9];
	UBYTE	g9f15[2];
	UBYTE	g9f16[16];
	UBYTE	g9f33[3];
	UBYTE	g9f1b[4];
	UBYTE	g9f1c[9];
	UBYTE	g9f1d[15];
	UBYTE g9f35;
	UBYTE	g9f3c[2];
	UBYTE	g9f3d;
	UBYTE	g9f41[4];
	unsigned int gCounter; 
	UBYTE	g9b[2];
	UBYTE	g9f21[3];
	UBYTE	g9f06[LENSINGLEAID];
	UBYTE uLen_g9f06;
	UBYTE	g9f34[3]; 
	UBYTE	g9f39=0x07;
	UBYTE g9f4e[21]; 

	UBYTE g81[4];  
	UBYTE g9f3a[4];
	
	UBYTE g9f04[4];
	UBYTE g9f26[8];
	UBYTE uLen_g9f26;
	UBYTE g5f36=0x02;
	UBYTE g57[19];
	UBYTE uLen_g57;
	UBYTE uLen_g57_gpo;
	UBYTE gbf0c[222];
	UBYTE uLen_gbf0c;
	UBYTE gATC[2];
	UBYTE uLen_gATC;
	UBYTE g9f36[2];
	UBYTE uLen_g9f36;
	UBYTE g8f;
	UBYTE uLen_g8f=0;
	UBYTE uLen_g5a;
	UBYTE g5a[10];	
	
	
	int  gMaxM=2000;
	int  gMaxM_Online=1000;	//单位：分，需要联机
	//---------以下 20120214 增加
	UBYTE uLen_g9f38;
	UBYTE g9f38[255];	
	
	UBYTE uLen_g84;
	UBYTE g84[64];	
	
	UBYTE uLen_g94;	
	UBYTE g94[255];	

	UBYTE uLen_g93;//20130516	
	UBYTE g93[255];	
	
	UBYTE uLen_ga5;
	UBYTE ga5[255];	
	UBYTE gdf1a[16];
	UBYTE uLen_g50;
	UBYTE g50[16];	
	UBYTE g87;	
	UBYTE uLen_g87;	
	UBYTE uLen_g5f2d;
	UBYTE g5f2d[8];	
	UBYTE g9f11;	
	UBYTE uLen_g9f12;
	UBYTE g9f12[16];
	UBYTE g6f[256];	
	UBYTE uLen_g6f;
	UBYTE uTagRepeat_5a=0x00;
	UBYTE uTagRepeat_57=0x00;
	UBYTE uTagRepeat_5f24=0x00;
	UBYTE RID_CAidx[21][5+1] = 
	 {{0xA0,0x00,0x00,0x03,0x33,0x80},
	 	
		{0xA0,0x00,0x00,0x03,0x03,0x94},
		{0xA0,0x00,0x00,0x00,0x03,0x96},
		{0xA0,0x00,0x00,0x00,0x03,0x97},
		{0xA0,0x00,0x00,0x00,0x03,0x50},
		{0xA0,0x00,0x00,0x00,0x03,0x51},
		{0xA0,0x00,0x00,0x00,0x03,0x53},
		
		{0xA0,0x00,0x00,0x99,0x99,0xE1},
		{0xA0,0x00,0x00,0x99,0x99,0xE2},
		{0xA0,0x00,0x00,0x99,0x99,0xE3},
		{0xA0,0x00,0x00,0x99,0x99,0xE4},
		{0xA0,0x00,0x00,0x99,0x99,0xE5},
		{0xA0,0x00,0x00,0x99,0x99,0xE6},
		
		{0xA0,0x00,0x00,0x00,0x04,0xFE},
		{0xA0,0x00,0x00,0x00,0x04,0xFC},
		{0xA0,0x00,0x00,0x00,0x04,0xFB},
		{0xA0,0x00,0x00,0x00,0x04,0xFD},
		{0xA0,0x00,0x00,0x00,0x04,0xFA},
		{0xA0,0x00,0x00,0x00,0x04,0xFF},
		
		{0xA0,0x00,0x00,0x00,0x65,0x02},
		{0xA0,0x00,0x00,0x00,0x65,0x03}
	};
	
	
	//20130504 谷文龙
	UBYTE gdf60;//CAPP交易指示位,0：表示终端不支持扩展应用 1：表示支持分段扣费交易 2：表示支持脱机预授权交易 3：表示支持脱机预授权完成交易
	UBYTE gdf61;//分段扣费应用标识 (在文件控制信息（FCI）中发卡行自定义数据BF0C中返回) ,1：表示卡片仅支持分段扣费应用 2：表示卡片支持扩展应用
							//如果DF6101 不等于0x01或0x02、0x81、0x82，交易拒绝。如果DF6101 bit8为1表示支持R-MAC。
	
	//20130522
	UBYTE gBalance[4]; 
	UBYTE g9f79[6];//当前余额:电子现金余额（EC Balance）
	UBYTE gdf62[6];//透支限额
	UBYTE gdf63[6];//已透支额
	
	int gn9f79=0;
	int gndf62=0;
	int gndf63=0;
	
	UBYTE gCardBusinessType=0x00;//0-普通卡 0x90-员工卡 20130609

	//20130617
	UBYTE g9f27;
	UBYTE uLen_g9f27;

//	UBYTE g9f09[2];//应用版本号
//	UBYTE uLen_g9f09;

	UBYTE g9f74[6];
	UBYTE uLen_g9f74;

	UBYTE g9f63[16];
	UBYTE uLen_g9f63;

	//20130724
	UBYTE g77[256];
	UBYTE uLen_g77;

	//20130827
	UBYTE g9f69[256];
	int uLen_g9f69;


	UBYTE g9f08[2];
	int uLen_g9f08;
	//20130902
	UBYTE g70[256];
	UBYTE uLen_g70;

	UBYTE g77[256];
	UBYTE uLen_g77;

	typedef struct
	{
		UBYTE TradeTime[7]; 	//交易时间
		UBYTE TradeNum[3];	//交易流水号，HEX，由检票机维护，但要求连续。
		UBYTE Flag;
		
	} R_Entry ;
	R_Entry* gR_Entry;

	typedef struct
	{
		UBYTE TradeTime[7]; 	//交易时间
		UBYTE TradeNum[3];	//交易流水号，HEX，由检票机维护，但要求连续。
		UBYTE Amount[3];	//交易金额。
		
		UBYTE TradeType;//
		UBYTE RFU[2];//
	} R_Exit ;
	R_Exit* gR_Exit;
	

	typedef struct
	{
		UBYTE TradeTime[7]; 	//交易时间
		UBYTE TradeNum[3];	//交易流水号，HEX，由检票机维护，但要求连续。
		UBYTE BlackLevel;//黑卡等级
		
	} R_Lock ;
	R_Lock* gR_Lock;



	typedef struct
	{
		UBYTE TradeTime[7]; 	//交易时间
		UBYTE TradeNum[3];	//交易流水号，HEX，由检票机维护，但要求连续。
		UBYTE Amount[3];	//交易金额。
		
		UBYTE Status;//更新后的状态标志
		UBYTE EntryTermNo[4];//进站轨道交通终端编号
		UBYTE ExitTermNo[4];	//出站轨道交通终端编号
		UBYTE AmountEXP[3];	//预计交易金额。
		UBYTE TimeFlag;//交易时间标志
		UBYTE RFU[3+3];//
	} R_Update ;
	R_Update* gR_Update;



	typedef struct
	{
		UBYTE ID[2]; 	//ID标识（区号）
		UBYTE RecLen; 	//记录长度

		UBYTE ValidFlag;	//应用有效标识（0表示无效；1表示有效）
		UBYTE AppExtFlag;	//扩展应用标识（1表示本应用采用分段扣费；2表示应用采用脱机预授权消费）
		UBYTE AppLockFlag;//应用锁定标志（0表示应用没有锁定；1表示应用锁定）

		UBYTE Status;	//状态标志

		UBYTE EntryTime[7];	//进站交易时间
		UBYTE EntryTermNo[4];	//进站轨道交通终端编号
		UBYTE ExitTime[7];	//出站交易时间
		UBYTE ExitTermNo[4];	//出站轨道交通终端编号

		
		UBYTE LastTime[7];	//末笔交易时间
		UBYTE LastTermNo[4];	//末笔轨道交通终端编号
		UBYTE MonthAmount[3];	//当月轨道交通累计金额
		UBYTE MonthCount[2];	//当月轨道交通累计次数
		
		UBYTE Amount[2];	//实际交易金额
		UBYTE RFU[17];	//保留
	} F_Data0221 ;
	F_Data0221* g_Data0221;
	
	
	typedef struct
	{
		UBYTE TradeType;	//交易类型
		UBYTE TermNo[4];	//
		UBYTE Amount[4];	//
		UBYTE BalanceAft[4];	//交易后余额
		UBYTE TradeTime[7];	//交易时间
		UBYTE RFU[12];	//保留
	} F_Data1E ;
	F_Data1E* g_Data1E;
	
	typedef struct
	{
		UBYTE ID; 	//复合消费标志
		UBYTE RecLen; 	//记录长度
		UBYTE AppLockFlag;//应用锁定标志（0表示应用没有锁定；1表示应用锁定）
		UBYTE Status;	//状态标志

		UBYTE EntryTime[7];	//进站交易时间
		UBYTE EntryTermNo[4];	//进站轨道交通终端编号
		UBYTE ExitTime[7];	//出站交易时间
		UBYTE ExitTermNo[4];	//出站轨道交通终端编号

		
		UBYTE LastTime[7];	//末笔交易时间
		UBYTE LastTermNo[4];	//末笔轨道交通终端编号
		UBYTE MonthAmount[3];	//当月轨道交通累计金额
		UBYTE MonthCount[2];	//当月轨道交通累计次数
		
		UBYTE  Amount[2];	//实际交易金额
		UBYTE RFU[17];	//保留
	} F_DataMEC19 ;
	F_DataMEC19* g_DataMEC19;
	
	
	typedef struct
	{
		UBYTE AppAccount[10]; 	//应用主帐号（BCD）
		UBYTE AccountSn; 	//主帐号序列号

		UBYTE IssueBankCode[2];	//发卡行代码
		UBYTE BussiType;	//卡业务类型
		UBYTE Validdate[4];//应用有效期, YYYYMMDD, BCD 码

		UBYTE Balance[4];	//钱包余额，分，HEX

		UBYTE Overdrawlimited[3];	//透支限额，分，HEX
		UBYTE Buf0221[41];	//0221文件数据
		UBYTE Buf0321[5];	//联乘优惠信息（行业+MMDDhhmm）
		UBYTE AID[17];
	
		UBYTE RFU[8];	//保留
	} D_CardInfo ;
	D_CardInfo* g_CardInfo;
	

	typedef struct
	{
		UBYTE CardSn[4];//卡唯一代码（应用序列号后4字节）
		UBYTE AppSn[10]; 	//应用序列号
			
		UBYTE CardType;	//卡类型
		UBYTE Validdate[4];//应用有效期, YYYYMMDD, BCD 码

		UBYTE Balance[4];	//钱包余额，分，HEX

		UBYTE Flag;	//范围标志
		UBYTE LineBitMap[6];		//线路位图
		UBYTE StationBitMap[12];		//车站位图
		UBYTE Buf19[41];	//0x19文件数据
		
	
		UBYTE RFU[5];	//保留
	} D_MECCardInfo ;
	D_MECCardInfo* g_MECCardInfo;


	typedef struct
	{
		UBYTE EntryData[1+8]; 	//应用密文
		UBYTE AppInfoData[1+1]; 	//应用信息数据

		UBYTE IssueBankData[1+32];	//发卡行应用数据
		UBYTE Rand[1+4];	//不可预知数
		UBYTE Counter[1+2];//票卡计数器

		UBYTE TVR[1+5];	//终端验证结果

		UBYTE AIP[1+2];	//应用交互特征
		UBYTE TermFunc[1+3];	//终端性能
		UBYTE AppVer[1+2];	//应用版本号
		UBYTE AuthCode[1+6];//电子现金发卡行授权码
		UBYTE ProductID[1+16];		//产品信息标识
		UBYTE DDAFlag;//DDA数据完成标志
		UBYTE RFU[3];	//保留
	} D_Entry ;
	D_Entry* gD_Entry;
	
	typedef struct
	{
		UBYTE TransNum[3]; //交易流水号
		UBYTE AppSn[10]; 	//应用序列号，卡号
		UBYTE ATC[2];	//ATC
		UBYTE RFU[1];	
	} stru_TransData_Head ;


	typedef struct
	{
		UBYTE KeyIndex; 	//密钥索引
		UBYTE KeyVer; 	//密钥版本

		UBYTE OffTradeCount[2];	//脱机交易序号
		UBYTE TermTradeCount[4];	//终端交易序号
		UBYTE TAC[4];//
		UBYTE RFU[4];	//保留
	} D_MECEntry ;
	D_MECEntry* gD_MECEntry;
	
	typedef struct
	{
		UBYTE KeyIndex; 	//密钥索引
		UBYTE KeyVer; 	//密钥版本

		UBYTE OffTradeCount[2];	//脱机交易序号
		UBYTE TermTradeCount[4];	//终端交易序号
		UBYTE BALANCE[3];	//交易后余额
		UBYTE TAC[4];//
		
	} D_MECExit ;
	D_MECExit* gD_MECExit;


	typedef struct
	{
		UBYTE Ver[4];
		UBYTE R_MAC_Flag[1];
		UBYTE LCYH_Flag[1];//联乘优惠
		UBYTE Days[4];
		UBYTE SHA1[20];
		UBYTE RFU[10];
		UBYTE RecordNum[1];
			
	} S_TerminalParam_Head;
	S_TerminalParam_Head* g_TP_Head=NULL;//20130722
	typedef struct
	{
		UBYTE RID[5];
		UBYTE CAIndex;	
		UBYTE CAValiddate[4];
		UBYTE HashFlag;	//01:SHA-1
		UBYTE CAFlag;	//01 :RSA
		UBYTE ModuleLen[2];
		UBYTE Module[256];
		UBYTE CAExp[4];//公钥内容e
		UBYTE SHA1[20];
		UBYTE RFU[2];
			
	} S_TerminalParam_Record;//每条记录296字节
	S_TerminalParam_Record* g_TP_Record=NULL;//20130722
	//20130725
	typedef struct
	{
		UBYTE KeyIndex; 	//密钥索引
		UBYTE KeyVer; 	//密钥版本

		UBYTE OffTradeCount[2];	//脱机交易序号 uAtc
		UBYTE TermTradeCount[4];	//终端交易序号,uPSAMCounter
		UBYTE Amount[3];	//交易额
		UBYTE uBalance[4];//交易前金额
		UBYTE TransNum[3];//交易流水号
		UBYTE AppSn[10]; 	//应用序列号，卡号,gAppSN
		UBYTE RFU[4];
	} S_MECBuffer ;
	S_MECBuffer* gS_MECBuffer=NULL;


	//20130619
	int nLen_AFL=0;
	UBYTE AFL[256];
	void init_tagList(void);
	int PPSE_Select(void);
	int L2Tester(void);
	int EMU_GetCardinfo(UBYTE *outbuf,UWORD *outbytes);
	int EMU_Entry(UBYTE *outbuf,UWORD *outbytes);
	int EMU_Exit(UBYTE *outbuf,UWORD *outbytes);
	int EMU_fDDA(UBYTE *outbuf,UWORD *outbytes);
	
	int fdda_L2(UBYTE* uAID);
	int initparam(void);
	int checkparam(void);
	void	String2Bytes(unsigned int nLen,char *pszStr,unsigned char *pszBuf);
	void GetTagFrmTLV(UBYTE* uTlv,int nLen_TLV);
	int Parse_AIDRESP(UBYTE* uInData,int nLen_InData);

	//20120413
	int wr_Record_eeprom(UBYTE* uBuf,int nLen);
	int wr_Record_flash(UBYTE* uBuf,int nLen);
	int wr_Trade(void);
	int rd_Record_flash(UBYTE* uBuf,int nRecordno);
	void	BCD2String(unsigned int nLen,UBYTE *pszBCD,char *pszStr);
	int L2Tester();
	
	void *pth001E();
	//void *pth801A();
	void pth801A();
#endif
