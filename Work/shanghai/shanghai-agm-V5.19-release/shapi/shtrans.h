//shtrans.h

#ifndef _SHTRANS_H_
#define _SHTRANS_H_



#define gCityCode0		0x20
#define gCityCode1		0x00

#define CNTCARD		   0x54			//�Ϻ��ƴο�
#define COMMCARD	   0x53			//�Ϻ���ͨ��
//�Ϻ� ticket family 2019-06-19
#define SH_TICKET_JTB		0x01	//��ͨ����

#define BOM_READER          0x13
#define GATE_ENTRY_READER   0x11
#define GATE_EXIT_READER    0x12

//Ӧ�����
#define CE_OK            0x00
#define CE_CHECKERROR    0x01
#define CE_BADCOMMAND    0x02
#define CE_BADPARAM      0x03
#define CE_STATUSERROR   0x04

#define CE_NOT_SAME_OPERATOR  0x05
#define CE_SAMERROR      0x06
#define CE_CARDSTATUSERROR 0x07

#define CE_NOCARD        0x08

#define CE_AUTHENERROR   0x10		//��֤ʧ�ܣ�����˫����֤��MACУ�����
#define CE_READERROR     0x11
#define CE_WRITEERROR    0x12		//д�����ϣ�����Ӧ����Ӧ�����
#define CE_CARDREMOVED   0x13		//���������п��ƶ������ڸ���Ӧ��������������ʱ��
#define CE_INVALIDCARD   0x14
#define CE_INVALIDFLAG   0x15
#define CE_NORECORD      0x16
#define CE_PUTBACKCARD1	 0x17
#define CE_PUTBACKCARD2	 0x18
#define CE_NCSCSAM 	     0x19
#define CE_STSAM 	       0x1a
#define CE_PSAMERROR 	   0x1b
#define CE_ISAMERROR 	   0x1c
#define CE_MAC2ERR		   0x20
#define CE_NOFUNCTION	   		0x21	//��Ƭ��֧�ִ˹���
#define CE_NOLOG			   0x24
#define CE_MAC1ERR		   0x25
#define	CE_FORBID_TICKET 0x33

#define CE_AUTHFALL      0x40
#define CE_NOAUTH        0x41
#define	CE_OVERTIME			 0x42
#define	CE_NO_ENTRY			 0x44
#define	CE_NOPARAM		   0x53
#define CE_NOKEY         0x60
#define CE_MACERR      	 0x61
#define CE_AUTHERR       0x62
#define CE_PINERROR      0x63
#define CE_MODIFYPIN     0x64

#define	CE_BLUENOADDRESS	0x50		//δ�ѵ�ָ����ַ�������㲥
#define CE_BLUEUNONLIE		0x51		//����ָ��������ַʧ��
#define CE_BLUEOVERTIME		0x52		//��������Ӧ�����ݳ�ʱ

#define	CE_QRERROR				0x71
                

#define	CE_CONTINUE_MULTIFRAME	0x92
#define	CE_TOTAL_FRAME			    0x93
#define	CE_FILE_LENGTH		    	0x94
#define	CE_MD5					        0x95
#define	CE_FORMAT				        0x96
#define	CE_NO_FILE			      	0x97
#define	CE_CRC					        0x98

#define CE_OK_PF_MOBILE         0x99

#define CE_READERERROR   0x70
#define CE_UNKNOWN       0x7f 

#define OP_ERR			0xff
#define MAX_TRY    		15

#define REC_ENTRY			0
#define REC_EXIT			1

//2013/11/18 10:51:39
#define CE_NO_PPSE 0x7c

//eeprom
#define ADDR_CHECK_B32	2

//Ӳ�����Ӧ����
#define TEST_REQUESTERR					0x01	//requestʧ��
#define TEST_ANTICOLLERR				0x02	//anticoll error
#define TEST_SELECTERR					0x03	//select error

#define TEST_SAMSELECTERR				0x10	//
#define TEST_SAMATRFUNERR				0x11	//atr����ʧ��
#define TEST_SAMATRRESERR				0x12	//atrӦ��ʧ��


//����
extern UBYTE gCPUflag;
extern UBYTE gRestoreFlag,gClrFlag;
extern UBYTE gB32[20],gB33[20], gB34[20];
extern UBYTE gB32BK[20],gB33BK[20];
extern UBYTE gBFirst;
extern UBYTE ChangeDate,gB29[20];
extern UBYTE save[11];	
extern UBYTE recordsum[3],recordcnt,recordtime[2];
extern UBYTE KeyBuf[60],gB29[20];

extern UBYTE hh_trans_id;
extern code UBYTE trans_id[14];
//
extern UBYTE gM1Size0x53Flag;

//2019-06-19
extern UBYTE sh_ticket_family;

UBYTE getcardinfo(void);
UBYTE trans_a(void);
UBYTE trans_b(void);
UBYTE trans_c(void);
UBYTE trans_d(void);
UBYTE trans_a2(void);
UBYTE trans_b2(void);
UBYTE trans_c2(void);
UBYTE trans_d2(void);
UBYTE trans_ms(void);
UBYTE trans_entry(void);
UBYTE trans_exit(UBYTE offsetnum);
UBYTE trans_lock(UBYTE lock_type);
UBYTE trans_readrecord(UBYTE *record_buf);
UBYTE trans_decrement(void);
UBYTE trans_update(UBYTE update_type,UBYTE *rec_buf);
void trans_gen_card_record(UBYTE record_type,UBYTE *gCardRecord);
UBYTE hh_getcardinf(UBYTE ID);
UBYTE hh_trans_sale(void);
UBYTE hh_trans_entry(void);
UBYTE hh_trans_entry2(UBYTE b14com);
UBYTE hh_trans_update(UBYTE *rec_buf);
UBYTE get_metro_rec(UBYTE *record_buf);
UBYTE checkrec_inmemory(UBYTE type);
#endif