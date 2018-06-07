// FormReport2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormReport2.h"

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



// CFormReport2

IMPLEMENT_DYNCREATE(CFormReport2, CFormView)

CFormReport2::CFormReport2()
	: CFormView(CFormReport2::IDD)
{
	m_eIDX_Shift=eIDX_Shift_DAY;
}

CFormReport2::~CFormReport2()
{
}

void CFormReport2::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormReport2, CFormView)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

END_MESSAGE_MAP()


// CFormReport2 진단입니다.

#ifdef _DEBUG
void CFormReport2::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormReport2::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormReport2 메시지 처리기입니다.


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
LRESULT CFormReport2::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	time= CTime::GetCurrentTime();
	CEtc::ApplyTimeOffset(time, theConfigBank.m_Option.m_OffsetHour, 0);
	m_sel_time.Format(_T("%04u-%02u-%02u"),time.GetYear(), time.GetMonth(), time.GetDay());
	sel_date=0;
	if((int)time.GetHour()<14) //현재 시간에 맞춰서 작업조 설정
		m_eIDX_Shift=eIDX_Shift_DAY;
	else if((int)time.GetHour()<22)
		m_eIDX_Shift=eIDX_Shift_SW;
	else
		m_eIDX_Shift=eIDX_Shift_GY;
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		//SetHideLevelCHECK(FALSE);
		m_alarm_map.clear();
		ClearData();
		AlarmUdate(m_eIDX_Shift);
		
	}
	else
	{

	}

	return 0;
}


// 표시 언어가 변경되었다.
LRESULT CFormReport2::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXLBL_ALARM_DAY);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormReport2::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}
BEGIN_EVENTSINK_MAP(CFormReport2, CFormView)
	ON_EVENT(CFormReport2, IDC_GXLBL_ALARM_DAY, DISPID_CLICK, CFormReport2::ClickGxlblAlarmDay, VTS_NONE)
	ON_EVENT(CFormReport2, IDC_GXLBL_ALARM_SW, DISPID_CLICK, CFormReport2::ClickGxlblAlarmSw, VTS_NONE)
	ON_EVENT(CFormReport2, IDC_GXLBL_ALARM_GY, DISPID_CLICK, CFormReport2::ClickGxlblAlarmGy, VTS_NONE)
	ON_EVENT(CFormReport2, IDC_GXLBL_ALARM_PREV, DISPID_CLICK, CFormReport2::ClickGxlblAlarmPrev, VTS_NONE)
	ON_EVENT(CFormReport2, IDC_GXLBL_ALARM_NEXT, DISPID_CLICK, CFormReport2::ClickGxlblAlarmNext, VTS_NONE)
	ON_EVENT(CFormReport2, IDC_GXLBL_ALARM_TODAY, DISPID_CLICK, CFormReport2::ClickGxlblAlarmToday, VTS_NONE)
	ON_EVENT(CFormReport2, IDC_GXLBL_ALARM_TOTAL, DISPID_CLICK, CFormReport2::ClickGxlblAlarmTotal, VTS_NONE)
	//ON_EVENT(CFormReport2, IDC_GXBTN_HIDE_CHECK, DISPID_CLICK, CFormReport2::ClickGxbtnHideCheck, VTS_NONE)
END_EVENTSINK_MAP()

