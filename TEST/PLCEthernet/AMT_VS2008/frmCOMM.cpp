// frmCOMM.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "frmCOMM.h"
#include "MainFrm.h"


// CFormCOMM

IMPLEMENT_DYNCREATE(CFormCOMM, CFormView)

#define SEND_MODE						0
#define RECIVE_MODE						1
#define TIMER_INFO_DISPLAY_COMM			3
#define TIMER_CMD_INSPECTION_YES		4
#define TIMER_INSPECTION_END			5	// 검사 종료 후 일정 시간 대기용
#define TIMER_CONTACT_READY				6

CFormCOMM::CFormCOMM()
	: CFormView(CFormCOMM::IDD)
{
	gl_pComm = this;

	LoadSocketParam();

	m_iReciveSize = 0;
	m_PanelSelect= 1;
	m_iPanelSelectCnt= 1;
	m_nSec = 0;
	m_bVIWait = FALSE;
	m_iCurEQState = eEqNothing;
	m_iCurProcessState = ePanelNothing;
	m_nAlarmState = 0;
	m_nAlarmID = ERR_NONE;
	m_strAlarmText = _T("");
	m_bQCFlag = FALSE;

	m_strPanelIn = _T("");
	m_strPanelOut = _T("");

	m_nBakMsg = 0;
	m_dAlarmSetTimer = 0.0;
}

CFormCOMM::~CFormCOMM()
{
}

void CFormCOMM::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_XNETCTRL1, XNetCtrl);
	DDX_Control(pDX, IDC_EDIT_LC_HISTORY, m_LCHistory);
	DDX_Control(pDX, IDC_EDIT_VI_HISTORY, m_VIHistory);
	DDX_Control(pDX, IDC_RADIO_COMM1, m_radioCommLoaderAddr);
	DDX_Control(pDX, IDC_RADIO_COMM2, m_radioCommLoaderInfo);
	DDX_Control(pDX, IDC_RADIO_COMM3, m_radioCommPanelInfo);
	DDX_Control(pDX, IDC_RADIO_COMM4, m_radioCommUnloaderAddr);
	DDX_Control(pDX, IDC_MSFLEXGRID_COMM_INFO1, m_CommInfoGrid1);
	DDX_Control(pDX, IDC_MSFLEXGRID_COMM_INFO2, m_CommInfoGrid2);
}

BEGIN_MESSAGE_MAP(CFormCOMM, CFormView)
	ON_MESSAGE(WM_PACKET_RECEIVE, OnPacketReceive)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_RADIO_COMM1, &CFormCOMM::OnBnClickedRadioComm1)
	ON_BN_CLICKED(IDC_RADIO_COMM2, &CFormCOMM::OnBnClickedRadioComm2)
	ON_BN_CLICKED(IDC_RADIO_COMM3, &CFormCOMM::OnBnClickedRadioComm3)
	ON_BN_CLICKED(IDC_RADIO_COMM4, &CFormCOMM::OnBnClickedRadioComm4)
END_MESSAGE_MAP()


// CFormCOMM diagnostics

#ifdef _DEBUG
void CFormCOMM::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormCOMM::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormCOMM message handlers

BOOL CFormCOMM::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CFormCOMM::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	InitXNet();
	InitSocketVI();
	InitSocketPG();

	InitCommInfoGrid();
	m_radioCommLoaderInfo.SetCheck(BST_CHECKED);

	SetTimer(TIMER_INFO_DISPLAY_COMM, 1000, NULL);

}
BEGIN_EVENTSINK_MAP(CFormCOMM, CFormView)
	ON_EVENT(CFormCOMM, IDC_XNETCTRL1, 1, CFormCOMM::ReceiveXnetctrl, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CFormCOMM, IDC_XNETCTRL1, 3, CFormCOMM::CommCloseXNetCtrl, VTS_NONE)
	ON_EVENT(CFormCOMM, IDC_XNETCTRL1, 2, CFormCOMM::CommEstablishXnetctrl, VTS_NONE)
END_EVENTSINK_MAP()

//________________________________________ L/C 관련. ________________________________________//
//__________XNet.OCX 사용(일반 소켓으론 연결이 안됨)아무래도 계층이 틀린듯. __________//
//__________________________________________________________________________________________//
void CFormCOMM::InitXNet()
{
		//////////////////////////////////////////////////////////////////////////
		// AMT L/C IP = 192.168.10.1		// MMT L/C IP = 192.168.10.2
		// AMT_A Port = 3100		// AMT_B Port = 3200
		// MMT1(A1) Port = 3100	// MMT2(A2) Port = 3200	// MMT3(B1) Port = 3300	// MMT4(B2) Port = 3400
		// AMT_A UniCode = 1		// AMT_B UniCode = 2
		// MMT1(A1) UniCode = 1	// MMT2(A2) UniCode = 2	// MMT3(B1) UniCode = 3	// MMT4(B2) UniCode = 4
		//////////////////////////////////////////////////////////////////////////

	long i = 0;
	XNetCtrl.SetActive(1/*CLIENT*/);

	XNetCtrl.SetIP(m_SocketLC.GetIP());
	XNetCtrl.SetPort(m_SocketLC.GetPort());
	XNetCtrl.SetSpoolDir(".");
	XNetCtrl.SetSpoolEnabled(FALSE);
	i = XNetCtrl.Start();

	int iMachineType = GetMachinePCType();

	m_iUNI_CODE = 1;
}

