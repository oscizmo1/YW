// frmTeach.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "frmTeach.h"
#include "MainFrm.h"


// CFormTeach

IMPLEMENT_DYNCREATE(CFormTeach, CFormView)

CFormTeach::CFormTeach()
	: CFormView(CFormTeach::IDD)
{
	m_iMoveAction = actNONE;
	m_bPosMoving = FALSE;
	m_bLive = FALSE;
	m_pTabDlgNorCCD = NULL;
	m_pTabDlgPinCCD = NULL;
	m_pTabDlgPattern = NULL;
}

CFormTeach::~CFormTeach()
{
}

void CFormTeach::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	for( int i=0; i < STATIC_TEACH_VIEW_MAX ; i++)
		DDX_Control(pDX, IDC_TEACH_LEFT_CAM+i, m_pctTeachGrap[i]);
	DDX_Control(pDX, IDC_TAB_TEACHING, m_ctrlTabTeach);
	DDX_Control(pDX, IDC_LABEL_MODEL_NAME, m_ctrlModelName);
	DDX_Control(pDX, IDC_ISWITCHLEDX_TOP_LAMP, m_iswxTeachTopLight);
	DDX_Control(pDX, IDC_ISWITCHLEDX_BOTTOM_LAMP, m_iswxTeachBtmLight);
}

BEGIN_MESSAGE_MAP(CFormTeach, CFormView)
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_TEACHING, &CFormTeach::OnSelchangeTabTeaching)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_TEACHING, &CFormTeach::OnSelchangingTabTeaching)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CMD_TEACH_LIVE, &CFormTeach::OnTeachViewLiveGrabStart)
	ON_BN_CLICKED(IDC_CMD_TEACH_GRAB, &CFormTeach::OnTeachViewLiveGrabStop)
	ON_BN_CLICKED(IDC_BTN_MOVE_READY, &CFormTeach::OnMoveToReadyPos)
	ON_BN_CLICKED(IDC_BTN_MOVE_GRAB, &CFormTeach::OnMoveToGrabPos)
	ON_BN_CLICKED(IDC_CMD_BLOW_ON, &CFormTeach::OnBlowOn)
	ON_BN_CLICKED(IDC_CMD_BLOW_OFF, &CFormTeach::OnBlowOff)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CUR_MODEL_SAVE, &CFormTeach::OnCurPosModelSave)
	ON_BN_CLICKED(IDC_BTN_TOP_CLAMP_ON, &CFormTeach::OnBnClickedBtnTopClampOn)
	ON_BN_CLICKED(IDC_BUTTON1, &CFormTeach::OnTestBtn1)
	ON_BN_CLICKED(IDC_RADIO_CENTER, &CFormTeach::OnBnClickedRadioCenter)
	ON_BN_CLICKED(IDC_RADIO_SIDE, &CFormTeach::OnBnClickedRadioSide)
	ON_BN_CLICKED(IDC_BTN_TOP_CLAMP_OFF, &CFormTeach::OnBnClickedBtnTopClampOff)
	ON_BN_CLICKED(IDC_CMD_CYLINDER_ON, &CFormTeach::OnBnClickedCmdCylinderOn)
	ON_BN_CLICKED(IDC_CMD_CYLINDER_OFF, &CFormTeach::OnBnClickedCmdCylinderOff)
//	ON_BN_CLICKED(IDC_BTN_NTHER_MODEL_SAVE, &CFormTeach::OnBnClickedBtnNtherModelSave)
	ON_BN_CLICKED(IDC_CMD_TILTING_UP_POS, &CFormTeach::OnBnClickedCmdTiltingUpPos)
	ON_BN_CLICKED(IDC_CMD_TILT_DOWN_POS, &CFormTeach::OnBnClickedCmdTiltDownPos)
	ON_BN_CLICKED(IDC_BTN_CONTACT_ALIGN_CHECK, &CFormTeach::OnBnClickedBtnContactAlignCheck)
	ON_BN_CLICKED(IDC_BTN_MOVE_PCB_CHANGE_POS, &CFormTeach::OnBnClickedBtnMovePcbChangePos)
END_MESSAGE_MAP()


// CFormTeach diagnostics

#ifdef _DEBUG
void CFormTeach::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormTeach::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormTeach message handlers

