#include "include.h"
#include "include.c"

int main(int argc, char * argv[]){
	int ret, returnlen;
	unsigned char buf[500], returnbuf[500];
	
	char version = 0x01;
	#ifdef FILE_DEBUG_PRINT
	printf("\nTP software update Version:%02x\n", version);
	#endif
	
	ret = fileManager(argv[1]);
	if(ret) return ret;
		
	memset(buf, 0x00, 30);
	memcpy(&buf[0], "\xf2\x00", 2);
	ret = update(argv[1], buf);
	if(ret == 0){
		
		#ifdef FILE_DEBUG_PRINT
		printf("\nSuccess: TP software update The last instruct success\n");
		#endif
		
	}else{
		return -3;
	}
	
	sleep(60);
	return 0;
}
