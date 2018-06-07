// FormSetup1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormSignal.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxNumPadEx.h"
#include "UI\Dialog\DlgBoxKeyBoard.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CFormSignal

IMPLEMENT_DYNCREATE(CFormSignal, CFormView)

CFormSignal::CFormSignal()
	: CFormView(CFormSignal::IDD)
{
	//kjpark 20161021 Setup 신호기 UI 구현
	//kjpark 20170908 Setup 시그널 페이지 수정
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
			m_bButtonCheck[i][j] = FALSE;
	}	

	//kjpark 20170908 Setup 시그널 페이지 수정
	m_nCellButton[JIG_ID_A][JIG_CH_1] = IDC_GXBTN_TEACH1_PG_A1;

	m_nCellButton[JIG_ID_B][JIG_CH_1] = IDC_GXBTN_TEACH1_PG_B1;

#ifdef NOT_USED
	m_nCellButton[ZONE_ID_A][JIG_CH_1] = IDC_GXBTN_TEACH1_PG_A1;
// 	m_nCellButton[ZONE_ID_A][JIG_CH_3] = IDC_GXBTN_TEACH1_PG_A3;
// 	m_nCellButton[ZONE_ID_A][JIG_CH_4] = IDC_GXBTN_TEACH1_PG_A4;
// 	m_nCellButton[ZONE_ID_A][JIG_CH_5] = IDC_GXBTN_TEACH1_PG_A5;
	m_nCellButton[JIG_ID_B][JIG_CH_1] = IDC_GXBTN_TEACH1_PG_B1;
// 	m_nCellButton[JIG_ID_B][JIG_CH_3] = IDC_GXBTN_TEACH1_PG_B3;
// 	m_nCellButton[JIG_ID_B][JIG_CH_4] = IDC_GXBTN_TEACH1_PG_B4;
// 	m_nCellButton[JIG_ID_B][JIG_CH_5] = IDC_GXBTN_TEACH1_PG_B5;
	m_nCellButton[JIG_ID_C][JIG_CH_1] = IDC_GXBTN_TEACH1_PG_C1;
// 	m_nCellButton[JIG_ID_C][JIG_CH_3] = IDC_GXBTN_TEACH1_PG_C3;
// 	m_nCellButton[JIG_ID_C][JIG_CH_4] = IDC_GXBTN_TEACH1_PG_C4;
// 	m_nCellButton[JIG_ID_C][JIG_CH_5] = IDC_GXBTN_TEACH1_PG_C5;
// 	m_nCellButton[JIG_ID_D][JIG_CH_1] = IDC_GXBTN_TEACH1_PG_D1;
// 	m_nCellButton[JIG_ID_D][JIG_CH_2] = IDC_GXBTN_TEACH1_PG_D2;
// 	m_nCellButton[JIG_ID_D][JIG_CH_3] = IDC_GXBTN_TEACH1_PG_D3;
// 	m_nCellButton[JIG_ID_D][JIG_CH_4] = IDC_GXBTN_TEACH1_PG_D4;
// 	m_nCellButton[JIG_ID_D][JIG_CH_5] = IDC_GXBTN_TEACH1_PG_D5;
#endif
	m_bIsMutli = TRUE;
	m_bIsShwoWindow = FALSE;
	m_nPatternNo=101;
	m_nSquareThickness=5;
	m_nSquareColorLineRed=0;
	m_nSquareColorLineGreen=0;
	m_nSquareColorLineBlue=0;
	m_nSquareSize=100;
	m_nSquareXPoint=0;
	m_nSquareYPoint=0;
	m_nMSGBackColorRed=0;
	m_nMSGBackColorGreen=0;
	m_nMSGBackColorBlue=0;
	m_nMSGFontColorRed=255;
	m_nMSGFontColorGreen=255;
	m_nMSGFontColorBlue=255;
	m_nMSGXPoint=20;
	m_nMSGYPoint=20;
	m_nMSGFontSize=50;
	m_strMSG=_T("Test");
}

CFormSignal::~CFormSignal()
{
}

void CFormSignal::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//kjpark 20161021 Setup 신호기 UI 구현
	DDX_Control(pDX, IDC_LIST_TEACH1_LOG, m_PGLog);
}

BEGIN_MESSAGE_MAP(CFormSignal, CFormView)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

END_MESSAGE_MAP()

