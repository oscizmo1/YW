// frmCIM.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "frmCIM.h"


extern void Log(LPCSTR format, int iLogID);
// CFormCIM

IMPLEMENT_DYNCREATE(CFormCIM, CFormView)

CFormCIM::CFormCIM()
	: CFormView(CFormCIM::IDD)
{
	for( int i = 0 ; i < 31 ; i++ )
	{
		m_bError[i] = FALSE;
		m_bHold[i] = FALSE;
	}
}

CFormCIM::~CFormCIM()
{
}

void CFormCIM::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	for( int i = 0 ; i < 31 ; i++ )
	{
		DDX_Control(pDX, IDC_CHK_CIM_ERR1 + i, m_chkError[i]);
		DDX_Control(pDX, IDC_CHK_CIM_HOLD1 + i, m_chkHold[i]);
	}
}

BEGIN_MESSAGE_MAP(CFormCIM, CFormView)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR1, &CFormCIM::OnChkErr1)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR2, &CFormCIM::OnChkErr2)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR3, &CFormCIM::OnChkErr3)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR4, &CFormCIM::OnChkErr4)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR5, &CFormCIM::OnChkErr5)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR6, &CFormCIM::OnChkErr6)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR7, &CFormCIM::OnChkErr7)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR8, &CFormCIM::OnChkErr8)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR9, &CFormCIM::OnChkErr9)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR10, &CFormCIM::OnChkErr10)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR11, &CFormCIM::OnChkErr11)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR12, &CFormCIM::OnChkErr12)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR13, &CFormCIM::OnChkErr13)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR14, &CFormCIM::OnChkErr14)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR15, &CFormCIM::OnChkErr15)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR16, &CFormCIM::OnChkErr16)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR17, &CFormCIM::OnChkErr17)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR18, &CFormCIM::OnChkErr18)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR19, &CFormCIM::OnChkErr19)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR20, &CFormCIM::OnChkErr20)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR21, &CFormCIM::OnChkErr21)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR22, &CFormCIM::OnChkErr22)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR23, &CFormCIM::OnChkErr23)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR24, &CFormCIM::OnChkErr24)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR25, &CFormCIM::OnChkErr25)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR26, &CFormCIM::OnChkErr26)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR27, &CFormCIM::OnChkErr27)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR28, &CFormCIM::OnChkErr28)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR29, &CFormCIM::OnChkErr29)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR30, &CFormCIM::OnChkErr30)
	ON_BN_CLICKED(IDC_CHK_CIM_ERR31, &CFormCIM::OnChkErr31)

	ON_BN_CLICKED(IDC_CHK_CIM_HOLD1, &CFormCIM::OnChkHold1)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD2, &CFormCIM::OnChkHold2)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD3, &CFormCIM::OnChkHold3)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD4, &CFormCIM::OnChkHold4)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD5, &CFormCIM::OnChkHold5)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD6, &CFormCIM::OnChkHold6)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD7, &CFormCIM::OnChkHold7)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD8, &CFormCIM::OnChkHold8)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD9, &CFormCIM::OnChkHold9)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD10, &CFormCIM::OnChkHold10)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD11, &CFormCIM::OnChkHold11)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD12, &CFormCIM::OnChkHold12)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD13, &CFormCIM::OnChkHold13)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD14, &CFormCIM::OnChkHold14)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD15, &CFormCIM::OnChkHold15)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD16, &CFormCIM::OnChkHold16)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD17, &CFormCIM::OnChkHold17)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD18, &CFormCIM::OnChkHold18)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD19, &CFormCIM::OnChkHold19)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD20, &CFormCIM::OnChkHold20)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD21, &CFormCIM::OnChkHold21)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD22, &CFormCIM::OnChkHold22)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD23, &CFormCIM::OnChkHold23)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD24, &CFormCIM::OnChkHold24)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD25, &CFormCIM::OnChkHold25)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD26, &CFormCIM::OnChkHold26)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD27, &CFormCIM::OnChkHold27)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD28, &CFormCIM::OnChkHold28)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD29, &CFormCIM::OnChkHold29)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD30, &CFormCIM::OnChkHold30)
	ON_BN_CLICKED(IDC_CHK_CIM_HOLD31, &CFormCIM::OnChkHold31)
END_MESSAGE_MAP()


// CFormCIM diagnostics

#ifdef _DEBUG
void CFormCIM::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormCIM::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormCIM message handlers

BOOL CFormCIM::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CFormCIM::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	for( int i = 0 ; i < 31 ; i++ )
	{
		m_chkError[i].SetCheck(m_bError[i]);
		m_chkHold[i].SetCheck(m_bHold[i]);
	}
}
BEGIN_EVENTSINK_MAP(CFormCIM, CFormView)
	ON_EVENT(CFormCIM, IDC_LABEL_CIM_SET_ERRNUM, DISPID_CLICK, CFormCIM::OnClickErrNumSet, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_LABEL_CIM_CNT_ERRNUM, DISPID_CLICK, CFormCIM::OnClickErrNumCnt, VTS_NONE)
END_EVENTSINK_MAP()

void CFormCIM::OnClickErrNumSet()
{
	// TODO: Add your message handler code here
}

void CFormCIM::OnClickErrNumCnt()
{
	// TODO: Add your message handler code here
}

