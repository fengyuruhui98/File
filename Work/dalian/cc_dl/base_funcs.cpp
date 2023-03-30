//
// Created by HHJT_CJP on 19-12-10.
//

#include "base_funcs.h"

/*=========================================================================
   CIniFile : The constructor
*========================================================================*/
CIniFile::CIniFile (void)
{
    m_pEntry      = NULL;
    m_pCurEntry   = NULL;
    m_result [0]  = 0;
    m_pIniFile    = NULL;
}

/*=========================================================================
   CIniFile : The destructor
*========================================================================*/
CIniFile::~CIniFile (void)
{
    FreeAllMem ();
}

/*=========================================================================
   CIniFile : GetVersion
   Info     : The version is BCD coded. It maintain the major version in
              the upper 8 bits and the minor in the lower.
              0x0120 means version 1.20
*========================================================================*/
UINT CIniFile::GetVersion (void)
{
    return 0x0030;
}

/*=========================================================================
   OpenIniFile
  -------------------------------------------------------------------------
   Job : Opens an ini file or creates a new one if the requested file
         doesnt exists.
/*========================================================================*/
BOOL CIniFile::OpenIniFile (CCHR * FileName)
{
    char   Str [255];
    char   *pStr;
    struct ENTRY *pEntry;
    int Len;
    FreeAllMem ();

    if (FileName == NULL)                             { return FALSE; }
    if ((m_pIniFile = fopen (FileName, "r")) == NULL) { return FALSE; }

    while (fgets (Str, 255, m_pIniFile) != NULL)
    {
        pStr = strchr (Str, '\n');
        if (pStr != NULL) { *pStr = 0; }
        pEntry = MakeNewEntry ();
        if (pEntry == NULL) { return FALSE; }

#ifdef INI_REMOVE_CR
        Len = strlen(Str);
        if ( Len > 0 )
        {
            if ( Str[Len-1] == '\r' )
            {
                Str[Len-1] = '\0';
            }
        }
#endif

        pEntry->pText = (char *)malloc (strlen (Str)+1);
        if (pEntry->pText == NULL)
        {
            FreeAllMem ();
            return FALSE;
        }
        strcpy (pEntry->pText, Str);
        pStr = strchr (Str,';');
        if (pStr != NULL) { *pStr = 0; } /* Cut all comments */
        if ( (strstr (Str, "[") > 0) && (strstr (Str, "]") > 0) ) /* Is Section */
        {
            pEntry->Type = tpSECTION;
        }
        else
        {
            if (strstr (Str, "=") > 0)
            {
                pEntry->Type = tpKEYVALUE;
            }
            else
            {
                pEntry->Type = tpCOMMENT;
            }
        }
        m_pCurEntry = pEntry;
    }
    fclose (m_pIniFile);
    m_pIniFile = NULL;
    return TRUE;
}

/*=========================================================================
   CloseIniFile
  -------------------------------------------------------------------------
   Job : Closes the ini file without any modifications. If you want to
         write the file use WriteIniFile instead.
/*========================================================================*/
void CIniFile::CloseIniFile (void)
{
    FreeAllMem ();
    if (m_pIniFile != NULL)
    {
        fclose (m_pIniFile);
        m_pIniFile = NULL;
    }
}

/*=========================================================================
   WriteIniFile
  -------------------------------------------------------------------------
   Job : Writes the iniFile to the disk and close it. Frees all memory
         allocated by WriteIniFile;
/*========================================================================*/
bool CIniFile::WriteIniFile (const char *pFileName)
{
    struct ENTRY *pEntry = m_pEntry;
    if (m_pIniFile != NULL)
    {
        fclose (m_pIniFile);
    }
    if ((m_pIniFile = fopen (pFileName, "wb")) == NULL)
    {
        FreeAllMem ();
        return FALSE;
    }

    while (pEntry != NULL)
    {
        if (pEntry->Type != tpNULL)
        {
#			ifdef INI_REMOVE_CR
            fprintf (m_pIniFile, "%s\n", pEntry->pText);
#			else
            fprintf (m_pIniFile, "%s\r\n", pEntry->pText);
#			endif
        }
        pEntry = pEntry->pNext;
    }

    fclose (m_pIniFile);
    m_pIniFile = NULL;
    return TRUE;
}


/*=========================================================================
   WriteString : Writes a string to the ini file
*========================================================================*/
void CIniFile::WriteString (CCHR *pSection, CCHR *pKey, CCHR *pValue)
{
    EFIND List;
    char  Str [255];

    if (ArePtrValid (pSection, pKey, pValue) == FALSE) { return; }
    if (FindKey  (pSection, pKey, &List) == TRUE)
    {
        sprintf (Str, "%s=%s%s", List.KeyText, pValue, List.Comment);
        FreeMem (List.pKey->pText);
        List.pKey->pText = (char *)malloc (strlen (Str)+1);
        strcpy (List.pKey->pText, Str);
    }
    else
    {
        if ((List.pSec != NULL) && (List.pKey == NULL)) /* section exist, Key not */
        {
            AddKey (List.pSec, pKey, pValue);
        }
        else
        {
            AddSectionAndKey (pSection, pKey, pValue);
        }
    }
}

/*=========================================================================
   WriteBool : Writes a boolean to the ini file
*========================================================================*/
void CIniFile::WriteBool (CCHR *pSection, CCHR *pKey, bool Value)
{
    char Val [2] = {'0',0};
    if (Value != 0) { Val [0] = '1'; }
    WriteString (pSection, pKey, Val);
}

/*=========================================================================
   WriteInt : Writes an integer to the ini file
*========================================================================*/
void CIniFile::WriteInt (CCHR *pSection, CCHR *pKey, int Value)
{
    char Val [12]; /* 32bit maximum + sign + \0 */
    sprintf (Val, "%d", Value);
    WriteString (pSection, pKey, Val);
}

/*=========================================================================
   WriteDouble : Writes a double to the ini file
*========================================================================*/
void CIniFile::WriteDouble (CCHR *pSection, CCHR *pKey, double Value)
{
    char Val [32]; /* DDDDDDDDDDDDDDD+E308\0 */
    sprintf (Val, "%1.10lE", Value);
    WriteString (pSection, pKey, Val);
}


/*=========================================================================
   ReadString : Reads a string from the ini file
*========================================================================*/
char *CIniFile::ReadString (CCHR *pSection, CCHR *pKey, char *pDefault)
{
    EFIND List;
    if (ArePtrValid (pSection, pKey, pDefault) == FALSE) { return pDefault; }
    if (FindKey  (pSection, pKey, &List) == TRUE)
    {
        strcpy (m_result, List.ValText);
        return m_result;
    }
    return pDefault;
}

/*=========================================================================
   ReadBool : Reads a boolean from the ini file

*========================================================================*/
BOOL CIniFile::ReadBool (CCHR *pSection, CCHR *pKey, BOOL Default)
{
    char Val [2] = {"0"};
    if (Default != 0) { Val [0] = '1'; }
    return (atoi (ReadString (pSection, pKey, Val))?1:0); /* Only allow 0 or 1 */
}

/*=========================================================================
   ReadInt : Reads a integer from the ini file
*========================================================================*/
int CIniFile::ReadInt (CCHR *pSection, CCHR *pKey, int Default)
{
    char Val [12];
    sprintf (Val,"%d", Default);
    return (atoi (ReadString (pSection, pKey, Val)));
}

/*=========================================================================
   ReadDouble : Reads a double from the ini file
*========================================================================*/
double CIniFile::ReadDouble (CCHR *pSection, CCHR *pKey, double Default)
{
    double Val;
    sprintf (m_result, "%1.10lE", Default);
    sscanf (ReadString (pSection, pKey, m_result), "%lE", &Val);
    return Val;
}

/*=========================================================================
   DeleteKey : Deletes an entry from the ini file
*========================================================================*/
bool CIniFile::DeleteKey (CCHR *pSection, CCHR *pKey)
{
    EFIND         List;
    struct ENTRY *pPrev;
    struct ENTRY *pNext;

    if (FindKey (pSection, pKey, &List) == TRUE)
    {
        pPrev = List.pKey->pPrev;
        pNext = List.pKey->pNext;
        if (pPrev)
        {
            pPrev->pNext=pNext;
        }
        if (pNext)
        {
            pNext->pPrev=pPrev;
        }
        FreeMem (List.pKey->pText);
        FreeMem (List.pKey);
        return TRUE;
    }
    return FALSE;
}




/* Here we start with our helper functions */

void CIniFile::FreeMem (void *pPtr)
{
    if (pPtr != NULL) { free (pPtr); }
}

void CIniFile::FreeAllMem (void)
{
    struct ENTRY *pEntry;
    struct ENTRY *pNextEntry;
    pEntry = m_pEntry;
    while (1)
    {
        if (pEntry == NULL) { break; }
        pNextEntry = pEntry->pNext;
        FreeMem (pEntry->pText); /* Frees the pointer if not NULL */
        FreeMem (pEntry);
        pEntry = pNextEntry;
    }
    m_pEntry    = NULL;
    m_pCurEntry = NULL;
}

struct ENTRY *CIniFile::FindSection (CCHR *pSection)
{
    char Sec  [130];
    char iSec [130];
    struct ENTRY *pEntry;
    sprintf (Sec, "[%s]", pSection);
//	strupr  (Sec);
    pEntry = m_pEntry; /* Get a pointer to the first Entry */
    while (pEntry != NULL)
    {
        if (pEntry->Type == tpSECTION)
        {
            strcpy  (iSec, pEntry->pText);
//			strupr  (iSec);
            if (strcmp (Sec, iSec) == 0)
            {
                return pEntry;
            }
        }
        pEntry = pEntry->pNext;
    }
    return NULL;
}

bool CIniFile::FindKey  (CCHR *pSection, CCHR *pKey, EFIND *pList)
{
    char Search [130];
    char Found  [130];
    char Text   [255];
    char *pText;
    struct ENTRY *pEntry;
    pList->pSec        = NULL;
    pList->pKey        = NULL;
    pEntry = FindSection (pSection);
    if (pEntry == NULL) { return FALSE; }
    pList->pSec        = pEntry;
    pList->KeyText[0] = 0;
    pList->ValText[0] = 0;
    pList->Comment[0] = 0;
    pEntry = pEntry->pNext;
    if (pEntry == NULL) { return FALSE; }
    sprintf (Search, "%s",pKey);
//	strupr  (Search);
    while (pEntry != NULL)
    {
        if ((pEntry->Type == tpSECTION) || /* Stop after next section or EOF */
            (pEntry->Type == tpNULL   ))
        {
            return FALSE;
        }
        if (pEntry->Type == tpKEYVALUE)
        {
            strcpy (Text, pEntry->pText);
            pText = strchr (Text, ';');
            if (pText != NULL)
            {
                strcpy (pList->Comment, pText);
                *pText = 0;
            }
            pText = strchr (Text, '=');
            if (pText != NULL)
            {
                *pText = 0;
                strcpy (pList->KeyText, Text);
                strcpy (Found, Text);
                *pText = '=';
//				strupr (Found);
                /*            printf ("%s,%s\n", Search, Found); */
                if (strcmp (Found,Search) == 0)
                {
                    strcpy (pList->ValText, pText+1);
                    pList->pKey = pEntry;
                    return TRUE;
                }
            }
        }
        pEntry = pEntry->pNext;
    }
    return FALSE;
}

BOOL CIniFile::AddItem (char Type, CCHR *pText)
{
    struct ENTRY *pEntry = MakeNewEntry ();
    if (pEntry == NULL) { return FALSE; }
    pEntry->Type = Type;
    pEntry->pText = (char*)malloc (strlen (pText) +1);
    if (pEntry->pText == NULL)
    {
        free (pEntry);
        return FALSE;
    }
    strcpy (pEntry->pText, pText);
    pEntry->pNext   = NULL;
    if (m_pCurEntry != NULL) { m_pCurEntry->pNext = pEntry; }
    m_pCurEntry    = pEntry;
    return TRUE;
}

bool CIniFile::AddItemAt (struct ENTRY *pEntryAt, char Mode, CCHR *pText)
{
    struct ENTRY *pNewEntry;

    if (pEntryAt == NULL)  { return FALSE; }
    pNewEntry = (struct ENTRY*) malloc (sizeof (ENTRY));
    if (pNewEntry == NULL) { return FALSE; }
    pNewEntry->pText = (char *) malloc (strlen (pText)+1);
    if (pNewEntry->pText == NULL)
    {
        free (pNewEntry);
        return FALSE;
    }
    strcpy (pNewEntry->pText, pText);
    if (pEntryAt->pNext == NULL) /* No following nodes. */
    {
        pEntryAt->pNext   = pNewEntry;
        pNewEntry->pNext  = NULL;
    }
    else
    {
        pNewEntry->pNext = pEntryAt->pNext;
        pEntryAt->pNext  = pNewEntry;
    }
    pNewEntry->pPrev = pEntryAt;
    pNewEntry->Type  = Mode;
    return TRUE;
}

