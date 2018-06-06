#include "StdAfx.h"
#include "DeviceMotion.h"
#include "IbisApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void AxisMoveParam::operator=( const TEACH_PARAM &srcParam )
{
	idAxis = srcParam.idAxis;
	dTargetPosition = srcParam.dPos;
	dSpeed = srcParam.dSpeed;
	dAcc = srcParam.dAcc;
	dDec = srcParam.dDec;
}

CDeviceMotion::CDeviceMotion(void)
{
	m_nChainInterpolationDevice = -1;
}

CDeviceMotion::~CDeviceMotion(void)
{
}

BOOL CDeviceMotion::Open_Mp2100( MP2100_THREAD thr )
{
	for(int board=0; board<MP2100_BOARD_MAX; board++)
	{
		if(m_Mp2100[thr][board].OpenController(board)!=MP_SUCCESS)
			return FALSE;
		CMp2100::m_dwOpenState |= 0x1<<thr*board; 
	}

	return TRUE;
}

void CDeviceMotion::Close_Mp2100()
{
	if(CMp2100::m_dwOpenState!=0)
	{
		//for(int th=0; th<MP2100_THREAD_MAX; th++)
		for(int th=0; th<1; th++)// Sample 에서도 1번만 close 해준다.
		{
			for(int board=0; board<MP2100_BOARD_MAX; board++)
			{
				if(!m_Mp2100[th][board].CloseController())
					return;
			}
		}
		CMp2100::m_dwOpenState = 0;
	}

}

BOOL CDeviceMotion::Open_NMC2()
{
	int nMaxCount = NMC_CTRL_MAX;

	for(int nCtrl=0; nCtrl<nMaxCount; nCtrl++)
	{
		if(!m_Nmc2[nCtrl].OpenController(nCtrl+1))
			return FALSE;
		CNmc2::m_dwOpenState |= 0x1<<nCtrl; 
	}

	return TRUE;
}

void CDeviceMotion::Close_NMC2()
{
	if(CNmc2::m_dwOpenState!=0)
	{
		int nMaxCount = NMC_CTRL_MAX;

		for(int nCtrl=0; nCtrl<nMaxCount; nCtrl++)
		{
			if(!m_Nmc2[nCtrl].CloseController())
				return;
		}
		CNmc2::m_dwOpenState = 0;
	}

}

