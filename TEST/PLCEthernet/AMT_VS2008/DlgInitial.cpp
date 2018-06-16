// DlgInitial.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MT.h"
#include "DlgInitial.h"
#include "MainFrm.h"

#define TIMER_TILT_WAIT		0
#define TIMER_MANUAL_MOVE	1
#define TIMER_PLC_TILT_WAIT	2

using namespace Gdiplus;	// Color class
static enum
{
	ColorErr = 0,
	ColorIng,
	ColorEnd,
	ColorMax
};
static COLORREF ColorHome[ColorMax];

// CDlgInitial 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgInitial, CDialog)

CDlgInitial::CDlgInitial(CWnd* pParent /*=NULL*/)
: CDialog(CDlgInitial::IDD, pParent)
{
	m_pSetLayerdWindowAttributes = NULL;
	ColorHome[ColorErr] =	Color(Color::Red).ToCOLORREF();
	ColorHome[ColorIng] =	Color(Color::Yellow).ToCOLORREF();
	ColorHome[ColorEnd] =	Color(Color::Lime).ToCOLORREF();
	m_iMoveAction = actNONE;
	m_nReadyMoveStep = actNONE;
	m_bPosMoving = FALSE;
}

CDlgInitial::~CDlgInitial()
{
}

void CDlgInitial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABEL_END_HOME1	, m_lbHomeEnd[0]);
	DDX_Control(pDX, IDC_LABEL_END_HOME2	, m_lbHomeEnd[1]);
	DDX_Control(pDX, IDC_LABEL_END_HOME3	, m_lbHomeEnd[2]);
	DDX_Control(pDX, IDC_LABEL_END_HOME4	, m_lbHomeEnd[3]);
	DDX_Control(pDX, IDC_LABEL_END_HOME5	, m_lbHomeEnd[4]);
	DDX_Control(pDX, IDC_LABEL_END_HOME6	, m_lbHomeEnd[5]);
	DDX_Control(pDX, IDC_LABEL_END_HOME7	, m_lbHomeEnd[6]);
	DDX_Control(pDX, IDC_LABEL_END_HOME8	, m_lbHomeEnd[7]);
	DDX_Control(pDX, IDC_LABEL_END_HOME9	, m_lbHomeEnd[8]);
	DDX_Control(pDX, IDC_LABEL_MOVE_HOME1	, m_lbMoveHome[0]);
	DDX_Control(pDX, IDC_LABEL_MOVE_HOME2	, m_lbMoveHome[1]);
	DDX_Control(pDX, IDC_LABEL_MOVE_HOME3	, m_lbMoveHome[2]);
	DDX_Control(pDX, IDC_LABEL_MOVE_HOME4	, m_lbMoveHome[3]);
	DDX_Control(pDX, IDC_LABEL_MOVE_HOME5	, m_lbMoveHome[4]);
	DDX_Control(pDX, IDC_LABEL_MOVE_HOME6	, m_lbMoveHome[5]);
	DDX_Control(pDX, IDC_LABEL_MOVE_HOME7	, m_lbMoveHome[6]);
	DDX_Control(pDX, IDC_LABEL_MOVE_HOME8	, m_lbMoveHome[7]);
	DDX_Control(pDX, IDC_LABEL_MOVE_HOME9	, m_lbMoveHome[8]);
}


BEGIN_MESSAGE_MAP(CDlgInitial, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CDlgInitial::OnCmdInitCancel)
END_MESSAGE_MAP()


// CDlgInitial 메시지 처리기입니다.

