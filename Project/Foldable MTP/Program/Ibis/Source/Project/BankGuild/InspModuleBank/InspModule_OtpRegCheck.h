#pragma once
#include "InspModule_Base.h"

class CInspModule_OtpRegCheck : 	public CInspModule_Base
{
public:
	CInspModule_OtpRegCheck(void);
	~CInspModule_OtpRegCheck(void);

public:

	enum
	{
		stepStart = 0,
		stepEnd
	};
	int Run();
};