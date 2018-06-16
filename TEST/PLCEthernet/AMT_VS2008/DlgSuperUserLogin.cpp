// DlgSuperUserLogin.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "MainFrm.h"
#include "DlgSuperUserLogin.h"
#include "DlgKeyBoard.h"
#include "DlgNumPad.h"


// CDlgSuperUserLogin dialog

IMPLEMENT_DYNAMIC(CDlgSuperUserLogin, CDialog)

CDlgSuperUserLogin::CDlgSuperUserLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSuperUserLogin::IDD, pParent)
{

}

CDlgSuperUserLogin::~CDlgSuperUserLogin()
{
}

void CDlgSuperUserLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABEL_NAME, m_lblName);
	DDX_Control(pDX, IDC_LABEL_PASSWORD, m_lblPassWord);
}


BEGIN_MESSAGE_MAP(CDlgSuperUserLogin, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgSuperUserLogin::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSuperUserLogin message handlers
BEGIN_EVENTSINK_MAP(CDlgSuperUserLogin, CDialog)
	ON_EVENT(CDlgSuperUserLogin, IDC_LABEL_NAME, 1, CDlgSuperUserLogin::OnClickLabelName, VTS_NONE)
	ON_EVENT(CDlgSuperUserLogin, IDC_LABEL_PASSWORD, 1, CDlgSuperUserLogin::OnClickLabelPassword, VTS_NONE)
END_EVENTSINK_MAP()

void CDlgSuperUserLogin::OnClickLabelName()
{
	// TODO: Add your message handler code here
	CDlgKeyBoard dlg;
	CString strText;

	if( dlg.DoModal() == IDOK )
	{
		strText = dlg.m_strEditText;
		if( strText !="" )	
		{
			m_lblName.put_Caption(strText);
		}
	}	
}

void CDlgSuperUserLogin::OnClickLabelPassword()
{
	CDlgNumPad dlg;
	dlg.SetPasswordType();
	CString strText;

	if( dlg.DoModal() == IDOK )
	{
		strText = dlg.m_strEditText;
		if( strText !="" )	
		{
			if(strText == "YoungWoo")
			{
				AfxMessageBox("Super_User Login Success");
				GetDoc()->SetSuperUser(TRUE);
				m_lblPassWord.put_Caption("*******");				
			}
		}
	}	
}

void CDlgSuperUserLogin::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	::SendMessage(GetMainFrame()->m_hTop, WM_CHANGE_USER, (WPARAM)NULL, (LPARAM)NULL);
	::SendMessage(GetMainFrame()->m_hBottom, WM_CHANGE_USER, (WPARAM)NULL, (LPARAM)NULL);
	OnOK();
}

