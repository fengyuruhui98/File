//
// Created by HHJT_CJP on 19-12-8.
//
#include "base_dog.h"
#include "base_socket.h"
#include <iostream>

map<int,string> CDog::ms_map_tcpError = CDog::InitTcpError();
map<int,string> CDog::InitTcpError()
{
    map<int,string> mapTemp;
    mapTemp.clear();
    mapTemp.insert(pair<int,string>(-1001,"TCP_ERROR_EALREADY(socket is not blocking and previous connection attempt has not yet completed)"));
    mapTemp.insert(map<int,string>::value_type(-1002," TCP_ERROR_ENOTSOCK(Socket operation no nonsocket)"));
    mapTemp.insert(map<int,string>::value_type(-1003," TCP_ERROR_ENOBUFS(No buffer space available)"));
    mapTemp.insert(map<int,string>::value_type(-1004," TCP_ERROR_EMSGSIZE(Message is too long)"));
    mapTemp.insert(map<int,string>::value_type(-1005," TCP_ERROR_ENOTCONN(Socket is not connected)"));
    mapTemp.insert(map<int,string>::value_type(-1006," TCP_ERROR_EISCONN(Socket is already connected)"));
    mapTemp.insert(map<int,string>::value_type(-1007," TCP_ERROR_ECONNABORTED(Software caused connection abort)"));
    mapTemp.insert(map<int,string>::value_type(-1008," TCP_ERROR_ECONNREFUSED(Connection refused)"));
    mapTemp.insert(map<int,string>::value_type(-1009," TCP_ERROR_ECONNRESET(Connection reset by peer)"));
    mapTemp.insert(map<int,string>::value_type(-1010," TCP_ERROR_ETIMEDOUT(Connection timed out)"));
    mapTemp.insert(map<int,string>::value_type(-1011," TCP_ERROR_ENETDOWN(Network is down)"));
    mapTemp.insert(map<int,string>::value_type(-1012," TCP_ERROR_ENETUNREACH(Network is unreachable)"));
    mapTemp.insert(map<int,string>::value_type(-1013," TCP_ERROR_EIO(I/O error)"));
    mapTemp.insert(map<int,string>::value_type(-1014," TCP_ERROR_EBADF(s is not a valid file descriptor)"));
    mapTemp.insert(map<int,string>::value_type(-1015," TCP_ERROR_EHOSTUNREACH(No route to host)"));
    mapTemp.insert(map<int,string>::value_type(-1016," TCP_ERROR_EHOSTDOWN(Host is down)"));
    mapTemp.insert(map<int,string>::value_type(-1017," TCP_ERROR_EADDRINUSE(Address already in use )"));
    mapTemp.insert(map<int,string>::value_type(-1018," TCP_ERROR_EWOULDBLOCK(Resource temporarily unavailable)"));
    mapTemp.insert(map<int,string>::value_type(-1019," TCP_ERROR_EACCES(Permission denied)"));
    mapTemp.insert(map<int,string>::value_type(-1020," TCP_ERROR_EFAULT(Bad address)"));
    mapTemp.insert(map<int,string>::value_type(-1021," TCP_ERROR_EINVAL(Invalid argument)"));
    mapTemp.insert(map<int,string>::value_type(-1022," TCP_ERROR_EMFILE(Too many open files(sockets))"));
    mapTemp.insert(map<int,string>::value_type(-1023," TCP_ERROR_EINPROGRESS(Operaton now in progress)"));
    mapTemp.insert(map<int,string>::value_type(-1024," TCP_ERROR_EDESTADDRREQ(Destination address required)"));
    mapTemp.insert(map<int,string>::value_type(-1025," TCP_ERROR_EPROTOTYPE(Protocol wrong type for socket)"));
    mapTemp.insert(map<int,string>::value_type(-1026," TCP_ERROR_ENOPROTOOPT(Bad protocol option)"));
    mapTemp.insert(map<int,string>::value_type(-1027," TCP_ERROR_EPROTONOSUPPORT(Protocol not supported)"));
    mapTemp.insert(map<int,string>::value_type(-1028," TCP_ERROR_ESOCKTNOSUPPORT(Socket type not supported)"));
    mapTemp.insert(map<int,string>::value_type(-1029," TCP_ERROR_EOPNOTSUPP(Operation not supported)"));
    mapTemp.insert(map<int,string>::value_type(-1030," TCP_ERROR_EPFNOSUPPORT(Protocol family not supported)"));
    mapTemp.insert(map<int,string>::value_type(-1031," TCP_ERROR_EAFNOSUPPORT(Address family not supported by protocol family)"));
    mapTemp.insert(map<int,string>::value_type(-1032," TCP_ERROR_EADDRNOTAVAIL(Cannot assign requested address)"));
    mapTemp.insert(map<int,string>::value_type(-1033," TCP_ERROR_ENFILE(the system's table of open files is temporarily full)"));
    mapTemp.insert(map<int,string>::value_type(-1034," TCP_ERROR_ENOMEM(No memorty is avaliale)"));
    mapTemp.insert(map<int,string>::value_type(-1035," TCP_ERROR_EINTR(Interrupted function call)"));
    mapTemp.insert(map<int,string>::value_type(-1036," TCP_ERROR_DEFAULT(Other errors)"));

    return mapTemp;

}


