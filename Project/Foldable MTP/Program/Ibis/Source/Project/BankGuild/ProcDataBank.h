#pragma once
#include <vector>
using namespace std;

#include "Etc\StopWatch.h"
#include "ProcDataNGAlarm.h"
#include "ProcDataPG.h"

enum CIM_MSG_TYPE
{
	CIM_OPCALL_MSG,
	CIM_INTERLOCK_MSG,
	CIM_TERMINAL_MSG
};
struct CONST_MACHINE_STATE
{
	CString strName;
	enum ID 
	{
		MACHINE_PAUSE	= 0,
		MACHINE_INIT	,
		MACHINE_CHECK	,	// Restart하는 동안에는 이 상태
		MACHINE_IDLE	,
		MACHINE_RUN		,
		MACHINE_FAULT	,	
		MACHINE_MAINT	,	// Alarm 발생 이후 RESET 누른 상태
		MACHINE_INTERLOCK,	// CIM에서 Interlock Msg가 날라올 경우 상태 변환 [11/28/2016 OSC]
		MACHINE_OPCALL,		// OPCALL.. LAMP 전용이니 상태변경으로 쓰지 말것 [9/29/2017 OSC]
		MACHINE_PMMODE,		// PM mode상태 Lmap변경을 위해 추가 - LSH171128
		MACHINE_MAX
	};
};
extern const CONST_MACHINE_STATE			ConstMachineState[];

struct CONST_CIM_STATE
{
	CString strName;
	enum ID 
	{
		CIM_OFFLINE		= 0,
		CIM_REMOTE		= 1,
		CIM_LOCAL		= 2
	};
};
extern const CONST_CIM_STATE			ConstCimState[];

struct CONST_EQUIP_PART
{
	CString strNameKor;
	CString strNameEng;
	CString strNameVnm;
	enum ID 
	{
		EQUIP_PART_SHUTTLE1	= 0,
		EQUIP_PART_SHUTTLE2,
		EQUIP_PART_SHUTTLE3,
		EQUIP_PART_COMMON
	};
};
extern const CONST_EQUIP_PART			ConstEquipPart[];

struct CONST_EQUIP_UNIT
{
	CString strNameKor;
	CString strNameEng;
	CString strNameVnm;
	enum ID 
	{
		EQUIP_UNIT_LOAD_COMMON	= 0,
		EQUIP_UNIT_LOAD_CONV				,
		EQUIP_UNIT_SCARA_ROBOT			,
		EQUIP_UNIT_BUFFER_STG				,
		EQUIP_UNIT_LD_CELL_TR				,
		EQUIP_UNIT_PREALIGN					,
		EQUIP_UNIT_LD_TR						,
		EQUIP_UNIT_TRAY_COMMON			,
		EQUIP_UNIT_TRAY1						,
		EQUIP_UNIT_TRAY2						,
		EQUIP_UNIT_INSP_COMMON			,
		EQUIP_UNIT_AZONE					,
		EQUIP_UNIT_BZONE					,
		EQUIP_UNIT_CZONE					,
		EQUIP_UNIT_DZONE					,
		EQUIP_UNIT_UNLOAD_COMMON	,
		EQUIP_UNIT_UD_TR						,
		EQUIP_UNIT_UD_STG				,
		EQUIP_UNIT_UD_CELL_TR
	};
};
extern const CONST_EQUIP_UNIT			ConstEquipUnit[];

// ALARM 정보 [11/16/2016 WHLEE]
struct ALARM_INFOMATION
{
	ALARM_ID	iERR_ID;
	CString		szERR_NAME;
	CTime		TimeOccur;
	CTime		TimeClear;
	CString 	szTimeTake;
	CString		szLevel;

	void SetTakeTime()
	{
		CTimeSpan Elapse = TimeClear - TimeOccur;
		szTimeTake.Format(_T("%02d:%02d:%02d"), Elapse.GetHours(), Elapse.GetMinutes(), Elapse.GetSeconds() );
	}

	void WriteErrorBuf();	// cpp로 뺐다

};

//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
struct OPALL_ALARM_INFOMATION
{	
	CString		strUnitID;
	CString		strOpCallAlarm_NAME;
	CTime		TimeOccur;
	CString		strOpCallAlarm_Message;

