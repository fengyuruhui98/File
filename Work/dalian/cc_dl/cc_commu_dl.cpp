
#include "cc_commu_dl.h"

pthread_t*  CThreadPool::g_Server_1;
pthread_t*  CThreadPool::g_Server_4;
pthread_t   CThreadPool::g_ISCSHandle;          /* --- 综合监控服务器的线程句柄 -------- */

sem_t*   CThreadPool::g_SemServer_1;   /* ------- 同步线程池1信号量 ------------- */
sem_t*   CThreadPool::g_SemServer_4;   /* ------- 同步线程池4信号量 ------------- */
sem_t*   CThreadPool::g_SemParam ;     /* --- 信号量分配空间(同步参数队列) --------- */

int*     CThreadPool::g_ListenSock;
int*     CThreadPool::g_Client_1;           /* --- 规程1与客户端连接的socket句柄 --- */
int*     CThreadPool::g_Client_4;           /* --- 规程4与客户端连接的socket句柄 --- */
int    	 CThreadPool::g_Client_5;           /* --- 规程5与客户端连接的socket句柄 --- */
char     CThreadPool::ms_str_ThreadPoolErrorMessage[1024];
int      CThreadPool::g_StopFlag;
int      CThreadPool::g_MsgFlag;

/* ------------------------------------------------------
     功能描述:   初始化信号量,成功返回1,失败返回0
--------------------------------------------------------- */
int CThreadPool::Create_Sem()
{
    char          errmsg[500];

    /* --- 给信号量分配内存空间 --- */
    g_SemServer_1 = (sem_t*) calloc (1,sizeof(sem_t));  /* --- 信号量分配空间(同步规程1Accept函数) --- */
    g_SemServer_4 = (sem_t*) calloc (1,sizeof(sem_t));  /* --- 信号量分配空间(同步规程4Accept函数) --- */
    CThreadPool::g_SemParam     =(sem_t *)calloc (1,sizeof(sem_t));  /* --- 信号量分配空间(同步参数队列) --------- */

    /* --- 内存分配失败 --- */
    if (g_SemServer_1 == NULL || g_SemServer_4 == NULL || g_SemParam == NULL)
    {
        memset(errmsg,0x00,sizeof(errmsg));
        strcpy(errmsg,"Critical calloc error\n");
        CLog::WriteCommuLog(errmsg);

        return 0;
    }

    /* --- 初始化信号量 --- */

    sem_init(g_SemServer_1,0,1);
    sem_init(g_SemServer_4,0,1);
    sem_init(g_SemParam,0,1);

    return 1;
}

/* ------------------------------------------------------
      功能描述:   释放信号量
--------------------------------------------------------- */
void CThreadPool::Release_Sem()
{
   
    sem_destroy(g_SemServer_1);
    sem_destroy(g_SemServer_4);
    sem_destroy(CThreadPool::g_SemParam);
    
    free(g_SemServer_1); /* --- 释放信号量(同步规程1Accept函数) --- */
    free(g_SemServer_4); /* --- 释放信号量(同步规程4Accept函数) --- */
    free(g_SemParam);    /* --- 释放信号量(同步参数) --- */
 
    g_SemServer_1 = NULL;
    g_SemServer_4 = NULL;
    g_SemParam    = NULL;

}



/* ------------------------------------------------------
            功能描述:   初始化线程池
            返回值说明: 成功返回1,失败返回0
--------------------------------------------------------- */
int CThreadPool::Initial_ThreadPool()
{
    int           i;
    char          errmsg[500];

    /* --- 分配线程池空间 --- */
    g_Server_1    = (pthread_t*) calloc (MAX_CLIENT_NUMBER1,sizeof(pthread_t));
    g_Server_4    = (pthread_t*) calloc (MAX_CLIENT_NUMBER4,sizeof(pthread_t));
    if (g_Server_1 == NULL || g_Server_4 == NULL)
    {
      memset(errmsg,0x00,sizeof(errmsg));
			strcpy(errmsg,"Initial_ThreadPool calloc error\n");
			CLog::WriteCommuLog(errmsg);
		
      return 0;
    }

    /* --- 规程1线程池各线程句柄初始化 --- */
    for (i = 0; i < MAX_CLIENT_NUMBER1;i++)
    {
        g_Server_1[i] = 0;
    }
    /* --- 规程4线程池各线程句柄初始化 --- */
    for (i = 0; i < MAX_CLIENT_NUMBER4;i++)
    {
        g_Server_4[i] = 0;
    }
 
	
	/* --- 综合监控线程句柄初始化 --- */
	g_ISCSHandle = 0;
  
  return 1;
}


/* ------------------------------------------------------
            功能描述:   创建规程1线程池
            返回值说明: 成功返回1,失败返回0
--------------------------------------------------------- */
int CThreadPool::Create_ThreadPool_SC()
{
    int      ret,i;
    int      *index;

    for(i = 0;i < MAX_CLIENT_NUMBER1; i++)
    {
        index = (int*)malloc(sizeof(int));
        *index = i;
        ret = pthread_create(&g_Server_1[i],NULL,&Thread_Server_SC,(void *)index);
        if (ret != 0)
        {
            return 0;
        }
    }

    return 1;
}
/* ------------------------------------------------------
            功能描述:   创建规程4线程池
            返回值说明: 成功返回1,失败返回0
--------------------------------------------------------- */
int CThreadPool::Create_ThreadPool_ACC()
{
    int      ret,i;
    int      *index;

    for(i = 0;i < MAX_CLIENT_NUMBER4; i++)
    {
        index = (int*)malloc(sizeof(int));
        *index = i;
        ret = pthread_create(&g_Server_4[i],NULL,&Thread_Server_ACC,(void *)index);
        if (ret != 0)
        {
            return 0;
        }
    }
    return 1;
}





/* ------------------------------------------------------
            功能描述:   等待线程池1内线程退出
--------------------------------------------------------- */
void CThreadPool::Wait_ThreadPool_SC_Exit()
{
    pthread_t tmpTid;
    int       i;

    /* --- 给线程池1内线程发送退出信号 --- */
    for(i = 0; i < MAX_CLIENT_NUMBER1; i++)
    {
        tmpTid = g_Server_1[i];
        /* --- 给线程发送退出信号 --- */
        while(g_Server_1[i] != 0)
        {
            pthread_kill(g_Server_1[i],SIGUSR1);
            CommonFuncs::my_usleep(1);
        }
        /* --- 等待线程退出 --- */
        if (tmpTid != 0)
        {
            pthread_join(tmpTid,NULL);
        }
    }
}

/* ------------------------------------------------------
            功能描述:   等待线程池4内线程退出
--------------------------------------------------------- */
void CThreadPool::Wait_ThreadPool_ACC_Exit()
{
    pthread_t tmpTid;
    int       i;

    /* --- 给线程池1内线程发送退出信号 --- */
    for(i = 0; i < MAX_CLIENT_NUMBER4; i++)
    {
        tmpTid = g_Server_4[i];
        /* --- 给线程发送退出信号 --- */
        while(g_Server_4[i] != 0)
        {
            pthread_kill(g_Server_4[i],SIGUSR1);
            CommonFuncs::my_usleep(1);
        }
        /* --- 等待线程退出 --- */
        if (tmpTid != 0)
        {
            pthread_join(tmpTid,NULL);
        }
    }
}


/* ------------------------------------------------------
            功能描述:   等待综合监控线程退出 
--------------------------------------------------------- */
void  CThreadPool::Wait_ISCS_Exit()
{
	pthread_t tmpTid;
	
	tmpTid = g_ISCSHandle;
	if (tmpTid == 0)
	{
		return;
	}
	
	/* --- 给时钟校对线程发送退出信号 --- */
	while(g_ISCSHandle != 0)
	{
		pthread_kill(g_ISCSHandle,SIGUSR1);
		CommonFuncs::my_usleep(1);
	}
	/* --- 等待线程退出 --- */
	if (tmpTid != 0)
	{
		pthread_join(tmpTid,NULL);
	}
}

