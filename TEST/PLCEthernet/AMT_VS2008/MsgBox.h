#pragma once
#include "label.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "lbl_blu.h"



// CMsgBox 대화 상자입니다.

class CMsgBox : public CDialog
{
public:
	int m_nTitleNo;
	int m_nMsgID;
	int m_nDeviceChange;
	CString m_strTitle;
	CString m_strMsg;
	CString m_strMsg2;
	CString m_strAct;
	CString m_strCode;

	void SetMessage();
	void SetInit(int nTitleNo, int nMsgID);
	void ErrorLog(int nTitleNo, int nMsgNo, CString strErrMsg);
	void AlarmBitOff();

private:
	void ShowCancel(BOOL bShow);
	CRect rtOKNomal;

	
	DECLARE_DYNAMIC(CMsgBox)

public:
	CMsgBox(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CMsgBox(int nTitleNo, int nMsgID);
	virtual ~CMsgBox();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MSG };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CButton m_btnIDOK;
	CButton m_btnIDCANCEL;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CLbl_blu m_lblTitle;
	CLbl_blu m_lblMsg;
	CLbl_blu m_lblAct;
	CLbl_blu m_lblMsg2;
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};