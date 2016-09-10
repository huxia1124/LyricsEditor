
/********************************************************************
	created:	????-??-??   
	file base:	MP3Read
	file ext:	H
	author:		XiaHuan
	
	purpose:	Manage MP3 Synchrolized and Unsynchrolized lyrics
				operations

	requirement:	MFC
*********************************************************************/

#ifndef _MP3READ_H_
#define _MP3READ_H_

#include <string.h>
#include <afxtempl.h>

#pragma pack(1)


/////////////////////////////////////////////////////////////////////////
int strcmpx(const char *p1,const char *p2,int len) // Compare two strings,checking specified number of characters (Case sensitive)
{
	char *pa,*pb;
	pa=(char*)malloc(len+1);
	pb=(char*)malloc(len+1);

	strncpy(pa,p1,len);
	strncpy(pb,p2,len);
	pa[len]=0;
	pb[len]=0;

	int res=strcmp(pa,pb);

	free(pa);
	free(pb);
	return res;
}

int strcmpxi(const char *p1,const char *p2,int len) // Compare two strings,checking specified number of characters (NO Case sensitive)
{
	char *pa,*pb;
	pa=(char*)malloc(len+1);
	pb=(char*)malloc(len+1);

	strncpy(pa,p1,len);
	strncpy(pb,p2,len);
	pa[len]=0;
	pb[len]=0;

	int res=strcmpi(pa,pb);

	free(pa);
	free(pb);
	return res;
}
LONG getline(char *pBuf,const char *pScBuf) //Get a line of string,append with '\0' (ANSI)
{
	LONG nPos=0;
	while(pScBuf[nPos]!=0)
	{
		if(pScBuf[nPos]==13 && pScBuf[nPos+1]==10)
		{
			strncpy(pBuf,pScBuf,nPos+2);
			pBuf[nPos+2]=0;
			return nPos;
		}
		nPos++;
	}
	if(nPos>0)
	{
		strncpy(pBuf,pScBuf,nPos+2);
		pBuf[nPos+2]=0;
		return nPos;
	}
	return -1;
}
LONG getstring(char *pBuf,const char *pScBuf)  // Get a NULL-Termined string (ANSI)
{
	LONG nPos=0;
	while(pScBuf[nPos]!=0)
	{
		nPos++;
	}
	if(pBuf!=NULL)
		strncpy(pBuf,pScBuf,nPos+1);
	return nPos;
}
LONG getstringw(WCHAR *pBuf,const WCHAR *pScBuf) // Get a NULL-Termined string (UNICODE)
{
#ifndef _UNICODE
#define _UNICODE
#ifndef UNICODE
#define UNICODE
	LONG nPos=0;
	while(_T(pScBuf[nPos])!=_T(0))
	{
		nPos+=1;
	}
	if(pBuf!=NULL)
		wcsncpy(pBuf,pScBuf,nPos+2);
	return nPos;
#undef UNICODE
#endif
#undef _UNICODE
#endif
}

int isformat(const char *pScBuf,const char *pFormat) // Check if a string have specified format
{
	while(*pFormat!=0)
	{
		switch(*pFormat)
		{
		case '%':
			{
				pFormat++;
				int endpos=strcspn(pFormat,"NC%");
				if(endpos==0)
				{
					switch(pFormat[endpos])
					{
					case '%':
						if(*pScBuf!='%')
							return FALSE;
						break;
					case 'N':
						if(!isdigit(*pScBuf))
							return FALSE;
						break;
					case 'C':
						if(!isalpha(*pScBuf))
							return FALSE;
						break;
					}
					pScBuf++;
				}
				else //endpos>1
				{
					char chn[10];
					strncpy(chn,pFormat,endpos);
					chn[endpos]=0;
					int nck=atoi(chn);
					int i;
					switch(pFormat[endpos])
					{
					case 'N':
						{
							for(i=0;i<=nck-1;i++)
								if(!isdigit(pScBuf[i]))
									return FALSE;
							break;
						}
					case 'C':
						{
							for(i=0;i<=nck-1;i++)
								if(!isalpha(pScBuf[i]))
									return FALSE;
							break;
						}
					}
					pScBuf+=nck;
				}				
				pFormat+=(endpos+1);
				break;
			}
		default: //Not a format unit
			{
				if(*pScBuf!=*pFormat)
					return FALSE;
				pFormat++;
				pScBuf++;
			}
		}  // switch(*pFormat)
	}
	return TRUE;
}

int strchrr(const char *pScBuf,int c) // Reverse find
{
	int len=strlen(pScBuf);
	len--;
	while(len>=0)
	{
		if(pScBuf[len]==c)
			return len;
		len--;
	}
	return -1;
}
LONG getmilliseconds(const char *pBuf)  // Convert [??:??.??] or [??:??] to milliseconds
{
	if(isformat(pBuf,"[%2N:%2N.%2N]"))
	{
		return atoi(pBuf+1) * 60 * 1000
			+ atoi(pBuf+4) * 1000
			+ atoi(pBuf+7) * 10;
	}
	else if(isformat(pBuf,"[%2N:%2N]"))
	{
		return atoi(pBuf+1) * 60 * 1000
			+ atoi(pBuf+4) * 1000;
	}
	return 0;
}
LONG getsize(const BYTE *pSizeBuf) // Convert 4-byte[7-7-7-7] to a single Long value
{
	return pSizeBuf[0] * (1<<21)
		+ pSizeBuf[1] * (1<<14)
		+ pSizeBuf[2] * (1<<7)
		+ pSizeBuf[3] ;
}
LONG getsizex(const BYTE *pSizeBuf) // Convert 4-byte[8-8-8-8] to a single Long value
{
	return pSizeBuf[0] * (1<<24)
		+ pSizeBuf[1] * (1<<16)
		+ pSizeBuf[2] * (1<<8)
		+ pSizeBuf[3] ;
}
BOOL gensize(BYTE *pBuf,LONG nSize) // Convert a single Long value to 4-byte[7-7-7-7]
{
	pBuf[3]=(unsigned char)nSize;
	pBuf[3] &= 0x7F; //01111111
	nSize>>=7;

	pBuf[2]=(unsigned char)nSize;
	pBuf[2] &= 0x7F; //01111111
	nSize>>=7;

	pBuf[1]=(unsigned char)nSize;
	pBuf[1] &= 0x7F; //01111111
	nSize>>=7;

	pBuf[0]=(unsigned char)nSize;
	pBuf[0] &= 0x7F; //01111111

	return TRUE;
}
BOOL gensizex(BYTE *pBuf,LONG nSize) // Convert a single Long value to 4-byte[8-8-8-8]
{
	pBuf[3]=(unsigned char)nSize;
	nSize>>=8;

	pBuf[2]=(unsigned char)nSize;
	nSize>>=8;

	pBuf[1]=(unsigned char)nSize;
	nSize>>=8;

	pBuf[0]=(unsigned char)nSize;
	return TRUE;
}

