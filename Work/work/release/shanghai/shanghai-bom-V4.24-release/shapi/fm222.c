//fm222.c

#ifndef _FM222_C_
#define _FM222_C_
//start of file

UWORD fm_time_out = 50;


 
/*======================================================================================
函数:fm222_is_ok
功能:检查FM222模块是否正常
0:异常
1:正常
========================================================================================*/

UBYTE fm222_is_ok(void)
{
int ret;	
//
ret = fm_handshake();
if(ret) return 0;
//
return 1;		
}	

/*======================================================================================
函数:fm222_transceve
功能:ming wen
========================================================================================*/
UBYTE fm222_transceve(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes,UWORD time_out)
{
char sbuf[1000],rbuf[1000];
int sbytes,rbytes,rbytes2;
int i,ret;
UBYTE lrc;
	//
	if(inbytes <= 0) return (uchar)-3;	
	
	sbuf[0] = 0x55;
	sbuf[1] = 0x11;//明文
	sbuf[2] = inbuf[0];
	sbuf[3] = (inbytes-1)%256;
	sbuf[4] = (inbytes-1)/256;
	memcpy(&sbuf[5],&inbuf[1],inbytes-1);
	for(i=0,lrc=0;i<(inbytes+4);i++) lrc ^= sbuf[i];
	sbuf[4+inbytes] = lrc;
	sbytes = 5+inbytes;
	//
	fm_send_block(sbuf,sbytes);
	ret = fm_rece_block(rbuf,&rbytes);
	if(ret)  return (UBYTE)-2;
	//
	if(rbytes == 1) return (uchar)-1;
	if(rbuf[0] != 0x55) return (uchar)-2;
	//
	outbuf[0] = rbuf[2];
	rbytes2 = (UBYTE)rbuf[4];
	rbytes2 = rbytes2*256+(UBYTE)rbuf[3];
	
//	if((rbytes2+6) != rbytes){
//		 return (uchar)-4;
//	 }
	memcpy(&outbuf[1],&rbuf[5],rbytes2);
	//
//	for(i=0,lrc=0;i<(rbytes);i++) lrc ^= rbuf[i];
//	if(lrc != 0) return (uchar)-5;
	*outbytes = rbytes2+1;
	return 0;			
}


/*======================================================================================
函数:fm222_transceve
功能:ming wen
========================================================================================*/
UBYTE fm222_transceve_mingwen_return_mac(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes,UWORD time_out)
{
char sbuf[1000],rbuf[1000];
int sbytes,rbytes,rbytes2;
int i,ret;
UBYTE lrc;
	//
	if(inbytes <= 0) return (uchar)-3;	

	sbuf[0] = 0x55;
	sbuf[1] = 0x12;//明文,return 明文+MAC
	sbuf[2] = inbuf[0];
	sbuf[3] = (inbytes-1)%256;
	sbuf[4] = (inbytes-1)/256;
	memcpy(&sbuf[5],&inbuf[1],inbytes-1);
	for(i=0,lrc=0;i<(inbytes+4);i++) lrc ^= sbuf[i];
	sbuf[4+inbytes] = lrc;
	sbytes = 5+inbytes;
	//
	fm_send_block(sbuf,sbytes);
	ret = fm_rece_block(rbuf,&rbytes);
	if(ret)  return (UBYTE)-2;
	//
	if(rbytes == 1) return (uchar)-1;
	//
	outbuf[0] = rbuf[2];
	rbytes2 = (UBYTE)rbuf[4];
	rbytes2 = rbytes2*256+(UBYTE)rbuf[3];
	
//	if((rbytes2+6) != rbytes){
//		return (uchar)-4;
//	}
	memcpy(&outbuf[1],&rbuf[5],rbytes2);
	//
	//for(i=0,lrc=0;i<(rbytes+5);i++) lrc ^= rbuf[i];
	//if(lrc != 0) return (uchar)-5;
	*outbytes = rbytes2+1;
	return 0;			
}


