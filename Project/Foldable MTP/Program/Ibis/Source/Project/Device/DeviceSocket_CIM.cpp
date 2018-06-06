#include "StdAfx.h"
#include "DeviceSocket_CIM.h"
#include "UI\Form\FormSignal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CDeviceSocket_CIM::CDeviceSocket_CIM(void)
	:CDeviceSocket_Base()
{
}


CDeviceSocket_CIM::~CDeviceSocket_CIM(void)
{
}

void CDeviceSocket_CIM::CreateSocket()
{
	if(m_Socket.IsCreated())
		m_Socket.Close();

	m_Socket.SetServer(FALSE);

	//kjpark 20170901 ���� �ڵ鷯 ����
	if(m_Socket.Create())
	{
		m_Socket.SetReciveHwnd(this->m_hParent); // ���� Dlg �ּ� �Է�

	}	

	m_Socket.SetSocketName(SOCKET_CIM);
	m_Socket.SetIP(CIM_IP);
	m_Socket.SetPort(CIM_PORT);
	m_Socket.ConnectToServer();
}

void CDeviceSocket_CIM::PreparseMessage( CDataSocket* pSocket /*= NULL*/ )
{
	if(pSocket == NULL)	return;

	// 1�� �̻� �ް� �ִ°� ������ ������ �޾Ƴ����� ����������. [11/15/2016 OSC]
	if( (m_timerCIMMsg.Stop(FALSE) > 1.) && (m_QueueCIM.GetSize() > 0) )
	{
		m_QueueCIM.ClearQueue();
	}
// 	theLog[LOG_CIM].AddBuf(_T("RECV\tSize[%d] [%s]"), pSocket->GetLastLength(), pSocket->GetLastMsg());
	m_QueueCIM.Put((char*)pSocket->GetLastMsg(), pSocket->GetLastLength());
	m_timerCIMMsg.Start();

	while(m_QueueCIM.GetSize() >= sizeof(tag_Message))
	{
		ParseDataPCMessage();
	}
}

void CDeviceSocket_CIM::ParseDataPCMessage( )
{

	char msgbuf[sizeof(tag_Message)];
	tag_Message msg;
	m_QueueCIM.Get(msgbuf, sizeof(tag_Message));
	memcpy(&msg, msgbuf, sizeof(tag_Message));
	tag_Message_Unicode msgU;
	msgU = msg;

	CCellInfo *pCell = NULL;
	switch(msg.sMsgType)
	{
	case HOST_SET_CONTROL:	
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_CONTROL] \t %s, %s"), msgU.strMsgData[9], msgU.strMsgData[10]);
		if(msgU.strMsgData[9] == CRST_REMOTE)
			theProcBank.ChangeCimState(CONST_CIM_STATE::CIM_REMOTE);
		else if(msgU.strMsgData[9] == CRST_LOCAL)
			theProcBank.ChangeCimState(CONST_CIM_STATE::CIM_LOCAL);
		else
			theProcBank.ChangeCimState(CONST_CIM_STATE::CIM_OFFLINE);
		break;

	case HOST_SET_DATAPC_VERSION:	
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_DATAPC_VERSION] \t %s | %s"), msgU.strMsgData[9], msgU.strMsgData[8]);
		theProcBank.m_strDataPCVersion.Format(_T("%s"), msgU.strMsgData[9]);
		theProcBank.m_strEqipID.Format(_T("%s"), msgU.strMsgData[8]);
		// Unit ID ����ȭ
		theUnitStatusBank.SetUnitID(theProcBank.m_strEqipID);
		theUnitStatusBank.SaveStatus();	// ID���� ������ �ʼ�
		theConfigBank.SaveLastDataFile();
		::PostMessage(GetMainHandler()->m_pGUIFrame->GetDialogBar(eDLGBAR_Top)->m_hWnd, UDMSG_CIM_EQIP_ID_CHANGE, NULL, NULL);		// ���� �̺�Ʈ
		break;

	case HOST_SET_CELL_INFORMATION:	
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_CELL_INFORMATION] \t CellID %s, ProductID %s, CellInfoResult %s"), 
			msgU.strCellID, msgU.strMsgData[2], msgU.strMsgData[3]);

		pCell = theCellBank.SearchCellInfo(msgU.strCellID, CELL_POS_SHUTTLE1_CH1, CELL_POS_SHUTTLE2_CH1);
		if(pCell)
		{
			pCell->defaultData.m_strProductID.Format(_T("%s"), msgU.strMsgData[2]);
			pCell->defaultData.m_strCellInfoResult.Format(_T("%s"), msgU.strMsgData[3]);//������ "O" ������� "41,42" -> Mes �ڵ� �ο� �� ���h ó���Ѵ�
			theLog[LOG_TRACKING].AddBuf(_T("CellID[%s], InnerID[%s], Cell Info Result[%s]"), pCell->defaultData.m_strCellID, pCell->defaultData.m_strInnerID, pCell->defaultData.m_strCellInfoResult);
			if(pCell->defaultData.m_strCellInfoResult == TEXT_0)
			{
				pCell->defaultData.m_CellInfoResult = GOOD_CELL;
			}
			else
			{
				pCell->defaultData.m_CellInfoResult = REJECT_CELL;
			}

		}
		break;

	//kjpark 20170831 Interlock, Opcoall, Terminal UI �� ��� ����
	case HOST_SET_OPCALL:	
		{
			theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_OPCALL] \t OPCallID %s, Message %s%s%s%s"), 
				msgU.strMsgData[7], msgU.strMsgData[8], msgU.strMsgData[9], msgU.strMsgData[10], msgU.strMsgData[11]);

			theProcBank.m_strOPCallID = msgU.strMsgData[7];

			CString strUnitID = msgU.strMsgData[0];
			theProcBank.m_strOPCallUnitID = strUnitID;
			//kjpark 20170710 OP Call, Interlock  �����丮 ����
			//kjpark 20180101 MAX�϶� �迭ũ�� �Ѿ�� ���� ����
			if(theProcBank.m_MsgOpCallCnt >= OPCALLMAX) break;

			theProcBank.m_strOPCallMsg[theProcBank.m_MsgOpCallCnt].Format(_T("%s%s%s%s\n\t"), msgU.strMsgData[8], msgU.strMsgData[9], msgU.strMsgData[10], msgU.strMsgData[11]);

			//kjpark 20170711 OPCall, Interlock, TERMINAL ���ȭ�鿡 �����丮 �� �߰�(������û)
			OPALL_ALARM_INFOMATION OpcallElement;
			OpcallElement.strUnitID = strUnitID;
			OpcallElement.strOpCallAlarm_NAME = msgU.strMsgData[7];
			OpcallElement.strOpCallAlarm_Message.Format(_T("%s%s%s%s"), msgU.strMsgData[8], msgU.strMsgData[9], msgU.strMsgData[10], msgU.strMsgData[11]);
			theProcBank.SetWriteOpCallLog(OpcallElement);

			theProcBank.m_MsgOpCallCnt++;

			// ���� OPCALL�� ���
			if(strUnitID == theProcBank.m_strEqipID)
			{
				if(theProcBank.m_strOldOPCallID == (_T("")))
				{
					theProcBank.m_strOldOPCallID = theProcBank.m_strOPCallID; 
					theProcBank.SetOPCall(FALSE);
				}
			}
			// Unit Interlock�� ���
			else
			{
				if(theProcBank.m_strOldOPCallID == (_T("")))
				{
					theProcBank.m_strOldOPCallID = theProcBank.m_strOPCallID; 
					theProcBank.SetOPCall(TRUE);
				}
			}
		}
		break;
	//kjpark 20170831 Interlock, Opcoall, Terminal UI �� ��� ����
	case HOST_SET_INTERLOCK:	
		{
			theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_INTERLOCK] \t InterlockID %s, Message %s%s%s%s"), 
				msgU.strMsgData[7], msgU.strMsgData[8], msgU.strMsgData[9], msgU.strMsgData[10], msgU.strMsgData[11]);

			theProcBank.m_strInterlockID = msgU.strMsgData[7];

			//kjpark 20180101 MAX�϶� �迭ũ�� �Ѿ�� ���� ����
			if(theProcBank.m_MsgInterlockCnt >= INTERLOCKMAX) break;

			theProcBank.m_strInterlockMsg[theProcBank.m_MsgInterlockCnt].Format(_T("%s%s%s%s\n\t"), msgU.strMsgData[8], msgU.strMsgData[9], msgU.strMsgData[10], msgU.strMsgData[11]);

			//kjpark 20170711 OPCall, Interlock, TERMINAL ���ȭ�鿡 �����丮 �� �߰�(������û)
			//kjpark 20170928 OPCall, Interlock. terminel MSG �����丮 ���� ����
			INTERLOCK_ALARM_INFOMATION IntelockElement;
			IntelockElement.strInterlock_ID = msgU.strMsgData[7];
			IntelockElement.strInterlock_Message.Format(_T("%s%s%s%s"), msgU.strMsgData[8], msgU.strMsgData[9], msgU.strMsgData[10], msgU.strMsgData[11]);
			theProcBank.SetWriteInterlockLog(IntelockElement);

			theProcBank.m_MsgInterlockCnt++;

			if(theProcBank.m_strOldInterlockID == (_T("")))
			{
				theProcBank.m_strOldInterlockID = theProcBank.m_strInterlockID; 
				theProcBank.SetInterlock(EFST_STEP);
			}
		}
		break;

	//kjpark 20170831 Interlock, Opcoall, Terminal UI �� ��� ����
	case HOST_SET_TERMINAL:	
		{
#if 0
			theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_TERMINAL] \t Message : %s%s%s%s"), 
				msgU.strMsgData[8], msgU.strMsgData[9], msgU.strMsgData[10], msgU.strMsgData[11]);

			theProcBank.m_strTerminalMsg.Format(_T("%s%s%s%s"), msgU.strMsgData[8], msgU.strMsgData[9], msgU.strMsgData[10], msgU.strMsgData[11]);
			theProcBank.SetTerminalMsg();
