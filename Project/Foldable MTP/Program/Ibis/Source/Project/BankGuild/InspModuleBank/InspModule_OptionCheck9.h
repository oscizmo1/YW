#pragma once
#include "InspModule_Base.h"

class CInspModule_OptionCheck9 : 	public CInspModule_Base
{
public:
	CInspModule_OptionCheck9(void);
	~CInspModule_OptionCheck9(void);

public:

	enum
	{
		stepStart = 0,
		stepEnd
	};
	int Run();
};