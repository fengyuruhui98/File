/*-------------------------------------------------------------------------
	Function Name: 	WatchDiversity
	Time:			2005-5-29 16:22:52
	Author:			Hsin Honway(xinhongwei@sina.com)
	Parameters:
					pszMKKey	[in]		16�ֽڵ�����Կ
					pszPID		[in]		8�ֽڵķ�ɢ����
					pszSKKey	[out]		16�ֽڵķ�ɢ���
					bTriDes		[in]		DES����3DES���ܡ�
	Return:
	
	Remarks:
					�������Կ��ɢ�㷨	
					2005-05-29������ΰ�Ѿ���֤OK.	
-------------------------------------------------------------------------*/
unsigned char	WatchDiversity(unsigned char *pszMKKey,unsigned char *pszPID,unsigned char	*pszSKKey,unsigned char	bTriDes)
{
	int		i;
	unsigned char	pszBuf[16];
	unsigned char	pszKey[16];
	
	if(bTriDes)
	{
		memcpy(pszKey,pszMKKey,16);
		
		memset(pszBuf,0x00,16);
		
		//	���8�ֽ�
		des_encode(pszKey, pszPID, pszBuf);
		des_decode(pszKey+8, pszBuf,pszBuf+8);
		des_encode(pszKey, pszBuf+8, pszBuf);
		memcpy(pszSKKey,pszBuf,8);
		
		//	��������ȡ��
		memset(pszBuf,0x00,16);			
		for(i=0;i<8;i++) pszBuf[i] =~pszPID[i];
		
		//	�ұ�8�ֽ�
		des_encode(pszKey, pszBuf, pszBuf+8);
		des_decode(pszKey+8, pszBuf+8,pszBuf);
		des_encode(pszKey, pszBuf, pszBuf+8);	
		memcpy(pszSKKey+8,pszBuf+8,8);
	}
	else
	{
		des_encode(pszMKKey,pszPID, pszSKKey);
	}
	return 0;
}






/*-------------------------------------------------------------------------
	Function Name: 	CmdWatchCalMac
	Time:			2008-8-10 11:53:56
	Author:			Xin Hong-wei(kpsoliton@gmail.com)
	Parameters:
	
	Return:
	
	Remarks:
	����Ĺ������£�
		1. �����ݿ�pszBufIn����Ϊ��8�ֽ�Ϊ��λ���飻
		2. ������һ�����ݿ鳤��Ϊ8�ֽڣ���ô�������80 00 00 00 00 00 00 00
		3. ������һ�����ݿ鳤�Ȳ���8�ֽڣ���ô�ں������80,���������8�ֽڣ������00ֱ��Ϊ8�ֽڡ�
		4. ��ʼֵpszInitData�͵�һ�����ݿ����У�飬������ֵ����ԿpszKey���ܣ�
		5. �õ���ֵ��ڶ������ݿ����У�飬������ֵ����ԿpszKey���ܣ�
		6. ֱ�����һ�����ݿ����У�飬����Ϊֹ��
	
-------------------------------------------------------------------------*/
void	CmdWatchCalMac(unsigned short nLenIn, unsigned char *pszBufIn, unsigned char *pszInitData, unsigned char *pszKey, unsigned char *pszMAC, unsigned char	bTriDes)
{
unsigned char		pszBufAdd[8];
unsigned char		pszBuf[8];
unsigned short		i, j;

unsigned short		nLenTotal;		//	�������һ�����ݿ�ĳ��ȡ�
	
	nLenTotal = (nLenIn/8 + 1) * 8;		
	
	//	����Ĳ���
	memcpy(pszBufAdd,"\x80\x00\x00\x00\x00\x00\x00\x00",8);

	//	������ʼ��ֵ
	memcpy(pszBuf, pszInitData, 8);
	
	for(i = 0; i < nLenTotal; i = i + 8) 
	{
		//	ȡ��8���ֽ����м�ֵ���������
		for(j = 0; j < 8; j++)
		{
			if((i + j) < nLenIn)
			{
				pszBuf[j] = pszBufIn[i+j] ^ pszBuf[j];
			}
			else
			{
				pszBuf[j] = pszBufAdd[i + j - nLenIn] ^ pszBuf[j];
			}
		}
		
		//	DES����,�����3DES��ôֻ����벿�ֽ��м���
		des_encode(pszKey, pszBuf, pszBuf);
	
	}
	
	//	�����3DES����ô�Ұ벿�ֽ��ܣ���벿�ּ���
	if(bTriDes)
	{
		des_decode(pszKey + 8,pszBuf, pszBuf);
		des_encode(pszKey, pszBuf, pszBuf);
	}
	
	//	�õ�8���ֽڣ�ǰ��4����ΪMAC.
	memcpy(pszMAC, pszBuf, 4);

	return;
}

