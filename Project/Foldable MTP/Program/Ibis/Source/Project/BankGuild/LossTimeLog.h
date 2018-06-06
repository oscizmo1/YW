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
#define TARGET_TACT	6.		// ��ǥ�� �Ǵ� Tact. ó���ӵ��� �̰ź��� ũ�� Loss



#define TP_CODE_IDLE_RUNDOWN_RUNDOWN		_T("01000")	
#define TP_CODE_BM_BREAK_DOWN				_T("02000")	// ���� �˶� �߻��� �񰡵��� ��Ͽ� [7/10/2017 OSC]

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
	TLosstimeList		m_rLosstimeList;						// �񰡵� �׸���� �����͸� �����ϴ� ����ü
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
	CString GetTitle();			// ���� ù ������ ��ϵ� Title

	void SetMachineStopTime();	// ���� �ð��� ���� ���� ���� �ð����� �Է�
	void SetMachineResetTime();	// ���� �ð��� ���� �˶� ���� �ð����� �Է�
	void SetMachineStartTime();	// ���� �ð��� ���� ���� ���� �ð����� �Է�

	CSystemTime GetMachineStopTime();	// �����Ǳ� ������ �ð� ��ȯ
private:
	void SaveMachineStopTime();
	void SaveMachineResetTime();
	void SaveLastLosstimeItem(BOOL bWirteAlarmCode);
public:
	void ReadLastLossTimeItem();

	void ResetStringArray();	// String �迭�� �ʱ�ȭ�Ѵ�
	void DataToStringArray(BOOL isReset = FALSE);	// �� ������ ����� Data�� String �迭�� ����
	CString GetLogValue();		// String �迭�� �ִ� ������ �α� ��� ���� ���ٷ� �����Ͽ� return

	BOOL ShowLossTimeDlg(BOOL bCanNotCancel);
	void SetAlarmLossTime(int nAlarmId, CString strAlarmText);

	CFile m_fileLossTime;
	BOOL WriteLossTimeLog(BOOL bYesterday);

	void ChangeDay();

	void WriteLossTime_IDLE();

private:
	// ��� ���� ���� �ʾ����� Ȯ���ϱ� ���� ����Ʈ
	// �� ���� �Ϸ�� ������ AddTail�Ѵ�
	// ���߿� GetTail�ؼ� ���� Zone�� ���� �ʰ� ��������
	CList <ZONE_ID, ZONE_ID> m_listZoneFinish;
public:
	void AddZoneFinish(ZONE_ID zone);

};

