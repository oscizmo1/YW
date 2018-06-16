// DlgNumPad.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MT.h"
#include "DlgNumPad.h"


// CDlgNumPad ��ȭ �����Դϴ�.

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_IeditNum.put_Value("");
}

void CDlgNumPad::OnDataEnterCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	EndDialog(IDCANCEL);
}

void CDlgNumPad::OnEditDataEnter()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	m_strEditText = m_IeditNum.get_Value();
	EndDialog(IDOK);
}
// CDlgNumPad �޽��� ó�����Դϴ�.
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
