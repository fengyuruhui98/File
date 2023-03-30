#include <stdio.h>

unsigned char reader_status;

//函数原型：
//函数功能：相同本站的定义
//函数描述：
//参    数：
//返 回 值：
long EOD_3082_ExchangeStation()
{
FILE *intFile;
long lngPosition, lngnum;
struct PARA_3082 *tpTemppara;
long i;

    //检查文件的存在问题
    intFile = fopen("./para/3082.dat", "rb");
    if(intFile == NULL)
    {
    	tpExchangeSta3082 = NULL;
        return 1;
    }

    //
    fseek(intFile, 0, SEEK_END);
    lngPosition = ftell(intFile) + 1;
    lngnum = 0;
    fseek(intFile, 0, 0);
    fread(&tpPubTitle3082, sizeof(tpPubTitle3082), 1, intFile);
    LongToByte(tpPubTitle3082.lngExpiredTime, (unsigned char *)&tpPubTitle3082.lngExpiredTime);
    LongToByte(tpPubTitle3082.lngTableVersion, (unsigned char *)&tpPubTitle3082.lngTableVersion);

    lngnum = (lngPosition - sizeof(tpPubTitle3082))/sizeof(struct PARA_3082);
#ifdef DEBUG_PRINT
    printf("3082 version is %d record num %d struct len %d\n", tpPubTitle3082.lngTableVersion, lngnum, sizeof(struct PARA_3082));
#endif
    if(lngnum == 0)
    {
    	if(tpExchangeSta3082 != NULL)
    		free(tpExchangeSta3082);
    	tpExchangeSta3082 = NULL;
    	tpArraybound.tpExchangeSta3082 = 0;
        return 0;
    }
    tpTemppara = (struct PARA_3082 *)malloc(sizeof(struct PARA_3082) * lngnum);
    if(tpTemppara == NULL)
    {
    	return 1;
    }
    if(tpExchangeSta3082 != NULL)
        free(tpExchangeSta3082);
    tpExchangeSta3082 = tpTemppara;
    tpArraybound.tpExchangeSta3082 = lngnum;
    fread(tpExchangeSta3082, sizeof(struct PARA_3082), lngnum, intFile);
    fclose(intFile);
#ifdef DEBUG_PRINT
	printf("3082 exchange station number is %d\n", tpArraybound.tpExchangeSta3082);
	for(i = 0; i < tpArraybound.tpExchangeSta3082; i++)
	{
		printf("1st %02x%02x 2nd %02x%02x 3th %02x%02x 4thu %02x%02x 5fri %02x%02x 6sat %02x%02x\n",
				tpExchangeSta3082[i].bytStation[0][0], tpExchangeSta3082[i].bytStation[0][1], tpExchangeSta3082[i].bytStation[1][0], tpExchangeSta3082[i].bytStation[1][1], 
				tpExchangeSta3082[i].bytStation[2][0], tpExchangeSta3082[i].bytStation[2][1], tpExchangeSta3082[i].bytStation[3][0], tpExchangeSta3082[i].bytStation[3][1], 
				tpExchangeSta3082[i].bytStation[4][0], tpExchangeSta3082[i].bytStation[4][1], tpExchangeSta3082[i].bytStation[5][0], tpExchangeSta3082[i].bytStation[5][1]);
	}
#endif    
    //EOD_3085_ExitEntryAgain();
    return 0;
}

//
//函数原型：
//函数功能：相同本站的定义
//函数描述：
//参    数：
//返 回 值：
long EOD_3085_ExitEntryAgain()
{
FILE *intFile;
long lngPosition, lngnum;
struct PARA_3082 *tpTemppara;

    //检查文件的存在问题
    intFile = fopen("./para/3085.dat", "rb");
    if(intFile == NULL)
    {
        return 1;
    }

    //
    fseek(intFile, 0, SEEK_END);
    lngPosition = ftell(intFile) + 1;
    lngnum = 0;
    fseek(intFile, 0, 0);
    fread(&tpPubTitle3085, sizeof(tpPubTitle3085), 1, intFile);
    LongToByte(tpPubTitle3085.lngExpiredTime, (unsigned char *)&tpPubTitle3085.lngExpiredTime);
    LongToByte(tpPubTitle3085.lngTableVersion, (unsigned char *)&tpPubTitle3085.lngTableVersion);
    //printf("3085 version is %d\n", tpPubTitle3085.lngTableVersion);

    lngnum = (lngPosition - sizeof(tpPubTitle3085))/sizeof(struct PARA_3085);
    if(lngnum == 0)
        return 0;
    tpTemppara = (struct PARA_3082 *)malloc(sizeof(struct PARA_3082) * (lngnum + tpArraybound.tpExchangeSta3082));
    if(tpTemppara == NULL)
    {
    	return 1;
    }
    if(tpExchangeSta3082 != NULL)
    {
        memcpy(tpTemppara, tpExchangeSta3082, sizeof(struct PARA_3082) * tpArraybound.tpExchangeSta3082);
        free(tpExchangeSta3082);
    }
    tpExchangeSta3082 = tpTemppara;
    tpArraybound.tpExchangeSta3085 = lngnum;
    fread(&tpExchangeSta3082[tpArraybound.tpExchangeSta3082], sizeof(struct PARA_3082), lngnum, intFile);
    fclose(intFile);
    return 0;
}

//
//函数原型：
//函数功能：
//函数说明：节日表
//参    数：
//返 回 值：
long EOD_4001_Holiday()
{
FILE *intFile;
long lngPosition, lngHolidayVer, lngnum;
struct PARA_4001 *tpTemppara;
long i;

    //检查文件的存在问题
    intFile = fopen("./para/4001.dat", "rb");
    if(intFile == NULL)
    { 
        tpHoliday4001 = NULL;
        return 1;
    }

    //读取票值表
    fseek(intFile, 0, SEEK_END);
    lngPosition = ftell(intFile) + 1;
    lngnum = 0;
	  fseek(intFile, 0, 0);
    //节日表无生效时间
    fread(&lngHolidayVer, sizeof(lngHolidayVer), 1, intFile);
    LongToByte(lngHolidayVer, (unsigned char *)&tpPubTitle4001.lngTableVersion);
        
    lngnum = (lngPosition - sizeof(long))/sizeof(struct PARA_4001);
#ifdef DEBUG_PRINT
	printf("4001 version is %d record num is %d struct len is %d\n", tpPubTitle4001.lngTableVersion, lngnum, sizeof(struct PARA_4001));
#endif
	if(0 == lngnum)
	{
		if(tpHoliday4001 != NULL)
			free(tpHoliday4001);
		tpHoliday4001 = NULL;
		tpArraybound.tpHoliday4001 = 0;
		return 0;
	}
    tpTemppara = (struct PARA_4001 *)malloc(sizeof(struct PARA_4001) * lngnum);
    if(tpTemppara == NULL)
    {
    	return 1;
    }
    if(tpHoliday4001 != NULL)
        free(tpHoliday4001);
    tpHoliday4001 = tpTemppara;
    tpArraybound.tpHoliday4001 = lngnum;
    fread(tpHoliday4001, sizeof(struct PARA_4001), lngnum, intFile);
	for(i = 0; i < lngnum; i++)    
		LongToByte(tpHoliday4001[i].lngStartHoliday, (unsigned char *)&(tpHoliday4001[i].lngStartHoliday));
    fclose(intFile);
#ifdef DEBUG_PRINT
	printf("4001 holiday number is %d\n", tpArraybound.tpHoliday4001);
	for(i = 0; i < tpArraybound.tpHoliday4001; i++)
	{
		printf("%08x\t", tpHoliday4001[i].lngStartHoliday);
	}
	printf("\n");
#endif
    return 0;
}

//
//函数原型：
//函数功能：
//函数描述：车票类型表
//参    数：
//返 回 值：
long EOD_4002_TicketSet()
{
FILE *intFile;
short int intOpraRecLen;
long lngPosition, lngnum;
unsigned char bytNum;
struct PARA_4002_TICKET * tpTemppara;
long i;

    //检查文件的存在问题
    intFile = fopen("./para/4002.dat", "rb");
    if(intFile == NULL)
    {
        tpTicketInfo4002sub = NULL;
        return 1;
    }

    //
    fseek(intFile, 0, SEEK_END);
    lngnum = 0;
    lngPosition = ftell(intFile) + 1;
    fseek(intFile, 0, 0);
    fread(&tpPubTitle4002, sizeof(tpPubTitle4002), 1, intFile);
        LongToByte(tpPubTitle4002.lngExpiredTime, (unsigned char *)&tpPubTitle4002.lngExpiredTime);
        LongToByte(tpPubTitle4002.lngTableVersion, (unsigned char *)&tpPubTitle4002.lngTableVersion);

    fread(&tpTicketSet4002, sizeof(tpTicketSet4002), 1, intFile);

	ShortToByte(tpTicketSet4002.intLosePunish, (unsigned char *)&tpTicketSet4002.intLosePunish);
	ShortToByte(tpTicketSet4002.intOverfarePunish, (unsigned char *)&tpTicketSet4002.intOverfarePunish);
	ShortToByte(tpTicketSet4002.intOvertimePunish, (unsigned char *)&tpTicketSet4002.intOvertimePunish);
	ShortToByte(tpTicketSet4002.intUpdatePunish, (unsigned char *)&tpTicketSet4002.intUpdatePunish);
	ShortToByte(tpTicketSet4002.intStoreInitVal, (unsigned char *)&tpTicketSet4002.intStoreInitVal);

	lngnum = (lngPosition - sizeof(tpPubTitle4002) - sizeof(tpTicketSet4002))/sizeof(struct PARA_4002_TICKET);

#ifdef DEBUG_PRINT
    printf("4002 version is %d record num is %d struct len %d\n", tpPubTitle4002.lngTableVersion, lngnum, sizeof(struct PARA_4002_TICKET));
	printf("lose penalty:%d overfare penalty:%d overtime penalty %d entry/exit penalty %d operating next day %d",
			tpTicketSet4002.intLosePunish, tpTicketSet4002.intOverfarePunish, tpTicketSet4002.intOvertimePunish, tpTicketSet4002.intUpdatePunish, tpTicketSet4002.bytEffectivePeriod);
	for(i = 0; i < 16; i++)
	{
		if((i % 8) == 0) printf("\n");
		printf("%d\t", tpTicketSet4002.bytStayTime[i] * 6);
	}
	printf("\n");
#endif 
	if(0 == lngnum)
	{
		if(tpTicketInfo4002sub != NULL)
			free(tpTicketInfo4002sub);
		tpTicketInfo4002sub = NULL;
		tpArraybound.tpTicketInfo4002sub = 0;
		return 0;
	}
	tpTemppara = (struct PARA_4002_TICKET *)malloc(sizeof(struct PARA_4002_TICKET) * lngnum);
	if(tpTemppara == NULL)
	{
		return 1;
	}
    if(tpTicketInfo4002sub != NULL)
        free(tpTicketInfo4002sub);
    tpTicketInfo4002sub = tpTemppara;
    tpArraybound.tpTicketInfo4002sub = lngnum;
    fread(tpTicketInfo4002sub, sizeof(struct PARA_4002_TICKET), lngnum, intFile);
    //
	for(lngPosition = 0; lngPosition < lngnum; lngPosition++)
	{
		//
		LongToByte(tpTicketInfo4002sub[lngPosition].lngTicketNamePointer, (unsigned char *)&tpTicketInfo4002sub[lngPosition].lngTicketNamePointer);
		//tpTicketInfo4002sub[lngPosition].lngTicketNamePointer = (tpTicketInfo4002sub[lngPosition].lngTicketNamePointer) / 16;

		ShortToByte(tpTicketInfo4002sub[lngPosition].intTicketControl, (unsigned char *)&tpTicketInfo4002sub[lngPosition].intTicketControl);
		ShortToByte(tpTicketInfo4002sub[lngPosition].intTicketVerify, (unsigned char *)&tpTicketInfo4002sub[lngPosition].intTicketVerify);
		ShortToByte(tpTicketInfo4002sub[lngPosition].intEffectivePeriod, (unsigned char *)&tpTicketInfo4002sub[lngPosition].intEffectivePeriod);
		ShortToByte(tpTicketInfo4002sub[lngPosition].intSpecialPeriod, (unsigned char *)&tpTicketInfo4002sub[lngPosition].intSpecialPeriod);

		ShortToByte(tpTicketInfo4002sub[lngPosition].intInitVal, (unsigned char *)&tpTicketInfo4002sub[lngPosition].intInitVal);
		ShortToByte(tpTicketInfo4002sub[lngPosition].intDeposit, (unsigned char *)&tpTicketInfo4002sub[lngPosition].intDeposit);
		ShortToByte(tpTicketInfo4002sub[lngPosition].intCSCInitVal, (unsigned char *)&tpTicketInfo4002sub[lngPosition].intCSCInitVal);
		ShortToByte(tpTicketInfo4002sub[lngPosition].intCSCInitBonus, (unsigned char *)&tpTicketInfo4002sub[lngPosition].intCSCInitBonus);

		for(bytNum = 0; bytNum < 4; bytNum++)
			ShortToByte(tpTicketInfo4002sub[lngPosition].intCSCAddSel[bytNum], (unsigned char *)&tpTicketInfo4002sub[lngPosition].intCSCAddSel[bytNum]);
		for(bytNum = 0; bytNum < 4; bytNum++)
			ShortToByte(tpTicketInfo4002sub[lngPosition].intCSCAddBonus[bytNum], (unsigned char *)&tpTicketInfo4002sub[lngPosition].intCSCAddBonus[bytNum]);
            
		ShortToByte(tpTicketInfo4002sub[lngPosition].intAddedBonusTimes, (unsigned char *)&tpTicketInfo4002sub[lngPosition].intAddedBonusTimes);
		LongToByte(tpTicketInfo4002sub[lngPosition].lngAddedBonusAmount, (unsigned char *)&tpTicketInfo4002sub[lngPosition].lngAddedBonusAmount);
		ShortToByte(tpTicketInfo4002sub[lngPosition].intAddedBonusPercent, (unsigned char *)&tpTicketInfo4002sub[lngPosition].intAddedBonusPercent);
		ShortToByte(tpTicketInfo4002sub[lngPosition].intLianchAmount, (unsigned char *)&tpTicketInfo4002sub[lngPosition].intLianchAmount);
		ShortToByte(tpTicketInfo4002sub[lngPosition].intLianchPercent, (unsigned char *)&tpTicketInfo4002sub[lngPosition].intLianchPercent);
		
		ShortToByte(tpTicketInfo4002sub[lngPosition].intOvertimePunish, (unsigned char *)&tpTicketInfo4002sub[lngPosition].intOvertimePunish);
		ShortToByte(tpTicketInfo4002sub[lngPosition].intExitPunish, (unsigned char *)&tpTicketInfo4002sub[lngPosition].intExitPunish);
		
		LongToByte(tpTicketInfo4002sub[lngPosition].lngMonthAmount, (unsigned char *)&tpTicketInfo4002sub[lngPosition].lngMonthAmount);
		//单程票
		if(tpTicketInfo4002sub[lngPosition].bytTicketCode == 0x64)
		{
			tpTicketDef = tpTicketInfo4002sub[lngPosition];
			//SJTIndex = lngPosition;
		}
	}
    fclose(intFile);
#ifdef DEBUG_PRINT
	for(i = 0; i < tpArraybound.tpTicketInfo4002sub; i++)
	{
		printf("Ticketytpe %02x Ticketname %s\n", tpTicketInfo4002sub[i].bytTicketCode, tpTicketInfo4002sub[i].strTicketNameEn);
		printf("TicketControl %04x Ticket verify %04x\n",tpTicketInfo4002sub[i].intTicketControl, tpTicketInfo4002sub[i].intTicketVerify);
		printf("normalArea index %02x Extendtime %02x n-peakcode %02x rebatecode %02x costcode %02x\n", 
			tpTicketInfo4002sub[i].bytNormalAreaIndex, tpTicketInfo4002sub[i].bytNormalExtend, tpTicketInfo4002sub[i].bytNormalNotpeakCode, tpTicketInfo4002sub[i].bytNormalRebateCode, tpTicketInfo4002sub[i].bytNormalCostCode);
		printf("weekendArea index %02x Extendtime %02x n-peakcode %02x rebatecode %02x costcode %02x\n", 
			tpTicketInfo4002sub[i].bytWeekendAreaIndex, tpTicketInfo4002sub[i].bytWeekendExtend, tpTicketInfo4002sub[i].bytWeekendNotpeakCode, tpTicketInfo4002sub[i].bytWeekendRebateCode, tpTicketInfo4002sub[i].bytWeekendCostCode);
		printf("holidayArea index %02x Extendtime %02x n-peakcode %02x rebatecode %02x costcode %02x\n", 
			tpTicketInfo4002sub[i].bytHolidayAreaIndex, tpTicketInfo4002sub[i].bytHolidayExtend, tpTicketInfo4002sub[i].bytHolidayNotpeakCode, tpTicketInfo4002sub[i].bytHolidayRebateCode, tpTicketInfo4002sub[i].bytHolidayCostCode);
		printf("Added bonus times %d Added bonus amount(d) %d Added bonus percent %d \n", 
			tpTicketInfo4002sub[i].intAddedBonusTimes, tpTicketInfo4002sub[i].lngAddedBonusAmount, tpTicketInfo4002sub[i].intAddedBonusPercent);
		printf("Con-cosume bonus amount %d con-cosume bonus percent %d con-consume priority %d con-consume time %d\n",
			tpTicketInfo4002sub[i].intLianchAmount, tpTicketInfo4002sub[i].intLianchPercent,tpTicketInfo4002sub[i].bytBonusPriority, tpTicketInfo4002sub[i].bytLianchEffectime);
		printf("overtime penalty %d entry/exit penalty %d max-balance %02x%02x%02x month-amount %d\n", 
			tpTicketInfo4002sub[i].intOvertimePunish, tpTicketInfo4002sub[i].intExitPunish, tpTicketInfo4002sub[i].bytMaxBalance[0], tpTicketInfo4002sub[i].bytMaxBalance[1], tpTicketInfo4002sub[i].bytMaxBalance[2]);
		printf("max-month times %d max-day times %d\n", tpTicketInfo4002sub[i].bytMaxMonthTimes, tpTicketInfo4002sub[i].bytMaxDayTimes);
	}

#endif
    return 0;
}

