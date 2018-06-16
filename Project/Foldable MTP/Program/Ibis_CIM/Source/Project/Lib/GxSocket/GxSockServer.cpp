#include "stdafx.h"
#include "GxSocket\GxSockServer.h"
#include "GxSocket\GxSockClient.h"


CGxSockServer::CGxSockServer(int nID) : CAsyncSocket()
{
	m_bMultiConnect = FALSE;									// Client 접속 관리 모드, FALSE:단일 peer 접속만 허용, TRUE:다중 Peer 접속 허용
																		//단일 접속만 허용한다. Accept 발생시 기존 연결은 Close 한다.
	m_bMultiPeer		= FALSE;									// 동일 IP인 경우 다중 접속을 허용하지 않는다.

	m_sName		= _T("");
	m_nTag		= 0;

	m_nID			= nID;
	m_nIndex		= 0;												// Index 지정, 식별용

	m_pParent	= NULL;
	m_umAccept  = NULL;
	m_umClose	= NULL;
	m_umReceive	= NULL;

	m_nPort		= 0;

	m_nError		= 0;
	m_bCreate	= FALSE;

	m_nPktMode  = eGxCOMM_PKT_TOKEN;
	m_nPktIndex = 0;
	m_nPktLen   = 1;

	// Callback 함수
	m_pFncConnect	= NULL;										// 연결 이벤트 발생시 호출하는 Callback함수, Connect
	m_pFncClose		= NULL;										// 연결 끊김 이벤트 발생시 호출
	m_pFncReceive	= NULL;										// 수신 이벤트 발생시 호출하는 Callback함수
}

CGxSockServer::~CGxSockServer()
{
	Close();
}

//
//// nID = 0 base
//BOOL CGxSockServer::InitProfile(UINT nID, LPCTSTR pszFile)
//{
////	m_pSockMgr = CGxSockMgr::GetSockMgr();
//
//	m_nID = nID;
//
//	m_profile.m_sIniFile = pszFile;
//	m_profile.m_sSect.Format(_T("Server%d"), m_nID);
//	
//	m_profile.AddStr(_T("Name"),			m_sName);
//	m_profile.AddUint(_T("Type"),			m_nType);
//	m_profile.AddUint(_T("Index"),		m_nIndex);
//	m_profile.AddBool(_T("Simulate"),	m_bSimulate);
//
//	m_profile.AddUint(_T("Port"),			m_nPort);
//
//	return TRUE;
//}
//
//BOOL CGxSockServer::LoadProfile()
//{
//	m_profile.Load();
//	// m_trace.m_sFileName.Format(_T("Server%s"), m_sName);
//
//	return TRUE;
//}
//
//BOOL CGxSockServer::SaveProfile()
//{
//	m_profile.Save();
//
//	return TRUE;
//}
//
//BOOL CGxSockServer::Startup()
//{
//	Connect();
//
//	return TRUE;
//}


void CGxSockServer::HandleException(int nExp)
{
	m_debug.Write(_T("HandleException(%d)"), nExp);

	SetError( 1 ); // ERR_EXCEPTION);
}


// 다중 Client 접속을 허용할 것인가 ?
//
// Client 접속 관리 모드, 
// FALSE:단일 peer 접속만 허용, 
// TRUE:다중 Peer 접속 허용
void CGxSockServer::SetMultiConnect(BOOL bFlag)
{
	m_bMultiConnect = bFlag;
}


// 다중 Client 접속시 동일 IP에 대하여 다중 접속을 허용할 것인가 ?
//
//  m_bMultiConnect = TRUE 인 경우에만 해당
//				TRUE : 동일 IP의 다중 연결을 허용한다.
//				FALSE : 동일 IP인 경우 기존 연결은 끊고 새로운 연결을 받아들인다. 1개의 IP당 1개의 연결
//
void CGxSockServer::SetMultiPeer(BOOL bFlag)
{
	m_bMultiPeer = bFlag;
}


// 지정된 Index를 조회한다.
int CGxSockServer::GetIndex()
{
	return m_nIndex;
}

// 지정된 ID를 조회한다.
int CGxSockServer::GetID()
{
	return m_nID;
}


// 이름을 조회한다.
CString CGxSockServer::GetName()
{
	return m_sName;
}



int CGxSockServer::GetError()
{
	return m_nError;
}

void CGxSockServer::SetError(int nError)
{
	m_debug.Write(_T("SetError(%d)"), nError);
	
	m_nError = nError;
}

// Listen 중인가 ?
BOOL CGxSockServer::IsCreate()
{
	return m_bCreate;
}

