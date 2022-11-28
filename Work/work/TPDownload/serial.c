#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#ifndef SERIAL_DEBUG_PRINT
#define	SERIAL_DEBUG_PRINT
#endif

pthread_mutex_t	rd = PTHREAD_MUTEX_INITIALIZER;

unsigned char serialopen = 0;
struct timeval tv1,tv2;
struct timezone tz1,tz2;
int fd;

int serial(char * port, unsigned char *inbuf, int inlen, unsigned char *outbuf, int outlen, short timeMs){
	unsigned char fStat;
	unsigned char sendbuf[1024], serialname[20], resbuf[50], buff[100];
	int handleFD, ret, trytime, readlen;
	long sendlen;
	fd_set readfd, intervalfd;
	struct timeval timeout, intervalTimeout;
	
	//int uart_index = index;
	//sprintf(serialname,"/dev/ttySAC%d",uart_index);
	
	trytime = 0;
	if( !serialopen ){
		
		#ifdef SERIAL_DEBUG_PRINT
		printf("\n******************\n Open serial\n");
		#endif
		
		handleFD = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
		if(handleFD <= 0){
			
			#ifdef SERIAL_DEBUG_PRINT
			printf("open comm failure\n");
			#endif
			
			return 0xff;
		}
		fcntl(fd, F_SETFL, 0);
		serialopen = 0xff;
		speed_set(handleFD, 57600);
		parity_set(handleFD, 8, 1, 'n');
		
		#ifdef SERIAL_DEBUG_PRINT
		printf("open comm success\n");
		#endif
		
	}
	
	pthread_mutex_lock(&rd);
	
	gettimeofday(&tv1,&tz1);
	if(inlen != 0){
		memcpy(sendbuf, inbuf, inlen);
		sendlen = InsertDLE(sendbuf, inlen);
		//readcom(intFDFile, buff, 1);  
		//usleep(1);
		
		#ifdef SERIAL_DEBUG_PRINT
		printf("Inserted DLE sendbuf\n");
		#endif
		
		tcflush(handleFD, TCIOFLUSH);
		if( !writecom(handleFD, sendbuf, sendlen) ){
			
			#ifdef SERIAL_DEBUG_PRINT
			printf("write sendbuf failure\n");
			#endif
			
			pthread_mutex_unlock(&rd);
			return 0xff;
		}
	}
	
	do{
		FD_ZERO(&readfd);
		FD_SET(handleFD, &readfd);
		timeout.tv_sec = 0;
		timeout.tv_usec = timeMs * 1000;
		readlen = 0;
		ret = select(handleFD + 1, &readfd, NULL, NULL, &timeout);
		if(ret <= 0){
			pthread_mutex_unlock(&rd);
			return 0xff;			
		}
		
		#ifdef SERIAL_DEBUG_PRINT
		printf("got something\n");
		#endif
		
		do{
			fStat = readcom(handleFD, buff, 1);
			if(fStat){
				resbuf[readlen] = buff[0];
				
				#ifdef SERIAL_DEBUG_PRINT
				printf("GET:resbuf[%d]:%d\n", readlen, buff[0]);
				#endif
				
				readlen++;
				if(readlen >= 16){
					
					#ifdef SERIAL_DEBUG_PRINT
					printf("get 16+ byte\n");
					#endif
					
					if(memcmp(&resbuf[3], "\xf1\x00\x00\x14", 4) == 0 || inbuf[0] == 0xf2){
						
						#ifdef SERIAL_DEBUG_PRINT
						printf("return right\n");
						#endif
						
						pthread_mutex_unlock(&rd);
						return 0;
					}
					if (trytime < 3){
						trytime += 1;
						readlen = 0;
						continue;
					}else{
						pthread_mutex_unlock(&rd);
						return 0xff;
					}
					pthread_mutex_unlock(&rd);
					return 0xff;
				}	
			}
			FD_ZERO(&intervalfd);
			FD_SET(handleFD, &intervalfd);
			intervalTimeout.tv_sec = 0;
			intervalTimeout.tv_usec = 5000;
			ret = select(handleFD + 1, &intervalfd, NULL, NULL, &intervalTimeout);
			if(ret > 0) continue;
			//连续接收数据中间中断时间超过5ms
			
			#ifdef SERIAL_DEBUG_PRINT
			printf("read wrong\n");
			#endif
			
			pthread_mutex_unlock(&rd);
			return -1;
		}while(1);
	}while(1);
}

int InsertDLE(unsigned char *pbytData, int intLength){
unsigned char	tempData[600], bytCheck;
int	i, intInsertLen = 1, j;
	
	bytCheck = 0;
	for(i = 0; i < intLength; i++) 		bytCheck += *(pbytData + i);
	*(pbytData + intLength) = bytCheck;

	for(i = 0;i <= intLength; i++){
		bytCheck = *(pbytData + i);
		if((bytCheck == PROT_DT_STX) || (bytCheck == PROT_DT_ETX) || (bytCheck == PROT_DT_DLE)) 
			tempData[intInsertLen++] = PROT_DT_DLE;
		tempData[intInsertLen++] = bytCheck;
	}
	
	tempData[intInsertLen++] = PROT_DT_ETX;
	tempData[0] = PROT_DT_STX;
	
	memcpy(pbytData, tempData, intInsertLen);
	
	#ifdef SERIAL_DEBUG_PRINT
	printf("Package: ");
	for(j = 0; j < intInsertLen; j++){
		printf("%02X", pbytData[j]);
	}	
	printf("\n");
	#endif
	
	return intInsertLen;
}

