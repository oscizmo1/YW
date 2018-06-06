// FormReport1_Sub.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormReport1_Sub.h"

#include "GUIDefine.h"

#include "UI\Dialog\DlgReportSub.h"

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

// CFormReport1_Sub


IMPLEMENT_DYNCREATE(CFormReport1_Sub, CFormView)

CFormReport1_Sub::CFormReport1_Sub()
	: CFormView(CFormReport1_Sub::IDD)
{
	m_nCurSelectPart = IDC_GXLBL_REPORT_DAY;
	
	// Main list
	/* Time */
	m_nMainRecord[0][0] = IDC_GXLBL_REPORT_TIME_1;
	m_nMainRecord[1][0] = IDC_GXLBL_REPORT_TIME_2;
	m_nMainRecord[2][0] = IDC_GXLBL_REPORT_TIME_3;
	m_nMainRecord[3][0] = IDC_GXLBL_REPORT_TIME_4;
	m_nMainRecord[4][0] = IDC_GXLBL_REPORT_TIME_5;
	m_nMainRecord[5][0] = IDC_GXLBL_REPORT_TIME_6;
	m_nMainRecord[6][0] = IDC_GXLBL_REPORT_TIME_7;
	m_nMainRecord[7][0] = IDC_GXLBL_REPORT_TIME_8;
	m_nMainRecord[8][0] = IDC_GXLBL_REPORT_TIME_9;
	m_nMainRecord[9][0] = IDC_GXLBL_REPORT_TIME_10;
	m_nMainRecord[10][0] = IDC_GXLBL_REPORT_TIME_11;
	m_nMainRecord[11][0] = IDC_GXLBL_REPORT_TIME_12;
	m_nMainRecord[12][0] = NULL;

	/* Product */
	m_nMainRecord[0][1] = IDC_GXLBL_REPORT_NUM_1;
	m_nMainRecord[1][1] = IDC_GXLBL_REPORT_NUM_2;
	m_nMainRecord[2][1] = IDC_GXLBL_REPORT_NUM_3;
	m_nMainRecord[3][1] = IDC_GXLBL_REPORT_NUM_4;
	m_nMainRecord[4][1] = IDC_GXLBL_REPORT_NUM_5;
	m_nMainRecord[5][1] = IDC_GXLBL_REPORT_NUM_6;
	m_nMainRecord[6][1] = IDC_GXLBL_REPORT_NUM_7;
	m_nMainRecord[7][1] = IDC_GXLBL_REPORT_NUM_8;
	m_nMainRecord[8][1] = IDC_GXLBL_REPORT_NUM_9;
	m_nMainRecord[9][1] = IDC_GXLBL_REPORT_NUM_10;
	m_nMainRecord[10][1] = IDC_GXLBL_REPORT_NUM_11;
	m_nMainRecord[11][1] = IDC_GXLBL_REPORT_NUM_12;
	m_nMainRecord[12][1] = IDC_GXLBL_REPORT_TOTAL_1;

	/* Good */
	m_nMainRecord[0][2] = IDC_GXLBL_REPORT_GOOD_1;
	m_nMainRecord[1][2] = IDC_GXLBL_REPORT_GOOD_2;
	m_nMainRecord[2][2] = IDC_GXLBL_REPORT_GOOD_3;
	m_nMainRecord[3][2] = IDC_GXLBL_REPORT_GOOD_4;
	m_nMainRecord[4][2] = IDC_GXLBL_REPORT_GOOD_5;
	m_nMainRecord[5][2] = IDC_GXLBL_REPORT_GOOD_6;
	m_nMainRecord[6][2] = IDC_GXLBL_REPORT_GOOD_7;
	m_nMainRecord[7][2] = IDC_GXLBL_REPORT_GOOD_8;
	m_nMainRecord[8][2] = IDC_GXLBL_REPORT_GOOD_9;
	m_nMainRecord[9][2] = IDC_GXLBL_REPORT_GOOD_10;
	m_nMainRecord[10][2] = IDC_GXLBL_REPORT_GOOD_11;
	m_nMainRecord[11][2] = IDC_GXLBL_REPORT_GOOD_12;
	m_nMainRecord[12][2] = IDC_GXLBL_REPORT_TOTAL_2;

	/* Reject */
	m_nMainRecord[0][3] = IDC_GXLBL_REPORT_REJECT_1;
	m_nMainRecord[1][3] = IDC_GXLBL_REPORT_REJECT_2;
	m_nMainRecord[2][3] = IDC_GXLBL_REPORT_REJECT_3;
	m_nMainRecord[3][3] = IDC_GXLBL_REPORT_REJECT_4;
	m_nMainRecord[4][3] = IDC_GXLBL_REPORT_REJECT_5;
	m_nMainRecord[5][3] = IDC_GXLBL_REPORT_REJECT_6;
	m_nMainRecord[6][3] = IDC_GXLBL_REPORT_REJECT_7;
	m_nMainRecord[7][3] = IDC_GXLBL_REPORT_REJECT_8;
	m_nMainRecord[8][3] = IDC_GXLBL_REPORT_REJECT_9;
	m_nMainRecord[9][3] = IDC_GXLBL_REPORT_REJECT_10;
	m_nMainRecord[10][3] = IDC_GXLBL_REPORT_REJECT_11;
	m_nMainRecord[11][3] = IDC_GXLBL_REPORT_REJECT_12;
	m_nMainRecord[12][3] = IDC_GXLBL_REPORT_TOTAL_3;
	
	/* MCR Reading */
	m_nMainRecord[0][4] = IDC_GXLBL_REPORT_MCR_1;
	m_nMainRecord[1][4] = IDC_GXLBL_REPORT_MCR_2;
	m_nMainRecord[2][4] = IDC_GXLBL_REPORT_MCR_3;
	m_nMainRecord[3][4] = IDC_GXLBL_REPORT_MCR_4;
	m_nMainRecord[4][4] = IDC_GXLBL_REPORT_MCR_5;
	m_nMainRecord[5][4] = IDC_GXLBL_REPORT_MCR_6;
	m_nMainRecord[6][4] = IDC_GXLBL_REPORT_MCR_7;
	m_nMainRecord[7][4] = IDC_GXLBL_REPORT_MCR_8;
	m_nMainRecord[8][4] = IDC_GXLBL_REPORT_MCR_9;
	m_nMainRecord[9][4] = IDC_GXLBL_REPORT_MCR_10;
	m_nMainRecord[10][4] = IDC_GXLBL_REPORT_MCR_11;
	m_nMainRecord[11][4] = IDC_GXLBL_REPORT_MCR_12;
	m_nMainRecord[12][4] = IDC_GXLBL_REPORT_TOTAL_4;

	/* First contact */
	m_nMainRecord[0][5] = IDC_GXLBL_REPORT_FIRST_1;
	m_nMainRecord[1][5] = IDC_GXLBL_REPORT_FIRST_2;
	m_nMainRecord[2][5] = IDC_GXLBL_REPORT_FIRST_3;
	m_nMainRecord[3][5] = IDC_GXLBL_REPORT_FIRST_4;
	m_nMainRecord[4][5] = IDC_GXLBL_REPORT_FIRST_5;
	m_nMainRecord[5][5] = IDC_GXLBL_REPORT_FIRST_6;
	m_nMainRecord[6][5] = IDC_GXLBL_REPORT_FIRST_7;
	m_nMainRecord[7][5] = IDC_GXLBL_REPORT_FIRST_8;
	m_nMainRecord[8][5] = IDC_GXLBL_REPORT_FIRST_9;
	m_nMainRecord[9][5] = IDC_GXLBL_REPORT_FIRST_10;
	m_nMainRecord[10][5] = IDC_GXLBL_REPORT_FIRST_11;
	m_nMainRecord[11][5] = IDC_GXLBL_REPORT_FIRST_12;
	m_nMainRecord[12][5] = IDC_GXLBL_REPORT_TOTAL_5;

	/* First contact */
	m_nMainRecord[0][6] = IDC_GXLBL_REPORT_FINAL_1;
	m_nMainRecord[1][6] = IDC_GXLBL_REPORT_FINAL_2;
	m_nMainRecord[2][6] = IDC_GXLBL_REPORT_FINAL_3;
	m_nMainRecord[3][6] = IDC_GXLBL_REPORT_FINAL_4;
	m_nMainRecord[4][6] = IDC_GXLBL_REPORT_FINAL_5;
	m_nMainRecord[5][6] = IDC_GXLBL_REPORT_FINAL_6;
	m_nMainRecord[6][6] = IDC_GXLBL_REPORT_FINAL_7;
	m_nMainRecord[7][6] = IDC_GXLBL_REPORT_FINAL_8;
	m_nMainRecord[8][6] = IDC_GXLBL_REPORT_FINAL_9;
	m_nMainRecord[9][6] = IDC_GXLBL_REPORT_FINAL_10;
	m_nMainRecord[10][6] = IDC_GXLBL_REPORT_FINAL_11;
	m_nMainRecord[11][6] = IDC_GXLBL_REPORT_FINAL_12;
	m_nMainRecord[12][6] = IDC_GXLBL_REPORT_TOTAL_6;


	// Total list
	/* nTime */
	m_nTotalRecord[0][0] = NULL;
	m_nTotalRecord[1][0] = NULL;
	m_nTotalRecord[2][0] = NULL;

	/* Product */
	m_nTotalRecord[0][1] = IDC_GXLBL_REPORT_NUM_SUN1;
	m_nTotalRecord[1][1] = IDC_GXLBL_REPORT_NUM_SUN2;
	m_nTotalRecord[2][1] = IDC_GXLBL_REPORT_NUM_SUN3;

	/* Good */
	m_nTotalRecord[0][2] = IDC_GXLBL_REPORT_GOOD_SUN1;
	m_nTotalRecord[1][2] = IDC_GXLBL_REPORT_GOOD_SUN2;
	m_nTotalRecord[2][2] = IDC_GXLBL_REPORT_GOOD_SUN3;

	/* Reject */
	m_nTotalRecord[0][3] = IDC_GXLBL_REPORT_REJECT_SUN1;
	m_nTotalRecord[1][3] = IDC_GXLBL_REPORT_REJECT_SUN2;
	m_nTotalRecord[2][3] = IDC_GXLBL_REPORT_REJECT_SUN3;

	/* MCR Reading */
	m_nTotalRecord[0][4] = IDC_GXLBL_REPORT_MCR_SUN1;
	m_nTotalRecord[1][4] = IDC_GXLBL_REPORT_MCR_SUN2;
	m_nTotalRecord[2][4] = IDC_GXLBL_REPORT_MCR_SUN3;

	/* First contact */
	m_nTotalRecord[0][5] = IDC_GXLBL_REPORT_FIRST_SUN1;
	m_nTotalRecord[1][5] = IDC_GXLBL_REPORT_FIRST_SUN2;
	m_nTotalRecord[2][5] = IDC_GXLBL_REPORT_FIRST_SUN3;

	/* Final contact */
	m_nTotalRecord[0][6] = IDC_GXLBL_REPORT_FINAL_SUN1;
	m_nTotalRecord[1][6] = IDC_GXLBL_REPORT_FINAL_SUN2;
	m_nTotalRecord[2][6] = IDC_GXLBL_REPORT_FINAL_SUN3;

}

