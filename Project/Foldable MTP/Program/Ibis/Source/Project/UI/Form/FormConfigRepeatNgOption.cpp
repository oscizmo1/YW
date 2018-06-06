// FormConfigRepeatNgOption.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormConfigRepeatNgOption.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "IbisApp.h"
#include "UI/Dialog/DlgBoxEDIT.h"

#include "Etc/Ini.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CFormConfigNgOption

IMPLEMENT_DYNCREATE(CFormConfigNgOption, CFormView)

CFormConfigNgOption::CFormConfigNgOption()
	: CFormView(CFormConfigNgOption::IDD)
{
	bFirstInit = FALSE;

	m_iSetVelNumIndex=0;
	m_iSelectedRow=1;
	m_bEnableUI = FALSE;
}

CFormConfigNgOption::~CFormConfigNgOption()
{
}

void CFormConfigNgOption::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// GAUSGUI에서 OnInitialUpdate를 두번 호출하는 바람에 하나는 그냥 넘긴다 [10/5/2017 OSC]
	if(bFirstInit == FALSE)
	{
		bFirstInit = TRUE;
		return;
	}
	NGGrid_Init();
	ClickGxbtnConfigNgRead();
}
void CFormConfigNgOption::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_GRID_NG_LIST,m_Grid_Ng_List);
}

BEGIN_MESSAGE_MAP(CFormConfigNgOption, CFormView)


	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	ON_NOTIFY(NM_CLICK, IDC_GRID_NG_LIST, OnGridEndSelChange)
	ON_WM_TIMER()

END_MESSAGE_MAP()
// CFormConfigNgOption 진단입니다.

#ifdef _DEBUG
void CFormConfigNgOption::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormConfigNgOption::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormConfigNgOption 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CFormConfigNgOption, CFormView)
	ON_EVENT(CFormConfigNgOption, IDC_GXBTN_CONFIG_NG_ADD, DISPID_CLICK, CFormConfigNgOption::ClickGxbtnConfigNgAdd, VTS_NONE)
	ON_EVENT(CFormConfigNgOption, IDC_GXBTN_CONFIG_NG_DELETE, DISPID_CLICK, CFormConfigNgOption::ClickGxbtnConfigNgDelete, VTS_NONE)
	ON_EVENT(CFormConfigNgOption, IDC_GXBTN_CONFIG_NG_ON_OFF, DISPID_CLICK, CFormConfigNgOption::ClickGxbtnConfigNgOff, VTS_NONE)

	ON_EVENT(CFormConfigNgOption, IDC_GXBTN_CONFIG_NG_EDIT, DISPID_CLICK, CFormConfigNgOption::ClickGxbtnConfigNgEdit, VTS_NONE)
	ON_EVENT(CFormConfigNgOption, IDC_GXBTN_CONFIG_NG_SAVE, DISPID_CLICK, CFormConfigNgOption::ClickGxbtnConfigNgSave, VTS_NONE)
	ON_EVENT(CFormConfigNgOption, IDC_GXBTN_CONFIG_NG_READ, DISPID_CLICK, CFormConfigNgOption::ClickGxbtnConfigNgRead, VTS_NONE)
END_EVENTSINK_MAP()


LRESULT CFormConfigNgOption::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{

	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		/*m_bEditMode = FALSE;
		if(m_bEditMode)
			CGxUICtrl::SetButtonColor(this, IDC_GXBTN_EDIT_MODE, GXCOLOR_RED);
		else
			CGxUICtrl::SetButtonColor(this, IDC_GXBTN_EDIT_MODE, GXCOLOR_OFF);
		ShuttleBtn_Refresh();*/
		SetTimer(0, 100, NULL);
	}
	else
	{
		KillTimer(0);
	}

	return 0;
}

LRESULT CFormConfigNgOption::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	// CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_BOTTOM_MENU1);

	return 0;
}
// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormConfigNgOption::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}

