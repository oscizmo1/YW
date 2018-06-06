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
		for(int th=0; th<1; th++)// Sample ������ 1���� close ���ش�.
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
			return m_Nmc2[nDevice].ServoOFF(nInnerAxis);	// EZ ������ �ݴ��
		// ������ �������� �ʿ����
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
			bRtn = m_Nmc2[nDevice].ServoON(nInnerAxis);	// EZ ������ �ݴ��
		// ������ �������� �ʿ����

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
void CDeviceMotion::SetPosition(MP2100_THREAD thr, AXIS_ID axis, double dPosition)	// ������ ���� ��ġ���� �ٲ۴�
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
	//20160930 kw �ະ ����ó�� �ʿ�
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

		// Turn Table ���� ����ȸ���� ���...
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
		// Step Motor�� ó�� ���� ���۽� ź���� ���޾� �������� ���ϴ� ��찡 �־� ���� �ӵ��� �߰��� �ش� [8/7/2017 OSC]
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
		// 2 : Home Sensor�� ����
		// 1 : -Limit�� ����
		// 0 : +Limit�� ����
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
		// Step Motor�� ó�� ���� ���۽� ź���� ���޾� �������� ���ϴ� ��찡 �־� ���� �ӵ��� �߰��� �ش� [8/7/2017 OSC]
		if(theAxisBank.m_Axis[axis].m_MotorType == CAxisParam::MOTOR_TYPE_SERVO)
			dStartSpeed = 0;
		else
			dStartSpeed = dHomeSpeedPulse/10.;

		bRtn = m_Nmc2[nDevice].ZeroPointReturn(nInnerAxis, nOriginMode, dStartSpeed, dHomeSpeedPulse, theAxisBank.m_Axis[axis].m_dDefaultAccel, theAxisBank.m_Axis[axis].m_dDefaultDecel, dOffsetPulse);
	}
	return bRtn;
}
void CDeviceMotion::AxisStop(MP2100_THREAD thr, AXIS_ID axis)			// ���� ���� �� ����
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
void CDeviceMotion::AxisStopEMS(MP2100_THREAD thr, AXIS_ID axis)		// ������
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
	// �� ���¸� ��� AxisStatus�� �����Ͽ� ��ȯ�Ѵ�.
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
		// ���� Ÿ�Ժ��� ������,�˶� ���� ���� [6/26/2017 OSC]
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
			status.bServoOn = TRUE;	// �����̸� �������� ����
			status.bAlarmOn = FALSE;;	// �����̸� �˶��� ����
			status.bBusy = nmcdata.nBusy[nInnerAxis] ? TRUE:FALSE;
		}
// 		status.bWarningOn = nmcdata.nAlarm[nInnerAxis];	// NMC�� Warning�� ����
		status.bHomeOn = nmcdata.nNear[nInnerAxis];
		status.bPLimitOn = nmcdata.nPLimit[nInnerAxis];
		status.bNLimitOn = nmcdata.nMLimit[nInnerAxis];
		status.bOriginComplete = m_Nmc2[nDevice].CheckZRNC(nInnerAxis);
	}
	return status;
}
CString CDeviceMotion::GetAlarmCode(MP2100_THREAD thr, AXIS_ID axis)	// Ȥ�� ��ü���� 10����, 16���� �� ǥ�� ���̰� ������� ���ڿ��� ó���Ѵ�
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
		str.Format(_T("%X0"),lVal);	// 3�ڸ� Alarm�ε� ���ڸ��ۿ� ǥ�� ���ؼ� �ڿ� 0�� ���δ�
	}
	// NMC�� ���������� Alarm �ڵ带 �޴� ����� ����
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
		// NMC�� ��ũ�� �޾ƿ��� ����
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
BOOL CDeviceMotion::GetTargetPosition(MP2100_THREAD thr, AXIS_ID axis, double &dPos)	// �̵��� ��ǥ��ġ
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