CFormReport1_Sub::~CFormReport1_Sub()
{
}

void CFormReport1_Sub::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormReport1_Sub, CFormView)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	ON_WM_TIMER()
END_MESSAGE_MAP()


// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
LRESULT CFormReport1_Sub::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )																//	Report2 화면으로 들어올떄
	{
		// 현재 날짜로 값을 Setting - LSH161218
		ClickGxlblReportToday();

		// 파일을 읽어온다 - LSH171218
		m_ProductBank.LoadReprotFileDate(m_szCurrent_time);

		// UI 상태를 업데이트 한다 - LSH171218
		UpdateDisplay();
		UpdateUIData();

		// 타이머 시작 - LSH171218
		//SetTimer(TIMER1, 500, NULL);
	}
	else																		//	Report2 화면을 떠날때
	{
		// 타이머 시작 - LSH171218
		//KillTimer(TIMER1);
	}

	return 0;
}


void CFormReport1_Sub::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDEvent == TIMER1)
	{
		//UpdateUIData();
	}

	CFormView::OnTimer(nIDEvent);
}


// 표시 언어가 변경되었다.
LRESULT CFormReport1_Sub::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	// CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_BOTTOM_MENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormReport1_Sub::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}


// CFormReport1_Sub 진단입니다.

