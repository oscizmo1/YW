#pragma once

enum STATUS_AVAILABILITY
{
	STATUS_AVAILABILITY_DOWN = 1,	// 설비 DOWN
	STATUS_AVAILABILITY_UP = 2		// 설비 NORMAL
};

enum STATUS_INTERLOCK
{
	STATUS_INTERLOCK_ON = 1,		// 인터락 ON
	STATUS_INTERLOCK_OFF = 2		// 인터락 OFF
};

enum STATUS_MOVE
{
	STATUS_MOVE_PAUSE = 1,			// 설비 PAUSE
	STATUS_MOVE_RUNNING = 2			// 설비 RUN
};

enum STATUS_RUN
{
	STATUS_RUN_NOT_CELL = 1,		// 설비 안에 셀이 있음
	STATUS_RUN_IS_CELL = 2			// 설비 안에 셀이 없음
};

enum STATUS_FRONT
{
	STATUS_FRONT_UP = 1,			// 앞설비 Normal
	STATUS_FRONT_DOWN = 2			// 앞설비 Down
};

enum STATUS_REAR
{
	STATUS_REAR_UP = 1,				// 뒷설비 Normal
	STATUS_REAR_DOWN = 2			// 뒷설비 Down
};

enum STATUS_SAMPLE
{
	STATUS_SAMPLE_ON = 1,			// 샘플런 ON
	STATUS_SAMPLE_OFF = 2			// 샘플런 OFF
};


class CUnitStatus
{
public:
	CUnitStatus(void);
	~CUnitStatus(void);

	CString m_strUnitID;
	STATUS_AVAILABILITY m_AvilabilityState;
	STATUS_INTERLOCK m_InterlockState;
	STATUS_MOVE m_MoveState;
	STATUS_RUN m_RunState;
	STATUS_FRONT m_FrontState;
	STATUS_REAR m_RearState;
	STATUS_SAMPLE m_PP_SplState;
	CString m_strResonCode;
	CString m_strDescrition;

	STATUS_AVAILABILITY m_oldAvilabilityState;
	STATUS_INTERLOCK m_oldInterlockState;
	STATUS_MOVE m_oldMoveState;
	STATUS_RUN m_oldRunState;
	STATUS_FRONT m_oldFrontState;
	STATUS_REAR m_oldRearState;
	STATUS_SAMPLE m_oldPP_SplState;

	CCellInfo* m_pCell;

	void InitStatus();

	BOOL SetAvilability(STATUS_AVAILABILITY value);
	// Set 조건 : 인터락 발생시
	BOOL SetInterlock(STATUS_INTERLOCK value);
	// Set 조건 : 인터락 or Not Use 설정됬을 때 
	// Reset 조건 : 인터락, Not Use 모두 해제
	BOOL SetMove(STATUS_MOVE value);			
	// Set 조건 : 셀 있음
	// Reset 조건 : 셀 없음
	BOOL SetRun(STATUS_RUN value);

	CString ToString();
	void FromString(CString str);
};

