//misc_global.c

#ifndef _MISC_GLOBAL_C_
#define _MISC_GLOBAL_C_
//start of file

#define MAX_INT_LEVEL   512

UDWORD dwgIntMask;
////
UDWORD dwgIntMskFifo[512+1];
UBYTE  bgIntMskPtr=0;

UBYTE bgHaltFlag=0;
UBYTE bgRespCode;

/*======================================================================
������disbale_int
���ܣ�
========================================================================*/
//void  disable_int(void)
//{
//UDWORD tint;	
////
//tint = S3C2410SFR_INTMSK;
//S3C2410SFR_INTMSK = 0xffffffffL;
//if(bgIntMskPtr < MAX_INT_LEVEL){ 
//  dwgIntMskFifo[bgIntMskPtr++] = tint;
//  }
//  
//return;
//} 


/*======================================================================
������
���ܣ�
========================================================================*/
//void  enable_int(void)
//{
//S3C2410SFR_INTMSK = dwgIntMskFifo[--bgIntMskPtr];
//return;
//}



//end of file
#endif