#ifdef _DEBUG
void CFormReport1_Sub::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormReport1_Sub::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CFormReport1_Sub::Init()		//	시간 설정
{
	GetLocalTime(&m_CurDate);

	CTime cTime(m_CurDate);

	CEtc::ApplyTimeOffset(cTime, theConfigBank.m_Option.m_OffsetHour, 0);
	
	m_CurDate.wYear = (WORD)cTime.GetYear();
	m_CurDate.wMonth = (WORD)cTime.GetMonth();
	m_CurDate.wDay = (WORD)cTime.GetDay();

	m_szCurrent_time.Format(_T("%04u-%02u-%02u"),m_CurDate.wYear, m_CurDate.wMonth, m_CurDate.wDay);
}

void CFormReport1_Sub::UpdateDisplay()
{
	CGxUICtrl::SetButtonColor(this, IDC_GXLBL_REPORT_DAY, (m_nCurSelectPart == IDC_GXLBL_REPORT_DAY) ? GXCOLOR_ON : GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXLBL_REPORT_NIGHT, (m_nCurSelectPart == IDC_GXLBL_REPORT_NIGHT) ? GXCOLOR_ON : GXCOLOR_OFF);
}


void CFormReport1_Sub::UpdateUIData()			
{
	// 현재 날짜 업데이트 - LSH171208
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_REPORT_DATE, m_szCurrent_time);

	// 시작시간 종료시간 저장 변수 - LSH171218
	UINT nStartTime, nEndTime;

	//	해당작업조의 시작 시간
 	if ( m_nCurSelectPart == IDC_GXLBL_REPORT_DAY )
	{
		nStartTime = theConfigBank.m_Option.m_OffsetHour;
		nEndTime = theConfigBank.m_Option.m_OffsetHour+12;
	}
 	else if ( m_nCurSelectPart == IDC_GXLBL_REPORT_NIGHT )
	{
		nStartTime = theConfigBank.m_Option.m_OffsetHour+12;
		nEndTime = theConfigBank.m_Option.m_OffsetHour;

		
	}

	// Main list의 데이터를 가져온다 - LSH171217
	// Record와 시간의 값 동기화를 위한 변수 - LSH171218
	UINT nIter = 0;
	for(int nTime=nStartTime ; nTime!=(int)nEndTime ; nTime++ )
	{
		// ...23->24->0->1... 이렇게 증가할 수 있도록24가 될 경우 0을 넣어준다 - LSH171218
		if(nTime >= 24)
			nTime = 0;

		// 각 시간별 데이터를 가져온다 - LSH171217
		 m_ProductBank.GetProductData(&m_infoMainProductInfo[nIter], nTime);

		// Record값 증가 - LSH171218
		nIter++;
	}

	// 기존 데이터를 지우고 Data의 합을 계산한다 - LSH171218
	/* Day */
	m_infoTotalProductInfo[0].Clear();
	m_ProductBank.GetProductData(&m_infoTotalProductInfo[0], theConfigBank.m_Option.m_OffsetHour,theConfigBank.m_Option.m_OffsetHour+12);
	/* Night */
	m_infoTotalProductInfo[1].Clear();
	m_ProductBank.GetProductData(&m_infoTotalProductInfo[1], theConfigBank.m_Option.m_OffsetHour+12,theConfigBank.m_Option.m_OffsetHour);
	/* Whole */
	m_infoTotalProductInfo[2].Clear();
	m_ProductBank.GetProductData(&m_infoTotalProductInfo[2], theConfigBank.m_Option.m_OffsetHour,theConfigBank.m_Option.m_OffsetHour -1);


	// Main record수의 -1(Total 데이터 때문) 만큼 반복하여 데이터를 Main list에 입력 - LSH171218
	for(int nMainUiIter=0 ; nMainUiIter<MainRowMax-1 ; nMainUiIter++)
		SetStaticUIString(&m_infoMainProductInfo[nMainUiIter], m_nMainRecord[nMainUiIter]);

	// Main list에 Total data입력 - LSH171218
	if ( m_nCurSelectPart == IDC_GXLBL_REPORT_DAY )
		SetStaticUIString(&m_infoTotalProductInfo[0], m_nMainRecord[12]);
	else if ( m_nCurSelectPart == IDC_GXLBL_REPORT_NIGHT )
		SetStaticUIString(&m_infoTotalProductInfo[1], m_nMainRecord[12]);

	// Total record수 만큼 반복하여 데이터를 Total list에 입력 - LSH171218
	for(int nTotalUiIter=0 ; nTotalUiIter<TotalRowMax ; nTotalUiIter++)
		SetStaticUIString(&m_infoTotalProductInfo[nTotalUiIter], m_nTotalRecord[nTotalUiIter]);
}


