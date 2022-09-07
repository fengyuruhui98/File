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

extern UBYTE bgSHJTBpsamIndex;					//上海交通部PSAM编号
extern UBYTE ch_transport_psam_id[6];	//20190326 交通部SAM卡号

int ModifySamID(UBYTE *newid);
int ResetShangHaiCardSam(void);
int ResetShangHaiMetroSam(void);
int ResetShangHaiExpoSam(void);
int ResetShangHaiMobileSam(void);
UBYTE SHGetKeyAStep1(UBYTE *keybuf,UBYTE n);			//上海公交卡密钥
UBYTE SHGetKeyAStep2(void);
UBYTE SHTransTACStep1(UBYTE *tacbuf);							//上海公交卡TAC
UBYTE SHGetTacStep2(void);
UBYTE SMGetKeyAStep1(UBYTE *keybuf,UBYTE n);			//上海地铁单程票密钥
UBYTE SMGetKeyAStep2(void);
UBYTE SHMetroTACStep1(UBYTE *tacbuf);							//上海地铁单程票TAC
UBYTE SMGetTacStep2(void);
UBYTE SHSendAuthResp(void);

UBYTE SEGetAuthCodeStep1(UBYTE *issuebuf);				//世博计算发行认证码
UBYTE SEGetKeyStep1(UBYTE *keybuf,UBYTE SectorNum,UBYTE mode);//世博密钥计算
UBYTE COMMONGetSAMStep2(void);										//取SAM数据公共步骤
int select_1001or1002_app(UBYTE bpgSmpsamAppSjt);

//end of file
#endif