void WriteFile(CFile &tgFile,CFile &scFile,LONG nStart,LONG nCount) // Write Tag data appended with partial file data to a specified file
{
	scFile.Seek(nStart,CFile::begin);
#define BUFLEN 100000
	LONG nUnit,nLeft;
	nUnit=nCount/BUFLEN;
	nLeft=nCount%BUFLEN;
	char buf[BUFLEN];
	for(LONG i=1;i<=nUnit;i++)
	{
		scFile.Read(buf,BUFLEN);
		tgFile.Write(buf,BUFLEN);
	}
	scFile.Read(buf,nLeft);
	tgFile.Write(buf,nLeft);
#undef BUFLEN

}
void ReturnToText(char *pBuf)
{
	while(*pBuf!=0)
	{
		if(*pBuf==13 && *(pBuf+1)==10)
		{
			*pBuf='\\';
			*(pBuf+1)='n';
			pBuf+=2;
			continue;
		}
		pBuf++;
	}
}
void ReturnToTextW(WCHAR *pBuf)
{
	while(*pBuf!=0)
	{
		if(*pBuf==13 && *(pBuf+1)==10)
		{
			*pBuf=L'\\';
			*(pBuf+1)=L'n';
			pBuf+=2;
			continue;
		}
		pBuf++;
	}
}
void TextToReturn(char *pBuf)
{
	while(*pBuf!=0)
	{
		if(*pBuf=='\\' && *(pBuf+1)=='n')
		{
			*pBuf=13;
			*(pBuf+1)=10;
			pBuf+=2;
			continue;
		}
		pBuf++;
	}
}
void TextToReturnW(WCHAR *pBuf)
{
	while(*pBuf!=0)
	{
		if(*pBuf==L'\\' && *(pBuf+1)==L'n')
		{
			*pBuf=13;
			*(pBuf+1)=10;
			pBuf+=2;
			continue;
		}
		pBuf++;
	}
}

int formatlen(char *pFormat)
{
	int len=0;
	while(*pFormat!=0)
	{
		if(*pFormat=='%')
		{
			pFormat++;
			if(*pFormat=='%')
			{
				pFormat++;
				len++;
			}
			else
			{
				int pos=strcspn(pFormat,"NC");
				if(pos==0)
				{
					// Impossible
				}
				len+=atoi(pFormat);
				pFormat+=pos+1;
			}
		}
		else
		{
			pFormat++;
			len++;
		}
	}
	return len;
}

int formatfindr(char *pBuf,char *pFormat)
{
	int len=formatlen(pFormat);
	int lenp=strlen(pBuf);
	for(int i=lenp-len;i>=0;i--)
	{
		if(isformat(pBuf+i,pFormat))
			return i+len;
	}
	return -1;
}


/////////////////////////////////////////////////////////////////////////
class MTagHeader;
class MTagExtHeader;
class MTagFooter;
class MTagFrame;
class MTag;
class MTagSYLTFrame;
class MTagUSLTFrame;


struct TAGHEADERINFO
{
	BYTE iMajorVersion;
	BYTE iReVersion;
	LONG nTagSize;
	BYTE iFlags;
};

struct HEADER
{
	char chID[3];
	union
	{
		WORD wVersion;
		struct
		{
			BYTE iMajorVersion;
			BYTE iReVersion;
		};
	};
	union
	{
		BYTE iFlags;
		struct
		{
			unsigned char bUnsynchronisation: 1 ;
			unsigned char bExtendedHeader: 1 ;
			unsigned char bExperimentalIndicator: 1 ;
			unsigned char bFooterPresent: 1 ;
			unsigned char _bFlag04:1;
			unsigned char _bFlag05:1;
			unsigned char _bFlag06:1;
			unsigned char _bFlag07:1;
		};
	};
	BYTE aSize[4];
};

typedef struct HEADER FOOTER;

struct FRAME
{
	char chID[4];
	BYTE aSize[4];
	union
	{
		WORD wFlags;
		struct
		{
			unsigned char _bFlag00:1;
			unsigned char bTagAlter:1;
			unsigned char bFileAlter:1;
			unsigned char bReadOnly:1;
			unsigned char _bFlag04:1;
			unsigned char _bFlag05:1;
			unsigned char _bFlag06:1;
			unsigned char _bFlag07:1;

			unsigned char _bFlag08:1;
			unsigned char bGrouping:1;
			unsigned char _bFlag10:1;
			unsigned char _bFlag11:1;
			unsigned char bCompression:1;
			unsigned char bEncryption:1;
			unsigned char bUnsynchronisation:1;
			unsigned char bDataLengthIndicator:1;
		};
	};
};

struct FRAMEINFO
{
	char chID[4];
	LONG32 nSize;
	union
	{
		WORD wFlags;
		struct
		{
			unsigned char _bFlag00:1;
			unsigned char bTagAlter:1;
			unsigned char bFileAlter:1;
			unsigned char bReadOnly:1;
			unsigned char _bFlag04:1;
			unsigned char _bFlag05:1;
			unsigned char _bFlag06:1;
			unsigned char _bFlag07:1;

			unsigned char _bFlag08:1;
			unsigned char bGrouping:1;
			unsigned char _bFlag10:1;
			unsigned char _bFlag11:1;
			unsigned char bCompression:1;
			unsigned char bEncryption:1;
			unsigned char bUnsynchronisation:1;
			unsigned char bDataLengthIndicator:1;
		};
	};
};

struct SYLTHEADER
{
	BYTE iTextEncoding;
	char chLanguage[3];
	BYTE iTimeStampFormat;
	BYTE iContentType;
};
struct USLTHEADER
{
	BYTE iTextEncoding;
	char chLanguage[3];
};
struct LYRICSLINE
{
	char chText[1022];
	LONG32 nMilliseconds;
};

///////////////////////////////////////////////////////////////
class MTagHeader
{
protected:
	LONG m_nTagSize;
	HEADER m_TagHeaderInfo;
	BOOL m_bAvailable;
public:
	BOOL ReadFromFile(LPCTSTR lpszFileName);
	BOOL GetTagInfo(TAGHEADERINFO &taginfo);
	MTagHeader();
	static void MakeSize(BYTE *pSizeArr,LONG nSize);

};
///////////////////////////////////////////////////////////////

class MTagFooter:public MTagHeader
{
protected:
public:
	BOOL ReadFromFile(LPCTSTR lpszFileName);
	MTagFooter();
};
///////////////////////////////////////////////////////////////
class MTagFrame
{
private:
	FRAME m_BasicFrameInfo;
	FRAMEINFO m_FrameInfo;
	BOOL m_bAvailable;
public:
	MTagFrame();
	BOOL Read(CFile &file,LONG nPosStart);
	BOOL Read(void *pMem,LONG nPosStart=0);
	BOOL GetFrameInfo(FRAMEINFO &frameinfo);
	BOOL IsFrameType(LPCTSTR lpszFrameID);
	LONG GetFrameSize();
	BOOL IsFrame();
	static void GetFrameInfo(FRAMEINFO &finfo,const FRAME &m_Frame);
};
///////////////////////////////////////////////////////////////
class MTag
{
private:
	void *m_pTag;
	LONG m_nSize;
protected:

public:
	MTag();
	~MTag();
	BOOL Read(LPCTSTR lpszFileName);
	static BOOL IsValid(LPCTSTR lpszFileName);
	BOOL ModifyFrame(LPCTSTR lpszTargetFrameID,void *pScMem,LONG nScMemSize);

	BOOL ModifySYLT(LPCTSTR lpszTargetFrameID,void *pScMem,LONG nScMemSize);
	BOOL ModifyUSLT(LPCTSTR lpszTargetFrameID,void *pScMem,LONG nScMemSize);

	LONG SearchFrame(LPCTSTR lpszFrameID);
	static LONG SearchFrame(LPCTSTR lpszFrameID,void *pMem,LONG nMemSize);
	void* GetDataPointer();
	LONG GetTagSize();
	void Reset();
	void CreateEmpty();
	void Release();

};
///////////////////////////////////////////////////////////////
class MTagSYLTFrame
{
protected:
	void *m_pFrame;
	LONG m_nSize;
	SYLTHEADER m_Header;
	CArray <LYRICSLINE,LYRICSLINE&> m_Lyrics;
	LONG m_nLines;
	LPTSTR m_lpszTitle;
	LPTSTR m_lpszLanguage;
	int m_bShort; //Short format [??:??]
public:
	MTagSYLTFrame();
	~MTagSYLTFrame();

