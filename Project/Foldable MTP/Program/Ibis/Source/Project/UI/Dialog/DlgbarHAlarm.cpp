// DlgbarHAlarm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DlgbarHAlarm.h"
#include "afxdialogex.h"

#include "..\..\GUIDefine.h"

//GxGUIFrame ���
#include "GausGUI\GxGUIFrame.h"
#include "GausGUI\GxUICtrl.h"



// CDlgbarHAlarm ��ȭ �����Դϴ�.


////////////////////////////////	[W] �˶� �Ҹ޴� Ŭ���� �߰� 160920



IMPLEMENT_DYNAMIC(CDlgbarHAlarm, CDialogBar)

CDlgbarHAlarm::CDlgbarHAlarm()
{

}

CDlgbarHAlarm::~CDlgbarHAlarm() 
{
}

void CDlgbarHAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}   


BEGIN_MESSAGE_MAP(CDlgbarHAlarm, CDialogBar)
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	ON_MESSAGE(GxMSG_GUI_CHANGE_VIEW_END,	OnChangeViewEnd)			// ���� ȭ���� ���� �Ϸ�Ǿ���.
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// ǥ�� �� �ٲ����.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDlgbarHAlarm, CDialogBar)
	ON_EVENT(CDlgbarHAlarm, IDC_BTNX_HALARM_SUBMENU1, DISPID_CLICK, CDlgbarHAlarm::OnClickBtnxSubMenu1, VTS_NONE)
	ON_EVENT(CDlgbarHAlarm, IDC_BTNX_HALARM_SUBMENU2, DISPID_CLICK, CDlgbarHAlarm::OnClickBtnxSubMenu2, VTS_NONE)
	ON_EVENT(CDlgbarHAlarm, IDC_BTNX_HALARM_SUBMENU3, DISPID_CLICK, CDlgbarHAlarm::OnClickBtnxSubMenu3, VTS_NONE)
	ON_EVENT(CDlgbarHAlarm, IDC_BTNX_HALARM_SUBMENU4, DISPID_CLICK, CDlgbarHAlarm::OnClickBtnxSubMenu4, VTS_NONE)
END_EVENTSINK_MAP()
// CDlgbarHAlarm �޽��� ó�����Դϴ�.




// ���� �޴� ���� ȭ�� ��ȯ �Ϸ�
LRESULT CDlgbarHAlarm::OnChangeViewEnd(WPARAM wParam, LPARAM lParam)
{
	//DispCurrentSelect( (UINT)wParam );		// ���õ� �޴� ����

	return 0;
}

// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
LRESULT CDlgbarHAlarm::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : ȭ�� ������ ǥ�� �ʱ�ȭ�� ���ְ� ���ſ� Ÿ�̸Ӹ� ����/���� �����ش�.
	BOOL bFlag = (BOOL)wParam;
	// ȭ�� ���� Ÿ�̸Ӹ� �����Ű��� �����ΰ� ?
	if ( bFlag )
	{

	}
	else
	{

	}

	return 0;
}


// ǥ�� �� ����Ǿ���.
LRESULT CDlgbarHAlarm::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// ������ ���� ǥ�ø� �����ϵ��� �Ѵ�.
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_HALARM_SUBMENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// ���� ���õ� ��ư�� ���� �޸����ش�.
void CDlgbarHAlarm::DispCurrentSelect(UINT nID)
{
	// ���� ����� ȭ���� �ش� ��ư�� �ش�ȴٸ� ��ư�� ���� �޸� ���ش�.
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HALARM_SUBMENU1, ((nID == eFORM_Alarm1) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HALARM_SUBMENU2, ((nID == eFORM_Alarm2) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HALARM_SUBMENU3, ((nID == eFORM_Alarm3) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HALARM_SUBMENU4, ((nID == eFORM_Alarm4) ?	GXCOLOR_ON : GXCOLOR_OFF));
}




// ���� ȭ������ ���� �����Ѵ�.
void CDlgbarHAlarm::ChangeForm(int nIdx, BOOL bForce)
{
	// CMainFrame���� Message�� ȣ�� ��ȯ�� �����Ѵ�.
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)nIdx, (LPARAM)bForce);
}


void CDlgbarHAlarm::OnClickBtnxSubMenu1()
{
	ChangeForm( eFORM_Alarm1 );
}


void CDlgbarHAlarm::OnClickBtnxSubMenu2()
{
	ChangeForm( eFORM_Alarm2 );
}


void CDlgbarHAlarm::OnClickBtnxSubMenu3()
{
	ChangeForm( eFORM_Alarm3 );
}


void CDlgbarHAlarm::OnClickBtnxSubMenu4()
{
	ChangeForm( eFORM_Alarm4 );
}




