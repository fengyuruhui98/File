
#include "base_socket.h"


char CSocket::ms_str_SocketErrorMessage[1024];
int CSocket::Tcp_GetSocketErrorMsg(const char *buffer,unsigned int error)
{
    int tcpErrorCode;
    memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));

    switch(error)
    {
        case ENOTSOCK: /* --- Socket operation no nonsocket ---*/
            tcpErrorCode = TCP_ERROR_ENOTSOCK;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_ENOTSOCK\n",buffer);
            break;

        case ENOBUFS: /* --- No buffer space available ---*/
            tcpErrorCode = TCP_ERROR_ENOBUFS;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_ENOBUFS\n",buffer);
            break;

        case EMSGSIZE: /* --- Message is too long ---*/
            tcpErrorCode = TCP_ERROR_EMSGSIZE;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_EMSGSIZE\n",buffer);
            break;

        case ENOTCONN: /* --- Socket is not connected ---*/
            tcpErrorCode = TCP_ERROR_ENOTCONN;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_ENOTCONN\n",buffer);
            break;

        case EISCONN: /* --- Socket is already connected ---*/
            tcpErrorCode = TCP_ERROR_EISCONN;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_EISCONN\n",buffer);
            break;

        case ECONNABORTED: /* --- Software caused connection abort ---*/
            tcpErrorCode = TCP_ERROR_ECONNABORTED;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_ECONNABORTED\n",buffer);
            break;

        case ECONNREFUSED: /* --- Connection refused ---*/
            tcpErrorCode = TCP_ERROR_ECONNREFUSED;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_ECONNREFUSED\n",buffer);
            break;

        case ECONNRESET: /* --- Connection reset by peer ---*/
            tcpErrorCode = TCP_ERROR_ECONNRESET;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_ECONNRESET\n",buffer);
            break;

        case ETIMEDOUT: /* --- Connection timed out ---*/
            tcpErrorCode = TCP_ERROR_ETIMEDOUT;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_ETIMEDOUT\n",buffer);
            break;

        case ENETDOWN: /* --- Network is down ---*/
            tcpErrorCode = TCP_ERROR_ENETDOWN;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_ENETDOWN\n",buffer);
            break;

        case ENETUNREACH: /* --- Network is unreachable ---*/
            tcpErrorCode = TCP_ERROR_ENETUNREACH;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_ENETUNREACH\n",buffer);
            break;

        case EHOSTUNREACH: /* --- Network dropped connection on reset ---*/
            tcpErrorCode = TCP_ERROR_EHOSTUNREACH;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_EHOSTUNREACH\n",buffer);
            break;

        case EHOSTDOWN: /* --- Network subsystem is unavailable ---*/
            tcpErrorCode = TCP_ERROR_EHOSTDOWN;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_EHOSTDOWN\n",buffer);
            break;

        case EALREADY: /* --- No route to host ---*/
            tcpErrorCode = TCP_ERROR_EALREADY;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_EALREADY\n",buffer);
            break;

        case EADDRINUSE: /* --- Address already in use ---*/
            tcpErrorCode = TCP_ERROR_EADDRINUSE;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_EADDRINUSE\n",buffer);
            break;

        case EWOULDBLOCK: /* --- Resource temporarily unavailable ---*/
            tcpErrorCode = TCP_ERROR_EWOULDBLOCK;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_EWOULDBLOCK\n",buffer);
            break;

        case EACCES: /* --- Permission denied ---*/
            tcpErrorCode = TCP_ERROR_EACCES;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_EACCES\n",buffer);
            break;

        case EIO: /* --- Permission denied ---*/
            tcpErrorCode = TCP_ERROR_EIO;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_EIO\n",buffer);
            break;

        case EFAULT: /* --- Bad address ---*/
            tcpErrorCode = TCP_ERROR_EFAULT;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_EFAULT\n",buffer);
            break;

        case EINVAL: /* --- Invalid argument ---*/
            tcpErrorCode = TCP_ERROR_EINVAL;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_EINVAL\n",buffer);
            break;

        case EBADF: /* --- Too many open files(sockets) ---*/
            tcpErrorCode = TCP_ERROR_EBADF;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_EBADF\n",buffer);
            break;

        case EMFILE: /* --- Too many open files(sockets) ---*/
            tcpErrorCode = TCP_ERROR_EMFILE;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_EMFILE\n",buffer);
            break;

        case ENFILE: /* --- Too many open files(sockets) ---*/
            tcpErrorCode = TCP_ERROR_ENFILE;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_ENFILE\n",buffer);
            break;

        case EINPROGRESS: /* --- Operaton now in progress ---*/
            tcpErrorCode = TCP_ERROR_EINPROGRESS;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_EINPROGRESS\n",buffer);
            break;

        case ENOMEM: /* --- Operaton now in progress ---*/
            tcpErrorCode = TCP_ERROR_ENOMEM;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_ENOMEM\n",buffer);
            break;

        case EDESTADDRREQ: /* --- Destination address required ---*/
            tcpErrorCode = TCP_ERROR_EDESTADDRREQ;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_EDESTADDRREQ\n",buffer);
            break;

        case EPROTOTYPE: /* --- Protocol wrong type for socket ---*/
            tcpErrorCode = TCP_ERROR_EPROTOTYPE;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_EPROTOTYPE\n",buffer);
            break;

        case ENOPROTOOPT: /* --- Bad protocol option ---*/
            tcpErrorCode = TCP_ERROR_ENOPROTOOPT;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_ENOPROTOOPT\n",buffer);
            break;

        case EPROTONOSUPPORT: /* --- Protocol not supported ---*/
            tcpErrorCode = TCP_ERROR_EPROTONOSUPPORT;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_EPROTONOSUPPORT\n",buffer);
            break;

        case ESOCKTNOSUPPORT: /* --- Socket type not supported ---*/
            tcpErrorCode = TCP_ERROR_ESOCKTNOSUPPORT;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_ESOCKTNOSUPPORT\n",buffer);
            break;

        case EOPNOTSUPP: /* --- Operation not supported ---*/
            tcpErrorCode = TCP_ERROR_EOPNOTSUPP;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_EOPNOTSUPP\n",buffer);
            break;

        case EPFNOSUPPORT: /* --- Protocol family not supported ---*/
            tcpErrorCode = TCP_ERROR_EPFNOSUPPORT;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_EPFNOSUPPORT\n",buffer);
            break;

        case EAFNOSUPPORT: /* --- Address family not supported by protocol family ---*/
            tcpErrorCode = TCP_ERROR_EAFNOSUPPORT;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_EAFNOSUPPORT\n",buffer);
            break;

        case EADDRNOTAVAIL: /* --- Cannot assign requested address ---*/
            tcpErrorCode = TCP_ERROR_EADDRNOTAVAIL;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_EADDRNOTAVAIL\n",buffer);
            break;

        case EINTR: /* --- Interrupted function call ---*/
            tcpErrorCode = TCP_ERROR_EINTR;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_EINTR\n",buffer);
            break;

        default: /* --- Other errors ---*/
            tcpErrorCode = TCP_ERROR_DEFAULT;
            sprintf(ms_str_SocketErrorMessage,"%s:TCP_ERROR_DEFAULT\n",buffer);
            break;
    }

 
 		//�ڴ���ms_str_SocketErrorMessageǰ����ϸô��������ʱ��,����׷��
    time_t    long_time;
    struct tm newtime;
    struct timeval t_msec;
    char bufferError[2048];

    /* --- ���� --- */
    gettimeofday(&t_msec,NULL);
    time(&long_time);
    memset(&newtime,0x00,sizeof(struct tm));
    localtime_r(&long_time,&newtime);
    sprintf(bufferError,"[%02d:%02d:%02d:%03d %04d��%02d��%02d��]\t",newtime.tm_hour,newtime.tm_min,newtime.tm_sec,t_msec.tv_usec/1000,
            (newtime.tm_year+1900),(newtime.tm_mon+1),newtime.tm_mday);

		strcat(bufferError,"\t");
    strcat(bufferError,ms_str_SocketErrorMessage);
 		memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
 		strcpy(ms_str_SocketErrorMessage,bufferError);
 
    return tcpErrorCode;
}

