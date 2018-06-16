 // DlgTeachTabNormalCCD.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MT.h"
#include "DlgTeachTabNormalCCD.h"
#include "DlgNumPad.h"
#include "MainFrm.h"


// CDlgTeachTabNormalCCD 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTeachTabNormalCCD, CDialog)

CDlgTeachTabNormalCCD::CDlgTeachTabNormalCCD(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTeachTabNormalCCD::IDD, pParent)
{
	m_iCurSetPos = READY_POS;
	m_bInitDlg = FALSE;
	m_dJogSpeed = 1;
	m_bLeftUpEvent = FALSE;
	m_bLeftDownEvent = FALSE;
	m_bLeftLeftEvent = FALSE;
	m_bLeftRightEvent = FALSE;
	m_bRightUpEvent = FALSE;
	m_bRightLeftEvent = FALSE;
	m_bRightDownEvent = FALSE;
	m_bRightRightEvent = FALSE;
}

CDlgTeachTabNormalCCD::~CDlgTeachTabNormalCCD()
{
}

void CDlgTeachTabNormalCCD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID_POS, m_cGridPosVal);
	for( int i=0 ; i < BTN_NORMAL_MAX ; i++ )
		DDX_Control(pDX, IDC_RADIO_GRAP_POS +i, m_btnNormalCCD[i]);
	for( int i=0 ; i < 2 ; i++ )
		DDX_Control(pDX, IDC_BTN_SET_CUR_POS +i, m_btnSetPosition[i]);
	for( int i=0 ; i < LABEL_POS_MAX ; i++ )
		DDX_Control(pDX, IDC_LABEL_CUR_POS_LX + i , m_lbCurPos[i]);
	DDX_Control(pDX, IDC_ISWITCHSLIDERX_JOG_SPD, m_SliderSpeed);
	DDX_Control(pDX, IDC_BTN_LEFTJOGUP, m_ctrlLeftJogUp);
	DDX_Control(pDX, IDC_BTN_LEFTJOGLEFT, m_ctrlLeftJogLeft);
	DDX_Control(pDX, IDC_BTN_LEFTJOGDOWN, m_ctrlLeftJogDown);
	DDX_Control(pDX, IDC_BTN_LEFTJOGRIGHT, m_ctrlLeftJogRight);
	DDX_Control(pDX, IDC_BTN_RIGHTJOGUP, m_ctrlRightJogUp);
	DDX_Control(pDX, IDC_BTN_RIGHTJOGLEFT, m_ctrlRightJogLeft);
	DDX_Control(pDX, IDC_BTN_RIGHTJOGDOWN, m_ctrlRightJogDown);
	DDX_Control(pDX, IDC_BTN_RIGHTJOGRIGHT, m_ctrlRightJogRight);
	DDX_Control(pDX, IDC_CHK_TILT, m_ctrlTiltChk);
	DDX_Control(pDX, IDC_LABEL_AUTO_CURRENT_MODEL, m_lblAutoCurrentModel);
	DDX_Control(pDX, IDC_LABEL_AUTO_CURRENT_MODEL2, m_lblAutoCurrentModel2);
}


BEGIN_MESSAGE_MAP(CDlgTeachTabNormalCCD, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_RADIO_GRAP_POS, &CDlgTeachTabNormalCCD::OnSetGrapPos)
	ON_BN_CLICKED(IDC_RADIO_READY_POS, &CDlgTeachTabNormalCCD::OnSetReadyPos)
	ON_BN_CLICKED(IDC_RADIO_PCB_CHANGE_POS, &CDlgTeachTabNormalCCD::OnSetPCBChangePos)
	ON_BN_CLICKED(IDC_BTN_SET_CUR_POS, &CDlgTeachTabNormalCCD::OnSetCurPos)
	ON_BN_CLICKED(IDC_BTN_SET_POS_MOVE, &CDlgTeachTabNormalCCD::OnSetPosMove)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHK_TILT, &CDlgTeachTabNormalCCD::OnBnClickedChkTilt)
END_MESSAGE_MAP()


// CDlgTeachTabNormalCCD 메시지 처리기입니다.

BOOL CDlgTeachTabNormalCCD::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_cGridPosVal.put_Rows(3);
	m_cGridPosVal.put_Cols(3);

	m_cGridPosVal.put_ColWidth(0 , 625);	
	m_cGridPosVal.put_ColWidth(1 , 975);
	m_cGridPosVal.put_ColWidth(2 , 1050);
	m_cGridPosVal.put_CellAlignment(4);

	for( int i =0 ; i < 3 ; i ++ )
	{
		for( int j=0; j < 3 ; j ++ )
		{
			m_cGridPosVal.put_Row(i) ;
			m_cGridPosVal.put_Col(j) ; 
			m_cGridPosVal.put_CellAlignment(4);
		}
	}

	m_cGridPosVal.put_RowHeight( 0 , 430);
	m_cGridPosVal.put_RowHeight( 1 , 450);
	m_cGridPosVal.put_RowHeight( 2 , 450);


	m_cGridPosVal.put_TextMatrix( 1 , 0 , "Left");
	m_cGridPosVal.put_TextMatrix( 2 , 0 , "Right");
	m_cGridPosVal.put_TextMatrix( 0 , 1 , "X Axis");
	m_cGridPosVal.put_TextMatrix( 0 , 2 , "Y Axis");


	m_iCurSetPos = READY_POS;

	m_SliderSpeed.put_PositionLabels("1mm, 10mm, 20mm, 30mm, 60mm");
	m_SliderSpeed.put_ShowPositionLabels(TRUE);
	m_SliderSpeed.put_ShowPositionIndicators(FALSE);
	m_SliderSpeed.put_TrackStyle(3);
	m_SliderSpeed.put_PositionIndicatorStyle(0);
	m_SliderSpeed.put_PositionIndicatorBevelStyle(1);
	m_SliderSpeed.put_Orientation(1);
	m_SliderSpeed.put_PositionIndicatorSize(3);
	m_SliderSpeed.put_PositionIndicatorMargin(10);
	m_SliderSpeed.put_PositionIndicatorColor(0x000000FF);
	m_SliderSpeed.put_PointerStyle(4);
	m_SliderSpeed.put_EndsMargin(15);



	SetInitDisplayData();
	SetGridReadyPos();
	SetCurposDisplay();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgTeachTabNormalCCD::SetInitDisplayData(void)
{
	for(int i = 0; i < MAX_CAM_NUM; i++)
	{
		m_dReadyPos[i].dx = GetDoc()->m_pAmtData->m_CurModelInfo.ReadyPos[i].dx;
		m_dReadyPos[i].dy = GetDoc()->m_pAmtData->m_CurModelInfo.ReadyPos[i].dy;
		m_dGrabPos[i].dx = GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[i].dx;
		m_dGrabPos[i].dy = GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[i].dy;
		m_dPCBChangePos[i].dx = GetDoc()->m_pAmtData->PCBChangePos.dx[i];
		m_dPCBChangePos[i].dy = GetDoc()->m_pAmtData->PCBChangePos.dy[i];
	}

	m_dTiltingPos			= 		GetDoc()->m_dTiltingPos;
	m_dUnTiltingPos 		= 		GetDoc()->m_dUnTiltingPos;

}

