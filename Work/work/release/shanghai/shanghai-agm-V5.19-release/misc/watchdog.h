//watchdog.h

#ifndef _WATCHDOG_H_
#define _WATCHDOG_H_
//start of file

//Ԥ����
#define WATCHDOG_CNT_VALUE  (UDWORD)((float)1600000*PCLK/128.0/16.0+0.5)  //1.6s

//����
void watchdog_init(void);
void watchdog(void);

//end of file
#endif




