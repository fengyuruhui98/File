//mcmh.h

#ifndef _MCMH_H_
#define _MCMH_H_
//start of file

#define MCMH_MAX_TRY    3

extern UBYTE gKeyA[16][6],gKeyB[16][6];

UBYTE mcmh_get_cardsnr(UBYTE *cardsnr);
UBYTE mcmh_read(UBYTE block, UBYTE *outbuf,UBYTE op_type,UBYTE key_type);
UBYTE mcmh_read_simple(UBYTE block, UBYTE *outbuf,UBYTE op_type,UBYTE key_type);
UBYTE mcmh_write(UBYTE block, UBYTE *inbuf,UBYTE op_type,UBYTE key_type);
UBYTE mcmh_transfer(UBYTE block, UBYTE op_type,UBYTE key_type);
UBYTE mcmh_restore(UBYTE block, UBYTE op_type,UBYTE key_type);
UBYTE mcmh_increment(UBYTE block, UDWORD value,UBYTE op_type,UBYTE key_type);
UBYTE mcmh_decrement(UBYTE block, UDWORD value,UBYTE op_type,UBYTE key_type);
UBYTE mcmh_set_key(UBYTE sector,UBYTE key_type,UBYTE *inbuf);
UBYTE mcmh_get_key(UBYTE sector,UBYTE key_type,UBYTE *outbuf);

//end of file
#endif

