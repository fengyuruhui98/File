//addr.h

#ifndef _ADDR_H_
#define _ADDR_H_
//start of file

//EEPROM���ݶ���
#define ADDR_CHECK_B32	   2    //20�ֽڣ� 0-3����Ψһ��,4..19:B32  
#define	ADDR_QR_REC		   192	//64�ֽ�:4SN, 10card,8 diverify, 4 GMT , 4 TAC
#define ADDR_SHJT_REC      256  //8*16�ֽڣ��潻ͨ����¼���洢��¼��ʽ��3����ˮ�ţ���4�����ţ�		   
                                //��2�����׼���������3������4��TAC��
#define ADDR_METRO_REC     384  //8*16�ֽڣ��污�Ϳ���¼���洢��¼��ʽ��4����ˮ�ţ���2��ʹ�ô�����
                                //��4��TAC����4�����ţ���2������00��	                             
#define ADDR_RECOVERY_DATA 512  //15:��������ʼ����ز����ݴ�: ��������ʼ����14�ֽ�ָ��+LRC
#define ADDR_START_REC_PTR 527  //6:���ݽṹ����λ��¼����ʼָ��

#define ADDR_START_REC_BASE 768 //512�ֽڣ�64����������¼����ʼ��ַ��
                                //��¼�ṹ��timestr7+lrc
                                
#define	ADDR_ODA_REC    2560 //��ODA��¼���洢��ʽ��3����ˮ�ţ�+2�����д��룩+10��Ӧ�����˺ţ�
														 //+1�����˺����кţ�+4��TAC��
#define	ADDR_TRANSPORT_REC  1280	//��ODA��¼,�洢��¼��ʽ���洢��¼��ʽ��3����ˮ�ţ���10�����ţ���2�����׼�������+1����Կ������
																	//+1���㷨��ʶ��+2���ѻ�������ţ�+4��PSAM��ˮ�ţ���3������4��TAC��							                       
                                


//end of file
#endif


