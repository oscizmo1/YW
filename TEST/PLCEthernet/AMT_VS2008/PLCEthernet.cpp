// PLCEthernet.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "PLCEthernet.h"
#include "MainFrm.h"

/*
UINT PLCMonitorProc(LPVOID lParam)
{
	CPLCEthernet* pDlg = (CPLCEthernet* )lParam;
	CString strDevice;
	// 여기서 실제 PLC와 Data를 주고받는다.
	// 조금 읽으나 많이 읽으나 속도는 똑같으므로
	// 읽을 때는 모든 영역을 읽고
	// 적을 때는 자기 영역만 적는다.
	// (PLC 영역은 써도 안써지니 상관없지만 다른 PC영역까지 적으면 곤란..)
	// 1 Loop에 약 0.23ms 정도 소요
// 	CStopWatch a;
// 	a.Start();

	while(pDlg->m_bThreadRun)
	{
		// Bit
		strDevice = _T("B000");		
// 		pDlg->m_ActQJ71E71.ReadDeviceBlock2(strDevice, MAX_PLC_BIT_SIZE, pDlg->m_bitRecv);
		pDlg->m_ActQJ71E71Udp.ReadDeviceBlock2(strDevice, MAX_PLC_BIT_SIZE, pDlg->m_bitRecv);
		strDevice.Format(_T("B%03X"), UNLOAD_OUTPUT_START);	
// 		pDlg->m_ActQJ71E71.WriteDeviceBlock2(strDevice, USE_PLC_BIT_SIZE, &pDlg->m_bitSend[UNLOAD_OUTPUT_START/0x10]);
		pDlg->m_ActQJ71E71Udp.WriteDeviceBlock2(strDevice, USE_PLC_BIT_SIZE, &pDlg->m_bitSend[UNLOAD_OUTPUT_START/0x10]);

//		// Word 영역은 읽고 쓰는데 너무 오래 걸려서  [5/6/2009 OSC]
//		// GlassDataRead()와 GlassDataWrite()에서 직접 호출한다.
//		strDevice = _T("W000");		
//		pDlg->m_ActQJ71E71.ReadDeviceBlock2(strDevice, MAX_PLC_WORD_SIZE, pDlg->m_WordRecv);
//		strDevice.Format(_T("W%03X"), DATA_OUTPUT_START);	
//		pDlg->m_ActQJ71E71.WriteDeviceBlock2(strDevice, USE_PLC_WORD_SIZE, &pDlg->m_WordSend[DATA_OUTPUT_START]);

		Sleep(10);
// 		TRACE("%f\n", a.Stop(TRUE));
	}
	pDlg->m_pThread = NULL;
	return 0L;
}
*/
// CPLCEthernet dialog

IMPLEMENT_DYNAMIC(CPLCEthernet, CDialog)

CPLCEthernet::CPLCEthernet(CWnd* pParent /*=NULL*/)
	: CDialog(CPLCEthernet::IDD, pParent)
{
	memset(m_bitRecv, 0, MAX_PLC_BIT_SIZE);
	memset(m_bitSend, 0, MAX_PLC_BIT_SIZE);
	memset(m_WordRecv, 0, MAX_PLC_WORD_SIZE);
	memset(m_WordSend, 0, MAX_PLC_WORD_SIZE);
}

CPLCEthernet::~CPLCEthernet()
{
}

void CPLCEthernet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ACTQJ71E71TCP1, m_ActQJ71E71);
	DDX_Control(pDX, IDC_ACTQJ71E71UDP1, m_ActQJ71E71Udp);
}

