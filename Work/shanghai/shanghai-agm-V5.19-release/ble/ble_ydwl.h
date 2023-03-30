#ifndef BLE_YDWL_H
#define BLE_YDWL_H

unsigned char sumcheck(unsigned char *inbuf, unsigned short len);
long ydwl_Insert7E(unsigned char *pbytData, int intLength);
long ydwl_Delete7E(unsigned char *pbytData, int intLength);
int ydwl_Comm(unsigned char *inbuf, unsigned char inlen, unsigned char *outbuf, unsigned short *outLen, unsigned short timeMs);

int open_BLE(unsigned char *com);
void ();


int bleOnline(unsigned char *inbuf, unsigned char inlen, unsigned char *outbuf);
int bleSend(unsigned char *inbuf, unsigned char inlen, unsigned char *backdata ,int *backdatalength);
int bleOffline();

#endif