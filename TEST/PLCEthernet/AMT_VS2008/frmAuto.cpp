// MTMain.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MT.h"
#include "frmAuto.h"
#include "DlgInitial.h"
#include "DlgModelChange.h"
#include "DlgModleLoading.h"
#include "DlgCellInfo.h"

#define TIMER_CONNECTION	0
using namespace Gdiplus;	// Color class



#define	TXT_MODE_NORMAL			_T("NORMAL")		// 정상 동작 모드
#define TXT_MODE_STAGE			_T("STAGE")			// 단일 스테이지 동작 모드		얼라인 컨택만 무한 반복... 틸팅도?
#define	TXT_MODE_ALIGN_SKIP		_T("ALIGN SKIP")	// 얼라인 스킵(매뉴얼 컨텍) 모드
#define	TXT_MODE_VISION_SKIP	_T("ALIGN+VI SKIP")	// 얼라인 + 검사 스킵 모드

// CFormAuto

IMPLEMENT_DYNCREATE(CFormAuto, CFormView)

CFormAuto::CFormAuto()
	: CFormView(CFormAuto::IDD)
	, m_ctlInspection_Counter()
{
	m_pDevice = NULL;
	m_bLive = FALSE;
	m_bConnectionLC = TRUE;
	m_bConnectionVI = FALSE;
}

CFormAuto::~CFormAuto()
{

}

