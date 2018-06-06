// FormMain.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormMain.h"

#include "GUIDefine.h"
#include "MainFrm.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"

// DLL에 존재하는 각종 Dialog 사용
#include "UI\GausGUI\GxNumPad.h"
#include "UI\GausGUI\GxNumPadEx.h"
#include "UI\GausGUI\GxMsgBox.h"

#include "IbisApp.h"
#include "UI\Dialog\DlgUserLogin.h"
#include "UI\Dialog\DlgbarBottom.h"
#include "UI\Dialog\DlgMCRManualRead.h"
#include "UI\Dialog\DlgOtherDefect.h"
// kjpark 20171016 계측기 코릴레이션 하강 / 원복 버튼
// #include "UI\Dialog\DlgCoRelation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


enum
{
	TIMER_MONITOR = 0,
	TIMER_STEP
};

// CFormMain

IMPLEMENT_DYNCREATE(CFormMain, CFormView)

CFormMain::CFormMain()
	: CFormView(CFormMain::IDD),
	m_StateCorelation(STATE_CORELATION, MP2100_THREAD_MAIN_PROCESS)
{
	m_bFormInit = FALSE;

	m_bCH1lSelect = FALSE;
	m_bCH2lSelect = FALSE;
	m_bCH3lSelect = FALSE;
	m_bCH4lSelect = FALSE;
	m_bCH5lSelect = FALSE;
	m_bCH6lSelect = FALSE;
}

CFormMain::~CFormMain()
{
}

void CFormMain::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormMain, CFormView)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(UDMSG_COUNT_UPDATE,			OnCountUpdate)			// MCR이 수신되었다.
	ON_MESSAGE(WM_UPDATE_MCRDATA_CH1,		OnUpdateMCRData_CH1)			// MCR이 수신되었다.
	ON_MESSAGE(WM_UPDATE_MCRDATA_CH2,		OnUpdateMCRData_CH3)			// MCR이 수신되었다.
	ON_WM_TIMER()

END_MESSAGE_MAP()									  

BEGIN_EVENTSINK_MAP(CFormMain, CFormView)
	ON_EVENT(CFormMain, IDC_GXBTN_MODE_PM, DISPID_CLICK, CFormMain::ClickBtnModePM, VTS_NONE)
	
	ON_EVENT(CFormMain, IDC_GXBTN_STOP, DISPID_CLICK, CFormMain::ClickGxbtnStop, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXBTN_START, DISPID_CLICK, CFormMain::ClickGxbtnStart, VTS_NONE)
	//kjpark 20171016 계측기 코릴레이션 하강 / 원복 버튼
	ON_EVENT(CFormMain, IDC_GXSTC_MAIN_CO_RELATION, DISPID_CLICK, CFormMain::ClickGxbtnCorelation, VTS_NONE)	
	ON_EVENT(CFormMain, IDC_GXBTN_MAIN_CH1_NOT_USE, DISPID_CLICK, CFormMain::ClickGxbtnMainCh1NotUse, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXBTN_MAIN_CH1_SKIP, DISPID_CLICK, CFormMain::ClickGxbtnMainCh1Skip, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXBTN_MAIN_CH2_NOT_USE, DISPID_CLICK, CFormMain::ClickGxbtnMainCh3NotUse, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXBTN_MAIN_CH2_SKIP, DISPID_CLICK, CFormMain::ClickGxbtnMainCh3Skip, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_MAIN_COUNT_CLEAR, DISPID_CLICK, CFormMain::ClickGxstcMainCountClear, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXBTN_PRODUCT_END, DISPID_CLICK, CFormMain::ClickGxbtnProductEnd, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_MAIN_CH1, DISPID_CLICK, CFormMain::ClickGxstcMainCh1, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_MAIN_CH2, DISPID_CLICK, CFormMain::ClickGxstcMainCh3, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXBTN_CREATECELLINFO, DISPID_CLICK, CFormMain::ClickGxbtnCreatecellinfo, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXBTN_LOADING_STOP, DISPID_CLICK, CFormMain::ClickGxbtnLoadingStop, VTS_NONE)
END_EVENTSINK_MAP()

// CFormMain 진단입니다.

#ifdef _DEBUG
void CFormMain::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormMain::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG



// CFormMain 메시지 처리기입니다.

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
LRESULT CFormMain::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		if(m_bFormInit == FALSE)
		{
			AZoneSkiipButton_Init();
			CellStatus_Init();
			OptionMonitor_Init();
			if(theConfigBank.m_System.m_bInlineMode)
			{
				CGxUICtrl::SetStaticVisible(this, IDC_GXSTC_MAIN_INLINE_MODE, TRUE);
			}
			else
			{
				CGxUICtrl::SetStaticVisible(this, IDC_GXSTC_MAIN_INLINE_MODE, FALSE);
			}
			m_bFormInit = TRUE;
		}

		UpdateCellCreateButton();
		OptionMonitor_Update();
		::PostMessage(m_hWnd, UDMSG_COUNT_UPDATE, NULL, NULL);
		SetTimer(TIMER_MONITOR, 200, NULL);
	}
	else
	{
		KillTimer(TIMER_MONITOR);
		KillTimer(TIMER_STEP);
	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CFormMain::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_GXSTC_MAIN_CH1_CELL_LOADING);
	
	return 0;
}


LRESULT CFormMain::OnCountUpdate(WPARAM wParam, LPARAM lParam)
{
	int nTotal;
	double dMCRRate, dCONTACTRate;
	CString strMCR, strCONTACT;
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		for(int ch = 0; ch < JIG_CH_MAX; ch++)
		{
			CGxUICtrl::SetStaticInt(this, theProductBank.m_MainCountGOOD[jig][ch].uID, theProductBank.m_MainCountGOOD[jig][ch].iValue);
 			CGxUICtrl::SetStaticInt(this, theProductBank.m_MainCountNG[jig][ch].uID, theProductBank.m_MainCountNG[jig][ch].iValue);

 			nTotal = theProductBank.m_MainCountGOOD[jig][ch].iValue + theProductBank.m_MainCountNG[jig][ch].iValue;
 			dMCRRate = 0;
			dCONTACTRate = 0;
 			if(nTotal > 0)
			{
 				dMCRRate = ((double)theProductBank.m_MainCountMCR[jig][ch].iValue / (double)nTotal) * 100.;
				dCONTACTRate = ((double)theProductBank.m_MainCountCONTACT[jig][ch].iValue / (double)nTotal) * 100.;
			}
			strMCR.Format(_T("%d (%.1f%%)"), theProductBank.m_MainCountMCR[jig][ch].iValue, dMCRRate);
 			CGxUICtrl::SetStaticString(this, theProductBank.m_MainCountMCR[jig][ch].uID, strMCR);

			strCONTACT.Format(_T("%d (%.1f%%)"), theProductBank.m_MainCountCONTACT[jig][ch].iValue, dCONTACTRate);
			CGxUICtrl::SetStaticString(this, theProductBank.m_MainCountCONTACT[jig][ch].uID, strCONTACT);
		}
	}
	CTime time = theProductBank.m_timeMainCount;
	CString strTime;
	strTime.Format(_T("%02d-%02d %02d:%02d:%02d"), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_CLEAR_TIME, strTime);
	return 0;
}

