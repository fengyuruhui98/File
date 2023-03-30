
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

 
 		//在错误ms_str_SocketErrorMessage前面加上该错误产生的时间,便于追踪
    time_t    long_time;
    struct tm newtime;
    struct timeval t_msec;
    char bufferError[2048];

    /* --- 毫秒 --- */
    gettimeofday(&t_msec,NULL);
    time(&long_time);
    memset(&newtime,0x00,sizeof(struct tm));
    localtime_r(&long_time,&newtime);
    sprintf(bufferError,"[%02d:%02d:%02d:%03d %04d年%02d月%02d日]\t",newtime.tm_hour,newtime.tm_min,newtime.tm_sec,t_msec.tv_usec/1000,
            (newtime.tm_year+1900),(newtime.tm_mon+1),newtime.tm_mday);

		strcat(bufferError,"\t");
    strcat(bufferError,ms_str_SocketErrorMessage);
 		memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
 		strcpy(ms_str_SocketErrorMessage,bufferError);
 
    return tcpErrorCode;
}

/* ------------------------------------------------------
            功能描述:   生成socket句柄
			返回值说明: 成功返回句柄,失败返回错误代码
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
            功能描述:   设置发送缓冲区、接收缓冲区的大小
			返回值说明: 设置成功返回1,失败返回错误代码
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
   功能描述:  启动服务器进行侦听
   参数说明:  port: 侦听的端口号
   返回值说明:成功返回侦听的socket句柄,失败返回错误代码
--------------------------------------------------------- */
int CSocket::Tcp_Listen(int port)
{
    int                  listenfd;
    int                  ret;
    struct sockaddr_in   servaddr;
    int                  ReusePort;

    /* --- 生成socket句柄--- */
    listenfd = Tcp_Socket(SOCK_STREAM);
    if (listenfd < 0)
    {
        return listenfd;
    }

    memset(&servaddr,0x00,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons((unsigned short)port);

    /* --- 绑定端口 --- */
    ReusePort = 1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&ReusePort,sizeof(ReusePort));
    ret = bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    if (ret != 0)
    {
        ret = Tcp_GetSocketErrorMsg("bind",errno);
        Tcp_Close(listenfd);
        return ret;
    }

    /* --- 设置发送缓冲区和接收缓冲区 --- */
    ret = Tcp_SetSocketBuf(listenfd);
    if (ret < 0)
    {
        Tcp_Close(listenfd);
        return ret;
    }

    /* --- 侦听 --- */
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
            功能描述:   关闭socket句柄
			返回值说明: 成功返回1,失败返回错误代码
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
        /* --- 关闭socket句柄失败返回错误代码 --- */
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
            功能描述:   接收来自客户端的连接请求
            返回值说明: 成功返回连接的socket句柄,失败返回0
--------------------------------------------------------- */
int CSocket::Tcp_Accept(int listenfd,char *clnip,int &port)
{
    int   							ret,connfd;
    struct sockaddr_in  cln_adr;
    socklen_t						cln_len;

    /* --- 等待来自客户端的连接请求 --- */
    while(1)
    {
        memset(&cln_adr,0x00,sizeof(cln_adr));
        cln_len = sizeof(cln_adr);
        connfd = accept(listenfd,(struct sockaddr*)&cln_adr,&cln_len);
        /* --- 接收连接失败 --- */
        if (connfd < 0)
        {
            ret = Tcp_GetSocketErrorMsg("accept",errno);
            return ret;
        }
        else
        {
            /* --- 客户端连接的信息 --- */
						strcpy(clnip,inet_ntoa(cln_adr.sin_addr));
						port = htons(cln_adr.sin_port);

            return connfd;
        }
    }
}


/* ------------------------------------------------------
   功能描述:   向服务器端发送连接请求
   参数说明:   server_ip:  服务器端的IP地址
               server_port:服务器端侦听的端口号
   返回值说明: 成功返回连接的socket句柄,失败返回错误代码
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

    /* --- 创建socket句柄 --- */
    sockfd = Tcp_Socket(SOCK_STREAM);
    if (sockfd < 0)
    {
        return sockfd;
    }

    /* --- 设置服务器的IP和端口 --- */
    memset(&server_adr,0x00,sizeof(server_adr));
    server_adr.sin_family = AF_INET;
    server_adr.sin_addr.s_addr = inet_addr(server_ip);
    server_adr.sin_port = htons((unsigned short)server_port);

    /* --- 获得socket阻塞标志 --- */
    flags = fcntl(sockfd,F_GETFL,0);

    ret = Tcp_SetSocketBuf(sockfd);            /* --- 设置发送缓冲区和接收缓冲区 --- */
    if (ret < 0)  /* --- 设置失败 --- */
    {
        Tcp_Close(sockfd);
        return ret;
    }

    error = 0;
    fcntl(sockfd,F_SETFL,flags | O_NONBLOCK);  /* --- 设置为非阻塞socket --- */

    ret = connect(sockfd,(struct sockaddr *)&server_adr,sizeof(server_adr));/* --- 与服务器端进行连接 --- */

    if (ret < 0)  /* --- 连接失败 --- */
    {
        memset(info,0x00,sizeof(info));
        sprintf(info,"connect %s",server_ip);
        ret = Tcp_GetSocketErrorMsg(info,errno);
        if (ret != TCP_ERROR_EINPROGRESS)  /* - -- 因设置成非阻塞导致连接不能立即完成 --- */
        {
            Tcp_Close(sockfd);
            return ret;
        }
    }
    if (ret == 0)         /* --- 连接立即完成 --- */
    {
        goto done;
    }

    /* --- 给连接设置超时 --- */
    FD_ZERO(&read_fd);
    FD_SET(sockfd,&read_fd);
    write_fd = read_fd;
    timeoutSTRU.tv_sec  = timeout / 1000;
    timeoutSTRU.tv_usec = timeout % 1000;
    ret = select(sockfd+1,&read_fd,&write_fd,NULL,&timeoutSTRU);

    if (ret == 0)        /* --- 连接超时 --- */
    {
        errno = ETIMEDOUT;
        memset(info,0x00,sizeof(info));
        sprintf(info,"connect %s",server_ip);
        ret = Tcp_GetSocketErrorMsg(info,errno);
        Tcp_Close(sockfd);	 /* --- 关闭套接字 --- */
        return ret;
    }
    if (ret < 0)          /* --- select 出错 --- */
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
    fcntl(sockfd,F_SETFL,flags);    /* --- 重新设置为阻塞socket --- */
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
   功能描述:  向客户端发送应答
   参数说明:  sockfd:   socket句柄
              buffer:   发送缓冲区的指针
		      send_len: 发送的长度
			  nleft:    未发送的字节数
			  nsend:    调用一次send发送的字节数
   返回值说明:发送成功返回1,失败返回错误代码;
   错误信息存在于该类的静态变量(ms_str_SocketErrorMessage)中
--------------------------------------------------------- */
int CSocket::Tcp_Send(int sockfd,char *buffer,int snd_len)
{
    int             nleft,nsend;
    int             ret;
    char            *ptr;

    if (snd_len > MAX_BAOWEN_LENGTH)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        sprintf(ms_str_SocketErrorMessage,"报文大小大于%dK,不能发送!\n",MAX_BAOWEN_LENGTH);
        return -6; //-6 PACKAGE_TOO_BIG_ERROR
    }

    nleft = snd_len;
    ptr = buffer;
    struct timeval timeout;
    /* --- 设置超时 --- */
    timeout.tv_sec = 15;
    timeout.tv_usec = 0;
    setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(timeout));

    /* --- 循环调用send函数,直到发送send_len个字节完后或者发送失败 --- */
    while(nleft > 0)
    {
        nsend = send(sockfd,ptr,nleft,MSG_NOSIGNAL);
        if (nsend < 0) /* --- 发送失败返回错误代码 --- */
        {
            ret = Tcp_GetSocketErrorMsg("send",errno);
            /* --- 禁止超时 --- */
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
    /* --- 禁止超时 --- */
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(timeout));

    return 1;            /* --- 发送成功返回1 --- */
}

