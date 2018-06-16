// UserSet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MT.h"
#include "UserSet.h"
#include ".\userset.h"
#include "DlgNumPad.h"

#include "MainFrm.h"

// CUserSet 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUserSet, CDialog)
CUserSet::CUserSet(CWnd* pParent /*=NULL*/)
	: CDialog(CUserSet::IDD, pParent)
{
}

CUserSet::~CUserSet()
{
}

void CUserSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_DIALOG_USER_SET_COMBO_LIST, m_cmbListLevel);
	DDX_Control(pDX, IDD_DIALOG_USER_SET_COMBO_SELECTED, m_cmbSelectedLevel);
	DDX_Control(pDX, IDD_DIALOG_USER_SET_LIST, m_listUser);
	DDX_Control(pDX, IDD_DIALOG_USER_SET_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDD_DIALOG_USER_SET_EDIT_PASSWORD, m_editPwd);
	DDX_Control(pDX, IDC_LABEL_PASSWORD, m_lblPassWord);
}


BEGIN_MESSAGE_MAP(CUserSet, CDialog)
	ON_CBN_SELCHANGE(IDD_DIALOG_USER_SET_COMBO_LIST, OnCbnSelchangeDialogUserSetComboList)
	ON_LBN_SELCHANGE(IDD_DIALOG_USER_SET_LIST, OnLbnSelchangeDialogUserSetList)
	ON_BN_CLICKED(IDD_DIALOG_USER_SET_BUTTON_ADD, OnBnClickedDialogUserSetButtonAdd)
	ON_BN_CLICKED(IDD_DIALOG_USER_SET_BUTTON_DELETE, OnBnClickedDialogUserSetButtonDelete)
	ON_BN_CLICKED(IDD_DIALOG_USER_SET_BUTTON_SAVE, OnBnClickedDialogUserSetButtonSave)
	ON_BN_CLICKED(IDCANCEL, &CUserSet::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_CHANGE, &CUserSet::OnBnClickedBtnChange)
END_MESSAGE_MAP()


// CUserSet 메시지 처리기입니다.

BOOL CUserSet::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_cmbListLevel.AddString("SUPER_USER");
	m_cmbListLevel.AddString("ENGINEER");
	m_cmbListLevel.AddString("TECHNICIAN");
	m_cmbListLevel.AddString("OPERATOR");

	m_cmbSelectedLevel.AddString("SUPER_USER");
	m_cmbSelectedLevel.AddString("ENGINEER");
	m_cmbSelectedLevel.AddString("TECHNICIAN");
	m_cmbSelectedLevel.AddString("OPERATOR");

	UserInfoLoad();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CUserSet::UserInfoLoad()
{
	CFileFind FileFinder;
	CFile file;

	if(FileFinder.FindFile(USER_PATH))
	{
		if(file.Open(USER_PATH,CFile::modeRead,NULL))
		{
			file.Read(&m_UserInfo,(UINT)sizeof(USER_INFO));
			file.Close();
		}
	}
	else
	{
		for(int i=0;i<100;i++)
		{
			m_UserInfo.nLEVEL[i]=OPERATOR;
			m_UserInfo.strName[i] = _T("");
			m_UserInfo.strPwd[i] = _T("");
		}
		UserInfoSave();
	}
}

void CUserSet::UserInfoSave()
{
	CFile file;
	file.Open(USER_PATH,CFile::modeCreate|CFile::modeWrite,NULL);
	file.Write(&m_UserInfo,(UINT)sizeof(USER_INFO));
	file.Close();
}

void CUserSet::OnCbnSelchangeDialogUserSetComboList()
{
	while(m_listUser.GetCount()>0)	m_listUser.DeleteString(0);

	CString strName;

	for(int i=0;i<100;i++)
	{
		strName.Format("%s",m_UserInfo.strName[i]);
		if(m_UserInfo.nLEVEL[i]==m_cmbListLevel.GetCurSel() && strName.Trim()!="")
		{
			m_listUser.AddString(strName.Trim());
		}
	}

	m_cmbSelectedLevel.SetCurSel(m_cmbListLevel.GetCurSel());
	m_editName.SetWindowText("");
	m_editPwd.SetWindowText("");
}

void CUserSet::OnLbnSelchangeDialogUserSetList()
{
	CString strName,strName2;

	m_listUser.GetText(m_listUser.GetCurSel(),strName);
	for(int i=0;i<100;i++)
	{
		strName2.Format("%s",m_UserInfo.strName[i]);
		if(m_UserInfo.nLEVEL[i]==m_cmbListLevel.GetCurSel() && strName.Trim()==strName2.Trim())
		{
			m_cmbSelectedLevel.SetCurSel(m_cmbListLevel.GetCurSel());
			m_editName.SetWindowText(strName.Trim());
			m_editPwd.SetWindowText(m_UserInfo.strPwd[i]);
		}
	}
}

void CUserSet::OnBnClickedDialogUserSetButtonAdd()
{
	CString strSection, strKeyName, strValue;
	CIni ini;
	
	ini.SetIniFilename(USER_PATH);

	GetDlgItemText(IDD_DIALOG_USER_SET_COMBO_SELECTED, strSection);
	GetDlgItemText(IDD_DIALOG_USER_SET_EDIT_NAME, strKeyName);
	GetDlgItemText(IDD_DIALOG_USER_SET_EDIT_PASSWORD, strValue);

	ini.WriteString(strSection, strKeyName, strValue);
}

void CUserSet::OnBnClickedDialogUserSetButtonDelete()
{
	CString strName,strPwd,strName2;
	m_editName.GetWindowText(strName);
	m_editPwd.GetWindowText(strPwd);

	if(((CMainFrame*)GetParentFrame())->m_nUserLevel>m_cmbSelectedLevel.GetCurSel())
	{
		EventWrite("상위 등급 사용자는 삭제할 수 없습니다.",true);
		return;
	}

	if(((CMainFrame*)GetParentFrame())->m_nUserLevel==m_cmbSelectedLevel.GetCurSel()
		&& ((CMainFrame*)GetParentFrame())->m_strUserName==strName.Trim())
	{
		EventWrite("현재 사용자는 삭제할 수 없습니다.",true);
		return;
	}

	if(m_cmbSelectedLevel.GetCurSel()==-1 || strName.Trim().GetLength()==0 || strPwd.Trim().GetLength()==0)
	{
		EventWrite("사용자 정보를 확인하세요.",true);
		return;
	}

	for(int i=0;i<100;i++)
	{
		strName2.Format("%s",m_UserInfo.strName[i]);
		if(m_UserInfo.nLEVEL[i]==m_cmbSelectedLevel.GetCurSel() && strName2.Trim()==strName.Trim())
		{
			m_UserInfo.strName[i] = _T("");
			OnCbnSelchangeDialogUserSetComboList();
			return;
		}
	}
}

void CUserSet::OnBnClickedDialogUserSetButtonSave()
{
	UserInfoSave();
	EventWrite("저장되었습니다.",true);
}

void CUserSet::EventWrite(CString strEvent,bool bPopup,bool bLog)
{
	if(bPopup)
	{
		;
	}
}
void CUserSet::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
BEGIN_EVENTSINK_MAP(CUserSet, CDialog)
END_EVENTSINK_MAP()

void CUserSet::OnClickLabelUserSelect()
{
	// TODO: Add your message handler code here
}

void CUserSet::OnBnClickedBtnChange()
{
	// TODO: Add your control notification handler code here
	CDlgNumPad dlg;
	if(dlg.DoModal() == IDOK)
	{

	}
}
