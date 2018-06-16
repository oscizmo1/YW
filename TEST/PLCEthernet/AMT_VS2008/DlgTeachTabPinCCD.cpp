// DlgTeachTabPinCCD.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MT.h"
#include "DlgTeachTabPinCCD.h"


// CDlgTeachTabPinCCD 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTeachTabPinCCD, CDialog)

CDlgTeachTabPinCCD::CDlgTeachTabPinCCD(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTeachTabPinCCD::IDD, pParent)
{

	m_iZaxisJogSpeedCount = 7 ;
	m_dJogSpeed = 1 ;
	m_dJogXOldPos = 0.0 ;
	m_dJogYOldPos = 0.0 ;

	m_bInitDlg = FALSE;
	m_bCurViewShow = FALSE ;
	m_iCurSetPos = 0;

	m_bZaxisJogMoveEnable = FALSE ;
}

CDlgTeachTabPinCCD::~CDlgTeachTabPinCCD()
{
}

void CDlgTeachTabPinCCD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ISWITCHQUADX_POGOCAM, m_JogPadPogoCam);
	DDX_Control(pDX, IDC_ISWITCHSLIDERX_JOG_SPD, m_SliderSpeed);
	DDX_Control(pDX, IDC_MSFLEXGRID_POS, m_cGridPosVal);
	for( int i = 0 ; i < 3 ; i ++ )
		DDX_Control(pDX, IDC_LABEL_X_POS + i , m_lbCurPos[i]);

	DDX_Control(pDX, IDC_ISWITCHSLIDERX_ZAXIS_JOG, m_SliderZaxisJog);
}


BEGIN_MESSAGE_MAP(CDlgTeachTabPinCCD, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_SET_CUR_POS, &CDlgTeachTabPinCCD::OnSetCurPos)
	ON_BN_CLICKED(IDC_BTN_SET_POS_MOVE, &CDlgTeachTabPinCCD::OnSetPosMove)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO_GRAP_POS, &CDlgTeachTabPinCCD::OnBnClickedRadioGrapPos)
	ON_BN_CLICKED(IDC_RADIO_READY_POS, &CDlgTeachTabPinCCD::OnBnClickedRadioReadyPos)
	ON_BN_CLICKED(IDC_BUTTON_UP, &CDlgTeachTabPinCCD::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, &CDlgTeachTabPinCCD::OnBnClickedButtonDown)
END_MESSAGE_MAP()


// CDlgTeachTabPinCCD 메시지 처리기입니다.

BOOL CDlgTeachTabPinCCD::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cGridPosVal.put_Rows(2);
	m_cGridPosVal.put_Cols(3);

	m_cGridPosVal.put_ColWidth(0 , 700);	m_cGridPosVal.put_CellAlignment(4);
	m_cGridPosVal.put_ColWidth(1 , 927);
	m_cGridPosVal.put_ColWidth(2 , 927);

	for( int i =0 ; i < 2 ; i ++ )
	{
		for( int j=0; j < 3 ; j ++ )
		{
			m_cGridPosVal.put_Row(i) ;
			m_cGridPosVal.put_Col(j) ; 
			m_cGridPosVal.put_CellAlignment(4);
		}
	}

	m_cGridPosVal.put_RowHeight( 0 , 500);
	m_cGridPosVal.put_RowHeight( 1 , 550);

	m_cGridPosVal.put_TextMatrix( 0 , 0 , "X 축");
	m_cGridPosVal.put_TextMatrix( 0 , 1 , "Y 축");
	m_cGridPosVal.put_TextMatrix( 0 , 2 , "Z 축");
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


	m_SliderZaxisJog.put_PositionLabels("3mm , 2mm, 1mm, 0mm, -1mm , -2mm, -3mm");
	m_SliderZaxisJog.put_ShowPositionLabels(TRUE);
	m_SliderZaxisJog.put_ShowPositionIndicators(FALSE);
//	m_SliderZaxisJog.put_TrackStyle(3);
//	m_SliderZaxisJog.put_PositionIndicatorStyle(0);
//	m_SliderZaxisJog.put_PositionIndicatorBevelStyle(1);
//	m_SliderZaxisJog.put_Orientation(1);
//	m_SliderZaxisJog.put_PositionIndicatorSize(3);
//	m_SliderZaxisJog.put_PositionIndicatorMargin(10);
	m_SliderZaxisJog.put_PositionIndicatorColor(0x000000FF);
	m_SliderZaxisJog.put_PointerStyle(4);
	m_SliderZaxisJog.put_Position(m_iZaxisJogSpeedCount /2);

