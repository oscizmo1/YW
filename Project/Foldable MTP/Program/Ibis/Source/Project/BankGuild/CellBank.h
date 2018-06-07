#pragma once
#include "BankGuild\MCDefine.h"
#include "BankGuild\CellLogItem.h"
#include "BankGuild\CellDefectInfo.h"

// CCellInfo m_Cell[MAX_CELL_INFO_BUFFER]의 Index 저장소
// 설비에서 Cell이 이송될 때 각 Unit간 Cell Index만 전달하여 관리한다
class CCellTag
{
public:
	int m_nIndex;
	CCellTag(void) { Init(); }
	void Init() { m_nIndex = -1; }
	BOOL IsExist() { return (m_nIndex >= 0) ?TRUE:FALSE; }
};

class DefaultCellInfomation
{
public:
	CString m_strInnerID;//INNER ID
	CString m_strCellID;//CELL ID 
	CString m_strReadUnitMCR;//READ UNIT
	JIG_ID m_JigId;
	JIG_CH m_JigCh;

	CString m_strUNIATS_VERSION;
	CString m_strMICRO_DLL_VERSION;
	CString m_strMACRO_DLL_VERSION;
	CString	m_strSETBIN_VERSION;	

	//kjpark 20170907 CellLog Title 및 해당 항목 추가
	CString m_strPG_Host_Version;//TX HOST VERSION
	CString m_strPG_Client_Version;//PG_UI:신호기 Client 버전. 반드시 기록
	CString m_strPG_MAC_Address;//CH Card MAC
	TMD_INFO_STATUS m_bReceive_TMD_Info;
	CString m_strTMD_Info;
	//kjpark 20180113 Tmd Version 추가
	CString m_strTXFPGAVersion;
	CString m_strTXCPLDVersion;
	CString m_strTXOSVersion;

	CString m_strProductID;
	CString m_strCellInfoResult;
	CString m_strStepID;

	//kjpark 20171010 TMD March 구현
	CLASS_CELL	CellMixing;
	CString		m_strCellMixing;

	//kjpark 20170912 Cellinfomation 결과
	CLASS_CELL m_CellInfoResult;
	BOOL m_bPGAlarm;
	CString m_strPGAlarmName;
	ZONE_ID m_PGAlarmZone;

	CLASS_CELL m_LastClass;
	CString m_strLastResult;
	CString MesCode;
	CString m_strCIMJudge;	// G,L,R 등 CIM용 판정

	CString	m_str_Engineer_SSO_ID;//엔지니어 사번 기록
	CString	m_str_Operator_SSO_ID;//검사원 사번 기록
	int FinalJudge;		

	BOOL m_bDefectPrinted;	// 라벨 출력을 했으면 TRUE
	BOOL m_bIsInspectionSkip;

	//kjpark 20170706 default = 0 / Jobstart = 2 / Invalid = 1
	//kjpark 20170710 Job Start 수신 여부 및 Invalid/Job Start 판정
	//kjpark 20170907 Tracking CellInfomation, JobProcess 추가
	JOB_PROCESS_STATE m_nInspectInvalidType;
	//kjpark 20180123 CELL INFO, JOB PROCESS Cellid 추가
	CString m_strJobProcessState;
	//kjpark 20170912 MCR에서 읽은 셀아이디 정상 판별
	BOOL m_bMCR_OK;
	//kjpark 20170912 TrackIn 중복보고 방지
	BOOL m_bTrackinFinish;	
	// TrackOut 중복보고 방지 [10/8/2017 OSC]
	BOOL m_bTrackOutFinish;	

	// 리트라이 가능한 셀인지 여부 [9/29/2017 OSC]
	BOOL m_bRetryAble;

	// AB Retry시에는 첫 불량에는 R, 두번째는  L 보고 [9/26/2017 OSC]
	BOOL m_bRetryAB;
	// 셀로그용 AB Rule. 리트라이 위해 R보고 할 때는 'A'
	// 리트라이 해서 L이나 G보고할 때는 'AB', 리트라이 할 필요 없어도 'AB'
	// GOOD이면 공백
	CString m_strABRule;
	// 리트라이 일 경우 이전 최종판정값
	CString m_strAResult;