/* ------------------------------------------------------
     功能描述:   接收来自规程1的客户端连接请求, 连接成功后,
	             处理与客户端的通信
--------------------------------------------------------- */
void* CThreadPool::Thread_Server_SC(void* lpParam)
{
    int          connfd,sequence,ret;
    char         errmsg[500];

    BYTE         *p,buffer[4];
    int          len;
    WORD         MsgType;
    char         sleip[16];
    int 				 port,i,flag;

    /* --- 线程索引 --- */
    sequence = *(int*)lpParam;
    free(lpParam);
    lpParam = NULL;

    for(;;)
    {
        /* --- 接收客户端的连接请求--- */
        ret = sem_wait(g_SemServer_1);
	
        if (ret < 0)
        {
            if (g_StopFlag == 1)
            {
                sem_post(g_SemServer_1); /* --- 释放信号量 --- */

                memset(errmsg,0x00,sizeof(errmsg));
                sprintf(errmsg,"规程1:线程%d exit\n",sequence);
                CLog::WriteCommuLog(errmsg);

                g_Server_1[sequence] = 0;

                return NULL;
            }
            else
            {
                sem_post(g_SemServer_1);
                continue;
            }
        }
        memset(sleip,0x00,sizeof(sleip));
        connfd = CSocket::Tcp_Accept(g_ListenSock[0],sleip,port);
        //如果是长连接的话，需要更新SC列表结构
        //先查找是否有相同的连接句柄存在，若存在，先关闭连接句柄
        //再查找对应的IP,更新SC长连接句柄
        flag = 0;
				if(CConfig::g_Conn_L == 1 && connfd > 0)
				{
					sem_wait(&CCommu::g_CSScList);
					for(i=0;i<MAX_SLE_COUNT;i++)
					{
						if(CConfig::g_ScList[i].sockhandle == connfd)
						{
							memset(errmsg,0x00,sizeof(errmsg));
							sprintf(errmsg,"SC(%s:%s)连接句柄:%d已存在,主动断开,重新接收新的连接\n",CConfig::g_ScList[i].sccode,CConfig::g_ScList[i].scip,CConfig::g_ScList[i].sockhandle);
							CLog::WriteCommuLog(errmsg);
							CSocket::Tcp_Close(CConfig::g_ScList[i].sockhandle);
							CConfig::g_ScList[i].sockhandle = -1;
							flag = 1;
							break;
						}	
					}
					if(flag == 1)
					{
						sem_post(&CCommu::g_CSScList);
						sem_post(g_SemServer_1);
						continue;
					}
					for(i=0;i<MAX_SLE_COUNT;i++)
					{
						if(strcmp(CConfig::g_ScList[i].scip,sleip) == 0)
						{
							if(CConfig::g_ScList[i].sockhandle > 0)
							{
								memset(errmsg,0x00,sizeof(errmsg));
								sprintf(errmsg,"主动断开SC(%s:%s)上一连接,连接句柄:%d\n",CConfig::g_ScList[i].sccode,CConfig::g_ScList[i].scip,CConfig::g_ScList[i].sockhandle);
								CLog::WriteCommuLog(errmsg);
								CSocket::Tcp_Close(CConfig::g_ScList[i].sockhandle);
							}
							CConfig::g_ScList[i].sockhandle = connfd;
								
							break;
						}	
					}
					sem_post(&CCommu::g_CSScList);
				}
				sem_post(g_SemServer_1);
 
        if (connfd < 0)  /* --- accept函数出错--- */
        {
            CLog::WriteCommuLog(CSocket::ms_str_SocketErrorMessage);

            if(g_StopFlag == 1)
            {
                memset(errmsg,0x00,sizeof(errmsg));
                sprintf(errmsg,"规程1:线程%d exit\n",sequence);
                CLog::WriteCommuLog(errmsg);

                g_Server_1[sequence] = 0;

                return NULL;
            }
        }
        else  /* --- 接收成功 --- */
        {
        		memset(errmsg,0x00,sizeof(errmsg));
						sprintf(errmsg,"SC:%s连接SC(%d:%d)成功,连接句柄:%d\n",sleip,CCommu::g_Port1,port,connfd);
						CLog::WriteCommuLog(errmsg);
			
            g_Client_1[sequence] = connfd;
            ret = CCommu::Recv_ScPackage(CConfig::g_Conn_L,connfd,sleip,port);
            g_Client_1[sequence] = 0;
            
            memset(errmsg,0x00,sizeof(errmsg));
						sprintf(errmsg,"SC:%s主动断开连接,连接句柄:%d!ret = %d \n",sleip,connfd,ret);
						CLog::WriteCommuLog(errmsg);
						
						/* --- 允许长连接更新SC列表结构 --- */
						if(CConfig::g_Conn_L == 1)
						{
							sem_wait(&CCommu::g_CSScList);
							for(i=0;i<MAX_SLE_COUNT;i++)
							{
								if(strcmp(CConfig::g_ScList[i].scip,sleip) == 0)
								{
									if(CConfig::g_ScList[i].sockhandle == connfd)
									{
										CConfig::g_ScList[i].sockhandle = -1;
									}
									else
									{
										memset(errmsg,0x00,sizeof(errmsg));
										sprintf(errmsg,"SC:%s使用新的连接句柄:%d!\n",CConfig::g_ScList[i].scip,CConfig::g_ScList[i].sockhandle);
										CLog::WriteCommuLog(errmsg);	
									}
									break;
								}	
							}
							sem_post(&CCommu::g_CSScList);
						}
 
            if(g_StopFlag == 1)
            {
                memset(errmsg, 0x00, sizeof(errmsg));
                sprintf(errmsg, "规程1:线程%d exit\n", sequence);
                CLog::WriteCommuLog(errmsg);
                g_Server_1[sequence] = 0;

                if (ret == -2) {
                    /* --- 通知应用进程重启通信服务 --- */
                    g_MsgFlag   = 2;
                    sem_post(&CMessage::g_SemReadMsg);
                }

                return NULL;
            }
        }
    }

    return NULL;
}
/* ------------------------------------------------------
     功能描述:   接收来自规程4的客户端连接请求, 连接成功后,
	             处理与客户端的通信
--------------------------------------------------------- */
void* CThreadPool::Thread_Server_ACC(void* lpParam) 
{
    int 	connfd, sequence, ret;
    char 	errmsg[500];
    char 	clnip[16];    
    int 	port;

    /* --- 线程索引 --- */
    sequence = *(int *) lpParam;
    free(lpParam);
    lpParam = NULL;

    for (;;) 
    {
        /* --- 接收客户端的连接请求--- */
        ret = sem_wait(g_SemServer_4);
        if (ret < 0) 
        {
            if (g_StopFlag == 1) 
            {
                sem_post(g_SemServer_4); /* --- 释放信号量 --- */

                memset(errmsg, 0x00, sizeof(errmsg));
                sprintf(errmsg, "规程4:线程%d exit\n", sequence);
                CLog::WriteCommuLog(errmsg);

                g_Server_4[sequence] = 0;

                return NULL;
            } 
            else 
            {
                sem_post(g_SemServer_4);
                continue;
            }
        }
        memset(clnip, 0x00, sizeof(clnip));
        port = 0;
        connfd = CSocket::Tcp_Accept(g_ListenSock[1], clnip,port);
        sem_post(g_SemServer_4);

        if (connfd < 0)  /* --- accept函数出错--- */
        {
            CLog::WriteCommuLog(CSocket::ms_str_SocketErrorMessage);

            if (g_StopFlag == 1) {

                memset(errmsg, 0x00, sizeof(errmsg));
                sprintf(errmsg, "规程4:线程%d exit\n", sequence);
                CLog::WriteCommuLog(errmsg);

                g_Server_4[sequence] = 0;

                return NULL;
            }
        } 
        else  /* --- 接收成功 --- */
        {
        		/* --- 接收成功 --- */
						memset(errmsg,0x00,sizeof(errmsg));
						sprintf(errmsg,"ACC:%s连接LCC(%d:%d)成功,连接句柄:%d\n",clnip,CCommu::g_Port4,port,connfd);
						CLog::WriteCommuLog(errmsg);
        	
            g_Client_4[sequence] = connfd;
            ret = CCommu::Recv_ACCPackage(connfd, clnip,port);
            g_Client_4[sequence] = 0;
            
            memset(errmsg,0x00,sizeof(errmsg));
						sprintf(errmsg,"ACC:%s断开连接,连接句柄:%d!\n",clnip,connfd);
						CLog::WriteCommuLog(errmsg);
            
            if (g_StopFlag == 1) {

                memset(errmsg, 0x00, sizeof(errmsg));
                sprintf(errmsg, "规程4:线程%d exit\n", sequence);
                CLog::WriteCommuLog(errmsg);

                g_Server_4[sequence] = 0;
                return NULL;
            }
        }
    }

    return NULL;
}

void* CThreadPool::Thread_Get_ISCS(void* lpParam) 
{ 
	int          ret;
	char         errmsg[500];
	BYTE         ISCSBuf[CLIENT_ISCSPKG_LEN];
	char         clnip[16];
	int          port;
	

	for(;;)
	{
		memset(clnip,0x00,sizeof(clnip));
		port = 0;
		g_Client_5 = CSocket::Tcp_Accept(g_ListenSock[MAX_LISTEN_NUMBER-1],clnip,port);
		
		if (g_Client_5 < 0)  /* --- accept函数出错--- */
		{
			 CLog::WriteCommuLog(CSocket::ms_str_SocketErrorMessage);
			 	
			if(g_StopFlag == 1)
			{				
				memset(errmsg,0x00,sizeof(errmsg));
				strcpy(errmsg,"综合监控线程exit\n");
				CLog::WriteCommuLog(errmsg);
					
				g_ISCSHandle = 0;
 
				return NULL;
			}
		}
		else  /* --- 接收成功 --- */
		{
			/* --- 接收成功 --- */
			memset(errmsg,0x00,sizeof(errmsg));
			sprintf(errmsg,"ISCS:%s连接LCC(%d:%d)成功,连接句柄:%d\n",clnip,CCommu::g_Port5,port,g_Client_5);
			CLog::WriteCommuLog(errmsg);
			
			for(;;)
			{
				struct timeval time_out;
				memset(ISCSBuf,0x00,CLIENT_ISCSPKG_LEN);
				/* --- 设置超时 --- */
				time_out.tv_sec = 30;
				time_out.tv_usec = 0;
				setsockopt(g_Client_5,SOL_SOCKET,SO_RCVTIMEO,(char *)&time_out,sizeof(time_out));
				ret = CCommu::RecvMsg(g_Client_5,(char*)ISCSBuf,CLIENT_ISCSPKG_LEN);
				/* --- 禁止超时 --- */
				time_out.tv_sec = 0;
				time_out.tv_usec = 0;
				setsockopt(g_Client_5,SOL_SOCKET,SO_RCVTIMEO,(char *)&time_out,sizeof(time_out));
				if(ret == SUCCESS)
				{	
					ISCS::DealWithISCSMsg(ISCSBuf,g_Client_5);
				}
				else
				{
					memset(errmsg,0x00,sizeof(errmsg));
					sprintf(errmsg,"ISCS:%s断开连接,连接句柄:%d!\n",clnip,g_Client_5);
					CLog::WriteCommuLog(errmsg);
					
					CSocket::Tcp_Close(g_Client_5);
					break;
				}
				
				if(g_StopFlag == 1)
				{
					memset(errmsg,0x00,sizeof(errmsg));
					strcpy(errmsg,"综合监控线程exit\n");
					CLog::WriteCommuLog(errmsg);
					g_ISCSHandle = 0;
						
					return NULL;
				}
			}
		}
	}
}  


/* ------------------------------------------------------
            功能描述:   释放线程池
--------------------------------------------------------- */
void  CThreadPool::Release_ThreadPool()
{
    /* --- 释放线程池 --- */
    free(g_Server_1);
    free(g_Server_4);

    g_Server_1 = NULL;
    g_Server_4 = NULL;
    
    g_ISCSHandle = 0;
}


/* ------------------------------------------------------
功能描述:   初始化套接字句柄,成功返回1,失败返回0
--------------------------------------------------------- */
int CThreadPool::Create_ListenSock()
{
    int           i;
    char          errmsg[500];

    g_ListenSock = (int*) calloc (MAX_LISTEN_NUMBER,sizeof(int));
    if (g_ListenSock == NULL)
    {
        memset(errmsg,0x00,sizeof(errmsg));
        strcpy(errmsg,"Create_SockHandle error\n");
        CLog::WriteCommuLog(errmsg);
        return 0;
    }

    for (i = 0;i < MAX_LISTEN_NUMBER;i++)
    {
        g_ListenSock[i] = 0;
    }

    return 1;
}


/* ------------------------------------------------------
            功能描述:   停止服务器的侦听
--------------------------------------------------------- */
void CThreadPool::Close_ListenSock()
{
    int i;

    for(i = 0; i < MAX_LISTEN_NUMBER;i++)
    {
        if(g_ListenSock[i] != 0)
            CSocket::Tcp_Close(g_ListenSock[i]);
    }
}


/* ------------------------------------------------------
      功能描述:   释放侦听套接字句柄
--------------------------------------------------------- */
void CThreadPool::Release_ListenSock()
{
    free(g_ListenSock);
    g_ListenSock = NULL;
}


