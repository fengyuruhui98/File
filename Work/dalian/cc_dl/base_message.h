//
// Created by HHJT_CJP on 19-12-9.
//

#ifndef _BASE_MESSAGE_H
#define _BASE_MESSAGE_H

#include "sys_head.h"

#define MAX_MSG_NUMBER     128  /* ------- ��Ϣ��������Ϣ�������� ----- */

#ifndef MAX_BAOWEN_LENGTH
#define MAX_BAOWEN_LENGTH  8*1024  /* ------- ��Ϣ������󳤶� ----- */
#endif

#define LEN_MSGSTRU		    sizeof(struct MESSAGE)
#define LEN_MSGQTRU		    sizeof(struct MESSAGE_QUEUE)

/* ----------------------------------------------------- */
/*                 ��Ϣ���е���ض���                    */
/* ----------------------------------------------------- */
#pragma pack (1)
 
struct MESSAGE   /*--- ��Ϣ�ṹ ---*/
{
	int		 length;                   /* ------- ��Ϣ���� ------------------ */
	int		 priority;                 /* ------- ��Ϣ���ȼ� ---------------- */
	int		 sockfd;                   /* ------- ���ķ��ͷ���socket��� ---- */
	char   data[MAX_BAOWEN_LENGTH + 3]; /* ------- ���� ---------------------- */
	char   clnip[16];
	int    port;
	struct MESSAGE  *next;                    /* ------- ָ����һ����Ϣ ------------ */
};

struct MESSAGE_QUEUE  /*--- ��Ϣ���нṹ ---*/
{
	int              msg_qnum;                /* ------- ��Ϣ����------------- */
	struct MESSAGE   *msg_first;              /* ------- ָ���һ����Ϣ ---��-- */
	struct MESSAGE   *msg_last;               /* ------- ָ�����һ����Ϣ ----- */
};
 
#pragma pack ()


class CMessage 
{
public:
    static MESSAGE_QUEUE    *g_MsgQ;
    static sem_t            *g_SemMsg;
    static sem_t            g_SemReadMsg;
public:

    static struct MESSAGE* FindInsertPosition(int priority);
    static void MsgQInsert(int priority,struct MESSAGE *Msg);
    static int  MsgWrite(int sockfd,int priority,char *buffer,int message_len,char *clnip,int port);
    static int  MsgRead(char *buffer,int *inform_type,char *clnip,int &port);
    static int  MsgReadAll(void);
    static void MsgDelete();
    
    static int       Create_Sem();
    static void      Release_Sem();
    static int 			Initial_MsgQ();  //��ʼ����Ϣ����
    static void 		MsgQRelease();  //�ͷ���Ϣ����

};


#endif
