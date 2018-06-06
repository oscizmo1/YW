#include "StdAfx.h"
#include "SystemTime.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CSystemTime::CSystemTime(void)
{
	m_wMilliseconds = 0;
}


CSystemTime::CSystemTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, WORD wMsec)
{
	m_Time = CTime(nYear, nMonth, nDay, nHour, nMin, nSec);
	m_wMilliseconds = wMsec;
}

CSystemTime::~CSystemTime(void)
{
}

int CSystemTime::GetYear()
{
	return m_Time.GetYear();
}

int CSystemTime::GetMonth()
{
	return m_Time.GetMonth();
}

int CSystemTime::GetDay()
{
	return m_Time.GetDay();
}

int CSystemTime::GetHour()
{
	return m_Time.GetHour();
}

int CSystemTime::GetMinute()
{
	return m_Time.GetMinute();
}

int CSystemTime::GetSecond()
{
	return m_Time.GetSecond();
}

WORD CSystemTime::GetMillisecond()
{
	return m_wMilliseconds;
}

CString CSystemTime::Format(LPCTSTR pFormat)
{
	return m_Time.Format(pFormat);
}

void CSystemTime::SyncSystemTime()
{
	SYSTEMTIME time;
	::GetLocalTime(&time);
	m_Time = CTime(time);
	m_wMilliseconds = time.wMilliseconds;
}

CTime CSystemTime::ToCTime()
{
	return m_Time;
}

CTime CSystemTime::GetSystemTime()
{
	CSystemTime time;
	time.SyncSystemTime();
	return time.ToCTime();
}

void CSystemTime::operator=( const CTime &src )
{
	m_Time = src;
	m_wMilliseconds = 0;
}

void CSystemTime::operator=( const SYSTEMTIME &src )
{
	m_Time = CTime(src);
	m_wMilliseconds = src.wMilliseconds;
}

void CSystemTime::operator+=( const CTimeSpan& src )
{
	m_Time += src;
}

void CSystemTime::operator-=( const CTimeSpan& src )
{
	m_Time -= src;
}

CSystemTimeSpan CSystemTime::operator-(const CSystemTime& src) const
{
	CSystemTimeSpan span;
	span.m_TimeSpan = m_Time - src.m_Time;
	span.m_wMilliseconds = m_wMilliseconds;
	if(span.m_wMilliseconds < src.m_wMilliseconds)
	{
		span.m_TimeSpan -= CTimeSpan(0, 0, 0, 1);
		span.m_wMilliseconds += 1000;

	}
	span.m_wMilliseconds -= src.m_wMilliseconds;

	return span;
}

CSystemTime CSystemTime::operator-(const CSystemTimeSpan& src) const
{
	CSystemTime time;
	time.m_Time = m_Time - src.m_TimeSpan;
	WORD wMilliseconds = m_wMilliseconds;
	if(wMilliseconds < src.m_wMilliseconds)
	{
		time.m_Time -= CTimeSpan(0, 0, 0, 1);
		wMilliseconds += 1000;
	}
	time.m_wMilliseconds = wMilliseconds - src.m_wMilliseconds;

	return time;
}

CSystemTime CSystemTime::operator-(const CTimeSpan& src) const
{
	CSystemTime time;
	time.m_Time = m_Time - src;
	time.m_wMilliseconds = m_wMilliseconds;

	return time;
}

CSystemTimeSpan::CSystemTimeSpan(void)
{

}

CSystemTimeSpan::CSystemTimeSpan(long nDay, int nHour, int nMinute, int nSecond, int nMillisecond)
{
	m_TimeSpan = CTimeSpan(nDay, nHour, nMinute, nSecond);
	m_wMilliseconds = nMillisecond;
}

CSystemTimeSpan::CSystemTimeSpan(long nDay, int nHour, int nMinute, double dSecond)
{
	int nSec = (int)dSecond;
	m_wMilliseconds = (int)(dSecond*1000.)%1000;
	m_TimeSpan = CTimeSpan(nDay, nHour, nMinute, nSec);
}

CSystemTimeSpan::~CSystemTimeSpan(void)
{

}

int CSystemTimeSpan::GetHours()
{
	return m_TimeSpan.GetHours();
}

int CSystemTimeSpan::GetMinutes()
{
	return m_TimeSpan.GetMinutes();
}

double CSystemTimeSpan::GetSeconds()
{
	return (double)m_TimeSpan.GetSeconds() + ((double)m_wMilliseconds/1000.);
}

double CSystemTimeSpan::GetTotalSeconds()
{
	return (double)m_TimeSpan.GetTotalSeconds() + ((double)m_wMilliseconds/1000.);
}
