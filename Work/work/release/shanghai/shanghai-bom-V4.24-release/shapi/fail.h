//fail.h

#ifndef _FAIL_H
#define _FAIL_H

#define ADD_CLR_FLAG  0x66
#define SALE_CLR_FLAG 0x77 
extern uchar  gFailUpdataFlag;
extern  uchar gFailUpdataCardSnr[4];
extern  ulong gFailUpdataB6;
extern uint  gFailUpdataCardCnt;
extern  uchar gFailUpdataRecPtr;
extern  uchar gFailUpdataMetroStatue;
void fail_init(void);
void fail_b1_add(void);
void fail_trans_add(void);
void fail_add_add(void);
void fail_b1_clr(void);
void fail_trans_clr(void);
void fail_add_clr(uchar flag);
//void fail_unadd_clr(void);
uchar fail_b1_chk(void);
uchar fail_add_chk(void);
//uchar fail_unadd_chk(void);
//uchar fail_add_unsale_chk(void);
uchar fail_trans_chk(void);
char fail_b1_restore(void);
char fail_add_restore(void);
//char fail_unadd_restore(void);
//char fail_add_unsale_restore(void);
char fail_trans_restore(void);


#endif