#if 0
void CFormReport2::SetHideLevelCHECK(BOOL bHide)
{
	m_bHideLevelCHECK = bHide;
	if(m_bHideLevelCHECK)
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_HIDE_CHECK_LAMP, GXCOLOR_ON);
	else
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_HIDE_CHECK_LAMP, GXCOLOR_OFF);
}
#endif
void CFormReport2::AlarmUdate( int sel )
{
	CTime today_time= CTime::GetCurrentTime();
	CEtc::ApplyTimeOffset(today_time, theConfigBank.m_Option.m_OffsetHour, 0);
	CGxButtonEx* pAlarmDay = (CGxButtonEx*)GetDlgItem( IDC_GXLBL_ALARM_DAY );
	CGxButtonEx* pAlarmSw = (CGxButtonEx*)GetDlgItem( IDC_GXLBL_ALARM_SW );
	CGxButtonEx* pAlarmGy = (CGxButtonEx*)GetDlgItem( IDC_GXLBL_ALARM_GY );
	CGxButtonEx* pAlarmTotal = (CGxButtonEx*)GetDlgItem( IDC_GXLBL_ALARM_TOTAL );
	CGxButtonEx* pAlarmToday = (CGxButtonEx*)GetDlgItem( IDC_GXLBL_ALARM_TODAY );
	
	CGxStaticEx* pAlarmDate = (CGxStaticEx*)GetDlgItem( IDC_GXLBL_ALARM_DATE );
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
	if(sel==eIDX_Shift_DAY)
	{
		pAlarmDay->SetBackColor(Color(Color::Lime).ToCOLORREF());
		pAlarmSw->SetBackColor(Color(Color::White).ToCOLORREF());
		pAlarmGy->SetBackColor(Color(Color::White).ToCOLORREF());
		pAlarmTotal->SetBackColor(Color(Color::White).ToCOLORREF());
	}
	else if(sel==eIDX_Shift_SW)
	{
		
		pAlarmDay->SetBackColor(Color(Color::White).ToCOLORREF());
		pAlarmSw->SetBackColor(Color(Color::Lime).ToCOLORREF());
		pAlarmGy->SetBackColor(Color(Color::White).ToCOLORREF());
		pAlarmTotal->SetBackColor(Color(Color::White).ToCOLORREF());
	}
	else if(sel==eIDX_Shift_GY)
	{
		pAlarmDay->SetBackColor(Color(Color::White).ToCOLORREF());
		pAlarmSw->SetBackColor(Color(Color::White).ToCOLORREF());
		pAlarmGy->SetBackColor(Color(Color::Lime).ToCOLORREF());
		pAlarmTotal->SetBackColor(Color(Color::White).ToCOLORREF());
	}
	else
	{
		pAlarmDay->SetBackColor(Color(Color::White).ToCOLORREF());
		pAlarmSw->SetBackColor(Color(Color::White).ToCOLORREF());
		pAlarmGy->SetBackColor(Color(Color::White).ToCOLORREF());
		pAlarmTotal->SetBackColor(Color(Color::Lime).ToCOLORREF());
	}
	path.Format(_T("%s\\*"), theConfigBank.m_System.m_strDebugLogPath);
	
	pAlarmDate->SetCaption(m_sel_time);
	BOOL bWorking = finder.FindFile(path);//해당 폴더에 전체 디렉토리를 검색
	
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
			logpath.Format(_T("%s\\%s\\ErrorLog.csv"),theConfigBank.m_System.m_strDebugLogPath, dir);
			BOOL errorexist=errorfinder.FindFile(logpath);
			if(errorexist)
			{
				m_vecDirTemp.push_back(dir);	//로그가 저장된 폴더안에서 ErrorLog.csv파일이 있는 경로를 저장함.
			}
			path.Delete(path.GetLength()-1);
		}
	}

	int m_vecLength=(int)m_vecDirTemp.size();
	if (m_vecLength==0) return;

	FILE *fStream;

	filename.Format(_T("%s\\%s\\ErrorLog.csv"), theConfigBank.m_System.m_strDebugLogPath, m_vecDirTemp[0]);
	errno_t e = _tfopen_s(&fStream, filename.GetString(), _T("r,ccs=UTF-8"));

	if (e != 0) return;
	CStdioFile		file(fStream);
	CString strBuf, strItem, strSpan, strSpanHour, strSpanMin, strSpanSec;
	CTokenizer t, tSpan;
	ALARM_LOG_DATA data;
	int time_num=0;
	int shift;
	m_alarm_map.clear();
	while(file.ReadString(strBuf))
	{

		t = CTokenizer(strBuf);
		strItem = t.GetNextToken(_T(","));
		
		//m_today_count++; //하루동안 발생한 총 알람 수량
		//기록된 시간값 추출
		if(strItem!=_T("START TIME"))
			time_num=_ttoi(strItem.Right(9));
		
		if(sel==eIDX_Shift_Today)
		{
			shift=3;
		}
	    else if(time_num<14) //작업조 설정
		{
			shift=0;
		}
		else if (time_num<22)
		{
			shift=1;
		}
		else
		{
			shift=2;
		}
		if(shift==m_eIDX_Shift)
		{
			int alarm_code;
			pair<map<int, ALARM_LOG_DATA>::iterator, bool > pr;
 			strItem = t.GetNextToken(_T(","));
 			strSpan = t.GetNextToken(_T(","));
			strItem = t.GetNextToken(_T(","));
			if(strItem!=_T("CODE"))
			{
				alarm_code=_ttoi(strItem);
				strItem = t.GetNextToken(_T(","));
				alarm_input[alarm_code].Alarm_name=strItem;
				strItem = t.GetNextToken(_T(","));
				// CHECK를 숨기는 옵션이 켜져있으면 제외한다 [7/13/2017 OSC]
				if(m_bHideLevelCHECK)
				{
					if(_ttoi(strItem)==ALARM_LEVEL_CHECK)
					{
						continue;
					}
				}
				if(_ttoi(strItem)==ALARM_LEVEL_WARNING)
					alarm_input[alarm_code].nLevel=_T("LIGHT");
				else if(_ttoi(strItem)==ALARM_LEVEL_CHECK)
					alarm_input[alarm_code].nLevel=_T("SERIOUS");	// ALARM, CHECK 따로 구분 안되니 다 ALARM으로 [7/13/2017 OSC]
				else if(_ttoi(strItem)==ALARM_LEVEL_ALARM)
					alarm_input[alarm_code].nLevel=_T("SERIOUS");
				else
					alarm_input[alarm_code].nLevel=_T("PASSWORD");

				tSpan = CTokenizer(strSpan);
				strSpanHour = tSpan.GetNextToken(_T(":"));
				strSpanMin = tSpan.GetNextToken(_T(":"));
				strSpanSec = tSpan.GetNextToken(_T(":"));
				data.nCount = 1;
				data.span = CTimeSpan(0, _tstoi(strSpanHour), _tstoi(strSpanMin), _tstoi(strSpanSec));
				pr = m_alarm_map.insert(pair<int,ALARM_LOG_DATA>(alarm_code, data));
				if(pr.second!=TRUE) //알람코드가 중복이 된다면 수량을 ++ 시킨다.
				{
					m_alarm_map[alarm_code].nCount++;
					m_alarm_map[alarm_code].span += data.span;
				}
			}
			
			
		}
		
	}
	file.Close();
	
	ListUpdate();
}

template<template <typename> class P = std::less >
struct compare_pair_second {
	template<class T1, class T2> bool operator()(const std::pair<T1, T2>&left, const std::pair<T1, T2>&right) {
		return P<T2>()(left.second, right.second);
	}
};

