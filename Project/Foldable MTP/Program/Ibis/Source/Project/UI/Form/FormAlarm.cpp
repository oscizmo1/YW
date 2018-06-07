// CFormAlarm1.cpp : 구현 파일입니다.
#include "stdafx.h"
#include "FormAlarm.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
//kjpark 20161021 Setup 신호기 UI 구현
#include "UI\GausGUI\GxMsgBox.h"
#include "UI\GausGUI\GxStaticEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CFormAlarm1

IMPLEMENT_DYNCREATE(CFormAlarm, CFormView)

CFormAlarm::CFormAlarm()
	: CFormView(CFormAlarm::IDD)
{

}

CFormAlarm::~CFormAlarm()
{
}

void CFormAlarm::ViewAlarmMsg()
{
	CGxStaticEx* pstcAlarmName = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_ALARM_NAME);
	CGxStaticEx* pstcAlarmTime = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_ALARM_TIME);
	CGxStaticEx* pstcAlarmId = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_ALARM_ID);
	CGxStaticEx* pstcAlarmLevel = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_ALARM_LEVEL);
	CGxStaticEx* pstcAlarmPart = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_ALARM_PART);
	CGxStaticEx* pstcAlarmCause_KOR = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_ALARM_CAUSE_1);
	CGxStaticEx* pstcAlarmCause_ENG = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_ALARM_ITEM8);
	CGxStaticEx* pstcAlarmCause_VTN = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_ALARM_ITEM10);
	
	CGxStaticEx* pstcAlarmMethod_KOR = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_ALARM_METHOD_1);
	CGxStaticEx* pstcAlarmMethod_ENG = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_ALARM_ITEM12);
	CGxStaticEx* pstcAlarmMethod_VTN = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_ALARM_ITEM13);

	if(theProcBank.m_AlarmID != ALM_NONE)
	{
		CString strCaption;

		pstcAlarmName->SetCaption(theProcBank.m_AlarmMsg[theProcBank.m_AlarmID].strName[GetMainHandler()->m_nLangIdx]);

		strCaption.Format(_T("%04d-%02d-%02d %02d:%02d:%02d.%03d"), 
			theProcBank.m_AlarmTime.wYear, theProcBank.m_AlarmTime.wMonth, theProcBank.m_AlarmTime.wDay,
			theProcBank.m_AlarmTime.wHour, theProcBank.m_AlarmTime.wMinute, theProcBank.m_AlarmTime.wSecond,
			theProcBank.m_AlarmTime.wMilliseconds);
		pstcAlarmTime->SetCaption(strCaption);

		strCaption.Format(_T("%d"), theProcBank.m_AlarmID);
		pstcAlarmId->SetCaption(strCaption);

		strCaption.Format(_T("%d"), theProcBank.m_AlarmMsg[theProcBank.m_AlarmID].nAlarmLevel);
		pstcAlarmLevel->SetCaption(strCaption);

		pstcAlarmPart->SetCaption(ConstEquipPart[theProcBank.GetEqiupPart_to_AlarmID(theProcBank.m_AlarmID)].strNameKor);

		//pstcAlarmCause_KR->SetCaption(theProcBank.m_AlarmMsg[theProcBank.m_AlarmID].strCause[GetMainHandler()->m_nLangIdx]);
		pstcAlarmCause_KOR->SetCaption(theProcBank.m_AlarmMsg[theProcBank.m_AlarmID].strCause[LAN_KOR]);
		pstcAlarmCause_ENG->SetCaption(theProcBank.m_AlarmMsg[theProcBank.m_AlarmID].strCause[LAN_ENG]);
		pstcAlarmCause_VTN->SetCaption(theProcBank.m_AlarmMsg[theProcBank.m_AlarmID].strCause[LAN_VNM]);

		pstcAlarmMethod_KOR->SetCaption(theProcBank.m_AlarmMsg[theProcBank.m_AlarmID].strMethod[LAN_KOR]);
		pstcAlarmMethod_ENG->SetCaption(theProcBank.m_AlarmMsg[theProcBank.m_AlarmID].strMethod[LAN_ENG]);
		pstcAlarmMethod_VTN->SetCaption(theProcBank.m_AlarmMsg[theProcBank.m_AlarmID].strMethod[LAN_VNM]);
	}
