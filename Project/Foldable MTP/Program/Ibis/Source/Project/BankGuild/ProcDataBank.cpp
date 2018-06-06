#include "StdAfx.h"
#include "ProcDataBank.h"
#include "IbisApp.h"
#include "MainFrm.h"
#include "UI\GausGUI\GxUICtrl.h"
//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
#include "UI\Dialog\DlgMsgTerminal.h"
#include "UI\Dialog\DlgMsgOPCall.h"
#include "UI\Dialog\DlgMsgInterlock.h"
#include "UI\Dialog\DIgCimMsg.h"
#include "Etc/FileSupport.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const CONST_MACHINE_STATE	ConstMachineState[] =
{
	_T("PAUSE"),
	_T("INIT"),
	_T("CHECK"),
	_T("IDLE"),
	_T("RUN"),
	_T("FAULT"),
	_T("MAINT"),
	_T("INTERLOCK"),
	_T("OPCALL"),
	_T("PMMODE")
};

const CONST_CIM_STATE	ConstCimState[] =
{
	_T("OFFLINE"),
	_T("REMOTE"),
	_T("LOCAL")
};

const CONST_EQUIP_PART		ConstEquipPart[] =
{
	{	_T("로더부"),			_T("LOAD"),			_T("LOAD")		},
	{	_T("검사부"),			_T("INSP"),			_T("INSP")			},
	{	_T("언로더부"),		_T("UNLOAD"),		_T("LOAD")		}
};

const CONST_EQUIP_UNIT		ConstEquipUnit[] =
{
	{	_T("EQUIP_UNIT_LOAD_COMMON"		),		_T("EQUIP_UNIT_LOAD_COMMON"		), _T("EQUIP_UNIT_LOAD_COMMON"	)	},
	{	_T("EQUIP_UNIT_LOAD_CONV"			),		_T("EQUIP_UNIT_LOAD_CONV"			), _T("EQUIP_UNIT_LOAD_CONV"			)	},
	{	_T("EQUIP_UNIT_SCARA_ROBOT	"		),		_T("EQUIP_UNIT_SCARA_ROBOT	"		), _T("EQUIP_UNIT_SCARA_ROBOT"		)	},
	{	_T("EQUIP_UNIT_BUFFER_STG"				),		_T("EQUIP_UNIT_BUFFER_STG"				), _T("EQUIP_UNIT_BUFFER_STG"			)	},
	{	_T("EQUIP_UNIT_LD_CELL_TR"				),		_T("EQUIP_UNIT_LD_CELL_TR"				), _T("EQUIP_UNIT_LD_CELL_TR"			)	},
	{	_T("EQUIP_UNIT_PREALIGN"				),		_T("EQUIP_UNIT_PREALIGN"				), _T("EQUIP_UNIT_PREALIGN"				)	},
	{	_T("EQUIP_UNIT_LD_TR"					),		_T("EQUIP_UNIT_LD_TR"					), _T("EQUIP_UNIT_LD_TR"					)	},
	{	_T("EQUIP_UNIT_TRAY_COMMON"		),		_T("EQUIP_UNIT_TRAY_COMMON"		), _T("EQUIP_UNIT_TRAY_COMMON"		)	},
	{	_T("EQUIP_UNIT_TRAY1"					),		_T("EQUIP_UNIT_TRAY1"					), _T("EQUIP_UNIT_TRAY1"					)	},
	{	_T("EQUIP_UNIT_TRAY2"					),		_T("EQUIP_UNIT_TRAY2"					), _T("EQUIP_UNIT_TRAY2"					)	},
	{	_T("EQUIP_UNIT_INSP_COMMON"		),		_T("EQUIP_UNIT_INSP_COMMON"		), _T("EQUIP_UNIT_INSP_COMMON"		)	},
	{	_T("EQUIP_UNIT_AZONE"					),		_T("EQUIP_UNIT_AZONE"					), _T("EQUIP_UNIT_AZONE"				)	},
	{	_T("EQUIP_UNIT_BZONE"					),		_T("EQUIP_UNIT_BZONE"					), _T("EQUIP_UNIT_BZONE"				)	},
	{	_T("EQUIP_UNIT_CZONE"					),		_T("EQUIP_UNIT_CZONE"					), _T("EQUIP_UNIT_CZONE"				)	},
	{	_T("EQUIP_UNIT_DZONE"					),		_T("EQUIP_UNIT_DZONE"					), _T("EQUIP_UNIT_DZONE"				)	},
	{	_T("EQUIP_UNIT_UNLOAD_COMMON"	),		_T("EQUIP_UNIT_UNLOAD_COMMON"	), _T("EQUIP_UNIT_UNLOAD_COMMON"	)	},
	{	_T("EQUIP_UNIT_UD_TR"					),		_T("EQUIP_UNIT_UD_TR"					), _T("EQUIP_UNIT_UD_TR"					)	},
	{	_T("EQUIP_UNIT_UD_STG"			),		_T("EQUIP_UNIT_UD_STG"			), _T("EQUIP_UNIT_UD_STG"			)	},
	{	_T("EQUIP_UNIT_UD_CELL_TR"				),		_T("EQUIP_UNIT_UD_CELL_TR"				), _T("EQUIP_UNIT_UD_CELL_TR"			)	}
};

void ALARM_INFOMATION::WriteErrorBuf()
{
	CString szOccTime, szClearTime, szTakeTime;
	szOccTime.Format(_T("%04u-%02u-%02u %02d:%02d:%02d"),TimeOccur.GetYear(), TimeOccur.GetMonth(), TimeOccur.GetDay(), TimeOccur.GetHour(), TimeOccur.GetMinute(), TimeOccur.GetSecond() );
	szClearTime.Format(_T("%04u-%02u-%02u %02d:%02d:%02d"), TimeOccur.GetYear(), TimeOccur.GetMonth(), TimeOccur.GetDay(),TimeClear.GetHour(), TimeClear.GetMinute(), TimeClear.GetSecond() );
	szTakeTime = szTimeTake;

	CString szFileDate, szPath, szLine, szTitle;
	CTime timeCur = TimeClear;	

	CEtc::ApplyTimeOffset(timeCur, theConfigBank.m_Option.m_OffsetHour, 0);

	szFileDate.Format(_T("%04u-%02u-%02u"),(WORD)timeCur.GetYear(), (WORD)timeCur.GetMonth(), (WORD)timeCur.GetDay());
	szPath.Format(_T("%s\\%s\\ErrorLog.csv"), theConfigBank.m_System.m_strDebugLogPath, szFileDate) ;

	CStdioFile file;
	CFileException e;

	//szTitle.Format(_T("ERR_ID, ERR_NAME, OCCUR_TIME, CLEAR_TIME, TAKE_TIME"));
	//kjpark 20180131 에러로그에 EQPID 추가
	szTitle.Format(_T("START TIME, END TIME, SPAN, CODE, NAME, LEVEL , %s"), theProcBank.m_strEqipID);
	//szLine.Format(_T("%s,%s,%s,%s,%s"), iERR_ID, szERR_NAME, szOccTime, szClearTime, szTakeTime);
	szLine.Format(_T("%s,%s,%s,%d,%s,%s"), szOccTime, szClearTime, szTakeTime, iERR_ID, szERR_NAME, szLevel);

	if ( !file.Open(szPath.GetBuffer(), CFile::modeRead, &e))			
	{
		if ( file.Open(szPath.GetBuffer(), CFile::modeCreate|CFile::modeWrite, &e ) )
		{
			file.SeekToEnd();															
			file.WriteString(szTitle+_T("\n"));				
		}	
		else
		{
			theLog[LOG_ERROR_TEMP].AddBuf(_T("ErrorLog.csv 기록실패 %s"), szLine);
			return;
		}
	}
	file.Close();


	if ( file.Open(szPath.GetBuffer(), CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate, &e ) )
	{
		file.SeekToEnd();
		file.WriteString(szLine+_T("\n"));
		file.Close();
	}
	else
	{
		theLog[LOG_ERROR_TEMP].AddBuf(_T("ErrorLog.csv 기록실패 %s"), szLine);
		return;
	}
}