void CFormCOMM::ReceiveXnetctrl(long mesgType, long mesgID, long mesgLen)
{
	// TODO: Add your message handler code here
	CString strMsg;
	CString strMsgType;
	CString strItem[4096];	// Msg Body.//
	int	iItem, iPrev;		// Msg Index.//
	int	iItemCnt;			// Body .//Count
	int	iIndex = 0;

	CString sTemp = "";
	CString strText; 
	int	iUnitID = 0;

	int EQCode = 0, ProcessCode = 0;

	// Get Recv Msg.//
	strMsg = XNetCtrl.GetString(mesgID);
	if(strMsg.Left(1) != "@")
		return;
	CommLog(SocketName_LC, SocketName_MT, strMsg);

	// Msg Parsing Rule.//
	strMsg = strMsg.Mid(1, strMsg.GetLength() - 2);	// Remove Head And Tail.//
	iPrev = iItemCnt = 0;
	for(iItem = strMsg.Find("##", 1); iPrev != 1; iItem = strMsg.Find("##",iItem + 2))
	{
		if(iPrev == 0)			// First.//
			strMsgType = strMsg.Mid(iPrev, iItem);
		else if(iItem == -1)	// Last.//
			strItem[iItemCnt++] = strMsg.Mid(iPrev, strMsg.GetLength() - iPrev);
		else					// Middle.//
			strItem[iItemCnt++] = strMsg.Mid(iPrev, iItem - iPrev);
		iPrev = iItem + 2;
	}

	if(strMsgType == "TK")    // Inform data를 Unload에다 data를 흘려 준다.// ADDLKH
	{
		sTemp = GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sEPanelID;
		//LoaderBuf.m_sEPanelID;
		sTemp.TrimRight();
		if(strItem[0] != sTemp) // PANEL ID를 매칭 해본다.
		{
			memcpy(GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sReserved, "IDNG", 4); // FAIL DATA를 Unload에 보내준다.

			CommLog("PLC", SocketName_MT, GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sReserved);

		}
		else
		{
     		if(strItem[1] == "FAIL") 
			{
				memcpy(GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sReserved, "FAIL", 4); // FAIL DATA를 Unload에 보내준다.
				CommLog("PLC", SocketName_MT, GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sReserved);
			}
			else  
			{
				memcpy(GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sReserved, "PASS", 4); // FAIL DATA를 Unload에 보내준다.
				CommLog("PLC", SocketName_MT, GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sReserved);
			}
		}
	}
// 	else
// 	{
// 		memcpy(GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sReserved, "NULL", 4); // FAIL DATA를 Unload에 보내준다.
// 		CommLog("PLC", SocketName_MT, GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sReserved);
// 	}

	GetMelsec()->GlassDataWrite(GetDoc()->m_pEqMainSeq->m_PanelInfo, DATA_OUTPUT_START);

	iUnitID = atoi(strItem[iIndex++]);	

	if(strMsgType == "LE")		// PM Mode.//
	{
		EQCode = atoi(strItem[iIndex++]);

		switch(EQCode)
		{
		case 51:				// Pause.//
			strText.Format("[Stop] PAUSE -> Xnetctrl()-Function Call");
			memcpy(GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sReserved, "NULL", 4); // FAIL DATA를 Unload에 보내준다.
			CommLog(SocketName_LC, SocketName_MT, strText);
			GetApp()->ChangeStatus(STATUS_STOP);
			break;
		case 52:				// Resume.//
			//	pView->StartFunction();
			break;
		case 53:				// PM.//
			m_iPMCode = atoi(strItem[iIndex]);
// 			DoPM(ePMLC2Machine);
			break;
		case 54:				// Normal.//
			//	EQState2LC(eNormal);
			//	ProcessState2LC(eEqPInit);
			//	pView->m_pMelsec->SetEQState(eNormal);
			//	pView->m_pMelsec->SetProcessState(eEqPInit);
// 			ReleasePMMode();	// 요거 모달로는 안됨.//
			break;			
		case 81:				// Teminal Message.//
// 			strLog = pView->GetCurTime() + strItem[iIndex];
			//			pView->m_lblLCMSG.SetWindowText(strLog);
			break;
		
		case 82:				// Operator Call.//
			//			OPCall(strItem[iIndex]);
			break;
		}
	} 
	else if(strMsgType == "LJ")	// ProcessCode.//
	{
		ProcessCode = atoi(strItem[iIndex++]);
		if(ProcessCode == 0)
		{
		}
		else if(ProcessCode == 1)
		{
		}
		else if(ProcessCode == 2)// 검사 안한다.//
		{
			m_bProcessCancelMode = TRUE;
			m_bProcessCancelMode_Orgin = TRUE;
		}
		else if(ProcessCode == 3)// 검사 한다.//
		{
			m_bProcessCancelMode = TRUE;
			m_bProcessCancelMode_Orgin = TRUE;
		}
	}
	else if(strMsgType == "CP")	// File Server Setting.//
	{
// 		ReportFileServerSet2PG(strMsg);					// CP 정보를 Buffer에 저장.//
	}
	else if(strMsgType == "EO")	// Event Setting.//
	{
// 		ReportParamSet2PG(strMsg);						// EO 정보를 Buffer에 저장.//
	}
	else if(strMsgType == "BP")							// Bypass.//
	{
		if(strItem[0] == "ON")
		{
// 			if(strItem[1] == "AMT")
// 				pDoc->m_structSetting.bAMTByPass = TRUE;
// 			else if(strItem[1] == "MMT")
// 				pDoc->m_structSetting.bMMTByPass = TRUE;
		}
		else if(strItem[0] == "OFF")
		{
// 			if(strItem[1] == "AMT")
// 				pDoc->m_structSetting.bAMTByPass = FALSE;
// 			else if(strItem[1] == "MMT")
// 				pDoc->m_structSetting.bMMTByPass = FALSE;
		}
	}
	else if(strMsgType == "CYD")						// Defect Code 공유.//
	{
		int iCount = atoi(strItem[0]);		// 전체 수.//
		if(1 < iCount && iCount <= 100)
		{
// 			pDoc->m_structSetting.VIResult.iLineDataCount[0] = iCount;
// 			pDoc->m_structSetting.VIResult.iLineDataCount[1] = 0;
// 			pDoc->m_structSetting.VIResult.iLineDataCount[2] = 0;
// 			pDoc->m_structSetting.VIResult.iLineDataCount[3] = 0;
		}
		else if(100 < iCount && iCount <= 200)
		{
// 			pDoc->m_structSetting.VIResult.iLineDataCount[0] = 99;
// 			pDoc->m_structSetting.VIResult.iLineDataCount[1] = iCount - 101;
// 			pDoc->m_structSetting.VIResult.iLineDataCount[2] = 0;
// 			pDoc->m_structSetting.VIResult.iLineDataCount[3] = 0;
		}
		else if(200 < iCount && iCount <= 300)
		{
// 			pDoc->m_structSetting.VIResult.iLineDataCount[0] = 99;
// 			pDoc->m_structSetting.VIResult.iLineDataCount[1] = 99;
// 			pDoc->m_structSetting.VIResult.iLineDataCount[2] = iCount - 201;
// 			pDoc->m_structSetting.VIResult.iLineDataCount[3] = 0;
		}
		else if(300 < iCount && iCount <= 400)
		{
// 			pDoc->m_structSetting.VIResult.iLineDataCount[0] = 99;
// 			pDoc->m_structSetting.VIResult.iLineDataCount[1] = 99;
// 			pDoc->m_structSetting.VIResult.iLineDataCount[2] = 99;
// 			pDoc->m_structSetting.VIResult.iLineDataCount[3] = iCount - 301;
		}

		int iDataIndex = 8;
		for(int i = 0; i < iCount; i++)
		{
// 			int iIndex1 = atoi(strItem[i * iDataIndex + 1]) - 1;
// 			if(iIndex == -1)
// 				return;
// 			if(strItem[i * iDataIndex + 2] == "")
// 				pDoc->m_structSetting.VIResult.strCode[iIndex1]			= "????";
// 			else
// 				pDoc->m_structSetting.VIResult.strCode[iIndex1]			= strItem[i * iDataIndex + 2];
// 			if(strItem[i * iDataIndex + 3] == "")
// 				pDoc->m_structSetting.VIResult.strJudgement[iIndex1]	= "??";
// 			else
// 				pDoc->m_structSetting.VIResult.strJudgement[iIndex1]	= strItem[i * iDataIndex + 3];
// 
// 			if(strItem[i * iDataIndex + 4] == "ON")
// 				pDoc->m_structSetting.VIResult.bHold[iIndex1]			= TRUE;
// 			else
// 				pDoc->m_structSetting.VIResult.bHold[iIndex1]			= FALSE;
// 			if(strItem[i * iDataIndex + 5] == "ON")
// 				pDoc->m_structSetting.VIResult.bFutureHold[iIndex1]		= TRUE;
// 			else
// 				pDoc->m_structSetting.VIResult.bFutureHold[iIndex1]		= FALSE;
// 			if(strItem[i * iDataIndex + 6] == "")
// 				pDoc->m_structSetting.VIResult.strFutureTarget[iIndex1] = "???";
// 			else
// 				pDoc->m_structSetting.VIResult.strFutureTarget[iIndex1]	= strItem[i * iDataIndex + 6];
// 			if(strItem[i * iDataIndex + 7] == "OFF")
// 				pDoc->m_structSetting.VIResult.iCycleStop[iIndex1]		= 0;
// 			else
// 				pDoc->m_structSetting.VIResult.iCycleStop[iIndex1]		= atoi(strItem[i * iDataIndex + 7]);
// 			if(strItem[i * iDataIndex + 8] == "")
// 				pDoc->m_structSetting.VIResult.strCycleTarget[iIndex1] = "???";
// 			else
// 				pDoc->m_structSetting.VIResult.strCycleTarget[iIndex1]	= strItem[i * iDataIndex + 8];
		}
// 		pDoc->FileSaveRecipe();
// 		pView->m_pDlgVI->SelectTab();
	}
	else if(strMsgType == "IRD")							// 최종 검사 Table 공유.//
	{
		int iCount = atoi(strItem[0]);		// 전체 수.//
		for(int i = 0; i < iCount; i++)
		{
// 			if(strItem[i + 1] == "OK")
// 				pDoc->m_structSetting.VIResult.bInspectionTable[i] = 1;
// 			else
// 				pDoc->m_structSetting.VIResult.bInspectionTable[i] = 0;
		}
// 		pView->m_pDlgVI->DocToCell();
// 		pDoc->FileSaveRecipe();
	}
	else if(strMsgType == "IF")				// Inform data output.//
	{
// 		pView->m_lblInformDataOutput.SetWindowText(strItem[0]);
// 		pView->m_lblDescription.SetWindowText(strItem[1]);
	}

}

