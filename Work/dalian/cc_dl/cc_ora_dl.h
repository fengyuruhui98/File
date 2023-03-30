#ifndef CC_ORA_DL_H
#define CC_ORA_DL_H

 
#include "sys_head.h"
#include "base_funcs.h"
#include "cc_struct_dl.h"
#include "cc_commu_dl.h"
 
#define CONN_DB_COUNT   17

/* ------------------------------------------------------
	数据库连接说明:
	0 基础数据处理
	
	1 1001交易处理---HHJT_20210324
	2 1002交易处理---HHJT_20210324
	3 1003交易处理---HHJT_20210324
	4 1004交易处理---HHJT_20210324
	5 1005交易处理---HHJT_20210324
	6 1006交易处理---HHJT_20211221
		
	7 TVM交易处理 --5001报文-HHJT_20210324
	8 BOM交易处理 --5002报文-HHJT_20210324
	9 AGM交易处理 --5003报文-HHJT_20210324
 10 状态数据处理 --7010报文-HHJT_20210324
 11 FTP文件入库处理---HHJT_20210324
 12 FTP文件上传处理---HHJT_20210324
 13 7002 文件生成---HHJT_20210323
 14 日结(调用存储过程,删除历史数据,7001,3003文件生成)---HHJT_20210323
 15 20221207 --更新客流
 16 待定--- 
 
--------------------------------------------------------- */

class ORA
{
	private:
		static char ms_db_usename[50];
		static char ms_db_passwd[50];
		static char ms_db_dbname[50];
 
	public:
		static sem_t  			ms_CSSQL;	
		
	private:
		static int 					 init_db();
		static int   				 connect_to_db(int sequence,char *username,char *passwd,char *hostname);
		static void   			 disconnect_to_db(int sequence);
		static void          free_db();

	public:
		static int 					 DbStart(char *username,char *passwd,char *hostname);//变量初始化,建立连接
		static void 				 DbEnd();//断开连接,释放变量
 
		static int   				connect_to_db(char *username,char *passwd,char *hostname);
		static void 				getsclist();
		static void 				get_mlc_line_list();
		static void 				get_iscs_station_list();		
		static int 					get_session_number();
		static int 					get_pkg_number();
		static int           insert_into_6101(unsigned char* recvbuf,int buflen);
		static int           insert_into_6002(unsigned char* recvbuf,int buflen);
		 