	void WriteOpCallErrorBuf();	// cpp로 뺐다

};

//kjpark 20170928 OPCall, Interlock. terminel MSG 히스토리 따로 관리
struct INTERLOCK_ALARM_INFOMATION
{	
	CString		strUnit_ID;
	CString		strInterlock_ID;
	CTime		TimeOccur;
	CString		strInterlock_Message;

	void WriteInterlockErrorBuf();	// cpp로 뺐다

};

//kjpark 20170928 OPCall, Interlock. terminel MSG 히스토리 따로 관리
struct TERMINEL_ALARM_INFOMATION
{	
	CString		strTerminelAlarm_NAME;
	CTime		TimeOccur;
	CString		strTerminelAlarm_Message;

	void WriteTerminelErrorBuf();	// cpp로 뺐다

};

//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
#define ALARM_MAX 20
#define INTERLOCKMAX 20
#define JOBSTARTMAX 5
//kjpark 20180101 Teminal MSG 10개 리스트로 표시
#define OPCALLMAX 10

// 설비 구동중에 사용되는 휘발성 Data들 관리하는 Class
class CProcDataBank
{
public:
	CProcDataBank(void);
	~CProcDataBank(void);

	BOOL m_bProgramInitFinish;

private:
	HANDLE		m_hMutex;

	//////////////////////////////////////////////////////////////////////////
	// 설비 상태
private:
	CONST_MACHINE_STATE::ID m_OldMachineState;
	CONST_MACHINE_STATE::ID m_MachineState;
public:
	void ChangeMachineState(CONST_MACHINE_STATE::ID newState);
	CONST_MACHINE_STATE::ID GetMachineState() { return m_MachineState;};
	BOOL MachineIsRunState();
	BOOL ModuleIsRunState(CONST_MACHINE_STATE::ID id);
	//////////////////////////////////////////////////////////////////////////
	// CIM 관련
private:
	CONST_CIM_STATE::ID m_CimState;
public:
	void ChangeCimState(CONST_CIM_STATE::ID newState);
	CONST_CIM_STATE::ID GetCimState() { return m_CimState; }

	BOOL m_bMachineCellExist;
	BOOL m_bJigCellExist[JIG_ID_MAX];

	//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
	int		m_MsgOpCallCnt;
	int		m_MsgInterlockCnt;
	int		m_MsgJobStartCnt;

	CString m_strOPCallUnitID;
	CString m_strOPCallID;
	CString m_strOldOPCallID;
	CString m_strOPCallMsg[10];
	//kjpark 20180101 Teminal MSG 10개 리스트로 표시
	int		m_MsgTerminalCnt;
	CString m_strOldTerminal;
	CString m_strTerminalMsgs[10];

	// 이제 인터락이 내려와도 바로 발생시키지 않는다
	// 그로 인한 잠시 보관해 두는 버퍼 [11/24/2017 OSC]
	CList <INTERLOCK_ALARM_INFOMATION, INTERLOCK_ALARM_INFOMATION> m_listPreInterlockStep;
	CList <INTERLOCK_ALARM_INFOMATION, INTERLOCK_ALARM_INFOMATION> m_listPreInterlockLoad;
	CList <INTERLOCK_ALARM_INFOMATION, INTERLOCK_ALARM_INFOMATION> m_listPreInterlockTrans;
	void PreInterlock_Add(CString strEFST, INTERLOCK_ALARM_INFOMATION info);
	BOOL PreInterlock_IsEmpty(CString strEFST);
	void PreInterlock_Clear(CString strEFST);
	CString m_strInterlockType;
	CString m_strInterlockID;
	CString m_strOldInterlockID;
	CString m_strInterlockMsg[INTERLOCKMAX];

	CString m_strInterlockedUnitID;
	int		m_UnitInterlockCnt;
	CString m_strUnitInterlockID;
	CString m_strUnitInterlockMsg[INTERLOCKMAX];
	CList <INTERLOCK_ALARM_INFOMATION, INTERLOCK_ALARM_INFOMATION> m_listPreUnitInterlock;
	BOOL PreUnitInterlock_IsEmpty(JIG_ID jig);
	void PreUnitInterlock_Add(INTERLOCK_ALARM_INFOMATION info);
	void PreUnitInterlock_Clear();

private:
	BOOL m_bUserLoadingStop;		// 작업자가 로딩스탑을 하고 싶을 때
public:
	BOOL UserLoadingStop_Check();
	void UserLoadingStop_OnOff(BOOL bOn);
	BOOL LoadingStop_IsRequire();	// LoadingStop을 해야 하면 TRUE