/*======================================================================================
函数:fm222_transceve
功能:
========================================================================================*/
UBYTE fm222_transceve_mingwenmac(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes,UWORD time_out)
{
char sbuf[1000],rbuf[1000];
int sbytes,rbytes,rbytes2;
int i,ret;
UBYTE lrc;
//
if(inbytes <= 0) return (uchar)-3;	

sbuf[0] = 0x55;
sbuf[1] = 0x22;//明文+MAC,return 明文+MAC
sbuf[2] = inbuf[0];
sbuf[3] = (inbytes-1)%256;
sbuf[4] = (inbytes-1)/256;
memcpy(&sbuf[5],&inbuf[1],inbytes-1);
for(i=0,lrc=0;i<(inbytes+4);i++) lrc ^= sbuf[i];
sbuf[4+inbytes] = lrc;
sbytes = 5+inbytes;
//
fm_send_block(sbuf,sbytes);
ret = fm_rece_block(rbuf,&rbytes);
if(ret)  return (UBYTE)-2;
//
if(rbytes == 1) return (uchar)-1;
//
outbuf[0] = rbuf[2];
rbytes2 = (UBYTE)rbuf[4];
rbytes2 = rbytes2*256+(UBYTE)rbuf[3];

if((rbytes2+6) != rbytes){
	 return (uchar)-4;
	 }
memcpy(&outbuf[1],&rbuf[5],rbytes2);
//
for(i=0,lrc=0;i<(rbytes);i++) lrc ^= rbuf[i];
if(lrc != 0) return (uchar)-5;
*outbytes = rbytes2+1;
return 0;
}

/*======================================================================================
函数:fm222_transceve
功能:
========================================================================================*/
UBYTE fm222_transceve_miwen(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes,UWORD time_out)
{
char sbuf[1000],rbuf[1000];
int sbytes,rbytes,rbytes2;
int i,ret;
UBYTE lrc;
	//
	if(inbytes <= 0) return (uchar)-3;	
	
	sbuf[0] = 0x55;
	sbuf[1] = 0x41;//密文
	sbuf[2] = inbuf[0];
	sbuf[3] = (inbytes-1)%256;
	sbuf[4] = (inbytes-1)/256;
	memcpy(&sbuf[5],&inbuf[1],inbytes-1);
	for(i=0,lrc=0;i<(inbytes+4);i++) lrc ^= sbuf[i];
	sbuf[4+inbytes] = lrc;
	sbytes = 5+inbytes;
	//
	fm_send_block(sbuf,sbytes);
	ret = fm_rece_block(rbuf,&rbytes);
	if(ret)  return (UBYTE)-2;
	//
	if(rbytes == 1) return (uchar)-1;
	//if(rbuf[0] != 0x55) return (uchar)-2;
	//
	outbuf[0] = rbuf[2];
	rbytes2 = (UBYTE)rbuf[4];
	rbytes2 = rbytes2*256+(UBYTE)rbuf[3];
	
	//if((rbytes2+6) != rbytes){
	//	return (uchar)-4;
	//}
	memcpy(&outbuf[1],&rbuf[5],rbytes2);
	//
	//for(i=0,lrc=0;i<(rbytes);i++) lrc ^= rbuf[i];
	//if(lrc != 0) return (uchar)-5;
	*outbytes = rbytes2+1;
	return 0;
}

