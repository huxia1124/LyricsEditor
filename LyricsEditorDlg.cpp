// LyricsEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LyricsEditor.h"
#include "LyricsEditorDlg.h"
#include "AdvancedDialog.h"
#include "MP3Read.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CRichLabel	m_stEmail;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_STATIC_EMAIL, m_stEmail);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLyricsEditorDlg dialog

CLyricsEditorDlg::CLyricsEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLyricsEditorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLyricsEditorDlg)
	m_strScFile = _T("");
	m_strLyrics = _T("");
	m_strInfo = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLyricsEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLyricsEditorDlg)
	DDX_Control(pDX, IDC_STATIC_INFO, m_stInfo);
	DDX_Control(pDX, IDC_EDIT_SOURCEFILE, m_edtScFile);
	DDX_Control(pDX, IDC_EDIT_LYRICS, m_edtLyrics);
	DDX_Text(pDX, IDC_EDIT_SOURCEFILE, m_strScFile);
	DDX_Text(pDX, IDC_EDIT_LYRICS, m_strLyrics);
	DDX_Text(pDX, IDC_STATIC_INFO, m_strInfo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLyricsEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CLyricsEditorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_GETLYRICS, OnButtonGetlyrics)
	ON_BN_CLICKED(IDC_BUTTON_BROWSESC, OnButtonBrowsesc)
	ON_BN_CLICKED(IDC_BUTTON_ADVANCED, OnButtonAdvanced)
	ON_BN_CLICKED(IDC_BUTTON_EXPORTTOLRC, OnButtonExporttolrc)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, OnButtonAbout)
	ON_BN_CLICKED(IDC_BUTTON_IMPORTLRC, OnButtonImportlrc)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLyricsEditorDlg message handlers

BOOL CLyricsEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	m_pToolTip=NULL;

	m_pToolTip=new CToolTipCtrl();
	if(m_pToolTip)
	{
		m_pToolTip->Create(this);
		m_pToolTip->AddTool(GetDlgItem(IDC_BUTTON_SAVE),"将歌词内容保存到指定文件中");
		m_pToolTip->AddTool(GetDlgItem(IDC_BUTTON_GETLYRICS),"从指定的MP3文件中读取同步歌词");
		m_pToolTip->AddTool(GetDlgItem(IDC_EDIT_LYRICS),"在这里编辑同步歌词");
		m_pToolTip->AddTool(GetDlgItem(IDC_BUTTON_IMPORTLRC),"读取LRC文件的内容到歌词编辑器中");
		m_pToolTip->AddTool(GetDlgItem(IDC_BUTTON_EXPORTTOLRC),"将歌词编辑器中的内容保存到一个外部LRC文件中");
		m_pToolTip->AddTool(GetDlgItem(IDC_EDIT_SOURCEFILE),"指定要进行处理的MP3文件");
	}

	m_info.bMediaPlayerPatch=1;
	m_info.fPatch=0.001;
	m_strLyricsTitle="Lyrics Integrated with LyricsEditor by XiaHuan.";
	m_stInfo.SetTextColor(RGB(200,0,200));
	m_nLyricsOffset=0;
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLyricsEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLyricsEditorDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLyricsEditorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLyricsEditorDlg::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	UpdateData(TRUE);
	int nFile=DragQueryFile(hDropInfo,0xFFFFFFFF,NULL,0);
	if(nFile>=1)
	{
		TCHAR chFileName[MAX_PATH];
		DragQueryFile(hDropInfo,0,chFileName,MAX_PATH);

		CRect rcScFile;
		CRect rcTgFile;
		CRect rcLyrics;

		m_edtLyrics.GetWindowRect(&rcLyrics);
		m_edtScFile.GetWindowRect(&rcScFile);

		CPoint point;
		DragQueryPoint(hDropInfo,&point);

		ClientToScreen(&point);

		if(rcScFile.PtInRect(point))
		{
			m_strScFile=chFileName;
//			GenTargetFile(m_strTgFile,chFileName);
		}
		else if(rcLyrics.PtInRect(point))
		{
			ReadFromTextFile(m_strLyrics,chFileName);
			m_info.bMediaPlayerPatch=TRUE;
		}
		UpdateData(FALSE);
	}
	
	CDialog::OnDropFiles(hDropInfo);
}

