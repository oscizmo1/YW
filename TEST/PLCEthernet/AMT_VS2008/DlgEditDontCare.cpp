// DlgEditDontCare.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "DlgEditDontCare.h"


// CDlgEditDontCare dialog

IMPLEMENT_DYNAMIC(CDlgEditDontCare, CDialog)

static int ThicknessValue[ThicknessMax] = {3, 5, 10, 20};

CDlgEditDontCare::CDlgEditDontCare(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEditDontCare::IDD, pParent)
	, m_nThickness(0)
	, m_nMaskState(0)
{
	SetModelIndex(0);
	m_bLButtonDown = FALSE;
}

CDlgEditDontCare::~CDlgEditDontCare()
{
}

void CDlgEditDontCare::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_THICK1, m_nThickness);
	DDX_Radio(pDX, IDC_RADIO_MASK, m_nMaskState);
	DDX_Control(pDX, IDC_STATIC_DONTCARE, m_stcDontcare);
}


BEGIN_MESSAGE_MAP(CDlgEditDontCare, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgEditDontCare::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgEditDontCare::OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_RADIO_THICK1, &CDlgEditDontCare::OnBnClickedRadioThick1)
	ON_BN_CLICKED(vIDC_RADIO_THICK2, &CDlgEditDontCare::OnBnClickedRadioThick2)
	ON_BN_CLICKED(IDC_RADIO_THICK3, &CDlgEditDontCare::OnBnClickedRadioThick3)
	ON_BN_CLICKED(IDC_RADIO_THICK4, &CDlgEditDontCare::OnBnClickedRadioThick4)
	ON_BN_CLICKED(IDC_RADIO_MASK, &CDlgEditDontCare::OnBnClickedRadioMask)
	ON_BN_CLICKED(IDC_RADIO_UNMASK, &CDlgEditDontCare::OnBnClickedRadioUnmask)
	ON_BN_CLICKED(IDC_BTN_RESET, &CDlgEditDontCare::OnBnClickedBtnReset)
END_MESSAGE_MAP()


// CDlgEditDontCare message handlers

void CDlgEditDontCare::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_pVision->ApplyDontcare(m_nModelIndex);
	CString strPtrPath;
	strPtrPath.Format("%s\\%s\\%s.mod", GetApp()->m_strRecipePath , GetDoc()->m_pAmtData->m_CurModelInfo.strModelName, 
		GetAlignCtl()->GeneratePatternFileName(m_nModelIndex));
	m_pVision->SavePattern(m_nModelIndex, strPtrPath);
	OnOK();
}

void CDlgEditDontCare::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

BOOL CDlgEditDontCare::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_pVision = GetAlignCtl()->GetVision();
	m_nThickness = Thickness10;
	m_nMaskState = MaskState;

	CRect rect;
	rect = GetDoc()->m_pAmtData->m_CurModelInfo.rtPattern[m_nModelIndex] ;
	m_stcDontcare.GetWindowRect(&m_rtImage);
	ScreenToClient(&m_rtImage);
	CRect newRect(m_rtImage.left, m_rtImage.top, m_rtImage.left + (rect.Width()*(int)DONTCARE_SCALEFACTOR), m_rtImage.top + (rect.Height()*(INT)DONTCARE_SCALEFACTOR));
	m_rtImage = newRect;
	m_stcDontcare.MoveWindow(m_rtImage);
	m_OldPoint = CPoint(1000,1000);

	ASSERT(m_pVision->SelDontcareWindow(m_stcDontcare.m_hWnd));
	m_pVision->LoadDontcare(m_nModelIndex);
// 	m_pVision->DrawDontcareBrush(ThicknessValue[m_nThickness], CPoint(0, 0));

	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEditDontCare::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	m_pVision->DeselDontcareWindow();
	CDialog::OnClose();
}

void CDlgEditDontCare::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_rtImage.PtInRect(point))
	{
		m_bLButtonDown = TRUE;
}
	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgEditDontCare::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_rtImage.PtInRect(point) && m_bLButtonDown)
	{
		CPoint ptCenter = CPoint(point.x-m_rtImage.left, point.y-m_rtImage.top);
		m_pVision->MovingDontcareBrush(m_nMaskState, ThicknessValue[m_nThickness], ptCenter, m_OldPoint);
		m_OldPoint = ptCenter;
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgEditDontCare::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bLButtonDown = FALSE;
	m_OldPoint = CPoint(1000,1000);

	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgEditDontCare::OnBnClickedRadioThick1()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_pVision->ResizeDontcareBrush(ThicknessValue[m_nThickness], m_OldPoint);
}

void CDlgEditDontCare::OnBnClickedRadioThick2()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_pVision->ResizeDontcareBrush(ThicknessValue[m_nThickness], m_OldPoint);
}

void CDlgEditDontCare::OnBnClickedRadioThick3()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_pVision->ResizeDontcareBrush(ThicknessValue[m_nThickness], m_OldPoint);
}

void CDlgEditDontCare::OnBnClickedRadioThick4()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_pVision->ResizeDontcareBrush(ThicknessValue[m_nThickness], m_OldPoint);
}

void CDlgEditDontCare::OnBnClickedRadioMask()
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CDlgEditDontCare::OnBnClickedRadioUnmask()
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CDlgEditDontCare::OnBnClickedBtnReset()
{
	// TODO: Add your control notification handler code here
	m_pVision->ClearOverlayDontcare();
}
