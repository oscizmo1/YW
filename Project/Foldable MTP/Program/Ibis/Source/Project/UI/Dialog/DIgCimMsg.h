#pragma once
#include "afxcmn.h"


// CDIgCimMsg ��ȭ �����Դϴ�.

class CDIgCimMsg : public CDialog
{
	DECLARE_DYNAMIC(CDIgCimMsg)

public:
	CDIgCimMsg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual BOOL OnInitDialog();
	virtual ~CDIgCimMsg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_CIM_MSG };
	
protected:
	CListCtrl m_CimLog;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);		
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	
public:
	CString m_strID;
	CString m_strMessage;
	CIM_MSG_TYPE m_Msg_status;
	void SetLogListUpdate();
	void InitList();
	void SetStrID(CString strID, CIM_MSG_TYPE Msg_status)
	{	
		m_strID = strID;
		m_Msg_status=Msg_status;
	}
	void SetStrMsg( CString strMessage)
	{	
		m_strMessage = strMessage; 
	}
	void ClickGxbtnClose();
	void ClickGxbtnBuzzerOff();
};