#endif
			theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_TERMINAL] \t Message %s%s%s%s"), 
				msgU.strMsgData[8], msgU.strMsgData[9], msgU.strMsgData[10], msgU.strMsgData[11]);

			theProcBank.m_strTerminalMsg.Format(_T("%s%s%s%s"), msgU.strMsgData[8], msgU.strMsgData[9], msgU.strMsgData[10], msgU.strMsgData[11]);

			//kjpark 20170711 OPCall, Interlock, TERMINAL ���ȭ�鿡 �����丮 �� �߰�(������û)
			//kjpark 20170928 OPCall, Interlock. terminel MSG �����丮 ���� ����
			TERMINEL_ALARM_INFOMATION TerminelElement;
			//kjpark 20170929 Terminel TID ���
			TerminelElement.strTerminelAlarm_NAME = msgU.strMsgData[7];
			TerminelElement.strTerminelAlarm_Message.Format(_T("%s%s%s%s"), msgU.strMsgData[8], msgU.strMsgData[9], msgU.strMsgData[10], msgU.strMsgData[11]);
			theProcBank.SetWriteTerminelLog(TerminelElement);

			theProcBank.SetTerminalMsg();
		}
		break;

	case HOST_SET_LINE_NO:	
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_LINE_NO] \t"));
		break;

	case HOST_SET_CELL_LOT_INFORMATION:	
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_CELL_LOT_INFORMATION] \t Jig %c, Ch %d, CellID %s, OptionCode %s, Count %s"), 
			_T('A') + msgU.sUnit, msgU.sCellPos+1, msgU.strCellID, msgU.strMsgData[0], msgU.strMsgData[1]);
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_CELL_LOT_INFORMATION] \t Defect1 Data1 : %s, Data2 : %s"), 
			msgU.strMsgData[2], msgU.strMsgData[3]);
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_CELL_LOT_INFORMATION] \t Defect2 Data1 : %s, Data2 : %s"), 
			msgU.strMsgData[4], msgU.strMsgData[5]);
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_CELL_LOT_INFORMATION] \t Defect3 Data1 : %s, Data2 : %s"), 
			msgU.strMsgData[6], msgU.strMsgData[7]);
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_CELL_LOT_INFORMATION] \t Defect4 Data1 : %s, Data2 : %s"), 
			msgU.strMsgData[8], msgU.strMsgData[9]);
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_CELL_LOT_INFORMATION] \t Defect5 Data1 : %s, Data2 : %s"), 
			msgU.strMsgData[10], msgU.strMsgData[11]);


		pCell = theCellBank.SearchCellInfo((JIG_ID)msgU.sUnit, (JIG_CH)msgU.sCellPos, CELL_POS_SHUTTLE1_CH1, CELL_POS_SHUTTLE2_CH1);

		if(pCell)
		{
			CString strX, strY, strPatternName;
			int nData1, nData2;
			pCell->m_nDefectPointCount = min(_tstoi(msgU.strMsgData[1].GetString()), MAX_DEFECT_POINT);

			for(int i = 0; i < MAX_DEFECT_POINT; i++)
			{
				nData1 = 2+i*2;
				nData2 = 3+i*2;
				::AfxExtractSubString( strX,			msgU.strMsgData[nData2]	, 0, ',');			
				::AfxExtractSubString( strY,			msgU.strMsgData[nData2]	, 1, ',');			
				::AfxExtractSubString( strPatternName,	msgU.strMsgData[nData2]	, 2, ',');			
				pCell->m_DefectPoint[i].nX = _tstoi(strX.GetString());
				pCell->m_DefectPoint[i].nY = _tstoi(strY.GetString());
				pCell->m_DefectPoint[i].strPatternName = strPatternName;
				pCell->m_DefectPoint[i].strDefectName = msgU.strMsgData[nData1];

				theLog[LOG_SPECIAL_PROCESS].AddBuf(_T("[JIG%c][Ch%d] CellID : %s Defect Pattern Info Download [%d,%d,%s,%s]"),
					pCell->defaultData.m_JigId+_T('A'), pCell->defaultData.m_JigCh+1, 
					pCell->defaultData.m_strCellID,
					pCell->m_DefectPoint[i].nX,
					pCell->m_DefectPoint[i].nY,
					pCell->m_DefectPoint[i].strPatternName,
					pCell->m_DefectPoint[i].strDefectName
					);
			}
		}
		else
		{
			theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_CELL_LOT_INFORMATION] Cell Not Found"));
		}
		break;
	//kjpark 20170831 Interlock, Opcoall, Terminal UI �� ��� ����
	//kjpark 20170907 Tracking CellInfomation, JobProcess �߰�
	case HOST_SET_CELL_JOB_PROCESS_STATE:
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_CELL_JOB_PROCESS_STATE] \t CellID %s, JobResult %s, StepID %s, ProductID %s"), 
			 msgU.strCellID, msgU.strMsgData[0], msgU.strMsgData[1], msgU.strMsgData[2]);
		
		pCell = theCellBank.SearchCellInfo(msgU.strCellID, CELL_POS_SHUTTLE1_CH1, CELL_POS_SHUTTLE2_CH1);
		if(pCell)
		{
			pCell->defaultData.m_strStepID = msgU.strMsgData[1];
			//kjpark 20180123 CELL INFO, JOB PROCESS Cellid �߰�
			pCell->defaultData.m_strJobProcessState = msgU.strMsgData[3];
			// Product ID�� ������ ��� �������� ������� ��찡 �ִ�
			if(msgU.strMsgData[2].GetLength() > 1)
				pCell->defaultData.m_strProductID = msgU.strMsgData[2];
			//JOBSTART
			//INVALID
			if(msgU.strMsgData[0] == _T("JOBSTART"))
			{
				//RUN
				pCell->defaultData.m_nInspectInvalidType = JOB_START;
			}
			else if(msgU.strMsgData[0] == _T("INVALID"))
			{
				//STOP
				if(theConfigBank.m_CIM.TRACKING_CONTROL_InCheck())
				{
					pCell->defaultData.m_nInspectInvalidType = JOB_INVALID;
// 					pCell->defaultData.m_bIsInspectionSkip = TRUE;
// 					pCell->defaultData.m_strLastResult = _T("INVALID CELL");
// 					pCell->defaultData.m_LastClass = REJECT_CELL;
				}			
			}
		}
		break;

	case HOST_SET_PPID_CREATE:
		// �����κ��� �ű� ������ ���� [9/27/2017 OSC]
		// CIM �������� ������ ���� RMSData.ini�� ������ �ű� ���� ����� [9/27/2017 OSC]
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_PPID_CREATE]"));
		theRecipeBank.CreateRecipe_fromRMSData(msgU.strMsgData[0]);
		break;

	case HOST_SET_EQUIP_FUNCTION_CHANGE_REQ:
		// �������� Function ���� ��û
		{
			theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_EQUIP_FUNCTION_CHANGE_REQ] EFID %s, EFST %s, Msg %s"),
				msgU.strMsgData[0], msgU.strMsgData[1], msgU.strLongMsgData[0]);

			// ���� ���� ���� �Ǵ��Ͽ� ������ ACK or NACK�� ������ �Ѵ�
			int nAck = EquipFunction_CheckChangeable(msgU.strMsgData[0], msgU.strMsgData[1]);

			// �޼��� ���� - LSH171130
			tag_Message_Unicode msgAck;
			msgAck.sMsgType = EQUIP_SET_EQUIP_FUNCTION_CHANGE_REQ_ACK;
			msgAck.sEqst = (short)nAck;
			CString strType;
			strType = _T("EQUIP_SET_EQUIP_FUNCTION_CHANGE_REQ_ACK");

			// ��Ÿ�� ���� �α׸� ����� - LSH171130
			if(SendToDataPC(msgAck))
				theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] ACK : %d"), strType, msgU.sEqst);
			else
				theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), strType);

			if(nAck == 0)
			{
				EQUIP_FUNCTION *pFunction = theConfigBank.m_CIM.GetFunction((EFID_INDEX)(_tstoi(msgU.strMsgData[0])-1));

				// EFST �����͸� Set�Ѵ� - LSH171130
				FunctionChangeBuffer.EFID = pFunction->EFID;
				FunctionChangeBuffer.EFNAME = pFunction->EFNAME;
				FunctionChangeBuffer.EFST_TYPE1 = pFunction->EFST_TYPE1;
				FunctionChangeBuffer.EFST_TYPE2 = pFunction->EFST_TYPE2;
				FunctionChangeBuffer.EFST_TYPE3 = pFunction->EFST_TYPE3;
				FunctionChangeBuffer.EFST_TYPE4 = pFunction->EFST_TYPE4;
				FunctionChangeBuffer.EFST_Set(msgU.strMsgData[1]);
				FunctionChangeBuffer.bChangeFlag = TRUE;

				//if(pFunction->EFST_Set(msgU.strMsgData[1]))
				//	SendCmdEqpFunctionChangeToDataPC(pFunction, BYWHO_HOST);
			}

		}
		break;

	case HOST_SET_INTERLOCK_REQ:
		// �������� INTERLOCK ���� ��û
		{
			msgU.strMsgData[0];	// RCMD
			CString strType = msgU.strMsgData[1];	// Interlock Type
			CString strID = msgU.strMsgData[2];	// Interlock ID
			CString strUnitID = msgU.strMsgData[3];	// EQP ID or UNIT ID
			CString strMsg = msgU.strLongMsgData[0];	// Interlock MSG
			theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_INTERLOCK_REQ] Type %s/%s, UNIT ID %s, ID %s, Msg %s"),
				msgU.strMsgData[1], theConfigBank.m_CIM.GetFunction(EFID_INTERLOCK_CONTROL)->EFST_Get(), msgU.strMsgData[3], msgU.strMsgData[2], msgU.strLongMsgData[0]);

			// ���� ���� ���� �Ǵ��Ͽ� ������ ACK or NACK�� ������ �Ѵ�
			// INTERLOCK CONTROL�� �ȸ����� NACK
			BOOL bAble = FALSE;
			if(strUnitID == theProcBank.m_strEqipID)
				bAble = TRUE;
			// Unit�̸� Interlock ��� Ȯ��
			else if(theConfigBank.m_CIM.GetFunction(EFID_INTERLOCK_CONTROL)->EFST_Get() == strType)
				bAble = TRUE;

			tag_Message_Unicode msgAck;
			msgAck.sMsgType = EQUIP_SET_INTERLOCK_REQ_ACK;
			msgAck.sEqst = bAble;
			msgAck.strMsgData[0] = msgU.strMsgData[0];	// RCMD
			CString strCMD;
			strCMD = _T("EQUIP_SET_INTERLOCK_REQ_ACK");
			if(SendToDataPC(msgAck))
			{
				theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] ACK : %d"), strCMD, msgAck.sEqst);
			}
			else
			{
				theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), strCMD);
			}

			// Interlock �߻�
			if(bAble)
			{
				INTERLOCK_ALARM_INFOMATION IntelockElement;
				IntelockElement.strInterlock_ID = strID;
				IntelockElement.strInterlock_Message = strMsg;
				IntelockElement.strUnit_ID = strUnitID;

				// ���� ���Ͷ��� ���
				if(strUnitID == theProcBank.m_strEqipID)
				{
					if(theProcBank.m_MsgInterlockCnt > INTERLOCKMAX) break;


					// ���� �߻���ų �� ������ �� �￩�д�
					if( (strType == EFST_LOADING) || (strType == EFST_TRANSFER) )
					{
						theProcBank.PreInterlock_Add(strType, IntelockElement);
					}
					else
					{
						theProcBank.SetWriteInterlockLog(IntelockElement);

						theProcBank.m_strInterlockMsg[theProcBank.m_MsgInterlockCnt] = strMsg;
						theProcBank.m_MsgInterlockCnt++;

						theProcBank.m_strOldInterlockID = theProcBank.m_strInterlockID; 
						theProcBank.m_strInterlockID = IntelockElement.strInterlock_ID;
						theProcBank.SetInterlock(EFST_STEP);
					}
				}
				// Unit Interlock�� ���
				else
				{
					if(theProcBank.m_UnitInterlockCnt > INTERLOCKMAX) break;

					theProcBank.PreUnitInterlock_Add(IntelockElement);
				}
			}
		}
		break;

	default:
		theLog[LOG_CIM].AddBuf(_T("RECV\tUndefined COD [%d]"), msg.sMsgType);
		break;

	}
}