void CDlgTeachTabNormalCCD::SetGridReadyPos(void)
{
	CString strPosVal;
	if(m_ctrlTiltChk.GetCheck() == BST_CHECKED)
	{
		if( GetLineType() == REMOVE_TILTING_TYPE )
		{	// 2009.06.15
			double dPosVal = 0;
			if( GetMainFrame()->m_bMelsecRead )
			{
				AfxMessageBox("Axis-T Ready Position read fail.\nPlease re-push the ready position button.");
				return;
			}
			GetMainFrame()->m_bMelsecRead = TRUE;
			GetMelsec()->MelsecNetPosRead(TILT_INPUT_READY_POS, dPosVal);
			GetMainFrame()->m_bMelsecRead = FALSE;
			strPosVal.Format("%.3f", dPosVal);	m_cGridPosVal.put_TextMatrix( 1 , 1 , strPosVal );
		}
		else
			strPosVal.Format("%.3f" , m_dUnTiltingPos);		m_cGridPosVal.put_TextMatrix( 1 , 1 , strPosVal );
		strPosVal.Format("Not Use");					m_cGridPosVal.put_TextMatrix( 1 , 2 , strPosVal );
		strPosVal.Format("Not Use");					m_cGridPosVal.put_TextMatrix( 2 , 1 , strPosVal );
		strPosVal.Format("Not Use");					m_cGridPosVal.put_TextMatrix( 2 , 2 , strPosVal );
	}
	else
	{
		if( GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter )
		{
			strPosVal.Format("%.3f" , m_dReadyPos[CAM_1].dx);		m_cGridPosVal.put_TextMatrix( 1 , 1 , strPosVal );
			strPosVal.Format("%.3f" , m_dReadyPos[CAM_1].dy);		m_cGridPosVal.put_TextMatrix( 1 , 2 , strPosVal );

			strPosVal.Format("%.3f" , m_dReadyPos[CAM_2].dx);		m_cGridPosVal.put_TextMatrix( 2 , 1 , strPosVal );
			strPosVal.Format("%.3f" , m_dReadyPos[CAM_2].dy);		m_cGridPosVal.put_TextMatrix( 2 , 2 , strPosVal );
		}
		else
		{
			strPosVal.Format("%.3f" , m_dReadyPos[CAM_0].dx);		m_cGridPosVal.put_TextMatrix( 1 , 1 , strPosVal );
			strPosVal.Format("%.3f" , m_dReadyPos[CAM_0].dy);		m_cGridPosVal.put_TextMatrix( 1 , 2 , strPosVal );

			strPosVal.Format("%.3f" , m_dReadyPos[CAM_3].dx);		m_cGridPosVal.put_TextMatrix( 2 , 1 , strPosVal );
			strPosVal.Format("%.3f" , m_dReadyPos[CAM_3].dy);		m_cGridPosVal.put_TextMatrix( 2 , 2 , strPosVal );
		}
	}
	m_cGridPosVal.Refresh();
}

void CDlgTeachTabNormalCCD::SetGridPCBChangePos(void)
{
	CString strPosVal;
	if(m_ctrlTiltChk.GetCheck() == BST_CHECKED)
	{
		strPosVal.Format("Not Use");					m_cGridPosVal.put_TextMatrix( 1 , 1 , strPosVal );
		strPosVal.Format("Not Use");					m_cGridPosVal.put_TextMatrix( 1 , 2 , strPosVal );
		strPosVal.Format("Not Use");					m_cGridPosVal.put_TextMatrix( 2 , 1 , strPosVal );
		strPosVal.Format("Not Use");					m_cGridPosVal.put_TextMatrix( 2 , 2 , strPosVal );
	}
	else
	{
		if( GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter )
		{
			strPosVal.Format("%.3f" , m_dPCBChangePos[CAM_1].dx);		m_cGridPosVal.put_TextMatrix( 1 , 1 , strPosVal );
			strPosVal.Format("%.3f" , m_dPCBChangePos[CAM_1].dy);		m_cGridPosVal.put_TextMatrix( 1 , 2 , strPosVal );

			strPosVal.Format("%.3f" , m_dPCBChangePos[CAM_2].dx);		m_cGridPosVal.put_TextMatrix( 2 , 1 , strPosVal );
			strPosVal.Format("%.3f" , m_dPCBChangePos[CAM_2].dy);		m_cGridPosVal.put_TextMatrix( 2 , 2 , strPosVal );
		}
		else
		{
			strPosVal.Format("%.3f" , m_dPCBChangePos[CAM_0].dx);		m_cGridPosVal.put_TextMatrix( 1 , 1 , strPosVal );
			strPosVal.Format("%.3f" , m_dPCBChangePos[CAM_0].dy);		m_cGridPosVal.put_TextMatrix( 1 , 2 , strPosVal );

			strPosVal.Format("%.3f" , m_dPCBChangePos[CAM_3].dx);		m_cGridPosVal.put_TextMatrix( 2 , 1 , strPosVal );
			strPosVal.Format("%.3f" , m_dPCBChangePos[CAM_3].dy);		m_cGridPosVal.put_TextMatrix( 2 , 2 , strPosVal );
		}
	}
	m_cGridPosVal.Refresh();
}

void CDlgTeachTabNormalCCD::SetGridGrabPos(void)
{
	CString strPosVal;

	if(m_ctrlTiltChk.GetCheck() == BST_CHECKED)
	{
		if(GetLineType() == REMOVE_TILTING_TYPE)
		{	// 2009.06.15
			double dPosVal = 0;
			if( GetMainFrame()->m_bMelsecRead )
			{
				AfxMessageBox("Axis-T Inspection Position read fail.\nPlease re-push the grab position button.");
				return;
			}
			GetMainFrame()->m_bMelsecRead = TRUE;
			GetMelsec()->MelsecNetPosRead(TILT_INPUT_INSPECT_POS, dPosVal);
			GetMainFrame()->m_bMelsecRead = FALSE;
			strPosVal.Format("%.3f", dPosVal);	m_cGridPosVal.put_TextMatrix( 1 , 1 , strPosVal );
		}
		else
			strPosVal.Format("%.3f" , m_dTiltingPos);		m_cGridPosVal.put_TextMatrix( 1 , 1 , strPosVal );

		strPosVal.Format("Not Use");					m_cGridPosVal.put_TextMatrix( 1 , 2 , strPosVal );
		strPosVal.Format("Not Use");					m_cGridPosVal.put_TextMatrix( 2 , 1 , strPosVal );
		strPosVal.Format("Not Use");					m_cGridPosVal.put_TextMatrix( 2 , 2 , strPosVal );
	}
	else
	{
		if( GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter )
		{
			strPosVal.Format("%.3f" , m_dGrabPos[CAM_1].dx);		m_cGridPosVal.put_TextMatrix( 1 , 1 , strPosVal );
			strPosVal.Format("%.3f" , m_dGrabPos[CAM_1].dy);		m_cGridPosVal.put_TextMatrix( 1 , 2 , strPosVal );

			strPosVal.Format("%.3f" , m_dGrabPos[CAM_2].dx);		m_cGridPosVal.put_TextMatrix( 2 , 1 , strPosVal );
			strPosVal.Format("%.3f" , m_dGrabPos[CAM_2].dy);		m_cGridPosVal.put_TextMatrix( 2 , 2 , strPosVal );
		}
		else
		{
			strPosVal.Format("%.3f" , m_dGrabPos[CAM_0].dx);		m_cGridPosVal.put_TextMatrix( 1 , 1 , strPosVal );
			strPosVal.Format("%.3f" , m_dGrabPos[CAM_0].dy);		m_cGridPosVal.put_TextMatrix( 1 , 2 , strPosVal );

			strPosVal.Format("%.3f" , m_dGrabPos[CAM_3].dx);		m_cGridPosVal.put_TextMatrix( 2 , 1 , strPosVal );
			strPosVal.Format("%.3f" , m_dGrabPos[CAM_3].dy);		m_cGridPosVal.put_TextMatrix( 2 , 2 , strPosVal );
		}
	}

	m_cGridPosVal.Refresh();
}

void CDlgTeachTabNormalCCD::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if( bShow )
	{
		SetInitDisplayData();

		m_iCurSetPos = READY_POS ;

		CheckRadioButton(IDC_RADIO_GRAP_POS , IDC_RADIO_READY_POS , IDC_RADIO_READY_POS ) ;
		SetGridPos();
		SetTimer( NORMAL_CUR_POS , 100 , NULL);

		m_bInitDlg = TRUE ;
	}
	else
	{
		m_dJogLXOldPos = 0.0 ;
		m_dJogLYOldPos = 0.0 ;
		m_dJogRXOldPos = 0.0 ;
		m_dJogRYOldPos = 0.0 ;

		KillTimer(NORMAL_CUR_POS);
	}
}