bool CIniFile::AddSectionAndKey (CCHR *pSection, CCHR *pKey, CCHR *pValue)
{
    char Text [255];
    sprintf (Text, "[%s]", pSection);
    if (AddItem (tpSECTION, Text) == FALSE) { return FALSE; }
    sprintf (Text, "%s=%s", pKey, pValue);
    return AddItem (tpKEYVALUE, Text)? 1 : 0;
}

void CIniFile::AddKey (struct ENTRY *pSecEntry, CCHR *pKey, CCHR *pValue)
{
    char Text [255];
    sprintf (Text, "%s=%s", pKey, pValue);
    AddItemAt (pSecEntry, tpKEYVALUE, Text);
}

struct ENTRY *CIniFile::MakeNewEntry (void)
{
    struct ENTRY *pEntry;
    pEntry = (struct ENTRY *)malloc (sizeof (ENTRY));
    if (pEntry == NULL)
    {
        FreeAllMem ();
        return NULL;
    }
    if (m_pEntry == NULL)
    {
        m_pEntry = pEntry;
    }
    pEntry->Type  = tpNULL;
    pEntry->pPrev = m_pCurEntry;
    pEntry->pNext = NULL;
    pEntry->pText = NULL;
    if (m_pCurEntry != NULL)
    {
        m_pCurEntry->pNext = pEntry;
    }
    return pEntry;
}






/* ------------------------------------------------------
            功能描述:   获得系统字节序;
            大端字节序返回1,小端字节序返回0;            
--------------------------------------------------------- */
int CommonFuncs::GetEndian()
{
	union
	{
		short s;
		char  c[sizeof(short)];
	}un;
	un.s = 0x0102;

	if (un.c[0] == 1 && un.c[1] == 2) /* --- 大端字节序 --- */
	{
		return 1;
	}
	else                              /* --- 小端字节序 --- */
	{
		return 0;
	}
}




/* ------------------------------------------------------
            功能描述:   字符串转换成十六进制数据
--------------------------------------------------------- */
void CommonFuncs::bcdtobyte(char *in,char *out,int len)
{
    char temp[3];
    char *token;
    int  i;

    for(i=0;i<len;i++)
    {
        memset(temp,0,sizeof(temp));
        memcpy(temp,in+i*2,2);
        out[i]=(char)strtoul(temp,&token,16);
    }
}

/* ------------------------------------------------------
功能描述:   删除右端空格
--------------------------------------------------------- */
void CommonFuncs::delspace(char *temp,char *buffer)
{
    int i,count_space,count_validchar;

    /* -------- 去掉右端的空格 ---------- */
    count_space = 0;
    for(i = strlen(buffer)-1;i>=0;i--)
    {
        if(buffer[i] == ' ')
        {
            count_space = count_space + 1;
        }
        else
        {
            break;
        }
    }
    /* -------- 有效字符 ---------- */
    count_validchar = (strlen(buffer) - count_space);
    memcpy(temp,buffer,count_validchar);
}

/* ------------------------------------------------------
            功能描述:   字符串转换成十六进制数据
--------------------------------------------------------- */
void CommonFuncs::chartobyte(char *in,char *out)
{
    char temp[3];
    char *token;

    memset(temp,0,sizeof(temp));
    memcpy(temp,in,2);
    out[0]=(char)strtol(temp,&token,16);
    memset(temp,0,sizeof(temp));
    memcpy(temp,in+2,2);
    out[1]=(char)strtol(temp,&token,16);
    memset(temp,0,sizeof(temp));
    memcpy(temp,in+4,2);
    out[2]=(char)strtol(temp,&token,16);
    memset(temp,0,sizeof(temp));
    memcpy(temp,in+6,2);
    out[3]=(char)strtol(temp,&token,16);
}

/* ------------------------------------------------------
            功能描述:   字符串转换成十六进制数据
--------------------------------------------------------- */
void CommonFuncs::chartobyte(const char *in,char *out,int len)
{
	char temp[3];
	char *token;
	int  i;

	for(i=0;i<len;i++)
	{
		memset(temp,0,sizeof(temp));
		memcpy(temp,in+i*2,2);
		out[i]=(char)strtoul(temp,&token,16);
	}
}


/* ------------------------------------------------------
            功能描述:   无符号长整数字节序颠倒
--------------------------------------------------------- */
unsigned int CommonFuncs::dwordtowin(unsigned int in)
{
	char temp1[6];
	char temp2[6];
	unsigned int dwtemp;
	
	dwtemp = in;
	if (GetEndian() == 0) /* --- 小端字节序 --- */
	{
		memcpy(temp1,&in,4);
		temp2[0]=temp1[3];
		temp2[1]=temp1[2];
		temp2[2]=temp1[1];
		temp2[3]=temp1[0];
		memcpy(&dwtemp,temp2,4);
	}

	return dwtemp;
}

/* ------------------------------------------------------
            功能描述:   无符号短整数字节序颠倒
--------------------------------------------------------- */
unsigned short CommonFuncs::wordtowin(unsigned short in)
{
	char temp1[6];
	char temp2[6];
	unsigned short  wtemp;

	wtemp = in;
	if (GetEndian() == 0) /* --- 小端字节序 --- */
	{
		memcpy(temp1,&in,2);
		temp2[0]=temp1[1];
		temp2[1]=temp1[0];
		memcpy(&wtemp,temp2,2);
	}

	return wtemp;
     
}


/* ------------------------------------------------------
            功能描述:   读取文件filename到buffer
--------------------------------------------------------- */
int CommonFuncs::ReadFileBuffer(char *filename,char *buffer)
{
    FILE          *fp;
    size_t        count,readcount;
    char          errormsg[1000];
    int           filelen;
    struct stat   buf;

    fp = fopen(filename,"rb");
    if(fp == NULL)
    {
        return 0;
    }

    stat(filename, &buf );
    filelen = buf.st_size;
    if(filelen == 0)
    {
        fclose(fp);
        return 0;
    }

    count=0;
    readcount=0;
    while(!feof(fp))
    {
        readcount = fread(buffer+readcount,sizeof(char),filelen-count,fp);
        if((ferror(fp))&& (!feof(fp)))
        {
            fclose(fp);
            return 0;
        }
        count+=readcount;
        if((int)count==filelen)
        {
            fclose(fp);
            return (int)count;
        }
    }
    fclose(fp);

    return (int)count;
}

/* ------------------------------------------------------
            功能描述:   写buffer到文件
--------------------------------------------------------- */
int CommonFuncs::WriteBufferFile(const char *flag,char *filename,char *recv_buf,int buflen)
{
    int      nleft,ncount;
    FILE*    fp;
    char     *buffer;

    fp=fopen(filename,flag);
    if(fp==NULL)
    {
        return 0;
    }

    nleft = buflen;
    buffer=(char*)recv_buf;
    while(nleft > 0)
    {
        ncount = fwrite(buffer,sizeof(char),nleft,fp);
        if(ferror(fp))
        {
            fclose(fp);
            return 0;
        }
        nleft = nleft - ncount;
        if(nleft != 0)
        {
            buffer = buffer + ncount;
        }
    }
    fclose(fp);

    return 1;
}



/* ------------------------------------------------------
            功能描述:  多字节转换有符号整数
--------------------------------------------------------- */
int  CommonFuncs::ByteToInt(unsigned char *b_int,int count)
{
    unsigned char b_temp,b_array[4];
    int  i,rtn;
    int  i_temp,bit;

    b_temp = b_int[0];
    b_temp = b_temp >> 7;
    if(b_temp == 1)/* --- 负数 ------ */
    {
        for(i = 0;i<count;i++)
            b_array[i] = ~b_int[i];

        i_temp = 0;
        bit    = count - 1;
        for(i = 0;i<count;i++)
        {
            i_temp += (b_array[i] << (8 * bit));
            bit    -= 1;
        }
        rtn = -i_temp;
        rtn = rtn - 1;
    }
    else/* --- 正数 ------ */
    {
        i_temp = 0;
        bit    = count - 1;
        for(i = 0;i<count;i++)
        {
            i_temp += (b_int[i] << (8 * bit));
            bit    -= 1;
        }

        rtn = i_temp;
    }

    return rtn;
}

int CommonFuncs::code_convert(const char *from_charset,const char *to_charset,char *inbuf,size_t inlen,char *outbuf, size_t outlen) 
{  
  iconv_t cd;  
  char **pin = &inbuf;  
  char **pout = &outbuf;  

  cd = iconv_open(to_charset, from_charset);  
  if (cd == 0)  
      return -1;  
  memset(outbuf, 0, outlen);  
  if (iconv(cd, pin, &inlen, pout, &outlen) == -1)  
      return -1;  
  iconv_close(cd);  
  *pout = '\0';  

  return 0;  
}  

int CommonFuncs::utf82gbk(const char *inbuf, size_t inlen, char *outbuf, size_t outlen) 
{  
    return code_convert("utf-8", "gb2312", (char*)inbuf, inlen, outbuf, outlen);  
}  


int CommonFuncs::gbk2utf8(const char *inbuf, size_t inlen, char *outbuf, size_t outlen) 
{  
    return code_convert("gb2312", "utf-8", (char*)inbuf, inlen, outbuf, outlen);  
} 
 
/* ------------------------------------------------------
            功能描述:  utf8码转为gbk码
--------------------------------------------------------- */
int CommonFuncs::utf82gbk(char *gbkStr, const char *srcStr, int maxGbkStrlen)
{
    if (NULL == srcStr)
    {
        return -1;
    }
    //首先先将utf8编码转换为unicode编码
    if (NULL == setlocale(LC_ALL, "zh_CN.utf8")) //设置转换为unicode前的码,当前为utf8编码
    {
        return -1;
    }

    int unicodeLen = mbstowcs(NULL, srcStr, 0); //计算转换后的长度
    if (unicodeLen <= 0)
    {
        return -1;
    }
    wchar_t *unicodeStr = (wchar_t *) calloc(sizeof(wchar_t), unicodeLen + 1);
    mbstowcs(unicodeStr, srcStr, strlen(srcStr)); //将utf8转换为unicode

    //将unicode编码转换为gbk编码
    if (NULL == setlocale(LC_ALL, "zh_CN.gbk")) //设置unicode转换后的码,当前为gbk
    {
        free(unicodeStr);
        unicodeStr = NULL;
        return -1;
    }
    int gbkLen = wcstombs(NULL, unicodeStr, 0); //计算转换后的长度
    if (gbkLen <= 0)
    {
        free(unicodeStr);
        unicodeStr = NULL;
        return -1;
    }
    else if (gbkLen >= maxGbkStrlen) //判断空间是否足够
    {
        free(unicodeStr);
        unicodeStr = NULL;
        return -1;
    }
    wcstombs(gbkStr, unicodeStr, gbkLen);
    gbkStr[gbkLen] = 0; //添加结束符

    free(unicodeStr);
    unicodeStr = NULL;

    return gbkLen;
}


/* ------------------------------------------------------
            功能描述:  unicode码转为UTF8码
--------------------------------------------------------- */
void CommonFuncs::UCS2toUTF8(const unsigned short *ucs2, int count, char *utf8)
{
    unsigned short unicode;
    unsigned char bytes[4] = {0};
    int nbytes = 0;
    int i = 0, j = 0;
    int len=0;
  
    if((ucs2 != NULL) && (utf8 != NULL))
    {
			if(count  == 0)
			{
				len = 0;
      }
      else
      {
				for (i=0; i<count; i++)
				{
					unicode = ucs2[i];
					if (unicode < 0x80)
					{
						nbytes = 1;
						bytes[0] = unicode;
					}
					else if (unicode < 0x800)
					{
						nbytes = 2;
						bytes[1] = (unicode & 0x3f) | 0x80;
						bytes[0] = ((unicode << 2) & 0x1f00 | 0xc000) >> 8;
					}
					else
					{
						nbytes = 3;
						bytes[2] = (unicode & 0x3f) | 0x80;
						bytes[1] = ((unicode << 2) & 0x3f00 | 0x8000) >> 8;
						bytes[0] = ((unicode << 4) & 0x0f0000 | 0xe00000) >> 16;
					}
					for (j=0; j<nbytes; j++)
					{
						utf8[len] = bytes[j];
						len++;
	        }
      	}
      }   
      utf8[len] = 0x00;
   	}
}

void CommonFuncs::ConvertCnchar(char *source_str,int len,char *dest_str)
{
	int  i;
	int  step;
	WORD *UCS2_char,tmp_UCS2;
	char *utf8;
	char *temp;
	
	UCS2_char = (WORD*)malloc(len*sizeof(WORD));
	memset(UCS2_char,0x00,len);
	step = 0;
	for(i = 0;i < len/4; i++)
	{
		memcpy(&tmp_UCS2,source_str+i*4+2,sizeof(tmp_UCS2));
		UCS2_char[step++] = wordtowin(tmp_UCS2);
	}
	
	utf8 = (char*)malloc(len);
	memset(utf8,0x00,len);
	UCS2toUTF8(UCS2_char,len/4,utf8);
	temp = (char*)malloc(len);
	memset(temp,0x00,len);
	utf82gbk(temp,utf8,len);
	delspace(dest_str,temp);
	
	free(UCS2_char);
	UCS2_char = NULL;
	free(utf8);
	utf8 = NULL;
	free(temp);
	temp = NULL;
}

