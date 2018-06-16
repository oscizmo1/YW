#pragma once


// 사용자 정의 Message 상수 선언

#define _GAUS_LIB									// 소스 자체를 사용한다.

// 폼/다이얼로그 화면 전환에 필요한 공통 사용 일련 번호 선언
enum {

	// CFormView, 서브 화면 폼 
	eFORM_Default			= 0,			// 프로그램 시작시 기본적으로 가지고 있는  CFormView
	eFORM_CIM,								// Main 화면, CIM 통신 화면
	eFORM_FDC,								// FDC 항목 보기
	eFORM_RMS,								// RMS 항목 보기
	eFORM_ECM,								// CEM 항목 보기
	eFORM_SETTING,							// 설정 내용 보기

	// 통신 상태 변경
	eIDX_HostConnect		= 0,			// HOST와의 연결
	eIDX_HostOnline,						// SELECT / ONLINE 상태 변경
	eIDX_HostRecevie,						// 메세지 수신 처리
	
};

// 사용자 정의 Messge 정의
#define GxMSG_GUI_BASE					(WM_USER + 9500)		// 이 프로그램에서 사용할 사용자정의 메세지 Base
#define GxMSG_GUI_CHANGE_VIEW			(GxMSG_GUI_BASE + 1)	// 화면 전환 요청 BottomBar -> MainFrm, 실제 전환 수행
#define GxMSG_GUI_EQ_CONNECT			(GxMSG_GUI_BASE + 2)	// 설비와 TCP/IP 연결이 이루어졌다.
#define GxMSG_GUI_EQ_DISCONNECT		(GxMSG_GUI_BASE + 3)	// 설비와 TCP/IP 연결이 끊어졌다.
#define GxMSG_GUI_COMM_UPDATE			(GxMSG_GUI_BASE + 4)	// 통신 상태가 변경되었다.
#define GxMSG_GUI_INFO_UPDATE			(GxMSG_GUI_BASE + 5)	// 각종 정보가 변경되었다. 화면에 반영한다.

#define GxMSG_COMM_EQ_CONNECT			(GxMSG_GUI_BASE + 10)	// 설비와 TCP/IP 연결이 이루어졌다.
#define GxMSG_COMM_EQ_DISCONNECT		(GxMSG_GUI_BASE + 11)	// 설비와 TCP/IP 연결이 끊어졌다.
#define GxMSG_COMM_EQ_RECEIVE			(GxMSG_GUI_BASE + 12)	// 설비에서 데이터가 수신되었다.



// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==


// Y : 영우 (A : ANI)
// M : Main Program. (L : Loader, U : Unloader, D : Data)
// FTI : 포스터치설비 (MT : MT설비, FT : FT설비)
#define _PROGRAM_VERSION				_T("Y_D_MTP_18.05.31.01")	// Program Version
// #define _PROGRAM_VERSION				_T("2016.11.19.01")			// Data Program Version

#define _EQ_AMT							0
#define _EQ_SFT							1
#define _EQ_TYPE_							_EQ_AMT				// 0 : AMT, 1 : SFT

// Path
#define SETUP_PATH						"Config_CIM\\"
#define LOG_PATH						"Log"
#define RESULT_PATH						"RESULT_DATA"
#define TSP_PATH						"TSP_DATA"
#define CELL_PATH						"CELL_DATA"
#define ERR_IMG_PATH					"D:\\Error_Img"
#define CONTACT_IMG_PATH				"Img"
#define SERVER_PATH						"" //"X:"
#define A2_RESULT_PATH			        "" //"Y:"
#define A1_RESULT_PATH					"" //"Z:"
#define USER_MAIN_PATH					"D:\\YWDSP"
#define USER_LOG_PATH					"D:\\Log\\CIM"
#define TRACKOUT_INFO_PATH				"D:\\TRACKOUT INFO"
#define DEF_USER_MAIN_PATH				"" //"C:\\"	// D Drive가 없을 때
//#define REPAIR_LIST_PATH                "" //"D:\\repair"  // Repair 좌료 표시
#define ALARM_PATH						"Alarm\\"


#define S2F18REP_TIMER_PTRID			10000
#define TRUE_LOG_BY_MESG_TIMER		10001		//로그 기록 안하는 시간.
#define FALSE_LOG_BY_MESG_TIMER		10002		//로그 기록 하는 시간.
#define HOST_CONNECT_TIMER				10003		// HOST 연결 Timer

#define READ_FDC_TIMER					20000
#define VIEW_FDC_SV_TIMER				30000


//#define VIEW_TACTTIME_GRAPH_TIMER		31100
#define TITLE_TIMER						32000
#define GAONNURI_VER_TIMER				32001
#define SOCKET_STATUS_TIMER				33000
//#define SEM_CONNECTION_CHECK_TIMER		34000
//#define VIEW_TACTTIME_VALUE_TIMER		35000
// #define VIEW_CELL_DELTE_TIMER			36000
// #define VIEW_ALARM_DELTE_TIMER			37000
// #define VIEW_MCR_ALIGN_TIMER			38000


#define STR_DEF_LEN		255
#define USER_LOG_LEN	1024

#define SOCKET_NAME_CLIENT_EQ	_T("EQ") // _T("CLIENT_AFT")
#define SOCKET_NAME_SERVER		_T("SERVER")





// EVENT ID & ALARM CODE
// XCom 이 동작되는 중간에 발생하는 event 및 alarm Code

