// dlgMelSecMonitor.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "dlgMelSecMonitor.h"

/////////////////////////////////////////////////////////////////////////////
// CdlgMelSecMonitor dialog


CdlgMelSecMonitor::CdlgMelSecMonitor(CWnd* pParent /*=NULL*/)
	: CDialog(CdlgMelSecMonitor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CdlgMelSecMonitor)
	m_StationNo = 0;
	//}}AFX_DATA_INIT
}


void CdlgMelSecMonitor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CdlgMelSecMonitor)
	DDX_Radio(pDX, IDC_RADIO1, m_StationNo);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_0_0 , m_Check0[0 ]);
	DDX_Control(pDX, IDC_CHECK_0_1 , m_Check0[1 ]);
	DDX_Control(pDX, IDC_CHECK_0_2 , m_Check0[2 ]);
	DDX_Control(pDX, IDC_CHECK_0_3 , m_Check0[3 ]);
	DDX_Control(pDX, IDC_CHECK_0_4 , m_Check0[4 ]);
	DDX_Control(pDX, IDC_CHECK_0_5 , m_Check0[5 ]);
	DDX_Control(pDX, IDC_CHECK_0_6 , m_Check0[6 ]);
	DDX_Control(pDX, IDC_CHECK_0_7 , m_Check0[7 ]);
	DDX_Control(pDX, IDC_CHECK_0_8 , m_Check0[8 ]);
	DDX_Control(pDX, IDC_CHECK_0_9 , m_Check0[9 ]);
	DDX_Control(pDX, IDC_CHECK_0_10, m_Check0[10]);
	DDX_Control(pDX, IDC_CHECK_0_11, m_Check0[11]);
	DDX_Control(pDX, IDC_CHECK_0_12, m_Check0[12]);
	DDX_Control(pDX, IDC_CHECK_0_13, m_Check0[13]);
	DDX_Control(pDX, IDC_CHECK_0_14, m_Check0[14]);
	DDX_Control(pDX, IDC_CHECK_0_15, m_Check0[15]);
	DDX_Control(pDX, IDC_CHECK_1_0 , m_Check1[0 ]);
	DDX_Control(pDX, IDC_CHECK_1_1 , m_Check1[1 ]);
	DDX_Control(pDX, IDC_CHECK_1_2 , m_Check1[2 ]);
	DDX_Control(pDX, IDC_CHECK_1_3 , m_Check1[3 ]);
	DDX_Control(pDX, IDC_CHECK_1_4 , m_Check1[4 ]);
	DDX_Control(pDX, IDC_CHECK_1_5 , m_Check1[5 ]);
	DDX_Control(pDX, IDC_CHECK_1_6 , m_Check1[6 ]);
	DDX_Control(pDX, IDC_CHECK_1_7 , m_Check1[7 ]);
	DDX_Control(pDX, IDC_CHECK_1_8 , m_Check1[8 ]);
	DDX_Control(pDX, IDC_CHECK_1_9 , m_Check1[9 ]);
	DDX_Control(pDX, IDC_CHECK_1_10, m_Check1[10]);
	DDX_Control(pDX, IDC_CHECK_1_11, m_Check1[11]);
	DDX_Control(pDX, IDC_CHECK_1_12, m_Check1[12]);
	DDX_Control(pDX, IDC_CHECK_1_13, m_Check1[13]);
	DDX_Control(pDX, IDC_CHECK_1_14, m_Check1[14]);
	DDX_Control(pDX, IDC_CHECK_1_15, m_Check1[15]);
	DDX_Control(pDX, IDC_CHECK_2_0 , m_Check2[0 ]);
	DDX_Control(pDX, IDC_CHECK_2_1 , m_Check2[1 ]);
	DDX_Control(pDX, IDC_CHECK_2_2 , m_Check2[2 ]);
	DDX_Control(pDX, IDC_CHECK_2_3 , m_Check2[3 ]);
	DDX_Control(pDX, IDC_CHECK_2_4 , m_Check2[4 ]);
	DDX_Control(pDX, IDC_CHECK_2_5 , m_Check2[5 ]);
	DDX_Control(pDX, IDC_CHECK_2_6 , m_Check2[6 ]);
	DDX_Control(pDX, IDC_CHECK_2_7 , m_Check2[7 ]);
	DDX_Control(pDX, IDC_CHECK_2_8 , m_Check2[8 ]);
	DDX_Control(pDX, IDC_CHECK_2_9 , m_Check2[9 ]);
	DDX_Control(pDX, IDC_CHECK_2_10, m_Check2[10]);
	DDX_Control(pDX, IDC_CHECK_2_11, m_Check2[11]);
	DDX_Control(pDX, IDC_CHECK_2_12, m_Check2[12]);
	DDX_Control(pDX, IDC_CHECK_2_13, m_Check2[13]);
	DDX_Control(pDX, IDC_CHECK_2_14, m_Check2[14]);
	DDX_Control(pDX, IDC_CHECK_2_15, m_Check2[15]);
	DDX_Control(pDX, IDC_CHECK_3_0 , m_Check3[0 ]);
	DDX_Control(pDX, IDC_CHECK_3_1 , m_Check3[1 ]);
	DDX_Control(pDX, IDC_CHECK_3_2 , m_Check3[2 ]);
	DDX_Control(pDX, IDC_CHECK_3_3 , m_Check3[3 ]);
	DDX_Control(pDX, IDC_CHECK_3_4 , m_Check3[4 ]);
	DDX_Control(pDX, IDC_CHECK_3_5 , m_Check3[5 ]);
	DDX_Control(pDX, IDC_CHECK_3_6 , m_Check3[6 ]);
	DDX_Control(pDX, IDC_CHECK_3_7 , m_Check3[7 ]);
	DDX_Control(pDX, IDC_CHECK_3_8 , m_Check3[8 ]);
	DDX_Control(pDX, IDC_CHECK_3_9 , m_Check3[9 ]);
	DDX_Control(pDX, IDC_CHECK_3_10, m_Check3[10]);
	DDX_Control(pDX, IDC_CHECK_3_11, m_Check3[11]);
	DDX_Control(pDX, IDC_CHECK_3_12, m_Check3[12]);
	DDX_Control(pDX, IDC_CHECK_3_13, m_Check3[13]);
	DDX_Control(pDX, IDC_CHECK_3_14, m_Check3[14]);
	DDX_Control(pDX, IDC_CHECK_3_15, m_Check3[15]);
	DDX_Control(pDX, IDC_CHECK_4_0 , m_Check4[0 ]);
	DDX_Control(pDX, IDC_CHECK_4_1 , m_Check4[1 ]);
	DDX_Control(pDX, IDC_CHECK_4_2 , m_Check4[2 ]);
	DDX_Control(pDX, IDC_CHECK_4_3 , m_Check4[3 ]);
	DDX_Control(pDX, IDC_CHECK_4_4 , m_Check4[4 ]);
	DDX_Control(pDX, IDC_CHECK_4_5 , m_Check4[5 ]);
	DDX_Control(pDX, IDC_CHECK_4_6 , m_Check4[6 ]);
	DDX_Control(pDX, IDC_CHECK_4_7 , m_Check4[7 ]);
	DDX_Control(pDX, IDC_CHECK_4_8 , m_Check4[8 ]);
	DDX_Control(pDX, IDC_CHECK_4_9 , m_Check4[9 ]);
	DDX_Control(pDX, IDC_CHECK_4_10, m_Check4[10]);
	DDX_Control(pDX, IDC_CHECK_4_11, m_Check4[11]);
	DDX_Control(pDX, IDC_CHECK_4_12, m_Check4[12]);
	DDX_Control(pDX, IDC_CHECK_4_13, m_Check4[13]);
	DDX_Control(pDX, IDC_CHECK_4_14, m_Check4[14]);

	DDX_Control(pDX, IDC_CHECK_4_15, m_Check4[15]);
	
	DDX_Control(pDX, IDC_CHECK_3_16, m_Check5[4]);
	DDX_Control(pDX, IDC_CHECK_3_17, m_Check5[5]);
	DDX_Control(pDX, IDC_CHECK_4_16, m_Check6[4]);
	DDX_Control(pDX, IDC_CHECK_4_17, m_Check6[5]);
}


