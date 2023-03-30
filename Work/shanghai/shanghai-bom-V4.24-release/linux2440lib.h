//linux2440lib.h
//s3c2440 hardware lib under Linux 2.6 Kernel
//���ƣ�xux
//ʱ�䣺2010-11-16 10:39:50

#ifndef _LINUX2440LIB_H_
#define _LINUX2440LIB_H_
//start of file

//20101116����ͨ�ö�����Ӧ�ýӿں�������.doc

#define UBYTE unsigned char
#define UWORD unsigned short
#define UDWORD unsigned long

#define uchar unsigned char
#define uint  unsigned short
#define ulong unsigned long


//3.1	SAM����
#define MAX_SAM_INDEX     8
#define SAM_ETU_31        (256-(32/2))    //115200,32,2
#define SAM_ETU_62        (256-(62/2))    //57600,62,3
#define SAM_ETU_93        (256-(93/2))    //38400,1
#define SAM_ETU_372       (256-(372/2))   //9600,0
//
//3.1.1	SAM����ѡ��
UBYTE sam_select(UBYTE index);
//�������ܣ�SAM��ͨѶ����
//����������index �����ţ�0-7
//����ֵ��0-�ɹ�
//        !0-ѡ����ʧ��
//
//3.1.2	SAM����������
void sam_set(UBYTE index,UBYTE etu,UBYTE wait_etu);
//�������ܣ�SAM��ͨѶ����//����������index �����ţ�0-7
//etu ETU,SAM_ETU_93(�����ʣ�38400) �� SAM_ETU_372(�����ʣ�9600)
//wait_etu, 4
//����ֵ���޷���
//
//3.1.3	SAM����λ
UBYTE sam_atr(UBYTE channel,UBYTE *outbuf,UBYTE *outbytes);
//�������ܣ�SAM����λ
//����������channel �����ţ�0-7
//          Outbuf ��λӦ������
//          Outbytes ��λӦ���ַ�������
//����ֵ��0 ��λ�ɹ�
//        -1 ���������λ����
//
//3.1.4	SAM��PTS����
int sam_pts(int channel,int ta1);
//�������ܣ�SAM����λ
//����������channel �����ţ�0-7
//          Ta1 ����ѡ����μ��±��� 3-1 SAM��TA1�����벨���ʶ�Ӧ��
//����ֵ��0 ���óɹ�
//        -1����
//
//�� 3-1 SAM��TA1�����벨���ʶ�Ӧ��
//	TA1	������	��ע
//1	0x11	9600	
//2	0x12	19200	
//3	0x13	38400	
//4	0x94	55800	
//5	0x38	57600	
//6	0x95	111600	
//7	0x18	115200	
//
//3.1.5	SAM APDU
UBYTE sam_apdu(UBYTE channel, UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes,UWORD timeout, UBYTE expectlen);
UBYTE sam_apdu_ext(UBYTE channel, UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes,UWORD timeout, UBYTE expectlen);
//sam_apdu��expectlenδʵ�֣�����sam_apdu_ext��ʵ�֣�le>=expectlen>=2
//�������ܣ�7816 T0����-APDU
//����������channel �����ţ�0-7
//Inbuf APDU��������
//Inbytes APDU�����
//outbuf APDU���������(����SW1SW2)
//outbytes APDU��������ݳ���(����SW1SW2)
//timeoutΪ��ʱʱ�䣬��λ����
//Expectlen =0 �жϳ�ʱ����=0 (le>=expectlen>=2)���ճ���expectlen���������أ������Ȳ���expectlen����ʱ���д�
//����ֵ��0�ɹ�
//        -1 ����������������
//
//3.2	��ƵӦ��
//3.2.1	�л���Ƶͨ��
void rf_select(int channel);
//�������ܣ��л���Ƶͨ��
//����������channel 0-1��channel 1���ӿ���USB Type B���������ߣ�channel 0ΪԶ��USB���������ߡ�
//����ֵ����
//
//3.2.2	��ƵоƬ��λ
UBYTE rf_reset(void);
//�������ܣ���ʼ�����ر���Ƶ
//������������
//����ֵ��0-�ɹ�
//        ����-ʧ��
//
//3.2.3	���ù�����ʽ
#define ISO14443A_M1_TYPE    0        
#define ISO14443A_SH_TYPE    1
#define ISO14443B_M4_TYPE    2
#define ISO15693_ICODE1_TYPE 3
//
void set_card_type(UBYTE card_type);
//�������ܣ���Ƶ����ģʽѡ��
//����������card_type 0-ISO14443A_M1_TYPE M1�㷨�� 1--ISO14443A_SH_TYPE �����㷨��2-ISO14443B_M4_TYPE Type B
//����ֵ����
//
//3.2.4	������Ƶ
void mcml_pwr_on(void);
//�������ܣ���Ƶ����
//������������
//����ֵ����
//
//3.2.5	�ر���Ƶ
void mcml_pwr_off(void); 
//�������ܣ���Ƶ�ر�
//������������
//����ֵ����
//
//3.2.6	Ultra Light����ͻ��ѡ��
UBYTE UL_Anticoll_Select(UBYTE * psnr);
//�������ܣ�Ultra Light����ͻ��ѡ��
//����������psnr ����ָ�룬UltraLight����Ϊ7�ֽ�
//����ֵ��0 �ɹ�
//        -1 ����
//
//�����Ҫ�رշ���ͻ���ܣ������ſ�ͬʱ������������ʱ��Ϊ����Ŀǰ�����ǽ�ǰһ�ſ�halt��Ȼ����Ѱ���������Ȼ����Ѱ������˵����Ӧ��������2�ſ��������ֱ�ӷ��ش��󣻷��򣬹ر���Ƶ��Ѱ����
//3.2.7	Ultra Lightҳ��
UBYTE UL_Page_Read(UBYTE addr, UBYTE *pReaddata);
//�������ܣ�Ultra Lightҳ��
//����������addr ����ҳ��ַ��0-15
//          pReaddata ��������ָ��,16�ֽ�
//����ֵ��0 �ɹ�
//        -1 ����
//
//3.2.8	Ultra Lightҳд
UBYTE UL_Page_Write(UBYTE addr, UBYTE *pWritedata);
//�������ܣ�Ultra Lightҳд
//����������addr ��дҳ��ַ��3-15
//          pWritedata ��д������ָ��,4�ֽ�
//����ֵ��0 �ɹ�
//        -1 ����
//
#define PICC_REQSTD        0x26			// request idle
#define PICC_REQALL        0x52			// request all
//3.2.9	Mifare1Ѱ��
UBYTE mcml_request2(UBYTE request_type,UBYTE * atq);
//�������ܣ�MIFARE 1K��MIFARE 4K����ͻ��ѡ��
//����������request_typeѰ��ģʽ��0x26-Standard��0x52-ALL
//          Atq Ѱ��Ӧ�� answer to request,2�ֽ�
//����ֵ��0 �ɹ�
//        -1 ����
//
//3.2.10	Mifare1����ͻ
UBYTE mcml_anticoll(UBYTE * psnr);
//�������ܣ�MIFARE 1K��MIFARE 4K����ͻ��ѡ��
//����������psnr��ſ����׵�ַ������Ϊ4�ֽ�+1�ֽ�У��
//����ֵ��0 �ɹ�
//        -1 ����
//
//3.2.11	Mifare1ѡ��
UBYTE mcml_select(UBYTE * psnr, UBYTE *status);
//�������ܣ�MIFAREѡ��
//����������psnr��ſ����׵�ַ������Ϊ4�ֽ�+1�ֽ�У��
//          Status ������,1�ֽ�
//����ֵ��0 �ɹ�
//        -1 ����
//
//3.2.12	Mifare1������Կ
UBYTE mcml_load_key(UBYTE keyset,UBYTE keyab, UBYTE sectno,UBYTE *buf);
//�������ܣ�MIFARE 1K��MIFARE 4K������Կ
//����������keyset ��Կ����,0
//          keyab 0-��ԿA����0��ԿB
//          sectno ������,0-15
//          key ��Կָ�룬6�ֽ�
//����ֵ��0 �ɹ�
//        !0-ʧ��
//
//3.2.13	Mifare1��Կ��֤
UBYTE mcml_authentication(UBYTE keyset,UBYTE keyab,UBYTE sectno);
//�������ܣ�MIFARE 1K��MIFARE 4K��Կ��֤
//����������keyset ��Կ����,0
//          keyab 0-��ԿA����0��ԿB
//          sectno ������,0-15
//����ֵ��0 �ɹ�
//        -1 ����
//
//3.2.14	Mifare1���
UBYTE mcml_read(UBYTE block,UBYTE *outbuf);
//�������ܣ�MIFARE 1K��MIFARE 4K��ָ����
//����������block ��ţ�0-63
//          outbuf ����������ָ��,16�ֽ�
//����ֵ��0 �ɹ�
//        -1 ����
//
//3.2.15	Mifare1��д
UBYTE mcml_write(UBYTE block,UBYTE *outbuf);
//�������ܣ�MIFARE 1K��MIFARE 4Kдָ����
//����������block ��ţ�0-63
//          outbuf ��д�������,16�ֽ�
//����ֵ��0 �ɹ�
//        -1 ����
//
//3.2.16	Mifare1��ֵ
UBYTE mcml_decrement(UBYTE block, UDWORD value);
//�������ܣ���ָ����ֵ����Ƭ��������
//����������block���,0-63
//          value�����ֵ
//����ֵ��0 �ɹ�
//        -1 ����
//
//3.2.17	Mifare1��ֵ
UBYTE mcml_increment(UBYTE block, UDWORD value);
//�������ܣ���ָ����ֵ����Ƭ��������
//����������block���,0-63
//          value ������ֵ
//����ֵ��0 �ɹ�
//        -1 ����
//
//3.2.18	Mifare1 Restore
UBYTE mcml_restore(UBYTE addr);
//�������ܣ���Transfer�����෴������Ƭָ�����ݿ���������װ����Ƭ��������
//����������addr ΪҪ��װ���ݵĿ��,0-63
//����ֵ��0 �ɹ�
//        -1 ����
//
//3.2.19	Mifare1 Transfer
UBYTE mcml_transfer(UBYTE addr);
//�������ܣ���Increment��Decrementָ��󣬽���Ƭ�������е����ݴ��͵�ָ�����ݿ�
//����������addrΪ���͵����ݿ��,0-63
//����ֵ��0 �ɹ�
//        -1 ����
//
//3.2.20	Mifare1 ͣ��
UBYTE mcml_halt(void);
//�������ܣ�ͣ����ʹ������halt״̬
//������������
//����ֵ��0 �ɹ�
//        -1 ����
//
//3.2.21	CPU��ATS
UBYTE mifpro_ats(UBYTE in_cid,UBYTE *outbuf,UBYTE *outbytes);
//�������ܣ�CPU��ATS
//����������in_cid �ɶ�д�����ǽӴ���ָ����ID��
//          Outbuf ATSӦ������
//          Outbytes ATSӦ�����ݳ���
//����ֵ��0 �ɹ�
//        -1 ����
//
//3.2.22	CPU��ICMD
UBYTE mifpro_icmd(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UWORD *outbytes);
//�������ܣ�ִ�зǽӴ�ʽCPU������
//����������inbuf ��������
//          Inbytes �����
//          Outbuf Ӧ������
//          Outbytes Ӧ�����ݳ���
//����ֵ��0 �ɹ�
//        -1 ����
//3.2.23	CPU��deselect
UBYTE mifpro_deselect(UBYTE in_cid,UBYTE *outbuf,UBYTE *outbytes);
//�������ܣ�ִ�зǽӴ�ʽCPU��deselect
//����������in_cid�ǽӴ���ָ����ID��
//          Outbuf deselectӦ������
//          Outbytes deselectӦ�����ݳ���
//����ֵ��0 �ɹ�
//        -1 ����
//3.3	ʱ��
//3.3.1	��ʱ��
UWORD rtc_rd_time(UBYTE *outbuf);
//�������ܣ�RTC��ʱ��
//����������outbuf[0..5] ������ʱ�������6�ֽ�BCD����/��/��/ʱ/��/��
//����ֵ��0 �ɹ�
//        ���� ��RTCʧ��
//
//3.3.2	дʱ��
UWORD rtc_wr_time(UBYTE *inbuf);
//�������ܣ�RTC��ʱ��
//����������inbuf[0..5] ��д��ʱ�������6�ֽ�BCD����/��/��/ʱ/��/��
//����ֵ��0 �ɹ�
//        ���� дRTCʧ��
//
// 
//3.4	E2PROM
//3.4.1	E2PROM��
UWORD ee_read(UWORD addr,UWORD bytes,UBYTE *outbuf);
//�������ܣ�FM24CL64��
//����������addr ����ʼ��ַ��0-8191�ֽ�
//          bytes Ҫ��ȡ���ֽ���
//          outbuf ��������ָ��
//����ֵ��0-�ɹ�
//        ����-��ʧ��
//
//3.4.2	E2PROMд
UWORD ee_write(UWORD addr,UWORD bytes,UBYTE *inbuf);
//�������ܣ�FM24CL64д
//����������addr д��ʼ��ַ��0-8191�ֽ�
//          bytes Ҫд����ֽ���
//          outbuf д������ָ��
//����ֵ��0-�ɹ�
//        ����-��ʧ��
//
//
//3.5	Watchdog
#define WATCHDOG_START  0
#define WATCHDOG_STOP   1
//3.5.1	Watchdog_init
UWORD watchdog_init(UBYTE type, UBYTE timeout);
//�������ܣ�watchdog��ʼ��
//����������type,0-����watchdog��1-ͣ��watchdog
//          Timeout watchdog��ʱʱ��ֵ����λ�룻���42�룬һ������Ϊ2-6��
//����ֵ����
//
//3.5.2	Watchdog
UWORD watchdog(void);
//�������ܣ�ι��
//������������
//����ֵ����

