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

	void EnableAreaSensor(BOOL bCheckEnable);	// TRUE : Area Sensor Check 및 무효화신호 Off
	void SetSafeTilting(BOOL bSafe);			// FALSE : Tilting 신호 off(위험한 위치), TRUE : Tilting 신호 On(대기위치)
	void SetPCRun(BOOL bSet);				// FALSE : RUN Bit & PIO Ready On, TRUE : RUN Bit & PIO Ready Off (START, READY중일 때만 TRUE)

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

	BOOL ChkArmStretch();	// TRUE면 Arm 진입(stretch) 상태
	BOOL ChkPanelExist();	// TRUE면 Panel 감지
	BOOL ChkClampUnit(BOOL bSensorOff = TRUE);
	void SetMovePositionData(int iTarget);
	void SetMovePositionData_Auto(int nCamNum, int nBackOffset=0);//  [6/13/2009 janghan]
	int CheckMoveStageInterference(int iTargetPos);
	int MoveStage(int iTargetPos , bool bEndCheck = TRUE);//  [6/12/2009 janghan]
	int MoveStage_Auto(int iTargetPos, int nCamNum, bool bEndCheck = TRUE);
	void SetMoveStageSoftLimit(int iAxis , bool bEvent);
	int ChkContactUnitUp();
	void MoveYReady();

	BOOL m_bMoveTilting;	// Tilting Move관련 함수가 실행중일 동안 TRUE
	int StartMoveTilting(BOOL bTilt);
	BOOL m_bTilt;
	int CheckMoveTilting(BOOL bTilt);

	// Form에서 버튼으로 동작시키는 Sequence
	void ManualSeqContactUnitMove(int &nMoveAction, BOOL &bPosMoving);

	CMMC39* m_pMotion;
	CAmtData* m_pAmtData;

	BOOL	m_bEnableAreaSensor;	// TURE : 무효화신호 OFF, FLASE : 무효화신호 ON
	BOOL	m_bIgnoreInterlock;		// TURE : Arm stretch 무시, FLASE : Arm stretch 체크

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