//bIgnoreOrigin:TRUE  ���� Ȯ�ξ���
BOOL CDeviceMotion::IsAxisReady(MP2100_THREAD thr, AXIS_ID axis, BOOL bIgnoreOrigin)	// �ش� ���� ���� ���� �� �ִ� �������� Ȯ��
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

BOOL CDeviceMotion::IsTrouble(MP2100_THREAD thr, AXIS_ID axis)	// �࿡ ������ �ִ��� ������ Ȯ��(Busy�ʹ� ������� Alarm�̳� Limit ���� ������Ҹ� Ȯ��)
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

	// Origin�� �Ǿ����� ������ ��ġ�̵� �Ұ� [11/1/2016 OSC]
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

	if(rangeCompair == POS_EQUAL)			// ���� ��ġ�� ����	(Motor ���� ���ε� ���� Ȯ���Ѵ�)
		return (IsAxisReady(thr, axis, FALSE) && dFeedPos > (dCheckPos - dRange) && dFeedPos < (dCheckPos + dRange));
	else if(rangeCompair == POS_LOWER)	// ���� ��ġ���� �۰ų� ���� (Motor ������ Ȯ�� ���Ѵ�)
		return ( (IsTrouble(thr, axis) == FALSE) && (dFeedPos < (dCheckPos + dRange)) && (dTargetPos < (dCheckPos + dRange)) );
	else if(rangeCompair == POS_UPPER)	// ���� ��ġ���� ���ų� ���� (Motor ������ Ȯ�� ���Ѵ�)
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

//kjpark 20161030 ����ġ�� �ƴϸ� A���� A���� ������ ���̾�α�â ���
BOOL CDeviceMotion::CheckPosition( double dDefaultPos, double dNowPos, double dCheckLimit /*= 0.01*/ )	/* 0.01mm�̻� ���̳��� Ʋ���ٰ� ���� */
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
	// Group No ���� Ȯ��
	if( (param.nGroupNo < 0) && (param.nGroupNo > MAX_INTRPOLATION_GROUP) )
		return;

	CNmc2::INTERPOLATION_AXIS axistype = CNmc2::INTERPOLATION_2AXIS;
	if(param.axis3 != AXIS_ID_MAX)
		axistype = CNmc2::INTERPOLATION_3AXIS;

	// NMC������ Ȯ��
	if(theAxisBank.m_Axis[param.axis1].m_DeviceType != CAxisParam::DEVICE_TYPE_NMC)
		return;
	if(theAxisBank.m_Axis[param.axis2].m_DeviceType != CAxisParam::DEVICE_TYPE_NMC)
		return;
	if( (axistype == CNmc2::INTERPOLATION_3AXIS) && (theAxisBank.m_Axis[param.axis3].m_DeviceType != CAxisParam::DEVICE_TYPE_NMC) )
		return;

	// ���� NMC�� ����� ������ Ȯ��
	if(theAxisBank.m_Axis[param.axis1].m_nModuleNo != theAxisBank.m_Axis[param.axis2].m_nModuleNo)
		return;
	if(axistype == CNmc2::INTERPOLATION_3AXIS)
	{
		if(theAxisBank.m_Axis[param.axis1].m_nModuleNo != theAxisBank.m_Axis[param.axis3].m_nModuleNo)
			return;
	}


	m_ChainInterpolationParam = param;

	// �����̵� ����� ����� ��� ���ƾ� �Ѵ� [6/8/2017 OSC]
	CAxisParam	nodeM;
	theAxisBank.GetParamNode(param.axis1, nodeM);
	m_ChainInterpolationParam.dSpeed = PosToPulse(param.dSpeed, nodeM.m_dGearRatio);

	m_nChainInterpolationDevice = theAxisBank.m_Axis[param.axis1].m_nModuleNo;

	// ������ �ִٸ� Clear
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

	// �����̵� ����� ����� ��� ���ƾ� �Ѵ� [6/8/2017 OSC]
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
