#pragma once
#include "BankGuild\MCDefine.h"
#include "BankGuild\CellLogItem.h"
#include "BankGuild\CellDefectInfo.h"

// CCellInfo m_Cell[MAX_CELL_INFO_BUFFER]�� Index �����
// ���񿡼� Cell�� �̼۵� �� �� Unit�� Cell Index�� �����Ͽ� �����Ѵ�
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

	//kjpark 20170907 CellLog Title �� �ش� �׸� �߰�
	CString m_strPG_Host_Version;//TX HOST VERSION
	CString m_strPG_Client_Version;//PG_UI:��ȣ�� Client ����. �ݵ�� ���
	CString m_strPG_MAC_Address;//CH Card MAC
	TMD_INFO_STATUS m_bReceive_TMD_Info;
	CString m_strTMD_Info;
	//kjpark 20180113 Tmd Version �߰�
	CString m_strTXFPGAVersion;
	CString m_strTXCPLDVersion;
	CString m_strTXOSVersion;

	CString m_strProductID;
	CString m_strCellInfoResult;
	CString m_strStepID;

	//kjpark 20171010 TMD March ����
	CLASS_CELL	CellMixing;
	CString		m_strCellMixing;

	//kjpark 20170912 Cellinfomation ���
	CLASS_CELL m_CellInfoResult;
	BOOL m_bPGAlarm;
	CString m_strPGAlarmName;
	ZONE_ID m_PGAlarmZone;

	CLASS_CELL m_LastClass;
	CString m_strLastResult;
	CString MesCode;
	CString m_strCIMJudge;	// G,L,R �� CIM�� ����

	CString	m_str_Engineer_SSO_ID;//�����Ͼ� ��� ���
	CString	m_str_Operator_SSO_ID;//�˻�� ��� ���
	int FinalJudge;		

	BOOL m_bDefectPrinted;	// �� ����� ������ TRUE
	BOOL m_bIsInspectionSkip;

	//kjpark 20170706 default = 0 / Jobstart = 2 / Invalid = 1
	//kjpark 20170710 Job Start ���� ���� �� Invalid/Job Start ����
	//kjpark 20170907 Tracking CellInfomation, JobProcess �߰�
	JOB_PROCESS_STATE m_nInspectInvalidType;
	//kjpark 20180123 CELL INFO, JOB PROCESS Cellid �߰�
	CString m_strJobProcessState;
	//kjpark 20170912 MCR���� ���� �����̵� ���� �Ǻ�
	BOOL m_bMCR_OK;
	//kjpark 20170912 TrackIn �ߺ����� ����
	BOOL m_bTrackinFinish;	
	// TrackOut �ߺ����� ���� [10/8/2017 OSC]
	BOOL m_bTrackOutFinish;	

	// ��Ʈ���� ������ ������ ���� [9/29/2017 OSC]
	BOOL m_bRetryAble;

	// AB Retry�ÿ��� ù �ҷ����� R, �ι�°��  L ���� [9/26/2017 OSC]
	BOOL m_bRetryAB;
	// ���α׿� AB Rule. ��Ʈ���� ���� R���� �� ���� 'A'
	// ��Ʈ���� �ؼ� L�̳� G������ ���� 'AB', ��Ʈ���� �� �ʿ� ��� 'AB'
	// GOOD�̸� ����
	CString m_strABRule;
	// ��Ʈ���� �� ��� ���� ����������
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
		//kjpark 20170907 CellLog Title �� �ش� �׸� �߰�
		m_strPG_Host_Version.Empty();
		m_strPG_Client_Version.Empty();
		m_strPG_MAC_Address.Empty();
		m_bReceive_TMD_Info = TMD_INFO_NOT_RECEIVE;
		m_strTMD_Info.Empty();
	//kjpark 20180113 Tmd Version �߰�
		m_strTXFPGAVersion.Empty();
		m_strTXCPLDVersion.Empty();
		m_strTXOSVersion.Empty();
		m_strProductID.Empty();
		m_strStepID.Empty();

		//kjpark 20171010 TMD March ����
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
				
		//kjpark 20170907 Tracking CellInfomation, JobProcess �߰�
		m_nInspectInvalidType = JOB_DEFAULT;
		//kjpark 20180123 CELL INFO, JOB PROCESS Cellid �߰�
		m_strJobProcessState.Empty();
		m_bTrackOutFinish = FALSE;
		//kjpark 20170925 TRACKING CellID NG �� SKIP
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

	CString m_CORRELATION;				// 7��(G��) : "BG7000_CAL_DATE"�� 29��° �� ��
	CString m_BEF_255_MTP;				// 7��(G��) : "BEF_255"�� 2��° �� ��

	CString m_S_MID_255_PRE_L_MTP;		// 7��(G��) : "S_MID_255_PRE_"�� 2��° �� ��
	CString m_S_MID_255_PRE_X_MTP;		// 7��(G��) : "S_MID_255_PRE_"�� 3��° �� ��
	CString m_S_MID_255_PRE_Y_MTP;		// 7��(G��) : "S_MID_255_PRE_"�� 4��° �� ��
	CString m_S_MID_255_PRE_TCOLOR_MTP;	// 7��(G��) : "S_MID_255_PRE_"�� 7��° �� ��
	CString m_S_MID_255_PRE_MPCD_MTP;	// 7��(G��) : "S_MID_255_PRE_"�� 8��° �� ��

	CString m_300CD_V255_L_VERIFY;		// 7��(G��) : "300CD_V255_PRE"�� 2��° �� ��
	CString m_300CD_V255_X_VERIFY;		// 7��(G��) : "300CD_V255_PRE"�� 3��° �� ��
	CString m_300CD_V255_Y_VERIFY;		// 7��(G��) : "300CD_V255_PRE"�� 4��° �� ��
	CString m_300CD_V255_TCOLOR_VERIFY;	// 7��(G��) : "300CD_V255_PRE"�� 7��° �� ��
	CString m_300CD_V255_MPCD_VERIFY;	// 7��(G��) : "300CD_V255_PRE"�� 8��° �� ��

	//kjpark 20180129 MID ALPM 255 IBAT �׸��߰�
	CString m_MID_ALPM_255_IBAT;		// 7��(G��) : "MID_ALPM_255"�� 10��° �� ��

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

		//kjpark 20180129 MID ALPM 255 IBAT �׸��߰�
		m_MID_ALPM_255_IBAT.Empty();
	}
};
// Cell Data ����ü Class
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


	// �����˻��� A�� ���� ���� ~ ����˻��� A�� ���� ����
	CCellTactTime m_UnloadTactTime;

	// ���� : �������̽� �� ~ A�� ���� ����
	// �ܵ� : PG On ��ư ~ �˻� �� PG Off ��ư
	CCellTactTime m_TactTime;

	// ���� : A�� ���� ���� ~ �������̽� ����
	// �ܵ� : PG Off ~ PG On
	CCellTactTime m_WaitTime;

	// ���� : �������̽� �� ~ CELL_LOADING ����
	CCellTactTime m_LoadingTactTime;

	CCellTactTime m_TableTurnTime;

	// ��Ʋ MCR ��ġ ���� �Ϸ� ~ Inspection ��ġ �̵� ����
	CCellTactTime m_MCRReadTime;

	// A���� Cell Loading �� �ٸ� �˻�ð�. ��¥�� ���� ���Ĵ� AFT �˻縸 �Ű澲��
	CCellTactTime m_AZoneETCTime;

	// ��Ʋ C�� �������� C�� �˻� ���� ������
	CCellTactTime m_MTPReadyTime;

	CCellDefectInfo_Zone		m_AZone;
	//kjpark 20170912 MCR ��ġ������ ��Ÿ�� �߰�
	CCellDefectInfo_Zone		m_AZonetoMCRZone;
	CCellDefectInfo_Zone		m_MCRZonetoBZone;
	CCellDefectInfo_Zone		m_BZone;
	// C���� MTP �˻�ð� ���ĺ��� B�� AFT �˻� �Ϸ����
	CCellTactTime				m_BZoneETCTime;
	//kjpark 20170912 MCR ��ġ������ ��Ÿ�� �߰�
	CCellDefectInfo_Zone		m_BZonetoAZone;

	CCellDefectInfo_CellLoading m_CellLoading;
	CCellDefectInfo				m_MTPWrite;
	CCellDefectInfo				m_MTPVerify;
	CCellDefectInfo_Current		m_WhiteCurrent;
	//kjpark 20170907 CellLog Title �� �ش� �׸� �߰�
	CCellDefectInfo_Current		m_SleepCurrent;
	CCellDefectInfo_Current		m_HLPMCurrent;
	//kjpark 20170907 CellLog Title �� �ش� �׸� �߰�
	//kjpark 20170908 TSP START �߰�
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
	
	// CIM���� �����޴� �ҷ���ǥ [5/2/2017 OSC]
	int			m_nDefectPointCount;
	CIM_DEFECT_POINT m_DefectPoint[MAX_DEFECT_POINT];


	void Init();
	CCellDefectInfo*			GetDefectInfo(CString strINspModuleName);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Cell Log [11/16/2016 OSC]
	CList <CCellLogItem, CCellLogItem> m_listCellLogItem;	// �̰� Init�� �־��� �ʿ䰡 ���� [11/16/2016 OSC]
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

	// Cell ������ �߹��� Index ��ȣ
	int m_nNextGenIndex;
	CCellInfo m_Cell[MAX_CELL_INFO_BUFFER];

	// ����Ү Cell�� ���� ������ ��� ��ġ��ŭ ���۷� ����� ����
	// ���۳��� ���� tag�� �ְ�޴´�
	CCellTag m_CellTag[CELL_POS_MAX];

	CCellTag GetCellTag(CELL_POS pos);
	CCellTag GetCellTag(JIG_ID jig, JIG_CH nCh);			

	// Ư�� �������� �ش��ϴ� CellTag�� ã�� �Լ�
	CCellTag SearchCellTag(JIG_ID nJig, JIG_CH nCh, CELL_POS startPos = CELL_POS_SHUTTLE1_CH1, CELL_POS endPos = CELL_POS_SHUTTLE2_CH1);
	CCellTag SearchCellTag(CString strInnerID, CELL_POS startPos = CELL_POS_SHUTTLE1_CH1, CELL_POS endPos = CELL_POS_SHUTTLE2_CH1);


	void SetCellTag(CELL_POS pos, CCellTag tag);

	CCellTag RemoveCellTag(CELL_POS pos);
	CCellTag RemoveCellTag( JIG_ID jig, JIG_CH nCh);

	CCellTag CreateCellTag();
	void CreateCellInfo(CELL_POS pos);

	// CellTag�� ������� CellInfo�� ��ȯ�ϴ� �Լ�
	CCellInfo* GetCellInfo(CCellTag tag);
	CCellInfo* GetCellInfo(CELL_POS pos );
	CCellInfo* GetCellInfo(JIG_ID jig, JIG_CH nCh);
	CCellInfo* SearchCellInfo(JIG_ID nJig, JIG_CH nCh, CELL_POS startPos = CELL_POS_SHUTTLE1_CH1, CELL_POS endPos = CELL_POS_SHUTTLE2_CH1);
	CCellInfo* SearchCellInfo(int nJig, int nCh, CELL_POS startPos = CELL_POS_SHUTTLE1_CH1, CELL_POS endPos = CELL_POS_SHUTTLE2_CH1);
	CCellInfo* SearchCellInfo(CString strCellID, CELL_POS startPos = CELL_POS_SHUTTLE1_CH1, CELL_POS endPos = CELL_POS_SHUTTLE2_CH1);

	//kjpark 20161018 �� Tag shift
	void SetCellTagNextPosition(CELL_POS CurrentPos, CELL_POS NextPos);

	CFile m_fileCellData;
	BOOL WriteCellLog(CELL_POS pos);
	BOOL WriteCellLog(JIG_ID jig, JIG_CH ch);
	//kjpark 20180114 NG ���� ��� UI �߰�
	BOOL WriteNGResultLog(CCellInfo* pCell, CTime time);

	CFile m_fileCellDataTemp;
	BOOL WriteCellLogTemp(CELL_POS pos);
	BOOL WriteCellLogTemp(JIG_ID jig, JIG_CH ch);


	//////////////////////////////////////////////////////////////////////////
	// CELL_LOADING ���� [12/19/2017 OSC]
	// CellTag�� �����Ǳ� ���� ������ �ǹǷ� Cell Loading�� 
	// �ƿ� ���뺯���� ����ϰ� ���߿� �Űܴ�´�
	CCellInfo m_InfoCellLoading[JIG_ID_MAX][JIG_CH_MAX];
	CCellInfo* GetCellInfo_CellLoading(JIG_ID jig, JIG_CH nCh);

	//////////////////////////////////////////////////////////////////////////
	// ���α׷� ����۽� CellInfo����.. �� ������ �ʿ� ����
	// ���� ���� CIM�� O �����Ͽ� ���� ���� �� �ְ� �� �� ����
	// TrackIn ���� ������ ������ ���´�. [12/23/2017 OSC]
	void BackupCellToFile();
	void RestoreCellToFile();

};

