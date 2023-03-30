
#include "base_message.h"

/* ------------------------------------------------------
��������:   ��Ϣ���г�ʼ��
����ֵ˵��: �ɹ�����1,ʧ�ܷ���0
--------------------------------------------------------- */
MESSAGE_QUEUE * CMessage::g_MsgQ ;
sem_t    *CMessage::g_SemMsg = (sem_t*) calloc (1,sizeof(sem_t));  /* --- �ź�������ռ�(ͬ����Ϣ����) ---------- */
sem_t   CMessage::g_SemReadMsg;

/* ------------------------------------------------------
   ��������:  ��ȡ����Ϣ������е�λ��, ����λ��
              Ϊ��Ϣ������ͬ�����ȼ���Ϣ��β��
   ����˵��:  priority:��������Ϣ�����ȼ�
              first   :ָ��Ҫ�ҽڵ�ĺ�һλ��
		      ptr     :ָ��Ҫ�ҽڵ��λ��,����Ϣ����ptr����
   ����ֵ˵��:������Ϣ�����λ��,����NULL��ʾ��Ϣ����Ϊ��
--------------------------------------------------------- */
MESSAGE* CMessage::FindInsertPosition(int priority)
{
    struct MESSAGE *first,*ptr;

    ptr = NULL;
    first = g_MsgQ->msg_first;

    while(first != NULL)
    {
        /* --- �ҵ�������Ϣ���ȼ���ĵ�һ����Ϣ --- */
        if (first->priority > priority)
        {
            /* --- ����Ϣ������Ϣ���еĶ�ͷ --- */
            if (ptr == NULL)
            {
                return first;
            }
                /* ---------- ����Ϣ����ptr�ĺ��� -------------- */
            else
            {
                return ptr;
            }
        }
            /* --- �ҵ�������Ϣͬ���ȼ��ĵ�һ����Ϣ --- */
        else if (first->priority == priority)
        {
            /* ---------- �ҵ�������Ϣͬ���ȼ������һ����Ϣ -------------- */
            while( (first != NULL) && (first->priority == priority) )
            {
                ptr = first;
                first = first->next;
            }
            return ptr;
        }
        /* ---------- ������Ϣ�����ȼ���С,������һ��Ϣ -------------- */
        ptr = first;
        first = first->next;
    }

    return ptr;
}

/* ------------------------------------------------------
   ��������:  ����Ϣ�������������Ϣ
   ����˵��:  priority:��Ϣ�����ȼ�
              Msg     :ָ������Ϣ��ָ��
		      ptr     :ָ��Ҫ����Ϣ�����λ��
--------------------------------------------------------- */
void CMessage::MsgQInsert(int priority,struct MESSAGE *Msg)
{
    struct MESSAGE *ptr;

    /* ----- ��ȡ����Ϣ������е�λ�� ----- */
    ptr = FindInsertPosition(priority);
    if (ptr == NULL)                   /* ----- ��Ϣ����Ϊ�ն��� ------ */
    {
        g_MsgQ->msg_first = Msg;
        g_MsgQ->msg_last = Msg;
        g_MsgQ->msg_qnum = g_MsgQ->msg_qnum + 1;
    }
    else if (ptr == g_MsgQ->msg_first
             && ptr->priority > priority)/* --- ����Ϣ������Ϣ���еĶ�ͷ --- */
    {
        Msg->next = ptr;
        g_MsgQ->msg_first = Msg;
        g_MsgQ->msg_qnum = g_MsgQ->msg_qnum + 1;
    }
    else if (ptr->next == NULL)       /* --- ����Ϣ������Ϣ���еĶ�β --- */
    {
        ptr->next = Msg;
        g_MsgQ->msg_last = Msg;
        g_MsgQ->msg_qnum = g_MsgQ->msg_qnum + 1;
    }
    else                            /* --- ����Ϣ����ptr�ĺ��� --- */
    {
        Msg->next = ptr->next;
        ptr->next = Msg;
        g_MsgQ->msg_qnum = g_MsgQ->msg_qnum + 1;
    }
}