//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
void OPALL_ALARM_INFOMATION::WriteOpCallErrorBuf()
{
	CString szOccTime;
	szOccTime.Format(_T("%04u-%02u-%02u %02d:%02d:%02d"),TimeOccur.GetYear(), TimeOccur.GetMonth(), TimeOccur.GetDay(), TimeOccur.GetHour(), TimeOccur.GetMinute(), TimeOccur.GetSecond() );

	CString szFileDate, szPath, szLine, szTitle;
	CTime timeCur = TimeOccur;	

	if ( timeCur.GetHour() < 7 )
		timeCur += CTimeSpan(-1,0,0,0);

	szFileDate.Format(_T("%04u-%02u-%02u"),(WORD)timeCur.GetYear(), (WORD)timeCur.GetMonth(), (WORD)timeCur.GetDay());
	szPath.Format(_T("%s\\%s\\OpCallAlarmLog.csv"), theConfigBank.m_System.m_strDebugLogPath, szFileDate) ;

	CStdioFile file;
	CFileException e;

	szTitle.Format(_T("START TIME\\ALARM NAME\\ALARM MESSAGE\\UNIT"));
	szLine.Format(_T("%s\\%s\\%s\\%s"), szOccTime, strOpCallAlarm_NAME, strOpCallAlarm_Message, strUnitID);

	if ( !file.Open(szPath.GetBuffer(), CFile::modeRead, &e))			
	{
		if ( file.Open(szPath.GetBuffer(), CFile::modeCreate|CFile::modeWrite, &e ) )
		{
			file.SeekToEnd();															
			file.WriteString(szTitle+_T("\n"));				
		}	
		else
		{
			theLog[LOG_ERROR_TEMP].AddBuf(_T("OpCallAlarmLog.csv 기록실패 %s"), szLine);
			return;
		}
	}
	file.Close();


	if ( file.Open(szPath.GetBuffer(), CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate, &e ) )
	{
		file.SeekToEnd();
		file.WriteString(szLine+_T("\n"));
		file.Close();
	}
	else
	{
		theLog[LOG_ERROR_TEMP].AddBuf(_T("OpCallAlarmLog.csv 기록실패 %s"), szLine);
		return;
	}
}

//kjpark 20170928 OPCall, Interlock. terminel MSG 히스토리 따로 관리
void INTERLOCK_ALARM_INFOMATION::WriteInterlockErrorBuf()
{
	CString szOccTime;
	szOccTime.Format(_T("%04u-%02u-%02u %02d:%02d:%02d"),TimeOccur.GetYear(), TimeOccur.GetMonth(), TimeOccur.GetDay(), TimeOccur.GetHour(), TimeOccur.GetMinute(), TimeOccur.GetSecond() );

	CString szFileDate, szPath, szLine, szTitle;
	CTime timeCur = TimeOccur;	

	if ( timeCur.GetHour() < 7 )
		timeCur += CTimeSpan(-1,0,0,0);

	szFileDate.Format(_T("%04u-%02u-%02u"),(WORD)timeCur.GetYear(), (WORD)timeCur.GetMonth(), (WORD)timeCur.GetDay());
	szPath.Format(_T("%s\\%s\\InterlockAlarmLog.csv"), theConfigBank.m_System.m_strDebugLogPath, szFileDate) ;

	CStdioFile file;
	CFileException e;

	szTitle.Format(_T("START TIME\\ALARM NAME\\ALARM MESSAGE\\UNIT"));
	szLine.Format(_T("%s\\%s\\%s\\%s"), szOccTime, strInterlock_ID, strInterlock_Message, strUnit_ID);

	if ( !file.Open(szPath.GetBuffer(), CFile::modeRead, &e))			
	{
		if ( file.Open(szPath.GetBuffer(), CFile::modeCreate|CFile::modeWrite, &e ) )
		{
			file.SeekToEnd();															
			file.WriteString(szTitle+_T("\n"));				
		}	
		else
		{
			theLog[LOG_ERROR_TEMP].AddBuf(_T("InterlockAlarmLog.csv 기록실패 %s"), szLine);
			return;
		}
	}
	file.Close();


	if ( file.Open(szPath.GetBuffer(), CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate, &e ) )
	{
		file.SeekToEnd();
		file.WriteString(szLine+_T("\n"));
		file.Close();
	}
	else
	{
		theLog[LOG_ERROR_TEMP].AddBuf(_T("InterlockAlarmLog.csv 기록실패 %s"), szLine);
		return;
	}
}

//kjpark 20170928 OPCall, Interlock. terminel MSG 히스토리 따로 관리
void TERMINEL_ALARM_INFOMATION::WriteTerminelErrorBuf()
{
	CString szOccTime;
	szOccTime.Format(_T("%04u-%02u-%02u %02d:%02d:%02d"),TimeOccur.GetYear(), TimeOccur.GetMonth(), TimeOccur.GetDay(), TimeOccur.GetHour(), TimeOccur.GetMinute(), TimeOccur.GetSecond() );

	CString szFileDate, szPath, szLine, szTitle;
	CTime timeCur = TimeOccur;	

	if ( timeCur.GetHour() < 7 )
		timeCur += CTimeSpan(-1,0,0,0);

	szFileDate.Format(_T("%04u-%02u-%02u"),(WORD)timeCur.GetYear(), (WORD)timeCur.GetMonth(), (WORD)timeCur.GetDay());
	szPath.Format(_T("%s\\%s\\TeminelAlarmLog.csv"), theConfigBank.m_System.m_strDebugLogPath, szFileDate) ;

	CStdioFile file;
	CFileException e;

	szTitle.Format(_T("START TIME\\ALARM NAME\\ALARM MESSAGE"));
	szLine.Format(_T("%s\\%s\\%s"), szOccTime, strTerminelAlarm_NAME, strTerminelAlarm_Message);

	if ( !file.Open(szPath.GetBuffer(), CFile::modeRead, &e))			
	{
		if ( file.Open(szPath.GetBuffer(), CFile::modeCreate|CFile::modeWrite, &e ) )
		{
			file.SeekToEnd();															
			file.WriteString(szTitle+_T("\n"));				
		}	
		else
		{
			theLog[LOG_ERROR_TEMP].AddBuf(_T("TeminelAlarmLog.csv 기록실패 %s"), szLine);
			return;
		}
	}
	file.Close();


	if ( file.Open(szPath.GetBuffer(), CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate, &e ) )
	{
		file.SeekToEnd();
		file.WriteString(szLine+_T("\n"));
		file.Close();
	}
	else
	{
		theLog[LOG_ERROR_TEMP].AddBuf(_T("TeminelAlarmLog.csv 기록실패 %s"), szLine);
		return;
	}
}

void CProcDataBank::WriteErrorLogCsv()
{
	// ErrorLog.csv 기록 [6/30/2017 OSC]
	if (alarm_info.iERR_ID != ALM_NONE)
	{
		alarm_info.TimeClear = CTime::GetCurrentTime();		
		alarm_info.SetTakeTime();

		alarm_info.WriteErrorBuf();
		alarm_info.iERR_ID = ALM_NONE;
	}		
}

//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
void CProcDataBank::SetWriteOpCallLog(OPALL_ALARM_INFOMATION opcallElement)
{
	opCall_alarm_info.strOpCallAlarm_NAME = opcallElement.strOpCallAlarm_NAME;
	opCall_alarm_info.strOpCallAlarm_Message = opcallElement.strOpCallAlarm_Message;
	SYSTEMTIME OpCallAlarmTime;
	GetLocalTime(&OpCallAlarmTime);
	opCall_alarm_info.TimeOccur = CTime(OpCallAlarmTime);
	opCall_alarm_info.WriteOpCallErrorBuf();
	opCall_alarm_info.strOpCallAlarm_NAME.Empty();
	opCall_alarm_info.strOpCallAlarm_Message.Empty();	
}

//kjpark 20170928 OPCall, Interlock. terminel MSG 히스토리 따로 관리
void CProcDataBank::SetWriteInterlockLog(INTERLOCK_ALARM_INFOMATION InterloclElement)
{
	Interlock_alarm_info.strInterlock_ID = InterloclElement.strInterlock_ID;
	Interlock_alarm_info.strInterlock_Message = InterloclElement.strInterlock_Message;
	Interlock_alarm_info.strUnit_ID = InterloclElement.strUnit_ID;
	SYSTEMTIME InterlockAlarmTime;
	GetLocalTime(&InterlockAlarmTime);
	Interlock_alarm_info.TimeOccur = CTime(InterlockAlarmTime);
	Interlock_alarm_info.WriteInterlockErrorBuf();
	Interlock_alarm_info.strInterlock_ID.Empty();
	Interlock_alarm_info.strInterlock_Message.Empty();	
}

