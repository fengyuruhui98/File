//iso_block.c
//编制:邓建华
//时间:20090512

#ifndef _ISO_BLOCK_C_
#define _ISO_BLOCK_C_
//start of file

UBYTE bgCID;
UBYTE bgPCB;
UBYTE bgCIDFlag;
UWORD wgFWT;
UBYTE bgSFGI;
UBYTE bgWTX;

#define CID_FLAG     0x08       //=0x08: 有CID
#define MAX_RF_BUF   48 

//UBYTE bgISOType=ISO14443_TYPE_A;

/*=======================================================================================
函数:
功能:
入口参数:
inbuf:含PCB等
=========================================================================================*/
UBYTE iso_block_transceve(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes,UWORD timeout)
{
UWORD  i;
UBYTE  rece_bits;

if(bgIsoType == 0){
  rc_crc_enable();
  rc_crc_sel_14443a();
  rc_oddpari_enable();
  }
else{
	rc_crc_enable();
  rc_crc_sel_14443b();
  rc_pari_disable();
	}   
rc_clr_crypt();
rc_set_bitframe(0);
rc_idle();
rc_flush_fifo();
rc_clr_time_out();
//
rc_write_data_n(inbuf,inbytes);
//
rc_clr_irq();
rc_write_byte(REG_RC500_INTERRUPT_EN,(BIT_RXI|BIT_TIMERI)|0x80);
iso_block_set_time_out((timeout+4)/5);
rc_write_byte(REG_RC500_COMMAND,RC500_CMD_TRANSCEIVE);
//
for(i=0;i<6000;i++){
	//if(rc_irq()) break;
  if(rc_read_byte(REG_RC500_PRIMARY_STATUS)&0x08) break;
	delay_us(50);	
  }
rc_clr_time_out();
//
if(!(rc_read_byte(REG_RC500_INTERRUPT_RQ)&BIT_RXI)) return (UBYTE)-1;
if(rc_read_byte(REG_RC500_ERROR_FLAG)&0x0f) return (UBYTE)-2;

rece_bits = rc_rece_bits();
if(rece_bits%8) return (UBYTE)-3;

*outbytes = rece_bits/8;
rc_read_data_n(outbuf,*outbytes);
 
return 0;
}	


/*=============================================================================================
函数：iso_block_set_time_out
功能：设置超时
入口参数:
cnt_5ms: 以5ms为单位的数值
===============================================================================================*/
void iso_block_set_time_out(UBYTE cnt_5ms)
{
//
rc_write_byte(REG_RC500_TIMER_CLOCK,0x10);	  //(65536)/13.56M = 4833us
rc_write_byte(REG_RC500_TIMER_RELOAD,cnt_5ms);
//
return;
}


/*==================================================================================================
函数:
功能:
====================================================================================================*/
void pcb_reverse(void)
{
if(bgPCB) bgPCB=0;
else bgPCB=1;
return;			
}	

/*==================================================================================================
函数:mifpro_ats
功能:
====================================================================================================*/
//#define _DEBUG_MIFPRO_ATS_
UBYTE mifpro_ats(UBYTE cid,UBYTE *obuf,UBYTE *obytes)
{
UBYTE inbuf[16],inbytes;
UBYTE ret;
UBYTE fwi,i;

wgFWT=5;
bgSFGI=0;
bgCIDFlag=1;
bgCID = 0;
bgPCB = 1;
//
cid = cid&0x0f;
inbuf[0] = RATS_BLOCK;
inbuf[1] = 0x00|cid;
inbytes = 2;
ret = iso_block_transceve(inbuf,inbytes,obuf,obytes,ATS_TIME_OUT);

#ifdef _DEBUG_MIFPRO_ATS_
if(ret==0){
	printf("\nats:%d bytes",*obytes);
	for(i=0;i<*obytes;i++){
		if(i%16 == 0) printf("\n");
		printf("%02X ",(UBYTE)obuf[i]);	
		}
	}
else{
  printf("\nErr:ats,ret=%d",ret);
  } 	
#endif

if(ret!=0) return ret;
if((obuf[1]&0x70) == 0) return 0;
i = 2;
if(obuf[1]&0x10) i++;	
bgSFGI = obuf[i]&0x0f;
fwi = (UBYTE)(obuf[i]&0xf0)/16;
if(obuf[1]&0x20) i++;
if(obuf[1]&0x40){	
  if(obuf[i]&0x02) bgCIDFlag=1;
  bgCID = cid;
  }
  
wgFWT = 1;i=0;
while(i<fwi){
	wgFWT *= 2;
	i++;
	}
wgFWT = (wgFWT*3)/10;	

#ifdef _DEBUG_MIFPRO_ATS_
printf("\nSFGI=%d,FWT=%d,CID_FLAG=%d",bgSFGI,wgFWT,bgCIDFlag);
#endif

return 0;	
}	






