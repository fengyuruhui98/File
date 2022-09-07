//include.h
#ifndef _INCLUDE_H_
#define _INCLUDE_H_

//start of file

#define UDWORD unsigned long
#define UWORD  unsigned short
#define UBYTE  unsigned char
 
#define INT_LOW     0
#define INT_HIGH    1
#define LONG_HIGH3  3
#define LONG_HIGH2  2
#define LONG_HIGH1  1
#define LONG_HIGH0  0

#define uword_to_buf(inword,outbuf)   {*((UBYTE *)(outbuf))=*((char *)&inword+INT_HIGH);*((UBYTE*)(outbuf)+1)=*((char *)&inword+INT_LOW);}
#define buf_to_uword(inbuf,outword)   {*((char *)&outword+INT_HIGH)=*((UBYTE *)(inbuf)+0);*((char *)&outword+INT_LOW)=*((UBYTE *)(inbuf)+1);}
#define udword_to_buf(indword,outbuf) {*((UBYTE *)(outbuf))=(UBYTE)*((char *)&indword+LONG_HIGH3);*((UBYTE*)(outbuf)+1)=(UBYTE)*((char *)&indword+LONG_HIGH2);*((UBYTE *)(outbuf)+2)=*((char *)&indword+LONG_HIGH1);*((UBYTE *)(outbuf)+3)=*((char *)&indword+LONG_HIGH0);}
#define buf_to_udword(inbuf,outdword) {*((char *)&outdword+LONG_HIGH3)=*((UBYTE *)inbuf+0);*((char *)&outdword+LONG_HIGH2)=*((UBYTE *)(inbuf)+1);*((char *)&outdword+LONG_HIGH1)=*((UBYTE *)(inbuf)+2);*((char *)&outdword+LONG_HIGH0)=*((UBYTE *)(inbuf)+3);}

#define udword_to_buf3(indword,outbuf) {*((UBYTE*)(outbuf))=(UBYTE)*((char *)&indword+LONG_HIGH2);*((UBYTE *)(outbuf)+1)=*((char *)&indword+LONG_HIGH1);*((UBYTE *)(outbuf)+2)=*((char *)&indword+LONG_HIGH0);}
#define buf3_to_udword(inbuf,outdword) {*((char *)&outdword+LONG_HIGH3)=0;*((char *)&outdword+LONG_HIGH2)=*((UBYTE *)(inbuf));*((char *)&outdword+LONG_HIGH1)=*((UBYTE *)(inbuf)+1);*((char *)&outdword+LONG_HIGH0)=*((UBYTE *)(inbuf)+2);}

#define udword_to_buf_reverse(indword,outbuf) {*((UBYTE *)(outbuf))=(UBYTE)*((char *)&indword+LONG_HIGH0);*((UBYTE*)(outbuf)+1)=(UBYTE)*((char *)&indword+LONG_HIGH1);*((UBYTE *)(outbuf)+2)=*((char *)&indword+LONG_HIGH2);*((UBYTE *)(outbuf)+3)=*((char *)&indword+LONG_HIGH3);}
#define buf_to_udword_reverse(inbuf,outdword) {*((char *)&outdword+LONG_HIGH0)=*((UBYTE *)inbuf+0);*((char *)&outdword+LONG_HIGH1)=*((UBYTE *)(inbuf)+1);*((char *)&outdword+LONG_HIGH2)=*((UBYTE *)(inbuf)+2);*((char *)&outdword+LONG_HIGH3)=*((UBYTE *)(inbuf)+3);}
#define buf_to_uword_reverse(inbuf,outword)   {*((char *)&outword+INT_LOW)=*((UBYTE *)(inbuf)+0);*((char *)&outword+INT_HIGH)=*((UBYTE *)(inbuf)+1);}
#define uword_to_buf_reverse(inword,outbuf)   {*((UBYTE *)(outbuf))=*((char *)&inword+INT_LOW);*((UBYTE*)(outbuf)+1)=*((char *)&inword+INT_HIGH);}

