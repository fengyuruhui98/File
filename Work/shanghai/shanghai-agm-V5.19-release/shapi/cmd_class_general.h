//cmd_class_general.h

#ifndef _CMD_CLASS_GENERAL_H_
#define _CMD_CLASS_GENERAL_H_

typedef void cmd_sub_func_type(UBYTE *,UWORD,UBYTE *,UWORD *); 


extern cmd_sub_func_type  *class_general_func[256];

#define READVERSION			0x00
//#define GETNODE					0x01
//#define SETNODE					0x02
#define READEEPROM			0x03
#define WRITEEEPROM			0x04
#define READFLASH				0x05
#define WRITEFLASH			0x06
#define ERASEPAGE				0x07
#define SOFTRESET				0x0b
#define SETSTATUS				0x0e
#define SELECTRF				0x15
#define WRFLASHBUF			0x1c
#define BUF2FLASH				0x1d
#define FLASH2BUF				0x1e
#define RDFLASHBUF			0x1f
#define LOAD_KEY				0x20
#define AUTH_KEY				0x21
#define CARD_REQUEST		0x22
#define ANTICOLL				0x23
#define SELECTCARD			0x24
#define READBLOCK				0x25
#define WRITEBLOCK			0x26
#define HALTCARD				0x27
#define DECREMENT				0x29
#define POWEROFF				0x2a
#define RESTORE					0x2b
#define TRANSFER				0x2c
#define ATR							0x30
#define T0COMMAND				0x31
#define DESEN						0x32
#define PSAMRESET				0x40
#define DISPSET					0x46
#define BUZZERSET				0x47
#define CARDINFO				0x4a
#define READTOCKEN			0x4d
#define WRITETOCKEN			0x4e
#define HREADBLOCK			0x50
#define HWRITEBLOCK			0x51
#define GETLAST_EXETIME	0x52
#define SETSAMSPEED			0x53
#define NHHORM1					0x54
#define GETRESPONSE			0x55
#define RESETKEY				0x56
#define GETKEY					0x57
#define CRC_CCITT				0x58
#define ZTE_CMD					0x59
#define DIVERSITY				0x5a
#define CALMAC					0x5b
#define KEKOPERATION		0x5c

#define FORMATFLASH			0x70
#define CREATFILE				0x71
#define RWFILE					0x72
#define DELFILE					0x73
#define ADD2FILE				0x74
#define MIFPRO_CLASS		0x7c
#define M4_CLASS				0x91

#define TESTRESP				0x92



#define SHEXPOISSUE			0xe0
#define SHEXPOREADINFO	0xe1
#define SHEXPOVERIFY		0xe2
#define SHEXPOINI				0xe3

#define DEBUG_CLASS                  0xff
#define DEBUG_GET_SYSTIME           0x00
#define DEBUG_GET_LAST_EXEC_TIME    0x01
#define DEBUG_DELAY                 0x02
#define DEBUG_RC_RD_BYTE            0x03
#define DEBUG_RC_WR_BYTE            0x04
#define DEBUG_RD_PORT               0x05
#define DEBUG_WR_PORT               0x06
#define DEBUG_GET_RESET_REC         0x07

UBYTE gCommandErr;

void class_general_ini(void);
void class_general(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);


void class_general_getexetime(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_getresponse(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_getkey(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
//#ifdef _TESTCARD_
void class_general_resetkey(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
//#endif
void class_general_setsam(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_nhhorm1(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_selectrf(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_watchdiversity(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_watchcalmac(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
//void class_general_crc(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_ZTEcmd(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_kekoperation(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);

void class_general(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_version(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_bad_cmd(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
//void class_general_getnode(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
//void class_general_setnode(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_setstatus(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_reset(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);


void class_general_rdeeprom(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_wreeprom(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_rdflash(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_wrflash(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_erasepage(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_wrflashbuf(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_buf2flash(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_flash2buf(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_rdflashbuf(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);


void class_general_desencode(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_poweroff(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_loadkey(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_authentication(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_request(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_anticoll(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_select(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_readblock(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_writeblock(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_halt(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_decrement(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_restore(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_transfer(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_hreadblock(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_hwriteblock(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_cardinfo(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
//void class_general_m4(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_mifproclass(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);

void class_general_atr(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_T0command(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_samreset(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_readtocken(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_writetocken(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);

void class_general_expoissue(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_exporeadinfo(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_exporeaderini(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_expoverify(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_expodispset(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
void class_general_expobuzzerset(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);

//void class_general_formatflash(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
//void class_general_creatfile(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
//void class_general_rwfile(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
//void class_general_delfile(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);
//void class_general_add2file(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);

void cmd_debug_process(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes);


#endif