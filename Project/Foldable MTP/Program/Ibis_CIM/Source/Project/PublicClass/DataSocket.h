//////////////////////////////////////////////////////////////////////////
// CDataSocket & CClientSocket
// CDataSocket�� Server�� Client ������ Class
// CClientSocket�� Server�ν� �����ϴ� CDataSocket�� �����ϴ� Client Class
// Server�ν��� CDataSocket�� 1���� Client�� ��� �����ϴ�
// CClientSocket�� �ܺο��� ���� ������ ���� ���� CDataSocket�� �˾Ƽ� �����Ѵ�.
//
// �������� [31/5/2009 OSC]
// CSocket�� CAyncSocket���� ����
//////////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_PACKET_BUFFER 4096
#define WM_PACKET_RECEIVE	WM_USER+500
#define WM_RE_CONNECT		WM_USER+501	// �ڵ����� �翬�� �õ�(Client ����)
#define WM_SOCKET_CLOSE		WM_USER+502	// ������ ������ ��
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
	// Server��
public:
	virtual void OnAccept(int nErrorCode);
	BOOL SendClient(CString strSocketName, const void* lpBuf, int nBufLen);
	void AddClient(CClientSocket* pClient);
	CClientSocket* GetClientAt(CString socketname);
	CList <ClientSocketName, ClientSocketName> listClientName;
	void CDataSocket::DeleteClient();
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Client��
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