void CFormReport1_Sub::SetStaticUIString(ProductInfo* infoProduct, UINT* pRecord)			//	ProductInfo 의 정보를 static 에 셋팅해주는 함수
{
	// 퍼센트 계산 - LSH171218
	CString strPerMCR = _T("(") + m_ProductBank.CalcPercent(infoProduct->m_nTotalData[PRODUCT_COLUMN_PRODUCT], infoProduct->m_nTotalData[PRODUCT_COLUMN_MCRREADING]) + _T(")");
	CString strPerFirstContact = _T("(") + m_ProductBank.CalcPercent(infoProduct->m_nTotalData[PRODUCT_COLUMN_PRODUCT], infoProduct->m_nTotalData[PRODUCT_COLUMN_FIRSTCONTACT]) + _T(")");
	CString strPerFinalContact = _T("(") + m_ProductBank.CalcPercent(infoProduct->m_nTotalData[PRODUCT_COLUMN_PRODUCT], infoProduct->m_nTotalData[PRODUCT_COLUMN_FINALCONTACT]) + _T(")");

	// 데이터 입력
	if(pRecord[0] != NULL)
		CGxUICtrl::SetStaticString(this, pRecord[0],	CStringSupport::N2C((int)infoProduct->m_nHour) );
	if(pRecord[1] != NULL)
		CGxUICtrl::SetStaticString(this, pRecord[1],	CStringSupport::N2C((int)infoProduct->m_nTotalData[PRODUCT_COLUMN_PRODUCT]));
	if(pRecord[2] != NULL)
		CGxUICtrl::SetStaticString(this, pRecord[2],	CStringSupport::N2C((int)infoProduct->m_nTotalData[PRODUCT_COLUMN_GOOD]));
	if(pRecord[3] != NULL)
		CGxUICtrl::SetStaticString(this, pRecord[3],	CStringSupport::N2C((int)infoProduct->m_nTotalData[PRODUCT_COLUMN_REJECT]) );
	if(pRecord[4] != NULL)
		CGxUICtrl::SetStaticString(this, pRecord[4],	CStringSupport::N2C((int)infoProduct->m_nTotalData[PRODUCT_COLUMN_MCRREADING]) + strPerMCR);
	if(pRecord[5] != NULL)
		CGxUICtrl::SetStaticString(this, pRecord[5],	CStringSupport::N2C((int)infoProduct->m_nTotalData[PRODUCT_COLUMN_FIRSTCONTACT]) + strPerFirstContact);
	if(pRecord[6] != NULL)
		CGxUICtrl::SetStaticString(this, pRecord[6],	CStringSupport::N2C((int)infoProduct->m_nTotalData[PRODUCT_COLUMN_FINALCONTACT]) + strPerFinalContact);
}

