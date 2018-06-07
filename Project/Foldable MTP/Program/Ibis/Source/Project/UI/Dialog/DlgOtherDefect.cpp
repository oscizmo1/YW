// DlgDoor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgOtherDefect.h"


// CDlgOtherDefect 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgOtherDefect, CDialog)

CDlgOtherDefect::CDlgOtherDefect(JIG_CH ChannelNO, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOtherDefect::IDD, pParent)
{
	m_nLastGroupGridRow = 1;
	m_nLastElementGridRow = 1;
	m_eChannelNO = ChannelNO;
}

CDlgOtherDefect::~CDlgOtherDefect()
{
}

void CDlgOtherDefect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_DEFECT_GROUP, m_GridGroup);	
	DDX_Control(pDX, IDC_GRID_DEFECT_ELEMENT, m_GridElement);	
}


BEGIN_MESSAGE_MAP(CDlgOtherDefect, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_GRID_DEFECT_GROUP, OnGridGroupSelChange)
	ON_NOTIFY(NM_CLICK, IDC_GRID_DEFECT_ELEMENT, OnGridElementSelChange)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_DEFECT_CANCLE, &CDlgOtherDefect::OnClickedButtonDefectCancle)
	ON_BN_CLICKED(IDC_BUTTON_DEFECT_CONFIRM, &CDlgOtherDefect::OnClickedButtonDefectConfirm)
	ON_BN_CLICKED(IDC_BUTTON_GOOD, &CDlgOtherDefect::OnBnClickedButtonGood)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDlgOtherDefect, CDialog)
END_EVENTSINK_MAP()


	
BOOL CDlgOtherDefect::OnInitDialog()
{
	CDialog::OnInitDialog();
	InitGrid();
	InitGridElement();
	UpdateGroup();
	UpdateGridElement();

	return TRUE; 
}


void CDlgOtherDefect::InitGrid()
{
	m_GridGroup.SetEditable(FALSE);
	m_GridGroup.SetDefCellHeight(50);
	m_GridGroup.SetFixedColumnCount(1); 
	m_GridGroup.SetFixedRowCount(1);
	m_GridGroup.SetRowCount(theConfigBank.m_BZoneDefect.m_rDefectList.nGroupCount+1);
	m_GridGroup.SetColumnCount(2);
	m_GridGroup.SetRowResize(FALSE);
	m_GridGroup.SetColumnResize(FALSE);		
	m_GridGroup.SetListMode(FALSE);
	m_GridGroup.EnableSelection(FALSE);

	m_GridGroup.SetColumnWidth(0,55);
	m_GridGroup.SetColumnWidth(1,270);
	m_GridGroup.SetRowHeight(0,35);

	//이름
	m_GridGroup.SetItemText(0,0,_T("NO"));
	m_GridGroup.SetItemText(0,1,_T("NAME"));

	DWORD dwTextStyle = DT_RIGHT|DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_SINGLELINE;

	for(int i = 0; i < m_GridGroup.GetRowCount(); i++ )
	{
		m_GridGroup.SetItemFormat(i, 0, dwTextStyle);
		m_GridGroup.SetItemFormat(i, 1, dwTextStyle);
	}

}

void CDlgOtherDefect::InitGridElement()
{
	m_GridElement.SetEditable(FALSE);
	m_GridElement.SetDefCellHeight(37);
	m_GridElement.SetFixedColumnCount(1); 
	m_GridElement.SetFixedRowCount(1);
	m_GridElement.SetRowCount(21/*theConfigBank.m_BZoneDefect.m_rDefectList.nDefectCount[0]*/);
	m_GridElement.SetColumnCount(5);
	m_GridElement.SetRowResize(FALSE);
	m_GridElement.SetColumnResize(FALSE);		
	m_GridElement.SetListMode(FALSE);
	m_GridElement.EnableSelection(FALSE);

	m_GridElement.SetColumnWidth(0,49);
	m_GridElement.SetColumnWidth(1,200);
	m_GridElement.SetColumnWidth(2,200);
	m_GridElement.SetColumnWidth(3,200);
	m_GridElement.SetColumnWidth(4,60);
	m_GridElement.SetRowHeight(0,35);

	//이름
	m_GridElement.SetItemText(0,0,_T("NO"));
	m_GridElement.SetItemText(0,1,_T("이름"));
	m_GridElement.SetItemText(0,2,_T("NAME"));
	m_GridElement.SetItemText(0,3,_T("NAME"));
	m_GridElement.SetItemText(0,4,_T("CODE"));

	DWORD dwTextStyle = DT_RIGHT|DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_SINGLELINE;

	for(int i = 0; i < m_GridElement.GetRowCount(); i++ )
	{
		m_GridElement.SetItemFormat(i, 0, dwTextStyle);
		m_GridElement.SetItemFormat(i, 1, dwTextStyle);
		m_GridElement.SetItemFormat(i, 2, dwTextStyle);
		m_GridElement.SetItemFormat(i, 3, dwTextStyle);
		m_GridElement.SetItemFormat(i, 4, dwTextStyle);
	}


}