/* ------------------------------------------------------
            功能描述:   四种规程客户连接的初始化
            返回值说明: 成功返回1,失败返回0
--------------------------------------------------------- */
int CThreadPool::Create_ClientSock()
{
    int           i;
    char          errmsg[500];

    /* ------- 分配空间 ----- */
    g_Client_1 = (int*) calloc (MAX_CLIENT_NUMBER1,sizeof(int));
    g_Client_4 = (int*) calloc (MAX_CLIENT_NUMBER4,sizeof(int));
    if (g_Client_1 == NULL || g_Client_4 == NULL)
    {
        memset(errmsg,0x00,sizeof(errmsg));
        strcpy(errmsg,"Tcp_Initial_Clien calloc error\n");
        CLog::WriteCommuLog(errmsg);
        return 0;
    }

    /* ------- 规程1客户初始化 ----- */
    for(i = 0;i < MAX_CLIENT_NUMBER1;i++)
    {
        g_Client_1[i] = 0;
    }
    /* ------- 规程4客户初始化 ----- */
    for(i = 0;i < MAX_CLIENT_NUMBER4;i++)
    {
        g_Client_4[i] = 0;
    }

		g_Client_5 = 0;

    return 1;
}


/* ------------------------------------------------------
            功能描述:   断开所有已连接客户
--------------------------------------------------------- */
void CThreadPool::Close_ClientSock()
{
    int i;

    /* --- 断开规程1已连接客户 --- */
    for (i = 0; i < MAX_CLIENT_NUMBER1; i++)
    {
        if (g_Client_1[i] != 0)
        {
            CSocket::Tcp_Close(g_Client_1[i]);
        }
    }
    /* --- 断开规程4已连接客户 --- */
    for (i = 0; i < MAX_CLIENT_NUMBER4; i++)
    {
        if (g_Client_4[i] != 0)
        {
            CSocket::Tcp_Close(g_Client_4[i]);
        }
    }
    
    CSocket::Tcp_Close(g_Client_5);
		g_Client_5 = 0;
}


/* ------------------------------------------------------
      功能描述:   释放侦听套接字句柄
--------------------------------------------------------- */
void CThreadPool::Release_ClientSock()
{
    free(g_Client_1);
    free(g_Client_4);

    g_Client_1 = NULL;
    g_Client_4 = NULL;
}

/* ------------------------------------------------------
            功能描述:   通信线程初始化(socket、消息队列、
			            消息队列序列号、信号量、日志文件)
            返回值说明: 初始化成功返回1,失败返回0
--------------------------------------------------------- */
int CThreadPool::Server_Initial()
{
    int ret;
    /* --- 侦听套接字初始化 --- */
    ret = Create_ListenSock();
    if (ret == 0)
    {
        return 0;
    }

    /* ---  客户连接套接字初始化 --- */
    ret = Create_ClientSock();
    if (ret == 0)
    {
        Release_ListenSock();
        return 0;
    }

    /* --- 信号量初始化 --- */
    CMessage::Create_Sem();
    ret = Create_Sem();
    if (ret == 0)
    {
        Release_ListenSock();
        Release_ClientSock();
        return 0;
    }

    /* ---消息队列初始化 --- */
		sem_wait(CMessage::g_SemMsg);
		ret = CMessage::Initial_MsgQ();
		sem_post(CMessage::g_SemMsg);
    if (ret == 0)
    {
        Release_ListenSock();
        Release_ClientSock();
        Release_Sem();
        CMessage::MsgQRelease();
        
        return 0;
    }

    /* --- 线程池初始化 --- */
    ret = Initial_ThreadPool();
    if (ret == 0)
    {
        Release_ListenSock();
        Release_ClientSock();

        sem_wait(CMessage::g_SemMsg);
        CMessage::MsgQRelease();
        sem_post(CMessage::g_SemMsg);

        Release_Sem();
        return 0;
    }
    return 1;
}

 

/* ------------------------------------------------------
            功能描述:   创建四种规程的服务器进行侦听
            返回值说明: 创建成功返回1,失败返回0
--------------------------------------------------------- */
int CThreadPool::Server_Start()
{
 
	
    int           ret;
    char          errmsg[500];

    /* --- 创建规程1侦听套接字 --- */
    g_ListenSock[0] = CSocket::Tcp_Listen(CCommu::g_Port1);
    if (g_ListenSock[0] < 0)
    {
        return 0;
    }

    /* --- 创建规程1套接线程池 --- */
    ret = Create_ThreadPool_SC();
    if (ret == 0)
    {
        memset(errmsg,0x00,sizeof(errmsg));
        sprintf(errmsg,"Create_ThreadPool_SC Errors:%d\n",errno);
        CLog::WriteCommuLog(errmsg);
        CCommu::CommunicationStop();
        return 0;
    }


    /* --- 创建规程4侦听套接字 --- */
    g_ListenSock[1] = CSocket::Tcp_Listen(CCommu::g_Port4);
    if (g_ListenSock[1] < 0)
    {
        return 0;
    }

    /* --- 创建规程4套接线程池 --- */
    ret = Create_ThreadPool_ACC();
    if (ret == 0)
    {
        memset(errmsg,0x00,sizeof(errmsg));
        sprintf(errmsg,"Create_ThreadPool_ACC Errors:%d\n",errno);
        CLog::WriteCommuLog(errmsg);
        CCommu::CommunicationStop();
        return 0;
    }


	/* --- 创建综合监控服务器 --- */
	g_ListenSock[MAX_LISTEN_NUMBER-1] = CSocket::Tcp_Listen(CCommu::g_Port5);
	if (g_ListenSock[MAX_LISTEN_NUMBER-1] < 0)
	{
		CCommu::CommunicationStop();
		return 0;
	}
	
	ret = pthread_create(&CThreadPool::g_ISCSHandle,NULL,Thread_Get_ISCS,NULL);
	if (ret != 0)
	{
		memset(errmsg,0x00,sizeof(errmsg));
		sprintf(errmsg,"CreateIscsThread Errors:%d\n",errno);
		CLog::WriteCommuLog(errmsg);
		CCommu::CommunicationStop();
		return 0;
	}

	char  errMsg[1024];
	memset(errMsg,0x00,sizeof(errMsg));
	strcpy(errMsg,"通信动态库监听启动！\n");
	CLog::WriteCommuLog(errMsg);
		
  return 1;
}


/* ------------------------------------------------------
       功能描述:释放消息队列,停止服务器的侦听并且断开所有已连接的客户    
--------------------------------------------------------- */
void CThreadPool::Server_Close( )
{
    /* --- 设置退出侦听服务标志 --- */
    g_StopFlag = 1;
    

    /* --- 释放线程池 --- */
    Wait_ThreadPool_SC_Exit();
    Wait_ThreadPool_ACC_Exit();
    Wait_ISCS_Exit();
    
    Release_ThreadPool();

    /* --- 停止服务器的侦听 --- */
    Close_ListenSock();
    /* --- 断开所有已连接客户 --- */
    Close_ClientSock();
 
    /* --- 释放侦听套接字--- */
    Release_ListenSock();

    /* --- 释放已连接客户套接字--- */
    Release_ClientSock();
 
 		/* --- 释放信号量 --- */
	  Release_Sem();
}


 
sem_t 			ISCS::g_CS_ISCS;
SC_FLOW  	  ISCS::g_SC_FLOW[MAX_SLE_COUNT];
BYTE 				ISCS::g_ISCS_SndPkg[MAX_BAOWEN_LENGTH];
void ISCS::DealWithISCSMsg(BYTE *recvBuf,int connfd)
{	
	BYTE            sendBuf[MAX_BAOWEN_LENGTH],b_sccode[4];
	WORD            count_of_sle,start_addr;
	WORD            package_len,TCP_Num;
	ISCSMsg         m_ISCSMsg;
	char            applog[1024];
	int             i,rtn,nbyte,zhandh;
	unsigned int    dw_flow;
	
	/* -------- 获得参数值 -------*/
	m_ISCSMsg = *(ISCSMsg *)recvBuf;
	TCP_Num = CommonFuncs::wordtowin(m_ISCSMsg.TCP_Num);
	memcpy(&start_addr,recvBuf+CLIENT_ISCS_HEAD_1,2);
	start_addr =  CommonFuncs::wordtowin(m_ISCSMsg.Start_Addr);
	memcpy(&count_of_sle,recvBuf+CLIENT_ISCS_HEAD_2,2);
	count_of_sle = CommonFuncs::wordtowin(m_ISCSMsg.Register_Num);
	
	/* -------- 写入日志 -------*/
	memset(applog,0x00,sizeof(applog));
	sprintf(applog,"接收到综合监控报文(连接句柄 = %d,包序列号 = %d,寄存器数量 = %d,开始地址 = %d)\n",connfd,TCP_Num,count_of_sle,start_addr);
	CLog::WriteCommuLog(applog);
		
	/* --- 记入DEBUG日志 --- */
	if(DEBUG_FLAG)
	{
		CLog::WriteDebugLog(1,recvBuf,CLIENT_ISCSPKG_LEN);
	}
	
	if(m_ISCSMsg.Func_Code == 0x03 || m_ISCSMsg.Func_Code == 0x04)
	{
	}
	else
	{
		CSocket::Tcp_Close(connfd);
		memset(applog,0x00,sizeof(applog));
		strcpy(applog,"功能码错误\n");
		CLog::WriteCommuLog(applog);
		
		/* ----- -- 断开连接 ----*/	
		CSocket::Tcp_Close(connfd);
		return;
	}
	
	if(start_addr*2 + count_of_sle*2 >= (MAX_BAOWEN_LENGTH-SERVER_ISCS_HEAD_1))
	{
		CSocket::Tcp_Close(connfd);
		memset(applog,0x00,sizeof(applog));
		sprintf(applog,"起始地址:%d,寄存器个数:%d超出指定范围\n",start_addr,count_of_sle);
		CLog::WriteCommuLog(applog);	
		
		/* ----- -- 断开连接 ----*/	
		CSocket::Tcp_Close(connfd);
				
		return;
	}
	
	/* ----- -- 填充发送报文 ----*/	
	package_len = SERVER_ISCS_HEAD_1 + count_of_sle*2 - SERVER_ISCS_HEAD_2 - 2;
	memset(sendBuf,0x00,sizeof(sendBuf));
	memcpy(sendBuf,recvBuf,SERVER_ISCS_HEAD_2);
	package_len =  CommonFuncs::wordtowin(package_len);
	memcpy(sendBuf+SERVER_ISCS_HEAD_2,&package_len,2);
	memcpy(sendBuf+SERVER_ISCS_HEAD_2+2,recvBuf+SERVER_ISCS_HEAD_2+2,2);
	if(count_of_sle<= 123 )
		sendBuf[SERVER_ISCS_HEAD_1-1] = count_of_sle*2;
	memcpy(sendBuf+SERVER_ISCS_HEAD_1,(char*)g_ISCS_SndPkg+start_addr*2,count_of_sle*2);
	
	/* ----- -- 发送报文 ----*/	
	sendBuf[8] = count_of_sle * 2;
	rtn = CSocket::Tcp_Send(connfd,(char*)sendBuf,SERVER_ISCS_HEAD_1+count_of_sle*2);
	if(rtn == 1)
	{
		/* ----- -- 写入日志 ----*/	
		memset(applog,0x00,sizeof(applog));
		sprintf(applog,"发送综合监控应答报文成功(连接句柄 = %d,包序列号 = %d,报文长度 = %d)\n",connfd,TCP_Num,SERVER_ISCS_HEAD_1+count_of_sle*2);
		CLog::WriteCommuLog(applog);
		/* --- 记入DEBUG日志 --- */
		if(DEBUG_FLAG)
		{
			CLog::WriteDebugLog(1,sendBuf,SERVER_ISCS_HEAD_1+count_of_sle*2);
		}
	}	
	else
	{
		/* ----- -- 断开连接 ----*/	
		CSocket::Tcp_Close(connfd);
		
		memset(applog,0x00,sizeof(applog));
		sprintf(applog,"发送综合监控应答报文失败(连接句柄 = %d,包序列号 = %d,报文长度 = %d)\n",connfd,TCP_Num,SERVER_ISCS_HEAD_1+count_of_sle*2);
		CLog::WriteCommuLog(applog);
	}
}