/*==================================================================================================
函数:mifpro_deselect
功能:
====================================================================================================*/
//#define _DEBUG_MIFPRO_DESELECT_
UBYTE mifpro_deselect(UBYTE *outbuf)
{
UBYTE ret;
UBYTE inbuf[4],inbytes;
UBYTE outbytes;
#ifdef _DEBUG_MIFPRO_DESELECT_
UBYTE i;
#endif

inbuf[0] = 0xc2;
inbytes = 1;
if(bgCIDFlag){
	 inbuf[0] |= CID_FLAG;
	 inbuf[1] = bgCID;
	 inbytes++;
	 }
 
ret = iso_block_transceve(inbuf,inbytes,outbuf,&outbytes,DESELECT_TIME_OUT);
if(ret){
   #ifdef _DEBUG_MIFPRO_DESELECT_
   printf("\nErr:mifpro_deselect,ret=%d",(UBYTE)ret);
   #endif
	 return ret;
   }

#ifdef _DEBUG_MIFPRO_DESELECT_
printf("\nOK:mifpro_deselect,outbytes=%d",(UBYTE)outbytes);
for(i=0;i<outbytes;i++){
	 if(i%16 == 0) printf("\n");
	 printf("%02X ",(UBYTE)outbuf[i]);
	 }
#endif
return outbytes;	
}	



/*==================================================================================================
函数:mifpro_deselect
功能:
====================================================================================================*/
//#define _DEBUG_MIFPRO_WTX_
UBYTE mifpro_wtx(UBYTE *outbuf)
{
UBYTE ret;
UBYTE inbuf[4],inbytes;
UBYTE outbytes;
#ifdef _DEBUG_MIFPRO_WTX_
UBYTE i;
#endif

inbuf[0] = 0xf2;
inbytes = 1;
if(bgCIDFlag){
	 inbuf[0] |= CID_FLAG;
	 inbuf[1] = bgCID;
	 inbytes++;
	 }
inbuf[inbytes++] = bgWTX;
// 
ret = iso_block_transceve(inbuf,inbytes,outbuf,&outbytes,wgFWT*bgWTX);
if(ret){
   #ifdef _DEBUG_MIFPRO_WTX_
   printf("\nErr:mifpro_wtx,ret=%d",(UBYTE)ret);
   #endif
	 return ret;
   }

#ifdef _DEBUG_MIFPRO_WTX_
printf("\nOK:mifpro_wtx,outbytes=%d",(UBYTE)outbytes);
for(i=0;i<outbytes;i++){
	 if(i%16 == 0) printf("\n");
	 printf("%02X ",(UBYTE)outbuf[i]);
	 }
#endif
return outbytes;	
}	



/*==============================================================================
函数：mifpro_noack
功能：
================================================================================*/
//#define _DEBUG_MIFPRO_NOACK_
UBYTE mifpro_noack(UBYTE *outbuf)
{
UBYTE outbytes,inbytes;
UBYTE ret;
UBYTE inbuf[3];
#ifdef _DEBUG_MIFPRO_NOACK_
UBYTE i;
#endif

inbuf[0] = (bgPCB&0x01)|0xb2;
inbytes = 1;
if(bgCIDFlag){
	 inbuf[0] |= CID_FLAG; 
   inbuf[1] = bgCID;
   inbytes++;
   }
ret = iso_block_transceve(inbuf,inbytes,outbuf,&outbytes,wgFWT);
if(ret){
   #ifdef _DEBUG_MIFPRO_NOACK_
   printf("\nErr:mifpro_noack,ret=%d",(UBYTE)ret);
   #endif
	 return ret;
   }

#ifdef _DEBUG_MIFPRO_NOACK_
printf("\nOK:mifpro_noack,outbytes=%d",(UBYTE)outbytes);
for(i=0;i<outbytes;i++){
	 if(i%16 == 0) printf("\n");
	 printf("%02X ",(UBYTE)outbuf[i]);
	 }
#endif
return outbytes;
}	 

