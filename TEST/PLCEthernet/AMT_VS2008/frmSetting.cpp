// frmSetting.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "frmSetting.h"
#include "DlgKeyBoard.h" 
#include "DlgNumPad.h"
#include "MMC39.h"
#include "TowerLampDlg.h"
#include "DlgBluSetting.h"
#include "MMCWFP39.h"
#include "MainFrm.h"


// CFormSetting

IMPLEMENT_DYNCREATE(CFormSetting, CFormView)

CFormSetting::CFormSetting()
	: CFormView(CFormSetting::IDD)
	, m_dPosition(10.0)
	, m_MotorSpeed(0)
	, m_nInspectionResetCount(0)
{

}

CFormSetting::~CFormSetting()
{
}

void CFormSetting::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETTING_MOTORGRID, m_SettingMotorGrid);
	DDX_Control(pDX, IDC_SETTING_SOCKETGRID, m_SocketGrid);
	DDX_Text(pDX, IDC_EDIT_CURRENT_POSITION, m_dPosition);
	DDX_Control(pDX, IDC_COMBO1, m_cmb_MotionSelect);
	DDX_Control(pDX, IDC_LABEL_ALRAM1, m_ctrlALM1);
	DDX_Control(pDX, IDC_LABEL_ALRAM2, m_ctrlALM2);
	DDX_Control(pDX, IDC_LABEL_ALRAM3, m_ctrlALM3);
	DDX_Control(pDX, IDC_LABEL_ALRAM4, m_ctrlALM4);
	DDX_Control(pDX, IDC_LABEL_ALRAM5, m_ctrlALM5);
	DDX_Control(pDX, IDC_LABEL_ALRAM6, m_ctrlALM6);
	DDX_Control(pDX, IDC_LABEL_ALRAM7, m_ctrlALM7);
	DDX_Control(pDX, IDC_LABEL_ALRAM8, m_ctrlALM8);
	DDX_Control(pDX, IDC_LABEL_ALRAM9, m_ctrlALM9);
	DDX_Control(pDX, IDC_LABEL_ALRAM10, m_ctrlALM10);
	DDX_Control(pDX, IDC_LABEL_ALRAM11, m_ctrlALM11);
	DDX_Control(pDX, IDC_LABEL_ALRAM12, m_ctrlALM12);
	DDX_Control(pDX, IDC_LABEL_NEGLIMIT1, m_ctrlNegLimit1);
	DDX_Control(pDX, IDC_LABEL_NEGLIMIT2, m_ctrlNegLimit2);
	DDX_Control(pDX, IDC_LABEL_NEGLIMIT3, m_ctrlNegLimit3);
	DDX_Control(pDX, IDC_LABEL_NEGLIMIT4, m_ctrlNegLimit4);
	DDX_Control(pDX, IDC_LABEL_NEGLIMIT5, m_ctrlNegLimit5);
	DDX_Control(pDX, IDC_LABEL_NEGLIMIT6, m_ctrlNegLimit6);
	DDX_Control(pDX, IDC_LABEL_NEGLIMIT7, m_ctrlNegLimit7);
	DDX_Control(pDX, IDC_LABEL_NEGLIMIT8, m_ctrlNegLimit8);
	DDX_Control(pDX, IDC_LABEL_NEGLIMIT9, m_ctrlNegLimit9);
	DDX_Control(pDX, IDC_LABEL_NEGLIMIT10, m_ctrlNegLimit10);
	DDX_Control(pDX, IDC_LABEL_NEGLIMIT11, m_ctrlNegLimit11);
	DDX_Control(pDX, IDC_LABEL_NEGLIMIT12, m_ctrlNegLimit12);
	DDX_Control(pDX, IDC_LABEL_HOME1, m_ctrlHome1);
	DDX_Control(pDX, IDC_LABEL_HOME2, m_ctrlHome2);
	DDX_Control(pDX, IDC_LABEL_HOME3, m_ctrlHome3);
	DDX_Control(pDX, IDC_LABEL_HOME4, m_ctrlHome4);
	DDX_Control(pDX, IDC_LABEL_HOME5, m_ctrlHome5);
	DDX_Control(pDX, IDC_LABEL_HOME6, m_ctrlHome6);
	DDX_Control(pDX, IDC_LABEL_HOME7, m_ctrlHome7);
	DDX_Control(pDX, IDC_LABEL_HOME8, m_ctrlHome8);
	DDX_Control(pDX, IDC_LABEL_HOME9, m_ctrlHome9);
	DDX_Control(pDX, IDC_LABEL_HOME10, m_ctrlHome10);
	DDX_Control(pDX, IDC_LABEL_HOME11, m_ctrlHome11);
	DDX_Control(pDX, IDC_LABEL_HOME12, m_ctrlHome12);
	DDX_Control(pDX, IDC_LABEL_POSLIMIT2, m_ctrlPosLimit2);
	DDX_Control(pDX, IDC_LABEL_POSLIMIT3, m_ctrlPosLimit3);
	DDX_Control(pDX, IDC_LABEL_POSLIMIT4, m_ctrlPosLimit4);
	DDX_Control(pDX, IDC_LABEL_POSLIMIT5, m_ctrlPosLimit5);
	DDX_Control(pDX, IDC_LABEL_POSLIMIT6, m_ctrlPosLimit6);
	DDX_Control(pDX, IDC_LABEL_POSLIMIT7, m_ctrlPosLimit7);
	DDX_Control(pDX, IDC_LABEL_POSLIMIT8, m_ctrlPosLimit8);
	DDX_Control(pDX, IDC_LABEL_POSLIMIT9, m_ctrlPosLimit9);
	DDX_Control(pDX, IDC_LABEL_POSLIMIT10, m_ctrlPosLimit10);
	DDX_Control(pDX, IDC_LABEL_POSLIMIT11, m_ctrlPosLimit11);
	DDX_Control(pDX, IDC_LABEL_POSLIMIT12, m_ctrlPosLimit12);
	DDX_Control(pDX, IDC_LABEL_POSLIMIT1, m_ctrlPosLimit1);
	DDX_Control(pDX, IDC_LABEL_AMPPOWER1, m_ctrlServo1);
	DDX_Control(pDX, IDC_LABEL_AMPPOWER2, m_ctrlServo2);
	DDX_Control(pDX, IDC_LABEL_AMPPOWER3, m_ctrlServo3);
	DDX_Control(pDX, IDC_LABEL_AMPPOWER4, m_ctrlServo4);
	DDX_Control(pDX, IDC_LABEL_AMPPOWER5, m_ctrlServo5);
	DDX_Control(pDX, IDC_LABEL_AMPPOWER6, m_ctrlServo6);
	DDX_Control(pDX, IDC_LABEL_AMPPOWER7, m_ctrlServo7);
	DDX_Control(pDX, IDC_LABEL_AMPPOWER8, m_ctrlServo8);
	DDX_Control(pDX, IDC_LABEL_AMPPOWER9, m_ctrlServo9);
	DDX_Control(pDX, IDC_LABEL_AMPPOWER10, m_ctrlServo10);
	DDX_Control(pDX, IDC_LABEL_AMPPOWER11, m_ctrlServo11);
	DDX_Control(pDX, IDC_LABEL_AMPPOWER12, m_ctrlServo12);
	DDX_Text(pDX, IDC_EDIT_SPEED, m_MotorSpeed);
	DDX_Text(pDX, IDC_EDIT_INSP_COUNT, m_nInspectionResetCount);
	DDX_Control(pDX, IDC_ISWITCHLEDX_INSP_COUNT, m_Chk_InspCount);
}