BOOL CDeviceSocket_CIM::SendCmdToDataPC(int nMsgType, CString strData /*= _T("")*/)
{
	tag_Message_Unicode msgU;
	msgU.sMsgType = nMsgType;
	CString strType;

	switch(nMsgType)
	{
	case EQUIP_SET_CONTROL:
		strType = _T("EQUIP_SET_CONTROL");
		break;

	case EQUIP_SET_DATAPC_VERSION:
		{
			strType = _T("EQUIP_SET_DATAPC_VERSION");
			// �������� 6���ڹۿ� �� �޴´� [11/28/2017 OSC]
			CString strVersion = PROGRAM_VERSION;
			strVersion.Remove(_T('.'));
			msgU.strMsgData[0] = strVersion.Right(6);
		}
		break;

	case EQUIP_SET_OPCALL_EVENT:
		strType = _T("EQUIP_SET_OPCALL_EVENT");
		break;

	case EQUIP_SET_INTERLOCK_EVENT:
		strType = _T("EQUIP_SET_INTERLOCK_EVENT");

		// 9���� �־ ������(Gaonnuri�� 9�� �޴´�) - LSH171128
		msgU.strMsgData[9] = strData;	// Interlock Type
		break;

	case EQUIP_SET_CELL_EXIST:			// ���� Cell�� �ִ�, ���� ���� [12/1/2016 OSC]
		strType = _T("EQUIP_SET_CELL_EXIST");
		msgU.strMsgData[0] = strData;		// EQST_RUN_IS_CELL or EQST_RUN_NOT_CELL
		break;

	default:
		theLog[LOG_CIM].AddBuf(_T("SendCmdToDataPC - SEND Function Error %d"), nMsgType);
		return FALSE;
	}

	if(SendToDataPC(msgU))
	{
		theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] %s"), strType, msgU.strMsgData[0]);
		return TRUE;
	}
	else
	{
		theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), strType);
		return FALSE;
	}
}

