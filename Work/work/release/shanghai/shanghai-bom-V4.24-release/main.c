
#include "include.h"
#include "include.c"

pthread_t	g_pthtmrID,g_pthtmr2ID;
sem_t g_tmrwait;
//sem_t g_usermainztewait,g_usermainztereturn;

/*
function:display the led and set watch dog
*/
void *pthtmr()
{
char	led_ctrl = LED_OFF;

#ifdef DEBUG_PRINT
	printf("timer thread start\n");
#endif
	for(;;)
	{
		sem_wait(&g_tmrwait);
		sleep(1);
		watchdog();
		
		if(led_ctrl == LED_ON)
		{
			gled(LED_OFF);
			//rled(LED_OFF);
			led_ctrl = LED_OFF;
		}
		else
		{
			gled(LED_ON);
			//rled(LED_ON);
			led_ctrl = LED_ON;
		}
		sem_post(&g_tmrwait);
	}
}
 
/*
void *pthtmr2()
{
int i = 0;

#ifdef DEBUG_PRINT
	printf("timer2 thread start\n");
#endif
	for(;;)
	{
		delay_ms(1);
		
		//软时钟变量管理
		if(bgSoftTimerEn){
		for(i=0;i<MAX_TIMER_INDEX;i++){
		   if(!bpgSoftTimerFlag[i]){
		   	 dwpgSoftTimerCnt[i]++;
		   	 if((UDWORD)dwpgSoftTimerCnt[i] >= (UDWORD)dwpgSoftTimerLimit[i]){
		   	 	  bpgSoftTimerFlag[i] = 1;
		   	 	  }
		   	 }
		   }
		}
	}
}
*/

int main(int argc, char *argv[])
{
time_t	lnglocaltime;
FILE 	*fl, *f2;
int 	i, j, ret;
char 	filename[40];
unsigned char name_4016[10];
unsigned char chtemp[1000], out_buf[200];
unsigned char chreturn;
unsigned char buf[10];
pid_t	pid;
short firstdays, enddays;
struct tm tptime;
time_t lngtime;

	#ifdef _DEBUG_BOM_
	debug_printf("\nshanghai soft Ver = %02X",gVersion);
		debug_printf("\nshmt_read_pf4016 start1...");
	#endif
	
	//
	EOD_4020_WhiteList();
	EOD_4026_Cert();
	//
	Init_CRC32_Table();
	
	//4016
	sprintf(name_4016, "4016");
	shmt_read_pf4016(name_4016);
	memcpy(buf_4016,temp_4016,4);
	pf_mobile_ticket_para_init(temp_4016);
	#ifdef _DEBUG_BOM_
		debug_printf("\nshmt_read_pf4016 end1...\n");
	#endif

	mkdir("./prognew", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	mkdir("./progbak", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	mkdir("./cache", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	
	//set signal
	signal(SIGCHLD,SIG_IGN);
	
	pid = fork();
	if(pid < 0)
	{
		perror("The fork failed! may reboot the reader");
		system("reboot");
		exit(1);
	}else if(pid == 0)
	{
		printf("my parent pid is %d\n", getppid());
	}else
	{
		//
		//if(0 == FileisExist("./progbak/", "shanghai", chtemp))
		system("rm ./progbak/*");
		
		waitpid(pid, NULL, 0);
		//if(execl("/mnt/yaffs/shanghai/", "shanghai.sh", NULL) < 0)
		system("./sz.sh &");
			//perror("execl shanghai.sh failure");
		exit(0);
	}
	
	tp_ver = 0x21229;

	//buf[0] = gPPSMode;
	ee_read(2048,2,buf);
	if(lrc_chk2(buf,1) == 0)
		{
				gPPSMode = buf[0];

		}
		else{
			gPPSMode = 0;
		}
	buf[0] = gPPSMode;
	lrc_gen2(buf,1);
	ee_write(2048,2,buf);
		
		

	sem_init(&g_tmrwait,0,1);//  如果 pshared 的值为 0，那么信号量将被进程内的线程共享，并且应该放置在这个进程的所有线程都可见的地址上(如全局变量，或者堆上动态分配的变量)。
	sem_init(&g_samcalwait, 0, 0);
	sem_init(&g_samcalreturn, 0, 0);
	//sem_init(&g_usermainwait, 0, 0);
	sem_init(&g_usermainreturn, 0, 0);
	//sem_init(&g_usermainztewait, 0, 0);//
	
	// 
	ret = pthread_create(&g_pthtmrID, NULL, &pthtmr, NULL);
	//ret = pthread_create(&g_pthtmr2ID, NULL, &pthtmr2, NULL);
	ret = pthread_create(&g_pthusermainID, NULL, &pthusermain, NULL);
	
	//ret = pthread_create(&g_pthsamID, NULL, &pthsamcal, NULL);
	
	//ret = pthread_create(&g_pthusermainzteID, NULL, &pthusermainzte, NULL);
	

 
	//init
  //init_all();
  CurrentLocation = 0x09;//shanghai
  //mcu_powerctrl_clr();usleep(200*1000);
  //mcu_powerctrl_set();usleep(200*1000);
  //mcu1_reset();usleep(200*1000);
  //mcu2_reset();usleep(200*1000);
  //
  prot_zte_init(ZTE_UART_INDEX);
  
  //timer_init();//
  //task_com_enable();
  //sam_prot_enable();
  //soft_timer_enable();//
  //
  shmt_bom_ini();
//	//open comm
	if(argc > 2)
	{
		ret = openQR(argv[2]);
		if(ret < 0) return -1;
	}
	if(argc > 3)
	{
		ret = openBLE(argv[3]);
		if(ret < 0) return -1;
	}
//		ret = open_port("/dev/ttySAC1");
//	if(ret < 0) 
//	{
//		return -1;
//	}
//	zte_comm = ret;
//	speed_set(zte_comm, 115200);
//	//speed_set(zte_comm, 460800);
//	parity_set(zte_comm, 8, 1, 'n');
	
	ret = open_port(argv[1]);
	if(ret < 0) return -1;
	csc_comm = ret;
	speed_set(csc_comm, 57600);
	parity_set(csc_comm, 8, 1, 'n');
	com_serv(csc_comm, NULL, 0, NULL);

	//
	do{
		
	}
	while(1);

	sem_destroy(&g_samcalwait);
	sem_destroy(&g_samcalreturn);
	//sem_destroy(&g_usermainwait);
	sem_destroy(&g_usermainreturn);
	//sem_destroy(&g_usermainztewait);
		
	return 0;
}