void CFormCOMM::CommCloseXNetCtrl()	
{
	m_SocketLC.SetConnected(FALSE);
	CommLog(SocketName_LC, SocketName_MT, "LC Closed");
}

void CFormCOMM::CommEstablishXnetctrl()
{
	m_SocketLC.SetConnected(TRUE);
	CommLog(SocketName_LC, SocketName_MT, "LC Connected");
}

long CFormCOMM::PanelTransfer2LC(EPanelEvents iID)
{
	long lret;
	CString strInfo;

	LC_PANEL_INFO lcInfo;

	switch(iID)
	{
	case ePanelIn:
		lcInfo = GetDoc()->m_pEqMainSeq->m_PanelInfo;
		break;
	case ePanelOut:
		lcInfo = GetDoc()->m_pEqMainSeq->m_PanelInfo;
		break;
	case ePanelScrap:
		lcInfo = GetDoc()->m_pEqMainSeq->m_PanelInfo;
		break;
	case ePanelUnScrap:
		lcInfo = GetDoc()->m_pEqMainSeq->m_PanelInfo;
		break;
	case eCANNOTSAVE_RAW_DATA:
		lcInfo = GetDoc()->m_pEqMainSeq->m_PanelInfo;
		break;
	case eCANNOTSAVE_SUM_DATA:
		lcInfo = GetDoc()->m_pEqMainSeq->m_PanelInfo;
		break;
	case eLastGlassLoss:
		lcInfo = GetDoc()->m_pEqMainSeq->m_PanelInfo;
		break;
	}

	strInfo.Format("@UC##%d##%d##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s$",
		m_iUNI_CODE,				// UNI_CODE(A_SIDE Or B_SIDE).//	
		iID/*EvtType*/,					// Event Type.//
		lcInfo.szPanelInfo[LC_HPANELID		],		// 12Byte.//
		lcInfo.szPanelInfo[LC_EPANELID		],		// 12Byte.//
		lcInfo.szPanelInfo[LC_BATCHID		],		// 12Byte.//
		lcInfo.szPanelInfo[LC_PRODTYP		],		//  2Byte.//
		lcInfo.szPanelInfo[LC_PRODKND		],		//  2Byte.//
		lcInfo.szPanelInfo[LC_DEVICEID		],		// 18Byte.//
		lcInfo.szPanelInfo[LC_STEPID		],		// 10Byte.//
		lcInfo.szPanelInfo[LC_PPID			],		// 16Byte.//
		lcInfo.szPanelInfo[LC_THICKNESS		],		//  2Byte.//
		lcInfo.szPanelInfo[LC_INSPFLG		],		//  2Byte.//
		lcInfo.szPanelInfo[LC_PANELSIZE		],		//  4Byte.//
		lcInfo.szPanelInfo[LC_PANELPOS		],		//  2Byte.//
		lcInfo.szPanelInfo[LC_COUNT1		],		//  2Byte.//
		lcInfo.szPanelInfo[LC_COUNT2		],		//  2Byte.//
		lcInfo.szPanelInfo[LC_GRADE			],		//  6Byte.//
		lcInfo.szPanelInfo[LC_COMMENT		],		// 16Byte.//
		lcInfo.szPanelInfo[LC_COMPSIZE		],		//  2Byte.//
		lcInfo.szPanelInfo[LC_READFLG		],		//  2Byte.//
		lcInfo.szPanelInfo[LC_PANELSTATE	],		//  2Byte.//
		lcInfo.szPanelInfo[LC_JUDGEMENT		],		//  4Byte.//
		lcInfo.szPanelInfo[LC_CODE			],		//  4Byte.//
		lcInfo.szPanelInfo[LC_RUNLINE		],		// 20Byte.//
		lcInfo.szPanelInfo[LC_UNIQUEID		],		//  4Byte.//
		lcInfo.szPanelInfo[LC_PAIR_HID		],		// 12Byte.//
		lcInfo.szPanelInfo[LC_PAIR_EID		],		// 12Byte.//
		lcInfo.szPanelInfo[LC_PAIR_GRADE	],		//  6Byte.//
		lcInfo.szPanelInfo[LC_PAIR_UNIQUEID	],		//  4Byte.//
		lcInfo.szPanelInfo[LC_FLOWRECIPE	],		//  2Byte.//
		lcInfo.szPanelInfo[LC_RESERVED		],		//  2Byte.//
		lcInfo.szPanelInfo[LC_BITSIGNAL		]);		//  4Byte.//

	// 중복 전송 방지
	if(iID == ePanelIn)
	{
		if(m_strPanelIn == strInfo)
			return 0;
		else 
			m_strPanelIn = strInfo ;
	}
	if(iID == ePanelOut)
	{
		if(m_strPanelOut == strInfo)
			return 0;
		else 
			m_strPanelOut = strInfo ;
	}
	lret = SendString(strInfo, strInfo.GetLength());

	return lret;
}

long CFormCOMM::ProcessState2LC(int ProcessCode , CString strCode , CString strErrMsg)
{
	long lret;
	CString strMsg;

	///////////////////////////////////////////////////////
	// 2011.01.13 hccho
	//
	// Proc State 중복 보고를 막기위해 추가함.
	//
	if ( LoadLastEQProcess() == ProcessCode )
	{
		return 0;
	}
	//////////////////////////////////////////////////////

 	m_iCurProcessState = ProcessCode;
	if(strCode.GetLength() < 1)
		strMsg.Format("@UP##%d##%d$", 1, ProcessCode);
	else
		strMsg.Format("@UP##%d##%d##%s##%s$", 1, ProcessCode, strCode, strErrMsg);

	if(LoadLastEQProcessPacket() == strMsg)
		return 0;

	lret = SendString(strMsg, strMsg.GetLength());

	SaveLastEQProcess(ProcessCode, strMsg);

	return lret;
}