/* ------------------------------------------------------
--------------------------------------------------------- */
void CommonFuncs::my_sleep(int timeout)
{
    struct timeval      timeoutSTRU;

    timeoutSTRU.tv_sec  = timeout;
    timeoutSTRU.tv_usec = 0;
    select(0,NULL,NULL,NULL,&timeoutSTRU);
}

/* ------------------------------------------------------
--------------------------------------------------------- */
void CommonFuncs::my_usleep(int timeout)
{
    struct timeval      timeoutSTRU;

    timeoutSTRU.tv_sec  = timeout / 1000;
    timeoutSTRU.tv_usec = (timeout % 1000) * 1000;
    select(0,NULL,NULL,NULL,&timeoutSTRU);
}

 
/* ------------------------------------------------------
           功能描述:  把IP地址转换成四个BYTE的二进制数据 
--------------------------------------------------------- */
void CommonFuncs::GetHexIP(char* ip,BYTE* LocalIP)
{
	char tempip[4];
	int  i,step;

	memset(tempip,0x00,sizeof(tempip));
	step=0;
	
	/* --------- 转换第一段IP地址 ----------------*/
	for(i = 0;i < 4;i++)
	{
		if(ip[step]=='.')
		{
			step++;
			break;
		}
		tempip[i]=ip[step];
		step++;
	}
	LocalIP[0]=(BYTE)atoi(tempip);
	
	/* --------- 转换第二段IP地址 ----------------*/
	memset(tempip,0x00,sizeof(tempip));
	for(i=0;i<4;i++)
	{
		if(ip[step]=='.')
		{
			step++;
			break;
		}
		tempip[i]=ip[step];
		step++;
	}
	LocalIP[1]=(BYTE)atoi(tempip);
	
	/* --------- 转换第三段IP地址 ----------------*/
	memset(tempip,0x00,sizeof(tempip));
	for(i=0;i<4;i++)
	{
		if(ip[step]=='.')
		{
			step++;
			break;
		}
		tempip[i]=ip[step];
		step++;
	}
	LocalIP[2]=(BYTE)atoi(tempip);

	/* --------- 转换第四段IP地址 ----------------*/
	memset(tempip,0x00,sizeof(tempip));
	for(i=0;i<4;i++)
	{
		if(ip[step]=='.')
		{
			step++;
			break;
		}
		tempip[i]=ip[step];
		step++;
	}
	LocalIP[3]=(BYTE)atoi(tempip);
}

/* ------------------------------------------------------
   功能描述:  获得本机IP
   返回值说明:成功返回1,失败返回错误代码
--------------------------------------------------------- */
int CommonFuncs::GetLocalHostInfo(char *localIP)
{
    struct ifaddrs * ifAddrStruct=NULL;  
    void * tmpAddrPtr=NULL;  
    char addressBuffer[INET_ADDRSTRLEN]; 
  
    getifaddrs(&ifAddrStruct);  
  
    while (ifAddrStruct!=NULL)   
    {  
        if (ifAddrStruct->ifa_addr->sa_family==AF_INET)  
        {    
            tmpAddrPtr = &((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;  
            
            memset(addressBuffer,0x00,sizeof(addressBuffer)); 
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN); 
            if(strcmp(addressBuffer,"127.0.0.1") != 0 && strcmp(ifAddrStruct->ifa_name,"virbr0") != 0) 
            {
            	strcpy(localIP,addressBuffer);
            	break;
            }  
        }  
        else if (ifAddrStruct->ifa_addr->sa_family==AF_INET6)  
        {   
            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;  
            char addressBuffer[INET6_ADDRSTRLEN];  
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);     
        }   
        ifAddrStruct = ifAddrStruct->ifa_next;  
    }  
    
    return 1;  
}	

/* ------------------------------------------------------
            功能描述:  判断进程是否存在（1 存在 0 不存在）
--------------------------------------------------------- */
int CommonFuncs::detect_process(const char *process_name)  
 {   
        FILE   *ptr;   
        char   buff[512];   
        char   ps[128];   
    
    	memset(ps,0x00,sizeof(ps));
        sprintf(ps,"ps -ef | grep -c '%s$'",process_name);   
        strcpy(buff,"ABNORMAL");   
        if((ptr=popen(ps, "r"))   !=   NULL)   
        {   
        	while   (fgets(buff,   512,   ptr)   !=   NULL)   
                {   
                	if(atoi(buff) >= 2)   
                        {   
                        	pclose(ptr);   
                                return   1;   
                        }   
                 }   
         }   
         if(strcmp(buff,"ABNORMAL")==0)     /*ps   command   error*/   
         {
                return   0;                    
         } 
         pclose(ptr); 
         return   0;   
 }

/* ------------------------------------------------------
            功能描述:  进程设为守护进程
--------------------------------------------------------- */
void CommonFuncs::Daemon_init()
{
	int pid;
	int i;
	if(pid = fork())
		exit(0);
	else if(pid<0)
		exit(1);
	setsid();
	chdir("./");
	umask(0);
}

/* ----------------------------------------------------- */
/*                  判断文件是否存在   
存在：返回1；不存在返回0                  */
/* ----------------------------------------------------- */
int CommonFuncs::FileIsExist(char *dirname,char *name)
{
	int           rtn;
	struct dirent *dirp;
	DIR           *dp;
  
  rtn = 0;
  
  	/* -------- 打开目录 ---------- */
	if((dp = opendir(dirname)) == NULL)
	{
		return rtn;
	}
	
	/* -------- 读取目录 ---------- */
	while((dirp = readdir(dp)) != NULL)
	{
		if(strcmp(dirp->d_name,".") && strcmp(dirp->d_name,".."))
		{
			if(strstr(dirp->d_name,name) != NULL)
			{
				rtn = 1;
				break;
			}
		}
	}
	/* -------- 关闭目录 ---------- */
  closedir(dp);
  	
  return rtn;
}

/* ----------------------------------------------------- */
/*                  判断目录是否有文件      
返回：1 目录为空 ; 0  目录不为空    */
/* ----------------------------------------------------- */
int CommonFuncs::DirIsEmpty(char *dirname)
{
	int           rtn;
	struct dirent *dirp;
	DIR           *dp;
  
  	rtn = 1;
  	/* -------- 打开目录 ---------- */
	if((dp = opendir(dirname)) == NULL)
	{
		return 1;
	}
	/* -------- 读取目录 ---------- */
	while((dirp = readdir(dp)) != NULL)
	{
		if(strcmp(dirp->d_name,".") && strcmp(dirp->d_name,".."))
		{
			rtn = 0;
			break;
		}
	}
	/* -------- 关闭目录 ---------- */
  	closedir(dp);
  	
  	return rtn;
}

/* ----------------------------------------------------- */
/*                  获得文件夹的文件数                  */
/* ----------------------------------------------------- */
int CommonFuncs::CountOFDir(char *dirname)
{
	int           count;
	struct dirent *dirp;
	DIR           *dp;
  
  	count = 0;
  	/* -------- 打开目录 ---------- */
	if((dp = opendir(dirname)) == NULL)
	{
		return count;
	}
	/* -------- 读取目录 ---------- */
	while((dirp = readdir(dp)) != NULL)
	{
		if(strcmp(dirp->d_name,".")  == 0 || strcmp(dirp->d_name,"..") == 0)
			continue;
		else
			count++;

	}
	/* -------- 关闭目录 ---------- */
  	closedir(dp);
  	
  	return count;
}

//得到应用程序所在目录
int CommonFuncs::GetCfgFileName(char *pathname)  
{  
     
    //if(!getcwd(pathname, MAX_PATH))
    if(!getcwd(pathname, 256)) 
    {  
        return 0;  
    } 
    if(pathname[strlen(pathname) - 1] != '/')
		{
			strcat(pathname,"/");
		} 
    return 1; 
}

/* ----------------------------------------------------- */
/* 			将16进制数组转化为对应的16进制字符串	           */
/* ----------------------------------------------------- */
void CommonFuncs::Conver_Byte(BYTE *b_source,char *c_dest,int len)
{
	int  i;
	char tmp_CARD_ID[30];
	
	memset(tmp_CARD_ID,0x00,sizeof(tmp_CARD_ID));
	for(i = 0;i < len;i++)
		sprintf(tmp_CARD_ID+2*i,"%02X",b_source[i]);
	memcpy(c_dest,tmp_CARD_ID,len*2);
}

/* ----------------------------------------------------- */
/* 			转换高低字节	          													 */
/* ----------------------------------------------------- */
void CommonFuncs::Conver_H_L_Byte(BYTE *b_source,char *c_dest,int len)
{
	int  i;
	char tmp_CARD_ID[30];
	
	memset(tmp_CARD_ID,0x00,sizeof(tmp_CARD_ID));
	for(i = 0;i < len;i++)
		sprintf(tmp_CARD_ID+2*i,"%02X",b_source[i]);
	memcpy(c_dest,tmp_CARD_ID+len,len);
	memcpy(c_dest+len,tmp_CARD_ID,len);
}

/* ----------------------------------------------------- */
/* 			判断字符串是否是数字字符串，返回1说明是数字字符串，0不是 */
/* ----------------------------------------------------- */
int CommonFuncs::String_Isdigit(unsigned char *p,int len)
{
	int i,flag;
	
	flag = 1;
	for(i=0;i<len;i++)
	{
		if(p[i] != '0' && p[i] != '1' && p[i] != '2' && p[i] != '3' && p[i] != '4' &&
			 p[i] != '5' && p[i] != '6' && p[i] != '7' && p[i] != '8' && p[i] != '9' &&
			 p[i] != 'a' && p[i] != 'b' && p[i] != 'c' && p[i] != 'd' && p[i] != 'e' && p[i] != 'f' &&
			 p[i] != 'A' && p[i] != 'B' && p[i] != 'C' && p[i] != 'D' && p[i] != 'E' && p[i] != 'F')
		{
			flag = 0;
			break;
		}
	}
	return flag;
}

/* ------------------------------------------------------
            功能描述:   查找字符串中的某一个字符
--------------------------------------------------------- */
int CommonFuncs::FindChar(char *p,const char c)
{
	int i;

	for(i = 0;i < strlen(p); i++)
	{
		if(p[i] == c)
		{
			break;
		}
	}

	return i;
}

/* ------------------------------------------------------
    功能描述:  判断磁盘空间使用量是否超过输入百分比
    输入参数： fullPercen:满百分比(0~100);超过该范围默认是90
    					 diskMessage:存储磁盘信息
    返回值:		 超过满百分比返回true,否则返回false;    					 
--------------------------------------------------------- */
bool CommonFuncs::DiskIsFull(int fullPercent,char *diskMessage)
{
		struct statfs diskInfo;
		statfs("/", &diskInfo);
		unsigned long long totalBlocks = diskInfo.f_bsize;
		unsigned long long totalSize = totalBlocks * diskInfo.f_blocks;
		size_t mbTotalsize = totalSize>>20;
		unsigned long long freeDisk = diskInfo.f_bfree*totalBlocks;
		size_t mbFreedisk = freeDisk>>20;
		
		if(fullPercent <=0 || fullPercent >= 100)
		{
			fullPercent = 90;
		}		
		memset(diskMessage,0x00,sizeof(diskMessage));
		if(mbTotalsize * ( 100 - fullPercent )/100 >= mbFreedisk)
		{
			sprintf (diskMessage,"/  total=%dMB, free=%dMB 服务器磁盘空间使用量已超过%d%%\n", mbTotalsize, mbFreedisk,fullPercent);				
			return true;
		}
		else
		{
			sprintf (diskMessage,"/  total=%dMB, free=%dMB 服务器磁盘空间使用量未超过%d%%\n", mbTotalsize, mbFreedisk,fullPercent);				
			return false;
		}
}

  

/* ----------------------------------------------------- */
/* 			格式化目录(前后加上'/')	        								 */
/* ----------------------------------------------------- */
void CommonFuncs::FormatDir(char *c_source)
{
	if(strlen(c_source)<1) return;
	int len = strlen(c_source);
			
	char tmp[len + 10];	
	memset(tmp,0x00,sizeof(tmp));
	if(c_source[0] != '/')
	{
		//	sprintf(tmp,"/%s",c_source);
		sprintf(tmp,"%s",c_source);
	}
	else
	{
			sprintf(tmp,"%s",c_source);
	}	
	
	if(c_source[len - 1] != '/')
	{
			strcat(tmp,"/");	
	}	
	memset(c_source,0x00,sizeof(c_source));
	strcpy(c_source,tmp);
}





