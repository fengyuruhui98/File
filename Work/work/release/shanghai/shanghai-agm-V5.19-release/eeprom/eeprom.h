//eeprom.h
//eeprom操作程序
//编制：
//时间：

#ifndef _EEPROM_H_
#define _EEPROM_H_
//start of file


//函数----------------------------------------------------------------------------------------------------
UBYTE ee_write_lrc2(UWORD addr,UBYTE bytes,UBYTE *buf);
UBYTE ee_read_lrc2(UWORD addr,UBYTE bytes,UBYTE *buf);

//end of file
#endif
