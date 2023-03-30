//uart.c
//串口操作程序
//编制：邓建华
//时间：20060713

#ifndef _UART_C_
#define _UART_C_
//start of file

CYCLE_BUF gUart0SendCbuf,gUart0ReceCbuf;
UBYTE bpgUart0SendBuf[UART0_SEND_BUF_SIZE+1];
UBYTE bpgUart0ReceBuf[UART0_RECE_BUF_SIZE+1];
UBYTE bgUart0SendEmptyFlag;

CYCLE_BUF gUart1SendCbuf,gUart1ReceCbuf;
UBYTE bpgUart1SendBuf[UART1_SEND_BUF_SIZE+1];
UBYTE bpgUart1ReceBuf[UART1_RECE_BUF_SIZE+1];
UBYTE bgUart1SendEmptyFlag;

CYCLE_BUF gUart2SendCbuf,gUart2ReceCbuf;
UBYTE bpgUart2SendBuf[UART2_SEND_BUF_SIZE+1];
UBYTE bpgUart2ReceBuf[UART2_RECE_BUF_SIZE+1];
UBYTE bgUart2SendEmptyFlag;

CYCLE_BUF  *gUartSendCbuf[MAX_UART_NUM];
CYCLE_BUF  *gUartReceCbuf[MAX_UART_NUM];

/*==================================================================
函数：
功能：
====================================================================*/
void uart_init(void)
{

//缓冲区初始化	
cbuf_init(&gUart0SendCbuf,UART0_SEND_BUF_SIZE,bpgUart0SendBuf);
cbuf_init(&gUart0ReceCbuf,UART0_RECE_BUF_SIZE,bpgUart0ReceBuf);

cbuf_init(&gUart1SendCbuf,UART1_SEND_BUF_SIZE,bpgUart1SendBuf);
cbuf_init(&gUart1ReceCbuf,UART1_RECE_BUF_SIZE,bpgUart1ReceBuf);

cbuf_init(&gUart2SendCbuf,UART2_SEND_BUF_SIZE,bpgUart2SendBuf);
cbuf_init(&gUart2ReceCbuf,UART2_RECE_BUF_SIZE,bpgUart2ReceBuf);

gUartSendCbuf[UART0_INDEX] = &gUart0SendCbuf;
gUartSendCbuf[UART1_INDEX] = &gUart1SendCbuf;
gUartSendCbuf[UART2_INDEX] = &gUart2SendCbuf;

gUartReceCbuf[UART0_INDEX] = &gUart0ReceCbuf;
gUartReceCbuf[UART1_INDEX] = &gUart1ReceCbuf;
gUartReceCbuf[UART2_INDEX] = &gUart2ReceCbuf;


//uart配置
//#ifndef _EMU_WIN_
S3C2410SFR_ULCON0 = 0x03;     //8bits,1stop bits,no parity
S3C2410SFR_ULCON1 = 0x03;     //8bits,1stop bits,no parity
S3C2410SFR_ULCON2 = 0x03;     //8bits,1stop bits,no parity

S3C2410SFR_UCON0 = 0x0c5;
S3C2410SFR_UCON1 = 0x0c5;
S3C2410SFR_UCON2 = 0x0c5;    //enable [rx time out] interrupt, pulse mode int

S3C2410SFR_UFCON0 = 0x07;
S3C2410SFR_UFCON1 = 0x07;
S3C2410SFR_UFCON2 = 0x07;    //0B trans, 4B rece,enable FIFO,clr fifo

//S3C2410SFR_UFCON0 = 0x17;
//S3C2410SFR_UFCON1 = 0x17;
//S3C2410SFR_UFCON2 = 0x17;   //0B trans, 8B rece,enable FIFO,clr fifo

S3C2410SFR_UBRDIV0 = BAUD115200_DIV;
S3C2410SFR_UBRDIV1 = BAUD115200_DIV;
S3C2410SFR_UBRDIV2 = BAUD115200_DIV;

//中断设置
S3C2410SFR_INTMSK = S3C2410SFR_INTMSK&(UDWORD)(~INT_UART0_MASK);                                                //enable uart0 int
S3C2410SFR_INTSUBMSK = S3C2410SFR_INTSUBMSK&(UDWORD)(~(BIT_SUB_TXD0_MASK|BIT_SUB_RXD0_MASK|BIT_SUB_ERR0_MASK)); //enable uart0:txd,rxd,err
bgUart0SendEmptyFlag = 1;

S3C2410SFR_INTMSK = S3C2410SFR_INTMSK&(UDWORD)(~INT_UART1_MASK);                                                //enable uart1 int
S3C2410SFR_INTSUBMSK = S3C2410SFR_INTSUBMSK&(UDWORD)(~(BIT_SUB_TXD1_MASK|BIT_SUB_RXD1_MASK|BIT_SUB_ERR1_MASK)); //enable uart1:txd,rxd,err
bgUart1SendEmptyFlag = 1;

S3C2410SFR_INTMSK = S3C2410SFR_INTMSK&(UDWORD)(~INT_UART2_MASK);                                                //enable uart2 int
S3C2410SFR_INTSUBMSK = S3C2410SFR_INTSUBMSK&(UDWORD)(~(BIT_SUB_TXD2_MASK|BIT_SUB_RXD2_MASK|BIT_SUB_ERR2_MASK)); //enable uart2:txd,rxd,err
bgUart2SendEmptyFlag = 1;

//#endif

return;	
}	


