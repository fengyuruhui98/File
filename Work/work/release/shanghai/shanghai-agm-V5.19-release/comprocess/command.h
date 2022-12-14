//command.h
//命令处理器:基于研发部协议
//编制:邓建华
//时间:20080706

#ifndef _COMMAND_H_
#define _COMMAND_H_
//start of file

/*---------------------------------------------------------------------------------------------
系统命令区
----------------------------------------------------------------------------------------------*/
#define CMD_REPORT_VER   0x00      //系统版本报告
#define CMD_SET_STATUS   0x0e      //设置设备状态 
#define CMD_RESET        0x0b      //系统热启动
#define CMD_GET_NODE     0x01      //取设备的节点号
#define CMD_SET_NODE     0x02

/*---------------------------------------------------------------------------------------------
EEPROM操作
-----------------------------------------------------------------------------------------------*/
#ifndef _NO_EE_
#define CMD_RD_EE        0x03
#define CMD_WR_EE        0X04
#endif

/*---------------------------------------------------------------------------------------------
FLASH操作
----------------------------------------------------------------------------------------------*/
#ifndef _NO_FLASH_
#define CMD_RD_FLASH           0x05
#define CMD_WR_FLASH           0x06
#define CMD_ERASE_FLASH        0x07 
#define CMD_WR_FLASH_BUF       0x1c
#define CMD_FLASH_BUF_TO_MAIN  0x1d
#define  CMD_FLASH2BUF 	       0x1E
#define  CMD_READ_FLASHBUF     0x1F
#endif

/*---------------------------------------------------------------------------------------------
record操作
----------------------------------------------------------------------------------------------*/
//#define CMD_REC_GET_FORWARD    0x08
//#define CMD_REC_CLR_N          0x09
//#define CMD_REC_GET_BACKWARD   0x12
//#define CMD_REC_GET_NUM        0x16
//#define CMD_REC_SAVE           0x0c

/*-----------------------------------------------------------------------------------------------
实时时钟操作
------------------------------------------------------------------------------------------------*/
#define CMD_WR_TIME            0x14
#define CMD_RD_TIME            0x13 

/*---------------------------------------------------------------------------------------------
m1卡操作(0x20-0x2f)
-----------------------------------------------------------------------------------------------*/
#define CMD_MIF_START_CMD    0x20
#define CMD_MIF_END_CMD      0x2c
#define CMD_REQUEST          0x22
#define CMD_ANTICOLL         0x23
#define CMD_SELECT           0x24
#define CMD_LOAD_KEY         0x20
#define CMD_AUTHENTICATION   0x21
#define CMD_RD_BLOCK         0x25 
#define CMD_WR_BLOCK         0x26   
#define CMD_INCREASE         0x28 
#define CMD_DECREASE         0x29
#define CMD_RESTORE          0x2b 
#define CMD_TRANSFER         0x2c
#define CMD_HALT             0x27 
#define CMD_PWR_OFF          0x2a

/*-----------------------------------------------------------------------------------------------
CPU卡操作(ETU=372)
-------------------------------------------------------------------------------------------------*/
#define CMD_CPU_ATR   0x30          //ATR 
#define CMD_CPU_T0    0x31          //T0命令

/*-----------------------------------------------------------------------------------------------
按键操作
-------------------------------------------------------------------------------------------------*/
//#define CMD_KEY_CLASS   0x42
#define CMD_KEY_CLR     0x01
#define CMD_COMPUTE_TAC  0x4a 

//#define VOICE_CLASS     0x47

/*-----------------------------------------------------------------------------------------------
DES
-------------------------------------------------------------------------------------------------*/
#define CMD_DES_ENCODE  0x70
#define CMD_DES_DECODE  0x71


/*-----------------------------------------------------------------------------------------
Mifare pro类
-------------------------------------------------------------------------------------------*/
#define CMD_MIFPRO_CLASS          0x7c
#define CMD_MIFPRO_ATS            0x00 
#define CMD_MIFPRO_DESELECT       0x01
#define CMD_MIFPRO_ICMD_NOCHAIN   0x02
#define CMD_MIFPRO_ICMD_CHAIN     0x03
#define CMD_MIFPRO_WTX            0x04
#define CMD_MIFPRO_RBLOCK         0x05
#define CMD_CRC_A                 0x06
#define CMD_MIFPRO_ICMD           0x07
#define CMD_MIFPRO_NOACK          0x08

