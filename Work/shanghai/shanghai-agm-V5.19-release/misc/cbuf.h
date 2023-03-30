//cbuf.h

#ifndef _CBUF_H_
#define _CBUF_H_
//start of file

//�ṹ����-----------------------------------------------------
typedef struct {
  UWORD start_ptr;
  UWORD end_ptr;
  UWORD buf_size;
  UBYTE *buf; 	
  } CYCLE_BUF;	


//��������-----------------------------------------------------
void cbuf_init(CYCLE_BUF *cbuf,UWORD buf_size,UBYTE *buf);
UBYTE cbuf_is_empty(CYCLE_BUF *cbuf);
UBYTE cbuf_is_full(CYCLE_BUF *cbuf);
UBYTE cbuf_put_byte(CYCLE_BUF *cbuf,UBYTE inbyte);
UBYTE cbuf_get_byte(CYCLE_BUF *cbuf);
void cbuf_clr(CYCLE_BUF *cbuf);
UBYTE cbuf_check_byte(CYCLE_BUF *cbuf);

//end of file
#endif