// 접속된 Client 수
UINT CGxSockServer::GetClientCount()
{
	return m_listClient.GetCount();
}


// 지정 주소로 접속된 Client를 조회한다.
//
// CString sIP : 찾고자 하는 접속 IP
// int nPos : 몇번째로 나타나는 Client를 대상을 할것인가 ?
//
CGxSockClient* CGxSockServer::GetClient(CString sIP, int nPos)
{
	CGxSockClient* pClient = NULL;
	POSITION pos = m_listClient.GetHeadPosition();

	CString sIPAddr;
	UINT nPort;
	int nIdx = 0;

	// 모든 접속 객체에 대하여 
	while( pos != NULL )
	{
		pClient = m_listClient.GetNext(pos);
			
		// 정상적인 Client socket 객체라면
		if ( pClient != NULL )
		{
			pClient->GetPeerName(sIPAddr, nPort);				// 접속된 Client의 정보를 취득

			// 모두 끊거나 지정한 주소와 일치하면
			if ( sIPAddr.CompareNoCase( sIP ) == 0 )			// 지정된 IP를 가지는 Client를 찾았다.
			{
				// 지정한 순번째 찾아낸 Client인가 ?
				if ( nIdx == nPos )
				{
					return pClient;									// 해당 Client의 주소를 되돌려준다.
				}

				nIdx++;													// 찾아낸 순번 증가
			}
		} // of if normal pClient
	}//of while	

	return pClient;
}


// 지정 순번의 접속된 Client를 조회한다.
CGxSockClient* CGxSockServer::GetClient(int nIdx)
{
	CGxSockClient* pClient = NULL;
	POSITION pos = m_listClient.FindIndex( nIdx );			// 지정 Index의 위치를 조회

	if ( pos != NULL )												// 유효한 위치인 경우
	{
		pClient = m_listClient.GetAt(pos);						// 해당 위치의 객체를 조회한다.
	}

	return pClient;

}



// 각종 이벤트 메세지를 전송할 핸들러 치환
void CGxSockServer::SetMsgInfo(CWnd* pWnd, UINT umAccept, UINT umClose, UINT umReceive)
{
	if ( pWnd != NULL )
	{
		// 내부 변수 치환
		m_pParent   = pWnd;
		m_umAccept  = umAccept;
		m_umClose   = umClose;
		m_umReceive = umReceive;

		// 만약에 기존에 접속된 Client가 존재한다면 그 Client에게도 치환 작업을 한다.
		CGxSockClient* pClient = NULL;
		POSITION pos = m_listClient.GetHeadPosition();

		while( pos != NULL )
		{
			pClient = m_listClient.GetNext(pos);
			if ( pClient != NULL )
			{
				pClient->SetMsgInfo(pWnd, umAccept, umClose, umReceive);
			}
		}
	}//of if pWnd
}


// 패킷의 구분을 정의한다.
// byToken = Only eGxCOMM_PKT_TOKEN
// nIndex  = Only PKT_SIZE
// nLen    = Only PKT_SIZE
//
void CGxSockServer::SetPacketInfo(UINT nMode, CByteArray* parrToken, int nIndex, UINT nLen)
{
	// 내부 변수 치환
	m_nPktMode   = nMode;
	m_nPktIndex  = nIndex;
	m_nPktLen    = nLen;

	// 지정 토큰 사용 모드일 경우
	if( nMode == eGxCOMM_PKT_TOKEN )
	{
		if ( parrToken != NULL )
		{
			m_arrPktToken.Copy(*parrToken);
		}
	}


	// 만약에 기존에 접속된 Client가 존재한다면 그 Client에게도 치환 작업을 한다.
	CGxSockClient* pClient = NULL;
	POSITION pos = m_listClient.GetHeadPosition();

	while( pos != NULL )
	{
		pClient = m_listClient.GetNext(pos);

		if ( pClient != NULL )
		{
			pClient->SetPacketInfo(nMode, parrToken, nIndex, nLen);
		}
	}
}



