// FormReportStatisticsNGLastResult.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormReportStatisticsNGLastResult.h"

#include "GUIDefine.h"


//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxStaticEx.h"
#include "UI\GausGUI\GxButtonEx.h"
#include <algorithm>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



// CFormReportStatisticsNGLastResult

IMPLEMENT_DYNCREATE(CFormReportStatisticsNGLastResult, CFormView)

CFormReportStatisticsNGLastResult::CFormReportStatisticsNGLastResult()
	: CFormView(CFormReportStatisticsNGLastResult::IDD)
{
	m_eIDX_Shift=DAY_TIME;
	m_JigSelect = JIG_ID_A;
	//kjpark 20180121 NG 판정 통계 UI 추가 Azone, MTPWrite, MTPVerify 별로 정리
	m_defectType = AZONE_DEFECT_NG;
}

CFormReportStatisticsNGLastResult::~CFormReportStatisticsNGLastResult()
{
}

void CFormReportStatisticsNGLastResult::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_GRID_LIST_STATIC_NG_LIST_CH1, m_NGResultStaticGrid_CH1);
	DDX_GridControl(pDX, IDC_GRID_LIST_STATIC_NG_LIST_CH2, m_NGResultStaticGrid_CH2);
}

BEGIN_MESSAGE_MAP(CFormReportStatisticsNGLastResult, CFormView)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

END_MESSAGE_MAP()


// CFormReportStatisticsNGLastResult 진단입니다.

#ifdef _DEBUG
void CFormReportStatisticsNGLastResult::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormReportStatisticsNGLastResult::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormReportStatisticsNGLastResult 메시지 처리기입니다.


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
LRESULT CFormReportStatisticsNGLastResult::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	time= CTime::GetCurrentTime();
	CEtc::ApplyTimeOffset(time, theConfigBank.m_Option.m_OffsetHour, 0);
	m_sel_time.Format(_T("%04u-%02u-%02u"),time.GetYear(), time.GetMonth(), time.GetDay());
	sel_date=0;
	if((int)time.GetHour()<19) //현재 시간에 맞춰서 작업조 설정
		m_eIDX_Shift=DAY_TIME;
	else
		m_eIDX_Shift=NIGHT_TIME;
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		ClearData();
		NGListUdate(m_eIDX_Shift);		
	}
	else
	{

	}

	return 0;
}


// 표시 언어가 변경되었다.
LRESULT CFormReportStatisticsNGLastResult::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXLBL_ALARM_DAY);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormReportStatisticsNGLastResult::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}
BEGIN_EVENTSINK_MAP(CFormReportStatisticsNGLastResult, CFormView)
	ON_EVENT(CFormReportStatisticsNGLastResult, IDC_GXLBL_LASTRESULT_DAY, DISPID_CLICK, CFormReportStatisticsNGLastResult::ClickGxlblAlarmDay, VTS_NONE)
	ON_EVENT(CFormReportStatisticsNGLastResult, IDC_GXLBL_LASTRESULT_NIGHT, DISPID_CLICK, CFormReportStatisticsNGLastResult::ClickGxlblAlarmNight, VTS_NONE)	
	ON_EVENT(CFormReportStatisticsNGLastResult, IDC_GXLBL_LASTRESULT_PREV, DISPID_CLICK, CFormReportStatisticsNGLastResult::ClickGxlblAlarmPrev, VTS_NONE)
	ON_EVENT(CFormReportStatisticsNGLastResult, IDC_GXLBL_LASTRESULT_NEXT, DISPID_CLICK, CFormReportStatisticsNGLastResult::ClickGxlblAlarmNext, VTS_NONE)
	ON_EVENT(CFormReportStatisticsNGLastResult, IDC_GXLBL_LASTRESULT_TODAY, DISPID_CLICK, CFormReportStatisticsNGLastResult::ClickGxlblAlarmToday, VTS_NONE)	

	ON_EVENT(CFormReportStatisticsNGLastResult, IDC_GXLBL_LAST_RESULT_AJIG, DISPID_CLICK, CFormReportStatisticsNGLastResult::ClickGxlblLastReultA, VTS_NONE)
	ON_EVENT(CFormReportStatisticsNGLastResult, IDC_GXLBL_LAST_RESULT_BJIG, DISPID_CLICK, CFormReportStatisticsNGLastResult::ClickGxlblLastReultB, VTS_NONE)

	//kjpark 20180121 NG 판정 통계 UI 추가 Azone, MTPWrite, MTPVerify 별로 정리
	ON_EVENT(CFormReportStatisticsNGLastResult, IDC_GXLBL_LAST_RESULT_AZONE, DISPID_CLICK, CFormReportStatisticsNGLastResult::ClickGxlblLastReultAzone, VTS_NONE)
	ON_EVENT(CFormReportStatisticsNGLastResult, IDC_GXLBL_LAST_RESULT_MTPWRITE, DISPID_CLICK, CFormReportStatisticsNGLastResult::ClickGxlblLastReultMTPWrite, VTS_NONE)
	ON_EVENT(CFormReportStatisticsNGLastResult, IDC_GXLBL_LAST_RESULT_MTPVERIFY, DISPID_CLICK, CFormReportStatisticsNGLastResult::ClickGxlblLastReultMTPVerify, VTS_NONE)

	
