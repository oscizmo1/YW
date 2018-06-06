// ViewECM.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GaonNuri.h"
#include "ViewECM.h"

#include "GxGUI\GxIniFile.h"

// CViewECM

IMPLEMENT_DYNCREATE(CViewECM, CFormView)

CViewECM::CViewECM()
	: CFormView(CViewECM::IDD)
{

}

CViewECM::~CViewECM()
{
}

void CViewECM::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_ECM, m_ListECM);

}

BEGIN_MESSAGE_MAP(CViewECM, CFormView)
	ON_BN_CLICKED(IDC_BTN_ECM_RELOAD, &CViewECM::OnBnClickedBtnEcmReload)
END_MESSAGE_MAP()


// CViewECM �����Դϴ�.

#ifdef _DEBUG
void CViewECM::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewECM::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewECM �޽��� ó�����Դϴ�.


void CViewECM::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	//////////////////////////////////////////////////////////////////////////
	// ECM LIST ����
	m_ListECM.SetExtendedStyle(m_ListECM.GetExtendedStyle() | LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	//LV_COLUMN tempColumn;
	LV_COLUMN tempColumn;
	tempColumn.mask = LVCF_FMT|LVCF_TEXT|LVCF_WIDTH;
	tempColumn.fmt = LVCFMT_LEFT;

	tempColumn.cx = 40;
	tempColumn.pszText = "ECID";
	m_ListECM.InsertColumn(0,&tempColumn);

	tempColumn.cx = 250;
	tempColumn.pszText = "ECSETION";
	m_ListECM.InsertColumn(1,&tempColumn);

	tempColumn.cx = 70;
	tempColumn.pszText = "ECKEY";
	m_ListECM.InsertColumn(2,&tempColumn);

	tempColumn.cx = 100;
	tempColumn.pszText = "ECV";
	m_ListECM.InsertColumn(3,&tempColumn);

	tempColumn.cx = 250;
	tempColumn.pszText = "ECNAME";
	m_ListECM.InsertColumn(4,&tempColumn);

	//////////////////////////////////////////////////////////////////////////
	if (ReadECIDList() !=TRUE)
	{
		AfxMessageBox("There is no ECID file");
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	if(ReadECV() != TRUE)
	{
		AfxMessageBox("Search Fail : ECV Path");
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	ViewECDip();
}



CString CViewECM::MakeECString(CString str, BOOL bUpper)
{
	int nLen=str.GetLength();
	int nBreak=0;

	if (nLen <= 0 ) return str;			// ����ִٸ� ó���� ������ ����.

	while (str.GetAt(0) == 0x20 || str.GetAt(str.GetLength()-1) == 0x20
		|| str.GetAt(0) == '\t' || str.GetAt(str.GetLength()-1) == '\t'
		|| nBreak>=10)
	{
		str.Delete(str.Find("\t"));
		str.Trim(0x20);

		nBreak++;

		if ( nBreak >= nLen ) break;		// ������ �ִ� ���ڿ� �̻����� ó������ ���ϰ� �Ѵ�.
	}

	str.Replace(" ", "_");
	if (bUpper)
		str.MakeUpper();

	return str;
}



BOOL CViewECM::ReadECIDList()
{
	CGxIniFile	iniFile;
	CString sFile;
	CString sKey, sSection, sValue;					// ini ���Ͽ��� �о���̴� ����
	CString sECID, strECSection, strECKey, strECName;	// �׸� ����
	int nCount = 0;

	g_pData->m_ECID.Clear();
	sFile = CSettingParm::cECID_PATH;
	iniFile.SetIniFilename( sFile );

// [ECID_List]
// ECID_TotalCount	= 32
// ECID_1	=100,WORK_TABLE1_AXIS,CmdType

	// �׸� ����
	sSection = _T("ECID_List");
	nCount = iniFile.GetInt(sSection, _T("ECID_TotalCount"), 0);
	g_pData->m_ECID.m_nECID_TotalCount = nCount;

	for (int i = 0; i < nCount; i++)
	{
		sKey.Format(_T("ECID_%d"), i + 1);
		sValue = iniFile.GetString(sSection, sKey, _T(""));

		// ���Ͽ��� �о���� ���ڿ��� �и��Ѵ�.
		AfxExtractSubString( sECID,			sValue, 0, ',');		// ECID �и�
		AfxExtractSubString( strECSection,	sValue, 1, ',');		// Group �и�
		AfxExtractSubString( strECKey,		sValue, 2, ',');		// Key �и�
		AfxExtractSubString( strECName,		sValue, 3, ',');		// Name �и�

		// ������ ����ִٸ�,
		if ( sECID.IsEmpty() == FALSE )
		{
			strcpy_s(g_pData->m_ECID.m_cECID[i], sECID.Left( CCimECID::eMAX_LEN_ECID )); 
			strcpy_s(g_pData->m_ECID.m_cECSEC[i], strECSection.Left( CCimECID::eMAX_LEN_ECSEC ));
			strcpy_s(g_pData->m_ECID.m_cECKEY[i], strECKey.Left( CCimECID::eMAX_LEN_ECNAME ));
			strcpy_s(g_pData->m_ECID.m_cECNAME[i], strECName.Left( CCimECID::eMAX_LEN_ECNAME ));
		}
	}


	//BOOL fExist = finder.FindFile(sFile);
	//if (fExist > 0)
	//{
	//	CXmlSettings xmlSettings;

	//	if (xmlSettings.Initialize(sFile))
	//	{
	//		g_pData->m_ECID.m_nECID_TotalCount = xmlSettings.GetSettingLong("ECID", "ECID_TotalCount", 0);
	//		strXmlReadECID = new CString[g_pData->m_ECID.m_nECID_TotalCount];
	//		strXmlReadECSEC = new CString[g_pData->m_ECID.m_nECID_TotalCount];
	//		strXmlReadECNAME = new CString[g_pData->m_ECID.m_nECID_TotalCount];

	//		for (int i=0; i<g_pData->m_ECID.m_nECID_TotalCount; i++)
	//		{
	//			strXmlReadECID[i].Format("%s%d", "ECID",i);
	//			strcpy_s(g_pData->m_ECID.m_cECID[i], xmlSettings.GetSettingString("ECID", strXmlReadECID[i], _T("") ));

	//			strXmlReadECSEC[i].Format("%s%d", "ECSEC",i);
	//			strcpy_s(g_pData->m_ECID.m_cECSEC[i], xmlSettings.GetSettingString("ECID", strXmlReadECSEC[i], _T("") ));

	//			strXmlReadECNAME[i].Format("%s%d", "ECNAME",i);
	//			strcpy_s(g_pData->m_ECID.m_cECNAME[i], xmlSettings.GetSettingString("ECID", strXmlReadECNAME[i], _T("") ));
	//		}

	//		delete[] strXmlReadECID;
	//		delete[] strXmlReadECSEC;
	//		delete[] strXmlReadECNAME;
	//	}
	//} 
	//else
	//{
	//	return FALSE;
	//}	

	//finder.Close();

	return TRUE;
}

BOOL CViewECM::ReadECV()
{
	CString sFile;
	sFile = CSettingParm::cECID_ECV_PATH;

	CFileFind finder;
	BOOL fExist = finder.FindFile(sFile);

	if (fExist > 0)
	{
		CCimECID *pEC = &g_pData->m_ECID;
		CIni ini(sFile);
		for (int i=0; i<pEC->m_nECID_TotalCount; i++)
		{
			strcpy_s(pEC->m_cECV[i], ini.GetString(pEC->m_cECSEC[i] , pEC->m_cECKEY[i], _T(" ")).GetBuffer());
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CViewECM::ViewECDip()
{
	int nCnt=0;
	CCimECID *pEC = &g_pData->m_ECID;
	LVITEM lvItem;
	m_ListECM.DeleteAllItems();

	for(int i=0;i<pEC->m_nECID_TotalCount; i++)
	{
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = m_ListECM.GetItemCount();
		lvItem.pszText = (LPSTR)(LPCTSTR)pEC->m_cECID[i];
		lvItem.iSubItem 	= 0;
		m_ListECM.InsertItem(&lvItem);

		m_ListECM.SetItemText(lvItem.iItem,1,MakeECString((CString)pEC->m_cECSEC[i], TRUE));
		m_ListECM.SetItemText(lvItem.iItem,2,MakeECString((CString)pEC->m_cECKEY[i], TRUE));

		pEC->m_cECV[i];
		m_ListECM.SetItemText(lvItem.iItem,3,MakeECString((CString)pEC->m_cECV[i], TRUE));

		m_ListECM.SetItemText(lvItem.iItem,4,pEC->m_cECNAME[i]);

	}

	return TRUE;
}


void CViewECM::OnBnClickedBtnEcmReload()
{
	//////////////////////////////////////////////////////////////////////////
	if (ReadECIDList() !=TRUE)
	{
		AfxMessageBox("There is no ECID file");
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	if(ReadECV() != TRUE)
	{
		AfxMessageBox("Search Fail : ECV Path");
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	ViewECDip();
}
