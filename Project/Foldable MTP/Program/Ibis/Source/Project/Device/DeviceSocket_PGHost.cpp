#include "StdAfx.h"
#include "DeviceSocket_PGHost.h"
#include "UI\Form\FormSignal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CDeviceSocket_PGHost::CDeviceSocket_PGHost(void)
	:CDeviceSocket_Base()
{
}


CDeviceSocket_PGHost::~CDeviceSocket_PGHost(void)
{
}

void CDeviceSocket_PGHost::CreateSocket()
{
	if(m_Socket.IsCreated())
		m_Socket.Close();

	m_Socket.SetServer(TRUE);				// 서버로 설정한다.
	m_Socket.SetSocketName(SOCKET_PGPC);
	m_Socket.SetIP(PATERNGENRATOR_PC_IP);
	m_Socket.SetPort(PATERNGENRATOR_PC_PORT);

	ClientSocketName socketName;
	socketName.Name = SOCKET_PGPC;
	socketName.IP = PATERNGENRATOR_PC_IP;
	socketName.Port = PATERNGENRATOR_PC_PORT;

	m_Socket.AddClientName( socketName );
	m_Socket.SetReciveHwnd(m_hParent);

	if(m_Socket.Create(m_Socket.GetPort()))
	{
		m_Socket.Listen();											// 수신을 시작한다.
	}

}

void CDeviceSocket_PGHost::PreparseMessage( CDataSocket* pSocket /*= NULL*/ )
{
	wchar_t wszMsg[4096];
	int nMsgLen = pSocket->GetLastLength();

	if(nMsgLen > 4095) nMsgLen = 4095;

	CEtc::ConvertUnicode(pSocket->GetLastMsg(), nMsgLen, wszMsg, 4096);
	wszMsg[nMsgLen] = NULL;
	CString strMsg;
	strMsg.Format(_T("%s"), wszMsg);

	CString strLog;

	if (strMsg.Find( PG_CHECK ) == 0)		// "LineCheck" 글귀가 들어가지 않은 경우에만 Log를 남긴다.
	{			
		return;
	}
	else if (strMsg.Find( PG_LINECHECK ) == 0)		// "LineCheck" 글귀가 들어가지 않은 경우에만 Log를 남긴다.
	{			
		return;
	}
	else if (strMsg.Find( PG_LINECHECK ) == -1 && strMsg.Find( PG_CHECK ) == -1)		// "LineCheck" 글귀가 들어가지 않은 경우에만 Log를 남긴다.
	{			
		strLog.Format(_T("RECV %d\t%s"), nMsgLen, strMsg);
		theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
	}

	int i = 0;
	while (strMsg.Find(STX, 0) != -1)
	{
		if( i > 0) 
		{
			//strLog.Format(_T("%dth Parsing %d\t%s"), i+1, strMsg.GetLength(), strMsg);
			//SetAddLog(strLog);				
		}

		strMsg = ParseMessage(strMsg);
		i++;
	}

	if ( strMsg.GetLength() > 0)
	{
		//strLog.Format(_T("Parsing After %d\t%s"), strMsg.GetLength(), strMsg);
		//SetAddLog(strLog);			

	}
}

