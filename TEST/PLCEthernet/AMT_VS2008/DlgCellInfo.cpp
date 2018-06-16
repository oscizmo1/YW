// DlgCellInfo.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "DlgCellInfo.h"
#include "DlgKeyBoard.h"


// CDlgCellInfo dialog

IMPLEMENT_DYNAMIC(CDlgCellInfo, CDialog)

CDlgCellInfo::CDlgCellInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCellInfo::IDD, pParent)
{

}

CDlgCellInfo::~CDlgCellInfo()
{
}

void CDlgCellInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABEL_H_PANEL_ID, m_lblHPanelID);
	DDX_Control(pDX, IDC_LABEL_BATCH_ID, m_lblBatchID);
	DDX_Control(pDX, IDC_LABEL_DEVICE_ID, m_lblDeviceID);
	DDX_Control(pDX, IDC_LABEL_PAIR_H_ID, m_lblPairHID);
	DDX_Control(pDX, IDC_LABEL_PAIR_E_ID, m_lblPairEID);
	DDX_Control(pDX, IDC_LABEL_INSP_FLAG, m_lblInsFlag);
	DDX_Control(pDX, IDC_LABEL_JUDGEMENT, m_lblJudgement);
	DDX_Control(pDX, IDC_LABEL_PROD_KIND, m_lblProdKind);
}


BEGIN_MESSAGE_MAP(CDlgCellInfo, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgCellInfo::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgCellInfo::OnBnClickedCancel)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDlgCellInfo message handlers

void CDlgCellInfo::OnBnClickedOk()
{
	CString strLog;
	strLog.Format("[Glass Data Exchange]\tEPanelID:%s\tBatchID:%s\tDeviceID:%s", 
		GetDoc()->m_LastGlass.strEPanelID, GetDoc()->m_LastGlass.strBatchID, GetDoc()->m_LastGlass.strDeviceID);
	GetDoc()->SetLog(strLog, LOG_GLASS_DATA);
	GetDoc()->m_LastGlass.strHPanelID	 =		m_lblHPanelID.get_Caption();
	GetDoc()->m_LastGlass.strEPanelID	 =		m_lblHPanelID.get_Caption();
	GetDoc()->m_LastGlass.strBatchID	 =		m_lblBatchID.get_Caption();
	GetDoc()->m_LastGlass.strDeviceID	 =		m_lblDeviceID.get_Caption();
	GetDoc()->m_LastGlass.strPairH	 =			m_lblPairHID.get_Caption();
	GetDoc()->m_LastGlass.strPairE	 =			m_lblPairEID.get_Caption();
	GetDoc()->m_LastGlass.strInspection	 =		m_lblInsFlag.get_Caption();
	memcpy(GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sJudgement, m_lblJudgement.get_Caption().GetBuffer(0), m_lblJudgement.get_Caption().GetLength());
	memcpy(GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sProdKind, m_lblProdKind.get_Caption().GetBuffer(0), m_lblProdKind.get_Caption().GetLength());
	memcpy(GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sStepID, "7M020", 5);
	memcpy(GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sRunLine, "0", 1);
	memcpy(GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sUniqueID, "0", 1);
	GetDoc()->m_pEqMainSeq->LastGlassToGlassData();

	// Unscrap 보고
	GetComm()->PanelTransfer2LC(ePanelUnScrap) ;
	OnOK();
}

void CDlgCellInfo::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

BOOL CDlgCellInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDoc()->LoadLastGlassFile();
// 	m_lblHPanelID.put_Caption(GetDoc()->m_LastGlass.strHPanelID);
	m_lblHPanelID.put_Caption("");
	m_lblBatchID.put_Caption(GetDoc()->m_LastGlass.strBatchID);
	m_lblDeviceID.put_Caption(GetDoc()->m_LastGlass.strDeviceID);
