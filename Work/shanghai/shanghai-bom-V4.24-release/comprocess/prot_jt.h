//prot_jt.h
//研发部协议(增加包类型)
//编制：邓建华
//时间：20080705

#ifndef _PROT_JT_H_
#define _PROT_JT_H_
//start of file

#define UART_JT_CMD0_INDEX   UART0_INDEX

//预定义-----------------------------------------------------------------------------
#define PROT_JT_BLOCK_HEAD   0x7e
#define PROT_JT_SIGNAL       0x7d
#define PROT_JT_ACK_HEAD     0x41 

#define PROT_JT_SEND_IDLE           0
#define PROT_JT_SEND_WAIT_ACK0      1
#define PROT_JT_SEND_WAIT_ACK1      2
#define PROT_JT_SEND_OK             3
#define PROT_JT_SEND_ERR            4
#define PROT_JT_SEND_START          5

#define PROT_JT_RECE_WAIT_HEAD      0
#define PROT_JT_RECE_WAIT_LEN       1
#define PROT_JT_RECE_WAIT_SNODE     2
#define PROT_JT_RECE_WAIT_DNODE     3
#define PROT_JT_RECE_WAIT_TYPE      4
#define PROT_JT_RECE_WAIT_DATA      5
#define PROT_JT_RECE_WAIT_LRC1      6
#define PROT_JT_RECE_WAIT_LRC2      7
#define PROT_JT_RECE_WAIT_PROCESS   8

#define PROT_JT_SEND_WAIT_ACK0_TIMEOUT  100
#define PROT_JT_SEND_WAIT_ACK1_TIMEOUT  50
#define PROT_JT_RECE_WAIT_CHAR_TIMEOUT  50

#define PROT_JT_CMD_TYPE    0x80
#define PROT_JT_ACK_TYPE    0x00

#define PROT_JT_ANYONE_NODE  0xfe

//#define _PROT_JT_EXT_                //扩展模式，发送包类型


//变量--------------------------------------------------------------------------------
typedef struct{
	UBYTE send_state;
	UBYTE send_bytes;
	UBYTE rece_state;
	UBYTE rece_bytes;
	UBYTE send_sr_node;
	UBYTE send_dest_node;
	UBYTE rece_sr_node;
	UBYTE rece_dest_node;
	UBYTE send_type;
	UBYTE rece_type;
	UBYTE send_timer_index;
	UBYTE rece_timer_index;
	UBYTE rece_ptr;
	UBYTE signal;
	UBYTE discard;
	UBYTE acked;
	UBYTE discard_timer_index;
	UBYTE last_cmd[5];
	UBYTE last_cmd_bytes;
	UBYTE rece_buf[256];
  UBYTE send_buf[256];
  } JT_PROT;


//变量-------------------------------------------------------------------------------
extern JT_PROT  gJtProt0;

//函数--------------------------------------------------------------------------------
void prot_jt_init(JT_PROT *p,UBYTE sr_node,UBYTE dest_node,UBYTE uart_index);
void prot_jt_send_block(UBYTE uart_index,JT_PROT *p);
void prot_jt_rece_process(UBYTE uart_index,JT_PROT *p);
void prot_jt_send_process(UBYTE uart_index,JT_PROT *p);
void prot_jt_wait_send(JT_PROT *p);
UBYTE prot_jt_is_send_ok(JT_PROT *p);
UBYTE prot_jt_is_wait_process(JT_PROT *p);
void  prot_jt_processed(JT_PROT *p);
UBYTE prot_jt_rece_is_cmd(JT_PROT *p);
void  prot_jt_cmd_process(JT_PROT *p);
UBYTE prot_jt_is_free(JT_PROT *p);


//end of file
#endif

