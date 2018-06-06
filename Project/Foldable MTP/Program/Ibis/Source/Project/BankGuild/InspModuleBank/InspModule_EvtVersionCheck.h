#pragma once
#include "InspModule_Base.h"

class CInspModule_EvtVersionCheck : 	public CInspModule_Base
{
public:
	CInspModule_EvtVersionCheck(void);
	~CInspModule_EvtVersionCheck(void);

public:

	enum
	{
		stepStart = 0,
		stepEnd
	};
	int Run();
};