LRESULT CFormMain::OnUpdateMCRData_CH1( WPARAM wParam, LPARAM lParam )
{
// 	m_imgLive_CH1.LoadImage(theUnitFunc.GetLastSavedImage(JIG_ID_A, JIG_CH_1));
	m_imgLive_CH1.LoadImage(MCRPath_CH1);
	m_imgLive_CH1.Invalidate();
	return 0;
}

LRESULT CFormMain::OnUpdateMCRData_CH3( WPARAM wParam, LPARAM lParam )
{
// 	m_imgLive_CH3.LoadImage(theUnitFunc.GetLastSavedImage(JIG_ID_B, JIG_CH_1));
	m_imgLive_CH3.LoadImage(MCRPath_CH3);
	m_imgLive_CH3.Invalidate();
	return 0;
}


// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormMain::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}


void CFormMain::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);

	BOOL bContinue = FALSE;
	switch(nIDEvent)
	{
	case TIMER_MONITOR:
		AZoneNotUseButton_Update();
		CellStatus_Update();
		PMButton_UpdateState();
		MoniStopStaticStatus();

		SetTimer(TIMER_MONITOR, 100, NULL);
		break;
	case TIMER_STEP:
		m_StateCorelation.Run();
		if(m_StateCorelation.IsStoped() == FALSE)
		{
			SetTimer(nIDEvent, 100, NULL);
		}
		break;
	}
	
	CFormView::OnTimer(nIDEvent);
}

void CFormMain::PMButton_UpdateState()
{
	BOOL bNotRun = TRUE;
	if(theProcBank.MachineIsRunState())
	{
		bNotRun = FALSE;
	}
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_MODE_PM, bNotRun);
}


void CFormMain::MoniStopStaticStatus()
{
	if(theProcBank.LoadingStop_IsRequire())
	{
		if(theSocketInterFace.m_CIM.FunctionChangeBuffer.bChangeFlag)
		{
			CGxUICtrl::SetStaticVisible(this, IDC_GXSTC_MAIN_INTERLOCK_MODE, TRUE);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_MAIN_INTERLOCK_MODE, _T("FUNCTION CHANGE"));
		}
		else
		{
			CGxUICtrl::SetStaticVisible(this, IDC_GXSTC_MAIN_INTERLOCK_MODE, TRUE);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_MAIN_INTERLOCK_MODE, _T("LOADING STOP"));
		}
	}
	else if(theProcBank.PreUnitInterlock_IsEmpty(JIG_ID_MAX) == FALSE)
	{
		if(theConfigBank.m_CIM.INTERLOCK_CONTROL_TransferCheck())
		{
			CGxUICtrl::SetStaticVisible(this, IDC_GXSTC_MAIN_INTERLOCK_MODE, TRUE);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_MAIN_INTERLOCK_MODE, _T("TRANSFER STOP"));
		}
		else
		{
			CGxUICtrl::SetStaticVisible(this, IDC_GXSTC_MAIN_INTERLOCK_MODE, FALSE);
		}
	}
	else
	{
		CGxUICtrl::SetStaticVisible(this, IDC_GXSTC_MAIN_INTERLOCK_MODE, FALSE);
	}
}


// 지정 화면으로 폼을 변경한다.
void CFormMain::ChangeForm(int nIdx, BOOL bForce)
{
	// CMainFrame에게 Message로 호면 전환을 지시한다.
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)nIdx, (LPARAM)bForce);
}

void CFormMain::AZoneCellNGOnOff( JIG_ID jig, JIG_CH ch, BOOL bAZoneDefectCheck /*= FALSE*/ )
{
	CCellInfo *pCell = theCellBank.GetCellInfo(jig, ch);
	if(pCell)
	{
		if(bAZoneDefectCheck)
		{
			if( theProcBank.AZoneCellNG_Check(jig, ch) && (pCell->m_AZone.m_Class != GOOD_CELL) )
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			else
			{
				pCell->defaultData.m_bIsInspectionSkip = FALSE;
				theProcBank.AZoneCellNG_OnOff(jig, ch, FALSE);
			}
		}
		else
		{
			if(theProcBank.AZoneCellNG_Check(jig, ch))
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			else
				pCell->defaultData.m_bIsInspectionSkip = FALSE;
		}
	}
}

void CFormMain::RemoveCell( JIG_ID jig, JIG_CH ch )
{
	CCellTag tag = theCellBank.GetCellTag(jig, ch);
	if(tag.IsExist())
	{
		CCellInfo *pCell = theCellBank.GetCellInfo(jig, ch);
		// Track In 된 셀이면 'O'로 해서 Track Out [9/27/2017 OSC]
		if( pCell->defaultData.m_bTrackinFinish
			&& (pCell->defaultData.m_bTrackOutFinish == FALSE) )
		{
			theSocketInterFace.m_CIM.SendCmdCellToDataPC(EQUIP_SET_CELL_PROCESS_COMPLETE_UNLOAD, pCell, CIM_JUDGE_OUT);
			theLog[LOG_TRACKING].AddBuf(_T("CellID[%s], InnerID[%s], Track Out Send [O]"), pCell->defaultData.m_strCellID, pCell->defaultData.m_strInnerID);
		}
		
		// Cell Tag 삭제 [9/27/2017 OSC]
		theCellBank.RemoveCellTag(jig, ch);
	}
}

void CFormMain::AZoneSkiipButton_Init()
{
	m_vct_AZoneSkipButton.clear();
	SKIP_BUTTON button;

	button.nResourceID = IDC_GXBTN_MAIN_CH1_NOT_USE;
	button.pbSet = &theProcBank.m_bAZoneChannelNotUse[JIG_ID_A][JIG_CH_1];
	button.pbNotUseEnable = NULL;
	button.pbAZoneEnd = &theProcBank.m_bIsZoneEnd[JIG_ID_A][ZONE_ID_A];
	m_vct_AZoneSkipButton.push_back(button);

	button.nResourceID = IDC_GXBTN_MAIN_CH2_NOT_USE;
	button.pbSet = &theProcBank.m_bAZoneChannelNotUse[JIG_ID_B][JIG_CH_1];
	button.pbNotUseEnable = NULL;
	button.pbAZoneEnd = &theProcBank.m_bIsZoneEnd[JIG_ID_B][ZONE_ID_A];
	m_vct_AZoneSkipButton.push_back(button);

	button.nResourceID = IDC_GXBTN_MAIN_CH1_SKIP;
	button.pbSet = &theProcBank.m_bAZoneCellSkip[JIG_ID_A][JIG_CH_1];
	button.pbNotUseEnable = &theProcBank.m_bAZoneChannelNotUse[JIG_ID_A][JIG_CH_1];
	button.pbAZoneEnd = &theProcBank.m_bIsZoneEnd[JIG_ID_A][ZONE_ID_A];
	m_vct_AZoneSkipButton.push_back(button);

	button.nResourceID = IDC_GXBTN_MAIN_CH2_SKIP;
	button.pbSet = &theProcBank.m_bAZoneCellSkip[JIG_ID_B][JIG_CH_1];
	button.pbNotUseEnable = &theProcBank.m_bAZoneChannelNotUse[JIG_ID_B][JIG_CH_1];
	button.pbAZoneEnd = &theProcBank.m_bIsZoneEnd[JIG_ID_B][ZONE_ID_A];
	m_vct_AZoneSkipButton.push_back(button);
}

