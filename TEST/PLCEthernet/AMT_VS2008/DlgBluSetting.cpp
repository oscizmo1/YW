// DlgBluSetting.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "DlgBluSetting.h"
#include "DlgNumPad.h"
#include "MainFrm.h"

using namespace Gdiplus;	// Color class
#define ActiveBoder			CLbl_blu::ilbsLowered
#define NoActiveBoder		CLbl_blu::ilbsRaised
#define ActiveBevel			CPrev_port::ibsLowered
#define NoActiveBevel		CPrev_port::ibsRaised

// CDlgBluSetting dialog

IMPLEMENT_DYNAMIC(CDlgBluSetting, CDialog)

HWND hCommWnd;

CDlgBluSetting::CDlgBluSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBluSetting::IDD, pParent)
{
	m_nSelectedID = id_BLU;
	m_nSelectedParam = param_Port;
	m_nSelectedModel = BluModel_WT;

	for(int i = id_BLU; i < id_Max; i++)
	{
		for(int j = param_Port; j < param_Max; j++)
			m_nCurParamIndex[i][j] = 0;
	}
	ColorSetting();
}

CDlgBluSetting::~CDlgBluSetting()
{
}

void CDlgBluSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LBL_BLU_PORT,			m_lbl232Param[id_BLU][param_Port]);
	DDX_Control(pDX, IDC_LBL_BLU_BAUDRATE,		m_lbl232Param[id_BLU][param_Baudrate]);
	DDX_Control(pDX, IDC_LBL_BLU_DATABIT,		m_lbl232Param[id_BLU][param_DataBit]);
	DDX_Control(pDX, IDC_LBL_BLU_STOPBIT,		m_lbl232Param[id_BLU][param_StopBit]);
	DDX_Control(pDX, IDC_LBL_BLU_PARITYBIT,		m_lbl232Param[id_BLU][param_ParityBit]);
	DDX_Control(pDX, IDC_LBL_BLU_BGROUND,		m_lblBluBackGround);
	DDX_Control(pDX, IDC_LBL_BTM_PORT,			m_lbl232Param[id_BtmLight][param_Port]);
	DDX_Control(pDX, IDC_LBL_BTM_BAUDRATE,		m_lbl232Param[id_BtmLight][param_Baudrate]);
	DDX_Control(pDX, IDC_LBL_BTM_DATABIT,		m_lbl232Param[id_BtmLight][param_DataBit]);
	DDX_Control(pDX, IDC_LBL_BTM_STOPBIT,		m_lbl232Param[id_BtmLight][param_StopBit]);
	DDX_Control(pDX, IDC_LBL_BTM_PARITYBIT,		m_lbl232Param[id_BtmLight][param_ParityBit]);
	DDX_Control(pDX, IDC_LBL_BTM_BGROUND,		m_lblBtmBackGround);
	DDX_Control(pDX, IDC_LBL_OMIT_PORT,			m_lbl232Param[id_OMIT][param_Port]);
	DDX_Control(pDX, IDC_LBL_OMIT_BAUDRATE,		m_lbl232Param[id_OMIT][param_Baudrate]);
	DDX_Control(pDX, IDC_LBL_OMIT_DATABIT,		m_lbl232Param[id_OMIT][param_DataBit]);
	DDX_Control(pDX, IDC_LBL_OMIT_STOPBIT,		m_lbl232Param[id_OMIT][param_StopBit]);
	DDX_Control(pDX, IDC_LBL_OMIT_PARITYBIT,	m_lbl232Param[id_OMIT][param_ParityBit]);
	DDX_Control(pDX, IDC_LBL_OMIT_BGROUND,		m_lblOmitBackGround);
	DDX_Control(pDX, IDC_LBL_PORT,				m_lblParamTitle[param_Port]);
	DDX_Control(pDX, IDC_LBL_BAUDRATE,			m_lblParamTitle[param_Baudrate]);
	DDX_Control(pDX, IDC_LBL_DATABIT,			m_lblParamTitle[param_DataBit]);
	DDX_Control(pDX, IDC_LBL_STOPBIT,			m_lblParamTitle[param_StopBit]);
	DDX_Control(pDX, IDC_LBL_PARITYBIT,			m_lblParamTitle[param_ParityBit]);
	DDX_Control(pDX, IDC_ARROW_PREV,			m_PrevArrow);
	DDX_Control(pDX, IDC_ARROW_NEXT,			m_NextArrow);
	DDX_Control(pDX, IDC_EDIT_HISTORY_STATUS,	m_historyStatus);
	DDX_Control(pDX, IDC_SWH_WT, m_switch[BluModel_WT]);
	DDX_Control(pDX, IDC_SWH_WS, m_switch[BluModel_WS]);
	DDX_Control(pDX, IDC_SWH_HD, m_switch[BluModel_HD]);
	DDX_Control(pDX, IDC_SWH_ETC, m_switch[BluModel_ETC]);
	DDX_Control(pDX, IDC_LBL_BLU_1	, m_lblBluBright[0]);
	DDX_Control(pDX, IDC_LBL_BLU_2	, m_lblBluBright[1]);
	DDX_Control(pDX, IDC_LBL_BLU_3	, m_lblBluBright[2]);
	DDX_Control(pDX, IDC_LBL_BLU_4	, m_lblBluBright[3]);
	DDX_Control(pDX, IDC_LBL_BLU_5	, m_lblBluBright[4]);
	DDX_Control(pDX, IDC_LBL_BLU_6	, m_lblBluBright[5]);
	DDX_Control(pDX, IDC_LBL_BLU_7	, m_lblBluBright[6]);
	DDX_Control(pDX, IDC_LBL_BLU_8	, m_lblBluBright[7]);
	DDX_Control(pDX, IDC_LBL_BLU_9	, m_lblBluBright[8]);
	DDX_Control(pDX, IDC_LBL_BLU_10	, m_lblBluBright[9]);
	DDX_Control(pDX, IDC_LBL_BLU_11	, m_lblBluBright[10]);
	DDX_Control(pDX, IDC_LBL_BLU_12	, m_lblBluBright[11]);
	DDX_Control(pDX, IDC_LBL_BLU_13	, m_lblBluBright[12]);
	DDX_Control(pDX, IDC_LBL_BLU_14	, m_lblBluBright[13]);
	DDX_Control(pDX, IDC_LBL_BLU_15	, m_lblBluBright[14]);
	DDX_Control(pDX, IDC_LBL_BLU_16	, m_lblBluBright[15]);
	DDX_Control(pDX, IDC_LBL_BLU_17	, m_lblBluBright[16]);
	DDX_Control(pDX, IDC_LBL_BLU_18	, m_lblBluBright[17]);
	DDX_Control(pDX, IDC_LBL_BLU_19	, m_lblBluBright[18]);
	DDX_Control(pDX, IDC_LBL_BLU_20	, m_lblBluBright[19]);
	DDX_Control(pDX, IDC_LBL_BLU_21	, m_lblBluBright[20]);
	DDX_Control(pDX, IDC_LBL_BLU_22	, m_lblBluBright[21]);
	DDX_Control(pDX, IDC_LBL_BLU_23	, m_lblBluBright[22]);
	DDX_Control(pDX, IDC_LBL_BLU_24	, m_lblBluBright[23]);
	DDX_Control(pDX, IDC_LBL_BLU_25	, m_lblBluBright[24]);
	DDX_Control(pDX, IDC_LBL_BLU_26	, m_lblBluBright[25]);
	DDX_Control(pDX, IDC_LBL_BLU_27	, m_lblBluBright[26]);
	DDX_Control(pDX, IDC_LBL_BLU_28	, m_lblBluBright[27]);
	DDX_Control(pDX, IDC_LBL_BLU_29	, m_lblBluBright[28]);
	DDX_Control(pDX, IDC_LBL_BLU_30	, m_lblBluBright[29]);
	DDX_Control(pDX, IDC_LBL_BLU_31	, m_lblBluBright[30]);
	DDX_Control(pDX, IDC_LBL_BLU_32	, m_lblBluBright[31]);
	DDX_Control(pDX, IDC_LBL_OMIT_SETTING, m_lblOmitBright);
	DDX_Control(pDX, IDC_LBL_BTM_CH1_SETTING, m_lblBtmBright[BtmLight_Ch1]);
	DDX_Control(pDX, IDC_LBL_BTM_CH2_SETTING, m_lblBtmBright[BtmLight_Ch2]);
	DDX_Control(pDX, IDC_LBL_BTM_CH3_SETTING, m_lblBtmBright[BtmLight_Ch3]);
	DDX_Control(pDX, IDC_LBL_BTM_CH4_SETTING, m_lblBtmBright[BtmLight_Ch4]);
}


