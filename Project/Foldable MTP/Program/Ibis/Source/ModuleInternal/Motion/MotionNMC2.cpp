#include "stdafx.h"
#include "MotionNMC2.h"
#include "IbisApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DWORD CNmc2::m_dwOpenState	= 0x00000000;  
CNmc2::CNmc2()
{
	for(int i = 0; i < MAX_INTRPOLATION_GROUP; i++)
		m_InterpolationAxis[i] = INTERPOLATION_AXIS_MAX;
}
CNmc2::~CNmc2()
{

}
BOOL CNmc2::OpenController(const int nCtrlID)
{
	m_nControllerID = nCtrlID;
	short ret = 0;
	ret = nmc_OpenDevice(nCtrlID);
	if(ret != NMC_OK)
	{
		CString errMsg;
		errMsg.Format(_T("nmc_opendevice ErrorCode %d"), ret);
		theLog[LOG_NMC].AddBuf(errMsg);
		return FALSE;
	}
	
	if(nmc_SetHomeDelay(m_nControllerID, 1000 ) != NMC_OK)
		return FALSE;

	return TRUE;
}
BOOL CNmc2::CloseController()
{
	nmc_CloseDevice(m_nControllerID);

	return TRUE;
}
BOOL CNmc2::GetAlarmSignal(const int nAxisNo, LONG& lVal)
{
	if(!CheckOpen())
		return FALSE;

	short sSig = 0;
	//20160919 kw
	short nRet = 0;
	NMCAXESEXPR nmcAxesExpr;
	if(nmc_GetAxesExpress(m_nControllerID,&nmcAxesExpr) != NMC_OK)
		return FALSE;
	else
	{
		lVal = nmcAxesExpr.nAlarm[nAxisNo];
	}

	return TRUE;
}
BOOL CNmc2::GetInPosSignal(const int nAxisNo, WORD& wVal)
{
	if(!CheckOpen())
		return FALSE;

	short sSig = 0;
	//20160919 kw
	short nRet = 0;
	NMCAXESEXPR nmcAxesExpr;
	if(nmc_GetAxesExpress(m_nControllerID,&nmcAxesExpr) != NMC_OK)
		return FALSE;
	else
	{
		wVal = nmcAxesExpr.nInpo[nAxisNo];
	}

	return TRUE;
}
BOOL CNmc2::GetCmdPos(const int nAxisNo, double& dPos)
{
	if(!CheckOpen())
		return FALSE;

	//long lPulse;
	int nPulse;
	short ret = nmc_GetCmdPos(m_nControllerID, nAxisNo, &nPulse);
	if(ret != NMC_OK)
	{
		//GetLogCtrl()->AddBuf(LOG_NMC, _T("GetCmdPos\tnmc_GetCmdPos\tret=%d"), ret);
		theLog[LOG_NMC].AddBuf(_T("GetCmdPos\tnmc_GetCmdPos\tret=%d"), ret);
		return FALSE;
	}
	dPos = double(nPulse);
	return TRUE;
}
BOOL CNmc2::GetEncPos(const int nAxisNo, double& dPos)
{
	if(!CheckOpen())
		return FALSE;

	int nPulse;
	short ret = nmc_GetEncPos(m_nControllerID, nAxisNo, &nPulse);
	if(ret != NMC_OK)
	{
		//GetLogCtrl()->AddBuf(LOG_NMC, _T("GetEncPos\tnmc_GetEncPos\tret=%d"), ret);
		theLog[LOG_NMC].AddBuf(_T("GetEncPos\tnmc_GetEncPos\tret=%d"), ret);
		return FALSE;
	}
	dPos = double(nPulse);
	return TRUE;
}
BOOL CNmc2::GetNmcData(NMC_AXES_EXPR& data)
{
	if(!CheckOpen())
		return FALSE;

	if(nmc_GetAxesExpress(m_nControllerID,&data) != NMC_OK)
		return FALSE;

	return TRUE;
}
BOOL CNmc2::ServoON( int nAxisNo )
{
	if(!CheckOpen())
		return FALSE;

	nmc_SetServoOn(m_nControllerID, nAxisNo, 1);
	return TRUE;
}

