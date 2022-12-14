#ifndef _INCLUDE_H_
#define _INCLUDE_H_

#include <stdio.h>      /*标准输入输出定义*/
#include <stdlib.h>     /*标准函数库定义*/
#include <unistd.h>     /*Unix 标准函数定义*/
#include <sys/types.h>  /*数据类型，比如一些XXX_t的那种*/
#include <sys/stat.h>   /*定义了一些返回值的结构，没看明白*/
#include <fcntl.h>      /*文件控制定义*/
#include <termios.h>    /*PPSIX 终端控制定义*/
#include <errno.h>      /*错误号定义*/
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