void CDlgOtherDefect::UpdateGridElement()
{
	CString szNum = _T("");
	CString szInput1 = _T("");	
	CString szInput2 = _T("");	
	CString szInput3 = _T("");	
	CString szInput4 = _T("");	

	for(int i = 0; i < m_GridElement.GetRowCount(); i++ )
	{
		if ( i == 0 )	continue;

		szNum.Format(_T("%d"),i);
		if((UINT)i < theConfigBank.m_BZoneDefect.m_rDefectList.nDefectCount[m_nLastGroupGridRow-1]+1)
		{
			szInput1 = theConfigBank.m_BZoneDefect.m_rDefectList.rDefectData[m_nLastGroupGridRow-1][i-1].sDefect[0];
			szInput2 = theConfigBank.m_BZoneDefect.m_rDefectList.rDefectData[m_nLastGroupGridRow-1][i-1].sDefect[1];
			szInput3 = theConfigBank.m_BZoneDefect.m_rDefectList.rDefectData[m_nLastGroupGridRow-1][i-1].sDefect[2];
			szInput4 = theConfigBank.m_BZoneDefect.m_rDefectList.rDefectData[m_nLastGroupGridRow-1][i-1].sCode;
		}
		else
		{
			szInput1.Empty();
			szInput2.Empty();
			szInput3.Empty();
			szInput4.Empty();
		}
		m_GridElement.SetItemText(i, 0, szNum.GetBuffer());
		m_GridElement.SetItemText(i, 1, szInput1.GetBuffer());
		m_GridElement.SetItemText(i, 2, szInput2.GetBuffer());
		m_GridElement.SetItemText(i, 3, szInput3.GetBuffer());
		m_GridElement.SetItemText(i, 4, szInput4.GetBuffer());
	}
		
	HighlightCurElemet(FALSE);
	m_GridElement.Refresh();
	//SetTimer(DEFECT_GROUP_TIMER, 100, NULL);
}

void CDlgOtherDefect::UpdateGridElementRefersh()
{
	CString szNum = _T("");
	CString szInput1 = _T("");	
	CString szInput2 = _T("");	
	CString szInput3 = _T("");	

	for(UINT i = 0; i < theConfigBank.m_BZoneDefect.m_rDefectList.nDefectCount[m_nLastGroupGridRow-1]+1; i++ )
	{
		if ( i == 0 )	continue;

		szNum.Format(_T("%d"),i);
		m_GridElement.SetItemText(i, 0, szNum.GetBuffer());
		m_GridElement.SetItemText(i, 1, szInput1.GetBuffer());
		m_GridElement.SetItemText(i, 2, szInput2.GetBuffer());
		m_GridElement.SetItemText(i, 3, szInput3.GetBuffer());
	}

	m_GridElement.Refresh();
}

void CDlgOtherDefect::UpdateGroup()
{
#if 1
	CString szNum = _T("");
	CString szInput = _T("");	

	for(int i = 0; i < m_GridGroup.GetRowCount(); i++ )
	{
		if ( i == 0 )	continue;

		szNum.Format(_T("%d"),i);
		//szInput = theIOBank.GetInputIOParma((INPUT_ID)m_vecDoor[i-1].nIn).m_strIOName;
		szInput = theConfigBank.m_BZoneDefect.m_rDefectList.sGroupName[i-1][GetMainHandler()->m_nLangIdx];
		m_GridGroup.SetItemText(i, 0, szNum.GetBuffer());
		m_GridGroup.SetItemText(i, 1, szInput.GetBuffer());
	}

	//m_GridGroup.ExpandColumnsToFit();
	HighlightCurGroup(FALSE);
	m_GridGroup.Refresh();
#endif
}
//===============================================================================================================================
//===============================================================================================================================

BOOL CDlgOtherDefect::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
		return TRUE;
	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}



void CDlgOtherDefect::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);

	if(nIDEvent == DEFECT_GROUP_TIMER)
	{
		UpdateGroup();
		UpdateGridElement();
		SetTimer(DEFECT_GROUP_TIMER, 100,NULL);
	}

	CDialog::OnTimer(nIDEvent);
}




//===============================================================================================================================
//===============================================================================================================================

//===============================================================================================================================
//===============================================================================================================================
void CDlgOtherDefect::CurGroupChange( int nIndex)
{	
	/*
	m_TeachGrid.SetEditable(TRUE);							//	Teach Grid Clear해줌
	CCellRange cRange;
	cRange.Set(1,0,5,4);
	m_TeachGrid.ClearCells(cRange);
	m_TeachGrid.SetEditable(FALSE);		
	m_nCurTeachRow = -1;
	HighlightCurTeach(FALSE);

	m_CurAxis = axis;
	m_JogSpeed = JOG_LOW;	// 축이 바뀌면 조그 속도를 저속으로 바꾼다

	// 이전 선택한 Row를 지우고 새로 클릭한 Row를 색칠한다
	HighlightCurAxis(TRUE);

	UpdateMotorParam();

	UpdateTeachGrid();
	*/

}

