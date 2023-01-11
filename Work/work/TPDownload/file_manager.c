#ifndef FILE_DEBUG_PRINT
#define	FILE_DEBUG_PRINT
#endif

int fileManager(char * port){
	FILE * intFile;
	long lngnum, lngPosition;
	unsigned char buffer[500], returnbuf[500];
	unsigned short length, returnlen;
	int i, ret, tail;
	
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
  
  #ifdef FILE_DEBUG_PRINT
  printf("\nTotal:%ld packages,each package size:%d\n", (lngnum % 240 == 0 ? lngnum / 240 : lngnum /240 + 1), 240);
  #endif
  
  //��ȡ�ļ�
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
  	
		//���͵�i����
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
	memset(&buffer, 0x00, 247);
	if(lngnum % 240 != 0){
		buffer[0] = 0xf1;
  	buffer[1] = 0x00;
  	//memcpy(&buffer[2], length, 2);
  	buffer[3] = (unsigned char)length;
  	buffer[2] = length >> 8;
  	  	
  	buffer[5] = (unsigned char)i;
  	buffer[4] = i >> 8;
  	
		tail = lngnum % 240;
  	buffer[6] = tail;
  	
		
		lngPosition = 240 * i;
  	fseek(intFile, lngPosition, SEEK_SET);
		fread(&buffer, sizeof(char), tail, intFile);
		printf("\nTail package size :%02x\n", tail);
		printf("\nTranslating tail package:\n");
		
		//�������һ����
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
int update(char * port, char * buff){
	int ret = 0, returnlen;
	char * returnbuffer;
	ret = serial(port, buff, 2, returnbuffer, returnlen, 100);
	if(ret == 0){
		#ifdef FILE_DEBUG_PRINT
		printf("\nSuccess: TP software update The last instruct success\n");
		#endif
	}else{
		return -3;
	}
}

