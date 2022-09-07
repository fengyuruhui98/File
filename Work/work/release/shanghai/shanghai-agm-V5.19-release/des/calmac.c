/*-------------------------------------------------------------------------
	Function Name: 	WatchDiversity
	Time:			2005-5-29 16:22:52
	Author:			Hsin Honway(xinhongwei@sina.com)
	Parameters:
					pszMKKey	[in]		16字节的主密钥
					pszPID		[in]		8字节的分散因子
					pszSKKey	[out]		16字节的分散结果
					bTriDes		[in]		DES或者3DES加密。
	Return:
	
	Remarks:
					握奇的密钥分散算法	
					2005-05-29日辛宏伟已经验证OK.	
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
		
		//	左边8字节
		des_encode(pszKey, pszPID, pszBuf);
		des_decode(pszKey+8, pszBuf,pszBuf+8);
		des_encode(pszKey, pszBuf+8, pszBuf);
		memcpy(pszSKKey,pszBuf,8);
		
		//	输入数据取反
		memset(pszBuf,0x00,16);			
		for(i=0;i<8;i++) pszBuf[i] =~pszPID[i];
		
		//	右边8字节
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
	计算的过程如下：
		1. 把数据块pszBufIn划分为以8字节为单位的组；
		2. 如果最后一个数据块长度为8字节，那么后面添加80 00 00 00 00 00 00 00
		3. 如果最后一个数据块长度不是8字节，那么在后面添加80,如果还不够8字节，再添加00直到为8字节。
		4. 初始值pszInitData和第一个数据块异或校验，产生的值用密钥pszKey加密，
		5. 得到的值与第二个数据块异或校验，产生的值用密钥pszKey加密，
		6. 直到最后一个数据块异或校验，加密为止。
	
-------------------------------------------------------------------------*/
void	CmdWatchCalMac(unsigned short nLenIn, unsigned char *pszBufIn, unsigned char *pszInitData, unsigned char *pszKey, unsigned char *pszMAC, unsigned char	bTriDes)
{
unsigned char		pszBufAdd[8];
unsigned char		pszBuf[8];
unsigned short		i, j;

unsigned short		nLenTotal;		//	补足最后一个数据块的长度。
	
	nLenTotal = (nLenIn/8 + 1) * 8;		
	
	//	补足的部分
	memcpy(pszBufAdd,"\x80\x00\x00\x00\x00\x00\x00\x00",8);

	//	拷贝初始的值
	memcpy(pszBuf, pszInitData, 8);
	
	for(i = 0; i < nLenTotal; i = i + 8) 
	{
		//	取出8个字节与中间值做异或运算
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
		
		//	DES运算,如果是3DES那么只用左半部分进行加密
		des_encode(pszKey, pszBuf, pszBuf);
	
	}
	
	//	如果是3DES，那么右半部分解密，左半部分加密
	if(bTriDes)
	{
		des_decode(pszKey + 8,pszBuf, pszBuf);
		des_encode(pszKey, pszBuf, pszBuf);
	}
	
	//	得到8个字节，前面4个作为MAC.
	memcpy(pszMAC, pszBuf, 4);

	return;
}

