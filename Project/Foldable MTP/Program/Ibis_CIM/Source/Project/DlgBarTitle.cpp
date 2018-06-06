// DlgBarTitle.cpp : implementation file
//

#include "stdafx.h"
#include "GaonNuri.h"
#include "DlgBarTitle.h"

#include "CimClass\DataStructure.h"
#include "GxGUI\GxUICtrl.h"

// CDlgBarTitle dialog

IMPLEMENT_DYNAMIC(CDlgBarTitle, CDialogBar)

CDlgBarTitle::CDlgBarTitle()
{

}

CDlgBarTitle::~CDlgBarTitle()
{
}


void CDlgBarTitle::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgBarTitle, CDialogBar)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)

	ON_MESSAGE(GxMSG_GUI_EQ_CONNECT,		OnEQConnect)					// 운영PC와 연결 
	ON_MESSAGE(GxMSG_GUI_EQ_DISCONNECT,	OnEQDisconnect)				// 운영PC와 연결 끊김
	ON_MESSAGE(GxMSG_GUI_COMM_UPDATE,	OnHostCommUpdate)				// HOST 통신 데이터 수신 표시
	ON_MESSAGE(GxMSG_GUI_INFO_UPDATE,	OnInfoUpdate)					// 각종 정보가 변경되었다. 화면에 반영한다.

END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDlgBarTitle, CDialogBar)
	ON_EVENT(CDlgBarTitle, IDC_GXBTN_TOP_CLOCK, DISPID_CLICK, CDlgBarTitle::ClickGxbtnTopClock, VTS_NONE)
END_EVENTSINK_MAP()


// 운영 PC와 연결
LRESULT CDlgBarTitle::OnEQConnect(WPARAM wParam, LPARAM lParam)
{
//	UINT nIdx	= wParam;			

	CGxUICtrl::SetStaticColor(this, IDC_GXLBL_TOP_COMM_EQ,	GXCOLOR_ON);		// 운영PC와 연결

	return 0;
}

// 운영 PC와의 연결이 끊어짐
LRESULT CDlgBarTitle::OnEQDisconnect(WPARAM wParam, LPARAM lParam)
{
//	UINT nIdx	= wParam;			

	CGxUICtrl::SetStaticColor(this, IDC_GXLBL_TOP_COMM_EQ,	GXCOLOR_OFF);		// 운영PC와 연결

	return 0;
}

// HOST 통신 변경
LRESULT CDlgBarTitle::OnHostCommUpdate(WPARAM wParam, LPARAM lParam)
{
	UINT nIdx	= wParam;			
	UINT nFlag	= lParam;

	switch ( nIdx )
	{
		case eIDX_HostConnect :		// HOST와의 연결
			if ( nFlag == 0 )			// OFF
			{
				// OFFLINE 표시
				CGxUICtrl::SetStaticColor(this, IDC_GXLBL_TOP_COMM_HOST1,	GXCOLOR_OFF);		// CONNECT button OFF
			}
			else	// ON
			{
				CGxUICtrl::SetStaticColor(this, IDC_GXLBL_TOP_COMM_HOST1,	GXCOLOR_ON);		// CONNECT button ON
			}
			break;


		case eIDX_HostOnline :		// SELECT / ONLINE 상태 변경
			if ( nFlag == 0 )			// OFF
			{
				CGxUICtrl::SetStaticColor(this, IDC_GXLBL_TOP_COMM_HOST2,		GXCOLOR_OFF);		// OFFLINE 
			}
			else
			{
				CGxUICtrl::SetStaticColor(this, IDC_GXLBL_TOP_COMM_HOST2,		GXCOLOR_ON);		// ONLINE 
			}
			break;

		case eIDX_HostRecevie :		// 메세지 수신 처리
			// CGxUICtrl::SetStaticColor(this, IDC_GXLBL_CIM_CAP1, ( nFlag == 0 ) ? GXCOLOR_OFF : GXCOLOR_ON);
			break;
	} //of switch


	return 0;
}


// 각종 정보가 변경되었다. 화면에 반영한다.
LRESULT CDlgBarTitle::OnInfoUpdate(WPARAM wParam, LPARAM lParam)
{
	UINT nIdx	= wParam;			
	UINT nFlag	= lParam;

	DispInfo();

	return 0;
}


// CDlgBarTitle message handlers

