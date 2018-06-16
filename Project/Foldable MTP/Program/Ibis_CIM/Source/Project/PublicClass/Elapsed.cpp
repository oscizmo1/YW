#include "StdAfx.h"
#include "Elapsed.h"

CElapsed::CElapsed(void)
{
	Initialized = QueryPerformanceFrequency( (LARGE_INTEGER *)&Frequency );
}

CElapsed::~CElapsed(void)
{
}
