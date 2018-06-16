// frmAlign.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "frmAlign.h"
#include "DlgEditDontCare.h"


// CFormAlign

IMPLEMENT_DYNCREATE(CFormAlign, CFormView)

CFormAlign::CFormAlign()
: CFormView(CFormAlign::IDD)
, m_nFFC_edgeLevel_Min(0)
, m_nFFC_edgeLevel_Max(0)
, m_nFFC_markLevel_Min(0)
, m_nFFC_markLevel_Max(0)
, m_nPCB_edgeLevel_Min(0)
, m_nPCB_edgeLevel_Max(0)
, m_nPCB_markLevel_Min(0)
, m_nPCB_markLevel_Max(0)
, m_nLevelStep(0)
, m_bCheck_AutoTune(FALSE)
{
	m_iMoveSpeed = 0 ;
	m_iMoveTarget = 0 ;

	m_iSelCamera = CAM_0;
	m_iSelPattern = CAM_0_FPC_0;
	m_iCurPatternPos = -1;
	m_bPatReg = FALSE;

	m_iMoveAction = actNONE;
	m_bMouseMoveEnable = FALSE ;
	m_bLive = FALSE;
	m_bPosMoving = FALSE;
}

CFormAlign::~CFormAlign()
{
}

void CFormAlign::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ALIGN_CAM, m_pctPatternGrap[STATIC_ALIGN_GRAP_VIEW]);
	DDX_Control(pDX, IDC_STATIC_PATTERN_CAM, m_pctPatternGrap[STATIC_PATTERN_VIEW]);
	DDX_Control(pDX, IDC_ISWITCHQUADX_MOVE_POS, m_JogPadPosition);
	DDX_Control(pDX, IDC_ISWITCHQUADX_RECT_SIZE, m_JogPadRectSize);
	for( int i=0 ; i < ALIGN_SWITCH_MAX ; i++ )
		DDX_Control(pDX, IDC_ISWITCHLEDX_TOP_LAMP + i, m_SwitchCtrl[i] );
	DDX_Control(pDX, IDC_BTN_PTR_SEARCH, m_btnAlign[ALIGN_BTN_PTR_SEARCH]);
	DDX_Control(pDX, IDC_BTN_EDIT_DONT_CARE, m_btnAlign[ALIGN_BTN_PTR_EDIT_DONTCARE]);
	DDX_Control(pDX, IDC_BTN_CLAMP_ON, m_btnAlign[ALIGN_BTN_CLAMP_ON]);
	DDX_Control(pDX, IDC_BTN_CLAMP_OFF, m_btnAlign[ALIGN_BTN_CLAMP_OFF]);
	DDX_Control(pDX, IDC_BTN_MOVE_READY_POS, m_btnAlign[ALIGN_BTN_MOVE_READY]);
	DDX_Control(pDX, IDC_BTN_MOVE_GRAP_POS, m_btnAlign[ALIGN_BTN_MOVE_GRAB]);
	DDX_Control(pDX, IDC_BTN_ALIGN, m_btnAlign[ALIGN_BTN_ALIGN]);
	DDX_Control(pDX, IDC_BTN_BLOW_ON, m_btnAlign[ALIGN_BTN_BLOW_ON]);
	DDX_Control(pDX, IDC_BTN_BLOW_OFF, m_btnAlign[ALIGN_BTN_BLOW_OFF]);
	DDX_Control(pDX, IDC_BTN_GRAB_LIVE_START, m_btnAlign[ALIGN_BTN_GRAB_START]);
	DDX_Control(pDX, IDC_BTN_GRAB_LIVE_END, m_btnAlign[ALIGN_BTN_GRAB_END]);
	DDX_Control(pDX, IDC_BTN_PTR_SAVE, m_btnAlign[ALIGN_BTN_PTR_SAVE]);
	DDX_Control(pDX, IDC_BTN_REMOVE_PTN, m_btnAlign[ALIGN_BTN_REMOVE_PTN]);
	DDX_Control(pDX, IDC_MSFLEXGRID_ALIGN_RESULT, m_cGridAlignRet);
	DDX_Control(pDX, IDC_ISWITCHLEDX_SPEED, m_SwitchCtrlSpeed);
	DDX_Control(pDX, IDC_ISWITCHLEDX_TARGET, m_SwitchCtrlTarget);
	DDX_Control(pDX, IDC_BTN_LOAD_IMAGE, m_btnLoadImage);
	DDX_Control(pDX, IDC_RADIO_FPC_0, m_btnFpc0);
	DDX_Control(pDX, IDC_RADIO_FPC_1, m_btnFpc1);
	DDX_Control(pDX, IDC_RADIO_FPC_2, m_btnFpc2);
	DDX_Control(pDX, IDC_RADIO_PCB, m_btnPcb);
	DDX_Control(pDX, IDC_RADIO_CAM0, m_radCam0);
	DDX_Control(pDX, IDC_RADIO_CAM1, m_radCam1);
	DDX_Control(pDX, IDC_RADIO_CAM2, m_radCam2);
	DDX_Control(pDX, IDC_RADIO_CAM3, m_radCam3);
	DDX_Control(pDX, IDC_LABEL_MODEL_NAME, m_lblModel);
	DDX_Control(pDX, IDC_BTN_UNCONTACT_ALL, m_btnUncontactAll);
	DDX_Control(pDX, IDC_EDIT_CONDITION, m_EditCondition);
	DDX_Control(pDX, IDC_ISLIDERX1, m_slxCondition);
	DDX_Text(pDX, IDC_EDIT_FFC_EDGE_MIN, m_nFFC_edgeLevel_Min);
	DDV_MinMaxInt(pDX, m_nFFC_edgeLevel_Min, 0, 254);
	DDX_Text(pDX, IDC_EDIT_FFC_EDGE_MAX, m_nFFC_edgeLevel_Max);
	DDV_MinMaxInt(pDX, m_nFFC_edgeLevel_Max, 0, 254);
	DDX_Text(pDX, IDC_EDIT_FFC_MARK_MIN, m_nFFC_markLevel_Min);
	DDV_MinMaxInt(pDX, m_nFFC_markLevel_Min, 0, 254);
	DDX_Text(pDX, IDC_EDIT_FFC_MARK_MAX, m_nFFC_markLevel_Max);
	DDV_MinMaxLong(pDX, m_nFFC_markLevel_Max, 0, 254);
	DDX_Text(pDX, IDC_EDIT_PCB_EDGE_MIN, m_nPCB_edgeLevel_Min);
	DDV_MinMaxInt(pDX, m_nPCB_edgeLevel_Min, 0, 254);
	DDX_Text(pDX, IDC_EDIT_PCB_EDGE_MAX, m_nPCB_edgeLevel_Max);
	DDV_MinMaxInt(pDX, m_nPCB_edgeLevel_Max, 0, 254);
	DDX_Text(pDX, IDC_EDIT_PCB_MARK_MIN, m_nPCB_markLevel_Min);
	DDV_MinMaxInt(pDX, m_nPCB_markLevel_Min, 0, 254);
	DDX_Text(pDX, IDC_EDIT_PCB_MARK_MAX, m_nPCB_markLevel_Max);
	DDV_MinMaxInt(pDX, m_nPCB_markLevel_Max, 0, 254);
	DDX_Text(pDX, IDC_EDIT_LEVEL_STEP, m_nLevelStep);
	DDV_MinMaxInt(pDX, m_nLevelStep, 1, 20);
	DDX_Check(pDX, IDC_CHECK_AUTOTUNE, m_bCheck_AutoTune);
}

