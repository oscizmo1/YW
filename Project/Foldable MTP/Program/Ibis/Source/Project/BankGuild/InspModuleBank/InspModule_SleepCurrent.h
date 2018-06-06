#pragma once
#include "InspModule_Base.h"

class CInspModule_SleepCurrent : 	public CInspModule_Base
{
public:
	CInspModule_SleepCurrent(void);
	~CInspModule_SleepCurrent(void);

public:

	enum
	{
		stepStart = 0,
		stepEnd
	};
	int Run();
};