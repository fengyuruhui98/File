//init.c

#ifndef _INIT_C_
#define _INIT_C_
//start of file

UBYTE bgResetFlag=1;
UBYTE bgResetWrTimeFlag=1;
UWORD wgAbortCmdCnt=0;

/*=======================================================================================
函数：
功能：
=========================================================================================*/
void init_all(void)
{
UBYTE i;
	
bgHaltFlag = 0;	
bgDeviceStatus = 0;
//	
//isr_fill();	
//io_init();
//interrupt_init();
//watchdog_init();
timer_init();
//uart_init();
//debug_init();
//rtc_init();
//ee_init();
//nandf_init();
//sam_init();
//spi_init();

//usb reset
//usb_reset_low();
//delay_ms(50);
//usb_reset_high();

//
//prot_jt_init(&gJtProt0,1,0,UART_JT_CMD0_INDEX);   
//prot_dt_init(&gDtProt0,UART_DT_CMD_INDEX);
//prot_zte_init(ZTE_UART_INDEX);
//
timer_set(TIMER_500MS_INDEX,500);

//uart_change_baud(UART_DT_CMD_INDEX,BAUD57600_INDEX);
//
rf_select(RF_CHANNEL);
mcml_pwr_off();
set_card_type(ISO14443A_SH_TYPE);
//task_com_enable();
//sam_prot_enable();
soft_timer_enable();
//
//ResetShangHaiCardSam();
//ResetShangHaiMetroSam();
//ResetShangHaiMobileSam();
//
shmt_bom_ini();
//class_general_ini();
//
gRedSign = 0;
gled(LED_OFF);
rled(LED_OFF);
buzzerantenna(LED_OFF);
rledantenna(LED_OFF);			
gledantenna(LED_OFF);			

for(i=0;i<11;i++) save[i]=0;
gGetCardInfoStep=0xff;
find_ee_ptr1();
find_ee_ptr2();		
//
auto_recover();
reset_rec_init();
reset_rec_save();
//
CurrentLocation = 0x09;//shanghai
g24GFlag = 0;
return;
}

/*=========================================================================
函数：
功能：搜索空闲位置
===========================================================================*/
void find_ee_ptr1(void)
{
UWORD i;
UBYTE buf[3];
UBYTE last_ptr[3];
//
for(i=0;i<8;i++){
  ee_read(ADDR_SHJT_REC+i*16,3,buf);
	if(i==0){
		 memcpy(last_ptr,buf,3);
		 continue;
		 }
	if(memcmp(last_ptr,buf,3) >= 0){
		 break;
		 }
	memcpy(last_ptr,buf,3);
	}
if(i==8){
	Eepromptr1 = 0;
  }
else Eepromptr1=i;

return;	
}	  
			 
/*=========================================================================
函数：
功能：搜索空闲位置
===========================================================================*/
void find_ee_ptr2(void)
{
UWORD i;
UBYTE buf[4];
UBYTE last_ptr[4];
//
for(i=0;i<8;i++){
	ee_read(ADDR_METRO_REC+i*16,4,buf);
	if(i==0){
		 memcpy(last_ptr,buf,4);
		 continue;
		 }
	if(memcmp(last_ptr,buf,4) >= 0){
		 break;
		 }
	memcpy(last_ptr,buf,4);
	}
	if(i==8){
		Eepromptr2 = 0;
  	}
	else Eepromptr2=i;

	return;	
}	  


/*=========================================================================
函数：
功能：
===========================================================================*/
void auto_recover(void)
{
#ifdef _RECOVERY_AFTER_RESET_	
UBYTE buf[16];
UBYTE outbuf[16];
UWORD outbytes;
//
ee_read(ADDR_RECOVERY_DATA,15,buf);
if(lrc_chk(buf,14) != 0) return;
IniReader(buf,14,outbuf,&outbytes);
//
gStatus=STOP_STATUS;
gMode = STOP_MODE;
//
#endif
return;	
}

/*=========================================================================
函数：
功能：
===========================================================================*/
void auto_abort(void)
{	
UBYTE buf[16];
//	
if(!if_need_auto_abort()) return;

//abort的实际操作
gStatus = IDLE_STATUS;
gGetCardInfoStep = 0xff;
gTaskBusyFlag = 0;//2011-9-15 8:58:35
//	
rf_select(RF_CHANNEL);
mcml_pwr_off();
set_card_type(ISO14443A_SH_TYPE);
//
memcpy(buf,"\xa2\x32\x0\x0",4);
prot_zte_send_block(buf,4); 
zte_set_wait(200);
//
clr_auto_abort_flag();
return;
}
//end of file
#endif



