#include "StdAfx.h"
#include "LossTimeLog.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"
#include "UI\GausGUI\GxMsgBox.h"
#include "UI\Dialog\DlgTPCodeList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


const LOSSTIME_FIRST_CLASS	LosstimeFirstClass[] =
{
	_T("BM"),
	_T("PM"),
	_T("CM"),
	_T("Change Model"),
	_T("Material"),
};


CLossTimeLog::CLossTimeLog(void)
{
	m_listZoneFinish.RemoveAll();

	// Test - 나중에 지워주세요.
	//SetMachineStopTime();

}


CLossTimeLog::~CLossTimeLog(void)
{
}
// 비가동 정보를 파일에서 읽어온다.
BOOL CLossTimeLog::LoadLosstimeItemList()
{
	
	CString sFilename;
	CString sGroup, sKey, sData, sLine, sTok;
	int i, j, k, n, n1stCount, n2ndCount, n3rdCount;
	int nLangIdx = GetMainHandler()->m_nLangIdx;			// 현재 표시되고 있는 언어 index

	sFilename.Format(_T("%s\\%s"), theConfigBank.m_System.m_strProgramDataPath, NAME_FILE_LOSSTIME_LIST);
	CIni ini(sFilename);
	// 파일이 존재하는지 점검
	if(CFileSupport::FileCheck(sFilename)==FALSE)
	{
		CString sMsg;
		sMsg.Format(_T("* File not found : %s"), sFilename);

		CGxMsgBox	dlgMsgBox;
		dlgMsgBox.SetTitle(_T("알림"), _T("Notifications"), _T("thông báo"));
		dlgMsgBox.SetMessage(FALSE, sMsg, sMsg, sMsg, nLangIdx);
		dlgMsgBox.DoModal();


		//::AfxMessageBox( sMsg );																// 오류 내용을 보여준다.
		return FALSE;
	}

	// 파일이 존재하면 파일에서 데이터를 읽어온다.
	//iniFile.SetIniFilename( sFilename );			// file이름을 지정

	// AUto Run-Down 항목을 읽어온다.
	m_rLosstimeList.sAutoRunDownCode	= ini.GetString(_T("Setting"), _T("AutoRunDown"),		_T("RD>15190>AUTO RUN DOWN>AUTO RUN DOWN>AUTO RUN DOWN"));		// AUTO RUN DOWN
	m_rLosstimeList.sOPLossCode		= ini.GetString(_T("Setting"), _T("OPLoss"),			_T("RD_W>0110>OP LOSS>OP LOSS>OP LOSS"));								// OP LOSS 선택 항목
	m_rLosstimeList.sTactTimeOver		= ini.GetString(_T("Setting"), _T("TactTimeOver"),	_T("RD>01200>TACT TIME OVER>TACT TIME OVER>TACT TIME OVER"));	// TATC TIME OVER 선택 항목
	m_rLosstimeList.sCellInputOver	= ini.GetString(_T("Setting"), _T("CellInputTimeOver"),	_T("RD>15190>material shortage>material shortage>material shortage"));	// 자재 없음 선택 항목

	// 전체 First Class 그룹 수량
	n1stCount = ini.GetLong( _T("GROUP"), _T("COUNT"),	0);

	// 1st class 최대 수량을 제한한다.
	if ( n1stCount > TLosstimeList::eMAX_1stCount ) n1stCount = TLosstimeList::eMAX_1stCount;

	m_rLosstimeList.nCount_1st	= n1stCount;				// 1st class 수

	for (i = 0; i < n1stCount; i++)
	{
		sGroup.Format( _T("GROUP_%02d"), i );			// 그룹이름, 00 ~ 07

		m_rLosstimeList.rLosstime[i].sName = ini.GetString( sGroup, _T("NAME"), _T("") );		// 1st class 이름
		n2ndCount = ini.GetLong( sGroup, _T("COUNT"),	0);												// 해당 1st Class의 2nd class 수

		if ( n2ndCount > TLoss1stClass::eMAX_2ndCount ) n2ndCount = TLoss1stClass::eMAX_2ndCount;	// 2nd class수 제한

		m_rLosstimeList.rLosstime[i].n2ndCount = n2ndCount;			// 해당 1stClass내 존재하는 2nd class 갯수

		// 2nd class 해당되는  항목을 읽어온다.
		for (j = 0; j < n2ndCount; j++ )
		{
			sGroup.Format( _T("GROUP_%02d"), i );			// 그룹이름, 00 ~ 07
			sKey.Format(_T("SUB_%02d"), j);				// Key
			m_rLosstimeList.rLosstime[i].r2ndClass[j].sLine	= ini.GetString( sGroup, sKey, _T("") );		// 2st class 이름

			// 각 언어별로 2개의 2nd class Name이 존재한다.
			sLine = m_rLosstimeList.rLosstime[i].r2ndClass[j].sLine;
			for (n = 0; n < LAN_MAX; n++)
			{
				// 각 언어별 2nd Class 이름을 '>' 기호로 분리하지 못한 경우에는 바로 끝낸다.
				if ( AfxExtractSubString( m_rLosstimeList.rLosstime[i].r2ndClass[j].sName[n], sLine, n, DELIMITER_DEFECT) == FALSE )	break;
			}//of for n				

			// 3rd Class data 읽기
			sGroup.Format(_T("GROUP_%02d_SUB_%02d"), i, j);				// 3rd Class를 읽기위한 Group 이름 지정
			n3rdCount = ini.GetLong( sGroup, _T("COUNT"), 0 );		// 3rd class 항목 수

			if ( n3rdCount > TLoss2ndClass::eMAX_3rdCount ) n3rdCount = TLoss2ndClass::eMAX_3rdCount;	// 3rd class수 제한

			m_rLosstimeList.rLosstime[i].r2ndClass[j].n3rdCount = n3rdCount;			// 해당 2ndClass내 존재하는 3rd class 갯수

			// 최종적인 3rd Class 항목을 읽어온다.
			for (k = 0; k < n3rdCount; k++)
			{
				sKey.Format(_T("ITEM_%02d"), k);		// Key
				m_rLosstimeList.rLosstime[i].r2ndClass[j].r3rdClass[k].sLine	= ini.GetString( sGroup, sKey, _T("") );		// 3rd class Data

				sLine = m_rLosstimeList.rLosstime[i].r2ndClass[j].r3rdClass[k].sLine;

				//예: ITEM_00   = PM>000>Shift점검>프로그램 점검>Check Program

				// Code 발췌 (현재는 미사용)
				AfxExtractSubString( m_rLosstimeList.rLosstime[i].r2ndClass[j].r3rdClass[k].sCode, sLine, 1, DELIMITER_DEFECT);	// 첫번째 항목인 Defect Code를 읽어낸다.

				for (n = 0; n < LAN_MAX; n++)
				{
					// 각 언어별 3rd Class 이름을 '>' 기호로 분리하지 못한 경우에는 바로 끝낸다.
					if ( AfxExtractSubString(m_rLosstimeList.rLosstime[i].r2ndClass[j].r3rdClass[k].sReason[n], sLine, (3 + n), DELIMITER_DEFECT) == FALSE )	break;
				}//of for n				
			}//of for k
		}//of for j
	}//of for i

	return TRUE;
}

