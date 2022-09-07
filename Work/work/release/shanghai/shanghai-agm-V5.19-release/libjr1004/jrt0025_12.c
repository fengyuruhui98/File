//jrt0025_12.c
//ʵ��pboc2.0 jr/t0025 
//������ 20111128
#ifndef _JRT002512_C_
#define _JRT002512_C_
#include <stdio.h>
#include <sys/time.h>
//#include <linux2440lib.h>
//#include <jrt0025_12.h>
#include "utilities.h"

#define PSAM_NDX 4
#define LEN0221 64
#define LEN19 64

unsigned char gAPPAount_SN[11];
unsigned char gTerminalNo[4];
unsigned char gSAMNo[6];
unsigned char gBuf0221[LEN0221];
unsigned char gBuf19[LEN19];

UBYTE gAppSN[8],gF19_Thr[3];
T_APDU C_Apdu, R_Apdu;

pthread_t	g_pth801AID, g_pth001E;
sem_t g_801Await, g_801Areturn;
unsigned char mac_001e[4];

#define _NEW_GETTAGFRMTLV_

#define _PARAMCACERT_
#define _GET0321_

//#define _DEBUG_PBOCAPDU_
//#define _DEBUG_FDDA_L2_
#define _NOFDDA_

//#define _SOFTMAC_
//#define _DEBUG_PPSE_SEL_
//#define _DEBUG_L2T_
//#define _DEBUG_SOFTMAC_


//#define _DEBUG_PSAM_MAC_

//#define _DEBUG_SAMEXCH_
//#define _DEBUGEXETIME_
//#define _DEBUG_TRANSDATA_f

//#define _DEBUG_PARA_INIT_

//#define 	DEBUG_TIME_CPU

#ifdef _DEBUGEXETIME_ 
struct  timeval  start;
struct  timeval  end;
unsigned long timer;
#endif
int PPSE_MEC(UBYTE* outBuf,int* nOutLen);
int tlv_test_unpack(UBYTE*  buf,int nLen)
{
	int  i, j;
	int  count;
	pTlvData tlv = NULL;
	
	//count = unpack_tlv_data(&tlv, buf, sizeof(buf)/sizeof(buf[0]));
	count = unpack_tlv_data(&tlv, buf, nLen);
	if (count < 0)
	{
		printf("count failed, error code:%d\n", count);
	}

	printf("//Tlv Unpack Test\n");
	for (i=0; i<count; ++i)
	{
		printf("Tag:");
		for (j=0; j<tlv[i].tag.size; ++j)
		{
			printf("%02X", tlv[i].tag.data[j]);
			if((j+1)%16==0)
				printf("\n");
	
		}
		printf("\n");

		printf("Len:%d\n", tlv[i].val.size);
		printf("Val:");
		for (j=0; j<tlv[i].val.size; ++j)
		{
			printf("%02X ", tlv[i].val.data[j]);
			if((j+1)%16==0)
				printf("\n");
	
		}
		printf("\n");
	}
	
	free(tlv);
	tlv = NULL;
	
	return 0;
}

void gdes_encode(UBYTE *key,UBYTE *sr,UBYTE *dest)
{
	//Lib_Des(sr,dest,key,1);
	return; 	
}
void gdes_decode(UBYTE *key,UBYTE *sr,UBYTE *dest)
{
	//Lib_Des(sr,dest,key,0);
	return; 	
}	
void gdes3_encode(UBYTE *key,UBYTE *sr,UBYTE *dest)
{
	UBYTE buf[8];
	memcpy(buf,sr,8);	
	gdes_encode(key,buf,dest);
	gdes_decode(&key[8],dest,buf);
	gdes_encode(key,buf,dest);
	return;
}

void gdes3_decode(UBYTE *key,UBYTE *sr,UBYTE *dest)
{
	UBYTE buf[8];

	memcpy(buf,sr,8);	
	gdes_decode(key,buf,dest);
	gdes_encode(&key[8],dest,buf);
	gdes_decode(key,buf,dest);
	return;
}
UBYTE	WDiversity(UBYTE *pszMKKey,UBYTE *pszPID,UBYTE	*pszSKKey,UBYTE	bTriDes)
{
	int		i;
	UBYTE	pszBuf[16];
	UBYTE	pszKey[16];
	if(bTriDes)
	{
		memcpy(pszKey,pszMKKey,16);
		memset(pszBuf,0x00,16);

		gdes_encode(pszKey, pszPID, pszBuf);
		gdes_decode(pszKey+8, pszBuf,pszBuf+8);
		gdes_encode(pszKey, pszBuf+8, pszBuf);
		memcpy(pszSKKey,pszBuf,8);

		memset(pszBuf,0x00,16);			
		for(i=0;i<8;i++) pszBuf[i] =~pszPID[i];
		gdes_encode(pszKey, pszBuf, pszBuf+8);
		gdes_decode(pszKey+8, pszBuf+8,pszBuf);
		gdes_encode(pszKey, pszBuf, pszBuf+8);	
		memcpy(pszSKKey+8,pszBuf+8,8);

	}
	else
	{
		gdes_encode(pszMKKey,pszPID, pszSKKey);
	}
	return 0;
}

UBYTE	WDiversityExt(UBYTE *pszMKKey,UBYTE *pszPID,UBYTE	*pszSKKey,UBYTE	bTriDes)
{
	int		i;
	UBYTE	pszBuf[16];
	UBYTE	pszKey[16];
	if(bTriDes)
	{
		memcpy(pszKey,pszMKKey,16);
		memset(pszBuf,0x00,16);

		gdes_encode(pszKey, pszPID, pszBuf);
		gdes_decode(pszKey+8, pszBuf,pszBuf+8);
		gdes_encode(pszKey, pszBuf+8, pszBuf);

		memcpy(pszSKKey,pszBuf,8);

		memset(pszBuf,0x00,16);			
		for(i=6;i<8;i++) pszBuf[i] =pszPID[i] ^ 0xff;
		fprintf(stderr,"WDiversity EXT:");

		for(i=0;i<8;i++)
			fprintf(stderr,"%02X ",pszBuf[i]);
		fprintf(stderr,"\n\r:");

		gdes_encode(pszKey, pszBuf, pszBuf+8);
		gdes_decode(pszKey+8, pszBuf+8,pszBuf);
		gdes_encode(pszKey, pszBuf, pszBuf+8);	
		memcpy(pszSKKey+8,pszBuf+8,8);
	}
	else
	{
		gdes_encode(pszMKKey,pszPID, pszSKKey);
	}
	return 0;

}

void	WCalMac(UWORD nLenIn,UBYTE *pszBufIn,UBYTE *pszInitData,UBYTE *pszKey,UBYTE *pszMAC,UBYTE	bTriDes)
{
	UBYTE		pszBufAdd[8];
	UBYTE		pszBuf[8];
	UWORD		i,j;

	UWORD		nLenTotal;		//	\B2\B9\D7\E3\D7\EE\BA\F3�\B8\F6\CA\FD\BE�\E9\B5�\A4\B6�\A3
	nLenTotal = (nLenIn/8+1)*8;		

	memcpy(pszBufAdd,"\x80\x00\x00\x00\x00\x00\x00\x00",8);
	memcpy(pszBuf,pszInitData,8);

	for(i=0;i<nLenTotal;i=i+8) 
	{
		for(j=0;j<8;j++)
		{
			if((i+j)<nLenIn)
			{
				pszBuf[j] = pszBufIn[i+j] ^ pszBuf[j];
			}
			else
			{
				pszBuf[j] = pszBufAdd[i+j-nLenIn] ^ pszBuf[j];
			}
		}
		gdes_encode(pszKey,pszBuf,pszBuf);
	}
	if(bTriDes)
	{
		gdes_decode(pszKey+8,pszBuf,pszBuf);
		gdes_encode(pszKey,pszBuf,pszBuf);
	}
	memcpy(pszMAC,pszBuf,8);
	return;

}


int PSAM_SoftMAC(UBYTE* Key,UBYTE* Diversity,UBYTE* pszInitData,UBYTE* pszBuf,int nBufLen,UBYTE* MAC)
{

#ifdef _DEBUG_SOFTMAC_
	int i;
	UBYTE rbytes;
	UBYTE uTmp_Len;
#endif
	UBYTE buf[256],subKey[16];

#ifdef _DEBUG_SOFTMAC_
	fprintf(stderr,"\n\r Key:");
	for(i=0;i<16;i++)
		fprintf(stderr,"%02X ", Key[i]);
	fprintf(stderr,"\n\r");

	fprintf(stderr,"Diversity:");

	for(i=0;i<8;i++)
		fprintf(stderr,"%02X ", Diversity[i]);
	fprintf(stderr,"\n\r");

	fprintf(stderr,"pszInitData:");
	for(i=0;i<8;i++)
		fprintf(stderr,"%02X ", pszInitData[i]);
	fprintf(stderr,"\n\r");

	fprintf(stderr,"pszBuf,%d:",nBufLen);

	for(i=0;i<nBufLen;i++)
		fprintf(stderr,"%02X ", pszBuf[i]);
	fprintf(stderr,"\n\r");

#endif

	memset(buf,0x00,sizeof(buf));
	WDiversity(Key,Diversity,buf,1);
#ifdef _DEBUG_SOFTMAC_
	fprintf(stderr,"Diversity 1 Result :");
	for(i=0;i<16;i++)
		fprintf(stderr,"%02X ",buf[i]);
	fprintf(stderr,"\n\r");
#endif

	memcpy(subKey,buf,16);
	/*
	memset(buf,0x00,sizeof(buf));
	//WDiversityExt(subKey,Diversity+8,buf,1);
	WDiversity(subKey,Diversity+8,buf,1);
	memcpy(subKey,buf,16);
#ifdef _DEBUG_SOFTMAC_
	fprintf(stderr,"WDiversity 2��");
	for(i=0;i<16;i++)
		fprintf(stderr,"%02X ",subKey[i]);
	fprintf(stderr,"\n\r");
#endif

*/
	memset(buf,0x00,sizeof(buf));
	WCalMac(nBufLen,pszBuf,pszInitData,subKey,buf,1);
#ifdef _DEBUG_SOFTMAC_
	fprintf(stderr,"PSAM_SoftMAC :");
	for(i=0;i<8;i++)
		fprintf(stderr,"%02X ",buf[i]);
	fprintf(stderr,"\n\r");
#endif
	memcpy(MAC,buf,8);
	return 0;
}
UBYTE gascToUC(UBYTE  ch)
{
	UBYTE value;
	
	switch(ch){
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
				value = ch - '0';
				break;
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
				value = ch - 'a'+10;
				break;
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
				value = ch - 'A'+10;
				break;
	
	default:
				value = 0;
				break;
	    }
	return value;
}

int	MyCmdSamExchange(UBYTE index,UBYTE nInLen,UBYTE *pszDataIn,UBYTE *nOutLen,UBYTE *pszDataOut)
{
	UBYTE  rbytes;
	int ret,i;
	UBYTE buf[256],uTmp[12];
	UBYTE nResponseBytes;

	memset(buf,0x00,256);
#ifdef _DEBUG_SAMEXCH_
	printf("\n\r");
	printf("samapdu send(%d):\n\r",nInLen);
	for(i=0;i<nInLen;i++)
	{
		printf("%02X ",pszDataIn[i]);
		if((i+1)%16==0)
		printf("\n\r");
	}
	printf("\n\r");
#endif
	ret=sam_apdu(index,pszDataIn,nInLen,buf,&rbytes,500,0);
#ifdef _DEBUG_SAMEXCH_
	printf("\n\r");
	printf("sam_apdu,ret=%d recv(%d)bytes \n\r",ret,rbytes);
	for(i=0;i<rbytes;i++)
		printf("%02X ",buf[i]);
	printf("\n\r");
#endif
	if(ret!=0) 
		return -3;
	if(rbytes==2 && buf[0]==0x61)
	{
		nResponseBytes = buf[1];
		memcpy(uTmp,"\x00\xc0\x00\x00",4);	
		uTmp[4] = nResponseBytes;
		memset(buf,0x00,256);
		rbytes=0;
		ret=sam_apdu(index,uTmp,5,buf,&rbytes,600,0);
		#ifdef _DEBUG_SAMEXCH_
			fprintf(stderr,"ret=%d :\n\r",ret);
			for(i=0;i<rbytes;i++)
				fprintf(stderr,"%02X ",buf[i]);
			fprintf(stderr,"\n\r");
		#endif
		if(ret!=0) 
			return -6;
	}
	*nOutLen = rbytes;
	memcpy(pszDataOut,buf,rbytes);		
	return 0;
}


int	MyCmdSamExchange_Send(UBYTE index,UBYTE nInLen,UBYTE *pszDataIn)
{
	UBYTE  rbytes;
	int ret,i;
	UBYTE buf[256],uTmp[12];
	UBYTE nResponseBytes;

	memset(buf,0x00,256);
#ifdef _DEBUG_SAMEXCH_
	printf("\n\r");
	printf("MyCmdSamExchange_Send-->samapdu send(%d):\n\r",nInLen);
	for(i=0;i<nInLen;i++)
	{
		printf("%02X ",pszDataIn[i]);
		if((i+1)%16==0)
		printf("\n\r");
	}
	printf("\n\r");
#endif
	ret=sam_apdu_send(index,pszDataIn,nInLen);
	if(ret!=0) 
		return -3;
		
	return 0;
}

int	MyCmdSamExchange_Receive(UBYTE *nOutLen,UBYTE *pszDataOut)
{
	UBYTE  rbytes;
	int ret,i;
	UBYTE buf[256];
	UBYTE nResponseBytes;

	memset(buf,0x00,256);
	ret=sam_apdu_receive(buf,&rbytes);
#ifdef _DEBUG_SAMEXCH_
	printf("\n\r");
	printf("MyCmdSamExchange_Receive-->sam_apdu,ret=%d recv(%d)bytes \n\r",ret,rbytes);
	for(i=0;i<rbytes;i++)
		printf("%02X ",buf[i]);
	printf("\n\r");
#endif
	if(ret!=0) 
		return -3;
		
	*nOutLen = rbytes;
	memcpy(pszDataOut,buf,rbytes);		
	return 0;
}


int PSAM_MAC_FA(UBYTE index,UBYTE uNdx_Diversity,UBYTE* Diversity,UBYTE* pszInitData,UBYTE* uData,int nLen,UBYTE* MAC)
{
	int ret;
	int i;
	UBYTE rbytes,uTmp_Len,ta1_H,ta1_L,ta1;
	UBYTE pszBuf[64],buf[256];

	/*
	//80 1A 46 03 10
	rbytes=0;
	memset(pszBuf,0x00,sizeof(pszBuf));
	memcpy(pszBuf,"\x80\x1A\x28\x02\x08",5);
	pszBuf[3] = uNdx_Diversity;
	memcpy(pszBuf+5,Diversity,8);
	memset(buf,0x00,sizeof(buf));
	ret = MyCmdSamExchange(index, 13,pszBuf,&rbytes,buf);
	if(ret!=0) 
	{

#ifdef _DEBUG_PSAM_MAC_
		fprintf(stderr,"\n\r 80 1A:ret=%d ",ret);
#endif
		return ret;
	}
	if(buf[rbytes-2]!=0x90 || buf[rbytes-1]!=0x00) 
	{
		ret = buf[rbytes-2];
		ret = ret * 0x100+buf[rbytes-1];
#ifdef _DEBUG_PSAM_MAC_
		fprintf(stderr,"\n\r 80 1A:ret=%04X ",(unsigned int)ret);
#endif
		return ret;
	}
	*/
	//sem_wait(&g_801Areturn);
	ret = MyCmdSamExchange_Receive(buf,&rbytes);
	if(ret) return -1;
	
	//80 FA 05 00
	memset(pszBuf,0x00,sizeof(pszBuf));
	memcpy(pszBuf,"\x80\xFA\x05\x00\x50",5);
	memcpy(pszBuf+5,pszInitData,8);
	memcpy(pszBuf+5+8,uData,nLen);

	pszBuf[4]=nLen+8;
	memset(buf,0x00,sizeof(buf));
	ret = MyCmdSamExchange(index, 5+nLen+8,pszBuf,&rbytes,buf);
	if(ret!=0) 
	{
#ifdef _DEBUG_PSAM_MAC_
		fprintf(stderr,"\n\r 80 FA:ret=%d ",ret);
#endif
		return ret;
	}
	if(buf[rbytes-2]!=0x90 || buf[rbytes-1]!=0x00) 
	{
		ret = buf[rbytes-2];
		ret = ret * 0x100+buf[rbytes-1];
#ifdef _DEBUG_PSAM_MAC_
		fprintf(stderr,"\n\r 80 FA:ret=%04X ",(unsigned int)ret);
#endif
		return ret;

	}
	memcpy(MAC,buf,4);
	return 0;

}

int PSAM_MAC(UBYTE index,UBYTE uNdx_Diversity,UBYTE* Diversity,UBYTE* pszInitData,UBYTE* uData,int nLen,UBYTE* MAC)
{
	int ret;
	int i;
	UBYTE rbytes,uTmp_Len,ta1_H,ta1_L,ta1;
	UBYTE pszBuf[64],buf[256];

	
	//80 1A 46 03 10
	rbytes=0;
	memset(pszBuf,0x00,sizeof(pszBuf));
	memcpy(pszBuf,"\x80\x1A\x28\x02\x08",5);
	pszBuf[3] = uNdx_Diversity;
	memcpy(pszBuf+5,Diversity,8);
	memset(buf,0x00,sizeof(buf));
	ret = MyCmdSamExchange(index, 13,pszBuf,&rbytes,buf);
	if(ret!=0) 
	{

#ifdef _DEBUG_PSAM_MAC_
		fprintf(stderr,"\n\r 80 1A:ret=%d ",ret);
#endif
		return ret;
	}
	if(buf[rbytes-2]!=0x90 || buf[rbytes-1]!=0x00) 
	{
		ret = buf[rbytes-2];
		ret = ret * 0x100+buf[rbytes-1];
#ifdef _DEBUG_PSAM_MAC_
		fprintf(stderr,"\n\r 80 1A:ret=%04X ",(unsigned int)ret);
#endif
		return ret;
	}
	//80 FA 05 00
	memset(pszBuf,0x00,sizeof(pszBuf));
	memcpy(pszBuf,"\x80\xFA\x05\x00\x50",5);
	memcpy(pszBuf+5,pszInitData,8);
	memcpy(pszBuf+5+8,uData,nLen);

	pszBuf[4]=nLen+8;
	memset(buf,0x00,sizeof(buf));
	ret = MyCmdSamExchange(index, 5+nLen+8,pszBuf,&rbytes,buf);
	if(ret!=0) 
	{
#ifdef _DEBUG_PSAM_MAC_
		fprintf(stderr,"\n\r 80 FA:ret=%d ",ret);
#endif
		return ret;
	}
	if(buf[rbytes-2]!=0x90 || buf[rbytes-1]!=0x00) 
	{
		ret = buf[rbytes-2];
		ret = ret * 0x100+buf[rbytes-1];
#ifdef _DEBUG_PSAM_MAC_
		fprintf(stderr,"\n\r 80 FA:ret=%04X ",(unsigned int)ret);
#endif
		return ret;

	}
	memcpy(MAC,buf,4);
	return 0;

}
int PSAM_MAC1(UBYTE index,UBYTE* uData,UBYTE* uPSAMCounter,UBYTE* MAC)
{
	int ret;
	int i;
	UBYTE rbytes,uTmp_Len,ta1_H,ta1_L,ta1;
	UBYTE pszBuf[64],buf[256];

	
	//80 70 00 00  1C
	rbytes=0;
	memset(pszBuf,0x00,sizeof(pszBuf));
	memcpy(pszBuf,"\x80\x70\x00\x00\x1C",5);
	memcpy(pszBuf+5,uData,28);
//	pszBuf[33]=0x08;
	memset(buf,0x00,sizeof(buf));
	ret = MyCmdSamExchange(index,5 + 28,pszBuf,&rbytes,buf);
#ifdef _DEBUG_PSAM_MAC_
	fprintf(stderr,"\n\r 80 70:ret=%d ",ret);
#endif
	if(ret!=0) 
	{
		return ret;
	}
	if(buf[rbytes-2]!=0x90 || buf[rbytes-1]!=0x00) 
	{
		ret = buf[rbytes-2];
		ret = ret * 0x100+buf[rbytes-1];

		return ret;
	}
	memcpy(uPSAMCounter,buf,4);
	memcpy(MAC,buf+4,4);
	return 0;

}

int PSAM_MAC2_VERIFY(UBYTE index,UBYTE* uData)
{
	int ret;
	int i;
	UBYTE rbytes,uTmp_Len,ta1_H,ta1_L,ta1;
	UBYTE pszBuf[64],buf[256];

	
	//80 70 00 00  1C
	rbytes=0;
	memset(pszBuf,0x00,sizeof(pszBuf));
	memcpy(pszBuf,"\x80\x72\x00\x00\x04",5);
	memcpy(pszBuf+5,uData,4);
//	pszBuf[33]=0x08;
	memset(buf,0x00,sizeof(buf));
	ret = MyCmdSamExchange(index,5 + 4,pszBuf,&rbytes,buf);
	if(ret!=0) 
	{

#ifdef _DEBUG_PSAM_MAC_
		fprintf(stderr,"\n\r 80 72:ret=%d ",ret);
#endif
		return ret;
	}
	if(buf[rbytes-2]!=0x90 || buf[rbytes-1]!=0x00) 
	{
		ret = buf[rbytes-2];
		ret = ret * 0x100+buf[rbytes-1];
#ifdef _DEBUG_PSAM_MAC_
		fprintf(stderr,"\n\r 80 72 ret=%04X ",(unsigned int)ret);
#endif
		return ret;
	}
	
	
	return 0;

}

UBYTE gbcd(UBYTE ch)
{
	UBYTE c;
	
	ch = ch%100;
	c = ch%10;
	c=  c + (ch/10)*16;
	
	return c;
}
void	gBCD2Str(unsigned int nLen,UBYTE *pszBCD,char *pszStr)
{
	unsigned int		i;
	
	for(i=0;i<nLen;i++)
	{
		sprintf(pszStr+2*i,"%02X",pszBCD[i]);
	}
	
	return;
}
void	gStr2Bytes(unsigned int nLen,char *pszStr,UBYTE *pszBuf)
{
	unsigned int		i;

	for(i=0;i<nLen;i=i+2)
	{
		pszBuf[i/2] = gascToUC((UBYTE)pszStr[i])*0x10 + gascToUC((UBYTE)pszStr[i+1]) ;
	}

	return;
}
void	gInt2Bytes(int i,UBYTE *pszBytes)
{
	union 
	{
		int		ival;
		UBYTE	ch[4];
	}i2b;

	i2b.ival = i;

	pszBytes[0] = i2b.ch[3];
	pszBytes[1] = i2b.ch[2];
	pszBytes[2] = i2b.ch[1];
	pszBytes[3] = i2b.ch[0];

	return;
}

void	gShort32Bytes(int i,UBYTE *pszBytes)
{
	union 
	{
		int		ival;
		UBYTE	ch[4];
	}i2b;

	i2b.ival = i;

	pszBytes[0] = i2b.ch[2];
	pszBytes[1] = i2b.ch[1];
	pszBytes[2] = i2b.ch[0];
	

	return;
}

void	gShort2Bytes(int i,UBYTE *pszBytes)
{
	union 
	{
		int		ival;
		UBYTE	ch[4];
	}i2b;

	i2b.ival = i;

	pszBytes[0] = i2b.ch[1];
	pszBytes[1] = i2b.ch[0];


	return;
}

void	gBytes2Int(UBYTE *pszBytes,int *i)
{
	union 
	{
		int		ival;
		UBYTE	ch[4];
	}i2b;	

	i2b.ch[3] = pszBytes[0];
	i2b.ch[2] = pszBytes[1];
	i2b.ch[1] = pszBytes[2];
	i2b.ch[0] = pszBytes[3];
	*i = i2b.ival;

	return;

}

void	gBytes2Short3(UBYTE *pszBytes,int *i)
{
	union 
	{
		int		ival;
		UBYTE	ch[4];
	}i2b;	


	i2b.ch[2] = pszBytes[0];
	i2b.ch[1] = pszBytes[1];
	i2b.ch[0] = pszBytes[2];
	*i = i2b.ival;

	return;

}
void	gBytes2Short(UBYTE *pszBytes,int *i)
{
	union 
	{
		int		ival;
		UBYTE	ch[4];
	}i2b;	
	
	i2b.ch[1] = pszBytes[0];
	i2b.ch[0] = pszBytes[1];
	*i = i2b.ival;

	return;

}
//1 Ϊ������1
//0 Ϊż����1
unsigned parity(unsigned arg)  
{  
	unsigned a = arg;  
	  
	a ^= a >> 16;  
	a ^= a >> 8;  
	a ^= a >> 4;  
	a ^= a >> 2;  
	a ^= a >> 1;  
	  
	return a & 1;  
} 

int PBOC_apdu(T_APDU *C_Apdu , T_APDU *R_Apdu )
{
	int ret;
	int i;
#ifdef _DEBUG_PBOCAPDU_
	printf("C_Apdu:length=%d\n\r",C_Apdu->length);
	for(i=0;i<C_Apdu->length;i++)
	{
		printf("%02X",C_Apdu->data[i]);
		if((i+1)%32==0)
			printf("\n\r");
	}
	printf("\n\r");
#endif
	ret = mifpro_icmd(C_Apdu->data,C_Apdu->length,R_Apdu->data,(unsigned char*)&(R_Apdu->length));
#ifdef _DEBUG_PBOCAPDU_
	printf("R_Apdu:ret =%d ,R_Apdu->length=%d \n\r",ret,R_Apdu->length);
	for(i=0;i<R_Apdu->length;i++)
	{
		printf("%02X",R_Apdu->data[i]);
		if((i+1)%32==0)
			printf("\n\r");

	}
	printf("\n\r");
#endif
	return ret;
}
void init_tagList()
{
	//ppse:6F
	memset(tag_PPSE,0x00,sizeof(tag_PPSE));
	tag_PPSE[0].uTag[0]=0x6f;
	tag_PPSE[0].uParent[0]=0x00;
	sprintf(tag_PPSE[0].sDescribe,"FCIģ��");
	tag_PPSE[0].min_Len=0;
	tag_PPSE[0].max_Len=0;	//min.max == 0  �䳤
	sprintf(tag_PPSE[0].sCondition,"M");
	
	//ppse:84
	tag_PPSE[1].uTag[0]=0x84;
	tag_PPSE[1].uParent[0]=0x6f;
	sprintf(tag_PPSE[1].sDescribe,"2PAY.SYS.DDF01");
	tag_PPSE[1].min_Len=0x0E;
	tag_PPSE[1].max_Len=0x0E;	//min=max == 0x0e
	sprintf(tag_PPSE[1].sCondition,"M");
	
	//ppse:A5
	tag_PPSE[2].uTag[0]=0xA5;
	tag_PPSE[2].uParent[0]=0x6f;
	sprintf(tag_PPSE[2].sDescribe,"FCIר��ģ��");
	tag_PPSE[2].min_Len=0x00;
	tag_PPSE[2].max_Len=0x00;	
	sprintf(tag_PPSE[2].sCondition,"M");
	
	//ppse:BF0C
	memcpy(tag_PPSE[3].uTag,"\xBF\x0C",2);
	tag_PPSE[3].uParent[0]=0xA5;
	sprintf(tag_PPSE[3].sDescribe,"FCI�������Զ�������");
	tag_PPSE[3].min_Len=0x00;
	tag_PPSE[3].max_Len=0x00;	
	sprintf(tag_PPSE[3].sCondition,"M");
	
	//ppse:61
	tag_PPSE[4].uTag[0]=0x61;
	memcpy(tag_PPSE[4].uParent,"\xBF\x0C",2);
	sprintf(tag_PPSE[4].sDescribe,"Ŀ¼���");
	tag_PPSE[4].min_Len=0x00;
	tag_PPSE[4].max_Len=0x00;	
	sprintf(tag_PPSE[4].sCondition,"M");
	
	//ppse:4F
	tag_PPSE[5].uTag[0]=0x4F;
	tag_PPSE[5].uParent[0]=0x61;
	sprintf(tag_PPSE[5].sDescribe,"AID");
	tag_PPSE[5].min_Len=0x07;
	tag_PPSE[5].max_Len=0x08;	
	sprintf(tag_PPSE[5].sCondition,"M");
	
	//ppse:50
	tag_PPSE[6].uTag[0]=0x50;
	tag_PPSE[6].uParent[0]=0x61;
	sprintf(tag_PPSE[6].sDescribe,"Ӧ�ñ�ǩ");
	tag_PPSE[6].min_Len=0x04;
	tag_PPSE[6].max_Len=0x10;	
	sprintf(tag_PPSE[6].sCondition,"O");

	//ppse:87
	tag_PPSE[7].uTag[0]=0x87;
	tag_PPSE[7].uParent[0]=0x61;
	sprintf(tag_PPSE[7].sDescribe,"Ӧ������ָʾ��");
	tag_PPSE[7].min_Len=0x01;
	tag_PPSE[7].max_Len=0x01;	
	tag_PPSE[7].uValue[0]=0xff;
	sprintf(tag_PPSE[7].sCondition,"C*");
	
	return ;
}
int PPSE_MEC(UBYTE* outBuf,int* nOutLen)
{
	int nRetCode=0 ;
	//PPSE	
	memcpy ( C_Apdu.data , "\x00\xA4\x04\x00\x08",5);
	memcpy ( C_Apdu.data+5,"APPMEC02",8);
	C_Apdu.data[13] = 0x00;
	C_Apdu.length = 5+8+1 ;
 
	nRetCode = PBOC_apdu(&C_Apdu, &R_Apdu );
	if (nRetCode!=OK)
	{
		fprintf(stderr,"PPSE :uRetCode=%d",nRetCode);
		return (nRetCode) ;
	}
	
	if(R_Apdu.data[R_Apdu.length-2]!=0x90 ||R_Apdu.data[R_Apdu.length-1]!=0x00) 
	{
		nRetCode =R_Apdu.data[R_Apdu.length-2];
		nRetCode = nRetCode * 0x100+R_Apdu.data[R_Apdu.length-1];
		fprintf(stderr,"MEC :uRetCode=%04X",(unsigned int)nRetCode);
		return nRetCode;

	}
	
	//fprintf(stderr,"PPSE :uRetCode=%04X",(unsigned int)nRetCode);
	*nOutLen = R_Apdu.length;
	memcpy(outBuf,R_Apdu.data,R_Apdu.length);
	return 0;
}


