#include "stdafx.h"
#include "GxSocket\GxSockClient.h"
#include "GxSocket\GxSockServer.h"

// #include "GxSockMgr.h"


CGxSockClient::CGxSockClient(int nID, CGxSockServer* pServer/* =NULL */) : CAsyncSocket()
{
	m_pServer			= pServer;

	m_sName				= _T("");
	m_nTag				= 0;

	m_nID					= nID;
	m_nIndex				= 0;						

	m_sIP					= _T("");
	m_nPort				= 0;

	m_pOwner				= NULL;
	m_umConnect			= NULL;
	m_umClose			= NULL;
	m_umReceive			= NULL;
	m_umConnectFail	= NULL;						// 연결 재시도 요청


	m_nError				= 0;
	m_bConnect			= FALSE;

	// Callback 함수 
	m_pFncConnect		= NULL;						// 연결요청 이벤트 발생시 호출하는 Callback함수, Connect/Accept
	m_pFncClose			= NULL;						// 연결 끊김 이벤트 발생시 호출
	m_pFncReceive		= NULL;						// 수신 이벤트 발생시 호출하는 Callback함수
	m_pFncConnectFail	= NULL;						// 연결시도가 실패하였을때 호출하는 Callback함수
}

CGxSockClient::~CGxSockClient()
{
	Close();
}

//
//// nID = 0 base
//BOOL CGxSockClient::InitProfile(UINT nID, LPCTSTR pszFile)
//{
////	m_pSockMgr = CGxSockMgr::GetSockMgr();
//
//	m_nID = nID;
//
//	m_profile.m_sIniFile = pszFile;
//	m_profile.m_sSect.Format(_T("Client%d"), m_nID);
//
//	m_profile.AddStr(_T("Name"),			m_sName);
//	m_profile.AddUint(_T("Type"),			m_nType);
//	m_profile.AddUint(_T("Index"),		m_nIndex);
//	m_profile.AddBool(_T("Simulate"),	m_bSimulate);
//
//	m_profile.AddStr(_T("IP"),				m_sIP);
//	m_profile.AddUint(_T("Port"),			m_nPort);
//
//	return TRUE;
//}
//
//BOOL CGxSockClient::LoadProfile()
//{
//	m_profile.Load();
//	// m_trace.m_sFileName.Format(_T("Client%s"), m_sName);
//
//	return TRUE;
//}
//
//BOOL CGxSockClient::SaveProfile()
//{
//	m_profile.Save();
//
//	return TRUE;
//}
//
//BOOL CGxSockClient::Startup()
//{
//	Connect();
//
//	return TRUE;
//}


void CGxSockClient::HandleException(int nExp)
{
	m_debug.Write(_T("HandleException(%d)"), nExp);
	
	SetError( 1 );
}


// 지정된 Index를 조회한다.
int CGxSockClient::GetIndex()
{
	return m_nIndex;
}

// 지정된 ID를 조회한다.
int CGxSockClient::GetID()
{
	return m_nID;
}


// 이름을 조회한다.
CString CGxSockClient::GetName()
{
	return m_sName;
}




int CGxSockClient::GetError()
{
	return m_nError;
}

void CGxSockClient::SetError(int nError)
{
	m_debug.Write(_T("SetError(%d)"), nError);
	
	m_nError = nError;
}

BOOL CGxSockClient::IsConnect()
{
	return m_bConnect;
}


// 패킷 구분자 모드
UINT CGxSockClient::GetPacketMode(int& nIdx, int& nLen)
{
	return m_RcvQueue.GetPacketMode(nIdx, nLen);
}


// 각종 Event Handler 대입
void CGxSockClient::SetMsgInfo(CWnd* pWnd, UINT umConnect, UINT umClose, UINT umReceive, UINT umConnectFail)
{
	if ( pWnd != NULL)
	{
		m_pOwner			= pWnd;
		m_umConnect		= umConnect;
		m_umClose		= umClose;
		m_umReceive		= umReceive;
		m_umConnectFail= umConnectFail;											// 연결 재시도 요청
	}
}


// 패킷을 구분하는 구분 단위 지정
//
// UINT nMode : 패킷을 구분하는 방법
	//eGxCOMM_PKT_NONE	= 0,		// 수신된 데이터 그대로를 패킷으로 인정한다.
	//eGxCOMM_PKT_COUNT,				// 지정한 데이터 수신 수량으로 패킷의 기준을 삼는다.
	//eGxCOMM_PKT_TOKEN,				// 지정한 데이터를 패킷의 기준으로 삼는다.
	//eGxCOMM_PKT_SIZE_A,				// 가변길이, 선두 길이 정보가 ASCII 형식인 패킷   '0010' -> 길이 10 Bytes
	//eGxCOMM_PKT_SIZE_B,				// 가변길이, 선두 길이 정보가 Binary 데이터인 패킷 0x0000000a -> 길이 10 Bytes
