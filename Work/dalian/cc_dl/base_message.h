//
// Created by HHJT_CJP on 19-12-9.
//

#ifndef _BASE_MESSAGE_H
#define _BASE_MESSAGE_H

#include "sys_head.h"

#define MAX_MSG_NUMBER     128  /* ------- 消息队列中消息的最大个数 ----- */

#ifndef MAX_BAOWEN_LENGTH
#define MAX_BAOWEN_LENGTH  8*1024  /* ------- 消息报文最大长度 ----- */
#endif

#define LEN_MSGSTRU		    sizeof(struct MESSAGE)
#define LEN_MSGQTRU		    sizeof(struct MESSAGE_QUEUE)

/* ----------------------------------------------------- */
/*                 消息队列的相关定义                    */
/* ----------------------------------------------------- */
#pragma pack (1)
 
struct MESSAGE   /*--- 消息结构 ---*/
{
	int		 length;                   /* ------- 消息长度 ------------------ */
	int		 priority;                 /* ------- 消息优先级 ---------------- */
	int		 sockfd;                   /* ------- 报文发送方的socket句柄 ---- */
	char   data[MAX_BAOWEN_LENGTH + 3]; /* ------- 数据 ---------------------- */
	char   clnip[16];
	int    port;
	struct MESSAGE  *next;                    /* ------- 指向下一条消息 ------------ */
};

struct MESSAGE_QUEUE  /*--- 消息队列结构 ---*/
{
	int              msg_qnum;                /* ------- 消息个数------------- */
	struct MESSAGE   *msg_first;              /* ------- 指向第一条消息 ---－-- */
	struct MESSAGE   *msg_last;               /* ------- 指向最后一条消息 ----- */
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
    static int 			Initial_MsgQ();  //初始化消息队列
    static void 		MsgQRelease();  //释放消息队列

};


#endif
