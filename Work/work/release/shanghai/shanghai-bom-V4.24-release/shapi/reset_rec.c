//reset_rec.c

#ifndef _RESET_REC_C_
#define _RESET_REC_C_
//start of file

UBYTE bgResetRecPtr;

/*======================================================================================
函数：
功能：
========================================================================================*/
void reset_rec_init(void)
{
UBYTE buf[3];
UBYTE ret;
//
ret = ee_read_lrc2(ADDR_START_REC_PTR,1,buf);
if(ret) buf[0] = 0;
bgResetRecPtr = (UBYTE)buf[0]%MAX_RESET_REC;
//
return;	
}	


/*======================================================================================
函数：
功能：
========================================================================================*/
void reset_rec_save(void)
{
UBYTE buf[16];
UWORD addr;

buf[0] = 0x20;
rtc_rd_time(&buf[1]);
lrc_gen(buf,7);
addr = ADDR_START_REC_BASE+(UWORD)bgResetRecPtr*8;
ee_write(addr,8,buf);
//
bgResetRecPtr = (bgResetRecPtr+1)%MAX_RESET_REC;
buf[0] = bgResetRecPtr;
ee_write_lrc2(ADDR_START_REC_PTR,1,buf);
//
return;	
}	

/*======================================================================================
函数：
功能：
========================================================================================*/
UBYTE reset_rec_get(UBYTE index,UBYTE *outbuf)
{
UWORD addr;
UBYTE tch;
//
index = index%MAX_RESET_REC;
tch = (MAX_RESET_REC+bgResetRecPtr-1)%MAX_RESET_REC;
index = (MAX_RESET_REC+tch-index)%MAX_RESET_REC;
addr = ADDR_START_REC_BASE+(UWORD)index*8;	
ee_read(addr,8,outbuf);
if(lrc_chk(outbuf,7) != 0) return (UBYTE)-1;
//
return 0;	
}	



//end of file
#endif



