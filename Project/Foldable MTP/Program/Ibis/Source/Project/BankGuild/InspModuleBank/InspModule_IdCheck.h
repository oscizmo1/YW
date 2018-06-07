#pragma once
#include "InspModule_Base.h"

class CInspModule_IdCheck : 	public CInspModule_Base
{
public:
	CInspModule_IdCheck(void);
	~CInspModule_IdCheck(void);

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