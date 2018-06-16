// DlgNumPad.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MT.h"
#include "DlgNumPad.h"


// CDlgNumPad 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgNumPad, CDialog)

CDlgNumPad::CDlgNumPad(CWnd* pParent /*=NULL*/)
: CDialog(CDlgNumPad::IDD, pParent)
{
	m_bPasswordType = FALSE;
}

CDlgNumPad::~CDlgNumPad()
{
}

void CDlgNumPad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IEDITX1, m_IeditNum);
	DDX_Control(pDX, IDC_IPHONEPADX1, m_NumPadCtrl);
}


BEGIN_MESSAGE_MAP(CDlgNumPad, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CDlgNumPad::OnEditDataClear)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgNumPad::OnDataEnterCancel)
	ON_BN_CLICKED(IDC_BUTTON_DATA_ENTER, &CDlgNumPad::OnEditDataEnter)

END_MESSAGE_MAP()


void CDlgNumPad::OnEditDataClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_IeditNum.put_Value("");
}

void CDlgNumPad::OnDataEnterCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EndDialog(IDCANCEL);
}

void CDlgNumPad::OnEditDataEnter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_strEditText = m_IeditNum.get_Value();
	EndDialog(IDOK);
}
// CDlgNumPad 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDlgNumPad, CDialog)
	ON_EVENT(CDlgNumPad, IDC_IPHONEPADX1, 1, CDlgNumPad::OnKeyClickNumPad, VTS_I4 VTS_I2)
END_EVENTSINK_MAP()

void CDlgNumPad::OnKeyClickNumPad(long KeyCode, short AChar)
{
	CString strTmp;

	if( AChar == *"*" ) AChar = '.';
	if( AChar == *"#" ) AChar = '-';

	strTmp.Format("%s%c", m_IeditNum.get_Value() , AChar );

	m_IeditNum.put_Value(strTmp);

}

void CDlgNumPad::SetPasswordType()
{	
	m_bPasswordType = TRUE;
}
BOOL CDlgNumPad::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_bPasswordType)
		m_IeditNum.put_PasswordChar("*");

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
