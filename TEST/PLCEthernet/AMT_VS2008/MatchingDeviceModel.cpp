// MatchingDeviceModel.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "MatchingDeviceModel.h"

using namespace Gdiplus;	// Color class


// CMatchingDeviceModel dialog

IMPLEMENT_DYNAMIC(CMatchingDeviceModel, CDialog)

CMatchingDeviceModel::CMatchingDeviceModel(CWnd* pParent /*=NULL*/)
	: CDialog(CMatchingDeviceModel::IDD, pParent)
{
	m_strDeviceID = _T("");
	m_strModelName = _T("");
	m_nOldSelRow = 0;
}

CMatchingDeviceModel::~CMatchingDeviceModel()
{
}

void CMatchingDeviceModel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ILABELX_DEVICE_ID, m_lblDeviceID);
	DDX_Control(pDX, IDC_GRID_MODEL_NAME, m_gridModelName);
	DDX_Control(pDX, IDC_BTN_CREATE, m_btnCreate);
}


BEGIN_MESSAGE_MAP(CMatchingDeviceModel, CDialog)
	ON_BN_CLICKED(IDC_BTN_NOT_SELECT, &CMatchingDeviceModel::OnBnClickedBtnNotSelect)
	ON_BN_CLICKED(IDC_BTN_SELECT, &CMatchingDeviceModel::OnBnClickedBtnSelect)
	ON_BN_CLICKED(IDC_BTN_CREATE, &CMatchingDeviceModel::OnBnClickedBtnCreate)
END_MESSAGE_MAP()


// CMatchingDeviceModel message handlers

void CMatchingDeviceModel::OnBnClickedBtnNotSelect()
{
	OnCancel();
}

void CMatchingDeviceModel::OnBnClickedBtnSelect()
{
	OnOK();
}

void CMatchingDeviceModel::SetDeviceID( CString strDeviceID )
{
	m_strDeviceID = strDeviceID;
}
BOOL CMatchingDeviceModel::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lblDeviceID.put_Caption(m_strDeviceID);

	// 이전 항목 삭제
	m_gridModelName.Clear();
	m_gridModelName.put_Rows(1);
	m_gridModelName.put_Cols(1);
	m_gridModelName.put_TextMatrix(0,0,"Model Name");
	m_gridModelName.put_ColWidth(0, 5400);
	m_gridModelName.put_ColAlignment(0,4);
	m_gridModelName.put_SelectionMode(CMsflexgrid_pos::flexSelectionFree);

	CString strPath, strTemp, strModelName;
	strPath.Format("%s\\*.*", GetApp()->m_strRecipePath );

	CFileFind finder;
	BOOL bfind = finder.FindFile(strPath);
	long a = 1;

	while(bfind)
	{
		bfind = finder.FindNextFile();

		if( finder.IsDots() )
			continue;

		if( finder.IsDirectory() )
		{
			strTemp = finder.GetFileName();
			strModelName.Format("%s", strTemp);
			m_gridModelName.AddItem(strModelName, _variant_t(a++));
			m_gridModelName.put_RowHeight(a-1, 700);
			m_gridModelName.put_FontSize(12);
			m_gridModelName.put_FontBold(TRUE);
		}
	}

	finder.Close();
	m_gridModelName.put_FixedRows(1);

	// 옵션 설정시에만 Create 버튼 표시
	if(GetDoc()->m_bUseAutoCreate)
		m_btnCreate.ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

CString CMatchingDeviceModel::GetModelName()
{
	return m_strModelName;
}

void CMatchingDeviceModel::SelectModel( int nIndex )
{
	m_gridModelName.put_Row(m_nOldSelRow);
	m_gridModelName.put_Col(0);
	m_gridModelName.put_CellBackColor(Color(Color::White).ToCOLORREF());

	m_gridModelName.put_Row(nIndex);
	m_gridModelName.put_Col(0);
	m_gridModelName.put_CellBackColor(Color(Color::Lime).ToCOLORREF());
	m_nOldSelRow = nIndex;

	m_strModelName = m_gridModelName.get_Text();
}
BEGIN_EVENTSINK_MAP(CMatchingDeviceModel, CDialog)
	ON_EVENT(CMatchingDeviceModel, IDC_GRID_MODEL_NAME, DISPID_CLICK, CMatchingDeviceModel::ClickGridModelName, VTS_NONE)
END_EVENTSINK_MAP()

void CMatchingDeviceModel::ClickGridModelName()
{
	int nRow = m_gridModelName.get_MouseRow();
	SelectModel(nRow);
}

void CMatchingDeviceModel::OnBnClickedBtnCreate()
{
	// TODO: Add your control notification handler code here
}