CString CLossTimeLog::LoadTPItem(CString strCode)
{
	CString sFilename;
	sFilename.Format(_T("%s\\%s"), theConfigBank.m_System.m_strProgramDataPath, NAME_FILE_LOSSTIME_LIST);
	CIni ini(sFilename);

	return ini.GetString(_T("ShortCut"), strCode);
}

void CLossTimeLog::LoadTPNameToLosstimeRecode(CString strCode)
{
	CString strTPItem = LoadTPItem(strCode);
	::AfxExtractSubString( m_rLosstimeRecord.s1stClass, strTPItem, 0, DELIMITER_DEFECT);			// 1st class
	::AfxExtractSubString( m_rLosstimeRecord.MesCode, strTPItem, 1, DELIMITER_DEFECT);			// defaultCellInfomation.MesCode
	::AfxExtractSubString( m_rLosstimeRecord.s2ndClass, strTPItem, 2, DELIMITER_DEFECT);			// 2nd class
	::AfxExtractSubString( m_rLosstimeRecord.s3rdClass, strTPItem, 4, DELIMITER_DEFECT);			// 3rd class
	::AfxExtractSubString( m_rLosstimeRecord.sTPSystemName, strTPItem, 4, DELIMITER_DEFECT);	// 3rd class 영문 = TPSystem 설명

	SaveLastLosstimeItem(FALSE);
}