void CFormAuto::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_AUTO_APPLY_PROCESS_INITIALIZE, m_btnAuto[BTN_APPLY_PROCESS_INITIALIZE]	);
	DDX_Control(pDX, IDC_BTN_AUTO_CAM0_COUNT_RESET, 		m_btnAuto[BTN_CAM0_COUNT_RESET]			);
	DDX_Control(pDX, IDC_BTN_AUTO_CAM1_COUNT_RESET, 		m_btnAuto[BTN_CAM1_COUNT_RESET]			);
	DDX_Control(pDX, IDC_BTN_AUTO_CAM2_COUNT_RESET, 		m_btnAuto[BTN_CAM2_COUNT_RESET]			);
	DDX_Control(pDX, IDC_BTN_AUTO_CAM3_COUNT_RESET, 		m_btnAuto[BTN_CAM3_COUNT_RESET]			);
	DDX_Control(pDX, IDC_BTN_AUTO_START,					m_btnAuto[BTN_START]					);
	DDX_Control(pDX, IDC_BTN_AUTO_STOP,						m_btnAuto[BTN_STOP]						);
	DDX_Control(pDX, IDC_BTN_AUTO_PM_MODE,					m_btnAuto[BTN_PM_MODE]					);
	DDX_Control(pDX, IDC_BTN_AUTO_INITIALIZE,				m_btnAuto[BTN_INITIALIZE]				);
	DDX_Control(pDX, IDC_BTN_AUTO_MODEL_LOADING,			m_btnAuto[BTN_MODEL_LOADING]			);
	DDX_Control(pDX, IDC_RADIO_AUTO_NORMAL,					m_btnAuto[BTN_NORMAL]					);
	DDX_Control(pDX, IDC_RADIO_AUTO_STAGE,					m_btnAuto[BTN_STAGE]					);
	DDX_Control(pDX, IDC_RADIO_AUTO_ALIGN_SKIP,				m_btnAuto[BTN_ALIGN_SKIP]				);
	DDX_Control(pDX, IDC_RADIO_AUTO_ALIGN_VI_SKIP,			m_btnAuto[BTN_ALIGN_VI_SKIP]			);
	DDX_Control(pDX, IDC_RADIO_AUTO_LIVE, 					m_btnAuto[BTN_LIVE]						);
	DDX_Control(pDX, IDC_RADIO_AUTO_HALT, 					m_btnAuto[BTN_HALT]						);
	DDX_Control(pDX, IDC_RADIO_AUTO_SCRAP,					m_btnAuto[BTN_SCRAP]					);
	DDX_Control(pDX, IDC_RADIO_AUTO_UNSCRAP,				m_btnAuto[BTN_UNSCRAP]					);

	DDX_Control(pDX, IDC_STATIC_AUTO_CAM0, m_pctImageGrap[CAM_0]);
	DDX_Control(pDX, IDC_STATIC_AUTO_CAM1, m_pctImageGrap[CAM_1]);
	DDX_Control(pDX, IDC_STATIC_AUTO_CAM2, m_pctImageGrap[CAM_2]);
	DDX_Control(pDX, IDC_STATIC_AUTO_CAM3, m_pctImageGrap[CAM_3]);

	DDX_Control(pDX, IDC_LABEL_AUTO_CURRENT_MODEL_NAME, m_lbCurMoDelName);

	DDX_Control(pDX, IDC_LABEL_AUTO_RUN_STATUS_STATE, m_lblRun_State);
	DDX_Control(pDX, IDC_LABEL_AUTO_LC_ONLINE, m_lblConnectionLC);
	DDX_Control(pDX, IDC_LABEL_AUTO_VI_ONLINE, m_lblConnectionVI);
	DDX_Control(pDX, IDC_LABEL_AUTO_RUN_MODE_STATE, m_lblRunMode);
	DDX_Control(pDX, IDC_LABEL_AUTO_CONTACT_TT_VALUE, m_lblContactTact);
	DDX_Control(pDX, IDC_LABEL_AUTO_FLOWING_TT_VALUE, m_lblFlowingTact);
	DDX_Control(pDX, IDC_LABEL_AUTO_GRAB_TT_VALUE, m_lblPostGrabTact);
	DDX_Control(pDX, IDC_LABEL_AUTO_INSPECTION_TT_VALUE, m_lblTotalInspTact);
	DDX_Control(pDX, IDC_ISEVENSEGMENTINTEGERX_AUTO_CAM0_COUNT, m_segCamCount[CAM_0]);
	DDX_Control(pDX, IDC_ISEVENSEGMENTINTEGERX_AUTO_CAM1_COUNT, m_segCamCount[CAM_1]);
	DDX_Control(pDX, IDC_ISEVENSEGMENTINTEGERX_AUTO_CAM2_COUNT, m_segCamCount[CAM_2]);
	DDX_Control(pDX, IDC_ISEVENSEGMENTINTEGERX_AUTO_CAM3_COUNT, m_segCamCount[CAM_3]);
	DDX_Control(pDX, IDC_LABEL_AUTO_PANEL_LOADING_READY,	m_lblRunProcess[ProcLoadingReady]	);
	DDX_Control(pDX, IDC_LABEL_AUTO_PANEL_LOADING_DONE,		m_lblRunProcess[ProcLoadingDone]	);
	DDX_Control(pDX, IDC_LABEL_AUTO_GLASS_CLAMP_ON,			m_lblRunProcess[ProcGlassClampOn]	);
	DDX_Control(pDX, IDC_LABEL_AUTO_ALIGN,					m_lblRunProcess[ProcAlign]			);
	DDX_Control(pDX, IDC_LABEL_AUTO_CONTACT_ON,				m_lblRunProcess[ProcContactOn]		);
	DDX_Control(pDX, IDC_LABEL_AUTO_PRE_GRAB,				m_lblRunProcess[ProcPreGrab]		);
	DDX_Control(pDX, IDC_LABEL_AUTO_INSPECTION_START,		m_lblRunProcess[ProcInspectionStart]);
	DDX_Control(pDX, IDC_LABEL_AUTO_RELEASE_CONTACT,		m_lblRunProcess[ProcReleaseContact]	);
	DDX_Control(pDX, IDC_LABEL_AUTO_PANEL_UNLOADING_READY,	m_lblRunProcess[ProcUnloadingReady]	);
	DDX_Control(pDX, IDC_LABEL_AUTO_PANEL_UNLOADING_DONE,	m_lblRunProcess[ProcUnLoadingDone]	);
	DDX_Control(pDX, IDC_ISWITCHLEDX_AUTO_TOP_LIGHT, m_swxAutoTopLight);
	DDX_Control(pDX, IDC_ISWITCHLEDX_AUTO_BOTTOM_LIGHT, m_swxAutoBtmLight);
	DDX_Control(pDX, IDC_ISEVENSEGMENTINTEGERX_INSPECTION_COUNTER, m_ctlInspection_Counter);
}