BOOL CNmc2::ServoOFF( int nAxisNo )
{
	if(!CheckOpen())
		return FALSE;

	nmc_SetServoOn(m_nControllerID, nAxisNo, 0);
	return TRUE;
}
BOOL CNmc2::AlarmClear(const int nAxisNo, BOOL bSet)
{
	if(!CheckOpen())
		return FALSE;

	//::nmc2_alarmreset_onoff_time(m_nControllerID, nAxisNo, 0, 100);// None Blocking function
	nmc_SetAlarmResetOn(m_nControllerID, nAxisNo, bSet);// None Blocking function

	return TRUE;
}
BOOL CNmc2::ZeroPointSet(const int nAxisNo, double dPos)
{
	if(!CheckOpen())
		return FALSE;

	//if(::nmc2_set_enc(m_nControllerID, nAxisNo, lPulse) != NMC_OK)	return FALSE;
	if(nmc_SetEncPos(m_nControllerID, nAxisNo, dPos) != NMC_OK)	return FALSE;
	//if(::nmc2_set_cmd(m_nControllerID, nAxisNo, lPulse) != NMC_OK)	return FALSE;
	if(nmc_SetCmdPos(m_nControllerID, nAxisNo, dPos) != NMC_OK)	return FALSE;

	return TRUE;
}
BOOL CNmc2::ZeroPointReturn(const int nAxisNo, int nHomeMode, double dStartSpeed, double dHomeSpeed, double dAccType /*= ACC_300ms*/, double dDecType /*= ACC_300ms*/, double dOffsetPos/*=0.*/)
{
	if(!CheckOpen())
		return FALSE;


	double dAcc;
	dAcc =  dHomeSpeed * dAccType;

	double dDec = dHomeSpeed * dDecType;

	// Limit 감지시 급정지로 변경
	if(nmc_SetLimitStopMode(m_nControllerID, nAxisNo, 0) != NMC_OK)
		return FALSE;

	if(nmc_SetSCurveSpeed(m_nControllerID, nAxisNo, dStartSpeed, dAcc, dDec, dHomeSpeed ) != NMC_OK)
		return FALSE;
	if(nmc_SetHomeSpeed(m_nControllerID, nAxisNo, dHomeSpeed, dHomeSpeed/5., dHomeSpeed/5. ) != NMC_OK)
		return FALSE;
// 	if(nmc_SetHomeDelay(m_nControllerID, 1000 ) != NMC_OK)
// 		return FALSE;

	//if(::nmc2_home_start(m_nControllerID, nAxisNo, nHomeMode, 3, lOffsetPulse) != NMC_OK)	return FALSE;
	if(nmc_HomeMove(m_nControllerID, nAxisNo, nHomeMode, 3, dOffsetPos,0) != NMC_OK)	return FALSE;
	return TRUE;
}
BOOL CNmc2::AxisStop( const int nAxisNo, BOOL bEms /*= FALSE*/ )
{
	short ret;
	if(bEms)
	{
		//ret = ::nmc2_stop_motor(m_nControllerID, nAxisNo);
		ret = nmc_SuddenStop(m_nControllerID, nAxisNo);
		if(ret != NMC_OK)
		{
			//GetLogCtrl()->AddBuf(LOG_NMC, _T("AxisStop\tnmc2_stop_motor\tret=%d"), ret);
			theLog[LOG_NMC].AddBuf(_T("AxisStop\tnmc_SuddenStop\tret=%d"), ret);
			return FALSE;
		}
	}
	else
	{
		//ret = ::nmc2_slow_stop_motor(m_nControllerID, nAxisNo);
		ret = nmc_DecStop(m_nControllerID, nAxisNo);
		if(ret != NMC_OK)
		{
			//GetLogCtrl()->AddBuf(LOG_NMC, _T("AxisStop\tnmc2_slow_stop_motor\tret=%d"), ret);
			theLog[LOG_NMC].AddBuf(_T("AxisStop\tnmc_DecStop\tret=%d"), ret);
			return FALSE;
		}
	}

	return TRUE;
}
BOOL CNmc2::AxisMove( const int nAxisNo, double dPos, double dStartSpeed, double dSpeed, double dAccType /*= ACC_300ms*/, double dDecType /*= ACC_300ms*/ )
{
	if(!CheckOpen())
	{
		TRACE(_T("  Open실패\n"));
		return FALSE;
	}


	double dAcc;
	dAcc =  dSpeed * dAccType;// acc time 조절 가능

	double dDec = dSpeed * dDecType;// dec time 조절 가능

	short retSet = nmc_SetSCurveSpeed (m_nControllerID, nAxisNo, dStartSpeed, dAcc, dDec, dSpeed);
	if(retSet != NMC_OK)
	{
		theLog[LOG_NMC].AddBuf(_T("AxisMove\tnmc_SetSCurveSpeed\tretSet=%d"), retSet);
		return FALSE;
	}
	short nPositionMode;
	nPositionMode = 1; //상대위치 = 0, 절대위치 = 1
	short ret = nmc_VelMove(m_nControllerID, nAxisNo, dPos, dSpeed, nPositionMode);
	if(ret != NMC_OK)
	{
		theLog[LOG_NMC].AddBuf(_T("AxisMove\tnmc_VelMove\tret=%d"), ret);
		return FALSE;
	}

	return TRUE;
}