END_EVENTSINK_MAP()

//strStatisticsFileName.Format(_T("%s\\%04u-%02u-%02u\\NGResultStatistics.csv"	)	, theConfigBank.m_System.m_strDebugLogPath, time.GetYear(), time.GetMonth(), time.GetDay());
void CFormReportStatisticsNGLastResult::NGListUdate( int sel )
{
	CTime today_time= CTime::GetCurrentTime();
	CEtc::ApplyTimeOffset(today_time, theConfigBank.m_Option.m_OffsetHour, 0);
	CGxButtonEx* pAlarmDay = (CGxButtonEx*)GetDlgItem( IDC_GXLBL_LASTRESULT_DAY );
	CGxButtonEx* pAlarmNight = (CGxButtonEx*)GetDlgItem( IDC_GXLBL_LASTRESULT_NIGHT );	
		CGxButtonEx* pAlarmToday = (CGxButtonEx*)GetDlgItem( IDC_GXLBL_LASTRESULT_TODAY );
	
	CGxButtonEx* pLastResult_A = (CGxButtonEx*)GetDlgItem( IDC_GXLBL_LAST_RESULT_AJIG );
	CGxButtonEx* pLastResult_B = (CGxButtonEx*)GetDlgItem( IDC_GXLBL_LAST_RESULT_BJIG );
	CGxButtonEx* pLastResult_C = (CGxButtonEx*)GetDlgItem( IDC_GXLBL_LAST_RESULT_CJIG );

	//kjpark 20180121 NG 판정 통계 UI 추가 Azone, MTPWrite, MTPVerify 별로 정리
	CGxButtonEx* pLastResult_AZone = (CGxButtonEx*)GetDlgItem( IDC_GXLBL_LAST_RESULT_AZONE );
	CGxButtonEx* pLastResult_MTPWrite = (CGxButtonEx*)GetDlgItem( IDC_GXLBL_LAST_RESULT_MTPWRITE );
	CGxButtonEx* pLastResult_MTPVVerify = (CGxButtonEx*)GetDlgItem( IDC_GXLBL_LAST_RESULT_MTPVERIFY );

	if(m_JigSelect == JIG_ID_A)
	{
		pLastResult_A->SetBackColor(Color(Color::Lime).ToCOLORREF());
		pLastResult_B->SetBackColor(Color(Color::White).ToCOLORREF());
		pLastResult_C->SetBackColor(Color(Color::White).ToCOLORREF());
	}
	else if(m_JigSelect == JIG_ID_B)
	{
		pLastResult_A->SetBackColor(Color(Color::White).ToCOLORREF());
		pLastResult_B->SetBackColor(Color(Color::Lime).ToCOLORREF());
		pLastResult_C->SetBackColor(Color(Color::White).ToCOLORREF());
	}

	//kjpark 20180121 NG 판정 통계 UI 추가 Azone, MTPWrite, MTPVerify 별로 정리
	if(m_defectType == AZONE_DEFECT_NG)
	{
		pLastResult_AZone->SetBackColor(Color(Color::Lime).ToCOLORREF());
		pLastResult_MTPWrite->SetBackColor(Color(Color::White).ToCOLORREF());
		pLastResult_MTPVVerify->SetBackColor(Color(Color::White).ToCOLORREF());
	}
	else if(m_defectType == MTP_WRITE_DEFECT_NG)
	{
		pLastResult_AZone->SetBackColor(Color(Color::White).ToCOLORREF());
		pLastResult_MTPWrite->SetBackColor(Color(Color::Lime).ToCOLORREF());
		pLastResult_MTPVVerify->SetBackColor(Color(Color::White).ToCOLORREF());
	}
	else if(m_defectType == MTP_VIRIFY_DEFECT_NG)
	{
		pLastResult_AZone->SetBackColor(Color(Color::White).ToCOLORREF());
		pLastResult_MTPWrite->SetBackColor(Color(Color::White).ToCOLORREF());
		pLastResult_MTPVVerify->SetBackColor(Color(Color::Lime).ToCOLORREF());
	}
	


	CGxStaticEx* pAlarmDate = (CGxStaticEx*)GetDlgItem( IDC_GXLBL_LASTRESULT_DATE );
	CString logpath;
	CString path;
	CString filename;
	CFileFind finder;
	CFileFind errorfinder;
	std::vector<CString> m_vecDir;
	std::vector<CString> m_vecDirTemp;
	if(time.GetDay()==today_time.GetDay())
	{
		pAlarmToday->SetBackColor(Color(Color::Lime).ToCOLORREF());

	}
	else
	{
		pAlarmToday->SetBackColor(Color(Color::White).ToCOLORREF());
	}

	if(sel==DAY_TIME)
	{
		pAlarmDay->SetBackColor(Color(Color::Lime).ToCOLORREF());
		pAlarmNight->SetBackColor(Color(Color::White).ToCOLORREF());
	}
	else if(sel==NIGHT_TIME)
	{
		
		pAlarmDay->SetBackColor(Color(Color::White).ToCOLORREF());
		pAlarmNight->SetBackColor(Color(Color::Lime).ToCOLORREF());
	}	
	else
	{
		pAlarmDay->SetBackColor(Color(Color::White).ToCOLORREF());
		pAlarmNight->SetBackColor(Color(Color::White).ToCOLORREF());
	}
	path.Format(_T("%s\\*"), theConfigBank.m_System.m_strDebugLogPath);
	
	pAlarmDate->SetCaption(m_sel_time);
	BOOL bWorking = finder.FindFile(path);//해당 폴더에 전체 디렉토리를 검색

	m_mapLastResult_CH1.clear();
	m_mapLastResult_CH2.clear();

	while (bWorking)
	{   
		bWorking = finder.FindNextFile();
		if(finder.IsDirectory() && !finder.IsDots())
		{
			CString dir= finder.GetFileName();

			if(dir!=m_sel_time )//선택한 날짜만 지정함
			{
				continue;
			}
			logpath.Format(_T("%s\\%s\\NGResultStatistics.csv"),theConfigBank.m_System.m_strDebugLogPath, dir);
			BOOL errorexist=errorfinder.FindFile(logpath);
			if(errorexist)
			{
				m_vecDirTemp.push_back(dir);	//로그가 저장된 폴더안에서 ErrorLog.csv파일이 있는 경로를 저장함.
			}
			path.Delete(path.GetLength()-1);
		}
	}

	int m_vecLength=(int)m_vecDirTemp.size();
	if (m_vecLength==0) 
	{
		ListUpdate_CH1();
		ListUpdate_CH2();
		UpdateLastResultGrid_CH1();
		UpdateLastResultGrid_CH2();
		return;
	}

	//kjpark 20180121 NG 판정 통계 UI 추가 Azone, MTPWrite, MTPVerify 별로 정리
	CString sLine, strJig, strCH, strTimePart, strDefectType, strLastResult;
	int nTimePart = 0;
	
	FILE *fStream;

	filename.Format(_T("%s\\%s\\NGResultStatistics.csv"), theConfigBank.m_System.m_strDebugLogPath, m_vecDirTemp[0]);
	errno_t e = _tfopen_s(&fStream, filename.GetString(), _T("r,ccs=UTF-8"));
	if (e != 0) return ;

	CStdioFile		file(fStream);
	
	while ( file.ReadString(sLine) )
	{
		if (sLine.IsEmpty() )	continue;			// 공백일 경우 무시
		if (sLine.Left(1) == _T("?"))	continue;	// 처음 글자가 " ? " 일경우 다음 문장으로 넘어간다.;
		if (sLine.Left(1) == _T("/"))	continue;	// 처음 글자가 " / " 일경우 다음 문장으로 넘어간다.;
		if (sLine.Left(1) == _T("!"))	continue;	// 처음 글자가 " ! " 일경우 다음 문장으로 넘어간다.;
		if (sLine.Left(1) == _T(";"))	continue;	// 처음 글자가 " ; " 일경우 다음 문장으로 넘어간다.;

		// 문장 맨뒤에 ;;가 붙어나와서 삭제 [11/10/2017 OSC]
		sLine.Remove(_T(';'));
		::AfxExtractSubString( strJig,				sLine	, 0, ',');		
		::AfxExtractSubString( strCH,				sLine	, 1, ',');		
		::AfxExtractSubString( strTimePart,		sLine	, 2, ',');		
		//kjpark 20180121 NG 판정 통계 UI 추가 Azone, MTPWrite, MTPVerify 별로 정리
		::AfxExtractSubString( strDefectType,		sLine	, 3, ',');		
		::AfxExtractSubString( strLastResult,		sLine	, 4, ',');		


		JIG_ID jig = (JIG_ID)_ttoi(strJig);
		JIG_CH ch = (JIG_CH)_ttoi(strCH);
		WorkPartSub partTime = (WorkPartSub)_ttoi(strTimePart);
		//kjpark 20180121 NG 판정 통계 UI 추가 Azone, MTPWrite, MTPVerify 별로 정리
		DEFECT_TYPE defectType = (DEFECT_TYPE)_ttoi(strDefectType);
		
		//map<CString, int> m_mapLastResult_CH1;
		if(sel == partTime)
		{
			if(m_JigSelect == jig)
			{
				//kjpark 20180121 NG 판정 통계 UI 추가 Azone, MTPWrite, MTPVerify 별로 정리
				if(m_defectType == defectType)
				{
					pair<map<CString, int>::iterator, bool > pr;
					if(ch == JIG_CH_1)
					{
						pr = m_mapLastResult_CH1.insert(pair<CString,int>(strLastResult, 1));
						if(pr.second!=TRUE) //알람코드가 중복이 된다면 수량을 ++ 시킨다.
						{
							m_mapLastResult_CH1[strLastResult]++;					
						}
					}
					else
					{
						pr = m_mapLastResult_CH2.insert(pair<CString,int>(strLastResult, 1));
						if(pr.second!=TRUE) //알람코드가 중복이 된다면 수량을 ++ 시킨다.
						{
							m_mapLastResult_CH2[strLastResult]++;					
						}
					}
				}				
			}
		}
	}
	

	file.Close();
	
	ListUpdate_CH1();
	ListUpdate_CH2();
	UpdateLastResultGrid_CH1();
	UpdateLastResultGrid_CH2();

	//kjpark 20180121 NG 판정 통계 UI 추가 Azone, MTPWrite, MTPVerify 별로 정리
	CGxStaticEx* pAlarmTotalCount1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_LASTRESULT_COUNT_CH1);
	CGxStaticEx* pAlarmTotalCount2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_LASTRESULT_COUNT_CH2);

	int nTotalcount_ch1 = 0, nTotalcount_ch2 = 0;

	map<CString, int>::iterator itmap = m_mapLastResult_CH1.begin();
	for(; itmap != m_mapLastResult_CH1.end(); ++ itmap)
	{
		nTotalcount_ch1 = nTotalcount_ch1 + itmap->second;
	}

	itmap = m_mapLastResult_CH2.begin();
	for(; itmap != m_mapLastResult_CH2.end(); ++ itmap)
	{
		nTotalcount_ch2 = nTotalcount_ch2 + itmap->second;
	}

	CString strTotalCount_ch1, strTotalCount_ch2;
	strTotalCount_ch1.Format(_T("%d"), nTotalcount_ch1);
	strTotalCount_ch2.Format(_T("%d"), nTotalcount_ch2);
	pAlarmTotalCount1->SetCaption(strTotalCount_ch1);	
	pAlarmTotalCount2->SetCaption(strTotalCount_ch2);
}

