// DlgModelChange.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "DlgModelChange.h"
#include "DlgNumPad.h"
#include "DlgKeyBoard.h"

using namespace Gdiplus;	// Color class


// CDlgModelChange dialog

IMPLEMENT_DYNAMIC(CDlgModelChange, CDialog)

CDlgModelChange::CDlgModelChange(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgModelChange::IDD, pParent)
{
	bTiltChk = FALSE;
	m_nContactNum = 0;
	m_nOldSelRow = 1;
}

CDlgModelChange::~CDlgModelChange()
{
}

void CDlgModelChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABEL_MODELNAME, m_lblModelName);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_gridModelList);
	DDX_Control(pDX, IDC_LABEL_PANEL_SIZE, m_lblPanelSize);
	DDX_Control(pDX, IDC_COMBO_CONTACT_TYPE, m_cmbContactType);
	DDX_Control(pDX, IDC_COMBO_CONTACT_NUM, m_cmbContactNum);
	DDX_Control(pDX, IDC_COMBO_CONTACT_SELCTION, m_cmbContactSel);
	DDX_Control(pDX, IDC_LABEL_CONTACT_TYPE, m_lblContactType);
	DDX_Control(pDX, IDC_LABEL_CONTACCT_NUM, m_lblContactNum);
	DDX_Control(pDX, IDC_LABEL_SELCTION_CONTACTOR, m_lblContactSel);
	DDX_Control(pDX, IDC_BTN_ALIGN_TILT_USE, m_chkAlignTiltUse);
	DDX_Control(pDX, IDC_LABEL_SCORE_ALIGN, m_lblScoreAlign);
	DDX_Control(pDX, IDC_COMBO_CONTACT_TYPE2, m_cmbBLUModel);
	DDX_Control(pDX, IDC_BTN_MODEL_CREATE, m_chkModelCreate);
	DDX_Control(pDX, IDC_BTN_MODEL_CHANGE, m_chkModelChange);
	DDX_Control(pDX, IDC_LABEL_COMMENT, m_lblDeviceName);
	DDX_Control(pDX, IDC_LABEL_HANDLE_SCALE1, m_lblGripper1);
	DDX_Control(pDX, IDC_LABEL_HANDLE_SCALE2, m_lblGripper2);
	DDX_Control(pDX, IDC_LABEL_HANDLE_SCALE3, m_lblGripper3);
	DDX_Control(pDX, IDC_LABEL_HANDLE_SCALE4, m_lblGripper4);
}


BEGIN_MESSAGE_MAP(CDlgModelChange, CDialog)
	ON_BN_CLICKED(IDC_BTN_MODEL_LOADING, &CDlgModelChange::OnBnClickedBtnModelLoading)
	ON_CBN_SELCHANGE(IDC_COMBO_CONTACT_TYPE, &CDlgModelChange::OnCbnSelchangeComboContactType)
	ON_CBN_SELCHANGE(IDC_COMBO_CONTACT_NUM, &CDlgModelChange::OnCbnSelchangeComboContactNum)
	ON_CBN_SELCHANGE(IDC_COMBO_CONTACT_SELCTION, &CDlgModelChange::OnCbnSelchangeComboContactSelction)
	ON_BN_CLICKED(IDCANCEL, &CDlgModelChange::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_MODEL_DELET, &CDlgModelChange::OnBnClickedBtnModelDelet)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CDlgModelChange::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDOK, &CDlgModelChange::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgModelChange message handlers