BEGIN_MESSAGE_MAP(CFormAuto, CFormView)
	//////////////////////////////////////////////////////////////////////////
	// User Message
	ON_MESSAGE(WM_LABEL_CAPTION, &CFormAuto::OnLabelCaptionChange)
	ON_MESSAGE(WM_UPDATE_VI_TACT, &CFormAuto::OnUpdateVITact)
	ON_MESSAGE(WM_UPDATE_ALIGN_COUNT, &CFormAuto::OnUpdateAlignCount)
	ON_MESSAGE(WM_UPDATE_PROCESS, &CFormAuto::OnUpdateProcess)
	ON_MESSAGE(WM_UPDATE_INSPECT_COUNT, &CFormAuto::OnUpdateInspectCount)
	ON_MESSAGE(WM_CHANGE_USER, &CFormAuto::OnChangeUser)
	//////////////////////////////////////////////////////////////////////////
	ON_BN_CLICKED(IDC_BTN_AUTO_MODEL_LOADING, &CFormAuto::OnNewModelLoading)
	ON_BN_CLICKED(IDC_BTN_AUTO_INITIALIZE, &CFormAuto::OnSystemInitialize)
	ON_BN_CLICKED(IDC_RADIO_AUTO_LIVE, &CFormAuto::OnCCDImageLive)
	ON_BN_CLICKED(IDC_RADIO_AUTO_HALT, &CFormAuto::OnCCDImageHalt)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_AUTO_START, &CFormAuto::OnBnClickedBtnAutoStart)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_AUTO_PM_MODE, &CFormAuto::OnBnClickedBtnAutoPmMode)
	ON_BN_CLICKED(IDC_BTN_AUTO_STOP, &CFormAuto::OnBnClickedBtnAutoStop)
	ON_BN_CLICKED(IDC_RADIO_AUTO_NORMAL, &CFormAuto::OnBnClickedRadioAutoNormal)
	ON_BN_CLICKED(IDC_RADIO_AUTO_STAGE, &CFormAuto::OnBnClickedRadioAutoStage)
	ON_BN_CLICKED(IDC_RADIO_AUTO_ALIGN_SKIP, &CFormAuto::OnBnClickedRadioAutoAlignSkip)
	ON_BN_CLICKED(IDC_RADIO_AUTO_ALIGN_VI_SKIP, &CFormAuto::OnBnClickedRadioAutoAlignViSkip)
	ON_BN_CLICKED(IDC_BTN_AUTO_APPLY_PROCESS_INITIALIZE, &CFormAuto::OnBnClickedBtnAutoApplyProcessInitialize)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_AUTO_CAM0_COUNT_RESET, &CFormAuto::OnBnClickedBtnAutoCam0CountReset)
	ON_BN_CLICKED(IDC_BTN_AUTO_CAM1_COUNT_RESET, &CFormAuto::OnBnClickedBtnAutoCam1CountReset)
	ON_BN_CLICKED(IDC_BTN_AUTO_CAM2_COUNT_RESET, &CFormAuto::OnBnClickedBtnAutoCam2CountReset)
	ON_BN_CLICKED(IDC_BTN_AUTO_CAM3_COUNT_RESET, &CFormAuto::OnBnClickedBtnAutoCam3CountReset)
	ON_BN_CLICKED(IDC_BUTTON_COUNT_RESET, &CFormAuto::OnBnClickedButtonCountReset)
	ON_BN_CLICKED(IDC_BTN_PG_START, &CFormAuto::OnBnClickedBtnPgStart)
	ON_BN_CLICKED(IDC_BTN_PG_STOP, &CFormAuto::OnBnClickedBtnPgStop)
	ON_BN_CLICKED(IDC_BTN_AUTO_RESET, &CFormAuto::OnBnClickedBtnAutoReset)
END_MESSAGE_MAP()


// CFormAuto 진단입니다.

#ifdef _DEBUG
void CFormAuto::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormAuto::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CFormAuto 메시지 처리기입니다.

