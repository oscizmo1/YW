#pragma once

/********************************************************************
	created:	2014/10/02
	modefied:	07:10:2014   09:54
	author:		OSC
	
	purpose:	msec���� �����ϴ� �ð� Class
				SyncSystemTime() ȣ��� ǥ�ؽð��븦 �����ص� ���� 
					���󰡵��� SYSTEMTIME���� ����ð��� �����´�.
*********************************************************************/
class CSystemTimeSpan;

class CSystemTime
{
public:
	CSystemTime(void);
	CSystemTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, WORD wMsec);
	~CSystemTime(void);

private:
	CTime m_Time;
	WORD m_wMilliseconds;

public:
	int GetYear();
	int GetMonth();
	int GetDay();
	int GetHour();
	int GetMinute();
	int GetSecond();
	WORD GetMillisecond();

	CString Format(LPCTSTR pFormat);

	void SyncSystemTime();
	CTime ToCTime();
	static CTime GetSystemTime();

	void operator=(const CTime &src);
	void operator=(const SYSTEMTIME &src);

	CSystemTimeSpan operator-(const CSystemTime& src) const;
	CSystemTime operator-(const CSystemTimeSpan& src) const;
	CSystemTime operator-(const CTimeSpan& src) const;

	void operator+=(const CTimeSpan& src);
	void operator-=(const CTimeSpan& src);

};


class CSystemTimeSpan
{
public:
	CSystemTimeSpan(void);
	CSystemTimeSpan(long nDay, int nHour, int nMinute, int nSecond, int nMillisecond);
	CSystemTimeSpan(long nDay, int nHour, int nMinute, double dSecond);
	~CSystemTimeSpan(void);

	CTimeSpan m_TimeSpan;
	WORD m_wMilliseconds;

	
public:
	int GetHours();
	int GetMinutes();
	double GetSeconds();
	double GetTotalSeconds();

};

