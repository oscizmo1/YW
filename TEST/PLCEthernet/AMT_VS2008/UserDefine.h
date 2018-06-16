// Title Set
#define	MT_TITLE						"C:\\AMT_NEW"
#define MT_VERSION					"AMT VER 0.1b"


// Path Set
//#define MAIN_PATH					".:\\"MT_TITLE
#define MAIN_PATH					MT_TITLE
#define SYSTEM_PATH					MAIN_PATH"\\SYSTEM"
#define EVENT_PATH					MAIN_PATH"\\EVENT"
#define RECIPE_PATH					MAIN_PATH"\\RECIPE"
#define IMAGE_PATH					MAIN_PATH"\\IMAGE"
#define ALIGN_PATH					MAIN_PATH"\\ALIGN"
#define LOG_PATH					MAIN_PATH"\\LOG"

#define USER_PATH					SYSTEM_PATH"\\USER.ini"
#define ERROR_PATH					SYSTEM_PATH"\\ErrorCode.txt"
#define IODEFINE_PATH				SYSTEM_PATH"\\IO_Name_Define.INI"
#define MOTION_SOCKET_PARAM_PATH	SYSTEM_PATH"\\Motion_Socket Parameter.ini"
#define CONFIG_PATH					SYSTEM_PATH"\\Config.INI"
#define LASTGLASS_PATH				SYSTEM_PATH"\\LastGlass.INI"
#define DEVICE_INFO_PATH			SYSTEM_PATH"\\DeviceInfo.INI"
#define LC_INFO_PATH				SYSTEM_PATH"\\LC_Info.INI"
#define ENGINEER_DOWN_CODE_PATH		SYSTEM_PATH"\\EngineerDownCode.INI"

// Color Set#define RED						0
#define YELLOW					1
#define GREEN					2

#define COLOR_RED		RGB(255,0,0)
#define COLOR_GREEN		RGB(0,255,0)
#define COLOR_BLUE		RGB(0,0,255)
#define COLOR_GRAY		RGB(128,128,128)
#define COLOR_WHITE		RGB(255,255,255)

// Sensor Value
#define SENSOR_ON				1
#define SENSOR_OFF				0
#define SENSOR_MISMATCH		-1

#define UP							0
#define DOWN						1

#define M_PI							3.141592

#define		MAX_CAM_NUM				4	// ����Ǵ� Camera ����
#define		MAX_PATTERN_NUM			4	// Camera �ϳ��� �� ���Ǵ� ���� ���� (FPC*3, PCB)
#define		MAX_BUFFER_NUM			8	// Mil Buffer ���� 4(Auto)+2(Teach)+2(Align)
#define		MAX_MOD_BUFFER_NUM		16	// Mil Model & Result Buffer ���� 4(Cam Num)+3(FPC Model)+4(Cam Num)*1(PCB Model)

#define		LIMIT_SCORE			70	// Pattern search score �⺻��
#define		BINARIZE_CONDITION	60	// 2��ȭ Condition	�⺻��
#define     BINARIZE_LEVEL_STEP 5   //  [6/9/2009 janghan]  Auto Tune 2��ȭ ���� �ܰ� (5�� ���� )
#define     BINARIZE_MAX_LEVEL  254 //  [6/9/2009 janghan]  Auto Tune 2��ȭ MAX�� 

// Unit Position 
enum
{
	MOVE_NONE = -1,
	ORG_POS =0,
	READY_POS,
	GRAB_POS,
	CONTACT_CHK_POS,
	CONTACT_POS,
	SET_POS,
	PCB_CHANGE_POS
};

enum
{
	STATUS_READY,	// �ǳ� ���� RUN ��
	STATUS_STOP,	// EMO�� ERROR�� �ƴ� ��� ��������
	STATUS_INIT,	// �ʱ�ȭ���϶�
	STATUS_RUN,		// ��� ���� ��(�ǳ� ����)
	STATUS_ERROR,	// Error �߻� �� RESET ������
	STATUS_EMO,		// EMO ��ȣ�� �����ִ� ����
	STATUS_OPCALL,	// operator call
	STATUS_ALL
};




