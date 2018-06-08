
#pragma once

//////////////////////////////////////////////////////////////////////////
// 이곳 저곳 범용적으로 쓸 Define 모아두는 곳
//////////////////////////////////////////////////////////////////////////
//Define 부분
//////////////////////////////////////////////////////////////////////////
//kjpark 20161025 MCR 구현
#define	WM_SERIAL								WM_USER+0x9000
#define	WM_UPDATE_MCRCONNECT					WM_USER+0x9001

//kjpark 20170831 MCR 구현 채널 별 구현 완료
#define	WM_UPDATE_MCRDATA_CH1						WM_USER+0x9002
#define	WM_UPDATE_MCRLIVE_CH1						WM_USER+0x9003
#define	WM_UPDATE_MCRDATA_CH2						WM_USER+0x9004
#define	WM_UPDATE_MCRLIVE_CH2						WM_USER+0x9005


#define TRAY_PITCH_MAX			30	// Tray 최대 쌓을 수 있는 수량
#define OUT_LIFTER_PITCH_UP_OFFSET	5.
#define LIFTER_ALIGN_PITCH		-7.
#define LIFTER_DIVISION_PITCH	-30.
#define CHANNEL_PITCH	176.

#define MAX_CHAR_LENGTH 128
#define DEFECT_CHANNEL_COUNT 4
#define STX		0x02
#define ETX		0x03
#define ENQ		0x05
#define ACK		0x06
#define CR		0x0D
#define LF		0x0A
#define DEL		0x10
#define CAN		0x18
#define ESC		0x1B
#define NAK		0x15

//////////////////////////////////////////////////////////////////////////
//String 부분
//////////////////////////////////////////////////////////////////////////
#define TEXT_DEFECT_NG				_T(" NG")	// 공백 빼면 안됨
#define TEXT_DEFECT_TIME_OUT		_T(" TIMEOUT")		// 공백 빼면 안됨
#define TEXT_CONTACT_CURRENT_NG		_T("CONTACT_CURRENT_NG")
#define TEXT_CONTACT_NG				_T("CONTACT_NG")
#define TEXT_ID_CHECK_NG			_T("ID_CHECK_NG")
#define TEXT_MTP_Time_Over			_T("Mtp Time Over")	// ID Check Time Out시
#define TEXT_OPT_REG_NG				_T("OPT_REG_NG")
#define TEXT_Non_Display_AFT_ELVSS	_T("Non_Display_AFT_ELVSS")	// OTP REG Time Out시
#define TEXT_FAIL					_T("FAIL")
//kjpark 20180113 Cell infomation NG 시 Job Process 로 Last Result 남는 버그 수정
//kjpark 20180123 cellionfomation, jobprocess 페일시 라스트 리절트 정리
#define TEXT_CELL_INFO_FAIL_GOOD	 _T("CELL INFO FAIL_GOOD")
#define TEXT_CELL_INFO_FAIL_NG		 _T("CELL INFO FAIL_NG")
#define TEXT_CELL_INFO_TIMEOUT_GOOD	_T("CELL INFO TIMEOUT_GOOD")
#define TEXT_CELL_INFO_TIMEOUT_NG	_T("CELL INFO TIMEOUT_NG")
#define TEXT_VALIDATION_FAIL_GOOD	_T("VALIDATION FAIL_GOOD")	
#define TEXT_VALIDATION_FAIL_NG		_T("VALIDATION FAIL_NG")
#define TEXT_VALIDATION_TIMEOUT_GOOD _T("VALIDATION TIMEOUT_GOOD")	
#define TEXT_VALIDATION_TIMEOUT_NG  _T("VALIDATION TIMEOUT_NG")	
#define TEXT_MCR_READING_FAIL_GOOD	_T("MCR READING FAIL_GOOD")	// MCR 못읽었지만 양품일 경우 판정값
#define TEXT_MCR_READING_FAIL_NG	_T("MCR READING FAIL_NG")	// MCR 못읽었지만 불량일 경우 판정값
#define TEXT_UNDEFINED				_T("UNDEFINED")
#define TEXT_UNDEFINED_CODE			_T("USO56")
#define TEXT_TSP_IC					_T("TSP IC")
#define TEXT_HLPM_CURRENT_NG		_T("HLPM_CURRENT_NG")
#define TEXT_OPTION_CHECK1_NG		_T("OPTION_CHECK1_NG")
#define TEXT_OPTION_CHECK2_NG		_T("OPTION_CHECK2_NG")

