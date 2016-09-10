#if !defined(AFX_ADVANCEDDIALOG_H__C32C7360_BCDB_43A9_9BA1_25E25A44F66B__INCLUDED_)
#define AFX_ADVANCEDDIALOG_H__C32C7360_BCDB_43A9_9BA1_25E25A44F66B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdvancedDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdvancedDialog dialog

class CAdvancedDialog : public CDialog
{
// Construction
public:
	LONG32 m_nMsec;
	CAdvancedDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAdvancedDialog)
	enum { IDD = IDD_DIALOG_ADVANCED };
	CEdit	m_edtPatch;
	CSpinButtonCtrl	m_spPatch;
	BOOL	m_bMediaPath;
	CString	m_strLyricsTitle;
	BOOL	m_bULyrics;
	CString	m_strOffset;
	double	m_fPatch;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdvancedDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdvancedDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonReset();
	afx_msg void OnDeltaposSpinPatch(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckMediaplayerpatch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVANCEDDIALOG_H__C32C7360_BCDB_43A9_9BA1_25E25A44F66B__INCLUDED_)
