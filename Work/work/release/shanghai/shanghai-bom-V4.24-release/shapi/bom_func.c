#ifndef _BOM_FUNC_C_
#define _BOM_FUNC_C_

//#define	_EMU_

UBYTE  gB8[16],gB9[16],gB10[16],gB1[16],gB2[16];
uchar  gCardRecord[16];
uchar  gUpdateRecord[16];
UBYTE gAddAuthCode[8];

uchar CntCardNewStartDate[4];
uchar  CntCardFirstUse[4];
uchar  CntCardTotal[2];

uchar  gM1AddValKey[16];//
uchar  gM1AddValBlock[16];//
uchar  gAddValMAC[6];//

uchar  gM1SaleValKey[16];//
uchar  gM1SaleValBlock[16];//
uchar  gSaleValMAC[6];//1status+4MAC

//uchar  gTAC[6];//
uchar  gLineProtectionMAC[6];
UBYTE gShFormalCardKeyA0[6]={0xfc,0x61,0xcb,0xb5,0x37,0x51};
UBYTE gShTestCardKeyA0[6]={0x70,0xa4,0xf4,0x82,0x01,0x6d};
uchar gTransType;
uchar gApplicationSn[10]; //应用序列号	10
uchar gNewACardSnr[5];
uchar gApplicationTypeTag; //应用类型标识	1
UBYTE gB1[16];

/***********************************************************************************
函数：trans_setup_bakup
功能：建立备份
************************************************************************************/
uchar trans_setup_bakup(void)
{
uchar  buf[17];
uchar  bufagain[17];

#ifdef _EMU_
printf("\nTrans_setup_bakup now");
#endif
	
	buf[0] = 0x55;
	buf[1] = 0xaa; 
	buf[2] = 0x55;
	
	buf[8] = *((char *)&gCardCnt+1);
	buf[9] = *((char *)&gCardCnt+0);
	
	buf[7] = gCardRecPtr;
	buf[10] = gCardStatus;
	buf[13] = 0;
	memcpy(&buf[3],CntCardFirstUse,4);
	memcpy(&buf[11],CntCardTotal,2);
	buf[14] = gMetroIoFlag;
	lrc_gen2(buf,15);
	gDebugStep = 0x1530;
	if(hl_auth(AUTH_MODE_INT_KEYA,3,(UBYTE *)"\x0\x0\x0\x0\x0\x0") != 0) goto label_err_write;
	gDebugStep = 0x1531;
	if(mcml_write(12,buf) != 0) goto label_err_write;
	//回读，20110519
	gDebugStep = 0x1534;
	if(mcml_read(12,bufagain) != 0) goto label_err_write;
	gDebugStep = 0x1535;
	if(memcmp(buf,bufagain,16) != 0) goto label_err_write;
	return 0;
	
	label_err_write:
	gResponseStatus = CE_WRITEERROR;
	#ifdef _EMU_
	printf("\nError: write card");
	#endif
	return (uchar)-1;
}


char trans_credit_ini(void)
{
unsigned char ret;
#ifdef _EMU_
char i;
#endif

watchdog();
#ifdef _EMU_
printf("\ntrans_credit_ini now");
#endif

	gDebugStep = 0x2101;
	//initial for load
	memcpy(gBuf,"\x80\x50\x00\x02\x0b",5);
	gBuf[5] = 0x1;//密钥索引号
	udword_to_buf(gCardThisTransValue,&gBuf[6]);//交易金额
	memcpy(&gBuf[10],FM_Module_NO,6);//initial for load终端机编号
	gBufBytes = 16;
	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
	if(ret)  {
		gDebugStep = 0x2102;
		gResponseStatus = CE_WRITEERROR;  
		return (char)-1;
	}
	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x62) && ((uchar)gBuf[gBufBytes-1] == (uchar)0x81))  {
		gDebugStep = 0x2103;
		goto label_err_need_m1_restore;
	}

	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
		gDebugStep = 0x2104;
		gResponseStatus = CE_WRITEERROR;  
		return (char) -1;
	}

	return 0;

label_err_need_m1_restore:
	gResponseStatus = CE_NEED_M1_RESTORE;
	return (char) -1;
}


char trans_credit(void)
{
unsigned char ret;
#ifdef _EMU_
char i;
#endif

watchdog();

#ifdef _EMU_
printf("\ntrans_credit now");
#endif	

	gTransType = 0x2;

	gDebugStep = 0x2201;
	//credit for load
	memcpy(gBuf,"\x80\x52\x00\x00\x0b",5);
	memcpy(&gBuf[5],gTransTime,7);//date4+time3
	memcpy(&gBuf[12],gMAC,4);//mac2
	gBuf[16] = 0x04;
	gBufBytes = 17;
	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
	if(ret)  {
		gDebugStep = 0x2202;
		gResponseStatus = CE_WRITEERROR;  
		return (char)-1;
	}
	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x69) && (gBuf[gBufBytes-1] == 0x1))  {
		gDebugStep = 0x2203;
		gResponseStatus = CE_WRITEERROR;  
		return (char) -1;
	}
	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x69) && (gBuf[gBufBytes-1] == 0x85))  {
		gDebugStep = 0x2204;
		gResponseStatus = CE_WRITEERROR;  
		return (char) -1;
	}
	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x93) && (gBuf[gBufBytes-1] == 0x2))  {
		gDebugStep = 0x2205;
		gResponseStatus = CE_MAC2ERR;  
		return (char) -1;
	}
	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
		gDebugStep = 0x2206;
		gResponseStatus = CE_WRITEERROR;  
		return (char) -1;
	}
  
	memcpy(gTAC,gBuf,4);
	gDebugStep = 0x2208;
	return 0;	
}



char trans_cpu_app_unlock(void)
{
char ret;
int i;
unsigned char ptr;

watchdog();

#ifdef _EMU_
printf("\ntrans_cpu_app_unlock now.");
#endif

	gDebugStep = 0x2104;
	if(memcmp(gMAC,"\x0\x0\x0\x0",4) != 0)  goto label_online_cpu_app_unlock;//联机应用解锁

	//脱机应用解锁
	ret = iso_deselect(1);
	if(ret != 0){
		#ifdef _EMU_
		printf("\nErr:iso_deselect,ret=%d",ret);
		#endif
	}
#ifdef _EMU_  
printf("\nOK:iso_deselect.");
#endif

	//检查卡是否存在
	for(i=0;i<MAX_TRY;i++){
		ret = hl_active(0,REQ_CNT,gThisCardSnr,gBuf);
		if(ret != 0){
			continue;
		}
#ifdef _EMU_  
printf("\n卡号:");
for(i=0;i<4;i++) printf("%02X ",(UBYTE)gThisCardSnr[i]);
#endif
		goto label_con;
	}
	goto label_err_card_moved;

label_con:
    #ifndef _EMU_
      delay_ms(5);
    #endif
	ret = iso_rats(1,gBuf);	
	if(ret) {
    #ifndef _EMU_
      delay_ms(5);
    #endif
		ret = iso_rats(1,gBuf);
	}
	if(ret) {
    #ifndef _EMU_
      delay_ms(5);
    #endif
		ret = iso_rats(1,gBuf);
	}		
	if(ret)  {
  	   gDebugStep = 0x2105;
  	   goto label_err_read;
	}
  #ifdef _EMU_
  printf("\niso_rats:");
  for (i=0;i<16;i++)  printf(" %02x",gBuf[i]);
  #endif


  //select file
	memcpy(gBuf,"\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01\x00",15);
	gBufBytes = 15;
	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
	if(ret)  {
  	   gDebugStep = 0x2108;
  	   goto label_err_read;
	}
  #ifdef _EMU_
  printf("\niso_apdu select file:");
  for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  #endif

	//搜索9f 0C
	for(ptr=0;ptr<gBufBytes;ptr++){
    //if(memcmp(&gBuf[ptr],"\x0c\x1e",2) == 0) goto label_found_9f0c_ok;
		if(memcmp(&gBuf[ptr],"\x9f\x0c",2) == 0) 
		{
			ptr = ptr + 3;
			goto label_found_9f0c_ok;
		}
	}

	//read 0015
	memcpy(gBuf,"\x00\xb0\x95\x00\x1e",5);
	gBufBytes = 5;
	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
	if(ret)  {
		gDebugStep = 0x2094;
		goto label_err_read;
	}
  #ifdef _EMU_
  printf("\niso_apdu read 0015:");
  for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  #endif

	if(((uchar)gBuf[gBufBytes-2]!=(uchar)0x90)||(gBuf[gBufBytes-1]!=0x00))
	{
		gDebugStep = 0x2095;
		goto label_invalid;
	}
	ptr = 0;

label_found_9f0c_ok:
	memcpy(gCityCode,&gBuf[ptr+2],2);//城市代码
	memcpy(gApplicationSn,&gBuf[ptr+10],10);//应用序列号	10
	memcpy(gThisCpuCardSnr,&gBuf[ptr+12],8);//卡号,卡唯一代码？
	memset(gNewACardSnr,0,sizeof(gNewACardSnr));//下次取卡信息时重新计算keya 20090609	   
     #ifdef _EMU_  
     printf("\n城市代码: %02x %02x",gCityCode[0],gCityCode[1]);
     printf("\n卡号 gThisCpuCardSnr:");
     for(i=0;i<8;i++) printf(" %02X",(UBYTE)gThisCpuCardSnr[i]);
     #endif
	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x6a) && (gBuf[gBufBytes-1] == (uchar)0x81))  {
  	   gDebugStep = 0x2109;
  	   gResponseStatus = CE_NOFUNCTION;  
  	   goto label_cpu_app_unlock;
	}
	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x90) && (gBuf[gBufBytes-1] == 0))  {
  	   gDebugStep = 0x210a;
  	   return 0;
	}
	goto label_err_read;

label_cpu_app_unlock:
	//get random
	memcpy(gBuf,"\x00\x84\x00\x00\x04",5);
	gBufBytes = 5;
	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
	if(ret)  {
  	   gDebugStep = 0x210b;
  	   goto label_err_read;
	}
  #ifdef _EMU_
  printf("\niso_apdu get random:");
  for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  #endif
	memcpy(gRandom,gBuf,4);//	    
	
	sam_set(SAMLOCATION_5,SAM_ETU_93, 4);
	sam_select(SAMLOCATION_5);
	sam_atr(SAMLOCATION_5, gBuf,&gBufBytes);
  		#ifdef _EMU_
  		printf("\ncpu app unlock->sam_atr:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  		#endif
  		
	//read psam 0016
	memcpy(gBuf,"\x00\xb0\x96\x00\x00",5);
	gBufBytes = 5;
	ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes,0,0);
  		#ifdef _EMU_
  		printf("\ncpu app unlock->sam_apdu read 0016:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  		#endif
//	if(((uchar)gBuf[0] != (uchar)0x61) || (gBuf[1] != 0x06))
//	{
//  			  	gDebugStep = 0x210c;
//  			  	gResponseStatus = CE_PSAMERROR;
//  			  	return (char)-1;
//	}  		

	//get response
	if( (gBufBytes == 2) && (gBuf[0] == 0x61) )
	{
		memcpy(gBuf,"\x00\xc0\x00\x00\x06",5);
		gBufBytes = 5;
		ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes,0,0);
	  		#ifdef _EMU_
	  		printf("\ncpu app unlock->sam_apdu read 0016 get response:");
	  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
	  		#endif
	}		    		
		if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0x0))
		{
			gDebugStep = 0x210d;
	  			  	gResponseStatus = CE_PSAMERROR;
	  			  	return (char)-1;
		}  	
	//memcpy(psam_no,gBuf,6);


	memcpy(gBuf,"\x00\xa4\x00\x00\x02\x10\x01",7);
	gBufBytes = 7;
	ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes,0,0);
  		#ifdef _EMU_
  		printf("\ncpu app unlock->sam_apdu select 1001:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  		#endif
//	if(((uchar)gBuf[0] != (uchar)0x61) || (gBuf[1] != 0x28))
//	{
//  			  	gDebugStep = 0x210e;
//  			  	gResponseStatus = CE_PSAMERROR;
//  			  	return (char)-1;
//	}
//
	//get response
	if( (gBufBytes == 2) && (gBuf[0] == 0x61) )
	{
		memcpy(gBuf,"\x00\xc0\x00\x00\x28",5);
		gBufBytes = 5;
		ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes,0,0);
	  		#ifdef _EMU_
	  		printf("\ncpu app unlock->sam_apdu select 1001 get response:");
	  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
	  		#endif
	}
		if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0x0))
		{
	  			  	gDebugStep = 0x210f;
	  			  	gResponseStatus = CE_PSAMERROR;
	  			  	return (char)-1;
		}

	memcpy(gBuf,"\x00\xb0\x97\x00\x01",5);
	gBufBytes = 5;
	ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes,0,0);
  		#ifdef _EMU_
  		printf("\ncpu app unlock->sam_apdu read 0017:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  		#endif
	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0x0))
	{
		gDebugStep = 0x2110;
  			  	gResponseStatus = CE_PSAMERROR;
  			  	return (char)-1;
	}
	gKeyVersion = gBuf[0];	//密钥版本号
  		
	memcpy(gBuf,"\x80\x1A\x45\x01\x10",5);
	memcpy(&gBuf[5],gThisCpuCardSnr,8);
	memcpy(&gBuf[13],gCityCode,2);
	memcpy(&gBuf[15],"\xFF\x00\x00\x00\x00\x00",6);
	gBufBytes = 21;
	ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes,0,0);
  		#ifdef _EMU_
  		printf("\ncpu app unlock->sam_apdu cal mac step1:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  		#endif
	if(((uchar)gBuf[0] != (uchar)0x90) || (gBuf[1] != 0x0))
	{
  			  	gDebugStep = 0x2111;
  			  	gResponseStatus = CE_PSAMERROR;
  			  	return (char)-1;
	}  		
  		
	memcpy(gBuf,"\x80\xfA\x05\x00\x10",5);
	memcpy(&gBuf[5],gRandom,4);
	memcpy(&gBuf[9],"\x00\x00\x00\x00\x84\x18\x00\x00\x04\x80\x00\x00",12);
	gBufBytes = 21;
	ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes,0,0);
  		#ifdef _EMU_
  		printf("\ncpu app unlock->sam_apdu cal mac step2:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  		#endif
//	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x61) || (gBuf[gBufBytes-1] != 0x04))
//	{
//  			  	gDebugStep = 0x2112;
//  			  	gResponseStatus = CE_PSAMERROR;
//  			  	return (char)-1;
//	}
	//get response
	if( (gBufBytes == 2) && (gBuf[0] == 0x61) )
	{
		memcpy(gBuf,"\x00\xc0\x00\x00\x04",5);
		gBufBytes = 5;
		ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes,0,0);
	  		#ifdef _EMU_
	  		printf("\ncpu app unlock->sam_apdu cal mac step2 get response:");
	  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
	  		#endif
	}
		if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0x0))
		{
	  			  	gDebugStep = 0x2113;
	  			  	gResponseStatus = CE_PSAMERROR;
	  			  	return (char)-1;
		}  			    		
	memcpy(gMAC,gBuf,4);


//application unlock
label_online_cpu_app_unlock:  
	memcpy(gBuf,"\x84\x18\x00\x00\x04",5);
	memcpy(&gBuf[5],gMAC,4);
	gBufBytes = 9;
  #ifdef _EMU_
  printf("\niso_apdu app unlock:");
  for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  #endif
	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
	if(ret)  {
		gDebugStep = 0x2114;
		goto label_err_read;
	}
  #ifdef _EMU_
  printf("\niso_apdu app unlock-->resp:");
  for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  #endif
	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x62) && ((uchar)gBuf[gBufBytes-1] == (uchar)0x81))  
	{
//		gDebugStep = 0x2115;
//			  #ifdef _EMU_
//			  printf("\niso_apdu app unlock-->M1 need to restore,transfer to read in M1 mode.");
//			  #endif
//  	    ret = trans_getcardinfo();
//  	    if(ret)  goto label_err_read;
//			  #ifdef _EMU_
//			  printf("\niso_apdu app unlock-->M1 restore ok,transfer to read in CPU mode again.");
//			  #endif
//		gDebugStep = 0x2116;
//  	    ret = trans_getcardinfo_cpu();
//			  #ifdef _EMU_
//			  printf("\niso_apdu app unlock-->trans_getcardinfo_cpu,ret=%02x,gApplicationLockedFlag=%02x",(UBYTE)ret,(UBYTE)gApplicationLockedFlag);
//			  printf("\niso_apdu app unlock-->CPU read ok,try to app unlock again.");
//			  #endif
//		gDebugStep = 0x2117;
//		memcpy(gBuf,"\x84\x18\x00\x00\x04",5);
//		memcpy(&gBuf[5],gMAC,4);
//		gBufBytes = 9;
//			  #ifdef _EMU_
//			  printf("\niso_apdu app unlock again:");
//			  for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//			  #endif
//		ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
//		if(ret)  {
//			gDebugStep = 0x2118;
//			goto label_err_read;
//		}
//			  #ifdef _EMU_
//			  printf("\niso_apdu app unlock again-->resp:");
//			  for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//			  #endif
//		if(((uchar)gBuf[gBufBytes-2] == (uchar)0x62) && ((uchar)gBuf[gBufBytes-1] == (uchar)0x81))  
//		{
//			gDebugStep = 0x2119;  	   
			goto label_err_need_m1_restore;
//		}
	}

	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
  	   gDebugStep = 0x211a;
  	   goto label_err_read;
	}
  	   
	return 0;

label_err_read:
	gResponseStatus = CE_READERROR;
	return (char) -1;

