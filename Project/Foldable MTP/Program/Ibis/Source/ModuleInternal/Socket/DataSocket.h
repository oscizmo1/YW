//////////////////////////////////////////////////////////////////////////
// CDataSocket & CClientSocket
// CDataSocket�� Server�� Client ������ Class
// CClientSocket�� Server�ν� �����ϴ� CDataSocket�� �����ϴ� Client Class
// �ܺο��� ������ Message�� �̷������
// 

/* **************************************************************************
// Server ����
CDataSocket m_ServerSocket;
m_ServerSocket.SetReciveHwnd(this->GetSafeHwnd());
m_ServerSocket.SetSocketName(_T("SERVER_NAME"));
m_ServerSocket.SetServer(TRUE);
m_ServerSocket.SetPort(7000);

// AddClientName()�� ���� ����� IP�� listClientName ����Ʈ�� ����Ѵ�.
// Accept�� listClientName ����Ʈ�� ��ϵ� �͸� ����ȴ�.
ClientSocketName socketName;
socketName.Name = _T("CLIENT_NAME1");
socketName.IP = _T("192.168.10.101);
m_ServerSocket.AddClientName(socketName);
socketName.Name = _T("CLIENT_NAME2");
socketName.IP = _T("192.168.10.101);
m_ServerSocket.AddClientName(socketName);

if(m_ServerSocket.Create(m_ServerSocket.GetPort()))
{
m_ServerSocket.Listen();
}

 ************************************************************************** */

/* **************************************************************************
// Client ����
CDataSocket m_Socket;
m_Socket.SetReciveHwnd(this->GetSafeHwnd());
m_Socket.SetSocketName(_T("NAME"));
m_Socket.SetServer(FALSE);
m_Socket.SetIP(_T("127.0.0.1"));
m_Socket.SetPort(7000);
m_Socket.Create();
m_Socket.ConnectToServer();
// ���� ���� �� WM_RE_CONNECT �޼����� ����ϸ� ��� �翬��õ� �� �� �ִ�
 ************************************************************************** */


// History
// CSocket�� CAyncSocket���� ���� [31/5/2009 OSC]
// �������� ���� [9/8/2015 OSC]
// CientSocket Close�� hWnd �߸� �����°� ���� [3/8/2016 OSC]
// m_strErrorText & WM_SOCKET_ERROR_MSG ���� [7/11/2016 OSC]
// m_fSocketCreated ���� [7/11/2016 OSC]
// ����� ������ [7/11/2016 OSC]
// RemoveClient������ m_fSocketConnected = FALSE; [8/23/2017 OSC]
// GetClientCount() ���� [8/23/2017 OSC]
//////////////////////////////////////////////////////////////////////////
#pragma once

//kjpark 20161110 ���� UI �߰� �۾�
#define MAX_PACKET_BUFFER	4096*2
#define WM_PACKET_RECEIVE	WM_USER+500
#define WM_RE_CONNECT		WM_USER+501	// Connect�� ���� �ʾ� �翬���� �ʿ��� ������ ȣ��ȴ� (Client ����)
#define WM_SOCKET_CLOSE		WM_USER+502	// ������ ������ ��
#define WM_SOCKET_CONNECT	WM_USER+503	
#define UM_INITIAL_DATAUPDATE WM_USER+504 // View ȭ���� �ʱ�ȭ�ϰ� ����� Ȱ��ȭ / ��Ȱ��ȭ �Ѵ�.
/////////////////////////////////////////////////////////////////////////////
// CDataSocket command target
class CClientSocket;

struct ClientSocketName 
{
	CString IP;
	int Port;
	CString Name;
} ;

class CDataSocket : public CAsyncSocket
{
public:
	CDataSocket();
	virtual ~CDataSocket();

protected:
	UINT m_PortNo;
	CString m_strName;
	CString m_strIP;

	BOOL m_fSocketConnected;
	BOOL m_bIsServer;
	BOOL m_bClient;

	HWND m_hRcvWnd;
	CList <ClientSocketName, ClientSocketName> listClientName;
	CList <CClientSocket*, CClientSocket*> listClient;

// public:
	char m_lastMessage[MAX_PACKET_BUFFER];
	long m_Length;

public:
	void InitDataSocket();
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

	BOOL IsCreated() {	return m_hSocket == INVALID_SOCKET ? FALSE:TRUE; }
	BOOL IsConnected() { return m_fSocketConnected; }
	void SetServer(BOOL bIsServer) { m_bIsServer = bIsServer; }
	BOOL IsServer() {return m_bIsServer; }

	//////////////////////////////////////////////////////////////////////////
	// Server ����
public:
	virtual void OnAccept(int nErrorCode);
	BOOL SendClient(CString strSocketName, const void* lpBuf, int nBufLen);
	void AddClientName(ClientSocketName socketName);
	int GetClientNameCount();

	BOOL AddClient(CClientSocket* pClient);
	void RemoveClient(ClientSocketName socketName);
	CClientSocket* GetClientAt(CString socketname);
	int GetClientCount();
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Client ����
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

public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};
