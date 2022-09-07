//jt_append_cmd.c

#ifndef _JT_APPEND_CMD_C_
#define _JT_APPEND_CMD_C_
//start of file

/*=====================================================================================
º¯Êý:
¹¦ÄÜ:
=======================================================================================*/
void jt_append_cmd_process(UBYTE *inbuf,UBYTE inbytes,UBYTE *outbuf,UBYTE *outbytes)
{
inbuf = inbuf;
inbytes = inbytes;

outbuf[0] = 0xff;
*outbytes = 1;	
return;	
}	

//end of file
#endif