void CFormMain::AZoneNotUseButton_Update()
{
	SKIP_BUTTON button;
	int nCount = m_vct_AZoneSkipButton.size();

	if(theConfigBank.m_System.m_bInlineMode)
	{
		for(int i = 0; i < nCount; i++)
		{
			button = m_vct_AZoneSkipButton[i];
			// NG 버튼은 무조건 비활성화 [11/27/2017 OSC]
			if(button.pbNotUseEnable)
			{
				CGxUICtrl::SetButtonEnabled(this, button.nResourceID, FALSE);
			}
			else
			{
				if(*button.pbSet)
					CGxUICtrl::SetButtonColor(this, button.nResourceID, GXCOLOR_CRIMSON);
				else
					CGxUICtrl::SetButtonColor(this, button.nResourceID, GXCOLOR_DESELECT);
			}
		}
	}
	else
	{
		for(int i = 0; i < nCount; i++)
		{
			button = m_vct_AZoneSkipButton[i];

			// A Zone 끝났으면 NG 버튼만 비활성화 [9/27/2017 OSC]
			if(*button.pbAZoneEnd)
			{
				if(button.pbNotUseEnable)
					CGxUICtrl::SetButtonEnabled(this, button.nResourceID, FALSE);
			}
			else
			{
				if(button.pbNotUseEnable)
				{	// Not Use가 설정되어있으면 NG버튼 비활성화 [9/12/2017 OSC]
					if(*button.pbNotUseEnable)
						CGxUICtrl::SetButtonEnabled(this, button.nResourceID, FALSE);
					else
						CGxUICtrl::SetButtonEnabled(this, button.nResourceID, TRUE);
				}
				else
				{
					CGxUICtrl::SetButtonEnabled(this, button.nResourceID, TRUE);
				}

				if(*button.pbSet)
					CGxUICtrl::SetButtonColor(this, button.nResourceID, GXCOLOR_CRIMSON);
				else
					CGxUICtrl::SetButtonColor(this, button.nResourceID, GXCOLOR_DESELECT);
			}
		}
	}
}

void CFormMain::CellStatus_Init()
{
	m_vct_CellStatus.clear();
	CELL_STATUS control;

	control.uIDCellInfoResult = IDC_GXSTC_MAIN_CH1_CELL_INFO_RESULT_TEXT;
	control.uIDJobProcess = IDC_GXSTC_MAIN_CH1_JOB_PROCESS_TEXT;
	control.uIDCellLoading = IDC_GXSTC_MAIN_CH1_CELL_LOADING;
	control.uIDInnerID = IDC_GXSTC_MAIN_CH1_INNERID_TEXT;
	control.uIDCellID = IDC_GXSTC_MAIN_CH1_CELLID_TEXT;
	control.uIDProductID = IDC_GXSTC_MAIN_CH1_PRODUCTID_TEXT;
	control.uIDAZoneDefect = IDC_GXSTC_MAIN_CH1_AZONE_TEXT;
	control.uIDWriteDefect = IDC_GXSTC_MAIN_CH1_WRITE_TEXT;
	control.uIDVerifyDefect = IDC_GXSTC_MAIN_CH1_VERIFY_TEXT;
	control.uIDCZoneDefect = IDC_GXSTC_MAIN_CH1_CZONE_TEXT;
	control.uIDFinalDefect = IDC_GXSTC_MAIN_CH1_FINAL_TEXT;
	control.posCell = CELL_POS_SHUTTLE1_CH1;
	control.strOldCellID.Empty();
	m_vct_CellStatus.push_back(control);

	control.uIDCellInfoResult = IDC_GXSTC_MAIN_CH2_CELL_INFO_RESULT_TEXT;
	control.uIDJobProcess = IDC_GXSTC_MAIN_CH2_JOB_PROCESS_TEXT;
	control.uIDCellLoading = IDC_GXSTC_MAIN_CH2_CELL_LOADING;
	control.uIDInnerID = IDC_GXSTC_MAIN_CH2_INNERID_TEXT;
	control.uIDCellID = IDC_GXSTC_MAIN_CH2_CELLID_TEXT;
	control.uIDProductID = IDC_GXSTC_MAIN_CH2_PRODUCTID_TEXT;
	control.uIDAZoneDefect = IDC_GXSTC_MAIN_CH2_AZONE_TEXT;
	control.uIDWriteDefect = IDC_GXSTC_MAIN_CH2_WRITE_TEXT;
	control.uIDVerifyDefect = IDC_GXSTC_MAIN_CH2_VERIFY_TEXT;
	control.uIDCZoneDefect = IDC_GXSTC_MAIN_CH2_CZONE_TEXT;
	control.uIDFinalDefect = IDC_GXSTC_MAIN_CH2_FINAL_TEXT;
	control.posCell = CELL_POS_SHUTTLE2_CH1;
	control.strOldCellID.Empty();
	m_vct_CellStatus.push_back(control);
}