map<int,int> CDog::ms_map_tcpErrorSysConvert = CDog::InitTcpErrorSysConvert();
map<int,int> CDog::InitTcpErrorSysConvert()
{


    map<int, int> mapTemp;
    mapTemp.clear();
    mapTemp.insert(map<int, int>::value_type(EALREADY, -1001));
    mapTemp.insert(map<int, int>::value_type(ENOTSOCK, -1002));
    mapTemp.insert(map<int, int>::value_type(ENOBUFS, -1003));
    mapTemp.insert(map<int, int>::value_type(EMSGSIZE, -1004));
    mapTemp.insert(map<int, int>::value_type(ENOTCONN, -1005));
    mapTemp.insert(map<int, int>::value_type(EISCONN, -1006));
    mapTemp.insert(map<int, int>::value_type(ECONNABORTED, -1007));
    mapTemp.insert(map<int, int>::value_type(ECONNREFUSED, -1008));
    mapTemp.insert(map<int, int>::value_type(ECONNRESET, -1009));
    mapTemp.insert(map<int, int>::value_type(ETIMEDOUT, -1010));
    mapTemp.insert(map<int, int>::value_type(ENETDOWN, -1011));
    mapTemp.insert(map<int, int>::value_type(ENETUNREACH, -1012 ));
    mapTemp.insert(map<int, int>::value_type(EIO, -1013));
    mapTemp.insert(map<int, int>::value_type(EBADF, -1014));
    mapTemp.insert(map<int, int>::value_type(EHOSTUNREACH, -1015));
    mapTemp.insert(map<int, int>::value_type(EHOSTDOWN, -1016));
    mapTemp.insert(map<int, int>::value_type(EADDRINUSE,-1017));
    mapTemp.insert(map<int, int>::value_type(EWOULDBLOCK, -1018));
    mapTemp.insert(map<int, int>::value_type(EACCES, -1019));
    mapTemp.insert(map<int, int>::value_type(EFAULT, -1020));
    mapTemp.insert(map<int, int>::value_type(EINVAL, -1021));
    mapTemp.insert(map<int, int>::value_type(EMFILE, -1022));
    mapTemp.insert(map<int, int>::value_type(EINPROGRESS, -1023));
    mapTemp.insert(map<int, int>::value_type(EDESTADDRREQ, -1024));
    mapTemp.insert(map<int, int>::value_type(EPROTOTYPE, -1025));
    mapTemp.insert(map<int, int>::value_type(ENOPROTOOPT, -1026));
    mapTemp.insert(map<int, int>::value_type(EPROTONOSUPPORT, -1027));
    mapTemp.insert(map<int, int>::value_type(ESOCKTNOSUPPORT, -1028));
    mapTemp.insert(map<int, int>::value_type(EOPNOTSUPP,-1029));
    mapTemp.insert(map<int, int>::value_type(EPFNOSUPPORT, -1030));
    mapTemp.insert(map<int, int>::value_type(EAFNOSUPPORT, 1031));
    mapTemp.insert(map<int, int>::value_type(EADDRNOTAVAIL, -1032));
    mapTemp.insert(map<int, int>::value_type(ENFILE, -1033));
    mapTemp.insert(map<int, int>::value_type(ENOMEM, -1034));
    mapTemp.insert(map<int, int>::value_type(EINTR, -1035));

    return mapTemp;

}

int CDog::Tcp_GetSocketErrorMsg(string useName,int errorCode)
{
    map<int,int >::iterator iter1;
    int tcpErrorCode = -1036;

    iter1 = ms_map_tcpErrorSysConvert.find(errorCode);
    if (iter1 != ms_map_tcpErrorSysConvert.end())
    {
        tcpErrorCode = iter1->second;
    }

    map<int,string>::iterator iter2;
    iter2 = ms_map_tcpError.find(tcpErrorCode);
    WriteCommuLog(useName.append(iter2->second.c_str()).append("\n").c_str());

    return tcpErrorCode;
}

