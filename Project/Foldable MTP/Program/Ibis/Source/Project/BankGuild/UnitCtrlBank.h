#pragma once
#include "Device/DeviceMotion.h"
#include "Etc/StopWatch.h"
#include "BankGuild/CellBank.h"
#include "UnitCtrl_Base.h"

//////////////////////////////////////////////////////////////////////////
// Sequenc 식별 Name
#define STATE_FUNC							_T("FUNC")
#define STATE_RESTART						_T("RESTART_")
#define STATE_SHUTTLE1_AZONE				_T("SHUTTLE1_AZONE_")
#define STATE_SHUTTLE1_MOVE_AZONE			_T("SHUTTLE1_MOVE_AZONE_")
#define STATE_SHUTTLE1_MOVE_BZONE			_T("SHUTTLE1_MOVE_BZONE_")
#define STATE_SHUTTLE1_BZONE				_T("SHUTTLE1_BZONE_")
#define STATE_SHUTTLE2_AZONE				_T("SHUTTLE2_AZONE_")
#define STATE_SHUTTLE2_MOVE_AZONE			_T("SHUTTLE2_MOVE_AZONE_")
#define STATE_SHUTTLE2_MOVE_BZONE			_T("SHUTTLE2_MOVE_BZONE_")
#define STATE_SHUTTLE2_BZONE				_T("SHUTTLE2_BZONE_")
#define STATE_SHUTTLE3_AZONE				_T("SHUTTLE3_AZONE_")
#define STATE_SHUTTLE3_MOVE_AZONE			_T("SHUTTLE3_MOVE_AZONE_")
#define STATE_SHUTTLE3_MOVE_BZONE			_T("SHUTTLE3_MOVE_BZONE_")
#define STATE_SHUTTLE3_BZONE				_T("SHUTTLE3_BZONE_")

#define STATE_SHUTTLE1_PDT_IF				_T("SHUTTLE1_PDT_IF")
#define STATE_SHUTTLE2_PDT_IF				_T("SHUTTLE2_PDT_IF")
#define STATE_SHUTTLE3_PDT_IF				_T("SHUTTLE3_PDT_IF")

#define STATE_CORELATION					_T("CORELATION")

//////////////////////////////////////////////////////////////////////////

#define MAX_RTN_SIZE	20

class CUnitCtrlBank : public CUnitCtrl_Base
{
protected:
	CUnitCtrlBank()
		: CUnitCtrl_Base()	{};
public:
	CUnitCtrlBank(CString StateName, MP2100_THREAD nThreadID, BOOL bManual = FALSE, LOG_INDEX logindex = LOG_SEQUENCE)
			 : CUnitCtrl_Base(StateName, nThreadID, bManual, logindex)	{};

	~CUnitCtrlBank(void) {};

	// Shuttle1 기준으로 Alarm 번호와 현재 Shuttle Index를 넘겨주면 해당 Shuttle에 맞는 Alarm ID를 반환해줌 [8/30/2017 OSC]
	ALARM_ID GetAlarmID_of_Shuttle(ALARM_ID id, JIG_ID jig);

	BOOL CheckOriginInterlock(AXIS_ID idAxis);	
	// TeachMove 안에서 호출되며 Move Start가 되기 전에 체크하여 FALSE면 이동명령을 무시하고 return한다
	BOOL CheckTeachMoveInterlock(TEACH_PARAM::ID idTeach, double dOffset = 0.);	
	void TeachMove(TEACH_PARAM::ID idTeach, double dOffset = 0.);

	BOOL CellTagExist(CELL_POS pos);
	//kjpark 20170915 Restart 구현
	BOOL CellTagExist(JIG_ID jig, JIG_CH channel);
	BOOL CellTagExist(CELL_POS posStart, CELL_POS posEnd, CHECK_OPTION chkoption = CHECK_OR);
	void CellTagChange(CELL_POS posBefore, CELL_POS posAfter);