long CFormCOMM::EQState2LC(int EQCode , CString strCode , CString strErrMsg)
{
	long lret;
	CString strMsg;
	int iPMCode = 0;

	if(LoadLastEQState() == EQCode)
		return 0;

	CString strTemp,strTemp1;
	strTemp.Format("%d",LoadLastEQState());
	strTemp1.Format("%d",EQCode);
	GetDoc()->SetLog(strTemp,LOG_COMM);
	GetDoc()->SetLog(strTemp1,LOG_COMM);

	m_iCurEQState = EQCode;

	///////////////////////////////////////////////////////////
	// 2011.01.13 hccho
	//
	// -. EQ Fault 에서 PM 으로 변환 되는 경우는 Alarm Reset 할때 뿐이고, 
	//    Alarm Reset() 함수에서 EQ State 설정하고 있으므로 아래 부분은 삭제 함.
	// -. 중복 보고를 막기 위함.
	// 
	// -. 우선 주석 처리 해두니 참고 하세요.
	//
	//------------------------------------------------------------

// 	if(EQCode == eNormal)
// 	{
// 		// PM으로 전환 안되고 그냥 Normal로 오는
// 		// 경우의 수를 전부 찾기는 어려우므로 여기서 체크해서 
// 		// 한번 더 보내주기도 한다 [11/20/2010 OSC]
// 		if(LoadLastEQState() == eFault)
// 		{
// 			EQState2LC(ePM);
// 			Sleep(100);
// 		}
// 	}

	////////////////////////////////////////////////////////////

	if(strCode =="")
	{	
		strMsg.Format("@US##%d##%d##%d$", 1, EQCode, iPMCode);
	}
	else
	{
		strMsg.Format("@US##%d##%d##%s##%s$", 1, EQCode, strCode, strErrMsg);
	}

	lret = SendString(strMsg, strMsg.GetLength());

	SaveLastEQState((int)m_iCurEQState);

	return lret;
}

void CFormCOMM::Alarm2LC( int iOnOff, int iID/* = ERR_NONE*/, CString strErrorText/*  = _T("")*/)
{
	// Error On이나 Off가 연속 두번 날라가는걸 방지하기 위해
	if(iOnOff == m_nAlarmState)
		return;
	m_nAlarmState = iOnOff;
	// On 됬을 때랑 Off 됬을 때가 Alarm ID를 같게 하기 위해
	if(m_nAlarmState)
	{
		m_nAlarmID = iID;
		m_strAlarmText = strErrorText;
	}

	CString strMsg;
	// 경알람 0, 중알람 1.//
	int iAlram;
	if(iID == ERR_EMO_SWITCH_ON)	// EMO.//
		iAlram = 1;
	else
		iAlram = 0;

	strMsg.Format("@UA##%d##%d##%d##%d##%s$",
		m_iUNI_CODE,					// A Or B열 구분.//
		iOnOff,
		m_nAlarmID,
		iAlram,						// 경알람 0, 중알람 1.//
		m_strAlarmText);

	SendString(strMsg, strMsg.GetLength());
}

long CFormCOMM::InspectionResult2LC( CString strCode, CString strJudgement )
{
	long lret;
	CString strInfo;
	CString strCommand;
	EPanelEvents EvtType;

	if(m_bQCFlag == FALSE)
		strCommand = "IR";
	else
		strCommand = "QC";

	LC_PANEL_INFO lcInfo;
	lcInfo = GetDoc()->m_pEqMainSeq->m_PanelInfo;

	strInfo.Format("@%s##%d##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s$",
		strCommand,					// QC 인지 IR 인지.//
		m_iUNI_CODE,				// UNI_CODE(A_SIDE Or B_SIDE).//	
		lcInfo.szPanelInfo[LC_HPANELID		],		
		lcInfo.szPanelInfo[LC_EPANELID		],		
		lcInfo.szPanelInfo[LC_BATCHID		],		
		lcInfo.szPanelInfo[LC_PRODTYP		],		
		lcInfo.szPanelInfo[LC_PRODKND		],		
		lcInfo.szPanelInfo[LC_DEVICEID		],		
		lcInfo.szPanelInfo[LC_STEPID		],		
		lcInfo.szPanelInfo[LC_PPID			],		
		lcInfo.szPanelInfo[LC_THICKNESS		],		
		lcInfo.szPanelInfo[LC_INSPFLG		],		
		lcInfo.szPanelInfo[LC_PANELSIZE		],		
		lcInfo.szPanelInfo[LC_PANELPOS		],		
		lcInfo.szPanelInfo[LC_COUNT1		],		
		lcInfo.szPanelInfo[LC_COUNT2		],		
		lcInfo.szPanelInfo[LC_GRADE			],		
		lcInfo.szPanelInfo[LC_COMMENT		],		
		lcInfo.szPanelInfo[LC_COMPSIZE		],		
		lcInfo.szPanelInfo[LC_READFLG		],		
		lcInfo.szPanelInfo[LC_PANELSTATE	],		
		lcInfo.szPanelInfo[LC_JUDGEMENT		],		
		lcInfo.szPanelInfo[LC_CODE			],		
		lcInfo.szPanelInfo[LC_RUNLINE		],		
		lcInfo.szPanelInfo[LC_UNIQUEID		],		
		lcInfo.szPanelInfo[LC_PAIR_HID		],		
		lcInfo.szPanelInfo[LC_PAIR_EID		],		
		lcInfo.szPanelInfo[LC_PAIR_GRADE	],		
		lcInfo.szPanelInfo[LC_PAIR_UNIQUEID	],		
		lcInfo.szPanelInfo[LC_FLOWRECIPE	],	
		lcInfo.szPanelInfo[LC_OPID			],	
 		/*"0902061500"/ *GetDoc()->OPID* /,*/
// 		//					pInfo->m_sReserved,		
		lcInfo.szPanelInfo[LC_BITSIGNAL	],			
		lcInfo.szPanelInfo[LC_REFERDATA	],			
		lcInfo.szPanelInfo[LC_RAWPATH	],			
		lcInfo.szPanelInfo[LC_SUMPATH	],			
		lcInfo.szPanelInfo[LC_IMGPATH	],			
		lcInfo.szPanelInfo[LC_DISK	],				
		strJudgement,									// 최종 판정 Judge.//
		strCode										// 최종 판정 Code.//
		);			// 20Byte.//

	lret = SendString(strInfo, strInfo.GetLength());

	if(GetMachinePCType() == A_AMT || GetMachinePCType() == B_AMT)
	{
		// RTMS관련. AMT//
		EvtType = eVIResult;

		strInfo.Format("@IF##%d##%s##%s$",
			m_iUNI_CODE,				// UNI_CODE(A_SIDE Or B_SIDE).//	
			lcInfo.szPanelInfo[LC_RESERVED],			// 12Byte.//
			lcInfo.szPanelInfo[LC_REFERDATA]		//  4Byte.//
			);			// 40Byte.//
		lret = SendString(strInfo, strInfo.GetLength());
	}

	return lret;
}

