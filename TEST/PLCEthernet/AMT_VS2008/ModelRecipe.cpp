// ModelRecipe.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "ModelRecipe.h"
#include "DlgNumPad.h"
#include "DlgKeyBoard.h"
#include "DlgContactType.h"
#include "DlgContactNum.h"
#include "DlgContactor.h"
#include "DlgPCBPinNum.h"
#include "DlgPCBType.h"


// CModelRecipe dialog

IMPLEMENT_DYNAMIC(CModelRecipe, CDialog)

CModelRecipe::CModelRecipe(CWnd* pParent /*=NULL*/)
	: CDialog(CModelRecipe::IDD, pParent)
{

}

CModelRecipe::~CModelRecipe()
{
}

void CModelRecipe::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_MODELNAME, m_gridModel);
	DDX_Control(pDX, IDC_GRID_DEVICENAME, m_gridDevice);
	DDX_Control(pDX, IDC_LABEL_PANEL_SIZE_EDIT, m_lblPanelSize);
	DDX_Control(pDX, IDC_LABEL_COMMENT_EDIT, m_lblComment);
	DDX_Control(pDX, IDC_LABEL_ALIGN_SCORE_EDIT, m_lblAlignScore);
	DDX_Control(pDX, IDC_LABEL_CONTACT_TYPE_EDIT, m_lblContactType);
	DDX_Control(pDX, IDC_LABEL_CONTACT_NUM_EDIT, m_lblContactNum);
	DDX_Control(pDX, IDC_LABEL_CONTACTOR_SEL_EDIT, m_lblContactor);
	DDX_Control(pDX, IDC_LABEL_FPCEND_EDIT, m_lblFpcendLength);
	DDX_Control(pDX, IDC_LABEL_PIN_NUM_EDIT, m_lblPCBPinNum);
	DDX_Control(pDX, IDC_LABEL_PCB_TYPE_EDIT, m_lblPCBType);
	DDX_Control(pDX, IDC_LABEL_MODELNAME_EDIT, m_lblModelName);
}


BEGIN_MESSAGE_MAP(CModelRecipe, CDialog)
END_MESSAGE_MAP()


// CModelRecipe message handlers

BOOL CModelRecipe::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	GetModelInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CModelRecipe::SetGridTitleName(void)
{
	m_gridModel.put_FontSize(18);
	m_gridModel.put_FontBold(TRUE);
	m_gridModel.put_TextMatrix(0, 0, "Model Name");
	m_gridModel.put_ColWidth(0, 5760);
	m_gridModel.put_ColAlignment(0, 4);
	m_gridModel.put_FontSize(18);

	m_gridDevice.put_FontSize(12);
	m_gridDevice.put_FontBold(TRUE);
	m_gridDevice.put_TextMatrix(0,0, "Device Name");
	m_gridDevice.put_ColWidth(0, 2880);
	m_gridDevice.put_ColAlignment(0, 4);
	m_gridDevice.put_TextMatrix(0,1, "Model Name");
	m_gridDevice.put_ColWidth(1, 2880);
	m_gridDevice.put_ColAlignment(1, 4);

	m_gridModel.put_SelectionMode(CMsflexgrid_pos::flexSelectionFree);
	m_gridDevice.put_SelectionMode(CMsflexgrid_pos::flexSelectionFree);

}

