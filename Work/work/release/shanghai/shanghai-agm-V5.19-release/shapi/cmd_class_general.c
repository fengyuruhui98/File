//cmd_class_general.c


#ifndef _CMD_CLASS_GENERAL_C_
#define _CMD_CLASS_GENERAL_C_

cmd_sub_func_type  *class_general_func[256];
UBYTE lastcmd[200],lastcmdbyte;
/*===============================================================================================
����:class_general_ini
����:
=================================================================================================*/
void class_general_ini(void)
{
	UWORD i;
	for(i=0;i<256;i++)
	{
		class_general_func[i]=class_general_bad_cmd;
	}
	
	class_general_func[GETLAST_EXETIME]=class_general_getexetime;
	class_general_func[GETRESPONSE]=class_general_getresponse;
	class_general_func[GETKEY]=class_general_getkey;
//	#ifdef _TESTCARD_
	class_general_func[RESETKEY]=class_general_resetkey;
//	#endif
	
	class_general_func[READVERSION]=class_general_version;
	//class_general_func[GETNODE]=class_general_getnode;
	//class_general_func[SETNODE]=class_general_setnode;
	class_general_func[SETSTATUS]=class_general_setstatus;
	
	class_general_func[READEEPROM]=class_general_rdeeprom;
	class_general_func[WRITEEEPROM]=class_general_wreeprom;
	class_general_func[READFLASH]=class_general_rdflash;
	class_general_func[WRITEFLASH]=class_general_wrflash;
	class_general_func[ERASEPAGE]=class_general_erasepage;
	class_general_func[WRFLASHBUF]=class_general_wrflashbuf;
	class_general_func[BUF2FLASH]=class_general_buf2flash;
	class_general_func[FLASH2BUF]=class_general_flash2buf;
	class_general_func[RDFLASHBUF]=class_general_rdflashbuf;

	class_general_func[SOFTRESET]=class_general_reset;
	class_general_func[POWEROFF]=class_general_poweroff;
	class_general_func[ATR]=class_general_atr;
	class_general_func[T0COMMAND]=class_general_T0command;
	class_general_func[DESEN]=class_general_desencode;
	class_general_func[PSAMRESET]=class_general_samreset;
	class_general_func[SETSAMSPEED]=class_general_setsam;
	class_general_func[NHHORM1]=class_general_nhhorm1;
	class_general_func[SELECTRF]=class_general_selectrf;
	class_general_func[DIVERSITY]=class_general_watchdiversity;
	class_general_func[CALMAC]=class_general_watchcalmac;
	//class_general_func[M4_CLASS]=class_general_m4;
	
	class_general_func[LOAD_KEY]=class_general_loadkey;
	class_general_func[AUTH_KEY]=class_general_authentication;
	class_general_func[CARD_REQUEST]=class_general_request;
	class_general_func[ANTICOLL]=class_general_anticoll;
	class_general_func[SELECTCARD]=class_general_select;
	class_general_func[READBLOCK]=class_general_readblock;
	class_general_func[HREADBLOCK]=class_general_hreadblock;
	class_general_func[WRITEBLOCK]=class_general_writeblock;
	class_general_func[HWRITEBLOCK]=class_general_hwriteblock;
	class_general_func[HALTCARD]=class_general_halt;
	class_general_func[DECREMENT]=class_general_decrement;
	class_general_func[RESTORE]=class_general_restore;
	class_general_func[TRANSFER]=class_general_transfer;
	class_general_func[CARDINFO]=class_general_cardinfo;
	class_general_func[MIFPRO_CLASS]=class_general_mifproclass;
	
	class_general_func[READTOCKEN]=class_general_readtocken;
	class_general_func[WRITETOCKEN]=class_general_writetocken;
	
	//����
	class_general_func[SHEXPOISSUE]=class_general_expoissue;
	class_general_func[SHEXPOREADINFO]=class_general_exporeadinfo;
	class_general_func[SHEXPOVERIFY]=class_general_expoverify;
	class_general_func[SHEXPOINI]=class_general_exporeaderini;
	class_general_func[DISPSET]=class_general_expodispset;
	class_general_func[BUZZERSET]=class_general_expobuzzerset;
	
	//class_general_func[CRC_CCITT]=class_general_crc;
	class_general_func[ZTE_CMD]=class_general_ZTEcmd;
	class_general_func[KEKOPERATION]=class_general_kekoperation;
	
	class_general_func[DEBUG_CLASS]=cmd_debug_process;
	
	//�ļ�ϵͳ
//	class_general_func[FORMATFLASH]=class_general_formatflash;
//	class_general_func[CREATFILE]=class_general_creatfile;
//	class_general_func[RWFILE]=class_general_rwfile;
//	class_general_func[DELFILE]=class_general_delfile;
//	class_general_func[ADD2FILE]=class_general_add2file;
	return;
}
/*===============================================================================================
����:class_general
����:
=================================================================================================*/
void class_general(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
UBYTE bCmdType;
//UWORD i;
UDWORD start_time;

start_time = get_sys_time_cnt();
//
bCmdType = inbuf[0];
if(bCmdType!=GETRESPONSE)
{
	gCommandErr=bCmdType;
	memcpy(lastcmd,inbuf,inbytes);
	lastcmdbyte=inbytes;
}
if((UWORD)inbytes == 0) goto label_bad_param;	
class_general_func[bCmdType](&inbuf[0],inbytes,outbuf,outbytes); 	
//
if(bCmdType != GETLAST_EXETIME){
  dwgCmdExeTime = get_sys_time_cnt()-start_time;
  }
  
return;	

//��������
label_bad_param:
outbuf[0]=OP_ERR;
*outbytes=1;
return; 
}	
/*===============================================================================================
����:class_general_version
����:
=================================================================================================*/
void class_general_version(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
//	debug_printf("\x0a\x0d %02d",sizeof(FAT_BPB));
	if((UWORD)inbytes != 1) goto label_bad_param;
	memcpy(outbuf,"HHJT-MR-20120824-0",18);
	*outbytes=18;
	return;
	//��������
	label_bad_param:
	outbuf[0]=OP_ERR;
	*outbytes=1;
	return;
}

