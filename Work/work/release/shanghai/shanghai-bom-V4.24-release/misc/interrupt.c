//interrupt.c

#ifndef _INTERRUPT_C_
#define _INTERRUPT_C_
//start of file

irq_func *irq_func_index[32];
UDWORD dwpgSoftTimerCnt[MAX_TIMER_INDEX],dwpgSoftTimerLimit[MAX_TIMER_INDEX];
UBYTE  bpgSoftTimerFlag[MAX_TIMER_INDEX];
UBYTE  bgOsTimeTick;
UDWORD dwgSystemTimeCnt;

//
UBYTE bgTaskComEn;
UBYTE bgSamProtEn;
UBYTE bgSoftTimerEn;

/*======================================================================
函数：
功能：
========================================================================*/
void none_interrupt(void)
{
S3C2410SFR_SRCPND = 0xffffffffL;	
return;	
}	


/*======================================================================
函数：
功能：
========================================================================*/
void __irq IRQ_Handler(void)   
{
irq_func_index[S3C2410SFR_INTOFFSET]();
S3C2410SFR_INTPND = S3C2410SFR_INTPND;	
return;	
}	

/*======================================================================
函数：
功能：
========================================================================*/
void C_IRQHandler(void)
{
irq_func_index[S3C2410SFR_INTOFFSET]();
S3C2410SFR_INTPND = S3C2410SFR_INTPND;
return;
}

/*======================================================================
函数：
功能：
========================================================================*/
void interrupt_init(void)
{
UBYTE i;

S3C2410SFR_INTMOD    = 0;
S3C2410SFR_INTMSK    = 0xffffffffL;
S3C2410SFR_INTSUBMSK = 0xffffffffL;
S3C2410SFR_PRIORITY  = 0;  //优先级不反转
bgOsTimeTick = 0;

for(i=0;i<32;i++) irq_func_index[i] = none_interrupt;	
irq_func_index[INT_UART0_SRC]  = uart0_interrupt;
irq_func_index[INT_UART1_SRC]  = uart1_interrupt;
irq_func_index[INT_UART2_SRC]  = uart2_interrupt;
irq_func_index[INT_TIMER4_SRC] = timer4_interrupt;
irq_func_index[INT_SPI1_SRC]   = spi1_interrupt;

pISR_UNDEF  = (unsigned)halt_system;
pISR_SWI    = (unsigned)halt_system;
pISR_PABORT = (unsigned)halt_system;
pISR_DABORT = (unsigned)halt_system;

pISR_IRQ = (unsigned)IRQ_Handler;	
//pISR_IRQ = (unsigned)UCOS_IRQHandler;	

dwgSystemTimeCnt = 0;


return;
}	


/*======================================================================
函数：
功能：
========================================================================*/
void uart0_interrupt(void)  
{
UBYTE i,cnt;
UBYTE uerstat,urxh;	

//rxd----------------------------------------------------------------------------
if(S3C2410SFR_SUBSRCPND&BIT_SUB_RXD0_MASK){
  cnt = S3C2410SFR_UFSTAT0&0x0000000f;
  for(i=0;i<cnt;i++){
    gUart0ReceCbuf.buf[gUart0ReceCbuf.start_ptr] = S3C2410SFR_URXH0;
    gUart0ReceCbuf.start_ptr = (gUart0ReceCbuf.start_ptr+1)%gUart0ReceCbuf.buf_size;
    if(gUart0ReceCbuf.end_ptr == gUart0ReceCbuf.start_ptr){
       gUart0ReceCbuf.end_ptr = (gUart0ReceCbuf.end_ptr+1)%gUart0ReceCbuf.buf_size;
       }	 
    }
  //  	
  S3C2410SFR_SUBSRCPND = BIT_SUB_RXD0_MASK;	
  }	
//txd----------------------------------------------------------------------------
if(S3C2410SFR_SUBSRCPND&BIT_SUB_TXD0_MASK){
  cnt = (UBYTE)((UDWORD)(S3C2410SFR_UFSTAT0&0x000000f0)>>4);	
  cnt = 15-cnt;
  for(i=0;i<cnt;i++){
    if(gUart0SendCbuf.end_ptr == gUart0SendCbuf.start_ptr) break;	
    S3C2410SFR_UTXH0 = gUart0SendCbuf.buf[gUart0SendCbuf.end_ptr];
    gUart0SendCbuf.end_ptr = (gUart0SendCbuf.end_ptr+1)%gUart0SendCbuf.buf_size; 
    }
  if(i == 0) bgUart0SendEmptyFlag = 1;
  S3C2410SFR_SUBSRCPND = BIT_SUB_TXD0_MASK;		
  }	
//err----------------------------------------------------------------------------
if(S3C2410SFR_SUBSRCPND&BIT_SUB_ERR0_MASK){
  urxh = S3C2410SFR_URXH0;
  uerstat = S3C2410SFR_UERSTAT0;  
  S3C2410SFR_SUBSRCPND = BIT_SUB_ERR0_MASK;	
  }	
//uart0中断总出口-----------------------------------------------------------------
S3C2410SFR_SRCPND = INT_UART0_MASK;
//S3C2410SFR_INTPND = S3C2410SFR_INTPND;
return;		
}	


