
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "GaonNuri.h"
#include "GaonNuriDoc.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()

	// 사용자 정의 메세지 처리
	ON_MESSAGE(GxMSG_GUI_CHANGE_VIEW,	OnViewChange)					// 메인 화면을 변경하라는 지령
	ON_MESSAGE(GxMSG_GUI_INFO_UPDATE,	OnInfoUpdate)					// 각종 정보가 변경되었다. 화면에 반영한다.
	ON_MESSAGE(GxMSG_GUI_COMM_UPDATE,	OnHostCommUpdate)				// Server와의 통신 변경
	
	ON_MESSAGE(GxMSG_GUI_EQ_CONNECT,		OnEQConnect)					// 운영PC와 연결 
	ON_MESSAGE(GxMSG_GUI_EQ_DISCONNECT,	OnEQDisconnect)				// 운영PC와 연결 끊김



END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{

}

CMainFrame::~CMainFrame()
{

}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

 	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE;

	if ( cs.hMenu != NULL )
	{
		::DestroyMenu(cs.hMenu);
		cs.hMenu = NULL;
	}

	cs.cx = 760;
	cs.cy = 800;

	cs.lpszName = _T("GaonNuri D-AMT");

//
//	int cx = ::GetSystemMetrics(SM_CXFULLSCREEN);
//	int cy = ::GetSystemMetrics(SM_CYFULLSCREEN);
//
//	if ( cx >= 1280 )
//	{
//		cs.cx = 1280;
//		cs.cy = 1024;
//		cs.x = 0;
//		cs.y = 0;
//// 		cs.x = (cx-cs.cx) / 2;
//// 		cs.y = (cy-cs.cy) / 2;
//		cs.style = (WS_THICKFRAME | WS_MAXIMIZE |WS_POPUP);
//	}
//	else
//	{
//		cs.style = (WS_THICKFRAME | WS_MAXIMIZE |WS_POPUP);
//	}

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	if  (!m_wndMainTitle.Create(this,  
		IDD_BAR_TITLE, 
		CBRS_TOP, // |	//위치 위쪽
		//CBRS_TOOLTIPS |	//툴팁
		//CBRS_FLYBY | 
		//CBRS_HIDE_INPLACE, 
		IDD_BAR_TITLE)) 
	{ 
		TRACE0("Fail  to  create  DialogBar\n"); 
		return  -1; 
	} 

	if  (!m_wndMainMenu.Create(this,  
		IDD_BAR_MENU, 
		CBRS_BOTTOM, // |	//위치 아래쪽
		//CBRS_TOOLTIPS |	//툴팁
		//CBRS_FLYBY | 
		//CBRS_HIDE_INPLACE, 
		IDD_BAR_MENU)) 
	{ 
		TRACE0("Fail  to  create  DialogBar\n"); 
		return  -1; 
	} 

// 	if(!DataBaseConnect())
// 		return -1;

	

	if(!GlobalDataStructInit())
		return -1;

	if(!DataStructTrans())
		return -1;

	if(!OnCreateView())
		return -1;

	//GT.PARK - 운영에서 현제 사용 중인 Recipe를 가져 온다. [201504.08]
	if(!GetRecipe())
		return -1;

	CString strBatchPath;
	CFileFind finder;


	return 0;
}





