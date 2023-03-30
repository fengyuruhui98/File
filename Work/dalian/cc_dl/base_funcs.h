//
// Created by HHJT_CJP on 19-12-10.
//

#ifndef _BASE_FUNCS_H
#define _BASE_FUNCS_H

#include "sys_head.h"
#include "base_define.h"
#include "base_cjson.h"
#include "base_xml.h"

#define INI_REMOVE_CR
#define DONT_HAVE_STRUPR

#ifndef MAXBUFFERLEN
#define MAXBUFFERLEN     	        8*1024	    
#endif

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

#define tpNULL       0
#define tpSECTION    1
#define tpKEYVALUE   2
#define tpCOMMENT    3

struct ENTRY
{
    char   Type;
    char  *pText;
    struct ENTRY *pPrev;
    struct ENTRY *pNext;
};

struct EFIND
{
    struct ENTRY *pSec;
    struct ENTRY *pKey;
    char          KeyText [128];
    char          ValText [128];
    char          Comment [255];
} ;

/* Macros */
#define ArePtrValid(Sec,Key,Val) ((Sec!=NULL)&&(Key!=NULL)&&(Val!=NULL))

class CIniFile
{
public:
    CIniFile    (void);
    ~CIniFile   (void);
    UINT    GetVersion  (void);
    BOOL    OpenIniFile (CCHR *pFileName);
    BOOL    ReadBool    (CCHR *pSection, CCHR *pKey, BOOL   Default);
    int     ReadInt     (CCHR *pSection, CCHR *pKey, int    Default);
    double  ReadDouble  (CCHR *pSection, CCHR *pKey, double Default);
    char    *ReadString (CCHR *pSection, CCHR *pKey, char  *pDefault);

    void    WriteBool   (CCHR *pSection, CCHR *pKey, bool   Value);
    void    WriteInt    (CCHR *pSection, CCHR *pKey, int    Value);
    void    WriteDouble (CCHR *pSection, CCHR *pKey, double Value);
    void    WriteString (CCHR *pSection, CCHR *pKey, CCHR  *pValue);

    void    CloseIniFile ();
    bool    WriteIniFile (CCHR *pFileName);
    bool	DeleteKey (CCHR *pSection, CCHR *pKey);

protected:
    struct  ENTRY *m_pEntry;
    struct  ENTRY *m_pCurEntry;
    char    m_result [255];
    FILE    *m_pIniFile;
    void    AddKey     (struct ENTRY *pEntry, CCHR *pKey, CCHR *pValue);
    BOOL    AddItem    (char Type, CCHR *pText);
    bool    AddItemAt (struct ENTRY *pEntryAt, char Mode, CCHR *pText);
    void    FreeMem    (void *pPtr);
    void    FreeAllMem (void);
    bool    FindKey    (CCHR *pSection, CCHR *pKey, EFIND *pList);
    bool    AddSectionAndKey (CCHR *pSection, CCHR *pKey, CCHR *pValue);
    struct  ENTRY *MakeNewEntry (void);
    struct  ENTRY *FindSection (CCHR *pSection);
};


class CommonFuncs
{
public:
    static	int          	 GetEndian();	
    static	void           chartobyte(char *in,char *out);
    static	void           chartobyte(const char *in,char *out,int len);
    static	void           bcdtobyte(char *in,char *out,int len);
    static	unsigned int   dwordtowin(unsigned int in);
    static	unsigned short wordtowin(unsigned short in);
    static	int            ReadFileBuffer(char *filename,char *buffer);
    static	int            WriteBufferFile(const char *flag,char *filename,char *recv_buf,int buflen);
    static	int            ByteToInt(unsigned char *b_int,int count);
    static	void           delspace(char *temp,char *buffer);
    static	int            code_convert(const char *from_charset,const char *to_charset,char *inbuf,size_t inlen,char *outbuf, size_t outlen); 
    static	int            utf82gbk(const char *inbuf, size_t inlen, char *outbuf, size_t outlen); 
    static	int            gbk2utf8(const char *inbuf, size_t inlen, char *outbuf, size_t outlen);   
    static	int			   		 utf82gbk(char *gbkStr, const char *srcStr, int maxGbkStrlen);
    static	void					 UCS2toUTF8(const unsigned short *ucs2, int count, char *utf8);
    static	void 					 ConvertCnchar(char *source_str,int len,char *dest_str);
    static	void           my_sleep(int timeout);
    static	void           my_usleep(int timeout);
    static	void					 GetHexIP(char* ip,BYTE* LocalIP);
    static	int						 GetLocalHostInfo(char *localIP);	
    static	int   				 detect_process(const char *process_name);
    static	void 					 Daemon_init();
    static	int						 FileIsExist(char *dirname,char *name);
    static	int						 DirIsEmpty(char *dirname);
    static	int						 CountOFDir(char *dirname);
    static	int 					 GetCfgFileName(char *pathname);
    static	void 					 Conver_Byte(BYTE *b_source,char *c_dest,int len); 
    static	void					 Conver_H_L_Byte(BYTE *b_source,char *c_dest,int len); 
    static	int 					 String_Isdigit(unsigned char *p,int len);
    static	int 					 FindChar(char *p,const char c);
    static	bool 					 DiskIsFull(int fullPercent,char *diskMessage);
    static	void 					 FormatDir(char *c_source); 
    static	int 					 BcdToDec(int bcdInt);   
    static	int 					 DecToBcd(int decInt); 
    static	void 					 DeleteFile(char *dir,char *subStr); 
    static	void 					 EmptyDirFile(char *dir);     
};

 

