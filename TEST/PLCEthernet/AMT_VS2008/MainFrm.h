// MainFrm.h : CMainFrame 클래스의 인터페이스
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
	
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 특성
public:

// 작업
public:

// 재정의
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.

protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

// 메시지 맵 함수를 생성했습니다.
protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int	OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);

	//////////////////////////////////////////////////////////////////////////
	// MsgBox 관련
private:
	CMsgBox m_MsgBox;
	int m_nTitleNo;
	int m_nErrNo;
	BOOL bLampState;
	BOOL m_bShowMsgBox;	// 현재 DoModal중인지 확인하는 변수
public:
	void ShowMsgBox(int nTitleNo, int nErrNo);
	BOOL SetMsgNo(int nTitleNo, int nErrNo);	
	afx_msg LRESULT OnShowMsgBox(UINT wParam, LPARAM lParam);
	//////////////////////////////////////////////////////////////////////////
	// Engineer 유실코드 입력 창 [1/8/2011 OSC]
private:
	CDlgEngineerDownCode m_dlgEngineerDownCode;
	afx_msg LRESULT OnShowEngineerDownCode(UINT wParam, LPARAM lParam);
public:
	CString m_strEngineerDownCode;
	CString m_strEngineerDownText;
	// 유실코드 입력창 떠 있는 동안 TRUE [12/27/2010 OSC]
	// 입력 안된 상태에서 START 막기 위한 변수 [12/27/2010 OSC]
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