//kjpark 20170928 OPCall, Interlock. terminel MSG 히스토리 따로 관리
void CProcDataBank::SetWriteTerminelLog(TERMINEL_ALARM_INFOMATION TerminelElement)
{
	Terminel_alarm_info.strTerminelAlarm_NAME = TerminelElement.strTerminelAlarm_NAME;
	Terminel_alarm_info.strTerminelAlarm_Message = TerminelElement.strTerminelAlarm_Message;
	SYSTEMTIME TerminelAlarmTime;
	GetLocalTime(&TerminelAlarmTime);
	Terminel_alarm_info.TimeOccur = CTime(TerminelAlarmTime);
	Terminel_alarm_info.WriteTerminelErrorBuf();
	Terminel_alarm_info.strTerminelAlarm_NAME.Empty();
	Terminel_alarm_info.strTerminelAlarm_Message.Empty();	
}
CProcDataBank::CProcDataBank(void)
{
	m_strControlPCVersion = PROGRAM_VERSION;
	m_hMutex = ::CreateMutex(NULL, FALSE, NULL);
	m_MachineState = CONST_MACHINE_STATE::MACHINE_FAULT;
	m_OldMachineState = CONST_MACHINE_STATE::MACHINE_FAULT;
	m_CimState = CONST_CIM_STATE::CIM_OFFLINE;
	m_bProgramInitFinish = FALSE;
	m_bMachineCellExist = FALSE;
	ResetTemporarilyValue();

	//kjpark 20170831 MCR 구현 채널 별 구현 완료
	for(int jig = JIG_ID_A ; jig < JIG_ID_MAX ; jig++)
	{
		for(int ch = JIG_CH_1 ; ch < JIG_CH_MAX ; ch++)
		{
			m_bMCRResultRecive[jig][ch] = FALSE;
			m_strLastCellID[jig][ch].Empty();
			m_listRetryCell[jig][ch].RemoveAll();
			m_bAZoneCellSkip[jig][ch] = FALSE;
			m_bAZoneChannelNotUse[jig][ch] = FALSE;
			m_PGData[jig][ch].Init((JIG_ID)jig, (JIG_CH)ch);
		}
	}

	m_bLabelPrintOut = FALSE;
	m_pRetryCell = NULL;

	m_listPreInterlockStep.RemoveAll();
	m_listPreInterlockLoad.RemoveAll();
	m_listPreInterlockTrans.RemoveAll();

	alarm_info.iERR_ID = ALM_NONE;
	m_bUserLoadingStop = FALSE;

	PDT_IF_NoCheckVacOff_Clear();
}


CProcDataBank::~CProcDataBank(void)
{
	::CloseHandle(m_hMutex);
}

void CProcDataBank::ChangeMachineState( CONST_MACHINE_STATE::ID newState )
{
	if(m_MachineState != newState)
	{
		theLog[LOG_SYSTEM].AddBuf(_T("Machine State Change %s -> %s"), ConstMachineState[m_MachineState].strName, ConstMachineState[newState].strName);
		m_OldMachineState = m_MachineState;
		m_MachineState = newState;

		//////////////////////////////////////////////////////////////////////////
		if(m_MachineState == CONST_MACHINE_STATE::MACHINE_FAULT)
		{
			BOOL bBuzzerOn = TRUE;
			// Robot Alarm이면 부저를 울리지 않는다
			if(m_AlarmID >= ALM_ROBOT_ALARM_START)
				bBuzzerOn = FALSE;
			theUnitFunc.TowerLamp_Change(theConfigBank.m_Option.iMapLampSettingList[m_MachineState].Red.lamp_state
				,theConfigBank.m_Option.iMapLampSettingList[m_MachineState].Yellow.lamp_state
				, theConfigBank.m_Option.iMapLampSettingList[m_MachineState].Green.lamp_state
				,bBuzzerOn);
		}
		else if(m_MachineState == CONST_MACHINE_STATE::MACHINE_INTERLOCK)
		{
			theUnitFunc.TowerLamp_Change(theConfigBank.m_Option.iMapLampSettingList[m_MachineState].Red.lamp_state
				,theConfigBank.m_Option.iMapLampSettingList[m_MachineState].Yellow.lamp_state
				, theConfigBank.m_Option.iMapLampSettingList[m_MachineState].Green.lamp_state
				,TRUE);
		}
		else
		{
			theUnitFunc.TowerLamp_Change(theConfigBank.m_Option.iMapLampSettingList[m_MachineState].Red.lamp_state
				,theConfigBank.m_Option.iMapLampSettingList[m_MachineState].Yellow.lamp_state
				, theConfigBank.m_Option.iMapLampSettingList[m_MachineState].Green.lamp_state
				,FALSE);
		}

		//////////////////////////////////////////////////////////////////////////
		if(MachineIsRunState())
		{
			// MUTE 신호를주면 감지되도 Motor Power Off가 되지 않는다 [11/12/2016 OSC]
			theUnitFunc.LightCurtainMute_OnOff(JIG_ID_MAX, OFF);
			theUnitFunc.LightCurtainMuteLamp_OnOff(JIG_ID_MAX, OFF);
			::PostMessageA(GetMainWnd(), MSG_CIM_MESSAGE_UPDATE, NULL, CIM_MESSAGE_MAX);
		}
		else if(m_MachineState == CONST_MACHINE_STATE::MACHINE_INIT)
		{
			// MUTE 신호를주면 감지되도 Motor Power Off가 되지 않는다 [11/12/2016 OSC]
			theUnitFunc.LightCurtainMute_OnOff(JIG_ID_MAX, OFF);
			theUnitFunc.LightCurtainMuteLamp_OnOff(JIG_ID_MAX, OFF);
		}
		else
		{
			// MUTE 신호를주면 감지되도 Motor Power Off가 되지 않는다 [11/12/2016 OSC]
			theUnitFunc.LightCurtainMute_OnOff(JIG_ID_MAX, ON);
			theUnitFunc.LightCurtainMuteLamp_OnOff(JIG_ID_MAX, ON);
		}

	
		if(GetMainHandler())
		{
			::PostMessage(GetMainHandler()->m_pGUIFrame->GetDialogBar(eDLGBAR_Top)->m_hWnd, UDMSG_STATE_CHANGE, NULL, NULL);		// 연결 이벤트
			::PostMessage(GetMainHandler()->m_pGUIFrame->GetDialogBar(eDLGBAR_Bottom)->m_hWnd, UDMSG_STATE_CHANGE, NULL, NULL);		// 연결 이벤트
		}
	}
}

BOOL CProcDataBank::MachineIsRunState()
{
	return ModuleIsRunState(m_MachineState);
}

BOOL CProcDataBank::ModuleIsRunState(CONST_MACHINE_STATE::ID id)
{
	CONST_MACHINE_STATE::ID machineState = id;

	if( (machineState == CONST_MACHINE_STATE::MACHINE_RUN) 
		|| (machineState == CONST_MACHINE_STATE::MACHINE_IDLE)
		|| (machineState == CONST_MACHINE_STATE::MACHINE_CHECK))
		return TRUE;
	else
		return FALSE;
}

void CProcDataBank::ChangeCimState( CONST_CIM_STATE::ID newState )
{
	if(m_CimState != newState)
	{
		theLog[LOG_SYSTEM].AddBuf(_T("Cim State Change %s -> %s"), ConstCimState[m_CimState].strName, ConstCimState[newState].strName);
		m_CimState = newState;
	}
	if(GetMainHandler())
	{
		::PostMessage(GetMainHandler()->m_pGUIFrame->GetDialogBar(eDLGBAR_Top)->m_hWnd, UDMSG_CIM_STATE_CHANGE, NULL, NULL);		// 연결 이벤트
	}
}

