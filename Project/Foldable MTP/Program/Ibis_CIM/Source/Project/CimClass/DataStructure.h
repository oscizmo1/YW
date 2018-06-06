
#pragma once


#define MAX_CELL_ID						150		// 2016-08-26, jhLee, �ִ� Cell ID ũ��� 150�ڱ��� �����ϴ� (SDC �ȵ��� ���)


// #define MAX_JIG_CELL_CNT				4		//AMT��� JIG�ȿ� �ö� ���� �ִ� �ִ� CELL ����.
#define MAX_JIG_CELL_CNT				5		//AMT��� JIG�ȿ� �ö� ���� �ִ� �ִ� CELL ����.

#define MAX_JIG_CNT						4

#define CRST_OFF						"0"
#define CRST_REMOTE						"1"
#define CRST_LOCAL						"2"

#define EQST_MAINT_ON					"1"
#define EQST_MAINT_OFF					"2"

#define EQST_AVAILABILITY_DOWN			"1"
#define EQST_AVAILABILITY_UP			"2"

#define EQST_INTERLOCK_ON				"1"
#define EQST_INTERLOCK_OFF				"2"

#define EQST_MOVE_PAUSE					"1"
#define EQST_MOVE_RUNNING				"2"

#define EQST_RUN_IS_CELL				"2"
#define EQST_RUN_NOT_CELL				"1"

#define EQST_FRONT_UP					"1"
#define EQST_FRONT_DOWN					"2"

#define EQST_REAR_UP					"1"
#define EQST_REAR_DOWN					"2"

#define EQST_SAMPLE_ON					"1"
#define EQST_SAMPLE_OFF					"2"

#define OM_AUTO							"1"
#define OM_MANUAL						"2"
#define OM_REWORK						"3"

#define ALST_SET						"1"
#define ALST_CLEAR                      "2"

#define ALCD_LIGHT						"1"
#define ALCD_SERIOUS					"2"

#define PPIDST_EMPTIED					"1"	//����
#define PPIDST_INITIALIZE				"2"	//����
#define PPIDST_CHANGED					"3"	//PPID ����
#define PPIDST_PARA_CHANGED				"4"	//PARAMETER ����

#define MAX_ICT_X						100

#define MAX_MTP_X						50
#define MAX_MTP_Y						50

#define MAX_TSP_X						50
#define MAX_TSP_Y						50
#define MAX_TSP_DATA					20

// 2016-06-06, jhLe, SVID �� ECID �׸� ���� ���� ���� 10%�� �ٿ���.
#define SVID_MAX						1500		// 1500
#define ECID_MAX						1000		// 1000
#define TRID_MAX						10

#define TACT_MAX						100

#define PPID_MODE_CREATE				"1"
#define PPID_MODE_DELETE				"2"
#define PPID_MODE_MODIFY				"3"

#define MAX_ALARM						300
#define MAX_ALARM_LIST				2000		// 2000

#define MAX_MES_COUNT					12
#define MAX_MES_SIZE					40
#define MAX_LONG_MSG_COUNT				1
#define MAX_LONG_MSG_SIZE				150

#define MAX_REPAIR_CNT					20
#define MAX_REPAIR_SET					3

#define MAX_DEFECT_CNT					10

#define MAX_CELL_LOG_CNT				300

#define STR_UV_LEFT_STAGE				"UV_LEFT_STAGE"
#define STR_UV_RIGHT_STGAGE				"UV_RIGHT_STAGE"
#define STR_UV_RETURN_STAGE				"UV_RETURN_STAGE"

#define STR_ALIGN_LEFT_CAM               "ALIGN_LEFT_CAM"
#define STR_ALIGN_RIGHT_CAM              "ALIGN_RIGHT_CAM"
#define STR_ALIGN_CH_01_CAM              "ALIGN_CH_01_CAM"
#define STR_ALIGN_CH_02_CAM              "ALIGN_CH_02_CAM"
#define STR_ALIGN_CH_03_CAM              "ALIGN_CH_03_CAM"
#define STR_ALIGN_CH_04_CAM              "ALIGN_CH_04_CAM"

#define STR_PASS						"PASS"
#define STR_FAIL						"FAIL"

//0:CELLX ����X, 1:CELLO,����X, 2:CELLO,����0
#define  STR_FDC_STEPID_CELLX_ACTX		"0"
#define  STR_FDC_STEPID_CELLO_ACTX		"1"
#define  STR_FDC_STEPID_CELLO_ACTO		"2"

enum E_DEFINE_STR_LEN
{
	STR_LEN_FDC_PARA=41,
	STR_LEN_QMS_PARA=41,
	STR_LEN_CELL_LOG=61,
};

enum E_UV_READ_STAGE
{
	E_UV_LEFT_STAGE=0,
	E_UV_RIGHT_STGAGE,


	MAX_UV_READ_STAGE,
};

enum E_ALIGN_CAM
{	
	E_ALIGN_LEFT_CAM=0,
	E_ALIGN_RIGHT_CAM,

	MAX_ALIGN_CAM,
};

enum E_DIVISION	
{
	E_TEACH=0,
	E_RECIPE,
	E_CONSTANT,
	E_ERRLIST,
};

enum E_RESULT_FILE_TYPE
{
	E_CELL=0,
	E_ICT,
	E_INFO,
	E_PWR,
	E_VT1,
	E_VT2,
	E_MTPW,
	E_TSP,
	E_MTPR,
	E_INS_END,
};

enum E_OM
{
	E_AUTO_MODE	= 1,
	E_MANUAL_MODE,
	E_REWORK_MODE
};

enum E_CELL_POS
{
	E_CELL_POS_1 = 0,
	E_CELL_POS_2,
	E_CELL_POS_3,
	E_CELL_POS_4,
};

enum E_EQST
{
	E_MAINT = 0,
	E_AVAILABILITY,
	E_INTERLOCK,
	E_MOVE,
	E_RUN,
	E_FRONT,
	E_REAR,
	E_SAMPLE,
	EQST_INIT,
	E_MOVE_N_INTERLOCK,
	E_MOVE_N_AVAILABILITY,
	E_EQST_MAX,

};

enum E_EQST_MATCH
{
	E_EQST_MATCH_NONE=0,
	E_EQST_MATCH_PAUSE,
	E_EQST_MATCH_EXECUTING,
	E_EQST_MATCH_FAULT,
	E_EQST_MATCH_INTERLOCK,
	E_EQST_MATCH_CELL_EXIST,
	E_EQST_MATCH_CELL_NOT_EXIST,
	E_EQST_MATCH_MAX,
};

enum E_JIG_X
{
	JIG_1 = 0,
	JIG_2,
	JIG_3,
	JIG_4,
	JIG_MAX
};

enum E_UNIT
{
	E_UNIT_LOAD=0,
	E_UNIT_INSPECTION,
	E_UNIT_UNLOAD,
	E_UNIT_MAX,
};

enum E_CRST 
{
	E_CRST_OFFLINE = 0,
	E_CRST_REMOTE,
	E_CRST_LOCAL
};

enum E_CONTROL_STATE 
{
	E_CONTROL_OFFLINE = 1,
	E_CONTROL_LOCAL,
	E_CONTROL_REMOTE
};

