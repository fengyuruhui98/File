#ifndef QR_YDWL_H
#define QR_YDWL_H

#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <linux/serial.h>
#include <asm/ioctls.h>
#include <sys/io.h>
#include <time.h>

//#define DEBUG_TEST				1

typedef char (*CallBackFun)(char *p);


long EnableQR();
long DisableAll();
long SetMode ();
long SetInterval();
long LightControl(char LEDSwitch);
long BeepControl(char bytBeepFlag);


void InitScan();
void StartScan();
void EndScan();
//void scancontrol((*CallBackFun)(char *p) *backdata ,long *backdatalength);
int scancontrol(unsigned char *backdata ,long *backdatalength);

#endif