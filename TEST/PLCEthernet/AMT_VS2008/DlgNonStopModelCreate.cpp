// DlgNonStopModelCreate.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "DlgNonStopModelCreate.h"
#include "DlgKeyBoard.h"

using namespace Gdiplus;

enum	_FPCType		{ indexFFC_TYPE , indexFPC_TYPE};
enum	_PinNum		{ index68Pin , index80Pin , index96Pin};
enum	_ContactNum	{ indexONE_CONT , indexTWO_CONT, indexFOUR_CONT };
enum	_Contactor		{ indexCONTACT_0 , indexCONTACT_1 , indexCONTACT_2, indexCONTACT_3};

// CDlgNonStopModelCreate dialog

IMPLEMENT_DYNAMIC(CDlgNonStopModelCreate, CDialog)

CDlgNonStopModelCreate::CDlgNonStopModelCreate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNonStopModelCreate::IDD, pParent)
{
}

CDlgNonStopModelCreate::~CDlgNonStopModelCreate()
{
}

void CDlgNonStopModelCreate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LBL_FPC_TYPE, m_lblFPCType);
	DDX_Control(pDX, IDC_LBL_PIN_NUM, m_lblPinNum);
	DDX_Control(pDX, IDC_LBL_CONTACT_NUM, m_lblContactNum);
	DDX_Control(pDX, IDC_LBL_CONTACTOR, m_lblContactor);
	DDX_Control(pDX, IDC_ILABELX_COMMENT, m_lblComment);
	DDX_Control(pDX, IDC_PREV_FPC_TYPE, m_lblPrevFPCType);
	DDX_Control(pDX, IDC_PREV_PIN_NUM, m_lblPrevPinNum);
	DDX_Control(pDX, IDC_PREV_CONTACT_NUM, m_lblPrevContactNum);
	DDX_Control(pDX, IDC_PREV_CONTACTOR, m_lblPrevContactor);
	DDX_Control(pDX, IDC_NEXT_FPC_TYPE, m_lblNextFPCType);
	DDX_Control(pDX, IDC_NEXT_PIN_NUM, m_lblNextPinNum);
	DDX_Control(pDX, IDC_NEXT_CONTACT_NUM, m_lblNextContactNum);
	DDX_Control(pDX, IDC_NEXT_CONTACTOR, m_lblNextContactor);
}


BEGIN_MESSAGE_MAP(CDlgNonStopModelCreate, CDialog)
	ON_BN_CLICKED(IDC_BTN_CREATE, &CDlgNonStopModelCreate::OnBnClickedBtnCreate)
	ON_BN_CLICKED(IDC_BTN_CANCEL1, &CDlgNonStopModelCreate::OnBnClickedBtnCancel1)
END_MESSAGE_MAP()


// CDlgNonStopModelCreate message handlers

BOOL CDlgNonStopModelCreate::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitDialogText();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNonStopModelCreate::OnBnClickedBtnCreate()
{
	OnOK();
}

void CDlgNonStopModelCreate::OnBnClickedBtnCancel1()
{
	OnCancel();
}

