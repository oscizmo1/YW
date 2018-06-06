#pragma once

//////////////////////////////////////////////////////////////////////////
// CConfigOption
// 1. �⺻���� Option Parameter Class
// 2. ���� �׷�ȭ�ϱ� �ָ��� �͵��� �� ���� ��Ƴ��´�
#include <vector>
#include <map>
using namespace std;
#include "ProcDataBank.h"


class CConfigOption
{
public:
	CConfigOption(void);
	~CConfigOption(void);

	// Loof test��� ���� - LSH171124
	BOOL m_bUseLoofTest;

	// By Pass mode ��� ���� - LSH171208
	BOOL m_bUseByPass;

	//DOUBLE m_dBZoneCellLoadingTimeOut;//ID Check Timeout + OTP REG Check timeout

	int m_OffsetHour; // �˶� �α� ���� ���� ���ذ�(�⺻���� 7��)

	double m_dFDCCheckTime;	// �������� �������� �������� �ʱ� ���� �����ð� [3/25/2017 OSC]
	BOOL m_bFDCAlarm;		// ���񿡼� FDC Alarm�� �߻���Ű�� �ɼ� [3/28/2017 OSC]

	// CIM���� �����޴� �ҷ���ǥ [5/2/2017 OSC]

	//kjpark 20161022 ����->���� ���� ����
	map<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST> iMapLampSettingList;

	void ReadOptionFile(CString strPath);
	void SaveOptionFile(CString strPath, CString strBackupPath);

	//20170705 kjpark cell Jobstart ������� 
	//kjpark 20170907 Tracking CellInfomation, JobProcess �߰�
	BOOL m_bUseCellInforRequest; 	
	//kjpark 20180113 Tmd Version �߰�
	BOOL m_bUseHostVersionSend;
	//kjpark 20171010 TMD March ����
	BOOL m_bUseTMDNameMatch;

	BOOL m_bUseRetryAA;	// CellLoading ���н� ��Ʈ����
	BOOL m_bUseRetryAB;	// AB ��Ʈ����
	BOOL m_bUseNgoption;
	//kjpark 20180119 ������å�� ��û L ���� R�� �ɼ�ó��
	BOOL m_bUseNGChange_R;
	//kjpark 20180120 Cell Mixing ó��(����ȣ ����)
	BOOL m_bUseCellMixingA3A4;

	//kjpark 20180129 ��ũ���ÿ��� ���α� ����
	BOOL m_bUseIf_NotUseWriteCellLog;
};

