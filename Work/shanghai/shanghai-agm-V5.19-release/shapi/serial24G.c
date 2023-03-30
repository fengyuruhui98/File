//serial24G.c
#ifndef _SERIAL24G_C_
#define _SERIAL24G_C_
//start of file

UBYTE UID24G[8],AUTHDATA[8];

/****************************************************
������ZTE_ICMD
���ܣ������˵�Э�������ģ��ͨѶ
****************************************************/
//#define _DEBUG_ZTE_CMD_
int ZTE_ICMD(UBYTE slen,UBYTE *sbuf,UBYTE *rlen,UBYTE *outbuf,int otime)
{
	UBYTE buf[300];
	UBYTE *rece_buf;
	int ret;
  #ifdef _DEBUG_ZTE_CMD_
  UBYTE i;
  #endif
	
	zte_set_rece_ptr(rece_buf);
	buf[0] = 0xa2;
	buf[1] = 0x33;
	memcpy(&buf[2],sbuf,slen);
  prot_zte_send_block(buf,slen+2); 
  
  //20130902
  prot_zte_rece_process(otime);
  if(gZteProt0.send_state == ZTE_SEND_TIMEOUT)
	{
		//gDebugStep = 0x0a01;
	   goto label_exit;
	}
	//
  *rlen = 0;

//  while(1){
//  	if(is_zte_wait_process()) break;
//    if(is_zte_send_timeout()){
//    	ret = 3;
//    	goto label_exit;
//    	}
//    }

  if(zte_rece_bytes < 2){
  	 ret = 1;
  	 goto label_exit;
  	 }
  if(memcmp(rece_buf,"\x0\x0",2) != 0){
  	 memcpy(outbuf,rece_buf,2);
  	 ret = 2;
  	 goto label_exit;
  	 }
  memcpy(outbuf,&rece_buf[2],zte_rece_bytes-2);
  *rlen = zte_rece_bytes-2;
  return 0;
  
label_exit:
#ifdef _DEBUG_ZTE_CMD_
debug_printf("\x0d\x0a ZTE ret=%d,rece_bytes=%d",ret,zte_rece_bytes);
debug_printf("\x0d\x0a cmd:");
for(i=0;i<slen;i++){
	if(i%16 == 0) debug_printf("\x0d\x0a");
	debug_printf("%02X ",(UBYTE)sbuf[i]);	
	}
debug_printf("\x0d\x0a resp:");
for(i=0;i<*rlen;i++){
	if(i%16 == 0) debug_printf("\x0d\x0a");
	debug_printf("%02X ",(UBYTE)outbuf[i]);	
	}	
#endif
 return ret; 	 
}

/****************************************************
������ZTE_REQ
���ܣ������˵�Э��ѯ��
****************************************************/
int ZTE_REQ(void)
{
	UBYTE buf[5];
	UBYTE *rece_buf;
	
	zte_set_rece_ptr(rece_buf);
	
  memcpy(buf,"\xa2\x31\x00\x01",4);
  prot_zte_send_block(buf,4); 
  //20130903
   prot_zte_rece_process(50);
  if(gZteProt0.send_state == ZTE_SEND_TIMEOUT)
	{
		return 1;
	}

//  while(1){
//  	if(is_zte_wait_process()) break;
//    if(is_zte_send_timeout()) return 1;
//    }

  if(memcmp(rece_buf,"\x0\x0",2) != 0){  	 
  	 return 2;
  	 }
  return 0;
}

/****************************************************
������ZTE_REQ_para
���ܣ������˵�Э�������ѯ��
****************************************************/
int ZTE_REQ_para(void)
{
	UBYTE buf[40];
	UBYTE *rece_buf;
	
	zte_set_rece_ptr(rece_buf);
	
	buf[0] = 0xa2;
	buf[1] = 0x41;
	memcpy(&buf[2],UID24G,8);
	memcpy(&buf[10],AUTHDATA,8);
	buf[18] = 0x00;
	buf[19] = 0x30;
	
	prot_zte_send_block(buf,20); 

  while(1){
  	if(is_zte_wait_process()) break;
    if(is_zte_send_timeout()) return 1;
    }

  if(memcmp(rece_buf,"\x0\x0",2) != 0) return 2;
  return 0;
}

/****************************************************
������ZTE_UPDATE_RESP
���ܣ������˵�Э�������ѯ��
****************************************************/
int ZTE_UPDATE_RESP(void)
{
	UBYTE buf[40];
	UBYTE *rece_buf;
	
	zte_set_rece_ptr(rece_buf);
	
	gDebugStep = 0x0301;
	buf[0] = 0xa2;
	buf[1] = 0x42;
	memcpy(&buf[2],UID24G,8);
	memcpy(&buf[10],AUTHDATA,8);

	prot_zte_send_block(buf,18); 

  while(1){
  	if(is_zte_wait_process()) break;
    if(is_zte_send_timeout()) return 1;
    }

  if(memcmp(rece_buf,"\x0\x0",2) != 0) return 2;
  return 0;
}
/****************************************************
������ZTE_HALT
���ܣ������˵�Э��ѯ��
****************************************************/
int ZTE_HALT(void)
{
	UBYTE buf[5];
	UBYTE *rece_buf;
	
	zte_set_rece_ptr(rece_buf);
  memcpy(buf,"\xa2\x32\x0\x0",4);
  prot_zte_send_block(buf,4); 

  while(1){
  	if(is_zte_wait_process()) break;
    if(is_zte_send_timeout()) return 1;
    }

  if(memcmp(rece_buf,"\x0\x0",2) != 0) return 2;
  return 0;
	}
	
/****************************************************
������ZTE_GET_STATUS
���ܣ������˵�Э���ѯ���뿪״̬
****************************************************/
int ZTE_GET_STATUS(UBYTE *resp)
{
	UBYTE buf[20];
	UBYTE *rece_buf;
	
	zte_set_rece_ptr(rece_buf);

	buf[0] = 0xe0;
	buf[1] = 0x02;

  prot_zte_send_block(buf,4); 
  
  //20130903
  prot_zte_rece_process(50);
  if(gZteProt0.send_state == ZTE_SEND_TIMEOUT)
	{
		//gDebugStep = 0x0a01;
	  // goto label_exit;
	  return 1;
	}

//  while(1){
//  	if(is_zte_wait_process()) break;
//    if(is_zte_send_timeout()) return 1;
//    }

  if(memcmp(rece_buf,"\x0\x0",2) != 0) return 2;
  *resp = rece_buf[3];	
  return 0; 
}


//end of file
#endif