void CDlgTeachTabNormalCCD::OnSetGrapPos()
{
	m_iCurSetPos = GRAB_POS;
	SetGridGrabPos();
	if( GetLineType() == REMOVE_TILTING_TYPE )
		SetCurposDisplay();
}

void CDlgTeachTabNormalCCD::OnSetReadyPos()
{
	m_iCurSetPos = READY_POS;
	SetGridReadyPos();
	if( GetLineType() == REMOVE_TILTING_TYPE )
		SetCurposDisplay();
}

void CDlgTeachTabNormalCCD::OnSetPCBChangePos()
{
	m_iCurSetPos = PCB_CHANGE_POS;
	SetGridPCBChangePos();
	if( GetLineType() == REMOVE_TILTING_TYPE )
		SetCurposDisplay();
}

void CDlgTeachTabNormalCCD::OnSetCurPos()
{
	// Grab Position과 Ready Position을 모두 저장
	double dPos1;

	if(AfxMessageBox("Do you want 'Set to currented position'?", MB_YESNO) == IDNO)
		return;

	if( GetLineType() == REMOVE_TILTING_TYPE )
		SetCurposDisplay();

	if(m_iCurSetPos == PCB_CHANGE_POS)
	{
		POSITION_DATA PCBChangePos[MAX_CAM_NUM];
		PCBChangePos[CAM_0].dx = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X0);
		PCBChangePos[CAM_0].dy = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y0);
		PCBChangePos[CAM_1].dx = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X1);
		PCBChangePos[CAM_1].dy = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y1);
		PCBChangePos[CAM_2].dx = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X2);
		PCBChangePos[CAM_2].dy = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y2);
		PCBChangePos[CAM_3].dx = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X3);
		PCBChangePos[CAM_3].dy = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y3);

		// S/W Limit check
		for(int i = 0; i < MAX_CAM_NUM; i++)
		{
			if(CheckLimit(i, PCBChangePos[i].dx) == FALSE)	return;
			if(CheckLimit(i+4, PCBChangePos[i].dy) == FALSE)	return;
		}

		// 최종 적용
		for(int i = 0; i < MAX_CAM_NUM; i++)
		{
			m_dPCBChangePos[i].dx = PCBChangePos[i].dx;
			m_dPCBChangePos[i].dy = PCBChangePos[i].dy;
		}
	}
	else
	{//2009.06.07
		if( m_ctrlTiltChk.GetCheck() == BST_CHECKED )
		{
			if( GetLineType() == REMOVE_TILTING_TYPE )
			{
				if( GetMainFrame()->m_bMelsecRead )
				{
					AfxMessageBox("Axis-T Current Position read fail.\nPlease re-push the Set Current Position button.");
					return;
				}
				GetMainFrame()->m_bMelsecRead = TRUE;
				GetMelsec()->MelsecNetPosRead( TILT_INPUT_CUR_POS , dPos1 );
				GetMainFrame()->m_bMelsecRead = FALSE;
				if( m_iCurSetPos == GRAB_POS )
				{
					// m_dTiltingPos;
					if( dPos1 > 60. || dPos1 < 2. )
					{
						if( dPos1 < 80.)
						{
							GetMelsec()->MelsecNetBitOn(MEL_SW2_TILT_SAVE_INSP);
							GetDeviceCtl()->m_dToggleTimer = GetTickCount();
							GetMainFrame()->SetTimer(TIMER_BIT_TOGGLE, 500, NULL);
						}
						else
						{
							// 2009.07.03 검사 위치가 너무 높습니다.
							AfxMessageBox("This inspection position is higher than 80.");
						}
					}
					else
					{
						// 2009.07.03 검사 위치가 너무 낮습니다.
						AfxMessageBox("This inspection position is lower than 60.");
					}
				}
				else if( m_iCurSetPos == READY_POS )
				{
					if( dPos1 < 2.)
					{
						GetMelsec()->MelsecNetBitOn(MEL_SW2_TILT_SAVE_READY);
						//m_dUnTiltingPos;
						GetDeviceCtl()->m_dToggleTimer = GetTickCount();
						GetMainFrame()->SetTimer(TIMER_BIT_TOGGLE, 500, NULL);
					}
					else
					{
						// 2009.07.03 대기 위치가 너무 높습니다.
						AfxMessageBox("This ready position is higher than 2.");
					}
				}
			}

		}
		else
		{
			POSITION_DATA ReadyPos[MAX_CAM_NUM];
			POSITION_DATA GrabPos[MAX_CAM_NUM];

			STRUCT_MODEL_DATA* pModelData = &(GetDoc()->m_pAmtData->m_CurModelInfo);

			// 일단 현재 position을 Grab position에 저장한다.
			GrabPos[CAM_0].dx = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X0);
			GrabPos[CAM_0].dy = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y0);
			GrabPos[CAM_1].dx = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X1);
			GrabPos[CAM_1].dy = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y1);
			GrabPos[CAM_2].dx = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X2);
			GrabPos[CAM_2].dy = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y2);
			GrabPos[CAM_3].dx = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X3);
			GrabPos[CAM_3].dy = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y3);

			// 사용하는 Cam, 안하는 Cam 선정
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

			// Grab position을 Ready position에 저장
			for(int i = 0; i < MAX_CAM_NUM; i++)
			{
				if(bUse[i])
				{
					ReadyPos[i].dx = GrabPos[i].dx;
					// Ready Offset 차감하여 저장
					ReadyPos[i].dy = GrabPos[i].dy - pModelData->TV_StageReady;
				}
				else
				{
					// One Contact인데 Side 선택이면 현재 위치를 그대로 넣는다. [5/8/2009 OSC]
					if(pModelData->iContactNum == CAmtData::ONE_CONT && pModelData->iContactor ==  CAmtData::CONTACT_0
						&& i == CAM_1)
					{
						ReadyPos[i].dx = GrabPos[i].dx;
						ReadyPos[i].dy = GrabPos[i].dy;
					}
					else if(pModelData->iContactNum == CAmtData::ONE_CONT && pModelData->iContactor ==  CAmtData::CONTACT_3
						&& i == CAM_2)
					{
						ReadyPos[i].dx = GrabPos[i].dx;
						ReadyPos[i].dy = GrabPos[i].dy;
					}
					else
					{
						// 안쓰는 위치는 10mm로 고정
						ReadyPos[i].dx 	= GetDoc()->m_pAmtData->QUAD_NOTUSEDAXIS_POS;
						ReadyPos[i].dy 	= GetDoc()->m_pAmtData->QUAD_NOTUSEDAXIS_POS;
						GrabPos[i].dx 	= GetDoc()->m_pAmtData->QUAD_NOTUSEDAXIS_POS;
						GrabPos[i].dy 	= GetDoc()->m_pAmtData->QUAD_NOTUSEDAXIS_POS;
					}
				}
				if( ReadyPos[i].dx < GetDoc()->m_pAmtData->QUAD_NOTUSEDAXIS_POS ) ReadyPos[i].dx = GetDoc()->m_pAmtData->QUAD_NOTUSEDAXIS_POS;
				if( ReadyPos[i].dy < GetDoc()->m_pAmtData->QUAD_NOTUSEDAXIS_POS ) ReadyPos[i].dy = GetDoc()->m_pAmtData->QUAD_NOTUSEDAXIS_POS;
			}
			// S/W Limit check
			for(int i = 0; i < MAX_CAM_NUM; i++)
			{
				if(CheckLimit(i, ReadyPos[i].dx) == FALSE)	return;
				if(CheckLimit(i+4, ReadyPos[i].dy) == FALSE)	return;
				if(CheckLimit(i, GrabPos[i].dx) == FALSE)	return;
				if(CheckLimit(i+4, GrabPos[i].dy) == FALSE)	return;
			}
			// 최종 적용
			for(int i = 0; i < MAX_CAM_NUM; i++)
			{
				m_dReadyPos[i].dx = ReadyPos[i].dx;
				m_dReadyPos[i].dy = ReadyPos[i].dy;
				m_dGrabPos[i].dx = GrabPos[i].dx;
				m_dGrabPos[i].dy = GrabPos[i].dy;
			}
		}
	}

	SetGridPos();
}

