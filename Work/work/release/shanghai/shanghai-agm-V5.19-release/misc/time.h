//time.h
//时间：20080205
//编制：邓建华

#ifndef _TIME_H_
#define _TIME_H_
//start of file


#define TIMER_INDEX_DISCARD0      10
#define TIMER_INDEX_DISCARD1      11
#define TIMER_INDEX_DISCARD2      12
#define TIMER_CMD_PROCESS_INDEX   13           //命令执行时钟      
#define TIMER_SAM_SEND_INDEX      14            //SAM模组协议发送定时器 
#define TIMER_SAM_RECE_INDEX      15            //SAM模组协议接收定时器
#define TIMER_MS_POWEROFF			    16							//读卡关射频定时器
#define TIMER_MS_INDEX0			    	17							//毫秒定时器0  
#define TIMER_MS_INDEX1		    		18							//毫秒定时器1  
#define TIMER_MS_INDEX2			    	19							//毫秒定时器2  
#define TIMER_MS_INDEX3           20						  //毫秒定时器3
#define TIMER_INDEX_ZTE           21           
#define TIMER_500MS_INDEX         22            //500ms定时器
#define TIMER_INDEX_UART0_SEND    23 
#define TIMER_INDEX_UART0_RECE    24 
#define TIMER_INDEX_UART1_SEND    25 
#define TIMER_INDEX_UART1_RECE    26 
#define TIMER_INDEX_UART2_SEND    27 
#define TIMER_INDEX_UART2_RECE    28 
#define TIMER_INDEX_UART0         29 
#define TIMER_INDEX_UART1         30 
#define TIMER_INDEX_UART2         31 
#define TIMER_RESET               32          //系统复位时钟

#define MAX_TIMER_INDEX           128

//变量---------------------------------------------------------------------
extern UDWORD dwpgSoftTimerCnt[MAX_TIMER_INDEX],dwpgSoftTimerLimit[MAX_TIMER_INDEX];
extern UBYTE  bpgSoftTimerFlag[MAX_TIMER_INDEX];
extern UDWORD dwgSystemTimeCnt;

extern UBYTE bgTaskComEn;
extern UBYTE bgSamProtEn;
extern UBYTE bgSoftTimerEn;

#define task_com_disable()    {bgTaskComEn=0;}
#define task_com_enable()     {bgTaskComEn=1;}
#define sam_prot_disable()   {bgSamProtEn=0;}
#define sam_prot_enable()    {bgSamProtEn=1;}
#define soft_timer_disable() {bgSoftTimerEn=0;}
#define soft_timer_enable()  {bgSoftTimerEn=1;}



//函数定义
void timer_init(void);
void delay_us(UWORD delay_us);
void delay_ms(UWORD cnt);
UDWORD get_sys_time_cnt(void);
void delay_tick(void);

//end of file
#endif