//
// CByteArray* parrToken : eGxCOMM_PKT_TOKEN 모든인 경우 토큰 문자들 배열 포인터
// int nIndex  = 기준 위치
//			eGxCOMM_PKT_TOKEN : BCC/CRC가 토큰 문자 기준으로 어느 위치에 존재하는가 ?
//								0 : BCC/CRC가 존재하지 않는다.								
//								음수 : 토큰 문자 이전에 값이 온다.
//								양수 : 토큰 문자 이후에 값이 온다.
//			eGxCOMM_PKT_SIZE_A, eGxCOMM_PKT_SIZE_A : 길이 정보의 시작 위치
//
// UINT nLen  = 정보 길이
//			eGxCOMM_PKT_COUNT : 패킷으로 인정할 데이터의 길이
//			eGxCOMM_PKT_TOKEN : BCC/CRC 의 길이
//			eGxCOMM_PKT_SIZE_A, eGxCOMM_PKT_SIZE_A : 길이 정보의 크기
//
void CGxSockClient::SetPacketInfo(UINT nMode, CByteArray* parrToken, int nIndex, UINT nLen)
{
	m_RcvQueue.SetPacketInfo(nMode, parrToken, nIndex, nLen);
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
void CGxSockClient::SetCallbackFunction(CGxCallback* pConnect, CGxCallback* pClose, CGxCallback* pReceive, CGxCallback* pConnectFail)			
{
	m_pFncConnect		= pConnect;									// Connect 이벤트 발생시 호출하는 Callback함수
	m_pFncClose			= pClose;									// Close 이벤트 발생시 호출
	m_pFncReceive		= pReceive;									// 수신 이벤트 발생시 호출하는 Callback함수
	m_pFncConnectFail	= pConnectFail;							// 연결시도가 실패하였을때 호출하는 Callback함수
}



// 주소/포트 지정
void CGxSockClient::SetAddress(CString sAddr, int nPort)
{
	m_sIP		= sAddr;
	m_nPort	= (UINT)nPort;
}


// 지정하는 주소로 접속 시도
BOOL CGxSockClient::Connect(CString sAddr, int nPort)
{
	m_sIP		= sAddr;
	m_nPort	= (UINT)nPort;

	return Connect();
}



// 지정된 주소로 접속을 시도한다.
BOOL CGxSockClient::Connect()
{
	BOOL bRet = FALSE;

	try
	{
		// 이미 접속되어있다면 기존 접속은 종료한다.
		if ( m_bConnect == TRUE )
		{
			Close();
			Sleep(50);					// 약간의 시간 지연 후
		}

		bRet = CAsyncSocket::Create();
		if ( bRet == TRUE )
		{
			bRet = CAsyncSocket::Connect(m_sIP, m_nPort);
		}
	}
	catch( int nExp )
	{
		HandleException(nExp);
		// return FALSE;
	}

	return bRet;
}


// 접속을 끊어준다.
void CGxSockClient::Close() 
{
	try
	{
		Clear();							// 수신 버퍼 클리어

		// 접속을 종료한다.
		CAsyncSocket::ShutDown();
		CAsyncSocket::Close();

		// 테스트 후 계속 사용할것인지 여부 판단 ?
		CAsyncSocket::OnClose( 0 );
	}
	catch( int nExp )
	{
		HandleException(nExp);
		// return;
	}

	m_bConnect = FALSE;					// 종료 되었음으로 표시
}


// 수신 버퍼 Clear
void CGxSockClient::Clear()
{
	m_RcvQueue.Clear();
}

// 수신된 데이터중에 패킷을 나누는 기준에 부합한가 ?
UINT CGxSockClient::IsPacket()
{
	return m_RcvQueue.IsPacket();
}

// 수신된 데이터 길이를 조회한다 == Ispacket();
UINT CGxSockClient::GetReceiveSize()
{
	return m_RcvQueue.GetCount();			// 수신된 데이터 길이
}



// 수신 버퍼에서 지정된 기준의 패킷을 읽어낸다.
UINT CGxSockClient::GetPacket(BYTE* pbyData)
{
	return m_RcvQueue.GetPacket(pbyData);
}


// 지정된 길이만큼 패킷을 읽어온다.
UINT CGxSockClient::GetPacket(BYTE* pbyData, int nLen)
{
	return m_RcvQueue.GetPacket(pbyData, (UINT)nLen);
}



// 수신된 모든 내용을 CString 형으로 받아온다. MBCS -> Unicode
CString CGxSockClient::GetReceiveString()
{
	return GetReceiveString( m_RcvQueue.GetCount() );
}



// 지정된 길이만큼 CString 형으로 받아온다. MBCS -> Unicode
CString CGxSockClient::GetReceiveString(int nLen)
{
	CString sRcvData = _T("");
	if ( nLen <= 0 ) return sRcvData;					// 길이 지정 오류
	if ( nLen > m_RcvQueue.GetCount() ) nLen = m_RcvQueue.GetCount();			// 수신된 데이터 길이보다 크다면 길이 축소

	BYTE* byBuff = new BYTE[nLen + 1];						// 문자열의 끝을 널로 마무리하기 위해 1 Byte 더 잡는다.
	if ( byBuff )
	{
		int nReadLen = GetPacket( byBuff, nLen );			// 지정된 길이만큼을 수신 버퍼에서 데이터를 받아온다.
		byBuff[nLen] = '\0';										// 마지막은 널로 마무리해준다.
	
		if ( nReadLen > 0 )										// 정상적으로 데이터를 읽어냈다.
		{

#if defined(UNICODE) || defined(_UNICODE)			// Unicode 개발 환경에서 MBCS로 저장하는 경우
			sRcvData.GetBuffer( nReadLen );					// 지정된 길이만큼 버퍼를 확보한다.

			// MBCS -> Unicode로 변환한다.
			::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (char *)byBuff, -1, sRcvData.GetBuffer(), nReadLen);

			sRcvData.ReleaseBuffer();							// 버퍼 최적화
#else
			sRcvData.Format("%s", byBuff);
#endif

		}

		delete[] byBuff;
	}// of if byBuff

	return sRcvData;
}


