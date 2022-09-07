//SH_CPU_TRANS.h
#ifndef _SH_CPU_TRANS_H_
#define _SH_CPU_TRANS_H_

#define OP_ERR		0xff
#define M1LOCKED	0x10

extern UBYTE gPosID[6];
extern UBYTE gKeyVersion;
extern UBYTE gAppSn[10];
extern UBYTE gYaJin;
extern UBYTE gKeyBacon;
extern UBYTE gAluBacon;				//算法标志
extern UBYTE ICC[4];						//伪随机数
extern UDWORD gPurseOffset;
extern UBYTE gCPUValid;
extern UBYTE gNotShCard;
extern UBYTE gPSAMFileFlag;
extern UBYTE gCityCode[2];
extern UBYTE gRecCpu0017[48];

extern UBYTE gPPSMode;

//2018/07/30
extern UBYTE gODAData[100];

extern UBYTE JTBData[20];

char getcardinfo_CPU(void);
char trans_entry_cpu(void);
char trans_exit_cpu(UBYTE offsetnum);
char trans_lock_cpu(UBYTE *mac2,UBYTE lock_level);
char history_record(UWORD index,UBYTE *outbuf);
char trans_decrement_cpu(void);
char trans_update_cpu(UBYTE update_type,UBYTE *rec_buf);
char get_record(UBYTE SFI_index,UBYTE rec_index,UBYTE len,UBYTE *rec_buf);
char write_record(UBYTE SFI_index,UBYTE rec_index,UBYTE len,UBYTE *rec_buf,UBYTE cycleflag);
//
char write_capp_record(UBYTE SFI_index,UBYTE rec_index,UBYTE len,UBYTE *rec_buf,UBYTE cycleflag);
char get_capp_record(UBYTE SFI_index,UBYTE rec_index,UBYTE len,UBYTE *rec_buf);
//pf
char pf_mobile_ticket_process(void);


#endif