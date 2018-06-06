#include "StdAfx.h"
#include "DeviceSocket_ActiveAlign.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDeviceSocket_ActiveAlign::CDeviceSocket_ActiveAlign(void)
	:CDeviceSocket_Base()
{
}
CDeviceSocket_ActiveAlign::~CDeviceSocket_ActiveAlign(void)
{
}

void CDeviceSocket_ActiveAlign::CreateSocket()
{
	if(m_Socket.IsCreated())
		m_Socket.Close();

	m_Socket.SetServer(TRUE);				// 서버로 설정한다.

	m_Socket.SetSocketName(SOCKET_ACTIVEALIGN_PC);
	m_Socket.SetIP(ALIGN_PC_IP);
	m_Socket.SetPort(ALIGN_PC_ACTIVE_PORT);

	ClientSocketName socketName;
	socketName.Name = SOCKET_ACTIVEALIGN_PC;
	socketName.IP = ALIGN_PC_IP;
	socketName.Port = ALIGN_PC_ACTIVE_PORT;

	m_Socket.AddClientName( socketName );
	m_Socket.SetReciveHwnd(m_hParent);

	if(m_Socket.Create(m_Socket.GetPort()))
	{
		m_Socket.Listen();											// 수신을 시작한다.
	}
}

void CDeviceSocket_ActiveAlign:: PreparseMessage(CDataSocket* pSocket)
{
	int nMsgLen = pSocket->GetLastLength();

	wchar_t wszMsg[4096];
	if (nMsgLen > 4095) 
		nMsgLen = 4095;

	CEtc::ConvertUnicode(pSocket->GetLastMsg(), nMsgLen, wszMsg, 4096);
	wszMsg[nMsgLen] = NULL;

	CString strMsg;
	strMsg.Format(_T("%s"), wszMsg);

	ParseActiveAlignMessage(strMsg);


}
CString CDeviceSocket_ActiveAlign::ParseActiveAlignMessage(CString strReadingText)
{
	return 0;
}
