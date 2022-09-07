#ifndef _UTILITIES_H
#define _UTILITIES_H
#include "datatype.h"

typedef struct _TlvData{
	Bytes tag;
	Bytes val;
}TlvData, *pTlvData;
int count_tlv_substring(UBYTE *src, int size);
int unpack_tlv_data(pTlvData *dst, UBYTE *src, int size);
#endif

