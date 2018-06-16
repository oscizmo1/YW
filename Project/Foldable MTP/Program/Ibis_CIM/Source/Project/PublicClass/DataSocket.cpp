// DataSocket.cpp : implementation file
//

#include "stdafx.h"
#include "DataSocket.h"

#include "../GaonNuri.h"



static CList <CClientSocket*, CClientSocket*> listClient;
// static CClientSocket* pClientSocket = NULL;	// 일단 한개만 만들었다


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

	m_Length = 0;
	m_nRemainCount = 0;
}

CDataSocket::~CDataSocket()
{
// 	listClientName.RemoveAll();
// 
// 	CClientSocket* pClient;
// 	POSITION pos = listClient.GetHeadPosition();
// 	while(pos)
// 	{
// 		pClient = listClient.GetNext(pos);
// 		pClient->Close();
// 		delete pClient;
// 	}
// 	listClient.RemoveAll();
}
/////////////////////////////////////////////////////////////////////////////
// CDataSocket member functions

void CDataSocket::OnReceive(int nErrorCode) 
{
	int nRtnLen = Receive(m_lastMessage+m_nRemainCount, MAX_PACKET_BUFFER-m_nRemainCount);
	m_Length = nRtnLen;
	switch (nRtnLen)
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
		m_Length = 0;
		break;
	default:
		if(nRtnLen>0)
		{
			m_lastMessage[m_nRemainCount+nRtnLen] = NULL;
			if (m_hRcvWnd != NULL)
			{
				m_nRemainCount = ::SendMessage(m_hRcvWnd, WM_PACKET_RECEIVE, (WPARAM)this, m_nRemainCount+nRtnLen);
			}
		}
	}	


	memset(m_lastMessage+m_nRemainCount, NULL, sizeof(char)*(MAX_PACKET_BUFFER-m_nRemainCount));
	CAsyncSocket::OnReceive(nErrorCode);
}

void CDataSocket::OnClose(int nErrorCode) 
{
	m_fSocketCreated = FALSE;
	m_fSocketConnected = FALSE;

	CClientSocket* pClient;
	POSITION pos = listClient.GetHeadPosition();
	while(pos)
	{
		pClient = listClient.GetNext(pos);
		pClient->Close();
		delete pClient;
	}
	listClient.RemoveAll();

	CAsyncSocket::OnClose(nErrorCode);
}


void CDataSocket::OnConnect(int nErrorCode)
{
	if(nErrorCode==0)
	{
		m_fSocketConnected = TRUE;
		// ::SendMessage(m_hRcvWnd, WM_SOCKET_CONNECT, (WPARAM)this, NULL);
		::PostMessage(m_hRcvWnd, WM_SOCKET_CONNECT, (WPARAM)this, NULL);
	}
	else
	{
      switch(nErrorCode)
      {
         case WSAEADDRINUSE: 
            TRACE(_T("The specified address is already in use.\n"));
            break;
         case WSAEADDRNOTAVAIL: 
            TRACE(_T("The specified address is not available from ")
            _T("the local machine.\n"));
            break;
         case WSAEAFNOSUPPORT: 
            TRACE(_T("Addresses in the specified family cannot be ")
            _T("used with this socket.\n"));
            break;
         case WSAECONNREFUSED: 
//             TRACE(_T("The attempt to connect was forcefully rejected.\n"));
			if(m_bIsServer == FALSE)					// Socket	  Create 여부
				::SendMessage(m_hRcvWnd, WM_RE_CONNECT, (WPARAM)this, FALSE);
           break;
         case WSAEDESTADDRREQ: 
            TRACE(_T("A destination address is required.\n"));
            break;
         case WSAEFAULT: 
            TRACE(_T("The lpSockAddrLen argument is incorrect.\n"));
            break;
         case WSAEINVAL: 
            TRACE(_T("The socket is already bound to an address.\n"));
            break;
         case WSAEISCONN: 
            TRACE(_T("The socket is already connected.\n"));
            break;
         case WSAEMFILE: 
            TRACE(_T("No more file descriptors are available.\n"));
            break;
         case WSAENETUNREACH: 
            TRACE(_T("The network cannot be reached from this host ")
            _T("at this time.\n"));
            break;
         case WSAENOBUFS: 
            TRACE(_T("No buffer space is available. The socket ")
               _T("cannot be connected.\n"));
            break;
         case WSAENOTCONN: 
            TRACE(_T("The socket is not connected.\n"));
            break;
         case WSAENOTSOCK: 
            TRACE(_T("The descriptor is a file, not a socket.\n"));
            break;
         case WSAETIMEDOUT: 
//             TRACE(_T("The attempt to connect timed out without ")
//                _T("establishing a connection. \n"));
			if(m_bIsServer == FALSE)					// Socket	  Create 여부
				::SendMessage(m_hRcvWnd, WM_RE_CONNECT, (WPARAM)this, FALSE);
           break;
         default:
//             TCHAR szError[256];
//             _stprintf_s(szError, _T("OnConnect error: %d"), nErrorCode);
//             TRACE(szError);
			if(m_bIsServer == FALSE)					// Socket	  Create 여부
				::SendMessage(m_hRcvWnd, WM_RE_CONNECT, (WPARAM)this, FALSE);
          break;
      }
//       TRACE(_T("Please close the application"));
   }


	CAsyncSocket::OnConnect(nErrorCode);
}