BEGIN_MESSAGE_MAP(CFormSetting, CFormView)
	ON_BN_CLICKED(IDC_BTN_MOTIONSAVE, &CFormSetting::OnBnClickedBtnMotionsave)
	ON_BN_CLICKED(IDC_BTN_SOCKETSAVE, &CFormSetting::OnBnClickedBtnSocketsave)
//	ON_BN_CLICKED(IDC_CHECK_POWER, &CFormSetting::OnBnClickedCheckPower)
//	ON_BN_CLICKED(IDC_CHECK_HOMECHECK, &CFormSetting::OnBnClickedCheckHomecheck)
	ON_BN_CLICKED(IDC_BTN_RESET, &CFormSetting::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_SPEEDSAVE, &CFormSetting::OnBnClickedBtnSpeedsave)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CFormSetting::OnCbnSelchangeCombo1)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_TOWERLAMP, &CFormSetting::OnBnClickedBtnTowerlamp)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_BLU_SETTING, &CFormSetting::OnBnClickedBtnBluSetting)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_POWERON, &CFormSetting::OnBnClickedBtnPoweron)
	ON_BN_CLICKED(IDC_SEARCH_SEARCH, &CFormSetting::OnBnClickedSearchSearch)
	ON_BN_CLICKED(IDC_SERVO_ONOFF, &CFormSetting::OnBnClickedServoOnoff)
	ON_BN_CLICKED(IDC_BTN_INSP_COUNT_SET, &CFormSetting::OnBnClickedBtnInspCountSet)
END_MESSAGE_MAP()


// CFormSetting diagnostics

#ifdef _DEBUG
void CFormSetting::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormSetting::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormSetting message handlers

BOOL CFormSetting::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CFormSetting::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	m_cmb_MotionSelect.SetCurSel(0);
	SetMotionGrid();
	SetSocketGrid();
	SetTimer(MOTION_SENSOR_CHECK, 100, NULL);

	if(m_dlgBlu.Create(IDD_DIALOG_BLUSETTING) == TRUE)
	{
		m_dlgBlu.ShowWindow(SW_HIDE);
		m_dlgBlu.BringWindowToTop();
	}
	if(GetDoc()->m_bInspCount == FALSE)
		m_Chk_InspCount.put_Active(FALSE);
	else
		m_Chk_InspCount.put_Active(TRUE);
}
BEGIN_EVENTSINK_MAP(CFormSetting, CFormView)
	ON_EVENT(CFormSetting, IDC_SETTING_MOTORGRID, DISPID_CLICK, CFormSetting::ClickSettingMotorgrid, VTS_NONE)
	ON_EVENT(CFormSetting, IDC_SETTING_SOCKETGRID, DISPID_CLICK, CFormSetting::ClickSettingSocketgrid, VTS_NONE)
	ON_EVENT(CFormSetting, IDC_ISWITCHLEDX_INSP_COUNT, 2, CFormSetting::OnChangeUserIswitchledxInspCount, VTS_NONE)
END_EVENTSINK_MAP()


void CFormSetting::SetMotionGrid(void)
{
	CString strName[MOTOR_PARAM+1] = {"Motor","Encoder","Voltage","FeedBack","StepMode","CordinateDirection","EncoderDirection","Positive Limit",
		"Negative Limit","Home Sensor", "Amp Fault", "Amp Reset Level","Amp Enable Level","InPosition Level", "Pos Soft Limit","Neg Soft Limit","ACC/DEC","Origin Method"," ","Origin Speed"
		,"Origin Direction","Origin Time Out","Electron Gear Ratio", "Electron Gear Pitch", "AbsEncoderType"};
	m_SettingMotorGrid.put_FontSize(10);
	m_SettingMotorGrid.put_Cols(3);
	m_SettingMotorGrid.put_Rows(MOTOR_PARAM+2);
	m_SettingMotorGrid.put_FixedRows(1);
	m_SettingMotorGrid.put_TextMatrix(0,0,"Axis Param");
	m_SettingMotorGrid.put_ColWidth(0, 2000);
	m_SettingMotorGrid.put_TextMatrix(0,1,"Parameter");
	m_SettingMotorGrid.put_ColWidth(1, 1500);
	m_SettingMotorGrid.put_TextMatrix(0,2,"Explanation");
	m_SettingMotorGrid.put_ColWidth(2, 4500);
	for (int i=0;i<MOTOR_PARAM+1; i++)
	{
		m_SettingMotorGrid.put_TextMatrix(i+1, 0, strName[i]);
		m_SettingMotorGrid.put_RowHeight(i+1, 450);
	}

	CString strExplanation[MOTOR_PARAM+1] = {"0:SERVO  1:STEPPER  2:MICRO","0:OPEN_LOOP   1:CLOSE_LOOP   2:SEMI_LOOP","0:BIPOLAR   1:UNIPOLAR","0:ENCODER   1:ANALOG   2:BI_ANALOG","0:CW+CCW MODE   1:SIGN+PULSE MODE","0:CW   1:CCW","0:CW   1:CCW","0:LOW   1:HIGH",
		"0:LOW   1:HIGH","0:LOW   1:HIGH", "0:LOW   1:HIGH", "0:LOW   1:HIGH","0:LOW   1:HIGH","0:NONE   1:LOW   2:HIGH", "0 ~ 2147483000","0 ~ 2147483000","0 ~ 25000","0:HomeSen  1:HomeSen+Z  2:NEGLIMIT "," 3:NEGLIMIT+Z  4:POSLIMIT  5:POSLIMIT+Z","1 ~ 50"
		,"0:CW   1:CCW","0 ~ 100000000000", "0 ~ 100000000000","0 ~ 100000000000", "11:Samsung, 22:Sigma3"};
	for (int i=0;i<MOTOR_PARAM+1; i++)
	{
		m_SettingMotorGrid.put_TextMatrix(i+1, 2, strExplanation[i]);
	}
	m_SettingMotorGrid.put_ColAlignment(0,4);
	m_SettingMotorGrid.put_ColAlignment(2,1);
	GetDlgItemText(IDC_COMBO1,strComboName);
	strDirtoryName.Format("%s - %s","Motion Parameter",strComboName);
	int nAxisNum = m_cmb_MotionSelect.GetCurSel();
	MotionGrid(nAxisNum);

	m_SettingMotorGrid.put_ColAlignment(1,4);
}

