// FormReport1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormReport1.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxStaticEx.h"
#include "UI\GausGUI\GxButtonEx.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CFormReport1

IMPLEMENT_DYNCREATE(CFormReport1, CFormView)

CFormReport1::CFormReport1()
	: CFormView(CFormReport1::IDD)
{
	m_iPart = DAY;
	Init();
}

CFormReport1::~CFormReport1()
{
}

void CFormReport1::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormReport1, CFormView)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

END_MESSAGE_MAP()


// CFormReport1 진단입니다.

#ifdef _DEBUG
void CFormReport1::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormReport1::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CFormReport1::Init()		//	시간 설정
{
	GetLocalTime(&m_CurDate);

	CTime cTime(m_CurDate);

	CEtc::ApplyTimeOffset(cTime, theConfigBank.m_Option.m_OffsetHour, 0);
	
	m_CurDate.wYear = (WORD)cTime.GetYear();
	m_CurDate.wMonth = (WORD)cTime.GetMonth();
	m_CurDate.wDay = (WORD)cTime.GetDay();

	m_szCurrent_time.Format(_T("%04u-%02u-%02u"),m_CurDate.wYear, m_CurDate.wMonth, m_CurDate.wDay);

	
}

void CFormReport1::UpdateUI()			//	UI화면을 UPDATE 해줌
{

}


void CFormReport1::SetStaticUIString(ProductInfo tInfo)			//	ProductInfo 의 정보를 static 에 셋팅해주는 함수
{

}

void CFormReport1::SetStaticUIString(ProductInfo tInfo, UINT IDCount, UINT IDDefect, UINT IDReject, UINT IDPerMCR, UINT IDPerAlign, UINT IDCont)			//	ProductInfo 의 정보를 static 에 셋팅해주는 함수, ID를 입력받는다
{

}

// CFormReport1 메시지 처리기입니다.


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
LRESULT CFormReport1::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )																//	Report2 화면으로 들어올떄
	{
		m_ProductBank.LoadReportFile(m_szCurrent_time);
		UpdateUI();
	}
	else																		//	Report2 화면을 떠날때
	{

	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CFormReport1::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	// CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_BOTTOM_MENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormReport1::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}
BEGIN_EVENTSINK_MAP(CFormReport1, CFormView)
	ON_EVENT(CFormReport1, IDC_GXLBL_REPORT_DAY, DISPID_CLICK, CFormReport1::ClickDay, VTS_NONE)
	ON_EVENT(CFormReport1, IDC_GXLBL_REPORT_SW, DISPID_CLICK, CFormReport1::ClickSW, VTS_NONE)
	ON_EVENT(CFormReport1, IDC_GXLBL_REPORT_GY, DISPID_CLICK, CFormReport1::ClickGY, VTS_NONE)
	ON_EVENT(CFormReport1, IDC_GXLBL_REPORT_PREV, DISPID_CLICK, CFormReport1::ClickPrev, VTS_NONE)
	ON_EVENT(CFormReport1, IDC_GXLBL_REPORT_NEXT, DISPID_CLICK, CFormReport1::ClickNext, VTS_NONE)
	ON_EVENT(CFormReport1, IDC_GXLBL_REPORT_TODAY, DISPID_CLICK, CFormReport1::ClickGxlblReportToday, VTS_NONE)
END_EVENTSINK_MAP()

void CFormReport1::ClickDay()		//	DAY 버튼 클릭시
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"), _T("DAY"));
	m_iPart = DAY;
	UpdateUI();
	
}


void CFormReport1::ClickSW()		//	SW 버튼 클릭시
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"), _T("SW"));
	m_iPart = SW;
	UpdateUI();
}


void CFormReport1::ClickGY()		//GY 버튼 클릭시
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("GY"));
	m_iPart = GY;
	UpdateUI();
}



void CFormReport1::ClickPrev()		//	날짜 PREV 클릭시
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("PREV"));
	SetDateTime(-1);

	m_ProductBank.LoadReportFile(m_szCurrent_time.GetBuffer());
	UpdateUI();
}