// 수신된 패킷을  CString 형으로 받아온다. MBCS -> Unicode
CString CGxSockClient::GetPacketString()
{
	CString sRcvData = _T("");
	int nLen = m_RcvQueue.IsPacket();					// 패킷 수신여부 및 길이

	if ( nLen <= 0 ) return sRcvData;					// 수신된 패킷이 없다.
	if ( nLen > m_RcvQueue.GetCount() ) nLen = m_RcvQueue.GetCount();			// 수신된 데이터 길이보다 크다면 길이 축소

	BYTE* byBuff = new BYTE[nLen + 1];						// 문자열의 끝을 널로 마무리하기 위해 1 Byte 더 잡는다.
	if ( byBuff )
	{
		int nReadLen = GetPacket( byBuff, nLen );			// 지정된 길이만큼을 수신 버퍼에서 데이터를 받아온다.
		byBuff[nLen] = '\0';										// 마지막은 널로 마무리해준다.
	
		if ( nReadLen > 0 )										// 정사적으로 데이터를 읽어냈다.
		{
#if defined(UNICODE) || defined(_UNICODE)			// Unicode 개발 환경에서 MBCS로 저장하는 경우
			sRcvData.GetBuffer( nReadLen );					// 지정된 길이만큼 버퍼를 확보한다.

			// MBCS -> Unicode로 변환한다.
			::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (char *)byBuff, -1, sRcvData.GetBuffer(), nReadLen);

			sRcvData.ReleaseBuffer();							// 버퍼 최적화
#else
			sRcvData.Format("%s", byBuff);
#endif
		}

		delete[] byBuff;
	}// of if byBuff

	return sRcvData;
}


