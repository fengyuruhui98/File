//sh_sam.c


#ifndef _SH_SAM_C_
#define _SH_SAM_C_

UBYTE bgSHpsamIndex;					//上海公交卡PSAM编号
UBYTE bpgSHpsamNo[4];					//上海公交卡PSAM卡号
UBYTE bpgSmpsamAppSjtFlag=1;			//1:单程票应用,0:浦发应用
UBYTE bgSmpsamIndex;					//上海地铁PSAM编号
UBYTE	bgSmpsamADF[2];					//
UBYTE bpgSmpsamNo[4];					//上海地铁PSAM卡号

UBYTE bpgSmpsamNu[6];					//上海地铁PSAM卡号

UBYTE bgSMobilepsamIndex;				//上海地铁手机支付PSAM编号
UBYTE bpgSMobilepsamNo[6];			//上海地铁手机支付PSAM卡号

UBYTE bgExpopsamIndex;					//上海世博PSAM编号
UBYTE bpgExpopsamNo[4];					//上海世博PSAM卡号

UBYTE bgSHPsamValid=0;
UBYTE bgSmPsamValid=0;
UBYTE bgSHJTBPsamValid=0;//2019-06-21
UBYTE bgSMobilePsamValid=0;
UBYTE bgExposamValid=0;

UBYTE SHEXPOMODE;

//20190326 Add 
UBYTE bgSHJTBpsamIndex;					//上海交通部PSAM编号
UBYTE ch_transport_psam_id[6];	//交通部SAM卡号