void CDlgTeachTabNormalCCD::OnSetPosMove()
{
	if(AfxMessageBox("Do you want 'Go to Selected position'?", MB_YESNO) == IDNO)
		return;

	if(m_ctrlTiltChk.GetCheck() == BST_UNCHECKED)
	{
		double dTargetPosLX , dTargetPosRX , dTargetPosLY , dTargetPosRY;

		m_cGridPosVal.put_Col(1) , m_cGridPosVal.put_Row(1);	dTargetPosLX = atof( m_cGridPosVal.get_Text() );
		m_cGridPosVal.put_Col(2) , m_cGridPosVal.put_Row(1);	dTargetPosLY = atof( m_cGridPosVal.get_Text() );
		m_cGridPosVal.put_Col(1) , m_cGridPosVal.put_Row(2);	dTargetPosRX = atof( m_cGridPosVal.get_Text() );
		m_cGridPosVal.put_Col(2) , m_cGridPosVal.put_Row(2);	dTargetPosRY = atof( m_cGridPosVal.get_Text() );

		if( GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter )
		{
			GetDeviceCtl()->ContactMovePos[CAM_1].dx = dTargetPosLX;
			GetDeviceCtl()->ContactMovePos[CAM_1].dy = dTargetPosLY;
			GetDeviceCtl()->ContactMovePos[CAM_2].dx = dTargetPosRX;
			GetDeviceCtl()->ContactMovePos[CAM_2].dy = dTargetPosRY;
		}
		else
		{
			GetDeviceCtl()->ContactMovePos[CAM_0].dx = dTargetPosLX;
			GetDeviceCtl()->ContactMovePos[CAM_0].dy = dTargetPosLY;
			GetDeviceCtl()->ContactMovePos[CAM_3].dx = dTargetPosRX;
			GetDeviceCtl()->ContactMovePos[CAM_3].dy = dTargetPosRY;
		}

		GetDeviceCtl()->m_iTargetPos = SET_POS ;

		if( GetDeviceCtl()->MoveStage(SET_POS, FALSE) == MOVE_OK_END)
		{

		}
		else
		{
			//이동 할 수 없음..
		}
	}
	else
	{
		double dTiltPos;
		m_cGridPosVal.put_Col(1) , m_cGridPosVal.put_Row(1);	dTiltPos = atof( m_cGridPosVal.get_Text() );
		GetDoc()->m_pMotion->StartMove(AXIS_TILT_MOVE, dTiltPos, GetDoc()->m_pMotorParam[AXIS_TILT_MOVE].dSpeed);
	}
}

void CDlgTeachTabNormalCCD::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch( nIDEvent )
	{
	case NORMAL_CUR_POS:	
		if((m_ctrlTiltChk.GetCheck() == BST_CHECKED) && (GetLineType() == REMOVE_TILTING_TYPE))
			break;
		SetCurposDisplay();
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CDlgTeachTabNormalCCD::SetCurposDisplay(void)
{
	if(GetDoc()->m_pThreadLiveGrap == NULL)	return;

	CString strCurPos;
	if(m_ctrlTiltChk.GetCheck() == 0)
	{
		if( GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter )
		{
			strCurPos.Format("%.3f" , GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X1) );
			m_lbCurPos[LABEL_CUR_POS_LX].put_Caption( strCurPos) ;

			strCurPos.Format("%.3f" , GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X2) );
			m_lbCurPos[LABEL_CUR_POS_RX].put_Caption( strCurPos) ;

			strCurPos.Format("%.3f" , GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y1) );
			m_lbCurPos[LABEL_CUR_POS_LY].put_Caption( strCurPos) ;

			strCurPos.Format("%.3f" , GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y2) );
			m_lbCurPos[LABEL_CUR_POS_RY].put_Caption( strCurPos) ;
		}
		else
		{
			strCurPos.Format("%.3f" , GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X0) );
			m_lbCurPos[LABEL_CUR_POS_LX].put_Caption( strCurPos) ;

			strCurPos.Format("%.3f" , GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X3) );
			m_lbCurPos[LABEL_CUR_POS_RX].put_Caption( strCurPos) ;

			strCurPos.Format("%.3f" , GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y0) );
			m_lbCurPos[LABEL_CUR_POS_LY].put_Caption( strCurPos) ;

			strCurPos.Format("%.3f" , GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y3) );
			m_lbCurPos[LABEL_CUR_POS_RY].put_Caption( strCurPos) ;

		}
	}
	else
	{
		if( GetLineType() == REMOVE_TILTING_TYPE )
		{	// 2009.06.15
			double dCurPos = 0;
			if( GetMainFrame()->m_bMelsecRead )
				return;
			GetMainFrame()->m_bMelsecRead = TRUE;
			GetMelsec()->MelsecNetPosRead(TILT_INPUT_CUR_POS, dCurPos);
			GetMainFrame()->m_bMelsecRead = FALSE;
			strCurPos.Format("%.3f", dCurPos);
		}
		else
		{
			strCurPos.Format("%.3f", GetDoc()->m_pMotion->GetCmdPos(AXIS_TILT_MOVE));
		}

		m_lbCurPos[LABEL_CUR_POS_LX].put_Caption(strCurPos);
	}
}
BEGIN_EVENTSINK_MAP(CDlgTeachTabNormalCCD, CDialog)
	ON_EVENT(CDlgTeachTabNormalCCD, IDC_ISWITCHSLIDERX_JOG_SPD, 4, CDlgTeachTabNormalCCD::OnChangeJogSpeed, VTS_NONE)
	ON_EVENT(CDlgTeachTabNormalCCD, IDC_BTN_LEFTJOGUP, 6, CDlgTeachTabNormalCCD::OnMouseDownBtnLeftjogup, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgTeachTabNormalCCD, IDC_BTN_LEFTJOGUP, 8, CDlgTeachTabNormalCCD::OnMouseUpBtnLeftjogup, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgTeachTabNormalCCD, IDC_BTN_LEFTJOGLEFT, 6, CDlgTeachTabNormalCCD::OnMouseDownBtnLeftjogleft, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgTeachTabNormalCCD, IDC_BTN_LEFTJOGLEFT, 8, CDlgTeachTabNormalCCD::OnMouseUpBtnLeftjogleft, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgTeachTabNormalCCD, IDC_BTN_LEFTJOGDOWN, 6, CDlgTeachTabNormalCCD::OnMouseDownBtnLeftjogdown, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgTeachTabNormalCCD, IDC_BTN_LEFTJOGDOWN, 8, CDlgTeachTabNormalCCD::OnMouseUpBtnLeftjogdown, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgTeachTabNormalCCD, IDC_BTN_LEFTJOGRIGHT, 6, CDlgTeachTabNormalCCD::OnMouseDownBtnLeftjogright, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgTeachTabNormalCCD, IDC_BTN_LEFTJOGRIGHT, 8, CDlgTeachTabNormalCCD::OnMouseUpBtnLeftjogright, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgTeachTabNormalCCD, IDC_BTN_RIGHTJOGUP, 6, CDlgTeachTabNormalCCD::OnMouseDownBtnRightjogup, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgTeachTabNormalCCD, IDC_BTN_RIGHTJOGUP, 8, CDlgTeachTabNormalCCD::OnMouseUpBtnRightjogup, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgTeachTabNormalCCD, IDC_BTN_RIGHTJOGDOWN, 6, CDlgTeachTabNormalCCD::OnMouseDownBtnRightjogdown, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgTeachTabNormalCCD, IDC_BTN_RIGHTJOGDOWN, 8, CDlgTeachTabNormalCCD::OnMouseUpBtnRightjogdown, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgTeachTabNormalCCD, IDC_BTN_RIGHTJOGLEFT, 6, CDlgTeachTabNormalCCD::OnMouseDownBtnRightjogleft, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgTeachTabNormalCCD, IDC_BTN_RIGHTJOGLEFT, 8, CDlgTeachTabNormalCCD::OnMouseUpBtnRightjogleft, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgTeachTabNormalCCD, IDC_BTN_RIGHTJOGRIGHT, 6, CDlgTeachTabNormalCCD::OnMouseDownBtnRightjogright, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgTeachTabNormalCCD, IDC_BTN_RIGHTJOGRIGHT, 8, CDlgTeachTabNormalCCD::OnMouseUpBtnRightjogright, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgTeachTabNormalCCD, IDC_MSFLEXGRID_POS, DISPID_CLICK, CDlgTeachTabNormalCCD::OnClickGrid1, VTS_NONE)