BEGIN_MESSAGE_MAP(CPLCEthernet, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CPLCEthernet message handlers


void CPLCEthernet::EthernetProc()
{
	// Bit
	strDevice = _T("B000");		
	if( !GetMainFrame()->m_bMelsecRead )
	{
		GetMainFrame()->m_bMelsecRead = TRUE;
		// 		pDlg->m_ActQJ71E71.ReadDeviceBlock2(strDevice, MAX_PLC_BIT_SIZE, pDlg->m_bitRecv);
		m_ActQJ71E71Udp.ReadDeviceBlock2(strDevice, MAX_PLC_BIT_SIZE, m_bitRecv);
		GetMainFrame()->m_bMelsecRead = FALSE;
	}

	strDevice.Format(_T("B%03X"), UNLOAD_OUTPUT_START);	
	if( !GetMainFrame()->m_bMelsecWrite )
	{
		GetMainFrame()->m_bMelsecWrite = TRUE;
		// 		pDlg->m_ActQJ71E71.WriteDeviceBlock2(strDevice, USE_PLC_BIT_SIZE, &pDlg->m_bitSend[UNLOAD_OUTPUT_START/0x10]);
		m_ActQJ71E71Udp.WriteDeviceBlock2(strDevice, USE_PLC_BIT_SIZE, &m_bitSend[UNLOAD_OUTPUT_START/0x10]);
		GetMainFrame()->m_bMelsecWrite = FALSE;
	}

	/*		// Word 영역은 읽고 쓰는데 너무 오래 걸려서  [5/6/2009 OSC]
	// GlassDataRead()와 GlassDataWrite()에서 직접 호출한다.
	strDevice = _T("W000");		
	pDlg->m_ActQJ71E71.ReadDeviceBlock2(strDevice, MAX_PLC_WORD_SIZE, pDlg->m_WordRecv);
	strDevice.Format(_T("W%03X"), DATA_OUTPUT_START);	
	pDlg->m_ActQJ71E71.WriteDeviceBlock2(strDevice, USE_PLC_WORD_SIZE, &pDlg->m_WordSend[DATA_OUTPUT_START]);
	*/
	// 		TRACE("%f\n", a.Stop(TRUE));
	GetMainFrame()->SetTimer(PLC_ETHERNET, 50, NULL);
}

BOOL CPLCEthernet::Open()
{
	// MachineType별로 고정해버렸다 ㅋㅋㅋㅋ [5/19/2009 OSC]
	int nStationNo = 0;
	int nHostStationNo = 1;
	int nPortNo = 0;
	int nNetworkNo = 2;
	switch(GetMachinePCType())
	{
	case A_MMT1:
		nStationNo = 6;
	//	nPortNo = 5000;
		nPortNo = 4000;
		break;
	case A_MMT2:
		nStationNo = 7;
	//	nPortNo = 5000;
		nPortNo = 4001;
		break;
	case B_MMT1:
		nStationNo = 4;
	//	nPortNo = 5000;
		nPortNo = 4002;
		break;
	case B_MMT2:
		nStationNo = 5;
	//	nPortNo = 5000;
		nPortNo = 4003;
		break;
	}

	SetConnected(FALSE);
	if(this->Create(IDD_PLC_ETHERNET))
	{
		this->ShowWindow(SW_HIDE);

		/*// TCP
		{
			//////////////////////////////////////////////////////////////////////////
			// Host
			m_ActQJ71E71.put_ActCpuType(GetDoc()->m_nCpuType);
			m_ActQJ71E71.put_ActHostAddress(GetDoc()->m_strHostIP);
			m_ActQJ71E71.put_ActNetworkNumber(nNetworkNo);
			m_ActQJ71E71.put_ActStationNumber(nHostStationNo);
			//////////////////////////////////////////////////////////////////////////
			// PC
			m_ActQJ71E71.put_ActSourceNetworkNumber(nNetworkNo);
			m_ActQJ71E71.put_ActSourceStationNumber(nStationNo);
		//	m_ActQJ71E71.put_ActPortNumber(nPortNo);
			//////////////////////////////////////////////////////////////////////////
			m_ActQJ71E71.put_ActTimeOut(5000);
			long nRet = m_ActQJ71E71.Open();
			if(nRet == 0)
			{
				m_bThreadRun = TRUE;
				m_pThread = ::AfxBeginThread(PLCMonitorProc, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
				SetConnected(TRUE);
			}
			else
			{
				AfxMessageBox("PLC Not Connected!");
			}
		}//*/

		// UDP
		{
			//////////////////////////////////////////////////////////////////////////
			// Host
			m_ActQJ71E71Udp.put_ActCpuType(GetDoc()->m_nCpuType);
			m_ActQJ71E71Udp.put_ActHostAddress(GetDoc()->m_strHostIP);
			m_ActQJ71E71Udp.put_ActNetworkNumber(nNetworkNo);
			m_ActQJ71E71Udp.put_ActStationNumber(nHostStationNo);
			//////////////////////////////////////////////////////////////////////////
			// PC
			m_ActQJ71E71Udp.put_ActSourceNetworkNumber(nNetworkNo);
			m_ActQJ71E71Udp.put_ActSourceStationNumber(nStationNo);
			m_ActQJ71E71Udp.put_ActPortNumber(nPortNo);
			//////////////////////////////////////////////////////////////////////////
			m_ActQJ71E71Udp.put_ActTimeOut(5000);
			long nRet = m_ActQJ71E71Udp.Open();
			if(nRet == 0)
			{
				//m_bThreadRun = TRUE;
				//m_pThread = ::AfxBeginThread(PLCMonitorProc, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
				GetMainFrame()->SetTimer(PLC_ETHERNET, 50, NULL);
				SetConnected(TRUE);
			}
			else
			{
				AfxMessageBox("PLC Not Connected!");
			}
		}//*/
	}

	return IsConnected();
}

void CPLCEthernet::Close()
{
	if(IsConnected())
	{
		SetConnected(FALSE);
		m_ActQJ71E71Udp.Close();
		//m_ActQJ71E71.Close();
	}

	GetMainFrame()->KillTimer(PLC_ETHERNET);
// 	m_bThreadRun = FALSE;
// 	do 
// 	{
// 		Sleep(100);
// 	} while (m_pThread);
	DestroyWindow();
}

long CPLCEthernet::MelsecNetBitOn( short devno )
{
	if(IsConnected() == FALSE)		return -1;
	int nIndex = devno/0x10;
	short oldvalue = m_bitSend[nIndex];
	m_bitSend[nIndex] = oldvalue | (0x0001 << (devno%0x10));
	return 0;

}

long CPLCEthernet::MelsecNetBitOff( short devno )
{
	if(IsConnected() == FALSE)		return -1;

	int nIndex = devno/0x10;
	short oldvalue = m_bitSend[nIndex];
	short nbit = 0x0001 << (devno%0x10);
	nbit = ~nbit;
	m_bitSend[nIndex] = oldvalue & nbit;
	return 0;
}

short CPLCEthernet::MelsecNetBitRecv( short devno )
{
	if(IsConnected() == FALSE)		return 0;
	short ret = m_bitRecv[devno/0x10];
	ret = (ret & (0x0001 << (devno%0x10))) ? TRUE : FALSE;
	return ret;
}

void CPLCEthernet::GlassDataRead( PANEL_INFO &panelinfo, WORD wStartAddr )
{
	if(IsConnected() == FALSE)		return;

	CString strDevice = _T("W000");		
	m_ActQJ71E71Udp.ReadDeviceBlock2(strDevice, MAX_PLC_WORD_SIZE, m_WordRecv);
	//m_ActQJ71E71.ReadDeviceBlock2(strDevice, MAX_PLC_WORD_SIZE, m_WordRecv);
	GetMainFrame()->m_bMelsecRead = FALSE;

	memcpy(panelinfo.m_sHPanelID		, &m_WordRecv[wStartAddr], sizeHPANELID		);	wStartAddr += sizeHPANELID/2		;
	memcpy(panelinfo.m_sEPanelID		, &m_WordRecv[wStartAddr], sizeEPANELID		);	wStartAddr += sizeEPANELID/2		;
	memcpy(panelinfo.m_sBatchID			, &m_WordRecv[wStartAddr], sizeBATCHID		);	wStartAddr += sizeBATCHID/2			;
	memcpy(panelinfo.m_sProdType		, &m_WordRecv[wStartAddr], sizePRODTYP		);	wStartAddr += sizePRODTYP/2			;
	memcpy(panelinfo.m_sProdKind		, &m_WordRecv[wStartAddr], sizePRODKND		);	wStartAddr += sizePRODKND/2			;
	memcpy(panelinfo.m_sDeviceID		, &m_WordRecv[wStartAddr], sizeDEVICEID		);	wStartAddr += sizeDEVICEID/2		;
	memcpy(panelinfo.m_sStepID			, &m_WordRecv[wStartAddr], sizeSTEPID		);	wStartAddr += sizeSTEPID/2			;
	memcpy(panelinfo.m_sPPID			, &m_WordRecv[wStartAddr], sizePPID			);	wStartAddr += sizePPID/2			;
	memcpy(panelinfo.m_sThickness		, &m_WordRecv[wStartAddr], sizeTHICKNESS	);	wStartAddr += sizeTHICKNESS/2		;
	memcpy(panelinfo.m_sInspectionFlag	, &m_WordRecv[wStartAddr], sizeINSPFLG		);	wStartAddr += sizeINSPFLG/2			;
	memcpy(panelinfo.m_sPanelSize		, &m_WordRecv[wStartAddr], sizePANELSIZE	);	wStartAddr += sizePANELSIZE/2		;
	memcpy(panelinfo.m_sPanelPosition	, &m_WordRecv[wStartAddr], sizePANELPOS		);	wStartAddr += sizePANELPOS/2		;
	memcpy(panelinfo.m_sCount1			, &m_WordRecv[wStartAddr], sizeCOUNT1		);	wStartAddr += sizeCOUNT1/2			;
	memcpy(panelinfo.m_sCount2			, &m_WordRecv[wStartAddr], sizeCOUNT2		);	wStartAddr += sizeCOUNT2/2			;
	memcpy(panelinfo.m_sGrade			, &m_WordRecv[wStartAddr], sizeGRADE/2		);	wStartAddr += sizeGRADE/2			;
	memcpy(panelinfo.m_sComment			, &m_WordRecv[wStartAddr], sizeCOMMENT		);	wStartAddr += sizeCOMMENT/2			;
	memcpy(panelinfo.m_sCompSize		, &m_WordRecv[wStartAddr], sizeCOMPSIZE		);	wStartAddr += sizeCOMPSIZE/2		;
	memcpy(panelinfo.m_sReadingFlag		, &m_WordRecv[wStartAddr], sizeREADFLG		);	wStartAddr += sizeREADFLG/2			;
	memcpy(panelinfo.m_sPanelState		, &m_WordRecv[wStartAddr], sizePANELSTATE	);	wStartAddr += sizePANELSTATE/2		;
	memcpy(panelinfo.m_sJudgement		, &m_WordRecv[wStartAddr], sizeJUDGEMENT	);	wStartAddr += sizeJUDGEMENT/2		;
	memcpy(panelinfo.m_sCode			, &m_WordRecv[wStartAddr], sizeCODE/2		);	wStartAddr += sizeCODE/2			;
	memcpy(panelinfo.m_sRunLine			, &m_WordRecv[wStartAddr], sizeRUNLINE		);	wStartAddr += sizeRUNLINE/2			;
	memcpy(panelinfo.m_sUniqueID		, &m_WordRecv[wStartAddr], sizeUNIQUEID		);	wStartAddr += sizeUNIQUEID/2		;
	memcpy(panelinfo.m_sPairHPanelID	, &m_WordRecv[wStartAddr], sizePAIR_HID		);	wStartAddr += sizePAIR_HID/2		;
	memcpy(panelinfo.m_sPairEPanelID	, &m_WordRecv[wStartAddr], sizePAIR_EID		);	wStartAddr += sizePAIR_EID/2		;
	memcpy(panelinfo.m_sPairGrade		, &m_WordRecv[wStartAddr], sizePAIR_GRADE	);	wStartAddr += sizePAIR_GRADE/2		;
	memcpy(panelinfo.m_sPairUniqueID	, &m_WordRecv[wStartAddr], sizePAIR_UNIQUEID);	wStartAddr += sizePAIR_UNIQUEID/2	;
	memcpy(panelinfo.m_sFlowRecipe		, &m_WordRecv[wStartAddr], sizeFLOWRECIPE	);	wStartAddr += sizeFLOWRECIPE/2		;
	memcpy(panelinfo.m_sReserved		, &m_WordRecv[wStartAddr], sizeRESERVED		);	wStartAddr += sizeRESERVED/2		;
	memcpy(panelinfo.m_sBitsSignals		, &m_WordRecv[wStartAddr], sizeBITSIGNAL	);	wStartAddr += sizeBITSIGNAL/2		;
	memcpy(panelinfo.m_sReferData		, &m_WordRecv[wStartAddr], sizeREFERDATA	);
													 
}

void CPLCEthernet::GlassDataWrite( PANEL_INFO &panelinfo, WORD wStartAddr )
{
	if(IsConnected() == FALSE)		return;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sHPanelID		,	sizeHPANELID		);	wStartAddr += sizeHPANELID/2		;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sEPanelID		,	sizeEPANELID		);	wStartAddr += sizeEPANELID/2		;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sBatchID		,	sizeBATCHID			);	wStartAddr += sizeBATCHID/2			;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sProdType		,	sizePRODTYP			);	wStartAddr += sizePRODTYP/2			;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sProdKind		,	sizePRODKND			);	wStartAddr += sizePRODKND/2			;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sDeviceID		,	sizeDEVICEID		);	wStartAddr += sizeDEVICEID/2		;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sStepID			,	sizeSTEPID			);	wStartAddr += sizeSTEPID/2			;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sPPID			,	sizePPID			);	wStartAddr += sizePPID/2			;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sThickness		,	sizeTHICKNESS		);	wStartAddr += sizeTHICKNESS/2		;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sInspectionFlag	,	sizeINSPFLG			);	wStartAddr += sizeINSPFLG/2			;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sPanelSize		,	sizePANELSIZE		);	wStartAddr += sizePANELSIZE/2		;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sPanelPosition	,	sizePANELPOS		);	wStartAddr += sizePANELPOS/2		;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sCount1			,	sizeCOUNT1			);	wStartAddr += sizeCOUNT1/2			;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sCount2			,	sizeCOUNT2			);	wStartAddr += sizeCOUNT2/2			;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sGrade			,	sizeGRADE			);	wStartAddr += sizeGRADE/2			;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sComment		,	sizeCOMMENT			);	wStartAddr += sizeCOMMENT/2			;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sCompSize		,	sizeCOMPSIZE		);	wStartAddr += sizeCOMPSIZE/2		;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sReadingFlag	,	sizeREADFLG			);	wStartAddr += sizeREADFLG/2			;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sPanelState		,	sizePANELSTATE		);	wStartAddr += sizePANELSTATE/2		;
	memcpy(&m_WordSend[wStartAddr], 	" "							,	sizeJUDGEMENT		);
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sJudgement		,	sizeJUDGEMENT-2		);	wStartAddr += sizeJUDGEMENT/2		;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sCode			,	sizeCODE			);	wStartAddr += sizeCODE/2			;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sRunLine		,	sizeRUNLINE			);	wStartAddr += sizeRUNLINE/2			;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sUniqueID		,	sizeUNIQUEID		);	wStartAddr += sizeUNIQUEID/2		;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sPairHPanelID	,	sizePAIR_HID		);	wStartAddr += sizePAIR_HID/2		;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sPairEPanelID	,	sizePAIR_EID		);	wStartAddr += sizePAIR_EID/2		;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sPairGrade		,	sizePAIR_GRADE		);	wStartAddr += sizePAIR_GRADE/2		;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sPairUniqueID	,	sizePAIR_UNIQUEID	);	wStartAddr += sizePAIR_UNIQUEID/2	;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sFlowRecipe		,	sizeFLOWRECIPE		);	wStartAddr += sizeFLOWRECIPE/2		;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sReserved		,	sizeRESERVED		);	wStartAddr += sizeRESERVED/2		;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sBitsSignals	,	sizeBITSIGNAL		);	wStartAddr += sizeBITSIGNAL/2		;
	memcpy(&m_WordSend[wStartAddr], 	panelinfo.m_sReferData		,	sizeREFERDATA		);

	CString strDevice = _T("W000");		
	strDevice.Format(_T("W%03X"), DATA_OUTPUT_START);	
	m_ActQJ71E71Udp.WriteDeviceBlock2(strDevice, USE_PLC_WORD_SIZE, &m_WordSend[DATA_OUTPUT_START]);
	//m_ActQJ71E71.WriteDeviceBlock2(strDevice, USE_PLC_WORD_SIZE, &m_WordSend[DATA_OUTPUT_START]);
	GetMainFrame()->m_bMelsecWrite = FALSE;
}