BOOL CDlgBarTitle::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CBrush backBrush(RGB(222, 231, 242));
	CBrush* pOldBrush = pDC->SelectObject(&backBrush); 
	CRect rect; pDC->GetClipBox(&rect); 
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush); 
	return TRUE; 

	return CDialogBar::OnEraseBkgnd(pDC);
}

void CDlgBarTitle::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogBar::OnPaint() for painting messages

	//PAINTSTRUCT ps; 
	//CRect rect;
	//CDC *pDc;

	//CDC memDC;
	//BITMAP bit;
	//CBitmap bitmap, *pOldBkImg;

	//GetDlgItem(IDC_STATIC_LEFTIMG)->GetClientRect(&rect);
	//pDc = GetDlgItem(IDC_STATIC_LEFTIMG)->BeginPaint(&ps);

	//bitmap.LoadBitmap(IDB_BITMAP_SAMSUNG);
	//bitmap.GetBitmap(&bit);

	//memDC.CreateCompatibleDC(pDc);
	//pOldBkImg = memDC.SelectObject(&bitmap);
	//pDc->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bit.bmWidth, bit.bmHeight, SRCCOPY);

	//memDC.SelectObject(pOldBkImg);
	//bitmap.DeleteObject();
	//memDC.DeleteDC();
	//GetDlgItem(IDC_STATIC_LEFTIMG)->EndPaint(&ps);

	//GetDlgItem(IDC_STATIC_RIGHTIMG)->GetClientRect(&rect);
	//pDc = GetDlgItem(IDC_STATIC_RIGHTIMG)->BeginPaint(&ps);

	//bitmap.LoadBitmap(IDB_BITMAP_YWDSP2);
	//bitmap.GetBitmap(&bit);

	//memDC.CreateCompatibleDC(pDc);
	//pOldBkImg = memDC.SelectObject(&bitmap);
	//pDc->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bit.bmWidth, bit.bmHeight, SRCCOPY);

	//memDC.SelectObject(pOldBkImg);
	//bitmap.DeleteObject();
	//memDC.DeleteDC();
	//GetDlgItem(IDC_STATIC_RIGHTIMG)->EndPaint(&ps);

}

// 시계표시
void CDlgBarTitle::OnTimer(UINT_PTR nIDEvent)
{
//	// TODO: Add your message handler code here and/or call default
//	CString sTime;
//	CTime TitleTime = CTime::GetCurrentTime();
//
////	sTime.Format("%s-%s-%s", TitleTime.Format("%Y"), TitleTime.Format("%m"), TitleTime.Format("%d") );
////	m_lblDate.SetText( sTime );
//
//	sTime.Format("%02d:%02d:%02d", TitleTime.GetHour(), TitleTime.GetMinute(), TitleTime.GetSecond());
//	CGxUICtrl::SetStaticString(this, IDC_GXLBL_TOP_CLOCK, sTime);
	


		CString sClock;
		SYSTEMTIME tmNow;
		::GetLocalTime( &tmNow );			// 현재 시각

		CTime tmNow2 = CTime::GetCurrentTime();
		CTimeSpan timeOffset(0, 2, 0, 0);					// 하루
		tmNow2 -= timeOffset;								// 오늘 날짜에서 하루를 빼준다.

		// 시각 표시
		sClock.Format(_T("%04u-%02u-%02u\n K %02u:%02u:%02u\n V %02u:%02u:%02u"), tmNow.wYear, tmNow.wMonth, tmNow.wDay, tmNow.wHour, tmNow.wMinute, tmNow.wSecond,
			tmNow2.GetHour(), tmNow2.GetMinute(), tmNow2.GetSecond());

		CGxUICtrl::SetButtonString(this, IDC_GXBTN_TOP_CLOCK, sClock);				// 현재 시각을 표시한다.




	// m_lblTime.SetText( sTime );

//
// 	KillTimer(nIDEvent);
//
//	if (nIDEvent == TITLE_TIMER)
//	{
//// 		CTime TitleTime = CTime::GetCurrentTime();
//// 		cstr.Format("%s-%s-%s", TitleTime.Format("%Y"), 
//// 			TitleTime.Format("%m"),
//// 			TitleTime.Format("%d") );
//// 
//// 		m_lblDate.SetText(cstr);
//// 
//// 		cstr.Format("%02d:%02d:%02d", TitleTime.GetHour(), TitleTime.GetMinute(), TitleTime.GetSecond());
//// 		m_lblTime.SetText(cstr);
//
//		SetTimer(TITLE_TIMER, 1000, NULL);
//	}
//
//	if (nIDEvent == GAONNURI_VER_TIMER)
//	{
//		GetLogCtrl()->AddBuf(LOG_SYSTEM, _T("GaonNuri Version - %s"), g_pData->m_cVersionUseCellLog);
//		SetTimer(GAONNURI_VER_TIMER, 60*60*1000, NULL);
//	}

	CDialogBar::OnTimer(nIDEvent);
}