END_EVENTSINK_MAP()

void CDlgTeachTabNormalCCD::OnChangeJogSpeed()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if((m_ctrlTiltChk.GetCheck() == BST_CHECKED) && (GetLineType() == REMOVE_TILTING_TYPE))
	{
		switch( m_SliderSpeed.get_Position() )
		{
		case 0:	m_dJogSpeed = 50; break;
		case 1:	m_dJogSpeed = 100; break;
		case 2:	m_dJogSpeed = 150; break;
		case 3:	m_dJogSpeed = 200; break;
		case 4:	m_dJogSpeed = 250; break;
		}
	}
	else
	{
		switch( m_SliderSpeed.get_Position() )
		{
		case 0:	m_dJogSpeed = 1; break;
		case 1:	m_dJogSpeed = 10; break;
		case 2:	m_dJogSpeed = 20; break;
		case 3:	m_dJogSpeed = 30; break;
		case 4:	m_dJogSpeed = 60; break;
		}
	}

	if(GetLineType()==REMOVE_TILTING_TYPE)	// 2009.06.15
		GetMelsec()->MelsecNetWordSend(TILT_OUTPUT_JOG_SPEED, (short)m_dJogSpeed);
}




void CDlgTeachTabNormalCCD::OnMouseDownBtnLeftjogup(short Button, short Shift, long X, long Y)
{
	if(m_ctrlTiltChk.GetCheck() == 0)
	{
		if(GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter == TRUE)
		{
			if(m_bLeftUpEvent == FALSE)
			{
				m_ctrlLeftJogUp.put_Active(TRUE);
				GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_Y1 , false);
				GetDoc()->m_pMotion->MoveJog( AXIS_CCD_Y1 , GetDoc()->m_pMotorParam[AXIS_CCD_Y1].iAcc , m_dJogSpeed , TRUE );
				TRACE("Y Move Negative \n" );
				m_bLeftUpEvent = TRUE;
			}
			else
				return;
		}
		else
		{
			if(m_bLeftUpEvent == FALSE)
			{
				m_ctrlLeftJogUp.put_Active(TRUE);
				GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_Y0 , false);
				GetDoc()->m_pMotion->MoveJog( AXIS_CCD_Y0 , GetDoc()->m_pMotorParam[AXIS_CCD_Y0].iAcc , m_dJogSpeed , TRUE );
				TRACE("Y Move Negative \n" );
				m_bLeftUpEvent = TRUE;
			}
			else
				return;
		}
	}
	else
	{
		if(m_bLeftUpEvent == FALSE)
		{
			if( GetLineType() == REMOVE_TILTING_TYPE )
			{
				CString str;
				GetMelsec()->MelsecNetBitOn(MEL_SW2_TILT_JOG_P);
			}
			else
			{
	
				GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_TILT_MOVE , false);
				GetDoc()->m_pMotion->MoveJog( AXIS_TILT_MOVE , GetDoc()->m_pMotorParam[AXIS_TILT_MOVE].iAcc , m_dJogSpeed , TRUE );
	
	
			}
			m_ctrlLeftJogUp.put_Active(TRUE);
	
			TRACE("TILT - Up Move Up \n" );
			m_bLeftUpEvent = TRUE;
		}
		else
			return;
	}
}

void CDlgTeachTabNormalCCD::OnMouseUpBtnLeftjogup(short Button, short Shift, long X, long Y)
{
	if(m_ctrlTiltChk.GetCheck() == 0)
	{
		if(GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter == TRUE)
		{
			TRACE( "Mouse Up Jog Left Y \n");
			GetDoc()->m_pMotion->MoveStop( AXIS_CCD_Y1);
			GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_Y1 , false);
			m_bLeftUpEvent = FALSE;
			m_ctrlLeftJogUp.put_Active(FALSE);
		}
		else
		{
			TRACE( "Mouse Up Jog Left Y \n");
			GetDoc()->m_pMotion->MoveStop( AXIS_CCD_Y0);
			GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_Y0 , false);
			m_bLeftUpEvent = FALSE;
			m_ctrlLeftJogUp.put_Active(FALSE);

		}
	}
	else
	{
		TRACE( "Mouse Up Jog TILT - Up Stop \n");

		if( GetLineType() == REMOVE_TILTING_TYPE )
		{
			GetMelsec()->MelsecNetBitOff(MEL_SW2_TILT_JOG_P);
		}
		else
		{
			GetDoc()->m_pMotion->MoveStop( AXIS_TILT_MOVE);
			GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_TILT_MOVE , false);
		}

		m_bLeftUpEvent = FALSE;
		m_ctrlLeftJogUp.put_Active(FALSE);

	}

}

void CDlgTeachTabNormalCCD::OnMouseDownBtnLeftjogleft(short Button, short Shift, long X, long Y)
{
	if(GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter == TRUE)
	{
		if(m_bLeftLeftEvent == FALSE)
		{
			m_ctrlLeftJogLeft.put_Active(TRUE);
			GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_X1 , true);
			GetDoc()->m_pMotion->MoveJog( AXIS_CCD_X1 , GetDoc()->m_pMotorParam[AXIS_CCD_X1].iAcc , m_dJogSpeed , FALSE );
			TRACE("X Move Negative \n" );
			m_bLeftLeftEvent = TRUE;
		}
		else
			return;
	}
	else
	{
		if(m_bLeftLeftEvent == FALSE)
		{
			m_ctrlLeftJogLeft.put_Active(TRUE);
			GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_X0 , true);
			GetDoc()->m_pMotion->MoveJog( AXIS_CCD_X0 , GetDoc()->m_pMotorParam[AXIS_CCD_X0].iAcc , m_dJogSpeed , FALSE );
			TRACE("X Move Negative \n" );
			m_bLeftLeftEvent = TRUE;
		}
		else
			return;
	}
}

void CDlgTeachTabNormalCCD::OnMouseUpBtnLeftjogleft(short Button, short Shift, long X, long Y)
{
	if(GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter == TRUE)
	{
		TRACE( "Mouse Up Jog Right X \n");
	GetDoc()->m_pMotion->MoveStop( AXIS_CCD_X1);
		GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_X1 , true);
		m_bLeftLeftEvent = FALSE;
		m_ctrlLeftJogLeft.put_Active(FALSE);
	}
	else
	{
		TRACE( "Mouse Up Jog Right X \n");
		GetDoc()->m_pMotion->MoveStop( AXIS_CCD_X0);
		GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_X0, true);
		m_bLeftLeftEvent = FALSE;
		m_ctrlLeftJogLeft.put_Active(FALSE);
	}
}