int PPSE_Q(UBYTE* outBuf,int* nOutLen)
{
	int nRetCode=0 ;
	//PPSE	
	memcpy ( C_Apdu.data , "\x00\xA4\x04\x00\x0E\x32\x50\x41\x59\x2E\x53\x59\x53\x2E\x44\x44\x46\x30\x31\x00",20);
	C_Apdu.length = 20 ;
 
	nRetCode = PBOC_apdu(&C_Apdu, &R_Apdu );
	if (nRetCode!=OK)
	{
		fprintf(stderr,"PPSE :uRetCode=%d",nRetCode);
		return (nRetCode) ;
	}
	
	if(R_Apdu.data[R_Apdu.length-2]!=0x90 ||R_Apdu.data[R_Apdu.length-1]!=0x00) 
	{
		nRetCode =R_Apdu.data[R_Apdu.length-2];
		nRetCode = nRetCode * 0x100+R_Apdu.data[R_Apdu.length-1];
		fprintf(stderr,"PPSE :uRetCode=%04X",(unsigned int)nRetCode);
		return nRetCode;
	}
	
	//fprintf(stderr,"PPSE :uRetCode=%04X",(unsigned int)nRetCode);
	*nOutLen = R_Apdu.length;
	memcpy(outBuf,R_Apdu.data,R_Apdu.length);
	return 0;
}
int PPSE_Proc_L2(UBYTE*  inBuf,int nInLen,UBYTE* uAID,int* uLen_AID)
{
	int nRetCode=0 ;
	UBYTE uTmp[128],uTmp2[128],uLen_AID_Total;
	int uLen_Tmp,uLen_place_last;
#ifdef _DEBUG_PPSE_SEL_
	int i,j;
#endif
	//6F:	
	if(inBuf[0]!=tag_PPSE[0].uTag[0])
	{
		fprintf(stderr,"PPSE,Resp Tag= %02X",inBuf[0]);
		return (STATUS_PROTOCOL_ERROR) ;
	}
	tag_PPSE[0].uLength=inBuf[1];
	memcpy(tag_PPSE[0].uValue,inBuf+2,tag_PPSE[0].uLength);
	
	//84:
	if(tag_PPSE[0].uValue[0]!=tag_PPSE[1].uTag[0])
	{
		fprintf(stderr,"PPSE,Resp tag_PPSE[0].uValue[0]= %02X",tag_PPSE[0].uValue[0]);
		return (STATUS_PROTOCOL_ERROR) ;
	}
	tag_PPSE[1].uLength=tag_PPSE[0].uValue[1];
	memcpy(tag_PPSE[1].uValue,tag_PPSE[0].uValue+2,tag_PPSE[1].uLength);

	//A5:
	if(tag_PPSE[0].uValue[2+tag_PPSE[1].uLength]!=tag_PPSE[2].uTag[0])
	{
		fprintf(stderr,"PPSE,Resp tag_PPSE[0].uValue[%d]= %02X",2+tag_PPSE[1].uLength,tag_PPSE[0].uValue[2+tag_PPSE[1].uLength]);
		return (STATUS_PROTOCOL_ERROR) ;
	}
	tag_PPSE[2].uLength=tag_PPSE[0].uValue[2+tag_PPSE[1].uLength+1];
	memcpy(tag_PPSE[2].uValue,tag_PPSE[0].uValue+2+tag_PPSE[1].uLength+2,tag_PPSE[2].uLength);
	
	//BF0C:
	if(memcmp(tag_PPSE[2].uValue,tag_PPSE[3].uTag,2)!=0)
	{
		fprintf(stderr,"PPSE,Resp tag_PPSE[2].uValue[0,1]= %02X%02X",tag_PPSE[2].uValue[0],tag_PPSE[2].uValue[1]);
		return (STATUS_PROTOCOL_ERROR) ;
	}
	tag_PPSE[3].uLength=tag_PPSE[2].uValue[2];
	memcpy(tag_PPSE[3].uValue,tag_PPSE[2].uValue+3,tag_PPSE[3].uLength);
	
	memset(uTmp,0x00,sizeof(uTmp));
	uLen_Tmp = tag_PPSE[3].uLength;
	memcpy(uTmp,tag_PPSE[3].uValue,	uLen_Tmp);
	uLen_AID_Total =0;
	uLen_place_last=0;
	while(1)
	{
	#ifdef _DEBUG_PPSE_SEL_
		fprintf(stderr,"PPSE,uTmp %d�ֽ� >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>:",uLen_Tmp);
		for(i=0;i<uLen_Tmp;i++)
		{
			fprintf(stderr,"%02X ",uTmp[i]);
			
		}
		fprintf(stderr,"\r\n--------------------------------------------------");
	#endif
	
		//61:
		//if(tag_PPSE[3].uValue[0]!=tag_PPSE[4].uTag[0])
		if(uTmp[0]!=tag_PPSE[4].uTag[0])
		{
			fprintf(stderr,"PPSE,Resp uTmp[0]= %02X",uTmp[0]);
			return (STATUS_PROTOCOL_ERROR) ;
		}
		//tag_PPSE[4].uLength=tag_PPSE[3].uValue[1];
		//memcpy(tag_PPSE[4].uValue,tag_PPSE[3].uValue+2,tag_PPSE[4].uLength);
		tag_PPSE[4].uLength=uTmp[1];
		memcpy(tag_PPSE[4].uValue,uTmp+2,tag_PPSE[4].uLength);
		
		//4F:
		while(1)
		{
			if(tag_PPSE[4].uValue[0]==tag_PPSE[5].uTag[0])
			{
				break;
			}
			fprintf(stderr,"PPSE,Resp tag_PPSE[4].uValue[0]= %02X",tag_PPSE[4].uValue[0]);
			//����������AID������
			uLen_Tmp = uLen_Tmp - tag_PPSE[4].uLength-2;
			if(uLen_Tmp<=0)
			{
				//fprintf(stderr,"\n\r\n\r������ֹ,AID������");
				fprintf(stderr,"\n\r\n\r������ֹ");
				return ERR_NOAID;
			}	
			if(uLen_Tmp<=0)
				break;
					
			memcpy(uTmp2,uTmp + uTmp[1]+2 ,	uLen_Tmp);
			memcpy(uTmp,uTmp2,uLen_Tmp);
			tag_PPSE[4].uLength=uTmp[1];
			memcpy(tag_PPSE[4].uValue,uTmp+2,tag_PPSE[4].uLength);
				
		#ifdef _DEBUG_PPSE_SEL_
			fprintf(stderr,"PPSE,����AID ,uTmp %d�ֽ� >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>:",uLen_Tmp);
			for(i=0;i<uLen_Tmp;i++)
			{
				fprintf(stderr,"%02X ",uTmp[i]);
			}
			fprintf(stderr,"\r\n--------------------------------------------------");
		#endif
		}
		tag_PPSE[5].uLength=tag_PPSE[4].uValue[1];
		memcpy(tag_PPSE[5].uValue,tag_PPSE[4].uValue+2,tag_PPSE[5].uLength);
		//uAID�ṹ:AID����(1)+AID + ���ȼ�(1)
		uAID[uLen_AID_Total] = tag_PPSE[5].uLength;
		memcpy(uAID+1+uLen_AID_Total,tag_PPSE[5].uValue,tag_PPSE[5].uLength);
		uLen_AID_Total = uLen_AID_Total +  LENSINGLEAID + 2;  // AID ��󳤶�16
		memset(g9f06,0x00,sizeof(g9f06));
		memcpy(g9f06,uAID,*uLen_AID);
		//50:��ѡ
		tag_PPSE[6].uLength=0;
		if(tag_PPSE[4].uValue[2+tag_PPSE[5].uLength]==tag_PPSE[6].uTag[0])
		{
			tag_PPSE[6].uLength=tag_PPSE[4].uValue[2+tag_PPSE[5].uLength+1];
			memcpy(tag_PPSE[6].uValue,tag_PPSE[4].uValue+2+tag_PPSE[5].uLength+2,tag_PPSE[6].uLength);
		}
		//87:������
		//tag_PPSE[7].uValue[0] = 0xff;
		tag_PPSE[7].uLength=0;
		if(tag_PPSE[6].uLength==0)
		{
			fprintf(stderr,"PPSE,87:������:%02X %02X %02X",tag_PPSE[4].uValue[tag_PPSE[5].uLength+2+tag_PPSE[6].uLength],tag_PPSE[4].uValue[tag_PPSE[5].uLength+2+tag_PPSE[6].uLength+1],tag_PPSE[4].uValue[tag_PPSE[5].uLength+2+tag_PPSE[6].uLength+2]);
			if(tag_PPSE[4].uValue[tag_PPSE[5].uLength+2+tag_PPSE[6].uLength]==tag_PPSE[7].uTag[0])
			{
				tag_PPSE[7].uLength=tag_PPSE[4].uValue[tag_PPSE[5].uLength+2+tag_PPSE[6].uLength+1];
				memcpy(tag_PPSE[7].uValue,tag_PPSE[4].uValue+tag_PPSE[5].uLength+2+tag_PPSE[6].uLength+2,tag_PPSE[7].uLength);
			}
		}
		else
		{
			if(tag_PPSE[4].uValue[2+tag_PPSE[5].uLength+2+tag_PPSE[6].uLength]==tag_PPSE[7].uTag[0])
			{
				tag_PPSE[7].uLength=tag_PPSE[4].uValue[2+tag_PPSE[5].uLength+2+tag_PPSE[6].uLength+1];
				memcpy(tag_PPSE[7].uValue,tag_PPSE[4].uValue+2+tag_PPSE[5].uLength+2+tag_PPSE[6].uLength+2,tag_PPSE[7].uLength);
			}
		}
		if(tag_PPSE[7].uLength==0)
			break;
		uAID[uLen_AID_Total-1] =tag_PPSE[7].uValue[0];
		uLen_Tmp = uLen_Tmp - tag_PPSE[4].uLength-2;
		if(uLen_Tmp<=0)
			break;
		//���ȼ�
		uLen_place_last = uLen_place_last+ tag_PPSE[4].uLength+2;	
	
		memcpy(uTmp,tag_PPSE[3].uValue + uLen_place_last,	uLen_Tmp);
	#ifdef _DEBUG_PPSE_SEL_
		fprintf(stderr,"tag_PPSE[4].uLength+2= %d�ֽ� uLen_Tmp=%d,",tag_PPSE[4].uLength+2,uLen_Tmp);
		for(i=0;i<uLen_Tmp;i++)
			fprintf(stderr,"%02X ",uTmp[i]);
		fprintf(stderr,"\r\n--------------------------------------------------");
	#endif
	}
#ifdef _DEBUG_PPSE_SEL_
	fprintf(stderr,"PPSE,Resp >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>:");
	for(i=0;i<8;i++)
	{
		if(i==3)
			fprintf(stderr,"T: %02X%02X",tag_PPSE[i].uTag[0],tag_PPSE[i].uTag[1]);
		else
			fprintf(stderr,"T: %02X",tag_PPSE[i].uTag[0]);
		fprintf(stderr,"Describe: %s",tag_PPSE[i].sDescribe);
		fprintf(stderr,"L: %02X",tag_PPSE[i].uLength);
		fprintf(stderr,"V: ");
		for(j=0;j<tag_PPSE[i].uLength;j++)
			fprintf(stderr,"%02X ",tag_PPSE[i].uValue[j]);
		fprintf(stderr,"\r\n--------------------------------------------------");
	}
		
	fprintf(stderr,"\r\n PPSE,AID ���� >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>:\r\n");
	for(i=0;i<uLen_AID_Total;i++)
	{
		fprintf(stderr,"%02X ",uAID[i]);
	}
	fprintf(stderr,"\r\n--------------------------------------------------");

#endif
	*uLen_AID = uLen_AID_Total;
	return 0;
}
unsigned int PBOC_apdu_Ext(T_APDU *C_Apdu , T_APDU *R_Apdu )
{
	unsigned int  nRetCode=0;
	nRetCode = PBOC_apdu(C_Apdu,R_Apdu );
	if (nRetCode!=OK)
		return (nRetCode) ;
	
	if(R_Apdu->data[R_Apdu->length-2]!=0x90 ||R_Apdu->data[R_Apdu->length-1]!=0x00) 
	{
		nRetCode =R_Apdu->data[R_Apdu->length-2];
		nRetCode = nRetCode * 0x100+R_Apdu->data[R_Apdu->length-1];
		return nRetCode;
	}
	return 0;
} 
//#define _DEBUG_PARAMCACERT_
int fdda_L2(UBYTE* uAID)
{
//#ifdef _NOFDDA_
//	//fprintf(stderr,"\n\rNO fdda_l2 \n\r");
//	return 0;
//#endif

	UDWORD	tint0=0;
	UBYTE tint_time[32];

	unsigned uiModuleLen = 144,nLen_PKPICC,nLen_PKPISSUE,nLen_DD;
	unsigned char ucaModule[256];	
	char sTmp[1024];
	unsigned uiExpLen ;
	unsigned char ucaExp[3];
	unsigned char ucaCert[256],uHashValue[20];
	unsigned char uDataIn[512],uDataOut[256],uPublicKey_Issue[256],uPublicKey_PICC[256];
	unsigned int nLen_DataIn=0,nLen_Tmp;
	int i,ret;
	unsigned uiDataLen;
	unsigned char ucaData[256],uData_DD[256],uTmp[256];
	S_TerminalParam_Record* TP_Record;

	//20130827
#ifdef _DEBUG_FDDA_L2_
	fprintf(stderr,"\n\r g9f08:%02x%02x uLen_g9f08=%02x df61=%02x\n\r",g9f08[0],g9f08[1],uLen_g9f08,gdf61);	
#endif
	
	if(uLen_g9f69<=0)
	{
		if((uLen_g9f08>0) && (memcmp(g9f08,"\x00\x20",2)!=0))
			return ERR_FDDA00_9F08;
	}
	else
	{
	
		if(memcmp(g9f69+5,g9f6c,2)!=0)
			return ERR_FDDA01_9F6C;

		if(g9f69[7]!=gdf61)
			return ERR_FDDA01_9F61;

	}
	

	memset(ucaCert, 0x00, 256);
	memcpy(ucaCert,g90,uLen_g90);	
	
	
#ifdef _PARAMCACERT_
	//20130722
	if(g_TP_Head==NULL)
		return ERR_GTPHEADISNULL;
	//20130722
	if(g_TP_Record==NULL)
		return ERR_GTPRECORDISNULL;
	#ifdef _DEBUG_PARAMCACERT_
		printf("\n\r in PARAMCACERT , CAIDX:%02X",g8f);
	#endif
	for(i=0;i<g_TP_Head->RecordNum[0];i++)
	{
		TP_Record = (S_TerminalParam_Record *)((UBYTE*)g_TP_Record+sizeof(S_TerminalParam_Record)*i);
		//printf("\n\r i=%d,TP_Record->CAIndex=%02x ,TP_Record->RID:%02x\n\r",i,TP_Record->CAIndex,TP_Record->RID[0]);
		if((memcmp(uAID,TP_Record->RID,5)==0) && (g8f==TP_Record->CAIndex))
			break;
		
		//printf("\n\r after break,TP_Record->RID:%02x\n\r",TP_Record->RID[0]);
		
	}
	#ifdef _DEBUG_PARAMCACERT_
		printf("\n\r out of for loop ,i=%d,TP_Record->CAIndex=%02x \n\r",i,TP_Record->CAIndex);
	#endif
	if(i>=g_TP_Head->RecordNum[0])
	{
		printf("\n\rTransation Reject , NOCAIDX:%02X",g8f);
		
		return ERR_NOCAIDX;							
	}
	#ifdef _DEBUG_PARAMCACERT_
		printf("\n\r ucaExp=%02X,TP_Record->ModuleLen=%02X%02X :\n\r",TP_Record->CAExp[0],TP_Record->ModuleLen[0],TP_Record->ModuleLen[1]);	
		for(i=0;i<256;i++)
		{
			printf("%02X",TP_Record->Module[i]);
			if((i+1)% 16==0)
				printf("\n\r");
		
		}
		printf("\n\r");
	#endif
	uiExpLen=1;
	//20130724
	ucaExp[2]=TP_Record->CAExp[1];
	ucaExp[1]=TP_Record->CAExp[2];
	ucaExp[0]=TP_Record->CAExp[3];
	//memcpy(ucaExp,TP_Record->CAExp,1);
	//memcpy(ucaExp,TP_Record->CAExp,1);
	uiModuleLen = TP_Record->ModuleLen[0]*0x100+TP_Record->ModuleLen[1];
	memcpy(ucaModule,TP_Record->Module,uiModuleLen);
	#ifdef _DEBUG_PARAMCACERT_
		printf("\n\r ucaExp=%d,uiModuleLen=%d :\n\r",ucaExp[0],uiModuleLen);	
		for(i=0;i<uiModuleLen;i++)
		{
			printf("%02X",ucaModule[i]);
			if((i+1)% 16==0)
				printf("\n\r");
		
		}
		printf("\n\r");
	#endif
#else	
	printf("\n\r In default CA 20130717 \n\r");
	sprintf(sTmp,"B61645EDFD5498FB246444037A0FA18C0F101EBD8EFA54573CE6E6A7FBF63ED21D66340852B0211CF5EEF6A1CD989F66AF21A8EB19DBD8DBC3706D135363A0D683D046304F5A836BC1BC632821AFE7A2F75DA3C50AC74C545A754562204137169663CFCC0B06E67E2109EBA41BC67FF20CC8AC80D7B6EE1A95465B3B2657533EA56D92D539E5064360EA4850FED2D1BF");
	uiExpLen=1;
	memcpy(ucaExp,"\x03",1);
	gStr2Bytes(strlen(sTmp),sTmp,ucaModule);
	uiModuleLen = strlen(sTmp)/2;
#endif

#ifdef _DEBUG_FDDA_L2_
	fprintf(stderr,"\n\rAID:%02X%02X%02X%02X%02X,",uAID[0],uAID[1],uAID[2],uAID[3],uAID[4]);
	fprintf(stderr,"\n\rCA:  uiExpLen=%d:\n,",uiExpLen);
	for( i=0;i<uiExpLen;i++)
	{
		fprintf(stderr,"%02X ",ucaExp[i]);
	}
	fprintf(stderr,"\x0d\x0a uiModuleLen=%d:\n,",uiModuleLen);
	for( i=0;i<uiModuleLen;i++)
	{
		fprintf(stderr,"%02X ",ucaModule[i]);
	}
	fprintf(stderr,"\n\r");
#endif
	
	
#ifdef _DEBUGEXETIME_
	gettimeofday(&start,NULL);
	Lib_Rsa(ucaModule,uiModuleLen,ucaExp,uiExpLen,ucaCert,ucaData);

	gettimeofday(&end,NULL);
	timer = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
	fprintf(stderr,"rsa timer = %ld us\n",timer);
#else
	#ifdef _DEBUG_FDDA_L2_
		fprintf(stderr,"\x0d\x0a ucaCertLen=%d:\n,",uLen_g90);
		for(i=0;i<uLen_g90;i++)
		{
			fprintf(stderr,"%02X ",ucaCert[i]);
			if((i+1) % 16==0)
			fprintf(stderr,"\x0d\x0a ");

		}
		fprintf(stderr,"\x0d\x0a ");
	#endif
	//ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x02\x00", 3); //2013/11/16 19:25:54
	//ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x02\x01", 3); //2013/11/16 19:25:54
	//memset(tint_time,0,sizeof(tint_time));
	//tint0 = GetTickcount();
	Lib_Rsa(ucaModule,uiModuleLen,ucaExp,uiExpLen,ucaCert,ucaData);
	//tint0 = GetTickcount() - tint0;
	//udword_to_buf(tint0,&tint_time[4]);
	//memcpy(tint_time,"\x01\x02\x03\x04",4);
	//ReaderResponse(csc_comm, 0x00, 0xF0, tint_time, 7); //2013/11/16 19:25:54
#endif
	
	uiDataLen = uiModuleLen;
#ifdef _DEBUG_FDDA_L2_
	for( i=0;i<uiDataLen;i++)
	{
		fprintf(stderr,"%02X ",ucaData[i]);
		if((i+1)%16==0)
			fprintf(stderr,"\n");
	}
#endif
	if(ucaData[0]!=0x6a)
	{
		return ERR_NOT6A;
	}	
	if(ucaData[uiDataLen-1]!=0xBC)
	{
		
		return ERR_NOT6A;
	}	
	
	if(ucaData[1]!=0x02)
	{
		return ERR_DATAFORMAT;
	}	
	
	memset(uTmp,0x00,sizeof(uTmp));
	memset(sTmp,0x00,sizeof(sTmp));
	gBCD2Str(4,ucaData+2,sTmp);
	gBCD2Str(4,g5a,(char*)uTmp);
  	for(nLen_Tmp=0;nLen_Tmp<8;nLen_Tmp++)
  	{
  		if(sTmp[nLen_Tmp]=='f' || sTmp[nLen_Tmp]=='F')
  			break;
  	}

	if(nLen_Tmp>0)
	{
		if(memcmp(sTmp,uTmp,nLen_Tmp)!=0)
		{
			return ERR_DATAFORMAT;
		}	
	}

	uTmp[0]=ucaData[7];
	uTmp[1]=ucaData[6];
#ifdef _DEBUG_FDDA_L2_
	fprintf(stderr,"g9a=%02x%02x%02x,ucaData[7,6]%02x%02x,memcmp(g9a,uTmp,2)=%d,memcmp(uTmp,g9a,2)=%d\n",g9a[0],g9a[1],g9a[2],ucaData[7],ucaData[6],memcmp(g9a,uTmp,2),memcmp(uTmp,g9a,2));
#endif
	if(memcmp(g9a,uTmp,2)>0)
	{
		fprintf(stderr,"\n\r Invalid CA.");
		//20130514 DEBUG KILL
		return ERR_g5f24;
	}
	
	if(ucaData[11]!=1)
	{
		return ERR_g5f24;
	}
	if(ucaData[12]!=1)
	{
		return ERR_g5f24;
	}
	nLen_PKPISSUE = ucaData[13];
	
	nLen_DataIn = uiDataLen-20-2;	
	memset(uDataIn,0x00,sizeof(uDataIn));
	memcpy(uDataIn,ucaData+1,nLen_DataIn);
	
	memset(uDataOut,0x00,sizeof(uDataOut));
	memset(uHashValue,0x00,sizeof(uHashValue));
	memcpy(uHashValue,ucaData+uiDataLen-20-1,20);
#ifdef _DEBUG_FDDA_L2_
	fprintf(stderr,"\n\r��ϣ:\n");
	for( i=0;i<20;i++)
	{
		fprintf(stderr,"%02X ",uHashValue[i]);
	}
	
#endif
	
	memcpy(uDataIn+nLen_DataIn,g92,uLen_g92);
	nLen_DataIn =nLen_DataIn+uLen_g92;
	memcpy(uDataIn + nLen_DataIn,g9f32,uLen_g9f32);
	nLen_DataIn = nLen_DataIn + uLen_g9f32;
#ifdef _DEBUG_FDDA_L2_
	for( i=0;i<nLen_DataIn;i++)
	{
		fprintf(stderr,"%02X ",uDataIn[i]);
		if((i+1)%16==0)
			fprintf(stderr,"\n");
	}
#endif

	//Lib_Hash(uDataIn,nLen_DataIn,uDataOut);
	sha(uDataIn,nLen_DataIn,uDataOut);

#ifdef _DEBUG_FDDA_L2_
	fprintf(stderr,"\x0d\x0a:\n");
	for( i=0;i<20;i++)
		fprintf(stderr,"%02X ",uDataOut[i]);
#endif		
	if(memcmp(uHashValue,uDataOut,20))
	{
		fprintf(stderr,"\x0d\x0a ERR HASH ISSUE \n");
		return ERR_HASH_ISSUE;
	}

	memset(uPublicKey_Issue,0x00,sizeof(uPublicKey_Issue));
	memcpy(uPublicKey_Issue,uDataIn+14,nLen_PKPISSUE);
	
#ifdef _DEBUG_FDDA_L2_
	fprintf(stderr,"\x0d\x0a(%d�ֽ�):\n",nLen_PKPISSUE);
	for( i=0;i<128;i++)
	{
		fprintf(stderr,"%02X ",uPublicKey_Issue[i]);
		if((i+1)%16==0)
			fprintf(stderr,"\n");
	}
	
	fprintf(stderr,"\x0d\x0a gSDFORAUTH(%d�ֽ�):\n",uLen_gSDFORAUTH);
	for( i=0;i<uLen_gSDFORAUTH;i++)
	{
		fprintf(stderr,"%02x ",gSDFORAUTH[i]);
		if((i+1)%16==0)
			fprintf(stderr,"\n");
	}
#endif
	
#ifdef _DEBUG_FDDA_L2_
	fprintf(stderr,"\n\r2.------------------------------");	
#endif
	memset(ucaCert, 0x00, 256);
	memcpy(ucaCert,g9f46,uLen_g9f46);
	uiExpLen=uLen_g9f32;
	memcpy(ucaExp,g9f32,uLen_g9f32);
	
	uiDataLen=0;
	uiModuleLen=nLen_PKPISSUE;  
	memset(ucaData, 0x00, 256);
	memset(ucaModule, 0x00, 256);
	memcpy(ucaModule,uPublicKey_Issue,nLen_PKPISSUE);
	//tint0 = GetTickcount();
	Lib_Rsa(ucaModule,uiModuleLen,ucaExp,uiExpLen,ucaCert,ucaData);
	//tint0 = GetTickcount() - tint0;
	//udword_to_buf(tint0,&tint_time[8]);
	uiDataLen = uiModuleLen;
#ifdef _DEBUG_FDDA_L2_
	for( i=0;i<uiDataLen;i++)
	{
		fprintf(stderr,"%02X ",ucaData[i]);
		if((i+1)%16==0)
			fprintf(stderr,"\n");
	}
#endif
	if(ucaData[0]!=0x6a)
	{
		return ERR_NOT6A;
	}	
	
	if(ucaData[uiDataLen-1]!=0xBC)
	{
		
		return ERR_NOT6A;
	}	
	
	if(ucaData[1]!=0x04)
	{
		return ERR_DATAFORMAT;
	}	
	
	
	memset(uTmp,0x00,sizeof(uTmp));
	memset(sTmp,0x00,sizeof(sTmp));
	gBCD2Str(10,ucaData+2,sTmp);
	gBCD2Str(uLen_g5a,g5a,(char*)uTmp);
  	for(nLen_Tmp=0;nLen_Tmp<20;nLen_Tmp++)
  	{
  		if(sTmp[nLen_Tmp]=='f' || sTmp[nLen_Tmp]=='F')
  			break;
  	}

	if(nLen_Tmp>0)
	{
		if(memcmp(sTmp,uTmp,nLen_Tmp)!=0)
		{
			return ERR_NOPATCHPAN;
		}	
	}
	
	uTmp[0]=ucaData[13];
	uTmp[1]=ucaData[12];
	if(memcmp(g9a,uTmp,2)>0)
	{
		return ERR_g5f24;
	}
	if(ucaData[17]!=1)
	{
		return ERR_HASHID;
	}
	if(ucaData[18]!=1)
	{
		return ERR_ISSUEID;
	}
	if(ucaData[19]!=uLen_g9f4b)
	{
		return ERR_9f4b_EXT;
	}
	
	memcpy(uHashValue,ucaData+uiDataLen-20-1,20);

#ifdef _DEBUG_FDDA_L2_
	fprintf(stderr,"\n\r:\n");
	for( i=0;i<20;i++)
		fprintf(stderr,"%02X ",uHashValue[i]);
	
#endif
	nLen_PKPICC = ucaData[19] ;	

	nLen_DataIn = uiDataLen-20-2;	//gwl
	memset(uDataIn,0x00,sizeof(uDataIn));
	memcpy(uDataIn,ucaData+1,nLen_DataIn);
	memset(uDataOut,0x00,sizeof(uDataOut));
	memcpy(uDataIn+nLen_DataIn,g9f48,uLen_g9f48);
	nLen_DataIn = nLen_DataIn+uLen_g9f48;
	
#ifdef _DEBUG_FDDA_L2_
	fprintf(stderr,"\x0d\x0a tag 9f47[%dBYTES]:%02X\n",uLen_g9f47,g9f47[0]);
#endif
	memcpy(uDataIn+nLen_DataIn,g9f47,uLen_g9f47);
	nLen_DataIn = nLen_DataIn+uLen_g9f47;
	
	
	memcpy(uDataIn+nLen_DataIn,gSDFORAUTH,uLen_gSDFORAUTH);
	nLen_DataIn = nLen_DataIn+uLen_gSDFORAUTH;

	
	if(uLen_g9f4a>0)
	{
		memcpy(uDataIn + nLen_DataIn ,g82,2);
		nLen_DataIn = nLen_DataIn+2;
	}
#ifdef _DEBUG_FDDA_L2_
	fprintf(stderr,"\x0d\x0a HASH Data[%dBYTES]:\n",nLen_DataIn);
	for( i=0;i<nLen_DataIn;i++)
	{
		fprintf(stderr,"%02x ",uDataIn[i]);
		if((i+1)%16==0)
			fprintf(stderr,"\n");
	}
	
#endif
	sha(uDataIn,nLen_DataIn,uDataOut);

#ifdef _DEBUG_FDDA_L2_
	fprintf(stderr,"\x0d\x0a HASH Result :\n");
	for( i=0;i<20;i++)
		fprintf(stderr,"%02X ",uDataOut[i]);
#endif
	if(memcmp(uHashValue,uDataOut,20))
	{
		fprintf(stderr,"\x0d\x0a ERR HASH PICC00 ");
		return ERR_HASH_PICC;
	}

	memset(uPublicKey_PICC,0x00,sizeof(uPublicKey_Issue));
	memcpy(uPublicKey_PICC,uDataIn+20,nLen_PKPICC);
	
#ifdef _DEBUG_FDDA_L2_
	fprintf(stderr,"\x0d\x0a(%d�ֽ�):\n",nLen_PKPICC);
	for( i=0;i<nLen_PKPICC;i++)
	{
		fprintf(stderr,"%02X ",uPublicKey_PICC[i]);
		if((i+1)%16==0)
			fprintf(stderr,"\n");
	}

	
#endif
	memset(ucaCert, 0x00, 256);
	memcpy(ucaCert,g9f4b,uLen_g9f4b);
	
	uiExpLen=uLen_g9f47;
	memcpy(ucaExp,g9f47,uiExpLen);

	uiDataLen=0;
	uiModuleLen=nLen_PKPICC;
	memset(ucaData, 0x00, 256);
	memset(ucaModule, 0x00, 256);
	memcpy(ucaModule,uPublicKey_PICC,nLen_PKPICC);
  //tint0 = GetTickcount();
	Lib_Rsa(ucaModule,uiModuleLen,ucaExp,uiExpLen,ucaCert,ucaData);
	//tint0 = GetTickcount() - tint0;
	//udword_to_buf(tint0,&tint_time[12]);
	//ReaderResponse(csc_comm, 0x00, 0xF0, tint_time, 16); //2013/11/16 19:25:54
	uiDataLen = uiModuleLen;
#ifdef _DEBUG_FDDA_L2_
	for( i=0;i<uiDataLen;i++)
	{
		fprintf(stderr,"%02X ",ucaData[i]);
		if((i+1)%16==0)
			fprintf(stderr,"\n");
	}
#endif
	if(ucaData[0]!=0x6a)
	{
		return ERR_NOT6A;
	}	
	
	if(ucaData[uiDataLen-1]!=0xBC)
	{
		return ERR_NOTBC;
	}	
	if(ucaData[1]!=0x05)
	{
		return ERR_DATAFORMAT;
	}	
	
	memcpy(uHashValue,ucaData+uiDataLen-20-1,20);
#ifdef _DEBUG_FDDA_L2_
	fprintf(stderr,"\n\r:\n");
	for( i=0;i<20;i++)
		fprintf(stderr,"%02X ",uHashValue[i]);
#endif	
	nLen_DD = ucaData[3] ;	
	memcpy(uData_DD,ucaData+4,nLen_DD);
	nLen_DataIn = uiDataLen-20-2;	
	memset(uDataIn,0x00,sizeof(uDataIn));
	memcpy(uDataIn,ucaData+1,nLen_DataIn);
	memset(uDataOut,0x00,sizeof(uDataOut));
	
	//fdda01
	if(uLen_g9f69>0)
	{
		fprintf(stderr,"\x0d\x0a uLen_g9f69=%d \n",uLen_g9f69);
		fprintf(stderr,"\x0d\x0a ugRand=%02x%02x%02x%02x \r\n",ugRand[0],ugRand[1],ugRand[2],ugRand[3]);
		fprintf(stderr,"\x0d\x0a g9f02=%02x%02x%02x%02x%02x%02x \r\n",g9f02[0],g9f02[1],g9f02[2],g9f02[3],g9f02[4],g9f02[5]);
		fprintf(stderr,"\x0d\x0a g5f2a=%02x%02x \r\n",g5f2a[0],g5f2a[1],g9f02[2]);
		fprintf(stderr,"\x0d\x0a g9f69=%02x%02x%02x%02x%02x%02x%02x%02x \r\n",g9f69[0],g9f69[1],g9f69[2],g9f69[3],g9f69[4],g9f69[5],g9f69[6],g9f69[7]);
		memcpy(uDataIn +nLen_DataIn,  ugRand	,4);
		nLen_DataIn = nLen_DataIn+4;

		memcpy(uDataIn +nLen_DataIn,  g9f02_BAK,6);
		nLen_DataIn = nLen_DataIn+6;

		memcpy(uDataIn +nLen_DataIn,  g5f2a	,2);
		nLen_DataIn = nLen_DataIn+2;

		memcpy(uDataIn +nLen_DataIn,  g9f69	,uLen_g9f69);
		nLen_DataIn = nLen_DataIn + uLen_g9f69;
	}
	else
	{
	 
		memcpy(uDataIn +nLen_DataIn,  ugRand	,4);
		nLen_DataIn = nLen_DataIn+4;
	}
#ifdef _DEBUG_FDDA_L2_
	fprintf(stderr,"\x0d\x0a HASH Data:\n");
	for( i=0;i<nLen_DataIn;i++)
	{
		fprintf(stderr,"%02x ",uDataIn[i]);
		if((i+1)%16==0)
			fprintf(stderr,"\n");
	}
#endif	
	//Lib_Hash(uDataIn,nLen_DataIn,uDataOut);
	sha(uDataIn,nLen_DataIn,uDataOut);

#ifdef _DEBUG_FDDA_L2_
	fprintf(stderr,"\x0d\x0a:\n");
	for( i=0;i<20;i++)
		fprintf(stderr,"%02X ",uDataOut[i]);
	fprintf(stderr,"\x0d\x0a");
#endif
	if(memcmp(uHashValue,uDataOut,20))
	{
		
		return ERR_HASH_DD;
	}
	
#ifdef _DEBUG_FDDA_L2_
	fprintf(stderr,"\x0d\x0a fdda:last Data_DD(%d BYTES):\x0d\x0a",nLen_DD);
	for( i=0;i<nLen_DD;i++)
	{
		fprintf(stderr,"%02X ",uData_DD[i]);
		if((i+1)%16==0)
			fprintf(stderr,"\n");
	}
  	fprintf(stderr,"\x0d\x0a----------------------------\x0d\x0a");   
#endif
	return 0;
}