void CProcDataBank::PreInterlock_Add(CString strEFST, INTERLOCK_ALARM_INFOMATION info)
{
	if(strEFST == EFST_STEP)
		m_listPreInterlockStep.AddTail(info);
	else if(strEFST == EFST_TRANSFER)
		m_listPreInterlockTrans.AddTail(info);
	else if(strEFST == EFST_LOADING)
		m_listPreInterlockLoad.AddTail(info);
}

BOOL CProcDataBank::PreInterlock_IsEmpty(CString strEFST)
{
	if(strEFST == EFST_STEP)
		return m_listPreInterlockStep.GetCount() == 0 ? TRUE:FALSE;
	else if(strEFST == EFST_TRANSFER)
		return m_listPreInterlockTrans.GetCount() == 0 ? TRUE:FALSE;
	else if(strEFST == EFST_LOADING)
		return m_listPreInterlockLoad.GetCount() == 0 ? TRUE:FALSE;

	return TRUE;
}

void CProcDataBank::PreInterlock_Clear(CString strEFST)
{
	if(strEFST == EFST_STEP)
		m_listPreInterlockStep.RemoveAll();
	else if(strEFST == EFST_TRANSFER)
		m_listPreInterlockTrans.RemoveAll();
	else if(strEFST == EFST_LOADING)
		m_listPreInterlockLoad.RemoveAll();
}

void CProcDataBank::PreUnitInterlock_Add(INTERLOCK_ALARM_INFOMATION info)
{
	m_listPreUnitInterlock.AddTail(info);
}

BOOL CProcDataBank::PreUnitInterlock_IsEmpty(JIG_ID jig)
{
	if(jig == JIG_ID_MAX)
	{
		return m_listPreUnitInterlock.GetCount() == 0 ? TRUE:FALSE;
	}
	else
	{
		INTERLOCK_ALARM_INFOMATION alarm;
		POSITION pos = m_listPreUnitInterlock.GetHeadPosition();
		while(pos)
		{
			alarm = m_listPreUnitInterlock.GetNext(pos);
			if(alarm.strUnit_ID == theUnitStatusBank.m_Unit[jig][JIG_CH_1].m_strUnitID)
				return FALSE;
		}
		return TRUE;
	}
}

void CProcDataBank::PreUnitInterlock_Clear()
{
	m_listPreUnitInterlock.RemoveAll();
}

BOOL CProcDataBank::UserLoadingStop_Check()
{
	return m_bUserLoadingStop;
}

void CProcDataBank::UserLoadingStop_OnOff(BOOL bOn)
{
	m_bUserLoadingStop = bOn;
}

BOOL CProcDataBank::LoadingStop_IsRequire()
{
	if(PreInterlock_IsEmpty(EFST_LOADING) == FALSE)
	{
		return TRUE;
	}
	if(PreUnitInterlock_IsEmpty(JIG_ID_MAX) == FALSE)
	{
		if(theConfigBank.m_CIM.INTERLOCK_CONTROL_LoadingCheck())
		{
			return TRUE;
		}
	}
	if(theSocketInterFace.m_CIM.FunctionChangeBuffer.bChangeFlag)
	{
		return TRUE;
	}
	if(m_bUserLoadingStop)
	{
		return TRUE;
	}

	return FALSE;
}

//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
void CProcDataBank::SetOPCall(BOOL bUnit)
{
	theUnitFunc.TowerLamp_Change(theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_OPCALL].Red.lamp_state
		,theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_OPCALL].Yellow.lamp_state
		, theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_OPCALL].Green.lamp_state
		,TRUE);

	::PostMessageA(GetMainWnd(), MSG_CIM_MESSAGE_UPDATE, NULL, CIM_MESSAGE_OPCALL);

	CDlgMsgOPCall dlgMsgBox;

	BOOL ret = dlgMsgBox.DoModal();

	theUnitFunc.TowerLamp_Change(theConfigBank.m_Option.iMapLampSettingList[m_MachineState].Red.lamp_state
		,theConfigBank.m_Option.iMapLampSettingList[m_MachineState].Yellow.lamp_state
		, theConfigBank.m_Option.iMapLampSettingList[m_MachineState].Green.lamp_state
		,FALSE);

	theSocketInterFace.m_CIM.SendCmdToDataPC(EQUIP_SET_OPCALL_EVENT);
}

//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
void CProcDataBank::SetInterlock(CString strInterlockType)
{
	theSocketInterFace.m_CIM.SendCmdStateToDataPC(EQUIP_SET_EQUIPMENT_STATUS_CHANGE, 
		E_EQST_MATCH_INTERLOCK, strInterlockType);
	// 이미 발생한 상태에 추가면 Interlock 발생 했다는 보고만 하고 만다 [12/9/2017 OSC]
	if(m_MachineState == CONST_MACHINE_STATE::MACHINE_INTERLOCK)
		return;
	ChangeMachineState(CONST_MACHINE_STATE::MACHINE_INTERLOCK);

	::PostMessageA(GetMainWnd(), MSG_CIM_MESSAGE_UPDATE, NULL, CIM_MESSAGE_INTERLOCK);

	theLossTimeLog.m_rLosstimeRecord.Clear();
	theLossTimeLog.LoadTPNameToLosstimeRecode(TP_CODE_BM_BREAK_DOWN);

	// 모달리스로 변경 [11/30/2017 OSC]
	m_strInterlockType = strInterlockType;
	::PostMessage(GetMainWnd(), MSG_SHOW_DIALOG, eDLG_MsgInterlock, NULL);
// 	CDlgMsgInterlock dlgMsgBox;
// 
// 	BOOL ret = dlgMsgBox.DoModal();
// 	theSocketInterFace.m_CIM.SendCmdToDataPC(EQUIP_SET_INTERLOCK_EVENT, strInterlockType);
// 
// 	ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);
// 	theSocketInterFace.m_CIM.SendCmdStateToDataPC(EQUIP_SET_EQUIPMENT_STATUS_CHANGE, 
// 		E_EQST_MATCH_PAUSE, _T(""));
}

