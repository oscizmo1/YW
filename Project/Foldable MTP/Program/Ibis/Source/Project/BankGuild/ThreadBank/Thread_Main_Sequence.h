#pragma once

#include "ThreadBank.h"

class CThread_Main_Sequence
{
public:
	CThread_Main_Sequence(void);
	~CThread_Main_Sequence(void);

	static UINT MainSequenceThr(LPVOID lpParam);

private:
	BYTE m_bRtnComp[MAX_RTN_SIZE];	
	BYTE m_bRtn[MAX_RTN_SIZE];

	void ResetReturnValue();
	BOOL IsReturnOk();


	CUnitCtrlFunc *m_pfunc;

public:
	CStateRestart		*m_pRestart;
	CStateAZone			*m_pAZone[JIG_ID_MAX];
	CStateBZone			*m_pBZone[JIG_ID_MAX];
	CStateMoveBZone		*m_pMoveBZone[JIG_ID_MAX];
	CStateMoveAZone		*m_pMoveAZone[JIG_ID_MAX];
	CStatePDT_IF		*m_pIF[JIG_ID_MAX];


	CELL_POS m_CellPosCh1[JIG_ID_MAX];
	CELL_POS m_CellPosCh2[JIG_ID_MAX];


	void StateAllReset();
	void StateAllRun();

	void GetStatePointer( );

	void GetCELL_POS();

	void Cell_CheckExist(CUnitCtrlFunc &_func);
	BOOL Confirm_Interlock(CUnitCtrlFunc &_func);
	void ConfirmStart_AZone(CUnitCtrlFunc &_func, JIG_ID jig);
	void ConfirmStart_MoveBZone(CUnitCtrlFunc &_func, JIG_ID jig);
	void ConfirmStart_MoveAZone(CUnitCtrlFunc &_func, JIG_ID jig);
	void ConfirmStart_BZone(CUnitCtrlFunc &_func, JIG_ID jig);

};

