#pragma once

//////////////////////////////////////////////////////////////////////////
// CConfigTimeOut
// 1. �⺻���� Option Parameter Class
// 2. ���� �׷�ȭ�ϱ� �ָ��� �͵��� �� ���� ��Ƴ��´�
#include <vector>
#include <map>
using namespace std;
#include "ProcDataBank.h"


class CConfigTimeOut
{
public:
	CConfigTimeOut(void);
	~CConfigTimeOut(void);

	double m_dCellLoading;
	double m_dMTPWrite;
	double m_dMTPVerify;
	double m_dCoprIctTest;
	double m_dDDIBlockTest;
	double m_dEvtVersionCheck;
	double m_dHlpmCurrent;
	double m_dIctTest;
	double m_dIdCheck;
	double m_dOptionCheck10;
	double m_dOptionCheck2;
	double m_dOptionCheck3;
	double m_dOptionCheck4;
	double m_dOptionCheck5;
	double m_dOptionCheck6;
	double m_dOptionCheck7;
	double m_dOptionCheck8;
	double m_dOptionCheck9;
	double m_dOtpRegCheck;
	double m_dPocErrorCheck;
	double m_dSleepCurrent;
	double m_dTeCheck;
	double m_dTspStart;
	double m_dWhiteCurrent;

	void ReadTimeOutFile(CString strPath);
	void SaveTimeOutFile(CString strPath, CString strBackupPath);

};