//kjpark 20161021 Setup 신호기 UI 구현
BEGIN_EVENTSINK_MAP(CFormSignal, CFormView)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_A1, DISPID_CLICK, CFormSignal::ClickBtnSelect_A1, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_A2, DISPID_CLICK, CFormSignal::ClickBtnSelect_A2, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_A3, DISPID_CLICK, CFormSignal::ClickBtnSelect_A3, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_A4, DISPID_CLICK, CFormSignal::ClickBtnSelect_A4, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_A5, DISPID_CLICK, CFormSignal::ClickBtnSelect_A5, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_B1, DISPID_CLICK, CFormSignal::ClickBtnSelect_B1, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_B2, DISPID_CLICK, CFormSignal::ClickBtnSelect_B2, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_B3, DISPID_CLICK, CFormSignal::ClickBtnSelect_B3, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_B4, DISPID_CLICK, CFormSignal::ClickBtnSelect_B4, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_B5, DISPID_CLICK, CFormSignal::ClickBtnSelect_B5, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_C1, DISPID_CLICK, CFormSignal::ClickBtnSelect_C1, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_C2, DISPID_CLICK, CFormSignal::ClickBtnSelect_C2, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_C3, DISPID_CLICK, CFormSignal::ClickBtnSelect_C3, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_C4, DISPID_CLICK, CFormSignal::ClickBtnSelect_C4, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_C5, DISPID_CLICK, CFormSignal::ClickBtnSelect_C5, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_D1, DISPID_CLICK, CFormSignal::ClickBtnSelect_D1, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_D2, DISPID_CLICK, CFormSignal::ClickBtnSelect_D2, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_D3, DISPID_CLICK, CFormSignal::ClickBtnSelect_D3, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_D4, DISPID_CLICK, CFormSignal::ClickBtnSelect_D4, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_D5, DISPID_CLICK, CFormSignal::ClickBtnSelect_D5, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_SELECT_ALL, DISPID_CLICK, CFormSignal::ClickBtnSelect_All, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_SELECT_CLEAR, DISPID_CLICK, CFormSignal::ClickBtnSelect_Clear, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_SELECT_S, DISPID_CLICK, CFormSignal::ClickBtnSelect_Single, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_SELECT_M, DISPID_CLICK, CFormSignal::ClickBtnSelect_Multi, VTS_NONE)

	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_AZONE, DISPID_CLICK, CFormSignal::ClickBtnZoneSelect_A, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_BZONE, DISPID_CLICK, CFormSignal::ClickBtnZoneSelect_B, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_CZONE, DISPID_CLICK, CFormSignal::ClickBtnZoneSelect_C, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_DZONE, DISPID_CLICK, CFormSignal::ClickBtnZoneSelect_D, VTS_NONE)

	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PATTERN1, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_ON, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PATTERN2, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_OFF, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PATTERN3, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_PREV, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PATTERN4, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_NEXT, VTS_NONE)

	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION1, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_Reset, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION2, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_CellLoading, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION3, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_TMDInfo, VTS_NONE)
	//kjpark 20180113 Tmd Version 추가
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_HOST_VERSION, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_HostVersion, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_CLIENT_VERSION, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_ClientVersion, VTS_NONE)	
	//kjpark 20171016 하이페리온 커넷트 체크 CMD send 기능
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_HYPERION, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_HyperionChk, VTS_NONE)	
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION9, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_TSPStart, VTS_NONE)

	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION7, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_IDCheck, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION8, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_OTPREG, VTS_NONE)

	//kjpark 20170908 Setup 시그널 페이지 수정
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_EVT_VERSION_CHECK, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_EVT_Version, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_TE_CHECK, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_TECheck, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_COPR_ICT_CHECK, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_COPR_ICT, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_POC_ERROR_CHECK, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_POC_Error, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_DDI_BLOCK_TEST_CHECK, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_DDI_Block_Test, VTS_NONE)	
	
	//kjpark 20161114 C Zone Foce 추가
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_FORCE, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_Force, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_FORCE2, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_Force2, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_FORCE3, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_Force3, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_FORCE1_BEF, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1FunctionForce1Bef, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_FORCE2_BEF, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1FunctionForce2Bef, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_FORCE3_BEF, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1FunctionForce3Bef, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_ICT_CHECK, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1FunctionIctCheck, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_OPTION_CHECK2, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1FunctionOptionCheck2, VTS_NONE)
	//kjpark 20170908 Setup 시그널 페이지 수정
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_OPTION_CHECK3, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1FunctionOptionCheck3, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_OPTION_CHECK4, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1FunctionOptionCheck4, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_OPTION_CHECK5, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1FunctionOptionCheck5, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_OPTION_CHECK6, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1FunctionOptionCheck6, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_OPTION_CHECK7, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1FunctionOptionCheck7, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_OPTION_CHECK8, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1FunctionOptionCheck8, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_OPTION_CHECK9, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1FunctionOptionCheck9, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_OPTION_CHECK10, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1FunctionOptionCheck10, VTS_NONE)
	//kjpark 20171010 신호기 UI 에 White Circle 추가
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_OPTION_SIGNAL_WHITE_CIRCLE, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1FunctionWhiteCircle, VTS_NONE)
	
	
	//kjpark 20170908 Setup 시그널 페이지 수정	
	//kjpark 20170911 신호기 UI MTP COMMAND 추가
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_MTP_WRITE, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1FunctionMTPWrite, VTS_NONE)	
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_MTP_VERIFY, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1FunctionMTPVerify, VTS_NONE)	
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_HLPM_CHECK, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1FunctionHlpmCheck, VTS_NONE)	
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_SLEEP_CURRENT, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1FunctionSleepCurrent, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_WHITE_CURRENT, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1FunctionWhiteCurrent, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXSTC_ZONE_A, DISPID_CLICK, CFormSignal::ClickGxstcZoneA, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXSTC_ZONE_B, DISPID_CLICK, CFormSignal::ClickGxstcZoneB, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXSTC_ZONE_C, DISPID_CLICK, CFormSignal::ClickGxstcZoneC, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXSTC_ZONE_D, DISPID_CLICK, CFormSignal::ClickGxstcZoneD, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXSTC_CH1, DISPID_CLICK, CFormSignal::ClickGxstcCh1, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXSTC_CH2, DISPID_CLICK, CFormSignal::ClickGxstcCh2, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXSTC_CH3, DISPID_CLICK, CFormSignal::ClickGxstcCh3, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXSTC_CH4, DISPID_CLICK, CFormSignal::ClickGxstcCh4, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXSTC_CH5, DISPID_CLICK, CFormSignal::ClickGxstcCh5, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXLBL_PATTERN_NO, DISPID_CLICK, CFormSignal::ClickGxlblPatternNo, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_DZONE_PATTERN_SHOW, DISPID_CLICK, CFormSignal::ClickGxbtnDzonePatternShow, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXLBL_SQUARE_THICKNESS, DISPID_CLICK, CFormSignal::ClickGxlblSquareThickness, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXLBL_SQUARE_SIZE, DISPID_CLICK, CFormSignal::ClickGxlblSquareSize, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXLBL_SQUARE_COLORLINE, DISPID_CLICK, CFormSignal::ClickGxlblSquareColorline, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXLBL_SQUARE_X, DISPID_CLICK, CFormSignal::ClickGxlblSquareX, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXLBL_SQUARE_Y, DISPID_CLICK, CFormSignal::ClickGxlblSquareY, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_SQUARE_ADD, DISPID_CLICK, CFormSignal::ClickGxbtnSquareAdd, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_SQUARE_DRAW, DISPID_CLICK, CFormSignal::ClickGxbtnSquareDraw, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXLBL_MSG_BACKCOLOR, DISPID_CLICK, CFormSignal::ClickGxlblMsgBackcolor, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXLBL_MSG_FONTCOLOR, DISPID_CLICK, CFormSignal::ClickGxlblMsgFontcolor, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXLBL_MSG_X, DISPID_CLICK, CFormSignal::ClickGxlblMsgX, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXLBL_MSG_Y, DISPID_CLICK, CFormSignal::ClickGxlblMsgY, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXLBL_MSG_FONTSIZE, DISPID_CLICK, CFormSignal::ClickGxlblMSGFontsize, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_MSG_ADD, DISPID_CLICK, CFormSignal::ClickGxbtnMsgAdd, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_MSG_DRAW, DISPID_CLICK, CFormSignal::ClickGxbtnMsgDraw, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION_OPTION_SIGNAL_POWERRESET, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1FunctionOptionSignalPowerReset, VTS_NONE)
	END_EVENTSINK_MAP()
// CFormSetup1 진단입니다.

#ifdef _DEBUG
void CFormSignal::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormSignal::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormSetup1 메시지 처리기입니다.


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
LRESULT CFormSignal::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		//kjpark 20161021 Setup 신호기 UI 구현
		m_bIsShwoWindow = TRUE;
		if(m_bIsMutli)
		{
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_PG_SELECT_S, GXCOLOR_OFF);
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_PG_SELECT_M, GXCOLOR_ON);
		}
		else
		{
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_PG_SELECT_S, GXCOLOR_ON);
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_PG_SELECT_M, GXCOLOR_OFF);
		}

		LV_COLUMN lCol;
		lCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;

		lCol.fmt = LVCFMT_CENTER;   // 칼럼의 정렬을 왼쪽정렬로 지정(LVCFMT_CENTER, LVCFMT_LEFT, LVCFMT_RIGHT중 선택)
		lCol.pszText = _T("NO");    //  칼럼의 제목을 지정
		lCol.iSubItem = 0;    
		CRect rect;//  서브아이템의 인덱스를 지정
		m_PGLog.GetWindowRect(rect);
		lCol.cx = rect.Width()*2;               //  칼럼의 넓이를 지정
		m_PGLog.InsertColumn(0, &lCol);  //  LVCOLUMN구조체로 만들어진 값을 토대로 리스트 컨트롤에 칼럼을 삽입
	}
	else
	{
		m_bIsShwoWindow = FALSE;
	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CFormSignal::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	// CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_BOTTOM_MENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormSignal::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}

//kjpark 20161021 Setup 신호기 UI 구현
BOOL CFormSignal::GetSelectButton(ZONE_ID zone, JIG_CH nCh)
{	
	m_bButtonCheck[zone][nCh] = !m_bButtonCheck[zone][nCh];

	if(m_bIsMutli)
	{
		if(m_bButtonCheck[zone][nCh])
		{
			CGxUICtrl::SetButtonColor(this,m_nCellButton[zone][nCh], GXCOLOR_ON);	
		}
		else
		{
			CGxUICtrl::SetButtonColor(this,m_nCellButton[zone][nCh], GXCOLOR_OFF);
		}
	}
	else
	{
		for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
		{
			for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
			{
				m_bButtonCheck[i][j] = FALSE;
				CGxUICtrl::SetButtonColor(this,m_nCellButton[i][j], GXCOLOR_OFF);
			}
		}
		m_bButtonCheck[zone][nCh] = TRUE;
		CGxUICtrl::SetButtonColor(this,m_nCellButton[zone][nCh], GXCOLOR_ON);
	}
	

	return m_bButtonCheck[zone][nCh];
}

//kjpark 20170908 Setup 시그널 페이지 수정
BOOL CFormSignal::GetSelectButton(JIG_ID shttle, JIG_CH nCh)
{	
	m_bButtonCheck[shttle][nCh] = !m_bButtonCheck[shttle][nCh];

	if(m_bIsMutli)
	{
		if(m_bButtonCheck[shttle][nCh])
		{
			CGxUICtrl::SetButtonColor(this,m_nCellButton[shttle][nCh], GXCOLOR_ON);	
		}
		else
		{
			CGxUICtrl::SetButtonColor(this,m_nCellButton[shttle][nCh], GXCOLOR_OFF);
		}
	}
	else
	{
		for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
		{
			for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
			{
				m_bButtonCheck[i][j] = FALSE;
				CGxUICtrl::SetButtonColor(this,m_nCellButton[i][j], GXCOLOR_OFF);
			}
		}
		m_bButtonCheck[shttle][nCh] = TRUE;
		CGxUICtrl::SetButtonColor(this,m_nCellButton[shttle][nCh], GXCOLOR_ON);
	}


	return m_bButtonCheck[shttle][nCh];
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnSelect_A1()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_A1"));
	GetSelectButton(JIG_ID_A, JIG_CH_1);
	
}

//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnSelect_A2()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_A2"));
	//GetSelectButton(JIG_ID_A, JIG_CH_2);
}