BOOL CFormTeach::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CFormTeach::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	ASSERT(GetAlignCtl()->GetVision()->SelTeacWindow(m_pctTeachGrap[STATIC_TEACH_LEFT_CAM].m_hWnd, m_pctTeachGrap[STATIC_TEACH_RIGHT_CAM].m_hWnd));

	TC_ITEM	iTem;
	char*	chTabName[2] = { "Camera Teaching" , "Pattern Setting" };

	CFont* pFont = new CFont;
	CSize sz;
	sz.cx = 300 , sz.cy = 35;

	pFont->CreateFont( 9, 9 ,0, 0, NULL, FALSE, FALSE, FALSE, ANSI_CHARSET,	OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
		DEFAULT_PITCH, "Arial Narrow");

	
	for( int i =0 ; i < 2 ; i ++ )
	{
		iTem.mask = TCIF_TEXT | TCIF_IMAGE ;
		iTem.pszText = chTabName[i];
		m_ctrlTabTeach.InsertItem(i , &iTem);
		m_ctrlTabTeach.SetFont(pFont);
		m_ctrlTabTeach.SetMinTabWidth(150);
		m_ctrlTabTeach.SetItemSize(sz);
	
	}

	CRect rect;
	m_pTabDlgNorCCD = new CDlgTeachTabNormalCCD;
	m_pTabDlgPattern = new CDlgTeachTabPattern;
	if(GetLineType() == A_GROUP_TYPE)
		m_pTabDlgPinCCD = new CDlgTeachTabPinCCD;


	m_pTabDlgNorCCD->Create( CDlgTeachTabNormalCCD::IDD , &m_ctrlTabTeach );
	m_ctrlTabTeach.GetWindowRect(&rect);
	m_pTabDlgNorCCD->MoveWindow( 0, 37 , rect.Width() , rect.Height() -37 );
	m_pTabDlgNorCCD->ShowWindow(SW_HIDE);

	m_pTabDlgPattern->Create( CDlgTeachTabPattern::IDD , &m_ctrlTabTeach );
	m_ctrlTabTeach.GetWindowRect(&rect);
	m_pTabDlgPattern->MoveWindow( 0, 37 , rect.Width() , rect.Height() -37 );
	m_pTabDlgPattern->ShowWindow(SW_HIDE);

	if(GetLineType() == A_GROUP_TYPE)
	{
		m_pTabDlgPinCCD->Create( CDlgTeachTabPinCCD::IDD , &m_ctrlTabTeach );
		m_ctrlTabTeach.GetWindowRect(&rect);
		m_pTabDlgPinCCD->MoveWindow( 0, 37 , rect.Width() , rect.Height()  -37);
		m_pTabDlgPinCCD->ShowWindow(SW_HIDE);
	}


	m_ctrlTabTeach.SetCurSel(0);
	m_pTabDlgNorCCD->ShowWindow(SW_SHOW);

	delete pFont;

	CheckRadioButton(IDC_RADIO_CENTER, IDC_RADIO_SIDE, IDC_RADIO_CENTER);	// 기보값 Center 선택 [2/9/2009 OSC]
}

void CFormTeach::OnDestroy()
{
	CFormView::OnDestroy();
	GetAlignCtl()->GetVision()->DeselTeacWindow();

	if( m_pTabDlgNorCCD) delete m_pTabDlgNorCCD;
	if( m_pTabDlgPattern ) delete m_pTabDlgPattern;
	if( m_pTabDlgPinCCD ) delete m_pTabDlgPinCCD;
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CFormTeach::OnSelchangeTabTeaching(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iSelTab = m_ctrlTabTeach.GetCurSel();

	switch( iSelTab )
	{
	case 0:
		{
			if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::ONE_CONT &&
				GetDoc()->m_pAmtData->m_CurModelInfo.iContactType == CAmtData::PIN_TYPE )
			{
				if(GetLineType() == A_GROUP_TYPE)
					m_pTabDlgPinCCD->ShowWindow(SW_SHOW);
				m_pTabDlgNorCCD->ShowWindow(SW_HIDE);
			}
			else
			{
				if(GetLineType() == A_GROUP_TYPE)
					m_pTabDlgPinCCD->ShowWindow(SW_HIDE);
				m_pTabDlgNorCCD->ShowWindow(SW_SHOW);
			}
		}
		break;
	case 1:
		{
			m_pTabDlgPattern->ShowWindow(SW_SHOW);
		}
		break;

	default:
		break;
	}
	*pResult = 0;
}

