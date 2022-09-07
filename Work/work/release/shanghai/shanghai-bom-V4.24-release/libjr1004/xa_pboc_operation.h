#ifndef SZ_PBOC_OPERATION_H
#define SZ_PBOC_OPERATION_H

unsigned char ch_sz_pboc_rollback;

struct TLVEntity tlv_ppse, tlv_aid, tlv_gpo;

char pboc_read_fci(unsigned char *in_buf, unsigned char in_len, unsigned char *out_buf);

#endif