/* ----------------------------------------------------- */
/* -------------16进制转10进制---0x20转化0d20------------*/ 
/*   ----------必须输入最好为0x2222等，不含有字符-------------*/      								 
/* ----------------------------------------------------- */
int CommonFuncs::BcdToDec(int bcdInt)
{
	int decInt = 0;
	char *stopstring;
	char  c_temp[9];
	memset(c_temp,0x00,sizeof(c_temp));
	sprintf(c_temp,"%08X",bcdInt);
	decInt = strtoul(c_temp,&stopstring,10);	
	return decInt;
}

 /* ----------------------------------------------------- */
/* -------------10进制转16进制---0d20转化0x20------------*/   								 
/* ----------------------------------------------------- */
int CommonFuncs::DecToBcd(int decInt)
{
	int hexInt = 0;
	char *stopstring;
	char  c_temp[9];
	memset(c_temp,0x00,sizeof(c_temp));
	sprintf(c_temp,"%08d",decInt);
	hexInt = strtoul(c_temp,&stopstring,16);	
	return hexInt;
}
 
/* -----------------------------------------------------
函数功能：删除指定目录中，含有指定字符串的文件  								 
----------------------------------------------------- */
void CommonFuncs::DeleteFile(char *dir,char *subStr)
{
	struct dirent *dirp;
	DIR           *dp;
	char           filename[256];
 
	/* --- 打开目录 --- */
	if((dp = opendir(dir)) == NULL)
		return;
	/* --- 读取目录 --- */
	while((dirp = readdir(dp)) != NULL)
	{
		if(strcmp(dirp->d_name,".") == 0 ||
			strcmp(dirp->d_name,"..") == 0)
			continue;
		if(strstr(dirp->d_name,subStr) != NULL)
		{
			memset(filename,0x00,sizeof(filename));
			sprintf(filename,"%s%s",dir,dirp->d_name);
			remove(filename);
		}
	}
	/* -------- 关闭目录 ---------- */
	closedir(dp);
}  
  
 
/* -----------------------------------------------------
函数功能：删除指定目录中的所有文件  								 
----------------------------------------------------- */
void CommonFuncs::EmptyDirFile(char *dir)
{
	struct dirent *dirp;
	DIR           *dp;
	char           filename[256];
 
	/* --- 打开目录 --- */
	if((dp = opendir(dir)) == NULL)
		return;
	/* --- 读取目录 --- */
	while((dirp = readdir(dp)) != NULL)
	{
		if(strcmp(dirp->d_name,".") == 0 ||
			strcmp(dirp->d_name,"..") == 0)
			continue;
		
		/*-------删除文件----------*/
		memset(filename,0x00,sizeof(filename));
		sprintf(filename,"%s%s",dir,dirp->d_name);
		remove(filename);
	}
	/* -------- 关闭目录 ---------- */
	closedir(dp);
}  
    
  
  
  
/* ------------------------------------------------------
           功能描述:   时间格式转换
--------------------------------------------------------- */
time_t CTime::GetGmtTime(const char *c_time)
{
    struct tm t;
    time_t gmt;
    char   temp[10];
    int	   year,month,day,hour,min,sec;

    memset(temp,0x00,sizeof(temp));
    memcpy(temp,c_time,4);
    year = atoi(temp);
    memset(temp,0x00,sizeof(temp));
    memcpy(temp,c_time+4,2);
    month = atoi(temp);
    memset(temp,0x00,sizeof(temp));
    memcpy(temp,c_time+6,2);
    day = atoi(temp);
    memset(temp,0x00,sizeof(temp));
    memcpy(temp,c_time+8,2);
    hour = atoi(temp);
    memset(temp,0x00,sizeof(temp));
    memcpy(temp,c_time+10,2);
    min = atoi(temp);
    memset(temp,0x00,sizeof(temp));
    memcpy(temp,c_time+12,2);
    sec = atoi(temp);

    t.tm_year = year - 1900;
    t.tm_mon  = month - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min  = min;
    t.tm_sec  = sec;
    gmt  = mktime(&t);

    return gmt;
}



/* ----------------------------------------------------- */
/*         获得本地时间:yyyyMMddHHmmss                   */
/* ----------------------------------------------------- */
void CTime::GetStringTime(char *buffer)
{
    time_t    long_time;
    struct tm newtime;

    time(&long_time);
    memset(&newtime,0x00,sizeof(struct tm));
    localtime_r(&long_time,&newtime);

    sprintf(buffer,"%04d%02d%02d%02d%02d%02d",(newtime.tm_year+1900),(newtime.tm_mon+1),newtime.tm_mday,
            newtime.tm_hour,newtime.tm_min,newtime.tm_sec);
}

/* ---------------------------------------------------------------------- */
/* 获得本地时间:yyyy-MM-dd HH:mm:ss 或者 yyyy-MM-dd+HH:mm:ss              */
/* ---------------------------------------------------------------------- */
void CTime::GetStringTime(char *buffer,bool isSpace)
{
    time_t    long_time;
    struct tm newtime;

    time(&long_time);
    memset(&newtime,0x00,sizeof(struct tm));
    localtime_r(&long_time,&newtime);
		
		if(isSpace)
		{
	    sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d",(newtime.tm_year+1900),(newtime.tm_mon+1),newtime.tm_mday,
      newtime.tm_hour,newtime.tm_min,newtime.tm_sec);
		}
		else
		{
	    sprintf(buffer,"%04d-%02d-%02d+%02d:%02d:%02d",(newtime.tm_year+1900),(newtime.tm_mon+1),newtime.tm_mday,
      newtime.tm_hour,newtime.tm_min,newtime.tm_sec);				
		}
}



/* ------------------------------------------------------
        功能描述:   把以秒为单位时间转换成字符串时间
--------------------------------------------------------- */
void CTime::DateToChar(unsigned int validdatetime, char *curtime)
{
    struct tm newtime;
    time_t longtime;

    memset(curtime,0x00,sizeof(curtime));
    longtime = (time_t)validdatetime;
    memset(&newtime,0x00,sizeof(struct tm));
    localtime_r(&longtime,&newtime);
    sprintf(curtime,"%04d%02d%02d %02d:%02d:%02d",(newtime.tm_year+1900),(newtime.tm_mon+1),newtime.tm_mday,
            newtime.tm_hour,newtime.tm_min,newtime.tm_sec);
}

/* ------------------------------------------------------
        功能描述:   把以秒为单位时间转换成字符串时间
--------------------------------------------------------- */
void CTime::DateToChar2(unsigned int validdatetime, char *curtime)
{
    struct tm newtime;
    time_t longtime;

    memset(curtime,0x00,sizeof(curtime));
    longtime = (time_t)validdatetime;
    memset(&newtime,0x00,sizeof(struct tm));
    localtime_r(&longtime,&newtime);
    sprintf(curtime,"%04d%02d%02d%02d%02d%02d",(newtime.tm_year+1900),(newtime.tm_mon+1),newtime.tm_mday,
            newtime.tm_hour,newtime.tm_min,newtime.tm_sec);
}

/* ----------------------------------------------------- */
/*                       获得本地时间                    */
/* ----------------------------------------------------- */
void CTime::GetLocalDateTime(char *buffer)
{
    time_t    long_time;
    struct tm newtime;

    time(&long_time);
    memset(&newtime,0x00,sizeof(struct tm));
    localtime_r(&long_time,&newtime);

    sprintf(buffer,"%04d%02d%02d%02d%02d%02d",(newtime.tm_year+1900),(newtime.tm_mon+1),newtime.tm_mday,
            newtime.tm_hour,newtime.tm_min,newtime.tm_sec);
}


/* ------------------------------------------------------
            功能描述:   时间格式转换
--------------------------------------------------------- */
time_t CTime::GetGmtTime(int year,int month,int day,int hour,int minute,int second)
{
    struct tm t;
    time_t gmt;

    t.tm_year = year - 1900;
    t.tm_mon  = month - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min  = minute;
    t.tm_sec  = second;
    gmt  = mktime(&t);

    return gmt;
}

/* ----------------------------------------------------- */
/*                       获得本地时间                    */
/* ----------------------------------------------------- */
void CTime::GetLocalTime(char *buffer)
{
    time_t    long_time;
    struct tm newtime;
    struct timeval t_msec;

    /* --- 毫秒 --- */
    gettimeofday(&t_msec,NULL);
    time(&long_time);
    memset(&newtime,0x00,sizeof(struct tm));
    localtime_r(&long_time,&newtime);

    sprintf(buffer,"%02d:%02d:%02d:%03d %04d年%02d月%02d日\t",newtime.tm_hour,newtime.tm_min,newtime.tm_sec,t_msec.tv_usec/1000,
            (newtime.tm_year+1900),(newtime.tm_mon+1),newtime.tm_mday);
}

/* ------------------------------------------------------
   功能描述:  输入年月日,返回到1970年1月1日的天数 
--------------------------------------------------------- */
int CTime::DateToDate2_t(int wYear,int wMonth,int wDay)
{
	int i = 0, dwDays = 0;

	for (i = 1970; i < wYear; i++)
	{
		if (((i % 4) == 0 && (i % 100) != 0) || (i % 400) == 0)
		{
			dwDays += 366;	//闰年
		}
		else
		{
			dwDays += 365;	//平年
		}
	}

	for (i = 1; i < wMonth; i++)
	{
		switch (i)
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			dwDays += 31;
			break;

		case 2:
			if (((wYear % 4) == 0 && (wYear % 100) != 0) || (wYear % 400) == 0)
			{
				dwDays += 29;	//闰年
			}
			else
			{
				dwDays += 28;	//平年
			}

			break;

		case 4:
		case 6:
		case 9:
		case 11:
			dwDays += 30;
			break;

		default:
			break;
		}
	}

	dwDays += wDay;

	return dwDays - 1;
}


void CTime::Date2_tToDate(int dwDayIdx,char *c_date)
{
	int tm_year,tm_mon,tm_mday;
	int dwDays = dwDayIdx + 1;

	tm_year = 1970;
	while (dwDays > 0)
	{
		if (((tm_year % 4) == 0 && (tm_year % 100) != 0) || (tm_year % 400) == 0)
		{
			//闰年
			if (dwDays > 366)
			{
				dwDays -= 366;
				tm_year++;
			}
			else
				break;
		}
		else
		{
			//平年
			if (dwDays > 365)
			{
				dwDays -= 365;
				tm_year++;
			}
			else
				break;
		}
	}

	tm_mon = 1;
	while (dwDays > 0)
	{
		if (tm_mon == 1 || tm_mon == 3 || tm_mon == 5 || tm_mon == 7 || tm_mon == 8 || tm_mon == 10 || tm_mon == 12)
		{
			if (dwDays > 31)
			{
				dwDays -= 31;
				tm_mon++;
			}
			else
				break;
		}
		else if (tm_mon == 2)
		{
			if (((tm_year % 4) == 0 && (tm_year % 100) != 0) || (tm_year % 400) == 0)
			{
				//闰年
				if (dwDays > 29)
				{
					dwDays -= 29;
					tm_mon++;
				}
				else
					break;
			}
			else
			{
				//平年
				if (dwDays > 28)
				{
					dwDays -= 28;
					tm_mon++;
				}
				else
					break;
			}
		}
		else if (tm_mon == 4 || tm_mon == 6 || tm_mon == 9 || tm_mon == 11)
		{
			if (dwDays > 30)
			{
				dwDays -= 30;
				tm_mon++;
			}
			else
				break;
		}
		else
		{
			;
		}
	}

	tm_mday = dwDays;
	
	sprintf(c_date,"%04d%02d%02d",tm_year,tm_mon,tm_mday);
}








/* ------------------------------------------------------
   功能描述:  返回当前时间到1970年1月1日的天数 
--------------------------------------------------------- */
int CTime::Days19700101ToNow()
{
	time_t          local_time;
	struct tm       newtime;	
	int             wYear,wMonth,wDay,wHour;
	
	/* --- 系统时间 --- */
	time(&local_time);
	memset(&newtime,0x00,sizeof(struct tm));
	localtime_r(&local_time,&newtime);
	wYear 	= newtime.tm_year + 1900;
	wMonth 	= newtime.tm_mon + 1;
	wDay 		= newtime.tm_mday;
	wHour		= newtime.tm_hour;
 
	int i = 0, dwDays = 0;

	for (i = 1970; i < wYear; i++)
	{
		if (((i % 4) == 0 && (i % 100) != 0) || (i % 400) == 0)
		{
			dwDays += 366;	//闰年
		}
		else
		{
			dwDays += 365;	//平年
		}
	}

	for (i = 1; i < wMonth; i++)
	{
		switch (i)
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			dwDays += 31;
			break;

		case 2:
			if (((wYear % 4) == 0 && (wYear % 100) != 0) || (wYear % 400) == 0)
			{
				dwDays += 29;	//闰年
			}
			else
			{
				dwDays += 28;	//平年
			}

			break;

		case 4:
		case 6:
		case 9:
		case 11:
			dwDays += 30;
			break;

		default:
			break;
		}
	}

	dwDays += wDay;

	if(wHour < 2) //两点整开始日切
	{
		dwDays--;
	}
	
	return dwDays - 1;
}

