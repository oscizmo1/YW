#include "StdAfx.h"
#include "StopWatch.h"

// timeGetTime()
#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")

CStopWatch::CStopWatch(void)
{
	m_StopCount = 0;
	Start();
}

CStopWatch::~CStopWatch(void)
{
}

void CStopWatch::Start()
{
	m_StartCount = timeGetTime();
}

double CStopWatch::Stop( bool StartAgain )
{
	m_StopCount = timeGetTime();
	double theElapsedTime = ElapsedTime();
	if(StartAgain)
		m_StartCount = m_StopCount; 
	return theElapsedTime;
}

double CStopWatch::ElapsedTime()
{
	if(m_StartCount > m_StopCount)
		return 0.;
	return ( (double)(m_StopCount - m_StartCount) / 1000. );
}