void CLyricsEditorDlg::GenTargetFile(CString &strTg, LPCTSTR lpszSc)
{
	strTg=lpszSc;
	if(!MTag::IsValid(lpszSc))
	{
		m_strInfo="无法识别源文件的格式! (Failed)";
		UpdateData(FALSE);
		return;
	}
	m_strInfo="";
	UpdateData(FALSE);
	int iExeAppend=strTg.ReverseFind('.');
	strTg.Insert(iExeAppend,"X");

}

BOOL CLyricsEditorDlg::ReadFromTextFile(CString &strTg, LPCTSTR lpszFileName)
{
	CFile file;
	if(!file.Open(lpszFileName,CFile::modeRead))
	{
		return FALSE;
	}

	CFileStatus fs;
	file.GetStatus(fs);

	if(fs.m_size>1024*1024)  //1M
	{
		return FALSE;
		file.Close();
	}

	void *p=malloc(fs.m_size+2);

	file.Read(p,fs.m_size);
	if(IsTextUnicode(p,fs.m_size,NULL))
	{
		((WCHAR*)p)[fs.m_size/2]=0;
		void *pa=malloc(fs.m_size+1);
		WideCharToMultiByte(CP_ACP,0,(WCHAR*)p,-1,(char*)pa,fs.m_size+1,NULL,NULL);
		strTg=(LPCTSTR)pa;
		free(pa);
	}
	else
	{
		((char*)p)[fs.m_size]=0;
		strTg=(LPCTSTR)p;
	}

	free(p);

	file.Close();
	return TRUE;
}

void CLyricsEditorDlg::OnButtonGetlyrics() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	m_strInfo="";

	CFileStatus fs;
	if(!CFile::GetStatus(m_strScFile,fs))
	{
		m_strInfo="无法打开源文件!\n\n请确定源文件存在并且未被其它程序访问! (Failed)";
		UpdateData(FALSE);
		MessageBox(m_strInfo,"ERROR",16);
		m_edtScFile.SetFocus();
		m_edtScFile.SetSel(0,-1);
		return;
	}



	if(!MTag::IsValid(m_strScFile))
	{
		m_strInfo="无法识别源文件的格式! (Failed)";
		UpdateData(FALSE);
		MessageBox(m_strInfo,"ERROR",16);
		return;
	}



	MTag tag;
	if(!tag.Read(m_strScFile))
	{
		m_strInfo="源文件中不含标准MP3文件信息!\n\n但该MP3文件可以加入歌词. (Succeed)";
		UpdateData(FALSE);
		MessageBox(m_strInfo,"WARNING",48);
		return;
	}

	MTagSYLTFrame sylt;

	if(sylt.FromTagData(tag.GetDataPointer(),tag.GetTagSize())==-1)
	{
		m_strInfo="没有找到同步歌词!";
		MessageBox(m_strInfo,"WARNING",48);
		UpdateData(FALSE);
		return;
	}

	sylt.PutToString(m_strLyrics);


	m_info.bMediaPlayerPatch=FALSE;

	UpdateData(FALSE);
}

void CLyricsEditorDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_strInfo="";

	CFileStatus fs;
	if(!CFile::GetStatus(m_strScFile,fs))
	{
		m_strInfo="无法打开源文件!\n\n请确定源文件存在并且未被其它程序访问! (Failed)";
		UpdateData(FALSE);
		MessageBox(m_strInfo,"ERROR",16);
		m_edtScFile.SetFocus();
		m_edtScFile.SetSel(0,-1);
		return;
	}

	if(!MTag::IsValid(m_strScFile))
	{
		m_strInfo="无法识别源文件的格式! 不能进行保存! (Failed)";
		UpdateData(FALSE);
		MessageBox(m_strInfo,"ERROR",16);
		return;
	}

	BeginWaitCursor();

	MTag tag;
	m_strInfo="";
	if(!tag.Read(m_strScFile))
	{
		m_strInfo+="源文件中没有找到ID3v2 Tag,目标文件中生成了一个ID3v2 Tag\n";
		tag.CreateEmpty();
//		return;
	}

	MTagSYLTFrame sylt;
	MTagUSLTFrame uslt;

	sylt.SetTitleText(m_strLyricsTitle);
	uslt.SetTitleText(m_strLyricsTitle);
	sylt.FromLRCFormat(m_strLyrics);
	uslt.FromLRCFormat(m_strLyrics);

	sylt.ApplyOffset(m_nLyricsOffset);
	
	if(m_info.bMediaPlayerPatch)
		sylt.MediaPlayerPatch(m_info.fPatch);

	tag.ModifyFrame("SYLT",sylt.GetFrameData(),sylt.GetFrameSize());
	if(m_info.bULyrics)
		tag.ModifyFrame("USLT",uslt.GetFrameData(),uslt.GetFrameSize());

