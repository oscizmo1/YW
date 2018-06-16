// UserChange.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MT.h"
#include "UserChange.h"
#include ".\userchange.h"
#include "UserSet.h"
#include "DlgNumPad.h"
#include "DlgUserClass.h"
#include "DlgKeyBoard.h"
#include "MainFrm.h"


// CUserChange 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUserChange, CDialog)
CUserChange::CUserChange(CWnd* pParent /*=NULL*/)
	: CDialog(CUserChange::IDD, pParent)
{
}

CUserChange::~CUserChange()
{
}

void CUserChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABEL_PASSWORD_INSERT, m_lblPassWord);
	DDX_Control(pDX, IDC_LABEL_CLASS_SELECT, m_lblUserClass);
}


BEGIN_MESSAGE_MAP(CUserChange, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CUserChange 메시지 처리기입니다.

void CUserChange::OnBnClickedOk()
{
	if(m_nUserClass != GetDoc()->m_nUserClass)
	{
		AfxMessageBox("Password is Empty");
		return;
	}
	GetDoc()->m_bEngLogin = (m_nUserClass == OPERATOR) ? FALSE:TRUE;
	GetDoc()->m_bSuperLogin = (m_nUserClass == SUPER_USER) ? TRUE:FALSE;

	::SendMessage(GetMainFrame()->m_hBottom, WM_CHANGE_USER, (WPARAM)NULL, (LPARAM)NULL);
	::SendMessage(GetMainFrame()->m_pForm[FORM_AUTO]->m_hWnd, WM_CHANGE_USER, (WPARAM)NULL, (LPARAM)NULL);
	OnOK();
}

BOOL CUserChange::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BEGIN_EVENTSINK_MAP(CUserChange, CDialog)
	ON_EVENT(CUserChange, IDC_LABEL_PASSWORD_INSERT, 1, CUserChange::OnClickLabelPasswordInsert, VTS_NONE)
	ON_EVENT(CUserChange, IDC_LABEL_CLASS_SELECT, 1, CUserChange::OnClickLabelClassSelect, VTS_NONE)
END_EVENTSINK_MAP()

void CUserChange::OnClickLabelPasswordInsert()
{
	CDlgKeyBoard dlg;
	dlg.SetPasswordType();
	if(m_lblUserClass.get_Caption() != "")
	{
			if(m_nUserClass != OPERATOR)
			{
				if( dlg.DoModal() == IDOK )
				{
					CString strText;

					strText = dlg.m_strEditText;
					strText.MakeUpper();
					if( strText !="" )	
					{
						if(m_lblUserClass.get_Caption() == "ENGINEER")
						{
							if(GetEngineerPassWord() == strText)
							{
								GetDoc()->m_bEngLogin = TRUE;
								GetDoc()->m_nUserClass = ENGINEER;
							}
							m_lblPassWord.put_Caption("****");
						}

					}
				}
			}
			else
			{
				m_lblPassWord.put_Caption("None");
				GetDoc()->m_nUserClass = m_nUserClass = OPERATOR;
			}
	}
	else
		AfxMessageBox("User Class was not set.");

}

void CUserChange::OnClickLabelClassSelect()
{
	CDlgUserClass dlg;
	dlg.DoModal();
	m_strUserClass = dlg.m_strUserClass;
	m_nUserClass = dlg.m_nUserClass;
	m_lblUserClass.put_Caption(m_strUserClass);

	if(m_nUserClass == OPERATOR)
	{
		m_lblPassWord.put_Caption("None");
		m_lblPassWord.put_Enabled(FALSE);
		// Operator는 password가 없으므로 바로 적용한다.
		GetDoc()->SetSuperUser(FALSE);
		GetDoc()->m_nUserClass = m_nUserClass;
	}
	else 
	{
		m_lblPassWord.put_Caption("Input Password");
		m_lblPassWord.put_Enabled(TRUE);
	}
}

