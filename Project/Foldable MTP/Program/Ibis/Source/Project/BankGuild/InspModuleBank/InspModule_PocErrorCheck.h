#pragma once
#include "InspModule_Base.h"

class CInspModule_PocErrorCheck : 	public CInspModule_Base
{
public:
	CInspModule_PocErrorCheck(void);
	~CInspModule_PocErrorCheck(void);

public:

	enum
	{
		stepStart = 0,
		stepEnd
	};
	int Run();
};