/* ------------------------------------------------------
            ��������:   ����socket���
			����ֵ˵��: �ɹ����ؾ��,ʧ�ܷ��ش������
--------------------------------------------------------- */
int CSocket::Tcp_Socket(int type)
{
    int sockfd;

    sockfd = socket(AF_INET,type,0);
    if (sockfd < 0)
    {
        sockfd = Tcp_GetSocketErrorMsg("socket",errno);
    }

    return sockfd;
}

/* ------------------------------------------------------
            ��������:   ���÷��ͻ����������ջ������Ĵ�С
			����ֵ˵��: ���óɹ�����1,ʧ�ܷ��ش������
--------------------------------------------------------- */
int CSocket::Tcp_SetSocketBuf(int sockfd)
{
    int SndBufLen,RcvBufLen;
    int ret1,ret2;

    SndBufLen = SEND_BUFFER_LENGTH;
    RcvBufLen = RECV_BUFFER_LENGTH;
    ret1 = setsockopt(sockfd,SOL_SOCKET,SO_SNDBUF,(char *)&SndBufLen,sizeof(SndBufLen));
    ret2 = setsockopt(sockfd,SOL_SOCKET,SO_RCVBUF,(char*)&RcvBufLen,sizeof(RcvBufLen));

    if (ret1 == 0 && ret2 == 0)
    {
        return 1;
    }
    else
    {
        return(Tcp_GetSocketErrorMsg("setsockopt",errno));
    }
}

/* ------------------------------------------------------
   ��������:  ������������������
   ����˵��:  port: �����Ķ˿ں�
   ����ֵ˵��:�ɹ�����������socket���,ʧ�ܷ��ش������
--------------------------------------------------------- */
int CSocket::Tcp_Listen(int port)
{
    int                  listenfd;
    int                  ret;
    struct sockaddr_in   servaddr;
    int                  ReusePort;

    /* --- ����socket���--- */
    listenfd = Tcp_Socket(SOCK_STREAM);
    if (listenfd < 0)
    {
        return listenfd;
    }

    memset(&servaddr,0x00,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons((unsigned short)port);

    /* --- �󶨶˿� --- */
    ReusePort = 1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&ReusePort,sizeof(ReusePort));
    ret = bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    if (ret != 0)
    {
        ret = Tcp_GetSocketErrorMsg("bind",errno);
        Tcp_Close(listenfd);
        return ret;
    }

    /* --- ���÷��ͻ������ͽ��ջ����� --- */
    ret = Tcp_SetSocketBuf(listenfd);
    if (ret < 0)
    {
        Tcp_Close(listenfd);
        return ret;
    }

    /* --- ���� --- */
    ret = listen(listenfd,5);
    if (ret != 0)
    {
        ret = Tcp_GetSocketErrorMsg("listen",errno);
        Tcp_Close(listenfd);
        return ret;
    }

    return listenfd;
}

/* ------------------------------------------------------
            ��������:   �ر�socket���
			����ֵ˵��: �ɹ�����1,ʧ�ܷ��ش������
--------------------------------------------------------- */
int CSocket::Tcp_Close(int sockfd)
{
    int   ret;
    unsigned int error;

    if(sockfd <= 0)
        return TCP_ERROR_ENOTSOCK;

    ret = close(sockfd);
    if (ret != 0)
    {
        error = errno;
        /* --- �ر�socket���ʧ�ܷ��ش������ --- */
        if (error != ENOTSOCK && error != EBADF)
        {
            ret = Tcp_GetSocketErrorMsg("close",error);
            return ret;
        }
        else
        {
            return TCP_ERROR_ENOTSOCK;
        }
    }
    else
    {
        return 1;
    }
}

/* ------------------------------------------------------
            ��������:   �������Կͻ��˵���������
            ����ֵ˵��: �ɹ��������ӵ�socket���,ʧ�ܷ���0
--------------------------------------------------------- */
int CSocket::Tcp_Accept(int listenfd,char *clnip,int &port)
{
    int   							ret,connfd;
    struct sockaddr_in  cln_adr;
    socklen_t						cln_len;

    /* --- �ȴ����Կͻ��˵��������� --- */
    while(1)
    {
        memset(&cln_adr,0x00,sizeof(cln_adr));
        cln_len = sizeof(cln_adr);
        connfd = accept(listenfd,(struct sockaddr*)&cln_adr,&cln_len);
        /* --- ��������ʧ�� --- */
        if (connfd < 0)
        {
            ret = Tcp_GetSocketErrorMsg("accept",errno);
            return ret;
        }
        else
        {
            /* --- �ͻ������ӵ���Ϣ --- */
						strcpy(clnip,inet_ntoa(cln_adr.sin_addr));
						port = htons(cln_adr.sin_port);

            return connfd;
        }
    }
}