void CFormMain::CellStatus_Update()
{
	CELL_STATUS control;
	CCellInfo *pCell;
	int nCount = m_vct_CellStatus.size();
	for(int i = 0; i < nCount; i++)
	{
		control = m_vct_CellStatus[i];

		pCell = theCellBank.GetCellInfo(control.posCell);
		if(pCell)
		{
			if(pCell->defaultData.m_CellInfoResult == NONE_CELL)
				CGxUICtrl::SetStaticColor(this, control.uIDCellInfoResult, GXCOLOR_WHITE);
			else if(pCell->defaultData.m_CellInfoResult == GOOD_CELL)
				CGxUICtrl::SetStaticColor(this, control.uIDCellInfoResult, GXCOLOR_ON);
			else
				CGxUICtrl::SetStaticColor(this, control.uIDCellInfoResult, GXCOLOR_RED);
			CGxUICtrl::SetStaticString(this, control.uIDCellInfoResult, pCell->defaultData.m_strCellInfoResult);

			switch(pCell->defaultData.m_nInspectInvalidType)
			{
			case JOB_DEFAULT:
				CGxUICtrl::SetStaticString(this, control.uIDJobProcess, _T("JOB_DEFAULT"));
				CGxUICtrl::SetStaticColor(this, control.uIDJobProcess, GXCOLOR_WHITE);
				break;
			case JOB_INVALID:
			case JOB_SKIP:
				CGxUICtrl::SetStaticString(this, control.uIDJobProcess, _T("JOB_INVALID"));
				CGxUICtrl::SetStaticColor(this, control.uIDJobProcess, GXCOLOR_RED);
				break;
			case JOB_START:
				CGxUICtrl::SetStaticString(this, control.uIDJobProcess, _T("JOB_START"));
				CGxUICtrl::SetStaticColor(this, control.uIDJobProcess, GXCOLOR_ON);
				break;
			default:
				CGxUICtrl::SetStaticString(this, control.uIDJobProcess, _T(""));
				CGxUICtrl::SetStaticColor(this, control.uIDJobProcess, GXCOLOR_WHITE);
				break;
			}

			switch(theCellBank.m_InfoCellLoading[pCell->defaultData.m_JigId][pCell->defaultData.m_JigCh].m_CellLoading.m_Class)
			{
			case NONE_CELL:
				CGxUICtrl::SetStaticColor(this, control.uIDCellLoading, GXCOLOR_WHITE);
				break;
			case GOOD_CELL:
				CGxUICtrl::SetStaticColor(this, control.uIDCellLoading, GXCOLOR_ON);
				break;
			default:
				CGxUICtrl::SetStaticColor(this, control.uIDCellLoading, GXCOLOR_RED);
				break;
			}

			CGxUICtrl::SetStaticString(this, control.uIDInnerID, pCell->defaultData.m_strInnerID);
			CGxUICtrl::SetStaticString(this, control.uIDCellID, pCell->defaultData.m_strCellID);
			if( pCell->defaultData.m_bMCR_OK || pCell->defaultData.m_strCellID.IsEmpty() )
				CGxUICtrl::SetStaticColor(this, control.uIDCellID, GXCOLOR_WHITE);
			else
				CGxUICtrl::SetStaticColor(this, control.uIDCellID, GXCOLOR_RED);

			CGxUICtrl::SetStaticString(this, control.uIDProductID, pCell->defaultData.m_strProductID);

			if(pCell->m_AZone.m_Class == GOOD_CELL)
				CGxUICtrl::SetStaticColor(this, control.uIDAZoneDefect, GXCOLOR_ON);
			else if(pCell->m_AZone.m_Class == REJECT_CELL)
				CGxUICtrl::SetStaticColor(this, control.uIDAZoneDefect, GXCOLOR_RED);
			else
				CGxUICtrl::SetStaticColor(this, control.uIDAZoneDefect, GXCOLOR_WHITE);
			CGxUICtrl::SetStaticString(this, control.uIDAZoneDefect, pCell->m_AZone.m_DefectName);

			if(pCell->m_MTPWrite.m_Class == GOOD_CELL)
				CGxUICtrl::SetStaticColor(this, control.uIDWriteDefect, GXCOLOR_ON);
			else if(pCell->m_MTPWrite.m_Class == REJECT_CELL)
				CGxUICtrl::SetStaticColor(this, control.uIDWriteDefect, GXCOLOR_RED);
			else
				CGxUICtrl::SetStaticColor(this, control.uIDWriteDefect, GXCOLOR_WHITE);
			CGxUICtrl::SetStaticString(this, control.uIDWriteDefect, pCell->m_MTPWrite.m_DefectName);

			if(pCell->m_MTPVerify.m_Class == GOOD_CELL)
				CGxUICtrl::SetStaticColor(this, control.uIDVerifyDefect, GXCOLOR_ON);
			else if(pCell->m_MTPVerify.m_Class == REJECT_CELL)
				CGxUICtrl::SetStaticColor(this, control.uIDVerifyDefect, GXCOLOR_RED);
			else
				CGxUICtrl::SetStaticColor(this, control.uIDVerifyDefect, GXCOLOR_WHITE);
			CGxUICtrl::SetStaticString(this, control.uIDVerifyDefect, pCell->m_MTPVerify.m_DefectName);

			if(pCell->m_CZone.m_Class == GOOD_CELL)
				CGxUICtrl::SetStaticColor(this, control.uIDCZoneDefect, GXCOLOR_ON);
			else if(pCell->m_CZone.m_Class == REJECT_CELL)
				CGxUICtrl::SetStaticColor(this, control.uIDCZoneDefect, GXCOLOR_RED);
			else
				CGxUICtrl::SetStaticColor(this, control.uIDCZoneDefect, GXCOLOR_WHITE);
			CGxUICtrl::SetStaticString(this, control.uIDCZoneDefect, pCell->m_CZone.m_DefectName);

			if(pCell->defaultData.m_LastClass == GOOD_CELL)
				CGxUICtrl::SetStaticColor(this, control.uIDFinalDefect, GXCOLOR_ON);
			else if(pCell->defaultData.m_LastClass == REJECT_CELL)
				CGxUICtrl::SetStaticColor(this, control.uIDFinalDefect, GXCOLOR_RED);
			else
				CGxUICtrl::SetStaticColor(this, control.uIDFinalDefect, GXCOLOR_WHITE);
			CGxUICtrl::SetStaticString(this, control.uIDFinalDefect, pCell->defaultData.m_strLastResult);

		}
		else
		{
			CGxUICtrl::SetStaticColor(this, control.uIDCellInfoResult, GXCOLOR_WHITE);
			CGxUICtrl::SetStaticString(this, control.uIDCellInfoResult, _T(""));
			CGxUICtrl::SetStaticColor(this, control.uIDJobProcess, GXCOLOR_WHITE);
			CGxUICtrl::SetStaticString(this, control.uIDJobProcess, _T(""));
			CGxUICtrl::SetStaticString(this, control.uIDInnerID, _T(""));
			CGxUICtrl::SetStaticString(this, control.uIDCellID, _T(""));
			CGxUICtrl::SetStaticColor(this, control.uIDCellID, GXCOLOR_WHITE);
			CGxUICtrl::SetStaticString(this, control.uIDProductID, _T(""));
			CGxUICtrl::SetStaticColor(this, control.uIDAZoneDefect, GXCOLOR_WHITE);
			CGxUICtrl::SetStaticString(this, control.uIDAZoneDefect, _T(""));
			CGxUICtrl::SetStaticColor(this, control.uIDWriteDefect, GXCOLOR_WHITE);
			CGxUICtrl::SetStaticString(this, control.uIDWriteDefect, _T(""));
			CGxUICtrl::SetStaticColor(this, control.uIDVerifyDefect, GXCOLOR_WHITE);
			CGxUICtrl::SetStaticString(this, control.uIDVerifyDefect, _T(""));
			CGxUICtrl::SetStaticColor(this, control.uIDCZoneDefect, GXCOLOR_WHITE);
			CGxUICtrl::SetStaticString(this, control.uIDCZoneDefect, _T(""));
			CGxUICtrl::SetStaticColor(this, control.uIDFinalDefect, GXCOLOR_WHITE);
			CGxUICtrl::SetStaticString(this, control.uIDFinalDefect, _T(""));
		}
	}

	//kjpark 20170928 스타트와 정지 중복 보고 하면 안됨
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN)
	{
		CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_START, FALSE);
		CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_STOP, TRUE);
	}
	else
	{
		CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_START, TRUE);
		CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_STOP, FALSE);
	}

	//kjpark 20171016 계측기 코릴레이션 하강 / 원복 버튼
	if(GetMainHandler()->m_bBackDoorOpen)
	{
		CGxUICtrl::SetButtonVisible(this, IDC_GXSTC_MAIN_CO_RELATION, TRUE);		
	}
	else
	{
		CGxUICtrl::SetButtonVisible(this, IDC_GXSTC_MAIN_CO_RELATION, FALSE);
	}
}