BOOL CMainFrame::DataStructTrans()
{
	CString strFileName;
	strFileName.Format("%s\\Setup.ini",CGaonNuriApp::GetSetupPath());
	CSettingParm::Read(strFileName);

	//strcpy_s(g_pData->m_cEQPID, CSettingParm::cEQID);
	//strcpy_s(g_pData->m_cCRST, CRST_OFF);

	if (strcmp(g_pData->m_cCRST, "") == 0)
	{
		strcpy_s(g_pData->m_cCRST, CRST_OFF);
	}

	//strcpy_s(g_pData->m_cPPID, GetMachine()->GetModelName());
//	strcpy_s(g_pData->m_cMDLN, CSettingParm::cEQNAME);
	strcpy_s(g_pData->m_cMDLN, _PROGRAM_VERSION);
	strcpy_s(g_pData->m_cEQPID, CSettingParm::cEQID);
//	strcpy_s(g_pData->m_cFDC_PATH, CSettingParm::cFDC_PATH);
	strcpy_s(g_pData->m_cECID_PATH, CSettingParm::cECID_PATH);
	int nLen = min(6, strlen(CSettingParm::cSOFT_REV));
	memset(g_pData->m_cSoftRev, NULL, 7);
	memcpy(g_pData->m_cSoftRev, CSettingParm::cSOFT_REV, nLen);

	strcpy_s(g_pData->m_cProcessID, CSettingParm::cProcessID);
	strcpy_s(g_pData->m_cAMTVersion, CSettingParm::cSOFT_REV);

	strcpy_s(g_pData->m_cOldMaint, CSettingParm::cMaint);
	strcpy_s(g_pData->m_cOldAvilability, CSettingParm::cAvilability);
	strcpy_s(g_pData->m_cOldInterlock, CSettingParm::cInterlock);
	strcpy_s(g_pData->m_cOldMove, CSettingParm::cMove);
	strcpy_s(g_pData->m_cOldRun, CSettingParm::cRun);
	strcpy_s(g_pData->m_cOldFront, CSettingParm::cFront);
	strcpy_s(g_pData->m_cOldRear, CSettingParm::cRear);
	strcpy_s(g_pData->m_cOldSampleLot, CSettingParm::cSampleLot);

	strcpy_s(g_pData->m_cOperatorID1, g_pData->m_cEQPID);		//D Zone 검사자 로그인 정보를 받을때까지는 EqpID로 한다.
	strcpy_s(g_pData->m_cOperatorID2, g_pData->m_cEQPID);		//B Zone 검사자 로그인 정보를 받을때까지는 EqpID로 한다.

	//
	//// m_wndMainTitle.m_lblEqpID.SetText(g_pData->m_cEQPID);
	//CGxUICtrl::SetStaticString(this, IDC_GXLBL_TOP_STATE6, g_pData->m_cEQPID);

	//// m_wndMainTitle.m_lblUserID1.SetText(g_pData->m_cOperatorID1);
	//// m_wndMainTitle.m_lblUserID2.SetText(g_pData->m_cOperatorID2);
	//CGxUICtrl::SetStaticString(this, IDC_GXLBL_TOP_STATE1, g_pData->m_cOperatorID1);
	//CGxUICtrl::SetStaticString(this, IDC_GXLBL_TOP_STATE2, g_pData->m_cOperatorID2);


	return TRUE;
}

BOOL CMainFrame::GlobalDataStructInit()
{
	g_pData->Clear();
	return TRUE;
}

BOOL CMainFrame::OnCreateView(void)
{
	CCreateContext context ;
	CDocument *pDoc = GetActiveDocument() ;
	context.m_pCurrentDoc = pDoc;
	UINT nID = AFX_IDW_PANE_FIRST;

	nID++;

	m_pViewCIM = NULL;
	m_pViewFDC = NULL;
	m_pViewRMS = NULL;
	m_pViewECM = NULL;
	m_pViewSETTING = NULL;


	DWORD dwStyle;// = ( WS_OVERLAPPED | WS_CHILD) ;// = (WS_OVERLAPPED & ~WS_VISIBLE);
	dwStyle &= ~(WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | WS_THICKFRAME);

	dwStyle = 0L;
	CRect rect = CFrameWnd::rectDefault;

	m_pViewCIM = new CViewCIM;
	((CView *)m_pViewCIM)->Create(NULL,NULL, dwStyle,rect/*CFrameWnd::rectDefault*/,this,nID,NULL) ; 

	m_pViewFDC = new CViewFDC;
	((CView *)m_pViewFDC)->Create(NULL,NULL, dwStyle,rect/*CFrameWnd::rectDefault*/,this,nID,NULL) ; 

	m_pViewRMS = new CViewRMS;
	((CView *)m_pViewRMS)->Create(NULL,NULL, dwStyle,rect/*CFrameWnd::rectDefault*/,this,nID,NULL) ; 

	m_pViewECM = new CViewECM;
	((CView *)m_pViewECM)->Create(NULL,NULL, dwStyle,rect/*CFrameWnd::rectDefault*/,this,nID,NULL) ; 

	m_pViewSETTING = new CViewSETTING;
	((CView *)m_pViewSETTING)->Create(NULL,NULL, dwStyle,rect/*CFrameWnd::rectDefault*/,this,nID,NULL) ; 

	return TRUE;
}