/* ------------------------------------------------------
      功能描述:   更新设备状态
--------------------------------------------------------- */
void ISCS::UpdateDeviceStatus(BYTE *p,int eventlevel)
{
	int  sletype;
	int  zhandh,shebh;
	int  nbyte;
	
	char stationNid[5];
	char errMsg[100];
		
	memset(stationNid,0x00,sizeof(stationNid));
	sprintf(stationNid,"%02X%02X",p[0],p[1]);
	
	int index = ISCS::GetIscsStationInfo(stationNid);
	memset(errMsg,0x00,sizeof(errMsg));
	sprintf(errMsg,"'UpdateDeviceStatus' stationNid[%s],index[%d]\n",stationNid,index);
	CLog::WriteCommuLog(errMsg); 	
	if(index == -1)
	{
		return;
	}	
 
	/* --- 获得设备类型 ---*/
	sletype = *(p+2);
	/* --- 获得设备编号 ---*/
	shebh = *(p+3);
	/* --- 获得站点号 ---*/
	//zhandh = *(p+1);
	zhandh = CConfig::ms_IscsStation[index].ISCS_STATION_ID;
	
	if(sletype == 0x31 || sletype == 0x32 || sletype == 0x33 || sletype == 0x34)/* --- gate ---*/
	{
		nbyte = zhandh * ISCS_STATION_BYTES + CConfig::ms_IscsStation[index].ISCS_AGM_START_BYTE
						+ (shebh - CConfig::ms_IscsStation[index].ISCS_AGM_START_OFFSET) * 2 - 1;
	}
	else if(sletype == 0x21)/* --- bom ---*/
	{
		nbyte = zhandh * ISCS_STATION_BYTES + CConfig::ms_IscsStation[index].ISCS_BOM_START_BYTE
						+ (shebh - CConfig::ms_IscsStation[index].ISCS_BOM_START_OFFSET) * 2 - 1;
	}
	else if(sletype == 0x11)/* --- tvm ---*/
	{
		nbyte = zhandh * ISCS_STATION_BYTES + CConfig::ms_IscsStation[index].ISCS_TVM_START_BYTE
						+ (shebh - CConfig::ms_IscsStation[index].ISCS_TVM_START_OFFSET) * 2 - 1;		
	}
	else if(sletype == 0x41)/* --- tcm ---*/
	{
		nbyte = zhandh * ISCS_STATION_BYTES + CConfig::ms_IscsStation[index].ISCS_TCM_START_BYTE
						+ (shebh - CConfig::ms_IscsStation[index].ISCS_TCM_START_OFFSET) * 2 - 1;		
	}
	else
		return;
	if(nbyte < 0 || nbyte > 8000)
		return;
		/* bit desc
		   0   0 离线     1 在线
		   1   0 暂停服务 1 正常服务
		   2   0 无报警   1 有报警
		   3   0 无故障   1 有故障 */
	if(eventlevel == 0)
	{
		g_ISCS_SndPkg[nbyte] = 0x03; /* --- 0011 ---*/
	}
	else if(eventlevel == 1)
	{
		g_ISCS_SndPkg[nbyte] = 0x07;/* --- 0111 ---*/
	}
	else if(eventlevel == 4)
	{
		g_ISCS_SndPkg[nbyte] = 0x01;/* --- 0001 ---*/
	}
	else if(eventlevel == 2)
	{
		g_ISCS_SndPkg[nbyte] = 0x07;/* --- 0111 ---*/
	}
	else if(eventlevel == 3)
	{
		g_ISCS_SndPkg[nbyte] = 0x0F; /* --- 1111 ---*/
	}
	else
	{
		g_ISCS_SndPkg[nbyte] = 0x02; /* --- 0010 ---*/
	}
	

	memset(errMsg,0x00,sizeof(errMsg));
	sprintf(errMsg,"zhandh:[%d] nbyte:[%d] nreg:[%d] g_ISCS_SndPkg[%d]:[%d] \n",zhandh,nbyte,nbyte/2,nbyte,g_ISCS_SndPkg[nbyte]);
	CLog::WriteCommuLog(errMsg); 
}

/* ------------------------------------------------------
      功能描述:   更新车站运营模式
--------------------------------------------------------- */
void 	ISCS::UpdateStationRunMode(char* stationNid,BYTE modeCode)
{
	int  		nbyte;
	char  	errMsg[100];
	
	int index = ISCS::GetIscsStationInfo(stationNid);
	memset(errMsg,0x00,sizeof(errMsg));
	sprintf(errMsg,"'UpdateStationRunMode' stationNid[%s],index[%d]\n",stationNid,index);
	CLog::WriteCommuLog(errMsg); 	
	if(index == -1)
	{
		return;
	}	
	
	nbyte = CConfig::ms_IscsStation[index].ISCS_STATION_ID * ISCS_STATION_BYTES
			+ CConfig::ms_IscsStation[index].ISCS_RUNMODE_START_BYTE + 1;
	
	
	if(nbyte < 0 || nbyte > 8000)
		return;
	
	g_ISCS_SndPkg[nbyte] = modeCode;
	
	memset(errMsg,0x00,sizeof(errMsg));
	sprintf(errMsg,"zhandh:[%d] nbyte:[%d] nreg:[%d] g_ISCS_SndPkg[%d]:[%d] \n",CConfig::ms_IscsStation[index].ISCS_STATION_ID ,nbyte,nbyte/2,nbyte,g_ISCS_SndPkg[nbyte]);
	CLog::WriteCommuLog(errMsg); 
}

void 		ISCS::UpdateIscsStatusSc(char* stationNid,int isConnect)
{
	int  		nbyte;
	char  	errMsg[100];
	
	int index = ISCS::GetIscsStationInfo(stationNid);
	memset(errMsg,0x00,sizeof(errMsg));
	sprintf(errMsg,"'UpdateIscsStatusSc' stationNid[%s],index[%d]\n",stationNid,index);
	CLog::WriteCommuLog(errMsg); 	
	if(index == -1)
	{
		return;
	}	
	
	nbyte = CConfig::ms_IscsStation[index].ISCS_STATION_ID * ISCS_STATION_BYTES
			+ CConfig::ms_IscsStation[index].ISCS_SNC_START_BYTE + 1;
	
	if(isConnect == 0) 
	{
			g_ISCS_SndPkg[nbyte] = (BYTE) 0x00; /*--离线--*/		
	}
	else	if(isConnect == 1) 
	{
			g_ISCS_SndPkg[nbyte] = (BYTE) 0x01; /*--在线-*/
	}			
	
	memset(errMsg,0x00,sizeof(errMsg));
	sprintf(errMsg,"zhandh:[%d] nbyte:%d nreg:[%d] isConnect:[%d] g_ISCS_SndPkg[%d]:[%d] \n",CConfig::ms_IscsStation[index].ISCS_STATION_ID,nbyte,nbyte/2,isConnect,nbyte,g_ISCS_SndPkg[nbyte]);
	CLog::WriteCommuLog(errMsg); 
}
 
void ISCS::updateIscsFlow()
{	
	BYTE            sendBuf[MAX_BAOWEN_LENGTH],b_sccode[4];

	char            applog[1024];
	int             i,rtn,nbyte,zhandh;
	unsigned int    dw_flow;
	char stationNid[5];
	int index;
 
	/* -------- 更新客流数据 -------*/
	for(i=0;i<MAX_SLE_COUNT;i++)
	{
		if(CConfig::g_ScList[i].scport > 0)
		{
			memset(stationNid,0x00,sizeof(stationNid));
			memcpy(stationNid,CConfig::g_ScList[i].sccode,4);

			index = ISCS::GetIscsStationInfo(stationNid);
			memset(applog,0x00,sizeof(applog));
			sprintf(applog,"'updateIscsFlow' stationNid[%s],index[%d]\n",stationNid,index);
			CLog::WriteCommuLog(applog); 	
			if(index == -1)
			{
				break;
			}					
					
			/* --- 获得站点号 ---*/
			zhandh = CConfig::ms_IscsStation[index].ISCS_STATION_ID;

			sem_wait(&ISCS::g_CS_ISCS);
				
			nbyte = zhandh * ISCS_STATION_BYTES + CConfig::ms_IscsStation[index].ISCS_EPF_START_BYTE;
			dw_flow = CommonFuncs::dwordtowin(g_SC_FLOW[i].EntryFlow);
			memcpy(g_ISCS_SndPkg+nbyte,&dw_flow,4);

			nbyte = zhandh * ISCS_STATION_BYTES + CConfig::ms_IscsStation[index].ISCS_XPF_START_BYTE;
			dw_flow = CommonFuncs::dwordtowin(g_SC_FLOW[i].ExitFlow);
			memcpy(g_ISCS_SndPkg+nbyte,&dw_flow,4);
			
			nbyte = zhandh * ISCS_STATION_BYTES + CConfig::ms_IscsStation[index].ISCS_SPF_START_BYTE;
			dw_flow = CommonFuncs::dwordtowin(g_SC_FLOW[i].SJTFlow);
 			memcpy(g_ISCS_SndPkg+nbyte,&dw_flow,4);
 			
			sem_post(&ISCS::g_CS_ISCS);
		}
	}
}

/*-----------
寻找原车站到综合监控车站列表的相关信息，寻找成功返回下标，
否则返回-1
----------*/
int 		ISCS::GetIscsStationInfo(char* stationNid)
{
	int index = -1;
	
	for(int i=0;i<MAX_SLE_COUNT;i++)
	{
		if(CConfig::ms_IscsStation[i].ISCS_STATION_ID > 0)
		{
			if(strcmp(stationNid,CConfig::ms_IscsStation[i].AFC_STATION_NID) == 0)
			{
				index = i;
				break;
			}			
		}		
	}
	
	char errMsg[500];
	memset(errMsg,0x00,sizeof(errMsg));
	sprintf(errMsg,"'GetIscsStationInfo' stationNid[%s],index[%d].\n",stationNid,index);
	CLog::WriteCommuLog(errMsg); 	
	
	return index;	
}  

