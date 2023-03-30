//prot_jt.c
//研发部协议(增加包类型)
//编制：邓建华
//时间：20080705

#ifndef _PROT_JT_C_
#define _PROT_JT_C_
//start of file

JT_PROT  gJtProt0;

/*============================================================================
函数：
功能：
==============================================================================*/
UBYTE prot_jt_is_free(JT_PROT *p)
{
if((p->send_state != PROT_JT_SEND_IDLE)
	&&(p->send_state != PROT_JT_SEND_OK)
	&&(p->send_state != PROT_JT_SEND_ERR)) return 0;
if(p->rece_state != PROT_JT_RECE_WAIT_HEAD) return 0;
return 1;	
}	


/*============================================================================
函数：prot_jt_init
功能：
==============================================================================*/
void prot_jt_init(JT_PROT *p,UBYTE sr_node,UBYTE dest_node,UBYTE uart_index)
{
p->send_state = PROT_JT_SEND_IDLE;
p->rece_state = PROT_JT_RECE_WAIT_HEAD;
switch(uart_index){
	case UART0_INDEX:
       p->send_timer_index = TIMER_INDEX_UART0_SEND; 
       p->rece_timer_index = TIMER_INDEX_UART0_RECE; 
       p->discard_timer_index = TIMER_INDEX_DISCARD0;
       break;
	case UART1_INDEX:
       p->send_timer_index = TIMER_INDEX_UART1_SEND; 
       p->rece_timer_index = TIMER_INDEX_UART1_RECE; 
       p->discard_timer_index = TIMER_INDEX_DISCARD1;
       break;
	default:
       p->send_timer_index = TIMER_INDEX_UART2_SEND; 
       p->rece_timer_index = TIMER_INDEX_UART2_RECE; 
       p->discard_timer_index = TIMER_INDEX_DISCARD2;
       break;
  }

p->signal = 0;
p->send_sr_node = sr_node;
p->send_dest_node = dest_node;	
return;	
}	

/*============================================================================
函数：prot_jt_send_block
功能：
==============================================================================*/
void prot_jt_send_block(UBYTE uart_index,JT_PROT *p)
{
UBYTE lrc1,lrc2;
UBYTE i,ch;
//
uart_put_byte_safe(uart_index,PROT_JT_BLOCK_HEAD);

#ifdef _PROT_JT_EXT_ 
uart_put_byte_safe(uart_index,p->send_bytes+3);
#else
uart_put_byte_safe(uart_index,p->send_bytes+2);
#endif

uart_put_byte_safe(uart_index,p->send_sr_node);
uart_put_byte_safe(uart_index,p->send_dest_node);

#ifdef _PROT_JT_EXT_ 
uart_put_byte_safe(uart_index,p->send_type);                  //发送的包类型:命令包或应答包
lrc1=p->send_sr_node^p->send_dest_node^p->send_type^0x33;
lrc2=p->send_sr_node+p->send_dest_node+p->send_type+0x33;
#else
lrc1=p->send_sr_node^p->send_dest_node^0x33;
lrc2=p->send_sr_node+p->send_dest_node+0x33;
#endif

//存储last_cmd
memcpy((char *)p->last_cmd,(char *)p->send_buf,5);
p->last_cmd_bytes = p->send_bytes;
//
for(i=0;i<p->send_bytes;i++){
	ch = p->send_buf[i];
	uart_put_byte_safe(uart_index,ch);
	lrc1=lrc1^ch;
	lrc2=lrc2+ch;
	}
uart_put_byte_safe(uart_index,lrc1);
uart_put_byte_safe(uart_index,lrc2);
//
p->send_state = PROT_JT_SEND_WAIT_ACK0;
p->rece_state = PROT_JT_RECE_WAIT_HEAD;
timer_set(p->send_timer_index,PROT_JT_SEND_WAIT_ACK0_TIMEOUT);
return;
}	

/*============================================================================
函数：prot_jt_send_process
功能：
==============================================================================*/
void prot_jt_send_process(UBYTE uart_index,JT_PROT *p)
{
UBYTE ch;
//	
switch(p->send_state){
	case PROT_JT_SEND_START:
		   prot_jt_send_block(uart_index,p);
		   break;
  case PROT_JT_SEND_WAIT_ACK0:
  	   if(timer_check(p->send_timer_index)) goto label_send_err;
  	   if(uart_rece_is_empty(uart_index)) return;
       ch = uart_get_byte(uart_index);
       if((UBYTE)ch != (UBYTE)PROT_JT_ACK_HEAD) goto label_send_err;  	      	 
       p->send_state = PROT_JT_SEND_WAIT_ACK1;
       timer_set(p->send_timer_index,PROT_JT_SEND_WAIT_ACK1_TIMEOUT);  	   
  	   goto label_wait_ack1;
  case PROT_JT_SEND_WAIT_ACK1:
  	   label_wait_ack1:
  	   if(timer_check(p->send_timer_index)) goto label_send_err;
  	   if(uart_rece_is_empty(uart_index)) return;
       ch = uart_get_byte(uart_index);
       if((UBYTE)ch == (UBYTE)p->send_dest_node) goto label_send_ok;
       if((UBYTE)p->send_dest_node == (UBYTE)PROT_JT_ANYONE_NODE){
       	   if((UBYTE)ch != p->send_sr_node) goto label_send_ok; 
       	   goto label_send_err;
       	   }
       goto label_send_err;      
  default:  //PROT_JT_SEND_IDLE,PROT_JT_SEND_OK,PROT_JT_SEND_ERR:  	
  	   break;
	}
	
return;	

label_send_ok:
p->send_state = PROT_JT_SEND_OK;
return;  	

label_send_err:
p->send_state = PROT_JT_SEND_ERR;
return;  	
}	

