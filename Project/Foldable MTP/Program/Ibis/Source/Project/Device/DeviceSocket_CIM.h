#pragma once
#include "DeviceSocket_Base.h"
#include "SocketDefine_CIM.h"
#include "BankGuild/UnitStatus.h"

#define CIM_IP			_T("127.0.0.1")
#define CIM_PORT		6000
#define SOCKET_CIM		_T("CIM")	// 3, SONIC


class CDeviceSocket_CIM : 	public CDeviceSocket_Base
{
public:
	CDeviceSocket_CIM(void);
	~CDeviceSocket_CIM(void);

	CQueue m_QueueCIM;
	CStopWatch m_timerCIMMsg;

	EQUIP_FUNCTION FunctionChangeBuffer;

	void CreateSocket();
	void PreparseMessage(CDataSocket* pSocket = NULL);	// 유니코드 변환 및 StartChar 유무 확인하여 ParseMessage 호출
	void ParseDataPCMessage();				// 실제 Parsing

	BOOL SendCmdToDataPC(int nMsgType, CString strData = _T(""));
	BOOL SendCmdCellToDataPC(int nMsgType, CCellInfo *pCell, CString strExtraData = _T(""));
	BOOL SendCmdAlarmToDataPC(ALARM_ID id, BOOL bSet, BOOL bLightAlarm);
	BOOL SendCmdStateToDataPC(int nMsgType, E_EQST_MATCH state, CString strCode);
	BOOL SendToDataPC(tag_Message_Unicode msgU);
	BOOL SendCmdTPSystemToDataPC( CString strCode, CString strName );
	BOOL SendCmdLoginToDataPC( int nMsgType, CString strID = _T(""), CString strPW = _T("") );
	BOOL SendCmdRecipeToDataPC( int nMsgType, CString strRecipeName, CString strOldRecipeName = _T("") );

	BOOL SendCmdEqpFunctionChangeToDataPC( EQUIP_FUNCTION *pFunction, UINT nIndex, BOOL bLastIndexFlag = FALSE, CString strBYWHO = BYWHO_EQP);
	BOOL SendCmdEqpFunctionReportToDataPC( );
	int EquipFunction_CheckChangeable(CString strEFID, CString strEFST);

	BOOL SendUnitStatusReport();
	BOOL SendUnitStatusChange(JIG_ID jig, JIG_CH ch, CUnitStatus unit);
	BOOL SendUnitStatusInterlockConfirm(JIG_ID jig, JIG_CH ch, CCellInfo *pCell);
};