/*======================================================================================
函数:fm222_transceve
功能:
========================================================================================*/
UBYTE fm222_transceve_miwenmac(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes,UWORD time_out)
{
char sbuf[1000],rbuf[1000];
int sbytes,rbytes,rbytes2;
int i,ret;
UBYTE lrc;
//
if(inbytes <= 0) return (uchar)-3;	

sbuf[0] = 0x55;
sbuf[1] = (UBYTE)0x81;//密文+MAC
sbuf[2] = inbuf[0];
sbuf[3] = (inbytes-1)%256;
sbuf[4] = (inbytes-1)/256;
memcpy(&sbuf[5],&inbuf[1],inbytes-1);
for(i=0,lrc=0;i<(inbytes+4);i++) lrc ^= sbuf[i];
sbuf[4+inbytes] = lrc;
sbytes = 5+inbytes;
//
fm_send_block(sbuf,sbytes);
ret = fm_rece_block(rbuf,&rbytes);
if(ret)  return (UBYTE)-2;
//
if(rbytes == 1) return (uchar)-1;
if(rbuf[0] != 0x55) return (uchar)-2;
//
outbuf[0] = rbuf[2];
rbytes2 = (UBYTE)rbuf[4];
rbytes2 = rbytes2*256+(UBYTE)rbuf[3];

if((rbytes2+6) != rbytes){
	 return (uchar)-4;
	 }
memcpy(&outbuf[1],&rbuf[5],rbytes2);
//
for(i=0,lrc=0;i<(rbytes);i++) lrc ^= rbuf[i];
if(lrc != 0) return (uchar)-5;
*outbytes = rbytes2+1;
return 0;
}


/*======================================================================================
函数:
功能:设置卡片操作类型
========================================================================================*/
//UBYTE set_card_type(UBYTE card_type)
//{
//UBYTE inbuf[256],outbuf[256];
//UBYTE inbytes,outbytes;
//UBYTE ret;
//
//inbuf[0] = FM222_CMD_CARDTYPE;
//switch(card_type){
//  case ISO14443A_SH_TYPE:
//  	   card_type = 2;
//  	   break;
//  case ISO14443B_TYPE:
//       card_type = 1;
//       break;   	   
//  default:
//  	   card_type = 0;
//  	   break;
//	}
//inbuf[1] = card_type;
//inbytes = 2;
//ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);//all 500,20090703
//if(ret) return ret;
//
//if(outbytes != 1) return 2;
//if(outbuf[0] != 0) return 3;
//
//return 0;		
//}	


/*======================================================================================
函数:
功能:
返回:
0:有卡
其他:无卡
========================================================================================*/
//UBYTE mcml_request(UBYTE *atq)
//{
//UBYTE inbuf[256],outbuf[256];
//UBYTE inbytes,outbytes;
//UBYTE ret;
//
//inbuf[0] = FM222_CMD_REQUEST;
//inbuf[1] = 0x00;
//inbuf[2] = 0x03;//0x03,20090831
//inbytes = 3;
//ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
//if(ret) return ret;
//
//if(outbytes != 3) return 2;
//if(outbuf[0] != 0) return 3;
//memcpy(atq,&outbuf[1],2);	
//
//return 0;	
//}	