#define TEXT_TSP_TIME_OUT					_T("TSP_TIME_OUT")
#define TEXT_ID_CHECK_TIME_OUT				_T("ID_CHECK_TIME_OUT")
#define TEXT_EVT_VERSION_CHECK_TIME_OUT		_T("EVT_VERSION_CHECK_TIME_OUT")
#define TEXT_OTPREG_CHECK_TIME_OUT			_T("OTPREG_CHECK_TIME_OUT")
#define TEXT_FORCE_START1_BEF_TIME_OUT		_T("FORCE_START1_BEF_TIME_OUT")
#define TEXT_FORCE_START1_AFT_TIME_OUT		_T("FORCE_START1_AFT_TIME_OUT")
#define TEXT_FORCE_START2_BEF_TIME_OUT		_T("FORCE_START2_BEF_TIME_OUT")
#define TEXT_FORCE_START2_AFT_TIME_OUT		_T("FORCE_START2_AFT_TIME_OUT")
#define TEXT_FORCE_START3_BEF_TIME_OUT		_T("FORCE_START3_BEF_TIME_OUT")
#define TEXT_FORCE_START3_AFT_TIME_OUT		_T("FORCE_START3_AFT_TIME_OUT")
#define TEXT_OPTION_CHECK1_TIME_OUT			_T("OPTION_CHECK1_TIME_OUT")
#define TEXT_OPTION_CHECK2_TIME_OUT			_T("OPTION_CHECK2_TIME_OUT")
#define TEXT_HLPM_CURRENT_TIME_OUT			_T("HLPM_CURRENT_TIME_OUT")
#define TEXT_I_BAT_OVER						_T("i_bat OVER")
//kjpark 20171010 TMD March 구현
#define TEXT_CELL_MIXING					_T("CELL MIXING")	//20171001 BKH, 혼류 발생시 bin2처리 하기 위해 추가
#define TEXT_WHITE_CURRENT_TIME_OUT			_T("WHITE_CURRENT_TIME_OUT")
#define TEXT_MTP_WRITE_TIME_OUT				_T("MTP_WRITE_TIME_OUT")
#define TEXT_MTP_VERIFY_TIME_OUT			_T("MTP_VERIFYE_TIME_OUT")

#define GOOD				_T("GOOD")
#define REJECT				_T("REJECT")
#define OK					_T("OK")
#define NG					_T("NG")
#define FAIL				_T("FAIL")
#define EXT				_T("EXT")

#define SKIP				_T("SKIP")
#define TIMEOUT_NG	  _T("TIMEOUT")

#define AB_RULE_A		_T("A")
#define AB_RULE_AB		_T("AB")
#define AB_RULE_GOOD	_T("")

#define TEXT_0		_T("0")
//kjpark 20170831 MCR 구현 채널 별 구현 완료
#define MCRPath_CH1 _T("D:/YWDSP/Image/MCR_Image_Shuttle_CH1.jpg")
#define MCRPath_CH3 _T("D:/YWDSP/Image/MCR_Image_Shuttle_CH3.jpg")
//////////////////////////////////////////////////////////////////////////
//enum 부분
//////////////////////////////////////////////////////////////////////////
enum JIG_ID
{
	JIG_ID_A,
	JIG_ID_B,
//	JIG_ID_C,
// 	JIG_ID_D,
	JIG_ID_MAX
};

