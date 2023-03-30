//
// Created by HHJT_CJP on 19-12-10.
//

#ifndef _BASE_SOCKET_H
#define _BASE_SOCKET_H

#define SEND_BUFFER_LENGTH	32*1024     /* ------- ���ͻ������Ĵ�С ------------- */
#define RECV_BUFFER_LENGTH  32*1024     /* ------- ���ջ������Ĵ�С ------------- */

#ifndef MAX_BAOWEN_LENGTH
#define MAX_BAOWEN_LENGTH		8*1024	    /* ------- ���ĵ���󳤶� ----- */
#endif

#define BLOCKSIZE           1024        /* --- һ�ν��յ��ֽ�(FTP) ---- */

/* ------------------------------------------- */
/*                 Socket�����������          */
/* ------------------------------------------- */
#define TCP_ERROR_EALREADY      -1001 /* --- socket is not blocking and previous connection attempt has not yet completed---*/
#define TCP_ERROR_ENOTSOCK			-1002 /* --- Socket operation no nonsocket -------------*/
#define TCP_ERROR_ENOBUFS				-1003 /* --- No buffer space available -----------------*/
#define TCP_ERROR_EMSGSIZE			-1004 /* --- Message is too long -----------------------*/
#define TCP_ERROR_ENOTCONN			-1005 /* --- Socket is not connected -------------------*/
#define TCP_ERROR_EISCONN				-1006 /* --- Socket is already connected -------------------*/
#define TCP_ERROR_ECONNABORTED	-1007 /* --- Software caused connection abort ----------*/
#define TCP_ERROR_ECONNREFUSED	-1008 /* --- Connection refused ------------------------*/
#define TCP_ERROR_ECONNRESET		-1009 /* --- Connection reset by peer ------------------*/
#define TCP_ERROR_ETIMEDOUT			-1010 /* --- Connection timed out ----------------------*/
#define TCP_ERROR_ENETDOWN			-1011 /* --- Network is down ---------------------------*/
#define TCP_ERROR_ENETUNREACH		-1012 /* --- Network is unreachable --------------------*/
#define TCP_ERROR_EIO      			-1013 /* --- I/O error ---*/
#define TCP_ERROR_EBADF     		-1014 /* ---  s is not a valid file descriptor ---*/
#define TCP_ERROR_EHOSTUNREACH	-1015 /* --- No route to host --------------------------*/
#define TCP_ERROR_EHOSTDOWN			-1016 /* --- Host is down ------------------------------*/
#define TCP_ERROR_EADDRINUSE		-1017 /* --- Address already in use --------------------*/
#define TCP_ERROR_EWOULDBLOCK		-1018 /* --- Resource temporarily unavailable ----------*/
#define TCP_ERROR_EACCES				-1019 /* --- Permission denied ----------*/
#define TCP_ERROR_EFAULT  			-1020 /* --- Bad address ----------*/
#define TCP_ERROR_EINVAL     		-1021 /* --- Invalid argument ----------*/
#define TCP_ERROR_EMFILE    		-1022 /* --- Too many open files(sockets) ----------*/
#define TCP_ERROR_EINPROGRESS		-1023 /* --- Operaton now in progress ----------*/
#define TCP_ERROR_EDESTADDRREQ	-1024 /* --- Destination address required ----------*/
#define TCP_ERROR_EPROTOTYPE		-1025 /* --- Protocol wrong type for socket ----------*/
#define TCP_ERROR_ENOPROTOOPT		-1026 /* --- Bad protocol option ----------*/
#define TCP_ERROR_EPROTONOSUPPORT -1027 /* --- Protocol not supported ----------*/
#define TCP_ERROR_ESOCKTNOSUPPORT -1028 /* --- Socket type not supported ----------*/
#define TCP_ERROR_EOPNOTSUPP	    -1029 /* --- Operation not supported ----------*/
#define TCP_ERROR_EPFNOSUPPORT	  -1030 /* --- Protocol family not supported ----------*/
#define TCP_ERROR_EAFNOSUPPORT		-1031 /* --- Address family not supported by protocol family ----------*/
#define TCP_ERROR_EADDRNOTAVAIL		-1032 /* --- Cannot assign requested address ----------*/
#define TCP_ERROR_ENFILE   	      -1033 /* --- the system's table of open files is temporarily full ---*/
#define TCP_ERROR_ENOMEM	        -1034 /* --- No memorty is avaliale ---*/
#define TCP_ERROR_EINTR         	-1035 /* --- Interrupted function call ----------*/
#define TCP_ERROR_DEFAULT	        -1036 /* --- Other errors ---*/


/* ------------------------------------------- */
/*                 �Զ�������������          */
/* ------------------------------------------- */
#ifndef SUCCESS
#define SUCCESS                    1          /* --- �����ɹ����� ----------------- */
#endif

