// MsgBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MT.h"
#include "MsgBox.h"
#include "MainFrm.h"


// CMsgBox 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMsgBox, CDialog)

CMsgBox::CMsgBox(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgBox::IDD, pParent)
{
	m_nDeviceChange = 0;

}

CMsgBox::CMsgBox(int nTitleNo, int nMsgID)
	: CDialog(CMsgBox::IDD, NULL)
{
	SetInit(nTitleNo, nMsgID);
}

CMsgBox::~CMsgBox()
{
}

BOOL CMsgBox::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_btnIDOK.GetWindowRect(&rtOKNomal);
	ScreenToClient(&rtOKNomal);

	SetMessage();
	return TRUE;
}

void CMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnIDOK);
	DDX_Control(pDX, IDCANCEL, m_btnIDCANCEL);
	DDX_Control(pDX, IDC_ILABELX_TITLE, m_lblTitle);
	DDX_Control(pDX, IDC_ILABELX_MSG, m_lblMsg);
	DDX_Control(pDX, IDC_ILABELX_ACTION, m_lblAct);
	DDX_Control(pDX, IDC_ILABELX_MSG2, m_lblMsg2);
}


BEGIN_MESSAGE_MAP(CMsgBox, CDialog)
	ON_BN_CLICKED(IDOK, &CMsgBox::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMsgBox::OnBnClickedCancel)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMsgBox 메시지 처리기입니다.