	BOOL FromLRCFile(LPCTSTR lpszLRCFileName);
	BOOL FromLRCFormat(LPCTSTR lpszText);
	LONG FromTagData(void *pMem,LONG nSize);
	BOOL PutToEditBox(CEdit *pEdit);
	BOOL PutToString(CString &str);
	BOOL UpdateFrameData();
	void* GetFrameData();
	LONG GetFrameSize();
	BOOL ApplyOffset(LONG nOffsetMilliseconds);
	void MediaPlayerPatch(double fPatchRate);
	void SetTitleText(LPCTSTR lpszTitle);
	void SetLanguage(LPCTSTR lpszLanguage);
	void GetLanguage(LPTSTR lpszLanguageBuf);

};
///////////////////////////////////////////////////////////////
class MTagUSLTFrame
{
protected:
	void *m_pFrame;
	LONG m_nSize;
	USLTHEADER m_Header;
	void *m_pText;
	LONG m_nTextLength;
	LPTSTR m_lpszTitle;
	LPTSTR m_lpszLanguage;
public:
	MTagUSLTFrame();
	~MTagUSLTFrame();

	BOOL FromLRCFile(LPCTSTR lpszLRCFileName);
	BOOL FromLRCFormat(LPCTSTR lpszText);
	BOOL PutToEditBox(CEdit *pEdit);
	BOOL PutToString(CString &str);
	LONG FromTagData(void *pMem,LONG nSize);
	BOOL UpdateFrameData();
	void* GetFrameData();
	LPCTSTR GetTextPointer();
	LONG GetTextLength();
	LONG GetFrameSize();
	void SetTitleText(LPCTSTR lpszTitle);
	void SetLanguage(LPCTSTR lpszLanguage);
	void GetLanguage(LPTSTR lpszLanguageBuf);

};
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
LONG WriteToFile(MTag &newTag,LPCTSTR lpszOldFile,LPCTSTR lpszNewFile)
{
	MTag oldTag;
	if(!oldTag.Read(lpszOldFile))
	{
		oldTag.Reset();
	}

	int bOldNewEqual=0;
	char chTempFileName[MAX_PATH];
	if(lpszNewFile==NULL)
	{
		LPSTR pName=NULL;
		char chTempNamePart[MAX_PATH];
		GetFullPathName(lpszOldFile,MAX_PATH,chTempFileName,&pName);
		strcpy(chTempNamePart,pName);
		*pName='\0';
		strcat(chTempFileName,"~~~");
		strcat(chTempFileName,chTempNamePart);
		CFile::Rename(lpszOldFile,chTempFileName);
		lpszNewFile=lpszOldFile;
		lpszOldFile=chTempFileName;
		bOldNewEqual=1;
	}

	CFile oldFile;
	if(!oldFile.Open(lpszOldFile,CFile::modeRead))
		return -1;

	CFile newFile;
	if(!newFile.Open(lpszNewFile,CFile::modeCreate|CFile::modeWrite))
	{
		oldFile.Close();
		return -1;
	}

	CFileStatus fs;
	oldFile.GetStatus(fs);
	LONG nKeepStart;
	LONG nKeep;
	nKeepStart=oldTag.GetTagSize();
	nKeep=fs.m_size-nKeepStart;

	newFile.Write(newTag.GetDataPointer(),newTag.GetTagSize());
	WriteFile(newFile,oldFile,nKeepStart,nKeep);

	oldFile.Close();
	newFile.Close();
	if(bOldNewEqual)
	{
		CFileStatus fs;
		CFile::GetStatus(lpszOldFile,fs);
		fs.m_attribute &= (~(0x01));// readOnly removed
		CFile::SetStatus(lpszOldFile,fs);
		CFile::Remove(lpszOldFile);
	}
	return 1;
}
int InsertToArray(CArray <LYRICSLINE,LYRICSLINE&> *pLyricsArr,LYRICSLINE &LyricsLine,int nCurCount=-1)
{
	if(nCurCount<0)
		nCurCount=pLyricsArr->GetSize();

	for(int i=1;i<=nCurCount-1;i++)
	{
		if(LyricsLine.nMilliseconds > pLyricsArr->GetAt(i-1).nMilliseconds
			&& LyricsLine.nMilliseconds <= pLyricsArr->GetAt(i).nMilliseconds)
		{
			pLyricsArr->InsertAt(i,LyricsLine);
			return i;
		}
	}
	pLyricsArr->Add(LyricsLine);
	return nCurCount;
}

//////////////////////////////////////////////////////////////////////
MTagHeader::MTagHeader()
{
	ASSERT(sizeof(HEADER)==10);
	m_bAvailable=FALSE;
}

BOOL MTagHeader::ReadFromFile(LPCTSTR lpszFileName)
{
	CFile mfile;
	if(!mfile.Open(lpszFileName,CFile::modeRead))
	{
		m_bAvailable=FALSE;
		return FALSE;
	}

	mfile.SeekToBegin();
	mfile.Read(&m_TagHeaderInfo,sizeof(HEADER));
	mfile.Close();

	if(strcmpx("ID3",m_TagHeaderInfo.chID,3)!=0)
	{
		m_bAvailable=FALSE;
		return FALSE;
	}

	m_nTagSize= m_TagHeaderInfo.aSize[0] * (1<<21)
		+ m_TagHeaderInfo.aSize[1] * (1<<14)
		+ m_TagHeaderInfo.aSize[2] * (1<<7)
		+ m_TagHeaderInfo.aSize[3] ;

	m_bAvailable=TRUE;
	return TRUE;
}

BOOL MTagHeader::GetTagInfo(TAGHEADERINFO &taginfo)
{
	if(!m_bAvailable)
		return FALSE;

	taginfo.iFlags=m_TagHeaderInfo.iFlags;
	taginfo.iMajorVersion=m_TagHeaderInfo.iMajorVersion;
	taginfo.iReVersion=m_TagHeaderInfo.iReVersion;
	taginfo.nTagSize=m_nTagSize;

	return TRUE;
}

void MTagHeader::MakeSize(BYTE *pSizeArr,LONG nSize)
{
	ASSERT(nSize>=0);

	pSizeArr[3]=(unsigned char)nSize;
	pSizeArr[3] &= 0x7F; //01111111
	nSize>>=7;

	pSizeArr[2]=(unsigned char)nSize;
	pSizeArr[2] &= 0x7F; //01111111
	nSize>>=7;

	pSizeArr[1]=(unsigned char)nSize;
	pSizeArr[1] &= 0x7F; //01111111
	nSize>>=7;

	pSizeArr[0]=(unsigned char)nSize;
	pSizeArr[0] &= 0x7F; //01111111
}

