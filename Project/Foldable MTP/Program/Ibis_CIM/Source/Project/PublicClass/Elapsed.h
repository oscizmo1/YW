#pragma once

class CElapsed
{
public:
	CElapsed(void);
	~CElapsed(void);

private :    
	int Initialized;      
	__int64 Frequency;
	__int64 BeginTime;

public :    
	BOOL Begin()    
	{     
		if( ! Initialized )
			return 0;

		return QueryPerformanceCounter( (LARGE_INTEGER *)&BeginTime );      
	}

	double End()     
	{
		if( ! Initialized )        
			return 0.0;

		__int64 endtime;
		QueryPerformanceCounter( (LARGE_INTEGER *)&endtime );

		__int64 elapsed = endtime - BeginTime;
		
		return (double)elapsed / (double)Frequency;      
	}

	BOOL Available()
	{return Initialized; }
	__int64 GetFreq()
	{ return Frequency; }  


};
