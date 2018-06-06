#pragma once
#include "Socket/DataSocket.h"
#include "resource.h"
#include "BankGuild/CellBank.h"
//kjpark 20161021 Setup ��ȣ�� UI ����
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


// CSocketDialog ��ȭ �����Դϴ�.
//class CCellBank;
class CSocketDialog : public CDialog
{
	DECLARE_DYNAMIC(CSocketDialog)

public:
	CSocketDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSocketDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CSOCKETDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	afx_msg LRESULT OnPacketReceive( WPARAM wParam, LPARAM lParam );
	//kjpark 20160919 Topdlg�� connect status 
	afx_msg LRESULT OnSocketConnect( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnSocketClose( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()

public:
	void DeleteAllSocket();					//��� ������ �ݴ´�. 
	void SocketClosed(CDataSocket *pSocket, CClientSocket* pClientSocket);

	LRESULT OnReConnect( WPARAM wParam, LPARAM lParam );
	//kjpark 20160919 Topdlg�� connect status 	
	int GetSocketIndex(CString sName);
	BOOL GetCommConnected(int nIdx);				// 2014-12-11, jhLee, ���� ��� ���� Ȯ��


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
	//kjpark 20160919 Topdlg�� connect status 
	afx_msg void OnTimer(UINT nIDEvent);
};
