// FormReport5_Opcall_History.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "resource.h"
#include "FormReport4_Opcall_History.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CFormReport4_Opcall_History

IMPLEMENT_DYNCREATE(CFormReport4_Opcall_History, CFormView)

CFormReport4_Opcall_History::CFormReport4_Opcall_History()
	: CFormView(CFormReport4_Opcall_History::IDD)
{
	IsOpenFile=FALSE;
	logpath="";
	m_iUpDownParam=0;

	// 현재 시간 Setting - LSH171208
	GetLocalTime(&m_CurDate);
	CTime cTime(m_CurDate);
	CEtc::ApplyTimeOffset(cTime, theConfigBank.m_Option.m_OffsetHour, 0);
	m_CurDate.wYear = (WORD)cTime.GetYear();
	m_CurDate.wMonth = (WORD)cTime.GetMonth();
	m_CurDate.wDay = (WORD)cTime.GetDay();
	m_szCurrent_time.Format(_T("%04u-%02u-%02u"),m_CurDate.wYear, m_CurDate.wMonth, m_CurDate.wDay);
}

CFormReport4_Opcall_History::~CFormReport4_Opcall_History()
{
}

void CFormReport4_Opcall_History::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_GRID_SETUP4_OPCALL_HISTORY_LIST, m_OpCallAlarmGrid);
}

BEGIN_MESSAGE_MAP(CFormReport4_Opcall_History, CFormView)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

END_MESSAGE_MAP()


// CFormReport4_Opcall_History 진단입니다.

#ifdef _DEBUG
void CFormReport4_Opcall_History::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormReport4_Opcall_History::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormReport4_Opcall_History 메시지 처리기입니다.