//	m_SliderZaxisJog.put_EndsMargin(15);


	//현재 Z축 값을 셋팅한다..
	

	SetInitDisplayData();
	SetGridReadyPos();
	SetCurposDisplay();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
BEGIN_EVENTSINK_MAP(CDlgTeachTabPinCCD, CDialog)
	ON_EVENT(CDlgTeachTabPinCCD, IDC_ISWITCHQUADX_POGOCAM, 5, CDlgTeachTabPinCCD::OnJogMoveXPogocam, VTS_NONE)
	ON_EVENT(CDlgTeachTabPinCCD, IDC_ISWITCHQUADX_POGOCAM, 6, CDlgTeachTabPinCCD::OnJogMoveYPogocam, VTS_NONE)
	ON_EVENT(CDlgTeachTabPinCCD, IDC_ISWITCHSLIDERX_JOG_SPD, 4, CDlgTeachTabPinCCD::OnChangeJogSpeed, VTS_NONE)
	ON_EVENT(CDlgTeachTabPinCCD, IDC_ISWITCHQUADX_POGOCAM, 14, CDlgTeachTabPinCCD::OnMouseUpJogPadPogoCam, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgTeachTabPinCCD, IDC_ISWITCHSLIDERX_ZAXIS_JOG, 12, CDlgTeachTabPinCCD::OnPositionMoveFinish, VTS_NONE)
	ON_EVENT(CDlgTeachTabPinCCD, IDC_ISWITCHSLIDERX_ZAXIS_JOG, 5, CDlgTeachTabPinCCD::OnClickIswitchsliderxZaxisJog, VTS_NONE)
	ON_EVENT(CDlgTeachTabPinCCD, IDC_ISWITCHSLIDERX_ZAXIS_JOG, 4, CDlgTeachTabPinCCD::OnZaxisJogSpeedChange, VTS_NONE)
	ON_EVENT(CDlgTeachTabPinCCD, IDC_ISWITCHSLIDERX_ZAXIS_JOG, 1, CDlgTeachTabPinCCD::OnPositionChangeUserIswitchsliderxZaxisJog, VTS_NONE)
	ON_EVENT(CDlgTeachTabPinCCD, IDC_ISWITCHSLIDERX_ZAXIS_JOG, 7, CDlgTeachTabPinCCD::OnMouseDownIswitchsliderxZaxisJog, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void CDlgTeachTabPinCCD::OnJogMoveXPogocam()
{
	//여기서..Contact 상태를 check 해야 한다...

	if( m_bCurViewShow == FALSE )	return;
	if(  m_dJogXOldPos > m_JogPadPogoCam.get_ValueX()  )		// + , - 방향을 설정 하기 위함...
	{
		GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_X3 , false);
		GetDoc()->m_pMotion->MoveJog( AXIS_CCD_X3 , GetDoc()->m_pMotorParam[AXIS_CCD_X3].iAcc , m_dJogSpeed , FALSE );
		TRACE("X Move Negative \n" );
	}
	else
	{
		GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_X3 , true);
		GetDoc()->m_pMotion->MoveJog( AXIS_CCD_X3 , GetDoc()->m_pMotorParam[AXIS_CCD_X3].iAcc , m_dJogSpeed , TRUE );
		TRACE("X Move Posttive \n" );
	}
	m_dJogXOldPos = m_JogPadPogoCam.get_ValueX() ;}

void CDlgTeachTabPinCCD::OnJogMoveYPogocam()
{
	if( m_bCurViewShow == FALSE )	return;

	if(  m_dJogYOldPos > m_JogPadPogoCam.get_ValueY()  )		// + , - 방향을 설정 하기 위함...
	{
		GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_Y3 , false);
		GetDoc()->m_pMotion->MoveJog( AXIS_CCD_Y3 , GetDoc()->m_pMotorParam[AXIS_CCD_Y3].iAcc , m_dJogSpeed , FALSE );
		TRACE("Y Move Negative \n" );
	}
	else
	{
		GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_Y3 , true);
		GetDoc()->m_pMotion->MoveJog( AXIS_CCD_Y3 , GetDoc()->m_pMotorParam[AXIS_CCD_Y3].iAcc , m_dJogSpeed , TRUE );

		TRACE("Y Move Positive \n" );
	}
	m_dJogYOldPos = m_JogPadPogoCam.get_ValueY() ;}