/* ------------------------------------------------------
   ��������:   ��������˷�����������
   ����˵��:   server_ip:  �������˵�IP��ַ
               server_port:�������������Ķ˿ں�
   ����ֵ˵��: �ɹ��������ӵ�socket���,ʧ�ܷ��ش������
--------------------------------------------------------- */
int CSocket::Tcp_Connect(char *server_ip,int server_port,int timeout)
{
    int                 sockfd;
    int                 ret,error,len,flags;
    struct sockaddr_in  server_adr;
    fd_set              read_fd,write_fd;
    struct timeval      timeoutSTRU;
    char                errmsg[100];
    char                info[100];

    /* --- ����socket��� --- */
    sockfd = Tcp_Socket(SOCK_STREAM);
    if (sockfd < 0)
    {
        return sockfd;
    }

    /* --- ���÷�������IP�Ͷ˿� --- */
    memset(&server_adr,0x00,sizeof(server_adr));
    server_adr.sin_family = AF_INET;
    server_adr.sin_addr.s_addr = inet_addr(server_ip);
    server_adr.sin_port = htons((unsigned short)server_port);

    /* --- ���socket������־ --- */
    flags = fcntl(sockfd,F_GETFL,0);

    ret = Tcp_SetSocketBuf(sockfd);            /* --- ���÷��ͻ������ͽ��ջ����� --- */
    if (ret < 0)  /* --- ����ʧ�� --- */
    {
        Tcp_Close(sockfd);
        return ret;
    }

    error = 0;
    fcntl(sockfd,F_SETFL,flags | O_NONBLOCK);  /* --- ����Ϊ������socket --- */

    ret = connect(sockfd,(struct sockaddr *)&server_adr,sizeof(server_adr));/* --- ��������˽������� --- */

    if (ret < 0)  /* --- ����ʧ�� --- */
    {
        memset(info,0x00,sizeof(info));
        sprintf(info,"connect %s",server_ip);
        ret = Tcp_GetSocketErrorMsg(info,errno);
        if (ret != TCP_ERROR_EINPROGRESS)  /* - -- �����óɷ������������Ӳ���������� --- */
        {
            Tcp_Close(sockfd);
            return ret;
        }
    }
    if (ret == 0)         /* --- ����������� --- */
    {
        goto done;
    }

    /* --- ���������ó�ʱ --- */
    FD_ZERO(&read_fd);
    FD_SET(sockfd,&read_fd);
    write_fd = read_fd;
    timeoutSTRU.tv_sec  = timeout / 1000;
    timeoutSTRU.tv_usec = timeout % 1000;
    ret = select(sockfd+1,&read_fd,&write_fd,NULL,&timeoutSTRU);

    if (ret == 0)        /* --- ���ӳ�ʱ --- */
    {
        errno = ETIMEDOUT;
        memset(info,0x00,sizeof(info));
        sprintf(info,"connect %s",server_ip);
        ret = Tcp_GetSocketErrorMsg(info,errno);
        Tcp_Close(sockfd);	 /* --- �ر��׽��� --- */
        return ret;
    }
    if (ret < 0)          /* --- select ���� --- */
    {
        memset(info,0x00,sizeof(info));
        sprintf(info,"connect(select) %s",server_ip);
        ret = Tcp_GetSocketErrorMsg(info,errno);
        Tcp_Close(sockfd);
        return ret;
    }

    if (FD_ISSET(sockfd,&read_fd) || FD_ISSET(sockfd,&write_fd))
    {
        len = sizeof(error);
        if (getsockopt(sockfd,SOL_SOCKET,SO_ERROR,&error,( socklen_t *)&len) < 0)
        {
            memset(info,0x00,sizeof(info));
            sprintf(info,"connect(getsockopt) %s",server_ip);
            ret = Tcp_GetSocketErrorMsg(info,errno);
            Tcp_Close(sockfd);
            return ret;
        }
    }
done:
    fcntl(sockfd,F_SETFL,flags);    /* --- ��������Ϊ����socket --- */
    if (error)
    {
        Tcp_Close(sockfd);
        errno = error;
        memset(info,0x00,sizeof(info));
        sprintf(info,"connect %s",server_ip);
        ret = Tcp_GetSocketErrorMsg(info,errno);
        return ret;
    }

    return sockfd;
}

/* ------------------------------------------------------
   ��������:  ��ͻ��˷���Ӧ��
   ����˵��:  sockfd:   socket���
              buffer:   ���ͻ�������ָ��
		      send_len: ���͵ĳ���
			  nleft:    δ���͵��ֽ���
			  nsend:    ����һ��send���͵��ֽ���
   ����ֵ˵��:���ͳɹ�����1,ʧ�ܷ��ش������;
   ������Ϣ�����ڸ���ľ�̬����(ms_str_SocketErrorMessage)��
--------------------------------------------------------- */
int CSocket::Tcp_Send(int sockfd,char *buffer,int snd_len)
{
    int             nleft,nsend;
    int             ret;
    char            *ptr;

    if (snd_len > MAX_BAOWEN_LENGTH)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        sprintf(ms_str_SocketErrorMessage,"���Ĵ�С����%dK,���ܷ���!\n",MAX_BAOWEN_LENGTH);
        return -6; //-6 PACKAGE_TOO_BIG_ERROR
    }

    nleft = snd_len;
    ptr = buffer;
    struct timeval timeout;
    /* --- ���ó�ʱ --- */
    timeout.tv_sec = 15;
    timeout.tv_usec = 0;
    setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(timeout));

    /* --- ѭ������send����,ֱ������send_len���ֽ������߷���ʧ�� --- */
    while(nleft > 0)
    {
        nsend = send(sockfd,ptr,nleft,MSG_NOSIGNAL);
        if (nsend < 0) /* --- ����ʧ�ܷ��ش������ --- */
        {
            ret = Tcp_GetSocketErrorMsg("send",errno);
            /* --- ��ֹ��ʱ --- */
            timeout.tv_sec = 0;
            timeout.tv_usec = 0;
            setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(timeout));

            return ret;
        }
        else
        {
            nleft -= nsend;
            if (nleft != 0)
            {
                ptr += nsend;
            }
        }
    }
    /* --- ��ֹ��ʱ --- */
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(timeout));

    return 1;            /* --- ���ͳɹ�����1 --- */
}

/* ------------------------------------------------------
   ��������:  socket ���պ���
   ����˵��:  sockfd: socket���
              buffer: ���ջ�����
			  rcv_len:���յĳ���
   ����ֵ˵��:�ɹ����ؽ��յĳ���,�ͻ��˶Ͽ����ӷ���0,
              ʧ�ܷ��ش������
--------------------------------------------------------- */
int CSocket::Tcp_Recv(int sockfd,char *buffer,int rcv_len)
{
    int    nrcv;

    nrcv =  recv(sockfd,buffer,rcv_len,0);
    if (nrcv < 0)       /* --- recv��������ʧ�� --- */
    {
        nrcv = Tcp_GetSocketErrorMsg("recv",errno);
    }

    return nrcv;
}



/* ------------------------------------------------------
��������:���Ŀ¼����û��Ŀ¼��ʶ��,�����
--------------------------------------------------------- */
void CSocket::Ftp_ModifyPath(char *local_path,char *remote_path)
{
    if(local_path[strlen(local_path) - 1] != '/')
    {
        strcat(local_path,"/");
    }
    if (strchr(remote_path,'/') != NULL)
    {
        if (remote_path[strlen(remote_path) - 1] != '/')
        {
            strcat(remote_path,"/");
        }
    }
    else
    {
        if(remote_path[strlen(remote_path) - 1] != '\\')
        {
            strcat(remote_path,"\\");
        }
    }
}