label_invalid:
	gResponseStatus = CE_INVALIDCARD;
	#ifdef _EMU_
	printf("\nError: card invalid.");
	#endif
	return (char)-1;

label_err_card_moved:
	gResponseStatus = CE_CARDREMOVED;
	return (char) -1;	

label_err_need_m1_restore:
	gResponseStatus = CE_NEED_M1_RESTORE;
	return (char) -1;	 	   
}  	   	


char trans_cpu_get_random(void)
{
char ret;
int i;
unsigned char ptr;

#ifdef _EMU_
printf("\ntrans_cpu_get_random now.");
#endif

//	ret = iso_deselect(1);
//	if(ret != 0){
//		#ifdef _EMU_
//	  printf("\nErr:iso_deselect,ret=%d",ret);
//	  #endif
//	}
//#ifdef _EMU_  
//printf("\nOK:iso_deselect.");
//#endif
//	
//	//检查卡是否存在
//	for(i=0;i<MAX_TRY;i++){
//		ret = hl_active(0,REQ_CNT,gThisCardSnr,gBuf);
//		if(ret != 0){
//	   #ifdef _EMU_
//     printf("\nErr:hl_active,ret=%d",ret);
//     #endif
//			continue;
//		}
//     #ifdef _EMU_  
//     printf("\n卡号:");
//     for(i=0;i<4;i++) printf("%02X ",(UBYTE)gThisCardSnr[i]);
//     #endif
//		goto label_card_found_cpu_app_unlock;
//     }
//     gDebugStep = 0x2116;
//     gResponseStatus = CE_CARDREMOVED;
//     return (char)-1;
//     
//label_card_found_cpu_app_unlock:
//    #ifndef _EMU_
//      delay_ms(5);
//    #endif
//	ret = iso_rats(1,gBuf);	
//	if(ret) {
//    #ifndef _EMU_
//      delay_ms(5);
//    #endif
//		ret = iso_rats(1,gBuf);
//	}
//	if(ret) {
//    #ifndef _EMU_
//      delay_ms(5);
//    #endif
//		ret = iso_rats(1,gBuf);
//	}	
//	if(ret)  {
//  	   gDebugStep = 0x2117;
//  	   gResponseStatus = CE_CARDREMOVED;  
//  	   return (char)-1;
//	}
//  #ifdef _EMU_
//  printf("\niso_rats:");
//  for (i=0;i<16;i++)  printf(" %02x",gBuf[i]);
//  #endif
//
//	//select file
//	memcpy(gBuf,"\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01\xa5",15);
//	gBufBytes = 15;
//	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
//	if(ret)  {
//  	   gDebugStep = 0x2118;
//  	   gResponseStatus = CE_CARDREMOVED;  
//  	   return (char)-1;
//	}
//  #ifdef _EMU_
//  printf("\niso_apdu select file:");
//  for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//  #endif
//	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
//  	   gDebugStep = 0x2119;
//  	   //gResponseStatus = CE_READERROR;  
//  	   //return (char) -1;
//	}
//
//  //搜索9f 0C
//	for(ptr=0;ptr<gBufBytes;ptr++){
//	    //if(memcmp(&gBuf[ptr],"\x0c\x1e",2) == 0) goto label_found_9f0c_ok;
//	    if(memcmp(&gBuf[ptr],"\x9f\x0c",2) == 0) 
//	  	{
//	  		ptr = ptr + 3;
//	  		goto label_found_9f0c_ok;
//	  	}
//	}
//
//  //read 0015
//	memcpy(gBuf,"\x00\xb0\x95\x00\x1e",5);
//	gBufBytes = 5;
//	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
//	if(ret)  {
//  	 gDebugStep = 0x2094;
//  	 goto label_err_read;
//	}
//  #ifdef _EMU_
//  printf("\niso_apdu read 0015:");
//  for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//  #endif
//
//	if(((uchar)gBuf[gBufBytes-2]!=(uchar)0x90)||(gBuf[gBufBytes-1]!=0x00))
//	{
//		gDebugStep = 0x2095;
//		goto label_invalid;
//	}
//	ptr = 0;
//
//label_found_9f0c_ok:
//	memcpy(gThisCpuCardSnr,&gBuf[ptr+12],8);
//	memset(gNewACardSnr,0,sizeof(gNewACardSnr));//下次取卡信息时重新计算keya 20090609	   
//     #ifdef _EMU_  
//     printf("\n卡号 gThisCpuCardSnr:");
//     for(i=0;i<8;i++) printf(" %02X",(UBYTE)gThisCpuCardSnr[i]);
//     #endif

	 //get random
	memcpy(gBuf,"\x00\x84\x00\x00\x04",5);
	gBufBytes = 5;
	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
	if(ret)  {
  	   gDebugStep = 0x2096;
  	   gResponseStatus = CE_CARDREMOVED;  
  	   return (char)-1;
	}
  #ifdef _EMU_
  printf("\niso_apdu get random:");
  for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  #endif
	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
  	   gDebugStep = 0x2097;
  	   gResponseStatus = CE_READERROR;  
  	   return (char) -1;
	}
	memcpy(gRandom,gBuf,4);
  
	return 0;

label_invalid:
	gResponseStatus = CE_INVALIDCARD;
#ifdef _EMU_
printf("\nError: card invalid.");
#endif
	return (char)-1;

label_err_read:
	gResponseStatus = CE_READERROR;
#ifdef _EMU_
printf("\nError: card read error.");
#endif
	return (char)-1;
}  	
	



char trans_expiredate1(void)
{
char ret;
char i;
unsigned char ptr;

	watchdog();
	#ifdef _EMU_
	printf("\ntrans_expiredate1-->gCardNewExpiredDate:");
	for(i=0;i<4;i++) printf(" %02x",gCardNewExpiredDate[i]);
	printf("\ntrans_expiredate1-->gMAC:");
	for(i=0;i<4;i++) printf(" %02x",gMAC[i]);
	#endif

	if((memcmp(gCardNewExpiredDate,"\x0\x0\x0\x0",4) != 0)&&(memcmp(gMAC,"\x0\x0\x0\x0",4) == 0)) {
		gDebugStep = 0x2152;
		gResponseStatus = CE_BADPARAM;
		return (char)-1;	
	}
  
	if((memcmp(gCardNewExpiredDate,"\x0\x0\x0\x0",4) != 0)&&(memcmp(gMAC,"\x0\x0\x0\x0",4) != 0)) 
	{
		#ifdef _EMU_
		printf("\ntrans_expiredate1 now.");
		#endif
	  
		gDebugStep = 0x2153;
		memcpy(gTransTime,gCardNewExpiredDate,4);
		memcpy(&gTransTime[4],"\x11\x22\x33",3);
		if(check_time_valid() != 0)  return (char)-1;
	       	
		//cpu expire date
		memcpy(gBuf,"\x04\xd6\x95\x18\x08",5);//应用有效日期
		memcpy(&gBuf[5],gCardNewExpiredDate,4);
		memcpy(&gBuf[9],gMAC,4);
		gBufBytes = 13;
		ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
		if(ret)  {
			gDebugStep = 0x2154;
			gResponseStatus = CE_CARDREMOVED;  
			return (char)-1;
		}
		#ifdef _EMU_
		printf("\niso_apdu cpu expire date:");
		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
		#endif
		if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
			gDebugStep = 0x2155;
			gResponseStatus = CE_WRITEERROR;  
			return (char)-1;
		}	
	
		return 0;
	}

	if((memcmp(gCardNewExpiredDate,"\x0\x0\x0\x0",4) == 0)&&(memcmp(gMAC,"\x0\x0\x0\x0",4) != 0))  goto label_online_cpu_modify_m1_consumption;

//	#ifdef _EMU_
//	printf("\ntrans_expiredate1 now-->0015.8-->|0x80 get mac offline.");
//	#endif
//
//	ret = iso_deselect(1);
//	if(ret != 0){
//		#ifdef _EMU_
//		printf("\nErr:iso_deselect,ret=%d",ret);
//		#endif
//	}
//#ifdef _EMU_  
//	printf("\nOK:iso_deselect.");
//#endif
//       	
//	//检查卡是否存在
//	for(i=0;i<MAX_TRY;i++){
//		ret = hl_active(0,REQ_CNT,gThisCardSnr,gBuf);
//		if(ret != 0){
//			#ifdef _EMU_
//			printf("\nErr:hl_active,ret=%d",ret);
//			#endif
//			continue;
//		}
//		#ifdef _EMU_  
//			printf("\n卡号:");
//			for(i=0;i<4;i++) printf("%02X ",(UBYTE)gThisCardSnr[i]);
//		#endif
//		goto label_con;
//	}
//	goto label_err_card_moved;
//
//label_con:
//    #ifndef _EMU_
//      delay_ms(5);
//    #endif
//	ret = iso_rats(1,gBuf);	
//	if(ret) {
//    #ifndef _EMU_
//      delay_ms(5);
//    #endif
//    ret = iso_rats(1,gBuf);
//	}
//	if(ret) {
//    #ifndef _EMU_
//      delay_ms(5);
//    #endif
//    ret = iso_rats(1,gBuf);
//	}		
//	if(ret)  {
//  	   gDebugStep = 0x2141;
//  	   goto label_err_read;
//	}
//  #ifdef _EMU_
//  printf("\niso_rats:");
//  for (i=0;i<16;i++)  printf(" %02x",gBuf[i]);
//  #endif

  //read 0005
  /* //2011-3-30
  memcpy(gBuf,"\x00\xb0\x85\x00\x20",5);
  gBufBytes = 5;
  ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
  if(ret)  {
  	   gDebugStep = 0x2142;
  	   goto label_err_read;
  	   }
  #ifdef _EMU_
  printf("\niso_apdu 发卡方基本信息文件0005:");
  for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  #endif
  if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
  	   gDebugStep = 0x2143;
  	   goto label_err_read;
  	   } 
  */
//    
//	//select file
//	memcpy(gBuf,"\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01\x00",15);
//	gBufBytes = 15;
//	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
//	if(ret)  {
//		gDebugStep = 0x2144;
//		goto label_err_read;
//	}
//	#ifdef _EMU_
//	printf("\niso_apdu select file:");
//	for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//	#endif
//
//	//搜索9f 0C
//	for(ptr=0;ptr<gBufBytes;ptr++){
//	    //if(memcmp(&gBuf[ptr],"\x0c\x1e",2) == 0) goto label_found_9f0c_ok;
//	    if(memcmp(&gBuf[ptr],"\x9f\x0c",2) == 0) 
//	  	{
//	  		ptr = ptr + 3;
//	  		goto label_found_9f0c_ok;
//	  	}
//	}

//	//read 0015
//	memcpy(gBuf,"\x00\xb0\x95\x00\x1e",5);
//	gBufBytes = 5;
//	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
//	if(ret)  {
//		gDebugStep = 0x2094;
//		goto label_err_read;
//	}
//	#ifdef _EMU_
//	printf("\niso_apdu read 0015:");
//	for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//	#endif
//
//	if(((uchar)gBuf[gBufBytes-2]!=(uchar)0x90)||(gBuf[gBufBytes-1]!=0x00))
//	{
//		gDebugStep = 0x2095;
//		goto label_invalid;
//	}
//	ptr = 0;
//
//label_found_9f0c_ok:
//	memcpy(gCityCode,&gBuf[ptr+2],2);//城市代码
//	memcpy(gApplicationSn,&gBuf[ptr+10],10);//应用序列号	10
//	memcpy(gThisCpuCardSnr,&gBuf[ptr+12],8);//卡号,卡唯一代码？
//	memset(gNewACardSnr,0,sizeof(gNewACardSnr));//下次取卡信息时重新计算keya 20090609	   
//	#ifdef _EMU_  
//	printf("\n城市代码: %02x %02x",gCityCode[0],gCityCode[1]);
//	printf("\n卡号 gThisCpuCardSnr:");
//	for(i=0;i<8;i++) printf(" %02X",(UBYTE)gThisCpuCardSnr[i]);
//	#endif
//	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x6a) && (gBuf[gBufBytes-1] == (uchar)0x81))  {
//  	   gDebugStep = 0x2145;
//  	   gResponseStatus = CE_NOFUNCTION;  
//  	   return (char)-1;
//	}
//	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x90) && (gBuf[gBufBytes-1] == 0))  {
//  	   gDebugStep = 0x2146;
//  	   goto label_cpu_modify_m1_consumption;
//	}
//	goto label_err_read;

label_cpu_modify_m1_consumption:
	//get random
	memcpy(gBuf,"\x00\x84\x00\x00\x04",5);
	gBufBytes = 5;
	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
	if(ret)  {
		gDebugStep = 0x2147;
		goto label_err_read;
	}
	#ifdef _EMU_
		printf("\niso_apdu get random:");
		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
	#endif
	memcpy(gRandom,gBuf,4);//	    
	
	//
	sam_set(SAMLOCATION_5,SAM_ETU_93,4);
	sam_select(SAMLOCATION_5);
	sam_atr(SAMLOCATION_5, gBuf,&gBufBytes);
  	#ifdef _EMU_
  		printf("\ntrans_expiredate1->sam_atr:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  	#endif
  		
	//read psam 0016
	memcpy(gBuf,"\x00\xb0\x96\x00\x00",5);
	gBufBytes = 5;
	ret = sam_apdu(SAMLOCATION_5,gBuf,gBufBytes,gBuf,&gBufBytes,0,0);
  	#ifdef _EMU_
  		printf("\ntrans_expiredate1->sam_apdu read 0016:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  	#endif
	//if(((uchar)gBuf[gBufBytes - 2] != (uchar)0x61) || (gBuf[gBufBytes-1] != 0x06))
//	if(((uchar)gBuf[gBufBytes - 2] != (uchar)0x61) && (gBuf[gBufBytes-2] != 0x90))
//	{
//  		gDebugStep = 0x2148;
//		gResponseStatus = CE_PSAMERROR;
//		return (char)-1;
//	}  		

	//get response
	if( (gBufBytes == 2) && (gBuf[0] == 0x61) )
	{
		memcpy(gBuf,"\x00\xc0\x00\x00\x06",5);
		gBufBytes = 5;
		ret = sam_apdu(SAMLOCATION_5,gBuf,gBufBytes,gBuf,&gBufBytes,0,0);
	  	#ifdef _EMU_
	  		printf("\ntrans_expiredate1->sam_apdu read 0016 get response:");
	  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
	  	#endif
	}		    		
		if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0x0))
		{
	  			  	gDebugStep = 0x2149;
	  			  	gResponseStatus = CE_PSAMERROR;
	  			  	return (char)-1;
		}  	
//	memcpy(psam_no,gBuf,6);

	memcpy(gBuf,"\x00\xa4\x00\x00\x02\x10\x01",7);
	gBufBytes = 7;
	ret = sam_apdu(SAMLOCATION_5,gBuf,gBufBytes,gBuf,&gBufBytes,0,0);
  		#ifdef _EMU_
  		printf("\ntrans_expiredate1->sam_apdu select 1001:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  		#endif
	//if(((uchar)gBuf[0] != (uchar)0x61) || (gBuf[1] != 0x28))
//	if(((uchar)gBuf[gBufBytes - 2] != (uchar)0x61) && (gBuf[gBufBytes-2] != 0x90))
//	{
//  			  	gDebugStep = 0x214a;
//  			  	gResponseStatus = CE_PSAMERROR;
//  			  	return (char)-1;
//	}

	//get response
	if( (gBufBytes == 2) && (gBuf[0] == 0x61) )
	{
		memcpy(gBuf,"\x00\xc0\x00\x00\x28",5);
		gBufBytes = 5;
		ret = sam_apdu(SAMLOCATION_5,gBuf,gBufBytes,gBuf,&gBufBytes,0,0);
	  		#ifdef _EMU_
	  		printf("\ntrans_expiredate1->sam_apdu select 1001 get response:");
	  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
	  		#endif
	}
		if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0x0))
		{
	  			  	gDebugStep = 0x214b;
	  			  	gResponseStatus = CE_PSAMERROR;
	  			  	return (char)-1;
		}


	memcpy(gBuf,"\x00\xb0\x97\x00\x01",5);
	gBufBytes = 5;
	ret = sam_apdu(SAMLOCATION_5,gBuf,gBufBytes,gBuf,&gBufBytes,0,0);
  	#ifdef _EMU_
  		printf("\ntrans_expiredate1->sam_apdu read 0017:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  	#endif
	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0x0))
	{
  			  	gDebugStep = 0x214c;
  			  	gResponseStatus = CE_PSAMERROR;
  			  	return (char)-1;
	}
	gKeyVersion = gBuf[0];	//密钥版本号
	
	memcpy(gBuf,"\x80\x1A\x45\x01\x10",5);
	memcpy(&gBuf[5],gThisCpuCardSnr,8);
	memcpy(&gBuf[13],gCityCode,2);
	memcpy(&gBuf[15],"\xFF\x00\x00\x00\x00\x00",6);
	gBufBytes = 21;
	ret = sam_apdu(SAMLOCATION_5,gBuf,gBufBytes,gBuf,&gBufBytes,0,0);
  	#ifdef _EMU_
  		printf("\ntrans_expiredate1->sam_apdu cal mac step1:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  	#endif
	if(((uchar)gBuf[0] != (uchar)0x90) || (gBuf[1] != 0x0))
	{
  			  	gDebugStep = 0x214d;
  			  	gResponseStatus = CE_PSAMERROR;
  			  	return (char)-1;
	}  		
  		
	memcpy(gBuf,"\x80\xfA\x05\x00\x10",5);
	memcpy(&gBuf[5],gRandom,4);
	//memcpy(&gBuf[9],"\x00\x00\x00\x00\x84\x18\x00\x00\x04\x80\x00\x00",12);
	memcpy(&gBuf[9],"\x00\x00\x00\x00\x04\xD6\x95\x08\x05\x10\x80\x00",12);//20090604 according to a email from feng_man
	gBuf[18] = (uchar)(gApplicationTypeTag | (uchar)0x80);
	gBufBytes = 21;
	ret = sam_apdu(SAMLOCATION_5,gBuf,gBufBytes,gBuf,&gBufBytes,0,0);
  		#ifdef _EMU_
  		printf("\ntrans_expiredate1->sam_apdu cal mac step2:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  		#endif
//	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x61) || (gBuf[gBufBytes-1] != 0x04))
//	{
//  			  	gDebugStep = 0x214e;
//  			  	gResponseStatus = CE_PSAMERROR;
//  			  	return (char)-1;
//	}
	//get response
	if( (gBufBytes == 2) && (gBuf[0] == 0x61) )
	{
		memcpy(gBuf,"\x00\xc0\x00\x00\x04",5);
		gBufBytes = 5;
		ret = sam_apdu(SAMLOCATION_5,gBuf,gBufBytes,gBuf,&gBufBytes,0,0);
	  		#ifdef _EMU_
	  		printf("\ntrans_expiredate1->sam_apdu cal mac step2 get response:");
	  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
	  		#endif
	}
		if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0x0))
		{
	  			  	gDebugStep = 0x214f;
	  			  	gResponseStatus = CE_PSAMERROR;
	  			  	return (char)-1;
		}
	memcpy(gMAC,gBuf,4);

