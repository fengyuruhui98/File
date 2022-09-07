//serial24G.h
#ifndef _SERIAL24G_H_
#define _SERIAL24G_H_


int ZTE_REQ(void);
int ZTE_HALT(void);
int ZTE_GET_STATUS(UBYTE *resp);
int ZTE_ICMD(UBYTE slen,UBYTE *sbuf,UBYTE *rlen,UBYTE *outbuf,int otime);
int ZTE_REQ_para(void);


#endif