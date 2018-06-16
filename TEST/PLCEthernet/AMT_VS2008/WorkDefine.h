#define CAM_0		0	// Left Side
#define CAM_1		1	// Left Center
#define CAM_2		2	// Right Center
#define CAM_3		3	// Right Side

//////////////////////////////////////////////////////////////////////////
// MAX_BUFFER_NUM	8
#define DISP_AUTO_0				0
#define DISP_AUTO_1				1
#define DISP_AUTO_2				2
#define DISP_AUTO_3				3
#define DISP_TEACH_L			4
#define DISP_TEACH_R			5
#define DISP_ALIGN				6
#define DISP_ALIGN_PREV			7

//////////////////////////////////////////////////////////////////////////
// MAX_MOD_BUFFER_NUM	16
enum //  Remodel by [6/9/2009 janghan]
{	
	/*CAM_0_FPC_0,
	CAM_0_FPC_1,
	CAM_0_FPC_2,
	CAM_0_PCB,
	CAM_1_FPC_0,
	CAM_1_FPC_1,
	CAM_1_FPC_2,
	CAM_1_PCB,
	CAM_2_FPC_0,
	CAM_2_FPC_1,
	CAM_2_FPC_2,
	CAM_2_PCB,
	CAM_3_FPC_0,
	CAM_3_FPC_1,
	CAM_3_FPC_2,
	CAM_3_PCB*/

	CAM_0_FPC_0,
	CAM_0_FPC_1,
	CAM_0_PCB_0,
	CAM_0_PCB_1,
	CAM_1_FPC_0,
	CAM_1_FPC_1,
	CAM_1_PCB_0,
	CAM_1_PCB_1,
	CAM_2_FPC_0,
	CAM_2_FPC_1,
	CAM_2_PCB_0,
	CAM_2_PCB_1,
	CAM_3_FPC_0,
	CAM_3_FPC_1,
	CAM_3_PCB_0,
	CAM_3_PCB_1
};

#define IMAGE_SIZE_X				640L
#define IMAGE_SIZE_Y				480L
#define PATTERN_SIZE_X				193.0
#define PATTERN_SIZE_Y				150.0
#define DONTCARE_SIZE_X				407L
#define DONTCARE_SIZE_Y				307L
#define DONTCARE_SCALEFACTOR		2.0

// Log Define.//

#define LOG_XNET					1	// Used
#define LOG_EQ						2	// Used
#define LOG_MELSEC					3
#define LOG_LOGIN					4
#define LOG_ERROR					5	// Used
#define LOG_GLASS_DATA				6	// Used
#define LOG_TACT_TIME				7
#define LOG_INSPECTION				8
#define LOG_MELSEC_ERR				9
#define LOG_COMM					10	// Used
#define LOG_SYSTEM					11	// Used
#define LOG_ALIGN					12	// Used
#define LOG_MOVE_COMPLET			13	// Used
#define LOG_MOVE_PULSE				14	// Used
#define LOG_AUTOTUNE				15 //  [6/20/2009 SESL]
#define LOG_TILT_ERR				16 //  [8/14/2009 janghan]
#define LOG_Reset_Count             17 // 검사 카운트 리셋 [9/10/2009 janghan]
#define LOG_INSP_COUNT		        18 // Insp Count 10EA [12/1/2009 JJH-RomanticBoy]

// SeqMoveStage.//
enum
{
	MS_CHK_EQUIPMENT,

	// A군 제외	(어떻게 움직이는지 몰라서 우선 다 지웠다)
	MS_MOVE_Y,
	MS_ISOK_Y,
	MS_MOVE_X,
	MS_ISOK_X,
	MS_GRAB_CONTACT_IMG,

	MS_COMPLETE
};

// SeqCellSend Define.//
enum
{	// Not yet.
	SEQ_UNLD_CHK_REQUEST,				
	SEQ_UNLD_CHK_CONDITION,				
	SEQ_UNLD_DATA_WRITE,				
	SEQ_UNLD_OUT_READY_BIT_ON,
	SEQ_UNLD_CHK_LD_END,
	SEQ_UNLD_OUT_REQ_BIT_ON,			
	SEQ_UNLD_CHK_IN_REQ_BIT_ON,			
	SEQ_UNLD_CHK_IN_DO_BIT_ON,			
	SEQ_UNLD_CHK_IN_END_BIT_ON,			
	SEQ_UNLD_OUT_END_BIT_ON,			
	SEQ_UNLD_CHK_IN_ALL_BIT_OFF,		
	SEQ_UNLD_OUT_ALL_BIT_OFF,			
	SEQ_UNLD_CHK_INSP_END,				
	SEQ_UNLD_CHK_IN_INSP_END_ON,		
	SEQ_UNLD_ERR,				
	SEQ_UNLD_COMPLETE
};