BOOL CFormAuto::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.


	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void CFormAuto::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	GetAlignCtl()->GetVision()->SelAutoWindow( m_pctImageGrap[CAM_0].m_hWnd, m_pctImageGrap[CAM_1].m_hWnd, 
		m_pctImageGrap[CAM_2].m_hWnd, m_pctImageGrap[CAM_3].m_hWnd	);

	m_ctlInspection_Counter.put_Value((long)GetDoc()->m_nInspectionCount); //  [9/10/2009 janghan]

	UpdateDisplayData();

	OnChangeUser(NULL, NULL);
}



void CFormAuto::UpdateDisplayData(void)
{
	m_lbCurMoDelName.put_Caption(GetDoc()->m_pAmtData->m_CurModelInfo.strModelName);
	switch (GetApp()->m_RunMode)
	{
	case MODE_NORMAL:
		m_lblRunMode.put_Caption(TXT_MODE_NORMAL);
		break;
	case MODE_STAGE:
		m_lblRunMode.put_Caption(TXT_MODE_STAGE);
		break;
	case MODE_ALIGN_SKIP:
		m_lblRunMode.put_Caption(TXT_MODE_ALIGN_SKIP);
		break;
	case MODE_VISION_SKIP:
		m_lblRunMode.put_Caption(TXT_MODE_VISION_SKIP);
		break;
	}
	m_btnAuto[BTN_NORMAL+GetApp()->m_RunMode].SetCheck(BST_CHECKED);
	UpdateData(FALSE);
}

void CFormAuto::OnNewModelLoading()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CDlgModelChange dlgModel;
	dlgModel.DoModal();

	UpdateDisplayData();
}

void CFormAuto::OnSystemInitialize()
{
	GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;
	CDlgInitial InitDlg;
	InitDlg.DoModal();
	if(GetApp()->m_RunStatus == STATUS_INIT)
		GetApp()->ChangeStatus(STATUS_ERROR);
	GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;

	GetDoc()->m_bAutoTune_RUN = FALSE;//  [6/11/2009 janghan]
	GetDoc()->m_nYaxis_Back = 99;//  [6/11/2009 janghan]
	GetDoc()->m_fAutoDistance_X =  0.; //  [6/15/2009 janghan]
	GetDoc()->m_fPCBmarkFind_X = 0.; //  [6/15/2009 janghan]
	GetDoc()->m_fMarkFind_X = 0.; //  [6/15/2009 janghan]
	GetDoc()->m_strAutoTune_Reason = "";//  [6/21/2009 janghan]

	GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]
}


void CFormAuto::OnCCDImageLive()
{
	SeImageLiveView();
}

void CFormAuto::OnCCDImageHalt()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bLive = FALSE;
	GetDoc()->GrapAliveFlagAllStop();
}

void CFormAuto::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFormView::OnShowWindow(bShow, nStatus);

	if( bShow )
	{
		if(m_bLive)
			SeImageLiveView();
		SetTimer(TIMER_CONNECTION, 500, NULL);
		m_swxAutoTopLight.put_Active(GetDoc()->m_bLightOn[id_OMIT]);
		m_swxAutoBtmLight.put_Active(GetDoc()->m_bLightOn[id_BtmLight]);
	}
	else
	{
		GetDoc()->GrapAliveFlagAllStop();
		KillTimer(TIMER_CONNECTION);
	}
}