	void	Init()
	{
		m_strInnerID.Empty();
		m_strCellID.Empty();
		m_strReadUnitMCR.Empty();
		m_JigId = JIG_ID_MAX;
		m_JigCh = JIG_CH_MAX;
		
		m_strUNIATS_VERSION.Empty();
		m_strMICRO_DLL_VERSION.Empty();
		m_strMACRO_DLL_VERSION.Empty();
		m_strSETBIN_VERSION.Empty();	
		//kjpark 20170907 CellLog Title 및 해당 항목 추가
		m_strPG_Host_Version.Empty();
		m_strPG_Client_Version.Empty();
		m_strPG_MAC_Address.Empty();
		m_bReceive_TMD_Info = TMD_INFO_NOT_RECEIVE;
		m_strTMD_Info.Empty();
	//kjpark 20180113 Tmd Version 추가
		m_strTXFPGAVersion.Empty();
		m_strTXCPLDVersion.Empty();
		m_strTXOSVersion.Empty();
		m_strProductID.Empty();
		m_strStepID.Empty();

		//kjpark 20171010 TMD March 구현
		CellMixing= GOOD_CELL;
		m_strCellMixing=SKIP;

		m_strCellInfoResult.Empty();	
		m_CellInfoResult = NONE_CELL;
		m_bPGAlarm = FALSE;
		m_strPGAlarmName.Empty();
		m_PGAlarmZone = ZONE_ID_MAX;
				
		m_LastClass = NONE_CELL;
		m_strLastResult.Empty();
		MesCode.Empty();
		m_strCIMJudge.Empty();
		m_str_Engineer_SSO_ID.Empty();
		m_str_Operator_SSO_ID.Empty();
		FinalJudge = 0;

		m_bDefectPrinted = FALSE;
		m_bIsInspectionSkip = FALSE;
				
		//kjpark 20170907 Tracking CellInfomation, JobProcess 추가
		m_nInspectInvalidType = JOB_DEFAULT;
		//kjpark 20180123 CELL INFO, JOB PROCESS Cellid 추가
		m_strJobProcessState.Empty();
		m_bTrackOutFinish = FALSE;
		//kjpark 20170925 TRACKING CellID NG 시 SKIP
		m_bMCR_OK = FALSE;
		m_bTrackinFinish = FALSE;
		m_bRetryAble = FALSE;
		m_bRetryAB = FALSE;
		m_strABRule = AB_RULE_AB;
		m_strAResult.Empty();
	}
};

class PG_LOG_INFO
{
public:
	PG_LOG_INFO()
	{
		Init();
	}
	~PG_LOG_INFO() {}

	CString m_CORRELATION;				// 7열(G열) : "BG7000_CAL_DATE"의 29번째 열 값
	CString m_BEF_255_MTP;				// 7열(G열) : "BEF_255"의 2번째 열 값

	CString m_S_MID_255_PRE_L_MTP;		// 7열(G열) : "S_MID_255_PRE_"의 2번째 열 값
	CString m_S_MID_255_PRE_X_MTP;		// 7열(G열) : "S_MID_255_PRE_"의 3번째 열 값
	CString m_S_MID_255_PRE_Y_MTP;		// 7열(G열) : "S_MID_255_PRE_"의 4번째 열 값
	CString m_S_MID_255_PRE_TCOLOR_MTP;	// 7열(G열) : "S_MID_255_PRE_"의 7번째 열 값
	CString m_S_MID_255_PRE_MPCD_MTP;	// 7열(G열) : "S_MID_255_PRE_"의 8번째 열 값

	CString m_300CD_V255_L_VERIFY;		// 7열(G열) : "300CD_V255_PRE"의 2번째 열 값
	CString m_300CD_V255_X_VERIFY;		// 7열(G열) : "300CD_V255_PRE"의 3번째 열 값
	CString m_300CD_V255_Y_VERIFY;		// 7열(G열) : "300CD_V255_PRE"의 4번째 열 값
	CString m_300CD_V255_TCOLOR_VERIFY;	// 7열(G열) : "300CD_V255_PRE"의 7번째 열 값
	CString m_300CD_V255_MPCD_VERIFY;	// 7열(G열) : "300CD_V255_PRE"의 8번째 열 값

	//kjpark 20180129 MID ALPM 255 IBAT 항목추가
	CString m_MID_ALPM_255_IBAT;		// 7열(G열) : "MID_ALPM_255"의 10번째 열 값

	void Init()
	{
		m_CORRELATION.Empty();
		m_BEF_255_MTP.Empty();

		m_S_MID_255_PRE_L_MTP.Empty();
		m_S_MID_255_PRE_X_MTP.Empty();
		m_S_MID_255_PRE_Y_MTP.Empty();
		m_S_MID_255_PRE_TCOLOR_MTP.Empty();
		m_S_MID_255_PRE_MPCD_MTP.Empty();

		m_300CD_V255_L_VERIFY.Empty();
		m_300CD_V255_X_VERIFY.Empty();
		m_300CD_V255_Y_VERIFY.Empty();
		m_300CD_V255_TCOLOR_VERIFY.Empty();
		m_300CD_V255_MPCD_VERIFY.Empty();

		//kjpark 20180129 MID ALPM 255 IBAT 항목추가
		m_MID_ALPM_255_IBAT.Empty();
	}
};
// Cell Data 구조체 Class
class CCellInfo
{
public:

	CCellInfo(void) 
	{ 
		Init(); 
		LinkCellLogItem();
	}
	~CCellInfo(void){}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Cell LOG Part
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	DefaultCellInfomation defaultData;


	// 이전검사후 A존 도착 시점 ~ 현재검사후 A존 도착 시점
	CCellTactTime m_UnloadTactTime;

	// 물류 : 인터페이스 끝 ~ A존 도착 시점
	// 단동 : PG On 버튼 ~ 검사 후 PG Off 버튼
	CCellTactTime m_TactTime;

	// 물류 : A존 도착 시점 ~ 인터페이스 시작
	// 단동 : PG Off ~ PG On
	CCellTactTime m_WaitTime;

	// 물류 : 인터페이스 끝 ~ CELL_LOADING 시작
	CCellTactTime m_LoadingTactTime;

	CCellTactTime m_TableTurnTime;

	// 셔틀 MCR 위치 도착 완료 ~ Inspection 위치 이동 시작
	CCellTactTime m_MCRReadTime;

	// A존의 Cell Loading 외 다른 검사시간. 어짜피 점등 이후니 AFT 검사만 신경쓰자
	CCellTactTime m_AZoneETCTime;

	// 셔틀 C존 도착부터 C존 검사 시작 전까지
	CCellTactTime m_MTPReadyTime;

	CCellDefectInfo_Zone		m_AZone;
	//kjpark 20170912 MCR 위치에따른 택타임 추가
	CCellDefectInfo_Zone		m_AZonetoMCRZone;
	CCellDefectInfo_Zone		m_MCRZonetoBZone;
	CCellDefectInfo_Zone		m_BZone;
	// C존의 MTP 검사시간 이후부터 B존 AFT 검사 완료까지
	CCellTactTime				m_BZoneETCTime;
	//kjpark 20170912 MCR 위치에따른 택타임 추가
	CCellDefectInfo_Zone		m_BZonetoAZone;

	CCellDefectInfo_CellLoading m_CellLoading;
	CCellDefectInfo				m_MTPWrite;
	CCellDefectInfo				m_MTPVerify;
	CCellDefectInfo_Current		m_WhiteCurrent;
	//kjpark 20170907 CellLog Title 및 해당 항목 추가
	CCellDefectInfo_Current		m_SleepCurrent;
	CCellDefectInfo_Current		m_HLPMCurrent;
	//kjpark 20170907 CellLog Title 및 해당 항목 추가
	//kjpark 20170908 TSP START 추가
	CCellDefectInfo_Tsp			m_TSPStart;
	CCellDefectInfo				m_EVTVersionCheck;
	CCellDefectInfo				m_TECheck;
	CCellDefectInfo				m_IDCheck;
	CCellDefectInfo				m_OTPREGCheck;
	CCellDefectInfo				m_ICTCheck;
	CCellDefectInfo				m_COPRICTTest;
	CCellDefectInfo				m_PocErrorCheck;
	CCellDefectInfo				m_DDIBlockTest;
	CCellDefectInfo				m_OptionCheck2;
	CCellDefectInfo				m_OptionCheck3;
	CCellDefectInfo				m_OptionCheck4;
	CCellDefectInfo				m_OptionCheck5;
	CCellDefectInfo				m_OptionCheck6;
	CCellDefectInfo				m_OptionCheck7;
	CCellDefectInfo				m_OptionCheck8;
	CCellDefectInfo				m_OptionCheck9;
	CCellDefectInfo				m_OptionCheck10;

	PG_LOG_INFO					m_PG_LOG_INFO;
	
	// CIM에서 내려받는 불량좌표 [5/2/2017 OSC]
	int			m_nDefectPointCount;
	CIM_DEFECT_POINT m_DefectPoint[MAX_DEFECT_POINT];


	void Init();
	CCellDefectInfo*			GetDefectInfo(CString strINspModuleName);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Cell Log [11/16/2016 OSC]
	CList <CCellLogItem, CCellLogItem> m_listCellLogItem;	// 이건 Init에 넣어줄 필요가 없다 [11/16/2016 OSC]
	void LinkCellLogItem();
	CString GetCellLogTitle();
	CString GetCellLogValue();