/* ------------------------------------------------------
   功能描述:  返回输入秒数到1970年1月1日的天数 
--------------------------------------------------------- */
int CTime::Days19700101ToNow(int secondsFrom1970)
{
	struct tm       newtime;	
	time_t 					longtime;
	int             wYear,wMonth,wDay,wHour;

	/* --- 秒数赋值 --- */	
	longtime = (time_t)secondsFrom1970;
 
	memset(&newtime,0x00,sizeof(struct tm));
	localtime_r(&longtime,&newtime);
	wYear 	= newtime.tm_year + 1900;
	wMonth 	= newtime.tm_mon + 1;
	wDay 		= newtime.tm_mday;
	wHour		= newtime.tm_hour;
 
	int i = 0, dwDays = 0;

	for (i = 1970; i < wYear; i++)
	{
		if (((i % 4) == 0 && (i % 100) != 0) || (i % 400) == 0)
		{
			dwDays += 366;	//闰年
		}
		else
		{
			dwDays += 365;	//平年
		}
	}

	for (i = 1; i < wMonth; i++)
	{
		switch (i)
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			dwDays += 31;
			break;

		case 2:
			if (((wYear % 4) == 0 && (wYear % 100) != 0) || (wYear % 400) == 0)
			{
				dwDays += 29;	//闰年
			}
			else
			{
				dwDays += 28;	//平年
			}

			break;

		case 4:
		case 6:
		case 9:
		case 11:
			dwDays += 30;
			break;

		default:
			break;
		}
	}

	dwDays += wDay;

	if(wHour < 2) //两点整开始日切
	{
		dwDays--;
	}
	
	return dwDays - 1;   
};



 


int CTime::Seconds1970ToNow()
{
	struct timeb tp;
	ftime(&tp);    
  return tp.time;
}

int CTime::CheckTimeFormat(char *c_time)
{
	int    year,month,day,hour,minute,second;
	char   temp[5];
	int    rtn;
	
	rtn = 1;
		
	memset(temp,0x00,sizeof(temp));
	memcpy(temp,c_time,4);
	year = atoi(temp);
	memset(temp,0x00,sizeof(temp));
	memcpy(temp,c_time+4,2);
	month = atoi(temp);
	memset(temp,0x00,sizeof(temp));
	memcpy(temp,c_time+6,2);
	day = atoi(temp);
	memset(temp,0x00,sizeof(temp));
	memcpy(temp,c_time+8,2);
	hour = atoi(temp);
	memset(temp,0x00,sizeof(temp));
	memcpy(temp,c_time+10,2);
	minute = atoi(temp);
	memset(temp,0x00,sizeof(temp));
	memcpy(temp,c_time+12,2);
	second = atoi(temp);
	if((year >= 1 && year <= 2099) && (month  >= 1 && month  <= 12) && (day    >= 1 && day    <= 31) &&
		(hour  >= 0 && hour <= 23)   && (minute >= 0 && minute <= 59) && (second >= 0 && second <= 59))
	{
		;
	}
	else
	{
		rtn = 0;
		strcpy(c_time,"19700101000000");
	}
	return rtn;
}







//          CLog 静态变量定义
FILE*    CLog::g_AppLog_fp;
FILE*    CLog::g_CommLog_fp;
char    CLog::g_AppLogPath[MAX_PATH];
sem_t  	CLog::g_SemWriteAppLog;
sem_t*  CLog::g_SemWriteCommLog = (sem_t*) calloc (1,sizeof(sem_t));  /* ------  同步写日志的信号量------------- */

int CLog::InitLogStaticVar(char *appLogPath)
{
    memset(g_AppLogPath, 0x00, sizeof(g_AppLogPath));
    strcpy(g_AppLogPath,appLogPath);

    time_t       long_time;
    struct tm    newtime;
    char         filename[MAX_PATH];
    time(&long_time);
    memset(&newtime,0x00,sizeof(struct tm));
    localtime_r(&long_time,&newtime);

    memset(filename,0x00,sizeof(filename));
    sprintf(filename,"%s/%02d/%02d_%02d_AppLog.txt",g_AppLogPath,newtime.tm_mday,newtime.tm_mon+1,newtime.tm_mday);
 
    g_AppLog_fp = NULL;
    g_AppLog_fp = fopen(filename,"a+");
    if(g_AppLog_fp == NULL)
    {
        return 0;
    }

    memset(filename,0x00,sizeof(filename));
    sprintf(filename,"%s/%02d/%02d_%02d_CommunicationLog.txt",g_AppLogPath,newtime.tm_mday,newtime.tm_mon+1,newtime.tm_mday);
    g_CommLog_fp = NULL;
    g_CommLog_fp = fopen(filename,"a+");
    if(g_CommLog_fp == NULL)
    {
        return 0;
    }

    sem_init(g_SemWriteCommLog,0,1);
    sem_init(&g_SemWriteAppLog,0,1);

    return  1;
}


void CLog::ReleaseLogStaticVar()
{
    sem_wait(&g_SemWriteAppLog);
    fclose(g_CommLog_fp);
    g_CommLog_fp = NULL;
    sem_post(&g_SemWriteAppLog);

    sem_wait(&g_SemWriteAppLog);
    fclose(g_AppLog_fp);
    g_AppLog_fp = NULL;
    sem_post(&g_SemWriteAppLog);

    sem_destroy(g_SemWriteCommLog);
    free(g_SemWriteCommLog); /* --- 释放信号量(同步写操作日志) -------- */
    g_SemWriteCommLog = NULL;
 
}

/* ----------------------------------------------------- */
/*                       获得本地时间                    */
/* ----------------------------------------------------- */
void CLog::GetLocalTime(char *buffer)
{
    time_t    long_time;
    struct tm newtime;
    struct timeval t_msec;

    /* --- 毫秒 --- */
    gettimeofday(&t_msec,NULL);
    time(&long_time);
    memset(&newtime,0x00,sizeof(struct tm));
    localtime_r(&long_time,&newtime);
    sprintf(buffer,"%02d:%02d:%02d:%03d %04d年%02d月%02d日\t",newtime.tm_hour,newtime.tm_min,newtime.tm_sec,t_msec.tv_usec/1000,
            (newtime.tm_year+1900),(newtime.tm_mon+1),newtime.tm_mday);
   
}


/* ----------------------------------------------------- */
/*                       写日志文件                      */
/* ----------------------------------------------------- */
void CLog::WriteAppLog(const char* buffer)
{
    char	      errmsg[1024],filename[MAX_PATH];
    int         rtn;
    struct stat buf;

    memset(errmsg,0x00,sizeof(errmsg));
    GetLocalTime(errmsg);
    strcat(errmsg,"\t");
    strcat(errmsg,buffer);

    /* --- 获得文件名称 --- */
    time_t       long_time;
    struct tm    newtime;
    time(&long_time);
    memset(&newtime,0x00,sizeof(struct tm));
    localtime_r(&long_time,&newtime);
    memset(filename,0x00,sizeof(filename));
    sprintf(filename,"%s/%02d/%02d_%02d_AppLog.txt",g_AppLogPath,newtime.tm_mday,newtime.tm_mon+1,newtime.tm_mday);

    sem_wait(&g_SemWriteAppLog);      /* --- 获得信号量 --- */
    rtn = stat(filename, &buf );
    if(rtn != 0)
    {
    		if(g_AppLog_fp != NULL)
    		{
        	fclose(g_AppLog_fp);
    		}

        g_AppLog_fp = NULL;
        g_AppLog_fp = fopen(filename,"a+");
    }
    if(g_AppLog_fp != NULL)
    {
        fprintf(g_AppLog_fp,errmsg);
        fflush(g_AppLog_fp);
    }
    else
    {
        g_AppLog_fp = fopen(filename,"a+");
        fprintf(g_AppLog_fp,errmsg);
        fflush(g_AppLog_fp);
    }

    sem_post(&g_SemWriteAppLog);      /* --- 释放信号量 --- */
}

/* -----------------------------------------------------
参数说明：
				flag:0 写入applog;1 写入 commlog;
				buffer:报文缓冲区
				buflen:报文长度
   ----------------------------------------------------- */
void CLog::WriteDebugLog(int flag,unsigned char *buffer,int buflen)
{
	char info[100],*p;
	int  i;
	
	memset(info,0x00,sizeof(info));
	p = info;
	for(i=0;i<buflen;i++)
	{
		sprintf(p,"%02X ",buffer[i]);
		p += 3;
		if((i+1)%16==0)
		{
			strcat(info,"\n");
			if(flag == 0)
				WriteAppLog(info);
			else
				WriteCommuLog(info);
			memset(info,0x00,sizeof(info));
			p = info;
		}
	}
	if(buflen % 16 != 0)
		strcat(info,"\n");
	if(strlen(info) > 0)
	{
		if(flag == 0)
			WriteAppLog(info);
		else
			WriteCommuLog(info);
	}
}

 
/* ----------------------------------------------------- */
/*                       写日志文件                      */
/* ----------------------------------------------------- */
void CLog::WriteCommuLog(char *buffer)
{
    char	      errmsg[1024],filename[MAX_PATH];
    int         rtn;
    struct stat buf;

    memset(errmsg,0x00,sizeof(errmsg));
    GetLocalTime(errmsg);
    strcat(errmsg,buffer);
    /* --- 获得文件名称 --- */
    time_t       long_time;
    struct tm    newtime;
    time(&long_time);
    memset(&newtime,0x00,sizeof(struct tm));
    localtime_r(&long_time,&newtime);
    memset(filename,0x00,sizeof(filename));
    sprintf(filename,"%s/%02d/%02d_%02d_CommunicationLog.txt",g_AppLogPath,newtime.tm_mday,newtime.tm_mon+1,newtime.tm_mday);

    sem_wait(g_SemWriteCommLog);      /* --- 获得信号量 --- */
    rtn = stat(filename, &buf );
    if(rtn != 0)
    {
    	  if(g_CommLog_fp != NULL)
    		{
        	fclose(g_CommLog_fp);
    		}
    		
        g_CommLog_fp = NULL;
        g_CommLog_fp = fopen(filename,"a+");
    }
    if(g_CommLog_fp != NULL)
    {
        fprintf(g_CommLog_fp,errmsg);
        fflush(g_CommLog_fp);
    }
    else
    {
        g_CommLog_fp = fopen(filename,"a+");
        fprintf(g_CommLog_fp,errmsg);
        fflush(g_CommLog_fp);
    }
    sem_post(g_SemWriteCommLog);      /* --- 释放信号量 --- */
}





CMyJson::CMyJson()
{
	
}

CMyJson::CMyJson(char *ptext)
{
	m_cjson = NULL;
	m_cjson = cJSON_Parse(ptext);	
	if(m_cjson == NULL)
	{
			m_isFailed = true;
	}
	else
	{
			m_isFailed = false;
	}
	m_isUtf8Flag = true;
}

CMyJson::CMyJson(char *ptext,bool isUtf8Flag)
{
	m_cjson = NULL;
	m_cjson = cJSON_Parse(ptext);	
	if(m_cjson == NULL)
	{
			m_isFailed = true;
	}
	else
	{
			m_isFailed = false;
	}
	m_isUtf8Flag = isUtf8Flag;
}

CMyJson::~CMyJson()
{
	//printf("~CMyJson ~CMyJson ~CMyJson \n");
	if(m_cjson != NULL)
	{
		cJSON_Delete(m_cjson);
		m_cjson = NULL;				
		//printf("~CMyJson ~CMyJson ~CMyJson m_cjson != NULL \n");
	}
}

cJSON* 	CMyJson::GetObjectItem(const char *pitem)
{
	 return cJSON_GetObjectItem(m_cjson,pitem);
}	

void CMyJson::GetString(const char *pitem,char *pvalue)
{
  cJSON  *item = NULL;
  char   info[MAXBUFFERLEN];
	
	if(m_cjson == NULL)
		return ;
	
	/* --- 解析Json --- */
	item = cJSON_GetObjectItem(m_cjson, pitem);
	if(item == NULL)
		return;
	/* --- 输出pitem --- */
	memset(info,0x00,sizeof(info));
	if(m_isUtf8Flag)
	{
		CommonFuncs::utf82gbk(item->valuestring,strlen(item->valuestring),info,sizeof(info));
		strcpy(pvalue,info);
	}
	else
	{
		strcpy(pvalue,item->valuestring);
	}
}

int 	CMyJson::GetInt(const char *pitem)
{
  cJSON  *item = NULL;
  int    i_value;
  
	if(m_cjson == NULL)
		return 0;
		
	/* --- 解析Json --- */	
	item = cJSON_GetObjectItem(m_cjson, pitem);
	if(item == NULL)
		return 0;
	i_value = item->valueint;
 
	return i_value;
}	

void CMyJson::GetJsonPrintText(char *jsonText)
{
	char  *pOut = NULL;
	char   pLog[MAXBUFFERLEN];
	
	if(m_cjson == NULL)
	{
			return;
	}
	
	/* --- 解析Json --- */
	pOut = cJSON_Print(m_cjson);
	/* --- 输出Json --- */
	memset(jsonText,0x00,sizeof(jsonText));
	if(m_isUtf8Flag)
	{
		CommonFuncs::utf82gbk(pOut,strlen(pOut),pLog,sizeof(pLog));
		strcpy(jsonText,pLog);
	}
	else
	{
		strcpy(jsonText,pOut);
	}

	free(pOut);
	pOut = NULL;
}  
 
