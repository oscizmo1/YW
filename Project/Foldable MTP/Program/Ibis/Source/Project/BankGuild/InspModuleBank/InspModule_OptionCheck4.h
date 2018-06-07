#pragma once
#include "InspModule_Base.h"

class CInspModule_OptionCheck4 : 	public CInspModule_Base
{
public:
	CInspModule_OptionCheck4(void);
	~CInspModule_OptionCheck4(void);

public:

	enum
	{
		stepStart = 0,
		stepEnd
	};
	int Run();
};