/* ------------------------------------------------------
   功能描述:  socket 接收函数
   参数说明:  sockfd: socket句柄
              buffer: 接收缓冲区
			  rcv_len:接收的长度
   返回值说明:成功返回接收的长度,客户端断开连接返回0,
              失败返回错误代码
--------------------------------------------------------- */
int CSocket::Tcp_Recv(int sockfd,char *buffer,int rcv_len)
{
    int    nrcv;

    nrcv =  recv(sockfd,buffer,rcv_len,0);
    if (nrcv < 0)       /* --- recv函数调用失败 --- */
    {
        nrcv = Tcp_GetSocketErrorMsg("recv",errno);
    }

    return nrcv;
}



/* ------------------------------------------------------
功能描述:如果目录后面没有目录标识符,则加上
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
            功能描述:   控制连接建立成功后,向服务器端发送
			            用户名和密码等信息
			返回值说明: 成功返回1,失败返回错误代码
--------------------------------------------------------- */
int CSocket::Ftp_InitCtrlConn(int ctl_conn,char *user,char *pass,char *remote_path,char *fullpath)
{
    int ret;
    char command[100],buffer[BLOCKSIZE];

    /* --- 给服务器发送USER命令 --- */
    memset(command,0x00,sizeof(command));
    sprintf(command,"USER %s\r\n",user);
    ret = Tcp_Send(ctl_conn,command,strlen(command));
    if (ret < 0)
    {
        return ret;
    }
    /* --- 接收应答 --- */
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

    /* --- 给服务器发送PASS命令 --- */
    memset(command,0x00,sizeof(command));
    sprintf(command,"PASS %s\r\n",pass);
    ret = Tcp_Send(ctl_conn,command,strlen(command));
    if (ret < 0)
    {
        return ret;
    }
    /* --- 接收应答 --- */
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

    /* --- 给服务器发送CWD命令 --- */
    memset(command,0x00,sizeof(command));
    sprintf(command,"CWD %s\r\n",remote_path);
    ret = Tcp_Send(ctl_conn,command,strlen(command));
    if (ret < 0)
    {
        return ret;
    }
    /* --- 接收应答 --- */
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

    /* --- 给服务器发送TYPE命令 --- */
    memset(command,0x00,sizeof(command));
    strcpy(command,"TYPE I\r\n");
    ret = Tcp_Send(ctl_conn,command,strlen(command));
    if (ret < 0)
    {
        return ret;
    }
    /* --- 接收应答 --- */
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

		/* --- 给服务器发送PWD命令 --- */
		memset(command,0x00,sizeof(command));
		strcpy(command,"PWD\r\n");
		ret = Tcp_Send(ctl_conn,command,strlen(command));
		if (ret < 0)
		{
			return ret;
		}		
		/* --- 接收应答 --- */
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
            功能描述:   接收来自服务器端的应答
			返回值说明: 应答正确返回1,应答错误返回-1,
			            失败返回错误代码,连接断开返回0
--------------------------------------------------------- */
int CSocket::Ftp_RecvFtpSvrMsg(int sockfd,const char *code,const char *type,char *buffer)
{
    int    nrcv;
    char   errmsg[500];

    nrcv = Tcp_Recv(sockfd,buffer,BLOCKSIZE);
    if (nrcv < 0)       /* --- recv函数调用失败 --- */
    {
        ;
    }

    if ( (nrcv > 0) && buffer[0] != *code)   /* --- 应答错误 --- */
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
        if (nrcv == 0)                        /* --- 连接关闭 --- */
        {
            memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
            sprintf(ms_str_SocketErrorMessage,"%s: 服务器端已经关闭!\n",type);

            return 0;
        }

        if (strstr(buffer,code) != NULL)
        {
            return SUCCESS;
        }
        if ((strcmp(type,"RETR") == 0))
        {
            if (strstr(buffer,"125 ") != NULL)   /* --- WINDOWS 平台 --- */
            {
                return SUCCESS;
            }
        }
        if ((strcmp(type,"STOR") == 0))
        {
            if (strstr(buffer,"125 ") != NULL)   /* --- WINDOWS 平台 --- */
            {
                return SUCCESS;
            }
        }
        if ((strcmp(type,"LIST") == 0))
        {
            if (strstr(buffer,"125 ") != NULL)   /* --- WINDOWS 平台 --- */
            {
                return SUCCESS;
            }
        }

        /* --- 继续读取服务器返回的其他消息 --- */
        memset(buffer,0x00,strlen(buffer));
        nrcv = Tcp_Recv(sockfd,buffer,BLOCKSIZE);
        if (nrcv < 0)       /* --- recv函数调用失败 --- */
        {
            ;
        }
    }
}