void CFormSetting::SetSocketGrid(void)
{
	CString strName[SOCKET_PARAM] = {"LC IsServer","L/C IP","L/C PORT","VI IsServer","VI IP","VI PORT","PG IsServer","PG IP","PG PORT"};
	m_SocketGrid.put_FontSize(10);
	m_SocketGrid.put_Cols(3);
	m_SocketGrid.put_Rows(SOCKET_PARAM+1);
	m_SocketGrid.put_FixedRows(1);
	m_SocketGrid.put_TextMatrix(0,0,"SOCKET Param");
	m_SocketGrid.put_ColWidth(0, 2000);
	m_SocketGrid.put_TextMatrix(0,1,"Parameter");
	m_SocketGrid.put_ColWidth(1, 1500);
	m_SocketGrid.put_TextMatrix(0,2,"Explanation");
	m_SocketGrid.put_ColWidth(2, 4500);
	for (int i=0;i<SOCKET_PARAM; i++)
	{
		m_SocketGrid.put_TextMatrix(i+1, 0, strName[i]);
		m_SocketGrid.put_RowHeight(i+1, 450);
	}

	CString strExplanation[SOCKET_PARAM] = {"1:SERVER   0:CLIENT","IP ADDRESS","PORT NUMBER","1:SERVER   0:CLIENT","IP ADDRESS","PORT NUMBER","1:SERVER   0:CLIENT","IP ADDRESS","PORT NUMBER"};
	for (int i=0;i<SOCKET_PARAM; i++)
	{
		m_SocketGrid.put_TextMatrix(i+1, 2, strExplanation[i]);
	}
	m_SocketGrid.put_ColAlignment(0,4);
	m_SocketGrid.put_ColAlignment(2,1);
	m_SocketGrid.put_ColAlignment(1,4);

	GetSocketParam();
}

void CFormSetting::OnBnClickedBtnMotionsave()
{
	// TODO: Add your control notification handler code here
	int i = m_cmb_MotionSelect.GetCurSel();
	GetDlgItemText(IDC_COMBO1,strComboName);
	SettingMotionParam();
	GetDoc()->SaveMotorParameter(strComboName, i, FALSE);
}

void CFormSetting::OnBnClickedBtnSocketsave()
{
	SetSocketParam();
	GetComm()->SaveSocketParam();
}

void CFormSetting::ClickSettingMotorgrid()
{
	// TODO: Add your message handler code here
	int nCol = m_SettingMotorGrid.get_MouseCol();
	int nRow = m_SettingMotorGrid.get_MouseRow();
	CDlgKeyBoard Dlg;
	CDlgNumPad	 dlg;

	if(nCol == 0 || nCol == 2)
		return ;

	if(GetDoc()->m_bSuperLogin == TRUE)
	{
		switch(nRow)
		{
		case 0:		
			break;
		case 1:
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(1, 1, str);
			}
			break;
		case 2:		
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(2, 1, str);
			}
			break;
		case 3:
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(3, 1, str);
			}
			break;
		case 4:		
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(4, 1, str);
			}
			break;
		case 5:
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(5, 1, str);
			}
			break;
		case 6:		
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(6, 1, str);
			}
			break;
		case 7:
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(7, 1, str);
			}
			break;
		case 8:		
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(8, 1, str);
			}
			break;
		case 9:
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(9, 1, str);
			}
			break;
		case 10:		
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(10, 1, str);
			}
			break;
		case 11:
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(11, 1, str);
			}
			break;
		case 12:		
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(12, 1, str);
			}
			break;
		case 13:
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(13, 1, str);
			}
			break;
		case 14:		
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(14, 1, str);
			}
			break;
		case 15:
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(15, 1, str);
			}
			break;
		case 16:		
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(16, 1, str);
			}
			break;
		case 17:
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(17, 1, str);
			}
			break;
		case 18:		
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(18, 1, str);
			}
			break;
		case 19:
			break;
		case 20:		
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(20, 1, str);
			}
			break;
		case 21:
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(21, 1, str);
			}
			break;
		case 22:		
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(22, 1, str);
			}
			break;
		case 23:		
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(23, 1, str);
			}
			break;
		case 24:		
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(24, 1, str);
			}
			break;
		case 25:		
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SettingMotorGrid.put_TextMatrix(25, 1, str);
			}
			break;
		}
	}
	else
		AfxMessageBox("Please contact your administrator.");

}

void CFormSetting::ClickSettingSocketgrid()
{
	// TODO: Add your message handler code here
	int nCol = m_SocketGrid.get_MouseCol();
	int nRow = m_SocketGrid.get_MouseRow();
	CDlgKeyBoard Dlg;
	CDlgNumPad	 dlg;

	if(nCol == 0 || nCol == 2)
		return ;
	
	if(GetDoc()->m_bSuperLogin == TRUE)
	{
		switch(nRow)
		{
		case 0:		
			break;
		case 1:
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SocketGrid.put_TextMatrix(1, 1, str);
			}
			break;
		case 2:		
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SocketGrid.put_TextMatrix(2, 1, str);
			}
			break;
		case 3:
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SocketGrid.put_TextMatrix(3, 1, str);
			}
			break;
		case 4:		
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SocketGrid.put_TextMatrix(4, 1, str);
			}
			break;
		case 5:
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SocketGrid.put_TextMatrix(5, 1, str);
			}
			break;
		case 6:		
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SocketGrid.put_TextMatrix(6, 1, str);
			}
			break;
		case 7:
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SocketGrid.put_TextMatrix(7, 1, str);
			}
			break;
		case 8:		
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SocketGrid.put_TextMatrix(8, 1, str);
			}
			break;
		case 9:
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SocketGrid.put_TextMatrix(9, 1, str);
			}
			break;
		case 10:		
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SocketGrid.put_TextMatrix(10, 1, str);
			}
			break;
		case 11:
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SocketGrid.put_TextMatrix(11, 1, str);
			}
			break;
		case 12:		
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SocketGrid.put_TextMatrix(12, 1, str);
			}
			break;
		case 13:
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SocketGrid.put_TextMatrix(13, 1, str);
			}
			break;
		case 14:		
			if(Dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",Dlg.m_strEditText);
				m_SocketGrid.put_TextMatrix(14, 1, str);
			}
			break;
		case 15:
			if(dlg.DoModal() == IDOK)
			{
				UpdateData(FALSE);
				CString str;
				str.Format("%s",dlg.m_strEditText);
				m_SocketGrid.put_TextMatrix(15, 1, str);
			}
			break;

		}
	}
	else
		AfxMessageBox("Please contact your administrator.");
}

void CFormSetting::OnBnClickedBtnReset()
{
	// TODO: Add your control notification handler code here
	int nAxis = m_cmb_MotionSelect.GetCurSel();

	if( GetLineType() == REMOVE_TILTING_TYPE && nAxis== AXIS_TILT_MOVE )
	{	// 2009.06.15
		if( nAxis == AXIS_TILT_MOVE )
		{
			GetMelsec()->MelsecNetBitOn( MEL_SW2_TILT_ALARM_RST );
			GetDeviceCtl()->m_dToggleTimer = GetTickCount();
			GetMainFrame()->SetTimer( TIMER_BIT_TOGGLE, 1000, NULL );
		}
	}
	else
		GetDoc()->m_pMotion->ResetAxis(nAxis);
}

void CFormSetting::OnBnClickedBtnSpeedsave()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int nAxis = m_cmb_MotionSelect.GetCurSel();
	CString strTemp;
	if((GetDoc()->m_bEngLogin == TRUE) || (GetDoc()->m_bSuperLogin == TRUE))
	{
		GetDlgItemText(IDC_COMBO1, strTemp);
		GetDoc()->m_pMotorParam[nAxis].dSpeed = m_MotorSpeed ;
		GetDoc()->SaveMotorParameter(strTemp, nAxis);
		if( GetLineType() == REMOVE_TILTING_TYPE && nAxis == AXIS_TILT_MOVE )
		{	// 2009.06.15
			if( m_MotorSpeed < 100. )
			{
				m_MotorSpeed = 100.;
			}
			if( m_MotorSpeed > 3500. )
			{
				m_MotorSpeed = 3500.;
			}
			GetMelsec()->MelsecNetWordSend(TILT_OUTPUT_RUN_SPEED, (short)m_MotorSpeed);
		}
	}
	else
		AfxMessageBox("Engineers more than is available.");
}

