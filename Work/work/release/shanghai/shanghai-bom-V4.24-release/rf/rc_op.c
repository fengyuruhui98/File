//rc_op.c

#ifndef _RC_OP_C_
#define _RC_OP_C_
//start of file

//#define _RC_OP_LOCK_
UBYTE bgIsoType;   //0:type a 1:type b
UBYTE bgRequestOkFlag;

UBYTE const bpgRcInitValue[64] = {            \
	  0x80,0x1E,0x39,0x6D,0x00,0x60,0x28,0x31,  \
	  0x81,0x00,0x40,0x00,0x00,0xFF,0x63,0x00,  \
	  0x82,0x58,0x3F,0x3F,0x19,0x13,0x00,0x3B,  \
	  0x83,0x73,0x08,0xAD,0xFF,0x1E,0x41,0x48,  \
    0x84,0x06,0x03,0x63,0x63,0x00,0x00,0x84,  \
    0x85,0x04,0x0B,0x02,0x03,0x03,0x85,0x85,  \
    0x86,0x86,0x86,0x86,0x86,0x86,0x86,0x86,  \
    0x87,0x87,0x87,0x87,0x87,0x87,0x87,0xAE}; 
      

/*==============================================================================================
函数：rc_reg_restore
功能：
================================================================================================*/
void rc_reg_restore(void)
{
UBYTE i;

for(i=16;i<48;i++){
	if(i%8 == 0) continue;
  rc_write_byte(i,(UBYTE)bpgRcInitValue[i]);		
	}	
	
return;
}


/*==============================================================================================
函数：rc_init
功能：
================================================================================================*/
void rc_init0(void)
{
//硬复位
#ifdef _RC_OP_LOCK_
os_lock();
#endif
rc_rst_set();
delay_ms(20);
rc_rst_clr();
delay_ms(20);
////
rc_write_byte(REG_RC500_PAGE,0x00);

rc_reg_restore();

rc_write_byte(REG_RC500_CLOCK_Q_CONTROL,0x00); //计算90度相移的延时单元数
delay_us(100);
rc_write_byte(REG_RC500_CLOCK_Q_CONTROL,0x40);
//
rc_write_byte(REG_RC500_BIT_PHASE,0xAD);       //固定值，不可改变   
rc_write_byte(REG_RC500_RX_THRESHOLD,0xFF);    //接收阈值最低
rc_write_byte(REG_RC500_RX_CONTROL2,0x41);     //省电模式，解码源：内部解码器
//rc500_write_byte(REG_RC500_RX_CONTROL2,0x01);   //省电模式disable，解码源：内部解码器
//
rc_write_byte(REG_RC500_FIFO_LEVEL,0x04);   
rc_write_byte(REG_RC500_TIMER_CONTROL,0x02);   //数据传输完毕后自动运行  
//
rc_write_byte(REG_RC500_IRQ_PIN_CONFIG,0x03);  //中断低电平有效
//
rc_write_byte(REG_RC500_INTERRUPT_EN,0x7f);    //禁止所有中断
rc_write_byte(REG_RC500_INTERRUPT_RQ,0x7f);    //清除所有中断标记
//
bgIsoType = 0;
rc_power_off();
#ifdef _RC_OP_LOCK_
os_unlock();
#endif
return;
}

void rc_init(void)
{
rc_init0();
return;
}

/*=============================================================================
函数：rc_iso14443_typeb_init
功能：初始化RC531寄存器，使其可以对MM400L卡片操作
===============================================================================*/
void rc_iso14443_typeb_init(void)
{
#ifdef _RC_OP_LOCK_
os_lock();	
#endif

rc_write_byte(REG_RC500_RX_CONTROL1,0x73);
rc_write_byte(REG_RC500_CLOCK_Q_CONTROL,0xc8);
rc_write_byte(REG_RC500_CODER_CONTROL,0x20);
rc_write_byte(REG_RC500_DECODER_CONTROL,0x39);  //不允许一次接收多个帧
//rc_write_byte(REG_RC500_DECODER_CONTROL,0x79);      //允许一次接收多个帧
rc_write_byte(REG_RC500_RX_WAIT,0x08);
rc_write_byte(REG_RC500_CHANNEL_REDUNDANCY,0x2C);
rc_write_byte(REG_RC500_CRC_PRESET_LSB,0xFF);
rc_write_byte(REG_RC500_CRC_PRESET_MSB,0xFF);
rc_write_byte(REG_RC500_TIMER_CLOCK,0x0b);
rc_write_byte(REG_RC500_BIT_PHASE,0xad);
rc_write_byte(REG_RC500_MOD_WIDTH,0x3f);
rc_write_byte(REG_RC500_MOD_WIDTH_SOF,0x3f);
rc_write_byte(REG_RC500_TX_CONTROL,0x4b);
rc_write_byte(REG_RC500_RX_THRESHOLD,0x9b);
rc_write_byte(REG_RC500_TEST_DIGI_SELECT,0x83);
rc_write_byte(REG_RC500_TEST_ANA_SELECT,0x05);
rc_write_byte(REG_RC500_CW_CONDUCTANCE,0x3f);
rc_write_byte(REG_RC500_MOD_CONDUCTANCE,0x03);   //14%
rc_write_byte(REG_RC500_TYPEB_FRAMING,0x05);
rc_write_byte(REG_RC500_BPSK_DEM_CONTROL,0xfe);
//
bgIsoType = 1;

#ifdef _RC_OP_LOCK_
os_unlock();
#endif
return;	
}	

