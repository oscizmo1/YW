// DlgContactor.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "DlgContactor.h"


// CDlgContactor dialog

IMPLEMENT_DYNAMIC(CDlgContactor, CDialog)

CDlgContactor::CDlgContactor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgContactor::IDD, pParent)
{

}

CDlgContactor::~CDlgContactor()
{
}

void CDlgContactor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABEL_CONTACTOR1, m_lblContactor1);
	DDX_Control(pDX, IDC_LABEL_CONTACTOR2, m_lblContactor2);
	DDX_Control(pDX, IDC_LABEL_CONTACTOR3, m_lblContactor3);
	DDX_Control(pDX, IDC_LABEL_CONTACTOR4, m_lblContactor4);
}


BEGIN_MESSAGE_MAP(CDlgContactor, CDialog)
END_MESSAGE_MAP()


// CDlgContactor message handlers
BEGIN_EVENTSINK_MAP(CDlgContactor, CDialog)
	ON_EVENT(CDlgContactor, IDC_LABEL_CONTACTOR1, DISPID_CLICK, CDlgContactor::ClickLabelContactor1, VTS_NONE)
	ON_EVENT(CDlgContactor, IDC_LABEL_CONTACTOR2, DISPID_CLICK, CDlgContactor::ClickLabelContactor2, VTS_NONE)
	ON_EVENT(CDlgContactor, IDC_LABEL_CONTACTOR3, DISPID_CLICK, CDlgContactor::ClickLabelContactor3, VTS_NONE)
	ON_EVENT(CDlgContactor, IDC_LABEL_CONTACTOR4, DISPID_CLICK, CDlgContactor::ClickLabelContactor4, VTS_NONE)
END_EVENTSINK_MAP()

void CDlgContactor::ClickLabelContactor1()
{
	// TODO: Add your message handler code here
	strContactor = m_lblContactor1.get_Caption();
	OnOK();
}

void CDlgContactor::ClickLabelContactor2()
{
	// TODO: Add your message handler code here
	strContactor = m_lblContactor2.get_Caption();
	OnOK();
}

void CDlgContactor::ClickLabelContactor3()
{
	// TODO: Add your message handler code here
	strContactor = m_lblContactor3.get_Caption();
	OnOK();
}

void CDlgContactor::ClickLabelContactor4()
{
	// TODO: Add your message handler code here
	strContactor = m_lblContactor4.get_Caption();
	OnOK();
}
