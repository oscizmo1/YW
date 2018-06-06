#include "StdAfx.h"
#include "InspModule_MTPWrite.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CInspModule_MTPWrite::CInspModule_MTPWrite(void)
	: CInspModule_Base()
{
	m_ModuleName = INSP_MODULE_MTP_WRITE;
}


CInspModule_MTPWrite::~CInspModule_MTPWrite(void)
{
}

int CInspModule_MTPWrite::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;
	ResetReturnValue();
	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:		
// 		m_bRtn[0] = m_Func.LightCurtain_Check(m_Shuttle);
// 		m_bRtn[1] = m_Func.Shuttle_Y_INSP_Check(m_Shuttle);
		if(IsReturnOk())
		{
			// State Name을 모듈 이름 포함해서 다시 지정한다 [9/19/2017 OSC]
			m_strStateName += m_ModuleName;
			theLog[m_LogIndex].AddBuf(_T("[%s] stepStart"), m_strStateName);

// 			m_Func.Inspection_X_INSP_Move(m_Shuttle);
			m_Timer.Start();		
			nStep++;
		}
// 		else if(m_Timer.Stop(FALSE) > 1.)
// 		{
// 			// 항상 모터 체크보다 Light Curtain을 먼저 체크해서 우선권을 둔다 [9/27/2017 OSC]
// 			if(m_bRtn[0] == FALSE)			SetError(m_Func.GetAlarmID_of_Shuttle(ALM_SHUTTLE1_LIGHT_CURTAIN, m_Shuttle));
// 			else if(m_bRtn[1] == FALSE)		SetError(m_Func.GetAlarmID_of_Shuttle(ALM_AXIS_JIG_SHUTTLE_Y1, m_Shuttle));
// 		}
		break;
// 	//kjpark 20170925 MTP WRITE 할때 X축 동작 
// 	case stepZ_Down:
// 		m_bRtn[0] = m_Func.LightCurtain_Check(m_Shuttle);
// 		m_bRtn[1] = m_Func.Inspection_X_INSP_Check(m_Shuttle);
// 		if(IsReturnOk())
// 		{
// 			m_Func.Inspection_Z_INSP_Move(m_Shuttle);
// 			theTactTimeLog.m_InspectionZ_Down[m_Shuttle].Start();
// 			theLog[m_LogIndex].AddBuf(_T("[%s] stepZ_Down"), m_strStateName);
// 			m_Timer.Start();		
// 			nStep++;
// 		}	
// 		else if(m_Timer.Stop(FALSE) > 5.)
// 		{
// 			// 항상 모터 체크보다 Light Curtain을 먼저 체크해서 우선권을 둔다 [9/27/2017 OSC]
// 			if(m_bRtn[0] == FALSE)			SetError(m_Func.GetAlarmID_of_Shuttle(ALM_SHUTTLE1_LIGHT_CURTAIN, m_Shuttle));
// 			else if(m_bRtn[1] == FALSE)		SetError(m_Func.GetAlarmID_of_Shuttle(ALM_AXIS_INSPECTION_X1, m_Shuttle));
// 		}
// 		break;
	case stepZ_DownChk:		
		m_bRtn[0] = m_Func.LightCurtain_Check(m_Shuttle);
		m_bRtn[1] = m_Func.Inspection_Z_INSP_Check(m_Shuttle);
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepZ_DownChk"), m_strStateName);
			theTactTimeLog.m_InspectionZ_Down[m_Shuttle].End();

			// MTP 검사 [8/10/2017 OSC]
			m_Func.MTPWrite_Send(m_Shuttle);
			m_Timer.Start();		
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			// 항상 모터 체크보다 Light Curtain을 먼저 체크해서 우선권을 둔다 [9/27/2017 OSC]
			if(m_bRtn[0] == FALSE)			SetError(m_Func.GetAlarmID_of_Shuttle(ALM_SHUTTLE1_LIGHT_CURTAIN, m_Shuttle));
			else if(m_bRtn[1] == FALSE)		SetError(m_Func.GetAlarmID_of_Shuttle(ALM_AXIS_INSPECTION_Z1, m_Shuttle));
		}
		break;

	case stepMtpWrite_EndChk:		
		m_bRtn[0] = m_Func.GetCellSkipCheck(m_Shuttle, JIG_CH_1) ? m_Func.MTPWrite_Check(m_Shuttle, JIG_CH_1):TRUE;
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepMtpWrite_EndChk"), m_strStateName);

// 			m_Func.MTPVerify_Send(m_Shuttle);
			m_Timer.Start();		
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > theConfigBank.m_TimeOut.m_dMTPWrite)
		{
			m_Func.MTPWrite_TimeOut(m_Shuttle);
		}
		break;

// 	case stepMtpVerify_EndChk:		
// 		m_bRtn[0] = m_Func.GetCellSkipCheck(m_Shuttle, JIG_CH_1) ? m_Func.MTPVerify_Check(m_Shuttle, JIG_CH_1):TRUE;
// 		m_bRtn[1] = m_Func.GetCellSkipCheck(m_Shuttle, JIG_CH_2) ? m_Func.MTPVerify_Check(m_Shuttle, JIG_CH_2):TRUE;
// 		if(IsReturnOk())
// 		{
// 			theLog[m_LogIndex].AddBuf(_T("[%s] stepMtpVerify_EndChk"), m_strStateName);
// 
// 			// Z축 상승 [8/10/2017 OSC]
// 			m_Func.Inspection_Z_UP_Move(m_Shuttle);
// 			m_Timer.Start();		
// 			nStep++;
// 		}
// 		else if(m_Timer.Stop(FALSE) > 5.)
// 		{
// 			m_Func.MTPVerify_TimeOut(m_Shuttle);
// 		}
// 		break;

	case stepEnd:				
// 		m_bRtn[0] = m_Func.Inspection_Z_UP_Check(m_Shuttle);
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepEnd"), m_strStateName);
			nStep = stepIdle;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			SetError(m_Func.GetAlarmID_of_Shuttle(ALM_AXIS_INSPECTION_Z1, m_Shuttle));
		}
		break;

	}

	m_nStep = nStep;

	return nStep;
}
