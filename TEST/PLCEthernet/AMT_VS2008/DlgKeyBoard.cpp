// DlgKeyBoard.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MT.h"
#include "DlgKeyBoard.h"


// CDlgKeyBoard ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgKeyBoard, CDialog)

CDlgKeyBoard::CDlgKeyBoard(CWnd* pParent /*=NULL*/)
: CDialog(CDlgKeyBoard::IDD, pParent)
{
	m_bPasswordType = FALSE;
}

CDlgKeyBoard::~CDlgKeyBoard()
{
}

void CDlgKeyBoard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IKEYBOARDX, m_KeyBoardCtrl);
	DDX_Control(pDX, IDC_IEDITX1, m_EditKeyboardCtrl);
}


BEGIN_MESSAGE_MAP(CDlgKeyBoard, CDialog)

	ON_BN_CLICKED(IDC_BUTTON2, &CDlgKeyBoard::OnCancel)
	ON_BN_CLICKED(IDC_BTN_DATA_ENTER, &CDlgKeyBoard::OnDataEnter)
END_MESSAGE_MAP()


// CDlgKeyBoard �޽��� ó�����Դϴ�.

void CDlgKeyBoard::OnCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	EndDialog(IDCANCEL);
}

void CDlgKeyBoard::OnDataEnter()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_strEditText = m_EditKeyboardCtrl.get_Value();

	EndDialog(IDOK);
}

BOOL CDlgKeyBoard::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_KeyBoardCtrl.put_FocusHandle((long)m_EditKeyboardCtrl.m_hWnd );
	m_EditKeyboardCtrl.SetFocus();
	if(m_bPasswordType)
		m_EditKeyboardCtrl.put_PasswordChar("*");

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CDlgKeyBoard::SetPasswordType()
{
	m_bPasswordType = TRUE;
}