///*======================================================================================
//函数:
//功能:
//========================================================================================*/
//UBYTE mcml_anticoll(UBYTE *snr)
//{
//UBYTE inbuf[256],outbuf[256];
//UBYTE inbytes,outbytes;
//UBYTE ret;
//
//inbuf[0] = FM222_CMD_ANTICOLL;
//memcpy(&inbuf[1],"\x0\x0\x0\x0\x0",5);
//inbytes = 6;
//ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
//if(ret) return ret;
//
//if(outbytes != 5) return 2;
//if(outbuf[0] != 0) return 3;
//memcpy(snr,&outbuf[1],4);
//snr[4] = snr[0]^snr[1]^snr[2]^snr[3];	
//
//return 0;		
//}	
//
///*======================================================================================
//函数:
//功能:
//========================================================================================*/
//UBYTE mcml_select(UBYTE *snr,UBYTE *status)
//{
//UBYTE inbuf[256],outbuf[256];
//UBYTE inbytes,outbytes;
//UBYTE ret;
//
//inbuf[0] = FM222_CMD_SELECT;
//memcpy(&inbuf[1],snr,4);
//inbytes = 5;
//ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
//if(ret) return ret;
//
//if(outbytes != 2) return 2;
//if(outbuf[0] != 0) return 3;
//memcpy(status,&outbuf[1],1);	
//
//return 0;			
//}	
//
///*======================================================================================
//函数:
//功能:
//========================================================================================*/
//UBYTE mcml_load_key(UBYTE key_set,UBYTE keyab,UBYTE sectno,UBYTE *buf)
//{
//UBYTE inbuf[256],outbuf[256];
//UBYTE inbytes,outbytes;
//UBYTE ret;
//
//inbuf[0] = FM222_CMD_LOADKEY;
//if(keyab == 0) inbuf[1] = key_set;
//else inbuf[1] = key_set|BIT01_MASK;
//inbuf[2] = sectno;
//memcpy(&inbuf[3],buf,6);	
//inbytes = 9;
//ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
//if(ret) return ret;
//
//if(outbytes != 1) return 2;
//if(outbuf[0] != 0) return 3;
//
//return 0;			
//}	
//
///*======================================================================================
//函数:
//功能:
//========================================================================================*/
//UBYTE mcml_authentication(UBYTE key_set,UBYTE keyab,UBYTE sectno)
//{
//UBYTE inbuf[256],outbuf[256];
//UBYTE inbytes,outbytes;
//UBYTE ret;
//
//inbuf[0] = FM222_CMD_AUTHTICATION;
//if(keyab == 0) inbuf[1] = key_set;
//else inbuf[1] = key_set|BIT01_MASK;
//inbuf[2] = sectno;
//inbytes = 3;
//ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
//if(ret) return ret;
//
//if(outbytes != 1) return 2;
//if(outbuf[0] != 0) return 3;
//
//return 0;			
//}	
//
///*======================================================================================
//函数:
//功能:
//========================================================================================*/
//UBYTE mcml_read(UBYTE block,UBYTE *obuf)
//{
//UBYTE inbuf[256],outbuf[256];
//UBYTE inbytes,outbytes;
//UBYTE ret;
//
//inbuf[0] = FM222_CMD_READ;
//inbuf[1] = block;
//inbytes = 2;
//ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
//if(ret) return ret;
//
//if(outbytes != 17) return 2;
//if(outbuf[0] != 0) return 3;
//memcpy(obuf,&outbuf[1],16);
//
//return 0;				
//}	

/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE mcml_read_return_mac(UBYTE block,UBYTE *obuf,UBYTE *mac)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

	inbuf[0] = FM222_CMD_READ;
	inbuf[1] = block;
	inbytes = 2;
	ret = fm222_transceve_mingwen_return_mac(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
	if(ret) return ret;

	if(outbytes != 21) return 2;
	if(outbuf[0] != 0) return 3;
	memcpy(obuf,&outbuf[1],16);
	memcpy(mac,&outbuf[17],4);
	return 0;				
}


/*======================================================================================
函数:
功能:
========================================================================================*/
//UBYTE mcml_write(UBYTE block,UBYTE *ibuf)
//{
//UBYTE inbuf[256],outbuf[256];
//UBYTE inbytes,outbytes;
//UBYTE ret;
//
//inbuf[0] = FM222_CMD_WRITE;
//inbuf[1] = block;
//memcpy(&inbuf[2],ibuf,16);
//inbytes = 18;
//ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
//if(ret) return ret;
//
//if(outbytes != 1) return 2;
//if(outbuf[0] != 0) return 3;
//
//return 0;	
//}	



/*======================================================================================
函数:线路保护方式更新数据块
功能:(UBYTE auth_mode,UBYTE block,UBYTE *inbuf)
========================================================================================*/
UBYTE mcml_write_mac_mode(UBYTE block,UBYTE *ibuf,UBYTE *obuf)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_WRITE; //0x47
inbuf[1] = block;
memcpy(&inbuf[2],ibuf,20);
inbytes = 22;
ret = fm222_transceve_mingwenmac(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

memset(obuf,0,10);
if(outbytes != 6) return 2;//status + MAC
if(outbuf[0] != 0) return 3;
memcpy(obuf,&outbuf[1],5);
return 0;	
}