// 	else
// 	{
// 		pstcAlarmName->SetCaption(_T(""));
// 		pstcAlarmTime->SetCaption(_T(""));
// 		pstcAlarmId->SetCaption(_T(""));
// 		pstcAlarmLevel->SetCaption(_T(""));
// 		pstcAlarmPart->SetCaption(_T(""));
// 		pstcAlarmCause->SetCaption(_T(""));
// 		pstcAlarmMethod->SetCaption(_T(""));
// 	}
}

void CFormAlarm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormAlarm, CFormView)
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)				// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
END_MESSAGE_MAP()

//kjpark 20161021 Setup 신호기 UI 구현
BEGIN_EVENTSINK_MAP(CFormAlarm, CFormView)
	ON_EVENT(CFormAlarm, IDC_GXBTN_ALARM_RESET, DISPID_CLICK, CFormAlarm::ClickBtnAlarm_Reset, VTS_NONE)
	ON_EVENT(CFormAlarm, IDC_GXBTN_ALARM_RETRY, DISPID_CLICK, CFormAlarm::ClickBtnAlarm_Retry, VTS_NONE)
	ON_EVENT(CFormAlarm, IDC_GXBTN_ALARM_MUTE, DISPID_CLICK, CFormAlarm::ClickBtnAlarm_Buzzer_Mute, VTS_NONE)
	 
END_EVENTSINK_MAP()

// CFormAlarm1 진단입니다.

#ifdef _DEBUG
void CFormAlarm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormAlarm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG





// CFormAlarm1 메시지 처리기입니다.

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
LRESULT CFormAlarm::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		ViewAlarmMsg();


// 		if ( !m_imgUnit.GetImageLoaded() )		// 이미지를 로드하지 않았다면 파일을 불러온다.
// 		{
// 			CString sFilePath;
// 			char szProgramPath[_MAX_PATH]; 
// 
// 			// 파일 경로 지정
// 			GetModuleFileNameA( ::AfxGetInstanceHandle(), szProgramPath, _MAX_PATH);		// 실행파일 경로는 실행파일까지 들어간 Full Path
// 			sFilePath	= szProgramPath;									// 현재 실행중인 파일 이름
// 
// 			int nPos = sFilePath.ReverseFind('\\');						// 끝에서부터 \\ 구분자를 찾는다.
// 
// 			// 현재 폴더 경로
// 			sFilePath	= sFilePath.Left( nPos + 1 ) + _T("RecipeView.png");									// 현재 실행중인 파일의 경로
// 
// 			// m_imgUnit.LoadImage(_T("D:\\Test\\RecipeView.png"));
// 			m_imgUnit.LoadImage( sFilePath );
// 		}
	}
	else
	{

	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CFormAlarm::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	 CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_GXLBL_ALARM_CAP1);
	 CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXBTN_ALARM_RESET);
	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormAlarm::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}

//kjpark 20161021 Setup 신호기 UI 구현
void CFormAlarm::ClickBtnAlarm_Reset()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	theLossTimeLog.SetMachineResetTime();
	theProcBank.AlarmClear();
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("ALARM"),_T("RESET"));

}