// Callback 함수 대입
//
// 함수를 대입하고자 하는 곳에서는 아래와 같이 호출을 한다.
//
// CMyClass : 호출할 함수가 존재하는 Class
// void 
//	typedef TCallbackDefine< CMyClass > TmyCallbackDefine;
//	m_ClientSocket.SetCallbackFunction( new TmyCallbackDefine(this, &CMyClass::OnConnect), 
//							new TmyCallbackDefine(this, &CMyClass::OnClose), new TmyCallbackDefine(this, &CMyClass::OnRecv) );
//
void CGxSockServer::SetCallbackFunction(CGxCallback* pConnect, CGxCallback* pClose, CGxCallback* pReceive)			
{
	// 내부 변수 치환
	m_pFncConnect	= pConnect;								// Connect 이벤트 발생시 호출하는 Callback함수
	m_pFncClose		= pClose;									// Close 이벤트 발생시 호출
	m_pFncReceive	= pReceive;									// 수신 이벤트 발생시 호출하는 Callback함수


	// 만약에 기존에 접속된 Client가 존재한다면 그 Client에게도 치환 작업을 한다.
	CGxSockClient* pClient = NULL;
	POSITION pos = m_listClient.GetHeadPosition();

	while( pos != NULL )
	{
		pClient = m_listClient.GetNext(pos);

		if ( pClient != NULL )
		{
			pClient->SetCallbackFunction(pConnect, pClose, pReceive);			// 처리 함수 치환
		}
	}
}



// 포트 지정
void CGxSockServer::SetPort(int nPort)
{
	m_nPort = (UINT)nPort;
}

// 지정한 Port로 Listen을 시작한다.
BOOL CGxSockServer::Listen(int nPort)
{
	m_nPort = (UINT)nPort;

	return Listen();
}

// 기존 접속된 Client를 삭제하고 새로이 접속을 시작하도록한다.
BOOL CGxSockServer::Listen()
{
	BOOL bRet = FALSE;

	try
	{
		Close();								// 기존 접속을 모두 종료한다.

		bRet = CAsyncSocket::Create(m_nPort);
		if ( bRet == TRUE )
		{
			// 생성과  Listen까지 정상적으로 마쳐야 성공
			bRet = CAsyncSocket::Listen();

			m_bCreate = bRet;
			//?m_debug.Write(_T("Created(port:%d)"), m_nPort);
		}
	}
	catch( int nExp )
	{
		HandleException(nExp);
		return FALSE;
	}

	return bRet;
}

// Server socket을 닫는다.
void CGxSockServer::Close()
{

	try
	{
		m_bCreate = FALSE;

		CGxSockClient* pClient = NULL;
		POSITION pos = m_listClient.GetHeadPosition();

		while( pos != NULL )
		{
			pClient = m_listClient.GetNext(pos);
			
			// 정상적인 Client socket 객체라면
			if ( pClient != NULL )
			{
				pClient->ShutDown();
				pClient->Close();

				delete pClient;
			}
		}

		// list에 등록된 모든 Clent socket을 삭제한다.
		m_listClient.RemoveAll();

		CAsyncSocket::Close();
	}
	catch( int nExp )
	{
		HandleException(nExp);
		return;
	}
}


//  지정 / 모든 연결을 Close 시킨다.
void	CGxSockServer::Disconnect(CString sPeerIP)
{
	BOOL bAll = sPeerIP.IsEmpty();			// 공백을 넘겨온 경우 연결된 모든 Peer를 끊는다.

	CGxSockClient* pClient = NULL;
	POSITION pos = m_listClient.GetHeadPosition();
	POSITION posRemove = NULL;				// 삭제할 Client
	CString sIPAddr;
	UINT nPort;

	// 모든 접속 객체에 대하여 
	while( pos != NULL )
	{
		pClient = m_listClient.GetNext(pos);
			
		// 정상적인 Client socket 객체라면
		if ( pClient != NULL )
		{
			pClient->GetPeerName(sIPAddr, nPort);				// 접속된 Client의 정보를 취득

			// 모두 끊거나 지정한 주소와 일치하면
			if ( bAll || (sIPAddr.CompareNoCase( sPeerIP ) == 0) )
			{
				posRemove = m_listClient.Find( pClient );		// 삭제 대상 위치 탐색

				pClient->ShutDown();
				pClient->Close();

				delete pClient;

				m_listClient.RemoveAt( posRemove );				// 해당 Socket을 List에서 제거한다.
			}

		} // of if normal pClient
	}//of while
}


// BYTE * 로 전송 == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==

// 지정 Client에게 데이터를 전송한다.
//
// CString sIP : "" 널인경우에는 접속된 모든 Client에게 전송을 한다.
//
int CGxSockServer::Send(CString sIP, const void* pBuff, int nLen)
{
	BOOL bAll = sIP.IsEmpty();		// 모든 Client에게 전송을 할것인가 ?

	CGxSockClient* pClient = NULL;
	POSITION pos = m_listClient.GetHeadPosition();
	CString sIPAddr;
	UINT nPort;
	int nRet = 0;

	// 모든 접속 객체에 대하여 
	while( pos != NULL )
	{
		pClient = m_listClient.GetNext(pos);
			
		// 정상적인 Client socket 객체라면
		if ( pClient != NULL )
		{
			pClient->GetPeerName(sIPAddr, nPort);				// 접속된 Client의 정보를 취득

			// 모두에게 전송하거나 지정한 주소와 일치하면
			if ( bAll || (sIPAddr.CompareNoCase( sIP ) == 0) )
			{
				try
				{
					nRet = pClient->Send(pBuff, nLen);				// 해당 Client에게 전송한다.
				}
				catch( int nExp )
				{
					HandleException(nExp);
					return 0;
				}
			}

		} // of if normal pClient
	}//of while

	return nRet;
}



