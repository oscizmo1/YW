#include "StdAfx.h"
#include "Thread_IO_Sequence.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CThread_IO_Sequence::CThread_IO_Sequence(void)
{
}


CThread_IO_Sequence::~CThread_IO_Sequence(void)
{
}

UINT CThread_IO_Sequence::IOSequenceThr( LPVOID lpParam )
{
	// Thread에서는 theUnitFunc는 절대 쓰면 안됨!!! 아래 _func로 쓸것
	CUnitCtrlFunc					_func(MP2100_THREAD_IO_SEQUENCE);

	int nThreadIndex = (int)lpParam;

	CThread_IO_Sequence *pClass = &theThreadBank.m_Thread_IO_Sequence;

	pClass->Switch_IOInit();

	BOOL bPatternSetOn[JIG_ID_MAX]		= {TRUE, TRUE};

	CCellTag tagCell;

	// 위에 변수들과 LAMP 상태를 맞춘다
	_func.SetOutPutIO(Y_SHUTTLE_1_VACCUM_ON_SW_LAMP, OFF);
	_func.SetOutPutIO(Y_SHUTTLE_1_VACCUM_OFF_SW_LAMP, ON);
	_func.SetOutPutIO(Y_SHUTTLE_2_VACCUM_ON_SW_LAMP, OFF);
	_func.SetOutPutIO(Y_SHUTTLE_2_VACCUM_OFF_SW_LAMP, ON);


	while(theThreadInitializer.m_bThrExist[nThreadIndex] == FALSE)
	{
		Sleep(100);

		//kjpark 20161019 MainFrame timer 추가 - 인터락, 타워램프 컨트롤
		if(GetMainHandler() == NULL)
			continue;
		if(theProcBank.m_bProgramInitFinish == FALSE)
			continue;
		if(theProcBank.MachineIsRunState())
		{
			for(int jig = JIG_ID_A; jig < JIG_ID_MAX; jig++)
			{
				if(GetMainHandler()->m_bAZoneOperatorReady[jig])
				{
					pClass->Switch_Check(_func, (JIG_ID)jig);
				}
				else
				{
					if(bPatternSetOn[jig] == FALSE)
						bPatternSetOn[jig] = TRUE;
				}
			}

			for(int jig = 0; jig < JIG_ID_MAX; jig++)
			{
				//////////////////////////////////////////////////////////////////////////
				// Pattern Back State Check		
				if(pClass->swBack[jig].bRequireAction)
				{
					for(int i=JIG_CH_1; i<JIG_CH_MAX; i++)
					{
						if(theProcBank.AZoneCellNG_Check((JIG_ID)jig, (JIG_CH)i) == FALSE)
						{
							tagCell = theCellBank.GetCellTag((JIG_ID)jig, (JIG_CH)i);
							if(tagCell.IsExist())
							{
								_func.SendPGMessage(BACK, (JIG_ID)jig, JIG_CH(i), _T(""), TRUE);
							}
						}
					}

					pClass->swBack[jig].bRequireAction = FALSE;
				}
				//////////////////////////////////////////////////////////////////////////
				// Pattern Next State Check		
				if(pClass->swNext[jig].bRequireAction)
				{
					for(int i=JIG_CH_1; i<JIG_CH_MAX; i++)
					{
						if(theProcBank.AZoneCellNG_Check((JIG_ID)jig, (JIG_CH)i) == FALSE)
						{
							tagCell = theCellBank.GetCellTag((JIG_ID)jig, (JIG_CH)i);
							if(tagCell.IsExist())
							{
								_func.SendPGMessage(NEXT, (JIG_ID)jig, JIG_CH(i), _T(""), TRUE);
							}
						}
					}

					pClass->swNext[jig].bRequireAction = FALSE;
				}
				//////////////////////////////////////////////////////////////////////////
				// Pattern On Button Click
				if(pClass->swOnOff[jig].bRequireAction)
				{
					if(bPatternSetOn[jig])
					{
						if(theProcBank.LoadingStop_IsRequire() == FALSE)
						{
							for(int i=JIG_CH_1; i<JIG_CH_MAX; i++)
							{
								if(theProcBank.AZoneCellNG_Check((JIG_ID)jig, (JIG_CH)i) == FALSE)
								{
									_func.CellLoading_Send((JIG_ID)jig, JIG_CH(i), TRUE);
								}
							}
						}

						bPatternSetOn[jig] = FALSE;
					}
					else
					{
						for(int i=JIG_CH_1; i<JIG_CH_MAX; i++)
						{
							if(theProcBank.AZoneCellNG_Check((JIG_ID)jig, (JIG_CH)i) == FALSE)
							{
								_func.SendPGMessage(RESET, (JIG_ID)jig, JIG_CH(i));
							}
						}

						bPatternSetOn[jig] = TRUE;
					}
					pClass->swOnOff[jig].bRequireAction = FALSE;
				}
				//////////////////////////////////////////////////////////////////////////
				// Vaccum OnOff Button Click
				if(pClass->swVacuum[jig].bRequireActionOn)
				{
					for(int i=JIG_CH_1; i<JIG_CH_MAX; i++)
					{
						if(theProcBank.AZoneChannelNotUse_Check((JIG_ID)jig, (JIG_CH)i) == FALSE)
						{
							if(theCellBank.GetCellTag((JIG_ID)jig, (JIG_CH)i).IsExist())
								_func.Shuttle_Vac_OnOff((JIG_ID)jig, (JIG_CH)i, VAC_ON);
						}
					}
					pClass->swVacuum[jig].bRequireActionOn = FALSE;
				}
				if(pClass->swVacuum[jig].bRequireActionOff)
				{
					for(int i=JIG_CH_1; i<JIG_CH_MAX; i++)
					{
						if(theProcBank.AZoneChannelNotUse_Check((JIG_ID)jig, (JIG_CH)i) == FALSE)
						{
							_func.Shuttle_Vac_OnOff((JIG_ID)jig, (JIG_CH)i, VAC_OFF);
						}
					}
					pClass->swVacuum[jig].bRequireActionOff = FALSE;
				}
				//////////////////////////////////////////////////////////////////////////

			}// End of for(int jig = 0; jig < SHUTTLE_MAX; jig++)

		}// End of if(theProcBank.MachineIsRunState())

	}// End of while(theThreadInitializer.m_bThrExist[nThreadIndex] == FALSE)
	return 0;
}

