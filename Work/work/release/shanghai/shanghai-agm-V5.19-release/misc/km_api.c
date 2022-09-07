#ifndef _KM_BIN_API_C_
#define _KM_BIN_API_C_
//start of file

//#define DEBUG_PRINTF

/*
function:
parameter:
	*dirname:directory name
	*postfix:the filename may a part of filename
	*strRtn:the full filename
return:
	2:can't open the directory
	1:can't find the filename in the direcotry
	0:ok,find file name and 
*/
int FileisExist(char *dirname, char *postfix, char *strRtn)
{
int           rtn;
struct dirent *dirp;
DIR           *dp;
  
  	rtn = 1;
  	/* -------- 打开错误数据目录 ---------- */
  	//printf("dir the file exist %s, fix is %s\n", dirname, postfix);
	if((dp = opendir(dirname)) == NULL)
	{
		//printf("open dir failure\n");
		return 2;
	}
	/* -------- 读取错误数据目录 ---------- */
	while((dirp = readdir(dp)) != NULL)
	{
		if(strcmp(dirp->d_name,".") && strcmp(dirp->d_name,"..") )
		{
			if(postfix != NULL)
			{
				if(strstr(dirp->d_name, postfix) != NULL)
				{
					rtn = 0;
					strcpy(strRtn, dirp->d_name);
				}
				else
					continue;
			}
			else
			{
				rtn = 0;
				strcpy(strRtn, dirp->d_name);
			}
			closedir(dp);
			return rtn;
		}
	}
	/* -------- 关闭错误数据目录 ---------- */
	closedir(dp);
  	
  	return rtn;
}

/*
function:
parameter:
	*from_charset:from char set
	*to_charset:to char set
	*strRtn:input buffer from source
	*inlen: intput buffer length
	*outbuf:output buffer to destionation
	*outlen: max output buffer length
return:
	-1:can't open or malloc memory failure or convert failure
	0:ok,conver from source char set to destionation char set
*/
int code_convert(char *from_charset, char * to_charset, char * inbuf, int inlen, char *outbuf, int outlen)
{
iconv_t cd;
char *to_inbuf = NULL;
int i , len_in, len_out;

	cd = iconv_open(to_charset, from_charset);
	if(cd == 0)
		return -1;
	memset(outbuf, 0x00, outlen);
	to_inbuf = (char *)malloc(sizeof(short) * inlen);
	if(to_inbuf == NULL)
	{
		iconv_close(cd);
		return -1;
	}
	memcpy(to_inbuf, inbuf, inlen * 2);
	if(-1 == iconv(cd, &to_inbuf, &inlen, &outbuf, &outlen))
	{
		printf("\n%s\n", strerror(errno));
		iconv_close(cd);
		return -1;
	}

	iconv_close(cd);
	return 0;
}

/*
function:
parameter:
return:
*/
unsigned char *ShortToByte(short sh_in, unsigned char *out_buf)
{
union {
	short sh_union;
	unsigned char ch_union[2];
}sh_chUnion;

	sh_chUnion.sh_union = sh_in;
	out_buf[0] = sh_chUnion.ch_union[1];
	out_buf[1] = sh_chUnion.ch_union[0];

	return out_buf;
}

unsigned char *LongToByte(long lng_in, unsigned char *out_buf)
{
union {
	long lng_union;
	unsigned char ch_union[4];
}lng_chUnion;

	lng_chUnion.lng_union = lng_in;
	out_buf[0] = lng_chUnion.ch_union[3];
	out_buf[1] = lng_chUnion.ch_union[2];
	out_buf[2] = lng_chUnion.ch_union[1];
	out_buf[3] = lng_chUnion.ch_union[0];

	return out_buf;
}

/*
function:change the two bytes of big endian to short 
parameter:
return:the little endian short
*/
short ByteToShort(short *sh_in, unsigned char *out_buf)
{
union {
	short sh_union;
	unsigned char ch_union[2];
}sh_chUnion;

	sh_chUnion.ch_union[1] = out_buf[0];
	sh_chUnion.ch_union[0] = out_buf[1];
	*sh_in = sh_chUnion.sh_union;

	return *sh_in;
}

