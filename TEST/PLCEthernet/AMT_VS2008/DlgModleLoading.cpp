// DlgModleLoading.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "DlgModleLoading.h"


// CDlgModleLoading dialog

IMPLEMENT_DYNAMIC(CDlgModleLoading, CDialog)

CDlgModleLoading::CDlgModleLoading(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgModleLoading::IDD, pParent)
	, m_strDeviceName(_T(""))
{

}

CDlgModleLoading::~CDlgModleLoading()
{
}

void CDlgModleLoading::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_DEVICE_NAME, m_gridDeviceName);
	DDX_Control(pDX, IDC_LABEL_PANEL_SIZE_EDIT, m_lblPanelSize);
	DDX_Control(pDX, IDC_LABEL_FPC_EDIT, m_lblFPCType);
	DDX_Control(pDX, IDC_LABEL_CONTACTNUM_EDIT, m_lblContactNum);
	DDX_Control(pDX, IDC_LABEL_CONTACTOR_EDIT, m_lblContactor);
	DDX_Control(pDX, IDC_LABEL_FPC_LENGTH_EDIT, m_lblFPCLength);
	DDX_Control(pDX, IDC_LABEL_PIN_NUM_EDIT, m_lblPinNum);
	DDX_Control(pDX, IDC_LABEL_ALIGN_SUCCESS_EDIT, m_lblAlignSuccess);
	DDX_Control(pDX, IDC_LABEL_COMMENT_EDIT, m_lblComment);
	DDX_Control(pDX, IDC_LABEL_DEVICE_NAME_EDIT, m_lblDeviceName);
	DDX_Control(pDX, IDC_LABEL_NEW_PANEL_SIZE_EDIT, m_lblNewPanelSize);
}


BEGIN_MESSAGE_MAP(CDlgModleLoading, CDialog)
END_MESSAGE_MAP()


// CDlgModleLoading message handlers
BOOL CDlgModleLoading::DeleteAllFile(CString strPath)
{
	int nCount = 0;
	CFileFind finder;

	CString str(strPath);
	str += _T("\\*.*");

	BOOL bWorking = finder.FindFile(str);

	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		CString str = finder.GetFilePath();
		DeleteFile(str);
		nCount++;
	}
	finder.Close();

	return nCount;
}

BOOL CDlgModleLoading::DeletelINIFile(CString strPath)
{
	int nCount = 0;
	CFileFind finder;

	CString str(strPath);
	str += _T("\\System.INI");
	/*	str += _T("\\*.INI");*/

	BOOL bWorking = finder.FindFile(str);

	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		CString str = finder.GetFilePath();
		DeleteFile(str);
		nCount++;
	}
	finder.Close();

	return nCount;
}
BOOL CDlgModleLoading::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CreateGridTitle();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgModleLoading::SetLoadingData(void)
{
	CString strDeviceName, strFPCType, strContactor, strPinNum, strFPClen, strAlignSucc, strPanelSize;

	strDeviceName = GetDoc()->m_pAmtData->m_CurModelInfo.strDeviceName;
	m_lblDeviceName.put_Caption(strDeviceName);

	strAlignSucc.Format("%.2f %", GetDoc()->m_pAmtData->m_CurModelInfo.iAlignScore);
	m_lblAlignSuccess.put_Caption(strAlignSucc);

	switch (GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum)
	{
	case CAmtData::ONE_CONT:
		m_lblContactNum.put_Caption("One Contact");
		break;
	case CAmtData::TWO_CONT:
		m_lblContactNum.put_Caption("Two Contact");
		break;
	case CAmtData::FOUR_CONT:
		m_lblContactNum.put_Caption("Four Contact");
		break;
	}

	switch (GetDoc()->m_pAmtData->m_CurModelInfo.iContactor)
	{
	case CAmtData::CONTACT_0:
		m_lblContactNum.put_Caption("Contact 1");
		break;
	case CAmtData::CONTACT_1:
		m_lblContactNum.put_Caption("Contact 2");
		break;
	case CAmtData::CONTACT_2:
		m_lblContactNum.put_Caption("Contact 3");
		break;
	case CAmtData::CONTACT_3:
		m_lblContactNum.put_Caption("Contact 4");
		break;
	}
	
	switch (GetDoc()->m_pAmtData->m_CurModelInfo.iContactType)
	{
	case CAmtData::FFC_TYPE:
		m_lblFPCType.put_Caption("FFC Type");
		break;
	case CAmtData::FPC_TYPE:
		m_lblFPCType.put_Caption("FPC Type");
		break;
	case CAmtData::PIN_TYPE:
		m_lblFPCType.put_Caption("Pin Type");
		break;
	}

	strFPClen.Format("%d", GetDoc()->m_pAmtData->m_CurModelInfo.iFPCLength);
	m_lblFPCLength.put_Caption(strFPClen);

	strPanelSize.Format("%d", GetDoc()->m_pAmtData->m_CurModelInfo.iPanelSize);
	m_lblPanelSize.put_Caption(strPanelSize);

	strPinNum.Format("%d", GetDoc()->m_pAmtData->m_CurModelInfo.iPinNum);
	m_lblPinNum.put_Caption(strPinNum);
	

}

