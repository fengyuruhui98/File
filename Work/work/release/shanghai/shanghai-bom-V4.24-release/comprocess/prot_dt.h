//prot_dt.h
//地铁通讯协议
//编制:邓建华
//时间:20090427

#ifndef _PROT_DT_H_
#define _PROT_DT_H_
//start of file

#define UART_DT_CMD_INDEX  UART0_INDEX

#define PROT_DT_STX   0x02
#define PROT_DT_ETX   0x03
#define PROT_DT_DLE   0x10

#define PROT_DT_SEND_OK             0
#define PROT_DT_SEND_WAIT_ACK       1
#define PROT_DT_SEND_ERR            2
#define PROT_DT_SEND_START          3

#define PROT_DT_RECE_WAIT_STX       0
#define PROT_DT_RECE_WAIT_LEN_DLE   1
#define PROT_DT_RECE_WAIT_LEN       2
#define PROT_DT_RECE_WAIT_DATA_DLE  3
#define PROT_DT_RECE_WAIT_DATA      4
#define PROT_DT_RECE_WAIT_LRC_DLE   5
#define PROT_DT_RECE_WAIT_LRC       6
#define PROT_DT_RECE_WAIT_ETX       7
#define PROT_DT_RECE_WAIT_PROCESS   8



#define PROT_DT_WAIT_ACK_TIMEOUT        150  //150ms必须有应答
#define PROT_DT_RECE_WAIT_CHAR_TIMEOUT  50      
#define PROT_DT_MAX_RETRY          1

typedef struct{
	UBYTE send_state;
	UBYTE send_bytes;
	UBYTE rece_state;
	UBYTE rece_bytes;
	UBYTE send_retry;
	UBYTE send_timer_index;
	UBYTE rece_timer_index;
	UBYTE rece_ptr;
	UBYTE rece_buf[256];
	UBYTE last_rece_buf[256];
	UBYTE last_rece_bytes;
  UBYTE send_buf[256];
  } DT_PROT;


extern DT_PROT gDtProt0;

//函数
void prot_dt_init(DT_PROT *p,UBYTE uart_index);
void uart_put_byte_prot_dt(UBYTE uart_index,UBYTE inbyte);
void prot_dt_send_block(UBYTE uart_index,DT_PROT *p);
void prot_dt_send_process(UBYTE uart_index,DT_PROT *p);
void prot_dt_rece_process(UBYTE uart_index,DT_PROT *p);
void prot_dt_cmd_process(DT_PROT *p);
UBYTE prot_dt_is_wait_process(DT_PROT *p);
void prot_dt_processed(DT_PROT *p);

//end of file
#endif