long CFormCOMM::SendString(LPCTSTR strInfo, long nLen)
{
	CString strText;
	if(m_SocketLC.IsConnected())
	{
		long	lRet;
		lRet = XNetCtrl.SendString(strInfo, nLen);
		if(lRet == 0)
			strText = strInfo;
		else
			strText.Format("[LC Send Fail] %s", strInfo);

		CommLog(SocketName_MT, SocketName_LC, strText);
		m_LCHistory.AppendString(strInfo);
		return lRet;
	}
	else 
	{
		strText.Format("[LC Not Connected] %s", strInfo);
		CommLog(SocketName_MT, SocketName_LC, strText);
		return -1;
	}
}

void CFormCOMM::InitSocketVI()
{
	if(GetMachinePCType() == A_AMT || GetMachinePCType() == B_AMT)
	{
		if(m_SocketVI.Create(m_SocketVI.IsServer() ? m_SocketVI.GetPort():0))
		{
			m_SocketVI.SetCreated(TRUE);
			m_SocketVI.SetReciveHwnd(this->m_hWnd);
			m_SocketVI.SetSocketName(SocketName_VI);

			if(m_SocketVI.Listen() == FALSE)
				AfxMessageBox("Socket VI Listen Fail!");
		}
		else
		{
			m_SocketVI.Close();
		}
	}
}

LRESULT CFormCOMM::OnPacketReceive( WPARAM wParam, LPARAM lParam )
{
	CDataSocket *pDataSocket = (CDataSocket *)wParam;	

	if(pDataSocket->GetSocketName() == SocketName_VI)
	{
		ParseVIMessage(pDataSocket);
	}
	else if(pDataSocket->GetSocketName() == SocketName_PG)
	{
		ParsePGMessage(pDataSocket);
	}

	return 0L;
}

void CFormCOMM::LoginInfo2LC( ELogIn iID, CString strName, CString strID )
{
	CString strMsg, strTime;
	strMsg.Format("@LI##%d##%d##%s##%s", m_iUNI_CODE, iID, strID, strName);
	SendString(strMsg, strMsg.GetLength());

	CommLog(SocketName_MT, SocketName_LC, "[LoginInfo]"+strMsg);
}

void CFormCOMM::GlassStageDataDisplay()
{
}

void CFormCOMM::ParseVIMessage( CDataSocket* pDataSocket )
{
}

void CFormCOMM::ParsePGMessage( CDataSocket* pDataSocket )
{
	if(pDataSocket->GetLastLength() != sizeof(PG_INFO))
		return ;

	char* szMsg = pDataSocket->GetLastMsg();
	CString strLog = _T("");
	CString strLog1 = _T("");
	CString strLog2 = _T("");
	CString strCommand = _T("");
	PG_INFO info;
	int nOffset = 0;	// 휘도 offset 조절 용
	memcpy(&info, szMsg, sizeof(PG_INFO));

	if(info.m_sCommandID & 0x8000)
	{
		// Command
		switch(info.m_sCommand)
		{
		case CMD_OP_INFO:			// 로그인 정보.//
			strCommand.Format("CMD_OP_INFO");
			if(info.m_sStatus == 0)
			{
				info.m_sOPName[14] = NULL;	// PG에서 자꾸 OP 정보를 엉망으로 보내기 때문에 강제로 끊는다.
				info.m_sOPID[14] = NULL;	// PG에서 자꾸 OP 정보를 엉망으로 보내기 때문에 강제로 끊는다.
				strcpy_s(GetDoc()->m_pEqMainSeq->m_PGInfo.m_sOPName, info.m_sOPName);
				strcpy_s(GetDoc()->m_pEqMainSeq->m_PGInfo.m_sOPID, info.m_sOPID);
				strLog.Format("[ User Login ] Name : %s  ID : %s", info.m_sOPName, info.m_sOPID);
				CommLog(SocketName_PG, SocketName_MT, strLog);
				if(GetApp()->m_RunMode == MODE_VISION_SKIP)
					LoginInfo2LC(eLogIn, "00000000", "00000000");
				else
					LoginInfo2LC(eLogIn, info.m_sOPName, info.m_sOPID);
			}
			else
			{
				strcpy_s(GetDoc()->m_pEqMainSeq->m_PGInfo.m_sOPName, info.m_sOPName);
				strcpy_s(GetDoc()->m_pEqMainSeq->m_PGInfo.m_sOPID, info.m_sOPID);
				strLog.Format("[ User LogOut ] Name : %s  ID : %s", info.m_sOPName, info.m_sOPID);
				CommLog(SocketName_PG, SocketName_MT, strLog);
				LoginInfo2LC(eLogOut, info.m_sOPName, info.m_sOPID);
			}
			GetDoc()->m_pEqMainSeq->SaveLastGlass();
			break;
		case CMD_INSPECTION_REQ:	// 검사 요청.//
			strCommand.Format("CMD_INSPECTION_REQ");
			info.m_sCommand = CMD_INSPECTION_NO;
			strLog.Format("[Inspection Req MSG]");
			break;
		case CMD_PLC_UNLOAD:	// 검사 완료. 물류 진행해도 됨
			// Tact time 갱신
			strCommand.Format("CMD_PLC_UNLOAD");
			::SendMessage(GetMainFrame()->m_pForm[FORM_AUTO]->m_hWnd, WM_UPDATE_VI_TACT, NULL, (LPARAM)m_TactInspection.Stop(FALSE));
			strcpy_s(GetDoc()->m_pEqMainSeq->m_PGInfo.m_sJudgement, info.m_sJudgement);
			strcpy_s(GetDoc()->m_pEqMainSeq->m_PGInfo.m_sCode, info.m_sCode);
			strcpy_s(GetDoc()->m_pEqMainSeq->m_PGInfo.m_sRawPath, info.m_sRawPath);
			strcpy_s(GetDoc()->m_pEqMainSeq->m_PGInfo.m_sSumPath, info.m_sSumPath);
			strcpy_s(GetDoc()->m_pEqMainSeq->m_PGInfo.m_sImagePath, info.m_sImagePath);
			strcpy_s(GetDoc()->m_pEqMainSeq->m_PGInfo.m_sDisk, info.m_sDisk);
			GetDoc()->m_pEqMainSeq->PGDataToGlassData();

			strLog.Format("[Inspection End] [ID:%s] Judgement : %s  Code : %s", 
				GetDoc()->m_pEqMainSeq->m_PGInfo.m_sHPanelID, info.m_sJudgement, info.m_sCode);
			GetDoc()->SetLog(strLog, LOG_GLASS_DATA);
			SetTimer(TIMER_INSPECTION_END, 500, NULL);
			break;
		case CMD_DEFECT_CODE:		// 검사 결과 받음.//
			strCommand.Format("CMD_DEFECT_CODE");
			m_TactInspection.Stop(FALSE);
			strcpy_s(GetDoc()->m_pEqMainSeq->m_PGInfo.m_sJudgement, info.m_sJudgement);
			strcpy_s(GetDoc()->m_pEqMainSeq->m_PGInfo.m_sCode, info.m_sCode);
			strcpy_s(GetDoc()->m_pEqMainSeq->m_PGInfo.m_sRawPath, info.m_sRawPath);
			strcpy_s(GetDoc()->m_pEqMainSeq->m_PGInfo.m_sSumPath, info.m_sSumPath);
			strcpy_s(GetDoc()->m_pEqMainSeq->m_PGInfo.m_sImagePath, info.m_sImagePath);
			strcpy_s(GetDoc()->m_pEqMainSeq->m_PGInfo.m_sDisk, info.m_sDisk);
			GetDoc()->m_pEqMainSeq->PGDataToGlassData();
			if(info.m_sStatus == 1)
				m_bQCFlag = TRUE;
			else
				m_bQCFlag = FALSE;

			GetApp()->m_bInspectionEnd = TRUE;
			GetApp()->m_bInspectionResult = TRUE;
			GetApp()->m_bInspectionStart = FALSE;

			strLog.Format("[Inspection Result] [ID:%s] Judgement : %s  Code : %s", 
				GetDoc()->m_pEqMainSeq->m_PGInfo.m_sHPanelID, info.m_sJudgement, info.m_sCode);
			GetDoc()->SetLog(strLog, LOG_GLASS_DATA);
			break;
		case CMD_RETEST_REQ:
			strCommand.Format("CMD_RETEST_REQ");
			PGSend(CMD_CELL_INFO);
			strLog.Format("[CELL 정보 없음]");
			SetTimer(TIMER_CMD_INSPECTION_YES, 500, NULL);	// 여기서 CMD_INSPECTION_YES 전송
			break;
		case CMD_QC_SAMPLE:
			strCommand.Format("CMD_QC_SAMPLE");
			GetDoc()->m_pEqMainSeq->SetQCSampling();
			break;
// 		case CMD_SET_STOP:
// 		case CMD_RESET_STOP:
// 			pView->Send((INFO*)pInfo);
// 			break;
		case CMD_CLAMP_ON:
			strCommand.Format("CMD_CLAMP_ON");
			GetDeviceCtl()->ClampUnit(TRUE);
			break;
		case CMD_CLAMP_OFF:
			strCommand.Format("CMD_CLAMP_OFF");
			GetDeviceCtl()->ClampUnit(FALSE);
			break;
		case CMD_CONTACT_OFF: //CMD_RESET_STOP: Auto-Contact 해제
			strLog.Format("[Auto-Contact 해제]");
			KillTimer(TIMER_CONTACT_READY);
			GetDeviceCtl()->ContactUnit(UP);
			m_nContactOffTime = 0;
			SetTimer(TIMER_CONTACT_READY, 500, NULL);
			break;
		case CMD_BLU_BRIGHTNESS1: // BLU 휘도 +Offset 적용
		case CMD_BLU_BRIGHTNESS2: // BLU 휘도 -Offset 적용
			nOffset = info.m_sStatus;
			if((CMD_BLU_BRIGHTNESS2-info.m_sCommand))
				nOffset *= -1;
			strCommand.Format("CMD_BLU_BRIGHTNESS");
			strLog.Format("[휘도 조절 %d](구현 안됨)", nOffset);
			break;
		case CMD_OMIT_ON: 
			strCommand.Format("CMD_OMIT_ON");
			GetDoc()->OmitOn();
			break;
		case CMD_OMIT_OFF:
			strCommand.Format("CMD_OMIT_OFF");
// 			GetDoc()->OmitOff();
			GetDoc()->OmitOn();
			break;
		default:
			strLog.Format("[Parsing Fail] cmd ID : %X, cmd : %d", info.m_sCommandID, info.m_sCommand);
			break;
		}
		info.m_sCommandID = info.m_sCommandID & 0xfff;
		m_SocketPG.SendClient(&info, sizeof(PG_INFO));
	}
	else
	{
		strLog.Format("[Ask]\tcmd ID : %X, cmd : %d", info.m_sCommandID, info.m_sCommand);
	}
	strLog1.Format("Command:%s[%X]\t", strCommand, info.m_sCommandID);
	strLog2 = strLog1 + strLog;

	if(strLog2 != _T(""))
		CommLog(SocketName_PG, SocketName_MT, strLog2);
}

