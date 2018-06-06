#pragma once

#include "Motion/MP2100.h"
#include "BankGuild/AxisParamBank.h"
//20160921 kw
#include "Motion/MotionNMC2.h"
#include "BankGuild/TeachBank.h"

enum MP2100_THREAD
{
	// MP2100 �Լ��� ȣ��� Thread ������ŭ ����
	// �� Thread���� ���� Open����� �Ѵ�
	MP2100_THREAD_MAIN_PROCESS=0,
	MP2100_THREAD_MAIN_SEQUENCE,
	MP2100_THREAD_IO_SEQUENCE,
	MP2100_THREAD_FDC_MONITOR,
	MP2100_THREAD_IO_ALARM_CHECK,
	MP2100_THREAD_PDT_IF,
	MP2100_THREAD_MAX
};

enum MP2100_BOARD
{
	// MP2100 BOARD ����
	MP2100_BOARD_1=0,
	MP2100_BOARD_MAX
};
enum NMC2_CTRL
{
	NMC2_MODULE_1 = 0,
	NMC2_MODULE_2 = 1,
	NMC2_MODULE_3 = 2,
	NMC2_MODULE_4 = 3,
	NMC2_MODULE_5 = 4,
	NMC_CTRL_MAX
};

enum	POS_RANGE_COMPAIR
{
	POS_EQUAL=0,	// ���� ��ġ�� ����	(Motor ���� ���ε� ���� Ȯ���Ѵ�)
	POS_LOWER,		// ���� ��ġ���� �۰ų� ���� (Motor ������ Ȯ�� ���Ѵ�)
	POS_UPPER,		// ���� ��ġ���� ���ų� ���� (Motor ������ Ȯ�� ���Ѵ�)
};


class AxisMoveParam	// �̵���ɿ� ���� �Ķ����. �� �̵��Լ����� �ʿ��� �Ÿ� ���پ���
{
public:
	AxisMoveParam(void)
	{
		idAxis = AXIS_ID_MAX;
		dAcc = 0.;
		dDec = 0.;
		dSpeed = 0.;
		dTargetPosition = 0.;
	}

	AXIS_ID idAxis;
	double dAcc;//����
	double dDec;//����
	double dSpeed;//�ӵ�
	double dTargetPosition;//��ġ

	void operator=(const TEACH_PARAM &srcParam);
};

class AxisStatus
{
public:
	AxisStatus(void)
	{
		bServoOn = FALSE;
		bAlarmOn = FALSE;
		bWarningOn = FALSE;
		bBusy = FALSE;
		bHomeOn = FALSE;;
		bPLimitOn = FALSE;
		bNLimitOn = FALSE;
		bOriginComplete = FALSE;
	}

	BOOL bServoOn;
	BOOL bAlarmOn;
	BOOL bWarningOn;
	BOOL bBusy;
	BOOL bHomeOn;
	BOOL bPLimitOn;
	BOOL bNLimitOn;
	BOOL bOriginComplete;
};

struct CHAIN_INTERPOLATION_PARAM
{
	int nGroupNo;// NMC 0~3������ �׷� 0, 4~7������ �׷� 1
	AXIS_ID axis1;
	AXIS_ID axis2;
	AXIS_ID axis3;		// 2�ຸ���̸� ���⿡�� AXIS_ID_MAX�� ���´�
	double dSpeed;

	CHAIN_INTERPOLATION_PARAM()
	{
		nGroupNo = -1;
		axis1 = AXIS_ID_MAX;
		axis2 = AXIS_ID_MAX;
		axis3 = AXIS_ID_MAX;
		dSpeed = 0.;
	}
};


class CDeviceMotion
{
public:
	CDeviceMotion(void);
	~CDeviceMotion(void);

	CMp2100 m_Mp2100[MP2100_THREAD_MAX][MP2100_BOARD_MAX];
	//20160921 kw
	CNmc2 m_Nmc2[NMC_CTRL_MAX];

	BOOL	Open_Mp2100(MP2100_THREAD thr);
	void Close_Mp2100();

	BOOL Open_NMC2();
	void Close_NMC2();


	BOOL ServoOn(MP2100_THREAD thr, AXIS_ID axis);
	BOOL ServoOff(MP2100_THREAD thr, AXIS_ID axis);