BEGIN_MESSAGE_MAP(CdlgMelSecMonitor, CDialog)
	//{{AFX_MSG_MAP(CdlgMelSecMonitor)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_BN_CLICKED(IDC_RADIO6, OnRadio6)
	ON_BN_CLICKED(IDC_CHECK_0_0 , OnCheck00 )
	ON_BN_CLICKED(IDC_CHECK_0_1 , OnCheck01 )
	ON_BN_CLICKED(IDC_CHECK_0_2 , OnCheck02 )
	ON_BN_CLICKED(IDC_CHECK_0_3 , OnCheck03 )
	ON_BN_CLICKED(IDC_CHECK_0_4 , OnCheck04 )
	ON_BN_CLICKED(IDC_CHECK_0_5 , OnCheck05 )
	ON_BN_CLICKED(IDC_CHECK_0_6 , OnCheck06 )
	ON_BN_CLICKED(IDC_CHECK_0_7 , OnCheck07 )
	ON_BN_CLICKED(IDC_CHECK_0_8 , OnCheck08 )
	ON_BN_CLICKED(IDC_CHECK_0_9 , OnCheck09 )
	ON_BN_CLICKED(IDC_CHECK_0_10, OnCheck010)
	ON_BN_CLICKED(IDC_CHECK_0_11, OnCheck011)
	ON_BN_CLICKED(IDC_CHECK_0_12, OnCheck012)
	ON_BN_CLICKED(IDC_CHECK_0_13, OnCheck013)
	ON_BN_CLICKED(IDC_CHECK_0_14, OnCheck014)
	ON_BN_CLICKED(IDC_CHECK_0_15, OnCheck015)
	ON_BN_CLICKED(IDC_CHECK_1_0 , OnCheck10 )
	ON_BN_CLICKED(IDC_CHECK_1_1 , OnCheck11 )
	ON_BN_CLICKED(IDC_CHECK_1_2 , OnCheck12 )
	ON_BN_CLICKED(IDC_CHECK_1_3 , OnCheck13 )
	ON_BN_CLICKED(IDC_CHECK_1_4 , OnCheck14 )
	ON_BN_CLICKED(IDC_CHECK_1_5 , OnCheck15 )
	ON_BN_CLICKED(IDC_CHECK_1_6 , OnCheck16 )
	ON_BN_CLICKED(IDC_CHECK_1_7 , OnCheck17 )
	ON_BN_CLICKED(IDC_CHECK_1_8 , OnCheck18 )
	ON_BN_CLICKED(IDC_CHECK_1_9 , OnCheck19 )
	ON_BN_CLICKED(IDC_CHECK_1_10, OnCheck110)
	ON_BN_CLICKED(IDC_CHECK_1_11, OnCheck111)
	ON_BN_CLICKED(IDC_CHECK_1_12, OnCheck112)
	ON_BN_CLICKED(IDC_CHECK_1_13, OnCheck113)
	ON_BN_CLICKED(IDC_CHECK_1_14, OnCheck114)
	ON_BN_CLICKED(IDC_CHECK_1_15, OnCheck115)
	ON_BN_CLICKED(IDC_CHECK_2_0 , OnCheck20 )
	ON_BN_CLICKED(IDC_CHECK_2_1 , OnCheck21 )
	ON_BN_CLICKED(IDC_CHECK_2_2 , OnCheck22 )
	ON_BN_CLICKED(IDC_CHECK_2_3 , OnCheck23 )
	ON_BN_CLICKED(IDC_CHECK_2_4 , OnCheck24 )
	ON_BN_CLICKED(IDC_CHECK_2_5 , OnCheck25 )
	ON_BN_CLICKED(IDC_CHECK_2_6 , OnCheck26 )
	ON_BN_CLICKED(IDC_CHECK_2_7 , OnCheck27 )
	ON_BN_CLICKED(IDC_CHECK_2_8 , OnCheck28 )
	ON_BN_CLICKED(IDC_CHECK_2_9 , OnCheck29 )
	ON_BN_CLICKED(IDC_CHECK_2_10, OnCheck210)
	ON_BN_CLICKED(IDC_CHECK_2_11, OnCheck211)
	ON_BN_CLICKED(IDC_CHECK_2_12, OnCheck212)
	ON_BN_CLICKED(IDC_CHECK_2_13, OnCheck213)
	ON_BN_CLICKED(IDC_CHECK_2_14, OnCheck214)
	ON_BN_CLICKED(IDC_CHECK_2_15, OnCheck215)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CdlgMelSecMonitor message handlers

