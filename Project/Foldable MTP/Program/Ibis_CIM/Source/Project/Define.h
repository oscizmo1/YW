#pragma once


// ����� ���� Message ��� ����

#define _GAUS_LIB									// �ҽ� ��ü�� ����Ѵ�.

// ��/���̾�α� ȭ�� ��ȯ�� �ʿ��� ���� ��� �Ϸ� ��ȣ ����
enum {

	// CFormView, ���� ȭ�� �� 
	eFORM_Default			= 0,			// ���α׷� ���۽� �⺻������ ������ �ִ�  CFormView
	eFORM_CIM,								// Main ȭ��, CIM ��� ȭ��
	eFORM_FDC,								// FDC �׸� ����
	eFORM_RMS,								// RMS �׸� ����
	eFORM_ECM,								// CEM �׸� ����
	eFORM_SETTING,							// ���� ���� ����

	// ��� ���� ����
	eIDX_HostConnect		= 0,			// HOST���� ����
	eIDX_HostOnline,						// SELECT / ONLINE ���� ����
	eIDX_HostRecevie,						// �޼��� ���� ó��
	
};

// ����� ���� Messge ����
#define GxMSG_GUI_BASE					(WM_USER + 9500)		// �� ���α׷����� ����� ��������� �޼��� Base
#define GxMSG_GUI_CHANGE_VIEW			(GxMSG_GUI_BASE + 1)	// ȭ�� ��ȯ ��û BottomBar -> MainFrm, ���� ��ȯ ����
#define GxMSG_GUI_EQ_CONNECT			(GxMSG_GUI_BASE + 2)	// ����� TCP/IP ������ �̷������.
#define GxMSG_GUI_EQ_DISCONNECT		(GxMSG_GUI_BASE + 3)	// ����� TCP/IP ������ ��������.
#define GxMSG_GUI_COMM_UPDATE			(GxMSG_GUI_BASE + 4)	// ��� ���°� ����Ǿ���.
#define GxMSG_GUI_INFO_UPDATE			(GxMSG_GUI_BASE + 5)	// ���� ������ ����Ǿ���. ȭ�鿡 �ݿ��Ѵ�.

#define GxMSG_COMM_EQ_CONNECT			(GxMSG_GUI_BASE + 10)	// ����� TCP/IP ������ �̷������.
#define GxMSG_COMM_EQ_DISCONNECT		(GxMSG_GUI_BASE + 11)	// ����� TCP/IP ������ ��������.
#define GxMSG_COMM_EQ_RECEIVE			(GxMSG_GUI_BASE + 12)	// ���񿡼� �����Ͱ� ���ŵǾ���.



// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==


// Y : ���� (A : ANI)
// M : Main Program. (L : Loader, U : Unloader, D : Data)
// FTI : ������ġ���� (MT : MT����, FT : FT����)
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
#define DEF_USER_MAIN_PATH				"" //"C:\\"	// D Drive�� ���� ��
//#define REPAIR_LIST_PATH                "" //"D:\\repair"  // Repair �·� ǥ��
#define ALARM_PATH						"Alarm\\"


#define S2F18REP_TIMER_PTRID			10000
#define TRUE_LOG_BY_MESG_TIMER		10001		//�α� ��� ���ϴ� �ð�.
#define FALSE_LOG_BY_MESG_TIMER		10002		//�α� ��� �ϴ� �ð�.
#define HOST_CONNECT_TIMER				10003		// HOST ���� Timer

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
// XCom �� ���۵Ǵ� �߰��� �߻��ϴ� event �� alarm Code

#define EVTXC_NOT_CONNECTED		101		// change to "NOT CONNECTED" (HSMS) 					, TCP/IP Socket ������ ���������� ��Ÿ�� �̶��� SECS �޽����� �����ϸ� �ȵ˴ϴ�.
#define EVTXC_NOT_SELECTED		102		// change to "NOT SELECTED" (HSMS)						, TCP/IP Socket ������ �̷�������� ���� selected ������ ������ ��Ÿ���ϴ�. �̶��� SECS �޽����� �����ϸ� �ȵ˴ϴ�.
#define EVTXC_SELECTED			103		// change to "SELECTED" (HSMS)							, HSMS�� selected ���°� �Ǿ����� �ǹ��մϴ�. SECS �޽����� ������ �����մϴ�.

