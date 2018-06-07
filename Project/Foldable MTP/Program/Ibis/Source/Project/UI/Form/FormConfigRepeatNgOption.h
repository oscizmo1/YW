#pragma once
#include "resource.h"
#include "UI\GridCtrl\GridCtrl.h"



// CFormConfigNgOption 폼 뷰입니다.

class CFormConfigNgOption : public CFormView
{
	DECLARE_DYNCREATE(CFormConfigNgOption)

public:
	CFormConfigNgOption();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormConfigNgOption();

public:
	enum { IDD = IDD_FORM_CONFIG_REPEAT_NG_OPTION };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 화면 처리에 대한 메세지 함수
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// 표시 언어가 변경되었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	DECLARE_MESSAGE_MAP()

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	BOOL bFirstInit;

public:
	void ChangeOnOffValue(int nBtnID);
	void NGGrid_Init();
	void SetColor(int clr);

	//void NGGrid_Refresh();

protected:

	BOOL		m_bEnableUI;


	CGridCtrl	m_Grid_Ng_List;
	int			m_iSetVelNumIndex;
	int			m_iSelectedRow;
	BOOL		m_bEditMode;

	afx_msg void OnGridEndSelChange(NMHDR *pNotifyStruct, LRESULT *pResult);

public:
	DECLARE_EVENTSINK_MAP()
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ClickGxbtnConfigNgAdd();
	void ClickGxbtnConfigNgDelete();
	void ClickGxbtnConfigNgOff();
	void ClickGxbtnConfigNgEdit();
	void ClickGxbtnConfigNgSave();
	void ClickGxbtnConfigNgRead();
};