#pragma once

#include "NMC2/NMC2.h"
#pragma comment(lib, "NMC2.lib")
#include "MotionModuleDefine.h"

#define MAX_INTRPOLATION_GROUP	2	// 컨트롤러 하나당 동시에 구동 가능한 보간이동 그룹

#define NMCGroupNo(axis)	(axis/4)	// NMC 0~3번축은 그룹 0, 4~7번축은 그룹 1
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

	//--- ListMotion 연속 보간이동
	enum INTERPOLATION_AXIS
	{
		INTERPOLATION_2AXIS = 0,	// 2축 보간이동
		INTERPOLATION_3AXIS,		// 3축 보간이동
		INTERPOLATION_AXIS_MAX
	};
	enum INTERPOLATION_TYPE
	{
		INTERPOLATION_LINEAR = 0,	// 선형 보간이동
		INTERPOLATION_ARC,			// 원호 보간이동 (2축 전용)
		INTERPOLATION_TYPE_MAX
	};
	struct INTERPOLATION_POSITION
	{
		INTERPOLATION_TYPE type;

		// 선형보간
		double dPos1;	// 1축 이동위치
		double dPos2;	// 2축 이동위치
		double dPos3;	// 3축 이동위치
		
		// 원호보간
		double dCenter1;	// 중심점 X
		double dCenter2;	// 중심점 Y
		double dAngle;		// Angle

		double dStartSpeed;	// 시작속도
		double dSpeed;		// 속도
		double dAcc;		// 가속도 (dStartSpeed,dSpeed,이전 속도가 같으면 가속구간이 없으므로 0으로 해도 된다)
		double dDec;		// 감속도 (다음 보간이동이 바로 이어져서 멈출 필요가 없으면 0으로 해도 된다)

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
	// NMC 0~3번축은 그룹 0, 4~7번축은 그룹 1
	void	Interpolation_BeginListCreate(int nGroupNo, INTERPOLATION_AXIS axisType, int nAxis1No, int nAxis2No, int nAxis3No, double dMaxSpeed);	// 보간좌표 등록 시작
	void	Interpolation_AddPosition(int nGroupNo, INTERPOLATION_POSITION pos);	// 보간좌표 추가
	void	Interpolation_EndListCreate(int nGroupNo);		// 보간좌표 등록 종료
	void	Interpolation_ClearList(int nGroupNo);			// 보간좌표 클리어
	void	Interpolation_RunStop(int nGroupNo, BOOL bRun);	// 보간이동 시작/정지
	BOOL	Interpolation_IsStop(int nGroupNo);				// 연속보간이동 중인지 확인

	//--- Gantry 동기화 설정
	void	SetGantryAxis(BOOL bEnable, int nAxisMaster, int nAxisSlave);
};
