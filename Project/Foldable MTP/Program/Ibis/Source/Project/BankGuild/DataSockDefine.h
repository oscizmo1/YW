
#pragma once


#define MAX_CELL_ID						150		// 2016-08-26, jhLee, �ִ� Cell ID ũ��� 150�ڱ��� �����ϴ� (SDC �ȵ��� ���)

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

// 2016-06-06, jhLee, ������ ���忡�� ������ ����
// Q. S5F104 Current Alarm List Request �� ���ϸ� �������� Alarm�� Report�ϰ� �Ǿ��ִµ�, �������� Alarm�� �߻��Ǵ� ���� �ִ°� ?
// A. ���� �츮 ȸ�� ����� �� ������ 1���� Alarm�� �߻�������, Ÿ�� ����� �������� Alarm�� �߻��Ǵ� ��쵵 �ְ�, 
//    �׷� ��츦 �����ϱ� ���� �������� Alarm�� Listȭ�Ͽ� ���� �� �� �ֵ��� �Ǿ��ִ�.
//
// ���� : CArry ���� ���� �����̳ʸ� �̿��Ͽ� ���� �߻��Ǵ� Alarm���� �����Ͽ� �����ϰ� Clear�ϸ� �����ϴ� ���� ������ ������ ó���Ѵ�.
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

	//�κ� �˻� �Ϸ�
	EQUIP_SET_CELL_ICT_ENDINS,
	EQUIP_SET_CELL_INFO_ENDINS,
	EQUIP_SET_CELL_PWR_ENDINS,
	EQUIP_SET_CELL_MTPW_ENDINS,
	EQUIP_SET_CELL_TSP_ENDINS,
	EQUIP_SET_CELL_VT_ENDINS,
	EQUIP_SET_CELL_MTPR_ENDINS,

	//TACT & CREATE & DELETE
	EQUIP_SET_CELL_BEGIN,						// �̻��
	EQUIP_SET_CELL_END,							// �̻��
	EQUIP_SET_CELL_TACT,							// �̻��

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

	// 2016-02-13, jhLee, ���� �ҷ� LOSS ���� ���� Track In/Out ���� ���� ����
	EQUIP_SET_LOSS_TRACK_REPORT,				// Ư�� �ҷ� Track In/Out ó��

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
		memset(cCellID, 0x00, sizeof(cCellID));
	}
};

#pragma pack(pop)