label_online_cpu_modify_m1_consumption:
	
	#ifdef _EMU_
  printf("\ntrans_expiredate1 now-->0015.8-->|0x80 online.");
  printf("\ngApplicationTypeTag = %02x",gApplicationTypeTag);
  #endif
	       	
	//modify m1 consumption status
	memcpy(gBuf,"\x04\xd6\x95\x08\x05",5);//修改应用类型标识
	gBuf[5] = (uchar)(gApplicationTypeTag | (uchar)0x80);
	memcpy(&gBuf[6],gMAC,4);
	gBufBytes = 10;
	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
	if(ret)  {
  	   gDebugStep = 0x2150;
  	   gResponseStatus = CE_CARDREMOVED;  
  	   return (char)-1;
	}
  #ifdef _EMU_
  printf("\niso_apdu trans_expiredate1-->modify m1 consumption:");
  for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  #endif
	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x69) && (gBuf[gBufBytes-1] == (uchar)0x88))  {
  	   gDebugStep = 0x2b51;
  	   gResponseStatus = CE_MAC2ERR;  
  	   return (char)-1;
	}
	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
  	   gDebugStep = 0x2951;
  	   gResponseStatus = CE_WRITEERROR;  
  	   return (char)-1;
	}
  	   	
	gDebugStep = 0x2d51;
	return 0;

label_err_read:
	gResponseStatus = CE_READERROR;
	return (char) -1;

label_invalid:
	gResponseStatus = CE_INVALIDCARD;
#ifdef _EMU_
printf("\nError: card invalid.");
#endif
	return (char)-1;

label_err_card_moved:
	gResponseStatus = CE_CARDREMOVED;
	return (char) -1;
}


char trans_forbid_mifare(void)
{
char ret;
char i;
unsigned char ptr;


#ifdef _EMU_
printf("\ntrans_forbid mifare now.");
#endif

	gDebugStep = 0x2140;
	if(memcmp(gMAC,"\x0\x0\x0\x0",4) != 0)  goto label_online_cpu_forbid_mifare;//联机禁止mifare钱包

	//脱机禁止mifare钱包
	ret = iso_deselect(1);
	if(ret != 0){
		#ifdef _EMU_
		printf("\nErr:iso_deselect,ret=%d",ret);
		#endif
	}
#ifdef _EMU_  
printf("\nOK:iso_deselect.");
#endif

	//检查卡是否存在
	for(i=0;i<MAX_TRY;i++){
		ret = hl_active(0,REQ_CNT,gThisCardSnr,gBuf);
		if(ret != 0){
			#ifdef _EMU_
			printf("\nErr:hl_active,ret=%d",ret);
			#endif
	  		continue;
		}
#ifdef _EMU_  
printf("\n卡号:");
for(i=0;i<4;i++) printf("%02X ",(UBYTE)gThisCardSnr[i]);
#endif
		goto label_con;
	}
	goto label_err_card_moved;

label_con:
    #ifndef _EMU_
      delay_ms(5);
    #endif
	ret = iso_rats(1,gBuf);	
	if(ret) {
    #ifndef _EMU_
      delay_ms(5);
    #endif
		ret = iso_rats(1,gBuf);
	}
	if(ret) {
    #ifndef _EMU_
      delay_ms(5);
    #endif
		ret = iso_rats(1,gBuf);
	}		
	if(ret)  {
  	   gDebugStep = 0x2141;
  	   goto label_err_read;
	}
  #ifdef _EMU_
  printf("\niso_rats:");
  for (i=0;i<16;i++)  printf(" %02x",gBuf[i]);
  #endif

  //read 0005
  /* //2011-3-30
  memcpy(gBuf,"\x00\xb0\x85\x00\x20",5);
  gBufBytes = 5;
  ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
  if(ret)  {
  	   gDebugStep = 0x2142;
  	   goto label_err_read;
  	   }
  #ifdef _EMU_
  printf("\niso_apdu 发卡方基本信息文件0005:");
  for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  #endif
  if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
  	   gDebugStep = 0x2143;
  	   goto label_err_read;
  	   } 
  */

	//select file
	memcpy(gBuf,"\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01\x00",15);
	gBufBytes = 15;
	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
	if(ret)  {
  	   gDebugStep = 0x2144;
  	   goto label_err_read;
	}
	#ifdef _EMU_
	printf("\niso_apdu select file:");
	for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
	#endif

	//搜索9f 0C
	for(ptr=0;ptr<gBufBytes;ptr++){
	    //if(memcmp(&gBuf[ptr],"\x0c\x1e",2) == 0) goto label_found_9f0c_ok;
		if(memcmp(&gBuf[ptr],"\x9f\x0c",2) == 0) 
		{
	  		ptr = ptr + 3;
	  		goto label_found_9f0c_ok;
		}
	}

	//read 0015
	memcpy(gBuf,"\x00\xb0\x95\x00\x1e",5);
	gBufBytes = 5;
	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
	if(ret)  {
		gDebugStep = 0x2094;
		goto label_err_read;
	}
	#ifdef _EMU_
	printf("\niso_apdu read 0015:");
	for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
	#endif

	if(((uchar)gBuf[gBufBytes-2]!=(uchar)0x90)||(gBuf[gBufBytes-1]!=0x00))
	{
		gDebugStep = 0x2095;
		goto label_invalid;
	}
	ptr = 0;

label_found_9f0c_ok:
	memcpy(gCityCode,&gBuf[ptr+2],2);//城市代码
	memcpy(gApplicationSn,&gBuf[ptr+10],10);//应用序列号	10
	memcpy(gThisCpuCardSnr,&gBuf[ptr+12],8);//卡号,卡唯一代码？
	memset(gNewACardSnr,0,sizeof(gNewACardSnr));//下次取卡信息时重新计算keya 20090609	   
	#ifdef _EMU_  
	printf("\n城市代码: %02x %02x",gCityCode[0],gCityCode[1]);
	printf("\n卡号 gThisCpuCardSnr:");
	for(i=0;i<8;i++) printf(" %02X",(UBYTE)gThisCpuCardSnr[i]);
	#endif
	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x6a) && (gBuf[gBufBytes-1] == (uchar)0x81))  {
  	   gDebugStep = 0x2145;
  	   gResponseStatus = CE_NOFUNCTION;  
  	   return (char)-1;
	}
	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x90) && (gBuf[gBufBytes-1] == 0))  {
  	   gDebugStep = 0x2946;
  	   goto label_cpu_forbid_mifare;
	}
	goto label_err_read;

label_cpu_forbid_mifare:
	//get random
	memcpy(gBuf,"\x00\x84\x00\x00\x04",5);
	gBufBytes = 5;
	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
	if(ret)  {
		gDebugStep = 0x2147;
		goto label_err_read;
	}
	#ifdef _EMU_
	printf("\niso_apdu get random:");
	for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
	#endif
	memcpy(gRandom,gBuf,4);//	    
	
	sam_set(SAMLOCATION_5,SAM_ETU_93, 4);
	sam_select(SAMLOCATION_5);
	sam_atr(SAMLOCATION_5, gBuf,&gBufBytes);
  		#ifdef _EMU_
  		printf("\ntrans_forbid_mifare->sam_atr:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  		#endif
  		
	//read psam 0016
	memcpy(gBuf,"\x00\xb0\x96\x00\x00",5);
	gBufBytes = 5;
	ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes, 0,0);
  		#ifdef _EMU_
  		printf("\ntrans_forbid_mifare->sam_apdu read 0016:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  		#endif
//	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x61) || (gBuf[gBufBytes-1] != 0x06))
//	{
//		gDebugStep = 0x2148;
//		gResponseStatus = CE_PSAMERROR;
//		return (char)-1;
//	}  		

	//get response
	if( (gBufBytes == 2) && (gBuf[0] == 0x61) )
	{
		memcpy(gBuf,"\x00\xc0\x00\x00\x06",5);
		gBufBytes = 5;
		ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes, 0,0);
	  		#ifdef _EMU_
	  		printf("\ntrans_forbid_mifare->sam_apdu read 0016 get response:");
	  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
	  		#endif
	}
		if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0x0))
		{
	  			  	gDebugStep = 0x2149;
	  			  	gResponseStatus = CE_PSAMERROR;
			return (char)-1;
		}  			    		
//	memcpy(psam_no,gBuf,6);


	memcpy(gBuf,"\x00\xa4\x00\x00\x02\x10\x01",7);
	gBufBytes = 7;
	ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes, 0,0);
  		#ifdef _EMU_
  		printf("\ntrans_forbid_mifare->sam_apdu select 1001:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  		#endif
//	if(((uchar)gBuf[0] != (uchar)0x61) || (gBuf[1] != 0x28))
//	{
//  			  	gDebugStep = 0x214a;
//  			  	gResponseStatus = CE_PSAMERROR;
//  			  	return (char)-1;
//	}

	//get response
	if( (gBufBytes == 2) && (gBuf[0] == 0x61) )
	{
		memcpy(gBuf,"\x00\xc0\x00\x00\x28",5);
		gBufBytes = 5;
		ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes, 0,0);
	  		#ifdef _EMU_
	  		printf("\ntrans_forbid_mifare->sam_apdu select 1001 get response:");
	  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
	  		#endif
	}
		if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0x0))
		{
	  			  	gDebugStep = 0x214b;
	  			  	gResponseStatus = CE_PSAMERROR;
	  			  	return (char)-1;
		}  			    		


	memcpy(gBuf,"\x00\xb0\x97\x00\x01",5);
	gBufBytes = 5;
	ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes, 0,0);
  		#ifdef _EMU_
  		printf("\ntrans_forbid_mifare->sam_apdu read 0017:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  		#endif
	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0x0))
	{
  			  	gDebugStep = 0x214c;
  			  	gResponseStatus = CE_PSAMERROR;
  			  	return (char)-1;
	}
	gKeyVersion = gBuf[0];	//密钥版本号
  		
	memcpy(gBuf,"\x80\x1A\x45\x01\x10",5);
	memcpy(&gBuf[5],gThisCpuCardSnr,8);
	memcpy(&gBuf[13],gCityCode,2);
	memcpy(&gBuf[15],"\xFF\x00\x00\x00\x00\x00",6);
	gBufBytes = 21;
	ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes, 0,0);
  		#ifdef _EMU_
  		printf("\ntrans_forbid_mifare->sam_apdu cal mac step1:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  		#endif
	if(((uchar)gBuf[0] != (uchar)0x90) || (gBuf[1] != 0x0))
	{
  			  	gDebugStep = 0x214d;
  			  	gResponseStatus = CE_PSAMERROR;
  			  	return (char)-1;
	}  		
  		
	memcpy(gBuf,"\x80\xfA\x05\x00\x10",5);
	memcpy(&gBuf[5],gRandom,4);
	memcpy(&gBuf[9],"\x00\x00\x00\x00\x84\xcd\x00\x00\x04\x80\x00\x00",12);
	gBufBytes = 21;
	ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes, 0,0);
  		#ifdef _EMU_
  		printf("\ntrans_forbid_mifare->sam_apdu cal mac step2:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  		#endif
//	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x61) || (gBuf[gBufBytes-1] != 0x04))
//	{
//  			  	gDebugStep = 0x214e;
//  			  	gResponseStatus = CE_PSAMERROR;
//  			  	return (char)-1;
//	}
	//get response
	if( (gBufBytes == 2) && (gBuf[0] == 0x61) )
	{
		memcpy(gBuf,"\x00\xc0\x00\x00\x04",5);
		gBufBytes = 5;
		ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes, 0,0);
	  		#ifdef _EMU_
	  		printf("\ntrans_forbid_mifare->sam_apdu cal mac step2 get response:");
	  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
	  		#endif
	}
		if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0x0))
		{
	  			  	gDebugStep = 0x214f;
	  			  	gResponseStatus = CE_PSAMERROR;
	  			  	return (char)-1;
		}  			    		
	memcpy(gMAC,gBuf,4);

label_online_cpu_forbid_mifare:       	
	//forbid mifare1
	memcpy(gBuf,"\x84\xcd\x00\x00\x04",5);
	memcpy(&gBuf[5],gMAC,4);
	gBufBytes = 9;
	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
	if(ret)  {
  	   gDebugStep = 0x2150;
  	   gResponseStatus = CE_CARDREMOVED;  
  	   return (char)-1;
	}
  #ifdef _EMU_
  printf("\niso_apdu forbid mifare1:");
  for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  #endif
	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x65) && (gBuf[gBufBytes-1] == (uchar)0x81))  {
  	   gDebugStep = 0x2151;
  	   gResponseStatus = CE_WRITEERROR;  
		return (char)-1;
	}
	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x69) && (gBuf[gBufBytes-1] == (uchar)0x82))  {
  	   gDebugStep = 0x8151;
  	   gResponseStatus = CE_MAC2ERR;  
  	   return (char)-1;
	}
	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x69) && (gBuf[gBufBytes-1] == (uchar)0x88))  {
  	   gDebugStep = 0x9151;
  	   gResponseStatus = CE_MAC2ERR;  
  	   return (char)-1;
	}
	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
  	   gDebugStep = 0xa151;
  	   gResponseStatus = CE_UNKNOWN;  
  	   return (char)-1;
	}	

	return 0;

label_err_read:
	gResponseStatus = CE_READERROR;
	return (char) -1;

label_invalid:
	gResponseStatus = CE_INVALIDCARD;
	#ifdef _EMU_
	printf("\nError: card invalid.");
	#endif
	return (char)-1;

label_err_card_moved:
	gResponseStatus = CE_CARDREMOVED;
	return (char) -1;	
}



char trans_cpu_lock(char lock_type)
{
char ret;
int i;
unsigned char ptr;

#ifdef _EMU_
printf("\ntrans_cpu_lock now.");
#endif

	
	gDebugStep = 0x2120;
	if(memcmp(gMAC,"\x0\x0\x0\x0",4) != 0)  goto label_online_cpu_lock;//联机锁卡
	
	//脱机锁卡
	ret = iso_deselect(1);
	if(ret != 0){
		#ifdef _EMU_
		printf("\nErr:iso_deselect,ret=%d",ret);
		#endif
	}
#ifdef _EMU_  
printf("\nOK:iso_deselect.");
#endif

	//检查卡是否存在
	for(i=0;i<MAX_TRY;i++){
		ret = hl_active(0,REQ_CNT,gThisCardSnr,gBuf);
		if(ret != 0){
			#ifdef _EMU_
			printf("\nErr:hl_active,ret=%d",ret);
			#endif
		  	continue;
		}
#ifdef _EMU_  
printf("\n卡号:");
for(i=0;i<4;i++) printf("%02X ",(UBYTE)gThisCardSnr[i]);
#endif
		goto label_con;
	}
	goto label_err_card_moved;

label_con:
    #ifndef _EMU_
      delay_ms(5);
    #endif
	ret = iso_rats(1,gBuf);	
	if(ret) {
    #ifndef _EMU_
      delay_ms(5);
    #endif
    ret = iso_rats(1,gBuf);
	}
	if(ret) {
    #ifndef _EMU_
      delay_ms(5);
    #endif
		ret = iso_rats(1,gBuf);
	}		
	if(ret)  {
  	   gDebugStep = 0x2121;
  	   goto label_err_read;
	}
  #ifdef _EMU_
  printf("\niso_rats:");
  for (i=0;i<16;i++)  printf(" %02x",gBuf[i]);
  #endif

  //read 0005
  /* //2011-3-30
  memcpy(gBuf,"\x00\xb0\x85\x00\x20",5);
  gBufBytes = 5;
  ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
  if(ret)  {
  	   gDebugStep = 0x2122;
  	   goto label_err_read;
  	   }
  #ifdef _EMU_
  printf("\niso_apdu 发卡方基本信息文件0005:");
  for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  #endif
  if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
  	   gDebugStep = 0x2123;
  	   goto label_err_read;
  	   } 
  */

	//select file
	memcpy(gBuf,"\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01\x00",15);
	gBufBytes = 15;
	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
	if(ret)  {
		gDebugStep = 0x2124;
		goto label_err_read;
	}
	#ifdef _EMU_
	printf("\niso_apdu select file:");
	for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
	#endif

	//搜索9f 0C
	for(ptr=0;ptr<gBufBytes;ptr++){
	    //if(memcmp(&gBuf[ptr],"\x0c\x1e",2) == 0) goto label_found_9f0c_ok;
	    if(memcmp(&gBuf[ptr],"\x9f\x0c",2) == 0) 
	  	{
	  		ptr = ptr + 3;
	  		goto label_found_9f0c_ok;
	  	}
	}

	//read 0015
	memcpy(gBuf,"\x00\xb0\x95\x00\x1e",5);
	gBufBytes = 5;
	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
	if(ret)  {
		gDebugStep = 0x2094;
		goto label_err_read;
	}
	#ifdef _EMU_
	printf("\niso_apdu read 0015:");
	for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
	#endif

	if(((uchar)gBuf[gBufBytes-2]!=(uchar)0x90)||(gBuf[gBufBytes-1]!=0x00))
	{
		gDebugStep = 0x2095;
		goto label_invalid;
	}
	ptr = 0;

label_found_9f0c_ok:
	memcpy(gCityCode,&gBuf[ptr+2],2);//城市代码
	memcpy(gApplicationSn,&gBuf[ptr+10],10);//应用序列号	10
	memcpy(gThisCpuCardSnr,&gBuf[ptr+12],8);//卡号,卡唯一代码？
	memset(gNewACardSnr,0,sizeof(gNewACardSnr));//下次取卡信息时重新计算keya 20090609	   
	#ifdef _EMU_  
	printf("\n城市代码: %02x %02x",gCityCode[0],gCityCode[1]);
	printf("\n卡号 gThisCpuCardSnr:");
	for(i=0;i<8;i++) printf(" %02X",(UBYTE)gThisCpuCardSnr[i]);
	#endif
	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x6a) && (gBuf[gBufBytes-1] == (uchar)0x81))  {
		gDebugStep = 0x2125;
		gResponseStatus = CE_NOFUNCTION;  
		return (char)-1;
	}
	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x90) && (gBuf[gBufBytes-1] == 0))  {
		gDebugStep = 0x2126;
		goto label_cpu_lock;
	}
	goto label_err_read;