/*-----------------------------------------------------------------------------------------
DL CLASS
-------------------------------------------------------------------------------------------*/
#define CMD_NJAFC_CLASS      0x91
//#define CMD_M400_INIT        0x01
//#define CMD_M400_REQ         0x02
//#define CMD_M400_CMD         0x03
//#define CMD_M400_POLL        0x04
#define CMD_SET_CRYPT        0x07
//#define CMD_DISP_STRING          0x1c        //显示字符串 
//#define CMD_DISP_STRING_ENLARGE  0x1d
//#define CMD_DISP_IMAGE           0x1e         
//#define CMD_LCD_CLR              0x1f        //清屏
//#define CMD_SET_HZ_MODE          0x20  


#define ISO14443A_M1_TYPE    0        
#define ISO14443A_SH_TYPE    1
#define ISO14443B_M4_TYPE    2
#define ISO15693_ICODE1_TYPE 3



/*------------------------------------------------------------------------------------
ISO15693 命令集
--------------------------------------------------------------------------------------*/
//#define CMD_ISO15693_SET    0x00
//#define CMD_ISO15693_CMD    0x01

//#define DISP_CLASS  0xa1

/*------------------------------------------------------------------------------------
ISO15693 命令集
--------------------------------------------------------------------------------------*/
//#define SHIP_CLASS   0xa2

/*---------------------------------------------------------------------------------------------
DEBUG操作: FF FF
----------------------------------------------------------------------------------------------*/
#define DEBUG_CLASS                 0xff
#define DEBUG_GET_SYSTIME           0x00
#define DEBUG_GET_LAST_EXEC_TIME    0x01
#define DEBUG_DELAY                 0x02
#define DEBUG_RC_RD_BYTE            0x03
#define DEBUG_RC_WR_BYTE            0x04
#define DEBUG_RD_PORT               0x05
#define DEBUG_WR_PORT               0x06
#define DEBUG_CHG_BAUD              0x07
//#define DEBUG_SAM                   0x08
//#define DEBUG_LCD_CLASS             0x09
//#define DEBUG_ADC                   0x0a
//#define DEBUG_PRN_CLASS             0x0b
//#define DEBUG_GPRS_POWER            0x0c
#define DEBUG_GPRS_PUT              0x0d


#define DEBUG_TEMP                  0xff


/*---------------------------------------------------------------------------------------------
LCD操作
----------------------------------------------------------------------------------------------*/
#define CMD_LCD_CLR2                 0x00
#define CMD_LCD_BLACK               0x01
//#define CMD_LCD_DISP_1616           0x02
//#define CMD_LCD_DISP_1616_ENLARGE   0x03
//#define CMD_LCD_DISP_1612           0x04
//#define CMD_LCD_DISP_1612_ENLARGE   0x05
//#define CMD_LCD_DISP_1608           0x06
//#define CMD_LCD_DISP_1608_ENLARGE   0x07
#define CMD_LCD_DISP_STRING         0x08 
#define CMD_LCD_DISP_STRING_ENLARGE 0x09
#define CMD_LCD_DISP_IMAGE          0x0a
#define CMD_LCD_BLANK_LINE          0x0b
#define CMD_LCD_WR_CMD              0x0c
//#define CMD_LCD_WR_DATA             0x0d
#define CMD_LCD_RST                 0x0e


/*---------------------------------------------------------------------------------------------
PRN操作
----------------------------------------------------------------------------------------------*/
#define CMD_PRN_TO_HOME             0x00
#define CMD_PRN_FEED                0x01
#define CMD_PRN_GET_STATE           0x02
#define CMD_PRN_STRING              0x03
#define CMD_PRN_TEST                0x04
#define CMD_PRN_SET_POS             0x05


//函数------------------------------------------------------------------------------------------
void jt_cmd_process(UBYTE *inbuf,UBYTE inbytes,UBYTE *outuf,UBYTE *outbytes);
void cmd_debug_process(UBYTE *inbuf,UBYTE inbytes,UBYTE *outuf,UBYTE *outbytes);
void cmd_mifare_process(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes);
void cmd_njafc_process(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes);
void cmd_iso15693_process(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes);
void cmd_flash_process(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes);
void cmd_mifpro_process(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes);
void cmd_debug_sam(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes);
void cmd_lcd_process(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes);
void cmd_debug_prn(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes);
void psd_dl_proc(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes);
void cmd_ship_process(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes);

//end of file
#endif