CString CLossTimeLog::LoadTPName(CString strCode)
{
	CString strTPItem = LoadTPItem(strCode);
	CString strTPName;
	::AfxExtractSubString( strTPName, strTPItem, 4, DELIMITER_DEFECT);	// 3rd class 영문 = TPSystem 설명

	return strTPName;
}

CString CLossTimeLog::GetTitle()
{
	CString strtitle = _T("EQP ID,MODEL,ID,ALARM TYPE,START TIME,RESET TIME,END TIME,WAIT TIME,REPAIR TIME,DURATION,FIRST CLASS,SECOND CLASS,THIRD CLASS, TP SYSTEM LOSS NAME,TP SYSTEM LOSS CODE,ALARM CODE,REPAIR CODE\n");
	return strtitle;
}

void CLossTimeLog::SetMachineStopTime()
{
	// 현재 시간을 설비 정지 시작 시간으로 입력
	m_StartTime.SyncSystemTime();
	SaveMachineStopTime();
}

void CLossTimeLog::SetMachineResetTime()
{
	// 현재 시간을 설비 알람 리셋 시간으로 입력
	m_ResetTime.SyncSystemTime();
// 	
// 	m_strValue[ITEM_RESET_TIME].Format(_T("%04d-%02d-%02d %02d:%02d:%02d:%03d"),
// 		m_ResetTime.GetYear(), m_ResetTime.GetMonth(), m_ResetTime.GetDay(),
// 		m_ResetTime.GetHour(), m_ResetTime.GetMinute(), m_ResetTime.GetSecond(), m_ResetTime.GetMillisecond());

	SaveMachineResetTime();
}

void CLossTimeLog::SetMachineStartTime()
{
	// 현재 시간을 설비 가동 시작 시간으로 입력
	m_EndTime.SyncSystemTime();
	m_listZoneFinish.RemoveAll();
}

CSystemTime CLossTimeLog::GetMachineStopTime()
{
	return m_StartTime;
}

void CLossTimeLog::SaveMachineStopTime()
{
	CString strPath;
	strPath.Format(_T("%s\\LastLossTimeItem.ini"), theConfigBank.m_System.m_strConfigPath);
	CIni ini(strPath);
	ini.WriteLong(_T("MACHINE STOP TIME"), _T("YEAR"), m_StartTime.GetYear());
	ini.WriteLong(_T("MACHINE STOP TIME"), _T("MONTH"), m_StartTime.GetMonth());
	ini.WriteLong(_T("MACHINE STOP TIME"), _T("DAY"), m_StartTime.GetDay());
	ini.WriteLong(_T("MACHINE STOP TIME"), _T("HOUR"), m_StartTime.GetHour());
	ini.WriteLong(_T("MACHINE STOP TIME"), _T("MIN"), m_StartTime.GetMinute());
	ini.WriteLong(_T("MACHINE STOP TIME"), _T("SEC"), m_StartTime.GetSecond());
	ini.WriteLong(_T("MACHINE STOP TIME"), _T("MSEC"), m_StartTime.GetMillisecond());
}

void CLossTimeLog::SaveMachineResetTime()
{
	CString strPath;
	strPath.Format(_T("%s\\LastLossTimeItem.ini"), theConfigBank.m_System.m_strConfigPath);
	CIni ini(strPath);
	ini.WriteLong(_T("MACHINE RESET TIME"), _T("YEAR"), m_ResetTime.GetYear());
	ini.WriteLong(_T("MACHINE RESET TIME"), _T("MONTH"), m_ResetTime.GetMonth());
	ini.WriteLong(_T("MACHINE RESET TIME"), _T("DAY"), m_ResetTime.GetDay());
	ini.WriteLong(_T("MACHINE RESET TIME"), _T("HOUR"), m_ResetTime.GetHour());
	ini.WriteLong(_T("MACHINE RESET TIME"), _T("MIN"), m_ResetTime.GetMinute());
	ini.WriteLong(_T("MACHINE RESET TIME"), _T("SEC"), m_ResetTime.GetSecond());
	ini.WriteLong(_T("MACHINE RESET TIME"), _T("MSEC"), m_ResetTime.GetMillisecond());
}

