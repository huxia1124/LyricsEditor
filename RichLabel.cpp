// RichLabel.cpp : implementation file
//

#include "stdafx.h"
#include "RichLabel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRichLabel

CRichLabel::CRichLabel()
{
	m_crText=GetSysColor(COLOR_WINDOWTEXT);
	m_hBrush=::CreateSolidBrush(GetSysColor(COLOR_3DFACE));

	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT),sizeof(m_lf),&m_lf);

	m_font.CreateFontIndirect(&m_lf);
	m_bTimer=FALSE;
	m_bState=FALSE;
	m_bLink=FALSE;
	m_hCursor=NULL;
	m_Type=None;
	isMouseOn=FALSE;
	isMoveActive=FALSE;

	m_hwndBrush=::CreateSolidBrush(GetSysColor(COLOR_3DFACE));
}

CRichLabel::~CRichLabel()
{
	m_font.DeleteObject();
	::DeleteObject(m_hBrush);
}


BEGIN_MESSAGE_MAP(CRichLabel, CStatic)
	//{{AFX_MSG_MAP(CRichLabel)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRichLabel message handlers

CRichLabel& CRichLabel::SetText(const CString &strText)
{
	SetWindowText(strText);
	return *this;
}

CRichLabel& CRichLabel::SetTextColor(COLORREF crText)
{
	m_crText=crText;
	RedrawWindow();
	return *this;
}

CRichLabel& CRichLabel::FlashBackground(BOOL bActivate,unsigned int nDelay)
{
	if(m_bTimer)
		KillTimer(1);
	if(bActivate)
	{
		m_bState=FALSE;
		m_bTimer=TRUE;
		SetTimer(1,nDelay,NULL);
		m_Type=Background;
	}
	return *this;
}

CRichLabel& CRichLabel::FlashText(BOOL bActivate,unsigned int nDelay)
{
	if(m_bTimer)
	{
		SetWindowText(m_strText);
		KillTimer(1);
	}

	if(bActivate)
	{
		GetWindowText(m_strText);
		m_bState=FALSE;

		m_bTimer=TRUE;
		SetTimer(1,nDelay,NULL);
		m_Type=Text;
	}
	return *this;
}

CRichLabel& CRichLabel::SetBkColor(COLORREF crBkgnd)
{
	if(m_hBrush)
		::DeleteObject(m_hBrush);

	m_hBrush=::CreateSolidBrush(crBkgnd);
	return *this;
}

CRichLabel& CRichLabel::SetBorder(BOOL bSet)
{
	if(!bSet)
		ModifyStyle(WS_BORDER,0,SWP_DRAWFRAME);
	else
		ModifyStyle(0,WS_BORDER,SWP_DRAWFRAME);

	return *this;
}

CRichLabel& CRichLabel::SetSunken(BOOL bSet)
{
	if(!bSet)
		ModifyStyleEx(WS_EX_STATICEDGE,0,SWP_DRAWFRAME);
	else
		ModifyStyleEx(0,WS_EX_STATICEDGE,SWP_DRAWFRAME);

	return *this;
}

void CRichLabel::ReconstructFont()
{
	m_font.DeleteObject();
	BOOL bCreated=m_font.CreateFontIndirect(&m_lf);

	ASSERT(bCreated);
}

CRichLabel& CRichLabel::SetFontBold(BOOL bBold)
{
	m_lf.lfWeight=bBold?FW_BOLD:FW_NORMAL;
	ReconstructFont();
	RedrawWindow();

	return *this;
}

CRichLabel& CRichLabel::SetFontItalic(BOOL bSet)
{
	m_lf.lfItalic=bSet;
	ReconstructFont();
	RedrawWindow();

	return *this;
}

CRichLabel& CRichLabel::SetFontUnderline(BOOL bSet)
{
	m_lf.lfUnderline=bSet;
	ReconstructFont();
	RedrawWindow();

	return *this;
}

CRichLabel& CRichLabel::SetFontName(const CString &strFont)
{
	strcpy(m_lf.lfFaceName,strFont);
	ReconstructFont();
	RedrawWindow();

	return *this;
}

CRichLabel& CRichLabel::SetFontSize(int nSize)
{
	nSize*=-1;
	m_lf.lfHeight=nSize;
	ReconstructFont();
	RedrawWindow();

	return *this;
}

CRichLabel& CRichLabel::SetLink(BOOL bLink,BOOL bMouseActive)
{
	m_bLink=bLink;

	if(bLink)
		ModifyStyle(0,SS_NOTIFY);
	else
		ModifyStyle(SS_NOTIFY,0);

	isMoveActive=bMouseActive;

	return *this;
}

CRichLabel& CRichLabel::SetLinkCursor(HCURSOR hCursor)
{
	m_hCursor=hCursor;
	return *this;
}

void CRichLabel::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if(m_bLink==TRUE)
	{
		CString strLink;

		GetWindowText(strLink);
		strLink.Insert(0,"mailto:");
		ShellExecute(NULL,"open",strLink,NULL,NULL,SW_SHOWNORMAL);
	}

	CStatic::OnLButtonDown(nFlags, point);
}

void CRichLabel::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
//	AfxMessageBox("BBBBBB",MB_OK,NULL);
	m_bState=!m_bState;
	switch(m_Type)
	{
	case Text:
		if(m_bState)
			SetWindowText("");
		else
			SetWindowText(m_strText);
		break;
	case Background:
		InvalidateRect(NULL,FALSE);
		UpdateWindow();
		break;
	}

	CStatic::OnTimer(nIDEvent);
}

HBRUSH CRichLabel::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	if(CTLCOLOR_STATIC==nCtlColor)
	{
		pDC->SelectObject(&m_font);
		if(isMouseOn==TRUE)
		{
			int rv=GetRValue(m_crText);
			int gv=GetGValue(m_crText);
			int bv=GetBValue(m_crText);
			
			COLORREF stdColor=RGB(255-rv,gv,255-bv);
			pDC->SetTextColor(stdColor);
		}
		else
		{
			pDC->SetTextColor(m_crText);
		}

		pDC->SetBkMode(TRANSPARENT);
	}

	if(m_Type==Background)
	{
		if(!m_bState)
			return m_hwndBrush;
	}
	return m_hBrush;	
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}

BOOL CRichLabel::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_hCursor)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

void CRichLabel::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(isMoveActive==FALSE)
		return;
	CRect rc;
	GetClientRect(&rc);
	if(GetCapture()!=this&&isMouseOn==FALSE)
	{
		SetCapture();
		isMouseOn=TRUE;
		RedrawWindow();
	}
	else
	{
		if(!rc.PtInRect(point))
		{
			isMouseOn=FALSE;
			ReleaseCapture();
			RedrawWindow();
		}
	}
	CStatic::OnMouseMove(nFlags, point);
}
