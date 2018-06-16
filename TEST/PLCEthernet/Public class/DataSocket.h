//////////////////////////////////////////////////////////////////////////
// CDataSocket & CClientSocket
// CDataSocket�� Server�� Client ������ Class
// CClientSocket�� Server�ν� �����ϴ� CDataSocket�� �����ϴ� Client Class
// Server�ν��� CDataSocket�� 1���� Client�� ��� �����ϴ�
// CClientSocket�� �ܺο��� ���� ������ ���� ���� CDataSocket�� �˾Ƽ� �����Ѵ�.
// �������� [4/4/2009 OSC]
// CClient ��ü�� ���� ��ü�� ��ȯ
//////////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_PACKET_BUFFER 4096
#define WM_PACKET_RECEIVE	WM_USER+10

/////////////////////////////////////////////////////////////////////////////
// CDataSocket command target
class CClientSocket;

class CDataSocket : public CSocket
{
public:
	CDataSocket();
	virtual ~CDataSocket();

private:
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
	BOOL SendClient(const void* lpBuf, int nBufLen);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Client��
public:
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