/* ------------------------------------------------------
  功能描述:   建立数据连接
  返回值说明: 成功返回数据连接的socket句柄,失败返回错误代码
--------------------------------------------------------- */
int CSocket::Ftp_EstablishDataConn(int ctl_conn)
{
    char               *ip,command[100],buffer[BLOCKSIZE];
    char               data_conn_ip[16];
    int                data_conn;
    unsigned int       data_conn_info[10];
    int		           data_conn_port;
    int                ret;

    /* --- PASV模式,获得服务器端提供的数据连接的IP和端口 --- */
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
            功能描述:判断传输是否完成
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
            功能描述:   上传文件
			返回值说明: 成功返回1,失败返回错误代码
--------------------------------------------------------- */
int CSocket::Ftp_PutFile(int ctl_conn,char *local_path,char *localname,char *remotename)
{
    char         path[MAX_PATH],buffer[BLOCKSIZE];
    int          ret,count;
    FILE*        fp;
    char         errmsg[500],command[100];
    int          data_conn;
    struct timeval timeout;

    /* --- 建立数据连接 --- */
    data_conn = Ftp_EstablishDataConn(ctl_conn);
    if(data_conn < 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"数据连接建立出错\n");
        Tcp_Close(ctl_conn);
        return FTP_DATACONN_ERROR;
    }
    /* --- 设置数据连接超时 --- */
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

    /* --- 判断传输是否完成 --- */
    ret = Ftp_IsTransferComplete(buffer);

    /* --- 只读方式打开文件 --- */
    memset(path,0x00,sizeof(path));
    sprintf(path,"%s%s",local_path,localname);
    fp = fopen(path,"rb");
    if (fp == NULL)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        sprintf(ms_str_SocketErrorMessage,"fopen error: 打开文件%s错误!\n",path);
        Tcp_Close(data_conn);
        return FTP_OPENFILE_ERROR;
    }

    /* --- 上传文件 --- */
    while(!feof(fp))
    {
        memset(buffer,0x00,sizeof(buffer));
        count = fread(buffer,sizeof(char),BLOCKSIZE,fp);
        if(ferror(fp))
        {
            memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
            sprintf(ms_str_SocketErrorMessage,"fread error: 打开文件%s错误!\n",path);
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

    /* --- 关闭数据连接 --- */
    Tcp_Close(data_conn);

    /* --- 接收服务器端最后的应答消息 --- */
    if (!ret)
    {
        memset(buffer,0x00,sizeof(buffer));
        ret = Ftp_RecvFtpSvrMsg(ctl_conn,"226 ","文件上传结束",buffer);
    }
    /* --- 关闭文件句柄 --- */
    fclose(fp);

    return ret;
}

/* ------------------------------------------------------
            功能描述:   下载文件
			返回值说明: 成功返回1,失败返回错误代码
--------------------------------------------------------- */
int CSocket::Ftp_GetFile(int ctl_conn,int data_conn,char *local_path,char *name)
{
    char         *ptr,path[MAX_PATH],buffer[BLOCKSIZE];
    int          rcv_len,ret,count,number_bytes,nleft;
    FILE*        fp;
    char         errmsg[500],command[100];

    /* --- 只写方式打开文件 --- */
    memset(path,0x00,sizeof(path));
    sprintf(path,"%s%s",local_path,name);
    fp = fopen(path,"wb");
    if (fp == NULL)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        sprintf(ms_str_SocketErrorMessage,"fopen error: 打开文件%s错误!\n",path);
        Tcp_Close(data_conn);
        return FTP_OPENFILE_ERROR;
    }

    /* --- 向服务器发送下载命令 --- */
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
    /* --- 判断传输是否完成 --- */
    ret = Ftp_IsTransferComplete(buffer);

    /* --- 下载文件 --- */
    number_bytes = 0;
    do
    {
        memset(buffer,0x00,sizeof(buffer));
        /* --- 接受文件数据 --- */
        rcv_len = Tcp_Recv(data_conn,buffer,BLOCKSIZE);
        if (rcv_len < 0)       /* --- recv函数调用失败 --- */
        {
            Tcp_Close(data_conn);
            fclose(fp);
            return rcv_len;
        }

        /* --- 写入本地文件 --- */
        nleft = rcv_len;
        ptr = buffer;
        while(nleft > 0)
        {
            count = fwrite(ptr,sizeof(char),nleft,fp);
            /* --- 写入本地文件出错 --- */
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

    /* --- 关闭数据连接 --- */
    Tcp_Close(data_conn);

    /* --- 接收服务器端最后的应答消息 --- */
    if (!ret)
    {
        memset(buffer,0x00,sizeof(buffer));
        ret = Ftp_RecvFtpSvrMsg(ctl_conn,"226 ","文件下载结束",buffer);
    }

    /* --- 关闭文件句柄 --- */
    fclose(fp);

    return ret;
}

/* ------------------------------------------------------
功能描述:   获得目录下所有文件
返回值说明: 成功返回1,失败返回错误代码
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

    /* --- 发送FTP命令 --- */
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
    /* --- 建立数据连接 --- */
    data_conn = Ftp_EstablishDataConn(ctl_conn);
    if(!data_conn)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"数据连接建立出错\n");

        Tcp_Close(ctl_conn);
        return 0;
    }
    /* --- 设置数据连接超时 --- */
    timeout.tv_sec = 15;
    timeout.tv_usec = 0;
    setsockopt(data_conn,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(timeout));
    setsockopt(data_conn,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(timeout));

    /* --- 只写方式打开文件 --- */
    memset(path,0x00,sizeof(path));
    sprintf(path,"%s%s",LocalPath,LocalName);
    fp = fopen(path,"wb");
    if (fp == NULL)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        sprintf(ms_str_SocketErrorMessage,"fopen error: 打开文件%s错误!\n",path);

        /* --- 关闭数据连接 --- */
        Tcp_Close(data_conn);

        return FTP_OPENFILE_ERROR;
    }
    /* --- 发送FTP命令 --- */
    memset(command,0x00,sizeof(command));
    sprintf(command,"LIST %s\r\n",".");
    ret = Tcp_Send(ctl_conn,command,strlen(command));
    if (!ret)
    {
        /* --- 关闭数据连接 --- */
        Tcp_Close(data_conn);
        /* --- 关闭文件句柄 --- */
        fclose(fp);

        return 0;
    }
    memset(buffer,0x00,sizeof(buffer));
    ret = Ftp_RecvFtpSvrMsg(ctl_conn,"150 ","LIST",buffer);
    if (ret != 1)
    {
        /* --- 关闭数据连接 --- */
        Tcp_Close(data_conn);
        /* --- 关闭文件句柄 --- */
        fclose(fp);

        return 0;
    }
    ret = Ftp_IsTransferComplete(buffer);
    /* --- 目录列表信息写入文件 --- */
    do
    {
        memset(buffer,0x00,sizeof(buffer));
        rcv_len = Tcp_Recv(data_conn,buffer,BLOCKSIZE);
        if (rcv_len < 0)
        {
            memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
            strcpy(ms_str_SocketErrorMessage,"List 目录错误!\n");

            /* --- 关闭数据连接 --- */
            Tcp_Close(data_conn);
            /* --- 关闭文件句柄 --- */
            fclose(fp);

            return 0;
        }

        /* --- 写入本地文件 --- */
        nleft = rcv_len;
        ptr = buffer;
        while(nleft > 0)
        {
            count = fwrite(ptr,sizeof(char),nleft,fp);
            /* --- 写入本地文件出错 --- */
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

    /* --- 关闭数据连接 --- */
    Tcp_Close(data_conn);
    if (!ret)
    {
        memset(buffer,0x00,sizeof(buffer));
        ret = Ftp_RecvFtpSvrMsg(ctl_conn,"226 ","list end",buffer);
    }
    /* --- 关闭文件句柄 --- */
    fclose(fp);

    return ret;
}


int CSocket::Ftp_RenameRemoteFile(int ctl_conn,char *sourcePath,char *destPath)
{
    char         command[100],buffer[BLOCKSIZE];
    int          ret;
    /* --- 给服务器发送USER命令 --- */
    memset(command,0x00,sizeof(command));
    sprintf(command,"RNFR %s\r\n",sourcePath);
    ret = Tcp_Send(ctl_conn,command,strlen(command));
    if (ret < 0)
    {
        return ret;
    }
    /* --- 接收应答 --- */
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

    /* --- 给服务器发送USER命令 --- */
    memset(command,0x00,sizeof(command));
    sprintf(command,"RNTO %s\r\n",destPath);
    ret = Tcp_Send(ctl_conn,command,strlen(command));
    if (ret < 0)
    {
        return ret;
    }
    /* --- 接收应答 --- */
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
功能描述:   删除远程文件
返回值说明: 成功返回1,失败返回错误代码
--------------------------------------------------------- */
int CSocket::Ftp_DeleteFile(int ctl_conn,char *filename)
{
    int ret;
    char command[100],buffer[BLOCKSIZE];

    /* --- 给服务器发送USER命令 --- */
    memset(command,0x00,sizeof(command));
    sprintf(command,"DELE %s\r\n",filename);
    ret = Tcp_Send(ctl_conn,command,strlen(command));
    if (ret < 0)
    {
        return ret;
    }
    /* --- 接收应答 --- */
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
功能描述:   寻找指定字符所在索引
返回值说明: 成功返回字符所在位置
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
功能描述:   建立控制连接和数据连接并且下载目录列表
返回值说明: 成功返回1,失败返回错误代码
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
        strcpy(ms_str_SocketErrorMessage,"没有提供服务器端IP地址!\n");

        return FTP_NOREMOTEIP_ERROR;
    }
    if (strlen(user) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"没有提供用户名!\n");

        return FTP_NOUSER_ERROR;
    }
    if (strlen(pass) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"没有提供密码!\n");

        return FTP_NOPASSWD_ERROR;
    }
    if (strlen(local_path) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"没有提供本地目录名!\n");

        return FTP_NOLOCALPATH_ERROR;
    }
    if (strlen(local_name) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"没有提供本地文件名!\n");

        return FTP_NOLOCALPATH_ERROR;
    }
    if (strlen(remote_path) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"没有提供远程目录名(ready)!\n");

        return FTP_NOREMOTEPATH_ERROR;
    }

    memset(LocalPath,0x00,sizeof(LocalPath));
    memset(LocalName,0x00,sizeof(LocalName));
    memset(RemotePath,0x00,sizeof(RemotePath));
    strcpy(LocalPath,local_path);
    strcpy(LocalName,local_name);
    strcpy(RemotePath,remote_path);
    Ftp_ModifyPath(LocalPath,RemotePath);

    /* --- 建立控制连接 --- */
    ctl_conn = Tcp_Connect(control_conn_ip,port,5000);
    if (ctl_conn <= 0)
    {
        return ctl_conn;
    }
    /* --- 设置超时 --- */
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

    /* --- 向服务器发送用户名、密码等命令 --- */
    char fullpath[MAX_PATH];
	memset(fullpath,0x00,sizeof(fullpath));
    ret = Ftp_InitCtrlConn(ctl_conn,user,pass,RemotePath,fullpath);
    if (ret != SUCCESS)
    {
        Tcp_Close(ctl_conn);
        return ret;
    }

    /* --- 目录列表 --- */
    ret = Ftp_GetRemoteDirList(ctl_conn,LocalPath,LocalName);
    /* --- 关闭控制连接 --- */
    Tcp_Close(ctl_conn);

    return ret;
}