void CNmc2::ChangeAxisSpeed( int nInnerAxisNo, double dSpeed )
{
	if(!CheckOpen())
	{
		TRACE(_T("  Open실패\n"));
		return;
	}

	//long lRtn = ::nmc2_drive_pps(m_nControllerID, nInnerAxisNo, lVelocity);
	long lRtn = nmc_SetOverrideDriveSpeed(m_nControllerID, nInnerAxisNo, dSpeed);
	
	if( lRtn != NMC_OK)
	{
		TRACE(_T("  nmc2_abs_scurve %d \n"), lRtn);
		return;
	}
}
BOOL CNmc2::CheckOpen()
{
	return ((m_dwOpenState>>(m_nControllerID-1))&0x1 ? TRUE : FALSE);
}
BOOL CNmc2::CheckServo(const int nAxisNo)
{
	short nState =0;

	nmc_GetAxesMotionOut(m_nControllerID, &NmcOut);
// 	return (NmcOut.nServoOn[nAxisNo] &0x1 ? FALSE : TRUE);
	return (NmcOut.nServoOn[nAxisNo]);

}
BOOL CNmc2::CheckBusy(const int nAxisNo)
{
	if(!CheckOpen())
		return FALSE;

	short sSig;
	//if(::nmc2_busy_signal(m_nControllerID, nAxisNo, &sSig) != NMC_OK)
	if(nmc_GetBusyStatus(m_nControllerID, nAxisNo, &sSig) != NMC_OK)
		return FALSE;

	return sSig==1 ? TRUE : FALSE;
}
BOOL CNmc2::CheckZRNC(const int nAxisNo)
{
	if(!CheckOpen())
		return FALSE;

	NMC_HOME_FLAG	homeFlag;
	//if(::nmc2_get_homeflag_all(m_nControllerID, &homeFlag) != NMC_OK)
	if(::nmc_GetHomeStatus(m_nControllerID, &homeFlag) != NMC_OK)
	return FALSE;

	int m = sizeof(NMC_HOME_FLAG);

	//return (/*homeFlag.nSrchFlag[nAxisNo]==1 &&*/homeFlag.nStatusFlag[nAxisNo]==HOME_READY) ? TRUE:FALSE;
	return (/*homeFlag.nSrchFlag[nAxisNo]==1 &&*/homeFlag.nStatusFlag[nAxisNo]==0) ? TRUE:FALSE;
}
BOOL CNmc2::CheckAxisReady(const int nAxisNo, BOOL bIgnoreInposition)
{
	short ret;
	if(!CheckOpen())
		return FALSE;
	ret = nmc_GetAxesExpress(m_nControllerID,&m_nmcData);
	if( ret != NMC_OK)
	{
		//GetLogCtrl()->AddBuf(LOG_NMC, _T("CheckAxisReady\tnmc2_get_sensor_all\tret=%d"), ret);
		theLog[LOG_NMC].AddBuf(_T("CheckAxisReady\tnmc_GetAxesExpress\tret=%d"), ret);
		return FALSE;
	}

	//nError = Limit or Alarm or Emegency 가 온일대 On
	if(bIgnoreInposition)
	{
		if(m_nmcData.nAlarm[nAxisNo] || m_nmcData.nBusy[nAxisNo])
		{
			TRACE(_T("Control=%d,Axis=%d, %d,%d,%d\n"), m_nControllerID ,nAxisNo, m_nmcData.nAlarm[nAxisNo], m_nmcData.nBusy[nAxisNo] , m_nmcData.nInpo[nAxisNo]);
			return FALSE;
		}
	}
	else
	{
		if(m_nmcData.nAlarm[nAxisNo] || m_nmcData.nBusy[nAxisNo] || m_nmcData.nInpo[nAxisNo] == 0)
		{
			TRACE(_T("Control=%d,Axis=%d, %d,%d,%d\n"), m_nControllerID ,nAxisNo, m_nmcData.nAlarm[nAxisNo], m_nmcData.nBusy[nAxisNo] , m_nmcData.nInpo[nAxisNo]);
			return FALSE;
		}
	}
	return TRUE;
}
BOOL CNmc2::CheckTrouble(const int nAxisNo)
{
	// Return Value
	// TRUE = Trouble
	// FALSE = Not Trouble

	if(!CheckOpen())
		return TRUE;


	if(nmc_GetAxesExpress(m_nControllerID,&m_nmcData) != NMC_OK)
		return TRUE;

	//	if(data.nAlarm[nAxisNo] || data.nMLimit[nAxisNo] || data.nPLimit[nAxisNo])
	if(m_nmcData.nError[nAxisNo])// 위와 동일함.
		return TRUE;
	return FALSE;
}
BOOL CNmc2::CheckPLimit( const int nAxisNo )
{
	NMC_AXES_EXPR data;
	GetNmcData(data);
	return data.nPLimit[nAxisNo];
}

