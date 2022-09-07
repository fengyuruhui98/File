//reset_rec.h

#ifndef _RESET_REC_H_
#define _RESET_REC_H_
//start of file

#define MAX_RESET_REC   64

extern UBYTE bgResetRecPtr;

void reset_rec_init(void);
void reset_rec_save(void);
UBYTE reset_rec_get(UBYTE index,UBYTE *outbuf);


//end of file
#endif