/* ------------------------------------------------------
            ��������:   �������ӽ����ɹ���,��������˷���
			            �û������������Ϣ
			����ֵ˵��: �ɹ�����1,ʧ�ܷ��ش������
--------------------------------------------------------- */
int CSocket::Ftp_InitCtrlConn(int ctl_conn,char *user,char *pass,char *remote_path,char *fullpath)
{
    int ret;
    char command[100],buffer[BLOCKSIZE];

    /* --- ������������USER���� --- */
    memset(command,0x00,sizeof(command));
    sprintf(command,"USER %s\r\n",user);
    ret = Tcp_Send(ctl_conn,command,strlen(command));
    if (ret < 0)
    {
        return ret;
    }
    /* --- ����Ӧ�� --- */
    memset(buffer,0x00,sizeof(buffer));
    ret = Ftp_RecvFtpSvrMsg(ctl_conn,"331 ","USER",buffer);
    if (ret == 0)
    {
        return FTP_CTLCONNCLOSED_ERROR;
    }
    else if (ret == -1)
    {
        return FTP_USER_ERROR;
    }
    else if (ret < 0)
    {
        return ret;
    }

    /* --- ������������PASS���� --- */
    memset(command,0x00,sizeof(command));
    sprintf(command,"PASS %s\r\n",pass);
    ret = Tcp_Send(ctl_conn,command,strlen(command));
    if (ret < 0)
    {
        return ret;
    }
    /* --- ����Ӧ�� --- */
    memset(buffer,0x00,sizeof(buffer));
    ret = Ftp_RecvFtpSvrMsg(ctl_conn,"230 ","PASS",buffer);
    if (ret == 0)
    {
        return FTP_CTLCONNCLOSED_ERROR;
    }
    else if (ret == -1)
    {
        return FTP_PASS_ERROR;
    }
    else if (ret < 0)
    {
        return ret;
    }

    /* --- ������������CWD���� --- */
    memset(command,0x00,sizeof(command));
    sprintf(command,"CWD %s\r\n",remote_path);
    ret = Tcp_Send(ctl_conn,command,strlen(command));
    if (ret < 0)
    {
        return ret;
    }
    /* --- ����Ӧ�� --- */
    memset(buffer,0x00,sizeof(buffer));
    ret = Ftp_RecvFtpSvrMsg(ctl_conn,"250 ","CWD",buffer);
    if (ret == 0)
    {
        return FTP_CTLCONNCLOSED_ERROR;
    }
    else if (ret == -1)
    {
        return FTP_CWD_ERROR;
    }
    else if (ret < 0)
    {
        return ret;
    }

    /* --- ������������TYPE���� --- */
    memset(command,0x00,sizeof(command));
    strcpy(command,"TYPE I\r\n");
    ret = Tcp_Send(ctl_conn,command,strlen(command));
    if (ret < 0)
    {
        return ret;
    }
    /* --- ����Ӧ�� --- */
    memset(buffer,0x00,sizeof(buffer));
    ret = Ftp_RecvFtpSvrMsg(ctl_conn,"200 ","TYPE",buffer);
    if (ret == 0)
    {
        return FTP_CTLCONNCLOSED_ERROR;
    }
    else if (ret == -1)
    {
        return FTP_TYPE_ERROR;
    }
    else if (ret < 0)
    {
        return ret;
    }

		/* --- ������������PWD���� --- */
		memset(command,0x00,sizeof(command));
		strcpy(command,"PWD\r\n");
		ret = Tcp_Send(ctl_conn,command,strlen(command));
		if (ret < 0)
		{
			return ret;
		}		
		/* --- ����Ӧ�� --- */
		int  index;
		char *p;
		memset(buffer,0x00,sizeof(buffer));
		ret = Ftp_RecvFtpSvrMsg(ctl_conn,"257 ","PWD",buffer);
		p = buffer;
		index = Ftp_FindChar(p,0x22);
		p = p + index + 1;
		index = Ftp_FindChar(p,0x22);
		if(index > 0)
		{
			memcpy(fullpath,p,index);
			if(strchr(fullpath,'/') != NULL)
				if(fullpath[strlen(fullpath) - 1] != '/')
					strcat(fullpath,"/");
			else
				if(fullpath[strlen(fullpath) - 1] != '\\')
					strcat(fullpath,"\\");
		}
	
    return SUCCESS;
}


/* ------------------------------------------------------
            ��������:   �������Է������˵�Ӧ��
			����ֵ˵��: Ӧ����ȷ����1,Ӧ����󷵻�-1,
			            ʧ�ܷ��ش������,���ӶϿ�����0
--------------------------------------------------------- */
int CSocket::Ftp_RecvFtpSvrMsg(int sockfd,const char *code,const char *type,char *buffer)
{
    int    nrcv;
    char   errmsg[500];

    nrcv = Tcp_Recv(sockfd,buffer,BLOCKSIZE);
    if (nrcv < 0)       /* --- recv��������ʧ�� --- */
    {
        ;
    }

    if ( (nrcv > 0) && buffer[0] != *code)   /* --- Ӧ����� --- */
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        sprintf(ms_str_SocketErrorMessage,"%s: %s\n",type,buffer);

        return -1;
    }

    while (1)
    {
        if (nrcv < 0)
        {
            return nrcv;
        }
        if (nrcv == 0)                        /* --- ���ӹر� --- */
        {
            memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
            sprintf(ms_str_SocketErrorMessage,"%s: ���������Ѿ��ر�!\n",type);

            return 0;
        }

        if (strstr(buffer,code) != NULL)
        {
            return SUCCESS;
        }
        if ((strcmp(type,"RETR") == 0))
        {
            if (strstr(buffer,"125 ") != NULL)   /* --- WINDOWS ƽ̨ --- */
            {
                return SUCCESS;
            }
        }
        if ((strcmp(type,"STOR") == 0))
        {
            if (strstr(buffer,"125 ") != NULL)   /* --- WINDOWS ƽ̨ --- */
            {
                return SUCCESS;
            }
        }
        if ((strcmp(type,"LIST") == 0))
        {
            if (strstr(buffer,"125 ") != NULL)   /* --- WINDOWS ƽ̨ --- */
            {
                return SUCCESS;
            }
        }

        /* --- ������ȡ���������ص�������Ϣ --- */
        memset(buffer,0x00,strlen(buffer));
        nrcv = Tcp_Recv(sockfd,buffer,BLOCKSIZE);
        if (nrcv < 0)       /* --- recv��������ʧ�� --- */
        {
            ;
        }
    }
}