#define BIT00_MASK  0x00000001
#define BIT01_MASK  0x00000002
#define BIT02_MASK  0x00000004
#define BIT03_MASK  0x00000008
#define BIT04_MASK  0x00000010
#define BIT05_MASK  0x00000020
#define BIT06_MASK  0x00000040
#define BIT07_MASK  0x00000080
#define BIT08_MASK  0x00000100
#define BIT09_MASK  0x00000200
#define BIT10_MASK  0x00000400
#define BIT11_MASK  0x00000800
#define BIT12_MASK  0x00001000
#define BIT13_MASK  0x00002000
#define BIT14_MASK  0x00004000
#define BIT15_MASK  0x00008000
#define BIT16_MASK  0x00010000
#define BIT17_MASK  0x00020000
#define BIT18_MASK  0x00040000
#define BIT19_MASK  0x00080000
#define BIT20_MASK  0x00100000
#define BIT21_MASK  0x00200000
#define BIT22_MASK  0x00400000
#define BIT23_MASK  0x00800000
#define BIT24_MASK  0x01000000
#define BIT25_MASK  0x02000000
#define BIT26_MASK  0x04000000
#define BIT27_MASK  0x08000000
#define BIT28_MASK  0x10000000
#define BIT29_MASK  0x20000000
#define BIT30_MASK  0x40000000
#define BIT31_MASK  0x80000000
#define BIT0_MASK   BIT00_MASK
#define BIT1_MASK   BIT01_MASK
#define BIT2_MASK   BIT02_MASK
#define BIT3_MASK   BIT03_MASK
#define BIT4_MASK   BIT04_MASK
#define BIT5_MASK   BIT05_MASK
#define BIT6_MASK   BIT06_MASK
#define BIT7_MASK   BIT07_MASK
#define BIT8_MASK   BIT08_MASK
#define BIT9_MASK   BIT09_MASK

#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <iconv.h>
#include <errno.h>

//sam卡 卡片切频开关
#define	SAM_PPS_ON
#define CARD_PPS_ON

#define code
#define _MOBILEv1_1_
#define _PCBV12_  
#define _RECOVERY_AFTER_RESET_
//#define _debug_test_
#define RF_CHANNEL 0

//测试卡，读到此卡不返回信息
#define MUTE_CARD_SNR   "\xff\xff\xff\xff"

#define debug_printf    printf

#define mcml_request(a,b)  mcml_request2(a,b)

//linux2440lib
#include "linux2440lib.h"

//libjr1004
#include ".//libjr1004//libjr1004.h"
//#include ".//libjr1004//jrt0025_12.h"
//#include ".//libjr1004//rsa.h"
//#include ".//libjr1004//sha.h"
//#include ".//libjr1004//utilities.h"

//des
#include ".//des//fastdes.h"
#include ".//des//test_hard.h"

//eeprom
#include ".//eeprom//eeprom.h"

//rf
#include ".//rf//mcmh.h"

//task_emu
#include ".//task_emu//task_emu.h"


//misc
#include ".//misc//binEOD.h"
#include ".//misc//lrc.h"
#include ".//misc//timetools.h"
#include ".//misc//init.h"
#include ".//misc//time.h"
//#include ".//misc//md5.h"
#include ".//misc//km_api.h"

//#include ".//misc//uart.h"
//#include ".//misc//cbuf.h"


//shapi
#include ".//shapi//addr.h"
#include ".//shapi//sh_sam.h"
#include ".//shapi//sh_global.h"
#include ".//shapi//SH_CPU_TRANS.h"
#include ".//shapi//shtrans.h"
#include ".//shapi//shmt_bom_func.h"
#include ".//shapi//shmetro24G.h"
#include ".//shapi//serial24G.h"
//#include ".//shapi//cmd_class_general.h"
#include ".//shapi//shexpo.h"
#include ".//shapi//trans_245g.h"
#include ".//shapi//user_main.h"
#include ".//shapi//reset_rec.h"
//
#include ".//shapi//serial.h"
#include ".//shapi//bin_file_manage.h"
#include ".//shapi//qr.h"
#include ".//shapi//ble.h"
//
#include ".//shapi//sh_transport_operation.h"

#include ".//shapi//qr_jtb.h"
#include ".//shapi//qr_ssm.h"
#include ".//shapi//qr_stm.h"
#include ".//shapi//base64.h"
#include ".//shapi//sm.h"

#include ".//shapi//crc.h"

//comprocess
//#include ".//comprocess//prot_jt.h"
//#include ".//comprocess//cmd_dt.h"
//#include ".//comprocess//prot_dt.h"

#include ".//comprocess//prot_zte.h"

//#include ".//qr//qr_ydwl.h"
//#include ".//ble//ble_ydwl.h"


#include ".//qr//qr_fdwd.h"
#include ".//ble//ble_fdwd.h"

//xmodem
//#include ".//xmodem//xmodem.h"

//end of file
#endif