// CFormReport1_Sub 메시지 처리기입니다.


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

BEGIN_EVENTSINK_MAP(CFormReport1_Sub, CFormView)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_DAY, DISPID_CLICK, CFormReport1_Sub::ClickDay, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_NIGHT, DISPID_CLICK, CFormReport1_Sub::ClickNight, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_PREV, DISPID_CLICK, CFormReport1_Sub::ClickPrev, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_NEXT, DISPID_CLICK, CFormReport1_Sub::ClickNext, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_TODAY, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportToday, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_MCR_3, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportMcr3, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_MCR_1, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportMcr1, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_MCR_2, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportMcr2, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_MCR_4, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportMcr4, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_MCR_5, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportMcr5, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_MCR_6, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportMcr6, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_MCR_7, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportMcr7, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_MCR_8, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportMcr8, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_MCR_9, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportMcr9, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_MCR_10, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportMcr10, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_MCR_11, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportMcr11, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_MCR_12, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportMcr12, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_TOTAL_4, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportTotal4, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_GOOD_1, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportGood1, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_GOOD_2, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportGood2, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_GOOD_3, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportGood3, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_GOOD_4, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportGood4, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_GOOD_5, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportGood5, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_GOOD_6, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportGood6, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_GOOD_7, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportGood7, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_GOOD_8, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportGood8, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_GOOD_9, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportGood9, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_GOOD_10, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportGood10, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_GOOD_11, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportGood11, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_GOOD_12, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportGood12, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_NUM_1, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportNum1, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_NUM_2, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportNum2, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_NUM_3, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportNum3, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_NUM_4, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportNum4, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_NUM_5, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportNum5, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_NUM_6, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportNum6, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_NUM_7, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportNum7, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_NUM_8, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportNum8, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_NUM_9, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportNum9, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_NUM_10, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportNum10, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_NUM_11, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportNum11, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_NUM_12, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportNum12, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_TOTAL_1, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportTotal1, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_TOTAL_2, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportTotal2, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_REJECT_1, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportReject1, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_REJECT_2, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportReject2, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_REJECT_3, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportReject3, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_REJECT_4, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportReject4, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_REJECT_5, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportReject5, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_REJECT_6, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportReject6, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_REJECT_7, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportReject7, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_REJECT_8, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportReject8, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_REJECT_9, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportReject9, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_REJECT_10, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportReject10, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_REJECT_11, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportReject11, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_REJECT_12, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportReject12, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_TOTAL_3, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportTotal3, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FIRST_1, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFirst1, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FIRST_2, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFirst2, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FIRST_3, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFirst3, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FIRST_4, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFirst4, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FIRST_5, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFirst5, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FIRST_6, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFirst6, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FIRST_7, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFirst7, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FIRST_8, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFirst8, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FIRST_9, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFirst9, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FIRST_10, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFirst10, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FIRST_11, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFirst11, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FIRST_12, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFirst12, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_TOTAL_5, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportTotal5, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FINAL_1, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFinal1, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FINAL_2, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFinal2, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FINAL_3, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFinal3, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FINAL_4, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFinal4, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FINAL_5, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFinal5, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FINAL_6, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFinal6, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FINAL_7, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFinal7, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FINAL_8, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFinal8, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FINAL_9, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFinal9, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FINAL_10, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFinal10, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FINAL_11, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFinal11, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FINAL_12, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFinal12, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_TOTAL_6, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportTotal6, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_NUM_SUN1, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportNumSun1, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_NUM_SUN2, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportNumSun2, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_NUM_SUN3, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportNumSun3, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_GOOD_SUN1, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportGoodSun1, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_GOOD_SUN2, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportGoodSun2, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_GOOD_SUN3, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportGoodSun3, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_REJECT_SUN1, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportRejectSun1, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_REJECT_SUN2, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportRejectSun2, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_REJECT_SUN3, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportRejectSun3, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_MCR_SUN1, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportMcrSun1, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_MCR_SUN2, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportMcrSun2, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_MCR_SUN3, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportMcrSun3, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FIRST_SUN1, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFirstSun1, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FIRST_SUN2, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFirstSun2, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FIRST_SUN3, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFirstSun3, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FINAL_SUN1, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFinalSun1, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FINAL_SUN2, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFinalSun2, VTS_NONE)
	ON_EVENT(CFormReport1_Sub, IDC_GXLBL_REPORT_FINAL_SUN3, DISPID_CLICK, CFormReport1_Sub::ClickGxlblReportFinalSun3, VTS_NONE)
	END_EVENTSINK_MAP()