/*=============================================================================
函数：rc_iso14443_typea_init
功能：初始化RC531寄存器，使其可以对M1卡片操作
===============================================================================*/
void rc_iso14443_typea_init(void)
{
#ifdef _RC_OP_LOCK_
os_lock();	
#endif
bgIsoType = 0;
rc_write_byte(REG_RC500_TX_CONTROL,0x5b);            //0x11
rc_write_byte(REG_RC500_CW_CONDUCTANCE,0x3f);        //0x12
rc_write_byte(REG_RC500_MOD_CONDUCTANCE,0x3f);       //0x13
rc_write_byte(REG_RC500_CODER_CONTROL,0x19);         //0x14
rc_write_byte(REG_RC500_MOD_WIDTH,0x13);             //0x15   
rc_write_byte(REG_RC500_MOD_WIDTH_SOF,0x00);          //0x16 
rc_write_byte(REG_RC500_TYPEB_FRAMING,0x3b);         //0x17
rc_write_byte(REG_RC500_RX_CONTROL1,0x73);           //0x19
rc_write_byte(REG_RC500_DECODER_CONTROL,0x08);       //0x1a
rc_write_byte(REG_RC500_BIT_PHASE,0xad);             //0x1b
rc_write_byte(REG_RC500_RX_THRESHOLD,0xff);          //0x1c
rc_write_byte(REG_RC500_BPSK_DEM_CONTROL,0x1e);       //0x1d 
rc_write_byte(REG_RC500_CLOCK_Q_CONTROL,0x47);        //0x1f 
rc_write_byte(REG_RC500_RX_WAIT,0x06);               //0x21 
rc_write_byte(REG_RC500_CHANNEL_REDUNDANCY,0x0f);    //0x22
rc_write_byte(REG_RC500_CRC_PRESET_LSB,0x63);         //0x23 
rc_write_byte(REG_RC500_CRC_PRESET_MSB,0x63);         //0x24 
rc_write_byte(REG_RC500_TIMER_CLOCK,0x07);           //0x2a
rc_write_byte(REG_RC500_TEST_ANA_SELECT,0x09);        //0x3a
rc_write_byte(REG_RC500_TEST_DIGI_SELECT,0x09);       //0x3d
//
#ifdef _RC_OP_LOCK_
os_unlock();
#endif
return;	
}	

/*=============================================================================
函数：rc_iso14443_icode1_init
功能：
===============================================================================*/
void rc_iso15693_icode1_init(void)
{
//rc_init_15693_std_mode();	
return;	
}	


/*==============================================================================================
函数：rc_wait_irq
功能：最大等待20ms
复位后irq引脚高有效
================================================================================================*/
//#define _RC_IRQ_PIN_CHECK_
void rc_wait_irq(void)
{
UWORD i;

for(i=0;i<16000;i++){
	//if(rc_irq()) break;
  if(rc_read_byte(REG_RC500_PRIMARY_STATUS)&0x08) break;
	//delay_us(50);	
  }
rc_clr_time_out();
  
return;	
}	


/*==============================================================================================
函数：
功能：
================================================================================================*/
UBYTE rc_rece_bits(void)
{
UBYTE bits,bytes;

bytes = rc_read_byte(REG_RC500_FIFO_LENGTH);
bits = rc_read_byte(REG_RC500_SECONDARY_STATUS)&0x07;

if(bits==0) return (bytes*8);
return (bytes*8-8+bits);		
}	

/*==============================================================================================
函数：
功能：
================================================================================================*/
//#define _DEBUG_RC_SEND_CMD_
UBYTE rc_send_cmd(UBYTE *inbuf,UBYTE inbytes,UBYTE irq_mask)
{
UBYTE ret,rece_bits;	
#ifdef _DEBUG_RC_SEND_CMD_
char info_buf[50];
#endif
//
rc_idle();
rc_clr_irq();
rc_flush_fifo();
//
rc_write_byte(REG_RC500_INTERRUPT_EN,irq_mask|0x80);
// 
rc_write_data_n(inbuf,inbytes);
rc_write_byte(REG_RC500_COMMAND,RC500_CMD_TRANSCEIVE);
//
rc_wait_irq();
//
ret = rc_read_byte(REG_RC500_INTERRUPT_RQ);
if((ret&BIT_TIMERI)&&!(ret&BIT_RXI)){
   #ifdef _DEBUG_RC_SEND_CMD_
   debug_pintf("\x0d\x0a Err:send cmd,int flag=%02X",(UBYTE)ret);
   #endif	
	 return STATUS_IO_TIMEOUT;
	 }


ret = rc_read_byte(REG_RC500_ERROR_FLAG);
if(!(ret&NORMAL_ERR_MASK)) return 0;
#ifdef _DEBUG_RC_SEND_CMD_
debug_printf("\x0d\x0a Err:send cmd,errflag=%02X",(UBYTE)ret);
#endif	
//	
if(ret & BIT_COLL_ERR) return STATUS_COLLISION_ERROR;           //Collision Error
if(ret & BIT_PARITY_ERR) return STATUS_PARITY_ERROR;            //Parity Error
if(ret & BIT_FIFO_OVERFLOW) return STATUS_BUFFER_OVERFLOW;      //BufferOverflow Error
if(ret & BIT_FRAMING_ERR) return STATUS_FRAMING_ERROR;     
if(!(ret & BIT_CRC_ERR)) return STATUS_ERROR_NY_IMPLEMENTED;
//	
if(rc_read_byte(REG_RC500_FIFO_LENGTH) == 1){
	rece_bits = rc_read_byte(REG_RC500_SECONDARY_STATUS)&0x07;
	if((rece_bits == 4) || (rece_bits == 0)) return STATUS_ACK_SUPPOSED;
	}
return STATUS_CRC_ERROR;
}	




/*==============================================================================================
函数：rc_request
功能：
================================================================================================*/
UBYTE rc_request(UBYTE req_code,UBYTE *atq)
{
UBYTE rece_bits,status;	
#ifdef _DEBUG_RC500_REQUEST_
char info_buf[100];
#endif
	
//bgNhhAuthed = 0;   //去除auth标志

rc_crc_disable();
rc_oddpari_enable();
rc_clr_crypt();
rc_not_in_anticoll();
rc_set_bitframe(7);
rc_power_on();
rc_idle();

rc_set_time_out(REQ_TIME_OUT);
status = rc_send_cmd(&req_code,1,NORMAL_IRQ_MASK);
if(status != 0){
	#ifdef _DEBUG_RC500_REQUEST_
	os_lock();
	sprintf(info_buf,"\x0d\x0a Err:request 1,status=%02X",(UBYTE)status);
	debug_printf(info_buf);
	os_unlock();
	#endif
	return 1;
	}
	
rece_bits = rc_rece_bits();
if(rece_bits != 16){
	#ifdef _DEBUG_RC500_REQUEST_
	os_lock();
	sprintf(info_buf,"\x0d\x0a Err:request 2,rece_bits=%d",(UBYTE)rece_bits);
	debug_printf(info_buf);
	os_unlock();	
	#endif
	return 2;
  }

rc_read_data_n(atq,2);

return 0;	
}	