BEGIN_MESSAGE_MAP(CFormAlign, CFormView)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CMD_ALIGN_UPSIDE_LAMP, &CFormAlign::OnAlignUpsideLamp)
	ON_BN_CLICKED(IDC_BTN_PTR_SEARCH, &CFormAlign::OnPatternSearch)
	ON_BN_CLICKED(IDC_BTN_PTR_SAVE, &CFormAlign::OnPatternSave)
	ON_BN_CLICKED(IDC_RADIO_CAM0, &CFormAlign::OnSeleteCam0)
	ON_BN_CLICKED(IDC_RADIO_CAM1, &CFormAlign::OnSeleteCam1)
	ON_BN_CLICKED(IDC_RADIO_CAM2, &CFormAlign::OnSeleteCam2)
	ON_BN_CLICKED(IDC_RADIO_CAM3, &CFormAlign::OnSeleteCam3)
	ON_BN_CLICKED(IDC_RADIO_FPC_0, &CFormAlign::OnSeleteFPC_0)
	ON_BN_CLICKED(IDC_RADIO_PCB, &CFormAlign::OnSeletePCB)
	ON_BN_CLICKED(IDC_BTN_GRAB_LIVE_START, &CFormAlign::OnCCDImageGrabLiveStart)
	ON_BN_CLICKED(IDC_BTN_GRAB_LIVE_END, &CFormAlign::OnCCDImageGrabLiveEnd)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BTN_MOVE_READY_POS, &CFormAlign::OnMoveToReadyPos)
	ON_BN_CLICKED(IDC_BTN_MOVE_GRAP_POS, &CFormAlign::OnMoveToGrabPos)
	ON_BN_CLICKED(IDC_BTN_ALIGN, &CFormAlign::OnAlignAction)
	ON_BN_CLICKED(IDC_BTN_CENTER, &CFormAlign::OnDrawRectCenter)
	ON_BN_CLICKED(IDC_BTN_BLOW_ON, &CFormAlign::OnBlowOn)
	ON_BN_CLICKED(IDC_BTN_BLOW_OFF, &CFormAlign::OnBlowOff)
	ON_BN_CLICKED(IDC_BTN_CLAMP_ON, &CFormAlign::OnBnClickedBtnClampOn)
	ON_BN_CLICKED(IDC_BTN_CLAMP_OFF, &CFormAlign::OnBnClickedBtnClampOff)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_FPC_1, &CFormAlign::OnBnClickedRadioFpc1)
	ON_BN_CLICKED(IDC_RADIO_FPC_2, &CFormAlign::OnBnClickedRadioFpc2)
	ON_BN_CLICKED(IDC_BTN_LOAD_IMAGE, &CFormAlign::OnBnClickedBtnLoadImage)
	ON_BN_CLICKED(IDC_BTN_REMOVE_PTN, &CFormAlign::OnBnClickedBtnRemovePtn)
	ON_BN_CLICKED(IDC_BTN_EDIT_DONT_CARE, &CFormAlign::OnBnClickedBtnEditDontCare)
	ON_BN_CLICKED(IDC_BTN_UNCONTACT_ALL, &CFormAlign::OnBnClickedBtnUncontactAll)
	ON_EN_CHANGE(IDC_EDIT_CONDITION, &CFormAlign::OnEnChangeEditCondition)
	ON_BN_CLICKED(IDC_BUTTON_LEVEL_SET, &CFormAlign::OnBnClickedButtonLevelSet)
	ON_BN_CLICKED(IDC_BUTTON_RESET_PCBX, &CFormAlign::OnBnClickedButtonResetPcbx)
END_MESSAGE_MAP()


// CFormAlign diagnostics

#ifdef _DEBUG
void CFormAlign::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormAlign::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormAlign message handlers

BOOL CFormAlign::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CFormAlign::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	ASSERT(GetAlignCtl()->GetVision()->SelAlignWindow(m_pctPatternGrap[STATIC_ALIGN_GRAP_VIEW].m_hWnd, m_pctPatternGrap[STATIC_PATTERN_VIEW]));
	m_dJogXOldPos = 0.0 ;
	m_dJogYOldPos = 0.0 ;
	m_dJogSizeXOldPos = 0.0 ;
	m_dJogSizeYOldPos = 0.0 ;

	m_iMoveSpeed = 1 ;
	m_iMoveTarget = MOVE_RECT;

	GetDlgItem(IDC_STATIC_ALIGN_CAM)->GetWindowRect(&m_StaticAlignRect);
	ScreenToClient(&m_StaticAlignRect);
	CheckRadioButton(IDC_RADIO_FPC_0, IDC_RADIO_PCB, IDC_RADIO_FPC_0);

	m_nFFC_edgeLevel_Min = GetDoc()->m_nFFCEdge_Min;
	m_nFFC_edgeLevel_Max = GetDoc()->m_nFFCEdge_Max;
	m_nFFC_markLevel_Min = GetDoc()->m_nFFCMark_Min;
	m_nFFC_markLevel_Max = GetDoc()->m_nFFCMark_Max;
	m_nPCB_edgeLevel_Min = GetDoc()->m_nPCBEdge_Min;
	m_nPCB_edgeLevel_Max = GetDoc()->m_nPCBEdge_Max;
	m_nPCB_markLevel_Min = GetDoc()->m_nPCBMark_Min;
	m_nPCB_markLevel_Max = GetDoc()->m_nPCBMark_Max;
	m_nLevelStep = GetDoc()->m_nLevelStep;
	m_bCheck_AutoTune = GetDoc()->m_bAutoTune_Using;

	UpdateData(FALSE);

}
BEGIN_EVENTSINK_MAP(CFormAlign, CFormView)
	ON_EVENT(CFormAlign, IDC_ISWITCHQUADX_MOVE_POS, 5, CFormAlign::OnJogMoveXPosition, VTS_NONE)
	ON_EVENT(CFormAlign, IDC_ISWITCHQUADX_MOVE_POS, 6, CFormAlign::OnJogMoveYPosition, VTS_NONE)
	ON_EVENT(CFormAlign, IDC_ISWITCHQUADX_RECT_SIZE, 5, CFormAlign::OnJogXRectSize, VTS_NONE)
	ON_EVENT(CFormAlign, IDC_ISWITCHQUADX_RECT_SIZE, 6, CFormAlign::OnJogYRectSize, VTS_NONE)
	ON_EVENT(CFormAlign, IDC_ISWITCHLEDX_TOP_LAMP, 1, CFormAlign::OnTopLampOnOff, VTS_NONE)
	ON_EVENT(CFormAlign, IDC_ISWITCHLEDX_BOTTOM_LAMP, 1, CFormAlign::OnBottomLampOnOff, VTS_NONE)
	ON_EVENT(CFormAlign, IDC_ISWITCHQUADX_MOVE_POS, 12, CFormAlign::OnMouseDownMovePos, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CFormAlign, IDC_ISWITCHQUADX_MOVE_POS, 14, CFormAlign::OnMouseUpMovePos, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CFormAlign, IDC_ISWITCHQUADX_RECT_SIZE, 12, CFormAlign::OnMouseDownRectSize, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CFormAlign, IDC_ISWITCHQUADX_RECT_SIZE, 14, CFormAlign::OnMouseUpRectSize, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CFormAlign, IDC_ISWITCHLEDX_SPEED, 1, CFormAlign::OnSetChangeSpeed, VTS_NONE)
	ON_EVENT(CFormAlign, IDC_ISWITCHLEDX_TARGET, 1, CFormAlign::OnSetChangeTarget, VTS_NONE)
	ON_EVENT(CFormAlign, IDC_ISLIDERX1, 4, CFormAlign::OnPositionChangeIsliderx1, VTS_NONE)
