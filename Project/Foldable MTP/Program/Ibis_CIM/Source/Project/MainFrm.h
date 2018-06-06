
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once

#include "DlgBarTitle.h"
#include "DlgBarMenu.h"
#include "ViewCIM.h"
#include "ViewFDC.h"
#include "ViewRMS.h"
#include "ViewECM.h"
#include "ViewSETTING.h"

#include "DlgSockStatus.h"


class CMainFrame : public CFrameWnd
{
	
protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
	CView	*m_pOldView,
			*m_pNewView;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();

	CDlgBarTitle m_wndMainTitle;
	CDlgBarMenu  m_wndMainMenu;
	CViewCIM	*m_pViewCIM;
	CViewFDC	*m_pViewFDC;
	CViewRMS *m_pViewRMS;
	CViewECM *m_pViewECM;
	CViewSETTING *m_pViewSETTING;

	BOOL OnCreateView(void);
	int OnMenuSelect(UINT nItemID);

	afx_msg LRESULT OnViewChange(WPARAM wParam, LPARAM lParam);					// ȭ�� ���� ����
	afx_msg LRESULT OnHostCommUpdate(WPARAM wParam, LPARAM lParam);			// HOST ��� ������ ���� ǥ��
	afx_msg LRESULT OnInfoUpdate(WPARAM wParam, LPARAM lParam);					// ���� ������ ����Ǿ���. ȭ�鿡 �ݿ��Ѵ�.

	afx_msg LRESULT OnEQConnect(WPARAM wParam, LPARAM lParam);					// �PC�� ���� 
	afx_msg LRESULT OnEQDisconnect(WPARAM wParam, LPARAM lParam);				// �PC�� ���� ����



	//afx_msg void OnBnClickedButtonMenuCIM();
	//afx_msg void OnBnClickedButtonMenuFDC();
	//afx_msg void OnBnClickedButtonMenuCELL();
	//afx_msg void OnBnClickedButtonMenuTACTTIME();
	//afx_msg void OnBnClickedButtonMenuALIGNnMCR();
	//afx_msg void OnBnClickedButtonMenuALARM();
	//afx_msg void OnBnClickedButtonMenuRMSnECN();
	//afx_msg void OnBnClickedButtonMenuSETTING();
	//afx_msg void OnBnClickedButtonMainExit();


	BOOL DataStructTrans();
	BOOL InitializeSocket();
//	BOOL DataBaseConnect();
//	BOOL DataBaseDisConnect();
	BOOL GlobalDataStructInit();
	//GT.PARK - �PC�� ����� ���� ���� ���� ���,  RECIPE������ ���� � ini���� �������� ���� �ڵ�
	BOOL GetRecipe(); 

	CTime MakeCTimeType(CString strTime);
	CString MakeCStringType(CTime t);

	BOOL CheckNetworkDriverConnection();
	//BOOL CheckYmsNetworkDriverConnection();
	BOOL CheckQMSNetworkDriverConnection();
	BOOL CheckSignalResultNetworkConnection();
	BOOL CheckVtNetworkDriverConnection();
	BOOL IsFolder(CString strPath);
	BOOL IsFile(CString strPath);
		

};