BOOL CNmc2::CheckNLimit( const int nAxisNo )
{
	NMC_AXES_EXPR data;
	GetNmcData(data);
	return data.nMLimit[nAxisNo];
}

BOOL CNmc2::GetTargetPostion(short nAxisNo,double &pTargetPos)
{
	if(!CheckOpen())
		return FALSE;

	short ret = nmc_GetParaTargetPos(m_nControllerID, nAxisNo, &pTargetPos);
	if(ret != NMC_OK)
	{
		//GetLogCtrl()->AddBuf(LOG_NMC, _T("GetEncPos\tnmc_GetEncPos\tret=%d"), ret);
		theLog[LOG_NMC].AddBuf(_T("GetTargetPostion\tnmc_GetParaTargetPos\tret=%d"), ret);
		return FALSE;
	}
	return TRUE;
}
void CNmc2::Interpolation_BeginListCreate( int nGroupNo, INTERPOLATION_AXIS axisType, int nAxis1No, int nAxis2No, int nAxis3No, double dMaxSpeed )
{
	if(nGroupNo >= MAX_INTRPOLATION_GROUP)
	{
		theLog[LOG_NMC].AddBuf(_T("Interpolation_BeginListCreate \t Group Index Over %d"), nGroupNo);
		return;
	}

	m_InterpolationAxis[nGroupNo] = axisType;
	if(m_InterpolationAxis[nGroupNo] == INTERPOLATION_2AXIS)
		nAxis3No = -1;

	short ret = nmc_ContiSetMode(m_nControllerID, nGroupNo, 0, 0, nAxis1No, nAxis2No, nAxis3No, dMaxSpeed, 2, 2, 2);
	if(ret != NMC_OK)
	{
		theLog[LOG_NMC].AddBuf(_T("Interpolation_BeginListCreate \t nmc_ContiSetMode \t ret=%d"), ret);
	}
}

