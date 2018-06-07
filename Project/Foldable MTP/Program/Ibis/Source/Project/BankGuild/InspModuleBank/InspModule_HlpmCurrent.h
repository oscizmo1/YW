#pragma once
#include "InspModule_Base.h"

class CInspModule_HlpmCurrent : 	public CInspModule_Base
{
public:
	CInspModule_HlpmCurrent(void);
	~CInspModule_HlpmCurrent(void);

public:

	enum
	{
		stepStart = 0,
		stepEnd
	};
	int Run();
};