/*===============================================================================
函数：
功能：
=================================================================================*/
//#define _DEBUG_MIFPRO_ACK_
UBYTE mifpro_ack(UBYTE *outbuf)
{
UBYTE outbytes,inbytes;
UBYTE ret;
UBYTE inbuf[3];
#ifdef _DEBUG_MIFPRO_ACK_
UBYTE i;
#endif

pcb_reverse();
inbuf[0] = (bgPCB&0x01)|0xa2;
inbytes = 1;
if(bgCIDFlag){
	 inbuf[0] |= CID_FLAG; 
   inbuf[1] = bgCID;
   inbytes++;
   }
ret = iso_block_transceve(inbuf,inbytes,outbuf,&outbytes,wgFWT);
if(ret){
   #ifdef _DEBUG_MIFPRO_ACK_
   printf("\nErr:mifpro_ack,ret=%d",(UBYTE)ret);
   #endif
	 return ret;
   }

#ifdef _DEBUG_MIFPRO_ACK_
printf("\nOK:mifpro_ack,outbytes=%d",(UBYTE)outbytes);
for(i=0;i<outbytes;i++){
	 if(i%16 == 0) printf("\n");
	 printf("%02X ",(UBYTE)outbuf[i]);
	 }
#endif
return outbytes;
}	 



/*===============================================================================
函数：mifpro_icmd_nochain
功能：
=================================================================================*/
//#define _DEBUG_MIFPRO_ICMD_NOCHAIN_
UBYTE mifpro_icmd_nochain(UBYTE len,UBYTE *inbuf,UBYTE *outbuf)
{
UBYTE outbytes,inbytes;
UBYTE buf[64];
UBYTE ret;
#ifdef _DEBUG_MIFPRO_ICMD_NOCHAIN_
UBYTE i;
#endif

#ifdef _DEBUG_MIFPRO_ICMD_NOCHAIN_
printf("\nmifpro_icmd_nochain,cmd len=%d",len);
for(i=0;i<len;i++){
   if(i%16 == 0) printf("\n");
   printf("%02X ",(UBYTE)inbuf[i]);	
   }  
#endif

if((UBYTE)len > (UBYTE)MAX_RF_BUF) return (UBYTE)-1;

pcb_reverse();
buf[0] = (bgPCB&0x01)|0x02;
inbytes = 1;
if(bgCIDFlag){
   buf[0] |= CID_FLAG;
   buf[1] = bgCID;
   inbytes++;
   }
memcpy(&buf[inbytes],inbuf,len);
inbytes += len;

ret = iso_block_transceve(buf,inbytes,outbuf,&outbytes,wgFWT);
if(ret){
   #ifdef _DEBUG_MIFPRO_ICMD_NOCHAIN_
   printf("\nErr:mifpro_icmd_nochain,ret=%d",(UBYTE)ret);
   #endif
	 return ret;
   }

#ifdef _DEBUG_MIFPRO_ICMD_NOCHAIN_
printf("\nOK:mifpro_icmd_nochain,outbytes=%d",(UBYTE)outbytes);
for(i=0;i<outbytes;i++){
	 if(i%16 == 0) printf("\n");
	 printf("%02X ",(UBYTE)outbuf[i]);
	 }
#endif
return outbytes;
}	

/*===================================================================================
函数：mifpro_icmd_chain
功能：
=====================================================================================*/
//#define _DEBUG_MIFPRO_ICMD_CHAIN_
UBYTE mifpro_icmd_chain(UBYTE len,UBYTE *inbuf,UBYTE *outbuf)
{
UBYTE outbytes,inbytes;
UBYTE buf[64];
UBYTE ret;
#ifdef _DEBUG_MIFPRO_ICMD_CHAIN_
UBYTE i;
#endif

#ifdef _DEBUG_MIFPRO_ICMD_CHAIN_
printf("\nmifpro_icmd_chain,cmd len=%d",len);
for(i=0;i<len;i++){
   if(i%16 == 0) printf("\n");
   printf("%02X ",(UBYTE)inbuf[i]);	
   }  
#endif

if((UBYTE)len > (UBYTE)MAX_RF_BUF) return (UBYTE)-1;

pcb_reverse();
buf[0] = (bgPCB&0x01)|0x12;
inbytes = 1;
if(bgCIDFlag){
   buf[0] |= CID_FLAG;
   buf[1] = bgCID;
   inbytes++;
   }
memcpy(&buf[inbytes],inbuf,len);
inbytes += len;

ret = iso_block_transceve(buf,inbytes,outbuf,&outbytes,wgFWT);
if(ret){
   #ifdef _DEBUG_MIFPRO_ICMD_CHAIN_
   printf("\nErr:mifpro_icmd_chain,ret=%d",(UBYTE)ret);
   #endif
	 return ret;
   }

#ifdef _DEBUG_MIFPRO_ICMD_CHAIN_
printf("\nOK:mifpro_icmd_chain,outbytes=%d",(UBYTE)outbytes);
for(i=0;i<outbytes;i++){
	 if(i%16 == 0) printf("\n");
	 printf("%02X ",(UBYTE)outbuf[i]);
	 }
#endif
return outbytes;
}	