void CDlgModelChange::OnBnClickedBtnModelLoading()
{
	int nIndex, nContactor, nBLUModel;
	nIndex = m_gridModelList.get_Row();
	if(nIndex > 0)
	{
		m_strModelName		=	m_lblDeviceName.get_Caption();	
		m_strDeviceName		=	m_lblDeviceName.get_Caption();
		m_strPanelSize		=	m_lblPanelSize.get_Caption();
		m_nContactType		=	m_cmbContactType.GetCurSel();
		m_strResource		=	m_lblContactType.get_Caption();
		m_strAlignScore		=	m_lblScoreAlign.get_Caption();

		if(m_cmbContactNum.GetCurSel() == 3)
			m_nContactNum		=	CAmtData::FOUR_CONT;
		else if(m_cmbContactNum.GetCurSel() != 0)
			m_nContactNum		=	m_cmbContactNum.GetCurSel();
		else 
			return;

		nContactor = m_cmbContactSel.GetCurSel();
		if(nContactor == 1)
			m_nContactSel = CAmtData::CONTACT_0;
		else if(nContactor == 2)
			m_nContactSel = CAmtData::CONTACT_1;
		else if(nContactor == 3)
			m_nContactSel = CAmtData::CONTACT_2;
		else if(nContactor == 4)
			m_nContactSel = CAmtData::CONTACT_3;

		nBLUModel = m_cmbBLUModel.GetCurSel();
		if(nBLUModel == 1)
			m_nBLUModel = BluModel_WT;
		else if(nBLUModel == 2)
			m_nBLUModel = BluModel_WS;
		else if(nBLUModel == 3)
			m_nBLUModel = BluModel_HD;
		else if(nBLUModel == 4)
			m_nBLUModel = BluModel_ETC;



		m_strGripper1 = m_lblGripper1.get_Caption();
		m_strGripper2 = m_lblGripper2.get_Caption();
		m_strGripper3 = m_lblGripper3.get_Caption();
		m_strGripper4 = m_lblGripper4.get_Caption();

		ApplyData();
	}
	else
	{
		AfxMessageBox("선택되어진 Item이 없습니다.");
	}
}
BEGIN_EVENTSINK_MAP(CDlgModelChange, CDialog)
	ON_EVENT(CDlgModelChange, IDC_MSFLEXGRID1, DISPID_CLICK, CDlgModelChange::ClickMsflexgrid1, VTS_NONE)
	ON_EVENT(CDlgModelChange, IDC_LABEL_PANEL_SIZE, DISPID_CLICK, CDlgModelChange::ClickLabelPanelSize, VTS_NONE)
	ON_EVENT(CDlgModelChange, IDC_BTN_ALIGN_TILT_USE, 5, CDlgModelChange::OnClickBtnAlignTiltUse, VTS_NONE)
	ON_EVENT(CDlgModelChange, IDC_LABEL_SCORE_ALIGN, DISPID_CLICK, CDlgModelChange::ClickLabelScoreAlign, VTS_NONE)
	ON_EVENT(CDlgModelChange, IDC_LABEL_COMMENT, DISPID_CLICK, CDlgModelChange::ClickLabelComment, VTS_NONE)
	ON_EVENT(CDlgModelChange, IDC_BTN_MODEL_CREATE, 5, CDlgModelChange::OnClickBtnModelCreate, VTS_NONE)
	ON_EVENT(CDlgModelChange, IDC_BTN_MODEL_CHANGE, 5, CDlgModelChange::OnClickBtnModelChange, VTS_NONE)
	ON_EVENT(CDlgModelChange, IDC_LABEL_HANDLE_SCALE1, DISPID_CLICK, CDlgModelChange::ClickLabelHandleScale1, VTS_NONE)
	ON_EVENT(CDlgModelChange, IDC_LABEL_HANDLE_SCALE2, DISPID_CLICK, CDlgModelChange::ClickLabelHandleScale2, VTS_NONE)
	ON_EVENT(CDlgModelChange, IDC_LABEL_HANDLE_SCALE3, DISPID_CLICK, CDlgModelChange::ClickLabelHandleScale3, VTS_NONE)
	ON_EVENT(CDlgModelChange, IDC_LABEL_HANDLE_SCALE4, DISPID_CLICK, CDlgModelChange::ClickLabelHandleScale4, VTS_NONE)
END_EVENTSINK_MAP()

