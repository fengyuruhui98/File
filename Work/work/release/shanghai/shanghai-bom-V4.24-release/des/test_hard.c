#ifndef _TEST_HARD_C_
#define _TEST_HARD_C_

//#define	DEBUG_PRINT

#ifdef DEBUG_PRINT
#define _DEBUG_TEST_HARD_
#endif

/*==============================================================================
������reader_test_eeprom
���ܣ�����eeprom
================================================================================*/
char reader_test_eeprom(void)
{
	int ret;
	int i, j;
	char inbuf[1000], outbuf[1000];
	int eepromsize, eepromstep;
	
		eepromsize = 2048;
		eepromstep = 32;
		#ifdef DEBUG_PRINT
			printf("\nEEPROM���Կ�ʼ");
		#endif
		for(j = 0; j < (eepromsize / eepromstep); j++)
		{
			printf(".");
			memset(outbuf, 0, sizeof(outbuf));
			ret = ee_read(j * eepromstep, eepromstep, outbuf);
			if (ret)
			{
					#ifdef DEBUG_PRINT
								printf("\nErr:��EEPROMʧ��,add=%x!", j * eepromstep);
					#endif
				return 0x10;
	  		}
	 
			for(i = 0; i < eepromstep; i++)  inbuf[i] = (i % 256);
			ret = ee_write(j * eepromstep, eepromstep, inbuf);
			if (ret)
			{
					#ifdef DEBUG_PRINT
								printf("\nErr:дEEPROMʧ��,add=%x!", j * eepromstep);
					#endif
	  			return 0x20;
	  		}
	  
			memset(outbuf, 0, sizeof(outbuf));
			ret = ee_read(j * eepromstep, eepromstep, outbuf);
			if(ret)
			{
					#ifdef DEBUG_PRINT
						printf("\nErr:��EEPROMʧ��,add=%x!", j * eepromstep);
					#endif
	  			return 0x10;
	  		}
	
			if (memcmp(inbuf, outbuf, eepromstep) != 0)  
				printf("\n Err:EEPROM����ʧ��!");
		}
	
		#ifdef DEBUG_PRINT
			printf("\nOK:EEPROM���Գɹ�!");
		#endif
		return CE_OK;	
}
/*==============================================================================
������reader_test_rtc
���ܣ�����rtc
================================================================================*/
char reader_test_rtc(char write_control, unsigned char *time_bcd, unsigned char *out_len)
{
		int ret;
		char inbuf[100];
		
			if(write_control)
			{
				memcpy(inbuf, time_bcd, 6);
				ret = rtc_wr_time(inbuf);
				if (ret)
				{
					#ifdef DEBUG_PRINT
								printf("\nErr:дʱ��ʧ��!");
					#endif
			  		return  0xff;
			  	}
				#ifdef DEBUG_PRINT
						printf("\nOK:дʱ�ӳɹ� %02x%02x%02x %02x:%02x:%02x.",inbuf[0],inbuf[1],inbuf[2],inbuf[3],inbuf[4],inbuf[5]);
				#endif
			}
			memset(inbuf,0,sizeof(inbuf));
			ret = rtc_rd_time(inbuf);
			if (ret)
			{
				#ifdef DEBUG_PRINT
						printf("\nErr:��ʱ��ʧ��!");
				#endif
				return 0xff;
			}
			#ifdef DEBUG_PRINT
				printf("\nOK:��ʱ�ӳɹ� %02x-%02x-%02x %02x:%02x:%02x.",inbuf[0],inbuf[1],inbuf[2],inbuf[3],inbuf[4],inbuf[5]);
			#endif
			if(write_control)
			{
				if(memcmp(inbuf, time_bcd, 6) == 0)
					return CE_OK;	
				else
					return 0xff;
			}
			*out_len = 7;
			//time_bcd[0] = 0x20;
			memcpy(time_bcd, inbuf, 6);
			return CE_OK;
}
/*==============================================================================
������reader_test_littlesam
���ܣ�����PSAM
================================================================================*/
char reader_test_littlesam(char channel, unsigned char *out_buf, unsigned char *out_len)
{
		int ret;
		int i;
		int samindex;
		char outbuf[100];
		unsigned char outbytes;
		
			*out_len = 1;
			samindex = channel;
			//if(samindex != 0x07)
			//{
				sam_set(samindex, SAM_ETU_93, 4);
			//}else
			//	{
			//		sam_set(bgSMobilepsamIndex,SAM_ETU_372,16);
			//	}
		
			ret = sam_select(samindex);
			if (ret)
			{
				#ifdef DEBUG_PRINT
						printf("\nErr:ѡ��SAM%02d����ʧ��!", samindex);
				#endif
				out_buf[0] = 0x10;
		  		return 0xff;
		  	}
		
			ret = sam_atr(samindex, outbuf, &outbytes);
			if(ret)
			{
				#ifdef DEBUG_PRINT
						printf("\nErr:SAM%02d��λʧ��!",samindex);
				#endif
				out_buf[0] = 0x11;
		  		return 0xff;
		  	}
		  
			#ifdef DEBUG_PRINT
				printf("\nSAM%02d return :", samindex);  
				for(i = 0; i < outbytes; i++)  printf("%02x ", (unsigned char)outbuf[i]); 
			#endif
			if (outbuf[0] == 0x3b) 
			{
				#ifdef DEBUG_PRINT
						printf("\nOK:SAM%02d���Գɹ�!", samindex);
				#endif
		  	}else
		  	{
				#ifdef DEBUG_PRINT
				  		printf("\nErr:SAM%02d����ʧ��!", samindex);
				#endif
		  		out_buf[0] = 0x12;
				return 0xff;
			}
			*out_len = 0;
			return CE_OK;
}	
/*==============================================================================
������reader_test_antenna
���ܣ�����antenna
================================================================================*/
char reader_test_antenna(unsigned char *in_buf, unsigned char *out_buf, unsigned char *out_len)
{
		int ret;
		int i,j;
		int try_times;
		char inbuf[100], outbuf[100], chret, atslen;
		char channel;
		UBYTE CardSnr[16];
		UBYTE Key_test[16][6];
		UBYTE buf[20];
		UWORD	apduLen;
		
		#ifdef DEBUG_PRINT
			printf("\nantenna channel=%02X test-->",in_buf[0]);
			//if(channel)  printf("channel 1.");
			//else printf("channel 0.");
		#endif
			channel = in_buf[0];
			*out_len = 1;
			//rf_select(channel);
			rf_select(in_buf[0]);
			//
			//rf_reset();
			mcml_pwr_on();
			try_times = 3;
			set_timeout(5000);
			//
			//mcml_pwr_on();
			for(i = 0; i < try_times; i++)
			{
				memset(out_buf,0,sizeof(out_buf));
				//request
				ret = mcml_request2(PICC_REQSTD, outbuf);
				if(ret)
				{
					#ifdef DEBUG_PRINT
						printf("      Err: request! %d return %d", i, ret);
					#endif
					out_buf[0] = 0x01;
					set_timeout(3000);
		  			continue;//return;
		  		}
			
				#ifdef DEBUG_PRINT
						printf("request! %d outbuf=%02X %02X",i,outbuf[0],outbuf[1]);
				#endif
				//printf("Err:anticoll!");
				//according to the request result
				if( (outbuf[0] &0xC0) == 0x40 )
				{//ultralight
					ret = UL_Anticoll_Select(CardSnr);
					if(ret)
					{
						#ifdef	DEBUG_PRINT
							printf("Err:Ultralight anticoll failure\n");
						#endif
						set_timeout(3000);
						continue;
					}
					if(UL_Page_Read(0, outbuf) != 0)
					{
						#ifdef	DEBUG_PRINT
							printf("Err:Ultralight read Page 0~3 failure\n");
						#endif
						set_timeout(3000);
						continue;
					}
					if(UL_Page_Read(4, outbuf) != 0)
					{
						#ifdef	DEBUG_PRINT
							printf("Err:Ultralight read Page 4~7 failure\n");
						#endif
						set_timeout(3000);
						continue;
					}
					if(UL_Page_Read(8, outbuf) != 0)
					{
						#ifdef	DEBUG_PRINT
							printf("Err:Ultralight read Page 8~11 failure\n");
						#endif
						set_timeout(3000);
						continue;
					}
					if(UL_Page_Read(12, outbuf) != 0)
					{
						#ifdef	DEBUG_PRINT
							printf("Err:Ultralight read Page 12~15 failure\n");
						#endif
						set_timeout(3000);
						continue;
					}
				}
				//if((outbuf[0] == 0x44) && (outbuf[1] == 0x00))
				else if((memcmp(outbuf,"\x03\x00",2) == 0) || (memcmp(outbuf,"\x00\x36",2) == 0))
				{//single ticket card
					out_buf[1] = 0x01; //����Ʊ
				}else if((memcmp(outbuf,"\x04\x00",2) == 0) || (memcmp(outbuf,"\x08\x00",2) == 0) || (memcmp(outbuf,"\x02\x00",2) == 0)) //if((outbuf[0] == 0x04) && (outbuf[1] == 0x00))
				{//M1
					CardSnr[6]=outbuf[0];
					CardSnr[7]=outbuf[1];
					ret = mcml_anticoll(CardSnr);
					if (ret)
					{
						#ifdef DEBUG_PRINT
							printf("       Err:anticoll_M1!");
						#endif
			  			out_buf[0] = 0x02;
			  			continue; //return;
			  		}
		
					#ifdef DEBUG_PRINT
						printf(" anticoll_M1");
					#endif
			  		memcpy(inbuf, outbuf, 5);
					ret = mcml_select(CardSnr, outbuf);
					if (ret)
					{
						#ifdef DEBUG_PRINT
							printf("      Err:select_M1!");
						#endif
			  			out_buf[0] = 0x03;
			  			continue; //return;
			  		}
					#ifdef DEBUG_PRINT
								printf(" select_M1 outbuf[0]=%02X ",outbuf[0]);
					#endif
					out_buf[1] = 0x02; //M1
					//M1 end
								//ats
					//memcpy(Key_test,"\xfc\x61\xcb\xb5\x37\x51",6);   //��ʽ
					//ret=mcmh_read_simple(1,buf,1,0);
					//if(ret!=0)
					//CPU
					if(1)
					{ 
						//memcpy(Key_test,"\x70\xa4\xf4\x82\x01\x6d",6);   //����
						//ret=mcmh_read_simple(1,buf,0,0);
						//if(ret!=0)
						if(outbuf[0] != 0x53 )
						{
							mcml_pwr_off();
							set_timeout(5000);
							mcml_pwr_on();
							
							out_buf[1] = 0x03; //CPU
							
							ret = mcml_request2(PICC_REQSTD, outbuf);
							if(ret)
							{
							#ifdef DEBUG_PRINT
											printf("Err:request__CPU!");
							#endif
			  				out_buf[0] = 0x01;
			  				continue; //return;
							}	
						CardSnr[6]=outbuf[0];
						CardSnr[7]=outbuf[1];
						ret = mcml_anticoll(CardSnr);
						if (ret)
						{
							#ifdef DEBUG_PRINT
											printf("       Err:anticoll_CPU!");
							#endif
				  			out_buf[0] = 0x02;
				  			continue; //return;
			  			}
		
						#ifdef DEBUG_PRINT
									printf(" anticoll_CPU");
						#endif
			  			memcpy(inbuf, outbuf, 5);
						ret = mcml_select(CardSnr, outbuf);
						if (ret)
						{
							#ifdef DEBUG_PRINT
												printf("      Err:select_CPU!");
							#endif
				  			out_buf[0] = 0x03;
				  			continue; //return;
			  			}
					#ifdef DEBUG_PRINT
								printf(" select_CPU");
					#endif
						ret = mifpro_ats(0, inbuf, &atslen);
					if(ret != 0)
					{
						#ifdef DEBUG_PRINT
										printf("      Err:ats_CPU!");
						#endif
			 			out_buf[0] = 0x04;
			  		continue;
					}
					#ifdef DEBUG_PRINT
								printf(" ats_CPU in_buf[1]=%02X ",inbuf[1]);
					#endif
					//pps
					
					if(in_buf[1])
					{
						#ifdef DEBUG_PRINT
								printf("  PPS_CPU in_buf=%02X %02X %02X ",inbuf[0],inbuf[1],inbuf[2]);
						#endif
						if((inbuf[0] > 1) && (inbuf[1]&0x10))
						{
//							if((inbuf[2]&0x44) == 0x44) 
//							{
//								#ifdef DEBUG_PRINT
//										printf("      848PPS_CPU!");
//								#endif
//								ret = mifpro_pps(RF_PPS1_848K,inbuf);
//								if(ret)
//								{
//									gDebugStep = 0x8801;
//									out_buf[0] = 0x05;
//			  					continue;
//								}
//								ret = mifpro_set_speed(RF_SPEED_848K,RF_SPEED_848K);
//								if(ret)
//								{
//									gDebugStep = 0x8802;
//									out_buf[0] = 0x06;
//			  					continue;
//								}
//								goto next;
//							}else
							if((inbuf[2]&0x22) == 0x22) 
							{
								#ifdef DEBUG_PRINT
										printf("      424PPS_CPU!");
								#endif
								ret = mifpro_pps(RF_PPS1_424K,inbuf);
								if(ret)
								{
									#ifdef DEBUG_PRINT
										printf("    Err:424PPS!");
									#endif
									mcml_pwr_off();
									gDebugStep = 0x8803;
									out_buf[0] = 0x05;
			  					continue;
			  				}
								ret = mifpro_set_speed(RF_SPEED_424K,RF_SPEED_424K);
								if(ret)
								{
									#ifdef DEBUG_PRINT
									printf("    Err:424speed!");
									#endif
									mcml_pwr_off();
									gDebugStep = 0x8804;
									out_buf[0] = 0x06;
			  					continue;
								}
								#ifdef DEBUG_PRINT
										printf("      424PPS_CPU_end!");
								#endif
								goto next;
							}		
							else if((inbuf[2]&0x11) == 0x11) 
							{
								#ifdef DEBUG_PRINT
										printf("      212PPS_CPU!");
								#endif
								ret = mifpro_pps(RF_PPS1_212K,inbuf);
								if(ret)
								{
									printf("    212Err:PPS!");
									mcml_pwr_off();
									gDebugStep = 0x8805;
									out_buf[0] = 0x05;
			  					continue;
								}
								ret = mifpro_set_speed(RF_SPEED_212K,RF_SPEED_212K);
								if(ret)
								{
									printf("    212Err:speed!");
									mcml_pwr_off();
									gDebugStep = 0x8806;
									out_buf[0] = 0x06;
			  					continue;
								}
								#ifdef DEBUG_PRINT
										printf("      212PPS_CPU_end!");
								#endif
								goto next;
							}
					  }
					}
				
				next:		
					//pps end
					//select 3f 00
					memcpy(inbuf, "\x00\xa4\x00\x00\x02\x3f\x00", 7);
					//for(j = 0;j < 3;j++)
					//{
							ret = mifpro_icmd(inbuf, 7, outbuf, &apduLen);
							if(ret != 0)
							{
								#ifdef	DEBUG_PRINT
												printf("     Err:select 3f 00\n");
								#endif
								mcml_pwr_off();
								out_buf[0] = 0x07;
					  		continue;
							}
					//}
					//if(j >= 3 && ret !=0)
					//{
					//	printf("return j=%d\n",j);
					//	mcml_pwr_off();
					//	return 0xff;
					//}
					#ifdef DEBUG_PRINT
								printf(" select 3f00\n");
					#endif
						}
					}			
				}else
				{
						out_buf[1] = 0x04; //δ֪����
						ret = mcml_request2(PICC_REQALL, outbuf);
							if(ret)
								{
									#ifdef DEBUG_PRINT
													printf("Err:request__M1!");
									#endif
					  			out_buf[0] = 0x01;
					  			continue; //return;
								}
							CardSnr[6]=outbuf[0];
							CardSnr[7]=outbuf[1];
							ret = mcml_anticoll(CardSnr);
							if (ret)
							{
								#ifdef DEBUG_PRINT
												printf("       Err:anticoll_M1!");
								#endif
					  			out_buf[0] = 0x02;
					  			continue; //return;
					  		}
				
								#ifdef DEBUG_PRINT
											printf(" anticoll_M1");
								#endif
					  		memcpy(inbuf, outbuf, 5);
							ret = mcml_select(CardSnr, outbuf);
							if (ret)
							{
									#ifdef DEBUG_PRINT
													printf("      Err:select_M1!");
									#endif
					  			out_buf[0] = 0x03;
					  			continue; //return;
					  		}
							#ifdef DEBUG_PRINT
										printf(" select_M1 outbuf[0]=%02X ",outbuf[0]);
							#endif
				}
				mcml_pwr_off();
				delay_ms(5);
		  		break;
		 	} 
			if(i >= try_times)
			{
				#ifdef DEBUG_PRINT
					printf("return i=%d\n",i);
				#endif
				mcml_pwr_off();
				return 0xff;
			}
			
			#ifdef DEBUG_PRINT
				printf("-- ok");
			#endif
			out_buf[0] = 0;
			return CE_OK;
}	
/*==============================================================================
������
���ܣ�����2.4G
================================================================================*/
char reader_test_24G(unsigned char *in_buf, unsigned char *out_buf, unsigned char *out_len)
{
	UBYTE *rece_buf;
	UBYTE buf[257];
	int i,ret;
	int count = 5;
	*out_len = 1;
	  //Ѱ��,1ms
	  rece_buf = gZteProt0.rece_buf;
	  //printf("reader test 24G\n");
	  for(i=0;i<count;i++)
	  {
	  		//printf("1111111\n");
				gDebugStep=0x1104;
			   //memcpy(buf,"\xa2\x31\x01\x2c",4);
			   memcpy(buf,"\xa2\x31\x00\x28",4); //\xa2\x31\x00\x00
		     prot_zte_send_block(buf,4);
		     //timer_set(gZteProt0.send_timer_index,20);
		     prot_zte_rece_process(50);	//20
		     
					//printf("A2 31 01 2C\n");
					//out_buf[0] = 0x05;
			   if(gZteProt0.send_state == ZTE_SEND_TIMEOUT)
			   {
			   		//printf("333333\n");
			   		out_buf[0] = 0x01;
			   		continue;
			   }
			   //printf("2222222\n");
			 		gDebugStep=0x1105;
			 	 if(gZteProt0.rece_bytes < 2) continue;
			   if(memcmp(rece_buf,"\x0\x0",2) != 0){
			   	  if(memcmp(rece_buf,"\xa0\x01",2) == 0){
			   	  	memcpy(buf,"\xa2\x32\x00\x00",4);
		          prot_zte_send_block(buf,4);
		          prot_zte_rece_process(50);
		          if(gZteProt0.send_state == ZTE_SEND_TIMEOUT)
		          	out_buf[0] = 0x02;
			   				continue;
		          //zte_set_wait(100);
		          #ifdef DEBUG_PRINT
		          debug_printf("\x0d\x0a �Ͽ�����");
		          #endif     	
			        out_buf[0] = 0x03;
			   			continue;
			   	  	} 
			   	  out_buf[0] = 0x04;
			   		continue;
			   	  }
			   #ifdef DEBUG_PRINT
		     debug_printf("\x0d\x0a OK:link,next sel adf");
		     #endif  
		     	memcpy(buf,"\xa2\x32\x0\x0",4);
  				prot_zte_send_block(buf,4); 
					prot_zte_rece_process(30); 
					break;
		  }  
		  if(i >= count)
			{
				#ifdef DEBUG_PRINT
					printf("return i=%d\n",i);
				#endif
					memcpy(buf,"\xa2\x32\x0\x0",4);
  				prot_zte_send_block(buf,4); 
					prot_zte_rece_process(30);
				return 0xff;
			}
			#ifdef DEBUG_PRINT
				printf("-- ok!!!");
			#endif
			out_buf[0] = 0;
			return CE_OK;
}


