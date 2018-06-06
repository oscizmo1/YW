// FormTeach2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormTeach.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxStaticEx.h"
//kjpark 20161025 MCR 구현
#include "Etc/FileSupport.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CFormTeach, CFormView)

CFormTeach::CFormTeach()
	: CFormView(CFormTeach::IDD)
{
	//kjpark 20161025 MCR 구현
	m_bLiveMode = FALSE;
	m_bInit = FALSE;
	
}

CFormTeach::~CFormTeach()
{
	

}
void CFormTeach::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	//kjpark 20161025 MCR 구현
	DDX_Control(pDX, IDC_GXSTC_CELLTR_VIEW, m_imgLive);

}

BEGIN_MESSAGE_MAP(CFormTeach, CFormView)
	ON_WM_TIMER()
	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	//kjpark 20161025 MCR 구현
	ON_MESSAGE(WM_UPDATE_MCRDATA,		OnUpdateMCRData)			// MCR이 수신되었다.
	ON_MESSAGE(WM_UPDATE_MCRLIVE,		OnUpdateMCRLive)			// Live 화면이이 수신되었다.
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CFormTeach, CFormView)
	

	ON_EVENT(CFormTeach, IDC_GXBTN_CELLTR_CELLX_BUFF, DISPID_CLICK, CFormTeach::ClickBtnBufStg, VTS_NONE)
	ON_EVENT(CFormTeach, IDC_GXBTN_CELLTR_CELLX_MCRCH3, DISPID_CLICK, CFormTeach::ClickBtnMCRCh3, VTS_NONE)
	ON_EVENT(CFormTeach, IDC_GXBTN_CELLTR_CELLX_MCRCH2, DISPID_CLICK, CFormTeach::ClickBtnMCRCh2, VTS_NONE)
	ON_EVENT(CFormTeach, IDC_GXBTN_CELLTR_CELLX_MCRCH1, DISPID_CLICK, CFormTeach::ClickBtnMCRCh1, VTS_NONE)
	ON_EVENT(CFormTeach, IDC_GXBTN_CELLTR_CELLX_PRE, DISPID_CLICK, CFormTeach::ClickBtnPreAlign, VTS_NONE)
	//kjpark 20161025 MCR 구현
	ON_EVENT(CFormTeach, IDC_GXBTN_CELLTR_MCR_READ, DISPID_CLICK, CFormTeach::ClickBtnMCRRead, VTS_NONE)
	//ON_EVENT(CFormTeach, IDC_GXBTN_CELLTR_RESTART, DISPID_CLICK, CFormTeach::ClickBtnReConnect, VTS_NONE)
	ON_EVENT(CFormTeach, IDC_GXBTN_CELLTR_LIVE, DISPID_CLICK, CFormTeach::ClickBtnLiveMode, VTS_NONE)
END_EVENTSINK_MAP()

// CFormTeach2 진단입니다.

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


// CFormTeach2 메시지 처리기입니다.

// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
//
// CGxGUIFrame에서 ShowDialog를 수행하면 아래 순서대로 메세지를 호출한다.
//
// ShowDialog() 호출시
// 1. GxMSG_GUI_UPDATE_TIMER, TRUE
//
// HideDlgBar() 호출시
// 1. GxMSG_GUI_UPDATE_TIMER, FALSE
//

// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
LRESULT CFormTeach::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		//kjpark 20161025 MCR 구현
		m_imgLive.LoadImage(MCRPath);
		m_imgLive.Invalidate();			
		UpdateCellTRState();
		SetTimer(1, 100, NULL);

	}
	else
	{
		KillTimer(1);
	}
	//kjpark 20161025 MCR 구현
	m_bShow = bFlag;
	return 0;
}
void CFormTeach::OnInitialUpdate()
{
	if(m_bInit)	return;	// OnInitialUpdate 두번 실행되는 버그 방지 [10/24/2016 OSC]
	m_bInit = TRUE;
	CFormView::OnInitialUpdate();

	m_CurAxis = (AXIS_ID)AXIS_JIG_SHUTTLE_Y1;
	m_CurTeach = TEACH_PARAM::JIG_SHUTTLE_Y1_to_LOAD;
	m_CurID	  = IDC_GXBTN_CELLTR_CELLX_BUFF;
	m_AxisList[0] = IDC_GXBTN_CELLTR_CELLX_BUFF;
	m_AxisList[1] = IDC_GXBTN_CELLTR_CELLX_MCRCH3;
	m_AxisList[2] = IDC_GXBTN_CELLTR_CELLX_MCRCH2;
	m_AxisList[3] = IDC_GXBTN_CELLTR_CELLX_MCRCH1;
	m_AxisList[4] = IDC_GXBTN_CELLTR_CELLX_PRE;
	
}
void CFormTeach::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case 1:
		UpdateCellTRState();
		break;
	}

	CFormView::OnTimer(nIDEvent);
}
// 표시 언어가 변경되었다.
LRESULT CFormTeach::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	// CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_BOTTOM_MENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormTeach::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}