void CdlgMelSecMonitor::OnTimer(UINT nIDEvent) 
{
#define BASE_ADDR	0x0C0

	int m_InMyAddr= 0x0C0, m_OutMyAddr= 0x0E0, m_JogMyAddr= m_InMyAddr + 0x10;
	int m_InLoaderAddr= 0x00, m_OutUnloaderAddr= 0x20;
	int addrmul= 0, i;

	switch(m_StationNo){
	case 0: //amt1 100,120
		addrmul= 1;
		break;
	case 1: //mmt a-1 : 1c0, 1e0
		addrmul= 4;
		break;
	case 2:  // mmt a-2 : 200. 220
		addrmul= 5;
		break;
	case 3: //amt b : c0, e0
		addrmul= 0;
		break;
	case 4:  //mmt b-1 : 140, 160
		addrmul= 2;
		break; 
	case 5:  // mmt b-2 : 180, 1a0
		addrmul= 3;
		break;
		
	}
	m_InMyAddr= BASE_ADDR + (0x40 * addrmul);
	m_OutMyAddr= (BASE_ADDR + 0x20) + (0x40 * addrmul);
	m_JogMyAddr= m_InMyAddr + 0x10;
	if(m_StationNo== 0 || m_StationNo== 3) {
		m_InLoaderAddr= 0 + (0x40 * (addrmul));
		m_OutUnloaderAddr= (0 + 0x20) + (0x40 * (addrmul));
	}
	else {
		m_InLoaderAddr= 0x240 + (0x40 * (addrmul-2));
		m_OutUnloaderAddr= (0x240 + 0x20) + (0x40 * (addrmul-2));
	}

	CString strText;
	strText.Format("B-%04X", m_InMyAddr);	SetDlgItemText(IDC_STATIC_0, strText);
	strText.Format("B-%04X", m_JogMyAddr);	SetDlgItemText(IDC_STATIC_1, strText);
	strText.Format("B-%04X", m_OutMyAddr);	SetDlgItemText(IDC_STATIC_2, strText);
	strText.Format("B-%04X", m_InLoaderAddr);	SetDlgItemText(IDC_STATIC_3, strText);
	strText.Format("B-%04X", m_OutUnloaderAddr);	SetDlgItemText(IDC_STATIC_4, strText);
	
//	m_InLoaderAddr= 0x00, m_OutUnloaderAddr= 0x20;

	for(i= 0; i< 16; i++) {
		m_Check0[i].SetCheck(GetMelsec()->MelsecNetBitRecv(m_InMyAddr + i));
		m_Check1[i].SetCheck(GetMelsec()->MelsecNetBitRecv(m_JogMyAddr + i));
		m_Check2[i].SetCheck(GetMelsec()->MelsecNetBitRecv(m_OutMyAddr + i));
		m_Check3[i].SetCheck(GetMelsec()->MelsecNetBitRecv(m_InLoaderAddr + i));
		m_Check4[i].SetCheck(GetMelsec()->MelsecNetBitRecv(m_OutUnloaderAddr + i));
	}
	m_Check5[4].SetCheck(GetMelsec()->MelsecNetBitRecv(m_InLoaderAddr+0x10+4));
	m_Check5[5].SetCheck(GetMelsec()->MelsecNetBitRecv(m_InLoaderAddr+0x10+5));

	m_Check6[4].SetCheck(GetMelsec()->MelsecNetBitRecv(m_OutUnloaderAddr+0x10+4));
	m_Check6[5].SetCheck(GetMelsec()->MelsecNetBitRecv(m_OutUnloaderAddr+0x10+5));

//	m_Check0[15].SetCheck(GetMelsec()->MelsecNetBitRecv(GetMelsec()->m_sInLoaderAddr+0x10, 15));
	CDialog::OnTimer(nIDEvent);
}

