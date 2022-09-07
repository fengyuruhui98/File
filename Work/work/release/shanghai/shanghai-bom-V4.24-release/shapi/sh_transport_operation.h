//SH_TRANSPORT_OPERATION.h
#ifndef _SH_TRANSPORT_OPERATION_H_
#define _SH_TRANSPORT_OPERATION_H_

#define SH_TICKET_JTB		0x01	//交通部卡


#define	TRANSPORT_1A_LEN			128
#define TRANSPORT_1E_LEN			48

#define	SH_CODE_CITY				"\x29\x00"
#define	SH_CODE_ORGANIZATION		"\x02\x00\x29\x00\xff\xff\xff\xff"
#define	HANDLE_CODE_ORGANIZATION	"\x12\x00\x29\x00\xff\xff\xff\xff"	//受理方机构标识

UBYTE sh_ticket_family;

extern unsigned char transport_15_data[30];		//公共应用信息文件
extern unsigned char transport_16_data[55];		//持卡人基本信息文件
extern unsigned char transport_17_data[60];		//管理信息文件
extern unsigned char transport_1A_data[128];		//公共交通过程信息变长记录文件
extern unsigned char transport_1E_data[48];		//公共交通过程信息循环记录文件
extern unsigned char transport_balance[4];
extern unsigned char capp_init[19];			//init capp purcahse return
extern unsigned char capp_debit[8];			//debit capp purchase return


unsigned short Transport_GetFiles15(unsigned char *out_buf);
unsigned short Transport_GetFiles16(unsigned char *out_buf);
unsigned short Transport_GetFiles17(unsigned char *out_buf);
unsigned short Transport_GetFiles1E(char record, unsigned char *out_buf);

UBYTE sh_transport_get_info(unsigned char *data);
char sh_transport_entry(void);
char sh_transport_exit(UBYTE offsetnum);
char sh_transport_update(UBYTE update_type,UBYTE *rec_buf);
char sh_transport_lockcard(UBYTE update_type);

unsigned char cpu_cal_protect_mac(unsigned char sam_index,unsigned char *key, unsigned char *factor, unsigned char *in_data, unsigned char in_len, unsigned char *mac);

#endif