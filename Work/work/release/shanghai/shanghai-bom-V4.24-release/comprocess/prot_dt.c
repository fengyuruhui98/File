//prot_dt.c
//地铁通讯协议
//编制:邓建华
//时间:20090427

#ifndef _PROT_DT_C_
#define _PROT_DT_C_
//start of file

DT_PROT gDtProt0;

/*============================================================================
函数：prot_dt_init
功能：
==============================================================================*/
void prot_dt_init(DT_PROT *p,UBYTE uart_index)
{
p->send_state = PROT_DT_SEND_OK;
p->rece_state = PROT_DT_RECE_WAIT_STX;
switch(uart_index){
	case UART0_INDEX:
       p->send_timer_index = TIMER_INDEX_UART0_SEND; 
       p->rece_timer_index = TIMER_INDEX_UART0_RECE; 
       break;
	case UART1_INDEX:
       p->send_timer_index = TIMER_INDEX_UART1_SEND; 
       p->rece_timer_index = TIMER_INDEX_UART1_RECE; 
       break;
	default:
       p->send_timer_index = TIMER_INDEX_UART2_SEND; 
       p->rece_timer_index = TIMER_INDEX_UART2_RECE; 
       break;
  }

p->send_bytes = 0;
p->rece_bytes = 0;
return;	
}	

/*============================================================================
函数：uart_put_byte_prot_dt
功能：
==============================================================================*/
void uart_put_byte_prot_dt(UBYTE uart_index,UBYTE inbyte)
{
switch(inbyte){
  case PROT_DT_STX:
  case PROT_DT_ETX:
  case PROT_DT_DLE:		
  	   uart_put_byte_safe(uart_index,PROT_DT_DLE);
  	   break;
  default:
  	   break;	   
  }	
uart_put_byte_safe(uart_index,inbyte);
//  
return;	
}	


/*============================================================================
函数：prot_jt_send_block
功能：
==============================================================================*/
void prot_dt_send_block(UBYTE uart_index,DT_PROT *p)
{
UBYTE lrc;
UBYTE i,ch;
//
uart_put_byte_safe(uart_index,PROT_DT_STX);
//
uart_put_byte_prot_dt(uart_index,p->send_bytes);
lrc=p->send_bytes;
//
for(i=0;i<p->send_bytes;i++){
	if(i !=1) ch = p->send_buf[i];
	else ch = p->send_retry;	
	uart_put_byte_prot_dt(uart_index,ch);
	lrc=lrc+ch;
	}
uart_put_byte_prot_dt(uart_index,lrc);	
uart_put_byte_safe(uart_index,PROT_DT_ETX);
//
p->send_retry++;
//
p->send_state = PROT_DT_SEND_WAIT_ACK;
p->rece_state = PROT_DT_RECE_WAIT_STX;
timer_set(p->send_timer_index,PROT_DT_WAIT_ACK_TIMEOUT);
return;
}	


/*============================================================================
函数：prot_dt_send_process
功能：
==============================================================================*/
void prot_dt_send_process(UBYTE uart_index,DT_PROT *p)
{
//	
switch(p->send_state){
	case PROT_DT_SEND_START:
		   p->send_retry = 0;
		   prot_dt_send_block(uart_index,p);
		   if(PROT_DT_MAX_RETRY == 1){
		   	  p->send_state = PROT_DT_SEND_OK;
		   	  }
		   break;
  case PROT_DT_SEND_WAIT_ACK:
  	   if(timer_check(p->send_timer_index)){
  	   	 if(p->send_retry >= PROT_DT_MAX_RETRY) goto label_send_err;
  	   	 goto label_send_err;
  	   	 }
  	   prot_dt_send_block(uart_index,p);	 
  	   break;	  
  default:  
  	   break;
	}
	
return;	

label_send_err:
p->send_state = PROT_DT_SEND_ERR;
return;  	
}	