//
//函数原型：
//函数功能：
//函数描述：费率表
//参    数：
//返 回 值：
long EOD_4003_FareValue()
{
FILE *intFile;
long lngPosition, lngnum;
unsigned char bytNum;
struct PARA_4003 *tpTemppara;
    
    //
    intFile = fopen("./para/4003.dat", "rb");
    if(intFile == NULL)
    {
        tpFareValue4003 = NULL;
        return 1;
    }
    //
    fseek(intFile, 0, SEEK_END);
    lngPosition = ftell(intFile) + 1;
    fseek(intFile, 0, 0);
    lngnum = 0;
    fread(&tpPubTitle4003, sizeof(tpPubTitle4003), 1, intFile);
        LongToByte(tpPubTitle4003.lngExpiredTime, (unsigned char *)&tpPubTitle4003.lngExpiredTime);
        LongToByte(tpPubTitle4003.lngTableVersion, (unsigned char *)&tpPubTitle4003.lngTableVersion);
    lngnum = (lngPosition - sizeof(tpPubTitle4003))/sizeof(struct PARA_4003);
#ifdef DEBUG_PRINT
    printf("4003 version is %d record num is %d struct len is %d\n", tpPubTitle4003.lngTableVersion, lngnum, sizeof(struct PARA_4003));
#endif
    if(0 == lngnum)
    {
    	if(tpFareValue4003 != NULL)
    		free(tpFareValue4003);
    	tpFareValue4003 = NULL;
    	tpArraybound.tpFareValue4003 = 0;
    	return 0;
    }
    tpTemppara = (struct PARA_4003 *)malloc(sizeof(struct PARA_4003) * lngnum);
    if(tpTemppara == NULL)
    {
    	return 1;
    }
    if(tpFareValue4003 != NULL)
        free(tpFareValue4003);
    tpFareValue4003 = tpTemppara;
    tpArraybound.tpFareValue4003 = lngnum;
    fread(tpFareValue4003, sizeof(struct PARA_4003), lngnum, intFile);
    	for(lngPosition = 0; lngPosition < lngnum; lngPosition++)
    	{
            for(bytNum = 0; bytNum < 63; bytNum++)
                ShortToByte(tpFareValue4003[lngPosition].intFareValue[bytNum], (unsigned char *)&tpFareValue4003[lngPosition].intFareValue[bytNum]);
        }
    fclose(intFile);
#ifdef DEBUG_PRINT
	printf("fare value record number is %d\n", tpArraybound.tpFareValue4003);
#endif  
    return 0;
}

//
//函数原型：
//函数功能：
//函数描述：区域表
//参    数：
//返 回 值：
long EOD_4004_StationFare()
{
FILE *intFile;
unsigned long lngPosition, lngnum, lngTempi, lngTempj;
unsigned char bytNum, bytFareNum;
struct PARA_4004_FARE *tpTemppara;

    //
    intFile = fopen("./para/4004.dat", "rb");
    if(intFile == NULL)
    {
        tpStationFare4004sub = NULL;
        return 0;
    }
    //
    fseek(intFile, 0, SEEK_END);
    lngPosition = ftell(intFile) + 1;
    fseek(intFile, 0, 0);
    lngnum = 0;
    bytNum = 0;
    fread(&tpPubTitle4004, sizeof(tpPubTitle4004), 1, intFile);
        LongToByte(tpPubTitle4004.lngExpiredTime, (unsigned char *)&tpPubTitle4004.lngExpiredTime);
        LongToByte(tpPubTitle4004.lngTableVersion, (unsigned char *)&tpPubTitle4004.lngTableVersion);

    fread(&tpStationFare4004, sizeof(tpStationFare4004), 1, intFile);
		ShortToByte(tpStationFare4004.StationNum, (unsigned char *)&tpStationFare4004.StationNum);
	
    lngnum = (lngPosition - sizeof(tpPubTitle4004) - sizeof(tpStationFare4004))/sizeof(struct PARA_4004_FARE);
#ifdef DEBUG_PRINT
    printf("4004 version is %d station num %d record %d struct len %d\n", tpPubTitle4004.lngTableVersion, tpStationFare4004.StationNum, lngnum, sizeof(struct PARA_4004_FARE));
#endif

    if(0 == lngnum)
    {
    	if(tpStationFare4004sub != NULL)
    		free(tpStationFare4004sub);
    	tpStationFare4004sub = NULL;
    	tpArraybound.tpStationFare4004sub = 0;
    	return 0;
    }
    tpTemppara = (struct PARA_4004_FARE *)malloc(sizeof(struct PARA_4004_FARE) * lngnum);        
    if(tpTemppara == NULL)
    {
    	return 1;
    }
    if(tpStationFare4004sub != NULL)
        free(tpStationFare4004sub);
    tpStationFare4004sub = tpTemppara;
    tpArraybound.tpStationFare4004sub = lngnum;
    fread(tpStationFare4004sub, sizeof(struct PARA_4004_FARE), lngnum, intFile);
    fclose(intFile);

#ifdef DEBUG_PRINT
	for(lngTempi = 0; lngTempi < lngnum; lngTempi++)
	{
		printf("station %02x%02x fare %02x\n", tpStationFare4004sub[lngTempi].bytExitSta[0],tpStationFare4004sub[lngTempi].bytExitSta[1], tpStationFare4004sub[lngTempi].bytFare);
	}
#endif    
    return 0;
}

//
//函数原型：
//函数功能：
//函数描述：非高峰时段表
//参    数：
//返 回 值：
long EOD_4006_NotPeak()
{
FILE *intFile;
long lngPosition, lngnum;
unsigned char bytNum;
struct PARA_4006 *tpTemppara;

    //检查文件的存在问题
    intFile = fopen("./para/4006.dat", "rb");
    if(intFile == NULL)
    {
        tpNotPeak4006 = NULL;
        return 1;
    }
    //
    fseek(intFile, 0, SEEK_END);
    lngPosition = ftell(intFile) + 1;
    lngnum = 0;
    fseek(intFile, 0, 0);
	fread(&tpPubTitle4006, sizeof(tpPubTitle4006), 1, intFile);
        LongToByte(tpPubTitle4006.lngExpiredTime, (unsigned char *)&tpPubTitle4006.lngExpiredTime);
        LongToByte(tpPubTitle4006.lngTableVersion, (unsigned char *)&tpPubTitle4006.lngTableVersion);

	lngnum = (lngPosition - sizeof(tpPubTitle4006))/sizeof(struct PARA_4006);
#ifdef DEBUG_PRINT
	printf("4006 version is %d record num %d struct len %d\n", tpPubTitle4006.lngTableVersion, lngnum, sizeof(struct PARA_4006));
#endif
	if(0 == lngnum)
	{
		if(tpNotPeak4006 != NULL)
			free(tpNotPeak4006);
		tpNotPeak4006 = NULL;
		tpArraybound.tpNotPeak4006 = 0;
		return 0;
	}
	tpTemppara = (struct PARA_4006 *)malloc(sizeof(struct PARA_4006) * lngnum);
	if(tpTemppara == NULL)
	{
		return 1;
	}	
    if(tpNotPeak4006 != NULL)
        free(tpNotPeak4006);
    tpNotPeak4006 = tpTemppara;
    tpArraybound.tpNotPeak4006 = lngnum;
    fread(tpNotPeak4006, sizeof(struct PARA_4006), lngnum, intFile);
    fclose(intFile);
#ifdef DEBUG_PRINT
	
#endif
    return 0;
}

//
//函数原型：
//函数功能：车票映射关系表
//函数描述：
//参    数：
//返 回 值：
long EOD_4009_TicketMap()
{
FILE *intFile; 
long lngPosition, lngTempVersion, lngnum;
struct PARA_4009 *tpTemppara;

    //临时增加
    tpTicketMap4009 = (struct PARA_4009 *)calloc(1, sizeof(struct PARA_4009));
    //检查文件的存在问题
    intFile = fopen("./para/4009.dat", "rb");
    if(intFile == NULL)
    {
        tpTicketMap4009 = NULL;
        return 1;
    }

    //
    fseek(intFile, 0, SEEK_END);
    lngPosition = ftell(intFile) + 1;
    lngnum = 0;
    fseek(intFile, 0, 0);
	fread(&lngTempVersion, sizeof(long), 1, intFile);
        //tpPubTitle4009.lngExpiredTime = ChangeLongBytesOrder(tpPubTitle4009.lngExpiredTime)
        LongToByte(tpPubTitle4009.lngTableVersion, (unsigned char *)&lngTempVersion);

	lngnum = (lngPosition - sizeof(long))/sizeof(struct PARA_4009);
#ifdef DEBUG_PRINT
	printf("4009 version is %d record num is %d struct len %d\n", tpPubTitle4009.lngTableVersion, lngnum, sizeof(struct PARA_4009));
#endif
	if(0 == lngnum)
	{
		if(tpTicketMap4009 != NULL)
			free(tpTicketMap4009);
		tpTicketMap4009 = NULL;
		tpArraybound.tpTicketMap4009 = 0;
		return 0;
	}
	tpTemppara = (struct PARA_4009*)malloc(sizeof(struct PARA_4009) * lngnum);
	if(tpTemppara == NULL)
	{
		return 1;
	}
	if(tpTicketMap4009 != NULL)
		free(tpTicketMap4009);
	tpTicketMap4009 = tpTemppara;
	tpArraybound.tpTicketMap4009 = lngnum;
	//成批读取记录,高低字节在使用时进行转换
	fread(tpTicketMap4009, sizeof(struct PARA_4009), lngnum, intFile);
    fclose(intFile);
#ifdef DEBUG_PRINT

#endif
    return 0;
}