/*==============================================================================================
函数：rc_request
功能：
================================================================================================*/
//#define _DEBUG_RC500_REQUEST_
//UBYTE rc_request0(UBYTE req_code,UBYTE *atq)
//{
//UBYTE rece_bits,status;	
//	
//
//#ifdef _RC_OP_LOCK_
//os_lock();
//#endif
////bgNhhAuthed = 0;   //去除auth标志
//
////rc_crc_disable();
////rc_oddpari_enable();
////rc_clr_crypt();
////rc_not_in_anticoll();
////rc_set_bitframe(7);
////rc_power_on();
////rc_idle();
//  
//bgRequestOkFlag= 0;
//rc_set_time_out(REQ_TIME_OUT);
//status = rc_send_cmd(&req_code,1,NORMAL_IRQ_MASK);
//if(status != 0){
//	#ifdef _DEBUG_RC500_REQUEST_
//	debug_printf("\x0d\x0a Err:request 1,status=%02X",(UBYTE)status);
//	#endif
//	os_unlock();
//	return 1;
//	}
//	
//rece_bits = rc_rece_bits();
//if(rece_bits != 16){
//	#ifdef _DEBUG_RC500_REQUEST_
//	debug_printf("\x0d\x0a Err:request 2,rece_bits=%d",(UBYTE)rece_bits);
//	#endif
//	#ifdef _RC_OP_LOCK_
//	os_unlock();
//	#endif
//	return 2;
//  }
//
//rc_read_data_n(atq,2);
//
//#ifdef _RC_OP_LOCK_
//os_unlock();
//#endif
//if(memcmp(atq,"\x04\x00",2) == 0) bgRequestOkFlag=2;
//else bgRequestOkFlag=1;
//return 0;	
//}	
//
//
//UBYTE rc_request(UBYTE req_code,UBYTE *atq)
//{
//UBYTE buf[2],ret;
//
//rc_not_in_anticoll();
//rc_power_on();
//
//if(bgRequestOkFlag){
//   rc_crc_enable();
//   rc_idle();
//   rc_set_bitframe(0);
//   rc_oddpari_enable();
//   //
//   buf[0] = PICC_READ;buf[1] = 0xff;
//   rc_write_data_n(buf,2);
//   rc_write_byte(REG_RC500_COMMAND,RC500_CMD_TRANSCEIVE);
//	 //	 
//	 }
//	 
//rc_clr_crypt();
//rc_crc_disable();
//rc_set_bitframe(7);
//rc_oddpari_enable();
////	 
//ret = rc_request0(req_code,atq);
//return ret;
//}	


/*=======================================================================
函数：rc_anticoll
功能：
=========================================================================*/
UBYTE rc_anticoll(UBYTE select_code,UBYTE bcnt,UBYTE *snr)    
{
UBYTE ch,ch2;
UBYTE i,nbytes;
UBYTE rbuf[5],snr_in[4];
UBYTE rbytes=0,nbits=0,rbits=0,collpos=0,byteOffset;
 
#ifdef _RC_OP_LOCK_
os_lock();
#endif

rc_set_time_out(ANTICOLL_TIME_OUT);
rc_crc_disable();
rc_in_anticoll();
rc_clr_crypt();
rc_idle();
      
for(i=0;i<(bcnt/8);i++) snr_in[i] = snr[i];

loop:
	

rc_idle();                           // terminate probably running command
rc_flush_fifo();                     // flush FIFO buffer
rc_crc_disable();                    //禁止CRC校验
rc_oddpari_enable();                 //允许奇校验

nbits = bcnt % 8;                         // remaining number of bits
if(nbits){
  rc_set_bitframe((nbits << 4) | nbits);  // TxLastBits/RxAlign auf nb_bi
  nbytes = bcnt / 8 + 1;   
  if(nbits == 7){
     rc_set_bitframe(nbits);              // reset RxAlign to zero
     rc_pari_disable();                //禁止CRC校验，不允许奇校验
     }
  }
else{
  nbytes = bcnt / 8;
  }

rc_write_data(select_code);
rc_write_data(0x20 + ((bcnt/8) << 4) + nbits);    //number of bytes send
for(i=0;i<nbytes;i++){
   rc_write_data(snr_in[i]);
   }
 
//设置中断
rc_clr_irq();
rc_write_byte(REG_RC500_INTERRUPT_EN,(UBYTE)0xa8);  //允许超时中断和接收中断
rc_set_time_out(10);
rc_write_byte(REG_RC500_COMMAND,RC500_CMD_TRANSCEIVE);
   
//检测状态
rc_wait_irq();

if((rc_read_byte(REG_RC500_INTERRUPT_RQ) & 0x08) == 0){
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return (UBYTE)-1;    //无接收数据
	 }

if(rc_read_byte(REG_RC500_ERROR_FLAG)&0x01){
    collpos = rc_read_byte(REG_RC500_COLL_POS);
    }
   
rbytes = rc_read_byte(REG_RC500_FIFO_LENGTH);
rbits =  rc_rece_bits();
for(i=0;i<rbytes;i++) rbuf[i] = rc_read_data();
  
if(nbits == 7){
   ch = 0x00;
   for(i=0;i< rbytes;i++){
      ch2 = rbuf[i];
      rbuf[i] = (ch >> (i+1)) | (rbuf[i] << (7-i));
      ch = ch2;
      }
   rbits -= rbytes;                             // subtract received parity bits
   collpos += 7-(collpos+6)/9; 
   }
  
if((rc_read_byte(REG_RC500_ERROR_FLAG) & 0x0e) != 0){
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return (UBYTE)-1;      //有错误发生
	 }
if(rbits != (40 - bcnt)) goto label_err;
  
byteOffset = 0;
if(nbits != 0){ // last byte was not complete
   snr_in[nbytes - 1] = snr_in[nbytes - 1] | rbuf[0];
   byteOffset = 1;
   }
for(i=0;i<(4-nbytes);i++){     
   snr_in[nbytes + i] = rbuf[i + byteOffset];
   }
  
if((rc_read_byte(REG_RC500_ERROR_FLAG) & 0x01) != 0){
   bcnt = bcnt + collpos - nbits;
   goto loop;	
   }	  
  
//if((rbuf[0]^rbuf[1]^rbuf[2]^rbuf[3]^rbuf[4]) != 0) goto label_err;
snr[0] = snr_in[0];
snr[1] = snr_in[1];
snr[2] = snr_in[2];
snr[3] = snr_in[3];
snr[4] = rbuf[4];
goto label_ok;

label_err:
rc_not_in_anticoll();
#ifdef _RC_OP_LOCK_
os_unlock();
#endif
return (UBYTE)-1;  

label_ok:
rc_not_in_anticoll();
#ifdef _RC_OP_LOCK_
os_unlock();
#endif
return 0;  
}