/*============================================================================
函数：
功能：
==============================================================================*/
void prot_jt_wait_send(JT_PROT *p)
{
while(1){
   if(p->send_state == PROT_JT_SEND_OK) return;	
   if(p->send_state == PROT_JT_SEND_ERR) return;	
   if(p->send_state == PROT_JT_SEND_IDLE) return;	
//   delay_ms(1);
   } 
}	


/*============================================================================
函数：
功能：
==============================================================================*/
UBYTE prot_jt_is_send_ok(JT_PROT *p)
{
if(p->send_state == PROT_JT_SEND_OK) return 1;
return 0;		
}


/*============================================================================
函数：prot_jt_rece_block
功能：
==============================================================================*/
//#define _DEBUG_JT_RECE_
void prot_jt_rece_process(UBYTE uart_index,JT_PROT *p)
{
UBYTE ch;
UBYTE i,lrc;
#ifdef _DEBUG_JT_RECE_
UBYTE buf[100];
#endif
//	
if(p->rece_state == PROT_JT_RECE_WAIT_PROCESS) return;
if((p->send_state == PROT_JT_SEND_WAIT_ACK0)||
	 (p->send_state == PROT_JT_SEND_WAIT_ACK1)) return; 
//
//检查是否有数据
label_check_rece:
if(uart_rece_is_empty(uart_index)) return;
ch = uart_get_byte(uart_index);
#ifdef _DEBUG_JT_RECE_
os_lock();
sprintf((char *)buf,"[%02X]",(UBYTE)ch);
debug_printf((char *)buf);
os_unlock();
#endif

//
if(timer_check(p->rece_timer_index) && (p->rece_state != PROT_JT_RECE_WAIT_HEAD)){
  #ifdef _DEBUG_JT_RECE_
  debug_printf("\x0d\x0a Err:rece time out");
  #endif	
  goto label_rece_err;
	}
//接受数据块
switch(p->rece_state){
	case PROT_JT_RECE_WAIT_HEAD:
		   if(ch != PROT_JT_BLOCK_HEAD){
		   	 if(ch == PROT_JT_SIGNAL) p->signal = 1;
		   	 goto label_check_rece;
		   	 }
		   p->rece_state = PROT_JT_RECE_WAIT_LEN;
		   break;
	case PROT_JT_RECE_WAIT_LEN:
		   #ifdef _PROT_JT_EXT_ 
		   if((UBYTE)ch <= 3){
		   	 goto label_rece_err; 
		   	 }
		   p->rece_bytes = ch-3;	
		   #else
		   if((UBYTE)ch <= 2){
		   	 goto label_rece_err; 
		   	 }
		   p->rece_bytes = ch-2;			   
		   #endif
		   p->rece_state = PROT_JT_RECE_WAIT_SNODE;
		   break;
	case PROT_JT_RECE_WAIT_SNODE:
		   p->rece_sr_node = ch;
		   p->rece_state = PROT_JT_RECE_WAIT_DNODE;
		   break;
	case PROT_JT_RECE_WAIT_DNODE:
		   p->rece_dest_node = ch;
		   #ifdef _PROT_JT_EXT_ 
		   p->rece_state = PROT_JT_RECE_WAIT_TYPE;
		   #else
		   p->rece_state = PROT_JT_RECE_WAIT_DATA;		
		   p->rece_type = PROT_JT_CMD_TYPE;
		   p->rece_ptr = 0;   
		   #endif
		   break;
	case PROT_JT_RECE_WAIT_TYPE:
		   p->rece_type = ch;
		   p->rece_state = PROT_JT_RECE_WAIT_DATA;
		   p->rece_ptr = 0;
		   break;
  case PROT_JT_RECE_WAIT_DATA:
  	   p->rece_buf[p->rece_ptr] = ch;
  	   p->rece_ptr++;
  	   if((UBYTE)p->rece_ptr == (UBYTE)p->rece_bytes){
  	   	  p->rece_state = PROT_JT_RECE_WAIT_LRC1;
  	   	  } 
  	   break;
  case PROT_JT_RECE_WAIT_LRC1:
		   #ifdef _PROT_JT_EXT_ 
		   lrc = p->rece_sr_node^p->rece_dest_node^p->rece_type^0x33;	   
  	   #else
		   lrc = p->rece_sr_node^p->rece_dest_node^0x33;	     	   
  	   #endif
  	   for(i=0;i<p->rece_bytes;i++) lrc^=p->rece_buf[i];
  	   if((UBYTE)lrc != (UBYTE)ch){
  	   	   #ifdef _DEBUG_JT_RECE_
  	   	   debug_printf("\x0d\x0a Err:lrc1");
  	   	   #endif
           goto label_rece_err;
  	   	   }
  	   p->rece_state = PROT_JT_RECE_WAIT_LRC2;
  	   break;
  case PROT_JT_RECE_WAIT_LRC2:	   
		   #ifdef _PROT_JT_EXT_ 
  	   lrc = p->rece_sr_node+p->rece_dest_node+p->rece_type+0x33;	   	   
  	   #else
  	   lrc = p->rece_sr_node+p->rece_dest_node+0x33;	   	   
       #endif
  	   for(i=0;i<p->rece_bytes;i++) lrc += p->rece_buf[i];
  	   if((UBYTE)lrc != (UBYTE)ch){
  	   	  #ifdef _DEBUG_JT_RECE_
  	   	  debug_printf("\x0d\x0a Err:lrc2");
  	   	  #endif
  	   	  goto label_rece_err;
  	   	  }
  	   //是否发给本机的
  	   if((UBYTE)p->rece_sr_node == (UBYTE)p->send_sr_node){
  	   	   #ifdef _DEBUG_JT_RECE_
  	   	   debug_printf("\x0d\x0a Err:odd node");
  	   	   #endif
  	   	   goto label_rece_err;  	   
  	   	   }
  	   if(((UBYTE)p->rece_dest_node != (UBYTE)p->send_sr_node)    
  	   	 && ((UBYTE)p->rece_dest_node != (UBYTE)PROT_JT_ANYONE_NODE)){
  	   	   #ifdef _DEBUG_JT_RECE_
  	   	   os_lock();
  	   	   sprintf((char *)buf,"\nErr: dest_node=%02X",(UBYTE)p->rece_dest_node);
  	   	   debug_printf((char *)buf);
  	   	   os_unlock();
  	   	   #endif
  	   	 	 goto label_rece_err; 
  	   	 	 }
  	   //ACK
       #ifdef _DEBUG_JT_RECE_
       debug_printf("\x0d\x0a Err:ok,send ack");
       #endif	
   	   uart_put_byte_safe(uart_index,PROT_JT_ACK_HEAD);
  	   uart_put_byte_safe(uart_index,p->send_sr_node);
       //
  	   p->rece_state = PROT_JT_RECE_WAIT_PROCESS;
  	   return;
  default:
  	   goto label_rece_err; 
  }

//
goto label_wait_next_char;

label_rece_err:
p->rece_state = PROT_JT_RECE_WAIT_HEAD;
goto label_check_rece;

//等待下一个字符
label_wait_next_char:
timer_set(p->rece_timer_index,PROT_JT_RECE_WAIT_CHAR_TIMEOUT);
goto label_check_rece;
}	