void CDlgTeachTabNormalCCD::OnMouseDownBtnLeftjogdown(short Button, short Shift, long X, long Y)
{
	if(m_ctrlTiltChk.GetCheck() == 0)
	{
	if(GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter == TRUE)
	{
		if(m_bLeftDownEvent == FALSE)
		{
			m_ctrlLeftJogDown.put_Active(TRUE);
			GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_Y1 , true);
			GetDoc()->m_pMotion->MoveJog( AXIS_CCD_Y1 , GetDoc()->m_pMotorParam[AXIS_CCD_Y1].iAcc , m_dJogSpeed , FALSE );
			TRACE("Y Move Positive \n" );
			m_bLeftDownEvent = TRUE;
		}
		else
			return;
	}
	else
	{
		if(m_bLeftDownEvent == FALSE)
		{
			m_ctrlLeftJogDown.put_Active(TRUE);
			GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_Y0 , true);
			GetDoc()->m_pMotion->MoveJog( AXIS_CCD_Y0 , GetDoc()->m_pMotorParam[AXIS_CCD_Y0].iAcc , m_dJogSpeed , FALSE );
			TRACE("Y Move Positive \n" );
			m_bLeftDownEvent = TRUE;
		}
		else
			return;
	}
	}
	else
	{
		if(m_bLeftDownEvent == FALSE)
		{
			m_ctrlLeftJogDown.put_Active(TRUE);
			if( GetLineType() == REMOVE_TILTING_TYPE )
			{
				CString str;
				GetMelsec()->MelsecNetBitOn(MEL_SW2_TILT_JOG_N);
			}
			else
			{
				GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_TILT_MOVE , true);
				GetDoc()->m_pMotion->MoveJog( AXIS_TILT_MOVE , GetDoc()->m_pMotorParam[AXIS_TILT_MOVE].iAcc , m_dJogSpeed , FALSE );

			}

			TRACE("TILT - Down Move Down \n" );
			m_bLeftDownEvent = TRUE;
		}
		else
			return;
	}
}

void CDlgTeachTabNormalCCD::OnMouseUpBtnLeftjogdown(short Button, short Shift, long X, long Y)
{
	if(m_ctrlTiltChk.GetCheck() == 0)
	{
	if(GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter == TRUE)
	{
	TRACE( "Mouse Up Jog Left Y \n");
	GetDoc()->m_pMotion->MoveStop( AXIS_CCD_Y1);
			GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_Y1 , true);
		m_bLeftDownEvent = FALSE;
		m_ctrlLeftJogDown.put_Active(FALSE);
	}
	else
	{
		TRACE( "Mouse Up Jog Left Y \n");
		GetDoc()->m_pMotion->MoveStop( AXIS_CCD_Y0);
			GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_Y0 , true);
		m_bLeftDownEvent = FALSE;
		m_ctrlLeftJogDown.put_Active(FALSE);
	}
	}
	else
	{
		TRACE( "Mouse Up Jog Left TILT - DOWN Stop \n");

		if( GetLineType() == REMOVE_TILTING_TYPE )
		{
			CString str;
			GetMelsec()->MelsecNetBitOff(MEL_SW2_TILT_JOG_N);
		}
		else
		{
			GetDoc()->m_pMotion->MoveStop( AXIS_TILT_MOVE);
			GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_TILT_MOVE , true);

		}

		m_bLeftDownEvent = FALSE;
		m_ctrlLeftJogDown.put_Active(FALSE);
	}

}

void CDlgTeachTabNormalCCD::OnMouseDownBtnLeftjogright(short Button, short Shift, long X, long Y)
{
	if(GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter == TRUE)
	{
		if(m_bLeftRightEvent == FALSE)
		{
			GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_X1 , false);
			GetDoc()->m_pMotion->MoveJog( AXIS_CCD_X1 , GetDoc()->m_pMotorParam[AXIS_CCD_X1].iAcc , m_dJogSpeed , TRUE );
			TRACE("X Move Positive \n" );
			m_bLeftRightEvent = TRUE;
			m_ctrlLeftJogRight.put_Active(TRUE);
		}
		else
			return;
	}
	else
	{
		if(m_bLeftRightEvent == FALSE)
		{
			GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_X0 , false);
			GetDoc()->m_pMotion->MoveJog( AXIS_CCD_X0 , GetDoc()->m_pMotorParam[AXIS_CCD_X0].iAcc , m_dJogSpeed , TRUE);
			TRACE("X Move Positive \n" );
			m_bLeftRightEvent = TRUE;
			m_ctrlLeftJogRight.put_Active(TRUE);
		}
		else
			return;
	}
}

void CDlgTeachTabNormalCCD::OnMouseUpBtnLeftjogright(short Button, short Shift, long X, long Y)
{
	if(GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter == TRUE)
	{
		TRACE( "Mouse Up Jog Right X \n");
		GetDoc()->m_pMotion->MoveStop( AXIS_CCD_X1);
		GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_X1 , true);
		m_bLeftRightEvent = FALSE;
		m_ctrlLeftJogRight.put_Active(FALSE);
	}
	else
	{
		TRACE( "Mouse Up Jog Right X \n");
		GetDoc()->m_pMotion->MoveStop( AXIS_CCD_X0);
		GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_X0 , true);
		m_bLeftRightEvent = FALSE;
		m_ctrlLeftJogRight.put_Active(FALSE);
	}
}

void CDlgTeachTabNormalCCD::OnMouseDownBtnRightjogup(short Button, short Shift, long X, long Y)
{
	if(GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter == TRUE)
	{
		if(m_bRightUpEvent == FALSE)
		{
			GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_Y2 , false);
			GetDoc()->m_pMotion->MoveJog( AXIS_CCD_Y2 , GetDoc()->m_pMotorParam[AXIS_CCD_Y2].iAcc , m_dJogSpeed , TRUE );
			TRACE("Y Move Negative \n" );
			m_bRightUpEvent = TRUE;
			m_ctrlRightJogUp.put_Active(TRUE);
	}
	else
		return;
	}
	else
	{
		if(m_bRightUpEvent == FALSE)
		{
			GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_Y3 , false);
			GetDoc()->m_pMotion->MoveJog( AXIS_CCD_Y3 , GetDoc()->m_pMotorParam[AXIS_CCD_Y3].iAcc , m_dJogSpeed , TRUE );
			TRACE("Y Move Negative \n" );
			m_bRightUpEvent = TRUE;
			m_ctrlRightJogUp.put_Active(TRUE);
		}
		else
			return;
	}
}

void CDlgTeachTabNormalCCD::OnMouseUpBtnRightjogup(short Button, short Shift, long X, long Y)
{
	if(GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter == TRUE)
	{
		TRACE( "Mouse Up Jog Right Y \n");
		GetDoc()->m_pMotion->MoveStop( AXIS_CCD_Y2);
		GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_Y2 , false);
		m_bRightUpEvent = FALSE;
		m_ctrlRightJogUp.put_Active(FALSE);
	}
	else
	{
		TRACE( "Mouse Up Jog Right Y \n");
		GetDoc()->m_pMotion->MoveStop( AXIS_CCD_Y3);
		GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_Y3 , false);
		m_bRightUpEvent = FALSE;
		m_ctrlRightJogUp.put_Active(FALSE);
	}
}

void CDlgTeachTabNormalCCD::OnMouseDownBtnRightjogdown(short Button, short Shift, long X, long Y)
{
	if(GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter == TRUE)
	{
		if(m_bRightDownEvent == FALSE)
		{
			GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_Y2 , true);
			GetDoc()->m_pMotion->MoveJog( AXIS_CCD_Y2 , GetDoc()->m_pMotorParam[AXIS_CCD_Y2].iAcc , m_dJogSpeed , FALSE );
			TRACE("Y Move Positive \n" );
			m_bRightDownEvent = TRUE;
			m_ctrlRightJogDown.put_Active(TRUE);
		}
		else
			return;
	}
	else
	{
		if(m_bRightDownEvent == FALSE)
		{
			GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_Y3 , true);
			GetDoc()->m_pMotion->MoveJog( AXIS_CCD_Y3 , GetDoc()->m_pMotorParam[AXIS_CCD_Y3].iAcc , m_dJogSpeed , FALSE );
		TRACE("Y Move Positive \n" );
			m_bRightDownEvent = TRUE;
			m_ctrlRightJogDown.put_Active(TRUE);
	}
	else
		return;
	}
}