CString CPLCEthernet::NonStopDeviceRead()
{
	CString strDeviceID;
	CString strDevice = _T("W000");		
	m_ActQJ71E71Udp.ReadDeviceBlock2(strDevice, MAX_PLC_WORD_SIZE, m_WordRecv);
	//m_ActQJ71E71.ReadDeviceBlock2(strDevice, MAX_PLC_WORD_SIZE, m_WordRecv);
	GetMainFrame()->m_bMelsecRead = FALSE;
	char buf[sizeNonStopDeviceID+1];
	memcpy(buf, &m_WordRecv[NON_STOP_DEVICEID], sizeNonStopDeviceID	);
	buf[sizeNonStopDeviceID] = NULL;
	strDeviceID.Format(_T("%s"), buf);
	strDeviceID.Trim(_T(" "));
	strDeviceID.Trim(_T("	"));
	return strDeviceID;
}

void CPLCEthernet::NonStopFailTypeWrite( LPCTSTR szFailType )
{
	memcpy(&m_WordSend[NON_STOP_FAIL_TYPE], szFailType,	strlen(szFailType)	);

	CString strDevice = _T("W000");		
	strDevice.Format(_T("W%03X"), NON_STOP_FAIL_TYPE);	
	m_ActQJ71E71Udp.WriteDeviceBlock2(strDevice, sizeNonStopFailType, &m_WordSend[NON_STOP_FAIL_TYPE]);
	//m_ActQJ71E71.WriteDeviceBlock2(strDevice, sizeNonStopFailType, &m_WordSend[NON_STOP_FAIL_TYPE]);
	GetMainFrame()->m_bMelsecWrite = FALSE;
}