void CFormCIM::OnChkErr1(){SetDeffectCode(0);}
void CFormCIM::OnChkErr2(){SetDeffectCode(1);}
void CFormCIM::OnChkErr3(){SetDeffectCode(2);}
void CFormCIM::OnChkErr4(){SetDeffectCode(3);}
void CFormCIM::OnChkErr5(){SetDeffectCode(4);}
void CFormCIM::OnChkErr6(){SetDeffectCode(5);}
void CFormCIM::OnChkErr7(){SetDeffectCode(6);}
void CFormCIM::OnChkErr8(){SetDeffectCode(7);}
void CFormCIM::OnChkErr9(){SetDeffectCode(8);}
void CFormCIM::OnChkErr10(){SetDeffectCode(9);}
void CFormCIM::OnChkErr11(){SetDeffectCode(10);}
void CFormCIM::OnChkErr12(){SetDeffectCode(11);}
void CFormCIM::OnChkErr13(){SetDeffectCode(12);}
void CFormCIM::OnChkErr14(){SetDeffectCode(13);}
void CFormCIM::OnChkErr15(){SetDeffectCode(14);}
void CFormCIM::OnChkErr16(){SetDeffectCode(15);}
void CFormCIM::OnChkErr17(){SetDeffectCode(16);}
void CFormCIM::OnChkErr18(){SetDeffectCode(17);}
void CFormCIM::OnChkErr19(){SetDeffectCode(18);}
void CFormCIM::OnChkErr20(){SetDeffectCode(19);}
void CFormCIM::OnChkErr21(){SetDeffectCode(20);}
void CFormCIM::OnChkErr22(){SetDeffectCode(21);}
void CFormCIM::OnChkErr23(){SetDeffectCode(22);}
void CFormCIM::OnChkErr24(){SetDeffectCode(23);}
void CFormCIM::OnChkErr25(){SetDeffectCode(24);}
void CFormCIM::OnChkErr26(){SetDeffectCode(25);}
void CFormCIM::OnChkErr27(){SetDeffectCode(26);}
void CFormCIM::OnChkErr28(){SetDeffectCode(27);}
void CFormCIM::OnChkErr29(){SetDeffectCode(28);}
void CFormCIM::OnChkErr30(){SetDeffectCode(29);}
void CFormCIM::OnChkErr31(){SetDeffectCode(30);}
void CFormCIM::SetDeffectCode(int nErr)
{
	// nErr 코드로 저장.
	CString strLog;
	m_bError[nErr] = m_chkError[nErr].GetCheck();
	strLog.Format("m_bError[%d] = %d" , nErr , m_bError[nErr]);
	Log(strLog , LOG_EQ);
}

void CFormCIM::OnChkHold1(){SetHoldCode(0);}
void CFormCIM::OnChkHold2(){SetHoldCode(1);}
void CFormCIM::OnChkHold3(){SetHoldCode(2);}
void CFormCIM::OnChkHold4(){SetHoldCode(3);}
void CFormCIM::OnChkHold5(){SetHoldCode(4);}
void CFormCIM::OnChkHold6(){SetHoldCode(5);}
void CFormCIM::OnChkHold7(){SetHoldCode(6);}
void CFormCIM::OnChkHold8(){SetHoldCode(7);}
void CFormCIM::OnChkHold9(){SetHoldCode(8);}
void CFormCIM::OnChkHold10(){SetHoldCode(9);}
void CFormCIM::OnChkHold11(){SetHoldCode(10);}
void CFormCIM::OnChkHold12(){SetHoldCode(11);}
void CFormCIM::OnChkHold13(){SetHoldCode(12);}
void CFormCIM::OnChkHold14(){SetHoldCode(13);}
void CFormCIM::OnChkHold15(){SetHoldCode(14);}
void CFormCIM::OnChkHold16(){SetHoldCode(15);}
void CFormCIM::OnChkHold17(){SetHoldCode(16);}
void CFormCIM::OnChkHold18(){SetHoldCode(17);}
void CFormCIM::OnChkHold19(){SetHoldCode(18);}
void CFormCIM::OnChkHold20(){SetHoldCode(19);}
void CFormCIM::OnChkHold21(){SetHoldCode(20);}
void CFormCIM::OnChkHold22(){SetHoldCode(21);}
void CFormCIM::OnChkHold23(){SetHoldCode(22);}
void CFormCIM::OnChkHold24(){SetHoldCode(23);}
void CFormCIM::OnChkHold25(){SetHoldCode(24);}
void CFormCIM::OnChkHold26(){SetHoldCode(25);}
void CFormCIM::OnChkHold27(){SetHoldCode(26);}
void CFormCIM::OnChkHold28(){SetHoldCode(27);}
void CFormCIM::OnChkHold29(){SetHoldCode(28);}
void CFormCIM::OnChkHold30(){SetHoldCode(29);}
void CFormCIM::OnChkHold31(){SetHoldCode(30);}
void CFormCIM::SetHoldCode(int nHold)
{
	// nHold 코드로 저장.
	CString strLog;
	m_bHold[nHold] = m_chkHold[nHold].GetCheck();
	strLog.Format("m_bError[%d] = %d" , nHold , m_bHold[nHold]);
	Log(strLog , LOG_EQ);
}