void CFormReport4_Opcall_History::AlarmGrid()
{
	if(m_OpCallAlarmGrid.GetSafeHwnd() == NULL)return;

	m_OpCallAlarmGrid.SetEditable(FALSE);
	m_OpCallAlarmGrid.SetRowCount(20);
	m_OpCallAlarmGrid.SetColumnCount(5);	
	m_OpCallAlarmGrid.SetDefCellHeight(37);
	m_OpCallAlarmGrid.SetFixedColumnCount(1); 
	m_OpCallAlarmGrid.SetFixedRowCount(1);
	m_OpCallAlarmGrid.SetRowResize(FALSE);
	m_OpCallAlarmGrid.SetColumnResize(FALSE);
	m_OpCallAlarmGrid.SetListMode(FALSE);
	m_OpCallAlarmGrid.Refresh();

	m_OpCallAlarmGrid.SetColumnWidth(0,40);
	m_OpCallAlarmGrid.SetColumnWidth(1,200);
	m_OpCallAlarmGrid.SetColumnWidth(2,150);
	m_OpCallAlarmGrid.SetColumnWidth(3,150);
	m_OpCallAlarmGrid.SetColumnWidth(4,1100);

	m_OpCallAlarmGrid.SetItemText(0,0,_T("No"));
	m_OpCallAlarmGrid.SetItemText(0,1,_T("발생시간"));
	m_OpCallAlarmGrid.SetItemText(0,2,_T("UNIT"));
	m_OpCallAlarmGrid.SetItemText(0,3,_T("ID"));//Y_M_FTI_17.08.14.01	[KBH]	상위 메세지 리스트표기
	m_OpCallAlarmGrid.SetItemText(0,4,_T("MESSAGE"));//Y_M_FTI_17.08.14.01	[KBH]	상위 메세지 리스트표기


	CRect rect;
	m_OpCallAlarmGrid.GetClientRect(rect);
	int iRect = rect.Width();

	DWORD dwTextStyle = DT_RIGHT|DT_CENTER|DT_SINGLELINE|DT_VCENTER;
	LOGFONT lf;
	CFont* pGridFont =m_OpCallAlarmGrid.GetFont();
	pGridFont->GetLogFont(&lf);

	memcpy(lf.lfFaceName, _T("굴림체"), sizeof(_T("굴림체")));
	lf.lfHeight = 13;
	lf.lfWeight  = FW_BOLD;

	for(int iRow = 0; iRow<m_OpCallAlarmGrid.GetRowCount(); iRow++)
	{
		for(int iCol = 0; iCol<m_OpCallAlarmGrid.GetColumnCount(); iCol++)
		{
			m_OpCallAlarmGrid.SetItemFont(iRow, iCol, &lf);
			m_OpCallAlarmGrid.SetItemFormat(iRow, iCol, dwTextStyle);
		}
	}

}
// CFormReport3 메시지 처리기입니다.


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
LRESULT CFormReport4_Opcall_History::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		IsOpenFile=FALSE;
		AlarmGrid();
		UpdateDisplay();
	}
	else
	{

	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CFormReport4_Opcall_History::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	// CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_BOTTOM_MENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormReport4_Opcall_History::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}

void CFormReport4_Opcall_History::LoadArarm()
{
	std::vector<CString> m_vecAlarmNameTemp;
	std::vector<CString> m_vecAlarmMessageTemp;
	std::vector<CString> m_vecAlarm_StartTimeTemp;
	std::vector<CString> m_vecAlarm_UnitTemp;
	
	//알람 검색전 현재 시간 확인
	//kjpark 20170929 OPCall, Interlock. terminel 날짜 변경시 파일 검색 안되는 버그 수정
	CTime		TimeOccur;
	TimeOccur = CTime(m_CurDate);

	CString szOccTime, current_time;
	szOccTime.Format(_T("%04u-%02u-%02u %02d:%02d:%02d"),TimeOccur.GetYear(), TimeOccur.GetMonth(), TimeOccur.GetDay(), TimeOccur.GetHour(), TimeOccur.GetMinute(), TimeOccur.GetSecond() );

	CString szFileDate, szPath, szLine, szTitle;
	CTime timeCur = TimeOccur;	

	if ( timeCur.GetHour() < 7 )
		timeCur += CTimeSpan(-1,0,0,0);

	current_time.Format(_T("%04u-%02u-%02u"),(WORD)timeCur.GetYear(), (WORD)timeCur.GetMonth(), (WORD)timeCur.GetDay());
	
	m_vecAlarmName.clear();
	m_vecAlarmMessage.clear();
	m_vecAlarm_StartTime.clear();
	m_vecAlarm_Unit.clear();
	m_vecAlarm_StartTimeTemp.clear();
	m_vecAlarmNameTemp.clear();
	m_vecAlarmMessageTemp.clear();
	m_vecAlarm_UnitTemp.clear();
	CString path;
	CString filename;
	CFileFind finder;
	CFileFind errorfinder;
	std::vector<CString> m_vecDir;
	std::vector<CString> m_vecDirTemp;
	path.Format(_T("%s\\*"), theConfigBank.m_System.m_strDebugLogPath);

	BOOL bWorking = finder.FindFile(path);//해당 폴더에 전체 디렉토리를 검색
	if(!IsOpenFile)//파일 열기버튼을 클릭한게 아니라면 오늘 날짜를 기준으로 로그파일을 열게 한다.
	{
		while (bWorking)
		{   
			bWorking = finder.FindNextFile();
			if(finder.IsDirectory() && !finder.IsDots())
			{
				CString dir= finder.GetFileName();
				
				if(dir!=current_time )//오늘 날짜만 저장함
				{
					continue;
				}
				logpath.Format(_T("%s\\%s\\OpCallAlarmLog.csv"), theConfigBank.m_System.m_strDebugLogPath, dir);
				BOOL errorexist=errorfinder.FindFile(logpath);
				if(errorexist)
				{
					m_vecDirTemp.push_back(dir);	//로그가 저장된 폴더안에서 error.txt파일이 있는 경로를 저장함.
				}
				path.Delete(path.GetLength()-1);
			}
		}
	}
	else
	{
		m_vecDirTemp.push_back(_T("file_open"));
	}
	int m_vecLength=(int)m_vecDirTemp.size();

	//가장 최근 시간 값이 맨 위로 오도록 재정렬해서 넣어준다.
	for(int i=0; i<m_vecLength;i++)
	{
		m_vecDir.push_back(m_vecDirTemp[m_vecLength-(i+1)]);
	}
	if ((int)m_vecDir.size()==0) return;
	
	for(int i=0; i<(int)m_vecDir.size(); i++)
	{
		CString filename;
		FILE *fStream;
		
		filename.Format(IsOpenFile?_T("%s"),logpath:_T("%s\\%s\\OpCallAlarmLog.csv"), theConfigBank.m_System.m_strDebugLogPath, m_vecDir[i]);
		errno_t e = _tfopen_s(&fStream, filename.GetString(), _T("r,ccs=UTF-8"));

		if (e != 0) return;
		CStdioFile		file(fStream);
		CString strBuf, strItem;
		CTokenizer t;
		while(file.ReadString(strBuf))
		{
			t = CTokenizer(strBuf);
			strItem = t.GetNextToken(_T("\\"));
			if(strItem!=_T("START TIME"))
				m_vecAlarm_StartTimeTemp.push_back(strItem);
			strItem = t.GetNextToken(_T("\\"));			
			if(strItem!=_T("ALARM NAME"))
				m_vecAlarmNameTemp.push_back(strItem);
			strItem = t.GetNextToken(_T("\\"));
			if(strItem!=_T("ALARM MESSAGE"))
				m_vecAlarmMessageTemp.push_back(strItem);
			strItem = t.GetNextToken(_T("\\"));
			if(strItem!=_T("UNIT"))
				m_vecAlarm_UnitTemp.push_back(strItem);
		}	
		file.Close();
	

	}
	m_vecLength= m_vecAlarm_StartTimeTemp.size();
	for(int j=0; j<m_vecLength;j++)
	{
		m_vecAlarm_StartTime.push_back(m_vecAlarm_StartTimeTemp[m_vecLength-(j+1)]);
		m_vecAlarmName.push_back(m_vecAlarmNameTemp[m_vecLength-(j+1)]);
		m_vecAlarmMessage.push_back(m_vecAlarmMessageTemp[m_vecLength-(j+1)]);	
		m_vecAlarm_Unit.push_back(m_vecAlarm_UnitTemp[m_vecLength-(j+1)]);	
	}
	
	
}

void CFormReport4_Opcall_History::UpdateArarmGrid()
{
	CString sNum;

	LOGFONT lf;
	CFont* pGridFont =m_OpCallAlarmGrid.GetFont();
	pGridFont->GetLogFont(&lf);
	sNum.Format(_T("%d"),m_vecAlarm_StartTime.size());
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_NUM,sNum);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_FILENAME,logpath);
	memcpy(lf.lfFaceName, _T("굴림체"), sizeof(_T("굴림체")));
	lf.lfHeight = 14;
	lf.lfWeight  = FW_BOLD;
	m_OpCallAlarmGrid.SetRowCount(19);
	DWORD dwTextStyle = DT_LEFT|DT_SINGLELINE|DT_VCENTER;;
	
	for(int i = 0; i < m_OpCallAlarmGrid.GetRowCount(); i++ )
	{
		if ( i == 0 )	continue;
		else			m_OpCallAlarmGrid.SetRowHeight(i, 38);

		sNum.Format(_T("%d"),i+m_iUpDownParam);
		for(int j = 0; j < m_OpCallAlarmGrid.GetColumnCount(); j++ )
		{
			m_OpCallAlarmGrid.SetItemFont(i, j, &lf);
			m_OpCallAlarmGrid.SetItemFormat(i, j, dwTextStyle);

			if ( j == 0 )	m_OpCallAlarmGrid.SetItemText(i, j, sNum);
			else			
			{
				CString szText;
				if ( m_vecAlarm_StartTime.size() > UINT(i-1 + m_iUpDownParam) )
				{
					m_OpCallAlarmGrid.SetItemText(i, 1, m_vecAlarm_StartTime[i - 1 + m_iUpDownParam] );
					m_OpCallAlarmGrid.SetItemText(i, 2, m_vecAlarm_Unit[i - 1 + m_iUpDownParam] );//Y_M_FTI_17.08.14.01	[KBH]	상위 메세지 리스트표기
					m_OpCallAlarmGrid.SetItemText(i, 3, m_vecAlarmName[i - 1 + m_iUpDownParam] );//Y_M_FTI_17.08.14.01	[KBH]	상위 메세지 리스트표기
					m_OpCallAlarmGrid.SetItemText(i, 4, m_vecAlarmMessage[i - 1 + m_iUpDownParam] );//Y_M_FTI_17.08.14.01	[KBH]	상위 메세지 리스트표기
					m_OpCallAlarmGrid.SetFocusCell(m_OpCallAlarmGrid.GetFocusCell().row+m_iUpDownParam, m_OpCallAlarmGrid.GetFocusCell().col);
				}
				else
					m_OpCallAlarmGrid.SetItemText(i,j, _T(""));
			}
			
		}
	}
	// 한 화면이 넘어가서 횡스크롤은 살린다 [9/27/2017 OSC]
 	m_OpCallAlarmGrid.ShowScrollBar(SB_HORZ,TRUE);		//	횡 스크롤 제거
	//m_OpCallAlarmGrid.ShowScrollBar(SB_VERT,FALSE);		//	종 스크롤 제거
	m_OpCallAlarmGrid.Refresh();
}
BEGIN_EVENTSINK_MAP(CFormReport4_Opcall_History, CFormView)
	ON_EVENT(CFormReport4_Opcall_History, IDC_GXBTN_HALFDOWN, DISPID_CLICK, CFormReport4_Opcall_History::ClickGxbtnHalfdown, VTS_NONE)
	ON_EVENT(CFormReport4_Opcall_History, IDC_GXBTN_DOWN, DISPID_CLICK, CFormReport4_Opcall_History::ClickGxbtnDown, VTS_NONE)
	ON_EVENT(CFormReport4_Opcall_History, IDC_GXBTN_HALFUP, DISPID_CLICK, CFormReport4_Opcall_History::ClickGxbtnHalfup, VTS_NONE)
	ON_EVENT(CFormReport4_Opcall_History, IDC_GXBTN_UP, DISPID_CLICK, CFormReport4_Opcall_History::ClickGxbtnUp, VTS_NONE)
	ON_EVENT(CFormReport4_Opcall_History, IDC_GXBTN__LOADFILE, DISPID_CLICK, CFormReport4_Opcall_History::ClickGxbtn, VTS_NONE)
	ON_EVENT(CFormReport4_Opcall_History, IDC_GXLBL_REPORT_PREV, DISPID_CLICK, CFormReport4_Opcall_History::ClickGxlblReportPrev, VTS_NONE)
	ON_EVENT(CFormReport4_Opcall_History, IDC_GXLBL_REPORT_NEXT, DISPID_CLICK, CFormReport4_Opcall_History::ClickGxlblReportNext, VTS_NONE)
	ON_EVENT(CFormReport4_Opcall_History, IDC_GXLBL_REPORT_TODAY, DISPID_CLICK, CFormReport4_Opcall_History::ClickGxlblReportToday, VTS_NONE)
