// LyricsEditorDlg.h : header file
//

#if !defined(AFX_LYRICSEDITORDLG_H__C6323AAA_8F06_462E_AE0D_9A4FACC12FB4__INCLUDED_)
#define AFX_LYRICSEDITORDLG_H__C6323AAA_8F06_462E_AE0D_9A4FACC12FB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLyricsEditorDlg dialog
struct INFO
{
	union
	{
		DWORD dwFlags;
		struct
		{
			DWORD bMediaPlayerPatch:1;
			DWORD bULyrics:1;
		};
	};
	double fPatch;
};

class CLyricsEditorDlg : public CDialog
{
// Construction
public:
	LONG32 m_nLyricsOffset;
	CString m_strLyricsTitle;
	BOOL ReadFromTextFile(CString &strTg,LPCTSTR lpszFileName);
	void GenTargetFile(CString &strTg,LPCTSTR lpszSc);
	CLyricsEditorDlg(CWnd* pParent = NULL);	// standard constructor
	INFO m_info;

// Dialog Data
	//{{AFX_DATA(CLyricsEditorDlg)
	enum { IDD = IDD_LYRICSEDITOR_DIALOG };
	CRichLabel	m_stInfo;
	CEdit	m_edtScFile;
	CEdit	m_edtLyrics;
	CString	m_strScFile;
	CString	m_strLyrics;
	CString	m_strInfo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLyricsEditorDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl* m_pToolTip;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLyricsEditorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnButtonGetlyrics();
	afx_msg void OnButtonBrowsesc();
	afx_msg void OnButtonAdvanced();
	afx_msg void OnButtonExporttolrc();
	afx_msg void OnButtonAbout();
	afx_msg void OnButtonImportlrc();
	afx_msg void OnButtonSave();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LYRICSEDITORDLG_H__C6323AAA_8F06_462E_AE0D_9A4FACC12FB4__INCLUDED_)
