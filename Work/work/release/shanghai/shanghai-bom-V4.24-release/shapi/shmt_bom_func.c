//shmt_bom_func.c

#ifndef _SHMT_BOM_FUNC_C_
#define _SHMT_BOM_FUNC_C_

#ifdef DEBUG_PRINT
#define _DEBUG_BOM_
#endif

#ifndef ZDD_HISTORY_DEBUG
#define ZDD_HISTORY_DEBUG
#endif

#define _HUAMING_GATE_

//版本号
//UBYTE gVersion = 0x89;	//与SAM通讯修改
//sam+pps切频
//UBYTE gVersion = 0x64; 	//未切频
//UBYTE gVersion = 0x8D;//460800的版本2015/2/3 15:28:33
//UBYTE gVersion = 0x81;//115200的版本2015/2/3 15:28:33
//UBYTE gVersion = 0x83;//115200的版本2015/8/31 15:10:51
//UBYTE gVersion = 0x90;//460800的版本2015/8/31 15:10:51
//UBYTE gVersion = 0x91;//460800的版本2016/5/16
//UBYTE gVersion = 0x95;//460800的版本2017/5/32
//UBYTE gVersion = 0x96;//460800的版本2019/6/26
//UBYTE gVersion = 0x97;   //2022.06.07 zdd
UBYTE gVersion = 0x98;   //2022.08.23 readcardreturn
UBYTE gRespStep;

UBYTE gStatus, gStatusNot;
cmd_sub_func_type *shmt_bom_func[256];
UBYTE oldsend[100],oldcmd,oldtimes;
UWORD oldsendcounter;
UBYTE gReadCmdTimes;
//bom
unsigned char  gBufBytes;
unsigned char  gBuf[MAX_BUF+1];
unsigned char  gOldCmd,gOldCmdTimes;
unsigned char  gSBuf[MAX_BUF+1];
unsigned char  gSBufBytes;
unsigned char  gMAC[6];
unsigned int  gNumTotal,gNumTotalNot;
unsigned long gValueTotal,gValueTotalNot;
unsigned long gShouquanSum,gShouquanSumNot;
unsigned long gShouquanTime,gShouquanTimeNot;
unsigned char gShouquanDate[7];
unsigned char gShaoquanSign,gShaoquanSignNot;
unsigned long gBackTransCnt,gBackTransCntNot;


UBYTE gMode;
UBYTE check_time_valid(void)
{
	UBYTE i,high,low;
	UBYTE buf[7];
	UBYTE code month[12]={31,29,31,30,31,30,31,31,30,31,30,31};
	UWORD tint;

	for(i=0;i<7;i++)
	{
	  high = gTransTime[i]/16;
	  if(high > 9) goto label_err;
	  low = gTransTime[i]%16;
	  if(low > 9) goto label_err;
	  buf[i] = high*10+low;
	}

*((char *)&tint+INT_HIGH) = buf[0];
*((char *)&tint+INT_LOW) = buf[1];
if(buf[2] > 12) goto label_err;
if(buf[2] == 0) goto label_err;
if(buf[3] == 0) goto label_err;
if(buf[3] > month[buf[2]-1]) goto label_err;
/*check run4 nian2*/
if(buf[2] == 2 && buf[3] == 29){
  if(tint%4 != 0) goto label_err;
  if(tint%400 != 0 && tint%100 == 0) goto label_err;
  }

if(buf[4] > 23) goto label_err;
if(buf[5] > 59) goto label_err;
if(buf[6] > 59) goto label_err;

return 0;

label_err:
//gResponseStatus = CE_BADPARAM;
return OP_ERR;

}
/*===============================================================================================
函数:shmt_bom_ini
功能:
=================================================================================================*/
void shmt_bom_ini(void)
{
	UWORD i;
	for(i=0;i<256;i++)
	{
		shmt_bom_func[i]=shmt_bom_func_bad_cmd;
	}
	
	shmt_bom_func[SM_INITREADER]=IniReader;
	shmt_bom_func[SM_RESETREADER]=ResetReader;
	shmt_bom_func[SM_STOPREADER]=StopReader;
	shmt_bom_func[SM_READERSTATUS]=ReaderStatus;
	shmt_bom_func[SM_BEEP]=ReaderBeep;
	shmt_bom_func[SM_DISPSIGNAL]=ReaderDisp;
	shmt_bom_func[SM_ABORT]=AbortReader;
	shmt_bom_func[SM_CHANGE_BAUD]=ReaderChangeBaud;
	
	shmt_bom_func[SM_READCARD]=ReadyToRead;
	shmt_bom_func[SM_ENTRY]=ShCardEntry;
	shmt_bom_func[SM_EXIT]=ShCardExit;
	shmt_bom_func[SM_LOCKCARD]=ShCardLock;
	shmt_bom_func[SM_READRECORD]=ShCardReadRecord;
	shmt_bom_func[SM_GETPROOF]=ShCardGetProof;
	shmt_bom_func[OTX_GETPROOF]=ShCardGetProofEx;
	shmt_bom_func[OT_LOCKCARD1]=ShCardLockCPU;
	shmt_bom_func[OT_READRECORD1]=ShCardReadRecordCPU;
	shmt_bom_func[OT_PAY]=ShCardPay;
	shmt_bom_func[SM_UPDATE]=ShCardUpdate;		//公交卡更新
	
	shmt_bom_func[ST_ACTIVECARD]=ShMetroActive;
	shmt_bom_func[HH_SALECARD]=ShMetroSale;
	shmt_bom_func[HH_ENTRY]=ShMetroEntry;
	shmt_bom_func[HH_ENTRY2]=ShMetroSPEntry;
	shmt_bom_func[HH_EXIT]=ShMetroExit;
	shmt_bom_func[HH_LOCK]=ShMetroLock;
	shmt_bom_func[HH_UPDATE]=ShMetroUpdate;
	shmt_bom_func[HH_READRECORD]=ShMetroRecord;
	shmt_bom_func[HH_GETPROOF]=ShMetroGetProof;
	shmt_bom_func[HH_ADDVALUE]=ShMetroAddValue;//单程票加值
	shmt_bom_func[HH_EXPIREDATA]=ShMetroExpireData;//单程票续期
	shmt_bom_func[HH_RETURNCARD]=ShMetroReturnCard;//单程票退票
	
	shmt_bom_func[MT_ENTRY]=ShMobileEntry;
	shmt_bom_func[MT_EXIT]=ShMobileExit;
	shmt_bom_func[MT_LOCK]=ShMobileLock;
	shmt_bom_func[MT_UPDATE]=ShMobileUpdate;
	shmt_bom_func[MT_GETPROOF]=ShMobileGetproof;
	
	shmt_bom_func[QR_ENTRY] = ShQREntry;
	shmt_bom_func[QR_EXIT] = ShQRExit;
	shmt_bom_func[QR_UPDATE] = ShQRUpdate;
	shmt_bom_func[QR_GETPROOF] = ShQRGetproof;
	shmt_bom_func[QR_READ] = ShQRRead;
	
//	shmt_bom_func[TEST_OPENRED]=ShMetrotestopenred;
//	shmt_bom_func[TEST_OPENGREEN]=ShMetrotestopengreen;
//	shmt_bom_func[TEST_CLOSERED]=ShMetrotestclosered;
//	shmt_bom_func[TEST_CLOSEGREEN]=ShMetrotestclosegreen;
//	shmt_bom_func[TEST_REQUEST]=ShMetrotestrequest;
//	shmt_bom_func[TEST_LOAD_SJT_KEY]=ShMetrotestloadkey;
//	shmt_bom_func[TEST_SJTAuthen]=ShMetrotestSJTauth;
//	shmt_bom_func[TEST_SJTREAD]=ShMetrotestSJTread;
//	shmt_bom_func[TEST_SJTWRITE]=ShMetrotestSJTwrite;
//	shmt_bom_func[readstep]=ShMetrotestReadDebugstep;
	//PF
	shmt_bom_func[FILE_TRANSFER]=ShMetroPfFileTransfer;
	shmt_bom_func[ACTIVE_TP_FILE]=ShMetroPfActiveTpFile;
	shmt_bom_func[PBOC_ENTRY]=ShMetroPfPbocEntry;
	shmt_bom_func[PBOC_EXIT]=ShMetroPfPbocExit;
	shmt_bom_func[PBOC_UPDATE]=ShMetroPfPbocUpdate;
	shmt_bom_func[PBOC_GETPROOF]=ShMetroPfPbocGetProof;
	shmt_bom_func[PBOC_GETPARAMVER]=ShMetroPfPbocGetParamVer;
	shmt_bom_func[PBOC_SETPARAM]=ShMetroPfPbocSetParam;
	shmt_bom_func[PBOC_READRECORD]=ShMetroPfGetRec;
	shmt_bom_func[ET_ENTRY]=ShMetroPfEtEntry;
	shmt_bom_func[ET_EXIT]=ShMetroPfEtExit;
	shmt_bom_func[ET_UPDATE]=ShMetroPfEtUpdate;
	shmt_bom_func[ET_GETPROOF]=ShMetroPfEtGetProof;
	shmt_bom_func[ET_LOCKCARD]=ShMetroPfEtLockCard;
	shmt_bom_func[ET_READRECORD]=ShMetroPfEtGetRec;	
	shmt_bom_func[READER_VER]=ShMetroReaderVer;
	shmt_bom_func[OT_GETHWVER]=ShMetroGethwVer;
	shmt_bom_func[TEST_24G]=ShMetro24GModel;
	
	//BOM相关命令
	shmt_bom_func[OTX_GETKEY]=shCardGetkey;
	shmt_bom_func[OT_CREDIT_INI]=shCardCreditInit;
	shmt_bom_func[OT_CREDIT]=shCardCredit;
	shmt_bom_func[OTX_UPDATE]=shCardUpdateEx;	//计时计费卡更新
	shmt_bom_func[OT_UNBLOCK]=shCardUnlock;
	shmt_bom_func[OT_GETRAN]=shCardGetRan;
	shmt_bom_func[OT_EXPIREDATE1]=shCardExpiredate1;
	shmt_bom_func[OT_FORBID]=shCardForbid;
	shmt_bom_func[OT_LOCKCARD1]=shCardLockcard1;
	shmt_bom_func[OT_CalcMAC]=shCardCalcmac;
	shmt_bom_func[OT_ADDVALUE1]=shCardAddvalue1;
	shmt_bom_func[OT_READRECORD1]=shCardReadrecord1;
	shmt_bom_func[OT_CREDIT_INI1]=shCardCreditInit1;
	shmt_bom_func[OT_ENABLECARD1]=shCardEnableCard1;
	shmt_bom_func[OT_ENABLECARD2]=shCardEnableCard2;
	//ODA
	//shmt_bom_func[ODA_READOK]
	//shmt_bom_func[ODA_READERR]	
	shmt_bom_func[ODA_ENTRY]=shODAEntry;
	shmt_bom_func[ODA_EXIT]=shODAExit;
	shmt_bom_func[ODA_UPDATE]=shODAUpdate;
	shmt_bom_func[ODA_GETPROOF]=shODAGetProof;
	shmt_bom_func[ODA_ENROLL]=shODAEnroll;
	shmt_bom_func[ODA_DISABLE]=shODADisable;

	//交通部
	shmt_bom_func[TRANSPORT_ENTRY]=shTransportEntry;
	shmt_bom_func[TRANSPORT_EXIT]=shTransportExit;
	shmt_bom_func[TRANSPORT_UPDATE]=shTransportUpdate;
	shmt_bom_func[TRANSPORT_GRTPROOF]=shTransportGetProof;
	shmt_bom_func[TRANSPORT_LOCKCARD]=shTransportLockCard;
	shmt_bom_func[TRANSPORT_SETPARAM]=shWhiteListSetParam;
	shmt_bom_func[TRANSPORT_RECORD]=shTransportRecord;
	shmt_bom_func[JTB_CREDIT_INI]=shJTBCreditInit;
	shmt_bom_func[JTB_CREDIT]=shJTBCredit;
	
	//QR
	shmt_bom_func[MQR_SETPARAM] = shCertSetParam;
	shmt_bom_func[MQR_TAC] = ShMQRTac;

	shmt_bom_func[TEST_HARDWARE_1]=ShTestHardWare;
	shmt_bom_func[SET_DATETIME] = shSetDateTime;
	gStatus=STOP_STATUS;
	gMode = STOP_MODE;
	return;
}
/************************************************************************************************
函数：shmt_bom
功能：上海地铁协议总入口
************************************************************************************************/
void shmt_bom(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	
	if(inbuf[1]!=0)
	{
		if(inbuf[0]!=SM_READCARD)
		{
			if(inbuf[0]==oldcmd)
			{
				if(inbuf[1]!=oldtimes)
				{
					memcpy(outbuf,oldsend,oldsendcounter);
					outbuf[1]=inbuf[1];
					*outbytes=oldsendcounter;
					oldtimes=inbuf[1];
					return;
				}
			}
		}
	}
	if(inbytes<2)
		goto label_bad_param;
	
	oldcmd=inbuf[0];
	oldtimes=inbuf[1];
#ifdef DEBUG_PRINT
	printf("command %02x inbytes %d\n", inbuf[0], inbytes);
#endif
	shmt_bom_func[inbuf[0]](inbuf,inbytes,outbuf,outbytes);
	
	//20100210add
#ifndef _HUAMING_GATE_
	if((UBYTE)inbuf[0] != SM_READERSTATUS){
		 clr_auto_abort_flag();
	} 
	else{
	   abort_cmd_cnt_inc();
	}
#else
	clr_auto_abort_flag();
#endif
	   
	return;
	
label_bad_param:
	outbuf[0] = inbuf[0];
	outbuf[1] = 0;
	outbuf[2] = ACK_BADPARAM;	
	*outbytes = 3;
	return;
}

/************************************************************************************************
函数：ReaderChangeBaud
功能：读卡器更改波特率
************************************************************************************************/
void ReaderChangeBaud(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
#ifdef _DEBUG_BOM_
debug_printf("\nReaderChangeBaud...");
#endif

	gRespStep = 0;
	if(inbytes!=3)
	{
		goto label_para_err;
	}
	if(inbuf[2]>5)
	{
		goto label_para_err;
	}
	switch(inbuf[2])
	{
		case 0:
				gRespStep = 3;
				gCurBaudRate = 9600;
				break;
		case 1:
				gRespStep = 3;
				gCurBaudRate = 19200;
				break;
		case 2:
				gRespStep = 3;
				gCurBaudRate = 28800;
				break;
		case 3:
				gRespStep = 3;
				gCurBaudRate = 38400;
				break;
		case 4:
				gRespStep = 3;
				gCurBaudRate = 57600;
				break;
		case 5:
				gRespStep = 3;
				gCurBaudRate = 115200;
				break;
		default:
				goto label_para_err;
	}
	outbuf[0]=oldcmd;
	outbuf[1]=oldtimes;
	outbuf[2]=CE_OK;
	*outbytes =3;
	return;
label_para_err:
	gResponseStatus = CE_BADPARAM;
	shmt_bom_func_err(outbuf,outbytes);
	return;	
}
/************************************************************************************************
函数：IniReader
功能：初始化读卡器
************************************************************************************************/
void IniReader(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
		UBYTE buf[24];
		int ret;
#ifdef _DEBUG_BOM_
debug_printf("\nIniReader...");
#endif
	
	if(inbytes!=14)
	{
		goto label_para_err;
	}
	gPosNo[0]=inbuf[2];
	gPosNo[1]=inbuf[3];
	gPosNo[2]=0x18;
	gPosNo[3]=inbuf[4];
	if((inbuf[5]!=0x00)&&(inbuf[5]!=0x01))	//时间格式
	{
		goto label_para_err;
	}
	if((inbuf[6]!=0x00) && (inbuf[6]!=0x01))							//SAM卡类型
	{
		goto label_para_err;
	}
	gTransTime[0]=0x20;
	gTransTime[1]=0x08;
 	 memcpy(&gTransTime[2],&inbuf[7],5);
	switch(inbuf[13])
	{
  	case 0x20:gMode   = ENTRY_MODE;
  						gPosNo[2] = 0x1a;
  						break;
  	case 0x40:gMode   = EXIT_MODE;
  						gPosNo[2] = 0x15;
  						break;
  	case 0x60:gMode   = BOM_MODE;
  						break;
  	case 0x80:gMode   = ATVM_MODE;
  						gPosNo[2] = 0x13;
  						break;
  	default:	goto label_para_err;
  }
  if(gMode==BOM_MODE)
  {
  	buf[0] = 0x14;
  }
  else
  {
  	buf[0] = 0x11;
  }
  buf[1] = gPosNo[0];
  buf[2] = gPosNo[1];
  buf[3] = gPosNo[3];
  
//  #ifndef _USEOLDPSAM_
  if(memcmp(buf,bpgSHpsamNo,4)!=0)
  {
  	if(ModifySamID(buf)!=0)
  	{
  		if((bgSmPsamValid==0)&&(bgSMobilePsamValid==0))
  		{
  			gResponseStatus = CE_NOKEY;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			}	
  	}
  }
//  #endif
  gStatus = IDLE_STATUS;
	gGetCardInfoStep = 0xff;
	
	
	#ifdef _RECOVERY_AFTER_RESET_
  memcpy(buf,inbuf,14);
  lrc_gen(buf,14);
  ee_write(ADDR_RECOVERY_DATA,15,buf);
  #endif
	
	rf_select(RF_CHANNEL);
	mcml_pwr_off();
	set_card_type(ISO14443A_SH_TYPE);
	outbuf[0]=oldcmd;
	outbuf[1]=oldtimes;
	outbuf[2]=CE_OK;
	outbuf[3]=gMode|gStatus;
	*outbytes =4;
		//手机支付交易终端设备参数初始化
	memcpy(buf,gPosNo,4);
	memcpy(&buf[4],bpgSmpsamNu,6);
	
	//debug_printf("\n bpgSmpsamNu =%02x %02x %02x %02x %02x %02x",bpgSmpsamNu[0],bpgSmpsamNu[1],bpgSmpsamNu[2],bpgSmpsamNu[3],bpgSmpsamNu[4],bpgSmpsamNu[5]);

  /*
  if(check_24G_module())  bpgChk24GModuleFlag = 1;
  else bpgChk24GModuleFlag = 0;
  */
	pf_mobile_ticket_device_init(buf);

	//debug_printf(" shmt_read_pf4016 ");
	//shmt_read_pf4016();
	//ret = pf_mobile_ticket_para_init(buf_4016);
	//debug_printf(" pf_mobile_ticket_para_init ret=%X",ret);
	return;
label_para_err:
	gResponseStatus = CE_BADPARAM;
	shmt_bom_func_err(outbuf,outbytes);
	return;	
}
/************************************************************************************************
函数：ResetReader
功能：复位读卡器
************************************************************************************************/
void ResetReader(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	
	UBYTE buf[10];
	unsigned char name_4016[10];
#ifdef _DEBUG_BOM_
debug_printf("\nResetReader...\n");
#endif

	if(inbytes!=3)
	{
			goto label_para_err;
	}
	gStatus = STOP_STATUS;
//	gMode = STOP_MODE;
	gGetCardInfoStep = 0xff;
	gTaskBusyFlag = 1;
	
	ResetShangHaiCardSam();
	ResetShangHaiMetroSam();
	//ResetShangHaiMobileSam();//2015/1/12 16:54:19
	ResetShTransportSam();
	rf_select(RF_CHANNEL); 
	mcml_pwr_off();
	set_card_type(ISO14443A_SH_TYPE);

	outbuf[0]=oldcmd;
	outbuf[1]=oldtimes;
	outbuf[2]=CE_OK;
	if(gMode==BOM_MODE)
	{
		outbuf[3]=BOM_READER;
	}
	else
	{
		if(gMode==ENTRY_MODE)
		{
			outbuf[3]=GATE_ENTRY_READER;
		}
		else
		{
			outbuf[3]=GATE_EXIT_READER;
		}
	}
	outbuf[4]=gVersion;
	//if(!(bgSHPsamValid||bgSmPsamValid||bgSMobilePsamValid))
	if(!(bgSHPsamValid||bgSmPsamValid))
	{
		outbuf[2]=CE_NOKEY;
		*outbytes=3;
		return;
	}
	if(bgSHPsamValid)
	{
		outbuf[5]=0;
	}
	else
	{
		outbuf[5]=2;
	}
	memcpy(&outbuf[6],bpgSHpsamNo,4);
	memcpy(&outbuf[10],bpgSmpsamNo,4);
	//memcpy(&outbuf[14],bpgSMobilepsamNo,6);
	memcpy(&outbuf[14],ch_transport_psam_id,6);
	memset(&outbuf[20],0x00,6);  //2022.06.08zdd增加
	outbuf[26]=0;
	*outbytes=27;
	//*outbytes=15;
	//
	bgResetFlag = 0;
	DisableAll();
	//手机支付交易终端设备参数初始化
	//memcpy(buf,bpgSmpsamNu,4);
	//memcpy(&buf[4],bpgSmpsamNu,6);
	//debug_printf("\n bpgSmpsamNu =%02x %02x %02x %02x %02x %02x",bpgSmpsamNu[0],bpgSmpsamNu[1],bpgSmpsamNu[2],bpgSmpsamNu[3],bpgSmpsamNu[4],bpgSmpsamNu[5]);
	//pf_mobile_ticket_device_init(buf);
  // 
  
	return;
		
label_para_err:
	gResponseStatus = CE_BADPARAM;
	shmt_bom_func_err(outbuf,outbytes);
	return;
}
/************************************************************************************************
函数：StopReader
功能：停止读卡器
************************************************************************************************/
void StopReader(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
#ifdef _DEBUG_BOM_
debug_printf("\nStopReader...");
#endif

	if(inbytes!=3)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = STOP_STATUS;
	gMode = STOP_MODE;
	gGetCardInfoStep = 0xff;
	outbuf[0]=oldcmd;
	outbuf[1]=oldtimes;
	outbuf[2]=CE_OK;
	*outbytes=3;
	return;	
}

/************************************************************************************************
函数：ReaderBeep
功能：读卡器鸣叫
************************************************************************************************/
void ReaderBeep(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
#ifdef _DEBUG_BOM_
debug_printf("\nReaderBeep...");
#endif

	if(inbytes!=3)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	
	if(inbuf[2] == 1) 
	{
		beep(0);
		usleep(100 * 1000);
		beep(1);
	}
	
	if(inbuf[2] == 2)
	{
		beep(0);
		usleep(100*1000);
		beep(1);
		usleep(30*1000);
		beep(0);
		usleep(100*1000);
		beep(1);
	}
	outbuf[0]=oldcmd;
	outbuf[1]=oldtimes;
	outbuf[2]=CE_OK;
	outbuf[3]= 0;
	*outbytes=4;
	return;	
}