/*------16进制转10进制---0x20转化0d20-*/ 
/*输入最好为0x2222等，不含有字符*/
int     ISCS::HexToDec(int zhandh)
{
	int decInt = 0;
	char *stopstring;
	char  c_temp[9];
	memset(c_temp,0x00,sizeof(c_temp));
	sprintf(c_temp,"%08X",zhandh);
	decInt = strtoul(c_temp,&stopstring,10);	
	return decInt;
}
    
void ISCS::IscsInit()   //综合监控初始化 
{
	sem_init(&g_CS_ISCS,0,1);
	
	
	memset(g_ISCS_SndPkg,0x00,sizeof(g_ISCS_SndPkg));

	for(int i=0;i<MAX_SLE_COUNT;i++)
	{
		g_SC_FLOW[i].EntryFlow = 0;
		g_SC_FLOW[i].ExitFlow = 0;
		g_SC_FLOW[i].SJTFlow = 0;
	}	
}
 
void ISCS::IscsRelease()   //综合监控销毁 
{
	sem_destroy(&g_CS_ISCS);
}

 

 




int            CCommu::g_Port1;                   /* ------ 规程1侦听端口号 ----------------------------- */
int            CCommu::g_Port4;                   /* ------ 规程4侦听端口号 ----------------------------- */
int            CCommu::g_Port5;  									/* ------ 规程5侦听端口号 ----------------------------- */
sem_t          CCommu::g_CSScList; 
 
/* ------------------------------------------------------
            功能描述:   主线程函数(获得应用进程的句柄、
			            初始化、启动服务器进行侦听
            返回值说明: 成功返回1,失败返回0
--------------------------------------------------------- */
int CCommu::CommunicationMain(int port1,int port4,int port5)
{
   
 	int	 ret = 0;
	char errMsg[1024];
 
	/* --- 规程1的端口号 --- */
	g_Port1 = port1;
	
	/* --- 规程4的端口号 --- */
	g_Port4 = port4;

 	/* --- 规程5的端口号 --- */
	g_Port5 = port5;

 	sem_init(&g_CSScList,0,1);
 	
	/* --- 通信主线程初始化 --- */
	ret = CThreadPool::Server_Initial();
	if (ret == 0)
	{
		 memset(errMsg,0x00,sizeof(errMsg));
     strcpy(errMsg,"通信主线程初始化失败！\n");
     CLog::WriteAppLog(errMsg);
         
		return SERVER_INITIAL_ERROR;
	}
 

 	//g_InitialServer = 1;

	/* --- 初始化退出侦听服务标志 --- */
	CThreadPool::g_StopFlag = 0;
	/* --- 初始化报文类型变量 --- */
	CThreadPool::g_MsgFlag = -1;

	memset(errMsg,0x00,sizeof(errMsg));
	strcpy(errMsg,"通信动态库初始化完成！\n");
	CLog::WriteCommuLog(errMsg);
     	
	return SUCCESS;
}

 
/* ------------------------------------------------------------------
    功能描述:   释放消息队列,停止服务器的侦听并且断开所有已连接的客户,
    						并且释放信号量，释放内存
--------------------------------------------------------------------- */
void CCommu::CommunicationStop(void)
{
		CThreadPool::Server_Close();
			
		char errmsg[100];
		memset(errmsg,0x00,sizeof(errmsg));
		strcpy(errmsg,"通信动态库退出\n");
		CLog::WriteCommuLog(errmsg);
	
	
   /* --- 释放消息队列 --- */
    sem_wait(CMessage::g_SemMsg);
    CMessage::MsgQRelease();
    sem_post(CMessage::g_SemMsg);
    	
    /* --- 释放信号量 --- */
    CMessage::Release_Sem();    
    	
    sem_destroy(&g_CSScList);		
}

/* ------------------------------------------------------
   功能描述:  接收客户端的一个请求报文
   参数说明:  sockfd: socket句柄 
              buffer: 存储报文的内容
              msg_len:报文的长度			 
   返回值说明:成功返回1,失败返回错误代码
--------------------------------------------------------- */
int CCommu::RecvMsg(int sockfd,char *buffer,int msg_len)
{
	int nleft,nrcv;

	nleft = msg_len; // nleft:  未接收的的字节数
	while(nleft > 0)
	{
		nrcv = CSocket::Tcp_Recv(sockfd,buffer,nleft);
		if (nrcv <= 0)
		{
			return nrcv;
		}
		else
		{
			nleft -= nrcv;
			buffer = buffer + nrcv;
		}
	}
	return 1;
}
 
/* ------------------------------------------------------
   功能描述:  接收来自客户端的参数报文
   参数说明:  connfd: 已连接句柄句柄 
   返回值说明:客户端关闭返回0,接收到KILL信号返回-1,
              其他返回错误代码
--------------------------------------------------------- */
int CCommu::Recv_ACCPackage(int sockfd,char *clnip,int port) 
{ 
	int     ret;

	for(;;)
	{	
		/* --- 接收来自客户端的请求报文--- */
		ret = Prepare_ACCRecvPackage(sockfd,clnip,port);
		if(ret >= 0)        /* --- --- */
		{
			continue;
		}
		else               /* --- 连接中断 --- */
		{
			return ret;
		}
	}
}


/* ------------------------------------------------------
   功能描述:  接收来自中央的参数报文,放入缓冲区。
   参数说明:  sockfd: socket句柄 
			  message_len:报文的长度
   返回值说明:成功返回1,客户端断开连接返回0,
              失败返回错误代码
--------------------------------------------------------- */
int CCommu::Prepare_ACCRecvPackage(int sockfd,char *clnip,int port)
{
	int             ret;
	char            *ParamBuffer;
	int             ParamLength;
	char            errmsg[500];
	struct timeval  time_out;

	/* --- 设置超时 --- */
	time_out.tv_sec = 300;
	time_out.tv_usec = 0;
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&time_out,sizeof(time_out));
	/* --- 获得报文的长度Msg_Length --- */
	ParamBuffer = (char*)malloc(MAX_BAOWEN_LENGTH);
	memset(ParamBuffer,0x00,MAX_BAOWEN_LENGTH );
	ParamLength = GetMsgLen(sockfd,(unsigned char*)ParamBuffer);
		
	/* --- 禁止超时 --- */
	time_out.tv_sec = 0;
	time_out.tv_usec = 0;
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&time_out,sizeof(time_out));
	if (ParamLength < 0 || ParamLength == 0)
	{
		CSocket::Tcp_Close(sockfd);
		return -2;
	}
	if (ParamLength >= 8000)
	{
		memset(errmsg,0x00,sizeof(errmsg));
		sprintf(errmsg,"连接句柄:%d(%s:%d)报文太大!\n",sockfd,clnip,port);
		CLog::WriteCommuLog(errmsg);
		CSocket::Tcp_Close(sockfd);
		return -2;
	}
	/* --- 设置超时 --- */
	time_out.tv_sec = 30;
	time_out.tv_usec = 0;
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&time_out,sizeof(time_out));
	/* --- 接收一个完整的报文 --- */
	ret = RecvMsg(sockfd,ParamBuffer+sizeof(MsgHead),ParamLength-sizeof(MsgHead));
	/* --- 禁止超时 --- */
	time_out.tv_sec = 0;
	time_out.tv_usec = 0;
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&time_out,sizeof(time_out));
	
	if (ret <= 0)
	{
		CSocket::Tcp_Close(sockfd);
		free(ParamBuffer);
		ParamBuffer = NULL;
		return ret;
	}
		
	/* --- 把报文放进消息队列 --- */
	if(CThreadPool::g_StopFlag == 1)/* ------- 消息队列已满 ------- */
	{
		CSocket::Tcp_Close(sockfd);
		ret = -2;
	}
	else
	{
		sem_wait(CMessage::g_SemMsg);        /* --- 获得信号量 --- */	
		ret = CMessage::MsgWrite(sockfd,1,ParamBuffer,ParamLength,clnip,port);
		sem_post(CMessage::g_SemMsg); 
		if(ret == 1)
		{
			CThreadPool::g_MsgFlag = 0;
			sem_post(&CMessage::g_SemReadMsg);
		}
	}

	free(ParamBuffer);
	ParamBuffer = NULL;
	
	return ret;
}  

/* ------------------------------------------------------
功能描述:  接收CC应答报文
参数说明:  sockfd: socket句柄 
buffer: 接收缓冲区
返回值说明:成功返回接收到的长度,错误返回0
--------------------------------------------------------- */
int CCommu::RecvACCPackage_Client(int sockfd,unsigned char *buffer,int timedout)
{
	int             ret;
	int             PackageLength;
	struct timeval  stru_timeval; 

	/* --- 设置超时 --- */
	stru_timeval.tv_sec = timedout;
	stru_timeval.tv_usec = 0;
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&stru_timeval,sizeof(stru_timeval));
	PackageLength = GetMsgLen(sockfd,buffer);
	
	/* --- 禁止超时 --- */
	stru_timeval.tv_sec = 0;
	stru_timeval.tv_usec = 0;
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&stru_timeval,sizeof(stru_timeval));
	if (PackageLength == 0)
	{
		return 0;
	}
	if (PackageLength == TCP_ERROR_ECONNRESET)
	{
		return 0;
	}
	if (PackageLength < 0 )
	{
		return -3;
	}
	/* --- 报文太大 --- */
	if(PackageLength > MAX_BAOWEN_LENGTH)
	{
		return -1;	
	}
	/* --- 报文格式不合法 --- */
	if(PackageLength < LEN_OF_PackageHead+LEN_OF_PackageTail)
	{
		return -2;
	}
	/* --- 设置超时 --- */
	stru_timeval.tv_sec = 5;
	stru_timeval.tv_usec = 0;
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&stru_timeval,sizeof(stru_timeval));
	ret = RecvMsg(sockfd,(char*)buffer+sizeof(MsgHead),PackageLength - sizeof(MsgHead));
	/* --- 禁止超时 --- */
	stru_timeval.tv_sec = 0;
	stru_timeval.tv_usec = 0;
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&stru_timeval,sizeof(stru_timeval));
	if(ret <= 0)
	{
		return -3;
	}
	return PackageLength;
}




/* ------------------------------------------------------
   功能描述:  获得报文的长度
   参数说明:  sockfd: socket句柄 
   返回值说明:成功返回报文的长度,客户端断开连接返回0,
              失败返回错误代码
--------------------------------------------------------- */
int CCommu::GetMsgLen(int sockfd,unsigned char *buffer)
{
	int            n;
	int            AckMsgLen;
	BYTE           *p;
	
	n = RecvMsg(sockfd,(char*)buffer,LEN_OF_PackageHead);
	if (n < 0)       /* --- recv函数调用失败 --- */
	{
		AckMsgLen = CSocket::Tcp_GetSocketErrorMsg("recv",errno);
	}
	else if (n > 0)
	{
		p = (BYTE*)(buffer+30);
		AckMsgLen = (*p << 24) +(*(p+1) << 16)+(*(p+2) << 8)+(*(p+3));
		AckMsgLen = AckMsgLen + LEN_OF_PackageTail + LEN_OF_PackageHead;
	}
	else
	{
		AckMsgLen = 0;
	}

	return AckMsgLen;
}


