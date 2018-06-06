// FormCIM.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormCIM.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxMsgBox.h"
#include "UI\GausGUI\GxNumPad.h"
// CFormCIM

IMPLEMENT_DYNCREATE(CFormCIM, CFormView)

CFormCIM::CFormCIM()
	: CFormView(CFormCIM::IDD)
{
}

CFormCIM::~CFormCIM()
{
}

void CFormCIM::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormCIM, CFormView)
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CFormCIM, CFormView)
	ON_EVENT(CFormCIM, IDC_GXBTN_OPTION_SAVE, DISPID_CLICK, CFormCIM::ClickGxbtnOptionSave, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXBTN_CIM_CELL_TRACKING_ON, DISPID_CLICK, CFormCIM::ClickGxbtnCimCellTrackingOn, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXBTN_CIM_CELL_TRACKING_OFF, DISPID_CLICK, CFormCIM::ClickGxbtnCimCellTrackingOff, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXBTN_CIM_CELL_TRACKING_NOTHING, DISPID_CLICK, CFormCIM::ClickGxbtnCimCellTrackingNothing, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_TRACKING_CONTROL_TKIN, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimTrackingControlTkin, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_TRACKING_CONTROL_TKOUT, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimTrackingControlTkout, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_TRACKING_CONTROL_BOHT, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimTrackingControlBoht, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_TRACKING_CONTROL_NOTHING, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimTrackingControlNothing, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_MATERIAL_TRACKING_ON, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimMaterialTrackingOn, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_MATERIAL_TRACKING_OFF, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimMaterialTrackingOff, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_MATERIAL_TRACKING_NOTHING, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimMaterialTrackingNothing, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_CELL_MCR_MOVE_USE, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimCellMcrMoveUse, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_CELL_MCR_MOVE_UNUSE, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimCellMcrMoveUnuse, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_CELL_MCR_MOVE_NOTHING, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimCellMcrMoveNothing, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_MATERIAL_MCR_MOVE_USE, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimMaterialMcrMoveUse, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_MATERIAL_MCR_MOVE_UNUSE, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimMaterialMcrMoveUnuse, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_MATERIAL_MCR_MOVE_NOTHING, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimMaterialMcrMoveNothing, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_LOT_ASSIGN_INFORMATION_AUTO, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimLotAssignInformationAuto, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_LOT_ASSIGN_INFORMATION_MANU, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimLotAssignInformationManu, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_LOT_ASSIGN_INFORMATION_NOTHING, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimLotAssignInformationNothing, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_AGV_ACCESS_MODE_AUTO, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimAgvAccessModeAuto, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_AGV_ACCESS_MODE_MANU, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimAgvAccessModeManu, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_AGV_ACCESS_MODE_NOTHING, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimAgvAccessModeNothing, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_AREA_SENSOR_MODE_USE, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimAreaSensorModeUse, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_AREA_SENSOR_MODE_UNUSE, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimAreaSensorModeUnuse, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_AREA_SENSOR_MODE_NOTHING, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimAreaSensorModeNothing, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_SORT_MODE_USE, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimSortModeUse, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_SORT_MODE_UNUSE, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimSortModeUnuse, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_SORT_MODE_NOTHING, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimSortModeNothing, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_INTERLOCK_CONTROL_TRANSFER, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimInterlockControlTransfer, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_INTERLOCK_CONTROL_LOADING, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimInterlockControlLoading, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_INTERLOCK_CONTROL_STEP, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimInterlockControlStep, VTS_NONE)
	ON_EVENT(CFormCIM, IDC_GXSTC_CONFIG_CIM_INTERLOCK_CONTROL_OWN, DISPID_CLICK, CFormCIM::ClickGxstcConfigCimInterlockControlOwn, VTS_NONE)
END_EVENTSINK_MAP()


// CFormCIM 진단입니다.

#ifdef _DEBUG
void CFormCIM::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormCIM::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormCIM 메시지 처리기입니다.
LRESULT CFormCIM::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	BOOL bFlag = (BOOL)wParam;

	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		LoadOption();
		FunctionVector_InitValue();
		FunctionVector_Value_to_UI();

		if(theProcBank.MachineIsRunState())
		{
			EnableWindow(FALSE);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_CIM_OPTION_EDIT_MODE, _T("MONITOR MODE"));
		}
		else
		{
			EnableWindow(TRUE);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_CIM_OPTION_EDIT_MODE, _T("EDIT MODE"));
		}
	}
	else
	{
	}

	return 0;
}