// 지정된 문자열을 전송한다.
// Unicode -> MBCS로 전환되어 전송된다.
//
// return : 전송된 Byte 수, 이 길이는 CString의 GetLength()의 길이하고는 다를 수 있다.
//      0 보다 큰 값이면 전송 성공으로 간주
//
int CGxSockClient::SendString(CString sSendData)
{
	int nRet = 0;
	//? if ( !m_bConnect ) return nRet;							// 연결이 되어있지 않다면 전송할 수 없다.
	if ( sSendData.IsEmpty() ) return nRet;					// 전송 할 데이터가 없다.

#if defined(UNICODE) || defined(_UNICODE)			// Unicode 개발 환경에서 MBCS로 저장하는 경우

	// Unicode -> MBCS 변환을 해준다. CP_UTF8, CP_ACP
	int nLen = WideCharToMultiByte(CP_ACP, 0, sSendData.GetBuffer(), -1, NULL, 0, NULL, NULL);		// 데이터 길이 조사
	char *pszData = new char[ nLen ];														// 메모리 할당 (개행문자용 +1)

	if ( pszData )			// 메모리 할당 정상 확인
	{
		::WideCharToMultiByte(CP_ACP, 0, sSendData.GetBuffer(), -1, pszData, nLen, 0, 0);			// wchar* -> char* 변환

		if ( nLen > 1 )			// 전송 할 데이터가 NULL만 존재하는 것이 아니라면,
		{
			// 문자열로 보내기때문에 자동으로 널문자가 붙는 문제가 발생하여 1 Byte씩 더 전송이 된다.
			nLen--;			// 전송시에는 마지막 널('\0') 문자를 제외하고 전송한다. 
		}

		nRet = CAsyncSocket::Send(pszData, nLen);			// 전송한다.
		delete pszData;
	}
#else

	nRet = CAsyncSocket::Send(sSendData.GetBuffer(0), sSendData.GetLength());			// 전송한다.

#endif

	return nRet;
}



// Server와의 접속이 되었다.
void CGxSockClient::OnConnect(int nErrorCode)
{
	try
	{
		// 오류가 발생한 경우
		if( nErrorCode != 0 )
		{
			m_bConnect = FALSE;		// 연결 실패

			switch(nErrorCode)
			{
				case WSAEADDRINUSE: // WSAEADDRINUSE 지정한 주소가 이미 사용 중입니다.  
					TRACE(_T("The specified address is already in use.\n"));
					break;
				case WSAEADDRNOTAVAIL: // WSAEADDRNOTAVAIL 지정 된 주소는 로컬 컴퓨터에서 사용할 수 없습니다.  
					TRACE(_T("The specified address is not available from the local machine.\n"));
					break;
				case WSAEAFNOSUPPORT: // WSAEAFNOSUPPORT 주소에 지정 된 제품군이이 소켓을 사용할 수 없습니다.  
					TRACE(_T("Addresses in the specified family cannot be used with this socket.\n"));
					break;
				case WSAECONNREFUSED: // WSAECONNREFUSED 연결 시도가 강제로 거부 되었습니다.  
	             TRACE(_T("The attempt to connect was forcefully rejected.\n"));
				  break;
				case WSAEDESTADDRREQ: // WSAEDESTADDRREQ 대상 주소가 필요합니다.
					TRACE(_T("A destination address is required.\n"));
					break;
				case WSAEFAULT:		// WSAEFAULT 의 lpSockAddrLen 인수가 잘못 되었습니다.  
					TRACE(_T("The lpSockAddrLen argument is incorrect.\n"));
					break;
				case WSAEINVAL:		// WSAEINVAL 소켓 주소가 이미 바인딩되어 있습니다.  
					TRACE(_T("The socket is already bound to an address.\n"));
					break;
				case WSAEISCONN:		// WSAEISCONN 소켓이 이미 연결 되어 있습니다.  
					TRACE(_T("The socket is already connected.\n"));
					break;
				case WSAEMFILE:		// WSAEMFILE 더이상 파일 디스크립터를 이용할 수 없다.
					TRACE(_T("No more file descriptors are available.\n"));
					break;
				case WSAENETUNREACH:	// WSAENETUNREACH 네트워크에서이 호스트에 연결할 수 없습니다.  
					TRACE(_T("The network cannot be reached from this host at this time.\n"));
					break;
				case WSAENOBUFS:		// WSAENOBUFS버퍼 공간이 없습니다 사용할 수 있습니다.  소켓을 연결할 수 없습니다. 
					TRACE(_T("No buffer space is available. The socket cannot be connected.\n"));
					break;
				case WSAENOTCONN:		// WSAENOTCONN 소켓이 연결 되어 있지 않습니다.  
					TRACE(_T("The socket is not connected.\n"));
					break;
				case WSAENOTSOCK:		// WSAENOTSOCK 디스크립터는 소켓이 아니라 파일이다.
					TRACE(_T("The descriptor is a file, not a socket.\n"));
					break;
				case WSAETIMEDOUT:	// WSAETIMEDOUT 에 대 한 연결을 설정 하지 않고 연결 시도가 시간 초과. 
					 TRACE(_T("The attempt to connect timed out without establishing a connection. \n"));
					break;

			}//of switch

			// Event Handler가 지정되었다면 Message를 보낸다.
			if( m_pOwner && m_umConnectFail )
			{
				m_pOwner->PostMessage(m_umConnectFail, WPARAM(this), LPARAM(m_nID));	// 연결시도가 실패하였음을 메세지로 보낸다.
			}

			// 연결 실패시 실행할 Callback 함수가 지정되었다면 호출한다.
			if ( m_pFncConnectFail )
			{
				m_pFncConnectFail->Call( this );						
			}

			return CAsyncSocket::OnConnect(nErrorCode);
		}//of if error


		// 정상적으로 연결이 되었다.
		m_bConnect = TRUE;
		// m_debug.Write(_T("Connected(ip:%s, port:%d)"), m_sIP, m_nPort);

		// Event Handler가 지정되었다면 Message를 보낸다.
		if( m_pOwner && m_umConnect )
		{
			m_pOwner->PostMessage(m_umConnect, WPARAM(this), LPARAM(m_nID));			// 연결되었음을 메세지로 보낸다.
		}

		// Event발생시 호출할 Callback 함수가 지정되어있을 때 해당 함수 호출
		if ( m_pFncConnect )
		{
			m_pFncConnect->Call( this );						// Callback 함수 호출
		}

		CAsyncSocket::OnConnect(nErrorCode);
	}
	catch( int nExp )
	{
		HandleException(nExp);
		return;
	}
}