// 2015-09-21 JH-PARK DELETE
// enum E_YMS_RETURN
// {
// 	E_YMS_NONE=-1,
// 	E_YMS_SKIP,
// 	E_YMS_SUCESS,
// 	E_YMS_FAIL,
// 	E_YMS_SIGNAL_NOT_CONNECTION,
// 	E_YMS_NO_FILE,
// 	E_YMS_ERR_FORMAT,
// 	E_YMS_ERR_OPEN,
// 	E_YMS_ERR_SYSTEM,
// };

enum E_QMS_RETURN
{
	E_QMS_NONE=-1,
	E_QMS_SKIP,
	E_QMS_SUCESS,
	E_QMS_FAIL,
	E_QMS_SIGNAL_NOT_CONNECTION,
	E_QMS_NO_FILE,
	E_QMS_ERR_FORMAT,
	E_QMS_ERR_OPEN,
	E_QMS_ERR_SYSTEM,
};

//#endif




//////////////////////////////////////////////////////////////////////////

// 2016-06-06, jhLee, ������ ���忡�� ������ ����
// Q. S5F104 Current Alarm List Request �� ���ϸ� �������� Alarm�� Report�ϰ� �Ǿ��ִµ�, �������� Alarm�� �߻��Ǵ� ���� �ִ°� ?
// A. ���� �츮 ȸ�� ����� �� ������ 1���� Alarm�� �߻�������, Ÿ�� ����� �������� Alarm�� �߻��Ǵ� ��쵵 �ְ�, 
//    �׷� ��츦 �����ϱ� ���� �������� Alarm�� Listȭ�Ͽ� ���� �� �� �ֵ��� �Ǿ��ִ�.
//
// ���� : CArry ���� ���� �����̳ʸ� �̿��Ͽ� ���� �߻��Ǵ� Alarm���� �����Ͽ� �����ϰ� Clear�ϸ� �����ϴ� ���� ������ ������ ó���Ѵ�.
//
//

// tag_Alarm
struct tag_Alarm 
{
	char		m_cALST[2];   //1:Set, 2:Clear  
	char		m_cALCD[2];   //1:Light Alarm, 2:Serious Alarm
	char		m_cALID[11];
	char		m_cALTX[121];  


	tag_Alarm(void) 
	{
		memset(m_cALST, 0, 2);
		memset(m_cALCD, 0, 2);
		memset(m_cALID, 0, 11);      
		memset(m_cALTX, 0, 121);
	};

	tag_Alarm(tag_Alarm &rb) 
	{
		memcpy(m_cALST, rb.m_cALST, 2);		
		memcpy(m_cALCD, rb.m_cALCD, 2);
		memcpy(m_cALID, rb.m_cALID, 11);
		memcpy(m_cALTX, rb.m_cALTX, 121);

		*this = rb;
	};

	tag_Alarm& operator = (tag_Alarm &rb)
	{
		if (this == &rb)
			return *this;

		memcpy(m_cALST, rb.m_cALST, 2);		
		memcpy(m_cALCD, rb.m_cALCD, 2);
		memcpy(m_cALID, rb.m_cALID, 11);
		memcpy(m_cALTX, rb.m_cALTX, 121);

		return *this;
	};

	void Clear(void)
	{
		memset(m_cALST, 0, 2);
		memset(m_cALCD, 0, 2);
		memset(m_cALID, 0, 11);      
		memset(m_cALTX, 0, 121);
	};

	~tag_Alarm(void) { };
};

//////////////////////////////////////////////////////////////////////////
// tag_AlarmInsert
struct tag_AlarmInsert
{
	BOOL		bInsert;
	CTime		timeSet;
	char		m_cALCD[2];   //1:Light Alarm, 2:Serious Alarm
	char		m_cALID[11];
	char		m_cALTX[121];  
	CTime		timeReset;
	CTime		timeRun;

	SYSTEMTIME	m_systmTimeSet;
	SYSTEMTIME	m_systmTimeRun;


	tag_AlarmInsert(void) 
	{
		bInsert = FALSE;
		timeSet = CTime::GetCurrentTime();
		memset(m_cALCD, 0, 2);
		memset(m_cALID, 0, 11);      
		memset(m_cALTX, 0, 121);
		timeReset = CTime::GetCurrentTime();
		timeRun = CTime::GetCurrentTime();

		m_systmTimeSet	= SYSTEMTIME();		
		m_systmTimeRun	= SYSTEMTIME();	
	};

	tag_AlarmInsert(tag_AlarmInsert &rb) 
	{
		bInsert = rb.bInsert;
		timeSet = rb.timeSet;
		memcpy(m_cALCD, rb.m_cALCD, 2);
		memcpy(m_cALID, rb.m_cALID, 11);
		memcpy(m_cALTX, rb.m_cALTX, 121);
		timeReset = rb.timeReset;
		timeRun = rb.timeRun;

		m_systmTimeSet	= rb.m_systmTimeSet;		
		m_systmTimeRun	= rb.m_systmTimeRun;	

		*this = rb;
	};

	tag_AlarmInsert& operator = (tag_AlarmInsert &rb)
	{
		if (this == &rb)
			return *this;

		bInsert = rb.bInsert;
		timeSet = rb.timeSet;
		memcpy(m_cALCD, rb.m_cALCD, 2);
		memcpy(m_cALID, rb.m_cALID, 11);
		memcpy(m_cALTX, rb.m_cALTX, 121);
		timeReset = rb.timeReset;
		timeRun = rb.timeRun;

		m_systmTimeSet	= rb.m_systmTimeSet;		
		m_systmTimeRun	= rb.m_systmTimeRun;	


		return *this;
	};

	void Clear(void)
	{
		bInsert = FALSE;
		timeSet = CTime::GetCurrentTime();
		memset(m_cALCD, 0, 2);
		memset(m_cALID, 0, 11);      
		memset(m_cALTX, 0, 121);
		timeReset = CTime::GetCurrentTime();
		timeRun = CTime::GetCurrentTime();

		m_systmTimeSet	= SYSTEMTIME();		
		m_systmTimeRun	= SYSTEMTIME();	

	};

	~tag_AlarmInsert(void) { };
};

struct tag_Recipe_Data
{
	enum{ MAX_DATA_CNT = 5000, };
	enum{ MAX_DATA_LEN = 41, };

	int		nDataCount;
	char	cDataName[MAX_DATA_CNT][MAX_DATA_LEN];
	char	cDataValue[MAX_DATA_CNT][MAX_DATA_LEN];
	
	void Clear()
	{
		nDataCount = 0;
		for (int i=0; i<MAX_DATA_CNT; i++)
		{
			cDataName[i][0] = 0x00;
			cDataValue[i][0] = 0x00;
		}
	};

	BOOL AddData(char* pName, char* pV)
	{
		if(nDataCount<0 || nDataCount>= MAX_DATA_CNT)
			return FALSE;
		if(strlen(pName)<0 || strlen(pName)>=MAX_DATA_LEN)
			return FALSE;
		if(strlen(pV)<0 || strlen(pV)>=MAX_DATA_LEN)
			return FALSE;

		if(strlen(pName)<MAX_DATA_LEN)
			strcpy_s(cDataName[nDataCount], MAX_DATA_LEN, pName);
		else
		{
			memcpy(cDataName[nDataCount], pName, MAX_DATA_LEN);
			cDataName[nDataCount][MAX_DATA_LEN-1] = 0x00;
		}

		if(strlen(pV)<MAX_DATA_LEN)
			strcpy_s(cDataValue[nDataCount++], MAX_DATA_LEN, pV);
		else
		{
			memcpy(cDataValue[nDataCount], pV, MAX_DATA_LEN);
			cDataValue[nDataCount][MAX_DATA_LEN-1] = 0x00;
		}

		//TRACE("RecipeData - AddData(%s)(%s), Count(%d)\n", pName, pV, nDataCount);

		return TRUE;
	}	
};