void CFormCIM::FunctionVector_InitValue()
{
	EQUIP_FUNCTION_UI item;
	m_vecFunctionUI.clear();

	/* CELL_TRACKING */
	item.pFunction = m_configCIM.GetFunction(EFID_CELL_TRACKING);
	item.nID_EFST1 = IDC_GXBTN_CIM_CELL_TRACKING_ON;
	item.nID_EFST2 = IDC_GXBTN_CIM_CELL_TRACKING_OFF;
	item.nID_EFST3 = IDC_GXBTN_CIM_CELL_TRACKING_NOTHING;
	item.nID_EFST4 = 0;
	m_vecFunctionUI.push_back(item);

	/* TRACKING_CONTROL */
	item.pFunction = m_configCIM.GetFunction(EFID_TRACKING_CONTROL);
 	item.nID_EFST1 = IDC_GXSTC_CONFIG_CIM_TRACKING_CONTROL_TKIN;
 	item.nID_EFST2 = IDC_GXSTC_CONFIG_CIM_TRACKING_CONTROL_TKOUT;
 	item.nID_EFST3 = IDC_GXSTC_CONFIG_CIM_TRACKING_CONTROL_BOHT;
	item.nID_EFST4 = IDC_GXSTC_CONFIG_CIM_TRACKING_CONTROL_NOTHING;
	m_vecFunctionUI.push_back(item);

	/* MATERIAL_TRACKING */
	item.pFunction = m_configCIM.GetFunction(EFID_MATERIAL_TRACKING);
	item.nID_EFST1 = IDC_GXSTC_CONFIG_CIM_MATERIAL_TRACKING_ON;
	item.nID_EFST2 = IDC_GXSTC_CONFIG_CIM_MATERIAL_TRACKING_OFF;
	item.nID_EFST3 = IDC_GXSTC_CONFIG_CIM_MATERIAL_TRACKING_NOTHING;
	item.nID_EFST4 = 0;
	m_vecFunctionUI.push_back(item);

	/* CELL_MCR_MOVE */
	item.pFunction = m_configCIM.GetFunction(EFID_CELL_MCR_MODE);
	item.nID_EFST1 = IDC_GXSTC_CONFIG_CIM_CELL_MCR_MOVE_USE;
	item.nID_EFST2 = IDC_GXSTC_CONFIG_CIM_CELL_MCR_MOVE_UNUSE;
	item.nID_EFST3 = IDC_GXSTC_CONFIG_CIM_CELL_MCR_MOVE_NOTHING;
	item.nID_EFST4 = 0;
	m_vecFunctionUI.push_back(item);

	/* MATERIAL_MCR_MOVE */
	item.pFunction = m_configCIM.GetFunction(EFID_MATERIAL_MCR_MODE);
	item.nID_EFST1 = IDC_GXSTC_CONFIG_CIM_MATERIAL_MCR_MOVE_USE;
	item.nID_EFST2 = IDC_GXSTC_CONFIG_CIM_MATERIAL_MCR_MOVE_UNUSE;
	item.nID_EFST3 = IDC_GXSTC_CONFIG_CIM_MATERIAL_MCR_MOVE_NOTHING;
	item.nID_EFST4 = 0;
	m_vecFunctionUI.push_back(item);

	/* LOT_ASSIGN_INFORMATION */
	item.pFunction = m_configCIM.GetFunction(EFID_LOT_ASSIGN_INFORMATION);
	item.nID_EFST1 = IDC_GXSTC_CONFIG_CIM_LOT_ASSIGN_INFORMATION_AUTO;
	item.nID_EFST2 = IDC_GXSTC_CONFIG_CIM_LOT_ASSIGN_INFORMATION_MANU;
	item.nID_EFST3 = IDC_GXSTC_CONFIG_CIM_LOT_ASSIGN_INFORMATION_NOTHING;
	item.nID_EFST4 = 0;
	m_vecFunctionUI.push_back(item);

	/* AGV_ACCESS_MODE */
	item.pFunction = m_configCIM.GetFunction(EFID_AGV_ACCESS_MODE);
	item.nID_EFST1 = IDC_GXSTC_CONFIG_CIM_AGV_ACCESS_MODE_AUTO;
	item.nID_EFST2 = IDC_GXSTC_CONFIG_CIM_AGV_ACCESS_MODE_MANU;
	item.nID_EFST3 = IDC_GXSTC_CONFIG_CIM_AGV_ACCESS_MODE_NOTHING;
	item.nID_EFST4 = 0;
	m_vecFunctionUI.push_back(item);

	/* AREA_SENSOR_MODE */
	item.pFunction = m_configCIM.GetFunction(EFID_AREA_SENSOR_MODE);
	item.nID_EFST1 = IDC_GXSTC_CONFIG_CIM_AREA_SENSOR_MODE_USE;
	item.nID_EFST2 = IDC_GXSTC_CONFIG_CIM_AREA_SENSOR_MODE_UNUSE;
	item.nID_EFST3 = IDC_GXSTC_CONFIG_CIM_AREA_SENSOR_MODE_NOTHING;
	item.nID_EFST4 = 0;
	m_vecFunctionUI.push_back(item);

	/* SORT_MODE */
	item.pFunction = m_configCIM.GetFunction(EFID_SORT_MODE);
	item.nID_EFST1 = IDC_GXSTC_CONFIG_CIM_SORT_MODE_USE;
	item.nID_EFST2 = IDC_GXSTC_CONFIG_CIM_SORT_MODE_UNUSE;
	item.nID_EFST3 = IDC_GXSTC_CONFIG_CIM_SORT_MODE_NOTHING;
	item.nID_EFST4 = 0;
	m_vecFunctionUI.push_back(item);

	/* INTERLOCK_CONTROL */
	item.pFunction = m_configCIM.GetFunction(EFID_INTERLOCK_CONTROL);
	item.nID_EFST1 = IDC_GXSTC_CONFIG_CIM_INTERLOCK_CONTROL_TRANSFER;
	item.nID_EFST2 = IDC_GXSTC_CONFIG_CIM_INTERLOCK_CONTROL_LOADING;
	item.nID_EFST3 = IDC_GXSTC_CONFIG_CIM_INTERLOCK_CONTROL_STEP;
	item.nID_EFST4 = IDC_GXSTC_CONFIG_CIM_INTERLOCK_CONTROL_OWN;
	m_vecFunctionUI.push_back(item);

	// 사용하지 않는 Function은 비활성화 [12/9/2017 OSC]
	int nSize = m_vecFunctionUI.size();
	for(int i = 0; i < nSize; i++)
	{
		if(m_vecFunctionUI[i].pFunction->USE)
		{
			if(m_vecFunctionUI[i].nID_EFST1)
				CGxUICtrl::SetButtonEnabled(this, m_vecFunctionUI[i].nID_EFST1, TRUE);
			if(m_vecFunctionUI[i].nID_EFST2)
				CGxUICtrl::SetButtonEnabled(this, m_vecFunctionUI[i].nID_EFST2, TRUE);
			if(m_vecFunctionUI[i].nID_EFST3)
				CGxUICtrl::SetButtonEnabled(this, m_vecFunctionUI[i].nID_EFST3, TRUE);
			if(m_vecFunctionUI[i].nID_EFST4)
				CGxUICtrl::SetButtonEnabled(this, m_vecFunctionUI[i].nID_EFST4, TRUE);
		}
		else
		{
			if(m_vecFunctionUI[i].nID_EFST1)
				CGxUICtrl::SetButtonEnabled(this, m_vecFunctionUI[i].nID_EFST1, FALSE);
			if(m_vecFunctionUI[i].nID_EFST2)
				CGxUICtrl::SetButtonEnabled(this, m_vecFunctionUI[i].nID_EFST2, FALSE);
			if(m_vecFunctionUI[i].nID_EFST3)
				CGxUICtrl::SetButtonEnabled(this, m_vecFunctionUI[i].nID_EFST3, FALSE);
			if(m_vecFunctionUI[i].nID_EFST4)
				CGxUICtrl::SetButtonEnabled(this, m_vecFunctionUI[i].nID_EFST4, FALSE);
		}
		// CELL MCR MODE는 MCR 읽을 수 없는 설비만 NOTHING 설정 가능 [12/9/2017 OSC]
		if(m_vecFunctionUI[i].pFunction->EFNAME == _T("CELL MCR MODE"))
		{
			if(m_vecFunctionUI[i].nID_EFST3)
				CGxUICtrl::SetButtonEnabled(this, m_vecFunctionUI[i].nID_EFST3, FALSE);
		}
		// INTERLOCK CONTROL은 LOADING만 허용 [12/18/2017 OSC]
		if(m_vecFunctionUI[i].pFunction->EFNAME == _T("INTERLOCK CONTROL"))
		{
			if(m_vecFunctionUI[i].nID_EFST1)
				CGxUICtrl::SetButtonEnabled(this, m_vecFunctionUI[i].nID_EFST1, FALSE);
			if(m_vecFunctionUI[i].nID_EFST3)
				CGxUICtrl::SetButtonEnabled(this, m_vecFunctionUI[i].nID_EFST3, FALSE);
			if(m_vecFunctionUI[i].nID_EFST4)
				CGxUICtrl::SetButtonEnabled(this, m_vecFunctionUI[i].nID_EFST4, FALSE);
		}
	}
}