// SeqCellRecv Define.//
enum
{	// Not yet.
	SEQ_LD_CHK_REQUEST,
	SEQ_LD_CHK_PANEL,
	SEQ_LD_CHK_CONDITION,
	SEQ_LD_OUT_READY_BIT_ON,
	SEQ_LD_CHK_UNLD_END,
	SEQ_LD_OUT_REQ_BIT_ON,
	SEQ_LD_CHK_IN_REQ_BIT_ON,
	SEQ_LD_READ_DEVICEID,
	SEQ_LD_PCB_CHANGE_POS,
	SEQ_LD_FIND_DEVICEID,
	SEQ_LD_MATCH_DEVICEID,
	SEQ_LD_MODEL_CHANGE,
	SEQ_LD_MODEL_CHANGE_START_BIT_OFF,
	SEQ_LD_MODEL_CHANGE_CHK_BIT_OFF,
	SEQ_LD_CHK_IN_DO_BIT_ON,
	SEQ_LD_DATA_READ,
	SEQ_LD_CHK_IN_UP_BIT_ON,
	SEQ_LD_CHK_IN_END_BIT_ON,
	SEQ_LD_CHK_IN_ALL_BIT_OFF,
	SEQ_LD_ERR,
	SEQ_LD_COMPLETE
};

// SeqCellContProc Define.//
enum
{	// Not yet.
	MAIN_CHK_INTERLOCK,
	MAIN_BF_TILT_DOWN,
	MAIN_BF_CHK_TILT_DOWN,
	MAIN_BF_CONTACT_OFF,
	MAIN_BF_CHK_CONTACT_OFF,
	MAIN_BF_CLAMP_OFF,
	MAIN_BF_CHK_CLAMP_OFF,
	MAIN_BF_MOVE_READY_POS,
	MAIN_BF_SEARCH_CAM0_PCB,		
	MAIN_BF_SEARCH_CAM1_PCB,		
	MAIN_BF_SEARCH_CAM2_PCB,		
	MAIN_BF_SEARCH_CAM3_PCB,
	MAIN_CHK_GLS_EXIST,
	MAIN_UNSCRAP,
	MAIN_GLS_RECV,
	MAIN_CHK_GLS_RECV_COMP,
	MAIN_REPORT_GLS_IN,
	MAIN_CHK_INSP_FLAG,
	MAIN_GLS_CLAMP_ON,
	MAIN_CHK_GLS_CLAMP_ON,

	MAIN1_CHK_INTERLOCK,
	MAIN1_TILT_UP,
	MAIN1_ALIGN_START,
	MAIN1_CHK_ALIGN_COMP,
	MAIN1_SND_PG_START,
	MAIN1_CHK_TILT_UP,
	MAIN1_CHK_PG_END,
	MAIN1_CONTACT_OFF,
	MAIN1_CHK_CONTACT_OFF,
	MAIN1_CHK_INTERLOCK_,
	MAIN1_TILT_DOWN,
	MAIN1_MOVE_READY_POS,
	MAIN1_CHK_TILT_DOWN,

	MAIN2_ALIGN_START,
	MAIN2_CHK_ALIGN_COMP,
	MAIN2_SND_PRE_GRAB_START,
	MAIN2_CHK_PRE_GRAB_END,
	MAIN2_SND_POST_GRAB_START,
	MAIN2_CHK_POST_GRAB_END,
	MAIN2_CONTACT_OFF,
	MAIN2_CHK_CONTACT_OFF,
	MAIN2_MOVE_READY_POS,
	MAIN2_CHK_INSP_END,
	MAIN2_CHK_HOLD,
	MAIN2_CHK_QA,

	MAIN_REPORT_IR,
	MAIN_CLAMP_OFF,
	MAIN_CHK_CLAMP_OFF,
	MAIN_GLS_SEND,
	MAIN_ERR,
	MAIN_END
};

