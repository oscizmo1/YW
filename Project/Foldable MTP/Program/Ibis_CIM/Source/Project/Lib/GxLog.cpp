//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 사용자가 지정한 내용을 화면과 파일로 남긴다.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "GxLog.h"

#include <Shlobj.h>											// for SHCreateDirectoryEx()
#include <afxcmn.h>											// for CListCtrl


CGxLog::CGxLog()
{
	m_bDebugLog				= FALSE;							// 디버깅용 TRACE Log를 남길 것인가 ?

	m_pCaller				= NULL;							// 화면에 표시할 대상 (View)
	m_nListID				= 0;								// 화면 표시용 
	m_dwUIThread			= NULL;							// UI에서 호출하는 Main Thread ID

	m_pwszHeaderUnicode	= NULL;							// Unicode 기록용 Header 문자열
	m_pszHeaderMBCS		= NULL;							// MBCS 파일 기록용 Header 문자열


	// 기본적인 기능 속성
	m_nMaxLineCount		= 100;							// Listcontrol에 표시할 라인의 최대 줄수 제한
	m_nKeepDay				= 1;								// Log를 보관하는 일수


	// 폴더/파일이름 생성 규칙
	m_nDirNameMode			= eNAME_YearMonth;			// 자동으로 이름을 생성시 경로명 지정 모드 eNAME_None ~ 
	m_nDirDatePos			= ePOS_DateFirst;				// 자동으로 경로명을 생성시 날짜와 Group 이름을 어떻게 배치할 것인가 ? ePOS_DateNone ~
	m_nFileDatePos			= ePOS_DateLast;				// 자동으로 파일이름을 생성시 지정 파일 이름과 생성 날자를 어떻게 배치할 것인가 ? ePOS_DateNone ~

	m_nTodayHour			= 0;								// 하루의 기준을 정하는 시간 ( 0 ~ 23 )
	m_nTodayMin				= 0;								// 하루의 기준을 정하는 분 ( 0 ~ 59 )


	m_bIsFolderCreate		= TRUE;							// 폴더가 존재하지 않는다면 TRUE:강제로 만들어준다, FALSE:수행 포기
	m_bIsCustomToday		= FALSE;							// 하루를 정의하는 기준을 사용자가 임의로 지정한다. TRUE:SetToday() 설정값에의해 동작, FALSE:0시 기준으로 하루를 나눈다.
	m_bIsAutoDelete		= FALSE;							// 자동으로 과거 log 파일을 삭제할 것인가? TRUE:자동삭제, FALSE:삭제하지 않음
	m_bIsHourly				= FALSE;							// 파일을 매시간단위로 생성할것인가 ? TRUE:시간단위로 파일 생성, FALSE:일(日) 단위로 파일 생성
	m_bIsThreadLock		= FALSE;							// 파일 저장시 Multi-Thread 보호를 수해할 것인가 ? TRUE:보호 수행, FALSE:미수행(GUI 홤경에서는 반드시 FALSE로 지정)

	m_bIsUnicode			= TRUE;							// Unicode로 파일을 저장할 것인가 ? TRUE:Unicode로 파일저장, FALSE:MBCS(ANSI)로 파일 저장
	m_bIsDatePrint			= FALSE;							// 날짜를 자동으로 삽입하여 출력할 것인가 ? TRUE:각 줄마다 년-월-일 표기, FALSE:표기 안함
	m_bIsTimePrint			= TRUE;							// 시각을 자동으로 삽입하여 출력할 것인가 ? TRUE:각 줄마다 시:분:초:밀리초 표기, FALSE:표기안함
	m_bIsHeaderPrint		= FALSE;							// 파일의 시작마다 각 컬럼의 헤더를 출력할 것인가 ? TRUE:헤더 출력, FALSE:출력 안함.

	// 기록 옵션, 설정 옵션 및 기본 조건이 모두 성립되어야 지정한 기능을 수행한다.
	m_bIsEnabled			= TRUE;							// Log 기능을 사용할 것인가 ?
	m_bIsFileWrite			= TRUE;							// 파일에 기록을 할 것인가 ? TRUE:파일로 기록, FALSE:기록 안함
	m_bIsListWrite			= TRUE;							// 화면에 기록을 할 것인가 ? TRUE:화면 ListCtrl에 기록 함, FALSE:기록 안함.


	// 문자열 데이터
	m_sExtention			= _T("txt");					// 파일 확장자, 예:txt , csv, log ....
	m_sBasePath				= _T("");						// 로그가 기록되는 기본 폴더, 지정 폴더의 하위에 기록이 된다. 예:D:\Work\AMT\Log
	m_sGroupName			= _T("");						// 로그의 그룹(중간 구분자) 이름 지정, 예:D:\AUTO_TEST\LOG\05\CELL_DATA\20150512_CELL_Y01.csv 에서 'CELL_DATA'
	m_sFilename				= _T("");						// 로그를 기록 할 기본 파일이름, 본 파일 이름뒤에 날짜가 붙는다. 예:TCP
	m_sLastFilename		= _T("");						// 마지막에 적용된 파일이름
	m_sHeader				= _T("");						// 파일이 새로 생설될 때 출력하는 헤더 내용

}


