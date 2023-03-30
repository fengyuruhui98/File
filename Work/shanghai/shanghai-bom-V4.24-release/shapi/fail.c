/*
fail.c
ʧ�ܱ����
*/

#define MAX_FAIL  1


//Ѻ�𱸷���
 uchar gFailB1Flag[MAX_FAIL];
 uchar gFailB1Block[MAX_FAIL][16];
 uchar gFailB1CardSnr[MAX_FAIL][4];
 uchar gFailB1Key[MAX_FAIL][12];
 uchar gFailB1StartPtr,gFailB1EndPtr,gFailB1ThisPtr;

//���ʱ�����
 uchar gFailAddFlag[MAX_FAIL];
 uchar gFailAddCardSnr[MAX_FAIL][4];
 uchar gFailAddCardStatus[MAX_FAIL];
 uchar gFailAddBlock[MAX_FAIL][64];
 uchar gFailAddStartPtr,gFailAddEndPtr,gFailAddThisPtr;
 uchar gFailAddKey[MAX_FAIL][12];
 uint gFailAddCardCnt[MAX_FAIL];
 uchar gFailAddCardRecPtr[MAX_FAIL];
 uchar gFailAddUpdatedRecord[MAX_FAIL][16];


 uchar gFailB1UnsaleFlag; 
//
uchar  gFailUpdataFlag;
 uchar gFailUpdataCardSnr[4];
 ulong gFailUpdataB6;
uint  gFailUpdataCardCnt;
 uchar gFailUpdataRecPtr;
 uchar gFailUpdataMetroStatue;
/*********************************************************************
��ʼ����ʧ�ܱ�
**********************************************************************/
void fail_init(void)
{
gFailB1StartPtr=0;
gFailB1EndPtr=0;
gFailAddStartPtr=0;
gFailAddEndPtr=0;
gFailB1UnsaleFlag = 0;
return;
}

/*********************************************************************
Ѻ�������ݼ���
**********************************************************************/
void fail_b1_add(void)
{
uchar i;
uchar  buf[16];
gFailB1CardSnr[gFailB1StartPtr][0] = gThisCardSnr[0];
gFailB1CardSnr[gFailB1StartPtr][1] = gThisCardSnr[1];
gFailB1CardSnr[gFailB1StartPtr][2] = gThisCardSnr[2];
gFailB1CardSnr[gFailB1StartPtr][3] = gThisCardSnr[3];
//for(i=0;i<6;i++){
//    gFailB1Key[gFailB1StartPtr][i] = gKeyA[0][i];
//    gFailB1Key[gFailB1StartPtr][6+i] = gKeyB[0][i];
//    }
hl_auth(AUTH_MODE_EXT_KEYA,0,gShFormalCardKeyA0);    
mcml_read(1,buf);
for(i=0;i<16;i++) gFailB1Block[gFailB1StartPtr][i] = buf[i];	
gFailB1Flag[gFailB1StartPtr] = 0x55;

if(gFailB1StartPtr == (MAX_FAIL-1)) gFailB1StartPtr=0;
else gFailB1StartPtr++;

if(gFailB1EndPtr == gFailB1StartPtr){
  if(gFailB1EndPtr == (MAX_FAIL-1)) gFailB1EndPtr=0;
  else gFailB1EndPtr++;
  }	

return;	
}