// SeqContactUnit Define.//
enum
{
	SEQ_CONTACT_CHK_REQUEST,				// 0	:
	SEQ_CONTACT_CHK_INTERLOCK,				// 50	:
	SEQ_CONTACT_UNIT_OFF_CHK,
	SEQ_CONTACT_MOVE_READYPOS,				// 70	:
	SEQ_CONTACT_SEARCH_CAM0_PCB,			// 90	:
	SEQ_CONTACT_SEARCH_CAM1_PCB,			// 100	:
	SEQ_CONTACT_SEARCH_CAM2_PCB,			// 105	:
	SEQ_CONTACT_SEARCH_CAM3_PCB,
	SEQ_CONTACT_BLOW_ON,					// 110	:
	SEQ_CONTACT_MOVE_GRABPOS,				// 120	:
	SEQ_CONTACT_SEARCH_CAM0_FPC,			// 140	:
	SEQ_CONTACT_SEARCH_CAM1_FPC,			// 150	:
	SEQ_CONTACT_SEARCH_CAM2_FPC,			// 160	:
	SEQ_CONTACT_SEARCH_CAM3_FPC,
	SEQ_CONTACT_AUTOTUNE_CAM0_FFC_EDGE,//  [6/11/2009 janghan]
	SEQ_CONTACT_AUTOTUNE_CAM1_FFC_EDGE,//  [6/11/2009 janghan]
	SEQ_CONTACT_AUTOTUNE_CAM2_FFC_EDGE,//  [6/11/2009 janghan]
	SEQ_CONTACT_AUTOTUNE_CAM3_FFC_EDGE,//  [6/11/2009 janghan]
	//SEQ_CONTACT_AUTOTUNE_CAM4_FFC_EDGE,//  [6/11/2009 janghan]
	SEQ_CONTACT_AUTOTUNE_CAM0_PCB_EDGE,//  [6/11/2009 janghan]
	SEQ_CONTACT_AUTOTUNE_CAM1_PCB_EDGE,//  [6/11/2009 janghan]
	SEQ_CONTACT_AUTOTUNE_CAM2_PCB_EDGE,//  [6/11/2009 janghan]
	SEQ_CONTACT_AUTOTUNE_CAM3_PCB_EDGE,//  [6/11/2009 janghan]
	SEQ_CONTACT_AUTOTUNE_CAM0_FFC_MARK,//  [6/11/2009 janghan]
	SEQ_CONTACT_AUTOTUNE_CAM1_FFC_MARK,//  [6/11/2009 janghan]
	SEQ_CONTACT_AUTOTUNE_CAM2_FFC_MARK,//  [6/11/2009 janghan]
	SEQ_CONTACT_AUTOTUNE_CAM3_FFC_MARK,//  [6/11/2009 janghan]
	SEQ_CONTACT_AUTOTUNE_CAM0_PCB_MARK,//  [6/11/2009 janghan]
	SEQ_CONTACT_AUTOTUNE_CAM1_PCB_MARK,//  [6/11/2009 janghan]
	SEQ_CONTACT_AUTOTUNE_CAM2_PCB_MARK,//  [6/11/2009 janghan]
	SEQ_CONTACT_AUTOTUNE_CAM3_PCB_MARK,//  [6/11/2009 janghan]
	SEQ_CONTACT_AUTOTUNE_CAM0_MOVE_CENTER_CHK,//  [6/11/2009 janghan]
	SEQ_CONTACT_AUTOTUNE_CAM1_MOVE_CENTER_CHK,//  [6/11/2009 janghan]
	SEQ_CONTACT_AUTOTUNE_CAM2_MOVE_CENTER_CHK,//  [6/11/2009 janghan]
	SEQ_CONTACT_AUTOTUNE_CAM3_MOVE_CENTER_CHK,//  [6/11/2009 janghan]
	//SEQ_CONTACT_AUTOTUNE_CAM0_CALIBRATION, //  [6/11/2009 janghan]
	//SEQ_CONTACT_AUTOTUNE_CAM1_CALIBRATION, //  [6/11/2009 janghan]
	//SEQ_CONTACT_AUTOTUNE_CAM2_CALIBRATION, //  [6/11/2009 janghan]
	//SEQ_CONTACT_AUTOTUNE_CAM3_CALIBRATION, //  [6/11/2009 janghan]
	//SEQ_CONTACT_AUTOTUNE_CAM0_SORT_X,      //  [6/16/2009 janghan]
	//SEQ_CONTACT_AUTOTUNE_CAM1_SORT_X,      //  [6/16/2009 janghan]
	//SEQ_CONTACT_AUTOTUNE_CAM2_SORT_X,      //  [6/16/2009 janghan]
	//SEQ_CONTACT_AUTOTUNE_CAM3_SORT_X,      //  [6/16/2009 janghan]