void CFormTeach::OnSelchangingTabTeaching(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iTab = m_ctrlTabTeach.GetCurSel();

	switch( iTab )
	{
	case 0:
		{
			if(GetLineType() == A_GROUP_TYPE)
				m_pTabDlgPinCCD->ShowWindow(SW_HIDE);
			m_pTabDlgNorCCD->ShowWindow(SW_HIDE);
		}
		break;

	case 1:
		{
			m_pTabDlgPattern->ShowWindow(SW_HIDE);
		}
		break;
	}

	*pResult = 0;
}

void CFormTeach::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if( bShow )
	{
		m_ctrlModelName.put_Caption(GetDoc()->m_pAmtData->m_CurModelInfo.strModelName);

		if(m_bLive)
			SeImageLiveView();
		if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::ONE_CONT &&
			GetDoc()->m_pAmtData->m_CurModelInfo.iContactType == CAmtData::PIN_TYPE )
		{
			if(GetLineType() == A_GROUP_TYPE)
				m_pTabDlgPinCCD->ShowWindow(SW_SHOW);
			m_pTabDlgNorCCD->ShowWindow(SW_HIDE);
		}
		else
		{
			if(GetLineType() == A_GROUP_TYPE)
				m_pTabDlgPinCCD->ShowWindow(SW_HIDE);
			m_pTabDlgNorCCD->ShowWindow(SW_SHOW);
		}
		SetTimer(TEACH_MOVE , 100, NULL);

		if(GetApp()->IsRunStatus())	this->EnableWindow(FALSE);
		else						this->EnableWindow(TRUE);

		m_iswxTeachTopLight.put_Active(GetDoc()->m_bLightOn[id_OMIT]);
		m_iswxTeachBtmLight.put_Active(GetDoc()->m_bLightOn[id_BtmLight]);
	}
	else
	{
		GetDoc()->GrapAliveFlagAllStop();
		KillTimer(TEACH_MOVE);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CFormTeach::OnTeachViewLiveGrabStart()
{
	SeImageLiveView();
}

void CFormTeach::OnTeachViewLiveGrabStop()
{
	GetDoc()->GrapAliveFlagAllStop();
	m_bLive = FALSE;
}


void CFormTeach::OnMoveToReadyPos()
{
	if( m_bPosMoving == TRUE && m_iMoveAction != actNONE )
	{
		AfxMessageBox(" 현재 컨텍 UNIT 동작 중입니다" );
		return;
	}
	m_iMoveAction = actREADY_MOVE ;

}

void CFormTeach::OnMoveToGrabPos()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( m_bPosMoving == TRUE && m_iMoveAction != actNONE )
	{
		AfxMessageBox(" 현재 컨텍 UNIT 동작 중입니다" );
		return;
	}
	m_iMoveAction = actGRAB_MOVE ;
}

void CFormTeach::OnBlowOn()
{
	BLOW_UNIT(SENSOR_ON);
}

void CFormTeach::OnBlowOff()
{
	BLOW_UNIT(SENSOR_OFF);
}

void CFormTeach::OnTimer(UINT_PTR nIDEvent)
{
	int nRet = MOVE_ERR_NONE;

	switch( nIDEvent )
	{
	case TEACH_MOVE:
		if( m_iMoveAction != actNONE )	
			GetDeviceCtl()->ManualSeqContactUnitMove(m_iMoveAction, m_bPosMoving);
		break;
	}
	CFormView::OnTimer(nIDEvent);
}