// 지정 Client에게 데이터를 전송한다.
int CGxSockServer::Send(CGxSockClient* pClient, const void* pBuff, int nLen)
{
	// 정상적인 Client socket 객체라면
	if ( pClient != NULL )
	{
		POSITION pos = m_listClient.Find( pClient );			// 지정한 Client를 찾는다.

		// 정상적으로 등록된 Client라면,
		if ( pos != NULL )
		{
			try
			{
				return pClient->Send(pBuff, nLen);				// 해당 Client에게 전송한다.
			}
			catch( int nExp )
			{
				HandleException(nExp);
				return 0;
			}
		}
	} // of if normal pClient

	return 0;			// 전송 오류
}



// 지정 순번의 Client에게 데이터를 전송한다.
int CGxSockServer::Send(int nIdx, const void* pBuff, int nLen)
{
	CGxSockClient* pClient = GetClient(nIdx);						// 지정 순번의 Client를 조회한다.

	if ( pClient != NULL )												// Client가 존재한다면,
	{
		try
		{
			return pClient->Send(pBuff, nLen);						// 해당 Client에게 전송한다.
		}
		catch( int nExp )
		{
			HandleException(nExp);
			return 0;
		}
	}

	return 0;
}

// 접속한 모든 Client에게 데이터를 전송한다.
int CGxSockServer::Send(const void* pBuff, int nLen)
{
	CGxSockClient* pClient = NULL;
	POSITION pos = m_listClient.GetHeadPosition();
	int nRet = 0;

	// 모든 접속 객체에 대하여 
	while( pos != NULL )
	{
		pClient = m_listClient.GetNext(pos);
			
		// 정상적인 Client socket 객체라면
		if ( pClient != NULL )
		{
			try
			{
				nRet = pClient->Send(pBuff, nLen);				// 해당 Client에게 전송한다.
			}
			catch( int nExp )
			{
				HandleException(nExp);
				
			}
		} // of if normal pClient
	}//of while

	return nRet;
}





// CString으로 전송 == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==

// 지정 IP를 가지는 Client에게 String 데이터를 전송한다.
int CGxSockServer::SendString(CString sIP, CString sMsg)
{
	BOOL bAll = sIP.IsEmpty();		// 모든 Client에게 전송을 할것인가 ?

	CGxSockClient* pClient = NULL;
	POSITION pos = m_listClient.GetHeadPosition();
	CString sIPAddr;
	UINT nPort;
	int nRet = 0;

	// 모든 접속 객체에 대하여 
	while( pos != NULL )
	{
		pClient = m_listClient.GetNext(pos);
			
		// 정상적인 Client socket 객체라면
		if ( pClient != NULL )
		{
			pClient->GetPeerName(sIPAddr, nPort);				// 접속된 Client의 정보를 취득

			// 모두에게 전송하거나 지정한 주소와 일치하면
			if ( bAll || (sIPAddr.CompareNoCase( sIP ) == 0) )
			{
				try
				{
					nRet = pClient->SendString( sMsg );			// 해당 Client에게 전송한다.
				}
				catch( int nExp )
				{
					HandleException(nExp);
					return 0;
				}
			}

		} // of if normal pClient
	}//of while

	return nRet;
}


// 지정 Client에게 Strig 데이터를 전송한다.
int CGxSockServer::SendString(CGxSockClient* pClient, CString sMsg)
{
	// 정상적인 Client socket 객체라면
	if ( pClient != NULL )
	{
		POSITION pos = m_listClient.Find( pClient );			// 지정한 Client를 찾는다.

		// 정상적으로 등록된 Client라면,
		if ( pos != NULL )
		{
			try
			{
				return pClient->SendString( sMsg );				// 해당 Client에게 전송한다.
			}
			catch( int nExp )
			{
				HandleException(nExp);
				return 0;
			}
		}
	} // of if normal pClient

	return 0;			// 전송 오류
}


