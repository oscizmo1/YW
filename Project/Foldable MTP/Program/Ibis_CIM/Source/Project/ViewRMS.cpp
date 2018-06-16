// ViewRMSnECN.cpp : implementation file
//

#include "stdafx.h"
#include "GaonNuri.h"
#include "ViewRMS.h"
//#include "CimClass/DataStructure.h"

// CViewRMS

IMPLEMENT_DYNCREATE(CViewRMS, CFormView)

CViewRMS::CViewRMS()
: CFormView(CViewRMS::IDD)
{

}

CViewRMS::~CViewRMS()
{
}

void CViewRMS::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MODEL, m_cbModel);

	DDX_Control(pDX, IDC_LIST_RECIPE, m_ListRecipe);
}

BEGIN_MESSAGE_MAP(CViewRMS, CFormView)
	ON_CBN_SELCHANGE(IDC_COMBO_MODEL, &CViewRMS::OnCbnSelchangeComboModel)
	ON_CBN_DROPDOWN(IDC_COMBO_MODEL, &CViewRMS::OnCbnDropdownComboModel)
END_MESSAGE_MAP()


// CViewRMS diagnostics

#ifdef _DEBUG
void CViewRMS::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewRMS::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewRMS message handlers

BOOL CViewRMS::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if((pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4) || pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		return TRUE;

	return CFormView::PreTranslateMessage(pMsg);
}


void CViewRMS::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	//////////////////////////////////////////////////////////////////////////
	// Model 취득
	GetModel();

	//////////////////////////////////////////////////////////////////////////
	// REPORT LIST 설정
	m_ListRecipe.SetExtendedStyle(m_ListRecipe.GetExtendedStyle() | LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	LV_COLUMN tempColumn;
	tempColumn.mask = LVCF_FMT|LVCF_TEXT|LVCF_WIDTH;
	tempColumn.fmt = LVCFMT_LEFT;

	tempColumn.cx = 240;
	tempColumn.pszText = "PARAM_NAME";
	m_ListRecipe.InsertColumn(0,&tempColumn);

	tempColumn.cx = 270;
	tempColumn.pszText = "PARAM_VALUE";
	m_ListRecipe.InsertColumn(1,&tempColumn);

}



BOOL CViewRMS::GetModel()
{

	CString sModel;

	int nCnt;
	nCnt = m_cbModel.GetCount();

	for (int i=0; i<nCnt; i++)
	{
		m_cbModel.GetLBText(0, sModel);
		m_cbModel.DeleteString(0);
	}

	if(tag_Recipe_Validation::LoadRcpList() < 0)
	{
		AfxMessageBox("Load Recipe List Fail");
		return FALSE;
	}

	POSITION pos;
	pos = tag_Recipe_Validation::m_lstRcp.GetHeadPosition();

	while(pos)
	{
		m_cbModel.AddString(tag_Recipe_Validation::m_lstRcp.GetNext(pos));
	}

	return TRUE;
}


BOOL CViewRMS::EqModelChange(char* pPPID)
{
	CString strModel;
	GetModel();
	int nCnt = m_cbModel.GetCount();

	for (int i=0; i<nCnt; i++)
	{
		m_cbModel.GetLBText(i, strModel);
		if (strcmp(pPPID, strModel)==0)
		{
			m_cbModel.SetCurSel(i);
			GetListLoadRecipeParam();
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CViewRMS::EqModelSet(char* pFixPPID, char* pFixPPIDMODE)
{
	CString strModel;
	GetModel();
	int nCnt = m_cbModel.GetCount();

	if (strcmp(pFixPPIDMODE, PPID_MODE_CREATE)==0 || strcmp(pFixPPIDMODE, PPID_MODE_MODIFY)==0)
	{
		for (int i=0; i<nCnt; i++)
		{
			m_cbModel.GetLBText(i, strModel);
			if (strcmp(pFixPPID, strModel)==0)
			{
				m_cbModel.SetCurSel(i);
				GetListLoadRecipeParam();
				return TRUE;
			}
		}
	}
	else if (strcmp(pFixPPIDMODE, PPID_MODE_DELETE)==0)
	{
		if (nCnt>0)
		{
			m_cbModel.SetCurSel(0);
			GetListLoadRecipeParam();
			return TRUE;
		}
	}

	return FALSE;
}


CString CViewRMS::MakeParaString(CString str, BOOL bUpper)
{
	int nLen=0;
	int nBreak=0;

	if (nLen <= 0 ) return str;			// 비어있다면 처리할 내용이 없다.

	while (str.GetAt(0) == 0x20 || str.GetAt(str.GetLength()-1) == 0x20
		|| str.GetAt(0) == '\t' || str.GetAt(str.GetLength()-1) == '\t'
		|| nBreak>=10)
	{
		str.Delete(str.Find("\t"));
		str.Trim(0x20);

		nBreak++;

		if ( nBreak >= nLen ) break;		// 가지고 있는 문자열 이상으로 처리하지 못하게 한다.
	}

	str.Replace(" ", "_");
	if (bUpper)
		str.MakeUpper();

	return str;
}



//////////////////////////////////////////////////////////////////////////
// Click Event

void CViewRMS::OnCbnSelchangeComboModel()
{
	// TODO: Add your control notification handler code here
	GetListLoadRecipeParam();
}

BOOL CViewRMS::GetListLoadRecipeParam()
{

	tag_Recipe_Data		Param;

	int iSel;
	CString strModel;
	char cMode[2];
	LVITEM lvItem;
	CString str;
	CString varValue;

	m_ListRecipe.DeleteAllItems();

	strcpy_s(cMode, PPID_MODE_CREATE);

	iSel = m_cbModel.GetCurSel();
	if (iSel > -1)
	{
		m_cbModel.GetLBText(iSel, strModel);
	}
	else
		return FALSE;
	tag_Recipe_Validation::GetRcpParam((LPSTR)(LPCTSTR)strModel, &Param, cMode);

	if (Param.nDataCount>0)
	{
		for (int i=0; i<Param.nDataCount; i++)
		{
			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = m_ListRecipe.GetItemCount();
			lvItem.pszText = (LPSTR)(LPCTSTR)varValue;
			varValue = (LPSTR)(LPCTSTR)MakeParaString((CString)Param.cDataName[i], TRUE);
			lvItem.pszText = (LPSTR)(LPCTSTR)varValue;
			lvItem.iSubItem 	= 0;
			m_ListRecipe.InsertItem(&lvItem);	
			varValue = (LPSTR)(LPCTSTR)MakeParaString((CString)Param.cDataValue[i], FALSE);
			m_ListRecipe.SetItemText(lvItem.iItem,1,varValue);
		}
	}
	else
	{
		str.Format("There is no Model file(%s)", strModel);
		AfxMessageBox(str);
		return FALSE;
	}

	return TRUE;
}

void CViewRMS::OnCbnDropdownComboModel()
{
	// TODO: Add your control notification handler code here
}

void CViewRMS::OnBnClickedBtnEcReload()
{

}