BOOL CDeviceMotion::ServoOn( MP2100_THREAD thr, AXIS_ID axis )
{
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		return TRUE;
	}
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		return m_Mp2100[thr][nDevice].ServoON(nInnerAxis);
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		if(theAxisBank.m_Axis[axis].m_MotorType == CAxisParam::MOTOR_TYPE_SERVO)
			return m_Nmc2[nDevice].ServoON(nInnerAxis);
		else if(theAxisBank.m_Axis[axis].m_MotorType == CAxisParam::MOTOR_TYPE_EZ_SERVO)
			return m_Nmc2[nDevice].ServoOFF(nInnerAxis);	// EZ 서보는 반대다
		// 스텝은 서보온이 필요없다
	}

	return FALSE;
}
BOOL CDeviceMotion::ServoOff(MP2100_THREAD thr, AXIS_ID axis)
{
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
		return TRUE;

	BOOL bRtn = FALSE;
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		bRtn = m_Mp2100[thr][nDevice].ServoOFF(nInnerAxis);
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		if(theAxisBank.m_Axis[axis].m_MotorType == CAxisParam::MOTOR_TYPE_SERVO)
			bRtn = m_Nmc2[nDevice].ServoOFF(nInnerAxis);
		else if(theAxisBank.m_Axis[axis].m_MotorType == CAxisParam::MOTOR_TYPE_EZ_SERVO)
			bRtn = m_Nmc2[nDevice].ServoON(nInnerAxis);	// EZ 서보는 반대다
		// 스텝은 서보온이 필요없다

		bRtn  = m_Nmc2[nDevice].AxisStop(nInnerAxis);
	}
	return bRtn;
}
BOOL CDeviceMotion::ResetAlarm(MP2100_THREAD thr, AXIS_ID axis)
{
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
		return FALSE;

	BOOL bRtn = FALSE;
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		bRtn = m_Mp2100[thr][nDevice].BoardAlarmClear();
		Sleep(100);
		bRtn = m_Mp2100[thr][nDevice].AlarmClear(nInnerAxis);
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		bRtn = m_Nmc2[nDevice].AlarmClear(nInnerAxis, TRUE);
		Sleep(200);
		bRtn = m_Nmc2[nDevice].AlarmClear(nInnerAxis, FALSE);
	}

	return bRtn;
}
void CDeviceMotion::SetPosition(MP2100_THREAD thr, AXIS_ID axis, double dPosition)	// 강제로 현재 위치값을 바꾼다
{
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		return;
	}
	BOOL bRtn;
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;

	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		bRtn = m_Mp2100[thr][nDevice].ZeroPointSet(nInnerAxis, dPosition);
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		CAxisParam	nodeM;
		theAxisBank.GetParamNode(axis, nodeM);
		double dPulse;

		if(nodeM.m_dGearRatio>0.f)
			dPulse = PosToPulse(dPosition, nodeM.m_dGearRatio);
		else
			dPulse = double(dPosition);

		bRtn = m_Nmc2[nDevice].ZeroPointSet(nInnerAxis, dPulse);
	}
	if(!bRtn)
	{
		if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
		{
			theLog[LOG_MP2100].AddBuf(_T("%s failed SetPostion"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
		{
			theLog[LOG_NMC].AddBuf(_T("%s failed SetPostion"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		return;
	}
	//return;
}
BOOL CDeviceMotion::AxisMove(MP2100_THREAD thr, AXIS_ID axis, AxisMoveParam param)
{
	//20160930 kw 축별 예외처리 필요
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		return FALSE;
	}

	CAxisParam	nodeM;
	theAxisBank.GetParamNode(axis, nodeM);

	if(param.dSpeed == 0.f)
		param.dSpeed = nodeM.m_dDefaultMoveSpeed;

	if(param.dTargetPosition > nodeM.m_dPositiveLimit)
	{
		theLog[LOG_MOTOR_ERROR].AddBuf(_T("nodeM.dPositiveLimit - %d, %lf,%lf"), axis, param.dTargetPosition , nodeM.m_dPositiveLimit);
		return FALSE;
	}
	if(param.dTargetPosition < nodeM.m_dNegativeLimit)
	{
		theLog[LOG_MOTOR_ERROR].AddBuf(_T("nodeM.dNegativeLimit - %d, %lf,%lf"), axis, param.dTargetPosition , nodeM.m_dNegativeLimit);
		return FALSE;
	}
	if(param.dAcc == 0.)
	{
		param.dAcc = theAxisBank.m_Axis[axis].m_dDefaultAccel;
	}
	if(param.dDec == 0.)
	{
		param.dDec = theAxisBank.m_Axis[axis].m_dDefaultDecel;
	}

	BOOL bRtn;
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{

		SHORT sMoveType; 

		// Turn Table 같이 무한회전인 경우...
		if(theAxisBank.m_Axis[axis].m_bInfiniteMove)
		{
			double dEncPos;
			m_Mp2100[thr][nDevice].GetParam_FeedbackPos(nInnerAxis, dEncPos);

			param.dTargetPosition -= dEncPos;

			param.dTargetPosition = (param.dTargetPosition >= 0.) ? (param.dTargetPosition):(param.dTargetPosition + 360.);

			sMoveType			= MTYPE_RELATIVE;	
		}
		else
		{
			sMoveType			= MTYPE_ABSOLUTE;		
		}
		bRtn = m_Mp2100[thr][nDevice].AxisMove(nInnerAxis, param.dTargetPosition, param.dSpeed, sMoveType, param.dAcc, param.dDec);
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		double dPulsePosition = PosToPulse(param.dTargetPosition, nodeM.m_dGearRatio);
		double dPulseSpeed = PosToPulse(param.dSpeed, nodeM.m_dGearRatio);
		double dPulseStartSpeed;
		// Step Motor는 처음 구동 시작시 탄력을 못받아 움직이지 못하는 경우가 있어 시작 속도를 추가로 준다 [8/7/2017 OSC]
		if(theAxisBank.m_Axis[axis].m_MotorType == CAxisParam::MOTOR_TYPE_SERVO)
			dPulseStartSpeed = 0;
		else
			dPulseStartSpeed = dPulseSpeed/10.;
		bRtn = m_Nmc2[nDevice].AxisMove(nInnerAxis, dPulsePosition, dPulseStartSpeed, dPulseSpeed, param.dAcc, param.dDec);
	}
	if(bRtn == FALSE)
	{
		//if(axis != AXIS_ID::STP_UPPER_CONV)
		theLog[LOG_MOTOR_ERROR].AddBuf(_T("%s AxisMove Fail. Pos = %f"), nodeM.m_strAxisName, param.dTargetPosition);
	}

	return bRtn;
	
}
BOOL CDeviceMotion::AxisMoveInc(MP2100_THREAD thr, AXIS_ID axis, AxisMoveParam param)
{
	double dFeedPos;
	GetPosition(thr, axis, dFeedPos);
	param.dTargetPosition += dFeedPos;
	return AxisMove(thr, axis, param);	return FALSE;

}
BOOL CDeviceMotion::AxisMoveJog(MP2100_THREAD thr, AXIS_ID axis, BOOL bForward, AxisMoveParam param)
{
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		return FALSE;
	}

	CAxisParam	nodeM;
	theAxisBank.GetParamNode(axis, nodeM);

	if(!IsAxisReady(thr, axis, TRUE))
		return FALSE;

	double dVelocity = 0.f;
	double dPosition = 0.f;

	if(param.dSpeed!=0.0)								dVelocity	= param.dSpeed;

	if(bForward)	dPosition	= nodeM.m_dPositiveLimit;
	else			dPosition	= nodeM.m_dNegativeLimit;

	param.dTargetPosition = dPosition;
	return AxisMove(thr, axis, param);

}
BOOL CDeviceMotion::OriginRetrun(MP2100_THREAD thr, AXIS_ID axis)  // home search
{
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		return TRUE;
	}

	CAxisParam	nodeM;
	theAxisBank.GetParamNode(axis, nodeM);

	double dHomeSpeed = nodeM.m_dHomeSpeed;

	BOOL bRtn;
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	int nOriginMode;

	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)	
	{
		switch (theAxisBank.m_Axis[axis].m_OriginType)
		{
		case CAxisParam::ORIGIN_TYPE_HOME:
			nOriginMode = HMETHOD_HOMELS_ONLY;
			break;
		case CAxisParam::ORIGIN_TYPE_LIMIT_N:
			nOriginMode = HMETHOD_NOT_C;
			break;
		case CAxisParam::ORIGIN_TYPE_LIMIT_P:
			nOriginMode = HMETHOD_POT_C;
			break;
		case CAxisParam::ORIGIN_TYPE_Z_PULSE:
			nOriginMode = HMETHOD_C_ONLY;
			break;
		default:
			nOriginMode = HMETHOD_HOMELS_ONLY;
			break;
		}
		bRtn = m_Mp2100[thr][nDevice].ZeroPointReturn(nInnerAxis, dHomeSpeed, nOriginMode, theAxisBank.m_Axis[axis].m_dDefaultAccel, theAxisBank.m_Axis[axis].m_dDefaultDecel);
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		// 2 : Home Sensor로 원점
		// 1 : -Limit로 원점
		// 0 : +Limit로 원점
		double dOffsetPos = 0;
		switch (theAxisBank.m_Axis[axis].m_OriginType)
		{
		case CAxisParam::ORIGIN_TYPE_HOME:
			nOriginMode = 2;
			break;
		case CAxisParam::ORIGIN_TYPE_LIMIT_N:
			nOriginMode = 1;
			dOffsetPos = 1.;
			break;
		case CAxisParam::ORIGIN_TYPE_LIMIT_P:
			nOriginMode = 0;
			dOffsetPos = 1.;
			break;
		default:
			nOriginMode = 2;
			break;
		}

		double dStartSpeed;
		double dHomeSpeedPulse;
		double dOffsetPulse;
		if(nodeM.m_dGearRatio>0.f)
		{
			dHomeSpeedPulse = PosToPulse(dHomeSpeed, nodeM.m_dGearRatio);
			dOffsetPulse = PosToPulse(dOffsetPos, nodeM.m_dGearRatio);
		}
		else
		{
			dHomeSpeedPulse = dHomeSpeed;
			dOffsetPulse = dOffsetPos;
		}
		// Step Motor는 처음 구동 시작시 탄력을 못받아 움직이지 못하는 경우가 있어 시작 속도를 추가로 준다 [8/7/2017 OSC]
		if(theAxisBank.m_Axis[axis].m_MotorType == CAxisParam::MOTOR_TYPE_SERVO)
			dStartSpeed = 0;
		else
			dStartSpeed = dHomeSpeedPulse/10.;

		bRtn = m_Nmc2[nDevice].ZeroPointReturn(nInnerAxis, nOriginMode, dStartSpeed, dHomeSpeedPulse, theAxisBank.m_Axis[axis].m_dDefaultAccel, theAxisBank.m_Axis[axis].m_dDefaultDecel, dOffsetPulse);
	}
	return bRtn;
}
void CDeviceMotion::AxisStop(MP2100_THREAD thr, AXIS_ID axis)			// 감속 적용 후 정지
{
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		return;
	}
	BOOL bRtn = 0;
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)	
	{
		if(!m_Mp2100[thr][nDevice].AxisStop(nInnerAxis, 500))
		{
			theLog[LOG_MP2100].AddBuf(_T("%s AxisStop failed."), theAxisBank.m_Axis[axis].m_strAxisName);

			return ;
		}
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		bRtn = m_Nmc2[nDevice].AxisStop(nInnerAxis, 0);
		if(!bRtn)
		{
			theLog[LOG_NMC].AddBuf(_T("%s AxisStop failed."), theAxisBank.m_Axis[axis].m_strAxisName);

			return ;

		}
	}

}
void CDeviceMotion::AxisStopEMS(MP2100_THREAD thr, AXIS_ID axis)		// 급정지
{
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		return;
	}

	BOOL bRtn = 0;
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)	
	{
		if(!m_Mp2100[thr][nDevice].AxisStop(nInnerAxis, 200))
		{
			theLog[LOG_MP2100].AddBuf(_T("%s AxisStop failed."), theAxisBank.m_Axis[axis].m_strAxisName);

			return ;
		}
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		bRtn = m_Nmc2[nDevice].AxisStop(nInnerAxis, 1);
		if(!bRtn)
		{
			theLog[LOG_NMC].AddBuf(_T("%s AxisStop failed."), theAxisBank.m_Axis[axis].m_strAxisName);

			return ;

		}
	}

}
AxisStatus CDeviceMotion::GetAxisStatus(MP2100_THREAD thr, AXIS_ID axis)
{
	// 축 상태를 모아 AxisStatus로 통합하여 반환한다.
	AxisStatus status;

	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		return status;
	}

	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		WORD wVal;
		LONG lVal = 0;

		m_Mp2100[thr][nDevice].GetParam_DriveStatus(nInnerAxis, wVal);