//3.6	����
//3.6.1	reader_get_version
UBYTE reader_get_version(UBYTE *version, UBYTE *len);
//�������ܣ�ȡ�������ڲ��汾��
//����������version,�汾���ַ�������ʾΪ����HHJT2240_K2.6.32.2_H1.0_S1.0_20110429��   �ֱ��ʾ�ں˰汾��Ӳ���汾�����(��)�汾
//          len �汾���ַ�������
//����ֵ����

//LED option
#define LED_ON             0x00
#define LED_OFF            0x01
//3.6.2	rled
void rled(UBYTE option);
//�������ܣ�����˫ɫ���к�ɫ��
//����������option,0-����1-��
//����ֵ����
//
//3.6.3	gled
void gled(UBYTE option);
//�������ܣ�����˫ɫ������ɫ��
//����������option,0-����1-��
//����ֵ����
//
//3.6.4	rgled
void rgled(UBYTE option);
//�������ܣ�ͬʱ����˫ɫ���к�ɫ�ƺ���ɫ��
//����������option,0-����1-��
//����ֵ����
void beep(UBYTE option);

int  sam_apdu_send(UBYTE index,UBYTE *ibuf,UBYTE ibytes);
int  sam_apdu_receive(UBYTE *obuf,UBYTE *obytes);

