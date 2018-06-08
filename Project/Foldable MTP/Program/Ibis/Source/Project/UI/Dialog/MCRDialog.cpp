// MCRDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MCRDialog.h"
#include "afxdialogex.h"

// CMCRDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMCRDialog, CDialog)

	CMCRDialog::CMCRDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMCRDialog::IDD, pParent)
	, m_strMCRID(_T(""))
{
	m_bReadDone = FALSE;
}

CMCRDialog::~CMCRDialog()
{
}


void CMCRDialog::MkCreate(CWnd *pParent)
{
	Create(CMCRDialog::IDD, pParent);
}

void CMCRDialog::MkPos(CWnd *pWnd)
{
	CRect rtPos;

	pWnd->GetWindowRect(&rtPos);
	rtPos -= CPoint(rtPos.left, rtPos.top);

	SetWindowPos(&wndTop, rtPos.left, rtPos.top, rtPos.Width()+200, rtPos.Height()+200, SWP_SHOWWINDOW|SWP_NOMOVE); 
}


void CMCRDialog::MkDistroy()
{
	DestroyWindow();
}

void CMCRDialog::MkShow()
{
	ShowWindow(SW_SHOW);

	// 변수 초기화 [4/2/2018 LSH]
	m_strMCRID = _T("");
	m_bReadDone = FALSE;

	// Edit box에 Focus를 맞춘다 [4/2/2018 LSH]
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_MCR);
	pEdit->SetFocus();
}

void CMCRDialog::MkHide()
{
	ShowWindow(SW_HIDE);
}


void CMCRDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MCR, m_strMCRID);
}


BOOL CMCRDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}


BEGIN_MESSAGE_MAP(CMCRDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CMCRDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CMCRDialog 메시지 처리기입니다.


void CMCRDialog::OnBnClickedOk()
{
	// MCR 문자열의 길이가 16자 보다 크거나 0자보다 작을경우 Msg를 출력 [4/2/2018 LSH]
	if(m_strMCRID.GetLength() > 16 || m_strMCRID.GetLength() < 0)
	{
		CGxMsgBox dlgMsgBox;
		dlgMsgBox.SetLangName(0, _T("KOR"));
		dlgMsgBox.SetLangName(1, _T("ENG"));
		dlgMsgBox.SetLangName(2, _T("VNM"));
		dlgMsgBox.SetTitle(_T("Inform"), _T("Inform"), _T("Inform"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("Cell ID의 길이가 올바르지 않습니다."), 
			_T("The length of the Cell ID is incorrect.."), 
			_T("Độ dài của Cell ID không đúng."), 
			//theProcBank.GetLanguage() 
			GetMainHandler()->m_nLangIdx);
		dlgMsgBox.DoModal();
	}

	// 입력 완료 변수를 TRUE로 변경 [4/2/2018 LSH]
	m_bReadDone = TRUE;

	CDialog::OnOK();
}