	void PreInterlockHappen(CString strEFST);
	void PreUnitInterlockHappen();
	/*------------------------------------------------------------------------*/
	//Inspection Station Part
	/*------------------------------------------------------------------------*/
public:	
	void JudgeFinalClass(JIG_ID jig, JIG_CH ch);
	CString MesCode(CString strDefectName, BOOL &bRetryAble, int nJudge = CONST_JUDGE_NAME::JUDGE_MCR);
	int GetDefectFromJudge( ZONE_ID zone, CCellInfo *pCell, CString &strDefect, CLASS_CELL &nClass );
	int AZoneJudgeFlow( CCellInfo *pCell, CString &strDefect, CLASS_CELL &nClass );
	int BZoneJudgeFlow( CCellInfo *pCell, CString &strDefect, CLASS_CELL &nClass );
	int MCRJudgeFlow( CCellInfo *pCell, CString &strDefect, CLASS_CELL &nClass );
	int LastJudgeFlow( CCellInfo *pCell, CString &strDefect, CLASS_CELL &nClass );
	CONST_JUDGE_NAME::ID ModuleListJudgeFlow( std::vector<CString> *pVector, CCellInfo *pCell, CString &strDefect, CLASS_CELL &nClass );

	void JudgeZoneDefect(JIG_ID jig, ZONE_ID defectzone);
	//kjpark 20180120 Cell Mixing 처리(강원호 선임)
	BOOL GetCellSkipCheck( JIG_ID jig, JIG_CH nCh, BOOL bCheckContinue = FALSE);

	void Product_CountUpdate(JIG_ID jig);

	void AZoneCell_RemoveSkipCell(JIG_ID jig);	// 작업자가 SKIP이라고 처리한 채널의 CellTag를 삭제한다 [9/13/2017 OSC]

	BOOL CellInfo_GetInspFinish(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);		// 이 셀이 검사가 끝난 셀인지 확인

