#pragma once

#include "GausGUI.h"


// #include "resource.h"

// CGxMsgBox ��ȭ �����Դϴ�.

class _GAUS_GUI CGxMsgBox : public CDialog
{
	DECLARE_DYNAMIC(CGxMsgBox)

public:
	CGxMsgBox(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CGxMsgBox();

	// ��ȭ ���� �������Դϴ�.
	// enum { IDD = IDD_DLG_MSGBOX };

	enum { eMAX_LANG = 3 };


protected:


	int m_nResult;							// ������� ����
	BOOL m_bConfirm;						// ������� ������ ���� ���ΰ� ? TRUE:Yes/No�� ����, FALSE:OK�� ����
	int m_nLangIdx;						// ��� index

	CString m_sLangName[eMAX_LANG];	// ��� �̸�
	CString m_sTitle[eMAX_LANG];		// �� ���� ���ڿ�
	CString m_sMsg[eMAX_LANG];			// �� �޼��� ���ڿ�

	void DispLangSelect();				// ���� ���õ� ����� ��ư�� ���������ش�.


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();

	virtual void OnCancel();
	virtual void OnOK();
	void OnAbort();									// ���� ����


	// GxOCX Click Event
	DECLARE_EVENTSINK_MAP()
	afx_msg void OnBnClickedBtn_OK();
	afx_msg void OnBnClickedBtn_YES();
	afx_msg void OnBnClickedBtn_NO();

	afx_msg void OnBnClickedBtn_Lang1();
	afx_msg void OnBnClickedBtn_Lang2();
	afx_msg void OnBnClickedBtn_Lang3();

public:


	// ��� �޼����� �� ���� �����Ѵ�.
	void SetLangName(int nIdx, CString sName);			// ���� ������ ��� �̸��� �����Ѵ�.
	void SetTitle(CString sTitle1, CString sTitle2=_T(""), CString sTitle3=_T(""));			// ����
	void SetMessage(BOOL bConfirm, CString sMsg1, CString sMsg2=_T(""), CString sMsg3=_T(""), int nLang=0);


};
