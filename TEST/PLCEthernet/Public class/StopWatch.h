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
	// zero-point for time measurment
	LARGE_INTEGER m_StartTime;

	// last time stamp
	LARGE_INTEGER m_StopTime;
};