/////////////////////////////////////////////////////////////
BOOL MTagFooter::ReadFromFile(LPCTSTR lpszFileName)
{
	CFile mfile;
	if(!mfile.Open(lpszFileName,CFile::modeRead))
	{
		m_bAvailable=FALSE;
		return FALSE;
	}

	mfile.SeekToBegin();
	mfile.Read(&m_TagHeaderInfo,sizeof(HEADER));
	mfile.Close();

	if(strcmpx("3DI",m_TagHeaderInfo.chID,3)!=0)
	{
		m_bAvailable=FALSE;
		return FALSE;
	}

	m_nTagSize= m_TagHeaderInfo.aSize[0] * (1<<21)
		+ m_TagHeaderInfo.aSize[1] * (1<<14)
		+ m_TagHeaderInfo.aSize[2] * (1<<7)
		+ m_TagHeaderInfo.aSize[3] ;

	m_bAvailable=TRUE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////
MTagFrame::MTagFrame()
{
	m_bAvailable=FALSE;
}

BOOL MTagFrame::Read(CFile &file,LONG nPosStart)
{
	file.Seek(nPosStart,CFile::begin);
	if(file.Read(&m_BasicFrameInfo,sizeof(FRAME))<sizeof(FRAME))
	{
		m_bAvailable=FALSE;
		return FALSE;
	}

	strncpy(m_FrameInfo.chID,m_BasicFrameInfo.chID,4);
	m_FrameInfo.wFlags=m_BasicFrameInfo.wFlags;
	m_FrameInfo.nSize=getsizex(m_BasicFrameInfo.aSize);

	m_bAvailable=TRUE;

	return TRUE;
}
BOOL MTagFrame::Read(void *pMem,LONG nPosStart)
{
	if(pMem==NULL)
	{
		m_bAvailable=FALSE;
		return FALSE;
	}

	memcpy(&m_BasicFrameInfo,((char*)pMem)+nPosStart,sizeof(FRAME));

	strncpy(m_FrameInfo.chID,m_BasicFrameInfo.chID,4);
	m_FrameInfo.wFlags=m_BasicFrameInfo.wFlags;
	m_FrameInfo.nSize=getsizex(m_BasicFrameInfo.aSize);

	m_bAvailable=TRUE;
	return TRUE;
}

BOOL MTagFrame::GetFrameInfo(FRAMEINFO &frameinfo)
{
	if(!m_bAvailable)
		return FALSE;

	strncpy(frameinfo.chID,m_FrameInfo.chID,4);
	frameinfo.nSize=m_FrameInfo.nSize;
	frameinfo.wFlags=m_BasicFrameInfo.wFlags;

	return TRUE;
}
void MTagFrame::GetFrameInfo(FRAMEINFO &finfo,const FRAME &m_Frame)
{
	strncpy(finfo.chID,m_Frame.chID,4);
	finfo.nSize=getsizex(m_Frame.aSize);
	finfo.wFlags=m_Frame.wFlags;
}

BOOL MTagFrame::IsFrameType(LPCTSTR lpszFrameID)
{
	if(!m_bAvailable)
		return FALSE;

	if(strcmpx(lpszFrameID,m_BasicFrameInfo.chID,4)==0)
		return TRUE;

	return FALSE;
}
LONG MTagFrame::GetFrameSize()
{
	if(!m_bAvailable)
		return 0;

	return m_FrameInfo.nSize;
}
BOOL MTagFrame::IsFrame()
{
	if(isformat(m_BasicFrameInfo.chID,"%C"))
		return TRUE;
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////
MTag::MTag()
{
	m_pTag=NULL;
	m_nSize=0;
}
MTag::~MTag()
{
	if(m_pTag!=NULL)
	{
		free(m_pTag);
		m_pTag=NULL;
		m_nSize=0;
	}
}

BOOL MTag::Read(LPCTSTR lpszFileName)
{
	MTagHeader tagHeader;

	if(!tagHeader.ReadFromFile(lpszFileName))
		return FALSE;

	TAGHEADERINFO headerInfo;
	tagHeader.GetTagInfo(headerInfo);

	if(m_pTag!=NULL)
	{
		free(m_pTag);
		m_pTag=NULL;
		m_nSize=0;
	}

	m_pTag=malloc(headerInfo.nTagSize+10);
	if(m_pTag==NULL)
		return FALSE;
	m_nSize=headerInfo.nTagSize+10;

	CFile file;
	file.Open(lpszFileName,CFile::modeRead);

	file.SeekToBegin();
	file.Read(m_pTag,m_nSize);

	file.Close();

	return TRUE;
}

BOOL MTag::ModifyFrame(LPCTSTR lpszTargetFrameID,void *pScMem,LONG nScMemSize)
{
	if(m_pTag==NULL || m_nSize<10)
		return FALSE;
	if(pScMem==NULL || nScMemSize<=0)
		return FALSE;

	ModifySYLT(lpszTargetFrameID,pScMem,nScMemSize);
	return TRUE;
}
LONG MTag::SearchFrame(LPCTSTR lpszFrameID)
{
	if(m_pTag==NULL || m_nSize<=10)
		return -1;

	LONG nPosStart=10;
	
	//Get header for later use to indicate the start location of frames
	HEADER tagHeader;
	memcpy(&tagHeader,m_pTag,sizeof(HEADER));

	if(tagHeader.bExtendedHeader)
	{
		// Add code here ......

		// Get Extended header size
		// Then add to nPosStart to obtain the start location of frames
		;
	}

	MTagFrame frame;
	while(nPosStart<m_nSize)
	{
		frame.Read(m_pTag,nPosStart);
		if(!frame.IsFrame())
			return -1;
		if(frame.IsFrameType(lpszFrameID))
		{
			return nPosStart;
			break;
		}
		nPosStart+=frame.GetFrameSize();
		nPosStart+=sizeof(FRAME);
	}
	return -1;
}
LONG MTag::SearchFrame(LPCTSTR lpszFrameID,void *pMem,LONG nMemSize)
{
	if(pMem==NULL || nMemSize<=10)
		return -1;

	LONG nPosStart=10;
	
	//Get header for later use to indicate the start location of frames
	HEADER tagHeader;
	memcpy(&tagHeader,pMem,sizeof(HEADER));

	if(tagHeader.bExtendedHeader)
	{
		// Add code here ......

		// Get Extended header size
		// Then add to nPosStart to obtain the start location of frames
		;
	}

	MTagFrame frame;
	while(nPosStart<nMemSize)
	{
		frame.Read(pMem,nPosStart);
		if(frame.IsFrameType(lpszFrameID))
		{
			return nPosStart;
			break;
		}
		else if(!frame.IsFrame())
		{
			return -1;
		}
		nPosStart+=frame.GetFrameSize();
		nPosStart+=sizeof(FRAME);
	}
	return -1;
}
void* MTag::GetDataPointer()
{
	return m_pTag;
}
LONG MTag::GetTagSize()
{
	return m_nSize;
}
BOOL MTag::IsValid(LPCTSTR lpszFileName)
{
	CFile file;
	CFileException fe;
	if(!file.Open(lpszFileName,CFile::modeRead,&fe))
		return FALSE;
	char ch[5];
	byte uch[5];
	file.Read(ch,5);
	file.Close();
	memcpy(uch,ch,5);
	if( uch[0]==0xFF && uch[1]==0xFB )
		return TRUE;
	else if( ch[0]=='I' && ch[1]=='D' && ch[2]=='3' )
		return TRUE;
	return FALSE;
}
BOOL MTag::ModifySYLT(LPCTSTR lpszTargetFrameID,void *pScMem,LONG nScMemSize)
{
	LONG nPosTarget=SearchFrame(lpszTargetFrameID);
	if(nPosTarget>0)
	{
		HEADER header;
		memcpy(&header,m_pTag,sizeof(HEADER));

		// The following code perform the Modifying
		FRAME frame;
		FRAMEINFO frameinfo;
		memcpy(&frame,((char*)m_pTag)+nPosTarget,sizeof(FRAME));
		MTagFrame::GetFrameInfo(frameinfo,frame);

		void *pPerData;
		void *pNxtData;
		LONG nPreSize;
		LONG nNxtSize;

		//Save previous data
		nPreSize=nPosTarget;
		pPerData=malloc(nPreSize);
		memcpy(pPerData,m_pTag,nPreSize);

		//Save following data
		nNxtSize=m_nSize-(nPreSize+sizeof(FRAME)+frameinfo.nSize);
		pNxtData=malloc(nNxtSize);
		memcpy(pNxtData,((char*)m_pTag)+nPreSize+sizeof(FRAME)+frameinfo.nSize,nNxtSize);

		//Modify Size information in Tag_Header
		MTagHeader::MakeSize(header.aSize,nPreSize+nScMemSize+nNxtSize-sizeof(HEADER));
		memcpy(pPerData,&header,sizeof(HEADER));

		//Allocate new memory to fit new data
		free(m_pTag);
		m_nSize=nPreSize+nScMemSize+nNxtSize;
		m_pTag=malloc(m_nSize);

		//Make new data
		memcpy(m_pTag,pPerData,nPreSize);
		memcpy(((char*)m_pTag)+nPreSize,pScMem,nScMemSize);
		memcpy(((char*)m_pTag)+nPreSize+nScMemSize,pNxtData,nNxtSize);

		//free allocated template memory
		free(pPerData);
		free(pNxtData);
	}
	else if(nPosTarget<0)  //No SYLT present
	{
		HEADER header;
		memcpy(&header,m_pTag,sizeof(HEADER));
		nPosTarget=10;//getsize(header.aSize)+sizeof(HEADER);

		// The following code perform the Modifying
		void *pPerData;
		void *pNxtData;
		LONG nPreSize;
		LONG nNxtSize;

		//Save previous data
		nPreSize=nPosTarget;
		pPerData=malloc(nPreSize);
		memcpy(pPerData,m_pTag,nPreSize);

		//Save following data
		nNxtSize=m_nSize-nPreSize;
		pNxtData=malloc(nNxtSize);
		memcpy(pNxtData,((char*)m_pTag)+nPreSize,nNxtSize);

		//Modify Size information in Tag_Header
		MTagHeader::MakeSize(header.aSize,nPreSize+nScMemSize+nNxtSize-sizeof(HEADER));
		memcpy(pPerData,&header,sizeof(HEADER));

		//Allocate new memory to fit new data
		free(m_pTag);
		m_nSize=nPreSize+nScMemSize+nNxtSize;
		m_pTag=malloc(m_nSize);

		//Make new data
		memcpy(m_pTag,pPerData,nPreSize);
		memcpy(((char*)m_pTag)+nPreSize,pScMem,nScMemSize);
		memcpy(((char*)m_pTag)+nPreSize+nScMemSize,pNxtData,nNxtSize);

		//free allocated template memory
		free(pPerData);
		free(pNxtData);
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}
BOOL MTag::ModifyUSLT(LPCTSTR lpszTargetFrameID,void *pScMem,LONG nScMemSize)
{

	LONG nPosTarget=SearchFrame(lpszTargetFrameID);
	if(nPosTarget>0)
	{
		HEADER header;
		memcpy(&header,m_pTag,sizeof(HEADER));

		// The following code perform the Modifying
		FRAME frame;
		FRAMEINFO frameinfo;
		memcpy(&frame,((char*)m_pTag)+nPosTarget,sizeof(FRAME));
		MTagFrame::GetFrameInfo(frameinfo,frame);

		void *pPerData;
		void *pNxtData;
		LONG nPreSize;
		LONG nNxtSize;

		//Save previous data
		nPreSize=nPosTarget;
		pPerData=malloc(nPreSize);
		memcpy(pPerData,m_pTag,nPreSize);

		//Save following data
		nNxtSize=m_nSize-(nPreSize+sizeof(FRAME)+frameinfo.nSize);
		pNxtData=malloc(nNxtSize);
		memcpy(pNxtData,((char*)m_pTag)+nPreSize+sizeof(FRAME)+frameinfo.nSize,nNxtSize);

		//Modify Size information in Tag_Header
		MTagHeader::MakeSize(header.aSize,nPreSize+nScMemSize+nNxtSize-sizeof(HEADER));
		memcpy(pPerData,&header,sizeof(HEADER));

		//Allocate new memory to fit new data
		free(m_pTag);
		m_nSize=nPreSize+nScMemSize+nNxtSize;
		m_pTag=malloc(m_nSize);

		//Make new data
		memcpy(m_pTag,pPerData,nPreSize);
		memcpy(((char*)m_pTag)+nPreSize,pScMem,nScMemSize);
		memcpy(((char*)m_pTag)+nPreSize+nScMemSize,pNxtData,nNxtSize);

		//free allocated template memory
		free(pPerData);
		free(pNxtData);
	}
	else if(nPosTarget<0)  //No SYLT present
	{
		HEADER header;
		memcpy(&header,m_pTag,sizeof(HEADER));
		nPosTarget=getsize(header.aSize)+sizeof(HEADER);

		// The following code perform the Modifying
		void *pPerData;
		void *pNxtData;
		LONG nPreSize;
		LONG nNxtSize;

		//Save previous data
		nPreSize=nPosTarget;
		pPerData=malloc(nPreSize);
		memcpy(pPerData,m_pTag,nPreSize);

		//Save following data
		nNxtSize=m_nSize-nPreSize;
		pNxtData=malloc(nNxtSize);
		memcpy(pNxtData,((char*)m_pTag)+nPreSize,nNxtSize);

		//Modify Size information in Tag_Header
		MTagHeader::MakeSize(header.aSize,nPreSize+nScMemSize+nNxtSize-sizeof(HEADER));
		memcpy(pPerData,&header,sizeof(HEADER));

		//Allocate new memory to fit new data
		free(m_pTag);
		m_nSize=nPreSize+nScMemSize+nNxtSize;
		m_pTag=malloc(m_nSize);

		//Make new data
		memcpy(m_pTag,pPerData,nPreSize);
		memcpy(((char*)m_pTag)+nPreSize,pScMem,nScMemSize);
		memcpy(((char*)m_pTag)+nPreSize+nScMemSize,pNxtData,nNxtSize);

		//free allocated template memory
		free(pPerData);
		free(pNxtData);
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}
void MTag::Reset()
{
	m_nSize=0;
	if(m_pTag)
	{
		free(m_pTag);
	}
	m_pTag=NULL;
}
void MTag::CreateEmpty()
{
	Release();

	m_pTag=malloc(sizeof(HEADER)+sizeof(FRAME)*3);
	m_nSize=sizeof(HEADER)+sizeof(FRAME)*3;
	gensize(((HEADER*)m_pTag)->aSize,sizeof(FRAME)*3);
	strncpy(((HEADER*)m_pTag)->chID,"ID3",3);
	((HEADER*)m_pTag)->iFlags=0;
	((HEADER*)m_pTag)->iMajorVersion=3;
	((HEADER*)m_pTag)->iReVersion=0;
	((HEADER*)m_pTag)->iFlags=0;

	FRAME frame;
	gensizex(frame.aSize,0);
	strncpy(frame.chID,"WOAF",4);
	frame.wFlags=0;
	memcpy(((char*)m_pTag)+sizeof(HEADER),&frame,sizeof(FRAME));

	gensizex(frame.aSize,0);
	strncpy(frame.chID,"WOAR",4);
	frame.wFlags=0;
	memcpy(((char*)m_pTag)+sizeof(HEADER)+sizeof(FRAME),&frame,sizeof(FRAME));

	gensizex(frame.aSize,0);
	strncpy(frame.chID,"WOAS",4);
	frame.wFlags=0;
	memcpy(((char*)m_pTag)+sizeof(HEADER)+sizeof(FRAME)*2,&frame,sizeof(FRAME));

}
void MTag::Release()
{
	if(m_pTag)
	{
		free(m_pTag);
		m_pTag=NULL;
		m_nSize=0;
	}
}
/////////////////////////////////////////////////////////////////////////////////////
MTagSYLTFrame::MTagSYLTFrame()
{
	m_nSize=0;
	m_pFrame=NULL;
	m_nLines=0;
	m_lpszTitle=NULL;
	m_lpszLanguage=new char[3];
	m_bShort=0;
	strncpy(m_lpszLanguage,"chi",3);
}
MTagSYLTFrame::~MTagSYLTFrame()
{
	if(m_pFrame!=NULL)
	{
		free(m_pFrame);
		m_pFrame=NULL;
		m_nSize=0;
	}
	if(m_lpszTitle)
	{
		free(m_lpszTitle);
		m_lpszTitle=NULL;
	}
	if(m_lpszLanguage)
	{
		delete m_lpszLanguage;
		m_lpszLanguage=NULL;
	}
}
BOOL MTagSYLTFrame::FromLRCFile(LPCTSTR lpszLRCFileName)
{
	CFile lrcFile;
	if(!lrcFile.Open(lpszLRCFileName,CFile::modeRead))
		return FALSE;

	CFileStatus fs;
	lrcFile.GetStatus(fs);
	void *p=malloc(fs.m_size+2);

	lrcFile.Read(p,fs.m_size);
	((char*)p)[fs.m_size]=((char*)p)[fs.m_size+1]=0;

	FromLRCFormat((LPCTSTR)p);

	free(p);
	return TRUE;
}
BOOL MTagSYLTFrame::FromLRCFormat(LPCTSTR lpszText)
{
	LONG nPos=0;
	LONG nStep=0;
	char *pch=new char[1024];
	nStep=getline(pch,lpszText+nPos);

	m_Lyrics.RemoveAll();
	m_nLines=0;

	LONG len=strlen(lpszText);
	while(nStep>=0 && nPos<=len)
	{
		if(nStep==0)
		{
			// nPos+=nStep;
			nPos+=nStep+2;
			nStep=getline(pch,lpszText+nPos);
			continue;
		}
		if(pch[0]=='[')
		{
			if(strcmpxi(pch+1,"ti:",3)==0) //Title
			{
			}
			else if(strcmpxi(pch+1,"ar:",3)==0) //
			{
			}
			else if(strcmpxi(pch+1,"al:",3)==0) //
			{
			}
			else if(strcmpxi(pch+1,"by:",3)==0) //
			{
			}
			else
			{
				int det=0;
				// Time stamp format
				if(isformat(pch,"[%2N:%2N.%2N]"))
				{
					det=9;
					m_bShort=0;
				}
				else if(isformat(pch,"[%2N:%2N]"))
				{
					det=6;
					m_bShort=1;
				}
				else
					det=-1;
				if(det>0)
				{
					int lsted;
					char *pfsted=NULL;
					lsted=formatfindr(pch,"%2N]");

					pfsted=strchr(pch,']');
					LYRICSLINE lrc;
					strcpy(lrc.chText,pch+lsted);
					int len=strlen(lrc.chText);
					lrc.chText[len-1]=lrc.chText[len-2]=0;
					if(strlen(lrc.chText)==0)
						strcpy(lrc.chText," ");

					while(pfsted!=NULL)
					{
						lrc.nMilliseconds=getmilliseconds(pfsted-det);
						InsertToArray(&m_Lyrics,lrc,m_nLines);
						m_nLines++;
						pfsted=strchr(pfsted+1,']');
					}
				}
			}
		}
		nPos+=nStep+2;
		if(nPos<len)
			nStep=getline(pch,lpszText+nPos);
		else
			nStep=-1;
	}

	delete []pch;

	UpdateFrameData();
	return TRUE;
}
LONG MTagSYLTFrame::FromTagData(void *pMem,LONG nSize)
{
	if(pMem==NULL)
		return 0;

	LONG frameOffset=MTag::SearchFrame("SYLT",pMem,nSize);
	if(frameOffset<0)
		return -1;

	pMem=((char*)pMem)+frameOffset;

	m_nLines=0;
	m_Lyrics.RemoveAll();

	FRAME frame;
	memcpy(&frame,pMem,sizeof(FRAME));
	if(m_pFrame!=NULL)
	{
		free(m_pFrame);
		m_pFrame=NULL;
		m_nSize=0;
	}

	LONG nFrameSize=getsizex(frame.aSize);
	m_nSize=nFrameSize+sizeof(FRAME);
	m_pFrame=malloc(m_nSize);
	memcpy(m_pFrame,pMem,nFrameSize+sizeof(FRAME));
	memcpy(&m_Header,((char*)pMem)+sizeof(FRAME),sizeof(SYLTHEADER));

	// Load data into Lyrics list
	memcpy(&frame,m_pFrame,sizeof(FRAME));
	void *p=((char*)m_pFrame)+sizeof(FRAME);
	SYLTHEADER sylt;
	memcpy(&sylt,p,sizeof(sylt));
	p=((char*)p)+sizeof(SYLTHEADER);
	LONG nSizeLeft=getsizex(frame.aSize)-sizeof(SYLTHEADER);
	switch(sylt.iTextEncoding)
	{
	case 0: //ANSI ISO-8859-1
		{
			int desclen=getstring(NULL,(char*)p);
			LONG nCurRead=0;
			p=((char*)p)+desclen+1;
			nSizeLeft-=(desclen+1);

			char *pch=(char*)malloc(1000);
			LONG nPerRead=0;
			BYTE aTime[4];
			LYRICSLINE lyrics;
			while(nCurRead<nSizeLeft)
			{
				nPerRead=getstring(pch,(char*)p+nCurRead);
				nCurRead+=(nPerRead+1);
				memcpy(aTime,(char*)p+nCurRead,sizeof(aTime));
				nCurRead+=sizeof(aTime);

				ReturnToText(pch);
				strcpy(lyrics.chText,pch);
				lyrics.nMilliseconds=getsizex(aTime);
				InsertToArray(&m_Lyrics,lyrics,m_nLines);
				m_nLines++;
			}
			free(pch);
			break;
		}
	case 1: //UNICODE UTF-16
		{
			int desclen=getstringw(NULL,(WCHAR*)p);
			LONG nCurRead=0;
			p=((WCHAR*)p)+desclen+1;
			nSizeLeft-=((desclen+1)*2);

			WCHAR *pch=new WCHAR[1000];
			char *pchb=new char[1000];
			LONG nPerRead=0;
			BYTE aTime[4];
			LYRICSLINE lyrics;
			while(nCurRead<nSizeLeft)
			{
				nPerRead=getstringw(pch,((WCHAR*)((char*)p+nCurRead)));
				nCurRead+=(nPerRead+1)*2;
				memcpy(aTime,(char*)p+nCurRead,sizeof(aTime));
				nCurRead+=sizeof(aTime);

				WideCharToMultiByte(CP_ACP,0,pch,-1,pchb,1000,NULL,NULL);
				ReturnToText(pchb);
				strcpy(lyrics.chText,pchb);
				lyrics.nMilliseconds=getsizex(aTime);
				InsertToArray(&m_Lyrics,lyrics,m_nLines);
				m_nLines++;
			}
			delete []pchb;
			delete []pch;
			break;
		}
	case 2: //UNICODE UTF-16BE
	case 3: //UNICODE UTF-8
		;
	}


	return nFrameSize;
}

BOOL MTagSYLTFrame::PutToEditBox(CEdit *pEdit)
{
	CString str;
	PutToString(str);
	pEdit->SetWindowText(str);

	return TRUE;
}
BOOL MTagSYLTFrame::PutToString(CString &str)
{
	CString strTmp;
	str.Empty();
	for(LONG i=1;i<=m_nLines;i++)
	{
		strTmp.Format("[%.2d:%.2d.%.2d]%s%c%c",m_Lyrics[i-1].nMilliseconds/60000,(m_Lyrics[i-1].nMilliseconds/1000)%60,(m_Lyrics[i-1].nMilliseconds%1000)/10,m_Lyrics[i-1].chText,13,10);
		str+=strTmp;
	}

	return TRUE;
}
BOOL MTagSYLTFrame::UpdateFrameData()
{
	LONG nContentSize=m_nLines * sizeof(LONG32);  // Size of Time stamps
	WCHAR *markwch;
	LONG marklen=0;
	BOOL bTitleCreate=FALSE;

	if(m_lpszTitle==NULL)
	{
		m_lpszTitle=(LPTSTR)malloc(80);
		strcpy(m_lpszTitle,"Lyrics Integrated with LyricsEditor by XiaHuan.");
		bTitleCreate=TRUE;
	}
	marklen=MultiByteToWideChar(CP_ACP,0,m_lpszTitle,-1,NULL,0)*2;
	markwch=(WCHAR*)malloc(marklen);
	MultiByteToWideChar(CP_ACP,0,m_lpszTitle,-1,markwch,marklen);

	if(bTitleCreate)
	{
		bTitleCreate=FALSE;
		free(m_lpszTitle);
		m_lpszTitle=NULL;
	}

	for(LONG i=1;i<=m_nLines;i++)
	{
		nContentSize+=MultiByteToWideChar(CP_ACP,0,m_Lyrics[i-1].chText,-1,NULL,0)*2;
	}

	nContentSize+=marklen;  //Mark


	FRAME frame;
	strncpy(frame.chID,"SYLT",4);
	gensizex(frame.aSize,nContentSize+sizeof(SYLTHEADER));
	frame.wFlags=0;
	
	SYLTHEADER header;
	header.iContentType=1; //Lyrics
	header.iTextEncoding=1; //UTF-16
	strncpy(header.chLanguage,m_lpszLanguage,3);
	header.iTimeStampFormat=2; //Absolute time, 32 bit sized, using milliseconds as unit

	if(m_pFrame!=NULL)
	{
		free(m_pFrame);
		m_pFrame=NULL;
		m_nSize=0;
	}

	m_nSize=sizeof(FRAME)+sizeof(SYLTHEADER)+nContentSize;
	m_pFrame=malloc(m_nSize);

	LONG nCurPos=0;
	memcpy(m_pFrame,&frame,sizeof(FRAME));
	nCurPos+=sizeof(FRAME);
	memcpy(((char*)m_pFrame)+nCurPos,&header,sizeof(SYLTHEADER));
	nCurPos+=sizeof(SYLTHEADER);


	memcpy(((char*)m_pFrame)+nCurPos,markwch,marklen);
	nCurPos+=(marklen);

	BYTE aSize[4];
	WCHAR *pchw=new WCHAR[1002];

	for(i=1;i<=m_nLines;i++)
	{
		MultiByteToWideChar(CP_ACP,0,m_Lyrics[i-1].chText,-1,pchw,1000);
		TextToReturnW(pchw);
		memcpy(((char*)m_pFrame)+nCurPos,pchw,(wcslen(pchw)+1)*2);
		nCurPos+=(wcslen(pchw)+1)*2;
		gensizex(aSize,m_Lyrics[i-1].nMilliseconds);
		memcpy(((char*)m_pFrame)+nCurPos,aSize,sizeof(LONG32));
		nCurPos+=sizeof(LONG32);
	}

	ASSERT(nCurPos==m_nSize);
	delete []pchw;
	free(markwch);
	return TRUE;
}
void* MTagSYLTFrame::GetFrameData()
{
	return m_pFrame;
}
LONG MTagSYLTFrame::GetFrameSize()
{
	return m_nSize;
}
BOOL MTagSYLTFrame::ApplyOffset(LONG nOffsetMilliseconds)
{
	if(m_nLines<=0)
		return FALSE;
	for(LONG i=1;i<=m_nLines;i++)
	{
		if(m_Lyrics[i-1].nMilliseconds==0)
			continue;
		if(m_Lyrics[i-1].nMilliseconds+nOffsetMilliseconds<0)
			m_Lyrics[i-1].nMilliseconds=0;
		else
			m_Lyrics[i-1].nMilliseconds+=nOffsetMilliseconds;
	}
	UpdateFrameData();
	return TRUE;
}
void MTagSYLTFrame::MediaPlayerPatch(double fPatchRate)
{
	for(LONG i=1;i<=m_nLines;i++)
	{
		m_Lyrics[i-1].nMilliseconds+= (LONG) (m_Lyrics[i-1].nMilliseconds * fPatchRate);
	}
	UpdateFrameData();
}
void MTagSYLTFrame::SetTitleText(LPCTSTR lpszTitle)
{
	if(m_lpszTitle)
	{
		free(m_lpszTitle);
		m_lpszTitle=NULL;
	}

	m_lpszTitle=(LPTSTR)malloc(_tcslen(lpszTitle)+sizeof(TCHAR));

	_tcscpy(m_lpszTitle,lpszTitle);
	UpdateFrameData();
}
void MTagSYLTFrame::SetLanguage(LPCTSTR lpszLanguage)
{
	strncpy(m_lpszLanguage,lpszLanguage,3);
}
void MTagSYLTFrame::GetLanguage(LPTSTR lpszLanguageBuf)
{
	strncpy(lpszLanguageBuf,m_lpszLanguage,3);
	lpszLanguageBuf[3]='\0';
}

///////////////////////////////////////////////////////////////////////
MTagUSLTFrame::MTagUSLTFrame()
{
	m_pFrame=NULL;
	m_nSize=0;
	m_pText=NULL;
	m_nTextLength=0;
	m_lpszTitle=NULL;
	m_lpszLanguage=new char[3];
	strncpy(m_lpszLanguage,"chi",3);
}
MTagUSLTFrame::~MTagUSLTFrame()
{
	if(m_pFrame!=NULL)
	{
		free(m_pFrame);
		m_pFrame=NULL;
		m_nSize=0;
	}
	if(m_pText!=NULL)
	{
		free(m_pText);
		m_pText=NULL;
		m_nTextLength=0;
	}
	if(m_lpszTitle)
	{
		free(m_lpszTitle);
		m_lpszTitle=NULL;
	}
	if(m_lpszLanguage)
		delete m_lpszLanguage;
}
LONG MTagUSLTFrame::FromTagData(void *pMem,LONG nSize)
{
	if(pMem==NULL)
		return 0;

	LONG frameOffset=MTag::SearchFrame("USLT",pMem,nSize);
	if(frameOffset<0)
		return 0;

	pMem=((char*)pMem)+frameOffset;

	FRAME frame;
	memcpy(&frame,pMem,sizeof(FRAME));
	if(m_pFrame!=NULL)
	{
		free(m_pFrame);
		m_pFrame=NULL;
		m_nSize=0;
	}

	LONG nFrameSize=getsizex(frame.aSize);
	m_nSize=nFrameSize+sizeof(FRAME);
	m_pFrame=malloc(m_nSize);
	memcpy(m_pFrame,pMem,nFrameSize+sizeof(FRAME));
	memcpy(&m_Header,((char*)pMem)+sizeof(FRAME),sizeof(USLTHEADER));

	// Load data into Lyrics list
	memcpy(&frame,m_pFrame,sizeof(FRAME));
	void *p=((char*)m_pFrame)+sizeof(FRAME);
	USLTHEADER uslt;
	memcpy(&uslt,p,sizeof(uslt));
	p=((char*)p)+sizeof(USLTHEADER);
	LONG nSizeLeft=getsizex(frame.aSize)-sizeof(USLTHEADER);
	switch(uslt.iTextEncoding)
	{
	case 0: //ANSI ISO-8859-1
		{
			int desclen=getstring(NULL,(char*)p);
			LONG nCurRead=0;
			p=((char*)p)+desclen+1;
			nSizeLeft-=(desclen+1);

			if(m_pText)
			{
				free(m_pText);
				m_pText=NULL;
				m_nTextLength=0;
			}

			m_pText=malloc(nSizeLeft+1);
			m_nTextLength=nSizeLeft+1;
			memcpy(m_pText,p,nSizeLeft);
			((char*)m_pText)[nSizeLeft]=0;
			break;
		}
	case 1: //UNICODE UTF-16
		{
			int desclen=getstringw(NULL,(WCHAR*)p);
			LONG nCurRead=0;
			p=((WCHAR*)p)+desclen+1;
			nSizeLeft-=((desclen+1)*2);


			if(m_pText)
			{
				free(m_pText);
				m_pText=NULL;
				m_nTextLength=0;
			}

			WCHAR *pch=new WCHAR[nSizeLeft/2+1];

			memcpy(pch,p,nSizeLeft);
			((WCHAR*)pch)[nSizeLeft/2]=0;

			int lenNeed=WideCharToMultiByte(CP_ACP,0,pch,-1,NULL,0,NULL,NULL);
			m_pText=malloc(lenNeed+1);
			m_nTextLength=lenNeed+1;

			WideCharToMultiByte(CP_ACP,0,pch,-1,(char*)m_pText,lenNeed+1,NULL,NULL);
			delete []pch;
			break;
		}
	case 2: //UNICODE UTF-16BE
	case 3: //UNICODE UTF-8
		return 0;
	}

	return nFrameSize;
}
void* MTagUSLTFrame::GetFrameData()
{
	return m_pFrame;
}
LPCTSTR MTagUSLTFrame::GetTextPointer()
{
	return (LPCTSTR)m_pText;
}
LONG MTagUSLTFrame::GetTextLength()
{
	return strlen((char*)m_pText);
}
LONG MTagUSLTFrame::GetFrameSize()
{
	return m_nSize;
}
BOOL MTagUSLTFrame::PutToString(CString &str)
{
	if(m_pText==NULL)
		return FALSE;

	str=(LPCTSTR)m_pText;
	return TRUE;
}
BOOL MTagUSLTFrame::PutToEditBox(CEdit *pEdit)
{
	CString str;
	PutToString(str);
	pEdit->SetWindowText(str);
	return TRUE;
}
BOOL MTagUSLTFrame::FromLRCFile(LPCTSTR lpszLRCFileName)
{
	CFile lrcFile;
	if(!lrcFile.Open(lpszLRCFileName,CFile::modeRead))
		return FALSE;

	CFileStatus fs;
	lrcFile.GetStatus(fs);
	void *p=malloc(fs.m_size+2);

	lrcFile.Read(p,fs.m_size);
	((char*)p)[fs.m_size]=((char*)p)[fs.m_size+1]=0;

	FromLRCFormat((LPCTSTR)p);

	free(p);
	return TRUE;
}
BOOL MTagUSLTFrame::FromLRCFormat(LPCTSTR lpszText)
{
	LONG nPos=0;
	LONG nStep=0;
	char *pch=new char[1024];
	nStep=getline(pch,lpszText+nPos);

	LONG len=strlen(lpszText);
	CString str;
	while(nStep>=0 && nPos<=len)
	{
		if(nStep==0)
		{
			nPos+=nStep+2;
			nStep=getline(pch,lpszText+nPos);
			continue;
		}
		if(pch[0]=='[')
		{
			if(strcmpxi(pch+1,"ti:",3)==0) //Title
			{
			}
			else if(strcmpxi(pch+1,"ar:",3)==0) //
			{
			}
			else if(strcmpxi(pch+1,"al:",3)==0) //
			{
			}
			else if(strcmpxi(pch+1,"by:",3)==0) //
			{
			}
			else if(isformat(pch,"[%2N:%2N.%2N]") || isformat(pch,"[%2N:%2N]")) //Time stamp
			{
				int lsted;
				lsted=formatfindr(pch,"%2N]");
				str+=(LPCTSTR)(pch+lsted);
			}
		}
		nPos+=nStep+2;
		if(nPos<len)
			nStep=getline(pch,lpszText+nPos);
		else
			nStep=-1;
	}
	str.Replace("\\n"," - ");

	delete []pch;

	if(m_pText!=NULL)
	{
		free(m_pText);
		m_pText=NULL;
		m_nTextLength=0;
	}
	m_nTextLength=str.GetLength();
	m_pText=malloc(m_nTextLength+1);

	memcpy(m_pText,(LPCTSTR)str,m_nTextLength);
	((char*)m_pText)[m_nTextLength]=0;

	UpdateFrameData();
	return TRUE;
}
BOOL MTagUSLTFrame::UpdateFrameData()
{
	LONG factLen=0,nContentSize=0;
	WCHAR *markwch;
	LONG marklen=0;
	BOOL bTitleCreate=FALSE;

	if(m_lpszTitle==NULL)
	{
		m_lpszTitle=(LPTSTR)malloc(80);
		strcpy(m_lpszTitle,"Lyrics Integrated with LyricsEditor by XiaHuan.");
		bTitleCreate=TRUE;
	}
	marklen=MultiByteToWideChar(CP_ACP,0,m_lpszTitle,-1,NULL,0)*2;
	markwch=(WCHAR*)malloc(marklen);
	MultiByteToWideChar(CP_ACP,0,m_lpszTitle,-1,markwch,marklen);

	if(bTitleCreate)
	{
		bTitleCreate=FALSE;
		free(m_lpszTitle);
		m_lpszTitle=NULL;
	}


	nContentSize+=(MultiByteToWideChar(CP_ACP,0,(LPCTSTR)m_pText,-1,NULL,0)*2);
	factLen=nContentSize;

	nContentSize+=marklen;  //Mark


	FRAME frame;
	strncpy(frame.chID,"USLT",4);
	gensizex(frame.aSize,nContentSize+sizeof(USLTHEADER));
	frame.wFlags=0;
	
	USLTHEADER header;
	header.iTextEncoding=1; //UTF-16
	strncpy(header.chLanguage,m_lpszLanguage,3);

	if(m_pFrame!=NULL)
	{
		free(m_pFrame);
		m_pFrame=NULL;
		m_nSize=0;
	}

	m_nSize=sizeof(FRAME)+sizeof(USLTHEADER)+nContentSize;
	m_pFrame=malloc(m_nSize);

	LONG nCurPos=0;
	memcpy(m_pFrame,&frame,sizeof(FRAME));
	nCurPos+=sizeof(FRAME);
	memcpy(((char*)m_pFrame)+nCurPos,&header,sizeof(USLTHEADER));
	nCurPos+=sizeof(USLTHEADER);


	memcpy(((char*)m_pFrame)+nCurPos,markwch,marklen);
	nCurPos+=marklen;

	WCHAR *pchw=new WCHAR[factLen/2+1];

	MultiByteToWideChar(CP_ACP,0,(LPCTSTR)m_pText,-1,pchw,factLen/2+1);
	memcpy(((char*)m_pFrame)+nCurPos,pchw,factLen);
	nCurPos+=factLen;

	ASSERT(nCurPos==m_nSize);
	delete []pchw;
	free(markwch);

	return TRUE;
}
void MTagUSLTFrame::SetTitleText(LPCTSTR lpszTitle)
{
	if(m_lpszTitle)
	{
		free(m_lpszTitle);
		m_lpszTitle=NULL;
	}

	m_lpszTitle=(LPTSTR)malloc(_tcslen(lpszTitle)+sizeof(TCHAR));

	_tcscpy(m_lpszTitle,lpszTitle);
	UpdateFrameData();
}
void MTagUSLTFrame::SetLanguage(LPCTSTR lpszLanguage)
{
	strncpy(m_lpszLanguage,lpszLanguage,3);
}
void MTagUSLTFrame::GetLanguage(LPTSTR lpszLanguageBuf)
{
	strncpy(lpszLanguageBuf,m_lpszLanguage,3);
	lpszLanguageBuf[3]='\0';
}

#endif //_MP3READ_H_