/*======================================================================
函数：ModifySamID
功能：改写PSAM卡号
======================================================================*/
int ModifySamID(UBYTE *newid)
{
	int ret,retry,i;
	UBYTE outbuf[257]; 
	UBYTE outbytes;
	UBYTE inbuf[40];
	UBYTE inbytes;
	
	i = 0;
//	bgSHPsamValid = 0;
	bgSHpsamIndex = SAMLOCATION_5;
	sam_select(bgSHpsamIndex);
	sam_set(bgSHpsamIndex,SAM_ETU_93,4);//
	for(retry=0;retry<3;retry++)
	{
		sam_set(bgSHpsamIndex,SAM_ETU_93,4);//
		ret = sam_atr(bgSHpsamIndex,outbuf,&outbytes);
		if(ret != 0)
		{
			continue;
		}
	if( i >= 2)		//2014/4/11 13:14:33 速率切换两次失败时 跳过切换
	   		goto sam_wo_pps;	
		//115200
#ifdef SAM_PPS_ON		
	  ret = sam_pts(bgSHpsamIndex,0x18);
	  if(ret != 0)
	  {
	    #ifdef _DEBUG_RESETSHANGHAIMOBILESAM_
      debug_printf("\n Err:ResetShangHaiMobileSam,pps,retry=%d",retry);
      #endif
	   	continue;
	  }
	  i++;
	  //delay_ms(20);
	  
	  sam_select(bgSHpsamIndex);
	  //sam_set(bgSHpsamIndex,SAM_ETU_31,16);
	  sam_set(bgSHpsamIndex,SAM_ETU_31,4);
	  //
#endif  

sam_wo_pps:
		
		memcpy(inbuf,"\x00\xd6\x96\x00\x06\x20\x00",7);
		memcpy(&inbuf[7],newid,4);
		inbytes = 11;
	  ret = sam_apdu(bgSHpsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
	  if(ret!=0)
	  {
			continue;
		}
		//if((outbuf[0]!=0x90)||(outbuf[1]!=0x00))
		//printf("sam_apdu ModifySam outbuf= %02X %02X",outbuf[outbytes-2],outbuf[outbytes-1]);
		if((outbuf[outbytes - 2] != 0x90) ||((UBYTE)outbuf[outbytes - 1] != 0x00))
		{
			//printf("sam_apdu ModifySam outbuf= %02X %02X",outbuf[outbytes-2],outbuf[outbytes-1]);
			continue;
		}
		memcpy(gPosID,"\x20\x00",2);
		memcpy(&gPosID[2],newid,4);
	  memcpy(bpgSHpsamNo,newid,4);
	  memcpy(inbuf,"\x00\xa4\x00\x00\x02\x10\x01",7);
  	inbytes = 7;
  	ret = sam_apdu(bgSHpsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
  	if(ret != 0)
  	{
	    continue;
    }	
  	if((outbuf[outbytes - 2 ] != 0x61) &&((UBYTE)outbuf[outbytes - 2] != 0x90))
  	{
	    continue;
  	}
  	gPSAMFileFlag = 0x55;
  	bgSHPsamValid = 1;
  	return 0;	
	}
	memcpy(inbuf,"\x00\xa4\x00\x00\x02\x10\x03",7);
	inbytes = 7;
	ret = sam_apdu(bgSHpsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
	if(ret != 0)
	{
		bgSHPsamValid = 0;
    return -1;
  }	
	//if((outbuf[0] != 0x61) &&((UBYTE)outbuf[0] != 0x90))
	if((outbuf[outbytes - 2] != 0x61) &&((UBYTE)outbuf[outbytes - 2] != 0x90))
	{
		bgSHPsamValid = 0;
    return -1;
	}
	gPSAMFileFlag = 0x00;
	bgSHPsamValid = 1;
	return 0;	
//	return -1;
}
/*======================================================================
函数：ResetShangHaiCardSam
功能：PSAM初始化
========================================================================*/
#ifdef DUBUG_PRINT
#define _DEBUG_ResetShangHaiCardSam_
#endif
int ResetShangHaiCardSam(void)
{
	int ret,retry;
	UBYTE outbuf[257]; 
	UBYTE outbytes;
	UBYTE inbuf[257];
	UBYTE inbytes;
	int i;
	i = 0;
	#ifdef _DEBUG_ResetShangHaiCardSam_
			debug_printf("\n ResetShangHaiCardSam\n");
	#endif	

	gDebugStep=0x1000;
	bgSHpsamIndex = SAMLOCATION_5;
	sam_select(bgSHpsamIndex);
	//sam_set(bgSHpsamIndex,SAM_ETU_93,16);
	sam_set(bgSHpsamIndex,SAM_ETU_93,4);
	memset(bpgSHpsamNo,0,4);
	bgSHPsamValid = 0;
	for(retry=0;retry<3;retry++)
	{
		watchdog();
		//sam_set(bgSHpsamIndex,SAM_ETU_93,16);
		sam_set(bgSHpsamIndex,SAM_ETU_93,4);
		ret = sam_atr(bgSHpsamIndex,outbuf,&outbytes);
		gDebugStep=0x1001;
	  	if(ret != 0)
	  	{
	    	continue;
	    }
	if( i >= 2)		//2014/4/11 13:14:33 速率切换两次失败时 跳过切换
	   		goto sam_wo_pps; 
	   //115200
#ifdef SAM_PPS_ON	
		#ifdef DUBUG_PRINT   
			printf("\n Card_SAM_PPS115200");
		#endif
	  ret = sam_pts(bgSHpsamIndex,0x18);
	  if(ret != 0)
	  {
	    #ifdef _DEBUG_RESETSHANGHAIMOBILESAM_
      debug_printf("\n Err:ResetShangHaiMobileSam,pps,retry=%d",retry);
      #endif
	   	continue;
	  }
	  i++;
	  //delay_ms(20);
	  
	  sam_select(bgSHpsamIndex);
	  //sam_set(bgSHpsamIndex,SAM_ETU_31,16);
	  sam_set(bgSHpsamIndex,SAM_ETU_31,4);	//
	  //
#endif	  

sam_wo_pps:    	
	
		gDebugStep=0x1002;
	 	memcpy(inbuf,"\x00\xb0\x96\x00\x06",5);
	  	inbytes = 5;
	  	ret = sam_apdu(bgSHpsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
	  	
	  	#ifdef DUBUG_PRINT   
		  	printf("sam_apdu CardSam outbuf= ");
		  	for(i=0; i<outbytes;i++)
		  		printf(" %02X",outbuf[i]);
		  	printf("\n");
	  	#endif
	  	if(ret != 0)
	  	{
		    continue;
	    }	
	    gDebugStep=0x1003;
	  	if(outbytes != 8)
	  	{
		    continue;
	  	}
	  	gDebugStep=0x1004;
	  	memcpy(gPosID,outbuf,6);
	  	memcpy(bpgSHpsamNo,&outbuf[2],4);

		//#ifndef _USEOLDPSAM_
	  	memcpy(inbuf,"\x00\xa4\x00\x00\x02\x10\x01",7);
	  	inbytes = 7;
	  	ret = sam_apdu(bgSHpsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
	  	if(ret != 0)
	  	{
		    continue;
	    }	
	    gDebugStep=0x1005;
	  	//if((outbuf[0] != 0x61) &&((UBYTE)outbuf[0] != 0x90))
	  	if((outbuf[outbytes - 2] != 0x61) &&((UBYTE)outbuf[outbytes - 2] != 0x90))
	  	{
		    goto label_oldpsam;
	  	}
	  	memcpy(inbuf,"\x00\xb0\x97\x00\x01",5);
	  	inbytes = 5;
	  	ret = sam_apdu(bgSHpsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
	  	if(ret != 0)
	  	{
		    continue;
	    }	
	    if(outbytes != 3)
	  	{
		    continue;
	  	}
	  	gKeyVersion = outbuf[0];
	  	gPSAMFileFlag = 0x55;
	  	bgSHPsamValid = 1;
	  	return 0;	
//	  #else
label_oldpsam:
	 		memcpy(inbuf,"\x00\xa4\x00\x00\x02\x10\x03",7);
	  	inbytes = 7;
	  	ret = sam_apdu(bgSHpsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
	  	if(ret != 0)
	  	{
		    continue;
	    }	
	    gDebugStep=0x1005;
	  	//if((outbuf[0] != 0x61) &&((UBYTE)outbuf[0] != 0x90))
	  	if((outbuf[outbytes - 2] != 0x61) &&((UBYTE)outbuf[outbytes - 2] != 0x90))
	  	{
		    continue;
	  	}
	  	gPSAMFileFlag = 0x00;
//	 	#endif
	  	bgSHPsamValid = 1;
	  	return 0;	
	}
	//
	return -1;	
}	

/*======================================================================
函数：ResetShangHaiCardSam
功能：Transport PSAM初始化
========================================================================*/
//#ifdef DUBUG_PRINT
#define _DEBUG_ResetShTransportSam_
//#endif
int ResetShTransportSam(void)
{
int ret,retry;
	UBYTE outbuf[257]; 
	UBYTE outbytes;
	UBYTE inbuf[257];
	UBYTE inbytes;
	int i;
	i = 0;
#ifdef _DEBUG_ResetShTransportSam_
			debug_printf("\n ResetShTransportSam\n");
#endif	

	bgSHJTBPsamValid=0;
	bgSHJTBpsamIndex = SAMLOCATION_7;
	if(sam_select(bgSHJTBpsamIndex)!=0)
		printf("select ministry of transport psam error \n");
	sam_set(bgSHJTBpsamIndex,SAM_ETU_93,4);//38400
	//sam_set(bgSHpsamIndex,SAM_ETU_372,4);//9600

//		if(sam_select(i) != 0)
//		//	continue;
//			printf("select ministry of transport psam error \n");
//		//交通部正式psam卡波特率38400
//		sam_set(i, SAM_ETU_93, 4);
//		//测试波特率是9600
//		//sam_set(i, SAM_ETU_372, 4);
		for(retry = 0; retry < 3; retry++)
		{
		  	if((ret = sam_atr(bgSHJTBpsamIndex,outbuf,&outbytes)) != 0)
		  	{
		  		printf("sh-tranpsort sam atr return %02x\n", ret);
		    	continue;
		    }
		    printf("\nttb atr:");
		    for(i = 0; i < outbytes; i++)
		    	printf("%02x", outbuf[i]);
		    printf("\n");
		    
//		    ret = sam_pts(bgSHJTBpsamIndex, 0x18);		//115200
//		    printf("sam pts return %d\n", ret);
//		    sam_set(bgSHJTBpsamIndex, SAM_ETU_31, 4);	//115200
		    
		    //sleep(1);
		    //device id
		 		memcpy(inbuf,"\x00\xb0\x96\x00\x06",5);
		  	inbytes = 5;
	  		if((ret = sam_apdu(bgSHJTBpsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0)) != 0)
		  	{
		  		printf("sam apdu read file 16 return %02x\n", ret);
		  		printf("outbuf =");
		  		for(i=0;i<ret;i++)
		  		{
		  			printf("%02x ",outbuf[i]);
			   	}
			   	printf("\n");
			   	continue;
	    	}	
		  	if(outbytes != 8)
		  	{
		  		printf("file 16 len %02x status %02x %02x\n", outbytes, outbuf[0], outbuf[1]);
			    continue;
	  		}
	  		bgSHJTBPsamValid=1;
#ifdef _DEBUG_ResetShTransportSam_
	  		printf("ResetSAM...inbuf :");
	  		for(i=0;i<inbytes;i++)
	  		{
	  			printf(" %02x",inbuf[i]);
	  		}
	  		printf("\n");
	  		printf("ResetSAM...outbuf :");
	  		for(i=0;i<outbytes;i++)
	  		{
	  			printf(" %02x",outbuf[i]);
	  		}
	  		printf("\n");
#endif	  		
		  	memcpy(ch_transport_psam_id, outbuf, 6);
		  	printf("transport psam id :%02x %02x %02x %02x %02x %02x\n", ch_transport_psam_id[0], ch_transport_psam_id[1], ch_transport_psam_id[2], 
		  			ch_transport_psam_id[3], ch_transport_psam_id[4], ch_transport_psam_id[5]);
		  	//密钥索引		
//				memcpy(inbuf,"\x00\xb0\x97\x00\x01",5);
//	  		inbytes = 5;
//	  		ret = sam_apdu(bgSHpsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
//	  		printf("key_index: ");
//				for(i = 0; i < outbytes; i++) printf("%02x", outbuf[i]);
//				printf("\n");
//	  		if(ret != 0)
//	  		{
//		   	 continue;
//	    	}	
			  //select pboc psam file 8011
	  		memcpy(inbuf,"\x00\xa4\x00\x00\x02\x80\x11", 7);
	  		inbytes = 7;
		  	if(sam_apdu(bgSHJTBpsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0) != 0)
		  	{
			    continue;
	    	}
	    	printf("select tranport 8011 len %02x:%02x%02x\n", outbytes, outbuf[0], outbuf[1]);
		  	if((outbuf[0] == 0x61) && (outbytes == 2))
			  {
					memcpy(inbuf, "\x00\xc0\x00\x00", 4);
					inbuf[4] = outbuf[1];
					inbytes = 5;
					if(sam_apdu(bgSHJTBpsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0) != 0)
					{
						printf("select 8011 lenth %02x, %02x%02x\n", outbytes, outbuf[0], outbuf[1]);
						continue;
					}
				}
				printf("select 8011: ");
				for(i = 0; i < outbytes; i++) 
				{
					printf("%02x", outbuf[i]);
				}
				printf("\n");
				if((outbuf[outbytes - 2] != 0x90) || (outbuf[outbytes - 1] != 0))
					continue;
	  		
				break;
		}
}
/*======================================================================
函数：select_1001or1002_app
功能： 
========================================================================*/
//#define _DEBUG_switch_1001or1002_app
int select_1001or1002_app(UBYTE bpgSmpsamAppSjt)
{
	UBYTE outbuf[257];  
	UBYTE outbytes;
	UBYTE inbuf[257];
	UBYTE inbytes;
	int ret;
	bgSMPADF = bpgSmpsamAppSjt;
	//gDebugStep=0x1000;
	memcpy(bgSmpsamADF, "\x00\x00", 2);
	bgSmpsamIndex = SAMLOCATION_4;
	sam_select(bgSmpsamIndex);
	if(1 == bpgSmpsamAppSjt)
	{
			//if(1 == bpgSHpsamAppSjtFlag)return 0;
			memcpy(inbuf,"\x00\xa4\x00\x00\x02\x10\x01",7);
	}
	else{
		///if(0 == bpgSHpsamAppSjtFlag)return 0;
		memcpy(inbuf,"\x00\xa4\x00\x00\x02\x10\x02",7);
	}
	gDebugStep=0x4017;	
	inbytes = 7;
	ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
	if(ret != 0)
	{
			#ifdef _DEBUG_switch_1001or1002_app
				debug_printf("\n select1001or1002 send err\n");
		#endif
    return -1;
  }	
  gDebugStep=0x4018;
	if(((UBYTE)outbuf[0]!= (UBYTE)0x61) &&((UBYTE)outbuf[0]!= 0x90))
	{
			#ifdef _DEBUG_ResetShangHaiMetroSam_
				debug_printf("\n select1001or1002 answer err,get %d %d\n",outbuf[0],outbuf[1]);
			#endif
			return -2;
    
	} 
#ifdef _DEBUG_switch_1001or1002_app
				debug_printf("\n select1001or1002 ok\n");
#endif
	//bgSmPsamValid = 1;
	if(1 ==bpgSmpsamAppSjt)
		memcpy(bgSmpsamADF, "\x10\x01", 2);
		
	else
		memcpy(bgSmpsamADF, "\x10\x02", 2);
	return 0;	
	
}
/*======================================================================
函数：ResetShangHaiMetroSam
功能：PSAM初始化
========================================================================*/ 
#ifdef DEBUG_PRINT
#define _DEBUG_ResetShangHaiMetroSam_
#endif
int ResetShangHaiMetroSam(void)
{
	int ret,retry,i;
	UBYTE outbuf[257]; 
	UBYTE outbytes;
	UBYTE inbuf[257];
	UBYTE inbytes;
	gDebugStep=0x1000; 
	bgSmpsamIndex = SAMLOCATION_4;
	sam_select(bgSmpsamIndex);
	bgSMPADF=0;
	i = 0;
	//sam_set(bgSmpsamIndex,SAM_ETU_93,16);//FM,38400
	//sam_set(bgSmpsamIndex,SAM_ETU_372,16);//WD,9600  
	 	
	#ifdef _DEBUG_ResetShangHaiMetroSam_
			debug_printf("\n ResetShangHaiMetroSam\n");
	#endif	
	
	memset(bpgSmpsamNo,0,4); 
	bgSmPsamValid=0;
	for(retry=0;retry<3;retry++)
	{
		#ifdef _DEBUG_ResetShangHaiMetroSam_
			debug_printf("\n shmetro atr");
	  #endif
	  watchdog();
	  sam_select(bgSmpsamIndex);
  	sam_set(bgSmpsamIndex,SAM_ETU_93,16);//FM,38400
  	//sam_set(bgSmpsamIndex,SAM_ETU_372,16);//WD,9600 
		ret = sam_atr(bgSmpsamIndex,outbuf,&outbytes);
		gDebugStep=0x1001;
	  	if(ret != 0)
	  	{
	  		#ifdef _DEBUG_ResetShangHaiMetroSam_
						debug_printf("\n atr err");
	  		#endif	
	    	continue;
	    }
	   if( i >= 2)		//2014/4/11 13:14:33 速率切换两次失败时 跳过切换
	   		goto sam_wo_pps;
	   //115200
#ifdef SAM_PPS_ON		  
		#ifdef DUBUG_PRINT   
			debug_printf("\n Metro_SAM_PPS115200");
		#endif 
	  ret = sam_pts(bgSmpsamIndex,0x18);
	  if(ret != 0)
	  {
	    #ifdef _DEBUG_RESETSHANGHAIMOBILESAM_
      debug_printf("\n Err:ResetShangHaiMobileSam,pps,retry=%d",retry);
      #endif
	   	continue;
	  }
	    i++;
	  //delay_ms(20);
	  
	  sam_select(bgSmpsamIndex);
	  //sam_set(bgSmpsamIndex,SAM_ETU_31,16);
	  sam_set(bgSmpsamIndex,SAM_ETU_31,12);	//2014/4/11 9:54:07
	  //
#endif	  
	  
sam_wo_pps:
		  
	   #ifdef _DEBUG_ResetShangHaiMetroSam_
						debug_printf("\n get operator no");
	   #endif	
		gDebugStep=0x1002;
	 	memcpy(inbuf,"\x00\xb0\x96\x00\x06",5);
	  	inbytes = 5;
	  	ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
	  	if(ret != 0)
	  	{
	  		#ifdef _DEBUG_ResetShangHaiMetroSam_
						debug_printf("\n send getoper err");
	  		#endif
		    continue;
	    }	
	    gDebugStep=0x1003;
	  	if(outbytes != 8)
	  	{
	  		#ifdef _DEBUG_ResetShangHaiMetroSam_
						debug_printf("\n getoper answer bytes err get=%02x",outbytes);
	  		#endif
		    continue;
	  	} 
	  	gDebugStep=0x1004;
	  	memcpy(bpgSmpsamNo,&outbuf[2],4);
	  	memcpy(bpgSmpsamNu,outbuf,6);
	  	
	  #ifdef _DEBUG_ResetShangHaiMetroSam_
						debug_printf("\n oper no =%02x %02x %02x %02x",bpgSmpsamNo[0],bpgSmpsamNo[1],bpgSmpsamNo[2],bpgSmpsamNo[3]);
		#endif
		//ADF 1001:SJT
	  	memcpy(inbuf,"\x00\xa4\x00\x00\x02\x10\x01",7);
	  	inbytes = 7;
	  	ret = sam_apdu(bgSmpsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
	  	if(ret != 0)
	  	{
	  			#ifdef _DEBUG_ResetShangHaiMetroSam_
						debug_printf("\n selectfile1002 send err");
	  		#endif
		    continue;
	    }	
	    gDebugStep=0x1005;
	  	if(((UBYTE)outbuf[outbytes - 2]!= (UBYTE)0x61) &&((UBYTE)outbuf[outbytes -2]!= 0x90))
	  	{
	  			#ifdef _DEBUG_ResetShangHaiMetroSam_
						debug_printf("\n selectfile answer err,get %d %d",outbuf[0],outbuf[1]);
	  			#endif
		    continue;
	  	}
	  #ifdef _DEBUG_ResetShangHaiMetroSam_
						debug_printf("\n reset ok");
		#endif
	  	bgSmPsamValid = 1;
	  	memcpy(bgSmpsamADF, "\x10\x01", 2);
	  	return 0;	
	}
		#ifdef _DEBUG_ResetShangHaiMetroSam_
						debug_printf("\n reset err");
		#endif
	return -1;	
}
/*======================================================================
函数：ResetShangHaiExpoSam
功能：PSAM初始化
========================================================================*/ 
int ResetShangHaiExpoSam(void)
{
	int ret,retry;
	UBYTE outbuf[257]; 
	UBYTE outbytes;
	UBYTE inbuf[257];
	UBYTE inbytes;
	bgExpopsamIndex = SAMLOCATION_6;
	sam_select(bgExpopsamIndex);
	sam_set(bgExpopsamIndex,SAM_ETU_93,16);
	memset(bpgExpopsamNo,0,4);
	bgExposamValid = 0;
	
	for(retry=0;retry<3;retry++)
	{
		watchdog();
		ret = sam_atr(bgExpopsamIndex,outbuf,&outbytes);
		gDebugStep=0x1001;
	  if(ret != 0)
	  {
	   	continue;
	  }	
		gDebugStep=0x1002;
	 	memcpy(inbuf,"\x00\xb0\x96\x00\x06",5);
	  inbytes = 5;
	  ret = sam_apdu(bgExpopsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
	  if(ret != 0)
	  {
		   continue;
	  }	
	  gDebugStep=0x1003;
	  if(outbytes != 8)
	  {
		   continue;
	  }
	  gDebugStep=0x1004;
	  memcpy(bpgExpopsamNo,&outbuf[2],4);
	  memcpy(inbuf,"\x00\xa4\x00\x00\x02\x10\x01",7);
	  inbytes = 7;
	  ret = sam_apdu(bgExpopsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
	  if(ret != 0)
	  {
		   continue;
	  }	
	  gDebugStep=0x1005;
	  if(((UBYTE)outbuf[0]!= (UBYTE)0x61) &&((UBYTE)outbuf[0]!= 0x90))
	  {
		   continue;
	  }
	  bgExposamValid = 1;
	  return 0;	
	}
	return -1;	
	
	
}
/*======================================================================
函数：ResetShangHaiMobileSam
功能：手机支付PSAM初始化
========================================================================*/ 
#ifdef DEBUG_PRINT
#define _DEBUG_RESETSHANGHAIMOBILESAM_
#endif
int ResetShangHaiMobileSam(void)
{
	int ret,retry;
	UBYTE outbuf[257]; 
	UBYTE outbytes;
	UBYTE inbuf[257];
	UBYTE inbytes;

  #ifdef _DEBUG_RESETSHANGHAIMOBILESAM_
  debug_printf("\n ResetShangHaiMobileSam");
  #endif

	bgSMobilepsamIndex = SAMLOCATION_7;
	sam_select(bgSMobilepsamIndex);
	sam_set(bgSMobilepsamIndex,SAM_ETU_372,16);
	memset(bpgSMobilepsamNo,0,6);
	bgSMobilePsamValid = 0;
	
	for(retry=0;retry<3;retry++)
	{
	  watchdog();
	  sam_select(bgSMobilepsamIndex);
		sam_set(bgSMobilepsamIndex,SAM_ETU_372,16);
		ret = sam_atr(bgSMobilepsamIndex,outbuf,&outbytes);
		gDebugStep=0x1001;
	  if(ret != 0)
	  {
      #ifdef _DEBUG_RESETSHANGHAIMOBILESAM_
      debug_printf("\n Err:ResetShangHaiMobileSam,atr0,retry=%d",retry);
      #endif
	   	continue;
	  }	
	  ret = sam_pts(bgSMobilepsamIndex,0x13);
	  if(ret != 0)
	  {
	    #ifdef _DEBUG_RESETSHANGHAIMOBILESAM_
      debug_printf("\n Err:ResetShangHaiMobileSam,pps,retry=%d",retry);
      #endif
	   	continue;
	  }
	  
	  //delay_ms(20);
	  
	  sam_select(bgSMobilepsamIndex);
	  sam_set(bgSMobilepsamIndex,SAM_ETU_93,16);
		gDebugStep=0x1002;
	 	memcpy(inbuf,"\x00\xb0\x96\x00\x06",5);
	  inbytes = 5;
	  ret = sam_apdu(bgSMobilepsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
	  if(ret != 0)
	  {
	    #ifdef _DEBUG_RESETSHANGHAIMOBILESAM_
      debug_printf("\n Err:ResetShangHaiMobileSam,apdu0,retry=%d",retry);
      #endif
		   continue;
	  }	
	  gDebugStep=0x1003;
	  if(outbytes != 8)
	  {
		   continue;
	  }
	  gDebugStep=0x1004;
	  memcpy(bpgSMobilepsamNo,outbuf,6);
	  memcpy(inbuf,"\x00\xA4\x04\x00\x0E\x31\x50\x41\x59\x2E\x53\x59\x53\x2E\x41\x44\x46\x30\x31",19);
	  inbytes = 19;
	  ret = sam_apdu(bgSMobilepsamIndex,inbuf,inbytes,outbuf,&outbytes,0,0);
	  if(ret != 0)
	  {
	    #ifdef _DEBUG_RESETSHANGHAIMOBILESAM_
      debug_printf("\n Err:ResetShangHaiMobileSam,select apdu,retry=%d",retry);
      #endif
		   continue;
	  }	
	  gDebugStep=0x1005;
	  if(((UBYTE)outbuf[outbytes - 2]!= (UBYTE)0x61) &&((UBYTE)outbuf[outbytes - 2]!= 0x90))
	  {
		   continue;
	  }
	  bgSMobilePsamValid = 1;
	  return 0;	
	}
	return -1;		
	
}
/*******************************************
函数：SHGetKeyAStep1
功能：向task_key发送请求
*******************************************/
UBYTE SHGetKeyAStep1(UBYTE *keybuf,UBYTE n)
{
	if(n<11)
	{
		return 0xff;
	}
	memcpy(bgTaskKeyInbuf,keybuf,n);
	gbTaskKeyNum=n-10;
	set_task_key(TASK_KEY_GET_SHKEYA);
	return 0;
}
/*=====================================================================================
函数：SHGetKeyAStep2
功能：
=======================================================================================*/
UBYTE SHGetKeyAStep2(void)
{
	int i;
	
/*	sem_wait(&g_samcalreturn);
	if(bgTaskKeyFinishState==0)
	{
	  return 0;
	}
	return 1;*/
	
	for(i=0;i<1000;i++)
	{
		if(bgTaskKeyStep==TASK_KEY_FINISH) break;
		task_key();
		//delay_ms(1);	
	}
	if(i==1000)
	{
		return 1;
	}
	set_task_key(TASK_KEY_IDLE);
	if(bgTaskKeyFinishState==0)
	{
	  return 0;
	}
	return 1;
}	
/******************************************************
函数：SHTransTACStep1
功能：发送计算TAC的请求
******************************************************/
UBYTE SHTransTACStep1(UBYTE *tacbuf)
{
	memcpy(bgTaskKeyInbuf,KeyBuf,32);
	set_task_key(TASK_KEY_GET_SHTAC);
	return 0;
}
/*=====================================================================================
函数：SHGetTacStep2
功能：
=======================================================================================*/
UBYTE SHGetTacStep2(void)
{
	int i;
		
	/*sem_wait(&g_samcalreturn);
	if(bgTaskKeyFinishState==0)
	{
	  return 0;
	}
	return 1;*/

	for(i=0;i<1000;i++)
	{
		if(bgTaskKeyStep==TASK_KEY_FINISH) break;
    task_key();
    //delay_ms(1);
	}
	if(i==1000)
	{
		return 1;
	}
	set_task_key(TASK_KEY_IDLE);
	if(bgTaskKeyFinishState==0)
	{
	  return 0;
	}
	return 1;
}
/*******************************************
函数：SMGetKeyAStep1
功能：向task_key发送请求,计算上海单程票
*******************************************/
UBYTE SMGetKeyAStep1(UBYTE *keybuf,UBYTE n)
{
	if(n!=14)
	{
		return 0xff;
	}
	memcpy(bgTaskKeyInbuf,keybuf,n);
	set_task_key(TASK_KEY_GET_SMKEYA);
	return 0;
}
/*=====================================================================================
函数：SMGetKeyAStep2
功能：
=======================================================================================*/
UBYTE SMGetKeyAStep2(void)
{
	int i;

	/*sem_wait(&g_samcalreturn);
	//set_task_key(TASK_KEY_IDLE);
	if(bgTaskKeyFinishState==0)
	{
	  return 0;
	}
	return 1;*/
	//不需要
	for(i=0;i<1000;i++)
	{
		if(bgTaskKeyStep==TASK_KEY_FINISH) break;
		task_key();
		//delay_ms(1);
	}
	if(i==1000)
	{
		return 1;
	}
	set_task_key(TASK_KEY_IDLE);
	if(bgTaskKeyFinishState==0)
	{
	  return 0;
	}
	return 1;
}	
/******************************************************
函数：SHMetroTACStep1
功能：发送计算TAC的请求
******************************************************/
UBYTE SHMetroTACStep1(UBYTE *tacbuf)
{
	memcpy(bgTaskKeyInbuf,KeyBuf,32);
	set_task_key(TASK_KEY_GET_SMTAC);
	return 0;
}
/*=====================================================================================
函数：SMGetTacStep2
功能：
=======================================================================================*/
UBYTE SMGetTacStep2(void)
{
	int i;
	
	//20130901
	/*sem_wait(&g_samcalreturn);
	if(bgTaskKeyFinishState==0)
	{
	  return 0;
	}
	return 1;*/
	//NO	
	for(i=0;i<1000;i++)
	{
		if(bgTaskKeyStep==TASK_KEY_FINISH) break;
    task_key();
    //delay_ms(1);
	}
	if(i==1000)
	{
		return 1;
	}
	set_task_key(TASK_KEY_IDLE);
	if(bgTaskKeyFinishState==0)
	{
	  return 0;
	}
	return 1;
}
/*=====================================================================================
函数：SHSendAuthResp
功能：
=======================================================================================*/
UBYTE SHSendAuthResp(void)
{
	int i;
#ifdef _DEBUG_BOM_
debug_printf("\nSHSendAuthResp...");
#endif	
/*	
	sem_wait(&g_samcalreturn);
	if(bgTaskKeyFinishState==0)
	{
#ifdef _DEBUG_BOM_
debug_printf("\nSHSendAuthResp ok");
#endif
	  return 0;
	}
	return 1;	
*/	
	for(i=0;i<1000;i++)
	{
		if(bgTaskKeyStep==TASK_KEY_FINISH) break;
		task_key();
		//if(bgEnUCOS) OSTimeDly(1);
    //delay_ms(1);
	}
	if(i==1000)
	{
		return 1;
	}
	//bgTaskKeyStep = TASK_KEY_IDLE;
	set_task_key(TASK_KEY_IDLE);
	if(bgTaskKeyFinishState==0)
	{
#ifdef _DEBUG_BOM_
debug_printf("\nSHSendAuthResp ok");
#endif
	  return 0;
	}
	return 1;
}
/*=====================================================================================
函数：SEGetAuthCodeStep1
功能：计算世博发行认证码
=======================================================================================*/
UBYTE SEGetAuthCodeStep1(UBYTE *issuebuf)
{
	//os_lock();
	memcpy(bgTaskKeyInbuf,issuebuf,16);
	//bgTaskKeyStep=TASK_KEY_GET_SEAUTHCODE;
	set_task_key(TASK_KEY_GET_SEAUTHCODE);
	//os_unlock();
	//if(bgEnUCOS) OSTimeDly(1);
	return 0;
}
/*=====================================================================================
函数：SEGetKeyStep1
功能：计算世博密钥
=======================================================================================*/
UBYTE SEGetKeyStep1(UBYTE *keybuf,UBYTE SectorNum,UBYTE mode)
{
	//os_lock();
	memcpy(bgTaskKeyInbuf,keybuf,29+SectorNum);
	SHEXPOMODE=mode;
	gbTaskKeyNum=SectorNum;
	//bgTaskKeyStep=TASK_KEY_GET_SEKEY;
	set_task_key(TASK_KEY_GET_SEKEY);
	//os_unlock();
	//if(bgEnUCOS) OSTimeDly(1);
	return 0;
}
/*=====================================================================================
函数：COMMONGetSAMStep2
功能：
=======================================================================================*/
UBYTE COMMONGetSAMStep2(void)
{
	int i;
		
	for(i=0;i<1000;i++)
	{
		if(bgTaskKeyStep==TASK_KEY_FINISH) break;
		task_key();
		//if(bgEnUCOS) OSTimeDly(1);
		//delay_ms(1);
	}
	if(i==1000)
	{
		return 1;
	}
	//bgTaskKeyStep = TASK_KEY_IDLE;
	set_task_key(TASK_KEY_IDLE);
	if(bgTaskKeyFinishState==0)
	{
	  return 0;
	}
	return 1;
}
/*******************************************
函数：SMGetKeyAStep1
功能：向task_key发送请求,计算二维码消费密钥
*******************************************/
UBYTE QRGetKeyAStep1(UBYTE *KeyBuf)
{
	memcpy(bgTaskKeyInbuf,KeyBuf,15);
	set_task_key(TASK_KEY_GET_QRKEYA);
	return 0;
}

UBYTE QRGetKeyAStep2(void)
{
	int i;

	/*sem_wait(&g_samcalreturn);
	//set_task_key(TASK_KEY_IDLE);
	if(bgTaskKeyFinishState==0)
	{
	  return 0;
	}
	return 1;*/
	//不需要
	for(i=0;i<1000;i++)
	{
		if(bgTaskKeyStep==TASK_KEY_FINISH) break;
		task_key();
		//delay_ms(1);
	}
	if(i==1000)
	{
		return 1;
	}
	set_task_key(TASK_KEY_IDLE);
	if(bgTaskKeyFinishState==0)
	{
		return 0;
	}
	return 1;
}	
/*******************************************
函数：SMGetKeyAStep1
功能：向task_key发送请求,计算上海二维码QR TAC
*******************************************/
UBYTE SHQRTACStep1(UBYTE *tacbuf)
{
	memcpy(bgTaskKeyInbuf,tacbuf,32);
	set_task_key(TASK_KEY_GET_QRTAC);
	return 0;
}

UBYTE SHQRTACStep2()
{
	int i;
	
	//20130901
	/*sem_wait(&g_samcalreturn);
	if(bgTaskKeyFinishState==0)
	{
	  return 0;
	}
	return 1;*/
	//NO	
	for(i=0;i<1000;i++)
	{
		if(bgTaskKeyStep==TASK_KEY_FINISH) break;
    task_key();
    //delay_ms(1);
	}
	if(i==1000)
	{
		return 1;
	}
	set_task_key(TASK_KEY_IDLE);
	if(bgTaskKeyFinishState==0)
	{
	  return 0;
	}
	return 1;
}

/******************************************************
函数：SHODATACStep1
功能：向task_key发送请求,计算上海ODA TAC
******************************************************/
UBYTE SHODATACStep1(UBYTE *tacbuf)
{
	memcpy(bgTaskKeyInbuf,tacbuf,32);
	set_task_key(TASK_KEY_GET_ODATAC);
	return 0;
}

UBYTE SHODATACStep2(void)
{
	int i;
	
	//20130901
	/*sem_wait(&g_samcalreturn);
	if(bgTaskKeyFinishState==0)
	{
	  return 0;
	}
	return 1;*/
	//NO	
	for(i=0;i<1000;i++)
	{
		if(bgTaskKeyStep==TASK_KEY_FINISH) break;
    task_key();
    //delay_ms(1);
	}
	if(i==1000)
	{
		return 1;
	}
	set_task_key(TASK_KEY_IDLE);
	if(bgTaskKeyFinishState==0)
	{
	  return 0;
	}
	return 1;
}


/******************************************************
函数：SHJTBTACStep1
功能：向task_key发送请求,计算上海ODA TAC
******************************************************/
//UBYTE SHJTBTACStep1(UBYTE *tacbuf)
//{
//	memcpy(bgTaskKeyInbuf,tacbuf,32);
//	set_task_key(TASK_KEY_GET_JTBTAC);
//	return 0;
//}
//
//UBYTE SHJTBTACStep2(void)
//{
//	int i;
//	
//	//20130901
//	/*sem_wait(&g_samcalreturn);
//	if(bgTaskKeyFinishState==0)
//	{
//	  return 0;
//	}
//	return 1;*/
//	//NO	
//	for(i=0;i<1000;i++)
//	{
//		if(bgTaskKeyStep==TASK_KEY_FINISH) break;
//    task_key();
//    //delay_ms(1);
//	}
//	if(i==1000)
//	{
//		return 1;
//	}
//	set_task_key(TASK_KEY_IDLE);
//	if(bgTaskKeyFinishState==0)
//	{
//	  return 0;
//	}
//	return 1;
//}
#endif