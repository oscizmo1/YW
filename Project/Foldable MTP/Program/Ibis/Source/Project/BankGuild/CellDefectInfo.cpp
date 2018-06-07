#include "StdAfx.h"
#include "CellDefectInfo.h"
#include "BankGuild/InspModuleBank/InspModuleBank.h"


CCellTactTime::CCellTactTime(void)
{
	Init();
}

CCellTactTime::~CCellTactTime(void)
{

}

void CCellTactTime::SetTimeStart(SYSTEMTIME time /*= SYSTEMTIME()*/)
{
	// 시간 덮어쓰기 방지
	if(m_timeStart.wYear == 0)
	{
		if(time.wYear == 0)		GetLocalTime(&m_timeStart);
		else					m_timeStart = time;
	}
}

void CCellTactTime::SetTimeEnd(SYSTEMTIME time /*= SYSTEMTIME()*/)
{
	// 시간 덮어쓰기 방지
	if(m_timeEnd.wYear == 0)
	{
		if(time.wYear == 0)		GetLocalTime(&m_timeEnd);
		else					m_timeEnd = time;
	}
}

void CCellTactTime::Init()
{
	m_timeStart = SYSTEMTIME();
	m_timeEnd = SYSTEMTIME();
}

CCellDefectInfo::CCellDefectInfo(void)
	: CCellTactTime()
{
	Init();
}


CCellDefectInfo::~CCellDefectInfo(void)
{
}

void CCellDefectInfo::Init()
{
	CCellTactTime::Init();

	//kjpark 20170907 CellLog Title 및 해당 항목 추가
	m_strCommand.Empty();
	m_strWorkZone.Empty();
	m_Class = NONE_CELL;
	m_strMTPType.Empty();
	m_DefectName.Empty();
	m_strWorkZone = theInspModuleBank.FindWorkZone(m_strWorkZone);
	m_Judge = CONST_JUDGE_NAME::JUDGE_MCR;
}

void CCellDefectInfo::FindWorkZone()
{
	m_strWorkZone = theInspModuleBank.FindWorkZone(m_strCommand);
}

BOOL CCellDefectInfo::IsStarted()
{
	if(m_Class != NONE_CELL)
		return TRUE;
	if(m_timeStart.wYear != 0)
		return TRUE;
	return FALSE;
}

BOOL CCellDefectInfo::IsEnded()
{
	if(m_Class != NONE_CELL)
		return TRUE;
	if(m_timeEnd.wYear != 0)
		return TRUE;
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCellDefectInfo_Current::CCellDefectInfo_Current( void )
	: CCellDefectInfo()
{
	Init();
}

CCellDefectInfo_Current::~CCellDefectInfo_Current( void )
{

}

void CCellDefectInfo_Current::Text_to_Value()
{
	for (int i=IVSS ; i<DEFECTINFO_CURRENT_MAX ; i++)
	{
		m_CurrentValue[i] = _tstof(m_CurrentText[i].GetString());		
	}
}

void CCellDefectInfo_Current::Init()
{
	CCellDefectInfo::Init();

	for(int i = 0; i < DEFECTINFO_CURRENT_MAX; i++)
	{
		m_CurrentText[i].Empty();
		m_CurrentValue[i] = 0.;
	}

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCellDefectInfo_CellLoading::CCellDefectInfo_CellLoading( void )
	: CCellDefectInfo_Current()
{
	Init();
}

CCellDefectInfo_CellLoading::~CCellDefectInfo_CellLoading( void )
{

}

void CCellDefectInfo_CellLoading::Init()
{
	CCellDefectInfo_Current::Init();

	m_CellLoadingSend = FALSE;
	m_ContactCount = 0;
	m_FirstClass = NONE_CELL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCellDefectInfo_Zone::CCellDefectInfo_Zone( void )
{
	Init();
}

CCellDefectInfo_Zone::~CCellDefectInfo_Zone( void )
{

}

void CCellDefectInfo_Zone::SetTimeWait()
{
	if(m_timeWait.wYear == 0)
		GetLocalTime(&m_timeWait);
}

void CCellDefectInfo_Zone::Init()
{
	CCellDefectInfo::Init();
	m_RecvSetZone = FALSE;
	m_timeWait = SYSTEMTIME();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCellDefectInfo_Tsp::CCellDefectInfo_Tsp( void )
{
	Init();
}

CCellDefectInfo_Tsp::~CCellDefectInfo_Tsp( void )
{

}

void CCellDefectInfo_Tsp::Init()
{
	CCellDefectInfo::Init();
	for(int i = 0; i < MAX_TSP_RESULT; i++)
	{
		TspTitle[i].Empty();
		TspName[i].Empty();
	}

}