void CFormMain::OptionMonitor_Init()
{
	m_vct_OptionMonitor.clear();
	OPTION_MONITOR control;

	control.uID = IDC_GXSTC_MAIN_OPTION_CELL_MIXING;
	control.pbOption = &theConfigBank.m_Option.m_bUseTMDNameMatch;
	control.bOnGreen = TRUE;
	m_vct_OptionMonitor.push_back(control);

	control.uID = IDC_GXSTC_MAIN_OPTION_LOOF_TEST;
	control.pbOption = &theConfigBank.m_Option.m_bUseLoofTest;
	control.bOnGreen = FALSE;
	m_vct_OptionMonitor.push_back(control);

	//RETRY_AB 사용여부 표시 - JSW180115
	control.uID = IDC_GXSTC_MAIN_OPTION_AB_RETRY;
	control.pbOption = &theConfigBank.m_Option.m_bUseRetryAB;
	control.bOnGreen = TRUE;
	m_vct_OptionMonitor.push_back(control);
}

void CFormMain::OptionMonitor_Update()
{
	OPTION_MONITOR control;

	int nCount = m_vct_OptionMonitor.size();
	
	
		for(int i = 0; i < nCount; i++)
		{
			
			control = m_vct_OptionMonitor[i];

			if (control.bOnGreen == TRUE)
			{
				if(*control.pbOption)
					CGxUICtrl::SetStaticColor(this, control.uID, GXCOLOR_ON);
				else
					CGxUICtrl::SetStaticColor(this, control.uID, GXCOLOR_GRAY);
			}
			else
			{
				if(*control.pbOption)
					CGxUICtrl::SetStaticColor(this, control.uID, GXCOLOR_RED);
				else
					CGxUICtrl::SetStaticColor(this, control.uID, GXCOLOR_GRAY);
			}
		}
	

}

void CFormMain::ClickBtnModePM()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("MAIN"),_T("PM MODE"));
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	
	pMain->setModePM(TRUE);

	// 현재 Tower lamp의 상태를 PM mode로 바꾼다 - LSH171128
	theUnitFunc.TowerLamp_Change(theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_PMMODE].Red.lamp_state
		,theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_PMMODE].Yellow.lamp_state
		, theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_PMMODE].Green.lamp_state
		,FALSE);

	// 작업자 로그아웃 [12/3/2016 OSC]
	CUserAccount user;
	pMain->SetCurUserOperatorData(user);
	ChangeForm(eFORM_PM);
}


void CFormMain::ClickGxbtnStop()
{	
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("MAIN"),_T("STOP"));
	if(theProcBank.MachineIsRunState())
	{
		theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);

		theLossTimeLog.m_rLosstimeRecord.Clear();
		theLossTimeLog.SetMachineStopTime();
		theLossTimeLog.ShowLossTimeDlg(TRUE);

		// 설비상태변경 보고 후 TP에 보고 [3/20/2017 OSC]
		theSocketInterFace.m_CIM.SendCmdTPSystemToDataPC(
			theLossTimeLog.m_rLosstimeRecord.MesCode,
			theLossTimeLog.m_rLosstimeRecord.sTPSystemName);

		//kjpark 20170410 런 중에 Stop 해도 사용자 버튼이 눌리는 버그 수정
		GetMainHandler()->m_bAZoneOperatorReady[JIG_ID_A] = FALSE;
		GetMainHandler()->m_bAZoneOperatorReady[JIG_ID_B] = FALSE;
	}
	else if( (theProcBank.GetMachineState() != CONST_MACHINE_STATE::MACHINE_INIT)
		&& (theProcBank.m_AlarmID == ALM_NONE) )
	{
		theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);
		
	}
}


void CFormMain::ClickGxbtnStart()
{
	//kjpark 20180107 Remote Mode 가 아니면 팝업 발생
	if(theProcBank.GetCimState() != CONST_CIM_STATE::CIM_REMOTE)
	{
		CGxMsgBox	dlgMsgBox;
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
			_T("MACHINE Is not REMOTE\n To be Continue?"), 
			_T("MACHINE Is not REMOTE\n To be Continue?"), 
			_T("MACHINE Is not REMOTE\n To be Continue?"),   GetMainHandler()->m_nLangIdx);

		if(dlgMsgBox.DoModal() != IDOK)
			return;
	}

	if ( theProcBank.m_AlarmID != ALM_NONE )
		return;

	if(theProcBank.MachineIsRunState())
		return;
	
	if(theUnitFunc.DoorClose_Check() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			theProcBank.m_strLastKorMsg, 
			theProcBank.m_strLastEngMsg,		
			theProcBank.m_strLastVnmMsg , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();
		return;
	}

	// Auto Key가 있으니 필요없다