/*********************************************************************
�ӿ����������ݼ���
**********************************************************************/
void fail_add_add(void)
{
uchar i;

	gFailAddCardSnr[gFailAddStartPtr][0] = gThisCardSnr[0];
	gFailAddCardSnr[gFailAddStartPtr][1] = gThisCardSnr[1];
	gFailAddCardSnr[gFailAddStartPtr][2] = gThisCardSnr[2];
	gFailAddCardSnr[gFailAddStartPtr][3] = gThisCardSnr[3];
	gFailAddCardStatus[gFailAddStartPtr] = gCardStatus;
	for(i=0;i<16;i++){
	    gFailAddBlock[gFailAddStartPtr][i] = gB8[i];	
	    gFailAddBlock[gFailAddStartPtr][16+i] = gB9[i];	
	    gFailAddBlock[gFailAddStartPtr][32+i] = gB10[i];
	    gFailAddBlock[gFailAddStartPtr][48+i] = gM1AddValKey[i];	
	    }
	gFailAddFlag[gFailAddStartPtr] = 0x55;
	//for(i=0;i<6;i++){
	//    gFailAddKey[gFailAddStartPtr][i] = gKeyA[2][i];
	//    gFailAddKey[gFailAddStartPtr][6+i] = gKeyB[2][i];	
	//    }
	gFailAddCardCnt[gFailAddStartPtr] = gCardCnt;
	gFailAddCardRecPtr[gFailAddStartPtr] = gCardRecPtr;
	for(i=0;i<16;i++){
	   gFailAddUpdatedRecord[gFailAddStartPtr][i] = gCardRecord[i];
	   }
	
	
	if(gFailAddStartPtr == (MAX_FAIL-1)) gFailAddStartPtr=0;
	else gFailAddStartPtr++;
	
	if(gFailAddEndPtr == gFailAddStartPtr){
	  if(gFailAddEndPtr == (MAX_FAIL-1)) gFailAddEndPtr=0;
	  else gFailAddEndPtr++;
	  }	
	
	return;		
}


/*****************************************************************
��Ѻ��������
******************************************************************/
void fail_b1_clr(void)
{
uchar i;

i = gFailB1EndPtr;
loop:
if(i==gFailB1StartPtr) return;
if(gFailB1Flag[i] != 0x55) goto label_con;
if((uchar)gFailB1CardSnr[i][0] != (uchar)gThisCardSnr[0]) goto label_con;
if((uchar)gFailB1CardSnr[i][1] != (uchar)gThisCardSnr[1]) goto label_con;
if((uchar)gFailB1CardSnr[i][2] != (uchar)gThisCardSnr[2]) goto label_con;
if((uchar)gFailB1CardSnr[i][3] != (uchar)gThisCardSnr[3]) goto label_con;
gFailB1Flag[i] = 0x66;

label_con:
if(i == (MAX_FAIL-1)) i=0;
else i++;
goto loop;	
}

/*****************************************************************
��ӿ�������
******************************************************************/
void fail_add_clr(uchar flag)
{
uchar i,j;

i = gFailAddEndPtr;
j = (gFailAddStartPtr+MAX_FAIL-1)%MAX_FAIL;
loop:
if(i==gFailAddStartPtr) return;
if(gFailAddFlag[i] != 0x55){
   if(i != j) gFailAddFlag[i] = 0;   //ȷ��ֻ�����һ�ſ�����ȡ���ӿ�	
   goto label_con;
   }
if((uchar)gFailAddCardSnr[i][0] != (uchar)gThisCardSnr[0]) goto label_con;
if((uchar)gFailAddCardSnr[i][1] != (uchar)gThisCardSnr[1]) goto label_con;
if((uchar)gFailAddCardSnr[i][2] != (uchar)gThisCardSnr[2]) goto label_con;
if((uchar)gFailAddCardSnr[i][3] != (uchar)gThisCardSnr[3]) goto label_con;
if(i != j) gFailAddFlag[i] = 0;
else gFailAddFlag[i] = flag;

label_con:
if(i == (MAX_FAIL-1)) i=0;
else i++;
goto loop;	
}



/*****************************************************************
���Ѻ��������
******************************************************************/
uchar fail_b1_chk(void)
{
uchar i;

i = gFailB1EndPtr;
loop:
if(i==gFailB1StartPtr) return 0;
if(gFailB1Flag[i] != 0x55){
   if((gFailB1UnsaleFlag != 0x55) || (gFailB1Flag[i] != 0x66)) goto label_con;
   }
if((uchar)gFailB1CardSnr[i][0] != (uchar)gThisCardSnr[0]) goto label_con;
if((uchar)gFailB1CardSnr[i][1] != (uchar)gThisCardSnr[1]) goto label_con;
if((uchar)gFailB1CardSnr[i][2] != (uchar)gThisCardSnr[2]) goto label_con;
if((uchar)gFailB1CardSnr[i][3] != (uchar)gThisCardSnr[3]) goto label_con;
gFailB1ThisPtr = i;

#ifdef _EMU_
printf("\nFail b1 chk found");
#endif	
return 1; 

label_con:
if(i == (MAX_FAIL-1)) i=0;
else i++;
goto loop;	
}



