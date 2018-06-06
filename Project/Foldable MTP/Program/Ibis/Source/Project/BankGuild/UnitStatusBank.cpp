#include "StdAfx.h"
#include "UnitStatusBank.h"
#include "Etc/FileSupport.h"
#include "Etc/Ini.h"


CUnitStatusBank::CUnitStatusBank(void)
{
}


CUnitStatusBank::~CUnitStatusBank(void)
{
}

void CUnitStatusBank::SetUnitID(CString strEQPID)
{
	int nFullCh;
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		for(int ch = 0; ch < JIG_CH_MAX; ch++)
		{
			nFullCh = ((jig*JIG_CH_MAX) + ch)+1;
			m_Unit[jig][ch].m_strUnitID.Format(_T("%s_CN%02d"), strEQPID, nFullCh);
		}
	}
}

BOOL CUnitStatusBank::FindUnit(CString strUnitID, JIG_ID &jig, JIG_CH &ch)
{
	jig = JIG_ID_MAX;
	ch = JIG_CH_MAX;
	for(int i = 0; i < JIG_ID_MAX; i++)
	{
		for(int j = 0; j < JIG_CH_MAX; j++)
		{
			if(m_Unit[i][j].m_strUnitID == strUnitID)
			{
				jig = (JIG_ID)i;
				ch = (JIG_CH)j;
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CUnitStatusBank::SetCellInfo(JIG_ID jig, JIG_CH ch, CCellInfo *pCell)
{
	m_Unit[jig][ch].m_pCell = pCell;
}

void CUnitStatusBank::SetMoveStatus(JIG_ID jig, JIG_CH ch, STATUS_MOVE value)
{
	if(m_Unit[jig][ch].SetMove(value))
	{
		theSocketInterFace.m_CIM.SendUnitStatusChange(jig, ch, m_Unit[jig][ch]);
	}
}

void CUnitStatusBank::SetInterlock(CString strUnitID)
{
	JIG_ID jig;
	JIG_CH ch;
	if(FindUnit(strUnitID, jig, ch))
	{
		if(m_Unit[jig][ch].SetInterlock(STATUS_INTERLOCK_ON))
		{
			theProcBank.AZoneChannelNotUse_OnOff(jig, ch, TRUE);
			::PostMessage(GetMainWnd(), MSG_SHOW_DIALOG, eDLG_MsgUnitInterlock, NULL);
			theSocketInterFace.m_CIM.SendUnitStatusChange(jig, ch, m_Unit[jig][ch]);
			theUnitFunc.TowerLamp_Change(theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_INTERLOCK].Red.lamp_state
				,theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_INTERLOCK].Yellow.lamp_state
				, theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_INTERLOCK].Green.lamp_state
				,TRUE);
		}
		m_Unit[jig][ch].SetAvilability(STATUS_AVAILABILITY_UP);
	}
}

void CUnitStatusBank::SetInterlockCinform()
{
	// 인터락 발생했던 것들을 모두 컨펌한다.
	for(int i = 0; i < JIG_ID_MAX; i++)
	{
		for(int j = 0; j < JIG_CH_MAX; j++)
		{
			if(m_Unit[i][j].m_InterlockState == STATUS_INTERLOCK_ON)
			{
				//kjpark 20180120 유닛인터락 발생 후 다른채널에서 발생하면 이전꺼까지 보고하는 버그 수정
				if(theSocketInterFace.m_CIM.SendUnitStatusInterlockConfirm((JIG_ID)i, (JIG_CH)j, m_Unit[i][j].m_pCell))
					m_Unit[i][j].m_InterlockState = STATUS_INTERLOCK_OFF;
			}
		}
	}
}

void CUnitStatusBank::SaveStatus()
{
	CString strPath,strBackupPath;;
	strPath.Format(_T("%s\\UnitStatus.ini"), theConfigBank.m_System.m_strConfigPath);
	CString strFolderPath = CFileSupport::GetPathName(strPath);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return;
	}

	CIni ini(strPath);
	CString strKey;
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		for(int ch = 0; ch < JIG_CH_MAX; ch++)
		{
			strKey.Format(_T("%cJIG_CH%d"), jig+_T('A'), ch+1);
			ini.WriteString(_T("STATUS")	,strKey,  m_Unit[jig][ch].ToString() );
		}
	}
}

void CUnitStatusBank::ReadStatus()
{
	CString strPath,strBackupPath;;
	strPath.Format(_T("%s\\UnitStatus.ini"), theConfigBank.m_System.m_strConfigPath);
	CString strFolderPath = CFileSupport::GetPathName(strPath);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return;
	}

	CIni ini(strPath);
	CString strKey, strValue;
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		for(int ch = 0; ch < JIG_CH_MAX; ch++)
		{
			strKey.Format(_T("%cJIG_CH%d"), jig+_T('A'), ch+1);
			strValue = ini.GetString(_T("STATUS")	,strKey);
			if(strValue.IsEmpty() == FALSE)
				m_Unit[jig][ch].FromString(strValue);
		}
	}
}

void CUnitStatusBank::StatusInit()
{
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		for(int ch = 0; ch < JIG_CH_MAX; ch++)
		{
			m_Unit[jig][ch].m_AvilabilityState = STATUS_AVAILABILITY_UP;
			m_Unit[jig][ch].m_RunState = STATUS_RUN_NOT_CELL;
		}
	}
}