//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnSelect_A3()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_A3"));
// 	GetSelectButton(ZONE_ID_A, JIG_CH_3);
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnSelect_A4()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_A4"));
// 	GetSelectButton(ZONE_ID_A, JIG_CH_4);
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnSelect_A5()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_A5"));
// 	GetSelectButton(ZONE_ID_A, JIG_CH_5);
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnSelect_B1()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_B1"));
 	GetSelectButton(JIG_ID_B, JIG_CH_1);
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnSelect_B2()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_B2"));
 	//GetSelectButton(JIG_ID_B, JIG_CH_2);
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnSelect_B3()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_B3"));
// 	GetSelectButton(ZONE_ID_B, JIG_CH_3);
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnSelect_B4()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_B4"));
// 	GetSelectButton(ZONE_ID_B, JIG_CH_4);
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnSelect_B5()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_B5"));
// 	GetSelectButton(ZONE_ID_B, JIG_CH_5);
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnSelect_C1()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_C1"));
//	GetSelectButton(JIG_ID_C, JIG_CH_1);
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnSelect_C2()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_C2"));
	//GetSelectButton(JIG_ID_C, JIG_CH_2);
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnSelect_C3()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_C3"));
// 	GetSelectButton(ZONE_ID_C, JIG_CH_3);
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnSelect_C4()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_C4"));
// 	GetSelectButton(ZONE_ID_C, JIG_CH_4);
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnSelect_C5()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_C5"));
// 	GetSelectButton(ZONE_ID_C, JIG_CH_5);
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnSelect_D1()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_D1"));
// 	GetSelectButton(ZONE_ID_D, JIG_CH_1);
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnSelect_D2()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_D2"));
// 	GetSelectButton(ZONE_ID_D, JIG_CH_2);
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnSelect_D3()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_D3"));
// 	GetSelectButton(ZONE_ID_D, JIG_CH_3);
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnSelect_D4()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_D4"));
// 	GetSelectButton(ZONE_ID_D, JIG_CH_4);
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnSelect_D5()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_D5"));
// 	GetSelectButton(ZONE_ID_D, JIG_CH_5);
}

//kjpark 20161021 Setup 신호기 UI 구현
//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickBtnSelect_All()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_ALL"));

	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			m_bButtonCheck[i][j] = TRUE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][j], GXCOLOR_ON);
		}
	}	
	ClickBtnSelect_Multi();

#ifdef NOT_USED
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_ALL"));

	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			m_bButtonCheck[i][j] = TRUE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][j], GXCOLOR_ON);
		}
	}
	ClickBtnSelect_Multi();
#endif	
}
//kjpark 20161021 Setup 신호기 UI 구현
//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickBtnSelect_Clear()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_CLEAR"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			m_bButtonCheck[i][j] = FALSE;

			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][j], GXCOLOR_OFF);
		}
	}		
#ifdef NOT_USED
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_CLEAR"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			m_bButtonCheck[i][j] = FALSE;

			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][j], GXCOLOR_OFF);
		}
	}	
#endif
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnSelect_Single()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_SINGLE"));
	m_bIsMutli = FALSE;
	ClickBtnSelect_Clear();
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_PG_SELECT_S, GXCOLOR_ON);
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_PG_SELECT_M, GXCOLOR_OFF);
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnSelect_Multi()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PG_MULTI"));
	m_bIsMutli = TRUE;
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_PG_SELECT_S, GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_PG_SELECT_M, GXCOLOR_ON);
}

//kjpark 20161021 Setup 신호기 UI 구현
//kjpark 20161103 Zone Signal 동작 버그 수정
//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickBtnZoneSelect_A()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SET_ZONE_A"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{

				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(SET_ZONE_A, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}
#ifdef NOT_USED
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SET_ZONE_A"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(SET_ZONE_A, (JIG_ID)i,(JIG_CH)j);
				//theUnitFunc.SendPGMessage(SET_ZONE_A, (ZONE_ID)i, (JIG_CH)j);
			}
		}
	}
#endif
}
//kjpark 20161021 Setup 신호기 UI 구현
//kjpark 20161103 Zone Signal 동작 버그 수정
void CFormSignal::ClickBtnZoneSelect_B()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SET_ZONE_B"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(SET_ZONE_B, (JIG_ID)i,(JIG_CH)j);
				//theUnitFunc.SendPGMessage(SET_ZONE_B, (ZONE_ID)i, (JIG_CH)j);
			}
		}
	}		
}
//kjpark 20161021 Setup 신호기 UI 구현
//kjpark 20161103 Zone Signal 동작 버그 수정
//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickBtnZoneSelect_C()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SET_ZONE_C"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(SET_ZONE_C, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
#ifdef NOT_USED
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SET_ZONE_C"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(SET_ZONE_C, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
#endif
}
//kjpark 20161021 Setup 신호기 UI 구현
//kjpark 20161103 Zone Signal 동작 버그 수정
void CFormSignal::ClickBtnZoneSelect_D()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SET_ZONE_D"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(SET_ZONE_D, (JIG_ID)i,(JIG_CH)j);
				//theUnitFunc.SendPGMessage(SET_ZONE_D, (ZONE_ID)i, (JIG_CH)j);
			}
		}
	}		
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickBtnPGSelect_ON()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PATTERN_TURN_ON"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(TURN_ON, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickBtnPGSelect_OFF()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PATTERN_TURN_OFF"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(TURN_OFF, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickBtnPGSelect_PREV()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PATTERN_PREV"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(BACK, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickBtnPGSelect_NEXT()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PATTERN_NEXT"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(NEXT, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickBtnPGSelect_Reset()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PATTERN_RESET"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(RESET, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickBtnPGSelect_CellLoading()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PATTERN_CellLoading"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(CELL_LOADING, (JIG_ID)i,(JIG_CH)j, _T("MANUAL"));
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickBtnPGSelect_TMDInfo()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PATTERN_TMDInfo"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(TMD_INFO, (ZONE_ID)i, (JIG_CH)j);
			}
		}
	}	
}

//kjpark 20180113 Tmd Version 추가
void CFormSignal::ClickBtnPGSelect_HostVersion()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PATTERN_HOST VERSION"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(HOST_VER, (ZONE_ID)i, (JIG_CH)j);
			}
		}
	}	
}

//kjpark 20180113 Tmd Version 추가
void CFormSignal::ClickBtnPGSelect_ClientVersion()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PATTERN_CLIENT VERSION"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(CLIENT_VER, (ZONE_ID)i, (JIG_CH)j);
			}
		}
	}	
}