typedef CStringList						RCP_LIST;

struct tag_Recipe_Validation
{
	enum
	{
		ERR_STACK_OVERFLOW	= -2,
		ERR_SYNTEXT			= -1,
	};

	static RCP_LIST			m_lstRcp;

	static int LoadParamList(tag_Recipe_Data* pPParam, char* pFileName, char* pSimbolChar=NULL);
	static BOOL IsBlankLine(char* pLine);
	static BOOL IsSkipLine(char* pLine);
	static int IsRcpName(char* pFileName);
	static void GetRcpParam(char* pPPID, tag_Recipe_Data* pPParam, char* pMode);
	static void SaveRcp(CString strPath, CString strBackupPath,CString* ParamName, double* dParaVal, int nCount);


	// static BOOL GetXmlParam(CString strPPID);

	static int	LoadRcpList();
	//static BOOL CheckRcpName(LPCTSTR lpFileFullName);
};



struct tag_CellData
{

	//old char	m_cCellID[41];
	char	m_cCellID[MAX_CELL_ID + 1];
	char	m_cInnerID[41];
	char	m_cPPID[41];
	char	m_cProductID[41];
	char	m_cStepID[41];
	char	m_cPJID[41];
	char	m_cPlanQty[11];
	char	m_cProcessedQty[11];
	char	m_cReaderID[11];
	char	m_cReaderResultCode[2];
	char	m_cMaterialID[21];
	char	m_cMLN[11];
	char	m_cDVName[21];
	char	m_cDV[41];
	char	m_cJudge[2];
	char	m_cJudgeOper1[3];
	char	m_cJudgeOper2[3];
	char	m_cReasonCodeOper1[21];
	char	m_cReasonCodeOper2[21];
	char	m_cOperatorID1[21];
	char	m_cOperatorID2[21];
	char	m_cReasonCode[21];
	char	m_cDescription[41];
	char	m_cCellInfoResult[3];

	char	m_cDefectPos[41];				// 2016-11-19, jhLee, Defect ��ġ �߰�
	char	m_cProcessFlag[41];				// CEID 609, �ش� ������ �����Ͽ��°� ? Y/N

	char	m_cMTPWrite[41];				// MTP WRITE �ҷ��� �ϵ�? �ϴ� �߰�~ [9/27/2017 OSC]

	tag_CellData(void) 
	{
		Clear();
	}

	void Clear()
	{
		memset(m_cCellID, 0, sizeof(m_cCellID));
		memset(m_cInnerID, 0, sizeof(m_cInnerID));
		memset(m_cPPID, 0, sizeof(m_cPPID));
		memset(m_cProductID, 0, sizeof(m_cProductID));
		memset(m_cStepID, 0, sizeof(m_cStepID));
		memset(m_cPJID, 0, sizeof(m_cPJID));;
		memset(m_cPlanQty, 0, sizeof(m_cPlanQty));
		memset(m_cProcessedQty, 0, sizeof(m_cProcessedQty));
		memset(m_cReaderID, 0, sizeof(m_cReaderID));
		memset(m_cReaderResultCode, 0, sizeof(m_cReaderResultCode));
		memset(m_cMaterialID, 0, sizeof(m_cMaterialID));
		memset(m_cMLN, 0, sizeof(m_cMLN));
		memset(m_cDVName, 0, sizeof(m_cDVName));
		memset(m_cDV, 0, sizeof(m_cDV));;
		memset(m_cJudge, 0, sizeof(m_cJudge));
		memset(m_cJudgeOper1, 0, sizeof(m_cJudgeOper1));
		memset(m_cJudgeOper2, 0, sizeof(m_cJudgeOper2));
		memset(m_cReasonCodeOper1, 0, sizeof(m_cReasonCodeOper1));
		memset(m_cReasonCodeOper2, 0, sizeof(m_cReasonCodeOper2));
		memset(m_cOperatorID1, 0, sizeof(m_cOperatorID1));
		memset(m_cOperatorID2, 0, sizeof(m_cOperatorID2));
		memset(m_cReasonCode, 0, sizeof(m_cReasonCode));
		memset(m_cDescription, 0, sizeof(m_cDescription));
		memset(m_cCellInfoResult, 0, sizeof(m_cCellInfoResult));
		memset(m_cDefectPos, 0, sizeof(m_cDefectPos));
		memset(m_cProcessFlag, 0, sizeof(m_cProcessFlag));
	}
};

struct tag_LossCode		//20141223 sms
{
	char	m_cLossCode[20];
	char	m_cLossDesc[40];

	tag_LossCode(void)
	{
		memset(m_cLossCode, 0, 20);
		memset(m_cLossDesc, 0, 40);
	};

	void Clear(void)
	{
		memset(m_cLossCode, 0, 20);
		memset(m_cLossDesc, 0, 40);
	};

	tag_LossCode& operator = (tag_LossCode &rb)
	{
		if (this == &rb)
			return *this;

		memcpy(m_cLossCode, rb.m_cLossCode, 20);
		memcpy(m_cLossDesc, rb.m_cLossDesc, 40);

		return *this;
	};
};

class CCimUnitJig		//JIG RUN/DOWN���� ǥ��
{
public:

	char	m_cUnitID[41];		//UNIT ID
	char	m_cUnitST[2];		//UNIT STATUS 0:IDLE, 1:RUN, 2:DOWN

	tag_CellData		tCellData[MAX_JIG_CELL_CNT];	//JIG�ȿ� ��� ���� CELL����

	CCimUnitJig(void) 
	{
		Clear();
// 		memset(m_cUnitID, 0, sizeof(m_cUnitID));
// 		memset(m_cUnitST, 0, sizeof(m_cUnitST));
	};

	virtual ~CCimUnitJig(void) {};

	CCimUnitJig(CCimUnitJig &rb) 
	{
		for (int i=0; i<MAX_JIG_CELL_CNT; i++)
			tCellData[i] = rb.tCellData[i];
	};

	CCimUnitJig& operator= (CCimUnitJig &rb)
	{
		if (this == &rb)
			return *this;

		for (int i=0; i<MAX_JIG_CELL_CNT; i++)
			tCellData[i] = rb.tCellData[i];
		
		return *this;
	};

	void Clear()
	{
		memset(m_cUnitID, 0, sizeof(m_cUnitID));
		memset(m_cUnitST, 0, sizeof(m_cUnitST));

		for (int i=0; i<MAX_JIG_CELL_CNT; i++)
			tCellData[i].Clear();
	}
};

struct tag_TraceData
{
	BOOL	m_bSend;
	char	m_cSVID[SVID_MAX][21];
	char	m_cTRID[21];
	char	m_cDSPER[7];
	char	m_cREPGSZ[3];
	long	m_nTOTMP;
	short	m_nSVIDCnt;
	long	m_nSMPLN;
};

class CCimFDC
{
public:
	CCimFDC()
	{
		Clear();
	};

	~CCimFDC()
	{

	};