void CFormSetting::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
	int nAxis = m_cmb_MotionSelect.GetCurSel();
	CString strTemp;
	GetDlgItemText(IDC_COMBO1, strTemp);
	MotionGrid(nAxis);
}

void CFormSetting::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: Add your message handler code here
}

void CFormSetting::OnBnClickedBtnTowerlamp()
{
	// TODO: Add your control notification handler code here
	if((GetDoc()->m_bSuperLogin == TRUE) || (GetDoc()->m_bEngLogin == TRUE))
	{
		CTowerLampDlg dlg;
		dlg.DoModal();
	}
	else
		AfxMessageBox("More accessible to engineers.");
}

void CFormSetting::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	
	if(nIDEvent == MOTION_SENSOR_CHECK)
	{
		int nAxisNum;
		int nState;
		for(nAxisNum =0;nAxisNum<MAX_AXIS; nAxisNum++)
		{
			switch(nAxisNum)
			{
			case 0:
				nState = GetDoc()->m_pMotion->AxisStatus(AXIS_CCD_X0);
				if( nState & ST_AMP_FAULT ) {
					if( m_ctrlALM1.get_BackColor() != RGB(255,0,0) )
					m_ctrlALM1.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlALM1.get_BackColor() != RGB(0,255,0) )
					m_ctrlALM1.put_BackColor(RGB(0,255,0));
				if(nState & ST_AMP_POWER_ONOFF ) {
					if( m_ctrlServo1.get_BackColor() != RGB(255,0,0) )
					m_ctrlServo1.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlServo1.get_BackColor() != RGB(0,255,0) )
					m_ctrlServo1.put_BackColor(RGB(0,255,0));
				if(nState & ST_NEG_LIMIT ){
					if( m_ctrlNegLimit1.get_BackColor() != RGB(255,0,0) )
					m_ctrlNegLimit1.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlNegLimit1.get_BackColor() != RGB(0,255,0) )
					m_ctrlNegLimit1.put_BackColor(RGB(0,255,0));
				if(nState & ST_HOME_SWITCH ) {
					if( m_ctrlHome1.get_BackColor() != RGB(255,0,0) )
					m_ctrlHome1.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlHome1.get_BackColor() != RGB(0,255,0) )
					m_ctrlHome1.put_BackColor(RGB(0,255,0));
				if(nState & ST_POS_LIMIT ) {
					if( m_ctrlPosLimit1.get_BackColor() != RGB(255,0,0) )
					m_ctrlPosLimit1.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlPosLimit1.get_BackColor() != RGB(0,255,0) )
					m_ctrlPosLimit1.put_BackColor(RGB(0,255,0));
				break;

			case 1:
				nState = GetDoc()->m_pMotion->AxisStatus(AXIS_CCD_X1);
				if(nState & ST_AMP_FAULT ) {
					if( m_ctrlALM2.get_BackColor() != RGB(255,0,0) )
					m_ctrlALM2.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlALM2.get_BackColor() != RGB(0,255,0) )
					m_ctrlALM2.put_BackColor(RGB(0,255,0));
				if(nState & ST_AMP_POWER_ONOFF ) {
					if( m_ctrlServo2.get_BackColor() != RGB(255,0,0) )
					m_ctrlServo2.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlServo2.get_BackColor() != RGB(0,255,0) )
					m_ctrlServo2.put_BackColor(RGB(0,255,0));
				if(nState & ST_NEG_LIMIT ) {
					if( m_ctrlNegLimit2.get_BackColor() != RGB(255,0,0) )
					m_ctrlNegLimit2.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlNegLimit2.get_BackColor() != RGB(0,255,0) )
					m_ctrlNegLimit2.put_BackColor(RGB(0,255,0));
				if(nState & ST_HOME_SWITCH ) {
					if( m_ctrlHome2.get_BackColor() != RGB(255,0,0) )
					m_ctrlHome2.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlHome2.get_BackColor() != RGB(0,255,0) )
					m_ctrlHome2.put_BackColor(RGB(0,255,0));
				if(nState & ST_POS_LIMIT ) {
					if( m_ctrlPosLimit2.get_BackColor() != RGB(255,0,0) )
					m_ctrlPosLimit2.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlPosLimit2.get_BackColor() != RGB(0,255,0) )
					m_ctrlPosLimit2.put_BackColor(RGB(0,255,0));
				break;

			case 2:
				nState = GetDoc()->m_pMotion->AxisStatus(AXIS_CCD_X2);
				if(nState & ST_AMP_FAULT ) {
					if( m_ctrlALM3.get_BackColor() != RGB(255,0,0) )
					m_ctrlALM3.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlALM3.get_BackColor() != RGB(0,255,0) )
					m_ctrlALM3.put_BackColor(RGB(0,255,0));
				if(nState & ST_AMP_POWER_ONOFF ) {
					if( m_ctrlServo3.get_BackColor() != RGB(255,0,0) )
					m_ctrlServo3.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlServo3.get_BackColor() != RGB(0,255,0) )
					m_ctrlServo3.put_BackColor(RGB(0,255,0));
				if(nState & ST_NEG_LIMIT ) {
					if( m_ctrlNegLimit3.get_BackColor() != RGB(255,0,0) )
					m_ctrlNegLimit3.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlNegLimit3.get_BackColor() != RGB(0,255,0) )
					m_ctrlNegLimit3.put_BackColor(RGB(0,255,0));
				if(nState & ST_HOME_SWITCH ) {
					if( m_ctrlHome3.get_BackColor() != RGB(255,0,0) )
					m_ctrlHome3.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlHome3.get_BackColor() != RGB(0,255,0) )
					m_ctrlHome3.put_BackColor(RGB(0,255,0));
				if(nState & ST_POS_LIMIT ) {
					if( m_ctrlPosLimit3.get_BackColor() != RGB(255,0,0) )
					m_ctrlPosLimit3.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlPosLimit3.get_BackColor() != RGB(0,255,0) )
					m_ctrlPosLimit3.put_BackColor(RGB(0,255,0));
				break;

			case 3:
				nState = GetDoc()->m_pMotion->AxisStatus(AXIS_CCD_X3);
				if(nState & ST_AMP_FAULT ) {
					if( m_ctrlALM4.get_BackColor() != RGB(255,0,0) )
					m_ctrlALM4.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlALM4.get_BackColor() != RGB(0,255,0) )
					m_ctrlALM4.put_BackColor(RGB(0,255,0));
				if(nState & ST_AMP_POWER_ONOFF ) {
					if( m_ctrlServo4.get_BackColor() != RGB(255,0,0) )
					m_ctrlServo4.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlServo4.get_BackColor() != RGB(0,255,0) )
					m_ctrlServo4.put_BackColor(RGB(0,255,0));
				if(nState & ST_NEG_LIMIT ) {
					if( m_ctrlNegLimit4.get_BackColor() != RGB(255,0,0) )
					m_ctrlNegLimit4.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlNegLimit4.get_BackColor() != RGB(0,255,0) )
					m_ctrlNegLimit4.put_BackColor(RGB(0,255,0));
				if(nState & ST_HOME_SWITCH ) {
					if( m_ctrlHome4.get_BackColor() != RGB(255,0,0) )
					m_ctrlHome4.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlHome4.get_BackColor() != RGB(0,255,0) )
					m_ctrlHome4.put_BackColor(RGB(0,255,0));
				if(nState & ST_POS_LIMIT ) {
					if( m_ctrlPosLimit4.get_BackColor() != RGB(255,0,0) )
					m_ctrlPosLimit4.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlPosLimit4.get_BackColor() != RGB(0,255,0) )
					m_ctrlPosLimit4.put_BackColor(RGB(0,255,0));
				break;

			case 4:
				nState = GetDoc()->m_pMotion->AxisStatus(AXIS_CCD_Y0);
				if(nState & ST_AMP_FAULT ) {
					if( m_ctrlALM5.get_BackColor() != RGB(255,0,0) )
					m_ctrlALM5.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlALM5.get_BackColor() != RGB(0,255,0) )
					m_ctrlALM5.put_BackColor(RGB(0,255,0));
				if(nState & ST_AMP_POWER_ONOFF ) {
					if( m_ctrlServo5.get_BackColor() != RGB(255,0,0) )
					m_ctrlServo5.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlServo5.get_BackColor() != RGB(0,255,0) )
					m_ctrlServo5.put_BackColor(RGB(0,255,0));
				if(nState & ST_NEG_LIMIT ) {
					if( m_ctrlNegLimit5.get_BackColor() != RGB(255,0,0) )
					m_ctrlNegLimit5.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlNegLimit5.get_BackColor() != RGB(0,255,0) )
					m_ctrlNegLimit5.put_BackColor(RGB(0,255,0));
				if(nState & ST_HOME_SWITCH ) {
					if( m_ctrlHome5.get_BackColor() != RGB(255,0,0) )
					m_ctrlHome5.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlHome5.get_BackColor() != RGB(0,255,0) )
					m_ctrlHome5.put_BackColor(RGB(0,255,0));
				if(nState & ST_POS_LIMIT ) {
					if( m_ctrlPosLimit5.get_BackColor() != RGB(255,0,0) )
					m_ctrlPosLimit5.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlPosLimit5.get_BackColor() != RGB(0,255,0) )
					m_ctrlPosLimit5.put_BackColor(RGB(0,255,0));
				break;

			case 5:
				nState = GetDoc()->m_pMotion->AxisStatus(AXIS_CCD_Y1);
				if(nState & ST_AMP_FAULT ) {
					if( m_ctrlALM6.get_BackColor() != RGB(255,0,0) )
					m_ctrlALM6.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlALM6.get_BackColor() != RGB(0,255,0) )
					m_ctrlALM6.put_BackColor(RGB(0,255,0));
				if(nState & ST_AMP_POWER_ONOFF ) {
					if( m_ctrlServo6.get_BackColor() != RGB(255,0,0) )
					m_ctrlServo6.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlServo6.get_BackColor() != RGB(0,255,0) )
					m_ctrlServo6.put_BackColor(RGB(0,255,0));
				if(nState & ST_NEG_LIMIT ) {
					if( m_ctrlNegLimit6.get_BackColor() != RGB(255,0,0) )
					m_ctrlNegLimit6.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlNegLimit6.get_BackColor() != RGB(0,255,0) )
					m_ctrlNegLimit6.put_BackColor(RGB(0,255,0));
				if(nState & ST_HOME_SWITCH ) {
					if( m_ctrlHome6.get_BackColor() != RGB(255,0,0) )
					m_ctrlHome6.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlHome6.get_BackColor() != RGB(0,255,0) )
					m_ctrlHome6.put_BackColor(RGB(0,255,0));
				if(nState & ST_POS_LIMIT ) {
					if( m_ctrlPosLimit6.get_BackColor() != RGB(255,0,0) )
					m_ctrlPosLimit6.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlPosLimit6.get_BackColor() != RGB(0,255,0) )
					m_ctrlPosLimit6.put_BackColor(RGB(0,255,0));
				break;

			case 6:
				nState = GetDoc()->m_pMotion->AxisStatus(AXIS_CCD_Y2);
				if(nState & ST_AMP_FAULT ) {
					if( m_ctrlALM7.get_BackColor() != RGB(255,0,0) )
					m_ctrlALM7.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlALM7.get_BackColor() != RGB(0,255,0) )
					m_ctrlALM7.put_BackColor(RGB(0,255,0));
				if(nState & ST_AMP_POWER_ONOFF ) {
					if( m_ctrlServo7.get_BackColor() != RGB(255,0,0) )
					m_ctrlServo7.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlServo7.get_BackColor() != RGB(0,255,0) )
					m_ctrlServo7.put_BackColor(RGB(0,255,0));
				if(nState & ST_NEG_LIMIT ) {
					if( m_ctrlNegLimit7.get_BackColor() != RGB(255,0,0) )
					m_ctrlNegLimit7.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlNegLimit7.get_BackColor() != RGB(0,255,0) )
					m_ctrlNegLimit7.put_BackColor(RGB(0,255,0));
				if(nState & ST_HOME_SWITCH ) {
					if( m_ctrlHome7.get_BackColor() != RGB(255,0,0) )
					m_ctrlHome7.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlHome7.get_BackColor() != RGB(0,255,0) )
					m_ctrlHome7.put_BackColor(RGB(0,255,0));
				if(nState & ST_POS_LIMIT ) {
					if( m_ctrlPosLimit7.get_BackColor() != RGB(255,0,0) )
					m_ctrlPosLimit7.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlPosLimit7.get_BackColor() != RGB(0,255,0) )
					m_ctrlPosLimit7.put_BackColor(RGB(0,255,0));
				break;

			case 7:
				nState = GetDoc()->m_pMotion->AxisStatus(AXIS_CCD_Y3);
				if(nState & ST_AMP_FAULT ) {
					if( m_ctrlALM8.get_BackColor() != RGB(255,0,0) )
					m_ctrlALM8.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlALM8.get_BackColor() != RGB(0,255,0) )
					m_ctrlALM8.put_BackColor(RGB(0,255,0));
				if(nState & ST_AMP_POWER_ONOFF ) {
					if( m_ctrlServo8.get_BackColor() != RGB(255,0,0) )
					m_ctrlServo8.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlServo8.get_BackColor() != RGB(0,255,0) )
					m_ctrlServo8.put_BackColor(RGB(0,255,0));
				if(nState & ST_NEG_LIMIT ) {
					if( m_ctrlNegLimit8.get_BackColor() != RGB(255,0,0) )
					m_ctrlNegLimit8.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlNegLimit8.get_BackColor() != RGB(0,255,0) )
					m_ctrlNegLimit8.put_BackColor(RGB(0,255,0));
				if(nState & ST_HOME_SWITCH ) {
					if( m_ctrlHome8.get_BackColor() != RGB(255,0,0) )
					m_ctrlHome8.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlHome8.get_BackColor() != RGB(0,255,0) )
					m_ctrlHome8.put_BackColor(RGB(0,255,0));
				if(nState & ST_POS_LIMIT ){
					if( m_ctrlPosLimit8.get_BackColor() != RGB(255,0,0) )
					m_ctrlPosLimit8.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlPosLimit8.get_BackColor() != RGB(0,255,0) )
					m_ctrlPosLimit8.put_BackColor(RGB(0,255,0));
				break;

			case 8:
				nState = GetDoc()->m_pMotion->AxisStatus(AXIS_TILT_MOVE);
				if(nState & ST_AMP_FAULT ) {
					if( m_ctrlALM9.get_BackColor() != RGB(255,0,0) )
					m_ctrlALM9.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlALM9.get_BackColor() != RGB(0,255,0) )
					m_ctrlALM9.put_BackColor(RGB(0,255,0));
				if(nState & ST_AMP_POWER_ONOFF ) {
					if( m_ctrlServo9.get_BackColor() != RGB(255,0,0) )
					m_ctrlServo9.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlServo9.get_BackColor() != RGB(0,255,0) )
					m_ctrlServo9.put_BackColor(RGB(0,255,0));
				if(nState & ST_NEG_LIMIT ) {
					if( m_ctrlNegLimit9.get_BackColor() != RGB(255,0,0) )
					m_ctrlNegLimit9.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlNegLimit9.get_BackColor() != RGB(0,255,0) )
					m_ctrlNegLimit9.put_BackColor(RGB(0,255,0));
				if(nState & ST_HOME_SWITCH ) {
					if( m_ctrlHome9.get_BackColor() != RGB(255,0,0) )
					m_ctrlHome9.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlHome9.get_BackColor() != RGB(0,255,0) )
					m_ctrlHome9.put_BackColor(RGB(0,255,0));
				if(nState & ST_POS_LIMIT ){
					if( m_ctrlPosLimit9.get_BackColor() != RGB(255,0,0) )
					m_ctrlPosLimit9.put_BackColor(RGB(255,0,0));
				}
				else if( m_ctrlPosLimit9.get_BackColor() != RGB(0,255,0) )
					m_ctrlPosLimit9.put_BackColor(RGB(0,255,0));
				break;
			}
		}
	}
	CFormView::OnTimer(nIDEvent);
}