long ByteToLong(long *lng_in, unsigned char *out_buf)
{
union {
	long lng_union;
	unsigned char ch_union[4];
}lng_chUnion;

	lng_chUnion.ch_union[3] = out_buf[0];
	lng_chUnion.ch_union[2] = out_buf[1];
	lng_chUnion.ch_union[1] = out_buf[2];
	lng_chUnion.ch_union[0] = out_buf[3];

	*lng_in = lng_chUnion.lng_union;
	return *lng_in;
}
/*
function:realize the round function
*/
long sz_round(long x)
{
long lngintegral;
float y;

	y = x / 100.0;
	lngintegral = (long)y;
	y -= (float)lngintegral;
	y += 0.5;
	lngintegral += (long)y;
	
	lngintegral *= 100;
	return lngintegral;
}

/********************************************************
函 数 名：HexToBcd
函数功能：将3字节HEX交易流水号转换为8字节BCD，不足前补0
参    数：
					输入：in_buf 3字节HEC交易流水号
					输出：8字节BCD
*********************************************************/
unsigned char HexToBcd(unsigned char *in_buf, unsigned char *out_bcd)
{
	int i;
	long bcd;
	char buf[20];

	bcd = in_buf[0] * 256 * 256 + in_buf[1] * 256 + in_buf[2];
#ifdef DEBUG_PRINTF
	printf("\nin_buf :%02x %02x %02x\n",in_buf[0],in_buf[1],in_buf[2]);
	printf("bcd :%016d\n",bcd);
#endif
	
	sprintf(buf,"%016d",bcd);

#ifdef DEBUG_PRINTF
	printf("buf :\n");
	for (i = 0; i < 16; i++)
	{
		printf(" %02x", buf[i]);

	}
#endif
	
	for (i = 0; i<16; i = i + 2)
	{
		out_bcd[i / 2] = (buf[i] - 0x30) * 16 + (buf[i + 1] - 0x30);
	}

#ifdef DEBUG_PRINTF
	printf("\nout_buf :\n");
	for (i = 0; i < 8; i++)
	{
		printf(" %02x", out_bcd[i]);

	}
#endif
	//return *out_bcd;
	return 0;
}

/******************************************************************
Prototype :
Function  :
Parameters:
Returns   :
******************************************************************/
int	str2nib(unsigned char *strdata, unsigned char *hexdata)
{ 
int conv_len, cnt;
unsigned char ch, ch1;
	
	conv_len = strlen((const char *)strdata);
	if ( conv_len & 0x01 )
		ch1 = 0;
	else
		ch1 = 0x55;

	for (cnt = 0; cnt < conv_len; strdata++, cnt++)
	{
		if (*strdata >= 'a')
			ch = *strdata - 'a' + 10 ;
		else if (*strdata >= 'A')
			ch = *strdata- 'A' + 10 ;
		else if (*strdata >= '0')
			ch = *strdata-'0' ;
		else
			ch = 0;
		if ( ch1 == 0x55 )
			ch1 = ch ;
		else {
			*hexdata++ = (ch1 << 4) | ch ;
			ch1 = 0x55 ;
		}
	}
	if ( ch1 != 0x55 )     /*掳脩脳卯潞贸掳毛啪枚脳脰艙脷艗脫脠毛*/
		*hexdata = ch1 << 4 ;

	return (conv_len>>1);

 
}

int	nib2str(unsigned char *hexdata, unsigned char *strdata, int length)
{
int cnt = 0;
	
	length = length << 1;
	for (; cnt < length; cnt++, strdata++) 
	{
		*strdata = ((cnt & 0x01) ? (*hexdata++ & 0x0f) : (*hexdata >> 4));
		*strdata += ((*strdata > 9) ? ('A' - 10) : '0' );
	}
	return (cnt << 1);
}

//end of file
#endif
	