void Patchdata(UBYTE* D_Data,UBYTE* s_Data,UBYTE uLen_Std, UBYTE l_tlv,UBYTE uLeftorRight,UBYTE uWhat)
{
	UBYTE uTmp[255];
	
	memset(uTmp,uWhat,sizeof(uTmp)); 
	if(l_tlv<=uLen_Std)
	{
		memcpy(D_Data,s_Data,l_tlv);
	}
	else
	{
		if(uLeftorRight==0)
		{
			memcpy(D_Data,uTmp,l_tlv-uLen_Std);
			memcpy(D_Data+l_tlv-uLen_Std,s_Data,uLen_Std);
		}
		else
		{
			memcpy(D_Data,s_Data,uLen_Std);
			memcpy(D_Data+uLen_Std,uTmp,l_tlv-uLen_Std);
		}
	}
	return ;
	
}

//#define _DEBUG_Parse_AIDRESP_ 
int Parse_AIDRESP(UBYTE* uInData,int nLen_InData)
{
	int i,nTmp=0;
	UBYTE uFlag=0;
	UBYTE l_tlv,uTmp[32];
	
	GetTagFrmTLV(uInData,nLen_InData);
	GetTagFrmTLV(g6f,uLen_g6f);
	GetTagFrmTLV(ga5,uLen_ga5);
	GetTagFrmTLV(gbf0c,uLen_gbf0c);
	
	if(uLen_g9f38==0)
	{
		return ERR_PDOL0;
	}

	for(i=0;i<uLen_g9f38;i++)
	{
		if(g9f38[i]==0x9f && g9f38[i+1]==0x66 )
		{
			uFlag = 1;
			break;
		}	
	}
	if(uFlag ==0)
	{
		return ERR_NOTAG9f66;
	}
	if(gdf61!=0x01 && gdf61!=0x02 && gdf61!=0x81 && gdf61!=0x82)
	{
		return ERR_NOdf61;
	}
	return 0;
}
int GenPDOL(UBYTE* uOutData,int* nLen_OutData)
{
	int i,nTmp=0;
	UBYTE uFlag=0;
	UBYTE l_tlv,uTmp[32];
	
	memset(uTmp,0x00,sizeof(uTmp));
	nTmp = 0;
	for(i=0;i<uLen_g9f38;i++)
	{
		if(g9f38[i]==0x9f)
		{
			
			if(g9f38[i+1]==0x36)
			{
				memcpy(uOutData+nTmp,g9f36,g9f38[i+2]);
				nTmp = nTmp + g9f38[i+2];
			}
			if(g9f38[i+1]==0x37)
			{
				memcpy(uOutData+nTmp,ugRand,g9f38[i+2]);
				nTmp = nTmp + g9f38[i+2];
			}
			
			if(g9f38[i+1]==0x66)
			{
				memcpy(uOutData+nTmp,g9f66_PCD,g9f38[i+2]);
				nTmp = nTmp + g9f38[i+2];
			}
			
			if(g9f38[i+1]==0x01)
			{
				memcpy(uOutData+nTmp,g9f01,g9f38[i+2]);
				nTmp = nTmp + g9f38[i+2];
			}
			
			if(g9f38[i+1]==0x02)
			{
				l_tlv = g9f38[i+2];
				
				Patchdata(uOutData+nTmp,g9f02,6,l_tlv,0,0x00);
				nTmp = nTmp + l_tlv;
			}
			if(g9f38[i+1]==0x03)
			{
				l_tlv = g9f38[i+2];
				
				Patchdata(uOutData+nTmp,g9f03,6,l_tlv,0,0x00);
				nTmp = nTmp + l_tlv;
			}
			
			if(g9f38[i+1]==0x04)
			{
				l_tlv = g9f38[i+2];
				
				Patchdata(uOutData+nTmp,g9f04,4,l_tlv,0,0x00);
				nTmp = nTmp +l_tlv;
			}
			
			if(g9f38[i+1]==0x06)
			{
				l_tlv = g9f38[i+2];
				Patchdata(uOutData+nTmp,g9f06,LENSINGLEAID,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;
			}
			
			
			if(g9f38[i+1]==0x15)
			{
				
				l_tlv = g9f38[i+2];
				
				Patchdata(uOutData+nTmp,g9f15,2,l_tlv,1,0x00);
				
				nTmp = nTmp + l_tlv;
				
			}
			
			if(g9f38[i+1]==0x16)
			{
				
				l_tlv = g9f38[i+2];
				Patchdata(uOutData+nTmp,g9f16,15,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;
			}
			
			if(g9f38[i+1]==0x1a)
			{
				
				l_tlv = g9f38[i+2];
				Patchdata(uOutData+nTmp,g9f1a,2,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;
				
			}
			
			
			if(g9f38[i+1]==0x1b)
			{
				l_tlv = g9f38[i+2];
				Patchdata(uOutData+nTmp,g9f1b,4,l_tlv,0,0x00);
				nTmp = nTmp + l_tlv;
				
			}
			if(g9f38[i+1]==0x1c)
			{
				l_tlv = g9f38[i+2];
				Patchdata(uOutData+nTmp,g9f1c,8,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;
				
			}
			
			if(g9f38[i+1]==0x1d)
			{
				l_tlv = g9f38[i+2];
				Patchdata(uOutData+nTmp,g9f1d,15,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;				
			}
			
			
			if(g9f38[i+1]==0x1e)
			{
			
				l_tlv = g9f38[i+2];
				Patchdata(uOutData+nTmp,g9f1e,8,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;
			}
			
			if(g9f38[i+1]==0x21)
			{
				l_tlv = g9f38[i+2];
				Patchdata(uOutData+nTmp,g9f21,3,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;
				
			}
			
			
			
			if(g9f38[i+1]==0x33)
			{
				l_tlv = g9f38[i+2];
				Patchdata(uOutData+nTmp,g9f33,3,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;			
			}
			
			
			if(g9f38[i+1]==0x34)
			{
				l_tlv = g9f38[i+2];
	#ifdef _DEBUG_Parse_AIDRESP_
		fprintf(stderr,"����9f34(%d�ֽ�):",l_tlv);
	#endif
				Patchdata(uOutData+nTmp,g9f34,3,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;
				
			}
			
			
			if(g9f38[i+1]==0x35)
			{
				l_tlv = g9f38[i+2];
				Patchdata(uOutData+nTmp,&g9f35,1,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;
				
			}
			
			
			if(g9f38[i+1]==0x39)
			{
				l_tlv = g9f38[i+2];
				Patchdata(uOutData+nTmp,&g9f39,1,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;
			}
			
			
			if(g9f38[i+1]==0x3a)
			{
				l_tlv = g9f38[i+2];
				Patchdata(uOutData+nTmp,g9f3a,4,l_tlv,0,0x00);
				nTmp = nTmp + l_tlv;
				
			}
			
			
			
			if(g9f38[i+1]==0x3c)
			{
				l_tlv = g9f38[i+2];
				Patchdata(uOutData+nTmp,g9f3c,2,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;
				
			}
			
			
			if(g9f38[i+1]==0x3d)
			{
				l_tlv = g9f38[i+2];
				Patchdata(uOutData+nTmp,&g9f3d,1,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;
				
			}
			
			
			if(g9f38[i+1]==0x40)
			{
				
				l_tlv = g9f38[i+2];
				Patchdata(uOutData+nTmp,g9f40,5,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;
				
			}
			
			if(g9f38[i+1]==0x41)
			{
			
				l_tlv = g9f38[i+2];
	#ifdef _DEBUG_Parse_AIDRESP_
		fprintf(stderr,"����9f41(%d�ֽ�):",l_tlv);
	#endif
				Patchdata(uOutData+nTmp,g9f41,5,l_tlv,0,0x00);
				nTmp = nTmp + l_tlv;
			}
			
			if(g9f38[i+1]==0x4E)
			{
				//����9f4e
				l_tlv = g9f38[i+2];
				Patchdata(uOutData+nTmp,g9f4e,20,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;
				
			}
			
		}
		
		if(g9f38[i]==0x81 && g9f38[i+1]==0x04 )
		{
			//����81 
	#ifdef _DEBUG_Parse_AIDRESP_
		fprintf(stderr,"����81(%d�ֽ�):",g9f38[i+1]);
	#endif
			l_tlv = g9f38[i+1];
			Patchdata(uOutData+nTmp,g81,4,l_tlv,0,0x00);
			nTmp = nTmp + l_tlv;
		}
		
		if(g9f38[i]==0x95)
		{
			//����95 
				
			l_tlv = g9f38[i+1];
			Patchdata(uOutData+nTmp,g95,5,l_tlv,1,0x00);
			nTmp = nTmp + l_tlv;
			
		}
		
		if(g9f38[i]==0x9a)
		{
			if(g9f38[i+1]<=3)
			{
				memcpy(uOutData+nTmp,g9a+(3-g9f38[i+1]),g9f38[i+1]);
				nTmp = nTmp + g9f38[i+1];
			}
			else
			{
				l_tlv = g9f38[i+1];
				Patchdata(uOutData+nTmp,g9a,3,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;
			}
		}
		if(g9f38[i]==0x9b)
		{
			
			l_tlv = g9f38[i+1];
			Patchdata(uOutData+nTmp,g9b,2,l_tlv,1,0x00);
			nTmp = nTmp + l_tlv;
		}
		
		if(g9f38[i]==0x9c)
		{
			
			l_tlv = g9f38[i+1];
			Patchdata(uOutData+nTmp,&g9c,1,l_tlv,1,0x00);
			nTmp = nTmp + l_tlv;
		}

		if(g9f38[i]==0x5f)
		{
			if(g9f38[i+1]==0x2a)
			{
				//����5f2a
				
				l_tlv = g9f38[i+2];
				Patchdata(uOutData+nTmp,g5f2a,2,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;
			
			}
			
			if(g9f38[i+1]==0x30)
			{
				//����5f30
				l_tlv = g9f38[i+2];
	#ifdef _DEBUG_Parse_AIDRESP_
		fprintf(stderr,"����5f30(%d�ֽ�):",l_tlv);
	#endif
			
				Patchdata(uOutData+nTmp,g5f30,l_tlv,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;
			
			}
			
			if(g9f38[i+1]==0x36)
			{
				//����5f36
				
				l_tlv = g9f38[i+2];
				Patchdata(uOutData+nTmp,&g5f36,1,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;				
			}
			
			
		}	
		
		if(g9f38[i]==0xbf)
		{
			if(g9f38[i+1]==0x0c)
			{
				
				uLen_gbf0c = g9f38[i+2];
				l_tlv = g9f38[i+2];
				Patchdata(uOutData+nTmp,gbf0c,222,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;
				
			}
			
		}	
		
		//a5
		if(g9f38[i]==0xa5  )
		{
	
				//����a5
	#ifdef _DEBUG_Parse_AIDRESP_
		fprintf(stderr,"����a5(%d�ֽ�):",l_tlv);
	#endif
				l_tlv = g9f38[i+1];
				Patchdata(uOutData+nTmp,gdf1a,l_tlv,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;
		}
		
		if(g9f38[i]==0xdf && g9f38[i+1]==0x60 && g9f38[i+2]==0x01)
		{
				gdf60=0x01;
				memcpy(uOutData+nTmp,&gdf60,1);
				nTmp = nTmp + 1;
				
		}
		
		if(g9f38[i]==0xdf && g9f38[i+1]==0x1a )
		{
				//����df1a
				l_tlv = g9f38[i+2];
	#ifdef _DEBUG_Parse_AIDRESP_
		fprintf(stderr,"����df1a(%d�ֽ�):",l_tlv);
	#endif
				
				Patchdata(uOutData+nTmp,gdf1a,l_tlv,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;
		}
		if(g9f38[i]==0x9f && g9f38[i+1]==0x80 )
		{
				//����df1a
				l_tlv = g9f38[i+2];
	#ifdef _DEBUG_Parse_AIDRESP_
		fprintf(stderr,"����9f80(%d�ֽ�):",l_tlv);
	#endif
				
				Patchdata(uOutData+nTmp,gdf1a,l_tlv,l_tlv,1,0x00);
				nTmp = nTmp + l_tlv;
		}
	
	}
	
	*nLen_OutData = nTmp;
	return 0;
}
void GenRand(struct  timeval start_time)
{
	UDWORD dwgCmdExeTime,nTmp;
	UBYTE u1[4],u2[4],i;
	struct  timeval TwoTime;
	gettimeofday(&TwoTime,NULL);
	dwgCmdExeTime = 1000000 * (TwoTime.tv_sec-start_time.tv_sec)+ TwoTime.tv_usec-start_time.tv_usec;


	nTmp =TwoTime.tv_sec-start_time.tv_sec+start_time.tv_usec;
	memcpy(u1,&nTmp,4);
	memcpy(u2,&dwgCmdExeTime,4);
	for(i=0;i<4;i++)
	{
		ugRand[i] = u1[i] ^ u2[i] ;
	}
	ugRand[3] =( gAPPAount_SN[0] | gAPPAount_SN[1] ^ gAPPAount_SN[2] ^  gAPPAount_SN[3] ) ^ u1[1];
	return ;
}


void GenRand8(struct  timeval start_time)
{
	UDWORD dwgCmdExeTime,nTmp;
	UBYTE u1[4],u2[4],i;
	struct  timeval TwoTime;
	gettimeofday(&TwoTime,NULL);
	dwgCmdExeTime = 100090 * (TwoTime.tv_sec-start_time.tv_sec)+ TwoTime.tv_usec-start_time.tv_usec+432;


	nTmp =TwoTime.tv_sec-start_time.tv_sec+TwoTime.tv_usec;
	memcpy(u1,&nTmp,4);
	memcpy(u2,&dwgCmdExeTime,4);
	memcpy(ugRand_RMAC,u1,4);
	memcpy(ugRand_RMAC+4,u2,4);
	ugRand_RMAC[6]= ugRand_RMAC[2] ^ ugRand_RMAC[4]; 
	ugRand_RMAC[7] =( gAPPAount_SN[0] | gAPPAount_SN[1] ^ gAPPAount_SN[2] ^  gAPPAount_SN[3] ) ^ u1[1];
	return ;
}

int initparam()
{
	
	uLen_g9f46 =0;
	uLen_g9f32 =0;
	uLen_g9f47=0;
	uLen_g8f=0;
	uLen_g90=0;
	uLen_g5a =0;
	uLen_g9f4b=0;
	uLen_g9f48=0;
	
	//2012-3-6
	uLen_g90=0;
	uLen_g92=0;
	uLen_g9f32=0;
	uLen_gSDFORAUTH=0;
	uLen_g9f4a=0;
	uLen_g9f49=0;
	uLen_g9f4b=0;
	uLen_g9f10=0;
	uLen_gbf0c=0;
	uLen_g8f=0;
	uLen_g5a=0;
	uLen_g84=0;
	uLen_ga5=0;
	uLen_g50=0;
	uLen_g5f2d=0;
	uLen_g9f12=0;
	uTagRepeat_5a=0x00;
	uTagRepeat_57=0x00;
	uTagRepeat_5f24=0x00;
	uLen_g57 = 0;
	uLen_g57_gpo = 0;
	
	uLen_g6f=0;
	uLen_g82=0;
	uLen_g94=0;
	uLen_g9f38 =0;
	uLen_g93=0;//20130516
	memset(g5f24,0x00,sizeof(g5f24));
	//9f37 
  //	memcpy(ugRand,"\x01\x02\x03\x04",4);
	memcpy(g9f66_PCD,"\x28\x00\x00\x80",4);	
	memset(g9f02,0x00,6);
	//memcpy(g9f02,"\x00\x00\x00\x00\x09\x99",6);	
	memcpy(g9f03,"\x00\x00\x00\x00\x00\x00",6);	
	
	memcpy(g95,"\x00\x00\x00\x00\x00",5);	
	memcpy(g5f2a, "\x01\x56",2);
	//memcpy (g9a, "\x12\x01\x10",3); 
	memset(g9a,0x00,sizeof(g9a));
	g9c=0x00;//��������
	
	memcpy(g9f01,"\x31\x01\x01\x00\x00\x09",6);
	memcpy(g9f40,"\x20\x00\x00\x00\x00",5);
	memcpy(g9f34,"\x00\x00\x00",3);
	
	memset(g9f15,0x00,sizeof(g9f15));
	memset(g9f16,0x00,sizeof(g9f16));
	memset(g9f33,0x00,sizeof(g9f33));
	
	gMaxM_Online=1000;	
	memcpy(g81,"\x00\x00\x03\xe8",4);
	
	memcpy(g9f34,"\x00\x00\x00",3);
	
	memcpy(g9f04,"\x00\x00\x00\x00",4);
	sprintf((char*)g9f4e,"�Ϻ������ͨ");
	memcpy(g9f33,"\x00\x00\xc8",3);		
	g9f39=0x07;
	
	memcpy(g9f1a,"\x01\x56",2);	
	memcpy(g9f1b,"\x00\x00\x03\xe8",4);
	sprintf((char*)g9f1c,"12345678");
	sprintf((char*)g9f1e,"12345678");
	sprintf((char*)g9f16,"123456789012345");
	g9f35 = 0x26;
	memcpy(g9f3c,"\x01\x56",2);
	g9f3d =0x02;
	//memset(g9f41,0x00,sizeof(g9f41));
	//memcpy(g9f41,"\x00\x00\x00\x01",4);
	//ReadTrade(4,g9f41);
	//uTmp[0]=g9f41[3];
	//uTmp[1]=g9f41[2];
	//uTmp[2]=g9f41[1];
	//uTmp[3]=g9f41[0];
	
	memcpy(&gCounter,g9f41,4);
	memcpy(g9b,"\x00\x00",2);
	g9c = 0x00;  //����
	
	//20120118
	memset(gbf0c,0x00,sizeof(gbf0c));
	uLen_gbf0c=0;
	
	memset(g9f36,0x00,sizeof(g9f36));
	//memset(gATC,0x00,sizeof(gATC));
	memset(g5f30,0x00,sizeof(g5f30));
	memset(gdf1a,0x00,sizeof(gdf1a));
	
	
	//20130522
	gdf60=0x00;
	gdf61=0x00;
	
	
	memset(g9f79,0x00,sizeof(g9f79));
	memset(gdf62,0x00,sizeof(gdf62));
	memset(gdf63,0x00,sizeof(gdf63));
	
	gn9f79=0;
	gndf62=0;
	gndf63=0;	
	memset(gAPPAount_SN,0x00,sizeof(gAPPAount_SN));
	memset(gBuf0221,0x00,sizeof(gBuf0221));

	//20130609
	gCardBusinessType=0;
	//20130613
	memset(gF19_Thr,0x00,sizeof(gF19_Thr));
	memset(gAppSN,0x00,sizeof(gAppSN));

	memset(gBuf19,0x00,sizeof(gBuf19));

	//20130617
	g9f27=0x00;
	uLen_g9f27=0;

	memset(g9f08,0x00,sizeof(g9f08));
	uLen_g9f08=0;

	memset(g9f74,0x00,sizeof(g9f74));
	uLen_g9f74=0;

	memset(g9f63,0x00,sizeof(g9f63));
	uLen_g9f63=0;

	memset(g9f69,0x00,sizeof(g9f69));
	uLen_g9f69=0;

	uLen_gATC=0;

	//20130902
	memset(g77,0x00,sizeof(g77));
	uLen_g77=0;

	return 0;												
}
//#define _DEBUG_GetTagFrmTLV_
#ifdef _NEW_GETTAGFRMTLV_
void GetTagFrmTLV(UBYTE*  uTlv,int nLen_Tlv)
{
	int  i, j,nLen;
	int  count;
	UBYTE buf[512];
	pTlvData tlv = NULL;
	
/*
	if(uTlv[0]==0x70 || uTlv[0]==0x6F  || uTlv[0]==0xa5 || uTlv[0]==0x77)
	{
		nLen=uTlv[1];
		memcpy(buf,uTlv+2,nLen);
		if(uTlv[0]==0x6F)
		{
			uLen_g6f = nLen;
			memcpy(g6f,uTlv+2,nLen);
		}

	}

	if(uTlv[1]==0x81)
	{
		nLen=uTlv[2];
		memcpy(buf,uTlv+3,nLen);
	}
*/
	count = unpack_tlv_data(&tlv, uTlv,nLen_Tlv);
	if (count < 0)
	{
		printf("\r\n count failed, error code:%d\r\n", count);
	}
/*
	printf("\r\nTlv Unpack count=%d \n",count);
	for (i=0; i<count; ++i)
	{
		printf("Tag:");
		for (j=0; j<tlv[i].tag.size; ++j)
		{
			printf("%02X", tlv[i].tag.data[j]);
			if((j+1)%16==0)
				printf("\n");
	
		}
		printf("\n");

		printf("Len:%d\n", tlv[i].val.size);
		printf("Val:");
		for (j=0; j<tlv[i].val.size; ++j)
		{
			printf("%02X ", tlv[i].val.data[j]);
			if((j+1)%16==0)
				printf("\n");
	
		}
		printf("\n");
	}
*/		
	//nLen_AFL=0;
	//memset(AFL,0x00,sizeof(AFL));
	//uLen_g82 = 0;
	//memset(g82,0x00,sizeof(g82));
	//uLen_g57_gpo=0;
	//memset(g57,0x00,sizeof(g57));
	
	for (i=0; i<count; ++i)
	{
		
		if(memcmp(tlv[i].tag.data,"\x6f",tlv[i].tag.size)==0)
		{
			uLen_g6f = tlv[i].val.size;
			memcpy(g6f,tlv[i].val.data,tlv[i].val.size); 
		}
		if(memcmp(tlv[i].tag.data,"\x70",tlv[i].tag.size)==0)
		{
			uLen_g70 = tlv[i].val.size;
			memcpy(g70,tlv[i].val.data,tlv[i].val.size); 
		}

		if(memcmp(tlv[i].tag.data,"\x77",tlv[i].tag.size)==0)
		{
			uLen_g77 = tlv[i].val.size;
			memcpy(g77,tlv[i].val.data,tlv[i].val.size); 
		}

		if(memcmp(tlv[i].tag.data,"\xbf\x0c",tlv[i].tag.size)==0)
		{
			uLen_gbf0c = tlv[i].val.size;
			memcpy(gbf0c,tlv[i].val.data,tlv[i].val.size); 
		}
		
		if(memcmp(tlv[i].tag.data,"\x57",tlv[i].tag.size)==0)
		{
			uLen_g57 = tlv[i].val.size;
			memcpy(g57,tlv[i].val.data,tlv[i].val.size); 
		}
		
		if(memcmp(tlv[i].tag.data,"\x57",tlv[i].tag.size)==0)
		{
			uLen_g57 = tlv[i].val.size;
			memcpy(g57,tlv[i].val.data,tlv[i].val.size); 
		}

		if(memcmp(tlv[i].tag.data,"\x50",tlv[i].tag.size)==0)
		{
			uLen_g50 =tlv[i].val.size;
			memcpy(g50,tlv[i].val.data,tlv[i].val.size); 
		}

		if(memcmp(tlv[i].tag.data,"\x5a",tlv[i].tag.size)==0)
		{
			uTagRepeat_5a++;
			uLen_g5a =tlv[i].val.size;
			memcpy(g5a,tlv[i].val.data,tlv[i].val.size); 
		}
		
		if(memcmp(tlv[i].tag.data,"\x82",tlv[i].tag.size)==0)
		{

			uLen_g82 =tlv[i].val.size;
			memcpy(g82,tlv[i].val.data,tlv[i].val.size); 
		}

		if(memcmp(tlv[i].tag.data,"\x84",tlv[i].tag.size)==0)
		{
			uLen_g84 =tlv[i].val.size;
			memcpy(g84,tlv[i].val.data,tlv[i].val.size); 
		}

		if(memcmp(tlv[i].tag.data,"\x87",tlv[i].tag.size)==0)
		{
			uLen_g87 =tlv[i].val.size;
			memcpy(&g87,tlv[i].val.data,tlv[i].val.size); 
		}

		if(memcmp(tlv[i].tag.data,"\x8f",tlv[i].tag.size)==0)
		{
			uLen_g8f =tlv[i].val.size;
			memcpy(&g8f,tlv[i].val.data,tlv[i].val.size); 
			#ifdef _DEBUG_GetTagFrmTLV_
				fprintf(stderr,"��Կ����g8f=%02X \x0d\x0a",g8f);
			#endif
		}

		if(memcmp(tlv[i].tag.data,"\x90",tlv[i].tag.size)==0)
		{

			uLen_g90 =tlv[i].val.size;
			memcpy(g90,tlv[i].val.data,tlv[i].val.size); 
			#ifdef _DEBUG_GetTagFrmTLV_
			fprintf(stderr,"g90,uLen_tagValue=%02X:",uLen_tagValue);
			for(j=0;j<uLen_g90;j++)
				fprintf(stderr,"%02X ",g90[j]);
			fprintf(stderr,"\n\r");
			#endif
		}


		if(memcmp(tlv[i].tag.data,"\x92",tlv[i].tag.size)==0)
		{

			uLen_g92 =tlv[i].val.size;
			memcpy(g92,tlv[i].val.data,tlv[i].val.size); 
			
		}

		if(memcmp(tlv[i].tag.data,"\x93",tlv[i].tag.size)==0)
		{

			uLen_g93 =tlv[i].val.size;
			memcpy(g93,tlv[i].val.data,tlv[i].val.size); 
			
		}

		if(memcmp(tlv[i].tag.data,"\x94",tlv[i].tag.size)==0)
		{

			uLen_g94 =tlv[i].val.size;
			memcpy(g94,tlv[i].val.data,tlv[i].val.size); 
			nLen_AFL =uLen_g94;
			memcpy(AFL,g94,nLen_AFL); 

		}

		if(memcmp(tlv[i].tag.data,"\xa5",tlv[i].tag.size)==0)
		{
			uLen_ga5 =tlv[i].val.size;
			memcpy(ga5,tlv[i].val.data,tlv[i].val.size); 
		}

		if(memcmp(tlv[i].tag.data,"\x5F\x24",tlv[i].tag.size)==0)
		{
			uTagRepeat_5f24++; 
			memcpy(g5f24,tlv[i].val.data,tlv[i].val.size);
		}

		if(memcmp(tlv[i].tag.data,"\x5F\x2d",tlv[i].tag.size)==0)
		{
			memcpy(g5f2d,tlv[i].val.data,tlv[i].val.size);
		}

		if(memcmp(tlv[i].tag.data,"\x9f\x11",tlv[i].tag.size)==0)
		{
			memcpy(&g9f11,tlv[i].val.data,tlv[i].val.size); 
		}


		if(memcmp(tlv[i].tag.data,"\x9F\x12",tlv[i].tag.size)==0)
		{
			uLen_g9f12=tlv[i].val.size; 
			memcpy(g9f12,tlv[i].val.data,tlv[i].val.size);
		}


		if(memcmp(tlv[i].tag.data,"\x9F\x32",tlv[i].tag.size)==0)
		{
			uLen_g9f32=tlv[i].val.size; 
			memcpy(g9f32,tlv[i].val.data,tlv[i].val.size);
		}

		if(memcmp(tlv[i].tag.data,"\x9F\x38",tlv[i].tag.size)==0)
		{
			uLen_g9f38=tlv[i].val.size; 
			memcpy(g9f38,tlv[i].val.data,tlv[i].val.size);
		}
		
		if(memcmp(tlv[i].tag.data,"\x9F\x46",tlv[i].tag.size)==0)
		{
			uLen_g9f46=tlv[i].val.size; 
			memcpy(g9f46,tlv[i].val.data,tlv[i].val.size);
		}


		if(memcmp(tlv[i].tag.data,"\x9F\x47",tlv[i].tag.size)==0)
		{
			uLen_g9f47=tlv[i].val.size; 
			memcpy(g9f47,tlv[i].val.data,tlv[i].val.size);
		}
		
		if(memcmp(tlv[i].tag.data,"\x9F\x48",tlv[i].tag.size)==0)
		{
			uLen_g9f48=tlv[i].val.size; 
			memcpy(g9f48,tlv[i].val.data,tlv[i].val.size);
		}
		if(memcmp(tlv[i].tag.data,"\x9F\x4a",tlv[i].tag.size)==0)
		{
			uLen_g9f4a=tlv[i].val.size; 
			memcpy(g9f4a,tlv[i].val.data,tlv[i].val.size);
		}
		//df61 20130522 gwl
		if(memcmp(tlv[i].tag.data,"\xdf\x61",tlv[i].tag.size)==0)
		{
			memcpy(&gdf61,tlv[i].val.data,tlv[i].val.size); 
		}
		//9f27 20130617 gwl
		if(memcmp(tlv[i].tag.data,"\x9F\x27",tlv[i].tag.size)==0)
		{
			uLen_g9f27=tlv[i].val.size; 
			memcpy(&g9f27,tlv[i].val.data,tlv[i].val.size);
		}
		if(memcmp(tlv[i].tag.data,"\x9F\x08",tlv[i].tag.size)==0)
		{
			uLen_g9f08=tlv[i].val.size; 
			memcpy(g9f08,tlv[i].val.data,tlv[i].val.size);
		}
		if(memcmp(tlv[i].tag.data,"\x9F\x74",tlv[i].tag.size)==0)
		{
			uLen_g9f74=tlv[i].val.size; 
			memcpy(g9f74,tlv[i].val.data,tlv[i].val.size);
		}
		if(memcmp(tlv[i].tag.data,"\x9F\x63",tlv[i].tag.size)==0)
		{
			uLen_g9f63=tlv[i].val.size; 
			memcpy(g9f63,tlv[i].val.data,tlv[i].val.size);
		}
		if(memcmp(tlv[i].tag.data,"\x9F\x69",tlv[i].tag.size)==0)
		{
			uLen_g9f69=tlv[i].val.size; 
			memcpy(g9f69,tlv[i].val.data,tlv[i].val.size);
		}

	}




	return ;
}
#else
void GetTagFrmTLV(UBYTE* uTlv,int nLen_TLV)
{

	int i,j,nLen_Tmp;
	UBYTE uTmp[256],uTmp1[256],uName_tag[2];
	int uLen_tagName,uLen_tagValue,uPlace_tag;
	int nLen_Tmp_Total;
	
	Tag_All[0].uName[0]=0x81;
	Tag_All[0].uLength=1;
	
	Tag_All[1].uName[0]=0x90;
	Tag_All[1].uLength=1;
	
	memcpy(Tag_All[2].uName,"\x9f\x32",2);
	Tag_All[2].uLength=2;
	
	
	Tag_All[3].uName[0]=0x92;
	Tag_All[3].uLength=1;
	
	memcpy(Tag_All[4].uName,"\x9f\x46",2);
	Tag_All[4].uLength=2;
	
	memcpy(Tag_All[5].uName,"\x9f\x47",2);
	Tag_All[5].uLength=2;


	memcpy(Tag_All[6].uName,"\x9f\x48",2);
	Tag_All[6].uLength=2;


	memcpy(Tag_All[7].uName,"\x9f\x4a",2);
	Tag_All[7].uLength=2;
	
	
	memcpy(Tag_All[8].uName,"\x5f\x24",2);
	Tag_All[8].uLength=2;

	Tag_All[9].uName[0]=0x8f;
	Tag_All[9].uLength=1;
	
	Tag_All[10].uName[0]=0x70;
	Tag_All[10].uLength=1;
	
	Tag_All[11].uName[0]=0xa5;
	Tag_All[11].uLength=1;
	
	memcpy(Tag_All[12].uName,"\x9f\x38",2);
	Tag_All[12].uLength=2;
	
	Tag_All[13].uName[0]=0x84;
	Tag_All[13].uLength=1;
	
	Tag_All[14].uName[0]=0x5a;
	Tag_All[14].uLength=1;
	
	Tag_All[15].uName[0]=0x50;
	Tag_All[15].uLength=1;
	
	Tag_All[16].uName[0]=0x87;
	Tag_All[16].uLength=1;
	
	memcpy(Tag_All[17].uName,"\x5f\x2d",2);
	Tag_All[17].uLength=2;

	memcpy(Tag_All[18].uName,"\x9f\x11",2);
	Tag_All[18].uLength=2;

	memcpy(Tag_All[19].uName,"\x9f\x12",2);
	Tag_All[19].uLength=2;
	
	Tag_All[20].uName[0]=0x57;
	Tag_All[20].uLength=1;
	
	Tag_All[21].uName[0]=0x94;
	Tag_All[21].uLength=1;
	
	Tag_All[22].uName[0]=0x93;
	Tag_All[22].uLength=1;

	memcpy(Tag_All[23].uName,"\xdf\x61",2);
	Tag_All[23].uLength=2;
	
	memcpy(Tag_All[24].uName,"\x9f\x27",2);
	Tag_All[24].uLength=2;

	memcpy(Tag_All[25].uName,"\x9f\x09",2);
	Tag_All[25].uLength=2;

	memcpy(Tag_All[26].uName,"\x9f\x74",2);
	Tag_All[26].uLength=2;

	memcpy(Tag_All[27].uName,"\x9f\x63",2);
	Tag_All[27].uLength=2;

	memcpy(Tag_All[28].uName,"\x9f\x69",2);
	Tag_All[28].uLength=2;

	memcpy(Tag_All[29].uName,"\x9f\x08",2);
	Tag_All[29].uLength=2;

	memset(uTmp,0x00,sizeof(uTmp));
	nLen_Tmp =nLen_TLV;
	memcpy(uTmp,uTlv,nLen_Tmp);
	uPlace_tag =0;
	if(uTlv[0]==0x70 || uTlv[0]==0x6F  || uTlv[0]==0xa5 || uTlv[0]==0x77)
	{
		nLen_Tmp=uTlv[1];
		memcpy(uTmp,uTlv+2,nLen_Tmp);
		if(uTlv[0]==0x6F)
		{
			uLen_g6f = nLen_Tmp;
			memcpy(g6f,uTlv+2,nLen_Tmp);
		}

	}

	if(uTlv[1]==0x81)
	{
		nLen_Tmp=uTlv[2];
		memcpy(uTmp,uTlv+3,nLen_Tmp);
	}

	uLen_tagValue =nLen_Tmp;
	nLen_Tmp_Total = nLen_Tmp;
	//for(i=0;i<nLen_TLV;i++)
	i=0;
	while(1)
	{
		#ifdef _DEBUG_GetTagFrmTLV_
			fprintf(stderr,"TLV�⹹,uLen_tagValue=%02x,nLen_Tmp=%d,i=%d:",uLen_tagValue,nLen_Tmp,i);
			for(j=0;j<nLen_Tmp;j++)
				fprintf(stderr,"%02X",uTmp[j]);
			fprintf(stderr,"\n\r");
		#endif
		
		if(i>=nLen_Tmp)
			break;
		//�ڱ�ǩ�����ұ�ǩ
		#ifdef _DEBUG_GetTagFrmTLV_
			fprintf(stderr,"-----");
		#endif
		uLen_tagName =0;
		uLen_tagValue =0;
		for(j=0;j<TAGNUMBER;j++)
		{
			if(Tag_All[j].uName[0]==uTmp[i])
			{
				if(Tag_All[j].uLength==1)
				{
					uLen_tagName=1;
					nLen_Tmp_Total=nLen_Tmp_Total-1;
					#ifdef _DEBUG_GetTagFrmTLV_
						fprintf(stderr,"�ҵ���ǩ:tag %02X",Tag_All[j].uName[0]);
					#endif
					break;
				}
				
				if(Tag_All[j].uLength==2 && Tag_All[j].uName[1]==uTmp[i+1])
				{
					uLen_tagName=2;
					nLen_Tmp_Total=nLen_Tmp_Total-2;
					#ifdef _DEBUG_GetTagFrmTLV_
						fprintf(stderr,"�ҵ���ǩ:tag %02X%02X",Tag_All[j].uName[0],Tag_All[j].uName[1]);
					#endif
					break;
				}	
			}	
		}
		if(j>=TAGNUMBER)
		{
			i++;
			continue;
		}
		//�ҵ���ǩ
		memcpy(uName_tag,Tag_All[j].uName,uLen_tagName);
		uLen_tagValue = uTmp[i+uLen_tagName];
		uPlace_tag = i+uLen_tagName+1;
		nLen_Tmp_Total=nLen_Tmp_Total-1;
		if(uLen_tagValue==0x81)
		{						
			uLen_tagValue = uTmp[i+uLen_tagName+1];
			uPlace_tag++;
			nLen_Tmp_Total=nLen_Tmp_Total-1;
		}
		if(uLen_tagName==1)
		{
			// tag50
			if(uName_tag[0]==0x50)
			{
				uLen_g50 = uLen_tagValue;
				memcpy(g50,uTmp+uPlace_tag,uLen_tagValue);
				#ifdef _DEBUG_GetTagFrmTLV_
					fprintf(stderr,"g50,uLen_tagValue=%02X:",uLen_tagValue);
					for(j=0;j<uLen_g50;j++)
						fprintf(stderr,"%02X ",g50[j]);
					fprintf(stderr,"\n\r");
				#endif
			}
				
			// tag57
			if(uName_tag[0]==0x57)
			{
				uTagRepeat_57++;
				uLen_g57 = uLen_tagValue;
				memcpy(g57,uTmp+uPlace_tag,uLen_tagValue);
				#ifdef _DEBUG_GetTagFrmTLV_
					fprintf(stderr,"g57,uLen_tagValue=%02X,uTagRepeat_57=%02x:",uLen_tagValue,uTagRepeat_57);
					for(j=0;j<uLen_g57;j++)
						fprintf(stderr,"%02X ",g57[j]);
					fprintf(stderr,"\n\r");
				#endif
			}
			// tag5a
			if(uName_tag[0]==0x5a)
			{
				uTagRepeat_5a++;
				uLen_g5a = uLen_tagValue;
				memcpy(g5a,uTmp+uPlace_tag,uLen_tagValue);
				#ifdef _DEBUG_GetTagFrmTLV_
					fprintf(stderr,"g5a,uLen_tagValue=%02X:",uLen_tagValue);
					for(j=0;j<uLen_g5a;j++)
						fprintf(stderr,"%02X ",g5a[j]);
					fprintf(stderr,"\n\r");
				#endif
			}
			
			// tag84
			if(uName_tag[0]==0x84)
			{
				uLen_g84 = uLen_tagValue;
				memcpy(g84,uTmp+uPlace_tag,uLen_tagValue);
				#ifdef _DEBUG_GetTagFrmTLV_
					fprintf(stderr,"g84,uLen_tagValue=%02X:",uLen_tagValue);
					for(j=0;j<uLen_g84;j++)
						fprintf(stderr,"%02X ",g84[j]);
					fprintf(stderr,"\n\r");
				#endif
			}
			
			// tag87
			if(uName_tag[0]==0x87)
			{
				memcpy(&g87,uTmp+uPlace_tag,uLen_tagValue);
				#ifdef _DEBUG_GetTagFrmTLV_
					fprintf(stderr,"g87,uLen_tagValue=%02X:",uLen_tagValue);
					for(j=0;j<1;j++)
						fprintf(stderr,"%02X ",g87);
					fprintf(stderr,"\n\r");
				#endif
			}
			
			
			if(uName_tag[0]==0x8f)
			{
				g8f =uTmp[uPlace_tag];
				uLen_g8f = 1;
				#ifdef _DEBUG_GetTagFrmTLV_
					fprintf(stderr,"��Կ����g8f=%02X \x0d\x0a",g8f);
				#endif
			}
			#ifdef _DEBUG_GetTagFrmTLV_
					fprintf(stderr,"uPlace_tag=%d:",uPlace_tag);
			#endif
				// tag90
			if(uName_tag[0]==0x90 && uLen_g90==0 && uLen_tagValue<=(nLen_Tmp-uPlace_tag))
			{
				uLen_g90 = uLen_tagValue;
				memcpy(g90,uTmp+uPlace_tag,uLen_tagValue);
				#ifdef _DEBUG_GetTagFrmTLV_
					fprintf(stderr,"g90,uLen_tagValue=%02X:",uLen_tagValue);
					for(j=0;j<uLen_g90;j++)
						fprintf(stderr,"%02X ",g90[j]);
					fprintf(stderr,"\n\r");
				#endif
			}
			if(uName_tag[0]==0x92)
			{
				uLen_g92 = uLen_tagValue;
				memcpy(g92,uTmp+uPlace_tag,uLen_g92);
				#ifdef _DEBUG_L2T_
					fprintf(stderr,"g92,uLen_tagValue=%02X:",uLen_tagValue);
					for(j=0;j<uLen_g92;j++)
						fprintf(stderr,"%02X ",g92[j]);
					fprintf(stderr,"\n\r");
				#endif
			}
			
			// tag94
			if(uName_tag[0]==0x94 && uLen_g94==0 && uLen_tagValue<=(nLen_Tmp-uPlace_tag))
			{
				uLen_g94 = uLen_tagValue;
				memcpy(g94,uTmp+uPlace_tag,uLen_tagValue);
				#ifdef _DEBUG_GetTagFrmTLV_
					fprintf(stderr,"g94,uLen_tagValue=%02X:",uLen_tagValue);
					for(j=0;j<uLen_g94;j++)
						fprintf(stderr,"%02X ",g94[j]);
					fprintf(stderr,"\n\r");
				#endif
			}
			//20130516	
			// tag93
			if(uName_tag[0]==0x93 && uLen_g93==0 && uLen_tagValue<=(nLen_Tmp-uPlace_tag))
			{
				uLen_g93 = uLen_tagValue;
				memcpy(g93,uTmp+uPlace_tag,uLen_tagValue);
				#ifdef _DEBUG_GetTagFrmTLV_
					fprintf(stderr,"g93,uLen_tagValue=%02X:",uLen_tagValue);
					for(j=0;j<uLen_g93;j++)
						fprintf(stderr,"%02X ",g93[j]);
					fprintf(stderr,"\n\r");
				#endif
			}
			// tag a5
			if(uName_tag[0]==0xa5)
			{
				uLen_ga5 = uLen_tagValue;
				memcpy(ga5,uTmp+uPlace_tag,uLen_tagValue);
				#ifdef _DEBUG_GetTagFrmTLV_
					fprintf(stderr,"ga5,uLen_tagValue=%02X:",uLen_tagValue);
					for(j=0;j<uLen_ga5;j++)
						fprintf(stderr,"%02X ",ga5[j]);
					fprintf(stderr,"\n\r");
				#endif
			}
		
		}
		if(uLen_tagName==2)
		{
		
			//g5f24
			if(memcmp(uName_tag,"\x5f\x24",2)==0)
			{	
				uTagRepeat_5f24++;
				memcpy(g5f24,uTmp + uPlace_tag,uLen_tagValue);
				
		#ifdef _DEBUG_GetTagFrmTLV_
			fprintf(stderr,"g5f24,uLen_tagValue=%02X:",uLen_tagValue);
			for(j=0;j<uLen_tagValue;j++)
				fprintf(stderr,"%02X ",g5f24[j]);
			fprintf(stderr,"\n\r");
		#endif
			}
			
			
			//g5f2d
			if(memcmp(uName_tag,"\x5f\x2d",2)==0)
			{	
				memcpy(g5f2d,uTmp + uPlace_tag,uLen_tagValue);
				
		#ifdef _DEBUG_GetTagFrmTLV_
			fprintf(stderr,"g5f2d,uLen_tagValue=%02X:",uLen_tagValue);
			for(j=0;j<uLen_tagValue;j++)
				fprintf(stderr,"%02X ",g5f2d[j]);
			fprintf(stderr,"\n\r");
		#endif
			}
			
				
			//g9f11
			if(memcmp(uName_tag,"\x9f\x11",2)==0)
			{
				memcpy(&g9f11,uTmp + uPlace_tag,uLen_tagValue);			
				
			#ifdef _DEBUG_GetTagFrmTLV_
				fprintf(stderr,"g9f11,uLen_tagValue=%02X:",uLen_tagValue);
				for(j=0;j<1;j++)
					fprintf(stderr,"%02X ",g9f11);
				fprintf(stderr,"\n\r");
			#endif
			}
			
			
			//g9f12
			if(memcmp(uName_tag,"\x9f\x12",2)==0)
			{
				memcpy(g9f12,uTmp + uPlace_tag,uLen_tagValue);			
				uLen_g9f12 = uLen_tagValue;
			#ifdef _DEBUG_GetTagFrmTLV_
				fprintf(stderr,"g9f12,uLen_tagValue=%02X:",uLen_tagValue);
				for(j=0;j<uLen_tagValue;j++)
					fprintf(stderr,"%02X ",g9f12[j]);
				fprintf(stderr,"\n\r");
			#endif
			}
			
			//g9f32
		
			if(memcmp(uName_tag,"\x9f\x32",2)==0)
			{
				memcpy(g9f32,uTmp + uPlace_tag,uLen_tagValue);			
				uLen_g9f32 = uLen_tagValue;
			#ifdef _DEBUG_GetTagFrmTLV_
				fprintf(stderr,"g9f32,uLen_tagValue=%02X:",uLen_tagValue);
				for(j=0;j<uLen_tagValue;j++)
					fprintf(stderr,"%02X ",g9f32[j]);
				fprintf(stderr,"\n\r");
			#endif
			}
			
			//g9f38
			if(memcmp(uName_tag,"\x9f\x38",2)==0)
			{
				memcpy(g9f38,uTmp + uPlace_tag,uLen_tagValue);			
				uLen_g9f38 = uLen_tagValue;
			#ifdef _DEBUG_GetTagFrmTLV_
				fprintf(stderr,"g9f38,uLen_tagValue=%02X:",uLen_tagValue);
				for(j=0;j<uLen_tagValue;j++)
					fprintf(stderr,"%02X ",g9f38[j]);
				fprintf(stderr,"\n\r");
			#endif
			}
			
			
			//9f46
			if(memcmp(uName_tag,"\x9f\x46",2)==0)
			{
				memcpy(g9f46,uTmp + uPlace_tag,uLen_tagValue);
				uLen_g9f46 = uLen_tagValue;
				
			#ifdef _DEBUG_GetTagFrmTLV_
				fprintf(stderr,"g9f46,uLen_tagValue=%02X:",uLen_tagValue);
				for(j=0;j<uLen_tagValue;j++)
					fprintf(stderr,"%02X ",g9f46[j]);
				fprintf(stderr,"\n\r");
			#endif

			}
			//9f47
			if(memcmp(uName_tag,"\x9f\x47",2)==0)
			{
				memcpy(g9f47,uTmp + uPlace_tag,uLen_tagValue);
				uLen_g9f47 = uLen_tagValue;
				
			#ifdef _DEBUG_GetTagFrmTLV_
				fprintf(stderr,"g9f47,uLen_tagValue=%02X:",uLen_tagValue);
				for(j=0;j<uLen_tagValue;j++)
					fprintf(stderr,"%02X ",g9f47[j]);
				fprintf(stderr,"\n\r");
			#endif

			}
			
			//9f48
			if(memcmp(uName_tag,"\x9f\x48",2)==0)
			{
				memcpy(g9f48,uTmp + uPlace_tag,uLen_tagValue);
				uLen_g9f48 = uLen_tagValue;
				
			#ifdef _DEBUG_GetTagFrmTLV_
				fprintf(stderr,"g9f48,uLen_tagValue=%02X:",uLen_tagValue);
				for(j=0;j<uLen_tagValue;j++)
					fprintf(stderr,"%02X ",g9f48[j]);
				fprintf(stderr,"\n\r");
			#endif

			}
			
			//9f4a
			if(memcmp(uName_tag,"\x9f\x4a",2)==0 && uTmp[uPlace_tag]==0x82 )
			{
				memcpy(g9f4a,uTmp + uPlace_tag,uLen_tagValue);
				uLen_g9f4a = uLen_tagValue;
				
				#ifdef _DEBUG_GetTagFrmTLV_
					fprintf(stderr,"g9f4a,uLen_tagValue=%02X:",uLen_tagValue);
					for(j=0;j<uLen_tagValue;j++)
						fprintf(stderr,"%02X ",g9f4a[j]);
					fprintf(stderr,"\n\r");
				#endif
			}
		
			//df61 20130522 gwl
			if(memcmp(uName_tag,"\xdf\x61",2)==0 && uLen_tagValue==1)
			{
				memcpy(&gdf61,uTmp + uPlace_tag,uLen_tagValue);
				#ifdef _DEBUG_GetTagFrmTLV_
					fprintf(stderr,"gdf61,uLen_tagValue=%02X:",uLen_tagValue);
					fprintf(stderr,"%02X ",gdf61);
					fprintf(stderr,"\n\r");
				#endif
			}

			//9f27 20130617 gwl
			if(memcmp(uName_tag,"\x9f\x27",2)==0)
			{
				memcpy(&g9f27,uTmp + uPlace_tag,uLen_tagValue);
				uLen_g9f27 = uLen_tagValue;
				#ifdef _DEBUG_GetTagFrmTLV_
					fprintf(stderr,"g9f27,uLen_tagValue=%02X:",uLen_tagValue);
					fprintf(stderr,"%02X ",g9f27);
					fprintf(stderr,"\n\r");
				#endif
			}

			//9f08
			if(memcmp(uName_tag,"\x9f\x08",2)==0 && uLen_tagValue==2)
			{
				memcpy(g9f08,uTmp + uPlace_tag,uLen_tagValue);
				uLen_g9f08 = uLen_tagValue;
				
				//#ifdef _DEBUG_GetTagFrmTLV_
					fprintf(stderr,"g9f08,uLen_tagValue=%02X:",uLen_tagValue);
					for(j=0;j<uLen_tagValue;j++)
						fprintf(stderr,"%02X ",g9f08[j]);
					fprintf(stderr,"\n\r");
				//#endif
			}
		

			//9f74
			if(memcmp(uName_tag,"\x9f\x74",2)==0)
			{
				memcpy(g9f74,uTmp + uPlace_tag,uLen_tagValue);
				uLen_g9f74 = uLen_tagValue;
				
				#ifdef _DEBUG_GetTagFrmTLV_
					fprintf(stderr,"g9f74,uLen_tagValue=%02X:",uLen_tagValue);
					for(j=0;j<uLen_tagValue;j++)
						fprintf(stderr,"%02X ",g9f74[j]);
					fprintf(stderr,"\n\r");
				#endif
			}

			//9f63
			if(memcmp(uName_tag,"\x9f\x63",2)==0)
			{
				memcpy(g9f63,uTmp + uPlace_tag,uLen_tagValue);
				uLen_g9f63 = uLen_tagValue;

				#ifdef _DEBUG_GetTagFrmTLV_
					fprintf(stderr,"g9f63,uLen_tagValue=%02X:",uLen_tagValue);
					for(j=0;j<uLen_tagValue;j++)
						fprintf(stderr,"%02X ",g9f63[j]);
					fprintf(stderr,"\n\r");
				#endif
			}


			//9f69
			if(memcmp(uName_tag,"\x9f\x69",2)==0)
			{
				memcpy(g9f69,uTmp + uPlace_tag,uLen_tagValue);
				uLen_g9f69 = uLen_tagValue;

				//#ifdef _DEBUG_GetTagFrmTLV_
					fprintf(stderr,"g9f69,uLen_tagValue=%02X:",uLen_tagValue);
					for(j=0;j<uLen_tagValue;j++)
						fprintf(stderr,"%02X ",g9f69[j]);
					fprintf(stderr,"\n\r");
				//#endif
			}

		}
		//---------------------------
		//uTmpָ���ƶ�
		#ifdef _DEBUG_GetTagFrmTLV_
			fprintf(stderr,"uTmpָ���ƶ�ǰ,uLen_tagValue=%02X,nLen_Tmp_Total=%d",uLen_tagValue,nLen_Tmp_Total);
		#endif	
		nLen_Tmp_Total = nLen_Tmp_Total - uLen_tagValue;
		if(nLen_Tmp_Total<=0)
			break;
		memcpy(uTmp1,uTmp + uPlace_tag+uLen_tagValue,nLen_Tmp_Total);
		nLen_Tmp = nLen_Tmp_Total;
		memcpy(uTmp,uTmp1,nLen_Tmp);
		uLen_tagValue=0x00;		
		i=0;
		#ifdef _DEBUG_GetTagFrmTLV_
			fprintf(stderr,"uTmpָ���ƶ���,uLen_tagValue=%02X,nLen_Tmp_Total=%d",uLen_tagValue,nLen_Tmp_Total);
		#endif
	
	}//while ѭ������
	
	return ;
}
#endif
//#define _DEBUG_GETINFO_
int pf_mobile_ticket_get_info(char* inbuf,int nInLen,char  *outbuf)
{
	unsigned int nRetCode=0,i;
	UBYTE uTmp[256],Diversity[32],pszInitData[64],pszBuf[128],Key[32],R_MAC[4];
	UBYTE uAID[256],uAID_Single[1+LENSINGLEAID+1],uAID_Single_Tmp[1+LENSINGLEAID+1];
	int nLen_AID,nLen_Tmp;
	int j;
	
	UBYTE uPlace=0x00,u87;
	struct  timeval start_time;

	memset(ugRand_RMAC,0x00,sizeof(ugRand_RMAC));
	gettimeofday(&start_time,NULL);

	
	//printf("\n\r In pf_mobile_ticket_get_info \n\r");

	initparam();
	
	//if(checkparam()!=0)
	//      return ERR_CHKPARAM;
	
	outbuf[0]=0x00;
	outbuf[1]=0xF0;
	memcpy(g9f02,"\x00\x00\x00\x00\x00\x00",6);
	
	
	init_tagList();
	
	
	memset(uAID,0x00,sizeof(uAID));
	nLen_AID=0;
	nRetCode=PPSE_Proc_L2(inbuf,nInLen,uAID,&nLen_AID);
	if(nRetCode)
		return nRetCode;
	
	GenRand8(start_time);
	nLen_Tmp=0;
	memset(uTmp,0x00,sizeof(uTmp));
	#ifdef _DEBUG_L2T_
		fprintf(stderr,"\r\n PPSE_Proc_L2,AID :\r\n");
		for(i=0;i<nLen_AID;i++)
		{
			fprintf(stderr,"%02X ",uAID[i]);
		}
		fprintf(stderr,"\r\n--------------------------------------------------");
	#endif


	for(u87=0x01;u87<64;u87++)
	{
		for(i=0;i<(nLen_AID/(LENSINGLEAID+2));i++)
		{
			memcpy(uAID_Single,uAID+i*(LENSINGLEAID+2),LENSINGLEAID+2);
			//fprintf(stderr,"\r\n uAID_Single =%02x%02x \r\n",uAID_Single[0],uAID_Single[LENSINGLEAID+1]);
			if(uAID_Single[LENSINGLEAID+1]==u87)
			{
				memcpy(uTmp+nLen_Tmp,uAID_Single,LENSINGLEAID+2);
				nLen_Tmp = nLen_Tmp + LENSINGLEAID+2;
			}
		}
		if(nLen_Tmp >=nLen_AID)
			break;

	}
	if(memcmp(uAID,"\x08\xA0\x00\x00\x03\x33\x01x01\x02",9)!=0 && memcmp(uTmp,"\x00\x00\x00\x00",4)!=0) 
		memcpy(uAID,uTmp,nLen_AID);
#ifdef _DEBUG_L2T_
	fprintf(stderr,"\r\n -----order AID data:-----------\r\n");
	for(i=0;i<nLen_AID;i++)
	{
		fprintf(stderr,"%02X ",uAID[i]);
	}
	fprintf(stderr,"\r\n--------------------------------------------------");
#endif
	uPlace=0;
	while(1)
	{
		//AID
		memcpy ( C_Apdu.data , "\x00\xA4\x04\x00",4);
		memcpy(uAID_Single_Tmp,uAID+uPlace,LENSINGLEAID+2);
		C_Apdu.data[4]=uAID_Single_Tmp[0];
		//uAID_Single_Tmp[8]=0x04;
		memcpy(C_Apdu.data+5,uAID_Single_Tmp+1,C_Apdu.data[4]);
		C_Apdu.length = C_Apdu.data[4]+5 ;
		nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
		if(nRetCode==25219)
		{
			uPlace = uPlace + (LENSINGLEAID+2);
			if(uPlace>=nLen_AID)
			{
			#ifdef _DEBUG_L2T_
				fprintf(stderr,"no ");
			#endif
				return ERR_NOAIDPATCH;
			}
			continue;		
		}
		if (nRetCode!=OK)
		{
		#ifdef _DEBUG_L2T_
			fprintf(stderr,"no:uRetCode=%04X",nRetCode);
		#endif
			return (nRetCode) ;
		}
	
		uLen_g9f06 = uAID_Single_Tmp[0];
		memcpy(g9f06,uAID_Single_Tmp+1,uLen_g9f06);

		
		nRetCode = Parse_AIDRESP(R_Apdu.data,R_Apdu.length-2);
		if(uLen_g6f==0 || uLen_g84==0 || uLen_ga5==0)
		{
			uPlace = uPlace + (LENSINGLEAID+2);	
			if(uPlace>=nLen_AID)
			{
				return ERR_NOAIDPATCH;
			}
			continue;		
		}	
		if(nRetCode==ERR_PDOL0)
		{
			return ERR_NOPDOL;
		}
		if(nRetCode==ERR_NOTAG9f66)
		{
			return ERR_NOTAG9f66;
		}
				
		if(nRetCode!=OK)
			return nRetCode;
		
		//20130514  20130827
		memcpy ( C_Apdu.data , "\x80\xb4\x00\xa8\x02\x01\x21",7);
		C_Apdu.length = 7 ;
		if(gdf61==0x81 || gdf61==0x82)
		{
			C_Apdu.data[4]=0x0a;
			memcpy ( C_Apdu.data+7 , ugRand_RMAC,8);
			C_Apdu.length = C_Apdu.length +8;
			memcpy(pszBuf,C_Apdu.data,15);
		}
		nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
		if (nRetCode!=OK)
		{
		#ifdef _DEBUG_L2T_
			fprintf(stderr,"0121:uRetCode=%04X",nRetCode);
		#endif
			return (nRetCode) ;
		}
		if(gdf61==0x81 || gdf61==0x82)
		{
			if(R_Apdu.length<70)
				return ERR_PAGRMAC;
		}
		else
		{
			if(R_Apdu.length>66)
				return ERR_PAGRMAC;
		}
		////��Ч �ֶ� δ��
		if(memcmp(R_Apdu.data+3,"\x01\x01\x00",3)!=0)
		{
			return ERR_TRIPLEFLAG0121;

		}

		//ȡӦ�����˺�+���gAPPAount_SN
		memcpy(gAPPAount_SN,R_Apdu.data+6,11);
		
		g_CardInfo=(D_CardInfo*)(outbuf+2);
		memcpy(g_CardInfo->AppAccount,R_Apdu.data+6,10);
		g_CardInfo->AccountSn=R_Apdu.data[16];
		memcpy(g_CardInfo->IssueBankCode,R_Apdu.data+17,2);
		g_CardInfo->BussiType=R_Apdu.data[19];
			
		memcpy(g_CardInfo->Validdate,R_Apdu.data+24,4);
		//20130827 VERYFY R_MAC
		if(g_TP_Head->R_MAC_Flag[0]==1 && (gdf61==0x81 || gdf61==0x82))
		{
			memset(Diversity,0x00,16);
			memcpy(Diversity,gAPPAount_SN+3,8);

			memcpy(pszInitData,ugRand_RMAC,8);
			memcpy(R_MAC,R_Apdu.data+64,4);

			//fprintf(stderr,"\r\n 0121:R_MAC=%02X%02X%02X%02X \r\n",R_MAC[0],R_MAC[1],R_MAC[2],R_MAC[3]);
			memcpy(pszBuf,R_Apdu.data,64);
			pszBuf[64]=0x80;
			memset(uTmp,0x00,sizeof(uTmp));
			memcpy(Key,"\xD0\xC1\xF2\xAB\x61\x37\xFE\xDF\x97\x45\x3E\xA8\xA4\x75\x0D\x5E",16);

			PSAM_SoftMAC(Key,Diversity,pszInitData,pszBuf,64,uTmp);
/*
		 	nRetCode=PSAM_MAC(PSAM_NDX,1,Diversity,pszInitData,pszBuf,72,uTmp);
			if (nRetCode!=OK)
			{
				fprintf(stderr,"RMAC0121 PSAM_MAC:uRetCode=%04X",nRetCode);
				return (nRetCode) ;
			}

*/
			fprintf(stderr,"\r\n 0121:R_MAC=%02X%02X%02X%02X",R_MAC[0],R_MAC[1],R_MAC[2],R_MAC[3]);

			if(memcmp(R_MAC,uTmp,4)!=0)
				return ERR_RMAC;
		}

		//20130609 0-��ͨ�� 0x90-Ա����
		gCardBusinessType = g_CardInfo->BussiType;
		outbuf[1]=gCardBusinessType;
		if(gCardBusinessType==0x90)
		{
	
			nRetCode =PPSE_MEC(uTmp,&nLen_Tmp);
			if(nRetCode != OK)
				return nRetCode;

			//00 b0 read bin 0x15
			memset(C_Apdu.data,0x00,sizeof(C_Apdu.data));
			memcpy ( C_Apdu.data , "\x00\xb0\x95\x00\x1e",5);
			C_Apdu.length = 5;

			nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
			if(nRetCode != OK)
				return nRetCode;

			memcpy(gAppSN,R_Apdu.data +12,8);

			g_MECCardInfo=(D_MECCardInfo*)(outbuf+2);
			memcpy(g_MECCardInfo->CardSn,gAppSN+4,sizeof(g_MECCardInfo->CardSn));
			memcpy(g_MECCardInfo->AppSn,R_Apdu.data +10,sizeof(g_MECCardInfo->AppSn));
			
			g_MECCardInfo->CardType=R_Apdu.data[28];//������
			memcpy(g_MECCardInfo->Validdate,R_Apdu.data +24,sizeof(g_MECCardInfo->Validdate));//��Ч��
		
			
			//get balance  80 5c
			memcpy ( C_Apdu.data , "\x80\x5c\x00\x02\x04",5);
			C_Apdu.length = 5 ;
		 
			nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
			if (nRetCode!=OK)
			{
				return (nRetCode) ;
			}
			memset(gBalance,0x00,sizeof(gBalance));
			memcpy(gBalance,R_Apdu.data,4);//����Ǯ�����
			memcpy(g_MECCardInfo->Balance,gBalance,sizeof(g_MECCardInfo->Balance));
			
			
			//00 b0 read bin 0x06
			memset(C_Apdu.data,0x00,sizeof(C_Apdu.data));
			memcpy ( C_Apdu.data , "\x00\xb0\x86\x00\x20",5);
			C_Apdu.length = 5;

			nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
			if(nRetCode != OK)
				return nRetCode;
			//20130722
			g_MECCardInfo->Flag=R_Apdu.data[12+0];//��Χ��־(1��
			memcpy(g_MECCardInfo->LineBitMap,R_Apdu.data+12+1,sizeof(g_MECCardInfo->LineBitMap));//+��·λͼ��6��+
			memcpy(g_MECCardInfo->StationBitMap,R_Apdu.data+12+7,sizeof(g_MECCardInfo->StationBitMap));//+��վλͼ��12��
			
			//00 19
			memcpy ( C_Apdu.data , "\x00\xb2\x01\xcc\x40",5);
			C_Apdu.length = 5 ;
		 
			nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
			if (nRetCode!=OK)
			{
				return (nRetCode) ;
			}
			memset(gF19_Thr,0x00,sizeof(gF19_Thr));
			memcpy(gF19_Thr,R_Apdu.data,3);//�������ѱ�־(0x09) + ��¼���� + Ӧ��������־λ  20130613
			memcpy(gBuf19,R_Apdu.data,R_Apdu.length-2);
			memcpy(g_MECCardInfo->Buf19,R_Apdu.data+3,sizeof(g_MECCardInfo->Buf19));

			outbuf[0]=0x00;
			//20130726
			if(gF19_Thr[2]==0x01)
				return ERR_MEC_APPLOCK;
			return 0;

		}
/*20131117
		memcpy ( C_Apdu.data , "\x80\xb4\x00\xa8\x02\x02\x21",7);
		C_Apdu.length = 7 ;
	 	if(gdf61==0x81 || gdf61==0x82)
		{
			C_Apdu.data[4]=0x0a;
			memcpy ( C_Apdu.data+7 , ugRand_RMAC,8);
			C_Apdu.length = C_Apdu.length +8;
			memcpy(pszBuf,C_Apdu.data,15);
		}

		nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
		if (nRetCode!=OK)
		{
		#ifdef _DEBUG_L2T_
			fprintf(stderr,"0221:uRetCode=%04X",nRetCode);
		#endif
			return (nRetCode) ;
		}
		if(gdf61==0x81 || gdf61==0x82)
		{
			if(R_Apdu.length<70)
				return ERR_PAGRMAC;
		}
		else
		{
			if(R_Apdu.length>66)
				return ERR_PAGRMAC;
		}
		////��Ч �ֶ� δ��
		if(memcmp(R_Apdu.data+3,"\x01\x01\x00",3)!=0)
		{
			return ERR_TRIPLEFLAG0221;
		}
		if(g_TP_Head->R_MAC_Flag[0]==1 && (gdf61==0x81 || gdf61==0x82))
		{
			memset(pszBuf,0x00,sizeof(pszBuf));
			memcpy(pszBuf,R_Apdu.data,R_Apdu.length-6);
			memcpy(R_MAC,R_Apdu.data+64,4);

			memset(Diversity,0x00,16);

			memcpy(Diversity,gAPPAount_SN+3,8);
			memcpy(pszInitData,ugRand_RMAC,8);

			pszBuf[64]=0x80;
			memset(uTmp,0x00,sizeof(uTmp));
		
		 	nRetCode=PSAM_MAC(PSAM_NDX,2,Diversity,pszInitData,pszBuf,72,uTmp);
			if (nRetCode!=OK)
			{
				fprintf(stderr,"RMAC0221 PSAM_MAC:uRetCode=%04X",nRetCode);
				return (nRetCode) ;
			}
		
			fprintf(stderr,"\r\n 0221:R_MAC=%02X%02X%02X%02X \r\n",R_MAC[0],R_MAC[1],R_MAC[2],R_MAC[3]);

			if(memcmp(R_MAC,uTmp,4)!=0)
				return ERR_RMAC;

		}


#ifdef _DEBUG_GETINFO_
		printf("\n\r 0221:\n\r");
		for(i=0;i<R_Apdu.length;i++)
		{
			if((i+1) % 16==0)
				printf("\x0d\x0a ");
			printf("%02X ",R_Apdu.data[i]);	
		}
		printf("\n\r 0221:\n\r");
#endif
		//����0221 buffer
		memcpy(gBuf0221,R_Apdu.data,LEN0221);
		//memcpy(g_CardInfo->Buf0221,R_Apdu.data+6,sizeof(g_CardInfo->Buf0221));
		memcpy(g_CardInfo->Buf0221,R_Apdu.data+6,41);
	
		memcpy(g_CardInfo->AID,g9f06,uLen_g9f06);
	
#ifdef _GET0321_
		if(g_TP_Head->LCYH_Flag[0]==1)
		{
			//0321
			memcpy ( C_Apdu.data , "\x80\xb4\x00\xa8\x02\x03\x21",7);
			C_Apdu.length = 7 ;
			if(gdf61==0x81 || gdf61==0x82)
			{
				C_Apdu.data[4]=0x0a;
				memcpy ( C_Apdu.data+7 , ugRand_RMAC,8);
				C_Apdu.length = C_Apdu.length +8;
				memcpy(pszBuf,C_Apdu.data,15);
			}
			

			nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
			

			if (nRetCode!=OK)
			{
			#ifdef _DEBUG_L2T_
				fprintf(stderr,"2103:uRetCode=%04X",nRetCode);
			#endif
				return (nRetCode) ;
			}
			if(gdf61==0x81 || gdf61==0x82)
			{
				if(R_Apdu.length<70)
					return ERR_PAGRMAC;
			}
			else
			{
				if(R_Apdu.length>66)
					return ERR_PAGRMAC;
			}

			if(g_TP_Head->R_MAC_Flag[0]==1 && (gdf61==0x81 || gdf61==0x82))
			{
				memset(pszBuf,0x00,sizeof(pszBuf));
				memcpy(pszBuf,R_Apdu.data,R_Apdu.length-6);
				memcpy(R_MAC,R_Apdu.data+64,4);

				memset(Diversity,0x00,16);
	
				memcpy(Diversity,gAPPAount_SN+3,8);
				memcpy(pszInitData,ugRand_RMAC,8);
	
				pszBuf[64]=0x80;
				memset(uTmp,0x00,sizeof(uTmp));
			#ifdef _SOFTMAC_
				memcpy(Key,"\x8A\xA2\x94\x76\xDF\xBA\x3E\x5E\xAB\x97\x25\x13\x58\x92\xAE\x51",16);
				PSAM_SoftMAC(Key,Diversity,pszInitData,pszBuf,64,uTmp);
			#else	
			 	nRetCode=PSAM_MAC(PSAM_NDX,3,Diversity,pszInitData,pszBuf,72,uTmp);
				if (nRetCode!=OK)
				{
					fprintf(stderr,"RMAC0321 PSAM_MAC:uRetCode=%04X",nRetCode);
					return (nRetCode) ;
				}
			#endif
				fprintf(stderr,"\r\n 0321:R_MAC=%02X%02X%02X%02X \r\n",R_MAC[0],R_MAC[1],R_MAC[2],R_MAC[3]);

				if(memcmp(R_MAC,uTmp,4)!=0)
					return ERR_RMAC;

			}

			if(nRetCode == OK)
			{
				////��Ч �ֶ� δ��
				if(memcmp(R_Apdu.data+3,"\x01\x01\x00",3)!=0)
				{
					return ERR_TRIPLEFLAG0321;
				}
				memcpy(g_CardInfo->Buf0321,R_Apdu.data+7+2,sizeof(g_CardInfo->Buf0321));//20130808
			
			
				break;
			}
			if (nRetCode!=27013)  //0x6985=27103
			{
			#ifdef _DEBUG_L2T_
				fprintf(stderr,"������ֹ  AID:uRetCode=%04X,%d",nRetCode,nRetCode);
			#endif
				return (nRetCode) ;
			}
		}
#endif
		if(nRetCode == OK)
		{
		
			break;
		}
		if (nRetCode!=27013)  //0x6985=27103
		{
		#ifdef _DEBUG_L2T_
			fprintf(stderr,"������ֹ  AID:uRetCode=%04X,%d",nRetCode,nRetCode);
		#endif
			return (nRetCode) ;
		}


		uPlace = uPlace + (LENSINGLEAID+2);
		
		if(uPlace>=nLen_AID)
		{
		#ifdef _DEBUG_L2T_
			fprintf(stderr,"������ֹ,û��ƥ��ķǽӴ�����·��(AID)");
		#endif
			return ERR_NOAIDPATCH;
		}
	*/		
		break;
	}


	//20130521 banlance
	memcpy ( C_Apdu.data , "\x80\xca\x9f\x79",4);
	C_Apdu.length = 4 ;
	nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
	if (nRetCode!=OK)
	{
	#ifdef _DEBUG_L2T_
		fprintf(stderr,"80CA9f5d:uRetCode=%04X",nRetCode);
	#endif
		return (nRetCode) ;
	}
	
	memcpy(g9f79,R_Apdu.data+3,6);
	memset(uTmp,0,sizeof(uTmp));
	gBCD2Str(6,g9f79,uTmp);
	gn9f79=atoi(uTmp);
	
	memcpy ( C_Apdu.data , "\x80\xca\xdf\x62",4);
	C_Apdu.length = 4 ;
	nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
	if (nRetCode!=OK)
	{
	#ifdef _DEBUG_L2T_
		fprintf(stderr,"80CAdf62:uRetCode=%04X",nRetCode);
	#endif
		return (nRetCode) ;
	}
	memcpy(gdf62,R_Apdu.data+3,6);
	
	if(gn9f79<=0)
	{
		memcpy ( C_Apdu.data , "\x80\xca\xdf\x63",4);
		C_Apdu.length = 4 ;
		nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
		if (nRetCode!=OK)
		{
		#ifdef _DEBUG_L2T_
			fprintf(stderr,"80CAdf63:uRetCode=%04X",nRetCode);
		#endif
			return (nRetCode) ;
		}
		memcpy(gdf63,R_Apdu.data+3,6);
	}

	

	memset(uTmp,0,sizeof(uTmp));
	gBCD2Str(6,gdf62,uTmp);
	gndf62=atoi(uTmp);
	
	memset(uTmp,0,sizeof(uTmp));
	gBCD2Str(6,gdf63,uTmp);
	gndf63=atoi(uTmp);
	
	gInt2Bytes(gn9f79- gndf63,g_CardInfo->Balance);//����ʵ�����
	memcpy(gBalance,g_CardInfo->Balance,4);//����Ǯ�����	
	gShort32Bytes(gndf62,g_CardInfo->Overdrawlimited);//͸֧�޶�
#ifdef _DEBUG_GETINFO_
	printf("\n\r  gBuf0221:\n\r");
	for(i=0;i<41;i++)
	{
		if((i+1) % 16==0)
			printf("\x0d\x0a ");
		printf("%02X ",gBuf0221[i]);	
	}
		memcpy(g_CardInfo->Buf0221,gBuf0221+6,41);

		outbuf[0]=0x00;
	printf("\n\r  outbuf:\n\r");
	for(i=0;i<91;i++)
	{
		if((i+1) % 16==0)
			printf("\x0d\x0a ");
		printf("%02X ",outbuf[i]);	
	}	
#endif
	return 0;
}

int pf_mobile_ticket_get_0221(char *outbuf)
{
	unsigned int nRetCode=0,i;
	UBYTE uTmp[256],Diversity[32],pszInitData[64],pszBuf[128],Key[32],R_MAC[4];
	UBYTE uAID[256],uAID_Single[1+LENSINGLEAID+1],uAID_Single_Tmp[1+LENSINGLEAID+1];
	int nLen_AID,nLen_Tmp;
	int j;

		g_CardInfo=(D_CardInfo*)(outbuf+2);
		memcpy ( C_Apdu.data , "\x80\xb4\x00\xa8\x02\x02\x21",7);
		C_Apdu.length = 7 ;
	 	if(gdf61==0x81 || gdf61==0x82)
		{
			C_Apdu.data[4]=0x0a;
			memcpy ( C_Apdu.data+7 , ugRand_RMAC,8);
			C_Apdu.length = C_Apdu.length +8;
			memcpy(pszBuf,C_Apdu.data,15);
		}

		nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
		if (nRetCode!=OK)
		{
		#ifdef _DEBUG_L2T_
			fprintf(stderr,"0221:uRetCode=%04X",nRetCode);
		#endif
			return (nRetCode) ;
		}
		if(gdf61==0x81 || gdf61==0x82)
		{
			if(R_Apdu.length<70)
				return ERR_PAGRMAC;
		}
		else
		{
			if(R_Apdu.length>66)
				return ERR_PAGRMAC;
		}
		////��Ч �ֶ� δ��
		if(memcmp(R_Apdu.data+3,"\x01\x01\x00",3)!=0)
		{
			return ERR_TRIPLEFLAG0221;
		}
		if(g_TP_Head->R_MAC_Flag[0]==1 && (gdf61==0x81 || gdf61==0x82))
		{
			memset(pszBuf,0x00,sizeof(pszBuf));
			memcpy(pszBuf,R_Apdu.data,R_Apdu.length-6);
			memcpy(R_MAC,R_Apdu.data+64,4);

			memset(Diversity,0x00,16);

			memcpy(Diversity,gAPPAount_SN+3,8);
			memcpy(pszInitData,ugRand_RMAC,8);

			pszBuf[64]=0x80;
			memset(uTmp,0x00,sizeof(uTmp));
		
		 	nRetCode=PSAM_MAC(PSAM_NDX,2,Diversity,pszInitData,pszBuf,72,uTmp);
			if (nRetCode!=OK)
			{
				fprintf(stderr,"RMAC0221 PSAM_MAC:uRetCode=%04X",nRetCode);
				return (nRetCode) ;
			}
		
			fprintf(stderr,"\r\n 0221:R_MAC=%02X%02X%02X%02X \r\n",R_MAC[0],R_MAC[1],R_MAC[2],R_MAC[3]);

			if(memcmp(R_MAC,uTmp,4)!=0)
				return ERR_RMAC;

		}


#ifdef _DEBUG_GETINFO_
		printf("\n\r 0221:\n\r");
		for(i=0;i<R_Apdu.length;i++)
		{
			if((i+1) % 16==0)
				printf("\x0d\x0a ");
			printf("%02X ",R_Apdu.data[i]);	
		}
		printf("\n\r 0221:\n\r");
#endif
		//����0221 buffer
		memcpy(gBuf0221,R_Apdu.data,LEN0221);
		//memcpy(g_CardInfo->Buf0221,R_Apdu.data+6,sizeof(g_CardInfo->Buf0221));
		memcpy(g_CardInfo->Buf0221,R_Apdu.data+6,41);
	
		memcpy(g_CardInfo->AID,g9f06,uLen_g9f06);
	
#ifdef _GET0321_
		if(g_TP_Head->LCYH_Flag[0]==1)
		{
			//0321
			memcpy ( C_Apdu.data , "\x80\xb4\x00\xa8\x02\x03\x21",7);
			C_Apdu.length = 7 ;
			if(gdf61==0x81 || gdf61==0x82)
			{
				C_Apdu.data[4]=0x0a;
				memcpy ( C_Apdu.data+7 , ugRand_RMAC,8);
				C_Apdu.length = C_Apdu.length +8;
				memcpy(pszBuf,C_Apdu.data,15);
			}
			

			nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
			

			if (nRetCode!=OK)
			{
			#ifdef _DEBUG_L2T_
				fprintf(stderr,"2103:uRetCode=%04X",nRetCode);
			#endif
				return (nRetCode) ;
			}
			if(gdf61==0x81 || gdf61==0x82)
			{
				if(R_Apdu.length<70)
					return ERR_PAGRMAC;
			}
			else
			{
				if(R_Apdu.length>66)
					return ERR_PAGRMAC;
			}

			if(g_TP_Head->R_MAC_Flag[0]==1 && (gdf61==0x81 || gdf61==0x82))
			{
				memset(pszBuf,0x00,sizeof(pszBuf));
				memcpy(pszBuf,R_Apdu.data,R_Apdu.length-6);
				memcpy(R_MAC,R_Apdu.data+64,4);

				memset(Diversity,0x00,16);
	
				memcpy(Diversity,gAPPAount_SN+3,8);
				memcpy(pszInitData,ugRand_RMAC,8);
	
				pszBuf[64]=0x80;
				memset(uTmp,0x00,sizeof(uTmp));
			#ifdef _SOFTMAC_
				memcpy(Key,"\x8A\xA2\x94\x76\xDF\xBA\x3E\x5E\xAB\x97\x25\x13\x58\x92\xAE\x51",16);
				PSAM_SoftMAC(Key,Diversity,pszInitData,pszBuf,64,uTmp);
			#else	
			 	nRetCode=PSAM_MAC(PSAM_NDX,3,Diversity,pszInitData,pszBuf,72,uTmp);
				if (nRetCode!=OK)
				{
					fprintf(stderr,"RMAC0321 PSAM_MAC:uRetCode=%04X",nRetCode);
					return (nRetCode) ;
				}
			#endif
				fprintf(stderr,"\r\n 0321:R_MAC=%02X%02X%02X%02X \r\n",R_MAC[0],R_MAC[1],R_MAC[2],R_MAC[3]);

				if(memcmp(R_MAC,uTmp,4)!=0)
					return ERR_RMAC;

			}

			if(nRetCode == OK)
			{
				////��Ч �ֶ� δ��
				if(memcmp(R_Apdu.data+3,"\x01\x01\x00",3)!=0)
				{
					return ERR_TRIPLEFLAG0321;
				}
				memcpy(g_CardInfo->Buf0321,R_Apdu.data+7+2,sizeof(g_CardInfo->Buf0321));//20130808
			
				return OK;
				//break;
			}
			if (nRetCode!=27013)  //0x6985=27103
			{
			#ifdef _DEBUG_L2T_
				fprintf(stderr,"������ֹ  AID:uRetCode=%04X,%d",nRetCode,nRetCode);
			#endif
				return (nRetCode) ;
			}
		}
#endif
		if(nRetCode == OK)
		{
			return OK;
			//break;
		}
		if (nRetCode!=27013)  //0x6985=27103
		{
		#ifdef _DEBUG_L2T_
			fprintf(stderr,"������ֹ  AID:uRetCode=%04X,%d",nRetCode,nRetCode);
		#endif
			return (nRetCode) ;
		}

/*
		uPlace = uPlace + (LENSINGLEAID+2);
		
		if(uPlace>=nLen_AID)
		{
		#ifdef _DEBUG_L2T_
			fprintf(stderr,"������ֹ,û��ƥ��ķǽӴ�����·��(AID)");
		#endif
			return ERR_NOAIDPATCH;
		}
	*/	


}
//----------------------------------------------------------------------------
//����AFL����¼
//----------------------------------------------------------------------------
int ReadRecords()
{
	UBYTE uRecno_Begin,uRecno_End,uRecno,auth_flag,SFI;
	UBYTE uAUTH_With70=0,l_tlv;
	UBYTE	pfbuf[100];
	int i,nRetCode;
#ifdef _DEBUG_L2T_
	int j;
	UBYTE uTmp[128];
#endif
	//����AFL��
	uLen_gSDFORAUTH =0;
	memset(gSDFORAUTH,0x00,sizeof(gSDFORAUTH));
	for(i=0;i<nLen_AFL/4;i++)
	{
		uRecno_Begin=AFL[i*4+1];
#ifdef _DEBUG_L2T_
		fprintf(stderr,"����AFL��,SFI=%02X(%02X)",AFL[i*4],AFL[i*4] | 0x04);
#endif
		if(uRecno_Begin==0x00)
		{
		#ifdef _DEBUG_L2T_
			fprintf(stderr,"������ֹ,��1 ����¼�ļ�¼�ţ�����Ϊ0��");
		#endif
			return ERR_FIRSTRECNO ;
		}
		SFI = AFL[i*4]>>3;
		if(AFL[i*4]==0)
		{

		#ifdef _DEBUG_L2T_
			fprintf(stderr,"������ֹ,SFI����Ϊ0");
		#endif
			return ERR_SFI0 ;
		}	 
		if((AFL[i*4] | 0x04)==0xf8)
		{
		#ifdef _DEBUG_L2T_
			fprintf(stderr,"������ֹ,SFI����Ϊ31");
		#endif
			return ERR_SFI31 ;
		}	 
		if(SFI>=0x1f)
		{
		#ifdef _DEBUG_L2T_
			fprintf(stderr,"������ֹ,SFI=%02X",SFI);
		#endif
			return ERR_SFI31 ;
		}	 
		uRecno_End = AFL[i*4+2];
		if(uRecno_End<uRecno_Begin)
		{
		#ifdef _DEBUG_L2T_
			fprintf(stderr,"������ֹ,���һ����¼�ļ�¼�Ŵ���");
		#endif
			return ERR_LASTRECNO ;
		}
		
		auth_flag= AFL[i*4+3];
		if(auth_flag >(uRecno_End-uRecno_Begin+1))
		{
		#ifdef _DEBUG_L2T_
			fprintf(stderr,"������ֹ,����������֤��¼�ļ�¼�Ŵ���");
		#endif
			return ERR_AUTHRECNO ;
		}
		for(uRecno=uRecno_Begin;uRecno<=uRecno_End;uRecno++)
		{
				
#ifdef _DEBUG_L2T_
	fprintf(stderr,"����AFL��,SFI=%02x,uRecno=%02X,uRecno_Begin=%02X,uRecno_End=%02X, auth_flag=%02X",SFI,uRecno,uRecno_Begin,uRecno_End,auth_flag);
#endif
			//��������
			//if(uRecno==uRecno_End)
			//	return OK;
			memcpy ( C_Apdu.data , "\x00\xB2",2);
			C_Apdu.data[2]= uRecno;
			C_Apdu.data[3]= AFL[i*4] | 0x04;
			C_Apdu.data[4]=0x00;
			C_Apdu.length = 5 ;
			nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
			//fprintf(stderr,"\r\n PASSCARD:uRetCode=%d,i=%d,%d,%d,%d \r\n",nRetCode,i,nLen_AFL/4-1,uRecno,uRecno_End);
#ifdef DEBUG_TIME_CPU
	memcpy(pfbuf, "\x08\x00\x55\x00", 4);
	pfbuf[3] = uRecno;
	ReaderResponse(csc_comm, 0x00, 0xF0, pfbuf, 4);
#endif
			if (nRetCode!=OK)
			{
			#ifdef _DEBUG_L2T_
				fprintf(stderr,"������ֹ:uRetCode=%04X",nRetCode);
			#endif
				//fprintf(stderr,"\r\n PASSCARD:uRetCode=%d,i=%d,%d,%d,%d \r\n",nRetCode,i,nLen_AFL/4-1,uRecno,uRecno_End);
				//�����һ����¼��ʱ����������
				if((i==nLen_AFL/4-1) && (uRecno==uRecno_End) && (nRetCode<0 || nRetCode==255 ))
				{
					return ERR_PASS_CARD;
				}
				return (nRetCode) ;
			}
			//��¼ģ��
			if(R_Apdu.data[0]!=0x70 && SFI<11)
			//if(R_Apdu.data[0]!=0x70 && SFI<0x11)
			{
			#ifdef _DEBUG_L2T_
				fprintf(stderr,"������ֹ,��¼ģ�����[%02X]",R_Apdu.data[0]);
			#endif
				return ERR_MODEREC ;
			}		
			//��¼����
			if(R_Apdu.data[0]==0x70)
			{
				if(R_Apdu.data[1]==0x81)
				{
					if(R_Apdu.data[2]!=R_Apdu.length-5)
					{
					#ifdef _DEBUG_L2T_
						fprintf(stderr,"������ֹ,��¼�����ֽڴ���[%02X]",R_Apdu.data[2]);
					#endif
						return ERR_LENBYTE ;
					}
				}
				else
				{	
					if(R_Apdu.data[1]!=R_Apdu.length-4)
					{
					#ifdef _DEBUG_L2T_
						fprintf(stderr,"������ֹ,��¼�����ֽڴ���[%02X]",R_Apdu.data[1]);
					#endif
						return ERR_LENBYTE ;
					}
				}		
			}	
			if(auth_flag>0)
			{
				if( R_Apdu.data[0]==0x70)
				{
					if(SFI>=11)
					{
						memcpy(gSDFORAUTH+uLen_gSDFORAUTH,R_Apdu.data,R_Apdu.length-2);
						uLen_gSDFORAUTH = uLen_gSDFORAUTH + R_Apdu.length-2;
					}
					else
					{
						l_tlv = R_Apdu.data[1];
						if(l_tlv ==0x81)
						{
							l_tlv = R_Apdu.data[2];
							memcpy(gSDFORAUTH+uLen_gSDFORAUTH,R_Apdu.data+3,l_tlv);
						}
						else
						{
							memcpy(gSDFORAUTH+uLen_gSDFORAUTH,R_Apdu.data+2,l_tlv);
						}
					
						uLen_gSDFORAUTH = uLen_gSDFORAUTH + l_tlv;
					}
				}
				else
				{
					uAUTH_With70=0x01;
				}
			#ifdef _DEBUG_L2T_
				fprintf(stderr,"������֤��̬����,uLen_gSDFORAUTH=%d,l_tlv=%02X(hex),auth_flag=%02x(hex):",uLen_gSDFORAUTH,l_tlv,auth_flag);
				for(j=0;j<uLen_gSDFORAUTH;j++)
					fprintf(stderr,"%02X ",gSDFORAUTH[j]);
				fprintf(stderr,"\n\r");
			#endif
				auth_flag --;
			}
			GetTagFrmTLV( R_Apdu.data,R_Apdu.length-2);
			GetTagFrmTLV( g70,uLen_g70);
			if(g5f24[0]>=0x50)
			{
				return ERR_g5f24;
			}
			if(memcmp(g5f24,"\x00\x00\x00",3)&& (memcmp(g9a,g5f24,3)>0))
			{
				return ERR_g5f24;
			}
			if(uTagRepeat_5a>1 )
			{
				return ERR_DBL5a;
			}
			//fprintf(stderr,"\n\ruTagRepeat_57=%02X",uTagRepeat_57);
			if(uTagRepeat_57>1 )
			{
			#ifdef _DEBUG_L2T_
				fprintf(stderr,"������ֹ,����2 ��Ч�����ظ�.");
				return ERR_DBL57;
			#endif
			}
			if(uTagRepeat_5f24>1 )
			{
				return ERR_DBL5F24;
			}
	
		}
		
	}
	if(uAUTH_With70==0x01)
	{
		return -70;
	}

	//�Ӳ�����֤���������ұ�ǩ
	//57
	for(i=0;i<uLen_gSDFORAUTH;i++)
	{
		if(gSDFORAUTH[i]==0x57)
		{
			l_tlv = gSDFORAUTH[i+1];
			if(l_tlv<8)
				continue;
			if(l_tlv>19)
				continue;
				
			memset(g57,0x00,sizeof(g57));
			memcpy(g57,gSDFORAUTH+i+2,l_tlv);
			uLen_g57=l_tlv;
		#ifdef _DEBUG_L2T_
			fprintf(stderr,"ȡ g57,l_tlv=%02X:",l_tlv);
			for(j=0;j<l_tlv;j++)

				fprintf(stderr,"%02X ",g57[j]);
			fprintf(stderr,"\n\r");
		#endif
						
			break;
		}
	}
	#ifdef _DEBUG_L2T_
	fprintf(stderr,"g82[0]:%02X",g82[0]);
	#endif
	//20120419
	if((g82[0] & 0x60)==0x40)
	{
	#ifdef _DEBUG_L2T_
		fprintf(stderr,"\n\r���׾ܾ�");
		if(memcmp(g9f5d,"\x00\x00\x00\x00\x00\x00",6))
		{
			sprintf((char*)uTmp,"%02x%02x%02x%02x%02x%02x",g9f5d[0],g9f5d[1],g9f5d[2],g9f5d[3],g9f5d[4],g9f5d[5]);
		}
	#endif
	  	return ERR_NO82;
	}
	
	if((g82[0] & 0x60)==0x00)
	{
		return ERR_NO82;
	}
	
	
	if(g9f4a[0]!=0x82 && g9f4a[0]!=0x00 )
	{
		printf("\r\n g9f4a=%02x \r\n ",g9f4a[0]);
		return ERR_g9f4a;
	}
	
	
	if(uLen_g90==0 )
	{
		return ERR_NOTAG90;
	}
	if(uLen_g5a==0 )
	{
		return ERR_NOPAN;
	}
	if(uLen_g8f==0 )
	{
		return ERR_NOTAG8f;
	}
	
	
	if(uLen_g9f32==0 )
	{
		return ERR_9f32;
	}
	
	
		
	if(uLen_g9f46==0 )
	{
		return ERR_9f46;
	}
	
		
	if(uLen_g9f47==0 )
	{
		return ERR_9f47;
	}
	if(uLen_g9f4b==0 )
	{
		return ERR_9f4b;
	}
	return OK;
}
//
UBYTE ConvertTransType(UBYTE uTransType)
{
	switch(uTransType)
	{
		case 0:
		case 1:
			return 0x11;
		case 2:
			return 0x12;
		case 3:
			return 0x1f;
		case 5:
			return 0x04;
		case 6:
			return 0x26;
		case 7:
			return 0x27;
		case 8:
			return 0x19;
		case 9:
			return 0x35;
		case 10:
			return 0x34;
		case 11:
			return 0x06;
		case 12:
			return 0x07;
		case 13:
			return 0x14;
		default:
			return 0xff;
	}
} 
//��У��   У��λ��0ʱ������λ��Ӧ����������1����У��λ��1ʱ������λӦ����ż����1
unsigned char GetOddParityBit(unsigned char Data)//��У�� 
{
	unsigned char Count= 0;
	unsigned char i;
	unsigned char ReturnValue = 0;
	//printf("\n\r in GetOddParityBit:Data=%02X(HEX),",Data);
	for(i=0;i<8;i++)
	{

		if(Data&0x01==0x01)
		{
			Count++;
		}
		Data=Data>>1;
	}

	if((Count&0x01)==0x01)//������1
	{
		ReturnValue = 0;
	}
	else
	{
		ReturnValue = 1;
	}
	//printf(" result=%02x \n\r",ReturnValue);
	return ReturnValue;
}

//----------------------------------------------------------------------------
//������չӦ�����ݻ���
//20130619
//----------------------------------------------------------------------------
int UpdateCAppCache(int nTrade,char *inbuf)
{
	UBYTE uTmp[256],uStatus=0x00,uTradeTime[7];
	UBYTE buf[128],MAC[4],Diversity[16],Key[16],pszInitData[64],pszBuf[128];
	int nTotal=0;
	int nYJE=0,nAmount=0,nRetCode;
	UBYTE	pfbuf[100];
	
	UBYTE i;

	//update 0221
	memcpy(uTmp,"\x84\xDE\x00\xa8\x44",5);
	memcpy(uTmp+5,"\x02\x21\x3D\x01\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",16);
	memcpy(uTmp+21,"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",16);
	memcpy(uTmp+21+16,"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",16);
	memcpy(uTmp+21+32,"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",16);

	memcpy(uTmp+5,gBuf0221,LEN0221);
	g_Data0221 = (F_Data0221*)(uTmp+5);
	//���ݽ������ͼ�gBuf0221��д0221����
	if(nTrade==10041)
	{
		memcpy(uTradeTime,gR_Entry->TradeTime,7);
		//���ۼ���0
		if(memcmp(g_Data0221->EntryTime,gR_Entry->TradeTime,3)!=0)
		{
			memcpy(g_Data0221-> MonthAmount,"\x00\x00\x00",3);	//���¹����ͨ�ۼƽ��
			memcpy(g_Data0221->MonthCount,"\x00\x00",2);	//���¹����ͨ�ۼƴ���
		}

		//������վ :Bit3�� 0��ʾ������վ      1��ʾ��վ���˽�վ
		//if(inbuf[10] & 0x08==0x00)
		//printf("\n\r Entry Flag=%02X,inbuf10=%02X\n\r",gR_Entry->Flag,inbuf[10]);
		if((gR_Entry->Flag & 0x08)==0x00)
		{
			g_Data0221->Status =0x00;//��
			memcpy(g_Data0221->EntryTime,gR_Entry->TradeTime,sizeof(g_Data0221->EntryTime));
			memcpy(g_Data0221->EntryTermNo,gTerminalNo,sizeof(g_Data0221->EntryTermNo));
			//20130722
			memset(g_Data0221->Amount,0x00,sizeof(g_Data0221->Amount));
		}
		else
		{
			//����
			g_Data0221->Status = (g_Data0221->Status | 0x10) ; //bit4 =1��ʾ��վ���˽�վ			
			g_Data0221->Status = (g_Data0221->Status & 0x3E);//��bit7 bit6
		}
		//��ʱ 20130722
		if((gR_Entry->Flag & 0x10)==0x10)
		{
			g_Data0221->Status = g_Data0221->Status | 0x20;
		}

		g_Data0221->Status = (g_Data0221->Status | 0x40) ; //bit7 bit6 =0 1

		if(GetOddParityBit(g_Data0221->Status)==0)
			g_Data0221->Status =( g_Data0221->Status | 0x01);
		uStatus = g_Data0221->Status;
		//g_Data0221->Status = 0x41;
		memcpy(g_Data0221->LastTime,gR_Entry->TradeTime,sizeof(g_Data0221->LastTime));//ĩ��ʱ��
		memcpy(g_Data0221->LastTermNo,gTerminalNo,sizeof(g_Data0221->LastTermNo));//ĩ���ն�
		
	}
	//��վ
	if(nTrade==10042)
	{
		memcpy(uTradeTime,gR_Exit->TradeTime,7);
		//����������Ϊ0��1��9��10�򡰳�վ����ʱ�䡱������վ�����ͨ�ն˱�š�����ĩ�ʽ���ʱ�䡱����ĩ�ʹ����ͨ�ն˱�š�������д��
		//if(gR_Exit->TradeType==0 || gR_Exit->TradeType==1 || gR_Exit->TradeType==9 || gR_Exit->TradeType==10)
		//{   
			memcpy(g_Data0221->ExitTime,gR_Exit->TradeTime,sizeof(g_Data0221->ExitTime));
			memcpy(g_Data0221->ExitTermNo,gTerminalNo,sizeof(g_Data0221->EntryTermNo));
		//}
		memcpy(g_Data0221->LastTime,gR_Exit->TradeTime,sizeof(g_Data0221->LastTime));//ĩ��ʱ��
		memcpy(g_Data0221->LastTermNo,gTerminalNo,sizeof(g_Data0221->LastTermNo));//ĩ���ն�
			
		//ʵ�ʽ��׽��
		nYJE=0;
		nAmount = 0;
		gBytes2Short3(gR_Exit->Amount,&nAmount);
		gBytes2Short(g_Data0221->Amount,&nYJE);
		nAmount = nAmount +nYJE;
		gShort2Bytes(nAmount,g_Data0221->Amount);;//ʵ�ʽ��׽��=ʵ�ʽ��׽��+���׽��
		
	
	
		gBytes2Short3(g_Data0221->MonthAmount,&nYJE);
		gBytes2Short3(gR_Exit->Amount,&nAmount);
		nTotal = nYJE + nAmount;
	
		gShort32Bytes(nTotal,g_Data0221->MonthAmount);
		
		
//printf("\n\r g_Data0221->MonthCount=%02X %02X\n\r",g_Data0221->MonthCount[0],g_Data0221->MonthCount[1]);
		nTotal =0;
		gBytes2Short(g_Data0221->MonthCount,&nTotal);

//printf("\n\r nTotal= %d \n\r",nTotal);

		nTotal++;
		gShort2Bytes(nTotal,g_Data0221->MonthCount);	//�����ۼƴ���

//printf("\n\r nTotal= %d ,g_Data0221->MonthCount=%02X %02X\n\r",nTotal,g_Data0221->MonthCount[0],g_Data0221->MonthCount[1]);

		g_Data0221->Status = (g_Data0221->Status & 0x3e);
		g_Data0221->Status = (g_Data0221->Status | 0x80);
		
		//20130722 
		if(gR_Exit->TradeType==6 || gR_Exit->TradeType==7 || gR_Exit->TradeType==8 || gR_Exit->TradeType==9)
		{
			g_Data0221->Status = (g_Data0221->Status | 0x08);		
		}

		g_Data0221->Status = (g_Data0221->Status & 0xEF) ; //���վ���˽�վflag,20130724	
		if(GetOddParityBit(g_Data0221->Status)==0)
			g_Data0221->Status = (g_Data0221->Status | 0x01);
		//g_Data0221->Status =  0x81;

		uStatus = g_Data0221->Status;
	}
	
	//����
	if(nTrade==10043)
	{
		memcpy(uTradeTime,gR_Update->TradeTime,7);
		//���ۼ���0,20130724
		if(memcmp(g_Data0221->EntryTime,gR_Update->TradeTime,3)!=0)
		{
			memcpy(g_Data0221-> MonthAmount,"\x00\x00\x00",3);	//���¹����ͨ�ۼƽ��
			memcpy(g_Data0221->MonthCount,"\x00\x00",2);	//���¹����ͨ�ۼƴ���
		}


		g_Data0221->Status = gR_Update->Status;
		uStatus = g_Data0221->Status;
		if(GetOddParityBit(g_Data0221->Status)==0)
			g_Data0221->Status = (g_Data0221->Status | 0x01);

		memcpy(g_Data0221->EntryTermNo,gR_Update->EntryTermNo,4);
		memcpy(g_Data0221->ExitTermNo,gR_Update->ExitTermNo,4);
				
		memcpy(g_Data0221->LastTime,gR_Update->TradeTime,7);
		memcpy(g_Data0221->LastTermNo,gTerminalNo,sizeof(g_Data0221->LastTermNo));//ĩ���ն�
		//Bit2:0��ʾ����д��ʵ�ʽ��׽�� 1��ʾ��ʵ�ʽ��׽������
		if((gR_Update->TimeFlag & 0x04)==0x04)
		{
			memset(g_Data0221->Amount,0x00,2);
		} 	
		//Bit1:0��ʾ����д��վ��������/ʱ���� 1��ʾ�ò����еĽ�������/ʱ���д��վ��������/ʱ����
		if((gR_Update->TimeFlag & 0x02)==0x02)
		{
			memcpy(g_Data0221->ExitTime,gR_Update->TradeTime,7);
		} 	

		if((gR_Update->TimeFlag & 0x01)==0x01)
		{
			memcpy(g_Data0221->EntryTime,gR_Update->TradeTime,7);
		} 

	}
	memset(Diversity,0x00,16);
	//memcpy(Diversity,"\x03\x00\x05\x15\x91\xff\xff\x01",8);
	//gAPPAount_SN
	memcpy(Diversity,gAPPAount_SN+3,8);
	memcpy(pszInitData,"\x00\x00\x00\x00\x00\x00\x00\x00",8);
	memcpy(pszInitData+6,gATC,2);
	
	memset(pszBuf,0x00,sizeof(pszBuf));
	memcpy(pszBuf,uTmp,69);
	memset(buf,0x00,sizeof(buf));
#ifdef _SOFTMAC_
	memset(Key,0x00,16);
	memcpy(Key,"\xB0\x94\x19\x02\xD0\xC2\x45\xA7\x73\x38\x9B\x68\x5B\x9D\x29\x26",16);
	PSAM_SoftMAC(Key,Diversity,pszInitData,pszBuf,69,buf);
#else
	memcpy(pszBuf+69,"\x80\x00\x00",3);
	memset(buf,0x00,sizeof(buf));

 	//nRetCode=PSAM_MAC(PSAM_NDX,2,Diversity,pszInitData,pszBuf,69+3,buf);
 	nRetCode=PSAM_MAC_FA(PSAM_NDX,2,Diversity,pszInitData,pszBuf,69+3,buf);
	if (nRetCode!=OK)
	{
		fprintf(stderr,"0221 PSAM_MAC:uRetCode=%04X",nRetCode);
		return (nRetCode) ;
	}
#endif
#ifdef DEBUG_TIME_CPU
	memcpy(pfbuf, "\x08\x00\x52\x01", 4);
	ReaderResponse(csc_comm, 0x00, 0xF0, pfbuf, 4);
#endif
	
	sem_init(&g_801Await, 0, 0);
	sem_init(&g_801Areturn, 0, 0);
	nRetCode = pthread_create(&g_pth001E, NULL, &pth001E, NULL);
	memcpy(MAC,buf,4);
	memcpy ( C_Apdu.data , "\x84\xDE\x00\xa8\x44",5);
	memcpy ( C_Apdu.data + 5, uTmp+5,32+32);
	memcpy ( C_Apdu.data + 37+32, MAC,4);
	C_Apdu.length = 41+32 ;
	nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
	if (nRetCode!=OK)
	{
		//fprintf(stderr,"update 0221:uRetCode=%04X",nRetCode);
		return (nRetCode) ;
	}
	if(gdf61==0x81 || gdf61==0x82)
	{
		if(R_Apdu.length<6)
			return ERR_PAGRMAC;
	}
	else
	{	
		if(R_Apdu.length>2)
			return ERR_PAGRMAC;
	}
#ifdef DEBUG_TIME_CPU
	memcpy(pfbuf, "\x08\x00\x52\x03", 4);
	ReaderResponse(csc_comm, 0x00, 0xF0, pfbuf, 4);
#endif

	//20130827
	if(g_TP_Head->R_MAC_Flag[0]==1 && (gdf61==0x81 || gdf61==0x82))
	{
		memcpy(Diversity,gAPPAount_SN+3,8);
		memcpy(pszInitData,"\x00\x00\x00\x00\x00\x00\x00\x00",8);

		memcpy(pszInitData+4,MAC,4);
	
		memset(pszBuf,0x00,sizeof(pszBuf));
		memcpy(pszBuf,"\x90\x00\x80\x00",4);
		memset(buf,0x00,sizeof(buf));
	#ifdef _SOFTMAC_
		memset(Key,0x00,16);
		memcpy(Key,"\xB0\x94\x19\x02\xD0\xC2\x45\xA7\x73\x38\x9B\x68\x5B\x9D\x29\x26",16);
		PSAM_SoftMAC(Key,Diversity,pszInitData,pszBuf,8,buf);
	#else
		
		memset(buf,0x00,sizeof(buf));

	 	nRetCode=PSAM_MAC(PSAM_NDX,2,Diversity,pszInitData,pszBuf,8,buf);
		if (nRetCode!=OK)
		{
			fprintf(stderr,"0221 PSAM_MAC:uRetCode=%04X",nRetCode);
			return (nRetCode) ;
		}
	#endif
		fprintf(stderr,"\r\n 0221 R_MAC:%02X%02X%02X%02X \r\n",buf[0],buf[1],buf[2],buf[3]);
			
		if(memcmp(R_Apdu.data,buf,4)!=0)
			return ERR_RMAC;

	}
#ifdef _GET0321_	

	if((g_TP_Head->LCYH_Flag[0]==1) && (nTrade==10042))
	{
		//---- update 0321 --------------
		memset(uTmp,0x00,sizeof(uTmp));
		memcpy(uTmp,"\x84\xDE\x00\xa8\x44",5);
		memcpy(uTmp+5,"\x03\x21\x3D\x01\x01",5);

		//20130722
		uTmp[10]=0x00 ;//δ����
				
		uTmp[11]=uStatus ;
		uTmp[12]=gTerminalNo[1] ;//20130808
		uTmp[13]=gTerminalNo[3] ;//20130808
		uTmp[14]=0xC0;//��ҵ��Ϣ//20130808
		uTmp[14] = uTmp[14] | (gTerminalNo[0] & 0x3F);//20130808
		memcpy(uTmp+13+2,uTradeTime+2,4);//20130808

		memset(Diversity,0x00,16);
		memcpy(Diversity,"\x03\x00\x05\x15\x91\xff\xff\x01",8);
		memcpy(Diversity,gAPPAount_SN+3,8);
	
		memcpy(pszInitData,"\x00\x00\x00\x00\x00\x00\x00\x00",8);
		memcpy(pszInitData+6,gATC,2);
	
		memset(pszBuf,0x00,sizeof(pszBuf));
		memcpy(pszBuf,uTmp,69);
		memset(buf,0x00,sizeof(buf));
		#ifdef _SOFTMAC_
		memset(Key,0x00,16);
		memcpy(Key,"\x8A\xA2\x94\x76\xDF\xBA\x3E\x5E\xAB\x97\x25\x13\x58\x92\xAE\x51",16);
		PSAM_SoftMAC(Key,Diversity,pszInitData,pszBuf,69,buf);
		#else
		memcpy(pszBuf+69,"\x80\x00\x00",3);
		nRetCode=PSAM_MAC(PSAM_NDX,3,Diversity,pszInitData,pszBuf,69+3,buf);
		if (nRetCode!=OK)
		{
			fprintf(stderr,"0321 PSAM_MAC:uRetCode=%04X",nRetCode);
			return (nRetCode) ;
		}
		#endif
		memcpy(MAC,buf,4);
	
		memcpy ( C_Apdu.data , "\x84\xDE\x00\xa8\x44",5);
		memcpy ( C_Apdu.data + 5, uTmp+5,32+32);
		memcpy ( C_Apdu.data + 37+32, MAC,4);
		C_Apdu.length = 41+32 ;
		nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
		if (nRetCode!=OK)
		{
			fprintf(stderr,"update 0321:uRetCode=%04X",nRetCode);
			return (nRetCode) ;
		}
		if(gdf61==0x81 || gdf61==0x82)
		{
			if(R_Apdu.length<6)
				return ERR_PAGRMAC;
		}
		else
		{
			if(R_Apdu.length>2)
				return ERR_PAGRMAC;
		}
		//20130827
		if(g_TP_Head->R_MAC_Flag[0]==1 && (gdf61==0x81 || gdf61==0x82))
		{
			memcpy(Diversity,gAPPAount_SN+3,8);
			memcpy(pszInitData,"\x00\x00\x00\x00\x00\x00\x00\x00",8);

			memcpy(pszInitData+4,MAC,4);
	
			memset(pszBuf,0x00,sizeof(pszBuf));
			memcpy(pszBuf,"\x90\x00\x80\x00",4);
			memset(buf,0x00,sizeof(buf));
		
		
			memset(buf,0x00,sizeof(buf));

		 	nRetCode=PSAM_MAC(PSAM_NDX,3,Diversity,pszInitData,pszBuf,8,buf);
			if (nRetCode!=OK)
			{
				fprintf(stderr,"0321 PSAM_MAC:uRetCode=%04X",nRetCode);
				return (nRetCode) ;
			}
		
			fprintf(stderr,"\r\n 0321 R_MAC:%02X%02X%02X%02X \r\n",buf[0],buf[1],buf[2],buf[3]);
			if(memcmp(R_Apdu.data,buf,4)!=0)
				return ERR_RMAC;

		}
	}
#endif	
	

	//---- update 1e �ļ� --------------
	memset(uTmp,0x00,sizeof(uTmp));
	memcpy(uTmp,"\x84\xDE\x00\xf0\x24",5);
	g_Data1E = (F_Data1E* )(uTmp+5);
	//fprintf(stderr,"\n\rnTrade=%d",nTrade);
	if(nTrade==10041)
	{
		g_Data1E->TradeType=0x11;//0x99;//trade type
		memcpy(g_Data1E->TermNo,gTerminalNo,4);//trade termimal
		memcpy(g_Data1E->Amount,"\x00\x00\x00\x00",4);//trade amount
		memcpy(g_Data1E->TradeTime,gR_Entry->TradeTime,7);//trade datetime
		memcpy(g_Data1E->BalanceAft,gBalance,4);//20130801
	
	}	
	if(nTrade==10042)
	{
		g_Data1E->TradeType=ConvertTransType(gR_Exit->TradeType);
		memcpy(g_Data1E->TermNo,gTerminalNo,4);//trade termimal
		memcpy(g_Data1E->Amount+1,gR_Exit->Amount,3);//trade amount
		memcpy(g_Data1E->TradeTime,gR_Exit->TradeTime,7);//trade datetime
		//���׺����
		//fprintf(stderr,"\n\rgBalance��%02X%02X%02X%02X",gBalance[0],gBalance[1],gBalance[2],gBalance[3]);
		gBytes2Int(gBalance,&nYJE);
		gBytes2Short3(gR_Exit->Amount,&nAmount);
		nYJE = nYJE- nAmount;
		gInt2Bytes(nYJE,g_Data1E->BalanceAft);
	}

	if(nTrade==10043)
	{
		g_Data1E->TradeType=0x14;
		memcpy(g_Data1E->TermNo,gTerminalNo,4);//trade termimal
		memcpy(g_Data1E->Amount+1,gR_Update->Amount,3);//trade amount
		memcpy(g_Data1E->TradeTime,gR_Update->TradeTime,7);//trade datetime
		//���׺����
		//fprintf(stderr,"\n\rgBalance��%02X%02X%02X%02X",gBalance[0],gBalance[1],gBalance[2],gBalance[3]);
		gBytes2Int(gBalance,&nYJE);
		gBytes2Short3(gR_Update->Amount,&nAmount);
		nYJE = nYJE- nAmount;
		gInt2Bytes(nYJE,g_Data1E->BalanceAft);
	}
	
	memset(Diversity,0x00,16);
	memcpy(Diversity,gAPPAount_SN+3,8);
	
	memcpy(pszInitData,"\x00\x00\x00\x00\x00\x00\x00\x00",8);
	memcpy(pszInitData+6,gATC,2);
	
	memset(pszBuf,0x00,sizeof(pszBuf));
	memcpy(pszBuf,uTmp,37);
	memset(buf,0x00,sizeof(buf));
#ifdef _SOFTMAC_
	memset(Key,0x00,16);
	memcpy(Key,"\x1A\xA1\xE3\x73\x1F\x01\x46\xD5\x5E\x1F\xC1\x20\xCE\xC7\x76\xCD",16);

	PSAM_SoftMAC(Key,Diversity,pszInitData,pszBuf,37,buf);
#else	
	memcpy(pszBuf+37,"\x80\x00\x00",3);
	sem_wait(&g_801Areturn);
	//nRetCode=PSAM_MAC(PSAM_NDX,4,Diversity,pszInitData,pszBuf,37+3,buf);
	nRetCode = OK;
	if (nRetCode!=OK)
	{
		fprintf(stderr,"PSAM_MAC:uRetCode=%04X",nRetCode);
		return (nRetCode) ;
	}
#endif	
#ifdef DEBUG_TIME_CPU
	memcpy(pfbuf, "\x08\x00\x53\x00", 4);
	ReaderResponse(csc_comm, 0x00, 0xF0, pfbuf, 4);
#endif


	memcpy(MAC,buf,4);
	
	memcpy ( C_Apdu.data , "\x84\xDE\x00\xf0\x24",5);
	//
	memset(uTmp+5, 0x00, 32);
	memcpy ( C_Apdu.data + 5, uTmp+5,32);
	memcpy ( C_Apdu.data + 37, mac_001e,4);
	C_Apdu.length = 41;
	nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
	if (nRetCode!=OK)
	{
		fprintf(stderr,"update 1E:uRetCode=%04X",nRetCode);
		//return (nRetCode) ;
	}
	//20130827
	if(gdf61==0x81 || gdf61==0x82)
	{
		if(R_Apdu.length<6)
			return ERR_PAGRMAC;
	}
	else
	{
		if(R_Apdu.length>2)
			return ERR_PAGRMAC;
	}
	if(g_TP_Head->R_MAC_Flag[0]==1 && (gdf61==0x81 || gdf61==0x82))
	{
		memcpy(Diversity,gAPPAount_SN+3,8);
		memcpy(pszInitData,"\x00\x00\x00\x00\x00\x00\x00\x00",8);

		memcpy(pszInitData+4,MAC,4);

		memset(pszBuf,0x00,sizeof(pszBuf));
		memcpy(pszBuf,"\x90\x00\x80\x00",4);
		memset(buf,0x00,sizeof(buf));
	#ifdef _SOFTMAC_
		memset(Key,0x00,16);
		memcpy(Key,"\xB0\x94\x19\x02\xD0\xC2\x45\xA7\x73\x38\x9B\x68\x5B\x9D\x29\x26",16);
		PSAM_SoftMAC(Key,Diversity,pszInitData,pszBuf,8,buf);
	#else
	
		memset(buf,0x00,sizeof(buf));

	 	nRetCode=PSAM_MAC(PSAM_NDX,4,Diversity,pszInitData,pszBuf,8,buf);
		if (nRetCode!=OK)
		{
			fprintf(stderr,"1E PSAM_MAC:uRetCode=%04X",nRetCode);
			return (nRetCode) ;
		}
	#endif
		fprintf(stderr,"\r\n 1E R_MAC:%02X%02X%02X%02X \r\n",buf[0],buf[1],buf[2],buf[3]);
		if(memcmp(R_Apdu.data,buf,4)!=0)
			return ERR_RMAC;

	}
#ifdef DEBUG_TIME_CPU
	memcpy(pfbuf, "\x08\x00\x53\x01", 4);
	ReaderResponse(csc_comm, 0x00, 0xF0, pfbuf, 4);
#endif
	return OK;

}
//#define _DEBUG_GENOUT_
void GenOutBuf(UBYTE uDDAFlag,char* outbuf)
{
	int i,nRetCode;
#ifdef _DEBUG_GENOUT_
	printf("\n\r gen outbuf: \n\r");
#endif
	outbuf[0]=0x00;
	gD_Entry = (D_Entry*)(outbuf+1);
	memset(outbuf,0x00,sizeof(D_Entry)+1);
#ifdef _DEBUG_GENOUT_
	printf("\n\r uLen_g9f26=%d,uLen_g9f27=%d,uLen_g9f08 =%d \n\r",uLen_g9f26,uLen_g9f27,uLen_g9f08);
#endif	
	

#ifdef _DEBUG_GENOUT_
	printf("\n\r uLen_g9f26=%d,uLen_g9f27=%d,uLen_g9f10 =%d \n\r",uLen_g9f26,uLen_g9f27,uLen_g9f10);
#endif

	gD_Entry->EntryData[0] = uLen_g9f26;
	memcpy(gD_Entry->EntryData+1,g9f26,sizeof(gD_Entry->EntryData)-1);
	
	//AppInfoData  //Ӧ����Ϣ����

	gD_Entry->AppInfoData[0]=uLen_g9f27;
	gD_Entry->AppInfoData[1]=g9f27;
	//20130722 ��վ�������ݺ˶��Ƿ���ȷ
	gD_Entry->IssueBankData[0]= uLen_g9f10;
	memcpy(gD_Entry->IssueBankData+1,g9f10,sizeof(gD_Entry->IssueBankData)-1);

	gD_Entry->Rand[0]=4;
	memcpy(gD_Entry->Rand+1,ugRand,4);
	
	gD_Entry->Counter[0]=2;
	memcpy(gD_Entry->Counter+1,gATC,2);	
	
	gD_Entry->TVR[0]=5;
	memcpy(gD_Entry->TVR+1,g95,5);	

	gD_Entry->AIP[0]=uLen_g82;
	memcpy(gD_Entry->AIP+1,g82,uLen_g82);
#ifdef _DEBUG_GENOUT_
	printf("\n\r uLen_g82 =%d \n\r",uLen_g82);
#endif
	gD_Entry->TermFunc[0]=3;
	memcpy(gD_Entry->TermFunc+1,g9f33,3);
	//Ӧ�ð汾��
	gD_Entry->AppVer[0]=uLen_g9f08;
	memcpy(gD_Entry->AppVer+1,g9f08,2);

	
	// AuthCode[1+3];//�����ֽ𷢿�����Ȩ��
	gD_Entry->AuthCode[0]=uLen_g9f74;
	memcpy(gD_Entry->AuthCode+1,g9f74,sizeof(gD_Entry->AuthCode)-1);


	// ProductID[1+16];		//��Ʒ��Ϣ��ʶ
	gD_Entry->ProductID[0]=uLen_g9f63;
	memcpy(gD_Entry->ProductID+1,g9f63,sizeof(gD_Entry->ProductID)-1);

	gD_Entry->DDAFlag=uDDAFlag;
#ifdef  _DEBUG_GENOUT_
	printf("\n\r  L2Process outbuf:\n\r");
	for(i=0;i<(sizeof(D_Entry)+1);i++)
	{
		if((i+1) % 16==0)
			printf("\x0d\x0a ");
		printf("%02X ",outbuf[i]);	
	}
	printf("\x0d\x0a ");
#endif
	return ;
}
//#define _DEBUG_LB_WR_
int  L2Buffer_WR(UBYTE* uTransNum)
{
	int ret,i;
	UWORD uAddress;
	UBYTE uTmp[2];
#ifdef _DEBUG_LB_WR_
	printf("\n\r l2buff_wr nLen_g77=%d:",uLen_g77);	
	for(i=0;i<uLen_g77;i++)
		printf("%02X ",g77[i]);
#endif
	uAddress = EEPROMADDRESS;
	uTmp[0]=(UBYTE)uLen_g77;
	ee_write(uAddress,1,uTmp);
	uAddress = uAddress +1;
	ee_write(uAddress,uLen_g77,g77);
	uAddress = uAddress + 256;

#ifdef _DEBUG_LB_WR_
	printf("\n\r l2buff_wr ugRand(9f37)=%d:",4);	
	for(i=0;i<4;i++)
		printf("%02X ",ugRand[i]);
#endif
	ee_write(uAddress,4,ugRand);
	uAddress = uAddress + 4;

#ifdef _DEBUG_LB_WR_
	printf("\n\r l2buff_wr uTransNum=%d:",3);	
	for(i=0;i<3;i++)
		printf("%02X ",uTransNum[i]);
#endif
	ee_write(uAddress,3,uTransNum);
	uAddress = uAddress + 3;

#ifdef _DEBUG_LB_WR_
	printf("\n\r l2buff_wr gAPPAount_SN=%d:",10);	
	for(i=0;i<10;i++)
		printf("%02X ",gAPPAount_SN[i]);
#endif
	ee_write(uAddress,10,gAPPAount_SN);
	uAddress = uAddress + 10;


#ifdef _DEBUG_LB_WR_
	printf("\n\r l2buff_wr gATC=%d:",2);	
	for(i=0;i<2;i++)
		printf("%02X ",gATC[i]);
#endif
	ee_write(uAddress,2,gATC);
	uAddress = uAddress + 2;

	
	return 0;
}
//#define _DEBUG_LB_RD_
int  L2Buffer_RD(UBYTE* uTransNum)
{
	int ret,i;
	UWORD uAddress;
	UBYTE uTmp[2];
	//uLen_g90 ,g90
	uAddress = EEPROMADDRESS;
	ret = ee_read(uAddress,1,uTmp);
	if(ret)
		return ret;

	uLen_g77=uTmp[0];
	uAddress = uAddress +1;
	ret = ee_read(uAddress,uLen_g77,g77);
	if(ret)
		return ret;
#ifdef _DEBUG_LB_RD_
	printf("\n\r l2buff_rd uLen_g77=%d:",uLen_g77);	
	for(i=0;i<uLen_g77;i++)
		printf("%02X ",g77[i]);
#endif
	uAddress = uAddress +256;
	ret = ee_read(uAddress,4,ugRand);
	if(ret)
		return ret;

#ifdef _DEBUG_LB_RD_
	printf("\n\r l2buff_rd ugRand=%d:",4);	
	for(i=0;i<4;i++)
		printf("%02X ",ugRand[i]);
#endif

	uAddress = uAddress +4;
	ret = ee_read(uAddress,3,uTransNum);
	if(ret)
		return ret;
#ifdef _DEBUG_LB_RD_
	printf("\n\r l2buff_rd uTransNum=%d:",3);	

	for(i=0;i<3;i++)
		printf("%02X ",uTransNum[i]);
#endif
	uAddress = uAddress +3;
	ret = ee_read(uAddress,10,gAPPAount_SN);
	if(ret)
		return ret;
#ifdef _DEBUG_LB_RD_
	printf("\n\r l2buff_rd gAPPAount_SN=%d:",10);	
	for(i=0;i<10;i++)
		printf("%02X ",gAPPAount_SN[i]);
#endif
	uAddress = uAddress +10;
	ret = ee_read(uAddress,2,gATC);
	if(ret)
		return ret;

#ifdef _DEBUG_LB_RD_
	printf("\n\r l2buff_rd gATC=%d:",2);	
	for(i=0;i<2;i++)
		printf("%02X ",gATC[i]);
#endif
	return 0;
}

int tlv_gpo_unpack(UBYTE*  buf,int nLen)
{
	int  i, j;
	int  count;
	pTlvData tlv = NULL;
	
	//count = unpack_tlv_data(&tlv, buf, sizeof(buf)/sizeof(buf[0]));
	count = unpack_tlv_data(&tlv, buf, nLen);
	if (count < 0)
	{
		printf("count failed, error code:%d\n", count);
	}
/*
	printf("//Tlv Unpack Test\n");
	for (i=0; i<count; ++i)
	{
		printf("Tag:");
		for (j=0; j<tlv[i].tag.size; ++j)
		{
			printf("%02X", tlv[i].tag.data[j]);
			if((j+1)%16==0)
				printf("\n");
	
		}
		printf("\n");

		printf("Len:%d\n", tlv[i].val.size);
		printf("Val:");
		for (j=0; j<tlv[i].val.size; ++j)
		{
			printf("%02X ", tlv[i].val.data[j]);
			if((j+1)%16==0)
				printf("\n");
	
		}
		printf("\n");
	}
*/		
	nLen_AFL=0;
	memset(AFL,0x00,sizeof(AFL));
	uLen_g82 = 0;
	memset(g82,0x00,sizeof(g82));
	//uLen_g57_gpo=0;
	//memset(g57,0x00,sizeof(g57));
	
	for (i=0; i<count; ++i)
	{
		
		if(memcmp(tlv[i].tag.data,"\x9F\x36",tlv[i].tag.size)==0)
		{
			
			memcpy(gATC,tlv[i].val.data,2);
			uLen_gATC = tlv[i].val.size;
		}
		
		if(memcmp(tlv[i].tag.data,"\x94",tlv[i].tag.size)==0)
		{
			nLen_AFL =tlv[i].val.size;
			memcpy(AFL,tlv[i].val.data,tlv[i].val.size); 
		}

		if(memcmp(tlv[i].tag.data,"\x82",tlv[i].tag.size)==0)
		{
			uLen_g82 =tlv[i].val.size;
			memcpy(g82,tlv[i].val.data,tlv[i].val.size); 
		}
		if(memcmp(tlv[i].tag.data,"\x57",tlv[i].tag.size)==0)
		{
			uLen_g57_gpo =tlv[i].val.size;
			memcpy(g57,tlv[i].val.data,tlv[i].val.size); 
		}

		
		if(memcmp(tlv[i].tag.data,"\x9F\x26",tlv[i].tag.size)==0)
		{
			uLen_g9f26=tlv[i].val.size; 
			memcpy(g9f26,tlv[i].val.data,tlv[i].val.size);
		}

		if(memcmp(tlv[i].tag.data,"\x9F\x4b",tlv[i].tag.size)==0)
		{
			uLen_g9f4b=tlv[i].val.size; 
			memcpy(g9f4b,tlv[i].val.data,tlv[i].val.size);
		}
		

		if(memcmp(tlv[i].tag.data,"\x9F\x10",tlv[i].tag.size)==0)
		{
			uLen_g9f10=tlv[i].val.size; 
			memcpy(g9f10,tlv[i].val.data,tlv[i].val.size);
			memset(gCVR,0xff,4);
			if(uLen_g9f10>0)
			{
				memcpy(gCVR,g9f10+3,4);
		
			}
		}


		if(memcmp(tlv[i].tag.data,"\x9F\x5d",tlv[i].tag.size)==0)
		{
			uLen_g9f5d=tlv[i].val.size; 
			memcpy(g9f5d,tlv[i].val.data,tlv[i].val.size);
		}
		
		if(memcmp(tlv[i].tag.data,"\x9F\x6c",tlv[i].tag.size)==0)
		{
			uLen_g9f6c=tlv[i].val.size; 
			memcpy(g9f6c,tlv[i].val.data,tlv[i].val.size);
		}
		
	}
	
	if(uLen_gATC ==0)
		return ERR_NO9f36;
	if(uLen_g82 ==0)
		return ERR_NO82;
	if(uLen_g9f26 ==0)
		return ERR_NO9f26;
	if(uLen_g9f10 <7)
		return ERR_NO9f10;


	if((gCVR[1]&0x30)==0x00) 
		return ERR_CVR;

	if((gCVR[1]&0x30)==0x20)  //arqc,��������
		return ERR_CVR;
	if(nLen_AFL==0)
		return ERR_NO94;

	free(tlv);
	tlv = NULL;
	
	return 0;
}

int L2Process(int nTrade,char *inbuf,char *outbuf)
{
	unsigned int nRetCode=0,i;
	UBYTE uTmp[256],uATC[2];
	int nLen_AID,nLen_Tmp;	
	int j;
	UBYTE l_tlv, pfbuf[100];
	
	UBYTE uLen_Tag57=0,uLen_g9f5d=0;
	struct  timeval start_time;
	
	//#ifdef DEBUG_TIME_CPU
	//	ReaderResponse(csc_comm, 0x00, 0xF0, "\x01\x00\x40", 3);
	//#endif	
	pth801A();

	memset(ugRand,0x00,sizeof(ugRand));
	gettimeofday(&start_time,NULL);

	memset(uTmp,0x00,sizeof(uTmp));
	//nRetCode = rtc_rd_time(uTmp);
	//if(nRetCode !=  OK)
	//{
	//	fprintf(stderr,"ȡ������ʱ��ʧ��:uRetCode=%04X",nRetCode);
	//	return (nRetCode) ;
	//}
#ifdef _DEBUG_L2T_
	fprintf(stderr,"������ʱ��:");
	for(j=0;j<6;j++)
		fprintf(stderr,"%02X ",uTmp[j]);
	fprintf(stderr,"\n\r");
#endif

		sem_init(&g_801Await, 0, 0);
		sem_init(&g_801Areturn, 0, 0);
		//nRetCode = pthread_create(&g_pth801AID, NULL, &pth801A, NULL);
		//pth801A();

	//if(nTrade != 10041 && nTrade != 10042)
	//	return -10040; 
	GenRand(start_time);
	GenPDOL(uTmp,&nLen_Tmp);
	memcpy ( C_Apdu.data , "\x80\xA8\x00\x00",4);
	C_Apdu.length = 4 ;

	if(nLen_Tmp>=0x80)
	{
		C_Apdu.data[4]=1+2+nLen_Tmp;
	}
	else
	{
		C_Apdu.data[4]=1+1+nLen_Tmp;
	}
	C_Apdu.length ++; 
	
	C_Apdu.length++;
	C_Apdu.data[5] = 0x83;
	if(nLen_Tmp>=0x80)
	{
		C_Apdu.data[6] =0x81;
		C_Apdu.data[7] =(UBYTE) nLen_Tmp;
		C_Apdu.length = C_Apdu.length + 2;

	}
	else
	{
		C_Apdu.data[6] =(UBYTE) nLen_Tmp;
		C_Apdu.length ++ ;
	}
	memcpy ( C_Apdu.data +C_Apdu.length, uTmp,nLen_Tmp);
	C_Apdu.length = C_Apdu.length +nLen_Tmp ;
#ifdef DEBUG_TIME_CPU
	memcpy(pfbuf, "\x08\x00\x50\x00", 4);
	ReaderResponse(csc_comm, 0x00, 0xF0, pfbuf, 4);
#endif

	nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
#ifdef DEBUG_TIME_CPU
	memcpy(pfbuf, "\x08\x00\x51\x00", 4);
	ReaderResponse(csc_comm, 0x00, 0xF0, pfbuf, 4);
#endif
	//GPOģ��
	if(nRetCode == OK && R_Apdu.data[0]!=0x77)
	{
		return ERR_GPOMODE ;
	}	
	//��¼����
	//20120419
	if(nRetCode == OK && R_Apdu.data[1]==0x81)
	{
		if(R_Apdu.data[2]!=R_Apdu.length-5)
		{
			return ERR_GPOLEN ;
		}
	}
	else
	{	
		if(nRetCode == OK && R_Apdu.data[1]!=R_Apdu.length-4)
		{
			return ERR_GPOLEN ;
		}
	}			
	if(nRetCode != OK)
		return nRetCode;
		
	uLen_g77 =R_Apdu.length;
	memcpy(g77, R_Apdu.data,uLen_g77);
	//20130902
	if(g77[1]==0x81)
		nRetCode = tlv_gpo_unpack(g77+3,uLen_g77-3);	
	else
		nRetCode = tlv_gpo_unpack(g77+2,uLen_g77-2);	

	if (nRetCode!=OK)
	{
		fprintf(stderr,"\r\ntlv_gpo_unpack =%d \r\n",nRetCode);
		return (nRetCode) ;
	}
//	if(nTrade==10041)
//		L2Buffer_WR(gR_Entry->TradeNum);
//
//	if(nTrade==10042)
//		L2Buffer_WR(gR_Exit->TradeNum);
//
//	if(nTrade==10043)
//		L2Buffer_WR(gR_Update->TradeNum);

#ifdef DEBUG_TIME_CPU
	memcpy(pfbuf, "\x08\x00\x52\x00", 4);
	ReaderResponse(csc_comm, 0x00, 0xF0, pfbuf, 4);
#endif
	//����
	nRetCode = UpdateCAppCache(nTrade,inbuf);
	if (nRetCode!=OK)
	{
		fprintf(stderr,"UpdateCAppCache:uRetCode=%d",nRetCode);
		return (nRetCode) ;
	}
#ifdef DEBUG_TIME_CPU
	memcpy(pfbuf, "\x08\x00\x54\x00", 4);
	ReaderResponse(csc_comm, 0x00, 0xF0, pfbuf, 4);
#endif
	//����¼
	nRetCode = ReadRecords();
	if (nRetCode!=OK)
	{
		fprintf(stderr,"ReadRecords:uRetCode=%d",nRetCode);
		return (nRetCode) ;
	}
#ifdef DEBUG_TIME_CPU
	memcpy(pfbuf, "\x08\x00\x56\x00", 4);
	ReaderResponse(csc_comm, 0x00, 0xF0, pfbuf, 4);
#endif


#ifdef _DEBUGEXETIME_
	gettimeofday(&start,NULL);

	nRetCode = fdda_L2(g9f06);

	gettimeofday(&end,NULL);
	timer = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
	fprintf(stderr,"fdda timer = %ld us\n",timer);
#else
	nRetCode = fdda_L2(g9f06);
#endif	
	if (nRetCode!=OK)
	{
		if((g9f6c[0] & 0x20) ==0x20)
		{
		}
		else
		{
			switch(nRetCode)
			{
				case ERR_NOCAIDX:;
					break;
				default:
					break;		
			}
		}
		
#ifdef DEBUG_TIME_CPU
	memcpy(pfbuf, "\x08\x00\x56\x01", 4);
	ReaderResponse(csc_comm, 0x00, 0xF0, pfbuf, 4);
#endif
		return (nRetCode) ;
	}
	

	return OK;
}


#define _DEBUG_MB_WR_
int  MECBuffer_WR(UBYTE* uBuffer,int nLen)
{
	int i;
	UWORD uAddress;
	
#ifdef _DEBUG_MB_WR_
	printf("\n\r  MECBuffer_WR nLen=%d:\r\n",nLen);	
	for(i=0;i<nLen;i++)
	{
			
		printf("%02X ",uBuffer[i]);
		if((i+1)%16==0)
			printf("\r\n");	
	}
#endif
	uAddress = EEPROMADDRESS+EEPMECOFFSET;
	ee_write(uAddress,nLen,uBuffer);
	uAddress = uAddress + nLen;
	return 0;
}
#define _DEBUG_MB_RD_
int  MECBuffer_RD(UBYTE* uBuffer,int nLen)
{
	int ret,i;
	UWORD uAddress;
	
	uAddress = EEPROMADDRESS+EEPMECOFFSET;
	ret = ee_read(uAddress,nLen,uBuffer);
	if(ret)
		return ret;

#ifdef _DEBUG_MB_RD_
	printf("\n\r mecbuff_rd nLen=%d:\r\n",nLen);	
	for(i=0;i<nLen;i++)
	{
		
		printf("%02X ",uBuffer[i]);
		if((i+1)%16==0)
			printf("\r\n");
	}
#endif
	
	return 0;
}

int MECProcess(int nTrade,char *inbuf,char *outbuf)
{
	unsigned int nRetCode=0,i;
	UBYTE uTmp[256];
	//UBYTE AFL[256],uRecno_Begin,uRecno_End,uRecno;
	int nLen_Tmp;	
	int j;

	UBYTE buf[128],MAC1[4],MAC2[4],TAC[4],uPSAMCounter[4],Diversity[16],Key[16],pszInitData[64],pszBuf[128],uPriBuf[4];

	UBYTE NDX_MEC=0x01;
	UBYTE uBalance[4],uAtc[2],uOverdrawlimited[3],uKeyVer,uAlgoID,uRand[4];
	int nTotal=0,nYJE=0,nAmount=0;
	

#ifdef _DEBUG_MECP_	
	fprintf(stderr,"\x0d\x0a inbuf :");
	for(i=0;i<16;i++)
	{
		fprintf(stderr,"%02X ",inbuf[i]);
	}
	fprintf(stderr,"\x0d\x0a ");
#endif

	//80 50
	memset(C_Apdu.data,0x00,sizeof(C_Apdu.data));
	memcpy ( C_Apdu.data , "\x80\x50\x03\x02\x0b",5);
	C_Apdu.data[5] =NDX_MEC;//��Կ������
	
	memcpy ( C_Apdu.data +6 , "\x00\x00\x00\x00",4);
	if(nTrade==10042)
	{
		//C_Apdu.data[6]=0x00;
		memcpy(C_Apdu.data+7,gR_Exit->Amount,3);
	}
	if(nTrade==10043)
	{
		//C_Apdu.data[6]=0x00;
		memcpy(C_Apdu.data+7,gR_Update->Amount,3);
	}
	if(nTrade==10044 && gR_Lock->BlackLevel>0)
	{
		//��Ǯ�������
		memcpy(C_Apdu.data+6,gBalance,4);
	}


	memcpy ( C_Apdu.data +10 , gSAMNo,6);
	//memcpy ( C_Apdu.data +10 , "\x20\x00\x12\x01\x32\x01",6);
	C_Apdu.data[16] = 0x0f;//lc 					
	C_Apdu.length = 17 ;

	nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
	if(nRetCode != OK)
		return nRetCode;
	memcpy(uBalance,R_Apdu.data,4);
	memcpy(uAtc,R_Apdu.data+4,2); 
	memcpy(gATC,uAtc,2); 
	
	memcpy(uOverdrawlimited,R_Apdu.data+4+2,3); 
	uKeyVer = R_Apdu.data[9];
	uAlgoID = R_Apdu.data[10];
	memcpy(uRand,R_Apdu.data+11,4);
	//MAC1
	memset(uTmp,0x00,sizeof(uTmp));
	memcpy(uTmp,uRand,4);
	memcpy(uTmp+4,uAtc,2);
	if(nTrade==10041)
		memcpy(uTmp+6,"\x00\x00\x00\x00",4);//���׽��
	if(nTrade==10042)
	{
		uTmp[6]=0x00;
		memcpy(uTmp+7,gR_Exit->Amount,3);
	}
	if(nTrade==10043)
	{
		uTmp[6]=0x00;
		memcpy(uTmp+7,gR_Update->Amount,3);
	}
	if(nTrade==10044 && gR_Lock->BlackLevel>0)
	{
		//��Ǯ�������
		//uTmp[6]=0x00;
		memcpy(uTmp+6,gBalance,4);
	}


	uTmp[10] = 0x09; //0x09

	memcpy(uTmp+11,inbuf,7);//��������ʱ��
	uTmp[18]=uKeyVer;
	uTmp[19]=uAlgoID;
	memcpy(uTmp+20,gAppSN,8) ;//Ӧ�����кţ���ƬADF��0015�ļ��е�Ӧ�����к�
	memset(MAC1,0x00,sizeof(MAC1));
	memset(uPSAMCounter,0x00,sizeof(uPSAMCounter));

	nRetCode =PSAM_MAC1(PSAM_NDX,uTmp,uPSAMCounter,MAC1);
	if(nRetCode != OK)
		return nRetCode;


	//80 DC
	
	memset(C_Apdu.data,0x00,sizeof(C_Apdu.data));
	memcpy ( C_Apdu.data , "\x80\xdc\x09\xc8\x40",5);
	C_Apdu.data[2] = gBuf19[0];//�������ѱ�־
	//memcpy ( C_Apdu.data +5 , "\x09\x3e\x00",3);	

	memset(uTmp,0x00,sizeof(uTmp));	
	memcpy ( uTmp , gBuf19,sizeof(gBuf19));
	//���ݽ������ͼ�gBuf19��д19�ļ�����
	g_DataMEC19 = (F_DataMEC19*)uTmp;
	if(nTrade==10041)
	{
		if(memcmp(g_DataMEC19->EntryTime,inbuf,3)!=0)
		{
			memcpy(g_DataMEC19-> MonthAmount,"\x00\x00\x00",3);	//���¹����ͨ�ۼƽ��
			memcpy(g_DataMEC19->MonthCount,"\x00\x00",2);	//���¹����ͨ�ۼƴ���
		}


		//������վ :Bit3�� 0��ʾ������վ      1��ʾ��վ���˽�վ
		//if((inbuf[10] & 0x08)==0x00)
		if((gR_Entry->Flag & 0x08)==0x00)		
		{
			memset(g_DataMEC19->Amount,0x00,sizeof(g_DataMEC19->Amount));//20130722
			g_DataMEC19->Status = 0x00;
      		memcpy(g_DataMEC19->EntryTime,inbuf,7);
			memcpy(g_DataMEC19->EntryTermNo,gTerminalNo,4);
		}
		else
		{
			g_DataMEC19->Status =( g_DataMEC19->Status & 0x3e);
			g_DataMEC19->Status = g_DataMEC19->Status | 0x10;
			
		}	
		//��ʱ 20130722
		//if((inbuf[10] & 0x10)==0x10)
		if((gR_Entry->Flag & 0x10)==0x10)
		{
			g_DataMEC19->Status = g_DataMEC19->Status | 0x20;
		}		
		g_DataMEC19->Status = g_DataMEC19->Status | 0x40;
		if(GetOddParityBit(g_DataMEC19->Status )==0)
			g_DataMEC19->Status  = (g_DataMEC19->Status  | 0x01);


		memcpy(g_DataMEC19->LastTime,inbuf,7);	//ĩ��ʱ��
		memcpy(g_DataMEC19->LastTermNo,gTerminalNo,4);//ĩ���ն�
	}
	//��վ
	if(nTrade==10042)
	{
		
		//����������Ϊ0��1��9��10�򡰳�վ����ʱ�䡱������վ�����ͨ�ն˱�š�����ĩ�ʽ���ʱ�䡱����ĩ�ʹ����ͨ�ն˱�š�������д��
		//if(gR_Exit->TradeType==0 || gR_Exit->TradeType==1 || gR_Exit->TradeType==9 || gR_Exit->TradeType==10)
		//{
			memcpy(g_DataMEC19->ExitTime,inbuf,7);	//��վʱ��
			memcpy(g_DataMEC19->ExitTermNo,gTerminalNo,4);//��վ�ն�

			memcpy(g_DataMEC19->LastTime,inbuf,7);	//ĩ��ʱ��
			memcpy(g_DataMEC19->LastTermNo,gTerminalNo,4);//ĩ���ն�
		//}
		//20130722 
		if(gR_Exit->TradeType==6 || gR_Exit->TradeType==7 || gR_Exit->TradeType==8 || gR_Exit->TradeType==9)
		{
			g_DataMEC19->Status = (g_DataMEC19->Status | 0x08);		
		}

		g_DataMEC19->Status =( g_DataMEC19->Status & 0x3e);
		g_DataMEC19->Status =( g_DataMEC19->Status | 0x80);
		g_DataMEC19->Status = (g_DataMEC19->Status & 0xEF) ; //���վ���˽�վflag,20130724
		if(GetOddParityBit(g_DataMEC19->Status )==0)
			g_DataMEC19->Status  = (g_DataMEC19->Status  | 0x01);


		//ʵ�ʽ��׽��
		nYJE=0;
		nAmount = 0;
		gBytes2Short3(gR_Exit->Amount,&nAmount);
		gBytes2Short(g_DataMEC19->Amount,&nYJE);
		nAmount = nAmount +nYJE;
		gShort2Bytes(nAmount,g_DataMEC19->Amount);;//ʵ�ʽ��׽��=ʵ�ʽ��׽��+���׽��
		
	
		//���ۼƽ��
		gBytes2Short3(g_DataMEC19->MonthAmount,&nYJE);
		gBytes2Short3(gR_Exit->Amount,&nAmount);
		nTotal = nYJE + nAmount;
	
		gShort32Bytes(nTotal,g_DataMEC19->MonthAmount);


		//���ۼƴ���
		nTotal=0;
		gBytes2Short(g_DataMEC19->MonthCount,&nTotal);
		nTotal = nTotal +1;
		gShort2Bytes(nTotal,g_DataMEC19->MonthCount);
	}

	//����
	if(nTrade==10043)
	{
		if(memcmp(g_DataMEC19->EntryTime,gR_Update->TradeTime,3)!=0)
		{
			memcpy(g_DataMEC19->MonthAmount,"\x00\x00\x00",3);	//���¹����ͨ�ۼƽ��
			memcpy(g_DataMEC19->MonthCount,"\x00\x00",2);	//���¹����ͨ�ۼƴ���
		}


		g_DataMEC19->Status = gR_Update->Status;
		if(GetOddParityBit(g_DataMEC19->Status)==0)
			g_DataMEC19->Status = (g_DataMEC19->Status | 0x01);

		memcpy(g_DataMEC19->EntryTermNo,gR_Update->EntryTermNo,4);
		memcpy(g_DataMEC19->ExitTermNo,gR_Update->ExitTermNo,4);
				
		memcpy(g_DataMEC19->LastTime,gR_Update->TradeTime,7);
		memcpy(g_DataMEC19->LastTermNo,gTerminalNo,4);//ĩ���ն�
		//Bit2:0��ʾ����д��ʵ�ʽ��׽�� 1��ʾ��ʵ�ʽ��׽������
		if((gR_Update->TimeFlag & 0x04)==0x04)
		{
			memset(g_DataMEC19->Amount,0x00,2);
		} 	
		//Bit1:0��ʾ����д��վ��������/ʱ���� 1��ʾ�ò����еĽ�������/ʱ���д��վ��������/ʱ����
		if((gR_Update->TimeFlag & 0x02)==0x02)
		{
			memcpy(g_DataMEC19->ExitTime,gR_Update->TradeTime,7);
		} 	

		if((gR_Update->TimeFlag & 0x01)==0x01)
		{
			memcpy(g_DataMEC19->EntryTime,gR_Update->TradeTime,7);
		} 

	}
	if(nTrade==10044 )
	{
		g_DataMEC19->AppLockFlag=0x01;//suo������־
	}

	memcpy(C_Apdu.data+5,uTmp,64);
	C_Apdu.length = 69 ;

	nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
	if(nRetCode != OK)
		return nRetCode;

	//���潻������,20130725
	memset(uTmp,0,sizeof(uTmp));
	gS_MECBuffer=(S_MECBuffer*)uTmp;
	gS_MECBuffer->KeyIndex = NDX_MEC;
	gS_MECBuffer->KeyVer=uKeyVer;
	memcpy(gS_MECBuffer->OffTradeCount,uAtc,2);
	memcpy(gS_MECBuffer->TermTradeCount,uPSAMCounter,4);
	if(nTrade==10041)
		memcpy(gS_MECBuffer->TransNum,gR_Entry->TradeNum,3);

	if(nTrade==10042)
	{
		memcpy(gS_MECBuffer->Amount,gR_Exit->Amount,3);
		memcpy(gS_MECBuffer->TransNum,gR_Exit->TradeNum,3);

	}
	if(nTrade==10043)
	{
		memcpy(gS_MECBuffer->Amount,gR_Update->Amount,3);
		memcpy(gS_MECBuffer->TransNum,gR_Update->TradeNum,3);
	}
	if(nTrade==10044)
	{
		if(gR_Lock->BlackLevel>0)
			memcpy(gS_MECBuffer->Amount,gBalance+1,3);
		memcpy(gS_MECBuffer->TransNum,gR_Lock->TradeNum,3);
	}

	//printf("\r\n gBalance=%02x %02x %02x %02x \r\n",gBalance[0],gBalance[1],gBalance[2],gBalance[3]);
	memcpy(gS_MECBuffer->uBalance,gBalance,4);
	memcpy(gS_MECBuffer->AppSn,gAppSN,10);
	MECBuffer_WR(uTmp,sizeof(S_MECBuffer));

	//80 54	
	memset(C_Apdu.data,0x00,sizeof(C_Apdu.data));
	memcpy ( C_Apdu.data , "\x80\x54\x01\x00\x0f",5);
	memcpy ( C_Apdu.data +5 , uPSAMCounter,4);//�ն˽�����ˮ��
	memcpy ( C_Apdu.data +9 , inbuf,7);//��������ʱ��;
	memcpy ( C_Apdu.data +16 , MAC1,4);//
	C_Apdu.data[20] = 0x08;//lc 					
	C_Apdu.length = 21 ;

	nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
	if(nRetCode != OK)
		return nRetCode;

	//MAC2
	PSAM_MAC2_VERIFY(PSAM_NDX,R_Apdu.data+4);
	//tac
	memcpy(TAC,R_Apdu.data,4);
	outbuf[0]=0x00;
	if(nTrade==10041)//entry
	{
		gD_MECEntry=(D_MECEntry*)(outbuf+1);
		gD_MECEntry->KeyIndex= NDX_MEC;
		gD_MECEntry->KeyVer=uKeyVer;
		memcpy(gD_MECEntry->OffTradeCount,uAtc,2);
		memcpy(gD_MECEntry->TermTradeCount,uPSAMCounter,4);
		memcpy(gD_MECEntry->TAC,TAC,4);
		memset(gD_MECEntry->RFU,0x00,2);
	}
	
	
	if(nTrade==10042)//exit
	{
		gD_MECExit=(D_MECExit*)(outbuf+1);
		gD_MECExit->KeyIndex= NDX_MEC;
		gD_MECExit->KeyVer=uKeyVer;
		memcpy(gD_MECExit->OffTradeCount,uAtc,2);
		memcpy(gD_MECExit->TermTradeCount,uPSAMCounter,4);
		memcpy(gD_MECExit->TAC,TAC,4);

		nYJE=0;
		nAmount = 0;
		gBytes2Int(gBalance,&nYJE);
		gBytes2Short3(gR_Exit->Amount,&nAmount);
		nYJE = nYJE - nAmount;
		gShort32Bytes(nYJE,gD_MECExit->BALANCE);

	}
	
	if(nTrade==10043)//update
	{
		gD_MECExit=(D_MECExit*)(outbuf+1);
		gD_MECExit->KeyIndex= NDX_MEC;
		gD_MECExit->KeyVer=uKeyVer;
		memcpy(gD_MECExit->OffTradeCount,uAtc,2);
		memcpy(gD_MECExit->TermTradeCount,uPSAMCounter,4);
		memcpy(gD_MECExit->TAC,TAC,4);

		nYJE=0;
		nAmount = 0;
		gBytes2Int(gBalance,&nYJE);
		gBytes2Short3(gR_Update->Amount,&nAmount);
		nYJE = nYJE - nAmount;
		gShort32Bytes(nYJE,gD_MECExit->BALANCE);

	}
		
	if(nTrade==10044)//tac
	{
		gD_MECExit=(D_MECExit*)(outbuf+2);
		outbuf[1]=0x03;//3��ʾ�����������ɹ���
		gD_MECExit->KeyVer=uKeyVer;
		memcpy(gD_MECExit->OffTradeCount,uAtc,2);
		memcpy(gD_MECExit->TermTradeCount,uPSAMCounter,4);
		memcpy(gD_MECExit->TAC,TAC,4);
		
		if(gR_Lock->BlackLevel>0)
			memset(gD_MECExit->BALANCE,0x00,3);
		else
			memcpy(gD_MECExit->BALANCE,gBalance+1,3);
		
	}
	return 0;

}
int pf_mobile_ticket_device_init(char *inbuf)
{	
	int i;
#ifdef _PARAMCACERT_
	printf("\x0d\x0a 20130725-3 vPARA \r\n");	
#else	
	printf("\x0d\x0a 20130725 vDefault \r\n");
#endif
	memcpy(gTerminalNo,inbuf,4);	
	memcpy(gSAMNo,inbuf+4,6);
#ifdef _DEBUG_PARA_INIT_
	fprintf(stderr,"\x0d\x0a gTerminalNo :");
	for(i=0;i<4;i++)
	{
		fprintf(stderr,"%02X ",gTerminalNo[i]);
	}
	fprintf(stderr,"\x0d\x0a ");

	fprintf(stderr,"\x0d\x0a gSAMNo :");
	for(i=0;i<6;i++)
	{
		fprintf(stderr,"%02X ",gSAMNo[i]);
	}
	fprintf(stderr,"\x0d\x0a ");
#endif
	return 0;
}

int pf_mobile_ticket_para_init(char *inbuf)
{
	
#ifdef _DEBUG_PARA_INIT_
	int i;
	printf("in para init \n\r");
#endif
	g_TP_Head=(S_TerminalParam_Head* )inbuf;
	//g_TP_Record = (S_TerminalParam_Record*)(inbuf+sizeof(S_TerminalParam_Head));
	g_TP_Record = (S_TerminalParam_Record*)(inbuf+21+20);
#ifdef _DEBUG_PARA_INIT_
	printf("\n\r sizeof(S_TerminalParam_Head)=%d,sizeof(S_TerminalParam_Record)=%d,para:\n\r",
			sizeof(S_TerminalParam_Head),sizeof(S_TerminalParam_Record));
	//for(i=0;i<sizeof(S_TerminalParam_Record)*g_TP_Head->RecordNum+41;i++)
	for(i=0;i<(g_TP_Head->RecordNum[0] * 296)+41;i++)
	{
		printf("%02X ",inbuf[i]);
		if((i+1) % 16 ==0)
			printf("\n\r");
	}
	printf("\n\r");
#endif
	return 0;
}

int SaveTransData(char* TransData)
{
	
	FILE *stream;
	stru_TransData_Head* pHead;

	UBYTE uBuf[sizeof(stru_TransData_Head)];
#ifdef _DEBUG_TRANSDATA_
	int i;

	fprintf(stderr,"\n\r in save  TransData file(%d)BYTES:\n\r",sizeof(stru_TransData_Head)+sizeof(D_Entry));
	for(i=0;i<3;i++)
	{
		
		fprintf(stderr,"%02X ",gR_Entry->TradeNum[i]);
		if((i+1)%16==0)
			fprintf(stderr,"\n\r");
	}
	fprintf(stderr,"\n\r");	
	for(i=0;i<10;i++)
	{
		
		fprintf(stderr,"%02X ",gAPPAount_SN[i]);
		if((i+1)%16==0)
			fprintf(stderr,"\n\r");
	}
	fprintf(stderr,"\n\r");
	for(i=0;i<2;i++)
	{
		
		fprintf(stderr,"%02X ",gATC[i]);
		if((i+1)%16==0)
			fprintf(stderr,"\n\r");
	}
	fprintf(stderr,"\n\r");

	for(i=0;i<sizeof(D_Entry);i++)
	{
		
		fprintf(stderr,"%02X ",TransData[i]);
		if((i+1)%16==0)
			fprintf(stderr,"\n\r");
	}
	fprintf(stderr,"\n\r");
#endif
	
	if ((stream = fopen("TransDD.$$$", "wb")) == NULL) /* open file TEST.$$$ */
	{
		fprintf(stderr, "Cannot open output file.\n");
		return -1;
	}
	fseek(stream, 0, SEEK_SET); 
	memset(uBuf,0x00,sizeof(uBuf));
	pHead = (stru_TransData_Head*)uBuf;
	memcpy(pHead->TransNum, gR_Entry->TradeNum,sizeof(pHead->TransNum));
	memcpy(pHead->AppSn, gAPPAount_SN,sizeof(pHead->AppSn));
	memcpy(pHead->ATC, gATC,sizeof(pHead->ATC));

	fwrite(uBuf,sizeof(stru_TransData_Head), 1, stream); 
	fwrite(TransData,sizeof(D_Entry), 1, stream);
	fclose(stream); /*�ر��ļ�*/

	return 0;
}
//6849
int GetTransData(char* TransData)
{
	int i;	
	FILE *stream;

	if ((stream = fopen("TransDD.$$$", "rb")) == NULL) /* open file TEST.$$$ */
	{
		fprintf(stderr, "Cannot open input file.\n");
		return -1;
	}

	fseek(stream, 0, SEEK_SET);     
	fread(TransData, 1,sizeof(stru_TransData_Head)+sizeof(D_Entry),stream);	
#ifdef _DEBUG_TRANSDATA_
	fprintf(stderr,"\n\rstruc  TransData file(%d,%d)BYTES:\n\r",sizeof(stru_TransData_Head),sizeof(D_Entry));
	for(i=0;i<sizeof(stru_TransData_Head)+sizeof(D_Entry);i++)
	{
		
		fprintf(stderr,"%02X ",TransData[i]);
		if((i+1)%16==0)
			fprintf(stderr,"\n\r");
	}
	fprintf(stderr,"\n\r");
#endif
	fclose(stream);
	return 0;

}
//#define _DEBUG_ENTRY_
int pf_mobile_normal_ticket_entry(char *inbuf,char *outbuf)
{
	int ret,i;
#ifdef _DEBUG_ENTRY_	
	printf("\n\r  pf_mobile_normal_ticket_entry inbuf:");	
	for(i=0;i<11;i++)
		printf("%02X ",inbuf[i]);
#endif
	if(gCardBusinessType==0x00)
	{
		memset(g9f02,0x00,sizeof(g9f02));
		memcpy(g9a,inbuf+1,3);	//ȥ��0x20
		memcpy(g9f21,inbuf+4,3); //trade time
		gR_Entry=(R_Entry* )inbuf;
		memcpy(g9f02_BAK,g9f02,6);
		
		ret=L2Process(10041,inbuf,outbuf);
		if(ret==0)
			GenOutBuf(1,outbuf);	
		else
			GenOutBuf(0,outbuf);

	 
		//SaveTransData(outbuf+1);
		if(ret>MAXNOCARDRETCODE)
			ret = ret * -1;
	#ifdef _DEBUG_ENTRY_
		printf("\r\n L2Process return = %d \r\n",ret);
	#endif
		return ret;
	}
	else
	{
		return ERR_CALL_BUSITYPE;
	}
	
}
//#define _DEBUG_NORMALEXIT_
int pf_mobile_normal_ticket_exit(char *inbuf,char *outbuf)
{
	int i,ret,nAmount=0;
	unsigned char uTmp[16];
	#ifdef _DEBUG_NORMALEXIT_
		printf("\n\r pf_mobile_normal_ticket_exit inbuf:");	
		for(i=0;i<16;i++)
			printf("%02X ",inbuf[i]);
	#endif

	memset(g9f02,0x00,sizeof(g9f02));
	memset(uTmp,0x00,sizeof(uTmp));
	memcpy(uTmp+1,inbuf+10,3);
#ifdef _DEBUG_NORMALEXIT_
	fprintf(stderr,"\n\r uTmp:");	
	for(i=0;i<4;i++)
		fprintf(stderr,"%02X ",uTmp[i]);
#endif
	gBytes2Int(uTmp,&nAmount);
	sprintf((char*)uTmp,"%012d",nAmount);
	gStr2Bytes(12,uTmp,g9f02);
	
#ifdef _DEBUG_NORMALEXIT_
	fprintf(stderr,"\n\r inbuf:");	
	for(i=0;i<16;i++)
		fprintf(stderr,"%02X ",inbuf[i]);

	fprintf(stderr,"\n\r nAmount = %d,g9f02:",nAmount);
	for(i=0;i<6;i++)
		fprintf(stderr,"%02X ",g9f02[i]);
#endif
	memcpy(g9f02_BAK,g9f02,6);
	memcpy(g9a,inbuf+1,3);
	memcpy(g9f21,inbuf+4,3); //trade time
	gR_Exit=(R_Exit*)inbuf;
	ret=L2Process(10042,inbuf,outbuf);
	if(ret==0)
		GenOutBuf(1,outbuf);	
	else
		GenOutBuf(0,outbuf);
	//SaveTransData(outbuf+1);  ��Ҫ���ݳ�վ�����޸� 20130724
	if(ret>MAXNOCARDRETCODE)
		ret = ret * -1;
#ifdef _DEBUG_NORMALEXIT_
	printf("\r\n L2Process return = %d \r\n",ret);
#endif
	return ret;
}

int GetProofProcess(char *outbuf)
{
	unsigned int nRetCode=0,i;
	UBYTE uTmp[256],uATC[2];
	int nLen_AID,nLen_Tmp;	
	int j;
	UBYTE l_tlv;
	
	UBYTE uLen_Tag57=0,uLen_g9f5d=0;
	
	R_Apdu.length = uLen_g77;
	memcpy(R_Apdu.data,g77, uLen_g77);
	if(R_Apdu.data[0]!=0x77)
	{
		return ERR_GPOMODE ;
	}		
	if(R_Apdu.data[1]==0x81)
	{
		if(R_Apdu.data[2]!=R_Apdu.length-5)
		{
			return ERR_GPOLEN ;
		}
	}
	else
	{	
		if(R_Apdu.data[1]!=R_Apdu.length-4)
		{
			return ERR_GPOLEN ;
		}
	}			
		
	
	
	for(i=0;i<R_Apdu.length;i++)
	{
		if(R_Apdu.data[i]==0x9f && R_Apdu.data[i+1]==0x36 && R_Apdu.data[i+2]==0x02)
		{
			memcpy(uATC,R_Apdu.data+i+3,2); 
			break;
		}
	}
	
	if(i>=R_Apdu.length)
	{
		return ERR_NO9f36;


	}
 	memcpy(gATC,uATC,2);
	nLen_AFL=0;
	i=0;
	
	if(R_Apdu.data[i]==0x81)
		i++;
	i++;
	while(i<R_Apdu.length)
	{
		if(R_Apdu.data[i]==0x94 && R_Apdu.data[i-1]!=0x81)
		{
			if(R_Apdu.data[i+1]==0x81)
				i++;
			memset(AFL,0x00,sizeof(AFL));
			nLen_AFL = R_Apdu.data[i+1];
			if(nLen_AFL==0 )
				break;
			if(nLen_AFL+(i+1)<=R_Apdu.length) 
			{
				memcpy(AFL,R_Apdu.data+i+2,nLen_AFL); 
				break;
			}
			else
			{
				nLen_AFL = 0;
			}
		}
		i++;
	}
					
	for(i=0;i<R_Apdu.length;i++)
	{
		if(R_Apdu.data[i]==0x82)
		{
			l_tlv = R_Apdu.data[i+1];
			uLen_g82 = l_tlv;
			if(l_tlv!=2)
				continue;
					
			memset(g82,0x00,sizeof(g82));
			memcpy(g82,R_Apdu.data+i+2,l_tlv);
						
			break;
		}
	}
	if(i>=R_Apdu.length)
	{
		return ERR_NO82;

	}
	
	
	uLen_g57_gpo=0;
	for(i=0;i<R_Apdu.length;i++)
	{
		if(R_Apdu.data[i]==0x57)
		{
			uTagRepeat_57++;
			l_tlv = R_Apdu.data[i+1];
			if(l_tlv<8)
				continue;
			if(l_tlv>19)
				continue;
			uLen_g57_gpo =l_tlv ;
			memset(g57,0x00,sizeof(g57));
			memcpy(g57,R_Apdu.data+i+2,l_tlv);
							
			break;
		}
	}
	
	if(i>=R_Apdu.length)
	{
		uLen_g57_gpo=0;
	}
	memset(g9f26,0x00,sizeof(g9f26));
	for(i=0;i<R_Apdu.length;i++)
	{
		if(R_Apdu.data[i]==0x9f && R_Apdu.data[i+1]==0x26 && R_Apdu.data[i+2]==0x08)
		{
			memcpy(g9f26,R_Apdu.data+i+3,R_Apdu.data[i+2]); 
			uLen_g9f26=R_Apdu.data[i+2];
			break;
		}
	}
	
	if(i>=R_Apdu.length)
	{
		return ERR_NO9f26;

	}
	for(i=0;i<R_Apdu.length;i++)
	{
		if(R_Apdu.data[i]==0x9f && R_Apdu.data[i+1]==0x4b)
		{
			l_tlv = R_Apdu.data[i+2];
			if(l_tlv ==0x81)
			{
				l_tlv = R_Apdu.data[i+3];
				//if(l_tlv>254)
				//	continue;
				memcpy(g9f4b,R_Apdu.data+i+4,l_tlv);
			}
			else
			{
				//if(l_tlv>254)
				//	continue;
				memcpy(g9f4b,R_Apdu.data+i+3,l_tlv);
			}
						
			uLen_g9f4b = l_tlv;
								
			break;
		}
	}
	uLen_g9f10=0;
	for(i=0;i<R_Apdu.length;i++)
	{
		if(R_Apdu.data[i]==0x9f && R_Apdu.data[i+1]==0x10 )
		{
			l_tlv = R_Apdu.data[i+2];
			
			if(l_tlv ==0x81)
			{
				l_tlv = R_Apdu.data[i+3];

				if(l_tlv>32)	continue;//20130723
				memcpy(g9f10,R_Apdu.data+i+4,l_tlv);
			}
			else
			{
				if(l_tlv>32)	continue;//20130723
				memcpy(g9f10,R_Apdu.data+i+3,l_tlv);
			}
					
				
			uLen_g9f10 = l_tlv;
							
			break;
		}
	}
	if(i>=R_Apdu.length)
	{
		return ERR_NO9f10;

	}
	memset(gCVR,0xff,4);
	if(uLen_g9f10>0)
	{
		memcpy(gCVR,g9f10+3,4);
		
	}
	memset(g9f5d,0x00,sizeof(g9f5d));
	uLen_g9f5d =0;
	for(i=0;i<R_Apdu.length;i++)
	{
		if(R_Apdu.data[i]==0x9f && R_Apdu.data[i+1]==0x5d)
		{
			l_tlv = R_Apdu.data[i+2];
			if(l_tlv ==6)
			{
				memcpy(g9f5d,R_Apdu.data+i+3,l_tlv);
			}
			uLen_g9f5d = l_tlv;
							
			break;
		}
	}
	
	if((gCVR[1]&0x30)==0x00) 
	{
	
		return ERR_CVR;
	}

	if((gCVR[1]&0x30)==0x20)  //arqc,��������
	{
	
		return ERR_CVR;
	}
	
	
	if(uLen_g9f10<7)
	{
			return ERR_LEN9f10;
	
	}
	if(nLen_AFL==0)
	{
		return ERR_NO94;

	}
	//9f6c
	for(i=0;i<R_Apdu.length;i++)
	{
		if(R_Apdu.data[i]==0x9f && R_Apdu.data[i+1]==0x6c)
		{
			l_tlv = R_Apdu.data[i+2];
			memcpy(g9f6c,R_Apdu.data+i+3,l_tlv);
							
			break;
		}
	}
	
	nRetCode = ReadRecords();
	if (nRetCode!=OK)
	{
		fprintf(stderr,"ReadRecords:uRetCode=%d",nRetCode);
		return (nRetCode) ;
	}

	nRetCode = fdda_L2(g9f06);	
	if(nRetCode==0)
		GenOutBuf(1,outbuf);	
	else
		GenOutBuf(0,outbuf);

	if (nRetCode!=OK)
	{
		if((g9f6c[0] & 0x20) ==0x20)
		{
		}
		else
		{
			switch(nRetCode)
			{
				case ERR_NOCAIDX:;
					break;
				default:
					break;		
			}
		}
		
	
		return (nRetCode) ;
	}
	
	return OK;
}

int pf_mobile_normal_ticket_get_proof(char *inbuf,char *outbuf)
{
	
	int nRetCode=0;
	stru_TransData_Head* s;
	char cTransData[sizeof(stru_TransData_Head)+sizeof(D_Entry)];
	UBYTE uTradeNum[3];

	memset(uTradeNum,0x00,3);
 	nRetCode = L2Buffer_RD(uTradeNum);
	if(nRetCode != OK )
	{
		fprintf(stderr,"\x0d\x0a get_proof L2Buffer_RD=%d \n\r ",nRetCode);
		return nRetCode ;
	}
/*
	memset(cTransData,0x00,sizeof(cTransData));
	GetTransData(cTransData);
	s = (stru_TransData_Head*)cTransData;

	if(memcmp(inbuf,s->TransNum,3)!=0)
		return ERR_PROOV_TRANSNUM;

	if(memcmp(inbuf+3,s->AppSn,10)!=0)
		return ERR_PROOV_APPSN;


	memcpy(gATC,s->ATC,sizeof(gATC));
	memcpy(outbuf+10,cTransData+sizeof(stru_TransData_Head),sizeof(D_Entry));
*/
	
	if(memcmp(inbuf,uTradeNum,3)!=0)
		return ERR_PROOV_TRANSNUM;

	if(memcmp(inbuf+3,gAPPAount_SN,10)!=0)
		return ERR_PROOV_APPSN;


	memcpy ( C_Apdu.data , "\x80\x5a\x00\x00\x02",5);
	memcpy ( C_Apdu.data+5 , gATC,2);
	C_Apdu.data[7]=0x08;
	C_Apdu.length = 8 ;
	nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
	//20130722,nRetCode != OK,���������״��� 	
	if(nRetCode != OK )
	{
		fprintf(stderr,"\x0d\x0a get_proof 80 5a :%04X",nRetCode);
		return nRetCode ;
	}
//	outbuf[1]=0x08;
//	memcpy(outbuf+2,R_Apdu.data,8);


	nRetCode =  GetProofProcess(outbuf);
	if(nRetCode != OK )
	{
		fprintf(stderr,"\x0d\x0a GetProofProcess=%d \x0d\x0a ",nRetCode);
		return nRetCode ;
	}

	outbuf[0]=0x00;


	return 0;
}
#define _DEBUG_UPDATE_
int pf_mobile_normal_ticket_update(char *inbuf,char *outbuf)
{
	int i,ret,nAmount=0;
	unsigned char uTmp[16];
#ifdef _DEBUG_UPDATE_	
	printf("\n\r  pf_mobile_normal_ticket_update inbuf:");	
	for(i=0;i<29;i++)
		printf("%02X ",inbuf[i]);
#endif
	memset(g9f02,0x00,sizeof(g9f02));
	memset(uTmp,0x00,sizeof(uTmp));
	memcpy(uTmp+1,inbuf+10,3);
#ifdef _DEBUG_UPDATE_
	fprintf(stderr,"\n\r uTmp:");	
	for(i=0;i<4;i++)
		fprintf(stderr,"%02X ",uTmp[i]);
#endif
	gBytes2Int(uTmp,&nAmount);
	sprintf((char*)uTmp,"%012d",nAmount);
	gStr2Bytes(12,uTmp,g9f02);
#ifdef _DEBUG_UPDATE_
	fprintf(stderr,"\n\r inbuf:");	
	for(i=0;i<16;i++)
		fprintf(stderr,"%02X ",inbuf[i]);

	fprintf(stderr,"\n\r nAmount = %d,g9f02:",nAmount);
	for(i=0;i<6;i++)
		fprintf(stderr,"%02X ",g9f02[i]);
#endif
	memcpy(g9a,inbuf+1,3);
	memcpy(g9f21,inbuf+4,3); //trade time

	gR_Update=(R_Update*)inbuf;
	ret=L2Process(10043,inbuf,outbuf);
	if(ret==0)//20130724
		GenOutBuf(1,outbuf);	
	else
		GenOutBuf(0,outbuf);

#ifdef _DEBUG_UPDATE_
	printf("\n\r  L2Process return=%d:\n\r",ret);	
#endif
	if(ret>MAXNOCARDRETCODE)
		return ret * -1;
	return ret;
	

}
#define _DEBUG_MEC_ENTRY_
int pf_mobile_staff_ticket_entry(char *inbuf,char *outbuf)
{
	int ret,i;
#ifdef _DEBUG_MEC_ENTRY_
	fprintf(stderr,"\n\r pf_mobile_staff_ticket_entry inbuf:");	
	for(i=0;i<11;i++)
		fprintf(stderr,"%02X ",inbuf[i]);
#endif
	if(gCardBusinessType==0x90)
	{	
		/*
		ret = PPSE_MEC();
		if(ret!=0)
		{
			fprintf(stderr,"\n\r err PPSE MEC ret=%d",ret);
			return ret;
		}
		*/
		gR_Entry=(R_Entry* )inbuf;
		ret = MECProcess(10041,inbuf,outbuf);
		if(ret>0)
			ret = ret * (-1);
#ifdef _DEBUG_MEC_ENTRY_
		printf("\r\n MECProcess return=%d ,%04X\r\n",ret,ret*-1);
#endif
		return ret;
	}
	return ERR_CALL_BUSITYPE;
}
#define _DEBUF_MEC_EXIT_
int pf_mobile_staff_ticket_exit(char *inbuf,char *outbuf)
{
	int ret,i;
#ifdef _DEBUF_MEC_EXIT_
	fprintf(stderr,"\n\r pf_mobile_staff_ticket_entry inbuf:");	
	for(i=0;i<16;i++)
		fprintf(stderr,"%02X ",inbuf[i]);
#endif
	if(gCardBusinessType==0x90)
	{	
		/*
		ret = PPSE_MEC();
		if(ret!=0)
		{
			fprintf(stderr,"\n\r err PPSE MEC ret=%d",ret);
			return ret;
		}
		*/
		gR_Exit=(R_Exit*)inbuf;
		ret = MECProcess(10042,inbuf,outbuf);
		if(ret>0)
			ret = ret * (-1);
#ifdef _DEBUF_MEC_EXIT_
		printf("\r\n MECProcess return=%d ,%04X\r\n",ret,ret*-1);
#endif
		return ret;
	}
	return ERR_CALL_BUSITYPE;
}

int pf_mobile_staff_ticket_update(char *inbuf,char *outbuf)
{
	int ret,i;
/*
	printf("\n\r pf_mobile_staff_ticket_update inbuf:");	
	for(i=0;i<29;i++)
		printf("%02X ",inbuf[i]);

*/

	if(gCardBusinessType==0x90)
	{	
		
		gR_Update=(R_Update*)inbuf;
		ret = MECProcess(10043,inbuf,outbuf);
		
		if(ret>0)
			ret = ret * (-1);
		printf("\n\r MECProcess return =%d \n\r",ret);	
		return ret;
	}
	return ERR_CALL_BUSITYPE;
}

int pf_mobile_staff_ticket_get_proof(char *inbuf,char *outbuf)
{
	int ret=0;
	int nAtc=0;
	char uAtc[2];
	UBYTE uTmp[256],TAC[4];
	int 	nYJE=0;
	int nAmount = 0;
	//��ȡ���һ�ʽ�������,20130725
	memset(uTmp,0x00,sizeof(uTmp));
	ret = MECBuffer_RD(uTmp,sizeof(S_MECBuffer));
	if(ret!=0)
		return ERR_GTPROOL_EEPRDERR;

	gS_MECBuffer=(S_MECBuffer*)uTmp;
/*
	 = NDX_MEC;
	=uKeyVer;
	memcpy(gS_MECBuffer->OffTradeCount,uAtc,2);
	memcpy(,uPSAMCounter,4);
	if(nTrade==10041)
		memcpy(gS_MECBuffer->TransNum,gR_Entry->TradeNum,3);

	if(nTrade==10042)
	{
		memcpy(,gR_Exit->Amount,3);
		memcpy(gS_MECBuffer->TransNum,gR_Exit->TradeNum,3);

	}
	if(nTrade==10043)
	{
		memcpy(gS_MECBuffer->Amount,gR_Update->Amount,3);
		memcpy(gS_MECBuffer->TransNum,gR_Update->TradeNum,3);
	}
	memcpy(gS_MECBuffer->uBalance,gBalance,4);
	memcpy(gS_MECBuffer->AppSn,gAppSN,10);
	
*/

	memset(uAtc,0x00,sizeof(uAtc));
	gBytes2Short(gS_MECBuffer->OffTradeCount,&nAtc);
	nAtc++;
	gShort2Bytes(nAtc,uAtc);	
	memcpy ( C_Apdu.data , "\x80\x5a\x00\x09\x02",5);
	memcpy ( C_Apdu.data+5 , uAtc,2);//gATC +1

	C_Apdu.data[7]=0x08;
	C_Apdu.length = 8 ;

	
	ret = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
	if(ret>0)
		ret = ret * (-1);

	if(ret != OK )
	{
		fprintf(stderr,"\x0d\x0a mec get_proof 80 5a :%04X,gS_MECBuffer->OffTradeCount=%02X%02X",ret,gS_MECBuffer->OffTradeCount[0],gS_MECBuffer->OffTradeCount[1]);
		return ret ;
	}
	memcpy(TAC,R_Apdu.data+4,4);

	//20130725
	outbuf[0]=0;
	gD_MECExit=(D_MECExit*)(outbuf+1);
	gD_MECExit->KeyIndex= gS_MECBuffer->KeyIndex;
	gD_MECExit->KeyVer=gS_MECBuffer->KeyVer;

	//memcpy(gD_MECExit->OffTradeCount,uAtc,2);
	memcpy(gD_MECExit->OffTradeCount,gS_MECBuffer->OffTradeCount,2);
	memcpy(gD_MECExit->TermTradeCount,gS_MECBuffer->TermTradeCount,4);
	memcpy(gD_MECExit->TAC,TAC,4);

	nYJE=0;
	nAmount = 0;
	gBytes2Int(gS_MECBuffer->uBalance,&nYJE);
	printf("\r\n gS_MECBuffer:%d \r\n",nAmount);
	gBytes2Short3(gS_MECBuffer->Amount,&nAmount);
	nYJE = nYJE - nAmount;
	gShort32Bytes(nYJE,gD_MECExit->BALANCE);


	return 0;
}

int pf_mobile_staff_ticket_lock(char *inbuf,char *outbuf)
{
	int ret,i;

	printf("\n\r pf_mobile_staff_ticket_lock inbuf:");	
	for(i=0;i<29;i++)
		printf("%02X ",inbuf[i]);


	if(gCardBusinessType==0x90)
	{	
		
		gR_Lock=(R_Lock*)inbuf;
		ret = MECProcess(10044,inbuf,outbuf);
		
		if(ret>0)
			ret = ret * (-1);
		printf("\n\r MECProcess return =%d \n\r",ret);	
		return ret;
	}
	return ERR_CALL_BUSITYPE;
	
}
int pf_mobile_ticket_get_record(char  *inbuf,char  *outbuf)
{
	int nRetCode=0,nOffset;
	UBYTE i,uTmp,uRecordNo;

	C_Apdu.length = 5 ;

	uTmp = inbuf[1];	
	nOffset = 1;
	for(i=1;i<=8;i++)
	{
		if((uTmp & 0x01)==0x01)
		{
			uRecordNo = i;
			memcpy ( C_Apdu.data , "\x00\xb2\x01\xf4\x20",5);
			C_Apdu.data[2]=uRecordNo;
			nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
			if(nRetCode>0)
				nRetCode = nRetCode * -1;
			if (nRetCode!=OK)
			{
				return (nRetCode) ;
			}
			outbuf[nOffset]=uRecordNo-1;
			nOffset=nOffset+1;
			memcpy(outbuf+nOffset,R_Apdu.data,32);
			nOffset=nOffset+32;
		}
		uTmp = uTmp >> 1;
	}
	

	uTmp = inbuf[0];	
	for(i=9;i<=10;i++)
	{
		if((uTmp & 0x01)==0x01)
		{
			uRecordNo = i;
			memcpy ( C_Apdu.data , "\x00\xb2\x01\xf4\x20",5);
			C_Apdu.data[2]=uRecordNo;
			nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
			if(nRetCode>0)
				nRetCode = nRetCode * -1;
			if (nRetCode!=OK)
			{
				return (nRetCode) ;
			}
			outbuf[nOffset]=uRecordNo-1;
			nOffset=nOffset+1;
			memcpy(outbuf+nOffset,R_Apdu.data,32);
			nOffset=nOffset+32;
		}
		uTmp = uTmp >> 1;
		
	}
	
	outbuf[0]=0x00;
	return 0;
}
int pf_mobile_staff_ticket_get_record(char  *inbuf,char  *outbuf)
{
	int nRetCode=0,nOffset=1;
	UBYTE i,uTmp,uRecordNo;

	C_Apdu.length = 5 ;

	uTmp = inbuf[1];	
	for(i=1;i<=8;i++)
	{
		if((uTmp & 0x01)==0x01)
		{
			uRecordNo = i;
			memcpy ( C_Apdu.data , "\x00\xb2\x01\xc4\x17",5);//��18�ļ�
			C_Apdu.data[2]=uRecordNo;
			nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
			printf("\n\r buf1=%02X  get MEC transdata , 00 b2 nRetCode=  %04X\n\r",inbuf[1],nRetCode);
			if(nRetCode>0)
				nRetCode = nRetCode * -1;
			if (nRetCode!=OK)
			{
				return (nRetCode) ;
			}
			outbuf[nOffset]=uRecordNo-1;
			nOffset=nOffset+1;
			memcpy(outbuf+nOffset,R_Apdu.data,0x17);
			nOffset=nOffset+0x17;
		}
		uTmp = uTmp >> 1;
	}
	

	uTmp = inbuf[0];	
	for(i=9;i<=10;i++)
	{
		if((uTmp & 0x01)==0x01)
		{
			uRecordNo = i;
			memcpy ( C_Apdu.data , "\x00\xb2\x01\xc4\x17",5);
			C_Apdu.data[2]=uRecordNo;
			nRetCode = PBOC_apdu_Ext(&C_Apdu, &R_Apdu );
			printf("\n\r buf0=%02X  get MEC transdata , 00 b2 nRetCode=  %04X\n\r",inbuf[0],nRetCode);
			if(nRetCode>0)
				nRetCode = nRetCode * -1;
			if (nRetCode!=OK)
			{
				return (nRetCode) ;
			}
			outbuf[nOffset]=uRecordNo-1;
			nOffset=nOffset+1;
			memcpy(outbuf+nOffset,R_Apdu.data,0x17);
			nOffset=nOffset+0x17;
		}
		uTmp = uTmp >> 1;
		
	}
	
	outbuf[0]=0x00;
	return 0;
}
//20130729
//#define _DEBUG_PARAMSHA_
int pf_mobile_ticket_para_sha1(char *inbuf)
{
	S_TerminalParam_Head* pTmp;
	S_TerminalParam_Record* p_R;
	unsigned char uTmp[512],uDataOut[256];
	unsigned int nLen_DataIn=0;
	int i,j,priModuleLen;

	pTmp=(S_TerminalParam_Head*)inbuf;
#ifdef _DEBUG_PARAMSHA_
	printf("\n\r pf_mobile_ticket_para_sha1,inbuf:\n\r");	
	for(i=0;i<(pTmp->RecordNum[0] * 296)+41;i++)
	{
		printf("%02X ",inbuf[i]);
		if((i+1) % 16 ==0)
			printf("\n\r");
	}
	printf("\n\r");

	printf("\n\r param,sha1:\n\r");	
	for(i=0;i<20;i++)
	{
		printf("%02X ",pTmp->SHA1[i]);
		if((i+1) % 16 ==0)
			printf("\n\r");
	}
	printf("\n\r");

#endif
	
	memset(uDataOut,0x00,sizeof(uDataOut));
	nLen_DataIn = 296 * pTmp->RecordNum[0];
#ifdef _DEBUG_PARAMSHA_
	printf("\n\r param,nLen_DataIn=%d:\n\r",nLen_DataIn);	
	for(i=0;i<nLen_DataIn;i++)
	{
		printf("%02X ",inbuf[i+41]);
		if((i+1) % 16 ==0)
			printf("\n\r");
	}
	printf("\n\r");
#endif
	sha(inbuf+41,nLen_DataIn,uDataOut);

#ifdef _DEBUG_PARAMSHA_
	printf("\n\r SHA1,uDataOut:\n\r");	
	for(i=0;i<20;i++)
	{
		printf("%02X ",uDataOut[i]);
		if((i+1) % 16 ==0)
			printf("\n\r");
	}
	printf("\n\r");
#endif

	if(memcmp(uDataOut,pTmp->SHA1,20)!=0)
		return -1;
	
#ifdef _DEBUG_PARAMSHA_
	printf("\n\r reccount=%d \r\n",pTmp->RecordNum[0]);
#endif

	for(i=0;i<pTmp->RecordNum[0];i++)
	{
		
		p_R = ( S_TerminalParam_Record*)(inbuf+41+i*296);	

		nLen_DataIn =0;
		memset(uTmp,0,sizeof(uTmp));
		memcpy(uTmp,p_R->RID,5);
		nLen_DataIn = nLen_DataIn +5;
		uTmp[5]=p_R->CAIndex;
		nLen_DataIn = nLen_DataIn +1;

 		priModuleLen= p_R->ModuleLen[0]*0x100+p_R->ModuleLen[1];
		//priModuleLen =256;
		memcpy(uTmp+nLen_DataIn,p_R->Module,priModuleLen);
		nLen_DataIn = nLen_DataIn + priModuleLen;

		//memcpy(uTmp+nLen_DataIn,p_R->CAExp,4);
		//nLen_DataIn = nLen_DataIn +4;

		for(j=0;j<4;j++)
		{
			if(p_R->CAExp[j]!=0x00)
			{
				uTmp[nLen_DataIn]=p_R->CAExp[j];
				nLen_DataIn = nLen_DataIn +1;
			}
		}

#ifdef _DEBUG_PARAMSHA_
		printf("\n\r i=%d,uTmp[%d]:\n\r",i,nLen_DataIn);	
		for(j=0;j<nLen_DataIn;j++)
		{
			printf("%02X ",uTmp[j]);
			if((j+1) % 16 ==0)
				printf("\n\r");
		}
		printf("\n\r");
#endif
		sha(uTmp,nLen_DataIn,uDataOut);

#ifdef _DEBUG_PARAMSHA_
		printf("\n\r i=%d,p_R->SHA1:\n\r",i);	
		for(j=0;j<20;j++)
		{
			printf("%02X ",p_R->SHA1[j]);
			if((j+1) % 16 ==0)
				printf("\n\r");
		}
		printf("\n\r");
		printf("\n\r i=%d,SHA1,uDataOut:\n\r",i);	
		for(j=0;j<20;j++)
		{
			printf("%02X ",uDataOut[j]);
			if((j+1) % 16 ==0)
				printf("\n\r");
		}
		printf("\n\r");

#endif
		if(memcmp(uDataOut,p_R->SHA1,20)!=0)
			return -1 * (i+2) ;
	}
	return 0;
}

void pth801A()
//void *pth801A()
{
	int ret;
	int i;
	UBYTE rbytes,uTmp_Len,ta1_H,ta1_L,ta1;
	UBYTE pszBuf[64],buf[256];

	//80 1A 46 03 10
	rbytes=0;
	memset(pszBuf,0x00,sizeof(pszBuf));
	memcpy(pszBuf,"\x80\x1A\x28\x02\x08",5);
	pszBuf[3] = 2;
	memcpy(pszBuf+5,gAPPAount_SN+3,8);
	memset(buf,0x00,sizeof(buf));
	ret = MyCmdSamExchange_Send(PSAM_NDX, 13,pszBuf);
	if(ret!=0) 
	{

#ifdef _DEBUG_PSAM_MAC_
		fprintf(stderr,"\n\r 80 1A:ret=%d ",ret);
#endif
		sem_post(&g_801Areturn);
		return ;
	}
/*	if(buf[rbytes-2]!=0x90 || buf[rbytes-1]!=0x00) 
	{
		ret = buf[rbytes-2];
		ret = ret * 0x100+buf[rbytes-1];
#ifdef _DEBUG_PSAM_MAC_
		fprintf(stderr,"\n\r 80 1A:ret=%04X ",(unsigned int)ret);
#endif
		sem_post(&g_801Areturn);
		return ;
	}
	sem_post(&g_801Areturn);*/
	
	return;
}	// 
	
void *pth001E()
{
UBYTE uTmp[256];
UBYTE	Diversity[16],Key[16],pszInitData[64],pszBuf[128], buf[20];
	memset(pszInitData,0x00,64);
	
	
	memcpy(pszInitData,"\x00\x00\x00\x00\x00\x00\x00\x00",8);
	memcpy(pszInitData+6,gATC,2);
	
	memset(pszBuf,0x00,sizeof(pszBuf));
	memcpy(uTmp,"\x84\xDE\x00\xf0\x24",5);
	memcpy(pszBuf,uTmp,5);
	pszBuf[37] = 0x80;
	memset(buf,0x00,sizeof(buf));

	PSAM_MAC(PSAM_NDX,4,gAPPAount_SN+3,pszInitData,pszBuf,37+3,buf);
	memcpy(mac_001e, buf, 4);
	sem_post(&g_801Areturn);
	
	return;
}	// 
	
#endif