END_EVENTSINK_MAP()

void CFormAlign::OnJogMoveXPosition()
{
	CRect* pRect = &GetAlignCtl()->m_rtPattern;
	CPoint* pPont = &GetAlignCtl()->m_ptCenter;
	if(  m_dJogXOldPos > m_JogPadPosition.get_ValueX()  )		// + , - 방향을 설정 하기 위함...
	{
		// - 방향..
		m_iJogMode = MOVE_X_DOWN;
		switch( m_iMoveTarget )
		{
		case MOVE_RECT:
			if(pRect->left - m_iMoveSpeed >= 0)
			{
				pRect->OffsetRect(-m_iMoveSpeed, 0);
				pPont->Offset(-m_iMoveSpeed, 0);
			}
			break;
		case MOVE_CENTER:
			pPont->Offset(-m_iMoveSpeed, 0);
			break;
		default:
			break;
		}
	}
	else
	{
		// + 방향..
		m_iJogMode = MOVE_X_UP;
		switch( m_iMoveTarget )
		{
		case MOVE_RECT:
			if(pRect->right + m_iMoveSpeed <= IMAGE_SIZE_X - 1)
			{
				pRect->OffsetRect(m_iMoveSpeed, 0);
				pPont->Offset(m_iMoveSpeed, 0);
			}
			break;
		case MOVE_CENTER:
			pPont->Offset(m_iMoveSpeed, 0);
			break;
		default:
			break;
		}
	}

	GetAlignCtl()->DrawLine(DISP_ALIGN);	
	m_dJogXOldPos = m_JogPadPosition.get_ValueX();
}

void CFormAlign::OnJogMoveYPosition()
{
	CRect* pRect = &GetAlignCtl()->m_rtPattern;
	CPoint* pPont = &GetAlignCtl()->m_ptCenter;

	if(  m_dJogYOldPos > m_JogPadPosition.get_ValueY()  )		// + , - 방향을 설정 하기 위함...
	{
		// - 방향..
		m_iJogMode = MOVE_Y_DOWN;
		switch( m_iMoveTarget )
		{
		case MOVE_RECT:
			if(pRect->bottom + m_iMoveSpeed <= IMAGE_SIZE_Y - 1)
			{
				pRect->OffsetRect(0, m_iMoveSpeed);
				pPont->Offset(0, m_iMoveSpeed);
			}
			break;
		case MOVE_CENTER:
			pPont->Offset(0, m_iMoveSpeed);
			break;
		default:
			break;
		}
	}
	else
	{
		// + 방향..

		m_iJogMode = MOVE_Y_UP;
		switch( m_iMoveTarget )
		{
		case MOVE_RECT:
			if(pRect->top - m_iMoveSpeed >= 0)
			{
				pRect->OffsetRect(0, -m_iMoveSpeed);
				pPont->Offset(0, -m_iMoveSpeed);
			}
			break;
		case MOVE_CENTER:
			pPont->Offset(0, -m_iMoveSpeed);
			break;
		default:
			break;
		}
	}

	GetAlignCtl()->DrawLine(DISP_ALIGN);	
	m_dJogYOldPos = m_JogPadPosition.get_ValueY();
}

void CFormAlign::OnJogXRectSize()
{
	CRect* pRect = &GetAlignCtl()->m_rtPattern;
	CPoint* pPont = &GetAlignCtl()->m_ptCenter;

	if(  m_dJogSizeXOldPos > m_JogPadRectSize.get_ValueX()  )		// + , - 방향을 설정 하기 위함...
	{
		// -방향

		m_iJogMode = SIZE_X_DEFLATE;

		if(pRect->Width() - m_iMoveSpeed > MIN_RECT_SIZE)
			pRect->DeflateRect(m_iMoveSpeed, 0);
	}
	else
	{
		// +방향
		m_iJogMode = SIZE_X_INFLATE;

		if(pRect->Width() + m_iMoveSpeed < MAX_RECT_SIZE)
			pRect->InflateRect(m_iMoveSpeed, 0);
	}

	m_dJogSizeXOldPos = m_JogPadRectSize.get_ValueX();
	GetAlignCtl()->DrawLine(DISP_ALIGN);	
}

void CFormAlign::OnJogYRectSize()
{
	CRect* pRect = &GetAlignCtl()->m_rtPattern;
	CPoint* pPont = &GetAlignCtl()->m_ptCenter;

	if(  m_dJogSizeYOldPos > m_JogPadRectSize.get_ValueY()  )		// + , - 방향을 설정 하기 위함...
	{
		m_iJogMode = SIZE_Y_DEFLATE;
		//-방향
		if(pRect->Height() - m_iMoveSpeed > MIN_RECT_SIZE)
			pRect->DeflateRect(0, m_iMoveSpeed);
	}
	else
	{
		m_iJogMode = SIZE_Y_INFLATE;
		//+방향
		if(pRect->Height() + m_iMoveSpeed < MAX_RECT_SIZE)
			pRect->InflateRect(0, m_iMoveSpeed);
	}

	m_dJogSizeYOldPos = m_JogPadRectSize.get_ValueY();
	GetAlignCtl()->DrawLine(DISP_ALIGN);
}

void CFormAlign::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if( bShow )
	{
		m_SwitchCtrl[ALIGN_SWITCH_TOP_LAMP].put_Active(FALSE);
		m_SwitchCtrl[ALIGN_SWITCH_BOTT_LAMP].put_Active(FALSE);

		m_iMoveSpeed = 1 ;
		m_iMoveTarget = MOVE_RECT;
		m_iMoveAction = actNONE;

		m_dJogXOldPos = 0.0 ;
		m_dJogYOldPos = 0.0 ;
		m_dJogSizeXOldPos = 0.0 ;
		m_dJogSizeYOldPos = 0.0 ;

		m_JogPadPosition.put_ValueX(0);
		m_JogPadPosition.put_ValueY(0);

		m_JogPadRectSize.put_ValueX(0);
		m_JogPadRectSize.put_ValueY(0);

		m_iMoveSpeed = 1 ;
		m_iMoveTarget = MOVE_RECT;
		m_SwitchCtrlSpeed.put_Active(FALSE);
		m_SwitchCtrlTarget.put_Active(FALSE);

		SetCamSelectButton();
		SetPatternSelectButton();
		GetAlignCtl()->LoadPatPreview(m_iSelPattern);
		SetGridStructure();
		SetSuperUserControl();
		m_slxCondition.put_Position(GetDoc()->m_nBinarizeFPC[m_iSelCamera]);
		m_lblModel.SetWindowText(GetDoc()->m_pAmtData->m_CurModelInfo.strModelName);
		if(m_bLive)
			SeImageLiveView();

		SetTimer(ALIGN_MOVE , 100 , NULL);

		if(GetApp()->IsRunStatus())	this->EnableWindow(FALSE);
		else						this->EnableWindow(TRUE);
	}
	else
	{
		//여기에서.. 동영상 Live 정지.. Top , Bottom Lamp OFF 하도록 추가...
		// 
		GetDoc()->GrapAliveFlagAllStop();
		KillTimer(ALIGN_MOVE);
	}
}