/*****************************************************************
���ӿ�������
******************************************************************/
uchar fail_add_chk(void)
{
uchar i;

i = gFailAddEndPtr;
loop:
#ifdef _EMU_
printf("\nAdd chk i=%d",i);
#endif
if(i==gFailAddStartPtr){
    #ifdef _EMU_
    printf("\ni==gFailAddStartPtr");
    #endif	
    return 0;
    }
if(gFailAddFlag[i] != 0x55){
    #ifdef _EMU_
    printf("\nFlag != 0x55. It's %02X",(uchar)gFailAddFlag[i]);
    #endif	
    goto label_con;
    }
#ifdef _EMU_
printf("\nfail add flag set at ptr=%d",(uchar)i);
#endif	
if((uchar)gFailAddCardSnr[i][0] != (uchar)gThisCardSnr[0]) goto label_con;
if((uchar)gFailAddCardSnr[i][1] != (uchar)gThisCardSnr[1]) goto label_con;
if((uchar)gFailAddCardSnr[i][2] != (uchar)gThisCardSnr[2]) goto label_con;
if((uchar)gFailAddCardSnr[i][3] != (uchar)gThisCardSnr[3]) goto label_con;
gFailAddThisPtr = i;
#ifdef _EMU_
printf("\nCard snr ok");
#endif

if((uint)(gCardCnt-1) == (uint)gFailAddCardCnt[i]) return 1; 

label_con:
if(i == (MAX_FAIL-1)) i=0;
else i++;
goto loop;	
}

/*********************************************************************
Ѻ�����ָ�
**********************************************************************/
char fail_b1_restore(void)
{
uchar ret;
//uchar i;

#ifdef _EMU_
printf("\nfail b1 restore now");
#endif

//for(i=0;i<6;i++){
//   gKeyA[0][i] = gFailB1Key[gFailB1ThisPtr][i];
//   gKeyB[0][i] = gFailB1Key[gFailB1ThisPtr][i+6];
//   }
	
	ret = hl_auth(AUTH_MODE_EXT_KEYA,0,gShFormalCardKeyA0);
	if(ret){
		#ifdef _EMU_
	  printf("\nErr:hl_auth,sector0 with formal key,ret=%d",ret);
	  #endif
	  goto label_b1_restore_try_test_key;
	  }
	goto label_b1_restore_key_ok;

label_b1_restore_try_test_key:
	ret = hl_auth(AUTH_MODE_EXT_KEYA,0,gShTestCardKeyA0);
	if(ret){
		#ifdef _EMU_
	  printf("\nErr:hl_auth,sector0 with test key,ret=%d",ret);
	  #endif
	  }

label_b1_restore_key_ok:
	
if(mcml_write(1,gFailB1Block[gFailB1ThisPtr]) != 0){
        #ifdef _EMU_
        printf("\nfail b1 restore err");
        #endif
	return (char)-1;
	}
#ifdef _EMU_
printf("\nfail b1 restore ok");
#endif
gFailB1UnsaleFlag = 0;

return 1;   //�лָ���������Ӧ����Ӧ����
}

