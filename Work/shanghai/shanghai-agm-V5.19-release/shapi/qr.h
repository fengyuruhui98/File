#ifndef QR_H
#define QR_H


void print_debug(unsigned char *inbuf, unsigned char len);

//UBYTE getinfo_QR(unsigned char *data);
UBYTE getinfo_QR(unsigned char *data, long len);
UBYTE getinfo_QR_V1(unsigned char *data);
UBYTE getinfo_QR_V2(unsigned char *data);
UBYTE qr_trans_entry(unsigned char *inbuf, unsigned char *outbuf);
UBYTE qr_trans_exit(unsigned char *inbuf, unsigned char *outbuf);
UBYTE qr_trans_update(unsigned char *inbuf, unsigned char *outbuf);

UBYTE qr_cal_process_mac(unsigned char *in_buf, unsigned char *out_buf);
UBYTE qr_cal_auth(unsigned char *in_buf, unsigned char *out_buf);

UBYTE sh_mqr_tac(UBYTE *inbuf, UBYTE *obuf);

#endif