void CNmc2::Interpolation_AddPosition( int nGroupNo, INTERPOLATION_POSITION pos )
{
	short ret;
	// 3축 선형 보간이동
	if(m_InterpolationAxis[nGroupNo] == INTERPOLATION_3AXIS)
	{
		// 3축 보간이면 원호보간을 할 수 없다...
		if(pos.type == INTERPOLATION_ARC)
		{
			theLog[LOG_NMC].AddBuf(_T("Interpolation_AddPosition \t Can not added Arc"));
			return;
		}

		ret = nmc_ContiAddNodeLine3Axis(m_nControllerID, nGroupNo, pos.dPos1, pos.dPos2, pos.dPos3, pos.dStartSpeed, pos.dAcc, pos.dDec, pos.dSpeed, 2);
		if(ret != NMC_OK)
		{
			theLog[LOG_NMC].AddBuf(_T("Interpolation_AddPosition \t nmc_ContiAddNodeLine3Axis \t ret=%d"), ret);
		}
	}
	else
	{
		// 2축 원호 보간이동
		if(pos.type == INTERPOLATION_ARC)
		{
			ret = nmc_ContiAddNodeArc(m_nControllerID, nGroupNo, pos.dCenter1, pos.dCenter2, pos.dAngle, pos.dStartSpeed, pos.dAcc, pos.dDec, pos.dSpeed, 2);
			if(ret != NMC_OK)
			{
				theLog[LOG_NMC].AddBuf(_T("Interpolation_AddPosition \t nmc_ContiAddNodeArc \t ret=%d"), ret);
			}
		}
		// 2축 선형 보간이동
		else
		{
			ret = nmc_ContiAddNodeLine2Axis(m_nControllerID, nGroupNo, pos.dPos1, pos.dPos2, pos.dStartSpeed, pos.dAcc, pos.dDec, pos.dSpeed, 2);
			if(ret != NMC_OK)
			{
				theLog[LOG_NMC].AddBuf(_T("Interpolation_AddPosition \t nmc_ContiAddNodeLine2Axis \t ret=%d"), ret);
			}
		}
	}
}

void CNmc2::Interpolation_EndListCreate( int nGroupNo )
{
	short ret = nmc_ContiSetCloseNode(m_nControllerID, nGroupNo);
	if(ret != NMC_OK)
	{
		theLog[LOG_NMC].AddBuf(_T("Interpolation_EndListCreate \t nmc_ContiSetCloseNode \t ret=%d"), ret);
	}
}

void CNmc2::Interpolation_ClearList( int nGroupNo )
{
	m_InterpolationAxis[nGroupNo] = INTERPOLATION_AXIS_MAX;
	short ret = nmc_ContiSetNodeClear(m_nControllerID, nGroupNo);
	if(ret != NMC_OK)
	{
		theLog[LOG_NMC].AddBuf(_T("Interpolation_ClearList \t nmc_ContiSetNodeClear \t ret=%d"), ret);
	}
}

void CNmc2::Interpolation_RunStop( int nGroupNo, BOOL bRun )
{
	short ret = nmc_ContiRunStop(m_nControllerID, nGroupNo, bRun);
	if(ret != NMC_OK)
	{
		theLog[LOG_NMC].AddBuf(_T("Interpolation_RunStop \t nmc_ContiRunStop \t ret=%d"), ret);
	}
}

BOOL CNmc2::Interpolation_IsStop( int nGroupNo )
{
	NMCCONTISTATUS status;
	short ret = nmc_ContiGetStatus(m_nControllerID, &status);
	if(ret != NMC_OK)
	{
		theLog[LOG_NMC].AddBuf(_T("Interpolation_IsStop \t nmc_ContiGetStatus \t ret=%d"), ret);
	}
	// 연속 보간 실행 여부(1:Run, 0:Stop)
	return (status.nContiRun[nGroupNo] == 0) ? TRUE:FALSE;
}

void CNmc2::SetGantryAxis( BOOL bEnable, int nAxisMaster, int nAxisSlave )
{
	if(bEnable)
	{
		// 서로 다른 그룹의 축이면 구현 동기화 불가
		if(NMCGroupNo(nAxisMaster) != NMCGroupNo(nAxisSlave))
			return;

		short ret = nmc_SetGantryAxis(m_nControllerID, NMCGroupNo(nAxisMaster), nAxisMaster, nAxisSlave);
		if(ret != NMC_OK)
		{
			theLog[LOG_NMC].AddBuf(_T("SetGantryAxis \t nmc_SetGantryAxis \t ret=%d"), ret);
			return;
		}
	}

	short ret = nmc_SetGantryEnable(m_nControllerID, NMCGroupNo(nAxisMaster), bEnable);
	if(ret != NMC_OK)
	{
		theLog[LOG_NMC].AddBuf(_T("SetGantryAxis \t nmc_SetGantryEnable \t ret=%d"), ret);
		return;
	}
}
