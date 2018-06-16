// DlgPCBType.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "DlgPCBType.h"


// CDlgPCBType dialog

IMPLEMENT_DYNAMIC(CDlgPCBType, CDialog)

CDlgPCBType::CDlgPCBType(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPCBType::IDD, pParent)
{

}

CDlgPCBType::~CDlgPCBType()
{
}

void CDlgPCBType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABEL_NORMAL, m_lblNormal);
	DDX_Control(pDX, IDC_LABEL_SNB, m_lblSNB);
	DDX_Control(pDX, IDC_LABEL_LED, m_lblLED);
}


BEGIN_MESSAGE_MAP(CDlgPCBType, CDialog)
END_MESSAGE_MAP()


// CDlgPCBType message handlers
BEGIN_EVENTSINK_MAP(CDlgPCBType, CDialog)
	ON_EVENT(CDlgPCBType, IDC_LABEL_NORMAL, DISPID_CLICK, CDlgPCBType::ClickLabelNormal, VTS_NONE)
	ON_EVENT(CDlgPCBType, IDC_LABEL_SNB, DISPID_CLICK, CDlgPCBType::ClickLabelSnb, VTS_NONE)
	ON_EVENT(CDlgPCBType, IDC_LABEL_LED, DISPID_CLICK, CDlgPCBType::ClickLabelLed, VTS_NONE)
END_EVENTSINK_MAP()

void CDlgPCBType::ClickLabelNormal()
{
	// TODO: Add your message handler code here
	strPCBType = m_lblNormal.get_Caption();
	OnOK();
}

void CDlgPCBType::ClickLabelSnb()
{
	// TODO: Add your message handler code here
	strPCBType = m_lblSNB.get_Caption();
	OnOK();
}

void CDlgPCBType::ClickLabelLed()
{
	// TODO: Add your message handler code here
	strPCBType = m_lblLED.get_Caption();
	OnOK();
}