BEGIN_MESSAGE_MAP(CDlgBluSetting, CDialog)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CDlgBluSetting::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CDlgBluSetting::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_CLEAR_HISTORY, &CDlgBluSetting::OnBnClickedBtnClearHistory)
	ON_BN_CLICKED(IDC_BTN_BTM_ON, &CDlgBluSetting::OnBnClickedBtnBtmOn)
	ON_BN_CLICKED(IDC_BTN_BTM_OFF, &CDlgBluSetting::OnBnClickedBtnBtmOff)
	ON_BN_CLICKED(IDC_BTN_OPEN_PORT, &CDlgBluSetting::OnBnClickedBtnOpenPort)
	ON_BN_CLICKED(IDC_BTN_CLOSE_PORT, &CDlgBluSetting::OnBnClickedBtnClosePort)
	ON_MESSAGE(WM_COMM_READ , &CDlgBluSetting::OnCommRead)	
	ON_BN_CLICKED(IDC_BTN_BLU_ON, &CDlgBluSetting::OnBnClickedBtnBluOn)
	ON_BN_CLICKED(IDC_BTN_BLU_OFF, &CDlgBluSetting::OnBnClickedBtnBluOff)
	ON_BN_CLICKED(IDC_BTN_OMIT_ON, &CDlgBluSetting::OnBnClickedBtnOmitOn)
	ON_BN_CLICKED(IDC_BTN_OMIT_OFF, &CDlgBluSetting::OnBnClickedBtnOmitOff)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDlgBluSetting message handlers

BOOL CDlgBluSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	for (int i = id_BLU; i < id_Max; i++)
	{
		if(GetDoc()->m_SerialData[i].Used)
		{
			if(GetDoc()->SerialPortOpen(i))
			{
				GetDoc()->m_Comm[i].m_RcvHwnd = this->m_hWnd;
				// Contact 하부 조명은 포트 열림과 동시에 항상 On 해 놓는다.
				if(i == id_BtmLight)	GetDoc()->BtmOn();
			}
			else
				AfxMessageBox("Serial open fail!");
		}
	}

	m_historyStatus.ClearBuffer();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgBluSetting::Active232ID( int nSelectedID, int nParam )
{
	if(nSelectedID == id_BLU)		// BLU Setting 활성화
	{
		m_lblBluBackGround.put_BackGroundColor(ActiveBkColor);
		m_lblBluBackGround.put_BorderStyle(ActiveBoder);
		m_lblBtmBackGround.put_BackGroundColor(NoActiveBkColor);
		m_lblBtmBackGround.put_BorderStyle(NoActiveBoder);
		m_lblOmitBackGround.put_BackGroundColor(NoActiveBkColor);
		m_lblOmitBackGround.put_BorderStyle(NoActiveBoder);
	}
	if(nSelectedID == id_BtmLight)		// BTM Setting 활성화
	{
		m_lblBluBackGround.put_BackGroundColor(NoActiveBkColor);
		m_lblBluBackGround.put_BorderStyle(NoActiveBoder);
		m_lblBtmBackGround.put_BackGroundColor(ActiveBkColor);
		m_lblBtmBackGround.put_BorderStyle(ActiveBoder);
		m_lblOmitBackGround.put_BackGroundColor(NoActiveBkColor);
		m_lblOmitBackGround.put_BorderStyle(NoActiveBoder);
	}
	else if(nSelectedID == id_OMIT)	// OMIT Setting 활성화
	{
		m_lblBluBackGround.put_BackGroundColor(NoActiveBkColor);
		m_lblBluBackGround.put_BorderStyle(NoActiveBoder);
		m_lblBtmBackGround.put_BackGroundColor(NoActiveBkColor);
		m_lblBtmBackGround.put_BorderStyle(NoActiveBoder);
		m_lblOmitBackGround.put_BackGroundColor(ActiveBkColor);
		m_lblOmitBackGround.put_BorderStyle(ActiveBoder);
	}
	Active232Param(nParam);
}

