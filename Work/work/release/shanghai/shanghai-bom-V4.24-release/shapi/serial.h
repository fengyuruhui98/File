#ifndef SERIAL_H
#define SERIAL_H

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

//‘§∂®“Â********************************************************************
#define MAX_UART_NUM   3
#define UART0_INDEX    0
#define UART1_INDEX    1
#define UART2_INDEX    2

#define BAUD1200_INDEX     0
#define BAUD2400_INDEX     1
#define BAUD4800_INDEX     2
#define BAUD9600_INDEX     3
#define BAUD19200_INDEX    4
#define BAUD28800_INDEX    5
#define BAUD38400_INDEX    6
#define BAUD57600_INDEX    7
#define BAUD115200_INDEX   8
#define BAUD128000_INDEX   9
#define BAUD256000_INDEX   10
#define MAX_BAUD_INDEX     10

#define PROT_DT_STX   0x02
#define PROT_DT_ETX   0x03
#define PROT_DT_DLE   0x10

unsigned char retry_buf[600], blnRetryFlag;
long 		retry_len;
unsigned char blnCalMAC2, chSmartSAMIndex;

int csc_comm;
int zte_comm;

int open_port(char *serial_port);
int close_port(int fd);
void speed_set(int fd, int speed);
int parity_set(int fd, int databits, int stopbits, int parity);
unsigned char writecom(int fd, unsigned char *pdata, long lnglen);
unsigned char readcom(int fd, unsigned char *pdata, long lnglen);
//UBYTE uart_put_bytes(UBYTE index,UBYTE *inbuf,UWORD inbytes,UWORD time_out);
long sh_DeleteDLE(unsigned char *pbytData, int intLength);
long sh_InsertDLE(unsigned char *pbytdata, int intLength);

long ReaderResponse(int fd, unsigned char chCode, unsigned char chCommand, unsigned char *psend, unsigned short len);
unsigned char DealCommand(unsigned char *cmd_buf,int cmd_len, unsigned char *out_buf, unsigned short *out_len);

long communicate(int fd, char *psend, long len, char *preceived);


unsigned char DealSmartCommand(int fd, unsigned char *cmd_buf, int cmd_len, unsigned char *out_buf, unsigned char *out_len);
unsigned char SmartResponse(int fd, unsigned char response_node, unsigned char *psend, unsigned char len, unsigned char packageFlag);

#endif