//
// 지정한 화면으로 변경시켜준다.
//
int CMainFrame::OnMenuSelect(UINT nItemID)
{
	CGaonNuriDoc *pDoc = (CGaonNuriDoc *)GetActiveDocument();
	CRect rect;

	switch ( nItemID )
	{
	case eFORM_CIM :								// Main 화면, CIM 통신 화면 IDC_BUTTON_MENU_CIM:
		GetActiveView()->GetClientRect(&rect);
		if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CViewCIM)))   return true;
		m_pNewView = (CView*)m_pViewCIM;
		break;

	case eFORM_FDC :								// FDC 항목 보기 IDC_BUTTON_MENU_FDC:	
		GetActiveView()->GetClientRect(&rect);
		if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CViewFDC)))   return true;
		m_pNewView = (CView*)m_pViewFDC;
		break;
		
	case eFORM_RMS :								// RMS 항목 보기 IDC_BUTTON_MENU_RMS:	
		GetActiveView()->GetClientRect(&rect);
		if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CViewRMS)))   return true;
		m_pNewView = (CView*)m_pViewRMS;
		break;

	case eFORM_ECM :								// CEM 항목 보기
		GetActiveView()->GetClientRect(&rect);
		if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CViewECM)))   return true;
		m_pNewView = (CView*)m_pViewECM;
		break;

	case eFORM_SETTING :							// 설정 내용 보기 IDC_BUTTON_MENU_SETTING:	
		GetActiveView()->GetClientRect(&rect);
		if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CViewSETTING)))   return true;
		m_pNewView = (CView*)m_pViewSETTING;
		break;

	default:
		return false;
	}

	if(m_pNewView==GetActiveView() || m_pNewView == NULL)
	{
		return true;
	}

	m_pOldView = GetActiveView() ;

	m_pOldView->GetClientRect(&rect);
	m_pNewView->MoveWindow(rect);

	int nSwitchChildID = m_pNewView->GetDlgCtrlID();
	m_pNewView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
	m_pOldView->SetDlgCtrlID(nSwitchChildID);

	m_pOldView->ShowWindow(SW_HIDE);
	m_pNewView->ShowWindow(SW_SHOW);

	pDoc->AddView(m_pNewView);
	pDoc->RemoveView(m_pOldView);

	SetActiveView(m_pNewView);	
	RecalcLayout();

	return TRUE;
	// TODO: Add your message handler code here
}


// Message를 통해 화면 변경 요청이 들어왔다.
//
// wParam = MainMenu Select Button 
// lParam = SubMenu Select Button, 무시
//
LRESULT CMainFrame::OnViewChange(WPARAM wParam, LPARAM lParam)
{
	UINT nIdx	= wParam;					// 변경하고자 하는 화면 Index
	BOOL bForce = (BOOL)lParam;				// 강제로 화면을 변경할 것인가 ?

	OnMenuSelect( nIdx );					// 화면을 변경시켜준다.

	return 0;
}

// 운영 PC와 연결
LRESULT CMainFrame::OnEQConnect(WPARAM wParam, LPARAM lParam)
{
	UINT nIdx	= wParam;			

	// 상부 Title과 CIM 화면에게 연결을 알려준다.
	::PostMessage( m_wndMainTitle.m_hWnd, GxMSG_GUI_EQ_CONNECT, wParam, lParam);
	if ( m_pViewCIM ) ::PostMessage( m_pViewCIM->m_hWnd, GxMSG_GUI_EQ_CONNECT, wParam, lParam);


	return 0;
}