void CDlgTeachTabPinCCD::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	m_bCurViewShow = bShow ;

	if( bShow )
	{
	
		SetInitDisplayData();

		m_iCurSetPos = READY_POS ;

		CheckRadioButton(IDC_RADIO_GRAP_POS , IDC_RADIO_READY_POS , IDC_RADIO_READY_POS ) ;
		SetGridReadyPos();

		if( m_bInitDlg )
		{
			SetTimer( PIN_CUR_POS , 100 , NULL);
		}

		m_bInitDlg = TRUE ;
	}
	else
	{

		m_JogPadPogoCam.put_ValueY(0.0);
		m_JogPadPogoCam.put_ValueX(0.0);

		m_dJogXOldPos = 0.0 ;
		m_dJogYOldPos = 0.0 ;


		KillTimer(PIN_CUR_POS);
		//Timer 죽이기...
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CDlgTeachTabPinCCD::SetInitDisplayData(void)
{
	m_dGrabPos.dx = GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[CAM_2].dx;
	m_dGrabPos.dy = GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[CAM_2].dy;
	m_dGrabPos.dz = GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[CAM_2].dz;

	m_dReadyPos.dx = GetDoc()->m_pAmtData->m_CurModelInfo.ReadyPos[CAM_2].dx;
	m_dReadyPos.dy = GetDoc()->m_pAmtData->m_CurModelInfo.ReadyPos[CAM_2].dy;
	m_dReadyPos.dz = GetDoc()->m_pAmtData->m_CurModelInfo.ReadyPos[CAM_2].dz;
}

void CDlgTeachTabPinCCD::SetCurposDisplay(void)
{
	CString strCurPos;
	strCurPos.Format("%.3f" , GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X3) );
	m_lbCurPos[0].put_Caption( strCurPos) ;

	strCurPos.Format("%.3f" , GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y3) );
	m_lbCurPos[1].put_Caption( strCurPos) ;

	strCurPos.Format("%.3f" , GetDoc()->m_pMotion->GetCmdPos(AXIS_CONT_PIN_Z) );
	m_lbCurPos[2].put_Caption( strCurPos) ;
}

void CDlgTeachTabPinCCD::SetGridReadyPos(void)
{
	CString strPosVal;

	strPosVal.Format("%.3f" , m_dReadyPos.dx);
	m_cGridPosVal.put_TextMatrix( 1 , 0 , strPosVal );

	strPosVal.Format("%.3f" , m_dReadyPos.dy);
	m_cGridPosVal.put_TextMatrix( 1 ,1 , strPosVal );

	strPosVal.Format("%.3f" , m_dReadyPos.dz);
	m_cGridPosVal.put_TextMatrix( 1 , 2 , strPosVal );

	m_cGridPosVal.Refresh();
}

void CDlgTeachTabPinCCD::SetGridGrabPos(void)
{
	CString strPosVal;

	strPosVal.Format("%.3f" , m_dGrabPos.dx);
	m_cGridPosVal.put_TextMatrix( 1 , 0 , strPosVal );

	strPosVal.Format("%.3f" , m_dGrabPos.dy);
	m_cGridPosVal.put_TextMatrix( 1 ,1 , strPosVal );

	strPosVal.Format("%.3f" , m_dGrabPos.dz);
	m_cGridPosVal.put_TextMatrix( 1 , 2 , strPosVal );

	m_cGridPosVal.Refresh();
}

void CDlgTeachTabPinCCD::OnSetCurPos()
{
	if( m_iCurSetPos == READY_POS )
	{
		// 메시지 박스 Dispaly..
		m_dReadyPos.dx = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X3);
		m_dReadyPos.dy = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y3);
		m_dReadyPos.dz = GetDoc()->m_pMotion->GetCmdPos(AXIS_CONT_PIN_Z);

		SetGridReadyPos();

	}
	else
	{
		m_dGrabPos.dx = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X3);
		m_dGrabPos.dy = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y3);
		m_dGrabPos.dz = GetDoc()->m_pMotion->GetCmdPos(AXIS_CONT_PIN_Z);

		SetGridGrabPos();
	}
}

