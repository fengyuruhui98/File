#ifndef _INCLUDE_H_
#define _INCLUDE_H_

#include <stdio.h>      /*��׼�����������*/
#include <stdlib.h>     /*��׼�����ⶨ��*/
#include <unistd.h>     /*Unix ��׼��������*/
#include <sys/types.h>  /*�������ͣ�����һЩXXX_t������*/
#include <sys/stat.h>   /*������һЩ����ֵ�Ľṹ��û������*/
#include <fcntl.h>      /*�ļ����ƶ���*/
#include <termios.h>    /*PPSIX �ն˿��ƶ���*/
#include <errno.h>      /*����Ŷ���*/
#include <sys/time.h>
#include <string.h>
#include <linux/serial.h>
#include <asm/ioctls.h>
#include <sys/io.h>
#include <time.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "serial.h"
#include "file_manager.h"

extern unsigned char port[100];

#endif
