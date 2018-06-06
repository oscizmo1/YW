#pragma once

//////////////////////////////////////////////////////////////////////////
// CConfigOption
// 1. 기본적인 Option Parameter Class
// 2. 딱히 그룹화하기 애매한 것들은 다 여기 모아놓는다
#include <vector>
#include <map>
using namespace std;
#include "ProcDataBank.h"


class CConfigOption
{
public:
	CConfigOption(void);
	~CConfigOption(void);

	// Loof test사용 여부 - LSH171124
	BOOL m_bUseLoofTest;

	// By Pass mode 사용 여부 - LSH171208
	BOOL m_bUseByPass;

	//DOUBLE m_dBZoneCellLoadingTimeOut;//ID Check Timeout + OTP REG Check timeout

	int m_OffsetHour; // 알람 로그 전날 설정 기준값(기본값은 7시)

	double m_dFDCCheckTime;	// 순간적인 비정상값을 보고하지 않기 위한 유예시간 [3/25/2017 OSC]
	BOOL m_bFDCAlarm;		// 설비에서 FDC Alarm을 발생시키는 옵션 [3/28/2017 OSC]

	// CIM에서 내려받는 불량좌표 [5/2/2017 OSC]

	//kjpark 20161022 설정->램프 설정 구현
	map<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST> iMapLampSettingList;

	void ReadOptionFile(CString strPath);
	void SaveOptionFile(CString strPath, CString strBackupPath);

	//20170705 kjpark cell Jobstart 사용할지 
	//kjpark 20170907 Tracking CellInfomation, JobProcess 추가
	BOOL m_bUseCellInforRequest; 	
	//kjpark 20180113 Tmd Version 추가
	BOOL m_bUseHostVersionSend;
	//kjpark 20171010 TMD March 구현
	BOOL m_bUseTMDNameMatch;

	BOOL m_bUseRetryAA;	// CellLoading 실패시 리트라이
	BOOL m_bUseRetryAB;	// AB 리트라이
	BOOL m_bUseNgoption;
	//kjpark 20180119 김현규책임 요청 L 보고를 R로 옵션처리
	BOOL m_bUseNGChange_R;
	//kjpark 20180120 Cell Mixing 처리(강원호 선임)
	BOOL m_bUseCellMixingA3A4;

	//kjpark 20180129 스크랩시에도 셀로그 적음
	BOOL m_bUseIf_NotUseWriteCellLog;
};