void CLossTimeLog::SaveLastLosstimeItem(BOOL bWirteAlarmCode)
{
	CString strPath;
	strPath.Format(_T("%s\\LastLossTimeItem.ini"), theConfigBank.m_System.m_strConfigPath);
	CIni ini(strPath);
	ini.WriteString(_T("LOSSTIME ITEM"), _T("TP CODE"), m_rLosstimeRecord.MesCode);
	if(bWirteAlarmCode)
	{
		ini.WriteString(_T("LOSSTIME ITEM"), _T("2ND CLASS"), m_rLosstimeRecord.s2ndClass);
		ini.WriteString(_T("LOSSTIME ITEM"), _T("3RD CLASS"), m_rLosstimeRecord.s3rdClass);
		ini.WriteString(_T("LOSSTIME ITEM"), _T("TP NAME"), m_rLosstimeRecord.sTPSystemName);
		ini.WriteString(_T("LOSSTIME ITEM"), _T("ALARM CODE"), m_rLosstimeRecord.sAlarmCode);
	}
}

void CLossTimeLog::ReadLastLossTimeItem()
{
	CString strPath;
	strPath.Format(_T("%s\\LastLossTimeItem.ini"), theConfigBank.m_System.m_strConfigPath);
	CIni ini(strPath);

	// 파일 없을 경우 현재시간으로 기본값
	m_StartTime.SyncSystemTime();
	int nYear = ini.GetLong(_T("MACHINE STOP TIME"), _T("YEAR"), m_StartTime.GetYear());
	int nMonth = ini.GetLong(_T("MACHINE STOP TIME"), _T("MONTH"), m_StartTime.GetMonth());
	int nDay = ini.GetLong(_T("MACHINE STOP TIME"), _T("DAY"), m_StartTime.GetDay());
	int nHour = ini.GetLong(_T("MACHINE STOP TIME"), _T("HOUR"), m_StartTime.GetHour());
	int nMin = ini.GetLong(_T("MACHINE STOP TIME"), _T("MIN"), m_StartTime.GetMinute());
	int nSec = ini.GetLong(_T("MACHINE STOP TIME"), _T("SEC"), m_StartTime.GetSecond());
	int nMsec = ini.GetLong(_T("MACHINE STOP TIME"), _T("MSEC"), m_StartTime.GetMillisecond());
	m_StartTime = CSystemTime(nYear, nMonth, nDay, nHour, nMin, nSec, nMsec);

	m_ResetTime.SyncSystemTime();
	nYear = ini.GetLong(_T("MACHINE RESET TIME"), _T("YEAR"), m_ResetTime.GetYear());
	nMonth = ini.GetLong(_T("MACHINE RESET TIME"), _T("MONTH"), m_ResetTime.GetMonth());
	nDay = ini.GetLong(_T("MACHINE RESET TIME"), _T("DAY"), m_ResetTime.GetDay());
	nHour = ini.GetLong(_T("MACHINE RESET TIME"), _T("HOUR"), m_ResetTime.GetHour());
	nMin = ini.GetLong(_T("MACHINE RESET TIME"), _T("MIN"), m_ResetTime.GetMinute());
	nSec = ini.GetLong(_T("MACHINE RESET TIME"), _T("SEC"), m_ResetTime.GetSecond());
	nMsec = ini.GetLong(_T("MACHINE RESET TIME"), _T("MSEC"), m_ResetTime.GetMillisecond());
	m_ResetTime = CSystemTime(nYear, nMonth, nDay, nHour, nMin, nSec, nMsec);

	LoadTPNameToLosstimeRecode(ini.GetString(_T("LOSSTIME ITEM"), _T("TP CODE"), TP_CODE_MATERIAL_DOWN_MATERIAL));

	// Alarm 발생한 거라면 TP CODE만으로는 모르니 Alarm 코드도 불러온다  [7/10/2017 OSC]
	if(m_rLosstimeRecord.MesCode == TP_CODE_BM_BREAK_DOWN)
	{
		m_rLosstimeRecord.s2ndClass = ini.GetString(_T("LOSSTIME ITEM"), _T("2ND CLASS"), m_rLosstimeRecord.s2ndClass);
		m_rLosstimeRecord.s3rdClass = ini.GetString(_T("LOSSTIME ITEM"), _T("3RD CLASS"), m_rLosstimeRecord.s3rdClass);
		m_rLosstimeRecord.sTPSystemName = ini.GetString(_T("LOSSTIME ITEM"), _T("TP NAME"), m_rLosstimeRecord.sTPSystemName);
// 		m_rLosstimeRecord.sdefaultCellInfomation.MesCode = ini.GetString(_T("LOSSTIME ITEM"), _T("TP CODE"), m_rLosstimeRecord.sdefaultCellInfomation.MesCode);
		m_rLosstimeRecord.sAlarmCode = ini.GetString(_T("LOSSTIME ITEM"), _T("ALARM CODE"), m_rLosstimeRecord.sAlarmCode);
	}
}