BOOL CDeviceSocket_CIM::SendCmdCellToDataPC(int nMsgType, CCellInfo *pCell, CString strExtraData /*= _T("")*/)
{
	tag_Message_Unicode msgU;
	msgU.sMsgType = nMsgType;
	CString strType;
	CString strLog;

	switch(nMsgType)
	{
	case EQUIP_SET_CELL_DATA_CLEAR_REQUEST:		// ������ Cell�� �ʱ�ȭ�Ѵ�. ���� �̻�� [11/28/2016 OSC]
		strType = _T("EQUIP_SET_CELL_DATA_CLEAR_REQUEST");
		if(pCell == NULL)	return FALSE;

		msgU.sUnit = pCell->defaultData.m_JigId;
		msgU.sCellPos = pCell->defaultData.m_JigCh;

		strLog.Format(_T("Jig : %c, Ch : %d, InnerID : %s, CellID : %s"), 
			_T('A') + msgU.sUnit, msgU.sCellPos+1, pCell->defaultData.m_strInnerID, pCell->defaultData.m_strCellID);
		break;

	case EQUIP_SET_CELL_INFORMATION_REQUEST:
		strType = _T("EQUIP_SET_CELL_INFORMATION_REQUEST");
		if(pCell == NULL)	return FALSE;

		msgU.sUnit = pCell->defaultData.m_JigId;
		msgU.sCellPos = pCell->defaultData.m_JigCh;
		msgU.strMsgData[0] = pCell->defaultData.m_strInnerID;
		msgU.strCellID = pCell->defaultData.m_strCellID;

		strLog.Format(_T("Jig : %c, Ch : %d, InnerID : %s, CellID : %s"), 
			_T('A') + msgU.sUnit, msgU.sCellPos+1, msgU.strMsgData[0], msgU.strCellID);
		break;

	case EQUIP_SET_CELL_LOT_INFORMATION_REQUEST:
		// CIM���κ��� �߰� Data �䱸�� �� ���δ�.
		// OPTIONCODE�� �� �ִ³Ŀ����� �ҷ���ǥ, POC �� �װſ� �ش��ϴ� ������ �����´�
		strType = _T("EQUIP_SET_CELL_LOT_INFORMATION_REQUEST");
		if(pCell == NULL)	return FALSE;

		msgU.sUnit = pCell->defaultData.m_JigId;
		msgU.sCellPos = pCell->defaultData.m_JigCh;
		msgU.strMsgData[0] = strExtraData;		// OPTIONCODE 
		msgU.strCellID = pCell->defaultData.m_strCellID;

		strLog.Format(_T("Jig : %c, Ch : %d, InnerID : %s, CellID : %s, OPTIONCODE : %s"), 
			_T('A') + msgU.sUnit, msgU.sCellPos+1, pCell->defaultData.m_strInnerID, msgU.strCellID, msgU.strMsgData[0]);
		break;

	case EQUIP_SET_CELL_PROCESS_START_LOAD:	// TACK IN ���� [11/30/2016 OSC]
		strType = _T("EQUIP_SET_CELL_PROCESS_START_LOAD");
		if(pCell == NULL)	return FALSE;

		msgU.sUnit = pCell->defaultData.m_JigId;
		msgU.sCellPos = pCell->defaultData.m_JigCh;
		msgU.strCellID = pCell->defaultData.m_strCellID;
		msgU.strMsgData[0] = pCell->defaultData.m_strInnerID;	
		msgU.strMsgData[1] = pCell->defaultData.m_strStepID;	
		msgU.strMsgData[2] = pCell->defaultData.m_strProductID;		// m_strProductID	
		
		if(theConfigBank.m_CIM.TRACKING_CONTROL_InCheck())
		{
			msgU.strMsgData[6] = pCell->defaultData.m_bMCR_OK ? _T("0"):_T("1");		// ReaderResultCode �о����� 0, �ƴϸ� 1
			msgU.strMsgData[7].Format(_T("%d"), pCell->defaultData.m_JigCh+1);			// ReaderID 1~4
		}
		else
		{
			msgU.strMsgData[6] = _T("0");		// ReaderResultCode Tracking Validation�� ���ص� 0
			msgU.strMsgData[7] = _T("5");		// ReaderID, Tracking�� ���ϸ� 5
		}

		strLog.Format(_T("Jig : %c, Ch : %d, InnerID : %s, CellID : %s, ID Read : %s"), 
			_T('A') + msgU.sUnit, msgU.sCellPos+1, msgU.strMsgData[0], msgU.strCellID, msgU.strMsgData[6]);
		break;

	case EQUIP_SET_CELL_ENDINS:		// ��� �˻� �������� ���� [11/30/2016 OSC]
		strType = _T("EQUIP_SET_CELL_ENDINS");
		if(pCell == NULL)	return FALSE;
		msgU.sUnit = pCell->defaultData.m_JigId;
		msgU.sCellPos = pCell->defaultData.m_JigCh;
		msgU.strMsgData[0] = _T("Y");//pCell->defaultCellInfomation.m_strInnerID;	
		msgU.strCellID = pCell->defaultData.m_strCellID;

		//kjpark 20180129 ������å�� ��û L ���� R�� �ɼ�ó�� == ��Ÿ���� ����
		if(theConfigBank.m_Option.m_bUseNGChange_R
			&& pCell->defaultData.m_strCIMJudge == CIM_JUDGE_LOSS)
		{
			pCell->defaultData.m_strCIMJudge = CIM_JUDGE_RETEST;
		}

		msgU.strMsgData[3] = pCell->defaultData.m_strCIMJudge;		
		msgU.strMsgData[4] = pCell->defaultData.MesCode;	

		msgU.strMsgData[5] = CIM_JUDGE_GOOD;	// �۾���1�� ����
		//msgU.strMsgData[7];			// �۾���1 ������ �ҷ��ڵ�
		msgU.strMsgData[6] = CIM_JUDGE_GOOD;	// �۾���2�� ����
		//msgU.strMsgData[8];			// �۾���2 ������ �ҷ��ڵ�

		msgU.strMsgData[9] = pCell->defaultData.m_strLastResult;			// m_cDescription	

		strLog.Format(_T("Jig : %c, Ch : %d, InnerID : %s, CellID : %s, Judge %s, Code : %s, JudgeOper1 %s, CodeOper1 : %s, JudgeOper2 %s, CodeOper2 : %s, PrintName : %s"), 
			_T('A') + msgU.sUnit, msgU.sCellPos+1, pCell->defaultData.m_strInnerID, pCell->defaultData.m_strCellID, 
			msgU.strMsgData[3], msgU.strMsgData[4], 
			msgU.strMsgData[5], msgU.strMsgData[7],
			msgU.strMsgData[6], msgU.strMsgData[8], msgU.strMsgData[9]	);
		break;

	case EQUIP_SET_CELL_PROCESS_COMPLETE_UNLOAD:	// TRACK OUT ���� [11/30/2016 OSC]
		strType = _T("EQUIP_SET_CELL_PROCESS_COMPLETE_UNLOAD");
		if(pCell == NULL)	return FALSE;

		msgU.sUnit = pCell->defaultData.m_JigId;
		msgU.sCellPos = pCell->defaultData.m_JigCh;
		msgU.strCellID = pCell->defaultData.m_strCellID;
		msgU.strMsgData[0] = pCell->defaultData.m_strInnerID;	

		// TrackOut�� �� Judge ���� ���� [9/27/2017 OSC]
// 		if(pCell->defaultData.m_nInspectInvalidType == JOB_INVALID)
// 		{
// 			pCell->defaultData.m_strCIMJudge = CIM_JUDGE_OUT;
// 		}
// 		else if(strExtraData.IsEmpty() == FALSE)
		pCell->CimJudge_Judge(strExtraData);

		//kjpark 20180119 ������å�� ��û L ���� R�� �ɼ�ó��
		if(theConfigBank.m_Option.m_bUseNGChange_R
			&& pCell->defaultData.m_strCIMJudge == CIM_JUDGE_LOSS)
		{
				//kjpark 20180129 ������å�� ��û L ���� R�� �ɼ�ó�� == ��Ÿ���� ����
				pCell->defaultData.m_strCIMJudge = CIM_JUDGE_RETEST;
		}
		msgU.strMsgData[1] = pCell->defaultData.m_strCIMJudge;
		

		if(pCell->defaultData.m_strCIMJudge == CIM_JUDGE_OUT)
			msgU.strMsgData[2] = DESCRIPTION_ABORT;			// m_cDescription	
		else
			msgU.strMsgData[2] = pCell->defaultData.m_strLastResult;			// m_cDescription	

		//msgU.strMsgData[3];			// m_cDefectPos	

		msgU.strMsgData[4] = pCell->m_MTPWrite.m_DefectName;			// m_cMTPWrite	
		msgU.strMsgData[5] = pCell->defaultData.m_strProductID;		// m_strProductID	
		msgU.strMsgData[6] = pCell->defaultData.m_strStepID;			// m_strStepID	
		msgU.strMsgData[7] = pCell->defaultData.MesCode;				// MesCode	


		strLog.Format(_T("Jig : %c, Ch : %d, InnerID : %s, CellID : %s"), 
			_T('A') + msgU.sUnit, msgU.sCellPos+1, pCell->defaultData.m_strInnerID, pCell->defaultData.m_strCellID);

		pCell->defaultData.m_bTrackOutFinish = TRUE;
		break;

	default:
		theLog[LOG_CIM].AddBuf(_T("SendCellCmdToDataPC - SEND Function Error %d"), nMsgType);
		return FALSE;
	}

	if(SendToDataPC(msgU))
	{
		theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] %s"), strType, strLog);
		return TRUE;
	}
	else
	{
		theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), strType);
		return FALSE;
	}
}