void CFormAlign::OnAlignUpsideLamp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CFormAlign::OnTopLampOnOff()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CFormAlign::OnBottomLampOnOff()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}



void CFormAlign::OnPatternSearch()
{
	// FPC 패턴일 경우 0번부터 찾기 시작
	int nModelIndex;
	/*if((m_iSelPattern % MAX_CAM_NUM) != CAM_0_PCB)
		nModelIndex = m_iSelCamera * MAX_CAM_NUM;
	else*/
	nModelIndex = m_iSelPattern;
	
	GetAlignCtl()->ManualSearchPattern( nModelIndex );
	SetDisplayAlignData();
	CString strFilePath;
	strFilePath.Format("%s", IMAGE_PATH);
	CreateDirectory(strFilePath, NULL);
	strFilePath += "\\ManualSearch.jpg";
	CEtc::CaptureScreen(m_pctPatternGrap[STATIC_ALIGN_GRAP_VIEW].m_hWnd, strFilePath);
}

void CFormAlign::SetDisplayAlignData()
{
	CString strTmpData = _T("");
	strTmpData.Format("%.3f" , GetAlignCtl()->m_ManualAlignResult.FPCPos.x );
	m_cGridAlignRet.put_TextMatrix( 1 , 1 ,strTmpData );

	strTmpData.Format("%.3f" , GetAlignCtl()->m_ManualAlignResult.FPCPos.y );
	m_cGridAlignRet.put_TextMatrix( 2 , 1 ,strTmpData );

	strTmpData.Format("%.3f" , GetAlignCtl()->m_ManualAlignResult.PCBPos.x );
	m_cGridAlignRet.put_TextMatrix( 3 , 1 ,strTmpData );

	strTmpData.Format("%.3f" , GetAlignCtl()->m_ManualAlignResult.PCBPos.y );
	m_cGridAlignRet.put_TextMatrix( 4 , 1 ,strTmpData );

}

CPoint CFormAlign::CalcCenter(int iCenX, int iCenY, int iWidth, int iHeight)
{
	if(iWidth > iHeight) {
		if(iWidth > 220) {
			iCenX *=(int)((double)iWidth/(double)220);
			iCenY *=(int)((double)iWidth/(double)220);
		}
		else {
			iCenX *=(int)((double)200/(double)iWidth);
			iCenY *=(int)((double)200/(double)iWidth);
		}
	}
	else {
		if(iHeight > 200) {
			iCenX *=(int)((double)iHeight/(double)200);
			iCenY *=(int)((double)iHeight/(double)200);
		}
		else {
			iCenX *=(int)((double)200/(double)iHeight);
			iCenY *=(int)((double)200/(double)iHeight);
		}
	}
	return CPoint();
}

void CFormAlign::OnPatternSave()
{
	if(GetDoc()->m_bSuperLogin == TRUE)
	{
		GetAlignCtl()->SavePattern(m_iSelPattern, (long)m_slxCondition.get_Position());
		SetPatternSelectButton();
		GetAlignCtl()->LoadPatPreview(m_iSelPattern);
	}
	else
		AfxMessageBox("Super-User more than is available");
}

void CFormAlign::SetGridStructure()
{
	m_cGridAlignRet.put_Cols(2);
	m_cGridAlignRet.put_Rows(5);
	m_cGridAlignRet.put_ColWidth(0 , 1700);
	m_cGridAlignRet.put_ColWidth(1 , 1550);

	for( int i =0 ; i < 5 ; i ++ )
	{
		for( int j=0; j < 2 ; j ++ )
		{
			m_cGridAlignRet.put_Row(i) ;
			m_cGridAlignRet.put_Col(j) ; 
			m_cGridAlignRet.put_CellAlignment(4);
		}
		m_cGridAlignRet.put_RowHeight( i , 450);
	}

	m_cGridAlignRet.put_TextMatrix( 0 , 0 , " ");
	m_cGridAlignRet.put_TextMatrix( 0 , 1 , "Align 결과");

	m_cGridAlignRet.put_TextMatrix( 1 , 0 , "FPC-X");
	m_cGridAlignRet.put_TextMatrix( 2 , 0 , "FPC-Y");
	m_cGridAlignRet.put_TextMatrix( 3 , 0 , "PCB-X");
	m_cGridAlignRet.put_TextMatrix( 4 , 0 , "PCB-Y");

	SetDisplayAlignData();
}

void CFormAlign::SetCamSelectButton(void)
{
	GetDlgItem(IDC_RADIO_CAM0)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CAM1)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CAM2)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CAM3)->EnableWindow(TRUE);

	if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::ONE_CONT )
	{
		if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_0 )
		{
			//GetDlgItem(IDC_RADIO_CAM0)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_CAM1)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_CAM2)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_CAM3)->EnableWindow(FALSE);
			CheckRadioButton(IDC_RADIO_CAM0, IDC_RADIO_CAM3, IDC_RADIO_CAM0);
			m_iSelCamera = CAM_0;
		}
		else if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_1 )
		{
			GetDlgItem(IDC_RADIO_CAM0)->EnableWindow(FALSE);
			//GetDlgItem(IDC_RADIO_CAM1)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_CAM2)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_CAM3)->EnableWindow(FALSE);
			CheckRadioButton(IDC_RADIO_CAM0, IDC_RADIO_CAM3, IDC_RADIO_CAM1);
			m_iSelCamera = CAM_1;
		}
		else if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_2 )
		{
			GetDlgItem(IDC_RADIO_CAM0)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_CAM1)->EnableWindow(FALSE);
			//GetDlgItem(IDC_RADIO_CAM2)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_CAM3)->EnableWindow(FALSE);
			CheckRadioButton(IDC_RADIO_CAM0, IDC_RADIO_CAM3, IDC_RADIO_CAM2);
			m_iSelCamera = CAM_2;
		}
		else
		{
			GetDlgItem(IDC_RADIO_CAM0)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_CAM1)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_CAM2)->EnableWindow(FALSE);
			//GetDlgItem(IDC_RADIO_CAM3)->EnableWindow(FALSE);
			CheckRadioButton(IDC_RADIO_CAM0, IDC_RADIO_CAM3, IDC_RADIO_CAM3);
			m_iSelCamera = CAM_3;
		}
	}
	else if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::TWO_CONT )
	{
		GetDlgItem(IDC_RADIO_CAM0)->EnableWindow(FALSE);
		//GetDlgItem(IDC_RADIO_CAM1)->EnableWindow(FALSE);
		//GetDlgItem(IDC_RADIO_CAM2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CAM3)->EnableWindow(FALSE);
		CheckRadioButton(IDC_RADIO_CAM0, IDC_RADIO_CAM3, IDC_RADIO_CAM1);
		m_iSelCamera = CAM_1;
	}
	else if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::FOUR_CONT )
	{
		CheckRadioButton(IDC_RADIO_CAM0, IDC_RADIO_CAM3, IDC_RADIO_CAM0);
		m_iSelCamera = CAM_0;
	}
	if(		(m_iSelPattern < (m_iSelCamera*MAX_CAM_NUM))	||		( m_iSelPattern > ((m_iSelCamera*MAX_CAM_NUM) + CAM_0_PCB_0) )		)	
		m_iSelPattern = m_iSelCamera*MAX_CAM_NUM;
}