	BOOL ResetAlarm(MP2100_THREAD thr, AXIS_ID axis);
	//void SetPosition(MP2100_THREAD thr, double dPosition);	// ������ ���� ��ġ���� �ٲ۴�
	void SetPosition(MP2100_THREAD thr, AXIS_ID axis, double dPosition);	// //20160923 kw ������ �ʿ��Ͽ� �߰�

	BOOL AxisMove(MP2100_THREAD thr, AXIS_ID axis, AxisMoveParam param);
	BOOL AxisMoveInc(MP2100_THREAD thr, AXIS_ID axis, AxisMoveParam param);
	BOOL AxisMoveJog(MP2100_THREAD thr, AXIS_ID axis, BOOL bForward, AxisMoveParam param);
	BOOL OriginRetrun(MP2100_THREAD thr, AXIS_ID axis);

	void AxisStop(MP2100_THREAD thr, AXIS_ID axis);			// ���� ���� �� ����
	void AxisStopEMS(MP2100_THREAD thr, AXIS_ID axis);		// ������

	AxisStatus GetAxisStatus(MP2100_THREAD thr, AXIS_ID axis);
	CString GetAlarmCode(MP2100_THREAD thr, AXIS_ID axis);	// Ȥ�� ��ü���� 10����, 16���� �� ǥ�� ���̰� ������� ���ڿ��� ó���Ѵ�
	double GetTorque(MP2100_THREAD thr, AXIS_ID axis);		// int? double?
	BOOL GetPosition(MP2100_THREAD thr, AXIS_ID axis, double &dPos);
	BOOL GetTargetPosition(MP2100_THREAD thr, AXIS_ID axis, double &dPos);	// �̵��� ��ǥ��ġ

	BOOL IsAxisReady(MP2100_THREAD thr, AXIS_ID axis, BOOL bIgnoreOrigin);	// �ش� ���� ���� ���� �� �ִ� �������� Ȯ��
	BOOL IsAxisStop(MP2100_THREAD thr, AXIS_ID axis);		// �ش� ���� �����Ǿ� �ִ��� Ȯ��
	BOOL IsTrouble(MP2100_THREAD thr, AXIS_ID axis);		// �࿡ ������ �ִ��� ������ Ȯ��(Busy�ʹ� ������� Alarm�̳� Limit ���� ������Ҹ� Ȯ��)

	// ƼĪ�� ����
	BOOL TeachMove(MP2100_THREAD thr, TEACH_PARAM::ID idTeach, double dOffset = 0., double dSpeedRatio=1., BOOL bIgnoreInterlock = FALSE);
	BOOL CheckAxisPosition(MP2100_THREAD thr, AXIS_ID axis, double dCheckPos, double dRange = 0.05, POS_RANGE_COMPAIR rangeCompair = POS_EQUAL );
	BOOL CheckTeachPosition(MP2100_THREAD thr, TEACH_PARAM::ID idTeach, double dRange = 0.05, POS_RANGE_COMPAIR rangeCompair = POS_EQUAL, double dOffset = 0.);
	//kjpark 20161030 ����ġ�� �ƴϸ� A���� A���� ������ ���̾�α�â ���
	BOOL CheckPosition(double dDefaultPos, double dNowPos, double dCheckLimit /*= 0.01*/ );

	double PosToPulse(double dPos, double dGearRatio);
	double PulseToPos(double dPulse, double dGearRatio);

	//////////////////////////////////////////////////////////////////////////
	// ���Ӻ����̵� [5/30/2017 OSC]
private:
	CHAIN_INTERPOLATION_PARAM m_ChainInterpolationParam;
	int m_nChainInterpolationDevice;	// �����̵��� �� NMC ���
public:
	void ChainInterpolation_CreateGroupStart(CHAIN_INTERPOLATION_PARAM param);		// Group ���� ����
	void ChainInterpolation_AddPosition(CNmc2::INTERPOLATION_POSITION pos);			// ������ �߰�
	void ChainInterpolation_CreateGroupEnd();										// Group ���� ����
	void ChainInterpolation_Run();													// �����̵� ����
	void ChainInterpolation_Stop();													// �����̵� �ߴ�
	BOOL ChainInterpolation_IsStop();												// TRUE : �����̵���. FALSE : �����̵����� �ƴ�
	//////////////////////////////////////////////////////////////////////////
};