/* ------------------------------------------------------
   ��������:  �ѷ��������յ��ı���������Ϣ��
              ���ҷ�����Ϣ����
   ����˵��:  priority:��Ϣ�����ȼ� 
		      buffer  :ָ��ս��յı���
   ����ֵ˵��:�ɹ�����1,ʧ�ܷ���0
--------------------------------------------------------- */
int CMessage::MsgWrite(int sockfd,int priority,char *buffer,int message_len,char *clnip,int port)
{
	struct MESSAGE *Msg;
	char           errmsg[500];

	Msg = (struct MESSAGE *) malloc (LEN_MSGSTRU);

	/* --- �ڴ����ʧ�� --- */
	if (g_MsgQ == NULL) 
	{
		return -1;
	}
	/* --- �ڴ����ɹ� --- */
	else             
	{ 
		/* --- ������Ϣ --- */
		Msg->length = message_len;
		Msg->priority = priority;
		Msg->sockfd = sockfd;
		memset(Msg->data,0x00,sizeof(Msg->data));
		memcpy(Msg->data,buffer,message_len);
		strcpy(Msg->clnip,clnip);
		Msg->port = port;
		Msg->next = NULL;
	}

	/* --- ����Ϣ�������������Ϣ --- */
	MsgQInsert(priority,Msg);

	return 1;
}

 
/* ------------------------------------------------------
   ��������:  ɾ����Ϣ
   ����˵��:  first   :ָ��ҪҪɾ���Ľڵ�
	      ptr     :ָ��Ҫɾ���ڵ��ǰһ���ڵ�
--------------------------------------------------------- */
void CMessage::MsgDelete()
{
    struct MESSAGE *first,*last;

    first = g_MsgQ->msg_first;
    last  = g_MsgQ->msg_last;

    g_MsgQ->msg_first = first->next;
    g_MsgQ->msg_qnum = g_MsgQ->msg_qnum - 1;

    /* --- ��Ϣ������ֻ��һ����Ϣ --- */
    if (first == last)
    {
        g_MsgQ->msg_last = NULL;
    }

    free(first);
    first = NULL;
}

/* ------------------------------------------------------
   ��������:  ����Ϣ�������ȡ��һ����Ϣ
   ����˵��:  buffer:  Ӧ�ý����ṩ�Ĵ洢���ĵ��ڴ�
   ����ֵ˵��:��Ϣ���в�Ϊ��,���ر��ķ��ͷ���socket���
              ���򷵻�0
--------------------------------------------------------- */
int  CMessage::MsgRead(char *buffer,int *inform_type,char *clnip,int &port)
{
    struct MESSAGE *first;
    int            sockfd;

    sem_wait(g_SemMsg);      /* --- ����ź��� --- */

    first = g_MsgQ->msg_first;
    if (first == NULL)                   /* --- ��Ϣ����Ϊ�� --- */
    {
        sem_post(g_SemMsg);          /* --- �ͷ��ź��� --- */
        return -5 ; //MSGQ_EMPTY_ERROR -5 /* --- ��Ϣ����Ϊ�� ----------------- */
    }
    else
    {
        memcpy(buffer,first->data,first->length);
				sockfd = first->sockfd;
				*inform_type = first->priority;
				strcpy(clnip,first->clnip);
				port = first->port;
				MsgDelete();   /* --- ɾ���ɹ���ȡ����Ϣ --- */ 
    }

    sem_post(g_SemMsg);      /* --- �ͷ��ź��� --- */

    return sockfd;
}

/* ------------------------------------------------------
        ��������:  ����Ϣ�������ȡ������Ϣ
        1: --- �����˳����������־ g_StopFlag ---
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
        else   /* --- ��Ϣ����Ϊ�� --- */
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
    free(g_SemMsg);      /* --- �ͷ��ź���(ͬ����Ϣ����) ---------- */
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
             ��������:  �ͷ���Ϣ����
--------------------------------------------------------- */
void CMessage::MsgQRelease()
{
    struct MESSAGE *first,*last;

    first = g_MsgQ->msg_first;
    last = g_MsgQ->msg_last;
    if (first == NULL)   /* --- ��Ϣ������û����Ϣ --- */
    {
        if (g_MsgQ != NULL)
        {
            free(g_MsgQ);
            g_MsgQ = NULL;
        }
        return;
    }

    /* --- ��Ϣ���в�Ϊ�� --- */
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

    /* --- �ͷ����һ����Ϣ����Ϣ���� --- */
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


