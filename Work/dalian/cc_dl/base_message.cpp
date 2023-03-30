
#include "base_message.h"

/* ------------------------------------------------------
功能描述:   消息队列初始化
返回值说明: 成功返回1,失败返回0
--------------------------------------------------------- */
MESSAGE_QUEUE * CMessage::g_MsgQ ;
sem_t    *CMessage::g_SemMsg = (sem_t*) calloc (1,sizeof(sem_t));  /* --- 信号量分配空间(同步消息队列) ---------- */
sem_t   CMessage::g_SemReadMsg;

/* ------------------------------------------------------
   功能描述:  获取新消息插入队列的位置, 插入位置
              为消息队列中同类优先级消息的尾部
   参数说明:  priority:欲插入消息的优先级
              first   :指向要找节点的后一位置
		      ptr     :指向要找节点的位置,新消息插在ptr后面
   返回值说明:返回消息插入的位置,返回NULL表示消息队列为空
--------------------------------------------------------- */
MESSAGE* CMessage::FindInsertPosition(int priority)
{
    struct MESSAGE *first,*ptr;

    ptr = NULL;
    first = g_MsgQ->msg_first;

    while(first != NULL)
    {
        /* --- 找到比新消息优先级大的第一个消息 --- */
        if (first->priority > priority)
        {
            /* --- 新消息插在消息队列的队头 --- */
            if (ptr == NULL)
            {
                return first;
            }
                /* ---------- 新消息插在ptr的后面 -------------- */
            else
            {
                return ptr;
            }
        }
            /* --- 找到与新消息同优先级的第一个消息 --- */
        else if (first->priority == priority)
        {
            /* ---------- 找到与新消息同优先级的最后一个消息 -------------- */
            while( (first != NULL) && (first->priority == priority) )
            {
                ptr = first;
                first = first->next;
            }
            return ptr;
        }
        /* ---------- 比新消息的优先级更小,继续下一消息 -------------- */
        ptr = first;
        first = first->next;
    }

    return ptr;
}

/* ------------------------------------------------------
   功能描述:  向消息队列里插入新消息
   参数说明:  priority:消息的优先级
              Msg     :指向新消息的指针
		      ptr     :指向要新消息插入的位置
--------------------------------------------------------- */
void CMessage::MsgQInsert(int priority,struct MESSAGE *Msg)
{
    struct MESSAGE *ptr;

    /* ----- 获取新消息插入队列的位置 ----- */
    ptr = FindInsertPosition(priority);
    if (ptr == NULL)                   /* ----- 消息队列为空队列 ------ */
    {
        g_MsgQ->msg_first = Msg;
        g_MsgQ->msg_last = Msg;
        g_MsgQ->msg_qnum = g_MsgQ->msg_qnum + 1;
    }
    else if (ptr == g_MsgQ->msg_first
             && ptr->priority > priority)/* --- 新消息插在消息队列的队头 --- */
    {
        Msg->next = ptr;
        g_MsgQ->msg_first = Msg;
        g_MsgQ->msg_qnum = g_MsgQ->msg_qnum + 1;
    }
    else if (ptr->next == NULL)       /* --- 新消息插在消息队列的队尾 --- */
    {
        ptr->next = Msg;
        g_MsgQ->msg_last = Msg;
        g_MsgQ->msg_qnum = g_MsgQ->msg_qnum + 1;
    }
    else                            /* --- 新消息插在ptr的后面 --- */
    {
        Msg->next = ptr->next;
        ptr->next = Msg;
        g_MsgQ->msg_qnum = g_MsgQ->msg_qnum + 1;
    }
}


