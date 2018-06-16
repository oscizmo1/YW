// DlgPCBPinNum.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "DlgPCBPinNum.h"


// CDlgPCBPinNum dialog

IMPLEMENT_DYNAMIC(CDlgPCBPinNum, CDialog)

CDlgPCBPinNum::CDlgPCBPinNum(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPCBPinNum::IDD, pParent)
{

}

CDlgPCBPinNum::~CDlgPCBPinNum()
{
}

void CDlgPCBPinNum::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABEL_PCB68PIN, m_lbl68Pin);
	DDX_Control(pDX, IDC_LABEL_PCB80PIN, m_lbl80Pin);
	DDX_Control(pDX, IDC_LABEL_PCB96PIN, m_lbl96Pin);
}


BEGIN_MESSAGE_MAP(CDlgPCBPinNum, CDialog)
END_MESSAGE_MAP()


// CDlgPCBPinNum message handlers
BEGIN_EVENTSINK_MAP(CDlgPCBPinNum, CDialog)
	ON_EVENT(CDlgPCBPinNum, IDC_LABEL_PCB68PIN, DISPID_CLICK, CDlgPCBPinNum::ClickLabelPcb68pin, VTS_NONE)
	ON_EVENT(CDlgPCBPinNum, IDC_LABEL_PCB80PIN, DISPID_CLICK, CDlgPCBPinNum::ClickLabelPcb80pin, VTS_NONE)
	ON_EVENT(CDlgPCBPinNum, IDC_LABEL_PCB96PIN, DISPID_CLICK, CDlgPCBPinNum::ClickLabelPcb96pin, VTS_NONE)
END_EVENTSINK_MAP()

void CDlgPCBPinNum::ClickLabelPcb68pin()
{
	// TODO: Add your message handler code here
	strPinNum = m_lbl68Pin.get_Caption();

	OnOK();
}

void CDlgPCBPinNum::ClickLabelPcb80pin()
{
	// TODO: Add your message handler code here
	strPinNum = m_lbl80Pin.get_Caption();

	OnOK();
}

void CDlgPCBPinNum::ClickLabelPcb96pin()
{
	// TODO: Add your message handler code here
	strPinNum = m_lbl96Pin.get_Caption();

	OnOK();
}