void CFormCOMM::PGSend( unsigned char sCommand, CString strError /*= _T("")*/ )
{
	if(m_SocketPG.IsConnected() == FALSE)
		return;

	PG_INFO* pinfo = &(GetDoc()->m_pEqMainSeq->m_PGInfo);
	CString strLog = _T("");
	int nStatus = 0;
	switch(sCommand) 
	{
	case CMD_STATUS_INFO:				// 연결상태.//
		pinfo->m_sCommandID = 0x8101;
		nStatus = 1;
		nStatus += GetMelsec()->IsConnected() * 2;
		nStatus += m_SocketLC.IsConnected() * 4;
		if(nStatus == pinfo->m_sStatus)
			return;
		pinfo->m_sStatus = nStatus;
		strLog = "[Status Transfer]";
		break;
	case CMD_CELL_INFO:					// CellData 전송.//
		if(GetApp()->m_RunMode == MODE_STAGE)	// 패널이 무조건 있어야 함.//
		{
			GetDoc()->m_pEqMainSeq->ClearGlassData(GetDoc()->m_pEqMainSeq->m_PanelInfo);
			memcpy(GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, "0H6D2GD07D", 10);
			memcpy(GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sEPanelID, "0H6D2GD07D", 10);
			memcpy(GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sBatchID, "7H6D2GD07D", 10);
			
// 			if(GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sDeviceID[0] == NULL 
// 				|| GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sDeviceID[0] == _T(' ')) 
				memcpy(GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sDeviceID, "LTA400HA08-W02", 14);
		}
		GetDoc()->m_pEqMainSeq->GlassDataToPGData();		// Glass Data를 PG Data로 바꾼다.//
		pinfo->m_sCommandID = 0x8101;
		strLog = "[Glass Data]";
		break;
	case CMD_INSPECTION_YES:			// 검사 시작.//
		CommLog(SocketName_MT, SocketName_PG, GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID);
		CommLog(SocketName_MT, SocketName_PG, GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sEPanelID);
		m_TactInspection.Start();
		pinfo->m_sCommandID = 0x8102;
		strLog = "[Inspection Start]";
		break;
	case CMD_ERR_MSG:					// Error.//
		pinfo->m_sCommandID = 0x8104;
		strcpy_s(pinfo->m_sAlmMessage, strError.GetBuffer(0));
		strLog = "[Error Message]";
		if(strError == "PG OFF")
			pinfo->m_sStatus = 1;	// 이렇게 하면 PG가 STOP TEST를 수행한다.
		else
			pinfo->m_sStatus = 3;
		break;
	case CMD_CONTACT_COMPLETE:			// 컨텍 완료.//
		pinfo->m_sCommandID = 0x8105;
		break;
	case CMD_MODEL_CHANGE:			// 모델 변경.//	
		pinfo->m_sCommandID = 0x8102;
		strLog = "[Device Change Start]";
		break;
	}
	pinfo->m_sCommand = sCommand;
	pinfo->m_wPacketsize = sizeof(PG_INFO);
	if(m_SocketPG.SendClient(pinfo, sizeof(PG_INFO)))
	{
		// Success
	}
	else
	{
		strLog += "\tSend Fail";
	}
	CommLog(SocketName_MT, SocketName_PG, strLog);
}