void CFormSetting::OnBnClickedBtnBluSetting()
{
	if(GetDoc()->m_bSuperLogin == TRUE || GetDoc()->m_bEngLogin == TRUE)
	{
		m_dlgBlu.ShowWindow(SW_SHOW);
	}
	else
		AfxMessageBox("More accessible to engineers.");
}

void CFormSetting::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFormView::OnShowWindow(bShow, nStatus);

	if(bShow)
	{

		if(GetApp()->IsRunStatus())	this->EnableWindow(FALSE);
		else						this->EnableWindow(TRUE);
	}
	else
	{
		m_dlgBlu.ShowWindow(SW_HIDE);
	}
}

void CFormSetting::SettingMotionParam(void)
{
	int nMotor = m_cmb_MotionSelect.GetCurSel();
	int nParam;
	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(1,1));
	GetDoc()->m_pMotorParam[nMotor].iMotor = nParam;

	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(2,1));
	GetDoc()->m_pMotorParam[nMotor].iEncoder = nParam;

	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(3,1));
	GetDoc()->m_pMotorParam[nMotor].iVoltage = nParam;

	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(4,1));
	GetDoc()->m_pMotorParam[nMotor].iFeedback = nParam;
	
	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(5,1));
	if(nParam == 0)
	{
		GetDoc()->m_pMotorParam[nMotor].bStepMode = FALSE;
	}
	else if(nParam == 1)
	{
		GetDoc()->m_pMotorParam[nMotor].bStepMode = TRUE;
	}

	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(6,1));
	if(nParam == 0)
	{
		GetDoc()->m_pMotorParam[nMotor].bCordinateDir = FALSE;
	}
	else if(nParam == 1)
	{
		GetDoc()->m_pMotorParam[nMotor].bCordinateDir = TRUE;
	}

	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(7,1));
	if(nParam == 0)
	{
		GetDoc()->m_pMotorParam[nMotor].bEncoderDir = FALSE;
	}
	else if(nParam == 1)
	{
		GetDoc()->m_pMotorParam[nMotor].bEncoderDir = TRUE;
	}

	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(8,1));
	if(nParam == 0)
	{
		GetDoc()->m_pMotorParam[nMotor].bPosLimit = FALSE;
	}
	else if(nParam == 1)
	{
		GetDoc()->m_pMotorParam[nMotor].bPosLimit = TRUE;
	}

	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(9,1));
	if(nParam == 0)
	{
		GetDoc()->m_pMotorParam[nMotor].bNegLimit = FALSE;
	}
	else if(nParam == 1)
	{
		GetDoc()->m_pMotorParam[nMotor].bNegLimit = TRUE;
	}

	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(10,1));
	if(nParam == 0)
	{
		GetDoc()->m_pMotorParam[nMotor].bHomeSen = FALSE;
	}
	else if(nParam == 1)
	{
		GetDoc()->m_pMotorParam[nMotor].bHomeSen = TRUE;
	}
	
	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(11,1));
	if(nParam == 0)
	{
		GetDoc()->m_pMotorParam[nMotor].bAmpFault = FALSE;
	}
	else if(nParam == 1)
	{
		GetDoc()->m_pMotorParam[nMotor].bAmpFault = TRUE;
	}

	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(12,1));
	if(nParam == 0)
	{
		GetDoc()->m_pMotorParam[nMotor].bAmpReset = FALSE;
	}
	else if(nParam == 1)
	{
		GetDoc()->m_pMotorParam[nMotor].bAmpReset = TRUE;
	}

	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(13,1));
	if(nParam == 0)
	{
		GetDoc()->m_pMotorParam[nMotor].bAmpEnable = FALSE;
	}
	else if(nParam == 1)
	{
		GetDoc()->m_pMotorParam[nMotor].bAmpEnable = TRUE;
	}

	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(14,1));
	GetDoc()->m_pMotorParam[nMotor].iInPosion = nParam;

	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(15,1));
	GetDoc()->m_pMotorParam[nMotor].dPosSoftLimit = nParam;

	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(16,1));
	GetDoc()->m_pMotorParam[nMotor].dNegSoftLimit = nParam;

	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(17,1));
	GetDoc()->m_pMotorParam[nMotor].iAcc = nParam;

	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(18,1));
	GetDoc()->m_pMotorParam[nMotor].iHomeMethod = nParam;

	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(20,1));
	GetDoc()->m_pMotorParam[nMotor].iHomeSpeed = nParam;

	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(21,1));
	if(nParam == 0)
	{
		GetDoc()->m_pMotorParam[nMotor].bHomeDir = FALSE;
	}
	else if(nParam == 1)
	{
		GetDoc()->m_pMotorParam[nMotor].bHomeDir = TRUE;
	}

	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(22,1));
	GetDoc()->m_pMotorParam[nMotor].dwHomeTimeOut = nParam;

	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(23,1));
	GetDoc()->m_pMotorParam[nMotor].dGearRatio = nParam;

	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(24,1));
	GetDoc()->m_pMotorParam[nMotor].dGearPitch = nParam;

	nParam = atoi(m_SettingMotorGrid.get_TextMatrix(25,1));
	GetDoc()->m_pMotorParam[nMotor].nAbsEncoderType = nParam;
}