// 접속을 종료한다.
// CAsyncSocket의  OnClose()를 재정의 하여 처리해준다.
void CGxSockClient::OnClose(int nErrorCode)
{
	try
	{
		//위치 변경		Close();

		// Event Handler가 지정되었다면 OnClose Message를 보낸다.
		if( m_pOwner && m_umClose )
		{
			m_pOwner->PostMessage(m_umClose, WPARAM(this), LPARAM(m_nID));			// 연결이 끊어졌음을 메세지로 보낸다.
		}

		// Event발생시 호출할 Callback 함수가 지정되어있을 때 해당 함수 호출
		if ( m_pFncClose )
		{
			m_pFncClose->Call( this );						// Callback 함수 호출
		}

		Close();

		// 서버에 속한 Client 일때만 서버에 등록된 지금 CLient를 종료 처리하도록 한다.
		if( m_pServer != NULL )
		{
			m_pServer->OnCloseClient(this);					// 서버에서 Client 관리리스트에서의 삭제를 지시한다.
		}
		// 이미 위의 Close(); 함수에서 OnClose를 호출하였다.
		// else
		//	CAsyncSocket::OnClose(nErrorCode);
	}
	catch( int nExp )
	{
		HandleException(nExp);
	}

}

// 데이터를 수신하였을 때 처리
void CGxSockClient::OnReceive(int nErrorCode)
{
	if (  nErrorCode != 0 )	return;					// 오류가 있는 상태에서는 데이터를 읽어올 수 없다.

   //Receive를 분할 수신하면, 랜덤하게 100000번 내외에서 못받음 현상발생 !
   DWORD dwReadLen = 0;
   IOCtl(FIONREAD, &dwReadLen);						//패킷 길이를 먼저 파악하고

   if ( dwReadLen <= 0 ) return ;					// 수신받을 내용이 없다.

   BYTE* byRecv = new BYTE[ dwReadLen ];			// 수신 받을 길이만큼 메모리 할당
   int nLength = 0;										// 실제로 수신된 길이

	try
	{
		// 데이터를 수신한다.
		nLength = CAsyncSocket::Receive(byRecv, dwReadLen);
	}
	catch( int nExp )
	{
		HandleException(nExp);

		delete[] byRecv;										// 할당된 메모리 해제
		return;
	}

	// 수신된 데이터가 존재한다면
	if ( nLength > 0 )
	{
		m_RcvQueue.PutData(byRecv, nLength);						// Queue Buffer에 넣는다.
	}

	if( m_pOwner && m_umReceive )										// 수신 이벤트를 할당하였다면 
	{
		m_pOwner->PostMessage(m_umReceive, WPARAM(this), LPARAM(nLength));
	}

	// 수신처리 Call back 함수가 존재한다면
	if( m_pFncReceive )
	{
		m_pFncReceive->Call( this );									// 해당 수신 처리 함수를 호출한다.
	}

	delete[] byRecv;										// 할당된 메모리 해제

	CAsyncSocket::OnReceive(nErrorCode);
}

