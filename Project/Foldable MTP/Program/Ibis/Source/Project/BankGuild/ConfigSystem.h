#pragma once

//////////////////////////////////////////////////////////////////////////
// ConfigSystem
// 1. 모든 Parameter 중에서 최우선으로 읽는다.
// 2. 중요시되는 폴더나 설정등을 저장하는 곳
// 2. 함부로 바뀌지 않는 부분이므로 가급적 편집 UI는 구현하지 않는다.
//////////////////////////////////////////////////////////////////////////
// System 폴더는 항상 고정으로 가자...
#define PATH_SYSTEM	_T("D:\\YWDSP\\MachineData")
//////////////////////////////////////////////////////////////////////////

class CConfigSystem
{
public:
	CConfigSystem(void);
	~CConfigSystem(void);

	//////////////////////////////////////////////////////////////////////////
	// Path 정보
	CString m_strDebugLogPath;
	CString m_strRecipePath;
	CString m_strRecipeBackupPath;
	CString m_strConfigPath;
	CString m_strConfigBackupPath;
	CString m_strCellLogPath;
	CString m_strLosstimeLogPath;
	//kjpark 20161026 MCR 리드 이미지 경로 변경
	CString m_strMCRImageSavePath;
	CString m_strFDCPath;
	CString m_strProgramDataPath;
	CString m_strMachineDataPath;
	CString m_strPGLogPath;
	//////////////////////////////////////////////////////////////////////////
	// SYSTEM_PARAMETER
	CString m_strLineNo;
	//kjpark 20170828 MoveWindow 적용
	DOUBLE m_dDefaultWindow_X;
	DOUBLE m_dDoorOpenWindow_X;
	//kjaprk 20170904 통계 UI 12시간 타임으로 변경
	BOOL m_bWorkPartTime;//TRUE 12시간 FALSE 8시간

	// 물류 연동 [10/5/2017 OSC]
	BOOL m_bInlineMode;
	int m_MachineIndex;	// 물류 연동시 검사기 1번위치인지 2번위치인지 3번위치인지...

	// 물류 연동시 CIM Qual 모드 [12/15/2017 OSC]
	BOOL m_bCIMQualMode;
	//////////////////////////////////////////////////////////////////////////
	// SYSTEM OFFSET

	void ReadSystemFile(CString strPath);
};