// 운영 PC와의 연결이 끊어짐
LRESULT CMainFrame::OnEQDisconnect(WPARAM wParam, LPARAM lParam)
{
	UINT nIdx	= wParam;			

	// 상부 Title과 CIM 화면에게 연결이 끊어졌음을 알려준다.
	::PostMessage( m_wndMainTitle.m_hWnd, GxMSG_GUI_EQ_DISCONNECT, wParam, lParam);
	if ( m_pViewCIM ) ::PostMessage( m_pViewCIM->m_hWnd, GxMSG_GUI_EQ_DISCONNECT, wParam, lParam);

	return 0;
}


// HOST 통신 표시
LRESULT CMainFrame::OnHostCommUpdate(WPARAM wParam, LPARAM lParam)
{
	::PostMessage( m_wndMainTitle.m_hWnd, GxMSG_GUI_COMM_UPDATE, wParam, lParam);
	if ( m_pViewCIM ) ::PostMessage( m_pViewCIM->m_hWnd, GxMSG_GUI_COMM_UPDATE, wParam, lParam);

	return 0;
}	

// 각종 정보가 변경되었다. 화면에 반영한다.
LRESULT CMainFrame::OnInfoUpdate(WPARAM wParam, LPARAM lParam)
{
	::PostMessage( m_wndMainTitle.m_hWnd, GxMSG_GUI_INFO_UPDATE, wParam, lParam);

	//d if ( m_pViewCIM ) ::PostMessage( m_pViewCIM->m_hWnd, GxMSG_GUI_COMM_UPDATE, wParam, lParam);

	return 0;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnClose()
{
	if ( AfxMessageBox(_T("Are you want Quit Program ?"), MB_YESNO | MB_ICONQUESTION) == IDYES )
	{
		GetLogCtrl()->AddBuf(LOG_OPERATION, _T("Program Exit Start"));

		GetSockStsDlg()->DestroyWindow();
		delete GetSockStsDlg();

		//CString strFileName;
		//strFileName.Format("%s\\Setup.ini",CGaonNuriApp::GetSetupPath());
		//CSettingParm::Write(strFileName);


//d		m_wndMainTitle.DeleteThread();
//d		m_pViewCIM->DeleteThread();
//d		m_pViewFDC->WriteFdcList();

		m_pViewFDC->DeleteThread();

// 		free(g_pData);
// 		g_pData = NULL;

		//DataBaseDisConnect();
		// this->PostMessage(WM_CLOSE);
		GetLogCtrl()->AddBuf(LOG_OPERATION, _T("Program Exit Finish\n"));

		GetLogCtrl()->Release();
	}
	else
		return;

	CFrameWnd::OnClose();
}




BOOL CMainFrame::InitializeSocket()
{
	GetSockStsDlg()->Create(CDlgSockStatus::IDD, this);
	GetSockStsDlg()->ShowWindow(SW_HIDE);

	//d m_pViewCIM->SetTimer(SOCKET_STATUS_TIMER, 1000, NULL);

	return TRUE;
}

// BOOL CMainFrame::DataBaseConnect()
// {
// 	CString csConnect;
// 	csConnect.Format("Driver={Microsoft Access Driver (*.mdb)};Dbq=%s\\GaonNuri.mdb;DefaultDir=;Uid=Admin;Pwd=;", CGaonNuriApp::GetSetupPath());	
// 
// 	if (FALSE == theApp.m_ADOClass.Connect(csConnect))
// 	{
// 		AfxMessageBox("Connection failure to DataBase!");
// 		return FALSE;
// 	}
// 
// 	return TRUE;
// };

// BOOL CMainFrame::DataBaseDisConnect()
// {
// 	if (FALSE == theApp.m_ADOClass.DisConnect())
// 	{
// 		AfxMessageBox("DisConnection failure to DataBase!");
// 		return FALSE;
// 	}
// 
// 	return TRUE;
// }

CTime CMainFrame::MakeCTimeType(CString strTime)
{
	int nYear = atoi(strTime.Left(4));
	int nMon = atoi(strTime.Mid(5,2));
	int nDay = atoi(strTime.Mid(8,2));
	int nHour = atoi(strTime.Mid(11,2));
	int nMin = atoi(strTime.Mid(14,2));
	int nSec = atoi(strTime.Mid(17,2));

	CTime timeTime(nYear, nMon, nDay, nHour, nMin, nSec);

	return timeTime;
}

CString CMainFrame::MakeCStringType(CTime time)
{
	CString strTime;
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	return strTime;
}

BOOL CMainFrame::CheckSignalResultNetworkConnection()
{
	CString strStatus;
	CString strDrive;
	CString strSIGNAL_RESULT;
	char pTmp[256];
	BOOL	bCheck;
	
	bCheck = FALSE;
	
	memset(pTmp, '\0', 256);

	strSIGNAL_RESULT = CSettingParm::cPG_RESULT_PATH;
	GetLogicalDriveStrings(256, pTmp);

	for(int i=0 ; i<256 ; i++) 
	{
		if(pTmp[i] == '\0' && pTmp[i+1] == '\0')
			break;

		strDrive.Empty();
		while(pTmp[i] != '\0')
			strDrive += pTmp[i++];

		if (strSIGNAL_RESULT.Find(strDrive) > -1 )
		{
			bCheck = TRUE;
		}
	}

	if (bCheck)
	{
		return TRUE;
	} 
	else
	{
		return FALSE;
	}
}

BOOL CMainFrame::CheckNetworkDriverConnection()
{
	CString strS, strT,	strU, strALARM, strMODEL, strFDC_SV_PATH, strECID_ECV_PATH, strSIGNAL_RESULT;
	CString strMsg;

	//strS = CSettingParm::cQMS_DATA_PATH;
	//strT = CSettingParm::cQMS_IMG_PATH;
	//strU = CSettingParm::cQMS_DUMMY_PATH;
	//	strALARM = CSettingParm::cALARM_PATH;

	strMODEL = CSettingParm::cMODEL_PATH;
	strFDC_SV_PATH = CSettingParm::cFDC_SV_PATH;
	strECID_ECV_PATH = CSettingParm::cECID_ECV_PATH;
	strSIGNAL_RESULT = CSettingParm::cPG_RESULT_PATH;

	//if (!IsFolder(strS))
	//{
	//	strMsg.Format("QMS S Driver Disconnected(%s)", strS);
	//	AfxMessageBox(strMsg);
	//}

	//if (!IsFolder(strT))
	//{
	//	strMsg.Format("QMS T Driver Disconnected(%s)", strT);
	//	AfxMessageBox(strMsg);
	//}

	//if (!IsFolder(strU))
	//{
	//	strMsg.Format("QMS U Driver Disconnected(%s)", strU);
	//	AfxMessageBox(strMsg);
	//}

	//if (!IsFile(strALARM))
	//{
	//	strMsg.Format("There is not a Alarm list file(%s)", strALARM);
	//	AfxMessageBox(strMsg);
	//}

	if (!IsFolder(strMODEL))
	{
		strMsg.Format("There is not a Model folder(%s)", strMODEL);
		AfxMessageBox(strMsg);
	}

	if (!IsFile(strFDC_SV_PATH))
	{
		strMsg.Format("There is not a FDC SV file(%s)", strFDC_SV_PATH);
		AfxMessageBox(strMsg);
	}

	if (!IsFile(strECID_ECV_PATH))
	{
		strMsg.Format("There is not a ECV file(%s)", strECID_ECV_PATH);
		AfxMessageBox(strMsg);
	}

	if (!IsFolder(strSIGNAL_RESULT))
	{
		strMsg.Format("There is not a Singnal PC result file(%s)", strSIGNAL_RESULT);
		AfxMessageBox(strMsg);
	}

	return TRUE;

// 	CString strStatus;
// 	CString strDrive;
// 	CString strS, strT, strU;
// 	CString	strALARM, strMODEL, strFDC_SV_PATH, strECID_ECV_PATH, strSIGNAL_RESULT;
// 	char pTmp[256];
// 	BOOL	bCheck_S;
// 	BOOL	bCheck_T;
// 	BOOL	bCheck_U;
// 	BOOL	bCheck_ALARM;
// 	BOOL	bCheck_MODEL;
// 	BOOL	bCheck_FDC_SV_PATH;
// 	BOOL	bCheck_ECID_ECV_PATH;
// 	BOOL	bCheck_SIGNAL_RESULT;
// 
// 	bCheck_S = FALSE;
// 	bCheck_T = FALSE;
// 	bCheck_U = FALSE;
// 	bCheck_ALARM = FALSE;
// 	bCheck_MODEL = FALSE;
// 	bCheck_FDC_SV_PATH = FALSE;
// 	bCheck_ECID_ECV_PATH = FALSE;
// 	bCheck_SIGNAL_RESULT = FALSE;
// 
// 
// 	memset(pTmp, '\0', 256);
// 
// 	strS = CSettingParm::cHOST_SAVE_PATH;
// 	strT = CSettingParm::cHOST_SAVE_IMG_PATH;
// 	strU = CSettingParm::cHOST_SAVE_MONITOR_PATH;
// 	strALARM = CSettingParm::cALARM_PATH;
// 	strMODEL = CSettingParm::cMODEL_PATH;
// 	strFDC_SV_PATH = CSettingParm::cFDC_SV_PATH;
// 	strECID_ECV_PATH = CSettingParm::cECID_ECV_PATH;
// 	strSIGNAL_RESULT = CSettingParm::cSIGNAL_RESULT_PATH;
// 
// 	GetLogicalDriveStrings(256, pTmp);
// 
// 	for(int i=0 ; i<256 ; i++) 
// 	{
// 		if(pTmp[i] == '\0' && pTmp[i+1] == '\0')
// 			break;
// 
// 		strDrive.Empty();
// 		while(pTmp[i] != '\0')
// 			strDrive += pTmp[i++];
// 
// 		if (strS.Find(strDrive) > -1 )
// 		{
// 			bCheck_S = TRUE;
// 		}
// 		else if (strT.Find(strDrive) > -1)
// 		{
// 			bCheck_T = TRUE;
// 		}
// 		else if (strU.Find(strDrive) > -1)
// 		{
// 			bCheck_U = TRUE;
// 		}
// 		else if (strALARM.Find(strDrive) > -1)
// 		{
// 			bCheck_ALARM = TRUE;
// 		}
// 		else if (strMODEL.Find(strDrive) > -1)
// 		{
// 			bCheck_MODEL = TRUE;
// 		}
// 		else if (strFDC_SV_PATH.Find(strDrive) > -1)
// 		{
// 			bCheck_FDC_SV_PATH = TRUE;
// 		}
// 		else if (strECID_ECV_PATH.Find(strDrive) > -1)
// 		{
// 			bCheck_ECID_ECV_PATH = TRUE;
// 		}
// 		else if (strSIGNAL_RESULT.Find(strDrive) > -1)
// 		{
// 			bCheck_SIGNAL_RESULT = TRUE;
// 		}
// 	}
// 
// 	if (bCheck_S
// 		&& bCheck_T
// 		&& bCheck_U
// 		&& bCheck_ALARM
// 		&& bCheck_MODEL
// 		&& bCheck_FDC_SV_PATH
// 		&& bCheck_ECID_ECV_PATH
// 		&& bCheck_SIGNAL_RESULT)
// 	{
// 		return TRUE;
// 	} 
// 	else
// 	{
// 		AfxMessageBox("Confirm Network Driver Disconnected!.");
// 		return FALSE;
// 	}
}


//BOOL CMainFrame::CheckYmsNetworkDriverConnection()
BOOL CMainFrame::CheckQMSNetworkDriverConnection()
{
	CString strStatus;
	CString strDrive;
	CString strS, strT, strU;
	char pTmp[256];
	BOOL	bCheck_S;
	BOOL	bCheck_T;
	BOOL	bCheck_U;
	
	bCheck_S = FALSE;
	bCheck_T = FALSE;
	bCheck_U = FALSE;
	
	memset(pTmp, '\0', 256);

	strS = CSettingParm::cQMS_DATA_PATH;
	strT = CSettingParm::cQMS_IMG_PATH;
	strU = CSettingParm::cQMS_DUMMY_PATH;
	
	GetLogicalDriveStrings(256, pTmp);

	for(int i=0 ; i<256 ; i++) 
	{
		if(pTmp[i] == '\0' && pTmp[i+1] == '\0')
			break;

		strDrive.Empty();
		while(pTmp[i] != '\0')
			strDrive += pTmp[i++];

		if (strS.Find(strDrive) > -1 )
		{
			bCheck_S = TRUE;
		}
		else if (strT.Find(strDrive) > -1)
		{
			bCheck_T = TRUE;
		}
		else if (strU.Find(strDrive) > -1)
		{
			bCheck_U = TRUE;
		}
	}

	if (bCheck_S
		&& bCheck_T
		&& bCheck_U)
	{
		return TRUE;
	} 
	else
	{
		return FALSE;
	}
}

BOOL CMainFrame::CheckVtNetworkDriverConnection()
{
	CString strStatus;
	CString strDrive;
	CString strVT1, strVT2;
	char pTmp[256];
	BOOL	bCheck_VT1;
	BOOL	bCheck_VT2;

	bCheck_VT1 = FALSE;
	bCheck_VT2 = FALSE;

	memset(pTmp, '\0', 256);

	strVT1 = CSettingParm::cVT1_RESULT_PATH;
	strVT2 = CSettingParm::cVT2_RESULT_PATH;

	GetLogicalDriveStrings(256, pTmp);

	for(int i=0 ; i<256 ; i++) 
	{
		if(pTmp[i] == '\0' && pTmp[i+1] == '\0')
			break;

		strDrive.Empty();
		while(pTmp[i] != '\0')
			strDrive += pTmp[i++];

		if (strVT1.Find(strDrive) > -1 )
		{
			bCheck_VT1 = TRUE;
		}
		else if (strVT2.Find(strDrive) > -1)
		{
			bCheck_VT2 = TRUE;
		}
	}

	bCheck_VT2 = TRUE;


	if (bCheck_VT1
		&& bCheck_VT2)
	{
		return TRUE;
	} 
	else
	{
		return FALSE;
	}
}

//GT.PARK - 운영PC와 통신이 연결 되지 않은 경우,  RECIPE정보를 먼저 운영 ini에서 가져오기 위한 코드
BOOL CMainFrame::GetRecipe()
{
	//네트워크 드라이버가 운영에 어떤 폴더로 연결 되어있는지 확인 한다.
	CIni ini(CSettingParm::cRecipePath);

	strcpy_s(g_pData->m_cPPID, ini.GetString("SETTING", "Model Name", "").GetBuffer());


	return TRUE;

}

BOOL CMainFrame::IsFolder(CString strPath)
{
	// 폴더나 드라이브가 존재하는지 검사
	CFileFind cFile;
	BOOL b;

	if (strPath.GetLength() <= 3)	// 드라이브명 검사
	{
		CString strTmp;
		if (strPath.Right(1) == _T("\\"))
			strTmp = strPath + _T("*.*");
		else
			strTmp = strPath + _T("\\*.*");

		if (cFile.FindFile(strTmp))
			return TRUE;
	}
	else							// 폴더명 검사
	{
		b = cFile.FindFile(strPath);
		while (b)
		{
			b = cFile.FindNextFile();
			if (cFile.IsDirectory())
				return TRUE;
		}
	}

	return FALSE;
}

BOOL CMainFrame::IsFile(CString strPath)
{
	CFileFind finder;

	BOOL fExist = finder.FindFile(strPath);
	if (fExist > 0)
		return TRUE;
	else
		return FALSE;
}