void CDog::Get_Local_Time(char *buffer)
{
    time_t    long_time;
    struct tm *newtime;

    time(&long_time);
    newtime = localtime(&long_time);

    sprintf(buffer,"%02d:%02d:%02d %04d��%02d��%02d��\t",newtime->tm_hour,newtime->tm_min,newtime->tm_sec,
            (newtime->tm_year+1900),(newtime->tm_mon+1),newtime->tm_mday);
}


FILE *CDog::g_fp = fopen("./dog.txt","a+");
void CDog::WriteCommuLog(const char* buffer)
{
    struct stat buf;
    char errmsg[500];

    if(g_fp == NULL)
        return;

    memset(errmsg,0x00,sizeof(errmsg));
    Get_Local_Time(errmsg);

    strcat(errmsg,buffer);
    fprintf(g_fp,errmsg);
    fflush(g_fp);
}

int CDog::FindChar(char *p,const char c)
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

/* ----------------------------------------------------- */
/*                  �ж��ļ��Ƿ����                     */
/* ----------------------------------------------------- */
int CDog::FileLineCount()
{
    FILE  *fp;
    int   count,index;
    char  linetext[1024];
    char  *p,*q;
    char  temp[64];

    count = 0;
    fp = NULL;
    fp = fopen("/etc/ld.so.conf","rb");
    if(fp == NULL)
        return 0;
    while(!feof(fp))//��ȡ�ļ���Ϣ
    {
        /* --- ��ȡһ�� --- */
        memset(linetext,0x00,sizeof(linetext));
        fgets(linetext,sizeof(linetext),fp);
        /* --- ���з� --- */
        index = FindChar(linetext,0x0d);
        /* --- ���һ�� --- */
        if(index == 0)
            break;
        count = count + 1;
    }

    fclose(fp);

    return count;
}

/* ----------------------------------------------------- */
/*                  �ж��ļ��Ƿ����                     */
/* ----------------------------------------------------- */
int CDog::FileIsExist()
{
    int           rtn;
    struct dirent *dirp;
    DIR           *dp;

    rtn = 0;
    /* -------- �򿪴�������Ŀ¼ ---------- */
    if((dp = opendir(".")) == NULL)
    {
        return rtn;
    }
    /* -------- ��ȡ��������Ŀ¼ ---------- */
    while((dirp = readdir(dp)) != NULL)
    {
        if(strcmp(dirp->d_name,".") && strcmp(dirp->d_name,".."))
        {
            if(strcmp(dirp->d_name,"snc.cpp") == 0)
            {
                rtn = 1;
                break;
            }
        }
    }
    /* -------- �رմ�������Ŀ¼ ---------- */
    closedir(dp);

    return rtn;
}


int CDog::Tcp_Socket(int type)
{
    int sockfd;

    sockfd = socket(AF_INET,type,0);
    return sockfd;
}

