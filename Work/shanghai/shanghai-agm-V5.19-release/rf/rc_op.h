//rc_op.h

#ifndef _RC_OP_H_
#define _RC_OP_H_
//start of file


//#define REQ_TIME_OUT    3      //1ms
//#define SEL_TIME_OUT    14     //2ms
//#define AUTH_TIME_OUT   20     //3ms
//#define HALT_TIME_OUT   7      //1ms
//#define READ_TIME_OUT   20     //3ms
//#define WRITE1_TIME_OUT 7      //1ms
//#define WRITE2_TIME_OUT 100    //15ms
//#define VALUE1_TIME_OUT 14     //1ms
//#define VALUE2_TIME_OUT 20     //3ms
//#define VALUE3_TIME_OUT 100    //15ms
//
//#define ANTICOLL_TIME_OUT    67     //10ms


//Ԥ����------------------------------------------------------------------------
#define REQ_TIME_OUT    20     //3-->1ms,20-->3ms(2012/8/22 14:02:07)
#define SEL_TIME_OUT    14     //2ms
#define AUTH_TIME_OUT   20     //3ms
#define HALT_TIME_OUT   7      //1ms
#define READ_TIME_OUT   20     //3ms
#define WRITE1_TIME_OUT 7      //1ms
#define WRITE2_TIME_OUT 100    //15ms
#define VALUE1_TIME_OUT 14     //1ms
#define VALUE2_TIME_OUT 40     //3ms
#define VALUE3_TIME_OUT 100    //15ms

#define ANTICOLL_TIME_OUT    67     //10ms


#define NORMAL_IRQ_MASK   (BIT_RXI|BIT_TIMERI)
#define NORMAL_ERR_MASK   (BIT_RXI|BIT_TIMERI)
#define AUTH_IRQ_MASK     (BIT_IDLEI|BIT_TIMERI|BIT_ERRI)

//#define ant1sel_set()     gpa_set_bit(BIT12_MASK)
//#define ant1sel_clr()     gpa_clr_bit(BIT12_MASK)
//#define ant2sel_set()     gpa_set_bit(BIT08_MASK)
//#define ant2sel_clr()     gpa_clr_bit(BIT08_MASK)

#define m4_init()    rc_iso14443_typeb_init()
#define m1_init()    rc_iso14443_typea_init()

extern UBYTE bgIsoType;   //0:type a 1:type b


#define ISO14443A_M1_TYPE    0        
#define ISO14443A_SH_TYPE    1
#define ISO14443B_M4_TYPE    2
#define ISO15693_ICODE1_TYPE 3

//����---------------------------------------------------------------------------
void rc_init(void);
void rc_iso14443_typeb_init(void);
void rc_iso14443_typea_init(void);
void rc_wait_irq(void);
UBYTE rc_rece_bits(void);
UBYTE rc_send_cmd(UBYTE *inbuf,UBYTE inbytes,UBYTE irq_mask);
UBYTE rc_request(UBYTE req_code,UBYTE *atq);
UBYTE rc_anticoll(UBYTE sel_code,UBYTE bcnt,UBYTE *snr); 
UBYTE rc_select(UBYTE sel_code,UBYTE *snr,UBYTE *sak);
UBYTE rc_read(UBYTE block,UBYTE *outbuf);
UBYTE rc_read_4bytes(UBYTE block,UBYTE *outbuf);
UBYTE rc_write(UBYTE block,UBYTE *inbuf);
UBYTE rc_write_4bytes(UBYTE block,UBYTE *inbuf);
//UBYTE rc_value_op(UBYTE op_mode,UBYTE sr_block,UBYTE *value,UBYTE dest_block);
UWORD rc_crc_a(UBYTE *inbuf,UBYTE inbytes);
UBYTE rc_auth(UBYTE keyAB,UBYTE sector);
UBYTE rc_halta(void);
UBYTE rc_select_op_type(UBYTE op_type);
UBYTE rc_auth2(UBYTE keyAB,UBYTE sector);
UBYTE rc_value_op0(UBYTE op_mode,UBYTE sr_block,UBYTE *value);
UBYTE rc_transfer(UBYTE dest_block);

//end of file
#endif



