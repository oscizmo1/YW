// FormSetup2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormTeach_PDT_IF.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "IbisApp.h"
#include "UI\Dialog\DlgBoxKeyBoard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CFormTeachPDT_IO

IMPLEMENT_DYNCREATE(CFormTeach_PDT_IF, CFormView)

CFormTeach_PDT_IF::CFormTeach_PDT_IF()
	: CFormView(CFormTeach_PDT_IF::IDD)
{
	bFirstInit = FALSE;
	m_Jig = JIG_ID_A;
}

CFormTeach_PDT_IF::~CFormTeach_PDT_IF()
{
}

void CFormTeach_PDT_IF::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
		
	// GAUSGUI에서 OnInitialUpdate를 두번 호출하는 바람에 하나는 그냥 넘긴다 [10/5/2017 OSC]
	if(bFirstInit == FALSE)
	{
		bFirstInit = TRUE;
		return;
	}

	LBGrid_Init();
	LWGrid_Init();
	HotLineGrid_Init();
}


void CFormTeach_PDT_IF::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_MTP_LB, m_GridMTP_LB);
	DDX_Control(pDX, IDC_GRID_PDT_LB, m_GridPDT_LB);
	DDX_Control(pDX, IDC_GRID_MTP_LW, m_GridMTP_LW);
	DDX_Control(pDX, IDC_GRID_PDT_LW, m_GridPDT_LW);
	DDX_Control(pDX, IDC_GRID_HOT_LINE, m_GridHotLine);
}

BEGIN_MESSAGE_MAP(CFormTeach_PDT_IF, CFormView)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_NOTIFY(NM_CLICK, IDC_GRID_MTP_LB, OnGridClickMTP_LB)	
	ON_NOTIFY(NM_CLICK, IDC_GRID_MTP_LW, OnGridClickMTP_LW)	
	ON_NOTIFY(NM_CLICK, IDC_GRID_HOT_LINE, OnGridClickHotLine)	
	ON_WM_TIMER()

END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CFormTeach_PDT_IF, CFormView)
	ON_EVENT(CFormTeach_PDT_IF, IDC_GXBTN_SHUTTLE_1, DISPID_CLICK, CFormTeach_PDT_IF::ClickGxbtnShuttle1, VTS_NONE)
	ON_EVENT(CFormTeach_PDT_IF, IDC_GXBTN_SHUTTLE_2, DISPID_CLICK, CFormTeach_PDT_IF::ClickGxbtnShuttle2, VTS_NONE)
	ON_EVENT(CFormTeach_PDT_IF, IDC_GXBTN_EDIT_MODE, DISPID_CLICK, CFormTeach_PDT_IF::ClickGxbtnEditMode, VTS_NONE)
	ON_EVENT(CFormTeach_PDT_IF, IDC_GXBTN_STATE_CLEAR, DISPID_CLICK, CFormTeach_PDT_IF::ClickGxbtnStateClear, VTS_NONE)
END_EVENTSINK_MAP()



// CFormSetup2 진단입니다.