/*==================================================================
函数：
功能：
====================================================================*/
UBYTE uart_rece_is_empty(UBYTE index)
{
return cbuf_is_empty(gUartReceCbuf[index]);	
}	


/*==================================================================
函数：
功能：
====================================================================*/
UBYTE uart_send_is_empty(UBYTE index)
{
//	
return cbuf_is_empty(gUartSendCbuf[index]);	
}	

	
/*==================================================================
函数：
功能：
====================================================================*/
UBYTE uart_send_is_full(UBYTE index)
{
index = index%MAX_UART_NUM;
return cbuf_is_full(gUartSendCbuf[index]);	
}	

/*==================================================================
函数：
功能：
====================================================================*/
void uart_sendbuf_clr(UBYTE index)
{
index = index%MAX_UART_NUM;
cbuf_clr(gUartSendCbuf[index]);
return;	
}	


/*==================================================================
函数：
功能：
====================================================================*/
void uart_recebuf_clr(UBYTE index)
{
index = index%MAX_UART_NUM;
cbuf_clr(gUartReceCbuf[index]);	
return;	
}	


/*==================================================================
函数：
功能：
====================================================================*/
//void uart_int_enable(UBYTE index)
//{
////实际---------------------------------------------------------------
//#ifndef _EMU_WIN_
//index = index%MAX_UART_NUM;
////disable_int();
//switch(index){
//  case UART0_INDEX:
//      S3C2410SFR_INTMSK = S3C2410SFR_INTMSK&(UDWORD)(~INT_UART0_MASK); //enable uart0 int
//      break;
//  case UART1_INDEX:
//      S3C2410SFR_INTMSK = S3C2410SFR_INTMSK&(UDWORD)(~INT_UART1_MASK); //enable uart1 int
//      break;
//  case UART2_INDEX:
//      S3C2410SFR_INTMSK = S3C2410SFR_INTMSK&(UDWORD)(~INT_UART2_MASK); //enable uart2 int
//      break;
//  }
////enable_int();
//return;
//#endif
////仿真----------------------------------------------------------------
//#ifdef _EMU_WIN_
//index = index;
//return;
//#endif
//}	


/*==================================================================
函数：
功能：
====================================================================*/
//void uart_int_disable(UBYTE index)
//{
////实际---------------------------------------------------------------
//#ifndef _EMU_WIN_
//index = index%MAX_UART_NUM;
////disable_int();
//switch(index){
//  case UART0_INDEX:
//     S3C2410SFR_INTMSK = S3C2410SFR_INTMSK|INT_UART0_MASK;          //disable uart0 int
//     break;
//  case UART1_INDEX:
//     S3C2410SFR_INTMSK = S3C2410SFR_INTMSK|INT_UART1_MASK;          //disable uart1 int
//     break;
//  case UART2_INDEX:
//     S3C2410SFR_INTMSK = S3C2410SFR_INTMSK|INT_UART2_MASK;          //disable uart2 int
//     break;
//  }
////enable_int();
//return; 
//#endif
////仿真----------------------------------------------------------------
//#ifdef _EMU_WIN_
//index = index;
//return;
//#endif
//}



/*==================================================================
函数：
功能：
====================================================================*/
UBYTE uart_put_byte(UBYTE index,UBYTE inbyte)
{
UBYTE ret;
UBYTE cnt;
OS_CPU_SR cpu_sr;
	
index = index%MAX_UART_NUM;
os_int_disable();	
ret = cbuf_put_byte(gUartSendCbuf[index],inbyte);
if(ret != 0){
  os_int_enable();	
  return ret;
  }
//启动传输
#ifndef _EMU_WIN_
switch(index){
  case UART0_INDEX:
    cnt = (UBYTE)((UDWORD)(S3C2410SFR_UFSTAT0&0x000000f0)>>4);     
    if(bgUart0SendEmptyFlag || (cnt==0)){
       //uart_int_disable(index);
       ret = cbuf_get_byte(gUartSendCbuf[index]);	  	  		
       S3C2410SFR_UTXH0 = ret; 
       bgUart0SendEmptyFlag = 0;
       //uart_int_enable(index);
       }
    break;   
  case UART1_INDEX:
    cnt = (UBYTE)((UDWORD)(S3C2410SFR_UFSTAT1&0x000000f0)>>4);     
    if(bgUart1SendEmptyFlag || (cnt==0)){
       //uart_int_disable(index);
       ret = cbuf_get_byte(gUartSendCbuf[index]);	  	  		
       S3C2410SFR_UTXH1 = ret; 
       bgUart1SendEmptyFlag = 0;
       //uart_int_enable(index);
       }
    break;   
  case UART2_INDEX:
    cnt = (UBYTE)((UDWORD)(S3C2410SFR_UFSTAT2&0x000000f0)>>4);     
    if(bgUart2SendEmptyFlag  || (cnt==0)){
       //uart_int_disable(index);
       ret = cbuf_get_byte(gUartSendCbuf[index]);	  	  		
       S3C2410SFR_UTXH2 = ret; 
       bgUart2SendEmptyFlag = 0;
       //uart_int_enable(index);
       }
    break;   
  }
#endif
os_int_enable();	
return 0;
}	