// 	if(theUnitFunc.DoorLockChkStart()==FALSE)
// 		return;
	if(theUnitFunc.LightCurtain_Check() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			theProcBank.m_strLastKorMsg, 
			theProcBank.m_strLastEngMsg,		
			theProcBank.m_strLastVnmMsg , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();
		return;
	}

	//20170623 BKH, GPS Power가 OFF되어 있을시에는 Start를 하지 못하도록 막는다.
	if(theUnitFunc.GetInPutIOCheck(X_MC_ON_CHECK) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("MC Power가 Off되어 있습니다."), 
			_T("Please Turn On MC Power."),		
			_T("Vui lòng bật MC Power ON.") , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();
		return;
	}

	STO_STATE sto = theUnitFunc.STO_Check();
	if(sto != STO_READY)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			theProcBank.m_strLastKorMsg, 
			theProcBank.m_strLastEngMsg,		
			theProcBank.m_strLastVnmMsg , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();

		// STO_WARNING 상태면 진행은 가능하니 메세지만 표시하고 넘어간다 [9/19/2017 OSC]
		if(sto == STO_ALARM)
			return;
	}

	if(theUnitFunc.AutoTeachKey_AutoCheck() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			theProcBank.m_strLastKorMsg, 
			theProcBank.m_strLastEngMsg,		
			theProcBank.m_strLastVnmMsg , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();
		return; 
	}

	if( (theProcBank.m_bDryRunMode == FALSE)
		&& (theProcBank.GetCimState() == CONST_CIM_STATE::CIM_REMOTE) )
	{
		
// 			if(GetMainHandler()->GetCurEngeneerUserOperatorData().nLevel == eUSER_NONE)
// 			{
// 				CGxMsgBox	dlgMsgBox;
// 
// 				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
// 				dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
// 				dlgMsgBox.SetMessage(FALSE, 
// 					_T("B존 작업자를 로그인 해 주세요."), 
// 					_T("Please Login B Zone operator."),		
// 					_T("Vui lòng đăng nhập với tư cách người điều hành vào B Zone."), GetMainHandler()->m_nLangIdx );
// 
// 				dlgMsgBox.DoModal();
// 				return;
// 			}
		
			if(theConfigBank.m_System.m_bInlineMode == FALSE)
			{
				if(GetMainHandler()->GetCurOperatorUserInspectorData().nLevel == eUSER_NONE)
				{
					CGxMsgBox	dlgMsgBox;

					dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
					dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
					dlgMsgBox.SetMessage(FALSE, 
						_T("검사자를 로그인 해 주세요."), 
						_T("Please Login inspector."),		
						_T("Vui lòng đăng nhập với tư cách người kiểm tra vào."), GetMainHandler()->m_nLangIdx );

					dlgMsgBox.DoModal();
					return;
				}
			}
	}

// 	if(theConfigBank.m_System.m_bInlineMode)
// 	{
//  		if( (theUnitFunc.GetInPutIOCheck(X_HOT_LINE_PDT_ALIVE) == FALSE)
//  			|| (theUnitFunc.GetInPutIOCheck(X_HOT_LINE_PDT_NET_READY) == FALSE) )
//  		{
//  			CGxMsgBox	dlgMsgBox;
//  
//  			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
//  			dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
//  			dlgMsgBox.SetMessage(FALSE, 
//  				_T("ROBOT이 RUN 상태가 아닙니다."), 
//  				_T("ROBOT is not RUN."),		
//  				_T("ROBOT khong o trang thai RUN."), GetMainHandler()->m_nLangIdx );
//  
//  			dlgMsgBox.DoModal();
//  			return;
//  		}
//  		if(theUnitFunc.GetInPutIOCheck(X_HOT_LINE_PDT_EMERGENCY))
//  		{
//  			CGxMsgBox	dlgMsgBox;
//  
//  			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
//  			dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
//  			dlgMsgBox.SetMessage(FALSE, 
//  				_T("ROBOT이 EMERGENCY 상태입니다."), 
//  				_T("ROBOT is EMERGENCY."),		
//  				_T("ROBOT dang o trang thai EMERGENCY."), GetMainHandler()->m_nLangIdx );
//  
//  			dlgMsgBox.DoModal();
//  			return;
//  		}
//  		if( theUnitFunc.GetInPutIOCheck(X_HOT_LINE_PDT_DOOR_OPEN)
//  			|| theUnitFunc.GetInPutIOCheck(X_HOT_LINE_PDT_INTERLOCK) )
//  		{
//  			CGxMsgBox	dlgMsgBox;
//  
//  			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
//  			dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
//  			dlgMsgBox.SetMessage(FALSE, 
//  				_T("ROBOT의 DOOR 및 INTERLOCK 상태 확인해주세요"), 
//  				_T("Please check DOOR or INTERLOCK of ROBOT."),		
//  				_T("Xin hay kiem tra cua hoac interlock cua Robot."), GetMainHandler()->m_nLangIdx );
//  
//  			dlgMsgBox.DoModal();
//  			return;
//  		}
// 	}

	if(theRecipeBank.m_bModule_LoadSuccess == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("검사레시피 파일 오류로 진행이 불가합니다."), 
			_T("MTP_RECIPE File is wrong. Can not START."),		
			_T("File MTP_RECIPE bi SAI. Khong the START duoc.") , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();
		return; 
	}

	// B존 로그인 정보 동기화
	if(GetMainHandler()->GetCurEngeneerUserOperatorData().nLevel != 0)
		theSocketInterFace.m_CIM.SendCmdLoginToDataPC(EQUIP_SET_INS2_USER_LOGIN, GetMainHandler()->GetCurEngeneerUserOperatorData().sID);
	else
		theSocketInterFace.m_CIM.SendCmdLoginToDataPC(EQUIP_SET_INS2_USER_LOGOUT);

	// D존 로그인 정보 동기화
	if(GetMainHandler()->GetCurOperatorUserInspectorData().nLevel != 0)
		theSocketInterFace.m_CIM.SendCmdLoginToDataPC(EQUIP_SET_INS1_USER_LOGIN, GetMainHandler()->GetCurOperatorUserInspectorData().sID);
	else
		theSocketInterFace.m_CIM.SendCmdLoginToDataPC(EQUIP_SET_INS1_USER_LOGOUT);


	//kjparkk 20180131 PDT AB RULE 체크 기능 추가
#if 0
	if(theUnitFunc.PDT_IF_RETRY_AB_Check() != theConfigBank.m_Option.m_bUseRetryAB)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("AB RULE 검사 옵션이 다릅니다"), 
			_T("MTP_RECIPE File is wrong. Can not START."),		
			_T("File MTP_RECIPE bi SAI. Khong the START duoc.") , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();
		return; 
	}
#endif

	CGxMsgBox	dlgMsgBox;
	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
		_T("지금 RUN START 하시겠습니까?"), 
		_T("Do you want to start now?"),		
		_T("Bạn có muốn bắt đầu bây giờ?"), GetMainHandler()->m_nLangIdx );

	int nRet = dlgMsgBox.DoModal();
	if(nRet != IDOK)
	{
		return;
	}

	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("MAIN"),_T("START"));

	//////////////////////////////////////////////////////////////////////////
	// ErrorLog.csv 기록 [6/30/2017 OSC]
	theProcBank.WriteErrorLogCsv();


	theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_CHECK);
}

