//eeprom.h
//eeprom��������
//���ƣ�
//ʱ�䣺

#ifndef _EEPROM_H_
#define _EEPROM_H_
//start of file


//����----------------------------------------------------------------------------------------------------
UBYTE ee_write_lrc2(UWORD addr,UBYTE bytes,UBYTE *buf);
UBYTE ee_read_lrc2(UWORD addr,UBYTE bytes,UBYTE *buf);

//end of file
#endif
