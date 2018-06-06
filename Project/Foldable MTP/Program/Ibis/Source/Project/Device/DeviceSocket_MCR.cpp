#include "StdAfx.h"
#include "DeviceSocket_MCR.h"
#include "UI\Form\FormSignal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CDeviceSocket_MCR::CDeviceSocket_MCR(void)
	:CDeviceSocket_Base()
{
	m_pIDReader = NULL;
	//kjpark 20170831 MCR 구현 채널 별 구현 완료
	m_socketType = eMAX_COMMCount;
	m_jig = JIG_ID_MAX;
	m_jigCH = JIG_CH_MAX;
}


CDeviceSocket_MCR::~CDeviceSocket_MCR(void)
{
}

//kjpark 20170831 MCR 구현 채널 별 구현 완료
void CDeviceSocket_MCR::CreateSocket(JIG_ID jig, JIG_CH ch)
{
	//kjpark 20170919 MCR 재연결 
	m_Socket.Close();

	m_Socket.SetServer(FALSE);

	//kjpark 20170901 소켓 핸들러 셋팅
	if(m_Socket.Create())
	{
		m_Socket.SetReciveHwnd(this->m_hParent);
	}

	switch(jig)
	{
	case JIG_ID_A:
		{
			switch(ch)
			{
			case JIG_CH_1:
				m_Socket.SetSocketName(SOCKET_MCR_1);
				m_Socket.SetIP(MCR_CAMERA_IP_CH1);
				break;
			}
		}
		break;
	case JIG_ID_B:
		{
			switch(ch)
			{
			case JIG_CH_1:
				m_Socket.SetSocketName(SOCKET_MCR_3);
				m_Socket.SetIP(MCR_CAMERA_IP_CH3);
				break;
			}
		}
		break;
	}
	
	m_Socket.SetPort(MCR_CAMERA_PORT);
	m_Socket.ConnectToServer();

	m_jig = jig;
	m_jigCH = ch;
}

void CDeviceSocket_MCR::ConnectSocket()
{
	m_Socket.ConnectToServer();
}

//kjpark 20170831 MCR 구현 채널 별 구현 완료
void CDeviceSocket_MCR::CloseSocket()
{
	m_Socket.OnClose(0);
}

void CDeviceSocket_MCR::PreparseMessage( CDataSocket* pSocket /*= NULL*/ )
{
	int nMsgLen = pSocket->GetLastLength();

	wchar_t wszMsg[4096];

	if (nMsgLen > 4095) 
	{
		nMsgLen = 4095;
	}

	CEtc::ConvertUnicode(pSocket->GetLastMsg(), nMsgLen, wszMsg, 4096);
	wszMsg[nMsgLen] = NULL;

	CString strMsg;
	strMsg.Format(_T("%s"), wszMsg);
	//나중에 strMsg = strMsg.SpanExcluding(_T("\r\n"));			// CR\LF 제거

	if(nMsgLen > 0)
	{
		//TRACE(_T("<- %s\t%s\n"), pSocket->GetSocketName(), strMsg);

		ParseMCRMessage(strMsg);
	}
}

BOOL CDeviceSocket_MCR::ParseMCRMessage(CString strReadingText )
{
	//kjpark 20170831 MCR 구현 채널 별 구현 완료
	theProcBank.m_strLastCellID[m_jig][m_jigCH] = strReadingText.SpanExcluding(_T("\n\r"));
	theLog[LOG_SOCKET].AddBuf(_T("%cJIG CH%d MCR Read : %s"), m_jig+_T('A'), m_jigCH+1, theProcBank.m_strLastCellID[m_jig][m_jigCH]);
	return TRUE;
}


void CDeviceSocket_MCR::SetMCRDisconnectSDK()
{
	//kjpark 20170919 MCR 재연결시 죽는 버그 수정 MCR SDK 변경으로 인한 버그
	if ( m_pIDReader->GetIDReader() != NULL && m_pIDReader != NULL)
	{
		delete m_pIDReader;
		m_pIDReader =NULL;
	}
}

//kjpark 20170831 MCR 구현 채널 별 구현 완료
BOOL CDeviceSocket_MCR::SetMCRConnectSDK(SOCKET_TYPE socketType)
{
	m_socketType = socketType;
	//kjpark 20161025 MCR 구현
	m_pIDReader = new CGxIDReaderWrapper();				// 생성
	if ( m_pIDReader )
	{
		switch(socketType)
		{
		case eCOMM_MCR_1:
			m_pIDReader->SetEventHandle(GetMainWnd(), WM_UPDATE_MCRCONNECT, WM_UPDATE_MCRDATA_CH1, WM_UPDATE_MCRLIVE_CH1, socketType);
			m_pIDReader->SetAddress(MCR_CAMERA_IP_CH1);				// IP 주소			
			m_pIDReader->SetLiveImageName( MCRPath_CH1);
			break;
		case eCOMM_MCR_3:
			m_pIDReader->SetEventHandle(GetMainWnd(), WM_UPDATE_MCRCONNECT, WM_UPDATE_MCRDATA_CH2, WM_UPDATE_MCRLIVE_CH2, socketType);
			m_pIDReader->SetAddress(MCR_CAMERA_IP_CH3);				// IP 주소			
			m_pIDReader->SetLiveImageName( MCRPath_CH3);
			break;
		}		
		
		return TRUE;
	}
	return FALSE;
}
