// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#include "FlatSplitterwnd.h"
#include "MsgBox.h"
#include "ViewBottom.h"
#include "PLCEthernet.h"
#include "DlgEngineerDownCode.h"


class CViewTop;
class CViewMain;
// class CViewBottom;
// class CMsgBox;


enum
{
	FORM_AUTO,
	FORM_MANUAL,
	FORM_IO,
	FORM_TEACH,
	FORM_ALIGN,
	FORM_SETTING,
	FORM_COMM,
	FORM_CIM,
	FORM_LOG,
	
	FORM_MAX
};


#pragma once
class CMainFrame : public CFrameWnd
{
	
protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Ư��
public:

// �۾�
public:

// ������
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ����
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.

protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int	OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);

	//////////////////////////////////////////////////////////////////////////
	// MsgBox ����
private:
	CMsgBox m_MsgBox;
	int m_nTitleNo;
	int m_nErrNo;
	BOOL bLampState;
	BOOL m_bShowMsgBox;	// ���� DoModal������ Ȯ���ϴ� ����
public:
	void ShowMsgBox(int nTitleNo, int nErrNo);
	BOOL SetMsgNo(int nTitleNo, int nErrNo);	
	afx_msg LRESULT OnShowMsgBox(UINT wParam, LPARAM lParam);
	//////////////////////////////////////////////////////////////////////////
	// Engineer �����ڵ� �Է� â [1/8/2011 OSC]
private:
	CDlgEngineerDownCode m_dlgEngineerDownCode;
	afx_msg LRESULT OnShowEngineerDownCode(UINT wParam, LPARAM lParam);
public:
	CString m_strEngineerDownCode;
	CString m_strEngineerDownText;
	// �����ڵ� �Է�â �� �ִ� ���� TRUE [12/27/2010 OSC]
	// �Է� �ȵ� ���¿��� START ���� ���� ���� [12/27/2010 OSC]
	BOOL m_bShowEngineerDownCodeDlg;	
	//////////////////////////////////////////////////////////////////////////

public:
	CPLCEthernet	m_PlcEthernet;

	CFlatSplitterWnd	m_wndSplitter;
	HWND m_hTop, m_hBottom;

	CView*				m_pForm[FORM_MAX];
	CMTView*			m_pMainView;

	BOOL				m_bMelsecRead;
	BOOL				m_bMelsecWrite;	// 2009.07.16 Hui

	bool 				m_bPreInitialize;
	int					m_nUserLevel;
	CString				m_strUserName;

	BOOL				m_bWatchDog;
	void				StartMainFrmTimer();
	void 				PathInitialize();

	UINT				m_nCurrentFormID;
	BOOL CreateForm(CCreateContext* pContext);
	void				SwitchForm(UINT nFormID);
	virtual BOOL DestroyWindow();
	afx_msg LRESULT OnUnScrap(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnShowMatchingDeviceModel(UINT wParam, LPARAM lParam);
};
extern inline CMainFrame* GetMainFrame() { return (CMainFrame*)(AfxGetApp()->m_pMainWnd); }