void CDlgModelChange::ClickMsflexgrid1()
{
	// TODO: Add your message handler code here
	CIni ini;
	CString strDirFilePath, strSection, strEntry, strFilePath ;
	CString strModelName, strPanelSize, strResource, strContactNum, strContactor, strDeviceName, 
		strGripper1, strGripper2, strGripper3, strGripper4, strAlignScore ;
	int nPanelSize, nContactNum, nContactType, nContactor, nAlignScore, nBLU_Model;
	int nRow = m_gridModelList.get_MouseRow();
	int nCol = m_gridModelList.get_MouseCol();

	if((nRow == 0))
		return;
// 	strModelName.Format("%s_%s%s_%s", m_gridModelList.get_TextMatrix(nRow, 1), m_gridModelList.get_TextMatrix(nRow, 2), m_gridModelList.get_TextMatrix(nRow, 3), m_gridModelList.get_TextMatrix(nRow, 4));
	strModelName.Format("%s", m_gridModelList.get_TextMatrix(nRow, 1));

// 	m_lblModelName.put_Caption(strModelName);
	m_lblDeviceName.put_Caption(strModelName);

	strDirFilePath.Format("%s\\%s\\System.INI" , GetApp()->m_strRecipePath ,strModelName );
	
	ini.SetIniFilename(strDirFilePath);

	strSection = _T("MODEL INFO");		
	strEntry = _T("Panel_Size");		nPanelSize = ini.GetLong(strSection,	strEntry, 0);
	strEntry = _T("ContactNum");		nContactNum = ini.GetLong(strSection,	strEntry, 0);
	strEntry = _T("Resource");			strResource = ini.GetString(strSection,	strEntry, "");
	strEntry = _T("Contactor");			nContactor = ini.GetLong(strSection,	strEntry, 0);
	strEntry = _T("ContactType");		nContactType = ini.GetLong(strSection,	strEntry, 0);
	strEntry = _T("DeviceName");		strDeviceName = ini.GetString(strSection,	strEntry, "");
	strEntry = _T("Gripper1");			strGripper1 = ini.GetString(strSection,	strEntry, "");
	strEntry = _T("Gripper2");			strGripper2 = ini.GetString(strSection,	strEntry, "");
	strEntry = _T("Gripper3");			strGripper3 = ini.GetString(strSection,	strEntry, "");
	strEntry = _T("Gripper4");			strGripper4 = ini.GetString(strSection,	strEntry, "");
	strEntry = _T("Align Score");		nAlignScore = ini.GetLong(strSection,	strEntry, 0);
	strEntry = _T("Align Angle");		bTiltChk = ini.GetBoolean(strSection,	strEntry, FALSE);

	strSection = _T("BLU Model");		
	strEntry = _T("BLU_Model");			nBLU_Model = ini.GetLong(strSection,	strEntry, 0);

	strPanelSize.Format("%d", nPanelSize);
	m_lblPanelSize.put_Caption(strPanelSize);

	if(nContactNum == 4)
	{
		m_cmbContactNum.SetCurSel(3);
		m_cmbContactNum.GetLBText(3, strContactNum);
		m_lblContactNum.put_Caption(strContactNum);
	}
	else
	{
		m_cmbContactNum.SetCurSel(nContactNum);
		m_cmbContactNum.GetLBText(nContactNum, strContactNum);
		m_lblContactNum.put_Caption(strContactNum);
	}

	m_cmbContactType.SetCurSel(nContactType);
	m_cmbContactType.GetLBText(nContactType, strResource);
	m_lblContactType.put_Caption(strResource);

	m_cmbContactSel.SetCurSel(nContactor);
	m_cmbContactSel.GetLBText(nContactor, strContactor);
	m_lblContactSel.put_Caption(strContactor);

	m_lblDeviceName.put_Caption(strDeviceName);
	m_lblGripper1.put_Caption(strGripper1);
	m_lblGripper2.put_Caption(strGripper2);
	m_lblGripper3.put_Caption(strGripper3);
	m_lblGripper4.put_Caption(strGripper4);

	strAlignScore.Format("%d%%", nAlignScore);
	m_lblScoreAlign.put_Caption(strAlignScore);
	
	nBLU_Model = nBLU_Model++;
	m_cmbBLUModel.SetCurSel(nBLU_Model);

	m_chkAlignTiltUse.put_Active(bTiltChk);

	m_chkModelCreate.put_Active(FALSE);
	m_chkModelChange.put_Active(FALSE);
	SelectModel(nRow);
}

