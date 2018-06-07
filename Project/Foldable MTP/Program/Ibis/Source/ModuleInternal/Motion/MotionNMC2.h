#pragma once

#include "NMC2/NMC2.h"
#pragma comment(lib, "NMC2.lib")
#include "MotionModuleDefine.h"

#define MAX_INTRPOLATION_GROUP	2	// ��Ʈ�ѷ� �ϳ��� ���ÿ� ���� ������ �����̵� �׷�

#define NMCGroupNo(axis)	(axis/4)	// NMC 0~3������ �׷� 0, 4~7������ �׷� 1
class CNmc2
{
public:
	CNmc2();
	~CNmc2();
	enum
	{
		MAX_AXIS_CNT = 8,
	};
public:
	int		m_nControllerID;
	static DWORD m_dwOpenState;

	NMC_AXES_EXPR	m_nmcData;
	NMC_AXES_MOTION_OUT	NmcOut;	
public:
	BOOL	OpenController(const int nCtrlID);
	BOOL	CloseController();

	BOOL	GetAlarmSignal(const int nAxisNo, LONG& lVal);
	BOOL	GetInPosSignal(const int nAxisNo, WORD& wVal);
	BOOL	 GetCmdPos(const int nAxisNo, double& dPos);
	BOOL	GetEncPos(const int nAxisNo, double& dPos);
	BOOL	GetNmcData(NMC_AXES_EXPR& data);
	BOOL	GetTargetPostion(short nAxisNo,double& pTargetPos);

	//--- Step Control
	BOOL	ServoON(int nAxisNo);	
	BOOL	ServoOFF(int nAxisNo);	
	BOOL	AlarmClear(const int nAxisNo, BOOL bSet);
	BOOL	ZeroPointSet(const int nAxisNo, double dPos);
	BOOL ZeroPointReturn(const int nAxisNo, int nHomeMode, double dStartSpeed, double dHomeSpeed, double dAccType = ACC_300ms, double dDecType = ACC_300ms, double dOffsetPos=0.);
	BOOL	AxisStop(const int nAxisNo, BOOL bEms = FALSE);

	BOOL AxisMove(const int nAxisNo, double dPos, double dStartSpeed, double dSpeed, double dAccType = ACC_300ms, double dDecType = ACC_300ms);

	void	ChangeAxisSpeed(int nInnerAxisNo, double dSpeed);

	//--- Check Function
	BOOL	CheckOpen();
	BOOL	CheckServo(const int nAxisNo);	//JHPARK - 2013/05/23
	BOOL	CheckBusy(const int nAxisNo);
	BOOL	CheckZRNC(const int nAxisNo);
	BOOL 	CheckAxisReady(const int nAxisNo, BOOL bIgnoreInposition);
	BOOL	CheckTrouble(const int nAxisNo);
	BOOL	CheckPLimit(const int nAxisNo);
	BOOL	CheckNLimit(const int nAxisNo);

	//--- ListMotion ���� �����̵�
	enum INTERPOLATION_AXIS
	{
		INTERPOLATION_2AXIS = 0,	// 2�� �����̵�
		INTERPOLATION_3AXIS,		// 3�� �����̵�
		INTERPOLATION_AXIS_MAX
	};
	enum INTERPOLATION_TYPE
	{
		INTERPOLATION_LINEAR = 0,	// ���� �����̵�
		INTERPOLATION_ARC,			// ��ȣ �����̵� (2�� ����)
		INTERPOLATION_TYPE_MAX
	};
	struct INTERPOLATION_POSITION
	{
		INTERPOLATION_TYPE type;

		// ��������
		double dPos1;	// 1�� �̵���ġ
		double dPos2;	// 2�� �̵���ġ
		double dPos3;	// 3�� �̵���ġ
		
		// ��ȣ����
		double dCenter1;	// �߽��� X
		double dCenter2;	// �߽��� Y
		double dAngle;		// Angle

		double dStartSpeed;	// ���ۼӵ�
		double dSpeed;		// �ӵ�
		double dAcc;		// ���ӵ� (dStartSpeed,dSpeed,���� �ӵ��� ������ ���ӱ����� �����Ƿ� 0���� �ص� �ȴ�)
		double dDec;		// ���ӵ� (���� �����̵��� �ٷ� �̾����� ���� �ʿ䰡 ������ 0���� �ص� �ȴ�)

		INTERPOLATION_POSITION()
		{
			type = INTERPOLATION_TYPE_MAX;
			dPos1 = dPos2 = dPos3 = 0.;
			dCenter1 = dCenter2 = dAngle = 0.;
			dStartSpeed = dSpeed = dAcc = dDec = 0.;
		}
	};

private:
	INTERPOLATION_AXIS m_InterpolationAxis[MAX_INTRPOLATION_GROUP];	
public:
	// NMC 0~3������ �׷� 0, 4~7������ �׷� 1
	void	Interpolation_BeginListCreate(int nGroupNo, INTERPOLATION_AXIS axisType, int nAxis1No, int nAxis2No, int nAxis3No, double dMaxSpeed);	// ������ǥ ��� ����
	void	Interpolation_AddPosition(int nGroupNo, INTERPOLATION_POSITION pos);	// ������ǥ �߰�
	void	Interpolation_EndListCreate(int nGroupNo);		// ������ǥ ��� ����
	void	Interpolation_ClearList(int nGroupNo);			// ������ǥ Ŭ����
	void	Interpolation_RunStop(int nGroupNo, BOOL bRun);	// �����̵� ����/����
	BOOL	Interpolation_IsStop(int nGroupNo);				// ���Ӻ����̵� ������ Ȯ��

	//--- Gantry ����ȭ ����
	void	SetGantryAxis(BOOL bEnable, int nAxisMaster, int nAxisSlave);
};
