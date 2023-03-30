//shmetro24G.c
#ifndef _SHMETRO24G_C_
#define _SHMETRO24G_C_

#define MAX_PROVE_INDEX	1

UBYTE g24GFlag;
UBYTE EntryPosID[4],ExitPosID[4];
UDWORD TotalPurse,LastTransValue;
UWORD TotalCnt;
UBYTE LocationID;
UBYTE CurrentLocation;
UBYTE Record24G[65];
UBYTE OtherTransFlag;
UBYTE gTransTypeFlag;
UBYTE gPosCityID[2];
UBYTE gTransValueMax[4];
UBYTE gLastEntryTime[7];
UBYTE ProveDataIndex=0;
TRANSPROVE_inram ProveData[MAX_PROVE_INDEX];

/*====================================================================
函数:add_provedata
功能:
====================================================================*/
void add_provedata(void)
{
	memcpy(ProveData[ProveDataIndex].lastSimSn,gAppSn,10);
	uword_to_buf(gCardCnt,ProveData[ProveDataIndex].lastCardCnt);
	udword_to_buf(gPosTransCnt,ProveData[ProveDataIndex].lastPosTransCnt);
	ProveData[ProveDataIndex].keybacon=gKeyBacon;
//	ProveDataIndex = (ProveDataIndex+1)%MAX_PROVE_INDEX;
	ProveDataIndex = 0;
	return;
}

