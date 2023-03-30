//SH_TRANSPORT_OPERATION.h
#ifndef _SH_TRANSPORT_OPERATION_H_
#define _SH_TRANSPORT_OPERATION_H_

#define	TRANSPORT_1A_LEN			128
#define TRANSPORT_1E_LEN			48

#define	SH_CODE_CITY				"\x29\x00"	//���д���
#define	SH_CODE_ORGANIZATION		"\x02\x00\x29\x00\xff\xff\xff\xff"	//����������ʶ
#define	HANDLE_CODE_ORGANIZATION		"\x12\x00\x29\x00\xff\xff\xff\xff"	//����������ʶ


extern unsigned char transport_15_data[30];		//����Ӧ����Ϣ�ļ�
extern unsigned char transport_16_data[55];		//�ֿ��˻�����Ϣ�ļ�
extern unsigned char transport_17_data[60];		//������Ϣ�ļ�
extern unsigned char transport_1A_data[128];		//������ͨ������Ϣ�䳤��¼�ļ�
extern unsigned char transport_1E_data[48];		//������ͨ������Ϣѭ����¼�ļ�
extern unsigned char transport_balance[4];
extern unsigned char capp_init[19];			//init capp purcahse return
extern unsigned char capp_debit[8];			//debit capp purchase return
extern unsigned long CRC32_Value;
extern UBYTE temp_crc[4];

unsigned short Transport_GetFiles15(unsigned char *out_buf);
unsigned short Transport_GetFiles16(unsigned char *out_buf);
unsigned short Transport_GetFiles17(unsigned char *out_buf);
unsigned short Transport_GetFiles1E(char record, unsigned char *out_buf);

UBYTE sh_transport_get_info(unsigned char *data);
char sh_transport_entry(void);
char sh_transport_exit(UBYTE offsetnum);
char sh_transport_update(UBYTE update_type,UBYTE *rec_buf);
char sh_transport_lockcard(UBYTE lock_type);

unsigned char init_for_capp_purchase(UBYTE key_index, UDWORD transvalue, UBYTE *psam_id,UBYTE *outbuf);
unsigned char debit_for_capp_purchase(UBYTE *sam_sn, UBYTE *outbuf);
unsigned char credit_sam_for_purchase(UBYTE bgSHJTBpsamIndex, UBYTE *mac2, UBYTE *outbuf);
unsigned char cpu_cal_protect_mac(unsigned char sam_index,unsigned char *key, unsigned char *factor, unsigned char *in_data, unsigned char in_len, unsigned char *mac);


#endif