/*==============================================================================================
函数：rc_select
功能：
================================================================================================*/
UBYTE rc_select(UBYTE sel_code,UBYTE *snr,UBYTE *sak)
{
UBYTE rece_bits,status;	
UBYTE buf[7];
//	
#ifdef _RC_OP_LOCK_
os_lock();
#endif
rc_clr_crypt();
rc_crc_enable();
rc_oddpari_enable();
rc_not_in_anticoll();
rc_idle();
rc_set_bitframe(0);
//
buf[0] = sel_code;
buf[1] = NVB_MAX_PARAMETER;
memcpy(&buf[2],snr,5);
rc_set_time_out(SEL_TIME_OUT);
status = rc_send_cmd(buf,7,NORMAL_IRQ_MASK);
//
if((status) &&(status != STATUS_ACK_SUPPOSED)){
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return (status|0x80);
	 }
rece_bits = rc_rece_bits();
if(rece_bits == 4){
	status = rc_read_data();
  if((status&0x0a) != 0x0a) return status|0x40;
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
  return 0;	
	}
if(rece_bits != 8){
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return 2;
	 }
//
rc_read_data_n(sak,1);
//
#ifdef _RC_OP_LOCK_
os_unlock();
#endif
return 0;	
}	


/*==============================================================================================
函数：rc_read
功能：
================================================================================================*/
UBYTE rc_read(UBYTE block,UBYTE *outbuf)
{
UBYTE buf[2],status;
UBYTE rece_bits;
//
#ifdef _RC_OP_LOCK_
os_lock();
#endif

rc_not_in_anticoll();
rc_crc_enable();
rc_oddpari_enable();
rc_idle();
rc_set_bitframe(0);
//
buf[0] = PICC_READ;
buf[1] = block;
rc_set_time_out(READ_TIME_OUT);
status = rc_send_cmd(buf,2,NORMAL_IRQ_MASK);
if(status!=0){
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return 1;
	 }
rece_bits = rc_rece_bits();
if(rece_bits != 128){
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return 2;
	 }
rc_read_data_n(outbuf,16);	
//
#ifdef _RC_OP_LOCK_
os_unlock();
#endif
return 0;	
}	


UBYTE rc_read_4bytes(UBYTE block,UBYTE *outbuf)
{
UBYTE buf[2],status;
UBYTE rece_bits;
//
#ifdef _RC_OP_LOCK_
os_lock();
#endif
rc_not_in_anticoll();
rc_crc_enable();
rc_oddpari_enable();
rc_idle();
rc_set_bitframe(0);
//
buf[0] = PICC_READ;
buf[1] = block;
rc_set_time_out(READ_TIME_OUT);
status = rc_send_cmd(buf,2,NORMAL_IRQ_MASK);
if(status!=0){
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return 1;
	 }
rece_bits = rc_rece_bits();
if(rece_bits != 32){
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return 2;
	 }
rc_read_data_n(outbuf,4);	
//
#ifdef _RC_OP_LOCK_
os_unlock();
#endif
return 0;	
}	

/*==============================================================================================
函数：rc_write
功能：
================================================================================================*/
#ifdef _DEBUG_EN_
#define _DEBUG_RC_WRITE_
#endif
UBYTE rc_write(UBYTE block,UBYTE *inbuf)
{
UBYTE buf[2];
UBYTE status,rece_bits;
//

#ifdef _DEBUG_RC_WRITE_
debug_printf("\x0d\x0a write:start");
#endif

#ifdef _RC_OP_LOCK_
os_lock();
#endif
rc_not_in_anticoll();
rc_crc_enable();
rc_crc_rx_disable();
rc_oddpari_enable();
rc_idle();
rc_set_bitframe(0);
//
buf[0] = PICC_WRITE;
buf[1] = block;
rc_set_time_out(WRITE1_TIME_OUT);
status = rc_send_cmd(buf,2,NORMAL_IRQ_MASK);
if((status!=0)&&(status != STATUS_ACK_SUPPOSED)){
	 #ifdef _DEBUG_RC_WRITE_
	 debug_printf("\x0d\x0a write:err1,status=%02X",(UBYTE)status);
	 #endif
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return 1;
	 }
rece_bits = rc_rece_bits();	 
if(rece_bits != 4){
	 #ifdef _DEBUG_RC_WRITE_
	 debug_printf("\x0d\x0a write:err2,rece_bits=%d",rece_bits);	 
	 #endif 
	 #ifdef _RC_OP_LOCK_
   os_unlock();
 	 #endif
 	 return 2;
	 }
status = rc_read_data();
if((status&0x0a) != 0x0a){
	 #ifdef _DEBUG_RC_WRITE_
	 debug_printf("\x0d\x0a write:err3,status=%02X",(UBYTE)status);
	 #endif 
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return 3;	
	 }

rc_not_in_anticoll();
rc_crc_enable();
rc_crc_rx_disable();
rc_oddpari_enable();
rc_idle();
rc_set_bitframe(0);

rc_set_time_out(WRITE2_TIME_OUT);
status = rc_send_cmd(inbuf,16,NORMAL_IRQ_MASK);
if((status!=0)&&(status != STATUS_ACK_SUPPOSED)){
	 #ifdef _DEBUG_RC_WRITE_
	 debug_printf("\x0d\x0a write:err4,status=%02X",(UBYTE)status);
	 #endif
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return 4;
	 }
rece_bits = rc_rece_bits();	 
if(rece_bits != 4){
	 #ifdef _DEBUG_RC_WRITE_
	 debug_printf("\x0d\x0a write:err5,rece_bits=%d",rece_bits);	 
	 #endif 
	 #ifdef _RC_OP_LOCK_
   os_unlock();
 	 #endif
 	 return 5;
	 }
status = rc_read_data();
if((status&0x0a) != 0x0a){
	 #ifdef _DEBUG_RC_WRITE_
	 debug_printf("\x0d\x0a write:err6,status=%02X",status);	 
	 #endif 
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return 6;	
	 }

#ifdef _RC_OP_LOCK_
os_unlock();
#endif

	 #ifdef _DEBUG_RC_WRITE_
	 debug_printf("\x0d\x0a write:ok");
	 #endif


return 0;	
}	