void CDlgBluSetting::Active232Param( int nParam )
{
	for(int j = id_BLU; j < id_Max; j++)
	{
		for(int i = param_Port; i < param_Max; i++)
		{
			if(i == nParam)
			{
				// 선택한거 활성화
				m_lbl232Param[j][nParam].put_BackGroundColor(ActiveParamColor);
			}
			else
			{
				// 전에 활성화된거 초기화
				m_lbl232Param[j][i].put_BackGroundColor(NoActiveParamColor);
			}
		}
	}
	for(int i = param_Port; i < param_Max; i++)
	{
		if(i == nParam)
		{
			// 선택한거 활성화
			m_lblParamTitle[i].put_BorderStyle(ActiveBoder);
			m_lblParamTitle[i].put_BackGroundColor(ActiveParamColor);
		}
		else
		{
			// 전에 활성화된거 초기화
			m_lblParamTitle[i].put_BorderStyle(NoActiveBoder);
			m_lblParamTitle[i].put_BackGroundColor(NoActiveParamColor);
		}
	}
}
BEGIN_EVENTSINK_MAP(CDlgBluSetting, CDialog)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU, 1, CDlgBluSetting::OnClickLblBlu, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_PORT, 1, CDlgBluSetting::OnClickLblBluPort, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_BAUDRATE, 1, CDlgBluSetting::OnClickLblBluBaudrate, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_DATABIT, 1, CDlgBluSetting::OnClickLblBluDatabit, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_STOPBIT, 1, CDlgBluSetting::OnClickLblBluStopbit, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_PARITYBIT, 1, CDlgBluSetting::OnClickLblBluParitybit, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_OMIT, 1, CDlgBluSetting::OnClickLblOmit, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_OMIT_PORT, 1, CDlgBluSetting::OnClickLblOmitPort, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_OMIT_BAUDRATE, 1, CDlgBluSetting::OnClickLblOmitBaudrate, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_OMIT_DATABIT, 1, CDlgBluSetting::OnClickLblOmitDatabit, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_OMIT_STOPBIT, 1, CDlgBluSetting::OnClickLblOmitStopbit, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_OMIT_PARITYBIT, 1, CDlgBluSetting::OnClickLblOmitParitybit, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_PORT, 1, CDlgBluSetting::OnClickLblPort, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BAUDRATE, 1, CDlgBluSetting::OnClickLblBaudrate, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_DATABIT, 1, CDlgBluSetting::OnClickLblDatabit, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_STOPBIT, 1, CDlgBluSetting::OnClickLblStopbit, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_PARITYBIT, 1, CDlgBluSetting::OnClickLblParitybit, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_ARROW_PREV, 2, CDlgBluSetting::OnClickArrowPrev, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_ARROW_NEXT, 2, CDlgBluSetting::OnClickArrowNext, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_ARROW_PREV, 6, CDlgBluSetting::OnMouseDownArrowPrev, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDlgBluSetting, IDC_ARROW_NEXT, 6, CDlgBluSetting::OnMouseDownArrowNext, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDlgBluSetting, IDC_ARROW_PREV, 8, CDlgBluSetting::OnMouseUpArrowPrev, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDlgBluSetting, IDC_ARROW_NEXT, 8, CDlgBluSetting::OnMouseUpArrowNext, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CDlgBluSetting, IDC_SWH_WT, 2, CDlgBluSetting::OnChangeUserSwhWt, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_SWH_WS, 2, CDlgBluSetting::OnChangeUserSwhWs, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_SWH_HD, 2, CDlgBluSetting::OnChangeUserSwhHd, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_SWH_ETC, 2, CDlgBluSetting::OnChangeUserSwhEtc, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_1, 1, 	CDlgBluSetting::OnClickLblBlu1	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_2, 1, 	CDlgBluSetting::OnClickLblBlu2	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_3, 1, 	CDlgBluSetting::OnClickLblBlu3	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_4, 1, 	CDlgBluSetting::OnClickLblBlu4	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_5, 1, 	CDlgBluSetting::OnClickLblBlu5	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_6, 1, 	CDlgBluSetting::OnClickLblBlu6	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_7, 1, 	CDlgBluSetting::OnClickLblBlu7	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_8, 1, 	CDlgBluSetting::OnClickLblBlu8	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_9, 1, 	CDlgBluSetting::OnClickLblBlu9	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_10, 1, CDlgBluSetting::OnClickLblBlu10	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_11, 1, CDlgBluSetting::OnClickLblBlu11	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_12, 1, CDlgBluSetting::OnClickLblBlu12	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_13, 1, CDlgBluSetting::OnClickLblBlu13	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_14, 1, CDlgBluSetting::OnClickLblBlu14	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_15, 1, CDlgBluSetting::OnClickLblBlu15	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_16, 1, CDlgBluSetting::OnClickLblBlu16	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_17, 1, CDlgBluSetting::OnClickLblBlu17	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_18, 1, CDlgBluSetting::OnClickLblBlu18	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_19, 1, CDlgBluSetting::OnClickLblBlu19	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_20, 1, CDlgBluSetting::OnClickLblBlu20	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_21, 1, CDlgBluSetting::OnClickLblBlu21	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_22, 1, CDlgBluSetting::OnClickLblBlu22	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_23, 1, CDlgBluSetting::OnClickLblBlu23	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_24, 1, CDlgBluSetting::OnClickLblBlu24	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_25, 1, CDlgBluSetting::OnClickLblBlu25	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_26, 1, CDlgBluSetting::OnClickLblBlu26	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_27, 1, CDlgBluSetting::OnClickLblBlu27	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_28, 1, CDlgBluSetting::OnClickLblBlu28	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_29, 1, CDlgBluSetting::OnClickLblBlu29	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_30, 1, CDlgBluSetting::OnClickLblBlu30	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_31, 1, CDlgBluSetting::OnClickLblBlu31	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BLU_32, 1, CDlgBluSetting::OnClickLblBlu32	, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_OMIT_SETTING, 1, CDlgBluSetting::OnClickLblOmitSetting, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BTM, 1, CDlgBluSetting::OnClickLblBtm, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BTM_PORT, 1, CDlgBluSetting::OnClickLblBtmPort, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BTM_BAUDRATE, 1, CDlgBluSetting::OnClickLblBtmBaudrate, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BTM_DATABIT, 1, CDlgBluSetting::OnClickLblBtmDatabit, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BTM_STOPBIT, 1, CDlgBluSetting::OnClickLblBtmStopbit, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BTM_PARITYBIT, 1, CDlgBluSetting::OnClickLblBtmParitybit, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BTM_CH1_SETTING, 1, CDlgBluSetting::OnClickLblBtmCh1Setting, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BTM_CH2_SETTING, 1, CDlgBluSetting::OnClickLblBtmCh2Setting, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BTM_CH3_SETTING, 1, CDlgBluSetting::OnClickLblBtmCh3Setting, VTS_NONE)
ON_EVENT(CDlgBluSetting, IDC_LBL_BTM_CH4_SETTING, 1, CDlgBluSetting::OnClickLblBtmCh4Setting, VTS_NONE)
END_EVENTSINK_MAP()