//kjpark 20171016 하이페리온 커넷트 체크 CMD send 기능
void CFormSignal::ClickBtnPGSelect_HyperionChk()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PATTERN_Hyperion"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(HYPERION_CONNECT_CHK, (ZONE_ID)i, (JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickBtnPGSelect_TSPStart()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_TSP_START"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(TSP_START, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}		
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickBtnPGSelect_IDCheck()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_ID_CHACK"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(ID_CHECK, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickBtnPGSelect_OTPREG()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_OTP_REG"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(OTP_REG_CHECK, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickBtnPGSelect_EVT_Version()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_EVT_VERSION"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(EVT_VERSION_CHECK, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickBtnPGSelect_TECheck()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_TE_CHECK"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(TE_CHECK, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickBtnPGSelect_COPR_ICT()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_COPR_ICT_CHECK"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(COPR_ICT_TEST, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickBtnPGSelect_POC_Error()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_POC_ERROR_CHECK"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(POC_ERROR_CHECK, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickBtnPGSelect_DDI_Block_Test()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_DDI_BLOCK_CHECK"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(DDI_BLOCK_TEST, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickBtnPGSelect_Force()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_FORCE1_AFT"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(FORCE_START1_AFT, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickBtnPGSelect_Force2()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_FORCE2_AFT"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(FORCE_START2_AFT, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickBtnPGSelect_Force3()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_FORCE3_AFT"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(FORCE_START3_AFT, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::SetLogListUpdate(CString strMessage)
{
	m_PGLog.InsertItem(m_PGLog.GetItemCount(), strMessage);

	int nCount = m_PGLog.GetItemCount();
	if (nCount > 0)
		m_PGLog.EnsureVisible(nCount-1, FALSE);
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxbtnTeach1FunctionForce1Bef()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_FORCE1_BEF"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(FORCE_START1_BEF, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}


//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxbtnTeach1FunctionForce2Bef()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_FORCE2_BEF"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(FORCE_START2_BEF, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}


//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxbtnTeach1FunctionForce3Bef()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_FORCE3_BEF"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(FORCE_START3_BEF, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}


//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxbtnTeach1FunctionIctCheck()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_ICT_TEST"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(ICT_TEST, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxbtnTeach1FunctionOptionCheck2()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_OPTION_CHECK2_TEST"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(OPTION_CHECK2, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxbtnTeach1FunctionOptionCheck3()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_OPTION_CHECK3_TEST"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(OPTION_CHECK3, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxbtnTeach1FunctionOptionCheck4()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_OPTION_CHECK4_TEST"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(OPTION_CHECK4, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxbtnTeach1FunctionOptionCheck5()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_OPTION_CHECK5_TEST"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(OPTION_CHECK5, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxbtnTeach1FunctionOptionCheck6()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_OPTION_CHECK6_TEST"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(OPTION_CHECK6, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxbtnTeach1FunctionOptionCheck7()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_OPTION_CHECK7_TEST"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(OPTION_CHECK7, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxbtnTeach1FunctionOptionCheck8()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_OPTION_CHECK8_TEST"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(OPTION_CHECK8, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxbtnTeach1FunctionOptionCheck9()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_OPTION_CHECK9_TEST"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(OPTION_CHECK9, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxbtnTeach1FunctionOptionCheck10()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_OPTION_CHECK10_TEST"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(OPTION_CHECK10, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20171010 신호기 UI 에 White Circle 추가
void CFormSignal::ClickGxbtnTeach1FunctionWhiteCircle()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_OPTION_WHITE_CIRCLE"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(WHITE_CIRCLE, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170911 신호기 UI MTP COMMAND 추가
void CFormSignal::ClickGxbtnTeach1FunctionMTPWrite()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_MTP_WRITE"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(MTP_WRITE_PRESCALE, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	

}

//kjpark 20170911 신호기 UI MTP COMMAND 추가
void CFormSignal::ClickGxbtnTeach1FunctionMTPVerify()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_MTP_VERIFY"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(MTP_VERIFY, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	

}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxbtnTeach1FunctionHlpmCheck()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_HLPM_CURRENT_CHECK"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(HLPM_CURRENT_CHECK, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	

}


//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxbtnTeach1FunctionWhiteCurrent()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_WHITE_CURRENT_CHECK"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(WHITE_CURRENT_CHECK, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxbtnTeach1FunctionSleepCurrent()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_SLEEP_CURRENT_CHECK"));
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(SLEEP_CURRENT_CHECK, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxstcZoneA()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SHUTTLE1 ALL"));
	BOOL bAllSelect=CheckSelectAllZonePG(JIG_ID_A);
	for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
	{
		if(bAllSelect)
		{
			m_bButtonCheck[JIG_ID_A][j] = TRUE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[JIG_ID_A][j], GXCOLOR_ON);
		}
		else
		{
			m_bButtonCheck[JIG_ID_A][j] = FALSE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[JIG_ID_A][j], GXCOLOR_OFF);
		}
	}
	ClickBtnSelect_Multi();
}

//kjpark 20170908 Setup 시그널 페이지 수정
BOOL CFormSignal::CheckSelectAllZonePG(ZONE_ID zone)
{
	BOOL bAllSelectChk=TRUE;
	for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
	{
		if(m_bButtonCheck[zone][j] == TRUE)
		{
			bAllSelectChk=FALSE;
			break;
		}
	}
	return bAllSelectChk;
}

//kjpark 20170908 Setup 시그널 페이지 수정
BOOL CFormSignal::CheckSelectAllZonePG(JIG_ID jig)
{
	BOOL bAllSelectChk=TRUE;
	for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
	{
		if(m_bButtonCheck[jig][j] == TRUE)
		{
			bAllSelectChk=FALSE;
			break;
		}
	}
	return bAllSelectChk;
}

//kjpark 20170908 Setup 시그널 페이지 수정
BOOL CFormSignal::CheckSelectAllChanelPG(JIG_CH ch)
{
	BOOL bAllSelectChk=TRUE;
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		if(m_bButtonCheck[i][ch] == TRUE)
		{
			bAllSelectChk=FALSE;
			break;
		}
	}
	return bAllSelectChk;
}

//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxstcZoneB()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SHUTTLE2 ALL"));
		BOOL bAllSelect=CheckSelectAllZonePG(JIG_ID_B);
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(bAllSelect)
			{
				m_bButtonCheck[JIG_ID_B][j] = TRUE;
				CGxUICtrl::SetButtonColor(this,m_nCellButton[JIG_ID_B][j], GXCOLOR_ON);
			}
			else
			{
				m_bButtonCheck[JIG_ID_B][j] = FALSE;
				CGxUICtrl::SetButtonColor(this,m_nCellButton[JIG_ID_B][j], GXCOLOR_OFF);
			}
		}
		ClickBtnSelect_Multi();
}


//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxstcZoneC()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SHUTTLE3 ALL"));
	//BOOL bAllSelect=CheckSelectAllZonePG(JIG_ID_C);
	//for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
	//{
	//	if(bAllSelect)
	//	{
	//		m_bButtonCheck[JIG_ID_C][j] = TRUE;
	//		CGxUICtrl::SetButtonColor(this,m_nCellButton[JIG_ID_C][j], GXCOLOR_ON);
	//	}
	//	else
	//	{
	//		m_bButtonCheck[JIG_ID_C][j] = FALSE;
	//		CGxUICtrl::SetButtonColor(this,m_nCellButton[JIG_ID_C][j], GXCOLOR_OFF);
	//	}
	//}
	ClickBtnSelect_Multi();
}