void CFormCIM::FunctionVector_UIClear()
{

}

void CFormCIM::FunctionVector_Value_to_UI()
{
	int nCount = m_vecFunctionUI.size();
	for(int i = 0; i < nCount; i++)
	{
		if(m_vecFunctionUI[i].pFunction->USE)
		{
			if(m_vecFunctionUI[i].nID_EFST1)
			{
				if(m_vecFunctionUI[i].pFunction->EFST_Get() == m_vecFunctionUI[i].pFunction->EFST_TYPE1)
					CGxUICtrl::SetButtonColor(this, m_vecFunctionUI[i].nID_EFST1, GXCOLOR_ON);
				else
					CGxUICtrl::SetButtonColor(this, m_vecFunctionUI[i].nID_EFST1, GXCOLOR_OFF);
			}

			if(m_vecFunctionUI[i].nID_EFST2)
			{
				if(m_vecFunctionUI[i].pFunction->EFST_Get() == m_vecFunctionUI[i].pFunction->EFST_TYPE2)
					CGxUICtrl::SetButtonColor(this, m_vecFunctionUI[i].nID_EFST2, GXCOLOR_ON);
				else
					CGxUICtrl::SetButtonColor(this, m_vecFunctionUI[i].nID_EFST2, GXCOLOR_OFF);
			}

			if(m_vecFunctionUI[i].nID_EFST3)
			{
				if(m_vecFunctionUI[i].pFunction->EFST_Get() == m_vecFunctionUI[i].pFunction->EFST_TYPE3)
					CGxUICtrl::SetButtonColor(this, m_vecFunctionUI[i].nID_EFST3, GXCOLOR_ON);
				else
					CGxUICtrl::SetButtonColor(this, m_vecFunctionUI[i].nID_EFST3, GXCOLOR_OFF);
			}

			if(m_vecFunctionUI[i].nID_EFST4)
			{
				if(m_vecFunctionUI[i].pFunction->EFST_Get() == m_vecFunctionUI[i].pFunction->EFST_TYPE4)
					CGxUICtrl::SetButtonColor(this, m_vecFunctionUI[i].nID_EFST4, GXCOLOR_ON);
				else
					CGxUICtrl::SetButtonColor(this, m_vecFunctionUI[i].nID_EFST4, GXCOLOR_OFF);
			}

		}
	}
}

