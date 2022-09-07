//shmt_bom_func.c

#ifndef _SHMT_BOM_FUNC_C_
#define _SHMT_BOM_FUNC_C_

#ifdef DEBUG_PRINT
#define _DEBUG_BOM_
#endif

//#define SCANNERPROTOCOLUPDATE

#define _HUAMING_GATE_

unsigned char sendMAC[20];
//extern unsigned char blnQR = 0;
unsigned char ODAKeyBuf[100];
UBYTE odaBuf[100];
UBYTE capp_init_temp[4];
//版本号
//UBYTE gVersion = 0x89;	//与SAM通讯修改
//sam+pps切频
//UBYTE gVersion = 0x64; 	//未切频
//UBYTE gVersion = 0x8D;//460800的版本2015/2/3 15:28:33
//UBYTE gVersion = 0x81;//115200的版本2015/2/3 15:28:33
//UBYTE gVersion = 0x83;//115200的版本2015/8/31 15:10:51
//UBYTE gVersion = 0x90;//460800的版本2015/8/31 15:10:51
//UBYTE gVersion = 0x91;//460800的版本2016/5/16
//UBYTE gVersion = 0x92;//460800的版本2017/5/32
//UBYTE gVersion = 0xA6;	//460800的版本2017/11/27 增加蓝牙返回错误代码
//UBYTE gVersion = 0xA5;  //2018/11/12 异地卡版本
//UBYTE gVersion = 0xA7;  //2018/06/28 增加ODA
//UBYTE gVersion = 0xAD;	//2019/06/25	交通部
//UBYTE gVersion = 0xAE;	//2019/06/25	交通部二维码
//UBYTE gVersion = 0xB0;	//2021/11/29	  随申码测试
//UBYTE gVersion = 0xB1;	//2021/11/29	  读1E文件，5.17

//UBYTE gVersion = 0xB2;	//2022/08/08	  扫描头协议，5.18
//UBYTE gVersion = 0xB3; //2022/08/24 错误包 5.19
UBYTE gVersion = 0xB3; //2022/08/26 测试版本

UBYTE gRespStep;

UBYTE gStatus;
cmd_sub_func_type *shmt_bom_func[256];
UBYTE oldsend[100],oldcmd,oldtimes;
UWORD oldsendcounter;
UBYTE gReadCmdTimes;

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
	shmt_bom_func[SM_UPDATE]=ShCardUpdate;
	
	shmt_bom_func[ST_ACTIVECARD]=ShMetroActive;
	shmt_bom_func[HH_SALECARD]=ShMetroSale;
	shmt_bom_func[HH_ENTRY]=ShMetroEntry;
	shmt_bom_func[HH_ENTRY2]=ShMetroSPEntry;
	shmt_bom_func[HH_EXIT]=ShMetroExit;
	shmt_bom_func[HH_LOCK]=ShMetroLock;
	shmt_bom_func[HH_UPDATE]=ShMetroUpdate;
	shmt_bom_func[HH_GETPROOF]=ShMetroGetProof;
	
	shmt_bom_func[MT_ENTRY]=ShMobileEntry;
	shmt_bom_func[MT_EXIT]=ShMobileExit;
	shmt_bom_func[MT_LOCK]=ShMobileLock;
	shmt_bom_func[MT_UPDATE]=ShMobileUpdate;
	shmt_bom_func[MT_GETPROOF]=ShMobileGetproof;
	
	shmt_bom_func[QR_ENTRY] = ShQREntry;
	shmt_bom_func[QR_EXIT] = ShQRExit;
	shmt_bom_func[QR_UPDATE] = ShQRUpdate;
	shmt_bom_func[QR_GETPROOF] = ShQRGetproof;
	//
	shmt_bom_func[QR_AUTH]=ShQRAuth;
	shmt_bom_func[QR_MAC]=ShQRMac;
	
	shmt_bom_func[TEST_OPENRED]=ShMetrotestopenred;
	shmt_bom_func[TEST_OPENGREEN]=ShMetrotestopengreen;
	shmt_bom_func[TEST_CLOSERED]=ShMetrotestclosered;
	shmt_bom_func[TEST_CLOSEGREEN]=ShMetrotestclosegreen;
	shmt_bom_func[TEST_REQUEST]=ShMetrotestrequest;
	shmt_bom_func[TEST_LOAD_SJT_KEY]=ShMetrotestloadkey;
	shmt_bom_func[TEST_SJTAuthen]=ShMetrotestSJTauth;
	shmt_bom_func[TEST_SJTREAD]=ShMetrotestSJTread;
	shmt_bom_func[TEST_SJTWRITE]=ShMetrotestSJTwrite;
	shmt_bom_func[readstep]=ShMetrotestReadDebugstep;
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
	
	//ODA	
	shmt_bom_func[ODA_ENTRY] = shODAEntry;
	shmt_bom_func[ODA_EXIT] = shODAExit;
	shmt_bom_func[ODA_UPDATE] = shODAUpdate;
	shmt_bom_func[ODA_GETPROOF] = shODAGetProof;
	shmt_bom_func[ODA_ENROLL] = shODAEnroll;
	shmt_bom_func[ODA_DISABLE] = shODADisable;
	shmt_bom_func[ODA_ONLYREAD] = shODAOnlyRead;
	
	//交通部
	shmt_bom_func[TRANSPORT_ENTRY]=shTransportEntry;
	shmt_bom_func[TRANSPORT_EXIT]=shTransportExit;
	shmt_bom_func[TRANSPORT_UPDATE]=shTransportUpdate;
	shmt_bom_func[TRANSPORT_GRTPROOF]=shTransportGetProof;
	shmt_bom_func[TRANSPORT_LOCKCARD]=shTransportLockCard;
	shmt_bom_func[TRANSPORT_SETPARAM]=shWhiteListSetParam;
	shmt_bom_func[TRANSPORT_RECORD]=shTransportRecord;
	
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
	UBYTE a[2];
	a[0] = 0x20;
	a[1] = 0x00;
	UBYTE buf[24];
	int ret;
#ifdef _DEBUG_BOM_
	debug_printf("\nIniReader...");
#endif
	//printf("\nzddledinitchange\n");
	DisplaySignal(&a[0],a[1],NULL,NULL);
	a[0] = 0x10;
	DisplaySignal(&a[0],a[1],NULL,NULL);
	
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
	if((inbuf[6]!=0x00)&&((inbuf[6]!=0x01)))	//SAM卡类型
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
  		if((bgSmPsamValid==0)&&(bgSMobilePsamValid==0)&&(bgSHJTBPsamValid==0))
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
	UBYTE a[2];
	a[0] = 0x20;
	a[1] = 0x00;
	UBYTE buf[10];
	unsigned char name_4016[10];
#ifdef _DEBUG_BOM_
	debug_printf("\nResetReader...\n");