	void CellLog_SetOperatorID(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	void CellLog_TactTime_SetStartTime(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	void CellLog_TactTime_SetEndTime(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	void CellLog_SetUnloadTime(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	void CellLog_WaitTime_SetStartTime(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	void CellLog_WaitTime_SetEndTime(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	void CellLog_LoadingTactTime_SetStartTime(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	void CellLog_LoadingTactTime_SetEndTime(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	void CellLog_MCRReadTime_SetStartTime(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	void CellLog_MCRReadTime_SetEndTime(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	void CellLog_AZoneETCTime_SetStartTime(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	void CellLog_AZoneETCTime_SetEndTime(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	void CellLog_BZoneMTPReadyTime_SetStartTime(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	void CellLog_BZoneMTPReadyTime_SetEndTime(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	void CellLog_BZoneETCTime_SetStartTime(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	void CellLog_BZoneETCTime_SetEndTime(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	void CellLog_Write(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	//kjpark 20180107 신호기 로그에서 MTP 측정 값 가지고와서 셀로그에 넣기
	void GetMTP_Isnpection_Value(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	//////////////////////////////////////////////////////////////////////////
	// CIM [11/30/2016 OSC]
	//////////////////////////////////////////////////////////////////////////
	void CIM_BZoneCellInfoRequest();			// Cell ID를 통해 Cell 정보 요청. [11/30/2016 OSC]	
	void CIM_BZoneCellTrackIn();			// 설비에 들어왔다고 상위에 보고. [11/30/2016 OSC]
	void CIM_AZoneCellDefectInfoRequest();	// 불량좌표 정보 여청
	void CIM_CellExistInMachine();			// 설비에 Cell이 있는지, 없는지 보고
	void CIM_CellExistInMachine(BOOL bExist);	// 설비에 Cell이 있는지, 없는지 보고

	//kjpark 20170907 Tracking CellInfomation, JobProcess 추가
	void CIM_CellInfoRequest(JIG_ID jig, JIG_CH nCh = JIG_CH_MAX);			// Cell ID를 통해 Cell 정보 요청. [11/30/2016 OSC]
	//kjpark 20170710 CIM QUAL Jot Start, Cell Infomation Request 사용
	BOOL CheckCellInfomationRecive(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	BOOL CheckCellInfomationSuccess(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	BOOL CheckCellJobStartRecive(JIG_ID jig, JIG_CH ch);
	BOOL CheckCellJobStartSuccess(JIG_ID jig, JIG_CH ch);
	void CIM_SetCellInfoNG(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	void CIM_SetTrackOutNG(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	void CIM_CellTrackIn(JIG_ID jig, JIG_CH nCh = JIG_CH_MAX);			// 설비에 들어왔다고 상위에 보고. [11/30/2016 OSC]
	void CIM_CellCimJudge(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);			// G,R,L판정. [11/30/2016 OSC]
	void CIM_CellLoadingStop(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);		// 리트라이할 것들을 취소하고 L판정으로 변경
	void CIM_CellAllInspectionEnd(JIG_ID jig, JIG_CH nCh = JIG_CH_MAX);	// 모든 검사 종료
	void CIM_CellTrackOut(JIG_ID jig, JIG_CH nCh = JIG_CH_MAX);			// 설비에 나간다고 상위에 보고. D존에서 A존 회전시 한다

	// 현재 셀과 이전셀과 비교해서 리트라이 수행 여부 판단하여
	// 셀아이디가 바꼈으면 TrackOut보고, 안바꼈으면 리트라이 진행 [9/27/2017 OSC]
	void CIM_CellRetryCheck(JIG_ID jig, JIG_CH nCh = JIG_CH_MAX);		
	//////////////////////////////////////////////////////////////////
	//PG Socket Part
	//////////////////////////////////////////////////////////////////
	void SendPGMessage(CString strCommand, JIG_ID jig, CString strExtraData = _T(""), BOOL bIgnoreInspSkip = FALSE);
	void SendPGMessage(CString strCommand, JIG_ID jig, JIG_CH nCh, CString strExtraData = _T(""), BOOL bIgnoreInspSkip = FALSE);

	//kjpark 20170912 MCR에서 읽은 셀아이디 정상 판별
	BOOL GetMCRReadFinish(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);

	void SetZoneEnd(JIG_ID jig, ZONE_ID zone);
	BOOL GetZoneEnd(JIG_ID jig, ZONE_ID zone);
	void ResetZoneEnd(JIG_ID jig);

	void AZoneCellData_Create(JIG_ID jig, JIG_CH ch);
	void InlineCellData_Create(JIG_ID jig, JIG_CH ch, CString strCellID);
	void InlineCellData_Remove(JIG_ID jig, JIG_CH ch);

	// Cell Skip 설정된 것은 한바퀴 갔다 오면 자동 해제해줘야 한다. Not Use는 계속 유지 [9/16/2017 OSC]
	void AZoneCellSkip_Reset(JIG_ID jig);

	void PatternReset_Send(JIG_ID jig, JIG_CH ch);

	void AZone_SetTimeStart(JIG_ID jig);
	void AZone_SetTimeEnd(JIG_ID jig);
	void AZone_SetTimeWait(JIG_ID jig);

	//kjpark 20170912 MCR 위치에따른 택타임 추가
	void AZonetoMCR_SetTimeStart(JIG_ID jig);
	void AZonetoMCR_SetTimeEnd(JIG_ID jig);

	//kjpark 20170912 MCR 위치에따른 택타임 추가
	void MCRtoBZoneSetTimeStart(JIG_ID jig);
	void MCRtoBZoneSetTimeEnd(JIG_ID jig);

	void BZone_SetTimeStart(JIG_ID jig);
	void BZone_SetTimeEnd(JIG_ID jig);
	void BZone_SetTimeWait(JIG_ID jig);

	//kjpark 20170912 MCR 위치에따른 택타임 추가
	void BZonetoAZone_SetTimeStart(JIG_ID jig);
	void BZonetoAZone_SetTimeEnd(JIG_ID jig);

	void SetZoneA_Send(JIG_ID jig);
	BOOL SetZoneA_Check(JIG_ID jig);
	void SetZoneA_TimeOut(JIG_ID jig);
	BOOL AZoneDefect_GoodCheck(JIG_ID jig, JIG_CH channel);

	void SetZoneC_Send(JIG_ID jig);
	BOOL SetZoneC_Check(JIG_ID jig);
	void SetZoneC_TimeOut(JIG_ID jig);

	//kjpark 20161018 TMD_INFO 추가
	void TMD_INFO_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	//kjpark 20180113 Tmd Version 추가
	void Host_Version_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	void Client_Version_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL TMD_INFO_Check(JIG_ID jig, JIG_CH channel);
	void TMD_INFO_Timeout( JIG_ID jig);
	//kjpark 20171010 TMD March 구현
	void CellMixingBin2Input(JIG_ID jig, JIG_CH channel); //20171001 BKH, 혼류 발생시 bin2 입력을 하기 위한 함수 추가

	void CellLoading_SetStartTime(JIG_ID jig, JIG_CH channel);
	void CellLoading_SetEndTime(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	void CellLoading_Send(JIG_ID jig, JIG_CH channel, BOOL bUseInnerID);
	BOOL CellLoading_SendCheck(JIG_ID jig, JIG_CH channel);
	void CellLoading_InitInfo(JIG_ID jig, JIG_CH channel, BOOL bResultOnly);
	BOOL CellLoading_RecvCheck(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	BOOL CellLoading_GoodCheck(JIG_ID jig, JIG_CH channel);
	void CellLoading_TimeOut(JIG_ID jig);
	// CellLoading 전용 변수에 있는 것을 원래 Jig,Ch 변수로 옮긴다 [12/19/2017 OSC]
	void CellLoading_CopyInfo(JIG_ID jig, JIG_CH channel);

	//kjpark 20170911 신호기 command UnitBank 추가
	void MTPWrite_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL MTPWrite_IsStarted(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	BOOL MTPWrite_Check(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	void MTPWrite_TimeOut(JIG_ID jig);

	//kjpark 20170911 신호기 command UnitBank 추가
	void MTPVerify_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL MTPVerify_IsStarted(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	BOOL MTPVerify_Check(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);
	void MTPVerify_TimeOut(JIG_ID jig);

	//kjpark 20170911 신호기 command UnitBank 추가
	void WhiteCurrent_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL WhiteCurrent_Check(JIG_ID jig, JIG_CH channel);
	void WhiteCurrent_TimeOut(JIG_ID jig);

	void SleepCurrent_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL SleepCurrent_Check(JIG_ID jig, JIG_CH channel);
	void SleepCurrent_TimeOut(JIG_ID jig);

	void HlpmCurrent_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL HlpmCurrent_Check(JIG_ID jig, JIG_CH channel);
	void HlpmCurrent_TimeOut(JIG_ID jig);

	//kjpark 20170911 신호기 command UnitBank 추가
	void TSP_START_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL TSP_START_Check(JIG_ID jig, JIG_CH channel);
	void TSP_START_TimeOut(JIG_ID jig);

	//kjpark 20170911 신호기 command UnitBank 추가
	void EVTVersion_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL EVTVersion_Check(JIG_ID jig, JIG_CH channel);
	void EVTVersion_TimeOut(JIG_ID jig);

	void TECheck_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL TECheck_Check(JIG_ID jig, JIG_CH channel);
	void TECheck_TimeOut(JIG_ID jig);

	//kjpark 20170911 신호기 command UnitBank 추가
	void IDCheck_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL IDCheck_Check(JIG_ID jig, JIG_CH channel);
	void IDCheck_TimeOut(JIG_ID jig);

	//kjpark 20170911 신호기 command UnitBank 추가
	void OTPREG_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL OTPREG_Check(JIG_ID jig, JIG_CH channel);
	void OTPREG_TimeOut(JIG_ID jig);

	void ICTTest_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL ICTTest_Check(JIG_ID jig, JIG_CH channel);
	void ICTTest_TimeOut(JIG_ID jig);

	void COPR_ICTTest_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL COPR_ICTTest_Check(JIG_ID jig, JIG_CH channel);
	void COPR_ICTTest_TimeOut(JIG_ID jig);

	void POCErrorCheck_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL POCErrorCheck_Check(JIG_ID jig, JIG_CH channel);
	void POCErrorCheck_TimeOut(JIG_ID jig);

	void DDIBlockTest_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL DDIBlockTest_Check(JIG_ID jig, JIG_CH channel);
	void DDIBlockTest_TimeOut(JIG_ID jig);

	void OptionCheck2_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL OptionCheck2_Check(JIG_ID jig, JIG_CH channel);
	void OptionCheck2_TimeOut(JIG_ID jig);

	void OptionCheck3_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL OptionCheck3_Check(JIG_ID jig, JIG_CH channel);
	void OptionCheck3_TimeOut(JIG_ID jig);

	void OptionCheck4_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL OptionCheck4_Check(JIG_ID jig, JIG_CH channel);
	void OptionCheck4_TimeOut(JIG_ID jig);

	void OptionCheck5_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL OptionCheck5_Check(JIG_ID jig, JIG_CH channel);
	void OptionCheck5_TimeOut(JIG_ID jig);

	void OptionCheck6_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL OptionCheck6_Check(JIG_ID jig, JIG_CH channel);
	void OptionCheck6_TimeOut(JIG_ID jig);

	void OptionCheck7_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL OptionCheck7_Check(JIG_ID jig, JIG_CH channel);
	void OptionCheck7_TimeOut(JIG_ID jig);

	void OptionCheck8_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL OptionCheck8_Check(JIG_ID jig, JIG_CH channel);
	void OptionCheck8_TimeOut(JIG_ID jig);

	void OptionCheck9_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL OptionCheck9_Check(JIG_ID jig, JIG_CH channel);
	void OptionCheck9_TimeOut(JIG_ID jig);

	void OptionCheck10_Send(JIG_ID jig, JIG_CH channel = JIG_CH_MAX);
	BOOL OptionCheck10_Check(JIG_ID jig, JIG_CH channel);
	void OptionCheck10_TimeOut(JIG_ID jig);

	BOOL CalcurateDefectSquare(JIG_ID jig);
	BOOL SendDefectSquare(int nIndex, JIG_ID jig);

	CString GetCellID(JIG_CH nCh, JIG_ID jig);

	BOOL CheckTMDnProductIDMatch(JIG_ID jig, JIG_CH ch);
	BOOL CheckTMDnCellIDMatch(JIG_ID jig, JIG_CH ch);


	// 해당 지그가 로봇한테서 셀을 받을 상태인지 확인
	BOOL CellInfo_CheckLoadable(JIG_ID jig);

	// 해당 지그가 로봇한테서 셀을 줄 수 상태인지 확인
	BOOL CellInfo_CheckUnloadable(JIG_ID jig, JIG_CH ch = JIG_CH_MAX);

	//////////////////////////////////////////////////////////////////
	// IO Part
	//////////////////////////////////////////////////////////////////
	//kjpark 20161027 IO Output check bug 수정
	BOOL GetInPutIOCheck(INPUT_ID ID);	
	BOOL GetInPutIOCheck(INPUT_ID ID, ONOFF bValue);	
	BOOL GetOutPutIOCheck(OUTPUT_ID ID);
	BOOL GetOutPutIOCheck(OUTPUT_ID ID, ONOFF bValue);
	void SetOutPutIO(OUTPUT_ID ID, BOOL bValue);
	void SetOutPutIO(OUTPUT_ID ID, ONOFF bValue);

	BOOL ReadySwitch_Check(JIG_ID jig);

	BOOL AutoTeachKey_AutoCheck(BOOL bAlarm = FALSE);		
	BOOL AutoTeachKey_TeachCheck();

	BOOL DoorClose_Check(BOOL bAlarm = FALSE);
	BOOL DoorLockOn_Check();
	BOOL LightCurtain_Check(JIG_ID jig = JIG_ID_MAX, BOOL bAlarm = FALSE);
	void LightCurtainMute_OnOff(JIG_ID jig, ONOFF value);
	void LightCurtainMuteLamp_OnOff(JIG_ID jig, ONOFF value);
	BOOL LightCurtainMuteLamp_Check(JIG_ID jig);

	STO_STATE STO_Check();

	void TowerLamp_Change(int nRed, int nYellow, int nGreen, BOOL bBuzzer);

	void Temp_Check();
	void EMSSwitch_Check();
	void FanAlarm_Check();
	void MCPower_Check();

	
	void SetProductData(JIG_ID jig, JIG_CH ch);
	void ReadFromFilePGMS();

	BOOL LoadingStop_IsAble();		// Loading Stop 가능한지 확인
	BOOL LoadingStop_IsAble(JIG_ID jig);		// Loading Stop 가능한지 확인
	BOOL TransferStop_IsAble();		// Transfer Stop 가능한지 확인

	/*------------------------------------------------------------------------*/
	/////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
	//kjpark 20161025 MCR 구현
	//kjpark 20170902 Teach Shuttle UI 및 버튼 동작 추가
	void MCR_Trigger(JIG_ID jig, JIG_CH ch, BOOL bOn);
	//kjpark 20170912 MCR에서 읽은 셀아이디 정상 판별
	BOOL GetMCR_ResultRecive(JIG_ID jig, JIG_CH ch);
	CString GetSoftTriggerData(JIG_ID jig, JIG_CH ch);
	int GetSoftTriggerResult(JIG_ID jig, JIG_CH ch);
	BOOL SetLiveMode(JIG_ID jig, JIG_CH ch, BOOL bFlag);
	BOOL GetLiveMode(JIG_ID jig, JIG_CH ch);
	BOOL SetSoftTrigger(JIG_ID jig, JIG_CH ch);
	BOOL SaveResultImage(JIG_ID jig, JIG_CH ch, CString sFile);
	//kjpark 20170831 MCR 구현 채널 별 구현 완료
	CString GetLastSavedImage(JIG_ID jig, JIG_CH ch);	
	//kjpark 20170912 MCR Retry 추가
	void SaveLastMCRImage(JIG_ID jig, JIG_CH ch);
	//kjpark 20170912 MCR에서 읽은 셀아이디 정상 판별
	void SetMCRReadingID(JIG_ID shuttls, JIG_CH channel);
	//kjpark 20170912 MCR Retry 추가
	CString	MCR_GetLastID(JIG_ID shuttls, JIG_CH channel);
	BOOL MCR_SuccessCheck(JIG_ID jig, JIG_CH channel);
	//kjpark 20170831 MCR 구현 채널 별 구현 완료
	void SetMCR_Reset(JIG_ID jig, JIG_CH ch, BOOL bOn);

	//////////////////////////////////////////////////////////////////////////
	// Shuttle & Axis Function [9/11/2017 OSC]

	//kjpark 20180131 Robot Status interlock 회피 더 추가
	BOOL Shuttle_Y_LOAD_Move(JIG_ID jig);
	BOOL Shuttle_Y_LOAD_Check(JIG_ID jig);
	void Shuttle_Y_MANUAL_Move(JIG_ID jig);
	BOOL Shuttle_Y_MANUAL_Check(JIG_ID jig);
	void Shuttle_Y_INSP_Move(JIG_ID jig);
	BOOL Shuttle_Y_INSP_Check(JIG_ID jig);

	void Inspection_X_INSP_Move(JIG_ID jig);
	BOOL Inspection_X_INSP_Check(JIG_ID jig);

	void Inspection_Z_UP_Move(JIG_ID jig);
	BOOL Inspection_Z_UP_Check(JIG_ID jig);
	void Inspection_Z_INSP_Move(JIG_ID jig);
	BOOL Inspection_Z_INSP_Check(JIG_ID jig);

	void Active_ALIGN_X_LEFT_Move(JIG_ID jig);
	BOOL Active_ALIGN_X_LEFT_Check(JIG_ID jig);
	void Active_ALIGN_X_RIGHT_Move(JIG_ID jig);
	BOOL Active_ALIGN_X_RIGHT_Check(JIG_ID jig);
	//yjkim 180605 추가 V
	//vac
	void Shuttle_Vac_OnOff(JIG_ID jig, JIG_CH ch, VAC_STATE vac);
	BOOL Shuttle_Vac_Check(JIG_ID jig, JIG_CH ch, VAC_STATE vac);
	BOOL Shuttle_VacOut_Check(JIG_ID jig, JIG_CH ch, VAC_STATE vac);
	//blow
	void Shuttle_Blow_OnOff(JIG_ID jig, JIG_CH ch, BLOW_STATE blo);
	BOOL Shuttle_Blow_Check(JIG_ID jig, JIG_CH ch, BLOW_STATE blo);
	BOOL Shuttle_BlowOut_Check(JIG_ID jig, JIG_CH ch, BLOW_STATE blo);
	//fpcb vac
	void Shuttle_Fpcb_Vac_OnOff(JIG_ID jig, JIG_CH ch, VAC_STATE vac);
	BOOL Shuttle_Fpcb_Vac_Check(JIG_ID jig, JIG_CH ch, VAC_STATE vac);
	BOOL Shuttle_Fpcb_VacOut_Check(JIG_ID jig, JIG_CH ch, VAC_STATE vac);
	//fpcb blow
	void Shuttle_Fpcb_Blow_OnOff(JIG_ID jig, JIG_CH ch, BLOW_STATE blo);
	BOOL Shuttle_Fpcb_Blow_Check(JIG_ID jig, JIG_CH ch, BLOW_STATE blo);
	BOOL Shuttle_Fpcb_BlowOut_Check(JIG_ID jig, JIG_CH ch, BLOW_STATE blo);

	//TILT
	void Shuttle_Tilt_Up(JIG_ID jig, JIG_CH ch, TILT_STATE tilt);
	BOOL Shuttle_Tilt_Up_Check(JIG_ID jig, JIG_CH ch, TILT_STATE tilt);
	BOOL Shuttle_Tilt_UpOut_Check(JIG_ID jig, JIG_CH ch, TILT_STATE tilt);

	void Shuttle_Tilt_Down(JIG_ID jig, JIG_CH ch, TILT_STATE tilt);
	BOOL Shuttle_Tilt_Down_Check(JIG_ID jig, JIG_CH ch, TILT_STATE tilt);
	BOOL Shuttle_Tilt_DownOut_Check(JIG_ID jig, JIG_CH ch, TILT_STATE tilt);

	//IONIZER

	//AILGN

	//////////////////////////////////////////////////////////////////////////
	// PDT 물류설비 Interface

	// Skip여부에 따라 자동으로 Servival을 살린다 [10/5/2017 OSC]
	void MTP_IF_Servival_AllSet();
	void MTP_IF_Servival_AllReset();
	void MTP_IF_Servival_OnOff(JIG_ID jig, JIG_CH ch, BOOL bOn);
	void MTP_IF_Able_OnOff(JIG_ID jig, BOOL bOn);
	BOOL MTP_IF_Able_Check(JIG_ID jig);
	void MTP_IF_Start_OnOff(JIG_ID jig, BOOL bOn);
	void MTP_IF_Complete_OnOff(JIG_ID jig, BOOL bOn);
	void MTP_IF_CheckSensor_OnOff(JIG_ID jig, JIG_CH ch, BOOL bOn);
	void MTP_IF_VacuumStatus_OnOff(JIG_ID jig, JIG_CH ch, BOOL bOn);
	void MTP_IF_Reset(JIG_ID jig);
	void MTP_IF_CellInfo_Write(JIG_ID jig, JIG_CH ch);
	void MTP_IF_LoadingStop_OnOff(JIG_ID jig);

	BOOL PDT_IF_Servival_Check(JIG_ID jig);
	BOOL PDT_IF_Able_Check(JIG_ID jig);
	BOOL PDT_IF_Start_Check(JIG_ID jig);
	BOOL PDT_IF_Complete_Check(JIG_ID jig);
	BOOL PDT_IF_ArmStatus_Check(JIG_ID jig);
	//kjparkk 20180131 PDT AB RULE 체크 기능 추가
	BOOL PDT_IF_RETRY_AB_Check(JIG_ID jig = JIG_ID_A);	
	BOOL PDT_IF_VacOnReq_Check(JIG_ID jig, JIG_CH ch);
	BOOL PDT_IF_VacOffReq_Check(JIG_ID jig, JIG_CH ch);
	CString PDT_IF_CellID_Read(JIG_ID jig, JIG_CH ch);

	//////////////////////////////////////////////////////////////////////////
};

// State용이 아니라 그냥 UI 등에서 유닛 제어용으로 만든 Class
class CUnitCtrlFunc : 	public CUnitCtrlBank
{
public:
	CUnitCtrlFunc(MP2100_THREAD nThreadID) : CUnitCtrlBank(STATE_FUNC, nThreadID)
	{
	};
	~CUnitCtrlFunc(void) {};

	int Run() {return 0;};
};


