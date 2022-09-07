//include.c
#ifndef _INCLUDE_C_
#define _INCLUDE_C_

#define	DEBUG_PRINT	1
//start of file
//#include ".//libjr1004//sha.c"
//#include ".//libjr1004//rsa.c"
//#include ".//libjr1004//utilities.c"
//#include ".//libjr1004//jrt0025_12.c"
//des
#include ".//des//fastdes.c"
#include ".//des//test_hard.c"

//eeprom
#include ".//eeprom//eeprom.c"

//rf
#include ".//rf//mcmh.c"

//task_emu
#include ".//task_emu//task_emu.c"

//misc
#include ".//misc//lrc.c"
#include ".//misc//timetools.c"
#include ".//misc//init.c"
#include ".//misc//time.c"
//#include ".//misc//md5.c"
#include ".//misc//km_api.c"

//#include ".//misc//uart.c"
//#include ".//misc//cbuf.c"

//shapi
#include ".//shapi//sh_sam.c"
#include ".//shapi//sh_global.c"
#include ".//shapi//SH_CPU_TRANS.c"
#include ".//shapi//shtrans.c"
#include ".//shapi//shmt_bom_func.c"
#include ".//shapi//shmetro24G.c"
#include ".//shapi//serial24G.c"
//#include ".//shapi//cmd_class_general.c"
#include ".//shapi//shexpo.c"
#include ".//shapi//trans_245g.c"
#include ".//shapi//user_main.c"
#include ".//shapi//reset_rec.c"
//
#include ".//shapi//serial.c"
#include ".//shapi//bin_file_manage.c"
#include ".//shapi//qr.c"
#include ".//shapi//ble.c"
#include ".//shapi//crc.c"
#include ".//shapi//qr_jtb.c"
#include ".//shapi//qr_ssm.c"
#include ".//shapi//qr_stm.c"
#include ".//shapi//base64.c"
#include ".//shapi//sm.c"

#include ".//shapi//sh_transport_operation.c"

//comprocess
//#include ".//comprocess//prot_jt.c"
//#include ".//comprocess//cmd_dt.c"
//#include ".//comprocess//prot_dt.c"

#include ".//comprocess//prot_zte.c"

//#include ".//qr//qr_ydwl.c"
//#include ".//ble//ble_ydwl.c"

#include ".//qr//qr_fdwd.c"
#include ".//ble//ble_fdwd.c"

#include ".//shapi//bom_func.c"
#include ".//shapi//fm222.c"
#include ".//shapi//fail.c"


//end of file
#endif