#endif

	//printf("\nzddledresetchange\n");
	
	DisplaySignal(&a[0],a[1],NULL,NULL);
	a[0] = 0x10;
	DisplaySignal(&a[0],a[1],NULL,NULL);
	//rled(LED_OFF);
			//rled(LED_OFF);
			//led_ctrl = LED_OFF;
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
	ResetShTransportSam();
	//ResetShangHaiMobileSam();//2015/1/12 16:54:19
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
	//if(!(bgSHPsamValid||bgSmPsamValid))	
	if(!(bgSHPsamValid||bgSmPsamValid||bgSHJTBPsamValid))
	{
		outbuf[2]=CE_NOKEY;
		*outbytes=3;
		return;
	}
	if(bgSHPsamValid||bgSHJTBPsamValid)
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
	memset(&outbuf[20],0,6);
	outbuf[26]=0;
	*outbytes=27;
	//*outbytes=15;
	//
	bgResetFlag = 0;
	
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
函数：ReaderStatus
功能：读取读卡器状态
************************************************************************************************/
void ReaderStatus(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
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
	DisplaySignal(&inbuf[2],inbuf[3],NULL,NULL);
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
unsigned char blnQR = 0;                                                    //#######################################zdd20211210
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
	
	//set_card_type(ISO14443A_SH_TYPE);
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
	if(blnQR)
			getBlEOnlineReturn(sendMAC);
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
  gGetCardInfoStep=0;//0x00,可以读卡
  gTaskBusyFlag=0;
  //2013/11/13 19:33:26
  gAbortReaderFlag=0;
  //20131112?
  timer_set(TIMER_MS_POWEROFF,30000);
	gPowerOffFlag = 0;
	gTransTypeFlag = 0;
	outbuf[0] = SM_READCARD;//读卡命令0x30
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
#ifdef _DEBUG_BOM_
	printf("\nShCardEntry...gCPUflag=%02x\n",gCPUflag);
#endif
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
	UBYTE i,buf[20],ret;
	UBYTE obuf[256];
	UWORD j;
	UDWORD temp1,temp2,temp3;

#ifdef _DEBUG_BOM_
	debug_printf("\nShCardReadRecord...");
#endif

//	if(gMode != EXIT_MODE)
//	{
//		gResponseStatus = CE_BADCOMMAND;
//		goto label_err;
//	}
	if(inbytes!=4)
	{
		gResponseStatus = CE_BADPARAM;
		goto label_err;
	}
	if(gStatus==STOP_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		goto label_err;
	}
#ifdef _DEBUG_BOM_
	printf("\n39 sh_ticket_family = %d",sh_ticket_family);
#endif
	if(sh_ticket_family!=SH_TICKET_JTB)
	{//住建部
		#ifdef _DEBUG_BOM_
			printf("\ngCPUflag = %02x",gCPUflag);
		#endif
		if(gCPUflag==0x55)//CPU
		{
			buf_to_uword(&inbuf[2],j);
			#ifdef _DEBUG_BOM_
			printf("\nj...1: %04x\n",j);
			#endif
			gRelativePtr=0;
			switch(j)
			{
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
				default:gResponseStatus = CE_BADPARAM;
								goto label_err;
			}
			j=j<<3;
			#ifdef _DEBUG_BOM_
			printf("\nj...2: %04x\n",j);
			#endif
			//if(history_record(j,&outbuf[4]))
			if(history_record(j,&buf[0]))//2019-06-19 修改
			{
				goto label_err;
			}
			goto label_an;
		}
		for(i=0;i<20;i++)
		{
			if(mcml_request(PICC_REQSTD,buf)==0)
			{
				goto label_con;
			}
			delay_ms(3);
		}
	cardmoved:
		gResponseStatus = CE_CARDREMOVED;
		goto label_err;
	
	label_con:
		i=mcml_anticoll(gThisCardSnr);
		if(i!=0)
		{
			goto cardmoved;
		}
		i=mcml_select(gThisCardSnr,buf);
		if(i!=0)
		{
			goto cardmoved;
		}
		#ifdef _DEBUG_BOM_
			printf("\ninbuf[3]...: %02x",inbuf[3]);
		#endif
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
#ifdef _DEBUG_BOM_
		printf("\ninbuf[2]...: %02x\n",inbuf[2]);
#endif
	  if((inbuf[2]&0x08)==0x08)
		{
				
		  	gRelativePtr=11;
				i=mcmh_read(28,buf,0,0);
				#ifdef _DEBUG_BOM_
					printf("mcmh_read...i =%d",i);
				#endif
				if(i)
				{
			    gResponseStatus =CE_READERROR;
		  		goto label_err;
			  }
			 	#ifdef _DEBUG_BOM_	
					printf("\nmcmh_read buf :");
					for(i=0;i<16;i++)
					{
						printf("%02x ",buf[i]);
					}
					printf("\n");
				#endif
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
		if(trans_readrecord(buf) != 0) 
		{
		 		goto label_err;
		}

	label_an:
		outbuf[0] = oldcmd;
		outbuf[1] = oldtimes;
		outbuf[2] = CE_OK;
		outbuf[3] = gRelativePtr;
		memcpy(&outbuf[4],buf,16);
		*outbytes =20;
		return;
	}
	else 
	{//交通部
		buf_to_uword(&inbuf[2],j);
		gRelativePtr=0;
		//j=j<<3;
		//printf("\nj...3 :%04x\n",j);
		if(j==0x0001)
		{
			ret = get_record(0x06,1,0x17,&obuf[0]);//充值明细文件0008-->0006
#ifdef _DEBUG_BOM_	
			printf("\n0x06 get obuf :");
			for(i=0;i<23;i++)
			{
				printf("%02x ",obuf[i]);
			}
			printf("\n");
#endif
			if(ret)
			{
				goto label_err;
			}
			outbuf[0] = oldcmd;
			outbuf[1] = oldtimes;
			outbuf[2] = CE_OK;
			outbuf[3] = 0;
			//充资日期和时间：5字节
			memcpy(&outbuf[4],&obuf[17],5);
			//充资金额:3字节,取06文件6-9字节
			//memcpy(&outbuf[9],&obuf[6],3);
			outbuf[9] = obuf[8];
			outbuf[10] = obuf[7];
			outbuf[11] = obuf[6];
			//充资前金额:3字节
			*((char *)&temp1 + LONG_HIGH3) = 0;//temp1:充资后余额
			*((char *)&temp1 + LONG_HIGH2) = obuf[2];
			*((char *)&temp1 + LONG_HIGH1) = obuf[3];
			*((char *)&temp1 + LONG_HIGH0) = obuf[4];		
#ifdef _DEBUG_BOM_
			printf("\ntemp1 :%08x",temp1);
			printf("\ntemp1 :%d",temp1);
#endif
			*((char *)&temp2 + LONG_HIGH3) = obuf[5];//temp2:充资金额
			*((char *)&temp2 + LONG_HIGH2) = obuf[6];
			*((char *)&temp2 + LONG_HIGH1) = obuf[7];
			*((char *)&temp2 + LONG_HIGH0) = obuf[8];
#ifdef _DEBUG_BOM_
			printf("\ntemp2 :%08x",temp2);
			printf("\ntemp2 :%d",temp2);
#endif
			temp3 = temp1 - temp2;//temp3:充资前金额
#ifdef _DEBUG_BOM_
			printf("\ntemp3 :%08x",temp3);
			printf("\ntemp3 :%d\n",temp3);
#endif
			
			outbuf[12] = *((char *)&temp3+LONG_HIGH0);
			outbuf[13] = *((char *)&temp3+LONG_HIGH1);
			outbuf[14] = *((char *)&temp3+LONG_HIGH2);
			//充资后余额：3字节
			//memcpy(&outbuf[15],&obuf[2],3);
			outbuf[15] = obuf[4];
			outbuf[16] = obuf[3];
			outbuf[17] = obuf[2];
			//充资机号：4字节
			memcpy(&outbuf[18],&obuf[12],4);
			//操作员号：4字节
			memcpy(&outbuf[22],&obuf[12],4);
			//押金记录：1字节
			outbuf[26] = transport_15_data[29];
			//启动日期：4字节
			memcpy(&outbuf[27],&transport_15_data[20],4);
			//TAC：4字节
			memset(&outbuf[31],0,4);
			//RFU
			outbuf[35] = 0;
			
			*outbytes=36;
			return;
		}	
		if(j<=0x0400)
		{
			switch(j)
			{
				case 0x0002:	gRelativePtr = 1;
											break;
				case 0x0004:	gRelativePtr = 2;
											break;
				case 0x0008:	gRelativePtr = 3;
											break;
				case 0x0010:	gRelativePtr = 4;
											break;
				case 0x0020:	gRelativePtr = 5;
											break;
				case 0x0040:	gRelativePtr = 6;
											break;
				case 0x0080:	gRelativePtr = 7;
											break;
				case 0x0100:	gRelativePtr = 8;
											break;
				case 0x0200:	gRelativePtr = 9;
											break;
				case 0x0400:	gRelativePtr = 10;
											break;				
				default:
								gResponseStatus=CE_BADPARAM;
								gGetCardInfoStep=0xff;
								goto label_err;	
									break;									
			}			
			ret = get_record(0x07,gRelativePtr,0x10,&outbuf[4]);
#ifdef _DEBUG_BOM_
			printf("\nJTB read 0x07...ret=%d\n",ret);
			printf("\n0x07 get outbuf :");
			for(i=0;i<16;i++)
			{
				printf("%02x ",outbuf[i+4]);
			}
			printf("\n");
#endif	
			if(ret)
			{
				goto label_err;
			}
			if((lrc_chk2(&outbuf[4],15)!=0))
			{
				outbuf[0] = oldcmd;
				outbuf[1] = oldtimes;
				outbuf[2] = CE_OK;
				switch(gRelativePtr)
				{
					case 1:	outbuf[3] = 0x81;
									break;
					case 2:	outbuf[3] = 0x82;
									break;
					case 3:	outbuf[3] = 0x83;
									break;
					case 4:	outbuf[3] = 0x84;
									break;
					case 5:	outbuf[3] = 0x85;
									break;
					case 6:	outbuf[3] = 0x86;
									break;
					case 7:	outbuf[3] = 0x87;
									break;
					case 8:	outbuf[3] = 0x88;
									break;
					case 9:	outbuf[3] = 0x89;
									break;
					case 10:	outbuf[3] = 0x8A;
									break;
					default:
									break;
				}
				*outbytes=4;
				return;
			}
		}
		if (j==0x0800) 
		{
			ret = get_record(0x10,0x01,0x10,&outbuf[4]);
			
			if(ret)
			{
				goto label_err;
			}
			//if((lrc_chk2(&outbuf[4],15)!=0))
			//{
			//	memset(&outbuf[4],0,16);
			//}
			if((gCardStatus!=M1LOCKED)||(lrc_chk2(&outbuf[4],15)!=0))
			{	
				outbuf[0] = oldcmd;
				outbuf[1] = oldtimes;
				outbuf[2] = CE_OK;
				outbuf[3] = 0x8B;
				*outbytes=4;
				return;
			}
		}
		if (j==0x1000)
		{
			ret = get_record(0x10,0x01,0x10,&outbuf[4]);
			
			if(ret)
			{
				goto label_err;
			}
			//if((lrc_chk2(&outbuf[4],15)!=0))
			//{
			//	memset(&outbuf[4],0,16);
			//}
			if((gCardStatus!=M1LOCKED)||(lrc_chk2(&outbuf[4],15)!=0))
			{	
				outbuf[0] = oldcmd;
				outbuf[1] = oldtimes;
				outbuf[2] = CE_OK;
				outbuf[3] = 0x8C;
				*outbytes=4;
				return;
			}
		}
		if (j==0x2000)
		{
			ret = get_record(0x10,0x02,0x10,&outbuf[4]);
			
			if(ret)
			{
				goto label_err;
			}
			if((lrc_chk2(&outbuf[4],15)!=0))
			{
				outbuf[0] = oldcmd;
				outbuf[1] = oldtimes;
				outbuf[2] = CE_OK;
				outbuf[3] = 0x8D;
				*outbytes=4;
				return;
			}
			outbuf[0] = oldcmd;
			outbuf[1] = oldtimes;
			outbuf[2] = CE_OK;
			outbuf[3] = 13;
			*outbytes=20;
			return;
		}
		if(j>0x2000) 
		{
			gResponseStatus = CE_BADPARAM;
			goto label_err;
		}
		outbuf[0] = oldcmd;
		outbuf[1] = oldtimes;
		outbuf[2] = CE_OK;
		outbuf[3] = gRelativePtr;
		*outbytes=20;
		return;
	}	
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
#ifdef _DEBUG_BOM_
	printf("\n61 sh_ticket_family = %d",sh_ticket_family);
#endif
	if(sh_ticket_family!=SH_TICKET_JTB)
	{//住建部
			
		if((gStatus==IDLE_STATUS) || (gStatus==TRANS_STATUS))
		{
			mcml_pwr_off();
			set_card_type(ISO14443A_SH_TYPE);
			i = 0;
			while(i<8)
	 		{
	 			if(mcml_request(PICC_REQSTD,buf)==0)
	 			{
	 				printf("\nmcml_request...");
	 				break;
	 			}
	 			i++;
	 		}
	 		gDebugStep = 0x02;
	 		//printf("\ni = %d\n",i);
	 		if(i>=8)
	 		{
	 			gResponseStatus = CE_CARDREMOVED;
				shmt_bom_func_err(outbuf,outbytes);
				return;
	 		}
			gThisCardSnr[6] = buf[0];
			gThisCardSnr[7] = buf[1];
			//printf("\n\ngThisCardSnr[6]-[7] :%02x%02x\n",gThisCardSnr[6],gThisCardSnr[7]);
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
			#ifdef _DEBUG_BOM_
				printf("\nret...1 :%02x\n",ret);
			#endif
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
	#ifdef _DEBUG_BOM_
			printf("\nret...2= %02x\n",ret);
			printf("\ntint...1= %02x\n",tint);
	#endif
			//if(tint>=(ret-2))
			//	goto label_invalid;
			gCardStatus = out[tint+8];
			printf("\ngCardStatus...:%02x\n",gCardStatus);
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
		#ifdef _DEBUG_BOM_
		printf("\ntint...2= %04x\n",tint);
		#endif
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
	}
	else
	{//交通部
		if((gStatus==IDLE_STATUS) || (gStatus==TRANS_STATUS))
		{
			mcml_pwr_off();
			set_card_type(ISO14443A_SH_TYPE);
			i = 0;
			while(i<8)
	 		{
	 			if(mcml_request(PICC_REQSTD,buf)==0)
	 			{
	 				//printf("\nmcml_request...");
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
			//printf("\n\ngThisCardSnr[6]-[7] :%02x%02x\n",gThisCardSnr[6],gThisCardSnr[7]);
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
			memcpy(buf,"\x00\xA4\x04\x00\x08\xA0\x00\x00\x06\x32\x01\x01\x05\00",14);
			memset(out,0,sizeof(out));
			ret=mifpro_icmd0(14,buf,out);
			//printf("\nret...3 :%02x\n",ret);
			if(ret<0)
			{
				gResponseStatus=CE_READERROR;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			}
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
		}
		//printf("\ntint 2= %04x\n",tint);
		buf_to_uword(&inbuf[2],tint);
		//printf("\ntint 2= %04x\n",tint);
//		if(history_record(tint,&outbuf[3]))
//		{
//			#ifdef DEBUG_PRINT
//				printf("      Err: read record!\n");
//			#endif
//			gDebugStep = 0x09;
//			mcml_pwr_off();
//			goto label_err;
//		}
		if(tint<=8)
		{
			switch(tint)
			{
				case 1:	flag = 1;
								break;
				case 2:	flag = 2;
								break;
				case 4:	flag = 3;
								break;
				case 8:	flag = 4;
								break;
				default:
								gResponseStatus=CE_BADPARAM;
								gGetCardInfoStep=0xff;
								goto label_err;
			}
#ifdef _DEBUG_BOM_
			printf("\n***JTB get record 0x06***\n");
#endif
			ret = get_record(0x06,flag,0x17,&outbuf[4]);//充值明细文件0008-->0006
#ifdef _DEBUG_BOM_	
			printf("\n0x06 get outbuf :");
			for(i=0;i<23;i++)
			{
				printf("%02x ",outbuf[i+4]);
			}
			printf("\n");
#endif
			if(ret)
			{
				goto label_err;
			}
			goto label_out;
		}
		
		//printf("\ntint...2:%04x\n",tint);
		if(tint<=0x2000)
		{
			switch(tint)
			{
				case 0x0010:	flag = 1;
											break;
				case 0x0020:	flag = 2;
											break;
				case 0x0040:	flag = 3;
											break;
				case 0x0080:	flag = 4;
											break;
				case 0x0100:	flag = 5;
											break;
				case 0x0200:	flag = 6;
											break;
				case 0x0400:	flag = 7;
											break;
				case 0x0800:	flag = 8;
											break;
				case 0x1000:	flag = 9;
											break;
				case 0x2000:	flag = 10;
											break;				
				default:
								gResponseStatus=CE_BADPARAM;
								gGetCardInfoStep=0xff;
								goto label_err;
									break;									
			}
#ifdef _DEBUG_BOM_
			printf("\nflag... :%d\n",flag);
			printf("\n*** JTB get record 0x18***\n");
#endif
			ret = get_record(0x18,flag,0x17,&outbuf[4]);
#ifdef _DEBUG_BOM_
			printf("\n0x18 get outbuf :");
			for(i=0;i<23;i++)
			{
				printf("%02x ",outbuf[i+4]);
			}
			printf("\n");
#endif			
			if(ret)
			{
				goto label_err;
			}
			flag=flag+4;
			goto label_out;
		}
		if(tint==0x4000)
		{
//			gDebugStep = 0x09;
//			memcpy(buf,"\x00\xa4\x00\x00\x02\x10\x03",7);
//			ret = sam_apdu(bgSHJTBpsamIndex,buf,7,outbuf,&outbytes,0,0);
//			if(ret != 0)
//			{
//				bgSHPsamValid = 0;
//				gResponseStatus=CE_PSAMERROR;
//				gGetCardInfoStep=0xff;
//				goto label_err;
//		  }	
//		  gDebugStep = 0x0a;
//			//if((outbuf[0] != 0x61) &&((UBYTE)outbuf[0] != 0x90)) //2013/11/17 22:53:52
//			if((outbuf[outbytes-2] != 0x61) &&((UBYTE)outbuf[outbytes-2] != (UBYTE)0x90))
//			{
//				bgSHPsamValid = 0;
//				gResponseStatus=CE_PSAMERROR;
//				gGetCardInfoStep=0xff;
//				goto label_err;
//			}
//			gPSAMFileFlag = 0x00;
//			bgSHPsamValid = 1;
//			gDebugStep = 0x0b;
#ifdef _DEBUG_BOM_
			printf("\n***0x4000:get record 0x10***\n");
#endif			
			ret = get_record(0x10,0x01,0x10,&outbuf[4]);
			if(ret)
			{
				goto label_err;
			}
			if((lrc_chk2(&outbuf[4],15)!=0))
			{
				memset(&outbuf[4],0,16);
			}
			if(gCardStatus!=M1LOCKED)
			{
				gResponseStatus=CE_INVALIDCARD;
				goto label_err;
			}	
			outbuf[0] = oldcmd;
			outbuf[1] = oldtimes;
			outbuf[2] = CE_OK;
			outbuf[3] = 14;
			*outbytes = 20;

/*			
			ret = 0xaa;
			for(flag=0;flag<16;flag++)
			{
				ret^=buf[flag];
			}
			if(ret)
			{
				gResponseStatus=CE_INVALIDCARD;
				gGetCardInfoStep=0xff;
				return OP_ERR;
			}
			gDebugStep = 0x0c;
			//计算TAC
			//os_lock();
			memcpy(bgTaskKeyInbuf,buf,13);
			bgTaskKeyInbuf[13]=gCityCode0;
			bgTaskKeyInbuf[14]=gCityCode1;
			memcpy(&bgTaskKeyInbuf[15],gThisCardSnr,4);
			memset(&bgTaskKeyInbuf[19],0,13);
			set_task_key(TASK_KEY_GET_SHB28);
			//os_unlock();		
			//if(bgEnUCOS) OSTimeDly(1);
			if(SHGetTacStep2())
			{
				gResponseStatus=CE_PSAMERROR;
				gGetCardInfoStep=0xff;
				return OP_ERR;	
			}
			memcpy(gTAC,bgTaskKeyOutbuf,4);
			gDebugStep = 0x0d;	
			if((buf[13]!=gTAC[0])||(buf[14]!=gTAC[1]))
			{
				gResponseStatus =CE_INVALIDCARD;
  			gGetCardInfoStep=0xff;
				return OP_ERR;
			}
			memcpy(&outbuf[1],buf,16);
			memset(&outbuf[17],0,7);
			gDebugStep = 0x09;
			memcpy(buf,"\x00\xa4\x00\x00\x02\x10\x01",7);
			ret = sam_apdu(bgSHpsamIndex,buf,7,&outbuf[24],&outbytes,0,0);
			if(ret != 0)
			{
				bgSHPsamValid = 0;
				gResponseStatus=CE_PSAMERROR;
				gGetCardInfoStep=0xff;
				return OP_ERR;
		  }	
		  gDebugStep = 0x0a;
			//if((outbuf[24] != 0x61) &&((UBYTE)outbuf[24] != 0x90))  //2013/11/17 22:57:49
			if((outbuf[24+outbytes-2] != 0x61) &&((UBYTE)outbuf[24+outbytes-2] != (UBYTE)0x90))
			{
				bgSHPsamValid = 0;
				gResponseStatus=CE_PSAMERROR;
				gGetCardInfoStep=0xff;
				return OP_ERR;
			}
			gPSAMFileFlag = 0x55;
			bgSHPsamValid = 1;*/
			return;
		}
		if(tint==0x8000)
		{
#ifdef _DEBUG_BOM_			
			printf("\n***0x8000:get record 0x10***\n");
#endif
			ret = get_record(0x10,0x01,0x10,&outbuf[4]);

			if(ret)
			{
				goto label_err;
			}
			if((lrc_chk2(&outbuf[4],15)!=0))
			{
				memset(&outbuf[4],0,16);
			}
			if(gCardStatus!=M1LOCKED)
			{
				gResponseStatus=CE_INVALIDCARD;
				goto label_err;
			}
			outbuf[0] = oldcmd;
			outbuf[1] = oldtimes;
			outbuf[2] = CE_OK;
			outbuf[3]	=	15;
			*outbytes = 20;
			return;
		}	

	}
label_out:
		outbuf[0] = oldcmd;
		outbuf[1] = oldtimes;
		outbuf[2] = CE_OK;
		outbuf[3] = flag-1;//flag
		*outbytes = 27;
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
	//if((gStatus != IDLE_STATUS)&&(gStatus != TRANS_STATUS))
	if((gStatus != TRANS_STATUS))
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	gStatus = IDLE_STATUS;
	if(gCPUflag == 0x55)					//必须寻卡，对M1无影响，对CPU可选择AID
	{
/*//		mcml_pwr_off();
////		delay_ms(5);
//		//--------------------------------寻卡及选择AID
//		i = 0;
//		while(i<8)
// 		{
// 			if(mcml_request(PICC_REQSTD,buf)==0)
// 			{
// 				break;
// 			}
// 			i++;
// 		}
// 		gDebugStep = 0x02;
// 		if(i>=8)
// 		{
// 			goto readeeprom;
// 		}
//		gThisCardSnr[6] = buf[0];
//		gThisCardSnr[7] = buf[1];
//		gDebugStep = 0x03;
//		ret=mcml_anticoll(gThisCardSnr);
//		if(ret!=0)
//		{
//			goto readeeprom;
//		}
//		if(memcmp(gThisCardSnr,&inbuf[5],4) != 0)  		
//		{
//			gResponseStatus=CE_INVALIDCARD;
//			shmt_bom_func_err(outbuf,outbytes);
//			return;
//		}
//		gDebugStep = 0x04;
//		ret=mcml_select(gThisCardSnr,&i);
//		if(ret!=0)
//		{
//			goto readeeprom;
//		}
//		gDebugStep = 0x05;
//		gThisCardSnr[5] = i;
//		ret = mifpro_ats0(buf);
//		if(ret<0)
//		{
//			goto readeeprom;
//		}
//		gDebugStep = 0x06;
//		memcpy(buf,"\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01\x00",15);
//		ret = mifpro_icmd0(15,buf,out);
//		if(ret<0)
//		{
//			gResponseStatus=CE_READERROR;
//			shmt_bom_func_err(outbuf,outbytes);
//			return;
//		}
//		gDebugStep = 0x07;
//		if((out[ret-2]!=0x90)||(out[ret-1]!=0x00))
//		{
//			gResponseStatus=CE_INVALIDCARD;
//			shmt_bom_func_err(outbuf,outbytes);
//			return;
//		}*/
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
		
//		//---------credit sam for purchase
//		memcpy(buf,"\x80\x72\x00\x00\x04",5);
//		memcpy(&buf[5],out,4);
//		ret=sam_apdu(bgSHpsamIndex,buf,9,out,&i,0,0);
//		//---------return
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
	if((inbuf[13]!=0x64)&&(inbuf[13]!=0x65)&&(inbuf[13]!=0x68)&&(inbuf[13]!=0x69))
	{
		gResponseStatus = CE_BADPARAM;
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
	if(hh_trans_entry2(inbuf[13])!=0)
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
	memcpy(&outbuf[5],gTAC,4);
	//20070813协议
//	outbuf[9] = gPosNo[3];
//	outbuf[10]= gPosNo[1];
//	outbuf[11]= gPosNo[0];			
	outbuf[9] = hh_Entry[6];	
	outbuf[10] = hh_Entry[5];	
	outbuf[11] = hh_Entry[4];		
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
	if(inbuf[2]>3)
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
		printf("\n");
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

//获取参数版本
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

//下载参数：4016 
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
	
//#ifdef _DEBUG_BOM_
debug_printf("\nshWhiteListSetParam...");
//#endif

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
  
  cRet = binFileManage(0x88,inbuf,inbytes,&outbuf[12],&bytes);
  printf("\ncRet...:%d\n",cRet);
//  if(cRet!=0)
//  {
//  	outbuf[0] = oldcmd;
//		outbuf[1] = oldtimes;
//		//outbuf[2] = 0x00;
//		outbuf[2]	=	outbuf[12];
//		*outbytes = 2 + bytes;
//		return;
//  }
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = cRet;
	//outbuf[3] = 0x14;                       //读写器类型
	memcpy(&outbuf[4],"\x0\x0\x0\x0",4);//读写器内部软件版本（RFU）
	outbuf[7] = (gVersion & 0x1F);
	//4020版本号
	LongToByte(tpWhite4020.white_head.ParaVersion,&outbuf[8]);
//	outbuf[8] = buf_4020[0];
//	outbuf[9] = buf_4020[1];
//	outbuf[10] = buf_4020[2];
//	outbuf[11] = buf_4020[3];
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
#ifdef _DEBUG_BOM_
debug_printf("\nShMetroPfEtExit...ret=%d\n",ret);
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
int ret, rtn, i;
UBYTE obuf[256];
unsigned char respbuf[100], version[17], buffer[1024];
unsigned short len;
FILE *intFile;
long lngPosition, lngnum;
	
#ifdef _DEBUG_BOM_
debug_printf("\nShMetroGethwVer......");
#endif

	if(inbytes != 3)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	rtn = get_scanner_type(ISO14443A_SH_TYPE, respbuf, &len);
	memcpy(&version, &respbuf[4], 17);
	
#ifndef SCANNERPROTOCOLUPDATE
	if(memcmp(version, "\x41\x42\x37\x31\x33\x30\x46\x32\x32\x30\x33\x30\x32", 13) || memcmp(version, "\x41\x42\x37\x31\x33\x30\x46\x32\x32\x30\x33\x30\x33", 13))
		goto endscannerupdate;
	else memcpy(version, "\x41\x42\x37\x31\x33\x30\x46\x32\x32\x30\x33\x30\x33", 13);
#endif

#ifdef SCANNERPROTOCOLUPDATE
	if(memcmp(version, "\x41\x42\x37\x31\x33\x30\x46\x32\x32\x30\x33\x30\x33", 13))
	{
		//改变进入更新模式
		ret = scanner_update_prepare();
		if(ret != 0) 
		{
			printf("\nErr: scanner update prepare fail, will skip");
			goto endscannerupdate;
		}
		//检查文件的存在问题
	  intFile = fopen("./AB7130F220303.sml", "rb");
	  if(intFile == NULL)
	  	printf("\nErr: lack of scanner solid software\n");
	  //取文件长度
	  fseek(intFile, 0, SEEK_END);
	  lngnum = ftell(intFile);
	  fseek(intFile, 0, 0);
	  printf("\nLength: scanner solid software Length is %ld\n", lngnum);
	  if(lngnum == 0)
	  	printf("\nErr: scanner solid software is NULL\n");
	  //读取文件
	  for(i = 0; i < lngnum/128; i++)
	  {
	  	lngPosition = 128 * i;
	  	fseek(intFile, lngPosition, SEEK_SET);
	  	fread(&buffer, sizeof(char), 128, intFile);
	  	printf("\nTranslating The %d th package data:\n", i);
			//传送第i个包
	  	ret = send_scanner_software(&buffer, 128);
	  	if(ret == 0)
	  	{
	  		printf("\nSuccess\n");
	  		printf("\nSuccess: scanner solid software update The %d th package success\n", i);
	  	}else{
	  		goto endscannerupdate;
	  	}
		}
		printf("\nTransfered %d packages, still have 1 package to send.\n", i);
		if(lngnum/128 != 0)
		{
			memset(&buffer, 0x1a, 128);
			lngPosition += 128;
	  	fseek(intFile, lngPosition, SEEK_SET);
			int tail = lngnum % 128;
			fread(&buffer, sizeof(char), tail, intFile);
			printf("\nTail package size :%d\n", tail);
			printf("\nTranslating tail package:\n");
			
			//传送最后一个包
	  	ret = send_scanner_software(&buffer, 128);
	  	if(ret == 0)
	  	{
	  		printf("\nSuccess: scanner solid software update The last package success\n");	
	  	}else{
	  		goto endscannerupdate;
	  	}
		}
		end_scanner_software();
		printf("\nSuccess: scanner solid software update success\n");
		fclose(intFile);
	}
#endif

endscannerupdate:
  //返回上位机
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = 0x02; 				//厂家代码：1华铭，2华虹计通，3怡力，4邮通，5华腾，6华腾智能，7 DAT，8冠人
	//outbuf[4] = 0x00;   
	outbuf[4] = gVersion/32;	//硬件版本
	memcpy(&outbuf[5],"\x0\x0\x0\x0",4);  //软件版本
	outbuf[8] = (gVersion & 0x1F);
	//memcpy(&outbuf[9], &respbuf[4], 17);//扫描头版本，前十三为固件版本，后四为扫描头型号
	memcpy(&outbuf[9], &version, 17);
	*outbytes = 26;
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
debug_printf("\nShQR Entry...");
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
debug_printf("\nShQR Exit...");
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
函数：ShQRGetproof
功能：上海地铁二维码根据流水号取TAC
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
		gResponseStatus = CE_NORECORD;
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

void ShQRAuth(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
UBYTE buf[100], obuf[100];
	
	if(inbytes!=12)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if((gStatus != IDLE_STATUS))
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	
	if( qr_cal_auth(inbuf, obuf) != 0)
	{
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	memcpy(&outbuf[3],&obuf[0],4);
	*outbytes =7;
	return; 
}

void ShQRMac(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
UBYTE buf[100], obuf[100];

	if(inbytes!=86)
	{
		gResponseStatus = CE_BADPARAM;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	if((gStatus != IDLE_STATUS))
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	
	if( qr_cal_process_mac(inbuf, obuf) != 0)
	{
		gStatus = IDLE_STATUS;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	memcpy(&outbuf[3],&obuf[0], 40);
	*outbytes =43;
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

/************************************************************************************************
函数：shODAEntry
功能：ODA进站交易
************************************************************************************************/
void shODAEntry(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	int ret;
	UBYTE buf[20], obuf[256];

#ifdef _DEBUG_BOM_
	debug_printf("\nshODAEntry...");
#endif
	
//计算TAC
	//应用主账号 后8字节
	memcpy(ODAKeyBuf,&gODAData[4],8);
	//交易日期时间 4字节
	memcpy(&ODAKeyBuf[8],&inbuf[2],7);
	ODAKeyBuf[15]= 0x00;			//交易前余额,高在前
	ODAKeyBuf[16]= 0x00;
	ODAKeyBuf[17]= 0x00;			//交易金额,高在前
	ODAKeyBuf[18]= 0x00;
	//上次交易车站根据交易标志
	if((gODAData[37] & 0xC0) == 0x40)
	{
		ODAKeyBuf[19]= gODAData[45];
		ODAKeyBuf[20]= gODAData[46];
	}
	else 
	{
		ODAKeyBuf[19]= gODAData[56];
		ODAKeyBuf[20]= gODAData[57];
	}
	//设备编号
	memcpy(&ODAKeyBuf[21],gPosNo,4);
	//终端流水号
	memcpy(&ODAKeyBuf[25], &inbuf[9], 3);	
	//SAM卡号
	memcpy(&ODAKeyBuf[28],bpgSmpsamNo,4);
	
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
	SHODATACStep1(ODAKeyBuf);
	
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
			//获取TAC
			if(SHODATACStep2()!=0)		//取TAC
			{
				gResponseStatus = CE_PSAMERROR;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			}
			//备份TAC
			memcpy(gTAC,bgTaskKeyOutbuf,4);
			
			memcpy(&odaBuf[0],&inbuf[9],3);//流水号
			memcpy(&odaBuf[3],&gODAData[0],13);//13字节：2银行代码+10应用主账号+1主账号序列号
			memcpy(&odaBuf[16],gTAC,4);//TAC
			save_oda_record(odaBuf);
			
#ifdef _DEBUG_BOM_
	printf("\noda_zone_ticket_entry...ret=%d\n", ret);
#endif
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
	
	//获取TAC
	if(SHODATACStep2()!=0)		//取TAC
	{
		gResponseStatus = CE_PSAMERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	//备份TAC
	memcpy(gTAC,bgTaskKeyOutbuf,4);

	memcpy(&odaBuf[0],&inbuf[9],3);//流水号
	memcpy(&odaBuf[3],&gODAData[0],13);//13字节：2银行代码+10应用主账号+1主账号序列号
	memcpy(&odaBuf[16],gTAC,4);//TAC
	save_oda_record(odaBuf);	
	
#ifdef _DEBUG_BOM_
	printf("\nODA_Entry_TAC = %02x%02x%02x%02x\n", gTAC[0],gTAC[1],gTAC[2],gTAC[3]);
#endif

	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	//memcpy(&outbuf[3],&obuf[1],112);
	memcpy(&outbuf[3],&obuf[1],100);
	memcpy(&outbuf[103],gTAC,4);
	memcpy(&outbuf[107],&obuf[105],8);
	*outbytes = 115; 
	gRespStep = 1;
	gStatus=IDLE_STATUS;
	mcml_pwr_off();

	return;
}
/************************************************************************************************
函数：shODAExit
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

//计算TAC
	//应用主账号 8字节
	memcpy(ODAKeyBuf,&gODAData[4],8);
	//交易日期时间 7字节
	memcpy(&ODAKeyBuf[8],&inbuf[2],7);
	ODAKeyBuf[15]= 0x00;			//交易前余额,高在前
	ODAKeyBuf[16]= 0x00;
	ODAKeyBuf[17]= inbuf[13];			//交易金额,高在前
	ODAKeyBuf[18]= inbuf[12];
	//上次交易车站根据交易标志
	if((gODAData[37] & 0xC0) == 0x40)
	{
		ODAKeyBuf[19]= gODAData[45];
		ODAKeyBuf[20]= gODAData[46];
	}
	else 
	{
		ODAKeyBuf[19]= gODAData[56];
		ODAKeyBuf[20]= gODAData[57];
	}
	//设备编号
	memcpy(&ODAKeyBuf[21],gPosNo,4);
	//终端流水号
	memcpy(&ODAKeyBuf[25], &inbuf[9], 3);	
	//SAM卡号
	memcpy(&ODAKeyBuf[28],bpgSmpsamNo,4);
	
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
	SHODATACStep1(ODAKeyBuf);
	
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
			//获取TAC
			if(SHODATACStep2()!=0)		//取TAC
			{
				gResponseStatus = CE_PSAMERROR;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			}
			//备份TAC
			memcpy(gTAC,bgTaskKeyOutbuf,4);
			
			memcpy(&odaBuf[0],&inbuf[9],3);//流水号
			memcpy(&odaBuf[3],&gODAData[0],13);//13字节：2银行代码+10应用主账号+1主账号序列号
			memcpy(&odaBuf[16],gTAC,4);//TAC
			save_oda_record(odaBuf);
			
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
		else{
			gResponseStatus = CE_WRITEERROR;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
	}
	//获取TAC
	if(SHODATACStep2()!=0)		//取TAC
	{
		gResponseStatus = CE_PSAMERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	//备份TAC
	memcpy(gTAC,bgTaskKeyOutbuf,4);

	memcpy(&odaBuf[0],&inbuf[9],3);//流水号
	memcpy(&odaBuf[3],&gODAData[0],13);//13字节：2银行代码+10应用主账号+1主账号序列号
	memcpy(&odaBuf[16],gTAC,4);//TAC
	save_oda_record(odaBuf);	
	
#ifdef _DEBUG_BOM_
	printf("\nODA_Exit_TAC = %02x%02x%02x%02x\n", gTAC[0],gTAC[1],gTAC[2],gTAC[3]);
#endif
	
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	//memcpy(&outbuf[3],&obuf[1],112);
	memcpy(&outbuf[3],&obuf[1],100);
	memcpy(&outbuf[103],gTAC,4);
	memcpy(&outbuf[107],&obuf[105],8);
	*outbytes = 115;
	gStatus=IDLE_STATUS;

	mcml_pwr_off();
	return;
}

/************************************************************************************************
函数：shODAUpdate
功能：ODA更新交易
************************************************************************************************/
void shODAUpdate(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE ret;
	UBYTE buf[20], obuf[256];
	int i;
#ifdef _DEBUG_BOM_
	debug_printf("\nshODAUpdate...");
#endif
	
//计算TAC
	//应用主账号 8字节
	memcpy(ODAKeyBuf,&gODAData[2],8);
	//交易日期时间 4字节
	memcpy(&ODAKeyBuf[8],&inbuf[2],7);
	ODAKeyBuf[15]= 0x00;			//交易前余额,高在前
	ODAKeyBuf[16]= 0x00;
	ODAKeyBuf[17]= inbuf[13];			//交易金额,高在前
	ODAKeyBuf[18]= inbuf[12];
	//上次交易车站根据交易标志
	if((gODAData[37] & 0xC0) == 0x40)
	{
		ODAKeyBuf[19]= gODAData[45];
		ODAKeyBuf[20]= gODAData[46];
	}
	else 
	{
		ODAKeyBuf[19]= gODAData[56];
		ODAKeyBuf[20]= gODAData[57];
	}
	//设备编号
	memcpy(&ODAKeyBuf[21],gPosNo,4);
	//终端流水号
	memcpy(&ODAKeyBuf[25], &inbuf[9], 3);	
	//SAM卡号
	memcpy(&ODAKeyBuf[28],bpgSmpsamNo,4);
		
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
#ifdef _DEBUG_BOM_
	printf("\nODA_update...inbuf=");
	for(i=0;i<56;i++)
	{
		printf(" %02x",inbuf[i]);
	}
	printf("\n");
#endif
	ret = oda_zone_ticket_update(&inbuf[2], obuf);
	SHODATACStep1(ODAKeyBuf);
#ifdef _DEBUG_BOM_
	printf("\noda_zone_ticket_update...ret=%d\n", ret);
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
			//获取TAC
			if(SHODATACStep2()!=0)		//取TAC
			{
				gResponseStatus = CE_PSAMERROR;
				shmt_bom_func_err(outbuf,outbytes);
				return;
			}
			//备份TAC
			memcpy(gTAC,bgTaskKeyOutbuf,4);
			
			memcpy(&odaBuf[0],&inbuf[9],3);//流水号
			memcpy(&odaBuf[3],&gODAData[0],13);//13字节：2银行代码+10应用主账号+1主账号序列号
			memcpy(&odaBuf[16],gTAC,4);//TAC
			save_oda_record(odaBuf);
			
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
	
	//获取TAC
	if(SHODATACStep2()!=0)		//取TAC
	{
		gResponseStatus = CE_PSAMERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	//备份TAC
	memcpy(gTAC,bgTaskKeyOutbuf,4);
	
	memcpy(&odaBuf[0],&inbuf[9],3);//流水号
	memcpy(&odaBuf[3],&gODAData[0],13);//13字节：2银行代码+10应用主账号+1主账号序列号
	memcpy(&odaBuf[16],gTAC,4);//TAC
	save_oda_record(odaBuf);
	
#ifdef _DEBUG_BOM_
	printf("\nODA_Update_TAC = %d\n", gTAC);
#endif
	
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	//memcpy(&outbuf[3],&obuf[1],112);	
	memcpy(&outbuf[3],&obuf[1],100);
	memcpy(&outbuf[103],gTAC,4);
	memcpy(&outbuf[107],&obuf[105],8);
	*outbytes = 115; 
	gRespStep = 1;
	gStatus=IDLE_STATUS;
	mcml_pwr_off();

	return;
}

/************************************************************************************************
函数：shODAGetProof
功能：ODA重取交易认证码
************************************************************************************************/
void shODAGetProof(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	int ret;
	UBYTE obuf[256];
	UBYTE buf[20];

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
/*	if(get_oda_record(&inbuf[2],obuf)!=0)
	{
		gResponseStatus = CE_NORECORD;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}*/
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
		else
		{
			gResponseStatus = CE_WRITEERROR;
			shmt_bom_func_err(outbuf,outbytes);
			return;
		}
	}
	if(get_oda_record(&inbuf[2],buf)!=0)
	{
		gResponseStatus = CE_NORECORD;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
#ifdef _DEBUG_BOM_
	printf("\nbuf=%02x%02x%02x%02x\n", buf[0],buf[1],buf[2],buf[3]);
#endif
	//
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = obuf[0];
	//memcpy(&outbuf[3],&obuf[1],112);
	memcpy(&outbuf[3],&obuf[1],100);
	memcpy(&outbuf[103],buf,4);
	memcpy(&outbuf[107],&obuf[105],8);
	*outbytes = 115;
	return;
}

/************************************************************************************************
函数：shODAOnlyRead
功能：读ODA记录
************************************************************************************************/
void shODAOnlyRead(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE i;
	UBYTE cnt;
	UBYTE transtype;
	UBYTE buf[300], obuf[300];
	int ret,counter;

#ifdef _DEBUG_BOM_
	debug_printf("\nshODAOnlyRead...");
#endif

	gRespStep = 0;
	if(gStatus!=IDLE_STATUS)
	{
		gResponseStatus = CE_STATUSERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}

	mcml_pwr_on();
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
		
		#ifdef _DEBUG_BOM_
			debug_printf("\nshODAOnlyRead 33\n");
		#endif	
		
		gResponseStatus = CE_NOCARD;
		shmt_bom_func_err(outbuf,outbytes);
		return;
	}
	ret=mcml_anticoll(buf);
	if(ret!=0)
	{
		#ifdef _DEBUG_BOM_
		debug_printf("\x0d\x0a shODAOnlyRead mcml_anticoll Err\n");
		#endif
		
		gResponseStatus = CE_NOCARD;
		shmt_bom_func_err(outbuf,outbytes);
		return ;
	}
	gDebugStep = 0x3081;
	ret=mcml_select(buf,&cnt);
	if(ret!=0)
	{
		#ifdef _DEBUG_BOM_
		debug_printf("\x0d\x0a shODAOnlyRead mcml_select Err\n");
		#endif 
		
		gResponseStatus = CE_NOCARD;
		shmt_bom_func_err(outbuf,outbytes);
		return ;
	}
			
	gDebugStep = 0x3082;
	ret = mifpro_ats0(buf);
	if(ret<0)
	{
		#ifdef _DEBUG_BOM_
		debug_printf("shODAOnlyRead mifpro_ats0 Err\n");
		#endif 
		
		gResponseStatus = CE_NOCARD;
		shmt_bom_func_err(outbuf,outbytes);
		return ;
	}

	memcpy(buf,"\x00\xa4\x04\x00\x0e",5);
	memcpy(&buf[5],"2PAY.SYS.DDF01",14);
	ret = mifpro_icmd0(19,buf,obuf);
	
#ifdef _DEBUG_BOM_
	//debug_printf("\nshODAOnlyRead 2[%d]\n",ret);
	debug_printf("\nshODAOnlyRead 1[%d]\n",ret);

	for(i=0;i<ret;i++) debug_printf(" %02x",obuf[i]);
#endif
	
	if(ret<0)
	{
		gResponseStatus = CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return ;
	}	
	if((obuf[ret-2]!=0x90)||(obuf[ret-1]!=0x00)) {
		gResponseStatus = CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return ;
	}
	
	for(counter=0; counter < ret; counter++)
	{
		if(memcmp(&obuf[counter], "\xa0\x00\x00\x03\x33\x01", 6) == 0)
		{
			counter -= 1;
			break;
		}
	}
	
	/*memcpy(buf,"\x00\xa4\x04\x00\x0e",5);
	buf[4] = obuf[counter];
	memcpy(&buf[5], &obuf[counter + 1], counter);
	ret = mifpro_icmd0(5+counter,buf,obuf);
	#ifdef _DEBUG_BOM_
		debug_printf("\nshODAOnlyRead 2[%d]\n",ret);
		for(i=0;i<ret;i++) debug_printf(" %02x",obuf[i]);
	#endif*/
	memcpy(buf,"\x00\xa4\x04\x00\x0e",5);
 	buf[4] = obuf[counter];
	memcpy(&buf[5], &obuf[counter + 1], obuf[counter]);
		
#ifdef _DEBUG_BOM_
  debug_printf("\nshODAOnlyRead select aid\n");
  for(i=0;i<5+counter;i++) debug_printf(" %02x",buf[i]);
  debug_printf("\n");
 #endif
 	
 	ret = mifpro_icmd0(5+obuf[counter],buf,obuf);
 	
 #ifdef _DEBUG_BOM_
  debug_printf("\nshODAOnlyRead 2[%d]\n",ret);
  for(i=0;i<ret;i++) debug_printf(" %02x",obuf[i]);
  debug_printf("\n");
 #endif
 	
	if(ret<0)
	{
		gResponseStatus = CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return ;
	}	
	if((obuf[ret-2]!=0x90)||(obuf[ret-1]!=0x00)) {
		gResponseStatus = CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return ;
	}
	
	memcpy(buf, "\x00\xB0\x00\x00\x06\x01\x56\x29\x00\x00\x00", 11);
	ret = mifpro_icmd0(11,buf,obuf);
	
#ifdef _DEBUG_BOM_
	//debug_printf("\nshODAOnlyRead 2[%d]\n",ret);
	debug_printf("\nshODAOnlyRead 3[%d]\n",ret);

	for(i=0;i<ret;i++) debug_printf(" %02x",obuf[i]);
#endif
	
	if(ret<0)
	{
		gResponseStatus = CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return ;
	}	
	if((obuf[ret-2]!=0x90)||(obuf[ret-1]!=0x00)) {
		gResponseStatus = CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return ;
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = 0x01;
	memcpy(&outbuf[4],&obuf[0],ret -2);
	cnt = 4 + ret - 2;

	shmt_prot_send(outbuf,cnt);
	
	memcpy(buf, "\x00\xB0\x00\x00\x06\x01\x56\x29\x00\x00\x01", 11);
	ret = mifpro_icmd0(11,buf,obuf);
	
#ifdef _DEBUG_BOM_
	//debug_printf("\nshODAOnlyRead 2[%d]\n",ret);
	debug_printf("\nshODAOnlyRead 4[%d]\n",ret);

	for(i=0;i<ret;i++) debug_printf(" %02x",obuf[i]);
#endif
	
	if(ret<0)
	{
		gResponseStatus = CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return ;
	}	
	if((obuf[ret-2]!=0x90)||(obuf[ret-1]!=0x00)) {
		gResponseStatus = CE_READERROR;
		shmt_bom_func_err(outbuf,outbytes);
		return ;
	}
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	outbuf[3] = 0x82;
	memcpy(&outbuf[4],&obuf[0],ret -2);
	//*out_len = 4 + ret - 2;
	*outbytes = 4 + ret - 2;
	mcml_pwr_off();
	return;
}

/************************************************************************************************
函数：shODAEnroll
功能：ODA开通交易
************************************************************************************************/
void shODAEnroll(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	int ret;
	UBYTE obuf[256];
	
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
	
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = obuf[0];
	//memcpy(&outbuf[3],&obuf[1],91);
	memcpy(&outbuf[3],&obuf[1],112);          
	*outbytes = 115;
	return; 
}

/************************************************************************************************
函数：shODAEnroll
功能：ODA开通注销
************************************************************************************************/
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
	
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = obuf[0];
	//memcpy(&outbuf[3],&obuf[1],91);
	memcpy(&outbuf[3],&obuf[1],112);          
	*outbytes = 115;
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
	memcpy(capp_init_temp,&capp_init[4],2);
	memcpy(&outbuf[8],bgTaskKeyOutbuf,4);//PSAM流水号
	outbuf[12] = gTAC[0]; 
	outbuf[13] = gTAC[1];
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
	UBYTE buf[36];
	UBYTE ret;

#ifdef _DEBUG_BOM_
	debug_printf("\n\nshTransportExit...\n");
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

	ret = sh_transport_exit(transtype);
	//printf("\nsh_transport_exit...ret=: %02x\n",ret);
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
	outbuf[3]	=	1;//密钥索引
	outbuf[4]	=	capp_init[9];//密钥版本号
	outbuf[5]	=	capp_init[10];//算法标识
	memcpy(&outbuf[6],&capp_init[4],2);//脱机交易序号
	memcpy(capp_init_temp,&capp_init[4],2);
	memcpy(&outbuf[8],bgTaskKeyOutbuf,4);//PSAM流水号
	outbuf[12] = *((char *)&gCardThisRemain+LONG_HIGH0);
	outbuf[13] = *((char *)&gCardThisRemain+LONG_HIGH1);
	outbuf[14] = *((char *)&gCardThisRemain+LONG_HIGH2);
	outbuf[15] = gTAC[0];
	outbuf[16] = gTAC[1];
	outbuf[17] = gTAC[2];
	outbuf[18] = gTAC[3];
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
	UBYTE buf[36];

#ifdef _DEBUG_BOM_
	printf("\nshTransportUpdate...");
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
	memcpy(capp_init_temp,&capp_init[4],2);
	memcpy(&outbuf[8],bgTaskKeyOutbuf,4);//PSAM流水号
	outbuf[12] = *((char *)&gCardThisRemain+LONG_HIGH0);
	outbuf[13] = *((char *)&gCardThisRemain+LONG_HIGH1);
	outbuf[14] = *((char *)&gCardThisRemain+LONG_HIGH2); 	
	outbuf[15] = gTAC[0];
	outbuf[16] = gTAC[1];
	outbuf[17] = gTAC[2];
	outbuf[18] = gTAC[3];		
	*outbytes  = 19;
	
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
	int ret;
	UBYTE buf[64];
	UWORD cnt;
	UBYTE out[300];

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
	memcpy(buf,"\x80\x5a\x00\x09\x02",5);
	buf_to_uword(&inbuf[15],cnt);
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
	outbuf[3]	=	1;//密钥索引
	outbuf[4]	=	capp_init[9];//密钥版本号
	outbuf[5]	=	capp_init[10];//算法标识
	memcpy(&outbuf[6],capp_init_temp,2);//脱机交易序号(交易前)
	memcpy(&outbuf[8],bgTaskKeyOutbuf,4);//PSAM流水号
	outbuf[12] = *((char *)&gCardThisRemain+LONG_HIGH0);
	outbuf[13] = *((char *)&gCardThisRemain+LONG_HIGH1);
	outbuf[14] = *((char *)&gCardThisRemain+LONG_HIGH2); 	
	//printf("EX-gCardThisTransValue :%04x\n",gCardThisTransValue);
	//printf("EX-gCardThisRemain :%04x\n",gCardThisRemain);
	//TAC
	memcpy(&outbuf[15],&out[4],4);
	*outbytes = 19;
	gStatus=IDLE_STATUS;
	return;
		
readeeprom:
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
	//UBYTE outbuf[256];
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
	//tint = inbuf[2]+1;   //从1开始
	tint = inbuf[2];
	ret=Transport_GetFiles15(outbuf);

	if(ret<0)
	{
		goto label_err;
	}
	if(Transport_GetFiles1E(tint,&outbuf[4]))
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

/************************************************************************************************
函数：shTransportLockCard
功能：上海交通部锁卡
************************************************************************************************/
void shTransportLockCard(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{	
	int ret;
	
	UBYTE TransLockType;

#ifdef _DEBUG_BOM_
	debug_printf("\nshTransportLockCard...");
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
  //memset(obuf,0,sizeof(obuf));
  ret = sh_transport_lockcard(TransLockType);
  if(ret) 
  {
		gResponseStatus = ret;
		shmt_bom_func_err(outbuf,outbytes);
		return;  	
  }
  //
	outbuf[0] = oldcmd;
	outbuf[1] = oldtimes;
	outbuf[2] = CE_OK;
	if(TransLockType==0)
  {
  	outbuf[3] = 0x03;//锁卡结果
  	memset(&outbuf[9],0x00,4);//PSAM流水号
		memset(&outbuf[16],0x00,4);//TAC
  }
  else
  {
  	outbuf[3] = 0x03;//锁卡结果
  	memcpy(&outbuf[9],bgTaskKeyOutbuf,4);//PSAM流水号
		outbuf[16] = gTAC[0];
		outbuf[17] = gTAC[1];
		outbuf[18] = gTAC[2];
		outbuf[19] = gTAC[3];
  }
	outbuf[4]	=	1;
	outbuf[5]	=	capp_init[9];
	outbuf[6]	=	capp_init[10];
	memcpy(&outbuf[7],&capp_init[4],2);//脱机交易序号
	outbuf[13] = *((char *)&gCardThisRemain+LONG_HIGH0);
	outbuf[14] = *((char *)&gCardThisRemain+LONG_HIGH1);
	outbuf[15] = *((char *)&gCardThisRemain+LONG_HIGH2); 	

	*outbytes = 20;
	gStatus=IDLE_STATUS;
	return; 

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
	LongToByte(tpCert4026.cert_head.ParaVersion,&outbuf[8]);            //tpWhite4020.white_head.ParaVersion,&outbuf[8]
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
debug_printf("\nShMQRTac...");      //inbuf中第一个字节是命令重发序号

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