void CDlgTeachTabPinCCD::OnSetPosMove()
{
	double dTargetPosX ,  dTargetPosY , dTargetPosZ ;

	m_cGridPosVal.put_Col(0) , m_cGridPosVal.put_Row(1);	dTargetPosX = atof( m_cGridPosVal.get_Text() );
	m_cGridPosVal.put_Col(1) , m_cGridPosVal.put_Row(1);	dTargetPosY = atof( m_cGridPosVal.get_Text() );
	m_cGridPosVal.put_Col(2) , m_cGridPosVal.put_Row(1);	dTargetPosZ = atof( m_cGridPosVal.get_Text() );

	GetDeviceCtl()->ContactMovePos[CAM_2].dx = dTargetPosX;
	GetDeviceCtl()->ContactMovePos[CAM_2].dy = dTargetPosY;
	GetDeviceCtl()->ContactMovePos[CAM_2].dz = dTargetPosZ;

	GetDeviceCtl()->ContactMovePos[CAM_0].dx = 0.0;
	GetDeviceCtl()->ContactMovePos[CAM_0].dy = 0.0;
	GetDeviceCtl()->ContactMovePos[CAM_0].dz = 0.0;

	GetDeviceCtl()->ContactMovePos[CAM_1].dx = 0.0;
	GetDeviceCtl()->ContactMovePos[CAM_1].dy = 0.0;
	GetDeviceCtl()->ContactMovePos[CAM_1].dz = 0.0;

	GetDeviceCtl()->m_iTargetPos = SET_POS ;

	if( GetDeviceCtl()->MoveStage(SET_POS) == MOVE_OK_END)
	{

	}
	else
	{
		//이동 할 수 없음..
	}
}

void CDlgTeachTabPinCCD::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch( nIDEvent )
	{
	case PIN_CUR_POS:	SetCurposDisplay();
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CDlgTeachTabPinCCD::OnChangeJogSpeed()
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







void CDlgTeachTabPinCCD::OnMouseUpJogPadPogoCam(short Button, short Shift, long X, long Y)
{
	GetDoc()->m_pMotion->MoveStop( AXIS_CCD_X3);
	GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_X3 , false);

	Sleep(10);

	GetDoc()->m_pMotion->MoveStop( AXIS_CCD_Y3);
	GetDeviceCtl()->SetMoveStageSoftLimit(AXIS_CCD_Y3 , false);

}

void CDlgTeachTabPinCCD::OnBnClickedRadioGrapPos()
{
	SetInitDisplayData();
	SetGridGrabPos();
	m_iCurSetPos = GRAB_POS;
}

void CDlgTeachTabPinCCD::OnBnClickedRadioReadyPos()
{
	SetInitDisplayData();
	SetGridReadyPos();
	m_iCurSetPos = READY_POS;
}

void CDlgTeachTabPinCCD::OnBnClickedButtonUp()
{
	GetDoc()->m_pMotion->MoveJog(AXIS_CONT_PIN_Z , 2 , 0.5 , TRUE);
}

void CDlgTeachTabPinCCD::OnBnClickedButtonDown()
{
	GetDoc()->m_pMotion->MoveStop(AXIS_CONT_PIN_Z);

}

void CDlgTeachTabPinCCD::OnPositionMoveFinish()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	m_SliderZaxisJog.put_Position(m_iZaxisJogSpeedCount /2);
	m_bZaxisJogMoveEnable = FALSE ;
}

void CDlgTeachTabPinCCD::OnClickIswitchsliderxZaxisJog()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//m_bZaxisJogMoveEnable = TRUE ;
}

void CDlgTeachTabPinCCD::OnZaxisJogSpeedChange()
{
	if( m_bZaxisJogMoveEnable )
	{
		int iSpeedPos = m_SliderZaxisJog.get_Position();
		BOOL bDir = FALSE ;
		int iVel = 0 ;

		switch( iSpeedPos )
		{
		case 0 :
			{
				bDir = TRUE; 
				iVel = 3;
			}
			break;
		case 1:
			{
				bDir = TRUE; 
				iVel = 2;
			}
			break;
		case 2:
			{
				bDir = TRUE; 
				iVel = 1;
			}
			break;
		case 3:
			{
				GetDoc()->m_pMotion->MoveStop(AXIS_CONT_PIN_Z);
			}
			break;

		case 4:
			{
				bDir = FALSE; 
				iVel = 1;
			}
			break;


		case 5:
			{
				bDir = FALSE; 
				iVel = 2;
			}
			break;
		case 6:
			{
				bDir = FALSE; 
				iVel = 3;
			}
			break;
		}
		TRACE( "Dir = %d , pos = %d , Speed = %d \n" , bDir , iSpeedPos , iVel);

		GetDoc()->m_pMotion->MoveJog(AXIS_CONT_PIN_Z , 2 , iVel*0.5 , bDir);
	
	}
}

void CDlgTeachTabPinCCD::OnPositionChangeUserIswitchsliderxZaxisJog()
{
	//m_bZaxisJogMoveEnable = TRUE ;
}

void CDlgTeachTabPinCCD::OnMouseDownIswitchsliderxZaxisJog(short Button, short Shift, long X, long Y)
{
	m_bZaxisJogMoveEnable = TRUE ;
}