template<template <typename> class P = std::less >
struct compare_pair_second {
	template<class T1, class T2> bool operator()(const std::pair<T1, T2>&left, const std::pair<T1, T2>&right) {
		return P<T2>()(left.second, right.second);
	}
};

void CFormReportStatisticsNGLastResult::ClearData()
{
	CGxStaticEx* pAlarmTotalCount = (CGxStaticEx*)GetDlgItem( IDC_LIST_LASTRESULT_COUNT );
	pAlarmTotalCount->SetCaption(_T("0"));	
	
}
void CFormReportStatisticsNGLastResult::ListUpdate_CH1()
{
	ClearData();

	if(m_NGResultStaticGrid_CH1.GetSafeHwnd() == NULL)return;

	m_NGResultStaticGrid_CH1.SetEditable(FALSE);
	m_NGResultStaticGrid_CH1.SetRowCount(100);
	m_NGResultStaticGrid_CH1.SetColumnCount(3);	
	m_NGResultStaticGrid_CH1.SetDefCellHeight(57);
	m_NGResultStaticGrid_CH1.SetFixedColumnCount(1); 
	m_NGResultStaticGrid_CH1.SetFixedRowCount(1);
	m_NGResultStaticGrid_CH1.SetRowResize(FALSE);
	m_NGResultStaticGrid_CH1.SetColumnResize(FALSE);
	m_NGResultStaticGrid_CH1.SetListMode(FALSE);
	m_NGResultStaticGrid_CH1.Refresh();

	m_NGResultStaticGrid_CH1.SetColumnWidth(0,50);
	m_NGResultStaticGrid_CH1.SetColumnWidth(1,430);
	m_NGResultStaticGrid_CH1.SetColumnWidth(2,104);

	m_NGResultStaticGrid_CH1.SetRowHeight(0, 20);
	m_NGResultStaticGrid_CH1.SetItemText(0,0,_T("No"));
	m_NGResultStaticGrid_CH1.SetItemText(0,1,_T("LAST RESULT"));
	m_NGResultStaticGrid_CH1.SetItemText(0,2,_T("COUNT"));	



	DWORD dwTextStyle = DT_CENTER|DT_SINGLELINE|DT_VCENTER;
	LOGFONT lf;
	CFont* pGridFont =m_NGResultStaticGrid_CH1.GetFont();
	pGridFont->GetLogFont(&lf);

	memcpy(lf.lfFaceName, _T("굴림체"), sizeof(_T("굴림체")));
	lf.lfHeight = 13;
	lf.lfWeight  = FW_BOLD;

	for(int iRow = 0; iRow<m_NGResultStaticGrid_CH1.GetRowCount(); iRow++)
	{
		for(int iCol = 0; iCol<m_NGResultStaticGrid_CH1.GetColumnCount(); iCol++)
		{
			m_NGResultStaticGrid_CH1.SetItemFont(iRow, iCol, &lf);
			m_NGResultStaticGrid_CH1.SetItemFormat(iRow, iCol, dwTextStyle);
		}
		m_NGResultStaticGrid_CH1.SetRowHeight(iRow, 30);
	}
}