int CDog::Tcp_Close(int sockfd)
{
    int   ret;
    unsigned int error;

    ret = close(sockfd);
    if (ret != 0)
    {
        error = errno;
        /* --- �ر�socket���ʧ�ܷ��ش������ --- */
        return Tcp_GetSocketErrorMsg("close",error);
    }
    else
    {
        return 1;
    }
}
int CDog::Tcp_Connect(const char *server_ip,int server_port,int timeout)
{
    int                 sockfd;
    int                 ret,error,len,flags;
    struct sockaddr_in  server_adr;
    fd_set              read_fd,write_fd;
    struct timeval      timeoutSTRU;
    char                errmsg[100];

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

    error = 0;
    fcntl(sockfd,F_SETFL,flags | O_NONBLOCK);  /* --- ����Ϊ������socket --- */

    ret = connect(sockfd,(struct sockaddr *)&server_adr,sizeof(server_adr));/* --- ��������˽������� --- */




    if (ret < 0)  /* --- ����ʧ�� --- */
    {
        Tcp_Close(sockfd);
        return Tcp_GetSocketErrorMsg("connect",errno);
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
        ret = Tcp_GetSocketErrorMsg("connect",errno);
        Tcp_Close(sockfd);	 /* --- �ر��׽��� --- */
        return ret;
    }
    if (ret < 0)          /* --- select ���� --- */
    {
        ret = Tcp_GetSocketErrorMsg("select",errno);
        Tcp_Close(sockfd);
        return ret;
    }

    if (FD_ISSET(sockfd,&read_fd) || FD_ISSET(sockfd,&write_fd))
    {
        len = sizeof(error);
        if (getsockopt(sockfd,SOL_SOCKET,SO_ERROR,&error,( socklen_t *)&len) < 0)
        {
            ret = Tcp_GetSocketErrorMsg("getsockopt",errno);
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
        ret = Tcp_GetSocketErrorMsg("connect",errno);
        return ret;
    }

    return sockfd;
}

int CDog::SendPackage()
{
    int sockfd;
    int ret,n;
    char buffer[100];

		int PackageTotalLenth = 0x40;  
		int PackageLenth = 0x04;

    memset(buffer,0x00,sizeof(buffer));
    buffer[3] = PackageTotalLenth - PackageLenth;  //

    ret = 0;
    sockfd = Tcp_Connect("127.0.0.1",8002,5000);  
    if(sockfd > 0)
    {
        memset(buffer,0x00,sizeof(buffer));
        strcpy(buffer,"���ӳɹ�\n");
        WriteCommuLog(buffer);
        n = send(sockfd,buffer,PackageTotalLenth,0);
        if(n > 0)
        {
            memset(buffer,0x00,sizeof(buffer));
            strcpy(buffer,"���ͳɹ�\n");
            WriteCommuLog(buffer);
            
            ret = 1;
            
            n = recv(sockfd,buffer,500,0);
            if(n >= 0)
            {
                memset(buffer,0x00,sizeof(buffer));
                strcpy(buffer,"Ӧ��ɹ�\n");
                WriteCommuLog(buffer);
               // ret = 1;
            }
            else
            {
                memset(buffer,0x00,sizeof(buffer));
                strcpy(buffer,"Ӧ��ʧ��\n");
                WriteCommuLog(buffer);
            }
        }
        Tcp_Close(sockfd);
    }

    return ret;
}

/* ------------------------------------------------------
            ��������:  �жϽ����Ƿ���ڣ�1 ���� 0 �����ڣ�
--------------------------------------------------------- */
int CDog::detect_process(const char *process_name)
{
    FILE   *ptr;
    char   buff[512];
    char   ps[128];

    memset(ps,0x00,sizeof(ps));
    sprintf(ps,"ps -ef | grep -c '%s$'",process_name);
    strcpy(buff,"ABNORMAL");
    if((ptr=popen(ps, "r"))   !=   NULL)
    {
        while   (fgets(buff,   512,   ptr)   !=   NULL)
        {
            if(atoi(buff) >= 2)
            {
                pclose(ptr);
                return   1;
            }
        }
    }
    if(strcmp(buff,"ABNORMAL")==0)     /*ps   command   error*/
    {
        return   0;
    }
    pclose(ptr);
    return   0;
}


CDog::~CDog()
{
    fclose(g_fp);
}

/* ------------------------------------------------------
            ��������:  ������Ϊ�ػ�����
--------------------------------------------------------- */
void CDog::Daemon_init()
{
	int pid;
	int i;
	if(pid = fork())
		exit(0);
	else if(pid<0)
		exit(1);
	setsid();
	chdir("./");
	umask(0);
}


int main(int argc,char *argv[])
{

    char Script[256];
    int  rtn;
    char       buffer[500];
    int        count;
    char ora_path[64],user_path[64];
    
    /* --------- ������Ϊ�ػ����� ----------------*/
		CDog::Daemon_init();
 
    memset(buffer,0x00,sizeof(buffer));
    strcpy(buffer,"Dog Is Starting...\n");
    CDog::WriteCommuLog(buffer);

    while(1)
    {
    	  sleep(6);
    	  
        rtn = CDog::SendPackage();
        if(rtn == 1)
        {
            memset(buffer,0x00,sizeof(buffer));
            strcpy(buffer,"send success\n");
            CDog::WriteCommuLog(buffer);

          
        }
        else
        {
        	  memset(buffer,0x00,sizeof(buffer));
            strcpy(buffer,"send failed\n");
            CDog::WriteCommuLog(buffer);
            	
        	  memset(Script,0x00,sizeof(Script));
        		sprintf(Script,"./metro_center");
       		  system(Script);
       		  
       		  sleep(1);
       		  
       		  memset(Script,0x00,sizeof(Script));
        		sprintf(Script,"./snc");
       		  system(Script);
        
        }
    }
 
 
    return 0;

}






