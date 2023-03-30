//lrc.h

#ifndef _LRC_H
#define _LRC_H

void lrc_gen(UBYTE *buf,UWORD bytes);
UBYTE lrc_chk(UBYTE *buf,UWORD bytes);
void lrc_gen2(UBYTE *buf,UWORD bytes);
UBYTE lrc_chk2(UBYTE *buf,UWORD bytes);
UBYTE if_str_all_is(UBYTE *str,UBYTE value,UWORD n);
void lrc3_gen(UBYTE *buf,UWORD bytes);
UBYTE lrc3_chk(UBYTE *buf,UWORD bytes);
UBYTE purse_chk(UBYTE *inbuf);


#endif