enum JIG_CH
{
	JIG_CH_1,
//	JIG_CH_2,
	JIG_CH_MAX
};

enum ZONE_ID
{
	ZONE_ID_A,
	ZONE_ID_MOVE_A,
	ZONE_ID_B,
	ZONE_ID_MOVE_B,
	ZONE_ID_C,
	ZONE_ID_MOVE_C,
	ZONE_ID_MAX
};

enum LANGUAGE
{
	LAN_KOR = 0,
	LAN_ENG,
	LAN_VNM,
	LAN_MAX
};

enum VAC_STATE
{
	VAC_OFF = 0,
	VAC_ON = 1,
	VAC_STATE_MAX
};

enum BLOW_STATE
{
	BLOW_OFF = 0,
	BLOW_ON = 1,
	BLOW_STATE_MAX
};

enum TILT_STATE
{
	TILT_DOWN = 0,
	TILT_UP = 1,
	TILT_STATE_MAX
};

enum SENSOR_STATE
{
	SENSOR_OFF = 0,
	SENSOR_ON = 1,
	SENSOR_STATE_MAX
};

enum CYL_STATE
{
	CYL_BWD = 0,
	CYL_FWD = 1,
	CYL_UP = 0,
	CYL_DOWN = 1,
	CYL_STATE_MAX
};

// enum INSP_STATE
// {
// 	INSP_NONE = 0,
// 	INSP_ING,
// 	INSP_END,
// 	INSP_STATE_MAX
// };

enum CLASS_CELL
{
	NONE_CELL = 0,
	GOOD_CELL,
	REJECT_CELL,
	SKIP_CELL,	
	CLASS_CELL_MAX
};

//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
enum MCR_READ_SELECT
{
	MCR_NONE = 0,
	MCR_AUTO,
	MCR_MANUAL,
	MCR_READ_FAIL
};

//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
enum CLASS_RESULT
{
	RESULT_NONE = -1,	// 검사하기 전 상태 [11/5/2016 OSC]
	RESULT_NG = 0,
	RESULT_GOOD
};

//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
enum CLASS_DECISION
{
	DECISION_FAIL = 0,
	DECISION_PASS		
};

enum ONOFF
{
	OFF = 0,
	ON,
	ONOFFMAX,
};

enum TMD_INFO_STATUS
{
	TMD_INFO_NOT_RECEIVE = 0,
	TMD_INFO_RECEIVE,
	TMD_INFO_NG,
	TMD_INFO_DOWN,
	TMD_INFO_MAX,
};

enum SERIAL_MODE
{
	//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
	SERIAL_VACUUM_MAIN = 0,
	SERIAL_PC_BOX_TEMP,
	//kjpark 20170905 Temp 통신 연결 변경 232->485
	SERIAL_GMS,	
	SERIAL_LABELPRINT,	
	SERIAL_MAX
};

//kjpark 20161018 A Zone Cell exist Check
enum AZONE_CELL_EXIST
{
	//kjpark 20161105 Sequence 동작 수정
	CELL_IDLE,
	CELL_NONE,
	CELL_FULL,	
};

enum CELL_POS
{
	CELL_POS_SHUTTLE1_CH1 = 0,
//	CELL_POS_SHUTTLE1_CH2,
	CELL_POS_SHUTTLE2_CH1,
//	CELL_POS_SHUTTLE2_CH2,
//	CELL_POS_SHUTTLE3_CH1,
//	CELL_POS_SHUTTLE3_CH2,

	CELL_POS_MAX
};

//kjpark 20161019 MainFrame timer 추가 - 인터락, 타워램프 컨트롤
enum TMID
{
	TMID_RECONNECT,
	TMID_DOOR_INTERLOCK, 
	TMID_FLICK_LAMP,
	TMID_LOSS_TIME,
	TMID_LOGOUT_CHECK,
	TMID_PDT_AB_RULE_CHECK
};