void CFormAuto::SeImageLiveView(void)
{
	//////////////////////////////////////////////////////////////////////////
	// FormAuto Cam Live 구동 함수

	GetDoc()->GrapAliveFlagAllStop();
	GetAlignCtl()->OverlayClear(DISP_AUTO_0);
	GetAlignCtl()->OverlayClear(DISP_AUTO_1);
	GetAlignCtl()->OverlayClear(DISP_AUTO_2);
	GetAlignCtl()->OverlayClear(DISP_AUTO_3);

	if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::ONE_CONT && GetDoc()->m_pAmtData->m_CurModelInfo.iContactType == CAmtData::PIN_TYPE )
	{
		GetDoc()->m_bLiveGrapFlag[CAM_2] = TRUE;
		GetAlignCtl()->GrabNotUseTextDisp(DISP_AUTO_0);
		GetAlignCtl()->GrabNotUseTextDisp(DISP_AUTO_1);
		GetAlignCtl()->GrabNotUseTextDisp(DISP_AUTO_3);
	}
	else
	{
		if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::ONE_CONT )
		{
			if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_0 )
			{
				GetDoc()->m_bLiveGrapFlag[CAM_0] = TRUE ;
				GetAlignCtl()->GrabNotUseTextDisp(DISP_AUTO_1);
				GetAlignCtl()->GrabNotUseTextDisp(DISP_AUTO_2);
				GetAlignCtl()->GrabNotUseTextDisp(DISP_AUTO_3);

			}
			else	if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_1 )
			{
				GetDoc()->m_bLiveGrapFlag[CAM_1] = TRUE ;
				GetAlignCtl()->GrabNotUseTextDisp(DISP_AUTO_0);
				GetAlignCtl()->GrabNotUseTextDisp(DISP_AUTO_2);
				GetAlignCtl()->GrabNotUseTextDisp(DISP_AUTO_3);
			}
			else	if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_2 )
			{
				GetDoc()->m_bLiveGrapFlag[CAM_2] = TRUE ;
				GetAlignCtl()->GrabNotUseTextDisp(DISP_AUTO_0);
				GetAlignCtl()->GrabNotUseTextDisp(DISP_AUTO_1);
				GetAlignCtl()->GrabNotUseTextDisp(DISP_AUTO_3);
			}
			else	if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_3 )
			{
				GetDoc()->m_bLiveGrapFlag[CAM_3] = TRUE ;
				GetAlignCtl()->GrabNotUseTextDisp(DISP_AUTO_0);
				GetAlignCtl()->GrabNotUseTextDisp(DISP_AUTO_1);
				GetAlignCtl()->GrabNotUseTextDisp(DISP_AUTO_2);
			}		}
		else if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::TWO_CONT )
		{
			GetAlignCtl()->GrabNotUseTextDisp(DISP_AUTO_0);
			GetAlignCtl()->GrabNotUseTextDisp(DISP_AUTO_3);
			GetDoc()->m_bLiveGrapFlag[CAM_1] = TRUE ;
			GetDoc()->m_bLiveGrapFlag[CAM_2] = TRUE ;
		}
		else
		{
			GetDoc()->m_bLiveGrapFlag[CAM_0] = TRUE ;
			GetDoc()->m_bLiveGrapFlag[CAM_1] = TRUE ;
			GetDoc()->m_bLiveGrapFlag[CAM_2] = TRUE ;
			GetDoc()->m_bLiveGrapFlag[CAM_3] = TRUE ;
		}
	}
	m_bLive = TRUE;
}

void CFormAuto::OnBnClickedBtnAutoStart()
{
	GetDoc()->SetLog("프로그램 Start 버튼 누름",LOG_COMM);
	GetApp()->Start();
}

void CFormAuto::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CFormView::OnClose();
}

void CFormAuto::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: Add your message handler code here
	GetAlignCtl()->GetVision()->DeselAutoWindow();
}

void CFormAuto::OnBnClickedBtnAutoPmMode()
{
}

void CFormAuto::OnBnClickedBtnAutoStop()
{
	GetDoc()->SetLog("프로그램 Stop 버튼 누름",LOG_COMM);
	GetApp()->Stop();
}

void CFormAuto::OnBnClickedRadioAutoNormal()
{
	GetApp()->ChangeRunMode(MODE_NORMAL);
	UpdateDisplayData();
}

void CFormAuto::OnBnClickedRadioAutoStage()
{
	GetApp()->ChangeRunMode(MODE_STAGE);
	UpdateDisplayData();
}

void CFormAuto::OnBnClickedRadioAutoAlignSkip()
{
	GetApp()->ChangeRunMode(MODE_ALIGN_SKIP);
	UpdateDisplayData();
}

void CFormAuto::OnBnClickedRadioAutoAlignViSkip()
{
	GetApp()->ChangeRunMode(MODE_VISION_SKIP);
	UpdateDisplayData();
}

