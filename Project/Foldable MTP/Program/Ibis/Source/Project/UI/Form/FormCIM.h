#pragma once
#include "resource.h"

struct EQUIP_FUNCTION_UI
{
	EQUIP_FUNCTION_UI()
	{
		pFunction = NULL;
		nID_EFST1 = 0;
		nID_EFST2 = 0;
		nID_EFST3 = 0;
		nID_EFST4 = 0;
	}        

	EQUIP_FUNCTION *pFunction;
	UINT nID_EFST1;
	UINT nID_EFST2;
	UINT nID_EFST3;
	UINT nID_EFST4;
};

// CFormCIM �� ���Դϴ�.

class CFormCIM : public CFormView
{
	DECLARE_DYNCREATE(CFormCIM)

public:
	CFormCIM();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CFormCIM();

public:
	enum { IDD = IDD_FORM_CONFIG_CIM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()

public:	
	
	CConfigCIM m_configCIM;

	std::vector<EQUIP_FUNCTION_UI> m_vecFunctionUI;
	void FunctionVector_InitValue();
	void FunctionVector_UIClear();
	void FunctionVector_Value_to_UI();
	void FunctionVector_UI_to_Value(EFID_INDEX EFID, CString EFST);

	void LoadOption();
	void ClickGxbtnOptionSave();
	void ClickGxbtnCimCellTrackingOn();
	void ClickGxbtnCimCellTrackingOff();
	void ClickGxbtnCimCellTrackingNothing();
	void ClickGxstcConfigCimTrackingControlTkin();
	void ClickGxstcConfigCimTrackingControlTkout();
	void ClickGxstcConfigCimTrackingControlBoht();
	void ClickGxstcConfigCimTrackingControlNothing();
	void ClickGxstcConfigCimMaterialTrackingOn();
	void ClickGxstcConfigCimMaterialTrackingOff();
	void ClickGxstcConfigCimMaterialTrackingNothing();
	void ClickGxstcConfigCimCellMcrMoveUse();
	void ClickGxstcConfigCimCellMcrMoveUnuse();
	void ClickGxstcConfigCimCellMcrMoveNothing();
	void ClickGxstcConfigCimMaterialMcrMoveUse();
	void ClickGxstcConfigCimMaterialMcrMoveUnuse();
	void ClickGxstcConfigCimMaterialMcrMoveNothing();
	void ClickGxstcConfigCimLotAssignInformationAuto();
	void ClickGxstcConfigCimLotAssignInformationManu();
	void ClickGxstcConfigCimLotAssignInformationNothing();
	void ClickGxstcConfigCimAgvAccessModeAuto();
	void ClickGxstcConfigCimAgvAccessModeManu();
	void ClickGxstcConfigCimAgvAccessModeNothing();
	void ClickGxstcConfigCimAreaSensorModeUse();
	void ClickGxstcConfigCimAreaSensorModeUnuse();
	void ClickGxstcConfigCimAreaSensorModeNothing();
	void ClickGxstcConfigCimSortModeUse();
	void ClickGxstcConfigCimSortModeUnuse();
	void ClickGxstcConfigCimSortModeNothing();
	void ClickGxstcConfigCimInterlockControlTransfer();
	void ClickGxstcConfigCimInterlockControlLoading();
	void ClickGxstcConfigCimInterlockControlStep();
	void ClickGxstcConfigCimInterlockControlOwn();
};


