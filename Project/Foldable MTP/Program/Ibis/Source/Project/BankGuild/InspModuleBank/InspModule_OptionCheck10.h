#pragma once
#include "InspModule_Base.h"

class CInspModule_OptionCheck10 : 	public CInspModule_Base
{
public:
	CInspModule_OptionCheck10(void);
	~CInspModule_OptionCheck10(void);

public:

	enum
	{
		stepStart = 0,
		stepEnd
	};
	int Run();
};