void CFormAlign::OnSeleteCam0()
{
	m_iSelCamera = CAM_0;

	SeImageLiveView();
	SetPatternSelectButton();
	OnSeleteFPC_0();
}

void CFormAlign::OnSeleteCam1()
{
	m_iSelCamera = CAM_1;

	SeImageLiveView();
	SetPatternSelectButton();
	OnSeleteFPC_0();
}

void CFormAlign::OnSeleteCam2()
{
	m_iSelCamera = CAM_2;

	SeImageLiveView();
	SetPatternSelectButton();
	OnSeleteFPC_0();
}

void CFormAlign::OnSeleteCam3()
{
	m_iSelCamera = CAM_3;

	SeImageLiveView();
	SetPatternSelectButton();
	OnSeleteFPC_0();
}

void CFormAlign::OnSeleteFPC_0()
{
	m_iSelPattern = m_iSelCamera * 4;
	GetAlignCtl()->LoadPatPreview(m_iSelPattern);
	GetAlignCtl()->MoveRectToDefault(m_iSelPattern);
	m_slxCondition.put_Position(GetDoc()->m_nBinarizeFPC[m_iSelCamera]);
}

void CFormAlign::OnBnClickedRadioFpc1()
{
	m_iSelPattern = (m_iSelCamera * 4) + 1;
	GetAlignCtl()->LoadPatPreview(m_iSelPattern);
	GetAlignCtl()->MoveRectToDefault(m_iSelPattern);
	m_slxCondition.put_Position(GetDoc()->m_nBinarizeFPC_Mark[m_iSelCamera]);
}

void CFormAlign::OnBnClickedRadioFpc2()
{
	m_iSelPattern = (m_iSelCamera * 4) + 2;
	GetAlignCtl()->LoadPatPreview(m_iSelPattern);
	GetAlignCtl()->MoveRectToDefault(m_iSelPattern);
	m_slxCondition.put_Position(GetDoc()->m_nBinarizePCB[m_iSelCamera]);
}

void CFormAlign::OnSeletePCB()
{
	m_iSelPattern = (m_iSelCamera * 4) + 3;
	GetAlignCtl()->LoadPatPreview(m_iSelPattern);
	GetAlignCtl()->MoveRectToDefault(m_iSelPattern);
	m_slxCondition.put_Position(GetDoc()->m_nBinarizePCB_Mark[m_iSelCamera]);
}

void CFormAlign::OnCCDImageGrabLiveStart()
{
	GetDoc()->GrapAliveFlagAllStop();
	GetAlignCtl()->OverlayClear(DISP_ALIGN);

	GetDoc()->m_bLiveGrapFlag[m_iSelCamera] = TRUE;
	m_bLive = TRUE;
}

void CFormAlign::OnCCDImageGrabLiveEnd()
{
	GetDoc()->GrapAliveFlagAllStop();
	m_bLive = FALSE;
	if(GetDoc()->m_bUseBinary)
	{
		Sleep(100);
		GetAlignCtl()->GetVision()->Grab(DISP_ALIGN, m_iSelCamera, STATUS_STOP, TRUE);
		if((m_iSelPattern % MAX_CAM_NUM) != CAM_0_PCB_0)
			GetAlignCtl()->GrabBinarize(DISP_ALIGN, (long)m_slxCondition.get_Position());
	}
}

void CFormAlign::OnMouseDownMovePos(short Button, short Shift, long X, long Y)
{
	SetTimer(DRAW_PATTERN , 100 , NULL);
}

void CFormAlign::OnMouseUpMovePos(short Button, short Shift, long X, long Y)
{
	m_iJogMode = 8 ;
	KillTimer(DRAW_PATTERN);
}

void CFormAlign::OnTimer(UINT_PTR nIDEvent)
{
	switch( nIDEvent )
	{
	case DRAW_PATTERN :	SetPatternDrawLine();
		break;
	case ALIGN_MOVE:
		{
			if( m_iMoveAction != actNONE )	
				GetDeviceCtl()->ManualSeqContactUnitMove(m_iMoveAction, m_bPosMoving);
		}
		break;

	default:
		break;
	}

	CFormView::OnTimer(nIDEvent);
}

void CFormAlign::SetPatternDrawLine(void)
{
	CRect* pRect = &GetAlignCtl()->m_rtPattern;
	CPoint* pPont = &GetAlignCtl()->m_ptCenter;
	BOOL bMovable = TRUE;

	switch( m_iJogMode )
	{
	case MOVE_X_UP:
		switch( m_iMoveTarget )
		{
		case MOVE_RECT:
			if(pRect->right + m_iMoveSpeed <= IMAGE_SIZE_X - 1)
			{
				pRect->OffsetRect(m_iMoveSpeed, 0);
				pPont->Offset(m_iMoveSpeed, 0);
			}
			break;
		case MOVE_CENTER:
			pPont->Offset(m_iMoveSpeed, 0);
			break;
		default:
			break;
		}
		break;

	case MOVE_X_DOWN:
		switch( m_iMoveTarget )
		{
		case MOVE_RECT:
			if(pRect->left - m_iMoveSpeed >= 0)
			{
				pRect->OffsetRect(-m_iMoveSpeed, 0);
				pPont->Offset(-m_iMoveSpeed, 0);
			}
			break;
		case MOVE_CENTER:
			pPont->Offset(-m_iMoveSpeed, 0);
			break;
		default:
			break;
		}
		break;

	case MOVE_Y_UP:
		switch( m_iMoveTarget )
		{
		case MOVE_RECT:
			if(pRect->top - m_iMoveSpeed >= 0)
			{
				pRect->OffsetRect(0, -m_iMoveSpeed);
				pPont->Offset(0, -m_iMoveSpeed);
			}
			break;
		case MOVE_CENTER:
			pPont->Offset(0, -m_iMoveSpeed);
			break;
		default:
			break;
		}
		break;
	case MOVE_Y_DOWN:
		switch( m_iMoveTarget )
		{
		case MOVE_RECT:
			if(pRect->bottom + m_iMoveSpeed <= IMAGE_SIZE_Y - 1)
			{
				pRect->OffsetRect(0, m_iMoveSpeed);
				pPont->Offset(0, m_iMoveSpeed);
			}
			break;
		case MOVE_CENTER:
			pPont->Offset(0, m_iMoveSpeed);
			break;
		default:
			break;
		}
		break;
	case SIZE_X_INFLATE:
		if(pRect->Width() + m_iMoveSpeed < MAX_RECT_SIZE)
			pRect->InflateRect(m_iMoveSpeed, 0);
		break;

	case SIZE_X_DEFLATE:
		if(pRect->Width() - m_iMoveSpeed > MIN_RECT_SIZE)
			pRect->DeflateRect(m_iMoveSpeed, 0);
		break;

	case SIZE_Y_INFLATE:
		if(pRect->Height() + m_iMoveSpeed < MAX_RECT_SIZE)
			pRect->InflateRect(0, m_iMoveSpeed);
		break;
	case SIZE_Y_DEFLATE:
		if(pRect->Height() - m_iMoveSpeed > MIN_RECT_SIZE)
			pRect->DeflateRect(0, m_iMoveSpeed);
		break;
	default:
		bMovable = FALSE;
		break;
	}
	if(bMovable)	GetAlignCtl()->DrawLine(DISP_ALIGN);
}

void CFormAlign::OnMouseDownRectSize(short Button, short Shift, long X, long Y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SetTimer(DRAW_PATTERN , 100 , NULL);
}

