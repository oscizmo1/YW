#pragma once
#include "InspModule_Base.h"

class CInspModule_OptionCheck2 : 	public CInspModule_Base
{
public:
	CInspModule_OptionCheck2(void);
	~CInspModule_OptionCheck2(void);

public:

	enum
	{
		stepStart = 0,
		stepEnd
	};
	int Run();
};