//interrupt.h

#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_
//start of file


//预定义------------------------------------------------------------------
typedef void irq_func(void);    
#define MAX_TIMER_INDEX    32

//变量---------------------------------------------------------------------
extern irq_func *irq_func_index[32];
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

//#define disable_t4()   {S3C2410SFR_INTMSK = S3C2410SFR_INTMSK|INT_TIMER4_MASK;}
//#define enable_t4()   {S3C2410SFR_INTMSK = S3C2410SFR_INTMSK&(UDWORD)(~INT_TIMER4_MASK);}

//函数----------------------------------------------------------------------
#ifndef _EMU_WIN_
void __irq IRQ_Handler(void);
#endif

void C_IRQHandler(void);
void none_interrupt(void);  
void interrupt_init(void);
void uart0_interrupt(void);  
void uart1_interrupt(void);  
void uart2_interrupt(void); 
void spi1_interrupt(void); 
void timer4_interrupt(void);
void halt_system(void);




//end of file
#endif