BOOL CDlgModelChange::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	GetModelToFile();
	InitModel();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgModelChange::GetModelToFile(void)
{
	// 이전 항목 삭제
	m_gridModelList.Clear();
	m_gridModelList.put_Rows(1);

	CreateGridTitle();

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
			if(strTemp == GetDoc()->m_pAmtData->m_CurModelInfo.strDeviceName)
				m_nOldSelRow = a;
			strDirFilePath.Format("%s\\%s\\System.INI" , GetApp()->m_strRecipePath ,strTemp );
			ini.SetIniFilename(strDirFilePath);
			strSection = _T("MODEL INFO");

			nPanelSize = ini.GetLong(strSection, "Panel_Size", 0);
			strContactType = ini.GetString(strSection, "Resource", "FFC");
			nContactNum = ini.GetLong(strSection, "ContactNum", 0);
			strDeviceName = ini.GetString(strSection, "DeviceName");
//  			strModelName.Format("%d\t%d\t%d\t%s\t%s", a, nPanelSize, nContactNum, strContactType,strDeviceName);
			strModelName.Format("%d\t%s", a,strDeviceName);
			m_gridModelList.AddItem(strModelName, _variant_t(a++));
			m_gridModelList.put_RowHeight(a-1, 700);
			m_gridModelList.put_FontSize(12);
			m_gridModelList.put_FontBold(TRUE);
		}
	}

	finder.Close();
	SelectModel(m_nOldSelRow);
}

void CDlgModelChange::ClickLabelPanelSize()
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
		m_strPanelSize = m_lblPanelSize.get_Caption();


	}
}

void CDlgModelChange::OnCbnSelchangeComboContactType()
{
	int nIndex;
	CString strContactType;
	nIndex = m_cmbContactType.GetCurSel();
	m_cmbContactType.GetLBText(nIndex, strContactType);
	m_lblContactType.put_Caption(strContactType);
	m_strResource = m_lblContactType.get_Caption();

}

void CDlgModelChange::OnCbnSelchangeComboContactNum()
{
	int nIndex, nContactNum;
	CString strContactNum;
	nIndex = m_cmbContactNum.GetCurSel();
	m_cmbContactNum.GetLBText(nIndex, strContactNum);
	m_lblContactNum.put_Caption(strContactNum);
	switch (m_cmbContactNum.GetCurSel())
	{
	case 0:
		nContactNum =0;
		break;
	case 1:
		nContactNum = 1;
		m_cmbContactSel.EnableWindow(TRUE);
		break;
	case 2:
		nContactNum = 2;
		m_cmbContactSel.EnableWindow(FALSE);
		break;
	case 3:
		nContactNum = 4;
		m_cmbContactSel.EnableWindow(FALSE);
		break;
	}

}

void CDlgModelChange::OnCbnSelchangeComboContactSelction()
{
	int nIndex;
	CString strContactSel;
	nIndex = m_cmbContactSel.GetCurSel();
	m_cmbContactSel.GetLBText(nIndex, strContactSel);
	m_lblContactSel.put_Caption(strContactSel);
}

void CDlgModelChange::OnClickBtnAlignTiltUse()
{
	// TODO: Add your message handler code here
	if(bTiltChk == FALSE)
	{
		bTiltChk = TRUE;
		m_chkAlignTiltUse.put_Active(!bTiltChk);
		GetDoc()->m_pAmtData->m_CurModelInfo.bAlignAngle = bTiltChk;
	}
	else
	{
		bTiltChk = FALSE;
		m_chkAlignTiltUse.put_Active(!bTiltChk);
		GetDoc()->m_pAmtData->m_CurModelInfo.bAlignAngle = bTiltChk;
	}
}

void CDlgModelChange::ClickLabelScoreAlign()
{
	CDlgNumPad dlg;

	if( dlg.DoModal() == IDOK )
	{
		CString strText;

		strText.Format("%s%s", dlg.m_strEditText, "%");
		if( strText !="" )	m_lblScoreAlign.put_Caption(strText);
	}
}

void CDlgModelChange::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	m_cmbContactNum.SetCurSel(0);
	m_cmbContactSel.SetCurSel(0);
	m_cmbContactType.SetCurSel(0);
	m_lblContactNum.put_Caption("");
	m_lblContactSel.put_Caption("");
	m_lblContactType.put_Caption("");
	m_lblPanelSize.put_Caption("");
	m_lblScoreAlign.put_Caption("");
	m_chkModelCreate.put_Active(FALSE);
	m_chkModelChange.put_Active(FALSE);
	GetModelToFile();