void CFormAlign::OnMouseUpRectSize(short Button, short Shift, long X, long Y)
{
	m_iJogMode = 8 ;
	KillTimer(DRAW_PATTERN);
}

void CFormAlign::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bMouseMoveEnable = TRUE;

	if( m_bPatReg )
	{
		CPoint ptCenter = GetAlignCtl()->m_rtPattern.CenterPoint();
		CSize offsetSize = point-ptCenter;

		switch( m_iMoveTarget )
		{
		case MOVE_RECT:
			GetAlignCtl()->m_rtPattern.OffsetRect(offsetSize);
			GetAlignCtl()->m_ptCenter.Offset(offsetSize);
			break;
		case MOVE_CENTER:
			GetAlignCtl()->m_ptCenter = point;
			break;
		default:
			break;
		}

		GetAlignCtl()->DrawLine(DISP_ALIGN);
	}

	CFormView::OnLButtonDown(nFlags, point);
}

void CFormAlign::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_bMouseMoveEnable = FALSE;
	CFormView::OnLButtonUp(nFlags, point);
}

void CFormAlign::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_bMouseMoveEnable )
	{
		CRect* pRect = &GetAlignCtl()->m_rtPattern;
		CPoint* pPont = &GetAlignCtl()->m_ptCenter;

		if(m_StaticAlignRect.PtInRect(point))
		{
			CPoint ptCenter = pRect->CenterPoint();
			CSize offsetSize = point-ptCenter;

			switch( m_iMoveTarget )
			{
			case MOVE_RECT:
				pRect->OffsetRect(offsetSize);
				pPont->Offset(offsetSize);
				break;
			case MOVE_CENTER:
				*pPont = point;
				break;
			default:
				break;
			}

			GetAlignCtl()->DrawLine(DISP_ALIGN);
		}
	}
	CFormView::OnMouseMove(nFlags, point);
}

void CFormAlign::OnMoveToReadyPos()
{
	if( m_bPosMoving == TRUE && m_iMoveAction != actNONE )
	{
		AfxMessageBox(" 현재 컨텍 UNIT 동작 중입니다" );
		return;
	}

	m_iMoveAction = actREADY_MOVE ;
}

void CFormAlign::OnMoveToGrabPos()
{
	if( m_bPosMoving == TRUE && m_iMoveAction != actNONE )
	{
		AfxMessageBox(" 현재 컨텍 UNIT 동작 중입니다" );
		return;
	}
	m_iMoveAction = actGRAB_MOVE ;
}

void CFormAlign::OnAlignAction()
{
	if( GetDoc()->m_pEqMainSeq->m_SeqFlag.bCellContactComp)
	{
		GetDoc()->m_nYaxis_Back = 99;//  [6/11/2009 janghan]
		
		GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]
		GetDeviceCtl()->ClampUnit(TRUE);
		Sleep(100);
		GetDoc()->m_pEqMainSeq->m_SeqFlag.bCellContactReq = TRUE;
	}
	else
	{
		AfxMessageBox(" 현재 얼라인 동작 중입니다" );
	}
}


void CFormAlign::OnDrawRectCenter()
{
	GetAlignCtl()->ResetRect(m_iSelPattern);
}

void CFormAlign::OnBlowOn()
{
	BLOW_UNIT(SENSOR_ON);
}

void CFormAlign::OnBlowOff()
{
	BLOW_UNIT(SENSOR_OFF);
}

void CFormAlign::OnBnClickedBtnClampOn()
{
	CLAMP_UNIT(SENSOR_ON);
}

void CFormAlign::OnBnClickedBtnClampOff()
{
	CLAMP_UNIT(SENSOR_OFF);
}

void CFormAlign::OnSetChangeSpeed()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	if( m_SwitchCtrlSpeed.get_Active() )
	{
		m_SwitchCtrlSpeed.put_Caption("X2");
		m_iMoveSpeed = 2;
	}
	else
	{
		m_SwitchCtrlSpeed.put_Caption("X1");
		m_iMoveSpeed = 1;
	}
}

void CFormAlign::OnSetChangeTarget()
{
	if( m_SwitchCtrlTarget.get_Active() )
	{
		m_SwitchCtrlTarget.put_Caption("POINT");
		m_iMoveTarget = MOVE_CENTER;
	}
	else
	{
		m_SwitchCtrlTarget.put_Caption("RECT");
		m_iMoveTarget = MOVE_RECT;
	}
}

void CFormAlign::SeImageLiveView(void)
{
	GetDoc()->GrapAliveFlagAllStop();

	if(m_iSelCamera == CAM_0 )	
		GetDoc()->m_bLiveGrapFlag[CAM_0] = TRUE;
	else if( m_iSelCamera == CAM_1)	
		GetDoc()->m_bLiveGrapFlag[CAM_1] = TRUE;
	else if( m_iSelCamera == CAM_2)	
		GetDoc()->m_bLiveGrapFlag[CAM_2] = TRUE;
	else if( m_iSelCamera == CAM_3)	
		GetDoc()->m_bLiveGrapFlag[CAM_3] = TRUE;
}

void CFormAlign::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: Add your message handler code here
	GetAlignCtl()->GetVision()->DeselAlignWindow();
}

