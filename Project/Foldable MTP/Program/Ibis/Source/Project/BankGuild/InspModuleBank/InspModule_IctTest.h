#pragma once
#include "InspModule_Base.h"

class CInspModule_IctTest : 	public CInspModule_Base
{
public:
	CInspModule_IctTest(void);
	~CInspModule_IctTest(void);

public:

	enum
	{
		stepStart = 0,
// 		stepZ_DownChk,
// 		stepMtpWrite_EndChk,
		stepMtpVerify_EndChk,
		stepEnd
	};
	int Run();
};