void CFormReportStatisticsNGLastResult::ListUpdate_CH2()
{
	ClearData();
	
	if(m_NGResultStaticGrid_CH2.GetSafeHwnd() == NULL)return;

	m_NGResultStaticGrid_CH2.SetEditable(FALSE);
	m_NGResultStaticGrid_CH2.SetRowCount(100);
	m_NGResultStaticGrid_CH2.SetColumnCount(3);	
	m_NGResultStaticGrid_CH2.SetDefCellHeight(57);
	m_NGResultStaticGrid_CH2.SetFixedColumnCount(1); 
	m_NGResultStaticGrid_CH2.SetFixedRowCount(1);
	m_NGResultStaticGrid_CH2.SetRowResize(FALSE);
	m_NGResultStaticGrid_CH2.SetColumnResize(FALSE);
	m_NGResultStaticGrid_CH2.SetListMode(FALSE);
	m_NGResultStaticGrid_CH2.Refresh();
	
	m_NGResultStaticGrid_CH2.SetColumnWidth(0,50);
	m_NGResultStaticGrid_CH2.SetColumnWidth(1,430);
	m_NGResultStaticGrid_CH2.SetColumnWidth(2,104);
	
	m_NGResultStaticGrid_CH2.SetRowHeight(0, 20);
	m_NGResultStaticGrid_CH2.SetItemText(0,0,_T("No"));
	m_NGResultStaticGrid_CH2.SetItemText(0,1,_T("LAST RESULT"));
	m_NGResultStaticGrid_CH2.SetItemText(0,2,_T("COUNT"));	


	DWORD dwTextStyle = DT_CENTER|DT_SINGLELINE|DT_VCENTER;
	LOGFONT lf;
	CFont* pGridFont =m_NGResultStaticGrid_CH2.GetFont();
	pGridFont->GetLogFont(&lf);

	memcpy(lf.lfFaceName, _T("굴림체"), sizeof(_T("굴림체")));
	lf.lfHeight = 13;
	lf.lfWeight  = FW_BOLD;

	for(int iRow = 0; iRow<m_NGResultStaticGrid_CH2.GetRowCount(); iRow++)
	{
		for(int iCol = 0; iCol<m_NGResultStaticGrid_CH2.GetColumnCount(); iCol++)
		{
			m_NGResultStaticGrid_CH2.SetItemFont(iRow, iCol, &lf);
			m_NGResultStaticGrid_CH2.SetItemFormat(iRow, iCol, dwTextStyle);
		}
		m_NGResultStaticGrid_CH2.SetRowHeight(iRow, 30);
	}
}