void CLossTimeLog::ResetStringArray()
{
	// String 배열을 초기화한다
	m_strValue[ITEM_ID].Empty();
	m_strValue[ITEM_ALARM_TYPE] = _T("-");
	m_strValue[ITEM_START_TIME].Empty();
	m_strValue[ITEM_RESET_TIME].Empty();
	m_strValue[ITEM_END_TIME].Empty();
	m_strValue[ITEM_WAIT_TIME] = _T("0");
	m_strValue[ITEM_REPAIR_TIME] = _T("0");
	m_strValue[ITEM_DURATION] = _T("0");
	m_strValue[ITEM_FIRST_CLASS].Empty();
	m_strValue[ITEM_SECOND_CLASS].Empty();
	m_strValue[ITEM_THIRD_CLASS].Empty();
	m_strValue[ITEM_REPAIR_CODE].Empty();
#ifdef TEMP_NOT_USED
	m_strValue[ITEM_TP_LOSS_NAME].Empty();
	m_strValue[ITEM_TP_LOSS_CODE].Empty();
#else
	m_strValue[ITEM_TP_LOSS_NAME] = _T("UNDEFINE");
	m_strValue[ITEM_TP_LOSS_CODE] = _T("UNDEFINE");
#endif
	m_strValue[ITEM_TP_ALARM_CODE].Empty();
	//m_strValue[ITEM_TP_REPAIR_CODE].Empty();
}