void CDlgOtherDefect::OnGridGroupSelChange(NMHDR *pNotifyStruct, LRESULT *pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;	

	if ( pItem->iRow < 1 )		return;				// 메뉴 정의된 Row Click시 return	

	
	CString szClickedNumber = m_GridGroup.GetItemText(pItem->iRow,0);		//	클릭된 메뉴의 NO 값을 가져와서 int로 변경
	int iNum = _ttoi(szClickedNumber) - 1;
	
	m_nLastGroupGridRow = iNum+1;
	UpdateGridElement();
	HighlightCurGroup(TRUE);

	*pResult = 0;
}

void CDlgOtherDefect::OnGridElementSelChange(NMHDR *pNotifyStruct, LRESULT *pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;	

	if ( pItem->iRow < 1 )		return;				// 메뉴 정의된 Row Click시 return	


	CString szClickedNumber = m_GridElement.GetItemText(pItem->iRow,0);		//	클릭된 메뉴의 NO 값을 가져와서 int로 변경
	int iNum = _ttoi(szClickedNumber) - 1;
		
	m_nLastElementGridRow = iNum+1;
	HighlightCurElemet(TRUE);

	*pResult = 0;
}


void CDlgOtherDefect::HighlightCurGroup( BOOL bRefrash )
{
	// 이전 선택한 Row를 지우고 새로 클릭한 Row를 색칠한다

	for(UINT i = 0 ; i < theConfigBank.m_BZoneDefect.m_rDefectList.nGroupCount ; i++)
		m_GridGroup.SetItemBkColour(i+1, 1, Color(Color::White).ToCOLORREF());

	if( (m_nLastGroupGridRow >0) && (m_nLastGroupGridRow < m_GridGroup.GetRowCount()) )
		m_GridGroup.SetItemBkColour(m_nLastGroupGridRow, 1, Color(Color::LightSkyBlue).ToCOLORREF());

	if(bRefrash)	m_GridGroup.Refresh();
}

void CDlgOtherDefect::HighlightCurElemet( BOOL bRefrash )
{
	// 이전 선택한 Row를 지우고 새로 클릭한 Row를 색칠한다

	for(UINT i = 1 ; i < 16 ; i++)
	{
		m_GridElement.SetItemBkColour(i, 1, Color(Color::White).ToCOLORREF());
		m_GridElement.SetItemBkColour(i, 2, Color(Color::White).ToCOLORREF());
		m_GridElement.SetItemBkColour(i, 3, Color(Color::White).ToCOLORREF());
	}

	if( (m_nLastElementGridRow >0) && (m_nLastElementGridRow < m_GridElement.GetRowCount()) )
	{
		m_GridElement.SetItemBkColour(m_nLastElementGridRow, 1, Color(Color::LightSkyBlue).ToCOLORREF());
		m_GridElement.SetItemBkColour(m_nLastElementGridRow, 2, Color(Color::LightSkyBlue).ToCOLORREF());
		m_GridElement.SetItemBkColour(m_nLastElementGridRow, 3, Color(Color::LightSkyBlue).ToCOLORREF());
	}

	if(bRefrash)	m_GridElement.Refresh();
}


void CDlgOtherDefect::OnClickedButtonDefectCancle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EndDialog(IDCANCEL);
}


void CDlgOtherDefect::OnClickedButtonDefectConfirm()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theProcBank.iMapBZoneSkipDefect.at(m_eChannelNO).FinalDefectName[LAN_KOR] = m_GridElement.GetItemText(m_nLastElementGridRow, LAN_KOR+1);
	theProcBank.iMapBZoneSkipDefect.at(m_eChannelNO).FinalDefectName[LAN_ENG] = m_GridElement.GetItemText(m_nLastElementGridRow, LAN_ENG+1);
	theProcBank.iMapBZoneSkipDefect.at(m_eChannelNO).FinalDefectName[LAN_VNM] = m_GridElement.GetItemText(m_nLastElementGridRow, LAN_VNM+1);
	EndDialog(IDOK);
}


void CDlgOtherDefect::OnBnClickedButtonGood()
{
	theProcBank.iMapBZoneSkipDefect.at(m_eChannelNO).FinalDefectName[LAN_KOR].Empty();
	theProcBank.iMapBZoneSkipDefect.at(m_eChannelNO).FinalDefectName[LAN_ENG].Empty();
	theProcBank.iMapBZoneSkipDefect.at(m_eChannelNO).FinalDefectName[LAN_VNM].Empty();
	EndDialog(IDOK);
}