//kjpark 20161021 Setup 신호기 UI 구현
void CFormAlarm::ClickBtnAlarm_Retry()
{
// 	CGxMsgBox	dlgMsgBox;
// 	// STO 발생 상태인가 ?
// 	if ((theUnitFunc.LightCurtainOnCheck(X_INSP_B_ZONE_LC) == TRUE ||theUnitFunc.LightCurtainOnCheck(X_INSP_D_ZONE_LC) == TRUE )
// 		&& GetMainHandler()->GetSTODetected() == TRUE)		// 라이트 커튼이 감지되어 STO가 발동된 상태라면,
// 	{
// 		dlgMsgBox.SetTitle(_T("STO 초기화 확인"), _T("STO initialization"), _T("STO khởi tạo"));
// 		dlgMsgBox.SetMessage(FALSE, _T("RESET 버튼을 눌러 STO 초기화를 실시해주십시오."), _T("Press the RESET button on the STO initialization"), _T("Nhấn nút RESET trên khởi STO"), GetMainHandler()->GetLanguageType());
// 		dlgMsgBox.DoModal();
// 
// 		return;		// 실행 취소 
// 	}
// 
// 	// 만약 중요 Alarm에 의해 DOWN (Abort) 된 상태라면, 자동 Retry가 되지 않는다.	
// 	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
// 	{
// 		theUnitFunc.SetOutPutIO(Y_BUZZER_K1, OFF);
// 		theUnitFunc.SetOutPutIO(Y_BUZZER_K2, OFF);
// 		theUnitFunc.SetOutPutIO(Y_BUZZER_K3, OFF);
// 		theUnitFunc.SetOutPutIO(Y_BUZZER_K4, OFF);
// 
// 		
// 		dlgMsgBox.SetTitle(_T("알림"), _T("Notifications"), _T("thông báo"));
// 		dlgMsgBox.SetMessage(FALSE, _T("초기화를 수행하십시오."), _T("Perform the initialization."), _T("Thực hiện việc khởi tạo."),  GetMainHandler()->GetLanguageType());
// 		dlgMsgBox.DoModal();
// 	}
// 	theProcBank.AlarmClear();

#if 0
	// 비가동 사유를 자동으로 입력시켜준다.
	if ( pMCMain->m_rLosstimeRecord.sAlarmType	!= _T("-") )		// Alarm에 의해 멈추었다면, 
	{
		// Alarm에 의해 멈추었다면 자동으로 내용을 입력한다.
		::GetLocalTime( &(pMCMain->m_rLosstimeRecord.tmResetTime) );	// Alarm Reset 시각지정
		::GetLocalTime( &(pMCMain->m_rLosstimeRecord.tmEndTime) );		// Alarm 처리 종료하고 다시 시작 시간

		pMCMain->m_rLosstimeRecord.s1stClass = _T("MD");
		pMCMain->m_rLosstimeRecord.s2ndClass = _T("ETC");
		// 지정된 Alarm명 사용
		// pMCMain->m_rLosstimeRecord.s3rdClass = _T("EQ RETRY");
		pMCMain->m_rLosstimeRecord.sRepairCode = _T("999");


		// 파일로 기록을 한다.
		//old m_pMCMain->WriteLossTimeRecord( m_pMCMain->m_rLosstimeRecord, FALSE );			 
		m_pMCMain->WriteLossTimeRecord( m_pMCMain->m_rLosstimeRecord, FALSE );			 
		pMCMain->ClearLosstimeInfo();														// 비가동 임시기억 초기화

		::PostMessage( ( (CMainFrame*)AfxGetMainWnd() )->m_hWnd, GXMSG_REQUEST_START, (WPARAM)1, (LPARAM)0);
		return;
	}
#endif
}

//kjpark 20161021 Setup 신호기 UI 구현
void CFormAlarm::ClickBtnAlarm_Buzzer_Mute()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("ALARM"),_T("BUZZER MUTE"));
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	theUnitFunc.SetOutPutIO(Y_BUZZER_K1, OFF);
	theUnitFunc.SetOutPutIO(Y_BUZZER_K2, OFF);
	theUnitFunc.SetOutPutIO(Y_BUZZER_K3, OFF);
	theUnitFunc.SetOutPutIO(Y_BUZZER_K4, OFF);
}