int 		CMyJson::GetArraySize(const char *pitem)
{
	cJSON  *item = NULL;
  int    i_value;
	
	/* --- 解析Json --- */
	item = cJSON_GetObjectItem(m_cjson, pitem);
	if(item == NULL)
		return 0;
	i_value = cJSON_GetArraySize(item);
 
	return i_value;
}

int 		CMyJson::GetIntArray(const char *pitem,int* intArray)
{
	cJSON *cjsonTmp = NULL;
	cJSON *cjsonArrayTmp = NULL;
	int arrayLenth = 0;
	cjsonArrayTmp = cJSON_GetObjectItem(m_cjson, pitem);
	if(cjsonArrayTmp == NULL)
	{
			return 0;
	}
		
	arrayLenth = cJSON_GetArraySize(cjsonArrayTmp);
	if(arrayLenth > 0)
	{
		for(int i=0;i<arrayLenth;i++)
		{
			cjsonTmp = cJSON_GetArrayItem(cjsonArrayTmp,i);
			intArray[i] = cjsonTmp->valueint;
		}
	}
	cjsonTmp = NULL;
	cjsonArrayTmp = NULL;
	return arrayLenth;
}


int 		CMyJson::GetStringArray(const char *pitem,char** stringArray)
{
	cJSON *cjsonTmp = NULL;
	cJSON *cjsonArrayTmp = NULL;
	int arrayLenth = 0;
	char   info[8192];
		 
	cjsonArrayTmp = cJSON_GetObjectItem(m_cjson, pitem);
	if(cjsonArrayTmp == NULL)
	{
			return 0;
	}
		
	arrayLenth = cJSON_GetArraySize(cjsonArrayTmp);
	if(arrayLenth > 0)
	{
		for(int i=0;i<arrayLenth;i++)
		{
			cjsonTmp = cJSON_GetArrayItem(cjsonArrayTmp,i);
			if(m_isUtf8Flag)
			{
				memset(info,0x00,sizeof(info));
				CommonFuncs::utf82gbk(cjsonTmp->valuestring,strlen(cjsonTmp->valuestring),info,sizeof(info));
				strcpy(stringArray[i],info);
			}
			else
			{
				strcpy(stringArray[i],cjsonTmp->valuestring);
			}	 
		}
	}
	cjsonTmp = NULL;
	cjsonArrayTmp = NULL;
	return arrayLenth;
}

int 		CMyJson::GetJsonStringArray(const char *pitem,char** stringArray)
{
	cJSON *cjsonTmp = NULL;
	cJSON *cjsonArrayTmp = NULL;
	int arrayLenth = 0;
	char   info[8192];
		 
	cjsonArrayTmp = cJSON_GetObjectItem(m_cjson, pitem);
	if(cjsonArrayTmp == NULL)
	{
			return 0;
	}
		
	arrayLenth = cJSON_GetArraySize(cjsonArrayTmp);
	if(arrayLenth > 0)
	{
		for(int i=0;i<arrayLenth;i++)
		{
			cjsonTmp = cJSON_GetArrayItem(cjsonArrayTmp,i);
			
			memset(info,0x00,sizeof(info));
			CMyJson::cJson2Text(cjsonTmp,info);	
			strcpy(stringArray[i],info);			
		}
	}
	cjsonTmp = NULL;
	cjsonArrayTmp = NULL;
	return arrayLenth;
}




 
 


	
cJSON* 	CMyJson::GetArrayItem(cJSON *cjson,int index)
{
	return cJSON_GetArrayItem(cjson,index);
}

int 		CMyJson::GetArraySize(cJSON *cjson)
{
	return cJSON_GetArraySize(cjson);
}
	
int 		CMyJson::GetIntArray(cJSON *cjson,int* intArray)
{
	cJSON *cjsonTmp = NULL;
	int arrayLenth = 0;
	arrayLenth = cJSON_GetArraySize(cjson);
	if(arrayLenth > 0)
	{
		for(int i=0;i<arrayLenth;i++)
		{
			cjsonTmp = cJSON_GetArrayItem(cjson,i);
			intArray[i] = cjsonTmp->valueint;
		}
	}
	cjsonTmp = NULL;
	return arrayLenth;
}

int 		CMyJson::GetStringArray(cJSON *cjson,char** stringArray)
{
	bool m_isUtf8Flag = true;
	cJSON *cjsonTmp = NULL;
	int arrayLenth = 0;
	 char   info[8192];
	arrayLenth = cJSON_GetArraySize(cjson);
	if(arrayLenth > 0)
	{
		for(int i=0;i<arrayLenth;i++)
		{
			cjsonTmp = cJSON_GetArrayItem(cjson,i);
			if(m_isUtf8Flag)
			{
				CommonFuncs::utf82gbk(cjsonTmp->valuestring,strlen(cjsonTmp->valuestring),info,sizeof(info));
				strcpy(stringArray[i],info);
			}
			else
			{
				strcpy(stringArray[i],cjsonTmp->valuestring);
			}	 
		}
	}
	cjsonTmp = NULL;
	return arrayLenth;
}
	
int 		CMyJson::GetJsonStringArray(cJSON *cjson,const char *pitem,char** stringArray)
{
	cJSON *cjsonTmp = NULL;
	cJSON *cjsonArrayTmp = NULL;
	int arrayLenth = 0;
	char   info[8192];
		 
	cjsonArrayTmp = cJSON_GetObjectItem(cjson, pitem);
	if(cjsonArrayTmp == NULL)
	{
			return 0;
	}
		
	arrayLenth = cJSON_GetArraySize(cjsonArrayTmp);
	if(arrayLenth > 0)
	{
		for(int i=0;i<arrayLenth;i++)
		{
			cjsonTmp = cJSON_GetArrayItem(cjsonArrayTmp,i);
			
			memset(info,0x00,sizeof(info));
			CMyJson::cJson2Text(cjsonTmp,info);	
			strcpy(stringArray[i],info);			
		}
	}
	cjsonTmp = NULL;
	cjsonArrayTmp = NULL;
	return arrayLenth;
}	
		
void CMyJson::GetString(char *ptext,const char *pitem,char *pvalue)
{
	cJSON  *root = NULL;
  cJSON  *item = NULL;
  char   info[MAXBUFFERLEN];
	
	/* --- 解析Json --- */
	root = cJSON_Parse(ptext);
	item = cJSON_GetObjectItem(root, pitem);
	if(item == NULL)
		return;
	/* --- 输出pitem --- */
	memset(info,0x00,sizeof(info));
	CommonFuncs::utf82gbk(item->valuestring,strlen(item->valuestring),info,sizeof(info));
	strcpy(pvalue,info);
	/* --- 内存释放 --- */
	cJSON_Delete(root);
}

int 	CMyJson::GetInt(char *ptext,const char *pitem)
{
	cJSON  *root = NULL;
  cJSON  *item = NULL;
  int    i_value;
	
	/* --- 解析Json --- */
	root = cJSON_Parse(ptext);
	item = cJSON_GetObjectItem(root, pitem);
	if(item == NULL)
		return 0;
	i_value = item->valueint;
	/* --- 内存释放 --- */
	cJSON_Delete(root);
	
	return i_value;
}	

void CMyJson::GetChildNodeString(char *ptext,const char *pitem1,const char *pitem2,char *pvalue)
{
	cJSON  *root = NULL;
  cJSON  *item = NULL;
  char   info[MAXBUFFERLEN];
	
	/* --- 解析Json --- */
	root = cJSON_Parse(ptext);
	item = cJSON_GetObjectItem(root, pitem1);
	if(item == NULL)
		return;
	item = cJSON_GetObjectItem(item, pitem2);
	if(item == NULL)
		return;
	/* --- 输出pitem --- */
	memset(info,0x00,sizeof(info));
	CommonFuncs::utf82gbk(item->valuestring,strlen(item->valuestring),info,sizeof(info));
	strcpy(pvalue,info);
	/* --- 内存释放 --- */
	cJSON_Delete(root);
}

int 	CMyJson::GetChildNodeInt(char *ptext,const char *pitem1,const char *pitem2)
{
	cJSON  *root = NULL;
  cJSON  *item = NULL;
  int    i_value;
	
	/* --- 解析Json --- */
	root = cJSON_Parse(ptext);
	item = cJSON_GetObjectItem(root, pitem1);
	if(item == NULL)
		return -1;
	item = cJSON_GetObjectItem(item, pitem2);
	if(item == NULL)
		return -1;
	/* --- 输出pitem --- */
	i_value = item->valueint;
	/* --- 内存释放 --- */
	cJSON_Delete(root);
	
	return i_value;
}
 
void CMyJson::GetJsonPrintText(char *ptext,char *jsonText)
{
	char  *pOut = NULL;
	cJSON *root = NULL;
	char   pLog[MAXBUFFERLEN];
	
	/* --- 解析Json --- */
	root = cJSON_Parse(ptext);
	if(root != NULL)
	{
		pOut = cJSON_Print(root);
		/* --- 输出Json --- */
		memset(jsonText,0x00,sizeof(jsonText));
		CommonFuncs::utf82gbk(pOut,strlen(pOut),pLog,sizeof(pLog));
		strcpy(jsonText,pLog);
	}
	
	/* --- 内存释放 --- */
	cJSON_Delete(root);
	free(pOut);
	pOut = NULL;
} 

void CMyJson::cJson2Text(cJSON *cjson,char *jsonText)
{
	char  *pOut = NULL;

	/* --- 解析Json --- */
	if(cjson != NULL)
	{
		pOut = cJSON_Print(cjson);
		/* --- 输出Json --- */
		memset(jsonText,0x00,sizeof(jsonText));
		strcpy(jsonText,pOut);
	}
	
	/* --- 内存释放 --- */
	free(pOut);
	pOut = NULL;

} 