//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxstcZoneD()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("ZONE_D ALL"));
	// 	BOOL bAllSelect=CheckSelectAllZonePG(ZONE_ID_D);
	// 	for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
	// 	{
	// 		if(bAllSelect)
	// 		{
	// 			m_bButtonCheck[ZONE_ID_D][j] = TRUE;
	// 			CGxUICtrl::SetButtonColor(this,m_nCellButton[ZONE_ID_D][j], GXCOLOR_ON);
	// 		}
	// 		else
	// 		{
	// 			m_bButtonCheck[ZONE_ID_D][j] = FALSE;
	// 			CGxUICtrl::SetButtonColor(this,m_nCellButton[ZONE_ID_D][j], GXCOLOR_OFF);
	// 		}
	// 	}
	ClickBtnSelect_Multi();
}


//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxstcCh1()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("JIG_CH1 ALL"));
	BOOL bAllSelect=CheckSelectAllChanelPG(JIG_CH_1);
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		if(bAllSelect)
		{
			m_bButtonCheck[i][JIG_CH_1] = TRUE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][JIG_CH_1], GXCOLOR_ON);
		}
		else
		{
			m_bButtonCheck[i][JIG_CH_1] = FALSE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][JIG_CH_1], GXCOLOR_OFF);
		}
	}
	ClickBtnSelect_Multi();
}


//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxstcCh2()
{
	//theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("JIG_CH2 ALL"));
	//BOOL bAllSelect=CheckSelectAllChanelPG(JIG_CH_2);
	//for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	//{
	//	if(bAllSelect)
	//	{
	//		m_bButtonCheck[i][JIG_CH_2] = TRUE;
	//		CGxUICtrl::SetButtonColor(this,m_nCellButton[i][JIG_CH_2], GXCOLOR_ON);
	//	}
	//	else
	//	{
	//		m_bButtonCheck[i][JIG_CH_2] = FALSE;
	//		CGxUICtrl::SetButtonColor(this,m_nCellButton[i][JIG_CH_2], GXCOLOR_OFF);
	//	}
	//}
	//ClickBtnSelect_Multi();
}


//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxstcCh3()
{
	// 	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("JIG_CH3 ALL"));
	// 	BOOL bAllSelect=CheckSelectAllChanelPG(JIG_CH_3);
	// 	for(int i = SHUTTLE_1 ; i < JIG_ID_MAX ; i++)
	// 	{
	// 		if(bAllSelect)
	// 		{
	// 			m_bButtonCheck[i][JIG_CH_3] = TRUE;
	// 			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][JIG_CH_3], GXCOLOR_ON);
	// 		}
	// 		else
	// 		{
	// 			m_bButtonCheck[i][JIG_CH_3] = FALSE;
	// 			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][JIG_CH_3], GXCOLOR_OFF);
	// 		}
	// 	}
	// 	ClickBtnSelect_Multi();
}


//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxstcCh4()
{
	// 	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("JIG_CH4 ALL"));
	// 	BOOL bAllSelect=CheckSelectAllChanelPG(JIG_CH_4);
	// 	for(int i = SHUTTLE_1 ; i < JIG_ID_MAX ; i++)
	// 	{
	// 		if(bAllSelect)
	// 		{
	// 			m_bButtonCheck[i][JIG_CH_4] = TRUE;
	// 			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][JIG_CH_4], GXCOLOR_ON);
	// 		}
	// 		else
	// 		{
	// 			m_bButtonCheck[i][JIG_CH_4] = FALSE;
	// 			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][JIG_CH_4], GXCOLOR_OFF);
	// 		}
	// 	}
	// 	ClickBtnSelect_Multi();
}


//kjpark 20170908 Setup 시그널 페이지 수정
void CFormSignal::ClickGxstcCh5()
{
	// 	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("JIG_CH5 ALL"));
	// 	BOOL bAllSelect=CheckSelectAllChanelPG(JIG_CH_5);
	// 	for(int i = SHUTTLE_1 ; i < JIG_ID_MAX ; i++)
	// 	{
	// 		if(bAllSelect)
	// 		{
	// 			m_bButtonCheck[i][JIG_CH_5] = TRUE;
	// 			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][JIG_CH_5], GXCOLOR_ON);
	// 		}
	// 		else
	// 		{
	// 			m_bButtonCheck[i][JIG_CH_5] = FALSE;
	// 			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][JIG_CH_5], GXCOLOR_OFF);
	// 		}
	// 	}
	// 	ClickBtnSelect_Multi();
}
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef NOT_USED
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnPGSelect_ON()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PATTERN_TURN_ON"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(TURN_ON, (JIG_ID)i,(JIG_CH)j);
				//theUnitFunc.SendPGMessage(TURN_ON, (ZONE_ID)i, (JIG_CH)j);
			}
		}
	}	
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnPGSelect_OFF()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PATTERN_TURN_OFF"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(TURN_OFF, (JIG_ID)i,(JIG_CH)j);
				//theUnitFunc.SendPGMessage(TURN_OFF, (ZONE_ID)i, (JIG_CH)j);
			}
		}
	}	
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnPGSelect_PREV()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PATTERN_PREV"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(BACK, (JIG_ID)i,(JIG_CH)j);
				//theUnitFunc.SendPGMessage(BACK, (ZONE_ID)i, (JIG_CH)j);
			}
		}
	}	
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnPGSelect_NEXT()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PATTERN_NEXT"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(NEXT, (JIG_ID)i,(JIG_CH)j);
				//theUnitFunc.SendPGMessage(NEXT, (ZONE_ID)i, (JIG_CH)j);
			}
		}
	}	
}

//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnPGSelect_Reset()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PATTERN_RESET"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(RESET, (JIG_ID)i,(JIG_CH)j);
				//theUnitFunc.SendPGMessage(RESET, (ZONE_ID)i, (JIG_CH)j);
			}
		}
	}	
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnPGSelect_CellLoading()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PATTERN_CellLoading"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(CELL_LOADING, (JIG_ID)i,(JIG_CH)j);
				//theUnitFunc.SendPGMessage(CELL_LOADING, (ZONE_ID)i, (JIG_CH)j);
			}
		}
	}	
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnPGSelect_TMDInfo()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("PATTERN_TMDInfo"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				//theUnitFunc.SendPGMessage(TMD_INFO, (ZONE_ID)i, (JIG_CH)j);
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(TMD_INFO, (ZONE_ID)i, (JIG_CH)j);
			}
		}
	}	
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnPGSelect_TSPStart()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_TSP_START"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(TSP_START, (JIG_ID)i,(JIG_CH)j);
				//theUnitFunc.SendPGMessage(TSP_START, (ZONE_ID)i, (JIG_CH)j);
			}
		}
	}		
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnPGSelect_IDCheck()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_ID_CHACK"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(ID_CHECK, (JIG_ID)i,(JIG_CH)j);
				//theUnitFunc.SendPGMessage(ID_CHECK, (ZONE_ID)i, (JIG_CH)j);
			}
		}
	}	
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::ClickBtnPGSelect_OTPREG()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_OTP_REG"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(OTP_REG_CHECK, (JIG_ID)i,(JIG_CH)j);
				//theUnitFunc.SendPGMessage(OTP_REG_CHECK, (ZONE_ID)i, (JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickBtnPGSelect_EVT_Version()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_EVT_VERSION"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(EVT_VERSION_CHECK, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickBtnPGSelect_TECheck()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_TE_CHECK"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(TE_CHECK, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickBtnPGSelect_COPR_ICT()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_COPR_ICT_CHECK"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(COPR_ICT_TEST, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickBtnPGSelect_POC_Error()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_POC_ERROR_CHECK"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(POC_ERROR_CHECK, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickBtnPGSelect_DDI_Block_Test()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_DDI_BLOCK_CHECK"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(DDI_BLOCK_TEST, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20161114 C Zone Foce 추가