/*======================================================================
函数：
功能：
========================================================================*/
void uart1_interrupt(void)  
{
UBYTE i,cnt;
UBYTE uerstat,urxh;	

//rxd----------------------------------------------------------------------------
if(S3C2410SFR_SUBSRCPND&BIT_SUB_RXD1_MASK){
  cnt = S3C2410SFR_UFSTAT1&0x0000000f;
  for(i=0;i<cnt;i++){
    gUart1ReceCbuf.buf[gUart1ReceCbuf.start_ptr] = S3C2410SFR_URXH1;
    gUart1ReceCbuf.start_ptr = (gUart1ReceCbuf.start_ptr+1)%gUart1ReceCbuf.buf_size;
    if(gUart1ReceCbuf.end_ptr == gUart1ReceCbuf.start_ptr){
       gUart1ReceCbuf.end_ptr = (gUart1ReceCbuf.end_ptr+1)%gUart1ReceCbuf.buf_size;
       }	 
    }
  //  	
  S3C2410SFR_SUBSRCPND = BIT_SUB_RXD1_MASK;	
  }	
//txd----------------------------------------------------------------------------
if(S3C2410SFR_SUBSRCPND&BIT_SUB_TXD1_MASK){
  cnt = (UBYTE)((UDWORD)(S3C2410SFR_UFSTAT1&0x000000f0)>>4);	
  cnt = 15-cnt;
  for(i=0;i<cnt;i++){
    if(gUart1SendCbuf.end_ptr == gUart1SendCbuf.start_ptr) break;	
    S3C2410SFR_UTXH1 = gUart1SendCbuf.buf[gUart1SendCbuf.end_ptr];
    gUart1SendCbuf.end_ptr = (gUart1SendCbuf.end_ptr+1)%gUart1SendCbuf.buf_size; 
    }
  if(i == 0) bgUart1SendEmptyFlag = 1;
  S3C2410SFR_SUBSRCPND = BIT_SUB_TXD1_MASK;		
  }	
//err----------------------------------------------------------------------------
if(S3C2410SFR_SUBSRCPND&BIT_SUB_ERR1_MASK){
  urxh = S3C2410SFR_URXH1;
  uerstat = S3C2410SFR_UERSTAT1;  
  S3C2410SFR_SUBSRCPND = BIT_SUB_ERR1_MASK;	
  }	
//uart1中断总出口-----------------------------------------------------------------
S3C2410SFR_SRCPND = INT_UART1_MASK;
//S3C2410SFR_INTPND = S3C2410SFR_INTPND;
return;	
}	