	CString m_strTerminalMsg;

	CString m_strControlPCVersion;
	CString m_strDataPCVersion;
	CString m_strEqipID;
	
	void SetOPCall(BOOL bUnit);
	void SetInterlock(CString strInterlockType);
	//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
	void SetJobProcessMsg();
	void SetTerminalMsg();	

	CList <CCellInfo *, CCellInfo *> m_listRetryCell[JIG_ID_MAX][JIG_CH_MAX];
	CCellInfo *m_pRetryCell;
	BOOL RetryCellInfo_Find(JIG_ID jig, JIG_CH ch, CString strCellID);
	void RetryCellInfo_Add(JIG_ID jig, CCellInfo *pCell);
	void RetryCellInfo_Remove(JIG_ID jig, JIG_CH ch);
	void RetryCellInfo_AllTrackOut(JIG_ID jig, JIG_CH ch, int nLimitCount = 0);
	BOOL RetryCellInfo_IsExist();	// 리트라이 위해 쟁여두고 있는 Cell이 있는지 확인 [10/16/2017 OSC]
	//////////////////////////////////////////////////////////////////////////
	// Alarm 관련
	ALARM_MSG m_AlarmMsg[ALM_MAX];
	void ReadAlarmFile();
	void ReadPDTAlarmFile();
	ALARM_ID m_AlarmID;
	SYSTEMTIME m_AlarmTime;
	CONST_EQUIP_PART::ID m_AlarmPart;
	CONST_EQUIP_PART::ID GetEqiupPart_to_AlarmID(ALARM_ID id);
	void AlarmMsgHappen(ALARM_ID id, CString strRemark = _T(""));
	void AlarmHappen(ALARM_ID id, CString strRemark = _T("") );
	void AlarmClear();

	ALARM_ID m_WarningID;
	void WarningHappen(ALARM_ID id, CString strRemark = _T("") );
	void WarningClear();

	BOOL m_OldInput[MAX_IN_ID];
	//////////////////////////////////////////////////////////////////////////
	// 그 외 Run시 휘발성으로 관리되는 변수들

	void ResetTemporarilyValue();		// Lot End 등 설비 內 Cell을 전부 제거하고 처음부터 다시 돌릴 때 이거로 값들을 다 리셋한다

	//////////////////////////////////////////////////////////////////////////
	//Inspection Station Part
	//////////////////////////////////////////////////////////////////////////
	//kjpark 20170912 JobProcess가 Invalid가 되면 팝업 띄워즘
	void SetInvalidJobPorcessNotMachingID();

	CCellDefectInfo_Zone		m_AZone[JIG_ID_MAX][ZONE_ID_MAX];
	BOOL m_bIsSetZone[JIG_ID_MAX][ZONE_ID_MAX];	// 각 존마다 SET_ZONE을 날렸는지 확인. Restart시 중복전송 방지
	BOOL m_bIsZoneEnd[JIG_ID_MAX][ZONE_ID_MAX];	// 해당 존 Sequence가 끝났는지...
	//kjpark 20170912 MCR 중복 리딩 방지
	BOOL m_bMCRStepisFirstStep[JIG_ID_MAX];
	BOOL m_bAZoneReadyPressed[JIG_ID_MAX];		// 작업자 양수버튼 눌렀을 시 TRUE
	//////////////////////////////////////////////////////////////////////////