void CFormConfigNgOption::NGGrid_Init()
{
	//설정
	m_Grid_Ng_List.SetEditable(FALSE);
	m_Grid_Ng_List.SetFixedColumnCount(1); 
	m_Grid_Ng_List.SetFixedRowCount(1);
 	m_Grid_Ng_List.SetRowCount(27);
 	m_Grid_Ng_List.SetColumnCount(4);
 	m_Grid_Ng_List.SetRowResize(FALSE);
 	m_Grid_Ng_List.SetColumnResize(FALSE);		
 	m_Grid_Ng_List.SetListMode(FALSE);
 	m_Grid_Ng_List.EnableSelection(FALSE);

	//간격
	m_Grid_Ng_List.SetColumnWidth(0, 60); 
	m_Grid_Ng_List.SetColumnWidth(1, 180);
	m_Grid_Ng_List.SetColumnWidth(2, 180);


	// Title
	m_Grid_Ng_List.SetItemText(0,0,_T("no"));
	m_Grid_Ng_List.SetItemText(0,1,_T("Name"));
	m_Grid_Ng_List.SetItemText(0,2,_T("Count"));
	m_Grid_Ng_List.SetItemText(0,3,_T("State"));

	
	// Size Fit
	m_Grid_Ng_List.ExpandColumnsToFit();
	m_Grid_Ng_List.ExpandToFit();
	m_Grid_Ng_List.Refresh();

}
void CFormConfigNgOption::ClickGxbtnConfigNgAdd()
{
	CDlgBoxEdit dlgKey(NULL, _T(""));
	CString sztemp,str;
	if ( dlgKey.DoModal() == IDOK )
	{
		CString szInput = dlgKey.m_strReturnText;
		CString numInput = dlgKey.m_strReturnCount;
		
		for(int i=1;i<m_Grid_Ng_List.GetRowCount();i++)
		{
			// 중복검사
			if (szInput == m_Grid_Ng_List.GetItemText(i,1))
			{
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("이미 존재하고 있는 NG NAME 입니다."), 
					_T("NG NAME that already exist."),		
					_T("Đang tồn tại một NG NAME.") , GetMainHandler()->m_nLangIdx);

				dlgMsgBox.DoModal();
				return;
			}

			if(m_Grid_Ng_List.GetItemText(i,1)=="")
			{
				m_Grid_Ng_List.SetItemText(i,1,szInput);
				m_Grid_Ng_List.SetItemText(i,2,numInput);

				str.Format(_T("%d"),i);
				m_Grid_Ng_List.SetItemText(i,0,str);

				if(m_Grid_Ng_List.GetItemText(i,2)=="")
					m_Grid_Ng_List.SetItemText(i,2,_T("3"));

				break;
			}
		}
	}
	m_Grid_Ng_List.Refresh();
}

void CFormConfigNgOption::ClickGxbtnConfigNgDelete()
{
	CGxMsgBox	dlgMsgBox;
	int i= 0,j=0;

	if ( m_iSetVelNumIndex == 0 )	return;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
	dlgMsgBox.SetMessage(TRUE, 
		_T("정말로 삭제하시겠습니까?"), 
		_T("Are you sure you want to Delete it?"),		
		_T("Bạn chắc chắn muốn xóa?"), GetMainHandler()->m_nLangIdx );

	int nRet = dlgMsgBox.DoModal();
	if(nRet != IDOK)
		return;

	m_Grid_Ng_List.DeleteRow(m_iSelectedRow);
	m_Grid_Ng_List.InsertRow(_T(""));

	for(i=1 ; i<m_Grid_Ng_List.GetRowCount() ; i++)
	{
		if(m_Grid_Ng_List.GetItemText(i,0) == "")
			break;
		j++;
	}

	CString str;
	for(i=1 ; i<j+1 ; i++)
	{
		str.Format(_T("%d"),i);
		m_Grid_Ng_List.SetItemText(i,0,str);
	}
	m_iSetVelNumIndex = 0;
	m_Grid_Ng_List.Refresh();
}

void CFormConfigNgOption::ClickGxbtnConfigNgEdit()
{
	CString sztemp,str;

	if ( m_iSetVelNumIndex == 0 )	return;

	sztemp=m_Grid_Ng_List.GetItemText(m_iSelectedRow,1);
	str=m_Grid_Ng_List.GetItemText(m_iSelectedRow,2);

	CDlgBoxEdit dlgKey(NULL, sztemp, str);	

	if ( dlgKey.DoModal() == IDOK )
	{
		CString szInput = dlgKey.m_strReturnText;
		CString numInput = dlgKey.m_strReturnCount;

		for(int i=1;i<m_Grid_Ng_List.GetRowCount();i++)
		{
			// 중복검사

			if (szInput == m_Grid_Ng_List.GetItemText(i,1))
			{
				if(szInput == m_Grid_Ng_List.GetItemText(m_iSelectedRow,1))
					break;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("이미 존재하고 있는 NG NAME 입니다."), 
					_T("NG NAME that already exist."),		
					_T("Đang tồn tại một NG NAME.") , GetMainHandler()->m_nLangIdx);

				dlgMsgBox.DoModal();
				return;
			}
		}
		m_Grid_Ng_List.SetItemText(m_iSelectedRow,1,szInput);
		m_Grid_Ng_List.SetItemText(m_iSelectedRow,2,numInput);
	}
	m_Grid_Ng_List.Refresh();
}