void CFormAuto::OnBnClickedBtnAutoApplyProcessInitialize()
{
	if(GetApp()->IsRunStatus() == FALSE)
		GetDoc()->m_pEqMainSeq->ProcessInit();
}
BEGIN_EVENTSINK_MAP(CFormAuto, CFormView)
	ON_EVENT(CFormAuto, IDC_ISWITCHLEDX_AUTO_LOADING_LOCK, 5, CFormAuto::OnClickIswitchledxAutoLoadingLock, VTS_NONE)
	ON_EVENT(CFormAuto, IDC_ISWITCHLEDX_AUTO_BOTTOM_LIGHT, 5, CFormAuto::OnClickIswitchledxAutoBottomLight, VTS_NONE)
	ON_EVENT(CFormAuto, IDC_ISWITCHLEDX_AUTO_TOP_LIGHT, 5, CFormAuto::OnClickIswitchledxAutoTopLight, VTS_NONE)
END_EVENTSINK_MAP()

void CFormAuto::OnClickIswitchledxAutoLoadingLock()
{
}

LPARAM CFormAuto::OnLabelCaptionChange(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case STATUS_READY:
		m_lblRun_State.put_Caption("READY");
		break;
		
	case STATUS_INIT:
		m_lblRun_State.put_Caption("INIT");
		break;

	case STATUS_RUN:
		m_lblRun_State.put_Caption("RUN");
		break;

	case STATUS_STOP:
		m_lblRun_State.put_Caption("STOP");
		break;

	case STATUS_ERROR:
		m_lblRun_State.put_Caption("ERROR");
		break;

	case STATUS_EMO:
		m_lblRun_State.put_Caption("EMO");
		break;

	case STATUS_OPCALL:
		m_lblRun_State.put_Caption("CALL");
		break;
	}
	return 0L;
}

void CFormAuto::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == TIMER_CONNECTION)
	{
		CString strSocketName = SocketName_LC;
		CDataSocket* pSocket = &(GetComm()->m_SocketLC);
		if(m_bConnectionLC != pSocket->IsConnected())
		{
			m_bConnectionLC = pSocket->IsConnected();
			if(m_bConnectionLC)
			{
				m_lblConnectionLC.put_Caption(strSocketName + " ONLINE");
				m_lblConnectionLC.put_ForeColor(Color(Color::Lime).ToCOLORREF());
			}
			else
			{
				m_lblConnectionLC.put_Caption(strSocketName + " OFFLINE");
				m_lblConnectionLC.put_ForeColor(Color(Color::Red).ToCOLORREF());
			}
		}
		if(GetMachinePCType() == A_AMT || GetMachinePCType() == B_AMT)
		{
			pSocket = &(GetComm()->m_SocketVI);
			strSocketName = SocketName_VI;
		}
		else
		{
			pSocket = &(GetComm()->m_SocketPG);
			strSocketName = SocketName_PG;
		}
		if(m_bConnectionVI != pSocket->IsConnected())
		{
			m_bConnectionVI = pSocket->IsConnected();
			if(m_bConnectionVI)
			{
				m_lblConnectionVI.put_Caption(strSocketName + " ONLINE");
				m_lblConnectionVI.put_ForeColor(Color(Color::Lime).ToCOLORREF());
			}
			else
			{
				m_lblConnectionVI.put_Caption(strSocketName + " OFFLINE");
				m_lblConnectionVI.put_ForeColor(Color(Color::Red).ToCOLORREF());
			}
		}
	}

	SYSTEMTIME	time;//  [9/10/2009 janghan]
	GetLocalTime(&time);//  [9/10/2009 janghan]

	if(GetDoc()->m_nHourCheck != time.wHour)//  [9/10/2009 janghan]
	{
		OnBnClickedButtonCountReset();//매 1시간씩 정각에 Reset하고 Log File 로 저장한다.
		GetDoc()->m_nHourCheck = time.wHour;

	}

	CFormView::OnTimer(nIDEvent);
}

