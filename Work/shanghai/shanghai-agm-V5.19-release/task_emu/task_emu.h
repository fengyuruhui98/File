//task_emu.h

#ifndef _TASK_EMU_H_
#define _TASK_EMU_H_
//start of file

extern UBYTE	bgSAMADF;								//
extern UBYTE bgTest24GFlag;						//


//2013/11/13 19:31:46


extern UBYTE bgTaskKeySubStep;
extern UBYTE gTaskBusyFlag;
#define task_key_sub_step_clr()  {bgTaskKeySubStep=0;}

extern UBYTE gAbortReaderFlag;
extern UBYTE bpgChk24GModuleFlag;

//#define set_task_key(step) {disable_t4();bgTaskKeyStep=step;bgTaskKeySubStep=0;enable_t4();}

//
void task_com(void);
void task_key(void);
void task_key_buzzle(void);
void set_task_key(UBYTE step);


//end of file
#endif

