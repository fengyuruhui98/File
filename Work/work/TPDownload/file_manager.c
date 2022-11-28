#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>  /*数据类型，比如一些XXX_t的那种*/
#include <sys/stat.h>   /*定义了一些返回值的结构，没看明白*/
#include <fcntl.h>      /*文件控制定义*/

#ifndef FILE_DEBUG_PRINT
#define	FILE_DEBUG_PRINT
#endif

int fileManager(char * port){
	FILE * intFile;
	long lngnum, lngPosition;
	unsigned char buffer[500], returnbuf[500];
	unsigned short length, returnlen;
	int i, ret;
	
	intFile = fopen("./TP.tar", "rb");
	if(intFile == NULL){
		
		#ifdef FILE_DEBUG_PRINT
		printf("lack of TP software\n");
		#endif

		return -1;
	}
	fseek(intFile, 0, SEEK_END);
	lngnum = ftell(intFile);
  fseek(intFile, 0, 0);
  
	#ifdef FILE_DEBUG_PRINT
  printf("\nLength: TP software Length is %ld\n", lngnum);
	#endif

  if(lngnum == 0){
  	
  	#ifdef DEBUG_PRINT
  	printf("\nErr: scanner solid software is NULL\n");
  	#endif
  	
  	return -2;
  }
  
  if(lngnum % 240 == 0) 
  	length = lngnum/240;
  else length = lngnum /240 + 1;
  
  //读取文件
  for(i = 1; i <= lngnum/240; i++){
  	
  	memset(&buffer, 0x00, 247);
  	buffer[0] = 0xf1;
  	buffer[1] = 0x00;
  	//memcpy(&buffer[2], length, 2);
  	buffer[3] = (unsigned char)length;
  	buffer[2] = length >> 8;
  	  	
  	buffer[5] = (unsigned char)i;
  	buffer[4] = i >> 8;
  	
  	buffer[6] = 0xf0;
  	
  	lngPosition = 240 * i;
  	fseek(intFile, lngPosition, SEEK_SET);
  	fread(&buffer[7], sizeof(char), 240, intFile);
  	
  	#ifdef FILE_DEBUG_PRINT
  	printf("\nTranslating The %d th package data:\n", i);
  	#endif
  	
		//传送第i个包
  	ret = serial(port, buffer, 247, returnbuf, returnlen, 100);
  	if(ret == 0){
  		
  		#ifdef FILE_DEBUG_PRINT
  		printf("\nSuccess: scanner solid software update The %d th package success\n", i);
  		#endif
  		
  	}else{
  		return -3;
  	}
	}
	
	#ifdef FILE_DEBUG_PRINT
	printf("\nTransfered %d packages, still have 1 package to send.\n", i);
	#endif
	
	if(lngnum % 240 != 0){
		
		buffer[0] = 0xf1;
  	buffer[1] = 0x00;
  	//memcpy(&buffer[2], length, 2);
  	buffer[3] = (unsigned char)length;
  	buffer[2] = length >> 8;
  	  	
  	buffer[5] = (unsigned char)i;
  	buffer[4] = i >> 8;
  	
  	buffer[6] = 0xf0;
  	
		memset(&buffer, 0x00, 247);
		lngPosition += 240;
  	fseek(intFile, lngPosition, SEEK_SET);
		int tail = lngnum % 240;
		fread(&buffer, sizeof(char), tail, intFile);
		printf("\nTail package size :%d\n", tail);
		printf("\nTranslating tail package:\n");
		
		//传送最后一个包
  	ret = serial(port, buffer, tail + 7, returnbuf, returnlen, 100);
  	if(ret == 0)
  	{
  		#ifdef FILE_DEBUG_PRINT
  		printf("\nSuccess: All Frame send success\n");	
  		#endif
  	}else{
  		return -4;
  	}
	}
	fclose(intFile);
	return 0;
}





















	  
	  