// 		status.bBusy = ((wVal>>CMp2100::DRIVE_STATUS_MOTION_READY)&0x01) ? FALSE:TRUE;
		status.bBusy = IsAxisStop(thr, axis) ? FALSE:TRUE;
		status.bServoOn = ((wVal>>CMp2100::DRIVE_STATUS_RUNNING)&0x01) ? TRUE:FALSE;

		m_Mp2100[thr][nDevice].GetParam_Alarm(nInnerAxis, lVal);
		status.bAlarmOn = lVal ? TRUE:FALSE;
		m_Mp2100[thr][nDevice].GetParam_Warning(nInnerAxis, lVal);
		status.bWarningOn = lVal ? TRUE:FALSE;

		m_Mp2100[thr][nDevice].GetParam_ServoIO(nInnerAxis, wVal);
		status.bHomeOn = (wVal >> CMp2100::NETWORK_SERVO_IO_MONITOR_EXT1 & 0x01) ? TRUE:FALSE;
		status.bPLimitOn = (wVal >> CMp2100::NETWORK_SERVO_IO_MONITOR_POT & 0x01) ? TRUE:FALSE;
		status.bNLimitOn = (wVal >> CMp2100::NETWORK_SERVO_IO_MONITOR_NOT & 0x01) ? TRUE:FALSE;
		status.bOriginComplete = m_Mp2100[thr][nDevice].GetParamBit_ZRNC(nInnerAxis);
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		NMC_AXES_EXPR nmcdata;
		m_Nmc2[nDevice].GetNmcData(nmcdata);
		// 모터 타입별로 서보온,알람 상태 반전 [6/26/2017 OSC]
		if(theAxisBank.m_Axis[axis].m_MotorType == CAxisParam::MOTOR_TYPE_SERVO)
		{
			status.bServoOn = m_Nmc2[nDevice].CheckServo(nInnerAxis);
			status.bAlarmOn = nmcdata.nAlarm[nInnerAxis];
			status.bBusy = ( (nmcdata.nSReady[nInnerAxis] == FALSE) || nmcdata.nBusy[nInnerAxis] || (nmcdata.nInpo[nInnerAxis] == FALSE) ) ? TRUE:FALSE;
		}
		else if(theAxisBank.m_Axis[axis].m_MotorType == CAxisParam::MOTOR_TYPE_EZ_SERVO)
		{
			status.bServoOn = m_Nmc2[nDevice].CheckServo(nInnerAxis);
			status.bServoOn = status.bServoOn ? FALSE:TRUE;
			status.bAlarmOn = nmcdata.nAlarm[nInnerAxis];
			status.bBusy = ( nmcdata.nBusy[nInnerAxis] || (nmcdata.nInpo[nInnerAxis] == FALSE) ) ? TRUE:FALSE;
		}
		else if(theAxisBank.m_Axis[axis].m_MotorType == CAxisParam::MOTOR_TYPE_STEP)
		{
			status.bServoOn = TRUE;	// 스텝이면 서보온이 없다
			status.bAlarmOn = FALSE;;	// 스텝이면 알람이 없다
			status.bBusy = nmcdata.nBusy[nInnerAxis] ? TRUE:FALSE;
		}
