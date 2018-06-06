#pragma once

enum STATUS_AVAILABILITY
{
	STATUS_AVAILABILITY_DOWN = 1,	// ���� DOWN
	STATUS_AVAILABILITY_UP = 2		// ���� NORMAL
};

enum STATUS_INTERLOCK
{
	STATUS_INTERLOCK_ON = 1,		// ���Ͷ� ON
	STATUS_INTERLOCK_OFF = 2		// ���Ͷ� OFF
};

enum STATUS_MOVE
{
	STATUS_MOVE_PAUSE = 1,			// ���� PAUSE
	STATUS_MOVE_RUNNING = 2			// ���� RUN
};

enum STATUS_RUN
{
	STATUS_RUN_NOT_CELL = 1,		// ���� �ȿ� ���� ����
	STATUS_RUN_IS_CELL = 2			// ���� �ȿ� ���� ����
};

enum STATUS_FRONT
{
	STATUS_FRONT_UP = 1,			// �ռ��� Normal
	STATUS_FRONT_DOWN = 2			// �ռ��� Down
};

enum STATUS_REAR
{
	STATUS_REAR_UP = 1,				// �޼��� Normal
	STATUS_REAR_DOWN = 2			// �޼��� Down
};

enum STATUS_SAMPLE
{
	STATUS_SAMPLE_ON = 1,			// ���÷� ON
	STATUS_SAMPLE_OFF = 2			// ���÷� OFF
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
	// Set ���� : ���Ͷ� �߻���
	BOOL SetInterlock(STATUS_INTERLOCK value);
	// Set ���� : ���Ͷ� or Not Use �������� �� 
	// Reset ���� : ���Ͷ�, Not Use ��� ����
	BOOL SetMove(STATUS_MOVE value);			
	// Set ���� : �� ����
	// Reset ���� : �� ����
	BOOL SetRun(STATUS_RUN value);

	CString ToString();
	void FromString(CString str);
};

