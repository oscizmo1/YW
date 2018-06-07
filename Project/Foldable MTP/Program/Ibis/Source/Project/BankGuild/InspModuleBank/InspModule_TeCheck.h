#pragma once
#include "InspModule_Base.h"

class CInspModule_TeCheck : 	public CInspModule_Base
{
public:
	CInspModule_TeCheck(void);
	~CInspModule_TeCheck(void);

public:

	enum
	{
		stepStart = 0,
		stepEnd
	};
	int Run();
};