void CFormTeach::UpdateCellTRState()
{
	
	//VACCUM 관련 버튼들도 실시간으로 변하게 수정하였음.
// 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CELLZ_UP, (theUnitFunc.GetOutPutIOCheck(Y_CELL_TR_UP)? GXCOLOR_VACUUM_ON:GXCOLOR_OFF));
// 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CELLZ_DOWN, (theUnitFunc.GetOutPutIOCheck(Y_CELL_TR_DOWN)? GXCOLOR_VACUUM_ON:GXCOLOR_OFF));
// 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CH1_VAC_ON, (theUnitFunc.GetOutPutIOCheck(Y_CELL_TR_CH1_CELL_VAC)? GXCOLOR_VACUUM_ON:GXCOLOR_OFF));
// 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CH2_VAC_ON, (theUnitFunc.GetOutPutIOCheck(Y_CELL_TR_CH2_CELL_VAC)? GXCOLOR_VACUUM_ON:GXCOLOR_OFF));
// 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CH3_VAC_ON, (theUnitFunc.GetOutPutIOCheck(Y_CELL_TR_CH3_CELL_VAC)? GXCOLOR_VACUUM_ON:GXCOLOR_OFF));
}




void CFormTeach::ClickBtnBufStg()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH"), _T("CELL_TR_BUFSTG"));
// 	m_CurAxis = (AXIS_ID)AXIS_FORCE_JIG_SHUTTLE_Y1;
// 	m_CurTeach = TEACH_PARAM::FORCE_JIG_SHUTTLE_Y1_to_WAIT;
// 	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_CELLTR_CELLX_BUFF);

}


void CFormTeach::ClickBtnMCRCh3()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH"), _T("CELL_TR_MCR_CH3"));
// 	m_CurAxis = (AXIS_ID)AXIS_FORCE_JIG_SHUTTLE_Y1;
// 	m_CurTeach = TEACH_PARAM::FORCE_JIG_SHUTTLE_Y1_to_WAIT;
// 	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_CELLTR_CELLX_MCRCH3, CELLTR_CH3);
	
	//kjpark 20161025 MCR 구현
	/*GetMainHandler()->SetSelectedAxis(AXIS_LD_CELL_TR_X, TEACH_PARAM::LD_CELL_TR_X_to_MCR);
	cell_motor_btn = CELLTR_MCR_CH3_BTN;
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CELLX_BUFF,GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CELLX_MCRCH3,GXCOLOR_MOTOR_ON);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CELLX_MCRCH2,GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CELLX_MCRCH1,GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CELLX_PRE,GXCOLOR_OFF);
	UpdateCellTRState();*/
}


void CFormTeach::ClickBtnMCRCh2()
{
// 	//20170501 LMS OPERATION 추가
// 	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH"), _T("CELL_TR_MCR_CH2"));
// 	m_CurAxis = (AXIS_ID)AXIS_TRAY1_TR_Y;
// 	m_CurTeach = TEACH_PARAM::TRAY1_TR_Y_to_IN;
// 	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_CELLTR_CELLX_MCRCH2,CELLTR_CH2);
	/*GetMainHandler()->SetSelectedAxis(AXIS_LD_CELL_TR_X, TEACH_PARAM::LD_CELL_TR_X_to_MCR);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CELLX_BUFF,GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CELLX_MCRCH3,GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CELLX_MCRCH2,GXCOLOR_MOTOR_ON);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CELLX_MCRCH1,GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CELLX_PRE,GXCOLOR_OFF);
	cell_motor_btn = CELLTR_MCR_CH2_BTN;
	UpdateCellTRState();*/
}


void CFormTeach::ClickBtnMCRCh1()
{
	//20170501 LMS OPERATION 추가
// 	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH"), _T("CELL_TR_MCR_CH1"));
// 	m_CurAxis = (AXIS_ID)AXIS_TRAY1_TR_Y;
// 	m_CurTeach = TEACH_PARAM::TRAY1_TR_Y_to_IN;
// 	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_CELLTR_CELLX_MCRCH1,CELLTR_CH1);
	//kjpark 20161025 MCR 구현
	/*GetMainHandler()->SetSelectedAxis(AXIS_LD_CELL_TR_X, TEACH_PARAM::LD_CELL_TR_X_to_MCR);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CELLX_BUFF,GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CELLX_MCRCH3,GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CELLX_MCRCH2,GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CELLX_MCRCH1,GXCOLOR_MOTOR_ON);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CELLX_PRE,GXCOLOR_OFF);
	cell_motor_btn = CELLTR_MCR_CH1_BTN;
	UpdateCellTRState();*/
}


