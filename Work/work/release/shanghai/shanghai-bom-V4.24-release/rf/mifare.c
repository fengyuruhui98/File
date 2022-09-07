//mifare.c

#ifndef _MIFARE_C_
#define _MIFARE_C_
//start of file

//ֵ����������
//UBYTE  bgEm681ValueOperateMode;
//UBYTE  bgEm681ValueBlock;
//UDWORD dwgEm681Value;

/*=======================================================================
��������mcml_request:
��  �ܣ�
=========================================================================*/
UBYTE mcml_request(UBYTE req_code,UBYTE *atq)
{
UBYTE i;	
for(i=0;i<3;i++){	
   if(rc_request(req_code,atq) == 0) return 0;	
    }
return (UBYTE)-1;
}


/*=========================================================================
��������mcml_anticoll
��  �ܣ�����ײ���
��ڲ���: SNR:5�ֽڿ����к�ָ�룬���һ�ֽ�ΪУ���ֽ�
���ڲ���:
  0����ȷ
 ���ࣺ����
==========================================================================*/
UBYTE mcml_anticoll(UBYTE *snr)
{
UBYTE ret;
ret = rc_anticoll(PICC_ANTICOLL1,0,snr);       
return ret;
}

UBYTE mcml_anticoll2(UBYTE *snr)
{
UBYTE ret;
ret = rc_anticoll(PICC_ANTICOLL2,0,snr);       
return ret;
}


/*==========================================================================
��������mcml_select
��  �ܣ�ѡ��ָ�����кŵĿ�
��ڲ���:
	SNR: �����к��ַ���ָ��
	cardsize:���صĿ�������ʶָ��
���ڲ���:
 0:��ȷ
-1:����
=============================================================================*/
UBYTE mcml_select(UBYTE *snr,UBYTE *status)
{
UBYTE ret,buf[2];	
//
ret = rc_select(PICC_ANTICOLL1,snr,buf);
status[0] = buf[0];
//if(ret==0){
	gThisCardSnr[0] = snr[0];
	gThisCardSnr[1] = snr[1];
	gThisCardSnr[2] = snr[2];
	gThisCardSnr[3] = snr[3];	
	gThisCardSnr[4] = snr[4];	
//	}
//
return ret;
}

UBYTE mcml_select2(UBYTE *snr,UBYTE *status)
{
UBYTE ret,buf[2];	
//
ret = rc_select(PICC_ANTICOLL2,snr,buf);
status[0] = buf[0];
//
return ret;
}


/*===========================================================================
��������mcml_load_key
��  �ܣ�
=============================================================================*/
UBYTE mcml_load_key(UBYTE keyset,UBYTE keyab, UBYTE sectno,UBYTE *buf)
{
keyab = keyab;
keyset=keyset;
sectno=sectno;
gRc500Key[0] = buf[0];
gRc500Key[1] = buf[1];
gRc500Key[2] = buf[2];
gRc500Key[3] = buf[3];
gRc500Key[4] = buf[4];
gRc500Key[5] = buf[5];
return 0;
}


/*==========================================================================
����:mcml_authentication:
����:��֤
============================================================================*/
UBYTE mcml_authentication(UBYTE keyset,UBYTE keyab,UBYTE sectno)
{
UBYTE ret;
	
keyset = keyset;
if(keyab == KEYA) keyab = PICC_AUTHENT1A;
else keyab = PICC_AUTHENT1B;

ret = rc_auth(keyab,sectno);
//
return (UBYTE)ret;
}

UBYTE mcml_authentication2(UBYTE keyset,UBYTE keyab,UBYTE sectno)
{
UBYTE ret;
	
keyset = keyset;
if(keyab == KEYA) keyab = PICC_AUTHENT1A;
else keyab = PICC_AUTHENT1B;

ret = rc_auth2(keyab,sectno);
//
return (UBYTE)ret;
}


/*==========================================================================
����:mcml_read
����:��һ����
============================================================================*/
UBYTE mcml_read(UBYTE block,UBYTE *outbuf)
{
	
if(rc_read(block,outbuf) == 0) return 0;
return (UBYTE)-1;
}

UBYTE mcml_read_4bytes(UBYTE block,UBYTE *outbuf)
{
if(rc_read_4bytes(block,outbuf) == 0) return 0;
return (UBYTE)-1;
}


/*==========================================================================
������mcml_write
���ܣ�дһ����
============================================================================*/
UBYTE mcml_write(UBYTE block,UBYTE *inbuf)
{
if(rc_write(block,inbuf) == 0) return 0;
return (UBYTE)-1;
}

UBYTE mcml_write_4bytes(UBYTE block,UBYTE *inbuf)
{
if(rc_write_4bytes(block,inbuf) == 0) return 0;
return (UBYTE)-1;
}

/*===========================================================================
������mcml_increment
���ܣ�����һ��ֵ
=============================================================================*/
UBYTE mcml_increment(UBYTE addr, UDWORD value)
{
UBYTE buf[4];

//bgEm681ValueOperateMode = PICC_INCREMENT;
//dwgEm681Value = value;
//bgEm681ValueBlock = addr;
//return 0;	

udword_to_buf_reverse(value,buf);
return rc_value_op0(PICC_INCREMENT,addr,buf);
}

/*============================================================================
������mcml_decrement
���ܣ�����һ��ֵ
==============================================================================*/
UBYTE mcml_decrement(UBYTE addr,UDWORD value)
{
UBYTE buf[4];
	
//bgEm681ValueOperateMode = PICC_DECREMENT;
//dwgEm681Value = value;
//bgEm681ValueBlock = addr;
//return 0;	
udword_to_buf_reverse(value,buf);
return rc_value_op0(PICC_DECREMENT,addr,buf);
}

/*====================================================================================================
������mcml_transfer
���ܣ�ʵ��д��������
=====================================================================================================*/
UBYTE mcml_transfer(UBYTE addr)
{
//UBYTE value[4];
//
//switch(bgEm681ValueOperateMode){
//	case PICC_RESTORE:
//	case PICC_DECREMENT:
//	case PICC_INCREMENT:
//		   break;
//	default:
//		   return 1;	   		
//	}
//value[0] = *((char *)&dwgEm681Value+LONG_HIGH0);
//value[1] = *((char *)&dwgEm681Value+LONG_HIGH1);
//value[2] = *((char *)&dwgEm681Value+LONG_HIGH2);
//value[3] = *((char *)&dwgEm681Value+LONG_HIGH3);
////	
//return rc_value_op(bgEm681ValueOperateMode,bgEm681ValueBlock,value,addr);
return rc_transfer(addr);
}

/*==================================================================================================
������mcml_restore
���ܣ�
====================================================================================================*/
UBYTE mcml_restore(UBYTE addr)
{
//bgEm681ValueOperateMode = PICC_RESTORE;
//bgEm681ValueBlock = addr;
//return 0;	
UBYTE value[4];

memset(value,0,4);
return rc_value_op0(PICC_RESTORE,addr,value);
}


/*==================================================================================================
������mcml_pwr_off
���ܣ�
====================================================================================================*/
void mcml_pwr_off(void)
{
rc_power_off();
return;
}

/*==================================================================================================
������mcml_halt
���ܣ�
====================================================================================================*/
UBYTE mcml_halt(void)
{
return rc_halta();
}

//end of file
#endif

