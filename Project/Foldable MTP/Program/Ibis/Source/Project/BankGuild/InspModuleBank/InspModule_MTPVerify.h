#pragma once
#include "InspModule_Base.h"

class CInspModule_MTPVerify : 	public CInspModule_Base
{
public:
	CInspModule_MTPVerify(void);
	~CInspModule_MTPVerify(void);

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