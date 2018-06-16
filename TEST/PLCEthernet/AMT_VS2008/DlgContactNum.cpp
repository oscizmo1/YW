// DlgContactNum.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "DlgContactNum.h"


// CDlgContactNum dialog

IMPLEMENT_DYNAMIC(CDlgContactNum, CDialog)

CDlgContactNum::CDlgContactNum(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgContactNum::IDD, pParent)
{

}

CDlgContactNum::~CDlgContactNum()
{
}

void CDlgContactNum::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABEL_CONTACT4, m_lbl4ContactNum);
	DDX_Control(pDX, IDC_LABEL_CONTACT2, m_lbl2ContactNum);
	DDX_Control(pDX, IDC_LABEL_CONTACT1, m_lbl1ContactNum);
}


BEGIN_MESSAGE_MAP(CDlgContactNum, CDialog)
END_MESSAGE_MAP()


// CDlgContactNum message handlers
BEGIN_EVENTSINK_MAP(CDlgContactNum, CDialog)
	ON_EVENT(CDlgContactNum, IDC_LABEL_CONTACT4, DISPID_CLICK, CDlgContactNum::ClickLabelContact4, VTS_NONE)
	ON_EVENT(CDlgContactNum, IDC_LABEL_CONTACT2, DISPID_CLICK, CDlgContactNum::ClickLabelContact2, VTS_NONE)
	ON_EVENT(CDlgContactNum, IDC_LABEL_CONTACT1, DISPID_CLICK, CDlgContactNum::ClickLabelContact1, VTS_NONE)
END_EVENTSINK_MAP()

void CDlgContactNum::ClickLabelContact4()
{
	// TODO: Add your message handler code here
	strContactNum = m_lbl4ContactNum.get_Caption();
	OnOK();
}

void CDlgContactNum::ClickLabelContact2()
{
	// TODO: Add your message handler code here
	strContactNum = m_lbl2ContactNum.get_Caption();
	OnOK();

}

void CDlgContactNum::ClickLabelContact1()
{
	// TODO: Add your message handler code here
	strContactNum = m_lbl1ContactNum.get_Caption();
	OnOK();

}
