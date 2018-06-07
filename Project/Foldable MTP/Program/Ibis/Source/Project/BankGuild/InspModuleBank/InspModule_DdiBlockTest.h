#pragma once
#include "InspModule_Base.h"

class CInspModule_DDIBlockTest : 	public CInspModule_Base
{
public:
	CInspModule_DDIBlockTest(void);
	~CInspModule_DDIBlockTest(void);

public:

	enum
	{
		stepStart = 0,
		stepEnd
	};
	int Run();
};