void CFormSetting::MotionGrid( int nAxisNum )
{
	CString strTemp;
	strTemp.Format("%d",GetDoc()->m_pMotorParam[nAxisNum].iMotor);
	m_SettingMotorGrid.put_TextMatrix(1,1,strTemp);
	strTemp.Format("%d",GetDoc()->m_pMotorParam[nAxisNum].iEncoder);
	m_SettingMotorGrid.put_TextMatrix(2,1,strTemp);
	strTemp.Format("%d",GetDoc()->m_pMotorParam[nAxisNum].iVoltage);
	m_SettingMotorGrid.put_TextMatrix(3,1,strTemp);
	strTemp.Format("%d",GetDoc()->m_pMotorParam[nAxisNum].iFeedback);
	m_SettingMotorGrid.put_TextMatrix(4,1,strTemp);
	strTemp.Format("%d",GetDoc()->m_pMotorParam[nAxisNum].bStepMode);
	if(strTemp == "0")
		m_SettingMotorGrid.put_TextMatrix(5,1,"0");
	else
		m_SettingMotorGrid.put_TextMatrix(5,1,"1");
	strTemp.Format("%d",GetDoc()->m_pMotorParam[nAxisNum].bStepMode);
	if(strTemp == "0")
		m_SettingMotorGrid.put_TextMatrix(5,1,"0");
	else
		m_SettingMotorGrid.put_TextMatrix(5,1,"1");
	strTemp.Format("%d",GetDoc()->m_pMotorParam[nAxisNum].bCordinateDir);
	if(strTemp == "0")
		m_SettingMotorGrid.put_TextMatrix(6,1,"0");
	else
		m_SettingMotorGrid.put_TextMatrix(6,1,"1");
	strTemp.Format("%d",GetDoc()->m_pMotorParam[nAxisNum].bEncoderDir);
	if(strTemp == "0")
		m_SettingMotorGrid.put_TextMatrix(7,1,"0");
	else
		m_SettingMotorGrid.put_TextMatrix(7,1,"1");
	strTemp.Format("%d",GetDoc()->m_pMotorParam[nAxisNum].bPosLimit);
	if(strTemp == "0")
		m_SettingMotorGrid.put_TextMatrix(8,1,"0");
	else
		m_SettingMotorGrid.put_TextMatrix(8,1,"1");
	strTemp.Format("%d",GetDoc()->m_pMotorParam[nAxisNum].bNegLimit);
	if(strTemp == "0")
		m_SettingMotorGrid.put_TextMatrix(9,1,"0");
	else
		m_SettingMotorGrid.put_TextMatrix(9,1,"1");
	strTemp.Format("%d",GetDoc()->m_pMotorParam[nAxisNum].bHomeSen);
	if(strTemp == "0")
		m_SettingMotorGrid.put_TextMatrix(10,1,"0");
	else
		m_SettingMotorGrid.put_TextMatrix(10,1,"1");
	strTemp.Format("%d",GetDoc()->m_pMotorParam[nAxisNum].bAmpFault);
	if(strTemp == "0")
		m_SettingMotorGrid.put_TextMatrix(11,1,"0");
	else
		m_SettingMotorGrid.put_TextMatrix(11,1,"1");
	strTemp.Format("%d",GetDoc()->m_pMotorParam[nAxisNum].bAmpReset);
	if(strTemp == "0")
		m_SettingMotorGrid.put_TextMatrix(12,1,"0");
	else
		m_SettingMotorGrid.put_TextMatrix(12,1,"1");
	strTemp.Format("%d",GetDoc()->m_pMotorParam[nAxisNum].bAmpEnable);
	if(strTemp == "0")
		m_SettingMotorGrid.put_TextMatrix(13,1,"0");
	else
		m_SettingMotorGrid.put_TextMatrix(13,1,"1");
	strTemp.Format("%d",GetDoc()->m_pMotorParam[nAxisNum].iInPosion);
	m_SettingMotorGrid.put_TextMatrix(14,1,strTemp);
	strTemp.Format("%7.3f",GetDoc()->m_pMotorParam[nAxisNum].dPosSoftLimit);
	m_SettingMotorGrid.put_TextMatrix(15,1,strTemp);
	strTemp.Format("%7.3f",GetDoc()->m_pMotorParam[nAxisNum].dNegSoftLimit);
	m_SettingMotorGrid.put_TextMatrix(16,1,strTemp);
	strTemp.Format("%d",GetDoc()->m_pMotorParam[nAxisNum].iAcc);
	m_SettingMotorGrid.put_TextMatrix(17,1,strTemp);
	strTemp.Format("%d",GetDoc()->m_pMotorParam[nAxisNum].iHomeMethod);
	m_SettingMotorGrid.put_TextMatrix(18,1,strTemp);
	strTemp.Format("%d",GetDoc()->m_pMotorParam[nAxisNum].iHomeSpeed);
	m_SettingMotorGrid.put_TextMatrix(20,1,strTemp);
	strTemp.Format("%d",GetDoc()->m_pMotorParam[nAxisNum].bHomeDir);
	if(strTemp == "0")
		m_SettingMotorGrid.put_TextMatrix(21,1,"0");
	else
		m_SettingMotorGrid.put_TextMatrix(21,1,"1");

	strTemp.Format("%ld",GetDoc()->m_pMotorParam[nAxisNum].dwHomeTimeOut);
	m_SettingMotorGrid.put_TextMatrix(22,1,strTemp);

	strTemp.Format("%7.3f",GetDoc()->m_pMotorParam[nAxisNum].dGearRatio);
	m_SettingMotorGrid.put_TextMatrix(23,1,strTemp);

	strTemp.Format("%7.3f",GetDoc()->m_pMotorParam[nAxisNum].dGearPitch);
	m_SettingMotorGrid.put_TextMatrix(24,1,strTemp);

	m_MotorSpeed = GetDoc()->m_pMotorParam[nAxisNum].dSpeed;

	strTemp.Format("%d",GetDoc()->m_pMotorParam[nAxisNum].nAbsEncoderType);
	m_SettingMotorGrid.put_TextMatrix(25,1,strTemp);
	UpdateData(FALSE);

}

