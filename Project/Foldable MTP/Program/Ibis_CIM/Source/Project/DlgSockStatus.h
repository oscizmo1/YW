#pragma once

#include "GxSocket\GxSockServer.h"
#include "PublicClass/DataSocket.h"

// CDlgSockStatus dialog

class CDlgSockStatus : public CDialog
{
	DECLARE_DYNAMIC(CDlgSockStatus)

public:
	CDlgSockStatus(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSockStatus();

// Dialog Data
	enum { IDD = IDD_DLG_SOCK_STATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	BOOL m_bOnlyOnce;

	DECLARE_MESSAGE_MAP()
public:

	CGxSockServer*		m_pGxServer;
	BOOL SetGxSocketListen();																	// Socket 통신 수신처리


	virtual BOOL OnInitDialog();
	CDataSocket* m_pServerSocket;
	void CreateServerSocket();
	void DeleteServerSocket();

	BOOL RcvMsgFromAftClient(tag_Message* pData);
	BOOL RcvMsgFromMccClient(tag_Message* pData);

	BOOL SendMsgToControlPC(tag_Message* pData);
	BOOL GetSocketStatus();

protected:
	afx_msg LRESULT OnPacketReceive( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnReConnect( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnSocketClose( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnSocketConnect( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnUserMessage(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnTCPConnect(WPARAM wParam, LPARAM lParam);					// 운영PC와 연결 
	afx_msg LRESULT OnTCPDisconnect(WPARAM wParam, LPARAM lParam);				// 운영PC와 연결 끊김
	afx_msg LRESULT OnTCPReceive(WPARAM wParam, LPARAM lParam);					// 운영PC와 연결 


public:
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CCriticalSection csRcvMsg;
};


static CDlgSockStatus	*m_pSockStatus = NULL;
inline CDlgSockStatus	*GetSockStsDlg() 
{ 
	if(m_pSockStatus == NULL) 
	{
		m_pSockStatus = (CDlgSockStatus *) new CDlgSockStatus;
	}
	return m_pSockStatus; 
}