//kjpark 20161019 MainFrame timer 추가 - 인터락, 타워램프 컨트롤
enum LAMP_STATE
{
	LAMP_ON = 0,
	LAMP_FLICKER,
	LAMP_OFF,
	LAMP_MAX,
};

//kjpark 20161022 설정->램프 설정 구현
enum STATUS_LAMP
{
	STATUS_LAMP_RED,
	STATUS_LAMP_YELLOW,
	STATUS_LAMP_GREEN,	
};

enum CHECK_OPTION
{
	CHECK_AND,
	CHECK_OR
};

enum LOSSTIME_ITEM
{
	ITEM_EQUIPID = 0	,	// 설비 호기 표기
	ITEM_MODEL			,	// 현재 진행중인 PANEL MODEL 명
	ITEM_ID				,	// FIRST CLASS가 MD가 아닐경우 입력(STOP BUTTON을 누르면 사번을 입력하는 창 생성), 삼성직원일 경우, 사번입력, 협력사 직원일 경우 이름입력
	ITEM_ALARM_TYPE		,	// 중알람일 경우 A, 경알람일 경우 B ( FIRST CLASS가 MD일 경우에만 표기, 이외의 경우는 '-' 표기)
	ITEM_START_TIME		,	// 2013-03-04 12:45:00:000 ( EVENT 발생시각 - STOP BUTTON을 눌렀거나, ERROR가 발생했을때)
	ITEM_RESET_TIME		,	// 2013-03-04 12:27:00:000  (FIRST CLASS가 MD일 경우만 생성, ERROR MESSAGE를 RESET했을때)
	ITEM_END_TIME		,	// 2013-03-04 12:27:00:000  (조치가 완료되고 다시 START BUTTON을 누른시각)
	ITEM_WAIT_TIME		,	// RESET TIME - START TIME, 알람이 울린 후, 고장 조치 시작 까지의 시간, 초 단위로 표기
	ITEM_REPAIR_TIME	,	// END TIME - RESET TIME, 실제 조치가 일어난 시간, 초단위로 표기
	ITEM_DURATION		,	// 실제설비가 멈춘 시간 (END TIME - START TIME)
	ITEM_FIRST_CLASS	,	// 대분류
	ITEM_SECOND_CLASS	,	// 중분류	
	ITEM_THIRD_CLASS	,	// 소분류
	
	ITEM_TP_LOSS_NAME	,	// TP System
	ITEM_TP_LOSS_CODE	,
	ITEM_TP_ALARM_CODE	,	// 각 고장별 알람 코드 표기
	//ITEM_TP_REPAIR_CODE	,
	ITEM_REPAIR_CODE	,	// 조치 내역 표기(First Class가 MD, BM인 항목에만 표기)	

	ITEM_MAX
};

//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
enum JOB_PROCESS_STATE
{
	JOB_DEFAULT = 0,
	JOB_INVALID, 
	JOB_START,
	JOB_SKIP,
	JOB_MAX
};

//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
enum CIM_MESSAGE_STATE
{
	CIM_MESSAGE_INTERLOCK = 0,
	CIM_MESSAGE_OPCALL, 
	CIM_MESSAGE_TEMINAL,
	CIM_MESSAGE_JOBPROCESS,
	CIM_MESSAGE_MAX
};

// STO STATE [9/19/2017 OSC]
enum STO_STATE
{
	STO_READY = 0,	// STO 걸리지 않은 상태
	STO_WARNING,	// STO Input 신호 1개 불량인 상태 (동작은 가능하나 교체 필요)
	STO_ALARM		// STO 걸려서 이동 불가한 상태
};

//////////////////////////////////////////////////////////////////////////
//Struct 부분
//////////////////////////////////////////////////////////////////////////
typedef struct _MSGEvent {

	_MSGEvent ()
	{
		message = 0;
		nParam1 = 0;
		nParam2 = 0;
		nParam3 = 0;
		pParam	= NULL;
		pDest	= NULL;
		pParent = NULL;
	}
	UINT	message;
	int		nParam1;
	int		nParam2;
	int		nParam3;
	LPARAM	pParam;
	LPARAM	pDest;	
	LPARAM	pParent; 
} MSGEvent;