#define EVTXC_NOT_CONNECTED		101		// change to "NOT CONNECTED" (HSMS) 					, TCP/IP Socket 연결이 끊어졌음을 나타냄 이때는 SECS 메시지를 전송하면 안됩니다.
#define EVTXC_NOT_SELECTED		102		// change to "NOT SELECTED" (HSMS)						, TCP/IP Socket 연결이 이루어졌지만 아직 selected 되지는 않음을 나타냅니다. 이때는 SECS 메시지를 전송하면 안됩니다.
#define EVTXC_SELECTED			103		// change to "SELECTED" (HSMS)							, HSMS가 selected 상태가 되었음을 의미합니다. SECS 메시지의 전송이 가능합니다.

#define ALMXC_T1				201		// T1 Timeout											, intercharacter timout 이라고도 하며, SECS-1 에서 데이터 문자열을 수신하는 도중에 수신해야 할 데이타가 남아있는데 다음 데이터 문자열이 시간안에 도착하지 않음
#define ALMXC_T2				202		// T2 Timout											, Control timeout이라고도 하며 ,SEC-1에서 Control 무나져을 보낸 후 대응하지 응답 control 문자가 시간안에 도착하지 않음
#define ALMXC_T3				203		// T3 Timeout											, Transaction timeout , Primary Message를 전송후 해당하는 Secondary Message가 시간안에 받지 못함			
#define ALMXC_T4				204		// T4 Timeout											, Block timeout,	SECS-1 멀티블락 데이타 수신하는 도중에 다음 블락데이타가 시간안에 도착 하지않음
#define ALMXC_T5				205		// T5 Timeout											, HSMS에서 Socket이 연결된 후 selection handshaking을 진행하다 실패했을 대 connection을 끊고 다시 HSMS연결을 시도하기 까지 기다리는 시간
#define ALMXC_T6				206		// T6 Timeout											, Control timeout,	HSMS에서 selected.req와 같은 control메시지를 전송후 해당 응답메시지를 시간안에 받지 못함
#define ALMXC_T7				207		// T7 Timeout											, Not Selected timeout,	HSMS에서 Socket이 connected된 후 일정시간안에 selected.req를 받지 못함
#define ALMXC_T8				208		// T8 Timeout											, HSMS에서 데이터 문자열 수신 후 다음 문자 데이타를 시간안에 받지 못했을때 ( = T1 Timout)
#define ALMXC_RETRY				209		// Retry limit											, 재전송 회수가 넘었습니다.
#define ALMXC_GARBAGE_INPUT		210		// Garbage input										, 올바른 SECS 스트림이 아님
#define ALMXC_LOW_SEND			211		// Low level sending error								, SECS_1 Message 전송 실패
#define ALMXC_MSG_TIMEOUT		212		// Send Message Timeout									, XCom 내부에 보관하고 있던 Send Message data가 보관 시간이 초과되어 삭제
#define ALMXC_DUPLICATED_MSG	213		// Duplicated message									, 이전에 수신된 메시지와 같은 헤더 메시지 스트림이 도착
#define ALMXC_INITIALIZE		214		// Intialize() error									, 초기화 도중 에러 발생
#define ALMXC_START				215		// Start() error										, 시작 도중 에러 발생
#define ALMXC_PROTECTION		216		// Protection error										, XCom의 하드웨어키의 체크에 실패하였을때 발생, 하드웨어키를 정상적으로 설치했는데도 발생하면 하드웨어키의 접속상태가 불량,오류
#define ALMXC_INVALID_MSG		217		// Unknown or invalid structured message				, 메시지가 SECS MEssage Structure에 지정된 형식 아님
#define ALMXC_ABORT_TRANSACTION	218		// Abort transaction(fuction0)							, 트랜잭션이 Aboart
#define ALMXC_INVALID_INSTACE	219		// invaild XCom instance								, XCom 인스턴스가 라이센스에 지정된 수를 초과
#define ALMXC_INVALID_DATA_SEQ	220		// Invaild data setting sequence						, 호출된 Get 함수가 메시지 아이템의 현재 포멧과 다름
#define ALMXC_UNKNOWN_DEV_ID	221		// Unrecognized device ID								, 등록되지 않은 DeviceID를 가진 메시지
#define ALMXC_UNKNOWN_STREAM	222		// Unrecognized stream type								, 등록되지 않은 Stream 값을 가진 메시지
#define ALMXC_UNKNOWN_FUNC		223		// Unrecognized fuction type							, 등록되지 않은 Fuction 값을 가진 메시지
#define ALMXC_RESTART			224		// XCom restarted										, 내부에서 XCOM이 재시작
#define ALMXC_DATA_TOO_LONG		225		// data too long:too many items or too many list depth	, XCom configuration의 queue size 보다 큰 메시지가 수신

#define ALMXC_SOCKET_ERROR		301		// General socket error									, 기타 socket error, lparam 값 , Socket error값이 수신
#define ALMXC_NETDOWN			302		// Network is down										, Socket error 10050에 대한 이벤트 , 네트워크 서브 시스템에 에러
#define ALMXC_CONNRESET			303		// Connection reset by peer								, Socket error 10054에 대한 이벤트 , 원격 종료 -> 리셋
#define ALMXC_TIMEDOUT			304		// Connection timed out									, Socket error 10060에 대한 이벤트 , 접속시도 시간초과




enum
{
	E_CELL_PROCESS_START	= 401,
	
	
};





enum
{
	WM_USER_MSG = WM_USER+1	,

};
