#pragma once

class CStopWatch
{
public:
	CStopWatch(void);
	~CStopWatch(void);

	void Start();
	double Stop(bool StartAgain);
	double ElapsedTime();

protected:
	DWORD m_StartCount;
	DWORD m_StopCount;
};
