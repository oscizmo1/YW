#include "StdAfx.h"
#include "InspModule_MTPVerify.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CInspModule_MTPVerify::CInspModule_MTPVerify(void)
	: CInspModule_Base()
{
	m_ModuleName = INSP_MODULE_MTP_VERIFY;
}


CInspModule_MTPVerify::~CInspModule_MTPVerify(void)
{
}

int CInspModule_MTPVerify::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;
	ResetReturnValue();
	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:		
		m_bRtn[0] = m_Func.LightCurtain_Check(m_Shuttle);
		m_bRtn[1] = m_Func.Shuttle_Y_INSP_Check(m_Shuttle);
		m_bRtn[2] = m_Func.Inspection_Z_INSP_Check(m_Shuttle);
		if(IsReturnOk())
		{
			// State Name�� ��� �̸� �����ؼ� �ٽ� �����Ѵ� [9/19/2017 OSC]
			m_strStateName += m_ModuleName;
			theLog[m_LogIndex].AddBuf(_T("[%s] stepStart"), m_strStateName);

			// MTP �˻� [8/10/2017 OSC]
			m_Func.MTPVerify_Send(m_Shuttle);
			m_Timer.Start();		
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 1.)
		{
			// �׻� ���� üũ���� Light Curtain�� ���� üũ�ؼ� �켱���� �д� [9/27/2017 OSC]
			if(m_bRtn[0] == FALSE)		SetError(m_Func.GetAlarmID_of_Shuttle(ALM_SHUTTLE1_LIGHT_CURTAIN, m_Shuttle));
			else if(m_bRtn[1] == FALSE)	SetError(m_Func.GetAlarmID_of_Shuttle(ALM_AXIS_JIG_SHUTTLE_Y1, m_Shuttle));
			else if(m_bRtn[2] == FALSE) SetError(m_Func.GetAlarmID_of_Shuttle(ALM_AXIS_INSPECTION_Z1, m_Shuttle));
		}
		break;

	case stepMtpVerify_EndChk:		
		m_bRtn[0] = m_Func.GetCellSkipCheck(m_Shuttle, JIG_CH_1) ? m_Func.MTPVerify_Check(m_Shuttle, JIG_CH_1):TRUE;
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepMtpVerify_EndChk"), m_strStateName);

			// Z�� ��� [8/10/2017 OSC]
// 			m_Func.Inspection_Z_UP_Move(m_Shuttle);
// 			theTactTimeLog.m_InspectionZ_Up[m_Shuttle].Start();
			m_Timer.Start();		
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > theConfigBank.m_TimeOut.m_dMTPVerify)
		{
			m_Func.MTPVerify_TimeOut(m_Shuttle);
		}
		break;

	case stepEnd:				
// 		m_bRtn[0] = m_Func.LightCurtain_Check(m_Shuttle);
// 		m_bRtn[1] = m_Func.Inspection_Z_UP_Check(m_Shuttle);
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepEnd"), m_strStateName);
			theTactTimeLog.m_InspectionZ_Up[m_Shuttle].End();
			nStep = stepIdle;
		}
// 		else if(m_Timer.Stop(FALSE) > 5.)
// 		{
// 			// �׻� ���� üũ���� Light Curtain�� ���� üũ�ؼ� �켱���� �д� [9/27/2017 OSC]
// 			if(m_bRtn[0] == FALSE)		SetError(m_Func.GetAlarmID_of_Shuttle(ALM_SHUTTLE1_LIGHT_CURTAIN, m_Shuttle));
// 			else if(m_bRtn[1] == FALSE)	SetError(m_Func.GetAlarmID_of_Shuttle(ALM_AXIS_INSPECTION_Z1, m_Shuttle));
// 		}
		break;

	}

	m_nStep = nStep;

	return nStep;
}
