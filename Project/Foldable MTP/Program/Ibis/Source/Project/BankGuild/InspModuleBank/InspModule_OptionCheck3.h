#pragma once
#include "InspModule_Base.h"

class CInspModule_OptionCheck3 : 	public CInspModule_Base
{
public:
	CInspModule_OptionCheck3(void);
	~CInspModule_OptionCheck3(void);

public:

	enum
	{
		stepStart = 0,
		stepEnd
	};
	int Run();
};