void CModelRecipe::SelectModel( int nIndex )
{
	m_gridModel.put_Row(m_nOldSelRow);
	for(int i = 1; i < 5; i++)
	{
		m_gridModel.put_Col(i);
		m_gridModel.put_CellBackColor(RGB(255,255,255));
	}
	m_gridModel.put_Row(nIndex);
	for(int i = 1; i < 5; i++)
	{
		m_gridModel.put_Col(i);
		m_gridModel.put_CellBackColor(RGB(128,128,0));
	}
	m_nOldSelRow = nIndex;

	m_gridDevice.put_Row(m_nOldSelRow);
	for(int i = 1; i < 5; i++)
	{
		m_gridDevice.put_Col(i);
		m_gridDevice.put_CellBackColor(RGB(255,255,255));
	}
	m_gridDevice.put_Row(nIndex);
	for(int i = 1; i < 5; i++)
	{
		m_gridDevice.put_Col(i);
		m_gridDevice.put_CellBackColor(RGB(128,128,0));
	}
	m_nOldSelRow = nIndex;

}
BEGIN_EVENTSINK_MAP(CModelRecipe, CDialog)
	ON_EVENT(CModelRecipe, IDC_LABEL_PANEL_SIZE_EDIT, DISPID_CLICK, CModelRecipe::ClickLabelPanelSizeEdit, VTS_NONE)
	ON_EVENT(CModelRecipe, IDC_LABEL_COMMENT_EDIT, DISPID_CLICK, CModelRecipe::ClickLabelCommentEdit, VTS_NONE)
	ON_EVENT(CModelRecipe, IDC_LABEL_ALIGN_SCORE_EDIT, DISPID_CLICK, CModelRecipe::ClickLabelAlignScoreEdit, VTS_NONE)
	ON_EVENT(CModelRecipe, IDC_BTN_CLOSE, DISPID_CLICK, CModelRecipe::ClickBtnClose, VTS_NONE)
	ON_EVENT(CModelRecipe, IDC_LABEL_CONTACT_TYPE_EDIT, DISPID_CLICK, CModelRecipe::ClickLabelContactTypeEdit, VTS_NONE)
	ON_EVENT(CModelRecipe, IDC_LABEL_CONTACT_NUM_EDIT, DISPID_CLICK, CModelRecipe::ClickLabelContactNumEdit, VTS_NONE)
	ON_EVENT(CModelRecipe, IDC_LABEL_CONTACTOR_SEL_EDIT, DISPID_CLICK, CModelRecipe::ClickLabelContactorSelEdit, VTS_NONE)
	ON_EVENT(CModelRecipe, IDC_LABEL_FPCEND_EDIT, DISPID_CLICK, CModelRecipe::ClickLabelFpcendEdit, VTS_NONE)
	ON_EVENT(CModelRecipe, IDC_LABEL_PIN_NUM_EDIT, DISPID_CLICK, CModelRecipe::ClickLabelPinNumEdit, VTS_NONE)
	ON_EVENT(CModelRecipe, IDC_LABEL_PCB_TYPE_EDIT, DISPID_CLICK, CModelRecipe::ClickLabelPcbTypeEdit, VTS_NONE)
	ON_EVENT(CModelRecipe, IDC_GRID_MODELNAME, DISPID_CLICK, CModelRecipe::ClickGridModelname, VTS_NONE)
END_EVENTSINK_MAP()

void CModelRecipe::ClickLabelPanelSizeEdit()
{
	// TODO: Add your message handler code here
	CDlgNumPad dlg;
	int nPanelSize;

	if( dlg.DoModal() == IDOK )
	{
		CString strText;

		strText = dlg.m_strEditText;
		if( strText !="" )	
		{
			nPanelSize = atoi(strText);
			if(nPanelSize <= 0)
				AfxMessageBox("If you do not put is less than 0.");
			else
				m_lblPanelSize.put_Caption(strText);
		}
		strPanelSize = m_lblPanelSize.get_Caption();
	}
}

void CModelRecipe::ClickLabelCommentEdit()
{
	// TODO: Add your message handler code here
	CDlgKeyBoard dlg;

	if( dlg.DoModal() == IDOK )
	{
		CString strText;

		strText = dlg.m_strEditText;
		strText.MakeUpper();
		if( strText !="" )	
		{
			m_lblComment.put_Caption(strText);
		}
	}
}

void CModelRecipe::ClickLabelAlignScoreEdit()
{
	// TODO: Add your message handler code here
	CDlgNumPad dlg;

	if( dlg.DoModal() == IDOK )
	{
		CString strText;

		strText.Format("%s%s", dlg.m_strEditText, "%");
		if( strText !="" )	m_lblAlignScore.put_Caption(strText);
	}
}

void CModelRecipe::ClickBtnClose()
{
	// TODO: Add your message handler code here
	OnOK();
}

void CModelRecipe::ClickLabelContactTypeEdit()
{
	// TODO: Add your message handler code here
	CDlgContactType dlg;
	dlg.DoModal();

	m_strContactType = dlg.strContactType;
	m_lblContactType.put_Caption(m_strContactType);
}

void CModelRecipe::ClickLabelContactNumEdit()
{
	// TODO: Add your message handler code here
	CDlgContactNum dlg;
	dlg.DoModal();

	m_strContactNum = dlg.strContactNum;
	m_lblContactNum.put_Caption(m_strContactNum);
}

void CModelRecipe::ClickLabelContactorSelEdit()
{
	// TODO: Add your message handler code here
	CDlgContactor dlg;
	dlg.DoModal();

	m_strContactor = dlg.strContactor;
	m_lblContactor.put_Caption(m_strContactor);
}

void CModelRecipe::ClickLabelFpcendEdit()
{
	// TODO: Add your message handler code here
	CDlgNumPad dlg;

	if( dlg.DoModal() == IDOK )
	{
		CString strText;

		strText.Format("%s%s", dlg.m_strEditText, "mm");
		if( strText !="" )	m_lblFpcendLength.put_Caption(strText);
	}
}

void CModelRecipe::ClickLabelPinNumEdit()
{
	// TODO: Add your message handler code here
	CDlgPCBPinNum dlg;
	dlg.DoModal();

	m_strPCBPinNum = dlg.strPinNum;
	m_lblPCBPinNum.put_Caption(m_strPCBPinNum);

}

