#pragma once
#include "InspModule_Base.h"

class CInspModule_OptionCheck8 : 	public CInspModule_Base
{
public:
	CInspModule_OptionCheck8(void);
	~CInspModule_OptionCheck8(void);

public:

	enum
	{
		stepStart = 0,
		stepEnd
	};
	int Run();
};