void CFormTeach::OnCurPosModelSave()
{
// 	CUserChange dlg;
// 	dlg.DoModal();
	if(GetDoc()->m_bEngLogin == TRUE || GetDoc()->m_bSuperLogin == TRUE)
	{
		STRUCT_MODEL_DATA* pModelData = &(GetDoc()->m_pAmtData->m_CurModelInfo);
		if( GetLineType() != A_GROUP_TYPE )
		{
			BOOL bUse[MAX_CAM_NUM] = {TRUE, TRUE, TRUE, TRUE};
			if(pModelData->iContactNum == CAmtData::TWO_CONT)
			{
				bUse[CAM_0] = FALSE;	bUse[CAM_3] = FALSE;
			}
			else if(pModelData->iContactNum == CAmtData::ONE_CONT)
			{
				for(int i = CAmtData::CONTACT_0; i <= CAmtData::CONTACT_3; i++)
				{
					if(i != pModelData->iContactor)	bUse[i-1] = FALSE;
				}
			}

			for(int i = 0; i < MAX_CAM_NUM; i++)
			{
				if(bUse[i])
				{
					pModelData->ReadyPos[i].dx 	= m_pTabDlgNorCCD->m_dReadyPos[i].dx;
					pModelData->ReadyPos[i].dy 	= m_pTabDlgNorCCD->m_dReadyPos[i].dy;
					pModelData->GrapPos[i].dx 	= m_pTabDlgNorCCD->m_dGrabPos[i].dx;
					pModelData->GrapPos[i].dy 	= m_pTabDlgNorCCD->m_dGrabPos[i].dy;
				}
				else
				{
					// One Contact인데 Side 선택이면 현재 위치를 그대로 넣는다. [5/8/2009 OSC]
					if(pModelData->iContactNum == CAmtData::ONE_CONT && pModelData->iContactor ==  CAmtData::CONTACT_0
						&& i == CAM_1)
					{
						pModelData->ReadyPos[i].dx 	= m_pTabDlgNorCCD->m_dReadyPos[i].dx;
						pModelData->ReadyPos[i].dy 	= m_pTabDlgNorCCD->m_dReadyPos[i].dy;
						pModelData->GrapPos[i].dx 	= m_pTabDlgNorCCD->m_dGrabPos[i].dx;
						pModelData->GrapPos[i].dy 	= m_pTabDlgNorCCD->m_dGrabPos[i].dy;
					}
					else if(pModelData->iContactNum == CAmtData::ONE_CONT && pModelData->iContactor ==  CAmtData::CONTACT_3
						&& i == CAM_2)
					{
						pModelData->ReadyPos[i].dx 	= m_pTabDlgNorCCD->m_dReadyPos[i].dx;
						pModelData->ReadyPos[i].dy 	= m_pTabDlgNorCCD->m_dReadyPos[i].dy;
						pModelData->GrapPos[i].dx 	= m_pTabDlgNorCCD->m_dGrabPos[i].dx;
						pModelData->GrapPos[i].dy 	= m_pTabDlgNorCCD->m_dGrabPos[i].dy;
					}
					else
					{
						pModelData->ReadyPos[i].dx 	= GetDoc()->m_pAmtData->QUAD_NOTUSEDAXIS_POS;
						pModelData->ReadyPos[i].dy 	= GetDoc()->m_pAmtData->QUAD_NOTUSEDAXIS_POS;
						pModelData->GrapPos[i].dx 	= GetDoc()->m_pAmtData->QUAD_NOTUSEDAXIS_POS;
						pModelData->GrapPos[i].dy 	= GetDoc()->m_pAmtData->QUAD_NOTUSEDAXIS_POS;
					}
				}
				GetDoc()->m_pAmtData->PCBChangePos.dx[i] = m_pTabDlgNorCCD->m_dPCBChangePos[i].dx;
				GetDoc()->m_pAmtData->PCBChangePos.dy[i] = m_pTabDlgNorCCD->m_dPCBChangePos[i].dy;

				pModelData->TV_PCBX[i] = m_pTabDlgPattern->m_dTV_PCBX[i];
				pModelData->TV_FPCY[i] = m_pTabDlgPattern->m_dTV_FPCY[i];
			}
			GetDoc()->m_dUnTiltingPos = m_pTabDlgNorCCD->m_dUnTiltingPos;
			GetDoc()->m_dTiltingPos = m_pTabDlgNorCCD->m_dTiltingPos;

			GetDoc()->SaveTiltPos();
			GetDoc()->SavePCBChangePos();
		}
		else
		{
			pModelData->GrapPos[CAM_0].dx = 0.0;
			pModelData->GrapPos[CAM_0].dy = 0.0;
			pModelData->GrapPos[CAM_0].dz = 0.0;

			pModelData->ReadyPos[CAM_0].dx = 0.0;
			pModelData->ReadyPos[CAM_0].dy = 0.0;
			pModelData->ReadyPos[CAM_0].dz = 0.0;

			pModelData->GrapPos[CAM_1].dx = 0.0;
			pModelData->GrapPos[CAM_1].dy = 0.0;
			pModelData->GrapPos[CAM_1].dz = 0.0;

			pModelData->ReadyPos[CAM_1].dx = 0.0;
			pModelData->ReadyPos[CAM_1].dy = 0.0;
			pModelData->ReadyPos[CAM_1].dz = 0.0;

			if(GetLineType() == A_GROUP_TYPE)
			{
				pModelData->GrapPos[CAM_2].dx = m_pTabDlgPinCCD->m_dGrabPos.dx;
				pModelData->GrapPos[CAM_2].dy = m_pTabDlgPinCCD->m_dGrabPos.dy;
				pModelData->GrapPos[CAM_2].dz = m_pTabDlgPinCCD->m_dGrabPos.dz;

				pModelData->ReadyPos[CAM_2].dx = m_pTabDlgPinCCD->m_dReadyPos.dx;
				pModelData->ReadyPos[CAM_2].dy = m_pTabDlgPinCCD->m_dReadyPos.dy;
				pModelData->ReadyPos[CAM_2].dz = m_pTabDlgPinCCD->m_dReadyPos.dz;
			}
		}
		GetDoc()->SaveModelFile();

		if(m_pTabDlgNorCCD->IsWindowVisible())
		{
			m_pTabDlgNorCCD->SetInitDisplayData();
			m_pTabDlgNorCCD->SetGridPos();
		}
	}
}