// 		status.bWarningOn = nmcdata.nAlarm[nInnerAxis];	// NMC는 Warning이 없다
		status.bHomeOn = nmcdata.nNear[nInnerAxis];
		status.bPLimitOn = nmcdata.nPLimit[nInnerAxis];
		status.bNLimitOn = nmcdata.nMLimit[nInnerAxis];
		status.bOriginComplete = m_Nmc2[nDevice].CheckZRNC(nInnerAxis);
	}
	return status;
}
CString CDeviceMotion::GetAlarmCode(MP2100_THREAD thr, AXIS_ID axis)	// 혹시 업체별로 10진수, 16진수 등 표기 차이가 있을까봐 문자열로 처리한다
{
	CString str = _T("");
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		return str;
	}
	LONG lVal = 0;

	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		if(!m_Mp2100[thr][nDevice].GetParam_Alarm(nInnerAxis, lVal))
			return FALSE;
		str.Format(_T("%X0"),lVal);	// 3자리 Alarm인데 두자리밖에 표현 안해서 뒤에 0을 붙인다
	}
	// NMC는 범용제어기라 Alarm 코드를 받는 기능이 없다
// 	else
// 	{
// 		nBoard			= theAxisBank.m_Axis[axis].m_nModuleNo;
// 		nInnerAxisNo	= theAxisBank.m_Axis[axis].m_nInnerNo;
// 		m_Nmc2[nBoard].GetAlarmSignal(nInnerAxisNo, lVal);
// 	}
	return str;
}
double CDeviceMotion::GetTorque(MP2100_THREAD thr, AXIS_ID axis)		// int? double?
{
	double dTorque = 0;
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		return dTorque;
	}

	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		if(!m_Mp2100[thr][nDevice].GetParam_TorqueChk(nInnerAxis, dTorque))
			return dTorque;
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		// NMC는 토크를 받아오지 못함
		dTorque = 0.;
	}

	return dTorque;
}
BOOL CDeviceMotion::GetPosition(MP2100_THREAD thr, AXIS_ID axis, double &dPos)
{
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		return FALSE;
	}
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		if(!m_Mp2100[thr][nDevice].GetParam_FeedbackPos(nInnerAxis, dPos))
			return FALSE;
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		CAxisParam	nodeM;
		theAxisBank.GetParamNode(axis, nodeM);
		double dPulse;
		if(theAxisBank.m_Axis[axis].m_MotorType == CAxisParam::MOTOR_TYPE_STEP)
		{
			if(!m_Nmc2[nDevice].GetCmdPos(nInnerAxis, dPulse))
				return FALSE;
		}
		else
		{
			if(!m_Nmc2[nDevice].GetEncPos(nInnerAxis, dPulse))
				return FALSE;
		}

		if(nodeM.m_dGearRatio>0.f)
			dPos = PulseToPos(dPulse, nodeM.m_dGearRatio);
		else
			dPos = double(dPulse);
	}

	return TRUE;
}
BOOL CDeviceMotion::GetTargetPosition(MP2100_THREAD thr, AXIS_ID axis, double &dPos)	// 이동할 목표위치
{
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		return FALSE;
	}
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		if(!m_Mp2100[thr][nDevice].GetParam_TargetPos(nInnerAxis, dPos))
			return FALSE;
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{	
		CAxisParam	nodeM;
		theAxisBank.GetParamNode(axis, nodeM);
		double dPulse;
		if(!m_Nmc2[nDevice].GetTargetPostion(nInnerAxis,dPulse))
			return FALSE;

		if(nodeM.m_dGearRatio>0.f)
			dPos = PulseToPos(dPulse, nodeM.m_dGearRatio);
		else
			dPos = double(dPulse);
	}

	return TRUE;

}