void CModelRecipe::ClickLabelPcbTypeEdit()
{
	// TODO: Add your message handler code here
	CDlgPCBType dlg;
	dlg.DoModal();

	m_strPCBType = dlg.strPCBType;
	m_lblPCBType.put_Caption(m_strPCBType);
}

void CModelRecipe::ClickGridModelname()
{
	CIni ini;
	CString strDirFilePath, strSection, strEntry, strFilePath ;
	CString strModelName, strPanelSize, strContactType, strContactNum, strContactor, strDeviceName, 
		strGripper1, strGripper2, strGripper3, strGripper4, strAlignScore ;
	int nPanelSize, nContactNum, nContactType, nContactor, nAlignScore/*, nBLU_Model*/;
	int nRow = m_gridModel.get_MouseRow();
	int nCol = m_gridModel.get_MouseCol();

	if((nRow == 0))
		return;
	
	m_lblModelName.put_Caption(m_gridModel.get_TextMatrix(nRow, 0));
	strDirFilePath.Format("%s\\%s\\System.INI" , GetApp()->m_strRecipePath ,strModelName );
	ini.SetIniFilename(strDirFilePath);

	strSection = _T("MODEL INFO");		
	strEntry = _T("Panel_Size");		nPanelSize = ini.GetLong(strSection,	strEntry, 0);
	strEntry = _T("ContactNum");		nContactNum = ini.GetLong(strSection,	strEntry, 0);
	strEntry = _T("Resource");			strContactType = ini.GetString(strSection,	strEntry, "");
	strEntry = _T("Contactor");			nContactor = ini.GetLong(strSection,	strEntry, 0);
	strEntry = _T("ContactType");		nContactType = ini.GetLong(strSection,	strEntry, 0);
	strEntry = _T("DeviceName");		strDeviceName = ini.GetString(strSection,	strEntry, "");
	strEntry = _T("Gripper1");			strGripper1 = ini.GetString(strSection,	strEntry, "");
	strEntry = _T("Gripper2");			strGripper2 = ini.GetString(strSection,	strEntry, "");
	strEntry = _T("Gripper3");			strGripper3 = ini.GetString(strSection,	strEntry, "");
	strEntry = _T("Gripper4");			strGripper4 = ini.GetString(strSection,	strEntry, "");
	strEntry = _T("Align Score");		nAlignScore = ini.GetLong(strSection,	strEntry, 0);
	strEntry = _T("Align Angle");		bTiltChk = ini.GetBoolean(strSection,	strEntry, FALSE);

	strPanelSize.Format("%d", nPanelSize);
	m_lblPanelSize.put_Caption(strPanelSize);
	
	strContactNum.Format("%d", nContactNum);
	m_lblContactNum.put_Caption(strContactNum);

	m_lblContactType.put_Caption(strContactType);



}

void CModelRecipe::GetModelInfo(void)
{
	m_gridModel.Clear();
	m_gridModel.put_Rows(1);

	SetGridTitleName();

	CIni ini;
	CFileFind finder;
	long a = 1;

	CString strPath, strModelName, strTemp, strContactNum,strContactType, strPanelSize, strDeviceName;
	CString strDirFilePath, strSection, strEntry, strFilePath;
	int nPanelSize, nContactNum;

	strPath.Format("%s\\*.*", GetApp()->m_strRecipePath );

	BOOL bfind = finder.FindFile(strPath);

	while(bfind)
	{
		bfind = finder.FindNextFile();

		if( finder.IsDots() )
			continue;

		if( finder.IsDirectory() )
		{
			strTemp = finder.GetFileName();
			if(strTemp == GetDoc()->m_pAmtData->m_CurModelInfo.strModelName)
				m_nOldSelRow = a;
			strDirFilePath.Format("%s\\%s\\System.INI" , GetApp()->m_strRecipePath ,strTemp );
			ini.SetIniFilename(strDirFilePath);
			strSection = _T("MODEL INFO");

			nPanelSize = ini.GetLong(strSection, "Panel_Size", 0);
			strContactType = ini.GetString(strSection, "Resource", "FFC");
			nContactNum = ini.GetLong(strSection, "ContactNum", 0);
			strDeviceName = ini.GetString(strSection, "DeviceName");
			strModelName.Format("%d%d%s_%s", nPanelSize, nContactNum, strContactType,strDeviceName);
			m_gridModel.AddItem(strModelName, _variant_t(a++));
			m_gridModel.put_RowHeight(a, 700);
			m_gridModel.put_FontSize(12);
			m_gridModel.put_FontBold(TRUE);
		}
	}

	finder.Close();
	SelectModel(m_nOldSelRow);
}