BEGIN_EVENTSINK_MAP(CDlgNonStopModelCreate, CDialog)
	ON_EVENT(CDlgNonStopModelCreate, IDC_PREV_FPC_TYPE, 2, CDlgNonStopModelCreate::OnClickPrevFpcType, VTS_NONE)
	ON_EVENT(CDlgNonStopModelCreate, IDC_PREV_PIN_NUM, 2, CDlgNonStopModelCreate::OnClickPrevPinNum, VTS_NONE)
	ON_EVENT(CDlgNonStopModelCreate, IDC_PREV_CONTACT_NUM, 2, CDlgNonStopModelCreate::OnClickPrevContactNum, VTS_NONE)
	ON_EVENT(CDlgNonStopModelCreate, IDC_PREV_CONTACTOR, 2, CDlgNonStopModelCreate::OnClickPrevContactor, VTS_NONE)
	ON_EVENT(CDlgNonStopModelCreate, IDC_NEXT_FPC_TYPE, 2, CDlgNonStopModelCreate::OnClickNextFpcType, VTS_NONE)
	ON_EVENT(CDlgNonStopModelCreate, IDC_NEXT_PIN_NUM, 2, CDlgNonStopModelCreate::OnClickNextPinNum, VTS_NONE)
	ON_EVENT(CDlgNonStopModelCreate, IDC_NEXT_CONTACT_NUM, 2, CDlgNonStopModelCreate::OnClickNextContactNum, VTS_NONE)
	ON_EVENT(CDlgNonStopModelCreate, IDC_NEXT_CONTACTOR, 2, CDlgNonStopModelCreate::OnClickNextContactor, VTS_NONE)
	ON_EVENT(CDlgNonStopModelCreate, IDC_PREV_FPC_TYPE, 6, CDlgNonStopModelCreate::OnMouseDownPrevFpcType, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgNonStopModelCreate, IDC_PREV_PIN_NUM, 6, CDlgNonStopModelCreate::OnMouseDownPrevPinNum, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgNonStopModelCreate, IDC_PREV_CONTACT_NUM, 6, CDlgNonStopModelCreate::OnMouseDownPrevContactNum, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgNonStopModelCreate, IDC_PREV_CONTACTOR, 6, CDlgNonStopModelCreate::OnMouseDownPrevContactor, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgNonStopModelCreate, IDC_NEXT_FPC_TYPE, 6, CDlgNonStopModelCreate::OnMouseDownNextFpcType, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgNonStopModelCreate, IDC_NEXT_PIN_NUM, 6, CDlgNonStopModelCreate::OnMouseDownNextPinNum, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgNonStopModelCreate, IDC_NEXT_CONTACT_NUM, 6, CDlgNonStopModelCreate::OnMouseDownNextContactNum, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgNonStopModelCreate, IDC_NEXT_CONTACTOR, 6, CDlgNonStopModelCreate::OnMouseDownNextContactor, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgNonStopModelCreate, IDC_PREV_FPC_TYPE, 8, CDlgNonStopModelCreate::OnMouseUpPrevFpcType, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgNonStopModelCreate, IDC_PREV_PIN_NUM, 8, CDlgNonStopModelCreate::OnMouseUpPrevPinNum, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgNonStopModelCreate, IDC_PREV_CONTACT_NUM, 8, CDlgNonStopModelCreate::OnMouseUpPrevContactNum, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgNonStopModelCreate, IDC_PREV_CONTACTOR, 8, CDlgNonStopModelCreate::OnMouseUpPrevContactor, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgNonStopModelCreate, IDC_NEXT_FPC_TYPE, 8, CDlgNonStopModelCreate::OnMouseUpNextFpcType, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgNonStopModelCreate, IDC_NEXT_PIN_NUM, 8, CDlgNonStopModelCreate::OnMouseUpNextPinNum, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgNonStopModelCreate, IDC_NEXT_CONTACT_NUM, 8, CDlgNonStopModelCreate::OnMouseUpNextContactNum, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgNonStopModelCreate, IDC_NEXT_CONTACTOR, 8, CDlgNonStopModelCreate::OnMouseUpNextContactor, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgNonStopModelCreate, IDC_ILABELX_COMMENT, 1, CDlgNonStopModelCreate::OnClickIlabelxComment, VTS_NONE)
END_EVENTSINK_MAP()

void CDlgNonStopModelCreate::OnClickPrevFpcType()
{
	ChangeFPCType(FALSE);
}

void CDlgNonStopModelCreate::OnClickPrevPinNum()
{
	ChangePinNum(FALSE);
}

void CDlgNonStopModelCreate::OnClickPrevContactNum()
{
	ChangeContactNum(FALSE);
}

void CDlgNonStopModelCreate::OnClickPrevContactor()
{
	ChangeContactor(FALSE);
}

void CDlgNonStopModelCreate::OnClickNextFpcType()
{
	ChangeFPCType(TRUE);
}

void CDlgNonStopModelCreate::OnClickNextPinNum()
{
	ChangePinNum(TRUE);
}

void CDlgNonStopModelCreate::OnClickNextContactNum()
{
	ChangeContactNum(TRUE);
}

void CDlgNonStopModelCreate::OnClickNextContactor()
{
	ChangeContactor(TRUE);
}

void CDlgNonStopModelCreate::ChangeFPCType( BOOL bNext )
{
	if(bNext)		m_nFPCType++;
	else		m_nFPCType--;

	if(m_nFPCType < 0)	m_nFPCType = 1;
	if(m_nFPCType > 1)	m_nFPCType = 0;

	m_lblFPCType.put_Caption(m_szFPCType[m_nFPCType]);
}

void CDlgNonStopModelCreate::ChangePinNum( BOOL bNext )
{
	if(bNext)		m_nPinNum++;
	else		m_nPinNum--;

	if(m_nPinNum < 0)	m_nPinNum = 2;
	if(m_nPinNum > 2)	m_nPinNum = 0;

	m_lblPinNum.put_Caption(m_szPinNum[m_nPinNum]);
}

void CDlgNonStopModelCreate::ChangeContactNum( BOOL bNext )
{
	if(bNext)		m_nContactNum++;
	else		m_nContactNum--;

	if(m_nContactNum < 0)	m_nContactNum = 2;
	if(m_nContactNum > 2)	m_nContactNum = 0;

	m_lblContactNum.put_Caption(m_szContactNum[m_nContactNum]);
}