// List control button - LSH171218
/* Part */
void CFormReport1_Sub::ClickDay()		//	DAY 버튼 클릭시
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"), _T("DAY"));
	
	// Day UI ID - LSH171218
	m_nCurSelectPart = IDC_GXLBL_REPORT_DAY;

	// UI data 갱신 - LSH171218
	UpdateUIData();

	// Display 상태 갱신
	UpdateDisplay();
}

void CFormReport1_Sub::ClickNight()		//	SW 버튼 클릭시
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"), _T("NIGHT"));
	
	// Day UI ID - LSH171218
	m_nCurSelectPart = IDC_GXLBL_REPORT_NIGHT;

	// UI data 갱신 - LSH171218
	UpdateUIData();

	// Display 상태 갱신
	UpdateDisplay();
}

/* List data control */
void CFormReport1_Sub::ClickPrev()		//	날짜 PREV 클릭시
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("PREV"));
	SetDateTime(-1);

	// 오늘 날짜의 데이터를 읽어온다 - LSH171218
	m_ProductBank.LoadReprotFileDate(m_szCurrent_time.GetBuffer());

	// UI 갱신 - LSH171218
	UpdateUIData();
}

void CFormReport1_Sub::ClickNext()		// 날짜 NEXT 클릭시
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("NEXT"));
	SetDateTime(+1);

	// 오늘 날짜의 데이터를 읽어온다 - LSH171218
	m_ProductBank.LoadReprotFileDate(m_szCurrent_time.GetBuffer());
	
	// UI 갱신 - LSH171218
	UpdateUIData();
}

void CFormReport1_Sub::ClickGxlblReportToday()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("TODAY"));
	SetDateTime(0, TRUE);

	// 오늘 날짜의 데이터를 읽어온다 - LSH171218
	m_ProductBank.LoadReprotFileDate(m_szCurrent_time.GetBuffer());

	// UI 갱신 - LSH171218
	UpdateUIData();
}


// ETC
void CFormReport1_Sub::SetDateTime(int iValue, BOOL btoday)		//	PREV나 NEXT 클릭시 날짜를 바꿔줌
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


////////////////////// PRODUCT //////////////////////
void CFormReport1_Sub::ClickGxlblReportNum1()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[0], PRODUCT_COLUMN_PRODUCT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportNum2()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[1], PRODUCT_COLUMN_PRODUCT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportNum3()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[2], PRODUCT_COLUMN_PRODUCT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportNum4()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[3], PRODUCT_COLUMN_PRODUCT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportNum5()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[4], PRODUCT_COLUMN_PRODUCT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportNum6()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[5], PRODUCT_COLUMN_PRODUCT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportNum7()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[6], PRODUCT_COLUMN_PRODUCT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportNum8()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[7], PRODUCT_COLUMN_PRODUCT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportNum9()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[8], PRODUCT_COLUMN_PRODUCT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportNum10()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[9], PRODUCT_COLUMN_PRODUCT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportNum11()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[10], PRODUCT_COLUMN_PRODUCT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportNum12()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[11], PRODUCT_COLUMN_PRODUCT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportTotal1()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// 현재 선택된 작업조 구분 - LSH171219
	if(m_nCurSelectPart == IDC_GXLBL_REPORT_DAY)
		dlg.SetProductItem(&m_infoTotalProductInfo[0], PRODUCT_COLUMN_PRODUCT);
	else
		dlg.SetProductItem(&m_infoTotalProductInfo[1], PRODUCT_COLUMN_PRODUCT);

	dlg.DoModal();
}


////////////////////// GOOD //////////////////////
void CFormReport1_Sub::ClickGxlblReportGood1()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[0], PRODUCT_COLUMN_GOOD);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportGood2()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[1], PRODUCT_COLUMN_GOOD);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportGood3()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[2], PRODUCT_COLUMN_GOOD);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportGood4()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[3], PRODUCT_COLUMN_GOOD);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportGood5()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[4], PRODUCT_COLUMN_GOOD);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportGood6()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[5], PRODUCT_COLUMN_GOOD);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportGood7()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[6], PRODUCT_COLUMN_GOOD);

	dlg.DoModal();}

