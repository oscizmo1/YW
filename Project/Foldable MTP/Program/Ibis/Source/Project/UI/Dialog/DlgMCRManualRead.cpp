// DlgDoor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgMCRManualRead.h"


// CDlgMCRManualRead 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgMCRManualRead, CDialog)

CDlgMCRManualRead::CDlgMCRManualRead(CCellInfo* pCell, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMCRManualRead::IDD, pParent)
{
	m_pCell = pCell;	
}

CDlgMCRManualRead::~CDlgMCRManualRead()
{
}

void CDlgMCRManualRead::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_STATIC_MCR_READ_CELLID, pCellID);
	CDialog::DoDataExchange(pDX);
	
}


BEGIN_MESSAGE_MAP(CDlgMCRManualRead, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDlgMCRManualRead, CDialog)
	ON_EVENT(CDlgMCRManualRead, IDC_GXBTN_MCR_READ_OK, DISPID_CLICK, CDlgMCRManualRead::ClickBtnMCRReadOK, VTS_NONE)
	ON_EVENT(CDlgMCRManualRead, IDC_GXBTN_MCR_READ_CANCLE, DISPID_CLICK, CDlgMCRManualRead::ClickBtnMCRReadCancle, VTS_NONE)
	END_EVENTSINK_MAP()


	
BOOL CDlgMCRManualRead::OnInitDialog()
{	
	CDialog::OnInitDialog();

	CFont font;
	font.CreateFont( 25, // nHeight
		0, // nWidth
		0, // nEscapement
		0, // nOrientation
		1, // nWeight
		0, // bItalic
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		DEFAULT_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
		_T("굴림") ); // lpszFacename
	pCellID.SetFont(&font, TRUE);
	pCellID.SetFocus();
	font.Detach();
	
	

	return FALSE; 
}

//===============================================================================================================================
//===============================================================================================================================

BOOL CDlgMCRManualRead::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
		return TRUE;
	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}



void CDlgMCRManualRead::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);


	CDialog::OnTimer(nIDEvent);
}

void CDlgMCRManualRead::ClickBtnMCRReadOK()
{
 	pCellID.GetWindowText(m_pCell->defaultData.m_strCellID);
	EndDialog(IDOK);
}


void CDlgMCRManualRead::ClickBtnMCRReadCancle()
{
	EndDialog(IDCANCEL);
}