void CMyJson::GetDemo(char *cjsonText)
{	
	char jsonText[8192],pOut[8192],applog[8192];
	memset(jsonText,0x00,sizeof(jsonText));
	
	/*---------cjsonText 数据测试-------*/
	strcpy(jsonText,cjsonText);
	
	/*---------自定义 数据测试-------
	strcpy(jsonText, "{ \
\"stringSingle\": \"stringSingleTest\",\
\"intSingle\": 193,\
\"intArray\": [7, 8], \
\"stringArray\":[\"str1\",\"str2\"], \
\"jsonItemSingle\":{\"jsonItemSingleTestInt\":888,\"jsonItemSingleTestString\": \"jsonItemSingleTestString\"}, \
\"jsonItemArray\":[\
  {\"jsonItemArrayTestInt\":222},\
  {\"jsonItemArrayTestInt\":333},\
  {\"jsonItemArrayTestInt\":444},\
  {\"jsonItemArrayTestInt\":555}\
  ]\
}");
*/
	printf("----------GetDemo-----begin-----------\n");
	printf("\n----------jsonText-----\n");
	printf(jsonText);
	
	printf("\n----------处理jsonText-----begin----\n");	
	/* ---------原始 jsonText 生成CMyJson类 ----------------*/
	CMyJson CMyJsonRoot(jsonText,true); 
	if(CMyJsonRoot.m_isFailed)
	{
		printf("----------CMyJsonRoot CreateFailed ----\n");	
		return 	;
	}


	printf("\n----------GetJsonPrintText-----begin--\n");		
	memset(applog,0x00,sizeof(applog));
	CMyJsonRoot.GetJsonPrintText(applog)	;	
	printf(applog);
	printf("\n----------GetJsonPrintText-----end--\n");	
	
	printf("----------GetString-----begin--\n");	
	memset(pOut,0x00,sizeof(pOut));
	CMyJsonRoot.GetString("stringSingle",pOut);		
	
	memset(applog,0x00,sizeof(applog));
	sprintf(applog," GetString:%s\n",pOut);			 
	printf(applog);
	printf("----------GetString-----end--\n");	
	
	printf("----------GetInt-----begin--\n");		
	memset(applog,0x00,sizeof(applog));
	sprintf(applog," GetInt:%d\n",CMyJsonRoot.GetInt("intSingle"));			 
	printf(applog);
	printf("----------GetInt-----end--\n");	

	
	int  count = 0;
	count = CMyJsonRoot.GetArraySize("intArray"); 
	
	int intArray[count];
	printf("----------CMyJsonRoot.GetIntArray -----begin--\n");				
	CMyJsonRoot.GetIntArray("intArray",intArray);	
	for(int i = 0;i < count;i++) 
	{
		memset(applog,0x00,sizeof(applog));
		sprintf(applog,"arrayValue[%d] = %d \n",i,intArray[i]);			
		printf(applog);
	}
	printf("----------CMyJsonRoot.GetIntArray-----end--\n");	

	printf("----------CMyJson::GetIntArray -----begin--\n");				
	CMyJson::GetIntArray(CMyJsonRoot.GetObjectItem("intArray"),intArray);	
	for(int i = 0;i < count;i++) 
	{
		memset(applog,0x00,sizeof(applog));
		sprintf(applog,"arrayValue[%d] = %d \n",i,intArray[i]);			
		printf(applog);
	}
	printf("----------CMyJson::GetIntArray-----end--\n");	


	count = 0;
	count = CMyJsonRoot.GetArraySize("stringArray"); 
	//char* stringArray[count];
	char* stringArray[100];
	for(int i = 0;i <100;i++) 
	{
		stringArray[i] = new char[8*1024];
		memset(stringArray[i],0x00,sizeof(stringArray[i])); 
	}
	
	printf("----------CMyJsonRoot.GetStringArray -----begin--\n");				
	CMyJsonRoot.GetStringArray("stringArray",stringArray);	
	for(int i = 0;i < count;i++) 
	{
		memset(applog,0x00,sizeof(applog));
		sprintf(applog,"arrayValue[%d] = %s \n",i,stringArray[i]);			
		printf(applog);
	}
	printf("----------CMyJsonRoot.GetStringArray-----end--\n");	
	
	printf("----------CMyJson::GetStringArray -----begin--\n");	
	for(int i = 0;i <count;i++) 
	{
		memset(stringArray[i],0x00,sizeof(stringArray[i])); 
	}
	CMyJson::GetStringArray(CMyJsonRoot.GetObjectItem("stringArray"),stringArray);				
	for(int i = 0;i < count;i++) 
	{
		memset(applog,0x00,sizeof(applog));
		sprintf(applog,"arrayValue[%d] = %s \n",i,stringArray[i]);			
		printf(applog);
	}
	printf("----------CMyJson::GetStringArray-----end--\n");	
		
	printf("----------jsonItemArray -----begin--\n");	
	count = 0;
	count = CMyJsonRoot.GetArraySize("jsonItemArray"); 
	for(int i = 0;i <count;i++) 
	{
		memset(stringArray[i],0x00,sizeof(stringArray[i])); 
	}
	memset(applog,0x00,sizeof(applog));
	CMyJson::cJson2Text(CMyJsonRoot.GetObjectItem("jsonItemArray"),applog)	;	
	printf(applog);
	printf("\n\n\n");
	
	CMyJsonRoot.GetJsonStringArray("jsonItemArray",stringArray);	
	for(int i = 0;i < count;i++) 
	{
		memset(applog,0x00,sizeof(applog));
		sprintf(applog,"arrayValue[%d] = %s \n",i,stringArray[i]);			
		printf(applog);
		
		CMyJson CMyJsonTemp(stringArray[i]);
		memset(applog,0x00,sizeof(applog));
		sprintf(applog," jsonItemArrayTestInt:%d\n",CMyJsonTemp.GetInt("jsonItemArrayTestInt"));			 
		printf(applog);		
	}
	printf("----------jsonItemArray----end--\n");	  
 
	for(int i = 0;i <count;i++) 
	{
		 delete[] stringArray[i];
		 stringArray[i] = NULL;
	}

	printf("-----------jsonItemSingle---begin--\n");		
	memset(applog,0x00,sizeof(applog));
	CMyJson::cJson2Text(CMyJsonRoot.GetObjectItem("jsonItemSingle"),applog)	;	
	printf(applog);
	printf("\n");
	CMyJson CMyJsonL1jsonItemSingle(applog);
	memset(applog,0x00,sizeof(applog));	
	sprintf(applog,"jsonItemSingleTestInt:%d\n",CMyJsonL1jsonItemSingle.GetInt("jsonItemSingleTestInt"));	
	printf(applog);
	memset(pOut,0x00,sizeof(pOut));	
	CMyJsonL1jsonItemSingle.GetString("jsonItemSingleTestString",pOut);
	memset(applog,0x00,sizeof(applog));	
	sprintf(applog,"jsonItemSingleTestString:%s\n",pOut);	
	printf(applog);
	printf("----------jsonItemSingle-----end--\n");
 
	printf("----------处理jsonText-----end----\n");	
	printf("----------GetDemo-----end----------\n");
}
 
void 	CMyJson::CreateDemo()
{
	/*--------生成以下字符串-------
"{ \
\"stringSingle\": \"stringSingleTest\",\
\"intSingle\": 193,\
\"intArray\": [7, 8], \
\"stringArray\":[\"str1\",\"str2\"], \
\"jsonItemSingle\":{\"jsonItemSingleTestInt\":888,\"jsonItemSingleTestString\": \"jsonItemSingleTestString\"}, \
\"jsonItemArray\":[\
  {\"jsonItemArrayTestInt\":222},\
  {\"jsonItemArrayTestInt\":333},\
  {\"jsonItemArrayTestInt\":444},\
  {\"jsonItemArrayTestInt\":555}\
  ]\
}"
--------------------------------*/
	cJSON						*root,*cjsonItemArray;
	char            *p_json; //strlen(p_json)

	int intArray[2];
	intArray[0] = 7;
	intArray[1] = 8;	
	cJSON* cjsonIntArray = cJSON_CreateIntArray(intArray,2);
 
	cJSON* cjsonStringArray = cJSON_CreateArray();
	cJSON_AddItemToArray(cjsonStringArray, cJSON_CreateString("str1"));
	cJSON_AddItemToArray(cjsonStringArray, cJSON_CreateString("str2"));
	//cJSON_AddItemToArray(cjsonStringArray, cJSON_CreateNumber(888)); /*--add number test----*/
	
	cJSON* cjsonIempSingle = cJSON_CreateObject();
	cJSON_AddItemToObject(cjsonIempSingle, "jsonItemSingleTestInt", cJSON_CreateNumber(888));	
	cJSON_AddItemToObject(cjsonIempSingle, "jsonItemSingleTestString", cJSON_CreateString("jsonItemSingleTestString"));

	cJSON* cjsonIempTemp;
	cjsonItemArray = cJSON_CreateArray();
 	cjsonIempTemp = cJSON_CreateObject();
 	cJSON_AddItemToObject(cjsonIempTemp, "jsonItemArrayTestInt", cJSON_CreateNumber(222));	 	
 	cJSON_AddItemToArray(cjsonItemArray, cjsonIempTemp);
 	cjsonIempTemp = cJSON_CreateObject();
 	cJSON_AddItemToObject(cjsonIempTemp, "jsonItemArrayTestInt", cJSON_CreateNumber(333));	 	
 	cJSON_AddItemToArray(cjsonItemArray, cjsonIempTemp);
 	cjsonIempTemp = cJSON_CreateObject();
 	cJSON_AddItemToObject(cjsonIempTemp, "jsonItemArrayTestInt", cJSON_CreateNumber(444));	 	
 	cJSON_AddItemToArray(cjsonItemArray, cjsonIempTemp);
 	cjsonIempTemp = cJSON_CreateObject();
 	cJSON_AddItemToObject(cjsonIempTemp, "jsonItemArrayTestInt", cJSON_CreateNumber(555));	 	
 	cJSON_AddItemToArray(cjsonItemArray, cjsonIempTemp);
 
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "stringSingle", cJSON_CreateString("stringSingleTest"));
  cJSON_AddItemToObject(root, "intSingle", cJSON_CreateNumber(193));
  cJSON_AddItemToObject(root, "intArray", cjsonIntArray);
  cJSON_AddItemToObject(root, "stringArray", cjsonStringArray);
  cJSON_AddItemToObject(root, "jsonItemSingle", cjsonIempSingle);
	cJSON_AddItemToObject(root, "jsonItemArray", cjsonItemArray);

  printf("\n--------print myjson ------------begin------------\n"); 
	//p_json = cJSON_Print(root);
	p_json = cJSON_PrintUnformatted(root); 
	printf(p_json);
  printf("\n--------print myjson ------------end------------\n\n\n"); 
	
	GetDemo(p_json);
	
	/* --- 释放p_json --- */
	free(p_json);
	p_json = NULL;
	/* ---释放root  --- */
	cJSON_Delete(root);
	
	printf("\n--------CreateDemo------------end------------\n");	
} 


CMyXml::CMyXml()
{
}


CMyXml::CMyXml(char *myXmlFileName)
{
	m_myXmlDocument = new TiXmlDocument(myXmlFileName);
	m_lastOperateIsOk = m_myXmlDocument->LoadFile();
	if (m_lastOperateIsOk)
	{
		m_myXmlRootNode =  m_myXmlDocument->RootElement();
		m_myXmlCurrentNode = m_myXmlDocument->RootElement();
	}
}


CMyXml::~CMyXml()
{
	//printf("~CMyXml() ~CMyXml() ~CMyXml() \n");
	if(m_myXmlDocument)
	{
		delete m_myXmlDocument;
		m_myXmlDocument = NULL;

		//printf("~CMyXml() ~CMyXml() ~CMyXml() m_myXmlDocument \n");
	}
}


/* ------------------------------------------------------------------------------------
            功能描述:   得到当前节点下,所包含的子节点名字的数量
--------------------------------------------------------------------------------------- */
int CMyXml::GetChildNodeCount(TiXmlElement  *myXmlCurrentNode,char *childNodeName)
{
	int childNodeCount = 0;
	if(myXmlCurrentNode == NULL)
		return 0;

	TiXmlElement* pEle = myXmlCurrentNode; 
	for (pEle = myXmlCurrentNode->FirstChildElement(); pEle; pEle = pEle->NextSiblingElement())  
    {  
		if (strcmp(pEle->Value(),childNodeName)==0)
		{
		   childNodeCount++	;
		}
    }  

	return	childNodeCount; 
}

 
/* ------------------------------------------------------------------------------------------
            功能描述:   得到当前节点下,所包含的子节点名字的数量和输出符合条件的节点数组
--------------------------------------------------------------------------------------------- */
int CMyXml::GetChildNodeArray(TiXmlElement  *myXmlCurrentNode,char *childNodeName,TiXmlElement  **myXmlChildNodeArray)
{
	int childNodeCount = 0;
	if(myXmlCurrentNode == NULL)
		return 0;

	TiXmlElement* pEle = myXmlCurrentNode; 
	for (pEle = myXmlCurrentNode->FirstChildElement(); pEle; pEle = pEle->NextSiblingElement())  
    {  
		if (strcmp(pEle->Value(),childNodeName)==0)
		{
			myXmlChildNodeArray[childNodeCount]	= pEle;
			childNodeCount++	;
		}
    }  

	return	childNodeCount; 
}

/* ------------------------------------------------------------------------------------------
            功能描述:  递归得到myXmlStr 所在节点	myXmlNodePointer;
--------------------------------------------------------------------------------------------- */
bool CMyXml::GetNodePointer(TiXmlElement  *myXmlCurrentNode,char *myXmlStr,EnumMyXmlStrType enumMyXmlStrType,TiXmlElement* &myXmlNodePointer)
{
	if(myXmlCurrentNode == NULL || myXmlStr == NULL )
	{
		return false;
	}
	if (enumMyXmlStrType == EnumMyXmlStrType_NODE_NAME)
	{
		if (strcmp(myXmlStr,myXmlCurrentNode->Value()) == 0)
		{
			myXmlNodePointer = myXmlCurrentNode;
			return true;
		}
	}

	TiXmlNode* child = NULL;
	TiXmlText* childText = NULL;
	child = myXmlCurrentNode->FirstChild();	
	if(child && (enumMyXmlStrType == EnumMyXmlStrType_NODE_VALUE) )
	{
		childText = child->ToText();
		if(childText)
		{
			if (strcmp(myXmlStr,myXmlCurrentNode->GetText()) == 0)
			{
				myXmlNodePointer = myXmlCurrentNode;
				return true;
			}  
		}
	}

	TiXmlAttribute* nodeAttributeTemp = myXmlCurrentNode->FirstAttribute();
	if (nodeAttributeTemp != NULL && (enumMyXmlStrType == EnumMyXmlStrType_ATTRIBUTE_NAME||enumMyXmlStrType == EnumMyXmlStrType_ATTRIBUTE_VALUE))
	{
		for (nodeAttributeTemp = myXmlCurrentNode->FirstAttribute(); nodeAttributeTemp; nodeAttributeTemp = nodeAttributeTemp ->Next())  
		{  
			if (enumMyXmlStrType == EnumMyXmlStrType_ATTRIBUTE_NAME && strcmp(myXmlStr,nodeAttributeTemp->Name()) == 0)
			{
				myXmlNodePointer = myXmlCurrentNode;
				return true;
			}
			if (enumMyXmlStrType == EnumMyXmlStrType_ATTRIBUTE_VALUE && strcmp(myXmlStr,nodeAttributeTemp->Value()) == 0)
			{
				myXmlNodePointer = myXmlCurrentNode;
				return true;
			}
		}   
	}
 

	TiXmlElement* pEle = myXmlCurrentNode; 
	for (pEle = myXmlCurrentNode->FirstChildElement(); pEle; pEle = pEle->NextSiblingElement())  
    {  
		if(GetNodePointer(pEle,myXmlStr,enumMyXmlStrType,myXmlNodePointer))
		{
			return true;
		}
    }  
	return false;

};