/* ------------------------------------------------------
   功能描述:  接收来自客户端的请求报文
   返回值说明:客户端关闭返回0,接收到KILL信号返回-1,
              其他返回错误代码
--------------------------------------------------------- */
int CCommu::Recv_ScPackage(int flag,int connfd,char *clnip,int port) 
{ 
	int     recv_len;
	char    errmsg[500];

	for(;;)
	{	
		/* --- 接收来自客户端的请求报文--- */
		recv_len = DealScMsg(flag,connfd,clnip,port);
		if (recv_len == -1)     /* --- 消息队列写入出错 --- */
		{
			memset(errmsg,0x00,sizeof(errmsg));
			sprintf(errmsg,"SOCKET连接:%d写入消息队列出错!\n",connfd);
			CLog::WriteCommuLog(errmsg);
			continue;
		}
		else if (recv_len == 0) /* --- 客户端关闭或者消息队列已满 --- */
		{
			return 0;
		}
		else if (recv_len < 0) /* --- recv函数出错 --- */
		{
			return recv_len;
		}
	}
} 

/* ------------------------------------------------------
   功能描述:  接收来自客户端的请求报文,放入消息队列。
              首先调用 GetClnAckMsgLen函数读取头两个
			  字节,获得该报文的长度,如果大于
			  MAX_MESSAGE_LENGTH,说明是多包请求
   参数说明:  sockfd: socket句柄 
			  Msg_Length:报文的长度
   返回值说明:成功返回1,客户端断开连接返回0,
              失败返回错误代码
--------------------------------------------------------- */
int CCommu::DealScMsg(int flag,int sockfd,char *clnip,int port)
{
	int             Msg_Length;
	unsigned char   PackageHead[LEN_OF_PackageHead];
	int             ret;
	char            errmsg[500];
	struct timeval  time_out;

	if(flag == 0)
	{
		/* --- 设置超时 --- */
		time_out.tv_sec = 300;
		time_out.tv_usec = 0;
		setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&time_out,sizeof(time_out));
	}
	
	/* --- 获得报文的长度Msg_Length --- */
	memset(PackageHead,0x00,LEN_OF_PackageHead);
	Msg_Length = GetMsgLen(sockfd,PackageHead);
	
	if(flag == 0)
	{
		/* --- 禁止超时 --- */
		time_out.tv_sec = 0;
		time_out.tv_usec = 0;
		setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&time_out,sizeof(time_out));
	}
	if (Msg_Length < 0 || Msg_Length == 0)
	{
		CSocket::Tcp_Close(sockfd);
		return Msg_Length;
	}
	if (Msg_Length >= 8000)
	{
		memset(errmsg,0x00,sizeof(errmsg));
		sprintf(errmsg,"连接句柄:%d(%s:%d)报文太大!\n",sockfd,clnip,port);
		CLog::WriteCommuLog(errmsg);
		CSocket::Tcp_Close(sockfd);
		return 0;
	}
	/* --- 通信线程准备退出 --- */
	if(CThreadPool::g_StopFlag == 1)
	{
		CSocket::Tcp_Close(sockfd);
		return 0;
	}
	/* --- 消息队列已满 ---- */
	sem_wait(CMessage::g_SemMsg);

	if (CMessage::g_MsgQ->msg_qnum  > MAX_MSG_NUMBER)
	{
		memset(errmsg,0x00,sizeof(errmsg));
		strcpy(errmsg,"消息队列已满,重新初始化!\n");
		CLog::WriteCommuLog(errmsg);
		CLog::WriteAppLog(errmsg);
		CSocket::Tcp_Close(sockfd);

		CMessage::MsgQRelease();
		CMessage::Initial_MsgQ();		
			
		sem_post(CMessage::g_SemMsg);

		return 0;
	}
	sem_post(CMessage::g_SemMsg);

	ret = PrepareWriteMsgQ(sockfd,PackageHead,Msg_Length,clnip,port);

	if (ret == 1)   /* --- 处理成功,通知应用进程 --- */
	{
		CThreadPool::g_MsgFlag = 1;
		sem_post(&CMessage::g_SemReadMsg);
	}
	return ret;
}

/* ------------------------------------------------------
   功能描述:  处理单包请求
   参数说明:  sockfd: socket句柄 
              strMsgLen: 报文的头两个字节
              msg_len:报文的长度
   返回值说明:成功返回1,失败返回错误代码
--------------------------------------------------------- */
int CCommu::PrepareWriteMsgQ(int sockfd,unsigned char *PackageLenBuffer,int message_len,char *clnip,int port)
{
	int            ret;
	char           *buffer;
	struct timeval time_out;

	buffer = (char*)malloc(MAX_BAOWEN_LENGTH);
	memset(buffer,0x00,MAX_BAOWEN_LENGTH);
	memcpy(buffer,PackageLenBuffer,LEN_OF_PackageHead);
	
	/* --- 设置超时 --- */
	time_out.tv_sec = 30;
	time_out.tv_usec = 0;
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&time_out,sizeof(time_out));
	
	/* --- 接收一个完整的报文 --- */
	ret = RecvMsg(sockfd,buffer+sizeof(MsgHead),message_len-sizeof(MsgHead));
	/* --- 禁止超时 --- */
	time_out.tv_sec = 0;
	time_out.tv_usec = 0;
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&time_out,sizeof(time_out));
	if (ret <= 0)
	{
		CSocket::Tcp_Close(sockfd);
		free(buffer);
		buffer = NULL;
		return ret;
	}
	else
	{
		/* --- 把报文放进消息队列 --- */
		if(CThreadPool::g_StopFlag == 1)/* ------- 消息队列已满 ------- */
		{
			CSocket::Tcp_Close(sockfd);
			free(buffer);
			buffer = NULL;
			return 0;
		}
		sem_wait(CMessage::g_SemMsg);        /* --- 获得信号量 --- */	
		ret = CMessage::MsgWrite(sockfd,2,buffer,message_len,clnip,port);
		sem_post(CMessage::g_SemMsg);        /* --- 释放信号量 --- */	
	}
	free(buffer);
	buffer = NULL;
	return ret;
}



/* ------------------------------------------------------
功能描述:   生成应答包头  
--------------------------------------------------------- */
void CCommu::CreateAckPkgHead(int pkg_len,WORD msgtype,MsgHead *m_OldPackageHead,MsgHead *m_NewPackageHead)
{
	memcpy(m_NewPackageHead,m_OldPackageHead,LEN_OF_PackageHead);
	memcpy(m_NewPackageHead->b_SourceID,CConfig::g_CCIDCode,sizeof(CConfig::g_CCIDCode));
	memcpy(m_NewPackageHead->b_DestID,m_OldPackageHead->b_SourceID,sizeof(m_OldPackageHead->b_SourceID));
	m_NewPackageHead->w_MsgCode = CommonFuncs::wordtowin(msgtype);
	m_NewPackageHead->b_IsACK = 0x02;
	m_NewPackageHead->b_ZipType = 0x01;
	m_NewPackageHead->dw_ZipPkgLen = 0;
	m_NewPackageHead->b_EncryptType = 0x01;
	memset(m_NewPackageHead->b_EncryptCRC,0x00,sizeof(m_NewPackageHead->b_EncryptCRC));
	m_NewPackageHead->dw_PkgLen = CommonFuncs::dwordtowin(pkg_len);
}

/* ------------------------------------------------------
            功能描述:   给CC发送控制命令报文应答
--------------------------------------------------------- */
int CCommu::SendtoACK(int sockfd,int msgtype,MsgHead *m_OldPackageHead,char *PackageBody,unsigned short LenofPackageBody)
{
	BYTE				*Package;
	int					ret,length;
	MsgHead   	m_ack_msghead;
	char        filename[MAX_PATH];
	
	/* --- 为报文分配空间 --- */
	length = LEN_OF_PackageHead + LenofPackageBody + LEN_OF_PackageTail;
	Package = (BYTE*) malloc(length * sizeof(BYTE));
	memset(Package,0x00,length);
	
	/* --- 生成报文头 --- */
	memset(&m_ack_msghead,0x00,LEN_OF_PackageHead);
	CreateAckPkgHead(LenofPackageBody,msgtype,m_OldPackageHead,&m_ack_msghead);
		
	/* --- 生成报文 --- */
	memcpy(Package,&m_ack_msghead,LEN_OF_PackageHead);
	memcpy(Package+LEN_OF_PackageHead,PackageBody,LenofPackageBody);
	
	/* --- 生成CRC32校验 --- */
	unsigned int ul_crc;
	Crc::Init_CRC32_Table();
	ul_crc = Crc::Crc32((char*)Package,LEN_OF_PackageHead+LenofPackageBody,0);
	ul_crc = 	CommonFuncs::dwordtowin(ul_crc);
	memcpy(Package+LEN_OF_PackageHead+LenofPackageBody,&ul_crc,LEN_OF_PackageTail);
	
	/* --- 发送应答 --- */
	ret = CSocket::Tcp_Send(sockfd,(char*)Package,length);
	
  /*---发送应答报文内容-------------*/ 
  char oplogmsg[1024];
	memset(oplogmsg,0x00,sizeof(oplogmsg));
	sprintf(oplogmsg, "连接句柄:%d,报文类型:0x%04X的应答报文(长度:%d),内容如下\n",sockfd,msgtype,length);
	CLog::WriteAppLog(oplogmsg);
	CLog::WriteDebugLog(0,(unsigned char*)Package,length);
	
	/* --- 释放报文所占空间 --- */
	free(Package);
	Package = NULL;

	return ret;
 
}
 