void CFormTeach::OnBnClickedBtnTopClampOn()
{
	CLAMP_UNIT(SENSOR_ON);
}

void CFormTeach::SeImageLiveView(void)
{
	GetDoc()->GrapAliveFlagAllStop();
	GetAlignCtl()->OverlayClear(DISP_TEACH_L);
	GetAlignCtl()->OverlayClear(DISP_TEACH_R);

	if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::TWO_CONT ||  GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::FOUR_CONT)
	{
		if(	GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter)
		{
			GetDoc()->m_bLiveGrapFlag[CAM_1] = TRUE;
			GetDoc()->m_bLiveGrapFlag[CAM_2] = TRUE;
		}
		else
		{
			GetDoc()->m_bLiveGrapFlag[CAM_0] = TRUE;
			GetDoc()->m_bLiveGrapFlag[CAM_3] = TRUE;
		}
	}
	else
	{
		if(GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_0)
			GetDoc()->m_bLiveGrapFlag[CAM_0] = TRUE;
		else if(GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_1)
			GetDoc()->m_bLiveGrapFlag[CAM_1] = TRUE;
		else if(GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_2)
			GetDoc()->m_bLiveGrapFlag[CAM_2] = TRUE;
		else if(GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_3)
			GetDoc()->m_bLiveGrapFlag[CAM_3] = TRUE;
	}
	m_bLive = TRUE;
}

void CFormTeach::OnTestBtn1()
{
	if( GetDeviceCtl()->MoveStage(ORG_POS) )
	{
		TRACE("OK");
	}
	else
	{
		;//알람처리...
	}
}

void CFormTeach::OnBnClickedRadioCenter()
{
	// TODO: Add your control notification handler code here
	// Center Camera 선택 [2/9/2009 OSC]
	GetDoc()->m_pThreadLiveGrap->SetTeachCam(TRUE);
	m_pTabDlgNorCCD->SetGridPos();
}

void CFormTeach::OnBnClickedRadioSide()
{
	// Side Camera 선택 [2/9/2009 OSC]
	GetDoc()->m_pThreadLiveGrap->SetTeachCam(FALSE);
	m_pTabDlgNorCCD->SetGridPos();
}

void CFormTeach::OnBnClickedBtnTopClampOff()
{
	CLAMP_UNIT(SENSOR_OFF);
}

void CFormTeach::OnBnClickedCmdCylinderOn()
{
	CONTACT_UNIT(SENSOR_ON);
}

