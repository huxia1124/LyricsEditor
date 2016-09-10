// AdvancedDialog.cpp : implementation file
//

#include "stdafx.h"
#include "LyricsEditor.h"
#include "AdvancedDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdvancedDialog dialog


CAdvancedDialog::CAdvancedDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAdvancedDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdvancedDialog)
	m_bMediaPath = FALSE;
	m_strLyricsTitle = _T("");
	m_bULyrics = FALSE;
	m_strOffset = _T("");
	m_fPatch = 0.0;
	//}}AFX_DATA_INIT
}


void CAdvancedDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdvancedDialog)
	DDX_Control(pDX, IDC_EDIT_PATCH, m_edtPatch);
	DDX_Control(pDX, IDC_SPIN_PATCH, m_spPatch);
	DDX_Check(pDX, IDC_CHECK_MEDIAPLAYERPATCH, m_bMediaPath);
	DDX_Text(pDX, IDC_EDIT_LYRICSTITLE, m_strLyricsTitle);
	DDV_MaxChars(pDX, m_strLyricsTitle, 512);
	DDX_Check(pDX, IDC_CHECK_UNSYNCHRONISED_LYRICS, m_bULyrics);
	DDX_Text(pDX, IDC_EDIT_LYRICS_OFFSET, m_strOffset);
	DDX_Text(pDX, IDC_EDIT_PATCH, m_fPatch);
	DDV_MinMaxDouble(pDX, m_fPatch, -20., 20.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdvancedDialog, CDialog)
	//{{AFX_MSG_MAP(CAdvancedDialog)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PATCH, OnDeltaposSpinPatch)
	ON_BN_CLICKED(IDC_CHECK_MEDIAPLAYERPATCH, OnCheckMediaplayerpatch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvancedDialog message handlers

void CAdvancedDialog::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	m_fPatch=m_fPatch/100.0;
	UpdateData(FALSE);

	int k=1;
	if(m_strOffset.Left(1)=="-")
	{
		k=-1;
		m_strOffset=m_strOffset.Mid(1);
	}

	int dPos=m_strOffset.Find('.');
	int cPos=m_strOffset.Find(':');

	int nMin=0,nSec=0,nHund=0;

	if(dPos==-1 && cPos==-1)
		m_nMsec= k * atol(m_strOffset);
	else
	{
		if(dPos>=0)
		{
			nHund=atol(m_strOffset.Mid(dPos+1));
			if(cPos==-1)
				nSec=atol(m_strOffset.Left(dPos));
		}
		if(cPos>=0)
		{
			sscanf(m_strOffset,"%ld:%ld",&nMin,&nSec);
		}
		m_nMsec=k * (nMin*60*1000 + nSec*1000 + nHund*10);
	}

	CDialog::OnOK();
}

BOOL CAdvancedDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_strOffset.Format("%ld",m_nMsec);
	m_fPatch*=100.0;
	//m_spPatch.;
	m_spPatch.SetRange32(-20,20);
	UpdateData(FALSE);
	OnCheckMediaplayerpatch();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAdvancedDialog::OnButtonReset() 
{
	// TODO: Add your control notification handler code here
	m_fPatch=0.1;
	m_bMediaPath=TRUE;
	m_bULyrics=FALSE;
	m_strOffset="0";
	m_strLyricsTitle="Lyrics Integrated with LyricsEditor by XiaHuan.";
	UpdateData(FALSE);
}

void CAdvancedDialog::OnDeltaposSpinPatch(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_fPatch += ((double)pNMUpDown->iDelta)*0.01;
	UpdateData(FALSE);

	*pResult = 0;
}

void CAdvancedDialog::OnCheckMediaplayerpatch() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_bMediaPath)
		m_edtPatch.EnableWindow(TRUE);
	else
		m_edtPatch.EnableWindow(FALSE);

}
