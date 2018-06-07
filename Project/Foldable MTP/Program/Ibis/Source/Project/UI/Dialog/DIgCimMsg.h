#pragma once
#include "afxcmn.h"


// CDIgCimMsg 대화 상자입니다.

class CDIgCimMsg : public CDialog
{
	DECLARE_DYNAMIC(CDIgCimMsg)

public:
	CDIgCimMsg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual BOOL OnInitDialog();
	virtual ~CDIgCimMsg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_CIM_MSG };
	
protected:
	CListCtrl m_CimLog;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
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