void CFormAlign::OnBnClickedBtnLoadImage()
{
	// TODO: Add your control notification handler code here
	TCHAR szFilters[]= _T("JPEG Files (*.jpg)|*.jpg|TIFF Files (*.tif)|*.tif|BMP Files (*.bmp)|*.bmp|All Files (*.*)|*.*||");

	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg(TRUE, _T("Image File"), _T("*.jpg"),
		OFN_READONLY | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 
	// returns IDOK.
	if(fileDlg.DoModal() == IDOK)
	{
		CString pathName = fileDlg.GetPathName();

		GetAlignCtl()->GetVision()->LoadImage(pathName, DISP_ALIGN);
	}
}

void CFormAlign::SetPatternSelectButton( void )
{
	int nFpcNum = GetDoc()->m_pAmtData->m_CurModelInfo.nFPCModelNum[m_iSelCamera];
	switch (nFpcNum)
	{
	case 0:
		m_btnFpc1.EnableWindow(FALSE);
		m_btnFpc2.EnableWindow(FALSE);
		break;
	case 1:
		m_btnFpc1.EnableWindow(TRUE);
		m_btnFpc2.EnableWindow(FALSE);
		break;
	case 2:
		m_btnFpc1.EnableWindow(TRUE);
		m_btnFpc2.EnableWindow(TRUE);
		break;
	case 3:
		m_btnFpc1.EnableWindow(TRUE);
		m_btnFpc2.EnableWindow(TRUE);
		break;
	}
	if( m_iSelPattern != ((m_iSelCamera * 4) + 3) )
	{
// 		if(nFpcNum == 0)
		{
			m_iSelPattern = (m_iSelCamera * 4);
			CheckRadioButton(IDC_RADIO_FPC_0, IDC_RADIO_PCB, IDC_RADIO_FPC_0);
		}
// 		else
// 		{
// 			m_iSelPattern = ( m_iSelPattern > ( (m_iSelCamera * 4) + (nFpcNum - 1) ) ) ? (m_iSelCamera * 4) : m_iSelPattern;
// 			int nPatternIndex = (m_iSelPattern % MAX_CAM_NUM);
// 			CheckRadioButton(IDC_RADIO_FPC_0, IDC_RADIO_PCB, IDC_RADIO_FPC_0 + nPatternIndex);
// 		}
	}
}
void CFormAlign::OnBnClickedBtnRemovePtn()
{
	// Super user만 수정가능
	if(GetDoc()->m_bSuperLogin || GetDoc()->m_bUseBinary == FALSE)
	{
		GetAlignCtl()->RemoveFPCPattern(m_iSelPattern);
		SetPatternSelectButton();
		GetAlignCtl()->LoadPatPreview(m_iSelPattern);
	}
	else
		AfxMessageBox("Can not remove mark");
	
}

void CFormAlign::OnBnClickedBtnEditDontCare() //  [6/9/2009 janghan] - 함수 전체 수정 (AutoTune)
{
	/*if(GetAlignCtl()->GetVision()->IsPatternEmpty(m_iSelPattern) == FALSE)
	{
		CDlgEditDontCare dlg;
		dlg.SetModelIndex(m_iSelPattern);
		dlg.DoModal();

		GetAlignCtl()->LoadPatPreview(m_iSelPattern);
	}
	*/  //기능 사용 안함 - 2009.6.9 by jh

	CString strMessageBuf;
	
	int nModelIndex = 0;
	int nCameraIndex = 0;
	GetDoc()->m_fAutoDistance_X =  0.;
	GetDoc()->m_fPCBmarkFind_X = 0.;
	GetDoc()->m_fMarkFind_X = 0.;

	GetDoc()->m_nYaxis_Back = 99;


	// Super user만 수정가능
	if(GetDoc()->m_bSuperLogin || GetDoc()->m_bUseBinary == FALSE)
	{		
		//Edge Finding
		GetDoc()->m_nLevelCount = 100;//edge는 100이하에서 이상한 곳에서 Maxscore가 나올 가능성이 있어서...

		GetDoc()->m_fMaxScore = 0.f;

		nModelIndex = m_iSelCamera * 4;
		
		AfxMessageBox("Edge Tune");

		GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;//  [7/4/2009 SESL]

		do 
		{
			GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + BINARIZE_LEVEL_STEP;
			Sleep(20);

			GetAlignCtl()->SearchPattern2(nModelIndex,TRUE);

		} while (GetDoc()->m_nLevelCount < BINARIZE_MAX_LEVEL);

		if (GetDoc()->m_fMaxScore > 0.f)
		{
			GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 SESL]

			strMessageBuf.Format("MaxScore: %f, EdgeFind_X: %f", GetDoc()->m_fMaxScore, GetDoc()->m_fEdgeFind_X);
			AfxMessageBox(strMessageBuf);

			GetDoc()->m_nBinarizeFPC[nModelIndex/MAX_CAM_NUM] = GetDoc()->m_nTuneLevl;//  

			GetDoc()->SaveBinarizeCondition();//이진화 레벨 저장

			AfxMessageBox("Edge Level Updated! & Move Center");

			GetDoc()->m_nLevelCount = GetDoc()->m_nTuneLevl;
			//GetAlignCtl()->SearchPattern2( nModelIndex, FALSE );

			nCameraIndex = nModelIndex / MAX_CAM_NUM;

			GetAlignCtl()->ContactPosCalibaration_FFC_Edge(nCameraIndex);//이동 위치 환산	

			GetDoc()->m_nCamera_Auto = nCameraIndex;

			if( m_bPosMoving == TRUE && m_iMoveAction != actNONE )
			{
				AfxMessageBox(" 현재 컨텍 UNIT 동작 중입니다" );
				return;
			}
			m_iMoveAction = actAUTOTUNE_CENTER_MOVE ;

			//GetDeviceCtl()->MoveStage_Auto(GRAB_POS, nCameraIndex);//화면 중앙 이동			

			Sleep(200);

			GetDoc()->SetAlignPos(GetDoc()->m_nCamera_Auto);//현재 좌표 저장

		} 
		else
		{
			GetDoc()->m_nLevelCount = 100;//
			GetDoc()->m_fMaxScore = 0.f;

			AfxMessageBox("Edge Tune Fail.....");

			//PCB Edge Search.........
			
			AfxMessageBox("PCB Edge Search");

			nModelIndex = m_iSelCamera * 4 + 2;

			do 
			{
				GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + BINARIZE_LEVEL_STEP;
				Sleep(20);

				GetAlignCtl()->SearchPattern2(nModelIndex,TRUE);
			} while (GetDoc()->m_nLevelCount < BINARIZE_MAX_LEVEL);
			 
			if(GetDoc()->m_fMaxScore > 0.f)
			{
				GetDoc()->m_nBinarizePCB[nModelIndex/MAX_CAM_NUM] = GetDoc()->m_nTuneLevl;
				GetDoc()->SaveBinarizeCondition();//이진화 레벨 저장
				AfxMessageBox("FOV OUT!!!  Model Teaching Fail.....");
				return;
			} 
			else
			{
				AfxMessageBox("Piles !!  Axis Y Backward");//겹침 판정
				GetAlignCtl()->ContactPosCalibaration_FFC_Edge(nCameraIndex, TRUE);//이동 위치 환산

				//OnMoveToReadyPos();// Y축 후진
				GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 SESL]

			}
		}	

		//Mark Find.............
		GetDoc()->m_nLevelCount = 10;

		nModelIndex = m_iSelCamera *4 + 1;

		GetDoc()->m_fMaxScore = 0.f;

		
		AfxMessageBox("Mark Tune");

		GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;//  [7/4/2009 SESL]

		do 
		{
			GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + BINARIZE_LEVEL_STEP;

			Sleep(20);

			GetAlignCtl()->SearchPattern2(nModelIndex,TRUE);

		} while (GetDoc()->m_nLevelCount < BINARIZE_MAX_LEVEL);

		if (GetDoc()->m_fMaxScore != 0.f)
		{
			strMessageBuf.Format("MaxScore: %f, MarkFind_X: %f", GetDoc()->m_fMaxScore, GetDoc()->m_fMarkFind_X);
			AfxMessageBox(strMessageBuf);

			GetDoc()->m_nBinarizeFPC_Mark[nModelIndex/MAX_CAM_NUM] = GetDoc()->m_nTuneLevl;
			GetDoc()->SaveBinarizeCondition();//이진화 레벨 저장

			AfxMessageBox("Mark Level Updated!!!");

			//PCB Mark Find
			AfxMessageBox("PCB Mark Finding");

			GetDoc()->m_nLevelCount = 10;

			GetDoc()->m_fMaxScore = 0.f;

			nModelIndex = m_iSelCamera *4 + 3;

			do 
			{
				GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + BINARIZE_LEVEL_STEP;
				Sleep(20);

				GetAlignCtl()->SearchPattern2(nModelIndex,TRUE);
				
			} while (GetDoc()->m_nLevelCount < BINARIZE_MAX_LEVEL);

			if (GetDoc()->m_fMaxScore != 0)
			{
				GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 SESL]

				strMessageBuf.Format("MaxScore: %f, PCBMarkFind_X: %f", GetDoc()->m_fMaxScore, GetDoc()->m_fPCBmarkFind_X);
//				GetDoc()->m_fPCBMark_Fix_X[nModelIndex/MAX_CAM_NUM] = GetDoc()->m_fPCBmarkFind_X;//  [7/3/2009 janghan]
				AfxMessageBox(strMessageBuf);
				GetDoc()->m_nBinarizePCB_Mark[nModelIndex/MAX_CAM_NUM] = GetDoc()->m_nTuneLevl;
				strMessageBuf.Format("PCB - FFC = %f", GetDoc()->m_fPCBmarkFind_X - GetDoc()->m_fMarkFind_X);
				AfxMessageBox(strMessageBuf);

				AfxMessageBox("Sorting Start");	
				GetDoc()->SaveBinarizeCondition();//이진화 레벨 저장

				//최종 위치 확정 

				//nModelIndex = m_iSelCamera *4 + 1;//FFC Mark Find
				//GetDoc()->m_nLevelCount = GetDoc()->m_nBinarizeFPC_Mark[nModelIndex/MAX_CAM_NUM];// FFC Mark Level
				//GetAlignCtl()->SearchPattern2( nModelIndex, FALSE );//좌표 획득을 위해 한번 더 서치

				//nModelIndex = m_iSelCamera *4 + 3;;//PCB Mark Find
				//GetDoc()->m_nLevelCount = GetDoc()->m_nBinarizeFPC_Mark[nModelIndex/MAX_CAM_NUM];// PCB Mark Level
				//GetAlignCtl()->SearchPattern2( nModelIndex, FALSE );//좌표 획득을 위해 한번 더 서치

				GetDoc()->m_fAutoDistance_X =  (GetDoc()->m_fPCBmarkFind_X - GetDoc()->m_fMarkFind_X) * GetApp()->m_dFovX;

				GetAlignCtl()->ContactPosCalibaration_SortX(m_iSelCamera);//이동 위치 환산

				if( m_bPosMoving == TRUE && m_iMoveAction != actNONE )
				{
					AfxMessageBox(" 현재 컨텍 UNIT 동작 중입니다" );
					return;
				}
				m_iMoveAction = actAUTOTUNE_X_SORT ;
			
				//Sleep(100);

				//m_pAmtData->m_CurModelInfo.GrapPos[i].dx

				GetDoc()->SetAlignPos(GetDoc()->m_nCamera_Auto);//현재 좌표 저장

				GetDoc()->SaveModelFile();// 최종 위치 저장
				
			} 
			else AfxMessageBox("PCB Mark Find Fail...");
		}			
		else
		{
				AfxMessageBox("Mark Tune Fail......");
				AfxMessageBox("Broken Mark!!          Model Teaching Fail.....6..");
				return;
		}		
	}
	else AfxMessageBox("Super-User more than is available");

}

