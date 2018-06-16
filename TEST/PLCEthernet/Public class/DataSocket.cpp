// DataSocket.cpp : implementation file
//

#include "stdafx.h"
#include "DataSocket.h"
static CClientSocket* pClientSocket = NULL;	// 일단 한개만 만들었다

/////////////////////////////////////////////////////////////////////////////
// CDataSocket
CDataSocket::CDataSocket()
{
	m_PortNo = -1;
	m_fSocketCreated = FALSE;
	m_fSocketConnected = FALSE;
	m_strName = _T("");
	m_bIsServer = TRUE;
	m_hRcvWnd = NULL;
}

CDataSocket::~CDataSocket()
{
}
/////////////////////////////////////////////////////////////////////////////
// CDataSocket member functions

void CDataSocket::OnReceive(int nErrorCode) 
{
	m_Length = Receive(m_lastMessage, MAX_PACKET_BUFFER);
	switch (m_Length)
	{
	case 0:
		Close();
		break;
	case SOCKET_ERROR:
		if (GetLastError() != WSAEWOULDBLOCK) 
		{
			if (GetLastError() != WSAEMSGSIZE) {
				TRACE("OnReceive error: %d", GetLastError());
			} else {
				TRACE("The datagram was too large and was truncated");
			}
		}
		break;

	default:
		if (m_Length != SOCKET_ERROR && m_Length != 0) {
			m_lastMessage[m_Length] = NULL;

			if (m_hRcvWnd != NULL)
				::SendMessage(m_hRcvWnd, WM_PACKET_RECEIVE, (WPARAM)this, NULL);
		}	
	}	
	memset(m_lastMessage, NULL, sizeof(m_lastMessage));
	CSocket::OnReceive(nErrorCode);
}

void CDataSocket::OnClose(int nErrorCode) 
{
	m_fSocketCreated = FALSE;
	m_fSocketConnected = FALSE;
	if(pClientSocket)
	{
		pClientSocket->Close();
		delete pClientSocket;
		pClientSocket = NULL;
	}
	CSocket::OnClose(nErrorCode);
}


void CDataSocket::OnConnect(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class

	CSocket::OnConnect(nErrorCode);
}

void CDataSocket::OnAccept(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_fSocketConnected)	
	{
		pClientSocket->Close();
		delete pClientSocket;
		pClientSocket = NULL;
		m_fSocketConnected = FALSE;
	}
	pClientSocket = new CClientSocket;
	if (CSocket::Accept(*pClientSocket))
	{
		pClientSocket->SetServerSocket(this);
		m_fSocketConnected = TRUE;
	}
	else
	{
		pClientSocket->Close();
		delete pClientSocket;
		pClientSocket = NULL;
		AfxMessageBox(_T("Error : Accept() in socket"));
	}

	CSocket::OnAccept(nErrorCode);
}

BOOL CDataSocket::SendClient(const void* lpBuf, int nBufLen)
{
	if( nBufLen == pClientSocket->Send(lpBuf, nBufLen) )
		return TRUE;
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CClientSocket
CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}
void CClientSocket::OnReceive(int nErrorCode)
{
	m_pServerSocket->m_Length = Receive(m_pServerSocket->m_lastMessage, MAX_PACKET_BUFFER);
	if (m_pServerSocket->GetReciveHwnd() != NULL)
		::SendMessage(m_pServerSocket->GetReciveHwnd(), WM_PACKET_RECEIVE, (WPARAM)m_pServerSocket, NULL);

	switch (m_pServerSocket->m_Length)
	{
	case 0:
		Close();
		break;
	case SOCKET_ERROR:
		if (GetLastError() != WSAEWOULDBLOCK) 
		{
			if (GetLastError() != WSAEMSGSIZE) {
				TRACE("OnReceive error: %d", GetLastError());
			} else {
				TRACE("The datagram was too large and was truncated");
			}
		}
		break;

	default:
		if (m_pServerSocket->m_Length != SOCKET_ERROR && m_Length != 0) {
			m_pServerSocket->m_lastMessage[m_pServerSocket->m_Length] = NULL;

//			if (m_pServerSocket->GetReciveHwnd() != NULL)
//				::SendMessage(m_pServerSocket->GetReciveHwnd(), WM_PACKET_RECEIVE, (WPARAM)m_pServerSocket, NULL);
		}	
	}	
	memset(m_pServerSocket->m_lastMessage, NULL, sizeof(m_pServerSocket->m_lastMessage));

	CAsyncSocket::OnReceive(nErrorCode);
}

void CClientSocket::OnClose(int nErrorCode)
{
	m_pServerSocket->SetConnected(FALSE);

	CAsyncSocket::OnClose(nErrorCode);
}