/*	OnCancel();*/
}

void CDlgModelChange::OnBnClickedBtnModelDelet()
{
	int nIndex;
	CString str, strPath, strCurrentModel;
	nIndex = m_gridModelList.get_Row();
	strCurrentModel.Format("%s",m_gridModelList.get_TextMatrix(nIndex,1));
	if(nIndex != NULL)
	{
		if(strCurrentModel != GetDoc()->m_pAmtData->m_CurModelInfo.strModelName)
		{
			if(m_gridModelList.get_Rows() != 2)
			{
				CString strTemp;
				strTemp.Format("%s\n Do you want Delete?", strCurrentModel);
				if(AfxMessageBox(strTemp, MB_OKCANCEL) == IDOK)
				{
					str.Format("%s", m_lblDeviceName.get_Caption());
					strPath.Format("%s\\%s",GetApp()->m_strRecipePath, str);
					DeleteAllFile(strPath);
					RemoveDirectory(strPath);

					GetModelToFile();
					InitModel();
				}
			}
			else
				AfxMessageBox("There is no selected file");
		}
		else
		{
			AfxMessageBox("The current model can not Delete it.");
		}
	}
	else
	{
		AfxMessageBox("This item can not be deleted.");
	}
}
BOOL CDlgModelChange::DeleteAllFile(CString strPath)
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

BOOL CDlgModelChange::DeletelINIFile(CString strPath)
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
void CDlgModelChange::ClickLabelComment()
{
	CDlgKeyBoard dlg;

	if( dlg.DoModal() == IDOK )
	{
		CString strText;

		strText = dlg.m_strEditText;
		if( strText !="" )	
		{
			m_lblDeviceName.put_Caption(strText);
		}
	}
}


void CDlgModelChange::InitModel(void)
{
	CString strPanelSize, strContactNum, strContactType, strContactor, strBLUModel, strAlignScore;
	int nContactType, nContactNum, nContactor, nBLUModel, nAlignScore;

	m_lblModelName.put_Caption(GetDoc()->m_pAmtData->m_CurModelInfo.strModelName);
	strPanelSize.Format("%d", GetDoc()->m_pAmtData->m_CurModelInfo.iPanelSize);
	m_lblPanelSize.put_Caption(strPanelSize);
	nContactType = GetDoc()->m_pAmtData->m_CurModelInfo.iContactType;
	m_cmbContactType.SetCurSel(nContactType);
	m_cmbContactType.GetLBText(nContactType, strContactType);
	m_lblContactType.put_Caption(strContactType);
	nContactNum = GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum;	
	if(nContactNum == CAmtData::FOUR_CONT)
	{
		m_cmbContactNum.SetCurSel(3);
		m_cmbContactNum.GetLBText(3,strContactNum);
		m_lblContactNum.put_Caption(strContactNum);
	}
	else 
	{
		m_cmbContactNum.SetCurSel(nContactNum);
		m_cmbContactNum.GetLBText(nContactNum, strContactNum);
		m_lblContactNum.put_Caption(strContactNum);
	}

	nBLUModel = GetDoc()->m_pAmtData->m_CurModelInfo.m_nBluModel;
	m_cmbBLUModel.SetCurSel(nBLUModel+1);

	nContactor = GetDoc()->m_pAmtData->m_CurModelInfo.iContactor;
	m_cmbContactSel.SetCurSel(nContactor);
	m_cmbContactSel.GetLBText(nContactor, strContactor);
	m_lblContactSel.put_Caption(strContactor);

	nAlignScore  = GetDoc()->m_pAmtData->m_CurModelInfo.iAlignScore;
	strAlignScore.Format("%d", nAlignScore);
	m_lblScoreAlign.put_Caption(strAlignScore);

	m_lblGripper1.put_Caption(GetDoc()->m_pAmtData->m_CurModelInfo.strGripper1);
	m_lblGripper2.put_Caption(GetDoc()->m_pAmtData->m_CurModelInfo.strGripper2);
	m_lblGripper3.put_Caption(GetDoc()->m_pAmtData->m_CurModelInfo.strGripper3);
	m_lblGripper4.put_Caption(GetDoc()->m_pAmtData->m_CurModelInfo.strGripper4);

}

