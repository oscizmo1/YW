#pragma once

//////////////////////////////////////////////////////////////////////////
// ConfigSystem
// 1. ��� Parameter �߿��� �ֿ켱���� �д´�.
// 2. �߿�õǴ� ������ �������� �����ϴ� ��
// 2. �Ժη� �ٲ��� �ʴ� �κ��̹Ƿ� ������ ���� UI�� �������� �ʴ´�.
//////////////////////////////////////////////////////////////////////////
// System ������ �׻� �������� ����...
#define PATH_SYSTEM	_T("D:\\YWDSP\\MachineData")
//////////////////////////////////////////////////////////////////////////

class CConfigSystem
{
public:
	CConfigSystem(void);
	~CConfigSystem(void);

	//////////////////////////////////////////////////////////////////////////
	// Path ����
	CString m_strDebugLogPath;
	CString m_strRecipePath;
	CString m_strRecipeBackupPath;
	CString m_strConfigPath;
	CString m_strConfigBackupPath;
	CString m_strCellLogPath;
	CString m_strLosstimeLogPath;
	//kjpark 20161026 MCR ���� �̹��� ��� ����
	CString m_strMCRImageSavePath;
	CString m_strFDCPath;
	CString m_strProgramDataPath;
	CString m_strMachineDataPath;
	CString m_strPGLogPath;
	//////////////////////////////////////////////////////////////////////////
	// SYSTEM_PARAMETER
	CString m_strLineNo;
	//kjpark 20170828 MoveWindow ����
	DOUBLE m_dDefaultWindow_X;
	DOUBLE m_dDoorOpenWindow_X;
	//kjaprk 20170904 ��� UI 12�ð� Ÿ������ ����
	BOOL m_bWorkPartTime;//TRUE 12�ð� FALSE 8�ð�

	// ���� ���� [10/5/2017 OSC]
	BOOL m_bInlineMode;
	int m_MachineIndex;	// ���� ������ �˻�� 1����ġ���� 2����ġ���� 3����ġ����...

	// ���� ������ CIM Qual ��� [12/15/2017 OSC]
	BOOL m_bCIMQualMode;
	//////////////////////////////////////////////////////////////////////////
	// SYSTEM OFFSET

	void ReadSystemFile(CString strPath);
};