BOOL CDeviceSocket_CIM::SendCmdAlarmToDataPC(ALARM_ID id, BOOL bSet, BOOL bLightAlarm)
{
	tag_Message_Unicode msgU;
	msgU.sMsgType = EQUIP_SET_ALARM;
	CString strType = _T("EQUIP_SET_ALARM");
	msgU.strMsgData[0] = bSet ? ALST_SET:ALST_CLEAR;					// Set or Reset
	msgU.strMsgData[1] = bLightAlarm ? ALCD_LIGHT:ALCD_SERIOUS;									// ��˶�(Warning) or �߾˶�(Alarm)
	msgU.strMsgData[2].Format(_T("%d"), id);							// Alarm ID
	msgU.strMsgData[3] = theProcBank.m_AlarmMsg[id].strName[LAN_ENG];	// Alarm Eng Name

	if(SendToDataPC(msgU))
	{
		theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] Set : %s, Level : %s, ID : %s, Name : %s"), 
			strType, msgU.strMsgData[0], msgU.strMsgData[1], msgU.strMsgData[2], msgU.strMsgData[3]);
		return TRUE;
	}
	else
	{
		theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), strType);
		return FALSE;
	}
}

BOOL CDeviceSocket_CIM::SendCmdStateToDataPC(int nMsgType, E_EQST_MATCH state, CString strCode)
{
	tag_Message_Unicode msgU;
	msgU.sMsgType = nMsgType;
	CString strType;
	CString strDescription = _T("");
	if(state != E_EQST_MATCH_INTERLOCK)
	{
		if(strCode.IsEmpty() == FALSE)
			strDescription = theLossTimeLog.LoadTPName(strCode);
	}

	switch(nMsgType)
	{
	case EQUIP_SET_EQUIPMENT_STATUS_CHANGE:
		strType = _T("EQUIP_SET_EQUIPMENT_STATUS_CHANGE");
		msgU.sEqst = state;
		if(state ==  E_EQST_MATCH_INTERLOCK)
		{
			msgU.strMsgData[2] = strCode;	// Interlock Type
		}
		else
		{
			msgU.strMsgData[0] = strCode;
			msgU.strMsgData[1] = strDescription;
		}
		break;

	default:
		theLog[LOG_CIM].AddBuf(_T("SendStateToDataPC - SEND Function Error %d"), nMsgType);
		return FALSE;
	}

	if(SendToDataPC(msgU))
	{
		theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] State %d"), strType, msgU.sEqst);
		return TRUE;
	}
	else
	{
		theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), strType);
		return FALSE;
	}
}

