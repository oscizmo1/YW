#pragma once
#include "InspModule_Base.h"

class CInspModule_CoprIctTest : 	public CInspModule_Base
{
public:
	CInspModule_CoprIctTest(void);
	~CInspModule_CoprIctTest(void);

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