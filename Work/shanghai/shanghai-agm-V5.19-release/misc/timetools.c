//timetools.c

#ifndef _TIMETOOLS_C
#define _TIMETOOLS_C

//��ݱ�
UWORD code gYearTable[100] = {
    0,  366,  731, 1096, 1461, 1827, 2192, 2557, 2922, 3288,
 3653, 4018, 4383, 4749, 5114, 5479, 5844, 6210, 6575, 6940,
 7305, 7671, 8036, 8401, 8766, 9132, 9497, 9862,10227,10593,
10958,11323,11688,12054,12419,12784,13149,13515,13880,14245,
14610,14976,15341,15706,16071,16437,16802,17167,17532,17898,
18263,18628,18993,19359,19724,20089,20454,20820,21185,21550,
21915,22281,22646,23011,23376,23742,24107,24472,24837,25203,
25568,25933,26298,26664,27029,27394,27759,28125,28490,28855,
29220,29586,29951,30316,30681,31047,31412,31777,32142,32508,
32873,33238,33603,33969,34334,34699,35064,35430,35795,36160
};

//������
UWORD code gMonthTable1[12] = {
0,31,59,90,120,151,181,212,243,273,304,334
};
//����
UWORD code gMonthTable2[12] = {
0,31,60,91,121,152,182,213,244,274,305,335
};



/*=====================================================================================
������bcd2bin
���ܣ�
=======================================================================================*/
char bcd2bin(char inbyte)
{
return (((UBYTE)inbyte/0x10)*10+(UBYTE)inbyte%0x10);	
}	


/*=====================================================================================
������bin2bcd
���ܣ�
=======================================================================================*/
char bin2bcd(char inbyte)
{
inbyte = (UBYTE)inbyte%100;	
return (((UBYTE)inbyte/10)*16+(UBYTE)inbyte%10);	
}	

/*==============================================================
����:datestr2days
���ܣ������ڴ�ת������2000��ʼ������
buf:  CCYYMMDD
=================================================================*/
UWORD  datestr2days(UBYTE *date)
{
UWORD tint;
UBYTE ch;

//��
ch = bcd2bin(date[1]);
tint = gYearTable[ch];
//��
if(ch%4 == 0){                               //����
   ch = bcd2bin(date[2]);
   tint += gMonthTable2[ch-1];
   }
else{                                        //������
   ch = bcd2bin(date[2]);
   tint += gMonthTable1[ch-1];
   }
//��  
ch = bcd2bin(date[3]);
tint += (UWORD)(ch-1);

return tint;
}


/*==============================================================
����:days2datestr
���ܣ�
=================================================================*/
void  days2datestr(UWORD indays,UBYTE *out_datestr)
{
UBYTE i;

out_datestr[0] = 0x20;

//��
for(i=0;i<100;i++){
  if(indays < gYearTable[i+1]) break;
  }
out_datestr[1] = bin2bcd(i);  //��
indays -= (UWORD)gYearTable[i];

//����
if((i%4) == 0){  //����
  for(i=0;i<11;i++){
     if(indays < gMonthTable2[i+1]) break;
     }
  //��
  out_datestr[2] = bin2bcd(i+1);
  //��
  indays -= gMonthTable2[i];
  out_datestr[3] = bin2bcd(indays+1);
  }
else{             //������
  for(i=0;i<11;i++){
     if(indays < gMonthTable1[i+1]) break;
     }
  //��
  out_datestr[2] = bin2bcd(i+1);
  //��
  indays -= gMonthTable1[i];
  out_datestr[3] = bin2bcd(indays+1);
  }

return;
}  


/*==============================================================
����:timestr4to6
���ܣ�
=================================================================*/
void  timestr4to6(UBYTE *timestr4,UBYTE *timestr6)
{
UDWORD tlong;
UBYTE ch;

*((char *)&tlong+LONG_HIGH3) = timestr4[0];
*((char *)&tlong+LONG_HIGH2) = timestr4[1];
*((char *)&tlong+LONG_HIGH1) = timestr4[2];
*((char *)&tlong+LONG_HIGH0) = timestr4[3];

//��
ch = *((char *)&tlong+LONG_HIGH0);
ch = ch&0x3f;
timestr6[5] = bin2bcd(ch);
tlong = tlong >> 6;
//ʱ
ch = *((char *)&tlong+LONG_HIGH0);
ch = ch&0x1f;
timestr6[4] = bin2bcd(ch);
tlong = tlong >> 5;
//��
ch = *((char *)&tlong+LONG_HIGH0);
ch = ch&0x1f;
timestr6[3] = bin2bcd(ch);
tlong = tlong >> 5;
//�� 
ch = *((char *)&tlong+LONG_HIGH0);
ch = ch&0x0f;
timestr6[2] = bin2bcd(ch);
tlong = tlong >> 4;
//��
ch = *((char *)&tlong+LONG_HIGH0);
ch = ch&0xff;
timestr6[1] = bin2bcd(ch);

timestr6[0] = 0x20;

return;
}  