UBYTE rc_write_4bytes(UBYTE block,UBYTE *inbuf)
{
UBYTE buf[2];
UBYTE status,rece_bits;
//
#ifdef _RC_OP_LOCK_
os_lock();
#endif
rc_not_in_anticoll();
rc_crc_enable();
rc_crc_rx_disable();
rc_oddpari_enable();
rc_idle();
rc_set_bitframe(0);
//
buf[0] = PICC_WRITE;
buf[1] = block;
rc_set_time_out(WRITE1_TIME_OUT);
status = rc_send_cmd(buf,2,NORMAL_IRQ_MASK);
if((status!=0)&&(status != STATUS_ACK_SUPPOSED)){
	 #ifdef _DEBUG_RC_WRITE_
	 debug_printf("\x0d\x0a write:err1");
	 #endif
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return 1;
	 }
rece_bits = rc_rece_bits();	 
if(rece_bits != 4){
	 #ifdef _DEBUG_RC_WRITE_
	 debug_printf("\x0d\x0a write:err2");	 
	 #endif 
	 #ifdef _RC_OP_LOCK_
   os_unlock();
 	 #endif
 	 return 2;
	 }
status = rc_read_data();
if((status&0x0a) != 0x0a){
	 #ifdef _DEBUG_RC_WRITE_
	 debug_printf("\x0d\x0a write:err3,status=%02X",(UBYTE)status);
	 #endif 
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return 3;	
	 }

rc_not_in_anticoll();
rc_crc_enable();
rc_crc_rx_disable();
rc_oddpari_enable();
rc_idle();
rc_set_bitframe(0);

rc_set_time_out(WRITE2_TIME_OUT);
status = rc_send_cmd(inbuf,4,NORMAL_IRQ_MASK);
if((status!=0)&&(status != STATUS_ACK_SUPPOSED)){
	 #ifdef _DEBUG_RC_WRITE_
	 debug_printf("\x0d\x0a write:err4,status=%02X",(UBYTE)status);
	 #endif
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return 4;
	 }
rece_bits = rc_rece_bits();	 
if(rece_bits != 4){
	 #ifdef _DEBUG_RC_WRITE_
	 debug_printf("\x0d\x0a write:err5");	 
	 #endif 
	 #ifdef _RC_OP_LOCK_
   os_unlock();
 	 #endif
 	 return 5;
	 }
status = rc_read_data();
if((status&0x0a) != 0x0a){
	 #ifdef _DEBUG_RC_WRITE_
	 debug_printf("\x0d\x0a write:err6");	 
	 #endif 
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return 6;	
	 }

#ifdef _RC_OP_LOCK_
os_unlock();
#endif
return 0;	
}	