void CFormCIM::FunctionVector_UI_to_Value(EFID_INDEX EFID, CString EFST)
{
	if( theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN )
	{
		CGxMsgBox	dlgMsgBox;
		dlgMsgBox.SetTitle(_T("알림"), _T("Notifications"), _T("thông báo"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("설비 정지 후 변경이 가능합니다."), 
			_T("If you wnat to change Function. Please stop the machine."), 
			_T("If you wnat to change Function. Please stop the machine."), 
			GetMainHandler()->m_nLangIdx);
		dlgMsgBox.DoModal();
	}
	else
	{
		// Type 선택 - LSH171127
		m_vecFunctionUI[EFID].pFunction->EFST_Set(EFST);
	}
}

void CFormCIM::LoadOption()
{
	m_configCIM = theConfigBank.m_CIM;
}


void CFormCIM::ClickGxbtnOptionSave()
{
//	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
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
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("CIM"), _T("SAVE"));
	
	// CIM설정 저장 - LSH171130

	m_configCIM.SetFunction(EFID_CELL_TRACKING, m_vecFunctionUI[EFID_CELL_TRACKING].pFunction->EFST_Get());
	m_configCIM.SetFunction(EFID_TRACKING_CONTROL, m_vecFunctionUI[EFID_TRACKING_CONTROL].pFunction->EFST_Get());
	m_configCIM.SetFunction(EFID_MATERIAL_TRACKING, m_vecFunctionUI[EFID_MATERIAL_TRACKING].pFunction->EFST_Get());
	m_configCIM.SetFunction(EFID_CELL_MCR_MODE, m_vecFunctionUI[EFID_CELL_MCR_MODE].pFunction->EFST_Get());
	m_configCIM.SetFunction(EFID_MATERIAL_MCR_MODE, m_vecFunctionUI[EFID_MATERIAL_MCR_MODE].pFunction->EFST_Get());
	m_configCIM.SetFunction(EFID_LOT_ASSIGN_INFORMATION, m_vecFunctionUI[EFID_LOT_ASSIGN_INFORMATION].pFunction->EFST_Get());
	m_configCIM.SetFunction(EFID_AGV_ACCESS_MODE, m_vecFunctionUI[EFID_AGV_ACCESS_MODE].pFunction->EFST_Get());
	m_configCIM.SetFunction(EFID_AREA_SENSOR_MODE, m_vecFunctionUI[EFID_AREA_SENSOR_MODE].pFunction->EFST_Get());
	m_configCIM.SetFunction(EFID_SORT_MODE, m_vecFunctionUI[EFID_SORT_MODE].pFunction->EFST_Get());
	m_configCIM.SetFunction(EFID_INTERLOCK_CONTROL, m_vecFunctionUI[EFID_INTERLOCK_CONTROL].pFunction->EFST_Get());

	// 전이도 에따라 EFST 변경 - LSH171201
	if(m_configCIM.TransitionOfEFListState())
	{

		// UI -> Config
		theConfigBank.m_CIM = m_configCIM;

		// INI File 저장 및 상위 보고 - LSH171130
		theConfigBank.SaveCIMFile();
		theConfigBank.m_CIM.ConfirmCIMFunction();
	}

	// 저장한 상태 등이 있으니 원복 
	// Config -> UI
	m_configCIM = theConfigBank.m_CIM;

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}