/*==============================================================
����:timestr6to4
���ܣ�CCYYMMDDHHMM -> hexѹ����
=================================================================*/
void  timestr6to4(UBYTE *timestr6,UBYTE *timestr4)
{
UDWORD tlong;

tlong = (UBYTE)bcd2bin(timestr6[1]);
tlong = tlong << 4;
tlong += (UBYTE)bcd2bin(timestr6[2]);
tlong = tlong << 5;
tlong += (UBYTE)bcd2bin(timestr6[3]);
tlong = tlong << 5;
tlong += (UBYTE)bcd2bin(timestr6[4]);
tlong = tlong << 6;
tlong += (UBYTE)bcd2bin(timestr6[5]);

timestr4[0] = *((char *)&tlong+LONG_HIGH3);
timestr4[1] = *((char *)&tlong+LONG_HIGH2);
timestr4[2] = *((char *)&tlong+LONG_HIGH1);
timestr4[3] = *((char *)&tlong+LONG_HIGH0);

return;
}


/*==============================================================
����:
����:
=================================================================*/
void timestr4todate(UBYTE *timestr4)
{
UBYTE buf[6];
timestr4to6(timestr4,buf);
memcpy((char *)timestr4,(char *)buf,4);
return;
}

/*==============================================================
����:timestr6to4
����:CCYYMMDDHHMM
=================================================================*/
UWORD  time_chk_valid(UBYTE *timestr6)
{
UBYTE i,high,low;
UBYTE buf[7];
UBYTE code month[12]={31,29,31,30,31,30,31,31,30,31,30,31};

//BCD���
for(i=0;i<6;i++){
  high = timestr6[i]/16;
  if(high > 9) goto label_err;
  low = timestr6[i]%16;
  if(low > 9) goto label_err;
  buf[i] = high*10+low;
  }

//CC
if(buf[0] != 20) goto label_err;
//MM
if((buf[2] > 12) ||(buf[2] == 0)) goto label_err;
if((buf[2] == 2) && (buf[3] == 29)){
  if(buf[1]%4 != 0) goto label_err;  //������
  }
//DD
if(buf[3] > month[buf[2]-1]) goto label_err;
if(buf[3] == 0) goto label_err;
//HH
if(buf[4] > 23) goto label_err;
//MM
if(buf[5] > 59) goto label_err;

return 1;

//�Ƿ�
label_err:
return 0;  
}  


/*==============================================================
����:timestr6to4
���ܣ�
=================================================================*/
UWORD  date_chk_valid(UBYTE *datestr4)
{
UBYTE i,high,low;
UBYTE buf[5];
UBYTE code month[12]={31,29,31,30,31,30,31,31,30,31,30,31};

//BCD���
for(i=0;i<4;i++){
  high = datestr4[i]/16;
  if(high > 9) goto label_err;
  low = datestr4[i]%16;
  if(low > 9) goto label_err;
  buf[i] = high*10+low;
  }

//CC
if(buf[0] != 20) goto label_err;
//MM
if((buf[2] > 12) ||(buf[2] == 0)) goto label_err;
if((buf[2] == 2) && (buf[3] == 29)){
  if(buf[1]%4 != 0) goto label_err;  //������
  }
//DD
if(buf[3] > month[buf[2]-1]) goto label_err;
if(buf[3] == 0) goto label_err;

return 1;

//�Ƿ�
label_err:
return 0;  
}

/*==============================================================
����:timestr2long
���ܣ���ʱ�䴮ת������2000��ʼ������
buf:  year(1bcd),month,date,hour,minute,second
=================================================================*/
UDWORD timestr2long(UBYTE *buf)
{
UDWORD tlong;
UBYTE ch,ch1;

ch = bcd2bin(buf[0]);
tlong = (UDWORD)((UWORD)gYearTable[ch]);
ch1 = bcd2bin(buf[1]);
if(ch%4 == 0)
   tlong += (UDWORD)((UWORD)gMonthTable2[ch1-1]);
else
   tlong += (UDWORD)((UWORD)gMonthTable1[ch1-1]);
ch = bcd2bin(buf[2]);
tlong += (UDWORD)((UBYTE)(ch-1));
tlong *= 24;
ch = bcd2bin(buf[3]);
tlong += (UDWORD)((UBYTE)ch);
tlong *= 60;
ch = bcd2bin(buf[4]);
tlong += (UDWORD)((UBYTE)ch);
tlong *= 60;
ch = bcd2bin(buf[5]);
tlong += (UDWORD)((UBYTE)ch);

return tlong;
}

