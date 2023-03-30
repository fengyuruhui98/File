//trans_245g.h

#ifndef _TRANS_245G_H_
#define _TRANS_245G_H_
//start of file

//公共应用基本文件（0015）
typedef struct _G245_EF15_ {
 UBYTE issuer_id[8];      //1－8	发卡方标识	8
 UBYTE app_id;            //9	应用类型标识	1
 UBYTE app_ver;           //10	应用版本	1
 UBYTE app_snr[10];       //11－20	应用序列号	10
 UBYTE app_start_date[4]; //21－24	应用启动日期	4 - YYYYMMDD
 UBYTE app_end_date[4];   //25－28	应用有效日期	4 – YYYYMMDD
 UBYTE card_type;         //29	卡类型（BCD编码）	1
 UBYTE area_code;         //30	省编码（BCD编码）	1
 } G245_EF15;

//
extern UBYTE bgCheck24GStep;

#define g245_is_not_local()  (CurrentLocation!=LocationID)
#define zte_set_wait(ms)      {timer_set(gZteProt0.send_timer_index,ms);bgCheck24GStep = 0xff;}  


//函数
void check_245g(void);


//end of file
#endif