//bIgnoreOrigin:TRUE  원점 확인안함
BOOL CDeviceMotion::IsAxisReady(MP2100_THREAD thr, AXIS_ID axis, BOOL bIgnoreOrigin)	// 해당 축이 지령 받을 수 있는 상태인지 확인
{
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		return TRUE;
	}
	BOOL bRtn;
	BOOL bIgnore = TRUE;
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		if(!bIgnoreOrigin)
			bIgnore = m_Mp2100[thr][nDevice].GetParamBit_ZRNC(nInnerAxis);
		bRtn = m_Mp2100[thr][nDevice].IsAxisReady(nInnerAxis);
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		if(!bIgnoreOrigin)
		{
			bIgnore = m_Nmc2[nDevice].CheckZRNC(nInnerAxis);
		}
		if(theAxisBank.m_Axis[axis].m_MotorType == CAxisParam::MOTOR_TYPE_STEP)
			bRtn = m_Nmc2[nDevice].CheckAxisReady(nInnerAxis, TRUE);	
		else
			bRtn = m_Nmc2[nDevice].CheckAxisReady(nInnerAxis, FALSE);	
	}
	if(!bIgnore)
		return FALSE;
	return bRtn;
}

BOOL CDeviceMotion::IsAxisStop(MP2100_THREAD thr, AXIS_ID axis)
{
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		return TRUE;
	}

	BOOL bRtn;
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		bRtn = m_Mp2100[thr][nDevice].IsAxisStop(nInnerAxis);
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		bRtn = m_Nmc2[nDevice].CheckBusy(nInnerAxis) ? FALSE:TRUE;
	}

	return bRtn;
}

