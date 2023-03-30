#ifndef CC_COMMU_DL_H
#define CC_COMMU_DL_H

#include "sys_head.h"
#include "base_define.h"
#include "base_funcs.h"
#include "base_socket.h"
#include "base_message.h"
#include "base_md5.h"
#include "base_crc.h"
#include "cc_struct_dl.h"
#include "cc_base_dl.h"

#define LEN_CRITICALSESSION	    sizeof(CRITICAL_SECTION)
 
#define MAX_SPLITPACKAGE    500
#define TIME_GAP            0

#define MAX_CLIENT_NUMBER1      32          /* ------- 规程1连接的最大客户数 -------- */
#define MAX_CLIENT_NUMBER4      5           /* ------- 规程4连接的最大客户数 -------- */
#define MAX_LISTEN_NUMBER       4           /* ------- 最多侦听端口数 --------------- */


class CThreadPool
{
public:
    static int               g_StopFlag;
    static int               g_MsgFlag;            /* --- 报文类型的标志 -------------------- */
    static pthread_t         *g_Server_1;           /* --- 线程池1内各线程句柄 ------------- */
    static pthread_t         *g_Server_4;           /* --- 线程池4内各线程句柄 ------------- */
    static pthread_t         g_ISCSHandle;          /* --- 综合监控服务器的线程句柄 -------- */

    static sem_t             *g_SemServer_1;   			/* ------- 同步线程池1信号量 ------------- */
    static sem_t             *g_SemServer_4;   			/* ------- 同步线程池4信号量 ------------- */
    static sem_t             *g_SemParam;       		/* --- 信号量分配空间(同步参数队列) ------- */

    static int               *g_ListenSock;         /* --- 侦听套接口句柄 ------------------ */
    static int               *g_Client_1;           /* --- 规程1与客户端连接的socket句柄 --- */
    static int               *g_Client_4;           /* --- 规程4与客户端连接的socket句柄 --- */
    static int								g_Client_5;           /* --- 规程5与客户端连接的socket句柄 --- */
    static char 							ms_str_ThreadPoolErrorMessage[1024];

private:
    static int       Create_Sem();
    static void      Release_Sem();
    
    static int      Create_ListenSock();
    static void     Close_ListenSock();
    static void     Release_ListenSock();

    static int      Create_ClientSock();
    static void     Close_ClientSock();
    static void     Release_ClientSock();

    static int      Initial_ThreadPool();
    static int      Create_ThreadPool_SC();
    static int      Create_ThreadPool_ACC();
    static void*    Thread_Server_SC(void* lpParam);
    static void*    Thread_Server_ACC(void* lpParam);
    static void* 		Thread_Get_ISCS(void* lpParam); 
    static void     Wait_ThreadPool_SC_Exit();
    static void     Wait_ThreadPool_ACC_Exit();
    static void     Wait_ISCS_Exit();
    static void     Release_ThreadPool();
    static int      Recv_ScPackage(int flag,int connfd,char *clnip,int port);
    static int      Recv_ACCPackage(int sockfd,char *clnip,int port); 

public:
    static  int     Server_Initial();
    static  int     Server_Start();
    static  void    Server_Close();

}; 


//----------------综合监控相关处理------------------------- 

typedef struct _ISCSMsg
{
	unsigned short TCP_Num;
	unsigned short Proto_ID;
	unsigned short Length;
	unsigned char Unit_ID;
	unsigned char Func_Code;
	unsigned short Start_Addr;
	unsigned short Register_Num;
}ISCSMsg;


//约定设备编号原则,后两位从1开始
#define CLIENT_ISCSPKG_LEN      12          /* --- 综合监控发送报文长度 ---- */
#define SERVER_ISCS_HEAD_1      9           /* --- 综合监控报文包头长度1 ---- */
#define SERVER_ISCS_HEAD_2      4           /* --- 综合监控报文包头长度2 ---- */
#define CLIENT_ISCS_HEAD_1      8           /* --- 综合监控报文包头长度1 ---- */
#define CLIENT_ISCS_HEAD_2      10          /* --- 综合监控报文包头长度2 --- */
#define ISCS_STATION_BYTES  		100*2
/*
#define ISCS_STATION_START  		1
#define ISCS_STATION_END  		  18

#define ISCS_AGM_START_BYTE 		0			//闸机
#define ISCS_XAGM_START_BYTE 		0			//出站
#define ISCS_EAGM_START_BYTE 		0			//进站
#define ISCS_EXAGM_START_BYTE   0
#define ISCS_BOM_START_BYTE 		75*2
#define ISCS_TVM_START_BYTE 		50*2
#define ISCS_TCM_START_BYTE 		70*2					 
#define ISCS_SNC_START_BYTE 		80*2
#define ISCS_RUNMODE_START_BYTE 81*2
#define ISCS_IBP_START_BYTE     82*2//无
#define ISCS_EPF_START_BYTE 		88*2
#define ISCS_XPF_START_BYTE 		90*2
#define ISCS_SPF_START_BYTE 		92*2 
*/
 