void CFormReport2::ClearData()
{
	CGxStaticEx* pAlarmTotalCount = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_COUNT );

	//20170620 BKH 토탈 sapn time 값 추가
	CGxStaticEx* pAlarmTotalSpan = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_SPAN );
	CGxStaticEx* pAlarmNumber1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_1_1 );
	CGxStaticEx* pAlarmNumber2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_2_1 );
	CGxStaticEx* pAlarmNumber3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_3_1 );
	CGxStaticEx* pAlarmNumber4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_4_1 );
	CGxStaticEx* pAlarmNumber5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_5_1 );
	CGxStaticEx* pAlarmNumber6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_6_1 );
	CGxStaticEx* pAlarmNumber7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_7_1 );
	CGxStaticEx* pAlarmNumber8 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_8_1 );
	CGxStaticEx* pAlarmNumber9 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_9_1 );
	CGxStaticEx* pAlarmNumber10 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_10_1 );
	CGxStaticEx* pAlarmNumber11 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_11_1 );
	CGxStaticEx* pAlarmNumber12 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_12_1 );
	CGxStaticEx* pAlarmNumber13= (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_13_1 );
	CGxStaticEx* pAlarmNumber14= (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_14_1 );
	CGxStaticEx* pAlarmNumber15= (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_15_1 );
	CGxStaticEx* pAlarmNumber16= (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_16_1 );
	CGxStaticEx* pAlarmNumber17= (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_17_1 );
	CGxStaticEx* pAlarmNumber18= (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_18_1 );
	CGxStaticEx* pAlarmNumber19= (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_19_1 );
	CGxStaticEx* pAlarmNumber20 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_20_1 );
	CGxStaticEx* pAlarmNumber21 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_21_1 );
	CGxStaticEx* pAlarmNumber22 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_22_1 );

	CGxStaticEx* pAlarmName1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_1_2 );
	CGxStaticEx* pAlarmName2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_2_2 );
	CGxStaticEx* pAlarmName3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_3_2 );
	CGxStaticEx* pAlarmName4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_4_2 );
	CGxStaticEx* pAlarmName5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_5_2 );
	CGxStaticEx* pAlarmName6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_6_2 );
	CGxStaticEx* pAlarmName7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_7_2 );
	CGxStaticEx* pAlarmName8 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_8_2 );
	CGxStaticEx* pAlarmName9 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_9_2 );
	CGxStaticEx* pAlarmName10 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_10_2 );
	CGxStaticEx* pAlarmName11 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_11_2 );
	CGxStaticEx* pAlarmName12 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_12_2 );
	CGxStaticEx* pAlarmName13 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_13_2 );
	CGxStaticEx* pAlarmName14 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_14_2 );
	CGxStaticEx* pAlarmName15 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_15_2 );
	CGxStaticEx* pAlarmName16 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_16_2 );
	CGxStaticEx* pAlarmName17 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_17_2 );
	CGxStaticEx* pAlarmName18 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_18_2 );
	CGxStaticEx* pAlarmName19 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_19_2 );
	CGxStaticEx* pAlarmName20 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_20_2 );
	CGxStaticEx* pAlarmName21 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_21_2 );
	CGxStaticEx* pAlarmName22 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_22_2 );

	CGxStaticEx* pAlarmLevel1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_1_3 );
	CGxStaticEx* pAlarmLevel2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_2_3 );
	CGxStaticEx* pAlarmLevel3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_3_3 );
	CGxStaticEx* pAlarmLevel4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_4_3 );
	CGxStaticEx* pAlarmLevel5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_5_3 );
	CGxStaticEx* pAlarmLevel6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_6_3 );
	CGxStaticEx* pAlarmLevel7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_7_3 );
	CGxStaticEx* pAlarmLevel8 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_8_3 );
	CGxStaticEx* pAlarmLevel9 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_9_3 );
	CGxStaticEx* pAlarmLevel10 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_10_3 );
	CGxStaticEx* pAlarmLevel11 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_11_3 );
	CGxStaticEx* pAlarmLevel12 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_12_3 );
	CGxStaticEx* pAlarmLevel13 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_13_3 );
	CGxStaticEx* pAlarmLevel14 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_14_3 );
	CGxStaticEx* pAlarmLevel15 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_15_3 );
	CGxStaticEx* pAlarmLevel16 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_16_3 );
	CGxStaticEx* pAlarmLevel17 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_17_3 );
	CGxStaticEx* pAlarmLevel18 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_18_3 );
	CGxStaticEx* pAlarmLevel19 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_19_3 );
	CGxStaticEx* pAlarmLevel20 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_20_3 );
	CGxStaticEx* pAlarmLevel21 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_21_3 );
	CGxStaticEx* pAlarmLevel22 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_22_3 );

	CGxStaticEx* pAlarmCount1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_1_4 );
	CGxStaticEx* pAlarmCount2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_2_4 );
	CGxStaticEx* pAlarmCount3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_3_4 );
	CGxStaticEx* pAlarmCount4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_4_4 );
	CGxStaticEx* pAlarmCount5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_5_4 );
	CGxStaticEx* pAlarmCount6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_6_4 );
	CGxStaticEx* pAlarmCount7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_7_4 );
	CGxStaticEx* pAlarmCount8 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_8_4 );
	CGxStaticEx* pAlarmCount9 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_9_4 );
	CGxStaticEx* pAlarmCount10 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_10_4 );
	CGxStaticEx* pAlarmCount11 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_11_4 );
	CGxStaticEx* pAlarmCount12 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_12_4 );
	CGxStaticEx* pAlarmCount13 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_13_4 );
	CGxStaticEx* pAlarmCount14 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_14_4 );
	CGxStaticEx* pAlarmCount15 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_15_4 );
	CGxStaticEx* pAlarmCount16 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_16_4 );
	CGxStaticEx* pAlarmCount17 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_17_4 );
	CGxStaticEx* pAlarmCount18 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_18_4 );
	CGxStaticEx* pAlarmCount19 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_19_4 );
	CGxStaticEx* pAlarmCount20 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_20_4 );
	CGxStaticEx* pAlarmCount21 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_21_4 );
	CGxStaticEx* pAlarmCount22 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_22_4 );


	CGxStaticEx* pAlarmSpan1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_1_5 );
	CGxStaticEx* pAlarmSpan2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_2_5 );
	CGxStaticEx* pAlarmSpan3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_3_5 );
	CGxStaticEx* pAlarmSpan4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_4_5 );
	CGxStaticEx* pAlarmSpan5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_5_5 );
	CGxStaticEx* pAlarmSpan6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_6_5 );
	CGxStaticEx* pAlarmSpan7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_7_5 );
	CGxStaticEx* pAlarmSpan8 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_8_5 );
	CGxStaticEx* pAlarmSpan9 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_9_5 );
	CGxStaticEx* pAlarmSpan10 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_10_5 );
	CGxStaticEx* pAlarmSpan11 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_11_5 );
	CGxStaticEx* pAlarmSpan12 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_12_5 );
	CGxStaticEx* pAlarmSpan13 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_13_5 );
	CGxStaticEx* pAlarmSpan14 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_14_5 );
	CGxStaticEx* pAlarmSpan15 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_15_5 );
	CGxStaticEx* pAlarmSpan16 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_16_5 );
	CGxStaticEx* pAlarmSpan17 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_17_5 );
	CGxStaticEx* pAlarmSpan18 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_18_5 );
	CGxStaticEx* pAlarmSpan19 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_19_5 );
	CGxStaticEx* pAlarmSpan20 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_20_5 );
	CGxStaticEx* pAlarmSpan21 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_21_5 );
	CGxStaticEx* pAlarmSpan22 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_22_5 );

	pAlarmTotalCount->SetCaption(_T("0"));
	pAlarmTotalSpan->SetCaption(_T(""));
	pAlarmNumber1->SetCaption(_T(""));
	pAlarmNumber2->SetCaption(_T(""));
	pAlarmNumber3->SetCaption(_T(""));
	pAlarmNumber4->SetCaption(_T(""));
	pAlarmNumber5->SetCaption(_T(""));
	pAlarmNumber6->SetCaption(_T(""));
	pAlarmNumber7->SetCaption(_T(""));
	pAlarmNumber8->SetCaption(_T(""));
	pAlarmNumber9->SetCaption(_T(""));
	pAlarmNumber10->SetCaption(_T(""));
	pAlarmNumber11->SetCaption(_T(""));
	pAlarmNumber12->SetCaption(_T(""));
	pAlarmNumber13->SetCaption(_T(""));
	pAlarmNumber14->SetCaption(_T(""));
	pAlarmNumber15->SetCaption(_T(""));
	pAlarmNumber16->SetCaption(_T(""));
	pAlarmNumber17->SetCaption(_T(""));
	pAlarmNumber18->SetCaption(_T(""));
	pAlarmNumber19->SetCaption(_T(""));
	pAlarmNumber20->SetCaption(_T(""));
	pAlarmNumber21->SetCaption(_T(""));
	pAlarmNumber22->SetCaption(_T(""));

	pAlarmName1->SetCaption(_T(""));
	pAlarmName2->SetCaption(_T(""));
	pAlarmName3->SetCaption(_T(""));
	pAlarmName4->SetCaption(_T(""));
	pAlarmName5->SetCaption(_T(""));
	pAlarmName6->SetCaption(_T(""));
	pAlarmName7->SetCaption(_T(""));
	pAlarmName8->SetCaption(_T(""));
	pAlarmName9->SetCaption(_T(""));
	pAlarmName10->SetCaption(_T(""));
	pAlarmName11->SetCaption(_T(""));
	pAlarmName12->SetCaption(_T(""));
	pAlarmName13->SetCaption(_T(""));
	pAlarmName14->SetCaption(_T(""));
	pAlarmName15->SetCaption(_T(""));
	pAlarmName16->SetCaption(_T(""));
	pAlarmName17->SetCaption(_T(""));
	pAlarmName18->SetCaption(_T(""));
	pAlarmName19->SetCaption(_T(""));
	pAlarmName20->SetCaption(_T(""));
	pAlarmName21->SetCaption(_T(""));
	pAlarmName22->SetCaption(_T(""));

	pAlarmLevel1->SetCaption(_T(""));
	pAlarmLevel2->SetCaption(_T(""));
	pAlarmLevel3->SetCaption(_T(""));
	pAlarmLevel4->SetCaption(_T(""));
	pAlarmLevel5->SetCaption(_T(""));
	pAlarmLevel6->SetCaption(_T(""));
	pAlarmLevel7->SetCaption(_T(""));
	pAlarmLevel8->SetCaption(_T(""));
	pAlarmLevel9->SetCaption(_T(""));
	pAlarmLevel10->SetCaption(_T(""));
	pAlarmLevel11->SetCaption(_T(""));
	pAlarmLevel12->SetCaption(_T(""));
	pAlarmLevel13->SetCaption(_T(""));
	pAlarmLevel14->SetCaption(_T(""));
	pAlarmLevel15->SetCaption(_T(""));
	pAlarmLevel16->SetCaption(_T(""));
	pAlarmLevel17->SetCaption(_T(""));
	pAlarmLevel18->SetCaption(_T(""));
	pAlarmLevel19->SetCaption(_T(""));
	pAlarmLevel20->SetCaption(_T(""));
	pAlarmLevel21->SetCaption(_T(""));
	pAlarmLevel22->SetCaption(_T(""));

	pAlarmCount1->SetCaption(_T(""));
	pAlarmCount2->SetCaption(_T(""));
	pAlarmCount3->SetCaption(_T(""));
	pAlarmCount4->SetCaption(_T(""));
	pAlarmCount5->SetCaption(_T(""));
	pAlarmCount6->SetCaption(_T(""));
	pAlarmCount7->SetCaption(_T(""));
	pAlarmCount8->SetCaption(_T(""));
	pAlarmCount9->SetCaption(_T(""));
	pAlarmCount10->SetCaption(_T(""));
	pAlarmCount11->SetCaption(_T(""));
	pAlarmCount12->SetCaption(_T(""));
	pAlarmCount13->SetCaption(_T(""));
	pAlarmCount14->SetCaption(_T(""));
	pAlarmCount15->SetCaption(_T(""));
	pAlarmCount16->SetCaption(_T(""));
	pAlarmCount17->SetCaption(_T(""));
	pAlarmCount18->SetCaption(_T(""));
	pAlarmCount19->SetCaption(_T(""));
	pAlarmCount20->SetCaption(_T(""));
	pAlarmCount21->SetCaption(_T(""));
	pAlarmCount22->SetCaption(_T(""));

	pAlarmSpan1->SetCaption(_T(""));
	pAlarmSpan2->SetCaption(_T(""));
	pAlarmSpan3->SetCaption(_T(""));
	pAlarmSpan4->SetCaption(_T(""));
	pAlarmSpan5->SetCaption(_T(""));
	pAlarmSpan6->SetCaption(_T(""));
	pAlarmSpan7->SetCaption(_T(""));
	pAlarmSpan8->SetCaption(_T(""));
	pAlarmSpan9->SetCaption(_T(""));
	pAlarmSpan10->SetCaption(_T(""));
	pAlarmSpan11->SetCaption(_T(""));
	pAlarmSpan12->SetCaption(_T(""));
	pAlarmSpan13->SetCaption(_T(""));
	pAlarmSpan14->SetCaption(_T(""));
	pAlarmSpan15->SetCaption(_T(""));
	pAlarmSpan16->SetCaption(_T(""));
	pAlarmSpan17->SetCaption(_T(""));
	pAlarmSpan18->SetCaption(_T(""));
	pAlarmSpan19->SetCaption(_T(""));
	pAlarmSpan20->SetCaption(_T(""));
	pAlarmSpan21->SetCaption(_T(""));
	pAlarmSpan22->SetCaption(_T(""));
}
void CFormReport2::ListUpdate()
{
	CGxStaticEx* pAlarmTotalCount = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_COUNT );
	CGxStaticEx* pAlarmTotalSpan = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_SPAN );
	CGxStaticEx* pAlarmNumber1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_1_1 );
	CGxStaticEx* pAlarmNumber2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_2_1 );
	CGxStaticEx* pAlarmNumber3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_3_1 );
	CGxStaticEx* pAlarmNumber4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_4_1 );
	CGxStaticEx* pAlarmNumber5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_5_1 );
	CGxStaticEx* pAlarmNumber6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_6_1 );
	CGxStaticEx* pAlarmNumber7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_7_1 );
	CGxStaticEx* pAlarmNumber8 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_8_1 );
	CGxStaticEx* pAlarmNumber9 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_9_1 );
	CGxStaticEx* pAlarmNumber10 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_10_1 );
	CGxStaticEx* pAlarmNumber11 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_11_1 );
	CGxStaticEx* pAlarmNumber12 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_12_1 );
	CGxStaticEx* pAlarmNumber13= (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_13_1 );
	CGxStaticEx* pAlarmNumber14= (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_14_1 );
	CGxStaticEx* pAlarmNumber15= (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_15_1 );
	CGxStaticEx* pAlarmNumber16= (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_16_1 );
	CGxStaticEx* pAlarmNumber17= (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_17_1 );
	CGxStaticEx* pAlarmNumber18= (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_18_1 );
	CGxStaticEx* pAlarmNumber19= (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_19_1 );
	CGxStaticEx* pAlarmNumber20 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_20_1 );
	CGxStaticEx* pAlarmNumber21 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_21_1 );
	CGxStaticEx* pAlarmNumber22 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_22_1 );

	CGxStaticEx* pAlarmName1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_1_2 );
	CGxStaticEx* pAlarmName2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_2_2 );
	CGxStaticEx* pAlarmName3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_3_2 );
	CGxStaticEx* pAlarmName4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_4_2 );
	CGxStaticEx* pAlarmName5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_5_2 );
	CGxStaticEx* pAlarmName6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_6_2 );
	CGxStaticEx* pAlarmName7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_7_2 );
	CGxStaticEx* pAlarmName8 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_8_2 );
	CGxStaticEx* pAlarmName9 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_9_2 );
	CGxStaticEx* pAlarmName10 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_10_2 );
	CGxStaticEx* pAlarmName11 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_11_2 );
	CGxStaticEx* pAlarmName12 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_12_2 );
	CGxStaticEx* pAlarmName13 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_13_2 );
	CGxStaticEx* pAlarmName14 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_14_2 );
	CGxStaticEx* pAlarmName15 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_15_2 );
	CGxStaticEx* pAlarmName16 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_16_2 );
	CGxStaticEx* pAlarmName17 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_17_2 );
	CGxStaticEx* pAlarmName18 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_18_2 );
	CGxStaticEx* pAlarmName19 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_19_2 );
	CGxStaticEx* pAlarmName20 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_20_2 );
	CGxStaticEx* pAlarmName21 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_21_2 );
	CGxStaticEx* pAlarmName22 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_22_2 );

	CGxStaticEx* pAlarmLevel1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_1_3 );
	CGxStaticEx* pAlarmLevel2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_2_3 );
	CGxStaticEx* pAlarmLevel3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_3_3 );
	CGxStaticEx* pAlarmLevel4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_4_3 );
	CGxStaticEx* pAlarmLevel5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_5_3 );
	CGxStaticEx* pAlarmLevel6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_6_3 );
	CGxStaticEx* pAlarmLevel7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_7_3 );
	CGxStaticEx* pAlarmLevel8 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_8_3 );
	CGxStaticEx* pAlarmLevel9 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_9_3 );
	CGxStaticEx* pAlarmLevel10 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_10_3 );
	CGxStaticEx* pAlarmLevel11 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_11_3 );
	CGxStaticEx* pAlarmLevel12 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_12_3 );
	CGxStaticEx* pAlarmLevel13 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_13_3 );
	CGxStaticEx* pAlarmLevel14 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_14_3 );
	CGxStaticEx* pAlarmLevel15 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_15_3 );
	CGxStaticEx* pAlarmLevel16 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_16_3 );
	CGxStaticEx* pAlarmLevel17 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_17_3 );
	CGxStaticEx* pAlarmLevel18 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_18_3 );
	CGxStaticEx* pAlarmLevel19 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_19_3 );
	CGxStaticEx* pAlarmLevel20 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_20_3 );
	CGxStaticEx* pAlarmLevel21 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_21_3 );
	CGxStaticEx* pAlarmLevel22 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_22_3 );

	CGxStaticEx* pAlarmCount1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_1_4 );
	CGxStaticEx* pAlarmCount2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_2_4 );
	CGxStaticEx* pAlarmCount3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_3_4 );
	CGxStaticEx* pAlarmCount4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_4_4 );
	CGxStaticEx* pAlarmCount5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_5_4 );
	CGxStaticEx* pAlarmCount6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_6_4 );
	CGxStaticEx* pAlarmCount7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_7_4 );
	CGxStaticEx* pAlarmCount8 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_8_4 );
	CGxStaticEx* pAlarmCount9 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_9_4 );
	CGxStaticEx* pAlarmCount10 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_10_4 );
	CGxStaticEx* pAlarmCount11 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_11_4 );
	CGxStaticEx* pAlarmCount12 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_12_4 );
	CGxStaticEx* pAlarmCount13 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_13_4 );
	CGxStaticEx* pAlarmCount14 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_14_4 );
	CGxStaticEx* pAlarmCount15 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_15_4 );
	CGxStaticEx* pAlarmCount16 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_16_4 );
	CGxStaticEx* pAlarmCount17 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_17_4 );
	CGxStaticEx* pAlarmCount18 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_18_4 );
	CGxStaticEx* pAlarmCount19 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_19_4 );
	CGxStaticEx* pAlarmCount20 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_20_4 );
	CGxStaticEx* pAlarmCount21 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_21_4 );
	CGxStaticEx* pAlarmCount22 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_22_4 );


	CGxStaticEx* pAlarmSpan1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_1_5 );
	CGxStaticEx* pAlarmSpan2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_2_5 );
	CGxStaticEx* pAlarmSpan3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_3_5 );
	CGxStaticEx* pAlarmSpan4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_4_5 );
	CGxStaticEx* pAlarmSpan5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_5_5 );
	CGxStaticEx* pAlarmSpan6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_6_5 );
	CGxStaticEx* pAlarmSpan7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_7_5 );
	CGxStaticEx* pAlarmSpan8 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_8_5 );
	CGxStaticEx* pAlarmSpan9 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_9_5 );
	CGxStaticEx* pAlarmSpan10 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_10_5 );
	CGxStaticEx* pAlarmSpan11 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_11_5 );
	CGxStaticEx* pAlarmSpan12 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_12_5 );
	CGxStaticEx* pAlarmSpan13 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_13_5 );
	CGxStaticEx* pAlarmSpan14 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_14_5 );
	CGxStaticEx* pAlarmSpan15 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_15_5 );
	CGxStaticEx* pAlarmSpan16 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_16_5 );
	CGxStaticEx* pAlarmSpan17 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_17_5 );
	CGxStaticEx* pAlarmSpan18 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_18_5 );
	CGxStaticEx* pAlarmSpan19 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_19_5 );
	CGxStaticEx* pAlarmSpan20 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_20_5 );
	CGxStaticEx* pAlarmSpan21 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_21_5 );
	CGxStaticEx* pAlarmSpan22 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_22_5 );
	ClearData();
	if((int)m_alarm_map.size()==0)
		return;
	std::vector< std::pair< int, int > > vectorData;
	for (auto iter = m_alarm_map.begin(); iter != m_alarm_map.end(); ++iter)
		vectorData.emplace_back(std::make_pair(iter->first, iter->second.nCount));


	//저장된 Map의 value값을 vector값에 대입하여 내림차순으로 정렬한다.
	std::sort(vectorData.begin(), vectorData.end(), compare_pair_second<std::greater>());

	int selnum=(int)vectorData.size();
	int total=0;
	//pAlarmTotalCount->SetCaption(_T("%d"),selnum)
	CString val;

	int nAlarmID;

	if(selnum>=1)
	{
		nAlarmID = vectorData[0].first;

		val.Format(_T("%d"), nAlarmID);
		pAlarmNumber1->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].Alarm_name);
		pAlarmName1->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].nLevel);
		pAlarmLevel1->SetCaption(val);

		val.Format(_T("%d"), m_alarm_map[nAlarmID].nCount);
		pAlarmCount1->SetCaption(val);

		val.Format(_T("%02d:%02d:%02d"), m_alarm_map[nAlarmID].span.GetHours(), 
									m_alarm_map[nAlarmID].span.GetMinutes(),
									m_alarm_map[nAlarmID].span.GetSeconds());
		pAlarmSpan1->SetCaption(val);
		total_span=m_alarm_map[nAlarmID].span;
		total+=m_alarm_map[nAlarmID].nCount;
	}
	if(selnum>=2)
	{
		nAlarmID = vectorData[1].first;

		val.Format(_T("%d"), nAlarmID);
		pAlarmNumber2->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].Alarm_name);
		pAlarmName2->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].nLevel);
		pAlarmLevel2->SetCaption(val);

		val.Format(_T("%d"), m_alarm_map[nAlarmID].nCount);
		pAlarmCount2->SetCaption(val);

		val.Format(_T("%02d:%02d:%02d"), m_alarm_map[nAlarmID].span.GetHours(), 
			m_alarm_map[nAlarmID].span.GetMinutes(),
			m_alarm_map[nAlarmID].span.GetSeconds());
		pAlarmSpan2->SetCaption(val);
		total_span+=m_alarm_map[nAlarmID].span;
		total+=m_alarm_map[nAlarmID].nCount;
	}
	if(selnum>=3)
	{
		nAlarmID = vectorData[2].first;

		val.Format(_T("%d"), nAlarmID);
		pAlarmNumber3->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].Alarm_name);
		pAlarmName3->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].nLevel);
		pAlarmLevel3->SetCaption(val);

		val.Format(_T("%d"), m_alarm_map[nAlarmID].nCount);
		pAlarmCount3->SetCaption(val);

		val.Format(_T("%02d:%02d:%02d"), m_alarm_map[nAlarmID].span.GetHours(), 
			m_alarm_map[nAlarmID].span.GetMinutes(),
			m_alarm_map[nAlarmID].span.GetSeconds());
		pAlarmSpan3->SetCaption(val);
		total_span+=m_alarm_map[nAlarmID].span;
		total+=m_alarm_map[nAlarmID].nCount;
	}
	if(selnum>=4)
	{
		nAlarmID = vectorData[3].first;

		val.Format(_T("%d"), nAlarmID);
		pAlarmNumber4->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].Alarm_name);
		pAlarmName4->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].nLevel);
		pAlarmLevel4->SetCaption(val);

		val.Format(_T("%d"), m_alarm_map[nAlarmID].nCount);
		pAlarmCount4->SetCaption(val);

		val.Format(_T("%02d:%02d:%02d"), m_alarm_map[nAlarmID].span.GetHours(), 
			m_alarm_map[nAlarmID].span.GetMinutes(),
			m_alarm_map[nAlarmID].span.GetSeconds());
		pAlarmSpan4->SetCaption(val);
		total_span+=m_alarm_map[nAlarmID].span;
		total+=m_alarm_map[nAlarmID].nCount;
	}
	if(selnum>=5)
	{
		nAlarmID = vectorData[4].first;

		val.Format(_T("%d"), nAlarmID);
		pAlarmNumber5->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].Alarm_name);
		pAlarmName5->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].nLevel);
		pAlarmLevel5->SetCaption(val);

		val.Format(_T("%d"), m_alarm_map[nAlarmID].nCount);
		pAlarmCount5->SetCaption(val);

		val.Format(_T("%02d:%02d:%02d"), m_alarm_map[nAlarmID].span.GetHours(), 
			m_alarm_map[nAlarmID].span.GetMinutes(),
			m_alarm_map[nAlarmID].span.GetSeconds());
		pAlarmSpan5->SetCaption(val);
		total_span+=m_alarm_map[nAlarmID].span;
		total+=m_alarm_map[nAlarmID].nCount;
	}
	if(selnum>=6)
	{
		nAlarmID = vectorData[5].first;

		val.Format(_T("%d"), nAlarmID);
		pAlarmNumber6->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].Alarm_name);
		pAlarmName6->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].nLevel);
		pAlarmLevel6->SetCaption(val);

		val.Format(_T("%d"), m_alarm_map[nAlarmID].nCount);
		pAlarmCount6->SetCaption(val);

		val.Format(_T("%02d:%02d:%02d"), m_alarm_map[nAlarmID].span.GetHours(), 
			m_alarm_map[nAlarmID].span.GetMinutes(),
			m_alarm_map[nAlarmID].span.GetSeconds());
		pAlarmSpan6->SetCaption(val);
		total_span+=m_alarm_map[nAlarmID].span;
		total+=m_alarm_map[nAlarmID].nCount;
	}
	if(selnum>=7)
	{
		nAlarmID = vectorData[6].first;

		val.Format(_T("%d"), nAlarmID);
		pAlarmNumber7->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].Alarm_name);
		pAlarmName7->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].nLevel);
		pAlarmLevel7->SetCaption(val);

		val.Format(_T("%d"), m_alarm_map[nAlarmID].nCount);
		pAlarmCount7->SetCaption(val);

		val.Format(_T("%02d:%02d:%02d"), m_alarm_map[nAlarmID].span.GetHours(), 
			m_alarm_map[nAlarmID].span.GetMinutes(),
			m_alarm_map[nAlarmID].span.GetSeconds());
		pAlarmSpan7->SetCaption(val);
		total_span+=m_alarm_map[nAlarmID].span;
		total+=m_alarm_map[nAlarmID].nCount;
	}
	if(selnum>=8)
	{
		nAlarmID = vectorData[7].first;

		val.Format(_T("%d"), nAlarmID);
		pAlarmNumber8->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].Alarm_name);
		pAlarmName8->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].nLevel);
		pAlarmLevel8->SetCaption(val);

		val.Format(_T("%d"), m_alarm_map[nAlarmID].nCount);
		pAlarmCount8->SetCaption(val);

		val.Format(_T("%02d:%02d:%02d"), m_alarm_map[nAlarmID].span.GetHours(), 
			m_alarm_map[nAlarmID].span.GetMinutes(),
			m_alarm_map[nAlarmID].span.GetSeconds());
		pAlarmSpan8->SetCaption(val);
		total_span+=m_alarm_map[nAlarmID].span;
		total+=m_alarm_map[nAlarmID].nCount;
	}
	if(selnum>=9)
	{
		nAlarmID = vectorData[8].first;

		val.Format(_T("%d"), nAlarmID);
		pAlarmNumber9->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].Alarm_name);
		pAlarmName9->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].nLevel);
		pAlarmLevel9->SetCaption(val);

		val.Format(_T("%d"), m_alarm_map[nAlarmID].nCount);
		pAlarmCount9->SetCaption(val);

		val.Format(_T("%02d:%02d:%02d"), m_alarm_map[nAlarmID].span.GetHours(), 
			m_alarm_map[nAlarmID].span.GetMinutes(),
			m_alarm_map[nAlarmID].span.GetSeconds());
		pAlarmSpan9->SetCaption(val);
		total_span+=m_alarm_map[nAlarmID].span;
		total+=m_alarm_map[nAlarmID].nCount;
	}
	if(selnum>=10)
	{
		nAlarmID = vectorData[9].first;

		val.Format(_T("%d"), nAlarmID);
		pAlarmNumber10->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].Alarm_name);
		pAlarmName10->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].nLevel);
		pAlarmLevel10->SetCaption(val);

		val.Format(_T("%d"), m_alarm_map[nAlarmID].nCount);
		pAlarmCount10->SetCaption(val);

		val.Format(_T("%02d:%02d:%02d"), m_alarm_map[nAlarmID].span.GetHours(), 
			m_alarm_map[nAlarmID].span.GetMinutes(),
			m_alarm_map[nAlarmID].span.GetSeconds());
		pAlarmSpan10->SetCaption(val);
		total_span+=m_alarm_map[nAlarmID].span;
		total+=m_alarm_map[nAlarmID].nCount;
	}
	if(selnum>=11)
	{
		nAlarmID = vectorData[10].first;

		val.Format(_T("%d"), nAlarmID);
		pAlarmNumber11->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].Alarm_name);
		pAlarmName11->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].nLevel);
		pAlarmLevel11->SetCaption(val);

		val.Format(_T("%d"), m_alarm_map[nAlarmID].nCount);
		pAlarmCount11->SetCaption(val);

		val.Format(_T("%02d:%02d:%02d"), m_alarm_map[nAlarmID].span.GetHours(), 
			m_alarm_map[nAlarmID].span.GetMinutes(),
			m_alarm_map[nAlarmID].span.GetSeconds());
		pAlarmSpan11->SetCaption(val);
		total_span+=m_alarm_map[nAlarmID].span;
		total+=m_alarm_map[nAlarmID].nCount;
	}
	if(selnum>=12)
	{
		nAlarmID = vectorData[11].first;

		val.Format(_T("%d"), nAlarmID);
		pAlarmNumber12->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].Alarm_name);
		pAlarmName12->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].nLevel);
		pAlarmLevel12->SetCaption(val);

		val.Format(_T("%d"), m_alarm_map[nAlarmID].nCount);
		pAlarmCount12->SetCaption(val);

		val.Format(_T("%02d:%02d:%02d"), m_alarm_map[nAlarmID].span.GetHours(), 
			m_alarm_map[nAlarmID].span.GetMinutes(),
			m_alarm_map[nAlarmID].span.GetSeconds());
		pAlarmSpan12->SetCaption(val);
		total_span+=m_alarm_map[nAlarmID].span;
		total+=m_alarm_map[nAlarmID].nCount;
	}
	if(selnum>=13)
	{
		nAlarmID = vectorData[12].first;

		val.Format(_T("%d"), nAlarmID);
		pAlarmNumber13->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].Alarm_name);
		pAlarmName13->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].nLevel);
		pAlarmLevel13->SetCaption(val);

		val.Format(_T("%d"), m_alarm_map[nAlarmID].nCount);
		pAlarmCount13->SetCaption(val);

		val.Format(_T("%02d:%02d:%02d"), m_alarm_map[nAlarmID].span.GetHours(), 
			m_alarm_map[nAlarmID].span.GetMinutes(),
			m_alarm_map[nAlarmID].span.GetSeconds());
		pAlarmSpan13->SetCaption(val);
		total_span+=m_alarm_map[nAlarmID].span;
		total+=m_alarm_map[nAlarmID].nCount;
	}
	if(selnum>=14)
	{
		nAlarmID = vectorData[13].first;

		val.Format(_T("%d"), nAlarmID);
		pAlarmNumber14->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].Alarm_name);
		pAlarmName14->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].nLevel);
		pAlarmLevel14->SetCaption(val);

		val.Format(_T("%d"), m_alarm_map[nAlarmID].nCount);
		pAlarmCount14->SetCaption(val);

		val.Format(_T("%02d:%02d:%02d"), m_alarm_map[nAlarmID].span.GetHours(), 
			m_alarm_map[nAlarmID].span.GetMinutes(),
			m_alarm_map[nAlarmID].span.GetSeconds());
		pAlarmSpan14->SetCaption(val);
		total_span+=m_alarm_map[nAlarmID].span;
		total+=m_alarm_map[nAlarmID].nCount;
	}
	if(selnum>=15)
	{
		nAlarmID = vectorData[14].first;

		val.Format(_T("%d"), nAlarmID);
		pAlarmNumber15->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].Alarm_name);
		pAlarmName15->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].nLevel);
		pAlarmLevel15->SetCaption(val);

		val.Format(_T("%d"), m_alarm_map[nAlarmID].nCount);
		pAlarmCount15->SetCaption(val);

		val.Format(_T("%02d:%02d:%02d"), m_alarm_map[nAlarmID].span.GetHours(), 
			m_alarm_map[nAlarmID].span.GetMinutes(),
			m_alarm_map[nAlarmID].span.GetSeconds());
		pAlarmSpan15->SetCaption(val);
		total_span+=m_alarm_map[nAlarmID].span;
		total+=m_alarm_map[nAlarmID].nCount;
	}
	if(selnum>=16)
	{
		nAlarmID = vectorData[15].first;

		val.Format(_T("%d"), nAlarmID);
		pAlarmNumber16->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].Alarm_name);
		pAlarmName16->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].nLevel);
		pAlarmLevel16->SetCaption(val);

		val.Format(_T("%d"), m_alarm_map[nAlarmID].nCount);
		pAlarmCount16->SetCaption(val);

		val.Format(_T("%02d:%02d:%02d"), m_alarm_map[nAlarmID].span.GetHours(), 
			m_alarm_map[nAlarmID].span.GetMinutes(),
			m_alarm_map[nAlarmID].span.GetSeconds());
		pAlarmSpan16->SetCaption(val);
		total_span+=m_alarm_map[nAlarmID].span;
		total+=m_alarm_map[nAlarmID].nCount;
	}
	if(selnum>=17)
	{
		nAlarmID = vectorData[16].first;

		val.Format(_T("%d"), nAlarmID);
		pAlarmNumber17->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].Alarm_name);
		pAlarmName17->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].nLevel);
		pAlarmLevel17->SetCaption(val);

		val.Format(_T("%d"), m_alarm_map[nAlarmID].nCount);
		pAlarmCount17->SetCaption(val);

		val.Format(_T("%02d:%02d:%02d"), m_alarm_map[nAlarmID].span.GetHours(), 
			m_alarm_map[nAlarmID].span.GetMinutes(),
			m_alarm_map[nAlarmID].span.GetSeconds());
		pAlarmSpan17->SetCaption(val);
		total_span+=m_alarm_map[nAlarmID].span;
		total+=m_alarm_map[nAlarmID].nCount;
	}
	if(selnum>=18)
	{
		nAlarmID = vectorData[17].first;

		val.Format(_T("%d"), nAlarmID);
		pAlarmNumber18->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].Alarm_name);
		pAlarmName18->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].nLevel);
		pAlarmLevel18->SetCaption(val);

		val.Format(_T("%d"), m_alarm_map[nAlarmID].nCount);
		pAlarmCount18->SetCaption(val);

		val.Format(_T("%02d:%02d:%02d"), m_alarm_map[nAlarmID].span.GetHours(), 
			m_alarm_map[nAlarmID].span.GetMinutes(),
			m_alarm_map[nAlarmID].span.GetSeconds());
		pAlarmSpan18->SetCaption(val);
		total_span+=m_alarm_map[nAlarmID].span;
		total+=m_alarm_map[nAlarmID].nCount;
	}
	if(selnum>=19)
	{
		nAlarmID = vectorData[18].first;

		val.Format(_T("%d"), nAlarmID);
		pAlarmNumber19->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].Alarm_name);
		pAlarmName19->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].nLevel);
		pAlarmLevel19->SetCaption(val);

		val.Format(_T("%d"), m_alarm_map[nAlarmID].nCount);
		pAlarmCount19->SetCaption(val);

		val.Format(_T("%02d:%02d:%02d"), m_alarm_map[nAlarmID].span.GetHours(), 
			m_alarm_map[nAlarmID].span.GetMinutes(),
			m_alarm_map[nAlarmID].span.GetSeconds());
		pAlarmSpan19->SetCaption(val);
		total_span+=m_alarm_map[nAlarmID].span;
		total+=m_alarm_map[nAlarmID].nCount;
	}
	if(selnum>=20)
	{
		nAlarmID = vectorData[19].first;

		val.Format(_T("%d"), nAlarmID);
		pAlarmNumber20->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].Alarm_name);
		pAlarmName20->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].nLevel);
		pAlarmLevel20->SetCaption(val);

		val.Format(_T("%d"), m_alarm_map[nAlarmID].nCount);
		pAlarmCount20->SetCaption(val);

		val.Format(_T("%02d:%02d:%02d"), m_alarm_map[nAlarmID].span.GetHours(), 
			m_alarm_map[nAlarmID].span.GetMinutes(),
			m_alarm_map[nAlarmID].span.GetSeconds());
		pAlarmSpan20->SetCaption(val);
		total_span+=m_alarm_map[nAlarmID].span;
		total+=m_alarm_map[nAlarmID].nCount;
	}
	if(selnum>=21)
	{
		nAlarmID = vectorData[20].first;

		val.Format(_T("%d"), nAlarmID);
		pAlarmNumber21->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].Alarm_name);
		pAlarmName21->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].nLevel);
		pAlarmLevel21->SetCaption(val);

		val.Format(_T("%d"), m_alarm_map[nAlarmID].nCount);
		pAlarmCount21->SetCaption(val);

		val.Format(_T("%02d:%02d:%02d"), m_alarm_map[nAlarmID].span.GetHours(), 
			m_alarm_map[nAlarmID].span.GetMinutes(),
			m_alarm_map[nAlarmID].span.GetSeconds());
		pAlarmSpan21->SetCaption(val);
		total_span+=m_alarm_map[nAlarmID].span;
		total+=m_alarm_map[nAlarmID].nCount;
	}
	if(selnum>=22)
	{
		nAlarmID = vectorData[21].first;

		val.Format(_T("%d"), nAlarmID);
		pAlarmNumber22->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].Alarm_name);
		pAlarmName22->SetCaption(val);

		val.Format(_T("%s"), alarm_input[nAlarmID].nLevel);
		pAlarmLevel22->SetCaption(val);

		val.Format(_T("%d"), m_alarm_map[nAlarmID].nCount);
		pAlarmCount22->SetCaption(val);

		val.Format(_T("%02d:%02d:%02d"), m_alarm_map[nAlarmID].span.GetHours(), 
			m_alarm_map[nAlarmID].span.GetMinutes(),
			m_alarm_map[nAlarmID].span.GetSeconds());
		pAlarmSpan22->SetCaption(val);
		total_span+=m_alarm_map[nAlarmID].span;
		total+=m_alarm_map[nAlarmID].nCount;
	}
	val.Format(_T("%d"),total);
	pAlarmTotalCount->SetCaption(val);
	val.Format(_T("%02d:%02d:%02d"), total_span.GetHours(),total_span.GetMinutes(),total_span.GetSeconds());
	pAlarmTotalSpan->SetCaption(val);
}

