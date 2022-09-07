//fastdes.h

#ifndef _FASTDES_H_
#define _FASTDES_H_
//start of file

void des_encode(UBYTE *key,UBYTE *sr,UBYTE *dest);
void des_decode(UBYTE *key,UBYTE *sr,UBYTE *dest);
void des3_encode(UBYTE *key,UBYTE *sr,UBYTE *dest);
void des3_decode(UBYTE *key,UBYTE *sr,UBYTE *dest);

//end of file
#endif


