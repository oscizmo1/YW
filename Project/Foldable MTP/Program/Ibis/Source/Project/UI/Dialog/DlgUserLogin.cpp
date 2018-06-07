// CDlgUserLogin 메시지 처리기입니다.
// DlgUserLogin.cpp : 구현 파일입니다.
//

//- WHLEE Login 페이지 작업중 

#include "stdafx.h"
#include "DlgUserLogin.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxStaticEx.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "MainFrm.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CDlgUserLogin 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgUserLogin, CDialog)

CDlgUserLogin::CDlgUserLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUserLogin::IDD, pParent)
{
	m_bIDInputed = FALSE;
	m_bIDConfirm = TRUE;
}

CDlgUserLogin::~CDlgUserLogin()
{
}

void CDlgUserLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgUserLogin, CDialog)

	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgUserLogin 메시지 처리기입니다.

BEGIN_EVENTSINK_MAP(CDlgUserLogin, CDialog)
	ON_EVENT_RANGE(CDlgUserLogin, IDC_GXBTN_A,IDC_GXBTN_NINE, DISPID_CLICK, CDlgUserLogin::ClickGxbtnText, VTS_I4)
	ON_EVENT(CDlgUserLogin, IDC_GXBTN_ENTER, DISPID_CLICK, CDlgUserLogin::ClickGxbtnEnter, VTS_NONE)
	ON_EVENT(CDlgUserLogin, IDC_GXBTN_CLEAR, DISPID_CLICK, CDlgUserLogin::ClickGxbtnClear, VTS_NONE)
	ON_EVENT(CDlgUserLogin, IDC_GXBTN_CANCEL, DISPID_CLICK, CDlgUserLogin::ClickGxbtnCancel, VTS_NONE)
	ON_EVENT(CDlgUserLogin, IDC_GXBTN_LOGINOUT, DISPID_CLICK, CDlgUserLogin::ClickGxbtnLoginout, VTS_NONE)
	ON_EVENT(CDlgUserLogin, IDC_GXBTN_ENGINEER_MODE, DISPID_CLICK, CDlgUserLogin::ClickGxbtnEngineerMode, VTS_NONE)
	ON_EVENT(CDlgUserLogin, IDC_GXBTN_OPERATOR_MODE, DISPID_CLICK, CDlgUserLogin::ClickGxbtnOperatorMode, VTS_NONE)
END_EVENTSINK_MAP()


void CDlgUserLogin::ClickGxbtnText(UINT nID)
{
	CString szInputText;

	CGxButtonEx *btnClicked = (CGxButtonEx*)GetDlgItem(nID);
	szInputText = btnClicked->GetCaption();

	if ( m_bIDInputed == FALSE )
		DispUserID(szInputText.GetBuffer());
	else
		DispUserPw(szInputText.GetBuffer());
}
void CDlgUserLogin::DispUserID(CString strText)
{
	// strText를 받아 기존에 text에 항목에 붙여서 lable로 표시한다.;

	//ID값이 최대 8자리 이상 입력하지 못하게 한다.;
	if ( m_strID.GetLength() >= eMAX_UserIDLength )
	{
		return;
	}

	m_strID	=	m_strID + strText;
	//m_pGUICtrl->ApplyStaticString(this, IDC_GXLBL_ID_INPUT, m_strID);

	CGxStaticEx* pLblID = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_ID_INPUT);

	if ( pLblID == NULL )	return;

	pLblID->SetCaption(m_strID.GetBuffer());
}


void CDlgUserLogin::DispUserPw(CString strText)
{	
	if(m_strPw.GetLength() > 11 )
		return;

	// strText를 받아 기존에 text에 항목에 붙여서 lable로 표시한다.;
	m_strPw	=	m_strPw + strText;

	if (m_strPw == _T("") )	// 공백인데 Label에 표시할 이유가 없음;
		return;

	// Password를 *로 표시하기
	int nLen = m_strPw.GetLength();

	CString strDispPw = _T("");
	CString strTextStar = _T("*");

	for(int i=0; i<nLen; i++)
	{
		strDispPw = strDispPw + strTextStar;
	}

	//m_pGUICtrl->ApplyStaticString(this,IDC_GXLBL_PASS_INPUT, strDispPw);	// *로 표시한다.;

	CGxStaticEx* pLblPW = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_PASS_INPUT);

	if ( pLblPW == NULL )	return;

	pLblPW->SetCaption(strDispPw.GetBuffer());

}


void CDlgUserLogin::SetEngineerMode( BOOL bSet )
{
	m_bEngineerMode = bSet;

	if(m_bEngineerMode)
	{
		SetIDConfirm(FALSE);
	}
	else
	{
		SetIDConfirm(TRUE);
	}
}

