#pragma once

class CInspModule_Base : public CUnitCtrl_Base
{
public:
	CInspModule_Base(void);
	~CInspModule_Base(void);

	CString m_ModuleName;
	JIG_ID m_Shuttle;

	void SetShuttle(JIG_ID jig);	// MTP�� ���� ������ ���ֵ� ��

	void	Start() ;


protected:
	CUnitCtrlFunc m_Func;
};

