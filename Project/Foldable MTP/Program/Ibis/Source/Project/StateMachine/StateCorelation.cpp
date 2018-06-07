#include "stdafx.h"
#include "StateCorelation.h"
#include "IbisApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CStateCorelation::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{

	case stepStart:
		// Y축이 도피되어 있으면 step Jump [11/7/2017 OSC]
		m_bRtn[0] = Shuttle_Y_LOAD_Check(JIG_ID_A);
		m_bRtn[1] = Shuttle_Y_LOAD_Check(JIG_ID_B);
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepStart -> stepShuttleY_AZone_Check"), m_strStateName);
			m_Timer.Start();
			nStep = stepShuttleY_AZone_Check;
		}
		else
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepStart"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
	
		// Z축 Up
	case stepInspectionZ_UpMove:
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepInspectionZ_UpMove"), m_strStateName);
			Inspection_Z_UP_Move(JIG_ID_A);
			Inspection_Z_UP_Move(JIG_ID_B);
			m_Timer.Start();
			nStep++;
		}
		break;

		// Z축 Up 체크 -> Y Load Move
	case stepInspectionZ_UpCheck:
		m_bRtn[0] = Inspection_Z_UP_Check(JIG_ID_A);
		m_bRtn[1] = Inspection_Z_UP_Check(JIG_ID_B);
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepInspectionZ_UpCheck"), m_strStateName);
			Shuttle_Y_LOAD_Move(JIG_ID_A);
			Shuttle_Y_LOAD_Move(JIG_ID_B);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALM_AXIS_INSPECTION_Z1);
			else if(m_bRtn[1] == FALSE)	SetError(ALM_AXIS_INSPECTION_Z2);
		}
		break;

		// Y Load 체크 -> Z Down
	case stepShuttleY_AZone_Check:
		m_bRtn[0] = Shuttle_Y_LOAD_Check(JIG_ID_A);
		m_bRtn[1] = Shuttle_Y_LOAD_Check(JIG_ID_B);
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepShuttleY_AZone_Check"), m_strStateName);
			Inspection_Z_INSP_Move(JIG_ID_A);
			Inspection_Z_INSP_Move(JIG_ID_B);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALM_AXIS_JIG_SHUTTLE_Y1);
			else if(m_bRtn[1] == FALSE)	SetError(ALM_AXIS_JIG_SHUTTLE_Y2);
		}
		break;

		// Z축 Down 체크
	case stepInspectionZ_DownCheck:
		m_bRtn[0] = Inspection_Z_INSP_Check(JIG_ID_A);
		m_bRtn[1] = Inspection_Z_INSP_Check(JIG_ID_B);
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepInspectionZ_DownCheck"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALM_AXIS_INSPECTION_Z1);
			else if(m_bRtn[1] == FALSE)	SetError(ALM_AXIS_INSPECTION_Z2);
		}
		break;


	case stepEnd:

		theLog[m_LogIndex].AddBuf(_T("[%s] stepEnd"), m_strStateName);
		nStep = stepIdle;
		break;
	}

	if(m_nStep != stepIdle)
		m_nStep = nStep;

	if( m_bManualSequence && (m_nStep == stepIdle) )
	{
		CGxMsgBox dlgMsgBox;
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("COMPLETE."), 
			_T("COMPLETE"),		
			_T("COMPLETE") , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();
	}
	return nStep;
}