BOOL CDlgUserLogin::IsEngineerMode()
{
	return m_bEngineerMode;
}

void CDlgUserLogin::ClickGxbtnEnter()
{	
	if ( m_bIDInputed == FALSE )		//	현재 ID 입력후 Enter일 때
	{
		CGxStaticEx* pLblPW = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_PASS_INPUT);
		pLblPW->SetEnable(TRUE);
		m_bIDInputed = TRUE;
	}
	else							//	PASSWORD 입력후 Enter일 때
	{	
		// 상위보고하는거면 그냥 바로 CIM으로 요청하자 [12/1/2016 OSC]
		if(m_bIDConfirm)
		{
			theSocketInterFace.m_CIM.SendCmdLoginToDataPC(EQUIP_SET_USER_LOGIN_REQUEST, m_strID, m_strPw);
			CUserAccount user;
			user.sID = m_strID;
			user.sPassword = m_strPw;
			user.nLevel = eUSER_Operator;
			ChangeInspectorUser(user);
			CGxButtonEx* pBtnOk = (CGxButtonEx*)GetDlgItem( IDC_GXBTN_LOGINOUT );
			pBtnOk->SetEnable(TRUE);

		}
		else
		{
			// PM모드 해제는 물류작업자(BZone)가 하도록... [12/1/2016 OSC]
			CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
			int iUSerType = pMain->getUserType(m_strID, m_strPw);

			if ( !pMain->FindUser(m_strID) )
			{	
				AfxMessageBox(_T("Check the ID"));	
				ClearLoginData();	
				return;		
			}

			if  (iUSerType < 1 )			
			{	
				AfxMessageBox(_T("Check the password")); 
				ClearLoginData();	
				return;
			}		//	비번 틀림				

			CUserAccount user;
			user.sID = m_strID;
			user.sPassword = m_strPw;
			user.nLevel = pMain->getUserType(m_strID, m_strPw);
			ChangeOperatorUser(user);
			CGxButtonEx* pBtnOk = (CGxButtonEx*)GetDlgItem( IDC_GXBTN_LOGINOUT );
			pBtnOk->SetEnable(TRUE);
		}
	}
}

void CDlgUserLogin::ChangeOperatorUser( CUserAccount user )
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	pMain->SetCurUserOperatorData( user);
}


void CDlgUserLogin::ChangeInspectorUser( CUserAccount user )
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	pMain->SetCurUserInspectorData( user);
}

void CDlgUserLogin::ClearLoginData()
{
	m_strID = _T("");
	m_strPw = _T("");
	m_bIDInputed = FALSE;		

	CGxStaticEx* pLblID = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_ID_INPUT);
	CGxStaticEx* pLblPW = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_PASS_INPUT);

	if ( pLblID == NULL || pLblPW == NULL )	return;

	pLblID->SetCaption(_T(""));
	pLblPW->SetCaption(_T(""));

	pLblPW->SetEnable(FALSE);
}
void CDlgUserLogin::ClickGxbtnClear()
{
	ClearLoginData();
}

void CDlgUserLogin::ClickGxbtnCancel()
{
	::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
}

BOOL CDlgUserLogin::OnInitDialog()									///MODIFY NEED   임시생성 Dialog가 생성되면 login정보를 Load한다.
{
	CDialog::OnInitDialog();

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	pMain->LoadUserAccountMap();

	CGxStaticEx* pLblPW = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_PASS_INPUT);
	pLblPW->SetEnable(FALSE);

	if(m_bEngineerMode)
	{
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_ENGINEER_MODE, GXCOLOR_ON);
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_OPERATOR_MODE, GXCOLOR_OFF);
	}
	else
	{
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_ENGINEER_MODE, GXCOLOR_OFF);
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_OPERATOR_MODE, GXCOLOR_ON);
	}

	m_bBackDoorOpen = GetMainHandler()->m_bBackDoorOpen;
	SetTimer(0, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CDlgUserLogin::PreTranslateMessage(MSG* pMsg)						//	[W] 키보드의 종료 옵션들 제거
{
	if(pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
		return TRUE;
	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgUserLogin::ClickGxbtnLoginout()
{
	OnOK();
}


void CDlgUserLogin::ClickGxbtnEngineerMode()
{
	SetEngineerMode(TRUE);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_ENGINEER_MODE, GXCOLOR_ON);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_OPERATOR_MODE, GXCOLOR_OFF);
}


void CDlgUserLogin::ClickGxbtnOperatorMode()
{
	SetEngineerMode(FALSE);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_ENGINEER_MODE, GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_OPERATOR_MODE, GXCOLOR_ON);
}


void CDlgUserLogin::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(m_bBackDoorOpen != GetMainHandler()->m_bBackDoorOpen)
	{
		OnCancel();
	}
	CDialog::OnTimer(nIDEvent);
}