LPARAM CFormAuto::OnUpdateVITact(WPARAM wParam, LPARAM lParam)
{
	CString strCaption;
	strCaption.Format("%6.2f sec", (double)lParam);
	m_lblPostGrabTact.put_Caption(strCaption);
	return 0L;
}

LPARAM CFormAuto::OnUpdateAlignCount(WPARAM wParam, LPARAM lParam)
{
	if(GetDoc()->m_bAutoTune_RUN == FALSE)
	{
		int nCamIndex = (int)wParam;
		long count = m_segCamCount[nCamIndex].get_Value();
		m_segCamCount[nCamIndex].put_Value(count+1);
	}
	return 0L;
}

LPARAM CFormAuto::OnUpdateInspectCount(WPARAM wParam, LPARAM lParam)
{
	long count = GetDoc()->m_nInspectionCount;
	m_ctlInspection_Counter.put_Value(count+1);

	return 0L;
}

LPARAM CFormAuto::OnChangeUser(WPARAM wParam, LPARAM lParam)
{
	if(GetDoc()->m_bEngLogin || GetDoc()->m_bSuperLogin)
		m_btnAuto[BTN_ALIGN_VI_SKIP].EnableWindow(TRUE);
	else
		m_btnAuto[BTN_ALIGN_VI_SKIP].EnableWindow(FALSE);
	return 0L;
}

void CFormAuto::OnBnClickedBtnAutoCam0CountReset()
{
	m_segCamCount[CAM_0].put_Value(0);
}

void CFormAuto::OnBnClickedBtnAutoCam1CountReset()
{
	m_segCamCount[CAM_1].put_Value(0);
}

void CFormAuto::OnBnClickedBtnAutoCam2CountReset()
{
	m_segCamCount[CAM_2].put_Value(0);
}

void CFormAuto::OnBnClickedBtnAutoCam3CountReset()
{
	m_segCamCount[CAM_3].put_Value(0);
}

LPARAM CFormAuto::OnUpdateProcess( WPARAM wParam, LPARAM lParam )
{
	int nProcessNo = (int)wParam;
	COLORREF procColor;
	for(int i = 0; i < ProcMax; i++)
	{
		if(i == nProcessNo)
			procColor = Color(Color::Lime).ToCOLORREF();
		else
			procColor = Color(Color::DarkGreen).ToCOLORREF();

		m_lblRunProcess[i].put_ForeColor(procColor);
		m_lblRunProcess[i].put_BorderColor(procColor);
	}
	return 0L;
}
void CFormAuto::OnClickIswitchledxAutoBottomLight()
{
	if(m_swxAutoBtmLight.get_Active())
		GetDoc()->BtmOn();
	else
		GetDoc()->BtmOff();
}

void CFormAuto::OnClickIswitchledxAutoTopLight()
{
	if(m_swxAutoTopLight.get_Active())
		GetDoc()->OmitOff();
	else
		GetDoc()->OmitOn();
}

void CFormAuto::OnBnClickedButtonCountReset()
{
	CString tempString;

	tempString.Format("%d, Count Reset !!", GetDoc()->m_nInspectionCount );
	GetDoc()->SetLog(tempString, LOG_Reset_Count);

	m_ctlInspection_Counter.put_Value(0);
	GetDoc()->m_nInspectionCount = 0;

	GetDoc()->SaveInspectionCount();
}

void CFormAuto::OnBnClickedBtnPgStart()
{
	// TODO: Add your control notification handler code here
	GetDoc()->m_pEqMainSeq->m_bPGStart = TRUE;
	GetDoc()->m_pEqMainSeq->SeqCellContProc();

}

void CFormAuto::OnBnClickedBtnPgStop()
{
	// TODO: Add your control notification handler code here
	GetDoc()->m_pEqMainSeq->m_bPGStop = TRUE;
	GetDoc()->m_pEqMainSeq->SeqCellContProc();

}

void CFormAuto::OnBnClickedBtnAutoReset()
{
	GetDoc()->SetLog("프로그램 RESET 버튼 누름",LOG_COMM);
	GetApp()->Reset();
	// TODO: Add your control notification handler code here
}