/*===============================================================================================
����:class_general_getnode
����:
=================================================================================================*/
//void class_general_getnode(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
//{
//
//	if((UWORD)inbytes != 1) goto label_bad_param;
//	outbuf[0]=0x00;
//	outbuf[1]=gNode;
//	*outbytes=2;
//	return;
//	//��������
//	label_bad_param:
//	outbuf[0]=OP_ERR;
//	*outbytes=1;
//	return;
//}
/*===============================================================================================
����:class_general_setnode
����:
=================================================================================================*/
//void class_general_setnode(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
//{
//	if((UWORD)inbytes != 2) goto label_bad_param;
//	gNode = inbuf[1];
//	outbuf[0]=ACK_OK;
//	*outbytes=1;
//	return;
//	//��������
//	label_bad_param:
//	outbuf[0]=OP_ERR;
//	*outbytes=1;
//	return;
//}
/*===============================================================================================
����:class_general_setstatus
����:
=================================================================================================*/
void class_general_setstatus(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{

	if((UWORD)inbytes != 2) 
		goto label_bad_param;

  bgDeviceStatus = inbuf[1];
	outbuf[0]=ACK_OK;
	*outbytes=1;
	return;
	//��������
	label_bad_param:
	outbuf[0]=OP_ERR;
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_rdeeprom
����:
=================================================================================================*/
void class_general_rdeeprom(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UWORD addtemp,n;
	if((UWORD)inbytes != 4) 
	{
		goto label_bad_param;
	}
	addtemp=inbuf[1]*256+inbuf[2];
	n=inbuf[3];
	if((addtemp+n)>MAX_EEPROM_SIZE)
	{
		goto label_bad_param;
	}
	ee_read(addtemp,n,outbuf);
	*outbytes=n;
	return;
	//��������
	label_bad_param:
	outbuf[0]=OP_ERR;
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_wreeprom
����:
=================================================================================================*/
void class_general_wreeprom(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UWORD addtemp,n;
	if((UWORD)inbytes < 4) 
	{
		goto label_bad_param;
	}
	addtemp=inbuf[1]*256+inbuf[2];
	n=inbytes-3;
	if((addtemp+n)>MAX_EEPROM_SIZE)
	{
		goto label_bad_param;
	}
	ee_write(addtemp,n,inbuf+3);
	outbuf[0]=ACK_OK;
	*outbytes=1;
	return;
	//��������
	label_bad_param:
	outbuf[0]=OP_ERR;
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_rdflash
����:
=================================================================================================*/
void class_general_rdflash(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UDWORD page,addr;
	UBYTE buf[NAND_FLASH_PAGE_VALID_SIZE+NAND_FLASH_PAGE_APPEND_SIZE];

	if(inbytes!=7)
		goto label_bad_param;			
	page=inbuf[1];
	page<<=8;
	page+=inbuf[2];
	page<<=8;
	page+=inbuf[3];
	addr=inbuf[4];
	addr<<=8;
	addr+=inbuf[5];

	if((addr+inbuf[6])>(NAND_FLASH_PAGE_VALID_SIZE+NAND_FLASH_PAGE_APPEND_SIZE))
		goto label_bad_param;
	if(flash_rd_page(page,buf)==0)
	{		
		memcpy(outbuf,&buf[addr],inbuf[6]);
		outbuf[inbuf[6]]=ACK_OK;
		*outbytes=inbuf[6]+1;
		return;
	}
	//��������
	label_bad_param:
	outbuf[0]=OP_ERR;
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_wrflash
����:
=================================================================================================*/
void class_general_wrflash(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UDWORD page,addr;
	UBYTE buf[NAND_FLASH_PAGE_VALID_SIZE+NAND_FLASH_PAGE_APPEND_SIZE];
	
	if(inbytes<7)
		goto label_bad_param;	
	page=inbuf[1];
	page<<=8;
	page+=inbuf[2];
	page<<=8;
	page+=inbuf[3];
	addr=inbuf[4];
	addr<<=8;
	addr+=inbuf[5];
	if((addr+inbytes-6)>(NAND_FLASH_PAGE_VALID_SIZE+NAND_FLASH_PAGE_APPEND_SIZE))
		goto label_bad_param;
	if(flash_rd_page(page,buf)!=0)
	{
		goto label_bad_param;
	}
	memcpy(&buf[addr],&inbuf[6],inbytes-6);
	if(nandf_write_page(page,buf)==0)
	{
		outbuf[0]=ACK_OK;
		*outbytes=1;
		return;
	}
	//��������
	label_bad_param:
	outbuf[0]=OP_ERR;
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_erasepage
����:
=================================================================================================*/
void class_general_erasepage(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UDWORD page,temppage;
	
	if(inbytes!=4)
		goto label_bad_param;	
	page=inbuf[1];
	page<<=8;
	page+=inbuf[2];
	page<<=8;
	page+=inbuf[3];
	temppage=page/NAND_FLASH_PAGES_PER_BLOCK;
	
	if(nandf_erase_block(temppage)!=0)
		goto label_bad_param;
	
	outbuf[0]=ACK_OK;
	*outbytes=1;
	return;
	//��������
	label_bad_param:
	outbuf[0]=OP_ERR;
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_wrflashbuf
����:
=================================================================================================*/
void class_general_wrflashbuf(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UDWORD addr;
	//UBYTE pageinblock;
	if(inbytes<4)
		goto label_bad_param;
	addr=inbuf[1];
	addr<<=8;
	addr+=inbuf[2];
//	if(nandf_write_buf(addr,inbytes-3,&inbuf[3])==0)
	if(flash_write_buf(addr,inbytes-3,&inbuf[3])==0)
	{
		outbuf[0]=ACK_OK;
		*outbytes=1;
		return;
	}			
	//��������
	label_bad_param:
	outbuf[0]=OP_ERR;
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_buf2flash
����:���ݴӻ�����д��FLASH
=================================================================================================*/
void class_general_buf2flash(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UDWORD page;
	if(inbytes!=4)
		goto label_bad_param;
	page=inbuf[1];
	page<<=8;
	page+=inbuf[2];
	page<<=8;
	page+=inbuf[3];
	if(page>=NAND_FLASH_PAGE_NUM)
		goto label_bad_param;
//	if(nandf_page_buf_to_flash(page)==0)
	if(flash_buf_to_main(page)==0)
	{
		outbuf[0]=ACK_OK;
		*outbytes=1;
		return;
	}
	//��������
	label_bad_param:
	outbuf[0]=OP_ERR;
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_flash2buf
����:���ݴ�FLASH����������
=================================================================================================*/
void class_general_flash2buf(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UDWORD page;
	if(inbytes!=4)
		goto label_bad_param;
	page=inbuf[1];
	page<<=8;
	page+=inbuf[2];
	page<<=8;
	page+=inbuf[3];
//	if(nandf_page_flash_to_buf(page)==0)
	if(flash_main_to_buf(page)==0)
	{
		outbuf[0]=ACK_OK;
		*outbytes=1;
		return;
	}
	//��������
	label_bad_param:
	outbuf[0]=OP_ERR;
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_rdflashbuf
����:��ȡ������
=================================================================================================*/
void class_general_rdflashbuf(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UDWORD addr;
	if(inbytes!=4)
		goto label_bad_param;
	addr=inbuf[1];
	addr<<=8;
	addr+=inbuf[2];
	if((addr+inbuf[3])>=NAND_FLASH_PAGE_VALID_SIZE*NAND_FLASH_PAGES_PER_BLOCK)
		goto label_bad_param;
	//if(nandf_read_buf(addr,inbuf[3],&outbuf[1])==0)
	if(flash_read_buf(addr,inbuf[3],&outbuf[1])==0)
	{
		outbuf[0]=ACK_OK;
		*outbytes=inbuf[3]+1;
		return;
	}
	//��������
	label_bad_param:
	outbuf[0]=OP_ERR;
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_reset
����:��λ
=================================================================================================*/
void class_general_reset(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(inbytes!=1)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	set_to_reset();	
	outbuf[0]=ACK_OK;
	*outbytes = 1;
	return;
}
/*===============================================================================================
����:class_general_loadkey
����:������Կ
=================================================================================================*/
void class_general_loadkey(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(inbytes!=10)
	{
		gResponseStatus=ACK_EXPO_BADPARAM;
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	mcml_load_key(inbuf[1],inbuf[2],inbuf[3],&inbuf[4]);
	outbuf[0]=ACK_OK;
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_authentication
����:��֤��Կ
=================================================================================================*/
void class_general_authentication(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(inbytes!=4)
	{
		gResponseStatus=ACK_EXPO_BADPARAM;
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	if(mcml_authentication(inbuf[1],inbuf[2],inbuf[3])!=0)
	{
		gResponseStatus=ACK_EXPO_AUTHERR;
		outbuf[0]=OP_ERR;
	}
	else
	{
		outbuf[0]=ACK_OK;
	}
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_request
����:ѯ��
=================================================================================================*/
void class_general_request(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UWORD i,tint;
	
	if(inbytes!=3)
	{
		gResponseStatus=ACK_EXPO_BADPARAM;
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	if((inbuf[1]!=0x26)&&(inbuf[1]!=0x52)&&(inbuf[1]!=0x00))
	{
		gResponseStatus=ACK_EXPO_BADPARAM;
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	if(inbuf[1]==0)
	{
		inbuf[1]=PICC_REQSTD;
	}
	tint=inbuf[2];
	for(i=0;i<tint;i++)
	{
		if(mcml_request(inbuf[1],outbuf)==0)
		{
		//	outbuf[0]=ACK_OK;
			*outbytes =2;
			return;
		}
//		delay_ms(2);
	}
	gResponseStatus=ACK_EXPO_NOCARD;
	outbuf[0]=OP_ERR;
	*outbytes =1;
	return;
}
/*===============================================================================================
����:class_general_anticoll
����:����ײ
=================================================================================================*/
void class_general_anticoll(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(mcml_anticoll(outbuf)==0)
	{
	//	outbuf[0]=ACK_OK;
		*outbytes =5;
		return;
	}
	else
	{
		gResponseStatus=ACK_EXPO_ANTICOLLERR;
		outbuf[0]=OP_ERR;
		*outbytes =1;
		return;
	}
}
/*===============================================================================================
����:class_general_select
����:ѡ��
=================================================================================================*/
void class_general_select(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(inbytes!=6)
	{
		gResponseStatus=ACK_EXPO_BADPARAM;
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	if(mcml_select(&inbuf[1],&outbuf[1])==0)
	{
		outbuf[0]=ACK_OK;
		*outbytes=2;
		return;
	}
	gResponseStatus=ACK_EXPO_ANTICOLLERR;
	outbuf[0]=OP_ERR;
	*outbytes =1;
	return;
}
/*===============================================================================================
����:class_general_readblock
����:��������������һ��BLOCK
=================================================================================================*/
void class_general_readblock(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(inbytes!=2)
	{
		gResponseStatus=ACK_EXPO_BADPARAM;
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	if(mcml_read(inbuf[1],outbuf)==0)
	{
	//	outbuf[0]=ACK_OK;
		*outbytes = 16;
		return;
	}
	gResponseStatus=ACK_EXPO_RDCARDERR;
		outbuf[0]=OP_ERR;
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_writeblock
����:����д��������һ��BLOCK
=================================================================================================*/
void class_general_writeblock(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(inbytes!=18)
	{
		gResponseStatus=ACK_EXPO_BADPARAM;
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	if(mcml_write(inbuf[1],&inbuf[2])==0)
	{
		outbuf[0]=ACK_OK;
		*outbytes=1;
		return;
	}
	gResponseStatus=ACK_EXPO_WRCARDERR;
		outbuf[0]=OP_ERR;
	*outbytes=1;
	return;	
}
/*===============================================================================================
����:class_general_halt
����:ͣ��
=================================================================================================*/
void class_general_halt(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(mcml_halt()==0)
	{
		outbuf[0]=ACK_OK;
		*outbytes=1;
		return;
	}
	gResponseStatus=ACK_EXPO_BADPARAM;
	outbuf[0]=OP_ERR;
	*outbytes=1;
	return;	
}
/*===============================================================================================
����:class_general_decrement
����:Ǯ���ۿ�
=================================================================================================*/
void class_general_decrement(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UDWORD tlong;
	if(inbytes!=6)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	*((char *)&tlong+LONG_HIGH3) = inbuf[2];
	*((char *)&tlong+LONG_HIGH2) = inbuf[3];
	*((char *)&tlong+LONG_HIGH1) = inbuf[4];
	*((char *)&tlong+LONG_HIGH0) = inbuf[5];
	if(mcml_decrement(inbuf[1],tlong)==0)
	{
		outbuf[0]=ACK_OK;
		*outbytes=1;
		return;
	}
	outbuf[0]=OP_ERR;
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_poweroff
����:����Ƶģ��
=================================================================================================*/
void class_general_poweroff(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	mcml_pwr_off();
	outbuf[0]=ACK_OK;
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_restore
����:Ǯ���ָ�
=================================================================================================*/
void class_general_restore(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(inbytes!=2)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	if(mcml_restore(inbuf[1])==0)
	{
		outbuf[0]=ACK_OK;
		*outbytes=1;
		return;
	}
	outbuf[0]=OP_ERR;
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_transfer
����:Ǯ������ʵ�ʶ���
=================================================================================================*/
void class_general_transfer(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(inbytes!=2)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	if(mcml_transfer(inbuf[1])==0)
	{
		outbuf[0]=ACK_OK;
		*outbytes=1;
		return;
	}
	outbuf[0]=OP_ERR;
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_atr
����:SAM��ATR
=================================================================================================*/
void class_general_atr(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE i;
	UBYTE buf[100];
	UBYTE n;

	if((inbytes!=1)&&(inbytes!=2)&&(inbytes!=3)&&(inbytes!=4)&&(inbytes!=5)&&(inbytes!=6))
	{
		gResponseStatus=ACK_EXPO_BADPARAM;
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	if(inbytes==1)
	{
		for(i=0;i<3;i++)
		{
			if(sam_atr(inbuf[1],buf,&n)==0)
			{
				memcpy(outbuf,buf,n);
				*outbytes=n;
				return;
			}
		}
	}
	if((inbytes==2)||(inbytes==3))
	{
		sam_select(inbuf[1]);
		if(inbuf[2] == 1) sam_set(inbuf[1],SAM_ETU_372,16);
		else if(inbuf[2] == 2) sam_set(inbuf[1],SAM_ETU_115200,16);
		else if(inbuf[2] == 3) sam_set(inbuf[1],SAM_ETU_57600,16);
		else sam_set(inbuf[1],SAM_ETU_93,16);
		outbuf[0] = ACK_OK;
		*outbytes = 1;
		return;
	}
	if(inbytes==4)
	{
		if(sam_pps(inbuf[1],inbuf[2],buf,&n)==0)
		{
			memcpy(outbuf,buf,n);
			*outbytes=n;
			return;
		}
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	if(inbytes==5)
	{
    sam_select(inbuf[1]);
    sam_set(inbuf[1],inbuf[4],16);
		outbuf[0] = ACK_OK;
		*outbytes = 1;
		return;
	}
	if(inbytes==6)
	{
		if(sam_report_ver(inbuf[1],buf,&n)==0)
		{
			memcpy(outbuf,buf,n);
			*outbytes=n;
			return;
		}
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}	
}
/*===============================================================================================
����:class_general_T0command
����:SAM��T0ָ��
=================================================================================================*/
void class_general_T0command(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE SAMindex,ret,n;
	UBYTE outn;
	if(inbytes<7)
	{
		gResponseStatus=ACK_EXPO_BADPARAM;
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	SAMindex=inbuf[1];
	sam_select(SAMindex);
//	sam_set(SAMindex,SAM_ETU_93,16);
	n=inbytes-2;
	ret=sam_apdu(SAMindex,&inbuf[2],n,outbuf,&outn);
	if(ret!=0)
	{
		gResponseStatus=ACK_EXPO_PSAMERR;
		outbuf[0]=OP_ERR;
		outbuf[1]=ret;
		*outbytes=2;
		return;
	}
//	outbuf[0]=ACK_OK;
	*outbytes=outn;	
	return;
}
/*===============================================================================================
����:class_general_setsam
����:SAM����������
=================================================================================================*/
void class_general_setsam(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(inbytes!=3)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	if(inbuf[2]>1)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	sam_select(inbuf[1]);
	if(inbuf[2]==0)
	sam_set(inbuf[1],SAM_ETU_93,16);
	else
	sam_set(inbuf[1],SAM_ETU_372,16);
	outbuf[0]=ACK_OK;
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_nhhorm1
����:���翨��M1��ѡ��
=================================================================================================*/
void class_general_nhhorm1(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(inbytes!=2)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	if((inbuf[1]!=0)&&(inbuf[1]!=0x55)&&(inbuf[1]!=0x66))
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
//	gNhhFlag=inbuf[1];
	
	rc_init();
//	rc_iso14443_typea_init();
	if(inbuf[1]==0x55)
	{
		gNhhFlag=0x55;
		rc_select_sh_auth();
	}
	if(inbuf[1]==0x00)	
	{
		gNhhFlag=0;
		rc_select_mifare_auth();
	}
	//if(inbuf[1]==0x66)	
	//{
	//	rc_select_op_type(ISO14443B_NORMAL_TYPE);
	//}
	outbuf[0]=ACK_OK;
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_selectrf
����:����ѡ��
=================================================================================================*/
void class_general_selectrf(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(inbytes != 2) goto label_bad_para;
	#ifdef _PCBV11_ 
	rf_set(inbuf[1]);
	#endif
	outbuf[0] = ACK_OK;
	*outbytes = 1;
	return;	
	
	label_bad_para:
	outbuf[0] = ACK_BADPARAM;
	*outbytes = 1;
	return;
}
/*===============================================================================================
����:class_general_cardinfo
����:������Ϣ���ָ�
=================================================================================================*/
void class_general_cardinfo(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE i,buf[20];
	for(i=0;i<10;i++)
	{
		if(mcml_request(PICC_REQSTD,buf)==0)
		{
			if((buf[0]==0x04)&&(buf[1]==0x00))
			{
				goto shanghaicard;
			}
			if((buf[0]==0x03)&&(buf[1]==0x00))
			{
				i=hh_getcardinf(0);
				goto shmetro;
			}
			if((buf[0]==0x00)&&(buf[1]==0x36))
			{
				i=hh_getcardinf(1);
				goto shmetro;
			}
		}
	}
	gDebugStep=0x00;
	outbuf[0]=00;
	outbuf[1]=gDebugStep/256;
	outbuf[2]=gDebugStep%256;
	*outbytes=3;
	return;
shanghaicard:
	i=getcardinfo();
	if(i!=0)
	{
		outbuf[0]=01;
		outbuf[1]=gDebugStep/256;
		outbuf[2]=gDebugStep%256;
		memcpy(&outbuf[3],KeyBuf,12);
		*outbytes=15;
		return;
	}
	outbuf[0]=gThisCardSnr[0];
	outbuf[1]=gThisCardSnr[1];
	outbuf[2]=gThisCardSnr[2];
	outbuf[3]=gThisCardSnr[3];
	outbuf[4]=gCityCode0;
	outbuf[5]=gCityCode1;
	outbuf[6]=gCardType;
	outbuf[7]=gCardExpiredDate[0];
	outbuf[8]=gCardExpiredDate[1];
	outbuf[9]=gCardExpiredDate[2];
	outbuf[10]=gCardExpiredDate[3];
	outbuf[11]=*((char *)&gCardLastRemain+LONG_HIGH3);
	outbuf[12]=*((char *)&gCardLastRemain+LONG_HIGH2);
	outbuf[13]=*((char *)&gCardLastRemain+LONG_HIGH1);
	outbuf[14]=*((char *)&gCardLastRemain+LONG_HIGH0);
	outbuf[15]=gCardStatus;
	outbuf[16]=*((char *)&gCardAddSum+LONG_HIGH3);
	outbuf[17]=*((char *)&gCardAddSum+LONG_HIGH2);
	outbuf[18]=*((char *)&gCardAddSum+LONG_HIGH1);
	outbuf[19]=*((char *)&gCardAddSum+LONG_HIGH0);
	outbuf[20]=*((char *)&gCardPurse+LONG_HIGH0);
	outbuf[21]=*((char *)&gCardPurse+LONG_HIGH1);
	outbuf[22]=*((char *)&gCardPurse+LONG_HIGH2);
	outbuf[23]=*((char *)&gCardPurse+LONG_HIGH3);
	memcpy(&outbuf[24],gKeyA[7],6);
	*outbytes =30;
	return;
shmetro:
	if(i!=0)
	{
		outbuf[0]=02;
		outbuf[1]=gDebugStep/256;
		outbuf[2]=gDebugStep%256;
		*outbytes=3;
		return;
	}
	outbuf[0]=gThisCardSnr[0];
	outbuf[1]=gThisCardSnr[1];
	outbuf[2]=gThisCardSnr[2];
	outbuf[3]=gThisCardSnr[3];
	memcpy(&outbuf[4],hh_SaleBuf,12);
	memcpy(&outbuf[16],hh_B9,4);
	memcpy(&outbuf[20],hh_Entry,8);
	memcpy(&outbuf[28],hh_Exit,12);
	*outbytes =40;
	return;
}
/*************************************************************
������class_general_samreset
���ܣ��Ϻ���ͨ��PSAM��λ
*************************************************************/
void class_general_samreset(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(ResetShangHaiCardSam()==0)
	{
		memcpy(outbuf,bpgSHpsamNo,4);
	}
	else
	{
		memset(outbuf,0,4);
	}
	if(ResetShangHaiMetroSam()==0)
	{
		memcpy(&outbuf[4],bpgSmpsamNo,4);
	}
	else
	{
		memset(&outbuf[4],0,4);
	}
	outbuf[8] = gDebugStep/256;
	outbuf[9] = gDebugStep%256;
	*outbytes = 10;
	return;
}
/*************************************************************
������readtocken
���ܣ�������ƱBLOCK
*************************************************************/
void class_general_readtocken(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE i;
	UBYTE buf[20];
	if(inbytes!=2)
	{
		goto label_bad_param;
	}
//	for(i=0;i<5;i++)	
//	{
//		ret=mcml_request(PICC_REQALL,buf);	
//		if(ret==0)
//		{
//				break;
//		}
//	}
//	if(i==5)
//	{
//		goto label_bad_param;
//	}
	 memset(outbuf,0,4);
	 i=inbuf[1];
	if(tocken_read(i,buf)!=0) 
	{
		goto label_bad_param;
	}
	else
	{
	  memcpy(outbuf,buf,4);
	  *outbytes=4;
	}	
	return;
	//��������
	label_bad_param:
	outbuf[0]=OP_ERR;
	*outbytes=1;
	return;
}
/*************************************************************
������writetocken
���ܣ�д����ƱBLOCK
*************************************************************/
void class_general_writetocken(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE i;
	UBYTE buf[20];
	if(inbytes!=6)
	{
		goto label_bad_param;
	}
//	for(i=0;i<5;i++)	
//	{
//		ret=mcml_request(PICC_REQALL,buf);	
//		if(ret==0)
//		{
//				break;
//		}
//	}
//	if(i==5)
//	{
//		goto label_bad_param;
//	}
	i=inbuf[1];
	memcpy(buf,&inbuf[2],4);
	if(tocken_write(i,buf)!=0) 
	{
		goto label_bad_param;
	}
	outbuf[0]=ACK_OK;
	*outbytes =1;
	return;
	label_bad_param:
	outbuf[0]=OP_ERR;
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_hreadblock
����:�߼���������������һ��BLOCK
=================================================================================================*/
void class_general_hreadblock(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{

	if(inbytes!=2)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	if(mcmh_read(inbuf[1],outbuf,0,0)==0)
	{
//		delay_ms(1000);

//		if(mcml_read(inbuf[1],outbuf)==0)
//		{
			*outbytes = 16;
			return;
//		}
	}

	outbuf[0]=OP_ERR;
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_hwriteblock
����:�߼�����д��������һ��BLOCK
=================================================================================================*/
void class_general_hwriteblock(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(inbytes!=18)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	if(mcmh_write(inbuf[1],&inbuf[2],0,0)==0)
	{
		outbuf[0]=ACK_OK;
		*outbytes=1;
		return;
	}
	outbuf[0]=OP_ERR;
	*outbytes=1;
	return;	
}
/*===============================================================================================
����:class_general_expoissue
����:��������
=================================================================================================*/
void class_general_expoissue(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(inbytes!=29)
	{
		gResponseStatus=ACK_EXPO_BADPARAM;
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	memcpy(IssueDep,&inbuf[2],2);
	memcpy(IssueSn,&inbuf[4],4);
	cardversion=inbuf[8];
	memcpy(IssueGroup,&inbuf[9],5);
	IssueDev=inbuf[14];
	IssueStatus=inbuf[15];
	gCardType=inbuf[16];
	memcpy(AppointTime,&inbuf[17],4);
	memcpy(UseCount,&inbuf[21],4);
	memcpy(BusCount,&inbuf[25],4);
	if(shexpo_cardissue()!=0)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
//	outbuf[0]=ACK_OK;
	memcpy(&outbuf[0],ShExpoCardSn,8);
	memcpy(&outbuf[8],IssueSn,4);
	*outbytes=12;
	return;
}
/*===============================================================================================
����:class_general_exporeadinfo
����:������������Ϣ
=================================================================================================*/
void class_general_exporeadinfo(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE tempbuf[150];
	if(inbytes!=1)
	{
		gResponseStatus=ACK_EXPO_BADPARAM;
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	if(shexpo_cardinfo(tempbuf)!=0)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
//	outbuf[0]=ACK_OK;
	memcpy(outbuf,tempbuf,122);
	*outbytes=122;
	return;
}
/*===============================================================================================
����:class_general_expoverify
����:����У����Կ������
=================================================================================================*/
void class_general_expoverify(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(inbytes!=6)
	{
		gResponseStatus=ACK_EXPO_BADPARAM;
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	gCardType=inbuf[1];
	memcpy(AppointTime,&inbuf[2],4);
	if(shexpo_cardverify()!=0)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
//	outbuf[0]=ACK_OK;
	memcpy(outbuf,ShExpoCardSn,8);
	*outbytes=8;
	return;
}
/*===============================================================================================
����:class_general_exporeaderini
����:����У����Կ������
=================================================================================================*/
void class_general_exporeaderini(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(inbytes!=1)
	{
		gResponseStatus=ACK_EXPO_BADPARAM;
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	ResetShangHaiExpoSam();
	if(bgExposamValid==0)
	{
		gResponseStatus=ACK_EXPO_SAMNOTEXIST;
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
//	outbuf[0]=ACK_OK;
	memcpy(outbuf,bpgExpopsamNo,4);
	*outbytes=4;
	return;
}
/*===============================================================================================
����:class_general_expodispset
����:��������ʾ����
=================================================================================================*/
void class_general_expodispset(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE	temp;
	if(inbytes!=3)
	{
		gResponseStatus=ACK_EXPO_BADPARAM;
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	temp=inbuf[1]&0xf0;
	if((temp!=0x10)&&(temp!=0x20))
	{
		gResponseStatus=ACK_EXPO_BADPARAM;
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	temp=inbuf[1]&0x0f;
	if((temp!=0)&&(temp!=1)&&(temp!=2))
	{
		gResponseStatus=ACK_EXPO_BADPARAM;
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	display_set(inbuf[1],inbuf[2]);
	outbuf[0]=ACK_OK;
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_expobuzzerset
����:����������
=================================================================================================*/
void class_general_expobuzzerset(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UBYTE	temp;
	if(inbytes!=3)
	{
		gResponseStatus=ACK_EXPO_BADPARAM;
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	temp=inbuf[1]&0x0f;
	if((temp!=0)&&(temp!=1)&&(temp!=2))
	{
		gResponseStatus=ACK_EXPO_BADPARAM;
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	buzzer_set(inbuf[1],inbuf[2]);
	outbuf[0]=ACK_OK;
	*outbytes=1; 
	return;
}
/*===============================================================================================
����:class_general_desencode
����:DES����
=================================================================================================*/
void class_general_desencode(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(inbytes!=17)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	des_encode(&inbuf[1],&inbuf[9],outbuf);
	*outbytes=8;
	return;
}
/*===============================================================================================
����:class_general_getexetime
����:ȡ���ϴ������ʱ��
=================================================================================================*/
void class_general_getexetime(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(inbytes!=1)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	outbuf[0] = *((char *)&dwgCmdExeTime+LONG_HIGH3);
	outbuf[1] = *((char *)&dwgCmdExeTime+LONG_HIGH2);
	outbuf[2] = *((char *)&dwgCmdExeTime+LONG_HIGH1);
	outbuf[3] = *((char *)&dwgCmdExeTime+LONG_HIGH0);
	*outbytes=4;
	return;
}
/*===============================================================================================
����:class_general_getresponse
����:ȡ���ϴ������Ӧ�����
=================================================================================================*/
void class_general_getresponse(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	UWORD bytes;
	bytes=4+lastcmdbyte;
	outbuf[0]=ACK_OK;
	outbuf[1]=0;
	outbuf[2]=0;
	outbuf[3]=gResponseStatus;
	memcpy(&outbuf[4],lastcmd,lastcmdbyte);
	*outbytes=bytes;
	return;
}
/*===============================================================================================
����:class_general_resetkey
����:��������Ĳ��Կ���Կ�ָ��ɳ�ʼ״̬
=================================================================================================*/
//#ifdef _TESTCARD_
void class_general_resetkey(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(inbytes!=1)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	if(resetdefaultkey()!=0)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	outbuf[0]=ACK_OK;
	*outbytes=1; 
	return;
}
//#endif
/*===============================================================================================
����:class_general_formatflash
����:��ʽ��FLASH
=================================================================================================*/
//void class_general_formatflash(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
//{
//	UBYTE buf[NAND_FLASH_PAGE_VALID_SIZE],appbuf[NAND_FLASH_PAGE_APPEND_SIZE];
//	UBYTE i;
//	UDWORD page;
//	memset((UBYTE *)&IniBPB,0,sizeof(IniBPB));
//	
//	memcpy(IniBPB.BS_jmpBoot,"\xeb\x3c\x90",3);
//	memcpy(IniBPB.BS_OEMName,"\x4D\x53\x44\x4F\x53\x35\x2E\x30",8);
//	IniBPB.BPB_BytesPerSec[0]= NAND_FLASH_PAGE_VALID_SIZE%256;
//	IniBPB.BPB_BytesPerSec[1]= NAND_FLASH_PAGE_VALID_SIZE/256;
//	IniBPB.BPB_SecPerClus = 1;
//	IniBPB.BPB_RsvdSecCnt = 1;
//	IniBPB.BPB_NumFATs			= 2;
//	IniBPB.BPB_RootEntCnt[0]= 0;
//	IniBPB.BPB_RootEntCnt[1]= 2;
//	IniBPB.BPB_TotSec16[0]	=	0xff;
//	IniBPB.BPB_TotSec16[1]	=	0xff;
//	IniBPB.BPB_Media				= 0xf8;
//	IniBPB.BPB_FATSz16			=	252;
//	IniBPB.BPB_SecPerTrk		= 1;
//	IniBPB.BPB_NumHeads			=	1;
//	IniBPB.BPB_HiddSec			= 0;
//	IniBPB.BPB_TotSec32			= 0;
//	IniBPB.BS_DrvNum			= 0x80;
//	IniBPB.BS_BootSig			= 0x29;
//	IniBPB.BS_VolID[0]			= 0x58;
//	IniBPB.BS_VolID[1]			= 0x68;
//	IniBPB.BS_VolID[2]			= 0x78;
//	IniBPB.BS_VolID[3]			= 0x88;
//	memcpy(IniBPB.BS_VolLab,"POS MEMORY",10);
//	memcpy(IniBPB.BS_FilSysType,"FAT16",5);
//	memcpy(IniBPB.ExecutableCode,&DiskImage[62],448); 
//	memcpy(IniBPB.ExecutableMarker,"\x55\xaa",2);
//	memcpy(buf,(UBYTE *)&IniBPB,NAND_FLASH_PAGE_VALID_SIZE);
//
//
//	nandf_erase_block(77);
//
//	if(nandf_write_page(0x9bf,buf,appbuf)!=0)
//	{
//		outbuf[0]=OP_ERR;
//		*outbytes=1; 
//		return;
//	}
//	page=0x9c0;
//	memset(buf,0,NAND_FLASH_PAGE_VALID_SIZE);
//	memcpy(buf,"\xf8\xff\xff\xff",4);
//	nandf_erase_block(78);
//	if(nandf_write_page(page,buf,appbuf)!=0)
//	{
//		outbuf[0]=OP_ERR;
//		*outbytes=1; 
//		return;
//	}
//	page++;
//	memset(buf,0,4);
//	for(i=1;i<252;i++)
//	{
//		if((page%32)==0)
//		{
//			nandf_erase_block(page/32);
//		}	
//		if(nandf_write_page(page,buf,appbuf)!=0)
//		{
//			outbuf[0]=OP_ERR;
//			*outbytes=1; 
//			return;
//		}
//		page++;
//	}
//	memcpy(buf,"\xf8\xff\xff\xff",4);
//	if(nandf_write_page(page,buf,appbuf)!=0)
//	{
//		outbuf[0]=OP_ERR;
//		*outbytes=1; 
//		return;
//	}
//	page++;
//	memset(buf,0,4);
//	for(i=1;i<252;i++)
//	{
//		if((page%32)==0)
//		{
//			nandf_erase_block(page/32);
//		}	
//		if(nandf_write_page(page,buf,appbuf)!=0)
//		{
//			outbuf[0]=OP_ERR;
//			*outbytes=1; 
//			return;
//		}
//		page++;
//	}
//	for(i=0;i<32;i++)
//	{
//		if((page%32)==0)
//		{
//			nandf_erase_block(page/32);
//		}	
//		if(nandf_write_page(page,buf,appbuf)!=0)
//		{
//			outbuf[0]=OP_ERR;
//			*outbytes=1; 
//			return;
//		}
//		page++;
//	}	
//	outbuf[0]=ACK_OK;
//	*outbytes=1; 
//	return;
//	
//}
/*===============================================================================================
����:class_general_creatfile
����:����һ���ļ�
=================================================================================================*/
//void class_general_creatfile(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
//{
//	UDWORD filesize;
//	UBYTE name[11];
//	if(inbytes!=16)
//	{
//		outbuf[0]=OP_ERR;
//		*outbytes=1;
//		return;
//	}
//	memcpy(name,&inbuf[1],11);
//	filesize=inbuf[12];
//	filesize<<=8;
//	filesize+=inbuf[13];
//	filesize<<=8;
//	filesize+=inbuf[14];
//	filesize<<=8;
//	filesize+=inbuf[15];
//	if(GetFileID(name,&commdir)==1)
//	{
//		outbuf[0]=OP_ERR;
//		*outbytes=1;
//		return;
//	}
//	CreateFile(name,filesize);
//	outbuf[0]=ACK_OK;
//	*outbytes=1; 
//	return;
//}
/*===============================================================================================
����:class_general_delfile
����:ɾ��һ���ļ�
=================================================================================================*/
//void class_general_delfile(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
//{
//	UBYTE name[11];
//	if(inbytes!=12)
//	{
//		outbuf[0]=OP_ERR;
//		*outbytes=1;
//		return;
//	}
//	memcpy(name,&inbuf[1],11);
//	if(DelFile(name)!=0)
//	{
//		outbuf[0]=OP_ERR;
//		*outbytes=1;
//		return;
//	}
//	outbuf[0]=ACK_OK;
//	*outbytes=1; 
//	return;
//}
/*===============================================================================================
����:class_general_rwfile
����:��дһ���ļ�
=================================================================================================*/
//void class_general_rwfile(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
//{
//	UBYTE WRITEFLAG,name[11];
//	UDWORD start,size;
//	UBYTE ret;
//	
//	WRITEFLAG=inbuf[1];
//	memcpy(name,&inbuf[2],11);
//	*((char *)&start+LONG_HIGH3)=inbuf[13];
//	*((char *)&start+LONG_HIGH2)=inbuf[14];
//	*((char *)&start+LONG_HIGH1)=inbuf[15];
//	*((char *)&start+LONG_HIGH0)=inbuf[16];
//	
//	if(WRITEFLAG)
//	{
//		if(inbytes<18)
//		{
//			outbuf[0]=OP_ERR;
//			*outbytes=1;
//			return;
//		}	
//		size=inbytes-17;
//		ret=OperateFile(WRITEFLAG,name,start,size,&inbuf[17]);
//		if(ret==0)
//		{
//			outbuf[0]=ACK_OK;
//			*outbytes=1; 
//			return;
//		}
//		outbuf[0]=OP_ERR;
//		*outbytes=1;
//		return;
//	}
//	else
//	{
//		*((char *)&size+LONG_HIGH3)=inbuf[17];
//		*((char *)&size+LONG_HIGH2)=inbuf[18];
//		*((char *)&size+LONG_HIGH1)=inbuf[19];
//		*((char *)&size+LONG_HIGH0)=inbuf[20];
//		ret=OperateFile(WRITEFLAG,name,start,size,outbuf);
//		if(ret==0)
//		{
//			*outbytes=size; 
//			return;
//		}
//		outbuf[0]=OP_ERR;
//		*outbytes=1;
//		return;
//	}
//}
/*===============================================================================================
����:class_general_add2file
����:��һ���ļ�ĩβ��������
=================================================================================================*/
//void class_general_add2file(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
//{
//	UBYTE name[11];
//	UDWORD size;
//	if(inbytes<13)
//	{
//		outbuf[0]=OP_ERR;
//		*outbytes=1;
//		return;
//	}	
//	memcpy(name,&inbuf[1],11);
//	size=inbytes-12;
//	if(add2file(name,&inbuf[12],size)==0)
//	{
//		outbuf[0]=ACK_OK;
//		*outbytes=1; 
//		return;
//	}
//	outbuf[0]=OP_ERR;
//	*outbytes=1;
//	return;
//}
/*===============================================================================================
����:class_general_getkey
����:ȡ�õ�ǰ����Կ
=================================================================================================*/
void class_general_getkey(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(inbytes!=3)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	if((inbuf[1]!=0)&&(inbuf[1]!=1))
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	if(inbuf[2]>15)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	if(inbuf[1]==0)
	{
		memcpy(outbuf,gKeyA[inbuf[2]],6);
	}
	else
	{
		memcpy(outbuf,gKeyB[inbuf[2]],6);
	}
	*outbytes=6;
	return;
}
/*===============================================================================================
����:class_general_mifproclass
����:��TYPEA CPU������
=================================================================================================*/
void class_general_mifproclass(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	int len;
	UBYTE i;
	if(inbytes<2)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	switch(inbuf[1])
	{
		case 0:
			if(inbytes!=2)
			{
				outbuf[0]=OP_ERR;
				*outbytes=1;
				return;
			}
			len = mifpro_ats0(&outbuf[1]);
			if(len<0)
			{
				outbuf[0]=OP_ERR;
				*outbytes=1;
				return;
			}
			outbuf[0] = ACK_OK;
			*outbytes=len+1;
			return;
		case 0x07:
			if(inbytes<7)
			{
				outbuf[0]=OP_ERR;
				*outbytes=1;
				return;
			}
			len = mifpro_icmd0(inbytes-2,&inbuf[2],&outbuf[1]);
			if(len<0)
			{
				outbuf[0]=OP_ERR;
				*outbytes=1;
				return;
			}
			outbuf[0] = ACK_OK;
			*outbytes=len+1;
			return;
		case 0x09:
			if(inbytes<3)
			{
				outbuf[0]=OP_ERR;
				*outbytes=1;
				return;
			}
		//	len = mifpro500_transceive(&inbuf[2],inbytes-2,&outbuf[1],&i,MIFPRO_LONG_WAIT);
			len = iso_block_transceve(&inbuf[2],inbytes-2,&outbuf[1],&i,2000);
			if(len!=0)
			{
				outbuf[0]=OP_ERR;
				*outbytes=1;
				return;
			}
			outbuf[0] = ACK_OK;
			*outbytes=i+1;
			return;
		default:
			outbuf[0]=OP_ERR;
			*outbytes=1;
			return;
	}

}

/*===============================================================================================
����:class_general_watchdiversity
����:����������ɢ
=================================================================================================*/
void class_general_watchdiversity(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(inbytes!=26)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	WatchDiversity(&inbuf[2],&inbuf[18],&outbuf[1],inbuf[1]);
	outbuf[0] = ACK_OK;
	*outbytes=17;
	return;
}
/*===============================================================================================
����:class_general_watchcalmac
����:�����ݼ���MAC
=================================================================================================*/
void class_general_watchcalmac(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(inbytes<=18)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	if(inbuf[1]==0)
	{
		CmdWatchCalMac(inbytes-18,&inbuf[18],&inbuf[10],&inbuf[2],&outbuf[1],0);
	}
	else
	{
		CmdWatchCalMac(inbytes-26,&inbuf[26],&inbuf[18],&inbuf[2],&outbuf[1],inbuf[1]);
	}
	outbuf[0] = ACK_OK;
	*outbytes = 5;
	return;
}
/*===============================================================================================
����:class_general_kekoperation
����:��KEK��Կ�Ĳ���
=================================================================================================*/
void class_general_kekoperation(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	if(inbytes<2)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	switch(inbuf[1])
	{
		case 0://	if()
						outbuf[0] = ACK_OK;
		//				outbuf[1] = gKEKversion;
						*outbytes = 2;
						return;
		case 1:	
		default:break;
	}
	outbuf[0]=OP_ERR;
	*outbytes=1;
	return;
}
/*===============================================================================================
����:class_general_m4
����:��B���Ĳ���
=================================================================================================*/
//void class_general_m4(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
//{
//	char ret;
//	UBYTE i;
//	UWORD tint;
//	UDWORD tlong;
//	switch(inbuf[1])
//	{
//		case 1:
//			if(inbytes != 4)
//			{
//				outbuf[0]=OP_ERR;
//				*outbytes=1;
//				return;
//			}
//			buf_to_uword(&inbuf[2],tint);
//			tlong = tint;
//			tlong*=1000;
//			tlong = tlong/151;
//			tint = tlong;
//			m4_set_time_out(tint);
//			outbuf[0] = ACK_OK;
//			*outbytes=1;
//			return;
//		case 2:
//			if(inbytes!= 2)
//			{
//				outbuf[0]=OP_ERR;
//				*outbytes=1;
//				return;
//			}
//			m4_request();
//			outbuf[0] = ACK_OK;
//			*outbytes=1;
//			return;
//		case 3:
//			if(inbytes<= 2)
//			{
//				outbuf[0]=OP_ERR;
//				*outbytes=1;
//				return;
//			}
//			ret = m4_cmd(inbytes-2,&inbuf[2],&i,&outbuf[1]);
//			if(ret != 0)
//			{
//				outbuf[0]=OP_ERR;
//				outbuf[1]=ret;
//				*outbytes=2;
//				return;
//			}
//
//			*outbytes = i+1;
//			outbuf[0] = ACK_OK;
//			return;
//		case 7:
//			if(inbytes!=3)
//			{
//				outbuf[0]=OP_ERR;
//				*outbytes=1;
//				return;
//			}
//			ret = rc_select_op_type(inbuf[2]);
//			if(ret != 0)
//			{
//				outbuf[0]=OP_ERR;
//				*outbytes=1;
//				return;
//			}
//			outbuf[0] = ACK_OK;
//			*outbytes=1;
//			return;
//		case 8:
//			if(inbytes<= 2)
//			{
//				outbuf[0]=OP_ERR;
//				*outbytes=1;
//				return;
//			}
//			ret = TypeB_apdu(inbytes-2,&inbuf[2],&i,&outbuf[1]);
//			if(ret != 0)
//			{
//				outbuf[0]=OP_ERR;
//				outbuf[1]=ret;
//				*outbytes=2+i;
//				return;
//			}
//			*outbytes = i+1;
//			outbuf[0] = ACK_OK;
//			return;
//		default:
//			outbuf[0]=OP_ERR;
//			*outbytes=1;
//			return;
//	}
//	return;
//}
/*===============================================================================================
����:class_general_crc
����:����/У��CRC
=================================================================================================*/
//void class_general_crc(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
//{
//	UWORD ret;
//	if(inbytes<2)
//	{
//		outbuf[0]=OP_ERR;
//		*outbytes=1;
//		return;
//	}
//	ret = Crc(&inbuf[1],inbytes-1);
//	outbuf[0]=ACK_OK;
//	uword_to_buf(ret,&outbuf[1]);
//	*outbytes=3;
//	return;
//}
/*===============================================================================================
����:class_general_ZTEcmd
����:�����˶���ģ��ͨѶ
=================================================================================================*/
void class_general_ZTEcmd(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	int ret;
  UBYTE *rece_buf;
  UBYTE n;
  
  zte_set_rece_ptr(rece_buf);
  	
response:
	if(inbytes<2)
	{
		outbuf[0]=OP_ERR;
		*outbytes=1;
		return;
	}
	switch(inbuf[1])
	{
		case 1:
				prot_zte_send_block(&inbuf[2],inbytes-2);
				while(1){
  	      if(is_zte_wait_process()) break;
          if(is_zte_send_timeout()){
             outbuf[0] = 0xff;
						 *outbytes = 1;
						 return;
						 }
          }
				if(zte_rece_bytes>6){
					 goto label_get_ok;
					 }
				else {
					outbuf[0] = 0xfe;
					memcpy(&outbuf[1],rece_buf,zte_rece_bytes);
					*outbytes = zte_rece_bytes+1;
					return;							
					}
			
    label_get_ok:
				if((inbuf[2]==0xa2)&&(inbuf[3]==0x41))
				{
					if(memcmp(&rece_buf[2],"\x13\x00\x00\x01",4)==0)
					{
						inbuf[1] = 0x01;
						inbuf[2] = 0xa2;
						inbuf[3] = 0x42;
						memcpy(&inbuf[4],&rece_buf[6],16);
						inbytes = 20;
						goto response;
					}
				}
				outbuf[0] = ACK_OK;
				memcpy(&outbuf[1],rece_buf,zte_rece_bytes);
				*outbytes = zte_rece_bytes+1;
				return;
		case 2:
				ret = ZTE_ICMD(inbytes-2,&inbuf[2],&n,&outbuf[1]);
				if(ret==2)
				{
					outbuf[0] = 0xfe;
					memcpy(&outbuf[1],rece_buf,zte_rece_bytes);
					*outbytes = zte_rece_bytes+1;
					return;
				}
				outbuf[0] = ACK_OK;
				*outbytes = n+1;
				return;
		case 3:ret = ZTE_REQ();
					outbuf[0] = (char)ret;
					*outbytes = 1;
					return;
		case 4:ret = ZTE_HALT();
					outbuf[0] = (char)ret;
					*outbytes = 1;
					return;
		case 5:
					uart_init();
					InitComm(0,115200,1,0);
					switch(inbuf[2])
					{
						case 0:
							InitComm(ZTE_UART_INDEX,115200,1,0);
							break;
						case 1:
							InitComm(ZTE_UART_INDEX,57600,1,0);
							break;
						case 2:
							InitComm(ZTE_UART_INDEX,38400,1,0);
							break;
						case 3:
							InitComm(ZTE_UART_INDEX,19200,1,0);
							break;
						case 4:
							InitComm(ZTE_UART_INDEX,9600,1,0);
							break;
						default:
							outbuf[0] = 0xfe;
							*outbytes = 1;
							return;
					}
					outbuf[0] = ACK_OK;
					*outbytes = 1;
					return;
		default:
					outbuf[0]=OP_ERR;
					*outbytes=1;
					return;
	}
}
/*************************************************************
������
*************************************************************/
void class_general_bad_cmd(UBYTE *inbuf,UWORD inbytes,UBYTE *outbuf,UWORD *outbytes)
{
	gResponseStatus=ACK_EXPO_BADCOMMAND;
	outbuf[0]=OP_ERR;
	*outbytes=1;
	return;
}


/*=====================================================================================
����:cmd_debug_process
����:
=======================================================================================*/
void cmd_debug_process(UBYTE *ibuf,UWORD ibytes,UBYTE *outbuf,UWORD *outbytes)
{
UDWORD tudword,tudword2;
UWORD tuword;
UBYTE i;
UBYTE *inbuf;
UWORD inbytes;


inbuf = &ibuf[2];
inbytes = ibytes-2;
	
switch((UBYTE)inbuf[0]){	
case DEBUG_GET_SYSTIME:
	   if(inbytes == 1){
	      outbuf[0] = 0;
	      tudword = get_sys_time_cnt();	   
	      outbuf[1] = *((char *)&tudword+LONG_HIGH3);
	      outbuf[2] = *((char *)&tudword+LONG_HIGH2);
	      outbuf[3] = *((char *)&tudword+LONG_HIGH1);
	      outbuf[4] = *((char *)&tudword+LONG_HIGH0);
	      *outbytes = 5;
	      return;
	      }
	   if(inbytes == 2){
	      outbuf[0] = 0;
	      tudword = dwgSystemTimeCnt;	   
	      outbuf[1] = *((char *)&tudword+LONG_HIGH3);
	      outbuf[2] = *((char *)&tudword+LONG_HIGH2);
	      outbuf[3] = *((char *)&tudword+LONG_HIGH1);
	      outbuf[4] = *((char *)&tudword+LONG_HIGH0);
	      *outbytes = 5;
	      return;
	      }
	   goto label_err;
case DEBUG_GET_LAST_EXEC_TIME:
	   if(inbytes != 1) goto label_err;
	   outbuf[0] = 0;
	   outbuf[1] = *((char *)&dwgCmdExeTime+LONG_HIGH3);
	   outbuf[2] = *((char *)&dwgCmdExeTime+LONG_HIGH2);
	   outbuf[3] = *((char *)&dwgCmdExeTime+LONG_HIGH1);
	   outbuf[4] = *((char *)&dwgCmdExeTime+LONG_HIGH0);
	   *outbytes = 5;
	   return;
case DEBUG_DELAY:
	   if(inbytes != 3) goto label_err;
	   *((char *)&tuword+INT_HIGH) = inbuf[1];
	   *((char *)&tuword+INT_LOW) = inbuf[2];
	   delay_ms(tuword);
	   goto label_ok;	
#ifdef 	DEBUG_RC_RD_BYTE   
case DEBUG_RC_RD_BYTE:
	   if(inbytes != 2) goto label_rc_rd_all;
	   outbuf[1] = rc_read_byte(inbuf[1]);
	   outbuf[0] = 0;
	   *outbytes = 2;
	   return;	
	   
	   label_rc_rd_all:
	   if(inbytes != 1) goto label_err;
	   for(i=0;i<64;i++) outbuf[i+1] = rc_read_byte(i);
	   outbuf[0] = 0;
	   *outbytes = 65;	
	   return;
#endif
#ifdef 	DEBUG_RC_WR_BYTE   
case DEBUG_RC_WR_BYTE:
	   if(inbytes != 3) goto label_err;
	   rc_write_byte(inbuf[1],inbuf[2]);
	   goto label_ok;
#endif           
case DEBUG_RD_PORT:
	   if(inbytes != 5) goto label_err;
	   *((char *)&tudword+LONG_HIGH3) = inbuf[1];
	   *((char *)&tudword+LONG_HIGH2) = inbuf[2];
	   *((char *)&tudword+LONG_HIGH1) = inbuf[3];
	   *((char *)&tudword+LONG_HIGH0) = inbuf[4];
	   tudword2 = (u32)*(vu32 *)tudword;
	   outbuf[0] = 0;
	   outbuf[1] = *((char *)&tudword2+LONG_HIGH3);
	   outbuf[2] = *((char *)&tudword2+LONG_HIGH2);
	   outbuf[3] = *((char *)&tudword2+LONG_HIGH1);
	   outbuf[4] = *((char *)&tudword2+LONG_HIGH0);
	   *outbytes = 5;
	   return;	
case DEBUG_WR_PORT:
	   if(inbytes != 9) goto label_err;
	   *((char *)&tudword+LONG_HIGH3) = inbuf[1];
	   *((char *)&tudword+LONG_HIGH2) = inbuf[2];
	   *((char *)&tudword+LONG_HIGH1) = inbuf[3];
	   *((char *)&tudword+LONG_HIGH0) = inbuf[4];	   
	   *((char *)&tudword2+LONG_HIGH3) = inbuf[5];
	   *((char *)&tudword2+LONG_HIGH2) = inbuf[6];
	   *((char *)&tudword2+LONG_HIGH1) = inbuf[7];
	   *((char *)&tudword2+LONG_HIGH0) = inbuf[8];
	   *(vu32 *)tudword = tudword2;
	   goto label_ok;
#ifdef DEBUG_CHG_BAUD	   
case DEBUG_CHG_BAUD:
	   if(inbytes != 6) goto label_err;
	   *((char *)&tudword+LONG_HIGH3) = inbuf[2];
	   *((char *)&tudword+LONG_HIGH2) = inbuf[3];
	   *((char *)&tudword+LONG_HIGH1) = inbuf[4];
	   *((char *)&tudword+LONG_HIGH0) = inbuf[5];	   
	   uart_change_baud(inbuf[1],tudword);
	   goto label_ok;
#endif	   
#ifdef DEBUG_SAM           
case DEBUG_SAM:
	   if(inbytes < 2) goto label_err;
	   cmd_debug_sam(&inbuf[1],inbytes-1,outbuf,outbytes); 	   
	   return;
#endif           
#ifdef DEBUG_TEMP
case DEBUG_TEMP:
	   if(inbytes != 2) goto label_err;
     outbuf[0] = 0;
     outbuf[1] = 0;
     *outbytes = 2;
     return;
#endif	   
#ifdef DEBUG_LCD_CLASS
case DEBUG_LCD_CLASS:
	   if(inbytes < 2) goto label_err;
	   cmd_lcd_process(&inbuf[1],inbytes-1,outbuf,outbytes);
	   return;
#endif	
#ifdef DEBUG_ADC
case DEBUG_ADC:
	   if(inbytes != 2) goto label_err;
	   tuword = adc_get(inbuf[1]);
	   outbuf[0] = 0;
	   uword_to_buf(tuword,&outbuf[1]);	
	   *outbytes = 3;
	   return;
#endif	
#ifdef DEBUG_PRN_CLASS
case DEBUG_PRN_CLASS:
      if(inbytes < 2) goto label_err;
      cmd_debug_prn(&inbuf[1],inbytes-1,outbuf,outbytes);
      return;
#endif	   
#ifdef DEBUG_GPRS_POWER      
case DEBUG_GPRS_POWER:
	   if(inbytes != 2) goto label_err;
	   if(inbuf[1]){ gprs_pwr_on();}
	   else{ gprs_pwr_off();}		
	   goto label_ok;
#endif
#ifdef DEBUG_GPRS_PUT           
case DEBUG_GPRS_PUT:
     if(inbytes <=1 ) goto label_err;
     //ret = prot_lcd_tranceve(&inbuf[1],inbytes-1,&outbuf[1],outbytes,5000);
     //if(ret) goto label_err_ret;
     //outbuf[0]=0;
     //*outbytes = *outbytes+1;	
     //return;
     goto label_ok;
#endif     
#ifdef DEBUG_GET_RESET_REC  
case DEBUG_GET_RESET_REC:
	   if(inbytes != 2) goto label_err;
	   if(reset_rec_get(inbuf[1],&outbuf[1]) == 0){
	   	  outbuf[0] = 0;
	   	  *outbytes = 9;
	   	  return;
	   	  }	
	   outbuf[0] = 1;
	   outbuf[1] = 0xff;
	   *outbytes = 2;	  
	   return;
#endif
default:	   
	   break;
  }
goto label_err;
//
//label_err_ret:
//outbuf[0] = 0x01;	
//outbuf[0] = ret;
//*outbytes = 2;
//return;
//
label_err:
outbuf[0] = 0xff;
*outbytes = 1;
return;
//
label_ok:
outbuf[0] = 0x00;
*outbytes = 1;
return;
}


#endif