//trans_245g.h

#ifndef _TRANS_245G_H_
#define _TRANS_245G_H_
//start of file

//����Ӧ�û����ļ���0015��
typedef struct _G245_EF15_ {
 UBYTE issuer_id[8];      //1��8	��������ʶ	8
 UBYTE app_id;            //9	Ӧ�����ͱ�ʶ	1
 UBYTE app_ver;           //10	Ӧ�ð汾	1
 UBYTE app_snr[10];       //11��20	Ӧ�����к�	10
 UBYTE app_start_date[4]; //21��24	Ӧ����������	4 - YYYYMMDD
 UBYTE app_end_date[4];   //25��28	Ӧ����Ч����	4 �C YYYYMMDD
 UBYTE card_type;         //29	�����ͣ�BCD���룩	1
 UBYTE area_code;         //30	ʡ���루BCD���룩	1
 } G245_EF15;

//
extern UBYTE bgCheck24GStep;

#define g245_is_not_local()  (CurrentLocation!=LocationID)
#define zte_set_wait(ms)      {timer_set(gZteProt0.send_timer_index,ms);bgCheck24GStep = 0xff;}  


//����
void check_245g(void);


//end of file
#endif