void CDlgModleLoading::OpenModelFile(CString strModelName)
{
	CString strDirFilePath, strSection, strEntry, strFilePath, strDeviceName;

	strDirFilePath.Format("%s\\%s\\System.INI" , GetApp()->m_strRecipePath , strDeviceName);

	CIni ini;	
	ini.SetIniFilename(strDirFilePath);

	strSection = _T("MODEL INFO");																						

	m_strDeviceName 	= 	ini.GetString(strSection, "DeviceName", _T("0"));
	m_nFPCType			= 	ini.GetLong(strSection, "ContactType", 0);
	m_nPanelSize		= 	ini.GetLong(strSection, "Panel_Size", 40);
	m_nContactNum		= 	ini.GetLong(strSection, "ContactNum", 2);			
	m_nContactor		= 	ini.GetLong(strSection, "Contactor", 0);			
	m_nFPCLength		= 	ini.GetLong(strSection, "FPC_Length", 0);
	m_nAlignSuccess		= 	ini.GetLong(strSection, "Align Score", 70);			
	m_nContactPinNum	= 	ini.GetLong(strSection, "Contact Pin Num", 0);	
}
BEGIN_EVENTSINK_MAP(CDlgModleLoading, CDialog)
	ON_EVENT(CDlgModleLoading, IDC_GRID_DEVICE_NAME, DISPID_CLICK, CDlgModleLoading::ClickGridDeviceName, VTS_NONE)
END_EVENTSINK_MAP()

void CDlgModleLoading::ClickGridDeviceName()
{
	// TODO: Add your message handler code here

	CString strDirFilePath, strSection, strEntry, strFilePath ;
	CString strModelName, strPanelSize, strResource, strContactNum, strContactor, strDeviceName, strAlignSuccess; 
// 	int nContactNum, nContactType, nContactor;

	int nRow = m_gridDeviceName.get_MouseRow();
	int nCol = m_gridDeviceName.get_MouseCol();

	if(nRow == 0)
		return;

	strDeviceName = m_gridDeviceName.get_TextMatrix(nRow,nCol);
	m_lblDeviceName.put_Caption(strDeviceName);

	OpenModelFile(strDeviceName);

	strPanelSize.Format("%d", m_nPanelSize);
	m_lblPanelSize.put_Caption(strPanelSize);

	

	switch (m_nFPCType)
	{
	case 1:
		m_lblContactNum.put_Caption("One Contact");
		break;
	case 2:
		m_lblContactNum.put_Caption("Two Contact");
		break;
	case 4:
		m_lblContactNum.put_Caption("Four Contact");
		break;
	}

	switch (m_nContactor)
	{
	case 1:
		m_lblContactNum.put_Caption("Contact 1");
		break;
	case 2:
		m_lblContactNum.put_Caption("Contact 2");
		break;
	case 3:
		m_lblContactNum.put_Caption("Contact 3");
		break;
	case 4:
		m_lblContactNum.put_Caption("Contact 4");
		break;
	}

	switch (m_nFPCType)
	{
	case 1:
		m_lblFPCType.put_Caption("FFC Type");
		break;
	case 2:
		m_lblFPCType.put_Caption("FPC Type");
		break;
	case 3:
		m_lblFPCType.put_Caption("Pin Type");
		break;
	}

	strAlignSuccess.Format("%.1f", m_nAlignSuccess);
	m_lblAlignSuccess.put_Caption(strAlignSuccess);

}

void CDlgModleLoading::CreateGridTitle(void)
{
	m_gridDeviceName.put_Cols(1);
	m_gridDeviceName.put_Rows(2);
	m_gridDeviceName.put_TextMatrix(0,0,"Device Name");
	m_gridDeviceName.put_ColWidth(0, 5300);
	m_gridDeviceName.put_FontSize(24);
	m_gridDeviceName.put_ColAlignment(0,4);
	m_gridDeviceName.put_FixedRows(1);

}