BOOL CDeviceMotion::IsTrouble(MP2100_THREAD thr, AXIS_ID axis)	// 축에 문제가 있는지 없는지 확인(Busy와는 상관없이 Alarm이나 Limit 등의 문제요소만 확인)
{
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		return FALSE;
	}
	BOOL bRtn = FALSE;
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		bRtn = m_Mp2100[thr][nDevice].CheckTrouble(nInnerAxis);
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		bRtn = m_Nmc2[nDevice].CheckTrouble(nInnerAxis);
	}

	return bRtn;
}

BOOL CDeviceMotion::TeachMove( MP2100_THREAD thr, TEACH_PARAM::ID idTeach, double dOffset /*= 0.*/, double dSpeedRatio/*=1.*/, BOOL bIgnoreInterlock /*= FALSE*/ )
{
	CUnitCtrlFunc UnitCtrl(thr);

	if(bIgnoreInterlock == FALSE)
	{
		if(UnitCtrl.CheckTeachMoveInterlock(idTeach, dOffset) == FALSE)
		{
			theLog[LOG_MOTOR_ERROR].AddBuf(_T("%s Axis Interlock : %s"), 
				theAxisBank.m_Axis[theTeachBank.GetTeachParam( idTeach ).idAxis].m_strAxisName,
				theProcBank.m_strLastKorMsg);
			return FALSE;
		}
	}

	AxisMoveParam paramMove;
	paramMove = theTeachBank.GetTeachParam( idTeach );
	paramMove.dTargetPosition += dOffset;
	paramMove.dSpeed *= dSpeedRatio;

	// Origin이 되어있지 않으면 위치이동 불가 [11/1/2016 OSC]
	if(IsAxisReady(thr, paramMove.idAxis, FALSE) == FALSE)
	{
		theLog[LOG_MOTOR_ERROR].AddBuf(_T("%s Axis Not Ready"), 
			theAxisBank.m_Axis[theTeachBank.GetTeachParam( idTeach ).idAxis].m_strAxisName);
		return FALSE;
	}

	return AxisMove(thr, paramMove.idAxis, paramMove);
}