	BOOL m_bUseLotEnd;// Lot End 등 설비 內 Cell을 전부 제거하고 처음부터 다시 돌릴 때 이 기능을 사용할것인지 유무 추가
	BOOL m_bDryRunMode;
	//////////////////////////////////////////////////////////////////////////
	// Cell or Ch Skip [9/12/2017 OSC]
	BOOL m_bAZoneCellSkip[JIG_ID_MAX][JIG_CH_MAX];		// 1회성 해당 Cell 스킵
	BOOL m_bAZoneChannelNotUse[JIG_ID_MAX][JIG_CH_MAX];	// 해당 채널 스킵 (계속 유지)
	void AZoneChannelNotUse_OnOff(JIG_ID jig, JIG_CH ch, BOOL bOn);
	BOOL AZoneChannelNotUse_Check(JIG_ID jig, JIG_CH ch);
	void AZoneChannelNotUse_SyncUnitStatus();
	void AZoneCellNG_OnOff(JIG_ID jig, JIG_CH ch, BOOL bOn);
	BOOL AZoneCellNG_Check(JIG_ID jig, JIG_CH ch);
	//////////////////////////////////////////////////////////////////////////


	//kjpark 20170831 MCR 구현 채널 별 구현 완료
	BOOL m_bMCRResultRecive[JIG_ID_MAX][JIG_CH_MAX];
	CString m_strLastCellID[JIG_ID_MAX][JIG_CH_MAX];

	//////////////////////////////////////////////////////////////////////////


	map<JIG_CH, B_ZONE_DEFECT> iMapBZoneSkipDefect;

	//kjpark 20161121 MES code list read
	map<int, DEFECT_MES_CODE> iMapDefectList;

	ALARM_INFOMATION	alarm_info;
	void WriteErrorLogCsv();	// ErrorLog.csv 적을 게 있는지 확인해서 적는다 [12/18/2017 OSC]

	//////////////////////////////////////////////////////////////////////////
	//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
	OPALL_ALARM_INFOMATION	opCall_alarm_info;
	void SetWriteOpCallLog(OPALL_ALARM_INFOMATION opcallElement);
	
//kjpark 20170928 OPCall, Interlock. terminel MSG 히스토리 따로 관리
	INTERLOCK_ALARM_INFOMATION	Interlock_alarm_info;
	void SetWriteInterlockLog(INTERLOCK_ALARM_INFOMATION InterloclElement);

//kjpark 20170928 OPCall, Interlock. terminel MSG 히스토리 따로 관리
	TERMINEL_ALARM_INFOMATION	Terminel_alarm_info;
	void SetWriteTerminelLog(TERMINEL_ALARM_INFOMATION TERMINELElement);


//20170211 YJH Tray 배출. 
public:
	void ShowLosstimeDlg();
	//kjpark 20170225 LossTime ANI 보고 배끼기
	CString GetLossTimeAlarmName();

	CProcDataNGAlarm m_DataNGAlarm;

	
	BOOL m_bLabelPrintOut;

	// B존에 불량좌표 그릴 Data
	int m_nSquarePatternCnt[ZONE_ID_MAX][JIG_CH_MAX];
	SQUARE_PATTERN_INFO m_SquarePatternInfo[ZONE_ID_MAX][JIG_CH_MAX][MAX_DEFECT_POINT];
	void AddDZoneSquareData(JIG_CH ch, int nPatternNo, SQUARE_POINT point, ZONE_ID zone);
	void ClearSquareData(JIG_ID jig);


	//////////////////////////////////////////////////////////////////////////
	// Manual로 구동되는 State에서 PG Message 처리하기 위한 임시변수들 [6/16/2017 OSC]
	BOOL m_bEnableManualStateData;
	CCellInfo m_ManualCell[JIG_CH_MAX];
	void InitManualCellData(JIG_ID jig);
	//////////////////////////////////////////////////////////////////////////
	// Interlock Message
	CString m_strLastKorMsg, m_strLastEngMsg, m_strLastVnmMsg;
	//////////////////////////////////////////////////////////////////////////
	BOOL m_bPDT_IF_State_Clear;
	BOOL m_bAbleOnStatus[JIG_ID_MAX];
	BOOL m_bPDT_IF_NoCheckVacOff[JIG_ID_MAX][JIG_CH_MAX];
	void PDT_IF_NoCheckVacOff_Clear();

	//////////////////////////////////////////////////////////////////////////
	SYSTEMTIME m_OldUnloadTime[JIG_ID_MAX][JIG_CH_MAX];
	SYSTEMTIME m_OldWaitTime[JIG_ID_MAX][JIG_CH_MAX];

	//////////////////////////////////////////////////////////////////////////
	CProcDataPG m_PGData[JIG_ID_MAX][JIG_CH_MAX];
};

