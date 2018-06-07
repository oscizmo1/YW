// DlgDoor.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DlgDoor.h"
#include "UI\GausGUI\GxMsgBox.h"


// CDlgDoor ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgDoor, CDialog)

CDlgDoor::CDlgDoor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDoor::IDD, pParent)
{
}

CDlgDoor::~CDlgDoor()
{
}

void CDlgDoor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_DOOR_LIST, m_Grid);	
}


BEGIN_MESSAGE_MAP(CDlgDoor, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_GRID_DOOR_LIST, OnGridDoorSelChange)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDlgDoor, CDialog)
	ON_EVENT(CDlgDoor, IDC_GXBTN_DOOR_ALL_OPEN, DISPID_CLICK, CDlgDoor::ClickBtnDoorAllOpen, VTS_NONE)
	ON_EVENT(CDlgDoor, IDC_GXBTN_DOOR_ALL_CLOSE, DISPID_CLICK, CDlgDoor::ClickBtnDoorAllClose, VTS_NONE)
	ON_EVENT(CDlgDoor, IDC_GXBTN_DOOR_CANCEL, DISPID_CLICK, CDlgDoor::ClickBtnDoorCancel, VTS_NONE)
	END_EVENTSINK_MAP()


	
BOOL CDlgDoor::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_bBackDoorOpen = GetMainHandler()->m_bBackDoorOpen;
	InitGrid();
	UpdateGrid();
	SetTimer(ID_DOOR_TIMER, 100,NULL);

	return TRUE; 
}


void CDlgDoor::InitGrid()
{
 	tDoor t;

 	t.nIn = X_FRONT_DOOR1_SENSOR;						t.nOut = Y_FRONT_DOOR1_LOCK_ONOFF;					m_vecDoor.push_back(t);
 	t.nIn = X_FRONT_DOOR2_SENSOR;						t.nOut = Y_FRONT_DOOR2_LOCK_ONOFF;					m_vecDoor.push_back(t);
 	t.nIn = X_RIGNT_DOOR_SENSOR;						t.nOut = Y_RIGNT_DOOR_LOCK_ONOFF;					m_vecDoor.push_back(t);
 	t.nIn = X_BACK_DOOR1_SENSOR;						t.nOut = Y_BACK_DOOR1_LOCK_ONOFF;					m_vecDoor.push_back(t);
	//===============================================================================================================================
	m_Grid.SetEditable(FALSE);
	m_Grid.SetDefCellHeight(50);
	m_Grid.SetFixedColumnCount(1); 
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetRowCount(m_vecDoor.size() + 1);
	m_Grid.SetColumnCount(4);
	m_Grid.SetRowResize(FALSE);
	m_Grid.SetColumnResize(FALSE);		
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableSelection(FALSE);

	m_Grid.SetColumnWidth(0,10);
	m_Grid.SetColumnWidth(1,300);
	m_Grid.SetRowHeight(0,35);

	//�̸�
	m_Grid.SetItemText(0,0,_T("NO"));
	m_Grid.SetItemText(0,1,_T("NAME"));
	m_Grid.SetItemText(0,2,_T("OPEN/CLOSE"));
	m_Grid.SetItemText(0,3,_T("OUTPUT"));

	// Font & Style ����
	DWORD dwTextStyle = DT_RIGHT|DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_SINGLELINE;

	for(int i = 0; i < m_Grid.GetRowCount(); i++ )
	{
		m_Grid.SetItemFormat(i, 0, dwTextStyle);
		m_Grid.SetItemFormat(i, 1, dwTextStyle);
		m_Grid.SetItemFormat(i, 2, dwTextStyle);
		m_Grid.SetItemFormat(i, 3, dwTextStyle);
	}
}

void CDlgDoor::UpdateGrid()
{
	CString szNum = _T("");
	CString szInput = _T("");
	CString szStateIn = _T("");
	CString szStateOut = _T("");

	for(int i = 0; i < m_Grid.GetRowCount(); i++ )
	{
		if ( i == 0 )	continue;

		szNum.Format(_T("%d"),i);
		szInput = theIOBank.GetInputIOParma((INPUT_ID)m_vecDoor[i-1].nIn).m_strIOName;
		szStateIn = theUnitFunc.GetInPutIOCheck((INPUT_ID)m_vecDoor[i-1].nIn) ? _T("OPENED") : _T("CLOSED");
		szStateOut = theUnitFunc.GetOutPutIOCheck((OUTPUT_ID)m_vecDoor[i-1].nOut) ? _T("UNLOCK") : _T("LOCK");

		m_Grid.SetItemText(i, 0, szNum.GetBuffer());
		m_Grid.SetItemText(i, 1, szInput.GetBuffer());
		m_Grid.SetItemText(i, 2, szStateIn.GetBuffer());
		m_Grid.SetItemText(i, 3, szStateOut.GetBuffer());
	}

	m_Grid.ExpandColumnsToFit();
	m_Grid.Refresh();
}