///*======================================================================================
//函数:
//功能:
//========================================================================================*/
//UBYTE mcml_increment(UBYTE addr, UDWORD value)
//{
//UBYTE inbuf[256],outbuf[256];
//UBYTE inbytes,outbytes;
//UBYTE ret;
//
//inbuf[0] = FM222_CMD_INCREMENT;
//inbuf[1] = addr;
//udword_to_buf_reverse(value,&inbuf[2]);
//inbytes = 6;
//ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
//if(ret) return ret;
//
//if(outbytes != 1) return 2;
//if(outbuf[0] != 0) return 3;
//
//return 0;	
//}	
//
///*======================================================================================
//函数:
//功能:
//========================================================================================*/
//UBYTE mcml_decrement(UBYTE addr,UDWORD value)
//{
//UBYTE inbuf[256],outbuf[256];
//UBYTE inbytes,outbytes;
//UBYTE ret;
//
//inbuf[0] = FM222_CMD_DECREMENT;
//inbuf[1] = addr;
//udword_to_buf_reverse(value,&inbuf[2]);
//inbytes = 6;
//ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
//if(ret) return ret;
//
//if(outbytes != 1) return 2;
//if(outbuf[0] != 0) return 3;
//
//return 0;	
//}	
//
///*======================================================================================
//函数:
//功能:
//========================================================================================*/
//UBYTE mcml_transfer(UBYTE addr)
//{
//UBYTE inbuf[256],outbuf[256];
//UBYTE inbytes,outbytes;
//UBYTE ret;
//
//inbuf[0] = FM222_CMD_TRANSFER;
//inbuf[1] = addr;
//inbytes = 2;
//ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
//if(ret) return ret;
//
//if(outbytes != 1) return 2;
//if(outbuf[0] != 0) return 3;
//
//return 0;		
//}	
//
///*======================================================================================
//函数:
//功能:
//========================================================================================*/
//UBYTE mcml_restore(UBYTE addr)
//{
//UBYTE inbuf[256],outbuf[256];
//UBYTE inbytes,outbytes;
//UBYTE ret;
//
//inbuf[0] = FM222_CMD_RESTORE;
//inbuf[1] = addr;
//inbytes = 2;
//ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
//if(ret) return ret;
//
//if(outbytes != 1) return 2;
//if(outbuf[0] != 0) return 3;
//
//return 0;
//}	

/*======================================================================================
函数:
功能:
========================================================================================*/
//UBYTE mcml_pwr_off(void)
//{
//UBYTE inbuf[256],outbuf[256];
//UBYTE inbytes,outbytes;
//UBYTE ret;
//
//inbuf[0] = FM222_CMD_POWER;
//inbuf[1] = 0;
//inbytes = 2;
//ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
//if(ret) return ret;
//
//if(outbytes != 1) return 2;
//if(outbuf[0] != 0) return 3;
//
//return 0;
//}	


//UBYTE mcml_pwr_on(void)
//{
//UBYTE inbuf[256],outbuf[256];
//UBYTE inbytes,outbytes;
//UBYTE ret;
//
//inbuf[0] = FM222_CMD_POWER;
//inbuf[1] = 1;
//inbytes = 2;
//ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
//if(ret) return ret;
//
//if(outbytes != 1) return 2;
//if(outbuf[0] != 0) return 3;
//
//return 0;
//}	

/*======================================================================================
函数:
功能:
========================================================================================*/
//UBYTE mcml_halt(void)
//{
//UBYTE inbuf[256],outbuf[256];
//UBYTE inbytes,outbytes;
//UBYTE ret;
//
//inbuf[0] = FM222_CMD_HALT;
//inbytes = 1;
//ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
//if(ret) return ret;
//
//if(outbytes != 1) return 2;
//if(outbuf[0] != 0) return 3;
//
//return 0;
//}	