void CLossTimeLog::DataToStringArray(BOOL isReset)
{
	// 각 변수에 저장된 Data를 String 배열로 이전

	if(isReset) ResetStringArray();
	BOOL bMDClass = FALSE;

	// 설비 호기 표기
	m_strValue[ITEM_EQUIPID] = theProcBank.m_strEqipID;

	// 현재 진행중인 PANEL MODEL 명
	m_strValue[ITEM_MODEL] = theRecipeBank.m_strRecipeName;

	// 대,중,소분류
	m_strValue[ITEM_FIRST_CLASS] = m_rLosstimeRecord.s1stClass;
	m_strValue[ITEM_SECOND_CLASS] = m_rLosstimeRecord.s2ndClass;
	m_strValue[ITEM_THIRD_CLASS] = m_rLosstimeRecord.s3rdClass;

	if(m_rLosstimeRecord.sAlarmCode.GetLength() > 1)
		bMDClass = TRUE;

	// FIRST CLASS가 MD가 아닐경우 입력(STOP BUTTON을 누르면 사번을 입력하는 창 생성), 삼성직원일 경우, 사번입력, 협력사 직원일 경우 이름입력
	if(bMDClass == FALSE)
		m_strValue[ITEM_ID] = GetMainHandler()->GetCurOperatorUserInspectorData().sID;
	
	// 중알람일 경우 A, 경알람일 경우 B ( FIRST CLASS가 MD일 경우에만 표기, 이외의 경우는 '-' 표기)
	if(bMDClass)
		m_strValue[ITEM_ALARM_TYPE] = _T("A");
	
	// 2013-03-04 12:45:00:000 ( EVENT 발생시각 - STOP BUTTON을 눌렀거나, ERROR가 발생했을때)
	m_strValue[ITEM_START_TIME].Format(_T("%04d-%02d-%02d %02d:%02d:%02d:%03d"),
		m_StartTime.GetYear(), m_StartTime.GetMonth(), m_StartTime.GetDay(),
		m_StartTime.GetHour(), m_StartTime.GetMinute(), m_StartTime.GetSecond(), m_StartTime.GetMillisecond());

	// 2013-03-04 12:27:00:000  (FIRST CLASS가 MD일 경우만 생성, ERROR MESSAGE를 RESET했을때)
	if(bMDClass)
	{
		m_strValue[ITEM_RESET_TIME].Format(_T("%04d-%02d-%02d %02d:%02d:%02d:%03d"),
			m_ResetTime.GetYear(), m_ResetTime.GetMonth(), m_ResetTime.GetDay(),
			m_ResetTime.GetHour(), m_ResetTime.GetMinute(), m_ResetTime.GetSecond(), m_ResetTime.GetMillisecond());
	}
	else
	{
		m_strValue[ITEM_RESET_TIME] = _T("0");
	}

	// 2013-03-04 12:27:00:000  (조치가 완료되고 다시 START BUTTON을 누른시각)
	m_strValue[ITEM_END_TIME].Format(_T("%04d-%02d-%02d %02d:%02d:%02d:%03d"),
		m_EndTime.GetYear(), m_EndTime.GetMonth(), m_EndTime.GetDay(),
		m_EndTime.GetHour(), m_EndTime.GetMinute(), m_EndTime.GetSecond(), m_EndTime.GetMillisecond());

	// RESET TIME - START TIME, 알람이 울린 후, 고장 조치 시작 까지의 시간, 초 단위로 표기
	if(bMDClass)
	{
		CSystemTimeSpan span = m_ResetTime - m_StartTime;
		m_strValue[ITEM_WAIT_TIME].Format(_T("%.1f"), span.GetTotalSeconds());
	}
	else
	{
		m_strValue[ITEM_WAIT_TIME] = _T("0");
	}

	// END TIME - RESET TIME, 실제 조치가 일어난 시간, 초단위로 표기
	if(bMDClass)
	{
		CSystemTimeSpan span = m_EndTime - m_ResetTime;
		m_strValue[ITEM_REPAIR_TIME].Format(_T("%.1f"), span.GetTotalSeconds());
	}
	else
	{
		m_strValue[ITEM_REPAIR_TIME] = _T("0");
	}

	// 실제설비가 멈춘 시간 (END TIME - START TIME)
	CSystemTimeSpan span = m_EndTime - m_StartTime;
	m_strValue[ITEM_DURATION].Format(_T("%.1f"), span.GetTotalSeconds());

	m_strValue[ITEM_TP_LOSS_NAME] = m_rLosstimeRecord.sTPSystemName;
	m_strValue[ITEM_TP_LOSS_CODE] = m_rLosstimeRecord.MesCode;

	m_strValue[ITEM_TP_ALARM_CODE] = m_rLosstimeRecord.sAlarmCode;
}

CString CLossTimeLog::GetLogValue()
{
	// String 배열에 있는 값들을 로그 기록 위해 한줄로 통합하여 return
	CString strBuff;
	strBuff.ReleaseBuffer();
	TCHAR * pBuf = strBuff.GetBuffer(MAX_LOSSTIME_LOG_STRING);
	memset(pBuf, NULL, MAX_LOSSTIME_LOG_STRING);
	int nWriteStart = 0;
	int nLength;
// 
// 	if(m_strValue[ITEM_FIRST_CLASS] == _T("MD"))
// 	{
// 		CSystemTimeSpan span = m_EndTime - m_StartTime;
// 		if(span.GetTotalSeconds() > 60)
// 			m_strValue[ITEM_FIRST_CLASS] = _T("BM");
// 	}

	for(int i = 0; i < ITEM_MAX; i++)
	{
		if(i != 0)
		{
			memcpy(pBuf+nWriteStart, _T(",") , 1 * sizeof(TCHAR));
			nWriteStart += 1;
		}

		nLength = m_strValue[i].GetLength();
		if(nWriteStart + nLength > MAX_CELL_LOG_STRING)
			break;
		memcpy(pBuf+nWriteStart, m_strValue[i].GetString() , nLength * sizeof(TCHAR));
		nWriteStart += nLength;
	}

	// 줄 넘기기 [11/16/2016 OSC]
	//memcpy(pBuf+nWriteStart, _T("\n") , 1 * sizeof(TCHAR));
	memcpy(pBuf+nWriteStart, _T("\r\n") , 2 * sizeof(TCHAR));

	// MAX_LOSSTIME_LOG_STRING에서 남은 빈 공간 삭제
	strBuff.ReleaseBuffer();

	return strBuff;
}