END_EVENTSINK_MAP()


void CFormReport4_Opcall_History::SetDateTime(int iValue, BOOL btoday)		//	PREV나 NEXT 클릭시 날짜를 바꿔줌
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


void CFormReport4_Opcall_History::UpdateDisplay()
{
	// 현재 날짜 업데이트 - LSH171208
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_REPORT_DATE, m_szCurrent_time);

	// OP Call list를 가져온다 - LSH171208
	LoadArarm();

	// 현재 날짜 에 해당하는 정보를 Grid에 입력 - LSH171208
	UpdateArarmGrid();
}


//////////////////////// Date ////////////////////////
void CFormReport4_Opcall_History::ClickGxlblReportPrev()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("PREV"));
	SetDateTime(-1);

	// 화면 갱신 - LSH171208
	UpdateDisplay();
}

void CFormReport4_Opcall_History::ClickGxlblReportNext()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("NEXT"));
	SetDateTime(+1);

	// 화면 갱신 - LSH171208
	UpdateDisplay();
}

void CFormReport4_Opcall_History::ClickGxlblReportToday()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("PRODUCTION"),_T("TODAY"));
	SetDateTime(0, TRUE);

	// 화면 갱신 - LSH171208
	UpdateDisplay();
}


//////////////////////// Up Down ////////////////////////
void CFormReport4_Opcall_History::ClickGxbtnHalfdown()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("ALARMLIST"),_T("HALFDOWN"));
	if ( m_iUpDownParam + 3  >= (int)m_vecAlarm_StartTime.size() )		
		return;

	m_iUpDownParam += 3;
	UpdateArarmGrid();
}

