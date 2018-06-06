#pragma once
#include "InspModule_Base.h"

class CInspModule_CellLoading : 	public CInspModule_Base
{
public:
	CInspModule_CellLoading(void);
	~CInspModule_CellLoading(void);

public:

	enum
	{
		stepStart = 0,
		// 1st Recv Check -> if retry -> Reset
		stepRecvCheck,
		// Reset Delay -> Cell Loading
		stepResetDelay,
		stepEnd
	};
	int Run();
};