#define SERVER_NOT_INITIAL_ERROR   -1         /* --- ��������û�г�ʼ�� ---------- */
#define SERVER_INITIAL_ERROR       -2         /* --- ��������ʼ��ʧ�� ------------- */
#define SERVER_START_ERROR         -3         /* --- ����������ʧ�� --------------- */
#define CLOCK_ADJUST_ERROR         -4         /* --- ʱ��У��ʧ�� ----------------- */
#define MSGQ_EMPTY_ERROR           -5         /* --- ��Ϣ����Ϊ�� ----------------- */
#ifndef PACKAGE_TOO_BIG_ERROR
#define PACKAGE_TOO_BIG_ERROR      -6         /* --- ���Ĵ�С�����ر��й涨��32KB -- */
#endif
#define FTP_NOREMOTEIP_ERROR       -7         /* --- û���ṩԶ��IP(FTP) ---------- */
#define FTP_NOUSER_ERROR           -8         /* --- û���ṩ�û���(FTP) ---------- */
#define FTP_NOPASSWD_ERROR         -9         /* --- û���ṩ����(FTP) ------------ */
#define FTP_NOLOCALPATH_ERROR      -10        /* --- û���ṩ����Ŀ¼��(FTP) ------ */
#define FTP_NOREMOTEPATH_ERROR     -11        /* --- û���ṩԶ��Ŀ¼��(FTP) ------ */
#define FTP_NOFILENAME_ERROR       -12        /* --- û���ṩ�ļ���(FTP) ---------- */
#define FTP_USER_ERROR             -13        /* --- �û�������(FTP) -------------- */
#define FTP_PASS_ERROR             -14        /* --- �������(FTP) ---------------- */
#define FTP_CWD_ERROR              -15        /* --- ����Զ��Ŀ¼ʧ��(FTP) -------- */
#define FTP_TYPE_ERROR             -16        /* --- ���Ĵ�������ʧ��(FTP) -------- */
#define FTP_PASV_ERROR             -17        /* --- PASV����ʧ��(FTP) ------------ */
#define FTP_RETR_ERROR             -18        /* --- RETR����ʧ��(FTP) ------------ */
#define FTP_CTLCONN_ERROR          -19        /* --- �������ӽ���ʧ��(FTP) -------- */
#define FTP_DATACONN_ERROR         -20        /* --- �������ӽ���ʧ�� ------------- */
#define FTP_CTLCONNCLOSED_ERROR    -21        /* --- �������ӹر� ----------------- */
#define FTP_OPENFILE_ERROR         -22        /* --- �����ļ���ʧ��ʧ�� --------- */
#define FTP_WRITEFILE_ERROR        -23        /* --- д�뱾���ļ�ʧ�� ------------- */
#define FTP_READFILE_ERROR         -24        /* --- ��ȡ�����ļ�ʧ�� ------------- */
#define FTP_RNFR_ERROR             -25        /* --- RNFR����ʧ��(FTP) ------------ */
#define FTP_RNTO_ERROR             -26        /* --- RNTO����ʧ��(FTP) ------------ */
#define CONNECT_SERVERIP_ERROR     -27        /* --- û���ṩԶ��IP  -------------- */
#define GET_LOCALIP_ERROR          -28        /* --- ��ñ���IP����  -------------- */

#ifndef MAX_PATH
#define MAX_PATH     	        256	    /* ------- �ļ�������󳤶� ---- */
#endif


#include "sys_head.h"

class CSocket
{
private:
    ~CSocket();

public:
    static char  ms_str_SocketErrorMessage[1024]; //save tcp error message(after  call Tcp_GetSocketErrorMsg,Ftp_RecvFtpSvrMsg)
public:
    static int  Tcp_GetSocketErrorMsg(const char *buffer,unsigned int error);
    static int  Tcp_Socket(int type);
    static int  Tcp_SetSocketBuf(int sockfd);
    static int  Tcp_Listen(int port);
    static int  Tcp_Close(int sockfd);
    static int  Tcp_Accept(int listenfd,char *clnip,int &port);
    static int  Tcp_Connect(char *server_ip,int server_port,int timeout);
    static int  Tcp_Send(int sockfd,char *buffer,int snd_len);
    static int  Tcp_Recv(int sockfd,char *buffer,int rcv_len);

 public: 
    static void  Ftp_ModifyPath(char *local_path,char *remote_path);
    static int   Ftp_InitCtrlConn(int ctl_conn,char *user,char *pass,char *remote_path,char *fullpath);
    static int   Ftp_RecvFtpSvrMsg(int sockfd,const char *code,const char *type,char *buffer);
    static int   Ftp_EstablishDataConn(int ctl_conn);
    static int   Ftp_IsTransferComplete(char *buffer);
    static int   Ftp_PutFile(int ctl_conn,char *local_path,char *localname,char *remotename);
    static int   Ftp_GetFile(int ctl_conn,int data_conn,char *local_path,char *name);
    static int   Ftp_RenameRemoteFile(int ctl_conn,char *sourcePath,char *destPath);
    static int   Ftp_GetRemoteDirList(int ctl_conn,char *LocalPath,char *LocalName);
    static int   Ftp_DeleteFile(int ctl_conn,char *filename);
    static int   Ftp_FindChar(char *p,const char c); 
     
 	  static int   Ftp_GetDirList(char *control_conn_ip,int port,char *user,char *pass,char *local_path,const char *local_name,char *remote_path);
    static int   Ftp_PutFile(char *control_conn_ip,int port,char *user,char *pass,char *local_path,char *localname,char *remote_temppath,char *remote_readypath,char *remotename);
		static int   Ftp_GetFile(char *control_conn_ip,int port,char *user,char *pass,char *local_path,char *name,char *remote_path);

};

#endif //_BASE_SOCKET_H