/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_active(UBYTE req_mode,UBYTE req_cnt,UBYTE *snr,UBYTE *app_info)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

	inbuf[0] = FM222_CMD_HL_ACTIVE;
	inbuf[1] = req_mode; 
	inbuf[2] = req_cnt;
	inbytes = 3;
	ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
	if(ret) return ret;
	
	if(outbytes != 8) return 2;
	if(outbuf[0] != 0) return 3;
	memcpy(snr,&outbuf[3],4);
	memcpy(app_info,&outbuf[1],2);
	app_info[2] = outbuf[7];
	
	return 0;
}

/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_auth(UBYTE key_set,UBYTE sectno,UBYTE *inkey)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

	inbuf[0] = FM222_CMD_HL_AUTH;
	inbuf[1] = 0; inbuf[2] = 0;
	inbuf[3] = key_set;
	inbuf[4] = sectno;
	memcpy(&inbuf[5],inkey,6);
	inbytes = 11;
	ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
	if(ret) return ret;
	
	if(outbytes != 1) return 2;
	if(outbuf[0] != 0) return 3;

return 0;
}

/*======================================================================================
函数:密文
功能:load key
========================================================================================*/
UBYTE mcml_load_key_miwen_mode(UBYTE *inkey)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_HL_AUTH; //0x37
memcpy(&inbuf[1],inkey,16);
inbytes = 17;
ret = fm222_transceve_miwen(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 1) return 2;
if(outbuf[0] != 0) return 3;

return 0;
}


/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_read(UBYTE auth_mode,UBYTE block,UBYTE *obuf)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_HL_READ;
inbuf[1] = auth_mode;
inbuf[2] = block;
inbytes = 3;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 17) return 2;
if(outbuf[0] != 0) return 3;
memcpy(obuf,&outbuf[1],16);

return 0;
}
/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_token_read_no_auth(UBYTE block,UBYTE *obuf,UBYTE *uid)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_HL_TOKEN_READ_NO_AUTH;
inbuf[1] = block;
inbytes = 2;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 9) return 2;
if(outbuf[0] != 0) return 3;
memcpy(obuf,&outbuf[1],4);
memcpy(uid,&outbuf[5],4);

return 0;
}
/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_token_read(UBYTE auth_mode,UBYTE block,UBYTE *obuf,UBYTE *uid)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_HL_TOKEN_READ;
inbuf[1] = auth_mode;
inbuf[2] = block;
inbytes = 3;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 9) return 2;
if(outbuf[0] != 0) return 3;
memcpy(obuf,&outbuf[1],4);
memcpy(uid,&outbuf[5],4);

return 0;
}
/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_token_write(UBYTE auth_mode,UBYTE block,UBYTE *ibuf)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_HL_TOKEN_WRITE;
inbuf[1] = auth_mode;
inbuf[2] = block;
memcpy(&inbuf[3],ibuf,4);
inbytes = 7;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 1) return 2;
if(outbuf[0] != 0) return 3;

return 0;
}
/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_write(UBYTE auth_mode,UBYTE block,UBYTE *ibuf)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_HL_WRITE;
inbuf[1] = auth_mode;
inbuf[2] = block;
memcpy(&inbuf[3],ibuf,16);
inbytes = 19;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 1) return 2;
if(outbuf[0] != 0) return 3;

return 0;
}
/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE init_value(UBYTE block,UDWORD value)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_INIT_VALUE;
inbuf[1] = block;
udword_to_buf_reverse(value,&inbuf[2]);
inbytes = 6;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 1) return 2;
if(outbuf[0] != 0) return 3;

return 0;
}

/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_init_value(UBYTE auth_mode,UBYTE block,UDWORD value)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_HL_INIT_VALUE;
inbuf[1] = auth_mode;
inbuf[2] = block;
udword_to_buf_reverse(value,&inbuf[3]);
inbytes = 7;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 1) return 2;
if(outbuf[0] != 0) return 3;

return 0;
}
/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE read_value(UBYTE block,UDWORD *value)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_READ_VALUE;
inbuf[1] = block;
inbytes = 2;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 5) return 2;
if(outbuf[0] != 0) return 3;
buf_to_udword_reverse(&outbuf[1],*value);