//===============================================================================================================================
//===============================================================================================================================

BOOL CDlgDoor::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
		return TRUE;
	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}



void CDlgDoor::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);

	if(nIDEvent == ID_DOOR_TIMER)
	{
		UpdateGrid();
		SetTimer(ID_DOOR_TIMER, 100,NULL);
	}

	if(m_bBackDoorOpen != GetMainHandler()->m_bBackDoorOpen)
	{
		OnOK();
	}
	CDialog::OnTimer(nIDEvent);
}




//===============================================================================================================================
//===============================================================================================================================
void CDlgDoor::ClickBtnDoorAllOpen()
{
	if(theUnitFunc.AutoTeachKey_TeachCheck() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// ����� �̸��� ����
		dlgMsgBox.SetTitle(_T("�˸�"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			theProcBank.m_strLastKorMsg,										//��
			theProcBank.m_strLastEngMsg,												//��
			theProcBank.m_strLastVnmMsg, GetMainHandler()->m_nLangIdx );		//��

		//int nRet = dlgMsgBox.DoModal();
		//if(nRet != IDOK)
		dlgMsgBox.DoModal();
		return; 
	}
	std::vector<tDoor>::iterator it = m_vecDoor.begin();
	for(it = m_vecDoor.begin(); it != m_vecDoor.end(); it++ )
	{
		//kjpark 20161027 IO Output check bug ����
		theUnitFunc.SetOutPutIO((OUTPUT_ID)it->nOut, ON);
	}	
}


void CDlgDoor::ClickBtnDoorAllClose()
{
	std::vector<tDoor>::iterator it = m_vecDoor.begin();
	for(it = m_vecDoor.begin(); it != m_vecDoor.end(); it++ )
	{
		//kjpark 20161027 IO Output check bug ����
		theUnitFunc.SetOutPutIO((OUTPUT_ID)it->nOut, OFF);
	}	
}


void CDlgDoor::ClickBtnDoorCancel()
{
	EndDialog(IDCANCEL);
}

//===============================================================================================================================
//===============================================================================================================================


void CDlgDoor::OnGridDoorSelChange(NMHDR *pNotifyStruct, LRESULT *pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	int iRow = pItem->iRow;
	if ( iRow < 1 )		return;		//	0���� Ŭ���� return

	//BKH 20161202 AUTO KEY ����Ͻÿ��� ������ ������ ����
	if(theUnitFunc.AutoTeachKey_TeachCheck() == FALSE)
		return; 
	CString szClick = m_Grid.GetItemText(iRow, 1);
	CString szComp = _T("");
	CString szState = _T("");
	CIOParam tParam;

	BOOL	bState = FALSE;

	std::vector<tDoor>::iterator it = m_vecDoor.begin();
	for(it = m_vecDoor.begin(); it != m_vecDoor.end(); it++)
	{
		//bState = theUnitFunc.GetInPutIOCheck((INPUT_ID)it->nIn);
		tParam = theIOBank.GetInputIOParma((INPUT_ID)it->nIn);
		szComp = tParam.m_strIOName;
				
		szState = m_Grid.GetItemText(iRow,3);
		
		if ( szComp.CompareNoCase(szClick.GetBuffer()) == 0 )			//	������ 0�� ����
		{
			if ( szState == _T("LOCK") )
				//kjpark 20161027 IO Output check bug ����
				theUnitFunc.SetOutPutIO((OUTPUT_ID)it->nOut, ON);		//	���� ��״� ��ȣ�� �����ִٸ� UNLOCK ��ȣ�� �ش�
			else if ( szState == _T("UNLOCK") )	
				//kjpark 20161027 IO Output check bug ����
				theUnitFunc.SetOutPutIO((OUTPUT_ID)it->nOut, OFF);		//	���� �������� ��ȣ�� �����ִٸ� LOCK ��ȣ�� �ش�.
				
			break;
		}
	}
}