//
//函数原型：
//函数功能：
//函数描述：
//参    数：
//返 回 值：
long EOD_4015_CMCCMap()
{
FILE *intFile;
long lngPosition, lngTempVersion, lngnum;
struct PARA_4015 *tpTemppara;
long lngTempi, lngTempj;

    tpCMCCMap4015 = (struct PARA_4015 *)calloc(1, sizeof(struct PARA_4015));
    //检查文件的存在问题
    intFile = fopen("./para/4015.dat", "rb");
    if(intFile == NULL)
    {
        tpCMCCMap4015 = NULL;
        return 1;
    }

    
    fseek(intFile, 0, SEEK_END);
    lngPosition = ftell(intFile) + 1;
    lngnum = 0;
    fseek(intFile, 0, 0);
	fread(&lngTempVersion, sizeof(long), 1, intFile);
        LongToByte(tpPubTitle4015.lngTableVersion, (unsigned char *)&lngTempVersion);
        
	lngnum = (lngPosition - sizeof(long))/sizeof(struct PARA_4015);
#ifdef DEBUG_PRINT
	printf("4015 version is %d record num %d struct len %d\n", tpPubTitle4015.lngTableVersion, lngnum, sizeof(struct PARA_4015));
#endif
	if(0 == lngnum)
	{
		if(tpCMCCMap4015 != NULL)
			free(tpCMCCMap4015);
		tpCMCCMap4015 = NULL;
		tpArraybound.tpCMCCMap4015 = 0;
		return 0;
	}
	tpTemppara = (struct PARA_4015 *)malloc(sizeof(struct PARA_4015) * lngnum);
	if(tpTemppara == NULL)
	{
		return 1;
	}
	if(tpCMCCMap4015 != NULL)
		free(tpCMCCMap4015);
	tpCMCCMap4015 = tpTemppara;
	tpArraybound.tpCMCCMap4015 = lngnum;
	//一次读取所有记录,在使用时进行调整
	fread(tpCMCCMap4015, sizeof(struct PARA_4015), lngnum, intFile);
#ifdef DEBUG_PRINT

#endif
	fclose(intFile);
/*
    for(lngTempi = 0; lngTempi < lngnum; lngTempi++)
    {
        for(lngTempj = lngnum - 1; lngTempj >= lngTempi + 1; lngTempj--)
        {
            if(tpCMCCMap4015[lngTempj]. < tpFare[lngTempj - 1])
            {
                bytNum = tpFare[lngTempj];
                tpFare[lngTempj] = tpFare[lngTempj - 1];
                tpFare[lngTempj - 1] = bytNum;
            }
        }
    }
*/
	return 0;
}


long EOD_4020_WhiteList()
{
FILE *intFile;
long lngPosition, lngnum, file_len;
char	filename[100], eod_file[100];
long i, j, k;

	memset(eod_file, 0x00, 100);
	memset(filename, 0x00, 100);
	sprintf(eod_file, "4020");
	if(0 != FileisExist("./", eod_file, filename))
	{
		printf("4020 file not exist\n");
		return 2;
	}
	//check the file
	sprintf(eod_file, "./%s", filename);
    intFile = fopen(eod_file, "rb");
    if(intFile == NULL)
    {
    	printf("4020 file open failure\n");
        return 1;
    }
    //transfer head 1 + 38
	fseek(intFile, 0, SEEK_END);
	file_len = ftell(intFile);
	fseek(intFile, 0, 0);
    //fseek(intFile, 39, 0);		//not nead read
    fread(&tpWhite4020.white_head, sizeof(White_Head_t), 1, intFile);
    
	LongToByte(tpWhite4020.white_head.ParaVersion,(unsigned char *)&tpWhite4020.white_head.ParaVersion);
#ifdef	DEBUG_WHITE_4020
	printf("\n--------4020--------\n");	
	printf("ParaVersion :%04x\n",tpWhite4020.white_head.ParaVersion);
	printf("ContentNumber :%04x\n",tpWhite4020.white_head.ContentNumber);
	printf("SHA_value :%02x%02x%02x%02x%02x %02x%02x%02x%02x%02x %02x%02x%02x%02x%02x %02x%02x%02x%02x%02x\n",
					tpWhite4020.white_head.SHA_value[0],tpWhite4020.white_head.SHA_value[1],tpWhite4020.white_head.SHA_value[2],tpWhite4020.white_head.SHA_value[3],tpWhite4020.white_head.SHA_value[4],
					tpWhite4020.white_head.SHA_value[5],tpWhite4020.white_head.SHA_value[6],tpWhite4020.white_head.SHA_value[7],tpWhite4020.white_head.SHA_value[8],tpWhite4020.white_head.SHA_value[9],
					tpWhite4020.white_head.SHA_value[10],tpWhite4020.white_head.SHA_value[11],tpWhite4020.white_head.SHA_value[12],tpWhite4020.white_head.SHA_value[13],tpWhite4020.white_head.SHA_value[14],
					tpWhite4020.white_head.SHA_value[15],tpWhite4020.white_head.SHA_value[16],tpWhite4020.white_head.SHA_value[17],tpWhite4020.white_head.SHA_value[18],tpWhite4020.white_head.SHA_value[19]);
#endif
    //memcpy(temp_4020,tpWhite4020.white_head.ParaVersion,4);
    //LongToByte(tpWhite4020.white_head.ParaVersion,&buf_4020[0]);
    //增加判断保存的参数文件大小
//    if(tpWhite1932.paratitle.length != file_len)
//    {
//    	fclose(intFile);
//    	memset(&tpWhite1932.paratitle, 0x00, sizeof(ParaTitle));
//    	remove( eod_file );
//    	return 1;
//    }
	//判断内容记录数大小
//	if(tpWhite4020.white_head.ContentNumber == 0)
//  {
//		fclose(intFile);
//		return 1;
//  }

	if(tpWhite4020.White_Content_val != NULL)
	{
		free(tpWhite4020.White_Content_val);
	}
	tpWhite4020.White_Content_val = NULL;
	LongToByte(tpWhite4020.white_head.ContentNumber,(unsigned char *)&tpWhite4020.white_head.ContentNumber);
	//printf("\nContentNumber = %04x\n",tpWhite4020.white_head.ContentNumber);
	if(0 != tpWhite4020.white_head.ContentNumber)
	{
		tpWhite4020.White_Content_val = (White_Content_t	*)malloc(sizeof(White_Content_t) * tpWhite4020.white_head.ContentNumber);
		if(tpWhite4020.White_Content_val == NULL)
		{	printf("\nnei cun faild。。。\n");
			fclose(intFile);
			return 1;
		}
		fread(tpWhite4020.White_Content_val, sizeof(White_Content_t), tpWhite4020.white_head.ContentNumber, intFile);

#ifdef	DEBUG_WHITE_4020
		for(i = 0; i < tpWhite4020.white_head.ContentNumber; i++)
		{
			printf("CardIssuer :%02x%02x%02x%02x %02x%02x%02x%02x ",
				tpWhite4020.White_Content_val[i].CardIssuer[0],tpWhite4020.White_Content_val[i].CardIssuer[1],tpWhite4020.White_Content_val[i].CardIssuer[2],tpWhite4020.White_Content_val[i].CardIssuer[3],
				tpWhite4020.White_Content_val[i].CardIssuer[4],tpWhite4020.White_Content_val[i].CardIssuer[5],tpWhite4020.White_Content_val[i].CardIssuer[6],tpWhite4020.White_Content_val[i].CardIssuer[7]);
			printf("IIN :%02x%02x%02x%02x ",
				tpWhite4020.White_Content_val[i].IIN[0],tpWhite4020.White_Content_val[i].IIN[1],tpWhite4020.White_Content_val[i].IIN[2],tpWhite4020.White_Content_val[i].IIN[3]);
			printf("CardType :%02x ",tpWhite4020.White_Content_val[i].CardType);
			printf("RFU :%02x%02x ",tpWhite4020.White_Content_val[i].RFU[0],tpWhite4020.White_Content_val[i].RFU[1]);
			printf("TicketType :%02x \n",tpWhite4020.White_Content_val[i].TicketType);
		}
		
#endif		
	}

	fclose(intFile);
	return 0;
}

long EOD_4026_Cert()
{
FILE *intFile;
long lngPosition, lngnum, file_len;
char	filename[100], eod_file[100];
long i, j, k;

	memset(eod_file, 0x00, 100);
	memset(filename, 0x00, 100);
	sprintf(eod_file, "4026");
	if(0 != FileisExist("./", eod_file, filename))
	{
		printf("4026 file not exist\n");
		return 2;
	}
	//check the file
	sprintf(eod_file, "./%s", filename);
    intFile = fopen(eod_file, "rb");
    if(intFile == NULL)
    {
    	printf("4026 file open failure\n");
        return 1;
    }
    //transfer head 1 + 38
	fseek(intFile, 0, SEEK_END);
	file_len = ftell(intFile);
	fseek(intFile, 0, 0);
    //fseek(intFile, 39, 0);		//not nead read
    fread(&tpCert4026.cert_head, sizeof(Cert_Head_t), 1, intFile);
    
	LongToByte(tpCert4026.cert_head.ParaVersion,(unsigned char *)&tpCert4026.cert_head.ParaVersion);
#ifdef	DEBUG_CERT_4026
	printf("\n--------4026--------\n");	
	printf("ParaVersion :%04x\n",tpCert4026.white_head.ParaVersion);
	printf("ContentNumber :%04x\n",tpCert4026.white_head.ContentNumber);
	printf("SHA_value :");
	for(i = 0; i < 20; i++)
		printf("%02x", tpCert4026.white_head.SHA_value[i]);
	printf("\n");
#endif
    //memcpy(temp_4020,tpWhite4020.white_head.ParaVersion,4);
    //LongToByte(tpWhite4020.white_head.ParaVersion,&buf_4020[0]);
    //增加判断保存的参数文件大小
//    if(tpWhite1932.paratitle.length != file_len)
//    {
//    	fclose(intFile);
//    	memset(&tpWhite1932.paratitle, 0x00, sizeof(ParaTitle));
//    	remove( eod_file );
//    	return 1;
//    }
	//判断内容记录数大小
//	if(tpWhite4020.white_head.ContentNumber == 0)
//  {
//		fclose(intFile);
//		return 1;
//  }

	if(tpCert4026.Cert_Content_val != NULL)
	{
		free(tpCert4026.Cert_Content_val);
		tpCert4026.Cert_Content_val = NULL;
	}
	LongToByte(tpCert4026.cert_head.ContentNumber,(unsigned char *)&tpCert4026.cert_head.ContentNumber);
	//printf("\nContentNumber = %04x\n",tpWhite4020.white_head.ContentNumber);
	if(0 != tpCert4026.cert_head.ContentNumber)
	{
		tpCert4026.Cert_Content_val = (Cert_Content_t	*)malloc(sizeof(Cert_Content_t) * tpCert4026.cert_head.ContentNumber);
		if(tpCert4026.Cert_Content_val == NULL)
		{	
			printf("\nmemory malloc faild。。。\n");
			fclose(intFile);
			return 1;
		}
		fread(tpCert4026.Cert_Content_val, sizeof(Cert_Content_t), tpCert4026.cert_head.ContentNumber, intFile);

#ifdef	DEBUG_CERT_4026
		for(i = 0; i < tpWCert4026.cert_head.ContentNumber; i++)
		{
			printf("type %02x ver %02x serviceflag :%02x%02x%02x%02x rid %02x%02x%02x%02x%02x ",
				tpCert4026.Cert_Content_val[i].type, tpCert4026.Cert_Content_val[i].ver,
				tpCert4026.Cert_Content_val[i].service_flag[0],tpCert4026.Cert_Content_val[i].service_flag[1],tpCert4026.Cert_Content_val[i].service_flag[2],tpCert4026.Cert_Content_val[i].service_flag[3],
				tpCert4026.Cert_Content_val[i].rid[0],tpCert4026.Cert_Content_val[i].rid[1],tpCert4026.Cert_Content_val[i].rid[2],tpCert4026.Cert_Content_val[i].rid[3],tpCert4026.Cert_Content_val[i].rid[4]);
			printf("index %02x valid_date :%02x%02x al %02x al_flag %02x len %02x\n",
				tpCert4026.Cert_Content_val[i].index, tpCert4026.Cert_Content_val[i].valid_date[0],tpCert4026.Cert_Content_val[i].valid_date[1]
				tpCert4026.Cert_Content_val[i].al, tpCert4026.Cert_Content_val[i].al_flag, tpCert4026.Cert_Content_val[i].len);
			for(j = 0; j < 64; j++)
				printf("%02x", tpCert4026.Cert_Content_val[i].pb[j]);
			pirntf("\n");
			for(j = 0; j < 64; j++)
				printf("%02x", tpCert4026.Cert_Content_val[i].sign[j]);
		}
#endif		
	}

	fclose(intFile);
	return 0;
}

int get_cert_pb(unsigned char family, unsigned char version, unsigned char index, struct Cert_Content *outbuf)
{
int i;
	
	if( tpCert4026.Cert_Content_val == NULL )
		return 0xff;
		
	for(i = 0; i < tpCert4026.cert_head.ContentNumber; i++)
	{
#ifdef	DEBUG_PRINT
		printf("4026 type %02x ver %02x index %02x\n", tpCert4026.Cert_Content_val[i].type, tpCert4026.Cert_Content_val[i].ver, tpCert4026.Cert_Content_val[i].index);
#endif
		if( (family == tpCert4026.Cert_Content_val[i].type) 
			&& (version == tpCert4026.Cert_Content_val[i].ver) 
			&& (index == tpCert4026.Cert_Content_val[i].index) )
		{
			memcpy(outbuf, &tpCert4026.Cert_Content_val[i].type, sizeof(struct Cert_Content));
			break;
		}
	}
	if ( i >= tpCert4026.cert_head.ContentNumber )
		return 0xff;
	
	return 0;
}

