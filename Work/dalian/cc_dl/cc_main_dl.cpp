
#include "sys_head.h"
#include "base_message.h"
#include "base_define.h"
#include "base_md5.h"
#include "base_crc.h"
#include "base_funcs.h"
#include "base_socket.h"
#include "cc_struct_dl.h"
#include "cc_commu_dl.h"
#include "cc_ora_dl.h"
#include "cc_main_dl.h"

 
int           ACC::g_Acc_SockHandle;
bool          ACC::ms_AccIsFirstConnect;
MsgHead				ACC::ms_MsgHeadRcv;
sem_t					ACC::ms_CsRcv; 
	
void					ACC::InitAccStaticVar()
{
		sem_init(&ms_CsRcv,0,1);
}
void					ACC::ReleaseAccStaticVar()
{
		sem_destroy(&ms_CsRcv);
}

/* ------------------------------------------------------
函数功能：得到输入线路节点码的所在下标,失败返回-1
--------------------------------------------------------- */
int ACC::getlineinfo(char *lccode)
{
	int i,index;

	index = -1;
	for(i=0;i<MAX_SLE_COUNT;i++)
	{
		if (strcmp(CConfig::ms_MlcLineList[i].LINE_NID,lccode) == 0)
		{
			index = i;
			break;
		}
	}
	return index;
}
	
/* ------------------------------------------------------
函数功能：得到输入线路节点码的所在下标,失败返回-1
--------------------------------------------------------- */
int ACC::getlineinfo(int lineId)
{
	int i,index;

	index = -1;
	for(i=0;i<MAX_SLE_COUNT;i++)
	{
		if (CConfig::ms_MlcLineList[i].LINE_ID == lineId)
		{
			index = i;
			break;
		}
	}
	return index;
}
				
/* ------------------------------------------------------
函数功能：成功返回1, 信息返回-1;				
--------------------------------------------------------- */	
int ACC::GetFtpMsg(unsigned short msgtype,int lineIndex,FtpMsg *m_FtpMsg)
{
	switch(msgtype)
	{
		/*-----运营参数---------*/
		case 0x0001:
		case 0x0002:
		case 0x0004:
		case 0x0005:
		case 0x0006:
		case 0x0007:
		case 0x0008:
		case 0x0009:
		case 0x000A:
		case 0x0011:
		case 0x0012:
		case 0x0013:
		case 0x0015:
		case 0x0016:
		case 0x0017:
			
			memcpy(m_FtpMsg->ftpIP,CConfig::ms_MlcLineList[lineIndex].DOWN_SPECIAL_FTP_IP,sizeof(m_FtpMsg->ftpIP));				
			m_FtpMsg->ftpPort = CConfig::ms_MlcLineList[lineIndex].DOWN_SPECIAL_FTP_PORT;
			memcpy(m_FtpMsg->ftpUser,CConfig::ms_MlcLineList[lineIndex].DOWN_SPECIAL_FTP_USER,sizeof(m_FtpMsg->ftpUser));		
			memcpy(m_FtpMsg->ftpPass,CConfig::ms_MlcLineList[lineIndex].DOWN_SPECIAL_FTP_PASSWD,sizeof(m_FtpMsg->ftpPass));		
			memcpy(m_FtpMsg->ftpPath,CConfig::ms_MlcLineList[lineIndex].DIR_PARAM_RUN,sizeof(m_FtpMsg->ftpPath));	
			break;
			
		/*-----模式履历参数---------*/
		case 0x0018:
			memcpy(m_FtpMsg->ftpIP,CConfig::ms_MlcLineList[lineIndex].DOWN_NORMAL_FTP_IP,sizeof(m_FtpMsg->ftpIP));				
			m_FtpMsg->ftpPort = CConfig::ms_MlcLineList[lineIndex].DOWN_NORMAL_FTP_PORT;
			memcpy(m_FtpMsg->ftpUser,CConfig::ms_MlcLineList[lineIndex].DOWN_NORMAL_FTP_USER,sizeof(m_FtpMsg->ftpUser));		
			memcpy(m_FtpMsg->ftpPass,CConfig::ms_MlcLineList[lineIndex].DOWN_NORMAL_FTP_PASSWD,sizeof(m_FtpMsg->ftpPass));		
			memcpy(m_FtpMsg->ftpPath,CConfig::ms_MlcLineList[lineIndex].DIR_PARAM_MODE,sizeof(m_FtpMsg->ftpPath));	
			break;		
				
		/*-----黑名单参数---------*/
		case 0x000B:
		case 0x000C:
		case 0x000D:
		case 0x000E:	
		case 0x000F:	
		case 0x0401:
		case 0x0402:
		case 0x0403:
		case 0x0406:			
		/*	
			memcpy(m_FtpMsg->ftpIP,CConfig::ms_MlcLineList[lineIndex].DOWN_SPECIAL_FTP_IP,sizeof(m_FtpMsg->ftpIP));				
			m_FtpMsg->ftpPort = CConfig::ms_MlcLineList[lineIndex].DOWN_SPECIAL_FTP_PORT;
		*/
			memcpy(m_FtpMsg->ftpIP,CConfig::ms_MlcLineList[lineIndex].DOWN_NORMAL_FTP_IP,sizeof(m_FtpMsg->ftpIP));				
			m_FtpMsg->ftpPort = CConfig::ms_MlcLineList[lineIndex].DOWN_NORMAL_FTP_PORT;		
			memcpy(m_FtpMsg->ftpUser,CConfig::ms_MlcLineList[lineIndex].DOWN_NORMAL_FTP_USER,sizeof(m_FtpMsg->ftpUser));		
			memcpy(m_FtpMsg->ftpPass,CConfig::ms_MlcLineList[lineIndex].DOWN_NORMAL_FTP_PASSWD,sizeof(m_FtpMsg->ftpPass));		
			memcpy(m_FtpMsg->ftpPath,CConfig::ms_MlcLineList[lineIndex].DIR_PARAM_BLACKLIST,sizeof(m_FtpMsg->ftpPath));	
			break;	
		
			
		/*-----其他数据目录---------*/
		case 0x0019:
		case 0x001A:		
			/*					
			memcpy(m_FtpMsg->ftpIP,CConfig::ms_MlcLineList[lineIndex].DOWN_SPECIAL_FTP_IP,sizeof(m_FtpMsg->ftpIP));				
			m_FtpMsg->ftpPort = CConfig::ms_MlcLineList[lineIndex].DOWN_SPECIAL_FTP_PORT;
			*/				
			memcpy(m_FtpMsg->ftpIP,CConfig::ms_MlcLineList[lineIndex].DOWN_NORMAL_FTP_IP,sizeof(m_FtpMsg->ftpIP));				
			m_FtpMsg->ftpPort = CConfig::ms_MlcLineList[lineIndex].DOWN_NORMAL_FTP_PORT;
			memcpy(m_FtpMsg->ftpUser,CConfig::ms_MlcLineList[lineIndex].DOWN_NORMAL_FTP_USER,sizeof(m_FtpMsg->ftpUser));		
			memcpy(m_FtpMsg->ftpPass,CConfig::ms_MlcLineList[lineIndex].DOWN_NORMAL_FTP_PASSWD,sizeof(m_FtpMsg->ftpPass));		
			memcpy(m_FtpMsg->ftpPath,CConfig::ms_MlcLineList[lineIndex].DIR_PARAM_OTHER,sizeof(m_FtpMsg->ftpPath));	
			break;	


		/*-----对账数据目录---------*/
		case 0x2001:
		case 0x2002:
		case 0x2003:
		case 0x2004:	
		case 0x2005:			
		case 0x2006:
		case 0x2007:
		case 0x2008:
		case 0x2009:
		case 0x200A:
		case 0x200B:			
		case 0x200C:			
		case 0x2010:	
		case 0x2011:			
		case 0x2012:	
		case 0x2013:
		case 0x2014:
		case 0x2015:			
		case 0x2016:
		case 0x2017:
		case 0x2018:			
		case 0x2019:		
			memcpy(m_FtpMsg->ftpIP,CConfig::ms_MlcLineList[lineIndex].DOWN_NORMAL_FTP_IP,sizeof(m_FtpMsg->ftpIP));				
			m_FtpMsg->ftpPort = CConfig::ms_MlcLineList[lineIndex].DOWN_NORMAL_FTP_PORT;
			memcpy(m_FtpMsg->ftpUser,CConfig::ms_MlcLineList[lineIndex].DOWN_NORMAL_FTP_USER,sizeof(m_FtpMsg->ftpUser));		
			memcpy(m_FtpMsg->ftpPass,CConfig::ms_MlcLineList[lineIndex].DOWN_NORMAL_FTP_PASSWD,sizeof(m_FtpMsg->ftpPass));		
			memcpy(m_FtpMsg->ftpPath,CConfig::ms_MlcLineList[lineIndex].DIR_SETTLE,sizeof(m_FtpMsg->ftpPath));	
			break;							
 		/*-----库存数据目录---------*/
		case 0x3002:		
			memcpy(m_FtpMsg->ftpIP,CConfig::ms_MlcLineList[lineIndex].DOWN_NORMAL_FTP_IP,sizeof(m_FtpMsg->ftpIP));				
			m_FtpMsg->ftpPort = CConfig::ms_MlcLineList[lineIndex].DOWN_NORMAL_FTP_PORT;
			memcpy(m_FtpMsg->ftpUser,CConfig::ms_MlcLineList[lineIndex].DOWN_NORMAL_FTP_USER,sizeof(m_FtpMsg->ftpUser));		
			memcpy(m_FtpMsg->ftpPass,CConfig::ms_MlcLineList[lineIndex].DOWN_NORMAL_FTP_PASSWD,sizeof(m_FtpMsg->ftpPass));		
			memcpy(m_FtpMsg->ftpPath,CConfig::ms_MlcLineList[lineIndex].DIR_STOCK,sizeof(m_FtpMsg->ftpPath));	
			break;
			
 		/*-----交易数据目录---------*/
		case 0x1001:	
		case 0x1002:
		case 0x1003:	
		case 0x1004:
		case 0x1005:		
		case 0x1006:													
			memcpy(m_FtpMsg->ftpIP,CConfig::ms_MlcLineList[lineIndex].UP_FTP_IP,sizeof(m_FtpMsg->ftpIP));				
			m_FtpMsg->ftpPort = CConfig::ms_MlcLineList[lineIndex].UP_FTP_PORT;
			memcpy(m_FtpMsg->ftpUser,CConfig::ms_MlcLineList[lineIndex].UP_FTP_USER,sizeof(m_FtpMsg->ftpUser));		
			memcpy(m_FtpMsg->ftpPass,CConfig::ms_MlcLineList[lineIndex].UP_FTP_PASSWD,sizeof(m_FtpMsg->ftpPass));		
			memcpy(m_FtpMsg->ftpPath,CConfig::ms_MlcLineList[lineIndex].DIR_DATA_TRANS,sizeof(m_FtpMsg->ftpPath));	
			break;		
				
 		/*-----状态数据目录---------*/			
		case 0x7001:	
		case 0x7002:													
			memcpy(m_FtpMsg->ftpIP,CConfig::ms_MlcLineList[lineIndex].UP_FTP_IP,sizeof(m_FtpMsg->ftpIP));				
			m_FtpMsg->ftpPort = CConfig::ms_MlcLineList[lineIndex].UP_FTP_PORT;
			memcpy(m_FtpMsg->ftpUser,CConfig::ms_MlcLineList[lineIndex].UP_FTP_USER,sizeof(m_FtpMsg->ftpUser));		
			memcpy(m_FtpMsg->ftpPass,CConfig::ms_MlcLineList[lineIndex].UP_FTP_PASSWD,sizeof(m_FtpMsg->ftpPass));		
			memcpy(m_FtpMsg->ftpPath,CConfig::ms_MlcLineList[lineIndex].DIR_DATA_STATUS,sizeof(m_FtpMsg->ftpPath));	
			break;	
			
 		/*-----其他数据目录---------*/			
		case 0x4001:	
		case 0x4002:	
		case 0x4003:
				
		case 0x5001:				
		case 0x5002:	
		case 0x5003:	
		case 0x5004:	
		case 0x5005:
		case 0x5020:
		case 0x5021: 		/*-----20230129 新增---------*/
		case 0x5022: 		/*-----20230129 新增---------*/
										
		case 0x3001:	
		case 0x3003:	
		case 0x3005:			
		case 0x3006:	
		case 0x3007:	
		case 0x3008:	
 			
 		case 0x2FF1://实际上是需要上传的2001文件								
			memcpy(m_FtpMsg->ftpIP,CConfig::ms_MlcLineList[lineIndex].UP_FTP_IP,sizeof(m_FtpMsg->ftpIP));				
			m_FtpMsg->ftpPort = CConfig::ms_MlcLineList[lineIndex].UP_FTP_PORT;
			memcpy(m_FtpMsg->ftpUser,CConfig::ms_MlcLineList[lineIndex].UP_FTP_USER,sizeof(m_FtpMsg->ftpUser));		
			memcpy(m_FtpMsg->ftpPass,CConfig::ms_MlcLineList[lineIndex].UP_FTP_PASSWD,sizeof(m_FtpMsg->ftpPass));		
			memcpy(m_FtpMsg->ftpPath,CConfig::ms_MlcLineList[lineIndex].DIR_DATA_OTHER,sizeof(m_FtpMsg->ftpPath));	
			break;	
		default:
			return -1;
			break;										
	}
	
	return 1;
};

 
			
/* ------------------------------------------------------
         功能描述:   处理PACC对账文件
         失败返回 0
--------------------------------------------------------- */
int ACC::DealAccStatFile(char *path,unsigned short wMsgCode,int lineIndex,char *b_SettleDate)
{
	char 							logdata[1024],name[256];
	char 							FilePath[256],errorfilename[256];
	struct stat       buf;
	char              *Package;
	int               filelen;
	
	unsigned short 	  w_Records;
	unsigned int 	    dw_Records;
	
	time_t          	local_time;
	struct tm      		newtime;
	int             	rtn,mday;
	int               ret;
	
	
	memset(name,0x00,sizeof(name));
	sprintf(name,"%2d_%04X_%s",CConfig::ms_MlcLineList[lineIndex].LINE_ID,wMsgCode,b_SettleDate);
 
	ret = 0;
	memset(logdata,0x00,sizeof(logdata));
	sprintf(logdata, "收到来自ACC的%s对账/库存文件\n",name);
	CLog::WriteAppLog(logdata);
		
	rtn = GetAccParamFile(wMsgCode,lineIndex,name);	
	if(rtn != 1)
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata, "ACC对账文件(%s)下载失败\n",name);
		CLog::WriteAppLog(logdata);
		return 0;
	}	
		
	/* --- 读取文件内容 ------- */
	memset(FilePath,0x00,sizeof(FilePath));
	sprintf(FilePath,"%s%s",path,name);
	stat(FilePath,&buf);
	filelen = buf.st_size;
	if(filelen == 0)
	{
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"ACC对账/库存:%s格式错误,长度为0\n",name);
			CLog::WriteAppLog(logdata);
			
			return ret;
	}
	Package = (char*)malloc(filelen);
	memset(Package,0x00,filelen);
	CommonFuncs::ReadFileBuffer(FilePath,Package);
	/* --- 系统时间 --- */
	time(&local_time);
	memset(&newtime,0x00,sizeof(struct tm));
	localtime_r(&local_time,&newtime);
	mday = newtime.tm_mday;
	
	switch(wMsgCode)
	{
		/* --- ACC对账报文 --- */
		case 0x2002:
			
			Trans_2002_1 m_Trans_2002_1;
			memset(&m_Trans_2002_1,0x00,sizeof(m_Trans_2002_1));
			m_Trans_2002_1 = *(Trans_2002_1*)(Package + sizeof(MsgHead));
			w_Records = CommonFuncs::wordtowin(m_Trans_2002_1.w_Records);
			if(filelen != sizeof(MsgHead) + sizeof(Trans_2002_1)+ w_Records * sizeof(Trans_2002_2) + LEN_OF_PackageTail)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s格式错误\n",name);
				CLog::WriteAppLog(logdata);
				
				memset(errorfilename,0x00,sizeof(errorfilename));
				sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
				rename(FilePath,errorfilename);
				break;
			}
			if(w_Records == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s记录数为0,不予以处理\n",name);
				CLog::WriteAppLog(logdata);
				COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
				break;
			}
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::insert_into_2002((unsigned char*)Package,name,filelen);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s重复\n",name);
				CLog::WriteAppLog(logdata);
				remove(FilePath);
			}
			else
			{
				ret = 1;
			}
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;
			
		case 0x2003:
			
			Trans_2003_1 m_Trans_2003_1;
			memset(&m_Trans_2003_1,0x00,sizeof(m_Trans_2003_1));
			m_Trans_2003_1 = *(Trans_2003_1*)(Package + sizeof(MsgHead));
			w_Records = CommonFuncs::wordtowin(m_Trans_2003_1.w_Records);
			if(filelen != sizeof(MsgHead) + sizeof(Trans_2003_1)+ w_Records * sizeof(Trans_2003_2) + LEN_OF_PackageTail)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s格式错误\n",name);
				CLog::WriteAppLog(logdata);
				
				memset(errorfilename,0x00,sizeof(errorfilename));
				sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
				rename(FilePath,errorfilename);
				break;
			}
			if(w_Records == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s记录数为0,不予以处理\n",name);
				CLog::WriteAppLog(logdata);
				COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
				break;
			}
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::insert_into_2003((unsigned char*)Package,name,filelen);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s重复\n",name);
				CLog::WriteAppLog(logdata);
				remove(FilePath);
			}
			else
			{
				ret = 1;
			}
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;
			
		case 0x2004:
			
			Trans_2004_1 m_Trans_2004_1;
			memset(&m_Trans_2004_1,0x00,sizeof(m_Trans_2004_1));
			m_Trans_2004_1 = *(Trans_2004_1*)(Package + sizeof(MsgHead));
			w_Records = CommonFuncs::wordtowin(m_Trans_2004_1.w_Records);
			if(filelen != sizeof(MsgHead) + sizeof(Trans_2004_1)+ w_Records * sizeof(Trans_2004_2) + LEN_OF_PackageTail)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s格式错误\n",name);
				CLog::WriteAppLog(logdata);
				
				memset(errorfilename,0x00,sizeof(errorfilename));
				sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
				rename(FilePath,errorfilename);
				break;
			}
			if(w_Records == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s记录数为0,不予以处理\n",name);
				CLog::WriteAppLog(logdata);
				COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
				break;
			}
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::insert_into_2004((unsigned char*)Package,name,filelen);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s重复\n",name);
				CLog::WriteAppLog(logdata);
				remove(FilePath);
			}
			else
			{
				ret = 1;
			}
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;
			
		case 0x2005:
			
			Trans_2005_1 m_Trans_2005_1;
			memset(&m_Trans_2005_1,0x00,sizeof(m_Trans_2005_1));
			m_Trans_2005_1 = *(Trans_2005_1*)(Package + sizeof(MsgHead));
			w_Records = CommonFuncs::wordtowin(m_Trans_2005_1.w_Records);
			if(filelen != sizeof(MsgHead) + sizeof(Trans_2005_1)+ w_Records * sizeof(Trans_2005_2) + LEN_OF_PackageTail)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s格式错误\n",name);
				CLog::WriteAppLog(logdata);
				
				memset(errorfilename,0x00,sizeof(errorfilename));
				sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
				rename(FilePath,errorfilename);
				break;
			}
			if(w_Records == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s记录数为0,不予以处理\n",name);
				CLog::WriteAppLog(logdata);
				COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
				break;
			}
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::insert_into_2005((unsigned char*)Package,name,filelen);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s重复\n",name);
				CLog::WriteAppLog(logdata);
				remove(FilePath);
			}
			else
			{
				ret = 1;
			}
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;
			
		case 0x2006:
			
			Trans_2006_1 m_Trans_2006_1;
			memset(&m_Trans_2006_1,0x00,sizeof(m_Trans_2006_1));
			m_Trans_2006_1 = *(Trans_2006_1*)(Package + sizeof(MsgHead));
			w_Records = CommonFuncs::wordtowin(m_Trans_2006_1.w_Records);
			if(filelen != sizeof(MsgHead) + sizeof(Trans_2006_1)+ w_Records * sizeof(Trans_2006_2) + LEN_OF_PackageTail)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s格式错误\n",name);
				CLog::WriteAppLog(logdata);
				
				memset(errorfilename,0x00,sizeof(errorfilename));
				sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
				rename(FilePath,errorfilename);
				break;
			}
			if(w_Records == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s记录数为0,不予以处理\n",name);
				CLog::WriteAppLog(logdata);
				COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
				break;
			}
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::insert_into_2006((unsigned char*)Package,name,filelen);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s重复\n",name);
				CLog::WriteAppLog(logdata);
				remove(FilePath);
			}
			else
			{
				ret = 1;
				/* --- 转换成8004报文并且下发至SC --- */
				//Send_SC_8004((unsigned char*)Package);
			}
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;
			
		case 0x2007:
			
			Trans_2007_1 m_Trans_2007_1;
			memset(&m_Trans_2007_1,0x00,sizeof(m_Trans_2007_1));
			m_Trans_2007_1 = *(Trans_2007_1 *)(Package + sizeof(MsgHead));
			w_Records = CommonFuncs::wordtowin(m_Trans_2007_1.w_Records);
			if(filelen != sizeof(MsgHead) + sizeof(Trans_2007_1)+ w_Records * sizeof(Trans_2007_2) + LEN_OF_PackageTail)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s格式错误\n",name);
				CLog::WriteAppLog(logdata);
				
				memset(errorfilename,0x00,sizeof(errorfilename));
				sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
				rename(FilePath,errorfilename);
				break;
			}
			if(w_Records == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s记录数为0,不予以处理\n",name);
				CLog::WriteAppLog(logdata);
				COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
				break;
			}
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::insert_into_2007((unsigned char*)Package,name,filelen);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s重复\n",name);
				CLog::WriteAppLog(logdata);
				remove(FilePath);
			}
			else
			{
				ret = 1;
			}
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;
			
		case 0x2008:
			
			Trans_2008_1 m_Trans_2008_1;
			memset(&m_Trans_2008_1,0x00,sizeof(m_Trans_2008_1));
			m_Trans_2008_1 = *(Trans_2008_1 *)(Package + sizeof(MsgHead));
			w_Records = CommonFuncs::wordtowin(m_Trans_2008_1.w_Records);
			if(filelen != sizeof(MsgHead) + sizeof(Trans_2008_1)+ w_Records * sizeof(Trans_2008_2) + LEN_OF_PackageTail)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s格式错误\n",name);
				CLog::WriteAppLog(logdata);
				
				memset(errorfilename,0x00,sizeof(errorfilename));
				sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
				rename(FilePath,errorfilename);
				break;
			}
			if(w_Records == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s记录数为0,不予以处理\n",name);
				CLog::WriteAppLog(logdata);
				COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
				break;
			}
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::insert_into_2008((unsigned char*)Package,name,filelen);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s重复\n",name);
				CLog::WriteAppLog(logdata);
				remove(FilePath);
			}
			else
			{
				ret = 1;
			}
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;
			
		case 0x2009:
			
			Trans_2009_1 m_Trans_2009_1;
			memset(&m_Trans_2009_1,0x00,sizeof(m_Trans_2009_1));
			m_Trans_2009_1 = *(Trans_2009_1 *)(Package + sizeof(MsgHead));
			w_Records = CommonFuncs::wordtowin(m_Trans_2009_1.w_Records);
			if(filelen != sizeof(MsgHead) + sizeof(Trans_2009_1)+ w_Records * sizeof(Trans_2009_2) + LEN_OF_PackageTail)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s格式错误\n",name);
				CLog::WriteAppLog(logdata);
				
				memset(errorfilename,0x00,sizeof(errorfilename));
				sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
				rename(FilePath,errorfilename);
				break;
			}
			if(w_Records == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s记录数为0,不予以处理\n",name);
				CLog::WriteAppLog(logdata);
				COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
				break;
			}
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::insert_into_2009((unsigned char*)Package,name,filelen);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s重复\n",name);
				CLog::WriteAppLog(logdata);
				remove(FilePath);
			}
			else
			{
				ret = 1;
			}
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;
			
		case 0x200A:
			
			Trans_200A_1 m_Trans_200A_1;
			memset(&m_Trans_200A_1,0x00,sizeof(m_Trans_200A_1));
			m_Trans_200A_1 = *(Trans_200A_1 *)(Package + sizeof(MsgHead));
			w_Records = CommonFuncs::wordtowin(m_Trans_200A_1.w_Records);
			if(filelen != sizeof(MsgHead) + sizeof(Trans_200A_1)+ w_Records * sizeof(Trans_200A_2) + LEN_OF_PackageTail)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s格式错误\n",name);
				CLog::WriteAppLog(logdata);
				
				memset(errorfilename,0x00,sizeof(errorfilename));
				sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
				rename(FilePath,errorfilename);
				break;
			}
			if(w_Records == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s记录数为0,不予以处理\n",name);
				CLog::WriteAppLog(logdata);
				COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
				break;
			}
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::insert_into_200A((unsigned char*)Package,name,filelen);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s重复\n",name);
				CLog::WriteAppLog(logdata);
				remove(FilePath);
			}
			else
			{
				ret = 1;
			}
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;
			
		case 0x200B:
			
			Trans_200B_1 m_Trans_200B_1;
			memset(&m_Trans_200B_1,0x00,sizeof(m_Trans_200B_1));
			m_Trans_200B_1 = *(Trans_200B_1 *)(Package + sizeof(MsgHead));
			w_Records = CommonFuncs::wordtowin(m_Trans_200B_1.w_Records);
			if(filelen != sizeof(MsgHead) + sizeof(Trans_200B_1)+ w_Records * sizeof(Trans_200B_2) + LEN_OF_PackageTail)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s格式错误\n",name);
				CLog::WriteAppLog(logdata);
				
				memset(errorfilename,0x00,sizeof(errorfilename));
				sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
				rename(FilePath,errorfilename);
				break;
			}
			if(w_Records == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s记录数为0,不予以处理\n",name);
				CLog::WriteAppLog(logdata);
				COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
				break;
			}
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::insert_into_200B((unsigned char*)Package,name,filelen);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s重复\n",name);
				CLog::WriteAppLog(logdata);
				remove(FilePath);
			}
			else
			{
				ret = 1;
			}
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;
			
		case 0x200C:
			
			Trans_200C_1 m_Trans_200C_1;
			memset(&m_Trans_200C_1,0x00,sizeof(m_Trans_200C_1));
			m_Trans_200C_1 = *(Trans_200C_1 *)(Package + sizeof(MsgHead));
			w_Records = CommonFuncs::wordtowin(m_Trans_200C_1.w_Records);
			if(filelen != sizeof(MsgHead) + sizeof(Trans_200C_1)+ w_Records * sizeof(Trans_200C_2) + LEN_OF_PackageTail)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s格式错误\n",name);
				CLog::WriteAppLog(logdata);
				
				memset(errorfilename,0x00,sizeof(errorfilename));
				sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
				rename(FilePath,errorfilename);
				break;
			}
			if(w_Records == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s记录数为0,不予以处理\n",name);
				CLog::WriteAppLog(logdata);
				COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
				break;
			}
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::insert_into_200C((unsigned char*)Package,name,filelen);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s重复\n",name);
				CLog::WriteAppLog(logdata);
				remove(FilePath);
			}
			else
			{
				ret = 1;
			}
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;
			
		case 0x2010:
			
			Trans_2010 m_Trans_2010;
			memset(&m_Trans_2010,0x00,sizeof(m_Trans_2010));
			m_Trans_2010 = *(Trans_2010*)(Package + sizeof(MsgHead));

			if(filelen != sizeof(MsgHead) + sizeof(Trans_2010) + LEN_OF_PackageTail)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s格式错误\n",name);
				CLog::WriteAppLog(logdata);
				
				memset(errorfilename,0x00,sizeof(errorfilename));
				sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
				rename(FilePath,errorfilename);
				break;
			}
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::insert_into_2010((unsigned char*)Package,name,filelen);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s重复\n",name);
				CLog::WriteAppLog(logdata);
				remove(FilePath);
			}
			else
			{
				ret = 1;
			}
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;
			
		case 0x2011:
			
			Trans_2011_1 m_Trans_2011_1;
			memset(&m_Trans_2011_1,0x00,sizeof(m_Trans_2011_1));
			m_Trans_2011_1 = *(Trans_2011_1*)(Package + sizeof(MsgHead));
			dw_Records = CommonFuncs::dwordtowin(m_Trans_2011_1.dw_Records);
			if(filelen != sizeof(MsgHead) + sizeof(Trans_2011_1)+ dw_Records * sizeof(Trans_2011_2) + LEN_OF_PackageTail)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s格式错误\n",name);
				CLog::WriteAppLog(logdata);
				
				memset(errorfilename,0x00,sizeof(errorfilename));
				sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
				rename(FilePath,errorfilename);
				break;
			}
			if(dw_Records == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s记录数为0,不予以处理\n",name);
				CLog::WriteAppLog(logdata);
				COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
				break;
			}
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::insert_into_2011((unsigned char*)Package,name,filelen);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s重复\n",name);
				CLog::WriteAppLog(logdata);
				remove(FilePath);
			}
			else
			{
				ret = 1;
			}
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;
			
		case 0x2012:
			
			Trans_2012_1 m_Trans_2012_1;
			memset(&m_Trans_2012_1,0x00,sizeof(m_Trans_2012_1));
			m_Trans_2012_1 = *(Trans_2012_1*)(Package + sizeof(MsgHead));
			dw_Records = CommonFuncs::dwordtowin(m_Trans_2012_1.dw_Records);
			if(filelen != sizeof(MsgHead) + sizeof(Trans_2012_1)+ dw_Records * sizeof(Trans_2012_2) + LEN_OF_PackageTail)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s格式错误\n",name);
				CLog::WriteAppLog(logdata);
				
				memset(errorfilename,0x00,sizeof(errorfilename));
				sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
				rename(FilePath,errorfilename);
				break;
			}
			if(dw_Records == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s记录数为0,不予以处理\n",name);
				CLog::WriteAppLog(logdata);
				COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
				break;
			}
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::insert_into_2012((unsigned char*)Package,name,filelen);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s重复\n",name);
				CLog::WriteAppLog(logdata);
				remove(FilePath);
			}
			else
			{
				ret = 1;
			}
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;
			
		case 0x2013:
			
			Trans_2013_1 m_Trans_2013_1;
			memset(&m_Trans_2013_1,0x00,sizeof(m_Trans_2013_1));
			m_Trans_2013_1 = *(Trans_2013_1*)(Package + sizeof(MsgHead));
			w_Records = CommonFuncs::wordtowin(m_Trans_2013_1.w_Records);
			if(filelen != sizeof(MsgHead) + sizeof(Trans_2013_1)+ w_Records * sizeof(Trans_2013_2) + LEN_OF_PackageTail)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s格式错误\n",name);
				CLog::WriteAppLog(logdata);
				
				memset(errorfilename,0x00,sizeof(errorfilename));
				sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
				rename(FilePath,errorfilename);
				break;
			}
			if(w_Records == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s记录数为0,不予以处理\n",name);
				CLog::WriteAppLog(logdata);
				COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
				break;
			}
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::insert_into_2013((unsigned char*)Package,name,filelen);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s重复\n",name);
				CLog::WriteAppLog(logdata);
				remove(FilePath);
			}
			else
			{
				ret = 1;
			}
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;
			
		case 0x2014:
			
			Trans_2014_1 m_Trans_2014_1;
			memset(&m_Trans_2014_1,0x00,sizeof(m_Trans_2014_1));
			m_Trans_2014_1 = *(Trans_2014_1*)(Package + sizeof(MsgHead));
			w_Records = CommonFuncs::wordtowin(m_Trans_2014_1.w_Records);
			if(filelen != sizeof(MsgHead) + sizeof(Trans_2014_1)+ w_Records * sizeof(Trans_2014_2) + LEN_OF_PackageTail)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s格式错误\n",name);
				CLog::WriteAppLog(logdata);
				
				memset(errorfilename,0x00,sizeof(errorfilename));
				sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
				rename(FilePath,errorfilename);
				break;
			}
			if(w_Records == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s记录数为0,不予以处理\n",name);
				CLog::WriteAppLog(logdata);
				COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
				break;
			}
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::insert_into_2014((unsigned char*)Package,name,filelen);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s重复\n",name);
				CLog::WriteAppLog(logdata);
				remove(FilePath);
			}
			else
			{
				ret = 1;
			}
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;
			
		case 0x2015:
			
			Trans_2015 m_Trans_2015;
			memset(&m_Trans_2015,0x00,sizeof(m_Trans_2015));
			m_Trans_2015 = *(Trans_2015*)(Package + sizeof(MsgHead));

			if(filelen != sizeof(MsgHead) + sizeof(Trans_2015) + LEN_OF_PackageTail)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s格式错误\n",name);
				CLog::WriteAppLog(logdata);
				
				memset(errorfilename,0x00,sizeof(errorfilename));
				sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
				rename(FilePath,errorfilename);
				break;
			}
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::insert_into_2015((unsigned char*)Package,name,filelen);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s重复\n",name);
				CLog::WriteAppLog(logdata);
				remove(FilePath);
			}
			else
			{
				ret = 1;
			}
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;
			
		case 0x2016:
			
			Trans_2016_1 m_Trans_2016_1;
			memset(&m_Trans_2016_1,0x00,sizeof(m_Trans_2016_1));
			m_Trans_2016_1 = *(Trans_2016_1*)(Package + sizeof(MsgHead));
			dw_Records = CommonFuncs::dwordtowin(m_Trans_2016_1.dw_Records);
			if(filelen != sizeof(MsgHead) + sizeof(Trans_2016_1)+ dw_Records * sizeof(Trans_2016_2) + LEN_OF_PackageTail)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s格式错误\n",name);
				CLog::WriteAppLog(logdata);
				
				memset(errorfilename,0x00,sizeof(errorfilename));
				sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
				rename(FilePath,errorfilename);
				break;
			}
			if(dw_Records == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s记录数为0,不予以处理\n",name);
				CLog::WriteAppLog(logdata);
				COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
				break;
			}
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::insert_into_2016((unsigned char*)Package,name,filelen);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s重复\n",name);
				CLog::WriteAppLog(logdata);
				remove(FilePath);
			}
			else
			{
				ret = 1;
			}
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;
			
		case 0x2017:
			
			Trans_2017_1 m_Trans_2017_1;
			memset(&m_Trans_2017_1,0x00,sizeof(m_Trans_2017_1));
			m_Trans_2017_1 = *(Trans_2017_1*)(Package + sizeof(MsgHead));
			dw_Records = CommonFuncs::dwordtowin(m_Trans_2017_1.dw_Records);
			if(filelen != sizeof(MsgHead) + sizeof(Trans_2017_1)+ dw_Records * sizeof(Trans_2017_2) + LEN_OF_PackageTail)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s格式错误\n",name);
				CLog::WriteAppLog(logdata);
				
				memset(errorfilename,0x00,sizeof(errorfilename));
				sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
				rename(FilePath,errorfilename);
				break;
			}
			if(dw_Records == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s记录数为0,不予以处理\n",name);
				CLog::WriteAppLog(logdata);
				COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
				break;
			}
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::insert_into_2017((unsigned char*)Package,name,filelen);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s重复\n",name);
				CLog::WriteAppLog(logdata);
				remove(FilePath);
			}
			else
			{
				ret = 1;
			}
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;
			
		case 0x2018:
			
			Trans_2018_1 m_Trans_2018_1;
			memset(&m_Trans_2018_1,0x00,sizeof(m_Trans_2018_1));
			m_Trans_2018_1 = *(Trans_2018_1*)(Package + sizeof(MsgHead));
			w_Records = CommonFuncs::wordtowin(m_Trans_2018_1.w_Records);
			if(filelen != sizeof(MsgHead) + sizeof(Trans_2018_1)+ w_Records * sizeof(Trans_2018_2) + LEN_OF_PackageTail)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s格式错误\n",name);
				CLog::WriteAppLog(logdata);
				
				memset(errorfilename,0x00,sizeof(errorfilename));
				sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
				rename(FilePath,errorfilename);
				break;
			}
			if(w_Records == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s记录数为0,不予以处理\n",name);
				CLog::WriteAppLog(logdata);
				COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
				break;
			}
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::insert_into_2018((unsigned char*)Package,name,filelen);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s重复\n",name);
				CLog::WriteAppLog(logdata);
				remove(FilePath);
			}
			else
			{
				ret = 1;
			}
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;
			
		case 0x2019:
			
			Trans_2019_1 m_Trans_2019_1;
			memset(&m_Trans_2019_1,0x00,sizeof(m_Trans_2019_1));
			m_Trans_2019_1 = *(Trans_2019_1*)(Package + sizeof(MsgHead));
			w_Records = CommonFuncs::wordtowin(m_Trans_2019_1.w_Records);
			if(filelen != sizeof(MsgHead) + sizeof(Trans_2019_1)+ w_Records * sizeof(Trans_2019_2) + LEN_OF_PackageTail)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s格式错误\n",name);
				CLog::WriteAppLog(logdata);
				
				memset(errorfilename,0x00,sizeof(errorfilename));
				sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
				rename(FilePath,errorfilename);
				break;
			}
			if(w_Records == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s记录数为0,不予以处理\n",name);
				CLog::WriteAppLog(logdata);
				COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
				break;
			}
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::insert_into_2019((unsigned char*)Package,name,filelen);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC对账文件:%s重复\n",name);
				CLog::WriteAppLog(logdata);
				remove(FilePath);
			}
			else
			{
				ret = 1;
			}
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;

		case 0x3009:
			
			Trans_3009_1 m_Trans_3009_1;
			memset(&m_Trans_3009_1,0x00,sizeof(m_Trans_3009_1));
			m_Trans_3009_1 = *(Trans_3009_1 *)(Package + sizeof(MsgHead));
			w_Records = CommonFuncs::wordtowin(m_Trans_3009_1.w_Records);
			if(filelen != sizeof(MsgHead) + sizeof(Trans_3009_1)+ w_Records * sizeof(Trans_3009_2) + LEN_OF_PackageTail)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC非即时退换卡鉴定结果文件:%s格式错误\n",name);
				CLog::WriteAppLog(logdata);
				
				memset(errorfilename,0x00,sizeof(errorfilename));
				sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
				rename(FilePath,errorfilename);
				break;
			}
			if(w_Records == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC非即时退换卡鉴定结果文件:%s记录数为0,不予以处理\n",name);
				CLog::WriteAppLog(logdata);
				COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
				break;
			}
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::insert_into_3009((unsigned char*)Package,name,filelen);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC非即时退换卡鉴定结果文件:%s重复\n",name);
				CLog::WriteAppLog(logdata);
				remove(FilePath);
			}
			else
			{
				ret = 1;
			}
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;
			
			
		default:
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"ACC非即时退换卡鉴定结果文件:%s类型错误\n",name);
				CLog::WriteAppLog(logdata);
			break;
	}
	/* --- 释放内存 ---- */
	free(Package);
	Package = NULL;
	
	return ret;
}

/* ------------------------------------------------------
         功能描述:   处理PACC对账文件
         失败返回 0
--------------------------------------------------------- */
int ACC::DealAccFile3002(char *path,unsigned short wMsgCode,int lineIndex,char *name)
{
	char 							logdata[1024];
	char 							FilePath[256],errorfilename[256];
	struct stat       buf;
	char              *Package;
	int               filelen;
	
	unsigned short 	  w_Records;
	unsigned int 	    dw_Records;
	
	time_t          	local_time;
	struct tm      		newtime;
	int             	rtn,mday;
	int               ret;
	
	
	 
	ret = 0;
	memset(logdata,0x00,sizeof(logdata));
	sprintf(logdata, "收到来自ACC的%s对账/库存文件\n",name);
	CLog::WriteAppLog(logdata);
		
	rtn = GetAccParamFile(wMsgCode,lineIndex,name);	
	if(rtn != 1)
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata, "ACC对账文件(%s)下载失败\n",name);
		CLog::WriteAppLog(logdata);
		return 0;
	}	
		
	/* --- 读取文件内容 ------- */
	memset(FilePath,0x00,sizeof(FilePath));
	sprintf(FilePath,"%s%s",path,name);
	stat(FilePath,&buf);
	filelen = buf.st_size;
	if(filelen == 0)
	{
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"ACC对账/库存:%s格式错误,长度为0\n",name);
			CLog::WriteAppLog(logdata);
			
			return ret;
	}
	Package = (char*)malloc(filelen);
	memset(Package,0x00,filelen);
	CommonFuncs::ReadFileBuffer(FilePath,Package);
	/* --- 系统时间 --- */
	time(&local_time);
	memset(&newtime,0x00,sizeof(struct tm));
	localtime_r(&local_time,&newtime);
	mday = newtime.tm_mday;
 
	while(1)
	{
		Trans_3002_1 m_Trans_3002_1;
		memset(&m_Trans_3002_1,0x00,sizeof(m_Trans_3002_1));
		m_Trans_3002_1 = *(Trans_3002_1*)(Package + sizeof(MsgHead));
		w_Records = CommonFuncs::wordtowin(m_Trans_3002_1.w_Records);
		if(filelen != sizeof(MsgHead) + sizeof(Trans_3002_1)+ w_Records * sizeof(Trans_3002_2) + LEN_OF_PackageTail)
		{
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"ACC库存文件:%s格式错误\n",name);
			CLog::WriteAppLog(logdata);
			
			memset(errorfilename,0x00,sizeof(errorfilename));
			sprintf(errorfilename,"%s%s",CConfig::ErrorPath,name);
			rename(FilePath,errorfilename);
			break;
		}
		if(w_Records == 0)
		{
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"ACC库存文件:%s记录数为0,不予以处理\n",name);
			CLog::WriteAppLog(logdata);
			COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			break;
		}
		sem_wait(&ORA::ms_CSSQL);
		rtn = ORA::insert_into_3002((unsigned char*)Package,name,filelen);
		sem_post(&ORA::ms_CSSQL);
		if(rtn == 0)
		{
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"库存对账文件:%s重复\n",name);
			CLog::WriteAppLog(logdata);
			remove(FilePath);
		}
		else
		{
			ret = 1;
		}
		CPara::Copy_ParamFile(path,name,name,CConfig::StockPath);
		COtherFuncs::SaveHistoryData(2,FilePath,name,mday);
			
		break;
	} 
	/* --- 释放内存 ---- */
	free(Package);
	Package = NULL;
	
	return ret;
}
 


/* ----------------------------------------------------------------
            功能描述:   发送数据
---------------------------------------------------------------- */
int ACC::PrepareSendData_ACC()
{
	int             ret;
	char            filename[MAX_PATH];
	time_t          local_time;
	struct tm       newtime;
	int             mday;
	
	struct stat     buf;
	struct dirent   *dirp;
	DIR             *dp;
	char            path[MAX_PATH];
	char            errormsg[500];
	char          	c_msgtype[5];
	int           	msgtype;
	char            *stopstring;
	
	char            logdata[1024];

	FtpMsg 					m_FtpMsg;
	int							lineId,lineIndex;


  ret = 1;
  /* -------- 打开数据目录 ---------- */
  memset(path,0x00,sizeof(path));
	strcpy(path,CConfig::SendFilePath_ACC);
	if((dp = opendir(path)) == NULL)
	{
		return ret;
	}
	/* -------- 读取数据目录 ---------- */
	while((dirp = readdir(dp)) != NULL)
	{
		if(strcmp(dirp->d_name,".") == 0 ||
		   strcmp(dirp->d_name,"..") == 0)
		   	continue;
		/* -------- 获得待发送的一个数据文件 ---------- */
		memset(filename,0x00,sizeof(filename));
		strcpy(filename,CConfig::SendFilePath_ACC);
		strcat(filename,dirp->d_name);
		
		/* --- 获得线路标识类型 ---XX_XXXX_ XXXXXXXXYYYYMMDDHHMISSXX-- */
		memset(c_msgtype,0x00,sizeof(c_msgtype));
		memcpy(c_msgtype,dirp->d_name,2);
		lineId = strtoul(c_msgtype,&stopstring,16);
 
		/* --- 获得报文类型 --- */
		memset(c_msgtype,0x00,sizeof(c_msgtype));
		memcpy(c_msgtype,dirp->d_name+3,4);
		msgtype = strtoul(c_msgtype,&stopstring,16);
 
		/* -------- 删除非正常类型报文数据 ---------- */
		if(msgtype != 0x1001 && msgtype != 0x1002 && msgtype != 0x1003 && 
			 msgtype != 0x1004 && msgtype != 0x1005 && msgtype != 0x1006 && 
			 msgtype != 0x4001 && msgtype != 0x4002 && msgtype != 0x4003 && 
			 msgtype != 0x5001 && msgtype != 0x5002 && msgtype != 0x5003 && 
			 msgtype != 0x5004 && msgtype != 0x5005 && msgtype != 0x5020 &&
			 msgtype != 0x3001 && msgtype != 0x3003 && msgtype != 0x3005 && msgtype != 0x3006 && 
			 msgtype != 0x3007 && msgtype != 0x3008 &&
			 msgtype != 0x7001 && msgtype != 0x7002 && msgtype != 0x2001)
		{
			memset(errormsg,0x00,sizeof(errormsg));
			sprintf(errormsg,"文件格式错误:%s(PrepareSendData_ACC)\n",filename);
			CLog::WriteAppLog(errormsg);
				
			remove(filename);
			continue;
		}
		
				/* --- 系统时间 --- */
		time(&local_time);
		memset(&newtime,0x00,sizeof(struct tm));
		localtime_r(&local_time,&newtime);
		mday = newtime.tm_mday;

		stat(filename,&buf ); 
		if(local_time - buf.st_ctime >= 5)
		{
			lineIndex = getlineinfo(lineId);
			if(lineIndex == -1)
			{
				memset(errormsg,0x00,sizeof(errormsg));
				sprintf(errormsg,"获取线路索引错误:%s(PrepareSendData_ACC)\n",filename);
				CLog::WriteAppLog(errormsg);
			}
			memset(&m_FtpMsg,0x00,sizeof(FtpMsg));
			ret = GetFtpMsg(msgtype,lineIndex,&m_FtpMsg);			
			if(ret == -1)
			{
				memset(errormsg,0x00,sizeof(errormsg));
				sprintf(errormsg,"获取线路FTP信息错误:%s(PrepareSendData_ACC)\n",filename);
				CLog::WriteAppLog(errormsg);
			}
			/* ---------- 相关FTP信息 -------*/
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"PrepareSendData_ACC 相关FTP信息(ftpIP:%s-ftpPort:%d-ftpUser:%s-ftpPass:%s-ftpPath:%s)\n",m_FtpMsg.ftpIP,m_FtpMsg.ftpPort,m_FtpMsg.ftpUser,m_FtpMsg.ftpPass,m_FtpMsg.ftpPath);
			CLog::WriteAppLog(logdata);
 
			/* ---------- put文件至服务器 -------*/
			ret = CSocket::Ftp_PutFile(m_FtpMsg.ftpIP,m_FtpMsg.ftpPort,m_FtpMsg.ftpUser,m_FtpMsg.ftpPass,CConfig::SendFilePath_ACC,dirp->d_name,m_FtpMsg.ftpPath,NULL,dirp->d_name);
 
			if(ret != 1)
			{
				memset(errormsg,0x00,sizeof(errormsg));
				sprintf(errormsg,"FTP发送ACC文件失败(%s),失败代码:%d\n",filename,ret);
				CLog::WriteAppLog(errormsg);
				break;
			}
			else
			{
				memset(errormsg,0x00,sizeof(errormsg));
				sprintf(errormsg,"FTP发送ACC文件成功(%s)\n",filename);
				CLog::WriteAppLog(errormsg);
				/* -------- 删除文件 ---------
				remove(filename);- */
				
				/* --- 旧文件移到历史目录 --- */
				COtherFuncs::SaveHistoryData(2,filename,dirp->d_name,mday);
 
				/* --- 更新FTP文件表 --- */
				if(msgtype == 0x1001 || msgtype == 0x1002 || msgtype == 0x1003 ||
					 msgtype == 0x1004 || msgtype == 0x1005 || msgtype == 0x1006 ||
				   msgtype == 0x5001 || msgtype == 0x5002 || msgtype == 0x5003 ||
				   msgtype == 0x5004 || msgtype == 0x5005)
				{
					ORA::Update_Ftp_FileName(dirp->d_name);
				}
			}
	  }
	}   

	/* -------- 关闭数据目录 ---------- */
	closedir(dp);

	return ret;
}

/* ------------------------------------------------------
      功能描述:   发送数据到清分前置机线程函数
--------------------------------------------------------- */
void* ACC::ThreadSendData_ACC(void* lpParam)
{
	pthread_detach(pthread_self());
	
	while(1)
	{
		CommonFuncs::my_sleep(5);
		
		PrepareSendData_ACC();
	}

	return NULL;
}
	

/* ------------------------------------------------------
      功能描述:   文件拆分
--------------------------------------------------------- */
int  ACC::AppartFile(char *filename,char *name,char *filebuf,unsigned short msgtype,int filelen,int lineid)
{
	int  	filecount,read_len,record_len;
	int 	pkglen,sndfilelen;
	char 	*sendbuf;
	int  	i,rtn;
	DWORD 	sessionnum;
	char  	sendfilename[MAX_PATH],insertfilename[MAX_PATH],errormsg[1024];
	MsgHead       m_MsgHead;
	WORD         PkgRecords;
	char         c_PkgID[30];
	BYTE         b_PkgID;
	int          MaxPkgRecords;
	Lcc_Ftp_File m_Lcc_Ftp_File;
	
	read_len = 0;
	
	/* --- 包体长度 --- */
	if(msgtype == 0x1001)
	{
		filecount = filelen/(CConfig::g_MaxRecord*sizeof(Trans_1001_2));
		if(filecount >= 0xff)
			MaxPkgRecords = filelen/(200*sizeof(Trans_1001_2));
		else
			MaxPkgRecords = CConfig::g_MaxRecord;
		pkglen     = sizeof(Trans_1001_2)*MaxPkgRecords;
		record_len = sizeof(Trans_1001_2);
	}
	else if(msgtype == 0x1002)
	{
		filecount = filelen/(CConfig::g_MaxRecord*sizeof(Trans_1002_2));
		if(filecount >= 0xff)
			MaxPkgRecords = filelen/(200*sizeof(Trans_1002_2));
		else
			MaxPkgRecords = CConfig::g_MaxRecord;
		pkglen     = sizeof(Trans_1002_2)*MaxPkgRecords;
		record_len = sizeof(Trans_1002_2);
	}
	else if(msgtype == 0x1003)
	{
		filecount = filelen/(CConfig::g_MaxRecord*sizeof(Trans_1003_2));
		if(filecount >= 0xff)
			MaxPkgRecords = filelen/(200*sizeof(Trans_1003_2));
		else
			MaxPkgRecords = CConfig::g_MaxRecord;
		pkglen     = sizeof(Trans_1003_2)*MaxPkgRecords;
		record_len = sizeof(Trans_1003_2);
	}
	else if(msgtype == 0x1004)
	{
		filecount = filelen/(CConfig::g_MaxRecord*sizeof(Trans_1004_2));
		if(filecount >= 0xff)
			MaxPkgRecords = filelen/(200*sizeof(Trans_1004_2));
		else
			MaxPkgRecords = CConfig::g_MaxRecord;
		pkglen     = sizeof(Trans_1004_2)*MaxPkgRecords;
		record_len = sizeof(Trans_1004_2);
	}
	else if(msgtype == 0x1005)
	{
		filecount = filelen/(CConfig::g_MaxRecord*sizeof(Trans_1005_2));
		if(filecount >= 0xff)
			MaxPkgRecords = filelen/(200*sizeof(Trans_1005_2));
		else
			MaxPkgRecords = CConfig::g_MaxRecord;
		pkglen     = sizeof(Trans_1005_2)*MaxPkgRecords;
		record_len = sizeof(Trans_1005_2);
	}
	else if(msgtype == 0x1006)
	{
		filecount = filelen/(CConfig::g_MaxRecord*sizeof(Trans_1006_2));
		if(filecount >= 0xff)
			MaxPkgRecords = filelen/(200*sizeof(Trans_1006_2));
		else
			MaxPkgRecords = CConfig::g_MaxRecord;
		pkglen     = sizeof(Trans_1006_2)*MaxPkgRecords;
		record_len = sizeof(Trans_1006_2);
	}	
	else if(msgtype == 0x5001)
	{
		filecount = filelen/(CConfig::g_MaxRecord*sizeof(Trans_5001_2));
		if(filecount >= 0xff)
			MaxPkgRecords = filelen/(200*sizeof(Trans_5001_2));
		else
			MaxPkgRecords = CConfig::g_MaxRecord;
		pkglen     = sizeof(Trans_5001_2)*MaxPkgRecords;
		record_len = sizeof(Trans_5001_2);
	}
	else if(msgtype == 0x5002)
	{
		filecount = filelen/(CConfig::g_MaxRecord*sizeof(Trans_5002_2));
		if(filecount >= 0xff)
			MaxPkgRecords = filelen/(200*sizeof(Trans_5002_2));
		else
			MaxPkgRecords = CConfig::g_MaxRecord;
		pkglen     = sizeof(Trans_5002_2)*MaxPkgRecords;
		record_len = sizeof(Trans_5002_2);
	}
	else if(msgtype == 0x5003)
	{
		filecount = filelen/(CConfig::g_MaxRecord*sizeof(Trans_5003_2));
		if(filecount >= 0xff)
			MaxPkgRecords = filelen/(200*sizeof(Trans_5003_2));
		else
			MaxPkgRecords = CConfig::g_MaxRecord;
		pkglen     = sizeof(Trans_5003_2)*MaxPkgRecords;
		record_len = sizeof(Trans_5003_2);
	}
	else
	{
		memset(errormsg,0x00,sizeof(errormsg));
		sprintf(errormsg,"未定义的文件类型:%04X\n",msgtype);
		CLog::WriteAppLog(errormsg);
		
		return 1;
	}
	
	memset(errormsg,0x00,sizeof(errormsg));
	sprintf(errormsg,"------ 文件%s(文件大小:%d)开始拆分\n",filename,filelen);
	CLog::WriteAppLog(errormsg);
	
	/* --- 文件拆分个数 --- */
	filecount = filelen / pkglen;
	sndfilelen = sizeof(MsgHead) + 2 + pkglen + LEN_OF_PackageTail;
	/* --- 拆分文件 --- */
	for(i=0;i<filecount;i++)
	{
		/* --- 会话流水号 --- */
		sem_wait(&ORA::ms_CSSQL);
		
		sessionnum = ORA::get_session_number();
		b_PkgID = (BYTE)ORA::get_pkg_number();
		if(sessionnum == 0)/* --- 数据库异常,停止应用 --- */
		{
			memset(errormsg,0x00,sizeof(errormsg));
			strcpy(errormsg,"get_session_number error,db error\n");
			CLog::WriteAppLog(errormsg);
			
			CMainManage::RestartProgram(NULL);
		}
		/* --- 包头 --- */
		memset(&m_MsgHead,0x00,sizeof(MsgHead));
		memset(c_PkgID,0x00,sizeof(c_PkgID));
		
		BYTE 			b_code[4];
		memset(b_code,0x00,sizeof(b_code)); 
		CommonFuncs::chartobyte(CConfig::ms_MlcLineList[ACC::getlineinfo(lineid)].LINE_NID,(char*)b_code,sizeof(b_code));
			
		CPackage::Create_Pkg_Head(b_PkgID,&m_MsgHead,pkglen+2,msgtype,sessionnum,CConfig::g_ACCIDCode,c_PkgID,b_code);

		sem_post(&ORA::ms_CSSQL);
		/* --- 包文 --- */
		sendbuf = (char*)malloc(sndfilelen);
		memset(sendbuf,0x00,sndfilelen);
		PkgRecords = CommonFuncs::wordtowin(MaxPkgRecords);
		memcpy(sendbuf+sizeof(MsgHead),&PkgRecords,2);
		CPackage::CreatePackage(2,(char*)sendbuf,&m_MsgHead,(char*)(filebuf+read_len),pkglen);
		read_len = read_len + pkglen;
		/* --- 生成文件于待发送目录 --- */		
		memset(sendfilename,0x00,sizeof(sendfilename));
		sprintf(sendfilename,"%s%02X_%04X_%s",CConfig::SendFilePath_ACC,lineid,msgtype,c_PkgID);
		rtn = CommonFuncs::WriteBufferFile("wb",sendfilename,sendbuf,sndfilelen);
		/* --- 生成文件于待入库目录 --- */
		if(msgtype == 0x1001 || msgtype == 0x1002 || msgtype == 0x1003 ||
			 msgtype == 0x1004 || msgtype == 0x1005 || msgtype == 0x1006)
		{
			memset(insertfilename,0x00,sizeof(insertfilename));
			sprintf(insertfilename,"%s%02X_%04X_%s",CConfig::InsertTransFilePath,lineid,msgtype,c_PkgID);
			rtn = CommonFuncs::WriteBufferFile("wb",insertfilename,sendbuf,sndfilelen);
		}
		/* --- 释放内存 --- */
		free(sendbuf);
		sendbuf = NULL;
		if(rtn == 0)
		{
			memset(errormsg,0x00,sizeof(errormsg));
			sprintf(errormsg,"Write file(%s) error errno:%d strerror:%s\n",sendfilename,errno, strerror(errno));
			CLog::WriteAppLog(errormsg);
			return 0;
		}
		/* --- 更新FTP文件表 --- */
		if(msgtype == 0x1001 || msgtype == 0x1002 || msgtype == 0x1003 ||
			 msgtype == 0x1004 || msgtype == 0x1005 || msgtype == 0x1006 ||
			 msgtype == 0x5001 || msgtype == 0x5002 || msgtype == 0x5003)
		{
			memset(&m_Lcc_Ftp_File,0x00,sizeof(Lcc_Ftp_File));
			sprintf(m_Lcc_Ftp_File.FILE_NAME,"%02X_%04X_%s",lineid,msgtype,c_PkgID);
			m_Lcc_Ftp_File.FILE_LENGTH = sndfilelen;
			m_Lcc_Ftp_File.FILE_RECORDS = MaxPkgRecords;
			strcpy(m_Lcc_Ftp_File.BELONGS_FILE_NAME,name);
			ORA::Insert_Ftp_FileName(&m_Lcc_Ftp_File);
		}
		
		memset(errormsg,0x00,sizeof(errormsg));
		sprintf(errormsg,"第%d个文件%s(文件大小:%d)拆分完成\n",i+1,sendfilename,sndfilelen);
		CLog::WriteAppLog(errormsg);
	}
	
	/* --- 文件最后未达记录数组成包 --- */
	if(read_len < filelen)
	{
		/* --- 会话流水号 --- */
		sem_wait(&ORA::ms_CSSQL);
		
		sessionnum = ORA::get_session_number();
		b_PkgID = (BYTE)ORA::get_pkg_number();
		if(sessionnum == 0)/* --- 数据库异常,停止应用 --- */
		{
			memset(errormsg,0x00,sizeof(errormsg));
			strcpy(errormsg,"get_session_number error,db error\n");
			CLog::WriteAppLog(errormsg);
			
			CMainManage::RestartProgram(NULL);
		}
		/* --- 包头 --- */
		memset(&m_MsgHead,0x00,sizeof(MsgHead));
		memset(c_PkgID,0x00,sizeof(c_PkgID));
		pkglen = filelen - read_len;
		
			BYTE 			b_code[4];
		memset(b_code,0x00,sizeof(b_code)); 
		CommonFuncs::chartobyte(CConfig::ms_MlcLineList[ACC::getlineinfo(lineid)].LINE_NID,(char*)b_code,sizeof(b_code));
			
		CPackage::Create_Pkg_Head(b_PkgID,&m_MsgHead,pkglen+2,msgtype,sessionnum,CConfig::g_ACCIDCode,c_PkgID,b_code);
 
		sem_post(&ORA::ms_CSSQL);
		/* --- 包文 --- */
		sndfilelen = sizeof(MsgHead) + 2 + pkglen + LEN_OF_PackageTail;
		sendbuf = (char*)malloc(sndfilelen);
		memset(sendbuf,0x00,sndfilelen);
		PkgRecords = CommonFuncs::wordtowin(pkglen/record_len);
		memcpy(sendbuf+sizeof(MsgHead),&PkgRecords,2);
		CPackage::CreatePackage(2,(char*)sendbuf,&m_MsgHead,(char*)(filebuf+read_len),pkglen);
		/* --- 生成文件于发送目录 --- */
		memset(sendfilename,0x00,sizeof(sendfilename));
		sprintf(sendfilename,"%s%02X_%04X_%s",CConfig::SendFilePath_ACC,lineid,msgtype,c_PkgID);
		rtn = CommonFuncs::WriteBufferFile("wb",sendfilename,sendbuf,sndfilelen);
		/* --- 生成文件于待入库目录 --- */
		if(msgtype == 0x1001 || msgtype == 0x1002 || msgtype == 0x1003 ||
			 msgtype == 0x1004 || msgtype == 0x1005 || msgtype == 0x1006)
		{
			memset(insertfilename,0x00,sizeof(insertfilename));
			sprintf(insertfilename,"%s%02X_%04X_%s",CConfig::InsertTransFilePath,lineid,msgtype,c_PkgID);
			rtn = CommonFuncs::WriteBufferFile("wb",insertfilename,sendbuf,sndfilelen);
		}
		/* --- 释放内存 --- */
		free(sendbuf);
		sendbuf = NULL;
		if(rtn == 0)
		{
			memset(errormsg,0x00,sizeof(errormsg));
			sprintf(errormsg,"Write file(%s) error errno:%d strerror:%s\n",sendfilename,errno, strerror(errno));
			CLog::WriteAppLog(errormsg);
			return 0;
		}
		/* --- 更新FTP文件表 --- */
		if(msgtype == 0x1001 || msgtype == 0x1002 || msgtype == 0x1003 ||
			 msgtype == 0x1004 || msgtype == 0x1005 || msgtype == 0x1006 ||
			 msgtype == 0x5001 || msgtype == 0x5002 || msgtype == 0x5003)
		{
			memset(&m_Lcc_Ftp_File,0x00,sizeof(Lcc_Ftp_File));
			sprintf(m_Lcc_Ftp_File.FILE_NAME,"%02X_%04X_%s",lineid,msgtype,c_PkgID);
			m_Lcc_Ftp_File.FILE_LENGTH = sndfilelen;
			m_Lcc_Ftp_File.FILE_RECORDS = pkglen/record_len;
			strcpy(m_Lcc_Ftp_File.BELONGS_FILE_NAME,name);
			ORA::Insert_Ftp_FileName(&m_Lcc_Ftp_File);
		}
		
		memset(errormsg,0x00,sizeof(errormsg));
		sprintf(errormsg,"第%d个文件%s(文件大小:%d)拆分完成\n",filecount+1,sendfilename,sndfilelen);
		CLog::WriteAppLog(errormsg);
	} 	
	
	memset(errormsg,0x00,sizeof(errormsg));
	sprintf(errormsg,"------ 文件%s拆分结束\n",filename);
	CLog::WriteAppLog(errormsg);
	
	return 1;
}	
	
	
/* ------------------------------------------------------
      功能描述:   生成发送文件
--------------------------------------------------------- */
int ACC::CreateSendFile()
{
	char            tempfilename[MAX_PATH],errorfilename[MAX_PATH];
	char            errormsg[500];
	struct dirent   *dirp;
	DIR             *dp;
	char            path[MAX_PATH];
	time_t          local_time;
	struct stat     buf;
	int             rtn;
	
	char            *filebuf;
	char            c_msgtype[5],c_sec[3],*stopstring;
	int             msgtype;
	struct tm      newtime;
	int             mday;
	int             filesec,syssec,secgap;
	int             filehour,syshour;
	int							lineid;
  
  /* -------- 打开目录 ---------- */
  memset(path,0x00,sizeof(path));
	strcpy(path,CConfig::TempSendFilePath);
	if((dp = opendir(path)) == NULL)
	{
		return 1;
	}

	/* -------- 读取目录 ---------- */
	while((dirp = readdir(dp)) != NULL)
	{
		if(strcmp(dirp->d_name,".") == 0 ||
		   strcmp(dirp->d_name,"..") == 0)
		   	continue;

		/* --- 读取文件 --- */
		memset(tempfilename,0x00,sizeof(tempfilename));
		strcpy(tempfilename,CConfig::TempSendFilePath);
		strcat(tempfilename,dirp->d_name);
		
		/* --- 系统时间 --- */
		time(&local_time);
		memset(&newtime,0x00,sizeof(struct tm));
		localtime_r(&local_time,&newtime);
		mday = newtime.tm_mday;
		syssec = newtime.tm_min;
		syshour = newtime.tm_hour;
		secgap = syssec / CConfig::g_MaxDelay;
		/* --- 获得文件生成时间 --- */
		memset(c_sec,0x00,sizeof(c_sec));
		memcpy(c_sec,dirp->d_name+13,2);
		filehour = atoi(c_sec);
		memset(c_sec,0x00,sizeof(c_sec));
		memcpy(c_sec,dirp->d_name+15,2);
		filesec = atoi(c_sec);
		/* --- 获得报文类型 --- */
		memset(c_msgtype,0x00,sizeof(c_msgtype));
		memcpy(c_msgtype,dirp->d_name,4);
		msgtype = strtoul(c_msgtype,&stopstring,16);

		/* --- 获得线路标识 -1001.202103241646.0D.dat-- */
		memset(c_msgtype,0x00,sizeof(c_msgtype));
		memcpy(c_msgtype,dirp->d_name + 18,2);
		lineid = strtoul(c_msgtype,&stopstring,16);
 
		if(msgtype != 0x1001 && msgtype != 0x1002 && msgtype != 0x1003 && 
			 msgtype != 0x1004 && msgtype != 0x1005 && msgtype != 0x1006 &&
			 msgtype != 0x5001 && msgtype != 0x5002 && msgtype != 0x5003)
		{
			memset(errormsg,0x00,sizeof(errormsg));
			sprintf(errormsg,"文件格式错误:%s(CreateSendFile)\n",tempfilename);
			CLog::WriteAppLog(errormsg);
			memset(errorfilename,0x00,sizeof(errorfilename));
			strcpy(errorfilename,CConfig::ErrorPath);
			strcat(errorfilename,dirp->d_name);
			rename(tempfilename,errorfilename);
		}
		else if(secgap != filesec || syshour != filehour)
		{
			/* --- 生成文件于发送目录 --- */
			stat(tempfilename,&buf );
			filebuf = (char*)malloc(buf.st_size);
			memset(filebuf,0x00,buf.st_size);
			CommonFuncs::ReadFileBuffer(tempfilename,filebuf);
			rtn = AppartFile(tempfilename,dirp->d_name,filebuf,msgtype,buf.st_size,lineid);
			free(filebuf);
			filebuf = NULL;	
			if(rtn == 0)/* --- 数据库错误 --- */
			{
				break;
			}
			else
			{
				/* --- 旧文件移到历史目录 --- */
				COtherFuncs::SaveHistoryData(1,tempfilename,dirp->d_name,mday);
			}

		}
	}
	closedir(dp);
	
	return 1;
}	
	
/* ------------------------------------------------------
      功能描述:   生成发送文件线程函数
--------------------------------------------------------- */
void* ACC::ThreadCreateSendFile(void* lpParam)
{
	pthread_detach(pthread_self());
	
	while(1)
	{
		CommonFuncs::my_sleep(5);
		CreateSendFile();
	}
	
	return NULL;
}
	
/* ------------------------------------------------------
            功能描述:   模式切换通知
--------------------------------------------------------- */
void ACC::Send_Acc_6101(WORD msgtype,char *pkgbody,int len,char *scid)
{
	char			sendbuf[1024];
	int				sendlen,rsplen=0;
	char      rspbuf[1024],oplogmsg[1024];
	MsgHead   m_MsgHead;
	DWORD 		sessionnum;
	BYTE      b_PkgID;
	char      c_PkgID[30];
	int       rtn;
	
	BYTE      b_LcId[4];
	
	memset(b_LcId,0x00,sizeof(b_LcId));	
	int lineIndex;		
	int scindex = SC::getscinfo(scid);
	if(scindex == -1)
	{
		memset(oplogmsg,0x00,sizeof(oplogmsg));
		sprintf(oplogmsg, "Send_Acc_6101 获取sc(%s)索引失败\n",scid);
		CLog::WriteAppLog(oplogmsg);	
		
		memcpy(b_LcId,CConfig::g_CCIDCode,sizeof(b_LcId));					
	}
	else
	{
		lineIndex = ACC::getlineinfo(CConfig::g_ScList[scindex].lineId);

		if(lineIndex == -1)
		{
			memset(oplogmsg,0x00,sizeof(oplogmsg));
			sprintf(oplogmsg, "Send_Acc_6101 获取Lc(%d)索引失败\n",CConfig::g_ScList[scindex].lineId);
			CLog::WriteAppLog(oplogmsg);	
			
			memcpy(b_LcId,CConfig::g_CCIDCode,sizeof(b_LcId));					
		}				
		else
		{					
			CommonFuncs::chartobyte(CConfig::ms_MlcLineList[lineIndex].LINE_NID,(char*)b_LcId,sizeof(b_LcId));
		}
	}
	 
	/* --- 包头 --- */
	sem_wait(&ORA::ms_CSSQL);
	sessionnum = ORA::get_session_number();
	b_PkgID = (BYTE)ORA::get_pkg_number();
	memset(&m_MsgHead,0x00,sizeof(MsgHead));
	memset(c_PkgID,0x00,sizeof(c_PkgID));
	CPackage::Create_Pkg_Head(b_PkgID,&m_MsgHead,len,msgtype,sessionnum,CConfig::g_ACCIDCode,c_PkgID,b_LcId);
	sem_post(&ORA::ms_CSSQL);
	/* --- 包文 --- */
	memset(sendbuf,0x00,sizeof(sendbuf));
	sendlen = CPackage::CreatePackage(0,(char*)sendbuf,&m_MsgHead,(char*)pkgbody,len);
		
	memset(oplogmsg,0x00,sizeof(oplogmsg));
	sprintf(oplogmsg, "0x6101模式切换下发ACC报文:\n");
	CLog::WriteAppLog(oplogmsg);
	CLog::WriteDebugLog(0,(unsigned char*)sendbuf,sendlen);		
		
	/* --- 发送报文 --- */
	memset(rspbuf,0x00,sizeof(rspbuf));
	rtn = CCommu::SendANDRecv(CConfig::g_Socket_ACCIP,CConfig::g_ACCPort,(char *)sendbuf,sendlen,rspbuf,rsplen,CConfig::g_Wait_Ack_Time);
	if(rtn == 1)               /* ----- 转发成功 ---*/
	{
		Ack_6301  m_Ack_6301;
		memset(&m_Ack_6301,0x00,sizeof(Ack_6301));
		m_Ack_6301 = *(Ack_6301*)(rspbuf+sizeof(MsgHead));
		memset(oplogmsg,0x00,sizeof(oplogmsg));
		sprintf(oplogmsg, "0x6101模式切换命令下发ACC(%s:%d)成功(应答码:%02X)\n",CConfig::g_Socket_ACCIP,CConfig::g_ACCPort,m_Ack_6301.b_AckCode);
		CLog::WriteAppLog(oplogmsg);
	}
	else
	{
		if(rtn == 0)
		{
			memset(oplogmsg,0x00,sizeof(oplogmsg));
			sprintf(oplogmsg, "0x6101模式切换命令下发ACC(%s:%d)失败(服务器没有启动服务)\n",CConfig::g_Socket_ACCIP,CConfig::g_ACCPort);
			CLog::WriteAppLog(oplogmsg);
		}
		else
		{
			memset(oplogmsg,0x00,sizeof(oplogmsg));
			sprintf(oplogmsg, "0x6101模式切换命令下发ACC(%s:%d)失败(服务器应答超时)\n",CConfig::g_Socket_ACCIP,CConfig::g_ACCPort);
			CLog::WriteAppLog(oplogmsg);
		}
	}
}

/* ------------------------------------------------------
            功能描述:   定时查询ACC软件版本
--------------------------------------------------------- */
void ACC::Send_Acc_6102(WORD msgtype,int lineIndex)
{
	char			sendbuf[1024];
	Req_6102	m_Req_6102;
	int				sendlen,rsplen=0;
	char      rspbuf[1024],oplogmsg[1024];
	MsgHead   m_MsgHead;
	DWORD 		sessionnum;
	BYTE      b_PkgID;

	char      filename[MAX_PATH];
	int       i,rtn;
	char      c_PkgID[30];
	
	char 			lcParaPath[MAX_PATH];
	memset(lcParaPath,0x00,sizeof(lcParaPath));
	sprintf(lcParaPath, "%slc%d/",CConfig::g_CurParamPath,CConfig::ms_MlcLineList[lineIndex].LINE_ID);

	/* --- 包头 --- */
	sem_wait(&ORA::ms_CSSQL);
	
	sessionnum = ORA::get_session_number();
	b_PkgID = (BYTE)ORA::get_pkg_number();
	memset(&m_MsgHead,0x00,sizeof(MsgHead));
	memset(c_PkgID,0x00,sizeof(c_PkgID));
	
	BYTE 			b_code[4];
	memset(b_code,0x00,sizeof(b_code));
	CommonFuncs::chartobyte(CConfig::ms_MlcLineList[lineIndex].LINE_NID,(char*)b_code,sizeof(b_code));
 
	CPackage::Create_Pkg_Head(b_PkgID,&m_MsgHead,sizeof(Req_6102),0x6102,sessionnum,CConfig::g_ACCIDCode,c_PkgID,b_code);
		
	sem_post(&ORA::ms_CSSQL);
	/* --- 包体 --- */
	memset(&m_Req_6102,0x00,sizeof(Req_6102));
	m_Req_6102.wMsgCode = CommonFuncs::wordtowin(msgtype);
	m_Req_6102.b_ParamStatus = 0x01;
	/* --- 包文 --- */
	memset(sendbuf,0x00,sizeof(sendbuf));
	sendlen = CPackage::CreatePackage(0,(char*)sendbuf,&m_MsgHead,(char*)&m_Req_6102,sizeof(Req_6102));
		
	memset(oplogmsg,0x00,sizeof(oplogmsg));
	sprintf(oplogmsg, "线路(%s)6102运营参数版本查询请求开始(%s:%d)\n",CConfig::ms_MlcLineList[lineIndex].LINE_NID,CConfig::g_Socket_ACCIP,CConfig::g_ACCPort);
	CLog::WriteAppLog(oplogmsg);
	//CLog::WriteDebugLog(0,(unsigned char*)sendbuf,sendlen);	
		
	/* --- 发送报文 --- */
	memset(rspbuf,0x00,sizeof(rspbuf));
	rtn = CCommu::SendANDRecv(CConfig::g_Socket_ACCIP,CConfig::g_ACCPort,(char *)sendbuf,sendlen,rspbuf,rsplen,CConfig::g_Wait_Ack_Time);
	if(rtn == 1)               /* ----- 转发成功 ---*/
	{	
		
		memset(oplogmsg,0x00,sizeof(oplogmsg));
		sprintf(oplogmsg, "线路(%s)6102运营参数版本查询请求(%s:%d),应答报文\n",CConfig::ms_MlcLineList[lineIndex].LINE_NID,CConfig::g_Socket_ACCIP,CConfig::g_ACCPort);
		CLog::WriteAppLog(oplogmsg);
		//CLog::WriteDebugLog(0,(unsigned char*)rspbuf,rsplen);	
			
		Ack_6302  m_Ack_6302;
		BYTE      b_ParamType;
		char      c_AccParamVer[15],c_LccParamVer[15];
		char      ftpname[256];
		WORD      wMsgCode;
		memset(&m_Ack_6302,0x00,sizeof(Ack_6302));
		m_Ack_6302 = *(Ack_6302*)(rspbuf+sizeof(MsgHead));
		memset(oplogmsg,0x00,sizeof(oplogmsg));
		sprintf(oplogmsg, "线路(%s)6102运营参数版本查询请求下发成功(%s:%d)(应答码:%02X)\n",CConfig::ms_MlcLineList[lineIndex].LINE_NID,CConfig::g_Socket_ACCIP,CConfig::g_ACCPort,m_Ack_6302.b_AckCode);
		CLog::WriteAppLog(oplogmsg);
		
		if(m_Ack_6302.b_AckCode == 0x01)
		{
			wMsgCode = CommonFuncs::wordtowin(m_Ack_6302.wMsgCode);
			if(wMsgCode != msgtype)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "线路(%s)ACC应答的参数类型与请求的不一致(请求类型:%04X,应答类型:%04X)\n",CConfig::ms_MlcLineList[lineIndex].LINE_NID,msgtype,wMsgCode);
				CLog::WriteAppLog(oplogmsg);
			}
			else
			{
				/* ----比较参数版本---*/
				memset(c_AccParamVer,0x00,sizeof(c_AccParamVer));
				for(i=0;i<sizeof(m_Ack_6302.b_CurParamVer);i++)
					sprintf(c_AccParamVer+2*i,"%02X",m_Ack_6302.b_CurParamVer[i]);
 
				memset(c_LccParamVer,0x00,sizeof(c_LccParamVer));
				rtn = CPara::GetParamVer(NULL,msgtype,lcParaPath,c_LccParamVer);
					
				if(rtn == 0)
				{
					strcpy(c_LccParamVer,"00000000000000");
				}

				if(strcmp(c_LccParamVer,c_AccParamVer) == 0)
				{
					memset(oplogmsg,0x00,sizeof(oplogmsg));
					sprintf(oplogmsg, "线路(%s)参数0x%04X当前版本与ACC一致(%s)\n",CConfig::ms_MlcLineList[lineIndex].LINE_NID,msgtype,c_LccParamVer);
					CLog::WriteAppLog(oplogmsg);
				}
				else
				{
					memset(oplogmsg,0x00,sizeof(oplogmsg));
					sprintf(oplogmsg, "线路(%s)参数0x%04X准备下载(ACC当前版本:%s,LCC当前版本:%s)\n",CConfig::ms_MlcLineList[lineIndex].LINE_NID,msgtype,c_AccParamVer,c_LccParamVer);
					CLog::WriteAppLog(oplogmsg);
					
					/* ----下载参数文件---*/
					memset(ftpname,0x00,sizeof(ftpname));
					sprintf(ftpname,"%04X_%s",msgtype,c_AccParamVer);
					rtn = GetAccParamFile(msgtype,lineIndex,ftpname);
					
					/* ----处理参数文件---*/
					if(rtn == 1) 
					{
						memset(oplogmsg,0x00,sizeof(oplogmsg));
						sprintf(oplogmsg, "线路(%s)参数0x%04X(%s)下载成功\n",CConfig::ms_MlcLineList[lineIndex].LINE_NID,msgtype,ftpname);
						CLog::WriteAppLog(oplogmsg);						
						if(msgtype == 0x0015)
						{
					 		Get_Acc_Ctl_ParamFile(lineIndex,ftpname);
						}
						else
						{
							CPara::DealParam(msgtype,ftpname,lineIndex,CConfig::g_TmpParamPath);
						}
					}
					else
					{
						memset(oplogmsg,0x00,sizeof(oplogmsg));
						sprintf(oplogmsg, "线路(%s)参数0x%04X(%s)下载失败\n",CConfig::ms_MlcLineList[lineIndex].LINE_NID,msgtype,ftpname);
						CLog::WriteAppLog(oplogmsg);
					}					 
				}
			}
		}
	}
	else
	{
		if(rtn == 0)
		{
			memset(oplogmsg,0x00,sizeof(oplogmsg));
			sprintf(oplogmsg, "线路(%s)6102运营参数版本查询请求下发失败(服务器(%s:%d)没有启动服务)\n",CConfig::ms_MlcLineList[lineIndex].LINE_NID,CConfig::g_Socket_ACCIP,CConfig::g_ACCPort);
			CLog::WriteAppLog(oplogmsg);
		}
		else
		{
			memset(oplogmsg,0x00,sizeof(oplogmsg));
			sprintf(oplogmsg, "线路(%s)6102运营参数版本查询请求下发失败(服务器(%s:%d)应答超时)\n",CConfig::ms_MlcLineList[lineIndex].LINE_NID,CConfig::g_Socket_ACCIP,CConfig::g_ACCPort);
			CLog::WriteAppLog(oplogmsg);
		}
	}
	
	memset(oplogmsg,0x00,sizeof(oplogmsg));
	sprintf(oplogmsg, "线路(%s)6102运营参数版本查询请求结束(%s:%d)\n",CConfig::ms_MlcLineList[lineIndex].LINE_NID,CConfig::g_Socket_ACCIP,CConfig::g_ACCPort);
	CLog::WriteAppLog(oplogmsg);
}	
	
/* ------------------------------------------------------
            功能描述:  ACC命令报文处理函数
--------------------------------------------------------- */
int ACC::OnACCMessageIn(para *tmp_para,int tmp_handle,char *clnip,int clnport)
{
	int             handle;
	para            *m_para;
	unsigned int    bufferlen;
	MsgHead         m_MsgHead;

	char            sendid[10],oplogmsg[200];
	int             msgtype;
	unsigned int    chatseq;
		
	/* -------- 变量赋值 ---------- */
	m_para = tmp_para;
	handle = tmp_handle;
	
	/* -------- 填充变量m_para ---------- */
	memset(&m_MsgHead,0x00,sizeof(MsgHead));
	m_MsgHead = *(MsgHead*)m_para->recv_buf;   
	bufferlen = CommonFuncs::dwordtowin(m_MsgHead.dw_PkgLen) + sizeof(MsgHead) + LEN_OF_PackageTail;
	m_para->buflen = bufferlen;
	m_para->sockhandle = handle;
	
	/* -------- 获得报文类型 ---------- */
	msgtype = CommonFuncs::wordtowin(m_MsgHead.w_MsgCode);
		
	/* -------- 获得发送方节点标识码 ---------- */
	memset(sendid,0x00,sizeof(sendid));
	for(int i = 0;i < 4;i++)
	{
		sprintf(sendid + 2*i,"%02X",m_MsgHead.b_SourceID[i]);
	}
	/* -------- 获得报文会话流水号 ---------- */
	chatseq = CommonFuncs::dwordtowin(m_MsgHead.dw_PkgID);
 
	/* -------- 写入日志 ---------- */
	memset(oplogmsg,0x00,sizeof(oplogmsg));
	sprintf(oplogmsg, "收到来自ACC(%s,%s:%d)的0x%04X报文(会话流水号:%u,报文长度:%d)\n",sendid,clnip,clnport,msgtype,chatseq,bufferlen);
	CLog::WriteAppLog(oplogmsg);
	
	/* --- 记入DEBUG日志 --- */
	if(DEBUG_FLAG)
	{
		memset(oplogmsg,0x00,sizeof(oplogmsg));
		sprintf(oplogmsg,"ACC(%s)的0x%04X报文内容:\n",clnip,msgtype);
		CLog::WriteAppLog(oplogmsg);
		CLog::WriteDebugLog(0,(unsigned char*)m_para->recv_buf,m_para->buflen);
	}
	
	/* -------- 生成处理线程 ---------- */
	pthread_t hThreadHandle;
	pthread_create(&hThreadHandle, NULL, ThreadDealACCCmd, (void*)m_para);

	return 0;
}


/* ------------------------------------------------------
   功能描述: 删除主控文件及下辖文件
--------------------------------------------------------- */
void ACC::del_Acc_0015(int flag,char *filebuf,int count,char *name,char *fileExistPath)
{
	char 						filename[MAX_PATH];
	char 						logdata[1024],b_ParamVer[15];
	unsigned short 	w_ParamType;
	Para_0015_2 		m_Para_0015_2;
	int             i;
	
	if(flag == 0)
	{
		memset(logdata,0x00,sizeof(logdata));
		strcpy(logdata,"ACC主控参数下辖参数接收异常\n");
		CLog::WriteAppLog(logdata);
	}
	if(flag == 1)
	{
		memset(logdata,0x00,sizeof(logdata));
		strcpy(logdata,"ACC主控参数入库失败\n");
		CLog::WriteAppLog(logdata);
	}
	if(flag == 2)
	{
		memset(logdata,0x00,sizeof(logdata));
		strcpy(logdata,"ACC主控下辖参数入库失败\n");
		CLog::WriteAppLog(logdata);
	}
	/* --- 删除主控文件 ------- */
	memset(filename,0x00,sizeof(filename));
	sprintf(filename,"%s%s",fileExistPath,name);
	remove(filename);
	/* --- 删除下辖文件 ------- */
	for(i = 0;i<count;i++)
	{
		memset(&m_Para_0015_2,0x00,sizeof(Para_0015_2));
		m_Para_0015_2 = *(Para_0015_2 *)(filebuf + sizeof(MsgHead) + sizeof(Para_0015_1) + i * sizeof(Para_0015_2));
		w_ParamType = CommonFuncs::wordtowin(m_Para_0015_2.w_ParamType);
			
		memset(b_ParamVer,0x00,sizeof(b_ParamVer));
		for(int index = 0;index < sizeof(m_Para_0015_2.b_ParamVer);index++)
			sprintf(b_ParamVer+2*index,"%02X",m_Para_0015_2.b_ParamVer[index]);

		memset(filename,0x00,sizeof(filename));
		sprintf(filename,"%04X_%s",w_ParamType,b_ParamVer);	
		remove(filename);
	}
}


/* ------------------------------------------------------
            功能描述:  清分主控参数文件
--------------------------------------------------------- */
int ACC::Get_Acc_Ctl_ParamFile(int lineIndex,char *name)
{
	Para_0015_1			m_Para_0015_1;
	Para_0015_2			m_Para_0015_2;
	struct stat			buf;
	int							filelen,pkglen;
	char						ftpname[256],filename[MAX_PATH],logdata[1024];
	char						*filebuf;
	unsigned int  	dw_ParamRecords;
	unsigned short	w_ParamType;			
	char						b_ParamVer[15];			
	int			    		i,index,rtn,step;
	int      				ret,rtn_insert_db;

	char 			lcParaPath[MAX_PATH];
	memset(lcParaPath,0x00,sizeof(lcParaPath));
	sprintf(lcParaPath, "%slc%d/",CConfig::g_CurParamPath,CConfig::ms_MlcLineList[lineIndex].LINE_ID);
		
	ret = 0;
 
	/*--- 读取文件内容 ------- */
	memset(filename,0x00,sizeof(filename));
	sprintf(filename,"%s%s",CConfig::g_TmpParamPath,name);
	stat(filename,&buf);
	filelen = buf.st_size;
	filebuf = (char*)malloc(filelen);
	memset(filebuf,0x00,filelen);
	CommonFuncs::ReadFileBuffer(filename,filebuf);
		
	/* ----判断参数版本控制信息文件格式---*/
	memset(&m_Para_0015_1,0x00,sizeof(Para_0015_1));
	m_Para_0015_1 = *(Para_0015_1 *)(filebuf + sizeof(MsgHead));
	dw_ParamRecords = CommonFuncs::dwordtowin(m_Para_0015_1.dw_ParamRecords);
	pkglen = sizeof(MsgHead) + sizeof(Para_0015_1) + dw_ParamRecords * sizeof(Para_0015_2) + LEN_OF_PackageTail;
	if(filelen != pkglen)
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"参数版本控制信息文件(%s)格式错误:文件长度:%d,记录数:%d\n",filename,filelen,dw_ParamRecords);
		CLog::WriteAppLog(logdata);
		free(filebuf);
		filebuf = NULL;
		return 0;
	}
	/* --- 读取参数内容 ------- */
	step = 0;
	for(i = 0;i<dw_ParamRecords;i++)
	{
		rtn = 0;
		memset(&m_Para_0015_2,0x00,sizeof(Para_0015_2));
		m_Para_0015_2 = *(Para_0015_2 *)(filebuf + sizeof(MsgHead) + sizeof(Para_0015_1) + i * sizeof(Para_0015_2));
		w_ParamType = CommonFuncs::wordtowin(m_Para_0015_2.w_ParamType);
			
		memset(b_ParamVer,0x00,sizeof(b_ParamVer));
		for(index = 0;index < sizeof(m_Para_0015_2.b_ParamVer);index++)
			sprintf(b_ParamVer+2*index,"%02X",m_Para_0015_2.b_ParamVer[index]);
			
		switch(w_ParamType)
		{
			case 0x0001:/* ----线路参数---*/
			case 0x0002:/* ----车站参数---*/
			case 0x0004:/* ----换乘关系参数---*/
			case 0x0005:/* ----票型参数---*/
			case 0x0006:/* ----车站区间参数---*/
			case 0x0007:/* ----票价区间参数---*/
			case 0x0008:/* ----区段计次票区间票价参数---*/
			case 0x0009:/* ----区段定期票发售金额参数---*/
			case 0x000A:/* ----纪念票号段发售金额参数---*/
			case 0x0011:/* ----节假日参数---*/
			case 0x0012:/* ----高峰时段参数---*/
			case 0x0016:/* ----读写器参数---*/
			case 0x0013:/* ----TVM主界面版本参数---*/
			case 0x0017:/* ----读写器参数文件---*/
				memset(ftpname,0x00,sizeof(ftpname));
				sprintf(ftpname,"%04X_%s",w_ParamType,b_ParamVer);
				rtn = GetAccParamFile(w_ParamType,lineIndex,ftpname);
				if(rtn == 1)
				{
					rtn = CPara::Formate_ParamFile(CConfig::g_TmpParamPath,ftpname);
					if(rtn == 1)
					{ 
						step = step + 1;
					}
				}
				break;	
			default:
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"系统未定义的参数类型:%04X\n",w_ParamType);
				CLog::WriteAppLog(logdata);
				break;
			}
	}

	if(step == dw_ParamRecords)
	{
		rtn_insert_db = 1;
		/* ----ACC主控参数及其下辖参数生效---*/
		memset(logdata,0x00,sizeof(logdata));
		strcpy(logdata,"ACC主控参数下辖参数接收完成\n");
		CLog::WriteAppLog(logdata);

		/* --- 入库 ---*/
		memset(filename,0x00,sizeof(filename));
		sprintf(filename,"%s%s",CConfig::g_TmpParamPath,name);
		
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"insert_into_0015--%s---\n",filename);
		CLog::WriteAppLog(logdata);
			
		sem_wait(&ORA::ms_CSSQL);
		rtn = ORA::insert_into_0015(filename,lineIndex);
		sem_post(&ORA::ms_CSSQL);
		
		if(rtn == 0)/* --- 主控参数入库失败 ---*/
		{
			ret = 0;
			/* --- 删除主控文件及下辖文件 ------- */
			del_Acc_0015(1,filebuf,dw_ParamRecords,name,CConfig::g_TmpParamPath);
		}
		else/* --- 主控参数入库成功 ---*/
		{
			
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"主控参数入库成功,处理子参数,记录数(%d),处理开始\n",dw_ParamRecords);
			CLog::WriteAppLog(logdata);
					
			for(i = 0;i<dw_ParamRecords;i++)
			{
				memset(&m_Para_0015_2,0x00,sizeof(Para_0015_2));
				m_Para_0015_2 = *(Para_0015_2 *)(filebuf + sizeof(MsgHead) + sizeof(Para_0015_1) + i * sizeof(Para_0015_2));
				w_ParamType = CommonFuncs::wordtowin(m_Para_0015_2.w_ParamType);
					
				memset(b_ParamVer,0x00,sizeof(b_ParamVer));
				for(index = 0;index < sizeof(m_Para_0015_2.b_ParamVer);index++)
					sprintf(b_ParamVer+2*index,"%02X",m_Para_0015_2.b_ParamVer[index]);
				
				/* --- 入库 ---*/			
				memset(filename,0x00,sizeof(filename));
				sprintf(filename,"%s%04X_%s",CConfig::g_TmpParamPath,w_ParamType,b_ParamVer);
 
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"主控参数入库成功,处理子参数(%s)处理开始--------\n",filename);
				CLog::WriteAppLog(logdata);
 
				switch(w_ParamType)
				{
					case 0x0001:/* ----线路参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0001(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x0002:/* ----车站参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0002(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x0004:/* ----换乘关系参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0004(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x0005:/* ----票型参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0005(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x0006:/* ----车站区间参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0006(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x0007:/* ----票价区间参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0007(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x0008:/* ----区段计次票区间票价参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0008(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x0009:/* ----区段定期票发售金额参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0009(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x000A:/* ----纪念票号段发售金额参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_000A(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
						break;
					case 0x0011:/* ----节假日参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0011(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x0012:/* ----高峰时段参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0012(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x0016:/* ----读写器参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0016(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x0013:/* ----TVM主界面版本参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0013(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x0017:/* ----读写器参数文件---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0017(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
				}
				
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"主控参数入库成功,处理子参数(%s)处理结束--------\n",filename);
				CLog::WriteAppLog(logdata);
			}
			
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"主控参数入库成功,处理子参数,记录数(%d),处理结束\n",dw_ParamRecords);
			CLog::WriteAppLog(logdata);
			/* --- 入库后处理 ---*/
			if(rtn_insert_db == 1)/* --- 入库成功 ---*/
			{
				memset(logdata,0x00,sizeof(logdata));
				strcpy(logdata,"ACC主控参数及其下辖参数入库完成\n");
				CLog::WriteAppLog(logdata);
					
				/* --- 更改参数状态 ------- 
				sem_wait(&g_CSSQL);
				Update_Param_Status(0x0015);
				sem_post(&g_CSSQL);*/
							
				ret = 1;
				
				/* --- 删除当前参数目录文件 ------- */	
				CommonFuncs::DeleteFile(lcParaPath,(char*)"0015_");			
				
				/* --- 移动主控文件 ------- */
				CPara::Rename_ParamFile(1,CConfig::g_TmpParamPath,name,lcParaPath);

				/*------------日志打印--------------
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"CConfig::g_TmpParamPath:%s-name:%s-lcParaPath:%s-\n",CConfig::g_TmpParamPath,name,lcParaPath);
				CLog::WriteAppLog(logdata);*/
				
				/* --- 移动下辖文件 ------- */
				for(i = 0;i<dw_ParamRecords;i++)
				{
					memset(&m_Para_0015_2,0x00,sizeof(Para_0015_2));
					m_Para_0015_2 = *(Para_0015_2 *)(filebuf + sizeof(MsgHead) + sizeof(Para_0015_1) + i * sizeof(Para_0015_2));
					w_ParamType = CommonFuncs::wordtowin(m_Para_0015_2.w_ParamType);
						
					/* --- 更改参数状态 ------- 
					sem_wait(&g_CSSQL);
					Update_Param_Status(w_ParamType);
					sem_post(&g_CSSQL);*/	
					
					/* --- 删除当前参数目录文件 ------- */	
					char strTemp[100];
					memset(strTemp,0x00,sizeof(strTemp));
					sprintf(strTemp,"%04X_",w_ParamType);
					CommonFuncs::DeleteFile(lcParaPath,strTemp);
	 
					memset(b_ParamVer,0x00,sizeof(b_ParamVer));
					for(index = 0;index < sizeof(m_Para_0015_2.b_ParamVer);index++)
						sprintf(b_ParamVer+2*index,"%02X",m_Para_0015_2.b_ParamVer[index]);
	
					memset(ftpname,0x00,sizeof(ftpname));
					sprintf(ftpname,"%04X_%s",w_ParamType,b_ParamVer);
					CPara::Rename_ParamFile(0,CConfig::g_TmpParamPath,ftpname,lcParaPath);

					memset(logdata,0x00,sizeof(logdata));
					sprintf(logdata,"CConfig::g_TmpParamPath:%s-ftpname:%s-lcParaPath:%s-\n",CConfig::g_TmpParamPath,ftpname,lcParaPath);
					CLog::WriteAppLog(logdata);

				}
			}
			else  /* --- 入库失败 ---*/
			{
				/* --- 删除主控参数及其下辖参数已入成功的数据 ------- */
				sem_wait(&ORA::ms_CSSQL);
				ORA::Delete_Param_Status(lineIndex);
				sem_post(&ORA::ms_CSSQL);
				
				memset(logdata,0x00,sizeof(logdata));
				strcpy(logdata,"ACC主控参数及其下辖参数入库失败\n");
				CLog::WriteAppLog(logdata);
				ret = 0;
				/* --- 删除主控文件及下辖文件 ------- */
				del_Acc_0015(2,filebuf,dw_ParamRecords,name,CConfig::g_TmpParamPath);
			}
		}
	}
	else
	{
		ret = 0;
		/* --- 删除主控文件及下辖文件 ------- */
		del_Acc_0015(0,filebuf,dw_ParamRecords,name,CConfig::g_TmpParamPath);
	}

	free(filebuf);
	filebuf = NULL;

	return ret;
}

/* ------------------------------------------------------
            功能描述:  获取参数文件
            返回值：0 失败;  成功;
--------------------------------------------------------- */

int ACC::GetAccParamFile(unsigned short msgtype,int lineIndex,char *filename)
{
	int		rtn;
	char	logdata[1024];
	char  name[256];
	
	FtpMsg m_FtpMsg;
	memset(&m_FtpMsg,0x00,sizeof(FtpMsg));
	rtn = GetFtpMsg(msgtype,lineIndex,&m_FtpMsg);
	if(rtn == -1)
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"GetAccParamFile(msgtype:0x%04X-lineIndex:%d) ERROR\n",msgtype,lineIndex);
		CLog::WriteAppLog(logdata);		
			
		return 0;
	}
	
 	/* ---------- 相关FTP信息 -------*/
	memset(logdata,0x00,sizeof(logdata));
	sprintf(logdata,"GetAccParamFile 相关FTP信息(ftpIP:%s-ftpPort:%d-ftpUser:%s-ftpPass:%s-ftpPath:%s)\n",m_FtpMsg.ftpIP,m_FtpMsg.ftpPort,m_FtpMsg.ftpUser,m_FtpMsg.ftpPass,m_FtpMsg.ftpPath);
	CLog::WriteAppLog(logdata);
 
	/* ---------- 从服务器get文件 -------*/
	rtn = CSocket::Ftp_GetFile(m_FtpMsg.ftpIP,m_FtpMsg.ftpPort,m_FtpMsg.ftpUser,m_FtpMsg.ftpPass,CConfig::g_TmpParamPath,filename,m_FtpMsg.ftpPath);
	if(rtn == 1)
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"FTP获取ACC文件(msgtype:0x%04X-lineIndex:%d)成功(%s)\n",msgtype,lineIndex,filename);
		CLog::WriteAppLog(logdata);
	}
	else
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"FTP获取ACC文件(msgtype:0x%04X-lineIndex:%d)失败(%s),失败代码:%d)\n",msgtype,lineIndex,filename,rtn);
		CLog::WriteAppLog(logdata);

		memset(name,0x00,sizeof(name));
		sprintf(name,"%s%s",CConfig::g_TmpParamPath,filename);
		remove(name);
		
		rtn = 0;
	}

	return rtn;
}

/* ------------------------------------------------------
            功能描述:  控制命令报文处理线程函数
--------------------------------------------------------- */
void* ACC::ThreadDealACCCmd(void* inpara)
{
	pthread_detach(pthread_self());
	
	int 						i,rtn;
	unsigned short  msgtype;
	char						logdata[1024];
	char						sendid[9],recvid[9];
	MsgHead					m_req_msghead;
	char						filename[MAX_PATH];
	char            ftpname[256];
	char            temp[10];

	ACK_6201        m_ACK_6201;
	
	para *m_para=NULL;
	m_para=(para *)inpara;
	
	/* ---------------- 获得报文类型 --------------*/
	memset(&m_req_msghead,0x00,sizeof(MsgHead));
	m_req_msghead = *(MsgHead *)(m_para->recv_buf);
	msgtype = CommonFuncs::wordtowin(m_req_msghead.w_MsgCode);
	/* ----------- 获得发送方节点标识码 ------*/
	memset(sendid,0x00,sizeof(sendid));
	for(i=0;i<sizeof(m_req_msghead.b_SourceID);i++)
	{
		sprintf(sendid+2*i,"%02X",m_req_msghead.b_SourceID[i]);
	}
	
	/* ----------- 获得接收方节点标识码 ------*/
	memset(recvid,0x00,sizeof(recvid));
	for(i=0;i<sizeof(m_req_msghead.b_DestID);i++)
	{
		sprintf(recvid+2*i,"%02X",m_req_msghead.b_DestID[i]);
	}

	/* -------- 获得报文会话流水号 ---------- */
	int chatseq = CommonFuncs::dwordtowin(m_req_msghead.dw_PkgID);
 
	/* ----------- 获得线路下标索引标识码 ------*/		
	int lineIndex = getlineinfo(recvid);
	if(lineIndex == -1)
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"接收到清分中心(%s)发送的接收方节点为(%s),获取线路下标索引出错,报文不予处理！\n",sendid,recvid);
		CLog::WriteAppLog(logdata);
		
		return NULL;
	}

	/* ----------- 处理控制指令报文 ------*/
	switch(msgtype)
	{
	case 0x6001://通用命令	
		
		CMD_6001 m_CMD_6001;
		int      b_CMDType;
		memset(&m_CMD_6001,0x00,sizeof(CMD_6001));
		m_CMD_6001 = *(CMD_6001 *)(m_para->recv_buf+sizeof(MsgHead));
		b_CMDType = m_CMD_6001.b_CMDType;
		/* ----------- 时钟同步 ------*/
		if(b_CMDType == 1)
		{
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"接收到清分中心(%s)%04X报文,指令类型:时钟同步\n",sendid,msgtype);
			CLog::WriteAppLog(logdata);
			/* ---时钟同步 ---*/
			CSynTime::Adjust_System_Time();
		}
		/* ----------- 查询参数版本 ------*/
		if(b_CMDType == 2)
		{
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"接收到清分中心(%s)%04X报文,指令类型:查询参数版本\n",sendid,msgtype);
			CLog::WriteAppLog(logdata);
		}
		/* ----------- ping ------*/
		if(b_CMDType == 3)
		{
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"接收到清分中心(%s)%04X报文,指令类型:ping\n",sendid,msgtype);
			CLog::WriteAppLog(logdata);
		}
		/* ----生成应答报文体---*/
		memset(&m_ACK_6201,0x00,sizeof(ACK_6201));
		memcpy(m_ACK_6201.b_CMDID,m_CMD_6001.b_CMDID,sizeof(m_CMD_6001.b_CMDID));
		memcpy(m_ACK_6201.b_SourceID,m_CMD_6001.b_SourceID,sizeof(m_CMD_6001.b_SourceID));
		memcpy(m_ACK_6201.b_DestID,m_CMD_6001.b_DestID,sizeof(m_CMD_6001.b_DestID));
		m_ACK_6201.b_ACK = (BYTE)0x01;
		/* ----发送应答---*/
		CCommu::SendtoACK(m_para->sockhandle,0x6201,&m_req_msghead,(char*)&m_ACK_6201,sizeof(ACK_6201));

		break;

	case 0x6002://模式切换命令		
		
		CMD_6002 m_CMD_6002;
		BYTE     b_CmdType;
		memset(&m_CMD_6002,0x00,sizeof(CMD_6002));
		m_CMD_6002 = *(CMD_6002 *)(m_para->recv_buf+sizeof(MsgHead));
		/* ----生成应答报文体---*/
		memset(&m_ACK_6201,0x00,sizeof(ACK_6201));
		memcpy(m_ACK_6201.b_CMDID,m_CMD_6002.b_CMDID,sizeof(m_CMD_6002.b_CMDID));
		memcpy(m_ACK_6201.b_SourceID,m_CMD_6002.b_SourceID,sizeof(m_CMD_6002.b_SourceID));
		memcpy(m_ACK_6201.b_DestID,m_CMD_6002.b_DestID,sizeof(m_CMD_6002.b_DestID));
		m_ACK_6201.b_ACK = (BYTE)0x01;
		/* ----发送应答---*/
		CCommu::SendtoACK(m_para->sockhandle,0x6201,&m_req_msghead,(char*)&m_ACK_6201,sizeof(ACK_6201));

		b_CmdType = m_CMD_6002.b_CmdType;
		/* ----模式命令/广播---*/
		if(b_CmdType == 1)/* ----模式命令---*/
		{
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"接收到清分中心(%s)%04X报文(模式命令)\n",sendid,msgtype);
			CLog::WriteAppLog(logdata);
			
			char     scid[9];
			char     sendbuf[100];
			char 		 rspbuf[100];
			int      sendlen,rsplen;
			BYTE     to_NID[4];
			char     c_PkgID[30];
			MsgHead  m_MsgHead;

			memset(scid,0x00,sizeof(scid));
			sprintf(scid,"%02X%02X6101",m_CMD_6002.b_SCID[0],m_CMD_6002.b_SCID[1]);
 
 
 			BYTE 		sccode[4];
			/*-----更改接收方节点--------*/
			memset(sccode,0x00,sizeof(sccode));
			memset(&m_MsgHead,0x00,sizeof(MsgHead));
			m_MsgHead = *(MsgHead *)(m_para->recv_buf);
			CommonFuncs::chartobyte(scid,(char*)sccode,sizeof(sccode));					
			memcpy(m_MsgHead.b_DestID,sccode,4);
			memcpy(m_MsgHead.b_SourceID,CConfig::g_CCIDCode,4);
			memcpy(m_para->recv_buf,&m_MsgHead,sizeof(MsgHead));		

			unsigned int ul_crc;
			/*--- 生成CRC32校验 --- */
			Crc::Init_CRC32_Table();
			ul_crc = Crc::Crc32((char*)m_para->recv_buf,m_para->buflen - LEN_OF_PackageTail,0);
			ul_crc = CommonFuncs::dwordtowin(ul_crc);
			memcpy(m_para->recv_buf + m_para->buflen - LEN_OF_PackageTail,&ul_crc,LEN_OF_PackageTail);
				
			/*---发送报文内容-------------*/ 
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata, "更改接收方节点报文内容:\n");
			CLog::WriteAppLog(logdata);
			CLog::WriteDebugLog(0,(unsigned char*)m_para->recv_buf,m_para->buflen );		
				
			for(i=0;i<MAX_SLE_COUNT;i++)
			{
				if(strncmp(CConfig::g_ScList[i].sccode,scid,4) == 0 && CConfig::g_ScList[i].downloadflag == 1 && CConfig::g_ScList[i].scport > 0)
				{
					/* --- 发送 --- */
					rsplen = 0;
					memset(rspbuf,0x00,sizeof(rspbuf));
					rtn = CCommu::SendANDRecv(CConfig::g_ScList[i].scip,CConfig::g_ScList[i].scport,(char*)m_para->recv_buf,m_para->buflen,rspbuf,rsplen,CConfig::g_Wait_Ack_Time);
					if(rtn == 1)
					{
						memset(logdata,0x00,sizeof(logdata));
						sprintf(logdata,"清分中心(%s)%04X报文(模式命令)转发车站(%s:%s)成功\n",sendid,msgtype,CConfig::g_ScList[i].sccode,CConfig::g_ScList[i].scip);
						CLog::WriteAppLog(logdata);				
					}
					else
					{
						memset(logdata,0x00,sizeof(logdata));
						sprintf(logdata,"清分中心(%s)%04X报文(模式命令)转发车站(%s:%s)失败\n",sendid,msgtype,CConfig::g_ScList[i].sccode,CConfig::g_ScList[i].scip);
						CLog::WriteAppLog(logdata);
					}
					break;
				}
			}
		}
		else/* ----模式广播---*/
		{
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"接收到清分中心(%s)%04X报文(模式广播)\n",sendid,msgtype);
			CLog::WriteAppLog(logdata);
			/* ----转发给SC ---*/
			SC::getdatasle(0x6002,0,NULL,(unsigned char*)m_para->recv_buf,m_para->buflen);
		}

		break;

	case 0x6003://上传指定数据包		
		
		CMD_6003 m_CMD_6003;
		memset(&m_CMD_6003,0x00,sizeof(CMD_6003));
		m_CMD_6003 = *(CMD_6003 *)(m_para->recv_buf+sizeof(MsgHead));
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"接收到清分中心(%s)%04X报文\n",sendid,msgtype);
		CLog::WriteAppLog(logdata);
		/* ----生成应答报文体---*/
		memset(&m_ACK_6201,0x00,sizeof(ACK_6201));
		memcpy(m_ACK_6201.b_CMDID,m_CMD_6003.b_CMDID,sizeof(m_CMD_6003.b_CMDID));
		memcpy(m_ACK_6201.b_SourceID,m_CMD_6003.b_SourceID,sizeof(m_CMD_6003.b_SourceID));
		memcpy(m_ACK_6201.b_DestID,m_CMD_6003.b_DestID,sizeof(m_CMD_6003.b_DestID));
		m_ACK_6201.b_ACK = (BYTE)0x01;
		/* ----发送应答---*/
		CCommu::SendtoACK(m_para->sockhandle,0x6201,&m_req_msghead,(char*)&m_ACK_6201,sizeof(ACK_6201));
		/* ----转发给SC---*/
		SC::getdatasle(0x6003,0,NULL,(unsigned char*)m_para->recv_buf,m_para->buflen);

		break;

	case 0x6004://上传指定时间数据包		
		
		CMD_6004 m_CMD_6004;
		memset(&m_CMD_6004,0x00,sizeof(CMD_6004));
		m_CMD_6004 = *(CMD_6004 *)(m_para->recv_buf+sizeof(MsgHead));
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"接收到清分中心(%s)%04X报文\n",sendid,msgtype);
		CLog::WriteAppLog(logdata);
		/* ----生成应答报文体---*/
		memset(&m_ACK_6201,0x00,sizeof(ACK_6201));
		memcpy(m_ACK_6201.b_CMDID,m_CMD_6004.b_CMDID,sizeof(m_CMD_6004.b_CMDID));
		memcpy(m_ACK_6201.b_SourceID,m_CMD_6004.b_SourceID,sizeof(m_CMD_6004.b_SourceID));
		memcpy(m_ACK_6201.b_DestID,m_CMD_6004.b_DestID,sizeof(m_CMD_6004.b_DestID));
		m_ACK_6201.b_ACK = (BYTE)0x01;
		/* ----发送应答---*/
		CCommu::SendtoACK(m_para->sockhandle,0x6201,&m_req_msghead,(char*)&m_ACK_6201,sizeof(ACK_6201));
		/* ----转发给SC---*/
		SC::getdatasle(0x6004,0,NULL,(unsigned char*)m_para->recv_buf,m_para->buflen);

		break;

	case 0x6005://参数下载命令	HHJT_CJP 20210321 更改	

		CMD_6005 m_CMD_6005;
		int      b_ParamType;
		char     c_AccParamVer[15],c_LccParamVer[15];
		WORD     wMsgCode;
		int      rtn_insert_db;
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"接收到清分中心(%s)%04X报文\n",sendid,msgtype);
		CLog::WriteAppLog(logdata);
		
		/* --- FTP通知报文互斥 --- */
		sem_wait(&CPara::g_CSFTP);
		if(CPara::g_Ftp_Inform == 1)
		{
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"前次清分中心%04X报文(参数下载)还未处理结束,不予处理\n",msgtype);
			CLog::WriteAppLog(logdata);
			sem_post(&CPara::g_CSFTP);
			break;
		}
		CPara::g_Ftp_Inform = 1;
		sem_post(&CPara::g_CSFTP);
				
		/* ----报文解析---*/
		memset(&m_CMD_6005,0x00,sizeof(CMD_6005));
		m_CMD_6005 = *(CMD_6005 *)(m_para->recv_buf+sizeof(MsgHead));
		b_ParamType = m_CMD_6005.b_ParamType;
		memset(c_AccParamVer,0x00,sizeof(c_AccParamVer));
		for(i=0;i<sizeof(m_CMD_6005.b_ParamVer);i++)
			sprintf(c_AccParamVer+2*i,"%02X",m_CMD_6005.b_ParamVer[i]);

		/* ----指令类型获取参数类型---*/			
		switch(b_ParamType)
		{
			case 0x01: wMsgCode = 0x0015; break;
			case 0x02: wMsgCode = 0x0019; break;				
			case 0x03: wMsgCode = 0x001A; break;				
			case 0x04: wMsgCode = 0x0018; break;
			case 0x05: wMsgCode = 0x000E; break;
			case 0x06: wMsgCode = 0x000B; break;
			case 0x0C: wMsgCode = 0x001B; break;				
			case 0x0D: wMsgCode = 0x000C; break;				
			case 0x0E: wMsgCode = 0x000D; break;
			case 0x0F: wMsgCode = 0x000F; break;
			case 0x11: wMsgCode = 0x0401; break;
			case 0x12: wMsgCode = 0x0402; break;				
			case 0x13: wMsgCode = 0x0403; break;				
			case 0x10: wMsgCode = 0x0406; break;
			default: 
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata, "参数下载命令出现未知指令类型:0x02X%\n",b_ParamType);
				CLog::WriteAppLog(logdata); 			
				return NULL;
				break;
		}	
 
		char 			lcParaPath[MAX_PATH];
		memset(lcParaPath,0x00,sizeof(lcParaPath));
		sprintf(lcParaPath, "%slc%d/",CConfig::g_CurParamPath,CConfig::ms_MlcLineList[lineIndex].LINE_ID);
		
		/* ----0019/0x001A无参数版本直接下载---*/
		if(wMsgCode != 0x0019 && wMsgCode != 0x001A)
		{
			memset(c_LccParamVer,0x00,sizeof(c_LccParamVer));
			rtn = CPara::GetParamVer(NULL,wMsgCode,lcParaPath,c_LccParamVer);
			if(rtn == 0)
			{
				strcpy(c_LccParamVer,"00000000000000");
			}

			if(strcmp(c_LccParamVer,c_AccParamVer) == 0)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata, "参数%04X当前版本与ACC一致(%s)\n",wMsgCode,c_LccParamVer);
				CLog::WriteAppLog(logdata);
				
				/* --- 释放FTP通知报文互斥 --- */
				sem_wait(&CPara::g_CSFTP);
				CPara::g_Ftp_Inform = 0;
				sem_post(&CPara::g_CSFTP);
				
				/* ----生成应答报文体---*/
				memset(&m_ACK_6201,0x00,sizeof(ACK_6201));
				memcpy(m_ACK_6201.b_CMDID,m_CMD_6005.b_CMDID,sizeof(m_CMD_6005.b_CMDID));
				memcpy(m_ACK_6201.b_SourceID,m_CMD_6005.b_SourceID,sizeof(m_CMD_6005.b_SourceID));
				memcpy(m_ACK_6201.b_DestID,m_CMD_6005.b_DestID,sizeof(m_CMD_6005.b_DestID));
				m_ACK_6201.b_ACK = (BYTE)0x01;
				/* ----发送应答---*/
				CCommu::SendtoACK(m_para->sockhandle,0x6201,&m_req_msghead,(char*)&m_ACK_6201,sizeof(ACK_6201));
				
				break;
			}
		}
		
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata, "参数%04X准备下载(ACC当前版本:%s,LCC(%s)当前版本:%s)\n",wMsgCode,c_AccParamVer,recvid,c_LccParamVer);
		CLog::WriteAppLog(logdata);
		
		/* ----下载参数文件---*/
		memset(ftpname,0x00,sizeof(ftpname));
		sprintf(ftpname,"%04X_%s",wMsgCode,c_AccParamVer);
		rtn = GetAccParamFile(wMsgCode,lineIndex,ftpname);
		
		/* ----处理参数文件---*/
		if(rtn == 1) 
		{
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata, "线路(%s)参数0x%04X(%s)下载成功\n",CConfig::ms_MlcLineList[lineIndex].LINE_NID,wMsgCode,ftpname);
			CLog::WriteAppLog(logdata);						
			if(wMsgCode == 0x0015)
			{
		 		ACC::Get_Acc_Ctl_ParamFile(lineIndex,ftpname);
			}
			else
			{
				CPara::DealParam(wMsgCode,ftpname,lineIndex,CConfig::g_TmpParamPath);
			}
		}
		else
		{
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata, "线路(%s)参数0x%04X(%s)下载失败\n",CConfig::ms_MlcLineList[lineIndex].LINE_NID,wMsgCode,ftpname);
			CLog::WriteAppLog(logdata);
		}					 

		/* ----生成应答报文体---*/
		memset(&m_ACK_6201,0x00,sizeof(ACK_6201));
		memcpy(m_ACK_6201.b_CMDID,m_CMD_6005.b_CMDID,sizeof(m_CMD_6005.b_CMDID));
		memcpy(m_ACK_6201.b_SourceID,m_CMD_6005.b_SourceID,sizeof(m_CMD_6005.b_SourceID));
		memcpy(m_ACK_6201.b_DestID,m_CMD_6005.b_DestID,sizeof(m_CMD_6005.b_DestID));
		m_ACK_6201.b_ACK = (BYTE)0x01;
		/* ----发送应答---*/
		CCommu::SendtoACK(m_para->sockhandle,0x6201,&m_req_msghead,(char*)&m_ACK_6201,sizeof(ACK_6201));

		/* --- 释放FTP通知报文互斥 --- */
		sem_wait(&CPara::g_CSFTP);
		CPara::g_Ftp_Inform = 0;
		sem_post(&CPara::g_CSFTP);
		
		break;

	case 0x6006://车票回收命令		
		
		CMD_6006 m_CMD_6006;
		memset(&m_CMD_6006,0x00,sizeof(CMD_6006));
		m_CMD_6006 = *(CMD_6006 *)(m_para->recv_buf+sizeof(MsgHead));
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"接收到清分中心(%s)%04X报文\n",sendid,msgtype);
		CLog::WriteAppLog(logdata);
		/* ----生成应答报文体---*/
		memset(&m_ACK_6201,0x00,sizeof(ACK_6201));
		memcpy(m_ACK_6201.b_CMDID,m_CMD_6006.b_CMDID,sizeof(m_CMD_6006.b_CMDID));
		memcpy(m_ACK_6201.b_SourceID,m_CMD_6006.b_SourceID,sizeof(m_CMD_6006.b_SourceID));
		memcpy(m_ACK_6201.b_DestID,m_CMD_6006.b_DestID,sizeof(m_CMD_6006.b_DestID));
		m_ACK_6201.b_ACK = (BYTE)0x01;
		/* ----发送应答---*/
		CCommu::SendtoACK(m_para->sockhandle,0x6201,&m_req_msghead,(char*)&m_ACK_6201,sizeof(ACK_6201));
		/* ----转发给SC---*/
		SC::getdatasle(0x6006,0,NULL,(unsigned char*)m_para->recv_buf,m_para->buflen);

		break;

	case 0x6007://下发对账数据	HHJT_CJP 20210321 更改		
		
		CMD_6007 m_CMD_6007;
		int      b_FileType;
		char     b_SettleDate[15];

		/* ----报文解析---*/
		memset(&m_CMD_6007,0x00,sizeof(CMD_6007));
		m_CMD_6007 = *(CMD_6007 *)(m_para->recv_buf+sizeof(MsgHead));
		b_FileType = m_CMD_6007.b_FileType;
		memset(b_SettleDate,0x00,sizeof(b_SettleDate));
		for(i=0;i<sizeof(m_CMD_6007.b_SettleDate);i++)
			sprintf(b_SettleDate+2*i,"%02X",m_CMD_6007.b_SettleDate[i]);

		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"接收到清分中心(%s)%04X报文,文件类型:0x%02X\n",sendid,msgtype,b_FileType);
		CLog::WriteAppLog(logdata);
						
		if(b_FileType == 0x01)/* ----一票通对账数据---*/
		{
			DealAccStatFile(CConfig::g_TmpParamPath,0x2002,lineIndex,b_SettleDate);
			DealAccStatFile(CConfig::g_TmpParamPath,0x2003,lineIndex,b_SettleDate);			 
			DealAccStatFile(CConfig::g_TmpParamPath,0x2005,lineIndex,b_SettleDate);
			DealAccStatFile(CConfig::g_TmpParamPath,0x2006,lineIndex,b_SettleDate);
			DealAccStatFile(CConfig::g_TmpParamPath,0x2007,lineIndex,b_SettleDate);	 
			DealAccStatFile(CConfig::g_TmpParamPath,0x3009,lineIndex,b_SettleDate);				
		}
		if(b_FileType == 0x02)/* ----一票通日结数据---*/
		{
			DealAccStatFile(CConfig::g_TmpParamPath,0x2004,lineIndex,b_SettleDate);		
		}
		if(b_FileType == 0x03)/* ----一卡通对账数据---*/
		{
			DealAccStatFile(CConfig::g_TmpParamPath,0x2009,lineIndex,b_SettleDate);
			DealAccStatFile(CConfig::g_TmpParamPath,0x200A,lineIndex,b_SettleDate);			 
			DealAccStatFile(CConfig::g_TmpParamPath,0x200B,lineIndex,b_SettleDate);
			DealAccStatFile(CConfig::g_TmpParamPath,0x200C,lineIndex,b_SettleDate);			 
		}
		if(b_FileType == 0x04)/* ----一卡通日结数据---*/
		{
			DealAccStatFile(CConfig::g_TmpParamPath,0x2008,lineIndex,b_SettleDate);	
		}
		if(b_FileType == 0x05)/* ----银联对账数据---*/
		{
			DealAccStatFile(CConfig::g_TmpParamPath,0x2011,lineIndex,b_SettleDate);
			DealAccStatFile(CConfig::g_TmpParamPath,0x2012,lineIndex,b_SettleDate);			 
			DealAccStatFile(CConfig::g_TmpParamPath,0x2013,lineIndex,b_SettleDate);
			DealAccStatFile(CConfig::g_TmpParamPath,0x2014,lineIndex,b_SettleDate);					
		}
		if(b_FileType == 0x06)/* ----银联日结数据---*/
		{
			DealAccStatFile(CConfig::g_TmpParamPath,0x2010,lineIndex,b_SettleDate);
		}

		if(b_FileType == 0x07)/* ----二维码对账数据---*/
		{
			DealAccStatFile(CConfig::g_TmpParamPath,0x2016,lineIndex,b_SettleDate);			 
			DealAccStatFile(CConfig::g_TmpParamPath,0x2017,lineIndex,b_SettleDate);
			DealAccStatFile(CConfig::g_TmpParamPath,0x2018,lineIndex,b_SettleDate);					
			DealAccStatFile(CConfig::g_TmpParamPath,0x2019,lineIndex,b_SettleDate);		
		}
		if(b_FileType == 0x08)/* ----二维码日结数据---*/
		{
			DealAccStatFile(CConfig::g_TmpParamPath,0x2015,lineIndex,b_SettleDate);
		}
		/* ----生成应答报文体---*/
		memset(&m_ACK_6201,0x00,sizeof(ACK_6201));
		memcpy(m_ACK_6201.b_CMDID,m_CMD_6007.b_CMDID,sizeof(m_CMD_6007.b_CMDID));
		memcpy(m_ACK_6201.b_SourceID,m_CMD_6007.b_SourceID,sizeof(m_CMD_6007.b_SourceID));
		memcpy(m_ACK_6201.b_DestID,m_CMD_6007.b_DestID,sizeof(m_CMD_6007.b_DestID));
		m_ACK_6201.b_ACK = (BYTE)0x01;
		/* ----发送应答---*/
		CCommu::SendtoACK(m_para->sockhandle,0x6201,&m_req_msghead,(char*)&m_ACK_6201,sizeof(ACK_6201));

		break;

	case 0x6008://调票结果通知		
		
		CMD_6008 m_CMD_6008;
		char     b_OrderNum[25];
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"接收到清分中心(%s)%04X报文\n",sendid,msgtype);
		CLog::WriteAppLog(logdata);
		/* ----报文解析---*/
		memset(&m_CMD_6008,0x00,sizeof(CMD_6008));
		m_CMD_6008 = *(CMD_6008 *)(m_para->recv_buf+sizeof(MsgHead));
		memset(b_OrderNum,0x00,sizeof(b_OrderNum));
		for(i=0;i<sizeof(m_CMD_6008.b_OrderNum);i++)
			sprintf(b_OrderNum+2*i,"%02X",m_CMD_6008.b_OrderNum[i]);
		/* ----获取调配单文件---*/
		memset(ftpname,0x00,sizeof(ftpname));
		sprintf(ftpname,"%04X_%s",0x3002,b_OrderNum);	
	
		rtn =	DealAccFile3002(CConfig::g_TmpParamPath,0x3002,lineIndex,ftpname);
		if(rtn == 1)/* ----转发给SC---*/
		{
			SC::Upload_SC_StockFile(ftpname,(char*)m_para->recv_buf,m_para->buflen);
		}
		
		/* ----生成应答报文体---*/
		memset(&m_ACK_6201,0x00,sizeof(ACK_6201));
		memcpy(m_ACK_6201.b_CMDID,m_CMD_6008.b_CMDID,sizeof(m_CMD_6008.b_CMDID));
		memcpy(m_ACK_6201.b_SourceID,m_CMD_6008.b_SourceID,sizeof(m_CMD_6008.b_SourceID));
		memcpy(m_ACK_6201.b_DestID,m_CMD_6008.b_DestID,sizeof(m_CMD_6008.b_DestID));
		m_ACK_6201.b_ACK = (BYTE)0x01;
		/* ----发送应答---*/
		CCommu::SendtoACK(m_para->sockhandle,0x6201,&m_req_msghead,(char*)&m_ACK_6201,sizeof(ACK_6201));

		break;

	default:
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"接收到清分中心(%s)未定义报文(%04X)\n",sendid,msgtype);
		CLog::WriteAppLog(logdata);
		/* ----生成应答报文体---*/
		memset(&m_ACK_6201,0x00,sizeof(ACK_6201));
		m_ACK_6201.b_ACK = (BYTE)0x06;
		/* ----发送应答---*/
		CCommu::SendtoACK(m_para->sockhandle,0x6201,&m_req_msghead,(char*)&m_ACK_6201,sizeof(ACK_6201));

		break;
	}
	
	/* ----释放内存 ---*/
	free(m_para);
	m_para = NULL;
	
	return NULL;
}


unsigned int SC::ms_chatseq = 1;
	
/* ------------------------------------------------------
            功能描述:  生成交易文件
--------------------------------------------------------- */
int SC::CreateTransFile(char *dir,int flag,unsigned char* recv_buf,int buflen)
{
	char          	filename[MAX_PATH];
	time_t        	local_time;
	struct tm     	newtime;
	char          	temp[100],oplogmsg[8192];
	int           	i,bodylen,rtn,cur_min,mod_min;
	MsgHead         m_MsgHead;
	char            sendid[10];
	unsigned short  msgtype;
	unsigned char   sendbuf[8192];
	DWORD 					sessionnum;
	char        		c_PkgID[30];
	BYTE         		b_PkgID;
	int             sendlen;
	BYTE         		b_LcId[4];
		
	/* -------- m_MsgHead ---------- */
	memset(&m_MsgHead,0x00,sizeof(MsgHead));
	m_MsgHead = *(MsgHead*)recv_buf; 
	/* -------- 获得报文类型 ---------- */
	msgtype = CommonFuncs::wordtowin(m_MsgHead.w_MsgCode);
	/* -------- 获得发送方节点标识码 ---------- */
	memset(sendid,0x00,sizeof(sendid));
	for(i = 0;i < 4;i++)
	{
		sprintf(sendid + 2*i,"%02X",m_MsgHead.b_SourceID[i]);
	}
 
	/* --- 获取线路节点码 --- */	
	memset(b_LcId,0x00,sizeof(b_LcId));
	
	int lineIndex;		
	int scindex = SC::getscinfo(sendid);
	if(scindex == -1)
	{
		memset(oplogmsg,0x00,sizeof(oplogmsg));
		sprintf(oplogmsg, "CreateTransFile 获取sc(%s)索引失败\n",sendid);
		CLog::WriteAppLog(oplogmsg);	
		
		memcpy(b_LcId,CConfig::g_CCIDCode,sizeof(b_LcId));					
	}
	else
	{
		lineIndex = ACC::getlineinfo(CConfig::g_ScList[scindex].lineId);

		if(lineIndex == -1)
		{
			memset(oplogmsg,0x00,sizeof(oplogmsg));
			sprintf(oplogmsg, "CreateTransFile 获取Lc(%d)索引失败\n",CConfig::g_ScList[scindex].lineId);
			CLog::WriteAppLog(oplogmsg);	
			
			memcpy(b_LcId,CConfig::g_CCIDCode,sizeof(b_LcId));					
		}				
		else
		{					
			CommonFuncs::chartobyte(CConfig::ms_MlcLineList[lineIndex].LINE_NID,(char*)b_LcId,sizeof(b_LcId));
		}
	}

	/* -------- 获得当前时间 ---------- */
	rtn = 1;
	time(&local_time);
	memset(&newtime,0x00,sizeof(struct tm));
	localtime_r(&local_time,&newtime);
	cur_min = newtime.tm_min;
	mod_min = cur_min/CConfig::g_MaxDelay;
	memset(temp,0x00,sizeof(temp));
	sprintf(temp,"%04d%02d%02d%02d%02d",(newtime.tm_year+1900),(newtime.tm_mon+1),newtime.tm_mday,
	                                     newtime.tm_hour,newtime.tm_min);
	/* -- 生成待入库文件 --- */
	if(msgtype == 0x7010)/* -- 即时入库 --- */
	{
		memset(filename,0x00,sizeof(filename));
		sprintf(filename,"%s%04X.%s.%010u.dat",dir,msgtype,sendid,ms_chatseq);
		rtn = CommonFuncs::WriteBufferFile("wb",filename,(char*)recv_buf,buflen);
		if(ms_chatseq > 0xFFFFFFFF)
		{
			ms_chatseq = 0;
		}
		ms_chatseq = ms_chatseq + 1;
	}
	else if(msgtype == 0x1001 || msgtype == 0x1002 || msgtype == 0x1003 ||
		 msgtype == 0x1004 || msgtype == 0x1005 || msgtype == 0x1006) /* -- 交易文件 --- */
	{
		;
	}
	else/* -- 其他文件 --- */
	{
		memset(filename,0x00,sizeof(filename));
		sprintf(filename,"%s%04X.%s.dat",dir,msgtype,temp);
		rtn = CommonFuncs::WriteBufferFile("ab+",filename,(char*)recv_buf,buflen);
	}
	if(rtn == 0)
	{
		memset(oplogmsg,0x00,sizeof(oplogmsg));
		sprintf(oplogmsg,"Write file(%s) error errno:%d strerror:%s\n",filename,errno, strerror(errno));
		CLog::WriteAppLog(oplogmsg);
		
		return rtn;
	}
	/* -------- 生成发送文件ACC(去掉包头/包尾) ---------- */
	rtn = 1;
	if(flag != 0)
	{
		memset(temp,0x00,sizeof(temp));
		sprintf(temp,"%04d%02d%02d%02d%02d",(newtime.tm_year+1900),(newtime.tm_mon+1),newtime.tm_mday,
	                                       newtime.tm_hour,mod_min);
		if(msgtype == 0x1001)
		{
			memset(filename,0x00,sizeof(filename));
			sprintf(filename,"%s%04X.%s.%02X.dat",CConfig::TempSendFilePath,msgtype,temp,b_LcId[0]);
			bodylen = buflen-(sizeof(MsgHead)+sizeof(Trans_1001_1)+LEN_OF_PackageTail);
			rtn = CommonFuncs::WriteBufferFile("ab+",filename,(char*)(recv_buf+sizeof(MsgHead)+sizeof(Trans_1001_1)),bodylen);
		}
		else if(msgtype == 0x1002)
		{
			memset(filename,0x00,sizeof(filename));
			sprintf(filename,"%s%04X.%s.%02X.dat",CConfig::TempSendFilePath,msgtype,temp,b_LcId[0]);
			bodylen = buflen-(sizeof(MsgHead)+sizeof(Trans_1002_1)+LEN_OF_PackageTail);
			rtn = CommonFuncs::WriteBufferFile("ab+",filename,(char*)(recv_buf+sizeof(MsgHead)+sizeof(Trans_1002_1)),bodylen);
		}
		else if(msgtype == 0x1003)
		{
			memset(filename,0x00,sizeof(filename));
			sprintf(filename,"%s%04X.%s.%02X.dat",CConfig::TempSendFilePath,msgtype,temp,b_LcId[0]);
			bodylen = buflen-(sizeof(MsgHead)+sizeof(Trans_1003_1)+LEN_OF_PackageTail);
			rtn = CommonFuncs::WriteBufferFile("ab+",filename,(char*)(recv_buf+sizeof(MsgHead)+sizeof(Trans_1003_1)),bodylen);
		}
		else if(msgtype == 0x1004)
		{
			memset(filename,0x00,sizeof(filename));
			sprintf(filename,"%s%04X.%s.%02X.dat",CConfig::TempSendFilePath,msgtype,temp,b_LcId[0]);
			bodylen = buflen-(sizeof(MsgHead)+sizeof(Trans_1004_1)+LEN_OF_PackageTail);
			rtn = CommonFuncs::WriteBufferFile("ab+",filename,(char*)(recv_buf+sizeof(MsgHead)+sizeof(Trans_1004_1)),bodylen);
		}
		else if(msgtype == 0x1005)
		{
			memset(filename,0x00,sizeof(filename));
			sprintf(filename,"%s%04X.%s.%02X.dat",CConfig::TempSendFilePath,msgtype,temp,b_LcId[0]);
			bodylen = buflen-(sizeof(MsgHead)+sizeof(Trans_1005_1)+LEN_OF_PackageTail);
			rtn = CommonFuncs::WriteBufferFile("ab+",filename,(char*)(recv_buf+sizeof(MsgHead)+sizeof(Trans_1005_1)),bodylen);
		}
		else if(msgtype == 0x1006)
		{
			memset(filename,0x00,sizeof(filename));
			sprintf(filename,"%s%04X.%s.%02X.dat",CConfig::TempSendFilePath,msgtype,temp,b_LcId[0]);
			bodylen = buflen-(sizeof(MsgHead)+sizeof(Trans_1006_1)+LEN_OF_PackageTail);
			rtn = CommonFuncs::WriteBufferFile("ab+",filename,(char*)(recv_buf+sizeof(MsgHead)+sizeof(Trans_1006_1)),bodylen);
		}
		else if(msgtype == 0x5001)
		{
			memset(filename,0x00,sizeof(filename));
			sprintf(filename,"%s%04X.%s.%02X.dat",CConfig::TempSendFilePath,msgtype,temp,b_LcId[0]);
			bodylen = buflen-(sizeof(MsgHead)+sizeof(Trans_5001_1)+LEN_OF_PackageTail);
			rtn = CommonFuncs::WriteBufferFile("ab+",filename,(char*)(recv_buf+sizeof(MsgHead)+sizeof(Trans_5001_1)),bodylen);
		}
		else if(msgtype == 0x5002)
		{
			memset(filename,0x00,sizeof(filename));
			sprintf(filename,"%s%04X.%s.%02X.dat",CConfig::TempSendFilePath,msgtype,temp,b_LcId[0]);
			bodylen = buflen-(sizeof(MsgHead)+sizeof(Trans_5002_1)+LEN_OF_PackageTail);
			rtn = CommonFuncs::WriteBufferFile("ab+",filename,(char*)(recv_buf+sizeof(MsgHead)+sizeof(Trans_5002_1)),bodylen);
		}
		else if(msgtype == 0x5003)
		{
			memset(filename,0x00,sizeof(filename));
			sprintf(filename,"%s%04X.%s.%02X.dat",CConfig::TempSendFilePath,msgtype,temp,b_LcId[0]);
			bodylen = buflen-(sizeof(MsgHead)+sizeof(Trans_5003_1)+LEN_OF_PackageTail);
			rtn = CommonFuncs::WriteBufferFile("ab+",filename,(char*)(recv_buf+sizeof(MsgHead)+sizeof(Trans_5003_1)),bodylen);
		}
		else
		{
			bodylen = buflen - (sizeof(MsgHead)+LEN_OF_PackageTail);
			/* --- 包头 --- */
			sem_wait(&ORA::ms_CSSQL);
			sessionnum = ORA::get_session_number();
			b_PkgID = (BYTE)ORA::get_pkg_number();
			memset(&m_MsgHead,0x00,sizeof(MsgHead));
			memset(c_PkgID,0x00,sizeof(c_PkgID));
			CPackage::Create_Pkg_Head(b_PkgID,&m_MsgHead,bodylen,msgtype,sessionnum,CConfig::g_ACCIDCode,c_PkgID,b_LcId);
			sem_post(&ORA::ms_CSSQL);
 
			/* --- 包文 --- */
			memset(sendbuf,0x00,sizeof(sendbuf));
			if(msgtype == 0x3001)/* --- 申请单位重新设定为线路ID --- */
				recv_buf[sizeof(MsgHead)+13] = b_LcId[0];
					
			sendlen = CPackage::CreatePackage(0,(char*)sendbuf,&m_MsgHead,(char*)(recv_buf+sizeof(MsgHead)),bodylen);	
			/* --- 生成文件于发送目录 --- */
			memset(filename,0x00,sizeof(filename));
			sprintf(filename,"%s%02X_%04X_%s",CConfig::SendFilePath_ACC,b_LcId[0],msgtype,c_PkgID);
			rtn = CommonFuncs::WriteBufferFile("wb",filename,(char*)sendbuf,sendlen);
			
			if(rtn == 0)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg,"Write file(%s) error errno:%d strerror:%s\n",filename,errno, strerror(errno));
				CLog::WriteAppLog(oplogmsg);
			}
			else
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "文件%s生成成功\n",filename);
				CLog::WriteAppLog(oplogmsg);		
			}
		}
		
		if(rtn == 0)
		{
			memset(oplogmsg,0x00,sizeof(oplogmsg));
			sprintf(oplogmsg, "Open file-%s-error-errno:%d strerror:%s\n",filename,errno, strerror(errno));
			CLog::WriteAppLog(oplogmsg);
		}
		else
		{
			memset(oplogmsg,0x00,sizeof(oplogmsg));
			sprintf(oplogmsg, "文件%s操作成功\n",filename);
			CLog::WriteAppLog(oplogmsg);	
		}
	}
	
	if(msgtype == 0x1001 || msgtype == 0x1002 || msgtype == 0x1003 ||
		 msgtype == 0x1004 || msgtype == 0x1005 || msgtype == 0x1006)
	{
		/* ----- 当天数据打包 ----- */
		memset(filename,0x00,sizeof(filename));
		sprintf(filename,"%s%02d/%s.%04X.%02d%02d.dat",CConfig::HisDataPath_LCC,newtime.tm_mday,sendid,msgtype,newtime.tm_mon+1,newtime.tm_mday);
		rtn = CommonFuncs::WriteBufferFile("ab+",filename,(char*)recv_buf,buflen);
	}
	return rtn;
}	
	
/* ------------------------------------------------------
            功能描述:   判断4001报文格式
            返回值说明：1 正常 0 错误
--------------------------------------------------------- */
int SC::format_4001(BYTE *body,int bodylen)
{
	Trans_4001_Head			m_Trans_4001_Head;
	Trans_4001_Record_1	m_Trans_4001_Record_1;
	Trans_4001_Record_2	m_Trans_4001_Record_2;
	BYTE          			*p;
	unsigned short 			wRecords,wRecords_1,wRecords_2;
	int             		len,flag;
	int									j,k,x;
		
	flag = 1;
		
	len = 0;
	len = len + sizeof(Trans_4001_Head);
	if(len > bodylen)
	{
		flag = 0;
		return flag;
	}
	p = body;
	memset(&m_Trans_4001_Head,0x00,sizeof(Trans_4001_Head));
	m_Trans_4001_Head = *(Trans_4001_Head *)p;
	wRecords = 	CommonFuncs::wordtowin(m_Trans_4001_Head.wRecords);	
	p = body + sizeof(Trans_4001_Head);			
	for(j=0;j<wRecords;j++)
	{
		len = len + sizeof(Trans_4001_Record_1);
		if(len > bodylen)
		{
			flag = 0;
			return flag;
		}
			
		memset(&m_Trans_4001_Record_1,0x00,sizeof(Trans_4001_Record_1));
		m_Trans_4001_Record_1 = *(Trans_4001_Record_1 *)p;
		wRecords_1 = CommonFuncs::wordtowin(m_Trans_4001_Record_1.wRecords_1);
		p = p + sizeof(Trans_4001_Record_1);
		for(k=0;k<wRecords_1;k++)
		{
			len = len + sizeof(Trans_4001_Record_2);
			if(len > bodylen)
			{
				flag = 0;
				return flag;
			}
			
			memset(&m_Trans_4001_Record_2,0x00,sizeof(Trans_4001_Record_2));
			m_Trans_4001_Record_2  = *(Trans_4001_Record_2 *)p;
			wRecords_2 = CommonFuncs::wordtowin(m_Trans_4001_Record_2.wRecords_2);
			p = p + sizeof(Trans_4001_Record_2);
			for(x=0;x<wRecords_2;x++)
			{
				len = len + sizeof(Trans_4001_Record_3);
				if(len > bodylen)
				{
					flag = 0;
					return flag;
				}
				
				p = p + sizeof(Trans_4001_Record_3);
			}
		}
			
		len = len + sizeof(Trans_4001_Record_4);
		if(len > bodylen)
		{
			flag = 0;
			return flag;
		}
			
		p = p + sizeof(Trans_4001_Record_4);
	}

	if(len != bodylen)
		flag = 0;
		
	return flag;
}	
	
/* ------------------------------------------------------
            功能描述:  SC报文处理函数
--------------------------------------------------------- */
int SC::OnSCMessageIn(para *tmp_para,int tmp_handle,char *clnip,int clnport)
{
	int             handle;
	para            *m_para;
	unsigned int    bufferlen;
	MsgHead         m_MsgHead;
	char            sendid[10],oplogmsg[200];
	unsigned short  msgtype,records;
	unsigned int    chatseq;
	int             i,ret,len;
	pthread_t       hThreadHandle;

	time_t		      local_time;
	struct tm       newtime;
	char            filepath[64],filename[64];
	char            tempname[256];
	WORD            filetype;
	struct stat     buf;
	int             rtn,mmonth,mday;
	BYTE b_AckCode;
	
	/* -------- 变量赋值 ---------- */
	m_para = tmp_para;
	handle = tmp_handle;
	
	/* -------- 填充变量m_para ---------- */
	memset(&m_MsgHead,0x00,sizeof(MsgHead));
	m_MsgHead = *(MsgHead*)m_para->recv_buf;   
	bufferlen = CommonFuncs::dwordtowin(m_MsgHead.dw_PkgLen) + sizeof(MsgHead) + LEN_OF_PackageTail;
	m_para->buflen = bufferlen;
	m_para->sockhandle = handle;
	/* -------- 获得报文类型 ---------- */
	msgtype = CommonFuncs::wordtowin(m_MsgHead.w_MsgCode);
	/* -------- 获得发送方节点标识码 ---------- */
	memset(sendid,0x00,sizeof(sendid));
	for(i = 0;i < 4;i++)
	{
		sprintf(sendid + 2*i,"%02X",m_MsgHead.b_SourceID[i]);
	}
	/* -------- 获得报文会话流水号 ---------- */
	chatseq = CommonFuncs::dwordtowin(m_MsgHead.dw_PkgID);
	
	/* -------- 写入日志 ---------- */
	memset(oplogmsg,0x00,sizeof(oplogmsg));
	sprintf(oplogmsg, "收到来自SC(%s,%s:%d)的0x%04X报文(会话流水号:%u,报文长度:%d)\n",sendid,clnip,clnport,msgtype,chatseq,bufferlen);
	CLog::WriteAppLog(oplogmsg);
 
	/* --- 记入DEBUG日志 ---*/ 
	if(DEBUG_FLAG)
	{
		memset(oplogmsg,0x00,sizeof(oplogmsg));
		sprintf(oplogmsg,"SC(%s,%s:%d)的%04X报文内容:\n",sendid,clnip,clnport,msgtype);
		CLog::WriteAppLog(oplogmsg);
		CLog::WriteDebugLog(0,(unsigned char*)m_para->recv_buf,m_para->buflen);
	}
			
			
	
	/* -------- HHJT_CJP 20220105 验CRC是否正确---------- */
	//摘出报文中的CRC校验码。
	unsigned 	int oldCrc = 0;
	memcpy(&oldCrc, (unsigned char*)m_para->recv_buf + m_para->buflen - LEN_OF_PackageTail, 4);
	oldCrc = CommonFuncs::dwordtowin(oldCrc) ;

	unsigned int newCrc;
	/*--- 生成CRC32校验 --- */
	Crc::Init_CRC32_Table();
	newCrc = Crc::Crc32((char*)m_para->recv_buf,m_para->buflen - LEN_OF_PackageTail,0);

	if( newCrc == oldCrc)
	{
		memset(oplogmsg,0x00,sizeof(oplogmsg));
		sprintf(oplogmsg,"'OnSleMessageIn' newCrc[0x%08X],oldCrc[0x%08X],Crc is ok!\n",newCrc,oldCrc);
		CLog::WriteAppLog(oplogmsg);
	}
	else //CRC校验失败
	{
		memset(oplogmsg,0x00,sizeof(oplogmsg));
		sprintf(oplogmsg,"'OnSleMessageIn' newCrc[0x%08X],oldCrc[0x%08X],Crc is failed!\n",newCrc,oldCrc);
		CLog::WriteAppLog(oplogmsg);		
	}		
			
			
	switch(msgtype)
	{
		/*--HHJT_CJP 20210321 交易数据处理------*/		
		case 0x1001:
			Trans_1001_1 m_Trans_1001_1;
			memset(&m_Trans_1001_1,0x00,sizeof(Trans_1001_1));
			m_Trans_1001_1 = *(Trans_1001_1 *)(m_para->recv_buf+sizeof(MsgHead));
			records = CommonFuncs::wordtowin(m_Trans_1001_1.w_Records);
			len = sizeof(MsgHead)+sizeof(Trans_1001_1)+records*sizeof(Trans_1001_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
				b_AckCode = (BYTE)0x00;
				CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
					
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertTransFilePath,1,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
					/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;
			
		case 0x1002:
			Trans_1002_1 m_Trans_1002_1;
			memset(&m_Trans_1002_1,0x00,sizeof(Trans_1002_1));
			m_Trans_1002_1 = *(Trans_1002_1 *)(m_para->recv_buf+sizeof(MsgHead));
			records = CommonFuncs::wordtowin(m_Trans_1002_1.w_Records);
			len = sizeof(MsgHead)+sizeof(Trans_1002_1)+records*sizeof(Trans_1002_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertTransFilePath,1,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;
			
		case 0x1003:
			Trans_1003_1 m_Trans_1003_1;
			memset(&m_Trans_1003_1,0x00,sizeof(Trans_1003_1));
			m_Trans_1003_1 = *(Trans_1003_1 *)(m_para->recv_buf+sizeof(MsgHead));
			records = CommonFuncs::wordtowin(m_Trans_1003_1.w_Records);
			len = sizeof(MsgHead)+sizeof(Trans_1003_1)+records*sizeof(Trans_1003_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertTransFilePath,1,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;
			
		case 0x1004:
			Trans_1004_1 m_Trans_1004_1;
			memset(&m_Trans_1004_1,0x00,sizeof(Trans_1004_1));
			m_Trans_1004_1 = *(Trans_1004_1 *)(m_para->recv_buf+sizeof(MsgHead));
			records = CommonFuncs::wordtowin(m_Trans_1004_1.w_Records);
			len = sizeof(MsgHead)+sizeof(Trans_1004_1)+records*sizeof(Trans_1004_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertTransFilePath,1,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;
			
		case 0x1005:
			Trans_1005_1 m_Trans_1005_1;
			memset(&m_Trans_1005_1,0x00,sizeof(Trans_1005_1));
			m_Trans_1005_1 = *(Trans_1005_1 *)(m_para->recv_buf+sizeof(MsgHead));
			records = CommonFuncs::wordtowin(m_Trans_1005_1.w_Records);
			len = sizeof(MsgHead)+sizeof(Trans_1005_1)+records*sizeof(Trans_1005_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertTransFilePath,1,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;		

		case 0x1006:
			Trans_1006_1 m_Trans_1006_1;
			memset(&m_Trans_1006_1,0x00,sizeof(Trans_1006_1));
			m_Trans_1006_1 = *(Trans_1006_1 *)(m_para->recv_buf+sizeof(MsgHead));
			records = CommonFuncs::wordtowin(m_Trans_1006_1.w_Records);
			len = sizeof(MsgHead)+sizeof(Trans_1006_1)+records*sizeof(Trans_1006_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertTransFilePath,1,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;		 
 
  		/*---HHJT_CJP 20210321----票务数据------*/
		case 0x3001:
			Trans_3001_1 m_Trans_3001_1;
			memset(&m_Trans_3001_1,0x00,sizeof(Trans_3001_1));
			m_Trans_3001_1 = *(Trans_3001_1 *)(m_para->recv_buf+sizeof(MsgHead));
			records = CommonFuncs::wordtowin(m_Trans_3001_1.w_Records);
			len = sizeof(MsgHead)+sizeof(Trans_3001_1)+records*sizeof(Trans_3001_2)+sizeof(Trans_3001_3)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,1,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;
			
		case 0x3003:
			Trans_3003_1 m_Trans_3003_1;
			memset(&m_Trans_3003_1,0x00,sizeof(Trans_3003_1));
			m_Trans_3003_1 = *(Trans_3003_1 *)(m_para->recv_buf+sizeof(MsgHead));
			records = CommonFuncs::wordtowin(m_Trans_3003_1.w_Records);
			len = sizeof(MsgHead)+sizeof(Trans_3003_1)+records*sizeof(Trans_3003_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,1,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;
			
		case 0x3005:
			Trans_3005_1 m_Trans_3005_1;
			memset(&m_Trans_3005_1,0x00,sizeof(Trans_3005_1));
			m_Trans_3005_1 = *(Trans_3005_1 *)(m_para->recv_buf+sizeof(MsgHead));
			records = CommonFuncs::wordtowin(m_Trans_3005_1.w_Records);
			len = sizeof(MsgHead)+sizeof(Trans_3005_1)+records*sizeof(Trans_3005_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,1,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;
			
		case 0x3006:
			Trans_3006_1 m_Trans_3006_1;
			memset(&m_Trans_3006_1,0x00,sizeof(Trans_3006_1));
			m_Trans_3006_1 = *(Trans_3006_1 *)(m_para->recv_buf+sizeof(MsgHead));
			records = CommonFuncs::wordtowin(m_Trans_3006_1.w_Records);
			len = sizeof(MsgHead)+sizeof(Trans_3006_1)+records*sizeof(Trans_3006_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,1,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;


		case 0x3007:
			Trans_3007_1 m_Trans_3007_1;
			memset(&m_Trans_3007_1,0x00,sizeof(Trans_3007_1));
			m_Trans_3007_1 = *(Trans_3007_1 *)(m_para->recv_buf+sizeof(MsgHead));
			records = CommonFuncs::wordtowin(m_Trans_3007_1.w_Records);
			len = sizeof(MsgHead)+sizeof(Trans_3007_1)+records*sizeof(Trans_3007_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,1,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;
			
		case 0x3008:
			Trans_3008_1 m_Trans_3008_1;
			memset(&m_Trans_3008_1,0x00,sizeof(Trans_3008_1));
			m_Trans_3008_1 = *(Trans_3008_1 *)(m_para->recv_buf+sizeof(MsgHead));
			records = CommonFuncs::wordtowin(m_Trans_3008_1.w_Records);
			len = sizeof(MsgHead)+sizeof(Trans_3008_1)+records*sizeof(Trans_3008_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,1,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;
		
		/*--HHJT_CJP 20210321 维护数据--------*/	
		case 0x4001:
			int format_flag;
			format_flag =  format_4001(m_para->recv_buf+sizeof(MsgHead),CommonFuncs::dwordtowin(m_MsgHead.dw_PkgLen));
			if(format_flag == 0)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,1,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;
			
		case 0x4002:
			Trans_4002_Head m_Trans_4002_Head;
			unsigned int dw_records;
			memset(&m_Trans_4002_Head,0x00,sizeof(Trans_4002_Head));
			m_Trans_4002_Head = *(Trans_4002_Head *)(m_para->recv_buf+sizeof(MsgHead));
			dw_records = CommonFuncs::dwordtowin(m_Trans_4002_Head.Records);
			len = sizeof(MsgHead)+sizeof(Trans_4002_Head)+dw_records*sizeof(Trans_4002_Record)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,0,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;
				
		case 0x4003:
			len = sizeof(MsgHead)+sizeof(Trans_4003)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,0,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;
			

		
		/*--HHJT_CJP 20210321 审计数据-------------------*/	
		case 0x5001:
			len = sizeof(MsgHead)+sizeof(Trans_5001_1)+sizeof(Trans_5001_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,1,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;
			
		case 0x5002:
			len = sizeof(MsgHead)+sizeof(Trans_5002_1)+sizeof(Trans_5002_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,1,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;
			
		case 0x5003:
			len = sizeof(MsgHead)+sizeof(Trans_5003_1)+sizeof(Trans_5003_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,1,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;

		case 0x5004:
			Trans_5004_Record_2 m_Trans_5004_Record_2;
			Trans_5004_Record_4 m_Trans_5004_Record_4;
			unsigned int 	dw_Records_1;
			unsigned int 	dw_Records_2;
			memset(&m_Trans_5004_Record_2,0x00,sizeof(Trans_5004_Record_2));
			m_Trans_5004_Record_2 = *(Trans_5004_Record_2 *)(m_para->recv_buf+sizeof(MsgHead)+
			                                                 sizeof(Trans_5004_Head)+sizeof(Trans_5004_Record_1));
			dw_Records_1 = CommonFuncs::dwordtowin(m_Trans_5004_Record_2.dw_Records_1);
			
			len = sizeof(MsgHead)+sizeof(Trans_5004_Head)+sizeof(Trans_5004_Record_1)+
			      sizeof(Trans_5004_Record_2)+dw_Records_1*sizeof(Trans_5004_Record_3);
			if(len > bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
				
				free(m_para);
				m_para = NULL;
				break;	
			}
			memset(&m_Trans_5004_Record_4,0x00,sizeof(Trans_5004_Record_4));
			m_Trans_5004_Record_4 = *(Trans_5004_Record_4 *)(m_para->recv_buf+sizeof(MsgHead)+
			                                                 sizeof(Trans_5004_Head)+sizeof(Trans_5004_Record_1)+
			                                                 sizeof(Trans_5004_Record_2)+dw_Records_1*sizeof(Trans_5004_Record_3));
			dw_Records_2 = CommonFuncs::dwordtowin(m_Trans_5004_Record_4.dw_Records_2);
			len = sizeof(MsgHead)+sizeof(Trans_5004_Head)+sizeof(Trans_5004_Record_1)+
			      sizeof(Trans_5004_Record_2)+dw_Records_1*sizeof(Trans_5004_Record_3)+
			      sizeof(Trans_5004_Record_4)+dw_Records_2*sizeof(Trans_5004_Record_5)+
			      LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,1,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;
			
		case 0x5005:
			Trans_5005_Head m_Trans_5005_Head;
			memset(&m_Trans_5005_Head,0x00,sizeof(Trans_5005_Head));
			m_Trans_5005_Head = *(Trans_5005_Head *)(m_para->recv_buf+sizeof(MsgHead));
			records = CommonFuncs::wordtowin(m_Trans_5005_Head.w_Records);
			len = sizeof(MsgHead)+sizeof(Trans_5005_Head)+records*sizeof(Trans_5005_Record_1)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,1,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;

		case 0x5010:
			Trans_5010  m_Trans_5010;		
			memset(&m_Trans_5010,0x00,sizeof(Trans_5010));			
			m_Trans_5010 = *(Trans_5010 *)(m_para->recv_buf+sizeof(MsgHead));	
			len = sizeof(MsgHead)+sizeof(Trans_5010)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,1,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;
			
	

		case 0x5020:
			len = sizeof(MsgHead)+sizeof(Trans_5020)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,0,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;

		case 0x5021: //20230129 新增【0x5021】BOM退故障单信息 
			len = sizeof(MsgHead)+sizeof(Trans_5021)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,0,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;

		case 0x5022: //20230129 【0x5022】BOM非营业性收支数据
			len = sizeof(MsgHead)+sizeof(Trans_5022)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,0,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;


		case 0x7001:
			Trans_7001_1 m_Trans_7001_1;
			memset(&m_Trans_7001_1,0x00,sizeof(Trans_7001_1));
			m_Trans_7001_1 = *(Trans_7001_1 *)(m_para->recv_buf+sizeof(MsgHead));
			records = CommonFuncs::wordtowin(m_Trans_7001_1.w_Records);
			len = sizeof(MsgHead)+sizeof(Trans_7001_1)+records*sizeof(Trans_7001_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,1,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;
			
		case 0x7002:
			Trans_7002_1 m_Trans_7002_1;
			memset(&m_Trans_7002_1,0x00,sizeof(Trans_7002_1));
			m_Trans_7002_1 = *(Trans_7002_1 *)(m_para->recv_buf+sizeof(MsgHead));
			records = CommonFuncs::wordtowin(m_Trans_7002_1.w_Records);
			len = sizeof(MsgHead)+sizeof(Trans_7002_1)+records*sizeof(Trans_7002_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,1,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;	

		case 0x7010:
			Trans_7010_1 m_Trans_7010_1;
			memset(&m_Trans_7010_1,0x00,sizeof(Trans_7010_1));
			m_Trans_7010_1 = *(Trans_7010_1 *)(m_para->recv_buf+sizeof(MsgHead));
			records = CommonFuncs::wordtowin(m_Trans_7010_1.w_Records);
			len = sizeof(MsgHead)+sizeof(Trans_7010_1)+records*sizeof(Trans_7010_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,0,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;


			/*--HHJT_CJP 20210321 现金管理子系统------*/	
		case 0xF001:
			Trans_F001_1 m_Trans_F001_1;
			memset(&m_Trans_F001_1,0x00,sizeof(Trans_F001_1));
			m_Trans_F001_1 = *(Trans_F001_1 *)(m_para->recv_buf+sizeof(MsgHead));
			records = CommonFuncs::wordtowin(m_Trans_F001_1.w_Records);
			len = sizeof(MsgHead)+sizeof(Trans_F001_1)+records*sizeof(Trans_F001_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,0,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			
			free(m_para);
			m_para = NULL;
			break;
 
		case 0xF002:
			Trans_F002_1 m_Trans_F002_1;
			memset(&m_Trans_F002_1,0x00,sizeof(Trans_F002_1));
			m_Trans_F002_1 = *(Trans_F002_1 *)(m_para->recv_buf+sizeof(MsgHead));
			records = CommonFuncs::wordtowin(m_Trans_F002_1.w_Records);
			len = sizeof(MsgHead)+sizeof(Trans_F002_1)+records*sizeof(Trans_F002_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,0,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;
			
		case 0xF003:
			Trans_F003_1 m_Trans_F003_1;
			memset(&m_Trans_F003_1,0x00,sizeof(Trans_F003_1));
			m_Trans_F003_1 = *(Trans_F003_1 *)(m_para->recv_buf+sizeof(MsgHead));
			records = CommonFuncs::wordtowin(m_Trans_F003_1.w_Records);
			len = sizeof(MsgHead)+sizeof(Trans_F003_1)+records*sizeof(Trans_F003_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,0,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;
			
		case 0xF004:
			Trans_F004_1 m_Trans_F004_1;
			memset(&m_Trans_F004_1,0x00,sizeof(Trans_F004_1));
			m_Trans_F004_1 = *(Trans_F004_1 *)(m_para->recv_buf+sizeof(MsgHead));
			records = CommonFuncs::wordtowin(m_Trans_F004_1.w_Records);
			len = sizeof(MsgHead)+sizeof(Trans_F004_1)+records*sizeof(Trans_F004_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,0,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;
			
		case 0xF005:
			Trans_F005_1 m_Trans_F005_1;
			memset(&m_Trans_F005_1,0x00,sizeof(Trans_F005_1));
			m_Trans_F005_1 = *(Trans_F005_1 *)(m_para->recv_buf+sizeof(MsgHead));
			records = CommonFuncs::wordtowin(m_Trans_F005_1.w_Records);
			len = sizeof(MsgHead)+sizeof(Trans_F005_1)+records*sizeof(Trans_F005_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,0,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;
			
		case 0xF006:
			Trans_F006_1 m_Trans_F006_1;
			memset(&m_Trans_F006_1,0x00,sizeof(Trans_F006_1));
			m_Trans_F006_1 = *(Trans_F006_1 *)(m_para->recv_buf+sizeof(MsgHead));
			records = CommonFuncs::wordtowin(m_Trans_F006_1.w_Records);
			len = sizeof(MsgHead)+sizeof(Trans_F006_1)+records*sizeof(Trans_F006_2)+LEN_OF_PackageTail;
			if(len != bufferlen)
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(oplogmsg);
				
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,bufferlen);
			}
			else
			{
				ret = SC::CreateTransFile(CConfig::InsertStatusFilePath,0,m_para->recv_buf,bufferlen);
				if(ret == 1)
				{
				/* ----发送应答---*/
					b_AckCode = (BYTE)0x00;
					CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_MsgHead,(char*)&b_AckCode,sizeof(b_AckCode));
				}
				else
				{
					CSocket::Tcp_Close(m_para->sockhandle);
				}
			}
			free(m_para);
			m_para = NULL;
			break;
				
		default:
			pthread_create(&hThreadHandle, NULL, ThreadDealSCCmd, (void*)m_para);

			break;	
	}

	return 0;
}	
	
/* ------------------------------------------------------
            功能描述:  控制命令报文处理线程函数
--------------------------------------------------------- */
void* SC::ThreadDealSCCmd(void* inpara)
{
	pthread_detach(pthread_self());
	
	int 						i,rtn;
	unsigned short  msgtype;
	char						logdata[1024];
	char						sendid[9],ipaddr[20];
	char            sendbuf[1024];
	int             sendlen;
	MsgHead					m_req_msghead;
	char	      	  rspbuf[8192];
	int           	rsplen=0;
	
	char	  				recvid[9];
	WORD  					ParamType;	   
  char        		c_ParamVer[15];
	char						OperateID[9];
	int     				Operator_FLAG;
	unsigned char   b_AckCode;
  char            APPLY_NO[25];
			
	int len;
	int lineIndex;
	
	para *m_para=NULL;
	m_para=(para *)inpara;
	
	/* ---------------- 获得报文类型 --------------*/
	memset(&m_req_msghead,0x00,sizeof(MsgHead));
	m_req_msghead = *(MsgHead *)(m_para->recv_buf);
	msgtype = CommonFuncs::wordtowin(m_req_msghead.w_MsgCode);
	/* ----------- 获得发送方节点标识码 ------*/
	memset(sendid,0x00,sizeof(sendid));
	for(i=0;i<sizeof(m_req_msghead.b_SourceID);i++)
	{
		sprintf(sendid+2*i,"%02X",m_req_msghead.b_SourceID[i]);
	}
	
	/* -----------COC控制台发出的报文 计算CRC校验 ------*/	
	if(msgtype == 0x6010 || msgtype == 0x6081 || 
		 msgtype == 0x6082 || msgtype == 0x6002 || 
		 msgtype == 0x6022)
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata, "'ThreadDealSCCmd' msgType[0x%04X],sendid[%s]\n",msgtype,sendid);
		CLog::WriteAppLog(logdata);
		
		/*--------更改发送方节点----------*/
		CommonFuncs::chartobyte(CConfig::g_CCID,(char*)m_req_msghead.b_SourceID,sizeof(m_req_msghead.b_SourceID));
 		memcpy(m_para->recv_buf,&m_req_msghead,sizeof(MsgHead));
 
 
		unsigned int ul_crc;
		/*--- 生成CRC32校验 --- */
		Crc::Init_CRC32_Table();
		ul_crc = Crc::Crc32((char*)m_para->recv_buf,m_para->buflen - LEN_OF_PackageTail,0);
		ul_crc = CommonFuncs::dwordtowin(ul_crc);
		memcpy(m_para->recv_buf + m_para->buflen - LEN_OF_PackageTail,&ul_crc,LEN_OF_PackageTail);
		
		/*---发送报文内容-------------*/ 
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata, "计算CRC之后的报文内容:\n");
		CLog::WriteAppLog(logdata);
		CLog::WriteDebugLog(0,(unsigned char*)m_para->recv_buf,m_para->buflen );	
	}
	
 
	
	/* ----------- 处理控制指令报文 ------*/
	switch(msgtype)
	{		
		case 0x3009:/* ---- 非即时退卡查询 ---*/ 
			Trans_3009_2	m_Trans_3009_2;    
			/* --- 变量赋值 ---*/	
			memset(APPLY_NO,0x00,sizeof(APPLY_NO));
			for(i=0;i<12;i++)
				sprintf(APPLY_NO+2*i,"%02X",m_para->recv_buf[sizeof(MsgHead)+i]);
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"接收到%04X报文(受理单号:%s)\n",msgtype,APPLY_NO);
			CLog::WriteAppLog(logdata);
			/* --- 查询单号 ---*/
			memset(&m_Trans_3009_2,0x00,sizeof(Trans_3009_2));
			sem_wait(&ORA::ms_CSSQL);
			rtn = ORA::query_3009_info(APPLY_NO,&m_Trans_3009_2);
			sem_post(&ORA::ms_CSSQL);
			if(rtn == 1)/* ----- 查询成功 ---*/
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata, "受理单号%s查询成功\n",APPLY_NO);
				CLog::WriteAppLog(logdata);
				/* ----发送应答---*/
				CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_req_msghead,(char*)&m_Trans_3009_2,sizeof(Trans_3009_2));
			}
			else
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata, "受理单号%s查询失败\n",APPLY_NO);
				CLog::WriteAppLog(logdata);
				/* ----发送应答---*/
				b_AckCode = (BYTE)0xFF;
				CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_req_msghead,(char*)&b_AckCode,sizeof(b_AckCode));
			}
			break;
			
			
		case 0x5011:
			Trans_5011  m_Trans_5011;
			memset(&m_Trans_5011,0x00,sizeof(Trans_5011));
			m_Trans_5011 = *(Trans_5011 *)(m_para->recv_buf+sizeof(MsgHead));	 
			len = sizeof(MsgHead)+sizeof(Trans_5011)+LEN_OF_PackageTail;
			if(len != m_para->buflen)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata, "%04X包文格式错误,SendID is %s\n",msgtype,sendid);
				CLog::WriteAppLog(logdata);
				
				/* ----发送应答---*/
				b_AckCode = (BYTE)0xFF;
				CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_req_msghead,(char*)&b_AckCode,sizeof(b_AckCode));
					
				/* ----生成错误文件---*/
				COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,m_para->recv_buf,m_para->buflen);
			}
			else
			{
				/* ----发送应答---*/
				b_AckCode = (BYTE)0x00;
				CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_req_msghead,(char*)&b_AckCode,sizeof(b_AckCode));
				
				CSocket::Tcp_Close(m_para->sockhandle);
					
				/* --- 操作员密码修改入库 --- */
				sem_wait(&ORA::ms_CSSQL);
				ORA::update_operator_passwd(&m_Trans_5011);
				sem_post(&ORA::ms_CSSQL);	
							
				/* --- 操作员密码修改转发所有SC --- */				
				Trans_5012		m_Trans_5012;
				memset(&m_Trans_5012,0x00,sizeof(Trans_5012));
				//可直接转发5011报文
		
				/* -------- 生成包头 ---------- */
				char            	c_PkgID[30],filebuf[1024],pkgbody[1024];
				MsgHead	       		m_MsgHead_temp;
				unsigned char 		b_sccode[4];

				memset(&m_MsgHead_temp,0x00,sizeof(MsgHead));
				memset(c_PkgID,0x00,sizeof(c_PkgID));
				memset(b_sccode,0x00,sizeof(b_sccode));
				CommonFuncs::chartobyte(CConfig::g_CCID,(char*)b_sccode,sizeof(b_sccode));
				CPackage::Create_Pkg_Head(1,&m_MsgHead_temp,sizeof(Trans_5011),0x5012,1,b_sccode,c_PkgID);

				memset(pkgbody,0x00,sizeof(pkgbody));
				memcpy(pkgbody,&m_Trans_5011,sizeof(Trans_5011));
				memset(filebuf,0x00,sizeof(filebuf));
				int filelen = CPackage::CreatePackage(0,(char*)filebuf,&m_MsgHead_temp,(char*)pkgbody,sizeof(Trans_5011));
							
			 	SC::getdatasle(0x5012,0,NULL,(unsigned char*)filebuf,filelen);
			}

			break;			
		
		case 0x6002:/* ----运营模式命令/广播---*/
			
			CMD_6002        m_CMD_6002;
			rtn = -1;
			/* --- 参数变量赋值 ---*/	
			memset(&m_CMD_6002,0x00,sizeof(CMD_6002));
			m_CMD_6002 = *(CMD_6002*)(m_para->recv_buf+sizeof(MsgHead));
			memset(recvid,0x00,sizeof(recvid));
			sprintf(recvid,"%02X%02X6101",m_CMD_6002.b_SCID[0],m_CMD_6002.b_SCID[1]);
				
			/* --- 转发 ---*/
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"接收到COC(%s)的0x%04X报文(执行方ID:%s,指令类型:%02X,模式类型:%02X)\n",sendid,msgtype,recvid,m_CMD_6002.b_CmdType,m_CMD_6002.b_RunMode);
			CLog::WriteAppLog(logdata);
					
			for(i=0;i<MAX_SLE_COUNT;i++)
			{
				if(strncmp(CConfig::g_ScList[i].sccode,recvid,4) == 0 && CConfig::g_ScList[i].downloadflag == 1 && CConfig::g_ScList[i].scport > 0)
				{
					rsplen = 0;
					memset(rspbuf,0x00,sizeof(rspbuf));
					rtn = CCommu::SendANDRecv(CConfig::g_ScList[i].scip,CConfig::g_ScList[i].scport,(char *)m_para->recv_buf,m_para->buflen,rspbuf,rsplen,CConfig::g_Wait_Ack_Time);
					break;
				}
			}
			if(rtn == 1)/* ----- 转发成功 ---*/
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata, "0x%04X转发%s成功\n",msgtype,recvid);
				CLog::WriteAppLog(logdata);
					
				/* ----发送应答---*/
				b_AckCode = (BYTE)0x00;
				CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_req_msghead,(char*)&b_AckCode,sizeof(b_AckCode));
			}
			else
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata, "0x%04X转发%s失败\n",msgtype,recvid);
				CLog::WriteAppLog(logdata);
					
				/* ----发送应答---*/		
				b_AckCode = (BYTE)0xFF;
				CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_req_msghead,(char*)&b_AckCode,sizeof(b_AckCode));
			}

			break;

		case 0x6101:/* ----运营模式切换通知---*/
			
			/* ----发送应答---*/
			b_AckCode = (BYTE)0x00;
			CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_req_msghead,(char*)&b_AckCode,sizeof(b_AckCode));
					
			Req_6101        m_Req_6101;
			rtn = -1;
			/* --- 参数变量赋值 ---*/	
			memset(&m_Req_6101,0x00,sizeof(Req_6101));
			m_Req_6101 = *(Req_6101*)(m_para->recv_buf+sizeof(MsgHead));
			memset(recvid,0x00,sizeof(recvid));
			sprintf(recvid,"%02X%02X6101",m_Req_6101.b_SCID[0],m_Req_6101.b_SCID[1]);
				
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"接收到SC(%s)的0x%04X报文(车站ID:%s,模式类型:%02X)\n",sendid,msgtype,recvid,m_Req_6101.b_RunMode);
			CLog::WriteAppLog(logdata);
 	
					
			/* --- 入库 --- */
			sem_wait(&ORA::ms_CSSQL);
			ORA::insert_into_6101(m_para->recv_buf,m_para->buflen);
			sem_post(&ORA::ms_CSSQL);
			/* --- SC转发的运营模式直接转发ACC ---*/	
			if(CConfig::g_RunMode_Flag == 1)
			{
				/* ----- 转发ACC ---*/
				ACC::Send_Acc_6101(msgtype,(char*)&m_Req_6101,sizeof(Req_6101),recvid);
			}
			break;
			
		case 0x6081:/* ---HHJT_CJP 20210324-参数版本查询---*/
			
			CMD_6081_Req_Head   m_CMD_6081_Req_Head;
			rtn = -1;
			/* --- 参数变量赋值 ---*/	
			memset(&m_CMD_6081_Req_Head,0x00,sizeof(CMD_6081_Req_Head));
			m_CMD_6081_Req_Head = *(CMD_6081_Req_Head*)(m_para->recv_buf+sizeof(MsgHead));
			memset(recvid,0x00,sizeof(recvid));
			sprintf(recvid,"%02X%02X6101",m_CMD_6081_Req_Head.b_DestID[0],m_CMD_6081_Req_Head.b_DestID[1]);
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"接收到0x%04X报文(执行方ID:%s)\n",msgtype,recvid);
			CLog::WriteAppLog(logdata);
			/* --- 转发 ---*/
			for(i=0;i<MAX_SLE_COUNT;i++)
			{
				if(strncmp(CConfig::g_ScList[i].sccode,recvid,4) == 0 && CConfig::g_ScList[i].downloadflag == 1 && CConfig::g_ScList[i].scport > 0)
				{
					rsplen = 0;
					memset(rspbuf,0x00,sizeof(rspbuf));
					rtn = CCommu::SendANDRecv(CConfig::g_ScList[i].scip,CConfig::g_ScList[i].scport,(char *)m_para->recv_buf,m_para->buflen,rspbuf,rsplen,CConfig::g_Wait_Ack_Time);
					break;
				}
			}
			if(rtn == 1)/* ----- 转发成功 ---*/
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata, "0x%04X转发%s成功\n",msgtype,recvid,rsplen);
				CLog::WriteAppLog(logdata);
				/* ----发送应答---*/
				CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_req_msghead,(char*)(rspbuf+sizeof(MsgHead)),(rsplen-sizeof(MsgHead)-LEN_OF_PackageTail));
			}
			else
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata, "0x%04X转发%s失败(%d)\n",msgtype,recvid,rtn);
				CLog::WriteAppLog(logdata);
					
				/* ----发送应答---*/		
				b_AckCode = (BYTE)0xFF;
				CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_req_msghead,(char*)&b_AckCode,sizeof(b_AckCode));
			}
			break;
			
			
		case 0x6082:/* ---HHJT_CJP 20210324-软件版本查询---*/
			
			CMD_6082_Req_Head   m_CMD_6082_Req_Head;
			rtn = -1;
			/* --- 参数变量赋值 ---*/	
			memset(&m_CMD_6082_Req_Head,0x00,sizeof(CMD_6082_Req_Head));
			m_CMD_6082_Req_Head = *(CMD_6082_Req_Head*)(m_para->recv_buf+sizeof(MsgHead));
			memset(recvid,0x00,sizeof(recvid));
			sprintf(recvid,"%02X%02X6101",m_CMD_6082_Req_Head.b_DestID[0],m_CMD_6082_Req_Head.b_DestID[1]);
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"接收到0x%04X报文(执行方ID:%s)\n",msgtype,recvid);
			CLog::WriteAppLog(logdata);
 
			/* --- 转发 ---*/
			for(i=0;i<MAX_SLE_COUNT;i++)
			{
				if(strncmp(CConfig::g_ScList[i].sccode,recvid,4) == 0 && CConfig::g_ScList[i].downloadflag == 1 && CConfig::g_ScList[i].scport > 0)
				{
					rsplen = 0;
					memset(rspbuf,0x00,sizeof(rspbuf));
					rtn = CCommu::SendANDRecv(CConfig::g_ScList[i].scip,CConfig::g_ScList[i].scport,(char *)m_para->recv_buf,m_para->buflen,rspbuf,rsplen,CConfig::g_Wait_Ack_Time);
					break;
				}
			}
			if(rtn == 1)/* ----- 转发成功 ---*/
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata, "0x%04X转发%s成功\n",msgtype,recvid,rsplen);
				CLog::WriteAppLog(logdata);
				/* ----发送应答---*/
				CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_req_msghead,(char*)(rspbuf+sizeof(MsgHead)),(rsplen-sizeof(MsgHead)-LEN_OF_PackageTail));
			}
			else
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata, "0x%04X转发%s失败(%d)\n",msgtype,recvid,rtn);
				CLog::WriteAppLog(logdata);
					
				/* ----发送应答---*/		
				b_AckCode = (BYTE)0xFF;
				CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_req_msghead,(char*)&b_AckCode,sizeof(b_AckCode));
			}
			break;		
	 
		case 0x6010:/* ---HHJT_CJP 20210324-控制命令下发---*/		
			CMD_6010        m_CMD_6010;
			rtn = -1;
			/* --- 参数变量赋值 ---*/	
			memset(&m_CMD_6010,0x00,sizeof(CMD_6010));
			m_CMD_6010 = *(CMD_6010*)(m_para->recv_buf+sizeof(MsgHead));
			memset(recvid,0x00,sizeof(recvid));
			sprintf(recvid,"%02X%02X6101",m_CMD_6010.b_DestID[0],m_CMD_6010.b_DestID[1]);
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"接收到0x%04X报文(执行方ID:%s,命令类型:0x%02X)\n",msgtype,recvid,m_CMD_6010.b_CmdCode);
			CLog::WriteAppLog(logdata);
			/* --- 转发 ---*/
			for(i=0;i<MAX_SLE_COUNT;i++)
			{
				if(strncmp(CConfig::g_ScList[i].sccode,recvid,4) == 0 && CConfig::g_ScList[i].downloadflag == 1 && CConfig::g_ScList[i].scport > 0)
				{
					rsplen = 0;
					memset(rspbuf,0x00,sizeof(rspbuf));
					rtn = CCommu::SendANDRecv(CConfig::g_ScList[i].scip,CConfig::g_ScList[i].scport,(char *)m_para->recv_buf,m_para->buflen,rspbuf,rsplen,CConfig::g_Wait_Ack_Time);
					break;
				}
			}
			if(rtn == 1)/* ----- 转发成功 ---*/
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata, "0x%04X转发%s成功\n",msgtype,recvid);
				CLog::WriteAppLog(logdata);
					
				/* ----发送应答---*/		
				b_AckCode = (BYTE)0x00;
				CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_req_msghead,(char*)&b_AckCode,sizeof(b_AckCode));
			}
			else
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata, "0x%04X转发%s失败\n",msgtype,recvid);
				CLog::WriteAppLog(logdata);
					
				/* ----发送应答---*/		
				b_AckCode = (BYTE)0xFF;
				CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_req_msghead,(char*)&b_AckCode,sizeof(b_AckCode));
			}
			
			break;
			

		case 0x8006:		/* --- 线路参数通知报文 ---*/
		
			Cfg8006 				m_Cfg8006;
			
			/* ----发送应答---*/		
			b_AckCode = (BYTE)0x00;
			CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_req_msghead,(char*)&b_AckCode,sizeof(b_AckCode));
						
			/* --- 参数变量赋值 ---*/	
			memset(&m_Cfg8006,0x00,sizeof(Cfg8006));
			m_Cfg8006 = *(Cfg8006*)(m_para->recv_buf+sizeof(MsgHead));
			memset(recvid,0x00,sizeof(recvid));
			for(i=0;i<sizeof(m_Cfg8006.SCID);i++)
				sprintf(recvid+2*i,"%02X",m_Cfg8006.SCID[i]);
			ParamType = CommonFuncs::wordtowin(m_Cfg8006.ParamType);
			memset(c_ParamVer,0x00,sizeof(c_ParamVer));
			for(i=0;i<sizeof(m_Cfg8006.ParamVer);i++)
				sprintf(c_ParamVer+2*i,"%02X",m_Cfg8006.ParamVer[i]);
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"接收到%04X报文(所属节点:%s,参数类型:%04X,参数版本:%s)\n",msgtype,recvid,ParamType,c_ParamVer);
			CLog::WriteAppLog(logdata);

			sem_wait(&ORA::ms_CSSQL);
			switch(ParamType)
			{
				case 0x0300:rtn = ORA::create0300param(recvid,c_ParamVer);break;
				case 0x0301:rtn = ORA::create0301param(recvid,c_ParamVer);break;
				case 0x0302:rtn = ORA::create0302param(recvid,c_ParamVer);break;
				case 0x0303:rtn = ORA::create0303param(recvid,c_ParamVer);break;				
				case 0x0304:rtn = ORA::create0304param(recvid,c_ParamVer);break;
				case 0x0305:rtn = ORA::create0305param(recvid,c_ParamVer);break;
				case 0x0306:rtn = ORA::create0306param(recvid,c_ParamVer);break;
				case 0x0307:rtn = ORA::create0307param(recvid,c_ParamVer);break;
				case 0x0501:rtn = ORA::create0501param(recvid,c_ParamVer);break;
				case 0x0502:rtn = ORA::create0502param(recvid,c_ParamVer);break;
				case 0x0503:rtn = ORA::create0503param(recvid,c_ParamVer);break;
				case 0x0504:rtn = ORA::create0504param(recvid,c_ParamVer);break;
				case 0x2001:
					char SETTLE_DATE[9];
					memset(SETTLE_DATE,0x00,sizeof(SETTLE_DATE));
					memcpy(SETTLE_DATE,c_ParamVer,8);
					ORA::create2001file(recvid,SETTLE_DATE);
					break;
				default:	
					memset(logdata,0x00,sizeof(logdata));
					sprintf(logdata,"系统未定义的参数类型%04X\n",ParamType);
					CLog::WriteAppLog(logdata);
				break;
			}
			sem_post(&ORA::ms_CSSQL);
	 
			break;		
			
		case 0x8007:		/* ---票务数据通知报文 ---*/
		
			Cfg8007 				m_Cfg8007;
			/* ----发送应答---*/		
			b_AckCode = (BYTE)0x00;
			CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_req_msghead,(char*)&b_AckCode,sizeof(b_AckCode));
				
			/* --- 参数变量赋值 ---*/	
			memset(&m_Cfg8007,0x00,sizeof(Cfg8007));
			m_Cfg8007 = *(Cfg8007*)(m_para->recv_buf+sizeof(MsgHead));
			
			memset(recvid,0x00,sizeof(recvid));
			for(i=0;i<sizeof(m_Cfg8007.SCID);i++)
				sprintf(recvid+2*i,"%02X",m_Cfg8007.SCID[i]);
			
			ParamType = CommonFuncs::wordtowin(m_Cfg8007.ParamType);
			
			memset(APPLY_NO,0x00,sizeof(APPLY_NO));
			for(i=0;i<sizeof(m_Cfg8007.APPLY_NO);i++)
				sprintf(APPLY_NO+2*i,"%02X",m_Cfg8007.APPLY_NO[i]);
			
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"接收到%04X报文(参数类型:%04X,申请单号:%s,申请单位:%s)\n",msgtype,ParamType,APPLY_NO,recvid);
			CLog::WriteAppLog(logdata);
			
			sem_wait(&ORA::ms_CSSQL);
			switch(ParamType)
			{
				case 0x3001:ORA::create3001file(APPLY_NO,recvid);break;
				case 0x3005:ORA::create3005file(APPLY_NO,recvid);break;				
				case 0x3006:ORA::create3006file(APPLY_NO,recvid);break;	
				default:
					memset(logdata,0x00,sizeof(logdata));
					sprintf(logdata,"系统未定义的参数类型%04X\n",ParamType);
					CLog::WriteAppLog(logdata);
					break;			
					
			}
			sem_post(&ORA::ms_CSSQL);

			break;	

	case 0x9401:/* --- 时钟请求报文 --- */
			char           CurrentTime[15],b_CurrentTime[7];
				
			/* ----发送应答---*/
			memset(CurrentTime,0x00,sizeof(CurrentTime));
			CTime::GetStringTime(CurrentTime);
			memset(b_CurrentTime,0x00,sizeof(b_CurrentTime));
			CommonFuncs::chartobyte(CurrentTime,b_CurrentTime,sizeof(b_CurrentTime));
			CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_req_msghead,b_CurrentTime,sizeof(b_CurrentTime));
			break;				
			
	case 0x9405:/* --- 人工导入参数通知报文 --- */

			Cfg9405 				m_Cfg9405;				
			/* --- 参数变量赋值 ---*/	
			memset(&m_Cfg9405,0x00,sizeof(Cfg9405));
			m_Cfg9405 = *(Cfg9405*)(m_para->recv_buf+sizeof(MsgHead));
			
			memset(recvid,0x00,sizeof(recvid));
			for(i=0;i<sizeof(m_Cfg9405.SCID);i++)
				sprintf(recvid+2*i,"%02X",m_Cfg9405.SCID[i]);
				
			lineIndex = ACC::getlineinfo(recvid);
			if(lineIndex == -1)
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"人工导入参数通知, 获取(recvid:%s) lineIndex 失败\n",recvid);
				CLog::WriteAppLog(logdata);
			}
			else
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"收到COC人工导入参数通知报文,所属节点(%s),处理开始\n",recvid);
				CLog::WriteAppLog(logdata);
						
				/* --- 处理导入参数 --- */				
				sem_wait(&CPara::g_CSFTP);	
				DealImportParam(0x0015,lineIndex);	
				DealImportParam(0x0018,lineIndex);	
				DealImportParam(0x0019,lineIndex);	
				DealImportParam(0x001A,lineIndex);	
				DealImportParam(0x000E,lineIndex);	
				DealImportParam(0x000B,lineIndex);	
				DealImportParam(0x001B,lineIndex);	
				DealImportParam(0x000C,lineIndex);	
				DealImportParam(0x000D,lineIndex);	
				DealImportParam(0x000F,lineIndex);	
				DealImportParam(0x0401,lineIndex);	
				DealImportParam(0x0402,lineIndex);	
				DealImportParam(0x0403,lineIndex);	
				DealImportParam(0x0406,lineIndex);		
				sem_post(&CPara::g_CSFTP);
					
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"收到COC人工导入参数通知报文,所属节点(%s),处理完毕\n",recvid);
				CLog::WriteAppLog(logdata);		
			}

			/* ----发送应答---*/						 
			b_AckCode = (BYTE)0x00;
			CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_req_msghead,(char*)&b_AckCode,sizeof(b_AckCode));
				
			break;				

		case 0x6022:/* ---HHJT_CJP 20230129 软件更新请求---*/
			CMD_6022   m_CMD_6022;
			rtn = -1;
			/* --- 参数变量赋值 ---*/	
			memset(&m_CMD_6022,0x00,sizeof(CMD_6022));
			m_CMD_6022 = *(CMD_6022*)(m_para->recv_buf+sizeof(MsgHead));
			memset(recvid,0x00,sizeof(recvid));
			sprintf(recvid,"%02X%02X6101",m_CMD_6022.b_DestID[0],m_CMD_6022.b_DestID[1]);
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"接收到[0x%04X]报文(执行方ID:[%s])\n",msgtype,recvid);
			CLog::WriteAppLog(logdata);
 
			/* --- 转发 ---*/
			for(i=0;i<MAX_SLE_COUNT;i++)
			{
				if( CConfig::g_ScList[i].scport > 0 && strncmp(CConfig::g_ScList[i].sccode,recvid,4) == 0 )
				{
					rsplen = 0;
					memset(rspbuf,0x00,sizeof(rspbuf));
					rtn = CCommu::SendANDRecv(CConfig::g_ScList[i].scip,CConfig::g_ScList[i].scport,(char *)m_para->recv_buf,m_para->buflen,rspbuf,rsplen,CConfig::g_Wait_Ack_Time);
					break;
				}
			}
			if(rtn == 1)/* ----- 转发成功 ---*/
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata, "[0x%04X]转发(%s)成功\n",msgtype,recvid);
				CLog::WriteAppLog(logdata);
				/* ----发送应答---*/
				CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_req_msghead,(char*)(rspbuf+sizeof(MsgHead)),(rsplen-sizeof(MsgHead)-LEN_OF_PackageTail));
			}
			else
			{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata, "[0x%04X]转发[%s]失败([%d])\n",msgtype,recvid,rtn);
				CLog::WriteAppLog(logdata);
					
				/* ----发送应答---*/		
				b_AckCode = (BYTE)0xFF;
				CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_req_msghead,(char*)&b_AckCode,sizeof(b_AckCode));
			}
			break;		
			
		default:
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"接收到SC(%s)未定义报文(%04X)\n",sendid,msgtype);
			CLog::WriteAppLog(logdata);
			/* ----发送应答---*/
			b_AckCode = (BYTE)0xF0;
			CCommu::SendtoACK(m_para->sockhandle,msgtype,&m_req_msghead,(char*)&b_AckCode,sizeof(b_AckCode));

			break;
	}
	
	/* ----释放内存 ---*/
	free(m_para);
	m_para = NULL;
	
	return NULL;
}
	
/* ------------------------------------------------------
      功能描述:   定时轮询SC参数版本线程函数
--------------------------------------------------------- */
void* SC::ThreadQueryScParam(void* inpara)
{	
	pthread_detach(pthread_self());
	
	int  rtn,zhandh,nbyte;
	char log[500];
	BYTE temp[4];
	
	while(1)
	{
		CommonFuncs::my_sleep(CConfig::QueryScParamTime);
			
		/* --- 轮询LCC参数,生成文件 --- */
		CPara::CreateParamFile();
 
		/*----------------分线路生成文件-----------------------*/
		for(int i = 0;i < MAX_SLE_COUNT;i++)
		{

			if(CConfig::ms_MlcLineList[i].LINE_ID > 0)
			{
				/* --- 轮询ACC参数版本 ---  HHJR_CJP 不再轮询ACC参数				*/
				ACC::Send_Acc_6102(0x0015,i);	//只能下载0015参数

				/*
				ACC::Send_Acc_6102(0x0019,i);			
				ACC::Send_Acc_6102(0x001A,i);		
				ACC::Send_Acc_6102(0x0018,i);
				ACC::Send_Acc_6102(0x000E,i);
				ACC::Send_Acc_6102(0x000B,i);	
				ACC::Send_Acc_6102(0x001B,i);	
				ACC::Send_Acc_6102(0x000C,i);		
				ACC::Send_Acc_6102(0x000D,i);
				ACC::Send_Acc_6102(0x000F,i);
				ACC::Send_Acc_6102(0x0401,i);	
				ACC::Send_Acc_6102(0x0402,i);	
				ACC::Send_Acc_6102(0x0403,i);		
				ACC::Send_Acc_6102(0x0406,i);
				*/
 	
 
				/* --- 生成设备统计信息文件 ---
				ORA::create7002file(CConfig::ms_MlcLineList[i].LINE_ID);*/

			}
		}
 
		for(int i=0;i<MAX_SLE_COUNT;i++)
		{
			if(CConfig::g_ScList[i].scport > 0 && CConfig::g_ScList[i].downloadflag == 1 && CConfig::g_ScList[i].IsConnect == 1)
			{
				SyncScParam(i);				 
			}
		}
	}
	return NULL;
}	
	
/* ------------------------------------------------------
      功能描述:   定时轮询SC状态线程函数
--------------------------------------------------------- */
void* SC::ThreadQueryScStatus(void* inpara)
{	
	pthread_detach(pthread_self());
	
	int  rtn;
	char applog[500];
	BYTE b_sccode[4];
	char stationNid[5];
	
	while(1)
	{
		CommonFuncs::my_sleep(CConfig::QueryScStatusTime);
 
		for(int i=0;i<MAX_SLE_COUNT;i++)
		{
			if(CConfig::g_ScList[i].scport > 0 )
			{
				/* --- 获得站点号 ---*/
				memset(b_sccode,0x00,sizeof(b_sccode));
				CommonFuncs::chartobyte(CConfig::g_ScList[i].sccode,(char*)b_sccode,sizeof(b_sccode));
									
				rtn = SC::SyncScTime(i);
				if(rtn == 0)
				{
					if(CConfig::g_ScList[i].IsConnect == 1)
					{
						memset(applog,0x00,sizeof(applog));
						sprintf(applog,"车站(%s:%s)与线路中央通信断开\n",CConfig::g_ScList[i].sccode,CConfig::g_ScList[i].scip);
						CLog::WriteAppLog(applog);

						CConfig::g_ScList[i].IsConnect = 0;
						SC::UpdateScStatus(i,1);
						
						/*------更新综合监控信息-----*/	
						memset(stationNid,0x00,sizeof(stationNid));
						sprintf(stationNid,"%02X%02X",b_sccode[0],b_sccode[1]);
						
						ISCS::UpdateIscsStatusSc(stationNid,0);		
					}
				}
				else
				{
					if(CConfig::g_ScList[i].IsConnect == 0)
					{
						memset(applog,0x00,sizeof(applog));
						sprintf(applog,"车站(%s:%s)与线路中央通信恢复正常\n",CConfig::g_ScList[i].sccode,CConfig::g_ScList[i].scip);
						CLog::WriteAppLog(applog);

						CConfig::g_ScList[i].IsConnect = 1;
						SC::UpdateScStatus(i,0);
						
						/*------更新综合监控信息-----*/
						memset(stationNid,0x00,sizeof(stationNid));
						sprintf(stationNid,"%02X%02X",b_sccode[0],b_sccode[1]);
						
						ISCS::UpdateIscsStatusSc(stationNid,1);			
							
					}
				}
			}
		}
	}
	return NULL;
}	 
	
	
/* ------------------------------------------------------
      功能描述:   更新SC通讯状态
--------------------------------------------------------- */
void SC::UpdateScStatus(int index,int flag)
{
	Trans_7010_1   m_Trans_7010_1;
	Trans_7010_2 		m_Trans_7010_2;
	char            	c_PkgID[30];
	MsgHead	       		m_MsgHead;
	unsigned char 		b_sccode[4];
	time_t            t_time;
	struct tm         newtime;
	char              c_time[15];
	char              filename[MAX_PATH],filebuf[1024],pkgbody[1024];
	int               filelen;

	/* -------- 当前时间 ---------- */
	time(&t_time);
	memset(&newtime,0x00,sizeof(struct tm));
	localtime_r(&t_time,&newtime);
	memset(c_time,0x00,sizeof(c_time));
	sprintf(c_time,"%04d%02d%02d%02d%02d%02d",(newtime.tm_year+1900),(newtime.tm_mon+1),newtime.tm_mday,
	                                           newtime.tm_hour,newtime.tm_min,newtime.tm_sec);
	                                           
	/* -------- 结构赋值 ---------- */
	memset(&m_Trans_7010_1,0x00,sizeof(Trans_7010_1));
	m_Trans_7010_1.w_Records = CommonFuncs::wordtowin(0x01);
	CommonFuncs::chartobyte(CConfig::g_ScList[index].sccode,(char*)m_Trans_7010_1.DEVICE_NID,sizeof(m_Trans_7010_1.DEVICE_NID));
	CommonFuncs::chartobyte(c_time,(char*)m_Trans_7010_1.OCCUR_TIME,sizeof(m_Trans_7010_1.OCCUR_TIME));
	
	if(flag == 1)
	{
		m_Trans_7010_1.DEVICE_STATUS = (BYTE)0x05;
		memset(&m_Trans_7010_2,0x00,sizeof(Trans_7010_2));		
		m_Trans_7010_2.EventCode[0] = 0x61;
		m_Trans_7010_2.EventCode[1] = 0x00;
		m_Trans_7010_2.EventCode[2] = 0x00;
		m_Trans_7010_2.EventCode[3] = 0x07;
	}
	else
	{
		m_Trans_7010_1.DEVICE_STATUS = (BYTE)0x00;
		memset(&m_Trans_7010_2,0x00,sizeof(Trans_7010_2));		
		m_Trans_7010_2.EventCode[0] = 0x61;
		m_Trans_7010_2.EventCode[1] = 0x00;
		m_Trans_7010_2.EventCode[2] = 0x00;
		m_Trans_7010_2.EventCode[3] = 0x00;
	}
	
	/* -------- 生成包头 ---------- */
	memset(&m_MsgHead,0x00,sizeof(MsgHead));
	memset(c_PkgID,0x00,sizeof(c_PkgID));
	memset(b_sccode,0x00,sizeof(b_sccode));
	CommonFuncs::chartobyte(CConfig::g_ScList[index].sccode,(char*)b_sccode,sizeof(b_sccode));
	CPackage::Create_Pkg_Head(1,&m_MsgHead,sizeof(Trans_7010_1)+sizeof(Trans_7010_2),0x7010,1,b_sccode,c_PkgID);
	/* -- 生成文件 -- */
	
	sem_wait(&ORA::ms_CSSQL);
	int pkg_number = ORA::get_pkg_number();
	sem_post(&ORA::ms_CSSQL);
	
	
	memset(pkgbody,0x00,sizeof(pkgbody));
	memcpy(pkgbody,&m_Trans_7010_1,sizeof(Trans_7010_1));
	memcpy(pkgbody+sizeof(Trans_7010_1),&m_Trans_7010_2,sizeof(m_Trans_7010_2));
	memset(filebuf,0x00,sizeof(filebuf));
	filelen = CPackage::CreatePackage(0,(char*)filebuf,&m_MsgHead,(char*)pkgbody,sizeof(Trans_7010_1)+sizeof(Trans_7010_2));
	memset(filename,0x00,sizeof(filename));
	sprintf(filename,"%s%04X.%s.%010u",CConfig::InsertStatusFilePath,0x7010,c_time,pkg_number);
	CommonFuncs::WriteBufferFile("wb",filename,filebuf,filelen);
 				
	char oplogmsg[100];
	memset(oplogmsg,0x00,sizeof(oplogmsg));
	sprintf(oplogmsg, "SC(%s:%s)状态文件(%s)生成成功成功\n",CConfig::g_ScList[index].sccode,CConfig::g_ScList[index].scip,filename);
	CLog::WriteAppLog(oplogmsg);
}	
	
/* ------------------------------------------------------
      功能描述:   转发库存文件
--------------------------------------------------------- */
void SC::Upload_SC_StockFile(char *filename,char *sendbuf,int sendlen)
{	
	int i,rtn,acklen;
	char ackbuf[100],oplogmsg[100];
	MsgHead m_MsgHead;
	CMD_6008 m_CMD_6008;
	
	for(i=0;i<MAX_SLE_COUNT;i++)
	{
		if(CConfig::g_ScList[i].scport > 0 && CConfig::g_ScList[i].downloadflag == 1)
		{
			acklen = 0;
			memset(ackbuf,0x00,sizeof(ackbuf));
			memset(&m_MsgHead,0x00,sizeof(MsgHead));
			m_MsgHead = *(MsgHead *)sendbuf;
			memcpy(m_MsgHead.b_SourceID,CConfig::g_CCIDCode,sizeof(CConfig::g_CCIDCode));
			CommonFuncs::chartobyte(CConfig::g_ScList[i].sccode,(char*)m_MsgHead.b_DestID,sizeof(m_MsgHead.b_DestID));
			memcpy(sendbuf,&m_MsgHead,sizeof(MsgHead));
			memset(&m_CMD_6008,0x00,sizeof(CMD_6008));
			m_CMD_6008 = *(CMD_6008 *)(sendbuf+sizeof(MsgHead));
			memcpy(m_CMD_6008.b_SourceID,CConfig::g_CCIDCode,sizeof(CConfig::g_CCIDCode));
			CommonFuncs::chartobyte(CConfig::g_ScList[i].sccode,(char*)m_CMD_6008.b_DestID,sizeof(m_CMD_6008.b_DestID));
			memcpy(sendbuf+sizeof(MsgHead),&m_CMD_6008,sizeof(CMD_6008));
			rtn = CCommu::SendANDRecv(CConfig::g_ScList[i].scip,CConfig::g_ScList[i].scport,sendbuf,sendlen,ackbuf,acklen,CConfig::g_Wait_Ack_Time);
			if(rtn == 1)               /* ----- 转发成功 ---*/
			{
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "库存文件(%s)下发成功,ip地址:%s\n",filename,CConfig::g_ScList[i].scip);
				CLog::WriteAppLog(oplogmsg);
			}
			else
			{
				if(rtn == 0)
				{
					memset(oplogmsg,0x00,sizeof(oplogmsg));
					sprintf(oplogmsg, "库存文件(%s)下发失败,ip地址:%s,没有启动服务\n",filename,CConfig::g_ScList[i].scip);
					CLog::WriteAppLog(oplogmsg);
				}
				else if(rtn == -100)
				{
					memset(oplogmsg,0x00,sizeof(oplogmsg));
					sprintf(oplogmsg, "库存文件(%s)下发失败,ip地址:%s,报文过大(%d)\n",filename,CConfig::g_ScList[i].scip);
					CLog::WriteAppLog(oplogmsg);
				}
				else
				{
					memset(oplogmsg,0x00,sizeof(oplogmsg));
					sprintf(oplogmsg, "库存文件(%s)下发失败,ip地址:%s,应答超时\n",filename,CConfig::g_ScList[i].scip);
					CLog::WriteAppLog(oplogmsg);
				}
			}
		}
	}
}	
	
int SC::DealAccCtlParamFile(int lineIndex,char *name)
{
	Para_0015_1			m_Para_0015_1;
	Para_0015_2			m_Para_0015_2;
	struct stat			buf;
	int							filelen,pkglen;
	char						ftpname[256],filename[MAX_PATH],logdata[1024];
	char						*filebuf;
	unsigned int  	dw_ParamRecords;
	unsigned short	w_ParamType;			
	char						b_ParamVer[15];			
	int			    		i,index,rtn,step;
	int      				ret,rtn_insert_db;

	char 			lcParaPath[MAX_PATH];
	memset(lcParaPath,0x00,sizeof(lcParaPath));
	sprintf(lcParaPath, "%slc%d/",CConfig::g_CurParamPath,CConfig::ms_MlcLineList[lineIndex].LINE_ID);
		
	ret = 0;
 
	/*--- 读取文件内容 ------- */
	memset(filename,0x00,sizeof(filename));
	sprintf(filename,"%s%s",CConfig::g_ImportParamPath,name);
	stat(filename,&buf);
	filelen = buf.st_size;
	filebuf = (char*)malloc(filelen);
	memset(filebuf,0x00,filelen);
	CommonFuncs::ReadFileBuffer(filename,filebuf);
		
	/* ----判断参数版本控制信息文件格式---*/
	memset(&m_Para_0015_1,0x00,sizeof(Para_0015_1));
	m_Para_0015_1 = *(Para_0015_1 *)(filebuf + sizeof(MsgHead));
	dw_ParamRecords = CommonFuncs::dwordtowin(m_Para_0015_1.dw_ParamRecords);
	pkglen = sizeof(MsgHead) + sizeof(Para_0015_1) + dw_ParamRecords * sizeof(Para_0015_2) + LEN_OF_PackageTail;
	if(filelen != pkglen)
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"参数版本控制信息文件(%s)格式错误:文件长度:%d,记录数:%d\n",filename,filelen,dw_ParamRecords);
		CLog::WriteAppLog(logdata);
		free(filebuf);
		filebuf = NULL;
		return 0;
	}
	/* --- 读取参数内容 ------- */
	step = 0;
	for(i = 0;i<dw_ParamRecords;i++)
	{
		rtn = 0;
		memset(&m_Para_0015_2,0x00,sizeof(Para_0015_2));
		m_Para_0015_2 = *(Para_0015_2 *)(filebuf + sizeof(MsgHead) + sizeof(Para_0015_1) + i * sizeof(Para_0015_2));
		w_ParamType = CommonFuncs::wordtowin(m_Para_0015_2.w_ParamType);
			
		memset(b_ParamVer,0x00,sizeof(b_ParamVer));
		for(index = 0;index < sizeof(m_Para_0015_2.b_ParamVer);index++)
			sprintf(b_ParamVer+2*index,"%02X",m_Para_0015_2.b_ParamVer[index]);
			
		switch(w_ParamType)
		{
			case 0x0001:/* ----线路参数---*/
			case 0x0002:/* ----车站参数---*/
			case 0x0004:/* ----换乘关系参数---*/
			case 0x0005:/* ----票型参数---*/
			case 0x0006:/* ----车站区间参数---*/
			case 0x0007:/* ----票价区间参数---*/
			case 0x0008:/* ----区段计次票区间票价参数---*/
			case 0x0009:/* ----区段定期票发售金额参数---*/
			case 0x000A:/* ----纪念票号段发售金额参数---*/
			case 0x0011:/* ----节假日参数---*/
			case 0x0012:/* ----高峰时段参数---*/
			case 0x0016:/* ----读写器参数---*/
			case 0x0013:/* ----TVM主界面版本参数---*/
			case 0x0017:/* ----读写器参数文件---*/
				step = step + 1;
				break;	
			default:
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"系统未定义的参数类型:%04X\n",w_ParamType);
				CLog::WriteAppLog(logdata);
				break;
			}
	}

	if(step == dw_ParamRecords)
	{
		rtn_insert_db = 1;
		/* ----ACC主控参数及其下辖参数生效---*/
		memset(logdata,0x00,sizeof(logdata));
		strcpy(logdata,"ACC主控参数下辖参数接收完成\n");
		CLog::WriteAppLog(logdata);

		/* --- 入库 ---*/
		memset(filename,0x00,sizeof(filename));
		sprintf(filename,"%s%s",CConfig::g_ImportParamPath,name);
		
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"insert_into_0015--%s---\n",filename);
		CLog::WriteAppLog(logdata);
			
		sem_wait(&ORA::ms_CSSQL);
		rtn = ORA::insert_into_0015(filename,lineIndex);
		sem_post(&ORA::ms_CSSQL);
		
		if(rtn == 0)/* --- 主控参数入库失败 ---*/
		{
			ret = 0;
			/* --- 删除主控文件及下辖文件 ------- */
			ACC::del_Acc_0015(1,filebuf,dw_ParamRecords,name,CConfig::g_ImportParamPath);
		}
		else/* --- 主控参数入库成功 ---*/
		{		
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"主控参数入库成功,处理子参数,记录数(%d),处理开始\n",dw_ParamRecords);
			CLog::WriteAppLog(logdata);
					
			for(i = 0;i<dw_ParamRecords;i++)
			{
				memset(&m_Para_0015_2,0x00,sizeof(Para_0015_2));
				m_Para_0015_2 = *(Para_0015_2 *)(filebuf + sizeof(MsgHead) + sizeof(Para_0015_1) + i * sizeof(Para_0015_2));
				w_ParamType = CommonFuncs::wordtowin(m_Para_0015_2.w_ParamType);
					
				memset(b_ParamVer,0x00,sizeof(b_ParamVer));
				for(index = 0;index < sizeof(m_Para_0015_2.b_ParamVer);index++)
					sprintf(b_ParamVer+2*index,"%02X",m_Para_0015_2.b_ParamVer[index]);
				
				/* --- 入库 ---*/			
				memset(filename,0x00,sizeof(filename));
				sprintf(filename,"%s%04X_%s",CConfig::g_ImportParamPath,w_ParamType,b_ParamVer);
 
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"主控参数入库成功,处理子参数(%s)处理开始--------\n",filename);
				CLog::WriteAppLog(logdata);
 
				switch(w_ParamType)
				{
					case 0x0001:/* ----线路参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0001(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x0002:/* ----车站参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0002(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x0004:/* ----换乘关系参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0004(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x0005:/* ----票型参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0005(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x0006:/* ----车站区间参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0006(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x0007:/* ----票价区间参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0007(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x0008:/* ----区段计次票区间票价参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0008(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x0009:/* ----区段定期票发售金额参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0009(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x000A:/* ----纪念票号段发售金额参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_000A(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
						break;
					case 0x0011:/* ----节假日参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0011(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x0012:/* ----高峰时段参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0012(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x0016:/* ----读写器参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0016(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x0013:/* ----TVM主界面版本参数---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0013(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
					case 0x0017:/* ----读写器参数文件---*/
						sem_wait(&ORA::ms_CSSQL);
						rtn = ORA::insert_into_0017(filename,lineIndex);
						sem_post(&ORA::ms_CSSQL);
						if(rtn == 0)
							rtn_insert_db = 0;
						break;
				}
				
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"主控参数入库成功,处理子参数(%s)处理结束--------\n",filename);
				CLog::WriteAppLog(logdata);
			}
			
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"主控参数入库成功,处理子参数,记录数(%d),处理结束\n",dw_ParamRecords);
			CLog::WriteAppLog(logdata);
			/* --- 入库后处理 ---*/
			if(rtn_insert_db == 1)/* --- 入库成功 ---*/
			{
				memset(logdata,0x00,sizeof(logdata));
				strcpy(logdata,"ACC主控参数及其下辖参数入库完成\n");
				CLog::WriteAppLog(logdata);
					
				/* --- 更改参数状态 ------- 
				sem_wait(&g_CSSQL);
				Update_Param_Status(0x0015);
				sem_post(&g_CSSQL);*/
							
				ret = 1;
				
				/* --- 删除当前参数目录文件 ------- */	
				CommonFuncs::DeleteFile(lcParaPath,(char*)"0015_");			
				
				/* --- 移动主控文件 ------- */
				CPara::Rename_ParamFile(1,CConfig::g_ImportParamPath,name,lcParaPath);

				/*------------日志打印--------------
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"CConfig::g_TmpParamPath:%s-name:%s-lcParaPath:%s-\n",CConfig::g_TmpParamPath,name,lcParaPath);
				CLog::WriteAppLog(logdata);*/
				
				/* --- 移动下辖文件 ------- */
				for(i = 0;i<dw_ParamRecords;i++)
				{
					memset(&m_Para_0015_2,0x00,sizeof(Para_0015_2));
					m_Para_0015_2 = *(Para_0015_2 *)(filebuf + sizeof(MsgHead) + sizeof(Para_0015_1) + i * sizeof(Para_0015_2));
					w_ParamType = CommonFuncs::wordtowin(m_Para_0015_2.w_ParamType);
						
					/* --- 更改参数状态 ------- 
					sem_wait(&g_CSSQL);
					Update_Param_Status(w_ParamType);
					sem_post(&g_CSSQL);*/	
					
					/* --- 删除当前参数目录文件 ------- */	
					char strTemp[100];
					memset(strTemp,0x00,sizeof(strTemp));
					sprintf(strTemp,"%04X_",w_ParamType);
					CommonFuncs::DeleteFile(lcParaPath,strTemp);
	 
					memset(b_ParamVer,0x00,sizeof(b_ParamVer));
					for(index = 0;index < sizeof(m_Para_0015_2.b_ParamVer);index++)
						sprintf(b_ParamVer+2*index,"%02X",m_Para_0015_2.b_ParamVer[index]);
	
					memset(ftpname,0x00,sizeof(ftpname));
					sprintf(ftpname,"%04X_%s",w_ParamType,b_ParamVer);
					CPara::Rename_ParamFile(0,CConfig::g_ImportParamPath,ftpname,lcParaPath);

					memset(logdata,0x00,sizeof(logdata));
					sprintf(logdata,"CConfig::g_ImportParamPath:%s-ftpname:%s-lcParaPath:%s-\n",CConfig::g_ImportParamPath,ftpname,lcParaPath);
					CLog::WriteAppLog(logdata);

				}
			}
			else  /* --- 入库失败 ---*/
			{
				/* --- 删除主控参数及其下辖参数已入成功的数据 ------- */
				sem_wait(&ORA::ms_CSSQL);
				ORA::Delete_Param_Status(lineIndex);
				sem_post(&ORA::ms_CSSQL);
				
				memset(logdata,0x00,sizeof(logdata));
				strcpy(logdata,"ACC主控参数及其下辖参数入库失败\n");
				CLog::WriteAppLog(logdata);
				ret = 0;
				/* --- 删除主控文件及下辖文件 ------- */
				ACC::del_Acc_0015(2,filebuf,dw_ParamRecords,name,CConfig::g_ImportParamPath);
			}
		}
	}
	else
	{
		ret = 0;
		/* --- 删除主控文件及下辖文件 ------- */
		ACC::del_Acc_0015(0,filebuf,dw_ParamRecords,name,CConfig::g_ImportParamPath);
	}

	free(filebuf);
	filebuf = NULL;

	return ret;
}

	
/* ------------------------------------------------------
功能描述:   处理导入参数
--------------------------------------------------------- */
void 	SC::DealImportParam(WORD wMsgCode,int lineIndex)
{
	char logdata[1024],filename[1024],subStr[20];
	
	memset(logdata,0x00,sizeof(logdata));
	sprintf(logdata,"导入参数处理开始\n");
	CLog::WriteAppLog(logdata);
				 
		
	/* --- 根据参数类型查找文件名 --- */		
	int isFind = 0;
	memset(subStr,0x00,sizeof(subStr));
	sprintf(subStr,"%04X_",wMsgCode);
 
	char dir[100]; 
	memset(dir,0x00,sizeof(dir));
	strcpy(dir,CConfig::g_ImportParamPath); 
 
	/* --- 打开目录 --- */
	struct dirent   *dirp;
	DIR             *dp;
	
	if((dp = opendir(dir)) == NULL)
		return ;
	/* --- 读取目录 --- */
	memset(filename,0x00,sizeof(filename));
	while((dirp = readdir(dp)) != NULL)
	{
		if(strcmp(dirp->d_name,".") == 0 ||
			strcmp(dirp->d_name,"..") == 0)
			continue;
		if(strstr(dirp->d_name,subStr) != NULL)
		{
			memset(filename,0x00,sizeof(filename));
			sprintf(filename,"%s",dirp->d_name);
			
			isFind = 1;
			break;
		}			
	}
	/* -------- 关闭目录 ---------- */
	closedir(dp);
 
 	if(isFind == 0)
 	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"substr:(%s)在目录(%s)未找到对应的文件\n",subStr,dir);
		CLog::WriteAppLog(logdata);
		
		return;
 	}	
		
	/* -------- 获取新旧版本，比较参数版本 ---------- */	
	char 			lcParaPath[MAX_PATH];
	memset(lcParaPath,0x00,sizeof(lcParaPath));
	sprintf(lcParaPath, "%slc%d/",CConfig::g_CurParamPath,CConfig::ms_MlcLineList[lineIndex].LINE_ID);
	
	char c_oldLccParamVer[15], c_newLccParamVer[15];
	int isNeedDeal = 0;
	memset(c_oldLccParamVer,0x00,sizeof(c_oldLccParamVer));
	int rtn = CPara::GetParamVer(NULL,wMsgCode,lcParaPath,c_oldLccParamVer);	
	if(rtn == 0)
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"参数(0x%04X)在目录(%s)中获取参数版本失败\n",wMsgCode,lcParaPath);
		CLog::WriteAppLog(logdata);		
			
		isNeedDeal = 1;	
		
	}	
	else
	{
		memset(c_newLccParamVer,0x00,sizeof(c_newLccParamVer));
		memcpy(c_newLccParamVer,filename + 5,14);
		
		if(strcmp(c_newLccParamVer,c_oldLccParamVer) == 0)
		{
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"参数(0x%04X)在当前版本(%s),导入参数版本(%s),版本一致,无需处理\n",wMsgCode,c_oldLccParamVer,c_newLccParamVer);
			CLog::WriteAppLog(logdata);		
		}
		else
		{
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"参数(0x%04X)在当前版本(%s),导入参数版本(%s),版本不一致,无需处理\n",wMsgCode,c_oldLccParamVer,c_newLccParamVer);
			CLog::WriteAppLog(logdata);	
				
			isNeedDeal = 1;	
		}
	}
	
	/* -----处理导入参数 ---------- */		
	if (isNeedDeal == 1) 
	{
		if(wMsgCode == 0x0015)
		{
	 		SC::DealAccCtlParamFile(lineIndex,filename);
		}
		else
		{
			CPara::DealParam(wMsgCode,filename,lineIndex,CConfig::g_ImportParamPath);
		}
	}
	
	/* -------- 删除指定文件夹的指定参数 ---------- */			
	char strTemp[100];
	memset(strTemp,0x00,sizeof(strTemp));
	sprintf(strTemp,"%04X_",wMsgCode);
	
	if(wMsgCode == 0x0015) //如果是0015参数，删除子参数
	{
		CommonFuncs::DeleteFile(CConfig::g_ImportParamPath,(char *)"0001_");
		CommonFuncs::DeleteFile(CConfig::g_ImportParamPath,(char *)"0002_");
		CommonFuncs::DeleteFile(CConfig::g_ImportParamPath,(char *)"0004_");			
		CommonFuncs::DeleteFile(CConfig::g_ImportParamPath,(char *)"0005_");
		CommonFuncs::DeleteFile(CConfig::g_ImportParamPath,(char *)"0006_");
		CommonFuncs::DeleteFile(CConfig::g_ImportParamPath,(char *)"0007_");
		CommonFuncs::DeleteFile(CConfig::g_ImportParamPath,(char *)"0008_");
		CommonFuncs::DeleteFile(CConfig::g_ImportParamPath,(char *)"0009_");
		CommonFuncs::DeleteFile(CConfig::g_ImportParamPath,(char *)"000A_");
		CommonFuncs::DeleteFile(CConfig::g_ImportParamPath,(char *)"0011_");
		CommonFuncs::DeleteFile(CConfig::g_ImportParamPath,(char *)"0012_");
		CommonFuncs::DeleteFile(CConfig::g_ImportParamPath,(char *)"0016_");
		CommonFuncs::DeleteFile(CConfig::g_ImportParamPath,(char *)"0013_");
		CommonFuncs::DeleteFile(CConfig::g_ImportParamPath,(char *)"0017_");			
	}
	CommonFuncs::DeleteFile(CConfig::g_ImportParamPath,strTemp);

}	
	
	
	
	
	
	
	
	
	
	
	 
 
   


 

 
ParamVer 		SC::tempParamVerList[MAX_PARAM_COUNT];

/* ------------------------------------------------------
			产生SC服务器的状态文件
--------------------------------------------------------- */
void SC::OccurStatusFile(BYTE *deviceNid,BYTE eventLevel) 
{
	char            c_time[15],c_deviceNid[9];
	char            pkgbody[1024],filebuf[1024];
	int             filelen;
	char            filename[MAX_PATH];
	unsigned int    sessionnum;

	/* -------- 当前时间 ---------- */
	memset(c_time,0x00,sizeof(c_time));
	CTime::GetStringTime(c_time);

	
	memset(pkgbody,0x00,sizeof(pkgbody)); 
	int recordCount = 0; 
	if(!CConfig::ms_AccIsConnect)
	{		

	}
	if(CConfig::ms_DiskIsFull)
	{		

	}
	if(CConfig::ms_SynTimeOverDiff)
	{		

	}
 	if(CConfig::ms_SynTimeError)
	{		

	}

	
	/* -- 生成状态入库文件 -- */
	memset(c_deviceNid,0x00,sizeof(c_deviceNid));
	for(int i=0;i<4;i++)
	{
		sprintf(c_deviceNid + 2*i,"%02X",deviceNid[i]);
	}
	

			
}


/*----------------------------------
更新参数列表,返回参数个数
------------------------------------*/ 
int SC::GetParamVerList(int scIndex)
{
	char temp[5],logdata[1024];
	memset(temp,0x00,sizeof(temp));
	memcpy(temp,CConfig::g_ScList[scIndex].sccode,4);
	
	int lineIndex = ACC::getlineinfo(CConfig::g_ScList[scIndex].lineId);
	if(lineIndex == -1)
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"GetParamVerList 获取(SC:%s) lineIndex 失败\n",temp);
		CLog::WriteAppLog(logdata);
		return 0;
	}
		
	char 			lcParaPath[MAX_PATH];
	memset(lcParaPath,0x00,sizeof(lcParaPath));
	sprintf(lcParaPath, "%slc%d/",CConfig::g_CurParamPath,CConfig::ms_MlcLineList[lineIndex].LINE_ID);	
	
	int count = 0;
	int           ret;
	char          c_ParamVer[15];
  int           rtn;
  
	/* -------- 0301 ---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(temp,0x0301,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x0301;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}
	
	/* -------- 0302 ---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(temp,0x0302,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x0302;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}
 
	/* -------- 0303 ---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x0303,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x0303;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}
 
	/* -------- 0304 ---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x0304,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x0304;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}

	/* -------- 0305 ---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x0305,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x0305;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}

	/* -------- 0306 ---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x0306,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x0306;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}	
 
	/* -------- 0307 ---------- 
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x0307,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x0307;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}		*/

	/* -------- 0501 ---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x0501,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x0501;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}		
	
	/* -------- 0502 ---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x0502,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x0502;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}			
	
	/* -------- 0503 ---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x0503,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x0503;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}			
		
	/* -------- 0504 ---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x0504,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x0504;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}		
	
	/* -------- 0015 ---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x0015,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x0015;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}		
	 
	/* -------- 000B ---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x000B,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x000B;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}		

	/* -------- 000C ---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x000C,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x000C;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}		

	/* -------- 000D ---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x000D,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x000D;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}		
	
	/* -------- 000E ---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x000E,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x000E;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}			

	/* -------- 000F ---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x000F,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x000F;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}		
 
	/* -------- 0018---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x0018,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x0018;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}		
			 
	/* -------- 0019---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x0019,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x0019;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}			 
	 
	/* -------- 001A---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x001A,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x001A;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}		
	
	/* -------- 001B---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x001B,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x001B;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}			
	 
	/* -------- 0401---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x0401,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x0401;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}			 

	/* -------- 0402---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x0402,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x0402;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}			 
 
	/* -------- 0403---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x0403,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x0403;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}				 

	/* -------- 0406---------- */
	memset(c_ParamVer,0x00,sizeof(c_ParamVer));
	rtn = CPara::GetParamVer(NULL,0x0406,lcParaPath,c_ParamVer);
	if(rtn == 1)
	{
		tempParamVerList[count].wParaCode = 0x0406;
		CommonFuncs::chartobyte(c_ParamVer,(char*)tempParamVerList[count].ParamVer,sizeof(tempParamVerList[count].ParamVer));
		count++;
	}			
	
	return count;
}
 
/*----------------------------------
产生参数查询0x6081报文
------------------------------------*/ 
int SC::CreateParamQuery(int scIndex,int paramCount,char *sendbuf)
{
	CMD_6081_Req_Head      m_CMD_6081_Req_Head;
 	CMD_6081_Req_Record    m_CMD_6081_Req_Record; 	
	MsgHead	       	m_MsgHead;
	int  	       		sndlen;
	char            c_PkgID[30];
	unsigned char   b_sccode[4];
	
	sndlen = 0;
	CommonFuncs::chartobyte(CConfig::g_ScList[scIndex].sccode,(char*)b_sccode,sizeof(b_sccode));
	int bodyLen = sizeof(CMD_6081_Req_Head) + paramCount * sizeof(CMD_6081_Req_Record);
	
	sem_wait(&ORA::ms_CSSQL);
	int sessionnum = ORA::get_session_number();
	sem_post(&ORA::ms_CSSQL);		

 	/* --- 包头 --- */
	memset(&m_MsgHead,0x00,sizeof(MsgHead));
	memset(c_PkgID,0x00,sizeof(c_PkgID));
	CPackage::Create_Pkg_Head(1,&m_MsgHead,bodyLen,0x6081,sessionnum,b_sccode,c_PkgID);
		
	/* --- 包体 --- */
	memset(&m_CMD_6081_Req_Head,0x00,sizeof(CMD_6081_Req_Head));
	memcpy(m_CMD_6081_Req_Head.b_DestID,b_sccode,4);	
	m_CMD_6081_Req_Head.b_ParamType = 0x00;
	
	char *bodyBuf = (char*)malloc(bodyLen);
	memcpy(bodyBuf,&m_CMD_6081_Req_Head,sizeof(CMD_6081_Req_Head));
	for(int i = 0;i < paramCount;i++)
	{
		memset(&m_CMD_6081_Req_Record,0x00,sizeof(CMD_6081_Req_Record));
		m_CMD_6081_Req_Record.w_ParamType = CommonFuncs::wordtowin(tempParamVerList[i].wParaCode);		
		memcpy(m_CMD_6081_Req_Record.b_ParamVer,tempParamVerList[i].ParamVer,7);

		memcpy(bodyBuf + sizeof(CMD_6081_Req_Head) + i * sizeof(CMD_6081_Req_Record),&m_CMD_6081_Req_Record,sizeof(CMD_6081_Req_Record));
	}

	/* --- 包文 --- */
	sndlen = CPackage::CreatePackage(0,(char*)sendbuf,&m_MsgHead,bodyBuf,bodyLen);
 
	free(bodyBuf);
	bodyBuf = NULL;
 
 	return sndlen;
} 
 
 
/* ------------------------------------------------------
产生参数同步0x6005报文
--------------------------------------------------------- */
int SC::CreateParamSync(WORD msgtype,unsigned char *sccode,unsigned char *ParamVer,char *sendbuf)
{
	CMD_6005        m_CMD_6005;
	unsigned char   cmdcode;
	MsgHead	       	m_MsgHead;
	int  	       		sndlen;
	char            c_PkgID[30];
	char            logdata[1024];

	sndlen = 1;
/*-------------------------------------
1（0x01）：运营参数
2（0x02）：票卡目录参数[0x0019]
3（0x03）：节点通信参数[0x001A]
4（0x04）：模式履历参数[0x0018]
5（0x05）：增量黑名单参数[0x000E]
6（0x06）：一票通全量黑名单参数[0x000B]
7（0x07）：LC参数版本控制信息[0x0300]
8（0x08）：预留
9（0x09）：预留
10（0x0A）：预留
11（0x0B）：预留
12（0x0C）：车票回收参数下发[0x001B]
13（0x0D）：SAM卡全量黑名单参数[0x000C]
14（0x0E）：一卡通全量黑名单参数[0x000D]
15（0x0F）：交通部白名单参数[0x000F]
16（0x10）：银联PAY黑名单参数[0x0406]
17（0x11）：银联PAY公钥参数[0x0401]
18（0x12）：互联网应用参数[0x0402]
19（0x13）：[0x0402]—需要ACC确认
20（0x14）~32（0x20）：预留
33(0x21)：【0x0301】车站节点配置参数
34(0x22)：【0x0302】操作员权限参数
35(0x23)：【0x0303】设备运营参数
36(0x24)：【0x0304】TVM参数信息
37(0x25)：【0x0305】BOM参数信息
38(0x26)：【0x0306】AG参数信息
39(0x27)：【0x0307】TCM参数信息-预留
40(0x28)：设备事件代码—预留
41(0x14)~48(0x20)：预留
49(0x31)：【0x0501】TVM设备软件
50(0x32)：【0x0502】AGM设备软件
51(0x33)：【0x0503】BOM设备软件
51(0x34)：【0x0504】TCM设备软件
-------------------------------------*/
	/* --- 参数类型 ---*/	
	switch(msgtype)
	{
		case 0x0015: cmdcode = 0x01; break;
		case 0x0018: cmdcode = 0x04; break;
		case 0x0019: cmdcode = 0x02; break;
		case 0x001A: cmdcode = 0x03; break;
		case 0x000E: cmdcode = 0x05; break;
		case 0x000B: cmdcode = 0x06; break;
		case 0x001B: cmdcode = 0x0C; break;
		case 0x000C: cmdcode = 0x0D; break;
		case 0x000D: cmdcode = 0x0E; break;
		case 0x000F: cmdcode = 0x0F; break;
		case 0x0401: cmdcode = 0x11; break;
		case 0x0402: cmdcode = 0x12; break;
		case 0x0403: cmdcode = 0x13; break;
		case 0x0406: cmdcode = 0x10; break;
 
		case 0x0301: cmdcode = 0x21; break;
		case 0x0302: cmdcode = 0x22; break;
		case 0x0303: cmdcode = 0x23; break;
		case 0x0304: cmdcode = 0x24; break;
		case 0x0305: cmdcode = 0x25; break;
		case 0x0306: cmdcode = 0x26; break;
		case 0x0307: cmdcode = 0x27; break;

		case 0x0501: cmdcode = 0x31; break;
		case 0x0502: cmdcode = 0x32; break;
		case 0x0503: cmdcode = 0x33; break;
		case 0x0504: cmdcode = 0x34; break;
									
		default:
			sndlen = 0;
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"系统未定义的参数类型:0x%04X\n",msgtype);
			CLog::WriteAppLog(logdata);
			break;
	}
	if(sndlen > 0)
	{
		sem_wait(&ORA::ms_CSSQL);
		int sessionnum = ORA::get_session_number();
		sem_post(&ORA::ms_CSSQL);				

		/* --- 包头 --- */
		memset(&m_MsgHead,0x00,sizeof(MsgHead));
		memset(c_PkgID,0x00,sizeof(c_PkgID));
		CPackage::Create_Pkg_Head(1,&m_MsgHead,sizeof(CMD_6005),0x6005,sessionnum,sccode,c_PkgID);
		/* --- 包体 --- */
		memset(&m_CMD_6005,0x00,sizeof(CMD_6005));
		memcpy(m_CMD_6005.b_SourceID,CConfig::g_CCIDCode,sizeof(CConfig::g_CCIDCode));
		memcpy(m_CMD_6005.b_DestID,sccode,4);
		m_CMD_6005.b_ParamType = cmdcode;
		memcpy(m_CMD_6005.b_ParamVer,ParamVer,7);
		/* --- 包文 --- */
		sndlen = CPackage::CreatePackage(0,(char*)sendbuf,&m_MsgHead,(char*)&m_CMD_6005,sizeof(CMD_6005));
	}
	
	return sndlen;
} 
 
 
 
/*----------------------------------
1.产生参数列表
2.发送参数查询_0x6081报文
3.解析参数应答报文并比较
4.发送参数同步_6005报文
------------------------------------*/ 
void  SC::SyncScParam(int scIndex)
{
	char oplogmsg[1024];
		
	memset(oplogmsg,0x00,sizeof(oplogmsg));
	sprintf(oplogmsg, "同步(%s:%s)参数开始！\n",CConfig::g_ScList[scIndex].sccode,CConfig::g_ScList[scIndex].scip);
	CLog::WriteAppLog(oplogmsg);
		
		
	int paramCount = SC::GetParamVerList(scIndex);

	if(paramCount == 0) 
	{
		return;
	}	
	
	int sendlen;
	char sendbuf[8192];

	memset(sendbuf,0x00,sizeof(sendbuf));		
	sendlen = SC::CreateParamQuery(scIndex,paramCount,sendbuf);

	if(sendlen == 0) 
	{
		return;
	}	
 
  /*---发送报文并接收应答-------------*/
  int ret,acklen,rsplen=0;
	char rspbuf[8192],ackbuf[8192];

  
  /*---发送报文内容-------------*/ 
	memset(oplogmsg,0x00,sizeof(oplogmsg));
	sprintf(oplogmsg, "参数查询发送报文(%s:%s,报文长度:%d)\n",CConfig::g_ScList[scIndex].sccode,CConfig::g_ScList[scIndex].scip,sendlen);
	CLog::WriteAppLog(oplogmsg);
	CLog::WriteDebugLog(0,(unsigned char*)sendbuf,sendlen);
		
	memset(rspbuf,0x00,sizeof(rspbuf));	
  ret = CCommu::SendANDRecv(CConfig::g_ScList[scIndex].scip,CConfig::g_ScList[scIndex].scport,(char *)sendbuf,sendlen,rspbuf,rsplen,CConfig::g_Wait_Ack_Time);
	
	if(ret == 1)   /* ----- 转发成功 ---*/
	{
		memset(oplogmsg,0x00,sizeof(oplogmsg));
		sprintf(oplogmsg, "参数查询下发成功(%s:%s)\n",CConfig::g_ScList[scIndex].sccode,CConfig::g_ScList[scIndex].scip);
		CLog::WriteAppLog(oplogmsg);
	}
	else
	{
		if(ret == 0)
		{
			memset(oplogmsg,0x00,sizeof(oplogmsg));
			sprintf(oplogmsg, "参数查询下发失败(%s:%s),没有启动服务\n",CConfig::g_ScList[scIndex].sccode,CConfig::g_ScList[scIndex].scip);
			CLog::WriteAppLog(oplogmsg);
		}
		else if(ret == -100)
		{
			memset(oplogmsg,0x00,sizeof(oplogmsg));
			sprintf(oplogmsg, "参数查询下发失败(%s:%s),报文过大(%d)\n",CConfig::g_ScList[scIndex].sccode,CConfig::g_ScList[scIndex].scip,sendlen);
			CLog::WriteAppLog(oplogmsg);
		}
		else
		{
			memset(oplogmsg,0x00,sizeof(oplogmsg));
			sprintf(oplogmsg, "参数查询下发失败(%s:%s),应答超时\n",CConfig::g_ScList[scIndex].sccode,CConfig::g_ScList[scIndex].scip);
			CLog::WriteAppLog(oplogmsg);
		}
		
		memset(oplogmsg,0x00,sizeof(oplogmsg));
		sprintf(oplogmsg, "同步(%s:%s)参数结束！\n",CConfig::g_ScList[scIndex].sccode,CConfig::g_ScList[scIndex].scip);
		CLog::WriteAppLog(oplogmsg);
		
		return;
	}
	
	/*---处理接收应答-------------*/ 
	memset(oplogmsg,0x00,sizeof(oplogmsg));
	sprintf(oplogmsg, "参数查询应答报文(%s:%s,报文长度:%d)\n",CConfig::g_ScList[scIndex].sccode,CConfig::g_ScList[scIndex].scip,rsplen);
	CLog::WriteAppLog(oplogmsg);
	CLog::WriteDebugLog(0,(unsigned char*)rspbuf,rsplen);
	 
	 
	CMD_6081_Ack_Head			m_CMD_6081_Ack_Head;
	CMD_6081_Ack_Record		m_CMD_6081_Ack_Record;
	
	memset(&m_CMD_6081_Ack_Head,0x00,sizeof(CMD_6081_Ack_Head));
	m_CMD_6081_Ack_Head = *(CMD_6081_Ack_Head *)(rspbuf + sizeof(MsgHead));
	if( (int)m_CMD_6081_Ack_Head.b_AckCode != 0x00)
	{
		memset(oplogmsg,0x00,sizeof(oplogmsg));
		sprintf(oplogmsg, "参数查询应答报文,应答码为0x%02X,后续不予处理\n",m_CMD_6081_Ack_Head.b_AckCode);
		CLog::WriteAppLog(oplogmsg);
			
		memset(oplogmsg,0x00,sizeof(oplogmsg));
		sprintf(oplogmsg, "同步(%s:%s)参数结束！\n",CConfig::g_ScList[scIndex].sccode,CConfig::g_ScList[scIndex].scip);
		CLog::WriteAppLog(oplogmsg);
			
		return ;
	}
	
	unsigned char 	b_sccode[4];
	memset(b_sccode,0x00,sizeof(b_sccode));
	CommonFuncs::chartobyte(CConfig::g_ScList[scIndex].sccode,(char*)b_sccode,sizeof(b_sccode));
 
	
	int ackParamCount = (rsplen - sizeof(MsgHead) - LEN_OF_PackageTail - sizeof(CMD_6081_Ack_Head))/sizeof(CMD_6081_Ack_Record);
	unsigned short  wParaCode; 
	char  					ccParamVer[15],scParamVer[15];
	int 						isFound,isNeedSync;
	
	for(int i = 0;i < paramCount;i++)
	{
		memset(ccParamVer,0x00,sizeof(ccParamVer));	
		for (int k = 0; k < sizeof(tempParamVerList[i].ParamVer); k++)
		{
			sprintf(ccParamVer + 2 * k,"%02X",tempParamVerList[i].ParamVer[k]);
		}	
		wParaCode = tempParamVerList[i].wParaCode;
		
		isFound = 0;
		isNeedSync = 0;
		for (int j=0; j < ackParamCount; j++)
		{
			memset(&m_CMD_6081_Ack_Record,0x00,sizeof(CMD_6081_Ack_Record));
			m_CMD_6081_Ack_Record = *(CMD_6081_Ack_Record *)(rspbuf + sizeof(MsgHead) + sizeof(CMD_6081_Ack_Head) + j * sizeof(CMD_6081_Ack_Record));
			
			memset(scParamVer,0x00,sizeof(scParamVer));	
			for (int k = 0; k < sizeof(m_CMD_6081_Ack_Record.ParamVer); k++)
			{
				sprintf(scParamVer + 2 * k,"%02X",m_CMD_6081_Ack_Record.ParamVer[k]);
			}	
			
			if (CommonFuncs::wordtowin(m_CMD_6081_Ack_Record.wParaCode) == wParaCode) 
			{
				if(strcmp(ccParamVer,scParamVer) == 0)
				{
					memset(oplogmsg,0x00,sizeof(oplogmsg));
					sprintf(oplogmsg, "设备(%s)的0x%04X的参本为%s,线路中央为%s,一致不需要同步\n",CConfig::g_ScList[scIndex].sccode,wParaCode,scParamVer,ccParamVer);
					CLog::WriteAppLog(oplogmsg);
				}
				else 
				{
					memset(oplogmsg,0x00,sizeof(oplogmsg));
					sprintf(oplogmsg, "设备(%s)的0x%04X的参本为%s,线路中央为%s,不一致需要同步\n",CConfig::g_ScList[scIndex].sccode,wParaCode,scParamVer,ccParamVer);
					CLog::WriteAppLog(oplogmsg);
					
					isNeedSync = 1;
				}
					
				isFound = 1;
				break;			
			}
		}
		
		if (isFound == 0 || isNeedSync == 1) 
		{
			/* --- 生成同步报文 ---*/
			memset(sendbuf,0x00,sizeof(sendbuf));
			sendlen = CreateParamSync(wParaCode,b_sccode,tempParamVerList[i].ParamVer,sendbuf);
			if(sendlen != 0)
			{
				
				/*---发送报文内容-------------*/ 
				memset(oplogmsg,0x00,sizeof(oplogmsg));
				sprintf(oplogmsg, "参数(0x%04X)同步发送报文(%s:%s,报文长度:%d)\n",wParaCode,CConfig::g_ScList[scIndex].sccode,CConfig::g_ScList[scIndex].scip,sendlen);
				CLog::WriteAppLog(oplogmsg);
				CLog::WriteDebugLog(0,(unsigned char*)sendbuf,sendlen);
 
				/* --- 发送包文 --- */
				acklen = 0;
				memset(ackbuf,0x00,sizeof(ackbuf));
				ret =  CCommu::SendANDRecv(CConfig::g_ScList[scIndex].scip,CConfig::g_ScList[scIndex].scport,sendbuf,sendlen,ackbuf,acklen,CConfig::g_Wait_Ack_Time);											
				if(ret != 1)
				{
					if(ret == 0)
					{
						memset(oplogmsg,0x00,sizeof(oplogmsg));
						sprintf(oplogmsg,"参数(0x%04X)转发SC(%s:%s)失败:连接SC失败\n",wParaCode,CConfig::g_ScList[scIndex].sccode,CConfig::g_ScList[scIndex].scip);
						CLog::WriteAppLog(oplogmsg);
					}
					else
					{
						memset(oplogmsg,0x00,sizeof(oplogmsg));
						sprintf(oplogmsg,"参数(0x%04X)转发SC(%s:%s)失败:等待SC应答超时\n",wParaCode,CConfig::g_ScList[scIndex].sccode,CConfig::g_ScList[scIndex].scip);
						CLog::WriteAppLog(oplogmsg);
					}
				}
				else
				{
					
					/*---处理接收应答-------------*/ 
					memset(oplogmsg,0x00,sizeof(oplogmsg));
					sprintf(oplogmsg, "参数(0x%04X)同步应答报文(%s:%s,报文长度:%d)\n",wParaCode,CConfig::g_ScList[scIndex].sccode,CConfig::g_ScList[scIndex].scip,acklen);
					CLog::WriteAppLog(oplogmsg);
					CLog::WriteDebugLog(0,(unsigned char*)ackbuf,acklen);
 
					memset(oplogmsg,0x00,sizeof(oplogmsg));
					sprintf(oplogmsg,"同步SC(%s:%s):0x%04X参数成功\n",CConfig::g_ScList[scIndex].sccode,CConfig::g_ScList[scIndex].scip,wParaCode);
					CLog::WriteAppLog(oplogmsg);
				}	
			}
		}
	}
	
	memset(oplogmsg,0x00,sizeof(oplogmsg));
	sprintf(oplogmsg, "同步(%s:%s)参数结束！\n",CConfig::g_ScList[scIndex].sccode,CConfig::g_ScList[scIndex].scip);
	CLog::WriteAppLog(oplogmsg);
}

/*----------------------------------
同步SC时钟
成功返回1,失败返回0
------------------------------------*/ 
int  SC::SyncScTime(int scIndex)
{
	char      		sendbuf[100],ackbuf[100];
	int       		acklen=0;
	int       		sendlen;
	int       		rtn;
	MsgHead       m_MsgHead;
	BYTE          sccode[4];
	char          c_PkgID[30];
	time_t    		local_time;
	unsigned int	dw_now;
	CMD_6011   		m_CMD_6011;
 
	/* --- 包体 --- */		
	local_time = time(NULL);
	dw_now = (unsigned int)local_time;
	memset(&m_CMD_6011,0x00,sizeof(CMD_6011));
	m_CMD_6011.dwNowTime = CommonFuncs::dwordtowin(dw_now);
		
	/* --- 包文 --- */
	memset(sccode,0x00,sizeof(sccode));
	CommonFuncs::chartobyte(CConfig::g_ScList[scIndex].sccode,(char*)sccode,sizeof(sccode));
	memset(&m_MsgHead,0x00,sizeof(MsgHead));
	memset(c_PkgID,0x00,sizeof(c_PkgID));
	CPackage::Create_Pkg_Head(1,&m_MsgHead,sizeof(CMD_6011),0x6011,1,sccode,c_PkgID);
	/* --- 包文 --- */
	memset(sendbuf,0x00,sizeof(sendbuf));
	sendlen = CPackage::CreatePackage(0,(char*)sendbuf,&m_MsgHead,(char*)&m_CMD_6011,sizeof(CMD_6011));
	/* --- 发送报文 ---*/
	memset(ackbuf,0x00,sizeof(ackbuf));
	rtn = CCommu::SendANDRecv(CConfig::g_ScList[scIndex].scip,CConfig::g_ScList[scIndex].scport,sendbuf,sendlen,ackbuf,acklen,CConfig::g_Wait_Ack_Time);
	if(rtn == 0)
		return 0;
	return 1;	 
}
	
/* ------------------------------------------------------
            给SC转发参数线程函数
--------------------------------------------------------- */
void* SC::senddatatosle(void* inpara)
{
	ParamStru *m_para=(ParamStru *)inpara;
	int ret,rsplen=0;
	char rspbuf[1000];
	char ipaddr[20];
	int  port;
	char oplogmsg[1024];
	
	/* ----- 获得SCIP和PORT ---*/
	memset(ipaddr,0x00,sizeof(ipaddr));
	strcpy(ipaddr,m_para->ipaddr);
	port = m_para->port;
	
	ret = CCommu::SendANDRecv(ipaddr,m_para->port,(char *)m_para->recv_buf,m_para->buflen,rspbuf,rsplen,CConfig::g_Wait_Ack_Time);
	
	memset(oplogmsg,0x00,sizeof(oplogmsg));
	sprintf(oplogmsg, "0x%04X下发,ip地址:%s,报文内容如下:\n",m_para->paramtype,ipaddr);
	CLog::WriteAppLog(oplogmsg);
	CLog::WriteDebugLog(0,(unsigned char*)m_para->recv_buf,m_para->buflen);	
		 
	
	if(ret == 1)               /* ----- 转发成功 ---*/
	{
		memset(oplogmsg,0x00,sizeof(oplogmsg));
		sprintf(oplogmsg, "0x%04X下发成功,ip地址:%s\n",m_para->paramtype,ipaddr);
		CLog::WriteAppLog(oplogmsg);
	}
	else
	{
		if(ret == 0)
		{
			memset(oplogmsg,0x00,sizeof(oplogmsg));
			sprintf(oplogmsg, "0x%04X下发失败,ip地址:%s,没有启动服务\n",m_para->paramtype,ipaddr);
			CLog::WriteAppLog(oplogmsg);
		}
		else if(ret == -100)
		{
			memset(oplogmsg,0x00,sizeof(oplogmsg));
			sprintf(oplogmsg, "0x%04X下发失败,ip地址:%s,报文过大(%d)\n",m_para->paramtype,ipaddr,m_para->buflen);
			CLog::WriteAppLog(oplogmsg);
		}
		else
		{
			memset(oplogmsg,0x00,sizeof(oplogmsg));
			sprintf(oplogmsg, "0x%04X下发失败,ip地址:%s,应答超时\n",m_para->paramtype,ipaddr);
			CLog::WriteAppLog(oplogmsg);
		}
	}
	
	/* ----- 释放内存 ---*/
	free(m_para->recv_buf);
	m_para->recv_buf = NULL;
	
	int i;
	for(i=0;i<MAX_SLE_COUNT;i++)
	{
		if(strcmp(CConfig::g_ScList[i].scip,ipaddr) == 0)
		{
				CConfig::g_ScList[i].uploadfag = 0;
				break;
		}
	}
	
	return NULL;
}

/* ------------------------------------------------------
            创建转发SC参数线程
            返回值说明：0 正常 1 错误
--------------------------------------------------------- */
int SC::getdatasle(WORD msgtype,WORD paramtype,char *scid,unsigned char * recvbuf,int buflen)
{
	ParamStru	m_para[MAX_SLE_COUNT];
	pthread_t       hThreadHandle;
	int             i,step,count;
	BYTE            sccode[5];
	MsgHead					m_MsgHead;
 
	count = 0;
	if(paramtype == 0x0301 || paramtype == 0x0302)/* ----- 对指定车站下发 ---*/
	{
		for(i=0;i<MAX_SLE_COUNT;i++)
		{
			if( CConfig::g_ScList[i].scport > 0 && (strcmp(CConfig::g_ScList[i].sccode,scid) == 0) && CConfig::g_ScList[i].downloadflag == 1)
			{

				/*-----更改接收方节点--------*/
				memset(sccode,0x00,sizeof(sccode));
				memset(&m_MsgHead,0x00,sizeof(MsgHead));
				m_MsgHead = *(MsgHead *)(recvbuf);
				CommonFuncs::chartobyte(CConfig::g_ScList[i].sccode,(char*)sccode,sizeof(sccode));					
				memcpy(m_MsgHead.b_DestID,sccode,4);
				memcpy(recvbuf,&m_MsgHead,sizeof(MsgHead));		


				unsigned int ul_crc;
				/*--- 生成CRC32校验 --- */
				Crc::Init_CRC32_Table();
				ul_crc = Crc::Crc32((char*)recvbuf,buflen - LEN_OF_PackageTail,0);
				ul_crc = CommonFuncs::dwordtowin(ul_crc);
				memcpy(recvbuf + buflen - LEN_OF_PackageTail,&ul_crc,LEN_OF_PackageTail);
	 
				memset(&m_para[count],0,sizeof(ParamStru));					
				m_para[count].recv_buf = (unsigned char *)malloc(buflen);
				memcpy(m_para[count].recv_buf,recvbuf,buflen);
				strcpy(m_para[count].ipaddr,CConfig::g_ScList[i].scip);
				m_para[count].buflen     = buflen;
				m_para[count].port       = CConfig::g_ScList[i].scport;
				m_para[count].paramtype  = msgtype;
				m_para[count].addinfo[0] = recvbuf[2];
				m_para[count].addinfo[1] = recvbuf[3];
				CConfig::g_ScList[i].uploadfag = 1;
				
				hThreadHandle = 0;
				pthread_create(&hThreadHandle,NULL,senddatatosle,(void *)&m_para[count]);
				pthread_join(hThreadHandle,NULL);
				break;
			}
		}
	}
	else
	{
		for(i=0;i<MAX_SLE_COUNT;i++)
		{
			if( CConfig::g_ScList[i].scport > 0 && CConfig::g_ScList[i].downloadflag == 1)
			{
				/*-----更改发送方接收方节点--------*/
				memset(sccode,0x00,sizeof(sccode));
				memset(&m_MsgHead,0x00,sizeof(MsgHead));
				m_MsgHead = *(MsgHead *)(recvbuf);
				CommonFuncs::chartobyte(CConfig::g_ScList[i].sccode,(char*)sccode,sizeof(sccode));					
				memcpy(m_MsgHead.b_DestID,sccode,4);
				memcpy(m_MsgHead.b_SourceID,CConfig::g_CCIDCode,4);
				memcpy(recvbuf,&m_MsgHead,sizeof(MsgHead));		
				
				unsigned int ul_crc;
				/*--- 生成CRC32校验 --- */
				Crc::Init_CRC32_Table();
				ul_crc = Crc::Crc32((char*)recvbuf,buflen - LEN_OF_PackageTail,0);
				ul_crc = CommonFuncs::dwordtowin(ul_crc);
				memcpy(recvbuf + buflen - LEN_OF_PackageTail,&ul_crc,LEN_OF_PackageTail);
				
				memset(&m_para[count],0,sizeof(ParamStru));
				m_para[count].recv_buf = (unsigned char *)malloc(buflen);
				memcpy(m_para[count].recv_buf,recvbuf,buflen);
				strcpy(m_para[count].ipaddr,CConfig::g_ScList[i].scip);
				m_para[count].buflen     = buflen;
				m_para[count].port       = CConfig::g_ScList[i].scport;
				m_para[count].paramtype  = msgtype;
				m_para[count].addinfo[0] = recvbuf[2];
				m_para[count].addinfo[1] = recvbuf[3];
				CConfig::g_ScList[i].uploadfag = 1;
				
				count++;
			}
		}
		
		
		
		for(i=0;i<count;i++)
		{
			hThreadHandle = 0;
			pthread_create(&hThreadHandle,NULL,senddatatosle,(void *)&m_para[i]);
			pthread_join(hThreadHandle,NULL);
		}
	}
	
	return 0;
} 

 
 
/* ------------------------------------------------------
函数功能：得到输入站点的所在下标(scid取前四位)
--------------------------------------------------------- */
int SC::getscinfo(char *sccodeOrScip)
{
	int i,index;

	index = -1;
	for(i=0;i<MAX_SLE_COUNT;i++)
	{
		if (strncmp(CConfig::g_ScList[i].sccode,sccodeOrScip,4) == 0)
		{
			index = i;
			break;
		}
		
		if (strcmp(CConfig::g_ScList[i].scip,sccodeOrScip) == 0)
		{
			index = i;
			break;
		}
	}
	return index;
}

 





 
sem_t  			CPara::g_CSFTP;
int  				CPara::g_Ftp_Inform;
 
void CPara::InitCParaStaticVar()
{
	sem_init(&CPara::g_CSFTP,0,1);
		g_Ftp_Inform = 0;
}	

void CPara::ReleaseCParaStaticVar()
{
	sem_destroy(&CPara::g_CSFTP);
}


/* ------------------------------------------------------
            功能描述:   生成参数文件
--------------------------------------------------------- */
void CPara::CreateParamFile()
{
	int i;
	char c_id[9];
	
	/* --- 生成0301参数文件 ---*/
	for(i=0;i<MAX_SLE_COUNT;i++)
	{
		if(CConfig::g_ScList[i].scport > 0)
		{
			memset(c_id,0x00,sizeof(c_id));
			strcpy(c_id,CConfig::g_ScList[i].sccode);
			sem_wait(&ORA::ms_CSSQL);
			ORA::create0301param(c_id,NULL);
			sem_post(&ORA::ms_CSSQL);
		}
	}
	/* --- 生成0302参数文件 ---*/
	for(i=0;i<MAX_SLE_COUNT;i++)
	{
		if(CConfig::g_ScList[i].scport > 0)
		{
			memset(c_id,0x00,sizeof(c_id));
			strcpy(c_id,CConfig::g_ScList[i].sccode);
			sem_wait(&ORA::ms_CSSQL);
			ORA::create0302param(c_id,NULL);
			sem_post(&ORA::ms_CSSQL);
		}
	}
	/* --- 生成0303参数文件 ---*/
	sem_wait(&ORA::ms_CSSQL);
	ORA::create0303param(NULL,NULL);
	sem_post(&ORA::ms_CSSQL);
	/* --- 生成0304参数文件 ---*/
	sem_wait(&ORA::ms_CSSQL);
	ORA::create0304param(NULL,NULL);
	sem_post(&ORA::ms_CSSQL);
	/* --- 生成0305参数文件 ---*/
	sem_wait(&ORA::ms_CSSQL);
	ORA::create0305param(NULL,NULL);
	sem_post(&ORA::ms_CSSQL);
	/* --- 生成0306参数文件 ---*/
	sem_wait(&ORA::ms_CSSQL);
	ORA::create0306param(NULL,NULL);
	sem_post(&ORA::ms_CSSQL);
	/* --- 生成0307参数文件 ---*/
	sem_wait(&ORA::ms_CSSQL);
	ORA::create0307param(NULL,NULL);
	sem_post(&ORA::ms_CSSQL);
	/* --- 生成0501参数文件 ---*/
	sem_wait(&ORA::ms_CSSQL);
	ORA::create0501param(NULL,NULL);
	sem_post(&ORA::ms_CSSQL);
	/* --- 生成0502参数文件 ---*/
	sem_wait(&ORA::ms_CSSQL);
	ORA::create0502param(NULL,NULL);
	sem_post(&ORA::ms_CSSQL);
	/* --- 生成0503参数文件 ---*/
	sem_wait(&ORA::ms_CSSQL);
	ORA::create0503param(NULL,NULL);
	sem_post(&ORA::ms_CSSQL);
		
}

/* ------------------------------------------------------
            功能描述:   定时生效参数线程
--------------------------------------------------------- */
void* CPara::ThreadValidateParam(void* lpParam)
{
	pthread_detach(pthread_self());

	int 	i;
	char 	c_id[9];
	
	while(1)
	{
		sleep(CConfig::ParamValidTime);
		
		for(i=0;i<MAX_SLE_COUNT;i++)
		{
			if(CConfig::g_ScList[i].scport > 0)
			{
				memset(c_id,0x00,sizeof(c_id));
				strcpy(c_id,CConfig::g_ScList[i].sccode);
				sem_wait(&ORA::ms_CSSQL);
				ORA::Validate_Single_Param("0301",c_id);
				sem_post(&ORA::ms_CSSQL);
			}
		}
		for(i=0;i<MAX_SLE_COUNT;i++)
		{
			if(CConfig::g_ScList[i].scport > 0)
			{
				memset(c_id,0x00,sizeof(c_id));
				strcpy(c_id,CConfig::g_ScList[i].sccode);
				sem_wait(&ORA::ms_CSSQL);
				ORA::Validate_Single_Param("0302",c_id);
				sem_post(&ORA::ms_CSSQL);
			}
		}
		sem_wait(&ORA::ms_CSSQL);
		ORA::Validate_Group_Param();
		sem_post(&ORA::ms_CSSQL);
	}
	return NULL;
}

/* ------------------------------------------------------
功能描述:   参数文件复制
--------------------------------------------------------- */
void CPara::Copy_ParamFile(char *oldpath,char *oldname,const char *newname,const char *newpath)
{
	char 				newfilename[256];
	char				oldfilename[256];
	char				*filebuffer;
	int					filelen;
	struct stat buf;
		
	/* --- 读取文件内容 ------- */
	memset(oldfilename,0x00,sizeof(oldfilename));
	sprintf(oldfilename,"%s%s",oldpath,oldname);
	filelen = stat(oldfilename, &buf );
	if(filelen != 0)
	{
		return;
	}
	filelen     = buf.st_size;
	filebuffer = (char*)malloc(filelen);
	CommonFuncs::ReadFileBuffer(oldfilename,filebuffer);
	/* --- 复制文件 ------- */
	memset(newfilename,0x00,sizeof(newfilename));
	sprintf(newfilename,"%s%s",newpath,newname);
	CommonFuncs::WriteBufferFile("wb",newfilename,filebuffer,filelen);
	/* --- 释放内存 ------- */
	free(filebuffer);
	filebuffer = NULL;
}
 
 
/* ------------------------------------------------------
功能描述:   参数文件重新命名
--------------------------------------------------------- */
void CPara::Rename_ParamFile(int flag,char *oldpath,char *name,char *newpath)
{
	char 				newfilename[256];
	char        oldfilename[256];

	memset(oldfilename,0x00,sizeof(oldfilename));
	sprintf(oldfilename,"%s%s",oldpath,name);
	memset(newfilename,0x00,sizeof(newfilename));
	sprintf(newfilename,"%s%s",newpath,name);
	rename(oldfilename,newfilename);
}
 



/* ------------------------------------------------------
         功能描述:   判断参数文件格式
--------------------------------------------------------- */
int CPara::Formate_ParamFile(char *path,char *name)
{
	char 							logdata[1024];
	char 							filename[256];
	char              ParamPath[256];
	struct stat       buf;
	unsigned int      filelen;
	unsigned short    ParamType;
	MsgHead           m_MsgHead;
	int               body_len;
	char              *Package;
	int               rtn;
	
	rtn = 1;
	
	/* --- 读取文件内容 ------- */
	memset(ParamPath,0x00,sizeof(ParamPath));
	sprintf(ParamPath,"%s%s",path,name);
	stat(ParamPath,&buf);
	filelen = buf.st_size;
	Package = (char*)malloc(filelen);
	memset(Package,0x00,filelen);
	CommonFuncs::ReadFileBuffer(ParamPath,Package);
	/* --- 读取参数内容 ------- */
	memset(&m_MsgHead,0x00,sizeof(MsgHead));
	m_MsgHead = *(MsgHead *)(Package);
	body_len = CommonFuncs::dwordtowin(m_MsgHead.dw_PkgLen);
	if(filelen != sizeof(MsgHead) + body_len + LEN_OF_PackageTail)
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"参数文件:%s格式错误\n",name);
		CLog::WriteAppLog(logdata);
				
		memset(filename,0x00,sizeof(filename));
		sprintf(filename,"%s%s",CConfig::ErrorPath,name);
		rename(ParamPath,filename);
		
		rtn = 0;
	}
	
	/* --- 释放内存 ---- */
	free(Package);
	Package = NULL;
	
	return rtn;
}

/* ------------------------------------------------------
   功能描述: 获得参数版本
   返回值说明：获取失败 0;获取成功 返回1
--------------------------------------------------------- */
int  CPara::GetParamVer(const char *scid,WORD type,char *dir,char *ver)
{
	
	 	char logdata[1024];
	 	
	 	
	struct dirent *dirp;
	DIR           *dp;
	char           filename[256];
	char           subStr[100];

	int isFind = 0;
	memset(subStr,0x00,sizeof(subStr));
	if(scid == NULL)
	{
		sprintf(subStr,"%04X_",type);
	}
	else
	{
		sprintf(subStr,"%04X_%s_",type,scid);
	}
 
	/* --- 打开目录 --- */
	if((dp = opendir(dir)) == NULL)
		return 0;
	/* --- 读取目录 --- */
	memset(filename,0x00,sizeof(filename));
	while((dirp = readdir(dp)) != NULL)
	{
		if(strcmp(dirp->d_name,".") == 0 ||
			strcmp(dirp->d_name,"..") == 0)
			continue;
		if(strstr(dirp->d_name,subStr) != NULL)
		{
			memset(filename,0x00,sizeof(filename));
			sprintf(filename,"%s%s",dir,dirp->d_name);
			
			isFind = 1;
			break;
		}			
	}
	/* -------- 关闭目录 ---------- */
	closedir(dp);
 
 	if(isFind == 0)
 	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"substr:(%s)在目录(%s)未找到对应的文件\n",subStr,dir);
		CLog::WriteAppLog(logdata);
		
		return 0;
 	}
 
	FILE		*fp;
	char		buffer[100];
	ParaHead	m_ParaHead;
	MsgHead		m_MsgHead;
	int     i;
 
	/* --- 打开参数文件 ------- */
	fp = fopen(filename,"rb");
	if (fp == NULL)
	{
		return 0;
	}
	
	if(type == 0x0019 || type == 0x001A)
	{
		/* --- 获得参数版本 ------- */
		fseek(fp,0L,SEEK_SET);
		memset(buffer,0x00,sizeof(buffer));
		fread(buffer,sizeof(char),sizeof(MsgHead),fp);
		fclose(fp);
		
		m_MsgHead = *(MsgHead *)buffer;
		for(i=0;i<7;i++)
			sprintf(ver+2*i,"%02X",m_MsgHead.b_PkgID[i+4]);
	}
	else
	{
		/* --- 获得参数版本 ------- */
		fseek(fp,0L,SEEK_SET);
		memset(buffer,0x00,sizeof(buffer));
		fread(buffer,sizeof(char),sizeof(MsgHead)+sizeof(ParaHead),fp);
		fclose(fp);
	
		m_ParaHead = *(ParaHead *)(buffer+sizeof(MsgHead));
		for(i=0;i<sizeof(m_ParaHead.ParamVer);i++)
			sprintf(ver+2*i,"%02X",m_ParaHead.ParamVer[i]);
	}

	return 1;
}


/* ------------------------------------------------------
            功能描述:   除主控参数外其它参数的下载、入库
            返回值说明: 1 处理成功 0;处理失败
            HHJT_CJP 20210323 
--------------------------------------------------------- */
int CPara::DealParam(unsigned short wMsgCode,char *fileName,int lineIndex,char *fileExistPath)
{
	char fileFullName[256],logdata[100];
	int  rtn,rtn_insert_db;

	char 			lcParaPath[MAX_PATH];
	memset(lcParaPath,0x00,sizeof(lcParaPath));
	sprintf(lcParaPath, "%slc%d/",CConfig::g_CurParamPath,CConfig::ms_MlcLineList[lineIndex].LINE_ID);
	
	/* --- 判断文件格式 --- */
	rtn = Formate_ParamFile(fileExistPath,fileName);

	if(rtn != 1) 
		return 0;
		
	/* --- 入库 ---*/
	rtn_insert_db = 1;
	memset(fileFullName,0x00,sizeof(fileFullName));
	sprintf(fileFullName,"%s%s",fileExistPath,fileName);
	switch(wMsgCode)
	{
		case 0x0019:
			sem_wait(&ORA::ms_CSSQL);
			rtn_insert_db = ORA::insert_into_0019(fileFullName,lineIndex);
			sem_post(&ORA::ms_CSSQL);
			break;
		case 0x001A:
			sem_wait(&ORA::ms_CSSQL);
			rtn_insert_db = ORA::insert_into_001A(fileFullName,lineIndex);
			sem_post(&ORA::ms_CSSQL);
			break;
		case 0x0018:
			sem_wait(&ORA::ms_CSSQL);
			rtn_insert_db = ORA::insert_into_0018(fileFullName,lineIndex);
			sem_post(&ORA::ms_CSSQL);
			break;
		case 0x000B:
			sem_wait(&ORA::ms_CSSQL);
			rtn_insert_db = ORA::insert_into_000B(fileFullName,lineIndex);
			sem_post(&ORA::ms_CSSQL);
			break;
		case 0x000C:
			sem_wait(&ORA::ms_CSSQL);
			rtn_insert_db = ORA::insert_into_000C(fileFullName,lineIndex);
			sem_post(&ORA::ms_CSSQL);
			break;
		case 0x000D:
			sem_wait(&ORA::ms_CSSQL);
			rtn_insert_db = ORA::insert_into_000D(fileFullName,lineIndex);
			sem_post(&ORA::ms_CSSQL);
			break;
		case 0x000E:
			sem_wait(&ORA::ms_CSSQL);
			rtn_insert_db = ORA::insert_into_000E(fileFullName,lineIndex);
			sem_post(&ORA::ms_CSSQL);
			break;
		case 0x000F:
			sem_wait(&ORA::ms_CSSQL);
			rtn_insert_db = ORA::insert_into_000F(fileFullName,lineIndex);
			sem_post(&ORA::ms_CSSQL);
			break;
		case 0x0401:
			sem_wait(&ORA::ms_CSSQL);
			rtn_insert_db = ORA::insert_into_0401(fileFullName,lineIndex);
			sem_post(&ORA::ms_CSSQL);
			break;
		case 0x0402:
			sem_wait(&ORA::ms_CSSQL);
			rtn_insert_db = ORA::insert_into_0402(fileFullName,lineIndex);
			sem_post(&ORA::ms_CSSQL);
			break;
		case 0x0403:
			sem_wait(&ORA::ms_CSSQL);
			rtn_insert_db = ORA::insert_into_0403(fileFullName,lineIndex);
			sem_post(&ORA::ms_CSSQL);
			break;
		case 0x0406:
			sem_wait(&ORA::ms_CSSQL);
			rtn_insert_db = ORA::insert_into_0406(fileFullName,lineIndex);
			sem_post(&ORA::ms_CSSQL);
			break;
		default:
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata, "参数类型0x%04X系统未定义\n",wMsgCode);
			CLog::WriteAppLog(logdata);
			rtn_insert_db = 0;
			break;
	}	
		
	/* --- 入库成功 ---*/
	if(rtn_insert_db == 1)
	{
		/* --- 删除当前参数目录文件 ------- */	
		char strTemp[100];
		memset(strTemp,0x00,sizeof(strTemp));
		sprintf(strTemp,"%04X_",wMsgCode);
		CommonFuncs::DeleteFile(lcParaPath,strTemp);
				
		/* --- 移动文件 ------- */
		CPara::Rename_ParamFile(0,fileExistPath,fileName,lcParaPath);
		
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata, "参数0x%04X(%s)入库完成\n",wMsgCode,fileFullName);
		CLog::WriteAppLog(logdata);
	}
	else
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata, "参数0x%04X(%s)入库失败\n",wMsgCode,fileFullName);
		CLog::WriteAppLog(logdata);

		remove(fileFullName);
	}	
 
	return rtn_insert_db;
}

			

 
 



/* ------------------------------------------------------
            功能描述:   删除错误数据
--------------------------------------------------------- */
void CThread::DelErrorData()
{
	int           ret;
	struct stat   buf;
	struct dirent *dirp;
	DIR           *dp;
	char          filename[MAX_PATH];
	time_t        local_time;
  
  ret = 1;
  	/* -------- 打开错误数据目录 ---------- */
	if((dp = opendir(CConfig::ErrorPath)) == NULL)
	{
		return;
	}
	/* -------- 读取错误数据目录 ---------- */
	while((dirp = readdir(dp)) != NULL)
	{
		if(strcmp(dirp->d_name,".") == 0 ||
		   strcmp(dirp->d_name,"..") == 0)
		   continue;
		
		/* -------- 读取错误数据文件 ---------- */
		memset(filename,0x00,sizeof(filename));
		strcpy(filename,CConfig::ErrorPath);
		strcat(filename,dirp->d_name);
		
		time(&local_time);
		stat(filename,&buf );
		if( (local_time - buf.st_ctime > 2) && 
		    (strstr(dirp->d_name,"dat3") == NULL))
		{
			/* -------- 删除错误数据文件 ---------- */
  			remove(filename);
  	}
	}
	/* -------- 关闭错误数据目录 ---------- */
  closedir(dp);
}

/* ------------------------------------------------------
            功能描述:   删除上月同天的历史数据
--------------------------------------------------------- */
void CThread::DelData(char *delpath,int curmonth,int curday)
{
	time_t         filetime;
	struct tm      strtm;
	struct stat    buf;
	char           delname[MAX_PATH];
	struct dirent  *dirp;
	DIR            *dp;

  /* -------- 打开待删除数据目录 ---------- */
	if((dp = opendir(delpath)) == NULL)
	{
		return;
	}

	/* --------- 删除目录里非当天的数据 ----------------*/	
	while((dirp = readdir(dp)) != NULL)
	{
		if(strcmp(dirp->d_name,".") == 0 ||
		   strcmp(dirp->d_name,"..") == 0)
		   continue;
		
		/* -------- 读取数据文件 ---------- */
		memset(delname,0x00,sizeof(delname));
		strcpy(delname,delpath);
		strcat(delname,dirp->d_name);
			
		stat(delname,&buf );
		filetime = buf.st_ctime;
		memset(&strtm,0x00,sizeof(struct tm));
		localtime_r(&filetime,&strtm);
		if(curmonth != strtm.tm_mon || curday != strtm.tm_mday)
		{
			remove(delname);
  	}   
	}
	/* -------- 关闭错误数据目录 ---------- */
  closedir(dp);	
}

/* ------------------------------------------------------
            功能描述:   每天2点至3点删除上周同天的历史数据
                        每周一删除错误数据
--------------------------------------------------------- */
void CThread::DelHistoryData()
{
	time_t         local_time;
	struct tm      strtm;
	int            curhour,curmonth,curday;
	char           delpath1[MAX_PATH],delpath2[MAX_PATH],delpath3[MAX_PATH];
	int            i;
	char           c_id[9];
	char           errormsg[1024];
	
	time(&local_time);
	memset(&strtm,0x00,sizeof(struct tm));
	localtime_r(&local_time,&strtm);
	curhour = strtm.tm_hour;
	if(curhour < 2 || curhour >= 3)
	{
		return;
	}
	/* ----- 日切 ------*/
	memset(errormsg,0x00,sizeof(errormsg));
	strcpy(errormsg,"日切开始\n");
	CLog::WriteAppLog(errormsg);
	sem_wait(&ORA::ms_CSSQL);
	/* --- 生成0302参数文件 ---*/
	for(i=0;i<MAX_SLE_COUNT;i++)
	{
		if(CConfig::g_ScList[i].scport > 0)
		{
			memset(c_id,0x00,sizeof(c_id));
			strcpy(c_id,CConfig::g_ScList[i].sccode);
			ORA::create0302param(c_id,"FFFFFFFF");
		}
	}		
	sem_post(&ORA::ms_CSSQL);	
		
	ORA::exec_batch();
	ORA::del_his_data();
 
	/*----------------分线路生成文件-----------------------*/
 	for(int i = 0;i < MAX_SLE_COUNT;i++)
	{
		if(CConfig::ms_MlcLineList[i].LINE_ID > 0)
		{
			/* --- 生成设备状态信息文件 ---*/
			ORA::create7001file(CConfig::ms_MlcLineList[i].LINE_ID);
			/* ----- 生成库存盘点文件------*/
			ORA::create3003file(CConfig::ms_MlcLineList[i].LINE_ID);
		}
	}

	memset(errormsg,0x00,sizeof(errormsg));
	strcpy(errormsg,"日切结束\n");
	CLog::WriteAppLog(errormsg);
	/* ----- 获得删除的历史数据的目录 ------*/
	curmonth = strtm.tm_mon;
	curday = strtm.tm_mday;
	memset(delpath1,0x00,sizeof(delpath1));
	sprintf(delpath1,"%s%02d/",CConfig::HisDataPath_LCC,curday);
	memset(delpath2,0x00,sizeof(delpath2));
	sprintf(delpath2,"%s%02d/",CConfig::HisDataPath_ACC,curday);
	memset(delpath3,0x00,sizeof(delpath3));
	sprintf(delpath3,"%s%02d/",CDir::m_AppLogPath,curday);
	/* ----- 每月1号清除error目录 ------*/
	if(curday == 1)
	{
		DelErrorData();
	}

	/* --------- 删除历史数据 -----*/
	DelData(delpath1,curmonth,curday);	
	DelData(delpath2,curmonth,curday);	
	DelData(delpath3,curmonth,curday);	
 
}

 
/* ------------------------------------------------------
      功能描述:   删除历史数据线程函数
--------------------------------------------------------- */
void* CThread::ThreadDelHistoryData(void* lpParam)
{
	pthread_detach(pthread_self());
	while(1)
	{
		sleep(3600);
		DelHistoryData();
	}
	return NULL;
}

/* ------------------------------------------------------
            功能描述:   读取待入库的交易数据
            返回值说明：数据库异常返回0，其他返回1
--------------------------------------------------------- */
int CThread::PrepareInsertTransData(char *filepath,WORD in_msgtype)
{
	int           ret;
	struct dirent *dirp;
	DIR           *dp;
	char          insertpath[MAX_PATH];
	time_t        local_time;
	char          errormsg[500];
	char          c_msgtype[5];
	int           msgtype;
	char          *stopstring;
	struct stat   buf;
  
  ret = 1;
  	/* -------- 打开待入库的交易数据目录 ---------- */
	if((dp = opendir(filepath)) == NULL)
	{
		return ret;
	}
	/* -------- 读取待入库的交易数据目录 ---------- */
	while((dirp = readdir(dp)) != NULL)
	{
		if(strcmp(dirp->d_name,".") == 0 ||
		   strcmp(dirp->d_name,"..") == 0)
		   continue;
		
		memset(insertpath,0x00,sizeof(insertpath));
		strcpy(insertpath,filepath);
		strcat(insertpath,dirp->d_name);
		/* --- 获得报文类型 --- */
		memset(c_msgtype,0x00,sizeof(c_msgtype));
		memcpy(c_msgtype,dirp->d_name+3,4);
		msgtype = strtoul(c_msgtype,&stopstring,16);
		/* -------- 删除非正常类型报文数据 ---------- */
		if(msgtype != 0x1001 && msgtype != 0x1002 && msgtype != 0x1003 &&
			 msgtype != 0x1004 && msgtype != 0x1005 && msgtype != 0x1006)
		{
			memset(errormsg,0x00,sizeof(errormsg));
			sprintf(errormsg,"文件格式错误:%s(PrepareInsertTransData)\n",insertpath);
			CLog::WriteAppLog(errormsg);
			
			remove(insertpath);
			continue;
		}
		
		if(in_msgtype != msgtype)
		{
			continue;
		}
		
		/* --- 系统时间 --- */
		time(&local_time);
		stat(insertpath,&buf);
		/* -------- 入库 ---------- */
		if(local_time - buf.st_mtime > 2)
		{
			ret = InsertTransData(insertpath,dirp->d_name,msgtype);
			if(ret == 0)/* -------- 数据库异常 ---------- */
			{
				break;
			}
			if(ret == 2)/* -------- 读取文件失败 ---------- */
			{
				continue;
			}		
			/* -------- 入库结束后的文件直接删除---------- */
			remove(insertpath);
	  }
	}
	/* -------- 关闭待入库的交易数据目录 ---------- */
  closedir(dp);
  	
  return ret;
}

/* ------------------------------------------------------
            功能描述:   交易数据报文入库
            返回值说明：数据库异常返回0，其他返回1
--------------------------------------------------------- */
int CThread::InsertTransData(char *filename,char *name,WORD msgtype)
{
	int             ret,result;
	char            errormsg[500];
	char            *buffer;
	struct stat     buf;
	int             filelen;

	ret = 1;
	/* -------- 获得文件大小 ---------- */
	stat(filename,&buf);
	filelen = buf.st_size;
	if(filelen == 0)
	{
			memset(errormsg,0x00,sizeof(errormsg));
			sprintf(errormsg,"InsertTransData:file %s 为空文件\n",filename);
			CLog::WriteAppLog(errormsg);
			
			remove(filename);
			return 2;
	}
	
	memset(errormsg,0x00,sizeof(errormsg));
	sprintf(errormsg,"InsertTransData---%s---处理开始---\n",filename);
	CLog::WriteAppLog(errormsg);	
		
	/* -------- 读取文件 ---------- */
	buffer = (char*)malloc(filelen);
	memset(buffer,0x00,filelen);
	CommonFuncs::ReadFileBuffer(filename,buffer);
	/* -------- 文件入库 ---------- */
	result = 0;       
	switch(msgtype)
	{
		case 0x1001:
			result = ORA::insert_into_1001(name,msgtype,(unsigned char*)buffer,filelen);
			break;
		case 0x1002:
			result = ORA::insert_into_1002(name,msgtype,(unsigned char*)buffer,filelen);
			break;	
		case 0x1003:
			result = ORA::insert_into_1003(name,msgtype,(unsigned char*)buffer,filelen);
			break;	
		case 0x1004:
			result = ORA::insert_into_1004(name,msgtype,(unsigned char*)buffer,filelen);
			break;	
		case 0x1005:
			result = ORA::insert_into_1005(name,msgtype,(unsigned char*)buffer,filelen);
			break;	
		case 0x1006:
			result = ORA::insert_into_1006(name,msgtype,(unsigned char*)buffer,filelen);
			break;
	}
	
	/* -------- 数据库异常,停止应用 ---------- */
	if(result == -1 && CMainManage::ms_CommExitFlag == 0)
	{
		ret = 0;
		memset(errormsg,0x00,sizeof(errormsg));
		strcpy(errormsg,"InsertTransData error,db error\n");
		CLog::WriteAppLog(errormsg);
			
		/* --- 释放内存 --- */
		free(buffer);
		buffer = NULL;
				
		CMainManage::RestartProgram(NULL);
	}
	else
	{
		;
	}
	/* --- 释放内存 --- */
	free(buffer);
	buffer = NULL;
		
	memset(errormsg,0x00,sizeof(errormsg));
	sprintf(errormsg,"InsertTransData---%s---处理结束---\n",filename);
	CLog::WriteAppLog(errormsg);	
		
	return ret;
}


/* ------------------------------------------------------
      功能描述:   交易数据文件入库线程函数
--------------------------------------------------------- */
void* CThread::ThreadInsertTransData_1001(void* lpParam)
{
	pthread_detach(pthread_self());
	while(1)
	{
		CommonFuncs::my_sleep(1);
		PrepareInsertTransData(CConfig::InsertTransFilePath,0x1001)	;		 
	}
	return NULL;
}

 
/* ------------------------------------------------------
      功能描述:   交易数据文件入库线程函数
--------------------------------------------------------- */
void* CThread::ThreadInsertTransData_1002(void* lpParam)
{
	pthread_detach(pthread_self());
	while(1)
	{
		CommonFuncs::my_sleep(1);
		PrepareInsertTransData(CConfig::InsertTransFilePath,0x1002);			 
	}
	return NULL;
}

/* ------------------------------------------------------
      功能描述:   交易数据文件入库线程函数
--------------------------------------------------------- */
void* CThread::ThreadInsertTransData_1003(void* lpParam)
{
	pthread_detach(pthread_self());
	while(1)
	{
		CommonFuncs::my_sleep(1);
		PrepareInsertTransData(CConfig::InsertTransFilePath,0x1003);		 
	}
	return NULL;
}

/* ------------------------------------------------------
      功能描述:   交易数据文件入库线程函数
--------------------------------------------------------- */
void* CThread::ThreadInsertTransData_1004(void* lpParam)
{
	pthread_detach(pthread_self());
	while(1)
	{
		CommonFuncs::my_sleep(1);
		PrepareInsertTransData(CConfig::InsertTransFilePath,0x1004);			 
	}
	return NULL;
}

/* ------------------------------------------------------
      功能描述:   交易数据文件入库线程函数
--------------------------------------------------------- */
void* CThread::ThreadInsertTransData_1005(void* lpParam)
{
	pthread_detach(pthread_self());
	while(1)
	{
		CommonFuncs::my_sleep(1);
		PrepareInsertTransData(CConfig::InsertTransFilePath,0x1005);		 
	}
	return NULL;
}
/* ------------------------------------------------------
      功能描述:   交易数据文件入库线程函数
--------------------------------------------------------- */
void* CThread::ThreadInsertTransData_1006(void* lpParam)
{
	pthread_detach(pthread_self());
	while(1)
	{
		CommonFuncs::my_sleep(1);
		PrepareInsertTransData(CConfig::InsertTransFilePath,0x1006);		 
	}
	return NULL;
}
/* ------------------------------------------------------
            功能描述:   读取待入库的状态寄存器数据
            返回值说明：数据库异常返回0，其他返回1
--------------------------------------------------------- */
int CThread::PrepareInsertStatusData(char *filepath,WORD in_msgtype)
{
	int           ret;
	struct stat   buf;
	struct dirent *dirp;
	DIR           *dp;
	char          insertpath[MAX_PATH];
	time_t        local_time;
	struct tm     newtime;
	int           mday;
	char          errormsg[500];
	char          c_msgtype[5];
	int           msgtype;
	char          *stopstring; 
	 
  ret = 1;
  
  	/* -------- 打开待入库的状态寄存器数据目录 ---------- */
	if((dp = opendir(filepath)) == NULL)
	{
		return ret;
	}
  	/* -------- 读取待入库的状态寄存器数据目录 ---------- */
	while((dirp = readdir(dp)) != NULL)
	{
		if(strcmp(dirp->d_name,".") == 0 ||
		   strcmp(dirp->d_name,"..") == 0)
		   continue;
				
		/* --- 获得文件全路径 --- */
		memset(insertpath,0x00,sizeof(insertpath));
		strcpy(insertpath,filepath);
		strcat(insertpath,dirp->d_name);
				
		/* --- 获得报文类型 --- */
		memset(c_msgtype,0x00,sizeof(c_msgtype));
		memcpy(c_msgtype,dirp->d_name,4);
		msgtype = strtoul(c_msgtype,&stopstring,16);		

		/* -------- 删除非正常类型报文数据 ---------- */
		if(msgtype != 0x4001 && msgtype != 0x4002 && msgtype != 0x4003 &&
			 msgtype != 0xF001 && msgtype != 0xF002 && msgtype != 0xF003 &&
			 msgtype != 0xF004 && msgtype != 0xF005 && msgtype != 0xF006 &&
			 msgtype != 0x3001 && msgtype != 0x3003 && msgtype != 0x3005 &&
			 msgtype != 0x3006 && msgtype != 0x3007 && msgtype != 0x3008 &&			 
			 msgtype != 0x5001 && msgtype != 0x5002 && msgtype != 0x5003 &&
			 msgtype != 0x5004 && msgtype != 0x5005 && msgtype != 0x5010 &&			 
			 msgtype != 0x5020 && msgtype != 0x5021 && msgtype != 0x5022 &&			
			 msgtype != 0x7001 && msgtype != 0x7002 && msgtype != 0x7010 
			 )
		{
			memset(errormsg,0x00,sizeof(errormsg));
			sprintf(errormsg,"文件类型错误:%s(PrepareInsertStatusData)\n",insertpath);
			CLog::WriteAppLog(errormsg);
			
			remove(insertpath);
			continue;
		}	
		
		if(in_msgtype != msgtype)
		{
			if(in_msgtype != 0x00)
			{
				continue;
			}
			else
			{
				if(msgtype == 0x5001 || msgtype == 0x5002 || msgtype == 0x5003 || msgtype == 0x7010)
				{
					continue;
				}
			}			
		}
 
		/* -------- 入库 ---------- */
		time(&local_time);
		stat(insertpath,&buf );
		if(local_time - buf.st_mtime > 2)
		{
			
			if(local_time - buf.st_mtime > 300)/* --- 超过5分钟的状态报文不入库直接删除 --- */
			{
				if(msgtype == 0x7010)
				{
					memset(errormsg,0x00,sizeof(errormsg));
					sprintf(errormsg,"状态文件超时:%s(PrepareInsertStatusData)\n",insertpath);
					CLog::WriteAppLog(errormsg);
					remove(insertpath);
					continue;
				}
			}
 
			if(msgtype == 0x7010)
			{
				ret = InsertStatusData(insertpath,dirp->d_name);
			}
			else
			{

				if(local_time - buf.st_ctime > 65)
				{
					ret = InsertStatusData(insertpath,dirp->d_name);
				}
				else
				{
					continue;
				}
			}
			if(ret == 0)/* -------- 数据库异常 ---------- */
			{
				break;
			}
			if(ret == 2)/* -------- 读取文件失败 ---------- */
			{
				continue;
			}
			/* -------- 入库结束后的文件移至历史数据目录（不保存状态数据） ---------- 
			if(msgtype == 0x7010 || msgtype == 0x7001)
			{
				remove(insertpath);			
			}
			else
			{
				memset(&newtime,0x00,sizeof(struct tm));
				localtime_r(&buf.st_ctime,&newtime);
				mday = newtime.tm_mday;

				COtherFuncs::SaveHistoryData(1,insertpath,dirp->d_name,mday);					 
			}*/
			/* -------- 入库结束后的文件直接删除----------*/ 
			remove(insertpath);
	  }
	  else
	  {
	  	;
	  }						
	}
	/* -------- 关闭待入库的状态寄存器数据目录 ---------- */
  closedir(dp);
  	
  return ret;
}

/* ------------------------------------------------------
            功能描述:   状态寄存器数据报文入库
            返回值说明：数据库异常返回0，其他返回1
--------------------------------------------------------- */
int CThread::InsertStatusData(char *filename,char *name)
{
	int             ret;
	char            errormsg[500];
	BYTE            buffer[MAX_BAOWEN_LENGTH];
	FILE            *fp;
	int             count,readcount,result,body_tail_len,bodylen;
	unsigned short  msgtype;
	char            sendpathname[MAX_PATH];
	MsgHead 			  m_msghead;

	ret = 1;
	/* -------- 打开交易数据文件 ---------- */
	fp = fopen(filename,"rb");
	if(fp == NULL)
	{
		memset(errormsg,0x00,sizeof(errormsg));
		sprintf(errormsg,"InsertStatusData:file %s open error\n",filename);
		CLog::WriteAppLog(errormsg);

		return 2;
	}

	memset(errormsg,0x00,sizeof(errormsg));
	sprintf(errormsg,"InsertStatusData---%s---处理开始---\n",filename);
	CLog::WriteAppLog(errormsg);
			
	/* -------- 获得文件大小 ---------- */
	struct stat   buf;
	int           filelen,FileCount;
	stat(filename, &buf );
	filelen = buf.st_size;
	FileCount = 0;
	
	int records = 0;
	/* -------- 读取整个交易数据文件 ---------- */
	while(!feof(fp) && FileCount < filelen)
	{
		/* -------- 包头 ---------- */
		readcount = 0;
		memset(buffer,0x00,sizeof(buffer));
		readcount = fread(buffer,sizeof(char),sizeof(MsgHead),fp);
       	        if((ferror(fp))&& (!feof(fp)))     
		{
			memset(errormsg,0x00,sizeof(errormsg));
			sprintf(errormsg,"InsertStatusData:file %s read error\n",filename);
			CLog::WriteAppLog(errormsg);

			fclose(fp);
			return 2;
		}
		/* -------- 包大小 ---------- */
		memset(&m_msghead,0x00,sizeof(MsgHead));
		m_msghead = *(MsgHead *)(buffer);
		bodylen = CommonFuncs::dwordtowin(m_msghead.dw_PkgLen);
		body_tail_len = bodylen + LEN_OF_PackageTail;
		/* -------- 获得报文类型 ---------- */
		msgtype = CommonFuncs::wordtowin(m_msghead.w_MsgCode); 
		
		count = 0;
		/* -------- 报文大小超过规定大小 ---------- */
		if((sizeof(MsgHead)+body_tail_len) > 8*1024)
		{
			BYTE *tmpBuf;
			tmpBuf = (BYTE*)malloc(sizeof(MsgHead)+body_tail_len);
			memset(tmpBuf,0x00,sizeof(MsgHead)+body_tail_len);
			memcpy(tmpBuf,buffer,sizeof(MsgHead));
			while((count < body_tail_len) && (!feof(fp)))
			{
				readcount = fread(tmpBuf+readcount,sizeof(BYTE),body_tail_len-count,fp);
				count += readcount;
			}
			/* -------- 错误报文生成错误文件 ---------- */
			memset(errormsg,0x00,sizeof(errormsg));
			sprintf(errormsg,"报文长度:%d超过规定大小:%d\n",sizeof(MsgHead)+body_tail_len,8*1024);
			CLog::WriteAppLog(errormsg);
			COtherFuncs::CreateErrorTransfile(CConfig::ErrorPath,tmpBuf,sizeof(MsgHead)+body_tail_len);
			
			free(tmpBuf);
			tmpBuf = NULL;
			FileCount = FileCount + sizeof(MsgHead) + body_tail_len;
			continue;
		}
		
		/* -------- 读取整个报文 ---------- */
		if(body_tail_len != 0)
		{
			while((count < body_tail_len) && (!feof(fp)))
			{
				readcount = fread(buffer+readcount,sizeof(BYTE),body_tail_len-count,fp);
				count += readcount;
			}
			
			result = 0;       
			/* -------- 交易数据报文入库 ---------- */
			
			switch(msgtype)
			{
				case 0x7010:
					result = ORA::insert_into_7010(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					break;
					
				case 0x5001:
					result = ORA::insert_into_5001(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					break;					
					
				case 0x5002:
					result = ORA::insert_into_5002(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					break;					
					
				case 0x5003:
					result = ORA::insert_into_5003(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					break;	

				case 0x7001:
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_7001(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;

				case 0x7002:
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_7002(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;

				case 0x5004:
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_5004(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;	

				case 0x5005:
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_5005(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;	

				case 0x5010:
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_5010(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;	
					
				case 0x5020:
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_5020(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;	
			 
				case 0x5021: //20230129 新增
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_5021(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;				 
			 
				case 0x5022: //20230129 新增
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_5022(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;				 
 
				case 0x3001:
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_3001(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;	

				case 0x3003:
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_3003(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;	

				case 0x3005:
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_3005(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;	

				case 0x3006:
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_3006(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;
						
				case 0x3007:
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_3007(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;						
											
				case 0x3008:
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_3008(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;																
											
				case 0x4001:
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_4001(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;
						
				case 0x4002:
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_4002(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;						
											
				case 0x4003:
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_4003(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;																
																						
				case 0xF001:
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_F001(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;
						
				case 0xF002:
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_F002(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;						
											
				case 0xF003:
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_F003(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;																						

				case 0xF004:
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_F004(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;
						
				case 0xF005:
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_F005(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;						
											
				case 0xF006:
					sem_wait(&ORA::ms_CSSQL);
					result = ORA::insert_into_F006(name,msgtype,(unsigned char*)buffer,sizeof(MsgHead) + body_tail_len);
					sem_post(&ORA::ms_CSSQL);
					break;											
									
				default:
					memset(errormsg,0x00,sizeof(errormsg));
					sprintf(errormsg,"InsertStatusData unkown msgtype:0x%04X\n",msgtype);
					CLog::WriteAppLog(errormsg);
					break;										
																				
			}
			
			records++;	
			FileCount = FileCount + sizeof(MsgHead) + body_tail_len;
			
			/* -------- 数据库异常,停止应用 ---------- */
			if(result == -1 && CMainManage::ms_CommExitFlag == 0)
			{
				memset(errormsg,0x00,sizeof(errormsg));
				sprintf(errormsg,"文件(%s)--InsertStatusData error,db error\n",name);
				CLog::WriteAppLog(errormsg);

				ret = 0;
				fclose(fp);
 
			
				CMainManage::RestartProgram(NULL);
			}
			else
			{
				;
			}
		}
	}
	/* -------- 关闭交易数据文件 ---------- */
	fclose(fp);

	memset(errormsg,0x00,sizeof(errormsg));
	sprintf(errormsg,"InsertStatusData---%s---处理结束---\n",filename);
	CLog::WriteAppLog(errormsg);
		
	return ret;
}
 
		
/* ------------------------------------------------------
      功能描述: 交易状态寄存器文件入库线程函数
--------------------------------------------------------- */
void* CThread::ThreadInsertStatusData_TvmAr(void* lpParam)
{
	pthread_detach(pthread_self());
	while(1)
	{
		CommonFuncs::my_sleep(1);
		PrepareInsertStatusData(CConfig::InsertStatusFilePath,0x5001);
	}
	return NULL;
}

/* ------------------------------------------------------
      功能描述: 交易状态寄存器文件入库线程函数
--------------------------------------------------------- */
void* CThread::ThreadInsertStatusData_BomAr(void* lpParam)
{
	pthread_detach(pthread_self());
	while(1)
	{
		CommonFuncs::my_sleep(1);
		PrepareInsertStatusData(CConfig::InsertStatusFilePath,0x5002);
	}
	return NULL;
}


/* ------------------------------------------------------
      功能描述: 交易状态寄存器文件入库线程函数
--------------------------------------------------------- */
void* CThread::ThreadInsertStatusData_AgmAr(void* lpParam)
{
	pthread_detach(pthread_self());
	while(1)
	{
		CommonFuncs::my_sleep(1);
		PrepareInsertStatusData(CConfig::InsertStatusFilePath,0x5003);
	}
	return NULL;
}

/* ------------------------------------------------------
      功能描述: 交易状态寄存器文件入库线程函数
--------------------------------------------------------- */
void* CThread::ThreadInsertStatusData_Status(void* lpParam)
{
	pthread_detach(pthread_self());
	while(1)
	{
		CommonFuncs::my_sleep(1);
		PrepareInsertStatusData(CConfig::InsertStatusFilePath,0x7010);
	}
	return NULL;
}

/* ------------------------------------------------------
      功能描述: 交易状态寄存器文件入库线程函数
--------------------------------------------------------- */
void* CThread::ThreadInsertStatusData_Other(void* lpParam)
{
	pthread_detach(pthread_self());
	while(1)
	{
		CommonFuncs::my_sleep(1);
		PrepareInsertStatusData(CConfig::InsertStatusFilePath,0x0000);
	}
	return NULL;
}

/* ------------------------------------------------------
            功能描述:   定时定时广播时钟线程
--------------------------------------------------------- */
void* CThread::ThreadQueryCCPort1(void* lpParam)
{
	pthread_detach(pthread_self());
	
	int sockfd;
	char errsg[100];

	while(1)
	{
		CommonFuncs::my_sleep(5);
			
	 	/* -------- 获得客流 ---------- */
		ORA::updateFlow();
 
		sem_wait(&ISCS::g_CS_ISCS);
		for(int i=0;i<MAX_SLE_COUNT;i++)
		{
			ISCS::g_SC_FLOW[i].EntryFlow = CConfig::g_ScList[i].EntryFlow;
			ISCS::g_SC_FLOW[i].ExitFlow  = CConfig::g_ScList[i].ExitFlow;
			ISCS::g_SC_FLOW[i].SJTFlow 	 = CConfig::g_ScList[i].SJTFlow;			
		}					    	
		sem_post(&ISCS::g_CS_ISCS);

		/*---------更新客流数据---------*/
		ISCS::updateIscsFlow();	
 

		sockfd = CSocket::Tcp_Connect(CConfig::g_LocalIP,CConfig::g_Lsn_ScPort,5000);
		if(sockfd < 0)
		{
			CommonFuncs::my_sleep(5);
			sockfd = CSocket::Tcp_Connect(CConfig::g_LocalIP,CConfig::g_Lsn_ScPort,5000);
			if(sockfd < 0)
			{
				CommonFuncs::my_sleep(5);
				sockfd = CSocket::Tcp_Connect(CConfig::g_LocalIP,CConfig::g_Lsn_ScPort,5000);
				if(sockfd < 0)
				{
					memset(errsg,0x00,sizeof(errsg));
					strcpy(errsg, "重新启动侦听服务\n");
					CLog::WriteAppLog(errsg);
			
					CMainManage::RestartProgram(NULL);
				}
			}
		}
		
		CSocket::CSocket::Tcp_Close(sockfd);
	}
	return NULL;
}








  





/* ----------------------------------------------------- */
/*                       构建NTP协议包                   */
/* ----------------------------------------------------- */
int CSynTime::construct_packet(char *packet)
{
	time_t	timer;
     	
  memset(packet, 0, NTP_PCK_LEN);
  packet[0] = 0x1B;
  
  time((time_t *)&timer);  
  timer = htonl(timer + JAN_1970 ); 
  memcpy(packet + 40, &timer, sizeof(timer));   

	return NTP_PCK_LEN;
}

/* ----------------------------------------------------- */
/*                       获取NTP时间                     */
/* ----------------------------------------------------- */
int CSynTime::get_ntp_time(int sockfd, struct sockaddr_in *server_addr, struct tm *net_tm)
{
	char           content[256];  
	time_t         timet;  
	int            temp;  
	int            addr_len = 16;  
	struct timeval block_time;  
	fd_set         sockfd_set;  
 
	construct_packet(content);  
	if(sendto(sockfd, content, NTP_PCK_LEN, 0, (struct sockaddr *)server_addr, addr_len) < 0) 
	{
		CSocket::Tcp_GetSocketErrorMsg("sendto",errno);
		CLog::WriteCommuLog(CSocket::ms_str_SocketErrorMessage); 
		return 0;
	}
		
	FD_ZERO(&sockfd_set);  
	FD_SET(sockfd, &sockfd_set);  
	block_time.tv_sec = CConfig::m_NtpTimedout;   
	block_time.tv_usec = 0;  
	if(select(sockfd + 1, &sockfd_set, NULL, NULL, &block_time ) > 0) 
	{  
		if (recvfrom(sockfd, content, sizeof(content), 0, (struct sockaddr *)server_addr, (socklen_t *)&addr_len) < 0) 
		{
			CSocket::Tcp_GetSocketErrorMsg("recvfrom",errno);
			CLog::WriteCommuLog(CSocket::ms_str_SocketErrorMessage); 
			return 0;
		} 
		else 
		{  
			memcpy(&temp, content + 40, 4);  
			temp = (time_t)(ntohl(temp) - JAN_1970);  
			timet = (time_t)(temp + 8 * 3600);  //beijing time zone
			memcpy(net_tm, gmtime(&timet), sizeof(struct tm)); 
			//net_tm->tm_hour = net_tm->tm_hour + 8;  
		}  
	}  
	else 
	{  
			CSocket::Tcp_GetSocketErrorMsg("recvfrom",errno);
			CLog::WriteCommuLog(CSocket::ms_str_SocketErrorMessage); 
			return 0; 
	}  

	return 1; 

}

/* ----------------------------------------------------- */
/*                       修改本地时间                    */
/* ----------------------------------------------------- */
int CSynTime::set_local_time(char *c_time)
{  
	int                  rtn,sockfd;  
	struct tm            net_tm;  
	struct sockaddr_in   addr;  
	int       					 year,month,day,hour,min,sec;
	char                 logdata[100];
   
	memset(&addr, 0, sizeof(addr));  
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(CConfig::m_NtpIP);  
	addr.sin_port = htons(CConfig::m_NtpPort);   
         
	if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)  
		return 0;   
	
	rtn = get_ntp_time(sockfd,&addr,&net_tm);
	if(rtn == 1)
	{
		year  = (int)net_tm.tm_year + 1900;
		month = (int)net_tm.tm_mon + 1;
		day   = (int)net_tm.tm_mday;
		hour  = (int)net_tm.tm_hour;
		min   = (int)net_tm.tm_min;
		sec   = (int)net_tm.tm_sec;
		if( (year >= 2013 && year <= 2038) && (month  >= 1 && month  <= 12) && (day    >= 1 && day    <= 31) &&
			  (hour >= 0    && hour <= 23)   && (min    >= 0 && min    <= 59) && (sec    >= 0 && sec    <= 59) )
		{
			sprintf(c_time,"%04d%02d%02d%02d%02d%02d",year,month,day,hour,min,sec);
		}
		else
		{
			memset(logdata,0x00,sizeof(logdata));
			sprintf(logdata,"时钟报文格式错误:%04d年%02d月%02d日%02d时%02d分%02d秒\n",year,month,day,hour,min,sec);
			CLog::WriteCommuLog(logdata);
			
			rtn = 0;
		}
	}
	
	CSocket::Tcp_Close(sockfd);
	
	return rtn;  
}

int CSynTime::UpdateSystemTime (time_t ulTime,char *c_time)
{
	char          logdata[500],temp[10];
	int           rtn;
	int	   			  year,month,day,hour,min,sec;

	memset(temp,0x00,sizeof(temp));
	memcpy(temp,c_time,4);
	year = atoi(temp);
	memset(temp,0x00,sizeof(temp));
	memcpy(temp,c_time+4,2);
	month = atoi(temp);
	memset(temp,0x00,sizeof(temp));
	memcpy(temp,c_time+6,2);
	day = atoi(temp);
	memset(temp,0x00,sizeof(temp));
	memcpy(temp,c_time+8,2);
	hour = atoi(temp);
	memset(temp,0x00,sizeof(temp));
	memcpy(temp,c_time+10,2);
	min = atoi(temp);
	memset(temp,0x00,sizeof(temp));
	memcpy(temp,c_time+12,2);
	sec = atoi(temp);
 
	time_t			local_time;
	unsigned int	dw_now,dw_ulTime;
	local_time = time(NULL);
	dw_now = (unsigned int)local_time;
	dw_ulTime = (unsigned int)ulTime;
 
	int time_diff = dw_ulTime - dw_now;
	int time_diff_define_sec = 24*60*60;
	rtn = 0;
	while(true)
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"ulTime:%d,dw_now:%d,时间差为:%d\n",dw_ulTime,dw_now,time_diff); 	
		CLog::WriteAppLog(logdata);
		
		if(time_diff > time_diff_define_sec || time_diff <  -1*time_diff_define_sec)
		{
				memset(logdata,0x00,sizeof(logdata));
				sprintf(logdata,"时间差大于%d秒,不予同步\n",time_diff_define_sec); 	
				CLog::WriteAppLog(logdata);
					
				CConfig::ms_SynTimeOverDiff = true;	
				CConfig::ms_SynTimeError = true;	
						
				rtn = -1;				
				break;	
		}
		else
		{
				CConfig::ms_SynTimeOverDiff = false;	
		}
		
		rtn = stime(&ulTime);
		memset(logdata,0x00,sizeof(logdata));
		if(rtn != 0 )
		{
			sprintf(logdata,"同步系统时钟失败:%04d年%02d月%02d日%02d时%02d分%02d秒\n",year,month,day,hour,min,sec);		
			CConfig::ms_SynTimeError = true;	
		}
		else
		{
			sprintf(logdata,"同步系统时钟成功:%04d年%02d月%02d日%02d时%02d分%02d秒\n",year,month,day,hour,min,sec);
			//system(" hwclock -w "); //修改系统时钟成功之后,修改硬件时钟
			CConfig::ms_SynTimeError = false;			
		}
		CLog::WriteAppLog(logdata);
		
		break;
	}
 
	SC::OccurStatusFile(CConfig::g_CCIDCode,2);
	
	return rtn;
}

void CSynTime::Adjust_System_Time()
{
	char 				 c_time[30];
	time_t       t_CurrentTime;
	int          ret;
	char         logdata[100];
	
	memset(c_time,0x00,sizeof(c_time));
	ret = set_local_time(c_time);
	if(ret == 1)
	{
		t_CurrentTime = CTime::GetGmtTime(c_time);
		UpdateSystemTime(t_CurrentTime,c_time);
	}
	else
	{
		
		memset(logdata,0x00,sizeof(logdata));
		strcpy(logdata,"时钟校对失败(NTP)\n");
		CLog::WriteAppLog(logdata);
	}
}

void* CSynTime::ThreadAdjustSystemTime(void* inpara)
{
	pthread_detach(pthread_self());
	
	while(1)
	{
		Adjust_System_Time();
		CommonFuncs::my_sleep(CConfig::m_NtpGap);

	}
	return NULL;
}
 



 

 
int 		CMainManage::ms_CommExitFlag;
	
/* ------------------------------------------------------
            功能描述:   初始化
--------------------------------------------------------- */
int CMainManage::MainStart()
{	
	int       i,ret;
	char      errmsg[500];
	pthread_t thread_handle;
	time_t    local_time;
	
	/* --------创建目录 ---------- */
	ret = CDir::CreateAppAllDir();
	if(ret == 0)
		return 0;
 
	/* -------- 初始化日志类 ---------- */
	 ret = CLog::InitLogStaticVar(CDir::m_AppLogPath);
	 		if(ret == 0)
		return 0;
 
	/* ----- 读取配置文件 ----------*/
	CConfig::InitConfigStaticVar(CDir::m_AppPath);
 
 	/* -------- 初始化Package类 ---------- */	
	CPackage::InitCPackageStaticVar();
 
 	/* -------- 初始化 CPara 类 ---------- */	
	CPara::InitCParaStaticVar();
 
 	/* -------- 初始化 ACC 类 ---------- */	
	ACC::InitAccStaticVar(); 

 	/* -------- 初始化 ISCS 类 ---------- */	
	ISCS::IscsInit(); 
 
 
	/* -------- 连接数据库 ---------- */
	struct timeval      timeoutSTRU;  
	for(i=0;i<20;i++)
	{
		ret = ORA::DbStart(CConfig::m_DBUserName,CConfig::m_DBUserPasswd,CConfig::m_DBHostName);
		if(ret == 1)
			break;
		timeoutSTRU.tv_sec  = 30;
		timeoutSTRU.tv_usec = 0;
		select(0,NULL,NULL,NULL,&timeoutSTRU);
	}
	if(i==20)
	{
		return 0;
	}
 
	memset(errmsg,0x00,sizeof(errmsg));
	sprintf(errmsg,"数据库联接成功\n");
	CLog::WriteAppLog(errmsg);
 

	/* --- 获得车站列表 --- */
	sem_wait(&ORA::ms_CSSQL);
	ORA::getsclist();
	sem_post(&ORA::ms_CSSQL);
	
	/* --- 获得线路列表 --- */
	sem_wait(&ORA::ms_CSSQL);
	ORA::get_mlc_line_list();
	sem_post(&ORA::ms_CSSQL); 

	/* --- 创建线路参数文件夹 --- */		
	for(int i = 0;i < MAX_SLE_COUNT;i++)
	{
		if(CConfig::ms_MlcLineList[i].LINE_ID > 0)
		{
			CDir::CreateLineParamDir(CConfig::ms_MlcLineList[i].LINE_ID);
		}
	}
 
 	/* -------- 生成参数文件 ---------- */
 	CPara::CreateParamFile();
 
 	/* -------- 获得设备的当前状态 ---------- */
 	sem_wait(&ORA::ms_CSSQL);
	ORA::GetSleStatus();
	sem_post(&ORA::ms_CSSQL); 
 
 	/* ---- 通信动态库初始化 ---- */
	ret = CCommu::CommunicationMain(CConfig::g_Lsn_ScPort,CConfig::g_Lsn_AccPort,CConfig::g_Lsn_ISCSPort);
	if (ret != 1)
	{
		memset(errmsg,0x00,sizeof(errmsg));
		strcpy(errmsg,"通信动态库(CommunicationMain)初始化失败\n");
		CLog::WriteAppLog(errmsg);
		return 0;
	}
 
	memset(errmsg,0x00,sizeof(errmsg));
	sprintf(errmsg,"通信动态库(SERVER)初始化成功(本机IP地址 = %s)\n",CConfig::g_LocalIP);
	CLog::WriteAppLog(errmsg);
	
   
 
 	/* --------- 创建校对时钟校对线程 ----------------*/
	pthread_create(&thread_handle,NULL,CSynTime::ThreadAdjustSystemTime,(void *)NULL);

	/* --------- 创建查询sc参数版本线程 ---------------- */
	ret = pthread_create(&thread_handle,NULL,SC::ThreadQueryScParam,(void *)NULL);

	/* --------- 创建查询sc状态线程 ---------------- */
	ret = pthread_create(&thread_handle,NULL,SC::ThreadQueryScStatus,(void *)NULL);

	/* --------- 创建生成发送文件线程 ---------------- */
	ret = pthread_create(&thread_handle,NULL,ACC::ThreadCreateSendFile,(void *)NULL);

	/* --------- 创建定时插入交易数据线程 ----------------*/
	ret = pthread_create(&thread_handle,NULL,CThread::ThreadInsertTransData_1001,(void *)NULL);
	ret = pthread_create(&thread_handle,NULL,CThread::ThreadInsertTransData_1002,(void *)NULL);
	ret = pthread_create(&thread_handle,NULL,CThread::ThreadInsertTransData_1003,(void *)NULL);
	ret = pthread_create(&thread_handle,NULL,CThread::ThreadInsertTransData_1004,(void *)NULL);
	ret = pthread_create(&thread_handle,NULL,CThread::ThreadInsertTransData_1005,(void *)NULL);
	ret = pthread_create(&thread_handle,NULL,CThread::ThreadInsertTransData_1006,(void *)NULL);										
 
	/* --------- 创建定时插入状态寄存器数据线程 ----------*/
	ret = pthread_create(&thread_handle,NULL,CThread::ThreadInsertStatusData_TvmAr,(void *)NULL);	
	ret = pthread_create(&thread_handle,NULL,CThread::ThreadInsertStatusData_BomAr,(void *)NULL);
	ret = pthread_create(&thread_handle,NULL,CThread::ThreadInsertStatusData_AgmAr,(void *)NULL);
	ret = pthread_create(&thread_handle,NULL,CThread::ThreadInsertStatusData_Status,(void *)NULL);	
	ret = pthread_create(&thread_handle,NULL,CThread::ThreadInsertStatusData_Other,(void *)NULL);
 
	/* --------- 创建定时发送数据线程 ----------------*/
	ret = pthread_create(&thread_handle,NULL,ACC::ThreadSendData_ACC,(void *)NULL);

	/* --------- 创建定时删除历史数据线程----------------*/
	ret = pthread_create(&thread_handle,NULL,CThread::ThreadDelHistoryData,(void *)NULL);

	
	/* --------- 创建定时轮询规程1端口线程----------------*/
	ret = pthread_create(&thread_handle,NULL,CThread::ThreadQueryCCPort1,(void *)NULL);

		
	/* --------- 创建定时生效参数线程----------------*/
	ret = pthread_create(&thread_handle,NULL,CPara::ThreadValidateParam,(void *)NULL);

		

	CMainManage::ms_CommExitFlag = 0;
	
 
	
	return 1;
}

void CMainManage::MainStop()
{
	char applog[100];
	
	//通信动态库退出	
	CCommu::CommunicationStop();
	memset(applog,0x00,sizeof(applog));
	strcpy(applog,"通信动态库退出完毕\n"); 
	CLog::WriteAppLog(applog);
		
	//断开数据库连接
	ORA::DbEnd();
	memset(applog,0x00,sizeof(applog));
	strcpy(applog,"断开数据库连接完毕\n"); 
	CLog::WriteAppLog(applog);

	memset(applog,0x00,sizeof(applog));
	strcpy(applog,"CC应用程序退出\n"); 
	CLog::WriteAppLog(applog);		
 	 
	//释放Package类变量		
	CPackage::ReleaseCPackageStaticVar();

 	
	//释放Para类变量		
	CPara::ReleaseCParaStaticVar();
 	
	//释放Acc类变量		
	ACC::ReleaseAccStaticVar();		

	//释放综合监控类变量			
	ISCS::IscsRelease();

	//释放日志类变量	
	CLog::ReleaseLogStaticVar(); 
}	
	
	


/* ------------------------------------------------------
            重新启动应用
--------------------------------------------------------- */
void  CMainManage::RestartProgram(char *path)
{
	char errormsg[1024];
	
	memset(errormsg,0x00,sizeof(errormsg)); 
	sprintf(errormsg, "RestartProgram errno:%d strerror:%s\n",errno, strerror(errno));
	CLog::WriteAppLog(errormsg);
			
	if (fork() == 0)	
	{	
		for (int i = 0; i < sysconf(_SC_OPEN_MAX); i++)
		{
    	if (i != STDIN_FILENO && i != STDOUT_FILENO && i != STDERR_FILENO)
    	{
    		close(i);
    	}
    }
    if(path == NULL)
			execl("./UpdateSoft","UpdateSoft",(char *) 0,(char *) 0); 
		else
			execl("./UpdateSoft","UpdateSoft",path,(char *) 0); 
		
		exit(0);
	}
	
	CommonFuncs::my_sleep(5);
	
	exit(0);
}

 
/********************************************/
/*            进程退出,释放内存             */
/********************************************/
void CMainManage::ReleaseMem()
{
	MainStop();
}

/********************************************/
/*                 捕捉信号                 */
/********************************************/
void CMainManage::CatchSignal(int the_sig)
{
	int ret;
	char oplogmsg[100];
	
	if(the_sig == SIGTERM)/* ---- 进程退出 ----*/
	{
		CMainManage::ms_CommExitFlag = 1;
 
		ReleaseMem();
		
		exit(0);	
	}
	else if(the_sig == SIGUSR2)/* ---- 重连数据库 ----*/
	{
		 
		ORA::DbStart(CConfig::m_DBUserName,CConfig::m_DBUserPasswd,CConfig::m_DBHostName);
		    
	}
	else if(the_sig == SIGSEGV)/* ---- 段错误信号忽略 ----*/
	{
		memset(oplogmsg,0x00,sizeof(oplogmsg));
		strcpy(oplogmsg,"SIGSEGV信号捕捉,程序即将重新初始化\n");
		CLog::WriteAppLog(oplogmsg);
		
		CMainManage::RestartProgram(NULL);
	}
	else if(the_sig == SIGBUS)/* ---- 段错误信号忽略 ----*/
	{
		memset(oplogmsg,0x00,sizeof(oplogmsg));
		strcpy(oplogmsg,"SIGBUS信号捕捉,程序即将重新初始化\n");
		CLog::WriteAppLog(oplogmsg);
		
		CMainManage::RestartProgram(NULL);
	}
	else
	{
		;
	}
	
	return;
}




/* ------------------------------------------------------
     功能描述:   初始化控制变量,成功返回1,失败返回0 
     返回值说明: -1:函数调用失败
                 0: ACC命令报文
                 1: SC报文
                 2: 消息队列满
                 3: 通信退出
                 >3:参数报文 
--------------------------------------------------------- */
int CMainManage::Wait_MsgReadSem()
{
	int ret;
	
	for(;;)
	{
		ret = sem_wait(&CMessage::g_SemReadMsg);
		if (ret < 0)                        
		{
			/* --- 收到信号 ---------- */
			if (errno == EINTR)
			{
				if(CThreadPool::g_StopFlag == 1)
				{
					ret = -3;
					break;
				}
				else
				{
					continue;
				}
			}
		}
		else
		{
			/* --- 报文通知 ---------- */
			ret = CThreadPool::g_MsgFlag;
			break;               
		}
	}
	
	return ret;
}
 
 
/* ------------------------------------------------------
            功能描述:  主函数
--------------------------------------------------------- */
int main(int argc,char *argv[])
{
	int ret,size;
	char applog[100],errmsg[100];
  pthread_t hThreadHandle;
	
	/* --------- 设置线程栈空间 ----------------*/
	pthread_attr_t attr;
	size_t         u_statck;
	
	u_statck = 2*1024*1024;
	pthread_attr_init(&attr );
	pthread_attr_setstacksize(&attr,u_statck);
	
	/* --------- 获得主进程的ID ----------------*/
	//g_pid = getpid();
		
	/* --------- 进程设为守护进程 ----------------*/
	CommonFuncs::Daemon_init();
		
	/* --------- 关联SIGTERM信号 ----------------*/
	sigset(SIGTERM, CMainManage::CatchSignal);
	/* --------- 关联SIGUSR1信号 ----------------*/
	sigset(SIGUSR1, CMainManage::CatchSignal);
	/* --------- 忽略SIGUSR2信号 ----------------*/
	sigset(SIGUSR2,SIG_IGN);
	/* --------- 忽略SIGPIPE信号 ----------------*/
	sigset(SIGPIPE,SIG_IGN);
	/* --------- 关联SIGSEGV信号 ----------------
	sigset(SIGSEGV,CMainManage::CatchSignal);*/
	/* --------- 关联SIGBUS信号 ----------------*/
	sigset(SIGBUS,CMainManage::CatchSignal);
	/* --------- 忽略SIGFPE 信号 ----------------*/
	sigset(SIGFPE,SIG_IGN);
	/* --------- 忽略SIGCHLD信号 ----------------*/
	sigset(SIGCHLD,SIG_IGN);
	
	/* ----- 忽略信号 --------
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigaction( SIGPIPE, &sa, 0 );
	sa.sa_handler = SIG_IGN;
	sigaction( SIGSEGV, &sa, 0 );
	sa.sa_handler = SIG_IGN;
	sigaction( SIGFPE, &sa, 0 );
	sa.sa_handler = SIG_IGN;
	sigaction( SIGCHLD, &sa, 0 );
	sigset(SIGPIPE,SIG_IGN);*/
	

	
	if(CommonFuncs::detect_process("./metro_center") == 1)
	{
		return 1;
	}
	
	/* --------- 初始化 ----------------*/
	ret = CMainManage::MainStart();
	if (ret == 1)
	{ 
		memset(applog,0x00,sizeof(applog));
		strcpy(applog,"CC应用程序启动\n");
		CLog::WriteAppLog(applog);
	}
	else
	{
		memset(applog,0x00,sizeof(applog));
		strcpy(applog,"CC应用程序启动失败\n");
		CLog::WriteAppLog(applog);
			
		CMainManage::ReleaseMem();
		exit(0);
	}

	/*---------------CMyJson-----TEST-----------------
	printf("----------CMyJson::CreateDemo()-------begin-------\n");
	CMyJson::CreateDemo();
	printf("----------CMyJson::CreateDemo()-------end-------\n");	*/
	
	/*---------------CMyXml-----TEST-----------------
	printf("----------CMyXml::GetPersonXmlDemo()-------begin-------\n");
	CMyXml::GetPersonXmlDemo(NULL);
	printf("----------CMyXml::GetPersonXmlDemo()-------end-------\n");	*/	
	
	/*---------------CMyHttp-----TEST-----------------
	printf("----------CMyHttp::MyHttpDemo()-------begin-------\n");
	CMyHttp::MyHttpDemo();
	printf("----------CMyHttp::MyHttpDemo()-------end-------\n");	*/	
	   		
  //生成程序版本号
	char version[100],filename[256];
	memset(version,0x00,sizeof(version));
	sprintf(version,"SoftVer:%d.%d.%d\n",PRIMARY_VERSION,SECOND_VERSION,THIRD_VERSION);
	memset(filename,0x00,sizeof(filename));
	strcpy(filename,"version.txt");
	CommonFuncs::WriteBufferFile("wb",filename,version,strlen(version));
 
	/* ----- 检查磁盘空间 ------*/
	memset(applog,0x00,sizeof(applog)); 
	CConfig::ms_DiskIsFull = CommonFuncs::DiskIsFull(90,applog);
	SC::OccurStatusFile(CConfig::g_CCIDCode,2);
	CLog::WriteAppLog(applog);	
 
	ret = CThreadPool::Server_Start();
	if (ret != 1)
	{
		memset(applog,0x00,sizeof(applog));
		strcpy(applog,"监听启动失败\n");
		CLog::WriteAppLog(applog);
	 
		CMainManage::ReleaseMem();
		exit(0);
	}
	else
	{
		memset(applog,0x00,sizeof(applog));
		strcpy(applog,"监听启动成功\n");
		CLog::WriteAppLog(applog);
			
	}
 
	/* --------- 读取报文 ----------------*/
	int             handle;
	para            *m_para;
	int             inform_type;
	char            clnip[16];
	int             clnport;
	
	while(1)
	{
		ret = CMainManage::Wait_MsgReadSem();
		if (ret == 0 || ret == 1)/* ----- 非参数报文 -------*/
		{
			/* -------- 从消息队列里读取报文 ---------- */
			m_para = (para*)malloc(sizeof(para));
			memset(m_para,0x00,sizeof(para));
			inform_type = 0;
			memset(clnip,0x00,sizeof(clnip));
			clnport = 0;
			
			handle = CMessage::MsgRead((char *)m_para->recv_buf,&inform_type,clnip,clnport);

			if(handle <= 0)
			{
				free(m_para);
				m_para = NULL;
			}
			else
			{
				if(inform_type == 1)/* ----- ACC报文通知 -------*/
				{
					ACC::OnACCMessageIn(m_para,handle,clnip,clnport);
				}
				else if(inform_type == 2)/* ----- SC报文通知 -------*/
				{
					SC::OnSCMessageIn(m_para,handle,clnip,clnport);

				}
				else
				{
					memset(applog,0x00,sizeof(applog));
					sprintf(applog, "未定义的报文通知类型:%d\n",inform_type);
					CLog::WriteAppLog(applog);
					
					free(m_para);
					m_para = NULL;
				}
			}	
		}
		else if (ret == 2)/* ----- 消息队列满 -------*/
		{
			memset(applog,0x00,sizeof(applog));
			strcpy(applog, "消息队列已满,断开所有连接\n");
			CLog::WriteAppLog(applog);

			CMainManage::RestartProgram(NULL);
		}
		else if(ret == -3)/* ----- 通信退出 -------*/
		{
			continue;
		}
		else             /* --------- 参数报文通知 -------*/
		{
			;
		}
	}
	
	return 1;
 
}