CGxLog::~CGxLog()
{
	//d TRACE(_T("! CGxLog::~CGxLog() call\n") );

	// 동적 할당한 해더 문자열이 존재한다면 메모리 해제해준다.
	if ( m_pwszHeaderUnicode )	delete[] m_pwszHeaderUnicode;				// Unicode 기록용 Header 문자열
	if ( m_pszHeaderMBCS )		delete[] m_pszHeaderMBCS;					// MBCS 파일 기록용 Header 문자열
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////


// 기록 여부를 지정한다.
void CGxLog::SetEnabled(BOOL bFlag)
{
	m_bIsEnabled = bFlag;										// 기록 여부 지정
}


// 기록 여부를 조회한다.
BOOL CGxLog::GetEnabled()
{
	return m_bIsEnabled;
}



// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ 
// List control에 표시하기위한 각 핸들을 지정해준다. 자동으로 화면에 기록하는 옵션이 활성화된다.
// 
// [in] CWnd* pViewHandle	: List control이 존재하는 폼의 CWnd 포인터, this->m_hWnd로 호출한다.
// [in] int nListID		: List Control의 Resourec ID
//
// 각 설정값들이 합당하다면 지정 설정값에 해당되는 기능 옵션은 활성화 된다.
// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
void CGxLog::SetListHandle(CWnd* pViewHandle, int nListID)			
{
	m_pCaller		= pViewHandle;
	m_nListID		= nListID;
	
	// 각 값들에 의해 각종 옵션을 자동으로 초기 설정해 준다.
	// m_bFlagListLog	= ((pViewHandle == NULL) || (nListID < 0) ) ? FALSE : TRUE;

	if ( !pViewHandle || (nListID <= 0) ) return;						// 인수가 제대로 설정되어있나 ?
	if ( !m_dwUIThread ) m_dwUIThread = AfxGetApp()->m_nThreadID;	// UI에서 호출하는 Main Thread ID


	// 예외상항 감지. - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	try 
	{	
		//? CListCtrl *pListLog = (CListCtrl *)(m_pCaller->GetDlgItem(m_nListID));	// Control의 포인터를 얻어낸다.
		CListCtrl *pListLog = (CListCtrl *)(m_pCaller->GetDlgItem(m_nListID));	// Control의 포인터를 얻어낸다.

		// Log display용 ListCtrl 조정
		if ( pListLog ) 
		{
			CRect rect;

			pListLog->GetClientRect(rect);
			pListLog->InsertColumn(0, _T("Description") , LVCFMT_LEFT, rect.right - 18);
		}
		else		// 지정 컨트롤을 찾을 수 없다. 잘못된 지정이다.
		{
			m_pCaller	= NULL;								// List 출력은 포기한다 ㅡ.ㅡ
			m_nListID	= -1;
		}

	} catch (...) {											// 지정한 인수들이 잘못된 값들이다.
		m_pCaller	= NULL;									// List 출력은 포기한다 ㅡ.ㅡ
		m_nListID	= 0;
	}	// end of try catch - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

}// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _


// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ 
// 파일로  기록을 남길 경로명과 파일이름을 지정해 준다.
//
// [in] CString strPath		: Log를 남길 경로
// [in] CString strFileName	: Log를 남길 File 이름
// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
void CGxLog::SetFilePath(CString sPath, CString sGroup, CString sFile)			
{
	m_sBasePath		= sPath;								// 로그가 기록되는 기본 폴더, 지정 폴더의 하위에 기록이 된다. 예:D:\Work\AMT\Log
	m_sGroupName	= sGroup;								// 로그의 그룹(중간 구분자) 이름 지정
	m_sFilename		= sFile;								// 로그를 기록 할 기본 파일이름, 본 파일 이름뒤에 날짜가 붙는다. 예:TCP

	m_dwUIThread = AfxGetApp()->m_nThreadID;				// UI에서 호출하는 Main Thread ID

	// 가장 뒤에 경로 구분자를 넣어준다.
	if ( m_sBasePath.Right(1) != _T("\\") ) m_sBasePath = m_sBasePath + _T("\\");
}// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _



// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ 
// Header 문자열을 지정한다.
// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
void CGxLog::SetHeader(CString sData)
{
	m_sHeader = sData;
	
	// 해더를 삭제하였다.
	if ( m_sHeader.IsEmpty() )
	{
		// 만약 할당된 해더 배열이 있다면 해제해 준다.

#if defined(UNICODE) || defined(_UNICODE)			// Unicode 개발 환경에서 MBCS로 저장하는 경우
		if ( m_pszHeaderMBCS )
		{
			delete m_pszHeaderMBCS;
			m_pszHeaderMBCS = NULL;
		}
#else
		if ( m_pwszHeaderUnicode )
		{
			delete[] m_pwszHeaderUnicode;
			m_pwszHeaderUnicode = NULL;
		}
#endif

		return;
	}


	int nLen;

#if defined(UNICODE) || defined(_UNICODE)			// Unicode 개발 환경에서 MBCS로 저장하는 경우

	// Unicode -> MBCS는 변환을 해준다. CP_UTF8, CP_ACP
	nLen = WideCharToMultiByte(CP_ACP, 0, m_sHeader.GetBuffer(), -1, NULL, 0, NULL, NULL);		// 해더 길이 조사
	m_pszHeaderMBCS = new char[ nLen + 1 ];

	if ( m_pszHeaderMBCS )								// 해더 문자열을 변환해준다.
	{
		::WideCharToMultiByte(CP_ACP, 0, m_sHeader.GetBuffer(), -1, m_pszHeaderMBCS, nLen, 0, 0);			// wchar* -> char* 변환
		m_pszHeaderMBCS[nLen - 1]	= '\n';			// 개행 문자
		m_pszHeaderMBCS[nLen]		= NULL;			// 종료 문자
	}

#else

	// MBCS -> Unicode로 변환을 해준다.
	nLen = ::MultiByteToWideChar(CP_ACP, 0, m_sHeader.GetBuffer(), -1, NULL, 0);
	m_pwszHeaderUnicode = new WCHAR[ nLen + 1 ];

	if ( m_pwszHeaderUnicode )								// 해더 문자열을 변환해준다.
	{
		::MultiByteToWideChar(CP_ACP, 0, m_sHeader.GetBuffer(), -1, m_pwszHeaderUnicode, nLen);
		m_pwszHeaderUnicode[nLen - 1]	= '\n';			// 개행 문자
		m_pwszHeaderUnicode[nLen]		= NULL;			// 종료 문자
	}

#endif

}


// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ 
// Data에 저장할 데이터의 접두문자를 구한다. (날짜/시각 등)
//
// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
CString CGxLog::GetPrefix()
{
	CString sPrefix = _T("");

	// 접두글자를 기록하라고 지정하였다면,
	if ( m_bIsTimePrint || m_bIsTimePrint ) 
	{
		SYSTEMTIME tmNow;
		::GetLocalTime( &tmNow );			// 현재 시각을 얻는다.

		if ( m_bIsDatePrint )					// 날짜를 출력하라고 지정하였다면,
		{
			sPrefix.Format(_T("%04u-%02u-%02u "), tmNow.wYear, tmNow.wMonth, tmNow.wDay);			// 년-월-일을 지정한다. 예:2015-09-01
		}

		if ( m_bIsTimePrint )					// 시각을 출력하라고 지정하였다면,
		{
			// 시:분:초:밀리초 시각을 표시한다. 예: 16:29:34:460
			sPrefix.AppendFormat(_T("%02u:%02u:%02u:%03u "), tmNow.wHour, tmNow.wMinute, tmNow.wSecond, tmNow.wMilliseconds);
		}
	}

	return sPrefix;
}




// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ 
// 하루를 구분하는  기준 시각을 지정한다.
// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
void CGxLog::SetToday(int nHour, int nMin)	
{
	// 합당한 값인지 조사한다.
	if ( (nHour >= 0) && (nHour <= 23) && (nMin >= 0) && (nMin <= 59) )
	{
		m_nTodayHour			= nHour;								// 하루를 구분하는 시간
		m_nTodayMin				= nMin;								// 하루를 구분하는 분

		// 0시 00분이 아니라면 사용자 지정 하루기준 적용이다.
		m_bIsCustomToday		= (m_nTodayHour != 0) || (m_nTodayMin != 0);
	}
}


// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ 
// 지정된 하루 단위 시각에 맞춰서 기록을 담당할 '오늘' 날자값을 얻어온다.
//
// [in] int nDaySpan : 오늘 기준으로 전/후 일자를 가감하여 오늘 날짜를 산출한다.
//							0 : 현재시각 기준
//							-1 : 현재시각으로 하루 전, 음수는 전일
//							1 : 현재시각으로 하루 뒤, 양수는 다음 날
// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
CTime CGxLog::GetToday(int nDaySpan)
{
	CTime tmNow = CTime::GetCurrentTime();					// 현재 시각을 가져온다.

	// 사용자가 지정한 하루 기준 시각을 사용하기로 설정되었다면 그 기준 시각에 해당되는 값을 구한다.
	if ( m_bIsCustomToday )
	{
		// 하루를 나누는 시각과 비교하여 시/분으로 기준을 나누어준다.
		if (tmNow.GetHour() == m_nTodayHour)				// Cell Log 파일 생성에 기준이되는 하루의 시작 시간 00 ~ 23
		{
			// 기준으로 정해놓은 '시'가 같은경우
			if(tmNow.GetMinute() < m_nTodayMin)				// Cell Log 파일 생성에 기준이된는 하루의 시작 분 00 ~ 59
			{
				// 지정한 '분'을 아직 넘기지 못했다면, 어제 날짜로 사용한다.
				CTimeSpan timeOffset(1, 0, 0, 0);			// 하루
				tmNow -= timeOffset;								// 오늘 날짜에서 하루를 빼준다.
			}
		}
		else if(tmNow.GetHour() <  m_nTodayHour)			// 아직 기준 시간에 도달하지 못했다 : 어제날짜
		{
			CTimeSpan timeOffset(1, 0, 0, 0);
			tmNow -= timeOffset; 
		}
	}

	// 임의로 날짜를 당기거나 미루도록 지정되어있다면,
	if ( nDaySpan != 0 )
	{
		// 지정한 날짜만큼 연산을 수행한다.
		CTimeSpan timeOffset(nDaySpan, 0, 0, 0);
		tmNow += timeOffset; 
	}

	return tmNow;
}




// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ 
// 지정한 시각에 해당되는 파이름을 생성한다.
//
// [in] CTime tmNow : 파일명을 생성하고자 하는 기준 시각
//
// return : 경로 생성 옵션 설정값에 따라 만들어진 파일 Full Path
//
// m_nDirNameMode, m_nDirDatePos, m_nFileDatePos 경우의 수에 따른 파일 경로명 조견표
//
// 0, 0, 0 : D:\UB-AMT\Log\Sample\LogTest.txt
// 0, 0, 1 : D:\UB-AMT\Log\Sample\20150904_LogTest.txt
// 0, 0, 2 : D:\UB-AMT\Log\Sample\LogTest_20150904.txt
//
// 0, 1, 0 : D:\UB-AMT\Log\Sample\LogTest.txt
// 0, 1, 1 : D:\UB-AMT\Log\Sample\20150904_LogTest.txt
// 0, 1, 2 : D:\UB-AMT\Log\Sample\LogTest_20150904.txt
//
// 0, 2, 0 : D:\UB-AMT\Log\Sample\LogTest.txt
// 0, 2, 1 : D:\UB-AMT\Log\Sample\20150904_LogTest.txt
// 0, 2, 2 : D:\UB-AMT\Log\Sample\LogTest_20150904.txt
//
//
// 1, 0, 0 : D:\UB-AMT\Log\Sample\LogTest.txt
// 1, 0, 1 : D:\UB-AMT\Log\Sample\20150904_LogTest.txt
// 1, 0, 2 : D:\UB-AMT\Log\Sample\LogTest_20150904.txt
//
// 1, 1, 0 : D:\UB-AMT\Log\2015\Sample\LogTest.txt
// 1, 1, 1 : D:\UB-AMT\Log\2015\Sample\20150904_LogTest.txt
// 1, 1, 2 : D:\UB-AMT\Log\2015\Sample\LogTest_20150904.txt
//
// 1, 2, 0 : D:\UB-AMT\Log\Sample\2015\LogTest.txt
// 1, 2, 1 : D:\UB-AMT\Log\Sample\2015\20150904_LogTest.txt
// 1, 2, 2 : D:\UB-AMT\Log\Sample\2015\LogTest_20150904.txt
//
//
// 2, 0, 0 : D:\UB-AMT\Log\Sample\LogTest.txt
// 2, 0, 1 : D:\UB-AMT\Log\Sample\20150904_LogTest.txt
// 2, 0, 2 : D:\UB-AMT\Log\Sample\LogTest_20150904.txt
//
// 2, 1, 0 : D:\UB-AMT\Log\2015\09\Sample\LogTest.txt
// 2, 1, 1 : D:\UB-AMT\Log\2015\09\Sample\20150904_LogTest.txt
// 2, 1, 2 : D:\UB-AMT\Log\2015\09\Sample\LogTest_20150904.txt
//
// 2, 2, 0 : D:\UB-AMT\Log\Sample\2015\09\LogTest.txt
// 2, 2, 1 : D:\UB-AMT\Log\Sample\2015\09\20150904_LogTest.txt
// 2, 2, 2 : D:\UB-AMT\Log\Sample\2015\09\LogTest_20150904.txt
//
//
// 3, 0, 0 : D:\UB-AMT\Log\Sample\LogTest.txt
// 3, 0, 1 : D:\UB-AMT\Log\Sample\20150904_LogTest.txt
// 3, 0, 2 : D:\UB-AMT\Log\Sample\LogTest_20150904.txt
//
// 3, 1, 0 : D:\UB-AMT\Log\2015\09\04\Sample\LogTest.txt
// 3, 1, 1 : D:\UB-AMT\Log\2015\09\04\Sample\20150904_LogTest.txt
// 3, 1, 2 : D:\UB-AMT\Log\2015\09\04\Sample\LogTest_20150904.txt
//
// 3, 2, 0 : D:\UB-AMT\Log\Sample\2015\09\04\LogTest.txt
// 3, 2, 1 : D:\UB-AMT\Log\Sample\2015\09\04\20150904_LogTest.txt
// 3, 2, 2 : D:\UB-AMT\Log\Sample\2015\09\04\LogTest_20150904.txt
//
//
// 4, 0, 0 : D:\UB-AMT\Log\Sample\LogTest.txt
// 4, 0, 1 : D:\UB-AMT\Log\Sample\20150904_LogTest.txt
// 4, 0, 2 : D:\UB-AMT\Log\Sample\LogTest_20150904.txt
//
// 4, 1, 0 : D:\UB-AMT\Log\09\Sample\LogTest.txt
// 4, 1, 1 : D:\UB-AMT\Log\09\Sample\20150904_LogTest.txt	-> Cell log 기록용 설정 값
// 4, 1, 2 : D:\UB-AMT\Log\09\Sample\LogTest_20150904.txt
//
// 4, 2, 0 : D:\UB-AMT\Log\Sample\09\LogTest.txt
// 4, 2, 1 : D:\UB-AMT\Log\Sample\09\20150904_LogTest.txt
// 4, 2, 2 : D:\UB-AMT\Log\Sample\09\LogTest_20150904.txt
//
//
// 5, 0, 0 : D:\UB-AMT\Log\Sample\LogTest.txt
// 5, 0, 1 : D:\UB-AMT\Log\Sample\20150904_LogTest.txt
// 5, 0, 2 : D:\UB-AMT\Log\Sample\LogTest_20150904.txt
//
// 5, 1, 0 : D:\UB-AMT\Log\04\Sample\LogTest.txt
// 5, 1, 1 : D:\UB-AMT\Log\04\Sample\20150904_LogTest.txt
// 5, 1, 2 : D:\UB-AMT\Log\04\Sample\LogTest_20150904.txt
//
// 5, 2, 0 : D:\UB-AMT\Log\Sample\04\LogTest.txt
// 5, 2, 1 : D:\UB-AMT\Log\Sample\04\20150904_LogTest.txt
// 5, 2, 2 : D:\UB-AMT\Log\Sample\04\LogTest_20150904.txt
//
//
// 6, 0, 0 : D:\UB-AMT\Log\Sample\LogTest.txt
// 6, 0, 1 : D:\UB-AMT\Log\Sample\20150904_LogTest.txt
// 6, 0, 2 : D:\UB-AMT\Log\Sample\LogTest_20150904.txt
//
// 6, 1, 0 : D:\UB-AMT\Log\201509\Sample\LogTest.txt
// 6, 1, 1 : D:\UB-AMT\Log\201509\Sample\20150904_LogTest.txt
// 6, 1, 2 : D:\UB-AMT\Log\201509\Sample\LogTest_20150904.txt
//
// 6, 2, 0 : D:\UB-AMT\Log\Sample\201509\LogTest.txt
// 6, 2, 1 : D:\UB-AMT\Log\Sample\201509\20150904_LogTest.txt	-> Loss Time (비가동 로그) 기록용 설정값
// 6, 2, 2 : D:\UB-AMT\Log\Sample\201509\LogTest_20150904.txt	-> Alarm 및 TACT Time 로그 기록용 설정값
//
//
// 7, 0, 0 : D:\UB-AMT\Log\Sample\LogTest.txt
// 7, 0, 1 : D:\UB-AMT\Log\Sample\20150904_LogTest.txt
// 7, 0, 2 : D:\UB-AMT\Log\Sample\LogTest_20150904.txt
//
// 7, 1, 0 : D:\UB-AMT\Log\20150904\Sample\LogTest.txt
// 7, 1, 1 : D:\UB-AMT\Log\20150904\Sample\20150904_LogTest.txt
// 7, 1, 2 : D:\UB-AMT\Log\20150904\Sample\LogTest_20150904.txt
//
// 7, 2, 0 : D:\UB-AMT\Log\Sample\20150904\LogTest.txt
// 7, 2, 1 : D:\UB-AMT\Log\Sample\20150904\20150904_LogTest.txt
// 7, 2, 2 : D:\UB-AMT\Log\Sample\20150904\LogTest_20150904.txt
//
//
// 8, 0, 0 : D:\UB-AMT\Log\Sample\LogTest.txt
// 8, 0, 1 : D:\UB-AMT\Log\Sample\20150904_LogTest.txt
// 8, 0, 2 : D:\UB-AMT\Log\Sample\LogTest_20150904.txt
//
// 8, 1, 0 : D:\UB-AMT\Log\0904\Sample\LogTest.txt
// 8, 1, 1 : D:\UB-AMT\Log\0904\Sample\20150904_LogTest.txt
// 8, 1, 2 : D:\UB-AMT\Log\0904\Sample\LogTest_20150904.txt
//
// 8, 2, 0 : D:\UB-AMT\Log\Sample\0904\LogTest.txt
// 8, 2, 1 : D:\UB-AMT\Log\Sample\0904\20150904_LogTest.txt
// 8, 2, 2 : D:\UB-AMT\Log\Sample\0904\LogTest_20150904.txt
//
// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
CString	CGxLog::GetFilePath(CTime tmNow)
{
	CString sPath = _T("");		// 경로
	CString sDirDate = _T("");	// 날짜 관련 중간 경로 이름

	// 폴더이름에 사용 할 날짜 옵션 처리
	switch ( m_nDirNameMode )
	{
		case eNAME_None	:	// 지정한 폴더에서만 파일 생성, 날짜가 없다.
			sDirDate = _T("");
			break;

		case eNAME_Year :	// 년별로 새로운 폴더 생성,				\LOG\2015\test.txt
			sDirDate.Format(_T("%04u"), tmNow.GetYear());
			break;

		case eNAME_Month :	// 년/월 별로 새로운 폴더 생성,			\LOG\2015\09\test.txt
			sDirDate.Format(_T("%04u\\%02u"), tmNow.GetYear(), tmNow.GetMonth());
			break;

		case eNAME_Day :	// 년/월/일 별로 새로운 폴더 생성,		\LOG\2015\09\03\test.txt
			sDirDate.Format(_T("%04u\\%02u\\%02u"), tmNow.GetYear(), tmNow.GetMonth(), tmNow.GetDay());
			break;

		case eNAME_MonthOnly :	// 월별로 생성한다. (년은 생략),		\LOG\09\test.txt
			sDirDate.Format(_T("%02u"), tmNow.GetMonth());
			break;

		case eNAME_DayOnly :	// 일별로 생성한다. (년/월은 생략),		\LOG\03\test.txt
			sDirDate.Format(_T("%02u"), tmNow.GetDay());
			break;
			
		case eNAME_YearMonth :	// 년월 별로 새로운 폴더 생성,			\LOG\201509\test.txt
			sDirDate.Format(_T("%04u%02u"), tmNow.GetYear(), tmNow.GetMonth());
			break;
			
		case eNAME_YearMonthDay :	// 년월일 별로 새로운 폴더 생성,		\LOG\20150903\test.txt
			sDirDate.Format(_T("%04u%02u%02u"), tmNow.GetYear(), tmNow.GetMonth(), tmNow.GetDay());
			break;

		case eNAME_MonthDay :	// 월일 별로 새로운 폴더 생성,			\LOG\0903\test.txt
		default:
			sDirDate.Format(_T("%02u%02u"), tmNow.GetMonth(), tmNow.GetDay());
			break;

	}//of switch ( m_nDirNameMode )


	// 자동으로 날짜 경로명 생서시 중간 그룹 이름과 생성 날자를 어떻게 배치할 것인가 ? 
	switch ( m_nDirDatePos )
	{
		case ePOS_DateNone :	// 파일 이름에 날짜 정보를 넣지 않는다. -> 사용 금지, 파일이 거대해질 수 있다.
			if ( !m_sGroupName.IsEmpty() )	// 그룹 이름이 지정되어있고
			{
				sPath.Format(_T("%s%s\\"), m_sBasePath, m_sGroupName);			// 기본경로 + 그룹이름
			}
			else
				sPath = m_sBasePath;		// 그룹 이름이 없는 경우에는 기본 경로 그대로를 이용한다.
			break;


		case ePOS_DateFirst :	// 파일 이름/ 중간 구분자 앞에 날짜를 넣는다.	D:\AUTO_TEST\LOG\05\CELL_DATA\20150512_CELL_Y01.csv
			if ( sDirDate.IsEmpty() )		// 날짜를 사용하지 않는다.
			{
				if ( !m_sGroupName.IsEmpty() )	// 그룹 이름이 지정되어있는가 ?
				{
					sPath.Format(_T("%s%s\\"), m_sBasePath, m_sGroupName);			// 기본경로 + 그룹이름
				}
				else
					sPath = m_sBasePath;		// 날짜 폴더도 없고 그룹 이름이 없는 경우에는 기본 경로 그대로를 이용한다.
			}
			else		// 날짜 그룹이 존재
			{
				if ( !m_sGroupName.IsEmpty() )	// 그룹 이름이 지정되어있는가 ?
				{
					sPath.Format(_T("%s%s\\%s\\"), m_sBasePath, sDirDate, m_sGroupName);		// 기본경로 + 날짜 + 그룹이름
				}
				else
					sPath.Format(_T("%s%s\\"), m_sBasePath, sDirDate);			// 기본경로 + 날짜 폴더
			}
			break;


		case ePOS_DateLast :	// 파일 이름/ 중간 구분자 뒤에 날짜를 넣는다.	D:\Machine_Data\Alarm_Data\20150110\Alarm_20150110.csv
		default:

			if ( sDirDate.IsEmpty() )		// 날짜를 사용하지 않는다.
			{
				if ( !m_sGroupName.IsEmpty() )									// 그룹 이름이 지정되어있는가 ?
				{
					sPath.Format(_T("%s%s\\"), m_sBasePath, m_sGroupName);		// 기본경로 + 그룹이름
				}
				else
					sPath = m_sBasePath;		// 날짜 폴더도 없고 그룹 이름이 없는 경우에는 기본 경로 그대로를 이용한다.
			}
			else		// 날짜 그룹이 존재
			{
				if ( !m_sGroupName.IsEmpty() )									// 그룹 이름이 지정된 경우
				{
					sPath.Format(_T("%s%s\\%s\\"), m_sBasePath, m_sGroupName, sDirDate);		// 기본경로 + 날짜 + 그룹이름
				}
				else															// 그룹 이름이 없는 경우
					sPath.Format(_T("%s%s\\"), m_sBasePath, sDirDate);			// 기본경로 + 날짜 폴더
			}
			break;
	}//of switch ( m_nFileDatePos )



	// 자동으로 파일이름을 생성시 지정 파일 이름과 생성 날자를 어떻게 배치할 것인가 ? 
	switch ( m_nFileDatePos )
	{
		case ePOS_DateNone :	// 파일 이름에 날짜 정보를 넣지 않는다. -> 사용 금지, 파일이 거대해질 수 있다.
			sPath.AppendFormat(_T("%s.%s"), m_sFilename, m_sExtention);
			break;

		case ePOS_DateFirst :	// 파일 이름/ 중간 구분자 앞에 날짜를 넣는다.	D:\AUTO_TEST\LOG\05\CELL_DATA\20150512_CELL_Y01.csv
			if ( !m_bIsHourly )		// 시간단위가 아닌 일별 단위로 생성하라고 지정되었다면,
			{
				sPath.AppendFormat(_T("%04u%02u%02u_%s.%s"), tmNow.GetYear(), tmNow.GetMonth(), tmNow.GetDay(), m_sFilename, m_sExtention);
			}
			else // 시간단위로 파일을 생성할 것인가 ?
				sPath.AppendFormat(_T("%04u%02u%02u_%02u_%s.%s"), tmNow.GetYear(), tmNow.GetMonth(), tmNow.GetDay(), tmNow.GetHour(), m_sFilename, m_sExtention);
			break;

		case ePOS_DateLast :	// 파일 이름/ 중간 구분자 뒤에 날짜를 넣는다.	D:\Machine_Data\Alarm_Data\20150110\Alarm_20150110.csv
		default:
			if ( !m_bIsHourly )		// 시간단위가 아닌 일별 단위로 생성하라고 지정되었다면,
			{
				sPath.AppendFormat(_T("%s_%04u%02u%02u.%s"), m_sFilename, tmNow.GetYear(), tmNow.GetMonth(), tmNow.GetDay(), m_sExtention);
			}
			else // 시간단위로 파일을 생성할 것인가 ?
				sPath.AppendFormat(_T("%s_%04u%02u%02u_%02u.%s"), m_sFilename, tmNow.GetYear(), tmNow.GetMonth(), tmNow.GetDay(), tmNow.GetHour(), m_sExtention);
			break;
	}//of switch ( m_nFileDatePos )

	return sPath;
}



// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ 
// List의 내용을 모두 지운다.
// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
void CGxLog::ClearList()
{
	// 지정 내용 확인
	if ( m_pCaller && (m_nListID > 0) )		
	{
		try 
		{	// 예외상항 감지, 강제 종료시에는 예외상황이 발생 할 우려가 있다.
			CListCtrl *pListLog = (CListCtrl *)(m_pCaller->GetDlgItem(m_nListID));	// Control의 포인터를 얻어낸다.
			
			if ( pListLog ) 
			{
				pListLog->DeleteAllItems();
			} // of if (pListLog) {
		} catch (...)	
		{	// 예외 상항 발생
			if (m_bDebugLog) TRACE(_T("Exception ERROR ! [CGxLog class] Resource Not Found ! : AppendData()"));
		} 
	}
}// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _


// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ 
// 지정된 보관 기간이 지난 이전 로그를 삭제해준다. (외부에서 호출하는 Trigger 함수)
// 삭제 기준 시각은 현재 함수가 호출된 시각으로 보며, 기본적으로 m_nKeepDay 설정값으로 대상을 찾는다.
//
// [in] int nDay : 기본적인 삭제 대상일로부터 얼마나 더 과거의 날짜를 대상으로 삼을것인가 ?
//				0 : 삭제 대상 당일,					예) 삭제대상:9월 4일 -> 9월 4일 그대로 사용
//				1 : 삭제 대상일로부터 1일 전,		예) 삭제대상:9월 4일 -> 9월 3일, 하루 전
//				2 : 삭제 대상일로부터 2일 전,		예) 삭제대상:9월 4일 -> 9월 2일, 이틀 전
//
// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
BOOL CGxLog::RemoveOldLog(int nDay)
{
	CTime tmDest;
	
	//old tmDest = CTime::GetCurrentTime() - CTimeSpan((m_nKeepDay + nDay), 0, 0, 0);	// 삭제 대상의 시각을 구한다.
	tmDest = GetToday( - (m_nKeepDay + nDay));				// 삭제 대상의 시각을 구한다.

	CString sDestFile = GetFilePath( tmDest );				// 삭제 대상이 되는 파일 이름을 구한다.

	// 1. 삭제 대상 파일이 존재하는가 ?
	if ( IsExistFile( sDestFile ) )
	{
		// 2. 대상 파일을 삭제한다.
		try
		{
			CFile::Remove( sDestFile );

			if (m_bDebugLog) TRACE(_T("CGxLog::RemoveOldLog(%s) File remove ok, %s\n"), m_sFilename, sDestFile);
		}
		catch(...)
		{
			if (m_bDebugLog) TRACE(_T("* CGxLog::RemoveOldLog(%s) File remove FAIL ! : %s\n"), m_sFilename, sDestFile);
			return FALSE;		// 수행 실패
		}
	}
	else	// 파일이 존재하지 않으면 이미 이전에 삭제를 마친것으로 판단하여 폴더를 삭제하도록 한다.
	{
		// TRACE(_T("File Not Found : %s\n"), sDestFile);

		// 폴더가 비었는지 확인하여 빈 폴더이면 삭제하도록 한다.
		RemoveEmptyFolder( sDestFile );
	}

	return TRUE;
}



// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ 
// List control에 표시하기위한 각 핸들을 지정해준다. 자동으로 화면에 기록하는 옵션이 활성화된다.
// 
// [in] CWnd* pViewHandle	: List control이 존재하는 폼의 CWnd 포인터, this->m_hWnd로 호출한다.
// [in] int nListID		: List Control의 Resourec ID
//
// 각 설정값들이 합당하다면 지정 설정값에 해당되는 기능 옵션은 활성화 된다.
// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _

void CGxLog::WriteToList(CString &sData)
{
	// 화면에 표시하도록 하였다면,
	if ( m_pCaller && (m_nListID > 0) )		// 기록 조건 : List에 기록하도록 지정 AND CWnd 지정 AND ListCtrl 리소스 ID 존재
	{
		try {	// 예외상항 감지, 강제 종료시에는 예외상황이 발생 할 우려가 있다.
			CListCtrl *pListLog = (CListCtrl *)(m_pCaller->GetDlgItem(m_nListID));	// Control의 포인터를 얻어낸다.
			 
			if ( pListLog ) 
			{
				//d pListLog->LockWindowUpdate();

					if (pListLog->GetItemCount() >= m_nMaxLineCount)	pListLog->DeleteItem(0);	// 지정 갯수를 유지한다.
					pListLog->InsertItem( pListLog->GetItemCount(), sData);						// 데이터 추가

					pListLog->EnsureVisible(pListLog->GetItemCount() - 1, TRUE);				// 마지막에 포커스되게한다. 

				//d pListLog->UnlockWindowUpdate();
			} // of if (pListLog) {
		} catch (...)	
		{	// 예외 상항 발생
			if (m_bDebugLog) TRACE(_T("Exception ERROR ! [CGxLog class] ListCtrl Resource Not Found ! : Log()\n"));
		} 
	} // of if list print
}



// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ 
// 지정 파일에 유니코드로 저장을 한다.(Thread Safety 지원)
//
// [in] CString sPath		: Log를 남길 File의 절대 경로
// [in] CString sDatae		: Log를 남길 데이터
// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
void CGxLog::WriteToUnicodeFile_s(CString sPath, CString &sData)
{
	if ( sPath.IsEmpty() ) return;					// 파일 이름을 지정하지 않았다면 수행할 수 없다.

	FILE*	pFile;										// 기본적으로 사용되는 File Handle
	errno_t	errFileOpen;							// 파일을 정상적으로 Open 하였나 ?
	BOOL	bLockFlag = ( m_bIsThreadLock && (m_dwUIThread != GetCurrentThreadId()) );	// 호출한 Thread 를 조사하여 Thread lock을 수행할것인지 지정한다.
	CSingleLock slWriteLock( &m_CSLogWrite );		// 자원에 대한 중복 사용금지

	// 헤더 출력 조건 = 헤더출력 옵션지정 AND 해더내용존재 AND 파일이 존재하지 않음(즉, 새로운 파일 생성)
	BOOL	bHeader = m_bIsHeaderPrint && !m_sHeader.IsEmpty() && !IsExistFile(sPath);

#if defined(UNICODE) || defined(_UNICODE)			// Unicode 개발 환경에서 Unicode로 저장하는 경우

	if ( bLockFlag ) slWriteLock.Lock(); // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		try
		{
			// 별도의 변환없이 Unicode 파일을 지정하여 열어서 곧바로 기록해준다.
			// 파일에 대하여 최소한으로 접근하도록 한다.
			errFileOpen = _tfopen_s( &pFile, sPath, _T("a+t, ccs=UTF-8") );		// 파일을 성공적으로 열었는지 

			if ( (errFileOpen == 0) && pFile )
			{
				if ( bHeader )										// 해더를 출력해야하는 조건인가 ?
				{
					_ftprintf( pFile, _T("%s\n"), m_sHeader);		// 해더 출력
				}

				_ftprintf( pFile, _T("%s\n"), sData);				// 지정한 데이터를 출력한다.
			}
				
		// 도중에 __finally로 가려면 __leave 명령을 수행한다.
		}
		catch(...)													// 예외 처리 
		{
			if (m_bDebugLog) TRACE(_T("\n* CGxLog::WriteToUnicodeFile_s(%s) File open & write FAIL [%s] -> %d\n"), m_sFilename, sPath, errFileOpen);
		}
		// finally		// ㅊ++에는 try ~ finally가 없다
		{
			if ( (errFileOpen == 0) && pFile )
			{
				fclose( pFile );												
			}
		}

	if ( bLockFlag ) slWriteLock.Unlock(); // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#else		// Unicode 환경이 아닐경우

	// MBCS -> Unicode는 변환을 해준다.
	int nLen = MultiByteToWideChar(CP_ACP, 0, sData.GetBuffer(), -1, NULL, 0);			// 데이터 길이를 알아내어 
	WCHAR *pszData = new WCHAR[ nLen + 1];												// 메모리 할당 (개행문자용 +1)

	if ( pszData )			// 메모리 할당 정상 확인
	{
		::MultiByteToWideChar(CP_ACP, 0, sData.GetBuffer(), -1, pszData, nLen);
		pszData[nLen - 1]	= '\n';			// 개행 문자
		pszData[nLen]		= NULL;			// 종료 문자

		if ( bLockFlag ) slWriteLock.Lock(); // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	
			try
			{
				errFileOpen = _tfopen_s( &pFile, sPath, _T("a+t, ccs=UTF-8") );		// 파일을 성공적으로 열었는지 

				if ( (errFileOpen == 0) && pFile )
				{
					if ( bHeader && m_pwszHeaderUnicode )
					{
						fputws( m_pwszHeaderUnicode, pFile );						// 해더를 파일에 기록한다.
					}

					fputws( pszData, pFile );							// 파일에 기록한다.
				}
			}
			catch(...)
			{
				if (m_bDebugLog) TRACE(_T("\n* CGxLog::WriteToUnicodeFile_s(%s) File open & write FAIL [%s] -> %d\n"), m_sFilename, sPath, errFileOpen);
			}
			// finally													// 최종 처리 
			{
				if ( (errFileOpen == 0) && pFile )
				{
					fclose( pFile );												
				}
			}

		if ( bLockFlag ) slWriteLock.Unlock(); // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		delete[] pszData;													// 할당된 메모리 해제
	}//of if NOT NULL

#endif //of if unicode

}




// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ 
// 지정 파일에 MBCS로 저장을 한다.(Thread Safety 지원)
//
// [in] CString sPath		: Log를 남길 File의 절대 경로
// [in] CString sDatae		: Log를 남길 데이터
// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
void CGxLog::WriteToMBCSFile_s(CString sPath, CString &sData)
{
	if ( sPath.IsEmpty() ) return;					// 파일 이름을 지정하지 않았다면 수행할 수 없다.

	FILE*	pFile;									// 기본적으로 사용되는 File Handle
	errno_t	errFileOpen;							// 파일을 정상적으로 Open 하였나 ?
	BOOL	bLockFlag = ( m_bIsThreadLock && (m_dwUIThread != GetCurrentThreadId()) );	// 호출한 Thread 를 조사하여 Thread lock을 수행할것인지 지정한다.
	CSingleLock slWriteLock( &m_CSLogWrite );		// 자원에 대한 중복 사용금지

	// 헤더 출력 조건 = 헤더출력 옵션지정 AND 해더내용존재 AND 파일이 존재하지 않음(즉, 새로운 파일 생성)
	BOOL	bHeader = m_bIsHeaderPrint && !m_sHeader.IsEmpty() && !IsExistFile(sPath);


#if defined(UNICODE) || defined(_UNICODE)			// Unicode 개발 환경에서 MBCS로 저장하는 경우

	// Unicode -> MBCS는 변환을 해준다. CP_UTF8, CP_ACP
	int nLen = WideCharToMultiByte(CP_ACP, 0, sData.GetBuffer(), -1, NULL, 0, NULL, NULL);		// 데이터 길이 조사
	char *pszData = new char[ nLen + 1];														// 메모리 할당 (개행문자용 +1)

	if ( pszData )			// 메모리 할당 정상 확인
	{
		::WideCharToMultiByte(CP_ACP, 0, sData.GetBuffer(), -1, pszData, nLen, 0, 0);			// wchar* -> char* 변환
		pszData[nLen - 1]	= '\n';			// 개행 문자
		pszData[nLen]		= NULL;			// 종료 문자

		if ( bLockFlag ) slWriteLock.Lock(); // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	
			try
			{
				errFileOpen = _tfopen_s( &pFile, sPath, _T("a+t") );	// 파일을 성공적으로 열었는지 

				if ( (errFileOpen == 0) && pFile )
				{
					if ( bHeader && m_pszHeaderMBCS )
					{
						fputs( m_pszHeaderMBCS, pFile );				// 파일에 Header를 기록한다.
					}

					fputs( pszData, pFile );							// 파일에 기록한다.
				}
			}
			catch(...)
			{
				if (m_bDebugLog) TRACE(_T("\n* CGxLog::WriteToMBCSFile_s(%s) File open & write FAIL [%s] -> %d\n"), m_sFilename, sPath, errFileOpen);
			}
			// finally													// 최종 처리 
			{
				if ( (errFileOpen == 0) && pFile )
				{
					fclose( pFile );												
				}
			}

		if ( bLockFlag ) slWriteLock.Unlock(); // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		delete pszData;													// 할당된 메모리 해제
	}//of if NOT NULL

#else		// Unicode 환경이 아닐경우

	if ( bLockFlag ) slWriteLock.Lock(); // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		try
		{
			// 별도의 변환없이 ANSI 파일을 지정하여 열어서 곧바로 기록해준다.
			errFileOpen = _tfopen_s( &pFile, sPath, _T("a+t") );		// 파일을 성공적으로 열었는지 

			if ( (errFileOpen == 0) && pFile )
			{
				if ( bHeader )										// 해더를 출력해야하는 조건인가 ?
				{
					_ftprintf( pFile, _T("%s\n"), m_sHeader);		// 해더 출력
				}

				_ftprintf( pFile, _T("%s\n"), sData);				// 지정한 데이터를 출력한다.
			}
				
		// 도중에 __finally로 가려면 __leave 명령을 수행한다.
		}
		catch(...)													// 예외 처리 
		{
			if (m_bDebugLog) TRACE(_T("\n* CGxLog::WriteToMBCSFile_s(%s) File open & write FAIL [%s] -> %d\n"), m_sFilename, sPath, errFileOpen);
		}
		// finally		// ㅊ++에는 try ~ finally가 없다
		{
			if ( (errFileOpen == 0) && pFile )
			{
				fclose( pFile );												
			}
		}

	if ( bLockFlag ) slWriteLock.Unlock(); // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif //of if unicode

}



// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ 
// 주어진 양식문대로 로그를 남긴다.
//
// [in] printf()와 호환되는 서식 지정 문자열과 변수 리스트
// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
void CGxLog::Write(LPCTSTR szFmt, ...)
{
	if ( !m_bIsEnabled ) return;							// Log를 기록하지 않는다면, 바로 되돌아간다.
	if ( !m_bIsFileWrite && !m_bIsListWrite ) return;		// List 혹은 File에 기록을 하지 않는다면 수행하지 않는다.

	CString sLogData = _T("");

	// Format the message text
	va_list argptr;
	va_start(argptr, szFmt);
	sLogData.FormatV(szFmt, argptr);
	va_end(argptr);

	// 기록할 문자열을 완성한다.
	// sLogData = GetPrefix() + sLogData;					// 데이터 앞에 날짜와 시간을 만들어 준다.
	sLogData.Insert(0, GetPrefix());

	// 출력할 내용이 없다면 수행하지 않는다.
	if ( sLogData.IsEmpty() ) return;

	// 화면에 표시
	if ( m_bIsListWrite )	WriteToList( sLogData );


	// 파일로 저장하는 설정이 올바르다면, 파일로 저장한다.
	if ( m_bIsFileWrite && !m_sBasePath.IsEmpty() && !m_sFilename.IsEmpty() )
	{
		CString sFilePath = GetFilePath( GetToday(0) );		// 현재 시각으로 파일을 생성하여 기록한다.
		BOOL bFolerCheck = CheckFolder( sFilePath, m_bIsFolderCreate );	// 지정한 경로의 폴더가 존재하는지 점검한다.

		// 지정한 폴더가 존재해야 파일을 생성할 수 있다.
		if ( bFolerCheck )
		{
			// Unicode로 저장을 할것인지 여부를 비교하여 Unicode 미 MBCS로 저장을 수행시킨다.
			if ( m_bIsUnicode )	
			{
				WriteToUnicodeFile_s(sFilePath, sLogData);
			}
			else
				WriteToMBCSFile_s(sFilePath, sLogData);
		}
	}

}// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _




// 주어진 문자열을 로그로 남긴다. (지정한 시각으로 파일을 생성하는 기준시각을 적용한다)
//
// [in] int nDaySapn : 파일을 생성할 기준 날짜, -n : 이전일, 0 : 오늘, +n : 내일 이후, -1:어제,1:내일
// [in] printf()와 호환되는 서식 지정 문자열과 변수 리스트
// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
void CGxLog::WriteDay(int nDaySpan, LPCTSTR szFmt, ...)
{
	if ( !m_bIsEnabled ) return;							// Log를 기록하지 않는다면, 바로 되돌아간다.
	if ( !m_bIsFileWrite && !m_bIsListWrite ) return;		// List 혹은 File에 기록을 하지 않는다면 수행하지 않는다.

	CString sLogData = _T("");

	// Format the message text
	va_list argptr;
	va_start(argptr, szFmt);
	sLogData.FormatV(szFmt, argptr);
	va_end(argptr);

	// 기록할 문자열을 완성한다.
	// sLogData = GetPrefix() + sLogData;					// 데이터 앞에 날짜와 시간을 만들어 준다.
	sLogData.Insert(0, GetPrefix());

	// 출력할 내용이 없다면 수행하지 않는다.
	if ( sLogData.IsEmpty() ) return;


	// 화면에 표시
	if ( m_bIsListWrite )	WriteToList( sLogData );

	// 파일로 저장하는 설정이 올바르다면, 파일로 저장한다.
	if ( m_bIsFileWrite && !m_sBasePath.IsEmpty() && !m_sFilename.IsEmpty() )
	{
		CString sFilePath = GetFilePath( GetToday(nDaySpan) );		// 현재 시각으로 파일을 생성하여 기록한다.
		BOOL bFolerCheck = CheckFolder( sFilePath, m_bIsFolderCreate );	// 지정한 경로의 폴더가 존재하는지 점검한다.

		// 지정한 폴더가 존재해야 파일을 생성할 수 있다.
		if ( bFolerCheck )
		{
			// Unicode로 저장을 할것인지 여부를 비교하여 Unicode 미 MBCS로 저장을 수행시킨다.
			if ( m_bIsUnicode )	
			{
				WriteToUnicodeFile_s(sFilePath, sLogData);
			}
			else
				WriteToMBCSFile_s(sFilePath, sLogData);
		}
	}

}// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _



//////////////////////////////////////////////////////////////////////////////////////////////////////////////


// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ 
// 지정한 경로에 파일이 존재하는지 찾아본다.
// 
// [in]  CString sPath : 조사할 파일 경로
//
// return : TRUE - 지정한 파일이 존재한다.
//			FALSE - 지정한 파일이 존재하지 않는다.
// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
BOOL CGxLog::IsExistFile(CString sFilePath)
{
	if ( sFilePath.IsEmpty() ) return FALSE;			// 주어진 경로가 공백이라면 바로 FALSE리턴

	DWORD dwRet = GetFileAttributes( sFilePath );		// 지정 경로의 속성 획득

	// 지정 경로가 존재하지 않거나 폴더라면 
	if ( (dwRet == -1 ) || ( dwRet & FILE_ATTRIBUTE_DIRECTORY) ) 
	{
		return FALSE;									// 지정 파일이 존재하지 않는다.
	}

	return TRUE;										// 지정 파일이 존재한다.
}// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _



// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ 
// 지정한 경로에 폴더가 존재하는지 찾아본다.
// 
// [in]  CString sPath : 조사할 경로
//
// return : TRUE - 지정한 폴더가 존재한다.
//			FALSE - 지정한 폴더가 존재하지 않는다.
// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
BOOL CGxLog::IsExistFolder(CString sPath)
{
	if ( sPath.IsEmpty() ) return FALSE;			// 주어진 경로가 공백이라면 바로 FALSE리턴

	DWORD dwRet = GetFileAttributes( sPath );		// 지정 경로의 속성 획득

	// 지정 경로가 존재하지 않거나 폴더가 아니라면 
	if ( (dwRet == -1 ) || !( dwRet & FILE_ATTRIBUTE_DIRECTORY) ) 
	{
		return FALSE;								// 지정 폴더가 존재하지 않는다.
	}

	return TRUE;									// 지정 폴더가 존재한다.
}// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _



// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ 
// 지정 폴더가 피어있는지 조사한다.
//
// [in]  CString sPath : 조사할 경로
//
// return : TRUE - 지정한 폴더가 비어있다.
//			FALSE - 지정한 폴더가 비어있지 않다.
// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
BOOL CGxLog::IsEmptyFolder(CString sPath)
{
	CFileFind finder;
	BOOL bWorking;

	// 종료 문자가 역슬래쉬로 끝나는지 조사하여 와일드카드를 더해준다.
	if ( sPath.Right(1) == _T("\\") )
	{
		sPath.Append(_T("*.*"));
	}
	else
		sPath.Append(_T("\\*.*"));


    bWorking = finder.FindFile( sPath );				// 파일을 찾아낸다.

    while ( bWorking )									// 파일을 찾을 수 있으면 순환
    {
        bWorking = finder.FindNextFile();				// 다음 파일을 찾는다.

        if ( finder.IsDots() )							// ., .. 가 검색되었으면 무시한다. 
		{ 
			continue;
		}

		// ., .. 이외의 이름이 나오면 폴더이건 파일이건 비어있지 않다고 판정한다.
		return FALSE;
    }

	return TRUE;
}



// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ 
// 폴더의 존재 여부를 확인하고 존재하지 않는다면 새로이 만들어 준다. (선택사양)
//
// [in] CString sPath : 검사하고자 하는 폴더 경로
// [in] BOOL bCreate : 지정 폴더가 존재하지 않을 경우 새로이 만들것인가 ? 
//					TRUE : 새로이 만들어준다.
//					FALSE : 만들지 않는다.
// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
BOOL CGxLog::CheckFolder(CString sPath, BOOL bCreate)
{
	if ( sPath.IsEmpty() ) return FALSE;			// 주어진 경로가 공백이라면 바로 FALSE리턴

	CString sDrive, sFolder;

	// 지정 경로에서 실제적인 경로 부분만 추출하여 사용한다.
	_tsplitpath_s( sPath, sDrive.GetBuffer(_MAX_DRIVE), _MAX_DRIVE, sFolder.GetBuffer(_MAX_DIR), _MAX_DIR, NULL, 0, NULL, 0 );
	sDrive.ReleaseBuffer();
	sFolder.ReleaseBuffer();
	sFolder = sDrive + _T("\\") + sFolder;
	
	DWORD dwRet = GetFileAttributes( sFolder );		// 지정 경로의 속성 획득

	// 지정 경로가 존재하지 않거나 폴더가 아니라면 
	if ( (dwRet == -1 ) || !( dwRet & FILE_ATTRIBUTE_DIRECTORY) ) 
	{
		// 지정 폴더가 존재하지 않는경우 생성하라는 옵션이 지정되어있다면
		if ( bCreate )
		{
			SHCreateDirectoryEx(NULL, sFolder, NULL);			// 폴더를 생성해준다.

			// 생성 결과를 다시 조회한다.
			dwRet = GetFileAttributes( sFolder );				// 지정 경로의 속성 획득
			return ( (dwRet != -1 ) && ( dwRet & FILE_ATTRIBUTE_DIRECTORY) );
		}

		return FALSE;								
	}

	return TRUE;
}





// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ 
// 지정 폴더가 비어있다면 삭제를 한다.
//
// [in] CString sDir : 삭제할 폴더 경로
//
// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
void CGxLog::RemoveEmptyFolder(CString sPath)
{
	if ( sPath.IsEmpty() ) return;			// 주어진 경로가 공백이라면 바로 FALSE리턴

	int i, j, nPos;
	CString sDrive, sFolder;
	CString sTemp;
	CStringArray arrPath;							// 삭제를 시킬 경로 
	BOOL bRet = FALSE;

	// 지정 경로에서 실제적인 경로 부분만 추출하여 사용한다.
	_tsplitpath_s( sPath, sDrive.GetBuffer(_MAX_DRIVE), _MAX_DRIVE, sFolder.GetBuffer(_MAX_DIR), _MAX_DIR, NULL, 0, NULL, 0 );
	sDrive.ReleaseBuffer();
	sFolder.ReleaseBuffer();
	sFolder = sDrive + _T("\\") + sFolder;

	nPos = 0;
	sTemp = sFolder.Tokenize(_T("\\"), nPos);			// 경로를 요소별로 분리해낸다.
	while ( sTemp != _T("") )
	{
		arrPath.Add( sTemp );
		sTemp = sFolder.Tokenize(_T("\\"), nPos);
	}

	// 분리된 경로 문자열을 역으로 조립하여 본 Log class에서 생성한 폴더일때 삭제를 수행한다.
	for (i = 0; i < arrPath.GetSize(); i++)
	{
		// 깊은 경로로부터 조립을 시작한다.
		sFolder = _T("");
		for (j = 0; j < (arrPath.GetSize() - i); j++)
		{
			sFolder.AppendFormat(_T("%s\\"), arrPath[j]);
		}

		// TRACE(_T("CGxLog::RemoveEmptyFolder() Check Dir :  %s\n"), sFolder);

		// 기본으로 지정된 Base Path가 아니라면,
		if ( sFolder.CompareNoCase( m_sBasePath ) != 0 )
		{
			if ( IsExistFolder( sFolder ) )					// 폴더 존재여부 확인
			{
				// 빈 폴더일 경우 삭제를 수행한다.
				if ( IsEmptyFolder( sFolder ) )	//d PathIsDirectoryEmpty( sFolder ) )
				{
					try
					{
						RemoveDirectory( sFolder );			// 폴더 삭제

						if (m_bDebugLog) TRACE(_T("CGxLog::RemoveEmptyFolder(%s) Folder remove ok, %s\n"), m_sFilename, sFolder);
					}
					catch(...)
					{
						if (m_bDebugLog) TRACE(_T("*CGxLog::RemoveEmptyFolder(%s) Folder remove FAIL !, %s\n"), m_sFilename, sFolder);
						//d break;		// 더이상의 수행은 포기한다.
					}
				}
				else
					if (m_bDebugLog) TRACE(_T("CGxLog::RemoveEmptyFolder(%s) Not Empty, %s\n"), m_sFilename, sFolder);
			}
			else
				if (m_bDebugLog) TRACE(_T("CGxLog::RemoveEmptyFolder(%s) Not Exist, %s\n"), m_sFilename, sFolder);

		}
		else			// for 文 순회를 마친다.
			break;
	}

	arrPath.RemoveAll();
}



// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ 
// 지정 파일에 유니코드로 저장을 한다
//
// [in] CString sPath		: Log를 남길 File의 절대 경로
// [in] CString sDatae		: Log를 남길 데이터
// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
void CGxLog::WriteToUnicodeFile(CString sPath, CString &sData)
{
	if ( sPath.IsEmpty() ) return;					// 파일 이름을 지정하지 않았다면 수행할 수 없다.

	FILE*	pFile;									// 기본적으로 사용되는 File Handle
	errno_t	errFileOpen;							// 파일을 정상적으로 Open 하였나 ?

#if defined(UNICODE) || defined(_UNICODE)			// Unicode 개발 환경에서 Unicode로 저장하는 경우

	try
	{
		// 별도의 변환없이 Unicode 파일을 지정하여 열어서 곧바로 기록해준다.
		// 파일에 대하여 최소한으로 접근하도록 한다.
		errFileOpen = _tfopen_s( &pFile, sPath, _T("a+t, ccs=UTF-8") );		// 파일을 성공적으로 열었는지 

		if ( (errFileOpen == 0) && pFile )
		{
			_ftprintf( pFile, _T("%s\n"), sData);				// 지정한 데이터를 출력한다.
		}
				
	// 도중에 __finally로 가려면 __leave 명령을 수행한다.
	}
	catch(...)													// 예외 처리 
	{
		TRACE(_T("\n* CGxLog::WriteToUnicodeFile() File open & write FAIL [%s] -> %d\n"), sPath, errFileOpen);
	}
	// finally		// C++에는 try ~ finally가 없다
	{
		if ( (errFileOpen == 0) && pFile )
		{
			fclose( pFile );												
		}
	}

#else		// Unicode 환경이 아닐경우

	// MBCS -> Unicode는 변환을 해준다.
	int nLen = MultiByteToWideChar(CP_ACP, 0, sData.GetBuffer(), -1, NULL, 0);			// 데이터 길이를 알아내어 
	WCHAR *pszData = new WCHAR[ nLen + 1];												// 메모리 할당 (개행문자용 +1)

	if ( pszData )			// 메모리 할당 정상 확인
	{
		::MultiByteToWideChar(CP_ACP, 0, sData.GetBuffer(), -1, pszData, nLen);

		pszData[nLen - 1]	= '\n';			// 개행 문자
		pszData[nLen]		= NULL;			// 종료 문자

		try
		{
			errFileOpen = _tfopen_s( &pFile, sPath, _T("a+t, ccs=UTF-8") );		// 파일을 성공적으로 열었는지 

			if ( (errFileOpen == 0) && pFile )
			{
				fputws( pszData, pFile );							// 파일에 기록한다.
			}
		}
		catch(...)
		{
			// if (m_bDebugLog)  TRACE(_T("\n* CGxLog::WriteToUnicodeFile() File open & write FAIL [%s] -> %d\n"), sPath, errFileOpen);
		}
		// finally													// 최종 처리 
		{
			if ( (errFileOpen == 0) && pFile )
			{
				fclose( pFile );												
			}
		}

		delete[] pszData;													// 할당된 메모리 해제
	}//of if NOT NULL

#endif //of if unicode

}




// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ 
// 지정 파일에 MBCS로 저장을 한다.(Thread Safety 지원)
//
// [in] CString sPath		: Log를 남길 File의 절대 경로
// [in] CString sDatae		: Log를 남길 데이터
// _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
void CGxLog::WriteToMBCSFile(CString sPath, CString &sData)
{
	if ( sPath.IsEmpty() ) return;					// 파일 이름을 지정하지 않았다면 수행할 수 없다.

	FILE*	pFile;									// 기본적으로 사용되는 File Handle
	errno_t	errFileOpen;							// 파일을 정상적으로 Open 하였나 ?

#if defined(UNICODE) || defined(_UNICODE)			// Unicode 개발 환경에서 MBCS로 저장하는 경우

	// Unicode -> MBCS는 변환을 해준다. CP_UTF8, CP_ACP
	int nLen = WideCharToMultiByte(CP_ACP, 0, sData.GetBuffer(), -1, NULL, 0, NULL, NULL);		// 데이터 길이 조사
	char *pszData = new char[ nLen + 1];														// 메모리 할당 (개행문자용 +1)

	if ( pszData )			// 메모리 할당 정상 확인
	{
		::WideCharToMultiByte(CP_ACP, 0, sData.GetBuffer(), -1, pszData, nLen, 0, 0);			// wchar* -> char* 변환

		pszData[nLen - 1]	= '\n';			// 개행 문자
		pszData[nLen]		= NULL;			// 종료 문자

		try
		{
			errFileOpen = _tfopen_s( &pFile, sPath, _T("a+t") );		// 파일을 성공적으로 열었는지 

			if ( (errFileOpen == 0) && pFile )
			{
				fputs( pszData, pFile );							// 파일에 기록한다.
			}
		}
		catch(...)
		{
			TRACE(_T("\n* CGxLog::WriteToMBCSFile() File open & write FAIL [%s] -> %d\n"), sPath, errFileOpen);
		}
		// finally													// 최종 처리 
		{
			if ( (errFileOpen == 0) && pFile )
			{
				fclose( pFile );												
			}
		}

		delete pszData;													// 할당된 메모리 해제
	}//of if NOT NULL

#else		// Unicode 환경이 아닐경우

	try
	{
		// 별도의 변환없이 ANSI 파일을 지정하여 열어서 곧바로 기록해준다.
		errFileOpen = _tfopen_s( &pFile, sPath, _T("a+t") );		// 파일을 성공적으로 열었는지 

		if ( (errFileOpen == 0) && pFile )
		{
			_ftprintf( pFile, _T("%s\n"), sData);				// 지정한 데이터를 출력한다.
		}
				
	// 도중에 __finally로 가려면 __leave 명령을 수행한다.
	}
	catch(...)													// 예외 처리 
	{
		// if (m_bDebugLog)  TRACE(_T("\n* CGxLog::WriteToMBCSFile() File open & write FAIL [%s] -> %d\n"), sPath, errFileOpen);
	}
	// finally		// ㅊ++에는 try ~ finally가 없다
	{
		if ( (errFileOpen == 0) && pFile )
		{
			fclose( pFile );												
		}
	}

#endif //of if unicode

}