/*============================================================================
函数：
功能：
==============================================================================*/
UBYTE prot_jt_is_wait_process(JT_PROT *p)
{
if(p->rece_state == PROT_JT_RECE_WAIT_PROCESS) return 1;
return 0;		
}


/*============================================================================
函数：
功能：
==============================================================================*/
void prot_jt_processed(JT_PROT *p)
{
p->rece_state = PROT_JT_RECE_WAIT_HEAD;
return;		
}

/*============================================================================
函数：
功能：
==============================================================================*/
UBYTE prot_jt_rece_is_cmd(JT_PROT *p)
{
if((UBYTE)p->rece_type == (UBYTE)PROT_JT_CMD_TYPE) return 1;
return 0;
}


/*============================================================================
函数：
功能：
==============================================================================*/
void prot_jt_cmd_process(JT_PROT *p)
{
UBYTE inbuf[257],outbuf[257],inbytes;
//UBYTE outbytes;
UWORD outbytes;
UBYTE dest_node;
	
if(!prot_jt_is_wait_process(p)) return;
//应答包:等待其他进程处理
#ifdef _PROT_JT_EXT_  
if(!prot_jt_rece_is_cmd(p)){
	if(!p->discard){
	   timer_set(p->discard_timer_index,500);
	   p->discard = 1;
	   }
	else{
		 if(timer_check(p->discard_timer_index)){
		 	  prot_jt_processed(p);   //抛弃处理
		 	  p->discard = 0;
		 	  }
		 } 
  return;
  }
#endif

	 
//命令包
dest_node = p->rece_sr_node;
inbytes = p->rece_bytes;
memcpy((char *)inbuf,(char *)p->rece_buf,inbytes);
//
//jt_cmd_process(inbuf,inbytes,outbuf,&outbytes);
class_general(inbuf,inbytes,outbuf,&outbytes);
//
prot_jt_processed(p);
p->send_dest_node = dest_node;
p->send_bytes = outbytes;
p->send_type = PROT_JT_ACK_TYPE;
memcpy((char *)p->send_buf,(char *)outbuf,outbytes);
p->send_state = PROT_JT_SEND_START; 
return;
}	



//end of file
#endif