void CDlgBluSetting::OnClickLblBlu()
{
	m_nSelectedID = id_BLU;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::OnClickLblBluPort()
{
	m_nSelectedID = id_BLU;
	m_nSelectedParam = param_Port;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::OnClickLblBluBaudrate()
{
	m_nSelectedID = id_BLU;
	m_nSelectedParam = param_Baudrate;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::OnClickLblBluDatabit()
{
	m_nSelectedID = id_BLU;
	m_nSelectedParam = param_DataBit;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::OnClickLblBluStopbit()
{
	m_nSelectedID = id_BLU;
	m_nSelectedParam = param_StopBit;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::OnClickLblBluParitybit()
{
	m_nSelectedID = id_BLU;
	m_nSelectedParam = param_ParityBit;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::OnClickLblOmit()
{
	m_nSelectedID = id_OMIT;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::OnClickLblOmitPort()
{
	m_nSelectedID = id_OMIT;
	m_nSelectedParam = param_Port;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::OnClickLblOmitBaudrate()
{
	m_nSelectedID = id_OMIT;
	m_nSelectedParam = param_Baudrate;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::OnClickLblOmitDatabit()
{
	m_nSelectedID = id_OMIT;
	m_nSelectedParam = param_DataBit;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::OnClickLblOmitStopbit()
{
	m_nSelectedID = id_OMIT;
	m_nSelectedParam = param_StopBit;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::OnClickLblOmitParitybit()
{
	m_nSelectedID = id_OMIT;
	m_nSelectedParam = param_ParityBit;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::ColorSetting()
{
	ActiveBkColor =	Color(Color::Turquoise).ToCOLORREF();
	NoActiveBkColor = Color(Color::DarkGray).ToCOLORREF();
	ActiveParamColor = Color(Color::MediumAquamarine).ToCOLORREF();
	NoActiveParamColor = Color(Color::LightYellow).ToCOLORREF();
	ActiveBrightnessColor = Color(Color::Lime).ToCOLORREF();
	NoActiveBrightnessColor = Color(Color::WhiteSmoke).ToCOLORREF();
}
void CDlgBluSetting::OnClickLblPort()
{
	m_nSelectedParam = param_Port;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::OnClickLblBaudrate()
{
	m_nSelectedParam = param_Baudrate;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::OnClickLblDatabit()
{
	m_nSelectedParam = param_DataBit;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::OnClickLblStopbit()
{
	m_nSelectedParam = param_StopBit;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::OnClickLblParitybit()
{
	m_nSelectedParam = param_ParityBit;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::OnClickArrowPrev()
{
	ShiftParam(m_nSelectedID, m_nSelectedParam, FALSE);
}

void CDlgBluSetting::OnClickArrowNext()
{
	ShiftParam(m_nSelectedID, m_nSelectedParam, TRUE);
}

void CDlgBluSetting::ShiftParam( int nSelectedID, int nParam, BOOL bNext )
{
	if(bNext)
	{
		m_nCurParamIndex[m_nSelectedID][nParam]++;
	}
	else
	{
		m_nCurParamIndex[m_nSelectedID][nParam]--;
	}
	ViewAllParam(nSelectedID);
}

void CDlgBluSetting::ViewAllParam(int nSelectedID)
{
	for(int j = param_Port; j < param_Max; j++)
	{
		switch(j)
		{
		case param_Port:
			if(m_nCurParamIndex[nSelectedID][j] >= Index_Port_Max)
				m_nCurParamIndex[nSelectedID][j] = Index_Port_1;
			else if(m_nCurParamIndex[nSelectedID][j] < Index_Port_1)
				m_nCurParamIndex[nSelectedID][j] = Index_Port_4;
			m_lbl232Param[nSelectedID][j].put_Caption(GetDoc()->m_szPortNo[m_nCurParamIndex[nSelectedID][j]]);
			break;
		case param_Baudrate:
			if(m_nCurParamIndex[nSelectedID][j] >= Index_Baud_Max)
				m_nCurParamIndex[nSelectedID][j] = Index_Baud_110;
			else if(m_nCurParamIndex[nSelectedID][j] < Index_Baud_110)
				m_nCurParamIndex[nSelectedID][j] = Index_Baud_115200;
			m_lbl232Param[nSelectedID][j].put_Caption(GetDoc()->m_szBaudRate[m_nCurParamIndex[nSelectedID][j]]);
			break;
		case param_DataBit:
			if(m_nCurParamIndex[nSelectedID][j] >= Index_DataBit_Max)
				m_nCurParamIndex[nSelectedID][j] = Index_DataBit_5;
			else if(m_nCurParamIndex[nSelectedID][j] < Index_DataBit_5)
				m_nCurParamIndex[nSelectedID][j] = Index_DataBit_8;
			m_lbl232Param[nSelectedID][j].put_Caption(GetDoc()->m_szDataBit[m_nCurParamIndex[nSelectedID][j]]);
			break;
		case param_StopBit:
			if(m_nCurParamIndex[nSelectedID][j] >= Index_StopBit_Max)
				m_nCurParamIndex[nSelectedID][j] = Index_StopBit_10;
			else if(m_nCurParamIndex[nSelectedID][j] < Index_StopBit_10)
				m_nCurParamIndex[nSelectedID][j] = Index_StopBit_20;
			m_lbl232Param[nSelectedID][j].put_Caption(GetDoc()->m_szStopBit[m_nCurParamIndex[nSelectedID][j]]);
			break;
		case param_ParityBit:
			if(m_nCurParamIndex[nSelectedID][j] >= Index_ParityBit_Max)
				m_nCurParamIndex[nSelectedID][j] = Index_ParityBit_Even;
			else if(m_nCurParamIndex[nSelectedID][j] < Index_ParityBit_Even)
				m_nCurParamIndex[nSelectedID][j] = Index_ParityBit_Space;
			m_lbl232Param[nSelectedID][j].put_Caption(GetDoc()->m_szParityBit[m_nCurParamIndex[nSelectedID][j]]);
		default:
			break;
		}
	}
}

void CDlgBluSetting::OnMouseDownArrowPrev(short Button, short Shift, long X, long Y)
{
	m_PrevArrow.put_BevelStyle(ActiveBevel);
	m_PrevArrow.put_Active(TRUE);
}

void CDlgBluSetting::OnMouseDownArrowNext(short Button, short Shift, long X, long Y)
{
	m_NextArrow.put_BevelStyle(ActiveBevel);
	m_NextArrow.put_Active(TRUE);
}

void CDlgBluSetting::OnMouseUpArrowPrev(short Button, short Shift, long X, long Y)
{
	m_PrevArrow.put_BevelStyle(NoActiveBevel);
	m_PrevArrow.put_Active(FALSE);
}

void CDlgBluSetting::OnMouseUpArrowNext(short Button, short Shift, long X, long Y)
{
	m_NextArrow.put_BevelStyle(NoActiveBevel);
	m_NextArrow.put_Active(FALSE);
}

void CDlgBluSetting::OnBnClickedBtnApply()
{
	ApplyToDoc();
	GetDoc()->SaveSerialData();
	GetDoc()->SaveBrightness();
}

void CDlgBluSetting::OnBnClickedBtnClose()
{
	// TODO: Add your control notification handler code here
	ShowWindow(SW_HIDE);
}

void CDlgBluSetting::SelectModel( int nSelectedModel )
{
	BOOL bActive;
	for(int i = BluModel_WT; i < BluModel_Max; i++)
	{
		bActive = (i == nSelectedModel) ? TRUE:FALSE;
		m_switch[i].put_Active(bActive);
	}
	UpdateBluBrightness(nSelectedModel, FALSE);
}

void CDlgBluSetting::OnBnClickedBtnClearHistory()
{
	m_historyStatus.ClearBuffer();
}

void CDlgBluSetting::OnChangeUserSwhWt()
{
	m_nSelectedModel = BluModel_WT;
	SelectModel(m_nSelectedModel);
}

void CDlgBluSetting::OnChangeUserSwhWs()
{
	m_nSelectedModel = BluModel_WS;
	SelectModel(m_nSelectedModel);
}

void CDlgBluSetting::OnChangeUserSwhHd()
{
	m_nSelectedModel = BluModel_HD;
	SelectModel(m_nSelectedModel);
}

void CDlgBluSetting::OnChangeUserSwhEtc()
{
	m_nSelectedModel = BluModel_ETC;
	SelectModel(m_nSelectedModel);
}

void CDlgBluSetting::OnClickLblBlu1()
{
	EditBluBrightness(0);
}

void CDlgBluSetting::OnClickLblBlu2()
{
	EditBluBrightness(1);
}

void CDlgBluSetting::OnClickLblBlu3()
{
	EditBluBrightness(2);
}

void CDlgBluSetting::OnClickLblBlu4()
{
	EditBluBrightness(3);
}

void CDlgBluSetting::OnClickLblBlu5()
{
	EditBluBrightness(4);
}

void CDlgBluSetting::OnClickLblBlu6()
{
	EditBluBrightness(5);
}

void CDlgBluSetting::OnClickLblBlu7()
{
	EditBluBrightness(6);
}

void CDlgBluSetting::OnClickLblBlu8()
{
	EditBluBrightness(7);
}

void CDlgBluSetting::OnClickLblBlu9()
{
	EditBluBrightness(8);
}

void CDlgBluSetting::OnClickLblBlu10()
{
	EditBluBrightness(9);
}

void CDlgBluSetting::OnClickLblBlu11()
{
	EditBluBrightness(10);
}

void CDlgBluSetting::OnClickLblBlu12()
{
	EditBluBrightness(11);
}

void CDlgBluSetting::OnClickLblBlu13()
{
	EditBluBrightness(12);
}

void CDlgBluSetting::OnClickLblBlu14()
{
	EditBluBrightness(13);
}

void CDlgBluSetting::OnClickLblBlu15()
{
	EditBluBrightness(14);
}

void CDlgBluSetting::OnClickLblBlu16()
{
	EditBluBrightness(15);
}

void CDlgBluSetting::OnClickLblBlu17()
{
	EditBluBrightness(16);
}

void CDlgBluSetting::OnClickLblBlu18()
{
	EditBluBrightness(17);
}

void CDlgBluSetting::OnClickLblBlu19()
{
	EditBluBrightness(18);
}

void CDlgBluSetting::OnClickLblBlu20()
{
	EditBluBrightness(19);
}

void CDlgBluSetting::OnClickLblBlu21()
{
	EditBluBrightness(20);
}

void CDlgBluSetting::OnClickLblBlu22()
{
	EditBluBrightness(21);
}

void CDlgBluSetting::OnClickLblBlu23()
{
	EditBluBrightness(22);
}

void CDlgBluSetting::OnClickLblBlu24()
{
	EditBluBrightness(23);
}

void CDlgBluSetting::OnClickLblBlu25()
{
	EditBluBrightness(24);
}

void CDlgBluSetting::OnClickLblBlu26()
{
	EditBluBrightness(25);
}

void CDlgBluSetting::OnClickLblBlu27()
{
	EditBluBrightness(26);
}

void CDlgBluSetting::OnClickLblBlu28()
{
	EditBluBrightness(27);
}

void CDlgBluSetting::OnClickLblBlu29()
{
	EditBluBrightness(28);
}

void CDlgBluSetting::OnClickLblBlu30()
{
	EditBluBrightness(29);
}

void CDlgBluSetting::OnClickLblBlu31()
{
	EditBluBrightness(30);
}

void CDlgBluSetting::OnClickLblBlu32()
{
	EditBluBrightness(31);
}

void CDlgBluSetting::EditBluBrightness( int nBrightIndex )
{
	m_lblBluBright[nBrightIndex].put_BackGroundColor(ActiveBrightnessColor);
	CDlgNumPad dlg;
	if(dlg.DoModal() == IDOK)
	{
		int nNum = (int)atoi(dlg.m_strEditText);
		if(nNum <= 255 && nNum >= 0)
		{
			CString strNum;
			strNum.Format("%d", nNum);
			m_lblBluBright[nBrightIndex].put_Caption(strNum);

			m_nBluBrightness[m_nSelectedModel][nBrightIndex] = nNum;

			SetBluBrightness(nBrightIndex);
		}
	}
	m_lblBluBright[nBrightIndex].put_BackGroundColor(NoActiveBrightnessColor);
}
void CDlgBluSetting::OnClickLblOmitSetting()
{
	m_lblOmitBright.put_BackGroundColor(ActiveBrightnessColor);
	CDlgNumPad dlg;
	if(dlg.DoModal() == IDOK)
	{
		int nNum = (int)atoi(dlg.m_strEditText);
		if(nNum <= 255 && nNum >= 0)
		{
			CString strNum;
			strNum.Format("%d", nNum);
			m_lblOmitBright.put_Caption(strNum);

			UpdateOmitBrightness(TRUE);
		}
	}
	m_lblOmitBright.put_BackGroundColor(NoActiveBrightnessColor);
	OmitOn();
}

void CDlgBluSetting::LoadFromDoc()
{
	for (int i = id_BLU; i < id_Max; i++)
	{
		m_nCurParamIndex[i][param_Port]			= GetDoc()->m_SerialData[i].IndexPortNo		;
		m_nCurParamIndex[i][param_Baudrate]		= GetDoc()->m_SerialData[i].IndexBaudRate	;
		m_nCurParamIndex[i][param_DataBit]		= GetDoc()->m_SerialData[i].IndexDataBit	;
		m_nCurParamIndex[i][param_StopBit]		= GetDoc()->m_SerialData[i].IndexStopBit	;
		m_nCurParamIndex[i][param_ParityBit]	= GetDoc()->m_SerialData[i].IndexParityBit	;
	}
	for(int j = BluModel_WT; j < BluModel_Max; j++)
	{
		for(int i = 0; i < 32; i++)
			m_nBluBrightness[j][i] = GetDoc()->m_nBluBrightness[j][i];
	}
	for(int i = BtmLight_Ch1; i < BtmLight_ChMax; i++)
		m_nBtmBrightness[i] = GetDoc()->m_nBtmLightBrightness[i];

	m_nOmitBrightness = GetDoc()->m_nOmitBrightness;
}

void CDlgBluSetting::ApplyToDoc()
{
	for (int i = id_BLU; i < id_Max; i++)
	{
		GetDoc()->m_SerialData[i].IndexPortNo		= m_nCurParamIndex[i][param_Port]		;
		GetDoc()->m_SerialData[i].IndexBaudRate		= m_nCurParamIndex[i][param_Baudrate]	;
		GetDoc()->m_SerialData[i].IndexDataBit		= m_nCurParamIndex[i][param_DataBit]	;
		GetDoc()->m_SerialData[i].IndexStopBit		= m_nCurParamIndex[i][param_StopBit]	;
		GetDoc()->m_SerialData[i].IndexParityBit	= m_nCurParamIndex[i][param_ParityBit]	;
	}

	for(int j = BluModel_WT; j < BluModel_Max; j++)
	{
		for(int i = 0; i < 32; i++)
			GetDoc()->m_nBluBrightness[j][i] = m_nBluBrightness[j][i];
	}
	for(int i = BtmLight_Ch1; i < BtmLight_ChMax; i++)
		GetDoc()->m_nBtmLightBrightness[i] = m_nBtmBrightness[i];

	GetDoc()->m_nOmitBrightness = m_nOmitBrightness;
}

void CDlgBluSetting::UpdateBluBrightness( int nModel, BOOL bSaveToParam )
{
	if(bSaveToParam)
	{
		for(int i = 0; i < 32; i++)
			m_nBluBrightness[nModel][i] = (int)atoi(m_lblBluBright[i].get_Caption());
	}
	else
	{
		CString strText;
		for(int i = 0; i < 32; i++)
		{
			strText.Format("%d", m_nBluBrightness[nModel][i]);
			m_lblBluBright[i].put_Caption(strText);
			SetBluBrightness(i, FALSE);
		}
		SaveBluBrightness();
	}
}

void CDlgBluSetting::UpdateBtmBrightness( BOOL bSaveToParam )
{
	if(bSaveToParam)
	{
		for(int i = BtmLight_Ch1; i < BtmLight_ChMax; i++)
		m_nBtmBrightness[i] = (int)atoi(m_lblBtmBright[i].get_Caption());
	}
	else
	{
		CString strText;
		for(int i = BtmLight_Ch1; i < BtmLight_ChMax; i++)
		{
			strText.Format("%d", m_nBtmBrightness[i]);
			m_lblBtmBright[i].put_Caption(strText);
		}
	}
}

void CDlgBluSetting::UpdateOmitBrightness( BOOL bSaveToParam )
{
	if(bSaveToParam)
	{
		m_nOmitBrightness = (int)atoi(m_lblOmitBright.get_Caption());
	}
	else
	{
		CString strText;
		strText.Format("%d", m_nOmitBrightness);
		m_lblOmitBright.put_Caption(strText);
	}
}
void CDlgBluSetting::OnClickLblBtm()
{
	m_nSelectedID = id_BtmLight;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::OnClickLblBtmPort()
{
	m_nSelectedID = id_BtmLight;
	m_nSelectedParam = param_Port;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::OnClickLblBtmBaudrate()
{
	m_nSelectedID = id_BtmLight;
	m_nSelectedParam = param_Baudrate;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::OnClickLblBtmDatabit()
{
	m_nSelectedID = id_BtmLight;
	m_nSelectedParam = param_DataBit;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::OnClickLblBtmStopbit()
{
	m_nSelectedID = id_BtmLight;
	m_nSelectedParam = param_StopBit;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::OnClickLblBtmParitybit()
{
	m_nSelectedID = id_BtmLight;
	m_nSelectedParam = param_ParityBit;
	Active232ID(m_nSelectedID, m_nSelectedParam);
}

void CDlgBluSetting::OnClickLblBtmCh1Setting()
{
	EditBtmBrightness(BtmLight_Ch1);
}

void CDlgBluSetting::OnClickLblBtmCh2Setting()
{
	EditBtmBrightness(BtmLight_Ch2);
}

void CDlgBluSetting::OnClickLblBtmCh3Setting()
{
	EditBtmBrightness(BtmLight_Ch3);
}

void CDlgBluSetting::OnClickLblBtmCh4Setting()
{
	EditBtmBrightness(BtmLight_Ch4);
}

void CDlgBluSetting::EditBtmBrightness( int nBrightIndex )
{
	m_lblBtmBright[nBrightIndex].put_BackGroundColor(ActiveBrightnessColor);
	CDlgNumPad dlg;
	if(dlg.DoModal() == IDOK)
	{
		int nNum = (int)atoi(dlg.m_strEditText);
		if(nNum <= 255 && nNum >= 0)
		{
			CString strNum;
			strNum.Format("%d", nNum);
			m_lblBtmBright[nBrightIndex].put_Caption(strNum);
			UpdateBtmBrightness(TRUE);
			BtmOn(nBrightIndex);
		}
	}
	m_lblBtmBright[nBrightIndex].put_BackGroundColor(NoActiveBrightnessColor);
}
void CDlgBluSetting::OnBnClickedBtnBtmOn()
{
	for (int i = BtmLight_Ch1; i < BtmLight_ChMax; i++)
		BtmOn(i);
}

void CDlgBluSetting::OnBnClickedBtnBtmOff()
{
	BtmOff();
}

void CDlgBluSetting::AddStatusLog( int nID, int nStatus, LPCTSTR szText )
{
	CString strOutText;
	switch(nID)
	{
	case id_BLU:		strOutText = _T("[BLU]\t");	break;
	case id_BtmLight:	strOutText = _T("[BTM]\t");	break;
	case id_OMIT:		strOutText = _T("[OMIT]\t");	break;
	}
	switch(nStatus)
	{
	case id_BLU:		strOutText += _T("\t");	break;
	case id_BtmLight:	strOutText += _T("[SEND]\t");	break;
	case id_OMIT:		strOutText += _T("[RECEIVE]\t");	break;
	}
	strOutText += szText;
	m_historyStatus.AppendString(strOutText);
}
void CDlgBluSetting::OnBnClickedBtnOpenPort()
{
	if(GetDoc()->m_SerialData[m_nSelectedID].Used)
	{
		if(GetDoc()->SerialPortOpen(m_nSelectedID))
			AddStatusLog(m_nSelectedID, Log_None, "Open success");
		else
			AddStatusLog(m_nSelectedID, Log_None, "Open fail");
	}
}

void CDlgBluSetting::OnBnClickedBtnClosePort()
{
	GetDoc()->SerialPortClose(m_nSelectedID);
	AddStatusLog(m_nSelectedID, Log_None, "Close");
}

LRESULT CDlgBluSetting::OnCommRead(WPARAM wParam, LPARAM lParam)
{
	// Serial Message parsing
	CCommThread* pComm = (CCommThread*)lParam;
	int nSize = pComm->m_QueueRead.GetSize();
	BYTE queData;
	char szReadBuffer[BUFF_SIZE];
	 int i;
	for( i = 0; i < nSize; i++ )
	{
		pComm->m_QueueRead.GetByte(&queData);//큐 버퍼에  들어온 데이터 넣기 
		szReadBuffer[i] = queData;
	}
	szReadBuffer[i] = NULL;
	pComm->m_QueueRead.Clear();
	// 로그 표시
	if(this->IsWindowVisible())
	{
		if(pComm->m_sPortName == GetDoc()->m_szPortNo[GetDoc()->m_SerialData[id_BLU].IndexPortNo])
			AddStatusLog(id_BLU, Log_Receive, szReadBuffer);
		else if(pComm->m_sPortName == GetDoc()->m_szPortNo[GetDoc()->m_SerialData[id_BtmLight].IndexPortNo])
			AddStatusLog(id_BtmLight, Log_Receive, szReadBuffer);
		else if(pComm->m_sPortName == GetDoc()->m_szPortNo[GetDoc()->m_SerialData[id_OMIT].IndexPortNo])
			AddStatusLog(id_OMIT, Log_Receive, szReadBuffer);
	}

	return 0L;
}

void CDlgBluSetting::OnBnClickedBtnBluOn()
{
	BluOn();
}

void CDlgBluSetting::OnBnClickedBtnBluOff()
{
	BluOff();
}

void CDlgBluSetting::OnBnClickedBtnOmitOn()
{
	OmitOn();
}

void CDlgBluSetting::OnBnClickedBtnOmitOff()
{
	OmitOff();
}

void CDlgBluSetting::OmitOn()
{
	CString strMsg;
	strMsg.Format("setbrightness 0 %d%c", m_nOmitBrightness, 0x0D);
	GetDoc()->nMakeAsciiData(id_OMIT, strMsg);
	AddStatusLog(id_OMIT, Log_Send, strMsg);
	GetDoc()->m_bLightOn[id_OMIT] = TRUE;
}

void CDlgBluSetting::OmitOff()
{
	CString strMsg;
	strMsg.Format("setbrightness 0 0%c", 0x0D);
	GetDoc()->nMakeAsciiData(id_OMIT, strMsg);
	AddStatusLog(id_OMIT, Log_Send, strMsg);
	GetDoc()->m_bLightOn[id_OMIT] = FALSE;
}

void CDlgBluSetting::SetBluBrightness( int nCh, BOOL bSave /*= TRUE*/ )
{
	CString strValue;

	strValue.Format("setbrightness %d %d%c", nCh , m_nBluBrightness[m_nSelectedModel][nCh], 0x0D);
	GetDoc()->nMakeAsciiData(id_BLU, strValue);
	AddStatusLog(id_BLU, Log_Send, strValue);
	Sleep(10);
	
	if(bSave)		SaveBluBrightness();
}

void CDlgBluSetting::SaveBluBrightness()
{
	CString strValue;
	strValue.Format("save%c", 0x0D);
	GetDoc()->nMakeAsciiData(id_BLU, strValue);
	AddStatusLog(id_BLU, Log_Send, strValue);
	Sleep(10);
}

void CDlgBluSetting::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		LoadFromDoc();
		Active232ID(m_nSelectedID, m_nSelectedParam);
		SelectModel(m_nSelectedModel);
		ViewAllParam(id_BLU);
		ViewAllParam(id_BtmLight);
		ViewAllParam(id_OMIT);
		//UpdateBluBrightness(m_nSelectedModel, FALSE);	SelectModel()에서 호출한다.
		UpdateBtmBrightness(FALSE);
		UpdateOmitBrightness(FALSE);
		m_historyStatus.ClearBuffer();

		CView* pView = GetMainFrame()->GetActiveView();
		CRect rect;
		pView->GetWindowRect(&rect);
		this->MoveWindow(&rect);
	}
	else
	{
		// 모델정보에 Blu모델 정보가 들어갈 경우 그에 맞게 Blu 밝기 save
		SelectModel(GetDoc()->m_pAmtData->m_CurModelInfo.m_nBluModel);
		BluOff();
// 		OmitOff();
		OmitOn();		
		// 하부 조명은 항상 On
		GetDoc()->BtmOn();
	}
}

void CDlgBluSetting::BtmOn( int nCh )
{
	CString strMsg;
	strMsg.Format("L%d%03d%c%c", nCh+1, m_nBtmBrightness[nCh], 0x0D/*CR*/, 0x0A/*LF*/);
	GetDoc()->nMakeAsciiData(id_BtmLight, strMsg);
	AddStatusLog(id_BtmLight, Log_Send, strMsg);
	Sleep(10);
	GetDoc()->m_bLightOn[id_BtmLight] = TRUE;
}

void CDlgBluSetting::BtmOff()
{
	CString strMsg;
	for (int i = BtmLight_Ch1; i < BtmLight_ChMax; i++)
	{
		strMsg.Format("E%d%c%c", i+1, 0x0D/*CR*/, 0x0A/*LF*/);
		GetDoc()->nMakeAsciiData(id_BtmLight, strMsg);
		AddStatusLog(id_BtmLight, Log_Send, strMsg);
		Sleep(10);
	}
	GetDoc()->m_bLightOn[id_BtmLight] = FALSE;
}

void CDlgBluSetting::BluOn()
{
	CString strMsg;
	strMsg.Format("setonex %x%c", 0xffffffff , 0x0D);
	GetDoc()->nMakeAsciiData(id_BLU, strMsg);
	AddStatusLog(id_BLU, Log_Send, strMsg);
	GetDoc()->m_bLightOn[id_BLU] = TRUE;
}

void CDlgBluSetting::BluOff()
{
	CString strMsg;
	strMsg.Format("setonex %x%c", 0x00000000 , 0x0D);
	GetDoc()->nMakeAsciiData(id_BLU, strMsg);
	AddStatusLog(id_BLU, Log_Send, strMsg);
	GetDoc()->m_bLightOn[id_BLU] = FALSE;
}