#ifndef _SHEXPO_H_
#define _SHEXPO_H_

#define ACK_EXPO_OK						0x00
#define ACK_EXPO_BADCOMMAND		0x02
#define ACK_EXPO_BADPARAM			0x03
#define ACK_EXPO_NOCARD 			0x11
#define ACK_EXPO_ANTICOLLERR	0x12
#define ACK_EXPO_AUTHERR			0x13
#define ACK_EXPO_INVALIDCARD  0x14
#define ACK_EXPO_WRCARDERR		0x15
#define ACK_EXPO_RDCARDERR		0x16
#define ACK_EXPO_SAMNOTEXIST	0x17
#define ACK_EXPO_PSAMERR			0x18

#define EM681_MAX_SECTOR  64

extern UBYTE Rand[10];
extern UBYTE IssueSn[5];
extern UBYTE AuthCode[10];
extern UBYTE IssueDep[2];
extern UBYTE gKeyACal[EM681_MAX_SECTOR][6];
extern UBYTE gKeyBCal[EM681_MAX_SECTOR][6];
extern UBYTE IssueGroup[6];
extern UBYTE IssueDev;
extern UBYTE IssueStatus;
extern UBYTE AppointTime[4];
extern UBYTE UseCount[4];
extern UBYTE BusCount[4];
extern UBYTE SurfaceSn[4];
extern UBYTE ShExpoCardSn[8];



UBYTE shexpo_cardissue(void);
UBYTE shexpo_cardinfo(UBYTE *outbuf);
UBYTE shexpo_cardverify(void);
UBYTE shexpo_calkey(UBYTE kid2,UBYTE sectnum,UBYTE KEYAB);
UBYTE shexpo_SNAuth(void);
UBYTE card_restore(void);
//#ifdef _TESTCARD_
UBYTE resetdefaultkey(void);
//#endif
#endif