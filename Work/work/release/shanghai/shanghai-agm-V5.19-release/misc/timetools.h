//timetools.h

#ifndef _TIMETOOLS_H_
#define _TIMETOOLS_H_
//start of file

//#define  TIME2000   0x386d89d0L		//ԭ����ʱ��
//#define  TIME2000   0x386CD300L			//GMT1970-1-1 0:0:0
#define  TIME2000   0x386d4380L		//����ʱ��1970-1-1 0��0��0
#define  DAY2000	10957			//from 1970-1-1
#define  WEEK1970	  4


//����
UWORD datestr2days(UBYTE *date);
void days2datestr(UWORD indays,UBYTE *out_datestr);
void  timestr4to6(UBYTE *timestr4,UBYTE *timestr6);
void  timestr6to4(UBYTE *timestr6,UBYTE *timestr4);
UWORD  time_chk_valid(UBYTE *timestr6);
UWORD  date_chk_valid(UBYTE *datestr4);
void timestr4todate(UBYTE *timestr4);
UDWORD timestr2long(UBYTE *buf);
void long2timestr(UDWORD longt,UBYTE *buf);
void get_month_end_date(UBYTE *now_date,UBYTE *month_end_date);

char bcd2bin(char inbyte);
char bin2bcd(char inbyte);

void set_timeout(long delay_time);
char DaysToWeek(unsigned short shDays);


//end of file
#endif