void CDlgTeachTabNormalCCD::OnMouseUpBtnRightjogdown(short Button, short Shift, long X, long Y)
{
	if(GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter == TRUE)
	{
		TRACE( "Mouse Up Jog Right Y \n");
		GetDoc()->m_pMotion->MoveStop( AXIS_CCD_Y2);
		GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_Y2 , false);
		m_bRightDownEvent = FALSE;
		m_ctrlRightJogDown.put_Active(FALSE);
	}
	else
	{
		TRACE( "Mouse Up Jog Right Y \n");
		GetDoc()->m_pMotion->MoveStop( AXIS_CCD_Y3);
		GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_Y3 , false);
		m_bRightDownEvent = FALSE;
		m_ctrlRightJogDown.put_Active(FALSE);
	}
}

void CDlgTeachTabNormalCCD::OnMouseDownBtnRightjogleft(short Button, short Shift, long X, long Y)
{
	if(GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter == TRUE)
	{
		if(m_bRightLeftEvent == FALSE)
	{
		GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_X2 , false);
		GetDoc()->m_pMotion->MoveJog( AXIS_CCD_X2 , GetDoc()->m_pMotorParam[AXIS_CCD_X2].iAcc , m_dJogSpeed , TRUE );
			TRACE("X Move Negative \n" );
			m_bRightLeftEvent = TRUE;
			m_ctrlRightJogLeft.put_Active(TRUE);
	}
	else
			return;
	}
	else
	{
		if(m_bRightLeftEvent == FALSE)
		{
			GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_X3 , false);
			GetDoc()->m_pMotion->MoveJog( AXIS_CCD_X3 , GetDoc()->m_pMotorParam[AXIS_CCD_X3].iAcc , m_dJogSpeed , TRUE );
			TRACE("X Move Negative \n" );
			m_bRightLeftEvent = TRUE;
			m_ctrlRightJogLeft.put_Active(TRUE);
	}
	else
		return;
	}

}

void CDlgTeachTabNormalCCD::OnMouseUpBtnRightjogleft(short Button, short Shift, long X, long Y)
{
	if(GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter == TRUE)
	{
		TRACE( "Mouse Up Jog Right X \n");
		GetDoc()->m_pMotion->MoveStop( AXIS_CCD_X2);
		GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_X2 , false);
		m_bRightLeftEvent = FALSE;
		m_ctrlRightJogLeft.put_Active(FALSE);
	}
	else
	{
		TRACE( "Mouse Up Jog Right X \n");
		GetDoc()->m_pMotion->MoveStop( AXIS_CCD_X3);
		GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_X3 , false);
		m_bRightLeftEvent = FALSE;
		m_ctrlRightJogLeft.put_Active(FALSE);
	}

}

void CDlgTeachTabNormalCCD::OnMouseDownBtnRightjogright(short Button, short Shift, long X, long Y)
{
	if(GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter == TRUE)
	{
		if(m_bRightRightEvent == FALSE)
		{
			GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_X2 , true);
			GetDoc()->m_pMotion->MoveJog( AXIS_CCD_X2 , GetDoc()->m_pMotorParam[AXIS_CCD_X2].iAcc , m_dJogSpeed , FALSE );
			TRACE("X Move Positive \n" );
			m_bRightRightEvent = TRUE;
			m_ctrlRightJogRight.put_Active(TRUE);
		}
		else
			return;
	}
	else
	{
		if(m_bRightRightEvent == FALSE)
		{
			GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_X3 , true);
			GetDoc()->m_pMotion->MoveJog( AXIS_CCD_X3 , GetDoc()->m_pMotorParam[AXIS_CCD_X3].iAcc , m_dJogSpeed , FALSE );
			TRACE("X Move Positive \n" );
			m_bRightRightEvent = TRUE;
			m_ctrlRightJogRight.put_Active(TRUE);
	}
	else
		return;
	}

}

void CDlgTeachTabNormalCCD::OnMouseUpBtnRightjogright(short Button, short Shift, long X, long Y)
{
	if(GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter == TRUE)
	{
		TRACE( "Mouse Up Jog Right X \n");
		GetDoc()->m_pMotion->MoveStop( AXIS_CCD_X2);
		GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_X2 , false);
		m_bRightRightEvent = FALSE;
		m_ctrlRightJogRight.put_Active(FALSE);
	}
	else
	{
		TRACE( "Mouse Up Jog Right X \n");
		GetDoc()->m_pMotion->MoveStop( AXIS_CCD_X3);
		GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_X3 , false);
		m_bRightRightEvent = FALSE;
		m_ctrlRightJogRight.put_Active(FALSE);
	}

}

void CDlgTeachTabNormalCCD::OnBnClickedChkTilt()
{
	// TODO: Add your control notification handler code here
	int nTilt;
	nTilt = m_ctrlTiltChk.GetCheck();
	if(nTilt == BST_CHECKED)
	{
		m_SliderSpeed.put_PositionLabels("50mm, 100mm, 150mm, 200mm, 250mm");
		m_ctrlLeftJogUp.put_Visible(TRUE);
		m_ctrlLeftJogDown.put_Visible(TRUE);
		m_ctrlLeftJogLeft.put_Visible(FALSE);
		m_ctrlLeftJogRight.put_Visible(FALSE);
		m_ctrlRightJogUp.put_Visible(FALSE);
		m_ctrlRightJogDown.put_Visible(FALSE);
		m_ctrlRightJogLeft.put_Visible(FALSE);
		m_ctrlRightJogRight.put_Visible(FALSE);
		m_lbCurPos[LABEL_CUR_POS_RX].put_Caption("Not Use");
		m_lbCurPos[LABEL_CUR_POS_LY].put_Caption("Not Use");
		m_lbCurPos[LABEL_CUR_POS_RY].put_Caption("Not Use");
		m_lblAutoCurrentModel.put_Caption("Z Axis");
		m_lblAutoCurrentModel2.put_Caption("Not Use");
		m_cGridPosVal.put_TextMatrix(0,1,"Pos");
		m_cGridPosVal.put_TextMatrix(0,2,"Not use");
		m_cGridPosVal.put_TextMatrix(1,0 , " ");
		m_cGridPosVal.put_TextMatrix(2,0 , " ");
		m_btnNormalCCD[BTN_READY_POS].SetWindowText("Wait Position");
		m_btnNormalCCD[BTN_GRAP_POS].SetWindowText("Tilt Position");
		KillTimer(NORMAL_CUR_POS);
	}
	else
	{
		m_SliderSpeed.put_PositionLabels("1mm, 10mm, 20mm, 30mm, 60mm");
		m_ctrlLeftJogUp.put_Visible(TRUE);
		m_ctrlLeftJogDown.put_Visible(TRUE);
		m_ctrlLeftJogLeft.put_Visible(TRUE);
		m_ctrlLeftJogRight.put_Visible(TRUE);
		m_ctrlRightJogUp.put_Visible(TRUE);
		m_ctrlRightJogDown.put_Visible(TRUE);
		m_ctrlRightJogLeft.put_Visible(TRUE);
		m_ctrlRightJogRight.put_Visible(TRUE);
		m_lbCurPos[LABEL_CUR_POS_RX].put_Caption("0.0");
		m_lbCurPos[LABEL_CUR_POS_LY].put_Caption("0.0");
		m_lbCurPos[LABEL_CUR_POS_RY].put_Caption("0.0");
		m_lblAutoCurrentModel.put_Caption("X Axis");
		m_lblAutoCurrentModel2.put_Caption("Y Axis");
		m_cGridPosVal.put_TextMatrix(0,1,"X Axis");
		m_cGridPosVal.put_TextMatrix(0,2,"Y Axis");
		m_cGridPosVal.put_TextMatrix(1,0 , "Left");
		m_cGridPosVal.put_TextMatrix(2,0 , "Right");
		m_btnNormalCCD[BTN_READY_POS].SetWindowText("Ready Position");
		m_btnNormalCCD[BTN_GRAP_POS].SetWindowText("Grab Position");
		SetTimer(NORMAL_CUR_POS, 100, NULL);
	}
	if(m_iCurSetPos == READY_POS)
		SetGridReadyPos();
	else if(m_iCurSetPos == GRAB_POS)
		SetGridGrabPos();
	else
		SetGridPCBChangePos();
	if( GetLineType() == REMOVE_TILTING_TYPE )
		SetCurposDisplay();
}

