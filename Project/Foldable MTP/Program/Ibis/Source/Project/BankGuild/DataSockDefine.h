
#pragma once


#define MAX_CELL_ID						150		// 2016-08-26, jhLee, 최대 Cell ID 크기는 150자까지 가능하다 (SDC 안도현 사원)

#define MAX_MES_COUNT					12
#define MAX_MES_SIZE					40



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

// 2016-06-06, jhLee, 이충현 차장에게 문의한 내용
// Q. S5F104 Current Alarm List Request 에 의하면 복수개의 Alarm을 Report하게 되어있는데, 여러개의 Alarm이 발생되는 설비가 있는가 ?
// A. 현재 우리 회사 설비는 한 순간에 1개의 Alarm만 발생되지만, 타사 설비는 복수개의 Alarm이 발생되는 경우도 있고, 
//    그런 경우를 수렴하기 위해 복수개의 Alarm을 List화하여 보고 할 수 있도록 되어있다.
//
// 향후 : CArry 등의 가변 컨테이너를 이용하여 실제 발생되는 Alarm만을 수집하여 보관하고 Clear하면 삭제하는 동적 데이터 관리로 처리한다.
//
//

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
	EQUIP_SET_UNIT_STATUS_CHANGE_LOAD,
	EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,
	EQUIP_SET_UNIT_STATUS_CHANGE_UNLOAD,
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

	//부분 검사 완료
	EQUIP_SET_CELL_ICT_ENDINS,
	EQUIP_SET_CELL_INFO_ENDINS,
	EQUIP_SET_CELL_PWR_ENDINS,
	EQUIP_SET_CELL_MTPW_ENDINS,
	EQUIP_SET_CELL_TSP_ENDINS,
	EQUIP_SET_CELL_VT_ENDINS,
	EQUIP_SET_CELL_MTPR_ENDINS,

	//TACT & CREATE & DELETE
	EQUIP_SET_CELL_BEGIN,						// 미사용
	EQUIP_SET_CELL_END,							// 미사용
	EQUIP_SET_CELL_TACT,							// 미사용

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

	EQUIP_SET_USER_LOGIN_REQUEST,   // 58, 작업자의 Login 검증을 상위 서버에 요청한다.
	EQUIP_SET_USER_LOGOUT_REQUEST,	// 59, 작업자의 Login 검증을 상위 서버에 요청한다.
	HOST_SET_USER_LOGOUT_REQUEST,   // 60, 서버에서 설비를 세우고 작업자를 강제로 Logout 시키라는 지령 메세지
	
	// 추가된 Message
	EQUIP_SET_CELL_LOT_INFORMATION_REQUEST,		// 2016-01-04, jhLee, S3F109, POCH 처리를 위한 Cell Lot Information 이슈 발생
	HOST_SET_CELL_LOT_INFORMATION,				// 2016-01-04, jhLee, S3F109, POCH 처리를 위한 Cell Lot Information 응답

	// 2016-01-20, 지정 Cell Data의 내용을 Clear 하는 요청
	EQUIP_SET_CELL_DATA_CLEAR_REQUEST,			// 지정 Cell Data의 내용을 Clear

	// 2016-02-13, jhLee, 점등 불량 LOSS 보고를 위한 Track In/Out 통합 보고 지령
	EQUIP_SET_LOSS_TRACK_REPORT,				// 특정 불량 Track In/Out 처리

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
		memset(cCellID, 0x00, sizeof(cCellID));
	}
};

#pragma pack(pop)