void CFormReport1_Sub::ClickGxlblReportGood8()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[7], PRODUCT_COLUMN_GOOD);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportGood9()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[8], PRODUCT_COLUMN_GOOD);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportGood10()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[9], PRODUCT_COLUMN_GOOD);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportGood11()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[10], PRODUCT_COLUMN_GOOD);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportGood12()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[11], PRODUCT_COLUMN_GOOD);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportTotal2()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// 현재 선택된 작업조 구분 - LSH171219
	if(m_nCurSelectPart == IDC_GXLBL_REPORT_DAY)
		dlg.SetProductItem(&m_infoTotalProductInfo[0], PRODUCT_COLUMN_GOOD);
	else
		dlg.SetProductItem(&m_infoTotalProductInfo[1], PRODUCT_COLUMN_GOOD);

	dlg.DoModal();
}


////////////////////// REJECT //////////////////////
void CFormReport1_Sub::ClickGxlblReportReject1()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[0], PRODUCT_COLUMN_REJECT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportReject2()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[1], PRODUCT_COLUMN_REJECT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportReject3()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[2], PRODUCT_COLUMN_REJECT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportReject4()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[3], PRODUCT_COLUMN_REJECT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportReject5()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[4], PRODUCT_COLUMN_REJECT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportReject6()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[5], PRODUCT_COLUMN_REJECT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportReject7()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[6], PRODUCT_COLUMN_REJECT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportReject8()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[7], PRODUCT_COLUMN_REJECT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportReject9()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[8], PRODUCT_COLUMN_REJECT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportReject10()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[9], PRODUCT_COLUMN_REJECT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportReject11()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[10], PRODUCT_COLUMN_REJECT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportReject12()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[11], PRODUCT_COLUMN_REJECT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportTotal3()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// 현재 선택된 작업조 구분 - LSH171219
	if(m_nCurSelectPart == IDC_GXLBL_REPORT_DAY)
		dlg.SetProductItem(&m_infoTotalProductInfo[0], PRODUCT_COLUMN_REJECT);
	else
		dlg.SetProductItem(&m_infoTotalProductInfo[1], PRODUCT_COLUMN_REJECT);

	dlg.DoModal();
}


////////////////////// MCR //////////////////////
void CFormReport1_Sub::ClickGxlblReportMcr1()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[0], PRODUCT_COLUMN_MCRREADING);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportMcr2()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[1], PRODUCT_COLUMN_MCRREADING);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportMcr3()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[2], PRODUCT_COLUMN_MCRREADING);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportMcr4()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[3], PRODUCT_COLUMN_MCRREADING);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportMcr5()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[4], PRODUCT_COLUMN_MCRREADING);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportMcr6()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[5], PRODUCT_COLUMN_MCRREADING);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportMcr7()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[6], PRODUCT_COLUMN_MCRREADING);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportMcr8()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[7], PRODUCT_COLUMN_MCRREADING);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportMcr9()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[8], PRODUCT_COLUMN_MCRREADING);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportMcr10()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[9], PRODUCT_COLUMN_MCRREADING);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportMcr11()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[10], PRODUCT_COLUMN_MCRREADING);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportMcr12()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[11], PRODUCT_COLUMN_MCRREADING);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportTotal4()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// 현재 선택된 작업조 구분 - LSH171219
	if(m_nCurSelectPart == IDC_GXLBL_REPORT_DAY)
		dlg.SetProductItem(&m_infoTotalProductInfo[0], PRODUCT_COLUMN_MCRREADING);
	else
		dlg.SetProductItem(&m_infoTotalProductInfo[1], PRODUCT_COLUMN_MCRREADING);

	dlg.DoModal();
}


////////////////////// FIRST CONTACT //////////////////////
void CFormReport1_Sub::ClickGxlblReportFirst1()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[0], PRODUCT_COLUMN_FIRSTCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFirst2()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[1], PRODUCT_COLUMN_FIRSTCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFirst3()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[2], PRODUCT_COLUMN_FIRSTCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFirst4()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[3],  PRODUCT_COLUMN_FIRSTCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFirst5()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[4], PRODUCT_COLUMN_FIRSTCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFirst6()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[5], PRODUCT_COLUMN_FIRSTCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFirst7()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[6], PRODUCT_COLUMN_FIRSTCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFirst8()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[7], PRODUCT_COLUMN_FIRSTCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFirst9()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[8], PRODUCT_COLUMN_FIRSTCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFirst10()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[9], PRODUCT_COLUMN_FIRSTCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFirst11()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[10], PRODUCT_COLUMN_FIRSTCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFirst12()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[11], PRODUCT_COLUMN_FIRSTCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportTotal5()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// 현재 선택된 작업조 구분 - LSH171219
	if(m_nCurSelectPart == IDC_GXLBL_REPORT_DAY)
		dlg.SetProductItem(&m_infoTotalProductInfo[0], PRODUCT_COLUMN_FIRSTCONTACT);
	else
		dlg.SetProductItem(&m_infoTotalProductInfo[1], PRODUCT_COLUMN_FIRSTCONTACT);

	dlg.DoModal();
}