void CFormAlign::OnBnClickedBtnUncontactAll()
{
	if( m_bPosMoving == TRUE && m_iMoveAction != actNONE )
	{
		AfxMessageBox(" 현재 컨텍 UNIT 동작 중입니다" );
		return;
	}
	// Inspection End
	GetComm()->PGSend(CMD_ERR_MSG, "PG OFF");
	GetDoc()->BluOff();

	m_iMoveAction = actREADY_MOVE ;
}

void CFormAlign::SetSuperUserControl()
{
	if(GetDoc()->m_bSuperLogin)
	{
		m_btnLoadImage.ShowWindow(SW_SHOW);
// 		m_slxCondition.ShowWindow(SW_SHOW);
// 		m_EditCondition.ShowWindow(SW_SHOW);
	}
	else
	{
		m_btnLoadImage.ShowWindow(SW_HIDE);
// 		m_slxCondition.ShowWindow(SW_HIDE);
// 		m_EditCondition.ShowWindow(SW_HIDE);
	}
}

void CFormAlign::OnPositionChangeIsliderx1()
{
// 	GetDoc()->m_nBinarizeFPC[m_iSelCamera] = (long)m_slxCondition.get_Position();
	CString strText;
	strText.Format("%d", (long)m_slxCondition.get_Position());
	m_EditCondition.SetWindowText(strText);
	GetAlignCtl()->GrabBinarize(DISP_ALIGN, (long)m_slxCondition.get_Position());
}

void CFormAlign::OnEnChangeEditCondition()
{
	CString strText;
	m_EditCondition.GetWindowText(strText);

	long nCondition = (long)atoi(strText.GetBuffer(0));
	m_slxCondition.put_Position((double)nCondition);
	GetAlignCtl()->GrabBinarize(DISP_ALIGN, nCondition);
}
BOOL CFormAlign::SetAlignPos()
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
	}
	// S/W Limit check
	for(int i = 0; i < MAX_CAM_NUM; i++)
	{
		if(CheckLimit(i, ReadyPos[i].dx) == FALSE)	return 0;
		if(CheckLimit(i+4, ReadyPos[i].dy) == FALSE) return 0;
		if(CheckLimit(i, GrabPos[i].dx) == FALSE)	return 0;
		if(CheckLimit(i+4, GrabPos[i].dy) == FALSE)	return 0;
	}
	// 최종 적용
	for(int i = 0; i < MAX_CAM_NUM; i++)
	{
		/*m_dReadyPos[i].dx = ReadyPos[i].dx;
		m_dReadyPos[i].dy = ReadyPos[i].dy;
		m_dGrabPos[i].dx = GrabPos[i].dx;
		m_dGrabPos[i].dy = GrabPos[i].dy;*/

		pModelData->ReadyPos[i].dx = ReadyPos[i].dx;
		pModelData->ReadyPos[i].dy = ReadyPos[i].dy;
		pModelData->GrapPos[i].dx = GrabPos[i].dx;
		pModelData->GrapPos[i].dy = GrabPos[i].dy;

		GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[i].dx = pModelData->GrapPos[i].dx;
		GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[i].dy = pModelData->GrapPos[i].dy;
		//m_pAmtData->m_CurModelInfo.GrapPos[i].dz

	}

	return 1;
}

BOOL CFormAlign::CheckLimit( int nAxis, double dPos )
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
void CFormAlign::OnBnClickedButtonLevelSet()//  [6/28/2009 janghan]
{
	if (GetDoc()->m_bSuperLogin)
	{
		UpdateData(TRUE);

		GetDoc()->m_nFFCEdge_Min = m_nFFC_edgeLevel_Min;
		GetDoc()->m_nFFCEdge_Max = m_nFFC_edgeLevel_Max;
		GetDoc()->m_nFFCMark_Min = m_nFFC_markLevel_Min;
		GetDoc()->m_nFFCMark_Max = m_nFFC_markLevel_Max;
		GetDoc()->m_nPCBEdge_Min = m_nPCB_edgeLevel_Min;
		GetDoc()->m_nPCBEdge_Max = m_nPCB_edgeLevel_Max;
		GetDoc()->m_nPCBMark_Min = m_nPCB_markLevel_Min;
		GetDoc()->m_nPCBMark_Max = m_nPCB_markLevel_Max;
		GetDoc()->m_nLevelStep   = m_nLevelStep;

		GetDoc()->m_bAutoTune_Using = m_bCheck_AutoTune;

		GetDoc()->SaveBinarizeLevelRange();

	}
	else AfxMessageBox("Super-User more than is available");
}

void CFormAlign::OnBnClickedButtonResetPcbx()//  [7/3/2009 janghan] 고정된 PCB X좌표를 Resst
{
	if (GetDoc()->m_bSuperLogin)
	{
		/*GetDoc()->m_fPCBMark_Fix_X[0] = 0.f;
		GetDoc()->m_fPCBMark_Fix_X[1] = 0.f;
		GetDoc()->m_fPCBMark_Fix_X[2] = 0.f;
		GetDoc()->m_fPCBMark_Fix_X[3] = 0.f;*/
	}
}
