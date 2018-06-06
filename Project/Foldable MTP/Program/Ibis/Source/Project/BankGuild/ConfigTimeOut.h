#pragma once

//////////////////////////////////////////////////////////////////////////
// CConfigTimeOut
// 1. 기본적인 Option Parameter Class
// 2. 딱히 그룹화하기 애매한 것들은 다 여기 모아놓는다
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