void CFormSignal::ClickBtnPGSelect_Force()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_FORCE1_AFT"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(FORCE_START1_AFT, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20161114 C Zone Foce 추가
void CFormSignal::ClickBtnPGSelect_Force2()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_FORCE2_AFT"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(FORCE_START2_AFT, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20161114 C Zone Foce 추가
void CFormSignal::ClickBtnPGSelect_Force3()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_FORCE3_AFT"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(FORCE_START3_AFT, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::SetLogListUpdate(CString strMessage)
{
	m_PGLog.InsertItem(m_PGLog.GetItemCount(), strMessage);

	int nCount = m_PGLog.GetItemCount();
	if (nCount > 0)
		m_PGLog.EnsureVisible(nCount-1, FALSE);
}

void CFormSignal::ClickGxbtnTeach1FunctionForce1Bef()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_FORCE1_BEF"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(FORCE_START1_BEF, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}


void CFormSignal::ClickGxbtnTeach1FunctionForce2Bef()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_FORCE2_BEF"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(FORCE_START2_BEF, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}


void CFormSignal::ClickGxbtnTeach1FunctionForce3Bef()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_FORCE3_BEF"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(FORCE_START3_BEF, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}


void CFormSignal::ClickGxbtnTeach1FunctionIctCheck()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_ICT_TEST"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(ICT_TEST, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickGxbtnTeach1FunctionOptionCheck2()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_OPTION_CHECK2_TEST"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(OPTION_CHECK2, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickGxbtnTeach1FunctionOptionCheck3()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_OPTION_CHECK3_TEST"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(OPTION_CHECK3, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickGxbtnTeach1FunctionOptionCheck4()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_OPTION_CHECK4_TEST"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(OPTION_CHECK4, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickGxbtnTeach1FunctionOptionCheck5()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_OPTION_CHECK5_TEST"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(OPTION_CHECK5, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickGxbtnTeach1FunctionOptionCheck6()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_OPTION_CHECK6_TEST"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(OPTION_CHECK6, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickGxbtnTeach1FunctionOptionCheck7()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_OPTION_CHECK7_TEST"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(OPTION_CHECK7, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickGxbtnTeach1FunctionOptionCheck8()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_OPTION_CHECK8_TEST"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(OPTION_CHECK8, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickGxbtnTeach1FunctionOptionCheck9()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_OPTION_CHECK9_TEST"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(OPTION_CHECK9, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickGxbtnTeach1FunctionOptionCheck10()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_OPTION_CHECK10_TEST"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(OPTION_CHECK10, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickGxbtnTeach1FunctionHlpmCheck()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_HLPM_CURRENT_CHECK"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(HLPM_CURRENT_CHECK, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	

}


void CFormSignal::ClickGxbtnTeach1FunctionWhiteCurrent()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_WHITE_CURRENT_CHECK"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(WHITE_CURRENT_CHECK, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickGxbtnTeach1FunctionSleepCurrent()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_SLEEP_CURRENT_CHECK"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(SLEEP_CURRENT_CHECK, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickGxbtnTeach1FunctionWhitePattern()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_WHITE_PATTERN_CHECK"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(WHITE_PATTERN, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickGxbtnTeach1FunctionCrossPattern()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SPECIAL_CROSS_PATTERN_CHECK"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(CROSS_PATTERN, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickGxstcZoneA()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("ZONE_A ALL"));
	BOOL bAllSelect=CheckSelectAllZonePG(ZONE_ID_A);
	for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
	{
		if(bAllSelect)
		{
			m_bButtonCheck[ZONE_ID_A][j] = TRUE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[ZONE_ID_A][j], GXCOLOR_ON);
		}
		else
		{
			m_bButtonCheck[ZONE_ID_A][j] = FALSE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[ZONE_ID_A][j], GXCOLOR_OFF);
		}
	}
	ClickBtnSelect_Multi();
}

BOOL CFormSignal::CheckSelectAllZonePG(ZONE_ID zone)
{
	BOOL bAllSelectChk=TRUE;
	for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
	{
		if(m_bButtonCheck[zone][j] == TRUE)
		{
			bAllSelectChk=FALSE;
			break;
		}
	}
	return bAllSelectChk;
}


BOOL CFormSignal::CheckSelectAllChanelPG(JIG_CH ch)
{
	BOOL bAllSelectChk=TRUE;
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		if(m_bButtonCheck[i][ch] == TRUE)
		{
			bAllSelectChk=FALSE;
			break;
		}
	}
	return bAllSelectChk;
}

void CFormSignal::ClickGxstcZoneB()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("ZONE_B ALL"));
// 	BOOL bAllSelect=CheckSelectAllZonePG(ZONE_ID_B);
// 	for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
// 	{
// 		if(bAllSelect)
// 		{
// 			m_bButtonCheck[ZONE_ID_B][j] = TRUE;
// 			CGxUICtrl::SetButtonColor(this,m_nCellButton[ZONE_ID_B][j], GXCOLOR_ON);
// 		}
// 		else
// 		{
// 			m_bButtonCheck[ZONE_ID_B][j] = FALSE;
// 			CGxUICtrl::SetButtonColor(this,m_nCellButton[ZONE_ID_B][j], GXCOLOR_OFF);
// 		}
// 	}
	ClickBtnSelect_Multi();
}


void CFormSignal::ClickGxstcZoneC()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("ZONE_C ALL"));
	BOOL bAllSelect=CheckSelectAllZonePG(ZONE_ID_B);
	for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
	{
		if(bAllSelect)
		{
			m_bButtonCheck[ZONE_ID_B][j] = TRUE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[ZONE_ID_B][j], GXCOLOR_ON);
		}
		else
		{
			m_bButtonCheck[ZONE_ID_B][j] = FALSE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[ZONE_ID_B][j], GXCOLOR_OFF);
		}
	}
	ClickBtnSelect_Multi();
}


void CFormSignal::ClickGxstcZoneD()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("ZONE_D ALL"));
// 	BOOL bAllSelect=CheckSelectAllZonePG(ZONE_ID_D);
// 	for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
// 	{
// 		if(bAllSelect)
// 		{
// 			m_bButtonCheck[ZONE_ID_D][j] = TRUE;
// 			CGxUICtrl::SetButtonColor(this,m_nCellButton[ZONE_ID_D][j], GXCOLOR_ON);
// 		}
// 		else
// 		{
// 			m_bButtonCheck[ZONE_ID_D][j] = FALSE;
// 			CGxUICtrl::SetButtonColor(this,m_nCellButton[ZONE_ID_D][j], GXCOLOR_OFF);
// 		}
// 	}
	ClickBtnSelect_Multi();
}


void CFormSignal::ClickGxstcCh1()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("JIG_CH1 ALL"));
	BOOL bAllSelect=CheckSelectAllChanelPG(JIG_CH_1);
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		if(bAllSelect)
		{
			m_bButtonCheck[i][JIG_CH_1] = TRUE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][JIG_CH_1], GXCOLOR_ON);
		}
		else
		{
			m_bButtonCheck[i][JIG_CH_1] = FALSE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][JIG_CH_1], GXCOLOR_OFF);
		}
	}
	ClickBtnSelect_Multi();
}


void CFormSignal::ClickGxstcCh2()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("JIG_CH2 ALL"));
	BOOL bAllSelect=CheckSelectAllChanelPG(JIG_CH_2);
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		if(bAllSelect)
		{
			m_bButtonCheck[i][JIG_CH_2] = TRUE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][JIG_CH_2], GXCOLOR_ON);
		}
		else
		{
			m_bButtonCheck[i][JIG_CH_2] = FALSE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][JIG_CH_2], GXCOLOR_OFF);
		}
	}
	ClickBtnSelect_Multi();
}