/*********************************************************************
�ӿ����ָ�
**********************************************************************/
char fail_add_restore(void)
{
uchar i,ch;
uint tint;
uchar  buf[17];
uchar ret;

#ifdef _EMU_
printf("\nfail add restore now!!!");
printf("\nplease press any key to continue...");
#endif

//for(i=0;i<6;i++){
//   gKeyA[2][i] = gFailAddKey[gFailAddThisPtr][i];
//   gKeyB[2][i] = gFailAddKey[gFailAddThisPtr][i+6];
//   }

//����B12����
if(trans_setup_bakup() != 0) return (char)-1;

for(i=0;i<11;i++) buf[i] = 0;
buf[11]=gFailAddCardStatus[gFailAddThisPtr];
buf[12]=gFailAddCardRecPtr[gFailAddThisPtr]%10;
tint = gFailAddCardCnt[gFailAddThisPtr];

buf[13]=*((char *)&tint+1);
buf[14]=*((char *)&tint+0);

lrc_gen2(buf,15);
gDebugStep = 0x350c;
ret = hl_auth(AUTH_MODE_INT_KEYA,3,(UBYTE *)"\x0\x0\x0\x0\x0\x0");	 
if(ret){
	       #ifdef _EMU_
         printf("\nErr:hl_auth,ret=%d",ret);
       	 #endif
       	 return (char)-1;
       	}
if(mcml_write(13,buf) != 0) return (char)-1;

//ʹ��ǰ��洢�Ŀ��ϼ�¼�����ӿ��¼
for(i=0;i<16;i++) buf[i] = 0xff;
ch = gFailAddCardRecPtr[gFailAddThisPtr]%10;
ch = gCardRecPos[ch];
for(i=0;i<16;i++) buf[i] = gFailAddUpdatedRecord[gFailAddThisPtr][i];

gDebugStep = 0x350d;
ret = hl_auth(AUTH_MODE_INT_KEYA,ch/4,(UBYTE *)"\x0\x0\x0\x0\x0\x0");	 
if(ret){
	       #ifdef _EMU_
         printf("\nErr:hl_auth,ret=%d",ret);
       	 #endif
       	 return (char)-1;
       	}
if(mcml_write(ch,buf) != 0) return (char)-1;

//if(mcmh_write(8,&gFailAddBlock[gFailAddThisPtr][0],0,1) != 0) return (char)-1;
//if(mcmh_write(9,&gFailAddBlock[gFailAddThisPtr][16],2,1) != 0) return (char)-1;
//if(mcmh_write(10,&gFailAddBlock[gFailAddThisPtr][32],2,1) != 0) return (char)-1;

//clear B12
for(i=0;i<16;i++) buf[i] = 0;
gDebugStep = 0x350e;
ret = hl_auth(AUTH_MODE_INT_KEYA,3,(UBYTE *)"\x0\x0\x0\x0\x0\x0");	 
if(ret){
	       #ifdef _EMU_
         printf("\nErr:hl_auth,ret=%d",ret);
       	 #endif
       	 return (char)-1;
       	}
mcml_write(12,buf);  //��ʱ���ж��Ƿ�ɹ�

gCardCnt = gFailAddCardCnt[gFailAddThisPtr];
gCardRecPtr = gFailAddCardRecPtr[gFailAddThisPtr];

#ifdef _EMU_
printf("\nfail add restore ok");
#endif
return 1;   //�лָ���������Ӧ����Ӧ����
}



/********************************************************************************
�����ļ�������
**********************************************************************************/
#ifdef _EMU_
void fail_add_file_save(void)
{
uchar i,j;
FILE *fp;

fp = fopen("addsave.txt","wt+");
if(!fp){
   printf("\nError: open file addsave.txt");
   return;	
   }	
fprintf(fp,"%02X ",(uchar)gFailAddStartPtr);
fprintf(fp,"%02X ",(uchar)gFailAddEndPtr);
fprintf(fp,"%02X ",(uchar)gFailAddThisPtr);
for(i=0;i<MAX_FAIL;i++){
  fprintf(fp,"\n");
  fprintf(fp,"%02X",(uchar)gFailAddFlag[i]);
  fprintf(fp,"\n");
  for(j=0;j<4;j++) fprintf(fp,"%02X ",(uchar)gFailAddCardSnr[i][j]);
  fprintf(fp,"\n");
  for(j=0;j<16;j++) fprintf(fp,"%02X ",(uchar)gFailAddBlock[i][j]);	
  fprintf(fp,"\n");
  for(j=0;j<16;j++) fprintf(fp,"%02X ",(uchar)gFailAddBlock[i][j+16]);	
  fprintf(fp,"\n");
  for(j=0;j<16;j++) fprintf(fp,"%02X ",(uchar)gFailAddBlock[i][j+32]);	
  fprintf(fp,"\n");
  for(j=0;j<18;j++) fprintf(fp,"%02X ",(uchar)gFailAddKey[i][j]);	
  fprintf(fp,"\n%04X",(uint)gFailAddCardCnt[i]);
  fprintf(fp,"\n%02X",(uchar)gFailAddCardRecPtr[i]);
  }	

fclose(fp);
return;
}	




