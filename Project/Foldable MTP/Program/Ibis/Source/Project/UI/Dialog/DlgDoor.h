#pragma once
#include "UI\GridCtrl\GridCtrl.h"

#include <vector>

typedef struct
{
	int nIn;
	int nOut;
}tDoor;

enum
{
	ID_DOOR_TIMER = 103
};


// CDlgDoor ��ȭ �����Դϴ�.

class CDlgDoor : public CDialog
{
	DECLARE_DYNAMIC(CDlgDoor)

public:
	CDlgDoor(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgDoor();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_DOOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	void OnGridDoorSelChange(NMHDR *pNotifyStruct, LRESULT *pResult);
	void InitGrid();
	void UpdateGrid();
	void ClickBtnDoorAllOpen();
	void ClickBtnDoorAllClose();
	void ClickBtnDoorCancel();

protected:
	CGridCtrl m_Grid;

	std::vector<tDoor> m_vecDoor;

	BOOL m_bBackDoorOpen;

public:

	
	


};