void CMsgBox::SetMessage()
{
	CString strLatDeviceID, strDeviceID;
	m_strCode.Format("%d_KOR", m_nMsgID);
	m_strMsg = _T("");
	m_strMsg2 = _T("");
	switch(m_nTitleNo)
	{
	case MSG_NORMAL:
		m_strTitle.Format("NORMAL");
		ShowCancel(FALSE);
		break;
	case MSG_CHECK:
		m_strTitle.Format("CHECK");
		if(GetLanguageType() == LAN_ENG)
		{
			m_strCode.Format("%d_ENG", m_nMsgID);
			GetDoc()->m_mapErrorCode.Lookup(m_strCode, m_strMsg);
			m_strCode.Format("%d_KOR", m_nMsgID);
			GetDoc()->m_mapErrorCode.Lookup(m_strCode, m_strMsg2);
		}
		else
		{
			m_strCode.Format("%d_KOR", m_nMsgID);
			GetDoc()->m_mapErrorCode.Lookup(m_strCode, m_strMsg2);
		}
		m_strCode.Format("%d_ARG", m_nMsgID);
		GetDoc()->m_mapErrorCode.Lookup(m_strCode, m_strAct);

		m_lblMsg.put_BackGroundColor(RGB(255,255,0));
		m_lblMsg.put_FontColor(RGB(0,0,0));
		m_lblMsg2.put_BackGroundColor(RGB(255,255,0));
		m_lblMsg2.put_FontColor(RGB(0,0,0));
		ShowCancel(FALSE);
		break;
	case MSG_ERR_AUTO:
		m_strTitle.Format("AUTO ERROR");

		if(GetLanguageType() == LAN_ENG)
		{
			m_strCode.Format("%d_ENG", m_nMsgID);
			GetDoc()->m_mapErrorCode.Lookup(m_strCode, m_strMsg);
			m_strCode.Format("%d_KOR", m_nMsgID);
			GetDoc()->m_mapErrorCode.Lookup(m_strCode, m_strMsg2);
		}
		else
		{
			m_strCode.Format("%d_KOR", m_nMsgID);
			GetDoc()->m_mapErrorCode.Lookup(m_strCode, m_strMsg2);
		}
		m_strCode.Format("%d_ARG", m_nMsgID);
		GetDoc()->m_mapErrorCode.Lookup(m_strCode, m_strAct);
		GetDeviceCtl()->SetAlarmBit(TRUE);

		/////////////////////////////////////////////////////////////////////
		// 2011.01.13 hccho
		//
		// 보고순서를 맞추기 위해 아해주석 부분 Alarm2LC() 밑으로 옮김.
		//
		//GetComm()->EQState2LC(eFault, GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sCode, m_strMsg) ;
		//GetComm()->ProcessState2LC(ePause, GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sCode, m_strMsg) ;
		//
		//////////////////////////////////////////////////////////////////////

		// Auto일 때만 LC 상태 보고
		strLatDeviceID.Format("%s", GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sDeviceID);
		strDeviceID.Format("%s", GetDoc()->m_LastGlass.strDeviceID);
		if(strDeviceID.Left(10) != strLatDeviceID.Left(10))
			GetDoc()->m_bDeviceChange = TRUE;
		else
			GetDoc()->m_bDeviceChange = FALSE;

		/////////////////////////////////////
		// 2011.01.13 HCCHO
		//
		// DEVICE change test용 없애고 알람 보고 함.
		//
		//if(!GetDoc()->m_bDeviceChange)
		if (1)
		{
			if( (GetTickCount() - GetComm()->m_dAlarmSetTimer) > 60000 )
				GetComm()->m_nBakMsg = 0;	// 2009.07.08

			//////////////////////////////////////////
			// 2011.01.13 hccho
			//
			// 이전과 동일한 알람도 상태가 변하므로 보고한다
			//
			//if( (GetComm()->m_nBakMsg != m_nMsgID) )
			if( 1 )
			{
				//////////////////////////////////////////////////////
				// 2011.01.13 hccho
				//
				// 데이터 정합성 개조를 위해 모든 알람 보고로 수정. 
				//  -.알람보고 없이 상태 변경만 보고되는 경우 때문
				//
				//////////////////////////////////////////////////////
				//
// 				switch ( m_nMsgID )
// 				{
				/////////////////////////////////////////////////////
				// 2011.01.13 hccho
				// 
				// 아래 주석은 기존 막아 놓은 알람들임.
				// 필요하면 풀어줄 것.
				/*
				case ERR_PANELDATA_RECEIVE_FAIL :
				case ERR_AREA_SENSOR :
				case ERR_TILTING_MOTOR_MOVING :
				case ERR_AUTOTUNE :
				case ERR_AUTOTUNE_FOV_OUT :
				case ERR_AUTOTUNE_MARK_LINE_BAD :
				case ERR_HANDLER_CRASH_POS :
				case CHK_INSP_COUNT :

					break;
				*/
// 				default :
					GetComm()->Alarm2LC(1, m_nMsgID, m_strMsg);
					GetComm()->m_nBakMsg = m_nMsgID ;
					GetComm()->m_dAlarmSetTimer = GetTickCount();

					//////////////////////////////////////////////////////
					// 2011.01.13 hccho
					// EQ, PROC Status 보고 옮김.
					//
					GetComm()->EQState2LC(eFault, GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sCode, m_strMsg) ;
					GetComm()->ProcessState2LC(ePause, GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sCode, m_strMsg) ;
								//////////////////////////////////////////////////////
// 				}
				////////////////////////////////////////////////////////////

				// 2011.01.13 아래 코드를 위의 switch 문으로 변경 : 주석처리
				/*										}
				if(m_nMsgID != ERR_PANELDATA_RECEIVE_FAIL)
				{
					if(m_nMsgID != ERR_AREA_SENSOR)
					{
						if(m_nMsgID != ERR_TILTING_MOTOR_MOVING)
						{
							if(m_nMsgID != ERR_AUTOTUNE)
							{
								if(m_nMsgID != ERR_AUTOTUNE_FOV_OUT)
								{
									if(m_nMsgID != ERR_AUTOTUNE_MARK_LINE_BAD)
									{
										if(m_nMsgID != ERR_HANDLER_CRASH_POS)
										{
											if(m_nMsgID != CHK_INSP_COUNT)
											{
												if (!bIgnore_LC_Report)//  [8/18/2009 janghan]
												{
													GetComm()->Alarm2LC(1, m_nMsgID, m_strMsg);
													GetComm()->m_nBakMsg = m_nMsgID ;
													GetComm()->m_dAlarmSetTimer = GetTickCount();
												}
											}
										}
									}
								}
							}
						}
						
					}
				}
				*/
			}
		}
		else
		{
			if(m_nDeviceChange < 5)
				m_nDeviceChange++;
			else
				GetDoc()->m_bDeviceChange = FALSE;
		}

		ShowCancel(FALSE);
		break;
	case MSG_ERR_MANUAL:
		m_strTitle.Format("MANUAL ERROR");

		if(GetLanguageType() == LAN_ENG)
		{
			m_strCode.Format("%d_ENG", m_nMsgID);
			GetDoc()->m_mapErrorCode.Lookup(m_strCode, m_strMsg);
			m_strCode.Format("%d_KOR", m_nMsgID);
			GetDoc()->m_mapErrorCode.Lookup(m_strCode, m_strMsg2);
		}
		else
		{
			m_strCode.Format("%d_KOR", m_nMsgID);
			GetDoc()->m_mapErrorCode.Lookup(m_strCode, m_strMsg2);
		}
		m_strCode.Format("%d_ARG", m_nMsgID);
		GetDoc()->m_mapErrorCode.Lookup(m_strCode, m_strAct);
		GetDeviceCtl()->SetAlarmBit(TRUE);

		GetComm()->Alarm2LC(1, m_nMsgID, m_strMsg);

		GetComm()->EQState2LC(eFault, GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sCode, m_strMsg) ;
		GetComm()->ProcessState2LC(ePause, GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sCode, m_strMsg) ;

		ShowCancel(FALSE);
		break;
	case MSG_ERR_INIT:
		m_strTitle.Format("INITIALIZE ERROR");
		if(GetLanguageType() == LAN_ENG)
		{
			m_strCode.Format("%d_ENG", m_nMsgID);
			GetDoc()->m_mapErrorCode.Lookup(m_strCode, m_strMsg);
			m_strCode.Format("%d_KOR", m_nMsgID);
			GetDoc()->m_mapErrorCode.Lookup(m_strCode, m_strMsg2);
		}
		else
		{
			m_strCode.Format("%d_KOR", m_nMsgID);
			GetDoc()->m_mapErrorCode.Lookup(m_strCode, m_strMsg2);
		}
		m_strCode.Format("%d_ARG", m_nMsgID);
		GetDoc()->m_mapErrorCode.Lookup(m_strCode, m_strAct);
		GetDeviceCtl()->SetAlarmBit(TRUE);
		ShowCancel(FALSE);
		break;
	case MSG_INFO:
		m_strTitle.Format("Information");
		ShowCancel(FALSE);
		break;
	case MSG_INSP:
		m_strTitle.Format("SELECT INSP MODE");
		ShowCancel(FALSE);
		break;
	case MSG_EXIT:
		m_strTitle.Format("EXIST PROGRAM");
		m_strMsg = "Do you want exit MMT_NEW program?";
		m_strAct = "Click OK or CANCEL";
		ShowCancel(TRUE);
		break;
	default:
		break;
	}

	m_lblTitle.put_Caption(m_strTitle);
	m_lblMsg.put_Caption(m_strMsg);
	m_lblMsg2.put_Caption(m_strMsg2);
	m_lblAct.put_Caption(m_strAct);
	ErrorLog(m_nTitleNo, m_nMsgID, m_strMsg);
	m_btnIDOK.SetFocus();
}
void CMsgBox::OnBnClickedOk()
{
	GetDoc()->SetLog("Message 창 자체내 리셋됨",LOG_COMM);
	GetApp()->Reset();
	if(m_nTitleNo == MSG_EXIT)
		::PostMessage(GetMainFrame()->GetSafeHwnd(), WM_CLOSE, NULL, NULL);
	if(m_nMsgID == CHK_PCB_CHANGE)
		GetDoc()->m_bPCBChangeComp = TRUE;
	OnOK();
}