void CDlgNonStopModelCreate::ChangeContactor( BOOL bNext )
{
	if(bNext)		m_nContactor++;
	else		m_nContactor--;

	if(m_nContactor < 0)	m_nContactor = 3;
	if(m_nContactor > 3)	m_nContactor = 0;

	m_lblContactor.put_Caption(m_szContactor[m_nContactor]);
}

void CDlgNonStopModelCreate::InitDialogText()
{
	m_szFPCType[0] = _T("FFC");
	m_szFPCType[1] = _T("FPC");
	m_szPinNum[0] = _T("68Pin");
	m_szPinNum[1] = _T("80Pin");
	m_szPinNum[2] = _T("96Pin");
	m_szContactNum[0] = _T("1Contact");
	m_szContactNum[1] = _T("2Contact");
	m_szContactNum[2] = _T("4Contact");
	m_szContactor[0] = _T("1st Contactor");
	m_szContactor[1] = _T("2nd Contactor");
	m_szContactor[2] = _T("3rd Contactor");
	m_szContactor[3] = _T("4th Contactor");

	m_szComment = _T("NORMAL");
	m_nFPCType = 0;
	m_nPinNum = 0;
	m_nContactNum = 0;
	m_nContactor = 0;

	m_lblFPCType.put_Caption(m_szFPCType[m_nFPCType]);
	m_lblPinNum.put_Caption(m_szPinNum[m_nPinNum]);
	m_lblContactNum.put_Caption(m_szContactNum[m_nContactNum]);
	m_lblContactor.put_Caption(m_szContactor[m_nContactor]);
	m_lblComment.put_Caption(m_szComment);
}
void CDlgNonStopModelCreate::OnMouseDownPrevFpcType(short Button, short Shift, long X, long Y)
{
	m_lblPrevFPCType.put_Active(TRUE);
}

void CDlgNonStopModelCreate::OnMouseDownPrevPinNum(short Button, short Shift, long X, long Y)
{
	m_lblPrevPinNum.put_Active(TRUE);
}

void CDlgNonStopModelCreate::OnMouseDownPrevContactNum(short Button, short Shift, long X, long Y)
{
	m_lblPrevContactNum.put_Active(TRUE);
}

void CDlgNonStopModelCreate::OnMouseDownPrevContactor(short Button, short Shift, long X, long Y)
{
	m_lblPrevContactor.put_Active(TRUE);
}

void CDlgNonStopModelCreate::OnMouseDownNextFpcType(short Button, short Shift, long X, long Y)
{
	m_lblNextFPCType.put_Active(TRUE);
}

void CDlgNonStopModelCreate::OnMouseDownNextPinNum(short Button, short Shift, long X, long Y)
{
	m_lblNextPinNum.put_Active(TRUE);
}

void CDlgNonStopModelCreate::OnMouseDownNextContactNum(short Button, short Shift, long X, long Y)
{
	m_lblNextContactNum.put_Active(TRUE);
}

void CDlgNonStopModelCreate::OnMouseDownNextContactor(short Button, short Shift, long X, long Y)
{
	m_lblNextContactor.put_Active(TRUE);
}

void CDlgNonStopModelCreate::OnMouseUpPrevFpcType(short Button, short Shift, long X, long Y)
{
	m_lblPrevFPCType.put_Active(FALSE);
}

void CDlgNonStopModelCreate::OnMouseUpPrevPinNum(short Button, short Shift, long X, long Y)
{
	m_lblPrevPinNum.put_Active(FALSE);
}

void CDlgNonStopModelCreate::OnMouseUpPrevContactNum(short Button, short Shift, long X, long Y)
{
	m_lblPrevContactNum.put_Active(FALSE);
}

void CDlgNonStopModelCreate::OnMouseUpPrevContactor(short Button, short Shift, long X, long Y)
{
	m_lblPrevContactor.put_Active(FALSE);
}

void CDlgNonStopModelCreate::OnMouseUpNextFpcType(short Button, short Shift, long X, long Y)
{
	m_lblNextFPCType.put_Active(FALSE);
}

void CDlgNonStopModelCreate::OnMouseUpNextPinNum(short Button, short Shift, long X, long Y)
{
	m_lblNextPinNum.put_Active(FALSE);
}

void CDlgNonStopModelCreate::OnMouseUpNextContactNum(short Button, short Shift, long X, long Y)
{
	m_lblNextContactNum.put_Active(FALSE);
}

void CDlgNonStopModelCreate::OnMouseUpNextContactor(short Button, short Shift, long X, long Y)
{
	m_lblNextContactor.put_Active(FALSE);
}

void CDlgNonStopModelCreate::OnClickIlabelxComment()
{
	CDlgKeyBoard dlg;
	dlg.DoModal();
	m_szComment = dlg.m_strEditText;
	m_lblComment.put_Caption(m_szComment);
}