////////////////// CELL_TRACKING //////////////////
void CFormCIM::ClickGxbtnCimCellTrackingOn()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_CELL_TRACKING)->EFST_TYPE1;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_CELL_TRACKING, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}

void CFormCIM::ClickGxbtnCimCellTrackingOff()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_CELL_TRACKING)->EFST_TYPE2;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_CELL_TRACKING, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}

void CFormCIM::ClickGxbtnCimCellTrackingNothing()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_CELL_TRACKING)->EFST_TYPE3;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_CELL_TRACKING, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}



////////////////// TRACKING_CONTROL //////////////////
void CFormCIM::ClickGxstcConfigCimTrackingControlTkin()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_TRACKING_CONTROL)->EFST_TYPE1;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_TRACKING_CONTROL, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}

void CFormCIM::ClickGxstcConfigCimTrackingControlTkout()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_TRACKING_CONTROL)->EFST_TYPE2;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_TRACKING_CONTROL, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}

void CFormCIM::ClickGxstcConfigCimTrackingControlBoht()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_TRACKING_CONTROL)->EFST_TYPE3;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_TRACKING_CONTROL, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}

void CFormCIM::ClickGxstcConfigCimTrackingControlNothing()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_TRACKING_CONTROL)->EFST_TYPE4;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_TRACKING_CONTROL, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}



////////////////// MATERIAL_TRACKING //////////////////
void CFormCIM::ClickGxstcConfigCimMaterialTrackingOn()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_MATERIAL_TRACKING)->EFST_TYPE1;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_TRACKING_CONTROL, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}

void CFormCIM::ClickGxstcConfigCimMaterialTrackingOff()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_MATERIAL_TRACKING)->EFST_TYPE2;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_TRACKING_CONTROL, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}

void CFormCIM::ClickGxstcConfigCimMaterialTrackingNothing()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_MATERIAL_TRACKING)->EFST_TYPE3;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_TRACKING_CONTROL, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}



////////////////// CELL_MCR_MOVE //////////////////
void CFormCIM::ClickGxstcConfigCimCellMcrMoveUse()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_CELL_MCR_MODE)->EFST_TYPE1;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_CELL_MCR_MODE, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}

void CFormCIM::ClickGxstcConfigCimCellMcrMoveUnuse()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_CELL_MCR_MODE)->EFST_TYPE2;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_CELL_MCR_MODE, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}