	BOOL	m_bCheck[SVID_MAX];
	char	m_cSetUNIT[SVID_MAX][41];
	char	m_cSetSVID[SVID_MAX][21];
	char	m_cSetSVNAME[SVID_MAX][41];

	//old char	m_cSetSV[SVID_MAX][41];
	char	m_cSetSV[SVID_MAX][MAX_CELL_ID + 1];			// ����
	//kjpark 20170929 FDC ������, ������, ���� ����
	char	m_cSetCV[SVID_MAX][MAX_CELL_ID + 1];			// ������
	char	m_cSetRV[SVID_MAX][MAX_CELL_ID + 1];			// ������

	char	m_cSetSVMAX[SVID_MAX][21];
	char	m_cSetSVMIN[SVID_MAX][21];
	char	m_cSetSVTYPE[SVID_MAX][21];
	char	m_cSetSVKEY[SVID_MAX][21];
	char	m_cMappingSVNAME[SVID_MAX][41];		//20141222 sms add.

	int		m_nSetWarring;
	int		m_nDipWarring;

	char	m_cID[25];
	long	m_nFDC_TotalCount;

	tag_TraceData m_tTraceData[TRID_MAX];
	tag_TraceData m_tTraceBuf;	

	int GetTraceData(CString strTRID)
	{
		int nIndex=-1;
		for (int i=0; i<TRID_MAX; i++)
		{
			if (strcmp(m_tTraceData[i].m_cTRID, strTRID)==0)
			{
				nIndex = i;
				break;
			}
		}
		return nIndex;
	}

	int AddTraceData(tag_TraceData tData)
	{
		int nIndex=-1;
		for (int i=0; i<TRID_MAX; i++)
		{
			if (!m_tTraceData[i].m_bSend)
			{
				TraceSendClear(i);
				m_tTraceData[i] = tData;
				nIndex = i;
				break;
			}
		}

		return nIndex;
	}

	void TraceBufClear()
	{
		m_tTraceBuf.m_bSend = FALSE;
		memset(m_tTraceBuf.m_cSVID, 0x00,SVID_MAX*21*sizeof(char));
		m_tTraceBuf.m_cTRID[0] = 0x00;
		m_tTraceBuf.m_cDSPER[0] = 0x00;
		m_tTraceBuf.m_cREPGSZ[0]= 0x00;
		m_tTraceBuf.m_nTOTMP = 0;
		m_tTraceBuf.m_nSVIDCnt = 0;
		m_tTraceBuf.m_nSMPLN = 1;
	}

	void TraceSendClear(int nIndex)
	{
		m_tTraceData[nIndex].m_bSend = FALSE;
		memset(m_tTraceData[nIndex].m_cSVID, 0x00,SVID_MAX*21*sizeof(char));
		m_tTraceData[nIndex].m_cTRID[0] = 0x00;
		m_tTraceData[nIndex].m_cDSPER[0] = 0x00;
		m_tTraceData[nIndex].m_cREPGSZ[0]= 0x00;
		m_tTraceData[nIndex].m_nTOTMP = 0;
		m_tTraceData[nIndex].m_nSVIDCnt = 0;
		m_tTraceData[nIndex].m_nSMPLN = 1;
	}

	CCimFDC& operator= (CCimFDC &rb)
	{
		if (this == &rb)
			return *this;

		memcpy(m_cID, rb.m_cID, 25);
		m_nFDC_TotalCount = rb.m_nFDC_TotalCount;
		memcpy(m_cSetUNIT, rb.m_cSetUNIT, 41);
		memcpy(m_cSetSVID, rb.m_cSetSVID,21);
		memcpy(m_cSetSVNAME, rb.m_cSetSVNAME,41);
		memcpy(m_cSetSV, rb.m_cSetSV, 41);
		//kjpark 20170929 FDC ������, ������, ���� ����
		memcpy(m_cSetCV, rb.m_cSetCV, 41);
		memcpy(m_cSetRV, rb.m_cSetRV, 41);
		memcpy(m_cSetSVMAX, rb.m_cSetSVMAX, 21);
		memcpy(m_cSetSVMIN, rb.m_cSetSVMIN, 21);
		memcpy(m_cSetSVTYPE, rb.m_cSetSVTYPE, 21);
		memcpy(m_cSetSVKEY, rb.m_cSetSVKEY, 21);
		memcpy(m_cMappingSVNAME, rb.m_cMappingSVNAME,41);	//20141222 sms

		m_nSetWarring = rb.m_nSetWarring;
		m_nDipWarring = rb.m_nDipWarring;

		return *this;
	};

	void Clear()
	{
		m_cID[0] = 0x00;
		m_nFDC_TotalCount = 0;

		memset(m_cSetSVID, 0x00,SVID_MAX*21*sizeof(char));
		memset(m_cSetUNIT, 0x00,SVID_MAX*41*sizeof(char));
		memset(m_cSetSVNAME, 0x00,SVID_MAX*41*sizeof(char));
		memset(m_cSetSV, 0x00,SVID_MAX*41*sizeof(char));
		//kjpark 20170929 FDC ������, ������, ���� ����
		memset(m_cSetCV, 0x00,SVID_MAX*41*sizeof(char));
		memset(m_cSetRV, 0x00,SVID_MAX*41*sizeof(char));
		memset(m_cSetSVMAX, 0x00,SVID_MAX*21*sizeof(char));
		memset(m_cSetSVMIN, 0x00,SVID_MAX*21*sizeof(char));
		memset(m_cSetSVTYPE, 0x00,SVID_MAX*21*sizeof(char));
		memset(m_cSetSVKEY, 0x00,SVID_MAX*21*sizeof(char));
		memset(m_cMappingSVNAME, 0x00,SVID_MAX*41*sizeof(char));	//20141222 sms

		m_nSetWarring = 0;
		m_nDipWarring = 0;

		for (int i=0; i<SVID_MAX; i++)
			m_bCheck[i] = FALSE;

		for (int i=0; i<TRID_MAX; i++)
		{
			TraceSendClear(i);
		}

	};

	
};

class CCimECID
{
public:

	enum {
		eMAX_LEN_ECID		= 8,
		eMAX_LEN_ECSEC		= 40,
		eMAX_LEN_ECNAME	= 40,
		eMAX_LEN_ECV		= 40,
	};

	CCimECID()
	{
		m_nECID_TotalCount = 0;
		m_cECID[0][0] = 0x00;
		m_cECSEC[0][0] = 0x00;
		m_cECKEY[0][0] = 0x00;
		m_cECNAME[0][0] = 0x00;
		m_cECV[0][0] = 0x00;
	};

	~CCimECID()
	{

	};

	char	m_cECID[ECID_MAX][9];
	char	m_cECSEC[ECID_MAX][41];
	char	m_cECKEY[ECID_MAX][41];
	char	m_cECNAME[ECID_MAX][41];
	char	m_cECV[ECID_MAX][41];
	long	m_nECID_TotalCount;

	CCimECID& operator= (CCimECID &rb)
	{
		if (this == &rb)
			return *this;

		m_nECID_TotalCount = rb.m_nECID_TotalCount;
		memcpy(m_cECID, rb.m_cECID, 9);
		memcpy(m_cECSEC, rb.m_cECSEC,41);
		memcpy(m_cECKEY, rb.m_cECKEY,41);
		memcpy(m_cECNAME, rb.m_cECNAME,41);
		memcpy(m_cECV, rb.m_cECV,41);

		return *this;
	};