/* ------------------------------------------------------
      功能描述:   建立控制连接和数据连接并且上传文件
	  返回值说明: 成功返回1,失败返回错误代码
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
        strcpy(ms_str_SocketErrorMessage,"没有提供服务器端IP地址!\n");

        return FTP_NOREMOTEIP_ERROR;
    }
    if (strlen(user) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"没有提供用户名!\n");

        return FTP_NOUSER_ERROR;
    }
    if (strlen(pass) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"没有提供密码!\n");

        return FTP_NOPASSWD_ERROR;
    }
    if (strlen(local_path) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"没有提供本地目录名!\n");

        return FTP_NOLOCALPATH_ERROR;
    }
    if (strlen(remote_temppath) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"没有提供远程目录名(temp)!\n");

        return FTP_NOREMOTEPATH_ERROR;
    }
    if (strlen(localname) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"没有提供本地文件名!\n");

        return FTP_NOFILENAME_ERROR;
    }
    if (strlen(remotename) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"没有提供远程文件名!\n");

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

    /* --- 给路径名加上"/"或者"\\" --- */
    memset(LocalPath,0x00,sizeof(LocalPath));
    memset(RemotePath,0x00,sizeof(RemotePath));
    strcpy(LocalPath,local_path);
    strcpy(RemotePath,remote_temppath);
    Ftp_ModifyPath(LocalPath,RemotePath);

    /* --- 建立控制连接 --- */
    ctl_conn = Tcp_Connect(control_conn_ip,port,5000);
    if (ctl_conn <= 0)
    {
        return ctl_conn;
    }
    /* --- 设置超时 --- */
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

    /* --- 向服务器发送用户名、密码等命令 --- */
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
				
    /* --- 上传文件 --- */
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
    /* --- 关闭控制连接 --- */
    Tcp_Close(ctl_conn);

    return ret;
}



