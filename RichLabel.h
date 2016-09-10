#if !defined(AFX_RICHLABEL_H__508EE66F_083C_11D5_8088_886D70E65E68__INCLUDED_)
#define AFX_RICHLABEL_H__508EE66F_083C_11D5_8088_886D70E65E68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RichLabel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRichLabel window
enum FlashType{None,Text,Background};

class CRichLabel : public CStatic
{
// Construction
public:
	CRichLabel();
protected:
	COLORREF m_crText;
	HBRUSH m_hBrush;
	HBRUSH m_hwndBrush;
	LOGFONT m_lf;
	CFont m_font;
	CString m_strText;
	BOOL m_bState;
	BOOL m_bTimer;
	BOOL m_bLink;
	FlashType m_Type;
	HCURSOR m_hCursor;
	BOOL isMouseOn;
	BOOL isMoveActive;


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichLabel)
	//}}AFX_VIRTUAL

// Implementation
public:
	CRichLabel& SetLinkCursor(HCURSOR hCursor);
	CRichLabel& SetLink(BOOL bLink=TRUE,BOOL bMouseActive=TRUE);
	CRichLabel& SetFontSize(int nSize);
	CRichLabel& SetFontName(const CString &strFont);
	CRichLabel& SetFontUnderline(BOOL bSet=TRUE);
	CRichLabel& SetFontItalic(BOOL bSet=TRUE);
	CRichLabel& SetFontBold(BOOL bBold=TRUE);
	void ReconstructFont();
	CRichLabel& SetSunken(BOOL bSet=TRUE);
	CRichLabel& SetBorder(BOOL bSet=TRUE);
	CRichLabel& SetBkColor(COLORREF crBkgnd);
	CRichLabel& FlashText(BOOL bActivate=TRUE,unsigned int nDelay=500);
	CRichLabel& FlashBackground(BOOL bActivate=TRUE,unsigned int nDelay=500);
	CRichLabel& SetTextColor(COLORREF crText);
	CRichLabel& SetText(const CString &strText);
	virtual ~CRichLabel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRichLabel)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RICHLABEL_H__508EE66F_083C_11D5_8088_886D70E65E68__INCLUDED_)