BOOL CLossTimeLog::ShowLossTimeDlg(BOOL bCanNotCancel)
{
	SetMachineStartTime();

// 	CDIgLossTime dlgLossTime;			
// 
// 	dlgLossTime.SetLossTime(m_StartTime, m_EndTime, m_EndTime - m_StartTime);
// 	dlgLossTime.m_bAlarmHappend = bAlarm;
// 	int nRet = dlgLossTime.DoModal();
// 	if(nRet != IDOK)
// 		return FALSE;
// 
// 	m_rLosstimeRecord.n1stClass = dlgLossTime.m_nSelect_1st;
// 	m_rLosstimeRecord.n2ndClass = dlgLossTime.m_nSelect_2nd;
// 	m_rLosstimeRecord.n3rdClass = dlgLossTime.m_nSelect_3rd;
// 	m_rLosstimeRecord.s1stClass = dlgLossTime.m_strSelect_1st;
// 	m_rLosstimeRecord.s2ndClass = dlgLossTime.m_strSelect_2nd;
// 	m_rLosstimeRecord.s3rdClass = dlgLossTime.m_strSelect_3rd;
// 	m_rLosstimeRecord.sdefaultCellInfomation.MesCode = dlgLossTime.m_strdefaultCellInfomation.MesCode;
// 	m_rLosstimeRecord.sTPSystemName = dlgLossTime.m_strTPName;

	CDlgTPCodeList dlgTpCodeList;
	dlgTpCodeList.m_strTPCode = m_rLosstimeRecord.MesCode;
	dlgTpCodeList.SetLossTime(m_StartTime, m_EndTime, m_EndTime - m_StartTime);
	dlgTpCodeList.m_bCanNotCancel = bCanNotCancel;
	int nRet = dlgTpCodeList.DoModal();
	if(nRet != IDOK)
		return FALSE;

	LoadTPNameToLosstimeRecode(dlgTpCodeList.m_strTPCode);

	SetMachineStartTime();

	DataToStringArray();
	//WriteLossTimeLog();

	return TRUE;
}

void CLossTimeLog::SetAlarmLossTime(int nAlarmId, CString strAlarmText)
{
	m_rLosstimeRecord.Clear();
	ResetStringArray();
	SetMachineStopTime();

	LoadTPNameToLosstimeRecode(TP_CODE_BM_BREAK_DOWN);

	m_rLosstimeRecord.s2ndClass = theProcBank.GetLossTimeAlarmName();
	m_rLosstimeRecord.s3rdClass = strAlarmText;
	m_rLosstimeRecord.sTPSystemName = strAlarmText;
	m_rLosstimeRecord.sAlarmCode = CStringSupport::N2C(nAlarmId);

	SaveLastLosstimeItem(FALSE);
}

