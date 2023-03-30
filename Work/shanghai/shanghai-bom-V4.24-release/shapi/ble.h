#ifndef BLE_H
#define BLE_H

int openBLE(unsigned char *com);
void closeBLE();

void *pthBLE();

void startBLEOnline(unsigned char *inbuf);
int getBlEOnlineReturn(unsigned char *outbuf);
void sendBLEData(unsigned char *inbuf, unsigned char inlen);
int getBLESendReturn(unsigned char *outbuf, int *outlen);
void stopBLEOffline();


#endif