/*==============================================================================================
函数：rc_value_op
功能：
================================================================================================*/
//#define _DEBUG_RC_VALUE_OP_
//UBYTE rc_value_op(UBYTE op_mode,UBYTE sr_block,UBYTE *value,UBYTE dest_block)
//{
//UBYTE status,rece_bits;
//UBYTE buf[2];
////
//#ifdef _RC_OP_LOCK_
//os_lock();
//#endif
//rc_not_in_anticoll();
//rc_crc_enable();
//rc_crc_rx_disable();
//rc_oddpari_enable();
//rc_idle();
//rc_set_bitframe(0);
////
//buf[0] = op_mode;
//buf[1] = sr_block;
//rc_set_time_out(VALUE1_TIME_OUT);
//status = rc_send_cmd(buf,2,NORMAL_IRQ_MASK);
//if((status!=0)&&(status != STATUS_ACK_SUPPOSED)){
//	  #ifdef _DEBUG_RC_VALUE_OP_
//	  debug_printf("\x0d\x0a value op:err1,status=%2X",status);	 
//	  #endif 
//	  #ifdef _RC_OP_LOCK_
//    os_unlock();
//    #endif
//    return 1;
//    }
//rece_bits = rc_rece_bits();    
//if(rece_bits != 4){
//	  #ifdef _DEBUG_RC_VALUE_OP_
//	  debug_printf("\x0d\x0a value op:err2,rece_bits=%d",rece_bits);	 
//	  #endif 
//	  #ifdef _RC_OP_LOCK_
//    os_unlock();
//	  #endif
//	  return 2;
//	  }
//status = rc_read_data();
//if((status&0x0a) != 0x0a){
//	  #ifdef _DEBUG_RC_VALUE_OP_
//	  debug_printf("\x0d\x0a value op:err3,status=%02X",(UBYTE)status);	 
//	  #endif 
//	  #ifdef _RC_OP_LOCK_
//	  os_unlock();
//	  #endif
//	  return 3;	
//	  }
////
//rc_not_in_anticoll();
//rc_crc_enable();
//rc_crc_rx_disable();
//rc_oddpari_enable();
//rc_idle();
//rc_set_bitframe(0);
//
//rc_set_time_out(VALUE2_TIME_OUT);
//status = rc_send_cmd(value,4,NORMAL_IRQ_MASK);
//if(status == STATUS_IO_TIMEOUT) goto label_transfer;
//if((status!=0)&&(status != STATUS_ACK_SUPPOSED)){
//	  #ifdef _DEBUG_RC_VALUE_OP_
//	  debug_printf("\x0d\x0a value op:err4,status=%2X",status);	 
//	  #endif 
//	  #ifdef _RC_OP_LOCK_
//	  os_unlock();
//	  #endif
//    return 4;
//    }
//rece_bits = rc_rece_bits();    
//if(rece_bits != 5){
//	  #ifdef _DEBUG_RC_VALUE_OP_
//	  debug_printf("\x0d\x0a value op:err5,rece_bits=%d",rece_bits);	 
//	  #endif 
//	  #ifdef _RC_OP_LOCK_
//    os_unlock();
//	  #endif
//	  return 5;
//	  }
//status = rc_read_data();
//if((status&0x0a) != 0x0a){
//	  #ifdef _DEBUG_RC_VALUE_OP_
//	  debug_printf("\x0d\x0a value op:err6,status=%02X",(UBYTE)status);	 
//	  #endif 
//	  #ifdef _RC_OP_LOCK_
//    os_unlock();
//	  #endif
//	  return 6;	
//	  }
////
//label_transfer:
//rc_not_in_anticoll();
//rc_crc_enable();
//rc_crc_rx_disable();
//rc_oddpari_enable();
//rc_idle();
//rc_set_bitframe(0);
//	
//buf[0] = PICC_TRANSFER;
//buf[1] = dest_block;
//rc_set_time_out(VALUE3_TIME_OUT);
//status = rc_send_cmd(buf,2,NORMAL_IRQ_MASK);
//if((status!=0)&&(status != STATUS_ACK_SUPPOSED)){
//	  #ifdef _DEBUG_RC_VALUE_OP_
//	  debug_printf("\x0d\x0a value op:err7,status=%2X",status);	 
//	  #endif 
//	  #ifdef _RC_OP_LOCK_
//    os_unlock();
//    #endif
//    return 7;
//    }
//rece_bits = rc_rece_bits();    
//if(rece_bits != 4){
//	  #ifdef _DEBUG_RC_VALUE_OP_
//	  debug_printf("\x0d\x0a value op:err8,rece_bits=%d",rece_bits);	 
//	  #endif 
//	  #ifdef _RC_OP_LOCK_
//    os_unlock();
//	  #endif
//	  return 8;
//	  }
//status = rc_read_data();
//if((status&0x0a) != 0x0a){
//	  #ifdef _DEBUG_RC_VALUE_OP_
//	  debug_printf("\x0d\x0a value op:err9,status=%02X",(UBYTE)status);	 
//	  #endif 
//	  #ifdef _RC_OP_LOCK_
//	  os_unlock();
//	  #endif
//	  return 9;	
//	  }
////
//#ifdef _RC_OP_LOCK_
//os_unlock();
//#endif
//return 0;
//}


#ifdef _DEBUG_
//#define _DEBUG_RC_VALUE_OP0_
#endif
UBYTE rc_value_op0(UBYTE op_mode,UBYTE sr_block,UBYTE *value)
{
UBYTE status,rece_bits;
UBYTE buf[2];
//
#ifdef _RC_OP_LOCK_
os_lock();
#endif
rc_not_in_anticoll();
rc_crc_enable();
rc_crc_rx_disable();
rc_oddpari_enable();
rc_idle();
rc_set_bitframe(0);
//
buf[0] = op_mode;
buf[1] = sr_block;
rc_set_time_out(VALUE1_TIME_OUT);
status = rc_send_cmd(buf,2,NORMAL_IRQ_MASK);
if((status!=0)&&(status != STATUS_ACK_SUPPOSED)){
	  #ifdef _DEBUG_RC_VALUE_OP0_
	  debug_printf("\x0d\x0a value op0:err1,status=%2X",status);	 
	  #endif 
	  #ifdef _RC_OP_LOCK_
    os_unlock();
    #endif
    return 1;
    }
rece_bits = rc_rece_bits();    
if(rece_bits != 4){
	  #ifdef _DEBUG_RC_VALUE_OP0_
	  debug_printf("\x0d\x0a value op0:err2,rece_bits=%d",rece_bits);	 
	  #endif 
	  #ifdef _RC_OP_LOCK_
    os_unlock();
	  #endif
	  return 2;
	  }
status = rc_read_data();
if((status&0x0a) != 0x0a){
	  #ifdef _DEBUG_RC_VALUE_OP0_
	  debug_printf("\x0d\x0a value op0:err3,status=%02X",(UBYTE)status);	 
	  #endif 
	  #ifdef _RC_OP_LOCK_
	  os_unlock();
	  #endif
	  return 3;	
	  }
//
rc_not_in_anticoll();
rc_crc_enable();
rc_crc_rx_disable();
rc_oddpari_enable();
rc_idle();
rc_set_bitframe(0);

rc_set_time_out(VALUE2_TIME_OUT);
status = rc_send_cmd(value,4,NORMAL_IRQ_MASK);
if(status == STATUS_IO_TIMEOUT) goto label_exit;
if((status!=0)&&(status != STATUS_ACK_SUPPOSED)){
	  #ifdef _DEBUG_RC_VALUE_OP0_
	  debug_printf("\x0d\x0a value op0:err4,status=%2X",status);	 
	  #endif 
	  #ifdef _RC_OP_LOCK_
	  os_unlock();
	  #endif
    return 4;
    }
rece_bits = rc_rece_bits();    
if(rece_bits != 5){
	  #ifdef _DEBUG_RC_VALUE_OP0_
	  debug_printf("\x0d\x0a value op0:err5,rece_bits=%d",rece_bits);	 
	  #endif 
	  #ifdef _RC_OP_LOCK_
    os_unlock();
	  #endif
	  return 5;
	  }
status = rc_read_data();
if((status&0x0a) != 0x0a){
	  #ifdef _DEBUG_RC_VALUE_OP0_
	  debug_printf("\x0d\x0a value op0:err6,status=%02X",(UBYTE)status);	 
	  #endif 
	  #ifdef _RC_OP_LOCK_
    os_unlock();
	  #endif
	  return 6;	
	  }

label_exit:
#ifdef _RC_OP_LOCK_
os_unlock();
#endif
return 0;
}