	void Clear()
	{
		m_nECID_TotalCount = 0;
		//m_cECID[0][0] = 0x00;
		memset(m_cECID, 0x00,ECID_MAX*9*sizeof(char));
		memset(m_cECSEC, 0x00,ECID_MAX*41*sizeof(char));
		memset(m_cECKEY, 0x00,ECID_MAX*41*sizeof(char));
		memset(m_cECNAME, 0x00,ECID_MAX*41*sizeof(char));
		memset(m_cECV, 0x00,ECID_MAX*41*sizeof(char));
// 		m_cECSEC[0][0] = 0x00;
// 		m_cECNAME[0][0] = 0x00;
// 		m_cECV[0][0] = 0x00;
	};
};


//MES OPCALL EVENT
#define MAX_OPCALL_MES 100
struct tag_OPCallMES
{
	char	m_cOPCallID[21];
	char	m_cOPCallType[21];
	char	m_cOPCallMSG[121];

	tag_OPCallMES(void)
	{
		memset(m_cOPCallID, 0, 21);
		memset(m_cOPCallType, 0, 21);
		memset(m_cOPCallMSG, 0, 121);
	};

	void Clear(void)
	{
		memset(m_cOPCallID, 0, 21);
		memset(m_cOPCallType, 0, 21);
		memset(m_cOPCallMSG, 0, 121);
	};

	tag_OPCallMES& operator = (tag_OPCallMES &rb)
	{
		if (this == &rb)
			return *this;

		memcpy(m_cOPCallID, rb.m_cOPCallID, 21);
		memcpy(m_cOPCallType, rb.m_cOPCallType, 21);
		memcpy(m_cOPCallMSG, rb.m_cOPCallMSG, 121);

		return *this;
	};

};

enum INTERLOCK_STATE
{
	INTERLOCK_NONE = 0,
	INTERLOCK_ADD,
	INTERLOCK_HAPPEN
};

//MES INTERLOCK EVENT
#define MAX_INTERLOCK_MES 100
class tag_InterlockMES
{
public:
	char	m_cInterlockID[21];
	char	m_cInterlockMSG[121];
	UINT	m_cInterlockType;
	INTERLOCK_STATE	m_cInterlockState;

	tag_InterlockMES()
	{
		Clear();
	};

	void Clear(void)
	{
		memset(m_cInterlockID, 0, 21);
		memset(m_cInterlockMSG, 0, 121);
		m_cInterlockType = NULL;
		m_cInterlockState = INTERLOCK_NONE;
	};

	// ���Ͷ� ������ �߰��Լ� �߰� - LSH171128
	void SetInterLock(CString strID, CString strMsg, UINT nType)
	{
		/* ID */
		strcpy_s(m_cInterlockID, strID);
		/* MSG */
		strcpy_s(m_cInterlockMSG, strMsg);
		/* TYPE */
		m_cInterlockType = nType;
		/* STATE */
		m_cInterlockState = INTERLOCK_ADD;
	}
};

// ���ÿ� ó���� �� �ִ� CellInformation CellID ����
#define MAX_CELL_INFORMATION_CNT		10
class tag_CellInformation
{
public:
	tag_CellInformation(void)
	{
		Clear();
	};
	~tag_CellInformation(void) {}

private:
	CStringList	m_listCell;		// CellInfomationDownload ó����
	CString		m_strLastCellID;

public:
	void Clear()
	{
		m_listCell.RemoveAll();
		m_strLastCellID.Empty();
	}

	void AddCellID(CString strCellID)
	{
		m_listCell.AddTail(strCellID);
		m_strLastCellID = strCellID;
		if(m_listCell.GetCount() > MAX_CELL_INFORMATION_CNT)
			m_listCell.RemoveHead();
	}

	BOOL CheckCellID(CString strCellID)
	{
		if(m_listCell.Find(strCellID))
			return TRUE;

		return FALSE;
	}

	CString GetLastCellID()
	{
		return m_strLastCellID;
	}
};

class tag_CellLotInformation : public tag_CellInformation
{
public:
	tag_CellLotInformation(void) : tag_CellInformation()
	{
		Clear();
	};
	~tag_CellLotInformation(void) {}

private:
	CString		m_strCode;

public:
	void Clear()
	{
		tag_CellInformation::Clear();
		m_strCode.Empty();
	}

	void SetOptionCode(CString strCode)
	{
		m_strCode = strCode;
	}

	CString GetOptionCode()
	{
		return m_strCode;
	}

};

// ���ÿ� ó���� �� �ִ� CelTrackIn CellID ����
#define MAX_CELL_TRACKIN_CNT		10
class tag_CellTrackIn
{
public:
	tag_CellTrackIn(void)
	{
		Clear();
	};
	~tag_CellTrackIn(void) {}

private:
	CStringList	m_listCellID;		// ProcessStart ó����
	tag_CellData m_CellData;

public:
	void Clear()
	{
		m_listCellID.RemoveAll();
		m_CellData.Clear();
	}

	tag_CellData* GetCellDataPtr()
	{
		return &m_CellData;
	}

	void AddCellID(CString strCellID)
	{
		m_listCellID.AddTail(strCellID);
		if(m_listCellID.GetCount() > MAX_CELL_TRACKIN_CNT)
			m_listCellID.RemoveHead();
	}

	BOOL CheckCellID(CString strCellID)
	{
		if(m_listCellID.Find(strCellID))
			return TRUE;

		return FALSE;
	}

};

class CDataStructure
{
public:
	CDataStructure(void)
	{
		Clear();
	};
	~CDataStructure(void) {}


protected:


public:
	//BOOL				m_nAliveCount;

	char				m_cFDC_PATH[256];
	char				m_cECID_PATH[256];
	char				m_cCRST[2];				//0:OFF-LINE, 1:ON-LINE REMOTE, 2:ON-LINE LOCAL
	char				m_cOldCRST[2];				//0:OFF-LINE, 1:ON-LINE REMOTE, 2:ON-LINE LOCAL
	char				m_cToCRST[2];
	char				m_cEQPID[41];
	char				m_cMaint[2];			//Maint(Equipment Status), 1:Maint On, 2:Maint Off 
	char				m_cAvilability[2];			//Availability(Equipment Status), 1:Down, 2:Up 
	char				m_cInterlock[2];			//Interlock(Equipment Status), 1:I/L On, 2:I/L Off 
	char				m_cMove[2];			//Move Status(Equipment Status), 1:Pause, 2:Running 
	char				m_cRun[2];			//Run Status(Equipment Status), 1:Idle, 2:Run 

	char				m_cFront[2];		//Front State, 1:Down, 2:Up
	char				m_cRear[2];			//Rear State, 1:Down, 2:Up
	char				m_cSampleLot[2];	//Sample Lot state, 1:On, 2:Off

	char				m_cOldMaint[2];			//Maint(Equipment Status), 1:Maint On, 2:Maint Off 
	char				m_cOldAvilability[2];			//Availability(Equipment Status), 1:Down, 2:Up 
	char				m_cOldInterlock[2];			//Interlock(Equipment Status), 1:I/L On, 2:I/L Off 
	char				m_cOldMove[2];			//Move Status(Equipment Status), 1:Pause, 2:Running 
	char				m_cOldRun[2];			//Run Status(Equipment Status), 1:Idle, 2:Run 

