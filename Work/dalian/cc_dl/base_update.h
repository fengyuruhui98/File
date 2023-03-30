//
// Created by HHJT_CJP on 19-12-8.
//

#ifndef _BASE_UPDATE_H
#define _BASE_UPDATE_H

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <dirent.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <netdb.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <exception>
#include <map>
#include <string>
#include <queue>
#include <map>

using namespace std;

class CUpdate
{
public:
    ~CUpdate();
public:
    static std::map<int,string> ms_map_tcpError;
    static std::map<int,int> ms_map_tcpErrorSysConvert;
    static FILE  *g_fp;
public:
    static map<int,string> InitTcpError();
    static map<int,int> InitTcpErrorSysConvert();
    static int  Tcp_GetSocketErrorMsg(string useName,int errorCode);
    static void  Get_Local_Time(char *buffer);
    static void  WriteCommuLog(const char* buffer);
    static int   detect_process(const char *process_name);

    static int   Tcp_Socket(int type);
    static int   Tcp_Close(int sockfd);
    static int   Tcp_Connect(const char *server_ip,int server_port,int timeout);
    static int   SendPackage();
    static int 	FileIsExist();
    static int 	FileLineCount();
    static int FindChar(char *p,const char c);

};

#endif
