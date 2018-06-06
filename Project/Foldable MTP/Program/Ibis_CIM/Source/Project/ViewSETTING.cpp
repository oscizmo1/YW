// ViewSETTING.cpp : implementation file
//

#include "stdafx.h"
#include "GaonNuri.h"
#include "ViewSETTING.h"

// CViewSETTING

IMPLEMENT_DYNCREATE(CViewSETTING, CFormView)

CViewSETTING::CViewSETTING()
	: CFormView(CViewSETTING::IDD)
{

}

CViewSETTING::~CViewSETTING()
{
}

void CViewSETTING::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_LIST_SETUP, m_lstSetup);
}

BEGIN_MESSAGE_MAP(CViewSETTING, CFormView)
	
	ON_BN_CLICKED(IDC_BTN_SETUP_READ, &CViewSETTING::OnBnClickedBtnSetupRead)
	ON_BN_CLICKED(IDC_BTN_SETUP_MODIFY, &CViewSETTING::OnBnClickedBtnSetupModify)
	ON_BN_CLICKED(IDC_BTN_SETUP_SAVE, &CViewSETTING::OnBnClickedBtnSetupSave)

END_MESSAGE_MAP()


// CViewSETTING diagnostics

#ifdef _DEBUG
void CViewSETTING::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewSETTING::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewSETTING message handlers

BOOL CViewSETTING::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if((pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4) || pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		return TRUE;

	return CFormView::PreTranslateMessage(pMsg);
}

void CViewSETTING::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	bEditable = FALSE;
	GetSetup();

	
}



void CViewSETTING::GetSetup()
{
	CString settingFile,str,strSection;
	char cLine[1024];
	FILE	*fp;
	errno_t		err = 0;
	char* p0 = &cLine[0];
	char* pS, *pE;
	int nCount=0;
	CString buff;
	COLORREF clr;
	LVITEM lvItem;

	m_lstSetup.DeleteAllItems();
	m_lstSetup.SetColumnHeader(_T("SECTION,150; NAME,200; VALE,290"));
	m_lstSetup.SetGridLines(TRUE); 
	m_lstSetup.SetEditable(FALSE);
	m_lstSetup.SetSortable(TRUE);

	settingFile.Format("%s\\Setup.ini", CGaonNuriApp::GetSetupPath());
	err = fopen_s(&fp, settingFile, "r");	
	if(err != 0)
		return;


	while(fgets(p0, 1024,fp)!=NULL)
	{
		if(strchr(p0, '#')!=NULL)// 주석
			continue;
		if(strchr(p0, ';')!=NULL)// 주석
			continue;
		if(strlen(p0) < 2)// 공백
			continue;

		pS = p0;

		if((pE=strchr(pS, '='))==NULL)
		{

			CTokenizer token = pS;
			buff = token.GetNextToken("[");
			buff = token.GetNextToken("]");
			clr = RGB(rand() % 128+128, rand() % 128+128, rand() % 128+128);	//2012.07.04.

			continue;
		}
		*(pE++) = 0x00;

		pS = pE;
		if((pE=strchr(pS, '\n'))!=NULL)			*(pE++) = 0x00;
		else if((pE=strchr(pS, 0x0a))!=NULL)	*(pE++) = 0x00;
		else if((pE=strchr(pS, 0x0d))!=NULL)	*(pE++) = 0x00;
		else if((pE=strchr(pS, 0x00))!=NULL)	*(pE++) = 0x00;
		else
		{
			nCount = 0;
			break;
		}

		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = m_lstSetup.GetItemCount();
		lvItem.pszText = (LPSTR)(LPCTSTR)buff;
		lvItem.iSubItem 	= 0;
		m_lstSetup.InsertItem(&lvItem);	

		m_lstSetup.SetItemBkColor(lvItem.iItem, 0, clr, TRUE );
		m_lstSetup.SetItemBkColor(lvItem.iItem, 1, clr, TRUE );
		m_lstSetup.SetItemBkColor(lvItem.iItem, 2, clr, TRUE );

		if (p0)
		{
			m_lstSetup.SetItemText(lvItem.iItem,1,p0);	
		}

		if (pS)
		{
			m_lstSetup.SetItemText(lvItem.iItem,2,pS);	
		}

		nCount++;
	}

	fclose(fp);
}

void CViewSETTING::OnBnClickedBtnSetupRead()
{
	// TODO: Add your control notification handler code here
	GetSetup();
}

void CViewSETTING::OnBnClickedBtnSetupModify()
{
	// TODO: Add your control notification handler code here
	m_lstSetup.SetEditable(TRUE);
	bEditable = TRUE;
}

void CViewSETTING::OnBnClickedBtnSetupSave()
{
	// TODO: Add your control notification handler code here

	if (bEditable)
	{
		CString str;
		char cFileFullName[256], cFileBkFullName[256];
		CString strOldFile, strNewFile;
		sprintf_s(cFileFullName, 256, "%s\\Setup.ini", CGaonNuriApp::GetSetupPath());
		CTime t = CTime::GetCurrentTime();
		sprintf_s(cFileBkFullName, 256, "%s\\Setup_%04d%02d%02d%02d%02d%02d.ini", CGaonNuriApp::GetSetupPath(), t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond());
		::rename(cFileFullName, cFileBkFullName);

		CString settingFile,strValue,strSection,strKey;
		CIni ini(cFileFullName);

		for (int i=0; i<m_lstSetup.GetItemCount(); i++)
		{
			strSection = m_lstSetup.GetItemText(i, 0);
			strKey = m_lstSetup.GetItemText(i, 1);
			strValue = m_lstSetup.GetItemText(i, 2);
			if (strKey.GetLength() > 0)
				ini.WriteString(strSection, strKey, strValue);
		}

		GetSetup();
		((CMainFrame *)AfxGetMainWnd())->DataStructTrans();
		bEditable = FALSE;
		CSettingParm::Read(cFileFullName);
	}
	else
		AfxMessageBox("Is not editable state!");


	

}
