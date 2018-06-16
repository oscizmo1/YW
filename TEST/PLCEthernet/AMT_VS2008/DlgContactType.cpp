// DlgContactType.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "DlgContactType.h"

// CDlgContactType dialog

IMPLEMENT_DYNAMIC(CDlgContactType, CDialog)


CDlgContactType::CDlgContactType(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgContactType::IDD, pParent)
{

}

CDlgContactType::~CDlgContactType()
{
}

void CDlgContactType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABEL_FFC, m_lblFFC);
	DDX_Control(pDX, IDC_LABEL_FPC, m_lblFPC);
	DDX_Control(pDX, IDC_LABEL_PIN, m_lblPin);
}


BEGIN_MESSAGE_MAP(CDlgContactType, CDialog)
END_MESSAGE_MAP()


// CDlgContactType message handlers
BEGIN_EVENTSINK_MAP(CDlgContactType, CDialog)
	ON_EVENT(CDlgContactType, IDC_LABEL_FFC, DISPID_CLICK, CDlgContactType::ClickLabelFfc, VTS_NONE)
	ON_EVENT(CDlgContactType, IDC_LABEL_FPC, DISPID_CLICK, CDlgContactType::ClickLabelFpc, VTS_NONE)
	ON_EVENT(CDlgContactType, IDC_LABEL_PIN, DISPID_CLICK, CDlgContactType::ClickLabelPin, VTS_NONE)
END_EVENTSINK_MAP()

void CDlgContactType::ClickLabelFfc()
{
	// TODO: Add your message handler code here
	strContactType = m_lblFFC.get_Caption();
	OnOK();
}

void CDlgContactType::ClickLabelFpc()
{
	// TODO: Add your message handler code here
	strContactType = m_lblFPC.get_Caption();
	OnOK();
}

BOOL CDlgContactType::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	if(GetLineType() != A_GROUP_TYPE)
	{
		m_lblPin.EnableWindow(FALSE);
		m_lblPin.put_BackColor(RGB(0,255,0));
	}
	else
	{
		m_lblPin.EnableWindow(TRUE);
		m_lblPin.put_BackColor(RGB(0,0,0));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgContactType::ClickLabelPin()
{
	// TODO: Add your message handler code here
	strContactType = m_lblPin.get_Caption();
	OnOK();
}
