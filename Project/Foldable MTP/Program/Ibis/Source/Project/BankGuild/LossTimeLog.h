#pragma once
#include "Etc/SystemTime.h"
#include "Etc\StopWatch.h"
#include "BankGuild\MCDefine.h"

struct LOSSTIME_FIRST_CLASS
{
	CString strName;
	enum ID 
	{
	
		BM	= 0			,	// Break Machine
		PM				,	// PM
		CM				,	// Engineer Down
		ChangeModel		,	// Job Change
		Material		,	// Run Down
		MAX_FIRST
	};

};
extern const LOSSTIME_FIRST_CLASS			LosstimeFirstClass[];

#define MAX_LOSSTIME_LOG_STRING	1024
#define TARGET_TACT	6.		// 목표가 되는 Tact. 처리속도가 이거보다 크면 Loss



#define TP_CODE_IDLE_RUNDOWN_RUNDOWN		_T("01000")	
#define TP_CODE_BM_BREAK_DOWN				_T("02000")	// 설비 알람 발생시 비가동에 기록용 [7/10/2017 OSC]

#define TP_CODE_BM_BREAK_DOWN_MANUAL		_T("03000")
#define TP_CODE_PM_REGULAR_PM				_T("12000")
#define TP_CODE_PM_CHECK_EQUIPMENT			_T("15100")
#define TP_CODE_PM_CHECK_QUALITY			_T("15200")
#define TP_CODE_CM_IMPROVE_EQUIPMENT		_T("17200")
#define TP_CODE_CM_IMPROVE_PROCESSING		_T("17300")
#define TP_CODE_CM_SETUP_NEW_PRODUCT		_T("14000")
#define TP_CODE_CHANGE_MODEL_SAME_MODEL		_T("41100")
#define TP_CODE_CHANGE_MODEL_DIFFER_MODEL	_T("41200")
#define TP_CODE_MATERIAL_CHANGE_MATERIAL	_T("35000")
#define TP_CODE_MATERIAL_DOWN_MATERIAL		_T("51000")


struct TPSystemItem
{
	CString strCode;
	CString strName1st;
	CString strName2nd;
	CString strName3rd;
};

class CLossTimeLog
{

public:
	CLossTimeLog(void);
	~CLossTimeLog(void);
	BOOL LoadLosstimeItemList();
	TLosstimeList		m_rLosstimeList;						// 비가동 항목과련 데이터를 관리하는 구조체
	TLosstimeRecord		m_rLosstimeRecord;

	CString LoadTPItem(CString strCode);
	void LoadTPNameToLosstimeRecode(CString strCode);
	CString LoadTPName(CString strCode);

private:
	CString m_strValue[ITEM_MAX];
	CSystemTime m_StartTime;
	CSystemTime m_ResetTime;
	CSystemTime m_EndTime;



public:
	void SetLostimeElement(int nIdx, CString strDescription = _T(""));
	CString GetLostimeElement(int nIdx);
	CString GetTitle();			// 파일 첫 생성시 기록될 Title

	void SetMachineStopTime();	// 현재 시간을 설비 정지 시작 시간으로 입력
	void SetMachineResetTime();	// 현재 시간을 설비 알람 리셋 시간으로 입력
	void SetMachineStartTime();	// 현재 시간을 설비 가동 시작 시간으로 입력

	CSystemTime GetMachineStopTime();	// 정지되기 시작한 시간 반환
private:
	void SaveMachineStopTime();
	void SaveMachineResetTime();
	void SaveLastLosstimeItem(BOOL bWirteAlarmCode);
public:
	void ReadLastLossTimeItem();

	void ResetStringArray();	// String 배열을 초기화한다
	void DataToStringArray(BOOL isReset = FALSE);	// 각 변수에 저장된 Data를 String 배열로 이전
	CString GetLogValue();		// String 배열에 있는 값들을 로그 기록 위해 한줄로 통합하여 return

	BOOL ShowLossTimeDlg(BOOL bCanNotCancel);
	void SetAlarmLossTime(int nAlarmId, CString strAlarmText);

	CFile m_fileLossTime;
	BOOL WriteLossTimeLog(BOOL bYesterday);

	void ChangeDay();

	void WriteLossTime_IDLE();

private:
	// 어느 존이 가장 늦었는지 확인하기 위한 리스트
	// 각 존이 완료될 때마다 AddTail한다
	// 나중에 GetTail해서 나온 Zone이 가장 늦게 끝난거임
	CList <ZONE_ID, ZONE_ID> m_listZoneFinish;
public:
	void AddZoneFinish(ZONE_ID zone);

};

