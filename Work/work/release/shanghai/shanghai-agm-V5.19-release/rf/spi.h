//spi.h
//S3C2410��SPI��������
//���ƣ��˽���
//ʱ�䣺20080227

#ifndef _SPI_H_
#define _SPI_H_
//start of file

void spi_init(void);
void spi_send_byte(UBYTE inbyte);
UBYTE spi_rece_byte(void);

//end of file
#endif