	SEQ_CONTACT_CALIBRATION,				// 200	:
	SEQ_CONTACT_MOVE_CONTACTPOS,			// 210	:
	SEQ_CONTACT_UNIT_ON,					// 230	:
	SEQ_CONTACT_BLOW_OFF,					// 250	:
	SEQ_CONTACT_ERROR,						
	SEQ_CONTACT_COMPLETE					// 260	:
};

// VI Code Define.//
enum
{	// Not yet.	
	TBLK,	TGBL,	MMIS,	MNDI,	PPTC,	LDDF,	LGDF,	LDDS,	LGGS,	LDLO,
	DCHS,	HHID,	HHGO,	ECSM,	GAP1,	LDCS,	LGCS,	LTKS,	LGOP,	LGLO,
	LMAN,	LDOP,	LGDS,	DHIO,	DALL,	DLAL,	DOJD,	DOFD,	DOFO,	DOTV,
	ZOO4
	// 
};

enum
{
	id_BLU,
	id_BtmLight,
	id_OMIT,
	id_Max
};
enum
{
	Index_Port_1,
	Index_Port_2,
	Index_Port_3,
	Index_Port_4,
	Index_Port_Max
};
enum
{
	Index_Baud_110,
	Index_Baud_300,
	Index_Baud_1200,
	Index_Baud_2400,
	Index_Baud_4800,
	Index_Baud_9600,
	Index_Baud_19200,
	Index_Baud_38400,
	Index_Baud_57600,
	Index_Baud_115200,
	Index_Baud_Max
};
enum
{
	Index_DataBit_5,
	Index_DataBit_6,
	Index_DataBit_7,
	Index_DataBit_8,
	Index_DataBit_Max
};
enum
{
	Index_StopBit_10,
	Index_StopBit_15,
	Index_StopBit_20,
	Index_StopBit_Max
};
enum
{
	Index_ParityBit_Even,
	Index_ParityBit_Odd,
	Index_ParityBit_None,
	Index_ParityBit_Mark,
	Index_ParityBit_Space,
	Index_ParityBit_Max
};
enum
{
	BluModel_WT,
	BluModel_WS,
	BluModel_HD,
	BluModel_ETC,
	BluModel_Max
};
enum
{
	BtmLight_Ch1,
	BtmLight_Ch2,
	BtmLight_Ch3,
	BtmLight_Ch4,
	BtmLight_ChMax
};

enum
{
	ON_MODE = 0,
	BLINK_MODE = 1,
	OFF_MODE = 2,
	LAMP_ALL
};
#define SocketName_LC	_T("LC")
#define SocketName_VI	_T("VI")
#define SocketName_PG	_T("PG")
#define SocketName_MT	_T("MT")
#define SocketName_PLC	_T("PLC")

#define WM_LABEL_CAPTION			WM_USER+1
#define WM_UPDATE_VI_TACT			WM_USER+2
#define WM_UNSCRAP					WM_USER+3
#define WM_UPDATE_ALIGN_COUNT		WM_USER+4
#define WM_UPDATE_PROCESS			WM_USER+5
#define WM_SHOW_MSG_BOX				WM_USER+6
#define WM_CHANGE_USER				WM_USER+7
#define WM_MATCHING_DEVICE_MODEL	WM_USER+8
#define WM_UPDATE_MODEL_NAME		WM_USER+9
#define WM_UPDATE_INSPECT_COUNT     WM_USER+10//  [9/10/2009 janghan]
#define WM_SHOW_ENGINEER_DOWN_CODE	WM_USER+11	// Engineer 유실코드 입력창 표시 [1/8/2011 OSC]


// Form Auto Process Monitoring
enum 
{
	ProcLoadingReady,
	ProcLoadingDone,
	ProcGlassClampOn,
	ProcAlign,
	ProcContactOn,
	ProcPreGrab,
	ProcInspectionStart,
	ProcReleaseContact,
	ProcUnloadingReady,
	ProcUnLoadingDone,
	ProcMax
};
