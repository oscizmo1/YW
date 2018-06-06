#pragma once

class CInspModule_Base : public CUnitCtrl_Base
{
public:
	CInspModule_Base(void);
	~CInspModule_Base(void);

	CString m_ModuleName;
	JIG_ID m_Shuttle;

	void SetShuttle(JIG_ID jig);	// MTP는 존이 없으니 없애도 됨

	void	Start() ;


protected:
	CUnitCtrlFunc m_Func;
};