return 0;
}

/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_read_value(UBYTE auth_mode,UBYTE block,UDWORD *value)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_HL_READ_VALUE;
inbuf[1] = auth_mode;
inbuf[2] = block;
inbytes = 3;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 5) return 2;
if(outbuf[0] != 0) return 3;
buf_to_udword_reverse(&outbuf[1],*value);

return 0;
}

/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_increment(UBYTE auth_mode,UBYTE addr, UDWORD value)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_HL_INCREMENT;
inbuf[1] = auth_mode;
inbuf[2] = addr;
udword_to_buf_reverse(value,&inbuf[3]);
inbytes = 7;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 1) return 2;
if(outbuf[0] != 0) return 3;

return 0;
}

/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_decrement(UBYTE auth_mode,UBYTE addr,UDWORD value)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_HL_DECREMENT;
inbuf[1] = auth_mode;
inbuf[2] = addr;
udword_to_buf_reverse(value,&inbuf[3]);
inbytes = 7;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 1) return 2;
if(outbuf[0] != 0) return 3;

return 0;
}

/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_get_random(UBYTE mode,UBYTE *obuf)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_GET_RANDOM;
inbuf[1] = mode;
inbytes = 2;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if((outbytes != 5)&&(outbytes != 9)&&(outbytes != 17)) return 2;
if(outbuf[0] != 0) return 3;

memcpy(obuf,&outbuf[1],outbytes-1);

return 0;
}

/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_get_key(UBYTE *ibuf)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_GET_KEY;
memcpy(&inbuf[1],ibuf,30);
inbytes = 31;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 1) return 2;
if(outbuf[0] != 0) return 3;

return 0;
}

/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_card_value_check(UDWORD value,UBYTE *mac)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_CARD_VALUE_CHECK;
udword_to_buf_reverse(value,&inbuf[1]);
memcpy(&inbuf[5],mac,8);
inbytes = 14;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 1) return 2;
if(outbuf[0] != 0) return 3;

return 0;
}
/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_data_restore(UBYTE block)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_DATA_RESTORE;
inbuf[1] = block;
inbytes = 2;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 1) return 2;
if(outbuf[0] != 0) return 3;

return 0;
}
/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_get_trans_result(UBYTE *trans_time,UBYTE *tac)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_GET_TRANS_RESULT;
memcpy(&inbuf[1],trans_time,6);
inbytes = 7;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 6) return 2;
if(outbuf[0] != 0) return 3;
memcpy(tac,&outbuf[1],5);	
return 0;
}

/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_cal_tac(UBYTE key_index,UBYTE *ibuf,UBYTE ibytes,UBYTE *tac)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_CALC_TAC;
inbuf[1] = key_index;
memcpy(&inbuf[2],ibuf,ibytes);
inbytes = 2+ibytes;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 5) return 2;
if(outbuf[0] != 0) return 3;
memcpy(tac,&outbuf[1],4);
return 0;
}

/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_cal_mac(UBYTE key_index,UBYTE *ibuf,UBYTE ibytes,UBYTE *tac)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_CALC_MAC;
inbuf[1] = key_index;
memcpy(&inbuf[2],ibuf,ibytes);
inbytes = 2+ibytes;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 5) return 2;
if(outbuf[0] != 0) return 3;
memcpy(tac,&outbuf[1],4);

return 0;
}
/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_check_mac(UBYTE key_index,UBYTE *ibuf,UBYTE ibytes)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_CHECK_MAC;
inbuf[1] = key_index;
memcpy(&inbuf[2],ibuf,ibytes);
inbytes = 2+ibytes;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 1) return 2;
if(outbuf[0] != 0) return 3;

return 0;
}
/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_data_encrypt(UBYTE key_index,UBYTE *ibuf,UBYTE ibytes,UBYTE *obuf)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_DATA_ENCRYPT;
inbuf[1] = key_index;
memcpy(&inbuf[2],ibuf,ibytes);
inbytes = 2+ibytes;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != (1+ibytes)) return 2;
if(outbuf[0] != 0) return 3;
memcpy(obuf,&outbuf[1],ibytes);