void CFormSignal::ClickGxstcCh3()
{
// 	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("JIG_CH3 ALL"));
// 	BOOL bAllSelect=CheckSelectAllChanelPG(JIG_CH_3);
// 	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
// 	{
// 		if(bAllSelect)
// 		{
// 			m_bButtonCheck[i][JIG_CH_3] = TRUE;
// 			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][JIG_CH_3], GXCOLOR_ON);
// 		}
// 		else
// 		{
// 			m_bButtonCheck[i][JIG_CH_3] = FALSE;
// 			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][JIG_CH_3], GXCOLOR_OFF);
// 		}
// 	}
// 	ClickBtnSelect_Multi();
}


void CFormSignal::ClickGxstcCh4()
{
// 	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("JIG_CH4 ALL"));
// 	BOOL bAllSelect=CheckSelectAllChanelPG(JIG_CH_4);
// 	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
// 	{
// 		if(bAllSelect)
// 		{
// 			m_bButtonCheck[i][JIG_CH_4] = TRUE;
// 			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][JIG_CH_4], GXCOLOR_ON);
// 		}
// 		else
// 		{
// 			m_bButtonCheck[i][JIG_CH_4] = FALSE;
// 			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][JIG_CH_4], GXCOLOR_OFF);
// 		}
// 	}
// 	ClickBtnSelect_Multi();
}


void CFormSignal::ClickGxstcCh5()
{
// 	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("JIG_CH5 ALL"));
// 	BOOL bAllSelect=CheckSelectAllChanelPG(JIG_CH_5);
// 	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
// 	{
// 		if(bAllSelect)
// 		{
// 			m_bButtonCheck[i][JIG_CH_5] = TRUE;
// 			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][JIG_CH_5], GXCOLOR_ON);
// 		}
// 		else
// 		{
// 			m_bButtonCheck[i][JIG_CH_5] = FALSE;
// 			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][JIG_CH_5], GXCOLOR_OFF);
// 		}
// 	}
// 	ClickBtnSelect_Multi();
}
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////






void CFormSignal::ClickGxlblPatternNo()
{
	
	CGxNumPadEx	dlgNumPad;
	int dOldValue = m_nPatternNo;
	dlgNumPad.SetTitle(_T("D Zone Show Pattern Change"));
	dlgNumPad.SetValue(0, 9999, dOldValue);
	if ( dlgNumPad.DoModal() == IDOK )
	{
		//int findOffset=0;
		m_nPatternNo = _tstoi(dlgNumPad.GetValue());
		//변경된 설정값을 임시 저장
		CGxUICtrl::SetStaticString(this, IDC_GXLBL_PATTERN_NO, dlgNumPad.GetValue());
		
		
	}
}


void CFormSignal::ClickGxbtnDzonePatternShow()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("DRAW_DZONE_PATTERN_SHOW"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.ShowDZonePattern(m_nPatternNo,(JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}


void CFormSignal::ClickGxlblSquareThickness()
{
	CGxNumPadEx	dlgNumPad;
	int dOldValue = m_nSquareThickness;
	dlgNumPad.SetTitle(_T("Draw Square Thickness Change"));
	dlgNumPad.SetValue(0, 999, dOldValue);
	if ( dlgNumPad.DoModal() == IDOK )
	{
		//int findOffset=0;
		m_nSquareThickness = _tstoi(dlgNumPad.GetValue());
		//변경된 설정값을 임시 저장
		CGxUICtrl::SetStaticString(this, IDC_GXLBL_SQUARE_THICKNESS, dlgNumPad.GetValue());
	}
}


void CFormSignal::ClickGxlblSquareSize()
{
	CGxNumPadEx	dlgNumPad;
	int dOldValue = m_nSquareSize;
	dlgNumPad.SetTitle(_T("Draw Square Size Change"));
	dlgNumPad.SetValue(0, 999, dOldValue);
	if ( dlgNumPad.DoModal() == IDOK )
	{
		//int findOffset=0;
		m_nSquareSize = _tstoi(dlgNumPad.GetValue());
		//변경된 설정값을 임시 저장
		CGxUICtrl::SetStaticString(this, IDC_GXLBL_SQUARE_SIZE, dlgNumPad.GetValue());
	}
}


void CFormSignal::ClickGxlblSquareColorline()
{
	CGxNumPadEx	dlgNumPad;
	int dOldValue = m_nSquareColorLineRed;
	dlgNumPad.SetTitle(_T("Draw Square ColorLine Red Change"));
	dlgNumPad.SetValue(0, 255, dOldValue, FALSE);
	if ( dlgNumPad.DoModal() == IDOK )
	{
		//int findOffset=0;
		m_nSquareColorLineRed = _tstoi(dlgNumPad.GetValue());
		//변경된 설정값을 임시 저장
		
	}
	dOldValue = m_nSquareColorLineGreen;
	dlgNumPad.SetTitle(_T("Draw Square ColorLine Green Change"));
	dlgNumPad.SetValue(0, 255, dOldValue, FALSE);
	if ( dlgNumPad.DoModal() == IDOK )
	{
		//int findOffset=0;
		m_nSquareColorLineGreen = _tstoi(dlgNumPad.GetValue());
		//변경된 설정값을 임시 저장

	}
	dOldValue = m_nSquareColorLineBlue;
	dlgNumPad.SetTitle(_T("Draw Square ColorLine Blue Change"));
	dlgNumPad.SetValue(0, 255, dOldValue, FALSE);
	if ( dlgNumPad.DoModal() == IDOK )
	{
		//int findOffset=0;
		m_nSquareColorLineBlue = _tstoi(dlgNumPad.GetValue());
		//변경된 설정값을 임시 저장

	}
	CString strValue;
	strValue.Format(_T("%d,%d,%d"),m_nSquareColorLineRed,m_nSquareColorLineGreen,m_nSquareColorLineBlue);
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_SQUARE_COLORLINE,strValue);
}


void CFormSignal::ClickGxlblSquareX()
{
	CGxNumPadEx	dlgNumPad;
//	int dOldValue = m_nSquareXPoint;
	dlgNumPad.SetTitle(_T("Draw Square X Point Change"));
//	dlgNumPad.SetValue(0, 999, dOldValue);
	if ( dlgNumPad.DoModal() == IDOK )
	{
		//int findOffset=0;
		m_nSquareXPoint = _tstoi(dlgNumPad.GetValue());
		//변경된 설정값을 임시 저장
		CGxUICtrl::SetStaticString(this, IDC_GXLBL_SQUARE_X, dlgNumPad.GetValue());
	}
}


void CFormSignal::ClickGxlblSquareY()
{
	CGxNumPadEx	dlgNumPad;
//	int dOldValue = m_nSquareYPoint;
	dlgNumPad.SetTitle(_T("Draw Square Y Point Change"));
//	dlgNumPad.SetValue(0, 999, dOldValue);
	if ( dlgNumPad.DoModal() == IDOK )
	{
		//int findOffset=0;
		m_nSquareYPoint = _tstoi(dlgNumPad.GetValue());
		//변경된 설정값을 임시 저장
		CGxUICtrl::SetStaticString(this, IDC_GXLBL_SQUARE_Y, dlgNumPad.GetValue());
	}
}


void CFormSignal::ClickGxbtnSquareAdd()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("DRAW_SQUARE_ADD"));
	SQUARE_POINT sqpoint;
	sqpoint.nThickness=m_nSquareThickness;
	sqpoint.nSize=m_nSquareSize;
	sqpoint.nX=m_nSquareXPoint;
	sqpoint.nY=m_nSquareYPoint;
	sqpoint.colorLine=RGB(m_nSquareColorLineRed,m_nSquareColorLineGreen, m_nSquareColorLineBlue);
	theSocketInterFace.m_PGHost.AddPGSquare(sqpoint);
	
}


