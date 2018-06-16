// MMC39.h: interface for the CMMC39 class.
//
//////////////////////////////////////////////////////////////////////
#pragma once


enum axis_type_t {
	AXIS_ALL = -1,
	AXIS_SLX  = 1,
	AXIS_SLX2 ,
	AXIS_SRX,
	AXIS_SRX2,
	AXIS_SLY2,
	AXIS_SLY,
	AXIS_SRY,
	AXIS_SRY2,
	AXIS_WLX,
	AXIS_WRX,
	AXIS_WY
};

#define INC_ENC_MOTOR		0
#define ABS_ENC_MOTOR		1
#define RETRY_NO			3

UINT HomeSearchProc(LPVOID lParam);

class CMMC39  
{
public:
	CMMC39();
	virtual ~CMMC39();


public:
	BOOL InitMC();
	BOOL InitMotion();
	void MoveJog(int iAxis , int iAcc , double dVel , int iDir);
	int StartMove(int iAxis, double dPos , double dVel );
	void MoveAllStop();
	void MoveStop(int iAxis);
	double GetCmdPos(int iAxis);
	BOOL SetAmpEnable(int iAxis , BOOL bAction);
	void E_Stop(int iAxis);
	void E_StopAll();
	double GetActPos(int iAxis);
	BOOL ResetAxis(int Axis);
	BOOL IsMoveOK(int iAxis);
	BOOL CheckMoveDoneLoop(int iAxis , DWORD timeout);
	BOOL ClearAxis(int axis);
	void ClearStatusAll();
	void SetSwLimit(int axis, double neg_limit, double pos_limit, int iEvent);
	BOOL InitParameter();
	int	 MoveHomeAbs(int iAxis);
	int	 MoveHomeEnc(int iAxis);
	int AxisStatus(int iAxis);
	BOOL GetServoOnStatus(int iAxis);	// TRUE�� Servo On
	BOOL CheckNoEvent(int nAxis);		// TRUE�� No Event
	BOOL CheckHwLimit(int nAxis);		// TRUE�� ���� �ȵ�
	BOOL CheckAmpFault(int nAxis);		// TRUE�� Alarm �ƴ�
	BOOL CheckAxisError(int nAxis);		// TRUE�� No Error
	BOOL CheckHomeSensor(int nAxis);	// TRUE�� ����

	BOOL	m_Init_Motion;

	int    m_nOriginFlag[MAX_AXIS_BUFFER];	// �ະ�� ������ �Ϸ�Ǿ����� Ȯ��
	void HomeFlagResetAll();
	BOOL CheckHomeFlagAll();

	double dMotionFullStrokeX;
	double dMotionFullStrokeY;
	double dMotionFullStrokeSingle;


};