/*==================================================================
函数：
功能：
====================================================================*/
UBYTE uart_put_byte_safe(UBYTE index,UBYTE inbyte)
{
while(uart_send_is_full(index)){
	 //watchdog();
	 //Sleep(50);
	 delay_ms(1);
	 }

return uart_put_byte(index,inbyte);
}	


/*==================================================================
函数：
功能：
====================================================================*/
UBYTE uart_put_bytes(UBYTE index,UBYTE *inbuf,UWORD inbytes,UWORD time_out)
{
UWORD i,ptr;

for(i=0,ptr=0;ptr<inbytes;){
  if(uart_put_byte(index,inbuf[ptr]) == 0){
     ptr++;
     continue;
     }
  delay_ms(1);	
  i++;
  if(i >= time_out) return (UBYTE)-1;
  }
  
return 0;	
}	



/*==================================================================
函数：
功能：
====================================================================*/
UBYTE uart_get_byte(UBYTE index)
{
UBYTE ret;	
OS_CPU_SR  cpu_sr;

index = index%MAX_UART_NUM;
os_int_disable();
ret = cbuf_get_byte(gUartReceCbuf[index]);
os_int_enable();
return ret;
}	


/*==================================================================
函数：uart_change_baud
功能：
====================================================================*/
void uart_change_baud(UBYTE index,UBYTE baud_index)
{
UWORD w_div;
OS_CPU_SR  cpu_sr;

w_div = uart_get_div(baud_index);

#ifndef _EMU_WIN_	
switch(index){
  case UART0_INDEX:   
   while(!bgUart0SendEmptyFlag){};  //等待发送缓冲区数据完毕
   os_int_disable();
   S3C2410SFR_UBRDIV0 = w_div;
   uart_recebuf_clr(index); 
   os_int_enable();   
   break;
  case UART1_INDEX:   
   while(!bgUart1SendEmptyFlag){};  //等待发送缓冲区数据完毕
   os_int_disable();
   S3C2410SFR_UBRDIV1 = w_div;
   uart_recebuf_clr(index); 
   os_int_enable();
   break;
  case UART2_INDEX:   
   while(!bgUart2SendEmptyFlag){};  //等待发送缓冲区数据完毕
   os_int_disable();
   S3C2410SFR_UBRDIV2 = w_div;
   uart_recebuf_clr(index); 
   os_int_enable();   
   break;
  }
#endif
#ifdef _EMU_WIN_
w_div = w_div;
#endif
return;	
}	


/*==================================================================
函数：
功能：
====================================================================*/
#ifdef _EMU_WIN_
void uart_put_rece_byte(UBYTE index,UBYTE inbyte)
{
index = index%MAX_UART_NUM;
cbuf_put_byte(gUartReceCbuf[index],inbyte);
return;
}
#endif


/*==================================================================
函数：
功能：
====================================================================*/
#ifdef _EMU_WIN_
UBYTE uart_get_send_byte(UBYTE index)
{
return cbuf_get_byte(gUartSendCbuf[index]);
}
#endif

/*==================================================================
函数：
功能：
====================================================================*/
UWORD uart_get_div(UBYTE baud_index)
{
UWORD w_div;
	
//波特率选择
switch(baud_index){
  case BAUD256000_INDEX:
       w_div = BAUD256000_DIV;
       break;
  case BAUD128000_INDEX:
       w_div = BAUD128000_DIV;
       break;
  case BAUD115200_INDEX:
       w_div = BAUD115200_DIV;
       break;
  case BAUD57600_INDEX:
       w_div = BAUD57600_DIV;
       break;
  case BAUD38400_INDEX:
       w_div = BAUD38400_DIV;
       break;
  case BAUD28800_INDEX:
       w_div = BAUD28800_DIV;
       break;
  case BAUD19200_INDEX:
       w_div = BAUD19200_DIV;
       break;
  case BAUD4800_INDEX:
       w_div = BAUD4800_DIV;
       break;       
  case BAUD2400_INDEX:
       w_div = BAUD2400_DIV;
       break;       
  case BAUD1200_INDEX:
       w_div = BAUD1200_DIV;
       break;              
  default:
       w_div = BAUD9600_DIV;
       break;  	
  }		
return w_div;
}

/*==================================================================
函数：
功能：
====================================================================*/
UBYTE uart_check_byte(UBYTE index)
{
return cbuf_check_byte(gUartReceCbuf[index%MAX_UART_NUM]);	
}	


//end of file
#endif

