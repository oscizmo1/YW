#pragma once
#include "DeviceSocket_Base.h"
#include "SocketDefine_CIM.h"
#include "IDReader\GxIDReaderWrapper.h"


//kjpark 20170831 MCR ���� ä�� �� ���� �Ϸ�
#define MCR_CAMERA_IP_CH1		 _T("192.168.10.150")
//#define MCR_CAMERA_IP_CH1		 _T("127.0.0.1")
#define MCR_CAMERA_IP_CH2		 _T("192.168.10.151")
#define MCR_CAMERA_IP_CH3		 _T("192.168.10.152")
#define MCR_CAMERA_IP_CH4		 _T("192.168.10.153")
#define MCR_CAMERA_IP_CH5		 _T("192.168.10.154")
#define MCR_CAMERA_IP_CH6		 _T("192.168.10.155")

#define MCR_CAMERA_PORT			23

#define SOCKET_MCR_1	_T("MCR_1")		// Shuttle #1 Ch1
#define SOCKET_MCR_2	_T("MCR_2")		// Shuttle #1 Ch2
#define SOCKET_MCR_3	_T("MCR_3")		// Shuttle #2 Ch1
#define SOCKET_MCR_4	_T("MCR_4")		// Shuttle #2 Ch2
#define SOCKET_MCR_5	_T("MCR_5")		// Shuttle #3 Ch1
#define SOCKET_MCR_6	_T("MCR_6")		// Shuttle #3 Ch2


class CDeviceSocket_MCR : 	public CDeviceSocket_Base
{
public:
	CDeviceSocket_MCR(void);
	~CDeviceSocket_MCR(void);

	//kjpark 20170831 MCR ���� ä�� �� ���� �Ϸ�
	void CreateSocket(JIG_ID jig, JIG_CH ch);
	void ConnectSocket();
	void CloseSocket();
	void PreparseMessage(CDataSocket* pSocket = NULL);	// �����ڵ� ��ȯ �� StartChar ���� Ȯ���Ͽ� ParseMessage ȣ��

	BOOL ParseMCRMessage(CString strReadingText);				// ���� Parsing


	CGxIDReaderWrapper *m_pIDReader;				// Cognex ID Reader
	//kjpark 20170831 MCR ���� ä�� �� ���� �Ϸ�
	SOCKET_TYPE m_socketType;
	JIG_ID m_jig;
	JIG_CH m_jigCH;
	void SetMCRDisconnectSDK();
	BOOL SetMCRConnectSDK(SOCKET_TYPE socketType);
};
