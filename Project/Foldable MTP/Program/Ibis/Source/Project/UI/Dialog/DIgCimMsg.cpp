// DIgCimMsg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DIgCimMsg.h"
#include "afxdialogex.h"
#include "UI\GausGUI\GxUICtrl.h"

// CDIgCimMsg 대화 상자입니다.

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


// CDIgCimMsg 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDIgCimMsg, CDialog)
	ON_EVENT(CDIgCimMsg, IDC_GXBTN_CLOSE, DISPID_CLICK, CDIgCimMsg::ClickGxbtnClose, VTS_NONE)
	ON_EVENT(CDIgCimMsg, IDC_GXBTN_BUZZER_OFF, DISPID_CLICK, CDIgCimMsg::ClickGxbtnBuzzerOff, VTS_NONE)
END_EVENTSINK_MAP()

LRESULT CDIgCimMsg::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
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

	lCol.fmt = LVCFMT_CENTER;   // 칼럼의 정렬을 왼쪽정렬로 지정(LVCFMT_CENTER, LVCFMT_LEFT, LVCFMT_RIGHT중 선택)
	lCol.pszText = _T("MESSAGE");    //  칼럼의 제목을 지정
	lCol.iSubItem = 0;    
	CRect rect;//  서브아이템의 인덱스를 지정
	m_CimLog.GetWindowRect(rect);
	//lCol.cx = rect.Width()+1100;               //  칼럼의 넓이를 지정(Scroll Bar를 안생기게 하기 위해 -5 한다)
	lCol.cx = rect.Width()-5; 
	m_CimLog.InsertColumn(0, &lCol);  //  LVCOLUMN구조체로 만들어진 값을 토대로 리스트 컨트롤에 칼럼을 삽입

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
	//일단 여기까지만... ㅎㅎㅎ
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
