#pragma once
#include "InspModule_Base.h"

class CInspModule_MTPWrite : 	public CInspModule_Base
{
public:
	CInspModule_MTPWrite(void);
	~CInspModule_MTPWrite(void);

public:

	enum
	{
		stepStart = 0,
		//kjpark 20170925 MTP WRITE 할때 X축 동작 
// 		stepZ_Down,
		stepZ_DownChk,
		stepMtpWrite_EndChk,
// 		stepMtpVerify_EndChk,
		stepEnd
	};
	int Run();
};