BOOL CDeviceMotion::CheckAxisPosition(MP2100_THREAD thr, AXIS_ID axis, double dCheckPos, double dRange /*= 0.05*/, POS_RANGE_COMPAIR rangeCompair /*= POS_EQUAL*/ )
{
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		return TRUE;
	}

	double dFeedPos, dTargetPos;
	if(!GetPosition(thr, axis, dFeedPos))	return FALSE;
	if(!GetTargetPosition(thr, axis, dTargetPos))	return FALSE;

	if(rangeCompair == POS_EQUAL)			// 현재 위치와 같음	(Motor 정지 여부도 같이 확인한다)
		return (IsAxisReady(thr, axis, FALSE) && dFeedPos > (dCheckPos - dRange) && dFeedPos < (dCheckPos + dRange));
	else if(rangeCompair == POS_LOWER)	// 현재 위치보다 작거나 같음 (Motor 정지는 확인 안한다)
		return ( (IsTrouble(thr, axis) == FALSE) && (dFeedPos < (dCheckPos + dRange)) && (dTargetPos < (dCheckPos + dRange)) );
	else if(rangeCompair == POS_UPPER)	// 현재 위치보다 높거나 같음 (Motor 정지는 확인 안한다)
		return ( (IsTrouble(thr, axis) == FALSE) && (dFeedPos > (dCheckPos - dRange)) && (dTargetPos > (dCheckPos - dRange)) );

	return FALSE;
}

BOOL CDeviceMotion::CheckTeachPosition( MP2100_THREAD thr, TEACH_PARAM::ID idTeach, double dRange /*= 0.05*/, POS_RANGE_COMPAIR rangeCompair /*= POS_EQUAL*/, double dOffset /*= 0.*/ )
{
	TEACH_PARAM paramTeach = theTeachBank.GetTeachParam( idTeach );

	double dTeachPos = paramTeach.dPos + dOffset;
	BOOL bRet = CheckAxisPosition(thr, paramTeach.idAxis, dTeachPos, dRange, rangeCompair);
	return bRet;
}

//kjpark 20161030 정위치가 아니면 A존에 A지그 가도록 다이얼로그창 띄움
BOOL CDeviceMotion::CheckPosition( double dDefaultPos, double dNowPos, double dCheckLimit /*= 0.01*/ )	/* 0.01mm이상 차이나면 틀리다고 간주 */
{
	if(max(dDefaultPos+dCheckLimit, dNowPos) == dNowPos)	return FALSE;
	if(min(dDefaultPos-dCheckLimit, dNowPos) == dNowPos)	return FALSE;

	return TRUE;
}
double CDeviceMotion::PosToPulse(double dPos, double dGearRatio)
{
	return CEtc::Round(dPos*dGearRatio);
}
double CDeviceMotion::PulseToPos(double dPulse, double dGearRatio)
{
	return dPulse/dGearRatio;
}

