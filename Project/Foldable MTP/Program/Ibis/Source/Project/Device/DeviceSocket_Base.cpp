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
		// ����� Name �� ��ŭ ����Ǿ� ������ TRUE
		return (m_Socket.GetClientNameCount() == m_Socket.GetClientCount()) ? TRUE:FALSE;
	}
}
