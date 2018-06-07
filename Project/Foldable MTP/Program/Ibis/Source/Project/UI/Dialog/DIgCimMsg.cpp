// DIgCimMsg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DIgCimMsg.h"
#include "afxdialogex.h"
#include "UI\GausGUI\GxUICtrl.h"

// CDIgCimMsg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDIgCimMsg, CDialog)

CDIgCimMsg::CDIgCimMsg(CWnd* pParent /*=NULL*/)
	: CDialog(CDIgCimMsg::IDD, pParent)
{
	//m_strID=_T("");
	//m_strMessage=_T("");
}

BOOL CDIgCimMsg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//m_strID=_T("aaabbbb");
	InitList();
	SetLogListUpdate();
	return TRUE;  // return TRUE unless you set the focus to a control
}

CDIgCimMsg::~CDIgCimMsg()
{
}

void CDIgCimMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_GRID_CIM_MSG_LIST, m_GridCimMessage);
	DDX_Control(pDX, IDC_LIST_CIM_MSG_LOGLIST, m_CimLog);
}


BEGIN_MESSAGE_MAP(CDIgCimMsg, CDialog)
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)	
END_MESSAGE_MAP()


// CDIgCimMsg �޽��� ó�����Դϴ�.
BEGIN_EVENTSINK_MAP(CDIgCimMsg, CDialog)
	ON_EVENT(CDIgCimMsg, IDC_GXBTN_CLOSE, DISPID_CLICK, CDIgCimMsg::ClickGxbtnClose, VTS_NONE)
	ON_EVENT(CDIgCimMsg, IDC_GXBTN_BUZZER_OFF, DISPID_CLICK, CDIgCimMsg::ClickGxbtnBuzzerOff, VTS_NONE)
END_EVENTSINK_MAP()

LRESULT CDIgCimMsg::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : ȭ�� ������ ǥ�� �ʱ�ȭ�� ���ְ� ���ſ� Ÿ�̸Ӹ� ����/���� �����ش�.
	BOOL bFlag = (BOOL)wParam;
	// ȭ�� ���� Ÿ�̸Ӹ� �����Ű��� �����ΰ� ?
	if ( bFlag )
	{

	}
	else
	{
		//m_bIsShwoWindow = FALSE;
	}

	return 0;
}
void CDIgCimMsg::ClickGxbtnClose()
{
	OnOK();
}

void CDIgCimMsg::InitList()
{
	LV_COLUMN lCol;
	lCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | TCS_MULTILINE;

	lCol.fmt = LVCFMT_CENTER;   // Į���� ������ �������ķ� ����(LVCFMT_CENTER, LVCFMT_LEFT, LVCFMT_RIGHT�� ����)
	lCol.pszText = _T("MESSAGE");    //  Į���� ������ ����
	lCol.iSubItem = 0;    
	CRect rect;//  ����������� �ε����� ����
	m_CimLog.GetWindowRect(rect);
	//lCol.cx = rect.Width()+1100;               //  Į���� ���̸� ����(Scroll Bar�� �Ȼ���� �ϱ� ���� -5 �Ѵ�)
	lCol.cx = rect.Width()-5; 
	m_CimLog.InsertColumn(0, &lCol);  //  LVCOLUMN����ü�� ������� ���� ���� ����Ʈ ��Ʈ�ѿ� Į���� ����

}
void CDIgCimMsg::SetLogListUpdate()
{	
		//m_CimLog.EnsureVisible(m_CimLog.GetItemCount()-1,FALSE);
	//m_CimLog.SetTextColor(Color::Blue);
	switch(m_Msg_status)
	{
	case CIM_OPCALL_MSG:
		CGxUICtrl::SetStaticString(this, IDC_GXSTC_CIMLOG, _T("OPCALL ID : ")+m_strID);
		break;
	case CIM_INTERLOCK_MSG:
		CGxUICtrl::SetStaticString(this, IDC_GXSTC_CIMLOG, _T("INTERLOCK ID : ")+m_strID);
		break;
	case CIM_TERMINAL_MSG:
		CGxUICtrl::SetStaticString(this, IDC_GXSTC_CIMLOG, _T("TERMINAL Message"));
		break;
	}
	//m_strID=_T("ID:  ")+m_strID+_T("");
	//m_CimLog.InsertItem(m_CimLog.GetItemCount(), m_strID);
	int length=m_strMessage.GetLength();
	//�ϴ� ���������... ������
	// 	m_CimLog.InsertItem(m_CimLog.GetItemCount(), m_strMessage);
	int nLineMax = 107;
	if(length>nLineMax)
	{
		int nIndex = 0;
		while(nIndex < length)
		{
			m_CimLog.InsertItem(m_CimLog.GetItemCount(), m_strMessage.Mid(nIndex, nLineMax));
			nIndex += nLineMax;
		}


// 		CString m_strMessageTemp=m_strMessage.Right(length-105);
// 		m_CimLog.InsertItem(m_CimLog.GetItemCount(), m_strMessage.Left(105));
// 		m_CimLog.InsertItem(m_CimLog.GetItemCount(), m_strMessageTemp.Left(105));
// 		if(length>200)
// 		{
// 			m_CimLog.InsertItem(m_CimLog.GetItemCount(), m_strMessage.Right(length-210));
// 		}
	}
	else
	{
		m_CimLog.InsertItem(m_CimLog.GetItemCount(), m_strMessage);
	}

//  	int nCount = m_CimLog.GetItemCount();
//  	if (nCount > 0)
//  		m_CimLog.EnsureVisible(nCount-1, FALSE);
}



void CDIgCimMsg::ClickGxbtnBuzzerOff()
{
// 	theUnitFunc.SetOutPutIO(Y_BUZZER_K1, OFF);
// 	theUnitFunc.SetOutPutIO(Y_BUZZER_K2, OFF);
// 	theUnitFunc.SetOutPutIO(Y_BUZZER_K3, OFF);
// 	theUnitFunc.SetOutPutIO(Y_BUZZER_K4, OFF);
}