/* ------------------------------------------------------
      功能描述:   建立控制连接和数据连接并且下载文件
	  返回值说明: 成功返回1,失败返回错误代码
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
        strcpy(ms_str_SocketErrorMessage,"没有提供服务器端IP地址!\n");

        return FTP_NOREMOTEIP_ERROR;
    }
    if (strlen(user) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"没有提供用户名!\n");

        return FTP_NOUSER_ERROR;
    }
    if (strlen(pass) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"没有提供密码!\n");

        return FTP_NOPASSWD_ERROR;
    }
    if (strlen(local_path) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"没有提供本地目录名!\n");

        return FTP_NOLOCALPATH_ERROR;
    }
    if (strlen(remote_path) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"没有提供远程目录名!\n");

        return FTP_NOREMOTEPATH_ERROR;
    }
    if (strlen(name) == 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"没有提供文件名!\n");

        return FTP_NOFILENAME_ERROR;
    }

    /* --- 给路径名加上"/"或者"\\" --- */
    memset(LocalPath,0x00,sizeof(LocalPath));
    memset(RemotePath,0x00,sizeof(RemotePath));
    strcpy(LocalPath,local_path);
    strcpy(RemotePath,remote_path);
    Ftp_ModifyPath(LocalPath,RemotePath);
    Ftp_ModifyPath(local_path,remote_path);

    /* --- 建立控制连接 --- */
    ctl_conn = Tcp_Connect(control_conn_ip,port,5000);
    if (ctl_conn <= 0)
    {
        return ctl_conn;
    }
    /* --- 设置超时 --- */
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

    /* --- 向服务器发送用户名、密码等命令 --- */
    char  fullpath[MAX_PATH];
		memset(fullpath,0x00,sizeof(fullpath));
		ret = Ftp_InitCtrlConn(ctl_conn,user,pass,RemotePath,fullpath);
    if (ret != SUCCESS)
    {
        Tcp_Close(ctl_conn);
        return ret;
    }

    /* --- 建立数据连接 --- */
    data_conn = Ftp_EstablishDataConn(ctl_conn);
    if(data_conn < 0)
    {
        memset(ms_str_SocketErrorMessage,0x00,sizeof(ms_str_SocketErrorMessage));
        strcpy(ms_str_SocketErrorMessage,"数据连接建立出错!\n");

        Tcp_Close(ctl_conn);
        return FTP_DATACONN_ERROR;
    }
    /* --- 设置数据连接超时 --- */
    setsockopt(data_conn,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(timeout));
    setsockopt(data_conn,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(timeout));

    /* --- 下载文件 --- */
    ret = Ftp_GetFile(ctl_conn,data_conn,LocalPath,name);

    /* --- 关闭控制连接 --- */
    Tcp_Close(ctl_conn);

    return ret;

}