BOOL CDlgBarTitle::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogBar::OnCommand(wParam, lParam);
}

void CDlgBarTitle::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler)
{
	// TODO: Add your specialized code here and/or call the base class
}

LRESULT CDlgBarTitle::HandleInitDialog(WPARAM wParam, LPARAM lParam)
{
	OnInitDialog();

	return CDialogBar::HandleInitDialog(wParam, lParam);
}


// 각종 정보를 보여준다.
void CDlgBarTitle::DispInfo()
{
	// m_wndMainTitle.m_lblEqpID.SetText(g_pData->m_cEQPID);

	CGxUICtrl::SetStaticString(this, IDC_GXLBL_TOP_INFO_EQID, g_pData->m_cEQPID);
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_TOP_INFO_MODEL, g_pData->m_cPPID);

	CGxUICtrl::SetStaticString(this, IDC_GXLBL_TOP_VERSION, g_pData->m_cVersionUseCellLog);		// Program version
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_TOP_OPERATOR, g_pData->m_cOperatorID1);

}



BOOL CDlgBarTitle::OnInitDialog()
{
	// UpdateData(FALSE);

	// TODO:  Add extra initialization here

	//m_lblStatus.SetBkColor( RGB(0, 0, 0) );
	//m_lblStatus.SetTextColor( RGB(128, 255, 0) );
	//m_lblStatus.SetFontName("arial bold");
	//m_lblStatus.SetFontBold(TRUE);
	//m_lblStatus.SetFontSize(25);


	//m_lblEqpID.SetBkColor( RGB(0, 0, 0) );
	//m_lblEqpID.SetTextColor( RGB(128, 255, 0) );
	//m_lblEqpID.SetFontName("arial bold");
	//m_lblEqpID.SetFontBold(TRUE);
	//m_lblEqpID.SetFontSize(25);


	//m_lblDate.SetBkColor( RGB(200, 200, 200) );
	//m_lblDate.SetTextColor( RGB(0, 0, 255) );
	//m_lblDate.SetFontName("arial bold");
	//m_lblDate.SetFontBold(TRUE);
	//m_lblDate.SetFontSize(20);

	//m_lblTime.SetBkColor( RGB(200, 200, 200) );
	//m_lblTime.SetTextColor( RGB(0, 0, 255) );
	//m_lblTime.SetFontName("arial bold");
	//m_lblTime.SetFontBold(TRUE);
	//m_lblTime.SetFontSize(20);

	//m_lblSocket.SetBkColor( RGB(0x40, 0x00, 0x00) );
	//m_lblSocket.SetTextColor( RGB(240, 240, 240) );
	//m_lblSocket.SetFontName("arial bold");
	//m_lblSocket.SetFontBold(TRUE);
	//m_lblSocket.SetFontSize(12);

	//m_lblSocketEqStatus.SetBkColor( RGB(255, 0, 0) );
	//m_lblSocketEqStatus.SetTextColor( RGB(0, 0, 0) );
	//m_lblSocketEqStatus.SetFontName("arial bold");
	//m_lblSocketEqStatus.SetFontBold(TRUE);
	//m_lblSocketEqStatus.SetFontSize(12);

	//m_lblVersion.SetBkColor( RGB(200, 200, 200) );
	//m_lblVersion.SetTextColor( RGB(0, 0, 0)  );
	//m_lblVersion.SetFontName("arial bold");
	//m_lblVersion.SetFontBold(TRUE);
	//m_lblVersion.SetFontSize(12);

	//m_lblVersion.SetText("Y_S_FT_16.05.30.1");



	//CString strVersion;
	//m_lblVersion.GetWindowText(strVersion);

//!	GetLogCtrl()->AddBuf(LOG_SYSTEM, _T("GaonNuri Version - %s"), strVersion);
//!	strcpy_s(g_pData->m_cVersionUseCellLog, strVersion);

	////사용할까 말까
	//m_lblUserTitle.SetBkColor( RGB(0x40, 0x00, 0x00) );
	//m_lblUserTitle.SetTextColor( RGB(240, 240, 240) );
	//m_lblUserTitle.SetFontName("arial bold");
	//m_lblUserTitle.SetFontBold(TRUE);
	//m_lblUserTitle.SetFontSize(12);

	//m_lblUserTitle1.SetBkColor( RGB(0x40, 0x00, 0x00) );
	//m_lblUserTitle1.SetTextColor( RGB(240, 240, 240) );
	//m_lblUserTitle1.SetFontName("arial bold");
	//m_lblUserTitle1.SetFontBold(TRUE);
	//m_lblUserTitle1.SetFontSize(12);

	//m_lblUserTitle2.SetBkColor( RGB(0x40, 0x00, 0x00) );
	//m_lblUserTitle2.SetTextColor( RGB(240, 240, 240) );
	//m_lblUserTitle2.SetFontName("arial bold");
	//m_lblUserTitle2.SetFontBold(TRUE);
	//m_lblUserTitle2.SetFontSize(12);

	//m_lblUserID1.SetBkColor( RGB(0, 255, 255) );
	//m_lblUserID1.SetTextColor( RGB(0, 0, 255) );
	//m_lblUserID1.SetFontName("arial bold");
	//m_lblUserID1.SetFontBold(TRUE);
	//m_lblUserID1.SetFontSize(12);


	//m_lblUserID2.SetBkColor( RGB(0, 255, 255) );
	//m_lblUserID2.SetTextColor( RGB(0, 0, 255) );
	//m_lblUserID2.SetFontName("arial bold");
	//m_lblUserID2.SetFontBold(TRUE);
	//m_lblUserID2.SetFontSize(12);

	SetTimer(TITLE_TIMER, 1000, NULL);

//d	SetTimer(GAONNURI_VER_TIMER, 60*60*1000, NULL);
//d	CreateThread();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



//
//void CDlgBarTitle::CreateThread()
//{
//	m_bTitleTimeThreadRun = TRUE;
//	m_pTitleTimeThread = AfxBeginThread(thrTitleTimeThread, (LPVOID)this);
//	if(m_pTitleTimeThread!=NULL)
//		m_hTitleTimeThread = m_pTitleTimeThread->m_hThread;
//	else
//		AfxMessageBox("!!! Attention !!! \n Cannot created Title time Thread");
//}
//
//UINT CDlgBarTitle::thrTitleTimeThread(LPVOID pParam)
//{
//	CDlgBarTitle *pOwner = (CDlgBarTitle*)pParam;
//	pOwner->m_bTitleTimeThreadRun = TRUE;
//	CString cstr;
//
//	while ( pOwner->m_bTitleTimeThreadRun )
//	{
//		CTime TitleTime = CTime::GetCurrentTime();
//		cstr.Format("%s-%s-%s", TitleTime.Format("%Y"), 
//			TitleTime.Format("%m"),
//			TitleTime.Format("%d") );
//
//		pOwner->m_lblDate.SetText(cstr);
//
//		cstr.Format("%02d:%02d:%02d", TitleTime.GetHour(), TitleTime.GetMinute(), TitleTime.GetSecond());
//		pOwner->m_lblTime.SetText(cstr);
//
//		Sleep(1000);
//	}
//
//	return 0;
//}
//
//void CDlgBarTitle::DeleteThread()
//{
//	if (m_hTitleTimeThread)
//	{
//		m_bTitleTimeThreadRun = FALSE;
//
//		DWORD dwExitCode = 0;
//		::GetExitCodeThread(m_hTitleTimeThread, &dwExitCode);
//		if(dwExitCode != 0)
//		{
//			if(::WaitForSingleObject(m_hTitleTimeThread, 5000) != WAIT_OBJECT_0)
//			{
//				::TerminateThread(m_hTitleTimeThread, 0);
//			}
//		}
//		m_pTitleTimeThread = NULL;
//		m_hTitleTimeThread = NULL;
//	}
//}

// 시계 클릭시
void CDlgBarTitle::ClickGxbtnTopClock()
{
	// 창 최소화 메세지 전송
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
}
