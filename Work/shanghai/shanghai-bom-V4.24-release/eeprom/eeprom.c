//eeprom.c
//eeprom操作程序
//编制：
//时间：

#ifndef _EEPROM_C_
#define _EEPROM_C_
//start of file

/********************************************************************************
函数:ee_write_lrc2
功能:按照lrc2结构更新数据
*********************************************************************************/
UBYTE ee_write_lrc2(UWORD addr,UBYTE bytes,UBYTE *buf)
{
UBYTE ret;
UBYTE lbuf[2];	
	
lrc_gen(buf,bytes);
ret = ee_write((UWORD)(addr+bytes+1),(UBYTE)(bytes+1),buf);
if(ret != 0) return ret;

lbuf[0] = 0x55;
lbuf[1] = 0x55;
ret = ee_write((UWORD)(addr+2*bytes+2),2,lbuf);
if(ret != 0) return ret;

ret = ee_write(addr,(UBYTE)(bytes+1),buf);
if(ret != 0) return ret;

lbuf[0] = 0;
lbuf[1] = 0;
ret = ee_write((UWORD)(addr+2*bytes+2),2,lbuf);
if(ret != 0) return ret;
	
return 0;	
}

/******************************************************************************
函数:ee_read_lrc2
功能:按照lrc2结构更新数据
*******************************************************************************/
UBYTE ee_read_lrc2(UWORD addr,UBYTE bytes,UBYTE *buf)
{
UBYTE lbuf[2];
UBYTE ret;

ret = ee_read((UWORD)(addr+2*bytes+2),2,lbuf);
if(ret != 0) return ret;	
if((lbuf[0] == 0x55) && (lbuf[1] == 0x55)){
   ret = ee_read((UWORD)(addr+bytes+1),(UBYTE)(bytes+1),buf);
   if(ret != 0) return ret;
   if(lrc_chk(buf,bytes) != 0) return (UBYTE)-5;
   return 0;	
   }	 	

ret = ee_read(addr,(UBYTE)(bytes+1),buf);
if(ret != 0) return ret;
if(lrc_chk(buf,bytes) != 0) return (UBYTE)-5;
	
return 0;	
}

char get_tongji(void)
{
uchar tbuf[20];
	
	if(ee_read_lrc2(200,10,tbuf)!=0)
		return (char)-1;
	*((char *)&gNumTotal+0)=tbuf[0];
	*((char *)&gNumTotal+1)=tbuf[1];
	
	*((char *)&gValueTotal+0)=tbuf[2];
	*((char *)&gValueTotal+1)=tbuf[3];
	*((char *)&gValueTotal+2)=tbuf[4];
	*((char *)&gValueTotal+3)=tbuf[5];
	
	*((char *)&gBackTransCnt+0)= tbuf[6];
	*((char *)&gBackTransCnt+1)= tbuf[7];
	*((char *)&gBackTransCnt+2)= tbuf[8];
	*((char *)&gBackTransCnt+3)= tbuf[9];
	
	gBackTransCntNot = ~gBackTransCnt;
	gNumTotalNot =~ gNumTotal;
	gValueTotalNot =~ gValueTotal;
	
	return 0;

}

char wri_tongji(void)
{
uchar tbuf[20];	

	tbuf[0] = *((char *)&gNumTotal+0);
	tbuf[1] = *((char *)&gNumTotal+1);
	
	tbuf[2] = *((char *)&gValueTotal+0);
	tbuf[3] = *((char *)&gValueTotal+1);
	tbuf[4] = *((char *)&gValueTotal+2);
	tbuf[5] = *((char *)&gValueTotal+3);
	tbuf[6] = *((char *)&gBackTransCnt+0);
	tbuf[7] = *((char *)&gBackTransCnt+1);
	tbuf[8] = *((char *)&gBackTransCnt+2);
	tbuf[9] = *((char *)&gBackTransCnt+3);
	
	//if(ee_write_lrc2(2,10,tbuf)!=0) return (char)-1;	
	if(ee_write_lrc2(800,10,tbuf)!=0) return (char)-1;
	
	return 0;
}
//end of file
#endif













