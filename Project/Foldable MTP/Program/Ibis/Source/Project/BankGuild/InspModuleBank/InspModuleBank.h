#pragma once

#include "InspModule_CellLoading.h"
#include "InspModule_MTPWrite.h"
#include "InspModule_MTPVerify.h"
#include "InspModule_WhiteCurrent.h"
#include "InspModule_SleepCurrent.h"
#include "InspModule_HlpmCurrent.h"
#include "InspModule_TspStart.h"
#include "InspModule_EvtVersionCheck.h"
#include "InspModule_TeCheck.h"
#include "InspModule_IdCheck.h"
#include "InspModule_OtpRegCheck.h"
#include "InspModule_IctTest.h"
#include "InspModule_CoprIctTest.h"
#include "InspModule_PocErrorCheck.h"
#include "InspModule_DdiBlockTest.h"
#include "InspModule_OptionCheck2.h"
#include "InspModule_OptionCheck3.h"
#include "InspModule_OptionCheck4.h"
#include "InspModule_OptionCheck5.h"
#include "InspModule_OptionCheck6.h"
#include "InspModule_OptionCheck7.h"
#include "InspModule_OptionCheck8.h"
#include "InspModule_OptionCheck9.h"
#include "InspModule_OptionCheck10.h"


class CInspModuleBank
{
public:
	CInspModuleBank(void);
	~CInspModuleBank(void);

	CInspModule_CellLoading m_ModuleCellLoading[JIG_ID_MAX];
	CInspModule_MTPWrite m_ModuleMTPWrite[JIG_ID_MAX];
	CInspModule_MTPVerify m_ModuleMTPVerify[JIG_ID_MAX];
	CInspModule_WhiteCurrent m_ModuleWhiteCurrent[JIG_ID_MAX];
	CInspModule_SleepCurrent m_ModulesleepCurrent[JIG_ID_MAX];
	CInspModule_HlpmCurrent m_ModuleHlpmCurrent[JIG_ID_MAX];
	CInspModule_TspStart m_ModuleTspStart[JIG_ID_MAX];
	CInspModule_EvtVersionCheck m_ModuleEvtVersionCheck[JIG_ID_MAX];
	CInspModule_TeCheck m_ModuleTeCheck[JIG_ID_MAX];
	CInspModule_IdCheck m_ModuleIdCheck[JIG_ID_MAX];
	CInspModule_OtpRegCheck m_ModuleOtpRegCheck[JIG_ID_MAX];
	CInspModule_IctTest m_ModuleIctTest[JIG_ID_MAX];
	CInspModule_CoprIctTest m_ModuleCoprIctTest[JIG_ID_MAX];
	CInspModule_PocErrorCheck m_ModulePocErrorCheck[JIG_ID_MAX];
	CInspModule_DDIBlockTest m_ModuleDDiBlockTest[JIG_ID_MAX];
	CInspModule_OptionCheck2 m_ModuleOptionCheck2[JIG_ID_MAX];
	CInspModule_OptionCheck3 m_ModuleOptionCheck3[JIG_ID_MAX];
	CInspModule_OptionCheck4 m_ModuleOptionCheck4[JIG_ID_MAX];
	CInspModule_OptionCheck5 m_ModuleOptionCheck5[JIG_ID_MAX];
	CInspModule_OptionCheck6 m_ModuleOptionCheck6[JIG_ID_MAX];
	CInspModule_OptionCheck7 m_ModuleOptionCheck7[JIG_ID_MAX];
	CInspModule_OptionCheck8 m_ModuleOptionCheck8[JIG_ID_MAX];
	CInspModule_OptionCheck9 m_ModuleOptionCheck9[JIG_ID_MAX];
	CInspModule_OptionCheck10 m_ModuleOptionCheck10[JIG_ID_MAX];

	// Name으로 해당 Module Pointer를 찾아서 반환한다 [8/10/2017 OSC]
	CInspModule_Base* GetModule(CString strModuleName, JIG_ID jig);
	// 모듈이 해당 벡터 안에 있는지 확인하여 해당 Index를 넘겨준다. 없으면 return -1 [9/8/2017 OSC]
	int FindModule(std::vector<CString> *pVector, CString strModuleName);


	// 주어진 모듈이름 다음에 이어서 할 모듈을 반환하는 함수 [8/10/2017 OSC]
private:
	CInspModule_Base* GetNextModule(std::vector<CString> *pVector, CString strModuleName, JIG_ID jig);
public:
	CInspModule_Base* GetNextModule_AZone_Bef(CString strModuleName, JIG_ID jig);
	CInspModule_Base* GetNextModule_AZone_Must(CString strModuleName, JIG_ID jig);
	CInspModule_Base* GetNextModule_AZone_Aft(CString strModuleName, JIG_ID jig);
	CInspModule_Base* GetNextModule_BZone_Bef(CString strModuleName, JIG_ID jig);
	CInspModule_Base* GetNextModule_BZone_Must(CString strModuleName, JIG_ID jig);
	CInspModule_Base* GetNextModule_BZone_Aft(CString strModuleName, JIG_ID jig);

	CString FindWorkZone(CString strModuleName);
};