/* ------------------------------------------------------
  ��������:   ������������
  ����ֵ˵��: �ɹ������������ӵ�socket���,ʧ�ܷ��ش������
--------------------------------------------------------- */
int CSocket::Ftp_EstablishDataConn(int ctl_conn)
{
    char               *ip,command[100],buffer[BLOCKSIZE];
    char               data_conn_ip[16];
    int                data_conn;
    unsigned int       data_conn_info[10];
    int		           data_conn_port;
    int                ret;

    /* --- PASVģʽ,��÷��������ṩ���������ӵ�IP�Ͷ˿� --- */
    memset(command,0x00,sizeof(command));
    strcpy(command,"PASV\r\n");
    ret = Tcp_Send(ctl_conn,command,strlen(command));
    if (ret < 0)
    {
        return ret;
    }
    memset(buffer,0x00,sizeof(buffer));
    ret = Ftp_RecvFtpSvrMsg(ctl_conn,"227 ","PASV",buffer);
    if (ret == 0)
    {
        return FTP_CTLCONNCLOSED_ERROR;
    }
    else if (ret == -1)
    {
        return FTP_PASV_ERROR;
    }
    else if (ret < 0)
    {
        return ret;
    }

    ip = strchr(buffer,'(');
    if(ip == NULL)
    {
        ip = strstr(buffer,"ing");
        if(ip != NULL)
        {
            sscanf(ip + 4,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                   &data_conn_info[2],&data_conn_info[3],&data_conn_info[4],&data_conn_info[5],
                   &data_conn_info[6],&data_conn_info[7],&data_conn_info[8],&data_conn_info[9],
                   &data_conn_info[0],&data_conn_info[1]);
        }
        else
            return FTP_PASV_ERROR;
    }
    else
    {
        sscanf(ip + 1,"%d,%d,%d,%d,%d,%d",&data_conn_info[2],&data_conn_info[3],&data_conn_info[4],
               &data_conn_info[5],&data_conn_info[0],&data_conn_info[1]);
    }
    sprintf(data_conn_ip,"%d.%d.%d.%d",data_conn_info[2],data_conn_info[3],
            data_conn_info[4],data_conn_info[5]);
    data_conn_port = data_conn_info[0] * 256 + data_conn_info[1];

    data_conn = Tcp_Connect(data_conn_ip,data_conn_port,5000);

    return data_conn;
}

/* ------------------------------------------------------
            ��������:�жϴ����Ƿ����
--------------------------------------------------------- */
int CSocket::Ftp_IsTransferComplete(char *buffer)
{
    char type[7];
    int  ret;

    ret = 0;
    memset(type,0x00,sizeof(type));
    type[0] = 0x0D;
    type[1] = 0x0A;
    strcat(type,"226 ");
    if (strstr(buffer,type) != NULL)
        ret = 1;

    return ret;
}


/* ------------------------------------------------------
            ��������:   �ϴ��ļ�
			����ֵ˵��: �ɹ�����1,ʧ�ܷ��ش������
--------------------------------------------------------- */
int CSocket::Ftp_PutFile(int ctl_conn,char *local_path,char *localname,char *remotename)
{
    char         path[MAX_PATH],buffer[BLOCKSIZE];
    int          ret,count;
    FILE*        fp;
    char         errmsg[500],command[100];
    int          data_conn;
    struct timeval timeout;

    /* --- ������������ --- */
    data_conn = Ftp_EstablishDataConn(ctl_conn);
    if(data_conn < 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"�������ӽ�������\n");
        Tcp_Close(ctl_conn);
        return FTP_DATACONN_ERROR;
    }
    /* --- �����������ӳ�ʱ --- */
    timeout.tv_sec = 15;
    timeout.tv_usec = 0;
    setsockopt(data_conn,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(timeout));
    setsockopt(data_conn,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(timeout));

    memset(command,0x00,sizeof(command));
    sprintf(command,"STOR %s\r\n",remotename);
    ret = Tcp_Send(ctl_conn,command,strlen(command));
    if (ret < 0)
    {
        Tcp_Close(data_conn);
        return ret;
    }
    memset(buffer,0x00,sizeof(buffer));
    ret = Ftp_RecvFtpSvrMsg(ctl_conn,"150 ","STOR",buffer);
    if (ret == 0)
    {
        Tcp_Close(data_conn);
        return FTP_CTLCONNCLOSED_ERROR;
    }
    else if (ret == -1)
    {
        Tcp_Close(data_conn);
        return FTP_RETR_ERROR;
    }
    else if (ret < 0)
    {
        Tcp_Close(data_conn);
        return ret;
    }

    /* --- �жϴ����Ƿ���� --- */
    ret = Ftp_IsTransferComplete(buffer);

    /* --- ֻ����ʽ���ļ� --- */
    memset(path,0x00,sizeof(path));
    sprintf(path,"%s%s",local_path,localname);
    fp = fopen(path,"rb");
    if (fp == NULL)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        sprintf(ms_str_SocketErrorMessage,"fopen error: ���ļ�%s����!\n",path);
        Tcp_Close(data_conn);
        return FTP_OPENFILE_ERROR;
    }

    /* --- �ϴ��ļ� --- */
    while(!feof(fp))
    {
        memset(buffer,0x00,sizeof(buffer));
        count = fread(buffer,sizeof(char),BLOCKSIZE,fp);
        if(ferror(fp))
        {
            memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
            sprintf(ms_str_SocketErrorMessage,"fread error: ���ļ�%s����!\n",path);
            fclose(fp);
            Tcp_Close(data_conn);
            return FTP_READFILE_ERROR;;
        }
        if(Tcp_Send(data_conn,buffer,count)<0)
        {
            fclose(fp);
            Tcp_Close(data_conn);
            return FTP_DATACONN_ERROR;
        }
    }

    /* --- �ر��������� --- */
    Tcp_Close(data_conn);

    /* --- ���շ�����������Ӧ����Ϣ --- */
    if (!ret)
    {
        memset(buffer,0x00,sizeof(buffer));
        ret = Ftp_RecvFtpSvrMsg(ctl_conn,"226 ","�ļ��ϴ�����",buffer);
    }
    /* --- �ر��ļ���� --- */
    fclose(fp);

    return ret;
}