struct CONST_TABLE_POS
{
	CString strName;
	enum
	{
		POS_1=0,
		POS_2,
		POS_3,
		POS_4,
		POS_MAX,
	};
};

//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
struct structMCR_READ
{
	CString		strMCRRead_Description;	
};


//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
const structMCR_READ MCRReadDefect[] =
{
	_T("NONE")			,
	_T("AUTO")		,
	_T("MANUAL"),
	_T("READ_FAIL")			,
};

//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
struct ClassResult
{
	CString		strResultClass;
	
};

//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
const ClassResult ClassResultStringArr[] =
{	
	_T("NG")		,
	_T("GOOD")			,
};

//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
struct ClassDecision
{
	CString		strResultClass;

};

//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
const ClassDecision ClassDecisionStringArr[] =
{	
	_T("FAIL")		,
	_T("PASS")			,
};




//kjpark 20161022 설정->램프 설정 구현
struct LAMP_SETTING_LIST_BUTTON
{
	CString strColor;
	UINT nResourceID;
// 	BOOL bCheck;
	LAMP_STATE lamp_state;
};

//kjpark 20161022 설정->램프 설정 구현
struct LAMP_SETTING_LIST
{
	CString strList;
	BOOL bSelect;
	UINT nResourceID;
	LAMP_SETTING_LIST_BUTTON Red;
	LAMP_SETTING_LIST_BUTTON Yellow;
	LAMP_SETTING_LIST_BUTTON Green;
};


enum LOG_INDEX
{
	LOG_SYSTEM,					// Program 내부 구동 관련 로그
	LOG_SEQUENCE,				// Seq 변경 현황 기록 로그
	LOG_SEQUENCE_RESTART,
	LOG_SEQUENCE_AZONE,
	LOG_SEQUENCE_SHUTTLE,
	LOG_SEQUENCE_BZONE,
	LOG_SEQUENCE_PDT_IF,
	LOG_SEQUENCE_CZONE,


	LOG_SOCKET,					// Socket 통신 관련 기록 로그
	LOG_CIM,					// CIM 통신 관련 기록 로그
	LOG_SOCKET_PG,				// 신호기 Packet 로그
	LOG_OP2PG_PACKET,			// 신호기 packet 처리한 결과로그
	LOG_SOCKET_ERR,	

	LOG_SERIAL,					// Serial 통신 관련 기록 로그
	LOG_SERIAL_GMS,				// GMS Serial 통신 관련 기록 로그
	LOG_OPERATION,				// 유저 버튼 조작 기록 로그
	LOG_ERROR,					// 에러명 기록 로그
	LOG_ERROR_TEMP,				// Alarm Mute 에러들 임시 저장 기록 로그 20161109 BKH
	LOG_MOTOR_ERROR,
	LOG_MP2100,		
	LOG_NMC,		
	LOG_FDC,		

	LOG_JUDGE,					// Cell 판정 현황

	LOG_TRACKING,				// Cell Track In / Out 현황
	LOG_MCR,					// MCR mapping 현황
	LOG_TACT_TIME,				// Tact Time 현황 로그

	LOG_RETRY,					// AA, AB, AC Retry 현황 기록 로그

	LOG_SPECIAL_PROCESS,		// 불량좌표 등 특수한 검사공정관련 로그 
	//kjpark 20180114 NG 판정 통계 UI 추가
	LOG_NG_RESULT_STATISTICS,
	LOG_MAX					// Log 폴더 총 갯수
};

#define OPERATION_TO_DEFECT 0x55
#define DEFECT_TO_OPERATION 0x33