/*==================================================================================
函数：mifpro_icmd
功能：
====================================================================================*/
//#define _DEBUG_MIFPRO_ICMD_
UBYTE mifpro_icmd(UBYTE *ibuf,UBYTE ibytes,UBYTE *obuf,UBYTE *obytes)
{
		UBYTE slen;
		char  ret;
		UBYTE rptr,sptr;
		UBYTE i;
		UBYTE outbuf[257];
		UBYTE offset;
		UBYTE last_pcb;

		sptr = 0;rptr = 0;
		if(bgCIDFlag) offset=2;
		else offset=1;	
	
label_send_loop:
		if(ibytes > MAX_RF_BUF){
		    slen = MAX_RF_BUF;
		    ret = mifpro_icmd_chain(slen,&ibuf[sptr],&outbuf[rptr]); 
		    last_pcb = bgPCB;
    }
		else{
			  slen = ibytes;
			  ret = mifpro_icmd_nochain(slen,&ibuf[sptr],&outbuf[rptr]); 
	  }   


label_resp_process:   
		if((UBYTE)ret > 128)  goto label_noack;	
		if(ret < offset) goto label_abnormal;

//rece no chain
		if((outbuf[rptr]&I_BLOCK_MASK) == I_BLOCK_NO_CHAIN){
			#ifdef _DEBUG_MIFPRO_ICMD_
			 printf("\nRece no chain");
			#endif 
		   for(i=0;i<(ret-offset);i++) outbuf[rptr+i] = outbuf[rptr+i+offset];
		   rptr += (ret-offset);
		   if((UBYTE)rptr > 200) return (UBYTE)-3; 
		   memcpy(obuf,outbuf,rptr);
		   *obytes = rptr;	
		   return 0;
   }

//rece chain
if((outbuf[rptr]&I_BLOCK_MASK) == I_BLOCK_CHAIN){
	 #ifdef _DEBUG_MIFPRO_ICMD_
	 printf("\nRece chain");
	 #endif 
   for(i=0;i<(ret-offset);i++) outbuf[rptr+i] = outbuf[rptr+i+offset];
   rptr += (ret-offset);
   if((UBYTE)rptr > 200) return (UBYTE)-4; 
   //ack
   ret = mifpro_ack(&outbuf[rptr]);
   goto label_resp_process;
   }

//wtx 
if((UBYTE)(outbuf[rptr]&WTX_BLOCK_MASK) == (UBYTE)WTX_BLOCK){
   bgWTX = outbuf[rptr+offset]&0x3f;
   ret = mifpro_wtx(&outbuf[rptr]);
   goto label_resp_process;
   }  

   	
//ack,continue	
if((outbuf[rptr]&R_BLOCK_MASK) == R_BLOCK_ACK){
   if((last_pcb&0x01) != (outbuf[rptr]&0x01)){  
       #ifdef _DEBUG_MIFPRO_ICMD_
       printf("\nrece last ack,go to resend");
       #endif
       goto label_send_loop;
       }
   #ifdef _DEBUG_MIFPRO_ICMD_
   printf("\nrece ack,con to send");
   #endif
	 sptr += slen;
	 if(ibytes <= slen){
	    #ifdef _DEBUG_MIFPRO_ICMD_
	 	  printf("\nErr:abnormal,rece ack but no data to send, len=%d,slen=%d",ibytes,slen);
	    #endif	
	 	  return (UBYTE)-5;
	 	  }	 	
	 ibytes -= slen;
	 goto label_send_loop;	
	 }

//其他块
#ifdef _DEBUG_MIFPRO_ICMD_
printf("\nErr:unknown pcb %02X.",(UBYTE)outbuf[rptr]);
#endif
return (UBYTE)-6;   

label_abnormal: 
#ifdef _DEBUG_MIFPRO_ICMD_ 
printf("\nErr:abnormal,ret=%d",(UBYTE)ret);
#endif 
return (UBYTE)-7; 

label_noack:
ret = mifpro_noack(&outbuf[rptr]);
if((UBYTE)ret > 128){
  ret = mifpro_noack(&outbuf[rptr]);
	if((UBYTE)ret > 128){
        #ifdef _DEBUG_MIFPRO_ICMD_ 		
        printf("\nErr:noack 2times,timeout");
        #endif
		return (UBYTE)-8;
		}
	}
goto label_resp_process;	
}


//end of file
#endif