void CDlgModelChange::OnClickBtnModelCreate()
{
	m_lblModelName.put_Caption(GetDoc()->m_pAmtData->m_CurModelInfo.strDeviceName);
	m_lblPanelSize.put_Caption("Size Insert");
	m_lblContactNum.put_Caption("");
	m_lblContactSel.put_Caption("");
	m_lblContactType.put_Caption("");
	m_lblGripper1.put_Caption("");
	m_lblGripper2.put_Caption("");
	m_lblGripper3.put_Caption("");
	m_lblGripper4.put_Caption("");
	m_lblScoreAlign.put_Caption("70 %");

	m_cmbContactNum.SetCurSel(0);
	m_cmbContactType.SetCurSel(0);
	m_cmbContactSel.SetCurSel(0);
	m_cmbBLUModel.SetCurSel(1);

	m_chkAlignTiltUse.put_Active(FALSE);
	m_chkModelChange.put_Active(FALSE);
}

void CDlgModelChange::OnClickBtnModelChange()
{
	m_chkModelCreate.put_Active(FALSE);
}

void CDlgModelChange::OnBnClickedBtnApply()
{
	CString strAlignScore , strModelName, strGridName, strDeviceName;
	int nContactNum, nRow, nCol, nBLUModel;
	nRow = m_gridModelList.get_Rows();
	nCol = m_gridModelList.get_Col();
	if(m_chkModelCreate.get_Active() == TRUE)
	{
		if(AfxMessageBox("Would you like to generate?", MB_OKCANCEL) == IDOK)
		{
			if(m_cmbContactNum.GetCurSel() == 0)
			{
				AfxMessageBox("Has not been selected.");
				return;
			}
			if((m_cmbContactNum.GetCurSel() == 1))
			{
				if(m_cmbContactSel.GetCurSel() == 0)
				{
					AfxMessageBox("Has not been selected.");
				}
			}
			// 신규 생성은 불러봤자 0이니까 불어오지 않는다.
			//GetDoc()->OpenModelFile(strModelName);

			GetDoc()->m_pAmtData->m_CurModelInfo.iPanelSize = atoi(m_lblPanelSize.get_Caption());
			GetDoc()->m_pAmtData->m_CurModelInfo.strDeviceName = m_lblDeviceName.get_Caption();
			GetDoc()->m_pAmtData->m_CurModelInfo.strGripper1 = m_lblGripper1.get_Caption();
			GetDoc()->m_pAmtData->m_CurModelInfo.strGripper2 = m_lblGripper2.get_Caption();
			GetDoc()->m_pAmtData->m_CurModelInfo.strGripper3 = m_lblGripper3.get_Caption();
			GetDoc()->m_pAmtData->m_CurModelInfo.strGripper4 = m_lblGripper4.get_Caption();
			GetDoc()->m_pAmtData->m_CurModelInfo.strResource = m_lblContactType.get_Caption();

			GetDoc()->m_pAmtData->m_CurModelInfo.iContactType = m_cmbContactType.GetCurSel();

			nContactNum = m_cmbContactNum.GetCurSel();
			if(nContactNum == 3)
			{
				nContactNum = 4;
				GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum = nContactNum;
			}
			else
				GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum = nContactNum;

			if((nContactNum == 3) || (nContactNum == 2))
				GetDoc()->m_pAmtData->m_CurModelInfo.iContactor = 2;
			else
				GetDoc()->m_pAmtData->m_CurModelInfo.iContactor = m_cmbContactSel.GetCurSel();

			strAlignScore = m_lblScoreAlign.get_Caption();

			GetDoc()->m_pAmtData->m_CurModelInfo.iAlignScore = atoi(strAlignScore.Left(2));

			GetDoc()->m_pAmtData->m_CurModelInfo.bAlignAngle = bTiltChk;

			nBLUModel = m_cmbBLUModel.GetCurSel();
			nBLUModel = nBLUModel--;

			if(nBLUModel <= 0)
				GetDoc()->m_pAmtData->m_CurModelInfo.m_nBluModel = 0;
			else
				GetDoc()->m_pAmtData->m_CurModelInfo.m_nBluModel = nBLUModel;
	
			strModelName.Format("%s",m_lblDeviceName.get_Caption());

			GetDoc()->m_pAmtData->m_CurModelInfo.strModelName = strModelName;
			m_lblModelName.put_Caption(strModelName);

			GetDoc()->SaveModelFile();
		}
	}
	else if(m_chkModelChange.get_Active() == TRUE)
	{
		if(m_cmbContactNum.GetCurSel() == 0)
		{
			AfxMessageBox("Has not been selected.");
			return;
		}
		if((m_cmbContactNum.GetCurSel() == 1))
		{
			if(m_cmbContactSel.GetCurSel() == 0)
			{
				AfxMessageBox("Has not been selected.");
			}
		}

		strDeviceName = m_lblDeviceName.get_Caption();
		GetDoc()->OpenModelFile(strDeviceName);

		GetDoc()->m_pAmtData->m_CurModelInfo.iPanelSize = atoi(m_lblPanelSize.get_Caption());
		GetDoc()->m_pAmtData->m_CurModelInfo.strDeviceName = m_lblDeviceName.get_Caption();
		GetDoc()->m_pAmtData->m_CurModelInfo.strGripper1 = m_lblGripper1.get_Caption();
		GetDoc()->m_pAmtData->m_CurModelInfo.strGripper2 = m_lblGripper2.get_Caption();
		GetDoc()->m_pAmtData->m_CurModelInfo.strGripper3 = m_lblGripper3.get_Caption();
		GetDoc()->m_pAmtData->m_CurModelInfo.strGripper4 = m_lblGripper4.get_Caption();
		GetDoc()->m_pAmtData->m_CurModelInfo.strResource = m_lblContactType.get_Caption();

		GetDoc()->m_pAmtData->m_CurModelInfo.iContactType = m_cmbContactType.GetCurSel();


		nContactNum = m_cmbContactNum.GetCurSel();
		if(nContactNum == 3)
		{
			nContactNum = 4;
			GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum = nContactNum;
		}
		else
			GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum = nContactNum;

		if((nContactNum == 3) || (nContactNum == 2))
			GetDoc()->m_pAmtData->m_CurModelInfo.iContactor = 0;
		else
			GetDoc()->m_pAmtData->m_CurModelInfo.iContactor = m_cmbContactSel.GetCurSel();

		strAlignScore = m_lblScoreAlign.get_Caption();

		GetDoc()->m_pAmtData->m_CurModelInfo.iAlignScore = atoi(strAlignScore.Left(2));

		GetDoc()->m_pAmtData->m_CurModelInfo.bAlignAngle = bTiltChk;

		nBLUModel = m_cmbBLUModel.GetCurSel();
		nBLUModel = nBLUModel--;

		if(nBLUModel <= 0)
			GetDoc()->m_pAmtData->m_CurModelInfo.m_nBluModel = 0;
		else
			GetDoc()->m_pAmtData->m_CurModelInfo.m_nBluModel = nBLUModel;

		strModelName.Format("%s", m_lblDeviceName.get_Caption());

		GetDoc()->m_pAmtData->m_CurModelInfo.strModelName = strModelName;
		m_lblModelName.put_Caption(strModelName);

		GetDoc()->SaveModelFile();

		nRow = m_gridModelList.get_Row();
		if(nRow != NULL)
		{
			CString strTemp, strPath, strPath2, str;
			strTemp.Format("%s", m_gridModelList.get_TextMatrix(nRow, 1));
			str.Format("%s", m_lblDeviceName.get_Caption());
			if(GetDoc()->m_pAmtData->m_CurModelInfo.strModelName != strTemp)
			{
				strPath.Format("%s\\%s", GetApp()->m_strRecipePath, strTemp);
				strPath2.Format("%s\\%s", GetApp()->m_strRecipePath, str);
				DeletelINIFile(strPath);
				CFileSupport::CopyDirectory(strPath, strPath2);
				DeleteAllFile(strPath);
				RemoveDirectory(strPath);
			}
		}
		else
		{
			AfxMessageBox("There is no selected file");
		}	
	}
	else
	{
		AfxMessageBox("Did not select anything.");
	}

	GetModelToFile();
	InitModel();

	m_chkModelCreate.put_Active(FALSE);
	m_chkModelChange.put_Active(FALSE);
}