void CFormReport1::ClickNext()		// 날짜 NEXT 클릭시
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("NEXT"));
	SetDateTime(+1);

	m_ProductBank.LoadReportFile(m_szCurrent_time.GetBuffer());
	UpdateUI();
}


void CFormReport1::SetDateTime(int iValue, BOOL btoday)		//	PREV나 NEXT 클릭시 날짜를 바꿔줌
{
	SYSTEMTIME today;
	GetLocalTime(&today);
	if(btoday)
	{
		GetLocalTime(&m_CurDate);
	}
	CTime Compare1(m_CurDate);
	CTime Compare2(today);

	CEtc::ApplyTimeOffset(Compare2, theConfigBank.m_Option.m_OffsetHour, 0);

	Compare1 += (CTimeSpan(iValue, 0, 0, 0));
	
	if ( Compare1 > Compare2 )
		return;

	CTime CalcTime(m_CurDate);
	CalcTime += CTimeSpan(iValue,0,0,0);
	m_CurDate.wYear = (WORD)CalcTime.GetYear();
	m_CurDate.wMonth = (WORD)CalcTime.GetMonth();
	m_CurDate.wDay = (WORD)CalcTime.GetDay();
			
	m_szCurrent_time.Format(_T("%04u-%02u-%02u"),m_CurDate.wYear, m_CurDate.wMonth, m_CurDate.wDay);
}

void CFormReport1::ClickGxlblReportToday()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("TODAY"));
	SetDateTime(0, TRUE);
	m_ProductBank.LoadReportFile(m_szCurrent_time.GetBuffer());
	UpdateUI();
}

void CFormReport1::ClickContact01()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FIRST CONTACT01"));
	if(m_iPart == DAY)
		DisplayContactDlg(7, TRUE);
	else if(m_iPart == GY)
		DisplayContactDlg(23, TRUE);
	else if(m_iPart == SW)
		DisplayContactDlg(15, TRUE);
}
void CFormReport1::ClickContact02()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FIRST CONTACT02"));
	if(m_iPart == DAY)
		DisplayContactDlg(8, TRUE);
	else if(m_iPart == GY)
		DisplayContactDlg(0, TRUE);
	else if(m_iPart == SW)
		DisplayContactDlg(16, TRUE);
}
void CFormReport1::ClickContact03()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FIRST CONTACT03"));
	if(m_iPart == DAY)
		DisplayContactDlg(9, TRUE);
	else if(m_iPart == GY)
		DisplayContactDlg(1, TRUE);
	else if(m_iPart == SW)
		DisplayContactDlg(17, TRUE);
}
void CFormReport1::ClickContact04()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FIRST CONTACT04"));
	if(m_iPart == DAY)
		DisplayContactDlg(10, TRUE);
	else if(m_iPart == GY)
		DisplayContactDlg(2, TRUE);
	else if(m_iPart == SW)
		DisplayContactDlg(18, TRUE);
}
void CFormReport1::ClickContact05()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FIRST CONTACT05"));
	if(m_iPart == DAY)
		DisplayContactDlg(11, TRUE);
	else if(m_iPart == GY)
		DisplayContactDlg(3, TRUE);
	else if(m_iPart == SW)
		DisplayContactDlg(19, TRUE);
}
void CFormReport1::ClickContact06()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FIRST CONTACT06"));
	if(m_iPart == DAY)
		DisplayContactDlg(12, TRUE);
	else if(m_iPart == GY)
		DisplayContactDlg(4, TRUE);
	else if(m_iPart == SW)
		DisplayContactDlg(20, TRUE);
}
void CFormReport1::ClickContact07()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FIRST CONTACT07"));
	if(m_iPart == DAY)
		DisplayContactDlg(13, TRUE);
	else if(m_iPart == GY)
		DisplayContactDlg(5, TRUE);
	else if(m_iPart == SW)
		DisplayContactDlg(21, TRUE);
}
void CFormReport1::ClickContact08()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FIRST CONTACT08"));
	if(m_iPart == DAY)
		DisplayContactDlg(14, TRUE);
	else if(m_iPart == GY)
		DisplayContactDlg(6, TRUE);
	else if(m_iPart == SW)
		DisplayContactDlg(22, TRUE);
}
void CFormReport1::ClickContactTotal()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FIRST CONTACT TOTAL"));
	DisplayContactDlg(0,23, TRUE);
}

