#pragma once
#include "label.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "lbl_blu.h"



// CMsgBox ��ȭ �����Դϴ�.

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
	CMsgBox(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	CMsgBox(int nTitleNo, int nMsgID);
	virtual ~CMsgBox();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MSG };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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