//	WriteToFile(tag,m_strScFile,m_strTgFile);
	WriteToFile(tag,m_strScFile,NULL);
	m_strInfo+="完成! (Suceeded)";
	UpdateData(FALSE);
	MessageBox(m_strInfo,"ERROR",64);
	
	EndWaitCursor();
}

void CLyricsEditorDlg::OnButtonBrowsesc() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CFileDialog fd(TRUE,"*.MP3","*.MP3",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"*.MP3|*.MP3||");

	if(fd.DoModal()==IDOK)
	{
		m_strScFile=fd.GetPathName();
		if(!MTag::IsValid(m_strScFile))
		{
			m_strInfo="无法识别源文件的格式!  (Warning)";
		}
//		GenTargetFile(m_strTgFile,m_strScFile);

		UpdateData(FALSE);
	}

}

void CLyricsEditorDlg::OnButtonAdvanced() 
{
	// TODO: Add your control notification handler code here
	CAdvancedDialog ad;
	ad.m_bMediaPath=m_info.bMediaPlayerPatch;
	ad.m_bULyrics=m_info.bULyrics;
	ad.m_strLyricsTitle=m_strLyricsTitle;
	ad.m_nMsec=m_nLyricsOffset;
	ad.m_fPatch=m_info.fPatch;
	if(ad.DoModal()==IDOK)
	{
		m_info.bMediaPlayerPatch=(ad.m_bMediaPath?1:0);
		m_info.bULyrics=(ad.m_bULyrics?1:0);
		m_strLyricsTitle=ad.m_strLyricsTitle;
		m_nLyricsOffset=ad.m_nMsec;
		m_info.fPatch=ad.m_fPatch;
	}
}

void CLyricsEditorDlg::OnButtonExporttolrc() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_strInfo="";

	if(m_strLyrics.GetLength()<=0)
	{
		m_strInfo="没有歌词内容,不能导出文件!";
		UpdateData(FALSE);
		MessageBox(m_strInfo,"ERROR",16);
		return;
	}
	CFileDialog fd(FALSE,"*.LRC","*.LRC",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"*.LRC|*.LRC||");

	if(fd.DoModal()==IDOK)
	{
		CFile file;
		if(!file.Open(fd.GetPathName(),CFile::modeCreate|CFile::modeWrite))
		{
			m_strInfo="无法保存LRC文件!";
			UpdateData(FALSE);
			MessageBox(m_strInfo,"ERROR",16);
			return;
		}

		file.Write(m_strLyrics,m_strLyrics.GetLength());
		char c=0;
		file.Write(&c,1);
		file.Close();
	}

	UpdateData(FALSE);

}

void CLyricsEditorDlg::OnButtonAbout() 
{
	// TODO: Add your control notification handler code here
	CAboutDlg ad;
	ad.DoModal();
}

void CLyricsEditorDlg::OnButtonImportlrc() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_strInfo="";
	CFileDialog fd(TRUE,"*.LRC","*.LRC",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"*.LRC|*.LRC||");

	if(fd.DoModal()==IDOK)
	{
		CFile file;
		if(!file.Open(fd.GetPathName(),CFile::modeRead))
		{
			m_strInfo="无法打开指定的LRC文件!";
			UpdateData(FALSE);
			MessageBox(m_strInfo,"ERROR",16);
			return;
		}

		CFileStatus fs;

		file.GetStatus(fs);
		char *pch=(char*)malloc(fs.m_size+2);
		file.Read(pch,fs.m_size);
		pch[fs.m_size]=pch[fs.m_size+1]=0;

		m_strLyrics=pch;

		free(pch);
		UpdateData(FALSE);
	}
	else
	{
		UpdateData(FALSE);
		return;
	}
	m_info.bMediaPlayerPatch=TRUE;
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_stEmail.SetTextColor(RGB(0,0,255));
	m_stEmail.SetFontUnderline(TRUE);
	m_stEmail.SetLink(TRUE,TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CLyricsEditorDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pToolTip)
		m_pToolTip->RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}

void CLyricsEditorDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	if(m_pToolTip)
	{
		delete m_pToolTip;
		m_pToolTip=NULL;
	}
	
	// TODO: Add your message handler code here
	
}