void CFormReport1::ClickContactFinal01()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FINAL CONTACT01"));
	if(m_iPart == DAY)
		DisplayContactDlg(7, FALSE);
	else if(m_iPart == GY)
		DisplayContactDlg(23, FALSE);
	else if(m_iPart == SW)
		DisplayContactDlg(15, FALSE);
}
void CFormReport1::ClickContactFinal02()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FINAL CONTACT02"));
	if(m_iPart == DAY)
		DisplayContactDlg(8, FALSE);
	else if(m_iPart == GY)
		DisplayContactDlg(0, FALSE);
	else if(m_iPart == SW)
		DisplayContactDlg(16, FALSE);
}
void CFormReport1::ClickContactFinal03()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FINAL CONTACT03"));
	if(m_iPart == DAY)
		DisplayContactDlg(9, FALSE);
	else if(m_iPart == GY)
		DisplayContactDlg(1, FALSE);
	else if(m_iPart == SW)
		DisplayContactDlg(17, FALSE);
}
void CFormReport1::ClickContactFinal04()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FINAL CONTACT04"));
	if(m_iPart == DAY)
		DisplayContactDlg(10, FALSE);
	else if(m_iPart == GY)
		DisplayContactDlg(2, FALSE);
	else if(m_iPart == SW)
		DisplayContactDlg(18, FALSE);
}
void CFormReport1::ClickContactFinal05()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FINAL CONTACT05"));
	if(m_iPart == DAY)
		DisplayContactDlg(11, FALSE);
	else if(m_iPart == GY)
		DisplayContactDlg(3, FALSE);
	else if(m_iPart == SW)
		DisplayContactDlg(19, FALSE);
}
void CFormReport1::ClickContactFinal06()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FINAL CONTACT06"));
	if(m_iPart == DAY)
		DisplayContactDlg(12, FALSE);
	else if(m_iPart == GY)
		DisplayContactDlg(4, FALSE);
	else if(m_iPart == SW)
		DisplayContactDlg(20, FALSE);
}
void CFormReport1::ClickContactFinal07()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FINAL CONTACT07"));
	if(m_iPart == DAY)
		DisplayContactDlg(13, FALSE);
	else if(m_iPart == GY)
		DisplayContactDlg(5, FALSE);
	else if(m_iPart == SW)
		DisplayContactDlg(21, FALSE);
}
void CFormReport1::ClickContactFinal08()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FINAL CONTACT08"));
	if(m_iPart == DAY)
		DisplayContactDlg(14, FALSE);
	else if(m_iPart == GY)
		DisplayContactDlg(6, FALSE);
	else if(m_iPart == SW)
		DisplayContactDlg(22, FALSE);
}
void CFormReport1::ClickContactFinalTotal()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FINAL CONTACT TOTAL"));
	DisplayContactDlg(0,23, FALSE);
}

void CFormReport1::ClickContactFirstTeamTotal()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FIRST CONTACT TEAMTOTAL"));
	if(m_iPart == DAY)
		ClickContactFirstDAY();
	else if(m_iPart == GY)
		ClickContactFirstGY();
	else if(m_iPart == SW)
		ClickContactFirstSW();
}
void CFormReport1::ClickContactFirstDAY()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FIRST CONTACT DAY"));
	DisplayContactDlg(7,14, TRUE);
}
void CFormReport1::ClickContactFirstSW()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FIRST CONTACT SW"));
	DisplayContactDlg(15,22, TRUE);
}
void CFormReport1::ClickContactFirstGY()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FIRST CONTACT GY"));
	DisplayContactDlg(23,06, TRUE);
}

void CFormReport1::ClickContactFinalTeamTotal()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FINAL CONTACT TEAMTOTAL"));
	if(m_iPart == DAY)
		ClickContactFinalDAY();
	else if(m_iPart == GY)
		ClickContactFinalGY();
	else if(m_iPart == SW)
		ClickContactFinalSW();
}
void CFormReport1::ClickContactFinalDAY()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FINAL CONTACT DAY"));
	DisplayContactDlg(7,14, FALSE);
}
void CFormReport1::ClickContactFinalSW()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FINAL CONTACT SW"));
	DisplayContactDlg(15,22, FALSE);
}
void CFormReport1::ClickContactFinalGY()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("FINAL CONTACT GY"));
	DisplayContactDlg(23,06, FALSE);
}