int get_qr_map(unsigned char *os_pb_id, unsigned char *os_orgnization, unsigned char physical, unsigned char version, unsigned char *ticket_type)
{
int i;
	
	if( tpWhite4020.White_Content_val == NULL )
		return 0xff;
		
	for(i = 0; i < tpWhite4020.white_head.ContentNumber; i++)
	{
		if( (memcmp(os_pb_id, tpWhite4020.White_Content_val[i].CardIssuer, 4) == 0) &&
			(memcmp(os_orgnization, tpWhite4020.White_Content_val[i].IIN, 4) == 0) )
		{
			if( (tpWhite4020.White_Content_val[i].CardType == 0xFF) && (tpWhite4020.White_Content_val[i].RFU[0] == 0xFF) )
			{//FF：不作比较
				*ticket_type = tpWhite4020.White_Content_val[i].TicketType;
				return 0;
			}
			if( (tpWhite4020.White_Content_val[i].CardType != 0xFF) && (tpWhite4020.White_Content_val[i].RFU[0] != 0xFF) 
				&& (tpWhite4020.White_Content_val[i].CardType == physical) && (tpWhite4020.White_Content_val[i].RFU[0] == version) )
			{
				*ticket_type = tpWhite4020.White_Content_val[i].TicketType;
				return 0;
			}
			if( (tpWhite4020.White_Content_val[i].CardType != 0xFF) && (tpWhite4020.White_Content_val[i].CardType == physical) )
			{
				*ticket_type = tpWhite4020.White_Content_val[i].TicketType;
				return 0;
			}
			if( (tpWhite4020.White_Content_val[i].RFU[0] != 0xFF) && (tpWhite4020.White_Content_val[i].RFU[0] == version) )
			{
				*ticket_type = tpWhite4020.White_Content_val[i].TicketType;
				return 0;
			}
		}
	}
	return 0xff;
}
//
//函数原型：
//函数功能：判断今天是否是假日
//函数描述：
//参    数：
//返 回 值：
unsigned char IsTodayHoliday()
{
	/*
unsigned char bytHdate[4], bytNHdate[4];
long lngHdayNum, lngRemainNum;
long lngMinute, lngStayTime;
//time_t lnglocaltime;
struct tm *tplocaltime;
    //
    time(&lnglocaltime);
    tplocaltime = localtime(&lnglocaltime);
    bytHdate[0] = (tplocaltime->tm_year + 1900) / 100;
    bytHdate[0] = (bytHdate[0] / 10) * 16 + bytHdate[0] % 10;
    bytHdate[1] = (tplocaltime->tm_year + 1900) % 100;
    bytHdate[1] = (bytHdate[1] / 10) * 16 + bytHdate[1] % 10;
    bytHdate[2] = tplocaltime->tm_mon + 1;
    bytHdate[2] = (bytHdate[2] / 10) * 16 + bytHdate[2] % 10;
    bytHdate[3] = tplocaltime->tm_mday;
    bytHdate[3] = (bytHdate[3] / 10) * 16 + bytHdate[3] % 10;
    lngStayTime = (long)(tpTicketSet4002.bytEffectivePeriod) * 60;
    lngMinute = (long)(tplocaltime->tm_hour) * 60 + tplocaltime->tm_min;
    lnglocaltime += 43200;
    tplocaltime = localtime(&lnglocaltime);
    bytNHdate[0] = (tplocaltime->tm_year + 1900) / 100;
    bytNHdate[0] = (bytNHdate[0] / 10) * 16 + bytNHdate[0] % 10;
    bytNHdate[1] = (tplocaltime->tm_year + 1900) % 100;
    bytNHdate[1] = (bytNHdate[1] / 10) * 16 + bytNHdate[1] % 10;
    bytNHdate[2] = tplocaltime->tm_mon + 1;
    bytNHdate[2] = (bytNHdate[2] / 10) * 16 + bytNHdate[2] % 10;
    bytNHdate[3] = tplocaltime->tm_mday;
    bytNHdate[3] = (bytNHdate[3] / 10) * 16 + bytNHdate[3] % 10;
    for(lngHdayNum = 0; lngHdayNum < tpArraybound.tpHoliday4001; lngHdayNum++)
    {
        if((memcmp(bytHdate, &tpHoliday4001[lngHdayNum].lngStartHoliday, 4) == 0) ||
           ((memcmp(bytNHdate, &tpHoliday4001[lngHdayNum].lngStartHoliday, 4) == 0) && (lngMinute < lngStayTime)))
            return 0xff;
    }*/
    return 0;
}