void CFormSetting::OnBnClickedBtnPoweron()
{
	int nAxis = m_cmb_MotionSelect.GetCurSel();
	switch(nAxis)
	{
	case 0:
		if(GetDoc()->m_pMotion->GetServoOnStatus(AXIS_CCD_X0))
		{
			GetDoc()->m_pMotion->SetAmpEnable(AXIS_CCD_X0, FALSE);
		}
		else
		{
			GetDoc()->m_pMotion->SetAmpEnable(AXIS_CCD_X0, TRUE);
		}
		break;
	case 1:
		if(GetDoc()->m_pMotion->GetServoOnStatus(AXIS_CCD_X1))
		{
			GetDoc()->m_pMotion->SetAmpEnable(AXIS_CCD_X1, FALSE);
		}
		else
		{
			GetDoc()->m_pMotion->SetAmpEnable(AXIS_CCD_X1,TRUE);
		}
		break;
	case 2:
		if(GetDoc()->m_pMotion->GetServoOnStatus(AXIS_CCD_X2))
		{
			GetDoc()->m_pMotion->SetAmpEnable(AXIS_CCD_X2, FALSE);
		}
		else
		{
			GetDoc()->m_pMotion->SetAmpEnable(AXIS_CCD_X2, TRUE);
		}
		break;
	case 3:
		if(GetDoc()->m_pMotion->GetServoOnStatus(AXIS_CCD_X3))
		{
			GetDoc()->m_pMotion->SetAmpEnable(AXIS_CCD_X3, FALSE);
		}
		else
		{
			GetDoc()->m_pMotion->SetAmpEnable(AXIS_CCD_X3, TRUE);
		}
		break;
	case 4:
		if(GetDoc()->m_pMotion->GetServoOnStatus(AXIS_CCD_Y0))
		{
			GetDoc()->m_pMotion->SetAmpEnable(AXIS_CCD_Y0, FALSE);
		}
		else
		{
			GetDoc()->m_pMotion->SetAmpEnable(AXIS_CCD_Y0, TRUE);
		}
		break;
	case 5:
		if(GetDoc()->m_pMotion->GetServoOnStatus(AXIS_CCD_Y1))
		{
			GetDoc()->m_pMotion->SetAmpEnable(AXIS_CCD_Y1, FALSE);
		}
		else
		{
			GetDoc()->m_pMotion->SetAmpEnable(AXIS_CCD_Y1, TRUE);
		}
		break;
	case 6:
		if(GetDoc()->m_pMotion->GetServoOnStatus(AXIS_CCD_Y2))
		{
			GetDoc()->m_pMotion->SetAmpEnable(AXIS_CCD_Y2, FALSE);
		}
		else
		{
			GetDoc()->m_pMotion->SetAmpEnable(AXIS_CCD_Y2, TRUE);
		}
		break;
	case 7:
		if(GetDoc()->m_pMotion->GetServoOnStatus(AXIS_CCD_Y3))
		{
			GetDoc()->m_pMotion->SetAmpEnable(AXIS_CCD_Y3, FALSE);
		}
		else
		{
			GetDoc()->m_pMotion->SetAmpEnable(AXIS_CCD_Y3, TRUE);
		}
		break;
	case 8:
		if(GetDoc()->m_pMotion->GetServoOnStatus(AXIS_TILT_MOVE))
		{
			GetDoc()->m_pMotion->SetAmpEnable(AXIS_TILT_MOVE, FALSE);
		}
		else
		{
			GetDoc()->m_pMotion->SetAmpEnable(AXIS_TILT_MOVE, TRUE);
		}
		break;
	}
}