/************************************************************************************************
函数：ReaderStatus
功能：读取读卡器状态
************************************************************************************************/
void ReaderStatus(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
UBYTE	buf[200], cpubuf[200];
UWORD cpulen;
int ret;

#ifdef _DEBUG_BOM_
debug_printf("\nReaderStatus...");
#endif

	if(inbytes!=2)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

	outbuf[0]=oldcmd;
	outbuf[1]=oldtimes;
	outbuf[2]=CE_OK;
	if(gStatus == TRANS_STATUS)
	{
		switch(gCardFamily)
		{
		case FAMILY_SJT:
			if(tocken_read(9,buf) != 0)
				gStatus = IDLE_STATUS;
			break;
		case FAMILY_M1:
			if( mcmh_read(lastBlock, buf, 2, 0) != 0)
				gStatus = IDLE_STATUS;
			break;
		case FAMILY_CPU_CARD:
		case FAMILY_CPU_PF:
		case FAMILY_CPU_EM:
			memcpy(buf, "\x00\x84\x00\x00\x04", 5);
			ret = mifpro_icmd(buf, 5, cpubuf, &cpulen);
			if(ret)
			{
				gStatus = IDLE_STATUS;
			}
			break;
		case FAMILY_QR:
			/*if(BLEStatus(buf, &cpulen) == 0)
			{
				if(buf[0] == 0)
					gStatus = IDLE_STATUS;
			}
			break;*/ //2022.06.08 zdd
			break;
			
		}
	}
	outbuf[3]=gMode|gStatus;

	*outbytes=4;
	return;
}
/************************************************************************************************
函数：ReaderDisp
功能：读取读卡器状态
************************************************************************************************/
void ReaderDisp(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
#ifdef _DEBUG_BOM_
debug_printf("\nReaderDisp...");
#endif

	if(inbytes!=4)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if((inbuf[2]&0x0f==0)&&(inbuf[3]!=0))
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(((inbuf[2]&0xf0)!=0x10)&&((inbuf[2]&0xf0)!=0x20))
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}	
	display_set(inbuf[2],inbuf[3]);
	outbuf[0]=oldcmd;
	outbuf[1]=oldtimes;
	outbuf[2]=CE_OK;
	outbuf[3]=0x00;
	*outbytes = 4;
	return;	
}
/************************************************************************************************
函数：AbortReader
功能：中止读卡器操作
************************************************************************************************/
void AbortReader(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
UBYTE buf[5];

#ifdef _DEBUG_BOM_
debug_printf("\nAbortReader...");
#endif
	
	if(inbytes!=3)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
#ifdef _DEBUG_BOM_
debug_printf("\ngMode=%X gStatus=%X bgResetFlag=%X ",gMode,gStatus,bgResetFlag);
#endif

		bgTest24GFlag = 1;	//2013-09-03

	//if((gStatus == STOP_STATUS) && (!bgResetFlag))
	if(gStatus == STOP_STATUS)
	{
		outbuf[0]=oldcmd;
		outbuf[1]=oldtimes;
		outbuf[2]=CE_STATUSERROR;
		outbuf[3]=gMode|gStatus;
		*outbytes=4;
		return;
	}

	gStatus = IDLE_STATUS;
	gGetCardInfoStep = 0xff;
	//gTaskBusyFlag = 0;//2011-9-15 9:00:40
	gTaskBusyFlag = 1; //2013-09-02 20:25:00
	bgTest24GFlag = 1;	//2013-09-03
	gAbortReaderFlag = 1; //2013/11/13 19:34:09
	
	set_card_type(ISO14443A_SH_TYPE);
	rf_select(RF_CHANNEL);
	mcml_pwr_off();
	//
	memcpy(buf,"\xa2\x32\x0\x0",4);
  //prot_zte_send_block(buf,4); 
	//zte_set_wait(200);
	//prot_zte_rece_process(30);
	//ZTE_HALT();
	//#ifdef _DEBUG_BOM_
	//debug_printf("\ngMode=%X gStatus=%X bgResetFlag=%X ",gMode,gStatus,bgResetFlag);
	//#endif
	//QR 
	DisableAll();
	//BLE
	bleOffline();
	
	
	outbuf[0]=oldcmd;
	outbuf[1]=oldtimes;
	outbuf[2]=CE_OK;
	outbuf[3]=gMode|gStatus;
	*outbytes=4;
	//
	return;
}
/************************************************************************************************
函数：ReadyToRead
功能：开始读卡信息
************************************************************************************************/
void ReadyToRead(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
#ifdef _DEBUG_BOM_
debug_printf("\nReadyToRead...");
#endif

	if(inbytes!=3)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus != IDLE_STATUS)
	{ 
    gResponseStatus = CE_STATUSERROR;
    shmt_bom_func_err(outbuf,outbytes);
		return;
  }
  //
  ReadMode=inbuf[2];
  gReadCmdTimes = inbuf[1];
  //
  switch(gMode)
  {
  	case ENTRY_MODE:
  	case EXIT_MODE:
  			if((inbuf[2]!=00)&&(inbuf[2]!=0x80)&&(inbuf[2]!=0x90))
  			{
				gResponseStatus = CE_BADPARAM;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			  }
			break;
	  case BOM_MODE:
			if((inbuf[2]!=0x81)&&(inbuf[2]!=0x91))
  			{
				gResponseStatus = CE_BADPARAM;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			  }
			break;
	  case ATVM_MODE:
			if((inbuf[2]!=0x02)&&(inbuf[2]!=0x80)&&(inbuf[2]!=0x90))
  			{
				gResponseStatus = CE_BADPARAM;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			  }
			break;
	  default:
			gResponseStatus = CE_BADPARAM;
			shmt_bom_func_err(outbuf,outbytes);
			return;
  }
  //2011-4-2 12:43:04
  //gStatus = IDLE_STATUS;
  //AbortReader(inbuf,3,outbuf,outbytes);
  //
	gStatus = READ_STATUS;
  gGetCardInfoStep=0;
  gTaskBusyFlag=0;
  //2013/11/13 19:33:26
  gAbortReaderFlag=0;
  //20131112?
  timer_set(TIMER_MS_POWEROFF,30000);
	gPowerOffFlag = 0;
	gTransTypeFlag = 0;
	outbuf[0] = SM_READCARD;
	outbuf[1] = gReadCmdTimes;
	outbuf[2] = 0;
	outbuf[3] = 0;
	outbuf[4] = 0;
	*outbytes =5;

	//QR
	InitScan();
	StartScan();
	//sem_init(&g_samcalreturn, 0, 0);
	sem_post(&g_usermainwait);
	
	//getcardinfo_CPU();
	return;
}
/************************************************************************************************
函数：ShCardEntry
功能：上海公交卡进站交易
************************************************************************************************/
void ShCardEntry(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE ret;
	UBYTE buf[20];

#ifdef _DEBUG_BOM_
debug_printf("\nShCardEntry...");
#endif
	
	gRespStep = 0;
	if(gMode != ENTRY_MODE)
	{
		gResponseStatus = CE_BADCOMMAND;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(inbytes!=13)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = WRITE_STATUS;
	memcpy(gTransTime,&inbuf[2],7);
	if(check_time_valid() != 0) 
	{
		gStatus=IDLE_STATUS; 
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	*((char *)&gPosTransCnt+LONG_HIGH3) = 0;
 	*((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[9];
  	*((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[10];
  	*((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[11];
 	if(inbuf[12]&0x18)
	{
		gStationTimeOverRideFlag=inbuf[12];
	}
	else
	{
		gStationTimeOverRideFlag=0;
	}
	if(gCPUflag==0x55)
	{
		ret = trans_entry_cpu();
		mcml_pwr_off();
	}
	else
	{
		ret=trans_entry();
		mcml_pwr_off();//
	}
	if(ret != 0)
	{
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = *((char *)&gCardCnt+INT_HIGH);
	outbuf[4] = *((char *)&gCardCnt+INT_LOW);	
	outbuf[5] = gTAC[0]; 
	outbuf[6] = gTAC[1];
	outbuf[7] = gTAC[2];
	outbuf[8] = gTAC[3];
	*outbytes = 9; 
	gRespStep = 1;
	gStatus=IDLE_STATUS;

	buf[0]=*((char *)&gPosTransCnt+LONG_HIGH2);
	buf[1]=*((char *)&gPosTransCnt+LONG_HIGH1);
	buf[2]=*((char *)&gPosTransCnt+LONG_HIGH0);
	if(gCPUflag==0x55)
	{
		memcpy(&buf[3],&gAppSn[6],4);
	}
	else
	{
		memcpy(&buf[3],gThisCardSnr,4);
	}
	buf[7]=*((char *)&gCardCnt+INT_HIGH);
	buf[8]=*((char *)&gCardCnt+INT_LOW);
	buf[9]=*((char *)&gCardThisRemain+LONG_HIGH0);
	buf[10]=*((char *)&gCardThisRemain+LONG_HIGH1);
	buf[11]=*((char *)&gCardThisRemain+LONG_HIGH2);
	memcpy(&buf[12],gTAC,4);
	save_trans_record(buf);	
	
	#ifdef _RECOVERY_AFTER_RESET_
	if(bgResetWrTimeFlag){
		 rtc_wr_time(&gTransTime[1]);
		 bgResetWrTimeFlag=0;
		 }
	#endif
	return;
}
/************************************************************************************************
函数：ShCardExit
功能：上海公交卡出站交易
************************************************************************************************/
void ShCardExit(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE transtype;
	UBYTE buf[20];
	UBYTE ret;

#ifdef _DEBUG_BOM_
debug_printf("\nShCardExit...");
#endif

	gRespStep = 0;
	if(gMode != EXIT_MODE)
	{
		gResponseStatus = CE_BADCOMMAND;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(inbytes!=18)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = WRITE_STATUS;
	memcpy(gTransTime,&inbuf[2],7);
	if(check_time_valid() != 0) 
	{
		gStatus=IDLE_STATUS; 
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	*((char *)&gPosTransCnt+LONG_HIGH3) = 0;							  			
  *((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[9];
  *((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[10];
  *((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[11];
	*((char *)&gCardThisTransValue+LONG_HIGH3) = 0;
	*((char *)&gCardThisTransValue+LONG_HIGH2) = inbuf[14];
	*((char *)&gCardThisTransValue+LONG_HIGH1) = inbuf[13];
	*((char *)&gCardThisTransValue+LONG_HIGH0) = inbuf[12];
	transtype= inbuf[15];
	if(ReadMode==0)
	{
		if(transtype>2)
		{
			gStatus=IDLE_STATUS;
			gResponseStatus=CE_BADPARAM;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
	}
	else	
	{
		if((transtype>12)||(transtype==2)||(transtype==3)||(transtype==4))
		{
			gStatus=IDLE_STATUS;
			gResponseStatus=CE_BADPARAM;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
	}
	#ifdef _DEBUG_BOM_
			debug_printf("\x0d\x0a entry function :trans_exit");
	#endif
	if(gCPUflag==0x55)
	{
		ret = trans_exit_cpu(transtype);
		//2013/10/29 16:36:34
		//mifpro_deselect(0,outbuf,outbytes);
		mcml_pwr_off();
	}
	else
	{
		ret = trans_exit(transtype);
		mcml_pwr_off();//
	}
	if(ret!=0)
	{
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = *((char *)&gCardCnt+INT_HIGH);
	outbuf[4] = *((char *)&gCardCnt+INT_LOW);
	outbuf[5] = *((char *)&gCardThisRemain+LONG_HIGH0);
	outbuf[6] = *((char *)&gCardThisRemain+LONG_HIGH1);
	outbuf[7] = *((char *)&gCardThisRemain+LONG_HIGH2); 	
	outbuf[8] = gTAC[0];
	outbuf[9] = gTAC[1];
	outbuf[10]= gTAC[2];
	outbuf[11]= gTAC[3];		
	*outbytes = 12;
	gStatus=IDLE_STATUS;
	gRespStep = 2;

	buf[0]=*((char *)&gPosTransCnt+LONG_HIGH2);
	buf[1]=*((char *)&gPosTransCnt+LONG_HIGH1);
	buf[2]=*((char *)&gPosTransCnt+LONG_HIGH0);
	if(gCPUflag==0x55)
	{
		memcpy(&buf[3],&gAppSn[6],4);
	}
	else
	{
		memcpy(&buf[3],gThisCardSnr,4);
	}
	buf[7]=*((char *)&gCardCnt+INT_HIGH);
	buf[8]=*((char *)&gCardCnt+INT_LOW);
	buf[9]=*((char *)&gCardThisRemain+LONG_HIGH0);
	buf[10]=*((char *)&gCardThisRemain+LONG_HIGH1);
	buf[11]=*((char *)&gCardThisRemain+LONG_HIGH2);
	memcpy(&buf[12],gTAC,4);
	save_trans_record(buf);
  
  #ifdef _RECOVERY_AFTER_RESET_
	if(bgResetWrTimeFlag){
		rtc_wr_time(&gTransTime[1]);
	  bgResetWrTimeFlag=0;
	  }
	#endif
	return;
}
/************************************************************************************************
函数：ShCardLock
功能：上海公交卡锁卡交易
************************************************************************************************/
void ShCardLock(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE TransLockType;

#ifdef _DEBUG_BOM_
debug_printf("\nShCardLock...");
#endif

	if(inbytes!=13)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	memcpy(gTransTime,&inbuf[2],7);
	if(check_time_valid() != 0) 
	{
		gStatus=IDLE_STATUS; 
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	*((char *)&gPosTransCnt+LONG_HIGH3) = 0;
  *((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[9];
  *((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[10];
  *((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[11];
  TransLockType = inbuf[12];
	gStatus = IDLE_STATUS;
	gGetCardInfoStep = 0xff;
	if(trans_lock(TransLockType) != 0) 
	{
		shmt_bom_func_err(outbuf,outbytes);
		return;
  }
  outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
  if(TransLockType==0)
  {
  	outbuf[3] = 1;
  }
  else
  {
  	outbuf[3] = 3;
  }
  *outbytes = 4;
	return;	
}
/************************************************************************************************
函数：ShCardLockCPU
功能：上海公交卡CPU卡锁卡交易
************************************************************************************************/
void ShCardLockCPU(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
#ifdef _DEBUG_BOM_
debug_printf("\nShCardLockCPU...");
#endif

	if(inbytes!=8)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(trans_lock_cpu(&inbuf[2],inbuf[6]))
	{
		mcml_pwr_off();
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	mcml_pwr_off();
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	*outbytes = 3;

	return;
}
/************************************************************************************************
函数：ShCardReadRecord
功能：上海公交卡读卡上交易记录及校验B28
************************************************************************************************/
void ShCardReadRecord(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE i,buf[50],ret;
	UBYTE obuf[256];
	UWORD j;
	UDWORD temp1,temp2,temp3;
	UWORD tint;

#ifdef _DEBUG_BOM_
debug_printf("\nShCardReadRecord...");
#endif

#ifdef ZDD_HISTORY_DEBUG
printf("\nzdd-ShCardReadRecord...\n");
#endif

//	if(gMode != EXIT_MODE)
//	{
//		gResponseStatus = CE_BADCOMMAND;
//		goto label_err;
//	}
	tint = inbuf[2];
	ret=Transport_GetFiles15(outbuf);

	if(ret<0)
	{
		goto label_err;
	}

	if(inbytes!=4)
	{
		#ifdef ZDD_HISTORY_DEBUG
		printf("\nzdd-inbytes! = 4\n");
		#endif
		gResponseStatus = CE_BADPARAM;
		goto label_err;
	}
	if(gStatus==STOP_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		goto label_err;
		gResponseStatus = CE_STATUSERROR;
		goto label_err;
	}
	#ifdef ZDD_HISTORY_DEBUG
			printf("\nzdd-zhujianbu\n");
			#endif
	if(gCPUflag==0x55)
	{
		buf_to_uword(&inbuf[2],j);
		gRelativePtr=0;
		switch(j)
		{
			case 0x0001:gRelativePtr = 0;
									break;
			case 0x0002:gRelativePtr = 1;
									break;
			case 0x0004:gRelativePtr = 2;
									break;
			case 0x0008:gRelativePtr = 3;
									break;
			case 0x0010:gRelativePtr = 4;
									break;
			case 0x0020:gRelativePtr = 5;
									break;
			case 0x0040:gRelativePtr = 6;
									break;
			case 0x0080:gRelativePtr = 7;
									break;
			case 0x0100:gRelativePtr = 8;
									break;
			case 0x0200:gRelativePtr = 9;
									break;
			case 0x0400:gRelativePtr = 10;
									break;
			case 0x0800:gRelativePtr = 11;
									break;
			case 0x1000:gRelativePtr = 12;
									break;
			default:gResponseStatus = CE_BADPARAM;
							goto label_err;
		}
		j=j<<3;
		if(history_record(j,&buf[0]))
		{
			goto label_err;
		}
		goto label_an;
	}
	//
//	for(i=0;i<20;i++)
//	{
//		if(mcml_request(PICC_REQSTD,buf)==0)
//		{
//			goto label_con;
//		}
//		delay_ms(3);
//	}
//cardmoved:
//	gResponseStatus = CE_CARDREMOVED;
//	goto label_err;
//
//label_con:
//	i=mcml_anticoll(gThisCardSnr);
//	if(i!=0)
//	{
//		goto cardmoved;
//	}
//	i=mcml_select(gThisCardSnr,buf);
//	if(i!=0)
//	{
//		goto cardmoved;
//	}
//	
	//充值记录
	if(inbuf[3]&0x01==0x01)
	{
		if(NormalCard==COMMCARD)
		{
			memcpy(gKeyA[0],"\xfc\x61\xcb\xb5\x37\x51",6);
			i=mcmh_read(1,buf,0,0);
			if(i!=0)
			{
				memcpy(gKeyA,"\x70\xa4\xf4\x82\x01\x6d",6);
				i=mcmh_read(1,buf,0,0);
				if(i!=0)
				{
					memset(&outbuf[26],0,5);
				}
				else
				{
					memcpy(&outbuf[26],&buf[1],5);
				}
				goto n1;
			}
			else
			{
				memcpy(&outbuf[26],&buf[1],5);
			}
	n1:
			i=mcmh_read(9,buf,0,0);
			if(i!=0)
			{
				gResponseStatus = CE_READERROR;
				goto label_err;
			}		
			memcpy(&outbuf[4],buf,14);
			i=mcmh_read(10,buf,2,0);
			if(i!=0)
			{
				gResponseStatus = CE_READERROR;
				goto label_err;
			}	
			memcpy(&outbuf[18],buf,8);
			memcpy(&outbuf[31],&buf[8],4);
			outbuf[35]=0;
		}
		if(NormalCard==CNTCARD)
		{
			memcpy(gKeyA[0],"\xfc\x61\xcb\xb5\x37\x51",6);
			i=mcmh_read(1,buf,0,0);
			if(i!=0)
			{
				memcpy(gKeyA,"\x70\xa4\xf4\x82\x01\x6d",6);
				i=mcmh_read(1,buf,0,0);
				if(i!=0)
				{
					memset(&outbuf[4],0,5);
				}
				else
				{
					memcpy(&outbuf[4],&buf[2],4);
				}
				goto n2;
			}
			memcpy(&outbuf[4],&buf[2],4);
		n2:
			outbuf[15]=buf[1];
			i=mcmh_read(13,buf,0,0);
			if(i!=0)
			{
				gResponseStatus = CE_READERROR;
				goto label_err;
			}
			memcpy(&outbuf[8],&buf[7],4);
			outbuf[12]=buf[5];
			outbuf[13]=buf[6];
			i=mcmh_read(8,buf,0,0);
			if(i!=0)
			{
				gResponseStatus = CE_READERROR;
				goto label_err;
			}
			outbuf[14]=buf[4];
			i=mcmh_read(4,buf,0,0);
			if(i!=0)
			{
				gResponseStatus = CE_READERROR;
				goto label_err;
			}
			memcpy(&outbuf[16],&buf[11],4);
			outbuf[0]=oldcmd;
			outbuf[1]=oldtimes;
			outbuf[2]=CE_OK;
			outbuf[3]=0x00;
			*outbytes =20;
			return;			
		}
		outbuf[0]=oldcmd;
		outbuf[1]=oldtimes;
		outbuf[2]=CE_OK;
		outbuf[3]=0x00;
		*outbytes=36;
		return;
	}
	//普通记录
	gRelativePtr=0;
	if((inbuf[3]&0x02)==0x02)      {gRelativePtr=1;  goto label_ok;}
	if((inbuf[3]&0x04)==0x04)      {gRelativePtr=2;  goto label_ok;}
	if((inbuf[3]&0x08)==0x08)      {gRelativePtr=3;  goto label_ok;}
	if((inbuf[3]&0x10)==0x10)      {gRelativePtr=4;  goto label_ok;}
	if((inbuf[3]&0x20)==0x20)      {gRelativePtr=5;  goto label_ok;}
	if((inbuf[3]&0x40)==0x40)      {gRelativePtr=6;  goto label_ok;}
	if((inbuf[3]&0x80)==0x80)      {gRelativePtr=7;  goto label_ok;}
	if((inbuf[2]&0x01)==0x01)      {gRelativePtr=8;  goto label_ok;}
	if((inbuf[2]&0x02)==0x02)      {gRelativePtr=9;  goto label_ok;}
	if((inbuf[2]&0x04)==0x04)      {gRelativePtr=10; goto label_ok;}
  //认证读B28
  
	if((inbuf[2]&0x08)==0x08)
	{
		gRelativePtr=11;
		i=mcmh_read(28,buf,0,0);
		if(i)
		{
			gResponseStatus =CE_READERROR;
	  		goto label_err;
		}
		  
		memcpy(&outbuf[4],buf,16);
		  //非老年卡 数据全为0 直接返回
			//if(memcmp(buf,"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",16)==0)
			//{
				//memcpy(&outbuf[1],buf,16);
				//memset(&outbuf[17],0,7);
				//outbuf[3] = 11;
				//return ;
			//}
			
		ret=buf[0];
		for(i=1;i<15;i++)
		{
			ret^=buf[i];
		}
		ret^=0xaa;
		if(ret!=buf[15])
		{
			//gResponseStatus =CE_INVALIDCARD;
  			//gResponseStatus = 0x8B;
  			//gResponseStatus = gRelativePtr | 0x80;
  			//goto label_err;
			outbuf[0] = oldcmd;
			outbuf[1] = oldtimes;
			outbuf[2] = CE_OK;
			outbuf[3] = gRelativePtr | 0x80;
			*outbytes =4;
			return;
		}
			//计算TAC
//			goto label_an;
			//os_lock();
			memcpy(bgTaskKeyInbuf,buf,13);
			bgTaskKeyInbuf[13]=gCityCode0;
			bgTaskKeyInbuf[14]=gCityCode1;
			memcpy(&bgTaskKeyInbuf[15],gThisCardSnr,4);
			memset(&bgTaskKeyInbuf[19],0,13);
			//bgTaskKeyStep=TASK_KEY_GET_SHB28;
			set_task_key(TASK_KEY_GET_SHB28);
			//os_unlock();
			//gDebugStep = 0x4001;
			//if(bgEnUCOS) OSTimeDly(1);
			//获得TAC
			//sem_wait(&g_samcalreturn);
//			if(bgTaskKeyFinishState != 0)
//			{
//	  		gResponseStatus = CE_PSAMERROR;
//				goto label_err;
//			}
			gDebugStep = 0x4001;
			for(j=0;j<1000;j++)
			{
				if(bgTaskKeyStep==TASK_KEY_FINISH)
				{
					break;
				}
				task_key();
				//if(bgEnUCOS) OSTimeDly(1);
				//delay_ms(1);
			}
			gDebugStep = 0x4002;
			if(j==1000)
			{
				gResponseStatus = CE_PSAMERROR;
				goto label_err;
			}	
			//bgTaskKeyStep = TASK_KEY_IDLE;
			set_task_key(TASK_KEY_IDLE);
			gDebugStep = 0x4003;
			if(bgTaskKeyFinishState!=0)	
			{
				gResponseStatus = CE_PSAMERROR;
				goto label_err;
			}
			gDebugStep = 0x4004;
			memcpy(gTAC,bgTaskKeyOutbuf,4);
			
			if((buf[13]!=gTAC[0])||(buf[14]!=gTAC[1]))
			{
				gResponseStatus =CE_INVALIDCARD;
	  		goto label_err;
			}
			goto label_an;
	}
	if((inbuf[2]&0x10)==0x10)
	{
		gRelativePtr=12;
		i=mcmh_read(28,buf,0,0);
		if(i)
		{
			gResponseStatus = CE_READERROR;
			goto label_err;
	    }
	    //
	    ret=buf[0];
		for(i=1;i<15;i++)
		{
			ret^=buf[i];
		}
		ret^=0xaa;
		if(ret!=buf[15])
		{
  			//gResponseStatus =CE_INVALIDCARD;
  			//gResponseStatus = 0x8C;
  			//gResponseStatus = gRelativePtr | 0x80;
  			//goto label_err;
			outbuf[0] = oldcmd;
			outbuf[1] = oldtimes;
			outbuf[2] = CE_OK;
			outbuf[3] = gRelativePtr | 0x80;
			*outbytes =4;
			return;
		}
	    //
		goto label_an;
	}
	if((inbuf[2]&0x20)==0x20)	
	{
		gRelativePtr=13;
		i=mcmh_read(29,buf,0,0);
		if(i)
		{
	  	  	gResponseStatus = CE_READERROR;
			goto label_err;
	    }
		goto label_an;
	}	
label_ok:
	if(gRelativePtr < 1) 
	{
		gResponseStatus = CE_BADPARAM;
		goto label_err;
	}
	if(gRelativePtr > 13) 
	{
		gResponseStatus = CE_BADPARAM;
		goto label_err;
	}
	i=mcmh_read(13,buf,0,0);
	if(i)
	{
	  	gResponseStatus = CE_READERROR;
		goto label_err;
	}
	gCardRecPtr=buf[12];
	if(trans_readrecord(&buf[1]) != 0) 
	{
		goto label_err;
	}
label_an:
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = gRelativePtr;
	memcpy(&outbuf[4],&buf[1],16);
	*outbytes =20;
	return;
label_err:
	shmt_bom_func_err(outbuf,outbytes);
	return;
}
/************************************************************************************************
函数：ShCardReadRecordCPU
功能：上海公交卡CPU卡读卡上交易记录及校验B28
************************************************************************************************/
void ShCardReadRecordCPU(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE buf[20],out[300],i;
	int ret;
	UWORD tint;
		UBYTE flag;
		tint = inbuf[2];
	ret=Transport_GetFiles15(outbuf);

	if(ret<0)
	{
		goto label_err;
	}

#ifdef _DEBUG_BOM_
debug_printf("\nShCardReadRecordCPU...");
#endif

	if(inbytes!=4)
	{
		gResponseStatus = CE_BADPARAM;
		goto label_err;
	}
	if((gStatus!=IDLE_STATUS)&&(gStatus!=TRANS_STATUS))
	{
		gResponseStatus = CE_STATUSERROR;
		goto label_err;
	}
	gDebugStep = 0x01;
	if((gStatus==IDLE_STATUS) || (gStatus==TRANS_STATUS))
	{
		#ifdef DEBUG_PRINT
			printf("      1x1x1x1x1x!\n");
		#endif
		mcml_pwr_off();
		i = 0;
		while(i<8)
 		{
 			if(mcml_request(PICC_REQSTD,buf)==0)
 			{
 				break;
 			}
 			i++;
 		}
 		gDebugStep = 0x02;
 		if(i>=8)
 		{
 			gResponseStatus = CE_CARDREMOVED;
			shmt_bom_func_err(outbuf,outbytes);
			return;
 		}
		gThisCardSnr[6] = buf[0];
		gThisCardSnr[7] = buf[1];
		gDebugStep = 0x03;
		ret=mcml_anticoll(gThisCardSnr);
		if(ret!=0)
		{
			gResponseStatus = CE_CARDREMOVED;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
		gDebugStep = 0x04;
		ret=mcml_select(gThisCardSnr,&i);
		if(ret!=0)
		{
			gResponseStatus = CE_CARDREMOVED;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
		gDebugStep = 0x05;
		gThisCardSnr[5] = i;
		ret = mifpro_ats0(buf);
		if(ret<0)
		{
			gResponseStatus = CE_CARDREMOVED;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
		gDebugStep = 0x06;
		memcpy(buf,"\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01\x00",15);
		ret = mifpro_icmd0(15,buf,out);
		if(ret<0)
		{
			gResponseStatus=CE_READERROR;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
		gDebugStep = 0x07;
		if((out[ret-2]!=0x90)||(out[ret-1]!=0x00))
		{
			goto label_invalid;
		}
		for(tint=0;tint<ret-2;tint++)
		{
			if(out[tint]!=0x9f)
				continue;
			tint++;
			if(out[tint]!=0x0c)
				continue;
			tint++;
			//if(out[tint]!=0x1e)
			//	goto label_invalid;
			tint++;
			if((ret-tint-2)<0x1e)
				goto label_invalid;
			break;
		}
		//if(tint>=(ret-2))
		//	goto label_invalid;
		gApplicationTypeTag = gCardStatus = out[tint+8];
		memcpy(gAppSn,&out[tint+10],10);
		if(gCardStatus&0x80)
		{
			gCardStatus = M1LOCKED;
		}
		if(memcmp(gAppSn,"\x20\x00",2)!=0)
		{
			gCardStatus = M1LOCKED;
		}
		if((out[tint+9]<10)&&(out[tint+9]!=2))
		{
			gCardStatus = 0;
		}
	}
	gDebugStep = 0x08;
	buf_to_uword(&inbuf[2],tint);
	if(history_record(tint,&outbuf[3]))
	{
		#ifdef DEBUG_PRINT
			printf("      Err: read record!\n");
		#endif
		gDebugStep = 0x09;
		mcml_pwr_off();
		goto label_err;
	}
	//mcml_pwr_off();
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	if((tint <= 0x2000)&&(tint > 8)) {
		if(gCardStatus!=M1LOCKED)
		{
			//file 0007 0x10
			*outbytes = 20;
			return;
		}
		else
		{
			//file 0018 0x17
			*outbytes = 27;
			return;			
		}
	}
	if(tint >= 0x4000)  *outbytes = 20;
		else *outbytes =27;
	return;
label_err:
	shmt_bom_func_err(outbuf,outbytes);
	return;
label_invalid:
	gResponseStatus=CE_INVALIDCARD;
	shmt_bom_func_err(outbuf,outbytes);
	return;
}
/************************************************************************************************
函数：ShCardGetProof
功能：根据流水号取TAC
************************************************************************************************/
void ShCardGetProof(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
#ifdef _DEBUG_BOM_
debug_printf("\nShCardGetProof...");
#endif

	if(inbytes!=5)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=IDLE_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(get_trans_record(&inbuf[2],outbuf,0)!=0)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	*outbytes = 16;
	return;
}
/************************************************************************************************
函数：ShCardGetProofEx
功能：根据多个参数取TAC
************************************************************************************************/
void ShCardGetProofEx(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE i;
	int ret;
	UWORD cnt;
	UBYTE buf[20],out[300];

#ifdef _DEBUG_BOM_
debug_printf("\nShCardGetProofEx...");
#endif


	if(inbytes!=11)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if((gStatus != IDLE_STATUS)&&(gStatus != TRANS_STATUS))
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = IDLE_STATUS;
	if(gCPUflag == 0x55)					//必须寻卡，对M1无影响，对CPU可选择AID
	{
		mcml_pwr_off();
//		delay_ms(5);
		//--------------------------------寻卡及选择AID
		i = 0;
		while(i<8)
 		{
 			if(mcml_request(PICC_REQSTD,buf)==0)
 			{
 				break;
 			}
 			i++;
 		}
 		gDebugStep = 0x02;
 		if(i>=8)
 		{
 			goto readeeprom;
 		}
		gThisCardSnr[6] = buf[0];
		gThisCardSnr[7] = buf[1];
		gDebugStep = 0x03;
		ret=mcml_anticoll(gThisCardSnr);
		if(ret!=0)
		{
			goto readeeprom;
		}
		if(memcmp(gThisCardSnr,&inbuf[5],4) != 0)  		
		{
			gResponseStatus=CE_INVALIDCARD;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
		gDebugStep = 0x04;
		ret=mcml_select(gThisCardSnr,&i);
		if(ret!=0)
		{
			goto readeeprom;
		}
		gDebugStep = 0x05;
		gThisCardSnr[5] = i;
		ret = mifpro_ats0(buf);
		if(ret<0)
		{
			goto readeeprom;
		}
		gDebugStep = 0x06;
		memcpy(buf,"\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01\x00",15);
		ret = mifpro_icmd0(15,buf,out);
		if(ret<0)
		{
			gResponseStatus=CE_READERROR;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
		gDebugStep = 0x07;
		if((out[ret-2]!=0x90)||(out[ret-1]!=0x00))
		{
			gResponseStatus=CE_INVALIDCARD;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
		//----------------------------get_trans_prove
		gDebugStep = 0x08;
		memcpy(buf,"\x80\x5a\x00\x09\x02",5);
		buf_to_uword(&inbuf[9],cnt);
		cnt++;
		uword_to_buf(cnt,&buf[5]);
		buf[7] = 0x08;
		ret = mifpro_icmd0(8,buf,out);
		if(ret<0)
		{
			gResponseStatus=CE_READERROR;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
		if((out[ret-2]!=0x90)||(out[ret-1]!=0x00)||(ret!=10))
		{
			goto readeeprom;
		}
		outbuf[0] = oldcmd;
		outbuf[1] = oldtimes;
		outbuf[2] = CE_OK;
		memcpy(&outbuf[3],&out[4],4);
		
		//---------credit sam for purchase
		memcpy(buf,"\x80\x72\x00\x00\x04",5);
		memcpy(&buf[5],out,4);
		ret=sam_apdu(bgSHpsamIndex,buf,9,out,&i,0,0);
		//---------return
		*outbytes = 7;
		return;
	}	
readeeprom:
	if(get_trans_record(&inbuf[2],buf,1)!=0)
	{
		gResponseStatus = CE_NORECORD;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = buf[12];
	outbuf[4] = buf[13];
	outbuf[5] = buf[14];
	outbuf[6] = buf[15];
	*outbytes = 7;
	return;
}
/************************************************************************************************
函数：ShCardPay
功能：交通卡扣款
************************************************************************************************/
void ShCardPay(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE ret;
	UBYTE buf[20];

#ifdef _DEBUG_BOM_
debug_printf("\nShCardPay...");
#endif

	if(inbytes!=16)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus != TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}	
	memcpy(gTransTime,&inbuf[2],7);
	if(check_time_valid() != 0) 
	{		
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	*((char *)&gPosTransCnt+LONG_HIGH3) = 0;							  			
  *((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[9];
  *((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[10];
  *((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[11];
	*((char *)&gCardThisTransValue+LONG_HIGH3) = 0;
	*((char *)&gCardThisTransValue+LONG_HIGH2) = inbuf[14];
	*((char *)&gCardThisTransValue+LONG_HIGH1) = inbuf[13];
	*((char *)&gCardThisTransValue+LONG_HIGH0) = inbuf[12];
	gStatus=IDLE_STATUS; 
	if(gCPUflag==0x55)
	{
		ret = trans_decrement_cpu();
		mcml_pwr_off();
	}
	else
	{
		ret = trans_decrement();
	}
	if(ret)
	{
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = *((char *)&gCardCnt+INT_HIGH);
	outbuf[4] = *((char *)&gCardCnt+INT_LOW);
	outbuf[5] = *((char *)&gCardThisRemain+LONG_HIGH0);
	outbuf[6] = *((char *)&gCardThisRemain+LONG_HIGH1);
	outbuf[7] = *((char *)&gCardThisRemain+LONG_HIGH2); 	
	outbuf[8] = gTAC[0];
	outbuf[9] = gTAC[1];
	outbuf[10]= gTAC[2];
	outbuf[11]= gTAC[3];		
	*outbytes = 12;
	gStatus=IDLE_STATUS;

	buf[0]=*((char *)&gPosTransCnt+LONG_HIGH2);
	buf[1]=*((char *)&gPosTransCnt+LONG_HIGH1);
	buf[2]=*((char *)&gPosTransCnt+LONG_HIGH0);
	if(gCPUflag==0x55)
	{
		memcpy(&buf[3],&gAppSn[6],4);
	}
	else
	{
		memcpy(&buf[3],gThisCardSnr,4);
	}
	buf[7]=*((char *)&gCardCnt+INT_HIGH);
	buf[8]=*((char *)&gCardCnt+INT_LOW);
	buf[9]=*((char *)&gCardThisRemain+LONG_HIGH0);
	buf[10]=*((char *)&gCardThisRemain+LONG_HIGH1);
	buf[11]=*((char *)&gCardThisRemain+LONG_HIGH2);
	memcpy(&buf[12],gTAC,4);
	save_trans_record(buf);
 return;
}
/************************************************************************************************
函数：ShMetroActive
功能：上海地铁单程票激活                              added in 2016.05.16
************************************************************************************************/
void ShMetroActive(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE buf[20];
  UBYTE buf1[16]; 
  UWORD i; 
#ifdef _DEBUG_BOM_
debug_printf("\nShMetroActive...");
#endif

	if(inbytes!=19)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
//	if((inbuf[13]!=0x64)&&(inbuf[13]!=0x65)&&(inbuf[13]!=0x68)&&(inbuf[13]!=0x69))
//	{
//		gResponseStatus = CE_BADPARAM;
//		shmt_bom_func_err(outbuf,outbytes);
//		return;
//	}
	gStatus = WRITE_STATUS;
	gTransTime[0]=inbuf[5];
	gTransTime[1]=inbuf[4];
	gTransTime[2]=inbuf[3];
	gTransTime[3]=inbuf[2];
	*((char *)&gPosTransCnt+LONG_HIGH3) = inbuf[9];							  			
  *((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[8];
  *((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[7];
  *((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[6];
  hh_mid[0] = inbuf[11];				   //此处用制造商代码替代操作员号 
	hh_mid[1] = inbuf[10];
  hh_trans_id = inbuf[12];
  gCardType = inbuf[13];
  gCardThisTransValue = 0;
  *((char *)&gCardThisTransValue+LONG_HIGH1) = inbuf[15];
  *((char *)&gCardThisTransValue+LONG_HIGH0) = inbuf[14];
  
	//计算TAC
	memcpy(KeyBuf,gThisCardSnr,4);
	memcpy(&KeyBuf[4],gTransTime,4);
	KeyBuf[8] = hh_SaleBuf[8];			//交易前余额,高在前
	KeyBuf[9] = hh_SaleBuf[9];
	KeyBuf[10]= *((char *)&gCardThisTransValue+LONG_HIGH1);		//交易金额,高在前
	KeyBuf[11]= *((char *)&gCardThisTransValue+LONG_HIGH0);
	KeyBuf[12]= hh_trans_Line;
	KeyBuf[13]= hh_trans_Station;
	memcpy(&KeyBuf[14],gPosNo,4);
	KeyBuf[16]= 0x13;
	KeyBuf[18]= *((char *)&gPosTransCnt+LONG_HIGH3);
	KeyBuf[19]= *((char *)&gPosTransCnt+LONG_HIGH2);
	KeyBuf[20]= *((char *)&gPosTransCnt+LONG_HIGH1);
	KeyBuf[21]= *((char *)&gPosTransCnt+LONG_HIGH0);
	KeyBuf[22]= *((char *)&gCardCnt+INT_HIGH);
	KeyBuf[23]= *((char *)&gCardCnt+INT_LOW);
	memcpy(&KeyBuf[24],bpgSmpsamNo,4);
	KeyBuf[28]= 0x80;
	KeyBuf[29]= 0x00;
	KeyBuf[30]= 0x00;
	KeyBuf[31]= 0x00;
	SHMetroTACStep1(KeyBuf);
	
	if(SMGetTacStep2()!=0)												//取TAC
	{
		gResponseStatus = CE_PSAMERROR;
		return;
	}
	memcpy(gTAC,bgTaskKeyOutbuf,4);	
	buf[0]=hh_trans_id;
	i=gCardCnt+1;
	buf[1]=*((char *)&i+INT_HIGH);
	buf[2]=*((char *)&i+INT_LOW);					
	lrc_gen2(buf,3);
			
  outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = *((char *)&gCardCnt+INT_LOW);
	outbuf[4] = *((char *)&gCardCnt+INT_HIGH); 	
	outbuf[5] = *((char *)&gCardThisTransValue+LONG_HIGH0);
	outbuf[6] = *((char *)&gCardThisTransValue+LONG_HIGH1);	
	memcpy(&outbuf[7],gTAC,4);										
	*outbytes = 11;
  gStatus=IDLE_STATUS;
	mcml_halt();
	buf[0]=*((char *)&gPosTransCnt+LONG_HIGH3);
	buf[1]=*((char *)&gPosTransCnt+LONG_HIGH2);
	buf[2]=*((char *)&gPosTransCnt+LONG_HIGH1);
	buf[3]=*((char *)&gPosTransCnt+LONG_HIGH0);
	buf[4]=*((char *)&gCardCnt+INT_LOW);
	buf[5]=*((char *)&gCardCnt+INT_HIGH);
	memcpy(&buf[6],gTAC,4);
	memcpy(&buf[10],gThisCardSnr,4); 	
	buf[14]=0;
	buf[15]=0;
	save_sigal_record(buf);
	return;	
}
/************************************************************************************************
函数：ShCardUpdate
功能：交通卡更新
************************************************************************************************/
void ShCardUpdate(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE ret;
	UBYTE buf[20];

#ifdef _DEBUG_BOM_
debug_printf("\nShCardUpdate...");
#endif

	if(inbytes!=32)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus != TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}	
	if((inbuf[2]!=0x01)&&(inbuf[2]!=0x02))
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	memcpy(gTransTime,&inbuf[3],7);
	if(check_time_valid() != 0) 
	{		
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	*((char *)&gPosTransCnt+LONG_HIGH3) = 0;							  			
	*((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[10];
	*((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[11];
	*((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[12];
	*((char *)&gCardThisTransValue+LONG_HIGH3) = 0;
	*((char *)&gCardThisTransValue+LONG_HIGH2) = inbuf[15];
	*((char *)&gCardThisTransValue+LONG_HIGH1) = inbuf[14];
	*((char *)&gCardThisTransValue+LONG_HIGH0) = inbuf[13];
	gStatus=IDLE_STATUS; 
	if(gCPUflag==0x55)
	{
		ret = trans_update_cpu(inbuf[2],&inbuf[16]);
		mcml_pwr_off();
	}
	else
	{
		ret = trans_update(inbuf[2],&inbuf[16]);
		mcml_pwr_off();
	}
	if(ret)
	{
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = *((char *)&gCardCnt+INT_HIGH);
	outbuf[4] = *((char *)&gCardCnt+INT_LOW);
	outbuf[5] = *((char *)&gCardThisRemain+LONG_HIGH0);
	outbuf[6] = *((char *)&gCardThisRemain+LONG_HIGH1);
	outbuf[7] = *((char *)&gCardThisRemain+LONG_HIGH2); 	
	outbuf[8] = gTAC[0];
	outbuf[9] = gTAC[1];
	outbuf[10]= gTAC[2];
	outbuf[11]= gTAC[3];		
	*outbytes = 12;

	buf[0]=*((char *)&gPosTransCnt+LONG_HIGH2);
	buf[1]=*((char *)&gPosTransCnt+LONG_HIGH1);
	buf[2]=*((char *)&gPosTransCnt+LONG_HIGH0);
	if(gCPUflag==0x55)
	{
		memcpy(&buf[3],&gAppSn[6],4);
	}
	else
	{
		memcpy(&buf[3],gThisCardSnr,4);
	}
	buf[7]=*((char *)&gCardCnt+INT_HIGH);
	buf[8]=*((char *)&gCardCnt+INT_LOW);
	buf[9]=*((char *)&gCardThisRemain+LONG_HIGH0);
	buf[10]=*((char *)&gCardThisRemain+LONG_HIGH1);
	buf[11]=*((char *)&gCardThisRemain+LONG_HIGH2);
	memcpy(&buf[12],gTAC,4);
	save_trans_record(buf);
	return;
}
/************************************************************************************************
函数：ShMetroAddValue
功能：上海地铁单程票加值
************************************************************************************************/
void ShMetroAddValue(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE buf[20];
	
#ifdef _DEBUG_BOM_
debug_printf("\nShMetroAddValue...");
#endif

	if(inbytes!=18)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	
	gCardThisTransValue=0;
	gCardThisRemain = 0;
	gTransTime[0]=inbuf[5];
	gTransTime[1]=inbuf[4];
	gTransTime[2]=inbuf[3];
	gTransTime[3]=inbuf[2];
	*((char *)&gPosTransCnt+LONG_HIGH3) = inbuf[9];							  			
	*((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[8];
	*((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[7];
	*((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[6];
	hh_mid[0] = inbuf[11];				   
	hh_mid[1] = inbuf[10];
	*((char *)&gCardThisTransValue+LONG_HIGH0) =  inbuf[13];
	*((char *)&gCardThisTransValue+LONG_HIGH1) =  inbuf[14];
	*((char *)&gCardThisRemain+LONG_HIGH0) =  inbuf[15];
	*((char *)&gCardThisRemain+LONG_HIGH1) =  inbuf[16];	
	hh_trans_id=inbuf[17];
	
	if(hh_trans_addvalue()!=0)
	{
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = *((char *)&gCardCnt+INT_LOW);
	outbuf[4] = *((char *)&gCardCnt+INT_HIGH);
	outbuf[5] = *((char *)&gCardThisRemain+LONG_HIGH0);
	outbuf[6] = *((char *)&gCardThisRemain+LONG_HIGH1);
	memcpy(&outbuf[7],gTAC,4);
	*outbytes =11;
	mcml_pwr_off();//
	
	buf[0]=*((char *)&gPosTransCnt+LONG_HIGH3);
	buf[1]=*((char *)&gPosTransCnt+LONG_HIGH2);
	buf[2]=*((char *)&gPosTransCnt+LONG_HIGH1);
	buf[3]=*((char *)&gPosTransCnt+LONG_HIGH0);
	buf[4]=*((char *)&gCardCnt+INT_LOW);
	buf[5]=*((char *)&gCardCnt+INT_HIGH);
	memcpy(&buf[6],gTAC,4);
	memcpy(&buf[10],gThisCardSnr,4); 	
	buf[14]=0;
	buf[15]=0;
	save_sigal_record(buf);
	return;
}

/************************************************************************************************
函数：ShMetroExpireData
功能：上海地铁单程票续期
************************************************************************************************/
void ShMetroExpireData(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE buf[20];
	
#ifdef _DEBUG_BOM_
debug_printf("\nShMetroExpireData...");
#endif

	if(inbytes!=19)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	
	gCardThisTransValue=0;
	gCardThisRemain = 0;
	gTransTime[0]=inbuf[5];
	gTransTime[1]=inbuf[4];
	gTransTime[2]=inbuf[3];
	gTransTime[3]=inbuf[2];
	*((char *)&gPosTransCnt+LONG_HIGH3) = inbuf[9];							  			
	*((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[8];
	*((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[7];
	*((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[6];
	hh_mid[0] = inbuf[11];				   
	hh_mid[1] = inbuf[10];
	hh_trans_id=inbuf[18];
	
	if(hh_trans_expiredata(&inbuf[2])!=0)
	{
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = *((char *)&gCardCnt+INT_LOW);
	outbuf[4] = *((char *)&gCardCnt+INT_HIGH);
	memcpy(&outbuf[5],gTAC,4);
	*outbytes =9;
	mcml_pwr_off();//
	
	buf[0]=*((char *)&gPosTransCnt+LONG_HIGH3);
	buf[1]=*((char *)&gPosTransCnt+LONG_HIGH2);
	buf[2]=*((char *)&gPosTransCnt+LONG_HIGH1);
	buf[3]=*((char *)&gPosTransCnt+LONG_HIGH0);
	buf[4]=*((char *)&gCardCnt+INT_LOW);
	buf[5]=*((char *)&gCardCnt+INT_HIGH);
	memcpy(&buf[6],gTAC,4);
	memcpy(&buf[10],gThisCardSnr,4); 	
	buf[14]=0;
	buf[15]=0;
	save_sigal_record(buf);
	return;
}
/************************************************************************************************
函数：ShMetroReturnCard
功能：上海地铁单程票退票
************************************************************************************************/
void ShMetroReturnCard(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE buf[20];
	
#ifdef _DEBUG_BOM_
debug_printf("\nShMetroExpireData...");
#endif

	if(inbytes!=15)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	
	gCardThisTransValue=0;
	gCardThisRemain = 0;
	gTransTime[0]=inbuf[5];
	gTransTime[1]=inbuf[4];
	gTransTime[2]=inbuf[3];
	gTransTime[3]=inbuf[2];
	*((char *)&gPosTransCnt+LONG_HIGH3) = inbuf[9];							  			
	*((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[8];
	*((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[7];
	*((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[6];
	hh_mid[0] = inbuf[11];				   
	hh_mid[1] = inbuf[10];
	*((char *)&gCardThisTransValue+LONG_HIGH0) =  inbuf[12];
	*((char *)&gCardThisTransValue+LONG_HIGH1) =  inbuf[13];
	hh_trans_id=inbuf[14];
	
	if(hh_trans_returndata()!=0)
	{
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = *((char *)&gCardCnt+INT_LOW);
	outbuf[4] = *((char *)&gCardCnt+INT_HIGH);
	memcpy(&outbuf[5],gTAC,4);
	*outbytes =9;
	mcml_pwr_off();//
	
	buf[0]=*((char *)&gPosTransCnt+LONG_HIGH3);
	buf[1]=*((char *)&gPosTransCnt+LONG_HIGH2);
	buf[2]=*((char *)&gPosTransCnt+LONG_HIGH1);
	buf[3]=*((char *)&gPosTransCnt+LONG_HIGH0);
	buf[4]=*((char *)&gCardCnt+INT_LOW);
	buf[5]=*((char *)&gCardCnt+INT_HIGH);
	memcpy(&buf[6],gTAC,4);
	memcpy(&buf[10],gThisCardSnr,4); 	
	buf[14]=0;
	buf[15]=0;
	save_sigal_record(buf);
	return;
}

/************************************************************************************************
函数：ShMetroSale
功能：上海地铁单程票售票
************************************************************************************************/
void ShMetroSale(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE buf[20];

#ifdef _DEBUG_BOM_
debug_printf("\nShMetroSale...");
#endif

	if(inbytes!=19)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	
//	if((inbuf[13]!=0x64)&&(inbuf[13]!=0x65)&&(inbuf[13]!=0x68)&&(inbuf[13]!=0x69))
//	{
//		gResponseStatus = CE_BADPARAM;
//		shmt_bom_func_err(outbuf,outbytes);
//		return;
//	}
	gStatus = WRITE_STATUS;
	gTransTime[0]=inbuf[5];
	gTransTime[1]=inbuf[4];
	gTransTime[2]=inbuf[3];
	gTransTime[3]=inbuf[2];
	*((char *)&gPosTransCnt+LONG_HIGH3) = inbuf[9];							  			
	*((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[8];
	*((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[7];
	*((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[6];
	hh_mid[0] = inbuf[11];				   //此处用制造商代码替代操作员号 
	hh_mid[1] = inbuf[10];
	hh_trans_id = inbuf[12];
	gCardType = inbuf[13];
	gCardThisTransValue = 0;
	*((char *)&gCardThisTransValue+LONG_HIGH1) = inbuf[15];
	*((char *)&gCardThisTransValue+LONG_HIGH0) = inbuf[14];
	if(hh_trans_sale()!=0)
	{
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = *((char *)&gCardCnt+INT_LOW);
	outbuf[4] = *((char *)&gCardCnt+INT_HIGH); 	
	outbuf[5] = *((char *)&gCardThisTransValue+LONG_HIGH0);
	outbuf[6] = *((char *)&gCardThisTransValue+LONG_HIGH1);	
	memcpy(&outbuf[7],gTAC,4);										
	*outbytes = 11;
	gStatus=IDLE_STATUS;
	mcml_halt();

	buf[0]=*((char *)&gPosTransCnt+LONG_HIGH3);
	buf[1]=*((char *)&gPosTransCnt+LONG_HIGH2);
	buf[2]=*((char *)&gPosTransCnt+LONG_HIGH1);
	buf[3]=*((char *)&gPosTransCnt+LONG_HIGH0);
	buf[4]=*((char *)&gCardCnt+INT_LOW);
	buf[5]=*((char *)&gCardCnt+INT_HIGH);
	memcpy(&buf[6],gTAC,4);
	memcpy(&buf[10],gThisCardSnr,4); 	
	buf[14]=0;
	buf[15]=0;
	save_sigal_record(buf);
	return;	
}
/************************************************************************************************
函数：ShMetroEntry
功能：上海地铁单程票进站交易
************************************************************************************************/
void ShMetroEntry(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE buf[20];
	#ifdef _debug_test_
	UDWORD	tint0=0,tint1=0;
	#endif

#ifdef _DEBUG_BOM_
debug_printf("\nShMetroEntry...");
#endif
	
	if(gMode != ENTRY_MODE)
	{
		gResponseStatus = CE_BADCOMMAND;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(inbytes!=11)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = WRITE_STATUS;
	gTransTime[0]=inbuf[5];
	gTransTime[1]=inbuf[4];
	gTransTime[2]=inbuf[3];
	gTransTime[3]=inbuf[2];
	*((char *)&gPosTransCnt+LONG_HIGH3) = inbuf[9];							  			
  *((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[8];
  *((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[7];
  *((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[6];
  hh_trans_id=inbuf[10];	
	gCardThisTransValue=0;
	
	#ifdef _debug_test_
	tint0 = GetTickcount();
	#endif
	if(hh_trans_entry()!=0)
	{	
		mcml_pwr_off();
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	#ifdef _debug_test_
	tint1 = GetTickcount();
	debug_printf("\n sh_metro_entry(%d) \n",tint1-tint0);
	#endif
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = *((char *)&gCardCnt+INT_LOW);
	outbuf[4] = *((char *)&gCardCnt+INT_HIGH); 		
	memcpy(&outbuf[5],gTAC,4);										
	*outbytes = 9;
	gStatus=IDLE_STATUS;
	mcml_pwr_off();//
	buf[0]=*((char *)&gPosTransCnt+LONG_HIGH3);
	buf[1]=*((char *)&gPosTransCnt+LONG_HIGH2);
	buf[2]=*((char *)&gPosTransCnt+LONG_HIGH1);
	buf[3]=*((char *)&gPosTransCnt+LONG_HIGH0);
	buf[4]=*((char *)&gCardCnt+INT_LOW);
	buf[5]=*((char *)&gCardCnt+INT_HIGH);
	memcpy(&buf[6],gTAC,4);
	memcpy(&buf[10],gThisCardSnr,4); 	
	buf[14]=0;
	buf[15]=0;
	save_sigal_record(buf);
	
	#ifdef _RECOVERY_AFTER_RESET_
	if(bgResetWrTimeFlag){
		 rtc_wr_time(&gTransTime[1]);
		 bgResetWrTimeFlag=0;
		 }
	#endif
	return;	
}
/************************************************************************************************
函数：ShMetroSPEntry
功能：上海地铁专用票进站交易
************************************************************************************************/
void ShMetroSPEntry(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE buf[20];

#ifdef _DEBUG_BOM_
debug_printf("\nShMetroSPEntry...");
#endif

	if(gMode != ENTRY_MODE)
	{
		gResponseStatus = CE_BADCOMMAND;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(inbytes!=15)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	//print_debug(hh_Entry, 8);
	gStatus = WRITE_STATUS;
	gCardThisTransValue=0;
	gCardThisRemain = 0;
	gTransTime[0]=inbuf[5];
	gTransTime[1]=inbuf[4];
	gTransTime[2]=inbuf[3];
	gTransTime[3]=inbuf[2];
	*((char *)&gPosTransCnt+LONG_HIGH3) = inbuf[9];							  			
  *((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[8];
  *((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[7];
  *((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[6];
	hh_trans_id=inbuf[10];
	*((char *)&gCardThisRemain+LONG_HIGH0) =  inbuf[11];
	*((char *)&gCardThisRemain+LONG_HIGH1) =  inbuf[12];	
//	print_debug(hh_Entry, 8);
	if(hh_trans_entry2(inbuf[13])!=0)
	{	
		gStatus = IDLE_STATUS;	
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
//	print_debug(hh_Entry, 8);
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = *((char *)&gCardCnt+INT_LOW);
	outbuf[4] = *((char *)&gCardCnt+INT_HIGH);
	memcpy(&outbuf[5],gTAC,4);
	//20070813协议
//	outbuf[9] = gPosNo[3];
//	outbuf[10]= gPosNo[1];
//	outbuf[11]= gPosNo[0];			
	outbuf[9] = hh_Entry[6];	
	outbuf[10] = hh_Entry[5];	
	outbuf[11] = hh_Entry[4];		
//	print_debug(outbuf, 12);					
	*outbytes = 12;
	gStatus=IDLE_STATUS;
	mcml_pwr_off();//
	buf[0]=*((char *)&gPosTransCnt+LONG_HIGH3);
	buf[1]=*((char *)&gPosTransCnt+LONG_HIGH2);
	buf[2]=*((char *)&gPosTransCnt+LONG_HIGH1);
	buf[3]=*((char *)&gPosTransCnt+LONG_HIGH0);
	buf[4]=*((char *)&gCardCnt+INT_LOW);
	buf[5]=*((char *)&gCardCnt+INT_HIGH);
	memcpy(&buf[6],gTAC,4);
	memcpy(&buf[10],gThisCardSnr,4); 	
	buf[14]=0;
	buf[15]=0;
	save_sigal_record(buf);
	return;	
}
/************************************************************************************************
函数：ShMetroExit
功能：上海地铁单程票出站交易
************************************************************************************************/
void ShMetroExit(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE buf[20];
	#ifdef _debug_test_
	UDWORD	tint0=0,tint1=0;
	#endif
	
#ifdef _DEBUG_BOM_
debug_printf("\nShMetroExit...");
#endif

	if(gMode != EXIT_MODE)
	{
		gResponseStatus = CE_BADCOMMAND;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(inbytes!=16)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = WRITE_STATUS;
	gCardThisTransValue=0;
	gCardThisRemain = 0;
	gTransTime[0]=inbuf[5];
	gTransTime[1]=inbuf[4];
	gTransTime[2]=inbuf[3];
	gTransTime[3]=inbuf[2];
	*((char *)&gPosTransCnt+LONG_HIGH3) = inbuf[9];							  			
  *((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[8];
  *((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[7];
  *((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[6];
	*((char *)&gCardThisTransValue+LONG_HIGH0) =  inbuf[10];
	*((char *)&gCardThisTransValue+LONG_HIGH1) =  inbuf[11];
	*((char *)&gCardThisRemain+LONG_HIGH0) =  inbuf[12];
	*((char *)&gCardThisRemain+LONG_HIGH1) =  inbuf[13];	
	hh_trans_id=inbuf[14];
	
	#ifdef _debug_test_
	tint0 = GetTickcount();
	#endif
	if(hh_trans_exit(inbuf[15])!=0)
	{	
		mcml_pwr_off();
		gStatus = IDLE_STATUS;	
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	#ifdef _debug_test_
	tint1 = GetTickcount();
	debug_printf("\n sh_metro_exit(%d) \n",tint1-tint0);
	#endif
	
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = *((char *)&gCardCnt+INT_LOW);
	outbuf[4] = *((char *)&gCardCnt+INT_HIGH);
	outbuf[5] = *((char *)&gCardThisRemain+LONG_HIGH0);
	outbuf[6] = *((char *)&gCardThisRemain+LONG_HIGH1); 	
	memcpy(&outbuf[7],gTAC,4);										
	*outbytes = 11;
	gStatus=IDLE_STATUS;
	mcml_pwr_off();//
	buf[0]=*((char *)&gPosTransCnt+LONG_HIGH3);
	buf[1]=*((char *)&gPosTransCnt+LONG_HIGH2);
	buf[2]=*((char *)&gPosTransCnt+LONG_HIGH1);
	buf[3]=*((char *)&gPosTransCnt+LONG_HIGH0);
	buf[4]=*((char *)&gCardCnt+INT_LOW);
	buf[5]=*((char *)&gCardCnt+INT_HIGH);
	memcpy(&buf[6],gTAC,4);
	memcpy(&buf[10],gThisCardSnr,4); 	
	buf[14]=0;
	buf[15]=0;
	save_sigal_record(buf);
	
	#ifdef _RECOVERY_AFTER_RESET_
	if(bgResetWrTimeFlag){
		 rtc_wr_time(&gTransTime[1]);
		 bgResetWrTimeFlag=0;
		 }
	#endif
	return;
}
/************************************************************************************************
函数：ShMetroLock
功能：上海地铁单程票锁卡交易
************************************************************************************************/
void ShMetroLock(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE buf[20];

#ifdef _DEBUG_BOM_
debug_printf("\nShMetroLock...");
#endif

	if(inbytes!=16)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = WRITE_STATUS;
	gCardThisTransValue=0;
	gCardThisRemain = 0;
	gTransTime[0]=inbuf[5];
	gTransTime[1]=inbuf[4];
	gTransTime[2]=inbuf[3];
	gTransTime[3]=inbuf[2];
	*((char *)&gPosTransCnt+LONG_HIGH3) = inbuf[9];							  			
	*((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[8];
	*((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[7];
	*((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[6];
	*((char *)&gCardThisTransValue+LONG_HIGH0) =  inbuf[10];
	*((char *)&gCardThisTransValue+LONG_HIGH1) =  inbuf[11];
	*((char *)&gCardThisRemain+LONG_HIGH0) =  inbuf[12];
	*((char *)&gCardThisRemain+LONG_HIGH1) =  inbuf[13];	
	hh_trans_id=inbuf[14];
	
	//计算TAC
	memcpy(KeyBuf,gThisCardSnr,4);
	memcpy(&KeyBuf[4],gTransTime,4);
	KeyBuf[8] = hh_SaleBuf[8];								//交易前余额,高在前
	KeyBuf[9] = hh_SaleBuf[9];
	KeyBuf[10]= *((char *)&gCardThisTransValue+LONG_HIGH1);					//交易金额,高在前
	KeyBuf[11]= *((char *)&gCardThisTransValue+LONG_HIGH0);
	KeyBuf[12]= hh_trans_Line;
	KeyBuf[13]= hh_trans_Station;
	memcpy(&KeyBuf[14],gPosNo,4);
	if(gMode==EXIT_MODE)
	{
		KeyBuf[16]=0x15;
	}
	if(gMode==ENTRY_MODE)
	{
		KeyBuf[16]=0x1a;
	}
	KeyBuf[18]= *((char *)&gPosTransCnt+LONG_HIGH3);
	KeyBuf[19]= *((char *)&gPosTransCnt+LONG_HIGH2);
	KeyBuf[20]= *((char *)&gPosTransCnt+LONG_HIGH1);
	KeyBuf[21]= *((char *)&gPosTransCnt+LONG_HIGH0);
	KeyBuf[22]= *((char *)&gCardCnt+INT_HIGH);
	KeyBuf[23]= *((char *)&gCardCnt+INT_LOW);
	memcpy(&KeyBuf[24],bpgSmpsamNo,4);
	KeyBuf[28]= 0x80;
	KeyBuf[29]= 0x00;
	KeyBuf[30]= 0x00;
	KeyBuf[31]= 0x00;
	SHMetroTACStep1(KeyBuf);
	
	//开始交易
	buf[0]=hh_B9[0];
	buf[1]=hh_SaleBuf[8];
	buf[2]=hh_SaleBuf[9];
	lrc_gen2(buf,3);
	if(tocken_write(15,buf)!=0)
	{
		gResponseStatus=CE_WRITEERROR;
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	buf[0]=0;
	buf[1]=0;
	buf[2]=hh_SaleBuf[10];
	buf[3]=hh_SaleBuf[11];
	if(tocken_write(7,buf)!=0)
	{
		gResponseStatus=CE_WRITEERROR;
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(SMGetTacStep2()!=0)												//取TAC
	{
		gResponseStatus = CE_PSAMERROR;
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	memcpy(gTAC,bgTaskKeyOutbuf,4);	
	
	buf[0]=hh_trans_id;
	gCardCnt++;
	buf[1]=*((char *)&gCardCnt+INT_HIGH);
	buf[2]=*((char *)&gCardCnt+INT_LOW);
	gCardCnt--;
	lrc_gen2(buf,3);
	if(tocken_write(9,buf)!=0)
	{
		gResponseStatus=CE_WRITEERROR;
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = *((char *)&gCardCnt+INT_LOW);
	outbuf[4] = *((char *)&gCardCnt+INT_HIGH);
	outbuf[5] = 0;
	outbuf[6] = 0;
	memcpy(&outbuf[7],gTAC,4);
	*outbytes =11;
	
	gStatus = IDLE_STATUS;
	mcml_halt();
	buf[0]=*((char *)&gPosTransCnt+LONG_HIGH3);
	buf[1]=*((char *)&gPosTransCnt+LONG_HIGH2);
	buf[2]=*((char *)&gPosTransCnt+LONG_HIGH1);
	buf[3]=*((char *)&gPosTransCnt+LONG_HIGH0);
	buf[4]=*((char *)&gCardCnt+INT_LOW);
	buf[5]=*((char *)&gCardCnt+INT_HIGH);
	memcpy(&buf[6],gTAC,4);
	memcpy(&buf[10],gThisCardSnr,4); 	
	buf[14]=0;
	buf[15]=0;
	save_sigal_record(buf);
	
	return;	
}
/************************************************************************************************
函数：ShMetroUpdate
功能：上海地铁单程票更新交易
************************************************************************************************/
void ShMetroUpdate(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE buf[20];

#ifdef _DEBUG_BOM_
debug_printf("\nShMetroUpdate...");
#endif

	if(inbytes!=23)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(inbuf[2] > 4)  //2022.05.06
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = IDLE_STATUS;
	gCardThisTransValue=0;
	gCardThisRemain = 0;
	gTransTime[0]=inbuf[10];
	gTransTime[1]=inbuf[9];
	gTransTime[2]=inbuf[8];
	gTransTime[3]=inbuf[7];
	*((char *)&gPosTransCnt+LONG_HIGH3) = inbuf[14];							  			
	*((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[13];
	*((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[12];
	*((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[11];
	*((char *)&gCardThisTransValue+LONG_HIGH0) =  inbuf[15];
	*((char *)&gCardThisTransValue+LONG_HIGH1) =  inbuf[16];
	*((char *)&gCardThisRemain+LONG_HIGH0) =  inbuf[17];
	*((char *)&gCardThisRemain+LONG_HIGH1) =  inbuf[18];	
	hh_trans_id=inbuf[3];
	if(hh_trans_update(&inbuf[2])!=0)
	{
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = *((char *)&gCardCnt+INT_LOW);
	outbuf[4] = *((char *)&gCardCnt+INT_HIGH);
	outbuf[5] = *((char *)&gCardThisRemain+LONG_HIGH0);
	outbuf[6] = *((char *)&gCardThisRemain+LONG_HIGH1);
	memcpy(&outbuf[7],gTAC,4);
	*outbytes =11;
	mcml_pwr_off();//
	
	buf[0]=*((char *)&gPosTransCnt+LONG_HIGH3);
	buf[1]=*((char *)&gPosTransCnt+LONG_HIGH2);
	buf[2]=*((char *)&gPosTransCnt+LONG_HIGH1);
	buf[3]=*((char *)&gPosTransCnt+LONG_HIGH0);
	buf[4]=*((char *)&gCardCnt+INT_LOW);
	buf[5]=*((char *)&gCardCnt+INT_HIGH);
	memcpy(&buf[6],gTAC,4);
	memcpy(&buf[10],gThisCardSnr,4); 	
	buf[14]=0;
	buf[15]=0;
	save_sigal_record(buf);
	return;
}
/************************************************************************************************
函数：ShMetroGetProof
功能：上海地铁单程票根据流水号取TAC
************************************************************************************************/
void ShMetroGetProof(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE buf[20];

#ifdef _DEBUG_BOM_
debug_printf("\nShMetroGetProof...");
#endif

	if(inbytes!=9)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=IDLE_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(get_sigal_record(&inbuf[2],buf)!=0)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	memcpy(&outbuf[3],&buf[4],6);
	*outbytes =9;
	return; 
}
/************************************************************************************************
函数：ShMobileEntry
功能：上海地铁手机钱包支付进站交易
************************************************************************************************/
void ShMobileEntry(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE ret;
  UBYTE buf[5];

#ifdef _DEBUG_BOM_
debug_printf("\nShMobileEntry...");
#endif
	
	gRespStep = 0;
	if(gMode != ENTRY_MODE)
	{
		gResponseStatus = CE_BADCOMMAND;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(inbytes!=16)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = WRITE_STATUS;
	memcpy(gTransTime,&inbuf[2],7);
	if(check_time_valid() != 0) 
	{
		gStatus=IDLE_STATUS; 
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	*((char *)&gPosTransCnt+LONG_HIGH3) = 0;
 	*((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[9];
  *((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[10];
  *((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[11];
 	if(inbuf[12]&0x18)
	{
		gStationTimeOverRideFlag=inbuf[12];
	}
	else
	{
		gStationTimeOverRideFlag=0;
	}
	gTransValueMax[0] = 0;
	gTransValueMax[1] = inbuf[15];
	gTransValueMax[2] = inbuf[14];
	gTransValueMax[3] = inbuf[13];
	
	g24GFlag = 0x55;
	
	ret = trans_entry_24G();
	//nandf_write_buf(28,12,&TimeBuf[28]);	
	//flash_write_buf(28,12,&TimeBuf[28]);	
	if(ret != 0)
	{
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

	//2010-3-30 16:02
	memcpy(buf,"\xa2\x32\x0\x0",4);
  prot_zte_send_block(buf,4); 
	//zte_set_wait(100);
	prot_zte_rece_process(30);
	//ZTE_HALT();
  
	gStatus=IDLE_STATUS;
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = DEFAULT_KEY_INDEX;
	outbuf[4] = gKeyBacon;
	uword_to_buf(gCardCnt,&outbuf[5]);
	udword_to_buf(gPosTransCnt,&outbuf[7]);
	memcpy(&outbuf[11],gTAC,4);
	outbuf[15] = 0;
	outbuf[16] = 0;
	*outbytes =17;
	gCPUflag = 0x55;
	gRespStep = 1;
	return; 
}
/************************************************************************************************
函数：ShMobileExit
功能：上海地铁手机钱包支付出站交易
************************************************************************************************/
void ShMobileExit(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
UBYTE ret;
UBYTE transtype;
UBYTE buf[5];

#ifdef _DEBUG_BOM_
debug_printf("\nShMobileExit...");
#endif

  gRespStep = 0;

  if(gMode != EXIT_MODE)
	{
		gResponseStatus = CE_BADCOMMAND;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(inbytes!=18)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = WRITE_STATUS;
	memcpy(gTransTime,&inbuf[2],7);
	if(check_time_valid() != 0) 
	{
		gStatus=IDLE_STATUS; 
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	*((char *)&gPosTransCnt+LONG_HIGH3) = 0;							  			
  	*((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[9];
  	*((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[10];
  	*((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[11];
	*((char *)&gCardThisTransValue+LONG_HIGH3) = 0;
	*((char *)&gCardThisTransValue+LONG_HIGH2) = inbuf[14];
	*((char *)&gCardThisTransValue+LONG_HIGH1) = inbuf[13];
	*((char *)&gCardThisTransValue+LONG_HIGH0) = inbuf[12];
	
	transtype= inbuf[15];
	if(ReadMode==0)
	{
		if(transtype>2)
		{
			gStatus=IDLE_STATUS;
			gResponseStatus=CE_BADPARAM;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
	}
	else	
	{
		if((transtype>12)||(transtype==2)||(transtype==3)||(transtype==4))
		{
			gStatus=IDLE_STATUS;
			gResponseStatus=CE_BADPARAM;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
	}
	
	g24GFlag = 0x55;
	
	ret = trans_exit_24G(transtype);
	if(ret!=0)
	{
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

	//2010-3-30 16:02
	memcpy(buf,"\xa2\x32\x0\x0",4);
  prot_zte_send_block(buf,4); 
	//zte_set_wait(100);
	prot_zte_rece_process(30);
	//ZTE_HALT();

	gStatus=IDLE_STATUS;
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = DEFAULT_KEY_INDEX;
	outbuf[4] = gKeyBacon;
	uword_to_buf(gCardCnt,&outbuf[5]);
	udword_to_buf(gPosTransCnt,&outbuf[7]);
	outbuf[11] = *((char *)&gCardThisRemain+LONG_HIGH0);
	outbuf[12] = *((char *)&gCardThisRemain+LONG_HIGH1);
	outbuf[13] = *((char *)&gCardThisRemain+LONG_HIGH2); 	
	outbuf[14] = gTAC[0];
	outbuf[15] = gTAC[1];
	outbuf[16] = gTAC[2];
	outbuf[17] = gTAC[3];	
	*outbytes = 18;
	gCPUflag = 0x55;
	gRespStep = 1;
	return; 
}
/************************************************************************************************
函数：ShMobileLock
功能：上海地铁手机钱包支付改写异地异常交易标志
************************************************************************************************/
void ShMobileLock(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE ret;
	
	gRespStep = 0;
//	if(gMode != ENTRY_MODE)
//	{
//		gResponseStatus = CE_BADCOMMAND;
//		shmt_bom_func_err(outbuf,outbytes);
//		return;
//	}
	if(CurrentLocation==LocationID)
	{
		gResponseStatus = CE_BADCOMMAND;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(inbytes!=12)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = WRITE_STATUS;
	memcpy(gTransTime,&inbuf[2],7);
	if(check_time_valid() != 0) 
	{
		gStatus=IDLE_STATUS; 
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	
	g24GFlag = 0x55;
	
	ret = trans_lock_24G();
	if(ret != 0)
	{
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus=IDLE_STATUS;
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = DEFAULT_KEY_INDEX;
	outbuf[4] = gKeyBacon;
	uword_to_buf(gCardCnt,&outbuf[5]);
	udword_to_buf(gPosTransCnt,&outbuf[7]);
	memcpy(&outbuf[11],gTAC,4);
	*outbytes =15;
	gCPUflag = 0x55;
	gRespStep = 1;
	return; 
}
/************************************************************************************************
函数：ShMobileUpdate
功能：上海地铁手机钱包支付更新交易
************************************************************************************************/
void ShMobileUpdate(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE ret;
	UBYTE buf[20];

#ifdef _DEBUG_BOM_
debug_printf("\nShMobileUpdate...");
#endif
	
	gRespStep = 0;
	if(gMode != BOM_MODE)
	{
		gResponseStatus = CE_BADCOMMAND;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(inbytes!=31)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = WRITE_STATUS;
	memcpy(gTransTime,&inbuf[2],7);
	if(check_time_valid() != 0) 
	{
		gStatus=IDLE_STATUS; 
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	*((char *)&gPosTransCnt+LONG_HIGH3) = 0;							  			
  	*((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[9];
  	*((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[10];
  	*((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[11];
	*((char *)&gCardThisTransValue+LONG_HIGH3) = 0;
	*((char *)&gCardThisTransValue+LONG_HIGH2) = inbuf[14];
	*((char *)&gCardThisTransValue+LONG_HIGH1) = inbuf[13];
	*((char *)&gCardThisTransValue+LONG_HIGH0) = inbuf[12];
	
	g24GFlag = 0x55;
	
	ret = trans_update_24G(&inbuf[15]);
	if(ret != 0)
	{
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	//20130903
	memcpy(buf,"\xa2\x32\x0\x00",4);
  prot_zte_send_block(buf,4); 
	//prot_zte_rece_process(30);
	
	gStatus=IDLE_STATUS;
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = DEFAULT_KEY_INDEX;
	outbuf[4] = gKeyBacon;
	uword_to_buf(gCardCnt,&outbuf[5]);
	udword_to_buf(gPosTransCnt,&outbuf[7]);
	outbuf[11] = *((char *)&gCardThisRemain+LONG_HIGH0);
	outbuf[12] = *((char *)&gCardThisRemain+LONG_HIGH1);
	outbuf[13] = *((char *)&gCardThisRemain+LONG_HIGH2); 	
	outbuf[14] = gTAC[0];
	outbuf[15] = gTAC[1];
	outbuf[16] = gTAC[2];
	outbuf[17] = gTAC[3];	
	*outbytes = 18;
	gCPUflag = 0x55;
	gRespStep = 1;
	return; 
}
/************************************************************************************************
函数：ShMobileGetproof
功能：上海地铁手机钱包重取TAC
************************************************************************************************/
void ShMobileGetproof(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE ret;

#ifdef _DEBUG_BOM_
debug_printf("\nShMobileGetproof...");
#endif

	if(inbytes!=15)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if((gStatus!=TRANS_STATUS)&&(gStatus!=IDLE_STATUS))
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	//gStatus = IDLE_STATUS;
	ret = trans_getproof_24G(&inbuf[5]);
	if(ret != 0)
	{		
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = IDLE_STATUS;
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = DEFAULT_KEY_INDEX;
	outbuf[4] = gKeyBacon;
	uword_to_buf(gCardCnt,&outbuf[5]);
	udword_to_buf(gPosTransCnt,&outbuf[7]);
	memcpy(&outbuf[11],gTAC,4);
	outbuf[15] = 0;
	outbuf[16] = 0;
	*outbytes =17;
	gCPUflag = 0x55;
	gRespStep = 1;
	return; 
}
/************************************************************************************************
函数：ShMetrotestopenred
功能：上海地铁亮天线盘红灯
************************************************************************************************/
void ShMetrotestopenred(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
#ifdef _DEBUG_BOM_
debug_printf("\nShMetrotestopenred...");
#endif

	if(inbytes!=2)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	rledantenna(LED_ON);
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	*outbytes = 3;
	return; 
}
/************************************************************************************************
函数：ShMetrotestopengreen
功能：上海地铁亮天线盘绿灯
************************************************************************************************/
void ShMetrotestopengreen(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
#ifdef _DEBUG_BOM_
debug_printf("\nShMetrotestopengreen...");
#endif


	if(inbytes!=2)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gledantenna(LED_ON);
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	*outbytes = 3;
	return; 
}
/************************************************************************************************
函数：ShMetrotestclosered
功能：上海地铁灭天线盘红灯
************************************************************************************************/
void ShMetrotestclosered(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
#ifdef _DEBUG_BOM_
debug_printf("\nShMetrotestclosered...");
#endif

	if(inbytes!=2)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	rledantenna(LED_OFF);
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	*outbytes = 3;
	return; 
}
/************************************************************************************************
函数：ShMetrotestclosegreen
功能：上海地铁灭天线盘绿灯
************************************************************************************************/
void ShMetrotestclosegreen(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
#ifdef _DEBUG_BOM_
debug_printf("\nShMetrotestclosegreen...");
#endif

	if(inbytes!=2)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gledantenna(LED_OFF);
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	*outbytes = 3;
	return; 
}
/************************************************************************************************
函数：ShMetrotestrequest
功能：上海地铁单程票测试询卡
************************************************************************************************/
void ShMetrotestrequest(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE i,buf[5];

#ifdef _DEBUG_BOM_
debug_printf("\nShMetrotestrequest...");
#endif

	if(inbytes!=2)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	for(i=0;i<10;i++)
	{
		if(mcml_request(PICC_REQALL,buf)==0)
		{
			outbuf[2] = CE_OK;
			goto answer;
		}		
	}
	outbuf[2] = CE_UNKNOWN;
answer:
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	*outbytes =3;
	return; 
}
/************************************************************************************************
函数：ShMetrotestloadkey
功能：上海地铁单程票测试加载密钥
************************************************************************************************/
void ShMetrotestloadkey(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
#ifdef _DEBUG_BOM_
debug_printf("\nShMetrotestloadkey...");
#endif

	if(inbytes!=9)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	mcml_load_key(0,0,0,&inbuf[3]);
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	*outbytes =3;
	return; 
}
/************************************************************************************************
函数：ShMetrotestSJTauth
功能：上海地铁单程票测试验证密钥
************************************************************************************************/
void ShMetrotestSJTauth(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
#ifdef _DEBUG_BOM_
debug_printf("\nShMetrotestSJTauth...");
#endif

	if(inbytes!=3)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(mcml_authentication(0,0,0)!=0) 
		if(mcml_authentication(0,0,0)!=0)
			if(mcml_authentication(0,0,0)!=0)
			{
				outbuf[2] = CE_UNKNOWN;
				goto answer;
			}
	outbuf[2] = CE_OK;
answer:
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	*outbytes =3;
	return; 
}
/************************************************************************************************
函数：ShMetrotestSJTread
功能：上海地铁单程票测试读卡
************************************************************************************************/
void ShMetrotestSJTread(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
#ifdef _DEBUG_BOM_
debug_printf("\nShMetrotestSJTread...");
#endif

	if(inbytes!=3)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(tocken_read(inbuf[2],&outbuf[3])!=0)
	{
		gResponseStatus = CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	*outbytes = 7;
	return; 
}
/************************************************************************************************
函数：ShMetrotestSJTwrite
功能：上海地铁单程票测试写卡
************************************************************************************************/
void ShMetrotestSJTwrite(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
#ifdef _DEBUG_BOM_
debug_printf("\nShMetrotestSJTwrite...");
#endif

	if(inbytes!=7)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(tocken_read(inbuf[2],&inbuf[3])!=0)
	{
		gResponseStatus = CE_WRITEERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	*outbytes = 3;
	return;	
}

void ShMetrotestReadDebugstep(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
int cnt,i;
UBYTE ch;
#ifdef _DEBUG_BOM_
debug_printf("\nShMetrotestReadDebugstep...");
#endif

	if(inbytes!=2)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

//test start
//cnt = writecom(zte_comm,"\x02\x00\x04\xA2\x31\x00\x00\x93\x03",9);
//outbuf[9] = cnt;
//delay_ms(10);
//cnt = readcom(zte_comm,&outbuf[11],7);
//outbuf[10] = cnt;
//test end


	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3]=(UBYTE)(gDebugStep/256);
	outbuf[4]=(UBYTE)(gDebugStep%256);
	outbuf[5] = gGetCardInfoStep;
	outbuf[6] = gStatus;
	outbuf[7] = gTaskBusyFlag;
	outbuf[8] = wgAbortCmdCnt;
	*outbytes = 9;
	return;	
}

/*===============================================================================================
函数：shmt_bom_func_err
功能：错误命令处理
=================================================================================================*/
//#define _DEBUG_SHMT_BOM_FUNC_ERR_
void shmt_bom_func_err(UBYTE *outbuf,UWORD *outbytes)
{
	outbuf[0]=oldcmd;
	outbuf[1]=oldtimes;
	outbuf[2]=gResponseStatus;
	outbuf[3]=(UBYTE)(gDebugStep/256);
	outbuf[4]=(UBYTE)(gDebugStep%256);
	#ifdef _DEBUG_SHMT_BOM_FUNC_ERR_
	outbuf[5]=gGetCardInfoStep;
	outbuf[6]=gStatus;
	outbuf[7]=gTaskBusyFlag;
	outbuf[8]=wgAbortCmdCnt;
	*outbytes=9;
	#else
	*outbytes=3;
	#endif
	return;
}
/*===============================================================================================
函数：shmt_bom_func_bad_cmd
功能：不支持的命令处理
=================================================================================================*/
void shmt_bom_func_bad_cmd(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	outbuf[0]=inbuf[0];
	outbuf[1]=inbuf[1];
	outbuf[2]=CE_BADCOMMAND;
	*outbytes=3;
	return;
}

//PF
void ShMetroPfFileTransfer(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
int bytes;
UBYTE cRet;

#ifdef _DEBUG_BOM_
debug_printf("\nShMetroPfFileTransfer...");
#endif

	if(inbytes < 4)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
  //
  cRet = binFileManage(0x66,inbuf,inbytes,&outbuf[12],&bytes);
  //
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = cRet;
	outbuf[3] = 0x14;                       //读写器类型
	memcpy(&outbuf[4],"\x00\x00\x00\x00",4);//读写器内部软件版本（RFU）
	//memcpy(&outbuf[8],"\x55\x66\x77\x88",4);//读写器内部参数版本（4016版本号）
	memcpy(&outbuf[8],buf_4016,4);	//4016版本号
	*outbytes = 12 + bytes;
	return; 
}

void ShMetroPfActiveTpFile(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
#ifdef _DEBUG_BOM_
debug_printf("\nShMetroPfActiveTpFile...");
#endif

	if(inbytes < 2)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
  //
  active_tp_file(NULL,NULL);
  //
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = 0x15;       
	*outbytes = 4;
	return; 
}

void ShMetroPfPbocEntry(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
int ret;
UBYTE cnt;
UBYTE obuf[256];
UBYTE pfbuf[500];
unsigned short pfret;
UBYTE des_buf[100];
UBYTE des_bytes[10];

#ifdef _debug_test_
UDWORD	tint0=0,tint1=0;
#endif
#ifdef _DEBUG_BOM_
debug_printf("\nShMetroPfPbocEntry...");
#endif

	gRespStep = 0;
	if(gMode != ENTRY_MODE)
	{
		gResponseStatus = CE_BADCOMMAND;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(inbytes!=13)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = WRITE_STATUS;
	memcpy(gTransTime,&inbuf[2],7);
	if(check_time_valid() != 0) 
	{
		gStatus=IDLE_STATUS; 
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	
	#ifdef _debug_test_
tint0 = GetTickcount();
#endif

#ifdef DEBUG_TIME_CPU
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x20", 3);
#endif
/*#ifdef DEBUG_TIME_CPU
  //TEST-20130901
  memcpy(pfbuf, "\x80\xA8\x00\x00\x24\x83\x22\x28\x00\x00\x80\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x56\x00\x00\x00\x00\x00\x01\x56\x13\x05\x15\x00\x11\x22\x33\x44\x00", 41);
  ret = mifpro_icmd0(41, pfbuf, obuf);

	if(ret < 0)
	{
		memcpy(pfbuf, "\x01\x00\x21\xff", 4);
		pfret = abs(ret);
		pfbuf[4] = (UBYTE)(pfret >> 8);
		pfbuf[5] = (UBYTE)(pfret);
	}else
	{
		memcpy(pfbuf, "\x01\x00\x21\x00", 4);
		pfbuf[4] = (UBYTE)((ret) >> 8);
		pfbuf[5] = (UBYTE)((ret));
	}
	ReaderResponse(csc_comm, 0x00, 0xF0, pfbuf, 6);
	memcpy(pfbuf, "\x80\x1a\x28\x02\x08\x00\x11\x22\x33\x44\x55\x66\x77", 13);
	ret = sam_apdu(bgSmpsamIndex,pfbuf,13,obuf,&cnt,0,0);
	memcpy(pfbuf, "\x01\x00\xFF", 3);
	pfbuf[3] = (UBYTE)(ret >> 8);
	pfbuf[4] = (UBYTE)ret;
	ReaderResponse(csc_comm, 0x00, 0xF0, pfbuf, 6);
	memset(pfbuf, 0x00, 100);
	memcpy(&pfbuf[8], "\x84\xde\x00\xA8\x44", 5);
	memset(&pfbuf[13], 0x00, 64);
	pfbuf[77] = 0x80;
	ret = sam_apdu(bgSmpsamIndex, pfbuf, 80, obuf, &cnt, 0,0);
	memcpy(pfbuf, "\x01\x00\xFe", 3);
	pfbuf[3] = (UBYTE)(ret >> 8);
	pfbuf[4] = (UBYTE)ret;
	ReaderResponse(csc_comm, 0x00, 0xF0, pfbuf, 6);
#endif */

	//
	//ReaderResponse(csc_comm, 0x00, 0xF0, "\x10\x00\x01", 3);
	memset(obuf,0,sizeof(obuf));
	
  ret = pf_mobile_normal_ticket_entry(&inbuf[2],obuf);
#ifdef _debug_test_
	tint1 = GetTickcount();
	debug_printf("\n pf_mobile_normal_ticket_entry(%d)ret=%d",tint1-tint0,ret);
#endif
	//	pfbuf[0] = 0x10;
	//	pfbuf[1] = (UBYTE)((ret) >> 8);
	//	pfbuf[2] = (UBYTE)((ret));
	//ReaderResponse(csc_comm, 0x00, 0xF0, pfbuf, 3);
	#ifdef _DEBUG_BOM_
		printf("\n pf_mobile_normal_ticket_entry:ret=%d",ret);
	#endif

#ifdef DEBUG_TIME_CPU
	memcpy(pfbuf, "\x01\x00\xFe", 3);
	if(ret < 0)
	{
		memcpy(pfbuf, "\x01\x00\x21\xff", 4);
		pfret = abs(ret);
		pfbuf[4] = (UBYTE)(pfret >> 8);
		pfbuf[5] = (UBYTE)(pfret);
	}else
	{
		memcpy(pfbuf, "\x01\x00\x21\x00", 4);
		pfbuf[4] = (UBYTE)((ret) >> 8);
		pfbuf[5] = (UBYTE)((ret));
	}
	ReaderResponse(csc_comm, 0x00, 0xF0, pfbuf, 6);
#endif
	if(ret > 0)
	{
		mcml_pwr_off();
		gStatus=IDLE_STATUS;
		gResponseStatus = CE_INVALIDCARD;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	else if(ret < 0)
		{
			mcml_pwr_off();
			gStatus=IDLE_STATUS;
			if(ret == -308)
			{
				gResponseStatus = CE_CARDREMOVED;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			}
			else{
				gResponseStatus = CE_WRITEERROR;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			}
		}

/*	if(ret != 0)
	{
		gStatus = IDLE_STATUS;
		gResponseStatus = ret;
		shmt_bom_func_err(outbuf,outbytes);
		return;
		//outbuf[2] = *((char*)&ret+1);	
		//outbuf[3] = *((char*)&ret+0);	
	}
	*/
		//2013/10/29 16:36:34
		//ret = mifpro_deselect(0,des_buf,des_bytes);
		//#ifdef _DEBUG_BOM_
		//	printf("Deselect ret=%d\n",ret);
		//#endif
		
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	memcpy(&outbuf[3],&obuf[1],96);
	*outbytes = 99; 
	gRespStep = 1;
	gStatus=IDLE_STATUS;
	//mcml_halt();
	mcml_pwr_off();
	 
	bgSMPADF = 1;
	set_task_key(TASK_KEY_SELECT_ADF);
	return; 
}

void ShMetroReaderVer(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
int ret;
UBYTE obuf[256];

#ifdef _DEBUG_BOM_
debug_printf("\nShMetroReaderVer...");
#endif

	gRespStep = 0;
	
	if(inbytes!=3)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = 0x00;
	outbuf[3] = 'A';
	memcpy(&outbuf[4],"\x00\x00\x00\x03",4);
	memcpy(&outbuf[8],"\x00\x00\x00\x07",4);
	*outbytes = 12; 
	gRespStep = 1;
	//gStatus=IDLE_STATUS;
	//mcml_halt();
	return; 
}

void ShMetroPfPbocExit(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
int ret;
UBYTE cnt;
UBYTE obuf[256],temp_buf[3];
#ifdef _debug_test_
UDWORD	tint0=0,tint1=0;
#endif	
#ifdef _DEBUG_BOM_
debug_printf("\nShMetroPfPbocExit...");
#endif

	//#ifdef DEBUG_TIME_CPU
	//	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x40", 3);
	//#endif

	if(gMode != EXIT_MODE)
	{
		gResponseStatus = CE_BADCOMMAND;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(inbytes!=18)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = WRITE_STATUS;
	//
	temp_buf[2]=inbuf[12];
	temp_buf[1]=inbuf[13];
	temp_buf[0]=inbuf[14];
	memcpy(&inbuf[12],temp_buf,3);
	#ifdef _debug_test_
		tint0 = GetTickcount();
	#endif
	
	#ifdef DEBUG_TIME_CPU
		ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x40", 3);
	#endif
	
	memset(obuf,0,sizeof(obuf));
	ret = pf_mobile_normal_ticket_exit(&inbuf[2],obuf);
	#ifdef _debug_test_
		tint1 = GetTickcount();
		debug_printf("\n pf_mobile_normal_ticket_exit(%d)ret=%d",tint1-tint0,ret);
	#endif

	#ifdef DEBUG_TIME_CPU
		ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x41", 3);
	#endif

	if(ret > 0)
	{
		mcml_pwr_off();
		gStatus=IDLE_STATUS;
		gResponseStatus = CE_INVALIDCARD;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	else if(ret < 0)
		{
			mcml_pwr_off();
			gStatus=IDLE_STATUS;
			if(ret == -308)
			{
				gResponseStatus = CE_CARDREMOVED;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			}
			else{
				gResponseStatus = CE_WRITEERROR;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			}
		}
/*	if(ret)
	{	
		//gStatus = IDLE_STATUS;
		//gResponseStatus = ret;	
		//shmt_bom_func_err(outbuf,outbytes);
		//return;
		gStatus=IDLE_STATUS;
		outbuf[0] = oldcmd;
		outbuf[1] = oldtimes;
		outbuf[2] = obuf[0];
		outbuf[3] = *((char*)&ret+1);	
		outbuf[4] = *((char*)&ret+0);	
		outbuf[5] = 0xff;			
		cnt = 6;
		shmt_prot_send(outbuf,cnt);
		return;
		
	}
*/
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	memcpy(&outbuf[3],&obuf[1],96);
	*outbytes = 99;
	gStatus=IDLE_STATUS;
	//mcml_halt();
	mcml_pwr_off();
	return; 
}
void ShMetroPfGetRec(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
int ret;
UBYTE obuf[256];
#ifdef _debug_test_
UDWORD	tint0=0,tint1=0;
#endif	
#ifdef _DEBUG_BOM_
debug_printf("\nShMetroPfGetRec...");
#endif

//	if(gMode != EXIT_MODE)
//	{
//		gResponseStatus = CE_BADCOMMAND;
//		shmt_bom_func_err(outbuf,outbytes);
//		return;
//	}
	if(inbytes!=4)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	//gStatus = WRITE_STATUS;
	//
	#ifdef _debug_test_
tint0 = GetTickcount();
#endif
	memset(obuf,0,sizeof(obuf));
	ret =  pf_mobile_ticket_get_record(&inbuf[2],obuf);
	#ifdef _debug_test_
tint1 = GetTickcount();
debug_printf("\n  pf_mobile_ticket_get_record(%d)ret=%d",tint1-tint0,ret);
#endif
	if(ret)
	{	
		gStatus = IDLE_STATUS;
		gResponseStatus = CE_READERROR;	
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = obuf[0];
	memcpy(&outbuf[3],&obuf[1],33);
	*outbytes = 36;
//	gStatus=IDLE_STATUS;
	//mcml_halt();
	
	return; 
}

void ShMetroPfEtGetRec(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
int ret;
UBYTE obuf[256];
#ifdef _debug_test_
UDWORD	tint0=0,tint1=0;
#endif	
#ifdef _DEBUG_BOM_
debug_printf("\nShMetroPfEtGetRec...");
#endif

//	if(gMode != EXIT_MODE)
//	{
//		gResponseStatus = CE_BADCOMMAND;
//		shmt_bom_func_err(outbuf,outbytes);
//		return;
//	}
	if(inbytes!=4)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	//gStatus = WRITE_STATUS;
	//
	memset(obuf,0,sizeof(obuf));
	#ifdef _debug_test_
tint0 = GetTickcount();
#endif
	memset(obuf,0,sizeof(obuf));
	ret =  pf_mobile_staff_ticket_get_record(&inbuf[2],obuf);
	#ifdef _debug_test_
tint1 = GetTickcount();
debug_printf("\n  pf_mobile_staff_ticket_get_record(%d)ret=%d",tint1-tint0,ret);
#endif
	if(ret)
	{	
		gStatus = IDLE_STATUS;
		gResponseStatus = CE_READERROR;	
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = obuf[0];
	memcpy(&outbuf[3],&obuf[1],33);
	*outbytes = 36;
	//gStatus=IDLE_STATUS;
	mcml_halt();
	return; 
} 
void ShMetroPfPbocUpdate(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
int ret;
UBYTE cnt;
UBYTE obuf[256],temp_buf[3];
	
#ifdef _DEBUG_BOM_
debug_printf("\nShMetroPfPbocUpdate...");
#endif

	if(inbytes!=31)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus != TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}	
	memcpy(gTransTime,&inbuf[2],7);
	if(check_time_valid() != 0) 
	{		
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus=IDLE_STATUS; 
	temp_buf[2]=inbuf[12];
	temp_buf[1]=inbuf[13];
	temp_buf[0]=inbuf[14];
	memcpy(&inbuf[12],temp_buf,3); 
	#ifdef _DEBUG_BOM_
		debug_printf("\npfpboc_update Start!");
	#endif
	memset(obuf,0,sizeof(obuf));
	ret = pf_mobile_normal_ticket_update(&inbuf[2],obuf);
	
	#ifdef _DEBUG_BOM_
		debug_printf("\npfpboc_update End! ret=%d ",ret);
	#endif
	
	if(ret > 0)
	{
		gResponseStatus = CE_INVALIDCARD;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	else if(ret < 0)
		{
			if((ret == -310) || (ret == -311))
			{
				gResponseStatus = CE_CARDREMOVED;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			}
			else{
				gResponseStatus = CE_WRITEERROR;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			}
		}
	/*
	if(ret)
	{
		gResponseStatus = ret;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
*/
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	memcpy(&outbuf[3],&obuf[1],96);		
	*outbytes = 99;
	return; 
}

void ShMetroPfPbocGetProof(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
int ret;
UBYTE obuf[256];
	
#ifdef _DEBUG_BOM_
debug_printf("\nShMetroPfPbocGetProof...");
#endif

	if(inbytes != 15)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus != TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	//memcpy(gTransTime,&inbuf[2],7);
	//if(check_time_valid() != 0) 
	//{		
	//	gResponseStatus = CE_BADPARAM;
	//	shmt_bom_func_err(outbuf,outbytes);
	//	return;
	//}
	gStatus=IDLE_STATUS;
  //
  memset(obuf,0,sizeof(obuf));
  ret = pf_mobile_normal_ticket_get_proof(&inbuf[2],obuf);
  
  	if(ret > 0)
	{
		gResponseStatus = CE_INVALIDCARD;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	else if(ret < 0)
		{
			if((ret == -311) || (ret == -310))
			{
				gResponseStatus = CE_CARDREMOVED;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			}
			else{
				gResponseStatus = CE_WRITEERROR;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			}
		}
//  if(ret) {
//		gResponseStatus = ret;
//		shmt_bom_func_err(outbuf,outbytes);
//		return;  	
//  }
  //
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = obuf[0];
	//memcpy(&outbuf[3],&obuf[1],91);
	memcpy(&outbuf[3],&obuf[1],96);          
	*outbytes = 99;
	return; 
}

void ShMetroPfPbocGetParamVer(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
		UBYTE buf[4];
#ifdef _DEBUG_BOM_
debug_printf("\nShMetroPfPbocGetParamVer...");
#endif

	if(inbytes != 3)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	
	//读写器类型
	if(gMode==BOM_MODE)
	{
		outbuf[3]=BOM_READER;
	}
	else
	{
		if(gMode==ENTRY_MODE)
		{
			outbuf[3]=GATE_ENTRY_READER;
		}
		else
		{
			outbuf[3]=GATE_EXIT_READER;
		}
	}
  //
	if(inbuf[2]==0x00)              //1,4020,else4026
	{
		memcpy(&outbuf[8],buf_4016,4);	//4016版本号
	}
	if(inbuf[2]==0x01)
	{
		LongToByte(tpWhite4020.white_head.ParaVersion,&outbuf[8]);
	}
	else
	{
		//memcpy(&outbuf[8],tpWhite4020.white_head.ParaVersion,4);	//4020版本号
		LongToByte(tpCert4026.cert_head.ParaVersion,&outbuf[8]); 
		//memcpy(&outbuf[8],&tpWhite4020.white_head.ParaVersion,4);
//		outbuf[8] = buf_4020[0];
//		outbuf[9] = buf_4020[1];
//		outbuf[10] = buf_4020[2];
//		outbuf[11] = buf_4020[3];
	}
  //
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	//outbuf[3] = 0x15; 
	memcpy(&outbuf[4],"\x0\x0\x0\x0",4);//读写器内部软件版本（RFU）
	outbuf[7] = (gVersion & 0x1F);      
	*outbytes = 12;
	return; 
}

//下4016
void ShMetroPfPbocSetParam(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
int ret;
int bytes;
UBYTE cRet;
	
#ifdef _DEBUG_BOM_
debug_printf("\nShMetroPfPbocSetParam...");
#endif

	if(inbytes < 2)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
  //
  //读写器类型
	if(gMode==BOM_MODE)
	{
		outbuf[3]=BOM_READER;
	}
	else
	{
		if(gMode==ENTRY_MODE)
		{
			outbuf[3]=GATE_ENTRY_READER;
		}
		else
		{
			outbuf[3]=GATE_EXIT_READER;
		}
	}
  
  //ret = pf_mobile_ticket_para_init(inbuf);//最后一个包时更新参数
  //
  cRet = binFileManage(0x99,inbuf,inbytes,&outbuf[12],&bytes);
  memcpy(&outbuf[8],buf_4016,4);	//4016版本号
  //
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = cRet;
	//outbuf[3] = 0x14;                       //读写器类型
	memcpy(&outbuf[4],"\x0\x0\x0\x0",4);//读写器内部软件版本（RFU）
	outbuf[7] = (gVersion & 0x1F);
	//memcpy(&outbuf[8],"\x55\x66\x77\x88",4);//读写器内部参数版本（4016版本号）
	*outbytes = 12 + bytes;
	return; 
}

//下载参数：4020 白名单
void shWhiteListSetParam(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
int ret;
int bytes;
UBYTE cRet;
	
#ifdef _DEBUG_BOM_
debug_printf("\nshWhiteListSetParam...");
#endif

	if(inbytes < 2)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
  //
  //读写器类型
	if(gMode==BOM_MODE)
	{
		outbuf[3]=BOM_READER;
	}
	else
	{
		if(gMode==ENTRY_MODE)
		{
			outbuf[3]=GATE_ENTRY_READER;
		}
		else
		{
			outbuf[3]=GATE_EXIT_READER;
		}
	}
  
  //ret = pf_mobile_ticket_para_init(inbuf);//最后一个包时更新参数
  //
  cRet = binFileManage(0x88,inbuf,inbytes,&outbuf[12],&bytes);
  //
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = cRet;
	//outbuf[3] = 0x14;                       //读写器类型
	memcpy(&outbuf[4],"\x0\x0\x0\x0",4);//读写器内部软件版本（RFU）
	outbuf[7] = (gVersion & 0x1F);
	//memcpy(&outbuf[8],"\x55\x66\x77\x88",4);//读写器内部参数版本（4020版本号）
	LongToByte(tpWhite4020.white_head.ParaVersion,&outbuf[8]);
	*outbytes = 12 + bytes;
	return; 
}


void ShMetroPfEtEntry(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
int ret;
UBYTE obuf[256];
#ifdef _debug_test_
UDWORD	tint0=0,tint1=0;
#endif
#ifdef _DEBUG_BOM_
debug_printf("\nShMetroPfEtEntry...");
#endif

	gRespStep = 0;
	if(gMode != ENTRY_MODE)
	{
		gResponseStatus = CE_BADCOMMAND;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(inbytes!=13)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = WRITE_STATUS;
	memcpy(gTransTime,&inbuf[2],7);
	if(check_time_valid() != 0) 
	{
		gStatus=IDLE_STATUS; 
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
#ifdef DEBUG_TIME_CPU
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x30", 3);
#endif
	#ifdef _debug_test_
tint0 = GetTickcount();
#endif
	memset(obuf,0,sizeof(obuf));
  ret = pf_mobile_staff_ticket_entry(&inbuf[2],obuf);
  #ifdef _debug_test_
tint1 = GetTickcount();
debug_printf("\n pf_mobile_staff_ticket_entry(%d),ret=%d",tint1-tint0,ret);
#endif
#ifdef DEBUG_TIME_CPU
	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x31", 3);
#endif
 

		if( ret < 0)
		{
			mcml_pwr_off();
			gStatus=IDLE_STATUS;
			if(ret == -308)
			{
				gResponseStatus = CE_CARDREMOVED;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			}
			else{
				gResponseStatus = CE_WRITEERROR;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			}
		}

	/*if(ret != 0)
	{
		gStatus = IDLE_STATUS;
		gResponseStatus = ret;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	*/
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = obuf[0];
	memcpy(&outbuf[3],&obuf[1],14);
	*outbytes = 17; 
	gRespStep = 1;
	gStatus=IDLE_STATUS;
	mcml_pwr_off();//
	return; 
}

void ShMetroPfEtExit(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
int ret;
UBYTE obuf[256],temp_buf[3];
#ifdef _debug_test_
UDWORD	tint0=0,tint1=0;
#endif	
#ifdef _DEBUG_BOM_
debug_printf("\nShMetroPfEtExit...");
#endif

	if(gMode != EXIT_MODE)
	{
		gResponseStatus = CE_BADCOMMAND;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(inbytes!=18)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = WRITE_STATUS;
	//
	temp_buf[2]=inbuf[12];
	temp_buf[1]=inbuf[13];
	temp_buf[0]=inbuf[14];
	memcpy(&inbuf[12],temp_buf,3);
	#ifdef _debug_test_
	tint0 = GetTickcount();
	#endif
	memset(obuf,0,sizeof(obuf));
	ret = pf_mobile_staff_ticket_exit(&inbuf[2],obuf);
	#ifdef _debug_test_
	tint1 = GetTickcount(); 
	debug_printf("\n pf_mobile_staff_ticket_exit(%d)ret=%d",tint1-tint0,ret);
	#endif


		if(ret < 0)
		{
			mcml_pwr_off();
			gStatus=IDLE_STATUS;
			if(ret == -308)
			{
				gResponseStatus = CE_CARDREMOVED;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			}
			else{
				gResponseStatus = CE_WRITEERROR;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			}
		}
/*	if(ret)
	{	
		gStatus = IDLE_STATUS;
		gResponseStatus = ret;	
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	*/
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = obuf[0];
	memcpy(&outbuf[3],&obuf[1],15);
	outbuf[11] = obuf[11];outbuf[13] = obuf[9]; //钱包余额（低字节在前）
	*outbytes = 18;
	gStatus=IDLE_STATUS;
	mcml_pwr_off();//
	return; 
}

void ShMetroPfEtUpdate(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
int ret;
UBYTE obuf[256],temp_buf[3]; 
	
#ifdef _DEBUG_BOM_
debug_printf("\nShMetroPfEtUpdate...");
#endif

	if(inbytes!=31)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus != TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}	
	memcpy(gTransTime,&inbuf[2],7);
	if(check_time_valid() != 0) 
	{		
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus=IDLE_STATUS; 
	temp_buf[2]=inbuf[12];
	temp_buf[1]=inbuf[13];
	temp_buf[0]=inbuf[14];
	memcpy(&inbuf[12],temp_buf,3);
	#ifdef _DEBUG_BOM_
		debug_printf("\npfpboc_update Start! ");
	#endif
	memset(obuf,0,sizeof(obuf));
	ret = pf_mobile_staff_ticket_update(&inbuf[2],obuf);
	
	#ifdef _DEBUG_BOM_
		debug_printf("\npfpboc_update End!");
	#endif
	
		if(ret < 0)
		{
			if(ret == (-308))
			{
				gResponseStatus = CE_CARDREMOVED;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			}
			else{
				gResponseStatus = CE_WRITEERROR;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			}
		}
/*	if(ret)
	{
		gResponseStatus = ret;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	*/
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = obuf[0];
	memcpy(&outbuf[3],&obuf[1],15);		
	outbuf[11] = obuf[11];outbuf[13] = obuf[9]; //钱包余额（低字节在前）
	*outbytes = 18;
	return; 
}

void ShMetroPfEtGetProof(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
int ret;
UBYTE obuf[256];
	
#ifdef _DEBUG_BOM_
debug_printf("\nShMetroPfEtGetProof...");
#endif

	if(inbytes != 15)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus != TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	//memcpy(gTransTime,&inbuf[2],7);
	//if(check_time_valid() != 0) 
	//{		
	//	gResponseStatus = CE_BADPARAM;
	//	shmt_bom_func_err(outbuf,outbytes);
	//	return;
	//}
	gStatus=IDLE_STATUS;	
  //
  memset(obuf,0,sizeof(obuf));
  ret = pf_mobile_staff_ticket_get_proof(&inbuf[2],obuf);
  

		if(ret < 0)
		{
			if((ret == -311) || (ret == -310))
			{
				gResponseStatus = CE_CARDREMOVED;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			}
			else{
				gResponseStatus = CE_WRITEERROR;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			}
		}
  
/*  if(ret) {
		gResponseStatus = ret;
		shmt_bom_func_err(outbuf,outbytes);
		return;  	
  }
  */
  //
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = obuf[0];
	memcpy(&outbuf[3],&obuf[1],15);  
	outbuf[11] = obuf[11];outbuf[13] = obuf[9]; //
	*outbytes = 18;
	return; 
}

void ShMetroPfEtLockCard(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
int ret;
UBYTE obuf[256];
	
#ifdef _DEBUG_BOM_
debug_printf("\nShMetroPfEtLockCard...");
#endif

	if(inbytes != 13)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus != TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	memcpy(gTransTime,&inbuf[2],7);
	if(check_time_valid() != 0) 
	{		
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus=IDLE_STATUS;	
  //
  memset(obuf,0,sizeof(obuf));
  ret = pf_mobile_staff_ticket_lock(&inbuf[2],obuf);
  if(ret) {
		gResponseStatus = ret;
		shmt_bom_func_err(outbuf,outbytes);
		return;  	
  }
  //
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = obuf[0];
	memcpy(&outbuf[3],&obuf[1],16);
	outbuf[12] = obuf[12];outbuf[14] = obuf[10]; //钱包余额（低字节在前）       
	*outbytes = 19;
	return; 
}
/************************************************************************************************
函数：ShMetroGethwVer
功能：读写器软硬件版本
************************************************************************************************/
void ShMetroGethwVer(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
int ret;
UBYTE obuf[256];
	
#ifdef _DEBUG_BOM_
debug_printf("\nShMetroGethwVer......");
#endif

	if(inbytes != 3)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

  //
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = 0x02; 				//厂家代码：1华铭，2华虹计通，3怡力，4邮通，5华腾，6华腾智能，7 DAT，8冠人
	//outbuf[4] = 0x00;   
	outbuf[4] = gVersion/32;	//硬件版本
	memcpy(&outbuf[5],"\x0\x0\x0\x0",4);  //软件版本
	outbuf[8] = (gVersion & 0x1F);
	*outbytes = 9;
	return; 
}
/************************************************************************************************
函数：ShTestHardWare
功能：读写器硬件检测
************************************************************************************************/
void ShTestHardWare(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
int ret;
UBYTE obuf[256];
UBYTE out_buf[100];
UBYTE olen[10];
int cnt;
UBYTE ee_buf[8192];
UWORD ee_addr,ee_bytes;
	
#ifdef _DEBUG_BOM_
debug_printf("\nShTestHardWare......");
#endif

	if(inbytes != 10)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	switch(inbuf[2])
	{
		case 1:		//antenna
					obuf[0] = reader_test_antenna(&inbuf[3], out_buf, olen);
					if(obuf[0])
						{
							//gResponseStatus = out_buf[0];
							//shmt_bom_func_err(outbuf,outbytes);
							outbuf[0] = oldcmd;
							outbuf[1] = oldtimes;
							outbuf[2] = out_buf[0];
							outbuf[3] = out_buf[1];
							*outbytes = 4;
							return;
						}
							outbuf[0] = oldcmd;
							outbuf[1] = oldtimes;
							//outbuf[1] = inbuf[1];
							outbuf[2] = CE_OK;
							outbuf[3] = out_buf[1];
							*outbytes = 4;
							return; 
					break;
		case 2:
					obuf[0] = reader_test_littlesam(inbuf[3], out_buf, olen);
					if(obuf[0])
						{
							gResponseStatus = out_buf[0];
							shmt_bom_func_err(outbuf,outbytes);
							//outbuf[0] = oldcmd;
							//outbuf[1] = inbuf[2];
							//outbuf[2] = inbuf[3];
							//outbuf[3] = out_buf[0];
							//*outbytes = 4;
							return;
						}
					break;
		case 3:
					obuf[0] = reader_test_eeprom();
					if(obuf[0])
						{
							gResponseStatus = obuf[0];
							shmt_bom_func_err(outbuf,outbytes);
							//outbuf[0] = oldcmd;
							//outbuf[1] = inbuf[2];
							//outbuf[2] = inbuf[3];
							//outbuf[3] = obuf[0];
							//*outbytes = 4;
							return;
						}
					break;
		case 4:
					obuf[0] = reader_test_rtc(1, &inbuf[4], olen);
					if(obuf[0])
						{
							gResponseStatus = obuf[0];
							shmt_bom_func_err(outbuf,outbytes);
							//outbuf[0] = oldcmd;
							//outbuf[1] = inbuf[2];
							//outbuf[2] = obuf[0];
							//*outbytes = 3;
							return;
						}
						else{
							outbuf[0] = oldcmd;
							outbuf[1] = oldtimes;
							outbuf[2] = CE_OK;
							memcpy(&outbuf[3],&inbuf[3],7);
							*outbytes = 3+7;
							return;
						}
					break;
		case 7:
					obuf[0] = reader_test_24G(0,out_buf,olen);
					//printf("obuf[0] = %02X\n",obuf[0]);
					if(obuf[0])
					{
							gResponseStatus = out_buf[0];
							shmt_bom_func_err(outbuf,outbytes);
							return;
					}
					
					break;
		case 5:
					if(inbuf[3] == 0) //lred
					{
							if(inbuf[4] == 1)
									rled(LED_ON);
							else
									rled(LED_OFF);
					}
					else{
							if(inbuf[4] == 1)
										gled(LED_ON);
								else
										gled(LED_OFF);
					}
					break;
		case 6:
					*((char *)&ee_addr+INT_HIGH) = inbuf[3];
					*((char *)&ee_addr+INT_LOW) = inbuf[4];
					*((char *)&ee_bytes+INT_HIGH) = inbuf[5];
					*((char *)&ee_bytes+INT_LOW) = inbuf[6];
					if(ee_addr > 8191 || ee_bytes > (8191 - ee_addr))
					{
							gResponseStatus = CE_BADPARAM;
							shmt_bom_func_err(outbuf,outbytes);
							return;
					}
					memset(ee_buf,0,sizeof(ee_buf));
					//ReaderResponse(csc_comm, 0x00, 0xEE, "\x01\x00\x00", 3);
					ret = ee_read(ee_addr,ee_bytes,ee_buf);
					if(ret)
					{
							gResponseStatus = 0x10;
							shmt_bom_func_err(outbuf,outbytes);
							return;
					}
					//ReaderResponse(csc_comm, 0x00, 0xEE, "\x01\x00\x01", 3);
					ret = ee_write(ee_addr,ee_bytes,ee_buf);
					//ReaderResponse(csc_comm, 0x00, 0xEE, "\x01\x00\x02", 3);
					if(ret)
					{
							gResponseStatus = 0x20;
							shmt_bom_func_err(outbuf,outbytes);
							return;
					}
					
					break;
		case 9:
					if(inbuf[3] == 1)					
							//global_debug_flag_set();
					if(inbuf[3] == 0)
							//glabal_debug_flag_clr();
					//global_debug_flag_init();
					
					break;
		default:	
					gResponseStatus = CE_BADPARAM;
					shmt_bom_func_err(outbuf,outbytes);
					return;		
	}

  //
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	//outbuf[1] = inbuf[1];
	outbuf[2] = CE_OK;
	*outbytes = 3;
	return; 
}
/************************************************************************************************
函数：ShMetro24GModel
功能：2.4G模块测试
************************************************************************************************/
void ShMetro24GModel(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	int ret;
	UBYTE buf[10];
	//int i;
	//UBYTE buf[20];
		
	#ifdef _DEBUG_BOM_
	debug_printf("\nShMetro24GModel......");
	#endif

	if(inbytes != 3)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

	outbuf[4] = gPPSMode;
	
	bgTest24GFlag = 0;
	if(inbuf[2] != 0)
	{
			gPPSMode = 1;
	}
	else{
			gPPSMode = 0;
	}
	buf[0] = gPPSMode;
	lrc_gen2(buf,1);
	ee_write(2048,2,buf);

	outbuf[0] = TEST_24G;
	outbuf[1] = gReadCmdTimes;
	outbuf[2] = 0;
	outbuf[3] = gPPSMode;
	//outbuf[4] = 0;
	*outbytes =5;
	
	//sem_post(&g_usermainztewait);
	return; 
}

/************************************************************************************************
函数：ShQREntry
功能：上海地铁二维码进站交易
************************************************************************************************/
void ShQREntry(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE buf[100], obuf[100];
	#ifdef _debug_test_
	UDWORD	tint0=0,tint1=0;
	#endif

#ifdef _DEBUG_BOM_
debug_printf("\nShQREntry...");
#endif
	
	if(gMode != ENTRY_MODE)
	{
		gResponseStatus = CE_BADCOMMAND;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(inbytes != 20)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus != TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = WRITE_STATUS;
	gTransTime[0]=inbuf[5];
	gTransTime[1]=inbuf[4];
	gTransTime[2]=inbuf[3];
	gTransTime[3]=inbuf[2];
	*((char *)&gPosTransCnt+LONG_HIGH3) = inbuf[15];							  			
	*((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[14];
	*((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[13];
	*((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[12];
	gCardThisTransValue=0;
	
	#ifdef _debug_test_
		tint0 = GetTickcount();
	#endif
	
	if(qr_trans_entry(inbuf, obuf)!=0)
	{	
		mcml_pwr_off();
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	#ifdef _debug_test_
		tint1 = GetTickcount();
		debug_printf("\n sh_metro_entry(%d) \n",tint1-tint0);
	#endif
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	//手机当前时间
	memcpy(&outbuf[3], &obuf[1], 4); 		
	memcpy(&outbuf[7],gTAC,4);
	memset(&outbuf[11], 0x00, 3);
	*outbytes = 14;
	gStatus=IDLE_STATUS;
	//mcml_pwr_off();//
	
	#ifdef _RECOVERY_AFTER_RESET_
		if(bgResetWrTimeFlag){
		 rtc_wr_time(&gTransTime[1]);
		 bgResetWrTimeFlag=0;
		 }
	#endif
	
	return;	
}

/************************************************************************************************
函数：ShQRExit
功能：上海地铁二维码出站交易
************************************************************************************************/
void ShQRExit(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE buf[100], obuf[100];
	#ifdef _debug_test_
	UDWORD	tint0=0,tint1=0;
	#endif

#ifdef _DEBUG_BOM_
debug_printf("\nShqr exit...");
#endif
	
	if(gMode != EXIT_MODE)
	{
		gResponseStatus = CE_BADCOMMAND;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(inbytes!=22)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = WRITE_STATUS;
	gTransTime[0]=inbuf[5];
	gTransTime[1]=inbuf[4];
	gTransTime[2]=inbuf[3];
	gTransTime[3]=inbuf[2];
	*((char *)&gPosTransCnt+LONG_HIGH3) = inbuf[9];							  			
	*((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[8];
	*((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[7];
	*((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[6];
	*((char *)&gCardThisTransValue+LONG_HIGH0) =  inbuf[16];
	*((char *)&gCardThisTransValue+LONG_HIGH1) =  inbuf[17];
	
	#ifdef _debug_test_
	tint0 = GetTickcount();
	#endif
	if(qr_trans_exit(inbuf, obuf)!=0)
	{	
		mcml_pwr_off();
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	#ifdef _debug_test_
	tint1 = GetTickcount();
	debug_printf("\n sh_qr_exit(%d) \n",tint1-tint0);
	#endif
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	//手机当前时间
	memcpy(&outbuf[3], &obuf[1], 4); 		
	memcpy(&outbuf[7],gTAC,4);
	memset(&outbuf[11], 0x00, 3);
	*outbytes = 14;
	gStatus=IDLE_STATUS;

	
	#ifdef _RECOVERY_AFTER_RESET_
	if(bgResetWrTimeFlag){
		 rtc_wr_time(&gTransTime[1]);
		 bgResetWrTimeFlag=0;
		 }
	#endif
	return;	
}

/************************************************************************************************
函数：ShCardUpdate
功能：二维码更新
	从命令代码起始
************************************************************************************************/
void ShQRUpdate(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE ret;
	UBYTE obuf[200];

#ifdef _DEBUG_BOM_
debug_printf("\nSh QR Update...");
#endif

	if(inbytes!=47)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus != TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}	
	memcpy(gTransTime,&inbuf[2],4);
	*((char *)&gPosTransCnt+LONG_HIGH3) = inbuf[12];							  			
	*((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[13];
	*((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[14];
	*((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[15];
	*((char *)&gCardThisTransValue+LONG_HIGH3) = 0;
	*((char *)&gCardThisTransValue+LONG_HIGH2) = inbuf[18];
	*((char *)&gCardThisTransValue+LONG_HIGH1) = inbuf[17];
	*((char *)&gCardThisTransValue+LONG_HIGH0) = inbuf[16];
	gStatus=IDLE_STATUS; 
	ret = qr_trans_update(inbuf,obuf);
	if(ret)
	{
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	//
	memcpy(&outbuf[3], &obuf[1], 4);
	memcpy(&outbuf[7], gTAC, 4);
	memset(&outbuf[11],0x00, 3);		
	*outbytes = 14;

	return;
}
/************************************************************************************************
函数：ShMetroGetProof
功能：上海地铁单程票根据流水号取TAC
************************************************************************************************/
void ShQRGetproof(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE buf[20];

#ifdef _DEBUG_BOM_
debug_printf("\nSh QR GetProof...");
#endif

	if(inbytes!=24)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if((gStatus != IDLE_STATUS)&&(gStatus != TRANS_STATUS))
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(get_qr_record(&inbuf[2],buf)!=0)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	memcpy(&outbuf[3],&buf[0],8);
	memset(&outbuf[11], 0x00, 3);
	*outbytes =14;
	return; 
}

/************************************************************************************************
函数：ShQRRead
功能：上海地铁纯读取二维码信息
************************************************************************************************/
void ShQRRead(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
UBYTE buf[100], obuf[100];
UWORD olen;
	#ifdef _debug_test_
	UDWORD	tint0=0,tint1=0;
	#endif

#ifdef _DEBUG_BOM_
debug_printf("\nShQRread...");
#endif
	
//	if(gMode != ENTRY_MODE)
//	{
//		gResponseStatus = CE_BADCOMMAND;
//		shmt_bom_func_err(outbuf,outbytes);
//		return;
//	}
//	if(inbytes != 20)
//	{
//		gResponseStatus = CE_BADPARAM;
//		shmt_bom_func_err(outbuf,outbytes);
//		return;
//	}
	if(gStatus == READ_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	
	InitScan();
	StartScan();
	if((gResponseStatus = qr_read(inbuf, obuf, &olen)) !=0 )
	{	
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	//
	outbuf[3] = 0x80;
	memcpy(&outbuf[4], &obuf[0], olen); 		
	*outbytes = 4 + olen;
	
	return;	
}

void shSetDateTime(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes)
{
UBYTE buf[24], timeBCD[7];
int ret;

	memcpy(gTransTime,&inbuf[2],7);
	if(check_time_valid() != 0) 
	{
		//gStatus=IDLE_STATUS; 
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

	//矫正读写器内时间
	memcpy(&timeBCD[0], &inbuf[2], 7);
	rtc_wr_time(&timeBCD[1]);

	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	*outbytes =  3; 

	return ;
}


//BOM相关应用
void shCardGetkey(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes)
{
    if(inbytes != 3)
 	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
   
    if((gStatus != IDLE_STATUS) && (gStatus != TRANS_STATUS)){
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
    //
    if(hl_get_module_no(gBuf) == 0) 
    	memcpy(&outbuf[3],gBuf,6);
    else  
    {
		if(hl_get_module_no(gBuf) == 0) 
			memcpy(&outbuf[3],gBuf,6);
		else 
		{
			gResponseStatus = CE_COMERR;
			memset(&outbuf[3],0,6);
			gSBufBytes = 25;
	        goto label_send;
		}
	}
    //
    if(hl_get_random(0,gBuf) == 0)  
    	memcpy(&outbuf[9],gBuf,16); 
    else  
    {
		if(hl_get_random(0,gBuf) == 0)  
			memcpy(&outbuf[9],gBuf,16);
		else {
			gResponseStatus = CE_COMERR;
			memset(&outbuf[9],0,16);
			gSBufBytes = 25;
			goto label_send;
		}
	}
label_send:
    gResponseStatus = CE_OK;
    memcpy(FM_Module_NO,&outbuf[3],6);
    
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = gResponseStatus;
    *outbytes = 25;
}

void ShMetroRecord(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes)
{
UBYTE	hh_ReadRecordMode;
UBYTE	i;

	if(inbytes != 3) 
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
    if(gStatus != TRANS_STATUS){
        gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
     hh_ReadRecordMode = inbuf[2];

	if(tocken_read(5,&hh_SaleBuf[0])!=0)
	{
		gResponseStatus=CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(tocken_read(6,&hh_SaleBuf[4])!=0)
	{
		gResponseStatus=CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(tocken_read(7,&hh_SaleBuf[8])!=0)
	{
		gResponseStatus=CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(tocken_read(10,&hh_Entry[0])!=0)
	{
		gResponseStatus=CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(tocken_read(11,&hh_Entry[4])!=0)
	{
		gResponseStatus=CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(tocken_read(12,&hh_Exit[0])!=0)
	{
		gResponseStatus=CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(tocken_read(13,&hh_Exit[4])!=0)
	{
		gResponseStatus=CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(tocken_read(14,&hh_Exit[8])!=0)
	{
		gResponseStatus=CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	
	
	gResponseStatus = CE_OK;
	if(hh_ReadRecordMode==0x01)
	{//01 = 售票记录
		for(i=0;i<12;i++)
			outbuf[3+i] = hh_SaleBuf[i];
     
		*outbytes = 15;
		goto label_send;
	}
	if(hh_ReadRecordMode==0x02)
	{//02 = 进站记录
		for(i=0;i<8;i++)
			outbuf[3+i] = hh_Entry[i];
     
		*outbytes = 11;
		goto label_send;
	}
	if(hh_ReadRecordMode==0x03)
	{//03 = 售票记录 + 进站记录
		for(i=0;i<12;i++)
			outbuf[3+i] = hh_SaleBuf[i];
		for(i=0;i<8;i++)
			outbuf[15+i] = hh_Entry[i];
		*outbytes = 23;
		goto label_send;
	}
	if(hh_ReadRecordMode==0x04)
	{//04 = 出站记录
		for(i=0;i<12;i++)
			outbuf[3+i] = hh_Exit[i];
     
		*outbytes = 15;
		goto label_send;
	}
	if(hh_ReadRecordMode==0x05)
	{//05 = 出站记录 + 售票记录
		for(i=0;i<12;i++)
			outbuf[3+i] = hh_SaleBuf[i];
		for(i=0;i<12;i++)
			outbuf[15+i] = hh_Exit[i];
		*outbytes = 27;
		goto label_send;
	}
	if(hh_ReadRecordMode==0x06)
	{//06 = 进站记录 + 出站记录
		for(i=0;i<8;i++)
			outbuf[3+i] = hh_Entry[i];
		for(i=0;i<12;i++)
			outbuf[11+i] = hh_Exit[i];
     
		*outbytes = 23;
		goto label_send;
	}
	if(hh_ReadRecordMode==0x07)
	{//07 = 三条记录
		for(i=0;i<12;i++)
			outbuf[3+i] = hh_SaleBuf[i];
		for(i=0;i<8;i++)
			outbuf[15+i] = hh_Entry[i];
		for(i=0;i<12;i++)
			outbuf[23+i] = hh_Exit[i];
     
		*outbytes = 35;
		goto label_send;
	}
	
label_send:
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = gResponseStatus;
	
	return;
}

void shCardCreditInit(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes)
{

	if(inbytes != 6) 	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

	if(gStatus != TRANS_STATUS)  
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
    //     
    *((char *)&gCardThisTransValue+3) = inbuf[5];
    *((char *)&gCardThisTransValue+2) = inbuf[4];
    *((char *)&gCardThisTransValue+1) = inbuf[3];
    *((char *)&gCardThisTransValue+0) = inbuf[2];
    if(trans_credit_ini() != 0) {
    	gStatus = IDLE_STATUS;
    	//gResponseStatus = CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gResponseStatus = CE_OK;	    
	memcpy(&gSBuf[3],gBuf,16);  	    
	gResponseStatus = CE_OK;
	gSBufBytes = 19;
	
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = gResponseStatus;
	memcpy(&outbuf[3], &gSBuf[3], gSBufBytes);
	*outbytes = gSBufBytes;
	return ;
}

void shCardCredit(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes)
{

	if(inbytes != 13) 	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

	if(gStatus != TRANS_STATUS) 
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
    //     
    memcpy(gTransTime,&inbuf[2],7);
    if(check_time_valid() != 0) 	{
		gStatus=IDLE_STATUS; 
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

    memcpy(gMAC,&inbuf[9],4);
	gStatus = IDLE_STATUS;
	gStatusNot = ~gStatus;
	gGetCardInfoStep = 0xff;
    if(trans_credit() != 0)  {
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

    //record
    gBuf[0]=*((char *)&gPosTransCnt+1);
    gBuf[1]=*((char *)&gPosTransCnt+2);
    gBuf[2]=*((char *)&gPosTransCnt+3);
    memcpy(&gBuf[3],gThisCardSnr,4);
    gBuf[7] = *((char *)&gCardCnt+0);
    gBuf[8] = *((char *)&gCardCnt+1);
    gBuf[9] = *((char *)&gCardThisRemain+2);
    gBuf[10] = *((char *)&gCardThisRemain+1);
    gBuf[11] = *((char *)&gCardThisRemain+0);
    gBuf[12] = gTAC[0];
    gBuf[13] = gTAC[1];
    gBuf[14] = gTAC[2];
    gBuf[15] = gTAC[3];  
    
    save_trans_record(gBuf);
  
	gResponseStatus = CE_OK;
	gStatus = IDLE_STATUS;
	gStatusNot = ~gStatus;
	memcpy(&gSBuf[3],gTAC,4);
	gSBufBytes = 7;
	
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = gResponseStatus;
	memcpy(&outbuf[3], &gSBuf[3], gSBufBytes);
	*outbytes = gSBufBytes;
	
	return ;
}

void shCardUpdateEx(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes)
{

	return ;
}

void shCardUnlock(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes)
{

	if(inbytes != 6) 	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

	if(gStatus != TRANS_STATUS){
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return ;
	}
	memcpy(gMAC,&gBuf[2],4);
  
	if (trans_cpu_app_unlock() != 0)  {
		shmt_bom_func_err(outbuf,outbytes);
		return ;
	}
  
	gResponseStatus = CE_OK;
	gStatus = IDLE_STATUS;
	gStatusNot = ~gStatus;
	gSBufBytes = 3;
	
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = gResponseStatus;
	*outbytes = gSBufBytes;
	
	return ;
}

void shCardGetRan(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes)
{
	int ret;
	UWORD tint;
	tint = inbuf[2];
	
	ret=Transport_GetFiles15(outbuf);
	if(ret<0)
	{
		printf("\nTransport_GetFiles15_error");
	}

	if(inbytes != 3) 	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

	if(gStatus != TRANS_STATUS){
       gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return ;
	}
	if(trans_cpu_get_random() != 0)  {
		shmt_bom_func_err(outbuf,outbytes);
		return ;
	}
  
	gResponseStatus = CE_OK;
	memcpy(&gSBuf[3],gRandom,4);
	gSBufBytes = 7;
	
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = gResponseStatus;
	memcpy(&outbuf[3], &gSBuf[3], gSBufBytes);
	*outbytes = gSBufBytes;
	
	return ;
}

void shCardExpiredate1(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes)
{

	if(inbytes != 10)	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

	if(gStatus != TRANS_STATUS){
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

	memcpy(gCardNewExpiredDate,&inbuf[2],4);
	memcpy(gMAC,&inbuf[6],4);
  
	if(trans_expiredate1() != 0)  {
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	
	gResponseStatus = CE_OK;
	gStatus = IDLE_STATUS;
	gStatusNot = ~gStatus;
	gSBufBytes = 3;
	
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = gResponseStatus;
	*outbytes = gSBufBytes;
	
	return ;
}

void shCardForbid(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes)
{

	if(inbytes != 6) 	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

	if(gStatus != TRANS_STATUS){
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	memcpy(gMAC, &inbuf[2], 4);
  
	if(trans_forbid_mifare() != 0) {
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}	   
  	   	
	gResponseStatus = CE_OK;
	gStatus = IDLE_STATUS;
	gStatusNot = ~gStatus;
	gSBufBytes = 3;
	
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = gResponseStatus;
	*outbytes = gSBufBytes;
	
	return ;
}

void shCardLockcard1(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes)
{

	if(inbytes != 8) 	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

	if(gStatus != TRANS_STATUS){
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	memcpy(gMAC,&inbuf[2],6);//4MAC+locktype+rfu     
  
	//if(trans_lock(gMAC[4]) != 0) goto label_status_report;
  
	if(trans_cpu_lock(gMAC[4]) != 0)  {
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}	   
	
	gResponseStatus = CE_OK;
	gStatus = IDLE_STATUS;
	gStatusNot = ~gStatus;
	gSBufBytes = 3;
	
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = gResponseStatus;
	*outbytes = gSBufBytes;
	
	return ;
}

void shCardCalcmac(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes)
{

    if(inbytes != 6) 	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

    
    if((gStatus != IDLE_STATUS) && (gStatus != TRANS_STATUS)) {
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
    
    if(hl_cal_mac(0x0d, &inbuf[2],4,gBuf) == 0) 
		memcpy(&gSBuf[3],gBuf,4);
	else {
		gStatus = IDLE_STATUS;
		gResponseStatus = CE_MACERR;
		memset(&gSBuf[3],0,4);
		gSBufBytes = 7;
		goto label_send;  	    
	}      
    gResponseStatus = CE_OK;
    gSBufBytes = 7;
label_send:
	
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = gResponseStatus;
	memcpy(&outbuf[3], &gSBuf[3], gSBufBytes);
	*outbytes = gSBufBytes;
	
	return ;
}


void shCardAddvalue1(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes)
{
ulong  tlong1;
ulong  tlong2;

    if(inbytes != 51) 	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

    if(gStatus != TRANS_STATUS) {
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
    //     
    memcpy(&gTransTime[0], &inbuf[2], 7);
    if(check_time_valid() != 0) {
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
    *((char *)&gPosTransCnt+0) = 0;
    *((char *)&gPosTransCnt+1) = inbuf[9];
    *((char *)&gPosTransCnt+2) = inbuf[10];
    *((char *)&gPosTransCnt+3) = inbuf[11];
    
    *((char *)&gCardThisTransValue+3) = 0; 
    *((char *)&gCardThisTransValue+2) = inbuf[14];   
    *((char *)&gCardThisTransValue+1) = inbuf[13];   
    *((char *)&gCardThisTransValue+0) = inbuf[12];   

    memcpy(gM1AddValKey,&inbuf[15],16);
    memcpy(gM1AddValBlock,&inbuf[31],16);
    memcpy(gAddValMAC,&inbuf[47],4);
     
    if((ulong)gCardLastRemain>(ulong)0x7fffffff)
    {
    	tlong1=~((ulong)gCardLastRemain)+1;
    	if((ulong)tlong1<(ulong)gCardThisTransValue)
    	{
    		tlong1=gCardThisTransValue-tlong1;
    	  //if((ulong)tlong1>100000) 	goto label_para_err;
    	}
    }
    else
    {
    	tlong1=(ulong)gCardLastRemain+(ulong)gCardThisTransValue;
    	//if((ulong)tlong1>100000)	goto label_para_err;
    }

	gStatus = IDLE_STATUS;
	gStatusNot = ~gStatus;
	gGetCardInfoStep = 0xff;
    if(gBackTransCntNot != ~gBackTransCnt)
    get_tongji();
    if(gValueTotalNot != ~gValueTotal)
    get_tongji();
    if(gNumTotalNot != ~gNumTotal)
    get_tongji();
    
//    tlong1=timestr2long(&gTransTime[1]);
//    if(gShouquanTime<tlong1)
//    {	
//    	gResponseStatus =CE_AUTHFAIL;
//    	goto label_status_report; 
//    }
//    tlong2=gValueTotal+gCardThisTransValue;
//    if(gShouquanSum<tlong2)
//    {	
//    	gResponseStatus =CE_AUTHFAIL;
//    	goto label_status_report; 
//    }

     if(gCardStatus != 0x02){
        gResponseStatus = CE_CARDSTATUSERROR;
 		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
   
     if((long)gCardThisTransValue < 0) {
        gResponseStatus = CE_BADPARAM;
 		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
    
    memset(gBuf,0,100);
    if(trans_add() != 0) {
    	gStatus = IDLE_STATUS;
		gSBuf[2] = gResponseStatus; 
		gSBuf[3] = gAddValMAC[0];
		memset(&gSBuf[4],0,11);
		gSBuf[11] = gAddValMAC[1];//MAC
		gSBuf[12] = gAddValMAC[2];
		gSBuf[13] = gAddValMAC[3];
		gSBuf[14] = gAddValMAC[4];
		gSBufBytes = 15;
		goto label_send;
	}
    gResponseStatus = CE_OK;
    gSBuf[3] = gAddValMAC[0];
    gSBuf[4] = *((char *)&gCardThisRemain+0);//3
    gSBuf[5] = *((char *)&gCardThisRemain+1);//2
    gSBuf[6] = *((char *)&gCardThisRemain+2);//1
    gSBuf[7] = gTAC[0];//TAC
    gSBuf[8] = gTAC[1];
    gSBuf[9] = gTAC[2];
    gSBuf[10] = gTAC[3];
      
    gSBuf[11] = gAddValMAC[1];//MAC
    gSBuf[12] = gAddValMAC[2];
    gSBuf[13] = gAddValMAC[3];
    gSBuf[14] = gAddValMAC[4];
	gStatus = IDLE_STATUS;
	gStatusNot = ~gStatus;     
	gSBufBytes = 15;
          
label_send:
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = gResponseStatus;
	
	memcpy(&outbuf[3], &gSBuf[3], gSBufBytes);
	*outbytes = gSBufBytes;
	
    gValueTotal+=gCardThisTransValue;
    gNumTotal++;
    gBackTransCnt = gPosTransCnt;
    gBackTransCntNot = ~gBackTransCnt;
    gValueTotalNot = ~gValueTotal;
    gNumTotalNot = ~gNumTotal;
    wri_tongji();
    if(gShouquanSum>gValueTotal)  tlong2=gShouquanSum-gValueTotal;
    else  tlong2=0;
    
    if(gShouquanTime>tlong1) tlong1=gShouquanTime-tlong1;
    else  tlong1=0;
       
    //record
    gBuf[0]=*((char *)&gPosTransCnt+1);
    gBuf[1]=*((char *)&gPosTransCnt+2);
    gBuf[2]=*((char *)&gPosTransCnt+3);
    memcpy(&gBuf[3],gThisCardSnr,4);
    gBuf[7] = *((char *)&gCardCnt);
    gBuf[8] = *((char *)&gCardCnt+1);
    gBuf[9] = *((char *)&gCardThisRemain+2);
    gBuf[10] = *((char *)&gCardThisRemain+1);
    gBuf[11] = *((char *)&gCardThisRemain+0);
    gBuf[12] = gAddValMAC[1];
    gBuf[13] = gAddValMAC[2];
    gBuf[14] = gAddValMAC[3];
    gBuf[15] = gAddValMAC[4];  
    
    save_trans_record(gBuf);
    
	mcml_pwr_off();	
	return ;
}

void shCardReadrecord1(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes)
{
uchar LRC;
uchar i;
UWORD tint;
int ret;

tint = inbuf[2];
	ret=Transport_GetFiles15(outbuf);

	if(ret<0)
	{
		printf("\nTransport_GetFile15_error");
	}


	printf("\nzdd-61-in\n");
	if(inbytes != 4) 	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

    if((gStatus != TRANS_STATUS) && (gStatus != IDLE_STATUS)) {
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
  	printf("\nzdd-61-status-right\n");
      gRelativePtr=0;
      if((uchar)(inbuf[3]&0x01)==(uchar)0x01)      {gRelativePtr=0;  goto label_relativeptr_lookup_ok;}
      if((uchar)(inbuf[3]&0x02)==(uchar)0x02)      {gRelativePtr=1;  goto label_relativeptr_lookup_ok;}
      if((uchar)(inbuf[3]&0x04)==(uchar)0x04)      {gRelativePtr=2;  goto label_relativeptr_lookup_ok;}
      if((uchar)(inbuf[3]&0x08)==(uchar)0x08)      {gRelativePtr=3;  goto label_relativeptr_lookup_ok;}
      if((uchar)(inbuf[3]&0x10)==(uchar)0x10)      {gRelativePtr=4;  goto label_relativeptr_lookup_ok;}
      if((uchar)(inbuf[3]&0x20)==(uchar)0x20)      {gRelativePtr=5;  goto label_relativeptr_lookup_ok;}
      if((uchar)(inbuf[3]&0x40)==(uchar)0x40)      {gRelativePtr=6;  goto label_relativeptr_lookup_ok;}
      if((uchar)(inbuf[3]&0x80)==(uchar)0x80)      {gRelativePtr=7;  goto label_relativeptr_lookup_ok;}
      if((uchar)(inbuf[2]&0x01)==(uchar)0x01)      {gRelativePtr=8;  goto label_relativeptr_lookup_ok;}
      if((uchar)(inbuf[2]&0x02)==(uchar)0x02)      {gRelativePtr=9;  goto label_relativeptr_lookup_ok;}
      if((uchar)(inbuf[2]&0x04)==(uchar)0x04)      {gRelativePtr=10; goto label_relativeptr_lookup_ok;}
      if((uchar)(inbuf[2]&0x08)==(uchar)0x08)      {gRelativePtr=11; goto label_relativeptr_lookup_ok;}  
      if((uchar)(inbuf[2]&0x10)==(uchar)0x10)      {gRelativePtr=12; goto label_relativeptr_lookup_ok;}  
      if((uchar)(inbuf[2]&0x20)==(uchar)0x20)      {gRelativePtr=13; goto label_relativeptr_lookup_ok;}
      if((uchar)(inbuf[2]&0x40)==(uchar)0x40)      {gRelativePtr=14; goto label_relativeptr_lookup_ok;}
      if((uchar)(inbuf[2]&0x80)==(uchar)0x80)      {gRelativePtr=15; goto label_relativeptr_lookup_ok;}
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}      
		
label_relativeptr_lookup_ok:
	printf("\nzdd-61-inlabel-right\n");
	if(trans_read_cpu_record(gRelativePtr) != 0) 	{
		gStatus = IDLE_STATUS;
		printf("\nzdd-61-trans_read_cpu_record_err\n");
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

		printf("\nzdd-61-err\n");

	gResponseStatus = CE_OK;
	gSBuf[3] = gRelativePtr;
	if(gRelativePtr > 14)  {
		memcpy(gCardRecord,gBuf,16);
		//block lrc check
		LRC = 0xaa;
		for(i=0;i<16;i++) LRC ^= gCardRecord[i];
		if(LRC != 0) {
			gResponseStatus = CE_INVALIDCARD;
			shmt_bom_func_err(outbuf,outbytes);
			return ;
			//gSBuf[3] = gRelativePtr | 0x80;
			//gSBufBytes = 3;
			//goto label_send;
		}
		memcpy(&gSBuf[4],gBuf,16);
		gSBufBytes = 20;
		goto label_send;
	}
	  
	if(gRelativePtr == 14) {            
		//认证码验证
		memcpy(gCardRecord,gBuf,16);
		memcpy(gSBuf,gCardRecord,13);
		//block lrc check
		LRC = 0xaa;
		for(i=0;i<16;i++) LRC ^= gCardRecord[i];
		if(LRC != 0) {
			gResponseStatus = CE_INVALIDCARD;
			shmt_bom_func_err(outbuf,outbytes);
			return ;
        	//gSBuf[3] = gRelativePtr | 0x80;
	   	  	//gSBufBytes = 3;
	   	  	//goto label_send;
	   	}
	   	
		gSBuf[13]=0x20;
		gSBuf[14]=0x00;
		memcpy(&gSBuf[15],gThisCardSnr,4);
		memcpy(&gSBuf[19],"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",13); 
		//if(samType==0)//psam
		{
			if(psam_get_chk(gSBuf,&gSBuf[40])!=0)
			{
				//gResponseStatus = 
				gStatus = IDLE_STATUS;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			}
			if(memcmp(&gSBuf[40],&gCardRecord[13],2)!=0)
			{
				gResponseStatus = CE_OK;
	            gSBuf[3] = gRelativePtr | 0x40;
	            gSBufBytes = 4;
	            goto label_send;
		   }
      }
      
	      gResponseStatus = CE_OK;
	      gSBuf[3] = gRelativePtr;
	      memcpy(&gSBuf[4],gCardRecord,16);
	      gSBufBytes = 20;
	      goto label_send;	            
	  }        
	            
	if(gRelativePtr < 4)  {
		memcpy(&gSBuf[4],gBuf,23);
		gSBufBytes = 27;
		goto label_send;
	}
	if(memcmp(gCityCode,"\x20\x00",2) != 0) {//非上海卡读历史记录返回0018文件，23字节
		memcpy(&gSBuf[4],gBuf,23);
		gSBufBytes = 27;
		goto label_send;		
	}
	if(!gM1ConsumptionStatus) {//00为M1消费未关闭
		memcpy(&gSBuf[4],gBuf,16);
		gSBufBytes = 20;		
	}
	else {                    
		memcpy(&gSBuf[4],gBuf,23);
		gSBufBytes = 27;
	}	

label_send:
	
  	outbuf[0] = oldcmd;
  	outbuf[1] = oldtimes;
  	outbuf[2] = CE_OK;
  	
  	memcpy(&outbuf[3], &gSBuf[3], gSBufBytes);
  	*outbytes = gSBufBytes;
	return ;
}

void shCardCreditInit1(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes)
{

    if(inbytes != 2)	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

    if(gStatus != TRANS_STATUS){
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
    
    memcpy(&gSBuf[3],gCardIssueNo,2);
    memcpy(&gSBuf[5],gCardMac,1);
    
    if(trans_credit_ini1() != 0)	{
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

    outbuf[0] = oldcmd;
    outbuf[1] = oldtimes;
    outbuf[2] = CE_OK;
    memcpy(&outbuf[3],&gB1[8],2);//gCardIssueNo
    memcpy(&outbuf[5],&gB1[10],1);//gCardMac
    
    //M1 block 1 or 8
    if (gCardStatus == 0x1)   //卡片未启用
	{
		memcpy(&outbuf[6],gB1,16);
	}
    else memcpy(&outbuf[6],gB8,16);
    
    memcpy(&outbuf[22],gLineProtectionMAC,4);
    
    *outbytes = 26;
	return ;
}

void shCardEnableCard2(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes)
{

	if(inbytes != 10)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
    if(gStatus != TRANS_STATUS){
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	memcpy(gTransTime,&inbuf[2],4);
	memcpy(&gTransTime[4],"\x11\x22\x33",3);
	if(check_time_valid() != 0) 
	{
		gStatus=IDLE_STATUS; 
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	memcpy(CntCardNewStartDate,&inbuf[2],4);
	memcpy(gMAC,&inbuf[6],4);
  
	if(trans_sale_cpu() != 0)
	{
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
       
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	*outbytes = 3;
	gStatus=IDLE_STATUS;

	return ;
}

void shCardEnableCard1(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes)
{
UBYTE	i;
ulong  tlong1;
ulong  tlong2;

	if(inbytes != 51)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
    if(gStatus != TRANS_STATUS){
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
    for(i=0;i<7;i++) gTransTime[i] = inbuf[i+2];
    if(check_time_valid() != 0) 
	{
		gStatus=IDLE_STATUS; 
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
    *((char *)&gPosTransCnt+0) = 0;
    *((char *)&gPosTransCnt+1) = inbuf[9];
    *((char *)&gPosTransCnt+2) = inbuf[10];
    *((char *)&gPosTransCnt+3) = inbuf[11];
    
    *((char *)&gCardThisTransValue+3) = 0; 
    *((char *)&gCardThisTransValue+2) = inbuf[14];   
    *((char *)&gCardThisTransValue+1) = inbuf[13];   
    *((char *)&gCardThisTransValue+0) = inbuf[12];   

    memcpy(gM1SaleValKey, &inbuf[15],16);
    memcpy(gM1SaleValBlock, &inbuf[31],16);
    memcpy(gSaleValMAC, &inbuf[47],4);
     
    if((ulong)gCardLastRemain>(ulong)0x7fffffff)
    {
    	tlong1=~((ulong)gCardLastRemain)+1;
    	if((ulong)tlong1<(ulong)gCardThisTransValue)
    	{
    		tlong1=gCardThisTransValue-tlong1;
    	  //if((ulong)tlong1>100000) 	goto label_para_err;
    	}
    }
    else
    {
    	tlong1=(ulong)gCardLastRemain+(ulong)gCardThisTransValue;
    	//if((ulong)tlong1>100000)	goto label_para_err;
    }

    //enter_idle();
    if(gBackTransCntNot != ~gBackTransCnt)
    get_tongji();
    if(gValueTotalNot != ~gValueTotal)
    get_tongji();
    if(gNumTotalNot != ~gNumTotal)
    get_tongji();
    
//    tlong1=timestr2long(&gTransTime[1]);
//    if(gShouquanTime<tlong1)
//    {	
//    	gResponseStatus =CE_AUTHFAIL;
//    	goto label_status_report; 
//    }
//    tlong2=gValueTotal+gCardThisTransValue;
//    if(gShouquanSum<tlong2)
//    {	
//    	gResponseStatus =CE_AUTHFAIL;
//    	goto label_status_report; 
//    }
    if(trans_sale() != 0) {
		gResponseStatus = CE_OK;
		gSBuf[3] = gSaleValMAC[0];
		memset(&gSBuf[4],0,11);
		gSBuf[11] = gSaleValMAC[1];//MAC
		gSBuf[12] = gSaleValMAC[2];
		gSBuf[13] = gSaleValMAC[3];
		gSBuf[14] = gSaleValMAC[4];
		gSBufBytes = 15;
		goto label_send;
	}
    gResponseStatus = CE_OK;
    gSBuf[3] = gSaleValMAC[0];
    gSBuf[4] = *((char *)&gCardThisRemain+0);
    gSBuf[5] = *((char *)&gCardThisRemain+1);
    gSBuf[6] = *((char *)&gCardThisRemain+2);
    gSBuf[7] = gTAC[1];//TAC
    gSBuf[8] = gTAC[2];
    gSBuf[9] = gTAC[3];
    gSBuf[10] = gTAC[4];
      
    gSBuf[11] = gSaleValMAC[1];//MAC
    gSBuf[12] = gSaleValMAC[2];
    gSBuf[13] = gSaleValMAC[3];
    gSBuf[14] = gSaleValMAC[4];
          
    gValueTotal+=gCardThisTransValue;
    gNumTotal++;
    gBackTransCnt = gPosTransCnt;
    gBackTransCntNot = ~gBackTransCnt;
    gValueTotalNot = ~gValueTotal;
    gNumTotalNot = ~gNumTotal;
    wri_tongji();
    if(gShouquanSum>gValueTotal)  tlong2=gShouquanSum-gValueTotal;
    else  tlong2=0;
    
    if(gShouquanTime>tlong1) tlong1=gShouquanTime-tlong1;
    else  tlong1=0;

      
    //record
    gBuf[0]=*((char *)&gPosTransCnt+1);
    gBuf[1]=*((char *)&gPosTransCnt+2);
    gBuf[2]=*((char *)&gPosTransCnt+3);
    memcpy(&gBuf[3],gThisCardSnr,4);
    gBuf[7] = *((char *)&gCardCnt);
    gBuf[8] = *((char *)&gCardCnt+1);
    gBuf[9] = *((char *)&gCardThisRemain+2);
    gBuf[10] = *((char *)&gCardThisRemain+1);
    gBuf[11] = *((char *)&gCardThisRemain+0);
    gBuf[12] = gSaleValMAC[1];
    gBuf[13] = gSaleValMAC[2];
    gBuf[14] = gSaleValMAC[3];
    gBuf[15] = gSaleValMAC[4];  
    
    save_trans_record(gBuf);

label_send:   
	gStatus = IDLE_STATUS;
	gStatusNot = ~gStatus;     

	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	*outbytes = 15;
	gStatus=IDLE_STATUS;
	memcpy(&outbuf[3], &gSBuf[3], 12);

	return ;
}

/************************************************************************************************
函数：
功能：ODA进站交易
************************************************************************************************/
void shODAEntry(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	int ret;
	UBYTE buf[20], obuf[256];

#ifdef _DEBUG_BOM_
	debug_printf("\nshODAEntry...");
#endif
	
	gRespStep = 0;
	if(gMode != ENTRY_MODE)
	{
		gResponseStatus = CE_BADCOMMAND;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(inbytes!=13)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = WRITE_STATUS;
	memcpy(gTransTime,&inbuf[2],7);
	if(check_time_valid() != 0) 
	{
		gStatus=IDLE_STATUS; 
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	*((char *)&gPosTransCnt+LONG_HIGH3) = 0;
 	*((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[9];
  	*((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[10];
  	*((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[11];
 	if(inbuf[12]&0x18)
	{
		gStationTimeOverRideFlag=inbuf[12];
	}
	else
	{
		gStationTimeOverRideFlag=0;
	}
	ret = oda_zone_ticket_entry(&inbuf[2], obuf);
#ifdef _DEBUG_BOM_
	printf("\noda_zone_ticket_entry...ret=%d\n", ret);
#endif
	if(ret > 0)
	{
		mcml_pwr_off();
		gStatus=IDLE_STATUS;
		gResponseStatus = CE_INVALIDCARD;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	else if(ret < 0)
	{
		mcml_pwr_off();
		gStatus=IDLE_STATUS;
		if(ret == -308)
		{
			gResponseStatus = CE_CARDREMOVED;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
		else
		{
			gResponseStatus = CE_WRITEERROR;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	memcpy(&outbuf[3],&obuf[1],96);
	*outbytes = 99; 
	gRespStep = 1;
	gStatus=IDLE_STATUS;
	mcml_pwr_off();

	return;
}
/************************************************************************************************
函数：ShCardExit
功能：ODA出站交易
************************************************************************************************/
void shODAExit(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE transtype;
	UBYTE buf[20], obuf[256];
	int ret;

#ifdef _DEBUG_BOM_
debug_printf("\nshODAExit...");
#endif

	gRespStep = 0;
	if(gMode != EXIT_MODE)
	{
		gResponseStatus = CE_BADCOMMAND;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(inbytes!=18)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = WRITE_STATUS;
	memcpy(gTransTime,&inbuf[2],7);
	if(check_time_valid() != 0) 
	{
		gStatus=IDLE_STATUS; 
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	*((char *)&gPosTransCnt+LONG_HIGH3) = 0;							  			
	*((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[9];
	*((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[10];
	*((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[11];
	*((char *)&gCardThisTransValue+LONG_HIGH3) = 0;
	*((char *)&gCardThisTransValue+LONG_HIGH2) = inbuf[14];
	*((char *)&gCardThisTransValue+LONG_HIGH1) = inbuf[13];
	*((char *)&gCardThisTransValue+LONG_HIGH0) = inbuf[12];
	
	ret =  oda_zone_ticket_exit(&inbuf[2],obuf);
#ifdef _DEBUG_BOM_
	printf("\noda_zone_ticket_exit...ret=%d\n", ret);
#endif
	if(ret > 0)
	{
		mcml_pwr_off();
		gStatus=IDLE_STATUS;
		gResponseStatus = CE_INVALIDCARD;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	else if(ret < 0)
	{
		mcml_pwr_off();
		gStatus=IDLE_STATUS;
		if(ret == -308)
		{
			gResponseStatus = CE_CARDREMOVED;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
		else{
			gResponseStatus = CE_WRITEERROR;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	memcpy(&outbuf[3],&obuf[1],96);
	*outbytes = 99;
	gStatus=IDLE_STATUS;

	mcml_pwr_off();
	return;
}
void shODAUpdate(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE ret;
	UBYTE buf[20], obuf[256];
	UBYTE odaTac[32], tacBak[32];

#ifdef _DEBUG_BOM_
debug_printf("\nshODAUpdate...");
#endif
	
	gRespStep = 0;
	if(inbytes!=56)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus!=TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = WRITE_STATUS;
	memcpy(gTransTime,&inbuf[2],7);
	if(check_time_valid() != 0) 
	{
		gStatus=IDLE_STATUS; 
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	//
	memset(odaTac, 0x00, sizeof(odaTac));
	memcpy(odaTac, &gODARec1[4], 8);
	memcpy(&odaTac[8], &inbuf[2], 7);
	odaTac[17] = inbuf[13];
	odaTac[18] = inbuf[12];
	if( (gODARec2[0] & 0xC0) == 0x40)
	{
		memcpy(&odaTac[19], &gODARec2[8], 2);
	}else
	{
		memcpy(&odaTac[19], &gODARec2[19], 2);
	}
	memcpy(&odaTac[21], gPosNo, 4);
	memcpy(&odaTac[25], &inbuf[9], 3);
	memcpy(&odaTac[28], bpgSmpsamNo, 4);
	//
	memcpy(tacBak, &inbuf[9], 3);
	memcpy(&tacBak[3], &gODARec1[0], 13);
	
	ret = oda_zone_ticket_update(&inbuf[2], obuf);
#ifdef _DEBUG_BOM_
	printf("\noda_zone_ticket_update...ret=%d\n", ret);
#endif
	SHODATACStep1(odaTac);
	if(ret > 0)
	{
		mcml_pwr_off();
		gStatus=IDLE_STATUS;
		gResponseStatus = CE_INVALIDCARD;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	else if(ret < 0)
	{
		mcml_pwr_off();
		gStatus=IDLE_STATUS;
		if(ret == -308)
		{
			gResponseStatus = CE_CARDREMOVED;
			shmt_bom_func_err(outbuf,outbytes);
			if(SHODATACStep2()!=0)			//取TAC
			{
				memset(bgTaskKeyOutbuf, 0xFF, 4);
			}
			memcpy(gTAC,bgTaskKeyOutbuf,4);	
			memcpy(&tacBak[16], gTAC, 4);
			save_oda_record(tacBak);
			return;
		}
		else
		{
			gResponseStatus = CE_WRITEERROR;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
	}
	if(SHODATACStep2()!=0)			//取TAC
	{
		memset(bgTaskKeyOutbuf, 0xFF, 4);
	}
	memcpy(gTAC,bgTaskKeyOutbuf,4);
	memcpy(&tacBak[16], gTAC, 4);
	save_oda_record(tacBak);
			
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	memcpy(&outbuf[3],&obuf[1],100);
	memcpy(&outbuf[103], gTAC, 4);
	
	*outbytes = 116; 
	gRespStep = 1;
	gStatus=IDLE_STATUS;
	mcml_pwr_off();

	return;
}

void shODAGetProof(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
int ret;
UBYTE obuf[256], buf[100];
	
#ifdef _DEBUG_BOM_
debug_printf("\nshODAGetProof...");
#endif

	if(inbytes != 18)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus != TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus=IDLE_STATUS;
	//
	memset(obuf,0,sizeof(obuf));
	ret = oda_zone_ticket_get_proof(&inbuf[2],obuf);
#ifdef _DEBUG_BOM_
	printf("\noda_zone_ticket_get_proof...ret=%d\n", ret);
#endif
  
  	if(ret > 0)
	{
		gResponseStatus = CE_INVALIDCARD;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	else if(ret < 0)
	{
		if((ret == -311) || (ret == -310))
		{
			gResponseStatus = CE_CARDREMOVED;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
		else{
			gResponseStatus = CE_WRITEERROR;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
	}
	//
	if(get_oda_record(&inbuf[2],buf)!=0)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = obuf[0];
	//memcpy(&outbuf[3],&obuf[1],91);
	memcpy(&outbuf[3],&obuf[1],100);  
	//
	memcpy(&outbuf[103], buf, 4); 
	*outbytes = 116;
	return; 
}

void shODAEnroll(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
int ret, i;
UBYTE obuf[256], counter, buf[300], cnt;
	
#ifdef _DEBUG_BOM_
debug_printf("\nshODAEnroll... select AID again");
#endif

	if(inbytes != 133)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus != TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus=IDLE_STATUS;
	//
	mcml_pwr_off();
	usleep(5000);
	set_card_type(ISO14443A_SH_TYPE);
	for(counter=0;counter<3;counter++)
	{
		memset(buf,0,sizeof(buf));
		ret=mcml_request2(PICC_REQALL,buf); //2014/9/11 10:00:16
		if (ret == 0) 
			break;
	}
	if(counter == 3)
	{
		mcml_pwr_off();	
		#ifdef _DEBUG_user_main_
		debug_printf("\nshODAEnrollXXXXXXX 33\n");
		#endif	
		gResponseStatus = CE_NOCARD;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	ret=mcml_anticoll(gThisCardSnr);
	if(ret!=0)
	{
		#ifdef _GETCARDINFO_CPU_
		debug_printf("\x0d\x0a shODAEnroll mcml_anticoll Err\n");
		#endif
		gResponseStatus = CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gDebugStep = 0x3081;
	ret=mcml_select(gThisCardSnr,&cnt);
	if(ret!=0)
	{
		#ifdef _GETCARDINFO_CPU_
		debug_printf("\x0d\x0a shODAEnroll mcml_select Err\n");
		#endif 
		gResponseStatus = CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	ret = mifpro_ats0(buf);
	if(ret<0)
	{
		#ifdef _GETCARDINFO_CPU_
		debug_printf("shODAEnroll mifpro_ats0 Err\n");
		#endif 
		gResponseStatus = CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	memcpy(buf,"\x00\xa4\x04\x00\x0e",5);
	memcpy(&buf[5],"2PAY.SYS.DDF01",14);
#ifdef _debug_pf_mobile_ticket_process_
	debug_printf("shODAEnroll 1\n");
	for(i=0;i<19;i++) debug_printf(" %02x",buf[i]);
#endif

	ret = mifpro_icmd0(19,buf,outbuf);
	if(ret < 0)
	{
		gResponseStatus = CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}	
	
	if((outbuf[ret-2]!=0x90)||(outbuf[ret-1]!=0x00)) {
		gResponseStatus = CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	
	memset(obuf,0,sizeof(obuf));
	ret = oda_zone_ticket_enroll(&inbuf[2],obuf);
#ifdef _DEBUG_BOM_
	printf("\noda_zone_ticket_enroll...ret=%d\n", ret);
#endif
  
  	if(ret > 0)
	{
		gResponseStatus = CE_INVALIDCARD;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	else if(ret < 0)
	{
		if((ret == -311) || (ret == -310))
		{
			gResponseStatus = CE_CARDREMOVED;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
		else{
			gResponseStatus = CE_WRITEERROR;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
	}
	//
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = obuf[0];
	//memcpy(&outbuf[3],&obuf[1],91);
	memcpy(&outbuf[3],&obuf[1],96);          
	*outbytes = 99;
	return; 
}

void shODADisable(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
int ret;
UBYTE obuf[256];
	
#ifdef _DEBUG_BOM_
debug_printf("\nshODADisable...");
#endif

	if(inbytes != 12)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus != TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus=IDLE_STATUS;
	//
	memset(obuf,0,sizeof(obuf));
	ret =  oda_zone_ticket_disable(&inbuf[2],obuf);
#ifdef _DEBUG_BOM_
	printf("\noda_zone_ticket_disable...ret=%d\n", ret);
#endif
  
  	if(ret > 0)
	{
		gResponseStatus = CE_INVALIDCARD;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	else if(ret < 0)
	{
		if((ret == -311) || (ret == -310))
		{
			gResponseStatus = CE_CARDREMOVED;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
		else{
			gResponseStatus = CE_WRITEERROR;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
	}
	//
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = obuf[0];
	//memcpy(&outbuf[3],&obuf[1],91);
	memcpy(&outbuf[3],&obuf[1],96);          
	*outbytes = 99;
	return; 
}

/************************************************************************************************
函数：shTransportEntry
功能：交通部进站
************************************************************************************************/
void shTransportEntry(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE ret;
	UBYTE buf[32],obuf[256];

#ifdef _DEBUG_BOM_
debug_printf("\nshTransportEntry...");
#endif
	
	gRespStep = 0;
	if(gMode != ENTRY_MODE)
	{
		gResponseStatus = CE_BADCOMMAND;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(inbytes!=13)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
//	if(gStatus!=TRANS_STATUS)
//	{
//		gResponseStatus = CE_STATUSERROR;
//		shmt_bom_func_err(outbuf,outbytes);
//		return;
//	}
	gStatus = WRITE_STATUS;
	memcpy(gTransTime,&inbuf[2],7);
	if(check_time_valid() != 0) 
	{
		gStatus=IDLE_STATUS; 
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	*((char *)&gPosTransCnt+LONG_HIGH3) = 0;
 	*((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[9];
  	*((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[10];
  	*((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[11];
 	if(inbuf[12]&0x18)
	{
		gStationTimeOverRideFlag=inbuf[12];
	}
	else
	{
		gStationTimeOverRideFlag=0;
	}

	ret=sh_transport_entry();
	if(ret != 0)
	{
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	//outbuf[3] = *((char *)&gCardCnt+INT_HIGH);
	//outbuf[4] = *((char *)&gCardCnt+INT_LOW);	
	outbuf[3]	=	1;
	outbuf[4]	=	capp_init[9];
	outbuf[5]	=	capp_init[10];
	memcpy(&outbuf[6],&capp_init[4],2);//脱机交易序号
	memcpy(&outbuf[8],bgTaskKeyOutbuf,4);//PSAM流水号
	outbuf[12] = gTAC[0]; 
	outbuf[14] = gTAC[1];
	outbuf[14] = gTAC[2];
	outbuf[15] = gTAC[3];
	memset(&outbuf[16],0x00,2);
	*outbytes = 18; 
	gRespStep = 1;
	gStatus=IDLE_STATUS;
	
	buf[0]=*((char *)&gPosTransCnt+LONG_HIGH2);
	buf[1]=*((char *)&gPosTransCnt+LONG_HIGH1);
	buf[2]=*((char *)&gPosTransCnt+LONG_HIGH0);
	memcpy(&buf[3],&transport_15_data[10],10);//卡号
	memcpy(&buf[13],&capp_init[4],2);//计数器值
	buf[15]	=	1;
	buf[16]	=	capp_init[9];
	buf[17]	=	capp_init[10];
	memcpy(&buf[18],&capp_init[4],2);//脱机交易序号
	memcpy(&buf[20],bgTaskKeyOutbuf,4);//PSAM流水号
	buf[24]=*((char *)&gCardThisRemain+LONG_HIGH0);
	buf[25]=*((char *)&gCardThisRemain+LONG_HIGH1);
	buf[26]=*((char *)&gCardThisRemain+LONG_HIGH2);
	buf[27] = gTAC[0]; 
	buf[28] = gTAC[1];
	buf[29] = gTAC[2];
	buf[30] = gTAC[3];
	save_transport_record(buf);
	
	
	return;
}

/************************************************************************************************
函数：shTransportExit
功能：交通部出站
************************************************************************************************/
void shTransportExit(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE transtype;
	UBYTE buf[20];
	UBYTE ret;

#ifdef _DEBUG_BOM_
debug_printf("\nshTransportExit...");
#endif

	gRespStep = 0;
	if(gMode != EXIT_MODE)
	{
		gResponseStatus = CE_BADCOMMAND;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(inbytes!=18)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
//	if(gStatus!=TRANS_STATUS)
//	{
//		gResponseStatus = CE_STATUSERROR;
//		shmt_bom_func_err(outbuf,outbytes);
//		return;
//	}
	gStatus = WRITE_STATUS;
	memcpy(gTransTime,&inbuf[2],7);
	if(check_time_valid() != 0) 
	{
		gStatus=IDLE_STATUS; 
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	*((char *)&gPosTransCnt+LONG_HIGH3) = 0;							  			
	*((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[9];
	*((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[10];
	*((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[11];
	*((char *)&gCardThisTransValue+LONG_HIGH3) = 0;
	*((char *)&gCardThisTransValue+LONG_HIGH2) = inbuf[14];
	*((char *)&gCardThisTransValue+LONG_HIGH1) = inbuf[13];
	*((char *)&gCardThisTransValue+LONG_HIGH0) = inbuf[12];
	transtype= inbuf[15];
	if(ReadMode==0)
	{
		if(transtype>2)
		{
			gStatus=IDLE_STATUS;
			gResponseStatus=CE_BADPARAM;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
	}
	else	
	{
		if((transtype>12)||(transtype==2)||(transtype==3)||(transtype==4))
		{
			gStatus=IDLE_STATUS;
			gResponseStatus=CE_BADPARAM;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
	}
	#ifdef _DEBUG_BOM_
			debug_printf("\x0d\x0a entry function :trans_exit");
	#endif

	ret = sh_transport_exit(transtype);

	mcml_pwr_off();

	if(ret!=0)
	{
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3]	=	1;
	outbuf[4]	=	capp_init[9];
	outbuf[5]	=	capp_init[10];
	memcpy(&outbuf[6],&capp_init[4],2);//脱机交易序号
	memcpy(&outbuf[8],bgTaskKeyOutbuf,4);//PSAM流水号
	outbuf[12] = *((char *)&gCardThisRemain+LONG_HIGH0);
	outbuf[13] = *((char *)&gCardThisRemain+LONG_HIGH1);
	outbuf[14] = *((char *)&gCardThisRemain+LONG_HIGH2); 	
	outbuf[15] = gTAC[0];
	outbuf[16] = gTAC[1];
	outbuf[17]= gTAC[2];
	outbuf[18]= gTAC[3];		
	*outbytes = 19;
	gStatus=IDLE_STATUS;
	gRespStep = 2;
	
	buf[0]=*((char *)&gPosTransCnt+LONG_HIGH2);
	buf[1]=*((char *)&gPosTransCnt+LONG_HIGH1);
	buf[2]=*((char *)&gPosTransCnt+LONG_HIGH0);
	memcpy(&buf[3],&transport_15_data[10],10);//卡号
	memcpy(&buf[13],&capp_init[4],2);//计数器值
	buf[15]	=	1;
	buf[16]	=	capp_init[9];
	buf[17]	=	capp_init[10];
	memcpy(&buf[18],&capp_init[4],2);//脱机交易序号
	memcpy(&buf[20],bgTaskKeyOutbuf,4);//PSAM流水号
	buf[24]=*((char *)&gCardThisRemain+LONG_HIGH0);
	buf[25]=*((char *)&gCardThisRemain+LONG_HIGH1);
	buf[26]=*((char *)&gCardThisRemain+LONG_HIGH2);
	buf[27] = gTAC[0]; 
	buf[28] = gTAC[1];
	buf[29] = gTAC[2];
	buf[30] = gTAC[3];
	save_transport_record(buf);

	return;
}

/************************************************************************************************
函数：shTransportUpdate
功能：交通部更新
************************************************************************************************/
void shTransportUpdate(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE ret;
	UBYTE buf[50];

#ifdef _DEBUG_BOM_
debug_printf("\nshTransportUpdate...");
#endif

	if(inbytes!=32)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus != TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}	
	if((inbuf[2]!=0x01)&&(inbuf[2]!=0x02))
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	memcpy(gTransTime,&inbuf[3],7);
	if(check_time_valid() != 0) 
	{		
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	*((char *)&gPosTransCnt+LONG_HIGH3) = 0;							  			
  *((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[10];
  *((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[11];
  *((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[12];
	*((char *)&gCardThisTransValue+LONG_HIGH3) = 0;
	*((char *)&gCardThisTransValue+LONG_HIGH2) = inbuf[15];
	*((char *)&gCardThisTransValue+LONG_HIGH1) = inbuf[14];
	*((char *)&gCardThisTransValue+LONG_HIGH0) = inbuf[13];
	gStatus=IDLE_STATUS; 

	ret=sh_transport_update(inbuf[2],&inbuf[16]);
	mcml_pwr_off();
	if(ret)
	{
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3]	=	1;
	outbuf[4]	=	capp_init[9];
	outbuf[5]	=	capp_init[10];
	memcpy(&outbuf[6],&capp_init[4],2);//脱机交易序号
	memcpy(&outbuf[8],bgTaskKeyOutbuf,4);//PSAM流水号
	outbuf[12] = *((char *)&gCardThisRemain+LONG_HIGH0);
	outbuf[13] = *((char *)&gCardThisRemain+LONG_HIGH1);
	outbuf[14] = *((char *)&gCardThisRemain+LONG_HIGH2); 	
	outbuf[15] = gTAC[0];
	outbuf[16] = gTAC[1];
	outbuf[17]= gTAC[2];
	outbuf[18]= gTAC[3];		
	*outbytes = 19;

	buf[0]=*((char *)&gPosTransCnt+LONG_HIGH2);
	buf[1]=*((char *)&gPosTransCnt+LONG_HIGH1);
	buf[2]=*((char *)&gPosTransCnt+LONG_HIGH0);
	memcpy(&buf[3],&transport_15_data[10],10);//卡号
	memcpy(&buf[13],&capp_init[4],2);//计数器值
	buf[15]	=	1;
	buf[16]	=	capp_init[9];
	buf[17]	=	capp_init[10];
	memcpy(&buf[18],&capp_init[4],2);//脱机交易序号
	memcpy(&buf[20],bgTaskKeyOutbuf,4);//PSAM流水号
	buf[24]=*((char *)&gCardThisRemain+LONG_HIGH0);
	buf[25]=*((char *)&gCardThisRemain+LONG_HIGH1);
	buf[26]=*((char *)&gCardThisRemain+LONG_HIGH2);
	buf[27] = gTAC[0]; 
	buf[28] = gTAC[1];
	buf[29] = gTAC[2];
	buf[30] = gTAC[3];
	save_transport_record(buf);
	return;
}


/************************************************************************************************
函数：shTransportGetProof
功能：交通部 根据多个参数取TAC
************************************************************************************************/
void shTransportGetProof(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE buf[40];

#ifdef _DEBUG_BOM_
debug_printf("\nshTransportGetProof...");
#endif

	if(inbytes!=17)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if((gStatus != IDLE_STATUS)&&(gStatus != TRANS_STATUS))
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(get_transport_record(&inbuf[2],buf)!=0)
	{
		gResponseStatus = CE_NORECORD;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	memcpy(&outbuf[3],&buf[15],16);
	*outbytes =19;
	return; 
}


/************************************************************************************************
函数：shTransportRecord
功能：上海交通部读卡上交易记录及校验B28
************************************************************************************************/
void shTransportRecord(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE buf[20],out[300],i;
	int ret;
	UWORD tint;

#ifdef _DEBUG_BOM_
	debug_printf("\nshTransportRecord...");
#endif

	if(inbytes!=3)
	{
		gResponseStatus = CE_BADPARAM;
		goto label_err;
	}
/*	if((gStatus!=IDLE_STATUS)&&(gStatus!=TRANS_STATUS))
	{
		gResponseStatus = CE_STATUSERROR;
		goto label_err;
	}
	gDebugStep = 0x01;
	if((gStatus==IDLE_STATUS) || (gStatus==TRANS_STATUS))
	{
		#ifdef DEBUG_PRINT
			printf("      1x1x1x1x1x!\n");
		#endif
		mcml_pwr_off();
		set_card_type(ISO14443A_SH_TYPE);
		i = 0;
		while(i<8)
 		{
 			if(mcml_request(PICC_REQSTD,buf)==0)
 			{
 				break;
 			}
 			i++;
 		}
 		gDebugStep = 0x02;
 		if(i>=8)
 		{
 			gResponseStatus = CE_CARDREMOVED;
			shmt_bom_func_err(outbuf,outbytes);
			return;
 		}
		gThisCardSnr[6] = buf[0];
		gThisCardSnr[7] = buf[1];
		gDebugStep = 0x03;
		ret=mcml_anticoll(gThisCardSnr);
		if(ret!=0)
		{
			gResponseStatus = CE_CARDREMOVED;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
		gDebugStep = 0x04;
		ret=mcml_select(gThisCardSnr,&i);
		if(ret!=0)
		{
			gResponseStatus = CE_CARDREMOVED;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
		gDebugStep = 0x05;
		gThisCardSnr[5] = i;
		ret = mifpro_ats0(buf);
		if(ret<0)
		{
			gResponseStatus = CE_CARDREMOVED;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
		gDebugStep = 0x06;
		memcpy(buf,"\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01\x00",15);
		ret = mifpro_icmd0(15,buf,out);
		if(ret<0)
		{
			gResponseStatus=CE_READERROR;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
		gDebugStep = 0x07;
		if((out[ret-2]!=0x90)||(out[ret-1]!=0x00))
		{
			goto label_invalid;
		}
		for(tint=0;tint<ret-2;tint++)
		{
			if(out[tint]!=0x9f)
				continue;
			tint++;
			if(out[tint]!=0x0c)
				continue;
			tint++;
			//if(out[tint]!=0x1e)
			//	goto label_invalid;
			tint++;
			if((ret-tint-2)<0x1e)
				goto label_invalid;
			break;
		}
		//if(tint>=(ret-2))
		//	goto label_invalid;
		gCardStatus = out[tint+8];
		memcpy(gAppSn,&out[tint+10],10);
		if(gCardStatus&0x80)
		{
			gCardStatus = M1LOCKED;
		}
		if(memcmp(gAppSn,"\x20\x00",2)!=0)
		{
			gCardStatus = M1LOCKED;
		}
		if((out[tint+9]<10)&&(out[tint+9]!=2))
		{
			gCardStatus = 0;
		}
	}
*/	
	gDebugStep = 0x08;
	tint = inbuf[2];
	
	ret=Transport_GetFiles15(outbuf);
	if(ret<0)
	{
		goto label_err;
	}
	if((gResponseStatus = Transport_GetFiles1E(tint,&outbuf[4])) != 0)
	{
		#ifdef DEBUG_PRINT
			printf("      Err: read record!\n");
		#endif
		gDebugStep = 0x09;
		mcml_pwr_off();
		goto label_err;
	}
	//mcml_pwr_off();
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = inbuf[2];
	*outbytes =52;
	return;
label_err:
	shmt_bom_func_err(outbuf,outbytes);
	return;
label_invalid:
	gResponseStatus=CE_INVALIDCARD;
	shmt_bom_func_err(outbuf,outbytes);
	return;
}

void shTransportLockCard(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	
int ret;
UBYTE obuf[256];
	
#ifdef _DEBUG_BOM_
debug_printf("\nshTransportLockCard...");
#endif

	if(inbytes != 13)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if(gStatus != TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	memcpy(gTransTime,&inbuf[2],7);
	if(check_time_valid() != 0) 
	{		
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus=IDLE_STATUS;	
	*((char *)&gPosTransCnt+LONG_HIGH3) = 0;
	*((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[9];
	*((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[10];
	*((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[11];
	gGetCardInfoStep = 0xff;
	memset(bgTaskKeyOutbuf, 0x00, 4);
	memset(gTAC, 0x00, 4);
	//
	ret = sh_transport_lockcard(inbuf[12]);
	if(ret) {
		shmt_bom_func_err(outbuf,outbytes);
		return;  	
	}
  //
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	if(inbuf[12]==0)
	{
		outbuf[3] = 3;
	}
	else
	{
		outbuf[3] = 3;
	}
	outbuf[4]	=	1;
	outbuf[5]	=	capp_init[9];
	outbuf[6]	=	capp_init[10];
	memcpy(&outbuf[7],&capp_init[4],2);//脱机交易序号
	memcpy(&outbuf[9],bgTaskKeyOutbuf,4);//PSAM流水号
	outbuf[13] = *((char *)&gCardThisRemain+LONG_HIGH0);
	outbuf[14] = *((char *)&gCardThisRemain+LONG_HIGH1);
	outbuf[15] = *((char *)&gCardThisRemain+LONG_HIGH2); 	
	outbuf[16] = gTAC[0];
	outbuf[17] = gTAC[1];
	outbuf[18] = gTAC[2];
	outbuf[19] = gTAC[3];
	*outbytes = 20;
	return; 

}

void shJTBCreditInit(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes)
{

	if(inbytes != 6) 	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

	if(gStatus != TRANS_STATUS)  
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
    //     
    *((char *)&gCardThisTransValue+3) = inbuf[5];
    *((char *)&gCardThisTransValue+2) = inbuf[4];
    *((char *)&gCardThisTransValue+1) = inbuf[3];
    *((char *)&gCardThisTransValue+0) = inbuf[2];
     if(jtb_credit_ini() != 0)	{
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

	gResponseStatus = CE_OK;	    
	memcpy(&gSBuf[3],gBuf,16);  	    
	gResponseStatus = CE_OK;
	gSBufBytes = 19;
	
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = gResponseStatus;
	memcpy(&outbuf[3], &gSBuf[3], gSBufBytes);
	*outbytes = gSBufBytes;
	return ;
}

void shJTBCredit(UBYTE *inbuf, UWORD inbytes, UBYTE *outbuf, UWORD *outbytes)
{

	if(inbytes != 13) 	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

	if(gStatus != TRANS_STATUS) 
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
    //     
    memcpy(gTransTime,&inbuf[2],7);
    if(check_time_valid() != 0) 	{
		gStatus=IDLE_STATUS; 
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

    memcpy(gMAC,&inbuf[9],4);
	gStatus = IDLE_STATUS;
	gStatusNot = ~gStatus;
	gGetCardInfoStep = 0xff;
    if(jtb_credit() != 0)  {
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

    //record
    gBuf[0]=*((char *)&gPosTransCnt+1);
    gBuf[1]=*((char *)&gPosTransCnt+2);
    gBuf[2]=*((char *)&gPosTransCnt+3);
    memcpy(&gBuf[3],gThisCardSnr,4);
    gBuf[7] = *((char *)&gCardCnt+0);
    gBuf[8] = *((char *)&gCardCnt+1);
    gBuf[9] = *((char *)&gCardThisRemain+2);
    gBuf[10] = *((char *)&gCardThisRemain+1);
    gBuf[11] = *((char *)&gCardThisRemain+0);
    gBuf[12] = gTAC[0];
    gBuf[13] = gTAC[1];
    gBuf[14] = gTAC[2];
    gBuf[15] = gTAC[3];  
    
    save_trans_record(gBuf);
  
	gResponseStatus = CE_OK;
	gStatus = IDLE_STATUS;
	gStatusNot = ~gStatus;
	memcpy(&gSBuf[3],gTAC,4);
	gSBufBytes = 7;
	
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = gResponseStatus;
	memcpy(&outbuf[3], &gSBuf[3], gSBufBytes);
	*outbytes = gSBufBytes;
	
	return ;
}

//下载参数：4026 
void shCertSetParam(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
int ret;
int bytes;
UBYTE cRet;
	
#ifdef _DEBUG_BOM_
debug_printf("\nshWhiteListSetParam...");
#endif

	if(inbytes < 2)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
  //
  //读写器类型
	if(gMode==BOM_MODE)
	{
		outbuf[3]=BOM_READER;
	}
	else
	{
		if(gMode==ENTRY_MODE)
		{
			outbuf[3]=GATE_ENTRY_READER;
		}
		else
		{
			outbuf[3]=GATE_EXIT_READER;
		}
	}
  
  //ret = pf_mobile_ticket_para_init(inbuf);//最后一个包时更新参数
  //
  cRet = binFileManage(0x68,inbuf,inbytes,&outbuf[12],&bytes);
  //
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = cRet;
	//outbuf[3] = 0x14;                       //读写器类型
	memcpy(&outbuf[4],"\x0\x0\x0\x0",4);//读写器内部软件版本（RFU）
	outbuf[7] = (gVersion & 0x1F);
	//memcpy(&outbuf[8],"\x55\x66\x77\x88",4);//读写器内部参数版本（4020版本号）
	LongToByte(tpCert4026.cert_head.ParaVersion,&outbuf[8]); 
	*outbytes = 12 + bytes;
	return; 
}

/************************************************************************************************
函数：ShQREntry
功能：上海地铁二维码进出站交易
************************************************************************************************/
void ShMQRTac(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE buf[100], obuf[100];
	#ifdef _debug_test_
	UDWORD	tint0=0,tint1=0;
	#endif

#ifdef _DEBUG_BOM_
debug_printf("\nShMQRTac...");
#endif
	
	if(gStatus != TRANS_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = WRITE_STATUS;
	gTransTime[0]=inbuf[5];
	gTransTime[1]=inbuf[4];
	gTransTime[2]=inbuf[3];
	gTransTime[3]=inbuf[2];
	*((char *)&gPosTransCnt+LONG_HIGH3) = inbuf[15];							  			
	*((char *)&gPosTransCnt+LONG_HIGH2) = inbuf[14];
	*((char *)&gPosTransCnt+LONG_HIGH1) = inbuf[13];
	*((char *)&gPosTransCnt+LONG_HIGH0) = inbuf[12];
	gCardThisTransValue=0;
	
	#ifdef _debug_test_
		tint0 = GetTickcount();
	#endif
	
	if(sh_mqr_tac(inbuf, obuf)!=0)
	{	
		mcml_pwr_off();
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	#ifdef _debug_test_
		tint1 = GetTickcount();
		debug_printf("\n sh_metro_entry(%d) \n",tint1-tint0);
	#endif
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	//tac
	memcpy(&outbuf[3], &obuf[0], 4); 		
	memset(&outbuf[7], 0x00, 8);
	*outbytes = 15;
	gStatus=IDLE_STATUS;
	//mcml_pwr_off();//
	
	#ifdef _RECOVERY_AFTER_RESET_
		if(bgResetWrTimeFlag){
		 rtc_wr_time(&gTransTime[1]);
		 bgResetWrTimeFlag=0;
		 }
	#endif
	
	return;	
}

//end of file
#endif