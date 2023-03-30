//sh_sam.h


#ifndef _SH_SAM_H_
#define _SH_SAM_H_
//start of file

#define SAMLOCATION_7		7
#define SAMLOCATION_6		6
#define SAMLOCATION_5		5
#define SAMLOCATION_4 	4

#define ONLYKEYA				1
#define ONLYKEYB				2
#define KEYAANDB				3


extern UBYTE bgSHpsamIndex;
extern UBYTE bpgSHpsamNo[4];
extern UBYTE bpgSmpsamAppSjtFlag;
extern UBYTE bgSmpsamIndex;
extern UBYTE	bgSmpsamADF[2];					//
extern UBYTE bpgSmpsamNo[4];

extern UBYTE bpgSmpsamNu[6];

extern UBYTE bgSMobilepsamIndex;
extern UBYTE bpgSMobilepsamNo[6];

extern UBYTE bgSHPsamValid;
extern UBYTE bgSmPsamValid; 
extern UBYTE bgSMobilePsamValid;

extern UBYTE SHEXPOMODE;

extern UBYTE bgSHJTBpsamIndex;					//�Ϻ���ͨ��PSAM���
extern UBYTE ch_transport_psam_id[6];	//20190326 ��ͨ��SAM����

int ModifySamID(UBYTE *newid);
int ResetShangHaiCardSam(void);
int ResetShangHaiMetroSam(void);
int ResetShangHaiExpoSam(void);
int ResetShangHaiMobileSam(void);
UBYTE SHGetKeyAStep1(UBYTE *keybuf,UBYTE n);			//�Ϻ���������Կ
UBYTE SHGetKeyAStep2(void);
UBYTE SHTransTACStep1(UBYTE *tacbuf);							//�Ϻ�������TAC
UBYTE SHGetTacStep2(void);
UBYTE SMGetKeyAStep1(UBYTE *keybuf,UBYTE n);			//�Ϻ���������Ʊ��Կ
UBYTE SMGetKeyAStep2(void);
UBYTE SHMetroTACStep1(UBYTE *tacbuf);							//�Ϻ���������ƱTAC
UBYTE SMGetTacStep2(void);
UBYTE SHSendAuthResp(void);

UBYTE SEGetAuthCodeStep1(UBYTE *issuebuf);				//�������㷢����֤��
UBYTE SEGetKeyStep1(UBYTE *keybuf,UBYTE SectorNum,UBYTE mode);//������Կ����
UBYTE COMMONGetSAMStep2(void);										//ȡSAM���ݹ�������
int select_1001or1002_app(UBYTE bpgSmpsamAppSjt);

//end of file
#endif