void CFormReport2::ClickGxlblAlarmDay()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRESENT"),_T("DAY"));
	m_eIDX_Shift=eIDX_Shift_DAY;
	AlarmUdate(m_eIDX_Shift);
}


void CFormReport2::ClickGxlblAlarmSw()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRESENT"),_T("SW"));
	m_eIDX_Shift=eIDX_Shift_SW;
	AlarmUdate(m_eIDX_Shift);
}


void CFormReport2::ClickGxlblAlarmGy()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRESENT"),_T("GY"));
	m_eIDX_Shift=eIDX_Shift_GY;	
	AlarmUdate(m_eIDX_Shift);
}



void CFormReport2::ClickGxlblAlarmPrev()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRESENT"),_T("PREV"));
	CGxStaticEx* pAlarmDate = (CGxStaticEx*)GetDlgItem( IDC_GXLBL_ALARM_DATE );
	ClearData();
	//sel_date++;
	//time= CTime::GetCurrentTime();
	CTimeSpan span(1,0,0,0);
	time=time-span;
	m_sel_time.Format(_T("%04u-%02u-%02u"),time.GetYear(), time.GetMonth(), time.GetDay());

	AlarmUdate(m_eIDX_Shift);
}


void CFormReport2::ClickGxlblAlarmNext()
{
	CGxStaticEx* pAlarmDate = (CGxStaticEx*)GetDlgItem( IDC_GXLBL_ALARM_DATE );
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
	AlarmUdate(m_eIDX_Shift);
}


void CFormReport2::ClickGxlblAlarmToday()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRESENT"),_T("TODAY"));
	ClearData();
	time= CTime::GetCurrentTime();
	CEtc::ApplyTimeOffset(time, theConfigBank.m_Option.m_OffsetHour, 0);
	m_sel_time.Format(_T("%04u-%02u-%02u"),time.GetYear(), time.GetMonth(), time.GetDay());
	AlarmUdate(m_eIDX_Shift);
}



void CFormReport2::ClickGxlblAlarmTotal()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRESENT"),_T("TOTAL"));
	m_eIDX_Shift=eIDX_Shift_Today;
	AlarmUdate(m_eIDX_Shift);
}

#if 0
void CFormReport2::ClickGxbtnHideCheck()
{
	if(m_bHideLevelCHECK)
		SetHideLevelCHECK(FALSE);
	else
		SetHideLevelCHECK(TRUE);

	AlarmUdate(m_eIDX_Shift);
}
#endif