BOOL CDeviceSocket_CIM::SendCmdRecipeToDataPC( int nMsgType, CString strRecipeName, CString strOldRecipeName /*= _T("")*/ )
{
	tag_Message_Unicode msgU;
	msgU.sMsgType = nMsgType;
	CString strType;

	switch(nMsgType)
	{

	case EQUIP_SET_PPID_CREATE:
		strType = _T("EQUIP_SET_PPID_CREATE");
		msgU.strMsgData[7] = strRecipeName;	
		break;

	case EQUIP_SET_PPID_MODIFY:
		strType = _T("EQUIP_SET_PPID_MODIFY");
		msgU.strMsgData[7] = strRecipeName;	
		break;

	case EQUIP_SET_PPID_DELETE:
		strType = _T("EQUIP_SET_PPID_DELETE");
		msgU.strMsgData[7] = strRecipeName;	
		break;

	case EQUIP_SET_PPID_CHANGE:
		strType = _T("EQUIP_SET_PPID_CHANGE");
		msgU.strMsgData[7] = strOldRecipeName;	
		msgU.strMsgData[8] = strRecipeName;	
		break;

	case EQUIP_SET_PPID_OPERATION:
		strType = _T("EQUIP_SET_PPID_OPERATION");
		msgU.strMsgData[8] = strRecipeName;	
		break;

	default:
		theLog[LOG_CIM].AddBuf(_T("SendCmdRecipeToDataPC - SEND Function Error %d"), nMsgType);
		return FALSE;
	}

	if(SendToDataPC(msgU))
	{
		theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] Recipe Name : %s"), strType, msgU.strMsgData[7]);
		return TRUE;
	}
	else
	{
		theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), strType);
		return FALSE;
	}
}

BOOL CDeviceSocket_CIM::SendCmdLoginToDataPC( int nMsgType, CString strID /*= _T("")*/, CString strPW /*= _T("")*/ )
{
	tag_Message_Unicode msgU;
	msgU.sMsgType = nMsgType;
	CString strType;

	switch(nMsgType)
	{
	case EQUIP_SET_INS1_USER_LOGIN:		// GaonNuri�� ���� ������ ��� �� GaonNuri�� ID ����ȭ ���� ����ϴ� CMD [11/30/2016 OSC]
		strType = _T("EQUIP_SET_INS1_USER_LOGIN");
		msgU.strMsgData[5] = strID;	
		break;

	case EQUIP_SET_INS1_USER_LOGOUT:		// GaonNuri�� ���� ������ ��� �� GaonNuri�� ID ����ȭ ���� ����ϴ� CMD [11/30/2016 OSC]
		strType = _T("EQUIP_SET_INS1_USER_LOGOUT");
		break;

	case EQUIP_SET_INS2_USER_LOGIN:		// GaonNuri�� ���� ������ ��� �� GaonNuri�� ID ����ȭ ���� ����ϴ� CMD [11/30/2016 OSC]
		strType = _T("EQUIP_SET_INS2_USER_LOGIN");
		msgU.strMsgData[5] = strID;	
		break;

	case EQUIP_SET_INS2_USER_LOGOUT:		// GaonNuri�� ���� ������ ��� �� GaonNuri�� ID ����ȭ ���� ����ϴ� CMD [11/30/2016 OSC]
		strType = _T("EQUIP_SET_INS2_USER_LOGOUT");
		break;

	case EQUIP_SET_USER_LOGIN_REQUEST:		// MES�� User Login �㰡 ��û
		strType = _T("EQUIP_SET_USER_LOGIN_REQUEST");
		msgU.strMsgData[5] = strID;	
		msgU.strMsgData[6] = strPW;	
		break;

	case EQUIP_SET_USER_LOGOUT_REQUEST:		// MES�� User Logout �㰡 ��û
		strType = _T("EQUIP_SET_USER_LOGOUT_REQUEST");
		msgU.strMsgData[5] = strID;	
		msgU.strMsgData[6] = strPW;	
		break;

	default:
		theLog[LOG_CIM].AddBuf(_T("SendCmdLoginToDataPC - SEND Function Error %d"), nMsgType);
		return FALSE;
	}

	if(SendToDataPC(msgU))
	{
		theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] ID : %s"), strType, msgU.strMsgData[5]);
		return TRUE;
	}
	else
	{
		theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), strType);
		return FALSE;
	}
}