void CFormReportStatisticsNGLastResult::UpdateLastResultGrid_CH1()
{
	CString sNum;
	if(m_mapLastResult_CH1.size() > 30)
		m_NGResultStaticGrid_CH1.SetRowCount(m_mapLastResult_CH1.size()+1);
	else
		m_NGResultStaticGrid_CH1.SetRowCount(30);

	if(m_mapLastResult_CH1.size() > 0)
	{
		std::vector< std::pair< CString, int > > vectorData;
		for (auto iter = m_mapLastResult_CH1.begin(); iter != m_mapLastResult_CH1.end(); ++iter)
			vectorData.emplace_back(std::make_pair(iter->first, iter->second));


		//저장된 Map의 value값을 vector값에 대입하여 내림차순으로 정렬한다.
		std::sort(vectorData.begin(), vectorData.end(), compare_pair_second<std::greater>());
		int i = 1;
		for (auto iter = vectorData.begin(); iter != vectorData.end(); ++iter)
		{
			sNum.Format(_T("%d"),i);
			CString strLastResult, strLastResultCount;	
			strLastResult = iter->first;
			strLastResultCount.Format(_T("%d"), iter->second);
			m_NGResultStaticGrid_CH1.SetItemText(i, 0,sNum);
			m_NGResultStaticGrid_CH1.SetItemText(i, 1,iter->first);
			m_NGResultStaticGrid_CH1.SetItemText(i, 2, strLastResultCount);

			i++;
		}		
	}
	else
	{
		for(int i = 0; i < m_NGResultStaticGrid_CH1.GetRowCount(); i++ )
		{
			if ( i == 0 )	continue;			

			sNum.Format(_T("%d"),i);
			for(int j = 0; j < m_NGResultStaticGrid_CH1.GetColumnCount(); j++ )
			{
				if ( j == 0 )	m_NGResultStaticGrid_CH1.SetItemText(i, j, sNum);
				else			
				{
					m_NGResultStaticGrid_CH1.SetItemText(i,j, _T(""));
				}
			}
		}
	}
	
	
	// 한 화면이 넘어가서 횡스크롤은 살린다 [9/27/2017 OSC]
	m_NGResultStaticGrid_CH1.ShowScrollBar(SB_HORZ,FALSE);		//	횡 스크롤 제거
	m_NGResultStaticGrid_CH1.ShowScrollBar(SB_VERT,TRUE);		//	종 스크롤 제거
	//m_NGResultStaticGrid_CH1.ShowScrollBar(SB_VERT,FALSE);		//	종 스크롤 제거
	m_NGResultStaticGrid_CH1.Refresh();
}