enum SUB_COMMAND
{
	SUB_COMAND_NOT_DEFECT = 0x80,
	SUB_COMAND_GOOD = 0x40,
	SUB_COMAND_NG = 0x20,
	SUB_COMAND_INFOMATION = 0x10,
	SUB_COMAND_LOGIN = 0x02,
	SUB_COMAND_LOGIN_RESULT = 0x01,
};

//20170331 kjpark 택타임 UI 변경 및 시간별 택타임 표기
enum TimeHour
{
	DAY_7 = 7,
	DAY_8,
	DAY_9,
	DAY_10,
	DAY_11,
	DAY_12,
	DAY_13,
	DAY_14,
	SW_15,
	SW_16,
	SW_17,
	SW_18,
	SW_19,
	SW_20,
	SW_21,
	SW_22,
	GY_23,
	GY_0 = 0,
	GY_1,
	GY_2,
	GY_3,
	GY_4,
	GY_5,
	GY_6
};

//kjpark 20180114 NG 판정 통계 UI 추가
enum WorkPart
{
	DAY = 0,
	SW,
	GY
};

//kjaprk 20170904 통계 UI 12시간 타임으로 변경
enum WorkPartSub
{
	DAY_TIME,
	NIGHT_TIME,
	PART_TIME_MAX
};

//kjpark 20180121 NG 판정 통계 UI 추가 Azone, MTPWrite, MTPVerify 별로 정리
enum DEFECT_TYPE
{
	AZONE_DEFECT_NG,
	MTP_WRITE_DEFECT_NG,
	MTP_VIRIFY_DEFECT_NG,
	//kjpark 20180129 MCR reading fail 시 초기값 미지정으로 인한 버그 수정
	DEFECT_DEFAULT
};

struct B_ZONE_DEFECT_ELEMENT
{
	CLASS_CELL Result;
	UINT nResourceID;	
	CString strCaption;
};

struct B_ZONE_DEFECT
{
	B_ZONE_DEFECT_ELEMENT Final;
	B_ZONE_DEFECT_ELEMENT Cellloading;
	B_ZONE_DEFECT_ELEMENT CellID;
	CString FinalDefectName[LAN_MAX];
};

//kjpark 20161121 MES code list read
struct DEFECT_MES_CODE
{
	CString strPopup;
	CString strPopup_KOR;
	CString strPopup_ENG;
	CString strMES_CODE;
	BOOL bRetryAble;
};


// CIM에서 내려받는 불량좌표 [5/2/2017 OSC]
#define MAX_DEFECT_POINT	5
struct CIM_DEFECT_POINT
{
	int nX, nY;
	CString strDefectName;
	CString strPatternName;

	CIM_DEFECT_POINT()
	{
		nX = nY = 0;
		strDefectName.Empty();
		strPatternName.Empty();
	}
};

// 파일에서 불러오는 패턴 정보
struct DEFECT_PATTERN
{
	CString strDefectName;
	CString strPatternName;
	int nPatternNo;
	COLORREF colorLine;
	int nThickness;
	int nSize;

	DEFECT_PATTERN()
	{
		strDefectName.Empty();
		strPatternName.Empty();
		nPatternNo = 0;
		colorLine = RGB(0,0,0);
		nThickness = 5;
		nSize = 100;
	}
};

struct SQUARE_POINT
{
	int		nThickness;	// 두께
	int		nSize;		// 크기
	COLORREF colorLine;	// 선 색상
	int		nX, nY;		// 중심위치

	SQUARE_POINT()
	{
		nThickness = 5;
		nSize = 100;
		colorLine = RGB(0,0,0);
		nX = nY = 0;
	}
};

// 최종적으로 한 화면에 표시할 정보들
struct SQUARE_PATTERN_INFO
{
	int nPatternNo;
	CList <SQUARE_POINT, SQUARE_POINT> listSquarePoint;

	SQUARE_PATTERN_INFO()
	{
		nPatternNo = 0;
		listSquarePoint.RemoveAll();
	}
};