////////////////////// FINAL CONTACT //////////////////////
void CFormReport1_Sub::ClickGxlblReportFinal1()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[0], PRODUCT_COLUMN_FINALCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFinal2()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[1], PRODUCT_COLUMN_FINALCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFinal3()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[2], PRODUCT_COLUMN_FINALCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFinal4()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[3], PRODUCT_COLUMN_FINALCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFinal5()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[4], PRODUCT_COLUMN_FINALCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFinal6()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[5], PRODUCT_COLUMN_FINALCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFinal7()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[6], PRODUCT_COLUMN_FINALCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFinal8()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[7], PRODUCT_COLUMN_FINALCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFinal9()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[8], PRODUCT_COLUMN_FINALCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFinal10()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[9], PRODUCT_COLUMN_FINALCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFinal11()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[10], PRODUCT_COLUMN_FINALCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFinal12()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoMainProductInfo[11], PRODUCT_COLUMN_FINALCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportTotal6()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// 현재 선택된 작업조 구분 - LSH171219
	if(m_nCurSelectPart == IDC_GXLBL_REPORT_DAY)
		dlg.SetProductItem(&m_infoTotalProductInfo[0], PRODUCT_COLUMN_FINALCONTACT);
	else
		dlg.SetProductItem(&m_infoTotalProductInfo[1], PRODUCT_COLUMN_FINALCONTACT);

	dlg.DoModal();
}




////////////////////// TOTAL PRODUCT //////////////////////
void CFormReport1_Sub::ClickGxlblReportNumSun1()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoTotalProductInfo[0], PRODUCT_COLUMN_PRODUCT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportNumSun2()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoTotalProductInfo[1], PRODUCT_COLUMN_PRODUCT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportNumSun3()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoTotalProductInfo[2], PRODUCT_COLUMN_PRODUCT);

	dlg.DoModal();
}


////////////////////// TOTAL GOOD //////////////////////
void CFormReport1_Sub::ClickGxlblReportGoodSun1()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoTotalProductInfo[0], PRODUCT_COLUMN_GOOD);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportGoodSun2()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoTotalProductInfo[1], PRODUCT_COLUMN_GOOD);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportGoodSun3()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoTotalProductInfo[2], PRODUCT_COLUMN_GOOD);

	dlg.DoModal();
}


////////////////////// TOTAL REJECT //////////////////////
void CFormReport1_Sub::ClickGxlblReportRejectSun1()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoTotalProductInfo[0], PRODUCT_COLUMN_REJECT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportRejectSun2()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoTotalProductInfo[1], PRODUCT_COLUMN_REJECT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportRejectSun3()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoTotalProductInfo[2], PRODUCT_COLUMN_REJECT);

	dlg.DoModal();
}


////////////////////// TOTAL MCR //////////////////////
void CFormReport1_Sub::ClickGxlblReportMcrSun1()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoTotalProductInfo[0], PRODUCT_COLUMN_MCRREADING);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportMcrSun2()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoTotalProductInfo[1], PRODUCT_COLUMN_MCRREADING);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportMcrSun3()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoTotalProductInfo[2], PRODUCT_COLUMN_MCRREADING);

	dlg.DoModal();
}


////////////////////// TOTAL FIRST CONTACT //////////////////////
void CFormReport1_Sub::ClickGxlblReportFirstSun1()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoTotalProductInfo[0], PRODUCT_COLUMN_FIRSTCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFirstSun2()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoTotalProductInfo[1], PRODUCT_COLUMN_FIRSTCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFirstSun3()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoTotalProductInfo[2], PRODUCT_COLUMN_FIRSTCONTACT);

	dlg.DoModal();
}


////////////////////// TOTAL FINAL CONTACT //////////////////////
void CFormReport1_Sub::ClickGxlblReportFinalSun1()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoTotalProductInfo[0], PRODUCT_COLUMN_FINALCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFinalSun2()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoTotalProductInfo[1], PRODUCT_COLUMN_FINALCONTACT);

	dlg.DoModal();
}

void CFormReport1_Sub::ClickGxlblReportFinalSun3()
{
	// dlg 객체 생성 - LSH171218
	CDlgReportSub dlg;

	// dlg 값 전달 - LSH171218
	dlg.SetProductItem(&m_infoTotalProductInfo[2], PRODUCT_COLUMN_FINALCONTACT);

	dlg.DoModal();
}