#pragma once
#include "InspModule_Base.h"

class CInspModule_OptionCheck7 : 	public CInspModule_Base
{
public:
	CInspModule_OptionCheck7(void);
	~CInspModule_OptionCheck7(void);

public:

	enum
	{
		stepStart = 0,
		stepEnd
	};
	int Run();
};