return 0;
}
/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_data_decrypt(UBYTE key_index,UBYTE *ibuf,UBYTE ibytes,UBYTE *obuf)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_DATA_DECRYPT;
inbuf[1] = key_index;
memcpy(&inbuf[2],ibuf,ibytes);
inbytes = 2+ibytes;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != (1+ibytes)) return 2;
if(outbuf[0] != 0) return 3;
memcpy(obuf,&outbuf[1],ibytes);

return 0;
}
/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_get_module_no(UBYTE *obuf)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_GET_MODULE_NO;
inbytes = 1;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if((outbytes != 5)&&(outbytes != 11)) return 2;
if(outbuf[0] != 0) return 3;
if(outbytes == 5){
   memset(obuf,0,6);
   memcpy(&obuf[6],&outbuf[1],4);
   }	
else memcpy(obuf,&outbuf[1],10);   

return 0;
}

/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_get_module_software_version(UBYTE *obuf)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_GET_MODULE_SWVER;
inbytes = 1;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 15) return 2;
if(outbuf[0] != 0) return 3;
memcpy(obuf,&outbuf[1],14);   

return 0;
}

/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE hl_update_module_no(UBYTE *ibuf,UBYTE *resp)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_UPDATE_MODULE_NO;
memcpy(&inbuf[1],ibuf,6);
inbytes = 7;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 2) return 2;
if(outbuf[0] != 0) return 3;
*resp = outbuf[1];

return 0;
}


/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE iso_rats(UBYTE cid,UBYTE *obuf)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_RATS;
inbuf[1] = cid;
inbytes = 2;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;
/*
if(outbytes != 17) return 2;
if(outbuf[0] != 0) return 3;
memcpy(obuf,&outbuf[1],16);
*/
if(outbytes < 10) return 2;//2011-04-25,上海公交卡返回16字节，但沪通卡只返回14字节
if(outbuf[0] != 0) return 3;
memset(obuf,0,16);
memcpy(obuf,&outbuf[1],outbytes-1);

return 0;
}
/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE iso_pps(UBYTE *ibuf,UBYTE *obuf)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_PPS;
memcpy(&inbuf[1],ibuf,2);
inbytes = 3;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 2) return 2;
if(outbuf[0] != 0) return 3;
*obuf = outbuf[1];	

return 0;
}
/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE iso_iblock(UBYTE *ibuf,UBYTE ibytes,UBYTE *obuf,UBYTE *obytes)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_IBLOCK;
memcpy(&inbuf[1],ibuf,ibytes);
inbytes = 1+ibytes;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbuf[0] != 0) return 3;
memcpy(obuf,&outbuf[1],outbytes-1);
*obytes = outbytes-1; 	

return 0;
}
/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE iso_apdu_access(UBYTE *ibuf,UBYTE ibytes,UBYTE *obuf,UBYTE *obytes)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

	inbuf[0] = FM222_CMD_APDU_ACCESS;
	memcpy(&inbuf[1],ibuf,ibytes);
	inbytes = 1+ibytes;
	ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
	if(ret) return ret;
	
	if(outbuf[0] != 0) return 3;
	memcpy(obuf,&outbuf[1],outbytes-1);
	*obytes = outbytes-1; 	

	return 0;
}	

/*======================================================================================
函数:
功能:
========================================================================================*/
UBYTE iso_deselect(UBYTE cid)
{
UBYTE inbuf[256],outbuf[256];
UBYTE inbytes,outbytes;
UBYTE ret;

inbuf[0] = FM222_CMD_DESELECT;
inbuf[1] = cid;
inbytes = 2;
ret = fm222_transceve(inbuf,inbytes,outbuf,&outbytes,fm_time_out);
if(ret) return ret;

if(outbytes != 1) return 2;
if(outbuf[0] != 0) return 3;

return 0;
}



//end of file
#endif