void CDlgModelChange::ClickLabelHandleScale1()
{
	CDlgNumPad dlg;

	if( dlg.DoModal() == IDOK )
	{
		CString strText;

		strText.Format("%s%s", dlg.m_strEditText, "mm");
		if( strText !="" )	m_lblGripper1.put_Caption(strText);
	}
}

void CDlgModelChange::ClickLabelHandleScale2()
{
	CDlgNumPad dlg;

	if( dlg.DoModal() == IDOK )
	{
		CString strText;

		strText.Format("%s%s", dlg.m_strEditText, "mm");
		if( strText !="" )	m_lblGripper2.put_Caption(strText);
	}
}

void CDlgModelChange::ClickLabelHandleScale3()
{
	CDlgNumPad dlg;

	if( dlg.DoModal() == IDOK )
	{
		CString strText;

		strText.Format("%s%s", dlg.m_strEditText, "mm");
		if( strText !="" )	m_lblGripper3.put_Caption(strText);
	}
}

void CDlgModelChange::ClickLabelHandleScale4()
{
	CDlgNumPad dlg;

	if( dlg.DoModal() == IDOK )
	{
		CString strText;

		strText.Format("%s%s", dlg.m_strEditText, "mm");
		if( strText !="" )	m_lblGripper4.put_Caption(strText);
	}
}