#ifdef _DEBUG
void CFormTeach_PDT_IF::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormTeach_PDT_IF::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormSetup2 메시지 처리기입니다.

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
LRESULT CFormTeach_PDT_IF::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{

	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		m_bEditMode = FALSE;
		if(m_bEditMode)
			CGxUICtrl::SetButtonColor(this, IDC_GXBTN_EDIT_MODE, GXCOLOR_RED);
		else
			CGxUICtrl::SetButtonColor(this, IDC_GXBTN_EDIT_MODE, GXCOLOR_OFF);
		ShuttleBtn_Refresh();
		SetTimer(0, 100, NULL);
	}
	else
	{
		KillTimer(0);
	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CFormTeach_PDT_IF::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	// CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_BOTTOM_MENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormTeach_PDT_IF::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}


void CFormTeach_PDT_IF::LBGrid_Init()
{
	//설정
	m_GridMTP_LB.SetEditable(FALSE);
	m_GridMTP_LB.SetFixedColumnCount(1); 
	m_GridMTP_LB.SetFixedRowCount(1);
	m_GridMTP_LB.SetRowCount(33);
	m_GridMTP_LB.SetColumnCount(3);
	m_GridMTP_LB.SetRowResize(FALSE);
	m_GridMTP_LB.SetColumnResize(FALSE);		
	m_GridMTP_LB.SetListMode(FALSE);
	m_GridMTP_LB.EnableSelection(FALSE);

	m_GridPDT_LB.SetEditable(FALSE);
	m_GridPDT_LB.SetFixedColumnCount(1); 
	m_GridPDT_LB.SetFixedRowCount(1);
	m_GridPDT_LB.SetRowCount(33);
	m_GridPDT_LB.SetColumnCount(3);
	m_GridPDT_LB.SetRowResize(FALSE);
	m_GridPDT_LB.SetColumnResize(FALSE);		
	m_GridPDT_LB.SetListMode(FALSE);
	m_GridPDT_LB.EnableSelection(FALSE);
	m_GridPDT_LB.EnableWindow(FALSE);

	//간격
	m_GridMTP_LB.SetColumnWidth(0, 60); 
	m_GridMTP_LB.SetColumnWidth(1, 180);
	m_GridMTP_LB.SetColumnWidth(2, 180);

	m_GridPDT_LB.SetColumnWidth(0, 60); 
	m_GridPDT_LB.SetColumnWidth(1, 180);
	m_GridPDT_LB.SetColumnWidth(2, 180);

	// Title
	m_GridMTP_LB.SetItemText(0,0,_T("Address"));
	m_GridMTP_LB.SetItemText(0,1,_T("Name"));
	m_GridMTP_LB.SetItemText(0,2,_T("State"));

	m_GridPDT_LB.SetItemText(0,0,_T("Address"));
	m_GridPDT_LB.SetItemText(0,1,_T("Name"));
	m_GridPDT_LB.SetItemText(0,2,_T("State"));

	// Name
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH1_SURVIVAL+1				, 1, _T("CH1_SURVIVAL"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_ABLE+1						, 1, _T("ABLE"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_START+1						, 1, _T("START"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH1_CHECK_SENSOR_ON+1		, 1, _T("CH1_CHECK_SENSOR_ON"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_COMPLETE+1					, 1, _T("COMPLETE"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH1_PRODUCT+1				, 1, _T("CH1_PRODUCT"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH1_INSPECTION_OK+1			, 1, _T("CH1_INSPECTION_OK"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH1_INSPECTION_NG+1			, 1, _T("CH1_INSPECTION_NG"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH1_INSPECTION_RETRY_AA+1	, 1, _T("CH1_INSPECTION_RETRY_AA"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH1_INSPECTION_RETRY_AB+1	, 1, _T("CH1_INSPECTION_RETRY_AB"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH1_INSPECTION_RETRY_AC+1	, 1, _T("CH1_INSPECTION_RETRY_AC"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH1_LOADING_STOP+1			, 1, _T("CH1_LOADING_STOP"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH1_VAC_ON_STATUS+1			, 1, _T("CH1_VAC_ON_STATUS"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH1_VAC_OFF_STATUS+1		, 1, _T("CH1_VAC_OFF_STATUS"));

// 	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH2_SURVIVAL+1				, 1, _T("CH2_SURVIVAL"));
// 	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH2_ABLE+1					, 1, _T("CH2_ABLE"));
// 	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH2_START+1					, 1, _T("CH2_START"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH2_CHECK_SENSOR_ON+1		, 1, _T("CH2_CHECK_SENSOR_ON"));
// 	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH2_COMPLETE+1				, 1, _T("CH2_COMPLETE"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH2_PRODUCT+1				, 1, _T("CH2_PRODUCT"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH2_INSPECTION_OK+1			, 1, _T("CH2_INSPECTION_OK"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH2_INSPECTION_NG+1			, 1, _T("CH2_INSPECTION_NG"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH2_INSPECTION_RETRY_AA+1	, 1, _T("CH2_INSPECTION_RETRY_AA"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH2_INSPECTION_RETRY_AB+1	, 1, _T("CH2_INSPECTION_RETRY_AB"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH2_INSPECTION_RETRY_AC+1	, 1, _T("CH2_INSPECTION_RETRY_AC"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH2_LOADING_STOP+1			, 1, _T("CH2_LOADING_STOP"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH2_VAC_ON_STATUS+1			, 1, _T("CH2_VAC_ON_OFF_STATUS"));
	m_GridMTP_LB.SetItemText(CDevicePDT_IF::MTP_CH2_VAC_OFF_STATUS+1		, 1, _T("CH2_VAC_OFF_STATUS"));


	m_GridPDT_LB.SetItemText(CDevicePDT_IF::PDT_SURVIVAL+1					, 1, _T("SURVIVAL"));
	m_GridPDT_LB.SetItemText(CDevicePDT_IF::PDT_ABLE+1						, 1, _T("ABLE"));
	m_GridPDT_LB.SetItemText(CDevicePDT_IF::PDT_START+1						, 1, _T("START"));
	m_GridPDT_LB.SetItemText(CDevicePDT_IF::PDT_ARM_STATUS+1				, 1, _T("ARM_STATUS"));
	m_GridPDT_LB.SetItemText(CDevicePDT_IF::PDT_COMPLETE+1					, 1, _T("COMPLETE"));
	//kjparkk 20180131 PDT AB RULE 체크 기능 추가
	m_GridPDT_LB.SetItemText(CDevicePDT_IF::PDT_RETRY_AB_RLUE+1				, 1, _T("PDT_RETRY_AB_RLUE"));
	
	m_GridPDT_LB.SetItemText(CDevicePDT_IF::PDT_CH1_VAC_ON_REQ+1			, 1, _T("CH1_VAC_ON_REQ"));
	m_GridPDT_LB.SetItemText(CDevicePDT_IF::PDT_CH1_VAC_OFF_REQ+1			, 1, _T("CH1_VAC_OFF_REQ"));

// 	m_GridPDT_LB.SetItemText(CDevicePDT_IF::PDT_CH2_SURVIVAL+1				, 1, _T("CH2_SURVIVAL"));
// 	m_GridPDT_LB.SetItemText(CDevicePDT_IF::PDT_CH2_ABLE+1					, 1, _T("CH2_ABLE"));
// 	m_GridPDT_LB.SetItemText(CDevicePDT_IF::PDT_CH2_START+1					, 1, _T("CH2_START"));
// 	m_GridPDT_LB.SetItemText(CDevicePDT_IF::PDT_CH2_ARM_STATUS+1			, 1, _T("CH2_ARM_STATUS"));
// 	m_GridPDT_LB.SetItemText(CDevicePDT_IF::PDT_CH2_COMPLETE+1				, 1, _T("CH2_COMPLETE"));
	m_GridPDT_LB.SetItemText(CDevicePDT_IF::PDT_CH2_VAC_ON_REQ+1			, 1, _T("CH2_VAC_ON_REQ"));
	m_GridPDT_LB.SetItemText(CDevicePDT_IF::PDT_CH2_VAC_OFF_REQ+1			, 1, _T("CH2_VAC_OFF_REQ"));

	// Size Fit
	m_GridMTP_LB.ExpandColumnsToFit();	
	m_GridMTP_LB.ExpandToFit();
	m_GridMTP_LB.Refresh();

	m_GridPDT_LB.ExpandColumnsToFit();	
	m_GridPDT_LB.ExpandToFit();
	m_GridPDT_LB.Refresh();

}

void CFormTeach_PDT_IF::LBGrid_Refresh()
{
	// Address
	CString strAddr = _T("");
	for ( int iRow = 1; iRow < m_GridMTP_LB.GetRowCount(); iRow++ )
	{
		strAddr.Format(_T("%X"),iRow + theDevicePDT_IF.MTP_LB_Calcurate_FullAddr(m_Jig, CDevicePDT_IF::MTP_CH1_SURVIVAL) -1);
		m_GridMTP_LB.SetItemText(iRow, 0, strAddr);	
	}

	for ( int iRow = 1; iRow < m_GridPDT_LB.GetRowCount(); iRow++ )
	{
		strAddr.Format(_T("%X"),iRow + theDevicePDT_IF.PDT_LB_Calcurate_FullAddr(m_Jig, CDevicePDT_IF::PDT_SURVIVAL) -1);
		m_GridPDT_LB.SetItemText(iRow, 0, strAddr);	
	}

	CString sNumOutput = _T("");
	CDevicePDT_IF::LB_MTP_ADDR addrMTP;
	CDevicePDT_IF::LB_PDT_ADDR addrPDT;
	for ( int iRow = 1; iRow < m_GridMTP_LB.GetRowCount(); iRow++ )
	{
		addrMTP = (CDevicePDT_IF::LB_MTP_ADDR)(iRow-1);

		if(theDevicePDT_IF.MTP_LB_Get(m_Jig, addrMTP))
		{
			if(m_GridMTP_LB.GetItemBkColour(iRow, 2) != RGB(0,255,0))
			{
				m_GridMTP_LB.SetItemBkColour(iRow, 2, RGB(0,255,0));
			}
		}
		else
		{
			if(m_GridMTP_LB.GetItemBkColour(iRow, 2) != RGB(255,255,255))
			{
				m_GridMTP_LB.SetItemBkColour(iRow, 2, RGB(255,255,255));
			}
		}


		addrPDT = (CDevicePDT_IF::LB_PDT_ADDR)(iRow-1);

		if(theDevicePDT_IF.PDT_LB_Get(m_Jig, addrPDT))
		{
			if(m_GridPDT_LB.GetItemBkColour(iRow, 2) != RGB(0,255,0))
			{
				m_GridPDT_LB.SetItemBkColour(iRow, 2, RGB(0,255,0));
			}
		}
		else
		{
			if(m_GridPDT_LB.GetItemBkColour(iRow, 2) != RGB(255,255,255))
			{
				m_GridPDT_LB.SetItemBkColour(iRow, 2, RGB(255,255,255));
			}
		}
	}

	m_GridMTP_LB.Refresh();
	m_GridPDT_LB.Refresh();

}

void CFormTeach_PDT_IF::LWGrid_Init()
{
	//설정
	m_GridMTP_LW.SetEditable(FALSE);
	m_GridMTP_LW.SetFixedColumnCount(1); 
	m_GridMTP_LW.SetFixedRowCount(1);
	m_GridMTP_LW.SetRowCount(7);
	m_GridMTP_LW.SetColumnCount(3);
	m_GridMTP_LW.SetRowResize(FALSE);
	m_GridMTP_LW.SetColumnResize(FALSE);		
	m_GridMTP_LW.SetListMode(FALSE);
	m_GridMTP_LW.EnableSelection(FALSE);

	m_GridPDT_LW.SetEditable(FALSE);
	m_GridPDT_LW.SetFixedColumnCount(1); 
	m_GridPDT_LW.SetFixedRowCount(1);
	m_GridPDT_LW.SetRowCount(7);
	m_GridPDT_LW.SetColumnCount(3);
	m_GridPDT_LW.SetRowResize(FALSE);
	m_GridPDT_LW.SetColumnResize(FALSE);		
	m_GridPDT_LW.SetListMode(FALSE);
	m_GridPDT_LW.EnableSelection(FALSE);
	m_GridPDT_LW.EnableWindow(FALSE);


	//간격
	m_GridMTP_LW.SetColumnWidth(0, 60); 
	m_GridMTP_LW.SetColumnWidth(1, 180);
	m_GridMTP_LW.SetColumnWidth(2, 180);

	m_GridPDT_LW.SetColumnWidth(0, 60); 
	m_GridPDT_LW.SetColumnWidth(1, 180);
	m_GridPDT_LW.SetColumnWidth(2, 180);


	// Title
	m_GridMTP_LW.SetItemText(0,0,_T("Address"));
	m_GridMTP_LW.SetItemText(0,1,_T("Name"));
	m_GridMTP_LW.SetItemText(0,2,_T("State"));

	m_GridPDT_LW.SetItemText(0,0,_T("Address"));
	m_GridPDT_LW.SetItemText(0,1,_T("Name"));
	m_GridPDT_LW.SetItemText(0,2,_T("State"));


	// Name

	m_GridMTP_LW.SetItemText(1				, 1, _T("CH1_CELL_ID"));
	m_GridMTP_LW.SetItemText(2				, 1, _T("CH1_NG_CODE"));
	m_GridMTP_LW.SetItemText(3				, 1, _T("CH2_CELL_ID"));
	m_GridMTP_LW.SetItemText(4				, 1, _T("CH2_NG_CODE"));

	m_GridPDT_LW.SetItemText(1				, 1, _T("CH1_CELL_ID"));
	m_GridPDT_LW.SetItemText(2				, 1, _T("CH1_ROBOT_STATUS"));
	m_GridPDT_LW.SetItemText(3				, 1, _T("CH1_ROBOT_ALARM_CODE"));
	m_GridPDT_LW.SetItemText(4				, 1, _T("CH2_CELL_ID"));
	m_GridPDT_LW.SetItemText(5				, 1, _T("CH2_ROBOT_STATUS"));
	m_GridPDT_LW.SetItemText(6				, 1, _T("CH2_ROBOT_ALARM_CODE"));


	// Size Fit
	m_GridMTP_LW.ExpandColumnsToFit();	
	m_GridMTP_LW.ExpandToFit();
	m_GridMTP_LW.Refresh();

	m_GridPDT_LW.ExpandColumnsToFit();	
	m_GridPDT_LW.ExpandToFit();
	m_GridPDT_LW.Refresh();
}

void CFormTeach_PDT_IF::LWGrid_Refresh()
{
	CString strAddr = _T("");
	strAddr.Format(_T("%X"), theDevicePDT_IF.MTP_LW_Calcurate_FullAddr(m_Jig, CDevicePDT_IF::MTP_CH1_CELL_ID));	m_GridMTP_LW.SetItemText(1, 0, strAddr);	
	strAddr.Format(_T("%X"), theDevicePDT_IF.MTP_LW_Calcurate_FullAddr(m_Jig, CDevicePDT_IF::MTP_CH1_NG_CODE));	m_GridMTP_LW.SetItemText(2, 0, strAddr);	
	strAddr.Format(_T("%X"), theDevicePDT_IF.MTP_LW_Calcurate_FullAddr(m_Jig, CDevicePDT_IF::MTP_CH2_CELL_ID));	m_GridMTP_LW.SetItemText(3, 0, strAddr);	
	strAddr.Format(_T("%X"), theDevicePDT_IF.MTP_LW_Calcurate_FullAddr(m_Jig, CDevicePDT_IF::MTP_CH2_NG_CODE));	m_GridMTP_LW.SetItemText(4, 0, strAddr);	

	strAddr.Format(_T("%X"), theDevicePDT_IF.PDT_LW_Calcurate_FullAddr(m_Jig, CDevicePDT_IF::PDT_CH1_CELL_ID));				m_GridPDT_LW.SetItemText(1, 0, strAddr);	
	strAddr.Format(_T("%X"), theDevicePDT_IF.PDT_LW_Calcurate_FullAddr(JIG_ID_A, CDevicePDT_IF::PDT_CH1_ROBOT_STATUS));		m_GridPDT_LW.SetItemText(2, 0, strAddr);	
	strAddr.Format(_T("%X"), theDevicePDT_IF.PDT_LW_Calcurate_FullAddr(JIG_ID_A, CDevicePDT_IF::PDT_CH1_ROBOT_ALARM_CODE));	m_GridPDT_LW.SetItemText(3, 0, strAddr);	
	strAddr.Format(_T("%X"), theDevicePDT_IF.PDT_LW_Calcurate_FullAddr(m_Jig, CDevicePDT_IF::PDT_CH2_CELL_ID));				m_GridPDT_LW.SetItemText(4, 0, strAddr);	
	strAddr.Format(_T("%X"), theDevicePDT_IF.PDT_LW_Calcurate_FullAddr(JIG_ID_A, CDevicePDT_IF::PDT_CH2_ROBOT_STATUS));		m_GridPDT_LW.SetItemText(5, 0, strAddr);	
	strAddr.Format(_T("%X"), theDevicePDT_IF.PDT_LW_Calcurate_FullAddr(JIG_ID_A, CDevicePDT_IF::PDT_CH2_ROBOT_ALARM_CODE));	m_GridPDT_LW.SetItemText(6, 0, strAddr);	

	m_GridMTP_LW.SetItemText(1	, 2, theDevicePDT_IF.MTP_LW_ReadCellID(m_Jig, JIG_CH_1));
	m_GridMTP_LW.SetItemText(2	, 2, theDevicePDT_IF.MTP_LW_ReadNGCode(m_Jig, JIG_CH_1));

	m_GridPDT_LW.SetItemText(1	, 2, theDevicePDT_IF.PDT_LW_ReadCellID(m_Jig, JIG_CH_1));
	m_GridPDT_LW.SetItemText(2	, 2, theDevicePDT_IF.PDT_LW_ReadRobotStatusString(JIG_ID_A, JIG_CH_1));
	m_GridPDT_LW.SetItemText(3	, 2, theDevicePDT_IF.PDT_LW_ReadRobotAlarmCodeString(JIG_ID_A, JIG_CH_1));

	m_GridMTP_LW.Refresh();
	m_GridPDT_LW.Refresh();
}

void CFormTeach_PDT_IF::HotLineGrid_Init()
{
	//설정
	m_GridHotLine.SetEditable(FALSE);
	m_GridHotLine.SetFixedColumnCount(1); 
	m_GridHotLine.SetFixedRowCount(1);
	m_GridHotLine.SetRowCount(9);
	m_GridHotLine.SetColumnCount(3);
	m_GridHotLine.SetRowResize(FALSE);
	m_GridHotLine.SetColumnResize(FALSE);		
	m_GridHotLine.SetListMode(FALSE);
	m_GridHotLine.EnableSelection(FALSE);


	//간격
	m_GridHotLine.SetColumnWidth(0, 200); 
	m_GridHotLine.SetColumnWidth(1, 30);
	m_GridHotLine.SetColumnWidth(2, 30);


	// Title
	m_GridHotLine.SetItemText(0,0,_T("Name"));
	m_GridHotLine.SetItemText(0,1,_T("MTP"));
	m_GridHotLine.SetItemText(0,2,_T("PDT"));


	// Name
	m_GridHotLine.SetItemText(1		, 0, _T("ALIVE"));
	m_GridHotLine.SetItemText(2		, 0, _T("EMERGENCY"));
	m_GridHotLine.SetItemText(3		, 0, _T("NET_READY"));
	m_GridHotLine.SetItemText(4		, 0, _T("DOOR_OPEN"));
	m_GridHotLine.SetItemText(5		, 0, _T("INTERLOCK"));
	m_GridHotLine.SetItemText(6		, 0, _T("INTERACTIVE_2"));
	m_GridHotLine.SetItemText(7		, 0, _T("INTERACTIVE_3"));
	m_GridHotLine.SetItemText(8		, 0, _T("SUB_EMO"));

	// No
	m_GridHotLine.SetItemText(1		, 1, CStringSupport::N2C(Y_HOT_LINE_MTP_ALIVE, _T("%X")));
	m_GridHotLine.SetItemText(2		, 1, CStringSupport::N2C(Y_HOT_LINE_MTP_EMERGENCY, _T("%X")));
	m_GridHotLine.SetItemText(3		, 1, CStringSupport::N2C(Y_HOT_LINE_MTP_NET_READY, _T("%X")));
	m_GridHotLine.SetItemText(4		, 1, CStringSupport::N2C(Y_HOT_LINE_MTP_DOOR_OPEN, _T("%X")));
	m_GridHotLine.SetItemText(5		, 1, CStringSupport::N2C(Y_HOT_LINE_MTP_INTERLOCK, _T("%X")));
	m_GridHotLine.SetItemText(6		, 1, CStringSupport::N2C(Y_HOT_LINE_MTP_INTERACTIVE_2, _T("%X")));
	m_GridHotLine.SetItemText(7		, 1, CStringSupport::N2C(Y_HOT_LINE_MTP_INTERACTIVE_3, _T("%X")));
	m_GridHotLine.SetItemText(8		, 1, CStringSupport::N2C(Y_HOT_LINE_MTP_SUB_EMO, _T("%X")));

	m_GridHotLine.SetItemText(1		, 2, CStringSupport::N2C(X_HOT_LINE_PDT_ALIVE, _T("%X")));
	m_GridHotLine.SetItemText(2		, 2, CStringSupport::N2C(X_HOT_LINE_PDT_EMERGENCY, _T("%X")));
	m_GridHotLine.SetItemText(3		, 2, CStringSupport::N2C(X_HOT_LINE_PDT_NET_READY, _T("%X")));
	m_GridHotLine.SetItemText(4		, 2, CStringSupport::N2C(X_HOT_LINE_PDT_DOOR_OPEN, _T("%X")));
	m_GridHotLine.SetItemText(5		, 2, CStringSupport::N2C(X_HOT_LINE_PDT_INTERLOCK, _T("%X")));
	m_GridHotLine.SetItemText(6		, 2, CStringSupport::N2C(X_HOT_LINE_PDT_INTERACTIVE_2, _T("%X")));
	m_GridHotLine.SetItemText(7		, 2, CStringSupport::N2C(X_HOT_LINE_PDT_INTERACTIVE_3, _T("%X")));
	m_GridHotLine.SetItemText(8		, 2, CStringSupport::N2C(X_HOT_LINE_PDT_SUB_EMO, _T("%X")));


	// Size Fit
	m_GridHotLine.ExpandColumnsToFit();	
	m_GridHotLine.ExpandToFit();
	m_GridHotLine.Refresh();
}

void CFormTeach_PDT_IF::HotLineGrid_Refresh()
{	
	CString strInputPLCNo = m_GridHotLine.GetItemText(1,1);
	CString strOutputPLCNo = m_GridPDT_LB.GetItemText(1,1);
	
	int nIBit		= X_HOT_LINE_PDT_ALIVE;
	int nOBit		= Y_HOT_LINE_MTP_ALIVE;
	int nIStationNum =	nIBit / DEV_INPUT;
	int nOStationNum =	nOBit / DEV_OUTPUT;
	int nISlaveNum =	nIBit % DEV_INPUT;
	int nOSlaveNum =	nOBit % DEV_INPUT;

	WORD nInput = theDeviceIO.ReadInWord(nIStationNum);
	WORD nOutput = theDeviceIO.ReadOutWord(nOStationNum);

	int nRow;

	for(int i=nISlaveNum; i < DEV_INPUT; i++)
	{
		nRow = i-nISlaveNum + 1;
		if(nInput >> i & 0x01)
		{
			if(m_GridHotLine.GetItemBkColour(nRow, 2) != RGB(0,255,0))
			{
				m_GridHotLine.SetItemBkColour(nRow, 2, RGB(0,255,0));
			}
		}
		else
		{
			if(m_GridHotLine.GetItemBkColour(nRow, 2) != RGB(255,255,255))
			{
				m_GridHotLine.SetItemBkColour(nRow, 2, RGB(255,255,255));
			}
		}
	}

	for(int i=nOSlaveNum; i < DEV_OUTPUT; i++)
	{
		nRow = i-nOSlaveNum + 1;
		if(nOutput >> i & 0x01)
		{
			if(m_GridHotLine.GetItemBkColour(nRow, 1) != RGB(0,255,0))
			{
				m_GridHotLine.SetItemBkColour(nRow, 1, RGB(0,255,0));
			}
		}
		else
		{
			if(m_GridHotLine.GetItemBkColour(nRow, 1) != RGB(255,255,255))
			{
				m_GridHotLine.SetItemBkColour(nRow, 1, RGB(255,255,255));
			}
		}
	}

	m_GridHotLine.Refresh();
}


void CFormTeach_PDT_IF::ShuttleBtn_Refresh()
{
	COLORREF bOn = GXCOLOR_ON;
	COLORREF bOff = GXCOLOR_OFF;
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_SHUTTLE_1, m_Jig == JIG_ID_A ? bOn:bOff);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_SHUTTLE_2, m_Jig == JIG_ID_B ? bOn:bOff);
}

void CFormTeach_PDT_IF::OnGridClickMTP_LB(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	if(m_bEditMode == FALSE)
		return;
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
		
	if ( pItem->iRow < 1 )
		return;

	CString szSelected = m_GridMTP_LB.GetItemText(pItem->iRow,0);

	CDevicePDT_IF::LB_MTP_ADDR addr = (CDevicePDT_IF::LB_MTP_ADDR)(pItem->iRow-1);
	if(m_GridMTP_LB.GetItemBkColour(pItem->iRow, 2) == RGB(0,255,0))
		theDevicePDT_IF.MTP_LB_Set(m_Jig, addr, FALSE);
	else
		theDevicePDT_IF.MTP_LB_Set(m_Jig, addr, TRUE);
}


void CFormTeach_PDT_IF::OnGridClickMTP_LW(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	if(m_bEditMode == FALSE)
		return;
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	if ( pItem->iRow < 1 )
		return;

	CString szSelected = m_GridPDT_LB.GetItemText(pItem->iRow,0);

	if(pItem->iRow == 1)
	{
		CDlgBoxKeyBoard dlgKey;
		if ( dlgKey.DoModal() == IDOK )
		{
			CString szInput = dlgKey.m_strReturnText;
			theDevicePDT_IF.MTP_LW_WriteCellID(m_Jig, JIG_CH_1, szInput);
		}
	}
	else if(pItem->iRow == 2)
	{
		CDlgBoxKeyBoard dlgKey;
		if ( dlgKey.DoModal() == IDOK )
		{
			CString szInput = dlgKey.m_strReturnText;
			theDevicePDT_IF.MTP_LW_WriteNGCode(m_Jig, JIG_CH_1, szInput);
		}
	}
}

void CFormTeach_PDT_IF::OnGridClickHotLine(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	if(m_bEditMode == FALSE)
		return;
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	if ( pItem->iRow < 1 )
		return;
	if ( pItem->iColumn != 1 )
		return;

	if(m_GridHotLine.GetItemBkColour(pItem->iRow, 1) == RGB(0,255,0))
		theDeviceIO.WriteOutBit(Y_HOT_LINE_MTP_ALIVE + pItem->iRow-1, FALSE);
	else
		theDeviceIO.WriteOutBit(Y_HOT_LINE_MTP_ALIVE + pItem->iRow-1, TRUE);
}

void CFormTeach_PDT_IF::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);
	switch(nIDEvent)
	{
	case 0:
		LBGrid_Refresh();
		LWGrid_Refresh();
		HotLineGrid_Refresh();
		SetTimer(nIDEvent, 200, NULL);
		break;
	}
	CFormView::OnTimer(nIDEvent);
}


void CFormTeach_PDT_IF::ClickGxbtnShuttle1()
{
	m_Jig = JIG_ID_A;
	ShuttleBtn_Refresh();
}


void CFormTeach_PDT_IF::ClickGxbtnShuttle2()
{
	m_Jig = JIG_ID_B;
	ShuttleBtn_Refresh();
}


void CFormTeach_PDT_IF::ClickGxbtnEditMode()
{
	if(theProcBank.MachineIsRunState() == FALSE)
	{
		m_bEditMode = !m_bEditMode;
		if(m_bEditMode)
			CGxUICtrl::SetButtonColor(this, IDC_GXBTN_EDIT_MODE, GXCOLOR_RED);
		else
			CGxUICtrl::SetButtonColor(this, IDC_GXBTN_EDIT_MODE, GXCOLOR_OFF);
	}
}


void CFormTeach_PDT_IF::ClickGxbtnStateClear()
{
	CGxMsgBox	dlgMsgBox;
	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
		_T("물류통신을 초기화 하시겠습니까?"), 
		_T("Do you want I/F State init?"),		
		_T("Ban muon khoi tao lai trang thai truyen thong I/F?"), GetMainHandler()->m_nLangIdx );

	int nRet = dlgMsgBox.DoModal();
	if(nRet != IDOK)
	{
		return;
	}
	theProcBank.m_bPDT_IF_State_Clear = TRUE;
}