#define ALMXC_T1				201		// T1 Timeout											, intercharacter timout �̶�� �ϸ�, SECS-1 ���� ������ ���ڿ��� �����ϴ� ���߿� �����ؾ� �� ����Ÿ�� �����ִµ� ���� ������ ���ڿ��� �ð��ȿ� �������� ����
#define ALMXC_T2				202		// T2 Timout											, Control timeout�̶�� �ϸ� ,SEC-1���� Control �������� ���� �� �������� ���� control ���ڰ� �ð��ȿ� �������� ����
#define ALMXC_T3				203		// T3 Timeout											, Transaction timeout , Primary Message�� ������ �ش��ϴ� Secondary Message�� �ð��ȿ� ���� ����			
#define ALMXC_T4				204		// T4 Timeout											, Block timeout,	SECS-1 ��Ƽ��� ����Ÿ �����ϴ� ���߿� ���� �������Ÿ�� �ð��ȿ� ���� ��������
#define ALMXC_T5				205		// T5 Timeout											, HSMS���� Socket�� ����� �� selection handshaking�� �����ϴ� �������� �� connection�� ���� �ٽ� HSMS������ �õ��ϱ� ���� ��ٸ��� �ð�
#define ALMXC_T6				206		// T6 Timeout											, Control timeout,	HSMS���� selected.req�� ���� control�޽����� ������ �ش� ����޽����� �ð��ȿ� ���� ����
#define ALMXC_T7				207		// T7 Timeout											, Not Selected timeout,	HSMS���� Socket�� connected�� �� �����ð��ȿ� selected.req�� ���� ����
#define ALMXC_T8				208		// T8 Timeout											, HSMS���� ������ ���ڿ� ���� �� ���� ���� ����Ÿ�� �ð��ȿ� ���� �������� ( = T1 Timout)
#define ALMXC_RETRY				209		// Retry limit											, ������ ȸ���� �Ѿ����ϴ�.
#define ALMXC_GARBAGE_INPUT		210		// Garbage input										, �ùٸ� SECS ��Ʈ���� �ƴ�
#define ALMXC_LOW_SEND			211		// Low level sending error								, SECS_1 Message ���� ����
#define ALMXC_MSG_TIMEOUT		212		// Send Message Timeout									, XCom ���ο� �����ϰ� �ִ� Send Message data�� ���� �ð��� �ʰ��Ǿ� ����
#define ALMXC_DUPLICATED_MSG	213		// Duplicated message									, ������ ���ŵ� �޽����� ���� ��� �޽��� ��Ʈ���� ����
#define ALMXC_INITIALIZE		214		// Intialize() error									, �ʱ�ȭ ���� ���� �߻�
#define ALMXC_START				215		// Start() error										, ���� ���� ���� �߻�
#define ALMXC_PROTECTION		216		// Protection error										, XCom�� �ϵ����Ű�� üũ�� �����Ͽ����� �߻�, �ϵ����Ű�� ���������� ��ġ�ߴµ��� �߻��ϸ� �ϵ����Ű�� ���ӻ��°� �ҷ�,����
#define ALMXC_INVALID_MSG		217		// Unknown or invalid structured message				, �޽����� SECS MEssage Structure�� ������ ���� �ƴ�
#define ALMXC_ABORT_TRANSACTION	218		// Abort transaction(fuction0)							, Ʈ������� Aboart
#define ALMXC_INVALID_INSTACE	219		// invaild XCom instance								, XCom �ν��Ͻ��� ���̼����� ������ ���� �ʰ�
#define ALMXC_INVALID_DATA_SEQ	220		// Invaild data setting sequence						, ȣ��� Get �Լ��� �޽��� �������� ���� ����� �ٸ�
#define ALMXC_UNKNOWN_DEV_ID	221		// Unrecognized device ID								, ��ϵ��� ���� DeviceID�� ���� �޽���
#define ALMXC_UNKNOWN_STREAM	222		// Unrecognized stream type								, ��ϵ��� ���� Stream ���� ���� �޽���
#define ALMXC_UNKNOWN_FUNC		223		// Unrecognized fuction type							, ��ϵ��� ���� Fuction ���� ���� �޽���
#define ALMXC_RESTART			224		// XCom restarted										, ���ο��� XCOM�� �����
#define ALMXC_DATA_TOO_LONG		225		// data too long:too many items or too many list depth	, XCom configuration�� queue size ���� ū �޽����� ����

#define ALMXC_SOCKET_ERROR		301		// General socket error									, ��Ÿ socket error, lparam �� , Socket error���� ����
#define ALMXC_NETDOWN			302		// Network is down										, Socket error 10050�� ���� �̺�Ʈ , ��Ʈ��ũ ���� �ý��ۿ� ����
#define ALMXC_CONNRESET			303		// Connection reset by peer								, Socket error 10054�� ���� �̺�Ʈ , ���� ���� -> ����
#define ALMXC_TIMEDOUT			304		// Connection timed out									, Socket error 10060�� ���� �̺�Ʈ , ���ӽõ� �ð��ʰ�




enum
{
	E_CELL_PROCESS_START	= 401,
	
	
};





enum
{
	WM_USER_MSG = WM_USER+1	,

};