void CFormTeach::OnBnClickedCmdCylinderOff()
{
	// Inspection End
	GetComm()->PGSend(CMD_ERR_MSG, "PG OFF");
	GetDoc()->BluOff();
	Sleep(500);

	CONTACT_UNIT(SENSOR_OFF);
}

void CFormTeach::OnBnClickedCmdTiltingUpPos()
{
	if(GetDeviceCtl()->m_bMoveTilting == FALSE)
	{
		if( (GetLineType() == REMOVE_TILTING_TYPE) )
			if( !GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_PRE_SIGN )  )
				GetMelsec()->MelsecNetBitOn( MEL_SW2_TILT_PRE_SIGN );
		if(GetDeviceCtl()->StartMoveTilting(TRUE) == MOVE_OK_START)
		{
			GetDeviceCtl()->m_dTimer = GetTickCount() ;
			GetMainFrame()->SetTimer(TIMER_TILT_WAIT, 500, NULL);
		}
		else
		{
			if( (GetLineType() == REMOVE_TILTING_TYPE) )
				if( GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_PRE_SIGN )  )
					GetMelsec()->MelsecNetBitOff( MEL_SW2_TILT_PRE_SIGN );
			//	::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_ERR_MANUAL, ERR_TILTING_MOTOR_MOVING);
			//	GetApp()->ChangeStatus(STATUS_ERROR);
		}
	}
}

void CFormTeach::OnBnClickedCmdTiltDownPos()
{
	if(GetDeviceCtl()->m_bMoveTilting == FALSE)
	{
		if( (GetLineType() == REMOVE_TILTING_TYPE) )
			if( !GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_PRE_SIGN )  )
				GetMelsec()->MelsecNetBitOn( MEL_SW2_TILT_PRE_SIGN );
		if(GetDeviceCtl()->StartMoveTilting(FALSE) == MOVE_OK_START)
		{
			GetDeviceCtl()->m_dTimer = GetTickCount() ;
			GetMainFrame()->SetTimer(TIMER_TILT_WAIT, 500, NULL);
		}
		else
		{
			if( (GetLineType() == REMOVE_TILTING_TYPE) )
				if( GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_PRE_SIGN )  )
					GetMelsec()->MelsecNetBitOff( MEL_SW2_TILT_PRE_SIGN );
			//	::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_ERR_MANUAL, ERR_TILTING_MOTOR_MOVING);
			//	GetApp()->ChangeStatus(STATUS_ERROR);
		}
	}
}


void CFormTeach::OnBnClickedBtnContactAlignCheck()
{
	// TODO: Add your control notification handler code here
	if( m_bPosMoving == TRUE && m_iMoveAction != actNONE )
	{
		AfxMessageBox(" 현재 컨텍 UNIT 동작 중입니다" );
		return;
	}
	m_iMoveAction = actCONTACT_CHECK_MOVE ;
	CONTACT_UNIT(FALSE);
	BLOW_UNIT(TRUE);
}
BEGIN_EVENTSINK_MAP(CFormTeach, CFormView)
	ON_EVENT(CFormTeach, IDC_ISWITCHLEDX_TOP_LAMP, 5, CFormTeach::OnClickIswitchledxTopLamp, VTS_NONE)
END_EVENTSINK_MAP()

void CFormTeach::OnClickIswitchledxTopLamp()
{
	if(m_iswxTeachTopLight.get_Active())
	{
		GetDoc()->OmitOn();
	}
	else
	{
		GetDoc()->OmitOff();
	}
}

void CFormTeach::OnBnClickedBtnMovePcbChangePos()
{
	if( m_bPosMoving == TRUE && m_iMoveAction != actNONE )
	{
		AfxMessageBox(" 현재 컨텍 UNIT 동작 중입니다" );
		return;
	}
	m_iMoveAction = actPCB_CHANGE_MOVE ;
	
	// FIX된 PCB X 좌표를 Reset [7/3/2009 janghan] - 보류중
	/*GetDoc()->m_fPCBMark_Fix_X[0] = 0.f;
	GetDoc()->m_fPCBMark_Fix_X[1] = 0.f;
	GetDoc()->m_fPCBMark_Fix_X[2] = 0.f;
	GetDoc()->m_fPCBMark_Fix_X[3] = 0.f;*/
}