void CFormCOMM::InitSocketPG()
{
	if(GetMachinePCType() == A_MMT1 || GetMachinePCType() == A_MMT2
		|| GetMachinePCType() == B_MMT1 || GetMachinePCType() == B_MMT2)
	{
		if(m_SocketPG.Create(m_SocketPG.IsServer() ? m_SocketPG.GetPort():0))
		{
			m_SocketPG.SetCreated(TRUE);
			m_SocketPG.SetReciveHwnd(this->m_hWnd);
			m_SocketPG.SetSocketName(SocketName_PG);

			if(m_SocketPG.Listen() == FALSE)
				AfxMessageBox("Socket PG Listen Fail!");
		}
		else
			m_SocketPG.Close();
	}
}
void CFormCOMM::OnDestroy()
{
	CFormView::OnDestroy();

	m_SocketLC.Close();
	m_SocketVI.Close();
	m_SocketPG.Close();
}

void CFormCOMM::SaveSocketParam(void)
{
	CString strSection, strKeyName;
	strSection.Format("Socket Parameter");
	CIni ini;
	ini.SetIniFilename(MOTION_SOCKET_PARAM_PATH);
	ini.WriteBoolean(	strSection,		"LC IsServer",				m_SocketLC.IsServer()		);
	ini.WriteString(	strSection,		"LC IP",					m_SocketLC.GetIP()			);
	ini.WriteLong(		strSection,		"LC PORT",					m_SocketLC.GetPort()		);
	ini.WriteBoolean(	strSection,		"VI IsServer",				m_SocketVI.IsServer()		);
	ini.WriteString(	strSection, 	"VI IP",					m_SocketVI.GetIP()			);
	ini.WriteLong(		strSection, 	"VI PORT",					m_SocketVI.GetPort()		);
	ini.WriteBoolean(	strSection,		"PG IsServer",				m_SocketPG.IsServer()		);
	ini.WriteString(	strSection, 	"PG IP",					m_SocketPG.GetIP()			);
	ini.WriteLong(		strSection, 	"PG PORT",					m_SocketPG.GetPort()		);
}

void CFormCOMM::LoadSocketParam(void)
{
	CString strSection, strKeyName;
	strSection.Format("Socket Parameter");
	CIni ini;
	ini.SetIniFilename(MOTION_SOCKET_PARAM_PATH);
	m_SocketLC.SetServer(	ini.GetBoolean(strSection,		"LC IsServer",	FALSE)					)		;
	m_SocketLC.SetIP(		ini.GetString(strSection,		"LC IP",		_T("192.168.10.2"))		)		;
	m_SocketLC.SetPort(		ini.GetLong(strSection,				"LC PORT",		3100)				)		;
	m_SocketVI.SetServer(	ini.GetBoolean(strSection,		"VI IsServer",	TRUE)					)		;
	m_SocketVI.SetIP(		ini.GetString(strSection,		"VI IP",		_T("192.168.10.111"))	)		;
	m_SocketVI.SetPort(		ini.GetLong(strSection,				"VI PORT",		8000)				)		;
	m_SocketPG.SetServer(	ini.GetBoolean(strSection,		"PG IsServer",	TRUE)					)		;
	m_SocketPG.SetIP(		ini.GetString(strSection,		"PG IP",		_T("192.168.10.135"))	)		;
	m_SocketPG.SetPort(		ini.GetLong(strSection,				"PG PORT",		8000)				)		;
}

void CFormCOMM::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMER_INFO_DISPLAY_COMM: 
		if(m_SocketPG.IsConnected() && GetApp()->IsRunStatus() == FALSE)		// 연결상태를 계속 전송.//
			PGSend(CMD_STATUS_INFO);
		break;
	case TIMER_CMD_INSPECTION_YES: 
		KillTimer(TIMER_CMD_INSPECTION_YES);
		PGSend(CMD_INSPECTION_YES);
		break;
	case TIMER_INSPECTION_END:
		KillTimer(TIMER_INSPECTION_END);
		GetApp()->m_bInspectionEnd = TRUE;
		break;
	case TIMER_CONTACT_READY:
		KillTimer(TIMER_CONTACT_READY);
		if(ContactOff() == FALSE)
		{
			m_nContactOffTime++;
			// time limit
			if(m_nContactOffTime < 10)
				SetTimer(TIMER_CONTACT_READY, 100, NULL);
		}
		break;
	default:
		break;
	}

	CFormView::OnTimer(nIDEvent);
}

void CFormCOMM::CommLog( LPCTSTR szFrom, LPCTSTR szTo, LPCTSTR szText )
{
	CString strTime, strLog;
	CTime time = CTime::GetCurrentTime();
	strLog.Format("[%s -> %s] %s", szFrom, szTo, szText);
	strTime.Format("[%02d:%02d:%02d] ", time.GetHour(), time.GetMinute(), time.GetSecond());
	if(szFrom == SocketName_LC || szTo == SocketName_LC)
		m_LCHistory.AppendString(strTime + strLog);
	else
		m_VIHistory.AppendString(strTime + strLog);

	GetDoc()->SetLog(strLog, LOG_COMM);
}