void CFormReport4_Opcall_History::ClickGxbtnDown()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("ALARMLIST"),_T("DOWN"));
	if ( m_iUpDownParam + 7 >=  (int)m_vecAlarm_StartTime.size()  )		
		return;

	m_iUpDownParam += 7;
	UpdateArarmGrid();
}

void CFormReport4_Opcall_History::ClickGxbtnHalfup()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("ALARMLIST"),_T("HALFUP"));
	if ( m_iUpDownParam - 3  < 0 )		
		m_iUpDownParam = 0;
	else
		m_iUpDownParam -= 3;

	UpdateArarmGrid();
}

void CFormReport4_Opcall_History::ClickGxbtnUp()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("ALARMLIST"),_T("UP"));
	if ( m_iUpDownParam - 7  < 0 )		
		m_iUpDownParam = 0;
	else
		m_iUpDownParam -= 7;

	UpdateArarmGrid();
}


void CFormReport4_Opcall_History::ClickGxbtn()
{
	char szFile[256];
  //API file dialog
  OPENFILENAMEA ofn;
  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = NULL;
  ofn.lpstrFile = szFile;
  ofn.lpstrFile[0] = '\0';
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFilter = "OpCallAlarmLog.csv\0OpCallAlarmLog.csv\0";
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = (LPCSTR)theConfigBank.m_System.m_strDebugLogPath.GetString();
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
   BOOL ret = GetOpenFileNameA(&ofn);
  if (ret) 
  {
 	  theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("ALARMLIST"),_T("LOAD OpCallAlarmLog FILE"));
   // load map and enter training mode
	  IsOpenFile=TRUE;
	  logpath=ofn.lpstrFile;
	  IsOpenFile;
	  m_iUpDownParam=0;
	  LoadArarm();
	  UpdateArarmGrid();
 } 
}