label_cpu_lock:
	//get random
	memcpy(gBuf,"\x00\x84\x00\x00\x04",5);
	gBufBytes = 5;
	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
	if(ret)  {
		gDebugStep = 0x2127;
		goto label_err_read;
	}
	#ifdef _EMU_
	printf("\niso_apdu get random:");
	for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
	#endif
	memcpy(gRandom,gBuf,4);//	    
	
	sam_set(SAMLOCATION_5,SAM_ETU_93, 4);
	sam_select(SAMLOCATION_5);
	sam_atr(SAMLOCATION_5, gBuf,&gBufBytes);
  		#ifdef _EMU_
  		printf("\ncpu lock->sam_atr:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  		#endif
  		
	//read psam 0016
	memcpy(gBuf,"\x00\xb0\x96\x00\x00",5);
	gBufBytes = 5;
	ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes, 0,0);
  		#ifdef _EMU_
  		printf("\ncpu lock->sam_apdu read 0016:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  		#endif
//	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x61) || (gBuf[gBufBytes-1] != 0x06))
//	{
//		gDebugStep = 0x2130;
//		gResponseStatus = CE_PSAMERROR;
//		return (char)-1;
//	}  		

	//get response
	if( (gBufBytes == 2) && (gBuf[0] == 0x61) )
	{
		memcpy(gBuf,"\x00\xc0\x00\x00\x06",5);
		gBufBytes = 5;
		ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes,0,0);
	  		#ifdef _EMU_
	  		printf("\ncpu lock->sam_apdu read 0016 get response:");
	  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
	  		#endif
	}
		if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0x0))
		{
			gDebugStep = 0x2131;
			gResponseStatus = CE_PSAMERROR;
			return (char)-1;
		}  			    		
//	memcpy(psam_no,gBuf,6);


	memcpy(gBuf,"\x00\xa4\x00\x00\x02\x10\x01",7);
	gBufBytes = 7;
	ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes,0,0);
  		#ifdef _EMU_
  		printf("\ncpu lock->sam_apdu select 1001:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  		#endif
//	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x61) || (gBuf[gBufBytes-1] != 0x28))
//	{
//		gDebugStep = 0x2132;
//		gResponseStatus = CE_PSAMERROR;
//		return (char)-1;
//	}

	//get response
	if( (gBufBytes == 2) && (gBuf[0] == 0x61) )
	{
		memcpy(gBuf,"\x00\xc0\x00\x00\x28",5);
		gBufBytes = 5;
		ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes,0,0);
	  		#ifdef _EMU_
	  		printf("\ncpu lock->sam_apdu select 1001 get response:");
	  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
	  		#endif
	}
		if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0x0))
		{
			gDebugStep = 0x2133;
			gResponseStatus = CE_PSAMERROR;
			return (char)-1;
		}  			    		


	memcpy(gBuf,"\x00\xb0\x97\x00\x01",5);
	gBufBytes = 5;
	ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes,0,0);
  		#ifdef _EMU_
  		printf("\ncpu lock->sam_apdu read 0017:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  		#endif
	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0x0))
	{
		gDebugStep = 0x2134;
		gResponseStatus = CE_PSAMERROR;
		return (char)-1;
	}
	gKeyVersion = gBuf[0];	//密钥版本号
  		
	memcpy(gBuf,"\x80\x1A\x45\x01\x10",5);
	memcpy(&gBuf[5],gThisCpuCardSnr,8);
	memcpy(&gBuf[13],gCityCode,2);
	memcpy(&gBuf[15],"\xFF\x00\x00\x00\x00\x00",6);
	gBufBytes = 21;
	ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes,0,0);
  		#ifdef _EMU_
  		printf("\ncpu lock->sam_apdu cal mac step1:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  		#endif
	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0x0))
	{
		gDebugStep = 0x2135;
		gResponseStatus = CE_PSAMERROR;
		return (char)-1;
	}  		
  		
	memcpy(gBuf,"\x80\xfA\x05\x00\x10",5);
	memcpy(&gBuf[5],gRandom,4);
	memcpy(&gBuf[9],"\x00\x00\x00\x00\x84\x1e\x00\x00\x04\x80\x00\x00",12);
	if(lock_type == 0x1) memcpy(&gBuf[9],"\x00\x00\x00\x00\x84\x1e\x00\x01\x04\x80\x00\x00",12);
	gBufBytes = 21;
	ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes,0,0);
  		#ifdef _EMU_
  		printf("\ncpu lock->sam_apdu cal mac step2:");
  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
  		#endif
//	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x61) || (gBuf[gBufBytes-1] != 0x04))
//	{
//		gDebugStep = 0x2136;
//		gResponseStatus = CE_PSAMERROR;
//		return (char)-1;
//	}
	//get response
	if( (gBufBytes == 2) && (gBuf[0] == 0x61) )
	{
		memcpy(gBuf,"\x00\xc0\x00\x00\x04",5);
		gBufBytes = 5;
		ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes, 0,0);
	  		#ifdef _EMU_
	  		printf("\ncpu app unlock->sam_apdu cal mac step2 get response:");
	  		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
	  		#endif
	}
		if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0x0))
		{
			gDebugStep = 0x2137;
			gResponseStatus = CE_PSAMERROR;
			return (char)-1;
		}  			    		
	memcpy(gMAC,gBuf,4);


//online cpu lock
label_online_cpu_lock:  
	memcpy(gBuf,"\x84\x1e\x00\x01\x04",5);
	memcpy(&gBuf[5],gMAC,4);
	gBufBytes = 9;
	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
	if(ret)  {
  	   gDebugStep = 0x2138;
  	   goto label_err_read;
	}
	#ifdef _EMU_
	printf("\niso_apdu app unlock:");
	for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
	#endif
	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
		gDebugStep = 0x2139;
		goto label_err_read;
	}
  	   
	return 0;

label_err_read:
	gResponseStatus = CE_READERROR;
	return (char) -1;

label_invalid:
	gResponseStatus = CE_INVALIDCARD;
	#ifdef _EMU_
	printf("\nError: card invalid.");
	#endif
	return (char)-1;

label_err_card_moved:
	gResponseStatus = CE_CARDREMOVED;
	return (char) -1;	 	   
}


/***************************************************************************************
函数：trans_add
功能：加款交易
****************************************************************************************/
char trans_add(void)
{
unsigned char  buf[200];
unsigned char  bufagain[200];
unsigned char buf0[200],buf1[200],buf2[200];
//unsigned long  tlong;
uchar i;
uint tint;
uchar ret;

#ifdef _EMU_
printf("\ntrans_add now.");
#endif

	//检查卡是否存在
	gDebugStep = 0x1501;   
	for(i=0;i<MAX_TRY;i++){
		ret = hl_active(0,REQ_CNT,gThisCardSnr,gBuf);
		if(ret != 0){
			#ifdef _EMU_
				printf("\nErr:hl_active,ret=%d",ret);
			#endif
			continue;
		}
		#ifdef _EMU_  
			printf("\n卡号:");
			for(i=0;i<4;i++) printf("%02X ",(UBYTE)gThisCardSnr[i]);
			printf("\nTagType[0],TagType[1] and Size:");
			for(i=0;i<3;i++) printf("%02X ",(UBYTE)gBuf[i]);
		#endif
		goto label_begin;
	}
	gDebugStep = 0x1502;
	gResponseStatus = CE_CARDREMOVED;
	goto label_err_get_transresult; 

label_begin:
	gDebugStep = 0x1503;	
	
	//获取B9,B10
	gDebugStep = 0x1504;
	ret = hl_auth(AUTH_MODE_INT_KEYA,2,(UBYTE *)"\x0\x0\x0\x0\x0\x0");	 
	if(ret){
		#ifdef _EMU_
			printf("\nErr:hl_auth,ret=%d",ret);
		#endif
		goto label_err_get_transresult; 
	}
	gDebugStep = 0x1505;       	
	if(mcml_read(9,gB9) != 0) goto label_err_get_transresult; 
	gDebugStep = 0x1506;
	if(mcml_read(10,gB10) != 0) goto label_err_get_transresult; 
	gDebugStep = 0x1507;
	if(trans_setup_bakup() != 0) goto label_err_get_transresult; 

	//写卡上的加款私有记录
	//形成B9
	gDebugStep = 0x1508;
	for(i=0;i<5;i++) buf1[i] = gTransTime[1+i];
	//本次加款金额
	buf1[5] = *((char *)&gCardThisTransValue+0);
	buf1[6] = *((char *)&gCardThisTransValue+1);
	buf1[7] = *((char *)&gCardThisTransValue+2);
	
	//卡前次剩余金额
	buf1[8] = *((char *)&gCardLastRemain+0);
	buf1[9] = *((char *)&gCardLastRemain+1);
	i = *((char *)&gCardLastRemain+3);
	if((unsigned char)i > 127) buf1[10] = 0xff;
	else buf1[10] = *((char *)&gCardLastRemain+2);

	//卡本次剩余金额
	gCardThisRemain = gCardLastRemain+gCardThisTransValue;
	
	buf1[11] = *((char *)&gCardThisRemain+0); 
	buf1[12] = *((char *)&gCardThisRemain+1); 
	i = *((char *)&gCardThisRemain+3);
	if((uchar)i > (uchar)127) buf1[13] = 0xff;
	else buf1[13] = *((char *)&gCardThisRemain+2);
	
	//卡计数器
	buf1[14] = *((char *)&gCardCnt+1);
	buf1[15] = *((char *)&gCardCnt+0);
	
	//计算TAC
	gDebugStep = 0x15a7;
	buf0[0] = 0x0;//交易类型标志	1
	buf0[1] = *((char *)&gPosTransCnt+1);//终端交易流水号	3
	buf0[2] = *((char *)&gPosTransCnt+2);
	buf0[3] = *((char *)&gPosTransCnt+3);
	//memcpy(&buf0[4],gCityCode,2);//城市代码	2
	buf0[4] = gCityCode0;
	buf0[5] = gCityCode1;
	memcpy(&buf0[6],gThisCardSnr,4);//卡唯一代码	4
	buf0[10] = gCardType;//卡类型	1
	buf0[11] = *((char *)&gCardLastRemain+0);//交易前余额	3
	buf0[12] = *((char *)&gCardLastRemain+1);
	buf0[13] = *((char *)&gCardLastRemain+2);//msb
	
	buf0[14] = *((char *)&gCardThisTransValue+0);//交易金额	3
	buf0[15] = *((char *)&gCardThisTransValue+1);
	buf0[16] = *((char *)&gCardThisTransValue+2);//msb
	
	memcpy(&buf0[17],gTransTime,7);//交易日期	4 交易时间	3
	
	buf0[24] = *((char *)&gCardCnt+1);//卡交易计数器	2
	buf0[25] = *((char *)&gCardCnt+0);
	memcpy(&buf0[26],&FM_Module_NO[2],4);//更新：SAM卡号	4-->充资：FM模块号
	memcpy(&buf0[30],"\x80\x00",2);
	#ifdef _EMU_
		printf("\nhl_cal_tac-->buf0:");
		for(i=0;i<32;i++) printf(" %02x",buf0[i]);
	#endif
	ret = hl_cal_tac(0x0A,buf0,32, gTAC);//MAC
	if(ret)  {
		gDebugStep = 0x1518; 
		#ifdef _EMU_
			printf("\nErr:hl_cal_tac,ret=%d",ret);
		#endif
		goto label_err_get_transresult;   	    
	}
	#ifdef _EMU_
		printf("\nhl_cal_tac-->tac:");
		for(i=0;i<4;i++) printf(" %02x",gTAC[i]);
	#endif

	buf2[0] = *((char *)&gPosNo+1);
	buf2[1] = *((char *)&gPosNo+2);
	buf2[2] = 0x18;
	buf2[3] = *((char *)&gPosNo+3);
	
	
	for(i=0;i<4;i++){
		buf2[4+i] = FM_Module_NO[2+i];
		buf2[8+i] = gTAC[i];//
		buf2[12+i] = 0;
	}

	//写卡上公共记录
	trans_gen_card_record(0x99, gCardRecord);//卡上加款记录
	gDebugStep = 0x1509;
	ret = hl_auth(AUTH_MODE_INT_KEYA,gCardRecPos[gCardRecPtr]/4,(UBYTE *)"\x0\x0\x0\x0\x0\x0");	 
	if(ret){
		#ifdef _EMU_
			printf("\nErr:hl_auth,ret=%d",ret);
		#endif
		goto label_err_get_transresult; 
	}
	gDebugStep = 0x150a;       	
	if(mcml_read(gCardRecPos[gCardRecPtr],buf) != 0) goto label_err_get_transresult; 
	gDebugStep = 0x150b;
	if(mcml_write(gCardRecPos[gCardRecPtr],gCardRecord) != 0) goto label_err_get_transresult; 
	memcpy(gCardRecord,buf,16);

	//写B13
	for(i=0;i<11;i++) buf[i] = 0;
	i = (gCardRecPtr+1)%10;
	buf[11] = gCardStatus;
	buf[12] = i;
	tint = gCardCnt+1;
	
	buf[13] = *((char *)&tint+1);
	buf[14] = *((char *)&tint+0);
	/* //cut in 2012-01-10
	memcpy(&buf[7],CntCardFirstUse,4);
	CntCardTotal[0]++;
	if(CntCardTotal[0]==0) CntCardTotal[1]++;
	memcpy(&buf[5],CntCardTotal,2);
	*/
	lrc_gen2(buf,15);
	gDebugStep = 0x150c;
	ret = hl_auth(AUTH_MODE_INT_KEYA,3,(UBYTE *)"\x0\x0\x0\x0\x0\x0");	 
	if(ret){
		#ifdef _EMU_
			printf("\nErr:hl_auth,ret=%d",ret);
		#endif
		goto label_err_get_transresult; 
	}
	gDebugStep = 0x150d;
	if(mcml_write(13,buf) != 0) goto label_err_get_transresult; 
	//回读，20110519
	gDebugStep = 0x15d0;
	if(mcml_read(13,bufagain) != 0) goto label_err_get_transresult;
	gDebugStep = 0x15d1;
	if(memcmp(buf,bufagain,16) != 0) goto label_err_get_transresult;
	
	//清除B12
	gDebugStep = 0x150e;
	for(i=0;i<16;i++) buf[i]=0;
	ret = hl_auth(AUTH_MODE_INT_KEYA,3,(UBYTE *)"\x0\x0\x0\x0\x0\x0");	 
	if(ret){
		#ifdef _EMU_
		printf("\nErr:hl_auth,ret=%d",ret);
		#endif
		goto label_err_get_transresult; 
	}
	gDebugStep = 0x150f;
	if(mcml_write(12,buf) != 0) goto label_err_get_transresult; 
	//回读，20110519
	gDebugStep = 0x15d2;
	if(mcml_read(12,bufagain) != 0) goto label_err_get_transresult;
	gDebugStep = 0x15d3;
	if(memcmp(buf,bufagain,16) != 0) goto label_err_get_transresult;
	
	//建立充资区备份
	fail_add_add();
	
	//更新卡上充资区
	gDebugStep = 0x1510;
	
	//密文方式 密钥加载
	gDebugStep = 0x1513;
	memcpy(buf0,gM1AddValBlock,16);
	memcpy(&buf0[16],gAddValMAC,4); 
	memset(gAddValMAC,0xff,6);
	ret = mcml_load_key_miwen_mode(gM1AddValKey);	 
	if(ret){
		#ifdef _EMU_
		printf("\nErr:mcml_load_key_miwen_mode,ret=%d",ret);
		#endif
		goto label_err_get_transresult; 
	}
	
	gDebugStep = 0x1511;
	if(mcml_write(10,buf2) != 0) goto label_err_get_transresult;
	//回读，20110519
	gDebugStep = 0x15d3;
	if(mcml_read(10,bufagain) != 0) goto label_err_get_transresult;
	gDebugStep = 0x15d4;
	if(memcmp(buf2,bufagain,16) != 0) goto label_err_get_transresult;
	
	gDebugStep = 0x1512;
	if(mcml_write(9,buf1) != 0) goto label_err_get_transresult;
	//回读，20110519
	gDebugStep = 0x15d5;
	if(mcml_read(9,bufagain) != 0) goto label_err_get_transresult;
	gDebugStep = 0x15d6;
	if(memcmp(buf1,bufagain,16) != 0) goto label_err_get_transresult;
	
	//MAC保护方式更新数据块8
	gDebugStep = 0x1514;    	    
	if(mcml_write_mac_mode(8,buf0,gAddValMAC) != 0) goto label_err_get_transresult;
	//回读，20110519
	gDebugStep = 0x15d7;
	if(mcml_read(8,bufagain) != 0) goto label_err_get_transresult;
	gDebugStep = 0x15d8;
	if(memcmp(buf0,bufagain,16) != 0) goto label_err_get_transresult;
	
	//取交易结果指令获取交易结果TAC码
label_err_get_transresult:
	memset(gAddValMAC,0xff,6);
	memset(gNewACardSnr,0,sizeof(gNewACardSnr));//下次取卡信息时重新计算keya，否则取卡信息有问题？20090521	
	ret = hl_get_trans_result(&gTransTime[1],gAddValMAC);//MAC--协议中所谓的“模块响应MAC”
	if(ret)  {
		#ifdef _EMU_
			printf("\nErr:hl_get_trans_result,ret=%d",ret);
		#endif
		goto label_err_comerr;  	    
	}
	
	//清除充资备份区
	fail_add_clr(ADD_CLR_FLAG);
	
	//MAC在前面已经形成
	return 0;

label_err_comerr:
	gResponseStatus = CE_COMERR;
	return (char)-1;
}