/*======================================================================
函数：
功能：
========================================================================*/
void uart2_interrupt(void)  
{
UBYTE i,cnt;
UBYTE uerstat,urxh;	

//rxd----------------------------------------------------------------------------
if(S3C2410SFR_SUBSRCPND&BIT_SUB_RXD2_MASK){
  cnt = S3C2410SFR_UFSTAT2&0x0000000f;
  for(i=0;i<cnt;i++){
    gUart2ReceCbuf.buf[gUart2ReceCbuf.start_ptr] = S3C2410SFR_URXH2;
    gUart2ReceCbuf.start_ptr = (gUart2ReceCbuf.start_ptr+1)%gUart2ReceCbuf.buf_size;
    if(gUart2ReceCbuf.end_ptr == gUart2ReceCbuf.start_ptr){
       gUart2ReceCbuf.end_ptr = (gUart2ReceCbuf.end_ptr+1)%gUart2ReceCbuf.buf_size;
       }	 
    }
  //  	
  S3C2410SFR_SUBSRCPND = BIT_SUB_RXD2_MASK;	
  }	
//txd----------------------------------------------------------------------------
if(S3C2410SFR_SUBSRCPND&BIT_SUB_TXD2_MASK){
  cnt = (UBYTE)((UDWORD)(S3C2410SFR_UFSTAT2&0x000000f0)>>4);	
  cnt = 15-cnt;
  for(i=0;i<cnt;i++){
    if(gUart2SendCbuf.end_ptr == gUart2SendCbuf.start_ptr) break;	
    S3C2410SFR_UTXH2 = gUart2SendCbuf.buf[gUart2SendCbuf.end_ptr];
    gUart2SendCbuf.end_ptr = (gUart2SendCbuf.end_ptr+1)%gUart2SendCbuf.buf_size; 
    }
  if(i == 0) bgUart2SendEmptyFlag = 1;
  S3C2410SFR_SUBSRCPND = BIT_SUB_TXD2_MASK;		
  }	
//err----------------------------------------------------------------------------
if(S3C2410SFR_SUBSRCPND&BIT_SUB_ERR2_MASK){
  urxh = S3C2410SFR_URXH2;
  uerstat = S3C2410SFR_UERSTAT2;  
  S3C2410SFR_SUBSRCPND = BIT_SUB_ERR2_MASK;	
  }	
//uart2中断总出口-----------------------------------------------------------------
S3C2410SFR_SRCPND = INT_UART2_MASK;
//S3C2410SFR_INTPND = S3C2410SFR_INTPND;
return;	
}	


/*======================================================================
函数：timer4_interrupt
功能：使用timer4作为系统时钟，间隔1ms
========================================================================*/
void timer4_interrupt(void)
{
UDWORD i;

dwgSystemTimeCnt++;


if(bgTaskComEn) task_com();

//while(!uart_rece_is_empty(UART0_INDEX)){
//	 uart_put_byte(UART1_INDEX,uart_get_byte(UART0_INDEX));
//	 }
//while(!uart_rece_is_empty(UART1_INDEX)){
//	 uart_put_byte(UART0_INDEX,uart_get_byte(UART1_INDEX));
//	 }

  
if(bgSamProtEn){  
  sam_prot_send_process();
  sam_prot_rece_process();
  }

task_key_buzzle();
task_key();
  
//软时钟变量管理
if(bgSoftTimerEn){
for(i=0;i<MAX_TIMER_INDEX;i++){
   if(!bpgSoftTimerFlag[i]){
   	 dwpgSoftTimerCnt[i]++;
   	 if((UDWORD)dwpgSoftTimerCnt[i] >= (UDWORD)dwpgSoftTimerLimit[i]){
   	 	  bpgSoftTimerFlag[i] = 1;
   	 	  }
   	 }
   }
}

//if(bgEnUCOS){
//	bgOsTimeTick++;
//	if(bgOsTimeTick >= (1000/OS_TICKS_PER_SEC)){
//		bgOsTimeTick=0;
//		OSTimeTick();
//		}
//  }
  
//Timer4中断出口
S3C2410SFR_SRCPND = INT_TIMER4_MASK;	
//S3C2410SFR_INTPND = S3C2410SFR_INTPND;
return;
}	


/*======================================================================
函数：halt_system
功能：
========================================================================*/
void halt_system(void)
{
  while(1);
}


/*======================================================================
函数：spi1_interrupt
功能：
========================================================================*/
void spi1_interrupt(void) 
{
S3C2410SFR_SRCPND = INT_SPI1_MASK;		
return;
}


//end of file
#endif

