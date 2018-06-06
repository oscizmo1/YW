#pragma once
#include "..\..\Resource.h"
#include "ui/gridctrl/gridctrl.h"


// CDlgPageIO ��ȭ �����Դϴ�.

class CDlgPageIO : public CDialog
{
	DECLARE_DYNAMIC(CDlgPageIO)

public:
	CDlgPageIO(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgPageIO();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_IO_PAGE };

DECLARE_EVENTSINK_MAP()

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void SetPageCount(int iRow)	{	m_iRowCount = iRow;	};
	void SetBoolInput(BOOL val) {	m_bInput = val; }


protected:
	afx_msg void OnGridSelChange(NMHDR *pNotifyStruct, LRESULT *pResult);
protected:
	CGridCtrl	m_GridPage;
	BOOL		m_bInput;
	int			m_iRowCount;
	int			m_iMove;
		

public:
	void InitGrid();
	void UpdateGrid();

	void ClickBtnPageDown();
	void ClickBtnPageUp();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
};