/*
*/
unsigned char binFileManage(unsigned char chcmd, unsigned char *cmd_buf, int cmd_len, unsigned char *out_buf, int *out_len)
{
unsigned char chEodnum, chFramelen;
unsigned char chret, filename[100], cache_file[150], oldfilename[20];
FILE	*fl;
unsigned long lngbyte4, i;
unsigned short shbyte2, mode_days;
char  *filebuf, filepath[200], chfile[40], chprefix[20];
int	ret;
unsigned char outlen;
char shaRes[20];

#ifdef _DEBUG_PRINT_
	debug_printf("\nbinFileManage %s",chcmd);
#endif

	switch(chcmd)
	{
	case 0x11:
		//set station mode
		ByteToLong((long *)&lngbyte4, &cmd_buf[7]);
		tpStationWaiverMode.waivermode_len = lngbyte4;
		if(tpStationWaiverMode.waivermode_val != NULL)
		{
			free(tpStationWaiverMode.waivermode_val);
			tpStationWaiverMode.waivermode_val = NULL;
		}
		if(tpStationWaiverMode.waivermode_len != 0)
			tpStationWaiverMode.waivermode_val = (unsigned char *)malloc((long)tpStationWaiverMode.waivermode_len * 5);
		if((tpStationWaiverMode.waivermode_val == NULL) && (tpStationWaiverMode.waivermode_len != 0))
		{
			chret = CE_UNKNOWN;
			*out_len = 0;
		}else
		{
			if(tpStationWaiverMode.waivermode_len != 0)
				memcpy(tpStationWaiverMode.waivermode_val, &cmd_buf[11], (long)tpStationWaiverMode.waivermode_len * 5);
			/*fl = fopen("./para/waivermode", "w+");
				fwrite(&tpStationWaiverMode.waivermode_len, 1, 2, fl);
				fwrite(tpStationWaiverMode.waivermode_val, 1, (long)tpStationWaiverMode.waivermode_len * 3, fl);
			fclose(fl); */
			reader_status = KM_RW_IDLE;
			//check whether the not uploaded record or not
			if((para_type = ee_read_last_record(0x6002, out_buf, &outlen, 0)) != 0)
			{
				reader_status = KM_RW_RECORD;
			}
			chret = CE_OK;
			*out_len = 0;
		}
		break;
	case 0x09:
		//get station mode information
		*out_len = 0;
		/*
		for(i = 0; i < EodWaiverDateMasterConfig.StationModeInfo.StationModeInfo_len; i++)
		{
			ByteToShort((short *)&shbyte2, &cmd_buf[13]);
			ByteToShort((short *)&mode_days, &cmd_buf[15]);
			if((shbyte2 == EodWaiverDateInfo[i].StationID)
			   && (mode_days == second2days(EodWaiverDateInfo[i].ModeStartDateTime.Time_HI, EodWaiverDateInfo[i].ModeStartDateTime.Time_LO, NULL)))
			{
				*out_len = 1;
				out_buf[0] = EodWaiverDateInfo[i].ModeCode;
				chret = ERR_OK;
				break;
			}
			
		}
		if(i >= EodWaiverDateMasterConfig.StationModeInfo.StationModeInfo_len)
			chret = ERR_NOPARAMETER;
		*/
		break;
	case 0x60://get the EOD version
		if(cmd_buf[7] == 0)
		{
			chEodnum = 0;
			get_cur_para_ver(0x3082, &chEodnum, tpPubTitle3082.lngTableVersion, out_buf);
			get_cur_para_ver(0x4001, &chEodnum, tpPubTitle4001.lngTableVersion, out_buf);
			get_cur_para_ver(0x4002, &chEodnum, tpPubTitle4002.lngTableVersion, out_buf);
			get_cur_para_ver(0x4003, &chEodnum, tpPubTitle4003.lngTableVersion, out_buf);
			get_cur_para_ver(0x4004, &chEodnum, tpPubTitle4004.lngTableVersion, out_buf);
			get_cur_para_ver(0x4006, &chEodnum, tpPubTitle4006.lngTableVersion, out_buf);
			get_cur_para_ver(0x4009, &chEodnum, tpPubTitle4009.lngTableVersion, out_buf);
			get_cur_para_ver(0x4015, &chEodnum, tpPubTitle4015.lngTableVersion, out_buf);
		}else
		{
			chEodnum = 0;
			get_temp_para_ver("3082.", 0x3082, &chEodnum, 0, out_buf);
			get_temp_para_ver("4001.", 0x4001, &chEodnum, 0, out_buf);
			get_temp_para_ver("4002.", 0x4002, &chEodnum, 0, out_buf);
			get_temp_para_ver("4003.", 0x4003, &chEodnum, 0, out_buf);
			get_temp_para_ver("4004.", 0x4004, &chEodnum, 0, out_buf);
			get_temp_para_ver("4006.", 0x4006, &chEodnum, 0, out_buf);
			get_temp_para_ver("4009.", 0x4009, &chEodnum, 0, out_buf);
			get_temp_para_ver("4015.", 0x4015, &chEodnum, 0, out_buf);
		}	
		if(chEodnum > 0)
		{
			out_buf[0] = chEodnum;
			chret = CE_OK;
			*out_len = chEodnum * 6 + 1;
		}else
		{
			chret = CE_NO_FILE;
			out_buf[0] = 0;
			*out_len = 1;
		}
		break;
	case 0x61:		//deactive the EOD file
		chEodnum = 0;
		get_temp_para_ver("3082.", 0x3082, &chEodnum, 1, out_buf);
		get_temp_para_ver("4001.", 0x4001, &chEodnum, 1, out_buf);
		get_temp_para_ver("4002.", 0x4002, &chEodnum, 1, out_buf);
		get_temp_para_ver("4003.", 0x4003, &chEodnum, 1, out_buf);
		get_temp_para_ver("4004.", 0x4004, &chEodnum, 1, out_buf);
		get_temp_para_ver("4006.", 0x4006, &chEodnum, 1, out_buf);
		get_temp_para_ver("4009.", 0x4009, &chEodnum, 1, out_buf);
		get_temp_para_ver("4015.", 0x4015, &chEodnum, 1, out_buf);

		out_buf[0] = chEodnum;
		chret = CE_OK;
		*out_len = chEodnum * 6 + 1;
		break;
	case 0x62:		//active the EOD file
		chEodnum = 0;
		ret = active_eod_file("3082.", (long *)&lngbyte4, 9);
		if(255 == ret)
		{
			ShortToByte(0x3082, &out_buf[0]);
			LongToByte(lngbyte4, &out_buf[2]);
			*out_len = 6;
			chret = CE_MD5;
			break;
		}else if (0 == ret)
		{
			ShortToByte(0x3082, &out_buf[chEodnum * 6 + 1]);
			LongToByte(lngbyte4, &out_buf[chEodnum * 6 + 3]);
			chEodnum += 1;
			EOD_3082_ExchangeStation();
		}
		ret = active_eod_file("4001.", (long *)&lngbyte4, 9);
		if(255 == ret)
		{
			ShortToByte(0x4001, &out_buf[0]);
			LongToByte(lngbyte4, &out_buf[2]);
			*out_len = 6;
			chret = CE_MD5;
			break;
		}else if (0 == ret)
		{
			ShortToByte(0x4001, &out_buf[chEodnum * 6 + 1]);
			LongToByte(lngbyte4, &out_buf[chEodnum * 6 + 3]);
			chEodnum += 1;
			EOD_4001_Holiday();
		}
		ret = active_eod_file("4002.", (long *)&lngbyte4, 9);
		if(255 == ret)
		{
			ShortToByte(0x4002, &out_buf[0]);
			LongToByte(lngbyte4, &out_buf[2]);
			*out_len = 6;
			chret = CE_MD5;
			break;
		}else if (0 == ret)
		{
			ShortToByte(0x4002, &out_buf[chEodnum * 6 + 1]);
			LongToByte(lngbyte4, &out_buf[chEodnum * 6 + 3]);
			chEodnum += 1;
			EOD_4002_TicketSet();
		}
		ret = active_eod_file("4003.", (long *)&lngbyte4, 9);
		if(255 == ret)
		{
			ShortToByte(0x4003, &out_buf[0]);
			LongToByte(lngbyte4, &out_buf[2]);
			*out_len = 6;
			chret = CE_MD5;
			break;
		}else if (0 == ret)
		{
			ShortToByte(0x4003, &out_buf[chEodnum * 6 + 1]);
			LongToByte(lngbyte4, &out_buf[chEodnum * 6 + 3]);
			chEodnum += 1;
			EOD_4003_FareValue();
		}
		ret = active_eod_file("4004.", (long *)&lngbyte4, 9);
		if(255 == ret)
		{
			ShortToByte(0x4004, &out_buf[0]);
			LongToByte(lngbyte4, &out_buf[2]);
			*out_len = 6;
			chret = CE_MD5;
			break;
		}else if (0 == ret)
		{
			ShortToByte(0x4004, &out_buf[chEodnum * 6 + 1]);
			LongToByte(lngbyte4, &out_buf[chEodnum * 6 + 3]);
			chEodnum += 1;
			EOD_4004_StationFare();
		}
		ret = active_eod_file("4006.", (long *)&lngbyte4, 9);
		if(255 == ret)
		{
			ShortToByte(0x4006, &out_buf[0]);
			LongToByte(lngbyte4, &out_buf[2]);
			*out_len = 6;
			chret = CE_MD5;
			break;
		}else if (0 == ret)
		{
			ShortToByte(0x4006, &out_buf[chEodnum * 6 + 1]);
			LongToByte(lngbyte4, &out_buf[chEodnum * 6 + 3]);
			chEodnum += 1;
			EOD_4006_NotPeak();
		}
		ret = active_eod_file("4009.", (long *)&lngbyte4, 9);
		if(255 == ret)
		{
			ShortToByte(0x4009, &out_buf[0]);
			LongToByte(lngbyte4, &out_buf[2]);
			*out_len = 6;
			chret = CE_MD5;
			break;
		}else if (0 == ret)
		{
			ShortToByte(0x4009, &out_buf[chEodnum * 6 + 1]);
			LongToByte(lngbyte4, &out_buf[chEodnum * 6 + 3]);
			chEodnum += 1;
			EOD_4009_TicketMap();
		}
		ret = active_eod_file("4015.", (long *)&lngbyte4, 9);
		if(255 == ret)
		{
			ShortToByte(0x4015, &out_buf[0]);
			LongToByte(lngbyte4, &out_buf[2]);
			*out_len = 6;
			chret = CE_MD5;
			break;
		}else if (0 == ret)
		{
			ShortToByte(0x4015, &out_buf[chEodnum * 6 + 1]);
			LongToByte(lngbyte4, &out_buf[chEodnum * 6 + 3]);
			chEodnum += 1;
			EOD_4015_CMCCMap();
		}
		chret = CE_OK;
		out_buf[0] = chEodnum;
		*out_len = chEodnum * 6 + 1;
		break;
	case 0x63:		//get the TP version
		memset(&out_buf[0], 0x00, 9);
		sprintf(&out_buf[0], "HHJT");
		memcpy(&out_buf[9], "20111024", 4);
		LongToByte(tp_ver, &out_buf[13]);
		chret = CE_OK;
		*out_len = 4;
		break;
	case 0x64:		//delete the TP file
		if(0 == FileisExist("./paranew/", "shanghai", filename))
		{
			sprintf(filepath, "./paranew/%s", filename);
			remove(filepath);
		}
		chret = CE_OK;
		*out_len = 0;
		break;
	case 0xC5:		//station mode list version
		LongToByte(wdmc_ver, &out_buf[0]);
		chret = 21;
		*out_len = 4;
		break;
	case 0xC6:		//active station mode list
		*out_len = 4;
		ret = active_eod_file("WAIVERDATEMASTERCONFIGFILE.", (long *)&lngbyte4, 27);
		LongToByte(lngbyte4, &out_buf[0]);
		if(255 == ret)
		{
			chret = CE_MD5;
		}else if (ret)
		{
			chret = CE_NO_FILE;
		}else
		{
			//xdr_file_waiverdate();
			chret = CE_OK;
		}
		break;
	case 0x66:		//download the EOD file and TP program.
		ByteToShort((short *)&shbyte2, &cmd_buf[4]);	//current frame number
		chFramelen = cmd_buf[6];
		if(shbyte2 == 1)		//first frame
		{
			//eod_download.filelen = ByteToLong((long *)&lngbyte4, &cmd_buf[50]);
			//memset(eod_download.filename, 0x00, 41);
			//memcpy(eod_download.filename, &cmd_buf[10], 40);
			//memcpy(eod_download.md5, &cmd_buf[54], 16);
			eod_download.curFrame = shbyte2;
			eod_download.curFilelen = 0;
			eod_download.filetype = 1;
			ByteToShort((short *)&eod_download.totalFrame, &cmd_buf[2]);
			if(eod_download.filetype == 0)
			{
				eod_download.filename[0] = toupper(eod_download.filename[0]);
				eod_download.filename[1] = toupper(eod_download.filename[1]);
				eod_download.filename[2] = toupper(eod_download.filename[2]);
				sprintf(eod_download.filepath, "./cache/%s", eod_download.filename);
			}else if(eod_download.filetype == 1)
			{
				sprintf(eod_download.filename, "shanghai");
				sprintf(eod_download.filepath, "./prognew/%s", eod_download.filename);
			}else if(eod_download.filetype == 2)
			{
				for(i = 0; i < 26; i++) eod_download.filename[i] = toupper(eod_download.filename[i]);
				sprintf(eod_download.filepath, "./cache/%s", eod_download.filename);
			}else
			{
				chret = CE_BADCOMMAND;
				memcpy(&out_buf[0], &cmd_buf[3], 2);
				memcpy(&out_buf[2], &cmd_buf[1], 2);
				*out_len = 4;
				break;
			}
			//first check whether the file name in the cache direcotry or not or delete it
			memset(cache_file, 0x00, 150);
			memset(filename, 0x00, 100);
			memcpy(cache_file, eod_download.filename, 9);
			if(0 == FileisExist("./cache/", cache_file, filename))
			{
				sprintf(cache_file, "./cache/%s", filename);
				remove(cache_file);
			}
			fl = fopen(eod_download.filepath, "w+");
			fseek(fl, 0, SEEK_SET);
			fwrite(&cmd_buf[7], 1, chFramelen , fl);
			fclose(fl);
			eod_download.curFilelen = chFramelen;
			chret = CE_OK;
			memcpy(&out_buf[0], &cmd_buf[2], 2);
			memcpy(&out_buf[2], &cmd_buf[4], 2);
			*out_len = 4;
#ifdef DEBUG_PRINT			
			printf("\nfirst frame filelen:%d totalframe:%d curframe:%d filename:%s\n", eod_download.filelen, eod_download.totalFrame, shbyte2, eod_download.filename);
#endif
		}else if (shbyte2 != eod_download.totalFrame)
		{
#ifdef DEBUG_PRINT			
			printf("\ncontinuous frame frame:%d totalframe:%d curframe:%d filename:%s\n", shbyte2, eod_download.totalFrame, eod_download.curFrame, eod_download.filename);
#endif
			//first check the frame continuous
			if(shbyte2 != eod_download.curFrame + 1)
			{
				chret = CE_CONTINUE_MULTIFRAME;	//not continuous frame
				memcpy(&out_buf[0], &cmd_buf[2], 2);
				memcpy(&out_buf[2], &cmd_buf[4], 2);
				*out_len = 4;
				break;
			}
			eod_download.curFrame = shbyte2;
			//check the total frame number
			/*ByteToShort((short *)&shbyte2, &cmd_buf[4]);
			if(shbyte2 != eod_download.totalFrame)
			{
				chret = CE_TOTAL_FRAME;	//not continuous frame
				memcpy(&out_buf[0], &cmd_buf[2], 2);
				memcpy(&out_buf[2], &cmd_buf[4], 2);
				*out_len = 4;
				break;
			}*/
			//record the file and it's length, 
			eod_download.curFilelen += chFramelen;
			//sprintf(filepath, "./prognew/%s", eod_download.filename);
			fl = fopen(eod_download.filepath, "a+");
			fseek(fl, 0, SEEK_END);
			fwrite(&cmd_buf[7], 1, chFramelen , fl);
			fclose(fl);
			chret = CE_OK;
			memcpy(&out_buf[0], &cmd_buf[2], 2);
			memcpy(&out_buf[2], &cmd_buf[4], 2);
			*out_len = 4;
		}else
		{
#ifdef DEBUG_PRINT			
			printf("\ncontinuous++ frame frame:%d totalframe:%d curframe:%d filename:%s\n", shbyte2, eod_download.totalFrame, eod_download.curFrame, eod_download.filename);
#endif
			//first check the frame continuous
			if(shbyte2 != eod_download.curFrame + 1)
			{
				chret = CE_CONTINUE_MULTIFRAME;	//not continuous frame
				memcpy(&out_buf[0], &cmd_buf[2], 2);
				memcpy(&out_buf[2], &cmd_buf[4], 2);
				*out_len = 4;
				break;
			}
			//check the file length
			/*eod_download.curFilelen += (chFramelen - 1);
			if(eod_download.filelen != eod_download.curFilelen)
			{
				chret = CE_FILE_LENGTH;
				memcpy(&out_buf[0], &cmd_buf[3], 2);
				memcpy(&out_buf[2], &cmd_buf[1], 2);
				*out_len = 4;
				break;
			}*/
			//sprintf(filepath, "./prognew/%s", eod_download.filename);
			fl = fopen(eod_download.filepath, "a+");
				fseek(fl, 0, SEEK_END);
				fwrite(&cmd_buf[7], 1, chFramelen , fl);
			fclose(fl);
			//check the md5
			/*fl = fopen(eod_download.filepath, "r+");
				fseek(fl, 0, SEEK_END);
				lngbyte4 = ftell(fl);
				fseek(fl, 0, SEEK_SET);
				filebuf = (char *)malloc(lngbyte4);
				fread(filebuf, 1, lngbyte4, fl);
			md5_str((unsigned char *)filebuf, lngbyte4, out_buf);
			free(filebuf);
			if(memcmp(eod_download.md5, out_buf, 16) != 0)
			{
				chret = CE_MD5;
				memcpy(&out_buf[0], &cmd_buf[3], 2);
				memcpy(&out_buf[2], &cmd_buf[1], 2);
				*out_len = 4;
				break;
			}*/
			//new file ok
			/*if(eod_download.filetype == SZ_DOWNLOAD_TP)
			{
				fseek(fl, 0, SEEK_END);
				fwrite(eod_download.md5, 1, 16, fl);
			}
			fclose(fl);*/
			memset(chprefix, 0x00, 20);
			memcpy(chprefix, eod_download.filename, 9);
			if(0 == FileisExist("./paranew/", chprefix, chfile))
			{
				sprintf(filepath, "./paranew/%s", chfile);
				remove(filepath);
			}
			sprintf(filepath, "./paranew/%s", eod_download.filename);
			if(eod_download.filetype != SZ_DOWNLOAD_TP)
				rename(eod_download.filepath, filepath); 
			chret = CE_OK;
			memcpy(&out_buf[0], &cmd_buf[2], 2);
			memcpy(&out_buf[2], &cmd_buf[4], 2);
			*out_len = 4;
		}
		break;
	case 0x65:
		chret = CE_MD5;
		*out_len = 0;
		if (0 != FileisExist("./prognew/", NULL, filename))
			break;
		sprintf(filepath, "./prognew/%s", filename);
		fl = fopen(filepath, "r+");
			fseek(fl, 0, SEEK_END);
			lngbyte4 = ftell(fl);
			if((lngbyte4 - 16) <= 0)
			{
				fclose(fl);
				return chret;
			}
			fseek(fl, 0, SEEK_SET);
			filebuf = (char *)malloc(lngbyte4);
			fread(filebuf, 1, lngbyte4, fl);
		fclose(fl);
		//md5_str((unsigned char *)filebuf, lngbyte4 - 16, out_buf);
		//if(memcmp(out_buf, &filebuf[lngbyte4 - 16], 16) == 0)
		{
			chret = CE_OK;
			*out_len = 0;
			active_tp_file(filename, NULL);
		}
		free(filebuf);
		break;
		
	case 0x99:		//4016
		shbyte2 = cmd_buf[2];
		chFramelen = cmd_buf[3];
		printf("shbyte2= %d",shbyte2);
		//memset(eod_download.filename,0,sizeof(eod_download.filename));
		if(shbyte2 == 0)		//first frame
		{
			memset(eod_download.filename,0,sizeof(eod_download.filename));
			printf("shbyte2= %d",shbyte2);
			eod_download.curFrame = shbyte2;
			eod_download.curFilelen = 0;
			eod_download.filetype = 1;
			//ByteToShort((short *)&eod_download.totalFrame, &cmd_buf[2]);
			if(eod_download.filetype == 0)
			{
				eod_download.filename[0] = toupper(eod_download.filename[0]);
				eod_download.filename[1] = toupper(eod_download.filename[1]);
				eod_download.filename[2] = toupper(eod_download.filename[2]);
				sprintf(eod_download.filepath, "./%s", eod_download.filename);
			}else if(eod_download.filetype == 1)
			{
				sprintf(eod_download.filename, "5016");
				sprintf(eod_download.filepath, "./%s", eod_download.filename);
			}else if(eod_download.filetype == 2)
			{
				for(i = 0; i < 26; i++) eod_download.filename[i] = toupper(eod_download.filename[i]);
				sprintf(eod_download.filepath, "./%s", eod_download.filename);
			}else
			{
				chret = CE_BADCOMMAND;
				out_buf[0]= 2;
				*out_len = 1;
				break;
			}
			//first check whether the file name in the cache direcotry or not or delete it
			memset(cache_file, 0x00, 150);
			memset(filename, 0x00, 100);
			memcpy(cache_file, eod_download.filename, 9);
			if(0 == FileisExist("./", cache_file, filename))
			{
				sprintf(cache_file, "./%s", filename);
				remove(cache_file);
			}
			fl = fopen(eod_download.filepath, "w+");
			fseek(fl, 0, SEEK_SET);
			fwrite(&cmd_buf[4], 1, chFramelen , fl);
			
//			printf("\n");
//			for(i = 0;i < 250;i++)
//				printf(" %02X",cmd_buf[4+i]);
			
			fclose(fl);
			eod_download.curFilelen = chFramelen;
			chret = CE_OK;
			out_buf[0]= 0;
			*out_len = 1;
		
#ifdef DEBUG_PRINT			
			printf("\nfirst frame filelen:%d  curframe:%d filename:%s\n", eod_download.filelen, shbyte2, eod_download.filename);
#endif
		}//else if (shbyte2 != eod_download.totalFrame)
			else if ((shbyte2 & 0x80 ) == 0)
		{
#ifdef DEBUG_PRINT			
			printf("\ncontinuous frame frame:%d  curframe:%d filename:%s\n", shbyte2, eod_download.curFrame, eod_download.filename);
#endif
			//first check the frame continuous
			if(shbyte2 != eod_download.curFrame + 1)
			{
				chret = CE_CONTINUE_MULTIFRAME;	//not continuous frame
				out_buf[0]= 0x02;
				*out_len = 1;
				break;
			}
			eod_download.curFrame = shbyte2;
			eod_download.curFilelen += chFramelen;
			//sprintf(filepath, "./prognew/%s", eod_download.filename);
			fl = fopen(eod_download.filepath, "a+");
			fseek(fl, 0, SEEK_END);
			fwrite(&cmd_buf[4], 1, chFramelen , fl);			
			fclose(fl);   
			//
//			printf("\n");                  
//			for(i = 0;i < 250;i++)         
//				printf(" %02X",cmd_buf[4+i]);
				
			chret = CE_OK;
			out_buf[0]= 0;
			*out_len = 1;
		}else
		{
#ifdef DEBUG_PRINT			
			printf("\ncontinuous++ frame frame:%d  curframe:%d filename:%s\n", shbyte2,  eod_download.curFrame, eod_download.filename);
#endif

			if((shbyte2 | 0x80) == 0x80)		//判断第一包数据是否为最后一包
			{
					sprintf(eod_download.filename, "5016");
					sprintf(eod_download.filepath, "./%s", eod_download.filename);
						
					memset(cache_file, 0x00, 150);
					memset(filename, 0x00, 100);
					memcpy(cache_file, eod_download.filename, 9);
					if(0 == FileisExist("./", cache_file, filename))
					{
						sprintf(cache_file, "./%s", filename);
						remove(cache_file);
					}
					fl = fopen(eod_download.filepath, "w+");
					fseek(fl, 0, SEEK_SET);
					fwrite(&cmd_buf[4], 1, chFramelen , fl);
					fclose(fl);
			
			}else
			{
			
					if((shbyte2 & 0x7F) != eod_download.curFrame + 1)
					{				
						chret = CE_CONTINUE_MULTIFRAME;	//not continuous frame
						out_buf[0]= 0x02;
						*out_len = 1;
						break;
					}
						fl = fopen(eod_download.filepath, "a+");
						fseek(fl, 0, SEEK_END);
						fwrite(&cmd_buf[4], 1, chFramelen , fl);
						fclose(fl);
			}
				
//				printf("\n");                  
//				for(i = 0;i < 250;i++)         
//					printf(" %02X",cmd_buf[4+i]);

				
			eod_download.curFrame = (shbyte2 & 0x7F);
			chret = CE_OK;
			
			//#ifdef _DEBUG_BOM_
				debug_printf("\n4016 loadDown end...");
			//#endif
			
				//4016
			shmt_read_pf4016(eod_download.filename);
			#ifdef _DEBUG_BOM_
				debug_printf("\nshmt_read_pf4016 end2...");
			#endif
			ret = pf_mobile_ticket_para_sha1(temp_4016);
			if(ret != 0)
			{
				//ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x11", 3);
				debug_printf("\n4016 sha1 failure...");
				out_buf[0] = 0x03;
				*out_len = 1;
				
				sprintf(eod_download.filename, "4016");
				//sprintf(filepath, "./%s", eod_download.filename);
				shmt_read_pf4016(eod_download.filename);
				pf_mobile_ticket_para_init(temp_4016);
				memcpy(buf_4016,temp_4016,4);
			}else if(ret == 0)
				{
				//ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x12", 3);
				//memset(eod_download.filename,0,41);	
				sprintf(eod_download.filename, "4016");
				//memset(chprefix, 0x00, 20);
				memset(chfile, 0x00, 20);
				memset(chprefix, 0x00, 20);
				//memset(filepath,0x00,20);
				//memcpy(chprefix, oldfilename, 4);
				memcpy(chprefix, eod_download.filename, 4);
				
				if(0 == FileisExist("./", chprefix, chfile))
				{
					sprintf(filepath, "./%s", chfile);
					if(remove(filepath) != 0)
					{
						debug_printf("\nsremove 4016 Err!\n");					
					} 
					for(ret=0;ret<10;ret++)
							debug_printf("\n%02X ",filepath[ret]); 
				} 
				//sprintf(eod_download.filepath, "./%s", eod_download.filename); 
				sprintf(filepath, "./%s", eod_download.filename);
				//if(eod_download.filetype != SZ_DOWNLOAD_TP)
				rename(eod_download.filepath, filepath); 
				shmt_read_pf4016(eod_download.filename);
				pf_mobile_ticket_para_init(temp_4016);
				memcpy(buf_4016,temp_4016,4);	//提取正确的4016版本号
				out_buf[0]= 0;
				*out_len = 1;
			}
		} 

		break;
	case 0x88:		//4020
		shbyte2 = cmd_buf[2];
		chFramelen = cmd_buf[3];
		//memset(eod_download.filename,0,sizeof(eod_download.filename));
		if(shbyte2 == 0)		//first frame
		{
			memset(eod_download.filename,0,sizeof(eod_download.filename));
			printf("shbyte2= %d",shbyte2);
			eod_download.curFrame = shbyte2;
			eod_download.curFilelen = 0;
			eod_download.filetype = 1;
			//ByteToShort((short *)&eod_download.totalFrame, &cmd_buf[2]);
			if(eod_download.filetype == 0)
			{
				eod_download.filename[0] = toupper(eod_download.filename[0]);
				eod_download.filename[1] = toupper(eod_download.filename[1]);
				eod_download.filename[2] = toupper(eod_download.filename[2]);
				sprintf(eod_download.filepath, "./%s", eod_download.filename);
			}else if(eod_download.filetype == 1)
			{
				sprintf(eod_download.filename, "5020");
				sprintf(eod_download.filepath, "./%s", eod_download.filename);
			}else if(eod_download.filetype == 2)
			{
				for(i = 0; i < 26; i++) eod_download.filename[i] = toupper(eod_download.filename[i]);
				sprintf(eod_download.filepath, "./%s", eod_download.filename);
			}else
			{
				chret = CE_BADCOMMAND;
				out_buf[0]= 2;
				*out_len = 1;
				break;
			}
			//first check whether the file name in the cache direcotry or not or delete it
			memset(cache_file, 0x00, 150);
			memset(filename, 0x00, 100);
			memcpy(cache_file, eod_download.filename, 9);
			if(0 == FileisExist("./", cache_file, filename))
			{
				sprintf(cache_file, "./%s", filename);
				remove(cache_file);
			}
			fl = fopen(eod_download.filepath, "w+");
			fseek(fl, 0, SEEK_SET);
			fwrite(&cmd_buf[4], 1, chFramelen , fl);
			
//			printf("\n");
//			for(i = 0;i < 250;i++)
//				printf(" %02X",cmd_buf[4+i]);
			
			fclose(fl);
			eod_download.curFilelen = chFramelen;
			chret = CE_OK;
			out_buf[0]= 0;
			*out_len = 1;
		
#ifdef DEBUG_PRINT			
			printf("\nfirst frame filelen:%d  curframe:%d filename:%s\n", eod_download.filelen, shbyte2, eod_download.filename);
#endif
		}//else if (shbyte2 != eod_download.totalFrame)
		else if ((shbyte2 & 0x80 ) == 0)
		{
#ifdef DEBUG_PRINT			
			printf("\ncontinuous frame frame:%d  curframe:%d filename:%s\n", shbyte2, eod_download.curFrame, eod_download.filename);
#endif
			//first check the frame continuous
			if(shbyte2 != eod_download.curFrame + 1)//判断当前包是否连续
			{
				chret = CE_CONTINUE_MULTIFRAME;	//not continuous frame
				out_buf[0]= 0x02;
				*out_len = 1;
				break;
			}
			eod_download.curFrame = shbyte2;
			eod_download.curFilelen += chFramelen;
			//sprintf(filepath, "./prognew/%s", eod_download.filename);
			fl = fopen(eod_download.filepath, "a+");
			fseek(fl, 0, SEEK_END);
			fwrite(&cmd_buf[4], 1, chFramelen , fl);			
			fclose(fl);   
			//
//			printf("\n");                  
//			for(i = 0;i < 250;i++)         
//				printf(" %02X",cmd_buf[4+i]);
				
			chret = CE_OK;
			out_buf[0]= 0;
			*out_len = 1;
		}else
		{
#ifdef DEBUG_PRINT			
			printf("\ncontinuous++ frame frame:%d  curframe:%d filename:%s\n", shbyte2,  eod_download.curFrame, eod_download.filename);
#endif

			if((shbyte2 | 0x80) == 0x80)		//判断第一包数据是否为最后一包
			{
					sprintf(eod_download.filename, "5020");
					sprintf(eod_download.filepath, "./%s", eod_download.filename);
						
					memset(cache_file, 0x00, 150);
					memset(filename, 0x00, 100);
					memcpy(cache_file, eod_download.filename, 9);
					if(0 == FileisExist("./", cache_file, filename))
					{
						sprintf(cache_file, "./%s", filename);
						remove(cache_file);
					}
					fl = fopen(eod_download.filepath, "w+");
					fseek(fl, 0, SEEK_SET);
					fwrite(&cmd_buf[4], 1, chFramelen , fl);
					fclose(fl);
			
			}else
			{
			
					if((shbyte2 & 0x7F) != eod_download.curFrame + 1)//判断最后一包是否连续
					{				
						chret = CE_CONTINUE_MULTIFRAME;	//not continuous frame
						out_buf[0]= 0x02;
						*out_len = 1;
						break;
					}
					fl = fopen(eod_download.filepath, "a+");
					fseek(fl, 0, SEEK_END);
					fwrite(&cmd_buf[4], 1, chFramelen , fl);
					fclose(fl);
			}
				
//				printf("\n");                  
//				for(i = 0;i < 250;i++)         
//					printf(" %02X",cmd_buf[4+i]);

				
			eod_download.curFrame = (shbyte2 & 0x7F);
			chret = CE_OK;
			
			#ifdef _DEBUG_BOM_
				debug_printf("\n4020 loadDown end...");
			#endif
			
				//4020
			//shmt_read_4020(eod_download.filename);
			#ifdef _DEBUG_BOM_
				debug_printf("\nshmt_read_4020 end2...");
			#endif
			//ret = pf_mobile_ticket_para_sha1(temp_4020);
			fl = fopen(eod_download.filepath, "rb");
				fseek(fl, 0, SEEK_END);
				lngbyte4 = ftell(fl);;
				filebuf = (char *) malloc(lngbyte4);
				if( filebuf == NULL)
				{
					fclose(fl);
					out_buf[0] = 0x03;
					*out_len = 1;
					free(filebuf);
					break;
				}
				fseek(fl, 0, 0);
				fread(filebuf, 1, lngbyte4, fl);
			fclose(fl);
			sha(filebuf+28, lngbyte4-28, shaRes);
			ret = memcmp(shaRes, &filebuf[8], 20);
			free(filebuf);
			if(ret != 0)
			{
				//ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x11", 3);
				
				chret = out_buf[0] = 0x03;
				*out_len = 1;
				
				//sprintf(eod_download.filename, "4020");
				//sprintf(filepath, "./%s", eod_download.filename);
				//shmt_read_4020(eod_download.filename);
				//pf_mobile_ticket_para_init(temp_4020);
				//memcpy(buf_4020,temp_4020,4);
			}else if(ret == 0)
			{
				//ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x12", 3);
				//memset(eod_download.filename,0,41);	
				sprintf(eod_download.filename, "4020");
				//memset(chprefix, 0x00, 20);
				memset(chfile, 0x00, 20);
				memset(chprefix, 0x00, 20);
				//memset(filepath,0x00,20);
				//memcpy(chprefix, oldfilename, 4);
				memcpy(chprefix, eod_download.filename, 4);
				
				if(0 == FileisExist("./", chprefix, chfile))
				{
					sprintf(filepath, "./%s", chfile);
					if(remove(filepath) != 0)
					{
						debug_printf("\nsremove 4020 Err!\n");					
					} 
					for(ret=0;ret<10;ret++)
							debug_printf("\n%02X ",filepath[ret]); 
				} 
				//sprintf(eod_download.filepath, "./%s", eod_download.filename); 
				sprintf(filepath, "./%s", eod_download.filename);
				//if(eod_download.filetype != SZ_DOWNLOAD_TP)
				rename(eod_download.filepath, filepath); 
				//shmt_read_4020(eod_download.filename);
				EOD_4020_WhiteList();
				//pf_mobile_ticket_para_init(temp_4020);
				//memcpy(buf_4020,temp_4020,4);	//提取正确的4020版本号
				out_buf[0]= 0;
				*out_len = 1;
			}
		} 
		break;
	case 0x68:		//4026
		shbyte2 = cmd_buf[2];
		chFramelen = cmd_buf[3];
		//memset(eod_download.filename,0,sizeof(eod_download.filename));
		if(shbyte2 == 0)		//first frame
		{
			memset(eod_download.filename,0,sizeof(eod_download.filename));
			printf("shbyte2= %d",shbyte2);
			eod_download.curFrame = shbyte2;
			eod_download.curFilelen = 0;
			eod_download.filetype = 1;
			//ByteToShort((short *)&eod_download.totalFrame, &cmd_buf[2]);
			if(eod_download.filetype == 0)
			{
				eod_download.filename[0] = toupper(eod_download.filename[0]);
				eod_download.filename[1] = toupper(eod_download.filename[1]);
				eod_download.filename[2] = toupper(eod_download.filename[2]);
				sprintf(eod_download.filepath, "./%s", eod_download.filename);
			}else if(eod_download.filetype == 1)
			{
				sprintf(eod_download.filename, "5026");
				sprintf(eod_download.filepath, "./%s", eod_download.filename);
			}else if(eod_download.filetype == 2)
			{
				for(i = 0; i < 26; i++) eod_download.filename[i] = toupper(eod_download.filename[i]);
				sprintf(eod_download.filepath, "./%s", eod_download.filename);
			}else
			{
				chret = CE_BADCOMMAND;
				out_buf[0]= 2;
				*out_len = 1;
				break;
			}
			//first check whether the file name in the cache direcotry or not or delete it
			memset(cache_file, 0x00, 150);
			memset(filename, 0x00, 100);
			memcpy(cache_file, eod_download.filename, 9);
			if(0 == FileisExist("./", cache_file, filename))
			{
				sprintf(cache_file, "./%s", filename);
				remove(cache_file);
			}
			fl = fopen(eod_download.filepath, "w+");
			fseek(fl, 0, SEEK_SET);
			fwrite(&cmd_buf[4], 1, chFramelen , fl);
			
//			printf("\n");
//			for(i = 0;i < 250;i++)
//				printf(" %02X",cmd_buf[4+i]);
			
			fclose(fl);
			eod_download.curFilelen = chFramelen;
			chret = CE_OK;
			out_buf[0]= 0;
			*out_len = 1;
		
#ifdef DEBUG_PRINT			
			printf("\nfirst frame filelen:%d  curframe:%d filename:%s\n", eod_download.filelen, shbyte2, eod_download.filename);
#endif
		}//else if (shbyte2 != eod_download.totalFrame)
		else if ((shbyte2 & 0x80 ) == 0)
		{
#ifdef DEBUG_PRINT			
			printf("\ncontinuous frame frame:%d  curframe:%d filename:%s\n", shbyte2, eod_download.curFrame, eod_download.filename);
#endif
			//first check the frame continuous
			if(shbyte2 != eod_download.curFrame + 1)//判断当前包是否连续
			{
				chret = CE_CONTINUE_MULTIFRAME;	//not continuous frame
				out_buf[0]= 0x02;
				*out_len = 1;
				break;
			}
			eod_download.curFrame = shbyte2;
			eod_download.curFilelen += chFramelen;
			//sprintf(filepath, "./prognew/%s", eod_download.filename);
			fl = fopen(eod_download.filepath, "a+");
			fseek(fl, 0, SEEK_END);
			fwrite(&cmd_buf[4], 1, chFramelen , fl);			
			fclose(fl);   
			//
//			printf("\n");                  
//			for(i = 0;i < 250;i++)         
//				printf(" %02X",cmd_buf[4+i]);
				
			chret = CE_OK;
			out_buf[0]= 0;
			*out_len = 1;
		}else
		{
#ifdef DEBUG_PRINT			
			printf("\ncontinuous++ frame frame:%d  curframe:%d filename:%s\n", shbyte2,  eod_download.curFrame, eod_download.filename);
#endif

			if((shbyte2 | 0x80) == 0x80)		//判断第一包数据是否为最后一包
			{
					sprintf(eod_download.filename, "5026");
					sprintf(eod_download.filepath, "./%s", eod_download.filename);
						
					memset(cache_file, 0x00, 150);
					memset(filename, 0x00, 100);
					memcpy(cache_file, eod_download.filename, 9);
					if(0 == FileisExist("./", cache_file, filename))
					{
						sprintf(cache_file, "./%s", filename);
						remove(cache_file);
					}
					fl = fopen(eod_download.filepath, "w+");
					fseek(fl, 0, SEEK_SET);
					fwrite(&cmd_buf[4], 1, chFramelen , fl);
					fclose(fl);
			
			}else
			{
			
					if((shbyte2 & 0x7F) != eod_download.curFrame + 1)//判断最后一包是否连续
					{				
						chret = CE_CONTINUE_MULTIFRAME;	//not continuous frame
						out_buf[0]= 0x02;
						*out_len = 1;
						break;
					}
					fl = fopen(eod_download.filepath, "a+");
					fseek(fl, 0, SEEK_END);
					fwrite(&cmd_buf[4], 1, chFramelen , fl);
					fclose(fl);
			}
				
//				printf("\n");                  
//				for(i = 0;i < 250;i++)         
//					printf(" %02X",cmd_buf[4+i]);

				
			eod_download.curFrame = (shbyte2 & 0x7F);
			chret = CE_OK;
			
			#ifdef _DEBUG_BOM_
				debug_printf("\n4020 loadDown end...");
			#endif
			
				//4020
			//shmt_read_4020(eod_download.filename);
			#ifdef _DEBUG_BOM_
				debug_printf("\nshmt_read_4026 end2...");
			#endif
			//ret = pf_mobile_ticket_para_sha1(temp_4020);
			fl = fopen(eod_download.filepath, "rb");
				fseek(fl, 0, SEEK_END);
				lngbyte4 = ftell(fl);;
				filebuf = (char *) malloc(lngbyte4);
				if( filebuf == NULL)
				{
					fclose(fl);
					out_buf[0] = 0x03;
					*out_len = 1;
					free(filebuf);
					break;
				}
				fseek(fl, 0, 0);
				fread(filebuf, 1, lngbyte4, fl);
			fclose(fl);
			sha(filebuf+32, lngbyte4-32, shaRes);
			ret = memcmp(shaRes, &filebuf[12], 20);        //2022.06.07  8->12
			free(filebuf);
			if(ret != 0)
			{
				chret = out_buf[0] = 0x03;
				*out_len = 1;
			}else if(ret == 0)
			{
				//ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x12", 3);
				//memset(eod_download.filename,0,41);	
				sprintf(eod_download.filename, "4026");
				//memset(chprefix, 0x00, 20);
				memset(chfile, 0x00, 20);
				memset(chprefix, 0x00, 20);
				//memset(filepath,0x00,20);
				//memcpy(chprefix, oldfilename, 4);
				memcpy(chprefix, eod_download.filename, 4);
				
				if(0 == FileisExist("./", chprefix, chfile))
				{
					sprintf(filepath, "./%s", chfile);
					if(remove(filepath) != 0)
					{
						debug_printf("\nsremove 4026 Err!\n");					
					} 
					for(ret=0;ret<10;ret++)
							debug_printf("\n%02X ",filepath[ret]); 
				} 
				//sprintf(eod_download.filepath, "./%s", eod_download.filename); 
				sprintf(filepath, "./%s", eod_download.filename);
				//if(eod_download.filetype != SZ_DOWNLOAD_TP)
				rename(eod_download.filepath, filepath); 
				//shmt_read_4020(eod_download.filename);
				EOD_4026_Cert();
				//pf_mobile_ticket_para_init(temp_4020);
				//memcpy(buf_4020,temp_4020,4);	//提取正确的4020版本号
				out_buf[0]= 0;
				*out_len = 1;
			}
		} 
		break;
	}

	return (unsigned char)chret;
}
/*
function:active the new eod file
parameter:
return:
	0:succesfully active the defined eod file
	255:the defined eod file have wrong MD5
	nonzero:the file not exist
*/
int active_eod_file(char * filename, long *lngver, char ver_len)
{
char chnewfile[50], chfile[50], newfilepath[200], filepath[200];
int ret;
char *filebuf;
long file_len, eod_ver;
FILE	*fl;

	*lngver = 0;
	ret = FileisExist("./paranew/", filename, chnewfile);
	if(ret != 0)
		return ret;
	//first check new eod file according to the md5
	sprintf(newfilepath, "./paranew/%s", chnewfile);
	//*lngver = atol(&chnewfile[ver_len]);
	fl = fopen(newfilepath, "rb");
		fread(&eod_ver, 1, sizeof(eod_ver), fl);
	fclose(fl);
	LongToByte(eod_ver, (unsigned char *)lngver);
	
	//find the current eod file and delete
	//maybe find all the same eod file and delete it.??
	ret = FileisExist("./para/", filename, chfile);
	if(ret == 0)
	{
		sprintf(filepath, "./para/%s", chfile);
		remove(filepath);
	}
	//copy the new eod file
	sprintf(filepath, "./para/%s", chnewfile);

	rename(newfilepath, filepath);
	
	return 0;
}
/*
function:active the new TP file
parameter:
return:
	0:succesfully active the defined eod file
	255:the defined eod file have wrong MD5
	nonzero:the file not exist
*/
int active_tp_file(char * filename, long *lngver)
{
FILE	*fl;
char 	temp[200], chresponse;
unsigned long 	ret;
	
	/*printf("generate the update shanghai.sh file\n");
	if(0 ==  FileisExist("./", "shanghai.sh", temp))
	{
		fl = fopen("./shanghai.sh", "w+");
		sprintf(temp, "if [ -f ./progbak/%s ]\n", filename);
		fwrite(temp, 1, strlen(temp), fl);
		sprintf(temp, "then\n");
		fwrite(temp, 1, strlen(temp), fl);
		sprintf(temp, "	echo \"\nprogram error! now back to the old version....\"\n");
		fwrite(temp, 1, strlen(temp), fl);
		sprintf(temp, "	mv ./progbak/%s .\n", filename);
		fwrite(temp, 1, strlen(temp), fl);
		sprintf(temp, "else\n");
		fwrite(temp, 1, strlen(temp), fl);
		sprintf(temp, "	if [ -f ./prognew/update ]\n");
		fwrite(temp, 1, strlen(temp), fl);
		sprintf(temp, "	then\n");
		fwrite(temp, 1, strlen(temp), fl);
		sprintf(temp, "		echo \"now update the reader application...\"\n");
		fwrite(temp, 1, strlen(temp), fl);
		sprintf(temp, "		rm ./prognew/update\n");
		fwrite(temp, 1, strlen(temp), fl);
		sprintf(temp, "		cp %s ./progbak/.\n", filename);
		fwrite(temp, 1, strlen(temp), fl);
		sprintf(temp, "		mv ./prognew/%s .\n", filename);
		fwrite(temp, 1, strlen(temp), fl);
		sprintf(temp, "	fi\n");
		fwrite(temp, 1, strlen(temp), fl);
		sprintf(temp, "fi\n");
		fwrite(temp, 1, strlen(temp), fl);
		sprintf(temp, "echo \"run the reader application\"\n");
		fwrite(temp, 1, strlen(temp), fl);
		sprintf(temp, "chmod 700 %s -R\n", filename);
		fwrite(temp, 1, strlen(temp), fl);
		sprintf(temp, "./%s /dev/ttySAC0\n", filename);
		fwrite(temp, 1, strlen(temp), fl);
		fclose(fl);
		system("chmod 700 shanghai.sh");
	}*/
	//execl("chmod", "chmod",  "-R", "700",  "*", NULL);
	if(0 == FileisExist("./", "shanghai.sh", temp))
		system("chmod 700 shanghai.sh");
	if(0 == FileisExist("./", "auto.sh", temp))
		system("chmod 700 auto.sh");
	//sleep(1);
	//watchdog_init(0, 65535);
	fl = fopen("./prognew/update", "w+");
	fclose(fl);
	//write ee
	chresponse = 1;
	if(0 != (ret = ee_write(EE_RESTART, 1, &chresponse)))
		;//printf("ee write failure %d\n", ret);
	if(0 != (ret = ee_read(EE_RESTART, 1, &chresponse)))
		;//printf("ee read failure %d\n", ret);
	//printf("active tp status %d\n", chresponse);
	exit(0);
}