		static int 					insert_into_1001(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_1002(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_1003(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_1004(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_1005(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_1006(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);

		static int 					insert_into_4001(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_4002(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_4003(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_5001(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_5002(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_5003(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_5004(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_5005(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_5010(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_5020(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_5021(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_5022(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);		
		 
		static int 					insert_into_3001(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_3003(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_3005(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_3006(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_3007(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_3008(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		 
		static void  				 create2001file(const char *c_recvid,const char *c_date);
		static int           insert_into_2002(unsigned char* recvbuf,char *name,int buflen);
		static int           insert_into_2003(unsigned char* recvbuf,char *name,int buflen);
		static int           insert_into_2004(unsigned char* recvbuf,char *name,int buflen);
		static int           insert_into_2005(unsigned char* recvbuf,char *name,int buflen);
		static int           insert_into_2006(unsigned char* recvbuf,char *name,int buflen);
		static int           insert_into_2007(unsigned char* recvbuf,char *name,int buflen);
		static int           insert_into_2008(unsigned char* recvbuf,char *name,int buflen);
		static int           insert_into_2009(unsigned char* recvbuf,char *name,int buflen);
		static int           insert_into_200A(unsigned char* recvbuf,char *name,int buflen);
		static int           insert_into_200B(unsigned char* recvbuf,char *name,int buflen);
		static int           insert_into_200C(unsigned char* recvbuf,char *name,int buflen);
		static int           insert_into_2010(unsigned char* recvbuf,char *name,int buflen);
		static int           insert_into_2011(unsigned char* recvbuf,char *name,int buflen);
		static int           insert_into_2012(unsigned char* recvbuf,char *name,int buflen);
		static int           insert_into_2013(unsigned char* recvbuf,char *name,int buflen);
		static int           insert_into_2014(unsigned char* recvbuf,char *name,int buflen);
		static int           insert_into_2015(unsigned char* recvbuf,char *name,int buflen);
		static int           insert_into_2016(unsigned char* recvbuf,char *name,int buflen);
		static int           insert_into_2017(unsigned char* recvbuf,char *name,int buflen);
		static int           insert_into_2018(unsigned char* recvbuf,char *name,int buflen);
		static int           insert_into_2019(unsigned char* recvbuf,char *name,int buflen);
		static int           insert_into_3002(unsigned char* recvbuf,char *name,int buflen);
		static int           insert_into_3009(unsigned char* recvbuf,char *name,int buflen);
		 
		static void 					exec_batch();
		static void 					del_his_data();
		 
		static void 				 Validate_Single_Param(const char *msgtype,const char *c_id);
		static void          Validate_Group_Param();
		static void          Update_Param_Status(unsigned short wMsgCode);
		static void          Delete_Param_Status(int lineIndex);
		
		static int           insert_into_0001(char *filename,int lineIndex);
		static int           insert_into_0002(char *filename,int lineIndex);
		static int           insert_into_0004(char *filename,int lineIndex);
		static int           insert_into_0005(char *filename,int lineIndex);
		static int           insert_into_0006(char *filename,int lineIndex);
		static int           insert_into_0007(char *filename,int lineIndex);
		static int           insert_into_0008(char *filename,int lineIndex);
		static int           insert_into_0009(char *filename,int lineIndex);
		static int           insert_into_000A(char *filename,int lineIndex);
		static int           insert_into_0011(char *filename,int lineIndex);
		static int           insert_into_0012(char *filename,int lineIndex);
		static int           insert_into_0013(char *filename,int lineIndex);
		static int           insert_into_0016(char *filename,int lineIndex);
		static int           insert_into_0017(char *filename,int lineIndex);
		static int           insert_into_0015(char *filename,int lineIndex);
		static int           insert_into_000B(char *filename,int lineIndex);
		static int           insert_into_000C(char *filename,int lineIndex);
		static int           insert_into_000D(char *filename,int lineIndex);
		static int           insert_into_000E(char *filename,int lineIndex);
		static int           insert_into_000F(char *filename,int lineIndex);
		static int           insert_into_0018(char *filename,int lineIndex);
		static int           insert_into_0019(char *filename,int lineIndex);
		static int           insert_into_001A(char *filename,int lineIndex);
		static int           insert_into_0401(char *filename,int lineIndex);
		static int           insert_into_0402(char *filename,int lineIndex);
		static int           insert_into_0403(char *filename,int lineIndex);
		static int           insert_into_0406(char *filename,int lineIndex);
		
		static int   				 create0300param(char *c_recvid,const char *c_ver);
		static int   				 create0301param(char *c_id,const char *c_ver);
		static int  				 create0302param(char *c_id,const char *c_ver);
		static int  				 create0303param(char *c_recvid,const char *c_ver);
		static int  				 create0304param(char *c_recvid,const char *c_ver);
		static int  				 create0305param(char *c_recvid,const char *c_ver);
		static int  				 create0306param(char *c_recvid,const char *c_ver);
		static int  				 create0307param(char *c_recvid,const char *c_ver);
		static int 				   create0501param(char *c_recvid,const char *c_ver);
		static int 				   create0502param(char *c_recvid,const char *c_ver); 
 		static int 				   create0503param(char *c_recvid,const char *c_ver);
 		static int 				   create0504param(char *c_recvid,const char *c_ver);
 		 
		static void 				 create3001file(const char *c_apply_no,const char *c_recv_id);
		static void          create3003file(int lineId);
		static void 				 create3005file(const char *c_apply_no,const char *c_recv_id);
		static void 				 create3006file(const char *c_apply_no,const char *c_recv_id);
		 
		static void          create7001file(int lineId);
		static void          create7002file(int lineId);
		static int 					insert_into_7001(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_7002(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_7010(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
				 
		static void					Get_Last_Day(char *c_date);
		 
		static int           query_3009_info(char *applyid,Trans_3009_2 *m_Trans_3009_2);
		 
		static int 					insert_into_F001(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_F002(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_F003(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_F004(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_F005(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		static int 					insert_into_F006(char *name,WORD msgtype,unsigned char* recvbuf,int buflen);
		 
		static void          update_operator_passwd(Trans_5011 *m_Trans_5011);
		 
		static void          Insert_Ftp_FileName(Lcc_Ftp_File *m_Lcc_Ftp_File);
		static void          Update_Ftp_FileName(char *name); 
			
		static void          updateFlow();			
		static void          GetSleStatus();
				
				
};
 
                                                          
#endif