char trans_read_cpu_record(uchar recordno)
{
unsigned char ret;
char i;


#ifdef _EMU_
printf("\ntrans_read_cpu_record now.");
#endif

	if (gStatus == TRANS_STATUS)  goto label_cpu_read_files;

	//under IDLE_STATUS
	ret = iso_deselect(1);
	if(ret != 0){
	#ifdef _EMU_
	printf("\nErr:iso_deselect,ret=%d",ret);
	#endif
	}
#ifdef _EMU_  
printf("\nOK:iso_deselect.");
#endif

	//检查卡是否存在
	for(i=0;i<MAX_TRY;i++){
		ret = hl_active(0,REQ_CNT,gThisCardSnr,gBuf);
		if(ret != 0){
			#ifdef _EMU_
		  printf("\nErr:hl_active,ret=%d",ret);
		  #endif
		  continue;
		}
#ifdef _EMU_  
printf("\n卡号:");
for(i=0;i<4;i++) printf("%02X ",(UBYTE)gThisCardSnr[i]);
#endif
		goto label_con;
	}
	goto label_err_card_moved;

label_con:
    #ifndef _EMU_
      delay_ms(5);
    #endif
	ret = iso_rats(1,gBuf);	
	if(ret) {
    #ifndef _EMU_
      delay_ms(5);
    #endif
    ret = iso_rats(1,gBuf);
	}
	if(ret) {
    #ifndef _EMU_
      delay_ms(5);
    #endif
    ret = iso_rats(1,gBuf);
	}		
	if(ret)  {
  	   gDebugStep = 0x2160;
  	   goto label_err_read;
	}
	#ifdef _EMU_
	printf("\niso_rats:");
	for (i=0;i<16;i++)  printf(" %02x",gBuf[i]);
	#endif

	//select file
	memcpy(gBuf,"\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01\x00",15);
	gBufBytes = 15;
	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
	if(ret)  {
  	   gDebugStep = 0x2161;
  	   goto label_err_read;
	}
	#ifdef _EMU_
	printf("\niso_apdu select file:");
	for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
	#endif
	memcpy(gCityCode,&gBuf[24],2);//城市代码	     
	memcpy(gThisCpuCardSnr,&gBuf[34],8);//卡号,卡唯一代码？
	memset(gNewACardSnr,0,sizeof(gNewACardSnr));//下次取卡信息时重新计算keya 20090609	   
	#ifdef _EMU_  
	printf("\n城市代码: %02x %02x",gCityCode[0],gCityCode[1]);
	printf("\n卡号 gThisCpuCardSnr:");
	for(i=0;i<8;i++) printf(" %02X",(UBYTE)gThisCpuCardSnr[i]);
	#endif
	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x6a) && (gBuf[gBufBytes-1] == (uchar)0x81))  {
  	   gDebugStep = 0x2162;
  	   gResponseStatus = CE_NOFUNCTION;
  	   return (char)-1;
	}
	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x90) && (gBuf[gBufBytes-1] == 0))  {
  	   gDebugStep = 0x2163;
  	   goto label_cpu_read_files;
	}
	gResponseStatus = CE_READERROR;
	return (char)-1;

  
label_cpu_read_files:
	if ((recordno == 15) || (recordno == 14)) {
		//read 0010
		memcpy(gBuf,"\x00\xb2\x00\x84\x10",5);
		gBuf[2] = 1;
		gBufBytes = 5;
		ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
		if(ret)  {
	  	   gDebugStep = 0x2164;
	  	   gResponseStatus = CE_READERROR;
			return -1;
		}
		#ifdef _EMU_
		printf("\niso_apdu 公共复合消费文件0010 no.%02d:",1);
		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
		#endif
		if(((uchar)gBuf[gBufBytes-2] == (uchar)0x6a) && (gBuf[gBufBytes-1] == 0x83))  {
	  	   gDebugStep = 0x2165;
	  	   goto label_err_record_not_found;
		}
		if(((uchar)gBuf[gBufBytes-2] == (uchar)0x6a) && (gBuf[gBufBytes-1] == 0x82))  {
	  	   gDebugStep = 0x2166;
	  	   goto label_err_record_not_found;
		}
		if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
	  	   gDebugStep = 0x2167;
	  	   goto label_err_read;
		}
		return 0;	   
	}
  
	if((recordno == 0)||(recordno == 1)||(recordno == 2)||(recordno == 3)) {
		//read 0008-->0006 
		//memcpy(gBuf,"\x00\xb2\x00\x44\x17",5);
		memcpy(gBuf,"\x00\xb2\x00\x34\x17",5);
		gBuf[2] = recordno+1;
		gBufBytes = 5;
		ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
		if(ret)  {
	  	   gDebugStep = 0x2168;
	  	   goto label_err_read;
		}
	#ifdef _EMU_
	printf("\niso_apdu 充值交易明细文件0008-->0006 no.%02d:",recordno+1);
	for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
	#endif
		if(((uchar)gBuf[gBufBytes-2] == (uchar)0x6a) && (gBuf[gBufBytes-1] == 0x83))  {
	  	   gDebugStep = 0x2169;
	  	   goto label_err_record_not_found;
		}
		if(((uchar)gBuf[gBufBytes-2] == (uchar)0x6a) && (gBuf[gBufBytes-1] == 0x82))  {
	  	   gDebugStep = 0x216a;
	  	   goto label_err_record_not_found;
		}
		if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
	  	   gDebugStep = 0x216b;
	  	   goto label_err_read;
		}
		return 0;	   
	}

	if((recordno >= 4) && (recordno <= 13)) {
		if(memcmp(gCityCode,"\x20\x00",2) != 0)   goto label_not_shanghai_card;//非上海卡直接读0018文件，不用管M1消费是否关闭，20090624
  	
		if(!gM1ConsumptionStatus) {//00为M1消费未关闭
			//read 0007
			memcpy(gBuf,"\x00\xb2\x00\x3c\x10",5);
			gBuf[2] = recordno - 3;
			gBufBytes = 5;
			ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
			if(ret)  {
		  	   gDebugStep = 0x216c;
		  	   goto label_err_read;
			}
#ifdef _EMU_
printf("\niso_apdu 交易明细记录文件0007 no.%02d:",recordno-3);
for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
#endif
			if(((uchar)gBuf[gBufBytes-2] == (uchar)0x6a) && (gBuf[gBufBytes-1] == 0x83))  {
				gDebugStep = 0x216d;
				goto label_err_record_not_found;
			}
			if(((uchar)gBuf[gBufBytes-2] == (uchar)0x6a) && (gBuf[gBufBytes-1] == 0x82))  {
		  	   gDebugStep = 0x216e;
		  	   goto label_err_record_not_found;
			}
			if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
		  	   gDebugStep = 0x216f;
		  	   goto label_err_read;
			}
  	   
			if(lrc_chk2(gBuf,15) != 0 || gBuf[0]==0 || (unsigned char)gBuf[0] == (unsigned char)0xff){
#ifdef _EMU_
   printf("\nErr:Record lrc check or first byte is 0 or 0xff:");
   for(i=0;i<16;i++) printf("%02X ",(uchar)gBuf[i]);
   printf("\nplease press any key to continue...");
#endif	
				gResponseStatus = CE_NORECORD;
				return (char)-1;
			} 
#ifdef _EMU_
   printf("\nRecord Read ok.");
#endif
   	   
			return 0;  	
  	
		}
		else  {//M1消费已关闭,纯cpu卡
			label_not_shanghai_card:	
			//read 0018
			memcpy(gBuf,"\x00\xb2\x00\xc4\x17",5);
			gBuf[2] = recordno - 3;
			gBufBytes = 5;
			ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
			if(ret)  {
		  	   gDebugStep = 0x216c;
		  	   goto label_err_read;
			}
#ifdef _EMU_
	  printf("\niso_apdu 交易明细记录文件0018 no.%02d:",recordno-3);
  for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
#endif
			if(((uchar)gBuf[gBufBytes-2] == (uchar)0x6a) && (gBuf[gBufBytes-1] == 0x83))  {
				gDebugStep = 0x216d;
				goto label_err_record_not_found;
			}
			if(((uchar)gBuf[gBufBytes-2] == (uchar)0x6a) && (gBuf[gBufBytes-1] == 0x82))  {
		  	   gDebugStep = 0x216e;
		  	   goto label_err_record_not_found;
			}
			if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
		  	   gDebugStep = 0x216f;
		  	   goto label_err_read;
			}
			return 0;
		} 	   
	}

label_err_read:
	gResponseStatus = CE_READERROR;
	#ifdef _EMU_
	printf("\nError: read card");
	#endif
	return (char)-1;

label_err_record_not_found:
	gResponseStatus = CE_NOLOG;
	#ifdef _EMU_
	printf("\nError: record not found");
	#endif
	return (char)-1;

label_err_card_moved:
	gResponseStatus = CE_CARDREMOVED;
	#ifdef _EMU_
	printf("\nError: card removed");
	#endif
	return (char)-1;	
}


char trans_credit_ini1(void)
{
unsigned char i;
unsigned char ret;
unsigned char  buf[17];

#ifdef _EMU_
printf("\ntrans_credit_ini1 now");
#endif

	//检查卡是否存在
	for(i=0;i<MAX_TRY;i++){
		ret = hl_active(0,REQ_CNT,gThisCardSnr,gBuf);
		if(ret != 0){
		#ifdef _EMU_
		  printf("\nErr:hl_active,ret=%d",ret);
		#endif
			continue;
	}
#ifdef _EMU_  
	printf("\n卡号:");
	for(i=0;i<4;i++) printf("%02X ",(UBYTE)gThisCardSnr[i]);
#endif
		goto label_con;
	}

	gResponseStatus = CE_CARDREMOVED;
	return (UBYTE)-1;

label_con:
	if (gCardStatus == 0x1)   //卡片未启用  
	{	
		gDebugStep = 0x1600;
		//formal key
		ret = hl_auth(AUTH_MODE_EXT_KEYA,0,gShFormalCardKeyA0);
		if(ret){
#ifdef _EMU_
  printf("\nErr:hl_auth,sector0,ret=%d",ret);
#endif
		}
		gDebugStep = 0x1601;  
		ret = mcml_read_return_mac(1,buf,gLineProtectionMAC);
		if(ret){
		#ifdef _EMU_
		printf("\nErr:mcml_read,block%d,ret=%d",1,ret);
		#endif
		}
		else goto label_key_ok;

//test key
//ret = hl_auth(AUTH_MODE_EXT_KEYA,0,gShTestCardKeyA0);
//if(ret){
//	#ifdef _EMU_
//  printf("\nErr:hl_auth,sector0,ret=%d",ret);
//  #endif
//  }
//gDebugStep = 0x1603;  
//ret = mcml_read_return_mac(1,buf,gLineProtectionMAC);
//if(ret){
//	   #ifdef _EMU_
//     printf("\nErr:mcml_read,block%d,ret=%d",1,ret);
//     #endif
//     goto label_invalid;
//  	 }
//else goto label_key_ok;

		goto label_invalid;

label_key_ok:
		gDebugStep = 0x1604;
		if(lrc_chk2(buf,15) != 0){
	   		gDebugStep = 0x1605;
	    	goto label_invalid;
		}

		memcpy(gB1,buf,16);
		return 0;
	}
	else  //卡片启用
	{
		ret = sam_getnewkeya();
		if(ret)
		{
			#ifdef _EMU_
			printf("\nErr:sam_getnewkeya,,ret=%d",ret);
			#endif
			goto label_err_read;
		}
		ret = hl_auth(AUTH_MODE_INT_KEYA,2,(UBYTE *)"\x0\x0\x0\x0\x0\x0");
		if(ret){
			#ifdef _EMU_
			printf("\nErr:hl_auth,sector0,ret=%d",ret);
			#endif
			goto label_err_read;
		}
		gDebugStep = 0x1606;  
		ret = mcml_read_return_mac(8,buf,gLineProtectionMAC);
		if(ret){
		#ifdef _EMU_
		printf("\nErr:mcml_read,block%d,ret=%d",1,ret);
		#endif
			goto label_err_read;
		}
  	 
		gDebugStep = 0x1607;
     	 	
		memcpy(gB8,buf,16);	
		return 0;	
	}

label_invalid:
	gResponseStatus = CE_INVALIDCARD;               
	return (UBYTE)-1;
	
label_err_read:
	gResponseStatus = CE_READERROR;
	return (UBYTE)-1;
}