/* ------------------------------------------------------
            功能描述:   判断连接是否是长连接，
            长连接返回1;短连接返回0;
--------------------------------------------------------- */
int CCommu::SleIsLConn(char *ip,int port,int conn_sockhandle)
{
	int sockfd;
	
	if(CConfig::g_Conn_L == 0)/* --- 短连接 --- */
		return 0;
	if(conn_sockhandle <= 0)/* --- 短连接 --- */
		return 0;
	
	sockfd = CSocket::Tcp_Connect(ip,port,1000);
	if(sockfd > 0)/* --- 短连接 --- */
	{
		CSocket::Tcp_Close(sockfd);
		return 0;
	}
	
	return 1;/* --- 长连接 --- */
}
/* ------------------------------------------------------
   功能描述:  接收SLE应答报文
   参数说明:  sockfd: socket句柄 
              buffer: 接收缓冲区
              recv_time_out:等待应答时间
   返回值说明:成功返回接收到的长度,服务器端断开连接返回0,
              失败返回错误代码
--------------------------------------------------------- */
int CCommu::RecvACKPackage(int sockfd,char *buffer,int timedout,int buflen)
{
	struct timeval      timeout,timeout1; 
	fd_set              read_fd;
	int                 AckMessageLen;
	int                 ret;
	char                errormsg[100];

	if(sockfd < 0)
	{
		memset(errormsg,0x00,sizeof(errormsg));
		sprintf(errormsg,"RecvACKPackage error,sockfd = %d\n",sockfd);
		CLog::WriteCommuLog(errormsg);
		
		return -1;
	}
		
	/* --- 给接收设置超时 --- */
	FD_ZERO(&read_fd);
	FD_SET((unsigned int)sockfd,&read_fd);
	timeout.tv_sec = timedout;
	timeout.tv_usec = 0;
	
	ret = select(sockfd+1,&read_fd,NULL,NULL,&timeout);
	if (ret > 0)  /* --- 接收缓冲区有消息可读 --- */
	{
		/* --- 设置超时 --- */
		timeout1.tv_sec = timedout;
		timeout1.tv_usec = 0;
		setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout1,sizeof(timeout1));
	
		/* --- 获得报文的长度Msg_Length --- */
		AckMessageLen = GetMsgLen(sockfd,(unsigned char*)buffer);
		if (AckMessageLen <= 0)
		{
			CSocket::Tcp_Close(sockfd);
			return AckMessageLen;
		}
		if(AckMessageLen > buflen)
		{
			memset(errormsg,0x00,sizeof(errormsg));
			sprintf(errormsg,"连接句柄:%d(RecvACKPackage)报文太大!\n",sockfd);
			CLog::WriteCommuLog(errormsg);
			CSocket::Tcp_Close(sockfd);
			return -100;
		}
		/* --- 获得报文内容 --- */
		ret = RecvMsg(sockfd,buffer+sizeof(MsgHead),AckMessageLen-sizeof(MsgHead));
		/* --- 禁止超时 --- */
		timeout1.tv_sec = 0;
		timeout1.tv_usec = 0;
		setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout1,sizeof(timeout1));
		if (ret <= 0)
		{
			CSocket::Tcp_Close(sockfd);
			return ret;
		}
		else
		{
			return AckMessageLen;
		}
	}
	else if (ret == 0)  /* --- 接收超时 --- */
	{
		errno = ETIMEDOUT;
		ret = CSocket::Tcp_GetSocketErrorMsg("Recv",errno);
		CLog::WriteCommuLog(CSocket::ms_str_SocketErrorMessage);
		CSocket::Tcp_Close(sockfd);
		return ret;
	}
	else      /* --- 错误 --- */
	{
		ret = CSocket::Tcp_GetSocketErrorMsg("select",errno);
		CLog::WriteCommuLog(CSocket::ms_str_SocketErrorMessage);
		CSocket::Tcp_Close(sockfd);
		return ret;
	}        
}

/* ------------------------------------------------------
   功能描述:  接收SLE应答报文
   参数说明:  sockfd: socket句柄 
              buffer: 接收缓冲区
              recv_time_out:等待应答时间
   返回值说明:成功返回接收到的长度,服务器端断开连接返回0,
              失败返回错误代码
--------------------------------------------------------- */
int CCommu::RecvACKPackage(int sockfd,char *buffer,int timedout)
{
	struct timeval      timeout,timeout1; 
	fd_set              read_fd;
	int                 AckMessageLen;
	int                 ret;
	unsigned char       PackageLenBuffer[LEN_OF_PackageHead];
	int                 nRcvNetTimeout;
	char                errormsg[100];

	if(sockfd < 0)
	{
		memset(errormsg,0x00,sizeof(errormsg));
		sprintf(errormsg,"RecvACKPackage error,sockfd = %d\n",sockfd);
		CLog::WriteCommuLog(errormsg);
		
		return -1;
	}
		
	/* --- 给接收设置超时 --- */
	FD_ZERO(&read_fd);
	FD_SET((unsigned int)sockfd,&read_fd);
	timeout.tv_sec = timedout;
	timeout.tv_usec = 0;
	
	ret = select(sockfd+1,&read_fd,NULL,NULL,&timeout);
	if (ret > 0)  /* --- 接收缓冲区有消息可读 --- */
	{
		/* --- 设置超时 --- */
		timeout1.tv_sec = timedout;
		timeout1.tv_usec = 0;
		setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout1,sizeof(timeout1));
	
		/* --- 获得报文的长度Msg_Length --- */
		memset(PackageLenBuffer,0x00,LEN_OF_PackageHead);
		AckMessageLen = GetMsgLen(sockfd,PackageLenBuffer);
		if (AckMessageLen <= 0)
		{
			CSocket::Tcp_Close(sockfd);
			return AckMessageLen;
		}
		if ((AckMessageLen+LEN_OF_PackageHead) >= MAX_BAOWEN_LENGTH)
		{
			memset(errormsg,0x00,sizeof(errormsg));
			sprintf(errormsg,"连接句柄:%d(RecvACKPackage)报文太大!\n",sockfd);
			CLog::WriteCommuLog(errormsg);
			CSocket::Tcp_Close(sockfd);
			return 0;
		}
		memcpy(buffer,PackageLenBuffer,LEN_OF_PackageHead);
		
		/* --- 获得报文内容 --- */
		ret = RecvMsg(sockfd,buffer+LEN_OF_PackageHead,AckMessageLen-sizeof(MsgHead));
		/* --- 禁止超时 --- */
		timeout1.tv_sec = 0;
		timeout1.tv_usec = 0;
		setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout1,sizeof(timeout1));
		if (ret <= 0)
		{
			CSocket::Tcp_Close(sockfd);
			return ret;
		}
		else
		{
			return AckMessageLen;
		}
	}
	else if (ret == 0)  /* --- 接收超时 --- */
	{
		errno = ETIMEDOUT;
		ret = CSocket::Tcp_GetSocketErrorMsg("Recv",errno);
		CSocket::Tcp_Close(sockfd);
		return ret;
	}
	else      /* --- 错误 --- */
	{
		ret = CSocket::Tcp_GetSocketErrorMsg("select",errno);
		CSocket::Tcp_Close(sockfd);
		return ret;
	}        
}


/* ------------------------------------------------------
            等待长连接SC的非MACK应答报文
--------------------------------------------------------- */
int CCommu::Wait_L_Conn_Ack(int index,char *buffer,int &acklen,WORD msgtype)
{
	int Sle_L_Ack_Time;
	int ret;

	if(index < 0)
		return -1010;
	
	ret = 1;
	Sle_L_Ack_Time = 0;
	while(1)
	{
		CommonFuncs::my_usleep(50);
		
		sem_wait(&g_CSScList);
		if(CConfig::g_ScList[index].buflen > 0 && CConfig::g_ScList[index].msgtype == msgtype)
		{
			memcpy(buffer,(char*)CConfig::g_ScList[index].recv_buf,CConfig::g_ScList[index].buflen);
			acklen = CConfig::g_ScList[index].buflen;
			CConfig::g_ScList[index].buflen = 0;
			CConfig::g_ScList[index].msgtype = 0;
	
			sem_post(&g_CSScList);				
			break;
		}
		else
		{
			sem_post(&g_CSScList);
			
			Sle_L_Ack_Time = Sle_L_Ack_Time + 50;
			if(Sle_L_Ack_Time > CConfig::g_Wait_Ack_Time * 1000)
			{
				ret = -1010;
				CConfig::g_ScList[index].buflen = 0;
			  CConfig::g_ScList[index].msgtype = 0;
				
				break;
			}
		}
	}
	
	return ret;
}

/* ------------------------------------------------------
            等待短连接SC的非MACK应答报文
--------------------------------------------------------- */
int CCommu::Wait_S_Conn_Ack(int index,char * sendbuf,int sendlen,char *rspbuf,int &rsplen,WORD req_msgtype,WORD ack_msgtype,int buflen)
{
	int  ret,port,sockfd;
	char ipaddr[20];

	if(index < 0)
		return -1010;

	memset(ipaddr,0x00,sizeof(ipaddr));
	strcpy(ipaddr,CConfig::g_ScList[index].scip);
	port = CConfig::g_ScList[index].scport;
	
	ret = 0;
	rsplen = 0;
	sockfd = CSocket::Tcp_Connect(ipaddr,port,5000);
	if(sockfd > 0)
	{
		ret = CSocket::Tcp_Send(sockfd,sendbuf,sendlen);
		if(ret > 0)
		{
			if(req_msgtype == ack_msgtype)
			{
				rsplen = RecvACKPackage(sockfd,rspbuf,CConfig::g_Wait_Ack_Time,buflen);
						
				if(rsplen > 0)
				{
					ret = 1;
				}
				else
				{
					ret = rsplen;
				}
			}
			else
			{
				ret = Wait_L_Conn_Ack(index,rspbuf,rsplen,ack_msgtype);
			}
		}
		CSocket::Tcp_Close(sockfd);
	}
	return ret;
}


/* ------------------------------------------------------
            转发、等待应答
--------------------------------------------------------- */
int CCommu::SendANDRecv_L_S(int index,char * sendbuf,int sendlen,char *rspbuf,int &rsplen,WORD req_msgtype,WORD ack_msgtype,int buflen)
{
	int ret;
	
	if(SleIsLConn(CConfig::g_ScList[index].scip,CConfig::g_ScList[index].scport,CConfig::g_ScList[index].sockhandle) > 0)/* ----- 长连接 ---*/  
	{
		ret = CSocket::Tcp_Send(CConfig::g_ScList[index].sockhandle,sendbuf,sendlen);
		if(ret < 0)
		{
			ret = 0;
		}
		else
		{
			/* ----- 等待长连接的应答 ---*/
			ret = Wait_L_Conn_Ack(index,rspbuf,rsplen,ack_msgtype);
		}
	}
	else/* ----- 短连接 ---*/
	{
		ret = Wait_S_Conn_Ack(index,sendbuf,sendlen,rspbuf,rsplen,req_msgtype,ack_msgtype,buflen);
	}
	
	return ret;
}

/* ------------------------------------------------------
     功能描述:  连接、发送、接收
     返回值说明:成功返回应答报文长度,连接失败返回0
                其他错误返回SOCKET错误代码
--------------------------------------------------------- */
int CCommu::SendANDRecv(char *ipaddr,int port,char * sendbuf,int sendlen,char *rspbuf,int &rsplen,int timedout)
{
	int sockfd;
	int ret;

	ret = 0;
	rsplen = 0;
	sockfd = CSocket::Tcp_Connect(ipaddr,port,timedout);
	if(sockfd > 0)
	{	
		ret = CSocket::Tcp_Send(sockfd,sendbuf,sendlen);
		if(ret > 0)
		{
			rsplen = RecvACKPackage(sockfd,rspbuf,timedout/1000);
			if(rsplen > 0)
			{
				ret = 1;
			}
			else if(rsplen == 0)
			{
				ret = -1;
			}
			else
			{
				ret = rsplen;
			}
		}
		CSocket::Tcp_Close(sockfd);
	}
	
	return ret;
}