/*
function:check the station ID valid
parameter:
return:
	0:succesfully find the station id in eod 01 file.
	208:the eod 01 file is not exist.
	205:the station id can't find in the eod 01 file.
*/
char check_station_id(unsigned char *station_id)
{

	if(memcmp(station_id, tpStationFare4004.bytSCID, 2) == 0)
		return 0;
	
	return CE_NOPARAM;
}

/*
function: calculate the overtime according to the fare
return:
	0:successfully get the over time(second)
	208:the eod 02 file is not exist
	205:the faretier can't be found in the eod 02 file
*/
int cal_fare_time(unsigned char faretier, unsigned long *lngovertime)
{
unsigned long 	i;

	if(faretier > 63)
		return CE_NOPARAM;
		
	*lngovertime = tpTicketSet4002.bytStayTime[faretier] * 6 * 60;
#ifdef DEBUG_PRINT
	printf("mileclass(x) %02x overtime is %d\n", faretier, *lngovertime);
#endif
	return 0;
}
/*
function:overtime
parameter:
	*entrytime:
	*curtime:
	*mileclass:
*/
char cal_overtime(unsigned char *entrytime, unsigned char *curtime, unsigned char farezone, unsigned char station_mode)
{
long lngovertime;
long lngHisecond1, lngHisecond2, lngLosecond1, lngLosecond2;
char chCode;
unsigned short i;

	//current station is set to the failure mode
	if(tpwaivermode.cur_sta_failure)
		return 0;
	//current station is set to the date mode
	if(tpwaivermode.cur_sta_date)
		return 0;
	//current station is set to the fare mode
	if(tpwaivermode.cur_sta_fare)
		return 0;
	//other station is set date or sensitive duration and DATE mode in the ticket
	//if((tpwaivermode.oth_sta_date || tpwaivermode.sen_sta_date) && (station_mode == SZ_WAIVER_DATE))
	//	return 0;
	//
	ByteToLong(&lngHisecond1, entrytime);
	ByteToLong(&lngHisecond2, curtime);

#ifdef DEBUG_PRINT
	printf("entry time %d  cur time %d \n", lngHisecond1, lngHisecond2);
#endif
	if(0 != (chCode = cal_fare_time(farezone, (unsigned long *)&lngovertime)))
		return chCode;
	
	//whether overtime or not
	if((lngHisecond2 - lngHisecond1) > lngovertime)
		return CE_OVERTIME;
		
	return 0;
}

