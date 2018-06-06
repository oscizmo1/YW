#include "StdAfx.h"
#include "DeviceSocket_Base.h"


CDeviceSocket_Base::CDeviceSocket_Base(void)
{
	m_hParent = NULL;
}


CDeviceSocket_Base::~CDeviceSocket_Base(void)
{
}

void CDeviceSocket_Base::SetParentHWnd( HWND hWnd )
{
	m_hParent = hWnd;
}

BOOL CDeviceSocket_Base::IsConnected()
{
	if(m_Socket.IsServer() == FALSE)
	{
		return m_Socket.IsConnected();
	}
	else
	{
		// 등록한 Name 수 만큼 연결되어 있으면 TRUE
		return (m_Socket.GetClientNameCount() == m_Socket.GetClientCount()) ? TRUE:FALSE;
	}
}