/* ------------------------------------------------------
            ��������:   �����ļ�
			����ֵ˵��: �ɹ�����1,ʧ�ܷ��ش������
--------------------------------------------------------- */
int CSocket::Ftp_GetFile(int ctl_conn,int data_conn,char *local_path,char *name)
{
    char         *ptr,path[MAX_PATH],buffer[BLOCKSIZE];
    int          rcv_len,ret,count,number_bytes,nleft;
    FILE*        fp;
    char         errmsg[500],command[100];

    /* --- ֻд��ʽ���ļ� --- */
    memset(path,0x00,sizeof(path));
    sprintf(path,"%s%s",local_path,name);
    fp = fopen(path,"wb");
    if (fp == NULL)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        sprintf(ms_str_SocketErrorMessage,"fopen error: ���ļ�%s����!\n",path);
        Tcp_Close(data_conn);
        return FTP_OPENFILE_ERROR;
    }

    /* --- ������������������� --- */
    memset(command,0x00,sizeof(command));
    sprintf(command,"RETR %s\r\n",name);
    ret = Tcp_Send(ctl_conn,command,strlen(command));
    if (ret < 0)
    {
        fclose(fp);
        Tcp_Close(data_conn);
        return ret;
    }
    memset(buffer,0x00,sizeof(buffer));
    ret = Ftp_RecvFtpSvrMsg(ctl_conn,"150 ","RETR",buffer);
    if (ret == 0)
    {
        fclose(fp);
        Tcp_Close(data_conn);
        return FTP_CTLCONNCLOSED_ERROR;
    }
    else if (ret == -1)
    {
        fclose(fp);
        Tcp_Close(data_conn);
        return FTP_RETR_ERROR;
    }
    else if (ret < 0)
    {
        fclose(fp);
        Tcp_Close(data_conn);
        return ret;
    }
    /* --- �жϴ����Ƿ���� --- */
    ret = Ftp_IsTransferComplete(buffer);

    /* --- �����ļ� --- */
    number_bytes = 0;
    do
    {
        memset(buffer,0x00,sizeof(buffer));
        /* --- �����ļ����� --- */
        rcv_len = Tcp_Recv(data_conn,buffer,BLOCKSIZE);
        if (rcv_len < 0)       /* --- recv��������ʧ�� --- */
        {
            Tcp_Close(data_conn);
            fclose(fp);
            return rcv_len;
        }

        /* --- д�뱾���ļ� --- */
        nleft = rcv_len;
        ptr = buffer;
        while(nleft > 0)
        {
            count = fwrite(ptr,sizeof(char),nleft,fp);
            /* --- д�뱾���ļ����� --- */
            if(ferror(fp))
            {
                memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
                strcpy(ms_str_SocketErrorMessage,"fwrite error\n");
                Tcp_Close(data_conn);
                fclose(fp);
                return FTP_WRITEFILE_ERROR;
            }
            nleft  -= count;
            ptr += count;
        }
        number_bytes += rcv_len;
    }while (rcv_len);

    /* --- �ر��������� --- */
    Tcp_Close(data_conn);

    /* --- ���շ�����������Ӧ����Ϣ --- */
    if (!ret)
    {
        memset(buffer,0x00,sizeof(buffer));
        ret = Ftp_RecvFtpSvrMsg(ctl_conn,"226 ","�ļ����ؽ���",buffer);
    }

    /* --- �ر��ļ���� --- */
    fclose(fp);

    return ret;
}

/* ------------------------------------------------------
��������:   ���Ŀ¼�������ļ�
����ֵ˵��: �ɹ�����1,ʧ�ܷ��ش������
--------------------------------------------------------- */
int CSocket::Ftp_GetRemoteDirList(int ctl_conn,char *LocalPath,char *LocalName)
{
    char	command[100],buffer[BLOCKSIZE];
    int		ret,data_conn,rcv_len;
    char	errmsg[100];
    int		count,nleft;
    char	*ptr,path[MAX_PATH];
    FILE*   fp;
    struct timeval timeout;

    /* --- ����FTP���� --- */
    memset(command,0x00,sizeof(command));
    strcpy(command,"TYPE A\r\n");
    ret = Tcp_Send(ctl_conn,command,strlen(command));
    if (!ret)
    {
        return 0;
    }
    memset(buffer,0x00,sizeof(buffer));
    ret = Ftp_RecvFtpSvrMsg(ctl_conn,"200 ","TYPE",buffer);
    if (ret != 1)
    {
        return 0;
    }
    /* --- ������������ --- */
    data_conn = Ftp_EstablishDataConn(ctl_conn);
    if(!data_conn)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"�������ӽ�������\n");

        Tcp_Close(ctl_conn);
        return 0;
    }
    /* --- �����������ӳ�ʱ --- */
    timeout.tv_sec = 15;
    timeout.tv_usec = 0;
    setsockopt(data_conn,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(timeout));
    setsockopt(data_conn,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(timeout));

    /* --- ֻд��ʽ���ļ� --- */
    memset(path,0x00,sizeof(path));
    sprintf(path,"%s%s",LocalPath,LocalName);
    fp = fopen(path,"wb");
    if (fp == NULL)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        sprintf(ms_str_SocketErrorMessage,"fopen error: ���ļ�%s����!\n",path);

        /* --- �ر��������� --- */
        Tcp_Close(data_conn);

        return FTP_OPENFILE_ERROR;
    }
    /* --- ����FTP���� --- */
    memset(command,0x00,sizeof(command));
    sprintf(command,"LIST %s\r\n",".");
    ret = Tcp_Send(ctl_conn,command,strlen(command));
    if (!ret)
    {
        /* --- �ر��������� --- */
        Tcp_Close(data_conn);
        /* --- �ر��ļ���� --- */
        fclose(fp);

        return 0;
    }
    memset(buffer,0x00,sizeof(buffer));
    ret = Ftp_RecvFtpSvrMsg(ctl_conn,"150 ","LIST",buffer);
    if (ret != 1)
    {
        /* --- �ر��������� --- */
        Tcp_Close(data_conn);
        /* --- �ر��ļ���� --- */
        fclose(fp);

        return 0;
    }
    ret = Ftp_IsTransferComplete(buffer);
    /* --- Ŀ¼�б���Ϣд���ļ� --- */
    do
    {
        memset(buffer,0x00,sizeof(buffer));
        rcv_len = Tcp_Recv(data_conn,buffer,BLOCKSIZE);
        if (rcv_len < 0)
        {
            memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
            strcpy(ms_str_SocketErrorMessage,"List Ŀ¼����!\n");

            /* --- �ر��������� --- */
            Tcp_Close(data_conn);
            /* --- �ر��ļ���� --- */
            fclose(fp);

            return 0;
        }

        /* --- д�뱾���ļ� --- */
        nleft = rcv_len;
        ptr = buffer;
        while(nleft > 0)
        {
            count = fwrite(ptr,sizeof(char),nleft,fp);
            /* --- д�뱾���ļ����� --- */
            if(ferror(fp))
            {
                memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
                strcpy(ms_str_SocketErrorMessage,"fwrite error\n");

                Tcp_Close(data_conn);
                fclose(fp);
                return FTP_WRITEFILE_ERROR;
            }
            nleft  -= count;
            ptr += count;
        }
    }while (rcv_len);

    /* --- �ر��������� --- */
    Tcp_Close(data_conn);
    if (!ret)
    {
        memset(buffer,0x00,sizeof(buffer));
        ret = Ftp_RecvFtpSvrMsg(ctl_conn,"226 ","list end",buffer);
    }
    /* --- �ر��ļ���� --- */
    fclose(fp);

    return ret;
}