/* ------------------------------------------------------
   功能描述:  把服务器接收到的报文生成消息，
              并且放入消息队列
   参数说明:  priority:消息的优先级 
		      buffer  :指向刚接收的报文
   返回值说明:成功返回1,失败返回0
--------------------------------------------------------- */
int CMessage::MsgWrite(int sockfd,int priority,char *buffer,int message_len,char *clnip,int port)
{
	struct MESSAGE *Msg;
	char           errmsg[500];

	Msg = (struct MESSAGE *) malloc (LEN_MSGSTRU);

	/* --- 内存分配失败 --- */
	if (g_MsgQ == NULL) 
	{
		return -1;
	}
	/* --- 内存分配成功 --- */
	else             
	{ 
		/* --- 生成消息 --- */
		Msg->length = message_len;
		Msg->priority = priority;
		Msg->sockfd = sockfd;
		memset(Msg->data,0x00,sizeof(Msg->data));
		memcpy(Msg->data,buffer,message_len);
		strcpy(Msg->clnip,clnip);
		Msg->port = port;
		Msg->next = NULL;
	}

	/* --- 向消息队列里插入新消息 --- */
	MsgQInsert(priority,Msg);

	return 1;
}

 
/* ------------------------------------------------------
   功能描述:  删除消息
   参数说明:  first   :指向要要删除的节点
	      ptr     :指向要删除节点的前一个节点
--------------------------------------------------------- */
void CMessage::MsgDelete()
{
    struct MESSAGE *first,*last;

    first = g_MsgQ->msg_first;
    last  = g_MsgQ->msg_last;

    g_MsgQ->msg_first = first->next;
    g_MsgQ->msg_qnum = g_MsgQ->msg_qnum - 1;

    /* --- 消息队列中只有一条消息 --- */
    if (first == last)
    {
        g_MsgQ->msg_last = NULL;
    }

    free(first);
    first = NULL;
}

/* ------------------------------------------------------
   功能描述:  从消息队列里读取第一条消息
   参数说明:  buffer:  应用进程提供的存储报文的内存
   返回值说明:消息队列不为空,返回报文发送方的socket句柄
              否则返回0
--------------------------------------------------------- */
int  CMessage::MsgRead(char *buffer,int *inform_type,char *clnip,int &port)
{
    struct MESSAGE *first;
    int            sockfd;

    sem_wait(g_SemMsg);      /* --- 获得信号量 --- */

    first = g_MsgQ->msg_first;
    if (first == NULL)                   /* --- 消息队列为空 --- */
    {
        sem_post(g_SemMsg);          /* --- 释放信号量 --- */
        return -5 ; //MSGQ_EMPTY_ERROR -5 /* --- 消息队列为空 ----------------- */
    }
    else
    {
        memcpy(buffer,first->data,first->length);
				sockfd = first->sockfd;
				*inform_type = first->priority;
				strcpy(clnip,first->clnip);
				port = first->port;
				MsgDelete();   /* --- 删除成功读取的消息 --- */ 
    }

    sem_post(g_SemMsg);      /* --- 释放信号量 --- */

    return sockfd;
}

/* ------------------------------------------------------
        功能描述:  从消息队列里读取所有消息
        1: --- 设置退出侦听服务标志 g_StopFlag ---
--------------------------------------------------------- */
int  CMessage::MsgReadAll(void)
{
    int            msg_qnum;

    while(1)
    {
        msg_qnum = g_MsgQ->msg_qnum;
        if (msg_qnum != 0)
        {
            sem_post(&g_SemReadMsg);
        }
        else   /* --- 消息队列为空 --- */
        {
            break;
        }
    }

    return 1;
}

int   CMessage::Create_Sem()
{
	  sem_init(&g_SemReadMsg,0,0);
    sem_init(g_SemMsg,0,1);
    return 1;
}

void      CMessage::Release_Sem()
{
		sem_destroy(g_SemMsg);
    free(g_SemMsg);      /* --- 释放信号量(同步消息队列) ---------- */
    g_SemMsg      = NULL;
}
    

int CMessage::Initial_MsgQ()
{
  

    g_MsgQ = (struct MESSAGE_QUEUE *) malloc (sizeof(MESSAGE_QUEUE));
    if (g_MsgQ == NULL)
    {
        return 0;
    }
    else
    {
        g_MsgQ->msg_qnum = 0;
        g_MsgQ->msg_first = NULL;
        g_MsgQ->msg_last = NULL;
    }

    return 1;
}

/* ------------------------------------------------------
             功能描述:  释放消息队列
--------------------------------------------------------- */
void CMessage::MsgQRelease()
{
    struct MESSAGE *first,*last;

    first = g_MsgQ->msg_first;
    last = g_MsgQ->msg_last;
    if (first == NULL)   /* --- 消息队列中没有消息 --- */
    {
        if (g_MsgQ != NULL)
        {
            free(g_MsgQ);
            g_MsgQ = NULL;
        }
        return;
    }

    /* --- 消息队列不为空 --- */
    while(first != last)
    {
        g_MsgQ->msg_first = first->next;
        if (first != NULL)
        {
            free(first);
            first = NULL;
        }
        first = g_MsgQ->msg_first;
    }

    /* --- 释放最后一条消息及消息队列 --- */
    if (first != NULL)
    {
        free(first);
        first = NULL;
    }
    if (g_MsgQ != NULL)
    {
        free(g_MsgQ);
        g_MsgQ = NULL;
    } 
}


