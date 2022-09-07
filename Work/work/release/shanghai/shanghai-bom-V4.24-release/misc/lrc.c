//lrc.c
//编制：邓建华
//时间：20000411


/********************************************************
函数：lrc_gen
功能：产生校验
*********************************************************/
void lrc_gen(UBYTE *buf,UWORD bytes)
{
UWORD i;

buf[bytes] = 0x33;
for(i=0;i<bytes;i++) buf[bytes] ^= buf[i];

return;
}

void lrc_gen2(UBYTE *buf,UWORD bytes)
{
UWORD i;

buf[bytes] = 0;
for(i=0;(UWORD)i<(UWORD)bytes;i++) buf[bytes] ^= buf[i];

return;
}


/********************************************************
函数：lrc_chk
功能：检查校验是否正确
*********************************************************/
UBYTE lrc_chk(UBYTE *buf,UWORD bytes)
{
UWORD i;
UBYTE lrc;

lrc = 0x33;
for(i=0;i<bytes;i++) lrc ^= buf[i];

if(lrc != buf[bytes]) return (UBYTE)-1;
return 0;
}

UBYTE lrc_chk2(UBYTE *buf,UWORD bytes)
{
UWORD i;
UBYTE lrc;

lrc = 0;
for(i=0;(UWORD)i<(UWORD)bytes;i++) lrc ^= buf[i];

if(lrc != buf[bytes]) return (UBYTE)-1;
return 0;
}


/*=============================================================================
函数：if_str_all_is
功能：比较是否一个串的n个字节是否同一数值
返回：
1：是
0：不是
===============================================================================*/
UBYTE if_str_all_is(UBYTE *str,UBYTE value,UWORD n)
{
UWORD i;

for(i=0;i<n;i++){  
  if((UBYTE)str[i] != (UBYTE)value) return 0; 
  } 

return 1;  
}  

/********************************************************
函数：lrc3_chk
功能：检查校验是否正确
*********************************************************/
UBYTE lrc3_chk(UBYTE *buf,UWORD bytes)
{
UWORD i;
UBYTE lrc;

lrc = 0x5a;
for(i=0;i<bytes;i++) lrc ^= buf[i];

if(lrc != buf[bytes]) return (UBYTE)-1;
return 0;
}

/********************************************************
函数：lrc3_gen
功能：产生校验
*********************************************************/
void lrc3_gen(UBYTE *buf,UWORD bytes)
{
UWORD i;

buf[bytes] = 0x5a;
for(i=0;i<bytes;i++) buf[bytes] ^= buf[i];

return;
}


/*************************************************************
检查钱包结构
**************************************************************/
UBYTE purse_chk(UBYTE *inbuf)
{
	UDWORD tlong;	

	if((UBYTE)inbuf[0] != (UBYTE)inbuf[8]) return 1; 
	if((UBYTE)inbuf[1] != (UBYTE)inbuf[9]) return 1; 
	if((UBYTE)inbuf[2] != (UBYTE)inbuf[10]) return 1; 
	if((UBYTE)inbuf[3] != (UBYTE)inbuf[11]) return 1; 
	if((UBYTE)inbuf[0] != (UBYTE)~inbuf[4]) return 1; 
	if((UBYTE)inbuf[1] != (UBYTE)~inbuf[5]) return 1; 
	if((UBYTE)inbuf[2] != (UBYTE)~inbuf[6]) return 1; 
	if((UBYTE)inbuf[3] != (UBYTE)~inbuf[7]) return 1; 

//	if((UBYTE)inbuf[12] != 0x00) return 1;
//	if((UBYTE)inbuf[13] != (UBYTE)0xff) return 1;
//	if((UBYTE)inbuf[14] != 0x00) return 1;
//	if((UBYTE)inbuf[15] != (UBYTE)0xff) return 1;
	if((UBYTE)inbuf[12]!=(UBYTE)inbuf[14])
	{
		return 1;
	}
	if((UBYTE)inbuf[13]!=(UBYTE)inbuf[15])
	{
		return 1;
	}
	if((UBYTE)inbuf[12]!=(UBYTE)(~inbuf[13]))
	{
		return 1;
	}


	*((char *)&tlong+LONG_HIGH0) = inbuf[0];
	*((char *)&tlong+LONG_HIGH1) = inbuf[1];
	*((char *)&tlong+LONG_HIGH2) = inbuf[2];
	*((char *)&tlong+LONG_HIGH3) = inbuf[3];


	//if((UDWORD)tlong < (UDWORD)0xf0000000) return 1;

	return 0;
}