void CFormReportStatisticsNGLastResult::UpdateLastResultGrid_CH2()
{
	CString sNum;
	if(m_mapLastResult_CH2.size() > 30)
		m_NGResultStaticGrid_CH2.SetRowCount(m_mapLastResult_CH2.size()+1);
	else
		m_NGResultStaticGrid_CH2.SetRowCount(30);

	if(m_mapLastResult_CH2.size() > 0)
	{
		std::vector< std::pair< CString, int > > vectorData;
		for (auto iter = m_mapLastResult_CH2.begin(); iter != m_mapLastResult_CH2.end(); ++iter)
			vectorData.emplace_back(std::make_pair(iter->first, iter->second));


		//저장된 Map의 value값을 vector값에 대입하여 내림차순으로 정렬한다.
		std::sort(vectorData.begin(), vectorData.end(), compare_pair_second<std::greater>());
		int i = 1;
		for (auto iter = vectorData.begin(); iter != vectorData.end(); ++iter)
		{
			sNum.Format(_T("%d"),i);
			CString strLastResult, strLastResultCount;	
			strLastResult = iter->first;
			strLastResultCount.Format(_T("%d"), iter->second);
			m_NGResultStaticGrid_CH2.SetItemText(i, 0,sNum);
			m_NGResultStaticGrid_CH2.SetItemText(i, 1,iter->first);
			m_NGResultStaticGrid_CH2.SetItemText(i, 2, strLastResultCount);
			i++;
		}	
	}	
	else
	{
		for(int i = 0; i < m_NGResultStaticGrid_CH2.GetRowCount(); i++ )
		{
			if ( i == 0 )	continue;			

			sNum.Format(_T("%d"),i);
			for(int j = 0; j < m_NGResultStaticGrid_CH2.GetColumnCount(); j++ )
			{
				if ( j == 0 )	m_NGResultStaticGrid_CH2.SetItemText(i, j, sNum);
				else			
				{
					m_NGResultStaticGrid_CH2.SetItemText(i,j, _T(""));
				}
			}
		}
	}


	// 한 화면이 넘어가서 횡스크롤은 살린다 [9/27/2017 OSC]
	m_NGResultStaticGrid_CH2.ShowScrollBar(SB_HORZ,FALSE);		//	횡 스크롤 제거
	m_NGResultStaticGrid_CH2.ShowScrollBar(SB_VERT,TRUE);		//	종 스크롤 제거
	//m_NGResultStaticGrid_CH1.ShowScrollBar(SB_VERT,FALSE);		//	종 스크롤 제거
	m_NGResultStaticGrid_CH2.Refresh();
}

