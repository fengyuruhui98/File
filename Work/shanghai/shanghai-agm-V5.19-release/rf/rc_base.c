//rc_base.c

#ifndef _RC_BASE_C_
#define _RC_BASE_C_
//start of file


//��������
UBYTE gRc500Key[6];
UBYTE gThisCardSnr[16];
UBYTE gNhhFlag=0;


/*=============================================================================================
������rc522_write_byte
���ܣ�
===============================================================================================*/
void rc_write_byte(UBYTE addr,UBYTE inbyte)
{
addr = ((addr&0x3f)<<1)&0x7f;	
rc_cs_clr();
spi_send_byte(addr);
spi_send_byte(inbyte);
rc_cs_set();
return;
}

/*=============================================================================================
������rc522_read_byte
���ܣ�
===============================================================================================*/
UBYTE rc_read_byte(UBYTE addr)
{
UBYTE ch;	
//
rc_cs_clr();
spi_send_byte((addr<<1)|0x80);
ch = spi_rece_byte();
rc_cs_set();
//
return ch;
}



/*=============================================================================================
������rc_write_bytes
���ܣ�
===============================================================================================*/
void rc_write_bytes(UBYTE addr,UBYTE *inbuf,UBYTE inbytes)
{
UBYTE i;
	
for(i=0;i<inbytes;i++){
   rc_write_byte(addr,inbuf[i]);
   }

return;
}	

/*=============================================================================================
������rc_read_bytes
���ܣ�
===============================================================================================*/
void rc_read_bytes(UBYTE addr,UBYTE *outbuf,UBYTE outbytes)
{
UBYTE i;	
//
for(i=0;i<outbytes;i++){
	outbuf[i] = rc_read_byte(addr);
	}
//
return;
}


/*=============================================================================================
������rc_set_time_out
���ܣ����ó�ʱ
��ڲ���:
cnt_150us: ��150usΪ��λ����ֵ
===============================================================================================*/
void rc_set_time_out(UWORD cnt_150us)
{
//
rc_write_byte(REG_RC500_TIMER_CLOCK,0x0b);	  //2048/13.56M = 151us
rc_write_byte(REG_RC500_TIMER_RELOAD,cnt_150us);
//
return;
}


/*=============================================================================================
������rc_clr_bits
���ܣ�
===============================================================================================*/
void  rc_clr_bits(UBYTE addr,UBYTE mask)
{
UBYTE tch;
//
tch = rc_read_byte(addr);
rc_write_byte(addr,tch&(UBYTE)(~mask));
//
return;	
}	

/*=============================================================================================
������rc_set_bits
���ܣ�
===============================================================================================*/
void  rc_set_bits(UBYTE addr,UBYTE mask)
{
UBYTE tch;
//
tch = rc_read_byte(addr);
rc_write_byte(addr,tch|mask);
//
return;		
}	




//end of file
#endif