BOOL CDeviceSocket_CIM::SendCmdTPSystemToDataPC( CString strCode, CString strName )
{
	tag_Message_Unicode msgU;
	msgU.sMsgType = EQUIP_SET_LOSS_CODE;
	CString strType = _T("EQUIP_SET_LOSS_CODE");
	msgU.strMsgData[0] = strCode;
	msgU.strMsgData[1] = strName;

	// ���� ���µ� ���� ���� [6/14/2017 OSC]
	msgU.sEqst = E_EQST_MATCH_PAUSE;

	if(SendToDataPC(msgU))
	{
		theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] Code : %s, Name : %s"), strType, msgU.strMsgData[0], msgU.strMsgData[1]);
		return TRUE;
	}
	else
	{
		theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), strType);
		return FALSE;
	}
}

BOOL CDeviceSocket_CIM::SendToDataPC(tag_Message_Unicode msgU)
{
	msgU.sMsgClass = CLASS_CONTROL;
	tag_Message msg;
	msgU.Convert_tag_Message(&msg);
	int nLen = sizeof(tag_Message);
	int nSendLen = m_Socket.Send(&msg, nLen);
	if(nSendLen == sizeof(tag_Message))
	{
// 		theLog[LOG_CIM].AddBuf(_T("SEND OK\t[%d/%d]"), nSendLen, nLen);
		return TRUE;
	}
	else
	{
// 		theLog[LOG_CIM].AddBuf(_T("SEND NG\t[%d/%d]"), nSendLen, nLen);
		return FALSE;
	}
}

BOOL CDeviceSocket_CIM::SendCmdEqpFunctionChangeToDataPC( EQUIP_FUNCTION *pFunction, UINT nIndex, BOOL bLastIndexFlag, CString strBYWHO)
{
	// BYWHO : BYWHO_HOST, BYWHO_EQP, BYWHO_OPER, BYWHO_ETC

	tag_Message_Unicode msgU;
	msgU.sMsgType = EQUIP_SET_EQUIP_FUNCTION_CHANGE;
	msgU.strMsgData[0] = CStringSupport::N2C(pFunction->EFID);
	msgU.strMsgData[1] = pFunction->EFNAME;
	msgU.strMsgData[2] = pFunction->EFST_Get();
	msgU.strMsgData[3] = pFunction->EFST_Old;
	msgU.strMsgData[4].Format(_T("%d"),nIndex);
	msgU.strMsgData[5].Format(_T("%d"),bLastIndexFlag);
	msgU.strMsgData[6] = strBYWHO;

	if(SendToDataPC(msgU))
	{
		theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] EFID : %s, EFNAME : %s, EFST : %s, EFST_Old : %s"), 
			_T("EQUIP_SET_EQUIP_FUNCTION_CHANGE"), msgU.strMsgData[0], msgU.strMsgData[1],
			msgU.strMsgData[2], msgU.strMsgData[3]);
		return TRUE;
	}
	else
	{
		theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), _T("EQUIP_SET_EQUIP_FUNCTION_CHANGE"));
		return FALSE;
	}
}

BOOL CDeviceSocket_CIM::SendCmdEqpFunctionReportToDataPC()
{
	tag_Message_Unicode msgU;
	msgU.sMsgType = EQUIP_SET_EQUIP_FUNCTION_REPORT;

	// EFID ������ �ִ´�
	msgU.strMsgData[0] = theConfigBank.m_CIM.GetFunction(EFID_CELL_TRACKING)->EFST_Get();
	msgU.strMsgData[1] = theConfigBank.m_CIM.GetFunction(EFID_TRACKING_CONTROL)->EFST_Get();
	msgU.strMsgData[2] = theConfigBank.m_CIM.GetFunction(EFID_MATERIAL_TRACKING)->EFST_Get();
	msgU.strMsgData[3] = theConfigBank.m_CIM.GetFunction(EFID_CELL_MCR_MODE)->EFST_Get();
	msgU.strMsgData[4] = theConfigBank.m_CIM.GetFunction(EFID_MATERIAL_MCR_MODE)->EFST_Get();
	msgU.strMsgData[5] = theConfigBank.m_CIM.GetFunction(EFID_LOT_ASSIGN_INFORMATION)->EFST_Get();
	msgU.strMsgData[6] = theConfigBank.m_CIM.GetFunction(EFID_AGV_ACCESS_MODE)->EFST_Get();
	msgU.strMsgData[7] = theConfigBank.m_CIM.GetFunction(EFID_AREA_SENSOR_MODE)->EFST_Get();
	msgU.strMsgData[8] = theConfigBank.m_CIM.GetFunction(EFID_SORT_MODE)->EFST_Get();
	msgU.strMsgData[9] = theConfigBank.m_CIM.GetFunction(EFID_INTERLOCK_CONTROL)->EFST_Get();

	if(SendToDataPC(msgU))
	{
		theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] "), 
			_T("EQUIP_SET_EQUIP_FUNCTION_REPORT"));
		return TRUE;
	}
	else
	{
		theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), _T("EQUIP_SET_EQUIP_FUNCTION_CHANGE"));
		return FALSE;
	}
}