class ISCS
{
public:
    static  sem_t         g_CS_ISCS;
    static  SC_FLOW       g_SC_FLOW[MAX_SLE_COUNT];
    static  BYTE 					g_ISCS_SndPkg[MAX_BAOWEN_LENGTH];

public:
    static  void 		DealWithISCSMsg(BYTE *recvBuf,int connfd);
    static  void 		UpdateDeviceStatus(BYTE *p,int eventlevel);
    static  void 		UpdateStationRunMode(char* stationNid,BYTE modeCode);
    static  void 		UpdateIscsStatusSc(char* stationNid,int isConnect); 
    static  void 		updateIscsFlow();   
    static  int 		GetIscsStationInfo(char* stationNid);      
    static	int     HexToDec(int zhandh);
         
    static  void IscsInit();   		//综合监控初始化 
    static  void IscsRelease();	  //综合监控销毁 
};




class CCommu
{
public:
static  int                   g_Port1;                   /* ------ 规程1侦听端口号 ----------------------------- */
static  int                   g_Port4;                   /* ------ 规程4侦听端口号 ----------------------------- */
static  int                   g_Port5;                   /* ------ 规程5侦听端口号 ----------------------------- */
static  sem_t         				g_CSScList;
public:
    static  int 	CommunicationMain(int port1,int port4,int port5);
    static  void  CommunicationStop(void);
    
    static  int 	RecvMsg(int sockfd,char *buffer,int msg_len);
    
    static  int   Recv_ACCPackage(int sockfd,char *clnip,int port);
    static  int   Prepare_ACCRecvPackage(int sockfd,char *clnip,int port);
    static  int 	RecvACCPackage_Client(int sockfd,unsigned char *buffer,int timedout);
    
    static  int		GetMsgLen(int sockfd,unsigned char *buffer);
    
    static  int		Recv_ScPackage(int flag,int connfd,char *clnip,int port); 
    static  int		DealScMsg(int flag,int sockfd,char *clnip,int port);
    static  int 	PrepareWriteMsgQ(int sockfd,unsigned char *PackageLenBuffer,int message_len,char *clnip,int port);

    static  void  CreateAckPkgHead(int pkg_len,WORD msgtype,MsgHead *m_OldPackageHead,MsgHead *m_NewPackageHead);  
    static  int		SendtoACK(int sockfd,int msgtype,MsgHead *m_OldPackageHead,char *PackageBody,unsigned short LenofPackageBody);

    static  int   SleIsLConn(char *ip,int port,int conn_sockhandle);
    static  int   RecvACKPackage(int sockfd,char *buffer,int timedout,int buflen);
    static  int   RecvACKPackage(int sockfd,char *buffer,int timedout);
    static  int		Wait_L_Conn_Ack(int index,char *buffer,int &acklen,WORD msgtype);
    static  int		Wait_S_Conn_Ack(int index,char * sendbuf,int sendlen,char *rspbuf,int &rsplen,WORD req_msgtype,WORD ack_msgtype,int buflen);
    static  int 	SendANDRecv_L_S(int index,char * sendbuf,int sendlen,char *rspbuf,int &rsplen,WORD req_msgtype,WORD ack_msgtype,int buflen);
    static  int 	SendANDRecv(char *ipaddr,int port,char * sendbuf,int sendlen,char *rspbuf,int &rsplen,int timedout);
};

class COtherFuncs
{
 
public:
    static  int		CreateErrorTransfile(const char *dir,unsigned char* recv_buf,int buflen);
    static void 	SaveHistoryData(int flag,char *curpath,char *name,int mday);
};
 
//所有有关包处理的类
class CPackage
{
	private:
		static	sem_t   	    g_CSSessionNum;
		static	unsigned int  g_sessionnum;
		static	unsigned short g_Lcc_Count;
	public:		
		static 	void 					Create_Pkg_Head(unsigned char id,MsgHead *m_MsgHead,int bodylen,WORD msgtype,unsigned int session,unsigned char *toid,char *c_PkgID);
		static 	void 					Create_Pkg_Head(unsigned char id,MsgHead *m_MsgHead,int bodylen,WORD msgtype,unsigned int session,unsigned char *toid,char *c_PkgID,unsigned char *sourceid);

	 	static 	int 					CreatePackage(int offset,char *Package,MsgHead *m_PackageHead,char *PackageBody,int LenPackageBody);

	 	static 	void  InitCPackageStaticVar();
	 	static 	void	ReleaseCPackageStaticVar();	
 
}; 
 
 
#endif