void CMsgBox::SetInit( int nTitleNo, int nMsgID )
{
	m_nTitleNo = nTitleNo;
	m_nMsgID = nMsgID;


}
void CMsgBox::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
	//ShowWindow(SW_HIDE);
}

void CMsgBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	if(bShow)
	{
		//CRect rect;
		//GetMainFrame()->m_pForm[FORM_AUTO]->GetWindowRect(&rect);
		//this->MoveWindow(&rect);
	}
	else
	{
		//AlarmBitOff();
	}
}

void CMsgBox::ShowCancel( BOOL bShow )
{
	if(bShow)
	{
		m_btnIDOK.MoveWindow(&rtOKNomal);
		m_btnIDCANCEL.ShowWindow(SW_SHOW);
	}
	else
	{
		CRect rtOK, rtBox;
		m_btnIDOK.GetWindowRect(&rtOK);
		ScreenToClient(&rtOK);
		this->GetWindowRect(&rtBox);
		ScreenToClient(&rtBox);
		rtOK.OffsetRect(  ( rtBox.CenterPoint().x - rtOK.CenterPoint().x )  , 0);
		m_btnIDOK.MoveWindow(&rtOK);

		m_btnIDCANCEL.ShowWindow(SW_HIDE);
	}
}

void CMsgBox::ErrorLog( int nTitleNo, int nMsgNo, CString strErrMsg )
{
	CString strTitle = _T("");
	if(nTitleNo == MSG_ERR_AUTO)
	{
		strTitle = "[AUTO]";
	}
	else if(nTitleNo == MSG_ERR_MANUAL)
	{
		strTitle = "[MANUAL]";
	}
	else if(nTitleNo == MSG_ERR_INIT)
	{
		strTitle = "[INIT]";
	}
	if(strTitle != _T(""))
	{
		CString strLog;
		strLog.Format("%s\t%d\t%s", strTitle, nMsgNo, strErrMsg);
		GetDoc()->SetLog(strLog, LOG_ERROR);
	}
}

void CMsgBox::AlarmBitOff()
{
	GetDeviceCtl()->SetAlarmBit(FALSE);
}
void CMsgBox::OnClose()
{

	CDialog::OnClose();
}

void CMsgBox::OnDestroy()
{
	AlarmBitOff();
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}


BOOL CMsgBox::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN){  if((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE)){          return TRUE;        }    }
	if(pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)		return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}