//kjpark 20171016 계측기 코릴레이션 하강 / 원복 버튼
void CFormMain::ClickGxbtnCorelation()
{
	if(m_StateCorelation.IsStoped() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("모터 이동중입니다."), 
			_T("Now Moving..."), 
			_T("Motor dang chuyen dong...") , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();
		return;
	}

	if(theUnitFunc.DoorClose_Check() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			theProcBank.m_strLastKorMsg, 
			theProcBank.m_strLastEngMsg,		
			theProcBank.m_strLastVnmMsg , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();
		return;
	}

	if(theUnitFunc.LightCurtain_Check() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			theProcBank.m_strLastKorMsg, 
			theProcBank.m_strLastEngMsg,		
			theProcBank.m_strLastVnmMsg , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();
		return;
	}


	if(theUnitFunc.AutoTeachKey_AutoCheck() == FALSE)
	{
		if(theUnitFunc.DoorLockOn_Check() == FALSE)
		{
			CGxMsgBox	dlgMsgBox;
			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
			dlgMsgBox.SetMessage(FALSE, 
				theProcBank.m_strLastKorMsg, 
				theProcBank.m_strLastEngMsg,		
				theProcBank.m_strLastVnmMsg , GetMainHandler()->m_nLangIdx);

			dlgMsgBox.DoModal();
			return;
		}
	}

	if(theUnitFunc.GetInPutIOCheck(X_MC_ON_CHECK) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("MC Power가 Off되어 있습니다."), 
			_T("Please Turn On MC Power."),		
			_T("Xin vui lòng bật ON MC Power.") , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();
		return;
	}

	STO_STATE sto = theUnitFunc.STO_Check();
	if(sto != STO_READY)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			theProcBank.m_strLastKorMsg, 
			theProcBank.m_strLastEngMsg,		
			theProcBank.m_strLastVnmMsg , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();

		// STO_WARNING 상태면 진행은 가능하니 메세지만 표시하고 넘어간다 [9/19/2017 OSC]
		if(sto == STO_ALARM)
			return;
	}

	CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("확인"), _T("Question"), _T("Question"));
	dlgMsgBox.SetMessage(TRUE, 
		_T("Corelation 위치로 이동하시겠습니까?"), 
		_T("Move to Corelation position?"), 
		_T("Ban muon di chuyen den vi tri Corelation?") , GetMainHandler()->m_nLangIdx);

	if(dlgMsgBox.DoModal() != IDOK)
		return;


	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("MAIN"), _T("Corelation"));	

	m_StateCorelation.Reset();
	m_StateCorelation.Start();
	SetTimer(TIMER_STEP, 100, NULL);
}


void CFormMain::ClickGxbtnMainCh1NotUse()
{
	theProcBank.AZoneChannelNotUse_OnOff(JIG_ID_A, JIG_CH_1, !theProcBank.AZoneChannelNotUse_Check(JIG_ID_A, JIG_CH_1));
	if(theConfigBank.m_System.m_bInlineMode)
	{
		if(theCellBank.GetCellTag(CELL_POS_SHUTTLE1_CH1).IsExist())
		{
			CGxMsgBox	dlgMsgBox;
			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
			dlgMsgBox.SetMessage(TRUE, 
				_T("CH1 Cell을 제거하시겠습니까?"), 
				_T("Do you remove CH1 cell?"),		
				_T("Ban muon lay Cell o CH1 ra?"), GetMainHandler()->m_nLangIdx);

			if(dlgMsgBox.DoModal() != IDOK)
			{
				//kjpark 20180109 NOT USE -> No 시 다시 원복 해줌
				theProcBank.AZoneChannelNotUse_OnOff(JIG_ID_A, JIG_CH_1, !theProcBank.AZoneChannelNotUse_Check(JIG_ID_A, JIG_CH_1));
				return;
			}
			if(theConfigBank.m_Option.m_bUseIf_NotUseWriteCellLog) theUnitFunc.CellLog_Write(JIG_ID_A, JIG_CH_1);
			RemoveCell(JIG_ID_A, JIG_CH_1);
		}
	}
	else
	{
		if(theUnitFunc.GetZoneEnd(JIG_ID_A, ZONE_ID_A))
		{
			CGxMsgBox	dlgMsgBox;
			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
			dlgMsgBox.SetMessage(TRUE, 
				_T("CH1 Cell을 제거하시겠습니까?"), 
				_T("Do you remove CH1 cell?"),		
				_T("Ban muon lay Cell o CH1 ra?"), GetMainHandler()->m_nLangIdx);

			if(dlgMsgBox.DoModal() != IDOK)
			{
				//kjpark 20180109 NOT USE -> No 시 다시 원복 해줌
				theProcBank.AZoneChannelNotUse_OnOff(JIG_ID_A, JIG_CH_1, !theProcBank.AZoneChannelNotUse_Check(JIG_ID_A, JIG_CH_1));
				return;
			}
			if(theConfigBank.m_Option.m_bUseIf_NotUseWriteCellLog) theUnitFunc.CellLog_Write(JIG_ID_A, JIG_CH_1);
			RemoveCell(JIG_ID_A, JIG_CH_1);
		}
		else
		{
			AZoneCellNGOnOff(JIG_ID_A, JIG_CH_1);
		}
	}
}


void CFormMain::ClickGxbtnMainCh1Skip()
{
	theProcBank.AZoneCellNG_OnOff(JIG_ID_A, JIG_CH_1, !theProcBank.AZoneCellNG_Check(JIG_ID_A, JIG_CH_1));
	AZoneCellNGOnOff(JIG_ID_A, JIG_CH_1, TRUE);
}

void CFormMain::ClickGxbtnMainCh3NotUse()
{
	theProcBank.AZoneChannelNotUse_OnOff(JIG_ID_B, JIG_CH_1, !theProcBank.AZoneChannelNotUse_Check(JIG_ID_B, JIG_CH_1));
	if(theConfigBank.m_System.m_bInlineMode)
	{
		if(theCellBank.GetCellTag(CELL_POS_SHUTTLE2_CH1).IsExist())
		{
			CGxMsgBox	dlgMsgBox;
			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
			dlgMsgBox.SetMessage(TRUE, 
				_T("CH3 Cell을 제거하시겠습니까?"), 
				_T("Do you remove CH3 cell?"),		
				_T("Ban muon lay Cell o CH3 ra?"), GetMainHandler()->m_nLangIdx);

			if(dlgMsgBox.DoModal() != IDOK)
			{
				//kjpark 20180109 NOT USE -> No 시 다시 원복 해줌
				theProcBank.AZoneChannelNotUse_OnOff(JIG_ID_B, JIG_CH_1, !theProcBank.AZoneChannelNotUse_Check(JIG_ID_B, JIG_CH_1));
				return;
			}
			if(theConfigBank.m_Option.m_bUseIf_NotUseWriteCellLog) theUnitFunc.CellLog_Write(JIG_ID_B, JIG_CH_1);
			RemoveCell(JIG_ID_B, JIG_CH_1);
		}
	}
	else
	{
		if(theUnitFunc.GetZoneEnd(JIG_ID_B, ZONE_ID_A))
		{
			CGxMsgBox	dlgMsgBox;
			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
			dlgMsgBox.SetMessage(TRUE, 
				_T("CH3 Cell을 제거하시겠습니까?"), 
				_T("Do you remove CH3 cell?"),		
				_T("Ban muon lay Cell o CH3 ra?"), GetMainHandler()->m_nLangIdx);

			if(dlgMsgBox.DoModal() != IDOK)
			{
				//kjpark 20180109 NOT USE -> No 시 다시 원복 해줌
				theProcBank.AZoneChannelNotUse_OnOff(JIG_ID_B, JIG_CH_1, !theProcBank.AZoneChannelNotUse_Check(JIG_ID_B, JIG_CH_1));
				return;
			}
			if(theConfigBank.m_Option.m_bUseIf_NotUseWriteCellLog) theUnitFunc.CellLog_Write(JIG_ID_B, JIG_CH_1);
			RemoveCell(JIG_ID_B, JIG_CH_1);
		}
		else
		{
			AZoneCellNGOnOff(JIG_ID_B, JIG_CH_1);
		}
	}
}


