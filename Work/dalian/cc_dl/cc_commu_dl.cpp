
#include "cc_commu_dl.h"

pthread_t*  CThreadPool::g_Server_1;
pthread_t*  CThreadPool::g_Server_4;
pthread_t   CThreadPool::g_ISCSHandle;          /* --- �ۺϼ�ط��������߳̾�� -------- */

sem_t*   CThreadPool::g_SemServer_1;   /* ------- ͬ���̳߳�1�ź��� ------------- */
sem_t*   CThreadPool::g_SemServer_4;   /* ------- ͬ���̳߳�4�ź��� ------------- */
sem_t*   CThreadPool::g_SemParam ;     /* --- �ź�������ռ�(ͬ����������) --------- */

int*     CThreadPool::g_ListenSock;
int*     CThreadPool::g_Client_1;           /* --- ���1��ͻ������ӵ�socket��� --- */
int*     CThreadPool::g_Client_4;           /* --- ���4��ͻ������ӵ�socket��� --- */
int    	 CThreadPool::g_Client_5;           /* --- ���5��ͻ������ӵ�socket��� --- */
char     CThreadPool::ms_str_ThreadPoolErrorMessage[1024];
int      CThreadPool::g_StopFlag;
int      CThreadPool::g_MsgFlag;

/* ------------------------------------------------------
     ��������:   ��ʼ���ź���,�ɹ�����1,ʧ�ܷ���0
--------------------------------------------------------- */
int CThreadPool::Create_Sem()
{
    char          errmsg[500];

    /* --- ���ź��������ڴ�ռ� --- */
    g_SemServer_1 = (sem_t*) calloc (1,sizeof(sem_t));  /* --- �ź�������ռ�(ͬ�����1Accept����) --- */
    g_SemServer_4 = (sem_t*) calloc (1,sizeof(sem_t));  /* --- �ź�������ռ�(ͬ�����4Accept����) --- */
    CThreadPool::g_SemParam     =(sem_t *)calloc (1,sizeof(sem_t));  /* --- �ź�������ռ�(ͬ����������) --------- */

    /* --- �ڴ����ʧ�� --- */
    if (g_SemServer_1 == NULL || g_SemServer_4 == NULL || g_SemParam == NULL)
    {
        memset(errmsg,0x00,sizeof(errmsg));
        strcpy(errmsg,"Critical calloc error\n");
        CLog::WriteCommuLog(errmsg);

        return 0;
    }

    /* --- ��ʼ���ź��� --- */

    sem_init(g_SemServer_1,0,1);
    sem_init(g_SemServer_4,0,1);
    sem_init(g_SemParam,0,1);

    return 1;
}

/* ------------------------------------------------------
      ��������:   �ͷ��ź���
--------------------------------------------------------- */
void CThreadPool::Release_Sem()
{
   
    sem_destroy(g_SemServer_1);
    sem_destroy(g_SemServer_4);
    sem_destroy(CThreadPool::g_SemParam);
    
    free(g_SemServer_1); /* --- �ͷ��ź���(ͬ�����1Accept����) --- */
    free(g_SemServer_4); /* --- �ͷ��ź���(ͬ�����4Accept����) --- */
    free(g_SemParam);    /* --- �ͷ��ź���(ͬ������) --- */
 
    g_SemServer_1 = NULL;
    g_SemServer_4 = NULL;
    g_SemParam    = NULL;

}



/* ------------------------------------------------------
            ��������:   ��ʼ���̳߳�
            ����ֵ˵��: �ɹ�����1,ʧ�ܷ���0
--------------------------------------------------------- */
int CThreadPool::Initial_ThreadPool()
{
    int           i;
    char          errmsg[500];

    /* --- �����̳߳ؿռ� --- */
    g_Server_1    = (pthread_t*) calloc (MAX_CLIENT_NUMBER1,sizeof(pthread_t));
    g_Server_4    = (pthread_t*) calloc (MAX_CLIENT_NUMBER4,sizeof(pthread_t));
    if (g_Server_1 == NULL || g_Server_4 == NULL)
    {
      memset(errmsg,0x00,sizeof(errmsg));
			strcpy(errmsg,"Initial_ThreadPool calloc error\n");
			CLog::WriteCommuLog(errmsg);
		
      return 0;
    }

    /* --- ���1�̳߳ظ��߳̾����ʼ�� --- */
    for (i = 0; i < MAX_CLIENT_NUMBER1;i++)
    {
        g_Server_1[i] = 0;
    }
    /* --- ���4�̳߳ظ��߳̾����ʼ�� --- */
    for (i = 0; i < MAX_CLIENT_NUMBER4;i++)
    {
        g_Server_4[i] = 0;
    }
 
	
	/* --- �ۺϼ���߳̾����ʼ�� --- */
	g_ISCSHandle = 0;
  
  return 1;
}


/* ------------------------------------------------------
            ��������:   �������1�̳߳�
            ����ֵ˵��: �ɹ�����1,ʧ�ܷ���0
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
            ��������:   �������4�̳߳�
            ����ֵ˵��: �ɹ�����1,ʧ�ܷ���0
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
            ��������:   �ȴ��̳߳�1���߳��˳�
--------------------------------------------------------- */
void CThreadPool::Wait_ThreadPool_SC_Exit()
{
    pthread_t tmpTid;
    int       i;

    /* --- ���̳߳�1���̷߳����˳��ź� --- */
    for(i = 0; i < MAX_CLIENT_NUMBER1; i++)
    {
        tmpTid = g_Server_1[i];
        /* --- ���̷߳����˳��ź� --- */
        while(g_Server_1[i] != 0)
        {
            pthread_kill(g_Server_1[i],SIGUSR1);
            CommonFuncs::my_usleep(1);
        }
        /* --- �ȴ��߳��˳� --- */
        if (tmpTid != 0)
        {
            pthread_join(tmpTid,NULL);
        }
    }
}

/* ------------------------------------------------------
            ��������:   �ȴ��̳߳�4���߳��˳�
--------------------------------------------------------- */
void CThreadPool::Wait_ThreadPool_ACC_Exit()
{
    pthread_t tmpTid;
    int       i;

    /* --- ���̳߳�1���̷߳����˳��ź� --- */
    for(i = 0; i < MAX_CLIENT_NUMBER4; i++)
    {
        tmpTid = g_Server_4[i];
        /* --- ���̷߳����˳��ź� --- */
        while(g_Server_4[i] != 0)
        {
            pthread_kill(g_Server_4[i],SIGUSR1);
            CommonFuncs::my_usleep(1);
        }
        /* --- �ȴ��߳��˳� --- */
        if (tmpTid != 0)
        {
            pthread_join(tmpTid,NULL);
        }
    }
}


