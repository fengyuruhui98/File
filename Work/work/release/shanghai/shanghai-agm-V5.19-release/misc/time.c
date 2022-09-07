//time.c
//时间操作
//时间：20080205
//编制：邓建华

#ifndef _TIME_C_
#define _TIME_C_
//start of file

//变量---------------------------------------------------------------------
UDWORD dwpgSoftTimerCnt[MAX_TIMER_INDEX],dwpgSoftTimerLimit[MAX_TIMER_INDEX];
UBYTE  bpgSoftTimerFlag[MAX_TIMER_INDEX];
UDWORD dwgSystemTimeCnt = 0;

UBYTE bgTaskComEn;
UBYTE bgSamProtEn;
UBYTE bgSoftTimerEn;

/*=============================================================================
函数：timer_init
功能：
===============================================================================*/
void timer_init(void)
{
UDWORD i;	
//

for(i=0;i<MAX_TIMER_INDEX;i++){
  dwpgSoftTimerCnt[i]=0;
  dwpgSoftTimerLimit[i] = 0;
  bpgSoftTimerFlag[i] = 0;
  }

//
return;	
}	

/*=============================================================================
函数：delay_us
功能：利用Timer4的计数来确定延时
要求：由于Timer4定时间隔为1000us，因此本函数要求delay_us在1000us以内
说明：计数器count_down,调试中发现连续的读TCNTO4可能导致T4停止
修改为直接软件延时
===============================================================================*/
void delay_us(UWORD delay_us)
{
//实际--------------------------------------------------------------------------	
#ifndef _EMU_WIN_	
usleep(delay_us);
return;	
#endif
//仿真---------------------------------------------------------------------------
#ifdef _EMU_WIN_
delay_us = delay_us;
return;
#endif
}	

/*=============================================================================
函数：delay_ms
功能：
===============================================================================*/
void delay_ms(UWORD cnt)
{
//实际--------------------------------------------------------------------------	
#ifndef _EMU_WIN_	
UWORD i;

for(i=0;i<cnt;i++){
  delay_us(500);	
  delay_us(500);	
  }

return;	
#endif

//仿真--------------------------------------------------------------------------	
#ifdef _EMU_WIN_	
Sleep(cnt);
return;
#endif
}	

/*=============================================================================
函数：delay_ms
功能：
===============================================================================*/
UDWORD get_sys_time_cnt(void)
{
  return timer_get_ms();
}	


/*=============================================================================
函数：
功能：
===============================================================================*/
void delay_tick(void)
{
return;  
}	

//end of file
#endif