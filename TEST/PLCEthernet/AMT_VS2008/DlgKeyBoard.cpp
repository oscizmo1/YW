// DlgKeyBoard.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MT.h"
#include "DlgKeyBoard.h"


// CDlgKeyBoard 대화 상자입니다.

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


// CDlgKeyBoard 메시지 처리기입니다.

void CDlgKeyBoard::OnCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EndDialog(IDCANCEL);
}

void CDlgKeyBoard::OnDataEnter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_strEditText = m_EditKeyboardCtrl.get_Value();

	EndDialog(IDOK);
}

BOOL CDlgKeyBoard::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_KeyBoardCtrl.put_FocusHandle((long)m_EditKeyboardCtrl.m_hWnd );
	m_EditKeyboardCtrl.SetFocus();
	if(m_bPasswordType)
		m_EditKeyboardCtrl.put_PasswordChar("*");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgKeyBoard::SetPasswordType()
{
	m_bPasswordType = TRUE;
}
