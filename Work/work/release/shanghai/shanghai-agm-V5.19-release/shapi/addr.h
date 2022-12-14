//addr.h

#ifndef _ADDR_H_
#define _ADDR_H_
//start of file

//EEPROM内容定义
#define ADDR_CHECK_B32	   2    //20字节： 0-3：卡唯一号,4..19:B32  
#define	ADDR_QR_REC		   192	//64字节:4SN, 10card,8 diverify, 4 GMT , 4 TAC
#define ADDR_SHJT_REC      256  //8*16字节：存交通卡记录，存储记录格式：3（流水号）＋4（卡号）		   
                                //＋2（交易计数器）＋3（余额）＋4（TAC）
#define ADDR_METRO_REC     384  //8*16字节：存薄型卡记录，存储记录格式：4（流水号）＋2（使用次数）
                                //＋4（TAC）＋4（卡号）＋2（保留00）	                             
#define ADDR_RECOVERY_DATA 512  //15:读卡器初始化相关参数暂存: 读卡器初始化的14字节指令+LRC
#define ADDR_START_REC_PTR 527  //6:备份结构。复位记录的起始指针

#define ADDR_START_REC_BASE 768 //512字节：64条：重启记录的起始地址。
                                //记录结构：timestr7+lrc
                                
#define	ADDR_ODA_REC    2560 //存ODA记录，存储格式：3（流水号）+2（银行代码）+10（应用主账号）
														 //+1（主账号序列号）+4（TAC）
#define	ADDR_TRANSPORT_REC  1280	//存ODA记录,存储记录格式：存储记录格式：3（流水号）＋10（卡号）＋2（交易计数器）+1（密钥索引）
																	//+1（算法标识）+2（脱机交易序号）+4（PSAM流水号）＋3（余额）＋4（TAC）							                       
                                


//end of file
#endif



