//linux2440lib.h
//s3c2440 hardware lib under Linux 2.6 Kernel
//编制：xux
//时间：2010-11-16 10:39:50

#ifndef _LINUX2440LIB_H_
#define _LINUX2440LIB_H_
//start of file

//20101116地铁通用读卡器应用接口函数描述.doc

#define UBYTE unsigned char
#define UWORD unsigned short
#define UDWORD unsigned long

#define uchar unsigned char
#define uint  unsigned short
#define ulong unsigned long


//3.1	SAM操作
#define MAX_SAM_INDEX     8
#define SAM_ETU_31        (256-(32/2))    //115200,32,2
#define SAM_ETU_62        (256-(62/2))    //57600,62,3
#define SAM_ETU_93        (256-(93/2))    //38400,1
#define SAM_ETU_372       (256-(372/2))   //9600,0
//
//3.1.1	SAM卡座选择
UBYTE sam_select(UBYTE index);
//函数功能：SAM卡通讯设置
//函数参数：index 卡座号，0-7
//返回值：0-成功
//        !0-选择卡座失败
//
//3.1.2	SAM卡速率设置
void sam_set(UBYTE index,UBYTE etu,UBYTE wait_etu);
//函数功能：SAM卡通讯设置//函数参数：index 卡座号，0-7
//etu ETU,SAM_ETU_93(波特率：38400) 或 SAM_ETU_372(波特率：9600)
//wait_etu, 4
//返回值：无返回
//
//3.1.3	SAM卡复位
UBYTE sam_atr(UBYTE channel,UBYTE *outbuf,UBYTE *outbytes);
//函数功能：SAM卡复位
//函数参数：channel 卡座号，0-7
//          Outbuf 复位应答数据
//          Outbytes 复位应答字符串长度
//返回值：0 复位成功
//        -1 参数错误或复位错误
//
//3.1.4	SAM卡PTS设置
int sam_pts(int channel,int ta1);
//函数功能：SAM卡复位
//函数参数：channel 卡座号，0-7
//          Ta1 速率选择，请参见下表，表 3-1 SAM卡TA1参数与波特率对应表
//返回值：0 设置成功
//        -1错误
//
//表 3-1 SAM卡TA1参数与波特率对应表
//	TA1	波特率	备注
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
//sam_apdu中expectlen未实现，而在sam_apdu_ext中实现，le>=expectlen>=2
//函数功能：7816 T0命令-APDU
//函数参数：channel 卡座号，0-7
//Inbuf APDU命令数据
//Inbytes APDU命令长度
//outbuf APDU命令返回数据(包含SW1SW2)
//outbytes APDU命令返回数据长度(包含SW1SW2)
//timeout为超时时间，单位毫秒
//Expectlen =0 判断超时，！=0 (le>=expectlen>=2)接收长度expectlen后立即返回，若长度不足expectlen并超时则判错
//返回值：0成功
//        -1 参数错误或命令错误
//
//3.2	射频应用
//3.2.1	切换射频通道
void rf_select(int channel);
//函数功能：切换射频通道
//函数参数：channel 0-1；channel 1连接靠近USB Type B插座的天线，channel 0为远离USB插座的天线。
//返回值：无
//
//3.2.2	射频芯片复位
UBYTE rf_reset(void);
//函数功能：初始化并关闭射频
//函数参数：无
//返回值：0-成功
//        其他-失败
//
//3.2.3	设置工作方式
#define ISO14443A_M1_TYPE    0        
#define ISO14443A_SH_TYPE    1
#define ISO14443B_M4_TYPE    2
#define ISO15693_ICODE1_TYPE 3
//
void set_card_type(UBYTE card_type);
//函数功能：射频工作模式选择
//函数参数：card_type 0-ISO14443A_M1_TYPE M1算法， 1--ISO14443A_SH_TYPE 华虹算法，2-ISO14443B_M4_TYPE Type B
//返回值：无
//
//3.2.4	开启射频
void mcml_pwr_on(void);
//函数功能：射频开启
//函数参数：无
//返回值：无
//
//3.2.5	关闭射频
void mcml_pwr_off(void); 
//函数功能：射频关闭
//函数参数：无
//返回值：无
//
//3.2.6	Ultra Light防冲突与选卡
UBYTE UL_Anticoll_Select(UBYTE * psnr);
//函数功能：Ultra Light防冲突与选卡
//函数参数：psnr 卡号指针，UltraLight卡号为7字节
//返回值：0 成功
//        -1 错误
//
//如果需要关闭防冲突功能，即两张卡同时出现在天线区时判为错误，目前做法是将前一张卡halt，然后再寻卡，如果仍然可以寻到卡则说明感应区至少有2张卡，则可以直接返回错误；否则，关闭射频再寻卡。
//3.2.7	Ultra Light页读
UBYTE UL_Page_Read(UBYTE addr, UBYTE *pReaddata);
//函数功能：Ultra Light页读
//函数参数：addr 待读页地址，0-15
//          pReaddata 读出数据指针,16字节
//返回值：0 成功
//        -1 错误
//
//3.2.8	Ultra Light页写
UBYTE UL_Page_Write(UBYTE addr, UBYTE *pWritedata);
//函数功能：Ultra Light页写
//函数参数：addr 待写页地址，3-15
//          pWritedata 待写入数据指针,4字节
//返回值：0 成功
//        -1 错误
//
#define PICC_REQSTD        0x26			// request idle
#define PICC_REQALL        0x52			// request all
//3.2.9	Mifare1寻卡
UBYTE mcml_request2(UBYTE request_type,UBYTE * atq);
//函数功能：MIFARE 1K或MIFARE 4K防冲突与选卡
//函数参数：request_type寻卡模式，0x26-Standard，0x52-ALL
//          Atq 寻卡应答 answer to request,2字节
//返回值：0 成功
//        -1 错误
//
//3.2.10	Mifare1防冲突
UBYTE mcml_anticoll(UBYTE * psnr);
//函数功能：MIFARE 1K或MIFARE 4K防冲突与选卡
//函数参数：psnr存放卡号首地址，卡号为4字节+1字节校验
//返回值：0 成功
//        -1 错误
//
//3.2.11	Mifare1选卡
UBYTE mcml_select(UBYTE * psnr, UBYTE *status);
//函数功能：MIFARE选卡
//函数参数：psnr存放卡号首地址，卡号为4字节+1字节校验
//          Status 卡容量,1字节
//返回值：0 成功
//        -1 错误
//
//3.2.12	Mifare1加载密钥
UBYTE mcml_load_key(UBYTE keyset,UBYTE keyab, UBYTE sectno,UBYTE *buf);
//函数功能：MIFARE 1K或MIFARE 4K加载密钥
//函数参数：keyset 密钥分组,0
//          keyab 0-密钥A，非0密钥B
//          sectno 扇区号,0-15
//          key 密钥指针，6字节
//返回值：0 成功
//        !0-失败
//
//3.2.13	Mifare1密钥认证
UBYTE mcml_authentication(UBYTE keyset,UBYTE keyab,UBYTE sectno);
//函数功能：MIFARE 1K或MIFARE 4K密钥认证
//函数参数：keyset 密钥分组,0
//          keyab 0-密钥A，非0密钥B
//          sectno 扇区号,0-15
//返回值：0 成功
//        -1 错误
//
//3.2.14	Mifare1块读
UBYTE mcml_read(UBYTE block,UBYTE *outbuf);
//函数功能：MIFARE 1K或MIFARE 4K读指定块
//函数参数：block 块号，0-63
//          outbuf 读出块数据指针,16字节
//返回值：0 成功
//        -1 错误
//
//3.2.15	Mifare1块写
UBYTE mcml_write(UBYTE block,UBYTE *outbuf);
//函数功能：MIFARE 1K或MIFARE 4K写指定块
//函数参数：block 块号，0-63
//          outbuf 待写入块数据,16字节
//返回值：0 成功
//        -1 错误
//
//3.2.16	Mifare1减值
UBYTE mcml_decrement(UBYTE block, UDWORD value);
//函数功能：减指定的值到卡片缓冲区中
//函数参数：block块号,0-63
//          value块减少值
//返回值：0 成功
//        -1 错误
//
//3.2.17	Mifare1加值
UBYTE mcml_increment(UBYTE block, UDWORD value);
//函数功能：加指定的值到卡片缓冲区中
//函数参数：block块号,0-63
//          value 块增加值
//返回值：0 成功
//        -1 错误
//
//3.2.18	Mifare1 Restore
UBYTE mcml_restore(UBYTE addr);
//函数功能：与Transfer命令相反，将卡片指定数据块中数据重装到卡片缓冲区中
//函数参数：addr 为要重装数据的块号,0-63
//返回值：0 成功
//        -1 错误
//
//3.2.19	Mifare1 Transfer
UBYTE mcml_transfer(UBYTE addr);
//函数功能：在Increment和Decrement指令后，将卡片缓冲区中的数据传送到指定数据块
//函数参数：addr为传送的数据块号,0-63
//返回值：0 成功
//        -1 错误
//
//3.2.20	Mifare1 停卡
UBYTE mcml_halt(void);
//函数功能：停卡，使卡处于halt状态
//函数参数：无
//返回值：0 成功
//        -1 错误
//
//3.2.21	CPU卡ATS
UBYTE mifpro_ats(UBYTE in_cid,UBYTE *outbuf,UBYTE *outbytes);
//函数功能：CPU卡ATS
//函数参数：in_cid 由读写器给非接触卡指定的ID号
//          Outbuf ATS应答数据
//          Outbytes ATS应答数据长度
//返回值：0 成功
//        -1 错误
//
//3.2.22	CPU卡ICMD
UBYTE mifpro_icmd(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UWORD *outbytes);
//函数功能：执行非接触式CPU卡命令
//函数参数：inbuf 命令数据
//          Inbytes 命令长度
//          Outbuf 应答数据
//          Outbytes 应答数据长度
//返回值：0 成功
//        -1 错误
//3.2.23	CPU卡deselect
UBYTE mifpro_deselect(UBYTE in_cid,UBYTE *outbuf,UBYTE *outbytes);
//函数功能：执行非接触式CPU卡deselect
//函数参数：in_cid非接触卡指定的ID号
//          Outbuf deselect应答数据
//          Outbytes deselect应答数据长度
//返回值：0 成功
//        -1 错误
//3.3	时钟
//3.3.1	读时间
UWORD rtc_rd_time(UBYTE *outbuf);
//函数功能：RTC读时间
//函数参数：outbuf[0..5] 待读出时间参数，6字节BCD，年/月/日/时/分/秒
//返回值：0 成功
//        其他 读RTC失败
//
//3.3.2	写时间
UWORD rtc_wr_time(UBYTE *inbuf);
//函数功能：RTC读时间
//函数参数：inbuf[0..5] 待写入时间参数，6字节BCD，年/月/日/时/分/秒
//返回值：0 成功
//        其他 写RTC失败
//
// 
//3.4	E2PROM
//3.4.1	E2PROM读
UWORD ee_read(UWORD addr,UWORD bytes,UBYTE *outbuf);
//函数功能：FM24CL64读
//函数参数：addr 读起始地址，0-8191字节
//          bytes 要读取的字节数
//          outbuf 读出数据指针
//返回值：0-成功
//        其他-读失败
//
//3.4.2	E2PROM写
UWORD ee_write(UWORD addr,UWORD bytes,UBYTE *inbuf);
//函数功能：FM24CL64写
//函数参数：addr 写起始地址，0-8191字节
//          bytes 要写入的字节数
//          outbuf 写入数据指针
//返回值：0-成功
//        其他-读失败
//
//
//3.5	Watchdog
#define WATCHDOG_START  0
#define WATCHDOG_STOP   1
//3.5.1	Watchdog_init
UWORD watchdog_init(UBYTE type, UBYTE timeout);
//函数功能：watchdog初始化
//函数参数：type,0-启用watchdog，1-停用watchdog
//          Timeout watchdog定时时间值，单位秒；最大42秒，一般设置为2-6秒
//返回值：无
//
//3.5.2	Watchdog
UWORD watchdog(void);
//函数功能：喂狗
//函数参数：无
//返回值：无

//3.6	其他
//3.6.1	reader_get_version
UBYTE reader_get_version(UBYTE *version, UBYTE *len);
//函数功能：取读卡器内部版本号
//函数参数：version,版本号字符串，表示为形如HHJT2240_K2.6.32.2_H1.0_S1.0_20110429，   分别表示内核版本，硬件版本，软件(库)版本
//          len 版本号字符串长度
//返回值：无

//LED option
#define LED_ON             0x00
#define LED_OFF            0x01
//3.6.2	rled
void rled(UBYTE option);
//函数功能：设置双色灯中红色灯
//函数参数：option,0-亮，1-灭
//返回值：无
//
//3.6.3	gled
void gled(UBYTE option);
//函数功能：设置双色灯中绿色灯
//函数参数：option,0-亮，1-灭
//返回值：无
//
//3.6.4	rgled
void rgled(UBYTE option);
//函数功能：同时设置双色灯中红色灯和绿色灯
//函数参数：option,0-亮，1-灭
//返回值：无
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