void CThread_IO_Sequence::Switch_IOInit()
{
	swBack[JIG_ID_A].iSwInput = X_SHUTTLE_1_PATTERN_BACK_SW;
	swBack[JIG_ID_A].oSwLamp = Y_SHUTTLE_1_PATTERN_BACK_SW_LAMP;
	swNext[JIG_ID_A].iSwInput = X_SHUTTLE_1_PATTERN_NEXT_SW;
	swNext[JIG_ID_A].oSwLamp = Y_SHUTTLE_1_PATTERN_NEXT_SW_LAMP;
	swOnOff[JIG_ID_A].iSwInput = X_SHUTTLE_1_PATTERN_ONOFF_SW;
	swOnOff[JIG_ID_A].oSwLamp = Y_SHUTTLE_1_PATTERN_ONOFF_SW_LAMP;
	swVacuum[JIG_ID_A].iSwInputOn = X_SHUTTLE_1_VACCUM_ON_SW;
	swVacuum[JIG_ID_A].iSwInputOff = X_SHUTTLE_1_VACCUM_OFF_SW;
	swVacuum[JIG_ID_A].oSwLampOn = Y_SHUTTLE_1_VACCUM_ON_SW_LAMP;
	swVacuum[JIG_ID_A].oSwLampOff = Y_SHUTTLE_1_VACCUM_OFF_SW_LAMP;
	swReady[JIG_ID_A].iLeftSw = X_SHUTTLE_1_VACCUM_READY_SW;
	swReady[JIG_ID_A].iRightSw = X_SHUTTLE_1_PATTERN_READY_SW;

	swBack[JIG_ID_B].iSwInput = X_SHUTTLE_2_PATTERN_BACK_SW;
	swBack[JIG_ID_B].oSwLamp = Y_SHUTTLE_2_PATTERN_BACK_SW_LAMP;
	swNext[JIG_ID_B].iSwInput = X_SHUTTLE_2_PATTERN_NEXT_SW;
	swNext[JIG_ID_B].oSwLamp = Y_SHUTTLE_2_PATTERN_NEXT_SW_LAMP;
	swOnOff[JIG_ID_B].iSwInput = X_SHUTTLE_2_PATTERN_ONOFF_SW;
	swOnOff[JIG_ID_B].oSwLamp = Y_SHUTTLE_2_PATTERN_ONOFF_SW_LAMP;
	swVacuum[JIG_ID_B].iSwInputOn = X_SHUTTLE_2_VACCUM_ON_SW;
	swVacuum[JIG_ID_B].iSwInputOff = X_SHUTTLE_2_VACCUM_OFF_SW;
	swVacuum[JIG_ID_B].oSwLampOn = Y_SHUTTLE_2_VACCUM_ON_SW_LAMP;
	swVacuum[JIG_ID_B].oSwLampOff = Y_SHUTTLE_2_VACCUM_OFF_SW_LAMP;
	swReady[JIG_ID_B].iLeftSw = X_SHUTTLE_2_VACCUM_READY_SW;
	swReady[JIG_ID_B].iRightSw = X_SHUTTLE_2_PATTERN_READY_SW;

	bOldVacuumOn[JIG_ID_A]		= FALSE;
	bOldVacuumOn[JIG_ID_B]		= FALSE;
}

