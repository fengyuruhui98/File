//cmd_dt.c

#ifndef _CMD_DT_C_
#define _CMD_DT_C_
//start of file

/*====================================================================================
º¯Êý:dt_cmd_process
¹¦ÄÜ:
======================================================================================*/
void dt_cmd_process(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes)
{	
UDWORD start_time;
//UDWORD ptr;

start_time = get_sys_time_cnt();
	
switch((UBYTE)inbuf[0]){
case (UBYTE)0xff:
	outbuf[0] = inbuf[0];outbuf[1] = inbuf[1];
  outbuf[2] = 0;
  udword_to_buf(dwgCmdExeTime,&outbuf[3]);
  *outbytes = 7;
	return;    	
default:
  break;
  }	
bgRespCode = 2;
goto label_rpt_state;

//
//label_err_para:
//bgRespCode = 3;
//goto label_rpt_state;

//
label_rpt_state:
outbuf[0] = inbuf[0];outbuf[1] = inbuf[1];
outbuf[2] = bgRespCode;
*outbytes = 3;		
goto label_exit;

label_exit:
if((UBYTE)inbuf[0] != (UBYTE)0xff){
  dwgCmdExeTime = get_sys_time_cnt()-start_time;
  }
return;
}	




//end of file
#endif