// 	m_lblPairHID.put_Caption(GetDoc()->m_LastGlass.strPairH);
// 	m_lblPairEID.put_Caption(GetDoc()->m_LastGlass.strPairE);
// 	m_lblInsFlag.put_Caption("ON");
// 	m_lblJudgement.put_Caption("OK");
// 	m_lblProdKind.put_Caption("US");
	m_lblPairHID.put_Caption("          ");
	m_lblPairEID.put_Caption("          ");
	m_lblInsFlag.put_Caption("          ");
	m_lblJudgement.put_Caption("        ");
	m_lblProdKind.put_Caption("         ");

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCellInfo::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{

		BringWindowToTop();
	}
}
BEGIN_EVENTSINK_MAP(CDlgCellInfo, CDialog)
	ON_EVENT(CDlgCellInfo, IDC_LABEL_H_PANEL_ID, DISPID_CLICK, CDlgCellInfo::ClickLabelHPanelId, VTS_NONE)
	ON_EVENT(CDlgCellInfo, IDC_LABEL_BATCH_ID, DISPID_CLICK, CDlgCellInfo::ClickLabelBatchId, VTS_NONE)
	ON_EVENT(CDlgCellInfo, IDC_LABEL_DEVICE_ID, DISPID_CLICK, CDlgCellInfo::ClickLabelDeviceId, VTS_NONE)
	ON_EVENT(CDlgCellInfo, IDC_LABEL_PAIR_H_ID, DISPID_CLICK, CDlgCellInfo::ClickLabelPairHId, VTS_NONE)
	ON_EVENT(CDlgCellInfo, IDC_LABEL_PAIR_E_ID, DISPID_CLICK, CDlgCellInfo::ClickLabelPairEId, VTS_NONE)
	ON_EVENT(CDlgCellInfo, IDC_LABEL_INSP_FLAG, DISPID_CLICK, CDlgCellInfo::ClickLabelInspFlag, VTS_NONE)
	ON_EVENT(CDlgCellInfo, IDC_LABEL_JUDGEMENT, DISPID_CLICK, CDlgCellInfo::ClickLabelJudgement, VTS_NONE)
	ON_EVENT(CDlgCellInfo, IDC_LABEL_PROD_KIND, DISPID_CLICK, CDlgCellInfo::ClickLabelProdKind, VTS_NONE)
END_EVENTSINK_MAP()

void CDlgCellInfo::ClickLabelHPanelId()
{
	CDlgKeyBoard dlg;
	if( dlg.DoModal() == IDOK )
	{
		dlg.m_strEditText.Trim(_T(" "));		// [7/18/2009 Hui] " " 문자 처리.
		dlg.m_strEditText.Trim(_T("	"));	// [7/18/2009 Hui] tab 문자 처리.
		dlg.m_strEditText.Trim(_T("\t"));	// [7/18/2009 Hui] tab 문자 처리.
		dlg.m_strEditText.MakeUpper();

		if(dlg.m_strEditText.GetLength() > 7)	// [7/18/2009 Hui] 8자리 미만으로 기입할 수 없게..
		{
			m_lblHPanelID.put_Caption(dlg.m_strEditText.Left(sizeHPANELID));	// [7/18/2009 Hui] 최대 자릿수 12로 Fix.
		}
		else
			AfxMessageBox("HGlassID's Length must be '8'!");
	}
}

void CDlgCellInfo::ClickLabelBatchId()
{
	CDlgKeyBoard dlg;
	if( dlg.DoModal() == IDOK )
	{
		dlg.m_strEditText.Trim(_T(" "));		// [7/18/2009 Hui] " " 문자 처리.
		dlg.m_strEditText.Trim(_T("	"));	// [7/18/2009 Hui] tab 문자 처리.
		dlg.m_strEditText.Trim(_T("\t"));	// [7/18/2009 Hui] tab 문자 처리.
		dlg.m_strEditText.MakeUpper();

		if(dlg.m_strEditText.GetLength() > 4)	// [7/18/2009 Hui] 8자리 미만으로 기입할 수 없게..
		{
			m_lblBatchID.put_Caption(dlg.m_strEditText.Left(sizeBATCHID));
		}
		else
			AfxMessageBox("HGlassID's Length must be '5'!");
	}
}

void CDlgCellInfo::ClickLabelDeviceId()
{
	CDlgKeyBoard dlg;
	if( dlg.DoModal() == IDOK )
	{
		dlg.m_strEditText.Trim(_T(" "));		// [7/18/2009 Hui] " " 문자 처리.
		dlg.m_strEditText.Trim(_T("	"));	// [7/18/2009 Hui] tab 문자 처리.
		dlg.m_strEditText.Trim(_T("\t"));	// [7/18/2009 Hui] tab 문자 처리.
		dlg.m_strEditText.MakeUpper();

		if(dlg.m_strEditText.GetLength() > 9)	// [7/18/2009 Hui] 8자리 미만으로 기입할 수 없게..
		{
			m_lblDeviceID.put_Caption(dlg.m_strEditText.Left(sizeDEVICEID));
		}
		else
			AfxMessageBox("HGlassID's Length must be '10'!");
	}
}

void CDlgCellInfo::ClickLabelPairHId()
{
	CDlgKeyBoard dlg;
	if( dlg.DoModal() == IDOK )
		m_lblPairHID.put_Caption(dlg.m_strEditText);
}

void CDlgCellInfo::ClickLabelPairEId()
{
	CDlgKeyBoard dlg;
	if( dlg.DoModal() == IDOK )
		m_lblPairEID.put_Caption(dlg.m_strEditText);
}

void CDlgCellInfo::ClickLabelInspFlag()
{
	// TODO: Add your message handler code here
}

void CDlgCellInfo::ClickLabelJudgement()
{
	// TODO: Add your message handler code here
}

void CDlgCellInfo::ClickLabelProdKind()
{
	// TODO: Add your message handler code here
}

BOOL CDlgCellInfo::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN){  if((pMsg->wParam == VK_ESCAPE)){          return TRUE;        }    }
	return CDialog::PreTranslateMessage(pMsg);
}