void CFormSignal::ClickGxbtnSquareDraw()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("DRAW_SQUARE_DRAW"));
	int nCount = theSocketInterFace.m_PGHost.m_listSquare.GetCount();
	if(nCount<1)//20170504 BKH, 만약 SquareAdd버튼을 누르지 않은 상태라면 기본값으로 Add를 먼저 시킨후 Draw를 수행한다.
	{
		ClickGxbtnSquareAdd();
	}
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.DrawPGSquare((JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
	theSocketInterFace.m_PGHost.ClearPGSqueare();
}


void CFormSignal::ClickGxlblMsgBackcolor()
{
	CGxNumPadEx	dlgNumPad;
	int dOldValue = m_nMSGBackColorRed;
	dlgNumPad.SetTitle(_T("Draw MSG BackColor Red Change"));
	dlgNumPad.SetValue(0, 255, dOldValue, FALSE);
	if ( dlgNumPad.DoModal() == IDOK )
	{
		//int findOffset=0;
		m_nMSGBackColorRed = _tstoi(dlgNumPad.GetValue());
		//변경된 설정값을 임시 저장

	}
	dOldValue = m_nMSGBackColorGreen;
	dlgNumPad.SetTitle(_T("Draw MSG BackColor Green Change"));
	dlgNumPad.SetValue(0, 255, dOldValue, FALSE);
	if ( dlgNumPad.DoModal() == IDOK )
	{
		//int findOffset=0;
		m_nMSGBackColorGreen = _tstoi(dlgNumPad.GetValue());
		//변경된 설정값을 임시 저장

	}
	dOldValue = m_nMSGBackColorBlue;
	dlgNumPad.SetTitle(_T("Draw MSG BackColor Blue Change"));
	dlgNumPad.SetValue(0, 255, dOldValue, FALSE);
	if ( dlgNumPad.DoModal() == IDOK )
	{
		//int findOffset=0;
		m_nMSGBackColorBlue = _tstoi(dlgNumPad.GetValue());
		//변경된 설정값을 임시 저장

	}
	CString strValue;
	strValue.Format(_T("%d,%d,%d"),m_nMSGBackColorRed,m_nMSGBackColorGreen,m_nMSGBackColorBlue);
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_MSG_BACKCOLOR,strValue);
}


void CFormSignal::ClickGxlblMsgFontcolor()
{
	CGxNumPadEx	dlgNumPad;
	int dOldValue = m_nMSGFontColorRed;
	dlgNumPad.SetTitle(_T("Draw MSG FontColor Red Change"));
	dlgNumPad.SetValue(0, 255, dOldValue, FALSE);
	if ( dlgNumPad.DoModal() == IDOK )
	{
		//int findOffset=0;
		m_nMSGFontColorRed = _tstoi(dlgNumPad.GetValue());
		//변경된 설정값을 임시 저장

	}
	dOldValue = m_nMSGFontColorGreen;
	dlgNumPad.SetTitle(_T("Draw MSG FontColor Green Change"));
	dlgNumPad.SetValue(0, 255, dOldValue, FALSE);
	if ( dlgNumPad.DoModal() == IDOK )
	{
		//int findOffset=0;
		m_nMSGFontColorGreen = _tstoi(dlgNumPad.GetValue());
		//변경된 설정값을 임시 저장

	}
	dOldValue = m_nMSGFontColorBlue;
	dlgNumPad.SetTitle(_T("Draw MSG FontColor Blue Change"));
	dlgNumPad.SetValue(0, 255, dOldValue, FALSE);
	if ( dlgNumPad.DoModal() == IDOK )
	{
		//int findOffset=0;
		m_nMSGFontColorBlue = _tstoi(dlgNumPad.GetValue());
		//변경된 설정값을 임시 저장

	}
	CString strValue;
	strValue.Format(_T("%d,%d,%d"),m_nMSGFontColorRed,m_nMSGFontColorGreen,m_nMSGFontColorBlue);
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_MSG_FONTCOLOR,strValue);
}


void CFormSignal::ClickGxlblMsgX()
{
	CGxNumPadEx	dlgNumPad;
//	int dOldValue = m_nMSGXPoint;
	dlgNumPad.SetTitle(_T("Draw MSG X Point Change"));
//	dlgNumPad.SetValue(0, 999, dOldValue);
	if ( dlgNumPad.DoModal() == IDOK )
	{
		//int findOffset=0;
		m_nMSGXPoint = _tstoi(dlgNumPad.GetValue());
		//변경된 설정값을 임시 저장
		CGxUICtrl::SetStaticString(this, IDC_GXLBL_MSG_X, dlgNumPad.GetValue());
	}
}


void CFormSignal::ClickGxlblMsgY()
{
	CGxNumPadEx	dlgNumPad;
//	int dOldValue = m_nMSGYPoint;
	dlgNumPad.SetTitle(_T("Draw MSG Y Point Change"));
//	dlgNumPad.SetValue(0, 999, dOldValue);
	if ( dlgNumPad.DoModal() == IDOK )
	{
		//int findOffset=0;
		m_nMSGYPoint = _tstoi(dlgNumPad.GetValue());
		//변경된 설정값을 임시 저장
		CGxUICtrl::SetStaticString(this, IDC_GXLBL_MSG_Y, dlgNumPad.GetValue());
	}
}


void CFormSignal::ClickGxlblMSGFontsize()
{
	CGxNumPadEx	dlgNumPad;
	int dOldValue = m_nMSGFontSize;
	dlgNumPad.SetTitle(_T("Draw MSG FontSize Change"));
	dlgNumPad.SetValue(0, 999, dOldValue);
	if ( dlgNumPad.DoModal() == IDOK )
	{
		//int findOffset=0;
		m_nMSGFontSize = _tstoi(dlgNumPad.GetValue());
		//변경된 설정값을 임시 저장
		CGxUICtrl::SetStaticString(this, IDC_GXLBL_MSG_FONTSIZE, dlgNumPad.GetValue());
	}
}


void CFormSignal::ClickGxbtnMsgAdd()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("DRAW_MSG_ADD"));
	CDlgBoxKeyBoard dlgKey;

	if ( dlgKey.DoModal() == IDOK )
	{
		m_strMSG=dlgKey.m_strReturnText;
		theSocketInterFace.m_PGHost.AddPGMsg(m_strMSG);
	}
}



void CFormSignal::ClickGxbtnMsgDraw()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("DRAW_MSG_DRAW"));
	for(int i = ZONE_ID_A ; i < ZONE_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.m_PGHost.DrawPGMsg((JIG_ID)i,(JIG_CH)j,RGB(m_nMSGBackColorRed,m_nMSGBackColorGreen,m_nMSGBackColorBlue),RGB(m_nMSGFontColorRed,m_nMSGFontColorGreen,m_nMSGFontColorBlue),m_nMSGXPoint,m_nMSGYPoint,m_nMSGFontSize);
			}
		}
	}
	theSocketInterFace.m_PGHost.ClearPGMsg();
}


void CFormSignal::ClickGxbtnTeach1FunctionOptionSignalPowerReset()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("SIGNAL RESET"));

	if(m_bButtonCheck[JIG_ID_A][JIG_CH_1])
	{
		theUnitFunc.SetOutPutIO(Y_SIGNAL_PC1_RESET, ON);
	}
	if(m_bButtonCheck[JIG_ID_B][JIG_CH_1])
	{
		theUnitFunc.SetOutPutIO(Y_SIGNAL_PC2_RESET, ON);
	}
		
	Sleep(1000);


	if(m_bButtonCheck[JIG_ID_A][JIG_CH_1])
	{
		theUnitFunc.SetOutPutIO(Y_SIGNAL_PC1_RESET, OFF);
	}
	if(m_bButtonCheck[JIG_ID_B][JIG_CH_1])
	{
		theUnitFunc.SetOutPutIO(Y_SIGNAL_PC2_RESET, OFF);
	}

}