/*==============================================================================
������
���ܣ�����2.4G
return: 1:��2.4Gģ��
        0:û��2.4Gģ��
================================================================================*/
//#define _DEBUG_READER_TEST_IF_24G_EXIST_
char reader_test_if_24G_exist(void)
{
	UBYTE *rece_buf;
	UBYTE buf[257];
	int i,ret;
  UBYTE debug_buf[64];

  //Ѱ��,1ms
  rece_buf = gZteProt0.rece_buf;

	gDebugStep=0x1104;
   //memcpy(buf,"\xa2\x31\x01\x2c",4);
   memcpy(buf,"\xa2\x31\x00\x28",4); //\xa2\x31\x00\x00
   prot_zte_send_block(buf,4);
   //timer_set(gZteProt0.send_timer_index,20);
   prot_zte_rece_process(50);	//20
   
		//printf("A2 31 01 2C\n");
		//out_buf[0] = 0x05;
   if(gZteProt0.send_state == ZTE_SEND_TIMEOUT)
   {
   		return 0;
   }
   //printf("2222222\n");
 		gDebugStep=0x1105;
		#ifdef _DEBUG_READER_TEST_IF_24G_EXIST_
		uword_to_buf(gDebugStep,debug_buf);
		debug_buf[2] = zte_rece_bytes;
		memcpy(&debug_buf[3],rece_buf,zte_rece_bytes);
		ReaderResponse(csc_comm,0x00,0xf0,debug_buf,zte_rece_bytes+3);
		#endif
		
 	 if(gZteProt0.rece_bytes < 2) return 0;
   if(memcmp(rece_buf,"\x0\x0",2) != 0){
   	  if(memcmp(rece_buf,"\xa0\x06",2) == 0){
   	  	memcpy(buf,"\xa2\x32\x00\x00",4);
        prot_zte_send_block(buf,4);
        prot_zte_rece_process(50);
        //zte_set_wait(100);
        #ifdef DEBUG_PRINT
        debug_printf("\x0d\x0a �Ͽ�����");
        #endif     	
   			return 1;
   	  }
   	  if(memcmp(rece_buf,"\xa0\x09",2) == 0){
   	  	memcpy(buf,"\xa2\x32\x00\x00",4);
        prot_zte_send_block(buf,4);
        prot_zte_rece_process(50);
        //zte_set_wait(100);
        #ifdef DEBUG_PRINT
        debug_printf("\x0d\x0a �Ͽ�����");
        #endif     	
   			return 1;
   	  } 
   		return 0;
   	}
		#ifdef DEBUG_PRINT
		debug_printf("\x0d\x0a �Ͽ�����");
		#endif  
		memcpy(buf,"\xa2\x32\x0\x0",4);
		prot_zte_send_block(buf,4); 
		prot_zte_rece_process(30); 
		return 1;
}

char check_24G_module(void)
{
char cnt = 0;
int i;

for(i=0;i<5;i++) {
  if(reader_test_if_24G_exist()) cnt++;
  	set_timeout(2000);
}

if(cnt > 2)  return 1;
	else return 0;	
}


#endif