int CDeviceSocket_CIM::EquipFunction_CheckChangeable(CString strEFID, CString strEFST)
{
	// 0 : Accepted
	// 1 : Cell ID ����ġ
	// 2 : RCMD�� ���ǵȰ� �ƴ�
	// 3 : ������ ���� �ʾ� ������ �� ����
	// 4 : ������ Ư�� ������ ������ �� ����
	// 5 : ���� �ش� ����� ���� ������ �� ����
	// 6 : �̹� ���� ���� ������ �ִ�

	// ���� ��ȯ Flag - LSH171129
	int ack = 0;

	EFID_INDEX index = (EFID_INDEX)(_tstoi(strEFID)-1);
	// EFID�� �ش��ϴ� function�� �����´� - LSH171129
	EQUIP_FUNCTION * pFunction = theConfigBank.m_CIM.GetFunction(index);

	// ���񿡼� ��� �Ұ����� �ɼ� üũ
	if(theConfigBank.m_CIM.Function_CheckUse(index) == FALSE)
		ack = 5;

	if(ack != 0)
		return ack;

	ack = 2;
	// �����û�� EFST�� Functoin�� �������� Ȯ�� - LSH171129
	if(pFunction->EFST_TYPE1.IsEmpty() == FALSE)
	{
		if(pFunction->EFST_TYPE1 == strEFST)
			ack = 0;
	}
	if(pFunction->EFST_TYPE2.IsEmpty() == FALSE)
	{
		if(pFunction->EFST_TYPE2 == strEFST)
			ack = 0;
	}
	if(pFunction->EFST_TYPE3.IsEmpty() == FALSE)
	{
		if(pFunction->EFST_TYPE3 == strEFST)
			ack = 0;
	}
	if(pFunction->EFST_TYPE4.IsEmpty() == FALSE)
	{
		if(pFunction->EFST_TYPE4 == strEFST)
			ack = 0;
	}
	if(ack != 0)
		return ack;

	if(theConfigBank.m_CIM.GetFunction(index)->EFST_Get() == strEFST)
	{
		ack = 6;
	}
	if(ack != 0)
		return ack;

	if(index == EFID_CELL_MCR_MODE)
	{
		if(strEFST == EFST_NOTHING)
			ack = 3;
	}
	if(index == EFID_CELL_TRACKING)
	{
		if(theConfigBank.m_CIM.GetFunction(EFID_CELL_MCR_MODE)->EFST_Get() == EFST_USE)
		{
			if(strEFST == EFST_NOTHING)
				ack = 3;
		}
		if(theConfigBank.m_CIM.GetFunction(EFID_CELL_MCR_MODE)->EFST_Get() != EFST_USE)
		{
			if(strEFST != EFST_NOTHING)
				ack = 3;
		}
	}
	if(index == EFID_TRACKING_CONTROL)
	{
		if(theConfigBank.m_CIM.GetFunction(EFID_CELL_MCR_MODE)->EFST_Get() == EFST_USE)
		{
			if(strEFST == EFST_NOTHING)
				ack = 3;
		}
		if(theConfigBank.m_CIM.GetFunction(EFID_CELL_MCR_MODE)->EFST_Get() != EFST_USE)
		{
			if(strEFST != EFST_NOTHING)
				ack = 3;
		}
		if(theConfigBank.m_CIM.GetFunction(EFID_CELL_TRACKING)->EFST_Get() == EFST_ON)
		{
			if(strEFST == EFST_NOTHING)
				ack = 3;
		}
		if(theConfigBank.m_CIM.GetFunction(EFID_CELL_TRACKING)->EFST_Get() != EFST_ON)
		{
			if(strEFST != EFST_NOTHING)
				ack = 3;
		}
		if(strEFST == EFST_TKOUT)
			ack = 3;
		if(strEFST == EFST_BOTH)
			ack = 3;
	}
	// INTERLOCK CONTROL�� LOADING�� ��� [12/18/2017 OSC]
	if(index == EFID_INTERLOCK_CONTROL)
	{
		if(strEFST != EFST_LOADING)
			ack = 5;
	}

	return ack;
}

BOOL CDeviceSocket_CIM::SendUnitStatusReport()
{
	tag_Message_Unicode msgU;
	msgU.sMsgType = EQUIP_SET_UNIT_STATUS_REPORT;

	// ���¸� ��ä�� �Ѱ��ش�
	msgU.strMsgData[0] = theUnitStatusBank.m_Unit[JIG_ID_A][JIG_CH_1].ToString();
	msgU.strMsgData[2] = theUnitStatusBank.m_Unit[JIG_ID_B][JIG_CH_1].ToString();

	if(SendToDataPC(msgU))
	{
		theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] "), 
			_T("EQUIP_SET_UNIT_STATUS_REPORT"));
		return TRUE;
	}
	else
	{
		theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), _T("EQUIP_SET_UNIT_STATUS_REPORT"));
		return FALSE;
	}
}

BOOL CDeviceSocket_CIM::SendUnitStatusChange(JIG_ID jig, JIG_CH ch, CUnitStatus unit)
{
	tag_Message_Unicode msgU;
	msgU.sMsgType = EQUIP_SET_UNIT_STATUS_CHANGE;

	msgU.sUnit = jig;
	msgU.sCellPos = ch;
	// ���¸� ��ä�� �Ѱ��ش�
	msgU.strMsgData[0] = unit.ToString();
// 	msgU.strMsgData[0] = unit.m_strUnitID;
// 	msgU.strMsgData[1].Format(_T("%d", unit.m_AvilabilityState));
// 	msgU.strMsgData[2].Format(_T("%d", unit.m_InterlockState));
// 	msgU.strMsgData[3].Format(_T("%d", unit.m_MoveState));
// 	msgU.strMsgData[4].Format(_T("%d", unit.m_RunState));
// 	msgU.strMsgData[5].Format(_T("%d", unit.m_FrontState));
// 	msgU.strMsgData[6].Format(_T("%d", unit.m_RearState));
// 	msgU.strMsgData[7].Format(_T("%d", unit.m_PP_SplState));
// 	msgU.strMsgData[8].Format(_T("%d", unit.m_AvilabilityState));
// 	msgU.strMsgData[9] = unit.m_strResonCode;
// 	msgU.strLongMsgData[0] = unit.m_strDescrition;

	if(SendToDataPC(msgU))
	{
		theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] "), 
			_T("EQUIP_SET_UNIT_STATUS_CHANGE"));
		return TRUE;
	}
	else
	{
		theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), _T("EQUIP_SET_UNIT_STATUS_CHANGE"));
		return FALSE;
	}
}

BOOL CDeviceSocket_CIM::SendUnitStatusInterlockConfirm(JIG_ID jig, JIG_CH ch, CCellInfo *pCell)
{
	if(pCell == NULL)
		return FALSE;

	tag_Message_Unicode msgU;
	msgU.sMsgType = EQUIP_SET_UNIT_INTERLOCK_COMFIRM;

	msgU.sUnit = jig;
	msgU.sCellPos = ch;
	msgU.strMsgData[0] = theUnitStatusBank.m_Unit[jig][ch].ToString();
	msgU.strMsgData[1] = pCell->defaultData.m_strProductID;
	msgU.strMsgData[2] = pCell->defaultData.m_strStepID;
	msgU.strCellID = pCell->defaultData.m_strCellID;

	if(SendToDataPC(msgU))
	{
		theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] "), 
			_T("EQUIP_SET_UNIT_INTERLOCK_COMFIRM"));
		return TRUE;
	}
	else
	{
		theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), _T("EQUIP_SET_UNIT_INTERLOCK_EVENT"));
		return FALSE;
	}
}