	char				m_cOldFront[2];		//Front State, 1:Down, 2:Up
	char				m_cOldRear[2];			//Rear State, 1:Down, 2:Up
	char				m_cOldSampleLot[2];	//Sample Lot state, 1:On, 2:Off

	char				m_cMDLN[20];				//Equipment Model Type
	char				m_cPPID[41];
	char				m_cOldPPID[41];

	char				m_cOperatorID1[21];	// D Zone �˻���
	char				m_cOperatorID2[21];	// B Zone �۾���
	//GT.PARK - USER���� ��� �߰�.
	char	m_cOptionInfo[11];
	char	m_cComment[41];
	char	m_cPassword[41];
	///
	char				m_cSoftRev[7];			//Software revision code
	char				m_cOM[2];				//Operation Mode, 1: Auto Mode / 2: Manual Mode / 4: Rework Mode
	char				m_cPPIDST[2];			//PPID CHANGE STATUS 1:Emptied / 2:Initalize / 3:Changed
	char				m_cOPCallID[21];
	char				m_cOPCallType[21];
	char				m_cOPCallMSG[121];
	char				m_cReasonCode[21];
	char				m_cDescription[41];
	char				m_cLCTime[15];
	//kjpark 20170929 Terminel TID ���
	char				m_cTerminalID[21];
	char				m_cTerminalMsg[121];

	char				m_cInterlockID[21];
	char				m_cInterlockType[21];
	char				m_cInterlockMSG[121];

	char				m_cRepairInfoReq[MAX_CELL_ID + 1];
	char				m_cCodeInfoReq[2];
	
	tag_CellData		m_CellDummy;					// �Ͻ����� ������ ������


	CCimFDC				m_FDC;
	CCimECID			m_ECID;
	tag_OPCallMES		m_tOPCallMES[MAX_OPCALL_MES];
	short				m_nOPCallCount;
	tag_OPCallMES		m_tUnitOPCallMES[MAX_OPCALL_MES];
	short				m_nUnitOPCallCount;

	tag_InterlockMES	m_tInterlockMES[MAX_INTERLOCK_MES];

	tag_Alarm			m_tAlarm;
	tag_AlarmInsert		m_tAlarmInsert;
	tag_Alarm			m_tCurAlarmList[MAX_ALARM];
	int					m_nCurAlarmListCnt;

	char				m_cInitCellSizeX[41];
	char				m_cInitCellSizeY[41];
	char				m_cInitOrigin[41];

	char				m_cAMTVersion[41];		//VERSION,0.1
	char				m_cVersionUseCellLog[31];
	char				m_cProcessID[41];

	char				m_cFixPPID[41];
	char				m_cFixPPIDMode[2];

	short			m_sEqstMatch;
	short			m_sUnitSTMatch;
	short			m_sOldEqstMatch;

	char				m_cLineID[41];

	tag_LossCode		m_tLossCode;	//20141223 sms

	char		m_strInterlockClearType[20];

	tag_CellInformation m_CellInformation;
	tag_CellLotInformation m_CellLotInformation;
	tag_CellTrackIn		m_CellTrackIn;				// TrackIn�� ����
	tag_CellData			m_CellDataInspectionEnd;	// InspectionEnd�� ����
	tag_CellData			m_CellDataTrackOut;			// TrackOut�� ����


public:
	void Clear()
	{

		memset(m_cFDC_PATH, 0, sizeof(m_cFDC_PATH));
		memset(m_cECID_PATH, 0, sizeof(m_cECID_PATH));
		memset(m_cCRST, 0, sizeof(m_cCRST));
		memset(m_cToCRST, 0, sizeof(m_cToCRST));
		memset(m_cEQPID, 0, sizeof(m_cEQPID));
		memset(m_cMaint, 0, sizeof(m_cMaint));
		memset(m_cAvilability, 0, sizeof(m_cAvilability));
		memset(m_cInterlock, 0, sizeof(m_cInterlock));
		memset(m_cMove, 0, sizeof(m_cMove));
		memset(m_cRun, 0, sizeof(m_cRun));

		memset(m_cFront, 0, sizeof(m_cFront));
		memset(m_cRear, 0, sizeof(m_cRear));
		memset(m_cSampleLot, 0, sizeof(m_cSampleLot));

		memset( m_cOldMaint, 0, sizeof( m_cOldMaint));
		memset(m_cOldAvilability, 0, sizeof(m_cOldAvilability));
		memset(m_cOldInterlock, 0, sizeof(m_cOldInterlock));
		memset(m_cOldMove, 0, sizeof(m_cOldMove));
		memset(m_cOldRun, 0, sizeof(m_cOldRun));

		memset(m_cOldFront, 0, sizeof(m_cOldFront));
		memset(m_cOldRear, 0, sizeof(m_cOldRear));
		memset(m_cOldSampleLot, 0, sizeof(m_cOldSampleLot));

		memset(m_cMDLN, 0, sizeof(m_cMDLN));
		memset(m_cPPID, 0, sizeof(m_cPPID));
		memset(m_cOldPPID, 0, sizeof(m_cOldPPID));
		memset(m_cOperatorID1, 0, sizeof(m_cOperatorID1));
		memset(m_cOperatorID2, 0, sizeof(m_cOperatorID2));

		memset(m_cOptionInfo, 0, sizeof(m_cOptionInfo));
		memset(m_cComment, 0, sizeof(m_cComment));
		memset(m_cPassword, 0, sizeof(m_cPassword));

		memset(m_cSoftRev, 0, sizeof(m_cSoftRev));
		memset(m_cOM, 0, sizeof(m_cOM));
		memset(m_cPPIDST, 0, sizeof(m_cPPIDST));
		memset(m_cOPCallID, 0, sizeof(m_cOPCallID));
		memset(m_cOPCallType, 0, sizeof(m_cOPCallType));
		memset(m_cOPCallMSG, 0, sizeof(m_cOPCallMSG));
		memset(m_cReasonCode, 0, sizeof(m_cReasonCode));
		memset(m_cDescription, 0, sizeof(m_cDescription));
		memset(m_cLCTime, 0, sizeof(m_cLCTime));
		
		//kjpark 20170929 Terminel TID ���
		memset(m_cTerminalID, 0, sizeof(m_cTerminalID));
		memset(m_cTerminalMsg, 0, sizeof(m_cTerminalMsg));
		m_nOPCallCount = 0;
		m_nCurAlarmListCnt = 0;
		memset(m_cInterlockID, 0, sizeof(m_cInterlockID));
		memset(m_cInterlockType, 0, sizeof(m_cInterlockType));
		memset(m_cInterlockMSG, 0, sizeof(m_cInterlockMSG));

		memset(m_cRepairInfoReq, 0, sizeof(m_cRepairInfoReq));
		memset(m_cCodeInfoReq, 0, sizeof(m_cCodeInfoReq));
		
		memset(m_cInitCellSizeX, 0, sizeof(m_cInitCellSizeX));
		memset(m_cInitCellSizeY, 0, sizeof(m_cInitCellSizeY));
		memset(m_cInitOrigin, 0, sizeof(m_cInitOrigin));

		m_tAlarm.Clear();
		m_tAlarmInsert.Clear();

		memset(m_cAMTVersion, 0, sizeof(m_cAMTVersion));
		memset(m_cProcessID,0, sizeof(m_cProcessID));

		memset(m_cFixPPID, 0, sizeof(m_cFixPPID));
		memset(m_cFixPPIDMode, 0, sizeof(m_cFixPPIDMode));


		m_FDC.Clear();
		m_ECID.Clear();


		m_sEqstMatch = E_EQST_MATCH_NONE;
		m_sOldEqstMatch = E_EQST_MATCH_NONE;

		memset(m_cLineID, 0, sizeof(m_cLineID));

		for(int i = 0; i < MAX_OPCALL_MES; i++)
		{
			m_tOPCallMES[i].Clear();
			m_tUnitOPCallMES[i].Clear();
		}
		m_nOPCallCount = 0;
		m_nUnitOPCallCount = 0;

		for(int i = 0; i < MAX_INTERLOCK_MES; i++)
		{
			m_tInterlockMES[i].Clear();
		}

		memset(m_strInterlockClearType, 0, sizeof(m_strInterlockClearType));

		m_CellInformation.Clear();
		m_CellLotInformation.Clear();
		m_CellTrackIn.Clear();
		m_CellDataInspectionEnd.Clear();
		m_CellDataTrackOut.Clear();
	}