/*==============================================================================================
函数：
功能：
================================================================================================*/
#ifdef _DEBUG_
//#define _DEBUG_RC_TRANSFER_
#endif
UBYTE rc_transfer(UBYTE dest_block)
{
UBYTE status,rece_bits;
UBYTE buf[2];
	
#ifdef _RC_OP_LOCK_
os_lock();
#endif
	
rc_not_in_anticoll();
rc_crc_enable();
rc_crc_rx_disable();
rc_oddpari_enable();
rc_idle();
rc_set_bitframe(0);
	
buf[0] = PICC_TRANSFER;
buf[1] = dest_block;
rc_set_time_out(VALUE3_TIME_OUT);
status = rc_send_cmd(buf,2,NORMAL_IRQ_MASK);
if((status!=0)&&(status != STATUS_ACK_SUPPOSED)){
	  #ifdef _DEBUG_RC_TRANSFER_
	  debug_printf("\x0d\x0a rc_transfer:err7,status=%2X",status);	 
	  #endif 
	  #ifdef _RC_OP_LOCK_
    os_unlock();
    #endif
    return 7;
    }
rece_bits = rc_rece_bits();    
if(rece_bits != 4){
	  #ifdef _DEBUG_RC_TRANSFER_
	  debug_printf("\x0d\x0a rc_transfer:err8,rece_bits=%d",rece_bits);	 
	  #endif 
	  #ifdef _RC_OP_LOCK_
    os_unlock();
	  #endif
	  return 8;
	  }
status = rc_read_data();
if((status&0x0a) != 0x0a){
	  #ifdef _DEBUG_RC_TRANSFER_
	  debug_printf("\x0d\x0a rc_transfer:err9,status=%02X",(UBYTE)status);	 
	  #endif 
	  #ifdef _RC_OP_LOCK_
	  os_unlock();
	  #endif
	  return 9;	
	  }
//
#ifdef _RC_OP_LOCK_
os_unlock();
#endif
return 0;
}


/*==============================================================================================
函数：rc_crc_a
功能：
================================================================================================*/
UWORD rc_crc_a(UBYTE *inbuf,UBYTE inbytes)
{
UWORD tword;
//
#ifdef _RC_OP_LOCK_
os_lock();
#endif
rc_idle();
rc_flush_fifo();
// 
rc_write_data_n(inbuf,inbytes);
rc_write_byte(REG_RC500_COMMAND,RC500_CMD_CALCCRC);
while(rc_read_byte(REG_RC500_SECONDARY_STATUS)&0x20){};
//
*((char *)&tword+INT_HIGH) = rc_read_byte(REG_RC500_CRC_RESULT_MSB);
*((char *)&tword+INT_LOW) =  rc_read_byte(REG_RC500_CRC_RESULT_LSB);
//
#ifdef _RC_OP_LOCK_
os_unlock();
#endif
return tword;
}



/*================================================================
函数：rc_auth
功能：
==================================================================*/
UBYTE rc_auth(UBYTE keyAB,UBYTE sector)
{
UBYTE  keycoded[12];
UBYTE  i,ch1,ch2;
 
#ifdef _RC_OP_LOCK_
os_lock();   
#endif
//load key   
for(i=0;i<6;i++){
  ch1 = ((UBYTE)gRc500Key[i])&0x0f;
  ch2 = ch1*16;
  keycoded[2*i+1] = ch1 | ((UBYTE)(~ch2)&0xf0);

  ch1 = (UBYTE)gRc500Key[i]&(UBYTE)0xf0;
  ch2 = (UBYTE)ch1/16;
  keycoded[2*i] = ((UBYTE)~ch1&0xf0) | ch2;
  }

rc_idle();                    // terminate probably running command
rc_flush_fifo();              // flush FIFO buffer
rc_write_data_n(keycoded,12);

rc_clr_irq();
rc_write_byte(REG_RC500_INTERRUPT_EN,(UBYTE)BIT_SETI|BIT_IDLEI|BIT_TIMERI); //允许IDLE中断,超时中断
rc_set_time_out(20);                               //1.5ms
rc_write_byte(REG_RC500_COMMAND,RC500_CMD_LOADKEY);      

rc_wait_irq();
if(rc_read_byte(REG_RC500_INTERRUPT_RQ) & BIT_TIMERI){
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return (UBYTE)1; 
	 }
if(rc_read_byte(REG_RC500_ERROR_FLAG)){
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return (UBYTE)2;
	 }

//auth1--------------------------------------------------------
rc_idle();
rc_flush_fifo();
//
rc_write_data(keyAB);
rc_write_data(sector*4);
rc_write_data(gThisCardSnr[0]);
rc_write_data(gThisCardSnr[1]);
rc_write_data(gThisCardSnr[2]);
rc_write_data(gThisCardSnr[3]);
//
rc_clr_irq();
//允许超时中断,接收中断，IDLE
rc_write_byte(REG_RC500_INTERRUPT_EN,(UBYTE)BIT_SETI|BIT_IDLEI|BIT_TIMERI|BIT_RXI); 
rc_set_time_out(20);
rc_write_byte(REG_RC500_COMMAND,RC500_CMD_AUTHENT1);      
//
rc_wait_irq();
if(!(rc_read_byte(REG_RC500_INTERRUPT_RQ)&BIT_IDLEI)){
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return (UBYTE)3;
	 } 
if(rc_read_byte(REG_RC500_SECONDARY_STATUS) & 0x07){
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return (UBYTE)4;       // Check RxLastBits for error
	 }

//auth2-----------------------------------------------------------------
rc_idle();
rc_clr_irq();
//
rc_write_byte(REG_RC500_INTERRUPT_EN,(UBYTE)BIT_SETI|BIT_IDLEI|BIT_TIMERI|BIT_RXI); 
rc_set_time_out(20);
rc_write_byte(REG_RC500_COMMAND,RC500_CMD_AUTHENT2);      
//
rc_wait_irq();
if(rc_read_byte(REG_RC500_ERROR_FLAG)){
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return (UBYTE)5;
	 }
if(rc_read_byte(REG_RC500_CONTROL)&BIT_CRYPTO1ON){
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return 0;   //ok
	 }
//
#ifdef _RC_OP_LOCK_
os_unlock();
#endif
return (UBYTE)6;
}