void fail_add_file_load(void)
{
uchar i,j;
uint tint;
FILE *fp;

fp = fopen("addsave.txt","rt");
if(!fp){
   printf("\nError: open file addsave.txt");
   return;	
   }	

if(fscanf(fp,"%02X ",&tint) != 1) goto label_err;
gFailAddStartPtr = (uchar)tint;
if(fscanf(fp,"%02X ",&tint) != 1) goto label_err;
gFailAddEndPtr = (uchar)tint;
if(fscanf(fp,"%02X ",&tint) != 1) goto label_err;
gFailAddThisPtr = (uchar)tint;
for(i=0;i<MAX_FAIL;i++){
  if(fscanf(fp,"%02X ",&tint) != 1) goto label_err;
  gFailAddFlag[i] = (uchar)tint;
  for(j=0;j<4;j++){
         if(fscanf(fp,"%02X ",&tint) != 1) goto label_err;
         gFailAddCardSnr[i][j] = (uchar)tint;
  	 }
  for(j=0;j<48;j++){
         if(fscanf(fp,"%02X ",&tint) != 1) goto label_err;
         gFailAddBlock[i][j] = (uchar)tint;
  	 }
  for(j=0;j<18;j++){
         if(fscanf(fp,"%02X ",&tint) != 1) goto label_err;
         gFailAddKey[i][j] = (uchar)tint;
  	 }
  if(fscanf(fp,"%04X ",&tint) != 1) goto label_err;
  gFailAddCardCnt[i] = tint;
  if(fscanf(fp,"%02X ",&tint) != 1) goto label_err;
  gFailAddCardRecPtr[i] = (uchar)tint;
  }	

fclose(fp);
return;

label_err:
printf("\nErr: fail_add_file_load");
return;
}	


void fail_b1_file_save(void)
{
uchar i,j;
FILE *fp;

fp = fopen("b1save.txt","wt+");
if(!fp){
   printf("\nError: open file b1save.txt");
   return;	
   }	
fprintf(fp,"%02X ",(uchar)gFailB1StartPtr);
fprintf(fp,"%02X ",(uchar)gFailB1EndPtr);
fprintf(fp,"%02X ",(uchar)gFailB1ThisPtr);
for(i=0;i<MAX_FAIL;i++){
  fprintf(fp,"\n");
  fprintf(fp,"%02X",(uchar)gFailB1Flag[i]);
  fprintf(fp,"\n");
  for(j=0;j<4;j++) fprintf(fp,"%02X ",(uchar)gFailB1CardSnr[i][j]);
  fprintf(fp,"\n");
  for(j=0;j<16;j++) fprintf(fp,"%02X ",(uchar)gFailB1Block[i][j]);	
  fprintf(fp,"\n");
  for(j=0;j<12;j++) fprintf(fp,"%02X ",(uchar)gFailB1Key[i][j]);	
  }	

fclose(fp);
return;
}	


void fail_b1_file_load(void)
{
uchar i,j;
uint tint;
FILE *fp;

fp = fopen("b1save.txt","rt");
if(!fp){
   printf("\nError: open file b1save.txt");
   return;	
   }	

if(fscanf(fp,"%02X ",&tint) != 1) goto label_err;
gFailB1StartPtr = (uchar)tint;
if(fscanf(fp,"%02X ",&tint) != 1) goto label_err;
gFailB1EndPtr = (uchar)tint;
if(fscanf(fp,"%02X ",&tint) != 1) goto label_err;
gFailB1ThisPtr = (uchar)tint;
for(i=0;i<MAX_FAIL;i++){
  if(fscanf(fp,"%02X ",&tint) != 1) goto label_err;
  gFailB1Flag[i] = (uchar)tint;
  for(j=0;j<4;j++){
         if(fscanf(fp,"%02X ",&tint) != 1) goto label_err;
         gFailB1CardSnr[i][j] = (uchar)tint;
  	 }
  for(j=0;j<16;j++){
         if(fscanf(fp,"%02X ",&tint) != 1) goto label_err;
         gFailB1Block[i][j] = (uchar)tint;
  	 }
  for(j=0;j<12;j++){
         if(fscanf(fp,"%02X ",&tint) != 1) goto label_err;
         gFailB1Key[i][j] = (uchar)tint;
  	 }
  }	

fclose(fp);
return;

label_err:
printf("\nErr: fail_b1_file_load");
return;
}	

#endif