void CFormReportStatisticsNGLastResult::ClickGxlblAlarmDay()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRESENT"),_T("DAY"));
	m_eIDX_Shift=DAY_TIME;
	NGListUdate(m_eIDX_Shift);
}


void CFormReportStatisticsNGLastResult::ClickGxlblAlarmNight()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRESENT"),_T("NIGHT"));
	m_eIDX_Shift=NIGHT_TIME;
	NGListUdate(m_eIDX_Shift);
}

void CFormReportStatisticsNGLastResult::ClickGxlblLastReultA()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRESENT"),_T("JIG A"));
	CGxStaticEx* pAlarmDate = (CGxStaticEx*)GetDlgItem( IDC_GXLBL_LAST_RESULT_AJIG );
	
	m_sel_time.Format(_T("%04u-%02u-%02u"),time.GetYear(), time.GetMonth(), time.GetDay());
	m_JigSelect = JIG_ID_A;
	NGListUdate(m_eIDX_Shift);
}

void CFormReportStatisticsNGLastResult::ClickGxlblLastReultB()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRESENT"),_T("JIG B"));
	CGxStaticEx* pAlarmDate = (CGxStaticEx*)GetDlgItem( IDC_GXLBL_LAST_RESULT_BJIG );

	m_sel_time.Format(_T("%04u-%02u-%02u"),time.GetYear(), time.GetMonth(), time.GetDay());
	m_JigSelect = JIG_ID_B;
	NGListUdate(m_eIDX_Shift);
}


