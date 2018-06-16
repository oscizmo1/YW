// DlgEngineerDownCode.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "DlgEngineerDownCode.h"
#include "Color.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEngineerDownCode dialog


CDlgEngineerDownCode::CDlgEngineerDownCode(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEngineerDownCode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEngineerDownCode)
	m_nCode = 0;
	m_strText = _T("");
	//}}AFX_DATA_INIT
	m_nTotalNum = 0;
}


void CDlgEngineerDownCode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEngineerDownCode)
	DDX_Control(pDX, IDC_MSFLEXGRID_CODE, m_gridCode);
	DDX_Text(pDX, IDC_EDIT_CODE, m_nCode);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_strText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEngineerDownCode, CDialog)
	//{{AFX_MSG_MAP(CDlgEngineerDownCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEngineerDownCode message handlers

BOOL CDlgEngineerDownCode::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ini.SetIniFilename(ENGINEER_DOWN_CODE_PATH);

	LoadEngineerDownCode();	

	m_gridCode.put_Row(1);
	m_gridCode.put_Col(0);
	m_gridCode.put_CellBackColor(LIGHTGREEN);
	m_gridCode.put_Col(1);
	m_gridCode.put_CellBackColor(LIGHTGREEN);
	m_nCode = atoi(m_gridCode.get_TextMatrix(1, 0));
	m_strText = m_gridCode.get_TextMatrix(1, 1);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEngineerDownCode::LoadEngineerDownCode()
{
	// INI Read
	m_nTotalNum = m_ini.GetLong("Total", "Num");
	
	m_gridCode.put_Redraw(FALSE);
	m_gridCode.Clear();
	m_gridCode.put_Rows(1);
	m_gridCode.put_Cols(2);
	m_gridCode.put_TextMatrix(0, 0, "CODE");
	m_gridCode.put_TextMatrix(0, 1, "내용");
	m_gridCode.put_ColWidth(0, 800);
	m_gridCode.put_ColWidth(1, 9200);
	
	m_gridCode.put_Row(0);
	m_gridCode.put_Col(0);
	m_gridCode.put_CellFontSize(12.f);
	m_gridCode.put_Row(0);
	m_gridCode.put_Col(1);
	m_gridCode.put_CellFontSize(12.f);
	m_gridCode.put_AllowBigSelection(FALSE);
	m_gridCode.put_Cols(2);
	m_gridCode.put_ScrollBars(2);
	m_gridCode.put_ColAlignment(1, 1);
	m_gridCode.put_RowHeight(0, 400);

	CString szEntry, strItem, strText;
	int nCode;
	long nIndex = 0;
	for(int i = 0 ; i < m_nTotalNum ; i++)
	{
		szEntry.Format("List%03d", i);
		nCode = m_ini.GetLong("Code", szEntry);
		strText = m_ini.GetString("Text", szEntry);
		strItem.Format("%d\t%s", nCode, strText);
		nIndex = i+1;
		m_gridCode.AddItem(strItem, _variant_t( nIndex ));
		m_gridCode.put_RowHeight(nIndex, 400);
		m_gridCode.put_Row(nIndex);
		m_gridCode.put_Col(0);
		m_gridCode.put_CellFontSize(12.f);
		m_gridCode.put_Col(1);
		m_gridCode.put_CellFontSize(12.f);
	}
	
	m_gridCode.put_Sort(0);
	
	m_gridCode.put_Redraw(TRUE);

	m_nBackColor = m_gridCode.get_CellBackColor();
}

void CDlgEngineerDownCode::SaveEngineerDownCode()
{
	// INI파일에서 현재 내용을 싹 지운다.
	m_ini.DeleteSection("Total");
	m_ini.DeleteSection("Code");
	m_ini.DeleteSection("Text");
	
	CString szEntry;
	
	m_ini.WriteLong("Total", "Num", m_nTotalNum);
	
	int nRow = 0;
	for(int i = 0 ; i < m_nTotalNum ; i++)
	{
		nRow = i+1;
		szEntry.Format("List%03d", i);
		m_ini.WriteLong("Code", szEntry, atoi(m_gridCode.get_TextMatrix(nRow, 0)));
		m_ini.WriteString("Text", szEntry, m_gridCode.get_TextMatrix(nRow, 1));
	}
}

