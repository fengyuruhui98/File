#include "include.c"
#include "include.h"

pthread_t pthID1, pthID2;
sem_t timewait;

/*########################################################################
函数名：pthtmr
参数：NULL
返回值：NULL
功能：显示信号，设置看门狗
########################################################################*/
void *pthtmr(){
	char led_ctrl = LED_OFF;

#ifdef DEBUG_PRINT
	printf("\nDebugtag = main-0, timer thread start\n");
#endif
	for(;;){
		sem_wait(&timewait);
		sleep(1);
		watchdog();
		
		if(led_ctrl == LED_ON){
			gled(LED_OFF);
			led_ctrl = LED_OFF;	
		}
		else{
			gled(LED_ON);
			led_ctrl = LED_ON;
		}
		sem_post(&timewait);
	}		
}

/*########################################################################
函数名：main
参数：NULL
返回值：NULL
功能：主函数
########################################################################*/
int main(int argc, char *argv[]){
	time_t lngtime, lnglocaltime;
	FILE *f1, *f2;
	int i, j, ret;
	char filename[40];
	unsigned char chreturn, buf[10], name_4016[10], out_buf[200], chtemp[1000];
	pid_t pid;
	short firstdays, enddays;
	struct tm tptime;
	
#ifdef DEBUG_BOM
	printf("\nDebugtag = main-1, shmt_read_pf4016 start...\n");
	printf("\nShanghai soft Ver = %02x", Version);
#endif 
	//
	EOD_4020_WhiteList();
	EOD_4016_Cert();
	//
	Init_CRC32_Table();
	//4016
	memset(buf, 0x00, 10);
	sprintf(buf, "4016");
	shmt_read_pf4016(name_4016);
}