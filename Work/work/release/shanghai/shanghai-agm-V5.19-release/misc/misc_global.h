//global.h

#ifndef _MISC_GLOBAL_H_
#define _MISC_GLOBAL_H_
//start of file


#define code const
#define DLL_EXPORT

#define FCLK                 202.80                                    //CPUƵ��
//#define FCLK                 180.0                                     //CPUƵ��
#define PCLK                (FCLK/4)                                   //����Ƶ��
#define PCLK_INT_1000RATE   (UDWORD)((float)PCLK*(float)1000.0)  


#define ISR_BADDR    0x33ffff00

//#define os_lock()    {if(bgEnUCOS) OSSchedLock();}
//#define os_unlock()  {if(bgEnUCOS) OSSchedUnlock();}
//#define os_lock()    {}
//#define os_unlock()  {}

#define os_int_enable()   {OS_EXIT_CRITICAL();}
#define os_int_disable()  {OS_ENTER_CRITICAL();}


extern UBYTE bgHaltFlag;
extern UBYTE bgRespCode;

//����----------------------------------------------------------------------------------------
//void    disable_int(void);
//void    enable_int(void);

//#define disable_int()   {}
//#define enable_int()    {}



//end of file
#endif