/*************************************************************************************
函数：trans_sale
功能：售卡
**************************************************************************************/
char trans_sale(void)
{
uchar  buf[100],buf0[100],buf1[100],buf2[100];
ulong  tlong;
uchar i,ret;
uint tint;

#ifdef _EMU_
	printf("\ntrans_sale now.");
#endif

//if(samType!=1) return -1;
//if((uchar)ExternAuthStatusNot!=~(uchar)ExternAuthStatus)
//{
//	if(isam_auth_chk()!=0)
//	ExternAuthStatus=0;
//	else
//	ExternAuthStatus=0x55;
//	ExternAuthStatusNot=~ExternAuthStatus;
//}
//if((uchar)ExternAuthStatus!=0x55)
//goto label_no_auth;

	if(check_time_valid() != 0) return (char)-1;

	watchdog();	
//检查卡是否存在
	for(i=0;i<MAX_TRY;i++){
		ret = hl_active(0,REQ_CNT,gThisCardSnr,gBuf);
		if(ret != 0){
		#ifdef _EMU_
			printf("\nErr:hl_active,ret=%d",ret);
		#endif
			continue;
		}
#ifdef _EMU_  
	printf("\n卡号:");
	for(i=0;i<4;i++) printf("%02X ",(UBYTE)gThisCardSnr[i]);
#endif
		goto label_begin;
	}
	gResponseStatus = CE_CARDREMOVED;
	return (char)-1;   

label_begin:
	if(gCardStatus != 0x01){
		gResponseStatus = CE_CARDSTATUSERROR;
		return (char)-1;
	}

//if(sam_getnewkeyb() != 0) return -1;
  
	//建立备份
	if(trans_setup_bakup() != 0) return (char)-1;
	
	//获取B9,B10
	if(hl_auth(AUTH_MODE_INT_KEYA,2,(UBYTE *)"\x0\x0\x0\x0\x0\x0") != 0) goto label_err_read;
	if(mcml_read(9,gB9) != 0) goto label_err_read;
	if(mcml_read(10,gB10) != 0) goto label_err_read;
	//读B8
	if(hl_auth(AUTH_MODE_INT_KEYA,2,(UBYTE *)"\x0\x0\x0\x0\x0\x0") != 0) goto label_err_read;
	if(mcml_read(8,gB8) != 0) goto label_err_read;

	*((char *)&gCardAddSum+0) = gB8[0];
	*((char *)&gCardAddSum+1) = gB8[1];
	*((char *)&gCardAddSum+2) = gB8[2];
	*((char *)&gCardAddSum+3) = gB8[3];
	
	//写卡上公共交易记录
	gCardThisRemain = gCardLastRemain+gCardThisTransValue;
	trans_gen_card_record(0x92, gCardRecord);
	if(hl_auth(AUTH_MODE_INT_KEYA,gCardRecPos[gCardRecPtr]/4,(UBYTE *)"\x0\x0\x0\x0\x0\x0") != 0) goto label_err_read;
	if(mcml_read(gCardRecPos[gCardRecPtr],buf) != 0) goto label_err_read;
	if(mcml_write(gCardRecPos[gCardRecPtr],gCardRecord) != 0) goto label_err_write;
	memcpy(gCardRecord,buf,16);

	//写卡上加款私有加款记录
	for(i=0;i<5;i++) buf1[i] = gTransTime[1+i];
	
	buf1[5] = *((char *)&gCardThisTransValue+0);
	buf1[6] = *((char *)&gCardThisTransValue+1);
	buf1[7] = *((char *)&gCardThisTransValue+2);
	
	
	tlong = gCardPurse+gCardAddSum; //specail 
	gCardLastRemain = tlong;
	
	buf1[8] = *((char *)&tlong+0);   //special
	buf1[9] = *((char *)&tlong+1);   //special
	i = *((char *)&tlong+3);
	if((uchar)i > (uchar)127) buf1[10] = 0xff;
	else buf1[10] = *((char *)&tlong+2);

	//本次剩余金额
	gCardThisRemain = gCardLastRemain+gCardThisTransValue;
	
	buf1[11] = *((char *)&gCardThisRemain+0); 
	buf1[12] = *((char *)&gCardThisRemain+1); 
	i = *((char *)&gCardThisRemain+3);
	if((uchar)i > (uchar)127) buf1[13] = 0xff;
	else buf1[13] = *((char *)&gCardThisRemain+2);


	//卡计数器
	buf1[14] = *((char *)&gCardCnt+1);
	buf1[15] = *((char *)&gCardCnt+0);


//形成B10
//compute TAC
//if(sam_get_add_trans_ac() != 0) return -1;

	//计算TAC
	gDebugStep = 0x15b7;
	buf0[0] = 0x0;//交易类型标志	1
	buf0[1] = *((char *)&gPosTransCnt+1);//终端交易流水号	3
	buf0[2] = *((char *)&gPosTransCnt+2);
	buf0[3] = *((char *)&gPosTransCnt+3);
	memcpy(&buf0[4],gCityCode,2);//城市代码	2
	memcpy(&buf0[6],gThisCardSnr,4);//卡唯一代码	4
	buf0[10] = gCardType;//卡类型	1
	buf0[11] = *((char *)&gCardLastRemain+0);//交易前余额	3
	buf0[12] = *((char *)&gCardLastRemain+1);
	buf0[13] = *((char *)&gCardLastRemain+2);//msb
	
	buf0[14] = *((char *)&gCardThisTransValue+0);//交易金额	3
	buf0[15] = *((char *)&gCardThisTransValue+1);
	buf0[16] = *((char *)&gCardThisTransValue+2);//msb
	
	memcpy(&buf0[17],gTransTime,7);//交易日期	4 交易时间	3
	
	buf0[24] = *((char *)&gCardCnt+1);//卡交易计数器	2
	buf0[25] = *((char *)&gCardCnt+0);
	memcpy(&buf0[26],&FM_Module_NO[2],4);//更新：SAM卡号	4-->充资：FM模块号
	memcpy(&buf0[30],"\x80\x00",2);
#ifdef _EMU_
	printf("\nhl_cal_tac-->buf0:");
	for(i=0;i<32;i++) printf(" %02x",buf0[i]);
#endif
	ret = hl_cal_tac(0x0A,buf0,32,gTAC);//MAC
	if(ret)  {
            gDebugStep = 0x1518; 
	          #ifdef _EMU_
            printf("\nErr:hl_cal_tac,ret=%d",ret);
	          #endif
	          //goto label_err_write;  	    
	}
#ifdef _EMU_
	printf("\nhl_cal_tac-->tac:");
	for(i=0;i<4;i++) printf(" %02x",gTAC[i]);
#endif

	buf2[0] = *((char *)&gPosNo+1);
	buf2[1] = *((char *)&gPosNo+2);
	buf2[2] = 0x18;
	buf2[3] = *((char *)&gPosNo+3);

	for(i=0;i<4;i++){
		//buf2[4+i] = gWorkerNo[i];
		buf2[4+i] = FM_Module_NO[2+i];
		buf2[8+i] = gTAC[i];//
		buf2[12+i] = 0;
	}

	//gCardAddSum = 1;    //special
	tlong = gCardAddSum;
	gCardAddSum += gCardThisTransValue;

	//if(sam_getnewchongziAC() != 0) return -1;
	gCardAddSum = tlong;
	
	tlong = gCardAddSum +gCardThisTransValue;
	
	buf0[0] = *((char *)&tlong+0);
	buf0[1] = *((char *)&tlong+1);
	buf0[2] = *((char *)&tlong+2);
	buf0[3] = *((char *)&tlong+3);

	for(i=0;i<8;i++) buf0[4+i] = gAddAuthCode[i];
	for(i=0;i<4;i++){
		buf0[12+i] = gCardNewExpiredDate[i];
	}

	//write b13
	for(i=0;i<11;i++) buf[i] = 0;
	i = (gCardRecPtr+1)%10;
	buf[11] = 0x02;
	buf[12] = i;
	tint = gCardCnt+1;
	
	buf[13] = *((char *)&tint+1);
	buf[14] = *((char *)&tint+0);
	/* //cut in 2012-01-10
	memcpy(&buf[7],CntCardFirstUse,4);
	CntCardTotal[0]++;
	if(CntCardTotal[0]==0) CntCardTotal[1]++;
	memcpy(&buf[5],CntCardTotal,2);
	*/
	lrc_gen2(buf,15);
	if(hl_auth(AUTH_MODE_INT_KEYA,4,(UBYTE *)"\x0\x0\x0\x0\x0\x0") != 0) goto label_err_write;
	if(mcml_write(13,buf) != 0) goto label_err_write;
	 
	//将地铁标记设置为出站状态
	buf[0] = 0x81;
	for(i=0;i<5;i++) buf[i+1] = gTransTime[i+1];
	for(i=0;i<9;i++) buf[6+i] = 0;
	lrc_gen2(buf,15);
	if(hl_auth(AUTH_MODE_INT_KEYA,8,(UBYTE *)"\x0\x0\x0\x0\x0\x0") != 0) goto label_err_write;
	if(mcml_write(33,buf) != 0) goto label_err_write;
	for(i=0;i<15;i++) buf[i] = 0;
	buf[0] = 0x66;
	buf[15] = 0x66;
	if(hl_auth(AUTH_MODE_INT_KEYA,8,(UBYTE *)"\x0\x0\x0\x0\x0\x0") != 0) goto label_err_write;
	if(mcml_write(34,buf) != 0) goto label_err_write;

	//clear flag
	for(i=0;i<16;i++) buf[i]=0;
	if(hl_auth(AUTH_MODE_INT_KEYA,3,(UBYTE *)"\x0\x0\x0\x0\x0\x0") != 0) goto label_err_write;
	if(mcml_write(12,buf) != 0) goto label_err_write; 

	//write B1
	fail_b1_add();
	
	//建立充资区备份
	fail_add_add();
	
	//更新充资区

	//密文方式 密钥加载
	ret = mcml_load_key_miwen_mode(gM1SaleValKey);	 
	if(ret){
	#ifdef _EMU_
		printf("\nErr:mcml_load_key_miwen_mode,ret=%d",ret);
	#endif
		goto label_load_key_err;
	}

	//MAC保护方式更新数据块1
	memcpy(buf,gM1SaleValBlock,16);
	memcpy(&buf[16],gSaleValMAC,4);      	    
	//if(mcml_write_mac_mode(1,buf,gSaleValMAC) != 0) goto label_err_write;
	mcml_write_mac_mode(1,buf,gSaleValMAC);

	if(mcml_write(10,buf2) != 0) goto label_err_write;
	if(mcml_write(9,buf1) != 0) goto label_err_write;
	#ifdef _EMU_
		printf("Press a key to gen write error");
		goto label_err_write;
	#endif

	//取交易结果指令获取交易结果TAC码
	memset(gNewACardSnr,0,sizeof(gNewACardSnr));//下次取卡信息时重新计算keya，否则取卡信息有问题？20090521	

	ret = hl_get_trans_result(&gTransTime[1],gTAC);
	if(ret)  {
	#ifdef _EMU_
            printf("\nErr:hl_get_trans_result,ret=%d",ret);
	#endif
		goto label_err_write;  	    
	}

	fail_add_clr(SALE_CLR_FLAG);
	fail_b1_clr();

	//清其他块==============================================================
	for(i=0;i<16;i++) buf[i] = 0;
	for(i=8;i<9;i++){
		if(hl_auth(AUTH_MODE_INT_KEYA,i,(UBYTE *)"\x0\x0\x0\x0\x0\x0") != 0) goto label_err_write;
		if(mcml_write(i*4,buf) != 0) goto label_err_write;
		if(mcml_write(i*4+1,buf) != 0) goto label_err_write;
		if(mcml_write(i*4+2,buf) != 0) goto label_err_write;
	}	

	//MAC在前面已经形成
	return 0;

label_err_write:
	gResponseStatus = CE_WRITEERROR;
	return (char)-1;

label_err_read:
	gResponseStatus = CE_READERROR;
	return (char)-1;
label_load_key_err:
	gResponseStatus = CE_AUTHENERROR;
	return (char)-1;
}


char trans_sale_cpu(void)
{
char ret;
#ifdef _EMU_
char i;
#endif

#ifdef _EMU_
printf("\ntrans_sale_cpu now.");
#endif
       	
	//cpu sale
	memcpy(gBuf,"\x04\xd6\x95\x14\x08",5);//应用启动日期
	memcpy(&gBuf[5],CntCardNewStartDate,4);
	memcpy(&gBuf[9],gMAC,4);
	gBufBytes = 13;
	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
	if(ret)  {
		gDebugStep = 0x2881;
		gResponseStatus = CE_CARDREMOVED;  
		return (char)-1;
	}
#ifdef _EMU_
	printf("\niso_apdu trans_sale_cpu:");
	for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
#endif
	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
		gDebugStep = 0x2882;
		gResponseStatus = CE_WRITEERROR;  
		return (char)-1;
	}	

	return 0;	
}

uchar psam_get_chk(uchar *inbuf,uchar *outbuf)
{
uchar ret,i;

	sam_set(SAMLOCATION_5,SAM_ETU_93, 4);
	//if(ret){
	//#ifdef _EMU_
	//printf("\nErr:psam_get_chk->sam_set_speed,ret=%d",ret);
	//#endif
	//	return (char)-1;
	//}
	ret = sam_select(SAMLOCATION_5);
	if(ret){
	#ifdef _EMU_
	printf("\nErr:psam_get_chk->sam_select,ret=%d",ret);
	#endif
		return (char)-1;
	}	

	ret = sam_atr(SAMLOCATION_5, gBuf, &gBufBytes);
	if(ret){
	#ifdef _EMU_
	printf("\nErr:psam_get_chk->sam_atr,ret=%d",ret);
	#endif
		return (char)-1;
	}

#ifdef _EMU_
	printf("\npsam_get_chk-->sam_atr:");
	for(i=0;i<gBufBytes;i++) printf(" %02x",gBuf[i]);
#endif

	//select 1003
	memcpy(gBuf,"\x00\xa4\x00\x00\x02\x10\x03",7);
	gBufBytes = 7;
	
	ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes, 0, 0);
	
	if(ret != 0) goto label_err;

#ifdef _EMU_
printf("\npsam_get_chk-->select 1003:");
for(i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
#endif

	if(((uchar)gBuf[0] != (uchar)0x61) && ((uchar)gBuf[0] != (uchar)0x90)) goto label_err;
	
	
	gBuf[0] = 0x80;
	gBuf[1] = 0x1a;
	gBuf[2] = 0x44;
	gBuf[3] = 0x01;
	gBuf[4] = 0x10;
	gBuf[5] = gThisCardSnr[0];
	gBuf[6] = gThisCardSnr[1];
	gBuf[7] = gThisCardSnr[2];
	gBuf[8] = gThisCardSnr[3];
	gBuf[9] = 0xff;
	gBuf[10] = 0;
	gBuf[11] = 0;
	gBuf[12] = 0;
	gBuf[13] = gCityCode[0];
	gBuf[14] = gCityCode[1];
	gBuf[15] = 0xff;
	gBuf[16] = 0;
	gBuf[17] = 0;
	gBuf[18] = 0;
	gBuf[19] = 0;
	gBuf[20] = 0;
	gBufBytes = 21;
	//gReceiveNum=2;
	ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes, 0, 0);
#ifdef _EMU_
	printf("\npsam_get_chk-->80 1a:");
	for(i=0;i<gBufBytes;i++) printf(" %02x",gBuf[i]);
#endif
	if(ret != 0){
	goto label_err;
	}
	if(gBufBytes != 2){
		goto label_err;
	}
	if((unsigned char)gBuf[0] != (unsigned char)0x90){
		goto label_err;
	}
	if(gBuf[1] != 0x00) goto label_err;
	
	gBuf[0] = 0x80;
	gBuf[1] = 0xfa;
	gBuf[2] = 0x01;
	gBuf[3] = 0x00;
	gBuf[4] = 0x20;
	
	for(i=0;i<32;i++)
		gBuf[5+i] =inbuf[i];

	gBufBytes = 37;
	//gReceiveNum=2;
	ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes, 0, 0);
#ifdef _EMU_
printf("\npsam_get_chk-->80 fa:");
for(i=0;i<gBufBytes;i++) printf(" %02x",gBuf[i]);
#endif
	if(ret != 0) goto label_err;
	if(gBufBytes != 2) goto label_err;
	if(gBuf[0] != 0x61) goto label_err;
	if(gBuf[1] != 0x04) goto label_err;
	
	gDebugStep = 0x14c8;

	//取应答
	gBuf[0] = 0x00;
	gBuf[1] = 0xc0;
	gBuf[2] = 0x00;
	gBuf[3] = 0x00;
	gBuf[4] = 0x04;
	gBufBytes = 5;
	//gReceiveNum=6;
	ret = sam_apdu(SAMLOCATION_5, gBuf,gBufBytes,gBuf,&gBufBytes, 0, 0);
#ifdef _EMU_
	printf("\npsam_get_chk-->get response:");
	for(i=0;i<gBufBytes;i++) printf(" %02x",gBuf[i]);
#endif
	if(ret != 0) goto label_err;
	if(gBufBytes != 6) goto label_err;
	outbuf[0] = gBuf[0];
	outbuf[1] = gBuf[1];
	outbuf[2] = gBuf[2];
	outbuf[3] = gBuf[3];

	return 0; 
label_err:
	gResponseStatus = CE_ISAMERROR;
	return (char)-1;
}