//
void timer_clr(UBYTE index);
void timer_set(UBYTE index,UWORD limit);
UDWORD timer_get(UBYTE index);
UBYTE timer_check(UBYTE index);
UDWORD  timer_get_ms(void);

//2013/6/20 9:39:41
UBYTE mcml_read_4bytes(UBYTE block,UBYTE *outbuf);
UBYTE mcml_write_4bytes(UBYTE block,UBYTE *outbuf);

//2013/8/27 10:18:14
#define RF_SPEED_106K   0
#define RF_SPEED_212K   2
#define RF_SPEED_424K   4
#define RF_SPEED_848K   8
#define RF_PPS1_106K    0x00
#define RF_PPS1_212K    0x05
#define RF_PPS1_424K    0x0a
#define RF_PPS1_848K    0x0f
UBYTE mifpro_set_speed(UBYTE tx_speed,UBYTE rx_speed);
UBYTE mifpro_pps(UBYTE pps1,UBYTE *ppss);

//2013/11/17 22:51:30
UBYTE sam_apdu_ext(UBYTE channel, UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes,UWORD timeout, UBYTE expectlen);
void set_apdu_expect_len(int len);
void clr_apdu_expect_len(void);

//2013/11/25 9:51:37
int  sam_apdu_send_ext(UBYTE index,UBYTE *ibuf,UBYTE ibytes,UBYTE expectlen);
int  sam_apdu_receive_ext(UBYTE *obuf,UBYTE *obytes);

//2013/11/28 10:55:34
UBYTE rf_read_register(UBYTE addr,UBYTE *value);
UBYTE rf_write_register(UBYTE addr,UBYTE value);

//2014/4/10 13:30:43
#define ADDR_DEBUG_FLAG   256
void global_debug_flag_init(void);
void global_debug_flag_set(void);
void glabal_debug_flag_clr(void);

//2017/7/1 13:52
int FD_DisableQR();
int FD_EnableQR();


//end of file
#endif



