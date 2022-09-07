//fm222.h

#ifndef _FM222_H_
#define _FM222_H_
//start of file

#define FM222_CMD_CARDTYPE 0x40	
#define FM222_CMD_REQUEST  0x41	
#define FM222_CMD_ANTICOLL 0x42	
#define FM222_CMD_SELECT   0x43
#define FM222_CMD_AUTHTICATION  0x44
#define FM222_CMD_HALT     0x45
#define FM222_CMD_READ     0x46	
#define FM222_CMD_WRITE	   0x47
#define FM222_CMD_INCREMENT  0x48
#define FM222_CMD_DECREMENT  0x49	
#define FM222_CMD_RESTORE    0x4A	
#define FM222_CMD_TRANSFER   0x4B
#define FM222_CMD_LOADKEY    0x4C	
#define FM222_CMD_POWER      0xC7

#define FM222_CMD_GET_RANDOM     0xC0
#define FM222_CMD_GET_KEY        0xC1

#define FM222_CMD_HL_ACTIVE     0x36
#define FM222_CMD_HL_AUTH       0x37
#define FM222_CMD_HL_READ       0x33
#define FM222_CMD_HL_TOKEN_READ_NO_AUTH       0x3a
#define FM222_CMD_HL_TOKEN_READ       0x57
#define FM222_CMD_HL_TOKEN_WRITE      0x59
#define FM222_CMD_HL_WRITE      0x32
#define FM222_CMD_INIT_VALUE      0x56
#define FM222_CMD_HL_INIT_VALUE      0x34
#define FM222_CMD_READ_VALUE         0x58
#define FM222_CMD_HL_READ_VALUE      0x35
#define FM222_CMD_HL_INCREMENT  0x31
#define FM222_CMD_HL_DECREMENT  0x30
#define FM222_CMD_RATS          0x74
#define FM222_CMD_CARD_VALUE_CHECK  0x38
#define FM222_CMD_DATA_RESTORE      0xc8
#define FM222_CMD_GET_TRANS_RESULT  0xcb
#define FM222_CMD_CALC_TAC          0xc2
#define FM222_CMD_CALC_MAC          0xc3
#define FM222_CMD_CHECK_MAC         0xc4
#define FM222_CMD_DATA_ENCRYPT      0xc5
#define FM222_CMD_DATA_DECRYPT      0xc6
#define FM222_CMD_GET_MODULE_NO     0xc9 //module no and hardware version
#define FM222_CMD_UPDATE_MODULE_NO  0xca
#define FM222_CMD_GET_MODULE_SWVER  0xcc //module software version

#define FM222_CMD_RATS   0x74
#define FM222_CMD_PPS    0x75
#define FM222_CMD_IBLOCK    0x6f
#define FM222_CMD_DESELECT  0x66
#define FM222_CMD_APDU_ACCESS 0xb6



#define ISO14443A_M1_TYPE    0        
#define ISO14443A_SH_TYPE    1
#define ISO14443B_M4_TYPE    2
#define ISO15693_ICODE1_TYPE 3
#define ISO14443B_TYPE       4

#define AUTH_MODE_EXT_KEYA   0    //外部
#define AUTH_MODE_EXT_KEYB   1

#define AUTH_MODE_INT_KEYA   4    //内部
#define AUTH_MODE_INT_KEYB   5

//函数-------------------------------------------------------------------------
UBYTE fm222_is_ok(void);
UBYTE fm222_transceve(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes,UWORD time_out);
UBYTE fm222_transceve_mingwenmac(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes,UWORD time_out);
UBYTE fm222_transceve_miwen(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes,UWORD time_out);
UBYTE fm222_transceve_miwenmac(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes,UWORD time_out);