int CSocket::Ftp_RenameRemoteFile(int ctl_conn,char *sourcePath,char *destPath)
{
    char         command[100],buffer[BLOCKSIZE];
    int          ret;
    /* --- ������������USER���� --- */
    memset(command,0x00,sizeof(command));
    sprintf(command,"RNFR %s\r\n",sourcePath);
    ret = Tcp_Send(ctl_conn,command,strlen(command));
    if (ret < 0)
    {
        return ret;
    }
    /* --- ����Ӧ�� --- */
    memset(buffer,0x00,sizeof(buffer));
    ret = Ftp_RecvFtpSvrMsg(ctl_conn,"350 ","RNFR",buffer);
    if (ret == 0)
    {
        return FTP_CTLCONNCLOSED_ERROR;
    }
    else if (ret == -1)
    {
        return FTP_RNFR_ERROR;
    }
    else if (ret < 0)
    {
        return ret;
    }

    /* --- ������������USER���� --- */
    memset(command,0x00,sizeof(command));
    sprintf(command,"RNTO %s\r\n",destPath);
    ret = Tcp_Send(ctl_conn,command,strlen(command));
    if (ret < 0)
    {
        return ret;
    }
    /* --- ����Ӧ�� --- */
    memset(buffer,0x00,sizeof(buffer));
    ret = Ftp_RecvFtpSvrMsg(ctl_conn,"250 ","RNTO",buffer);
    if (ret == 0)
    {
        return FTP_CTLCONNCLOSED_ERROR;
    }
    else if (ret == -1)
    {
        return FTP_RNTO_ERROR;
    }
    else if (ret < 0)
    {
        return ret;
    }

    return SUCCESS;
}


/* ------------------------------------------------------
��������:   ɾ��Զ���ļ�
����ֵ˵��: �ɹ�����1,ʧ�ܷ��ش������
--------------------------------------------------------- */
int CSocket::Ftp_DeleteFile(int ctl_conn,char *filename)
{
    int ret;
    char command[100],buffer[BLOCKSIZE];

    /* --- ������������USER���� --- */
    memset(command,0x00,sizeof(command));
    sprintf(command,"DELE %s\r\n",filename);
    ret = Tcp_Send(ctl_conn,command,strlen(command));
    if (ret < 0)
    {
        return ret;
    }
    /* --- ����Ӧ�� --- */
    memset(buffer,0x00,sizeof(buffer));
    ret = Ftp_RecvFtpSvrMsg(ctl_conn,"250 ","DELE",buffer);
    if (ret == 0)
    {
        return FTP_CTLCONNCLOSED_ERROR;
    }
    else if (ret == -1)
    {
        return FTP_USER_ERROR;
    }
    else if (ret < 0)
    {
        return ret;
    }
    else
    {
        return SUCCESS;
    }
}

/* ------------------------------------------------------
��������:   Ѱ��ָ���ַ���������
����ֵ˵��: �ɹ������ַ�����λ��
--------------------------------------------------------- */
int  CSocket::Ftp_FindChar(char *p,const char c)
{
	int i;

	for(i = 0;i < strlen(p); i++)
	{
		if(p[i] == c)
		{
			break;
		}
	}

	return i;
}
/* ------------------------------------------------------
��������:   �����������Ӻ��������Ӳ�������Ŀ¼�б�
����ֵ˵��: �ɹ�����1,ʧ�ܷ��ش������
--------------------------------------------------------- */
int CSocket::Ftp_GetDirList(char *control_conn_ip,int port,char *user,char *pass,char *local_path,const char *local_name,char *remote_path)
{
    int   ctl_conn,ret;
    char  LocalPath[64],LocalName[64],RemotePath[64];
    char  buffer[BLOCKSIZE];
    struct timeval timeout;

    if (strlen(control_conn_ip) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"û���ṩ��������IP��ַ!\n");

        return FTP_NOREMOTEIP_ERROR;
    }
    if (strlen(user) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"û���ṩ�û���!\n");

        return FTP_NOUSER_ERROR;
    }
    if (strlen(pass) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"û���ṩ����!\n");

        return FTP_NOPASSWD_ERROR;
    }
    if (strlen(local_path) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"û���ṩ����Ŀ¼��!\n");

        return FTP_NOLOCALPATH_ERROR;
    }
    if (strlen(local_name) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"û���ṩ�����ļ���!\n");

        return FTP_NOLOCALPATH_ERROR;
    }
    if (strlen(remote_path) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"û���ṩԶ��Ŀ¼��(ready)!\n");

        return FTP_NOREMOTEPATH_ERROR;
    }

    memset(LocalPath,0x00,sizeof(LocalPath));
    memset(LocalName,0x00,sizeof(LocalName));
    memset(RemotePath,0x00,sizeof(RemotePath));
    strcpy(LocalPath,local_path);
    strcpy(LocalName,local_name);
    strcpy(RemotePath,remote_path);
    Ftp_ModifyPath(LocalPath,RemotePath);

    /* --- ������������ --- */
    ctl_conn = Tcp_Connect(control_conn_ip,port,5000);
    if (ctl_conn <= 0)
    {
        return ctl_conn;
    }
    /* --- ���ó�ʱ --- */
    timeout.tv_sec = 15;
    timeout.tv_usec = 0;
    setsockopt(ctl_conn,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(timeout));
    setsockopt(ctl_conn,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(timeout));

    memset(buffer,0x00,sizeof(buffer));
    ret = Ftp_RecvFtpSvrMsg(ctl_conn,"220 ","connect",buffer);
    if (ret != SUCCESS)
    {
        Tcp_Close(ctl_conn);
        return FTP_CTLCONN_ERROR;
    }

    /* --- ������������û�������������� --- */
    char fullpath[MAX_PATH];
	memset(fullpath,0x00,sizeof(fullpath));
    ret = Ftp_InitCtrlConn(ctl_conn,user,pass,RemotePath,fullpath);
    if (ret != SUCCESS)
    {
        Tcp_Close(ctl_conn);
        return ret;
    }

    /* --- Ŀ¼�б� --- */
    ret = Ftp_GetRemoteDirList(ctl_conn,LocalPath,LocalName);
    /* --- �رտ������� --- */
    Tcp_Close(ctl_conn);

    return ret;
}