/* ------------------------------------------------------
            ��������:   �ȴ��ۺϼ���߳��˳� 
--------------------------------------------------------- */
void  CThreadPool::Wait_ISCS_Exit()
{
	pthread_t tmpTid;
	
	tmpTid = g_ISCSHandle;
	if (tmpTid == 0)
	{
		return;
	}
	
	/* --- ��ʱ��У���̷߳����˳��ź� --- */
	while(g_ISCSHandle != 0)
	{
		pthread_kill(g_ISCSHandle,SIGUSR1);
		CommonFuncs::my_usleep(1);
	}
	/* --- �ȴ��߳��˳� --- */
	if (tmpTid != 0)
	{
		pthread_join(tmpTid,NULL);
	}
}

/* ------------------------------------------------------
     ��������:   �������Թ��1�Ŀͻ�����������, ���ӳɹ���,
	             ������ͻ��˵�ͨ��
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

    /* --- �߳����� --- */
    sequence = *(int*)lpParam;
    free(lpParam);
    lpParam = NULL;

    for(;;)
    {
        /* --- ���տͻ��˵���������--- */
        ret = sem_wait(g_SemServer_1);
	
        if (ret < 0)
        {
            if (g_StopFlag == 1)
            {
                sem_post(g_SemServer_1); /* --- �ͷ��ź��� --- */

                memset(errmsg,0x00,sizeof(errmsg));
                sprintf(errmsg,"���1:�߳�%d exit\n",sequence);
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
        //����ǳ����ӵĻ�����Ҫ����SC�б�ṹ
        //�Ȳ����Ƿ�����ͬ�����Ӿ�����ڣ������ڣ��ȹر����Ӿ��
        //�ٲ��Ҷ�Ӧ��IP,����SC�����Ӿ��
        flag = 0;
				if(CConfig::g_Conn_L == 1 && connfd > 0)
				{
					sem_wait(&CCommu::g_CSScList);
					for(i=0;i<MAX_SLE_COUNT;i++)
					{
						if(CConfig::g_ScList[i].sockhandle == connfd)
						{
							memset(errmsg,0x00,sizeof(errmsg));
							sprintf(errmsg,"SC(%s:%s)���Ӿ��:%d�Ѵ���,�����Ͽ�,���½����µ�����\n",CConfig::g_ScList[i].sccode,CConfig::g_ScList[i].scip,CConfig::g_ScList[i].sockhandle);
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
								sprintf(errmsg,"�����Ͽ�SC(%s:%s)��һ����,���Ӿ��:%d\n",CConfig::g_ScList[i].sccode,CConfig::g_ScList[i].scip,CConfig::g_ScList[i].sockhandle);
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
 
        if (connfd < 0)  /* --- accept��������--- */
        {
            CLog::WriteCommuLog(CSocket::ms_str_SocketErrorMessage);

            if(g_StopFlag == 1)
            {
                memset(errmsg,0x00,sizeof(errmsg));
                sprintf(errmsg,"���1:�߳�%d exit\n",sequence);
                CLog::WriteCommuLog(errmsg);

                g_Server_1[sequence] = 0;

                return NULL;
            }
        }
        else  /* --- ���ճɹ� --- */
        {
        		memset(errmsg,0x00,sizeof(errmsg));
						sprintf(errmsg,"SC:%s����SC(%d:%d)�ɹ�,���Ӿ��:%d\n",sleip,CCommu::g_Port1,port,connfd);
						CLog::WriteCommuLog(errmsg);
			
            g_Client_1[sequence] = connfd;
            ret = CCommu::Recv_ScPackage(CConfig::g_Conn_L,connfd,sleip,port);
            g_Client_1[sequence] = 0;
            
            memset(errmsg,0x00,sizeof(errmsg));
						sprintf(errmsg,"SC:%s�����Ͽ�����,���Ӿ��:%d!ret = %d \n",sleip,connfd,ret);
						CLog::WriteCommuLog(errmsg);
						
						/* --- �������Ӹ���SC�б�ṹ --- */
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
										sprintf(errmsg,"SC:%sʹ���µ����Ӿ��:%d!\n",CConfig::g_ScList[i].scip,CConfig::g_ScList[i].sockhandle);
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
                sprintf(errmsg, "���1:�߳�%d exit\n", sequence);
                CLog::WriteCommuLog(errmsg);
                g_Server_1[sequence] = 0;

                if (ret == -2) {
                    /* --- ֪ͨӦ�ý�������ͨ�ŷ��� --- */
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
     ��������:   �������Թ��4�Ŀͻ�����������, ���ӳɹ���,
	             ������ͻ��˵�ͨ��
--------------------------------------------------------- */
void* CThreadPool::Thread_Server_ACC(void* lpParam) 
{
    int 	connfd, sequence, ret;
    char 	errmsg[500];
    char 	clnip[16];    
    int 	port;

    /* --- �߳����� --- */
    sequence = *(int *) lpParam;
    free(lpParam);
    lpParam = NULL;

    for (;;) 
    {
        /* --- ���տͻ��˵���������--- */
        ret = sem_wait(g_SemServer_4);
        if (ret < 0) 
        {
            if (g_StopFlag == 1) 
            {
                sem_post(g_SemServer_4); /* --- �ͷ��ź��� --- */

                memset(errmsg, 0x00, sizeof(errmsg));
                sprintf(errmsg, "���4:�߳�%d exit\n", sequence);
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

        if (connfd < 0)  /* --- accept��������--- */
        {
            CLog::WriteCommuLog(CSocket::ms_str_SocketErrorMessage);

            if (g_StopFlag == 1) {

                memset(errmsg, 0x00, sizeof(errmsg));
                sprintf(errmsg, "���4:�߳�%d exit\n", sequence);
                CLog::WriteCommuLog(errmsg);

                g_Server_4[sequence] = 0;

                return NULL;
            }
        } 
        else  /* --- ���ճɹ� --- */
        {
        		/* --- ���ճɹ� --- */
						memset(errmsg,0x00,sizeof(errmsg));
						sprintf(errmsg,"ACC:%s����LCC(%d:%d)�ɹ�,���Ӿ��:%d\n",clnip,CCommu::g_Port4,port,connfd);
						CLog::WriteCommuLog(errmsg);
        	
            g_Client_4[sequence] = connfd;
            ret = CCommu::Recv_ACCPackage(connfd, clnip,port);
            g_Client_4[sequence] = 0;
            
            memset(errmsg,0x00,sizeof(errmsg));
						sprintf(errmsg,"ACC:%s�Ͽ�����,���Ӿ��:%d!\n",clnip,connfd);
						CLog::WriteCommuLog(errmsg);
            
            if (g_StopFlag == 1) {

                memset(errmsg, 0x00, sizeof(errmsg));
                sprintf(errmsg, "���4:�߳�%d exit\n", sequence);
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
		
		if (g_Client_5 < 0)  /* --- accept��������--- */
		{
			 CLog::WriteCommuLog(CSocket::ms_str_SocketErrorMessage);
			 	
			if(g_StopFlag == 1)
			{				
				memset(errmsg,0x00,sizeof(errmsg));
				strcpy(errmsg,"�ۺϼ���߳�exit\n");
				CLog::WriteCommuLog(errmsg);
					
				g_ISCSHandle = 0;
 
				return NULL;
			}
		}
		else  /* --- ���ճɹ� --- */
		{
			/* --- ���ճɹ� --- */
			memset(errmsg,0x00,sizeof(errmsg));
			sprintf(errmsg,"ISCS:%s����LCC(%d:%d)�ɹ�,���Ӿ��:%d\n",clnip,CCommu::g_Port5,port,g_Client_5);
			CLog::WriteCommuLog(errmsg);
			
			for(;;)
			{
				struct timeval time_out;
				memset(ISCSBuf,0x00,CLIENT_ISCSPKG_LEN);
				/* --- ���ó�ʱ --- */
				time_out.tv_sec = 30;
				time_out.tv_usec = 0;
				setsockopt(g_Client_5,SOL_SOCKET,SO_RCVTIMEO,(char *)&time_out,sizeof(time_out));
				ret = CCommu::RecvMsg(g_Client_5,(char*)ISCSBuf,CLIENT_ISCSPKG_LEN);
				/* --- ��ֹ��ʱ --- */
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
					sprintf(errmsg,"ISCS:%s�Ͽ�����,���Ӿ��:%d!\n",clnip,g_Client_5);
					CLog::WriteCommuLog(errmsg);
					
					CSocket::Tcp_Close(g_Client_5);
					break;
				}
				
				if(g_StopFlag == 1)
				{
					memset(errmsg,0x00,sizeof(errmsg));
					strcpy(errmsg,"�ۺϼ���߳�exit\n");
					CLog::WriteCommuLog(errmsg);
					g_ISCSHandle = 0;
						
					return NULL;
				}
			}
		}
	}
}  


/* ------------------------------------------------------
            ��������:   �ͷ��̳߳�
--------------------------------------------------------- */
void  CThreadPool::Release_ThreadPool()
{
    /* --- �ͷ��̳߳� --- */
    free(g_Server_1);
    free(g_Server_4);

    g_Server_1 = NULL;
    g_Server_4 = NULL;
    
    g_ISCSHandle = 0;
}


/* ------------------------------------------------------
��������:   ��ʼ���׽��־��,�ɹ�����1,ʧ�ܷ���0
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
            ��������:   ֹͣ������������
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
      ��������:   �ͷ������׽��־��
--------------------------------------------------------- */
void CThreadPool::Release_ListenSock()
{
    free(g_ListenSock);
    g_ListenSock = NULL;
}


/* ------------------------------------------------------
            ��������:   ���ֹ�̿ͻ����ӵĳ�ʼ��
            ����ֵ˵��: �ɹ�����1,ʧ�ܷ���0
--------------------------------------------------------- */
int CThreadPool::Create_ClientSock()
{
    int           i;
    char          errmsg[500];

    /* ------- ����ռ� ----- */
    g_Client_1 = (int*) calloc (MAX_CLIENT_NUMBER1,sizeof(int));
    g_Client_4 = (int*) calloc (MAX_CLIENT_NUMBER4,sizeof(int));
    if (g_Client_1 == NULL || g_Client_4 == NULL)
    {
        memset(errmsg,0x00,sizeof(errmsg));
        strcpy(errmsg,"Tcp_Initial_Clien calloc error\n");
        CLog::WriteCommuLog(errmsg);
        return 0;
    }

    /* ------- ���1�ͻ���ʼ�� ----- */
    for(i = 0;i < MAX_CLIENT_NUMBER1;i++)
    {
        g_Client_1[i] = 0;
    }
    /* ------- ���4�ͻ���ʼ�� ----- */
    for(i = 0;i < MAX_CLIENT_NUMBER4;i++)
    {
        g_Client_4[i] = 0;
    }

		g_Client_5 = 0;

    return 1;
}


/* ------------------------------------------------------
            ��������:   �Ͽ����������ӿͻ�
--------------------------------------------------------- */
void CThreadPool::Close_ClientSock()
{
    int i;

    /* --- �Ͽ����1�����ӿͻ� --- */
    for (i = 0; i < MAX_CLIENT_NUMBER1; i++)
    {
        if (g_Client_1[i] != 0)
        {
            CSocket::Tcp_Close(g_Client_1[i]);
        }
    }
    /* --- �Ͽ����4�����ӿͻ� --- */
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
      ��������:   �ͷ������׽��־��
--------------------------------------------------------- */
void CThreadPool::Release_ClientSock()
{
    free(g_Client_1);
    free(g_Client_4);

    g_Client_1 = NULL;
    g_Client_4 = NULL;
}

/* ------------------------------------------------------
            ��������:   ͨ���̳߳�ʼ��(socket����Ϣ���С�
			            ��Ϣ�������кš��ź�������־�ļ�)
            ����ֵ˵��: ��ʼ���ɹ�����1,ʧ�ܷ���0
--------------------------------------------------------- */
int CThreadPool::Server_Initial()
{
    int ret;
    /* --- �����׽��ֳ�ʼ�� --- */
    ret = Create_ListenSock();
    if (ret == 0)
    {
        return 0;
    }

    /* ---  �ͻ������׽��ֳ�ʼ�� --- */
    ret = Create_ClientSock();
    if (ret == 0)
    {
        Release_ListenSock();
        return 0;
    }

    /* --- �ź�����ʼ�� --- */
    CMessage::Create_Sem();
    ret = Create_Sem();
    if (ret == 0)
    {
        Release_ListenSock();
        Release_ClientSock();
        return 0;
    }

    /* ---��Ϣ���г�ʼ�� --- */
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

    /* --- �̳߳س�ʼ�� --- */
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
            ��������:   �������ֹ�̵ķ�������������
            ����ֵ˵��: �����ɹ�����1,ʧ�ܷ���0
--------------------------------------------------------- */
int CThreadPool::Server_Start()
{
 
	
    int           ret;
    char          errmsg[500];

    /* --- �������1�����׽��� --- */
    g_ListenSock[0] = CSocket::Tcp_Listen(CCommu::g_Port1);
    if (g_ListenSock[0] < 0)
    {
        return 0;
    }

    /* --- �������1�׽��̳߳� --- */
    ret = Create_ThreadPool_SC();
    if (ret == 0)
    {
        memset(errmsg,0x00,sizeof(errmsg));
        sprintf(errmsg,"Create_ThreadPool_SC Errors:%d\n",errno);
        CLog::WriteCommuLog(errmsg);
        CCommu::CommunicationStop();
        return 0;
    }


    /* --- �������4�����׽��� --- */
    g_ListenSock[1] = CSocket::Tcp_Listen(CCommu::g_Port4);
    if (g_ListenSock[1] < 0)
    {
        return 0;
    }

    /* --- �������4�׽��̳߳� --- */
    ret = Create_ThreadPool_ACC();
    if (ret == 0)
    {
        memset(errmsg,0x00,sizeof(errmsg));
        sprintf(errmsg,"Create_ThreadPool_ACC Errors:%d\n",errno);
        CLog::WriteCommuLog(errmsg);
        CCommu::CommunicationStop();
        return 0;
    }


	/* --- �����ۺϼ�ط����� --- */
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
	strcpy(errMsg,"ͨ�Ŷ�̬�����������\n");
	CLog::WriteCommuLog(errMsg);
		
  return 1;
}


/* ------------------------------------------------------
       ��������:�ͷ���Ϣ����,ֹͣ���������������ҶϿ����������ӵĿͻ�    
--------------------------------------------------------- */
void CThreadPool::Server_Close( )
{
    /* --- �����˳����������־ --- */
    g_StopFlag = 1;
    

    /* --- �ͷ��̳߳� --- */
    Wait_ThreadPool_SC_Exit();
    Wait_ThreadPool_ACC_Exit();
    Wait_ISCS_Exit();
    
    Release_ThreadPool();

    /* --- ֹͣ������������ --- */
    Close_ListenSock();
    /* --- �Ͽ����������ӿͻ� --- */
    Close_ClientSock();
 
    /* --- �ͷ������׽���--- */
    Release_ListenSock();

    /* --- �ͷ������ӿͻ��׽���--- */
    Release_ClientSock();
 
 		/* --- �ͷ��ź��� --- */
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
	
	/* -------- ��ò���ֵ -------*/
	m_ISCSMsg = *(ISCSMsg *)recvBuf;
	TCP_Num = CommonFuncs::wordtowin(m_ISCSMsg.TCP_Num);
	memcpy(&start_addr,recvBuf+CLIENT_ISCS_HEAD_1,2);
	start_addr =  CommonFuncs::wordtowin(m_ISCSMsg.Start_Addr);
	memcpy(&count_of_sle,recvBuf+CLIENT_ISCS_HEAD_2,2);
	count_of_sle = CommonFuncs::wordtowin(m_ISCSMsg.Register_Num);
	
	/* -------- д����־ -------*/
	memset(applog,0x00,sizeof(applog));
	sprintf(applog,"���յ��ۺϼ�ر���(���Ӿ�� = %d,�����к� = %d,�Ĵ������� = %d,��ʼ��ַ = %d)\n",connfd,TCP_Num,count_of_sle,start_addr);
	CLog::WriteCommuLog(applog);
		
	/* --- ����DEBUG��־ --- */
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
		strcpy(applog,"���������\n");
		CLog::WriteCommuLog(applog);
		
		/* ----- -- �Ͽ����� ----*/	
		CSocket::Tcp_Close(connfd);
		return;
	}
	
	if(start_addr*2 + count_of_sle*2 >= (MAX_BAOWEN_LENGTH-SERVER_ISCS_HEAD_1))
	{
		CSocket::Tcp_Close(connfd);
		memset(applog,0x00,sizeof(applog));
		sprintf(applog,"��ʼ��ַ:%d,�Ĵ�������:%d����ָ����Χ\n",start_addr,count_of_sle);
		CLog::WriteCommuLog(applog);	
		
		/* ----- -- �Ͽ����� ----*/	
		CSocket::Tcp_Close(connfd);
				
		return;
	}
	
	/* ----- -- ��䷢�ͱ��� ----*/	
	package_len = SERVER_ISCS_HEAD_1 + count_of_sle*2 - SERVER_ISCS_HEAD_2 - 2;
	memset(sendBuf,0x00,sizeof(sendBuf));
	memcpy(sendBuf,recvBuf,SERVER_ISCS_HEAD_2);
	package_len =  CommonFuncs::wordtowin(package_len);
	memcpy(sendBuf+SERVER_ISCS_HEAD_2,&package_len,2);
	memcpy(sendBuf+SERVER_ISCS_HEAD_2+2,recvBuf+SERVER_ISCS_HEAD_2+2,2);
	if(count_of_sle<= 123 )
		sendBuf[SERVER_ISCS_HEAD_1-1] = count_of_sle*2;
	memcpy(sendBuf+SERVER_ISCS_HEAD_1,(char*)g_ISCS_SndPkg+start_addr*2,count_of_sle*2);
	
	/* ----- -- ���ͱ��� ----*/	
	sendBuf[8] = count_of_sle * 2;
	rtn = CSocket::Tcp_Send(connfd,(char*)sendBuf,SERVER_ISCS_HEAD_1+count_of_sle*2);
	if(rtn == 1)
	{
		/* ----- -- д����־ ----*/	
		memset(applog,0x00,sizeof(applog));
		sprintf(applog,"�����ۺϼ��Ӧ���ĳɹ�(���Ӿ�� = %d,�����к� = %d,���ĳ��� = %d)\n",connfd,TCP_Num,SERVER_ISCS_HEAD_1+count_of_sle*2);
		CLog::WriteCommuLog(applog);
		/* --- ����DEBUG��־ --- */
		if(DEBUG_FLAG)
		{
			CLog::WriteDebugLog(1,sendBuf,SERVER_ISCS_HEAD_1+count_of_sle*2);
		}
	}	
	else
	{
		/* ----- -- �Ͽ����� ----*/	
		CSocket::Tcp_Close(connfd);
		
		memset(applog,0x00,sizeof(applog));
		sprintf(applog,"�����ۺϼ��Ӧ����ʧ��(���Ӿ�� = %d,�����к� = %d,���ĳ��� = %d)\n",connfd,TCP_Num,SERVER_ISCS_HEAD_1+count_of_sle*2);
		CLog::WriteCommuLog(applog);
	}
}



/* ------------------------------------------------------
      ��������:   �����豸״̬
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
 
	/* --- ����豸���� ---*/
	sletype = *(p+2);
	/* --- ����豸��� ---*/
	shebh = *(p+3);
	/* --- ���վ��� ---*/
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
		   0   0 ����     1 ����
		   1   0 ��ͣ���� 1 ��������
		   2   0 �ޱ���   1 �б���
		   3   0 �޹���   1 �й��� */
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
      ��������:   ���³�վ��Ӫģʽ
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
			g_ISCS_SndPkg[nbyte] = (BYTE) 0x00; /*--����--*/		
	}
	else	if(isConnect == 1) 
	{
			g_ISCS_SndPkg[nbyte] = (BYTE) 0x01; /*--����-*/
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
 
	/* -------- ���¿������� -------*/
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
					
			/* --- ���վ��� ---*/
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
Ѱ��ԭ��վ���ۺϼ�س�վ�б�������Ϣ��Ѱ�ҳɹ������±꣬
���򷵻�-1
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

/*------16����ת10����---0x20ת��0d20-*/ 
/*�������Ϊ0x2222�ȣ��������ַ�*/
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
    
void ISCS::IscsInit()   //�ۺϼ�س�ʼ�� 
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
 
void ISCS::IscsRelease()   //�ۺϼ������ 
{
	sem_destroy(&g_CS_ISCS);
}

 

 




int            CCommu::g_Port1;                   /* ------ ���1�����˿ں� ----------------------------- */
int            CCommu::g_Port4;                   /* ------ ���4�����˿ں� ----------------------------- */
int            CCommu::g_Port5;  									/* ------ ���5�����˿ں� ----------------------------- */
sem_t          CCommu::g_CSScList; 
 
/* ------------------------------------------------------
            ��������:   ���̺߳���(���Ӧ�ý��̵ľ����
			            ��ʼ����������������������
            ����ֵ˵��: �ɹ�����1,ʧ�ܷ���0
--------------------------------------------------------- */
int CCommu::CommunicationMain(int port1,int port4,int port5)
{
   
 	int	 ret = 0;
	char errMsg[1024];
 
	/* --- ���1�Ķ˿ں� --- */
	g_Port1 = port1;
	
	/* --- ���4�Ķ˿ں� --- */
	g_Port4 = port4;

 	/* --- ���5�Ķ˿ں� --- */
	g_Port5 = port5;

 	sem_init(&g_CSScList,0,1);
 	
	/* --- ͨ�����̳߳�ʼ�� --- */
	ret = CThreadPool::Server_Initial();
	if (ret == 0)
	{
		 memset(errMsg,0x00,sizeof(errMsg));
     strcpy(errMsg,"ͨ�����̳߳�ʼ��ʧ�ܣ�\n");
     CLog::WriteAppLog(errMsg);
         
		return SERVER_INITIAL_ERROR;
	}
 

 	//g_InitialServer = 1;

	/* --- ��ʼ���˳����������־ --- */
	CThreadPool::g_StopFlag = 0;
	/* --- ��ʼ���������ͱ��� --- */
	CThreadPool::g_MsgFlag = -1;

	memset(errMsg,0x00,sizeof(errMsg));
	strcpy(errMsg,"ͨ�Ŷ�̬���ʼ����ɣ�\n");
	CLog::WriteCommuLog(errMsg);
     	
	return SUCCESS;
}

 
/* ------------------------------------------------------------------
    ��������:   �ͷ���Ϣ����,ֹͣ���������������ҶϿ����������ӵĿͻ�,
    						�����ͷ��ź������ͷ��ڴ�
--------------------------------------------------------------------- */
void CCommu::CommunicationStop(void)
{
		CThreadPool::Server_Close();
			
		char errmsg[100];
		memset(errmsg,0x00,sizeof(errmsg));
		strcpy(errmsg,"ͨ�Ŷ�̬���˳�\n");
		CLog::WriteCommuLog(errmsg);
	
	
   /* --- �ͷ���Ϣ���� --- */
    sem_wait(CMessage::g_SemMsg);
    CMessage::MsgQRelease();
    sem_post(CMessage::g_SemMsg);
    	
    /* --- �ͷ��ź��� --- */
    CMessage::Release_Sem();    
    	
    sem_destroy(&g_CSScList);		
}

/* ------------------------------------------------------
   ��������:  ���տͻ��˵�һ��������
   ����˵��:  sockfd: socket��� 
              buffer: �洢���ĵ�����
              msg_len:���ĵĳ���			 
   ����ֵ˵��:�ɹ�����1,ʧ�ܷ��ش������
--------------------------------------------------------- */
int CCommu::RecvMsg(int sockfd,char *buffer,int msg_len)
{
	int nleft,nrcv;

	nleft = msg_len; // nleft:  δ���յĵ��ֽ���
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
   ��������:  �������Կͻ��˵Ĳ�������
   ����˵��:  connfd: �����Ӿ����� 
   ����ֵ˵��:�ͻ��˹رշ���0,���յ�KILL�źŷ���-1,
              �������ش������
--------------------------------------------------------- */
int CCommu::Recv_ACCPackage(int sockfd,char *clnip,int port) 
{ 
	int     ret;

	for(;;)
	{	
		/* --- �������Կͻ��˵�������--- */
		ret = Prepare_ACCRecvPackage(sockfd,clnip,port);
		if(ret >= 0)        /* --- --- */
		{
			continue;
		}
		else               /* --- �����ж� --- */
		{
			return ret;
		}
	}
}


/* ------------------------------------------------------
   ��������:  ������������Ĳ�������,���뻺������
   ����˵��:  sockfd: socket��� 
			  message_len:���ĵĳ���
   ����ֵ˵��:�ɹ�����1,�ͻ��˶Ͽ����ӷ���0,
              ʧ�ܷ��ش������
--------------------------------------------------------- */
int CCommu::Prepare_ACCRecvPackage(int sockfd,char *clnip,int port)
{
	int             ret;
	char            *ParamBuffer;
	int             ParamLength;
	char            errmsg[500];
	struct timeval  time_out;

	/* --- ���ó�ʱ --- */
	time_out.tv_sec = 300;
	time_out.tv_usec = 0;
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&time_out,sizeof(time_out));
	/* --- ��ñ��ĵĳ���Msg_Length --- */
	ParamBuffer = (char*)malloc(MAX_BAOWEN_LENGTH);
	memset(ParamBuffer,0x00,MAX_BAOWEN_LENGTH );
	ParamLength = GetMsgLen(sockfd,(unsigned char*)ParamBuffer);
		
	/* --- ��ֹ��ʱ --- */
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
		sprintf(errmsg,"���Ӿ��:%d(%s:%d)����̫��!\n",sockfd,clnip,port);
		CLog::WriteCommuLog(errmsg);
		CSocket::Tcp_Close(sockfd);
		return -2;
	}
	/* --- ���ó�ʱ --- */
	time_out.tv_sec = 30;
	time_out.tv_usec = 0;
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&time_out,sizeof(time_out));
	/* --- ����һ�������ı��� --- */
	ret = RecvMsg(sockfd,ParamBuffer+sizeof(MsgHead),ParamLength-sizeof(MsgHead));
	/* --- ��ֹ��ʱ --- */
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
		
	/* --- �ѱ��ķŽ���Ϣ���� --- */
	if(CThreadPool::g_StopFlag == 1)/* ------- ��Ϣ�������� ------- */
	{
		CSocket::Tcp_Close(sockfd);
		ret = -2;
	}
	else
	{
		sem_wait(CMessage::g_SemMsg);        /* --- ����ź��� --- */	
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
��������:  ����CCӦ����
����˵��:  sockfd: socket��� 
buffer: ���ջ�����
����ֵ˵��:�ɹ����ؽ��յ��ĳ���,���󷵻�0
--------------------------------------------------------- */
int CCommu::RecvACCPackage_Client(int sockfd,unsigned char *buffer,int timedout)
{
	int             ret;
	int             PackageLength;
	struct timeval  stru_timeval; 

	/* --- ���ó�ʱ --- */
	stru_timeval.tv_sec = timedout;
	stru_timeval.tv_usec = 0;
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&stru_timeval,sizeof(stru_timeval));
	PackageLength = GetMsgLen(sockfd,buffer);
	
	/* --- ��ֹ��ʱ --- */
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
	/* --- ����̫�� --- */
	if(PackageLength > MAX_BAOWEN_LENGTH)
	{
		return -1;	
	}
	/* --- ���ĸ�ʽ���Ϸ� --- */
	if(PackageLength < LEN_OF_PackageHead+LEN_OF_PackageTail)
	{
		return -2;
	}
	/* --- ���ó�ʱ --- */
	stru_timeval.tv_sec = 5;
	stru_timeval.tv_usec = 0;
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&stru_timeval,sizeof(stru_timeval));
	ret = RecvMsg(sockfd,(char*)buffer+sizeof(MsgHead),PackageLength - sizeof(MsgHead));
	/* --- ��ֹ��ʱ --- */
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
   ��������:  ��ñ��ĵĳ���
   ����˵��:  sockfd: socket��� 
   ����ֵ˵��:�ɹ����ر��ĵĳ���,�ͻ��˶Ͽ����ӷ���0,
              ʧ�ܷ��ش������
--------------------------------------------------------- */
int CCommu::GetMsgLen(int sockfd,unsigned char *buffer)
{
	int            n;
	int            AckMsgLen;
	BYTE           *p;
	
	n = RecvMsg(sockfd,(char*)buffer,LEN_OF_PackageHead);
	if (n < 0)       /* --- recv��������ʧ�� --- */
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
   ��������:  �������Կͻ��˵�������
   ����ֵ˵��:�ͻ��˹رշ���0,���յ�KILL�źŷ���-1,
              �������ش������
--------------------------------------------------------- */
int CCommu::Recv_ScPackage(int flag,int connfd,char *clnip,int port) 
{ 
	int     recv_len;
	char    errmsg[500];

	for(;;)
	{	
		/* --- �������Կͻ��˵�������--- */
		recv_len = DealScMsg(flag,connfd,clnip,port);
		if (recv_len == -1)     /* --- ��Ϣ����д����� --- */
		{
			memset(errmsg,0x00,sizeof(errmsg));
			sprintf(errmsg,"SOCKET����:%dд����Ϣ���г���!\n",connfd);
			CLog::WriteCommuLog(errmsg);
			continue;
		}
		else if (recv_len == 0) /* --- �ͻ��˹رջ�����Ϣ�������� --- */
		{
			return 0;
		}
		else if (recv_len < 0) /* --- recv�������� --- */
		{
			return recv_len;
		}
	}
} 

/* ------------------------------------------------------
   ��������:  �������Կͻ��˵�������,������Ϣ���С�
              ���ȵ��� GetClnAckMsgLen������ȡͷ����
			  �ֽ�,��øñ��ĵĳ���,�������
			  MAX_MESSAGE_LENGTH,˵���Ƕ������
   ����˵��:  sockfd: socket��� 
			  Msg_Length:���ĵĳ���
   ����ֵ˵��:�ɹ�����1,�ͻ��˶Ͽ����ӷ���0,
              ʧ�ܷ��ش������
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
		/* --- ���ó�ʱ --- */
		time_out.tv_sec = 300;
		time_out.tv_usec = 0;
		setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&time_out,sizeof(time_out));
	}
	
	/* --- ��ñ��ĵĳ���Msg_Length --- */
	memset(PackageHead,0x00,LEN_OF_PackageHead);
	Msg_Length = GetMsgLen(sockfd,PackageHead);
	
	if(flag == 0)
	{
		/* --- ��ֹ��ʱ --- */
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
		sprintf(errmsg,"���Ӿ��:%d(%s:%d)����̫��!\n",sockfd,clnip,port);
		CLog::WriteCommuLog(errmsg);
		CSocket::Tcp_Close(sockfd);
		return 0;
	}
	/* --- ͨ���߳�׼���˳� --- */
	if(CThreadPool::g_StopFlag == 1)
	{
		CSocket::Tcp_Close(sockfd);
		return 0;
	}
	/* --- ��Ϣ�������� ---- */
	sem_wait(CMessage::g_SemMsg);

	if (CMessage::g_MsgQ->msg_qnum  > MAX_MSG_NUMBER)
	{
		memset(errmsg,0x00,sizeof(errmsg));
		strcpy(errmsg,"��Ϣ��������,���³�ʼ��!\n");
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

	if (ret == 1)   /* --- ����ɹ�,֪ͨӦ�ý��� --- */
	{
		CThreadPool::g_MsgFlag = 1;
		sem_post(&CMessage::g_SemReadMsg);
	}
	return ret;
}

/* ------------------------------------------------------
   ��������:  ����������
   ����˵��:  sockfd: socket��� 
              strMsgLen: ���ĵ�ͷ�����ֽ�
              msg_len:���ĵĳ���
   ����ֵ˵��:�ɹ�����1,ʧ�ܷ��ش������
--------------------------------------------------------- */
int CCommu::PrepareWriteMsgQ(int sockfd,unsigned char *PackageLenBuffer,int message_len,char *clnip,int port)
{
	int            ret;
	char           *buffer;
	struct timeval time_out;

	buffer = (char*)malloc(MAX_BAOWEN_LENGTH);
	memset(buffer,0x00,MAX_BAOWEN_LENGTH);
	memcpy(buffer,PackageLenBuffer,LEN_OF_PackageHead);
	
	/* --- ���ó�ʱ --- */
	time_out.tv_sec = 30;
	time_out.tv_usec = 0;
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&time_out,sizeof(time_out));
	
	/* --- ����һ�������ı��� --- */
	ret = RecvMsg(sockfd,buffer+sizeof(MsgHead),message_len-sizeof(MsgHead));
	/* --- ��ֹ��ʱ --- */
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
		/* --- �ѱ��ķŽ���Ϣ���� --- */
		if(CThreadPool::g_StopFlag == 1)/* ------- ��Ϣ�������� ------- */
		{
			CSocket::Tcp_Close(sockfd);
			free(buffer);
			buffer = NULL;
			return 0;
		}
		sem_wait(CMessage::g_SemMsg);        /* --- ����ź��� --- */	
		ret = CMessage::MsgWrite(sockfd,2,buffer,message_len,clnip,port);
		sem_post(CMessage::g_SemMsg);        /* --- �ͷ��ź��� --- */	
	}
	free(buffer);
	buffer = NULL;
	return ret;
}



/* ------------------------------------------------------
��������:   ����Ӧ���ͷ  
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
            ��������:   ��CC���Ϳ��������Ӧ��
--------------------------------------------------------- */
int CCommu::SendtoACK(int sockfd,int msgtype,MsgHead *m_OldPackageHead,char *PackageBody,unsigned short LenofPackageBody)
{
	BYTE				*Package;
	int					ret,length;
	MsgHead   	m_ack_msghead;
	char        filename[MAX_PATH];
	
	/* --- Ϊ���ķ���ռ� --- */
	length = LEN_OF_PackageHead + LenofPackageBody + LEN_OF_PackageTail;
	Package = (BYTE*) malloc(length * sizeof(BYTE));
	memset(Package,0x00,length);
	
	/* --- ���ɱ���ͷ --- */
	memset(&m_ack_msghead,0x00,LEN_OF_PackageHead);
	CreateAckPkgHead(LenofPackageBody,msgtype,m_OldPackageHead,&m_ack_msghead);
		
	/* --- ���ɱ��� --- */
	memcpy(Package,&m_ack_msghead,LEN_OF_PackageHead);
	memcpy(Package+LEN_OF_PackageHead,PackageBody,LenofPackageBody);
	
	/* --- ����CRC32У�� --- */
	unsigned int ul_crc;
	Crc::Init_CRC32_Table();
	ul_crc = Crc::Crc32((char*)Package,LEN_OF_PackageHead+LenofPackageBody,0);
	ul_crc = 	CommonFuncs::dwordtowin(ul_crc);
	memcpy(Package+LEN_OF_PackageHead+LenofPackageBody,&ul_crc,LEN_OF_PackageTail);
	
	/* --- ����Ӧ�� --- */
	ret = CSocket::Tcp_Send(sockfd,(char*)Package,length);
	
  /*---����Ӧ��������-------------*/ 
  char oplogmsg[1024];
	memset(oplogmsg,0x00,sizeof(oplogmsg));
	sprintf(oplogmsg, "���Ӿ��:%d,��������:0x%04X��Ӧ����(����:%d),��������\n",sockfd,msgtype,length);
	CLog::WriteAppLog(oplogmsg);
	CLog::WriteDebugLog(0,(unsigned char*)Package,length);
	
	/* --- �ͷű�����ռ�ռ� --- */
	free(Package);
	Package = NULL;

	return ret;
 
}
 
/* ------------------------------------------------------
            ��������:   �ж������Ƿ��ǳ����ӣ�
            �����ӷ���1;�����ӷ���0;
--------------------------------------------------------- */
int CCommu::SleIsLConn(char *ip,int port,int conn_sockhandle)
{
	int sockfd;
	
	if(CConfig::g_Conn_L == 0)/* --- ������ --- */
		return 0;
	if(conn_sockhandle <= 0)/* --- ������ --- */
		return 0;
	
	sockfd = CSocket::Tcp_Connect(ip,port,1000);
	if(sockfd > 0)/* --- ������ --- */
	{
		CSocket::Tcp_Close(sockfd);
		return 0;
	}
	
	return 1;/* --- ������ --- */
}
/* ------------------------------------------------------
   ��������:  ����SLEӦ����
   ����˵��:  sockfd: socket��� 
              buffer: ���ջ�����
              recv_time_out:�ȴ�Ӧ��ʱ��
   ����ֵ˵��:�ɹ����ؽ��յ��ĳ���,�������˶Ͽ����ӷ���0,
              ʧ�ܷ��ش������
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
		
	/* --- ���������ó�ʱ --- */
	FD_ZERO(&read_fd);
	FD_SET((unsigned int)sockfd,&read_fd);
	timeout.tv_sec = timedout;
	timeout.tv_usec = 0;
	
	ret = select(sockfd+1,&read_fd,NULL,NULL,&timeout);
	if (ret > 0)  /* --- ���ջ���������Ϣ�ɶ� --- */
	{
		/* --- ���ó�ʱ --- */
		timeout1.tv_sec = timedout;
		timeout1.tv_usec = 0;
		setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout1,sizeof(timeout1));
	
		/* --- ��ñ��ĵĳ���Msg_Length --- */
		AckMessageLen = GetMsgLen(sockfd,(unsigned char*)buffer);
		if (AckMessageLen <= 0)
		{
			CSocket::Tcp_Close(sockfd);
			return AckMessageLen;
		}
		if(AckMessageLen > buflen)
		{
			memset(errormsg,0x00,sizeof(errormsg));
			sprintf(errormsg,"���Ӿ��:%d(RecvACKPackage)����̫��!\n",sockfd);
			CLog::WriteCommuLog(errormsg);
			CSocket::Tcp_Close(sockfd);
			return -100;
		}
		/* --- ��ñ������� --- */
		ret = RecvMsg(sockfd,buffer+sizeof(MsgHead),AckMessageLen-sizeof(MsgHead));
		/* --- ��ֹ��ʱ --- */
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
	else if (ret == 0)  /* --- ���ճ�ʱ --- */
	{
		errno = ETIMEDOUT;
		ret = CSocket::Tcp_GetSocketErrorMsg("Recv",errno);
		CLog::WriteCommuLog(CSocket::ms_str_SocketErrorMessage);
		CSocket::Tcp_Close(sockfd);
		return ret;
	}
	else      /* --- ���� --- */
	{
		ret = CSocket::Tcp_GetSocketErrorMsg("select",errno);
		CLog::WriteCommuLog(CSocket::ms_str_SocketErrorMessage);
		CSocket::Tcp_Close(sockfd);
		return ret;
	}        
}

/* ------------------------------------------------------
   ��������:  ����SLEӦ����
   ����˵��:  sockfd: socket��� 
              buffer: ���ջ�����
              recv_time_out:�ȴ�Ӧ��ʱ��
   ����ֵ˵��:�ɹ����ؽ��յ��ĳ���,�������˶Ͽ����ӷ���0,
              ʧ�ܷ��ش������
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
		
	/* --- ���������ó�ʱ --- */
	FD_ZERO(&read_fd);
	FD_SET((unsigned int)sockfd,&read_fd);
	timeout.tv_sec = timedout;
	timeout.tv_usec = 0;
	
	ret = select(sockfd+1,&read_fd,NULL,NULL,&timeout);
	if (ret > 0)  /* --- ���ջ���������Ϣ�ɶ� --- */
	{
		/* --- ���ó�ʱ --- */
		timeout1.tv_sec = timedout;
		timeout1.tv_usec = 0;
		setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout1,sizeof(timeout1));
	
		/* --- ��ñ��ĵĳ���Msg_Length --- */
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
			sprintf(errormsg,"���Ӿ��:%d(RecvACKPackage)����̫��!\n",sockfd);
			CLog::WriteCommuLog(errormsg);
			CSocket::Tcp_Close(sockfd);
			return 0;
		}
		memcpy(buffer,PackageLenBuffer,LEN_OF_PackageHead);
		
		/* --- ��ñ������� --- */
		ret = RecvMsg(sockfd,buffer+LEN_OF_PackageHead,AckMessageLen-sizeof(MsgHead));
		/* --- ��ֹ��ʱ --- */
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
	else if (ret == 0)  /* --- ���ճ�ʱ --- */
	{
		errno = ETIMEDOUT;
		ret = CSocket::Tcp_GetSocketErrorMsg("Recv",errno);
		CSocket::Tcp_Close(sockfd);
		return ret;
	}
	else      /* --- ���� --- */
	{
		ret = CSocket::Tcp_GetSocketErrorMsg("select",errno);
		CSocket::Tcp_Close(sockfd);
		return ret;
	}        
}


/* ------------------------------------------------------
            �ȴ�������SC�ķ�MACKӦ����
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
            �ȴ�������SC�ķ�MACKӦ����
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
            ת�����ȴ�Ӧ��
--------------------------------------------------------- */
int CCommu::SendANDRecv_L_S(int index,char * sendbuf,int sendlen,char *rspbuf,int &rsplen,WORD req_msgtype,WORD ack_msgtype,int buflen)
{
	int ret;
	
	if(SleIsLConn(CConfig::g_ScList[index].scip,CConfig::g_ScList[index].scport,CConfig::g_ScList[index].sockhandle) > 0)/* ----- ������ ---*/  
	{
		ret = CSocket::Tcp_Send(CConfig::g_ScList[index].sockhandle,sendbuf,sendlen);
		if(ret < 0)
		{
			ret = 0;
		}
		else
		{
			/* ----- �ȴ������ӵ�Ӧ�� ---*/
			ret = Wait_L_Conn_Ack(index,rspbuf,rsplen,ack_msgtype);
		}
	}
	else/* ----- ������ ---*/
	{
		ret = Wait_S_Conn_Ack(index,sendbuf,sendlen,rspbuf,rsplen,req_msgtype,ack_msgtype,buflen);
	}
	
	return ret;
}

/* ------------------------------------------------------
     ��������:  ���ӡ����͡�����
     ����ֵ˵��:�ɹ�����Ӧ���ĳ���,����ʧ�ܷ���0
                �������󷵻�SOCKET�������
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
	/* -------- ��ñ������� ---------- */
	msgtype = CommonFuncs::wordtowin(m_MsgHead.w_MsgCode);
	/* -------- ��÷��ͷ��ڵ��ʶ�� ---------- */
	memset(sendid,0x00,sizeof(sendid));
	for(i = 0;i < 4;i++)
	{
		sprintf(sendid + 2*i,"%02X",m_MsgHead.b_SourceID[i]);
	}
	/* -------- ��ñ��ĻỰ��ˮ�� ---------- */
	chatseq = CommonFuncs::dwordtowin(m_MsgHead.dw_PkgID);
	/* -------- ��õ�ǰʱ�� ---------- */
	memset(temp,0x00,sizeof(temp));
	time(&long_time);
	memset(&newtime,0x00,sizeof(struct tm));
	localtime_r(&long_time,&newtime);
	sprintf(temp,"%04d%02d%02d%02d%02d%02d",(newtime.tm_year+1900),(newtime.tm_mon+1),newtime.tm_mday,
	                                         newtime.tm_hour,newtime.tm_min,newtime.tm_sec);
		
	/* -------- �ļ��� ---------- */
	memset(filename,0x00,sizeof(filename));
	sprintf(filename,"%s%04X.%s.%010u.%s",dir,msgtype,sendid,chatseq,temp);
	/* -------- �����ļ� ---------- */
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
���ļ��Ƶ���ʷĿ¼
--------------------------------------------------------- */
void COtherFuncs::SaveHistoryData(int flag,char *curFileFullName,char *name,int mday)
{
	char           hispath[MAX_PATH];

	/* --- ��ñ������ʷ���ݵ�Ŀ¼ ---*/
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

	/* --------- ��������������������ʷ����Ŀ¼ ----------------*/
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
      ��������:  ������ͷ 
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
			msgtype == 0x5004 || msgtype == 0x5005)/* --- ����/�Ĵ��� ---*/
	{
		memcpy(m_MsgHead->b_PkgID,CConfig::g_CCIDCode,2);
		Lcc_Count = CommonFuncs::wordtowin(g_Lcc_Count);
		memcpy(m_MsgHead->b_PkgID+2,&Lcc_Count,2);
		g_Lcc_Count = g_Lcc_Count + 1;
		if(g_Lcc_Count > 0x9999)
			g_Lcc_Count = 0;
	}
	else/* --- ���� ---*/
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
      ��������:  ������ͷ (��Ҫ���뱾�ؽڵ�)
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
			msgtype == 0x5004 || msgtype == 0x5005)/* --- ����/�Ĵ��� ---*/
	{
		memcpy(m_MsgHead->b_PkgID,sourceid,2);
		Lcc_Count = CommonFuncs::wordtowin(g_Lcc_Count);
		memcpy(m_MsgHead->b_PkgID+2,&Lcc_Count,2);
		g_Lcc_Count = g_Lcc_Count + 1;
		if(g_Lcc_Count > 0x9999)
			g_Lcc_Count = 0;
	}
	else/* --- ���� ---*/
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
      ��������:   ���ɱ���
--------------------------------------------------------- */
int CPackage::CreatePackage(int offset,char *Package,MsgHead *m_PackageHead,char *PackageBody,int LenPackageBody)
{
	unsigned int ul_crc;
	memcpy(Package,m_PackageHead,sizeof(MsgHead));
	memcpy(Package+sizeof(MsgHead)+offset,PackageBody,LenPackageBody);
	/*--- ����CRC32У�� --- */
	Crc::Init_CRC32_Table();
	ul_crc = Crc::Crc32((char*)Package,sizeof(MsgHead)+offset+LenPackageBody,0);
	ul_crc = CommonFuncs::dwordtowin(ul_crc);
	memcpy(Package+sizeof(MsgHead)+offset+LenPackageBody,&ul_crc,LEN_OF_PackageTail);
	return (sizeof(MsgHead)+offset+LenPackageBody+LEN_OF_PackageTail);
} 
 
 
	