//UBYTE set_card_type(UBYTE card_type);
//UBYTE mcml_request(UBYTE *atq);
//UBYTE mcml_anticoll(UBYTE *snr);
//UBYTE mcml_select(UBYTE *snr,UBYTE *status);
//UBYTE mcml_load_key(UBYTE keyset,UBYTE keyab, UBYTE sectno,UBYTE *buf);
UBYTE mcml_load_key_miwen_mode(UBYTE *inkey);
//UBYTE mcml_authentication(UBYTE keyset,UBYTE keyab,UBYTE sectno);
//UBYTE mcml_read(UBYTE block,UBYTE *outbuf);
UBYTE mcml_read_return_mac(UBYTE block,UBYTE *obuf,UBYTE *mac);
//UBYTE mcml_write(UBYTE block,UBYTE *outbuf);
UBYTE mcml_write_mac_mode(UBYTE block,UBYTE *ibuf,UBYTE *obuf);
//UBYTE mcml_increment(UBYTE addr, UDWORD value);
//UBYTE mcml_decrement(UBYTE addr,UDWORD value);
//UBYTE mcml_transfer(UBYTE addr);
//UBYTE mcml_restore(UBYTE addr);
//UBYTE mcml_pwr_off(void);
//UBYTE mcml_pwr_on(void);
//UBYTE mcml_halt(void);

//组合命令
UBYTE hl_active(UBYTE req_mode,UBYTE req_cnt,UBYTE *snr,UBYTE *app_info);
UBYTE hl_auth(UBYTE key_set,UBYTE sectno,UBYTE *inkey);
UBYTE hl_read(UBYTE auth_mode,UBYTE block,UBYTE *obuf);
UBYTE hl_token_read_no_auth(UBYTE block,UBYTE *obuf,UBYTE *uid);
UBYTE hl_token_read(UBYTE auth_mode,UBYTE block,UBYTE *obuf,UBYTE *uid);
UBYTE hl_token_write(UBYTE auth_mode,UBYTE block,UBYTE *inbuf);
UBYTE hl_write(UBYTE auth_mode,UBYTE block,UBYTE *inbuf);
UBYTE init_value(UBYTE block,UDWORD value);
UBYTE hl_init_value(UBYTE auth_mode,UBYTE block,UDWORD value);
UBYTE read_value(UBYTE block,UDWORD *value);
UBYTE hl_read_value(UBYTE auth_mode,UBYTE block,UDWORD *value);
UBYTE hl_increment(UBYTE auth_mode,UBYTE addr, UDWORD value);
UBYTE hl_decrement(UBYTE auth_mode,UBYTE addr,UDWORD value);
UBYTE hl_get_random(UBYTE mode,UBYTE *obuf);
UBYTE hl_get_key(UBYTE *inbuf);
UBYTE hl_card_value_check(UDWORD value,UBYTE *mac);
UBYTE hl_data_restore(UBYTE block);
UBYTE hl_get_trans_result(UBYTE *trans_time,UBYTE *tac);
UBYTE hl_cal_tac(UBYTE key_index,UBYTE *ibuf,UBYTE ibytes,UBYTE *tac);
UBYTE hl_cal_mac(UBYTE key_index,UBYTE *inbuf,UBYTE inbytes,UBYTE *tac);
UBYTE hl_check_mac(UBYTE key_index,UBYTE *inbuf,UBYTE inbytes);
UBYTE hl_data_encrypt(UBYTE key_index,UBYTE *inbuf,UBYTE inbytes,UBYTE *obuf);
UBYTE hl_data_decrypt(UBYTE key_index,UBYTE *inbuf,UBYTE inbytes,UBYTE *obuf);
UBYTE hl_get_module_no(UBYTE *obuf);
UBYTE hl_get_module_software_version(UBYTE *obuf);
UBYTE hl_update_module_no(UBYTE *ibuf,UBYTE *resp);

UBYTE iso_rats(UBYTE cid,UBYTE *obuf);
UBYTE iso_pps(UBYTE *ibuf,UBYTE *obuf);
UBYTE iso_iblock(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *obytes);
UBYTE iso_apdu_access(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *obytes);	
UBYTE iso_deselect(UBYTE cid);





//end of file
#endif