/*==========================================================
函数：trans_entry_24G
功能：手机支付进站交易
==========================================================*/
//#define _DEBUG_TRANS_ENTRY_24G_
int trans_entry_24G(void)
{
	UBYTE inbuf[257];
	UBYTE outbuf[257];
	int ret;
	UBYTE inbytes,outbytes;
	UBYTE mac1[4],mac2[4];
	UBYTE buf[2];
	
	mobilecurrent = get_sys_time_cnt();
	mobilestarttime = mobilecurrent;
	
	gCardThisTransValue = 0;
	gDebugStep = 0;
	memcpy(inbuf,"\x80\x50\x03\x02\x0b",5);
	inbuf[5] = DEFAULT_KEY_INDEX;
	udword_to_buf(gCardThisTransValue,&inbuf[6]);
	memcpy(&inbuf[10],bpgSMobilepsamNo,6);
	inbytes = 16;
	//ee_write(200,16,inbuf);//
	ret = ZTE_ICMD(inbytes,inbuf,&outbytes,outbuf,50);
	
	mobilecurrent = get_sys_time_cnt();
	mobilestarttime = mobilecurrent-mobilestarttime;
	udword_to_buf(mobilestarttime,&TimeBuf[28]);
	mobilestarttime = mobilecurrent;
	
	if(ret)
	{
		goto label_readerr;
	}
	if(outbytes!=17)
	{
		goto label_invalid;
	}
	//ee_write(216,17,outbuf);//
	gDebugStep = 1;
	buf_to_uword(&outbuf[4],gCardCnt);
	gKeyBacon = outbuf[9];
	gAluBacon = outbuf[10];
	memcpy(ICC,&outbuf[11],4);
	gCardThisRemain = gCardLastRemain - gCardThisTransValue;
	//计算MAC1
	memcpy(inbuf,ICC,4);
	uword_to_buf(gCardCnt,&inbuf[4]);
	udword_to_buf(gCardThisTransValue,&inbuf[6]);
	inbuf[10] = 0x09;					//09:复合消费，06：电子钱包消费
	memcpy(&inbuf[11],gTransTime,7);
	inbuf[18] = gKeyBacon;
//	inbuf[18] = 0x01;
	inbuf[19] = gAluBacon;
	inbuf[20] = gKeyBacon;
//	inbuf[20] = 0x01;
	memcpy(&inbuf[21],&gAppSn[3],7);
	inbuf[28] = LocationID;
	memcpy(&inbuf[29],"\x80\x00\x00\x00\x00\x00\x00",7);
	memcpy(bgTaskKeyInbuf,inbuf,36);
	//bgTaskKeyStep = TASK_KEY_INIT_FOR_SAM_2;
	set_task_key(TASK_KEY_INIT_FOR_SAM_2);
	//OSTimeDly(1);
	gDebugStep = 2;
	
	//写公共记录
	
	if(gStationTimeOverRideFlag == 0x00) 
	{
		buf[0] = 0x41;		
	}
	else 
	{
		if(gStationTimeOverRideFlag==0x18)
		{
			if(gMetroIoFlag&0x08)
				buf[0]=	0x78;
			else
				buf[0]= 0x71;
		}
		else
		{
			buf[0]=gStationTimeOverRideFlag<<1;
			buf[0]+=0x40;
			if(buf[0]==0x50)
				if(gMetroIoFlag&0x08)
					buf[0]= 0x59;
		}
	}
	if(CurrentLocation!=LocationID)
	{
		if((gTransTime[1]!=Record24G[16])||(gTransTime[2]!=Record24G[17]))
		{
			TotalPurse = 0;
			TotalCnt = 0;
		}
		memcpy(inbuf,"\x80\xdc\x01\xcc\x40",5);
#ifndef _MOBILEv1_1_
		inbuf[2] = Record24G[0];
		inbuf[3] = 0xc8;
#endif
		memcpy(Record24G,"\x09\x3e\x00\x00",4);
		Record24G[3] = 0x10;
		Record24G[4] = 0x01;
		memcpy(&Record24G[5],bpgSMobilepsamNo,6);
//		memset(&Record24G[11],0,4);
		memcpy(&Record24G[11],gTransValueMax,4);
//		Record24G[11] = 0x00;
//		Record24G[12] = 0x00;
//		Record24G[13] = 0x03;						//预计最大交易金额10.00元
//		Record24G[14] = 0xe8;
		
		Record24G[22] = 0x02;
		Record24G[23] = 0x10;
		Record24G[24] = 0x00;
	//20090924 add
		Record24G[25] = 0x00;						//运营企业代码3字节
		Record24G[26] = 0x00;
		Record24G[27] = 0x01;
	//
		Record24G[28] = buf[0];//0x55;
		Record24G[37] = *((char *)&TotalPurse+LONG_HIGH2);
		Record24G[38] = *((char *)&TotalPurse+LONG_HIGH1);
		Record24G[39] = *((char *)&TotalPurse+LONG_HIGH0);		
		Record24G[40] = *((char *)&TotalCnt+INT_HIGH);
		Record24G[41] = *((char *)&TotalCnt+INT_LOW);
		
		if(!(gStationTimeOverRideFlag&0x08))			//出站换乘进站
		{
			memcpy(&Record24G[15],gTransTime,7);
			memcpy(&Record24G[29],gPosNo,4);
			memcpy(&Record24G[44],gTransTime,7);
			Record24G[42] = 0x00;
			Record24G[43] = 0x00;		
		}
		
	}
	else
	{
		if((gTransTime[1]!=Record24G[5])||(gTransTime[2]!=Record24G[6]))
		{
			TotalPurse = 0;
			TotalCnt = 0;
		}
		memcpy(inbuf,"\x80\xdc\x02\xcc\x40",5);
#ifndef _MOBILEv1_1_
		inbuf[2] = Record24G[0];
		inbuf[3] = 0xc8;
#endif
		memcpy(Record24G,"\x14\x3e\x00",3);
#ifdef _MOBILEv1_1_
		Record24G[0] = 0x09;
#endif
		Record24G[3] = buf[0];//0x55;		
		Record24G[19] = *((char *)&TotalPurse+LONG_HIGH2);
		Record24G[20] = *((char *)&TotalPurse+LONG_HIGH1);
		Record24G[21] = *((char *)&TotalPurse+LONG_HIGH0);		
		Record24G[22] = *((char *)&TotalCnt+INT_HIGH);
		Record24G[23] = *((char *)&TotalCnt+INT_LOW);
		
		if(!(gStationTimeOverRideFlag&0x08))		//出站换乘进站
		{
			memcpy(&Record24G[4],gTransTime,7);
			memcpy(&Record24G[11],gPosNo,4);
			memcpy(&Record24G[26],gTransTime,7);
			Record24G[24] = 0x00;
			Record24G[25] = 0x00;
		}

	}
	
	memcpy(&inbuf[5],Record24G,64);
	inbytes = 69;
	
	mobilecurrent = get_sys_time_cnt();
	mobilestarttime = mobilecurrent;
	
  //ee_write(500,69,inbuf);//
  
  //20130902
	//delay_ms(20);
	ret = ZTE_ICMD(inbytes,inbuf,&outbytes,outbuf,100);
	
	mobilecurrent = get_sys_time_cnt();
	mobilestarttime = mobilecurrent-mobilestarttime;
	udword_to_buf(mobilestarttime,&TimeBuf[32]);
	mobilestarttime = mobilecurrent;
	
	if(ret)
	{
		goto label_writeerr;
	}
	//ee_write(569,2,outbuf);//
	gDebugStep = 3;
	if((outbuf[0]!=0x90)||(outbuf[1]!=0x00))
	{
		goto label_invalid;
	}
	gDebugStep = 4;
	
	//取MAC1
	if(SHSendAuthResp())
	{
		goto label_samerr;
	}

	gDebugStep = 8;
	buf_to_udword(bgTaskKeyOutbuf,gPosTransCnt);
	memcpy(mac1,&bgTaskKeyOutbuf[4],4);
	//debit for capp purchase
	gDebugStep = 9;
	memcpy(inbuf,"\x80\x54\x01\x00\x0F",5);
	udword_to_buf(gPosTransCnt,&inbuf[5]);
	memcpy(&inbuf[9],gTransTime,7);
	memcpy(&inbuf[16],mac1,4);
	inbuf[20] = 0x08;
	inbytes = 21;
	//ee_write(233,21,inbuf);//

	mobilecurrent = get_sys_time_cnt();
	mobilestarttime = mobilecurrent;
	
	ret = ZTE_ICMD(inbytes,inbuf,&outbytes,outbuf,150);
	
	mobilecurrent = get_sys_time_cnt();
	mobilestarttime = mobilecurrent-mobilestarttime;
	udword_to_buf(mobilestarttime,&TimeBuf[36]);
	mobilestarttime = mobilecurrent;

	if(ret)
	{
		add_provedata();
		gResponseStatus=CE_CARDREMOVED;
		gGetCardInfoStep=0xff;
		return OP_ERR;
//		goto label_readerr;
	}
	gDebugStep = 10;
	if((outbuf[outbytes-2]!=0x90)||(outbuf[outbytes-1]!=0x00))
	{
		if((outbuf[outbytes-2]==0x93)&&(outbuf[outbytes-1]==0x02))
		{
			gResponseStatus=CE_MAC1ERR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		add_provedata();
		gDebugStep = outbuf[outbytes-2];
		goto label_invalid;
	}
	gDebugStep = 11;
	if(outbytes!=10)
	{
		add_provedata();
		goto label_invalid;
	}
	memcpy(gTAC,outbuf,4);
	memcpy(mac2,&outbuf[4],4);
	//credit sam for purchase
	memcpy(inbuf,"\x80\x72\x00\x00\x04",5);
	memcpy(&inbuf[5],mac2,4);
	inbytes = 9;
	ret=sam_apdu(bgSMobilepsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
	if(ret!=0)
	{
		goto label_samerr;
	}
	gDebugStep = 12;
	if((outbuf[outbytes-2]!=0x90)||(outbuf[outbytes-1]!=0x00))
	{
		gResponseStatus=CE_MAC2ERR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
	gDebugStep = TotalCnt;
	return 0;
label_writeerr:
     #ifdef _DEBUG_TRANS_ENTRY_24G_
     debug_printf("\x0d\x0a Err wr:24g entry,step=%d",gDebugStep);
     #endif	
		gResponseStatus=CE_WRITEERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_readerr:
		gResponseStatus=CE_READERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_invalid:
		gResponseStatus=CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_samerr:
		gResponseStatus=CE_PSAMERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
}
/*==========================================================
函数：trans_exit_24G
功能：手机支付出站交易
==========================================================*/
int trans_exit_24G(UBYTE transtype)
{
	UBYTE inbuf[257];
	UBYTE outbuf[257];
	int ret;
	UBYTE inbytes,outbytes;
//	UBYTE cnt;
	UBYTE mac1[4],mac2[4];
	UBYTE buf[2];
	
	memcpy(inbuf,"\x80\x50\x03\x02\x0b",5);
	inbuf[5] = DEFAULT_KEY_INDEX;
	udword_to_buf(gCardThisTransValue,&inbuf[6]);
	memcpy(&inbuf[10],bpgSMobilepsamNo,6);
	inbytes = 16;
	//ee_write(200,16,inbuf);//
	ret = ZTE_ICMD(inbytes,inbuf,&outbytes,outbuf,50);
	if(ret)
	{
		goto label_readerr;
	}
	if(outbytes!=17)
	{
		goto label_invalid;
	}
	//ee_write(216,17,outbuf);//
	buf_to_uword(&outbuf[4],gCardCnt);
	gKeyBacon = outbuf[9];
	gAluBacon = outbuf[10];
	memcpy(ICC,&outbuf[11],4);
	gCardThisRemain = gCardLastRemain - gCardThisTransValue;
	LastTransValue = LastTransValue+gCardThisTransValue;
	//计算MAC1
	memcpy(inbuf,ICC,4);
	uword_to_buf(gCardCnt,&inbuf[4]);
	udword_to_buf(gCardThisTransValue,&inbuf[6]);
	inbuf[10] = 0x09;					//09:复合消费，06：电子钱包消费
	memcpy(&inbuf[11],gTransTime,7);
	inbuf[18] = gKeyBacon;
	inbuf[19] = gAluBacon;
	inbuf[20] = gKeyBacon;
	memcpy(&inbuf[21],&gAppSn[3],7);
	inbuf[28] = LocationID;
	memcpy(&inbuf[29],"\x80\x00\x00\x00\x00\x00\x00",7);
	memcpy(bgTaskKeyInbuf,inbuf,36);
	//bgTaskKeyStep = TASK_KEY_INIT_FOR_SAM_2;
	set_task_key(TASK_KEY_INIT_FOR_SAM_2);
	//OSTimeDly(1);
	
	//写公共记录
//	if(gMetroIoFlag&0x20)
//		buf[0]=0xa9;
//	else
//		buf[0]=0x88;
	if((transtype>0x05)&&(transtype<10))
	{
		//if(gStationTimeOverRideFlag&0x20)
		//	buf[0]=0xa9;
		//else
		//	buf[0]=0x88;
		if(gStationTimeOverRideFlag&0x20)
			buf[0]=0x88;
		else
			buf[0]=0xa9;
	}
	else
	{	
		if(gStationTimeOverRideFlag&0x20)
			buf[0]=0xa0;
		else
			buf[0]=0x81;
	}
	if(CurrentLocation!=LocationID)
	{
		if((gTransTime[1]!=Record24G[16])||(gTransTime[2]!=Record24G[17]))
		{
			TotalPurse = LastTransValue;
			TotalCnt = 1;
		}
		else
		{
			TotalPurse = TotalPurse+gCardThisTransValue;
			TotalCnt++;
		}
		memcpy(inbuf,"\x80\xdc\x01\xcc\x40",5);
#ifndef _MOBILEv1_1_
		inbuf[2] = Record24G[0];
		inbuf[3] = 0xc8;
#endif
		memcpy(Record24G,"\x09\x3e\x00\x00",4);
		Record24G[3] = 0x10;
		Record24G[4] = 0x00;
		memcpy(&Record24G[5],bpgSMobilepsamNo,6);
		udword_to_buf(gCardThisTransValue,&Record24G[11]);
		memcpy(&Record24G[15],gTransTime,7);
		Record24G[22] = 0x02;
		Record24G[23] = 0x10;
		Record24G[24] = 0x00;
	//20090924 add
		Record24G[25] = 0x00;						//运营企业代码3字节
		Record24G[26] = 0x00;
		Record24G[27] = 0x01;
	//
		Record24G[28] = buf[0];//0x66;
		memcpy(&Record24G[33],gPosNo,4);		
		Record24G[37] = *((char *)&TotalPurse+LONG_HIGH2);
		Record24G[38] = *((char *)&TotalPurse+LONG_HIGH1);
		Record24G[39] = *((char *)&TotalPurse+LONG_HIGH0);		
		Record24G[40] = *((char *)&TotalCnt+INT_HIGH);
		Record24G[41] = *((char *)&TotalCnt+INT_LOW);
//		Record24G[42] = *((char *)&gCardThisTransValue+LONG_HIGH1);
//		Record24G[43] = *((char *)&gCardThisTransValue+LONG_HIGH0);		
		Record24G[42] = *((char *)&LastTransValue+LONG_HIGH1);
		Record24G[43] = *((char *)&LastTransValue+LONG_HIGH0);		
		Record24G[51] = gPosNo[1];
		Record24G[52] = gPosNo[3];
		Record24G[53] = gPosNo[0];
		Record24G[53] |= 0x80;		
		memcpy(&Record24G[54],&gTransTime[2],4);
	}
	else
	{
		if((gTransTime[1]!=Record24G[5])||(gTransTime[2]!=Record24G[6]))
		{
			TotalPurse = gCardThisTransValue;
			//TotalPurse = LastTransValue;
			TotalCnt = 1;
		}
		else
		{
			//2014/4/1 13:37:13
//			if((gTransTime[1]!=gLastEntryTime[1])||(gTransTime[2]!=gLastEntryTime[2]))
//			{
//				TotalPurse = gCardThisTransValue;
//				TotalCnt++;	
//			}
//			else
//			{
//				TotalPurse = TotalPurse+gCardThisTransValue;
//				TotalCnt++;	
//			}
			//
			TotalPurse = TotalPurse+gCardThisTransValue;
			TotalCnt++;		
		}
		memcpy(inbuf,"\x80\xdc\x02\xcc\x40",5);
#ifndef _MOBILEv1_1_
		inbuf[2] = Record24G[0];
		inbuf[3] = 0xc8;
#endif
		memcpy(Record24G,"\x14\x3e\x00",3);
#ifdef _MOBILEv1_1_
		Record24G[0] = 0x09;
#endif
		Record24G[3] = buf[0];//0x66;
		memcpy(&Record24G[4],gTransTime,7);
		memcpy(&Record24G[15],gPosNo,4);
		Record24G[19] = *((char *)&TotalPurse+LONG_HIGH2);
		Record24G[20] = *((char *)&TotalPurse+LONG_HIGH1);
		Record24G[21] = *((char *)&TotalPurse+LONG_HIGH0);		
		Record24G[22] = *((char *)&TotalCnt+INT_HIGH);
		Record24G[23] = *((char *)&TotalCnt+INT_LOW);
//		Record24G[24] = *((char *)&gCardThisTransValue+LONG_HIGH1);
//		Record24G[25] = *((char *)&gCardThisTransValue+LONG_HIGH0);	
		Record24G[24] = *((char *)&LastTransValue+LONG_HIGH1);
		Record24G[25] = *((char *)&LastTransValue+LONG_HIGH0);	
		Record24G[33] = gPosNo[1];
		Record24G[34] = gPosNo[3];
		Record24G[35] = gPosNo[0];
		Record24G[35]|=0x80;		
		memcpy(&Record24G[36],&gTransTime[2],4);
	}
	
	memcpy(&inbuf[5],Record24G,64);
	inbytes = 69;
	//ee_write(400,69,inbuf);//
	
	//20130902
	//delay_ms(20);
	ret = ZTE_ICMD(inbytes,inbuf,&outbytes,outbuf,200);
	if(ret)
	{
		goto label_writeerr;
	}
	//ee_write(469,2,outbuf);//
	if((outbuf[0]!=0x90)||(outbuf[1]!=0x00))
	{
		goto label_invalid;
	}
	
	//取MAC1
	if(SHSendAuthResp())
	{
		goto label_samerr;
	}
	buf_to_udword(bgTaskKeyOutbuf,gPosTransCnt);
	memcpy(mac1,&bgTaskKeyOutbuf[4],4);
	//debit for capp purchase
	memcpy(inbuf,"\x80\x54\x01\x00\x0F",5);
	udword_to_buf(gPosTransCnt,&inbuf[5]);
	memcpy(&inbuf[9],gTransTime,7);
	memcpy(&inbuf[16],mac1,4);
	inbuf[20] = 0x08;
	inbytes = 21;
	ret = ZTE_ICMD(inbytes,inbuf,&outbytes,outbuf,150);
	if(ret)
	{
		add_provedata();
		gResponseStatus=CE_CARDREMOVED;
		gGetCardInfoStep=0xff;
		return OP_ERR;
//		goto label_readerr;
	}
	if((outbuf[outbytes-2]!=0x90)||(outbuf[outbytes-1]!=0x00))
	{
		if((outbuf[outbytes-2]==0x93)&&(outbuf[outbytes-1]==0x02))
		{
			gResponseStatus=CE_MAC1ERR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		add_provedata();
		gDebugStep = outbuf[outbytes-2];
		goto label_invalid;
	}
	if(outbytes!=10)
	{
		add_provedata();
		goto label_invalid;
	}
	memcpy(gTAC,outbuf,4);
	memcpy(mac2,&outbuf[4],4);
	//credit sam for purchase
	memcpy(inbuf,"\x80\x72\x00\x00\x04",5);
	memcpy(&inbuf[5],mac2,4);
	inbytes = 9;
	ret=sam_apdu(bgSMobilepsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
	if(ret!=0)
	{
		goto label_samerr;
	}
	if((outbuf[outbytes-2]!=0x90)||(outbuf[outbytes-1]!=0x00))
	{
		gResponseStatus=CE_MAC2ERR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
	
	return 0;
label_writeerr:
		gResponseStatus=CE_WRITEERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_readerr:
		gResponseStatus=CE_READERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_invalid:
		gResponseStatus=CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_samerr:
		gResponseStatus=CE_PSAMERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
}
/*==========================================================
函数：trans_lock_24G
功能：手机支付改写异地异常交易标志
==========================================================*/
int trans_lock_24G(void)
{
	UBYTE inbuf[257];
	UBYTE outbuf[257];
	int ret;
	UBYTE inbytes,outbytes;
	UBYTE mac1[4],mac2[4];
	
	
	gCardThisTransValue = 0;
	gDebugStep = 0;
	memcpy(inbuf,"\x80\x50\x03\x02\x0b",5);
	inbuf[5] = DEFAULT_KEY_INDEX;
	udword_to_buf(gCardThisTransValue,&inbuf[6]);
	memcpy(&inbuf[10],bpgSMobilepsamNo,6);
	inbytes = 16;
	//ee_write(200,16,inbuf);//
	ret = ZTE_ICMD(inbytes,inbuf,&outbytes,outbuf,50);
	if(ret)
	{
		goto label_readerr;
	}
	if(outbytes!=17)
	{
		goto label_invalid;
	}
	//ee_write(216,17,outbuf);//
	gDebugStep = 1;
	buf_to_uword(&outbuf[4],gCardCnt);
	gKeyBacon = outbuf[9];
	gAluBacon = outbuf[10];
	memcpy(ICC,&outbuf[11],4);
	gCardThisRemain = gCardLastRemain - gCardThisTransValue;
	//计算MAC1
	memcpy(inbuf,ICC,4);
	uword_to_buf(gCardCnt,&inbuf[4]);
	udword_to_buf(gCardThisTransValue,&inbuf[6]);
	inbuf[10] = 0x09;					//09:复合消费，06：电子钱包消费
	memcpy(&inbuf[11],gTransTime,7);
	inbuf[18] = gKeyBacon;
	inbuf[19] = gAluBacon;
	inbuf[20] = gKeyBacon;
	memcpy(&inbuf[21],&gAppSn[3],7);
	inbuf[28] = LocationID;
	memcpy(&inbuf[29],"\x80\x00\x00\x00\x00\x00\x00",7);
	memcpy(bgTaskKeyInbuf,inbuf,36);
	//bgTaskKeyStep = TASK_KEY_INIT_FOR_SAM_2;
	set_task_key(TASK_KEY_INIT_FOR_SAM_2);
	//OSTimeDly(1);
	gDebugStep = 2;
	//写公共记录
	memcpy(inbuf,"\x80\xdc\x01\xcc\x40",5);
#ifndef _MOBILEv1_1_
	inbuf[2] = Record24G[0];
	inbuf[3] = 0xc8;
#endif
	Record24G[24] = 0x01;
	
	memcpy(&inbuf[5],Record24G,64);
	inbytes = 69;
	//ee_write(500,69,inbuf);//
  //delay_ms(20);
	ret = ZTE_ICMD(inbytes,inbuf,&outbytes,outbuf,100);
	if(ret)
	{
		goto label_writeerr;
	}
	//ee_write(569,2,outbuf);//
	gDebugStep = 3;
	if((outbuf[0]!=0x90)||(outbuf[1]!=0x00))
	{
		goto label_invalid;
	}
	gDebugStep = 4;
	
	//取MAC1
	if(SHSendAuthResp())
	{
		goto label_samerr;
	}

	gDebugStep = 8;
	buf_to_udword(bgTaskKeyOutbuf,gPosTransCnt);
	memcpy(mac1,&bgTaskKeyOutbuf[4],4);
	//debit for capp purchase
	gDebugStep = 9;
	memcpy(inbuf,"\x80\x54\x01\x00\x0F",5);
	udword_to_buf(gPosTransCnt,&inbuf[5]);
	memcpy(&inbuf[9],gTransTime,7);
	memcpy(&inbuf[16],mac1,4);
	inbuf[20] = 0x08;
	inbytes = 21;
	//ee_write(233,21,inbuf);//
	ret = ZTE_ICMD(inbytes,inbuf,&outbytes,outbuf,50);
	if(ret)
	{
		add_provedata();
		gResponseStatus=CE_CARDREMOVED;
		gGetCardInfoStep=0xff;
		return OP_ERR;
//		goto label_readerr;
	}
	gDebugStep = 10;
	if((outbuf[outbytes-2]!=0x90)||(outbuf[outbytes-1]!=0x00))
	{
		if((outbuf[outbytes-2]==0x93)&&(outbuf[outbytes-1]==0x02))
		{
			gResponseStatus=CE_MAC1ERR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		add_provedata();
		gDebugStep = outbuf[outbytes-2];
		goto label_invalid;
	}
	gDebugStep = 11;
	if(outbytes!=10)
	{
		add_provedata();
		goto label_invalid;
	}
	memcpy(gTAC,outbuf,4);
	memcpy(mac2,&outbuf[4],4);
	//credit sam for purchase
	memcpy(inbuf,"\x80\x72\x00\x00\x04",5);
	memcpy(&inbuf[5],mac2,4);
	inbytes = 9;
	ret=sam_apdu(bgSMobilepsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
	if(ret!=0)
	{
		goto label_samerr;
	}
	gDebugStep = 12;
	if((outbuf[outbytes-2]!=0x90)||(outbuf[outbytes-1]!=0x00))
	{
		gResponseStatus=CE_MAC2ERR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
	
	return 0;
label_writeerr:
		gResponseStatus=CE_WRITEERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_readerr:
		gResponseStatus=CE_READERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_invalid:
		gResponseStatus=CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_samerr:
		gResponseStatus=CE_PSAMERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
}
/*==========================================================
函数：trans_Update_24G
功能：手机支付更新交易
==========================================================*/
int trans_update_24G(UBYTE *updatecache)
{
	UBYTE inbuf[257];
	UBYTE outbuf[257];
	int ret;
	UBYTE inbytes,outbytes;
//	UBYTE cnt;
	UBYTE mac1[4],mac2[4];
	UBYTE buf[2];
	UBYTE timeflag;
	
	
	timeflag = updatecache[12];
	mobilecurrent = get_sys_time_cnt();
	mobilestarttime = mobilecurrent;
	
	gDebugStep = 0;
	memcpy(inbuf,"\x80\x50\x03\x02\x0b",5);
	inbuf[5] = DEFAULT_KEY_INDEX;
	udword_to_buf(gCardThisTransValue,&inbuf[6]);
	memcpy(&inbuf[10],bpgSMobilepsamNo,6);
	inbytes = 16;
	ret = ZTE_ICMD(inbytes,inbuf,&outbytes,outbuf,30);
	
	mobilecurrent = get_sys_time_cnt();
	mobilestarttime = mobilecurrent-mobilestarttime;
	udword_to_buf(mobilestarttime,&TimeBuf[28]);
	mobilestarttime = mobilecurrent;
	
	if(ret)
	{
		goto label_readerr;
	}
	if(outbytes!=17)
	{
		goto label_invalid;
	}
	gDebugStep = 1;
	buf_to_uword(&outbuf[4],gCardCnt);
	gKeyBacon = outbuf[9];
	gAluBacon = outbuf[10];
	memcpy(ICC,&outbuf[11],4);
	gCardThisRemain = gCardLastRemain - gCardThisTransValue;
	//计算MAC1
	memcpy(inbuf,ICC,4);
	uword_to_buf(gCardCnt,&inbuf[4]);
	udword_to_buf(gCardThisTransValue,&inbuf[6]);
	inbuf[10] = 0x09;					//09:复合消费，06：电子钱包消费
	memcpy(&inbuf[11],gTransTime,7);
	inbuf[18] = gKeyBacon;
	inbuf[19] = gAluBacon;
	inbuf[20] = gKeyBacon;
	memcpy(&inbuf[21],&gAppSn[3],7);
	inbuf[28] = LocationID;
	memcpy(&inbuf[29],"\x80\x00\x00\x00\x00\x00\x00",7);
	memcpy(bgTaskKeyInbuf,inbuf,36);
	//bgTaskKeyStep = TASK_KEY_INIT_FOR_SAM_2;
	set_task_key(TASK_KEY_INIT_FOR_SAM_2);
	//OSTimeDly(1);
	gDebugStep = 2;
	//写公共记录
	
//	if(gStationTimeOverRideFlag == 0x00) 
//	{
//		buf[0] = 0x41;		
//	}
//	else 
//	{
//		if(gStationTimeOverRideFlag==0x18)
//		{
//			if(gMetroIoFlag&0x08)
//				buf[0]=	0x78;
//			else
//				buf[0]= 0x71;
//		}
//		else
//		{
//			buf[0]=gStationTimeOverRideFlag<<1;
//			buf[0]+=0x40;
//			if(buf[0]==0x50)
//				if(gMetroIoFlag&0x08)
//					buf[0]= 0x59;
//		}
//	}
	
	buf[0] = updatecache[0];
	if(CurrentLocation!=LocationID)
	{
		if((gTransTime[1]!=Record24G[5])||(gTransTime[2]!=Record24G[6]))
		{
			TotalPurse = 0;
			TotalCnt = 0;
		}
		memcpy(inbuf,"\x80\xdc\x01\xcc\x40",5);
#ifndef _MOBILEv1_1_
		inbuf[2] = Record24G[0];
		inbuf[3] = 0xc8;
#endif
		memcpy(Record24G,"\x09\x3e\x00\x00",4);
		Record24G[3] = 0x10;
		Record24G[4] = 0x01;
		memcpy(&Record24G[5],bpgSMobilepsamNo,6);
		memset(&Record24G[11],0,4);
//		memcpy(&Record24G[11],gTransValueMax,4);
		Record24G[11] = 0x00;
		Record24G[12] = updatecache[11];
		Record24G[13] = updatecache[10];						//预计最大交易金额
		Record24G[14] = updatecache[9];
		if(timeflag&0x02)
		{
			memcpy(&Record24G[15],gTransTime,7);
		}
		Record24G[22] = 0x02;
		Record24G[23] = 0x10;
		Record24G[24] = 0x00;
	//20090924 add
		Record24G[25] = 0x00;						//运营企业代码3字节
		Record24G[26] = 0x00;
		Record24G[27] = 0x01;
	//
		Record24G[28] = buf[0];//0x55;
		
		memcpy(&Record24G[29],&updatecache[1],8);
//		Record24G[42] = *((char *)&gCardThisTransValue+LONG_HIGH1);
//		Record24G[43] = *((char *)&gCardThisTransValue+LONG_HIGH0);		
		if(timeflag&0x01)
		{
			memcpy(&Record24G[44],gTransTime,7);
		}
		//对0019文件实际交易金额清零
		if(timeflag&0x04)
		{
			memcpy(&Record24G[42],"\x00\x00",2);
		}
	}
	else
	{
		if((gTransTime[1]!=Record24G[5])||(gTransTime[2]!=Record24G[6]))
		{
			TotalPurse = 0;
			TotalCnt = 0;
		}//
		memcpy(inbuf,"\x80\xdc\x02\xcc\x40",5);
#ifndef _MOBILEv1_1_
		inbuf[2] = Record24G[0];
		inbuf[3] = 0xc8;
#endif
		memcpy(Record24G,"\x14\x3e\x00",3);
#ifdef _MOBILEv1_1_
		Record24G[0] = 0x09;
#endif
		Record24G[3] = buf[0];//0x55;
		Record24G[19] = *((char *)&TotalPurse+LONG_HIGH2);
		Record24G[20] = *((char *)&TotalPurse+LONG_HIGH1);
		Record24G[21] = *((char *)&TotalPurse+LONG_HIGH0);		
		Record24G[22] = *((char *)&TotalCnt+INT_HIGH);
		Record24G[23] = *((char *)&TotalCnt+INT_LOW);//
		if(timeflag&0x02)
		{
			memcpy(&Record24G[4],gTransTime,7);
		}
//		memcpy(&Record24G[4],gTransTime,7);
		memcpy(&Record24G[11],&updatecache[1],8);
//		Record24G[24] = *((char *)&gCardThisTransValue+LONG_HIGH1);
//		Record24G[25] = *((char *)&gCardThisTransValue+LONG_HIGH0);
		if(timeflag&0x01)
		{
			memcpy(&Record24G[26],gTransTime,7);
		}
		//对0019文件实际交易金额清零
		if(timeflag&0x04)
		{
		//	memcpy(&Record24G[26],gTransTime,7);
				memcpy(&Record24G[24],"\x00\x00",2);
		}
	}
	
	memcpy(&inbuf[5],Record24G,64);
	inbytes = 69;
	
	mobilecurrent = get_sys_time_cnt();
	mobilestarttime = mobilecurrent;
  //delay_ms(20);
	ret = ZTE_ICMD(inbytes,inbuf,&outbytes,outbuf,150);
	
	mobilecurrent = get_sys_time_cnt();
	mobilestarttime = mobilecurrent-mobilestarttime;
	udword_to_buf(mobilestarttime,&TimeBuf[32]);
	mobilestarttime = mobilecurrent;
	
	if(ret)
	{
		goto label_writeerr;
	}
	gDebugStep = 3;
	if((outbuf[0]!=0x90)||(outbuf[1]!=0x00))
	{
		goto label_invalid;
	}
	gDebugStep = 4;
	
	//取MAC1
	if(SHSendAuthResp())
	{
		goto label_samerr;
	}

	gDebugStep = 8;
	buf_to_udword(bgTaskKeyOutbuf,gPosTransCnt);
	memcpy(mac1,&bgTaskKeyOutbuf[4],4);
	//debit for capp purchase
	gDebugStep = 9;
	memcpy(inbuf,"\x80\x54\x01\x00\x0F",5);
	udword_to_buf(gPosTransCnt,&inbuf[5]);
	memcpy(&inbuf[9],gTransTime,7);
	memcpy(&inbuf[16],mac1,4);
	inbuf[20] = 0x08;
	inbytes = 21;

	mobilecurrent = get_sys_time_cnt();
	mobilestarttime = mobilecurrent;
	
	ret = ZTE_ICMD(inbytes,inbuf,&outbytes,outbuf,150);
	
	mobilecurrent = get_sys_time_cnt();
	mobilestarttime = mobilecurrent-mobilestarttime;
	udword_to_buf(mobilestarttime,&TimeBuf[36]);
	mobilestarttime = mobilecurrent;

//	ret = ZTE_ICMD(inbytes,inbuf,&outbytes,outbuf);
	if(ret)
	{
		add_provedata();
		
		gResponseStatus=CE_CARDREMOVED;
		gGetCardInfoStep=0xff;
		return OP_ERR;
//		goto label_readerr;
	}
	gDebugStep = 10;
	if((outbuf[outbytes-2]!=0x90)||(outbuf[outbytes-1]!=0x00))
	{
		if((outbuf[outbytes-2]==0x93)&&(outbuf[outbytes-1]==0x02))
		{
			gResponseStatus=CE_MAC1ERR;
			gGetCardInfoStep=0xff;
			return OP_ERR;
		}
		add_provedata();
		gDebugStep = outbuf[outbytes-2];
		goto label_invalid;
	}
	gDebugStep = 11;
	if(outbytes!=10)
	{
		add_provedata();
		goto label_invalid;
	}
	memcpy(gTAC,outbuf,4);
	memcpy(mac2,&outbuf[4],4);
	//credit sam for purchase
	memcpy(inbuf,"\x80\x72\x00\x00\x04",5);
	memcpy(&inbuf[5],mac2,4);
	inbytes = 9;
	ret=sam_apdu(bgSMobilepsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
	if(ret!=0)
	{
		goto label_samerr;
	}
	gDebugStep = 12;
	if((outbuf[outbytes-2]!=0x90)||(outbuf[outbytes-1]!=0x00))
	{
		gResponseStatus=CE_MAC2ERR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
	
	return 0;
label_writeerr:
		gResponseStatus=CE_WRITEERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_readerr:
		gResponseStatus=CE_READERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_invalid:
		gResponseStatus=CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_samerr:
		gResponseStatus=CE_PSAMERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
}
/*==========================================================
函数：trans_getproof_24G
功能：手机支付执行transprove命令
==========================================================*/
int trans_getproof_24G(UBYTE *sim_sn)
{
	UBYTE inbuf[257];
	UBYTE outbuf[257];
	UBYTE inbytes,outbytes;
	UBYTE cnt;
	UBYTE ret;
	UBYTE	resp;
	UWORD lastcnt;
	UBYTE tryflag=0;
	
	gDebugStep = 0x1101;
	add_provedata();//
	if(gStatus==IDLE_STATUS)
	{
label_try:
		ret = ZTE_GET_STATUS(&resp);
		if(ret)
		{
			gDebugStep = 0x1107;
			goto label_readerr;
		}
		if(resp==0)
		{
			ret = ZTE_REQ();
			if(ret)
			{
				gDebugStep = 0x1108;
				goto label_readerr;
			}
		}
		//选择应用
		memcpy(inbuf,"\x00\xA4\x04\x00\x10\xD1\x56\x00\x01\x01\x80\x03\x80\x00\x00\x00\x01\x00\x00\x00\x00",21);
		inbytes = 21;
		ret = ZTE_ICMD(inbytes,inbuf,&outbytes,outbuf,50);
		gDebugStep = 1;
		if(ret)
		{
			goto label_readerr;
		}
		if((outbuf[outbytes-2]!=0x90)||(outbuf[outbytes-1]!=0x00))
		{
			goto label_invalid;
		}
		gDebugStep = 0x1102;
		for(cnt=0;cnt<outbytes-2;cnt++)
		{
			if(outbuf[cnt]!=0x9f)
				continue;
			cnt++;
			if(outbuf[cnt]!=0x0c)
				continue;
			cnt++;
			if(outbuf[cnt]!=0x1e)
				goto label_invalid;
			cnt++;
			if((outbytes-cnt-2)<0x1e)
				goto label_invalid;
			break;
		}
		gDebugStep = 0x1103;
		if(cnt>=(outbytes-2))
			goto label_invalid;
		memcpy(gAppSn,&outbuf[cnt+10],10);
	}
	
	if(memcmp(gAppSn,sim_sn,10)!=0)
	{
		gDebugStep = 0x1109;
		gResponseStatus=CE_BADPARAM;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
	/*
	for(cnt=0;cnt<MAX_PROVE_INDEX;cnt++)
	{
		if(memcmp(gAppSn,ProveData[cnt].lastSimSn,10)==0)
		{
			break;
		}
	}
	if(cnt==MAX_PROVE_INDEX)
	{
		gDebugStep = 0x110A;
		gResponseStatus=CE_BADPARAM;
		gGetCardInfoStep=0xff;
		return OP_ERR;
	}
	*/
	buf_to_uword(ProveData[cnt].lastCardCnt,lastcnt);
	lastcnt++;
	memcpy(inbuf,"\x80\x5a\x00\x09\x02",5);
	uword_to_buf(lastcnt,&inbuf[5]);
	inbuf[7] = 8;
	inbytes = 8;
	ret = ZTE_ICMD(inbytes,inbuf,&outbytes,outbuf,30);
	gDebugStep = 0x110B;
	if(ret)
	{
		if(tryflag!=0)
			goto label_send_ff;
		tryflag=1;
		goto label_try;
//		goto label_readerr;
	}
	if((outbuf[outbytes-2]==0x94)&&(outbuf[outbytes-1]==0x06))
	{
label_send_ff:
		gDebugStep = 0x110C;
		memset(gTAC,0xff,4);
//		uword_to_buf(lastcnt,gCardCnt);
		gCardCnt = lastcnt-1;
//		memcpy(gPosTransCnt,ProveData[cnt].lastPosTransCnt,4);
		buf_to_udword(ProveData[cnt].lastPosTransCnt,gPosTransCnt);
		gKeyBacon = ProveData[cnt].keybacon;
		return 0;
	}
	gDebugStep = 0x110D;
	if((outbuf[outbytes-2]==0x90)&&(outbuf[outbytes-1]==0x00))
	{
//		if(outbytes!=10)
//		{
//			goto label_invalid;
//		}
		memcpy(gTAC,&outbuf[4],4);
//		uword_to_buf(lastcnt,gCardCnt);
		gCardCnt = lastcnt-1;
//		memcpy(gPosTransCnt,ProveData[cnt].lastPosTransCnt,4);
		buf_to_udword(ProveData[cnt].lastPosTransCnt,gPosTransCnt);
		gKeyBacon = ProveData[cnt].keybacon;
		//credit sam for purchase
		memcpy(inbuf,"\x80\x72\x00\x00\x04",5);
		memcpy(&inbuf[5],outbuf,4);
		inbytes = 9;
		ret=sam_apdu(bgSMobilepsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
		return 0;
	}
	goto label_send_ff;
label_readerr:
		gResponseStatus=CE_READERROR;
		gGetCardInfoStep=0xff;
		return OP_ERR;
label_invalid:
		gResponseStatus=CE_INVALIDCARD;
		gGetCardInfoStep=0xff;
		return OP_ERR;
}
#endif