void CFormConfigNgOption::ClickGxbtnConfigNgSave()
{
	CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
	dlgMsgBox.SetMessage(TRUE, 
		_T("수정하신 작업을 적용하겠습니까?"), 
		_T("Would you like to apply your modifications?"),		
		_T("Bạn có muốn áp dụng các thay đổi?"), GetMainHandler()->m_nLangIdx );

	int nRet = dlgMsgBox.DoModal();
	if(nRet != IDOK)
		return;

	CString strPath;
	strPath.Format(_T("D:\\Ibis\\Config\\NgOption.ini"));

	CIni ini(strPath);

	::DeleteFile(strPath);

	for(int i=1;i<m_Grid_Ng_List.GetRowCount();i++)
	{
		if(m_Grid_Ng_List.GetItemText(i,0)=="")
			break;
		ini.WriteString(m_Grid_Ng_List.GetItemText(i,0),_T("NAME"),m_Grid_Ng_List.GetItemText(i,1));
		ini.WriteString(m_Grid_Ng_List.GetItemText(i,0),_T("COUNT"),m_Grid_Ng_List.GetItemText(i,2));
		ini.WriteLong(m_Grid_Ng_List.GetItemText(i,0),_T("STATE"),m_Grid_Ng_List.GetItemBkColour(i,3));
	}
}

void CFormConfigNgOption::ClickGxbtnConfigNgRead()
{
	CString strPath,num;
	strPath.Format(_T("%s/NgOption.ini"), theConfigBank.m_System.m_strConfigPath); 

	CIni ini(strPath);

	for (int i=1;i<m_Grid_Ng_List.GetRowCount();i++)
	{
		for(int j = 0 ; j<3 ; j++)
			m_Grid_Ng_List.SetItemText(i,j,_T(""));
	}

	for(int i=1;i<m_Grid_Ng_List.GetRowCount();i++)
	{
		num.Format(_T("%d"),i);

		if(ini.GetString(num,_T("NAME"),m_Grid_Ng_List.GetItemText(i,1))=="")
			break;

		m_Grid_Ng_List.SetItemText(i,0,num);
		m_Grid_Ng_List.SetItemText(i,1,ini.GetString(num,_T("NAME"),m_Grid_Ng_List.GetItemText(i,1)));
		m_Grid_Ng_List.SetItemText(i,2,ini.GetString(num,_T("COUNT"),m_Grid_Ng_List.GetItemText(i,2)));
		m_Grid_Ng_List.SetItemBkColour(i,3,ini.GetLong(num,_T("STATE"),m_Grid_Ng_List.GetItemBkColour(i,3)));
	}
	m_Grid_Ng_List.Refresh();
}


void CFormConfigNgOption::ClickGxbtnConfigNgOff()
{
	ChangeOnOffValue(IDC_GXBTN_CONFIG_NG_ON_OFF);
}

void CFormConfigNgOption::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);
	switch(nIDEvent)
	{
	case 0:
		//LBGrid_Refresh();
		SetTimer(nIDEvent, 200, NULL);
		break;
	}
	CFormView::OnTimer(nIDEvent);
}

void CFormConfigNgOption::ChangeOnOffValue( int nBtnID )
{
	CGxButtonEx* pOnOff = (CGxButtonEx*)GetDlgItem(nBtnID);
	if(pOnOff)
	{
		if(pOnOff->GetCaption() ==_T("OFF"))
		{
			pOnOff->SetCaption(_T("ON"));
			pOnOff->SetBackColor(GXCOLOR_ON);
		}
		else
		{
			pOnOff->SetCaption(_T("OFF"));
			pOnOff->SetBackColor(GXCOLOR_OFF);
		}
	}
}

void CFormConfigNgOption::OnGridEndSelChange(NMHDR *pNotifyStruct, LRESULT *pResult)
{
	CString sztemp;
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;	

	if ( pItem->iRow < 1 )		return;

	m_iSelectedRow = pItem->iRow;
	sztemp=m_Grid_Ng_List.GetItemText(m_iSelectedRow,0);
	m_iSetVelNumIndex= _ttoi(sztemp);

	SetColor(m_iSelectedRow);
}

void CFormConfigNgOption::SetColor(int clr)
{
	if(m_Grid_Ng_List.GetItemText(clr,0)=="")return;

	if (m_Grid_Ng_List.GetItemBkColour(clr,3)==RGB(0,255,0))
	{
		m_Grid_Ng_List.SetItemBkColour(clr,3,RGB(255,255,255));
	}
	else {
		m_Grid_Ng_List.SetItemBkColour(clr,3,RGB(0,255,0));
	}
	m_Grid_Ng_List.Refresh();
}