void CFormCOMM::InitCommInfoGrid()
{
	long row = 0;
	// Index 증감하는게 더 불편하니 맨 밑줄 내용부터 밀어넣는다.
	m_CommInfoGrid1.put_FixedRows(0);
	m_CommInfoGrid1.put_Rows(0);
	m_CommInfoGrid1.put_ColWidth(0, 1460);
	m_CommInfoGrid1.put_ColWidth(1, 2100);
	m_CommInfoGrid1.put_ColAlignment(0, CMsflexgrid_pos::flexAlignCenterCenter);
	m_CommInfoGrid1.put_ColAlignment(1, CMsflexgrid_pos::flexAlignRightCenter);
	m_CommInfoGrid1.AddItem("HPanelID",			_variant_t(row++));
	m_CommInfoGrid1.AddItem("EPanelID",			_variant_t(row++));
	m_CommInfoGrid1.AddItem("BatchID",			_variant_t(row++));
	m_CommInfoGrid1.AddItem("Prodtype",			_variant_t(row++));
	m_CommInfoGrid1.AddItem("ProdKind",			_variant_t(row++));
	m_CommInfoGrid1.AddItem("DeviceID",			_variant_t(row++));
	m_CommInfoGrid1.AddItem("StepID",			_variant_t(row++));
	m_CommInfoGrid1.AddItem("PPID",				_variant_t(row++));
	m_CommInfoGrid1.AddItem("Thickness",		_variant_t(row++));
	m_CommInfoGrid1.AddItem("InspectionFlag",	_variant_t(row++));
	m_CommInfoGrid1.AddItem("PanelSize",		_variant_t(row++));
	m_CommInfoGrid1.AddItem("PanelPosition",	_variant_t(row++));
	m_CommInfoGrid1.AddItem("Count1",			_variant_t(row++));
	m_CommInfoGrid1.AddItem("Count2",			_variant_t(row++));
	m_CommInfoGrid1.AddItem("Grade",			_variant_t(row++));
	m_CommInfoGrid1.AddItem("RawPath",			_variant_t(row++));
	m_CommInfoGrid1.AddItem("SumPath",			_variant_t(row++));

	row = 0;
	m_CommInfoGrid2.put_FixedRows(0);
	m_CommInfoGrid2.put_Rows(0);
	m_CommInfoGrid2.put_ColWidth(0, 1460);
	m_CommInfoGrid2.put_ColWidth(1, 2100);
	m_CommInfoGrid2.put_ColAlignment(0, CMsflexgrid_pos::flexAlignCenterCenter);
	m_CommInfoGrid2.put_ColAlignment(1, CMsflexgrid_pos::flexAlignRightCenter);
	m_CommInfoGrid2.AddItem("Comment",			_variant_t(row++));
	m_CommInfoGrid2.AddItem("CompSize",			_variant_t(row++));
	m_CommInfoGrid2.AddItem("ReadingFlag",		_variant_t(row++));
	m_CommInfoGrid2.AddItem("PanelState",		_variant_t(row++));
	m_CommInfoGrid2.AddItem("Judgement",		_variant_t(row++));
	m_CommInfoGrid2.AddItem("Code",				_variant_t(row++));
	m_CommInfoGrid2.AddItem("RunLine",			_variant_t(row++));
	m_CommInfoGrid2.AddItem("UniqueID",			_variant_t(row++));
	m_CommInfoGrid2.AddItem("PairHPanelID",		_variant_t(row++));
	m_CommInfoGrid2.AddItem("PairEPanelID",		_variant_t(row++));
	m_CommInfoGrid2.AddItem("PairGrade",		_variant_t(row++));
	m_CommInfoGrid2.AddItem("PairUniqueID",		_variant_t(row++));
	m_CommInfoGrid2.AddItem("FlowRecipe",		_variant_t(row++));
	m_CommInfoGrid2.AddItem("BitsSignals",		_variant_t(row++));
	m_CommInfoGrid2.AddItem("ReferData",		_variant_t(row++));
	m_CommInfoGrid2.AddItem("ImagePath",		_variant_t(row++));
	m_CommInfoGrid2.AddItem("Disk",				_variant_t(row++));

	long nMaxRow = m_CommInfoGrid1.get_Rows();
	for(long i = 0; i < nMaxRow; i++)
	{
		m_CommInfoGrid1.put_RowHeight(i, 470);
		m_CommInfoGrid2.put_RowHeight(i, 470);
	}
}
void CFormCOMM::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFormView::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		if(m_radioCommLoaderAddr.GetCheck() == BST_CHECKED)
			OnBnClickedRadioComm1();
		else if(m_radioCommLoaderInfo.GetCheck() == BST_CHECKED)
			OnBnClickedRadioComm2();
		else if(m_radioCommPanelInfo.GetCheck() == BST_CHECKED)
			OnBnClickedRadioComm3();
		else if(m_radioCommUnloaderAddr.GetCheck() == BST_CHECKED)
			OnBnClickedRadioComm4();
	}
}

void CFormCOMM::OnBnClickedRadioComm1()
{
	PANEL_INFO info;
	GetDoc()->m_pEqMainSeq->ClearGlassData(info);
	if( !GetMainFrame()->m_bMelsecRead )
		GetMelsec()->GlassDataRead(info, DATA_INPUT_START);
	UpdateCommInfoGrid(info);
}

void CFormCOMM::OnBnClickedRadioComm2()
{
	UpdateCommInfoGrid(GetDoc()->m_pEqMainSeq->m_PanelInfoLoaderBuf);
}

void CFormCOMM::OnBnClickedRadioComm3()
{
	UpdateCommInfoGrid(GetDoc()->m_pEqMainSeq->m_PanelInfo);
}

void CFormCOMM::OnBnClickedRadioComm4()
{
	PANEL_INFO info;
	GetDoc()->m_pEqMainSeq->ClearGlassData(info);


// 	memcpy(info.m_sHPanelID, "0H6D2GD07D", 10);
// 	memcpy(info.m_sEPanelID, "0H6D2GD07D", 10);
// 	memcpy(info.m_sBatchID, "7H6D2GD07D", 10);
// 	memcpy(info.m_sDeviceID, "LTM460HA07-C01", 14);
// 	GetMelsec()->GlassDataWrite(info, DATA_OUTPUT_START);
// 	GetDoc()->m_pEqMainSeq->ClearGlassData(info);


	if( !GetMainFrame()->m_bMelsecRead )
		GetMelsec()->GlassDataRead(info, DATA_OUTPUT_START);
	UpdateCommInfoGrid(info);
}

void CFormCOMM::UpdateCommInfoGrid( PANEL_INFO info )
{
	int row = 0;

	LC_PANEL_INFO lcInfo;
	lcInfo = info;
	for(int i = 0; i < 16; i++)
	{
		m_CommInfoGrid1.put_TextMatrix(i,	1,	lcInfo.szPanelInfo[i]		);		
		m_CommInfoGrid2.put_TextMatrix(i,	1,	lcInfo.szPanelInfo[i+LC_COMMENT]		);		
	}
}

void CFormCOMM::NonStop2LC( int nBitAddress, CString strDeviceID )
{
	// LC 보고
	CString strMsg;
	int nEventID = 0;

	if(nBitAddress = NON_STOP_PC_START)
		nEventID = 54;
	else if(nBitAddress = NON_STOP_PC_END)
		nEventID = 55;
	else if(nBitAddress = NON_STOP_PC_FAIL)
		nEventID = 56;

	strMsg.Format("@UJ##%d##%d##%d##%s$",
//		m_iUNI_CODE,					// A Or B열 구분.// 추후 구현 예정
		nEventID,
		m_iCurEQState,
		m_iCurProcessState,						// 경알람 0, 중알람 1.//
		strDeviceID);

	SendString(strMsg, strMsg.GetLength());
}

int CFormCOMM::LoadLastEQState()
{
	CIni ini(LC_INFO_PATH);

	return (int)ini.GetLong("EQState", "EQState", ePM);
}

void CFormCOMM::SaveLastEQState( int nEQStateNo )
{
	CIni ini(LC_INFO_PATH);

	ini.WriteLong("EQState", "EQState", (int)nEQStateNo);
	int b = 0;
	b = ini.GetLong("EQState", "EQState", eEqNothing);
	if (nEQStateNo != b)
	{
		CString strTemp;
		strTemp.Format("Panel Defferent :  %d",  nEQStateNo);
		GetDoc()->SetLog(strTemp,LOG_COMM);
	}
}

CString CFormCOMM::LoadLastEQProcessPacket()
{
	CIni ini(LC_INFO_PATH);

	return ini.GetString("EQProcess", "EQProcess", "");
}

int CFormCOMM::LoadLastEQProcess()
{
	CIni ini(LC_INFO_PATH);

	return (int)ini.GetLong("EQProcess", "ID", ePause);
}

void CFormCOMM::SaveLastEQProcess( int process, CString strProcess )
{
	CIni ini(LC_INFO_PATH);

	ini.WriteString("EQProcess", "EQProcess", strProcess);
	ini.WriteLong("EQProcess", "ID", (int)process);

	int b = 0;
	b = ini.GetLong("EQProcess", "ID", ePNothing);
	if (process != b)
	{
		CString strTemp;
		strTemp.Format("Panel Defferent :  %d",  process);
		GetDoc()->SetLog(strTemp,LOG_COMM);
	}
}

BOOL CFormCOMM::ContactOff()
{
	if ( GetDeviceCtl()->ChkContactUnitUp() == ERR_NONE )
	{
		GetDeviceCtl()->MoveYReady();
		return TRUE;
	}
	else
		return FALSE;
}