/* ------------------------------------------------------
      ��������:   �����������Ӻ��������Ӳ����ϴ��ļ�
	  ����ֵ˵��: �ɹ�����1,ʧ�ܷ��ش������
--------------------------------------------------------- */
int CSocket::Ftp_PutFile(char *control_conn_ip,int port,char *user,char *pass,char *local_path,char *localname,char *remote_temppath,char *remote_readypath,char *remotename)
{
    int   ctl_conn,ret;
    char  LocalPath[MAX_PATH],RemotePath[MAX_PATH];
    char  buffer[BLOCKSIZE];
    char  RemoteTempFile[MAX_PATH],RemoteReadyFile[MAX_PATH];
    struct timeval timeout;
    char  tmp_name[64];

    if (strlen(control_conn_ip) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"û���ṩ��������IP��ַ!\n");

        return FTP_NOREMOTEIP_ERROR;
    }
    if (strlen(user) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"û���ṩ�û���!\n");

        return FTP_NOUSER_ERROR;
    }
    if (strlen(pass) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"û���ṩ����!\n");

        return FTP_NOPASSWD_ERROR;
    }
    if (strlen(local_path) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"û���ṩ����Ŀ¼��!\n");

        return FTP_NOLOCALPATH_ERROR;
    }
    if (strlen(remote_temppath) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"û���ṩԶ��Ŀ¼��(temp)!\n");

        return FTP_NOREMOTEPATH_ERROR;
    }
    if (strlen(localname) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"û���ṩ�����ļ���!\n");

        return FTP_NOFILENAME_ERROR;
    }
    if (strlen(remotename) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"û���ṩԶ���ļ���!\n");

        return FTP_NOFILENAME_ERROR;
    }
    if(remote_readypath == NULL)
    {
        memset(tmp_name,0x00,sizeof(tmp_name));
        sprintf(tmp_name,"%s.lck",remotename);
    }
    else
    {
        memset(tmp_name,0x00,sizeof(tmp_name));
        sprintf(tmp_name,"%s",remotename);
    }

    /* --- ��·��������"/"����"\\" --- */
    memset(LocalPath,0x00,sizeof(LocalPath));
    memset(RemotePath,0x00,sizeof(RemotePath));
    strcpy(LocalPath,local_path);
    strcpy(RemotePath,remote_temppath);
    Ftp_ModifyPath(LocalPath,RemotePath);

    /* --- ������������ --- */
    ctl_conn = Tcp_Connect(control_conn_ip,port,5000);
    if (ctl_conn <= 0)
    {
        return ctl_conn;
    }
    /* --- ���ó�ʱ --- */
    timeout.tv_sec = 15;
    timeout.tv_usec = 0;
    setsockopt(ctl_conn,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(timeout));
    setsockopt(ctl_conn,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(timeout));

    memset(buffer,0x00,sizeof(buffer));
    ret = Ftp_RecvFtpSvrMsg(ctl_conn,"220 ","connect",buffer);
    if (ret != SUCCESS)
    {
        Tcp_Close(ctl_conn);
        return FTP_CTLCONN_ERROR;
    }

    /* --- ������������û�������������� --- */
    char fullpath[MAX_PATH];
		memset(fullpath,0x00,sizeof(fullpath));
    ret = Ftp_InitCtrlConn(ctl_conn,user,pass,RemotePath,fullpath);
    if (ret != SUCCESS)
    {
        Tcp_Close(ctl_conn);
        return ret;
    }
		if(strlen(fullpath) > 0)
				strcpy(remote_temppath,fullpath);
				
    /* --- �ϴ��ļ� --- */
    ret = Ftp_PutFile(ctl_conn,LocalPath,localname,tmp_name);
    if(ret == SUCCESS)
    {
        if( remote_readypath != NULL)
        {
            memset(RemoteTempFile,0x00,sizeof(RemoteTempFile));
            sprintf(RemoteTempFile,"%s%s",remote_temppath,tmp_name);

            memset(RemoteReadyFile,0x00,sizeof(RemoteReadyFile));
            sprintf(RemoteReadyFile,"%s%s",remote_readypath,tmp_name);

            ret = Ftp_RenameRemoteFile(ctl_conn,RemoteTempFile,RemoteReadyFile);
        }
        else
        {
            memset(RemoteTempFile,0x00,sizeof(RemoteTempFile));
            sprintf(RemoteTempFile,"%s%s",remote_temppath,tmp_name);

            memset(RemoteReadyFile,0x00,sizeof(RemoteReadyFile));
            sprintf(RemoteReadyFile,"%s%s",remote_temppath,remotename);

            ret = Ftp_RenameRemoteFile(ctl_conn,RemoteTempFile,RemoteReadyFile);
        }
    }
    /* --- �رտ������� --- */
    Tcp_Close(ctl_conn);

    return ret;
}



/* ------------------------------------------------------
      ��������:   �����������Ӻ��������Ӳ��������ļ�
	  ����ֵ˵��: �ɹ�����1,ʧ�ܷ��ش������
--------------------------------------------------------- */
int CSocket::Ftp_GetFile(char *control_conn_ip,int port,char *user,char *pass,char *local_path,char *name,char *remote_path)
{
    int   ctl_conn,data_conn,ret;
    char  LocalPath[MAX_PATH],RemotePath[MAX_PATH];
    char  buffer[BLOCKSIZE];
    struct timeval timeout;

    if (strlen(control_conn_ip) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"û���ṩ��������IP��ַ!\n");

        return FTP_NOREMOTEIP_ERROR;
    }
    if (strlen(user) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"û���ṩ�û���!\n");

        return FTP_NOUSER_ERROR;
    }
    if (strlen(pass) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"û���ṩ����!\n");

        return FTP_NOPASSWD_ERROR;
    }
    if (strlen(local_path) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"û���ṩ����Ŀ¼��!\n");

        return FTP_NOLOCALPATH_ERROR;
    }
    if (strlen(remote_path) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"û���ṩԶ��Ŀ¼��!\n");

        return FTP_NOREMOTEPATH_ERROR;
    }
    if (strlen(name) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"û���ṩ�ļ���!\n");

        return FTP_NOFILENAME_ERROR;
    }

    /* --- ��·��������"/"����"\\" --- */
    memset(LocalPath,0x00,sizeof(LocalPath));
    memset(RemotePath,0x00,sizeof(RemotePath));
    strcpy(LocalPath,local_path);
    strcpy(RemotePath,remote_path);
    Ftp_ModifyPath(LocalPath,RemotePath);
    Ftp_ModifyPath(local_path,remote_path);

    /* --- ������������ --- */
    ctl_conn = Tcp_Connect(control_conn_ip,port,5000);
    if (ctl_conn <= 0)
    {
        return ctl_conn;
    }
    /* --- ���ó�ʱ --- */
    timeout.tv_sec = 15;
    timeout.tv_usec = 0;
    setsockopt(ctl_conn,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(timeout));
    setsockopt(ctl_conn,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(timeout));

    memset(buffer,0x00,sizeof(buffer));
    ret = Ftp_RecvFtpSvrMsg(ctl_conn,"220 ","connect",buffer);
    if (ret != SUCCESS)
    {
        Tcp_Close(ctl_conn);
        return FTP_CTLCONN_ERROR;
    }

    /* --- ������������û�������������� --- */
    char  fullpath[MAX_PATH];
		memset(fullpath,0x00,sizeof(fullpath));
		ret = Ftp_InitCtrlConn(ctl_conn,user,pass,RemotePath,fullpath);
    if (ret != SUCCESS)
    {
        Tcp_Close(ctl_conn);
        return ret;
    }

    /* --- ������������ --- */
    data_conn = Ftp_EstablishDataConn(ctl_conn);
    if(data_conn < 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"�������ӽ�������!\n");

        Tcp_Close(ctl_conn);
        return FTP_DATACONN_ERROR;
    }
    /* --- �����������ӳ�ʱ --- */
    setsockopt(data_conn,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(timeout));
    setsockopt(data_conn,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(timeout));

    /* --- �����ļ� --- */
    ret = Ftp_GetFile(ctl_conn,data_conn,LocalPath,name);

    /* --- �رտ������� --- */
    Tcp_Close(ctl_conn);

    return ret;

}