//kjpark 20180121 NG 판정 통계 UI 추가 Azone, MTPWrite, MTPVerify 별로 정리
void CFormReportStatisticsNGLastResult::ClickGxlblLastReultAzone()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRESENT"),_T("ZONE A"));
	CGxStaticEx* pAlarmDate = (CGxStaticEx*)GetDlgItem( IDC_GXLBL_LAST_RESULT_AZONE );

	m_sel_time.Format(_T("%04u-%02u-%02u"),time.GetYear(), time.GetMonth(), time.GetDay());
	m_defectType = AZONE_DEFECT_NG;
	NGListUdate(m_eIDX_Shift);
}

//kjpark 20180121 NG 판정 통계 UI 추가 Azone, MTPWrite, MTPVerify 별로 정리
void CFormReportStatisticsNGLastResult::ClickGxlblLastReultMTPWrite()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRESENT"),_T("MTP WRITE"));
	CGxStaticEx* pAlarmDate = (CGxStaticEx*)GetDlgItem( IDC_GXLBL_LAST_RESULT_AZONE );

	m_sel_time.Format(_T("%04u-%02u-%02u"),time.GetYear(), time.GetMonth(), time.GetDay());
	m_defectType = MTP_WRITE_DEFECT_NG;
	NGListUdate(m_eIDX_Shift);	
}

//kjpark 20180121 NG 판정 통계 UI 추가 Azone, MTPWrite, MTPVerify 별로 정리
void CFormReportStatisticsNGLastResult::ClickGxlblLastReultMTPVerify()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRESENT"),_T("MTP VERIFY"));
	CGxStaticEx* pAlarmDate = (CGxStaticEx*)GetDlgItem( IDC_GXLBL_LAST_RESULT_AZONE );

	m_sel_time.Format(_T("%04u-%02u-%02u"),time.GetYear(), time.GetMonth(), time.GetDay());
	m_defectType = MTP_VIRIFY_DEFECT_NG;
	NGListUdate(m_eIDX_Shift);
}
void CFormReportStatisticsNGLastResult::ClickGxlblAlarmPrev()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRESENT"),_T("PREV"));
	CGxStaticEx* pAlarmDate = (CGxStaticEx*)GetDlgItem( IDC_GXLBL_LASTRESULT_DATE );
	ClearData();
	//sel_date++;
	//time= CTime::GetCurrentTime();
	CTimeSpan span(1,0,0,0);
	time=time-span;
	m_sel_time.Format(_T("%04u-%02u-%02u"),time.GetYear(), time.GetMonth(), time.GetDay());

	NGListUdate(m_eIDX_Shift);
}

void CFormReportStatisticsNGLastResult::ClickGxlblAlarmNext()
{
	CGxStaticEx* pAlarmDate = (CGxStaticEx*)GetDlgItem( IDC_GXLBL_LASTRESULT_DATE );
	ClearData();
	//sel_date++;
	CTime cur_time= CTime::GetCurrentTime();
	CEtc::ApplyTimeOffset(cur_time, theConfigBank.m_Option.m_OffsetHour, 0);
	if(cur_time.GetDay()==time.GetDay())//오늘 이후의 날짜는 값이 없으므로 return
	{
		return;
		
	}
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRESENT"),_T("NEXT"));
	CTimeSpan span(1,0,0,0);

	time=time+span;
	m_sel_time.Format(_T("%04u-%02u-%02u"),time.GetYear(), time.GetMonth(), time.GetDay());
	NGListUdate(m_eIDX_Shift);
}


void CFormReportStatisticsNGLastResult::ClickGxlblAlarmToday()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRESENT"),_T("TODAY"));
	ClearData();
	time= CTime::GetCurrentTime();
	CEtc::ApplyTimeOffset(time, theConfigBank.m_Option.m_OffsetHour, 0);
	m_sel_time.Format(_T("%04u-%02u-%02u"),time.GetYear(), time.GetMonth(), time.GetDay());
	NGListUdate(m_eIDX_Shift);
}



void CFormReportStatisticsNGLastResult::ClickGxlblAlarmTotal()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRESENT"),_T("TOTAL"));
	m_eIDX_Shift=eIDX_Shift_Today-1;
	NGListUdate(m_eIDX_Shift);
}