/*====================================================================
����:long2timestr
���ܣ�����2000��ʼ������ת����ʱ�䴮
buf:  year(1bcd),month,date,hour,minute,second
======================================================================*/
void long2timestr(UDWORD longt,UBYTE *buf)
{
UDWORD seconds;
UWORD  tint,days;
UBYTE i;

	tint = (UWORD)((UDWORD)longt/(UDWORD)86400L);

	for(i=0;i<100;i++)
	{
		if(tint < gYearTable[i+1]) break;
	}
	buf[0] = bin2bcd(i);  //��

	days = (UWORD)tint - (UWORD)gYearTable[i];

seconds = longt%86400L;

if(i%4 == 0){  //����
  for(i=0;i<12;i++){
     if(days < gMonthTable2[i+1]) break;
     else if(i==11) break;
     }
  //��
  buf[1] = bin2bcd(i+1);
  //��
  i = days-gMonthTable2[i];
  buf[2] = bin2bcd(i+1);
  }
else{             //������
  for(i=0;i<12;i++){
     if(days < gMonthTable1[i+1]) break;
     else if(i==11) break;
     }
  //��
  buf[1] = bin2bcd(i+1);
  //��
  i = days-gMonthTable1[i];
  buf[2] = bin2bcd(i+1);
  }
//ʱ
i = (UBYTE)((UDWORD)seconds/(UDWORD)3600L);
buf[3] = bin2bcd(i);
//��
tint = (UWORD)((UDWORD)seconds%(UDWORD)3600L);
i = (UBYTE)((UWORD)tint/(UWORD)60);
buf[4] = bin2bcd(i);
//��
i = (UBYTE)((UWORD)tint%(UWORD)60);
buf[5] = bin2bcd(i);

return;
}


/*====================================================================
����:get_month_end_date
���ܣ�
======================================================================*/
void get_month_end_date(UBYTE *now_date,UBYTE *month_end_date)
{
UBYTE year,month,day;	
#ifdef _EMU_WIN_
AnsiString str;
int i;
#endif

memcpy((char *)month_end_date,(char *)now_date,3);
year = bcd2bin(now_date[1]);
month = bcd2bin(now_date[2]);
if(month==2){
  if((year%4)==0) day=0x29;
  else day=0x28;
  goto label_end;		 
  }
switch(month){
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
			 day = 0x31;
			 break;
  default:
  	   day = 0x30;
  	   break;
  }


label_end:
month_end_date[3] = day;
#ifdef _EMU_WIN_
str.printf("get_month_end_date:date=");
for(i=0;i<4;i++) str.cat_printf("%02X",(UBYTE)now_date[i]);
str.cat_printf("\nend date:");
for(i=0;i<4;i++) str.cat_printf("%02X",(UBYTE)month_end_date[i]);
debug_show(str,clPurple);
#endif
return;
}


/*====================================================================
����:time_wr_diff
���ܣ�
======================================================================*/
#ifdef _EMU_WIN_
void time_wr_diff(int diff)
{
TIniFile *ini_file;
AnsiString file_name,str;

file_name = ExtractFilePath(Application->ExeName)+"skj100.ini";

ini_file = new TIniFile(file_name);

str.printf("%d",diff);
ini_file->WriteString("ʱ���У","��ֵ",str);

delete ini_file;
return;  
}  
#endif

/*====================================================================
����:time_rd_diff
���ܣ�
======================================================================*/
#ifdef _EMU_WIN_
int  time_rd_diff(void)
{
TIniFile *ini_file;
AnsiString file_name,str;
int diff;

file_name = ExtractFilePath(Application->ExeName)+"skj100.ini";

if(!FileExists(file_name)){
 return 0;
 }

ini_file = new TIniFile(file_name);

str = ini_file->ReadString("ʱ���У","��ֵ",0);
sscanf(str.c_str(),"%d",&diff);

delete ini_file;
return diff;  
}
#endif

void set_timeout(long delay_time)
{
struct timeval timeout;
fd_set readfd;

	FD_ZERO(&readfd);
	FD_SET(0, &readfd);
	timeout.tv_sec = 0;
	//timeout.tv_usec = delay_time;
	//2013/11/16 16:10:15
	timeout.tv_usec = ( delay_time/2 );
	select(0, &readfd, NULL, NULL, &timeout);
}

/*
function:
return:
	0:sunday 1~6:mon-sat
*/
char DaysToWeek(unsigned short shDays)
{
	return ((shDays % 7) + WEEK1970) % 7;
}


  
//end of file
#endif