void CFormCIM::ClickGxstcConfigCimCellMcrMoveNothing()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_CELL_MCR_MODE)->EFST_TYPE3;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_CELL_MCR_MODE, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}



////////////////// MATERIAL_MCR_MOVE //////////////////
void CFormCIM::ClickGxstcConfigCimMaterialMcrMoveUse()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_MATERIAL_MCR_MODE)->EFST_TYPE1;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_MATERIAL_MCR_MODE, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}

void CFormCIM::ClickGxstcConfigCimMaterialMcrMoveUnuse()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_MATERIAL_MCR_MODE)->EFST_TYPE2;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_MATERIAL_MCR_MODE, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}

void CFormCIM::ClickGxstcConfigCimMaterialMcrMoveNothing()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_MATERIAL_MCR_MODE)->EFST_TYPE3;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_MATERIAL_MCR_MODE, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}



////////////////// LOT_ASSIGN_INFORMATION //////////////////
void CFormCIM::ClickGxstcConfigCimLotAssignInformationAuto()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_LOT_ASSIGN_INFORMATION)->EFST_TYPE1;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_LOT_ASSIGN_INFORMATION, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}

void CFormCIM::ClickGxstcConfigCimLotAssignInformationManu()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_LOT_ASSIGN_INFORMATION)->EFST_TYPE2;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_LOT_ASSIGN_INFORMATION, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}

void CFormCIM::ClickGxstcConfigCimLotAssignInformationNothing()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_LOT_ASSIGN_INFORMATION)->EFST_TYPE3;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_LOT_ASSIGN_INFORMATION, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}



////////////////// AGV_ACCESS_MODE //////////////////
void CFormCIM::ClickGxstcConfigCimAgvAccessModeAuto()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_AGV_ACCESS_MODE)->EFST_TYPE1;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_AGV_ACCESS_MODE, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}

void CFormCIM::ClickGxstcConfigCimAgvAccessModeManu()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_AGV_ACCESS_MODE)->EFST_TYPE2;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_AGV_ACCESS_MODE, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}

void CFormCIM::ClickGxstcConfigCimAgvAccessModeNothing()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_AGV_ACCESS_MODE)->EFST_TYPE3;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_AGV_ACCESS_MODE, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}



////////////////// AREA_SENSOR_MODE //////////////////
void CFormCIM::ClickGxstcConfigCimAreaSensorModeUse()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_AREA_SENSOR_MODE)->EFST_TYPE1;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_AREA_SENSOR_MODE, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}

void CFormCIM::ClickGxstcConfigCimAreaSensorModeUnuse()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_AREA_SENSOR_MODE)->EFST_TYPE2;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_AREA_SENSOR_MODE, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}

void CFormCIM::ClickGxstcConfigCimAreaSensorModeNothing()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_AREA_SENSOR_MODE)->EFST_TYPE3;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_AREA_SENSOR_MODE, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}



////////////////// SORT_MODE //////////////////
void CFormCIM::ClickGxstcConfigCimSortModeUse()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_SORT_MODE)->EFST_TYPE1;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_SORT_MODE, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}

void CFormCIM::ClickGxstcConfigCimSortModeUnuse()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_SORT_MODE)->EFST_TYPE2;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_SORT_MODE, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}

void CFormCIM::ClickGxstcConfigCimSortModeNothing()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_SORT_MODE)->EFST_TYPE3;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_SORT_MODE, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}



////////////////// INTERLOCK_CONTROL //////////////////
void CFormCIM::ClickGxstcConfigCimInterlockControlTransfer()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_INTERLOCK_CONTROL)->EFST_TYPE1;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_INTERLOCK_CONTROL, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}

void CFormCIM::ClickGxstcConfigCimInterlockControlLoading()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_INTERLOCK_CONTROL)->EFST_TYPE2;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_INTERLOCK_CONTROL, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}

void CFormCIM::ClickGxstcConfigCimInterlockControlStep()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_INTERLOCK_CONTROL)->EFST_TYPE3;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_INTERLOCK_CONTROL, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}

void CFormCIM::ClickGxstcConfigCimInterlockControlOwn()
{
	CString str = theConfigBank.m_CIM.GetFunction(EFID_INTERLOCK_CONTROL)->EFST_TYPE4;

	// Type 선택 - LSH171127
	FunctionVector_UI_to_Value(EFID_INTERLOCK_CONTROL, str);

	// 현재 UI상태 갱신 - LSH171127
	FunctionVector_Value_to_UI();
}