void CFormReport1::DisplayContactDlg(int nHour, BOOL bFirstContact)
{
	//지정 시간 합산하는 펑션 추가함. 사용안함. 

	//if(nHour == 24)	//지정 시간 합산하는 펑션 추가함. 사용안함. 
	//{
	//	// total 
	//	// cCONTACT_DATA* pData를 새로 만들어서 넘겨줘야함. 
	//	cCONTACT_DATA* pData = new cCONTACT_DATA;

	//	pData->nHour = 24;
	//	map<short, cCONTACT_DATA>::iterator mapIter;
	//	mapIter = m_ProductBank.m_mapContactReportDataSetLoad.begin();

	//	for(int i=0;i<m_ProductBank.m_mapContactReportDataSetLoad.size();i++,mapIter++)
	//	{
	//		for(int x=0;x<5;x++)
	//		{
	//			pData->vctAJigTotalContactCount[x] += mapIter->second.vctAJigTotalContactCount[x];
	//			pData->vctBJigTotalContactCount[x] += mapIter->second.vctBJigTotalContactCount[x];
	//			pData->vctCJigTotalContactCount[x] += mapIter->second.vctCJigTotalContactCount[x];
	//			pData->vctDJigTotalContactCount[x] += mapIter->second.vctDJigTotalContactCount[x];
	//			
	//			pData->m_vctAJigFirstContactSuccessCount[x].iValue += mapIter->second.m_vctAJigFirstContactSuccessCount[x].iValue;
	//			pData->m_vctBJigFirstContactSuccessCount[x].iValue += mapIter->second.m_vctBJigFirstContactSuccessCount[x].iValue;
	//			pData->m_vctCJigFirstContactSuccessCount[x].iValue += mapIter->second.m_vctCJigFirstContactSuccessCount[x].iValue;
	//			pData->m_vctDJigFirstContactSuccessCount[x].iValue += mapIter->second.m_vctDJigFirstContactSuccessCount[x].iValue;

	//			if(i==0)
	//			{
	//				pData->m_vctAJigFirstContactSuccessCount[x].uID += mapIter->second.m_vctAJigFirstContactSuccessCount[x].uID;
	//				pData->m_vctBJigFirstContactSuccessCount[x].uID += mapIter->second.m_vctBJigFirstContactSuccessCount[x].uID;
	//				pData->m_vctCJigFirstContactSuccessCount[x].uID += mapIter->second.m_vctCJigFirstContactSuccessCount[x].uID;
	//				pData->m_vctDJigFirstContactSuccessCount[x].uID += mapIter->second.m_vctDJigFirstContactSuccessCount[x].uID;
	//			}
	//		}
	//	}
	//
	//	ContactInfoDlg.SetDialogItem(pData);
	//	ContactInfoDlg.szDayCount = CStringSupport::N2C(m_ProductBank.m_MainCountGOOD.iValue);

	//	if(ContactInfoDlg.DoModal() == IDCANCEL)	delete pData;
	//}
	//else
	{
		// Time
// 		cCONTACT_DATA* pData = &m_ProductBank.m_mapContactReportDataSetLoad.find(nHour)->second;
// 		ContactInfoDlg.SetDialogItem(pData);
// 		ContactInfoDlg.szDay		= CStringSupport::N2C(m_ProductBank.m_MainCountINPUT.iValue);
// 		ContactInfoDlg.szDayCount	= CStringSupport::N2C(m_ProductBank.m_MainCountGOOD.iValue);
// 		ContactInfoDlg.szTitle		= _T("Time") + CStringSupport::N2C(nHour);// + _T("");
// 		ContactInfoDlg.m_bContactFirst = bFirstContact;
// 
// 		ContactInfoDlg.DoModal();
	}

	//ContactInfoDlg.DoModal();

}

void CFormReport1::DisplayContactDlg(int nStart_Hour, int nEnd_Hour, BOOL bFirstContact)
{

}