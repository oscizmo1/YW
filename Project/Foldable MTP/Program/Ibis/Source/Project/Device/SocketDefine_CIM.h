#pragma once

//////////////////////////////////////////////////////////////////////////
// DATA PC 통신 전용 Define [11/13/2016 OSC]
//////////////////////////////////////////////////////////////////////////

#define MAX_MES_COUNT					12
#define MAX_MES_SIZE					40
#define MAX_LONG_MSG_COUNT				1
#define MAX_LONG_MSG_SIZE				150

#define MAX_CELL_ID						150		// 2016-08-26, jhLee, 최대 Cell ID 크기는 150자까지 가능하다 (SDC 안도현 사원)


#define CRST_OFF						_T("0")
#define CRST_REMOTE						_T("1")
#define CRST_LOCAL						_T("2")

#define EQST_RUN_IS_CELL				_T("2")
#define EQST_RUN_NOT_CELL				_T("1")

#define ALST_SET						_T("1")
#define ALST_CLEAR                      _T("2")

#define ALCD_LIGHT						_T("1")
#define ALCD_SERIOUS					_T("2")

#define CIM_JUDGE_GOOD					_T("G")
#define CIM_JUDGE_RETEST				_T("R")
#define CIM_JUDGE_LOSS					_T("L")
#define CIM_JUDGE_OUT					_T("O")

#define DESCRIPTION_ABORT				_T("MANUAL_TRACKOUT")

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
	EQUIP_SET_INS1_USER_LOGIN,		// D존 검사자
	EQUIP_SET_INS2_USER_LOGIN,		// B존 작업자
	EQUIP_SET_CTL_USER_LOGOUT,
	EQUIP_SET_INS1_USER_LOGOUT,		// D존 검사자
	EQUIP_SET_INS2_USER_LOGOUT,		// B존 작업자

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

	EQUIP_SET_USER_LOGIN_REQUEST,   // 58, 작업자의 Login 검증을 상위 서버에 요청한다.
	EQUIP_SET_USER_LOGOUT_REQUEST,	// 59, 작업자의 Login 검증을 상위 서버에 요청한다.
	HOST_SET_USER_LOGOUT_REQUEST,   // 60, 서버에서 설비를 세우고 작업자를 강제로 Logout 시키라는 지령 메세지

	// 추가된 Message
	EQUIP_SET_CELL_LOT_INFORMATION_REQUEST,		// 2016-01-04, jhLee, S3F109, POCH 처리를 위한 Cell Lot Information 이슈 발생
	HOST_SET_CELL_LOT_INFORMATION,				// 2016-01-04, jhLee, S3F109, POCH 처리를 위한 Cell Lot Information 응답

	// 2016-01-20, 지정 Cell Data의 내용을 Clear 하는 요청
	EQUIP_SET_CELL_DATA_CLEAR_REQUEST,			// 지정 Cell Data의 내용을 Clear

	// 2017-06-23, HWARANG Cell Track In 시 JobProcess State 받는다.
	//kjpark 20170907 Tracking CellInfomation, JobProcess 추가
	HOST_SET_CELL_JOB_PROCESS_STATE,				

	// 상위로부터 신규 레시피 생성 [9/27/2017 OSC]
	HOST_SET_PPID_CREATE	,

	// EQIUPMENT FUNCTION LIST [11/14/2017 OSC]
	HOST_SET_EQUIP_FUNCTION_CHANGE_REQ,			// 상위에서 Function 변경 요청
	EQUIP_SET_EQUIP_FUNCTION_CHANGE_REQ_ACK,	// 상위 Function 변경 요청 가능/불가능 여부 회신
	EQUIP_SET_EQUIP_FUNCTION_REPORT,			// CIM과 Function List 동기화
	EQUIP_SET_EQUIP_FUNCTION_CHANGE,			// 설비에서 Function 변경 보고

	// INTERLOCK (기존 HOST_SET_INTERLOCK은 안쓴다 [11/16/2017 OSC]
	HOST_SET_INTERLOCK_REQ,				// 인터락 발생 가능한지 설비에 확인 요청
	EQUIP_SET_INTERLOCK_REQ_ACK,			// 인터락 발생 가능여부 회신

	// Unit Status Change [11/28/2017 OSC]
	EQUIP_SET_UNIT_STATUS_REPORT,		// CIM 프로그램과 상태 동기화
	EQUIP_SET_UNIT_STATUS_CHANGE,		// UNIT 상태 변경

	EQUIP_SET_UNIT_INTERLOCK_COMFIRM


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

//2014-11-03, jhLee, Data-PC와의 Structre size를 통일하기 위해 Struct Pack align을 8 Bytes로 변경한 뒤 원복 시켜준다.
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
	// 2016-08-26, jhLee, Cell ID가 150자까지 늘어났으므로 별도의 데이터로 처리한다.
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

class tag_Message_Unicode
{
public:
	tag_Message_Unicode(void) { Clear(); }
	~tag_Message_Unicode(void) {}

	short sMsgClass;	
	short sMsgType;
	short sEqst;	
	short sUnit;
	short sCellPos;
	short sTactPoint;
	CString strMsgData[MAX_MES_COUNT];
	CString strLongMsgData[MAX_LONG_MSG_COUNT];
	CString strCellID;

	void Clear();
	void operator=(const tag_Message &src);
	void Convert_tag_Message(tag_Message *pMsg);

};


class EQUIP_FUNCTION
{
public:
	EQUIP_FUNCTION(void) { Clear(); }
	~EQUIP_FUNCTION(void) {}

	BOOL USE;			// 해당 설비에서 사용 가능한 기능이 아니면 FALSE
	BOOL bChangeFlag;	// 데이터가 변경된적이 있다면 TRUE
	int EFID;			// ID
	CString EFNAME;		// Name
	CString EFST_TYPE1;	// EFST에 들어올 수 있는 값 1
	CString EFST_TYPE2;	// EFST에 들어올 수 있는 값 2
	CString EFST_TYPE3;	// EFST에 들어올 수 있는 값 3
	CString EFST_TYPE4;	// EFST에 들어올 수 있는 값 4
	CString EFST_Old;	// 이전 EFST값
private:
	CString EFST;		// 값 (위 TYPE 중 하나만 들어가고 그 외의 값은 거부한다)

public:
	void Clear();
	BOOL EFST_Set(CString strValue);
	BOOL EFST_Check(CString strValue);	// 기존하고 틀린 값이고, 형식에 맞으면 TRUE
	CString EFST_Get();

	// 변경 상태를 변경 및 반환한다 - LSH171130
	void SetChangeState(BOOL bFlag) {bChangeFlag = bFlag;};
	BOOL GetChangeState() {return bChangeFlag;};
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
enum EFID_INDEX
{
	EFID_CELL_TRACKING = 0,
	EFID_TRACKING_CONTROL,
	EFID_MATERIAL_TRACKING,
	EFID_CELL_MCR_MODE,
	EFID_MATERIAL_MCR_MODE,
	EFID_LOT_ASSIGN_INFORMATION,
	EFID_AGV_ACCESS_MODE,
	EFID_AREA_SENSOR_MODE,
	EFID_SORT_MODE,
	EFID_INTERLOCK_CONTROL,
	EFID_MAX
};

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
#define EFST_NOTHING	_T("NOTHING")	// 이건 해당 기능이 존재하지 않을 때 설정
//////////////////////////////////////////////////////////////////////////


#define BYWHO_HOST	_T("HOST")
#define BYWHO_EQP	_T("EQP")
#define BYWHO_OPER	_T("OPER")
#define BYWHO_ETC	_T("ETC")