void CDataSocket::OnAccept(int nErrorCode)
{
	if(IsServer() == FALSE)
		return;

	CClientSocket* pClient = new CClientSocket;
	if (CAsyncSocket::Accept(*pClient))
	{
		CString strIP;
		UINT nPort;
		pClient->SetServerSocket(this);
		pClient->GetPeerName(strIP, nPort);
		pClient->SetIP(strIP);
		pClient->SetPort(nPort);
		pClient->SetReciveHwnd( m_hRcvWnd );				// 핸들을 넘겨준다.
		AddClient(pClient);
		m_fSocketConnected = TRUE;

		::PostMessage(m_hRcvWnd, WM_SOCKET_CONNECT, (WPARAM)this, NULL);
	}
	else
	{
		pClient->Close();
		delete pClient;
		pClient = NULL;
		AfxMessageBox(_T("Error : Accept() in socket"));
	}

	CAsyncSocket::OnAccept(nErrorCode);
}

BOOL CDataSocket::SendClient( CString strSocketName, const void* lpBuf, int nBufLen )
{
	CClientSocket* pSocket;
	POSITION pos = listClient.GetHeadPosition();
	while(pos)
	{
		pSocket = listClient.GetNext(pos);
		if(pSocket->GetSocketName() == strSocketName)
		{
			if( nBufLen == pSocket->Send(lpBuf, nBufLen) )
				return TRUE;
			break;
		}
	}

	return FALSE;
}

BOOL CDataSocket::ConnectToServer()
{
	if(m_strIP == _T("") || m_PortNo == 0 || m_fSocketCreated == FALSE)
		return FALSE;

	return Connect(m_strIP, m_PortNo);
}

void CDataSocket::AddClient( CClientSocket* pClient )
{
	// 기존에 연결되있는게 있으면 해제한다.
	CClientSocket* pSocket;
	POSITION pos = listClient.GetHeadPosition();
	while(pos)
	{
		pSocket = listClient.GetNext(pos);
		if(pSocket->GetIP() == pClient->GetIP())
		{
			pSocket->Close();
			delete pSocket;
			if(pos)
			{
				listClient.GetPrev(pos);
				listClient.RemoveAt(pos);
			}
			else
			{
				listClient.RemoveAt(listClient.GetTailPosition());
			}
			break;
		}
	}

	// 소켓 이름을 부여한다.
	ClientSocketName socketname;
	pos = listClientName.GetHeadPosition();
	while(pos)
	{
		socketname = listClientName.GetNext(pos);
		if(pClient->GetIP() == socketname.IP)
		{
			pClient->SetSocketName(socketname.Name);
			listClient.AddTail(pClient);
			pClient->SetConnected(TRUE);
			return;
		}
	}
}

void CDataSocket::DeleteClient()
{
	CClientSocket* pClient;
	POSITION pos = listClient.GetHeadPosition();
	while(pos)
	{
		pClient = listClient.GetNext(pos);
		pClient->Close();
		delete pClient;
	}
	listClient.RemoveAll();
}

CClientSocket* CDataSocket::GetClientAt( CString socketname )
{
	// 기존에 연결되있는게 있으면 해제한다.
	CClientSocket* pSocket = NULL;
	POSITION pos = listClient.GetHeadPosition();
	while(pos)
	{
		pSocket = listClient.GetNext(pos);
		if(pSocket->GetSocketName() == socketname)
			return pSocket;
		else 
			pSocket = NULL;
	}
	return pSocket;
}
/////////////////////////////////////////////////////////////////////////////
// CClientSocket
CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}

void CClientSocket::OnReceive(int nErrorCode)		//이리로 온다
{

	if(this->GetSocketName() == SOCKET_NAME_CLIENT_EQ)	
	{
		tag_Message* pData = new tag_Message;
		ZeroMemory(pData, sizeof(tag_Message));

		BOOL m_bReadOK = FALSE; 
		while(!m_bReadOK)			//누락 되면... 과연 TCP가...
		{
			Sleep(70);
			DWORD dwReadSize=0;
			IOCtl( FIONREAD, &dwReadSize );
			if (dwReadSize >= sizeof(tag_Message))
			{
				if(Receive(pData, sizeof(tag_Message)))
				{
					::SendMessage(m_pServerSocket->GetReciveHwnd(), WM_PACKET_RECEIVE, (WPARAM)this, (LPARAM)pData);
					m_bReadOK = TRUE;
				}
			}
		}
		delete pData;
	}
	else
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

				if (m_pServerSocket->GetReciveHwnd() != NULL)
					::SendMessage(m_pServerSocket->GetReciveHwnd(), WM_PACKET_RECEIVE, (WPARAM)this, NULL);
			}	
		}

		memset(m_lastMessage, NULL, sizeof(m_lastMessage));
	}

	CAsyncSocket::OnReceive(nErrorCode);
}

void CClientSocket::OnClose(int nErrorCode)
{
	m_pServerSocket->SetConnected(FALSE);
	m_fSocketCreated = FALSE;
	m_fSocketConnected = FALSE;

	::PostMessage(m_hRcvWnd, WM_SOCKET_CLOSE, (WPARAM)this, NULL);

	CAsyncSocket::OnClose(nErrorCode);
}

void CClientSocket::WaitDataOrEvent(CSocket* sc, void* pBuff, DWORD dwGoal, HANDLE hEvent)
{
	DWORD avail = 0;
	while (avail < dwGoal)
	{
		if (WaitForSingleObject(hEvent, 0) == WAIT_OBJECT_0)
		throw SOCKET_ERROR;
		sc->IOCtl(FIONREAD, &avail);

	}
}