/*
function:calculate the 
parameter:
	date time and ticket type & SP
	faretier
	price
return:
	0:successfully get the fare value
	0x53:the eod 4004 file is not exist.
	0x44:the station id can't be found in the eod 4004 file and no correctly entry .
*/
int cal_fare_value(unsigned char *srcstation_id, unsigned char *curtime, struct PARA_4002_TICKET *ticket, unsigned char *faretier, unsigned short *lngFareValue)
{
unsigned short shDays, shExtendTime;
unsigned long lngMidnightSecond, i, j, lngcursecond;
char chweekday;
unsigned char datetypeid, time_bcd[7], time_dec[7];
unsigned char AreaIndex, ExtendTime, NotpeakIndex, RebateValueIndex, CostValueIndex;
unsigned char tickettype, peak_minute, peak_min_minute, peak_max_minute;
unsigned char peak_value, peak_min_value, peak_max_value;
unsigned char faregroupid, timeintervalid;

	//
	if(tpStationFare4004sub == NULL)
		return CE_NOPARAM;
	if(tpFareValue4003 == NULL)
		return CE_NOPARAM;
	//holiday
	ByteToLong(&lngcursecond, curtime);
	long2timestr(lngcursecond, time_bcd);
	time_dec[4] = (time_bcd[4] / 16) * 10 + (time_bcd[4] % 16);
	time_dec[5] = (time_bcd[5] / 16) * 10 + (time_bcd[5] % 16);
	shDays = DAY2000 + lngcursecond / (24 * 3600);
	chweekday = DaysToWeek(shDays);
#ifdef DEBUG_PRINT
	printf("cur %02x%02x-%02x-%02x %02x:%02x:%02x week is %d \n", time_bcd[0], time_bcd[1], time_bcd[2], time_bcd[3], time_bcd[4], time_bcd[5], time_bcd[6], chweekday);
#endif	
	for(i = 0; i < tpArraybound.tpHoliday4001; i++)
	{
		if((memcmp(time_bcd, &tpHoliday4001[i].lngStartHoliday, 4) == 0))
			break;
	}
	if(i < tpArraybound.tpHoliday4001)
	{//holiday
		AreaIndex = ticket->bytHolidayAreaIndex;
		ExtendTime = ticket->bytHolidayExtend;
		NotpeakIndex = ticket->bytHolidayNotpeakCode;
		RebateValueIndex = ticket->bytHolidayRebateCode;
		CostValueIndex = ticket->bytHolidayCostCode;
	}else if((chweekday == 0) || (chweekday == 6))
	{//sunday saturday
		AreaIndex = ticket->bytWeekendAreaIndex;
		ExtendTime = ticket->bytWeekendExtend;
		NotpeakIndex = ticket->bytWeekendNotpeakCode;
		RebateValueIndex = ticket->bytWeekendRebateCode;
		CostValueIndex = ticket->bytWeekendCostCode;
	}else
	{
		AreaIndex = ticket->bytNormalAreaIndex;
		ExtendTime = ticket->bytNormalExtend;
		NotpeakIndex = ticket->bytNormalNotpeakCode;
		RebateValueIndex = ticket->bytNormalRebateCode;
		CostValueIndex = ticket->bytNormalCostCode;
	}
#ifdef DEBUG_PRINT
	printf("area %d extend %d un-peak %d rebate %d cost %d\n", AreaIndex, ExtendTime, NotpeakIndex, RebateValueIndex, CostValueIndex);
#endif
	//
	for(i = 0; i < tpStationFare4004.StationNum; i++)
	{
#ifdef DEBUG_PRINT
		printf("src station %02x%02x, stationnum %d 4004 station %02x%02x\n", srcstation_id[0], srcstation_id[1], tpStationFare4004.StationNum,
				tpStationFare4004sub[AreaIndex * tpStationFare4004.StationNum + i].bytExitSta[0], 
				tpStationFare4004sub[AreaIndex * tpStationFare4004.StationNum + i].bytExitSta[1]);
#endif
		if(memcmp(srcstation_id,  tpStationFare4004sub[AreaIndex * tpStationFare4004.StationNum + i].bytExitSta, 2) == 0)
		{
			j = AreaIndex * tpStationFare4004.StationNum + i;
			*faretier = tpStationFare4004sub[j].bytFare;
#ifdef DEBUG_PRINT
			printf("from src-station(x) %02x%02x  fare(x) %02x\n", srcstation_id[0], srcstation_id[1], *faretier);
#endif
			break;
		}
	}
	//
	if(i >= tpStationFare4004.StationNum)
		return CE_NO_ENTRY;
	//weekend
	if(NotpeakIndex == 0xff)
	{//all price
		*lngFareValue = tpFareValue4003[CostValueIndex].intFareValue[*faretier];
	}else
	{
		peak_value = tpNotPeak4006[NotpeakIndex].bytPeakArray[chweekday][time_dec[4]];
		peak_minute = (time_dec[4] % 2) * 60 + time_dec[5];
		//extend check
		shExtendTime = ExtendTime * 5;
		if(shExtendTime > 15 * 60)
			shExtendTime = 900;
		//get extent time from current time to left
		lngcursecond -= shExtendTime;
		long2timestr(lngcursecond, time_bcd);
		time_dec[4] = (time_bcd[4] / 16) * 10 + (time_bcd[4] % 16);
		time_dec[5] = (time_bcd[5] / 16) * 10 + (time_bcd[5] % 16);
		peak_min_value = tpNotPeak4006[NotpeakIndex].bytPeakArray[chweekday][time_dec[4]];
		peak_min_minute = (time_dec[4] % 2) * 60 + time_dec[5];
		//get extent time from current time to right
		if(shExtendTime == 900)
			shExtendTime -= 1;
		lngcursecond += shExtendTime;
		long2timestr(lngcursecond, time_bcd);
		time_dec[4] = (time_bcd[4] / 16) * 10 + (time_bcd[4] % 16);
		time_dec[5] = (time_bcd[5] / 16) * 10 + (time_bcd[5] % 16);
		peak_max_value = tpNotPeak4006[NotpeakIndex].bytPeakArray[chweekday][time_dec[4]];
		peak_max_minute = (time_dec[4] % 2) * 60 + time_dec[5];
		
		if((peak_value & (1 << (peak_minute / 15))) || (peak_min_value & (1 << (peak_min_minute / 15))) || (peak_max_value & (1 << (peak_max_minute / 15))))
		{//un-peak
			*lngFareValue = tpFareValue4003[RebateValueIndex].intFareValue[*faretier];
		}else
		{
			*lngFareValue = tpFareValue4003[CostValueIndex].intFareValue[*faretier];
		}
	}
#ifdef DEBUG_PRINT	
	printf("the price is %d\n", *lngFareValue);
#endif
	return 0;
}