void CDlgTeachTabNormalCCD::OnClickGrid1()
{
	// TODO: Add your message handler code here
	int nCol = m_cGridPosVal.get_MouseCol();
	int nRow = m_cGridPosVal.get_MouseRow();
	CString str;

	CDlgNumPad	 dlg;
	POSITION_DATA* pCurPosData;
	if(m_iCurSetPos == READY_POS)
		pCurPosData = m_dReadyPos;
	else
		pCurPosData = m_dGrabPos;

	if( nRow == 0 || nCol == 0)
		return;

	if( dlg.DoModal() == IDOK )
	{
		int nAxis;
		double dValue = (double)atof(dlg.m_strEditText);
		if(m_ctrlTiltChk.GetCheck() == BST_CHECKED)
			nAxis = AXIS_TILT_MOVE;
		else
		{
			if( GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter )
			{
				if(nRow == 1)
				{
					if(nCol == 1)		// X
						nAxis = AXIS_CCD_X1;
					else if(nCol == 2)	// Y
						nAxis = AXIS_CCD_Y1;
				}
				else if(nRow == 2)
				{
					if(nCol == 1)		// X
						nAxis = AXIS_CCD_X2;
					else if(nCol == 2)	// Y
						nAxis = AXIS_CCD_Y2;
				}
			}
			else
			{
				if(nRow == 1)
				{
					if(nCol == 1)		// X
						nAxis = AXIS_CCD_X0;
					else if(nCol == 2)	// Y
						nAxis = AXIS_CCD_Y0;
				}
				else if(nRow == 2)
				{
					if(nCol == 1)		// X
						nAxis = AXIS_CCD_X3;
					else if(nCol == 2)	// Y
						nAxis = AXIS_CCD_Y3;
				}
			}
			if(nCol == 1 && GetDoc()->m_dMotionstrokeSingleX < dValue)
			{
				AfxMessageBox("This value is over the X-Axis stroke");
				return;
			}
			if((nAxis == AXIS_CCD_X0) && (pCurPosData[CAM_1].dx < dValue))
			{
				AfxMessageBox("This value is over the X1-Axis position");
				return;
			}
			if(nAxis == AXIS_CCD_X1)
			{
				if(pCurPosData[CAM_0].dx > dValue)
				{
					AfxMessageBox("This value is Lower the X0-Axis position");
					return;
				}
				if((pCurPosData[CAM_2].dx + dValue) > GetDoc()->m_dMotionstrokeFullX)
				{
					AfxMessageBox("This value is over the X-Axis full stroke");
					return;
				}
			}
			if(nAxis == AXIS_CCD_X2)
			{
				if(pCurPosData[CAM_3].dx > dValue)
				{
					AfxMessageBox("This value is Lower the X3-Axis position");
					return;
				}
				if((pCurPosData[CAM_1].dx + dValue) > GetDoc()->m_dMotionstrokeFullX)
				{
					AfxMessageBox("This value is over the X-Axis full stroke");
					return;
				}
			}
			if((nAxis == AXIS_CCD_X3) && (pCurPosData[CAM_2].dx < dValue))
			{
				AfxMessageBox("This value is over the X2-Axis position");
				return;
			}
		}
		if(GetDoc()->m_pMotorParam[nAxis].dPosSoftLimit < dValue)
		{
			AfxMessageBox("This value is over the S/W Positive limit");
			return;
		}
		if(GetDoc()->m_pMotorParam[nAxis].dNegSoftLimit > dValue)
		{
			AfxMessageBox("This value is lower the S/W Negative limit");
			return;
		}

		str.Format("%s",dlg.m_strEditText);
		m_cGridPosVal.put_TextMatrix(nRow, nCol, str);
	}


	if( m_iCurSetPos == READY_POS )
	{
		if(m_ctrlTiltChk.GetCheck() == BST_UNCHECKED)
		{
			if( GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter )
			{
				m_dReadyPos[CAM_1].dx = atof(m_cGridPosVal.get_TextMatrix( 1 , 1 ));
				m_dReadyPos[CAM_1].dy = atof(m_cGridPosVal.get_TextMatrix( 1 , 2 ));
				m_dReadyPos[CAM_2].dx = atof(m_cGridPosVal.get_TextMatrix( 2 , 1 ));
				m_dReadyPos[CAM_2].dy = atof(m_cGridPosVal.get_TextMatrix( 2 , 2 ));
			}
			else
			{
				m_dReadyPos[CAM_0].dx = atof(m_cGridPosVal.get_TextMatrix( 1 , 1 ));
				m_dReadyPos[CAM_0].dy = atof(m_cGridPosVal.get_TextMatrix( 1 , 2 ));
				m_dReadyPos[CAM_3].dx = atof(m_cGridPosVal.get_TextMatrix( 2 , 1 ));
				m_dReadyPos[CAM_3].dy = atof(m_cGridPosVal.get_TextMatrix( 2 , 2 ));
			}
		}
		else
		{
			m_dUnTiltingPos =  atof(m_cGridPosVal.get_TextMatrix( 1 , 1));
		}
	}
	else if( m_iCurSetPos == GRAB_POS )
	{
		if(m_ctrlTiltChk.GetCheck() == BST_UNCHECKED)
		{
			if( GetDoc()->m_pThreadLiveGrap->m_bSetTeachCenter )
			{
				m_dGrabPos[CAM_1].dx = atof(m_cGridPosVal.get_TextMatrix( 1 , 1));
				m_dGrabPos[CAM_1].dy = atof(m_cGridPosVal.get_TextMatrix( 1 , 2));
				m_dGrabPos[CAM_2].dx = atof(m_cGridPosVal.get_TextMatrix( 2 , 1));
				m_dGrabPos[CAM_2].dy = atof(m_cGridPosVal.get_TextMatrix( 2 , 2));
			}
			else
			{
				m_dGrabPos[CAM_0].dx = atof(m_cGridPosVal.get_TextMatrix( 1 , 1));
				m_dGrabPos[CAM_0].dy = atof(m_cGridPosVal.get_TextMatrix( 1 , 2));
				m_dGrabPos[CAM_3].dx = atof(m_cGridPosVal.get_TextMatrix( 2 , 1));
				m_dGrabPos[CAM_3].dy = atof(m_cGridPosVal.get_TextMatrix( 2 , 2));
			}
		}
		else
		{
			m_dTiltingPos =  atof(m_cGridPosVal.get_TextMatrix( 1 , 1));
		}
	}
}

void CDlgTeachTabNormalCCD::SetGridPos()
{
	if(m_iCurSetPos == READY_POS)
	{
		m_btnNormalCCD[BTN_READY_POS].SetCheck(BST_CHECKED);
		SetGridReadyPos();
	}
	else if(m_iCurSetPos == GRAB_POS)
	{
		m_btnNormalCCD[BTN_GRAP_POS].SetCheck(BST_CHECKED);
		SetGridGrabPos();
	}
	else if(m_iCurSetPos == PCB_CHANGE_POS)
	{
		m_btnNormalCCD[BTN_PCB_CHANGE_POS].SetCheck(BST_CHECKED);
		SetGridPCBChangePos();
	}
}

BOOL CDlgTeachTabNormalCCD::CheckLimit( int nAxis, double dPos )
{
	if(GetDoc()->m_pMotorParam[nAxis].dPosSoftLimit < dPos)
	{
		AfxMessageBox("This value is over the S/W Positive limit");
		return FALSE;
	}
	if(GetDoc()->m_pMotorParam[nAxis].dNegSoftLimit > dPos)
	{
		AfxMessageBox("This value is lower the S/W Negative limit");
		return FALSE;
	}
	return TRUE;
}