
// MainFrm.h : CMainFrame 클래스의 인터페이스
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
	
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// 생성된 메시지 맵 함수
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

	afx_msg LRESULT OnViewChange(WPARAM wParam, LPARAM lParam);					// 화면 변경 실행
	afx_msg LRESULT OnHostCommUpdate(WPARAM wParam, LPARAM lParam);			// HOST 통신 데이터 수신 표시
	afx_msg LRESULT OnInfoUpdate(WPARAM wParam, LPARAM lParam);					// 각종 정보가 변경되었다. 화면에 반영한다.

	afx_msg LRESULT OnEQConnect(WPARAM wParam, LPARAM lParam);					// 운영PC와 연결 
	afx_msg LRESULT OnEQDisconnect(WPARAM wParam, LPARAM lParam);				// 운영PC와 연결 끊김



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
	//GT.PARK - 운영PC와 통신이 연결 되지 않은 경우,  RECIPE정보를 먼저 운영 ini에서 가져오기 위한 코드
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