void CDlgModelChange::ApplyData(void)
{
	GetDoc()->m_pAmtData->m_CurModelInfo.strModelName		=		m_strModelName;
	GetDoc()->m_pAmtData->m_CurModelInfo.iPanelSize			=		atoi(m_strPanelSize);
	GetDoc()->m_pAmtData->m_CurModelInfo.iContactType		=		m_nContactType;
	GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum		=		m_nContactNum;
	GetDoc()->m_pAmtData->m_CurModelInfo.iContactor			=		m_nContactSel;
	GetDoc()->m_pAmtData->m_CurModelInfo.strDeviceName		=		m_strDeviceName;
	GetDoc()->m_pAmtData->m_CurModelInfo.strResource		=		m_strResource;
	GetDoc()->m_pAmtData->m_CurModelInfo.strGripper1		=		m_strGripper1;
	GetDoc()->m_pAmtData->m_CurModelInfo.strGripper2		=		m_strGripper2;
	GetDoc()->m_pAmtData->m_CurModelInfo.strGripper3		=		m_strGripper3;
	GetDoc()->m_pAmtData->m_CurModelInfo.strGripper4		=		m_strGripper4;
	GetDoc()->m_pAmtData->m_CurModelInfo.iAlignScore		=		atoi(m_strAlignScore);
	GetDoc()->m_pAmtData->m_CurModelInfo.m_nBluModel		=		m_nBLUModel;

	GetDoc()->OpenModelFile(m_strDeviceName);
	GetDoc()->SaveCurrentModelName();
}

void CDlgModelChange::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDlgModelChange::CreateGridTitle()
{
	m_gridModelList.put_Cols(2);
	m_gridModelList.put_Rows(1);
	m_gridModelList.put_TextMatrix(0,0,"No.");
	m_gridModelList.put_ColWidth(0, 700);
	m_gridModelList.put_ColAlignment(0,4);

	m_gridModelList.put_TextMatrix(0,1,"Device Name");
	m_gridModelList.put_ColWidth(1, 5315);
	m_gridModelList.put_ColAlignment(1,4);

 	m_gridModelList.put_SelectionMode(CMsflexgrid_pos::flexSelectionFree);
}

void CDlgModelChange::SelectModel( int nIndex )
{
	m_gridModelList.put_Row(m_nOldSelRow);
	for(int i = 1; i < 2; i++)
	{
		m_gridModelList.put_Col(i);
		m_gridModelList.put_CellBackColor(Color(Color::White).ToCOLORREF());
	}
	m_gridModelList.put_Row(nIndex);
	for(int i = 1; i < 2; i++)
	{
		m_gridModelList.put_Col(i);
		m_gridModelList.put_CellBackColor(Color(Color::Lime).ToCOLORREF());
	}
	m_nOldSelRow = nIndex;
}
