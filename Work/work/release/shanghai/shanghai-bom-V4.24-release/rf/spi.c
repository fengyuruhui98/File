//spi.c
//S3C2410��SPI��������
//���ƣ��˽���
//ʱ�䣺20080227

#ifndef _SPI_C_
#define _SPI_C_
//start of file


/*=====================================================================================
������spi_init
���ܣ�
=======================================================================================*/
void spi_init(void)
{
//RC522�������10M,PCLK=FCLK/4=50.7M
S3C2410SFR_SPPRE1 = 2;  //PCLK/2/(value+1) -> 50.7/6 = 8.45Mhz
//
S3C2410SFR_SPCON1 = 0x18;    //polling mode, enable sck, master,active high,format a,normal  
//
S3C2410SFR_SPPIN1 = 0x02;
//
return;
}

/*=====================================================================================
������spi_send_byte
���ܣ�
=======================================================================================*/
void spi_send_byte(UBYTE inbyte)
{
while(!(S3C2410SFR_SPSTA1&BIT00_MASK)){};
S3C2410SFR_SPTDAT1 = inbyte;
while(!(S3C2410SFR_SPSTA1&BIT00_MASK)){};
return;	
}	

/*=====================================================================================
������spi_rece_byte
���ܣ�
=======================================================================================*/
UBYTE spi_rece_byte(void)
{
while(!(S3C2410SFR_SPSTA1&BIT00_MASK)){};
S3C2410SFR_SPTDAT1 = 0xff;
while(!(S3C2410SFR_SPSTA1&BIT00_MASK)){};
return (S3C2410SFR_SPRDAT1);
}





//end of file
#endif