void CPLCEthernet::MelsecNetPosRead(short devno, double &dPos)
{
	CString strDevice = _T("W000");		
	m_ActQJ71E71Udp.ReadDeviceBlock2(strDevice, MAX_PLC_WORD_SIZE, m_WordRecv);
	//m_ActQJ71E71.ReadDeviceBlock2(strDevice, MAX_PLC_WORD_SIZE, m_WordRecv);

	BYTE buf[4];

	dPos = 0.0;

	memcpy( buf, &m_WordRecv[devno], 4 );
	

	dPos = (buf[0] + (buf[1] * 0x100) + (buf[2] * 0x10000) + (buf[3] * 0x1000000)) / 10000.;
}

void CPLCEthernet::MelsecNetWordSend(short devno, short num)
{
	m_WordSend[devno] = num;
	m_WordSend[devno+1] = 0; // 1word로 충분하여 2word 영역은 0으로...

	CString strDevice = _T("W000");		
	strDevice.Format(_T("W%03X"), devno);	
	m_ActQJ71E71Udp.WriteDeviceBlock2(strDevice, USE_PLC_WORD_SIZE, &m_WordSend[devno]);
	//m_ActQJ71E71.WriteDeviceBlock2(strDevice, USE_PLC_WORD_SIZE, &m_WordSend[devno]);

}