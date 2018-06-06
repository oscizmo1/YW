#pragma once
#include "Socket/DataSocket.h"
#include "resource.h"
#include "BankGuild/CellBank.h"
//kjpark 20161021 Setup 신호기 UI 구현
#include "GUIDefine.h"
#include "ETC/Queue.h"

#include "DeviceSocket_PGHost.h"
#include "DeviceSocket_CIM.h"
#include "DeviceSocket_MCR.h"
#include "DeviceSocket_ActiveAlign.h"


#define SOCKET_MAIN		_T("MAIN")		// Socket Index
enum
{
	TIMER_RE_CONNECT = 0,
	TIMER_MAX
};


// CSocketDialog 대화 상자입니다.
//class CCellBank;
class CSocketDialog : public CDialog
{
	DECLARE_DYNAMIC(CSocketDialog)

public:
	CSocketDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSocketDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CSOCKETDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	afx_msg LRESULT OnPacketReceive( WPARAM wParam, LPARAM lParam );
	//kjpark 20160919 Topdlg에 connect status 
	afx_msg LRESULT OnSocketConnect( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnSocketClose( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()

public:
	void DeleteAllSocket();					//모든 소켓을 닫는다. 
	void SocketClosed(CDataSocket *pSocket, CClientSocket* pClientSocket);

	LRESULT OnReConnect( WPARAM wParam, LPARAM lParam );
	//kjpark 20160919 Topdlg에 connect status 	
	int GetSocketIndex(CString sName);
	BOOL GetCommConnected(int nIdx);				// 2014-12-11, jhLee, 지정 통신 연결 확인


	//////////////////////////////////////
	//			Pattern Genarator		//
	//////////////////////////////////////
	CDeviceSocket_PGHost m_PGHost;

	//////////////////////////////////////
	//				UV MCR		//
	//////////////////////////////////////
	CDeviceSocket_MCR m_MCR[JIG_ID_MAX][JIG_CH_MAX];

	//////////////////////////////////////
	//				Data PC				//
	//////////////////////////////////////
	CDeviceSocket_CIM m_CIM;

	//////////////////////////////////////
	//			Avtive_Align			//
	//////////////////////////////////////
//	CDeviceSocket_ActiveAlign m_ActiveAlign;

	void SetSocketConnect();

private:
	//kjpark 20160919 Topdlg에 connect status 
	afx_msg void OnTimer(UINT nIDEvent);
};