int DeleteDLE(unsigned char *pbytData, int intLength){
unsigned char bytTempData[600], bytCheck;
int intDeleteLen, i, j;

	if(pbytData[0] != PROT_DT_STX) return 0;
	
	memcpy(bytTempData, pbytData, intLength);
	for(i = 1; i < intLength; i++){
		if(bytTempData[i] == PROT_DT_DLE)
			memcpy(&bytTempData[i], &bytTempData[i + 1], intLength - 1 - i);
	}
	
	intDeleteLen = bytTempData[1] + 4;
	bytCheck = 0; 
	for(i = 1; i <= bytTempData[1] + 1; i++)
		bytCheck += bytTempData[i];

	if(bytCheck == bytTempData[intDeleteLen - 2]){
		memcpy(pbytData, bytTempData, intDeleteLen);
		
		#ifdef SERIAL_DEBUG_PRINT
		printf("Unpackage: ");
		for(j = 0; j < intDeleteLen; j++){
			printf("%02X", pbytData[j]);
		}	
		printf("\n");
		#endif
		
		return intDeleteLen;
	}
	else{
		return 0;
	}
}

unsigned char writecom(int fd, unsigned char *pdata, long lnglen){
long lngwritelen;
FILE *intFile;
char chinfo[300];
long i;

	if(fd <= 0)
		return 0;
	tcflush(fd, TCIOFLUSH);
	lngwritelen = write(fd, pdata, lnglen);
	if(lngwritelen == -1)
		return 0;
	if(lngwritelen != lnglen)
		return 0;
	return 0xff;
}


unsigned char readcom(int fd, unsigned char *pdata, long lnglen){
long lngwritelen;

	if(fd <= 0)
		return 0;
	lngwritelen = read(fd, pdata, lnglen);
	if(lngwritelen == -1)
		return 0;
	if(lngwritelen != lnglen)
		return 0;
	return 0xff;
}


/*
set the serial port baud
*/
int speed_sel[] = {B460800, B115200, B57600, B38400, B19200, B9600};
int name_sel[] = {460800, 115200, 57600, 38400, 19200, 9600};
void speed_set(int fd, int speed)
{
int i, status, tempspeed;
struct termios opt;
struct serial_struct p;
speed_t lngspeed;
	
	if(fd <= 0)
		return ;
		
	tcgetattr(fd, &opt);
	tempspeed = speed;
	if(speed == 28800)
		tempspeed = 38400;
	for(i = 0; i < sizeof(speed_sel) / sizeof(int); i++)
	{
		if(tempspeed == name_sel[i])
		{
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&opt, speed_sel[i]);
			cfsetospeed(&opt, speed_sel[i]);

			status = tcsetattr(fd, TCSANOW, &opt);
			if(status != 0)
			{
				printf("set attr wrong!\n");
				return ;
			}
			tcflush(fd, TCIOFLUSH);//清空中断未完成的输入/输出请求数据
		}
	}
	lngspeed = cfgetispeed(&opt);
	if(speed == 28800)
	{
		ioctl(fd, TIOCGSERIAL, &p);
		p.flags = ASYNC_SPD_CUST;
		p.custom_divisor = 4;
		ioctl(fd, TIOCSSERIAL, &p);
	}
	lngspeed = cfgetispeed(&opt);
}

/*
set parity
*/
int parity_set(int fd, int databits, int stopbits, int parity)
{
struct termios opt;
	
	if(fd <= 0)
		return 1;

	tcgetattr(fd, &opt);
	
	opt.c_cflag &= ~CSIZE;
	switch(databits)
	{
	case 7:
		opt.c_cflag |= CS7;
		break;	
	case 8:
		opt.c_cflag |= CS8;
		break;
	}
	switch(parity)
	{
	case 'n':
	case 'N':
		opt.c_cflag &= ~PARENB;
		opt.c_iflag &= ~INPCK;
		break;
	case 'o':
	case 'O':
		opt.c_cflag |= (PARODD | PARENB);
		opt.c_iflag |= INPCK;
		break;
	case 'e':
	case 'E':
		opt.c_cflag |= PARENB;
		opt.c_cflag &= ~PARODD;
		opt.c_iflag |= INPCK;
		break;
	case 'S':
	case 's': /*as no parity*/
		opt.c_cflag &= ~PARENB;
		opt.c_cflag &= ~CSTOPB;
		break;	
	}
	switch(stopbits)
	{
	case 1:
		opt.c_cflag &= ~CSTOPB;
		break;
	case 2:
		opt.c_cflag |= CSTOPB;
	}
	if(parity != 'n')
		opt.c_iflag |= INPCK;
	opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	opt.c_oflag &= ~OPOST;
	
	opt.c_iflag &= ~(IXON | IXOFF | IXANY | ICRNL);
	tcflush(fd, TCIFLUSH);
	opt.c_cc[VTIME] = 0; //5  2013/11/12 19:19:10
	opt.c_cc[VMIN] = 0;
	tcsetattr(fd, TCSANOW, &opt);
	return(1);
}
