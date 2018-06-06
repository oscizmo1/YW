#pragma once

#include "ThreadBank.h"

class CThread_IO_Sequence
{
public:
	CThread_IO_Sequence(void);
	~CThread_IO_Sequence(void);

	//kjpark 20161019 IO Check Thread Ãß°¡
	static UINT IOSequenceThr(LPVOID lpParam);

public:
	struct PATTERN_SWITCH
	{
		INPUT_ID	iSwInput;
		OUTPUT_ID	oSwLamp;
		BOOL		bPressed;
		BOOL		bRequireAction;

		PATTERN_SWITCH ()
		{
			iSwInput = MAX_IN_ID;
			oSwLamp = MAX_OUT_ID;
			bPressed = FALSE;
			bRequireAction = FALSE;
		}
	};

	struct VACUUM_SWITCH
	{
		INPUT_ID	iSwInputOn, iSwInputOff;
		OUTPUT_ID	oSwLampOn, oSwLampOff;
		BOOL		bPressed;
		BOOL		bRequireActionOn, bRequireActionOff;

		VACUUM_SWITCH ()
		{
			iSwInputOn = iSwInputOff = MAX_IN_ID;
			oSwLampOn = oSwLampOff = MAX_OUT_ID;
			bPressed = FALSE;
			bRequireActionOn = bRequireActionOff = FALSE;
		}
	};

	struct READY_SWITCH
	{
		INPUT_ID	iLeftSw, iRightSw;
		CStopWatch timerPress;
		BOOL bPressStart;
		BOOL bPressEnd;

		READY_SWITCH ()
		{
			iLeftSw = iRightSw = MAX_IN_ID;
			bPressStart = FALSE;
			bPressEnd = TRUE;
		}
	};

public:
	PATTERN_SWITCH swBack[JIG_ID_MAX];
	PATTERN_SWITCH swNext[JIG_ID_MAX];
	PATTERN_SWITCH swOnOff[JIG_ID_MAX];
	VACUUM_SWITCH swVacuum[JIG_ID_MAX];
	READY_SWITCH swReady[JIG_ID_MAX];
	BOOL bOldVacuumOn[JIG_ID_MAX];

	void Switch_IOInit();
	void Switch_Check(CUnitCtrlFunc &_func, JIG_ID jig);
};