BEGIN_EVENTSINK_MAP(CDlgEngineerDownCode, CDialog)
    //{{AFX_EVENTSINK_MAP(CDlgEngineerDownCode)
	ON_EVENT(CDlgEngineerDownCode, IDC_CMD_INSERT, -600 /* Click */, OnClickCmdInsert, VTS_NONE)
	ON_EVENT(CDlgEngineerDownCode, IDC_CMD_DELETE, -600 /* Click */, OnClickCmdDelete, VTS_NONE)
	ON_EVENT(CDlgEngineerDownCode, IDC_CMD_EDIT, -600 /* Click */, OnClickCmdEdit, VTS_NONE)
	ON_EVENT(CDlgEngineerDownCode, IDC_CMD_SELECT, -600 /* Click */, OnClickCmdSelect, VTS_NONE)
	ON_EVENT(CDlgEngineerDownCode, IDC_MSFLEXGRID_CODE, -600 /* Click */, OnClickMsflexgridCode, VTS_NONE)
	ON_EVENT(CDlgEngineerDownCode, IDC_MSFLEXGRID_CODE, -601 /* DblClick */, OnDblClickMsflexgridCode, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDlgEngineerDownCode::OnClickCmdInsert() 
{
	UpdateData(TRUE);

	if(m_nTotalNum >= 999) return;
	if(m_nCode == 0 )
		return;
	if(m_strText.GetLength() > 40 )
		m_strText = m_strText.Left(40);
	
	m_gridCode.put_Redraw(FALSE);
	// 기존에 이미 있는지 확인한다.
	long totalRow = m_gridCode.get_Rows();
	long nIndex;
	for(nIndex = 1 ; nIndex < totalRow ; nIndex++)
	{
		if(m_nCode == atoi(m_gridCode.get_TextMatrix(nIndex, 0)))
		{
			m_gridCode.put_Row(nIndex);
			m_gridCode.put_Redraw(TRUE);
			return;
		}
	}
	
	// 마지막줄에 추가하고 새로 정렬한다.
	CString strItem;
	strItem.Format("%d\t%s", m_nCode, m_strText);
	nIndex = m_gridCode.get_Rows();
	m_gridCode.AddItem(strItem, _variant_t( nIndex ));
	m_gridCode.put_RowHeight(nIndex, 400);
	m_gridCode.put_Row(nIndex);
	m_gridCode.put_Col(0);
	m_gridCode.put_CellFontSize(12.f);
	m_gridCode.put_Col(1);
	m_gridCode.put_CellFontSize(12.f);
	m_gridCode.put_Sort(0);
	
	// 새로 추가된 항목 선택
	totalRow = m_gridCode.get_Rows();
	for(nIndex = 1 ; nIndex < totalRow ; nIndex++)
	{
		if(m_nCode == atoi(m_gridCode.get_TextMatrix(nIndex, 0)))
		{
			m_gridCode.put_Row(nIndex);
			break;
		}
	}
	m_gridCode.put_Redraw(TRUE);
	OnClickMsflexgridCode();
	
	// 총 수량 갱신
	m_nTotalNum = (totalRow-1);

	// File Save
	SaveEngineerDownCode();
}

void CDlgEngineerDownCode::OnClickCmdDelete() 
{
	CString szEntry, szValue, szText;
	
	szValue.Format("%s번 CODE를 제거하시겠습니까?", m_gridCode.get_TextMatrix(m_gridCode.get_Row(), 0));
	if(AfxMessageBox(szValue, MB_YESNO) == IDNO)
		return;
	
	// 원하는 항목을 리스트에서 삭제한다
	long nRow = m_gridCode.get_Row();
	if((nRow < 1) || (nRow >= m_gridCode.get_Rows()))
		return;
	m_gridCode.put_Redraw(FALSE);
	m_gridCode.RemoveItem(nRow);
	m_gridCode.put_Redraw(TRUE);
	m_gridCode.Refresh();
	
	// 총 수량 갱신
	m_nTotalNum = (m_gridCode.get_Rows()-1);
	
	// File Save
	SaveEngineerDownCode();

	m_gridCode.put_Row(0);
	m_nCode = 0;
	m_strText = "";
	UpdateData(FALSE);
}

void CDlgEngineerDownCode::OnClickCmdEdit() 
{
	UpdateData(TRUE);
	int nRow = m_gridCode.get_Row();
	CString strCode;
	strCode.Format("%d", m_nCode);
	m_gridCode.put_TextMatrix(nRow, 0, strCode);
	m_gridCode.put_TextMatrix(nRow, 1, m_strText);

	SaveEngineerDownCode();
}

void CDlgEngineerDownCode::OnClickCmdSelect() 
{
	GetDoc()->SetLog("유실 에러창 O K 버튼 누름",LOG_COMM);
	UpdateData(TRUE);
	if(m_nCode == 0)
		return;
	// 유실보고
	OnOK();
}

void CDlgEngineerDownCode::OnClickMsflexgridCode() 
{
	int nRow = m_gridCode.get_Row();
	m_nCode = atoi(m_gridCode.get_TextMatrix(nRow, 0));
	m_strText = m_gridCode.get_TextMatrix(nRow, 1);
	UpdateData(FALSE);

	m_gridCode.put_Redraw(FALSE);
	int totalRow = m_gridCode.get_Rows();
	for(int nIndex = 1 ; nIndex < totalRow ; nIndex++)
	{
		m_gridCode.put_Row(nIndex);
		if(nRow == nIndex)
		{
			m_gridCode.put_Col(0);
			m_gridCode.put_CellBackColor(LIGHTGREEN);
			m_gridCode.put_Col(1);
			m_gridCode.put_CellBackColor(LIGHTGREEN);
		}
		else
		{
			m_gridCode.put_Col(0);
			m_gridCode.put_CellBackColor(m_nBackColor);
			m_gridCode.put_Col(1);
			m_gridCode.put_CellBackColor(m_nBackColor);
		}
	}
	m_gridCode.put_Row(nRow);
	m_gridCode.put_Redraw(TRUE);
}

void CDlgEngineerDownCode::OnDblClickMsflexgridCode() 
{
	OnClickCmdSelect();
	
}

BOOL CDlgEngineerDownCode::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN) 
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
    }	
	
	return CDialog::PreTranslateMessage(pMsg);
}
