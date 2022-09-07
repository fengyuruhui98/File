//prot_zte.h
//中兴2.45G读写器协议

#ifndef _PROT_ZTE_H_
#define _PROT_ZTE_H_
//start of file

#define MAX_ZTE_BUF     512
#define ZTE_UART_INDEX  UART1_INDEX

#define ZTE_STX  0x02
#define ZTE_ETX  0x03

#define ZTE_RECE_WAIT_STX   0
#define ZTE_RECE_WAIT_LEN0  1
#define ZTE_RECE_WAIT_LEN1  2
#define ZTE_RECE_WAIT_DATA  3
#define ZTE_RECE_WAIT_LRC   4
#define ZTE_RECE_WAIT_ETX      5
#define ZTE_RECE_WAIT_PROCESS  6

#define ZTE_SEND_IDLE      0
#define ZTE_SEND_WAIT_ACK  1
#define ZTE_SEND_ERR       2
#define ZTE_SEND_TIMEOUT   3
//
#define ZTE_WAIT_ACK_TIMEOUT  600
#define ZTE_WAIT_CHAR_TIMEOUT 7		//20
//
typedef struct _ZTE_PROT_{
	UBYTE rece_state;
	UWORD rece_bytes;
	UBYTE send_state;
	UBYTE rece_timer_index;
	UBYTE send_timer_index;
	UWORD rece_ptr;
	UBYTE rece_buf[MAX_ZTE_BUF+1];
  } ZTE_PROT;


extern ZTE_PROT gZteProt0;

#define zte_rece_reset()   {gZteProt0.rece_state=ZTE_RECE_WAIT_STX;}
#define is_zte_wait_process()  (gZteProt0.rece_state==ZTE_RECE_WAIT_PROCESS)
#define is_zte_wait_stx()      (gZteProt0.rece_state==ZTE_RECE_WAIT_STX)
#define zte_processed()     zte_rece_reset()       
#define zte_clr_send_err()  {gZteProt0.send_state=ZTE_SEND_IDLE;}
#define is_zte_send_err()   (gZteProt0.send_state==ZTE_SEND_ERR)
#define is_zte_send_timeout()  timer_check(gZteProt0.send_timer_index)
#define zte_rece_bytes         gZteProt0.rece_bytes
#define zte_set_rece_ptr(ptr)      {ptr = gZteProt0.rece_buf;}
#define is_zte_send_free()         uart_send_is_empty(ZTE_UART_INDEX)

//函数
void prot_zte_send_block(UBYTE *buf,UWORD inbytes);
void prot_zte_rece_process(long msecond);
void prot_zte_init(UBYTE uart_index);

#ifdef _debug_test_
unsigned int GetTickcount();
#endif


//end of file
#endif



