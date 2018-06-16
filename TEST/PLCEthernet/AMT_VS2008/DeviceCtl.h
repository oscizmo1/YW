#pragma once

#include "CIF32.h"
#include "MMC39.h"
#include "AmtData.h"

class CDeviceCtl  
{
public:
	void SetMMC(CMMC39 *  pMotion) {m_pMotion = pMotion ;}
	void SetAmtData(CAmtData *  pAmpData) {m_pAmtData = pAmpData ;}

	void FrontLightUnit(BOOL bStatus);
	void BackLightUnit(BOOL bStatus);
	void ClampUnit(BOOL bStatus);
	void BlowUnit(BOOL bStatus);
	void ContactUnit(BOOL bStatus);

	void EnableAreaSensor(BOOL bCheckEnable);	// TRUE : Area Sensor Check �� ��ȿȭ��ȣ Off
	void SetSafeTilting(BOOL bSafe);			// FALSE : Tilting ��ȣ off(������ ��ġ), TRUE : Tilting ��ȣ On(�����ġ)
	void SetPCRun(BOOL bSet);				// FALSE : RUN Bit & PIO Ready On, TRUE : RUN Bit & PIO Ready Off (START, READY���� ���� TRUE)

	CDeviceCtl();
	virtual ~CDeviceCtl();

	//////////////////////////////////////////////////////////////////////////
	// singleton
public:
	static CDeviceCtl* Instance();
	static void Release();
private:
	static CDeviceCtl* _instance;
	//////////////////////////////////////////////////////////////////////////

public:
	CPLCComm*		m_pMelsec;

	BOOL ChkArmStretch();	// TRUE�� Arm ����(stretch) ����
	BOOL ChkPanelExist();	// TRUE�� Panel ����
	BOOL ChkClampUnit(BOOL bSensorOff = TRUE);
	void SetMovePositionData(int iTarget);
	void SetMovePositionData_Auto(int nCamNum, int nBackOffset=0);//  [6/13/2009 janghan]
	int CheckMoveStageInterference(int iTargetPos);
	int MoveStage(int iTargetPos , bool bEndCheck = TRUE);//  [6/12/2009 janghan]
	int MoveStage_Auto(int iTargetPos, int nCamNum, bool bEndCheck = TRUE);
	void SetMoveStageSoftLimit(int iAxis , bool bEvent);
	int ChkContactUnitUp();
	void MoveYReady();

	BOOL m_bMoveTilting;	// Tilting Move���� �Լ��� �������� ���� TRUE
	int StartMoveTilting(BOOL bTilt);
	BOOL m_bTilt;
	int CheckMoveTilting(BOOL bTilt);

	// Form���� ��ư���� ���۽�Ű�� Sequence
	void ManualSeqContactUnitMove(int &nMoveAction, BOOL &bPosMoving);

	CMMC39* m_pMotion;
	CAmtData* m_pAmtData;

	BOOL	m_bEnableAreaSensor;	// TURE : ��ȿȭ��ȣ OFF, FLASE : ��ȿȭ��ȣ ON
	BOOL	m_bIgnoreInterlock;		// TURE : Arm stretch ����, FLASE : Arm stretch üũ

	int		m_nMoveStep;
	int		m_iTargetPos;
	CString strMoveTarget;
	CString strMovePosition;

	POSITION_DATA	ContactMovePos[MAX_CAM_NUM];

	double m_dTimer;
	DWORD m_dToggleTimer;


	BOOL ChkMainPower(void);
	void SetAlarmBit(BOOL bOn);

	void DeviceLog(int nAxis, double dCommendPos, double dEncoderPos, CString strMoveTarget);

};

// inline CDeviceCtl* GetDeviceCtl() {	return CDeviceCtl::Instance();	}