// User Set
enum
{
	SUPER_USER,
	ENGINEER,
	OPERATOR,
	USER_MAX
};

// Timer Set
enum
{
	CLOCK,
	WATCH_DOG,
	IO_RUN_CHECK,
	MOVE_HOME,
	NORMAL_CUR_POS,
	PIN_CUR_POS,
	DRAW_PATTERN,
	ALIGN_MOVE,
	TEACH_MOVE,
	MOTION_SENSOR_CHECK,
	RE_ERROR_ALARM_CHECK,
	PLC_ETHERNET,
	TIMER_TILT_WAIT,
	TIMER_BIT_TOGGLE,
	TIMER_MAX
};

// Line Control

enum EProcessStates
{
	ePNothing = 0,
	eInit,			// Initializing
	eIdle,
	eSetup,
	eReady,
	eExecute,		// �ǳ� ���ͼ� ������
	ePause		// Stop & Error
};
enum EEqStates
{
	eEqNothing	= 0,	// NOTHING
	eNormal,			// NORMAL		Stop & Run
	eFault,				// FAULT			Error
	ePM					// PM
};
enum EPanelEvents
{
	ePanelNothing			= 0,
	eVIResult				= 1,
	eAlramReport		    = 2,
	ePanelScrap				= 14, 
	ePanelUnScrap			= 15,
	ePanelIn			    = 16,
	ePanelOut				= 17,
	eCANNOTSAVE_RAW_DATA	= 63,
	eCANNOTSAVE_SUM_DATA	= 64,
	eLastGlassLoss		    = 261
};
enum EPanelPosition
{
	ePosIn					= 0,
	ePosOut					= 1,
	ePosScrap				= 2,
	ePosUnScrap				= 3,
	ePosVIReport			= 4,
	ePosVIAlramReport		= 5,
	ePosCANNOTSAVE_RAW_DATA	= 63,
	ePosCANNOTSAVE_SUM_DATA	= 64
};
enum EPMMode
{	
	ePMMachine2LC = 0,
	ePMLC2Machine
};
enum ELogIn
{
	eLogIn					= 161,
	eLogOut					= 162
};

enum MsgTitle
{
	MSG_NORMAL,
	MSG_CHECK,
	MSG_ERR_AUTO,
	MSG_ERR_MANUAL,
	MSG_ERR_INIT,
	MSG_INFO,
	MSG_INSP,
	MSG_EXIT,
	MSG_AUTO_SELECT	// RunStatus�� ���� MSG_ERR_AUTO���� MSG_ERR_MANUAL���� �ڵ� �Ǻ�����
};


// PC ���� [2/05/2009 jjh]
typedef enum
{
	A_AMT,
	B_AMT,
	A_MMT1,
	A_MMT2,
	B_MMT1,
	B_MMT2
}MachinePCType;

// ȣ�⺰ ���� [2/05/2009 jjh]
typedef enum
{
	A_GROUP_TYPE,
	SINGLE_ARM_TYPE,
	DOUBLE_ARM_TYPE,
	SINGLE_ROBOT_TYPE,
	REMOVE_TILTING_TYPE		// PLC���� Tilting ���� [5/19/2009 OSC]
}LineType;

typedef enum
{
	LAN_KOR,		// Kor Version
	LAN_ENG,		// Eng Version
}LangueageType;

typedef enum
{
	MODE_NORMAL,		// ���� ���� ���
	MODE_STAGE,			// ���� �������� ���� ���		����� ���ø� ���� �ݺ�... ƿ�õ�?
	MODE_ALIGN_SKIP,	// ����� ��ŵ(�Ŵ��� ����) ���
	MODE_VISION_SKIP	// ����� + �˻� ��ŵ ���
}RunMode;

typedef enum
{
	AbsNone = 0,		// 
	AbsCSD3 = 11,	// Samsung Servo Motor
	AbsSigma3 = 22		// Sigma3 Yasukawa Servo Motor
};