void CDlgInitial::OnTimer(UINT_PTR nIDEvent)
{
	if(GetApp()->m_RunStatus != STATUS_INIT)
	{
		EndDialog(IDCANCEL);
		return;
	}
	int nRet = MOVE_ERR_NONE;

	switch(nIDEvent)
	{
	case MOVE_HOME: CheckMotionMoveHome();
		break;
	case TIMER_TILT_WAIT:
		if(GetApp()->m_RunStatus != STATUS_INIT)
		{
			EndDialog(IDCANCEL);
			return;
		}

		nRet = GetDeviceCtl()->CheckMoveTilting(GetDeviceCtl()->m_bTilt);
		if(nRet == MOVE_OK_END)	
		{
			KillTimer(TIMER_TILT_WAIT);
			m_iMoveAction = actREADY_MOVE ;
			m_nReadyMoveStep = actREADY_MOVE;
			SetTimer(TIMER_MANUAL_MOVE, 100, 0);
		}
		else if(nRet < MOVE_ERR_NONE)
		{
			::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_ERR_MANUAL, ERR_TILTING_MOTOR_MOVING);
			GetApp()->ChangeStatus(STATUS_ERROR);
			EndDialog(IDCANCEL);
			return;
		}
		break;
	case TIMER_MANUAL_MOVE:
		if( m_iMoveAction != actNONE )	
			GetDeviceCtl()->ManualSeqContactUnitMove(m_iMoveAction, m_bPosMoving);
		else
		{
			if( GetLineType() == REMOVE_TILTING_TYPE )
			{
				GetMelsec()->MelsecNetBitOn( MEL_SW2_TILT_MOVE_READY );

				KillTimer(TIMER_PLC_TILT_WAIT);

				GetDeviceCtl()->m_dToggleTimer = GetTickCount();
				GetMainFrame()->SetTimer(TIMER_BIT_TOGGLE, 1000, NULL);
			}
			GetApp()->ChangeStatus(STATUS_STOP);
			EndDialog(IDOK);
		}
		break;
	case TIMER_PLC_TILT_WAIT:
		{
			if( ((GetTickCount() - wTimer) > 500) && GetMelsec()->MelsecNetBitRecv(MEL_SW2_TILT_INIT_REQ) )
				GetMelsec()->MelsecNetBitOff(MEL_SW2_TILT_INIT_REQ);

			if(GetMelsec()->MelsecNetBitRecv(MEL_SW2_TILT_INIT_COMP) && (m_nReadyMoveStep != actREADY_MOVE))
			{
				m_lbHomeEnd[AXIS_TILT_MOVE].put_BackColor(ColorHome[ColorEnd]);
				m_iMoveAction = actREADY_MOVE ;
				m_nReadyMoveStep = actREADY_MOVE;
				SetTimer(TIMER_MANUAL_MOVE, 100, 0);
			}
			else if((GetTickCount() - wTimer) > 60000)
			{
				::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_ERR_MANUAL, ERR_TILTING_MOTOR_MOVING);
				GetApp()->ChangeStatus(STATUS_ERROR);
				EndDialog(IDCANCEL);
				return;
			}

		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CDlgInitial::OnInitDialog()
{
	CDialog::OnInitDialog();

	// USER32.DLL 를 로드한다.//
	HINSTANCE hUser32 = LoadLibrary("USER32.DLL");

	// USER32.DLL에서 SetLayeredWindowAttributes 함수 포인터 얻기.//
	m_pSetLayerdWindowAttributes = (SLWA)GetProcAddress(hUser32 , "SetLayeredWindowAttributes");

	// WS_EX_LAYERED 속성을 설정한다.//
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);

	// SetLayeredWindowAttributes 호출한다.//
	m_pSetLayerdWindowAttributes(GetSafeHwnd(), 0, (255 * 40) / 50, LWA_ALPHA);

	GetApp()->ChangeStatus(STATUS_INIT);

	// Inspection End
	GetComm()->PGSend(CMD_ERR_MSG, "PG OFF");
	GetDoc()->BluOff();

	// Check Arm Stretch
	if(GetDeviceCtl()->ChkArmStretch())
	{
		::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_ERR_INIT, ERR_HANDLER_CRASH_POS);
		EndDialog(IDCANCEL);
	}

	// Cylinder 초기화
	CONTACT_UNIT(SENSOR_OFF);
	DWORD dwTime = GetTickCount();
	while(GetTickCount() - dwTime < 2000)
	{
		if(GetDeviceCtl()->ChkContactUnitUp() == ERR_NONE)
			break;
		Sleep(100);
	}
	if(GetTickCount() - dwTime >= 2000)
	{
		::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_ERR_INIT, GetDeviceCtl()->ChkContactUnitUp());
		EndDialog(IDCANCEL);
	}
	BLOW_UNIT(SENSOR_OFF);
	CLAMP_UNIT(SENSOR_OFF);
	// 조명 초기화
	GetDoc()->BluOff();
	GetDeviceCtl()->FrontLightUnit(SENSOR_OFF);

	GetDoc()->m_pEqMainSeq->ProcessInit();

	// Motion 초기화 시작
	if(GetDoc()->m_pAmtData->m_Flag.bInitMotion == TRUE)
	{
		GetDoc()->m_pMotion->HomeFlagResetAll();
		::AfxBeginThread(HomeSearchProc, (LPVOID)0, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		SetTimer(MOVE_HOME , 100, NULL);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgInitial::CheckMotionMoveHome(void)
{
	int nEndCount = 0;

	for( int i = 0 ; i < MAX_AXIS ; i ++ )
	{
		switch(GetDoc()->m_pMotion->m_nOriginFlag[i])
		{
		case HOME_ERR:
			m_lbHomeEnd[i].put_BackColor(ColorHome[ColorErr]);
			KillTimer(MOVE_HOME);
			GetDlgItem(IDCANCEL)->SetWindowText( "Initialize Fail");
// 			::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_AUTO_SELECT, ERR_CONTACT_INIT_ERROR);
// 			GetApp()->ChangeStatus(STATUS_ERROR);
			nEndCount++;
			break;
		case HOME_ING:
			m_lbMoveHome[i].put_BackColor(ColorHome[ColorIng]);
			break;
		case HOME_END:
			m_lbHomeEnd[i].put_BackColor(ColorHome[ColorEnd]);
			if(i < MAX_AXIS-1)
			{
				if(GetDoc()->m_pMotion->m_nOriginFlag[i+1] == HOME_INIT)
					::AfxBeginThread(HomeSearchProc, (LPVOID)(i+1), THREAD_PRIORITY_NORMAL, 0, 0, NULL);
			}
			break;
		default:
			break;
		}
	}

	for( int i = 0 ; i < MAX_AXIS ; i ++ )
	{
		if(GetDoc()->m_pMotion->m_nOriginFlag[i] != HOME_END)
		{
			if(GetDoc()->m_pMotion->m_nOriginFlag[i] == HOME_ERR)
				nEndCount++;
			else
				return;
		}
	}
	if(nEndCount > 0)
	{
		KillTimer(MOVE_HOME);
 		GetApp()->ChangeStatus(STATUS_ERROR);
		GetDlgItem(IDCANCEL)->SetWindowText( "Initialize Fail");
	}
	else
	{
		KillTimer(MOVE_HOME);

		// Tilting 축이 없으면 Contact Ready Move로 건너 뜀 [5/31/2009 OSC]
		// 20090615 KHW Update.
		if(GetLineType() == REMOVE_TILTING_TYPE)
		{
// 			m_iMoveAction = actREADY_MOVE ;
// 			m_nReadyMoveStep = actREADY_MOVE;
// 			SetTimer(TIMER_MANUAL_MOVE, 100, 0);

			while(GetDeviceCtl()->ChkArmStretch())
			{
				// 2009.07.02 Hui 어차피 Run 중이 아니니까 상관 없자나.
				Sleep(10);
			}

			GetDeviceCtl()->m_bMoveTilting = TRUE;
			GetDeviceCtl()->EnableAreaSensor(TRUE);

			m_lbMoveHome[AXIS_TILT_MOVE].put_BackColor(ColorHome[ColorIng]);

			if( GetLineType() == REMOVE_TILTING_TYPE )
			{
				GetMelsec()->MelsecNetBitOn(MEL_SW2_TILT_INIT_REQ) ;
				wTimer = GetTickCount();
				SetTimer(TIMER_PLC_TILT_WAIT, 100, 0);
			}
		}
		else
		{
			// Tilting 동작 시작
			if(GetDeviceCtl()->StartMoveTilting(FALSE) == MOVE_OK_START)
			{
				GetDeviceCtl()->m_dTimer = GetTickCount();
				GetMainFrame()->SetTimer(TIMER_TILT_WAIT, 500, NULL);
			}
			else
			{
				::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_ERR_INIT, ERR_TILTING_MOTOR_MOVING);
				GetApp()->ChangeStatus(STATUS_ERROR);
				GetDlgItem(IDCANCEL)->SetWindowText( "Initialize Fail");
			}
		}
	}
}

void CDlgInitial::OnCmdInitCancel()
{
	KillTimer(MOVE_HOME);
	GetApp()->ChangeStatus(STATUS_STOP);
	OnCancel();
}