/*============================================================================
函数：prot_dt_rece_block
功能：
==============================================================================*/
void prot_dt_rece_process(UBYTE uart_index,DT_PROT *p)
{
UBYTE ch;
UBYTE i,lrc;
//	
if(p->rece_state == PROT_DT_RECE_WAIT_PROCESS) return;
if(timer_check(p->rece_timer_index) && (p->rece_state != PROT_DT_RECE_WAIT_STX)){
  goto label_rece_err;
	}
//
//检查是否有数据
label_check_rece:
if(uart_rece_is_empty(uart_index)) return;
ch = uart_get_byte(uart_index);
//uart_put_byte(uart_index,ch);
//
//接受数据块
switch(p->rece_state){
	case PROT_DT_RECE_WAIT_STX:
		   if(ch != PROT_DT_STX){
		   	 goto label_check_rece;
		   	 }
		   p->rece_state = PROT_DT_RECE_WAIT_LEN_DLE;
       p->send_state = PROT_DT_SEND_OK;
		   break;
	case PROT_DT_RECE_WAIT_LEN_DLE:
		   if(ch == PROT_DT_DLE){
		      p->rece_state = PROT_DT_RECE_WAIT_LEN;
		      }
		   else{
		   	  label_rece_len:
		   	  p->rece_bytes = ch;		   	  
		   	  p->rece_state = PROT_DT_RECE_WAIT_DATA_DLE;
		   	  p->rece_ptr = 0;
		   	  }   
		   break;
	case PROT_DT_RECE_WAIT_LEN:
		   goto label_rece_len;
	case PROT_DT_RECE_WAIT_DATA_DLE:
		   if(ch == PROT_DT_DLE){
		      p->rece_state = PROT_DT_RECE_WAIT_DATA;
		      }
		   else{
		   	  label_rece_data:
		   	  p->rece_buf[p->rece_ptr] = ch;		   	  
		   	  p->rece_state = PROT_DT_RECE_WAIT_DATA_DLE;
		   	  p->rece_ptr++;
		   	  if(p->rece_ptr == p->rece_bytes){
		   	  	 p->rece_state = PROT_DT_RECE_WAIT_LRC_DLE;
		   	  	 }
		   	  }   
		   break;
	case PROT_DT_RECE_WAIT_DATA:
		   goto label_rece_data;	
	case PROT_DT_RECE_WAIT_LRC_DLE:
		   if(ch == PROT_DT_DLE){
		      p->rece_state = PROT_DT_RECE_WAIT_LRC;
		      }
		   else{
		   	  label_rece_lrc:
		   	  lrc = p->rece_bytes;
		   	  for(i=0;i<p->rece_bytes;i++) lrc += p->rece_buf[i];
		   	  if((UBYTE)lrc != (UBYTE)ch) goto label_rece_err;
		   	  p->rece_state = PROT_DT_RECE_WAIT_ETX;
		   	  }   
		   break;		
  case PROT_DT_RECE_WAIT_LRC:
		   goto label_rece_lrc;	
  case PROT_DT_RECE_WAIT_ETX:
  	   if(ch == PROT_DT_ETX){
		      p->rece_state = PROT_DT_RECE_WAIT_PROCESS;
		      return;
		      }
  	   goto label_rece_err;
  default:
  	   goto label_rece_err; 
  }

//
goto label_wait_next_char;

label_rece_err:
p->rece_state = PROT_DT_RECE_WAIT_STX;
goto label_check_rece;

//等待下一个字符
label_wait_next_char:
timer_set(p->rece_timer_index,PROT_DT_RECE_WAIT_CHAR_TIMEOUT);
goto label_check_rece;
}	

/*============================================================================
函数：
功能：
==============================================================================*/
UBYTE prot_dt_is_wait_process(DT_PROT *p)
{
if(p->rece_state == PROT_DT_RECE_WAIT_PROCESS) return 1;
return 0;		
}


/*============================================================================
函数：
功能：
==============================================================================*/
void prot_dt_processed(DT_PROT *p)
{
p->rece_state = PROT_DT_RECE_WAIT_STX;
return;		
}



/*============================================================================
函数：
功能：
==============================================================================*/
void prot_dt_cmd_process(DT_PROT *p)
{
UWORD outbytes;	
UDWORD start_time;
OS_CPU_SR  cpu_sr;
	
if(!prot_dt_is_wait_process(p)) return;
//是否重发包
if(p->rece_buf[1] != 0){
	if(p->rece_buf[0] != p->last_rece_buf[0]) goto label_new;
	if(p->rece_bytes != p->last_rece_bytes) goto label_new;
	if(p->rece_bytes < 2){
     prot_dt_processed(p);
     return;
		 }	
	if(memcmp(&p->rece_buf[2],&p->last_rece_buf[2],p->last_rece_bytes-2) != 0) goto label_new;
	//重发前次应答	
	p->send_buf[1] = p->rece_buf[1];
	p->send_state = PROT_DT_SEND_START;
	return;
	}

label_new:
//dt_cmd_process(p->rece_buf,p->rece_bytes,p->send_buf,&p->send_bytes);
start_time = get_sys_time_cnt();
shmt_bom(p->rece_buf,p->rece_bytes,p->send_buf,&outbytes);
dwgCmdExeTime = get_sys_time_cnt()-start_time;
p->send_bytes = outbytes;
//
prot_dt_processed(p);
p->send_state = PROT_DT_SEND_START; 
p->last_rece_bytes = p->rece_bytes;
memcpy(p->last_rece_buf,p->rece_buf,p->rece_bytes);

os_int_disable();
//disable_t4();
prot_dt_send_process(UART_DT_CMD_INDEX,p);
//enable_t4();
os_int_enable();

//判断是否需要自动abort
//20100210add
auto_abort();

return;
}	

//end of file
#endif