#define NTP_PCK_LEN 	48
#define JAN_1970 		0x83aa7e80 /* 1900年～1970年之间的时间秒数 */

class CTime
{

public:
    static  time_t         GetGmtTime(const char *c_time);
    static	void           GetLocalDateTime(char *buffer);
    static	void           DateToChar(unsigned int validdatetime, char *curtime);
    static	void           DateToChar2(unsigned int validdatetime, char *curtime);
    static	void           GetStringTime(char *buffer);
    static	void           GetStringTime(char *buffer,bool isSpace);
    static	void           GetLocalTime(char *buffer);
    static	time_t 		   	 GetGmtTime(int year,int month,int day,int hour,int minute,int second);
    static	int						 DateToDate2_t(int wYear,int wMonth,int wDay);
    static	void 					 Date2_tToDate(int dwDayIdx,char *c_date);
    static	int						 Days19700101ToNow();
    static	int						 Days19700101ToNow(int secondsFrom1970);
    static	int						 Seconds1970ToNow(); 
    static	int						 CheckTimeFormat(char *c_time);    
};

class CLog
{

private:
    static FILE*     g_AppLog_fp;
    static FILE*     g_CommLog_fp;
    static char      g_AppLogPath[MAX_PATH];
    static sem_t  	 g_SemWriteAppLog;
    static sem_t*  	 g_SemWriteCommLog;

public:
    static int  InitLogStaticVar(char *appLogPath);
    static void WriteAppLog(const char *buffer);
    static void WriteDebugLog(int flag,unsigned char *buffer,int buflen);
    static void WriteCommuLog(char *buffer);
    static void ReleaseLogStaticVar();

private:
    static	void  GetLocalTime(char *buffer);
};


 //所有有关Json函数 
class CMyJson
{
	public:	
		CMyJson();
		CMyJson(char *ptext);
		CMyJson(char *ptext,bool isUtf8Flag);			
		virtual ~CMyJson();

	public:
		cJSON  *m_cjson;
		bool 		m_isUtf8Flag;
		bool 		m_isFailed ;
		
	public:
		cJSON* 	GetObjectItem(const char *pitem);
		void 		GetString(const char *pitem,char *pvalue);	
		int 		GetInt(const char *pitem);
		void		GetJsonPrintText(char *jsonText);
		int 		GetArraySize(const char *pitem);
		int 		GetIntArray(const char *pitem,int* intArray);
		int 		GetStringArray(const char *pitem,char** stringArray);
		int 		GetJsonStringArray(const char *pitem,char** stringArray);
	public:		
		static cJSON* GetArrayItem(cJSON *cjson,int index);
		static int 		GetArraySize(cJSON *cjson);
		static int 		GetIntArray(cJSON *cjson,int* intArray);
		static int 		GetStringArray(cJSON *cjson,char** stringArray); 	
		static int 		GetJsonStringArray(cJSON *cjson,const char *pitem,char** stringArray);	
		static void 	GetString(char *ptext,const char *pitem,char *pvalue);
		static int 		GetInt(char *ptext,const char *pitem);
		static void 	GetChildNodeString(char *ptext,const char *pitem1,const char *pitem2,char *pvalue);
		static int 		GetChildNodeInt(char *ptext,const char *pitem1,const char *pitem2);
		static void 	GetJsonPrintText(char *ptext,char *jsonText);
		static void 	cJson2Text(cJSON *cjson,char *jsonText);
		static void 	GetDemo(char *cjsonText); /*--json---处理测试函数--------*/
		static void 	CreateDemo(); /*---json---生成测试函数--------*/
}; 



enum EnumMyXmlStrType
{
	EnumMyXmlStrType_NODE_NAME,
	EnumMyXmlStrType_NODE_VALUE,
	EnumMyXmlStrType_ATTRIBUTE_NAME,
	EnumMyXmlStrType_ATTRIBUTE_VALUE,
};


class CMyXml
{
public:
	CMyXml();
	CMyXml(char *myXmlFileName);
	~CMyXml();
public:
	TiXmlDocument *m_myXmlDocument;		/*--xml文件--*/
	TiXmlElement  *m_myXmlRootNode;		/*--根节点----*/
	TiXmlElement  *m_myXmlCurrentNode;	/*---记录当前节点---*/
	bool m_lastOperateIsOk;				/*---记录上次操作是否成功---*/
public:
	static int	GetChildNodeCount(TiXmlElement  *myXmlCurrentNode,char *childNodeName);
	static int	GetChildNodeArray(TiXmlElement  *myXmlCurrentNode,char *childNodeName,TiXmlElement  **myXmlChildNodeArray);	
	static bool GetNodePointer(TiXmlElement  *myXmlCurrentNode,char *myXmlStr,EnumMyXmlStrType enumMyXmlStrType,TiXmlElement* &myXmlNodePointer);	
	static void GetPersonXmlDemo(char *demoXmlFileName );
	static void CreatePersonXmlDemo(char *demoXmlFileName );
};



#endif //_BASE_FUNCS_H