	void CurAlarmClear()
	{
		m_nCurAlarmListCnt = 0;
		for (int i=0; i<MAX_ALARM; i++)
		{
			m_tCurAlarmList[i].Clear();
		}
	}

	// �߻����� �ʰ� ����Ǿ� �ִ� ���Ͷ� ������ �˾ƿ´�
	UINT GetAddedInterlockCount()
	{
		int res = 0;

		for(int itor = 0 ; itor<MAX_INTERLOCK_MES ; itor++ )
		{
			if( m_tInterlockMES[itor].m_cInterlockState == INTERLOCK_ADD )
				res++;
		}

		return res;
	}

	// Interlock�� ������ �����´� - LSH1711278
	UINT GetInterlockCount()
	{
		int res = 0;

		for(int itor = 0 ; itor<MAX_INTERLOCK_MES ; itor++ )
		{
			if( m_tInterlockMES[itor].m_cInterlockState == INTERLOCK_HAPPEN )
				res++;
		}

		return res;
	}

	// ��Interlock type�� ������ �����´� - LSH1711278
	UINT GetInterlockCount(UINT Type)
	{
		int res = 0;

		for(int itor = 0 ; itor<MAX_INTERLOCK_MES ; itor++ )
			if( m_tInterlockMES[itor].m_cInterlockState )
				if(m_tInterlockMES[itor].m_cInterlockType == Type )
				{
					if( m_tInterlockMES[itor].m_cInterlockState == INTERLOCK_HAPPEN )
						res++;
				}

		return res;
	}
};

enum E_MESSAGE_CLASS
{
	CLASS_NONE=0,
	CLASS_CONTROL,
	CLASS_MCC,
};

enum E_MESSAGE_TYPE
{
	TYPE_SET_NONE=0,
	//TO DATA PC
	EQUIP_SET_CONTROL,
	EQUIP_SET_DATAPC_VERSION,
	EQUIP_SET_ALARM,
	EQUIP_SET_ERMS,
	EQUIP_SET_CELL_PROCESS_START_LOAD,
	EQUIP_SET_CELL_PROCESS_COMPLETE_UNLOAD,
	EQUIP_SET_CELL_ENDINS,
	EQUIP_SET_EQUIPMENT_STATUS_CHANGE,
	EQUIP_SET_PPID_CREATE,
	EQUIP_SET_PPID_DELETE,
	EQUIP_SET_PPID_MODIFY,
	EQUIP_SET_PPID_CHANGE,
	EQUIP_SET_PARAMETER_CHANGE,
	EQUIP_SET_PPID_OPERATION,
	EQUIP_SET_MATERIAL_CHANGE,
	EQUIP_SET_OPCALL_EVENT,
	EQUIP_SET_INTERLOCK_EVENT,
	EQUIP_SET_CELL_INFORMATION_REQUEST,
	EQUIP_SET_CODE_INFORMATION_REQUEST,
	EQUIP_SET_CONTROL_STATE_CHANGE_ONESELF_ACK,
	EQUIP_SET_CELL_LOADER_IN,
	EQUIP_SET_CELL_TESTER_OUT,
	EQUIP_SET_CELL_UNLOADER_OUT,

	//USER LOGIN & OUT
	EQUIP_SET_CTLU_SER_LOGIN,
	EQUIP_SET_INS1_USER_LOGIN,
	EQUIP_SET_INS2_USER_LOGIN,
	EQUIP_SET_CTL_USER_LOGOUT,
	EQUIP_SET_INS1_USER_LOGOUT,
	EQUIP_SET_INS2_USER_LOGOUT,

	//UV MCR READING
	EQUIP_SET_UV_READING_MCR,	//SONIC - 2013.07.22.
	//VISION ALIGN
	EQUIP_SET_ALIGN_MARK_FIND,	//SONIC - 2013.07.22.
	EQUIP_SET_LINE_NO,
	EQUIP_SET_CELL_EXIST,
	
	//LossCode
	EQUIP_SET_LOSS_CODE,		//20141223 sms

	//TO AFT
	HOST_SET_CONTROL,
	HOST_SET_MODULE,
	HOST_SET_PROC,
	HOST_SET_DATAPC_VERSION,
	HOST_SET_CELL_INFORMATION,		//SONIC - 2014.02.09. Add, Cell Tracking Information

	HOST_SET_OPCALL,
	HOST_SET_INTERLOCK,
	HOST_SET_TERMINAL,
	HOST_SET_CONTROL_STATE_CHANGE,
	HOST_SET_LINE_NO,

	EQUIP_SET_USER_LOGIN_REQUEST,   // 58, �۾����� Login ������ ���� ������ ��û�Ѵ�.
	EQUIP_SET_USER_LOGOUT_REQUEST,	// 59, �۾����� Login ������ ���� ������ ��û�Ѵ�.
	HOST_SET_USER_LOGOUT_REQUEST,   // 60, �������� ���� ����� �۾��ڸ� ������ Logout ��Ű��� ���� �޼���
	
	// �߰��� Message
	EQUIP_SET_CELL_LOT_INFORMATION_REQUEST,		// 2016-01-04, jhLee, S3F109, POCH ó���� ���� Cell Lot Information �̽� �߻�
	HOST_SET_CELL_LOT_INFORMATION,				// 2016-01-04, jhLee, S3F109, POCH ó���� ���� Cell Lot Information ����

	// 2016-01-20, ���� Cell Data�� ������ Clear �ϴ� ��û
	EQUIP_SET_CELL_DATA_CLEAR_REQUEST,			// ���� Cell Data�� ������ Clear

	HOST_SET_CELL_JOB_PROCESS_STATE,				// 2017-06-23, HWARANG Cell Track In �� JobProcess State �޴´�.

	HOST_SET_PPID_CREATE,							// �����κ��� �ű� ������ ���� [9/27/2017 OSC]

	// EQIUPMENT FUNCTION LIST [11/14/2017 OSC]
	HOST_SET_EQUIP_FUNCTION_CHANGE_REQ,			// �������� Function ���� ��û
	EQUIP_SET_EQUIP_FUNCTION_CHANGE_REQ_ACK,	// ���� Function ���� ��û ����/�Ұ��� ���� ȸ��
	EQUIP_SET_EQUIP_FUNCTION_REPORT,			// CIM�� Function List ����ȭ
	EQUIP_SET_EQUIP_FUNCTION_CHANGE,			// ���񿡼� Function ���� ����

	// INTERLOCK (���� HOST_SET_INTERLOCK�� �Ⱦ��� [11/16/2017 OSC]
	HOST_SET_INTERLOCK_REQ,				// ���Ͷ� �߻� �������� ���� Ȯ�� ��û
	EQUIP_SET_INTERLOCK_REQ_ACK,		// ���Ͷ� �߻� ���ɿ��� ȸ��

	// Unit Status Change [11/28/2017 OSC]
	EQUIP_SET_UNIT_STATUS_REPORT,		// CIM ���α׷��� ���� ����ȭ
	EQUIP_SET_UNIT_STATUS_CHANGE,		// UNIT ���� ����

	EQUIP_SET_UNIT_INTERLOCK_COMFIRM,	
	EQUIP_SET_UNIT_OPCALL_COMFIRM	

};

//2014-11-03, jhLee, Data-PC���� Structre size�� �����ϱ� ���� Struct Pack align�� 8 Bytes�� ������ �� ���� �����ش�.
#pragma pack(push, 1)

struct tag_Message
{
	short sMsgClass;	
	short sMsgType;
	short sEqst;	
	short sUnit;
	short sCellPos;
	short sTactPoint;
	char cMsgData[MAX_MES_COUNT][MAX_MES_SIZE];
	char cLongMsgData[MAX_LONG_MSG_COUNT][MAX_LONG_MSG_SIZE];
	//
	// 2016-08-26, jhLee, Cell ID�� 150�ڱ��� �þ���Ƿ� ������ �����ͷ� ó���Ѵ�.
	char cCellID[MAX_CELL_ID+1];

	tag_Message Clear()
	{
		sMsgClass = CLASS_NONE;
		sMsgType = TYPE_SET_NONE;
		sEqst = E_EQST_MATCH_NONE;
		sUnit = 0;
		sCellPos = 0;
		sTactPoint = 0;
		memset(cMsgData, 0x00, MAX_MES_COUNT*MAX_MES_SIZE*sizeof(char));
		memset(cLongMsgData, 0x00, MAX_LONG_MSG_COUNT*MAX_LONG_MSG_SIZE*sizeof(char));
		memset(cCellID, 0x00, sizeof(cCellID));
	}
};

#pragma pack(pop)


typedef CMap<CString, LPCTSTR, CString, LPCTSTR>	MAP_STR_STR;
typedef CMap<CString, LPCTSTR, int, int>			MAP_STR_INT;
typedef CList<CString, CString&>					LIST_STR;
class CDefNameMap
{
public:

protected:

public:
	static CString GetMesName(char* pAvtName);
	static CString GetMesCode(char* pAvtName);
	static CString GetMesCode(int nInx);
	static int GetFormatInx(char* pAvtName);

	static BOOL LoadAvtToMesNameMap(char* pFileName);
	static BOOL LoadMesToCodeMap(char* pFileName);
	static BOOL LoadManualFormatNameMap(char* pFileName);

	static const int STR_SIZE = 256;
	static const int MAP_LINE_CNT = SVID_MAX;
	//20141222 sms fdc ���� enum { MAP_KEY_COL=0, MAP_SUB1_COL, MAP_SUB2_COL, MAP_SUB3_COL, MAP_SUB4_COL, MAP_SUB5_COL, MAP_SUB6_COL, MAP_SUB7_COL, MAP_MAX_COL, };
	//kjpark 20170929 FDC ������, ������, ���� ����
	enum { MAP_KEY_COL=0, MAP_SUB1_COL, MAP_SUB2_COL, MAP_SUB3_COL, MAP_SUB4_COL, MAP_SUB5_COL, MAP_SUB6_COL, MAP_SUB7_COL, MAP_SUB8_COL, MAP_SUB9_COL, MAP_SUB10_COL, MAP_MAX_COL, };	//20141222 sms
	static int LoadFdcInfoMap(const int nArrayNo, char* pArray0, char* pFileName);
	static char cFdcInfoMap[MAP_LINE_CNT][MAP_MAX_COL][STR_SIZE];

// 	static int LoadTactInfoMap(const int nArrayNo, char* pArray0, char* pFileName);
// 	static char cTactInfoMap[MAP_LINE_CNT][MAP_MAX_COL][STR_SIZE];

};

class EQUIP_FUNCTION
{
public:
	EQUIP_FUNCTION(void) { Clear(); }
	~EQUIP_FUNCTION(void) {}

	BOOL USE;			// �ش� ���񿡼� ��� ������ ����� �ƴϸ� FALSE
	int EFID;			// ID
	CString EFNAME;		// Name
	CString EFST_TYPE1;	// EFST�� ���� �� �ִ� �� 1
	CString EFST_TYPE2;	// EFST�� ���� �� �ִ� �� 2
	CString EFST_TYPE3;	// EFST�� ���� �� �ִ� �� 3
	CString EFST_TYPE4;	// EFST�� ���� �� �ִ� �� 4
	CString EFST_Old;	// ���� EFST��
// private:
	CString EFST;		// �� (�� TYPE �� �ϳ��� ���� �� ���� ���� �ź��Ѵ�)

public:
	void Clear();
	BOOL EFST_Set(CString strValue);
	BOOL EFST_Check(CString strValue);	// �����ϰ� Ʋ�� ���̰�, ���Ŀ� ������ TRUE
	CString EFST_Get();
};

//////////////////////////////////////////////////////////////////////////
// Equipment Function List
// CELL TRACKING			: ON/OFF/NOTHING
// TRACKING CONTROL			: TKIN/TKOUT/BOTH/NOTHING
// MATERIAL TRACKING		: ON/OFF/NOTHING
// CELL MCR MODE			: USE/UNUSE/NOTHING
// MATERIAL MCR MODE		: USE/UNUSE/NOTHING
// LOT ASSIGN INFORMATION	: AUTO/MANU/NOTHING
// AGV ACCESS MODE			: AUTO/MANU/NOTHING
// AREA SENSOR MODE			: USE/UNUSE/NOTHING
// SORT MODE				: USE/UNUSE/NOTHING
// INTERLOCK CONTROL		: TRANSFER/LOADING/STEP/OWN
#define EFST_ON			_T("ON")
#define EFST_OFF		_T("OFF")
#define EFST_TKIN		_T("TKIN")		
#define EFST_TKOUT		_T("TKOUT")	
#define EFST_BOTH		_T("BOTH")	
#define EFST_USE		_T("USE")
#define EFST_UNUSE		_T("UNUSE")
#define EFST_AUTO		_T("AUTO")		
#define EFST_MANU		_T("MANU")		
#define EFST_TRANSFER	_T("TRANSFER")
#define EFST_LOADING	_T("LOADING")
#define EFST_STEP		_T("STEP")
#define EFST_OWN		_T("OWN")
#define EFST_NOTHING	_T("NOTHING")
//////////////////////////////////////////////////////////////////////////

#define BYWHO_HOST	_T("HOST")
#define BYWHO_EQP	_T("EQP")
#define BYWHO_OPER	_T("OPER")
#define BYWHO_ETC	_T("ETC")


extern CDataStructure*	g_pData;
extern CString g_strMyMessageType[];
extern CString g_strUnitName[];