void CFormMain::ClickGxbtnMainCh3Skip()
{
	theProcBank.AZoneCellNG_OnOff(JIG_ID_B, JIG_CH_1, !theProcBank.AZoneCellNG_Check(JIG_ID_B, JIG_CH_1));
	AZoneCellNGOnOff(JIG_ID_B, JIG_CH_1, TRUE);
}






void CFormMain::ClickGxstcMainCountClear()
{
// 	::PostMessage(GetMainWnd(), MSG_SHOW_DIALOG, eDLG_MsgInterlock, NULL);

	CTime time = CTime::GetCurrentTime();
	theProductBank.m_timeMainCount = time;
	CString strTime;
	strTime.Format(_T("%02d-%02d %02d:%02d:%02d"), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_CLEAR_TIME, strTime);
	theProductBank.MainCount_Init();
	theProductBank.MainCount_Save();
	::PostMessage(m_hWnd, UDMSG_COUNT_UPDATE, NULL, NULL);
	
}


void CFormMain::ClickGxbtnProductEnd()
{
	// 트랙아웃 안한 셀 들 다 트랙아웃 한다. [9/28/2017 OSC]
	if(theProcBank.MachineIsRunState() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
			_T("생산을 종료하시겠습니까?"), 
			_T("Do you want finish product?"),		
			_T("Do you want finish product?"), GetMainHandler()->m_nLangIdx);

		if(dlgMsgBox.DoModal() != IDOK)
			return;

		GetMainHandler()->AllCellTrackOut();
		theProcBank.ResetTemporarilyValue();

		// PDT Interface도 클리어
		theProcBank.m_bPDT_IF_State_Clear = TRUE;
// 		theSocketInterFace.m_CIM.SendCmdStateToDataPC(EQUIP_SET_EQUIPMENT_STATUS_CHANGE, 
// 			E_EQST_MATCH_PAUSE, _T(""));
	}
}


///////////////////////// Cell 정보 생성 /////////////////////////
void CFormMain::UpdateCellCreateButton()
{
	if(theConfigBank.m_System.m_bCIMQualMode)
	{
		CGxUICtrl::SetButtonVisible(this, IDC_GXBTN_CREATECELLINFO, TRUE);
	}
	else
	{
		CGxUICtrl::SetButtonVisible(this, IDC_GXBTN_CREATECELLINFO, FALSE);
	}

// 	// Button 활성 비활성 상태 정의 - LSH171215 
// 	if(m_bCH1lSelect || m_bCH2lSelect || m_bCH3lSelect ||
// 		m_bCH4lSelect || m_bCH5lSelect || m_bCH6lSelect)
// 		CGxUICtrl::SetButtonVisible(this, IDC_GXBTN_CREATECELLINFO, TRUE);
// 	else
// 		CGxUICtrl::SetButtonVisible(this, IDC_GXBTN_CREATECELLINFO, FALSE);
}


/* A Shuttle */
void CFormMain::ClickGxstcMainCh1()
{
	if(theConfigBank.m_System.m_bCIMQualMode == FALSE)
		return;

	if(m_bCH1lSelect)
	{
		// 선택된 상태로 변경 - LSH171215
		m_bCH1lSelect = FALSE;
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_MAIN_CH1, GXCOLOR_OFF);
	}
	else
	{
		// 비선택된 상태로 변경 - LSH171215
		m_bCH1lSelect = TRUE;
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_MAIN_CH1, GXCOLOR_ON);
	}

// 	// Create cell info button활성화 - LSH171215
// 	UpdateCellCreateButton();
}




/* B Shuttle */
void CFormMain::ClickGxstcMainCh3()
{
	if(theConfigBank.m_System.m_bCIMQualMode == FALSE)
		return;

	if(m_bCH3lSelect)
	{
		// 선택된 상태로 변경 - LSH171215
		m_bCH3lSelect = FALSE;
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_MAIN_CH2, GXCOLOR_OFF);
	}
	else
	{
		// 비선택된 상태로 변경 - LSH171215
		m_bCH3lSelect = TRUE;
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_MAIN_CH2, GXCOLOR_ON);
	}

// 	// Create cell info button활성화 - LSH171215
// 	UpdateCellCreateButton();
}



void CFormMain::ClickGxbtnCreatecellinfo()
{
	if(theProcBank.LoadingStop_IsRequire())
		return;
	// Cell  정보 생성 - LSH171215
	/* CH1 */
	if(m_bCH1lSelect)
	{
		if(theProcBank.AZoneChannelNotUse_Check(JIG_ID_A, JIG_CH_1) == FALSE)
		{
			theUnitFunc.AZoneCellData_Create(JIG_ID_A, JIG_CH_1);
			theUnitFunc.Shuttle_Vac_OnOff(JIG_ID_A, JIG_CH_1, VAC_ON);
		}
	}
	/* CH3 */
	if(m_bCH3lSelect)
	{
		if(theProcBank.AZoneChannelNotUse_Check(JIG_ID_B, JIG_CH_1) == FALSE)
		{
			theUnitFunc.AZoneCellData_Create(JIG_ID_B, JIG_CH_1);
			theUnitFunc.Shuttle_Vac_OnOff(JIG_ID_B, JIG_CH_1, VAC_ON);
		}
	}
}

// #include "FTP/FTPHandler.h"
void CFormMain::ClickGxbtnLoadingStop()
{
// 	CString a = _T("Anonymous");
// 	CString b;
// 	CFTPHandler ftp;
// 	BOOL ba = ftp.OpenFtp(_T("ftp://192.168.10.140/"), NULL, NULL);
// 	CString cc = ftp.GetLastFtpRespons();
// 	ba = 1;
// 	return;

	if(theProcBank.UserLoadingStop_Check())
	{
		theProcBank.UserLoadingStop_OnOff(FALSE);
	}
	else
	{
		CGxMsgBox	dlgMsgBox;
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
			_T("LOADING STOP을 하시겠습니까?"), 
			_T("Do you want LOADING STOP?"),		
			_T("Do you want LOADING STOP?"), GetMainHandler()->m_nLangIdx);

		if(dlgMsgBox.DoModal() != IDOK)
			return;

		theProcBank.UserLoadingStop_OnOff(TRUE);
	}
}
