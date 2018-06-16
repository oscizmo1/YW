// DlgUserClass.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "DlgUserClass.h"
#include "UserChange.h"


// CDlgUserClass dialog

IMPLEMENT_DYNAMIC(CDlgUserClass, CDialog)

CDlgUserClass::CDlgUserClass(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUserClass::IDD, pParent)
{

}

CDlgUserClass::~CDlgUserClass()
{
}

void CDlgUserClass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgUserClass, CDialog)
END_MESSAGE_MAP()


// CDlgUserClass message handlers
BEGIN_EVENTSINK_MAP(CDlgUserClass, CDialog)
	ON_EVENT(CDlgUserClass, IDC_LABEL_ENGINEER, 1, CDlgUserClass::OnClickLabelEngineer, VTS_NONE)
	ON_EVENT(CDlgUserClass, IDC_LABEL_OPERATOR, 1, CDlgUserClass::OnClickLabelOperator, VTS_NONE)
END_EVENTSINK_MAP()

void CDlgUserClass::OnClickLabelEngineer()
{
	m_strUserClass = "ENGINEER";
	m_nUserClass = ENGINEER;

	OnOK();
}

void CDlgUserClass::OnClickLabelOperator()
{
	m_strUserClass = "OPERATOR";
	m_nUserClass = OPERATOR;

	OnOK();
}