/*	 -----------------------GetPersonDemo.xml------文件内容-------------------
<?xml version="1.0" encoding="utf-8" ?>
<Persons>  
  <Person ID="1">
    <name ID="101" NameEn ="Person1">TestPersonNameCn1</name>
     <age>21</age>
     <sex>nan</sex>
  </Person> 
  <Person ID="2">
    <name ID="102" NameEn="Person2">TestPersonNameCn2</name>
    <age>22</age>
    <sex>nv</sex>
  </Person>
  <Person ID="3">
    <name ID="103" NameEn ="Person3">TestPersonNameCn3</name>
    <age>22</age>
    <sex>nan</sex>
  </Person>
</Persons>
-----------------------------------------------------------------------*/

void CMyXml::GetPersonXmlDemo(char *demoXmlFileName )
{
	printf("/*-----------------------GetPersonDemo----begin----------------*/ \n");

	char	xmlFileName[MAX_PATH],textFileName[MAX_PATH];
	char	logdata[1024],xmlStr[1024];
	bool	flag;

	/* -------- 文件名赋值 ---------- */
	memset(xmlFileName,0x00,sizeof(xmlFileName));
	if(demoXmlFileName == NULL)
	{
		sprintf(xmlFileName,"%s","PersonXmlDemo.xml");
	}
	else
	{
		sprintf(xmlFileName,"%s",demoXmlFileName);
	}

 	CreatePersonXmlDemo(xmlFileName);  //生成文件

	memset(textFileName,0x00,sizeof(textFileName));
	sprintf(textFileName,"%s","PersonXmlDemo.txt");
	
	 /* -------- 读取并打印到屏幕上 ---------- */
	 memset(logdata,0x00,sizeof(logdata));
 	 CommonFuncs::ReadFileBuffer(xmlFileName,logdata);
	 printf("----xmlPrint---begin--\n");
	 printf(logdata);
	 printf("----xmlPrint---end--\n");

	/* -------- xml文件转为txt文件 ---------- */
	CommonFuncs::WriteBufferFile("wb",textFileName,logdata,strlen(logdata));


	/* -------- 载入XML文件 ---------- */
	CMyXml myXml(xmlFileName);	 
	flag = myXml.m_lastOperateIsOk;
	if(flag == false)
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"%s 装载失败\n",xmlFileName);
		printf(logdata);
 
		return;
	}
 
	/* -------- 获得声明 ---------- */
	TiXmlDeclaration *pXmlDecl =  myXml.m_myXmlDocument->FirstChild()->ToDeclaration();
	if(pXmlDecl != NULL)
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"(%s)Declaration:Version:%s,Encoding:%s,Standalone:%s!\n",xmlFileName,pXmlDecl->Version(),pXmlDecl->Encoding(),pXmlDecl->Standalone());   
		printf(logdata);
	}

   	/* -------- 获得根元素 ---------- */
	TiXmlElement *RootElement = myXml.m_myXmlRootNode;

	/* -------- 获得指定子节点的指针数组 ---------- */
	memset(xmlStr,0x00,sizeof(xmlStr));
	strcpy(xmlStr,"Person");
	int childeCount = CMyXml::GetChildNodeCount(RootElement,xmlStr);
	TiXmlElement *pChildNode[100]; 
	if(childeCount == 0 || childeCount > 100)
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"子节点当前个数为：%d,不符合继续执行条件!\n",childeCount);
		printf(logdata);
		return;
	}
	childeCount = CMyXml::GetChildNodeArray(RootElement,xmlStr,pChildNode);
	for(int i=0;i<childeCount;i++)
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"pChildNode[%d]->Value() :%s!\n",i,pChildNode[i]->Value());
		printf(logdata);
	}

	printf("---GetNodePointer--------begin------------\n");
	TiXmlElement *pOutNode = NULL;
	memset(xmlStr,0x00,sizeof(xmlStr));
	strcpy(xmlStr,"age");	
	flag=  CMyXml::GetNodePointer(RootElement,xmlStr,EnumMyXmlStrType_NODE_NAME,pOutNode);
	if(flag)
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"NODE_NAME(%s)pOutNode->Value() :%s!\n",xmlStr,pOutNode->Value());
		printf(logdata);
	}
	else
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"NODE_NAME(%s)查找对应节点失败!\n",xmlStr);
		printf(logdata);
	}

	memset(xmlStr,0x00,sizeof(xmlStr));
	strcpy(xmlStr,"TestPersonNameCn1");	
	flag=  CMyXml::GetNodePointer(RootElement,xmlStr,EnumMyXmlStrType_NODE_VALUE,pOutNode);
	if(flag)
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"NODE_VALUE(%s)pOutNode->Value() :%s!\n",xmlStr,pOutNode->Value());
		printf(logdata);
	}
	else
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"NODE_VALUE(%s)查找对应节点失败!\n",xmlStr);
		printf(logdata);
	}
 
	memset(xmlStr,0x00,sizeof(xmlStr));
	strcpy(xmlStr,"NameEn");	
	flag=  CMyXml::GetNodePointer(RootElement,xmlStr,EnumMyXmlStrType_ATTRIBUTE_NAME,pOutNode);
	if(flag)
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"ATTRIBUTE_NAME(%s)pOutNode->Value() :%s!\n",xmlStr,pOutNode->Value());
		printf(logdata);
	}
	else
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"ATTRIBUTE_NAME(%s)查找对应节点失败!\n",xmlStr);
		printf(logdata);
	}

	memset(xmlStr,0x00,sizeof(xmlStr));
	strcpy(xmlStr,"Person1");	
	flag=  CMyXml::GetNodePointer(RootElement,xmlStr,EnumMyXmlStrType_ATTRIBUTE_VALUE,pOutNode);
	if(flag)
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"ATTRIBUTE_VALUE(%s)pOutNode->Value() :%s!\n",xmlStr,pOutNode->Value());
		printf(logdata);
	}
	else
	{
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"ATTRIBUTE_VALUE(%s)查找对应节点失败!\n",xmlStr);
		printf(logdata);
	}
	printf("---GetNodePointer--------end------------\n");

   //操作节点以及属性值,	GetNodePointer 中均有对应代码实现


 	printf("---gradually find xmlNode--------begin------------\n");
	TiXmlElement* root = myXml.m_myXmlDocument->FirstChildElement( "Persons" );
	if ( root )
	{
		TiXmlElement* element = root->FirstChildElement( "Person" );
		if ( element )
		{
			TiXmlElement* child = element->FirstChildElement( "name" );
			if ( child )
			{
				TiXmlElement* child2 = child->NextSiblingElement( "age" );
				if ( child2 )
				{
					 	printf("---gradually find xmlNode----age----success------------\n");
						memset(logdata,0x00,sizeof(logdata));
						sprintf(logdata,"child2->GetText():%s\n",child2->GetText());
						printf(logdata);
				}	 
			}
		}
	}
	

 	printf("---gradually find xmlNode--------end------------\n");

  printf("---docHandle find xmlNode--------begin------------\n");
	TiXmlHandle docHandle( myXml.m_myXmlDocument );
	//TiXmlElement* child22 = docHandle.FirstChild("Persons").FirstChild( "Person" ).FirstChild( "age" ).ToElement();
	 TiXmlElement* child22 = docHandle.FirstChild("Persons").Child("Person",1).FirstChild( "age" ).ToElement();
	if ( child22 )
	{
		printf("---docHandle find xmlNode----age----success------------\n");
		memset(logdata,0x00,sizeof(logdata));
		sprintf(logdata,"child22->GetText():%s\n",child22->GetText());
		printf(logdata);
	}
   	printf("---docHandle find xmlNode--------end------------\n");

	printf("/*-----------------------GetPersonDemo----end----------------*/ \n"); 
}

void CMyXml::CreatePersonXmlDemo(char *demoXmlFileName )
{
	char xmlFileName[256];
	memset(xmlFileName,0x00,sizeof(xmlFileName));
	if(demoXmlFileName == NULL)
	{
		sprintf(xmlFileName,"%s","PersonXmlDemo.xml");
	}
	else
	{
		sprintf(xmlFileName,"%s",demoXmlFileName);
	}
		 

	printf("/*-----------------------CreatePersonDemo----begin----------------*/ \n"); 
	TiXmlDocument *myXmlDocument= new   TiXmlDocument(demoXmlFileName);

	/*-----------------------创建根节点------------------*/
	TiXmlElement *RootElement=new TiXmlElement("Persons");
  myXmlDocument->LinkEndChild(RootElement);

	TiXmlElement *ChildElementPerson =NULL;
	TiXmlElement *ChildElementName = NULL;
	TiXmlText *NameContent  =NULL;

	/*----	设置节点-------------*/
	ChildElementPerson=new TiXmlElement("Person");
  RootElement->LinkEndChild(ChildElementPerson);

	/*----	设置节点-------------*/ 	
	ChildElementName = new TiXmlElement("name");
  ChildElementPerson->LinkEndChild(ChildElementName);
	
	/*----	设置节点属性-------------*/
  ChildElementName->SetAttribute("ID","101");   
  ChildElementName->SetAttribute("NameEn","Person1");

	/*----	设置节点标签中的内容-------------*/ 
	NameContent =new TiXmlText("TestPersonNameCn1");
	ChildElementName->LinkEndChild(NameContent);

 	/*----	设置节点-------------*/ 
	ChildElementName = new TiXmlElement("age");
  ChildElementPerson->LinkEndChild(ChildElementName);
	
	/*----	设置节点标签中的内容-------------*/ 
	NameContent =new TiXmlText("21");
	ChildElementName->LinkEndChild(NameContent);

  /*----	设置节点-------------*/ 
	ChildElementName = new TiXmlElement("sex");
  ChildElementPerson->LinkEndChild(ChildElementName);

 	/*----	设置节点标签中的内容-------------*/ 
	NameContent =new TiXmlText("nan");
	ChildElementName->LinkEndChild(NameContent);







 	/*----	设置节点-------------*/
	ChildElementPerson=new TiXmlElement("Person");
  RootElement->LinkEndChild(ChildElementPerson);

	/*----	设置节点-------------*/ 	
	ChildElementName = new TiXmlElement("name");
  ChildElementPerson->LinkEndChild(ChildElementName);
	
	/*----	设置节点属性-------------*/
  ChildElementName->SetAttribute("ID","102");   
  ChildElementName->SetAttribute("NameEn","Person2");

	/*----	设置节点标签中的内容-------------*/ 
	NameContent =new TiXmlText("TestPersonNameCn2");
	ChildElementName->LinkEndChild(NameContent);

 	/*----	设置节点-------------*/ 
	ChildElementName = new TiXmlElement("age");
  ChildElementPerson->LinkEndChild(ChildElementName);
	
	/*----	设置节点标签中的内容-------------*/ 
	NameContent =new TiXmlText("22");
	ChildElementName->LinkEndChild(NameContent);

  /*----	设置节点-------------*/ 
 	ChildElementName = new TiXmlElement("sex");
  ChildElementPerson->LinkEndChild(ChildElementName);

 	/*----	设置节点标签中的内容-------------*/ 
	NameContent =new TiXmlText("nv");
	ChildElementName->LinkEndChild(NameContent);








  	/*----	设置节点-------------*/
	ChildElementPerson=new TiXmlElement("Person");
  RootElement->LinkEndChild(ChildElementPerson);

	/*----	设置节点-------------*/ 	
	ChildElementName = new TiXmlElement("name");
  ChildElementPerson->LinkEndChild(ChildElementName);
	
	/*----	设置节点属性-------------*/
  ChildElementName->SetAttribute("ID","103");   
  ChildElementName->SetAttribute("NameEn","Person3");

	/*----	设置节点标签中的内容-------------*/ 
	NameContent =new TiXmlText("TestPersonNameCn3");
	ChildElementName->LinkEndChild(NameContent);

 	/*----	设置节点-------------*/ 
	ChildElementName = new TiXmlElement("age");
	ChildElementPerson->LinkEndChild(ChildElementName);
	
	/*----	设置节点标签中的内容-------------*/ 
	NameContent =new TiXmlText("23");
	ChildElementName->LinkEndChild(NameContent);

	/*----	设置节点-------------*/ 
	ChildElementName = new TiXmlElement("sex");
  ChildElementPerson->LinkEndChild(ChildElementName);

 	/*----	设置节点标签中的内容-------------*/ 
	NameContent = new TiXmlText("nan");
	ChildElementName->LinkEndChild(NameContent);



   /*--------删除节点,属性值------------------*/
	//RemoveChild( TiXmlNode* removeThis )方法删除父节点的子节点,
	//RemoveAttribute( const char * name )方法删除属性值.

	//ReplaceChild( TiXmlNode* replaceThis, const TiXmlNode& withThis )方法替换节点


	myXmlDocument->SaveFile();
	if (myXmlDocument)
	{
	  delete  myXmlDocument;
		myXmlDocument = NULL;
	}

	printf("/*-----------------------CreatePersonDemo----end----------------*/ \n"); 
}
