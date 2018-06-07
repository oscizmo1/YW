#pragma once
#include "InspModule_Base.h"

class CInspModule_OptionCheck5 : 	public CInspModule_Base
{
public:
	CInspModule_OptionCheck5(void);
	~CInspModule_OptionCheck5(void);

public:

	enum
	{
		stepStart = 0,
		stepEnd
	};
	int Run();
};