	// Retry
	BOOL Retry_CheckAble();

	BOOL CellInfo_CheckAble();
	BOOL TrackIn_CheckAble();
	BOOL InspectionEnd_CheckAble();
	BOOL TrackOut_CheckAble();
	BOOL TrackIn_CancelAble();
	void CimJudge_Judge(CString strForcedJudge = _T(""));
};




//20170306 kjpark WhiteCurrent ADD in Celllog
#define MAX_CELL_LOG_STRING	8192*2
#define MAX_CELL_INFO_BUFFER		1000
class CCellBank
{
public:
	CCellBank(void);
	~CCellBank(void);

	// Cell 생성시 발번될 Index 번호
	int m_nNextGenIndex;
	CCellInfo m_Cell[MAX_CELL_INFO_BUFFER];

	// 설비內 Cell이 존재 가능한 모든 위치만큼 버퍼로 만들어 놓고
	// 버퍼끼리 서로 tag를 주고받는다
	CCellTag m_CellTag[CELL_POS_MAX];

	CCellTag GetCellTag(CELL_POS pos);
	CCellTag GetCellTag(JIG_ID jig, JIG_CH nCh);			

	// 특정 구간에서 해당하는 CellTag를 찾는 함수
	CCellTag SearchCellTag(JIG_ID nJig, JIG_CH nCh, CELL_POS startPos = CELL_POS_SHUTTLE1_CH1, CELL_POS endPos = CELL_POS_SHUTTLE2_CH1);
	CCellTag SearchCellTag(CString strInnerID, CELL_POS startPos = CELL_POS_SHUTTLE1_CH1, CELL_POS endPos = CELL_POS_SHUTTLE2_CH1);


	void SetCellTag(CELL_POS pos, CCellTag tag);

	CCellTag RemoveCellTag(CELL_POS pos);
	CCellTag RemoveCellTag( JIG_ID jig, JIG_CH nCh);

	CCellTag CreateCellTag();
	void CreateCellInfo(CELL_POS pos);

	// CellTag를 기반으로 CellInfo를 반환하는 함수
	CCellInfo* GetCellInfo(CCellTag tag);
	CCellInfo* GetCellInfo(CELL_POS pos );
	CCellInfo* GetCellInfo(JIG_ID jig, JIG_CH nCh);
	CCellInfo* SearchCellInfo(JIG_ID nJig, JIG_CH nCh, CELL_POS startPos = CELL_POS_SHUTTLE1_CH1, CELL_POS endPos = CELL_POS_SHUTTLE2_CH1);
	CCellInfo* SearchCellInfo(int nJig, int nCh, CELL_POS startPos = CELL_POS_SHUTTLE1_CH1, CELL_POS endPos = CELL_POS_SHUTTLE2_CH1);
	CCellInfo* SearchCellInfo(CString strCellID, CELL_POS startPos = CELL_POS_SHUTTLE1_CH1, CELL_POS endPos = CELL_POS_SHUTTLE2_CH1);

	//kjpark 20161018 셀 Tag shift
	void SetCellTagNextPosition(CELL_POS CurrentPos, CELL_POS NextPos);

	CFile m_fileCellData;
	BOOL WriteCellLog(CELL_POS pos);
	BOOL WriteCellLog(JIG_ID jig, JIG_CH ch);
	//kjpark 20180114 NG 판정 통계 UI 추가
	BOOL WriteNGResultLog(CCellInfo* pCell, CTime time);

	CFile m_fileCellDataTemp;
	BOOL WriteCellLogTemp(CELL_POS pos);
	BOOL WriteCellLogTemp(JIG_ID jig, JIG_CH ch);


	//////////////////////////////////////////////////////////////////////////
	// CELL_LOADING 전용 [12/19/2017 OSC]
	// CellTag가 생성되기 전에 진행이 되므로 Cell Loading은 
	// 아예 전용변수로 사용하고 나중에 옮겨담는다
	CCellInfo m_InfoCellLoading[JIG_ID_MAX][JIG_CH_MAX];
	CCellInfo* GetCellInfo_CellLoading(JIG_ID jig, JIG_CH nCh);

	//////////////////////////////////////////////////////////////////////////
	// 프로그램 재시작시 CellInfo복원.. 다 저장할 필요 없이
	// 종료 전에 CIM에 O 보고하여 새로 돌릴 수 있게 끔 해 놓고
	// TrackIn 관련 정보만 저장해 놓는다. [12/23/2017 OSC]
	void BackupCellToFile();
	void RestoreCellToFile();

};

