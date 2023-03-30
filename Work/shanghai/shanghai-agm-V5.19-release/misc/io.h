//io.h

#ifndef _IO_H_
#define _IO_H_
//start of file

//常量定义*******************************************************************
#define LED_ON   0
#define LED_OFF  1

#define IO_INPUT   (UDWORD)(0)
#define IO_OUTPUT  (UDWORD)(1)

#define IO_PULLUP    0
#define IO_PULLNONE  1

#define IO_IIC  (UDWORD)(2)
#define IO_SPI  (UDWORD)(3)
#define IO_UART (UDWORD)(2)

//gpa
#define gpa_set_bit(bit_mask)   {wgGPAValue|=bit_mask;S3C2410SFR_GPADAT=wgGPAValue;}
#define gpa_clr_bit(bit_mask)   {wgGPAValue&=(UWORD)(~bit_mask);S3C2410SFR_GPADAT=wgGPAValue;}
//gpb
#define gpb_set_bit(bit_mask)   {wgGPBValue|=bit_mask;S3C2410SFR_GPBDAT=wgGPBValue;}
#define gpb_clr_bit(bit_mask)   {wgGPBValue&=(UWORD)(~bit_mask);S3C2410SFR_GPBDAT=wgGPBValue;}
//gpc
#define gpc_set_bit(bit_mask)   {wgGPCValue|=bit_mask;S3C2410SFR_GPCDAT=wgGPCValue;}
#define gpc_clr_bit(bit_mask)   {wgGPCValue&=(UWORD)(~bit_mask);S3C2410SFR_GPCDAT=wgGPCValue;}
//gpd
#define gpd_set_bit(bit_mask)   {wgGPDValue|=bit_mask;S3C2410SFR_GPDDAT=wgGPDValue;}
#define gpd_clr_bit(bit_mask)   {wgGPDValue&=(UWORD)(~bit_mask);S3C2410SFR_GPDDAT=wgGPDValue;}
//gpe
#define gpe_set_bit(bit_mask)   {wgGPEValue|=bit_mask;S3C2410SFR_GPEDAT=wgGPEValue;}
#define gpe_clr_bit(bit_mask)   {wgGPEValue&=(UWORD)(~bit_mask);S3C2410SFR_GPEDAT=wgGPEValue;}
//gpf
#define gpf_set_bit(bit_mask)   {wgGPFValue|=bit_mask;S3C2410SFR_GPFDAT=wgGPFValue;}
#define gpf_clr_bit(bit_mask)   {wgGPFValue&=(UWORD)(~bit_mask);S3C2410SFR_GPFDAT=wgGPFValue;}
//gpg
#define gpg_set_bit(bit_mask)   {wgGPGValue|=bit_mask;S3C2410SFR_GPGDAT=wgGPGValue;}
#define gpg_clr_bit(bit_mask)   {wgGPGValue&=(UWORD)(~bit_mask);S3C2410SFR_GPGDAT=wgGPGValue;}
//gph
#define gph_set_bit(bit_mask)   {wgGPHValue|=bit_mask;S3C2410SFR_GPHDAT=wgGPHValue;}
#define gph_clr_bit(bit_mask)   {wgGPHValue&=(UWORD)(~bit_mask);S3C2410SFR_GPHDAT=wgGPHValue;}
//
#define gpa_wr16(inword)        {wgGPAValue=inword;S3C2410SFR_GPADAT=wgGPAValue;}
#define gpb_wr16(inword)        {wgGPBValue=inword;S3C2410SFR_GPBDAT=wgGPBValue;}
#define gpc_wr16(inword)        {wgGPCValue=inword;S3C2410SFR_GPCDAT=wgGPCValue;}
#define gpd_wr16(inword)        {wgGPDValue=inword;S3C2410SFR_GPDDAT=wgGPDValue;}
#define gpe_wr16(inword)        {wgGPEValue=inword;S3C2410SFR_GPEDAT=wgGPEValue;}
#define gpf_wr16(inword)        {wgGPFValue=inword;S3C2410SFR_GPFDAT=wgGPFValue;}
#define gpg_wr16(inword)        {wgGPGValue=inword;S3C2410SFR_GPGDAT=wgGPGValue;}
#define gph_wr16(inword)        {wgGPHValue=inword;S3C2410SFR_GPHDAT=wgGPHValue;}
//
#define gpa_rd16()               S3C2410SFR_GPADAT
#define gpb_rd16()               S3C2410SFR_GPBDAT
#define gpc_rd16()               S3C2410SFR_GPCDAT
#define gpd_rd16()               S3C2410SFR_GPDDAT
#define gpe_rd16()               S3C2410SFR_GPEDAT
#define gpf_rd16()               S3C2410SFR_GPFDAT
#define gpg_rd16()               S3C2410SFR_GPGDAT
#define gph_rd16()               S3C2410SFR_GPHDAT
//
#define usb_reset_high()   gpd_set_bit(BIT02_MASK)    
#define usb_reset_low()    gpd_clr_bit(BIT02_MASK)    

//变量定义********************************************************************
extern UBYTE bgRledState,bgGledState;
extern UWORD wgGPAValue,wgGPBValue,wgGPCValue,wgGPDValue,wgGPEValue,wgGPFValue,wgGPGValue,wgGPHValue;

//函数定义********************************************************************
void io_init(void);
void rled(UBYTE option);
void gled(UBYTE option);


//end of file
#endif