void CFormSetting::SetSocketParam(void)
{
	GetComm()->m_SocketLC.SetServer(	(BOOL)atoi(m_SocketGrid.get_TextMatrix(1,1)));
	GetComm()->m_SocketLC.SetIP(		m_SocketGrid.get_TextMatrix(2,1));
	GetComm()->m_SocketLC.SetPort(	(long)atoi(m_SocketGrid.get_TextMatrix(3,1)));
	GetComm()->m_SocketVI.SetServer(	(BOOL)atoi(m_SocketGrid.get_TextMatrix(4,1)));
	GetComm()->m_SocketVI.SetIP(		m_SocketGrid.get_TextMatrix(5,1));
	GetComm()->m_SocketVI.SetPort(	(long)atoi(m_SocketGrid.get_TextMatrix(6,1)));
	GetComm()->m_SocketPG.SetServer(	(BOOL)atoi(m_SocketGrid.get_TextMatrix(7,1)));
	GetComm()->m_SocketPG.SetIP(		m_SocketGrid.get_TextMatrix(8,1));
	GetComm()->m_SocketPG.SetPort(	(long)atoi(m_SocketGrid.get_TextMatrix(9,1)));
}

void CFormSetting::GetSocketParam(void)
{
	CString strText;
	strText.Format("%d", GetComm()->m_SocketLC.IsServer());	m_SocketGrid.put_TextMatrix(1,1,  strText	);
	strText.Format("%s", GetComm()->m_SocketLC.GetIP());		m_SocketGrid.put_TextMatrix(2,1,  strText	);
	strText.Format("%ld", GetComm()->m_SocketLC.GetPort());	m_SocketGrid.put_TextMatrix(3,1,  strText	);
	strText.Format("%d", GetComm()->m_SocketVI.IsServer());	m_SocketGrid.put_TextMatrix(4,1,  strText	);
	strText.Format("%s", GetComm()->m_SocketVI.GetIP());		m_SocketGrid.put_TextMatrix(5,1,  strText	);
	strText.Format("%ld", GetComm()->m_SocketVI.GetPort());	m_SocketGrid.put_TextMatrix(6,1,  strText	);
	strText.Format("%d", GetComm()->m_SocketPG.IsServer());	m_SocketGrid.put_TextMatrix(7,1,  strText	);
	strText.Format("%s", GetComm()->m_SocketPG.GetIP());		m_SocketGrid.put_TextMatrix(8,1,  strText	);
	strText.Format("%ld", GetComm()->m_SocketPG.GetPort());	m_SocketGrid.put_TextMatrix(9,1,  strText	);
}

void CFormSetting::OnBnClickedSearchSearch()
{
	int nAxis = m_cmb_MotionSelect.GetCurSel();
	::AfxBeginThread(HomeSearchProc, (LPVOID)nAxis, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
}

void CFormSetting::OnBnClickedServoOnoff()
{
	int nAxis = m_cmb_MotionSelect.GetCurSel();

	if( GetLineType() == REMOVE_TILTING_TYPE && nAxis == AXIS_TILT_MOVE )
	{	// 2009.06.15
		if( !GetMelsec()->MelsecNetBitRecv(MEL_SW2_TILT_SERVO_STAT) ) 
		{
			GetMelsec()->MelsecNetBitOn(MEL_SW2_TILT_SERVO_ON);
			GetMelsec()->MelsecNetBitOff(MEL_SW2_TILT_SERVO_OFF);

			GetDeviceCtl()->m_dToggleTimer = GetTickCount();
			GetMainFrame()->SetTimer(TIMER_BIT_TOGGLE, 1000, NULL);
		}
		else
		{

			GetMelsec()->MelsecNetBitOn(MEL_SW2_TILT_SERVO_OFF);
			GetMelsec()->MelsecNetBitOff(MEL_SW2_TILT_SERVO_ON);

			GetDeviceCtl()->m_dToggleTimer = GetTickCount();
			GetMainFrame()->SetTimer(TIMER_BIT_TOGGLE, 1000, NULL);
		}
	}
	else
	{
		if(GetDoc()->m_pMotion->GetServoOnStatus(nAxis))
			GetDoc()->m_pMotion->SetAmpEnable(nAxis, FALSE);
		else
			GetDoc()->m_pMotion->SetAmpEnable(nAxis, TRUE);
	}
}

void CFormSetting::OnBnClickedBtnInspCountSet()
{
	// TODO: Add your control notification handler code here
	if((GetDoc()->m_bEngLogin == TRUE) || (GetDoc()->m_bSuperLogin == TRUE))
	{
		UpdateData(TRUE);
		GetDoc()->m_nInspection = m_nInspectionResetCount;
	}
	else
		AfxMessageBox("Engineers more than is available.");

}

void CFormSetting::OnChangeUserIswitchledxInspCount()
{
	// TODO: Add your message handler code here
	if(m_Chk_InspCount.get_Active())
		GetDoc()->m_bInspCount = TRUE;
	else
		GetDoc()->m_bInspCount = FALSE;
}