void CDeviceMotion::ChainInterpolation_CreateGroupStart( CHAIN_INTERPOLATION_PARAM param )
{
	// Group No 오류 확인
	if( (param.nGroupNo < 0) && (param.nGroupNo > MAX_INTRPOLATION_GROUP) )
		return;

	CNmc2::INTERPOLATION_AXIS axistype = CNmc2::INTERPOLATION_2AXIS;
	if(param.axis3 != AXIS_ID_MAX)
		axistype = CNmc2::INTERPOLATION_3AXIS;

	// NMC축인지 확인
	if(theAxisBank.m_Axis[param.axis1].m_DeviceType != CAxisParam::DEVICE_TYPE_NMC)
		return;
	if(theAxisBank.m_Axis[param.axis2].m_DeviceType != CAxisParam::DEVICE_TYPE_NMC)
		return;
	if( (axistype == CNmc2::INTERPOLATION_3AXIS) && (theAxisBank.m_Axis[param.axis3].m_DeviceType != CAxisParam::DEVICE_TYPE_NMC) )
		return;

	// 같은 NMC에 연결된 축인지 확인
	if(theAxisBank.m_Axis[param.axis1].m_nModuleNo != theAxisBank.m_Axis[param.axis2].m_nModuleNo)
		return;
	if(axistype == CNmc2::INTERPOLATION_3AXIS)
	{
		if(theAxisBank.m_Axis[param.axis1].m_nModuleNo != theAxisBank.m_Axis[param.axis3].m_nModuleNo)
			return;
	}


	m_ChainInterpolationParam = param;

	// 보간이동 축들의 기어비는 모두 같아야 한다 [6/8/2017 OSC]
	CAxisParam	nodeM;
	theAxisBank.GetParamNode(param.axis1, nodeM);
	m_ChainInterpolationParam.dSpeed = PosToPulse(param.dSpeed, nodeM.m_dGearRatio);

	m_nChainInterpolationDevice = theAxisBank.m_Axis[param.axis1].m_nModuleNo;

	// 기존꺼 있다면 Clear
	m_Nmc2[m_nChainInterpolationDevice].Interpolation_ClearList(param.nGroupNo);

	m_Nmc2[m_nChainInterpolationDevice].Interpolation_BeginListCreate(
		param.nGroupNo,
		axistype, 
		m_ChainInterpolationParam.axis1, 
		m_ChainInterpolationParam.axis2,
		m_ChainInterpolationParam.axis3,
		m_ChainInterpolationParam.dSpeed);
}

void CDeviceMotion::ChainInterpolation_AddPosition( CNmc2::INTERPOLATION_POSITION pos )
{
	if(m_nChainInterpolationDevice < 0)
		return;
	if(pos.type == CNmc2::INTERPOLATION_TYPE_MAX)
		return;

	// 보간이동 축들의 기어비는 모두 같아야 한다 [6/8/2017 OSC]
	CAxisParam	nodeM;
	theAxisBank.GetParamNode(m_ChainInterpolationParam.axis1, nodeM);
	if(pos.type == CNmc2::INTERPOLATION_LINEAR)
	{
		pos.dPos1 = PosToPulse(pos.dPos1, nodeM.m_dGearRatio);
		pos.dPos2 = PosToPulse(pos.dPos2, nodeM.m_dGearRatio);
		pos.dPos3 = PosToPulse(pos.dPos3, nodeM.m_dGearRatio);
	}
	else
	{
		pos.dCenter1 = PosToPulse(pos.dCenter1, nodeM.m_dGearRatio);
		pos.dCenter2 = PosToPulse(pos.dCenter2, nodeM.m_dGearRatio);
	}
	pos.dStartSpeed = PosToPulse(pos.dStartSpeed, nodeM.m_dGearRatio);
	pos.dSpeed = PosToPulse(pos.dSpeed, nodeM.m_dGearRatio);
	pos.dAcc = PosToPulse(pos.dAcc, nodeM.m_dGearRatio);
	pos.dDec = PosToPulse(pos.dDec, nodeM.m_dGearRatio);

	m_Nmc2[m_nChainInterpolationDevice].Interpolation_AddPosition(m_ChainInterpolationParam.nGroupNo, pos);
}

void CDeviceMotion::ChainInterpolation_CreateGroupEnd()
{
	if(m_nChainInterpolationDevice < 0)
		return;
	m_Nmc2[m_nChainInterpolationDevice].Interpolation_EndListCreate(m_ChainInterpolationParam.nGroupNo);
}

void CDeviceMotion::ChainInterpolation_Run()
{
	if(m_nChainInterpolationDevice < 0)
		return;
	m_Nmc2[m_nChainInterpolationDevice].Interpolation_RunStop(m_ChainInterpolationParam.nGroupNo, TRUE);
}

void CDeviceMotion::ChainInterpolation_Stop()
{
	if(m_nChainInterpolationDevice < 0)
		return;
	m_Nmc2[m_nChainInterpolationDevice].Interpolation_RunStop(m_ChainInterpolationParam.nGroupNo, FALSE);
}

BOOL CDeviceMotion::ChainInterpolation_IsStop()
{
	if(m_nChainInterpolationDevice < 0)
		return FALSE;
	return m_Nmc2[m_nChainInterpolationDevice].Interpolation_IsStop(m_ChainInterpolationParam.nGroupNo);
}
