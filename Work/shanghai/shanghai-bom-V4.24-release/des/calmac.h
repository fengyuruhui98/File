#ifndef _FASTDES_H_
#define _FASTDES_H_


unsigned char	WatchDiversity(unsigned char *pszMKKey,unsigned char *pszPID,unsigned char	*pszSKKey,unsigned char	bTriDes);
void	CmdWatchCalMac(unsigned short nLenIn, unsigned char *pszBufIn, unsigned char *pszInitData, unsigned char *pszKey, unsigned char *pszMAC, unsigned char	bTriDes);


#endif