void CdlgMelSecMonitor::OnDestroy() 
{
	CDialog::OnDestroy();	
	KillTimer(9000);
}

BOOL CdlgMelSecMonitor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
		SetDlgItemText(IDC_CHECK_0_0 , "Ready");
		SetDlgItemText(IDC_CHECK_0_1 , "Pause");
		SetDlgItemText(IDC_CHECK_0_2 , "Down");
		SetDlgItemText(IDC_CHECK_0_3 , "Alram");
		SetDlgItemText(IDC_CHECK_0_4 , "Unload Requese");
		SetDlgItemText(IDC_CHECK_0_5 , "Unloading");
		SetDlgItemText(IDC_CHECK_0_6 , "Unload Complete");
		SetDlgItemText(IDC_CHECK_0_7 , "VI-End");
		SetDlgItemText(IDC_CHECK_0_8 , "PM-MODE");
		SetDlgItemText(IDC_CHECK_0_9 , " ");
		SetDlgItemText(IDC_CHECK_0_10, "Memo-Mode");
		SetDlgItemText(IDC_CHECK_0_11, "Clamp-Off");
		SetDlgItemText(IDC_CHECK_0_12, " ");
		SetDlgItemText(IDC_CHECK_0_13, " ");
		SetDlgItemText(IDC_CHECK_0_14, " ");
		SetDlgItemText(IDC_CHECK_0_15, " ");

		SetDlgItemText(IDC_CHECK_1_0 , "Down-Time Start");
		SetDlgItemText(IDC_CHECK_1_1 , "Down-Time Reset");
		SetDlgItemText(IDC_CHECK_1_2 , " ");
		SetDlgItemText(IDC_CHECK_1_3 , " ");
		SetDlgItemText(IDC_CHECK_1_4 , " ");
		SetDlgItemText(IDC_CHECK_1_5 , " ");
		SetDlgItemText(IDC_CHECK_1_6 , " ");
		SetDlgItemText(IDC_CHECK_1_7 , "Load Comm. Clear");
		SetDlgItemText(IDC_CHECK_1_8 , "Unload Comm. Clear");
		SetDlgItemText(IDC_CHECK_1_9 , "OP Call");
		SetDlgItemText(IDC_CHECK_1_10, "Alram-Mode");
		SetDlgItemText(IDC_CHECK_1_11, "Watch-Dog (PC)");
		SetDlgItemText(IDC_CHECK_1_12, "Panel Check");
		SetDlgItemText(IDC_CHECK_1_13, "Door/Area Sensor");
		SetDlgItemText(IDC_CHECK_1_14, "Insp./Tilt");
		SetDlgItemText(IDC_CHECK_1_15, "PC Start");
	
		SetDlgItemText(IDC_CHECK_2_0 , "Ready");
		SetDlgItemText(IDC_CHECK_2_1 , "Pause");
		SetDlgItemText(IDC_CHECK_2_2 , "Down");
		SetDlgItemText(IDC_CHECK_2_3 , "Alram");
		SetDlgItemText(IDC_CHECK_2_4 , "Load Requese");
		SetDlgItemText(IDC_CHECK_2_5 , "Loading");
		SetDlgItemText(IDC_CHECK_2_6 , "Load Complete");
		SetDlgItemText(IDC_CHECK_2_7 , "VI-End");
		SetDlgItemText(IDC_CHECK_2_8 , "PM-MODE");
		SetDlgItemText(IDC_CHECK_2_9 , " ");
		SetDlgItemText(IDC_CHECK_2_10, "Memo-Mode");
		SetDlgItemText(IDC_CHECK_2_11, "Clamp-ON");
		SetDlgItemText(IDC_CHECK_2_12, " ");
		SetDlgItemText(IDC_CHECK_2_13, " ");
		SetDlgItemText(IDC_CHECK_2_14, " ");
		SetDlgItemText(IDC_CHECK_2_15, " ");

		SetDlgItemText(IDC_CHECK_3_0 , "Ready");
		SetDlgItemText(IDC_CHECK_3_1 , "Pause");
		SetDlgItemText(IDC_CHECK_3_2 , "Down");
		SetDlgItemText(IDC_CHECK_3_3 , "Alram");
		SetDlgItemText(IDC_CHECK_3_4 , "Unload Request");
		SetDlgItemText(IDC_CHECK_3_5 , "Unloading");
		SetDlgItemText(IDC_CHECK_3_6 , "Unload Complete");
		SetDlgItemText(IDC_CHECK_3_7 , "VI-End");
		SetDlgItemText(IDC_CHECK_3_8 , "PM-MODE");
		SetDlgItemText(IDC_CHECK_3_9 , " ");
		SetDlgItemText(IDC_CHECK_3_10, "Memo-Mode");
		SetDlgItemText(IDC_CHECK_3_11, "Unloader-Vacuum");
		SetDlgItemText(IDC_CHECK_3_12, "UnLoader-Up");
		SetDlgItemText(IDC_CHECK_3_13, " ");
		SetDlgItemText(IDC_CHECK_3_14, " ");
		SetDlgItemText(IDC_CHECK_3_15, " ");
		SetDlgItemText(IDC_CHECK_4_0 , "Ready");
		SetDlgItemText(IDC_CHECK_4_1 , "Pause");
		SetDlgItemText(IDC_CHECK_4_2 , "Down");
		SetDlgItemText(IDC_CHECK_4_3 , "Alram");
		SetDlgItemText(IDC_CHECK_4_4 , "Load Request");
		SetDlgItemText(IDC_CHECK_4_5 , "Loading");
		SetDlgItemText(IDC_CHECK_4_6 , "Load Complete");
		SetDlgItemText(IDC_CHECK_4_7 , "VI-End");
		SetDlgItemText(IDC_CHECK_4_8 , "PM-MODE");
		SetDlgItemText(IDC_CHECK_4_9 , " ");
		SetDlgItemText(IDC_CHECK_4_10, "Memo-Mode");
		SetDlgItemText(IDC_CHECK_4_11, "loader-Vacuum");
		SetDlgItemText(IDC_CHECK_4_12, "Loader-Up");
		SetDlgItemText(IDC_CHECK_4_13, " ");
		SetDlgItemText(IDC_CHECK_4_14, " ");
		SetDlgItemText(IDC_CHECK_4_15, " ");

		SetDlgItemText(IDC_CHECK_3_16, "4:Arm Straching");
		SetDlgItemText(IDC_CHECK_3_17, "5:eMMTLoading1");
		SetDlgItemText(IDC_CHECK_4_16, "4:Arm Straching");
		SetDlgItemText(IDC_CHECK_4_17, "5:eMMTLoading1");

		if(GetMelsec()->IsConnected())
			SetTimer(9000, 300, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CdlgMelSecMonitor::PreTranslateMessage(MSG* pMsg) 
{
    if(pMsg->message == WM_KEYDOWN){  if((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE)){          return TRUE;        }    }
	return CDialog::PreTranslateMessage(pMsg);
}

void CdlgMelSecMonitor::DispUpdate()
{
	UpdateData(TRUE);
/*	switch(m_StationNo) {
	case 0:
		SetDlgItemText(IDC_CHECK_0_0 , "Ready");
		SetDlgItemText(IDC_CHECK_0_1 , "Pause");
		SetDlgItemText(IDC_CHECK_0_2 , "Down");
		SetDlgItemText(IDC_CHECK_0_3 , "Alram");
		SetDlgItemText(IDC_CHECK_0_4 , "공급 요구");
		SetDlgItemText(IDC_CHECK_0_5 , "공급 중");
		SetDlgItemText(IDC_CHECK_0_6 , "공급 완료");
		SetDlgItemText(IDC_CHECK_0_7 , "VI-End");
		SetDlgItemText(IDC_CHECK_0_8 , "PM-MODE");
		SetDlgItemText(IDC_CHECK_0_9 , " ");
		SetDlgItemText(IDC_CHECK_0_10, "Memo-Mode");
		SetDlgItemText(IDC_CHECK_0_11, "Unloader-Vacuum");
		SetDlgItemText(IDC_CHECK_0_12, "Loader-Up");
		SetDlgItemText(IDC_CHECK_0_13, " ");
		SetDlgItemText(IDC_CHECK_0_14, " ");
		SetDlgItemText(IDC_CHECK_0_15, " ");

		SetDlgItemText(IDC_CHECK_1_c, "Down-Time Start");
		SetDlgItemText(IDC_CHECK_1_, "Down-Time Reset");
		SetDlgItemText(IDC_CHECK_1_, " ");
		SetDlgItemText(IDC_CHECK_1_, " ");
		SetDlgItemText(IDC_CHECK_1_, " ");
		SetDlgItemText(IDC_CHECK_1_, " ");
		SetDlgItemText(IDC_CHECK_1_, " ");
		SetDlgItemText(IDC_CHECK_1_, " ");
		SetDlgItemText(IDC_CHECK_1_, " ");
		SetDlgItemText(IDC_CHECK_1_, "OP Call");
		SetDlgItemText(IDC_CHECK_1_, "Alram-Mode");
		SetDlgItemText(IDC_CHECK_1_, "Watch-Dog (PC)");
		SetDlgItemText(IDC_CHECK_1_, "Panel Check");
		SetDlgItemText(IDC_CHECK_1_, "Door/Area Sensor");
		SetDlgItemText(IDC_CHECK_1_, "검사중/Tilt중");
		SetDlgItemText(IDC_CHECK_1_, "PC Start");
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	}*/
}

void CdlgMelSecMonitor::OnRadio1() 
{
	DispUpdate();
}

void CdlgMelSecMonitor::OnRadio2() 
{
	DispUpdate();
}

void CdlgMelSecMonitor::OnRadio3() 
{
	DispUpdate();
	
}

void CdlgMelSecMonitor::OnRadio4() 
{
	DispUpdate();
}

void CdlgMelSecMonitor::OnRadio5() 
{
	DispUpdate();
}

void CdlgMelSecMonitor::OnRadio6() 
{
	DispUpdate();
}
void CdlgMelSecMonitor::SetBitRtn(int s_ad, int e_ad)
{
#define BASE_ADDR	0x0C0

	int m_InMyAddr= 0x0C0, m_OutMyAddr= 0x0E0, m_JogMyAddr= m_InMyAddr + 0x10;
	int m_InLoaderAddr= 0x00, m_OutUnloaderAddr= 0x20;
	int addrmul= 0;

	switch(m_StationNo){
	case 0: //amt1 100,120
		addrmul= 1;
		break;
	case 1: //mmt a-1 : 1c0, 1e0
		addrmul= 4;
		break;
	case 2:  // mmt a-2 : 200. 220
		addrmul= 5;
		break;
	case 3: //amt b : c0, e0
		addrmul= 0;
		break;
	case 4:  //mmt b-1 : 140, 160
		addrmul= 2;
		break; 
	case 5:  // mmt b-2 : 180, 1a0
		addrmul= 3;
		break;
		
	}
	m_InMyAddr= BASE_ADDR + (0x40 * addrmul);
	m_OutMyAddr= (BASE_ADDR + 0x20) + (0x40 * addrmul);
	m_JogMyAddr= m_InMyAddr + 0x10;

	m_InLoaderAddr= 0x00, m_OutUnloaderAddr= 0x20;

	switch(s_ad)
	{
	case 0:
		if(GetMelsec()->MelsecNetBitRecv(m_InMyAddr+e_ad))
			m_Check0[e_ad].SetCheck(GetMelsec()->MelsecNetBitOff(m_InMyAddr+e_ad));
		else
			m_Check0[e_ad].SetCheck(GetMelsec()->MelsecNetBitOn(m_InMyAddr+e_ad));
		break;
	case 1:
		if(GetMelsec()->MelsecNetBitRecv(m_JogMyAddr+e_ad))
			m_Check1[e_ad].SetCheck(GetMelsec()->MelsecNetBitOff(m_JogMyAddr+e_ad));
		else
			m_Check1[e_ad].SetCheck(GetMelsec()->MelsecNetBitOn(m_JogMyAddr+e_ad));
		break;
	case 2:
		if(GetMelsec()->MelsecNetBitRecv(m_OutMyAddr+e_ad))
			m_Check2[e_ad].SetCheck(GetMelsec()->MelsecNetBitOff(m_OutMyAddr+e_ad));
		else
			m_Check2[e_ad].SetCheck(GetMelsec()->MelsecNetBitOn(m_OutMyAddr+e_ad));
		break;
	}
}

void CdlgMelSecMonitor::OnCheck00 () {	SetBitRtn(0,0 );	}
void CdlgMelSecMonitor::OnCheck01 () {	SetBitRtn(0,1 );	}
void CdlgMelSecMonitor::OnCheck02 () {	SetBitRtn(0,2 );	}
void CdlgMelSecMonitor::OnCheck03 () {	SetBitRtn(0,3 );	}
void CdlgMelSecMonitor::OnCheck04 () {	SetBitRtn(0,4 );	}
void CdlgMelSecMonitor::OnCheck05 () {	SetBitRtn(0,5 );	}
void CdlgMelSecMonitor::OnCheck06 () {	SetBitRtn(0,6 );	}
void CdlgMelSecMonitor::OnCheck07 () {	SetBitRtn(0,7 );	}
void CdlgMelSecMonitor::OnCheck08 () {	SetBitRtn(0,8 );	}
void CdlgMelSecMonitor::OnCheck09 () {	SetBitRtn(0,9 );	}
void CdlgMelSecMonitor::OnCheck010() {	SetBitRtn(0,10);	}
void CdlgMelSecMonitor::OnCheck011() {	SetBitRtn(0,11);	}
void CdlgMelSecMonitor::OnCheck012() {	SetBitRtn(0,12);	}
void CdlgMelSecMonitor::OnCheck013() {	SetBitRtn(0,13);	}
void CdlgMelSecMonitor::OnCheck014() {	SetBitRtn(0,14);	}
void CdlgMelSecMonitor::OnCheck015() {	SetBitRtn(0,15);	}
void CdlgMelSecMonitor::OnCheck10 () {	SetBitRtn(1,0 );	}
void CdlgMelSecMonitor::OnCheck11 () {	SetBitRtn(1,1 );	}
void CdlgMelSecMonitor::OnCheck12 () {	SetBitRtn(1,2 );	}
void CdlgMelSecMonitor::OnCheck13 () {	SetBitRtn(1,3 );	}
void CdlgMelSecMonitor::OnCheck14 () {	SetBitRtn(1,4 );	}
void CdlgMelSecMonitor::OnCheck15 () {	SetBitRtn(1,5 );	}
void CdlgMelSecMonitor::OnCheck16 () {	SetBitRtn(1,6 );	}
void CdlgMelSecMonitor::OnCheck17 () {	SetBitRtn(1,7 );	}
void CdlgMelSecMonitor::OnCheck18 () {	SetBitRtn(1,8 );	}
void CdlgMelSecMonitor::OnCheck19 () {	SetBitRtn(1,9 );	}
void CdlgMelSecMonitor::OnCheck110() {	SetBitRtn(1,10);	}
void CdlgMelSecMonitor::OnCheck111() {	SetBitRtn(1,11);	}
void CdlgMelSecMonitor::OnCheck112() {	SetBitRtn(1,12);	}
void CdlgMelSecMonitor::OnCheck113() {	SetBitRtn(1,13);	}
void CdlgMelSecMonitor::OnCheck114() {	SetBitRtn(1,14);	}
void CdlgMelSecMonitor::OnCheck115() {	SetBitRtn(1,15);	}
void CdlgMelSecMonitor::OnCheck20 () {	SetBitRtn(2,0 );	}
void CdlgMelSecMonitor::OnCheck21 () {	SetBitRtn(2,1 );	}
void CdlgMelSecMonitor::OnCheck22 () {	SetBitRtn(2,2 );	}
void CdlgMelSecMonitor::OnCheck23 () {	SetBitRtn(2,3 );	}
void CdlgMelSecMonitor::OnCheck24 () {	SetBitRtn(2,4 );	}
void CdlgMelSecMonitor::OnCheck25 () {	SetBitRtn(2,5 );	}
void CdlgMelSecMonitor::OnCheck26 () {	SetBitRtn(2,6 );	}
void CdlgMelSecMonitor::OnCheck27 () {	SetBitRtn(2,7 );	}
void CdlgMelSecMonitor::OnCheck28 () {	SetBitRtn(2,8 );	}
void CdlgMelSecMonitor::OnCheck29 () {	SetBitRtn(2,9 );	}
void CdlgMelSecMonitor::OnCheck210() {	SetBitRtn(2,10);	}
void CdlgMelSecMonitor::OnCheck211() {	SetBitRtn(2,11);	}
void CdlgMelSecMonitor::OnCheck212() {	SetBitRtn(2,12);	}
void CdlgMelSecMonitor::OnCheck213() {	SetBitRtn(2,13);	}
void CdlgMelSecMonitor::OnCheck214() {	SetBitRtn(2,14);	}
void CdlgMelSecMonitor::OnCheck215() {	SetBitRtn(2,15);	}