CString CDeviceSocket_PGHost::ParseMessage( CString strMsg )
{
	// 신호기에서 수신된 내용을 처리한다.
	int nIndex = strMsg.Find(STX);			// STX(시작위치)를 찾는다.
	CString strLog;

	if ( nIndex > 0)	// 0위치 이후에 STX가 발견되었다면,
	{
		strLog.Format(_T("Before STX Packet[%d] : %s"), nIndex, strMsg.Mid(0, nIndex));
		theLog[LOG_SOCKET_PG].AddBuf(strLog);		
		strMsg.Delete(0, nIndex);		// 이전 데이터는 삭제하여 무시한다.
	}


	CCellTag	tag;
	CCellInfo* pCell;

	CString strCommand = _T("");
	CString strJudge = _T("");

	int nJigNo;
	int ch;
	int nIdx;						// Cell 정보의 일련번호

	CString strTemp;
	CTokenizer t(strMsg);
	int nEtx;

	// Ack
	strTemp = t.GetNextToken(_T(","));

	if(strTemp.Find(PG_ACK) != -1)
	{
		strCommand = PG_ACK;
		t.GetNextToken(_T(","));
	}
	else if(strTemp.Find(PG_DOWN) != -1)
	{
		strCommand = PG_DOWN;
		t.GetNextToken(_T(","));
	}

	// Channel
	strTemp	= t.GetNextToken(_T(","));
	nIdx		= _tstoi(strTemp.GetString()) - 1;		// 일련번호로 들어온다. 1 ~ 4 -> 0 ~ 3으로 계산
	nJigNo	= (int)( nIdx / JIG_CH_MAX );				// Jig 번호 계산, 0 ~ 1
	ch			= (int)( nIdx % JIG_CH_MAX );				// 채널 번호 계산, 0 ~ 1
// 	ZONE_ID zone = theUnitFunc.GetAutoZoneID((JIG_ID)nJigNo);

	// Command
	if( (strCommand != PG_ACK) && (strCommand != PG_DOWN) )
	{
		strCommand = t.GetNextToken(_T(","));
		nEtx = strCommand.Find(ETX);

		if (nEtx > 5)	strCommand = strCommand.Mid(0, nEtx-4);

		// 쓸데없는건 Parsing하지 말자..
		if( (strCommand == PG_LINECHECK) )
		{
			return PacketClear(strMsg);
		}
	}

	nEtx = strMsg.Find(ETX);
	if(nEtx > 0)
		strLog.Format(_T("RECV %s"), strMsg.Left(strMsg.Find(ETX)+1));
	else
		strLog.Format(_T("RECV %s"), strMsg);
	SetAddLog(strLog);


	// Judge
	strJudge = t.GetNextToken(_T(","));

	nEtx = strJudge.Find(ETX);
	if (nEtx > 5)
	{
		strJudge = strJudge.Mid(0, nEtx-4);
	}

	// Force Manual Sequence에서 사용하기 위해... [6/16/2017 OSC]
	if(theProcBank.m_bEnableManualStateData && theProcBank.MachineIsRunState() == FALSE)
	{
		pCell = &theProcBank.m_ManualCell[ch];
	}
	// Inline모드시 Cell Loading은 Cell Data 생성 전이므로 따로 받는다 [12/19/2017 OSC]
	else if( strCommand == CELL_LOADING )
	{
		pCell = &theCellBank.m_InfoCellLoading[nJigNo][ch];
	}
	else
	{
		tag = theCellBank.SearchCellTag((JIG_ID)nJigNo, (JIG_CH)ch);

		if (tag.IsExist() == FALSE)		// 대상 Cell 정보를 찾지 못하면 더이상의 처리는 무의미하다 !
		{			
			if ( (strCommand == PG_RUN) && (strJudge == SET_ZONE_A) )
			{
				// ZONE_A는 셀 없어도 받아야 하므로 예외 [10/21/2017 OSC]
			}
			else
			{
				strLog.Format(_T("%s <==== %s ** Cell Not Found Shuttle:%c Ch:%d Msg:%s"), SOCKET_MAIN, SOCKET_PGPC, _T('A')+nJigNo+1, ch+1, strMsg);
				theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
				// 두개 붙어서 나올 것을 대비해 다음 STX까지 지운다
				return PacketClear(strMsg);
			}
		}

		pCell = theCellBank.GetCellInfo(tag);
	}

	// 아래 명령을 제외한 처리
	if(	(	TMD_INFO != strCommand) 
		&& (SET_ZONE_A != strCommand) 
		&& (SET_ZONE_B != strCommand)
		&& (SET_ZONE_C != strCommand) 
		&& (SET_ZONE_D != strCommand) 
		&& (PG_ACK != strCommand) 
		&& (PG_RUN != strCommand) )
	{
		strLog.Format(_T("%s <==== %s		Shuttle:%c Ch:%d InnerID:%s CellID:%s Command:%s Judge:%s"), 
			SOCKET_MAIN, SOCKET_PGPC, _T('A')+nJigNo+1, ch+1, pCell->defaultData.m_strInnerID, pCell->defaultData.m_strCellID, strCommand, strJudge);
		theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		//m_pLogPG->Log( 

	}//of if 

	// 지정 명령에대한 수신 결과로 선두가 ACK로 날라오는 경우
	if ( strCommand == PG_ACK )	// ACK로 날라오는거는 필요한 것만 Cell Search한다 [6/20/2013 OSC]
	{
		PatternGeneratorACK(pCell, strJudge);
	}
#ifdef NOT_USED
	else if(strCommand == FORCE_START1_BEF)
	{
		if(pCell->m_Zone_C.BZoneForceStart1BefClass == CELL_LOG_NONE)
		{
			if( (zone == ZONE_ID_B) || theProcBank.m_bEnableManualStateData )
			{
				if(strJudge == PG_JUDGE_GOOD)
				{
					pCell->m_Zone_C.BZoneForceStart1BefClass = CELL_LOG_OK;
					pCell->m_Zone_C.m_strBZoneDefectForceStart1Bef = strJudge;
				}
				else
				{
					pCell->m_Zone_C.BZoneForceStart1BefClass = CELL_LOG_NG;
					if(strJudge == NG)
					{
						strTemp = t.GetNextToken(_T(","));
						nEtx = strTemp.Find(ETX);
						if(nEtx > 5)
							strTemp = strTemp.Mid(0, nEtx-4);
					}
					else
					{
						strTemp = strJudge;
					}
					pCell->m_Zone_C.m_strBZoneDefectForceStart1Bef = strTemp;
					pCell->defaultData.m_bIsInspectionSkip = TRUE;
				}

				strLog.Format(_T("%s <==== %s Shuttle:%c Ch:%d InnerID:%s CellID:%s Command:%s Judge:%s"), 
					SOCKET_MAIN, SOCKET_PGPC, _T('A')+nJigNo+1, ch+1, pCell->defaultData.m_strInnerID, pCell->defaultData.m_strCellID, strCommand, strJudge);
				theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
			}
			else
			{
				theLog[LOG_SOCKET_ERR].AddBuf(_T("Zone Miss Match Command[%s] Now Zone[%c]"), strCommand, _T('A') + theUnitFunc.GetAutoZoneID((JIG_ID)nJigNo));
			}
		}

	}
	else if(strCommand == FORCE_START2_BEF)
	{
		if( (zone == ZONE_ID_B) && (pCell->m_Zone_C.BZoneForceStart2BefClass == CELL_LOG_NONE) )
		{
			if(strJudge == PG_JUDGE_GOOD)
			{
				pCell->m_Zone_C.BZoneForceStart2BefClass = CELL_LOG_OK;
				pCell->m_Zone_C.m_strBZoneDefectForceStart2Bef = strJudge;
			}
			else
			{
				pCell->m_Zone_C.BZoneForceStart2BefClass = CELL_LOG_NG;
				if(strJudge == NG)
				{
					strTemp = t.GetNextToken(_T(","));
					nEtx = strTemp.Find(ETX);
					if(nEtx > 5)
						strTemp = strTemp.Mid(0, nEtx-4);
				}
				else
				{
					strTemp = strJudge;
				}
				pCell->m_Zone_C.m_strBZoneDefectForceStart2Bef = strTemp;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}

			strLog.Format(_T("%s <==== %s Shuttle:%c Ch:%d InnerID:%s CellID:%s Command:%s Judge:%s"), 
				SOCKET_MAIN, SOCKET_PGPC, _T('A')+nJigNo+1, ch+1, pCell->defaultData.m_strInnerID, pCell->defaultData.m_strCellID, strCommand, strJudge);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		}
		else
		{
			theLog[LOG_SOCKET_ERR].AddBuf(_T("Zone Miss Match Command[%s] Now Zone[%c]"), strCommand, _T('A') + theUnitFunc.GetAutoZoneID((JIG_ID)nJigNo));
		}

	}
	else if(strCommand == FORCE_START3_BEF)
	{
		if( (zone == ZONE_ID_B) && (pCell->m_Zone_C.BZoneForceStart3BefClass == CELL_LOG_NONE) )
		{
			if(strJudge == PG_JUDGE_GOOD)
			{
				pCell->m_Zone_C.BZoneForceStart3BefClass = CELL_LOG_OK;
				pCell->m_Zone_C.m_strBZoneDefectForceStart3Bef = strJudge;
			}
			else
			{
				pCell->m_Zone_C.BZoneForceStart3BefClass = CELL_LOG_NG;
				if(strJudge == NG)
				{
					strTemp = t.GetNextToken(_T(","));
					nEtx = strTemp.Find(ETX);
					if(nEtx > 5)
						strTemp = strTemp.Mid(0, nEtx-4);
				}
				else
				{
					strTemp = strJudge;
				}
				pCell->m_Zone_C.m_strBZoneDefectForceStart3Bef = strTemp;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}

			strLog.Format(_T("%s <==== %s Shuttle:%c Ch:%d InnerID:%s CellID:%s Command:%s Judge:%s"), 
				SOCKET_MAIN, SOCKET_PGPC, _T('A')+nJigNo+1, ch+1, pCell->defaultData.m_strInnerID, pCell->defaultData.m_strCellID, strCommand, strJudge);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		}
		else
		{
			theLog[LOG_SOCKET_ERR].AddBuf(_T("Zone Miss Match Command[%s] Now Zone[%c]"), strCommand, _T('A') + theUnitFunc.GetAutoZoneID((JIG_ID)nJigNo));
		}

	}
	//kjpark 20161114 C Zone Foce 추가
	else if(strCommand == FORCE_START1_AFT)
	{
		//P0016Ch,15,FORCE_START,GOODB97B
		//P002BCh,15,FORCE_START,NG,TSP CM BAND(FORCE_RAW)E418		
		if(pCell->m_Zone_C.BZoneForceStart1Class == CELL_LOG_NONE)
		{
			if( (zone == ZONE_ID_B) || theProcBank.m_bEnableManualStateData )
			{
				if(PG_JUDGE_GOOD == strJudge)
				{
					pCell->m_Zone_C.BZoneForceStart1Class = CELL_LOG_OK;
					pCell->m_Zone_C.m_strBZoneDefectForceStart1 = strJudge;
				}
				else
				{
					pCell->m_Zone_C.BZoneForceStart1Class = CELL_LOG_NG;
					if(strJudge == NG)
					{
						strTemp = t.GetNextToken(_T(","));
						nEtx = strTemp.Find(ETX);
						if(nEtx > 5)
							strTemp = strTemp.Mid(0, nEtx-4);
					}
					else
					{
						strTemp = strJudge;
					}
					pCell->m_Zone_C.m_strBZoneDefectForceStart1 = strTemp;
					pCell->defaultData.m_bIsInspectionSkip = TRUE;
				}

				GetLocalTime(&pCell->cellTime.systmC_ForceStart1End);
				strLog.Format(_T("%s <==== %s Shuttle:%c Ch:%d InnerID:%s CellID:%s Command:%s Judge:%s"), 
					SOCKET_MAIN, SOCKET_PGPC, _T('A')+nJigNo+1, ch+1, pCell->defaultData.m_strInnerID, pCell->defaultData.m_strCellID, strCommand, strJudge);
				theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
			}
			else
			{
				theLog[LOG_SOCKET_ERR].AddBuf(_T("Zone Miss Match Command[%s] Now Zone[%c]"), strCommand, _T('A') + theUnitFunc.GetAutoZoneID((JIG_ID)nJigNo));
			}
		}
	}
	//kjpark 20161114 C Zone Foce 추가
	else if(strCommand == FORCE_START2_AFT)
	{
		if( (zone == ZONE_ID_B) && (pCell->m_Zone_C.BZoneForceStart2Class == CELL_LOG_NONE) )
		{
			if(PG_JUDGE_GOOD == strJudge)
			{
				pCell->m_Zone_C.BZoneForceStart2Class = CELL_LOG_OK;
				pCell->m_Zone_C.m_strBZoneDefectForceStart2 = strJudge;
			}
			else
			{
				pCell->m_Zone_C.BZoneForceStart2Class = CELL_LOG_NG;
				if(strJudge == NG)
				{
					strTemp = t.GetNextToken(_T(","));
					nEtx = strTemp.Find(ETX);
					if(nEtx > 5)
						strTemp = strTemp.Mid(0, nEtx-4);
				}
				else
				{
					strTemp = strJudge;
				}
				pCell->m_Zone_C.m_strBZoneDefectForceStart2 = strTemp;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
			GetLocalTime(&pCell->cellTime.systmC_ForceStart2End);
			strLog.Format(_T("%s <==== %s Shuttle:%c Ch:%d InnerID:%s CellID:%s Command:%s Judge:%s"), 
				SOCKET_MAIN, SOCKET_PGPC, _T('A')+nJigNo+1, ch+1, pCell->defaultData.m_strInnerID, pCell->defaultData.m_strCellID, strCommand, strJudge);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		}
		else
		{
			theLog[LOG_SOCKET_ERR].AddBuf(_T("Zone Miss Match Command[%s] Now Zone[%c]"), strCommand, _T('A') + theUnitFunc.GetAutoZoneID((JIG_ID)nJigNo));
		}
	}
	//kjpark 20161114 C Zone Foce 추가
	else if(strCommand == FORCE_START3_AFT)
	{
		if( (zone == ZONE_ID_B) && (pCell->m_Zone_C.BZoneForceStart3Class == CELL_LOG_NONE) )
		{
			if(PG_JUDGE_GOOD == strJudge)
			{
				pCell->m_Zone_C.BZoneForceStart3Class = CELL_LOG_OK;
				pCell->m_Zone_C.m_strBZoneDefectForceStart3 = strJudge;
			}
			else
			{
				pCell->m_Zone_C.BZoneForceStart3Class = CELL_LOG_NG;
				if(strJudge == NG)
				{
					strTemp = t.GetNextToken(_T(","));
					nEtx = strTemp.Find(ETX);
					if(nEtx > 5)
						strTemp = strTemp.Mid(0, nEtx-4);
				}
				else
				{
					strTemp = strJudge;
				}
				pCell->m_Zone_C.m_strBZoneDefectForceStart3 = strTemp;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
			GetLocalTime(&pCell->cellTime.systmC_ForceStart3End);
			strLog.Format(_T("%s <==== %s Shuttle:%c Ch:%d InnerID:%s CellID:%s Command:%s Judge:%s"), 
				SOCKET_MAIN, SOCKET_PGPC, _T('A')+nJigNo+1, ch+1, pCell->defaultData.m_strInnerID, pCell->defaultData.m_strCellID, strCommand, strJudge);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		}
		else
		{
			theLog[LOG_SOCKET_ERR].AddBuf(_T("Zone Miss Match Command[%s] Now Zone[%c]"), strCommand, _T('A') + theUnitFunc.GetAutoZoneID((JIG_ID)nJigNo));
		}
	}
#endif
	else if( strCommand == PG_RUN )	// 지정 명령에 대한 수행 결과가 수신되는 경우
	{
		if( strJudge == SET_ZONE_A)
			theProcBank.m_AZone[nJigNo][ch].m_RecvSetZone = TRUE;
// 			pCell->m_AZone.m_RecvSetZone = TRUE;
		else if( strJudge == SET_ZONE_C)
			pCell->m_BZone.m_RecvSetZone = TRUE;
	}
	else if( strCommand == PG_DOWN )
	{
		//P0013DOWN,Ch,11,TMD_INFO8502
		if( strJudge == TMD_INFO )
			pCell->defaultData.m_bReceive_TMD_Info = TMD_INFO_DOWN;
	}
	else if ( (strCommand == PG_SYSTEM_ERROR)
		|| (strCommand == CURRENT_ALRAM) || (strCommand == VOLTAGE_ALRAM) ) // 신호기 System Error 발생
	{
		pCell->defaultData.m_bPGAlarm = TRUE;
		pCell->defaultData.m_strPGAlarmName = strCommand;
// 		pCell->defaultCellInfomation.m_PGAlarmZone = theUnitFunc.GetAutoZoneID((JIG_ID)nJigNo);
		pCell->defaultData.m_bIsInspectionSkip = TRUE;
	}
	else if( strCommand == TMD_INFO )
	{
		// Cell Data 받기 전에 수신되기에 따로 받아뒀다가 Cell Loading 수신시 넣는다. [6/8/2013 OSC]
		// TMD_Info
		pCell->defaultData.m_strTMD_Info = strJudge;
		pCell->defaultData.m_strPG_Client_Version = t.GetNextToken(_T(","));
		strTemp = t.GetNextToken(_T(","), 9);
		nEtx = strTemp.Find(ETX);
		if(nEtx > 5)
			strTemp = strTemp.Mid(0, nEtx-4);
		pCell->defaultData.m_strPG_MAC_Address = strTemp;
		pCell->defaultData.m_bReceive_TMD_Info = TMD_INFO_RECEIVE;
		strLog.Format(_T("%s <==== %s Jig %d, Ch %d, TMD : %s, Client : %s, MAC Address : %s"), SOCKET_MAIN, SOCKET_PGPC, _T('A')+nJigNo+1, ch+1, pCell->defaultData.m_strTMD_Info, pCell->defaultData.m_strPG_Client_Version, pCell->defaultData.m_strPG_MAC_Address);
		theLog[LOG_OP2PG_PACKET].AddBuf(strLog);

	}
	//kjpark 20180113 Tmd Version 추가
	//kjpark 20180113 Tmd Version 추가 테스트 완료
	//kjpark 20180117 Tmd Version 추가 테스트 완료(강원호 선임 확인)
	//20180312 m_strTXFPGAVersion  ->  m_strTXCPLDVersion 수정
	else if( strCommand == CLIENT_VER )
	{
		CString strFPGA_Ver, strTemp;
		strTemp = t.GetNextToken(_T(","));//FPGA VERSION
		pCell->defaultData.m_strTXCPLDVersion = t.GetNextToken(_T("OS_VER"));
		pCell->defaultData.m_strTXCPLDVersion.Replace(_T(","), _T("."));
		pCell->defaultData.m_strTXCPLDVersion = pCell->defaultData.m_strTXCPLDVersion.Mid(0, pCell->defaultData.m_strTXCPLDVersion.GetLength()-1);
		
		strTemp = t.GetNextToken(_T(","));		
		strTemp = t.GetNextToken(_T(","));
		nEtx = strTemp.Find(ETX);
		if(nEtx > 5)
			strTemp = strTemp.Mid(0, nEtx-4);
		pCell->defaultData.m_strTXOSVersion = strTemp;
		strLog.Format(_T("%s <==== %s Jig %d, Ch %d, FPGA Version : %s, CPLD Version : %s, OS Version : %s"), SOCKET_MAIN, SOCKET_PGPC, _T('A')+nJigNo+1, ch+1, pCell->defaultData.m_strTXCPLDVersion, pCell->defaultData.m_strTXCPLDVersion, pCell->defaultData.m_strTXOSVersion);
		theLog[LOG_OP2PG_PACKET].AddBuf(strLog);

	}
	//kjpark 20180113 Tmd Version 추가
	else if( strCommand == HOST_VER )
	{
		//kjpark 20180113 Tmd Version 추가 테스트 완료
		pCell->defaultData.m_strPG_Host_Version = strJudge;
		strLog.Format(_T("%s <==== %s Jig %d, Ch %d, HOST Version : %s"), SOCKET_MAIN, SOCKET_PGPC, _T('A')+nJigNo+1, ch+1, pCell->defaultData.m_strPG_Host_Version);
		theLog[LOG_OP2PG_PACKET].AddBuf(strLog);

	}
	else if (CELL_LOADING == strCommand)
	{
		// Cell Loading 날린 적이 한번 이상 있어야 처리 [1/17/2017 OSC]
		if(pCell->m_CellLoading.m_CellLoadingSend)
		{
			pCell->m_CellLoading.m_ContactCount++;
			// Judge
			if(PG_JUDGE_GOOD == strJudge)
			{
				pCell->m_CellLoading.m_DefectName = strJudge;
				pCell->m_CellLoading.m_Class = GOOD_CELL;

				// 전류값 총 6개 : IBAT, IVSS, IVDD, IVCI, IBAT2, IDD2
				CString strTemp = t.GetNextToken(_T(","), 3);
				pCell->m_CellLoading.m_CurrentText[IBAT] = strTemp.GetString();					
				for (int i=IVSS ; i<DEFECTINFO_CURRENT_MAX ; i++)
				{
					// Title하고 같이 있으니 1칸 넘어감
					strTemp = t.GetNextToken(_T(","), 2);
					nEtx = strTemp.Find(ETX);
					if(nEtx > 5)
					{
						strTemp = strTemp.Mid(0, nEtx-4);
					}
					pCell->m_CellLoading.m_CurrentText[i] = strTemp;
				}
				pCell->m_CellLoading.Text_to_Value();	// 전류값 string 변수를 double 변수에 옮긴다

				// 컨택하는 구간은 작업자 구간이므로 다시 조치될 수도 있으니 컨택 성공하면 이전 알람상태를 해제해준다 [9/11/2017 OSC]
				pCell->defaultData.m_bPGAlarm = FALSE;
				pCell->defaultData.m_strPGAlarmName.Empty();
				pCell->defaultData.m_bIsInspectionSkip = FALSE;
			}
			else
			{
				pCell->m_CellLoading.m_DefectName = pCell->m_CellLoading.m_strCommand + TEXT_DEFECT_NG;
				pCell->m_CellLoading.m_Class = REJECT_CELL;
				if(theConfigBank.m_System.m_bInlineMode)
					pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
			if(pCell->m_CellLoading.m_ContactCount == 1)
				pCell->m_CellLoading.m_FirstClass = pCell->m_CellLoading.m_Class;
		}

	}	
	//20170306 kjpark WhiteCurrent ADD in Celllog
	else if(WHITE_CURRENT_CHECK == strCommand)
	{
		if(pCell->m_WhiteCurrent.m_Class == NONE_CELL)
		{
			if( (strJudge == PG_JUDGE_GOOD) || (strJudge == PG_JUDGE_NG) )
			{
				if(strJudge == PG_JUDGE_GOOD)
				{
					pCell->m_WhiteCurrent.m_Class = GOOD_CELL;
					pCell->m_WhiteCurrent.m_DefectName = strJudge;
				}
				else
				{
					pCell->m_WhiteCurrent.m_Class = REJECT_CELL;
					pCell->m_WhiteCurrent.m_DefectName = pCell->m_WhiteCurrent.m_strCommand + TEXT_DEFECT_NG;//TEXT_I_BAT_OVER;
					pCell->defaultData.m_bIsInspectionSkip = TRUE;
				}


				// 전류값 총 6개 : IBAT, IVSS, IVDD, IVCI, IBAT2, IDD2
				CString strTemp = t.GetNextToken(_T(","), 3);
				pCell->m_WhiteCurrent.m_CurrentText[IBAT] = strTemp.GetString();					
				for (int i=IVSS ; i<DEFECTINFO_CURRENT_MAX ; i++)
				{
					// Title하고 같이 있으니 1칸 넘어감
					strTemp = t.GetNextToken(_T(","), 2);
					nEtx = strTemp.Find(ETX);
					if(nEtx > 5)
					{
						strTemp = strTemp.Mid(0, nEtx-4);
					}
					pCell->m_WhiteCurrent.m_CurrentText[i] = strTemp;
				}
				pCell->m_WhiteCurrent.Text_to_Value();	// 전류값 string 변수를 double 변수에 옮긴다
				pCell->m_WhiteCurrent.SetTimeEnd();

			}
		}
	}
	else if(TSP_START == strCommand)
	{
		if( pCell->m_TSPStart.m_Class == NONE_CELL )
		{
			if(PG_JUDGE_GOOD == strJudge)
			{
				pCell->m_TSPStart.m_DefectName = strJudge;

				pCell->m_TSPStart.m_Class = GOOD_CELL;
			}
			else
			{
				pCell->m_TSPStart.m_Class = REJECT_CELL;
				if(strJudge == PG_JUDGE_NG)
				{
					strTemp = t.GetNextToken(_T(","));
					nEtx = strTemp.Find(ETX);
					if(nEtx > 5)
						strTemp = strTemp.Mid(0, nEtx-4);

					pCell->m_TSPStart.m_DefectName = strTemp;
				}
				else
				{
					pCell->m_TSPStart.m_DefectName = strJudge;

						// Data 개수 (다음 쉼표 갯수 == TSP Count)
						int nCycle = _tstoi(t.GetNextToken(_T(",")));

						if(nCycle > MAX_TSP_RESULT)
						{
							theLog[LOG_SEQUENCE].AddBuf(_T("%s <==== %s TSP count(%d) is over MAX(%d)"), SOCKET_MAIN, SOCKET_PGPC, nCycle, MAX_TSP_RESULT);
							nCycle = MAX_TSP_RESULT;
						}

						int i = 0;
						for (i=0 ; i<nCycle ; i++)
						{
							pCell->m_TSPStart.TspTitle[i] = t.GetNextToken(_T(","));
							pCell->m_TSPStart.TspName[i] = t.GetNextToken(_T(","));
						}
						i--;
						nEtx = pCell->m_TSPStart.TspName[i].Find(ETX);
						if(nEtx > 5)
							pCell->m_TSPStart.TspName[i] = pCell->m_TSPStart.TspName[i].Mid(0, nEtx-4);
				}
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}

			strLog.Format(_T("%s <==== %s %s, Shuttle:%c, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, _T('A')+nJigNo+1, ch+1, strJudge);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
			pCell->m_TSPStart.SetTimeEnd();
		}
	}
	else if(strCommand == ZONE_FIN)
	{
// 		if(zone == ZONE_ID_B)
// 		{
// 			
// 		}
// 		else if(zone == ZONE_ID_D)
// 		{			
// 			strLog.Format(_T("%s <==== %s %s, Shuttle:%c, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, _T('A')+nJigNo+1, ch+1, strJudge);
// 			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
// 			GetLocalTime(&pCell->cellTime.systmD_ZoneFinEndTime);
// 		}
// 		else
// 		{
// 			theLog[LOG_SOCKET_ERR].AddBuf(_T("Zone Miss Match Command[%s] Now Zone[%c]"), strCommand, _T('A') + theUnitFunc.GetAutoZoneID((JIG_ID)nJigNo));
// 		}
	}
	//kjpark 20170908 COMMAD 별 Receive 추가
	else if ( strCommand == TE_CHECK )		// TE_CHECK에 대한 판정 결과
	{
	//kjpark 20170908 COMMAD 별 Receive 추가
		if(pCell->m_TECheck.m_Class == NONE_CELL)
		{
			if(strJudge == PG_JUDGE_GOOD)
			{

				pCell->m_TECheck.m_Class = GOOD_CELL;		
				pCell->m_TECheck.m_DefectName = GOOD;
			}
			else
			{
				pCell->m_TECheck.m_Class = REJECT_CELL;
				pCell->m_TECheck.m_DefectName = pCell->m_TECheck.m_strCommand + TEXT_DEFECT_NG;
			}
			strLog.Format(_T("%s <==== %s %s, Shuttle:%c, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, _T('A')+nJigNo+1, ch+1, strJudge);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		}
		pCell->m_TECheck.SetTimeEnd();
	}
	//kjpark 20170908 COMMAD 별 Receive 추가
	else if ( strCommand == ID_CHECK )
	{
		
		if(pCell->m_IDCheck.m_Class == NONE_CELL)
		{
			if(strJudge == PG_JUDGE_GOOD)
			{
				pCell->m_IDCheck.m_Class = GOOD_CELL;
				pCell->m_IDCheck.m_DefectName = GOOD;
			}
			else
			{
				pCell->m_IDCheck.m_Class = REJECT_CELL;
				pCell->m_IDCheck.m_DefectName = pCell->m_IDCheck.m_strCommand + TEXT_DEFECT_NG;
			}
			strLog.Format(_T("%s <==== %s %s, Shuttle:%c, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, _T('A')+nJigNo+1, ch+1, strJudge);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		}
		pCell->m_IDCheck.SetTimeEnd();
	} 
	//kjpark 20170908 COMMAD 별 Receive 추가
	else if ( strCommand == OTP_REG_CHECK )
	{
		if(pCell->m_OTPREGCheck.m_Class == NONE_CELL)
		{
			if(strJudge == PG_JUDGE_GOOD)
			{
				pCell->m_OTPREGCheck.m_Class = GOOD_CELL;
				pCell->m_OTPREGCheck.m_DefectName = GOOD;

			}
			else
			{
				pCell->m_OTPREGCheck.m_Class = REJECT_CELL;
				pCell->m_OTPREGCheck.m_DefectName = pCell->m_OTPREGCheck.m_strCommand + TEXT_DEFECT_NG;
			}
			strLog.Format(_T("%s <==== %s %s, Shuttle:%c, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, _T('A')+nJigNo+1, ch+1, strJudge);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		}
		pCell->m_OTPREGCheck.SetTimeEnd();
	}
	//kjpark 20170908 COMMAD 별 Receive 추가
	else if ( strCommand == COPR_ICT_TEST )
	{
		if(pCell->m_COPRICTTest.m_Class == NONE_CELL)
		{
			if(strJudge == PG_JUDGE_GOOD)
			{

				pCell->m_COPRICTTest.m_Class = GOOD_CELL;	
				pCell->m_COPRICTTest.m_DefectName = GOOD;			
			}
			else
			{
				pCell->m_COPRICTTest.m_Class = REJECT_CELL;
				pCell->m_COPRICTTest.m_DefectName = pCell->m_COPRICTTest.m_strCommand + TEXT_DEFECT_NG;
			}
			strLog.Format(_T("%s <==== %s %s, Shuttle:%c, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, _T('A')+nJigNo+1, ch+1, strJudge);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		}
		pCell->m_COPRICTTest.SetTimeEnd();
	}
	//kjpark 20170908 COMMAD 별 Receive 추가
	else if ( strCommand == POC_ERROR_CHECK )
	{
		if(pCell->m_PocErrorCheck.m_Class == NONE_CELL)
		{
			if(strJudge == PG_JUDGE_GOOD)
			{

				pCell->m_PocErrorCheck.m_Class = GOOD_CELL;	
				pCell->m_PocErrorCheck.m_DefectName = GOOD;			
			}
			else
			{
				pCell->m_PocErrorCheck.m_Class = REJECT_CELL;
				pCell->m_PocErrorCheck.m_DefectName = pCell->m_PocErrorCheck.m_strCommand + TEXT_DEFECT_NG;
			}
			strLog.Format(_T("%s <==== %s %s, Shuttle:%c, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, _T('A')+nJigNo+1, ch+1, strJudge);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		}
		pCell->m_PocErrorCheck.SetTimeEnd();
	}
	//kjpark 20170908 COMMAD 별 Receive 추가
	else if ( strCommand == DDI_BLOCK_TEST )
	{
		if(pCell->m_DDIBlockTest.m_Class == NONE_CELL)
		{
			if(strJudge == PG_JUDGE_GOOD)
			{

				pCell->m_DDIBlockTest.m_Class = GOOD_CELL;	
				pCell->m_DDIBlockTest.m_DefectName = GOOD;			
			}
			else
			{
				pCell->m_DDIBlockTest.m_Class = REJECT_CELL;
				pCell->m_DDIBlockTest.m_DefectName = pCell->m_DDIBlockTest.m_strCommand + TEXT_DEFECT_NG;
			}
			strLog.Format(_T("%s <==== %s %s, Shuttle:%c, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, _T('A')+nJigNo+1, ch+1, strJudge);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		}
		pCell->m_DDIBlockTest.SetTimeEnd();
	}
	//kjpark 20170908 COMMAD 별 Receive 추가
	else if ( strCommand == EVT_VERSION_CHECK )
	{
		if(pCell->m_EVTVersionCheck.m_Class == NONE_CELL)
		{
			if(strJudge == PG_JUDGE_GOOD)
			{

				pCell->m_EVTVersionCheck.m_Class = GOOD_CELL;
				pCell->m_EVTVersionCheck.m_DefectName = GOOD;				
			}
			else
			{
				pCell->m_EVTVersionCheck.m_Class = REJECT_CELL;
				pCell->m_EVTVersionCheck.m_DefectName = pCell->m_EVTVersionCheck.m_strCommand + TEXT_DEFECT_NG;
			}
			strLog.Format(_T("%s <==== %s %s, Shuttle:%c, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, _T('A')+nJigNo+1, ch+1, strJudge);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		}
		pCell->m_EVTVersionCheck.SetTimeEnd();
	}
	//kjpark 20170908 COMMAD 별 Receive 추가
	else if(strCommand ==  OPTION_CHECK1 && strCommand == ICT_TEST)
	{
		if(pCell->m_ICTCheck.m_Class == NONE_CELL)
		{
			if(strJudge == PG_JUDGE_GOOD)
			{

				pCell->m_ICTCheck.m_Class = GOOD_CELL;	
				pCell->m_ICTCheck.m_DefectName = GOOD;			
			}
			else
			{
				pCell->m_ICTCheck.m_Class = REJECT_CELL;
				pCell->m_ICTCheck.m_DefectName = pCell->m_ICTCheck.m_strCommand + TEXT_DEFECT_NG;
			}
			strLog.Format(_T("%s <==== %s %s, Shuttle:%c, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, _T('A')+nJigNo+1, ch+1, strJudge);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		}
		pCell->m_ICTCheck.SetTimeEnd();
	}	
	//kjpark 20170908 COMMAD 별 Receive 추가
	else if(strCommand == SLEEP_CURRENT_CHECK)
	{
		if(pCell->m_SleepCurrent.m_Class == NONE_CELL)
		{
			if( (strJudge == PG_JUDGE_GOOD) || (strJudge == PG_JUDGE_NG) )
			{
				if(strJudge == PG_JUDGE_GOOD)
				{
					pCell->m_SleepCurrent.m_Class = GOOD_CELL;
					pCell->m_SleepCurrent.m_DefectName = strJudge;
				}
				else
				{
					pCell->m_SleepCurrent.m_Class = REJECT_CELL;
					pCell->m_SleepCurrent.m_DefectName = pCell->m_SleepCurrent.m_strCommand + TEXT_DEFECT_NG;
					pCell->defaultData.m_bIsInspectionSkip = TRUE;
				}


				// 전류값 총 6개 : IBAT, IVSS, IVDD, IVCI, IBAT2, IDD2
				CString strTemp = t.GetNextToken(_T(","), 3);
				pCell->m_SleepCurrent.m_CurrentText[IBAT] = strTemp.GetString();					
				for (int i=IVSS ; i<DEFECTINFO_CURRENT_MAX ; i++)
				{
					// Title하고 같이 있으니 1칸 넘어감
					strTemp = t.GetNextToken(_T(","), 2);
					nEtx = strTemp.Find(ETX);
					if(nEtx > 5)
					{
						strTemp = strTemp.Mid(0, nEtx-4);
					}
					pCell->m_SleepCurrent.m_CurrentText[i] = strTemp;
				}
				pCell->m_SleepCurrent.Text_to_Value();	// 전류값 string 변수를 double 변수에 옮긴다
				pCell->m_SleepCurrent.SetTimeEnd();

			}
		}
	}
	//kjpark 20170908 COMMAD 별 Receive 추가
	//kjpark 20170925 HLPM CURRENT MTP Vifity 에 들어 옴
	else if(strCommand == HLPM_CURRENT_CHECK || strCommand == HLPM_CURRENT_CHECK_AMTP)
	{
		if(pCell->m_HLPMCurrent.m_Class == NONE_CELL)
		{
			if( (strJudge == PG_JUDGE_GOOD) || (strJudge == PG_JUDGE_NG) )
			{
				if(strJudge == PG_JUDGE_GOOD)
				{
					pCell->m_HLPMCurrent.m_Class = GOOD_CELL;
					pCell->m_HLPMCurrent.m_DefectName = strJudge;
				}
				else
				{
					pCell->m_HLPMCurrent.m_Class = REJECT_CELL;
					pCell->m_HLPMCurrent.m_DefectName = pCell->m_HLPMCurrent.m_strCommand + TEXT_DEFECT_NG;
					pCell->defaultData.m_bIsInspectionSkip = TRUE;
				}


				// 전류값 총 6개 : IBAT, IVSS, IVDD, IVCI, IBAT2, IDD2
				CString strTemp = t.GetNextToken(_T(","), 3);
				pCell->m_HLPMCurrent.m_CurrentText[IBAT] = strTemp.GetString();					
				for (int i=IVSS ; i<DEFECTINFO_CURRENT_MAX ; i++)
				{
					// Title하고 같이 있으니 1칸 넘어감
					strTemp = t.GetNextToken(_T(","), 2);
					nEtx = strTemp.Find(ETX);
					if(nEtx > 5)
					{
						strTemp = strTemp.Mid(0, nEtx-4);
					}
					pCell->m_HLPMCurrent.m_CurrentText[i] = strTemp;
				}
				pCell->m_HLPMCurrent.Text_to_Value();	// 전류값 string 변수를 double 변수에 옮긴다
				pCell->m_HLPMCurrent.SetTimeEnd();

			}
		}
	}
	//kjpark 20170908 COMMAD 별 Receive 추가
	else if(strCommand ==  OPTION_CHECK2)
	{
		if(pCell->m_OptionCheck2.m_Class == NONE_CELL)
		{
			if(strJudge == PG_JUDGE_GOOD)
			{

				pCell->m_OptionCheck2.m_Class = GOOD_CELL;	
				pCell->m_OptionCheck2.m_DefectName = GOOD;			
			}
			else
			{
				pCell->m_OptionCheck2.m_Class = REJECT_CELL;
				pCell->m_OptionCheck2.m_DefectName = pCell->m_OptionCheck2.m_strCommand + TEXT_DEFECT_NG;
			}
			strLog.Format(_T("%s <==== %s %s, Shuttle:%c, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, _T('A')+nJigNo+1, ch+1, strJudge);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		}
		pCell->m_OptionCheck2.SetTimeEnd();
	}
	//kjpark 20170908 COMMAD 별 Receive 추가
	else if(strCommand ==  OPTION_CHECK3)
	{
		if(pCell->m_OptionCheck3.m_Class == NONE_CELL)
		{
			if(strJudge == PG_JUDGE_GOOD)
			{

				pCell->m_OptionCheck3.m_Class = GOOD_CELL;	
				pCell->m_OptionCheck3.m_DefectName = GOOD;			
			}
			else
			{
				pCell->m_OptionCheck3.m_Class = REJECT_CELL;
				pCell->m_OptionCheck3.m_DefectName = pCell->m_OptionCheck3.m_strCommand + TEXT_DEFECT_NG;
			}
			strLog.Format(_T("%s <==== %s %s, Shuttle:%c, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, _T('A')+nJigNo+1, ch+1, strJudge);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		}
		pCell->m_OptionCheck3.SetTimeEnd();
	}
	//kjpark 20170908 COMMAD 별 Receive 추가
	else if(strCommand ==  OPTION_CHECK4)
	{
		if(pCell->m_OptionCheck4.m_Class == NONE_CELL)
		{
			if(strJudge == PG_JUDGE_GOOD)
			{

				pCell->m_OptionCheck4.m_Class = GOOD_CELL;	
				pCell->m_OptionCheck4.m_DefectName = GOOD;			
			}
			else
			{
				pCell->m_OptionCheck4.m_Class = REJECT_CELL;
				pCell->m_OptionCheck4.m_DefectName = pCell->m_OptionCheck4.m_strCommand + TEXT_DEFECT_NG;
			}
			strLog.Format(_T("%s <==== %s %s, Shuttle:%c, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, _T('A')+nJigNo+1, ch+1, strJudge);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		}
		pCell->m_OptionCheck4.SetTimeEnd();
	}
	//kjpark 20170908 COMMAD 별 Receive 추가
	else if(strCommand ==  OPTION_CHECK5)
	{
		if(pCell->m_OptionCheck5.m_Class == NONE_CELL)
		{
			if(strJudge == PG_JUDGE_GOOD)
			{

				pCell->m_OptionCheck5.m_Class = GOOD_CELL;	
				pCell->m_OptionCheck5.m_DefectName = GOOD;			
			}
			else
			{
				pCell->m_OptionCheck5.m_Class = REJECT_CELL;
				pCell->m_OptionCheck5.m_DefectName = pCell->m_OptionCheck5.m_strCommand + TEXT_DEFECT_NG;
			}
			strLog.Format(_T("%s <==== %s %s, Shuttle:%c, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, _T('A')+nJigNo+1, ch+1, strJudge);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		}
		pCell->m_OptionCheck5.SetTimeEnd();
	}
	//kjpark 20170908 COMMAD 별 Receive 추가
	else if(strCommand ==  OPTION_CHECK6)
	{
		if(pCell->m_OptionCheck6.m_Class == NONE_CELL)
		{
			if(strJudge == PG_JUDGE_GOOD)
			{

				pCell->m_OptionCheck6.m_Class = GOOD_CELL;	
				pCell->m_OptionCheck6.m_DefectName = GOOD;			
			}
			else
			{
				pCell->m_OptionCheck6.m_Class = REJECT_CELL;
				pCell->m_OptionCheck6.m_DefectName = pCell->m_OptionCheck6.m_strCommand + TEXT_DEFECT_NG;
			}
			strLog.Format(_T("%s <==== %s %s, Shuttle:%c, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, _T('A')+nJigNo+1, ch+1, strJudge);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		}
		pCell->m_OptionCheck6.SetTimeEnd();
	}
	//kjpark 20170908 COMMAD 별 Receive 추가
	else if(strCommand ==  OPTION_CHECK7)
	{
		if(pCell->m_OptionCheck7.m_Class == NONE_CELL)
		{
			if(strJudge == PG_JUDGE_GOOD)
			{

				pCell->m_OptionCheck7.m_Class = GOOD_CELL;	
				pCell->m_OptionCheck7.m_DefectName = GOOD;			
			}
			else
			{
				pCell->m_OptionCheck7.m_Class = REJECT_CELL;
				pCell->m_OptionCheck7.m_DefectName = pCell->m_OptionCheck7.m_strCommand + TEXT_DEFECT_NG;
			}
			strLog.Format(_T("%s <==== %s %s, Shuttle:%c, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, _T('A')+nJigNo+1, ch+1, strJudge);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		}
		pCell->m_OptionCheck7.SetTimeEnd();
	}
	//kjpark 20170908 COMMAD 별 Receive 추가
	else if(strCommand ==  OPTION_CHECK8)
	{
		if(pCell->m_OptionCheck8.m_Class == NONE_CELL)
		{
			if(strJudge == PG_JUDGE_GOOD)
			{

				pCell->m_OptionCheck8.m_Class = GOOD_CELL;	
				pCell->m_OptionCheck8.m_DefectName = GOOD;			
			}
			else
			{
				pCell->m_OptionCheck8.m_Class = REJECT_CELL;
				pCell->m_OptionCheck8.m_DefectName = pCell->m_OptionCheck8.m_strCommand + TEXT_DEFECT_NG;
			}
			strLog.Format(_T("%s <==== %s %s, Shuttle:%c, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, _T('A')+nJigNo+1, ch+1, strJudge);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		}
		pCell->m_OptionCheck8.SetTimeEnd();
	}
	//kjpark 20170908 COMMAD 별 Receive 추가
	else if(strCommand ==  OPTION_CHECK9)
	{
		if(pCell->m_OptionCheck9.m_Class == NONE_CELL)
		{
			if(strJudge == PG_JUDGE_GOOD)
			{

				pCell->m_OptionCheck9.m_Class = GOOD_CELL;	
				pCell->m_OptionCheck9.m_DefectName = GOOD;			
			}
			else
			{
				pCell->m_OptionCheck9.m_Class = REJECT_CELL;
				pCell->m_OptionCheck9.m_DefectName = pCell->m_OptionCheck9.m_strCommand + TEXT_DEFECT_NG;
			}
			strLog.Format(_T("%s <==== %s %s, Shuttle:%c, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, _T('A')+nJigNo+1, ch+1, strJudge);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		}
		pCell->m_OptionCheck9.SetTimeEnd();
	}
	//kjpark 20170908 COMMAD 별 Receive 추가
	else if(strCommand ==  OPTION_CHECK10)
	{
		if(pCell->m_OptionCheck10.m_Class == NONE_CELL)
		{
			if(strJudge == PG_JUDGE_GOOD)
			{

				pCell->m_OptionCheck10.m_Class = GOOD_CELL;	
				pCell->m_OptionCheck10.m_DefectName = GOOD;			
			}
			else
			{
				pCell->m_OptionCheck10.m_Class = REJECT_CELL;
				pCell->m_OptionCheck10.m_DefectName = pCell->m_OptionCheck10.m_strCommand + TEXT_DEFECT_NG;
			}
			strLog.Format(_T("%s <==== %s %s, Shuttle:%c, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, _T('A')+nJigNo+1, ch+1, strJudge);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		}
		pCell->m_OptionCheck10.SetTimeEnd();
	}
	else if(strCommand == MTP_WRITE_PRESCALE)
	{
		// 대충 해 놓은거임.. POC설비 보고 수정해야 함...
		if(pCell->m_MTPWrite.m_Class == NONE_CELL)
		{
			if(strJudge == PG_JUDGE_GOOD)
			{
				pCell->m_MTPWrite.m_Class = GOOD_CELL;
				pCell->m_MTPWrite.m_DefectName = GOOD;
			}
			else
			{
				CString strTemp = t.GetNextToken(_T(","));
				nEtx = strTemp.Find(ETX);
				if(nEtx > 5)
					strTemp = strTemp.Mid(0, nEtx-4);

				pCell->m_MTPWrite.m_Class = REJECT_CELL;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
				pCell->m_MTPWrite.m_DefectName = strTemp;
			}
			pCell->m_MTPWrite.SetTimeEnd();
		}
	}
	//kjpark 20170911 신호기 UI MTP COMMAND 추가
	else if(strCommand == MTP_VERIFY)
	{
		if(pCell->m_MTPVerify.m_Class == NONE_CELL)
		{
			if(strJudge == PG_JUDGE_GOOD)
			{
				pCell->m_MTPVerify.m_Class = GOOD_CELL;
				pCell->m_MTPVerify.m_DefectName = GOOD;
			}
			else
			{
				CString strTemp = t.GetNextToken(_T(","));
				nEtx = strTemp.Find(ETX);
				if(nEtx > 5)
					strTemp = strTemp.Mid(0, nEtx-4);

				pCell->m_MTPVerify.m_Class = REJECT_CELL;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
				pCell->m_MTPVerify.m_DefectName = strTemp;
			}
			pCell->m_MTPVerify.SetTimeEnd();
		}
	}
	else
	{
		theLog[LOG_SOCKET_ERR].AddBuf(_T("Unknown Command [%s]"), strCommand);
	}
	//kjaprk 20161114 패킷 여러게 수신시 잘라내고 다시 파싱
	return PacketClear(strMsg);
}

CString CDeviceSocket_PGHost::PacketClear( CString strMsg )
{
	// 2개 이상 연달아 붙어 나올 것을 대비해 맨 앞 하나를 지운다.
	int nNextIndex = 0;

	nNextIndex = strMsg.Find(STX, 1);

	if(nNextIndex == -1)
		strMsg = _T("");
	else if(nNextIndex > 0)
		strMsg.Delete(0, nNextIndex);

	return strMsg;
}

void CDeviceSocket_PGHost::SetAddLog( CString strLog )
{
	theLog[LOG_SOCKET_PG].AddBuf(strLog);
	CFormSignal* pView = (CFormSignal*)GetMainHandler()->GetGUIFrame()->GetView(eFORM_Setup1);
	if(pView->m_bIsShwoWindow)
		pView->SetLogListUpdate(strLog);
}

void CDeviceSocket_PGHost::PatternGeneratorACK(CCellInfo* pCell, CString strJudge)
{
	theLog[LOG_OP2PG_PACKET].AddBuf(_T("<- %s, Shuttle:%c Ch:%d %s Ack Recived"),SOCKET_PGPC, pCell->defaultData.m_JigId+1, pCell->defaultData.m_JigCh+1, strJudge);
}


BOOL CDeviceSocket_PGHost::SendMassageToPatternGenerator(CString strCmdType, int jig, int ch, CString strExtraData /*= _T("")*/ )
{		
	CString strLog;
	//신호기로 메시지를 보낸다.  
	//kjpark 20161029 PG send Packet 버그 수정
	CClientSocket* pSocket = m_Socket.GetClientAt(SOCKET_PGPC);

	if ( pSocket == NULL )		// 연결 실패
	{
		//kjpark 20161021 Setup 신호기 UI 구현
		strLog.Format(_T("SEND FAIL(Not Connect) CMD : %s"), strCmdType);
		SetAddLog(strLog);

		return FALSE;
	}


	BYTE ucData[1024];

	CString strBuff		= _T("");
	CString strCount	= _T("");
	CString strTemp		= _T("");
	int nCount = 0;

	memset(ucData,	NULL,	sizeof(ucData));

	ucData[0] = STX;

	//Dest Code
	ucData[1] = CODEPGPC;

	// Message, 별도의 문자열 데이터가 필요로하는 명령이라면,
	if ( strExtraData.GetLength() > 0 )			// 2015-06-02, 특정 패턴 선택 지정 명령 추가
	{
		strBuff.Format(_T("Ch,%d,%s,%s"), ((jig * JIG_CH_MAX ) + ch + 1), strCmdType, strExtraData);
	}
	else
		strBuff.Format(_T("Ch,%d,%s"), ((jig * JIG_CH_MAX ) + ch + 1), strCmdType);		// 그 외는 채널 일련번호를 넘겨준다.

	// Count
	nCount = strBuff.GetLength();
	strCount.Format(_T("%04x"),nCount);

	// Merge
	strTemp.Format(_T("%s%s"), strCount, strBuff);
	nCount = strTemp.GetLength();

	CEtc::ConvertMultibyte((wchar_t *)strTemp.GetString(), (char *)&ucData[2], 1024-2);			// WCHAR CString을 Char 형으로 변환시켜준다.
	USHORT checksum = GetCRC16_Checksum((UCHAR *)&ucData[1], nCount+1);							// CRC16을 만들어준다.

	CString strCheckSum;
	strCheckSum.Format(_T("%04X"), checksum);

	CEtc::ConvertMultibyte((wchar_t *)strCheckSum.GetString(), (char *)&ucData[nCount + 2], 1024-2-nCount);

	// ETX
	ucData[nCount + 6] = ETX;

	// Socket을 통해 전송한다.
	//kjpark 20161029 PG send Packet 버그 수정
	if (m_Socket.SendClient(pSocket->GetSocketName(), ucData, nCount + 7))
	{
		strLog.Format(_T("SEND %s"), strTemp);
		SetAddLog(strLog);

		return TRUE;
	}

	//// 전송 실패	
	strLog.Format(_T("SEND FAIL %s"), strTemp);
	SetAddLog(strLog);
	return FALSE;
}

USHORT CDeviceSocket_PGHost::GetCRC16_Checksum( const void *buf, int len )
{
	char *cbuf = (char*)buf;

	register int counter;
	register unsigned short crc = 0;
	for( counter = 0; counter < len; counter++)
	{
		crc = (crc<<8) ^ crc16tab[((crc>>8) ^ *cbuf++)&0x00FF];
	}
	return crc;
}

void CDeviceSocket_PGHost::AddPGMsg( CString strMsg )
{
	m_listMsg.AddTail(strMsg);
}

void CDeviceSocket_PGHost::ClearPGMsg()
{
	m_listMsg.RemoveAll();
}

void CDeviceSocket_PGHost::DrawPGMsg( JIG_ID jig, JIG_CH ch, COLORREF colorBack, COLORREF colorFont, int nX, int nY, int nSize )
{
	CString strMsg;
	int nCount = m_listMsg.GetCount();	// Msg Line수

	if(nCount < 1)
		return;

	CString strFullData, strSpanData;

	// 배경색,글자색,x,y,글자크기
	strFullData.Format(_T("0x%02X%02X%02X,0x%02X%02X%02X,%d,%d,%d"), 
		GetRValue(colorBack), GetGValue(colorBack), GetBValue(colorBack),
		GetRValue(colorFont), GetGValue(colorFont), GetBValue(colorFont),
		nX, nY, nSize);

	POSITION pos = m_listMsg.GetHeadPosition();
	while(pos)
	{
		strMsg = m_listMsg.GetNext(pos);
		strSpanData.Format(_T(",%s"), strMsg);

		// Line 갯수마다 추가한다
		strFullData += strSpanData;
	}

	SendMassageToPatternGenerator(PG_MSG, jig, ch, strFullData);
}

void CDeviceSocket_PGHost::AddPGSquare( SQUARE_POINT sqpoint )
{
	m_listSquare.AddTail(sqpoint);
}

void CDeviceSocket_PGHost::ClearPGSqueare()
{
	m_listSquare.RemoveAll();
}

void CDeviceSocket_PGHost::DrawPGSquare( JIG_ID jig, JIG_CH ch )
{
	SQUARE_POINT sqpoint;
	int nCount = m_listSquare.GetCount();	// 사각형 갯수

	if(nCount < 1)
		return;

	CString strFullData, strSpanData;

	// 사각형 갯수
	strFullData.Format(_T("%d"), nCount);

	POSITION pos = m_listSquare.GetHeadPosition();
	while(pos)
	{
		sqpoint = m_listSquare.GetNext(pos);
		// 두께,크기,색상,X,Y 순서로 표기
		// RGB를 그냥 숫자로 표기하면 BGR 순서로 기록되기에 하나씩 기록
		strSpanData.Format(_T(",%d,%d,0x%02X%02X%02X,%d,%d"), sqpoint.nThickness, sqpoint.nSize, 
			GetRValue(sqpoint.colorLine), GetGValue(sqpoint.colorLine), GetBValue(sqpoint.colorLine),
			sqpoint.nX, sqpoint.nY);

		// 사각형 갯수마다 추가한다
		strFullData += strSpanData;
	}

	SendMassageToPatternGenerator(PG_SQUARE, jig, ch, strFullData);
}

void CDeviceSocket_PGHost::ShowDZonePattern( int nPatternNo, JIG_ID jig, JIG_CH ch )
{
	// D0_PTRN101 이라고 날리면 101패턴이 표기된다
	CString strCmd;
	strCmd.Format(_T("%s%d"), PG_D0_PTRN, nPatternNo);
	SendMassageToPatternGenerator(strCmd, jig, ch);
}