char bom_getcardinfo_cpu(void)
{
unsigned char i,ptr;
unsigned char ret;
unsigned long tlong1,tlong2;
unsigned char bytes,buf[100];

//#ifdef _EMU_
//	printf("\ntrans_getcardinfo_cpu now.");
//#endif
//
//	gNotShCard = 0;
//	gApplicationLockedFlag = 0x55;//应用已解锁
//
//	if(gPreviousCpuModeFlag == 0x55) goto label_previous_cpu_mode;
//
//	ret = iso_deselect(1);
//	if(ret != 0){
//	#ifdef _EMU_
//		printf("\nErr:iso_deselect,ret=%d",ret);
//	#endif
//	}
//#ifdef _EMU_  
//	printf("\nOK:iso_deselect.");
//#endif
//
//	watchdog();
////检查卡是否存在
//	for(i=0;i<MAX_TRY;i++){
//		ret = hl_active(0,REQ_CNT,gThisCardSnr,gBuf);
//		if(ret != 0){
//	#ifdef _EMU_
//		printf("\nErr:hl_active,ret=%d",ret);
//	#endif
//		continue;
//		}
//#ifdef _EMU_  
//		printf("\n卡号:");
//		for(i=0;i<4;i++) printf("%02X ",(UBYTE)gThisCardSnr[i]);
//		printf("\nTagType[0],TagType[1] and Size:");
//		for(i=0;i<3;i++) printf("%02X ",(UBYTE)gBuf[i]);
//#endif
//	
//		if(gBuf[2] == 0x53)  gCardSizeIs0x53Flag = 1;
//		memset(gNewACardSnr,0,sizeof(gNewACardSnr));//下次取卡信息时重新计算keya 20090609
//		goto label_con;
//	}
//	goto label_err_card_moved;
//
//label_con:
//    #ifndef _EMU_
//		delay_ms(5);
//    #endif
//	ret = iso_rats(1,gBuf);	
//	if(ret) {
//    #ifndef _EMU_
//		delay_ms(5);
//    #endif
//	ret = iso_rats(1,gBuf);
//	}		
//	if(ret) {
//    #ifndef _EMU_
//		delay_ms(5);
//    #endif
//		ret = iso_rats(1,gBuf);
//	}
//	if(ret)  {
//  	   gDebugStep = 0x2001;
//  	   goto label_err_read;
//	}
//	#ifdef _EMU_
//		printf("\niso_rats:");
//		for (i=0;i<16;i++)  printf(" %02x",gBuf[i]);
//	#endif
//
//label_previous_cpu_mode:  //20090703
//	gPreviousCpuModeFlag = 0;
//
//  //read 0005
//  /* //2011-3-15 外地cpu卡与上海cpu卡0005文件格式不一致，无需读0005文件；相关内容可以从0015文件获得
//  memcpy(gBuf,"\x00\xb0\x85\x00\x20",5);
//  gBufBytes = 5;
//  ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
//  if(ret)  {
//  	   gDebugStep = 0x2002;
//  	   goto label_err_read;
//  	   }
//  #ifdef _EMU_
//  printf("\niso_apdu 发卡方基本信息文件0005:");
//  for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//  #endif
//  if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
//  	   gDebugStep = 0x2003;
//  	   goto label_err_read;
//  	   } 
//  memcpy(gIssuerTag,gBuf,8);//发卡方标识	8
//  gApplicationTypeTag = gBuf[8]; //应用类型标识	1
//  gApplicationVer = gBuf[9]; //应用版本	1，卡片版本号
//  memcpy(gApplicationSn,&gBuf[10],10);//应用序列号	10
//  memcpy(gIssuerDate,&gBuf[20],4);//发卡日期	4 – YYYYMMDD 
//  memcpy(gIssuerDeviceNo,&gBuf[24],3);//发卡设备编号	3
//  memcpy(gIssuerOpNo,&gBuf[27],3);//发卡操作员号	3
//  memcpy(gApplicationVersion,&gBuf[30],2);//应用版本号	2
//  */
//
//	//select file
//	memcpy(gBuf,"\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01\x00",15);
//	gBufBytes = 15;
//	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
//	if(ret)  {
//  	   gDebugStep = 0x2004;
//  	   goto label_err_read;
//	}
//	#ifdef _EMU_
//		printf("\niso_apdu select file:");
//		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//	#endif
//  
//	if(gBufBytes < 2)  goto label_err_read;
//
//	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x93) && (gBuf[gBufBytes-1] == 0x03))  {
//		gDebugStep = 0x2cd5;
//		gResponseStatus = CE_INVALIDCARD;  
//		return (char) -1;
//	}
//
//	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) && (gBuf[gBufBytes-2] != 0x6a))  {
//		gDebugStep = 0x2cd6;
//		gResponseStatus = CE_READERROR;  
//		return (char) -1;
//	}
//
//	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x6a) && (gBuf[gBufBytes-1] == (uchar)0x81))  {
//  	   gDebugStep = 0x2cc1;
//  	   gApplicationLockedFlag = 0x0;//应用已锁定
//  	   bytes = gBufBytes;
//  	   memcpy(buf,gBuf,gBufBytes);  
//		   #ifdef _EMU_
//		   printf("\ntrans_getcardinfo_cpu-->try M1 restore,transfer to read in M1 mode.");
//		   #endif
//  	   ret = trans_getcardinfo();
//  	   if(ret)  goto label_err_read;
//  	   gBufBytes = bytes;
//  	   memcpy(gBuf,buf,gBufBytes);  	   
//	}
//
//	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x90) && (gBuf[gBufBytes-1] == 0x0))  {
//  	   gDebugStep = 0x2006;
//  	   gApplicationLockedFlag = 0x55;//应用已解锁  
//	}  
//  
//	//搜索9f 0C
//	for(ptr=0;ptr<gBufBytes;ptr++){
//    //if(memcmp(&gBuf[ptr],"\x0c\x1e",2) == 0) goto label_found_9f0c_ok;
//	    if(memcmp(&gBuf[ptr],"\x9f\x0c",2) == 0) 
//	  	{
//	  		ptr = ptr + 3;
//	  		goto label_found_9f0c_ok;
//		}
//	}
//
//	//read 0015
//	memcpy(gBuf,"\x00\xb0\x95\x00\x1e",5);
//	gBufBytes = 5;
//	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
//	if(ret)  {
//  	 gDebugStep = 0x2094;
//  	 goto label_err_read;
//	}
//	#ifdef _EMU_
//		printf("\niso_apdu read 0015:");
//		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//	#endif
//
//	if(((uchar)gBuf[gBufBytes-2]!=(uchar)0x90)||(gBuf[gBufBytes-1]!=0x00))
//	{
//		gDebugStep = 0x2095;
//		goto label_invalid;
//	}
//	ptr = 0;
//
label_found_9f0c_ok:
	memcpy(gCityCode,&gBuf[ptr+2],2);//城市代码
	gApplicationTypeTag = gBuf[ptr+8]; //应用类型标识	1
//	gApplicationVer = gBuf[ptr+9]; //应用版本	1,卡片版本号
//	memcpy(gInterConnect,&gBuf[ptr+10],2);//互联互通标识 2
	memcpy(gApplicationSn,&gBuf[ptr+10],10);//应用序列号	10
	memcpy(gThisCpuCardSnr,&gBuf[ptr+12],8);//卡号,卡唯一代码？	   
	memset(gNewACardSnr,0,sizeof(gNewACardSnr));//下次取卡信息时重新计算keya 20090609
//	#ifdef _EMU_  
//		printf("\n卡号 gThisCpuCardSnr:");
//		for(i=0;i<8;i++) printf(" %02X",(UBYTE)gThisCpuCardSnr[i]);
//		printf("\n应用序列号 gApplicationSn:");
//		for(i=0;i<10;i++) printf(" %02X",(UBYTE)gApplicationSn[i]);
//	#endif
//	memcpy(CntCardStartDate,&gBuf[ptr+20],4); //启用日期
//	memcpy(gCardExpiredDate,&gBuf[ptr+24],4);//有效日期
//	gCardType = gBuf[ptr+28];//卡类型
//	gCardDeposit = gBuf[ptr+29];//卡押金
//
//  #ifdef _EMU_
//	  printf("\ngCityCode: %02x %02x",gCityCode[0],gCityCode[1]);
//	  printf("\ngInterConnect: %02x %02x",gInterConnect[0],gInterConnect[1]);
//	  printf("\ngApplicationVer: %02x",gApplicationVer);  
//	  printf("\ngApplicationLockedFlag: %02x",gApplicationLockedFlag);
//  #endif
//
//	if((memcmp(gInterConnect,"\x20\x00",2) == 0) && (gApplicationVer == 0x02)) {
//		if(gApplicationLockedFlag == 0x55) {//对CPU卡永久锁定，然后返回M1内容
//            #ifdef _EMU_
//            printf("\ncard block forever,and then throw back M1 content.");
//            #endif
//			//get random
//			memcpy(gBuf,"\x00\x84\x00\x00\x04",5);
//			gBufBytes = 5;
//			ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
//			if(ret)  {
//				   gDebugStep = 0x2c0b;
//				   goto label_err_read;
//				   }
//			#ifdef _EMU_
//			printf("\niso_apdu get random:");
//			for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//			#endif
//			memcpy(gRandom,gBuf,4);//	    
//				
//			sam_set_speed(BUS_PSAM_INDEX,ETU93);
//			sam_select(BUS_PSAM_INDEX);
//			sam_atr(gBuf,&gBufBytes);
//			#ifdef _EMU_
//			printf("\ncpu app unlock->sam_atr:");
//			for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//			#endif
//			
//			//read psam 0016
//			memcpy(gBuf,"\x00\xb0\x96\x00\x00",5);
//			gBufBytes = 5;
//			ret = sam_apdu(gBuf,gBufBytes,gBuf,&gBufBytes);
//			#ifdef _EMU_
//			printf("\ncpu app unlock->sam_apdu read 0016:");
//			for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//			#endif
//			if(((uchar)gBuf[0] != (uchar)0x61) || (gBuf[1] != 0x06))
//				  {
//				  	gDebugStep = 0x2c0c;
//				  	gResponseStatus = CE_PSAMERR;
//				  	return (char)-1;
//				  }  		
//			
//			//get response
//			memcpy(gBuf,"\x00\xc0\x00\x00\x06",5);
//			gBufBytes = 5;
//			ret = sam_apdu(gBuf,gBufBytes,gBuf,&gBufBytes);
//			#ifdef _EMU_
//			printf("\ncpu app unlock->sam_apdu read 0016 get response:");
//			for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//			#endif
//			if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0x0))
//			{
//				gDebugStep = 0x2c0d;
//				  	gResponseStatus = CE_PSAMERR;
//				  	return (char)-1;
//			}  			    		
//			memcpy(psam_no,gBuf,6);
//			
//			memcpy(gBuf,"\x00\xa4\x00\x00\x02\x10\x01",7);
//			gBufBytes = 7;
//			ret = sam_apdu(gBuf,gBufBytes,gBuf,&gBufBytes);
//			#ifdef _EMU_
//			printf("\ncpu app unlock->sam_apdu select 1001:");
//			for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//			#endif
//			if(((uchar)gBuf[0] != (uchar)0x61) || (gBuf[1] != 0x28))
//				  {
//				  	gDebugStep = 0x2c0e;
//				  	gResponseStatus = CE_PSAMERR;
//				  	return (char)-1;
//				  }
//			
//			//get response
//			memcpy(gBuf,"\x00\xc0\x00\x00\x28",5);
//			gBufBytes = 5;
//			ret = sam_apdu(gBuf,gBufBytes,gBuf,&gBufBytes);
//			#ifdef _EMU_
//			printf("\ncpu app unlock->sam_apdu select 1001 get response:");
//			for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//			#endif
//			if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0x0))
//				  {
//				  	gDebugStep = 0x2c0f;
//				  	gResponseStatus = CE_PSAMERR;
//				  	return (char)-1;
//				  }  			    		
//			
//			memcpy(gBuf,"\x00\xb0\x97\x00\x01",5);
//			gBufBytes = 5;
//			ret = sam_apdu(gBuf,gBufBytes,gBuf,&gBufBytes);
//			#ifdef _EMU_
//			printf("\ncpu app unlock->sam_apdu read 0017:");
//			for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//			#endif
//			if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0x0))
//				  {
//				  	gDebugStep = 0x2c10;
//				  	gResponseStatus = CE_PSAMERR;
//				  	return (char)-1;
//				  }
//			gKeyVersion = gBuf[0];	//密钥版本号
//			
//			memcpy(gBuf,"\x80\x1A\x45\x01\x10",5);
//			memcpy(&gBuf[5],gThisCpuCardSnr,8);
//			memcpy(&gBuf[13],gCityCode,2);
//			memcpy(&gBuf[15],"\xFF\x00\x00\x00\x00\x00",6);
//			gBufBytes = 21;
//			ret = sam_apdu(gBuf,gBufBytes,gBuf,&gBufBytes);
//			#ifdef _EMU_
//			printf("\ncpu app unlock->sam_apdu cal mac step1:");
//			for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//			#endif
//			if(((uchar)gBuf[0] != (uchar)0x90) || (gBuf[1] != 0x0))
//				  {
//				  	gDebugStep = 0x2c11;
//				  	gResponseStatus = CE_PSAMERR;
//				  	return (char)-1;
//				  }  		
//			
//			memcpy(gBuf,"\x80\xfA\x05\x00\x10",5);
//			memcpy(&gBuf[5],gRandom,4);
//			memcpy(&gBuf[9],"\x00\x00\x00\x00\x84\x16\x00\x00\x04\x80\x00\x00",12);
//			gBufBytes = 21;
//			ret = sam_apdu(gBuf,gBufBytes,gBuf,&gBufBytes);
//			#ifdef _EMU_
//			printf("\ncpu app unlock->sam_apdu cal mac step2:");
//			for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//			#endif
//			if(((uchar)gBuf[gBufBytes-2] != (uchar)0x61) || (gBuf[gBufBytes-1] != 0x04))
//				  {
//				  	gDebugStep = 0x2c12;
//				  	gResponseStatus = CE_PSAMERR;
//				  	return (char)-1;
//				  }
//			//get response
//			memcpy(gBuf,"\x00\xc0\x00\x00\x04",5);
//			gBufBytes = 5;
//			ret = sam_apdu(gBuf,gBufBytes,gBuf,&gBufBytes);
//			#ifdef _EMU_
//			printf("\ncpu app unlock->sam_apdu cal mac step2 get response:");
//			for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//			#endif
//			if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0x0))
//				  {
//				  	gDebugStep = 0x2c13;
//				  	gResponseStatus = CE_PSAMERR;
//				  	return (char)-1;
//				  }  			    		
//			memcpy(gMAC,gBuf,4);
//
//			memcpy(gBuf,"\x84\x16\x00\x00\x04",5);
//			memcpy(&gBuf[5],gMAC,4);//MAC
//			gBufBytes = 9;
//			ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
//			if(ret)  {
//				   gDebugStep = 0x2c07;
//				   goto label_err_read;
//				   }
//			#ifdef _EMU_
//			printf("\niso_apdu card block 应用永久锁定:");
//			for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//			#endif
//			if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
//			  	   gDebugStep = 0x2c08;
//			  	   goto label_err_read;
//			}
//
//		}
//		//不做解锁，返回M1内容
//		#ifdef _EMU_
//			printf("\ncard not unblock,and then throw back M1 content.");
//		#endif
//		gDebugStep = 0x2c05;
//		gResponseStatus = CE_NOCARDUNBLOCK;  
//		return (char) -1;    	
//	}
//
//	if(gApplicationLockedFlag == 0x0)  {//城市代码非2000或卡版本号非02
//		gDebugStep = 0x2005;
//		gResponseStatus = CE_NOFUNCTION;  
//		return (char) -1;
//	}
//
//	//read 0015
//	memcpy(gBuf,"\x00\xb0\x95\x00\x1e",5);
//	gBufBytes = 5;
//	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
//	if(ret)  {
//  	   gDebugStep = 0x2007;
//  	   goto label_err_read;
//	}
//	#ifdef _EMU_
//		printf("\niso_apdu 公共应用基本信息文件0015:");
//		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//	#endif
//	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
//  	   gDebugStep = 0x2008;
//  	   goto label_err_read;
//	}
//	memcpy(gIssuerTag,gBuf,8);//发卡方标识	8
//	gApplicationTypeTag = gBuf[8]; //应用类型标识	1
//	if((gApplicationTypeTag&0x80) == (uchar)0x80)  gM1ConsumptionStatus = 0x01;//00-未关闭，01-已关闭
//	else gM1ConsumptionStatus = 0x0;
//	gApplicationVer = gBuf[9]; //应用版本	1,卡片版本号
//	memcpy(gInterConnect,&gBuf[10],2);//互联互通标识 2
//	memcpy(gApplicationSn,&gBuf[10],10);//应用序列号	10
//	memcpy(gApplicationStartDate,&gBuf[20],4);//应用启动日期	4 – YYYYMMDD 
//	memcpy(gApplicationExpiredDate,&gBuf[24],4);//应用有效日期4
//	gCardType = gBuf[28];//卡类型
//	gCardDeposit = gBuf[29];//押金  
//       
//	//initial for purchase
//	memcpy(gBuf,"\x80\x50\x01\x02\x0b",5);//initial for purchase
//	memcpy(&gBuf[5],"\x01\x00\x00\x00\x00\x11\x22\x33\x44\x55\x66",11);
//	gBufBytes = 16;
//	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
//	if(ret)  {
//  	   gDebugStep = 0x200b;
//  	   goto label_err_read;
//	}
//	#ifdef _EMU_
//		printf("\niso_apdu 电子存折文件0002 initial for purchase:");
//		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//	#endif
//	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x62) && (gBuf[gBufBytes-1] == (uchar)0x81))  {
//  	   gDebugStep = 0x260a;
//  	   if((memcmp(gCityCode,"\x20\x00",2) == 0) || (memcmp(gCityCode,"\x21\x53",2) == 0)) goto label_err_need_m1_restore;//需要做m1恢复
//  	      else goto label_err_read;
//	}
//	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x69) && (gBuf[gBufBytes-1] == (uchar)0x85))  {
//  	   gDebugStep = 0x270a;
//  	   if((memcmp(gCityCode,"\x20\x00",2) == 0) || (memcmp(gCityCode,"\x21\x53",2) == 0)) goto label_err_need_m1_restore;//需要做m1恢复
//  	      else goto label_err_read;
//	}  
//
//	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
//  	   gDebugStep = 0x200c;
//  	   goto label_err_read;
//	}
//	memcpy(gCardBalanceExt,gBuf,4);//余额+透支
//	*((char *)&gCardCnt+1) = gBuf[4];
//	*((char *)&gCardCnt+0) = gBuf[5];//计数器
//	memcpy(gCardOverDraft,&gBuf[6],3);//透支额
//	gKeyVersion = gBuf[9];//密钥版本号
//	gAlgFlag = gBuf[10];//算法标识（DPK）
//	memcpy(gPseduRandom,&gBuf[11],4);//伪随机数（IC卡）
//	buf_to_udword(gCardBalanceExt,tlong1);
//	tlong2 = gCardOverDraft[0]*256*256+gCardOverDraft[1]*256+gCardOverDraft[2];
//	gCardLastRemain = tlong1 - tlong2;
//	udword_to_buf(gCardLastRemain,gCardBalance);
//
//  #ifdef _OTHER_YIDIKA_TEST_
//  if(memcmp(gCityCode,"\x21\x53",2) == 0)  goto label_not_shanghai_card;
//  #endif
// 
//  //
//  #ifdef _EMU_
//  printf("\ngCardSizeIs0x53Flag=%02x",gCardSizeIs0x53Flag);
//  #endif
//  /*  //2012-5-14,去掉CPU卡0x53判断
//  if((gCardSizeIs0x53Flag == 0)&&(memcmp(gCityCode,"\x20\x00",2) == 0)) 
//  {
//      gResponseStatus = CE_INVALIDCARD;
//      return (char)-1;
//  }
//  
//  if((gCardSizeIs0x53Flag == 0)&&(memcmp(gCityCode,"\x20\x00",2) != 0)) */ 
//	if(memcmp(gCityCode,"\x20\x00",2) != 0) 
//	{
//      gNotShCard = 0x55;
//	  goto label_not_shanghai_card;//外地卡
//	}
//#ifdef _EMU_
//	printf("\n上海卡处理");
//#endif
//  /*if((memcmp(gCityCode,"\x20\x00",2) != 0) && (memcmp(gCityCode,"\x21\x53",2) != 0))
//  {
//	    gNotShCard = 0x55;
//	    goto label_not_shanghai_or_kunshan_changshu_wuxi_card;//外地卡
//  }*/
//  
//  //read 0010
//	for(ptr=1;ptr<2;ptr++) { //3,20090701,read 0010 record 1(gB29) only
//		memcpy(gBuf,"\x00\xb2\x00\x84\x10",5);
//		gBuf[2] = ptr+1;
//		gBufBytes = 5;
//		ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
//		if(ret)  {
//	  	   gDebugStep = 0x200d;
//	  	   goto label_err_read;
//		}
//		#ifdef _EMU_
//		printf("\niso_apdu 公共复合消费文件0010 no.%02d:",ptr+1);
//		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//		#endif
//		if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
//	  	   gDebugStep = 0x200e;
//	  	   goto label_err_read;
//		}
//	  //if (ptr==0)  memcpy(gB28,gBuf,16);
//		if (ptr==1)  memcpy(gB29,gBuf,16);
//	  //if (ptr==2)  memcpy(gB30,gBuf,16);  	   
//	}
//
//  //read 0011
//	for(ptr=0;ptr<3;ptr++) {
//	//memcpy(gBuf,"\x00\xb0\x91\x00\x30",5);
//		memcpy(gBuf,"\x00\xb2\x00\x8c\x10",5);
//		gBuf[2] = ptr+1;
//		gBufBytes = 5;
//		ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
//		if(ret)  {
//			gDebugStep = 0x200f;
//			goto label_err_read;
//		}
//		#ifdef _EMU_
//		printf("\niso_apdu 地铁复合消费文件0011 no.%02d:",ptr+1);
//		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//		#endif
//		if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
//	  	   gDebugStep = 0x2010;
//	  	   goto label_err_read;
//		}
//	  	   
//		if (ptr==0)  memcpy(gB32,gBuf,16);
//		if (ptr==1)  memcpy(gB33,gBuf,16);
//		if (ptr==2)  memcpy(gB34,gBuf,16);		   
//	}
//
//	return 0;
//
//label_not_shanghai_card:
//#ifdef _EMU_
//	printf("\n非上海卡处理");
//#endif
//	if((memcmp(gCityCode,CityCode_KunShan,2) != 0)&&(memcmp(gCityCode,CityCode_ChangShu,2) != 0)&&(memcmp(gCityCode,CityCode_WuXi,2) != 0)) 
//	{
//		goto label_not_shanghai_and_not_kunshan_changshu_wuxi_card;//外地卡
//	}
//#ifdef _EMU_
//	printf("\n昆山/常熟/无锡卡处理");
//#endif
////read 0011
//	for(ptr=0;ptr<3;ptr++) {
//		//memcpy(gBuf,"\x00\xb0\x91\x00\x30",5);
//		memcpy(gBuf,"\x00\xb2\x00\x8c\x10",5);
//		gBuf[2] = ptr+1;
//		gBufBytes = 5;
//		ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
//		if(ret)  {
//			gDebugStep = 0x200f;
//			goto label_err_read;
//		}
//#ifdef _EMU_
//		printf("\niso_apdu 地铁复合消费文件0011 no.%02d:",ptr+1);
//		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//#endif
//		if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
//			gDebugStep = 0x2010;
//			goto label_err_read;
//		}
//		
//		if (ptr==0)  memcpy(gB32,gBuf,16);
//		if (ptr==1)  memcpy(gB33,gBuf,16);
//		if (ptr==2)  memcpy(gB34,gBuf,16);		   
//	}
//
//	//read 0017
//	for(ptr=0;ptr<1;ptr++) {
//		//memcpy(gBuf,"\x00\xb2\x00\xbc\x30",5);
//		//gBuf[2] = ptr+1;
//		memcpy(gBuf,"\x00\xb2\x09\xb8\x30",5);//按照复合交易类型读记录,2011-7-30 10:13:42
//		gBufBytes = 5;
//		ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
//		if(ret)  {
//			gDebugStep = 0x298f;
//			goto label_err_read;
//		}
//	#ifdef _EMU_
//		printf("\niso_apdu 全国复合消费文件0017 no.%02d:",ptr+1);
//		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//	#endif
//		if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
//			gDebugStep = 0x2990;
//			goto label_err_read;
//		}
//	
//		memset(gB28,0,16);
//		memset(gB29,0,16);
//		memset(gB30,0,16);
//		memcpy(gRec0017,gBuf,0x30);	   
//	#ifdef _EMU_
//	printf("\ngRec0017:");
//	for(i=0;i<48;i++) printf(" %02x",(UBYTE)gRec0017[i]);
//		printf("\ngBuf[26]=%02x:",gBuf[26]);
//		printf("\ngBuf[26]&0xc0=%02x",gBuf[26]&0xc0);
//	#endif
//
//		if(gB34[0] == 0x55) 
//		{
//			if(memcmp(&gRec0017[28],&gB32[1],5) > 0) {
//				if ((uchar)gBuf[26] == (uchar)0x66) 
//				{   //出站 
//					memset(gB33,0,16);
//					memcpy(gB33,&gBuf[27],16);
//					lrc_gen2(gB33,15);//
//					//进站信息
//					memset(gB32,0,16);
//					gB32[0] = 0x41;
//					lrc_gen2(gB32,15);
//					//
//					memset(gB34,0,16);
//					gB34[0] = 0x66;
//					gB34[15] = 0x66;
//				}  
//				else if((uchar)gBuf[26] == (uchar)0x55)
//				{   //进站 
//					memset(gB32,0,16);
//					memcpy(gB32,&gBuf[27],16);
//					lrc_gen2(gB32,15);//
//					//出站信息
//					memset(gB33,0,16);
//					gB33[0] = 0x81;
//					lrc_gen2(gB33,15);
//					//
//					memset(gB34,0,16);
//					gB34[0] = 0x55;
//					gB34[15] = 0x55;  	
//				}
//				else
//				{  //进站
//					memset(gB32,0,16);
//					gB32[0] = 0x41;
//					lrc_gen2(gB32,15);//
//					//出站信息
//					memset(gB33,0,16);
//					gB33[0] = 0x81;
//					lrc_gen2(gB33,15);
//					//
//					memset(gB34,0,16);
//					gB34[0] = 0x66;
//					gB34[15] = 0x66;
//				}
//			}
//		}
//		else 
//		{
//			if(memcmp(&gRec0017[28],&gB33[1],5) > 0) {
//				if ((uchar)gBuf[26] == (uchar)0x66) 
//				{   //出站 
//					memset(gB33,0,16);
//					memcpy(gB33,&gBuf[27],16);
//					lrc_gen2(gB33,15);//
//					//进站信息
//					memset(gB32,0,16);
//					gB32[0] = 0x41;
//					lrc_gen2(gB32,15);
//					//
//					memset(gB34,0,16);
//					gB34[0] = 0x66;
//					gB34[15] = 0x66;
//				}  
//				else if((uchar)gBuf[26] == (uchar)0x55)
//				{   //进站 
//					memset(gB32,0,16);
//					memcpy(gB32,&gBuf[27],16);
//					lrc_gen2(gB32,15);//
//					//出站信息
//					memset(gB33,0,16);
//					gB33[0] = 0x81;
//					lrc_gen2(gB33,15);
//					//
//					memset(gB34,0,16);
//					gB34[0] = 0x55;
//					gB34[15] = 0x55;  	
//				}
//				else
//				{  //进站
//					memset(gB32,0,16);
//					gB32[0] = 0x41;
//					lrc_gen2(gB32,15);//
//					//出站信息
//					memset(gB33,0,16);
//					gB33[0] = 0x81;
//					lrc_gen2(gB33,15);
//					//
//					memset(gB34,0,16);
//					gB34[0] = 0x66;
//					gB34[15] = 0x66;
//				}
//			}
//		}
//	}
//
//	return 0;
//  
//label_not_shanghai_and_not_kunshan_changshu_wuxi_card:
//#ifdef _EMU_
//printf("\n非上海卡也非昆山/常熟/无锡卡处理");
//#endif
//	//read 0017
//	for(ptr=0;ptr<1;ptr++) {
//	  //memcpy(gBuf,"\x00\xb2\x00\xbc\x30",5);
//	  //gBuf[2] = ptr+1;
//		memcpy(gBuf,"\x00\xb2\x09\xb8\x30",5);//按照复合交易类型读记录,2011-7-30 10:13:42
//		gBufBytes = 5;
//		ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
//		if(ret)  {
//	  	   gDebugStep = 0x298f;
//	  	   goto label_err_read;
//		}
//	  #ifdef _EMU_
//		printf("\niso_apdu 全国复合消费文件0017 no.%02d:",ptr+1);
//		for (i=0;i<gBufBytes;i++)  printf(" %02x",gBuf[i]);
//	  #endif
//		if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
//	  	   gDebugStep = 0x2990;
//	  	   goto label_err_read;
//		}
//	  
//		memset(gB28,0,16);
//		memset(gB29,0,16);
//		memset(gB30,0,16);
//		memcpy(gRec0017,gBuf,0x30);	   
//	  #ifdef _EMU_
//		printf("\ngBuf[26]=%02x:",gBuf[26]);
//		printf("\ngBuf[26]&0xc0=%02x",gBuf[26]&0xc0);
//	  #endif
//		if ((uchar)gBuf[26] == (uchar)0x66) 
//		{  //出站 
//			memset(gB33,0,16);
//			memcpy(gB33,&gBuf[27],16);
//			lrc_gen2(gB33,15);//
//			//进站信息
//			memset(gB32,0,16);
//			gB32[0] = 0x41;
//			lrc_gen2(gB32,15);
//			//
//			memset(gB34,0,16);
//			gB34[0] = 0x66;
//			gB34[15] = 0x66;
//		}  
//		else if((uchar)gBuf[26] == (uchar)0x55)
//		{  //进站 
//			memset(gB32,0,16);
//		     memcpy(gB32,&gBuf[27],16);
//		     lrc_gen2(gB32,15);//
//		     //出站信息
//		     memset(gB33,0,16);
//		     gB33[0] = 0x81;
//		     lrc_gen2(gB33,15);
//		     //
//		     memset(gB34,0,16);
//		     gB34[0] = 0x55;
//		     gB34[15] = 0x55;  	
//		}
//		else
//		{  //进站
//		     memset(gB32,0,16);
//		     gB32[0] = 0x41;
//		     lrc_gen2(gB32,15);//
//		     //出站信息
//		     memset(gB33,0,16);
//		     gB33[0] = 0x81;
//		     lrc_gen2(gB33,15);
//		     //
//			memset(gB34,0,16);
//			gB34[0] = 0x66;
//			gB34[15] = 0x66;
//		}
//	}  	
//    	   
	return 0;
//
//label_err_card_moved:
//	gResponseStatus = CE_CARDREMOVED;
//#ifdef _EMU_
//	printf("\nError: card removed");
//#endif
//	return (char)-1;
//
//label_err_read:
//	gResponseStatus = CE_READERROR;
//#ifdef _EMU_
//	printf("\nError: read card");
//#endif
//	return (char)-1;
//
//label_invalid:
//	gResponseStatus = CE_INVALIDCARD;
//#ifdef _EMU_
//	printf("\nError: card invalid.");
//#endif
//	return (char)-1;
//
//label_err_need_m1_restore:
//	gResponseStatus = CE_NEED_M1_RESTORE;
//	gNeedM1RestoreFlag = 0x55;
//#ifdef _EMU_
//	printf("\nErr:trans_getcardinfo_cpu-->need to restore in m1 mode.");
//#endif
//	return (char)-1;
}
char sam_getnewkeya(void)
{
#ifdef _EMU_  
printf("\nsam_getnewkeya()");
#endif	
	
unsigned char i;
unsigned char ret;

	gDebugStep = 0x14a1;
	gBuf[0] = 0;   //keya
	gBuf[1] = 0xfe; //0xff; 1111 1110
	gBuf[2] = 0x03; //0xff; 0000 0011
	gBuf[3] = 0x05;   //用于计算的Key的索引
	gBuf[4] = 0x05;   //用于校验卡认证码的Key索引
	if(gB1[0] == 0x18) gBuf[5] = 0x18;
	else gBuf[5] = 0;

	gBuf[6] = gB1[6];//gCityCode[0];
	gBuf[7] = gB1[7];//gCityCode[1];

	gBuf[8] = gB1[8];//gCardIssueNo[0];
	gBuf[9] = gB1[9];//gCardIssueNo[1];
	gBuf[10] = gB1[10];//gCardMac[0];
	gBuf[11] = gB1[11];//gCardMac[1];
	gBuf[12] = gB1[12];//gCardMac[2];
	gBuf[13] = gB1[13];//gCardMac[3];
  
	memcpy(&gBuf[14],gCardDir,16);
	if(gB1[0] == 0x18)  memset(&gBuf[19],gCardDir[4],5);
  	  
	ret =  hl_get_key(gBuf);
	if(ret){
	#ifdef _EMU_
		printf("\nErr:hl_get_key,ret=%d",ret);
	#endif
		gDebugStep = 0x14a2;
		gResponseStatus = CE_KEY_CALERROR;
		return 1;
	}
#ifdef _EMU_  
	printf("\nOK:sam_getnewkeya()");
#endif

	gDebugStep = 0x14a3;
	//for(i=0;i<4;i++) gNewACardSnr[i]=gThisCardSnr[i];
	return 0;
}


