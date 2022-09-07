//init.h

#ifndef _INIT_H_
#define _INIT_H_
//start of file

extern UBYTE bgResetFlag;
extern UBYTE bgResetWrTimeFlag;
extern UWORD wgAbortCmdCnt;

#define ABORT_CMD_CNT      2

#define set_auto_abort_flag()    {wgAbortCmdCnt=1;}
#define abort_cmd_cnt_inc()      if(wgAbortCmdCnt){wgAbortCmdCnt++;} 
#define clr_auto_abort_flag()    {wgAbortCmdCnt=0;}
#define if_need_auto_abort()     (wgAbortCmdCnt>ABORT_CMD_CNT)        

//º¯Êý
void init_all(void);
void find_ee_ptr1(void);
void find_ee_ptr2(void);
void auto_recover(void);
void auto_abort(void);

//
//end of file
#endif