void CThread_IO_Sequence::Switch_Check( CUnitCtrlFunc &_func, JIG_ID jig )
{
	BOOL bVacuumOn				= FALSE;
	//////////////////////////////////////////////////////////////////////////
	// Pattern Back State Check		
	if(swBack[jig].bPressed == FALSE && swBack[jig].bRequireAction == FALSE)
	{
		if(_func.GetInPutIOCheck(swBack[jig].iSwInput))
		{
			_func.SetOutPutIO(swBack[jig].oSwLamp, ON);
			swBack[jig].bRequireAction = TRUE;
			swBack[jig].bPressed = TRUE;
		}

	}
	else if(swBack[jig].bPressed)
	{
		if(_func.GetInPutIOCheck(swBack[jig].iSwInput) == FALSE)
		{
			swBack[jig].bPressed = FALSE;
			_func.SetOutPutIO(swBack[jig].oSwLamp, OFF);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// Pattern Next State Check		
	if(swNext[jig].bPressed == FALSE && swNext[jig].bRequireAction == FALSE)
	{
		if(_func.GetInPutIOCheck(swNext[jig].iSwInput))
		{
			_func.SetOutPutIO(swNext[jig].oSwLamp, ON);
			swNext[jig].bRequireAction = TRUE;
			swNext[jig].bPressed = TRUE;
		}
	}
	else if(swNext[jig].bPressed)
	{
		if(_func.GetInPutIOCheck(swNext[jig].iSwInput) == FALSE)
		{
			swNext[jig].bPressed = FALSE;
			_func.SetOutPutIO(swNext[jig].oSwLamp, OFF);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// Pattern On Off State Check		
	if(swOnOff[jig].bPressed == FALSE && swOnOff[jig].bRequireAction == FALSE)
	{
		if(_func.GetInPutIOCheck(swOnOff[jig].iSwInput))
		{
			_func.SetOutPutIO(swOnOff[jig].oSwLamp, ON);
			swOnOff[jig].bRequireAction = TRUE;
			swOnOff[jig].bPressed = TRUE;
		}
	}
	else if(swOnOff[jig].bPressed)
	{
		if(_func.GetInPutIOCheck(swOnOff[jig].iSwInput) == FALSE)
		{
			swOnOff[jig].bPressed = FALSE;
			_func.SetOutPutIO(swOnOff[jig].oSwLamp, OFF);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// Vaccm State Check
	if( (swVacuum[jig].bPressed == FALSE) && (swVacuum[jig].bRequireActionOn == FALSE) && (swVacuum[jig].bRequireActionOff == FALSE) )
	{
		if( (_func.GetInPutIOCheck(swVacuum[jig].iSwInputOn) == FALSE) && _func.GetInPutIOCheck(swVacuum[jig].iSwInputOff) )
		{
			swVacuum[jig].bPressed = TRUE;
			swVacuum[jig].bRequireActionOn = FALSE;
			swVacuum[jig].bRequireActionOff = TRUE;
		}
		else if( _func.GetInPutIOCheck(swVacuum[jig].iSwInputOn) && (_func.GetInPutIOCheck(swVacuum[jig].iSwInputOff) == FALSE) )
		{
			swVacuum[jig].bPressed = TRUE;
			swVacuum[jig].bRequireActionOn = TRUE;
			swVacuum[jig].bRequireActionOff = FALSE;
		}
	}
	else if(swVacuum[jig].bPressed)
	{
		if( (_func.GetInPutIOCheck(swVacuum[jig].iSwInputOn) == FALSE) && (_func.GetInPutIOCheck(swVacuum[jig].iSwInputOff) == FALSE) )
			swVacuum[jig].bPressed = FALSE;
	}

	bVacuumOn = TRUE;
	for(int i=JIG_CH_1; i<JIG_CH_MAX; i++)
	{
		if(theProcBank.AZoneChannelNotUse_Check((JIG_ID)jig, (JIG_CH)i) == FALSE)
		{
			if(_func.Shuttle_VacOut_Check((JIG_ID)jig, (JIG_CH)i, VAC_ON) == FALSE)
				bVacuumOn = FALSE;
		}
	}
	if(bVacuumOn != bOldVacuumOn[jig])
	{
		if(bVacuumOn)
		{
			_func.SetOutPutIO(swVacuum[jig].oSwLampOn, ON);
			_func.SetOutPutIO(swVacuum[jig].oSwLampOff, OFF);
		}
		else
		{
			_func.SetOutPutIO(swVacuum[jig].oSwLampOn, OFF);
			_func.SetOutPutIO(swVacuum[jig].oSwLampOff, ON);
		}
		bOldVacuumOn[jig] = bVacuumOn;
	}
	//////////////////////////////////////////////////////////////////////////
	// Ready Button Check		
	if( _func.GetInPutIOCheck(swReady[jig].iLeftSw) || _func.GetInPutIOCheck(swReady[jig].iRightSw) )
	{
		if(swReady[jig].bPressEnd)
		{
			if(swReady[jig].bPressStart == FALSE)
			{
				swReady[jig].bPressStart = TRUE;
				swReady[jig].timerPress.Start();
			}
			swReady[jig].bPressEnd = FALSE;
		}
	}
	else
	{
		if(swReady[jig].bPressEnd == FALSE)
		{
			swReady[jig].bPressStart = FALSE;
			swReady[jig].bPressEnd = TRUE;
			theProcBank.m_bAZoneReadyPressed[jig] = FALSE;
		}
	}

	// 1초 안에 동시 눌러야 하고, 동시 눌렀어도 1초 지나면 무효 [9/11/2017 OSC]
	if(swReady[jig].bPressStart)
	{
		if(swReady[jig].timerPress.Stop(FALSE) < 1.)
		{
			if( _func.GetInPutIOCheck(swReady[jig].iLeftSw) && _func.GetInPutIOCheck(swReady[jig].iRightSw)
				&& (theProcBank.m_bAZoneReadyPressed[jig] == FALSE) )
			{
				theProcBank.m_bAZoneReadyPressed[jig] = TRUE;
			}
		}
		else
		{
			if(theProcBank.m_bAZoneReadyPressed[jig])
				theProcBank.m_bAZoneReadyPressed[jig] = FALSE;
		}
	}
	//////////////////////////////////////////////////////////////////////////
}
