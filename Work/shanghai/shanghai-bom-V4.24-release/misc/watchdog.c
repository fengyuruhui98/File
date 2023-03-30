//watchdog.c

#ifndef _WATCHDOG_C_
#define _WATCHDOG_C_
//start of file

/*=========================================================================
函数：watchdog_init
功能：
===========================================================================*/
void watchdog_init(void)
{
#ifndef _EMU_WIN_
//S3C2410SFR_WTCON = 0x00;    //disable watchdog
S3C2410SFR_WTDAT = WATCHDOG_CNT_VALUE;
S3C2410SFR_WTCNT = WATCHDOG_CNT_VALUE;
S3C2410SFR_WTCON = 0x8021;  //enable watchdog
return;
#endif
}


/*=========================================================================
函数：watchdog
功能：
===========================================================================*/
void watchdog(void)
{
#ifndef _EMU_WIN_
OS_CPU_SR cpu_sr;
//
os_int_disable();
S3C2410SFR_WTCNT = WATCHDOG_CNT_VALUE;
os_int_enable();
//
return;
#endif
}

//end of file
#endif