/*************************************************************************
**************************************************************************/
char sam_getnewkeyb(void)
{
#ifdef _EMU_  
printf("\nsam_getnewkeyb()");
#endif
	

unsigned char i;
unsigned char ret;

	gDebugStep = 0x14e0;
 
    if(gNewACardSnr[0] != gThisCardSnr[0]) goto label_con;
    if(gNewACardSnr[1] != gThisCardSnr[1]) goto label_con;
    if(gNewACardSnr[2] != gThisCardSnr[2]) goto label_con;
    if(gNewACardSnr[3] != gThisCardSnr[3]) goto label_con;
    #ifdef _EMU_
    printf("\nSkip get new key b");
    #endif
    return 0;

label_con:
	gDebugStep = 0x14e1;
	gBuf[0] = 1;   //keyb
	gBuf[1] = 0xff; 
	gBuf[2] = 0xff;
	gBuf[3] = 0x07;   //用于计算的Key的索引
	gBuf[4] = 0x07;   //用于校验卡认证码的Key索引
	gBuf[5] = 0;

	gBuf[6] = gCityCode[0];
	gBuf[7] = gCityCode[1];

	gBuf[8] = gCardIssueNo[0];
	gBuf[9] = gCardIssueNo[1];
	gBuf[10] = gCardMac[0];
	gBuf[11] = gCardMac[1];
	gBuf[12] = gCardMac[2];
	gBuf[13] = gCardMac[3];
  
	memcpy(&gBuf[14],gCardDir,16);

	ret =  hl_get_key(gBuf);
	if(ret){
		#ifdef _EMU_
		printf("\nErr:hl_get_key,ret=%d",ret);
		#endif
		gDebugStep = 0x14e2;
		gResponseStatus = CE_KEY_CALERROR;
	return 1;
	}
#ifdef _EMU_  
printf("\nOK:sam_getnewkeyb()");
#endif

	gDebugStep = 0x14e3;
	for(i=0;i<4;i++) gNewACardSnr[i]=gThisCardSnr[i];
	return 0;
}


char jtb_credit_ini(void)
{
unsigned char ret;
#ifdef _EMU_
char i;
#endif

watchdog();
#ifdef _EMU_
printf("\ntrans_credit_ini now");
#endif

	gDebugStep = 0x2101;
	//initial for load
	memcpy(gBuf,"\x80\x50\x00\x02\x0b",5);
	gBuf[5] = 0x1;//密钥索引号
	udword_to_buf(gCardThisTransValue,&gBuf[6]);//交易金额
	memcpy(&gBuf[10],FM_Module_NO,6);//initial for load终端机编号
	gBufBytes = 16;
	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
	if(ret)  {
		gDebugStep = 0x2102;
		gResponseStatus = CE_WRITEERROR;  
		return (char)-1;
	}
	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x62) && ((uchar)gBuf[gBufBytes-1] == (uchar)0x81))  {
		gDebugStep = 0x2103;
		goto label_err_need_m1_restore;
	}

	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
		gDebugStep = 0x2104;
		gResponseStatus = CE_WRITEERROR;  
		return (char) -1;
	}

	return 0;

label_err_need_m1_restore:
	gResponseStatus = CE_NEED_M1_RESTORE;
	return (char) -1;
}



char jtb_credit(void)
{
unsigned char ret;
#ifdef _EMU_
char i;
#endif

watchdog();

#ifdef _EMU_
printf("\ntrans_credit now");
#endif	

	gTransType = 0x2;

	gDebugStep = 0x2201;
	//credit for load
	memcpy(gBuf,"\x80\x52\x00\x00\x0b",5);
	memcpy(&gBuf[5],gTransTime,7);//date4+time3
	memcpy(&gBuf[12],gMAC,4);//mac2
	gBuf[16] = 0x04;
	gBufBytes = 17;
	ret = iso_apdu_access(gBuf,gBufBytes,gBuf,&gBufBytes);
	if(ret)  {
		gDebugStep = 0x2202;
		gResponseStatus = CE_WRITEERROR;  
		return (char)-1;
	}
	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x69) && (gBuf[gBufBytes-1] == 0x1))  {
		gDebugStep = 0x2203;
		gResponseStatus = CE_WRITEERROR;  
		return (char) -1;
	}
	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x69) && (gBuf[gBufBytes-1] == 0x85))  {
		gDebugStep = 0x2204;
		gResponseStatus = CE_WRITEERROR;  
		return (char) -1;
	}
	if(((uchar)gBuf[gBufBytes-2] == (uchar)0x93) && (gBuf[gBufBytes-1] == 0x2))  {
		gDebugStep = 0x2205;
		gResponseStatus = CE_MAC2ERR;  
		return (char) -1;
	}
	if(((uchar)gBuf[gBufBytes-2] != (uchar)0x90) || (gBuf[gBufBytes-1] != 0))  {
		gDebugStep = 0x2206;
		gResponseStatus = CE_WRITEERROR;  
		return (char) -1;
	}
  
	memcpy(gTAC,gBuf,4);
	gDebugStep = 0x2208;
	return 0;	
}

#endif