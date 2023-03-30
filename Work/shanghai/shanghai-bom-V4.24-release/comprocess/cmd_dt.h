//cmd_dt.h

#ifndef _CMD_DT_H_
#define _CMD_DT_H_
//start of file

//#define OT_INITREADER	0x20
//#define OT_RESETREADER	0x21
//#define OT_STOPREADER	0x22
//#define OT_READERSTATUS	0x23
//#define OT_DISPSIGNAL	0x2F
//#define OT_READCARD	0x30
//#define OT_ENTRY	0x31
//#define OT_EXIT	0x32
//#define OT_LOCKCARD	0x33
//#define OT_ADDVALUE	0x34
//#define OT_ENABLECARD	0x35
//#define OT_UPDATE	0x36
//#define OT_EXPIREDATE	0x37
//#define OT_READRECORD	0x39
//#define OT_GETPROOF	0x3A
//#define OT_ABORT	0x3F
//#define OT_TMOUNT	0x40
//#define OT_AUTH	0x41
//#define OT_GETAUTH	0x42
//#define OT_VERIFYPIN	0x70
//#define OT_MODIFYPIN	0x71
//#define OT_PAY	0x3B
//#define OTX_INITREADER	0X50
//#define OTX_ENTRY	0X51
//#define OTX_EXIT	0X52
//#define OTX_UPDATE	0X56
//#define OTX_GETPROOF	0X5A
//#define OTX_GETKEY	0X53
//#define OT_CREDIT_INI	0X54
//#define OT_CREDIT	0X55
//#define OT_UNBLOCK	0X57
//#define OT_GETRAN	0X58
//#define OT_EXPIREDATE1	0X59
//#define OT_FORBID	0X5B
//#define OT_LOCKCARD1	0X5C
//#define OT_CalcMAC	0X5D
//#define OT_CheckMAC	0X5E
//#define OT_ADDVALUE1	0X5F
//#define OT_ENABLECARD1	0X60
//#define OT_READRECORD1	0X61
//#define OT_CREDIT_INI1	0x62
//#define OT_ENABLECARD2	0x63
//
//
//#define CE_OK	           0x00	//����ɹ����
//#define CE_CHECKERROR	   0x01	//У��ʹ���
//#define CE_BADREQUEST	   0x02	//��Ч����
//#define CE_BADPARAM	     0x03	//���������
//#define CE_STATUSERROR	 0x04	//��ͨ����д��״̬��
//#define CE_AUTHENERROR	 0x10	//��֤ʧ��
//#define CE_READERROR	   0x11	//��������
//#define CE_WRITEERROR	   0x12	//д������
//#define CE_CARDREMOVED	 0x13	//���������п��ƶ�
//#define CE_INVALIDCARD	 0x14	//��Ч��
//#define CE_LOCKED	       0X15 //ֹ����
//#define CE_NORECORD	     0x16	//�޽��׼�¼
//#define CE_PUTBACKCARD1	 0x17	//����ʱ�屸��ʧ�ܣ��������¶���
//#define CE_PUTBACKCARD2	 0x18	//�����ۼƳ��ʽ��ʧ�ܣ��������¶���
//#define CE_NCSCSAM	     0x19	//�޽�ͨ��SAM��
//#define CE_STSAM	       0x1A	//�޵���ƱSAM��
//#define CE_PSAMERR	     0x1B	//PSAM������
//#define CE_ISAMERR	     0x1C	//ISAM������
//#define CE_MAC2ERR	     0x20	//MAC2����
//#define CE_NOFUNCTION	   0x21	//��Ƭ��֧�ִ˹���
//#define CE_MACERR	       0X22	//ģ�����MAC����
//#define CE_COMERR	       0X23	//ģ��ͨѶ����
//#define CE_NOLOG	       0X24	//�����޴˼�¼
//#define CE_MAC1ERR	     0x25	//MAC1����
//#define CE_NORANDOM	     0X4E	//��ǰ�������Ч
//#define CE_NOKEY	       0x60	//�޲�����Կ
//#define CE_ADD_MACERR	       0x61	//��ֵ��ȨMAC����
//#define CE_AUTHERR	     0x62	//�ⲿ��֤ʧ��
//#define CE_AUTHFAIL	     0x40	//��ֵ��Ȩ����
//#define CE_NOAUTH	       0x41	//δ��Ȩ
//#define CE_PINERROR	     0x63	//�ⲿ��֤��PIN ��
//#define CE_MODIFYPIN	   0x64	//�޸� �ⲿ��֤��PIN ʧ��
//#define CE_READERERROR	 0x70	//��ͨ����д������
//#define CE_UNKNOWN	     0x7F	//δ֪����


void dt_cmd_process(UBYTE *inbuf,UBYTE inbytes,UBYTE *outuf,UBYTE *outbytes);



//end of file
#endif