/*
function:calculate the fare tier according to the price
parameter:
	farevalue
	fare
return:
	0:successfully get the fare value
	208:no eod 07 file
	205:parameter error
	
*/
int fast_cal_fare(unsigned short shValue, unsigned char *faretier)
{
unsigned long i;

/*	if(Eod04 == NULL)
		return 208;
	
	for(i=0; i < Eod04->ExpressIssueFare.ExpressIssueFare_len; i++) 
	{
		if(Eod04->ExpressIssueFare.ExpressIssueFare_val[i].FareAmount == (long)shValue) 
		{
			*faretier = Eod04->ExpressIssueFare.ExpressIssueFare_val[i].FareTier;
#ifdef DEBUG_PRINT
			printf("from price %d to mileclass(x) %02x\n", shValue, *faretier);
#endif
			return 0;
		}
	}
*/	return 205;
}

/*
function:get parameter the special ticket type 
parameter:
	
return:
	0:successfully get the fare value
	208:no eod 03 file
	205:parameter error
	
*/
int get_ticket_para(unsigned char chTickettype, struct PARA_4002_TICKET *td)
{
unsigned long	i;
	
	td->bytTicketCode = chTickettype;
	if(tpTicketInfo4002sub == NULL)
		return CE_NO_FILE;
	for(i = 0; i < tpArraybound.tpTicketInfo4002sub; i++)
	{
		if(chTickettype == tpTicketInfo4002sub[i].bytTicketCode)
		{
			memcpy(td, &tpTicketInfo4002sub[i], sizeof(struct PARA_4002_TICKET));
			return 0;
		}
	}
	return CE_FORBID_TICKET;
}

/*
function:get used parameter vertion 
parameter:
	
return:
*/
void get_cur_para_ver(short eod_type, unsigned char *eod_num, long eod_ver, unsigned char *out_buf)
{
unsigned long	i;

	ShortToByte(eod_type, &out_buf[(*eod_num) * 6 + 1]);
	LongToByte(eod_ver, &out_buf[(*eod_num) * 6 + 3]);
	*eod_num += 1;
}

/*
function:get temporary parameter vertion
parameter:
	eod_ctrl:non zero. delete the temporary parameter;0. retain the parameter
return:
*/
void get_temp_para_ver(unsigned char *eod_file, short eod_type, unsigned char *eod_num, char eod_ctrl, unsigned char *out_buf)
{
unsigned long	i;
char filename[100], filepath[200];
unsigned long lngbyte4;
FILE	*fl;
struct PublicTitle 	pub_title;

	if(0 == FileisExist("./paranew/", eod_file, filename))
	{
		ShortToByte(eod_type, &out_buf[(*eod_num) * 6 + 1]);
		memset(filepath, 0x00, 200);
		sprintf(filepath, "./paranew/%s", filename);
		fl = fopen(filepath, "rb");
			fread(&lngbyte4, 1, sizeof(lngbyte4), fl);
		fclose(fl);
		memcpy(&out_buf[(*eod_num) * 6 + 3], &lngbyte4, 4);
		*eod_num += 1;
		if(eod_ctrl)
		{
			remove(filepath);
		}
	}

}
/*
function:get the station and system degrade mode
parameter:
	eod_ctrl:
return:
*/
void get_degrade_mode(unsigned char *src_station)
{
unsigned long	i;
char filename[100], filepath[200];
unsigned long lngbyte4;
unsigned short	curstation;
/*
	memset(&tpwaivermode.cur_sta_failure, 0x00, 10);
	for(i = 0; i < tpStationWaiverMode.waivermode_len; i++)
	{
		switch(tpStationWaiverMode.waivermode_val[i * 3 + 2])
		{
		case SZ_WAIVER_FAILURE:
			if(memcmp(src_station, &tpStationWaiverMode.waivermode_val[i * 3], 2) == 0)
				if(!tpwaivermode.cur_sta_failure) tpwaivermode.cur_sta_failure = 0xff;
			else
				if(!tpwaivermode.oth_sta_failure) tpwaivermode.oth_sta_failure = 0xff;
			break;
		case SZ_WAIVER_ENTRY:
			if(memcmp(src_station, &tpStationWaiverMode.waivermode_val[i * 3], 2) == 0)
				if(!tpwaivermode.cur_sta_entry) tpwaivermode.cur_sta_entry = 0xff;
			else
			{
				if(!tpwaivermode.oth_sta_entry) tpwaivermode.oth_sta_entry = 0xff;
				tpwaivermode.oth_entry_num += 1;
			}
			break;
		case SZ_WAIVER_DATE:
			if(memcmp(src_station, &tpStationWaiverMode.waivermode_val[i * 3], 2) == 0)
				if(!tpwaivermode.cur_sta_date) tpwaivermode.cur_sta_date = 0xff;
			else
				if(!tpwaivermode.oth_sta_date) tpwaivermode.oth_sta_date = 0xff;
			break;
		case SZ_WAIVER_FARE:
			if(memcmp(src_station, &tpStationWaiverMode.waivermode_val[i * 3], 2) == 0)
				if(!tpwaivermode.cur_sta_fare) tpwaivermode.cur_sta_fare = 0xff;
			else
				if(!tpwaivermode.oth_sta_fare) tpwaivermode.oth_sta_fare = 0xff;
			break;
		case SZ_WAIVER_EMERGENCY:
			if(memcmp(src_station, &tpStationWaiverMode.waivermode_val[i * 3], 2) == 0)
				if(!tpwaivermode.cur_sta_emergency) tpwaivermode.cur_sta_emergency = 0xff;
			else
				if(!tpwaivermode.oth_sta_emergency) tpwaivermode.oth_sta_emergency = 0xff;
			break;
		default:
			break;
		}
	}
	//mode list
	ByteToShort(&curstation, src_station);
	for(i = 0; i < EodWaiverDateMasterConfig.StationModeInfo.StationModeInfo_len; i++)
	{
		switch(EodWaiverDateInfo[i].ModeCode)
		{
		case SZ_WAIVER_FAILURE:
			if(EodWaiverDateInfo[i].StationID == curstation)
				tpwaivermode.cur_sta_failure = 0xff;
			break;
		case SZ_WAIVER_FARE:
			if(EodWaiverDateInfo[i].StationID == curstation)
				tpwaivermode.cur_sta_fare = 0xff;
			break;
		}
	}
*/	return ;
}

/*
*/
unsigned short km_get_record(unsigned char *cmd_buf, unsigned char *out_buf, unsigned char *out_len)
{
unsigned short record_type;
unsigned char upload_flag;

//#ifdef DEBUG_PRINT
	printf("\nget record command is %02x and length is %02x:\n", cmd_buf[6], cmd_buf[5]);
	printf("phycial ID %02x%02x%02x%02x%02x%02x%02x%02x ", cmd_buf[7], cmd_buf[8], cmd_buf[9], cmd_buf[10], cmd_buf[11], cmd_buf[12], cmd_buf[13], cmd_buf[14]);
	printf("SN %02x%02x%02x%02x history type %02x\n", cmd_buf[15], cmd_buf[16], cmd_buf[17], cmd_buf[18], cmd_buf[19]);
	printf("RFU:%02x%02x%02x%02x\n", cmd_buf[20], cmd_buf[21], cmd_buf[22], cmd_buf[23]);
//#endif	
	
	//current record
	if(cmd_buf[19] == 0)
	{
		printf("record type %04x\n", para_type);
		if((record_type = ee_read_last_record(para_type, &out_buf[2], out_len, 1)) != 0)
		{
			ShortToByte(para_type, out_buf);
			*out_len = *out_len + 2;
			return CE_OK;
		}
	}else
	{
		return CE_NORECORD;
	}
	return CE_NORECORD;
	
}
/*
*/
unsigned short ee_write_last_record(unsigned short message_type)
{
unsigned short addr, i;
unsigned char upload_flag, *ul6002, rollback;

	
	switch(message_type)
	{
	case 0x6002:
		rollback = 0;
		ee_write(EE_UL_BACKUP, 1, &rollback);
		addr = EE_UL_TRANSACTION;
		upload_flag = 1;
		ee_write(addr, 1, &upload_flag);
		addr += 1;
		ee_write(addr, 4, (UBYTE *)&position_6002);
		addr += 4;
		ee_write(addr, sizeof(tpUL6002), (UBYTE *)&tpUL6002);
#ifdef	DEBUG_PRINT
		ul6002 = (unsigned char *)&tpUL6002;
		for(i = 0; i < sizeof(tpUL6002); i++)
		{
			printf("%02x", *(ul6002 + i));
		}
		printf("\n");
#endif
		reader_status = KM_RW_RECORD;
		break;
	default:
		break;
	}
	return 0;
	
}

/*
message_type:transaction record
upload_control:0,read the saved record and upload then change the upload flag; or just read and check the flag
*/
unsigned short ee_read_last_record(unsigned short message_type, unsigned char *out_buf, unsigned char *out_len, char upload_control)
{
unsigned short addr;
unsigned char upload_flag;
int i;
	
	switch(message_type)
	{
	case 0x6002:
		addr = EE_UL_TRANSACTION;
		ee_read(addr, 1, &upload_flag);
		addr += 1;
		ee_read(addr, 4, (UBYTE *)&position_6002);
		addr += 4;
		*out_len = sizeof(tpUL6002);
		ee_read(addr, *out_len, out_buf);
				
		if(upload_flag == 1)
		{
			if(upload_control)
			{
				upload_flag = 0;
				ee_write(EE_UL_TRANSACTION, 1, &upload_flag);
				reader_status = KM_RW_IDLE;
				return 0x6002;
			}
			else
				return 0x6002;
		}
		break;
	default:
		break;
	}
	return 0;
	
}