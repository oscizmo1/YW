#pragma once
#include "InspModule_Base.h"

class CInspModule_TspStart : 	public CInspModule_Base
{
public:
	CInspModule_TspStart(void);
	~CInspModule_TspStart(void);

public:

	enum
	{
		stepStart = 0,
		stepEnd
	};
	int Run();
};