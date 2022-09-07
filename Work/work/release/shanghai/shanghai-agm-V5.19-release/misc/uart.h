//uart.h
//串口操作程序
//编制：邓建华
//时间：20060713

#ifndef _UART_H_
#define _UART_H_
//start of file

//预定义********************************************************************
#define MAX_UART_NUM   3
#define UART0_INDEX    0
#define UART1_INDEX    1
#define UART2_INDEX    2
//
#define UART0_SEND_BUF_SIZE   1200
#define UART0_RECE_BUF_SIZE   1200

#define UART1_SEND_BUF_SIZE   1200
#define UART1_RECE_BUF_SIZE   1200

#define UART2_SEND_BUF_SIZE   1200
#define UART2_RECE_BUF_SIZE   1200

#define UART0_SEL_OUT     0
#define UART0_SEL_HAND    1 
#define UART0_SEL_MODEM   2 

//波特率定义--------------------------------------------------------
#define BAUD256000_DIV    (UWORD)((float)PCLK*(1000000.0/256000.0/16.0) - 1)
#define BAUD128000_DIV    (UWORD)((float)PCLK*(1000000.0/128000.0/16.0) - 1)
#define BAUD115200_DIV    (UWORD)((float)PCLK*(1000000.0/115200.0/16.0) - 1)
#define BAUD57600_DIV     (UWORD)((float)PCLK*(1000000.0/57600.0/16.0) - 1)
#define BAUD38400_DIV     (UWORD)((float)PCLK*(1000000.0/38400.0/16.0) - 1)
#define BAUD28800_DIV     (UWORD)((float)PCLK*(1000000.0/28800.0/16.0) - 1)
#define BAUD19200_DIV     (UWORD)((float)PCLK*(1000000.0/19200.0/16.0) - 1)
#define BAUD9600_DIV      (UWORD)((float)PCLK*(1000000.0/9600.0/16.0) - 1)
#define BAUD4800_DIV      (UWORD)((float)PCLK*(1000000.0/4800.0/16.0) - 1)
#define BAUD2400_DIV      (UWORD)((float)PCLK*(1000000.0/2400.0/16.0) - 1)
#define BAUD1200_DIV      (UWORD)((float)PCLK*(1000000.0/1200.0/16.0) - 1)

//
#define BAUD57600_DIV2    (UWORD)((float)PCLK2*(1000000.0/57600.0/16.0) - 1)
#define BAUD115200_DIV2    (UWORD)((float)PCLK2*(1000000.0/115200.0/16.0) - 1)


#define BAUD1200_INDEX     0
#define BAUD2400_INDEX     1
#define BAUD4800_INDEX     2
#define BAUD9600_INDEX     3
#define BAUD19200_INDEX    4
#define BAUD28800_INDEX    5
#define BAUD38400_INDEX    6
#define BAUD57600_INDEX    7
#define BAUD115200_INDEX   8
#define BAUD128000_INDEX   9
#define BAUD256000_INDEX   10
#define MAX_BAUD_INDEX     10



//变量申明--------------------------------------------------------
extern CYCLE_BUF gUart0SendCbuf,gUart0ReceCbuf;
extern UBYTE bpgUart0SendBuf[UART0_SEND_BUF_SIZE+1];
extern UBYTE bpgUart0ReceBuf[UART0_RECE_BUF_SIZE+1];
extern UBYTE bgUart0SendEmptyFlag;

extern CYCLE_BUF gUart1SendCbuf,gUart1ReceCbuf;
extern UBYTE bpgUart1SendBuf[UART1_SEND_BUF_SIZE+1];
extern UBYTE bpgUart1ReceBuf[UART1_RECE_BUF_SIZE+1];
extern UBYTE bgUart1SendEmptyFlag;

extern CYCLE_BUF gUart2SendCbuf,gUart2ReceCbuf;
extern UBYTE bpgUart2SendBuf[UART2_SEND_BUF_SIZE+1];
extern UBYTE bpgUart2ReceBuf[UART2_RECE_BUF_SIZE+1];
extern UBYTE bgUart2SendEmptyFlag;

extern CYCLE_BUF  *gUartSendCbuf[MAX_UART_NUM];
extern CYCLE_BUF  *gUartReceCbuf[MAX_UART_NUM];


//函数申明---------------------------------------------------------
void uart_init(void);
UBYTE uart_rece_is_empty(UBYTE index);
UBYTE uart_send_is_full(UBYTE index);
void uart_sendbuf_clr(UBYTE index);
UBYTE uart_send_is_empty(UBYTE index);
void uart_recebuf_clr(UBYTE index);
//void uart_int_enable(UBYTE index);
//void uart_int_disable(UBYTE index);
UBYTE uart_put_byte(UBYTE index,UBYTE inbyte);
UBYTE uart_put_byte_safe(UBYTE index,UBYTE inbyte);
UBYTE uart_put_bytes(UBYTE index,UBYTE *inbuf,UWORD inbytes,UWORD time_out);
UBYTE uart_get_byte(UBYTE index);
void uart_change_baud(UBYTE index,UBYTE baud_index);
UWORD uart_get_div(UBYTE baud_index);
UBYTE uart_check_byte(UBYTE index);


//仿真
#ifdef _EMU_WIN_
void uart_put_rece_byte(UBYTE index,UBYTE inbyte);
UBYTE uart_get_send_byte(UBYTE index);
#endif

//end of file
#endif