int COtherFuncs::CreateErrorTransfile(const char *dir,unsigned char* recv_buf,int buflen)
{
		char         filename[MAX_PATH],errormsg[500];
	time_t         long_time;
	struct tm      newtime;
	char           temp[100];
	unsigned int  chatseq;
	int            ret,i;
	MsgHead         m_MsgHead;
	char            sendid[10];
	unsigned short  msgtype;
	
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
	/* -------- 获得报文会话流水号 ---------- */
	chatseq = CommonFuncs::dwordtowin(m_MsgHead.dw_PkgID);
	/* -------- 获得当前时间 ---------- */
	memset(temp,0x00,sizeof(temp));
	time(&long_time);
	memset(&newtime,0x00,sizeof(struct tm));
	localtime_r(&long_time,&newtime);
	sprintf(temp,"%04d%02d%02d%02d%02d%02d",(newtime.tm_year+1900),(newtime.tm_mon+1),newtime.tm_mday,
	                                         newtime.tm_hour,newtime.tm_min,newtime.tm_sec);
		
	/* -------- 文件名 ---------- */
	memset(filename,0x00,sizeof(filename));
	sprintf(filename,"%s%04X.%s.%010u.%s",dir,msgtype,sendid,chatseq,temp);
	/* -------- 生成文件 ---------- */
	ret = CommonFuncs::WriteBufferFile("wb",filename,(char*)recv_buf,buflen);
	if(ret == 0)
	{
		memset(errormsg,0x00,sizeof(errormsg));
		strcpy(errormsg,"CreateErrorTransfile error\n");
		CLog::WriteAppLog(errormsg);
	}
	return ret;
 
}

/* ------------------------------------------------------
将文件移到历史目录
--------------------------------------------------------- */
void COtherFuncs::SaveHistoryData(int flag,char *curFileFullName,char *name,int mday)
{
	char           hispath[MAX_PATH];

	/* --- 获得保存的历史数据的目录 ---*/
	if(flag == 1)
	{
		memset(hispath,0x00,sizeof(hispath));
		sprintf(hispath,"%s%02d/%s",CConfig::HisDataPath_LCC,mday,name);
	}
	else
	{
		memset(hispath,0x00,sizeof(hispath));
		sprintf(hispath,"%s%02d/%s",CConfig::HisDataPath_ACC,mday,name);
	}

	/* --------- 处理结束后的数据移至历史数据目录 ----------------*/
	rename(curFileFullName,hispath);	
}

 
sem_t   	    CPackage::g_CSSessionNum;
unsigned int  CPackage::g_sessionnum;
unsigned short CPackage::g_Lcc_Count;	
void					CPackage::InitCPackageStaticVar()
{
		sem_init(&g_CSSessionNum,0,1);
		g_sessionnum = 0;
		g_Lcc_Count = 0;
		
}
void					CPackage::ReleaseCPackageStaticVar()
{
		sem_destroy(&g_CSSessionNum);
}
 
/* ------------------------------------------------------
      功能描述:  创建包头 
--------------------------------------------------------- */
void CPackage::Create_Pkg_Head(unsigned char id,MsgHead *m_MsgHead,int bodylen,WORD msgtype,unsigned int session,unsigned char *toid,char *c_PkgID)
{
	time_t      long_time;
	struct tm	  newtime;
	char        c_time_buffer[30];
	BYTE        b_time_buffer[7];
	unsigned short Lcc_Count;
	int            i;

	time(&long_time);
	memset(&newtime,0x00,sizeof(struct tm));
	localtime_r(&long_time,&newtime);
	memset(c_time_buffer,0x00,sizeof(c_time_buffer));
	sprintf(c_time_buffer,"%04d%02d%02d%02d%02d%02d",(newtime.tm_year+1900),(newtime.tm_mon+1),newtime.tm_mday,
													                          newtime.tm_hour,newtime.tm_min,newtime.tm_sec);
	memset(b_time_buffer,0x00,sizeof(b_time_buffer));
	CommonFuncs::chartobyte(c_time_buffer,(char*)b_time_buffer,sizeof(b_time_buffer));
	m_MsgHead->dw_PkgDefault = CommonFuncs::dwordtowin(0xABCDEF89);
	memcpy(m_MsgHead->b_SourceID,CConfig::g_CCIDCode,sizeof(m_MsgHead->b_SourceID));
	memcpy(m_MsgHead->b_DestID,toid,sizeof(m_MsgHead->b_DestID));
	m_MsgHead->w_MsgCode = CommonFuncs::wordtowin(msgtype);
	m_MsgHead->dw_PkgID = CommonFuncs::dwordtowin(session);
	if( msgtype == 0x1001 || msgtype == 0x1002 || msgtype == 0x1003 ||
			msgtype == 0x5001 || msgtype == 0x5002 || msgtype == 0x5003 ||
			msgtype == 0x5004 || msgtype == 0x5005)/* --- 交易/寄存器 ---*/
	{
		memcpy(m_MsgHead->b_PkgID,CConfig::g_CCIDCode,2);
		Lcc_Count = CommonFuncs::wordtowin(g_Lcc_Count);
		memcpy(m_MsgHead->b_PkgID+2,&Lcc_Count,2);
		g_Lcc_Count = g_Lcc_Count + 1;
		if(g_Lcc_Count > 0x9999)
			g_Lcc_Count = 0;
	}
	else/* --- 其他 ---*/
	{
		memcpy(m_MsgHead->b_PkgID,CConfig::g_CCIDCode,sizeof(CConfig::g_CCIDCode));
	}
	memcpy(m_MsgHead->b_PkgID+sizeof(CConfig::g_CCIDCode),b_time_buffer,sizeof(b_time_buffer));
	m_MsgHead->b_PkgID[sizeof(m_MsgHead->b_PkgID)-1] = id;
	m_MsgHead->dw_PkgLen = CommonFuncs::dwordtowin(bodylen);
	m_MsgHead->b_IsACK = 0x01;
	m_MsgHead->b_ZipType = 0x01;
	m_MsgHead->dw_ZipPkgLen = 0;
	m_MsgHead->b_EncryptType = 0x01;
	
	/*sprintf(c_PkgID,"%s%s%02X",g_CCID,c_time_buffer,id);*/
	
	for(i=0;i<sizeof(m_MsgHead->b_PkgID);i++)
		sprintf(c_PkgID+2*i,"%02X",m_MsgHead->b_PkgID[i]);
}
 
/* ------------------------------------------------------
      功能描述:  创建包头 (需要传入本地节点)
--------------------------------------------------------- */
void CPackage::Create_Pkg_Head(unsigned char id,MsgHead *m_MsgHead,int bodylen,WORD msgtype,unsigned int session,unsigned char *toid,char *c_PkgID,unsigned char *sourceid)
{
	time_t      long_time;
	struct tm	  newtime;
	char        c_time_buffer[30];
	BYTE        b_time_buffer[7];
	unsigned short Lcc_Count;
	int            i;

	time(&long_time);
	memset(&newtime,0x00,sizeof(struct tm));
	localtime_r(&long_time,&newtime);
	memset(c_time_buffer,0x00,sizeof(c_time_buffer));
	sprintf(c_time_buffer,"%04d%02d%02d%02d%02d%02d",(newtime.tm_year+1900),(newtime.tm_mon+1),newtime.tm_mday,
													                          newtime.tm_hour,newtime.tm_min,newtime.tm_sec);
	memset(b_time_buffer,0x00,sizeof(b_time_buffer));
	CommonFuncs::chartobyte(c_time_buffer,(char*)b_time_buffer,sizeof(b_time_buffer));
	m_MsgHead->dw_PkgDefault = CommonFuncs::dwordtowin(0xABCDEF89);
	memcpy(m_MsgHead->b_SourceID,sourceid,sizeof(m_MsgHead->b_SourceID));
	memcpy(m_MsgHead->b_DestID,toid,sizeof(m_MsgHead->b_DestID));
	m_MsgHead->w_MsgCode = CommonFuncs::wordtowin(msgtype);
	m_MsgHead->dw_PkgID = CommonFuncs::dwordtowin(session);
	if( msgtype == 0x1001 || msgtype == 0x1002 || msgtype == 0x1003 ||
			msgtype == 0x5001 || msgtype == 0x5002 || msgtype == 0x5003 ||
			msgtype == 0x5004 || msgtype == 0x5005)/* --- 交易/寄存器 ---*/
	{
		memcpy(m_MsgHead->b_PkgID,sourceid,2);
		Lcc_Count = CommonFuncs::wordtowin(g_Lcc_Count);
		memcpy(m_MsgHead->b_PkgID+2,&Lcc_Count,2);
		g_Lcc_Count = g_Lcc_Count + 1;
		if(g_Lcc_Count > 0x9999)
			g_Lcc_Count = 0;
	}
	else/* --- 其他 ---*/
	{
		memcpy(m_MsgHead->b_PkgID,sourceid,sizeof(CConfig::g_CCIDCode));
	}
	
	memcpy(m_MsgHead->b_PkgID+sizeof(CConfig::g_CCIDCode),b_time_buffer,sizeof(b_time_buffer));
	m_MsgHead->b_PkgID[sizeof(m_MsgHead->b_PkgID)-1] = id;
	m_MsgHead->dw_PkgLen = CommonFuncs::dwordtowin(bodylen);
	m_MsgHead->b_IsACK = 0x01;
	m_MsgHead->b_ZipType = 0x01;
	m_MsgHead->dw_ZipPkgLen = 0;
	m_MsgHead->b_EncryptType = 0x01;
	
	/*sprintf(c_PkgID,"%s%s%02X",g_CCID,c_time_buffer,id);*/
	
	for(i=0;i<sizeof(m_MsgHead->b_PkgID);i++)
		sprintf(c_PkgID+2*i,"%02X",m_MsgHead->b_PkgID[i]);
}
  
/* ------------------------------------------------------
      功能描述:   生成报文
--------------------------------------------------------- */
int CPackage::CreatePackage(int offset,char *Package,MsgHead *m_PackageHead,char *PackageBody,int LenPackageBody)
{
	unsigned int ul_crc;
	memcpy(Package,m_PackageHead,sizeof(MsgHead));
	memcpy(Package+sizeof(MsgHead)+offset,PackageBody,LenPackageBody);
	/*--- 生成CRC32校验 --- */
	Crc::Init_CRC32_Table();
	ul_crc = Crc::Crc32((char*)Package,sizeof(MsgHead)+offset+LenPackageBody,0);
	ul_crc = CommonFuncs::dwordtowin(ul_crc);
	memcpy(Package+sizeof(MsgHead)+offset+LenPackageBody,&ul_crc,LEN_OF_PackageTail);
	return (sizeof(MsgHead)+offset+LenPackageBody+LEN_OF_PackageTail);
} 
 
 
	