//lrc.c
//���ƣ��˽���
//ʱ�䣺20000411


/********************************************************
������lrc_gen
���ܣ�����У��
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
//printf("lrc_gen2_0 :buf = %02x\n",buf[bytes]);
return;
}


/********************************************************
������lrc_chk
���ܣ����У���Ƿ���ȷ
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
������if_str_all_is
���ܣ��Ƚ��Ƿ�һ������n���ֽ��Ƿ�ͬһ��ֵ
���أ�
1����
0������
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
������lrc3_chk
���ܣ����У���Ƿ���ȷ
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
������lrc3_gen
���ܣ�����У��
*********************************************************/
void lrc3_gen(UBYTE *buf,UWORD bytes)
{
UWORD i;

buf[bytes] = 0x5a;
for(i=0;i<bytes;i++) buf[bytes] ^= buf[i];

return;
}


/*************************************************************
���Ǯ���ṹ
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
