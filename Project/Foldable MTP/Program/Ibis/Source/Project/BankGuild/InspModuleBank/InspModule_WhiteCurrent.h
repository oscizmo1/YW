#pragma once
#include "InspModule_Base.h"

class CInspModule_WhiteCurrent : 	public CInspModule_Base
{
public:
	CInspModule_WhiteCurrent(void);
	~CInspModule_WhiteCurrent(void);

public:

	enum
	{
		stepStart = 0,
		stepEnd
	};
	int Run();
};