// 지정 순번의 Client에게 String 데이터를 전송한다.
int CGxSockServer::SendString(int nIdx, CString sMsg)
{
	CGxSockClient* pClient = GetClient(nIdx);						// 지정 순번의 Client를 조회한다.

	if ( pClient != NULL )												// Client가 존재한다면,
	{
		try
		{
			return pClient->SendString( sMsg );						// 해당 Client에게 전송한다.
		}
		catch( int nExp )
		{
			HandleException(nExp);
			return 0;
		}
	}

	return 0;
}

// 접속한 모든 Client에게 String 데이터를 전송한다.
int CGxSockServer::SendString(CString sMsg)
{
	CGxSockClient* pClient = NULL;
	POSITION pos = m_listClient.GetHeadPosition();
	int nRet = 0;

	// 모든 접속 객체에 대하여 
	while( pos != NULL )
	{
		pClient = m_listClient.GetNext(pos);
			
		// 정상적인 Client socket 객체라면
		if ( pClient != NULL )
		{
			try
			{
				nRet = pClient->SendString( sMsg );				// 해당 Client에게 전송한다.
			}
			catch( int nExp )
			{
				HandleException(nExp);				
			}
		} // of if normal pClient
	}//of while

	return nRet;
}


// Client와의 접속이 이루어졌다.
void CGxSockServer::OnAccept(int nErrorCode)
{
	if ( nErrorCode != 0 ) return;			//오류가 발생하였다면 Accept 처리를 하지 않는다.

	try
	{
		CGxSockClient* pClient = new CGxSockClient(m_nID, this);
		BOOL bRet = CAsyncSocket::Accept( *pClient );

		// Accept 성공
		if ( bRet == TRUE )
		{
			if ( !m_bMultiConnect )					// 다중 접속이 아닐 경우에는 기존 접속을 모두 종료시킨다.
			{
				Disconnect();							// 모든 접속 끊기
			}
			else											// 다중 접속인 경우
				if ( !m_bMultiPeer )
				{
					CString sIP;
					UINT nPort;

					// 접속을 요구한 IP와 동일한 IP가 이미 존재한다면 해당 IP의 연결은 끊는다.
					pClient->GetPeerName( sIP, nPort );		
					Disconnect( sIP );
				}

			// 해당 Client socket에서 발생시킬 Event Message Handle을 등록한다.
			pClient->SetMsgInfo(m_pParent, m_umAccept, m_umClose, m_umReceive, NULL);
			pClient->SetPacketInfo(m_nPktMode, &m_arrPktToken, m_nPktIndex, m_nPktLen);
			pClient->SetCallbackFunction(m_pFncConnect, m_pFncClose, m_pFncReceive, NULL);			// 처리 함수 치환

			pClient->m_sName	= m_sName;							// Server와 동일한 구분자를 대입시켜준다.
			pClient->m_nIndex	= m_listClient.GetCount();		// 몇번째로 추기되는 Client인지 index를 대입시켜준다.

			m_listClient.AddTail(pClient);				// Client List에 추가한다.

			// Message를 보내도록 Handler가 정상적으로 등록되었다면
			if( (m_umAccept != NULL) && ( m_pParent != NULL ) )
			{
				// WPARAM : Event를 호출한 Server socket
				// LPARAM : Accept된 ClientSocket
				m_pParent->PostMessage(m_umAccept, WPARAM(this), LPARAM(pClient));		// Accept Message 전송
			}

			// Event발생시 호출할 Callback 함수가 지정되어있을 때 해당 함수 호출
			if ( m_pFncConnect )
			{
				m_pFncConnect->Call( pClient );						// Callback 함수 호출
			}

		} //of if Accept sucess
		else		// 바인딩 실패시 해당 소켓 삭제
		{
			pClient->Close();
			delete pClient;
		}

		CAsyncSocket::OnAccept(nErrorCode);
	}
	catch( int nExp )
	{
		HandleException(nExp);
		return;
	}
}

// Bind된 Clinet를 삭제한다.
// Client Socket의 Close를 수행시키고 관리 리스트에서 제거한다.
void CGxSockServer::OnCloseClient(CGxSockClient* pClient)
{
	try
	{
		// 연결된 Client list에서 해당 client 획득
		POSITION pos = m_listClient.Find( pClient );

		// 정상적인 Clent socket 인가 ?
		if ( pos != NULL )
		{
			// 종료 처리
			pClient->ShutDown();
			pClient->Close();

			// 객체를 삭제하고 list에서 제거한다.
			delete pClient;
			m_listClient.RemoveAt(pos);
		}
	}
	catch( int nExp )
	{
		HandleException(nExp);
		return;
	}
}

