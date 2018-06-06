#pragma once
#include "InspModule_Base.h"

class CInspModule_OptionCheck6 : 	public CInspModule_Base
{
public:
	CInspModule_OptionCheck6(void);
	~CInspModule_OptionCheck6(void);

public:

	enum
	{
		stepStart = 0,
		stepEnd
	};
	int Run();
};