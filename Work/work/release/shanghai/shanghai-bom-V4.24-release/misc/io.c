//io.c

#ifndef _IO_C_
#define _IO_C_
//start of file

UBYTE bgRledState,bgGledState;
UWORD wgGPAValue,wgGPBValue,wgGPCValue,wgGPDValue,wgGPEValue,wgGPFValue,wgGPGValue,wgGPHValue;

/*==============================================================================
函数：io_init
功能：
================================================================================*/
void io_init(void)
{
/*----------------------------------------------------
GPA:address and GCS
保持复位设置,不要改变
------------------------------------------------------*/

/*----------------------------------------------------
GPB:
0,1,2,3,4,7,8,9,10:no use -> input, pull up
5,6:output  -> 5:522ss1  6:522ss2
其他：芯片未引出
------------------------------------------------------*/
S3C2410SFR_GPBCON = (IO_INPUT<<0)|(IO_INPUT<<2)|(IO_INPUT<<4)|(IO_INPUT<<6)        
                    |(IO_OUTPUT<<8)|(IO_OUTPUT<<10)|(IO_OUTPUT<<12)|(IO_OUTPUT<<14)
                    |(IO_INPUT<<16)|(IO_INPUT<<18)|(IO_INPUT<<20);
S3C2410SFR_GPBUP = (IO_PULLUP<<0)|(IO_PULLUP<<1)|(IO_PULLUP<<2)|(IO_PULLUP<<3)
                    |(IO_PULLUP<<4)|(IO_PULLUP<<5)|(IO_PULLUP<<6)|(IO_PULLUP<<7)
                    |(IO_PULLUP<<8)|(IO_PULLUP<<9)|(IO_PULLUP<<10);
gpb_wr16(0xffff);

/*-----------------------------------------------------                    
GPC:
0,1,2,3,4,5,6,7,14:no use -> input,pull up
8:out ,FMWP
9:out, TSCL
10:out,COMSEL
11:out,PWRCTL
12:out,MRST1
13:out,TSDA
15:out,MRST2
-------------------------------------------------------*/
S3C2410SFR_GPCCON = (IO_INPUT<<0)|(IO_INPUT<<2)|(IO_INPUT<<4)|(IO_INPUT<<6)           
                    |(IO_INPUT<<8)|(IO_INPUT<<10)|(IO_INPUT<<12)|(IO_INPUT<<14)          
                    |(IO_OUTPUT<<16)|(IO_OUTPUT<<18)|(IO_OUTPUT<<20)|(IO_OUTPUT<<22)    
                    |(IO_OUTPUT<<24)|(IO_OUTPUT<<26)|(IO_INPUT<<28)|(IO_OUTPUT<<30);
S3C2410SFR_GPCUP =  (IO_PULLUP<<0)|(IO_PULLUP<<1)|(IO_PULLUP<<2)|(IO_PULLUP<<3)        
                    |(IO_PULLUP<<4)|(IO_PULLUP<<5)|(IO_PULLUP<<6)|(IO_PULLUP<<7)       
                    |(IO_PULLNONE<<8)|(IO_PULLNONE<<9)|(IO_PULLNONE<<10)|(IO_PULLNONE<<11)     
                    |(IO_PULLNONE<<12)|(IO_PULLNONE<<13)|(IO_PULLUP<<14)|(IO_PULLNONE<<15);
gpc_wr16(0xffff);
 
/*------------------------------------------------------ 
GPD:
0:out,GLED
1:out,RLED
2:out,USBRST
3:out,OUT1
4:out,OUT2
5:out,OUT3
6:input,IN1,pull up
7:input,IN2,pull up
8:OUT,522RST1
9:OUT,522RST2
10,11,12,13,14,15:no use, input,pullup
----------------------------------------------------------*/
S3C2410SFR_GPDCON = (IO_OUTPUT<<0)|(IO_OUTPUT<<2)|(IO_OUTPUT<<4)|(IO_OUTPUT<<6)            
                    |(IO_OUTPUT<<8)|(IO_OUTPUT<<10)|(IO_INPUT<<12)|(IO_INPUT<<14)             
                    |(IO_OUTPUT<<16)|(IO_OUTPUT<<18)|(IO_INPUT<<20)|(IO_INPUT<<22)           
                    |(IO_INPUT<<24)|(IO_INPUT<<26)|(IO_INPUT<<28)|(IO_INPUT<<30);
S3C2410SFR_GPDUP =  (IO_PULLNONE<<0)|(IO_PULLNONE<<1)|(IO_PULLNONE<<2)|(IO_PULLNONE<<3)    
                    |(IO_PULLNONE<<4)|(IO_PULLNONE<<5)|(IO_PULLUP<<6)|(IO_PULLUP<<7)       
                    |(IO_PULLNONE<<8)|(IO_PULLNONE<<9)|(IO_PULLUP<<10)|(IO_PULLUP<<11)     
                    |(IO_PULLUP<<12)|(IO_PULLUP<<13)|(IO_PULLUP<<14)|(IO_PULLUP<<15);
gpd_wr16(0xffff);

/*----------------------------------------------------------
GPE:
0,1,2,3,4,5,6,7,8,9,10,11,12,13:nouse,pullup
14:IICSCL->OUTPUT,FMSCL
15:IICSDA->INPUT,FMSDA 
------------------------------------------------------------*/
S3C2410SFR_GPECON =(IO_INPUT<<0)|(IO_INPUT<<2)|(IO_INPUT<<4)|(IO_INPUT<<6)      
                  |(IO_INPUT<<8)|(IO_INPUT<<10)|(IO_INPUT<<12)|(IO_INPUT<<14)               
                  |(IO_INPUT<<16)|(IO_INPUT<<18)|(IO_INPUT<<20)|(IO_INPUT<<22)  
                  |(IO_INPUT<<24)|(IO_INPUT<<26)|(IO_OUTPUT<<28)|(IO_INPUT<<30);
                  
S3C2410SFR_GPEUP =  (IO_PULLUP<<0)|(IO_PULLUP<<1)|(IO_PULLUP<<2)|(IO_PULLUP<<3)         
                    |(IO_PULLUP<<4)|(IO_PULLUP<<5)|(IO_PULLUP<<6)|(IO_PULLUP<<7)        
                    |(IO_PULLUP<<8)|(IO_PULLUP<<9)|(IO_PULLUP<<10)|(IO_PULLUP<<11)      
                    |(IO_PULLUP<<12)|(IO_PULLUP<<13)|(IO_PULLUP<<14)|(IO_PULLUP<<15);
gpe_wr16(0xffff);
//
S3C2410SFR_IICCON = 0xef;              //ack enable,interupt enable,clk=pclk/256 -> 200K
                                       //IICCON[5]必须为1，否则可能IIC不正常。
                                      
/*------------------------------------------------------------
GPF:
0:input,522IRQ1,pullup
1:input,522IRQ2,pullup
2,3,4,5,6,7:nouse,input,pullup
--------------------------------------------------------------*/
S3C2410SFR_GPFCON = (IO_INPUT<<0)|(IO_INPUT<<2)|(IO_INPUT<<4)|(IO_INPUT<<6)       
                   |(IO_INPUT<<8)|(IO_INPUT<<10)|(IO_INPUT<<12)|(IO_INPUT<<14); 
S3C2410SFR_GPFUP =  (IO_PULLUP<<0)|(IO_PULLUP<<1)|(IO_PULLUP<<2)|(IO_PULLUP<<3)   
                   |(IO_PULLUP<<4)|(IO_PULLUP<<5)|(IO_PULLUP<<6)|(IO_PULLUP<<7);


/*-------------------------------------------------------------
GPG:
0,1,2,3,4,8,9,10,11,12,13,14,15:no use,input,pullup
5:SPIMISO->522MISO
6:SPIMOSI->522MOSI
7:SPICLK->522CLK
----------------------------------------------------------------*/
S3C2410SFR_GPGCON = (IO_INPUT<<0)|(IO_INPUT<<2)|(IO_INPUT<<4)|(IO_INPUT<<6)                
                    |(IO_INPUT<<8)|(IO_SPI<<10)|(IO_SPI<<12)|(IO_SPI<<14)                  
                    |(IO_INPUT<<16)|(IO_INPUT<<18)|(IO_INPUT<<20)|(IO_INPUT<<22)           
                    |(IO_INPUT<<24)|(IO_INPUT<<26)|(IO_INPUT<<28)|(IO_INPUT<<30);
S3C2410SFR_GPGUP =  (IO_PULLNONE<<0)|(IO_PULLNONE<<1)|(IO_PULLNONE<<2)|(IO_PULLNONE<<3)        
                   |(IO_PULLNONE<<4)|(IO_PULLNONE<<5)|(IO_PULLNONE<<6)|(IO_PULLNONE<<7)        
                   |(IO_PULLNONE<<8)|(IO_PULLNONE<<9)|(IO_PULLNONE<<10)|(IO_PULLNONE<<11)      
                   |(IO_PULLNONE<<12)|(IO_PULLNONE<<13)|(IO_PULLNONE<<14)|(IO_PULLNONE<<15);

/*--------------------------------------------------------------
GPH:
8,9,10:nouse, input, pullup
0:CTS0 
1:RTS0
2:TXD0
3:RXD0
4:TXD1
5:RXD1
6:TXD2
7:RXD2
-----------------------------------------------------------------*/
S3C2410SFR_GPHCON = (IO_UART<<0)|(IO_UART<<2)|(IO_UART<<4)|(IO_UART<<6)       
                   |(IO_UART<<8)|(IO_UART<<10)|(IO_UART<<12)|(IO_UART<<14)    
                   |(IO_INPUT<<16)|(IO_INPUT<<18)|(IO_INPUT<<20);
S3C2410SFR_GPHUP =  (IO_PULLNONE<<0)|(IO_PULLNONE<<1)|(IO_PULLNONE<<2)|(IO_PULLNONE<<3)        
                   |(IO_PULLNONE<<4)|(IO_PULLNONE<<5)|(IO_PULLNONE<<6)|(IO_PULLNONE<<7)        
                   |(IO_PULLNONE<<8)|(IO_PULLNONE<<9)|(IO_PULLNONE<<10);
                   
//
rled(LED_OFF);
gled(LED_OFF);

return;	
}	

/*==============================================================================
函数：
功能：
================================================================================*/
void gled(UBYTE option)
{
if(option == LED_ON){gpd_clr_bit(BIT01_MASK);}
else {gpd_set_bit(BIT01_MASK);}
bgGledState = option;
return;
}
	
/*==============================================================================
函数：
功能：
================================================================================*/
void rled(UBYTE option)
{
//实际---------------------------------------------------------------------------	
if(option == LED_ON){gpd_clr_bit(BIT00_MASK);}
else{gpd_set_bit(BIT00_MASK);}
bgRledState = option;
return;
}	



//end of file
#endif