//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
void CProcDataBank::SetJobProcessMsg()
{
	//theUnitFunc.SetOutPutIO(Y_BUZZER_K1_PC_RACK_UP, ON);

	::PostMessageA(GetMainWnd(), MSG_CIM_MESSAGE_UPDATE, NULL, CIM_MESSAGE_JOBPROCESS);

	CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("확인"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(FALSE, 
		_T("Cell위치 안 맞습니다."), 
		_T("Cell is Not Maching."),		
		_T("Cell đặt không đúng."), GetMainHandler()->m_nLangIdx );

	int nRet = dlgMsgBox.DoModal();
	//theUnitFunc.SetOutPutIO(Y_BUZZER_K1_PC_RACK_UP, OFF);

	::PostMessageA(GetMainWnd(), MSG_CIM_MESSAGE_UPDATE, NULL, CIM_MESSAGE_MAX);
}

//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
void CProcDataBank::SetTerminalMsg()
{
	//kjpark 20170920 LOGIN/OUT 구현
	CGxMsgBox	dlgMsgBox;
	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경

	CTokenizer t(m_strTerminalMsg);
	CString strTemp = t.GetNextToken(_T("_"));
	CString strTitle, strMessage;
	if(strTemp == _T("LOGIN"))
	{
		strTitle =strTemp;
		strMessage = t.GetString();
		strTemp = t.GetNextToken(_T("_"));
		if(strTemp == _T("PASS"))
		{
			// 로그인 성공 [12/2/2016 OSC]			
		}
		else
		{
			// 로그인 실패 [12/2/2016 OSC]
			CUserAccount user;
			GetMainHandler()->SetCurUserInspectorData(user);
			strTemp = t.GetNextToken(_T("_"), 2);
		}
		dlgMsgBox.SetTitle(strTitle, strTitle, strTitle);
		dlgMsgBox.SetMessage(FALSE, 
			strMessage, 
			strMessage,		
			strMessage );

		dlgMsgBox.DoModal();
	}
	else if(strTemp == _T("LOGOUT"))
	{
		strTitle =strTemp;
		strMessage = t.GetString();
		strTemp = t.GetNextToken(_T("_"));
		if(strTemp == _T("PASS"))
		{
			// 로그아웃 성공 [12/2/2016 OSC]
			CUserAccount user = GetMainHandler()->GetCurOperatorUserInspectorData();
			user.Init();
			GetMainHandler()->SetCurUserInspectorData(user);			
		}
		else
		{
			// 로그아웃 실패 [12/2/2016 OSC]
			theSocketInterFace.m_CIM.SendCmdLoginToDataPC(EQUIP_SET_USER_LOGIN_REQUEST, GetMainHandler()->GetCurOperatorUserInspectorData().sID, GetMainHandler()->GetCurOperatorUserInspectorData().sPassword);			
		}
// 		strMessage = strTemp;
		dlgMsgBox.SetTitle(strTitle, strTitle, strTitle);
		dlgMsgBox.SetMessage(FALSE, 
			strMessage, 
			strMessage,		
			strMessage );

		dlgMsgBox.DoModal();
	}
	else
	{
		//kjpark 20170711 CIM MESSAGE STATUS 상태 표기
		::PostMessageA(GetMainWnd(), MSG_CIM_MESSAGE_UPDATE, NULL, CIM_MESSAGE_TEMINAL);
		
		//kjpark 20180101 Teminal MSG 10개 리스트로 표시
		if(theProcBank.m_MsgTerminalCnt >= 10) return;

		theProcBank.m_strTerminalMsgs[theProcBank.m_MsgTerminalCnt].Format(_T("%s\n\t"), m_strTerminalMsg);
		theProcBank.m_MsgTerminalCnt++;

	//kjpark 20180117 Teminal MSG 10개 리스트로 표시 버그 수정
		if(theProcBank.m_MsgTerminalCnt <= 1)
		{
			CDlgMsgTerminal dlgMsgTerminal;

			dlgMsgTerminal.DoModal();
		}
		
#ifdef DOMODAL
		strTitle.Format(_T("Terminal Message"));		
		strMessage = m_strTerminalMsg;

		dlgMsgBox.SetTitle(strTitle, strTitle, strTitle);
		dlgMsgBox.SetMessage(FALSE, 
			strMessage, 
			strMessage,		
			strMessage );

		dlgMsgBox.DoModal();
#endif
		
		//kjpark 20170711 CIM MESSAGE STATUS 상태 표기
		::PostMessageA(GetMainWnd(), MSG_CIM_MESSAGE_UPDATE, NULL, CIM_MESSAGE_MAX);
	}
}

BOOL CProcDataBank::RetryCellInfo_Find( JIG_ID jig, JIG_CH ch, CString strCellID )
{
	// 리트라이에 해당되는 Cell인지 확인하여 해당 Cell 정보를 넘겨준다 
	// 찾은 Cell 정보는 리스트에서 삭제 [9/27/2017 OSC]
	CCellInfo *pRetryCell;
	POSITION pos;
	POSITION posRemove;

	//kjpark 20180122 리트라이 셀도 아닌데 리트라이 셀로 인식하는 보그 테스트 중
	theLog[LOG_RETRY].AddBuf(_T("JIG = %d    CH = %d     CellID = %s"), jig, ch, strCellID);
	// 자기자신 채널 외에 다른 채널에서 전부 찾는다 [12/11/2017 OSC]
	for(int i = 0; i < JIG_ID_MAX; i++)
	{
		for(int j = 0; j < JIG_CH_MAX; j++)
		{
			// 자기 채널 건너뛰기
			if((i == jig) && (j == ch))
				continue;

			pos = m_listRetryCell[i][j].GetHeadPosition();
			while(pos)
			{
				posRemove = pos;
				pRetryCell = m_listRetryCell[i][j].GetNext(pos);

				//kjpark 20180122 리트라이 셀도 아닌데 리트라이 셀로 인식하는 보그 테스트 중
				theLog[LOG_RETRY].AddBuf(_T("JIG = %d    CH = %d      FINE ID = %s       pRetryCell JIG = %d    pRetryCell CH = %d     Parmeter  CellID = %s"), 
					i,j, pRetryCell->defaultData.m_strCellID, pRetryCell->defaultData.m_JigId, pRetryCell->defaultData.m_JigCh, strCellID);
				if(pRetryCell->defaultData.m_strCellID == strCellID
					&& (pRetryCell->defaultData.m_bRetryAB == FALSE) 
					&& (pRetryCell->defaultData.m_LastClass != GOOD_CELL)
					&& pRetryCell->defaultData.m_bRetryAble )
				{
					theProcBank.m_pRetryCell = pRetryCell;
					m_listRetryCell[i][j].RemoveAt(posRemove);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

void CProcDataBank::RetryCellInfo_Add( JIG_ID jig, CCellInfo *pCell )
{
	// AB Retry 확인하여 CellInfo 포인터 보관.. Tag번호가 한바퀴 돌 때 까지는 CellInfo가 초기화 안되니 이렇게 해도 된다 [9/26/2017 OSC]
	int ch = pCell->defaultData.m_JigCh;
	if( pCell->Retry_CheckAble() && pCell->defaultData.m_bMCR_OK == TRUE)
	{
		m_listRetryCell[jig][ch].AddTail(pCell);

		theLog[LOG_RETRY].AddBuf(_T("%cJIG %dCH InnerID[%s] CellID[%s] Retry Able. List Count [%d]"), 
			pCell->defaultData.m_JigId+_T('A'),
			ch+1,
			pCell->defaultData.m_strInnerID,
			pCell->defaultData.m_strCellID,
			m_listRetryCell[jig][ch].GetCount());

	}
}

void CProcDataBank::RetryCellInfo_Remove(JIG_ID jig, JIG_CH ch)
{
	if(m_listRetryCell[jig][ch].GetHeadPosition() && m_listRetryCell[jig][ch].GetCount() > 1000)
	{
		CCellInfo *pRetryCell = m_listRetryCell[jig][ch].RemoveHead();
		theSocketInterFace.m_CIM.SendCmdCellToDataPC(EQUIP_SET_CELL_PROCESS_COMPLETE_UNLOAD, pRetryCell, CIM_JUDGE_LOSS);
		theLog[LOG_RETRY].AddBuf(_T("%cJIG %dCH InnerID[%s] CellID[%s] count = %d  Retry Canceled"),
			pRetryCell->defaultData.m_JigId + _T('A'),
			ch + 1,
			pRetryCell->defaultData.m_strInnerID,
			pRetryCell->defaultData.m_strCellID,
			m_listRetryCell[jig][ch].GetCount());
	}
}

void CProcDataBank::RetryCellInfo_AllTrackOut( JIG_ID jig, JIG_CH ch, int nLimitCount /*= 0*/ )
{
	// 먼저 들어온 순서대로 순착적으로 Track Out [9/29/2017 OSC]
	CCellInfo *pRetryCell;
	while(m_listRetryCell[jig][ch].GetCount() > nLimitCount)
	{
		pRetryCell = m_listRetryCell[jig][ch].RemoveHead();
		theSocketInterFace.m_CIM.SendCmdCellToDataPC(EQUIP_SET_CELL_PROCESS_COMPLETE_UNLOAD, pRetryCell, CIM_JUDGE_LOSS);
		theLog[LOG_RETRY].AddBuf(_T("%cJIG %dCH InnerID[%s] CellID[%s] Retry Canceled ALL"), 
			pRetryCell->defaultData.m_JigId+_T('A'),
			ch+1,
			pRetryCell->defaultData.m_strInnerID,
			pRetryCell->defaultData.m_strCellID);
	}
}

BOOL CProcDataBank::RetryCellInfo_IsExist()
{
	// 리트라이 위해 쟁여두고 있는 Cell이 있는지 확인 [10/16/2017 OSC]
	for(int i = 0; i < JIG_ID_MAX; i++)
	{
		for(int ch = 0; ch < JIG_CH_MAX; ch++)
		{
			if(m_listRetryCell[i][ch].GetCount() > 0)
				return TRUE;
		}
	}
	return FALSE;
}

void CProcDataBank::ReadAlarmFile()
{
	CString filename;

	filename.Format(_T("%s\\AlarmList.ini"), theConfigBank.m_System.m_strProgramDataPath);


	for(int i = 0; i < ALM_MAX; i++)
	{
		m_AlarmMsg[i].nAlarmLevel = ALARM_LEVEL_ALARM;
		if(i >= ALM_ROBOT_ALARM_START)
		{
			// ROBOT ALARM
			for(int j = 0; j < LAN_MAX; j++)
			{

				m_AlarmMsg[i].strName[j] = _T("ROBOT_UNDEFINED_ALARM");
				m_AlarmMsg[i].strCause[j] = _T("NO_CAUSE");
				m_AlarmMsg[i].strMethod[j] = _T("NO_METHOD");
			}
		}
		else if(i == ALM_MTP_ALARM_END)
		{
			// 안쓰는 영역은 건너뛰기
			i = ALM_ROBOT_ALARM_START-1;
		}
		else
		{
			// 검사기 ALARM
			for(int j = 0; j < LAN_MAX; j++)
			{

				m_AlarmMsg[i].strName[j] = _T("NO_ERROR");
				m_AlarmMsg[i].strCause[j] = _T("NO_CAUSE");
				m_AlarmMsg[i].strMethod[j] = _T("NO_METHOD");
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 경알람 처리할것들은 여기서 하드코딩 [9/26/2017 OSC]
	m_AlarmMsg[ALM_SHUTTLE_1_INVALID_TIME_OVER].nAlarmLevel = ALARM_LEVEL_WARNING;
	m_AlarmMsg[ALM_SHUTTLE_2_INVALID_TIME_OVER].nAlarmLevel = ALARM_LEVEL_WARNING;
	m_AlarmMsg[ALM_SHUTTLE_3_INVALID_TIME_OVER].nAlarmLevel = ALARM_LEVEL_WARNING;
	m_AlarmMsg[ALM_SHUTTLE_1_JOB_INVALID].nAlarmLevel = ALARM_LEVEL_WARNING;
	m_AlarmMsg[ALM_SHUTTLE_2_JOB_INVALID].nAlarmLevel = ALARM_LEVEL_WARNING;
	m_AlarmMsg[ALM_SHUTTLE_3_JOB_INVALID].nAlarmLevel = ALARM_LEVEL_WARNING;
	m_AlarmMsg[ALM_SHUTTLE_1_CELLINFO_NG].nAlarmLevel = ALARM_LEVEL_WARNING;
	m_AlarmMsg[ALM_SHUTTLE_2_CELLINFO_NG].nAlarmLevel = ALARM_LEVEL_WARNING;
	m_AlarmMsg[ALM_SHUTTLE_3_CELLINFO_NG].nAlarmLevel = ALARM_LEVEL_WARNING;
	//////////////////////////////////////////////////////////////////////////

	FILE *fStream;
	errno_t e = _tfopen_s(&fStream, filename.GetString(), _T("r,ccs=UTF-8"));
	if (e != 0) return;
	CStdioFile		file(fStream);
	CString strBuf, strItem;
	int nIndex = -1;
	CTokenizer t;
	while(file.ReadString(strBuf))
	{
		// HEADER는 무시
		if(strBuf == _T("[HEADER]"))
			continue;

		if( (strBuf.Left(1) == _T("[")) && (strBuf.Right(1) == _T("]")) )
		{
			// SECTION Check
			strItem = strBuf.Trim(_T("[]"));
			if(strItem.GetLength() > 0)
			{
				nIndex = _tstoi(strItem.GetString());
				if(nIndex >= ALM_MAX)
					nIndex = -1;
			}
		}
		else if(nIndex >= 0)
		{
			// 그 외 나머지는 nIndex가 있을 때만 처리
			t = CTokenizer(strBuf);
			strItem = t.GetNextToken(_T("="));
			if(strItem == _T("ALARM_NAME_KOR"))
			{
				m_AlarmMsg[nIndex].strName[LAN_KOR] = t.GetNextToken(_T("="));
			}
			else if(strItem == _T("ALARM_NAME_ENG"))
			{
				m_AlarmMsg[nIndex].strName[LAN_ENG] = t.GetNextToken(_T("="));
			}
			else if(strItem == _T("ALARM_NAME_VNM"))
			{
				m_AlarmMsg[nIndex].strName[LAN_VNM] = t.GetNextToken(_T("="));
			}

			if(strItem == _T("CAUSE_KOR"))
			{
				m_AlarmMsg[nIndex].strCause[LAN_KOR] = t.GetNextToken(_T("="));
			}
			else if(strItem == _T("CAUSE_ENG"))
			{
				m_AlarmMsg[nIndex].strCause[LAN_ENG] = t.GetNextToken(_T("="));
			}
			else if(strItem == _T("CAUSE_VNM"))
			{
				m_AlarmMsg[nIndex].strCause[LAN_VNM] = t.GetNextToken(_T("="));
			}

			if(strItem == _T("METHOD_KOR"))
			{
				m_AlarmMsg[nIndex].strMethod[LAN_KOR] = t.GetNextToken(_T("="));
			}
			else if(strItem == _T("METHOD_ENG"))
			{
				m_AlarmMsg[nIndex].strMethod[LAN_ENG] = t.GetNextToken(_T("="));
			}
			else if(strItem == _T("METHOD_VNM"))
			{
				m_AlarmMsg[nIndex].strMethod[LAN_VNM] = t.GetNextToken(_T("="));
			}
		}
	}

	file.Close();

	return;
}

void CProcDataBank::ReadPDTAlarmFile()
{
	CString filename;

	filename.Format(_T("%s\\PDT_Alarmlist.csv"), theConfigBank.m_System.m_strConfigPath);


	for(int i = ALM_ROBOT_ALARM_START; i < ALM_MAX; i++)
	{
		m_AlarmMsg[i].nAlarmLevel = ALARM_LEVEL_ALARM;
		for(int j = 0; j < LAN_MAX; j++)
		{

			m_AlarmMsg[i].strName[j] = _T("ROBOT_UNDEFINED_ALARM");
			m_AlarmMsg[i].strCause[j] = _T("NO_CAUSE");
			m_AlarmMsg[i].strMethod[j] = _T("NO_METHOD");
		}
	}

	

#if 1
	// 파일이 존재하는지 점검
	if(CFileSupport::IsFile(filename) == FALSE) 	
	{	
		return;
	}

	// .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..

	CString sLine;						// 파일에서 읽어온 데이터	

	CString		sKey;					// 검색할 때 사용되는 Key값, 공백제거, 대문자 변환
	CString		strAlarmName;					// 파일에서 읽어들인 Defect 글자
	CString		strAlarmCode;					// 영문 및 한글 defect

	FILE *fStream;
	errno_t e = _tfopen_s(&fStream, filename.GetString(), _T("r,ccs=UTF-8"));	// UNICODE"));
	if (e != 0) return;

	CStdioFile		file(fStream);
	while ( file.ReadString(sLine) )
	{
		if (sLine.IsEmpty() )	continue;			// 공백일 경우 무시

		sLine.Remove(_T(';'));
		::AfxExtractSubString( strAlarmName,		sLine	, 0, ',');		// Matching	Name
		::AfxExtractSubString( strAlarmCode,		sLine	, 1, ',');		// Kor Name(미사용)

		int nAlarmCode = _tstoi(strAlarmCode);
		m_AlarmMsg[nAlarmCode].strName[LAN_KOR] = strAlarmName;
		m_AlarmMsg[nAlarmCode].strName[LAN_ENG] = strAlarmName;
		m_AlarmMsg[nAlarmCode].strName[LAN_VNM] = strAlarmName;

	}//of while
#else
	FILE *fStream;
	errno_t e = _tfopen_s(&fStream, filename.GetString(), _T("r,ccs=UTF-8"));
	if (e != 0) return;
	CStdioFile		file(fStream);
	CString strBuf, strItem;
	int nIndex = -1;
	CTokenizer t;
	while(file.ReadString(strBuf))
	{
		// HEADER는 무시
		if(strBuf == _T("[HEADER]"))
			continue;

		if( (strBuf.Left(1) == _T("[")) && (strBuf.Right(1) == _T("]")) )
		{
			// SECTION Check
			strItem = strBuf.Trim(_T("[]"));
			if(strItem.GetLength() > 0)
			{
				nIndex = _tstoi(strItem.GetString());
				if(nIndex >= ALM_MAX)
					nIndex = -1;
			}
		}
		else if(nIndex >= 0)
		{
			// 그 외 나머지는 nIndex가 있을 때만 처리
			t = CTokenizer(strBuf);
			strItem = t.GetNextToken(_T("="));
			if(strItem == _T("ALARM_NAME_KOR"))
			{
				m_AlarmMsg[nIndex].strName[LAN_KOR] = t.GetNextToken(_T("="));
			}
			else if(strItem == _T("ALARM_NAME_ENG"))
			{
				m_AlarmMsg[nIndex].strName[LAN_ENG] = t.GetNextToken(_T("="));
			}
			else if(strItem == _T("ALARM_NAME_VNM"))
			{
				m_AlarmMsg[nIndex].strName[LAN_VNM] = t.GetNextToken(_T("="));
			}

			if(strItem == _T("CAUSE_KOR"))
			{
				m_AlarmMsg[nIndex].strCause[LAN_KOR] = t.GetNextToken(_T("="));
			}
			else if(strItem == _T("CAUSE_ENG"))
			{
				m_AlarmMsg[nIndex].strCause[LAN_ENG] = t.GetNextToken(_T("="));
			}
			else if(strItem == _T("CAUSE_VNM"))
			{
				m_AlarmMsg[nIndex].strCause[LAN_VNM] = t.GetNextToken(_T("="));
			}

			if(strItem == _T("METHOD_KOR"))
			{
				m_AlarmMsg[nIndex].strMethod[LAN_KOR] = t.GetNextToken(_T("="));
			}
			else if(strItem == _T("METHOD_ENG"))
			{
				m_AlarmMsg[nIndex].strMethod[LAN_ENG] = t.GetNextToken(_T("="));
			}
			else if(strItem == _T("METHOD_VNM"))
			{
				m_AlarmMsg[nIndex].strMethod[LAN_VNM] = t.GetNextToken(_T("="));
			}
		}
	}
#endif

	file.Close();

	return;
}
CONST_EQUIP_PART::ID CProcDataBank::GetEqiupPart_to_AlarmID( ALARM_ID id )
{
	if( id >= ALM_COMMON_PART )
	{
		return CONST_EQUIP_PART::EQUIP_PART_COMMON;
	}
	else if(id >= ALM_SHUTTLE3_PART)
	{
		return CONST_EQUIP_PART::EQUIP_PART_SHUTTLE3;
	}
	else if(id >= ALM_SHUTTLE2_PART)
	{
		return CONST_EQUIP_PART::EQUIP_PART_SHUTTLE2;
	}
	else
	{
		return CONST_EQUIP_PART::EQUIP_PART_SHUTTLE1;
	}
}

void CProcDataBank::AlarmMsgHappen( ALARM_ID id, CString strRemark /*= _T("")*/ )
{
	CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(FALSE, 
		m_AlarmMsg[id].strName[LAN_KOR], 
		m_AlarmMsg[id].strName[LAN_ENG],		
		m_AlarmMsg[id].strName[LAN_VNM], GetMainHandler()->m_nLangIdx );

	dlgMsgBox.DoModal();
}

void CProcDataBank::AlarmHappen( ALARM_ID id, CString strRemark /*= _T("") */ )
{
	if (GetMainHandler()==NULL)
	 	return;
	// 중복 발생 방지 위해 Mutex 적용 [9/27/2017 OSC]
	if( WaitForSingleObject(m_hMutex, 10000) != WAIT_OBJECT_0)
	{
		TRACE(_T("AddBuf Mutex Error"));
		return;
	}

	// 중복알람 허용... [9/27/2017 OSC]
// 	// 기존 Alarm 상태가 아닐 때만 상태 변경 후 상위보고
// 	if(m_AlarmID == ALM_NONE)
	{
	 
	 	m_AlarmID = id;
	 	m_AlarmPart = GetEqiupPart_to_AlarmID(m_AlarmID);		// 로더/검사/언로더부 중 어느곳에서 났는지...


	 	GetLocalTime(&m_AlarmTime);

		// 이전 에러 ErrorLog.csv 기록 [6/30/2017 OSC]
		theProcBank.WriteErrorLogCsv();

		alarm_info.iERR_ID = m_AlarmID;
		alarm_info.szERR_NAME = m_AlarmMsg[id].strName[GetMainHandler()->m_nLangIdx];
		alarm_info.TimeOccur = CTime(m_AlarmTime);
		alarm_info.szLevel = CStringSupport::N2C(m_AlarmMsg[id].nAlarmLevel);

		// Restart중에 난 거는 Level에 ALARM_LEVEL_CHECK로 기록
		// Restart중에는 실제 RUN 한 거로 치지 않는다
		if(GetMachineState() == CONST_MACHINE_STATE::MACHINE_CHECK)
		{
			alarm_info.szLevel = CStringSupport::N2C(ALARM_LEVEL_CHECK);

			ChangeMachineState(CONST_MACHINE_STATE::MACHINE_FAULT);
		}
		else
		{
			if(GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
			{
				// IDLE 상태 기록 [7/10/2017 OSC]
				theLossTimeLog.WriteLossTime_IDLE();
			}
			// Alarm보고는 설비 상태보고보다 먼저 발생해야 한다 [11/30/2016 OSC]
			theSocketInterFace.m_CIM.SendCmdAlarmToDataPC(m_AlarmID, TRUE, FALSE);
			theLossTimeLog.SetAlarmLossTime(m_AlarmID, m_AlarmMsg[id].strName[LAN_ENG]);

			ChangeMachineState(CONST_MACHINE_STATE::MACHINE_FAULT);
			theSocketInterFace.m_CIM.SendCmdStateToDataPC(EQUIP_SET_EQUIPMENT_STATUS_CHANGE, 
				E_EQST_MATCH_FAULT, _T(""));
		}
		theLog[LOG_ERROR].AddBuf(_T("Alarm Happen\t%d\t%s\t%s\t%s"), m_AlarmID, m_AlarmMsg[id].strName[GetMainHandler()->m_nLangIdx],strRemark, alarm_info.szLevel);
 
		GetMainHandler()->setModePM(FALSE);
	 	::PostMessage( GetMainWnd(), GxMSG_GUI_CHANGE_VIEW, (WPARAM)eFORM_Alarm, (LPARAM)0);

	}
// 	else
// 	{
// 	 	// 이미 Alarm상태면 로그만 기록
// 	 	theLog[LOG_ERROR_TEMP].AddBuf(_T("Alarm Mute\t%d\t%s\t%s\t%d"), id, m_AlarmMsg[id].strName[GetMainHandler()->m_nLangIdx],strRemark, m_AlarmMsg[id].nAlarmLevel);
// 			
// 	}
	::ReleaseMutex(m_hMutex);

}

void CProcDataBank::WarningHappen( ALARM_ID id, CString strRemark /*= _T("") */ )
{
	// 기존 Alarm 상태가 아닐 때만 상태 변경 후 상위보고
	if (GetMainHandler()==NULL)
		return;
	m_WarningID = id;
	theSocketInterFace.m_CIM.SendCmdAlarmToDataPC(m_WarningID, TRUE, TRUE);
	theLog[LOG_ERROR].AddBuf(_T("Warning Happen\t%d\t%s"), m_WarningID, m_AlarmMsg[id].strName[GetMainHandler()->m_nLangIdx]);

	// 경알람은 시간 0으로 바로 적는다 [9/27/2017 OSC]
	alarm_info.iERR_ID = m_WarningID;
	alarm_info.szERR_NAME = m_AlarmMsg[id].strName[GetMainHandler()->m_nLangIdx];
	alarm_info.TimeOccur = CTime::GetCurrentTime();	
	alarm_info.szLevel = CStringSupport::N2C(m_AlarmMsg[id].nAlarmLevel);
	theProcBank.WriteErrorLogCsv();

	::PostMessageA(GetMainWnd(), MSG_WARNING_SHOW, id, NULL);
}

void CProcDataBank::WarningClear()
{
	// 기존 Alarm 상태일 때만 상위보고
	if(m_WarningID != ALM_NONE)
	{
		// Alarm보고는 설비 상태보고보다 먼저 발생해야 한다 [11/30/2016 OSC]
		theSocketInterFace.m_CIM.SendCmdAlarmToDataPC(m_WarningID, FALSE, TRUE);

		theLog[LOG_ERROR].AddBuf(_T("Warning Clear\t%d"), m_WarningID);
		m_WarningID = ALM_NONE;
	}
}

//kjpark 20170225 LossTime ANI 보고 배끼기
CString CProcDataBank::GetLossTimeAlarmName()
{
	CString strAlarmName = _T("");

	if(theProcBank.m_AlarmPart == CONST_EQUIP_PART::EQUIP_PART_SHUTTLE1)
	{
		strAlarmName = _T("Shuttle 1");
	}
	else if(theProcBank.m_AlarmPart == CONST_EQUIP_PART::EQUIP_PART_SHUTTLE2)
	{
		strAlarmName = _T("Shuttle 2");
	}
	else if(theProcBank.m_AlarmPart == CONST_EQUIP_PART::EQUIP_PART_SHUTTLE3)
	{
		strAlarmName = _T("Shuttle 3");
	}
	else if(theProcBank.m_AlarmPart == CONST_EQUIP_PART::EQUIP_PART_COMMON)
	{
		strAlarmName = _T("Common");
	}

	return strAlarmName;
}


void CProcDataBank::AddDZoneSquareData(JIG_CH ch, int nPatternNo, SQUARE_POINT point, ZONE_ID zone)
{
	// 4개 존 다 받을 수 있게 되어 있지만 실제 B, D만 쓴다
	// 기존에 있던 패턴이면 좌표만 추가한다.
	for(int i = 0; i < m_nSquarePatternCnt[zone][ch]; i++)
	{
		if(m_SquarePatternInfo[zone][ch][i].nPatternNo == nPatternNo)
		{
			m_SquarePatternInfo[zone][ch][i].listSquarePoint.AddTail(point);
			return;
		}
	}
	// 기존에 없는 거면 패턴 카운트 증가 후 신규 등록
	if( m_nSquarePatternCnt[zone][ch] < MAX_DEFECT_POINT )
	{
		m_nSquarePatternCnt[zone][ch]++;
		m_SquarePatternInfo[zone][ch][m_nSquarePatternCnt[zone][ch]-1].nPatternNo = nPatternNo;
		m_SquarePatternInfo[zone][ch][m_nSquarePatternCnt[zone][ch]-1].listSquarePoint.AddTail(point);
	}
}

void CProcDataBank::ClearSquareData(JIG_ID jig)
{
// 	if(jig == JIG_ID_MAX)
// 	{
// 		for(int j = 0; j < JIG_ID_MAX; j++)
// 		{
// 			for(int ch = 0; ch < JIG_CH_MAX; ch++)
// 			{
// 				m_nSquarePatternCnt[j][ch] = 0;
// 				for(int i = 0; i < MAX_DEFECT_POINT; i++)
// 				{
// 					m_SquarePatternInfo[j][ch][i].listSquarePoint.RemoveAll();
// 				}
// 			}
// 		}
// 	}
// 	else
// 	{
// 		for(int ch = 0; ch < JIG_CH_MAX; ch++)
// 		{
// 			m_nSquarePatternCnt[jig][ch] = 0;
// 			for(int i = 0; i < MAX_DEFECT_POINT; i++)
// 			{
// 				m_SquarePatternInfo[jig][ch][i].listSquarePoint.RemoveAll();
// 			}
// 		}
// 	}
}

void CProcDataBank::AlarmClear()
{
	// 기존 Alarm 상태일 때만 상위보고
	if(m_AlarmID != ALM_NONE)
	{
		if(m_OldMachineState != CONST_MACHINE_STATE::MACHINE_CHECK)
		{
			// Alarm보고는 설비 상태보고보다 먼저 발생해야 한다 [11/30/2016 OSC]
			theSocketInterFace.m_CIM.SendCmdAlarmToDataPC(m_AlarmID, FALSE, FALSE);
		}

		// 경알람 예외처리 [9/26/2017 OSC]
		if(m_AlarmMsg[m_AlarmID].nAlarmLevel != ALARM_LEVEL_WARNING)
			ChangeMachineState(CONST_MACHINE_STATE::MACHINE_MAINT);

		theLog[LOG_ERROR].AddBuf(_T("Alarm Clear\t%d"), m_AlarmID);
		m_AlarmID = ALM_NONE;
	}
}

void CProcDataBank::ResetTemporarilyValue()
{
	for(int jig = 0 ; jig < JIG_ID_MAX ; jig++)
	{
		for(int i = 0 ; i < ZONE_ID_MAX ; i++)
		{
			m_bIsSetZone[jig][i] = FALSE;
			m_bIsZoneEnd[jig][i] = FALSE;
		}
		//kjpark 20170912 MCR 중복 리딩 방지
		m_bMCRStepisFirstStep[jig] = FALSE;
		m_bAZoneReadyPressed[jig] = FALSE;

		for(int i = 0 ; i < JIG_CH_MAX ; i++)
		{
			m_bAZoneCellSkip[jig][i] = FALSE;
// 			m_bAZoneChannelNotUse[jig][i] = FALSE;
			m_AZone[jig][i].Init();
			m_bAbleOnStatus[jig] = -1;	// 초기값
			m_OldUnloadTime[jig][i] = SYSTEMTIME();
			m_OldWaitTime[jig][i] = SYSTEMTIME();
		}
	}
	m_bDryRunMode = FALSE;

	ClearSquareData(JIG_ID_MAX);


	m_bEnableManualStateData = FALSE;
	for(int i = 0; i < JIG_CH_MAX; i++)
		m_ManualCell[i].Init();

	m_bPDT_IF_State_Clear = FALSE;

	m_UnitInterlockCnt = 0;
}

void CProcDataBank::AZoneChannelNotUse_OnOff( JIG_ID jig, JIG_CH ch, BOOL bOn )
{
	// 미사용을 하면 Skip도 같이 바꿔준다 [9/12/2017 OSC]
	m_bAZoneChannelNotUse[jig][ch] = bOn;
	m_bAZoneCellSkip[jig][ch] = bOn;

	if(bOn)
	{
		// TP보고는 안하기로 협의했기에 임의 정지는 하지 않는다. [11/30/2017 OSC]
// 		theUnitStatusBank.SetMoveStatus(jig, ch, STATUS_MOVE_PAUSE);
	}
	else
	{
		theUnitStatusBank.SetMoveStatus(jig, ch, STATUS_MOVE_RUNNING);
	}
}

BOOL CProcDataBank::AZoneChannelNotUse_Check( JIG_ID jig, JIG_CH ch )
{
	return m_bAZoneChannelNotUse[jig][ch];
}

void CProcDataBank::AZoneChannelNotUse_SyncUnitStatus()
{
	// 파일에서 읽어온 유닛 상태와 동기화
	for(int jig = 0 ; jig < JIG_ID_MAX ; jig++)
	{
		for(int i = 0 ; i < JIG_CH_MAX ; i++)
		{
			if(theUnitStatusBank.m_Unit[jig][i].m_MoveState == STATUS_MOVE_PAUSE)
				m_bAZoneChannelNotUse[jig][i] = TRUE;
			else if(theUnitStatusBank.m_Unit[jig][i].m_InterlockState == STATUS_INTERLOCK_ON)
				m_bAZoneChannelNotUse[jig][i] = TRUE;
			else
				m_bAZoneChannelNotUse[jig][i] = FALSE;
		}
	}

}

void CProcDataBank::AZoneCellNG_OnOff( JIG_ID jig, JIG_CH ch, BOOL bOn )
{
	m_bAZoneCellSkip[jig][ch] = bOn;
}

BOOL CProcDataBank::AZoneCellNG_Check( JIG_ID jig, JIG_CH ch )
{
	return m_bAZoneCellSkip[jig][ch];
}

void CProcDataBank::ShowLosstimeDlg()
{
	theLossTimeLog.ShowLossTimeDlg(FALSE);
}

void CProcDataBank::InitManualCellData( JIG_ID jig )
{
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		m_ManualCell[i].Init();
		m_ManualCell[i].defaultData.m_JigId = jig;
		m_ManualCell[i].defaultData.m_JigCh = (JIG_CH)i;

	}
}

void CProcDataBank::PDT_IF_NoCheckVacOff_Clear()
{
	for(int jig = JIG_ID_A ; jig < JIG_ID_MAX ; jig++)
	{
		for(int ch = JIG_CH_1 ; ch < JIG_CH_MAX ; ch++)
		{
			m_bPDT_IF_NoCheckVacOff[jig][ch] = FALSE;
		}
	}
}
