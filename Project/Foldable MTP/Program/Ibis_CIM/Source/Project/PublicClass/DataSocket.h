//////////////////////////////////////////////////////////////////////////
// CDataSocket & CClientSocket
// CDataSocket은 Server와 Client 통합형 Class
// CClientSocket은 Server로써 동작하는 CDataSocket가 관리하는 Client Class
// Server로써의 CDataSocket은 1개의 Client만 허용 가능하다
// CClientSocket은 외부에서 따로 선언할 이유 없이 CDataSocket이 알아서 관리한다.
//
// 최종수정 [31/5/2009 OSC]
// CSocket을 CAyncSocket으로 변경
//////////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_PACKET_BUFFER 4096
#define WM_PACKET_RECEIVE	WM_USER+500
#define WM_RE_CONNECT		WM_USER+501	// 자동으로 재연결 시도(Client 전용)
#define WM_SOCKET_CLOSE		WM_USER+502	// 소켓이 끊겼을 때
#define WM_SOCKET_CONNECT	WM_USER+503	

/////////////////////////////////////////////////////////////////////////////
// CDataSocket command target
class CClientSocket;

typedef struct _ClientSocketName 
{
	CString IP;
	CString Name;
} ClientSocketName;

class CDataSocket : public CAsyncSocket
{
public:
	CDataSocket();
	virtual ~CDataSocket();

protected:
	long m_PortNo;
	CString m_strName;
	CString m_strIP;

	BOOL m_fSocketCreated;
	BOOL m_fSocketConnected;
	BOOL m_bIsServer;

	HWND m_hRcvWnd;

public:
	char m_lastMessage[MAX_PACKET_BUFFER];
	long m_Length;
	long m_nRemainCount;
	char m_SndBuff[MAX_PACKET_BUFFER];

public:
	void SetReciveHwnd(HWND hWnd) { m_hRcvWnd = hWnd; }
	HWND GetReciveHwnd() { return m_hRcvWnd; }
	void SetPort(long portNo) { m_PortNo = portNo; }
	long GetPort() { return m_PortNo; }
	void SetSocketName(CString strName) { m_strName = strName; }
	CString GetSocketName() { return m_strName; }
	void SetIP(CString ip) { m_strIP = ip; }
	CString GetIP() { return m_strIP; }
	char* GetLastMsg() { return m_lastMessage; }
	int GetLastLength() { return m_Length; }

	void SetCreated(BOOL fCreated) { m_fSocketCreated = fCreated; }
	BOOL IsCreated() {	return m_fSocketCreated; }
	void SetConnected(BOOL fConnected) { m_fSocketConnected = fConnected; }
	BOOL IsConnected() { return m_fSocketConnected; }
	void SetServer(BOOL bIsServer) { m_bIsServer = bIsServer; }
	BOOL IsServer() {return m_bIsServer; }
	
	//////////////////////////////////////////////////////////////////////////
	// Server용
public:
	virtual void OnAccept(int nErrorCode);
	BOOL SendClient(CString strSocketName, const void* lpBuf, int nBufLen);
	void AddClient(CClientSocket* pClient);
	CClientSocket* GetClientAt(CString socketname);
	CList <ClientSocketName, ClientSocketName> listClientName;
	void CDataSocket::DeleteClient();
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Client용
public:
	BOOL ConnectToServer();
	virtual void OnConnect(int nErrorCode);
	//////////////////////////////////////////////////////////////////////////

public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};

class CClientSocket	:	public CDataSocket
{
public:
	CClientSocket();
	virtual ~CClientSocket();

private:
	CDataSocket* m_pServerSocket;

public:
	void SetServerSocket(CDataSocket* pServerSocket) { m_pServerSocket = pServerSocket; }
	
	void WaitDataOrEvent(CSocket* sc, void* pBuff, DWORD dwGoal, HANDLE hEvent);

public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);


};