void CFormTeach::ClickBtnPreAlign()
{
	//20170501 LMS OPERATION 추가
// 	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH"), _T("CELL_TR_PREALIGN"));
// 	m_CurAxis = (AXIS_ID)AXIS_TRAY1_TR_Y;
// 	m_CurTeach = TEACH_PARAM::TRAY1_TR_Y_to_IN;
// 	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_CELLTR_CELLX_PRE);
	//kjpark 20161025 MCR 구현
	/*GetMainHandler()->SetSelectedAxis(AXIS_LD_CELL_TR_X, TEACH_PARAM::LD_CELL_TR_X_to_PREALIGN_STG);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CELLX_BUFF,GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CELLX_MCRCH3,GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CELLX_MCRCH2,GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CELLX_MCRCH1,GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_CELLX_PRE,GXCOLOR_MOTOR_ON);
	cell_motor_btn = CELLTR_PRE_ALIGN_BTN;
	UpdateCellTRState();*/
}

//kjpark 20161025 MCR 구현
void CFormTeach::ClickBtnMCRRead()
{
	//20170501 LMS OPERATION 추가
// 	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH"), _T("CELL_TR_READ_MCR"));
// 	theProcBank.m_strLastCellID.Empty();
// 	CGxUICtrl::SetStaticString(this, IDC_GXSTC_CELLTR_MCR_POS, _T(""));	
	//theUnitFunc.SetSoftTrigger();//소프트 트리거
	theUnitFunc.MCR_Trigger(TRUE);
	Sleep(200);
	theUnitFunc.MCR_Trigger(FALSE);
}

//kjpark 20161025 MCR 구현
void CFormTeach::ClickBtnLiveMode()
{
	/*
	if(theUnitFunc.GetLiveMode())
	{
		m_bLiveMode = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_LIVE, GXCOLOR_OFF);
	}
	else
	{
		m_bLiveMode = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_LIVE, GXCOLOR_VACUUM_ON);
	}	
	*/
	m_bLiveMode = !m_bLiveMode;
	theUnitFunc.SetLiveMode(SHUTTLE_1, JIG_CH_1, m_bLiveMode);
	if(m_bLiveMode)
	{
		//20170501 LMS OPERATION 추가
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH"), _T("CELL_TR_READ_LIVE"));
		CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_CELLTR_MCR_READ, FALSE);
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_LIVE, GXCOLOR_VACUUM_ON);
		
	}
	else
	{
		CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_CELLTR_MCR_READ, TRUE);
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_CELLTR_LIVE, GXCOLOR_OFF);
	}
}

//kjpark 20161025 MCR 구현
void CFormTeach::SetMCRLive()
{
	if ( m_bShow == FALSE) return ;

	// 취득한 이미지를 표시한다.
	if(CFileSupport::FileCheck(MCRPath)  && m_bShow/* && theUnitFunc.GetLiveMode()*/)
	{
		m_imgLive.LoadImage( MCRPath );
		m_imgLive.Invalidate();
	}

}

//kjpark 20161025 MCR 구현
void CFormTeach::SetMCRData(CString strBarcode)
{
	if(m_bShow == FALSE)
		return;

	if ( theUnitFunc.GetSoftTriggerResult(SHUTTLE_1, JIG_CH_1) == 1 )
	{
		CGxUICtrl::SetStaticString(this, IDC_GXSTC_CELLTR_MCR_POS, strBarcode);
	}
	else
		CGxUICtrl::SetStaticString(this, IDC_GXSTC_CELLTR_MCR_POS, strBarcode);
// 		CGxUICtrl::SetStaticString(this, IDC_GXSTC_CELLTR_MCR_POS, _T(""));


	CString sFile = theUnitFunc.GetLastSavedImage();
	if(CFileSupport::FileCheck(sFile)  && m_bShow )
	{
		m_imgLive.LoadImage( sFile );
		m_imgLive.Invalidate();

	}
	
}

// MCR이 수신되었다.
//kjpark 20161025 MCR 구현
LRESULT CFormTeach::OnUpdateMCRData(WPARAM wParam, LPARAM lParam)
{
	SetMCRData(theUnitFunc.GetSoftTriggerData(SHUTTLE_1, JIG_CH_1));

	return 0;
}


// MCR Live 영상 수신
//kjpark 20161025 MCR 구현
LRESULT CFormTeach::OnUpdateMCRLive(WPARAM wParam, LPARAM lParam)
{
	SetMCRLive();

	return 0;
}
void CFormTeach::SetCurPageAxis(AXIS_ID axis, TEACH_PARAM::ID param, UINT nID)
{
	m_CurAxis = axis;
	m_CurTeach = param;
	m_CurID	  = nID;	
	GetMainHandler()->SetSelectedAxis(m_CurAxis, m_CurTeach);
}

void CFormTeach::SetCurPageAxis(AXIS_ID axis, TEACH_PARAM::ID param, UINT nID, int nChanel)
{
	m_CurAxis = axis;
	m_CurTeach = param;
	m_CurID	  = nID;	
	if(nChanel==CELLTR_CH2)
		GetMainHandler()->SetSelectedAxis(m_CurAxis, m_CurTeach,CHANNEL_PITCH);
	else if(nChanel==CELLTR_CH1)
		GetMainHandler()->SetSelectedAxis(m_CurAxis, m_CurTeach,CHANNEL_PITCH*2);
	else
		GetMainHandler()->SetSelectedAxis(m_CurAxis, m_CurTeach);
}