UBYTE rc_auth2(UBYTE keyAB,UBYTE sector)
{
UBYTE  keycoded[12];
UBYTE  i,ch1,ch2;
 
#ifdef _RC_OP_LOCK_
os_lock();   
#endif
//load key   
for(i=0;i<6;i++){
  ch1 = ((UBYTE)gRc500Key[i])&0x0f;
  ch2 = ch1*16;
  keycoded[2*i+1] = ch1 | ((UBYTE)(~ch2)&0xf0);

  ch1 = (UBYTE)gRc500Key[i]&(UBYTE)0xf0;
  ch2 = (UBYTE)ch1/16;
  keycoded[2*i] = ((UBYTE)~ch1&0xf0) | ch2;
  }

rc_idle();                    // terminate probably running command
rc_flush_fifo();              // flush FIFO buffer
rc_write_data_n(keycoded,12);

rc_clr_irq();
rc_write_byte(REG_RC500_INTERRUPT_EN,(UBYTE)BIT_SETI|BIT_IDLEI|BIT_TIMERI); //允许IDLE中断,超时中断
rc_set_time_out(20);                               //1.5ms
rc_write_byte(REG_RC500_COMMAND,RC500_CMD_LOADKEY);      

rc_wait_irq();
if(rc_read_byte(REG_RC500_INTERRUPT_RQ) & BIT_TIMERI){
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return (UBYTE)1; 
	 }
//if(rc_read_byte(REG_RC500_ERROR_FLAG)){
//	 #ifdef _RC_OP_LOCK_
//   os_unlock();
//	 #endif
//	 return (UBYTE)2;
//	 }

//auth1--------------------------------------------------------
rc_idle();
rc_flush_fifo();
//
rc_write_data(keyAB);
rc_write_data(sector*4);
rc_write_data(gThisCardSnr[0]);
rc_write_data(gThisCardSnr[1]);
rc_write_data(gThisCardSnr[2]);
rc_write_data(gThisCardSnr[3]);
//
rc_clr_irq();
//允许超时中断,接收中断，IDLE
rc_write_byte(REG_RC500_INTERRUPT_EN,(UBYTE)BIT_SETI|BIT_IDLEI|BIT_TIMERI|BIT_RXI); 
rc_set_time_out(20);
rc_write_byte(REG_RC500_COMMAND,RC500_CMD_AUTHENT1);      
//
rc_wait_irq();
if(!(rc_read_byte(REG_RC500_INTERRUPT_RQ)&BIT_IDLEI)){
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return (UBYTE)3;
	 } 
if(rc_read_byte(REG_RC500_SECONDARY_STATUS) & 0x07){
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return (UBYTE)4;       // Check RxLastBits for error
	 }

//auth2-----------------------------------------------------------------
rc_idle();
rc_clr_irq();
//
rc_write_byte(REG_RC500_INTERRUPT_EN,(UBYTE)BIT_SETI|BIT_IDLEI|BIT_TIMERI|BIT_RXI); 
rc_set_time_out(20);
rc_write_byte(REG_RC500_COMMAND,RC500_CMD_AUTHENT2);      
//
rc_wait_irq();
if(rc_read_byte(REG_RC500_ERROR_FLAG)){
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return (UBYTE)5;
	 }
if(rc_read_byte(REG_RC500_CONTROL)&BIT_CRYPTO1ON){
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return 0;   //ok
	 }
//
#ifdef _RC_OP_LOCK_
os_unlock();
#endif
return (UBYTE)6;
}



/*==============================================================================================
函数：rc_halta
功能：
================================================================================================*/
//#define _DEBUG_RC_HALT_
UBYTE rc_halta(void)
{
UBYTE buf[2],status;

#ifdef _RC_OP_LOCK_
os_lock();
#endif
rc_not_in_anticoll();
rc_crc_enable();
rc_oddpari_enable();
rc_idle();
rc_set_bitframe(0);
//
buf[0] = PICC_HALT;
buf[1] = 0x00;
rc_set_time_out(HALT_TIME_OUT);
status = rc_send_cmd(buf,2,NORMAL_IRQ_MASK);
if((status==0)||(status==STATUS_IO_TIMEOUT)){
	 #ifdef _RC_OP_LOCK_
   os_unlock();
	 #endif
	 return 0;
	 }

#ifdef _DEBUG_RC_HALT_
debug_printf("\x0d\x0a err halt: status=%02X",(UBYTE)status);
#endif

#ifdef _RC_OP_LOCK_
os_unlock();
#endif
return 1;	
}	


/*==============================================================================================
函数：rc_select_op_type
功能：
================================================================================================*/
UBYTE rc_select_op_type(UBYTE op_type)
{
switch(op_type){	
  case ISO14443A_M1_TYPE:
  	   rc_iso14443_typea_init();
  	   rc_select_mifare_auth();
       return 0;
  case ISO14443A_SH_TYPE:
  	   rc_iso14443_typea_init();
  	   rc_select_sh_auth(); 
       return 0;
  case ISO14443B_M4_TYPE:
  	   rc_iso14443_typeb_init();
       return 0;
  case ISO15693_ICODE1_TYPE:
  	   rc_iso15693_icode1_init();
       return 0;
  default:	
  	   break;
  }
return (UBYTE)-1;
}	

/*==============================================================================================
函数：rf_select
功能：
================================================================================================*/
//void  rf_select(UBYTE channel)
//{
//channel %= 3;
//switch(channel){
//	case 0:
//	     ant1sel_clr();
//	     ant2sel_clr();
//	     break;	
//  case 1:
//  	   ant2sel_clr();
//  	   ant1sel_set();
//  	   break;
//  default:		
//  	   ant2sel_set();
//  	   ant2sel_set();
//  	   break;  
//  }
//	
//return;	
//}	


//end of file
#endif