BOOL CLossTimeLog::WriteLossTimeLog(BOOL bYesterday)
{
	CTime time = CTime::GetCurrentTime();
	CEtc::ApplyTimeOffset(time, theConfigBank.m_Option.m_OffsetHour, 0);
	if(bYesterday)
		time -= CTimeSpan(1, 0, 0, 0);
	CString strPath;
	strPath.Format(_T("%s\\%04d%02d%02d"), theConfigBank.m_System.m_strLosstimeLogPath, time.GetYear(), time.GetMonth(), time.GetDay());
	if(CFileSupport::DirectoryCheck(strPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strPath) == FALSE)
			return FALSE;
	}

	CString sFullPath;
	sFullPath.Format(_T("%s\\%04d%02d%02d_LOSSTIME_%s.csv"), strPath,
		time.GetYear(), time.GetMonth(), time.GetDay(), theConfigBank.m_System.m_strLineNo );	// MINS에서 사용되는 라인내 설비 ID, 3자리

	char szLogTextbuf[MAX_CELL_LOG_STRING];
	CString strLogText;
	if(_taccess(sFullPath, 0) == -1)	// *csv가 실제 존재하는지 판별하자 (0 : 존재 -1 : 미존재);
	{
		strLogText = GetTitle();
	}

	// 파일명이 바꼈으면 기존에 open해 놓은건 닫고 새로 open한다 [11/16/2016 OSC]
	if( (sFullPath != m_fileLossTime.GetFilePath()) && (m_fileLossTime.GetFilePath().GetLength() > 1) )
	{
		m_fileLossTime.Close();
	}
	if(m_fileLossTime.GetFilePath().GetLength() < 1)
	{
		if(m_fileLossTime.Open(sFullPath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::shareDenyWrite) == FALSE)  
		{
			m_fileLossTime.Abort();

			// 파일이 깨졌을 경우 기존꺼를 이름을 바꾼다
			if(CFileSupport::FileCheck(sFullPath))
			{
				CString strBackupFile;
				strBackupFile.Format(_T("%s\\%04d%02d%02d_LOSSTIME_%s_BAK_%02d%02d%02d.csv"), strPath,
					time.GetYear(), time.GetMonth(), time.GetDay(), theConfigBank.m_System.m_strLineNo,
					time.GetHour(), time.GetMinute(), time.GetSecond());	// MINS에서 사용되는 라인내 설비 ID, 3자리
				if(CFileSupport::MoveFile(sFullPath, strBackupFile) == FALSE)
				{
					return FALSE;
				}
			}
			else
			{
				return FALSE;
			}

			// 백업 성공했으면 다시 오픈 시도
			if(m_fileLossTime.Open(sFullPath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::shareDenyWrite) == FALSE)  
			{
				m_fileLossTime.Abort();
				return FALSE;
			}
		}
	}

	// Title 기록 [11/16/2016 OSC]
	if(strLogText.GetLength() > 0)
	{
		CEtc::ConvertMultibyte((wchar_t*)strLogText.GetString(), szLogTextbuf, MAX_CELL_LOG_STRING);
		m_fileLossTime.SeekToEnd();
		m_fileLossTime.Write(szLogTextbuf,strlen(szLogTextbuf));
	}

	// Log 기록 [11/16/2016 OSC]
	strLogText = GetLogValue();
	if(strLogText.GetLength() > 0 && m_strValue[ITEM_EQUIPID].GetLength() > 5)
	{
		CEtc::ConvertMultibyte((wchar_t*)strLogText.GetString(), szLogTextbuf, MAX_CELL_LOG_STRING);
		m_fileLossTime.SeekToEnd();
		m_fileLossTime.Write(szLogTextbuf,strlen(szLogTextbuf));
	}

	return TRUE;
}

void CLossTimeLog::ChangeDay()
{
	// 날짜가 바뀌면 24시간 구분하기 위해 나눠적는다 [6/19/2017 OSC]
	CString strOriginCode = m_rLosstimeRecord.MesCode;
	LoadTPNameToLosstimeRecode(strOriginCode);

	// 설비 Start한 시간을 06:59:59:999로 바꾸고 어제 날짜로 적는다
	SetMachineStartTime();
	CSystemTime time = CSystemTime(m_EndTime.GetYear(), m_EndTime.GetMonth(), m_EndTime.GetDay(), theConfigBank.m_Option.m_OffsetHour, 00, 00, 000);
	m_EndTime = time - CTimeSpan(0, 0, 0, 1);
	DataToStringArray();
	WriteLossTimeLog(TRUE);
	ResetStringArray();

	// 설비 Sop한 시간을 07:00:00:000으로 바꾼다  [6/19/2017 OSC]
	LoadTPNameToLosstimeRecode(strOriginCode);
	m_StartTime = time;
	SaveMachineStopTime();
}

void CLossTimeLog::WriteLossTime_IDLE()
{
	m_rLosstimeRecord.Clear();
	ResetStringArray();
	SetMachineStartTime();
	LoadTPNameToLosstimeRecode(TP_CODE_IDLE_RUNDOWN_RUNDOWN);
	DataToStringArray();
	WriteLossTimeLog(FALSE);
	SetMachineStopTime();
}

void CLossTimeLog::AddZoneFinish(ZONE_ID zone)
{
	m_listZoneFinish.AddTail(zone);
}

void CLossTimeLog::SetLostimeElement(int nIdx, CString strDescription)
{
	m_strValue[nIdx].Format(_T("%s"),strDescription);
}

CString CLossTimeLog::GetLostimeElement(int nIdx)
{
	return m_strValue[nIdx];
}