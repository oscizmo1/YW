// DlgBoxEDIT.cpp : implementation file
//

#include "stdafx.h"
//#include "IbisApp.h"
#include "DlgBoxEDIT.h"
#include "MainFrm.h"



// CDlgBoxEdit dialog

IMPLEMENT_DYNAMIC(CDlgBoxEdit, CDialog)

CDlgBoxEdit::CDlgBoxEdit(CWnd* pParent /*=NULL*/, CString strCaption, CString strValue)
	: CDialog(CDlgBoxEdit::IDD, pParent)
{
	m_strCaption	= strCaption;
	m_strValue		= strValue;
}

CDlgBoxEdit::~CDlgBoxEdit()
{
}

void CDlgBoxEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STA_NAME,		m_EditNum);
	DDX_Control(pDX, IDC_BTN_CLEAR,		m_btnClear);
	DDX_Control(pDX, IDC_BTN_CANCEL,	m_btnCancel);
	DDX_Control(pDX, IDC_BTN_ENTER,		m_btnEnter);

	DDX_Control(pDX, IDC_STA_NAME2, m_Count);
}


BEGIN_MESSAGE_MAP(CDlgBoxEdit, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgBoxEdit::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgBoxEdit::OnBnClickedCancel)
	
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CDlgBoxEdit::OnBnClkClear)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgBoxEdit::OnBnClkCancel)
	ON_BN_CLICKED(IDC_BTN_ENTER, &CDlgBoxEdit::OnBnClkEnter)
END_MESSAGE_MAP()


// CDlgBoxEdit message handlers
BOOL CDlgBoxEdit::OnInitDialog()
{
	CDialog::OnInitDialog();

	LOGFONT lf;
	GetFont()->GetLogFont (&lf);

	lf.lfWeight = FW_BOLD;
	lf.lfHeight = 25;
	m_fontBold.CreateFontIndirect (&lf);

	m_EditNum.SetFont (&m_fontBold);
	m_Count.SetFont (&m_fontBold);
	m_btnClear.SetFont (&m_fontBold);
	m_btnCancel.SetFont (&m_fontBold);
	m_btnEnter.SetFont (&m_fontBold);

	// TODO:  Add extra initialization here
	SetWindowText(m_strCaption);


	m_EditNum.SetWindowText(m_strCaption);
	m_Count.SetWindowText(m_strValue);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgBoxEdit::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	OnBnClkEnter();
}

void CDlgBoxEdit::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//OnCancel();
	OnBnClkCancel();
}
void CDlgBoxEdit::OnBnClkClear()
{
	m_strValue = _T("");
	m_EditNum.SetWindowText(m_strValue);
	m_Count.SetWindowText(m_strValue);
}
void CDlgBoxEdit::OnBnClkCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
void CDlgBoxEdit::OnBnClkEnter()
{
	// TODO: Add your control notification handler code here
	CString strValue,countValue;
	
	m_EditNum.GetWindowText(strValue);
	m_Count.GetWindowText(countValue);

	//if(m_strValue=="")
	if(strValue == "")
	{
		
// 		g_pMainFrm->MsgBox(MSG_ETC[CONST_MSG_ETC::W_NO_VAL].strName,_T("경고(警告)"),MB_OK);
		return ;
	}

	//if(m_bNotUsePoint && strValue.Find('.')>=0)
	//{
 //		g_pMainFrm->MsgBox(MSG_ETC[CONST_MSG_ETC::W_NO_TYPE].strName,_T("경고(警告)"),MB_OK);
	//	return ;		
	//}

	m_strReturnText	=	strValue;

	m_strReturnCount	=	countValue;

	OnOK();
}